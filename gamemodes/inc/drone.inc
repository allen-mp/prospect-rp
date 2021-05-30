// Drone system repurposed by Jay Cortez

#define DRONE_RESPAWN_TIME	30000

new
	droneVeh, Float:dronePlayerPos[3], droneControl = -1, dronecontrolVW = 0;

forward OnDroneRespawnDeploy();
public OnDroneRespawnDeploy()
{
	droneControl = -1;
	return 1;
}

CMD:deploydrone(playerid, params[])
{
	if(PlayerInfo[playerid][pGroup] != GROUP_S9 && PlayerInfo[playerid][pGroup] != GROUP_S99)
		return 1;

	if(droneControl == -2)
		return SendClientMessage(playerid, COLOR_GREY, "The drone takes time to repair, you must wait.");

	if(IsValidVehicle(droneVeh))
		return SendClientMessage(playerid, COLOR_GREY, "The drone has already been deployed.");
	
	new Float:vPos[4];

	GetXYInFrontOfPlayer(playerid, vPos[0], vPos[1], vPos[2], vPos[3], 0.5);
	droneVeh = CreateVehicle(501, vPos[0], vPos[1], vPos[2], 0.00, -1, -1, 1000);
	
	new droneString[128];
	format(droneString, sizeof(droneString), "* %s unpacks a drone and sets it down.", PlayerICName(playerid));
	ProxDetector(30.0, playerid, droneString, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	return 1;
}

CMD:controldrone(playerid, params[])
{
	if(PlayerInfo[playerid][pGroup] != GROUP_S9 && PlayerInfo[playerid][pGroup] != GROUP_S99)
		return 1;
	    
	if(droneControl == -2)
		return SendClientMessage(playerid, COLOR_GREY, "The drone is currently being repaired");
	
	if(droneControl != playerid && droneControl != -1)
		return SendClientMessage(playerid, COLOR_GREY, "The drone is currently being operated.");
	
	if(!IsValidVehicle(droneVeh))
		return SendClientMessage(playerid, COLOR_GREY, "The drone isn't yet deployed.");
		
	new droneString[128];
	if(droneControl == -1)
	{
		GetPlayerPos(playerid, dronePlayerPos[0], dronePlayerPos[1], dronePlayerPos[2]);
		dronecontrolVW = GetPlayerVirtualWorld(playerid);
		SetPlayerVirtualWorld(playerid, 0);
		PutPlayerInVehicle(playerid, droneVeh, 0);
		droneControl = playerid;
		format(droneString, sizeof(droneString), "* %s fiddles with the drone's remote control.", PlayerICName(playerid));
		ProxDetector(30.0, playerid, droneString, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		
		format(droneString, sizeof(droneString), "HQ: %s has taken control of the tactical drone.", PlayerOOCName(playerid));
		if(PlayerInfo[playerid][pGroup] == GROUP_S9) SendGroupMessage(GROUP_S9, COLOR_S9, droneString);
		else if(PlayerInfo[playerid][pGroup] == GROUP_S99) SendGroupMessage(GROUP_S99, COLOR_S9, droneString);
	}
	else
	{
		RemovePlayerFromVehicle(playerid);
		SetPlayerVirtualWorld(playerid, dronecontrolVW);
		SetPlayerPos(playerid, dronePlayerPos[0], dronePlayerPos[1], dronePlayerPos[2]);
		format(droneString, sizeof(droneString), "* %s puts away the drone's remote control.", PlayerICName(playerid));
		ProxDetector(30.0, playerid, droneString, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		droneControl = -1;
	}
	return 1;
}

stock OnDroneUpdate(playerid)
{
	if(droneControl == playerid)
	{
		for(new id = 0; id < MAX_PLAYERS; id++)
		{
			if(id == playerid) continue;
			new
				Float:px, Float:py, Float:pz;
			GetPlayerPos(id, px, py, pz);
			if(IsPlayerInRangeOfPoint(playerid, 2.6, px, py, pz))
			{
				SendClientMessage(droneControl, COLOR_S9, "[S9 Drone] The drone has been destroyed.");
				DestroyDrone(playerid);
				droneControl = -2;
				SetTimer("OnDroneRespawnDeploy", DRONE_RESPAWN_TIME, 0);
			}
		}
	}
	return 1;
}

stock DestroyDrone(playerid)
{
	if(IsPlayerConnected(playerid))
	{
		RemovePlayerFromVehicle(playerid);
		SetPlayerPos(playerid, dronePlayerPos[0], dronePlayerPos[1], dronePlayerPos[2]);
		SetPlayerVirtualWorld(playerid, dronecontrolVW);
	}
	DestroyVehicle(droneVeh);
	droneVeh = INVALID_VEHICLE_ID;
	return 1;
}

stock OnDroneRespawn(vehicleid)
{
	if(droneVeh == vehicleid)
		DestroyDrone(droneControl);
	return 1;
}

stock OnDroneSignalCheck(playerid)
{
	if(droneControl == playerid)
	{
		if(IsValidVehicle(droneVeh))
		{
			new Float:droneHealth;
			GetVehicleHealth(droneVeh, droneHealth);
			if(droneHealth < 450.0)
			{
				SendClientMessage(playerid, COLOR_S9, "[S9 Drone] The signal was cut due to damage.");
				SetVehicleHealth(droneVeh, 1000.0);
				DestroyDrone(playerid);
				droneControl = -2;
				SetTimer("OnDroneRespawnDeploy", DRONE_RESPAWN_TIME, 0);
			}
		}
	}
	return 1;
}
stock OnDroneLogout(playerid)
{
	if(droneControl == playerid)
	{
		droneControl = -1;
		return 1;
	}
	return 0;
}

stock droneAttacked(issuerid, playerid)
{
	if(droneControl != issuerid)
	{
		DestroyDrone(droneControl);
		droneControl = -2;
		SetTimer("OnDroneRespawnDeploy", DRONE_RESPAWN_TIME, 0);
		SendClientMessage(playerid, COLOR_S9, "[S9 Drone] The drone has been destroyed.");
	}
	return 1;
}

stock OnDroneDeath(vehicleid)
{
	if(droneControl > -1 && droneVeh == vehicleid)
	{
		DestroyDrone(droneControl);
		droneControl = -2;
		SetTimer("OnDroneRespawnDeploy", DRONE_RESPAWN_TIME, 0);
		SendClientMessage(droneControl, COLOR_S9, "[S9 Drone] The drone has been destroyed.");
	}
	return 1;
}
