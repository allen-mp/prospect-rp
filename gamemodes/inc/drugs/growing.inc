CMD:checkweed(playerid, params[])
{
	new weed = IsPlayerNearWeed(playerid);
	if(weed == -1) return SendClientMessage(playerid, COLOR_GREY, "You are not near any plants.");
	
	ApplyAnimationEx(playerid, "BOMBER", "BOM_Plant_In", 4.0, 0, 0, 0, 0, 0);

	new str[128];
	format(str, sizeof(str), "* %s inspects the weed plant.", PlayerICName(playerid));
	ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	
	format(str, sizeof(str), "This plant has %d weed grams.", WeedInfo[weed][WeedGrams]);
	SendClientMessage(playerid, COLOR_GREEN, str);
	return 1;
}

CMD:pickweed(playerid, params[])
{
	new weed = IsPlayerNearWeed(playerid), str[128];
	if(weed == -1) return SendClientMessage(playerid, COLOR_GREY, "You are not near any plants.");
	
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendClientMessage(playerid, COLOR_GREY, "You must be on foot to pick weed.");
	
	if(GotPlant[playerid] >= 1)
	{
		if(OldWeedX[playerid] != 0.0 && OldWeedY[playerid] != 0.0)
		{
			if(!IsPlayerInRangeOfPoint(playerid, 120.0, OldWeedX[playerid], OldWeedY[playerid], OldWeedZ[playerid]))
			{
				format(str, sizeof(str), "Teleport: [%d]%s is potentially Teleport Pickweeding.", playerid, PlayerICName(playerid));
				UpdateWarnings(str);
				return 1;
			}
		}
	}
	if(PlayerTied[playerid] || PlayerCuffed[playerid] || PlayerTazed[playerid] || PlayerFrozen[playerid]) return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	if(WeedPickTimer[playerid])
	 return SendClientMessage(playerid, COLOR_GREY, "You must wait 3 seconds before picking weed again.");
	
	if(WeedInfo[weed][WeedTime] < 2)
		return SendClientMessage(playerid, COLOR_GREY, "This plant is not ready to be picked, please wait at least 2 minutes for it to grow.");
	
	GetPlayerPos(playerid, WeedPickPosition[playerid][PosX], WeedPickPosition[playerid][PosY], WeedPickPosition[playerid][PosZ]);
	WeedPickTimer[playerid] = 1;
	
	format(str, sizeof(str), "* %s begins picking a weed plant.", PlayerICName(playerid));
	ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	
	SetTimerEx("Timer_WeedPick", 7000, 0, "ii", playerid, weed);
	return 1;
}

CMD:plantweed(playerid, params[])
{
	new str[128];
	if(PlayerInfo[playerid][pPotSeeds] < 10) return SendClientMessage(playerid, COLOR_GREY, "You need 10 seeds to plant your weed.");
	if(PlayerInfo[playerid][pPlantTime] > 0)
	{
		format(str, sizeof(str), "Please wait %d seconds before planting weed again.", PlayerInfo[playerid][pPlantTime]);
		return SendClientMessage(playerid, COLOR_GREY, str);
	}
	for(new weed = 0; weed < sizeof(WeedInfo); weed++)
	{
	if(WeedInfo[weed][WeedPlanted] == 0)
		{
			new Float:X2, Float:Y2, Float:Z2, Float:V;
			GetPlayerVelocity(playerid, X2, Y2, Z2);
			V = floatsqroot(floatpower(X2, 2) + floatpower(Y2, 2) + floatpower(Z2, 2));
			if(V > 0.05)
			{
				SendClientMessage(playerid, COLOR_GREY, "You must stop moving to plant your seeds.");
				return 1;
			}
			if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendClientMessage(playerid, COLOR_GREY, "You must be on foot to plant your seeds.");
			if(GetPlayerInterior(playerid) != 0) return SendClientMessage(playerid, COLOR_GREY, "You must be outside to plant your seeds.");
			if(GetPlayerVirtualWorld(playerid) != 0) return SendClientMessage(playerid, COLOR_GREY, "You must not be in a virtual world to plant your seeds.");
			if(PlayerInfo[playerid][pJob] == 4 || PlayerInfo[playerid][pDonateRank] >= 3)
			{
				GetPlayerPos(playerid, X2, Y2, Z2);
				new playername[MAX_PLAYER_NAME];
				GetPlayerICName(playerid, playername, 24);
				ApplyAnimationEx(playerid, "BOMBER", "BOM_Plant_In", 4.0, 0, 0, 0, 0, 0);
				weedObject[weed] = CreateDynamicObject(3409, X2, Y2, Z2 -2, 0, 0, 0);
				Streamer_Update(playerid);
				PlayerInfo[playerid][pPlantTime] = 1200;
				if(PlayerInfo[playerid][pAdmin] >= 1337)
				{
					PlayerInfo[playerid][pPlantTime] = 0;
				}
				WeedInfo[weed][WeedX] = X2;
				WeedInfo[weed][WeedY] = Y2;
				WeedInfo[weed][WeedZ] = Z2;
				WeedInfo[weed][WeedTime] = 0;
				WeedInfo[weed][WeedGrams] = 0;
				WeedInfo[weed][WeedPlanted] = 1;
				strmid(WeedInfo[weed][WeedPlanter], playername, 0, strlen(playername), 50);
				format(str, sizeof(str), "* %s plants some seeds.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				SendClientMessage(playerid, COLOR_GREEN, "You have planted your seeds! Every minute, your plant will make one gram of weed.");
				SendClientMessage(playerid, COLOR_GREEN, "If your plant isn't picked within 70 minutes, it will rot and you will lose it.");
				SendClientMessage(playerid, COLOR_GREEN, "Also remember that anyone can /pickweed if they find your plant, so be careful.");
				PlayerInfo[playerid][pPotSeeds] -= 10;
				break;
			}
			else return SendClientMessage(playerid, COLOR_GREY, "You are not a Drug Dealer/Gold VIP.");
		}
	}
	return 1;
}

// Admin cmds

CMD:setwt(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 1337)
		return AdmErrorMsg;

	new weedid, time;
	if(sscanf(params, "ii", weedid, time))
		return SendClientMessage(playerid, COLOR_GREY, "USAGE: /setwt [ID] [time]");

	WeedInfo[weedid][WeedTime] = time;

	new str[128];
	format(str, sizeof(str), "You have set weedid %d time to %d.", weedid, time);
	SendClientMessage(playerid, COLOR_GRAD1, str);
	return 1;
}