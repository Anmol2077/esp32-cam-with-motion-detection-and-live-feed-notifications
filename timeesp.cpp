#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "1.hpp"
#include <string.h>
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "esp_sntp.h"

#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "timeesp.hpp"

extern gbs_st gbs;

// Define the globals



app_time_t g_system_time = {0};
SemaphoreHandle_t g_time_mutex = NULL;
void initialize_time(void);
void init_global_time_module(void);
int check_internet_connection(void);


void init_global_time_module(void);
void cyclic_update_time_1sec(void);
app_time_t get_global_time(void);





void cyc_inittime()
{
	if((gbs.wificonnected == 1) && (gbs.wificonnected == 1) && (gbs.currentdatetimeinitialized == 0))
	{
		initialize_time();
		init_global_time_module();
		gbs.currentdatetimeinitialized = 1;
	}
}

void initialize_time(void) {
    // Initialize SNTP
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_init();

    // Set timezone to IST (UTC +5:30)
    setenv("TZ", "IST-5:30", 1);
    tzset();

    // Note: In a production environment, you would typically wait here
    // for the time to be set by checking sntp_get_sync_status()
}


void cyclic_time_task_500ms(void) {
    // State 1: Awaiting Synchronization
    if (gbs.currentdatetimereceived == 0) {
        // Non-blocking check of the SNTP subsystem
        if (sntp_get_sync_status() == SNTP_SYNC_STATUS_COMPLETED) {
            gbs.currentdatetimereceived = 1;
            printf("NTP done!\n");
        } else {
        		gbs.currentdatetimereceived = 0;
            // Still syncing. Exit immediately so other cyclic tasks can run.
            return;
        }
    }

    // State 2: Time is Synchronized (Normal Operation)
    if (gbs.currentdatetimereceived == 1) {
        time_t now;
        struct tm timeinfo;
        char time_str[64];

        // Fetch and format the time
        time(&now);
        localtime_r(&now, &timeinfo);
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &timeinfo);

        // Do something with the time (e.g., update an Edge AI payload or log)
        // printf("Current IST Time: %s\n", time_str);
    }
}

void cyc_checkinternet()
{
	if(gbs.wificonnected == 1)
	{
		if(1 == check_internet_connection())
		{
			gbs.internetisconnected = 1;
		}
		else
		{
			gbs.internetisconnected = 0;
		}
	}
}


// Returns 1 if internet is active, 0 if no internet
int check_internet_connection(void) {
    int sock;
    struct sockaddr_in dest_addr;

    // Target Google's highly reliable Public DNS server
    dest_addr.sin_addr.s_addr = inet_addr("8.8.8.8");
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(53); // Port 53 is used for DNS

    // Create a TCP socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock < 0) {
        return 0; // Failed to create socket
    }

    // CRITICAL: Set a timeout. We don't want this function to block your
    // scheduler indefinitely if the network is dropping packets.
    struct timeval timeout;
    timeout.tv_sec = 2;  // 2-second timeout
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

    // Attempt to connect to the server
    int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

    // Always clean up the socket to prevent memory/file descriptor leaks
    close(sock);

    if (err == 0) {
        return 1; // Connection successful: Internet is active!
    } else {
        return 0; // Connection failed: Connected to Wi-Fi, but no internet routing.
    }
}



void init_global_time_module(void) {
    g_time_mutex = xSemaphoreCreateMutex();
}

// ---------------------------------------------------------
// Call this cyclically from your 1-second task/scheduler
// ---------------------------------------------------------
void cyclic_update_time_1sec(void) {
    // Only update if NTP has successfully acquired the time at least once
    if (gbs.currentdatetimereceived == 0) {
        return;
    }

    time_t now;
    struct tm timeinfo;

    // 1. Get the current system time (automatically maintained by ESP32 internal hardware)
    time(&now);

    // 2. Convert to broken-down local time (handles timezones, leap years, month lengths)
    localtime_r(&now, &timeinfo);

    // 3. Thread-safely update the global custom structure
    if (xSemaphoreTake(g_time_mutex, pdMS_TO_TICKS(5)) == pdTRUE) {
        g_system_time.year     = timeinfo.tm_year + 1900; // tm_year is years since 1900
        g_system_time.month    = timeinfo.tm_mon + 1;     // tm_mon is 0-11
        g_system_time.day      = timeinfo.tm_mday;
        g_system_time.hour     = timeinfo.tm_hour;
        g_system_time.minute   = timeinfo.tm_min;
        g_system_time.second   = timeinfo.tm_sec;
        g_system_time.is_valid = true;

        xSemaphoreGive(g_time_mutex);
    }
}

// ---------------------------------------------------------
// Thread-safe getter function for other tasks to consume time
// ---------------------------------------------------------
app_time_t get_global_time(void) {
    app_time_t time_copy = {0};

    if (g_time_mutex != NULL) {
        if (xSemaphoreTake(g_time_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            time_copy = g_system_time;
            xSemaphoreGive(g_time_mutex);
        }
    }
    return time_copy;
}


