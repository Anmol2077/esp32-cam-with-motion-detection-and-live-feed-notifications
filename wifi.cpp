/*************************************************************************************************************************************************/
#include "1.hpp"
/*************************************************************************************************************************************************/
extern gbs_st gbs;
/*************************************************************************************************************************************************/
#include <WiFi.h>
/*************************************************************************************************************************************************/
const char* ssid[] = {"ak2", "AKXPS2055", "ak", "iPhone"};
const char* password[] = {"Anm@rtbas@2050", "HeloAnmll_!espnw#$", "ANMOLHUTT211", "Anjali123"};
uint8 attempts = 0;
uint8 currentwifi = 0;
/*************************************************************************************************************************************************/
void wifi_init() 
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid[currentwifi], password[currentwifi]);
  Serial.printf("conect: %s\n", ssid[currentwifi]);
}
/*************************************************************************************************************************************************/
void wifi_cyclic()
{
  if(gbs.wificonnected == 1)
  {
    switch(WiFi.status())
		{
      case WL_CONNECTED:
        break;

      default:
      WiFi.disconnect();
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid[currentwifi], password[currentwifi]);
      gbs.wificonnected = 0;
      break;
    }
  }
  else
  {
    switch(WiFi.status())
		{
      case WL_CONNECTED:
        gbs.wificonnected = 1;
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
        attempts = 0u;
      break;

      default:
        ++attempts;
        Serial.println(".");
      break;
    }

    if(attempts > 5)
    {
      Serial.printf("fail: %s\n", ssid[currentwifi]);
      attempts = 0;
      ++currentwifi;
      if(currentwifi > 2u)
      {
        currentwifi = 0;
      }
      WiFi.disconnect();
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid[currentwifi], password[currentwifi]);
      Serial.printf(" conect: %s\n", ssid[currentwifi]);
    }
  }
}

String getWiFiStatusString() {
    // Check if the ESP32 is actually connected to a network
    if (WiFi.status() != WL_CONNECTED) {
        return "WiFi:X";
    }

    // Get the SSID (Network Name)
    String ssid = WiFi.SSID();

    // Get the RSSI (Signal Strength in dBm)
    int rssi = WiFi.RSSI();

    // Combine them into a single formatted string
    // Example output: "Connected to: MyHomeWiFi | Signal: -55 dBm"
    String statusReport = "Connected to: " + ssid + " | Signal: " + String(rssi) + " dBm";

    return statusReport;
}
/*************************************************************************************************************************************************/
