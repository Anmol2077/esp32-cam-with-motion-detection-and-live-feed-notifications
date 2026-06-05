#ifndef HEADERS_STATES_H_
#define HEADERS_STATES_H_

#include <Arduino.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>


typedef unsigned long long uint64;
typedef unsigned long uint32;
typedef unsigned int uint8;
typedef bool boolean;

#define e_tpaused 0
#define e_tstarted 1
#define e_trunning 2
#define e_tcompleted 3

#define xcom  (uint8)0x01
#define xdisp (uint8)0x02
#define xlog  (uint8)0x04
#define xdem  (uint8)0x08

#define e_noid  0
#define e_notstarted  0
#define e_sdpending  1
#define e_sdcompleted  2
#define e_sdfailed  3

#define rpnone 0
#define rpdiag 1
#define rpcomm 2
#define rpspec 3

#define xinf  0x00
#define yinf  0x10
#define zinf  0x20
#define winf  0x30

#define xnon 0x00
#define xcyc 0x01
#define xrun 0x02

#define tnon 0x00
#define tdem 0x01
#define tlog 0x02
#define trep 0x03
#define tdel 0x04

#define xna  (uint8)0x00
#define xdg  (uint8)0x10
#define xcm  (uint8)0x20
#define xsp  (uint8)0x30

#define getmicrotime esp_timer_get_time()

#define xndel 0x00
#define xdel 0x01

#define  e_nofunction   0
#define  e_setup   1
#define  e_loop   2
#define  e_loop_0   3
#define  e_loop_1   4
#define  e_setup_0   5
#define  e_setup_1   6
#define  e_init_ioports   7
#define  e_init_power   8
#define  e_init_memory   9
#define  e_init_display   10
#define  e_init_com   11
#define  e_init_finalvalidation   12
#define  e_init_usercustomization   13
#define  e_rt_cyclic_call_core0   14
#define  e_rt_cyclic_call_core1   15
#define  e_rt_rtm1   16
#define  e_rt_rtm0   17
#define  e_rt1_task_2ms   18
#define  e_rt1_task_5ms   19
#define  e_rt1_task_10ms   20
#define  e_rt1_task_20ms   21
#define  e_rt1_task_50ms   22
#define  e_rt1_task_100ms   23
#define  e_rt1_task_200ms   24
#define  e_rt1_task_1s   25
#define  e_rt1_task_2s   26
#define  e_rt1_task_5s   27
#define  e_rt0_task_2ms   28
#define  e_rt0_task_5ms   29
#define  e_rt0_task_10ms   30
#define  e_rt0_task_20ms   31
#define  e_rt0_task_50ms   32
#define  e_rt0_task_100ms   33
#define  e_rt0_task_200ms   34
#define  e_rt0_task_1s   35
#define  e_rt0_task_2s   36
#define  e_rt0_task_5s   37
#define  e_report   38
#define  e_cyclic_rp_st_check   39
#define  e_on_diag_rqst   40
#define  e_cyclic_diagpfn   41
#define  e_once_initrng   42
#define  e_getrng32   43
#define  e_calkey   44
#define  e_rtev   45
#define  e_getcurrenttime   46
#define  e_getruntime   47
#define  e_cyclic_rpdisp   48
#define  e_cyclic_processbutton   49
#define  e_cyclic_on_onbtn_rqt   50
#define  e_process_btn_rqst   51
#define  e_cyclic_log   52
#define  e_cyclic_getsdrp   53
#define  e_ReadVoltage   54
#define  e_getdatetimeinfo   55
#define  e_getpwtime   56
#define  e_create_timer   57
#define  e_timer_expired   58
#define  e_start_timer   59
#define  e_delete_timer   60
#define  e_cyclic_timer   61
#define  e_cyclic_ntpclock   62
#define  e_cyclic_getntpclock   63
#define  e_once_getntpclock   64
#define  e_vpinMode   65
#define  e_adc_mapp   66
#define  e_pin_check   67
#define  e_vdigitalRead   68
#define  e_vanalogRead   69
#define  e_vdigitalWrite   70
#define  e_vanalogWrite   71
#define  e_setsecurity   72
#define  e_timer_created   73
#define  e_timer_started   74
#define  e_user_getsdrp   75
#define  e_cyclic_clientnalive 76
#define  e_init_com_signals 77
#define e_reset_mcu 78

#define getheap esp_get_free_heap_size()

void wifi_init();
void wifi_cyclic(); 
void firebase_cyc();
void camera_init_cyc();
void firebase_init_cyc();
void cctv_cyc();
void cyclic_diag();

void nofunction(uint8 *info,const uint32 *data);
void getcurrenttime(uint8 *info,const uint32 *data);
void getecuuptime(uint8 *info,const uint32 *data);
void report(uint32 id,uint32 info);

typedef struct{
	/* 0 */ uint32   setup0completed : 1;
	/* 1 */ uint32  setup1completed : 1;
	/* 2 */ uint32  initializationcompleted : 1;
	/* 3 */ uint32  rt0_task_2ms_running : 1;
	/* 4 */ uint32  rt0_task_5ms_running : 1;
	/* 5 */ uint32  rt0_task_10ms_running : 1;
	/* 6 */ uint32  rt0_task_20ms_running : 1;
	/* 7 */ uint32  rt0_task_50ms_running : 1;
	/* 8 */ uint32  rt0_task_100ms_running : 1;
	/* 9 */ uint32  rt0_task_200ms_running : 1;
	/* 10 */ uint32  rt0_task_1s_running : 1;
	/* 11 */ uint32  rt0_task_2s_running : 1;
	/* 12 */ uint32  rt0_task_5s_running : 1;
	/* 13 */ uint32  rt1_task_2ms_running : 1;
	/* 14 */ uint32  rt1_task_5ms_running : 1;
	/* 15 */ uint32  rt1_task_10ms_running : 1;
	/* 16 */ uint32  rt1_task_20ms_running : 1;
	/* 17 */ uint32  rt1_task_50ms_running : 1;
	/* 18 */ uint32  rt1_task_100ms_running : 1;
	/* 19 */ uint32  rt1_task_200ms_running : 1;
	/* 20 */ uint32  rt1_task_1s_running : 1;
	/* 21 */ uint32  rt1_task_2s_running : 1;
	/* 22 */ uint32  rt1_task_5s_running : 1;
	/* 23 */ uint32  display1initialized : 1;
	/* 24 */ uint32  display1connected : 1;
	/* 25 */ uint32  display1busy : 1;
	/* 26 */ uint32  gpexpander1intialized : 1;
	/* 27 */ uint32  gpexpander2intialized : 1;
	/* 28 */ uint32  gpexpander1connected : 1;
	/* 29 */ uint32  gpexpander2connected : 1;
	/* 30 */ uint32  gpexpander1busy : 1;
	/* 31 */ uint32  gpexpander2busy : 1;
	/* 32 */ uint32  wificonnected : 1;
	/* 33 */ uint32  wifibusy : 1;
	/* 34 */ uint32  anywificlientconnected : 1;
	/* 35 */ uint32  iswifion : 1;
	/* 36 */ uint32  espwifimodeisSTA : 1;
	/* 37 */ uint32  internetconnectedviawifi : 1;
	/* 38 */ uint32  i2cbusbusy : 1;
	/* 39 */ uint32  spibusbusy : 1;
	/* 40 */ uint32  sdcardinitialized : 1;
	/* 41 */ uint32  sdcardconnected : 1;
	/* 42 */ uint32  sdcardbusy : 1;
	/* 43 */ uint32  isloggingenabled : 1;
	/* 44 */ uint32  mcuinnormalmode : 1;
	/* 45 */ uint32  mcuinidlemode : 1;
	/* 46 */ uint32  mcuintempidlemode : 1;
	/* 47 */ uint32  powerconnected : 1;
	/* 48 */ uint32  cyclicerrordetected : 1;
	/* 49 */ uint32  currentlydaytime : 1;
	/* 50 */ uint32  button1state : 1;
	/* 51 */ uint32  button1prestate : 1;
	/* 52 */ uint32  button1pressed : 1;
	/* 53 */ uint32  button2state : 1;
	/* 54 */ uint32  button2prestate : 1;
	/* 55 */ uint32  button2pressed : 1;
	/* 56 */ uint32  button3state : 1;
	/* 57 */ uint32  button3prestate : 1;
	/* 58 */ uint32  button3pressed : 1;
	/* 59 */ uint32  gpioisinitialized : 1;
	/* 60 */ uint32  cyclicinternallogenabled : 1;
	/* 61 */ uint32  ntpclockupdated : 1;
	/* 62 */ uint32  core0_2ms_sintask : 1;
	/* 63 */ uint32  core0_5ms_sintask : 1;
	/* 64 */ uint32  core0_10ms_sintask : 1;
	/* 65 */ uint32  core0_20ms_sintask : 1;
	/* 66 */ uint32  core0_50ms_sintask : 1;
	/* 67 */ uint32  core0_100ms_sintask : 1;
	/* 68 */ uint32  core0_200ms_sintask : 1;
	/* 69 */ uint32  core0_1s_sintask : 1;
	/* 70 */ uint32  core0_2s_sintask : 1;
	/* 71 */ uint32  core0_5s_sintask : 1;
	/* 72 */ uint32  core1_2ms_sintask : 1;
	/* 73 */ uint32  core1_5ms_sintask : 1;
	/* 74 */ uint32  core1_10ms_sintask : 1;
	/* 75 */ uint32  core1_20ms_sintask : 1;
	/* 76 */ uint32  core1_50ms_sintask : 1;
	/* 77 */ uint32  core1_100ms_sintask : 1;
	/* 78 */ uint32  core1_200ms_sintask : 1;
	/* 79 */ uint32  core1_1s_sintask : 1;
	/* 80 */ uint32  core1_2s_sintask : 1;
	/* 81 */ uint32  core1_5s_sintask : 1;
	/* 82 */ uint32  extendedruntime : 1;
	/* 83 */ uint32  standruntime : 1;
	/* 84 */ uint32  wifienabled : 1;
	/* 85 */ uint32  bluetoothenabled : 1;
	/* 86 */ uint32  serialenabled : 1;
	/* 87 */ uint32  extendeddebug : 1;
	/* 88 */ uint32  wifistarted : 1;
	/* 89 */ uint32  basicsecurity : 1;
	/* 90 */ uint32  extendedsecurity : 1;
	/* 91 */ uint32  rngstarted : 1;
	/* 92 */ uint32  wifireadpending : 1;
	/* 93 */ uint32  websocketstarted : 1;
	/* 94 */ uint32  rt1_task_1m_running : 1;
	/* 95 */ uint32  rt1_task_2m_running : 1;
	/* 96 */ uint32  rt1_task_5m_running : 1;
	/* 97 */ uint32  rt1_task_10m_running : 1;
	/* 98 */ uint32  rt1_task_1h_running : 1;
	/* 99 */ uint32  core1_1m_sintask : 1;
	/* 100 */ uint32  core1_2m_sintask : 1;
	/* 101 */ uint32  core1_5m_sintask : 1;
	/* 102 */ uint32  core1_10m_sintask : 1;
	/* 103 */ uint32  core1_1h_sintask : 1;
	/* 104 */ uint32  fastdiagnostics : 1;
	/* 105 */ uint32  anyclientalive : 1;
	/* 106 */ uint32  extendedlog : 1;
	/* 107 */ uint32  reportallcom : 1;
	/* 108 */ uint32  reportalldisp : 1;
	/* 109 */ uint32  noreportingcom : 1;
	/* 110 */ uint32  noreportingdisp : 1;
	/* 111 */ uint32  noreportinglog : 1;
	/* 112 */ uint32  updateiofromsdrecord : 1;
	/* 113 */ uint32  disableallcomsignals : 1;
	/* 114 */ uint32  rt0_task_2ms_enabled : 1;
	/* 115 */ uint32  rt0_task_5ms_enabled : 1;
	/* 116 */ uint32  rt0_task_10ms_enabled : 1;
	/* 117 */ uint32  rt0_task_20ms_enabled : 1;
	/* 118 */ uint32  rt0_task_50ms_enabled : 1;
	/* 119 */ uint32  rt0_task_100ms_enabled : 1;
	/* 120 */ uint32  rt0_task_200ms_enabled : 1;
	/* 121 */ uint32  rt0_task_1s_enabled : 1;
	/* 122 */ uint32  rt0_task_2s_enabled : 1;
	/* 123 */ uint32  rt0_task_5s_enabled : 1;
	/* 124 */ uint32  rt1_task_2ms_enabled : 1;
	/* 125 */ uint32  rt1_task_5ms_enabled : 1;
	/* 126 */ uint32  rt1_task_10ms_enabled : 1;
	/* 127 */ uint32  rt1_task_20ms_enabled : 1;
	/* 128 */ uint32  rt1_task_50ms_enabled : 1;
	/* 129 */ uint32  rt1_task_100ms_enabled : 1;
	/* 130 */ uint32  rt1_task_200ms_enabled : 1;
	/* 131 */ uint32  rt1_task_1s_enabled : 1;
	/* 132 */ uint32  rt1_task_2s_enabled : 1;
	/* 133 */ uint32  rt1_task_5s_enabled : 1;
	/* 134 */ uint32  rt1_task_1m_enabled : 1;
	/* 135 */ uint32  rt1_task_2m_enabled : 1;
	/* 136 */ uint32  rt1_task_5m_enabled : 1;
	/* 137 */ uint32  rt1_task_10m_enabled : 1;
	/* 138 */ uint32  rt1_task_1h_enabled : 1;
	/* 139 */ uint32  fastdiagprocessenable : 1;
	/* 140 */ uint32  disablecomlogging : 1;
	/* 141 */ uint32  security1seedsent : 1;
	/* 142 */ uint32  disablecomtx : 1;
	/* 143 */ uint32  disablecomrx : 1;
	/* 144 */ uint32  delayedSWreset : 1;
	/* 145 */ uint32  delayedHWreset : 1;
	/* 146 */ uint32  specrequestpropending : 1;

	/* 147 */ uint32  wifioncereceived : 1;

  /* 148 */ uint32  cameraready : 1;
  /* 149 */ uint32  firebaseready : 1;
  /* 150 */ uint32  firebaseinit : 1;

  /* 151 */ uint32  motiondetected : 1;
  /* 152 */ uint32  core1setupdone : 1;
  /* 153 */ uint32  telegramstarted : 1;
  /* 154 */ uint32  telegramtrnsinprogress : 1;
  /* 155 */ uint32  cctvtransinprogress : 1;
  /* 156 */ uint32  telecameraneeded : 1;
  /* 157 */ uint32  telecameraready : 1;
  /* 158 */ uint32  currentdatetimereceived : 1;
  /* 159 */ uint32  internetisconnected : 1;
  /* 160 */ uint32  currentdatetimeinitialized : 1;
  /* 161 */ uint32  livecamfeedrunning : 1;
  /* 162 */ uint32  esprestartrequested : 1;
  /* 163 */ uint32  motiondetectionincooldown : 1;
  /* 164 */ uint32  motiondetectionstopped : 1;
  /* 165 */ uint32  pirsensorisready : 1;
  /* 166 */ uint32  pirsensorisdisabled : 1;
	uint32	 	empty166	 	:	 	1	;
	uint32	 	empty167	 	:	 	1	;
	uint32	 	empty168	 	:	 	1	;
	uint32	 	empty169	 	:	 	1	;
	uint32	 	empty170	 	:	 	1	;
	uint32	 	empty171	 	:	 	1	;
	uint32	 	empty172	 	:	 	1	;
	uint32	 	empty173	 	:	 	1	;
	uint32	 	empty174	 	:	 	1	;
	uint32	 	empty175	 	:	 	1	;
	uint32	 	empty176	 	:	 	1	;
	uint32	 	empty177	 	:	 	1	;
	uint32	 	empty178	 	:	 	1	;
	uint32	 	empty179	 	:	 	1	;
	uint32	 	empty180	 	:	 	1	;
	uint32	 	empty181	 	:	 	1	;
	uint32	 	empty182	 	:	 	1	;
	uint32	 	empty183	 	:	 	1	;
	uint32	 	empty184	 	:	 	1	;
	uint32	 	empty185	 	:	 	1	;
	uint32	 	empty186	 	:	 	1	;
	uint32	 	empty187	 	:	 	1	;
	uint32	 	empty188	 	:	 	1	;
	uint32	 	empty189	 	:	 	1	;
	uint32	 	empty190	 	:	 	1	;
	uint32	 	empty191	 	:	 	1	;
	uint32	 	empty192	 	:	 	1	;
	uint32	 	empty193	 	:	 	1	;
	uint32	 	empty194	 	:	 	1	;
	uint32	 	empty195	 	:	 	1	;
	uint32	 	empty196	 	:	 	1	;
	uint32	 	empty197	 	:	 	1	;
	uint32	 	empty198	 	:	 	1	;
	uint32	 	empty199	 	:	 	1	;
	uint32	 	empty200	 	:	 	1	;
	uint32	 	empty201	 	:	 	1	;
	uint32	 	empty202	 	:	 	1	;
	uint32	 	empty203	 	:	 	1	;
	uint32	 	empty204	 	:	 	1	;
	uint32	 	empty205	 	:	 	1	;
	uint32	 	empty206	 	:	 	1	;
	uint32	 	empty207	 	:	 	1	;
	uint32	 	empty208	 	:	 	1	;
	uint32	 	empty209	 	:	 	1	;
	uint32	 	empty210	 	:	 	1	;
	uint32	 	empty211	 	:	 	1	;
	uint32	 	empty212	 	:	 	1	;
	uint32	 	empty213	 	:	 	1	;
	uint32	 	empty214	 	:	 	1	;
	uint32	 	empty215	 	:	 	1	;
	uint32	 	empty216	 	:	 	1	;
	uint32	 	empty217	 	:	 	1	;
	uint32	 	empty218	 	:	 	1	;
	uint32	 	empty219	 	:	 	1	;
	uint32	 	empty220	 	:	 	1	;
	uint32	 	empty221	 	:	 	1	;
	uint32	 	empty222	 	:	 	1	;
	uint32	 	empty223	 	:	 	1	;
	uint32	 	empty224	 	:	 	1	;
	uint32	 	empty225	 	:	 	1	;
	uint32	 	empty226	 	:	 	1	;
	uint32	 	empty227	 	:	 	1	;
	uint32	 	empty228	 	:	 	1	;
	uint32	 	empty229	 	:	 	1	;
	uint32	 	empty230	 	:	 	1	;
	uint32	 	empty231	 	:	 	1	;
	uint32	 	empty232	 	:	 	1	;
	uint32	 	empty233	 	:	 	1	;
	uint32	 	empty234	 	:	 	1	;
	uint32	 	empty235	 	:	 	1	;
	uint32	 	empty236	 	:	 	1	;
	uint32	 	empty237	 	:	 	1	;
	uint32	 	empty238	 	:	 	1	;
	uint32	 	empty239	 	:	 	1	;
	uint32	 	empty240	 	:	 	1	;
	uint32	 	empty241	 	:	 	1	;
	uint32	 	empty242	 	:	 	1	;
	uint32	 	empty243	 	:	 	1	;
	uint32	 	empty244	 	:	 	1	;
	uint32	 	empty245	 	:	 	1	;
	uint32	 	empty246	 	:	 	1	;
	uint32	 	empty247	 	:	 	1	;
	uint32	 	empty248	 	:	 	1	;
	uint32	 	empty249	 	:	 	1	;
	uint32	 	empty250	 	:	 	1	;
	uint32	 	empty251	 	:	 	1	;
	uint32	 	empty252	 	:	 	1	;
	uint32	 	empty253	 	:	 	1	;
	uint32	 	empty254	 	:	 	1	;
	uint32	 	empty255	 	:	 	1	;
	uint32	 	empty256	 	:	 	1	;
	uint32	 	empty257	 	:	 	1	;
	uint32	 	empty258	 	:	 	1	;
	uint32	 	empty259	 	:	 	1	;
	uint32	 	empty260	 	:	 	1	;
	uint32	 	empty261	 	:	 	1	;
	uint32	 	empty262	 	:	 	1	;
	uint32	 	empty263	 	:	 	1	;
	uint32	 	empty264	 	:	 	1	;
	uint32	 	empty265	 	:	 	1	;
	uint32	 	empty266	 	:	 	1	;
	uint32	 	empty267	 	:	 	1	;
	uint32	 	empty268	 	:	 	1	;
	uint32	 	empty269	 	:	 	1	;
	uint32	 	empty270	 	:	 	1	;
	uint32	 	empty271	 	:	 	1	;
	uint32	 	empty272	 	:	 	1	;
	uint32	 	empty273	 	:	 	1	;
	uint32	 	empty274	 	:	 	1	;
	uint32	 	empty275	 	:	 	1	;
	uint32	 	empty276	 	:	 	1	;
	uint32	 	empty277	 	:	 	1	;
	uint32	 	empty278	 	:	 	1	;
	uint32	 	empty279	 	:	 	1	;
	uint32	 	empty280	 	:	 	1	;
	uint32	 	empty281	 	:	 	1	;
	uint32	 	empty282	 	:	 	1	;
	uint32	 	empty283	 	:	 	1	;
	uint32	 	empty284	 	:	 	1	;
	uint32	 	empty285	 	:	 	1	;
	uint32	 	empty286	 	:	 	1	;
	uint32	 	empty287	 	:	 	1	;
	uint32	 	empty288	 	:	 	1	;
	uint32	 	empty289	 	:	 	1	;
	uint32	 	empty290	 	:	 	1	;
	uint32	 	empty291	 	:	 	1	;
	uint32	 	empty292	 	:	 	1	;
	uint32	 	empty293	 	:	 	1	;
	uint32	 	empty294	 	:	 	1	;
	uint32	 	empty295	 	:	 	1	;
	uint32	 	empty296	 	:	 	1	;
	uint32	 	empty297	 	:	 	1	;
	uint32	 	empty298	 	:	 	1	;
	uint32	 	empty299	 	:	 	1	;
	uint32	 	empty300	 	:	 	1	;
	uint32	 	empty301	 	:	 	1	;
	uint32	 	empty302	 	:	 	1	;
	uint32	 	empty303	 	:	 	1	;
	uint32	 	empty304	 	:	 	1	;
	uint32	 	empty305	 	:	 	1	;
	uint32	 	empty306	 	:	 	1	;
	uint32	 	empty307	 	:	 	1	;
	uint32	 	empty308	 	:	 	1	;
	uint32	 	empty309	 	:	 	1	;
	uint32	 	empty310	 	:	 	1	;
	uint32	 	empty311	 	:	 	1	;
	uint32	 	empty312	 	:	 	1	;
	uint32	 	empty313	 	:	 	1	;
	uint32	 	empty314	 	:	 	1	;
	uint32	 	empty315	 	:	 	1	;
	uint32	 	empty316	 	:	 	1	;
	uint32	 	empty317	 	:	 	1	;
	uint32	 	empty318	 	:	 	1	;
	uint32	 	empty319	 	:	 	1	;
	uint32	 	empty320	 	:	 	1	;
	uint32	 	empty321	 	:	 	1	;
	uint32	 	empty322	 	:	 	1	;
	uint32	 	empty323	 	:	 	1	;
	uint32	 	empty324	 	:	 	1	;
	uint32	 	empty325	 	:	 	1	;
	uint32	 	empty326	 	:	 	1	;
	uint32	 	empty327	 	:	 	1	;
	uint32	 	empty328	 	:	 	1	;
	uint32	 	empty329	 	:	 	1	;
	uint32	 	empty330	 	:	 	1	;
	uint32	 	empty331	 	:	 	1	;
	uint32	 	empty332	 	:	 	1	;
	uint32	 	empty333	 	:	 	1	;
	uint32	 	empty334	 	:	 	1	;
	uint32	 	empty335	 	:	 	1	;
	uint32	 	empty336	 	:	 	1	;
	uint32	 	empty337	 	:	 	1	;
	uint32	 	empty338	 	:	 	1	;
	uint32	 	empty339	 	:	 	1	;
	uint32	 	empty340	 	:	 	1	;
	uint32	 	empty341	 	:	 	1	;
	uint32	 	empty342	 	:	 	1	;
	uint32	 	empty343	 	:	 	1	;
	uint32	 	empty344	 	:	 	1	;
	uint32	 	empty345	 	:	 	1	;
	uint32	 	empty346	 	:	 	1	;
	uint32	 	empty347	 	:	 	1	;
	uint32	 	empty348	 	:	 	1	;
	uint32	 	empty349	 	:	 	1	;
	uint32	 	empty350	 	:	 	1	;
	uint32	 	empty351	 	:	 	1	;
	uint32	 	empty352	 	:	 	1	;
	uint32	 	empty353	 	:	 	1	;
	uint32	 	empty354	 	:	 	1	;
	uint32	 	empty355	 	:	 	1	;
	uint32	 	empty356	 	:	 	1	;
	uint32	 	empty357	 	:	 	1	;
	uint32	 	empty358	 	:	 	1	;
	uint32	 	empty359	 	:	 	1	;
	uint32	 	empty360	 	:	 	1	;
	uint32	 	empty361	 	:	 	1	;
	uint32	 	empty362	 	:	 	1	;
	uint32	 	empty363	 	:	 	1	;
	uint32	 	empty364	 	:	 	1	;
	uint32	 	empty365	 	:	 	1	;
	uint32	 	empty366	 	:	 	1	;
	uint32	 	empty367	 	:	 	1	;
	uint32	 	empty368	 	:	 	1	;
	uint32	 	empty369	 	:	 	1	;
	uint32	 	empty370	 	:	 	1	;
	uint32	 	empty371	 	:	 	1	;
	uint32	 	empty372	 	:	 	1	;
	uint32	 	empty373	 	:	 	1	;
	uint32	 	empty374	 	:	 	1	;
	uint32	 	empty375	 	:	 	1	;
	uint32	 	empty376	 	:	 	1	;
	uint32	 	empty377	 	:	 	1	;
	uint32	 	empty378	 	:	 	1	;
	uint32	 	empty379	 	:	 	1	;
	uint32	 	empty380	 	:	 	1	;
	uint32	 	empty381	 	:	 	1	;
	uint32	 	empty382	 	:	 	1	;
	uint32	 	empty383	 	:	 	1	;
	uint32	 	empty384	 	:	 	1	;
	uint32	 	empty385	 	:	 	1	;
	uint32	 	empty386	 	:	 	1	;
	uint32	 	empty387	 	:	 	1	;
	uint32	 	empty388	 	:	 	1	;
	uint32	 	empty389	 	:	 	1	;
	uint32	 	empty390	 	:	 	1	;
	uint32	 	empty391	 	:	 	1	;
	uint32	 	empty392	 	:	 	1	;
	uint32	 	empty393	 	:	 	1	;
	uint32	 	empty394	 	:	 	1	;
	uint32	 	empty395	 	:	 	1	;
	uint32	 	empty396	 	:	 	1	;
	uint32	 	empty397	 	:	 	1	;
	uint32	 	empty398	 	:	 	1	;
	uint32	 	empty399	 	:	 	1	;
	uint32	 	empty400	 	:	 	1	;
	uint32	 	empty401	 	:	 	1	;
	uint32	 	empty402	 	:	 	1	;
	uint32	 	empty403	 	:	 	1	;
	uint32	 	empty404	 	:	 	1	;
	uint32	 	empty405	 	:	 	1	;
	uint32	 	empty406	 	:	 	1	;
	uint32	 	empty407	 	:	 	1	;
	uint32	 	empty408	 	:	 	1	;
	uint32	 	empty409	 	:	 	1	;
	uint32	 	empty410	 	:	 	1	;
	uint32	 	empty411	 	:	 	1	;
	uint32	 	empty412	 	:	 	1	;
	uint32	 	empty413	 	:	 	1	;
	uint32	 	empty414	 	:	 	1	;
	uint32	 	empty415	 	:	 	1	;
	uint32	 	empty416	 	:	 	1	;
	uint32	 	empty417	 	:	 	1	;
	uint32	 	empty418	 	:	 	1	;
	uint32	 	empty419	 	:	 	1	;
	uint32	 	empty420	 	:	 	1	;
	uint32	 	empty421	 	:	 	1	;
	uint32	 	empty422	 	:	 	1	;
	uint32	 	empty423	 	:	 	1	;
	uint32	 	empty424	 	:	 	1	;
	uint32	 	empty425	 	:	 	1	;
	uint32	 	empty426	 	:	 	1	;
	uint32	 	empty427	 	:	 	1	;
	uint32	 	empty428	 	:	 	1	;
	uint32	 	empty429	 	:	 	1	;
	uint32	 	empty430	 	:	 	1	;
	uint32	 	empty431	 	:	 	1	;
	uint32	 	empty432	 	:	 	1	;
	uint32	 	empty433	 	:	 	1	;
	uint32	 	empty434	 	:	 	1	;
	uint32	 	empty435	 	:	 	1	;
	uint32	 	empty436	 	:	 	1	;
	uint32	 	empty437	 	:	 	1	;
	uint32	 	empty438	 	:	 	1	;
	uint32	 	empty439	 	:	 	1	;
	uint32	 	empty440	 	:	 	1	;
	uint32	 	empty441	 	:	 	1	;
	uint32	 	empty442	 	:	 	1	;
	uint32	 	empty443	 	:	 	1	;
	uint32	 	empty444	 	:	 	1	;
	uint32	 	empty445	 	:	 	1	;
	uint32	 	empty446	 	:	 	1	;
	uint32	 	empty447	 	:	 	1	;
	uint32	 	empty448	 	:	 	1	;
	uint32	 	empty449	 	:	 	1	;
	uint32	 	empty450	 	:	 	1	;
	uint32	 	empty451	 	:	 	1	;
	uint32	 	empty452	 	:	 	1	;
	uint32	 	empty453	 	:	 	1	;
	uint32	 	empty454	 	:	 	1	;
	uint32	 	empty455	 	:	 	1	;
	uint32	 	empty456	 	:	 	1	;
	uint32	 	empty457	 	:	 	1	;
	uint32	 	empty458	 	:	 	1	;
	uint32	 	empty459	 	:	 	1	;
	uint32	 	empty460	 	:	 	1	;
	uint32	 	empty461	 	:	 	1	;
	uint32	 	empty462	 	:	 	1	;
	uint32	 	empty463	 	:	 	1	;
	uint32	 	empty464	 	:	 	1	;
	uint32	 	empty465	 	:	 	1	;
	uint32	 	empty466	 	:	 	1	;
	uint32	 	empty467	 	:	 	1	;
	uint32	 	empty468	 	:	 	1	;
	uint32	 	empty469	 	:	 	1	;
	uint32	 	empty470	 	:	 	1	;
	uint32	 	empty471	 	:	 	1	;
	uint32	 	empty472	 	:	 	1	;
	uint32	 	empty473	 	:	 	1	;
	uint32	 	empty474	 	:	 	1	;
	uint32	 	empty475	 	:	 	1	;
	uint32	 	empty476	 	:	 	1	;
	uint32	 	empty477	 	:	 	1	;
	uint32	 	empty478	 	:	 	1	;

}gbs_st;

const int FLASH_LED_PIN = 4;
const int PWM_CHANNEL = 4;    // Use channel 4 to avoid conflicts with camera internal timers
const int FREQUENCY = 5000;   // 5 kHz frequency prevents visible camera flickering
const int RESOLUTION = 8;     // 8-bit resolution gives a brightness scale of 0 to 255
extern gbs_st gbs;


#endif /* 1_HEADERS_STATES_H_ */
