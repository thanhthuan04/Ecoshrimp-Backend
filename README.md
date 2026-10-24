<div align="center">

  <h1>EcoShrimp - Core IoT Telemetry & AI Early Warning Engine</h1>
  <h3>Hệ Thống Backend Giám Sát, Dự Báo AI & Tự Động Hóa Ao Nuôi Tôm Thời Gian Thực</h3>

  <p>
    Nền tảng xử lý dữ liệu viễn trắc IoT trung tâm và tính toán khoa học dữ liệu nuôi trồng thủy sản (Giai đoạn Mô hình Thử nghiệm / Proof of Concept),
    cung cấp kiến trúc RESTful API và WebSocket hiệu năng cao, tiếp nhận luồng dữ liệu cảm biến đa thông số qua giao thức MQTT TLS,
    ứng dụng mô hình học máy XGBoost MultiOutputRegressor dự báo xu hướng biến thiên chất lượng nước và cơ chế đề xuất kích hoạt thiết bị phụ tải thích ứng.
  </p>

  <p>
    <img src="https://img.shields.io/badge/FastAPI-^0.115.8-009688?logo=fastapi&logoColor=white" alt="FastAPI">
    <img src="https://img.shields.io/badge/Python-3.11+-3776AB?logo=python&logoColor=white" alt="Python">
    <img src="https://img.shields.io/badge/MongoDB-Motor_Async-47A248?logo=mongodb&logoColor=white" alt="MongoDB">
    <img src="https://img.shields.io/badge/MQTT-HiveMQ_TLS-FF6600?logo=mqtt&logoColor=white" alt="MQTT">
    <img src="https://img.shields.io/badge/AI-XGBoost_Forecaster-1890FF?logo=scikitlearn&logoColor=white" alt="XGBoost">
    <img src="https://img.shields.io/badge/Pydantic-v2.10.6-E92063?logo=pydantic&logoColor=white" alt="Pydantic">
    <img src="https://img.shields.io/badge/Telegram-Alert_Bot-26A5E4?logo=telegram&logoColor=white" alt="Telegram Bot">
    <img src="https://img.shields.io/badge/status-Prototype_%2F_PoC-orange" alt="Status">
  </p>

</div>

<br />

# BACKEND API SERVICE & IOT TELEMETRY ENGINE

Đây là kho lưu trữ chứa mã nguồn **Backend Core Service** của hệ thống **EcoShrimp**. Dự án được nghiên cứu và phát triển dưới dạng **mô hình thử nghiệm khả thi (Proof of Concept - PoC)** trên nền tảng **FastAPI**, kết nối môi trường viễn trắc ao nuôi thông minh qua broker **MQTT (HiveMQ Cloud)** được mã hóa TLS, cung cấp luồng truyền phát dữ liệu thời gian thực qua **WebSocket**, tích hợp mô hình hồi quy đa biến **XGBoost** dự báo sớm các nguy cơ môi trường và gửi thông báo khẩn cấp tức thì qua **Telegram Bot**.

> [!NOTE]
> **Hiện trạng dự án:** Hệ thống hiện đang ở giai đoạn nguyên mẫu nghiên cứu (Prototype / PoC) phục vụ đồ án kỹ thuật, chưa được triển khai trên các ao/bể nuôi tôm thương phẩm thực tế. Dữ liệu thử nghiệm phần cứng hiện sử dụng mạch vi điều khiển ESP32 với các biến trở/nút vặn vật lý ngoài để mô phỏng tín hiệu cảm biến hoặc thông qua kịch bản phần mềm giả lập `scripts/simulate_sensors.py`. Mô hình AI hiện đóng vai trò cảnh báo sớm và đưa ra khuyến nghị tham khảo (Advisory Mode), chưa thực hiện điều khiển vòng lặp kín tự động cân bằng toàn diện sinh thái ao nuôi trong thực tế.

---

## Mục lục

- [1. Công nghệ & Thư viện Cốt lõi](#1-công-nghệ--thư-viện-cốt-lõi)
  - [Khung Ứng dụng & Máy chủ Bất đồng bộ](#khung-ứng-dụng--máy-chủ-bất-đồng-bộ)
  - [Giao thức IoT & Điều phối Thời gian thực](#giao-thức-iot--điều-phối-thời-gian-thực)
  - [Trí tuệ Nhân tạo & Dự báo Tham số Nước](#trí-tuệ-nhân-tạo--dự-báo-tham-số-nước)
  - [Cơ sở Dữ liệu, Thông báo & Tự động hóa](#cơ-sở-dữ-liệu-thông-báo--tự-động-hóa)
- [2. Kiến trúc Nghiệp vụ & Hiện trạng Thử nghiệm](#2-kiến-trúc-nghiệp-vụ--hiện-trạng-thử-nghiệm)
  - [2.1. Tiếp nhận & Xử lý Dữ liệu Cảm biến MQTT](#21-tiếp-nhận--xử-lý-dữ-liệu-cảm-biến-mqtt)
  - [2.2. Động cơ Dự báo Tham số Môi trường (XGBoost & Exponential Smoothing)](#22-động-cơ-dự-báo-tham-số-môi-trường-xgboost--exponential-smoothing)
  - [2.3. Hệ thống Cảnh báo Sớm Đa tầng & Tích hợp Telegram](#23-hệ-thống-cảnh-báo-sớm-đa-tầng--tích-hợp-telegram)
  - [2.4. Động cơ Điều khiển & Lập lịch Thiết bị Phụ tải](#24-động-cơ-điều-khiển--lập-lịch-thiết-bị-phụ-tải)
  - [2.5. Bảo mật & Xác thực Khóa Truy cập API](#25-bảo-mật--xác-thực-khóa-truy-cập-api)
  - [2.6. Hiện trạng Thử nghiệm Phần cứng & Giới hạn Can thiệp AI](#26-hiện-trạng-thử-nghiệm-phần-cứng--giới-hạn-can-thiệp-ai)
- [3. Danh mục API Endpoints & Giao thức Kết nối](#3-danh-mục-api-endpoints--giao-thức-kết-nối)
- [4. Hướng dẫn Cài đặt, Khởi chạy & Giả lập Dữ liệu](#4-hướng-dẫn-cài-đặt-khởi-chạy--giả-lập-dữ-liệu)
  - [4.1. Cài đặt Môi trường Backend](#41-cài-đặt-môi-trường-backend)
  - [4.2. Cấu hình Biến Môi trường (.env)](#42-cấu-hình-biến-môi-trường-env)
  - [4.3. Khởi chạy Máy chủ API](#43-khởi-chạy-máy-chủ-api)
  - [4.4. Hướng dẫn Sử dụng Kịch bản Giả lập Cảm biến (scripts/simulate_sensors.py)](#44-hướng-dẫn-sử-dụng-kịch-bản-giả-lập-cảm-biến-scriptssimulatesensorspy)
  - [4.5. Kiểm thử Lắng nghe Lệnh Điều khiển Phụ tải (scripts/test_mqtt.py)](#45-kiểm-thử-lắng-nghe-lệnh-điều-khiển-phụ-tải-scriptstestmqttpy)
  - [4.6. Triển khai Bằng Docker](#46-triển-khai-bằng-docker)
- [5. Cấu trúc Thư mục Dự án](#5-cấu-trúc-thư-mục-dự-án)
- [6. Tích hợp Hệ sinh thái & CI/CD](#6-tích-hợp-hệ-sinh-thái--cicd)
- [7. Tác giả](#7-tác-giả)

---

## 1. Công nghệ & Thư viện Cốt lõi

Hệ thống backend được tối ưu hóa với các phiên bản bảo mật và hiện đại nhất trên nền tảng Python 3.11+:

### Khung Ứng dụng & Máy chủ Bất đồng bộ

| Công nghệ / Thư viện | Phiên bản | Vai trò Kỹ thuật |
| :--- | :--- | :--- |
| **[FastAPI](https://fastapi.tiangolo.com/)** | `^0.115.8` | Khung ứng dụng Web ASGI hiệu năng cao, cung cấp RESTful API và tài liệu OpenAPI chuẩn hóa |
| **[Uvicorn](https://www.uvicorn.org/)** | `^0.34.0` | Máy chủ Web ASGI bất đồng bộ tối ưu hóa hiệu năng vòng lặp sự kiện |
| **[Pydantic v2](https://docs.pydantic.dev/)** | `^2.10.6` | Xác thực cấu trúc dữ liệu đầu vào/đầu ra nghiêm ngặt với hiệu năng từ Rust Core |
| **[Pydantic Settings](https://docs.pydantic.dev/latest/concepts/pydantic_settings/)** | `^2.8.0` | Quản lý biến môi trường an toàn từ tệp `.env` và `.env.local` |

---

### Giao thức IoT & Điều phối Thời gian thực

| Công nghệ / Thư viện | Phiên bản | Vai trò Kỹ thuật |
| :--- | :--- | :--- |
| **[Paho-MQTT](https://eclipse.dev/paho/index.php?page=clients/python/index.php)** | `^2.1.0` | Client MQTT v2 hỗ trợ mã hóa TLS/SSL, tự động tái kết nối và duy trì kết nối đám mây HiveMQ |
| **[WebSocket Protocol](https://fastapi.tiangolo.com/advanced/websockets/)** | `Native ASGI` | Truyền phát dữ liệu cảm biến tức thì đến giao diện Dashboard với độ trễ dưới 50ms |
| **[AsyncIO Scheduler](https://docs.python.org/3/library/asyncio.html)** | `Python Standard` | Vòng lặp nền định kỳ 60 giây kiểm tra khung giờ kích hoạt rơ-le thiết bị tự động |

---

### Trí tuệ Nhân tạo & Dự báo Tham số Nước

| Công nghệ / Thư viện | Phiên bản | Vai trò Kỹ thuật |
| :--- | :--- | :--- |
| **[XGBoost](https://xgboost.readthedocs.io/)** | `^2.1.4` | Mô hình Gradient Boosting MultiOutputRegressor dự báo 4 chỉ số môi trường ao nuôi tương lai |
| **[Scikit-Learn](https://scikit-learn.org/)** | `^1.6.1` | Tiền xử lý dữ liệu và cấu hình chuẩn hóa tham số huấn luyện mô hình |
| **[Joblib](https://joblib.readthedocs.io/)** | `^1.4.2` | Nạp và tuần tự hóa mô hình hồi quy (`pond_forecaster_xgb_latest.pkl`) |
| **[NumPy](https://numpy.org/)** | `^1.26.4` | Tính toán ma trận số học và thuật toán làm mịn lũy thừa đa chiều |
| **[Pandas](https://pandas.pydata.org/)** | `^2.2.3` | Xử lý tập dữ liệu chuỗi thời gian viễn trắc môi trường |

---

### Cơ sở Dữ liệu, Thông báo & Tự động hóa

| Công nghệ / Thư viện | Phiên bản | Vai trò Kỹ thuật |
| :--- | :--- | :--- |
| **[Motor](https://motor.readthedocs.io/)** | `^3.7.0` | Trình điều khiển bất đồng bộ cho MongoDB Atlas lưu trữ nhật ký cảm biến và cảnh báo |
| **[HTTPX](https://www.python-httpx.org/)** | `^0.28.1` | HTTP Client bất đồng bộ tương tác với Telegram Bot API gửi cảnh báo khẩn cấp |
| **[Python-Dotenv](https://github.com/theskumar/python-dotenv)** | `^1.0.1` | Nạp cấu hình từ môi trường cục bộ và container hóa |

---

## 2. Kiến trúc Nghiệp vụ & Hiện trạng Thử nghiệm

```mermaid
flowchart TD
    subgraph DataSources["Nguồn Dữ Liệu Đầu Vào (Thử Nghiệm)"]
        SensorsSim["Kịch bản Giả lập Phần mềm (scripts/simulate_sensors.py)"]
        SensorsHW["Mạch ESP32 Thử Nghiệm (Núm vặn biến trở / Potentiometers)"]
    end

    SensorsSim & SensorsHW -->|MQTT over TLS (Port 8883)| Broker["HiveMQ Cloud Broker"]
    Broker -->|Topic: pond1/sensors| MQTTClient["Paho-MQTT Client (Backend)"]
    
    MQTTClient --> Validate["Pydantic Validation (SensorLogCreate)"]
    Validate --> AI["XGBoost Forecaster (Dự báo xu hướng)"]
    AI --> Smooth["Bộ lọc Làm mịn Lũy thừa"]
    
    Smooth --> Broadcast["WebSocket Broadcast (/ws/data)"]
    Smooth --> AlertCheck["Kiểm tra Ngưỡng Vi phạm & Cảnh báo Sớm"]
    Smooth --> AutoRules["Động cơ Đề xuất / Tự động (Experimental Rules)"]
    Smooth --> DBInterval{"Chu kỳ 300s?"}
    
    AlertCheck -->|Nguy hiểm| Telegram["Telegram Bot Dispatcher"]
    AlertCheck -->|Lưu nhật ký| MongoAlerts[("MongoDB: alert_logs")]
    
    AutoRules -->|Lệnh ON/OFF| PublishActuator["Publish: pond1/actuators"]
    DBInterval -->|Đúng| MongoSensors[("MongoDB: sensor_logs")]
    
    Broadcast --> WebDashboard["Frontend Web Portal (Next.js)"]
    PublishActuator --> Relays["Mô hình Rơ-le Thử nghiệm (Quạt, Bơm, Đèn)"]
```

### 2.1. Tiếp nhận & Xử lý Dữ liệu Cảm biến MQTT

Backend lắng nghe liên tục trên topic `pond1/sensors` thông qua kênh truyền bảo mật TLS trên cổng 8883. Dữ liệu đầu vào bao gồm:
* **DO (Dissolved Oxygen):** Nồng độ oxy hòa tan trong nước ($mg/L$).
* **Nhiệt độ (Temperature):** Thước đo nhiệt độ nước ao ($\text{°C}$).
* **Độ pH:** Chỉ số kiềm / axit của môi trường nước.
* **Độ đục (Turbidity):** Thước đo chất rắn lơ lửng trong nước ($NTU$).
* **Mực nước (Water Level):** Trạng thái số xác định mức nước an toàn của ao.

### 2.2. Động cơ Dự báo Tham số Môi trường (XGBoost & Exponential Smoothing)

Để cảnh báo sớm nguy cơ tôm bị sốc môi trường hoặc thiếu oxy cục bộ, mô hình học máy tiếp nhận 4 chỉ số đo đạc và dự báo trạng thái môi trường trong 15 đến 60 phút tới. Nhằm loại bỏ nhiễu đột biến từ phần cứng thử nghiệm, giải thuật làm mịn lũy thừa được áp dụng:

$$
\mathbf{Y}_{\text{smoothed}} = 0.85 \cdot \mathbf{X}_{\text{current}} + 0.15 \cdot \mathbf{Y}_{\text{predicted}}
$$

Phương pháp kết hợp này đảm bảo tính ổn định của dự báo, giảm tỷ lệ cảnh báo giả và mang lại độ mượt mà cho các chỉ báo hiển thị.

### 2.3. Hệ thống Cảnh báo Sớm Đa tầng & Tích hợp Telegram

Hệ thống phân cấp cảnh báo thành hai tầng kiểm soát:
1. **Cảnh báo Vi phạm Tức thời (Instant Danger Thresholds):** Kích hoạt ngay khi thông số đo đạc vượt ra ngoài giới hạn an toàn quy định (`do_danger`, `temp_min`, `temp_max`, `ph_min`, `ph_max`, `turbidity_max`).
2. **Cảnh báo Sớm Trí tuệ Nhân tạo (AI Early Warning):** Kích hoạt khi thông số hiện tại vẫn nằm trong ngưỡng an toàn nhưng giá trị dự báo sẽ chạm ngưỡng nguy hiểm trong chu kỳ tới.

Thông báo được định tuyến bất đồng bộ qua Telegram Bot API kèm đầy đủ thông tin định lượng để người quản lý nắm bắt tình huống từ xa.

### 2.4. Động cơ Điều khiển & Lập lịch Thiết bị Phụ tải

Hệ thống hỗ trợ hai cơ chế vận hành thử nghiệm:
* **Chế độ Tự động (Auto Mode):** Kích hoạt rơ-le phụ tải theo các luật ngưỡng suy luận:
  * Kích hoạt máy sục khí (quạt oxy) khi DO dự báo giảm dưới ngưỡng nguy hiểm.
  * Kích hoạt đèn nhiệt khi nhiệt độ hạ thấp dưới ngưỡng tối thiểu.
  * Điều phối bơm cấp (`pump_in`) và bơm thoát (`pump_out`) khi pH hoặc độ đục bất thường.
* **Chế độ Lập lịch Thời gian (Timer Scheduling):** Vòng lặp định kỳ kiểm tra các cửa sổ thời gian hoạt động độc lập cho từng thiết bị (`timer_aerator`, `timer_pump_in`, `timer_pump_out`, `timer_light`).
* **Cơ chế Khóa Liên động An toàn (Safety Interlock):** Ngăn chặn thao tác kích hoạt bơm thoát khi cảm biến báo mực nước ở mức thấp.

### 2.5. Bảo mật & Xác thực Khóa Truy cập API

Các thao tác điều khiển thiết bị, cấu hình ngưỡng và thay đổi chế độ hệ thống bắt buộc phải đính kèm tiêu đề HTTP `X-API-Key`. Việc xác thực sử dụng hàm so sánh thời gian bất biến `secrets.compare_digest` nhằm loại bỏ hoàn toàn nguy cơ tấn công kênh kề đo lường thời gian (Timing Attack / CVE-2026-23996).

### 2.6. Hiện trạng Thử nghiệm Phần cứng & Giới hạn Can thiệp AI

Để đảm bảo tính minh bạch và chính xác về mặt kỹ thuật, hệ thống ghi nhận rõ các giới hạn trong giai đoạn nghiên cứu hiện tại:
* **Môi trường thử nghiệm phần cứng:** Chưa triển khai trực tiếp đầu dò công nghiệp vào môi trường ao nuôi thủy sản thực tế. Thay vào đó, nhóm phát triển sử dụng bo mạch thử nghiệm ESP32 tích hợp các **nút vặn biến trở xoay vật lý (Potentiometers)** qua module chuyển đổi ADS1115 nhằm giả lập sự thay đổi điện áp tương ứng với các mức nhiệt độ, pH, DO và độ đục khác nhau.
* **Mức độ can thiệp của AI:** Mô hình học máy hiện hoạt động ở tầng **khuyến nghị và cảnh báo sớm (Advisory / Early Warning)**. Trong điều kiện thực tế của hệ sinh thái ao nuôi tôm, việc cân bằng chất lượng nước đòi hỏi sự can thiệp đa yếu tố phức tạp (hóa chất xử lý, vi sinh, quạt nước cơ học, thời tiết tự nhiên). Vì vậy, AI trong hệ thống hiện tại chưa thể tự động điều phối vòng lặp kín để cân bằng hoàn toàn môi trường sinh thái thực tế mà vẫn cần sự can thiệp trực tiếp của con người.

---

## 3. Danh mục API Endpoints & Giao thức Kết nối

Hệ thống cung cấp giao diện RESTful API và WebSocket đồng bộ:

### Cấu hình Ngưỡng & Chế độ Vận hành (`/api/settings`)
* `GET /api/settings`: Truy xuất toàn bộ cấu hình ngưỡng an toàn, thông số tự động hóa và lịch trình timer hiện hành.
* `PATCH /api/settings`: Cập nhật cấu hình ngưỡng, bật/tắt tự động hóa thiết bị (Yêu cầu `X-API-Key`).

### Lịch sử Viễn trắc & Thống kê (`/api/history`)
* `GET /api/history`: Lấy danh sách bản ghi viễn trắc cảm biến theo bộ lọc phạm vi (`day`, `week`, `month`) và ngày mốc chỉ định (`date=YYYY-MM-DD`).

### Điều khiển Thiết bị & Chế độ Vận hành (`/api/control`)
* `POST /api/control`: Gửi lệnh điều khiển bật/tắt thiết bị phụ tải (`aerator`, `pump_in`, `pump_out`, `light`). Tự động chuyển hệ thống sang chế độ thủ công (Yêu cầu `X-API-Key`).
* `POST /api/control/mode`: Chuyển đổi trạng thái vận hành giữa `auto` và `manual` (Yêu cầu `X-API-Key`).

### Nhật ký Cảnh báo (`/api/alerts`)
* `GET /api/alerts`: Truy xuất lịch sử cảnh báo tức thời và cảnh báo sớm AI đã được phát hiện và gửi đi (hỗ trợ phân trang qua `limit` và `skip`).

### Kết nối Truyền phát Trực tiếp WebSocket (`/ws`)
* `WebSocket /ws/data`: Điểm cuối đăng ký luồng dữ liệu thời gian thực. Mỗi khi có bản tin cảm biến hợp lệ từ MQTT, máy chủ tự động phát quảng bá đến toàn bộ client đang kết nối.

### Kiểm tra Trạng thái Máy chủ (`/health`)
* `GET /health`: Kiểm tra tình trạng sẵn sàng phục vụ và môi trường hoạt động của máy chủ.

---

## 4. Hướng dẫn Cài đặt, Khởi chạy & Giả lập Dữ liệu

### 4.1. Cài đặt Môi trường Backend

Yêu cầu hệ thống: **Python 3.11.x**, cụm cơ sở dữ liệu **MongoDB Atlas**, tài khoản **HiveMQ Cloud MQTT** và **Telegram Bot Token**.

```bash
# 1. Tải mã nguồn Backend về máy cục bộ
git clone https://github.com/thanhthuan04/ecoshrimp-backend.git
cd ecoshrimp-backend

# 2. Khởi tạo môi trường ảo Python
python -m venv venv

# 3. Kích hoạt môi trường ảo (Windows PowerShell)
.\venv\Scripts\Activate.ps1
# (Trên Linux / macOS: source venv/bin/activate)

# 4. Nâng cấp pip và cài đặt thư viện phụ thuộc
pip install --upgrade pip
pip install -r requirements.txt
```

### 4.2. Cấu hình Biến Môi trường (.env)

Tạo tệp `.env` tại thư mục gốc của dự án với các thông số cấu hình:

```env
ENV=development
APP_NAME=EcoShrimp Backend

# Khóa bí mật API xác thực quyền điều khiển
API_KEY=your_secure_api_key_here

# Cơ sở dữ liệu MongoDB
MONGO_URI=mongodb+srv://<USERNAME>:<PASSWORD>@cluster.mongodb.net
MONGO_DB_NAME=ecoshrimp

# Cấu hình MQTT Broker HiveMQ
MQTT_HOST=your-cluster-id.s1.eu.hivemq.cloud
MQTT_PORT=8883
MQTT_USERNAME=your_mqtt_username
MQTT_PASSWORD=your_mqtt_password
MQTT_USE_TLS=true
MQTT_TOPIC_SENSOR=pond1/sensors
MQTT_TOPIC_ACTUATOR=pond1/actuators

# Tích hợp Telegram Bot
TELEGRAM_BOT_TOKEN=your_telegram_bot_token
TELEGRAM_CHAT_ID=your_telegram_chat_id

# Chia sẻ tài nguyên CORS
CORS_ORIGINS=http://localhost:3000,https://ecoshrimp-web.vercel.app

# Chu kỳ lưu dữ liệu vào MongoDB (giây)
SENSOR_LOG_INTERVAL_SECONDS=300
```

### 4.3. Khởi chạy Máy chủ API

```bash
# Khởi chạy máy chủ phát triển với chế độ tự động tải lại
uvicorn app.main:app --host 0.0.0.0 --port 8000 --reload
```

Truy cập tài liệu API:
* **Swagger UI:** `http://localhost:8000/docs`
* **Redoc UI:** `http://localhost:8000/redoc`

---

### 4.4. Hướng dẫn Sử dụng Kịch bản Giả lập Cảm biến (`scripts/simulate_sensors.py`)

Khi chưa có bo mạch vi điều khiển ESP32 hoặc không kết nối cảm biến phần cứng thật, hệ thống cung cấp sẵn công cụ giả lập luồng dữ liệu cảm biến đa thông số để kiểm thử toàn diện giao diện Web, biểu đồ, WebSocket và thuật toán cảnh báo.

#### Cơ chế hoạt động:
Kịch bản nạp cấu hình MQTT từ tệp `.env`, tự động kết nối bảo mật qua cổng TLS 8883 và liên tục sinh dữ liệu biến thiên ngẫu nhiên theo mô hình chuỗi thời gian thực tế:
* **Oxy hòa tan (DO):** Dao động quanh mức $5.0 \pm 0.1\,mg/L$.
* **Nhiệt độ (Temp):** Dao động quanh mức $30.0 \pm 0.2\,\text{°C}$.
* **Độ pH:** Dao động quanh mức $7.8 \pm 0.05$.
* **Độ đục (Turbidity):** Dao động quanh mức $10.0 \pm 0.5\,NTU$.
* **Mực nước (Level):** Giá trị số `1` (bình thường).

Mỗi 2 giây, kịch bản đóng gói cấu trúc JSON và xuất bản lên topic MQTT `pond1/sensors`:
```json
{
  "temp": 29.85,
  "ph": 7.78,
  "do": 4.92,
  "turbidity": 10.35,
  "level": 1
}
```

#### Các bước thực thi:

1. Đảm bảo máy chủ Backend đang hoạt động hoặc thông tin Broker HiveMQ trong `.env` đã chính xác.
2. Mở một cửa sổ dòng lệnh (Terminal) mới và kích hoạt môi trường ảo Python:
   ```bash
   .\venv\Scripts\Activate.ps1
   ```
3. Khởi chạy kịch bản giả lập:
   ```bash
   python scripts/simulate_sensors.py
   ```
4. Màn hình console sẽ hiển thị thông báo kết nối thành công và in liên tục các bản tin gửi đi:
   ```text
   Đã kết nối với MQTT Broker: your-cluster.hivemq.cloud:8883
   Bắt đầu gửi dữ liệu giả lập lên topic 'pond1/sensors'...
   Gửi: {'temp': 30.12, 'ph': 7.82, 'do': 5.05, 'turbidity': 9.85, 'level': 1}
   Gửi: {'temp': 30.08, 'ph': 7.79, 'do': 4.98, 'turbidity': 10.12, 'level': 1}
   ```
5. Mở giao diện Frontend tại `http://localhost:3000` để quan sát các thẻ chỉ số nhảy số tức thời và đồ thị Recharts vẽ dữ liệu thời gian thực.
6. Để dừng giả lập, nhấn tổ hợp phím `Ctrl + C`.

---

### 4.5. Kiểm thử Lắng nghe Lệnh Điều khiển Phụ tải (`scripts/test_mqtt.py`)

Để kiểm tra các lệnh bật/tắt thiết bị phụ tải được xuất bản từ Web Dashboard hoặc từ động cơ luật tự động, sử dụng kịch bản kiểm thử:

```bash
# Khởi chạy trình lắng nghe lệnh rơ-le trên topic pond1/actuators/#
python scripts/test_mqtt.py
```

Khi người dùng thao tác bật quạt hoặc bơm trên giao diện, terminal sẽ in ra kết quả nhận được theo thời gian thực:
```text
pond1/actuators/aerator -> ON
pond1/actuators/pump_in -> OFF
```

---

### 4.6. Triển khai Bằng Docker

```bash
# Xây dựng Docker Image
docker build -t ecoshrimp-backend:latest .

# Khởi chạy Container
docker run -d --name ecoshrimp-backend -p 8000:8000 --env-file .env ecoshrimp-backend:latest
```

---

## 5. Cấu trúc Thư mục Dự án

```text
ecoshrimp-backend/
├── .github/
│   └── workflows/
│       └── ci.yaml                # Tự động hóa CI/CD build Docker và trigger webhook deploy Render
├── app/
│   ├── ai/                        # Dịch vụ nạp mô hình và dự báo tham số nước
│   │   ├── forecast_service.py    # Logic nạp mô hình và giải thuật làm mịn lũy thừa
│   │   └── models/                # Trọng số mô hình hồi quy XGBoost đã huấn luyện
│   ├── core/                      # Cấu hình biến môi trường và cơ chế bảo mật
│   │   ├── database.py            # Kết nối cơ sở dữ liệu Motor MongoDB và khởi tạo index
│   │   ├── env.py                 # Khai báo Settings Pydantic
│   │   └── security.py            # Xác thực API Key an toàn chống Timing Attack
│   ├── models/                    # Thực thể dữ liệu ODM Pydantic
│   │   ├── alert_log.py           # Thực thể bản ghi cảnh báo và Enum AlertType
│   │   ├── sensor_log.py          # Thực thể số liệu cảm biến và dự báo
│   │   └── settings.py            # Thực thể cấu hình ngưỡng và lập lịch timer
│   ├── mqtt/                      # Giao tiếp IoT qua giao thức MQTT
│   │   ├── client.py              # Quản lý kết nối, đăng ký topic và vòng lặp Paho
│   │   ├── handlers.py            # Tiếp nhận, phân tích payload và kích hoạt chuỗi xử lý
│   │   └── publisher.py           # Xuất bản lệnh điều khiển rơ-le phụ tải
│   ├── repositories/              # Tầng tương tác dữ liệu MongoDB (Data Access Layer)
│   │   ├── alert_repository.py
│   │   ├── sensor_repository.py
│   │   └── settings_repository.py
│   ├── routers/                   # Các tuyến định tuyến RESTful Endpoints
│   │   ├── alerts.py              # Truy vấn nhật ký cảnh báo
│   │   ├── control.py             # Điều khiển thiết bị và chuyển chế độ vận hành
│   │   ├── history.py             # Truy vấn lịch sử viễn trắc theo khoảng thời gian
│   │   └── settings.py            # Quản trị thông số ngưỡng ao nuôi
│   ├── services/                  # Tầng xử lý logic nghiệp vụ trung tâm
│   │   ├── alert_query_service.py # Truy vấn danh sách cảnh báo
│   │   ├── alert_service.py       # Kiểm định ngưỡng vi phạm và gửi thông báo Telegram
│   │   ├── automation_service.py   # Động cơ ra quyết định điều khiển thiết bị tự động
│   │   ├── history_service.py     # Xử lý truy vấn dữ liệu lịch sử cảm biến
│   │   ├── scheduler_service.py    # Bộ lập lịch kiểm tra khung giờ timer định kỳ
│   │   └── settings_service.py     # Cập nhật và lưu trữ cấu hình ngưỡng
│   ├── ws/                        # Xử lý kết nối WebSocket thời gian thực
│   │   ├── manager.py             # Quản lý danh sách kết nối và broadcast dữ liệu
│   │   └── router.py              # Tuyến định tuyến kết nối /ws/data
│   └── main.py                    # Khởi tạo ứng dụng FastAPI, CORS và vòng đời Lifespan
├── archive/
│   └── Aquaponds Dataset.csv      # Tập dữ liệu nguồn huấn luyện mô hình học máy
├── ml/
│   └── train_forecaster.py        # Kịch bản huấn luyện XGBoost MultiOutputRegressor
├── scripts/
│   ├── simulate_sensors.py        # Kịch bản giả lập số liệu cảm biến đẩy lên MQTT khi không có thiết bị thật
│   └── test_mqtt.py               # Công cụ CLI lắng nghe và kiểm thử lệnh điều khiển
├── esp32_ecoshrimp.ino            # Firmware C++ ESP32 FreeRTOS kết nối HiveMQ Cloud
├── Dockerfile                     # Cấu hình đóng gói container nền tảng python:3.11-slim
├── requirements.txt               # Danh mục thư viện phụ thuộc có định phiên bản bảo mật
└── README.md                      # Tài liệu kỹ thuật chi tiết dự án
```

---

## 6. Tích hợp Hệ sinh thái & CI/CD

Dịch vụ Backend này hoạt động kết hợp với giao diện **EcoShrimp Web Portal** (Next.js 16 App Router):
* **Tích hợp Tầng Biên Thử nghiệm (Edge IoT):** Thiết bị ESP32 thu thập tín hiệu viễn trắc từ các núm vặn biến trở hoặc kịch bản giả lập phần mềm, đóng gói JSON và xuất bản lên broker HiveMQ Cloud qua cổng 8883 có mã hóa TLS.
* **Tích hợp Trực tiếp (WebSocket):** Backend tiếp nhận bản tin, thực thi dự báo XGBoost và đẩy trực tiếp xuống Web Dashboard với độ trễ thấp.
* **Quy trình Tự động hóa CI/CD:** Hệ thống tích hợp GitHub Actions workflow (`.github/workflows/ci.yaml`):
  * Tự động kiểm tra tính toàn vẹn khi tạo Pull Request vào nhánh `dev` hoặc `main`.
  * Tự động kích hoạt Webhook Deploy tới nền tảng **Render** mỗi khi nhánh `main` được cập nhật.

---

## 7. Tác giả

Dự án được nghiên cứu và phát triển bởi:

* **Hứa Viết Thái**  
  Email: [huavietthai299@gmail.com](mailto:huavietthai299@gmail.com) | GitHub: [https://github.com/hvt299](https://github.com/hvt299)
* **Nguyễn Thành Thuận**  
  Email: [ntthuan172@gmail.com](mailto:ntthuan172@gmail.com) | GitHub: [https://github.com/thanhthuan04](https://github.com/thanhthuan04)
