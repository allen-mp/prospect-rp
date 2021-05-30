/*

	file:			items/jammers.inc

	description:	Signal jammers for special organisations.

	author:			Day

	improved by:	Jay Cortez

	date improved:	5th April 2018

*/

#include <a_samp>
#include <foreach>

#define JAMMER_RANGE	30.0

// Jammer properties
enum jInfo
{
	jUniqueID,
	jObjectID,
	jPlaced,
	Float:jPos[4],
	jOn,
	jInt,
	jWorld,
	Text3D:jLabel
};
new JammerInfo[MAX_PLAYERS][jInfo];

// Places a jammer at the player
stock placeJammer(playerid)
{
	GetXYInFrontOfPlayer(playerid, JammerInfo[playerid][jPos][0], JammerInfo[playerid][jPos][1], JammerInfo[playerid][jPos][2], JammerInfo[playerid][jPos][3], 1);
	JammerInfo[playerid][jObjectID] = CreateDynamicObject(19513, JammerInfo[playerid][jPos][0], JammerInfo[playerid][jPos][1], JammerInfo[playerid][jPos][2] -1.0, 0, 0, JammerInfo[playerid][jPos][3], GetPlayerVirtualWorld(playerid));
	JammerInfo[playerid][jLabel] = CreateDynamic3DTextLabel("Signal Jammer [{FF0000}OFF{FFFFFF}]", 0xFFFFFFFF, JammerInfo[playerid][jPos][0], JammerInfo[playerid][jPos][1], JammerInfo[playerid][jPos][2] - 1 +0.88, 40.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, GetPlayerVirtualWorld(playerid), -1, -1, 30.0);
	SetPlayerSpecialAction(playerid, 0);
	ApplyAnimationEx(playerid, "CARRY", "putdwn", 3.0, 0, 0, 0, 0, 0, 0);
	JammerInfo[playerid][jPlaced] += 1;
	JammerInfo[playerid][jWorld] = GetPlayerVirtualWorld(playerid);
	JammerInfo[playerid][jInt] = GetPlayerInterior(playerid);
	return 1;
}

stock togJammer(playerid)
{
	if(!JammerInfo[playerid][jOn])
	{
		JammerInfo[playerid][jOn] = 1;
		SendClientMessage(playerid, 0x9ACD32AA, "Jammer activated.");
		DestroyDynamic3DTextLabel(JammerInfo[playerid][jLabel]);
		JammerInfo[playerid][jLabel] = CreateDynamic3DTextLabel("Signal Jammer [{9ACD32}ON{FFFFFF}]", 0xFFFFFFFF, JammerInfo[playerid][jPos][0], JammerInfo[playerid][jPos][1], JammerInfo[playerid][jPos][2] - 1 +0.88, 40.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, GetPlayerVirtualWorld(playerid), -1, -1, 30.0);
	
		// Cut phone calls
		foreach(new i: Player)
		{
			if(CheckForJammer(i))
				Mobile[i] = 999;
		}
	}
	else if(JammerInfo[playerid][jOn])
	{
		JammerInfo[playerid][jOn] = 0;
		SendClientMessage(playerid, 0xFF0000AA, "Jammer deactivated.");
		DestroyDynamic3DTextLabel(JammerInfo[playerid][jLabel]);
		JammerInfo[playerid][jLabel] = CreateDynamic3DTextLabel("Signal Jammer [{FF0000}OFF{FFFFFF}]", 0xFFFFFFFF, JammerInfo[playerid][jPos][0], JammerInfo[playerid][jPos][1], JammerInfo[playerid][jPos][2] - 1 +0.88, 40.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, GetPlayerVirtualWorld(playerid), -1, -1, 30.0);
	}
	return 1;
}

// Removes the player's jammer
stock removeJammer(playerid)
{
	JammerInfo[playerid][jOn] = 0;
	JammerInfo[playerid][jPlaced] = 0;
	JammerInfo[playerid][jPos][0] = 0; //x
	JammerInfo[playerid][jPos][1] = 0; //y
	JammerInfo[playerid][jPos][2] = 0; //z
	JammerInfo[playerid][jPos][3] = 0; //a
	JammerInfo[playerid][jWorld] = -1;

	DestroyDynamicObjectEx(JammerInfo[playerid][jObjectID]);
	DestroyDynamic3DTextLabel(JammerInfo[playerid][jLabel]);

	JammerInfo[playerid][jObjectID] = 0;
	JammerInfo[playerid][jLabel] = Text3D:INVALID_3DTEXT_ID;
	return 1;
}

// Check if the player is currently being jammed
stock CheckForJammer(playerid)
{
	foreach(new i: Player)
	{
		if(!JammerInfo[i][jPlaced]) continue;
		
		if(IsPlayerInRangeOfPoint(playerid, JAMMER_RANGE, JammerInfo[i][jPos][0], JammerInfo[i][jPos][1], JammerInfo[i][jPos][2]) && JammerInfo[i][jOn])
			return true;
	}

	return false;
}

// Checks if the player is allowed to use jammers
stock JammerAllowed(playerid) {
	return (
		PlayerInfo[playerid][pGroup] == GROUP_S9 ||
		PlayerInfo[playerid][pGroup] == GROUP_S99 ||
		PlayerInfo[playerid][pGroup] == GROUP_HITMAN ||
		PlayerInfo[playerid][pGroup] == GROUP_CRIME ||
		PlayerInfo[playerid][pMember] == FACTION_FBI ||
		PlayerInfo[playerid][pGroup] == GROUP_CARTEL
	);
}

// Command to place down a jammer
CMD:placejammer(playerid, params[])
{
	if(!JammerAllowed(playerid))
		return AdmErrorMsg;
	
	if(!PlayerInfo[playerid][pJammer])
		return SendClientMessage(playerid, COLOR_GREY, "You do not have a signal jammer.");
	
	if(JammerInfo[playerid][jPlaced])
		return SendClientMessage(playerid, COLOR_GREY, "You've already got a signal jammer placed somewhere.");

	placeJammer(playerid);
	PlayerInfo[playerid][pJammer] = 0;
	return 1;
}

// Command to remove a jammer
CMD:removejammer(playerid, params[])
{
	if(!JammerAllowed(playerid))
		return AdmErrorMsg;

	if(!JammerInfo[playerid][jPlaced])
		return SendClientMessage(playerid, COLOR_GREY, "You haven't placed a jammer yet.");

	if(!IsPlayerInRangeOfPoint(playerid, 1.0, JammerInfo[playerid][jPos][0], JammerInfo[playerid][jPos][1], JammerInfo[playerid][jPos][2]))
		return SendClientMessage(playerid, COLOR_GREY, "You are not near your signal jammer.");
	
	SetPlayerSpecialAction(playerid, 0);
	ApplyAnimationEx(playerid, "CARRY", "putdwn", 3.0, 0, 0, 0, 0, 0, 0);
	removeJammer(playerid);
	PlayerInfo[playerid][pJammer] = 1;
	return 1;
}

// Command to toggle a jammer
CMD:togjammer(playerid, params[])
	return cmd_jammer(playerid, params);

CMD:jammer(playerid, params[])
{
	if(!JammerAllowed(playerid))
		return AdmErrorMsg;

	if(!JammerInfo[playerid][jPlaced])
		return SendClientMessage(playerid, COLOR_GREY, "You haven't placed a jammer yet.");

	togJammer(playerid);
	return 1;
}

// Command for admins to find out who owns a jammer
CMD:jammerowner(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 2)
		return AdmErrorMsg;

	foreach(new i:Player)
	{
		if(!JammerInfo[i][jPlaced]) continue;
		
		if(IsPlayerInRangeOfPoint(playerid, 3.0, JammerInfo[i][jPos][0], JammerInfo[i][jPos][1], JammerInfo[i][jPos][2]))
		{
			new jammerStr[128];
			format(jammerStr, sizeof(jammerStr), "This jammer belongs to %s.", PlayerICName(i));
			SendClientMessage(playerid, COLOR_GREY, jammerStr);
			return 1;
		} 
	}

	SendClientMessage(playerid, COLOR_GREY, "No jammer was found near you.");
	return 1;
}

// Command to remove a jammer as an administrator
CMD:aremovejammer(playerid)
{
	if(PlayerInfo[playerid][pAdmin] < 2)
		return AdmErrorMsg;
	
	foreach(new i:Player)
	{
		if(!JammerInfo[i][jPlaced]) continue;
		
		if(IsPlayerInRangeOfPoint(playerid, 3.0, JammerInfo[i][jPos][0], JammerInfo[i][jPos][1], JammerInfo[i][jPos][2]))
		{
			removeJammer(i);

			new jammerStr[128];
			format(jammerStr, sizeof(jammerStr), "You have removed the signal jammer. (belonged to %s)", PlayerICName(i));
			SendClientMessage(playerid, COLOR_GREY, jammerStr);
			SendClientMessage(i, COLOR_GREY, "Your signal jammer has been removed by an admin.");
			return 1;
		}
	}

	SendClientMessage(playerid, COLOR_GREY, "No jammer was found near you.");
	return 1;
}

// Command to crush a jammer
CMD:crushjammer(playerid, params[])
{
	foreach(new i:Player)
	{
		if(!JammerInfo[i][jPlaced]) continue;
		
		if(IsPlayerInRangeOfPoint(playerid, 1.0, JammerInfo[i][jPos][0], JammerInfo[i][jPos][1], JammerInfo[i][jPos][2])) 
		{
			removeJammer(i);
			SetPlayerSpecialAction(playerid, 0);
			ApplyAnimationEx(playerid, "ped", "Jump_Land", 3.0, 0, 0, 0, 0, 0, 0);
			SendClientMessage(playerid, COLOR_GREY, "The signal jammer has been crushed.");
			return 1;
		}
	}

	SendClientMessage(playerid, COLOR_GREY, "No jammer was found near you.");
	return 1;
} 