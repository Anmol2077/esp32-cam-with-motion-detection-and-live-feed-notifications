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
