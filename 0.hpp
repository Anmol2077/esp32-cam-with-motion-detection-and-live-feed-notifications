
#ifndef HEADERS_STATES_H1_
#define HEADERS_STATES_H1_

#include "1.hpp"

/*************************************************************************************************************************************************/

/*************************************************************************************************************************************************/
/* ========================================================================== */
/* Cyclic Task Runners                              */
/* ========================================================================== */
void rt1_task_2ms(void);
void rt1_task_5ms(void);
void rt1_task_10ms(void);
void rt1_task_20ms(void);
void rt1_task_50ms(void);
void rt1_task_100ms(void);
void rt1_task_200ms(void);
void rt1_task_1s(void);
void rt1_task_2s(void);
void rt1_task_5s(void);

void rt1_task_2ms_once(void);
void rt1_task_5ms_once(void);
void rt1_task_10ms_once(void);
void rt1_task_20ms_once(void);
void rt1_task_50ms_once(void);
void rt1_task_100ms_once(void);
void rt1_task_200ms_once(void);
void rt1_task_1s_once(void);
void rt1_task_2s_once(void);
void rt1_task_5s_once(void);

void rt0_task_2ms(void);
void rt0_task_5ms(void);
void rt0_task_10ms(void);
void rt0_task_20ms(void);
void rt0_task_50ms(void);
void rt0_task_100ms(void);
void rt0_task_200ms(void);
void rt0_task_1s(void);
void rt0_task_2s(void);
void rt0_task_5s(void);

void rt0_task_2ms_once(void);
void rt0_task_5ms_once(void);
void rt0_task_10ms_once(void);
void rt0_task_20ms_once(void);
void rt0_task_50ms_once(void);
void rt0_task_100ms_once(void);
void rt0_task_200ms_once(void);
void rt0_task_1s_once(void);
void rt0_task_2s_once(void);
void rt0_task_5s_once(void);

void realtime1(void);
void rt_cyclic_call_core1(void);
void rt_cyclic_call_core0(void);
void wifi_init(void);
void wifi_cyclic(void);

void cctv_cyc(void);
void cyclic_diag(void);
void firebase_init_cyc(void);
void firebase_cyc(void);
void camera_init_cyc(void);

void cyc_sendpictotelegram();
void transmitTelegramPhoto();
void cyclic_detectMotion();
void setup_core1();
void cyclic_init_telegram();
void telegram_preparecamera();
void telegram_releasecamera();
void camera_init2_cyc();
void cyclic_time_task_500ms(void);
int check_internet_connection(void);
void cyc_checkinternet();
void cyc_inittime();
void cyclic_update_time_1sec(void);
void read_web_controls_500ms();
void update_camera_status_2sec();
void send_response_log_1sec();
void handlerestartrequest();
String Photo2Base64();
void cyclic_motiondetectioncooldownreset();
String getWiFiStatusString();
void cyclic_handleglobalcommandrequest();
void cyclic_resetglobalcommandrequest();
void getpwtime(String *str);


// Define the custom time structure for your application
typedef struct {
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;
    bool     is_valid; // Tells other tasks if the time is actually accurate yet
} app_time_t;

app_time_t get_global_time(void);
/*************************************************************************************************************************************************/

#define CURRENTTIME ustime

/*************************************************************************************************************************************************/
typedef struct{
	void (* rt_task)(void);
} rttask_st;
const uint64 task_period[] = {
		2000,	/* 	rt_task_2ms	*/
		5000,	/* 	rt_task_5ms	*/
		10000,	/* 	rt_task_10ms	*/
		20000,	/* 	rt_task_20ms	*/
		50000,	/* 	rt_task_50ms	*/
		100000,	/* 	rt_task_100ms	*/
		200000,	/* 	rt_task_200ms	*/
		1000000,	/* 	rt_task_1s	*/
		2000000,	/* 	rt_task_2s	*/
		5000000,	/* 	rt_task_5s	*/
};
const uint8 MAX_RT_TASKS = sizeof(task_period)/sizeof(uint64);

const rttask_st rt_task_list_1[] = {
		&rt1_task_2ms,
		&rt1_task_5ms,
		&rt1_task_10ms,
		&rt1_task_20ms,
		&rt1_task_50ms,
		&rt1_task_100ms,
		&rt1_task_200ms,
		&rt1_task_1s,
		&rt1_task_2s,
		&rt1_task_5s,
};


const rttask_st rt_task_list_0[] = {
		&rt0_task_2ms,
		&rt0_task_5ms,
		&rt0_task_10ms,
		&rt0_task_20ms,
		&rt0_task_50ms,
		&rt0_task_100ms,
		&rt0_task_200ms,
		&rt0_task_1s,
		&rt0_task_2s,
		&rt0_task_5s,
};
/*************************************************************************************************************************************************/

/*************************************************************************************************************************************************/


#endif /* 1_HEADERS_STATES_H_ */
