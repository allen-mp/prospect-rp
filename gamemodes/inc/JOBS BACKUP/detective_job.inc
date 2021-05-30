/*

	file:			jobs/detective_job.inc

	description:	The detective job for SARP.

	author:			Unknown

	improved by:	Jay Cortez

	date created:	29th March 2018

*/


// Command to track a player
CMD:find(playerid, params[])
{
	if(	PlayerInfo[playerid][pJob] != 1 &&
		PlayerInfo[playerid][pGroup] != GROUP_S9 &&
		PlayerInfo[playerid][pGroup] != GROUP_S99 &&
		PlayerInfo[playerid][pMember] != FACTION_FBI &&
		PlayerInfo[playerid][pGroup] != GROUP_HITMAN
	) return SendClientMessage(playerid, COLOR_GREY, "You are not a detective.");

	if(gettime() < UsedFind[playerid] + FReloadTime[playerid])
		return SendClientMessage(playerid, COLOR_GREY, "You've already searched for someone, you must wait your reload time.");

	if(CheckForJammer(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "Tracker signal blocked.");

	new id;
	if(sscanf(params, "u", id)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /find [playerid/PartOfName]");
	
	if(PlayerInfo[id][pPnumber] == 0)
		return SendClientMessage(playerid, COLOR_GREY, "That player doesn't have a cellphone.");

	if(!IsPlayerConnected(id))
		return SendClientMessage(playerid, COLOR_GREY, "Unknown player.");	

	if(PlayerInfo[id][pAdmin] &&  PlayerInfo[id][pStealthed] == 0 )
		return SendClientMessage(playerid, COLOR_GREY, "You cannot track admins.");

	if(id == playerid)
		return SendClientMessage(playerid, COLOR_GREY, "You cannot track yourself.");

	if(	
		GetPlayerInterior(id) != 0 ||
		PlayerInfo[id][pMask] ||
		GetPlayerState(id) == PLAYER_STATE_SPECTATING ||
		GetPlayerVirtualWorld(id) != 0 ||
		PlayerInfo[id][pAdmin] && PlayerInfo[id][pStealthed]
	) return SendClientMessage(playerid, COLOR_GREY, "That player is currently inside.");

	if(PhoneOffline[id])
		return SendClientMessage(playerid, COLOR_GREY, "No signal was returned.");

	if(CheckForJammer(id))
		return SendClientMessage(playerid, COLOR_GREY, "A very weak signal was returned.");

	new points, zone[MAX_ZONE_NAME], level = PlayerInfo[playerid][pDetSkill];
	GetPlayer2DZone(id, zone, MAX_ZONE_NAME);

	if(level >= 0 && level <= 49)
	{
		points = 4;
		FReloadTime[playerid] = 120;
	}
	else if(level >= 50 && level <= 99)
	{
		points = 6;
		FReloadTime[playerid] = 80;
	}
	else if(level >= 100 && level <= 199)
	{
		points = 8;
		FReloadTime[playerid] = 60;
	}
	else if(level >= 200 && level <= 399)
	{
		points = 10;
		FReloadTime[playerid] = 30;
	}
	else if(level >= 400)
	{
		points = 12;
		FReloadTime[playerid] = 20;
	}

	if(PlayerInfo[playerid][pGroup] == GROUP_S9 || PlayerInfo[playerid][pGroup] == GROUP_S99 || PlayerInfo[playerid][pMember] == FACTION_FBI) {
		FReloadTime[playerid] = 0;
		points = 30;
	}

	new trackStr[128];

	// Action
	format(trackStr, sizeof(trackStr), "* %s begins to track a signal.", PlayerICName(playerid));
	ProxDetector(15.0, playerid, trackStr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

	// Set the marker
	SetPlayerMarkerForPlayer(playerid, id, 0x9B0000AA);

	// Send message
	format(trackStr, sizeof(trackStr), "%s's cellphone signal points to %s.", PlayerICName(id), zone);
	SendClientMessage(playerid, COLOR_GREY, trackStr);

	FindingID[playerid] = id;
	FindTime[playerid] = 1;
	UsedFind[playerid] = gettime();
	FindTimePoints[playerid] = points;
	PlayerInfo[playerid][pDetSkill] ++;

	// Skill has been increased
	if(PlayerInfo[playerid][pDetSkill] == 50)
		SendClientMessage(playerid, COLOR_YELLOW, "* Your Detective Skill is now Level 2, you now have a 80 second reload time.");
	else if(PlayerInfo[playerid][pDetSkill] == 100)
		SendClientMessage(playerid, COLOR_YELLOW, "* Your Detective Skill is now Level 3, you now have a 60 second reload time.");
	else if(PlayerInfo[playerid][pDetSkill] == 200)
		SendClientMessage(playerid, COLOR_YELLOW, "* Your Detective Skill is now Level 4, you now have a 30 second reload time.");
	else if(PlayerInfo[playerid][pDetSkill] == 400)
		SendClientMessage(playerid, COLOR_YELLOW, "* Your Detective Skill is now Level 5, you now have a 20 second reload time.");
	return 1;
}

// Called every second
detective_SecondSync(playerid) {
	if(FindTime[playerid])
	{
		if(FindTime[playerid] == FindTimePoints[playerid])
		{
			PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
			displayCenterHUDInfo(playerid,  "~r~Tracker lost signal...", 8);
			FindTime[playerid] = 0;
			FindTimePoints[playerid] = 0;
			new target = FindingID[playerid];
			if(Called911[target] != 0)
			{
				SetAllCopCheckpoint(target);
			}
			else
			{
				SetPlayerToTeamColor(target);
			}
			FindingID[playerid] = 0;
		}
		else
		{
			new trackStr[128];
			format(trackStr, sizeof(trackStr), "~r~Tracker time left: ~w~%i", FindTimePoints[playerid] - FindTime[playerid]);
			displayCenterHUDInfo(playerid,  trackStr, 8); //display for 8 seconds
			FindTime[playerid] += 1;
		}
	}
	return 1;
}