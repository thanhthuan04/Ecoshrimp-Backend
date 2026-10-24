/*
  EcoShrimp - Firmware ESP32 (Hoàn chỉnh + FreeRTOS + Interlock + HiveMQ)
*/

#include <WiFi.h>
#include <WiFiClientSecure.h> // Thay vì WiFiClient thường để kết nối HiveMQ
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ==========================================
// 1. CẤU HÌNH MẠNG & MQTT BROKER (HỢP NHẤT BE)
// ==========================================
const char* WIFI_SSID = "TEN_WIFI_2.4G_CUA_BAN"; 
const char* WIFI_PASSWORD = "MAT_KHAU_WIFI";     

// Cấu hình HiveMQ Cloud của Backend
const char* MQTT_HOST = "xxxxxxxx.s1.eu.hivemq.cloud";
const int   MQTT_PORT = 8883; // Port TLS
const char* MQTT_USERNAME = "dien-username-that";
const char* MQTT_PASSWORD = "dien-password-that";

const char* TOPIC_SENSOR = "pond1/sensors";
const char* TOPIC_ACTUATOR_SUB = "pond1/actuators/#"; // Bắt toàn bộ lệnh điều khiển

// ==========================================
// 2. KHAI BÁO CHÂN (PINOUT)
// ==========================================
#define PIN_PUMP_IN  18
#define PIN_PUMP_OUT 19
#define PIN_LIGHT    25
#define PIN_AERATOR  26

#define PIN_DS18B20     4
#define PIN_WATER_LEVEL 32

#define RELAY_ON  HIGH
#define RELAY_OFF LOW

// ==========================================
// 3. KHỞI TẠO TÀI NGUYÊN (HARDWARE & RTOS)
// ==========================================
WiFiClientSecure espClient; 
PubSubClient client(espClient);

Adafruit_ADS1115 ads;
OneWire oneWire(PIN_DS18B20);
DallasTemperature ds18b20(&oneWire);

struct SensorData {
  float ph;
  float do_level;
  float turbidity;
  float temp;
  int water_level;
} currentSensors;

struct Command {
  int device_pin;
  int state;
};

QueueHandle_t commandQueue;
SemaphoreHandle_t sensorMutex;

// ==========================================
// 4. CÁC HÀM MẠNG (WIFI & MQTT)
// ==========================================
void setup_wifi() {
  Serial.print("Dang ket noi WiFi: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi Connected! IP: " + WiFi.localIP().toString());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Ket noi MQTT Broker... ");
    String clientId = "ESP32_EcoShrimp_" + String(random(0, 1000));
    
    // Kết nối với Username / Password
    if (client.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("Thanh cong!");
      client.subscribe(TOPIC_ACTUATOR_SUB); // Dùng Wildcard (#) thay vì subscribe 4 lần
    } else {
      Serial.printf("That bai, rc=%d, thu lai sau 5s...\n", client.state());
      vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
  }
}

// Hàm Callback xử lý lệnh từ Backend
void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) msg += (char)payload[i];
  String topicStr = String(topic);
  
  Command cmd;
  cmd.state = (msg == "1") ? RELAY_ON : RELAY_OFF;

  // Map topic vào Pin tương ứng
  if (topicStr.endsWith("/aerator")) cmd.device_pin = PIN_AERATOR;
  else if (topicStr.endsWith("/pump_in")) cmd.device_pin = PIN_PUMP_IN;
  else if (topicStr.endsWith("/pump_out")) cmd.device_pin = PIN_PUMP_OUT;
  else if (topicStr.endsWith("/light")) cmd.device_pin = PIN_LIGHT;
  else return; // Không khớp topic nào thì bỏ qua

  // Đẩy lệnh vào Queue cho Task Relay xử lý
  xQueueSend(commandQueue, &cmd, portMAX_DELAY);
}

// ==========================================
// 5. TASK 1: ĐỌC CẢM BIẾN (Core 0)
// ==========================================
void Task_ReadSensors(void *pvParameters) {
  for (;;) {
    // Đọc điện áp qua ADS1115
    float v_ph = ads.computeVolts(ads.readADC_SingleEnded(0));
    float v_do = ads.computeVolts(ads.readADC_SingleEnded(1));
    float v_tur = ads.computeVolts(ads.readADC_SingleEnded(2));

    // Đọc nhiệt độ và mực nước
    ds18b20.requestTemperatures();
    float t_val = ds18b20.getTempCByIndex(0);
    if (t_val <= -100.0) t_val = 0.0;
    
    // Đọc phao (Giả định: LOW = Có nước, HIGH = Cạn nước)
    // Bạn có thể đảo ngược lại tùy phần cứng thực tế
    int w_state = (digitalRead(PIN_WATER_LEVEL) == LOW) ? 1 : 0; 

    // Tính toán giả lập bằng biến trở (Như code cũ của bạn)
    float p_val = min((v_ph / 3.3f) * 14.0f, 14.0f);
    float d_val = min((v_do / 3.3f) * 10.0f, 10.0f);
    float tur_val = min((v_tur / 3.3f) * 100.0f, 100.0f);

    // Ghi vào Struct qua Mutex
    if (xSemaphoreTake(sensorMutex, portMAX_DELAY) == pdTRUE) {
      currentSensors.ph = p_val;
      currentSensors.do_level = d_val;
      currentSensors.turbidity = tur_val;
      currentSensors.temp = t_val;
      currentSensors.water_level = w_state;
      xSemaphoreGive(sensorMutex);
    }
    
    vTaskDelay(2000 / portTICK_PERIOD_MS); 
  }
}

// ==========================================
// 6. TASK 2: ĐIỀU KHIỂN & INTERLOCK (Core 1)
// ==========================================
void Task_ControlRelay(void *pvParameters) {
  Command rxCmd;
  for (;;) {
    if (xQueueReceive(commandQueue, &rxCmd, portMAX_DELAY)) {
      
      // LOGIC BẢO VỆ: Không cho bật bơm xả nếu cạn nước (Phao báo 0)
      if (rxCmd.device_pin == PIN_PUMP_OUT && rxCmd.state == RELAY_ON) {
        // Lấy trạng thái nước an toàn từ Struct
        int current_water_level = 0;
        if (xSemaphoreTake(sensorMutex, portMAX_DELAY) == pdTRUE) {
          current_water_level = currentSensors.water_level;
          xSemaphoreGive(sensorMutex);
        }

        if (current_water_level == 0) { // 0 = Cạn nước theo quy ước của Backend
          Serial.println("INTERLOCK: Nuoc can, huy lenh bat bom xa!");
          continue; // Bỏ qua lệnh bật bơm
        }
      }
      
      digitalWrite(rxCmd.device_pin, rxCmd.state);
      Serial.printf("[RELAY] PIN %d -> State: %d\n", rxCmd.device_pin, rxCmd.state);
    }
  }
}

// ==========================================
// 7. SETUP
// ==========================================
void setup() {
  Serial.begin(115200);

  // Setup chân Relay
  pinMode(PIN_PUMP_IN, OUTPUT);
  pinMode(PIN_PUMP_OUT, OUTPUT);
  pinMode(PIN_LIGHT, OUTPUT);
  pinMode(PIN_AERATOR, OUTPUT);
  pinMode(PIN_WATER_LEVEL, INPUT_PULLUP);

  digitalWrite(PIN_PUMP_IN, RELAY_OFF);
  digitalWrite(PIN_PUMP_OUT, RELAY_OFF);
  digitalWrite(PIN_LIGHT, RELAY_OFF);
  digitalWrite(PIN_AERATOR, RELAY_OFF);

  // Setup I2C & Cảm biến
  Wire.begin(21, 22);
  if (!ads.begin()) Serial.println("LOI: Khong tim thay ADS1115");
  else ads.setGain(GAIN_TWOTHIRDS);
  
  ds18b20.begin();

  // Setup Mạng
  setup_wifi();
  espClient.setInsecure(); // QUAN TRỌNG: Bỏ qua kiểm tra chứng chỉ SSL/TLS
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callback);

  // Setup FreeRTOS
  commandQueue = xQueueCreate(10, sizeof(Command));
  sensorMutex = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(Task_ReadSensors, "Sensors", 4096, NULL, 1, NULL, 0); // Đưa đọc cảm biến về Core 0
  xTaskCreatePinnedToCore(Task_ControlRelay, "Relays", 2048, NULL, 2, NULL, 1);
}

// ==========================================
// 8. VÒNG LẶP CHÍNH (Core 1)
// ==========================================
void loop() {
  if (!client.connected()) reconnect();
  client.loop(); 

  static unsigned long lastMsg = 0;
  if (millis() - lastMsg > 5000) {
    lastMsg = millis();

    StaticJsonDocument<256> doc;
    
    if (xSemaphoreTake(sensorMutex, portMAX_DELAY) == pdTRUE) {
      doc["ph"] = currentSensors.ph;
      doc["do"] = currentSensors.do_level;
      doc["turbidity"] = currentSensors.turbidity;
      doc["temp"] = currentSensors.temp;
      doc["level"] = currentSensors.water_level;
      xSemaphoreGive(sensorMutex);
    }

    char buffer[256];
    serializeJson(doc, buffer);
    client.publish(TOPIC_SENSOR, buffer);
    
    Serial.print("[MQTT] PUBLISH: ");
    Serial.println(buffer);
  }
}