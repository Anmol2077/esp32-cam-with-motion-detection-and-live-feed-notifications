
#include "Arduino.h"
#include "1.hpp"

extern unsigned long globalcommandrequest;
extern String esplog;

extern void getpwtime(String *str);

void handlerestartrequest()
{
	if(gbs.esprestartrequested == 1)
	{
		ESP.restart();
	}
}

void cyclic_handleglobalcommandrequest()
{
	switch(globalcommandrequest)
	{
		case 0:
			break;

		case 1:
		{
			int val = digitalRead(GPIO_NUM_12);
			esplog = "Pin12 : " + String(val);
			break;
		}
		case 2:
		{
			String s = "";
			getpwtime(&s);
			esplog = "UP: " + s;
			break;
		}
		case 3:
		{
			int val = digitalRead(GPIO_NUM_13);
			esplog = "Pin13 : " + String(val);
			break;
		}
		case 4:
		{
			gbs.pirsensorisdisabled = 1;
			esplog = "PIR disabled";
			break;
		}
		case 5:
		{
			gbs.pirsensorisdisabled = 0;
			esplog = "PIR enabled";
			break;
		}
		default:
		{
			esplog = "INVALID COMMAND";
			break;
		}

	}

	globalcommandrequest = 0;
}
