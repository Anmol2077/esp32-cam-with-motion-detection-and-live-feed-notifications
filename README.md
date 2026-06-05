# ESP32-CAM Telegram Motion Security Hub

An edge-AI security system built on the ESP32-CAM platform that captures a 10-second Motion JPEG (AVI) video clip upon motion detection, transmits it securely to a Telegram channel via the Telegram Bot API, and updates the user with an instant status alert containing a live-stream dashboard link.

---

## 🚀 Key Features

* **Dynamic Video Capture:** Automatically switches the camera sensor to VGA resolution to record a 10-second video at ~10 FPS upon event trigger.
* **Zero-Loss PSRAM Buffering:** Utilizes external PSRAM to sequentially cache raw JPEG frames, eliminating SD card write latency overhead during recording.
* **On-the-Fly RIFF/AVI Muxing:** Assembles a fully valid Motion JPEG `.avi` container directly in memory, patching tracking metadata blocks down to the byte before network transmission.
* **Secure API Tunneling:** Establishes a TLS/SSL connection directly to `api.telegram.org` over port 443 using chunked multipart/form-data streaming.
* **Real-time Alerts:** Sends a follow-up network status alert along with an external link to a live web-app monitoring dashboard.
* **Hardware Diagnostics Engine:** Features low-level ISR error interception. If the camera module fails to register over the $I^2C$ (SCCB) bus, it traps the MCU and flashes the onboard red LED (`GPIO 33`) to indicate a hardware panic.
* **Ambient Flash Control:** Implements high-frequency PWM (Pulse Width Modulation) via the updated ESP32 LEDC API to dimly light the front-facing white LED (`GPIO 4`) without causing camera frame flicker.

---

## 🛠️ Hardware Requirements

* **Development Board:** AI-Thinker ESP32-CAM (with OV2640 Sensor Module)
* **PSRAM:** Minimum 4MB External PSRAM enabled
* **Status Indicator:** Onboard Red LED (Hardwired to `GPIO 33`, Active-LOW)
* **Illumination:** Onboard White Flash LED (Hardwired to `GPIO 4`)

---

## 📐 System Architecture

The firmware utilizes a non-blocking sequence to capture data, generate headers, and handle network transitions securely.

[ Motion Event Trigger ]
              │
              ▼
[ Switch Sensor to VGA (640x480) ]
              │
              ▼
[ Capture 100 Frames -> Buffer to PSRAM ]
│
▼
[ Compute Total File Footprint + Padding ]
│
▼
[ Open Secure TLS Socket (Port 443) to Telegram ]
│
▼
[ Stream Multipart Headers -> AVI Video Data -> Boundaries ]
│
▼
[ Send Confirmation Alert Text + Live Dashboard URL ]
│
▼
[ Flush Buffers & Free Memory ]
│
▼
[ Revert Sensor to UXGA Standby Mode ]


---

## 📝 Firmware Configuration

Ensure your main configuration header includes your explicit bot tokens, chat IDs, and Wi-Fi access criteria:

#define WIFI_SSID "Your_WiFi_Network"
#define WIFI_PASS "Your_WiFi_Password"
#define BOTtoken  "1234567890:ABCdefGhIJKlmNoPQRsTUVwxyZ"
#define CHAT_ID   "-100123456789"
ESP32 Arduino Core Compatibility
This repository targets ESP32 Arduino Core v3.x.

Peripheral configuration for the dimming circuit utilizes the modern ledcAttach(pin, frequency, resolution) and ledcWrite(pin, duty) abstractions instead of deprecated legacy channels.

📂 Project Structure
Plaintext
├── ESP32_Telegram_Security.ino   # Main application initialization and state loop
├── camera_pins.h                  # Hardware-specific pin definitions for AI-Thinker
├── README.md                      # Project documentation
⚠️ Important Considerations & Shared Pins
GPIO 4 (White LED): This line is shared with the Data 1 line (HS2_DATA1) of the onboard MicroSD card slot when run in 4-bit SDMMC mode. If you intend to use an SD card alongside the dimming flash feature, initialize the card reader strictly in 1-bit mode:

C++
SD_MMC.begin("/sdcard", true); // The true flag forces 1-bit data bus routing
Video Playback: The resulting file is streamed as a Motion JPEG inside an .avi container. Because it does not utilize H.264 compression blocks, Telegram client applications treat it as a downloadable media document rather than an inline auto-playing video element.
