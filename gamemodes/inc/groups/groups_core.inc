// Groups
#define MAX_GROUPS 10

#define GROUP_NONE 			0
#define GROUP_HITMAN 		1	// Non-donated. [Hitman agency]
#define GROUP_CRIME 		2	// Non-donated. [Syndicate]
#define GROUP_REBELS 		3	// Donated. [Zherdev OPG] Owner: Lucas Maxwell
#define GROUP_CARTEL 		4	// Non-donated. [CJNG]
#define GROUP_TIJUANA 		5	// Donated. [The Network] Owner: Capo
#define GROUP_COMMISSION 	6	// Donated. [The Commission] Owner: Petrucci
#define GROUP_EXEC 			7	// Donated. [Executive Outcomes] Owner: Dragus
#define GROUP_LLS 			8	// Donated. [LLS] Owner: Melvin Johnson
#define GROUP_OPG 			9	// Donated. [The Grey Wolves] Owner: Heidi Stone
#define GROUP_S9			10  // Non-donated. [Sector 9]

// Group ranks
#define GROUP_HITMAN_RANKS 		6
#define GROUP_CRIME_RANKS 		5
#define GROUP_CARTEL_RANKS 		4
#define GROUP_REBELS_RANKS 		5
#define GROUP_TIJUANA_RANKS 	6
#define GROUP_COMMISSION_RANKS 	5
#define GROUP_EXEC_RANKS 		5
#define GROUP_LLS_RANKS 		5
#define GROUP_OPG_RANKS			5
#define GROUP_S9_RANKS			4

/*
	Variables, arrays and enums
*/

new groupNamechangeRequest[MAX_PLAYERS][25];
new groupLevelChangeRequest[MAX_PLAYERS];

// Group toggle radio
new groupRadioToggled[MAX_GROUPS+1];
new OOCgroupRadioToggled[MAX_GROUPS+1];

new GroupOffer[MAX_PLAYERS];


/*
	Functions
*/

stock GetGroupName(groupID) {
	new gname[64];
	switch(groupID)
	{
		case GROUP_HITMAN:		gname = "Hitman Agency";
		case GROUP_CRIME:		gname = "Cicada 3301";
		case GROUP_REBELS:		gname = "Zherdev OPG";
		case GROUP_CARTEL:		gname = "CJNG";
		case GROUP_TIJUANA:		gname = "Tijuana Cartel";
		case GROUP_COMMISSION:	gname = "The Commission";
		case GROUP_EXEC:		gname = "Executive Outcomes";
		case GROUP_LLS:			gname = "La Luna Sangre";
		case GROUP_OPG:			gname = "The Grey Wolves";
		case GROUP_S9:			gname = "Sector 9";
		default:				gname = "None";
	}
	return gname;
}

stock GetPlayerGroupName(playerid)
{
	new gname[64];
	format(gname, sizeof(gname), "%s", GetGroupName(PlayerInfo[playerid][pGroup]));
	return gname;
}

stock GetGroupColour(groupid)
{
	switch(groupid)
	{
		case GROUP_HITMAN: 	return COLOR_HITMAN;
		case GROUP_CRIME: 	return COLOR_CRIME;
		case GROUP_REBELS: 	return COLOR_REBELS;
		case GROUP_CARTEL: 	return COLOR_CARTEL_DE_TIJUANA;
		case GROUP_TIJUANA: 	return COLOR_BUREAU;
		case GROUP_COMMISSION: 	return COLOR_RAVENS;
		case GROUP_EXEC: 	return COLOR_EXEC_GROUP;
		case GROUP_LLS: 	return COLOR_LLS;
		case GROUP_OPG: 	return COLOR_PALERMO;
		case GROUP_S9:		return COLOR_S9;
	}
	return -1;
}

stock GetGroupMaxRanks(groupid)
{
	switch(groupid)
	{
		case GROUP_HITMAN: 		return GROUP_HITMAN_RANKS;
		case GROUP_CRIME: 		return GROUP_CRIME_RANKS;
		case GROUP_REBELS: 		return GROUP_REBELS_RANKS;
		case GROUP_CARTEL: 		return GROUP_CARTEL_RANKS;
		case GROUP_TIJUANA: 		return GROUP_TIJUANA_RANKS;
		case GROUP_COMMISSION: 	return GROUP_COMMISSION_RANKS;
		case GROUP_EXEC: 		return GROUP_EXEC_RANKS;
		case GROUP_LLS: 		return GROUP_LLS_RANKS;
		case GROUP_OPG: 		return GROUP_OPG_RANKS;
		case GROUP_S9:			return GROUP_S9_RANKS;
	}
	return -1;
}

stock IsGroupLeader(playerid) {
	if(!PlayerInfo[playerid][pGroup])
		return false;

	return PlayerInfo[playerid][pGroupRank] >= GetGroupMaxRanks(PlayerInfo[playerid][pGroup])-1;
}

stock GetPlayerGroupRankName(playerid)
{
	new rankname[64] = "None";
	switch(PlayerInfo[playerid][pGroup])
	{
		case GROUP_HITMAN: //hitman
		{
			switch(PlayerInfo[playerid][pGroupRank])
			{
				case 0:
					rankname = "Initiate";
				case 1:
					rankname = "Freelancer";
				case 2:
					rankname = "Operative";
				case 3:
					rankname = "Agent";
				case 4:
					rankname = "Elite Agent";
				case 5:
					rankname = "Vice Director";
				case 6:
					rankname = "Director";
			}
		}
		case GROUP_CRIME:
		{
			switch(PlayerInfo[playerid][pGroupRank])
			{
				case 5:
					rankname = "Director";
				case 4:
					rankname = "Assistant Director";
				case 3:
					rankname = "Specialist";
				case 2:
					rankname = "Operative";
				case 1:
					rankname = "Inductee";
				case 0:
					rankname = "Outsider";
			}
		}
		case GROUP_REBELS:
		{
			switch(PlayerInfo[playerid][pGroupRank])
			{
				case 5:
					rankname = "Avtoritet";
				case 4:
					rankname = "Derzhatel Obschaka";
				case 3:
					rankname = "Brigadier";
				case 2:
					rankname = "Bratok";
				case 1:
					rankname = "Boyevik";
				case 0:
					rankname = "Shestyorka";
			}
		}
		case GROUP_CARTEL:
		{
			switch(PlayerInfo[playerid][pGroupRank])
			{
				case 4:
					rankname = "Patron";
				case 3:
					rankname = "Capo";
				case 2:
					rankname = "Lugarteniente";
				case 1:
					rankname = "Sicario";
				case 0:
					rankname = "Soldado";
			}
		}
		case GROUP_TIJUANA:
		{
			switch(PlayerInfo[playerid][pGroupRank])
			{
				case 6:
					rankname = "Jefe";
				case 5:
					rankname = "Delegar";
				case 4:
					rankname = "Asociar";
				case 3:
					rankname = "Fideicomisario";
				case 2:
					rankname = "Empleado";
				case 1:
					rankname = "Soldado de Infantería";
				case 0:
					rankname = "Forastero";
			}
		}
		case GROUP_COMMISSION:
		{
			switch(PlayerInfo[playerid][pGroupRank])
			{
				case 5:
					rankname = "CEO";
				case 4:
					rankname = "Underboss";
				case 3:
					rankname = "Shots-caller";
				case 2:
					rankname = "Specialist";
				case 1:
					rankname = "Associate";
				case 0:
					rankname = "Informant";
			}
		}
		case GROUP_EXEC:
		{
			switch(PlayerInfo[playerid][pGroupRank])
			{
				case 5:
					rankname = "Prophet";
				case 4:
					rankname = "Chairman";
				case 3:
					rankname = "High Chancellor";
				case 2:
					rankname = "Sage";
				case 1:
					rankname = "Squire";
				case 0:
					rankname = "Apprentice";
			}
		}
		case GROUP_LLS:
		{
			switch(PlayerInfo[playerid][pGroupRank])
			{
				case 5:
					rankname = "Supremo";
				case 4:
					rankname = "Executive";
				case 3:
					rankname = "Secretary";
				case 2:
					rankname = "Enforcer";
				case 1:
					rankname = "Associate";
				case 0:
					rankname = "Employee";
			}
		}
		case GROUP_OPG:
		{
			switch(PlayerInfo[playerid][pGroupRank])
			{
				case 5:
					rankname = "Baskan (Boss)";
				case 4:
					rankname = "Baskan Yardimcisi (Underboss)";
				case 3:
					rankname = "Danisman (Consultant)";
				case 2:
					rankname = "Gorevli (Attendant)";
				case 1:
					rankname = "Employee (Eleman)";
				case 0:
					rankname = "Comez (Probationar)";
			}
		}
		case GROUP_S9:
		{
			switch(PlayerInfo[playerid][pGroupRank])
			{
				case 4:
					rankname = "Director";
				case 3:
					rankname = "Assistant Director";
				case 2:
					rankname = "Special Agent";
				case 1:
					rankname = "Senior Agent";
				case 0:
					rankname = "Agent";
			}
		}
	}
	return rankname;
}

SendGroupMessage(group, color, string2[])
{
	foreach( new i: Player )
	{
		if(PlayerInfo[i][pGroup] == group)
		{
			SendClientMessage(i, color, string2);
		}
	}
	return 1;
}

forward onOGMembers(playerid);
public onOGMembers(playerid)
{
	if(cache_num_rows() == 0) return SendClientMessage(playerid, COLOR_WHITE, "No offline members to display.");

	new memberstr[10000], lastlog[64], name[MAX_PLAYER_NAME], playerfrank, hanickname[13];

	for(new i = 0; i < cache_num_rows(); i++)
	{
		new temp[10];
		cache_get_field_content(i, "Name", name);
		cache_get_field_content(i, "LastLogin", lastlog);
		cache_get_field_content(i, "GroupRank", temp);
		cache_get_field_content(i, "pGroupNick", hanickname);

		playerfrank = strval(temp);
		if(cache_get_field_content_int(i, "Group") == GROUP_HITMAN) format(memberstr, sizeof(memberstr), "%s{3366FF}%s (%s) (%d)\t\t{FFFFFF}%s\n", memberstr, name, hanickname, playerfrank, lastlog);
		else format(memberstr, sizeof(memberstr), "%s{3366FF}%s (%d)\t\t{FFFFFF}%s\n", memberstr, name, playerfrank, lastlog);
	}

	ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_TABLIST, "Offline Members", memberstr, "Okay", "");
	return 1;
}

forward OnOGUninvite(playerid, name[]);
public OnOGUninvite(playerid, name[])
{
	if(cache_num_rows() != 1) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} That account doesn't exist.");

	if(cache_get_field_content_int(0, "Group") != PlayerInfo[playerid][pGroup]) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} That member does not belong to your group.");
	new str[128];
	format(str, sizeof(str), "* You have kicked %s from your group.", name);
	SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

	new query[300];
	mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `players` SET `Group`= 0,`GroupRank`= 0, `HAFakeBadge`= 0 WHERE `Name`= '%e'", name);
	mysql_pquery(sqlGameConnection, query);
	return 1;
}

Hook:groups_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_TIJUANA_ARMORY)
    {
        if(!response) return 1;
        switch(listitem)
        {
            case 0:
            {
                if(PlayerInfo[playerid][pCash] >= 100000)
                {
                    GiveMoney(playerid, -100000);
                    SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have purchased a Deagle for $100,000.");
                    GivePlayerGun(playerid, 24);
                }
                else return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
            }
            case 1:
            {
                if(PlayerInfo[playerid][pCash] >= 150000)
                {
                    GiveMoney(playerid, -150000);
                    SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have purchased a M4 for $150,000.");
                    GivePlayerGun(playerid, 31);
                }
                else return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
            }
            case 2:
            {
                if(PlayerInfo[playerid][pCash] >= 130000)
                {
                    GiveMoney(playerid, -130000);
                    SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have purchased an AK47 for $130,000.");
                    GivePlayerGun(playerid, 30);
                }
                else return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
            }
            case 3:
            {
                if(PlayerInfo[playerid][pCash] >= 45000)
                {
                    GiveMoney(playerid, -45000);
                    SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have purchased a Full Vest for $45,000.");
                    SetPlayerArmour(playerid, 100.00);
                }
                else return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
            }
            case 4:
            {
               SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have healed yourself using a first aid kit.");
               SetPlayerHealth(playerid, 100.00);
            }
            case 5:
            {
                if(PlayerInfo[playerid][pCash] >= 50000)
                {
                    GiveMoney(playerid, -50000);
                    SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have purchased an MP5 for $50,000.");
                    GivePlayerGun(playerid, 29);
                }
                else return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
            }
            case 6:
            {
                if(PlayerInfo[playerid][pCash] >= 30000)
                {
                    GiveMoney(playerid, -30000);
                    SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have purchased a Shotgun for $30,000.");
                    GivePlayerGun(playerid, 25);
                }
                else return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
            }
        }
    }

    return 1;
}

/* Returns true if the given group ID is a donor group */
isDonatedGroup(groupID) {
	return (
		groupID == GROUP_TIJUANA	||
		groupID == GROUP_COMMISSION	||
		groupID == GROUP_EXEC	||
		groupID == GROUP_LLS	||
		groupID == GROUP_OPG	||
		groupID == GROUP_REBELS
	);
}

// Needs to be moved for organisation
/*IsGroupLeaderOn(groupid) {
	foreach(new i: Player)
		if(PlayerInfo[i][pGroup] == groupid && IsGroupLeader(i))
			return 1;
	return 0;
}*/


/*
	Commands
*/

CMD:quitgroup(playerid, params[])
{
	new str[128];
	new conf[10];
	if(PlayerInfo[playerid][pGroup] == GROUP_NONE) return SendClientMessage(playerid, COLOR_GRAD1, "{FF0000}Error:{FFFFFF} You are not in a Group.");
	if(sscanf(params, "s[10]", conf)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /quitgroup 'confirm'");
	if(strcmp(conf, "confirm", true) == 0)
	{
		format(str, sizeof(str), "* You have quit the %s, you are now a civilian again.", GetPlayerGroupName(playerid));
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		PlayerInfo[playerid][pGroup] = GROUP_NONE;
		PlayerInfo[playerid][pGroupRank] = 0;
		PlayerInfo[playerid][pHAFakeBadge] = 0;
		SetPlayerMasked(playerid, false);
	}
	return 1;
}

CMD:ggiverank(playerid, params[])
{
	new str[128];
	if(PlayerInfo[playerid][pGroup] == GROUP_NONE) return SendClientMessage(playerid, COLOR_GRAD1, "{FF0000}Error:{FFFFFF} You are not in a group.");
	if(!IsGroupLeader(playerid)) return SendClientMessage(playerid, COLOR_GRAD1, "{FF0000}Error:{FFFFFF} You are not the group leader.");

	new giveplayerid, newrank;

	format(str, sizeof(str), "{00BFFF}Usage:{FFFFFF} /ggiverank [playerid/PartOfName] [0-%d]", GetGroupMaxRanks(PlayerInfo[playerid][pGroup]));
	if(sscanf(params, "u d", giveplayerid, newrank)) return SendClientMessage(playerid, COLOR_GREY, str);
	if(giveplayerid == playerid) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} You can't change your own rank.");
	if(!IsPlayerConnected(giveplayerid)) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} That player isn't connected.");
	if(PlayerInfo[giveplayerid][pGroup] != PlayerInfo[playerid][pGroup]) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} That player is not a member of your group.");

	format(str, sizeof(str), "{FF0000}Error:{FFFFFF} Your factions ranks go from 0 to %d.", GetGroupMaxRanks(PlayerInfo[playerid][pGroup]));
	if(newrank < 0 || newrank > GetGroupMaxRanks(PlayerInfo[playerid][pGroup])) return SendClientMessage(playerid, COLOR_GREY, str);

	if(PlayerInfo[giveplayerid][pGroupRank] > newrank)
	{
		format(str, sizeof(str), "* You have demoted %s to rank %d.", PlayerICName(giveplayerid), newrank);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

		format(str, sizeof(str), "* You have been demoted to Rank %d by Leader %s.", newrank, PlayerICName(playerid));
		SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
	}
	else
	{
		format(str, sizeof(str), "* You have been promoted to Rank %d by Leader %s.", newrank, PlayerICName(playerid));
		SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);

		format(str, sizeof(str), "* You have promoted %s to rank %d.", PlayerICName(giveplayerid), newrank);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
	}
	PlayerInfo[giveplayerid][pGroupRank] = newrank;
	return 1;
}

CMD:grouphelp(playerid)
{
	SendClientMessage(playerid, GetGroupColour(PlayerInfo[playerid][pGroup]), "____________________________________________");
	
	switch(PlayerInfo[playerid][pGroup]) {
		case GROUP_HITMAN: {
			SendClientMessage(playerid, COLOR_WHITE, "COMMANDS: /order /contracts /givehit /ranks /profile /poisonfood");
			SendClientMessage(playerid, COLOR_WHITE, "COMMANDS: /disguise /callerid /takebadge /placebadge /fakebadge /plantbomb /(h)itm(a)n(u)nder(c)over");
			SendClientMessage(playerid, COLOR_WHITE, "COMMANDS: /placejammer /crushjammer /removejammer /jammer /find");
		}
		case GROUP_CRIME:
		{
			SendClientMessage(playerid, COLOR_WHITE, "COMMANDS: /disguise /mask /syndicate /undercover");
		}
		case GROUP_CARTEL:
		{
			SendClientMessage(playerid, COLOR_WHITE, "COMMANDS: /mask /disguise /cjng /undercover");
		}
		case GROUP_S9:
		{
			SendClientMessage(playerid, COLOR_WHITE, "COMMANDS: /mask /disguise /s9 /profile");
			SendClientMessage(playerid, COLOR_WHITE, "COMMANDS: /callerid /fire /profile /fd");
			SendClientMessage(playerid, COLOR_WHITE, "COMMANDS: /edits9badge, /deploydrone /controldrone");
			SendClientMessage(playerid, COLOR_WHITE, "COMMANDS: /placejammer /crushjammer /removejammer /jammer /find");
		}
		case GROUP_LLS:
		{	
			SendClientMessage(playerid, COLOR_WHITE, "COMMANDS: /order");
		}
		
	}

	if(isDonatedGroup(PlayerInfo[playerid][pGroup]))
		SendClientMessage(playerid, COLOR_WHITE, "COMMANDS: /disguise /callerid /callerid");

	SendClientMessage(playerid, COLOR_GREY, "COMMANDS: /ginvite /guninvite /quitgroup /ggiverank /(g)roup(r)adio /(g)roup(o)oc(r)adio /gmembers");
	SendClientMessage(playerid, COLOR_GREY, "COMMANDS: /ogmembers /oguninvite /togglegroupradio (/togglegr) /togglegroupoocradio (/togglegor)");
	return 1;
}
CMD:ghelp(playerid) return cmd_grouphelp(playerid);

CMD:gmembers(playerid, params[])
{
	if(PlayerInfo[playerid][pGroup] == GROUP_NONE) return SendClientMessage(playerid, COLOR_GRAD1, "{FF0000}Error:{FFFFFF} You are not in a Group.");
	new memberstr[3600];
	foreach( new i: Player )
	{
		if(PlayerInfo[i][pGroup] == PlayerInfo[playerid][pGroup])
		{
			if(PlayerPaused[i]) 
			{
				if(PlayerInfo[playerid][pGroup] == GROUP_HITMAN) format(memberstr, sizeof(memberstr), "%s[{E9CF8D}Paused{FFFFFF}] %s (%s) - Rank: %s(%d){FFFFFF}\n", memberstr, PlayerOOCName(i), PlayerInfo[i][pGroupNick], GetPlayerGroupRankName(i), PlayerInfo[i][pGroupRank]);
				else format(memberstr, sizeof(memberstr), "%s[{E9CF8D}Paused{FFFFFF}] %s (%s) - Rank: %s(%d){FFFFFF}\n", memberstr, PlayerOOCName(i), PlayerInfo[i][pGroupNick], GetPlayerGroupRankName(i), PlayerInfo[i][pGroupRank]);
			}		
			else 
			{
				if(PlayerInfo[playerid][pGroup] == GROUP_HITMAN) format(memberstr, sizeof(memberstr), "%s%s (%s) - Rank: %s(%d){FFFFFF}\n", memberstr, PlayerOOCName(i), PlayerInfo[i][pGroupNick], GetPlayerGroupRankName(i), PlayerInfo[i][pGroupRank]);
				else format(memberstr, sizeof(memberstr), "%s%s (%s) - Rank: %s(%d){FFFFFF}\n", memberstr, PlayerOOCName(i), PlayerInfo[i][pGroupNick], GetPlayerGroupRankName(i), PlayerInfo[i][pGroupRank]);
			}	
		}
		ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_LIST, "Group Members Online", memberstr, "Close", "");
	}
	return 1;
}

CMD:ogmembers(playerid, params[])
{
	if(PlayerInfo[playerid][pGroup] > 0)
	{
		new query[128];
		mysql_format(sqlGameConnection, query, sizeof(query), "SELECT `Name`, `LastLogin`, `Group`, `GroupRank`, `GroupNick` FROM `players` WHERE `Adjustable`=1 AND `Group`=%d", PlayerInfo[playerid][pGroup]);
		mysql_pquery(sqlGameConnection, query, "onOGMembers", "i", playerid);
	}
	else
	{
		SendClientMessage(playerid, COLOR_WHITE, "You're not in a group.");
	}
	return 1;
}

CMD:ginvite(playerid, params[])
{
	if(PlayerInfo[playerid][pGroup] == GROUP_NONE) return SendClientMessage(playerid, COLOR_GRAD1, "{FF0000}Error:{FFFFFF} You are not in a group.");
	if(PlayerInfo[playerid][pGroupRank] < GetGroupMaxRanks(PlayerInfo[playerid][pGroup]) - 1) return SendClientMessage(playerid, COLOR_GRAD1, "{FF0000}Error:{FFFFFF} You are not the group leader.");

	new giveplayerid;
	new str[128];

	if(sscanf(params, "u", giveplayerid)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /ginvite [playerid/PartOfName]");
	if(!IsPlayerConnected(giveplayerid)) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} Invalid playerid.");

	// To avoid MG which has been happening recently.
	if(PlayerInfo[giveplayerid][pGroup] == GROUP_S9) {
		format(str, sizeof(str), "* You have invited %s to join %s.", PlayerICName(giveplayerid), GetPlayerGroupName(playerid));
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

		format(str, sizeof(str), "%s has just tried to invite you to join the %s. But S9 is gr8.", PlayerOOCName(giveplayerid), GetPlayerGroupName(playerid));
		SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
		return 1;
	}

	if(PlayerInfo[giveplayerid][pGroup] > 0) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} That player is already in a group.");

	format(str, sizeof(str), "* You have invited %s to join %s.", PlayerICName(giveplayerid), GetPlayerGroupName(playerid));
	SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

	format(str, sizeof(str), "* %s has invited you to join %s, (type /accept group) to accept.", PlayerICName(playerid), GetPlayerGroupName(playerid));
	SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
	GroupOffer[giveplayerid] = playerid;
	return 1;
}

CMD:guninvite(playerid, params[])
{
	if(PlayerInfo[playerid][pGroup] == GROUP_NONE) return SendClientMessage(playerid, COLOR_GRAD1, "{FF0000}Error:{FFFFFF} You are not in a group.");
	if(PlayerInfo[playerid][pGroupRank] < GetGroupMaxRanks(PlayerInfo[playerid][pGroup]) - 1) return SendClientMessage(playerid, COLOR_GRAD1, "{FF0000}Error:{FFFFFF} You are not the group leader.");

	new giveplayerid;
	new str[128];
	if(sscanf(params, "u", giveplayerid)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /guninvite [playerid/PartOfName]");
	if(giveplayerid == playerid) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} You can't uninvite yourself.");
	if(!IsPlayerConnected(giveplayerid)) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} Invalid playerid.");
	if(PlayerInfo[giveplayerid][pGroup] != PlayerInfo[playerid][pGroup]) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} That player is not in your group.");
	if(PlayerInfo[giveplayerid][pGroupRank] >= PlayerInfo[playerid][pGroupRank]) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} You can't kick out other group leaders.");

	format(str, sizeof(str), "* You have been kicked out of the %s by Leader %s.", GetPlayerGroupName(giveplayerid), PlayerICName(playerid));
	SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);

	format(str, sizeof(str), "* You have kicked out %s from the group.", PlayerOOCName(giveplayerid));
	SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

	PlayerInfo[giveplayerid][pGroup] = GROUP_NONE;
	PlayerInfo[giveplayerid][pGroupRank] = 0;
	PlayerInfo[giveplayerid][pHAFakeBadge] = 0;
	SetPlayerMasked(giveplayerid, false);
	return 1;
}

CMD:togor(playerid, params[])
{
	if(PlayerInfo[playerid][pMember] != 0)
	{
		if(ToggedOR[playerid] == false)
		{
			ToggedOR[playerid] = true;
			SendClientMessage(playerid, COLOR_WHITE, "You have disabled the OOC radio channel.");
		}
		else
		{
			ToggedOR[playerid] = false;
			SendClientMessage(playerid, COLOR_WHITE, "You have re-enabled the OOC radio channel.");
		}
	}
	else return SendClientMessage(playerid, COLOR_GREY, "You're not in a faction.");
	return 1;
}

CMD:makegroupleader(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 1337 || PlayerInfo[playerid][pFactionMod] > 0)
	{
		new giveplayerid, group;
		new str[128];
		if(sscanf(params, "ui", giveplayerid, group)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /makegroupleader [playerid/PartOfName] [number]");
		if(!IsPlayerConnected(giveplayerid)) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} Invalid playerid.");
		if(group < 0 || group > MAX_GROUPS) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} Group cannot be below 0 or above 10.");

	/*	if((group == 10 || group == 1) && PlayerInfo[playerid][pFactionDirector] != 1) {
			if(playerid == giveplayerid)
				format(str, sizeof(str), "%s just tried to put his self into HA or S9.", PlayerName(playerid));
			else
				format(str, sizeof(str), "%s just tried to make %s a member of S9 or HA.", PlayerName(playerid), PlayerName(giveplayerid));
			ABroadCast(COLOR_LIGHTYELLOW, str, 1);
			return SendClientMessage(playerid, COLOR_GREY, "You cannot make yourself a member of S9 or HA.");
		} */

		PlayerInfo[giveplayerid][pGroup] = group;
		PlayerInfo[giveplayerid][pGroupRank] = GetGroupMaxRanks(group);

		format(str, sizeof(str), "* You have given %s leadership of the %s.", PlayerOOCName(giveplayerid), GetPlayerGroupName(giveplayerid));
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		format(str, sizeof(str), "* %s has given you leadership of the %s.", PlayerOOCName(playerid), GetPlayerGroupName(giveplayerid));
		SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
	}
	else
	{
		return AdmErrorMsg;
	}
	return 1;
}

CMD:oguninvite(playerid, params[])
{
	// If the player is in a group, and they are co-leader, or leader, then you can uninvite. GetGroupMaxRanks -1 for co-leader+
	if(PlayerInfo[playerid][pGroup] != GROUP_NONE && PlayerInfo[playerid][pGroupRank] >= GetGroupMaxRanks(PlayerInfo[playerid][pGroup])-1)
	{
		new name[MAX_PLAYER_NAME];
		if(sscanf(params, "s[24]", name)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /oguninvite [full name(case sensitive)]");
		if(GetPlayerIDFromName(name) != INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} That player is currently logged in.");

		new query[128];
		mysql_format(sqlGameConnection, query, sizeof(query), "SELECT `GroupRank`, `Group` FROM `players` WHERE `Name` = '%e' LIMIT 1", name);
		mysql_pquery(sqlGameConnection, query, "OnOGUninvite", "is", playerid, name);
	}
	else
	{
		SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} You are not a group leader.");
	}
	return 1;
}

CMD:setgrouprank(playerid, params[]) {
	if(PlayerInfo[playerid][pAdmin] >= 2 && PlayerInfo[playerid][pGroup] != GROUP_NONE) {
		new toplayer, rank, grStr[128];
		if(sscanf(params, "ui", toplayer, rank)) return SendClientMessage(playerid, COLOR_GREY, "/setgrouprank [player] [rank]");

		// If the requested rank is greater than the group's max rank or less than 0
		if(rank > GetGroupMaxRanks(PlayerInfo[toplayer][pGroup]) || rank < 0) {
			format(grStr, sizeof(grStr), "Rank must be between 0 and %d", GetGroupMaxRanks(PlayerInfo[toplayer][pGroup]));
			return SendClientMessage(playerid, COLOR_RED, grStr);
		}

		if(PlayerInfo[playerid][pGroup] != PlayerInfo[toplayer][pGroup]) return SendClientMessage(playerid, COLOR_GREY, "That player is not in your group.");

		PlayerInfo[toplayer][pGroupRank] = rank;

		format(grStr, sizeof(grStr), "You have set %s's rank to %d.", PlayerOOCName(toplayer), rank);
		SendClientMessage(playerid, GetGroupColour(PlayerInfo[toplayer][pGroup]), grStr);

		format(grStr, sizeof(grStr), "An admin has set your group rank to %d", rank);
		SendClientMessage(toplayer, GetGroupColour(PlayerInfo[toplayer][pGroup]), grStr);
	}

	return 1;
}

// For faction leader
CMD:togglegroupradio(playerid) {
	new playerGroup = PlayerInfo[playerid][pGroup];

	if(playerGroup != GROUP_NONE && PlayerInfo[playerid][pGroupRank] >= GetGroupMaxRanks(playerGroup)-1) {
		switch(groupRadioToggled[playerGroup])
		{
			case 0:
			{
				groupRadioToggled[playerGroup] = 1;
				SendClientMessage(playerid, GetGroupColour(playerGroup), "You have turned group radio off.");
			}
			case 1:
			{
				groupRadioToggled[playerGroup] = 0;
				SendClientMessage(playerid, GetGroupColour(playerGroup), "You have turned group radio on.");
			}
		}
	}
	return 1;
}
CMD:togglegr(playerid) return cmd_togglegroupradio(playerid);

// For faction leader
CMD:togglegroupoocradio(playerid) {
	new playerGroup = PlayerInfo[playerid][pGroup];

	if(playerGroup != GROUP_NONE && PlayerInfo[playerid][pGroupRank] >= GetGroupMaxRanks(playerGroup)-1) {
		switch(OOCgroupRadioToggled[playerGroup])
		{
			case 0:
			{
				OOCgroupRadioToggled[playerGroup] = 1;
				SendClientMessage(playerid, GetGroupColour(playerGroup), "You have turned the OOC group radio off.");
			}
			case 1:
			{
				OOCgroupRadioToggled[playerGroup] = 0;
				SendClientMessage(playerid, GetGroupColour(playerGroup), "You have turned the OOC group radio on.");
			}
		}
	}
	return 1;
}
CMD:togglegor(playerid) return cmd_togglegroupoocradio(playerid);

CMD:groupradio(playerid, params[])
{
	if(PlayerInfo[playerid][pGroup] == GROUP_NONE)
		return SendClientMessage(playerid, COLOR_GRAD1, "{FF0000}Error:{FFFFFF} You are not in a Group.");

	// If the player's group radio is toggled, and they aren't a leader.
	if(groupRadioToggled[PlayerInfo[playerid][pGroup]] && PlayerInfo[playerid][pGroupRank] < GetGroupMaxRanks(PlayerInfo[playerid][pGroup])-1) 
		return SendClientMessage(playerid, COLOR_GREY, "The group radio is disabled.");

	new msg[128];

	if(sscanf(params, "s[128]", msg)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} (/g)roup(r)adio [message]");
	if(CheckForJammer(playerid)) return SendClientMessage(playerid, COLOR_GREY, "Radio signal has been jammed.");
	new str[128];
	if(PlayerInfo[playerid][pGroup] == GROUP_HITMAN) format(str, sizeof(str), "** %s %s (%s): %s **", GetPlayerGroupRankName(playerid), PlayerICName(playerid), PlayerInfo[playerid][pGroupNick], msg);
	else format(str, sizeof(str), "** %s %s: %s **", GetPlayerGroupRankName(playerid), PlayerICName(playerid), msg);

	SendGroupMessage(PlayerInfo[playerid][pGroup], GetGroupColour(PlayerInfo[playerid][pGroup]), str);

	// For FBI | DEA bugs
	foreach( new i: Player )
	{
		if(PlayerInfo[i][pWire] == 1 && !CheckForJammer(playerid) && !CheckForJammer(i))
		{
			new Float: x[3];
			GetPlayerPos(playerid, x[0], x[1], x[2]);
			if(IsPlayerInRangeOfPoint(i, 20.0, x[0], x[1], x[2]))
			{
				if(i == playerid)
				{
					format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s (radio): %s", PlayerOOCName(i), msg);
				}
				else
				{
					format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s's surroundings (radio): %s", PlayerOOCName(i), msg);
				}
				if(PlayerInfo[i][pMember] != 2) // if player isn't FBI, send to both FBI and S9
				{
					SendBuggedMessage(COLOR_ORANGE, str);
					SendS9BuggedMessage(COLOR_ORANGE, str);
				}	
				else SendS9BuggedMessage(COLOR_ORANGE, str); // if player is FBI, send to S9 only, this is to avoid FBI figuring out
				// 																							that s9 bugged them
			}
		}
	}
	
	return 1;
}

CMD:groupoocradio(playerid, params[])
{
	if(PlayerInfo[playerid][pGroup] == GROUP_NONE)
		return SendClientMessage(playerid, COLOR_GRAD1, "{FF0000}Error:{FFFFFF} You are not in a Group.");

	if(OOCgroupRadioToggled[PlayerInfo[playerid][pGroup]] && PlayerInfo[playerid][pGroupRank] < GetGroupMaxRanks(PlayerInfo[playerid][pGroup])-1) 
		return SendClientMessage(playerid, COLOR_GREY, "The group OOC radio is disabled.");

	new msg[128];
	new str[128];
	if(sscanf(params, "s[128]", msg)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /(g)roup(o)oc(r)adio [message]");
	if(PlayerInfo[playerid][pGroup] == GROUP_HITMAN) format(str, sizeof(str), "** (( %s %s (%s): %s )) **", GetPlayerGroupRankName(playerid), PlayerICName(playerid), PlayerInfo[playerid][pGroupNick], msg);
	else format(str, sizeof(str), "** (( %s %s: %s )) **", GetPlayerGroupRankName(playerid), PlayerICName(playerid), msg);

	SendGroupMessage(PlayerInfo[playerid][pGroup], GetGroupColour(PlayerInfo[playerid][pGroup]), str);
	
	return 1;
}

CMD:g(playerid, params[]) return cmd_groupradio(playerid, params);
CMD:gr(playerid, params[]) return cmd_groupradio(playerid, params);
CMD:gor(playerid, params[]) return cmd_groupoocradio(playerid, params);

/* 
	Tijuana Cartel Armory - approved by Smo and scripted by Hank
*/

CMD:crarmory(playerid)
{
   if(PlayerInfo[playerid][pGroup] != GROUP_TIJUANA) return SendClientMessage(playerid, COLOR_GREY, "You're not authorized to use this command.");
   if(!IsPlayerInRangeOfPoint(playerid, 5.0, 1091.78, 2121.75, 15.35)) return SendClientMessage(playerid, COLOR_GREY,"You're not at the armory of the Network.");
   new cartelstr[1000];
   format(cartelstr, sizeof(cartelstr), "Item\tPrice");
   format(cartelstr, sizeof(cartelstr), "%s\nDeagle\t$100,000\nM4\t$150,000\nAK47\t$130,000\nFull Vest\t$45,000", cartelstr);
   format(cartelstr, sizeof(cartelstr), "%s\nFull HP\tFree\nMP5\t$50,000\nShotgun\t$30,000", cartelstr);
   ShowPlayerDialog(playerid, DIALOG_TIJUANA_ARMORY, DIALOG_STYLE_TABLIST_HEADERS, "Cartel Armory", cartelstr, "Purchase", "Cancel");
   return 1;
}

CMD:sethanickname(playerid, params[])
{
	if(PlayerInfo[playerid][pGroup] != GROUP_HITMAN) return AdmErrorMsg;
	new id, name[13];
	if(sscanf(params, "us[13]", id, name)) return SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} /sethanickname [playerid/PartOfName] [nickname] (12 characters max)");
	if(!IsPlayerConnected(id) || id == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_GREY, "Invalid player.");
	if(PlayerInfo[id][pGroup] != GROUP_HITMAN) return SendClientMessage(playerid, COLOR_GREY, "That player is not in your group.");
	
	format(PlayerInfo[id][pGroupNick], 12, "%s", name);

	new msg[70];
	format(msg, sizeof(msg), "You have set %s's nickname to %s.", PlayerOOCName(id), PlayerInfo[id][pGroupNick]);
	SendClientMessage(playerid, GetGroupColour(PlayerInfo[playerid][pGroup]), msg);

	format(msg, sizeof(msg), "%s has set your nickname to %s", PlayerOOCName(playerid), PlayerInfo[id][pGroupNick]);
	SendClientMessage(id, GetGroupColour(PlayerInfo[playerid][pGroup]), msg);
	return 1;
}