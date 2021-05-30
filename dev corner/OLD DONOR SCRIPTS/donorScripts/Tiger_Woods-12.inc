/*
 ______                            
/\__  _\__                         
\/_/\ \/\_\     __      __   _ __  
   \ \ \/\ \  /'_ `\  /'__`\/\`'__\
    \ \ \ \ \/\ \L\ \/\  __/\ \ \/ 
     \ \_\ \_\ \____ \ \____\\ \_\ 
      \/_/\/_/\/___L\ \/____/ \/_/ 
                /\____/            
                \_/__/             
 __      __                      __            
/\ \  __/\ \                    /\ \           
\ \ \/\ \ \ \    ___     ___    \_\ \    ____  
 \ \ \ \ \ \ \  / __`\  / __`\  /'_` \  /',__\ 
  \ \ \_/ \_\ \/\ \L\ \/\ \L\ \/\ \L\ \/\__, `\
   \ `\___x___/\ \____/\ \____/\ \___,_\/\____/
    '\/__//__/  \/___/  \/___/  \/__,_ /\/___/ 

	
	UNKNOWN DEVELOPMENT RERQUEST
*/
	
new Float:CamPos[][] = { //Use this for any cameras!
	//campos x, camposy, camposz,     lookat x, lookaty, lookatz
	{145.0163, 1713.7521, 10018.9102, 144.5235, 1712.8848, 10018.6309},//1
	{145.0163, 1713.7521, 10018.9102, 144.0274, 1713.6217, 10018.7773},//2
	{145.0163, 1713.7521, 10018.9102, 145.3284, 1712.8058, 10018.2354},//3
	{133.9780, 1689.6569, 10016.7910, 133.3040, 1690.3929, 10014.9434},//4
	{133.9780, 1689.6569, 10016.7910, 133.1417, 1690.2018, 10016.7578},//5
	{133.9780, 1689.6569, 10016.7910, 133.7589, 1690.6305, 10016.6045},//6
	{143.7413, 1715.5237, 10015.3135, 142.9722, 1714.8876, 10014.9990},//7
	{124.4656, 1715.4156, 10015.8457, 125.1684, 1714.7078, 10015.4580},//8
	{119.8937, 1705.6431, 10015.9287, 120.4830, 1704.8304, 10015.4355},//9
	{149.5217, 1705.3497, 10018.4609, 148.9465, 1706.1708, 10017.8438},//10
	{149.5351, 1724.7557, 10018.5000, 149.0394, 1723.8851, 10018.2207},//11
	{149.5351, 1724.7557, 10018.5000, 148.5678, 1724.4951, 10018.0928},//12
	{1493.2236, 2860.3435, 16.3206, 1514.9635 ,2864.2524, 12.0862},//13
	{1460.5276, 2860.5461, 16.1168, 1477.8832, 2867.5828, 10.8431},//14#
	{1458.0160, 2831.5244, 19.5481, 1454.6670, 2818.0222, 11.2895},//15
	{1420.2155, 2797.7837, 19.6450, 1301.0417, 2783.8418, 10.8203},//16
	{1524.7358, 2762.8127, 17.6711, 1492.7655, 2773.2468, 10.8203},//17
	{1524.9965, 2783.9773, 17.5221, 1540.1783, 2754.3232, 10.6719},//18
	{1482.4075, 2777.2642, 20.5517, 1458.5457, 2773.4531, 10.8203},//19
	{1413.9020, 2777.9248, 17.8448, 1424.7291, 2771.9312, 14.8203},//20
	{1461.2344, 2847.0186, 13.1862, 1477.2556, 2854.8833, 10.8459}//21
};

new bool:tbss = false;
new tEChair = INVALID_PLAYER_ID;
new bool:inchair[MAX_PLAYERS];
new twShelf;
new pressTimer;
new twCircle;
new Text:blood;

forward BloodHide(playerid);
public BloodHide(playerid) {
    TextDrawHideForPlayer(playerid, blood);
	return 1;
}

Hook:TigerWoods_OnGameModeInit() {
	twCircle = CreateDynamicCircle(1477.7290,2853.6323, 55.0);
	blood = TextDrawCreate(306.000000, -2.000000, ".");
	TextDrawAlignment(blood, 2);
	TextDrawBackgroundColor(blood, 255);
	TextDrawFont(blood, 1);
	TextDrawLetterSize(blood, 0.889999, 51.199993);
	TextDrawColor(blood, -1);
	TextDrawSetOutline(blood, 0);
	TextDrawSetProportional(blood, 1);
	TextDrawSetShadow(blood, 1);
	TextDrawUseBox(blood, 1);
	TextDrawBoxColor(blood, -1207959526);
	TextDrawTextSize(blood, 29.000000, 664.000000);

    //    BOOKSHELF TIGER
   	twShelf = CreateDynamicObject(1742,145.973022,1712.458984,10016.094726,0.000000,0.000000,-90.700004); // closed
	return 1;
}

Hook:TigerWoods_OnPlayerConnect(playerid) {
	inchair[playerid] = false;
	return 1;
}

/*Hook:TigerWoods_OnPlayerDeath(playerid, killerid, reason) {
	if(tEChair == playerid)
    {
	 	TextDrawHideForPlayer(tEChair, tFlash[0]);
		TextDrawHideForPlayer(tEChair, tFlash[1]);
		TextDrawHideForPlayer(tEChair, tFlash[2]);
		TextDrawHideForPlayer(tEChair, tFlash[3]);
		tEChair = INVALID_PLAYER_ID;
		
  		if(twTimer) {
			KillTimer(twTimer);
			twTimer = 0;
		}
	}

	inChair[playerid] = false;
	return 1;
}*/

CMD:twmagic(playerid, params[]) {
	new playername[MAX_PLAYER_NAME];
	GetPlayerName(playerid, playername, MAX_PLAYER_NAME);
	if(IsPlayerInRangeOfPoint(playerid, 3.0, 145.6084, 1711.9868, 10016.0996)) {
		if(tbss == false) { // opened
			MoveDynamicObject(twShelf, 145.955444, 1711.018798,10016.094726, 2.0);
			tbss = true;
			format(donorstr, sizeof(donorstr), "%s searches the books shelf for his favorite book.", playername);
			ProxDetector(30.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		}
		else if(tbss == true) { // closed
			MoveDynamicObject(twShelf, 145.973022,1712.458984,10016.094726, 2.0);
			tbss = false;
		}
		return 1;
	}
	if(IsPlayerInRangeOfPoint(playerid, 2.0, 144.1367, 1721.3069, 10017.6279) && ((gettime() - pressTimer) >= 5)) {
   		ApplyAnimation(playerid, "HEIST9", "Use_SwipeCard", 10.0, 0, 0, 0, 0, 0);
	 	new Float:pH;
	  	GetPlayerHealth(tEChair, pH);
	  	SetPlayerHealth(tEChair, pH-20);
	  	TextDrawShowForPlayer(tEChair, blood);
	  	SetTimerEx("BloodHide", 4999, false, "i", tEChair);
	  	pressTimer = gettime();
	  	format(donorstr, sizeof(donorstr), "%s presses the switch.", playername);
	 	ProxDetector(30.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
   	}
   	else return SendClientMessage(playerid, -1, "It looks like you didn't wait 5 seconds to use the button again or you are not near it!");
	return 1;
}

CMD:twchair(playerid, params[]) {
	new playerb;
	if(sscanf(params, "u", playerb)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /twchair [playerid]");
	if(!IsPlayerConnected(playerb)) return SendClientMessage(playerid, COLOR_GREY, "That player isn't connected.");
	if(inchair[playerb]) return SendClientMessage(playerid, -1, "	He is already in the chair!");
	if(tEChair != INVALID_PLAYER_ID) return SendClientMessage(playerid, -1, "Release the guy that is in the chair already!");
	if(!IsPlayerInRangeOfPoint(playerid, 3.0, 145.7005, 1723.3237, 10017.4072)) return SendClientMessage(playerid, -1, "	You are not near the chair !");
	if(playerb == playerid) return SendClientMessage(playerid, COLOR_WHITE, "You really trying to kill yourself? Use /kill");
	if(IsPlayerInRangeOfPlayer(2.0, playerid, playerb)) {
		format(donorstr, sizeof(donorstr), "You have forced %s into the chair !", PlayerICName(playerb));
		SendClientMessage(playerid, COLOR_GREY, donorstr);
		format(donorstr, sizeof(donorstr), "You have been forced into the chair by %s!", PlayerICName(playerid));
		SendClientMessage(playerb, COLOR_GREY, donorstr);
		format(donorstr, sizeof(donorstr), "* %s forces %s into the electric chair. *", PlayerICName(playerid), PlayerICName(playerb));
		ProxDetector(30.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		ClearAnimations(playerb);
		SetPlayerPos(playerb, 145.7005, 1723.3237, 10017.4072);
		inchair[playerb] = true;
		tEChair = playerb;
		TogglePlayerControllable(playerb, 0);
		ApplyAnimation(playerb, "PED","SEAT_down",4.0,0,0,0,1,0);
		SetCameraBehindPlayer(playerb);
		SetPlayerFacingAngle(playerb, 174.1728);
	}
	else {
		SendClientMessage(playerid, COLOR_GREY, "	That player is not near you or the chair!");
		return 1;
	}
	return 1;
}

CMD:twexecute(playerid, params[]) {
	new playerb;
	if(sscanf(params, "u", playerb)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /twexecute [playerid]");
	if(!IsPlayerConnected(playerb)) return SendClientMessage(playerid, COLOR_GREY, "That player isn't connected.");
	if(!inchair[playerb]) return SendClientMessage(playerid, -1, "	He is not in the chair!");
	if(!IsPlayerInRangeOfPoint(playerid, 3.0, 144.4691, 1721.3744, 10017.0996)) return SendClientMessage(playerid, -1, "	You are not near the power switch !");
	if(playerb == playerid) return SendClientMessage(playerid, COLOR_WHITE, "You really trying to kill yourself? Use /kill");
	new Float:hp;
 	GetPlayerHealth(playerb, hp);
	if(hp < 21) {
		SetPlayerFacingAngle(playerid, 96.0777);
		format(donorstr, sizeof(donorstr), "* %s pulls a leaver, electrocuting %s with 2,000 volts. *", PlayerICName(playerid), PlayerICName(playerb));
		ProxDetector(30.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		new Float:health;
  		GetPlayerHealth(playerb, health);
		SetPlayerHealth(playerb, health-20);
		SendClientMessage(playerb, COLOR_RED, "You are now dead !");
		ClearAnimations(playerb);
		inchair[playerb] = false;
		tEChair = INVALID_PLAYER_ID;
		TogglePlayerControllable(playerb, 1);
		format(donorstr, sizeof donorstr, "You have killed %s", PlayerICName(playerb));
		SendClientMessage(playerid, COLOR_GREY, donorstr);
		return 1;
	}
	else {
		SetPlayerFacingAngle(playerid, 96.0777);
		format(donorstr, sizeof(donorstr), "* %s pulls a leaver, electrocuting %s with 2,000 volts. *", PlayerICName(playerid), PlayerICName(playerb));
		ProxDetector(30.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		new Float:health;
  		GetPlayerHealth(playerb, health);
		SetPlayerHealth(playerb, health-20);
		SendClientMessage(playerb, COLOR_RED, "You have been injured and lose 20 health !");
		return 1;
	}
}

CMD:twrelease(playerid, params[]) {
	new playerb;
	if(sscanf(params, "u", playerb)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /twrelease [playerid]");
	if(!IsPlayerConnected(playerb)) return SendClientMessage(playerid, COLOR_GREY, "That player isn't connected.");
	if(!inchair[playerb]) return SendClientMessage(playerid, -1, "	That player is not in the chair !");
	if(playerb == playerid) return SendClientMessage(playerid, COLOR_WHITE, "You should not be seeing this message");
	if(!IsPlayerInRangeOfPoint(playerid, 3.0, 145.7005, 1723.3237, 10017.4072)) return SendClientMessage(playerid, -1, "	You are not near the chair!");
	format(donorstr, sizeof(donorstr), "You released %s from the chair !", PlayerICName(playerb));
	SendClientMessage(playerid, COLOR_GREY, donorstr);
	format(donorstr, sizeof(donorstr), "You were released by %s!", PlayerICName(playerid));
	SendClientMessage(playerb, COLOR_GREY, donorstr);
	format(donorstr, sizeof(donorstr), "* %s released %s from the electric chair. *", PlayerICName(playerid), PlayerICName(playerb));
	ProxDetector(30.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	ClearAnimations(playerb);
	SetPlayerPos(playerb, 145.7005, 1723.3237, 10017.4072);
	inchair[playerb] = false;
	tEChair = INVALID_PLAYER_ID;
	TogglePlayerControllable(playerb, 1);
	SetPlayerFacingAngle(playerb, 174.1728);
	return 1;
}

CMD:twcam(playerid, params[]) {
	new playername[MAX_PLAYER_NAME], id;
 	GetPlayerName(playerid, playername, sizeof(playername));
  	if(IsPlayerInRangeOfPoint(playerid, 5.0, 148.1596, 1707.5944, 10017.0996) || IsPlayerInRangeOfPoint(playerid, 5.0, 1460, 2862, -5)) {
    	if(sscanf(params, "i", id)) return SendClientMessage(playerid, -1, "Usage:/twcam [0-21]");
    	if(id == 0) {
  			SetCameraBehindPlayer(playerid);
		  	SetPlayerPos(playerid, 148.1596, 1707.5944, 10017.099);
		  	TogglePlayerControllable(playerid, 1);
	    }
		if(id > 0 && id < 12) {
		    TogglePlayerControllable(playerid, true);
		  	SetPlayerPos(playerid, 148.1596, 1707.5944, 10017.099);
		  	TogglePlayerControllable(playerid, false);
			SetPlayerCameraPos(playerid, CamPos[id][0], CamPos[id][1], CamPos[id][2]);
			SetPlayerCameraLookAt(playerid, CamPos[id][3], CamPos[id][4], CamPos[id][5]);

		}
		if(id >= 12 && id < 22) {
			SetPlayerCameraPos(playerid, CamPos[id][0], CamPos[id][1], CamPos[id][2]);
			SetPlayerCameraLookAt(playerid, CamPos[id][3], CamPos[id][4], CamPos[id][5]);
  			SetPlayerPos(playerid, 1460, 2862, -5);
    		TogglePlayerControllable(playerid, 0);
		}
	}
	else SendClientMessage(playerid, -1, "You are not in range of the camera room");
    return 1;
}

CMD:twmic(playerid, params[]) {
	if(isnull(params)) return SendClientMessage(playerid, -1, "Usage: /twmic [text]");
	format(donorstr,sizeof(donorstr), "[CAMERA-CONTROL]%s <o: %s", PlayerICName(playerid), params);
	if(IsPlayerInRangeOfPoint(playerid, 5.0, 148.1596, 1707.5944, 10017.0996) || IsPlayerInRangeOfPoint(playerid, 5.0, 1460, 2862, -5)) {
		for(new i = 0; i < MAX_PLAYERS; i++) {
		    if(IsPlayerInRangeOfPoint(i, 35.0, 148.1596, 1707.5944, 10017.0996) || IsPlayerInDynamicArea(i, twCircle, 0)) {
		        SendClientMessage(i, COLOR_YELLOW, donorstr);
		    }
		}
	}
	else SendClientMessage(playerid, -1, "You are not in range of the camera room");
	return 1;
}

CMD:twcmds(playerid, params[]) {
	SendClientMessage(playerid, -1, "<Tiger Woods Commands>:");
	SendClientMessage(playerid, -1, "/twmagic, /twcam, /twchair, /twrelease, /twexecute /twmic");
	return 1;
}