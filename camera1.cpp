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

#define RED_LED_PIN 33

#include "esp_camera.h"
#include "Arduino.h"
#include "Base64.h"
#include <FirebaseClient.h>

#include "1.hpp"
#include "timeesp.hpp"
extern gbs_st gbs;

extern String getWiFiStatusString();

String esplog = "";
unsigned long globalcommandrequest = 0;
unsigned long prev_globalcommandrequest = 0;

// The API key can be obtained from Firebase console > Project Overview > Project settings.
#define API_KEY "AIzaSyCyURdbd0DwnmeegDRE6HdblOT9J7IQPuk"

// User Email and password that already registerd or added in your project.
#define USER_EMAIL "tstark49000@gmail.com"
#define USER_PASSWORD "Qwerty123456"
#define DATABASE_URL "https://espx-2051-default-rtdb.asia-southeast1.firebasedatabase.app/"

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

void transmitTelegram(camera_fb_t * fb);
void transmitTelegramAlert(uint8 v);

int safeStringToInt(String input);

void printError(int code, const String &msg);

void asyncCB(AsyncResult &aResult);

DefaultNetwork network; // initilize with boolean parameter to enable/disable network reconnection

UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD);

FirebaseApp app;

#if defined(ESP32) || defined(ESP8266) || defined(PICO_RP2040)
#include <WiFiClientSecure.h>
WiFiClientSecure ssl_client;
#elif defined(ARDUINO_ARCH_SAMD)
#include <WiFiSSLClient.h>
WiFiSSLClient ssl_client;
#endif

// In case the keyword AsyncClient using in this example was ambigous and used by other library, you can change
// it with other name with keyword "using" or use the class name AsyncClientClass directly.

using AsyncClient = AsyncClientClass;

AsyncClient aClient(ssl_client, getNetwork(network));

RealtimeDatabase Database;

void firebase_init_cyc()
{
  if((gbs.wificonnected == 1) && (gbs.firebaseinit == 0))
  {
    Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);
#if defined(ESP32) || defined(ESP8266) || defined(PICO_RP2040)
    ssl_client.setInsecure();
#if defined(ESP8266)
    ssl_client.setBufferSizes(4096, 1024);
#endif
#endif
    initializeApp(aClient, app, getAuth(user_auth));
    unsigned long ms = millis();
    ms = millis();
    while (app.isInitialized() && !app.ready() && millis() - ms < 120 * 1000)
        ;

    app.getApp<RealtimeDatabase>(Database);

    Database.url(DATABASE_URL);

    gbs.firebaseinit = 1;   
    gbs.firebaseready = 1; 
  }
}

void firebase_cyc()
{
  // if((gbs.firebaseready == 0) && (gbs.firebaseinit == 1))
  // {
  //   if(app.isInitialized()) 
  //   {  // Check if app is initialized
  //     if(app.ready())
  //     {
  //       app.getApp<RealtimeDatabase>(Database);
  //       Database.url(DATABASE_URL);
  //       gbs.firebaseready = 1;
  //       Serial.println("fb ok"); 
  //     }
  //   }
  // }
}

void camera_init_cyc()
{
  if((gbs.cameraready == 0) &&
		  (gbs.firebaseready == 1) &&
		  (gbs.telecameraneeded == 0) &&
		  (gbs.telecameraready == 0))
  {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    config.frame_size = FRAMESIZE_HVGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 12;
    config.fb_count = 2;

      // Init Camera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
      Serial.printf("Camera failed: error 0x%x", err);
      return;
    }

//    Serial.printf("cam ok");
    gbs.cameraready = 1;

    sensor_t * s = esp_camera_sensor_get();

     s->set_brightness(s, 0);     // -2 to 2
     s->set_contrast(s, 0);       // -2 to 2
     s->set_saturation(s, 0);     // -2 to 2
     s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
     s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
     s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
     s->set_wb_mode(s, 0);        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
     s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
     s->set_aec2(s, 0);           // 0 = disable , 1 = enable
     s->set_ae_level(s, 0);       // -2 to 2
     s->set_aec_value(s, 1200);    // 0 to 1200
     s->set_gain_ctrl(s, 1);      // 0 = disable , 1 = enable
     s->set_agc_gain(s, 0);       // 0 to 30
    // s->set_bpc(s, 0);            // 0 = disable , 1 = enable
    // s->set_wpc(s, 1);            // 0 = disable , 1 = enable
    // s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
    // s->set_lenc(s, 1);           // 0 = disable , 1 = enable
    // s->set_hmirror(s, 0);        // 0 = disable , 1 = enable
    // s->set_vflip(s, 0);          // 0 = disable , 1 = enable
    // s->set_dcw(s, 1);            // 0 = disable , 1 = enable
    // s->set_colorbar(s, 0);       // 0 = disable , 1 = enable
    s->set_gainceiling(s,GAINCEILING_64X);
    s->set_vflip(s,1);
    s->set_hmirror(s, 1);
    s->set_framesize(s,FRAMESIZE_HVGA);
  }
}

String urlencode(String str) {
  const char *msg = str.c_str();
  const char *hex = "0123456789ABCDEF";
  String encodedMsg = "";
  while (*msg != '\0') {
    if (('a' <= *msg && *msg <= 'z') || ('A' <= *msg && *msg <= 'Z') || ('0' <= *msg && *msg <= '9') || *msg == '-' || *msg == '_' || *msg == '.' || *msg == '~') {
      encodedMsg += *msg;
    } else {
      encodedMsg += '%';
      encodedMsg += hex[(unsigned char)*msg >> 4];
      encodedMsg += hex[*msg & 0xf];
    }
    msg++;
  }
  return encodedMsg;
}

String Photo2Base64() {
    camera_fb_t * fb = NULL;
    fb = esp_camera_fb_get();  
    if(!fb) {
      Serial.println("Cam cap failed");
      esplog = "camera failed";
      digitalWrite(RED_LED_PIN, LOW);  // LED ON (Active-Low)
      return "";
    }

    if(gbs.motiondetected == 1)
    {
    	    transmitTelegramAlert(0);
    		transmitTelegram(fb);
    		transmitTelegramAlert(1);
    		gbs.motiondetected = 0;
    		gbs.motiondetectionincooldown = 1;
    }

    String imageFile = "data:image/jpeg;base64,";
    char *input = (char *)fb->buf;
    char output[base64_enc_len(3)];
    for (int i=0;i<fb->len;i++) {
      base64_encode(output, (input++), 3);
      if (i%3==0) imageFile += urlencode(String(output));
    }

    esp_camera_fb_return(fb);
    fb = NULL;
    
    return imageFile;
}

void cctv_cyc()
{
  if((gbs.cameraready == 1) &&
     (gbs.firebaseready == 1) &&
     (gbs.wificonnected == 1) &&
     (gbs.telegramtrnsinprogress == 0))
  {
    gbs.cctvtransinprogress = 1;

    // Required for handling async operations
    app.loop();
    Database.loop();

    // 1. Check the 'live' button status from Firebase (0 = Stop, 1 = Live)
    // Using the same synchronous client method as your set function
    int isLive = Database.get<int>(aClient, "/live");

    // Only process and transmit if the live feed is requested
    if (isLive == 1) {

    		gbs.livecamfeedrunning = 1;
      // 2. Fetch and format the current global time
      app_time_t current_time = get_global_time();
      String timeStr = "[Time Not Synced]";

      if (current_time.is_valid) {
          char tBuffer[64];
          snprintf(tBuffer, sizeof(tBuffer), "%04d-%02d-%02d %02d:%02d:%02d",
                   current_time.year, current_time.month, current_time.day,
                   current_time.hour, current_time.minute, current_time.second);
          timeStr = String(tBuffer);
      }

      // 3. Convert Photo to Base64
      String stringValue = Photo2Base64();

      // 4. Send Image and Time to separate Firebase nodes
      Database.set<String>(aClient, "/counter", stringValue);
      Database.set<String>(aClient, "/cctv_time", timeStr);

      // Error Handling
      if (aClient.lastError().code() != 0) {
          printError(aClient.lastError().code(), aClient.lastError().message());
      }
    }
    else
    {
    		gbs.livecamfeedrunning = 0;
    }

    gbs.cctvtransinprogress = 0;
  }
}

void asyncCB(AsyncResult &aResult)
{
    if (aResult.appEvent().code() > 0)
    {
        Firebase.printf("Event task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.appEvent().message().c_str(), aResult.appEvent().code());
    }

    if (aResult.isDebug())
    {
        Firebase.printf("Debug task: %s, msg: %s\n", aResult.uid().c_str(), aResult.debug().c_str());
    }

    if (aResult.isError())
    {
        Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());
    }
}

void printError(int code, const String &msg)
{
    Firebase.printf("Error, msg: %s, code: %d\n", msg.c_str(), code);
}


/*************************************************************************************************************************************************/


void read_web_controls_500ms() {
    // Only attempt to read if we have an active network connection
    if ((gbs.firebaseready == 1) && (gbs.wificonnected == 1)) {

        // Required to maintain connection state for Mobizt RTDB client
        app.loop();
        Database.loop();

        // --- Read F1 to F5 ---
        int f1 = Database.get<int>(aClient, "/F1");
        int f2 = Database.get<int>(aClient, "/F2");
        int f3 = Database.get<int>(aClient, "/F3");
        int f4 = Database.get<int>(aClient, "/F4");
        int f5 = Database.get<int>(aClient, "/F5");

        // --- Read Text Box Command ---
        String cmd = Database.get<String>(aClient, "/command");

        // --- Execution Logic ---

        if (f1 == 1) {
            // F1 is pressed (Yellow)
//            Serial.println("Restart Requested");
            esplog = "F1 - CAM Restart Requested";
            gbs.esprestartrequested = 1;

            // Add your hardware code here (e.g., turn on a floodlight)
        }

        if (f2 == 1) {
            // F2 is pressed (Yellow)
//            Serial.println("E F2");
            gbs.motiondetected = 1;
            esplog = "F2 - motion simulation";
        }

        if (f3 == 1) {
            // F3 is pressed (Yellow)
//            Serial.println("E F3");
        		gbs.motiondetectionstopped = 1;
            esplog = "F3 - motion D stopped";
        }

        if (f4 == 1) {
            // F4 is pressed (Yellow)
//            Serial.println("E F4");
        		gbs.motiondetectionstopped = 0;
            esplog = "F4 - motion D resumed";
        }

        if (f5 == 1) {
            // F5 is pressed (Yellow)
//            Serial.println("E F5");
        		esplog = "F5 " + getWiFiStatusString();
        }

        if (cmd != "0") {
            // Command is something other than the default "0"
//            Serial.print("Custom Command: ");
        		if(prev_globalcommandrequest != safeStringToInt(cmd))
        		{
				esplog = "Command" + cmd;
				globalcommandrequest = safeStringToInt(cmd);
				prev_globalcommandrequest = globalcommandrequest;
        		}

//            Serial.println(cmd);

            // Example: if (cmd == "PAN_LEFT") { move_motor(); }
        }
    }
}

void cyclic_resetglobalcommandrequest()
{
	prev_globalcommandrequest = 0;
}

unsigned long camera_heartbeat = 0;

void update_camera_status_2sec() {
    // Check if we have an active connection to Wi-Fi and Firebase
    if ((gbs.firebaseready == 1) && (gbs.wificonnected == 1)) {

        // Keep the async operations and connection tasks alive
        app.loop();
        Database.loop();

        camera_heartbeat++;
        // Write '1' to indicate the camera is connected and active
        Database.set<int>(aClient, "/camon", camera_heartbeat);

    }
}

unsigned long nlog_counter = 0;

void send_response_log_1sec() {
    // Only attempt to write if network is active
    if ((gbs.firebaseready == 1) && (gbs.wificonnected == 1))
    {

    		if(esplog == "")
    		{
    			//nothing
    		}
    		else
    		{
    			esplog = String(nlog_counter) + " - " + esplog;

    	        app.loop();
    	        Database.loop();

    	        nlog_counter++;

    	        // Push to the '/nlog' node
    	        Database.set<String>(aClient, "/nlog", esplog);
    	        esplog = "";
    		}

    }
}

int safeStringToInt(String input) {
    // 1. Remove any accidental leading/trailing spaces
    input.trim();

    // 2. Catch empty strings immediately
    if (input.isEmpty()) {
        return 0;
    }

    char *endptr;
    const char *str = input.c_str();

    // 3. Convert the C-string to a long integer (base 10)
    long result = strtol(str, &endptr, 10);

    // 4. Safety Check: Did it fail to find any numbers at all?
    if (endptr == str) {
        return 0; // The string was completely non-numeric (e.g., "hello")
    }

    // 5. Safety Check: Did it stop converting early because it hit a letter?
    if (*endptr != '\0') {
        return 0; // The string was mixed (e.g., "123abc")
    }

    // 6. If it passed all checks, cast the result safely back to an int
    return (int)result;
}
