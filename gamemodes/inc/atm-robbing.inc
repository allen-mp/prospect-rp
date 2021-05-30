new bool:isPlayerRobbingATM[MAX_PLAYERS];
new PlayerBar:RobbingATM[MAX_PLAYERS];
new robatmtimer[MAX_PLAYERS];
new UsedRobATM[MAX_PLAYERS];
forward cancelrobbingatm(playerid);
public cancelrobbingatm(playerid)
{
	isPlayerRobbingATM[playerid] = false;
	SetPlayerProgressBarValue(playerid, RobbingATM[playerid], 0);
	HidePlayerProgressBar(playerid, RobbingATM[playerid]);
	KillTimer(robatmtimer[playerid]);
	foreach( new i: Player )
	{
				//if(IsPlayerConnected(i)) // Foreach type "Player" contains the IDS of players that are connected. So this is redundant. //
				//{
				if(PlayerInfo[i][pMember] == 1)
				{
					SetPlayerMarkerForPlayer(i, playerid, TCOLOR_WHITE);
					SetPlayerToTeamColor(playerid);
					backupType[playerid] = 0;
				}
				//}
	}
	return 1;

}

Hook:atm_OnPlayerConnect(playerid)
{
	RobbingATM[playerid] = CreatePlayerProgressBar(playerid,  290.0, 340.00, 65.0, 5.0, 0xFFFF00AA, 100.0, BAR_DIRECTION_RIGHT);
	isPlayerRobbingATM[playerid] = false;
	return 1;
}

Hook:atm_OnPlayerDisconnect(playerid, reason)
{
	cancelrobbingatm(playerid);
	DestroyPlayerProgressBar(playerid, RobbingATM[playerid]);
	return 1;
}

Hook:atm_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if((newkeys & KEY_SPRINT) && !(oldkeys & KEY_SPRINT))
	{
		if(isPlayerRobbingATM[playerid] != false)
		{
			if(GetPlayerProgressBarValue(playerid, RobbingATM[playerid]) < 100)
			{
				SetPlayerProgressBarValue(playerid, RobbingATM[playerid], GetPlayerProgressBarValue(playerid, RobbingATM[playerid])+0.75);
				if(GetPlayerProgressBarValue(playerid, RobbingATM[playerid]) >= 100)
				{
				   	new str[1024];
					new cashmoney = random(5000) + 5000;
					GiveMoney(playerid, cashmoney);
					format(str, sizeof(str), "You've successfully robbed %d from the ATM.", cashmoney);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					HidePlayerProgressBar(playerid, RobbingATM[playerid]);
					isPlayerRobbingATM[playerid] = false;
					foreach( new i: Player )
					{
						//if(IsPlayerConnected(i)) // Foreach type "Player" contains the IDS of players that are connected. So this is redundant. //
					//{
						if(PlayerInfo[i][pMember] == 1)
					{
					SetPlayerMarkerForPlayer(i, playerid, TCOLOR_WHITE);
					SetPlayerToTeamColor(playerid);
					backupType[playerid] = 0;
					}
				//}
				}

				}
			}
		}
	}
	return 1;
}

Hook:atm_OnPlayerUpdate(playerid)
{
	if(isPlayerRobbingATM[playerid] == true && !IsAtATM(playerid))
	{
		cancelrobbingatm(playerid);
		SendClientMessage(playerid, COLOR_GREY, "You've moved away from ATM so you stopped robbing it!");
	}
	return 1;
}
CMD:robatm(playerid, params[])
{
	new str[1024];
	new now = gettime();
	if(!IsAtATM(playerid)) return SendClientMessage(playerid, COLOR_GRAD2, "You are not at an ATM machine.");
	if(PlayerInfo[playerid][pScrew] < 1) return SendClientMessage(playerid, COLOR_GREY, "You need a screwdriver to do this.");
	if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can't use that command whilst being in a vehicle!");
	if(now < (UsedRobATM[playerid] + 250))
	{
		format(str, sizeof(str), "You have to wait %d seconds before you can use this command again!", (UsedRobATM[playerid] + 250) - now);
		SendClientMessage(playerid, COLOR_GREY, str);
		return 1;
	}
	isPlayerRobbingATM[playerid] = true;
	SetPlayerProgressBarValue(playerid, RobbingATM[playerid], 0);
	ShowPlayerProgressBar(playerid, RobbingATM[playerid]);
	SendClientMessage(playerid, -1, "You are now robbing ATM! Press {FF0000}SPACE {FFFFFF}repeatedly to rob it.");
	format(str, sizeof(str), "HQ: All units, camera has recorded %s's face (( DO NOT MG THE NAME )) robbing an ATM, check your GPS for the location!", PlayerICName(playerid));
	SendRadioMessage(1, TEAM_BLUE_COLOR, str);
	foreach( new i: Player )
	{
		if(IsPlayerConnected(i))
		{
			if(PlayerInfo[i][pMember] == 1 && !CheckForJammer(i))
			{
				SetPlayerMarkerForPlayer(i, playerid, 0x2641FEFF);
			}
		}
	}
	UsedRobATM[playerid] = gettime();
	return 1;
}