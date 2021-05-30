/*

Farmer job by Desmond

*/

#define VEHICLE_TRACTOR		(531)

new bool:isPlayerFarming[MAX_PLAYERS];
new PlayerBar:farmingProgressID[MAX_PLAYERS];
new farmerVehicle[MAX_PLAYERS], bool:finishingFarming[MAX_PLAYERS]; // vehicle id of the tractor, second var is to determine if a player is delivering the harvest
new farmingtimer[MAX_PLAYERS];

forward cancelfarming(playerid);
public cancelfarming(playerid)
{
	DisablePlayerCheckpointEx(playerid);
	CP[playerid] = CHECKPOINT_NONE;
	isPlayerFarming[playerid] = false;
	finishingFarming[playerid] = false;
	SetPlayerProgressBarValue(playerid, farmingProgressID[playerid], 0);
	HidePlayerProgressBar(playerid, farmingProgressID[playerid]);
	KillTimer(farmingtimer[playerid]);
	if(IsValidVehicle(farmerVehicle[playerid])) DestroyVehicle(farmerVehicle[playerid]);
	return 1;

}

forward GetPlayerFarmingPayment(playerid);
public GetPlayerFarmingPayment(playerid)
{
	new level = GetPlayerJobLevel(playerid, PlayerInfo[playerid][pJob]);
	new pay = 0;
	switch(level)
	{
		case 1: pay = 1000;
		case 2: pay = 1500;
		case 3: pay = 2250;
		case 4: pay = 3250;
		case 5: pay = 3800;
		default: pay = 1000; // just in case
	}
	return pay;
}

Hook:farmer_OnGameModeInit()
{
	iFarmer = CreateDynamicPickup(1239, 23, -40.0612, 26.5413, 3.1094);
	new Text3D:textFarmer = CreateDynamic3DTextLabel("Farmer Job\n{FFFFFF}/getjob", 0xFFFF91FF, -40.0612, 26.5413, 3.1094 +0.88, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);

	#pragma unused textFarmer
	return 1;
}

Hook:farmer_OnPlayerConnect(playerid)
{
	farmingProgressID[playerid] = CreatePlayerProgressBar(playerid,  290.0, 340.00, 65.0, 5.0, 0xFFFF00AA, 100.0, BAR_DIRECTION_RIGHT);
	isPlayerFarming[playerid] = false;
	finishingFarming[playerid] = false;
	return 1;
}

Hook:farmer_OnPlayerDisconnect(playerid, reason)
{
	cancelfarming(playerid);
	DestroyPlayerProgressBar(playerid, farmingProgressID[playerid]);
	return 1;
}
Hook:f_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if((newkeys & KEY_SPRINT) && !(oldkeys & KEY_SPRINT))
	{
		if(isPlayerFarming[playerid] != false)
		{
			if(GetPlayerProgressBarValue(playerid, farmingProgressID[playerid]) < 100)
			{
				SetPlayerProgressBarValue(playerid, farmingProgressID[playerid], GetPlayerProgressBarValue(playerid, farmingProgressID[playerid])+0.75);
				if(GetPlayerProgressBarValue(playerid, farmingProgressID[playerid]) >= 100)
				{
					SendClientMessage(playerid, -1, "You have harvested as much as you could.");
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "Drive the tractor to the checkpoint in 60 seconds!");
					SendClientMessage(playerid, -1, "TIP - leaving the tractor will stop you from finishing to deliver.");

					HidePlayerProgressBar(playerid, farmingProgressID[playerid]);

					new Float:pos[3];
					GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
					farmerVehicle[playerid] = CreateVehicle(VEHICLE_TRACTOR, pos[0], pos[1], pos[2], 0, 0, 0, -1);
					PutPlayerInVehicle(playerid, farmerVehicle[playerid], 0);
					finishingFarming[playerid] = true;
					CP[playerid] = CHECKPOINT_FARMING;
					SetPlayerCheckpointEx(playerid, 157.4377, -22.3081, 1.5781, 3);
					farmingtimer[playerid] = SetTimerEx("FarmingTimer", 60000, false, "i", playerid);
				}
			}	
		}
	}		
	return 1;
}

forward FarmingTimer(playerid);
public FarmingTimer(playerid)
{
	SendClientMessage(playerid, COLOR_GREY, "You have failed to deliver the harvest.");
	cancelfarming(playerid);
	return 1;
}

Hook:far_OnPlayerEnterCheckpoint(playerid)
{
	if(CP[playerid] == CHECKPOINT_FARMING && finishingFarming[playerid])
	{
		new pay = GetPlayerFarmingPayment(playerid);
		new msg[100];
		GiveMoney(playerid, pay);
		format(msg, sizeof(msg), "You have received $%d for delivering the harvest!", pay);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, msg);
		cancelfarming(playerid);
	}
	return 1;
}

Hook:farmer_OnPlayerExitVehicle(playerid, vehicleid)
{
	if(isPlayerFarming[playerid] && finishingFarming[playerid])
	{
		cancelfarming(playerid);
		SendClientMessage(playerid, COLOR_GREY, "You have left the tractor and therefore failed the delivery.");	
	}
	return 1;
}

CMD:startfarming(playerid, params[])
{
	if(PlayerInfo[playerid][pJob] != JOB_FARMER) return SendClientMessage(playerid, COLOR_GREY, "You are not a farmer!");
	if(!IsPlayerInRangeOfPoint(playerid, 60, 2.3479, -62.3304, 3.1172)) return SendClientMessage(playerid, COLOR_GREY, "You must be around the farm area at the Blueberry Farm!");
	if(isPlayerFarming[playerid]) return SendClientMessage(playerid, COLOR_GREY, "You are already farming!");

	isPlayerFarming[playerid] = true;
	SetPlayerProgressBarValue(playerid, farmingProgressID[playerid], 0);
	ShowPlayerProgressBar(playerid, farmingProgressID[playerid]);
	SendClientMessage(playerid, -1, "You are now farming! Press {FF0000}SPACE {FFFFFF}repeatedly to keep harvesting.");
	return 1;
}