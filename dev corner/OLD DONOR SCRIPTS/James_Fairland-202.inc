/*
	@Author; Calvin Catt
	http://www.gta-sarp.com/forums/showthread.php?282034-Development-Related-James_Fairland-s-Request-Calvin
	
	Player: James Fairland
	User ID: 202
	
	Extra requests: a safe and bar - /createsafe, /addbar
	Door lock code is within /enter
*/


#define JAMES_FAIRLAND_DOORID	927	// Link dynamic door... We'll need to get that ID too.
#define JAMES_FAIRLAND_ID		202 // James_Fairland's MySQL ID
new JAMES_FAIRLAND_LOCKED = true;	// Lock his door by default
new JAMES_FAIRLAND_MSG[128] = "Welcome. Enjoy your stay!";

CMD:jfalarm(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == JAMES_FAIRLAND_ID)
	{
		if(IsPlayerRestricted(playerid)) return SendClientMessage(playerid, COLOR_GRAD1, "You can not do this at this time.");
		SendClientMessage(playerid, COLOR_YELLOW, "Alarm activated, a police officer should be with you shortly.");
		new alarmMsg[128];
		if(callerID[playerid] == false) format(alarmMsg, sizeof(alarmMsg), "911 Call - Caller: %s", PlayerICName(playerid));
		else format(alarmMsg, sizeof(alarmMsg), "911 Call - Caller: Unknown (Blocked Caller ID)");
		SendCopMessage(COLOR_DBLUE, alarmMsg);
		format(alarmMsg, sizeof(alarmMsg), "Alarm: Panic button activated, Suspect: Unknown", CallDescription[playerid]);
		SendCopMessage(COLOR_DBLUE, alarmMsg);
		Called911[playerid] = 1;
		SetAllCopCheckpoint(playerid);
		return 1;
	}
	return AdmErrorMsg;
}

CMD:jflock(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == JAMES_FAIRLAND_ID)
	{
		if(IsPlayerRestricted(playerid)) return SendClientMessage(playerid, COLOR_GRAD1, "You can not do this at this time.");
		if(JAMES_FAIRLAND_LOCKED)
		{
			JAMES_FAIRLAND_LOCKED = false;
			SendClientMessage(playerid, COLOR_GRAD1, "The door has been unlocked.");
		}
		else {
			JAMES_FAIRLAND_LOCKED = true;
			SendClientMessage(playerid, COLOR_GRAD1, "The door has been locked.");
		}
		return 1;
	}
	return AdmErrorMsg;
}

CMD:jfmessage(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == JAMES_FAIRLAND_ID)
	{
		new msg[256];
		if(!sscanf(params, "s[128]", msg))
		{
			format(JAMES_FAIRLAND_MSG, sizeof(JAMES_FAIRLAND_MSG), "%s", msg);
			format(msg, sizeof(msg), "Sucessfully updated message: %s", JAMES_FAIRLAND_MSG);
			return 1;
		}
		else SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /jfmessage [message]");
	}
	return AdmErrorMsg;
}
