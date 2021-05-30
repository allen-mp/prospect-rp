new BusDriver[MAX_PLAYERS];
new Bus[5];


Hook:bus_OnGameModeInit()
{
    Bus[0] = AddStaticVehicle(431,2159.2722,-1187.7704,23.9231,88.4259,0,0); //
    Bus[1] = AddStaticVehicle(431,2159.3555,-1177.5687,23.9225,90.2475,0,0); //
    Bus[2] = AddStaticVehicle(431,2159.4106,-1169.3866,23.9157,90.0462,0,0); //
    Bus[3] = AddStaticVehicle(431,2159.2747,-1161.7330,23.9329,90.0729,0,0); //
    Bus[4] = AddStaticVehicle(431,2159.3218,-1150.4325,24.2703,90.0714,0,0); //
    CreateDynamicPickup(1239, 23, 2141.65, -1176.11, 23.99);
	CreateDynamic3DTextLabel("Bus Driver\nType /getjob to get it!", 0xFE9A2EFF, 2141.65, -1176.11, 23.99 +0.8, 40.0);
    return 1;
}

forward Timer_Bus(playerid);
public Timer_Bus(playerid)
{
    TogglePlayerControllable(playerid, true);
}



CMD:busfare(playerid, params[]) {

	if(!(PlayerInfo[playerid][pJob] == 28)) return SendClientMessage(playerid, COLOR_GREY, "You are not a bus driver, you cannot use this command.");
	if(!IsPlayerInRangeOfPoint(playerid, 80.0, 2159.3555,-1177.5687,23.9225)) return SendClientMessage(playerid, COLOR_GREY, "You are not at the bus depot, go there to start your fare.");
	if(BusDriver[playerid] == 1) return SendClientMessage(playerid, COLOR_GREY, "You are already doing your driver job, use /stopfare to stop it.");
	if(CP[playerid] > 0) return SendClientMessage(playerid, COLOR_GREY, "Delete current CP on your minimap, /killcp.");
	goingJob[playerid] = false;
	isRepairing[playerid] = false;
	if(!IsABus(GetPlayerVehicleID(playerid))) return SendClientMessage(playerid, COLOR_GREY, "You are not in a bus.");
	if(GetPlayerState(playerid)==PLAYER_STATE_DRIVER)
	{
		SetPlayerCheckpoint(playerid, 2106.9014,-1502.9048,23.9540, 5.0);
		CP[playerid] = CHECKPOINT_BUS;
		BusDriver[playerid] = 1;
		SendClientMessage(playerid, COLOR_YELLOW, "Transportation Department: {FFFFFF}You are on duty now. Please proceed to County General Hospital, look at radar.");
	}
	else return SendClientMessage(playerid, COLOR_GREY, "You are not a driver, you are unable to use this command!");
	return 1;
}

Hook:bus_OnPlayerEnterCheckpoint(playerid)
{
    if(IsPlayerInRangeOfPoint(playerid, 5, 2106.9014,-1502.9048,23.9540))
 	{
 		DisablePlayerCheckpoint(playerid);
 		TogglePlayerControllable(playerid, false);
 		SetTimerEx("Timer_Bus", 10000, 0, "i", playerid);
		if(BusDriver[playerid] == 1) SetPlayerCheckpoint(playerid, 2078.5737,-1782.6232,13.5469, 5);
		SendClientMessage(playerid, COLOR_WHITE, "Drive to the next bus stop which is Pizza Stacks.");
		GiveMoney(playerid, 1000);
		SendClientMessage(playerid, COLOR_GREEN, "You recieved $1000 for stopping at this bus stop.");
	}
	if(IsPlayerInRangeOfPoint(playerid, 5, 2078.5737,-1782.6232,13.5469))
 	{

 		DisablePlayerCheckpoint(playerid);
 		TogglePlayerControllable(playerid, false);
        SetTimerEx("Timer_Bus", 10000, 0, "i", playerid);
		if(BusDriver[playerid] == 1) SetPlayerCheckpoint(playerid, 1929.8773,-2160.5049,13.5469, 5);
		SendClientMessage(playerid, COLOR_WHITE, "Drive to the next bus stop which is Los Santos Internation Airport.");
		GiveMoney(playerid, 1000);
		SendClientMessage(playerid, COLOR_GREEN, "You recieved $1000 for stopping at this bus stop.");
	}
	if(IsPlayerInRangeOfPoint(playerid, 5, 1929.8773,-2160.5049,13.5469))
 	{

 		DisablePlayerCheckpoint(playerid);
 		TogglePlayerControllable(playerid, false);
        SetTimerEx("Timer_Bus", 10000, 0, "i", playerid);
		if(BusDriver[playerid] == 1) SetPlayerCheckpoint(playerid, 1395.0769,-1746.9596,13.5469, 5);
		SendClientMessage(playerid, COLOR_WHITE, "Drive to the next bus stop which is City Hall.");
		GiveMoney(playerid, 1000);
		SendClientMessage(playerid, COLOR_GREEN, "You recieved $1000 for stopping at this bus stop.");
	}
	if(IsPlayerInRangeOfPoint(playerid, 5, 1395.0769,-1746.9596,13.5469))
 	{

 		DisablePlayerCheckpoint(playerid);
 		TogglePlayerControllable(playerid, false);
        SetTimerEx("Timer_Bus", 10000, 0, "i", playerid);
		if(BusDriver[playerid] == 1) SetPlayerCheckpoint(playerid, 1222.3495,-1390.0649,13.4721, 5);
		SendClientMessage(playerid, COLOR_WHITE, "Drive to the next bus stop which is All Saints General Hospital.");
		GiveMoney(playerid, 1000);
		SendClientMessage(playerid, COLOR_GREEN, "You recieved $1000 for stopping at this bus stop.");
	}
	if(IsPlayerInRangeOfPoint(playerid, 5, 1222.3495,-1390.0649,13.4721))//
 	{

 		DisablePlayerCheckpoint(playerid);
 		TogglePlayerControllable(playerid, false);
        SetTimerEx("Timer_Bus", 10000, 0, "i", playerid);
		if(BusDriver[playerid] == 1) SetPlayerCheckpoint(playerid, 802.6487,-1354.8877,13.5469, 5);
		SendClientMessage(playerid, COLOR_WHITE, "Drive to the next bus stop which is Market Sation.");
		GiveMoney(playerid, 1000);
		SendClientMessage(playerid, COLOR_GREEN, "You recieved $1000 for stopping at this bus stop.");
	}
	if(IsPlayerInRangeOfPoint(playerid, 5, 802.6487,-1354.8877,13.5469))//
 	{

 		DisablePlayerCheckpoint(playerid);
 		TogglePlayerControllable(playerid, false);
        SetTimerEx("Timer_Bus", 10000, 0, "i", playerid);
		if(BusDriver[playerid] == 1) SetPlayerCheckpoint(playerid, 1375.8877,-995.6205,28.9569, 5);
		SendClientMessage(playerid, COLOR_WHITE, "Drive to the next bus stop which is Bank of Los Santos.");
		GiveMoney(playerid, 1000);
		SendClientMessage(playerid, COLOR_GREEN, "You recieved $1000 for stopping at this bus stop.");
	}
	if(IsPlayerInRangeOfPoint(playerid, 5, 1375.8877,-995.6205,28.9569))//
 	{

 		DisablePlayerCheckpoint(playerid);
 		TogglePlayerControllable(playerid, false);
        SetTimerEx("Timer_Bus", 10000, 0, "i", playerid);
		if(BusDriver[playerid] == 1) SetPlayerCheckpoint(playerid, 851.9943,-559.7278,17.0912, 5);
		SendClientMessage(playerid, COLOR_WHITE, "Drive to the next bus stop which is Driving School of Dillimore.");
		GiveMoney(playerid, 1000);
		SendClientMessage(playerid, COLOR_GREEN, "You recieved $1000 for stopping at this bus stop.");
	}
	if(IsPlayerInRangeOfPoint(playerid, 5, 851.9943,-559.7278,17.0912))//
 	{

 		DisablePlayerCheckpoint(playerid);
 		TogglePlayerControllable(playerid, false);
        SetTimerEx("Timer_Bus", 10000, 0, "i", playerid);
		if(BusDriver[playerid] == 1) SetPlayerCheckpoint(playerid, 257.6198,-77.5568,1.5781, 5);
		SendClientMessage(playerid, COLOR_WHITE, "Drive to the next bus stop which is Liquor Of Blueberry.");
		GiveMoney(playerid, 1000);
		SendClientMessage(playerid, COLOR_GREEN, "You recieved $1000 for stopping at this bus stop.");
	}
	if(IsPlayerInRangeOfPoint(playerid, 5, 257.6198,-77.5568,1.5781))//
 	{

 		DisablePlayerCheckpoint(playerid);
 		TogglePlayerControllable(playerid, false);
        SetTimerEx("Timer_Bus", 10000, 0, "i", playerid);
		if(BusDriver[playerid] == 1) SetPlayerCheckpoint(playerid, 1183.2899,-185.7505,39.8595, 5);
		SendClientMessage(playerid, COLOR_WHITE, "Drive to the next bus stop which is Red Country.");
		GiveMoney(playerid, 1000);
		SendClientMessage(playerid, COLOR_GREEN, "You recieved $1000 for stopping at this bus stop.");
	}
	if(IsPlayerInRangeOfPoint(playerid, 5, 1183.2899,-185.7505,39.8595))//
 	{

 		DisablePlayerCheckpoint(playerid);
 		TogglePlayerControllable(playerid, false);
        SetTimerEx("Timer_Bus", 10000, 0, "i", playerid);
		if(BusDriver[playerid] == 1) SetPlayerCheckpoint(playerid, 2144.3013,-1150.2538,24.3270, 5);
		SendClientMessage(playerid, COLOR_WHITE, "Drive to the next bus stop which is Bus Depot.");
		GiveMoney(playerid, 1000);
		SendClientMessage(playerid, COLOR_GREEN, "You recieved $1000 for stopping at this bus stop.");
	}
	if(IsPlayerInRangeOfPoint(playerid, 5, 2144.3013,-1150.2538,24.3270))//
 	{
 		new vid = GetPlayerVehicleID( playerid );
 		RemovePlayerFromVehicle( playerid );
 		SetVehicleToRespawn(vid);
 		DisablePlayerCheckpoint(playerid);
		BusDriver[playerid] = 0;
        new level = GetPlayerJobLevel(playerid, PlayerInfo[playerid][pJob]);
	    new pay = 0;
        new str[240];
	    switch(level)
	    {
		   case 1: pay = 10000;
		   case 2: pay = 20000;
		   case 3: pay = 30000;
		   case 4: pay = 40000;
		   case 5: pay = 50000;
		   default: pay = 10000; // just in case
	    }
		GiveMoney(playerid, pay);
        PlayerInfo[playerid][pBusSkill] += 1;
        format(str, sizeof(str), "You've completed your shift and recieved $%d as your salary, your job level is: %d.", pay, GetPlayerJobLevel(playerid, PlayerInfo[playerid][pJob]));
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
        CP[playerid] = 0;
	}
    return 1;
}

Hook:bus_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
    if(IsABus(vehicleid) && !ispassenger && (!(PlayerInfo[playerid][pJob] == 28)))
	{
		SendClientMessage(playerid, COLOR_RED, "Error: {FFFFFF}You must have bus job to access the vehicle.");
		PlayerPlaySound(playerid, 24600, 0.0, 0.0, 0.0);
		ClearAnimations(playerid);
	}
	return 1;
}

Hook:bus_OnPlayerExitVehicle(playerid, vehicleid)
{
	if(IsABus(GetPlayerVehicleID(playerid)) && BusDriver[playerid] == 1)
	{
		new vid = GetPlayerVehicleID( playerid );
 		SetVehicleToRespawn(vid);
		BusDriver[playerid]  = 0;
		DisablePlayerCheckpoint(playerid);
		CP[playerid] = 0;
	    SendClientMessage(playerid, COLOR_YELLOW, "Transportation Department: {FFFFFF}You exit the bus while on-duty, you won't get salary.");
	}
	return 1;
}

Hook:bus_OnPlayerConnect(playerid)
{
	BusDriver[playerid] = 0;
	return 1;
}

Hook:bus_OnPlayerDisconnect(playerid, reason)
{
	BusDriver[playerid] = 0;
	return 1;
}