#include "1.hpp"
extern gbs_st gbs;
struct tm timeinfo;
extern unsigned long long ustime;
#define currenttime ustime

#define e_diagpronone     0
#define e_diagpropending  1
#define e_diagprocomplete 2
#define e_diagrrreceived  3

#define e_diagst 0
#define e_checksecurity 1
#define e_getid 2
#define e_getdiagdata 3
#define e_custdiagchecks 4
#define e_securityseed 5
#define e_securitykey 6
#define e_diagfinal 7
#define e_diagupdatebuf 8

#define max_functions 20
#define MAX_MESSAGE_BUFFER 20
#define MAX_MESSAGE_BUFFER_FOR_COM 20
#define MAX_CYCLICERROR_BUFFER 10
#define MAX_WIFI 6
#define max_pwmchannels 6
#define max_adcchannels 6
#define max_clients 3
#define max_secattmps 4
#define max_timers 20
#define max_basicsecal 5

typedef struct{
	uint32 diagrqprostate:2;
	uint32 cycdiagrqstate:4;
	uint32 sprqprostate:2;
	uint32 cycsprqstate:4;
	uint32 reservedspec:10;
	uint32 reserveddiag:10;
}diagproinfo_st;

diagproinfo_st diagproinfo = {

};

String fn_rep_buf = "";
String wt_fn_rep_buf[max_functions] = {};
String wifi_read_data = "";
String user_rep_buf = "";

typedef struct{
	uint8 fninfo;
	uint8 current_id;
	uint8 pending_id;
	uint8 curwait_id;
	uint8 waiting_id;
	uint8 fnst;
	uint8 prefnst;
	uint8 fnstcnt;
	uint8 waiting_fnst[max_functions];
	uint32 waiting_func[max_functions];
	uint32 pending_func[max_functions];
	uint32 pending_data[max_functions];
}function_st;

typedef struct{
	uint32 seed;
	uint32 key;
	uint32 algo;
	uint8 attmp;
} security_st;

typedef struct{
	uint8 cyc:1;
	uint8 state:2;
	uint8 addinfo:5;
} timaddinfo_st;

typedef struct{
	timaddinfo_st addinfo;
	uint32 mdid;
	uint32 period;
	uint32 microtime;
}timer_st;

typedef struct{
	uint8 maxtimer;
	timer_st **mytimer;
}timers_st;

timers_st globtimer = {
		0,
		NULL,
};

security_st sec_d = {
		0xFFFFFFFF,
		0xFFFFFFFF,
		0,
		0,
};

#define e_start  0
#define e_s1    1
#define e_s2    2
#define e_s3    3
#define e_s4    4
#define e_s5    5
#define e_s6    6
#define e_s7    7
#define e_s8    8
#define e_s9    9
#define e_s10    10
#define e_s11    11
#define e_s12    12
#define e_s13    13
#define e_s14    14
#define e_s15    15
#define e_s16    16
#define e_s17    17
#define e_s18    18
#define e_s19    19
#define e_s20    20
#define e_s21    21
#define e_s22    22
#define e_s23    23
#define e_s24    24
#define e_s25    25
#define e_s26    26
#define e_s27    27
#define e_s28    28
#define e_s29    29
#define e_s30    30
#define e_s31    31
#define e_s32    32
#define e_s33    33
#define e_s34    34
#define e_s35    35
#define e_s36    36
#define e_s37    37
#define e_s38    38
#define e_s39    39
#define e_s40    40
#define e_s41    41
#define e_s42    42
#define e_s43    43
#define e_s44    44
#define e_s45    45
#define e_s46    46
#define e_s47    47
#define e_s48    48
#define e_s49    49
#define e_waiting   120
#define e_s121   121
#define e_done    253
#define e_completed  254
#define e_resetstate  255

typedef struct{
	uint32 securitytimer;
	uint32 blinktimer;
	uint32 swpwmtimer;
}timerpd_st;

timerpd_st cyctimes;

function_st fn_data_st = {
		0,
		0,
		0,
		0,
		0,
		e_start,
		e_start,
		0,
	   0,
	   0,
	   0,
	   0,
};

typedef struct{
	void (* diag_fn)(uint8 *, const uint32 *);
} diagfn_st;

const diagfn_st diag_fn_list[] = {
		&nofunction,
		&getcurrenttime,
		// &getruntime,
		&getecuuptime,//3
		// &setsecurity,
		// &activatesecurity,
		// &toggleextendedlog,//6
		// &setreportingmode,
		// &getseedforsec,
		// &readalldems,
		// &bitstatusinfo,//10
		// &comtogglerxtx,
		// &togglecomsignals,
};

void nofunction(uint8 *info,const uint32 *data)
{
	switch(fn_data_st.fnst)
	{
	case e_start:
		break;
	case e_s1:
		break;
	case e_s2:
		break;
	case e_s3:
		break;
	case e_s4:
		break;
	case e_s5:
		break;
	case e_s6:
		break;
	case e_s7:
		break;
	case e_done:
		fn_data_st.fnst = e_completed;
		break;
	case e_completed:
		break;
	}
}

String bufchr(uint8 t)
{
	switch(t)
	{
	case 0: return "\n";
	case 1: return " ";
	case 2: return " = ";
	case 3: return " - ";
	case 4: return " : ";
	case 5: return "::";
	case 6: return ",";
	case 7: return "@@";
	case 8: return "&&";
	case 9: return "$$";
	case 10: return "##";
	case 11: return "%%";
	case 12: return " id: ";
	case 13: return "us";
	case 14: return "SD init error";
	default: return "";
	}
}

void getpwtime(String *str)
{
	int64_t powerOnTime = currenttime;

	  int hours = powerOnTime / 3600000000LL;
	  int minutes = (powerOnTime % 3600000000LL) / 60000000LL;
	  int seconds = (powerOnTime % 60000000LL) / 1000000LL;

	  *str = String(hours) + ":" + String(minutes) + ":" + String(seconds);
}

uint8 getdatetimeinfo(String *str)
{
	uint8 st= 0;
	if(gbs.ntpclockupdated == 1)
	{
		// str = String(timeinfo.tm_mday) + "/" + String(timeinfo.tm_mon) + "/" + String(timeinfo.tm_year);
		// str += " - ";
		// str += String(timeinfo.tm_hour) + ":" + String(timeinfo.tm_min) + ":" + String(timeinfo.tm_sec);
		st = 1;

	}
	else
	{
		*str = "NTP clock Failed"+bufchr(3)+"ONTIME"+bufchr(3);
		String str1;
		getpwtime(&str1);
		*str += str1;
		*str += bufchr(3);
		str1 = "";
	}

	return st;
}

uint8 timer_expired(uint32 gbid,uint8 addinfo)
{
	uint8 st = 0;

	for(uint8 i = 0; i < globtimer.maxtimer; i++)
	{
		if(globtimer.mytimer[i]->mdid == gbid)
		{
			if(globtimer.mytimer[i]->addinfo.state == e_tcompleted)
			{
				if((addinfo & tdel) == tdel)
				{
					//dlogx("deleting timer ",gbid);
					free(globtimer.mytimer[i]);
					globtimer.mytimer[i] = NULL;
					--globtimer.maxtimer;

					for (uint8 j = i; j < globtimer.maxtimer; j++)
					{
						globtimer.mytimer[j] = globtimer.mytimer[j + 1];
					}

					globtimer.mytimer = (timer_st**)realloc(globtimer.mytimer, globtimer.maxtimer * sizeof(timer_st*));
				}

				st = 1;
			}
			break;
		}

		if(i == (globtimer.maxtimer - 1))
		{
			st = 2;
			if((addinfo&tdem) == tdem)
			{
			user_rep_buf = String(gbid);

			if((addinfo&tlog) == tlog)
			{
				report(1020,(uint8)(xdem|xlog));
			}
			else
			{
				report(1020,(uint8)(xdem));
			}
			}
		}
	}
	return st;
}

uint8 timer_created(uint32 gbid)
{
	uint8 st = 0;

	for (uint8 j = 0; j < globtimer.maxtimer; j++)
	{
		if(gbid == globtimer.mytimer[j]->mdid)
		{
			st = 1;
			break;
		}
	}

	return st;
}

uint8 create_timer(uint32 gbid, uint32 cyctime, uint8 addinfo)
{
	uint8 st = 1;

	timer_st** tempTimer = nullptr; // Temporary pointer to hold the reallocated memory

	for (uint8 j = 0; j < globtimer.maxtimer; j++)
	{
		if(gbid == globtimer.mytimer[j]->mdid)
		{
			if(globtimer.mytimer[j]->addinfo.state == e_tcompleted)
			{

			}
			st = 0;//timer already exists
		}
	}

	if(st == 1)
	{
		st = 0;

		if(globtimer.maxtimer < max_timers)
		{
			tempTimer = (timer_st**)calloc(1,sizeof(timer_st*));

			if(tempTimer != nullptr)
			{
				st = 1;
			}
			else
			{
				st = 5;
			}

			if(st == 1)
			{
				if(globtimer.maxtimer == 0)
				{
					globtimer.mytimer = (timer_st**)calloc(1,sizeof(timer_st*));

					if(globtimer.mytimer != nullptr)
					{
						st = 1;
					}
					else
					{
						st = 5;
					}
				}
				else
				{
					tempTimer = (timer_st**)realloc(globtimer.mytimer, (globtimer.maxtimer + 1) * sizeof(timer_st*));

					if(tempTimer != nullptr)
					{
						st = 1;
					}
					else
					{
						st = 5;
					}
				}
			}

			if((globtimer.mytimer != nullptr) && (tempTimer != nullptr) && (st == 1))
			{
				st = 0;
				globtimer.mytimer = tempTimer;

				globtimer.mytimer[globtimer.maxtimer] = (timer_st*)calloc(1,sizeof(timer_st));

				if (globtimer.mytimer[globtimer.maxtimer] != nullptr)
				{
					globtimer.mytimer[globtimer.maxtimer]->period = cyctime;
					globtimer.mytimer[globtimer.maxtimer]->mdid = gbid;
					globtimer.mytimer[globtimer.maxtimer]->addinfo.cyc = (((addinfo & xcyc) == xcyc)?1:0);

					if((((addinfo & xrun) == xrun)?1:0) == 1)
					{
						globtimer.mytimer[globtimer.maxtimer]->addinfo.state = e_trunning;

						globtimer.mytimer[globtimer.maxtimer]->microtime = (getmicrotime/1000);

					}
					else
					{
						globtimer.mytimer[globtimer.maxtimer]->addinfo.state = e_tstarted;
					}
					++globtimer.maxtimer;
					//dlogx("timer added ",gbid);
					st = 1;
				}
				else
				{
					free(globtimer.mytimer[globtimer.maxtimer]);
	                globtimer.mytimer = tempTimer;
	                st = 5;
				}
			}
			else
			{
				st = 5;
				free(tempTimer);
			}
		}
		else
		{
			user_rep_buf = String(gbid);

			report(1062,(uint8)(xdem|xlog));
			st = 0;
		}
	}

	if(st == 5)
	{
		st = 0;
		user_rep_buf =  "id : ";
		user_rep_buf += String(gbid);
		user_rep_buf += " | heap left:";
		user_rep_buf += String(getheap);

		report(1061,(uint8)(xdem|xlog|xcom));
	}

	return st;
}

void getecuuptime(uint8 *info,const uint32 *data)
{
	switch(fn_data_st.fnst)
	{
	case e_start:
		getpwtime(&fn_rep_buf);
		fn_data_st.fnst = e_s1;
		break;
	case e_s1:
		report(1015,xcom|xdisp|xdg);
		fn_data_st.fnst = e_completed;
		break;
	case e_completed:
		break;
	}
}

void getcurrenttime(uint8 *info,const uint32 *data)
{
	switch(fn_data_st.fnst)
	{
	case e_start:
		if(gbs.ntpclockupdated == 1)
		{
			fn_data_st.fnst = e_s1;
		}
		else
		{
			fn_data_st.fnst = e_s4;
		}
		break;
	case e_s1:
		report(1000,xcom|xdisp|xlog|xdg);
		fn_data_st.fnst = e_s2;
		break;
	case e_s2:
	{
		uint8 s = getdatetimeinfo(&fn_rep_buf);
		if(s == 1)
		{
			fn_data_st.fnst = e_s3;
		}
		else
		{
			fn_data_st.fnst = e_s4;
		}
		break;
	}
	case e_s3:
		report(0,xcom|xdisp|xlog|xdg);
		fn_data_st.fnst = e_completed;
		break;
	case e_s4:
		report(1000,xcom|xdisp|xlog|xdg);
		fn_data_st.fnst = e_completed;
		break;
	case e_s5:
		break;
	case e_s6:
		break;
	case e_s7:
		break;
	case e_done:
		fn_data_st.fnst = e_completed;
		break;
	case e_completed:
		break;
	}
}

uint32 getrng32()
{
	if(gbs.rngstarted == 1)
	{
		return random(0xFFFFFFFF);
	}
	else
	{
		if(gbs.wifistarted == 1)
		{
			randomSeed(esp_random());
			return random(0xFFFFFFFF);
		}
		else
		{
			return esp_random();
		}
		return (rand() % 100);
	}
}

const uint8 maxdiagfns = sizeof(diag_fn_list)/sizeof(diagfn_st);

boolean updbuff(String *str,uint8 sz)
{
	boolean st = 0;
	if ((*str).length() >= sz)
	{
		char* charArray = const_cast<char*>((*str).c_str());
		memmove(charArray, charArray + sz, (*str).length() - sz);
		// (*str).resize((*str).length() - sz);
		st = 1;
	}
	return st;
}

void cyclic_diag()
{
	static uint8 drpst = 0;

	switch(drpst)
	{
	case e_start:

		if(fn_data_st.current_id != 0)
		{
			drpst = e_s1;
		}
		else if(fn_data_st.waiting_id != 0)
		{
			drpst = e_s4;
		}
		break;
	case e_s1:
		(*diag_fn_list[fn_data_st.pending_func[fn_data_st.pending_id]].diag_fn)(&fn_data_st.fninfo, &fn_data_st.pending_data[fn_data_st.pending_id]);

		if(fn_data_st.fnst == e_completed)
		{
			fn_data_st.fnst = e_start;
			drpst = e_s2;
		}
		else if(fn_data_st.fnst == e_waiting)
		{
			if(fn_data_st.waiting_id < max_functions)
			{
				fn_data_st.waiting_func[fn_data_st.waiting_id] = fn_data_st.pending_func[fn_data_st.pending_id];
				fn_data_st.waiting_fnst[fn_data_st.waiting_id] = e_waiting;

				if(fn_rep_buf != "")
				{
					wt_fn_rep_buf[fn_data_st.waiting_id] += fn_rep_buf;
					fn_rep_buf = "";
				}

				++fn_data_st.waiting_id;
			}
			else
			{
				user_rep_buf = bufchr(12);

				user_rep_buf += String(fn_data_st.pending_data[fn_data_st.pending_id]);

				report(1046,xdem|xlog);
			}

			drpst = e_s2;
		}
		else
		{
			drpst = e_s1;
		}

		break;

	case e_s2:

		++fn_data_st.pending_id;

		if(fn_data_st.waiting_id != 0)
		{
			drpst = e_s4;
		}
		else
		{
			fn_data_st.fnst = e_start;
			drpst = e_s3;
		}
		break;

	case e_s4:
	{
		if(fn_data_st.curwait_id < fn_data_st.waiting_id)
		{
			if(fn_data_st.waiting_func[fn_data_st.curwait_id] != 0)
			{
				fn_data_st.fnst = fn_data_st.waiting_fnst[fn_data_st.curwait_id];

				fn_rep_buf = wt_fn_rep_buf[fn_data_st.curwait_id];
				wt_fn_rep_buf[fn_data_st.curwait_id] = "";

				(*diag_fn_list[fn_data_st.waiting_func[fn_data_st.curwait_id]].diag_fn)(&fn_data_st.fninfo, &fn_data_st.pending_data[fn_data_st.curwait_id]);

				if(fn_data_st.fnst == e_completed)
				{
					fn_data_st.waiting_func[fn_data_st.curwait_id] = 0;
					wt_fn_rep_buf[fn_data_st.curwait_id] = "";
					drpst = e_s8;
				}
				else
				{
					fn_data_st.waiting_fnst[fn_data_st.curwait_id] = fn_data_st.fnst;

					if(fn_rep_buf != "")
					{
						wt_fn_rep_buf[fn_data_st.curwait_id] += fn_rep_buf;
						fn_rep_buf = "";
					}

					//dlogx("diag wait end 1",0,0);
					drpst = e_s3;
				}

				fn_data_st.fnst = e_start;
			}

			++fn_data_st.curwait_id;
		}
		else
		{
			drpst = e_s5;
		}

		break;
	}
	case e_s5:

		fn_data_st.curwait_id = 0;
		fn_data_st.fnst = e_start;
		drpst = e_s3;

		break;

	case e_s8:

		if(fn_data_st.waiting_id > 1)
		{
			--fn_data_st.curwait_id;
			--fn_data_st.waiting_id;

			for(uint8 i = fn_data_st.curwait_id; i < fn_data_st.waiting_id; i++)
			{
				fn_data_st.waiting_func[i] = fn_data_st.waiting_func[i+1];
				fn_data_st.waiting_fnst[i] = fn_data_st.waiting_fnst[i+1];
				wt_fn_rep_buf[i] = wt_fn_rep_buf[i+1];
			}
		}
		else
		{
			fn_data_st.waiting_id = 0;
		}

		drpst = e_s4;

		break;

	case e_s3:

		if(fn_data_st.pending_id >= fn_data_st.current_id)
		{
			fn_data_st.pending_id = 0;
			fn_data_st.current_id = 0;
		}
		drpst = e_completed;
		break;

	case e_completed:
		drpst = e_resetstate;
		break;
	case e_resetstate:
		drpst = e_start;
		break;
	}
}

uint32 calkey(uint32 s,uint8 a)
{
	uint32 k = 0;
	if(s != 0xFFFF)
	{
		switch(a)
		{
		case 0:
		    for (int i = 0; i < 4; i++)
		    {
		        k <<= 8;
		        k |= (s & 0xFF) + 1;// Extract the least significant byte from num, add 1, and OR it with result
		        s >>= 8; // Shift num right by 8 bits
		    }
			break;
		case 1:
		    for (int i = 0; i < 4; i++)
		    {
		        k <<= 8;
		        k |= (s & 0xFF) - 1;// Extract the least significant byte from num, sub 1, and OR it with result
		        s >>= 8; // Shift num right by 8 bits
		    }
			break;
		case 2:
		    for (int i = 0; i < 4; i++)
		    {
		        k <<= 8;
		        k |= (s & 0xFF) + 2;// Extract the least significant byte from num, add 2, and OR it with result
		        s >>= 8; // Shift num right by 8 bits
		    }
			break;
		case 3:
		    for (int i = 0; i < 4; i++)
		    {
		    	k = (2*(s+k)) - 1;
		    }
			break;
		case 4:
		    	k = (2*s) + 1;
			break;
		default:
			k = s;
			break;
		}

	}
	return k;
}

void cyclic_diag_rqst()
{
	switch(diagproinfo.cycdiagrqstate)
	{
	case e_diagst:
		switch(diagproinfo.diagrqprostate)
		{
			case e_diagpronone:
			case e_diagprocomplete:
				break;
			default:
				diagproinfo.diagrqprostate = e_diagpropending;
				diagproinfo.cycdiagrqstate = e_getid;
				break;
		}
			break;

	case e_getid:
	{
		if(fn_data_st.current_id < max_functions)
		{
			uint8 i = 0;
			uint8 st = 1;
			String s = "";
			String d = "";

			while((i<6))
			{
				if((wifi_read_data[i] == ',') || (wifi_read_data[i] == '$')){break;}

				if(!isdigit(wifi_read_data[i]))
				{
					user_rep_buf = bufchr(4) + String(wifi_read_data[i]);
					report(1078,xdem|xcom|xlog);
					st = 0;
					break;
				}
				else
				{
					s += wifi_read_data[i];
				}

				++i;
			}

			if(wifi_read_data[i] != '$')
			{
				++i;
			}

			(void)updbuff(&wifi_read_data,i);

			if((s == "") || (st == 0))
			{
				diagproinfo.cycdiagrqstate = e_diagupdatebuf;
			}
			else
			{
				fn_data_st.pending_func[fn_data_st.current_id] = atoi(s.c_str());

				if(fn_data_st.pending_func[fn_data_st.current_id] < maxdiagfns)
				{
					diagproinfo.cycdiagrqstate = e_getdiagdata;
				}
				else
				{
					diagproinfo.cycdiagrqstate = e_diagupdatebuf;
					report(1019,xcom);
				}

				//dlogx("diag id final ",fn_data_st.pending_func[fn_data_st.current_id],0);
			}
		}
		else
		{
			diagproinfo.cycdiagrqstate = e_diagupdatebuf;
			report(1003,xcom);
		}
	}
		break;
	case e_getdiagdata:
	{
		uint8 i = 0;
		String d = "";
		uint8 st = 1;

		//dlogx("diag data pre",wifi_read_data,0);

		if((wifi_read_data[i] == '$') || (wifi_read_data[i] == ','))
		{
			d = '0';
		}
		else
		{
			while(i<9)
			{
				if((wifi_read_data[i] == '$')){break;}

				if(!isdigit(wifi_read_data[i]))
				{
					user_rep_buf = bufchr(4) + String(wifi_read_data[i]);
					report(1080,xdem|xcom|xlog);
					d = '0';
					st = 2;
					break;
				}
				else
				{
					d += wifi_read_data[i];
				}

				++i;
			}
		}

		++i;
		(void)updbuff(&wifi_read_data,i);

		//dlogx("diag data post wifi",wifi_read_data,0);
		//dlogx("diag data post",d,0);

		fn_data_st.pending_data[fn_data_st.current_id] = strtoul(d.c_str(), nullptr, 10);

		if(st != 2)
		{
			diagproinfo.cycdiagrqstate = e_checksecurity;
		}
		else
		{
			diagproinfo.cycdiagrqstate = e_diagupdatebuf;
		}

	}
		break;

		case e_checksecurity:

			if(gbs.basicsecurity == 0)
			{
				diagproinfo.cycdiagrqstate = e_custdiagchecks;
			}
			else
			{
				if(sec_d.attmp < max_secattmps)
				{
					if(fn_data_st.pending_func[fn_data_st.current_id] == 5)
					{
						if(gbs.security1seedsent == 0)
						{
							report(1065,xcom);
							diagproinfo.cycdiagrqstate = e_diagst;
							diagproinfo.diagrqprostate = e_diagprocomplete;
						}
						else
						{
							diagproinfo.cycdiagrqstate = e_securitykey;
						}
					}
					else if(fn_data_st.pending_func[fn_data_st.current_id] == 8)
					{
						diagproinfo.cycdiagrqstate = e_securityseed;
					}
					else
					{
						report(1001,xcom|xdisp);
						//dlogx("security detect1",gbs.basicsecurity,0);
						//dlogx("security detect1 dat",wifi_read_data,0);
						diagproinfo.cycdiagrqstate = e_diagst;
						diagproinfo.diagrqprostate = e_diagprocomplete;
					}
				}
				else
				{
					report(1022,xcom);

					if(timer_created(e_on_diag_rqst) == 0)
					{
						create_timer(e_on_diag_rqst,cyctimes.securitytimer,xrun);
					}
					else
					{
						if(timer_expired(e_on_diag_rqst,tdel) == 1)
						{
							sec_d.attmp = 0;
							report(1008,xcom|xdisp);
						}
						else
						{
							report(1002,xcom);
						}
					}

					diagproinfo.cycdiagrqstate = e_diagst;
				}
			}
			break;

		case e_securitykey:

			sec_d.key = calkey(sec_d.seed,sec_d.algo);
			gbs.security1seedsent = 0;

			//dlogx("server key ",sec_d.key);

			if(sec_d.key == fn_data_st.pending_data[fn_data_st.current_id])
			{
				gbs.basicsecurity = 0;
				report(1004,xcom|xdisp|xlog);
			}
			else
			{
				report(1005,xcom|xdisp);

				user_rep_buf = String((max_secattmps - sec_d.attmp));

				report(1006,xcom|xdisp);

				if(sec_d.attmp < max_secattmps)
				{
					++sec_d.attmp;
				}
				if(sec_d.attmp >= max_secattmps)
				{
					++sec_d.algo;
					if(sec_d.algo > 4)
					{
						sec_d.algo = 0;
					}

					create_timer(e_on_diag_rqst,cyctimes.securitytimer,xrun);
				}
			}

			diagproinfo.cycdiagrqstate = e_diagst;
			diagproinfo.diagrqprostate = e_diagprocomplete;
			break;

		case e_securityseed:
			sec_d.seed = getrng32();
			user_rep_buf = String(sec_d.seed);
			report(1007,xcom|xdisp);
			diagproinfo.cycdiagrqstate = e_diagst;
			diagproinfo.diagrqprostate = e_diagprocomplete;
			gbs.security1seedsent = 1;
			//dlogx("server key 1",calkey(sec_d.seed,sec_d.algo));
			break;

		case e_custdiagchecks:
		{
			uint8 wtst = 0;
			if(fn_data_st.waiting_id != 0)
			{
				for(uint8 i = 0; i < fn_data_st.waiting_id; i++)
				{
					if(fn_data_st.waiting_func[i] == fn_data_st.pending_func[fn_data_st.current_id])
					{
						user_rep_buf = bufchr(12) + String(fn_data_st.pending_func[fn_data_st.current_id]);
						report(1048,xdem|xlog|xcom);
						fn_data_st.pending_func[fn_data_st.current_id] = 0;
						fn_data_st.pending_data[fn_data_st.current_id] = 0;
						diagproinfo.cycdiagrqstate = e_diagst;
						diagproinfo.diagrqprostate = e_diagprocomplete;
						wtst = 1;
						break;
					}
				}
				if(wtst == 0)
				{
					//dlogx("wait fn not match",1,0);
					diagproinfo.cycdiagrqstate = e_diagfinal;
				}
			}
			else
			{
				diagproinfo.cycdiagrqstate = e_diagfinal;
			}
		}
			break;

		case e_diagfinal:
			++fn_data_st.current_id;
			diagproinfo.cycdiagrqstate = e_diagst;
			diagproinfo.diagrqprostate = e_diagprocomplete;
			break;
		case e_diagupdatebuf:
		{
			uint8 i = 0;
			uint8 st = 1;

			while((i < 200) && (st == 1))
			{
				if((wifi_read_data[i] == '$'))
				{
					st = 2;
					break;
				}
				else
				{
					++i;
				}
			}

			//dlogx("received buffer on e_diagupdatebuf pre",wifi_read_data,0);

			if(st == 2)
			{
				(void)updbuff(&wifi_read_data,++i);
			}
			else if(st == 1)
			{
				user_rep_buf = wifi_read_data;
				report(1079,xdem|xcom|xlog);
				wifi_read_data = "";
			}

			//dlogx("updated buffer on e_diagupdatebuf ",wifi_read_data,0);

			diagproinfo.cycdiagrqstate = e_diagst;
			diagproinfo.diagrqprostate = e_diagprocomplete;
		}
			break;
		default:
			diagproinfo.cycdiagrqstate = e_diagst;
			diagproinfo.diagrqprostate = e_diagprocomplete;
			break;
	}
}
