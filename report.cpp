#include "1.hpp"

#define maxreports 30
extern const uint8 max_states;
extern unsigned long long ustime;
extern gbs_st gbs;

String sp_rep_buf = "";
extern String user_rep_buf;
String user_sd_rep_buf = "";
extern String fn_rep_buf;
String com_buff = "";

typedef struct{
	uint8 readst:2;
	uint8 viacom:1;
	uint8 viadisp:1;
	uint8 vialog:1;
	uint8 viadem:1;
	uint8 otherinfo:2;
}reportbuff_st;

typedef struct{
	uint8 currentrpid;
	uint8 maxedrpid;
	uint8 peindrpid;
	uint8 comrpstate;
	uint8 disprpstate;
	uint8 logrpstate;
	uint8 sdreadstate;
	uint8 currentdemid;
	reportbuff_st rpaddinfo[maxreports];
	uint32 demids[maxreports];
	uint32 pendingmsgid[maxreports];
	String user_buff[maxreports];
}repinfo_st;

repinfo_st repinfoglobal = {
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		{0},
		{0},
		{0},
		"",
};

void report(uint32 id,uint32 info)
{
	if((((info & 0x01) == 0x01) ||
		((info & 0x02) == 0x02) ||
		((info & 0x04) == 0x04)))
	{
		if(repinfoglobal.peindrpid < (maxreports -2 ))
		{
			repinfoglobal.pendingmsgid[repinfoglobal.peindrpid] = id;
			repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].viacom = (((info & 0x01) == 0x01)?1:0);
			repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].viadisp = (((info & 0x02) == 0x02)?1:0);
			repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].vialog = (((info & 0x04) == 0x04)?1:0);
			repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].otherinfo = ((info & 0x30)>>4);

			if(gbs.reportallcom == 1)
			{
				repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].viacom = 1;
			}
			if(gbs.reportalldisp == 1)
			{
				repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].viadisp = 1;
			}
			if(gbs.noreportingcom == 1)
			{
				repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].viacom = 0;
				repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].viadisp = 1;
			}
			if(gbs.noreportingdisp == 1)
			{
				repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].viacom = 1;
				repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].viadisp = 0;
			}
			if(gbs.noreportinglog == 1)
			{
				repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].vialog = 0;
			}

			if((repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].otherinfo == rpcomm))
			{
				if(gbs.disablecomlogging == 1)
				{
					repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].vialog = 0;
				}
			}

			if(gbs.extendedlog == 1)
			{
				repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].vialog = 1;
			}

			if((repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].otherinfo == rpnone) && (user_rep_buf != ""))
			{
				repinfoglobal.user_buff[repinfoglobal.peindrpid] = user_rep_buf;
				user_rep_buf = "";
			}
			else if((repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].otherinfo == rpdiag) && (fn_rep_buf != ""))
			{
				repinfoglobal.user_buff[repinfoglobal.peindrpid] = fn_rep_buf;
				fn_rep_buf = "";
			}
			else if((repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].otherinfo == rpcomm) && (com_buff != ""))
			{
				repinfoglobal.user_buff[repinfoglobal.peindrpid] = com_buff;
				com_buff = "";
			}
			else if((repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].otherinfo == rpspec) && (sp_rep_buf != ""))
			{
				repinfoglobal.user_buff[repinfoglobal.peindrpid] = sp_rep_buf;
				user_rep_buf = "";
			}
			else
			{
				if(id == 0)
				{
					repinfoglobal.pendingmsgid[repinfoglobal.peindrpid] = 1025;
					info = 0x08;
					id = 1025;
					repinfoglobal.user_buff[repinfoglobal.peindrpid] = "";
					repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].readst = e_sdpending;

					repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].viacom = 1;
					repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].viadisp = 0;
					repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].vialog = 1;
					repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].otherinfo = rpnone;
					user_rep_buf = "";
					fn_rep_buf = "";
					com_buff = "";
					sp_rep_buf = "";

				}
			}

			if(repinfoglobal.pendingmsgid[repinfoglobal.peindrpid] == 0)
			{
				repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].readst = e_sdcompleted;
			}
			else
			{
				repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].readst = e_sdpending;
			}

			++repinfoglobal.peindrpid;

			if(repinfoglobal.maxedrpid < repinfoglobal.peindrpid)
			{
				repinfoglobal.maxedrpid = repinfoglobal.peindrpid;
			}
		}
		else
		{
			repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].otherinfo = ((info & 0x30)>>4);

			switch(repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].otherinfo)
			{
			case rpnone:
				user_rep_buf = "";
				break;
			case rpdiag:
				fn_rep_buf = "";
				break;
			case rpcomm:
				com_buff = "";
				break;
			case rpspec:
				sp_rep_buf = "";
				break;
			}

			repinfoglobal.pendingmsgid[(repinfoglobal.peindrpid)] = 1026;
			info = 0x08;
			id = 1026;
			repinfoglobal.user_buff[(repinfoglobal.peindrpid)] = "";
			repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].readst = e_sdpending;

			repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].viacom = 1;
			repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].viadisp = 0;
			repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].vialog = 1;
			repinfoglobal.rpaddinfo[repinfoglobal.peindrpid].otherinfo = rpnone;

			if(repinfoglobal.peindrpid < (maxreports - 1))
			{
				++repinfoglobal.peindrpid;
			}

			if(repinfoglobal.maxedrpid < repinfoglobal.peindrpid)
			{
				repinfoglobal.maxedrpid = repinfoglobal.peindrpid;
			}
		}
	}
	if((info & 0x08) == 0x08)
	{
		repinfoglobal.demids[repinfoglobal.currentdemid] = id;

		if(repinfoglobal.currentdemid < (maxreports -1 ))
		{
			++repinfoglobal.currentdemid;
		}
		else
		{
			repinfoglobal.demids[(repinfoglobal.currentdemid +1)] = 2200;
		}

	}
}