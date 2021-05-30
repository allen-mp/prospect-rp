CMD:undercover(playerid, params[])
{
	if(
		PlayerInfo[playerid][pGroup] != GROUP_HITMAN &&
		PlayerInfo[playerid][pGroup] != GROUP_CARTEL &&
		PlayerInfo[playerid][pGroup] != GROUP_CRIME &&
		PlayerInfo[playerid][pGroup] != GROUP_S9 &&
		PlayerInfo[playerid][pMember] != FACTION_FBI 
	) return 1;

	if(
		(PlayerInfo[playerid][pGroup] == GROUP_CARTEL && !IsPlayerNearCJNGHQ(playerid)) ||
		(PlayerInfo[playerid][pGroup] == GROUP_CRIME && !IsPlayerNearSyndicateHQ(playerid))
	) return SendClientMessage(playerid, COLOR_GREY, "You are not at your headquarters.");

	new newName[25], newLevel;

	if(sscanf(params, "s[25]d", newName, newLevel)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /undercover [name] [level]");
	if(!IsValidName(newName)) return SendClientMessage(playerid, -1, "The name you requested doesn't follow the format of: \"Firstname_Lastname\"");
	if(newLevel < 1 || newLevel > 50) return SendClientMessage(playerid, COLOR_GREY, "Your level must be between 1-50.");
	new str[128];
	format(str, sizeof(str), "Your name has to be no longer than %d characters.", 25);
	if(strlen(newName) > 25) return SendClientMessage(playerid, COLOR_GREY, str);

	new query[128];
	mysql_format(sqlGameConnection, query, sizeof(query), "SELECT `Name` FROM `players` WHERE `Name` = '%e' LIMIT 1", newName);
	mysql_pquery(sqlGameConnection, query, "OnUndercoverNamechange", "isi", playerid, newName, newLevel);

	groupNamechangeRequest[playerid] = newName;
	groupLevelChangeRequest[playerid] = newLevel;
	return 1;
}

CMD:hnc(playerid, params[]) return cmd_undercover(playerid, params);
CMD:fbiuc(playerid, params[]) return cmd_undercover(playerid, params);