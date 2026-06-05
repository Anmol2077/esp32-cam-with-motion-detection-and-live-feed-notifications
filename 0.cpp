
#include "0.hpp"

unsigned long long ustime;
TaskHandle_t Core0handle;
TaskHandle_t Core1handle;

uint64 task_0time_0[MAX_RT_TASKS];
uint64 task_1time_1[MAX_RT_TASKS];

/*************************************************************************************************************************************************/
void rt_cyclic_call_core1()
{
	for(uint8 i = 0; i < MAX_RT_TASKS; i++)
	{
		switch((CURRENTTIME - task_1time_1[i]) >= task_period[i])
		{
			case 0:
				break;
			default:
				task_1time_1[i] = CURRENTTIME;
				(*rt_task_list_1[i].rt_task)();
				break;
		}
	}
}
/*************************************************************************************************************************************************/
void Core1loop(void *parameter1)
{
	while(1)
	{
		ustime=esp_timer_get_time();
		realtime1();
	}
}
/*************************************************************************************************************************************************/

void realtime0()
{
  rt_cyclic_call_core0();
}
/*************************************************************************************************************************************************/
void rt_cyclic_call_core0()
{
	for(uint8 i = 0; i < MAX_RT_TASKS; i++)
	{
		switch((CURRENTTIME - task_0time_0[i]) >= task_period[i])
		{
			case 0:
				break;
			default:
				task_0time_0[i] = CURRENTTIME;
				(*rt_task_list_0[i].rt_task)();
				break;
		}
	}
}
/*************************************************************************************************************************************************/

/*************************************************************************************************************************************************/
void Core0loop(void *parameter0)
{
	while(1)
	{
		realtime0();
		vTaskDelay(pdMS_TO_TICKS(1));
	}
}

/*************************************************************************************************************************************************/
void setup()
{
	Serial.begin(115200);
	xTaskCreatePinnedToCore(Core0loop, "Core0loop", 8192, NULL, 1, &Core0handle, 0);
	xTaskCreatePinnedToCore(Core1loop, "Core1loop", 8192, NULL, 1, &Core1handle, 1);
	vTaskDelete(NULL);
	Serial.println("setup ok");
}
/*************************************************************************************************************************************************/
void loop() {Serial.println("never ever");}
/*************************************************************************************************************************************************/
