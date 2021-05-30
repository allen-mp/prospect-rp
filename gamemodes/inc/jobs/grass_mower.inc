CMD:grassmow(playerid, params[], help)
{
	if(PlayerInfo[playerid][pJob] != 23) return SendClientMessage(playerid, COLOR_GREY, "You're not a grass mower!");
	if(CuttingGrass[playerid] != 0) return SendClientMessage(playerid, COLOR_GREY, "You already have active job, /stopmowing to stop it");
	if(!GrassCar(GetPlayerVehicleID(playerid))) return SendClientMessage(playerid, COLOR_GREY, "You're not in a lawn mower!");
	new cutrandom = random(sizeof(RandomMowe));
	SetPlayerCheckpoint(playerid, RandomMowe[cutrandom][0], RandomMowe[cutrandom][1], RandomMowe[cutrandom][2], 5.0);
	CuttingGrass[playerid] = 1;
    CP[playerid] = CHECKPOINT_GRASS;
	SendClientMessage(playerid, COLOR_GREY, "You started with the grass mowning, follow the red markers.");
	return 1;
}

CMD:stopmowing(playerid, params[], help)
{
	if(PlayerInfo[playerid][pJob] != 23) return SendClientMessage(playerid, COLOR_GREY, "You're not a grass mower!");
	if(CuttingGrass[playerid] < 1) return SendClientMessage(playerid, COLOR_GREY, "You do not have active job. /grassmow to start it.");
	CuttingGrass[playerid] = 0;
	DisablePlayerCheckpointEx(playerid);
    CP[playerid] = 0;
	return 1;
}

Hook:GJ_OnPlayerEnterCheckpoint(playerid)
{
	new grassthing = GetPlayerVehicleID(playerid);
	new str[1024];
	if(CuttingGrass[playerid] >= 1 && GrassCar(grassthing) && GetPlayerState(playerid) == 2)
	{
	    new cutting = random(sizeof(RandomMowe));
		CuttingGrass[playerid] ++;
		SetPlayerCheckpoint(playerid, RandomMowe[cutting][0], RandomMowe[cutting][1], RandomMowe[cutting][2], 5.0);
        CP[playerid] = CHECKPOINT_GRASS;
		UseAdmCmdTimer[playerid]++;
		if(UseAdmCmdTimer[playerid] > 3)
		{
		    new reason[1024];
			format(str, sizeof(str), "{FF6347}SERVER: %s was banned, reason: (autoban) TP Hacks while mowing grass.", PlayerOOCName(playerid));
			SendClientMessageToAll(COLOR_WHITE, str);

			format(reason, sizeof(reason), "TPH Mowing Grass");
			BanAccount(playerid, INVALID_PLAYER_ID, reason); //log before banning the account
			return 1;
		}
		SetTimerEx("Timer_UseAdmCmd", 3 *1000, 0, "i", playerid);
		if(CuttingGrass[playerid] == 20)
		{
			new bonus = random(10000);
 			GiveMoney(playerid, 10000+bonus);
 			format(str, sizeof(str), "You've finished with grass mowing, your paycheck is $8000, and bonus $%d", bonus);
			SendClientMessage(playerid, COLOR_FAMED, str);
			CuttingGrass[playerid] = 0; DisablePlayerCheckpoint(playerid);
            CP[playerid] = 0;
		}
	}
	return 1;
}
Hook:GJ_OnPlayerStateChange(playerid, newstate, oldstate)
{
	new cargrassa = GetPlayerVehicleID(playerid);
	if(GrassCar(cargrassa))
	{
		if(PlayerInfo[playerid][pJob] != JOB_GRASS)
		{
			SendClientMessage(playerid, COLOR_FAMED, "You're not grass mower!");
			RemovePlayerFromVehicle(playerid);
		}
		return 1;
	}
	return 1;
}

Hook:GJ_OnGameModeInit()
{
	cargrass[0] = AddStaticVehicleEx(572,1955.8530,-1247.6792,19.3691,0.0653,3,3,650); //
	cargrass[1] = AddStaticVehicleEx(572,1952.6689,-1247.7767,19.2594,359.6955,1,1,650); //
	cargrass[2] = AddStaticVehicleEx(572,1949.6405,-1247.8177,19.1656,358.6474,2,2,650); //
	cargrass[3] = AddStaticVehicleEx(572,1946.8290,-1247.7072,19.0117,358.6920,3,3,650); //
	cargrass[4] = AddStaticVehicleEx(572,1943.6764,-1247.6243,18.7368,357.6494,0,0,650); //
	cargrass[5] = AddStaticVehicleEx(572,1940.3818,-1247.5013,18.3957,355.8244,6,6,650); //
	cargrass[6] = AddStaticVehicleEx(572,1937.0133,-1247.5010,18.0510,359.9681,11,11,650); //
	CreateDynamic3DTextLabel("Grass Mower\n{FFFFFF}/getjob", 0xFFFF91FF, 1961.1292,-1244.8138,19.9714 +0.60, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
	CreateDynamicPickup(1239, 23, 1961.1292,-1244.8138,19.9714);
	return 1;
}