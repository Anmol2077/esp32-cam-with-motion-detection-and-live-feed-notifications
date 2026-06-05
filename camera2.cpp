#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif __has_include(<WiFiNINA.h>)
#include <WiFiNINA.h>
#elif __has_include(<WiFi101.h>)
#include <WiFi101.h>
#elif __has_include(<WiFiS3.h>)
#include <WiFiS3.h>
#endif

#include "esp_camera.h"
#include "Arduino.h"
#include "Base64.h"
#include <FirebaseClient.h>

#include "1.hpp"
extern gbs_st gbs;\

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void camera_init2_cyc()
{
//  if((gbs.cameraready == 0) && (gbs.telecameraneeded == 1) && (gbs.telecameraready == 0))
//  {
//    camera_config_t config;
//    config.ledc_channel = LEDC_CHANNEL_0;
//    config.ledc_timer = LEDC_TIMER_0;
//    config.pin_d0 = Y2_GPIO_NUM;
//    config.pin_d1 = Y3_GPIO_NUM;
//    config.pin_d2 = Y4_GPIO_NUM;
//    config.pin_d3 = Y5_GPIO_NUM;
//    config.pin_d4 = Y6_GPIO_NUM;
//    config.pin_d5 = Y7_GPIO_NUM;
//    config.pin_d6 = Y8_GPIO_NUM;
//    config.pin_d7 = Y9_GPIO_NUM;
//    config.pin_xclk = XCLK_GPIO_NUM;
//    config.pin_pclk = PCLK_GPIO_NUM;
//    config.pin_vsync = VSYNC_GPIO_NUM;
//    config.pin_href = HREF_GPIO_NUM;
//    config.pin_sscb_sda = SIOD_GPIO_NUM;
//    config.pin_sscb_scl = SIOC_GPIO_NUM;
//    config.pin_pwdn = PWDN_GPIO_NUM;
//    config.pin_reset = RESET_GPIO_NUM;
//    config.xclk_freq_hz = 20000000;
//    config.pixel_format = PIXFORMAT_JPEG;
//
//    config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
//    config.jpeg_quality = 15;
//    config.fb_count = 1;
//
//      // Init Camera
//    esp_err_t err = esp_camera_init(&config);
//    if (err != ESP_OK) {
//      Serial.printf("Camera failed: error 0x%x", err);
//      return;
//    }
//
////    Serial.printf("cam ok tele");
//    gbs.telecameraready = 1;
//
//    sensor_t * s = esp_camera_sensor_get();
//
//     s->set_brightness(s, 0);     // -2 to 2
//     s->set_contrast(s, 0);       // -2 to 2
//     s->set_saturation(s, 0);     // -2 to 2
//     s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
//     s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
//     s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
//     s->set_wb_mode(s, 0);        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
//     s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
//     s->set_aec2(s, 0);           // 0 = disable , 1 = enable
//     s->set_ae_level(s, 0);       // -2 to 2
//     s->set_aec_value(s, 1200);    // 0 to 1200
//     s->set_gain_ctrl(s, 1);      // 0 = disable , 1 = enable
//     s->set_agc_gain(s, 0);       // 0 to 30
//    s->set_gainceiling(s,GAINCEILING_64X);
//    s->set_vflip(s,0);
//    s->set_hmirror(s, 0);
//    s->set_framesize(s,FRAMESIZE_UXGA);
//  }
}
