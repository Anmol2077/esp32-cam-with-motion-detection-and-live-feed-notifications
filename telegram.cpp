
#include "Arduino.h"
#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "esp_wifi.h"

#include "0.hpp"

#define BOTtoken "8656202682:AAGrmWPdnPoZfWu5IXqP2NpoOn1YBOfs4Zc" // Your API Token from @BotFather
#define CHAT_ID  "-1003735335950"                      // Your Telegram User or Group ID

// Network Client configurations
WiFiClientSecure clientTCP;
UniversalTelegramBot bot(BOTtoken, clientTCP);

extern gbs_st gbs;
extern String esplog;
uint32 testcount = 0;
uint8 frames_sent = 0;

#define NO_OF_FRAMES_IN_MOTION_TELEGRAM 2


void telegram_preparecamera()
{
	if(gbs.telecameraneeded == 0)
	{
		esp_camera_deinit();
		gbs.telecameraneeded = 1;
		gbs.cameraready = 0;
	}
}

void cyclic_detectMotion() {

	if((gbs.livecamfeedrunning == 0) &&
			(gbs.motiondetectionincooldown == 0) &&
			(gbs.motiondetectionstopped == 0))
	{
		bool val = digitalRead(GPIO_NUM_12);
		if(val == HIGH)
		{
			gbs.motiondetected = 1;
//			printf("m-det\n");
			esplog = "motion detected LIR";

		}
	}

	if(gbs.pirsensorisdisabled == 0)
	{
		if(gbs.pirsensorisready == 0)
		{
			bool val2 = digitalRead(GPIO_NUM_13);
			if(val2 == LOW)
			{
				gbs.pirsensorisready = 1;
				esplog = "PIR sense ready";
			}
		}
		else
		{
			bool val4 = digitalRead(GPIO_NUM_13);
			if(val4 == HIGH)
			{
				gbs.motiondetected = 1;
				gbs.pirsensorisready = 0;
				esplog = "motion detected PIR";
			}
		}
	}

}

void cyclic_init_telegram(){

	if((gbs.telegramstarted == 0) && (gbs.cameraready == 1) && (gbs.wificonnected == 1))
	{
		clientTCP.setInsecure();
		gbs.telegramstarted = 1;
	}
}

void cyc_sendpictotelegram()
{
	if(gbs.motiondetected == 1)
	{
		Photo2Base64();
		gbs.motiondetected = 0;
	}

}

void transmitTelegramPhoto() {
//  camera_fb_t * fb = esp_camera_fb_get();
//  if(!fb) {
//    return;
//  }
//
//  Serial.println("TS+");
//
//  String head = "--SecurityWire\r\nContent-Disposition: form-data; name=\"chat_id\"; \r\n\r\n" + String(CHAT_ID) + "\r\n--SecurityWire\r\nContent-Disposition: form-data; name=\"photo\"; filename=\"alert.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
//  String tail = "\r\n--SecurityWire--\r\n";
//
//  uint32_t totalLen = head.length() + fb->len + tail.length();
//
//  if (clientTCP.connect("api.telegram.org", 443)) {
//    clientTCP.println("POST /bot" + String(BOTtoken) + "/sendPhoto HTTP/1.1");
//    clientTCP.println("Host: api.telegram.org");
//    clientTCP.println("Content-Type: multipart/form-data; boundary=SecurityWire");
//    clientTCP.print("Content-Length: ");
//    clientTCP.println(totalLen);
//    clientTCP.println();
//
//    clientTCP.print(head);
//
//    uint8_t *fbBuf = fb->buf;
//    size_t fbLen = fb->len;
//    const size_t bufferSize = 1024;
//
//    for (size_t n=0; n<fbLen; n+=bufferSize) {
//      size_t currentChunkSize = (fbLen - n < bufferSize) ? fbLen - n : bufferSize;
//      clientTCP.write(fbBuf + n, currentChunkSize);
//    }
//
//    clientTCP.print(tail);
//    esp_camera_fb_return(fb);
//    fb = NULL;
//
//    clientTCP.stop(); // FIXED: Force clear the secure socket slot upon successful upload
//    return;
//  }
//
//  esp_camera_fb_return(fb);
//  fb = NULL;
//  clientTCP.stop(); // FIXED: Force clear the secure socket slot if the route fails
//  return;
}

void transmitTelegram(camera_fb_t * fb) {

  Serial.println("TS+");

  String head = "--SecurityWire\r\nContent-Disposition: form-data; name=\"chat_id\"; \r\n\r\n" + String(CHAT_ID) + "\r\n--SecurityWire\r\nContent-Disposition: form-data; name=\"photo\"; filename=\"alert.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
  String tail = "\r\n--SecurityWire--\r\n";

  uint32_t totalLen = head.length() + fb->len + tail.length();

  if (clientTCP.connect("api.telegram.org", 443)) {
    clientTCP.println("POST /bot" + String(BOTtoken) + "/sendPhoto HTTP/1.1");
    clientTCP.println("Host: api.telegram.org");
    clientTCP.println("Content-Type: multipart/form-data; boundary=SecurityWire");
    clientTCP.print("Content-Length: ");
    clientTCP.println(totalLen);
    clientTCP.println();

    clientTCP.print(head);

    uint8_t *fbBuf = fb->buf;
    size_t fbLen = fb->len;
    const size_t bufferSize = 1024;

    for (size_t n=0; n<fbLen; n+=bufferSize) {
      size_t currentChunkSize = (fbLen - n < bufferSize) ? fbLen - n : bufferSize;
      clientTCP.write(fbBuf + n, currentChunkSize);
    }

    clientTCP.print(tail);

    clientTCP.stop(); // FIXED: Force clear the secure socket slot upon successful upload
    return;
  }

  clientTCP.stop(); // FIXED: Force clear the secure socket slot if the route fails
  return;
}


void transmitTelegramAlert(uint8 v) {
  // URL encode spaces as %20 so the Telegram API parses the string correctly
  String message = "";
  if(v == 0)
  {
	  message = "Motion%20detected%20in%20top%20floor";
  }
  if(v == 1)
  {
	  message = "Check%20the%20live%20feed%20below%20:%3A%0Ahttps%3A%2F%2Fespx-2051.web.app%2F";
  }

  String payload = "chat_id=" + String(CHAT_ID) + "&text=" + message;

 // Serial.println("Sending text alert to Telegram...");

  if (clientTCP.connect("api.telegram.org", 443)) {
    clientTCP.println("POST /bot" + String(BOTtoken) + "/sendMessage HTTP/1.1");
    clientTCP.println("Host: api.telegram.org");
    clientTCP.println("Content-Type: application/x-www-form-urlencoded");
    clientTCP.print("Content-Length: ");
    clientTCP.println(payload.length());
    clientTCP.println();

    // Stream the data payload
    clientTCP.print(payload);

    // Await server confirmation loop
    uint32_t timeout = millis();
    while (clientTCP.connected() && (millis() - timeout < 5000)) {
      while (clientTCP.available()) {
        String line = clientTCP.readStringUntil('\n');
        // Serial.println(line); // Uncomment if you want to see the "HTTP/1.1 200 OK" response
        timeout = millis();
      }
    }
    //Serial.println("Text alert processed.");
  } else {
   // Serial.println("Connection to Telegram failed for text alert.");
  }

  // Always force close the secure socket to free up heap memory slots
  clientTCP.stop();
}

void cyclic_motiondetectioncooldownreset()
{
	gbs.motiondetectionincooldown = 0;
}
