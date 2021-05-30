new PlayerBar:AdrenalineBar[ MAX_PLAYERS ], bool:UsingAdrenaline[ MAX_PLAYERS ], PlayerUsingAdrenalineOn[MAX_PLAYERS], Float:ThatPlayerX[MAX_PLAYERS], Float:ThatPlayerY[MAX_PLAYERS], Float:ThatPlayerZ[MAX_PLAYERS];

forward CancelUsingAdrenaline(playerid);
public CancelUsingAdrenaline(playerid)
{
	UsingAdrenaline[playerid] = false;
	SetPlayerProgressBarValue(playerid, AdrenalineBar[playerid], 0);
	HidePlayerProgressBar(playerid, AdrenalineBar[playerid]);
	return 1;

}

Hook:adr_OnPlayerConnect(playerid)
{
	ThatPlayerX[playerid] = -1;
	ThatPlayerY[playerid] = -1;
	ThatPlayerZ[playerid] = -1;
	PlayerUsingAdrenalineOn[playerid] = INVALID_PLAYER_ID;
	AdrenalineBar[playerid] = CreatePlayerProgressBar(playerid,  290.0, 340.00, 65.0, 5.0, 0xFFFF0000, 350.0, BAR_DIRECTION_RIGHT);
	UsingAdrenaline[playerid] = false;
	return 1;
}

Hook:adr_OnPlayerDisconnect(playerid, reason)
{
	CancelUsingAdrenaline(playerid);
	DestroyPlayerProgressBar(playerid, AdrenalineBar[playerid]);
	return 1;
}

Hook:adr_OnPlayerUpdate(playerid)
{
	if(UsingAdrenaline[playerid] == true && !IsPlayerInRangeOfPoint(playerid, 5.0, ThatPlayerX[playerid], ThatPlayerY[playerid], ThatPlayerZ[playerid]))
	{
		PlayerInfo[playerid][pAdrenaline] = 0;
		CancelUsingAdrenaline(playerid);
		SendClientMessage(playerid, COLOR_GREY, "You've moved away from your patient so you stopped reviving him!");
	}
	return 1;
}

Hook:adr_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if((newkeys & KEY_SPRINT) && !(oldkeys & KEY_SPRINT))
	{
		if(UsingAdrenaline[playerid] != false)
		{
			if(GetPlayerProgressBarValue(playerid, AdrenalineBar[playerid]) < 350)
			{
				SetPlayerProgressBarValue(playerid, AdrenalineBar[playerid], GetPlayerProgressBarValue(playerid, AdrenalineBar[playerid])+0.75);
				if(GetPlayerProgressBarValue(playerid, AdrenalineBar[playerid]) >= 350)
				{
					new str[250];
					SetPlayerHealth(PlayerUsingAdrenalineOn[playerid], 100);
					SetPlayerArmour(PlayerUsingAdrenalineOn[playerid], 0);
					TextDrawHideForPlayer(PlayerUsingAdrenalineOn[playerid], Text:DieText);
					TextDrawHideForPlayer(PlayerUsingAdrenalineOn[playerid], Text:InjuredText);
					format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has FINISHED used adrenaline on %s.", PlayerICName(playerid), PlayerICName(PlayerUsingAdrenalineOn[playerid]));
					ABroadCast(COLOR_WHITE, str, 1);
					pInHospital[PlayerUsingAdrenalineOn[playerid]] = 0;
					pDeathState[PlayerUsingAdrenalineOn[playerid]] = PLAYER_STATE_NORMAL;
					PlayerCalledEMS[PlayerUsingAdrenalineOn[playerid]] = 0;
					ResetPlayerDamages(PlayerUsingAdrenalineOn[playerid]);
					ApplyAnimationEx(PlayerUsingAdrenalineOn[playerid], "PED", "getup_front", 4.0, 0, 1, 1, 0, 0, 1);
					for(new i = 0; i <= 12; i++)
					{
						lastpguns[playerid][i][0] = 0;
						lastpguns[playerid][i][1] = 0;
					}
					OnPlayerSave(PlayerUsingAdrenalineOn[playerid]);
					format(str, sizeof(str), "* %s has used adrenaline on %s", PlayerICName(playerid), PlayerICName(PlayerUsingAdrenalineOn[playerid]));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					PlayerInfo[playerid][pAdrenaline] = 0;
					HidePlayerProgressBar(playerid, AdrenalineBar[playerid]);
					UsingAdrenaline[playerid] = false;
					ThatPlayerX[playerid] = -1;
					ThatPlayerY[playerid] = -1;
					ThatPlayerZ[playerid] = -1;
				}
			}
		}
	}
	return 1;
}

CMD:useadrenaline(playerid, params[])
{
	if(PlayerInfo[playerid][pAdrenaline] < 1)
	{
		AdmErrorMsg;
		return 1;
	}
	new toplayerid;
	new str[250];
	if(!sscanf(params, "u", toplayerid))
	{
		if(IsPlayerConnected(toplayerid))
		{
			if(toplayerid != INVALID_PLAYER_ID)
			{
				if(GetDistanceBetweenPlayers(playerid, toplayerid) > 6 || GetPlayerState(toplayerid) == PLAYER_STATE_SPECTATING)
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is not in range.");
					return 1;
				}
				if(toplayerid == playerid) return SendClientMessage(playerid, COLOR_GREY, "You can't use this command on yourself");
				if(pDeathState[toplayerid] == PLAYER_STATE_REVIVE)
				{
					new Float:x, Float:y, Float:z;
					GetPlayerPos(toplayerid, Float:x, Float:y, Float:z);
					ThatPlayerX[playerid] = x;
					ThatPlayerY[playerid] = y;
					ThatPlayerZ[playerid] = z;
					PlayerUsingAdrenalineOn[playerid] = toplayerid;
					format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has STARTED using an adrenaline on %s.", PlayerICName(playerid), PlayerICName(toplayerid));
					ABroadCast(COLOR_WHITE, str, 1);
					SetPlayerProgressBarValue(playerid, AdrenalineBar[playerid], 0);
					ShowPlayerProgressBar(playerid, AdrenalineBar[playerid]);
					UsingAdrenaline[playerid] = true;
					PlayerInfo[playerid][pAdrenaline] = 0;
					SendClientMessage(playerid, -1, "Rapidly start tapping ''SPACE'' to revive your patient!");
					/*SetPlayerHealth(toplayerid, 100);
					SetPlayerArmour(toplayerid, 0);
					//ClearAnimationsEx(toplayerid);
					TextDrawHideForPlayer(toplayerid, Text:DieText);
					TextDrawHideForPlayer(toplayerid, Text:InjuredText);
					format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has used adrenaline on %s.", PlayerICName(playerid), PlayerICName(toplayerid));
					ABroadCast(COLOR_WHITE, str, 1);
					pInHospital[toplayerid] = 0;
					pDeathState[toplayerid] = PLAYER_STATE_NORMAL;
					PlayerCalledEMS[toplayerid] = 0;
					ResetPlayerDamages(toplayerid);
					ApplyAnimationEx(toplayerid, "PED", "getup_front", 4.0, 0, 1, 1, 0, 0, 1);
					for(new i = 0; i <= 12; i++)
					{
						lastpguns[playerid][i][0] = 0;
						lastpguns[playerid][i][1] = 0;
					}
					OnPlayerSave(toplayerid);
					format(str, sizeof(str), "* %s has used adrenaline on %s", PlayerICName(playerid), PlayerICName(toplayerid));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					PlayerInfo[playerid][pAdrenaline] = 0;*/
				}
				else
				{
					SendClientMessage(playerid, COLOR_GRAD1, "Player is not injured.");
				}
			}
		}
	}
	else
	{
		SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /useadrenaline [playerid/PartOfName]");
	}
	return 1;
}

CMD:getadrenaline(playerid, params[])
{
	new str[1024];
	if(PlayerInfo[playerid][pMember] > 4 || PlayerInfo[playerid][pMember] < 4)  return SendClientMessage(playerid, COLOR_GREY, "You're not SAFMD member!");
	if(!IsPlayerInRangeOfPoint(playerid, 2, 1188.70, -1349.31, 1093.86)) return SendClientMessage(playerid, COLOR_GREY, "You must be at SAFMD HQ.");
	if(PlayerInfo[playerid][pAdrenaline] > 0) return SendClientMessage(playerid, COLOR_GREY, "You can't get more than one adrenaline.");
	PlayerInfo[playerid][pAdrenaline] = 1;
	format(str, sizeof(str), "* %s prolongs their hand towards the locker as he takes adrenaline dose.", PlayerICName(playerid));
	ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	return 1;
}

CMD:stealadrenaline(playerid, params[])
{
	new str[1024];
	new now = gettime();
	if(!IsPlayerInRangeOfPoint(playerid, 2, 1188.70, -1349.31, 1093.86)) return SendClientMessage(playerid, COLOR_GREY, "You must be at SAFMD HQ.");
	if(PlayerInfo[playerid][pAdrenaline] > 0) return SendClientMessage(playerid, COLOR_GREY, "You can't get more than one adrenaline.");
	if(now < (AdrenalineSteal[playerid] + 60))
	{
		format(str, sizeof(str), "You have to wait %d seconds before using /stealadrenaline again!", (AdrenalineSteal[playerid] + 60) - now);
		SendClientMessage(playerid, COLOR_GREY, str);
		return 1;
	}
	new Adrenaline = random(2);
	switch(Adrenaline)
	 {
		case 0: 
		{
			format(str, sizeof(str), "* %s prolongs their hand towards the locker as he steals adrenaline dose.", PlayerICName(playerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerInfo[playerid][pAdrenaline] = 1;
		}
		case 1: 
		{
			AdrenalineSteal[playerid] = gettime();
			format(str, sizeof(str), "* %s prolongs their hand towards the locker as he attempts to steal adrenaline dose.", PlayerICName(playerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			format(str, sizeof(str), "* But fails (( %s ))",PlayerICName(playerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			SendClientMessage(playerid, COLOR_PURPLE, "* Your face has been recorded *");
			format(str, sizeof(str), "Dispatch: Security camera has recorded a person trying to steal adrenaline dose.");
			SendParaMessage(COLOR_DOCTOR, str);
			SendFactionMessage(FACTION_SAPD, GPSCOLOR_SAPD, str);
			format(str, sizeof(str), "Description: (( You would see %s on the video ))", PlayerICName(playerid));
			SendParaMessage(COLOR_DOCTOR, str);
			SendFactionMessage(FACTION_SAPD, GPSCOLOR_SAPD, str);

		}
	}
	return 1;
}

CMD:giveadrenaline(playerid, params[])
{
	new plo;
	if(PlayerInfo[playerid][pAdrenaline] < 1) return SendClientMessage(playerid, COLOR_GREY, "You do not have adrenaline dose!");
	if(!sscanf(params, "u", plo))
	{
	if(!IsPlayerInRangeOfPlayer(5.0, playerid, plo)) return SendClientMessage(playerid, COLOR_GREY, "You must be near a player you want to give adrenaline dose!");
	if(PlayerInfo[plo][pAdrenaline] > 0) return SendClientMessage(playerid, COLOR_GREY, "That player already have adrenaline dose!");
	PlayerInfo[playerid][pAdrenaline] = 0;
	PlayerInfo[plo][pAdrenaline] = 1;
	}
	else
	{
		SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /giveadrenaline [playerid/PartOfName]");
	}
	return 1;
}

CMD:dropadrenaline(playerid, params[])
{
	new str[1024];
	if(PlayerInfo[playerid][pAdrenaline] < 1) return SendClientMessage(playerid, COLOR_GREY, "You do not have adrenaline dose!");
	format(str, sizeof(str), "* %s throws his adrenaline dose on the floor.", PlayerICName(playerid));
	ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	PlayerInfo[playerid][pAdrenaline] = 0;
	return 1;
}

CMD:buyadrenaline(playerid, params[])
{
	if(!IsPlayerInRangeOfPoint(playerid, 3.0, 1175.73, -1324.19, 1089.57)) return SendClientMessage(playerid, COLOR_GREY, "You are not by the computer inside the All Saints Hospital.");
	new str[128];
	if(isnull(params)) 
	{
		SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /buyadrenaline confirm");
		SendClientMessage(playerid, COLOR_GREY, "TIP: With adrenaline you can revive your friend while FMD is not around.");
		SendClientMessage(playerid, COLOR_GREY, "TIP: CMDS: /giveadrenaline, /stealadrenaline, /buyadrenaline, /getadrenaline and /useadrenaline.");
		format(str, sizeof(str), "Adrenaline price is $250.000");
		SendClientMessage(playerid, COLOR_GREY, str);
		return 1;
	}
	if(PlayerInfo[playerid][pAdrenaline] == 1) return SendClientMessage(playerid, COLOR_GREY, "You can't have more than one adrenaline");
	if(!strcmp(params, "confirm", true))
	{
		PlayerInfo[playerid][pAdrenaline] = 1;
		GiveMoney(playerid, -250000);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "Congratulations, you have bought adrenaline!");
	}
	return 1;
}