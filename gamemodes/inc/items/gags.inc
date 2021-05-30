/*

	file:			items/gags.inc

	description:	Item that makes people unable to speak.

	author:			Jay Cortez

	date created:	~2017

*/

#define GAG_OBJ_INDEX1		8
#define GAG_OBJ_INDEX2		9

// Amount of minutes until the gag will auto remove itself
#define AUTO_REMOVE_GAG		30

CMD:gaghelp(playerid, params[])
{
	SendClientMessage(playerid, COLOR_YELLOW, "____________________________________________");
	SendClientMessage(playerid, COLOR_WHITE, "COMMANDS: /gag, /removegag");
	SendClientMessage(playerid, COLOR_WHITE, "You will automatically spit out your gag after ~30 minutes.");
	return 1;
}

CMD:gag(playerid, params[])
{
	if(!PlayerInfo[playerid][pGag])
		return SendClientMessage(playerid, COLOR_GREY, "You don't have any gags.");

	new gaggedPlayer;
	if(sscanf(params, "u", gaggedPlayer))
		return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /gag [playerid/partOfName]");
	
	if(!IsPlayerConnected(gaggedPlayer))
		return SendClientMessage(playerid, COLOR_GREY, "Unknown player.");

	if(	GetDistanceBetweenPlayers(playerid, gaggedPlayer) > 4 ||
		GetPlayerState(gaggedPlayer) == PLAYER_STATE_SPECTATING ||
		PlayerInfo[gaggedPlayer][pMask]
	)
		return SendClientMessage(playerid, COLOR_GREY, "You are not close enough to this person.");	

	if(IsPlayerGagged(gaggedPlayer))
		return SendClientMessage(playerid, COLOR_GREY, "This person is already gagged.");	

	if(	PlayerTied[gaggedPlayer] ||
		PlayerCuffed[gaggedPlayer] ||
		PlayerTazed[gaggedPlayer]) {
		ApplyGag(playerid, gaggedPlayer);
		gagRequester[gaggedPlayer] = INVALID_PLAYER_ID;
	}
	else {
		new gagStr[128];

		format(gagStr, sizeof(gagStr), "* You offered to gag %s.", PlayerICName(gaggedPlayer));
		SendClientMessage(playerid, COLOR_LIGHTBLUE, gagStr);

		format(gagStr, sizeof(gagStr), "* %s wants to gag you, (type /accept gag) to accept.", PlayerICName(playerid));
		SendClientMessage(gaggedPlayer, COLOR_LIGHTBLUE, gagStr);

		gagRequester[gaggedPlayer] = playerid;
	}
	return 1;
}

CMD:removegag(playerid, params[])
{
	new gaggedPlayer;
	if(sscanf(params, "u", gaggedPlayer))
		return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /removegag [playerid/partOfName]");
	
	if(!IsPlayerConnected(gaggedPlayer))
		return SendClientMessage(playerid, COLOR_GREY, "Unknown player.");

	if(	GetDistanceBetweenPlayers(playerid, gaggedPlayer) > 4 ||
		GetPlayerState(gaggedPlayer) == PLAYER_STATE_SPECTATING ||
		PlayerInfo[gaggedPlayer][pMask]
	)
		return SendClientMessage(playerid, COLOR_GREY, "You are not close enough to this person.");	

	if(	PlayerTied[playerid] ||
		PlayerCuffed[playerid] ||
		PlayerTazed[playerid]
	)
		return SendClientMessage(playerid, COLOR_GREY, "You cannot do this while restrained.");	

	if(!IsPlayerGagged(gaggedPlayer))
		return SendClientMessage(playerid, COLOR_GREY, "This person is not gagged.");

	// Remove it
	gagExpire[gaggedPlayer] = 0;
	RemoveGagObject(gaggedPlayer);

	// Action
	new gagAction[128];

	if(gaggedPlayer == playerid) {
		format(gagAction, sizeof(gagAction), "* %s rips the gag out of their own mouth.", PlayerICName(playerid));
	}
	else {
		format(gagAction, sizeof(gagAction), "* %s rips the gag out of %s's mouth.", PlayerICName(playerid), PlayerICName(gaggedPlayer));
	}
	
	ProxDetector(30.0, playerid, gagAction, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	return 1;
}

stock IsPlayerGagged(playerid) {
	return (gagExpire[playerid] != 0);
}

stock ApplyGag(gagger, gaggedPlayer) {
	PlayerInfo[gagger][pGag]--;

	// Animation
	if(!IsPlayerInAnyVehicle(gagger) && gagger != gaggedPlayer) {
		SetPlayerFacingPlayer(gagger, gaggedPlayer);
		OnePlayAnim(gagger, "BD_FIRE", "wash_up", 4.0, 0, 0, 0, 0, 0);
	}

	// Set gag expire
	gagExpire[gaggedPlayer] = gettime() + (60 * AUTO_REMOVE_GAG);

	// Inform messages
	new gagAction[128];
	if(gagger != gaggedPlayer) {
		format(gagAction, sizeof(gagAction), "* You were gagged by %s. (/gaghelp)", PlayerICName(gagger));
		SendClientMessage(gaggedPlayer, COLOR_LIGHTBLUE, gagAction);

		format(gagAction, sizeof(gagAction), "* You have gagged %s. (/gaghelp)", PlayerICName(gaggedPlayer));
		SendClientMessage(gagger, COLOR_LIGHTBLUE, gagAction);
	}
	else {
		SendClientMessage(gaggedPlayer, COLOR_LIGHTBLUE, "* You have gagged yourself. (/gaghelp)");
	}

	// Action line
	if(gagger != gaggedPlayer) {
		format(gagAction, sizeof(gagAction), "* %s puts a gag in %s's mouth.", PlayerICName(gagger), PlayerICName(gaggedPlayer));
	}
	else {
		format(gagAction, sizeof(gagAction), "* %s puts a gag in their own mouth.", PlayerICName(gagger));
	}
	ProxDetector(30.0, gagger, gagAction, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

	// HUD
	displayCenterHUDInfo(gaggedPlayer,  "~r~Gagged.", 8);

	// Objects
	AttachGagObject(gaggedPlayer);
	return 1;
}

Hook:gag_OnPlayerDeath(playerid, killerid, reason) {
	// Remove gag on death
	if(IsPlayerGagged(playerid)) {
		gagExpire[playerid] = 0;
		RemoveGagObject(playerid);
	}
	return 1;
}

stock AttachGagObject(playerid) {
	switch(GetPlayerSkin(playerid)) {
		case 0:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.113999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.403000, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.029000, -0.038999, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 1:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.113999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.403000, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.029000, -0.038999, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 2:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.113999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.403000, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.029000, -0.038999, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 3:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.403000, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.029000, -0.020999, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 4:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.113999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.403000, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.029000, -0.038999, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 5:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.032999, 0.143999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.403000, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.053000, -0.011999, -0.002999, 0.000000, -89.699981, 0.000000, 0.965998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 6:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.113999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.038998, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 7:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.020998, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 8:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.020998, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 9:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.021000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, -0.004000, -0.020998, 0.000000, 0.000000, -89.699981, 0.000000, 0.683998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 10:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.019999, -0.020998, 0.000000, 0.000000, -89.699981, 0.000000, 0.683998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 11:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.017000, 0.129998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.001999, -0.025999, 0.000000, 0.000000, -89.699981, 0.000000, 0.743998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 12:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.017000, 0.114998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.001999, -0.038998, 0.000000, 0.000000, -89.699981, 0.000000, 0.743998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 13:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.008000, 0.114998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.022999, -0.031999, 0.000000, 0.000000, -89.699981, 0.000000, 0.743998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 14:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.006000, 0.146998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.022999, -0.006998, 0.000000, 0.000000, -89.699981, 0.000000, 0.822997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 15:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.017000, 0.114997, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.001999, -0.038998, 0.000000, 0.000000, -89.699981, 0.000000, 0.743997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 16:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.007999, 0.126997, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.023999, -0.022997, 0.000000, 0.000000, -89.699981, 0.000000, 0.811997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 17:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.005999, 0.113997, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.023999, -0.035997, 0.000000, 0.000000, -89.699981, 0.000000, 0.811997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 18:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.023000, 0.113997, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, -0.005001, -0.037997, 0.000000, 0.000000, -89.699981, 0.000000, 0.811997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 19:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.007000, 0.113997, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.022998, -0.033997, 0.000000, 0.000000, -89.699981, 0.000000, 0.811997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 20:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.007000, 0.113997, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.023999, -0.040997, 0.000000, 0.000000, -89.699981, 0.000000, 0.811997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 21:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.019998, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.802997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 22:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.019998, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.802997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 23:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.019998, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.802997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 24:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.019998, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.802997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 25:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.019998, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.802997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 26:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.019998, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.802997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 27:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.019998, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.802997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 28:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.019998, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.802997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 29:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.019998, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.802997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 30:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.019998, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.802997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 31:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.019998, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.802997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 32:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.095998, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.019998, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.661997, 0.784998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 33:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.028999, 0.108998, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, -0.011001, -0.009997, 0.000000, 0.000000, -89.699981, 0.000000, 0.661997, 0.784998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 34:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.019000, 0.113996, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.030998, -0.033996, 0.000000, 0.000000, -89.699981, 0.000000, 0.811996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 35:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.016000, 0.113996, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.030998, -0.033996, 0.000000, 0.000000, -89.699981, 0.000000, 0.811996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 36:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.016000, 0.113996, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.030998, -0.033996, 0.000000, 0.000000, -89.699981, 0.000000, 0.811996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 37:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.013000, 0.113996, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.030998, -0.033996, 0.000000, 0.000000, -89.699981, 0.000000, 0.811996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 38:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.017000, 0.127997, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.030998, -0.026997, 0.000000, 0.000000, -89.699981, 0.000000, 0.681996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 39:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.017000, 0.127997, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.030998, -0.026997, 0.000000, 0.000000, -89.699981, 0.000000, 0.681996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 40:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.008999, 0.127997, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.007997, -0.026997, 0.000000, 0.000000, -89.699981, 0.000000, 0.681996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 41:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.008999, 0.127997, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.007997, -0.026997, 0.000000, 0.000000, -89.699981, 0.000000, 0.681996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 42:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.000000, 0.127997, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.018997, -0.026997, -0.002000, 0.000000, -89.699981, 0.000000, 0.787996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 43:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.043999, 0.106997, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, -0.027002, -0.046997, -0.002000, 0.000000, -89.699981, 0.000000, 0.787996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 44:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.017000, 0.130996, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.033997, -0.024997, -0.002000, 0.000000, -89.699981, 0.000000, 0.787996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 45:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.015999, 0.118996, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.001997, -0.035997, -0.002000, 0.000000, -89.699981, 0.000000, 0.787996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 46:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.004000, 0.145996, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.021997, -0.009997, -0.002000, 0.000000, -89.699981, 0.000000, 0.787996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 47:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.004000, 0.145996, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.021997, -0.009997, -0.002000, 0.000000, -89.699981, 0.000000, 0.787996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 48:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.004000, 0.127996, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.021997, -0.025997, -0.007000, 0.000000, -89.699981, 0.000000, 0.787996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 49:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.004000, 0.127996, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.021997, -0.025997, -0.007000, 0.000000, -89.699981, 0.000000, 0.787996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 50:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.004000, 0.094996, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.021997, -0.057997, -0.007000, 0.000000, -89.699981, 0.000000, 0.787996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 51:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.004000, 0.118996, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.025997, -0.033997, -0.007000, 0.000000, -89.699981, 0.000000, 0.787996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 52:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.004000, 0.118996, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.025997, -0.033997, -0.007000, 0.000000, -89.699981, 0.000000, 0.787996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 53:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.004000, 0.118996, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.025997, -0.033997, -0.007000, 0.000000, -89.699981, 0.000000, 0.787996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 54:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 55:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.005999, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.012999, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.685997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 56:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.005999, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.012999, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.685997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 57:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.026000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.044999, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.765997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 58:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010999, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.030999, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.765997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 59:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.001000, 0.120999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.016999, -0.037998, 0.000000, 0.000000, -89.699981, 0.000000, 0.765997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 60:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 61:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 62:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 63:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.008000, 0.118999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.009998, -0.033997, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 64:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.008000, 0.118999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.009998, -0.033997, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 65:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.008000, 0.114997, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.022998, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.743997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 66:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.008000, 0.114997, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.022998, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.743997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 67:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.008000, 0.114997, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.022998, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.743997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 68:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.008000, 0.114997, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.022998, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.743997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 69:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.010000, 0.120997, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.006998, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.743997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 70:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.010000, 0.120997, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.006998, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.743997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 71:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.000999, 0.100997, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.019998, -0.050999, 0.000000, 0.000000, -89.699981, 0.000000, 0.743997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 72:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.008000, 0.112998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.026998, -0.035999, 0.000000, 0.000000, -89.699981, 0.000000, 0.743997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 73:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.008000, 0.112998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.026998, -0.035999, 0.000000, 0.000000, -89.699981, 0.000000, 0.743997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 74:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.113999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.403000, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.029000, -0.038999, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 75:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.008000, 0.112998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.026998, -0.035999, 0.000000, 0.000000, -89.699981, 0.000000, 0.743997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 76:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.007999, 0.112998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.009998, -0.035999, 0.000000, 0.000000, -89.699981, 0.000000, 0.743997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 77:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.007999, 0.146998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.011998, -0.011999, 0.000000, 0.000000, -89.699981, 0.000000, 0.743997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 78:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.044000, 0.122998, 0.004002, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.065998, -0.029999, 0.000000, 0.000000, -89.699981, 0.000000, 0.743997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 79:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.011000, 0.122998, 0.004002, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.031998, -0.032999, 0.006999, 0.000000, -89.699981, 0.000000, 0.743997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 80:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.011000, 0.122998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.031998, -0.032999, -0.002000, 0.000000, -89.699981, 0.000000, 0.743997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 81:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.011000, 0.122998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.031998, -0.032999, -0.002000, 0.000000, -89.699981, 0.000000, 0.743997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 82:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 83:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 84:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 85:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 86:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.026000, 0.115999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.045998, -0.032997, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 87:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.012000, 0.138999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.007998, -0.008996, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 88:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.005000, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.013999, -0.023997, 0.000000, 0.000000, -89.699981, 0.000000, 0.689998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 89:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.017999, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.037998, -0.023997, 0.000000, 0.000000, -89.699981, 0.000000, 0.689998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 90:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.014000, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.003998, -0.023997, 0.000000, 0.000000, -89.699981, 0.000000, 0.689998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 91:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.014000, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.003998, -0.023997, 0.000000, 0.000000, -89.699981, 0.000000, 0.689998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 92:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.014000, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.003998, -0.023997, 0.000000, 0.000000, -89.699981, 0.000000, 0.689998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 93:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.014000, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.003998, -0.023997, 0.000000, 0.000000, -89.699981, 0.000000, 0.689998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 94:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.030000, 0.097999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, -0.014001, -0.055997, 0.000000, 0.000000, -89.699981, 0.000000, 0.689998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 95:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.113999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.038998, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 96:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.123998, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.033998, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 97:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.021000, 0.123998, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, -0.001001, -0.033998, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 98:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 99:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 100:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 101:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.020997, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 102:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.006000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.013999, -0.020997, -0.004000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 103:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010999, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027999, -0.020997, -0.004000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 104:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010999, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027999, -0.020997, -0.004000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 105:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010999, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027999, -0.020997, -0.004000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 106:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010999, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027999, -0.020997, -0.004000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 107:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010999, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027999, -0.020997, -0.004000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 108:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010999, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027999, -0.020997, -0.004000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 109:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010999, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027999, -0.020997, -0.004000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 110:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010999, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027999, -0.020997, -0.004000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 111:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.019000, 0.113999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.036999, -0.039997, -0.004000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 112:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.019000, 0.113999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.036999, -0.039997, -0.004000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 113:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010998, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027999, -0.020997, -0.004000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 114:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010998, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027999, -0.020997, -0.004000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 115:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010998, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027999, -0.020997, -0.004000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 116:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010998, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027999, -0.020997, -0.004000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 117:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.006001, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.013999, -0.020997, -0.004000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 118:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.006001, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.013999, -0.020997, -0.004000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 119:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.006001, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.013999, -0.020997, -0.004000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 120:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.006001, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.013999, -0.020997, -0.004000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 121:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002998, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.023999, -0.020997, 0.001000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 122:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002998, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.023999, -0.020997, 0.001000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 123:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002998, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.023999, -0.020997, 0.001000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 124:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.022998, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.043998, -0.020997, 0.001000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 125:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.022998, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.039999, -0.020997, 0.001000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 126:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.008998, 0.134999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.025999, -0.038997, 0.001000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 127:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.008998, 0.134999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.025999, -0.024997, 0.001000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 128:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.008998, 0.134999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.025999, -0.024997, 0.001000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 129:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.008998, 0.134999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.025999, -0.024997, 0.001000, 0.000000, -89.699981, 0.000000, 0.649997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 130:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.011001, 0.150998, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.006998, -0.001997, 0.001000, 0.000000, -89.699981, 0.000000, 0.703997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 131:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.011001, 0.150998, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.006998, -0.001997, 0.001000, 0.000000, -89.699981, 0.000000, 0.703997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 132:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.045999, 0.113999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, -0.026000, -0.046998, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 133:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.019999, 0.122998, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.039999, -0.029998, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 134:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.019999, 0.122998, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.039999, -0.029998, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 135:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.011000, 0.122998, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.006999, -0.029998, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 136:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.011999, 0.122998, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027999, -0.038998, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 137:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.011999, 0.122998, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027999, -0.038998, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 138:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.009000, 0.122998, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.008999, -0.034998, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 139:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.009000, 0.122998, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.008999, -0.034998, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 140:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.009000, 0.129998, 0.000001, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.008999, -0.019998, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 141:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.020000, 0.114999, 0.000001, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, -0.005000, -0.043998, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 142:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010999, 0.127999, -0.006998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.022998, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 143:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010999, 0.127999, -0.006998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.022998, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 144:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010999, 0.141999, -0.006998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.010998, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 145:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010999, 0.121999, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.028997, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 146:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010999, 0.121999, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.028997, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 147:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010999, 0.121999, 0.004001, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028999, -0.028997, 0.008000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 148:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.010000, 0.121999, 0.004001, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.007999, -0.028997, 0.000999, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 149:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.019999, 0.121999, 0.004001, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.037999, -0.028997, 0.000999, 0.000000, -89.699981, 0.000000, 0.869998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 150:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.014000, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.003998, -0.023996, 0.000000, 0.000000, -89.699981, 0.000000, 0.725997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 151:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.009999, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.008998, -0.023996, 0.000000, 0.000000, -89.699981, 0.000000, 0.725997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 152:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.009999, 0.102999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.008998, -0.049996, 0.000000, 0.000000, -89.699981, 0.000000, 0.725997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 153:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.014000, 0.111999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.032997, -0.036996, 0.000000, 0.000000, -89.699981, 0.000000, 0.725997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 154:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.014000, 0.111999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.000997, -0.036996, 0.000000, 0.000000, -89.699981, 0.000000, 0.725997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 155:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.008999, 0.129999, -0.006998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.025997, -0.020997, -0.004000, 0.000000, -89.699981, 0.000000, 0.774997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 156:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.007999, 0.129999, 0.005001, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.025997, -0.020997, 0.001000, 0.000000, -89.699981, 0.000000, 0.774997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 157:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.006000, 0.129999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.013997, -0.020997, 0.001000, 0.000000, -89.699981, 0.000000, 0.774997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 158:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.015000, 0.129999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.034997, -0.020997, 0.001000, 0.000000, -89.699981, 0.000000, 0.774997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 159:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002999, 0.129999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.022997, -0.020997, 0.001000, 0.000000, -89.699981, 0.000000, 0.774997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 160:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.015000, 0.104999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.033997, -0.044997, 0.001000, 0.000000, -89.699981, 0.000000, 0.774997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 161:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.015000, 0.119999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.033997, -0.029997, 0.001000, 0.000000, -89.699981, 0.000000, 0.774997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 162:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.015000, 0.119999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.033997, -0.029997, 0.001000, 0.000000, -89.699981, 0.000000, 0.774997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 163:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.015000, 0.119999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.033997, -0.029997, 0.001000, 0.000000, -89.699981, 0.000000, 0.774997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 164:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.015000, 0.119999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.033997, -0.029997, 0.001000, 0.000000, -89.699981, 0.000000, 0.774997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 165:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.007000, 0.124999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.022997, -0.029997, 0.001000, 0.000000, -89.699981, 0.000000, 0.774997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 166:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.004999, 0.124999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.010997, -0.029997, 0.001000, 0.000000, -89.699981, 0.000000, 0.774997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 167:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.008000, 0.124999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.023997, -0.029997, 0.001000, 0.000000, -89.699981, 0.000000, 0.774997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 168:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.008000, 0.124999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.023997, -0.029997, 0.001000, 0.000000, -89.699981, 0.000000, 0.774997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 169:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.004000, 0.112999, 0.005001, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.011997, -0.046996, 0.001000, 0.000000, -89.699981, 0.000000, 0.774997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 170:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.003999, 0.133999, -0.009998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.018997, -0.028997, -0.004999, 0.000000, -89.699981, 0.000000, 0.821997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 171:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.003999, 0.133999, -0.009998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.018997, -0.028997, -0.004999, 0.000000, -89.699981, 0.000000, 0.821997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 172:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.010000, 0.133999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.004997, -0.028997, -0.001999, 0.000000, -89.699981, 0.000000, 0.740997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 173:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002999, 0.133999, -0.008998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.020997, -0.022997, -0.007000, 0.000000, -89.699981, 0.000000, 0.772996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 174:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002999, 0.133999, -0.008998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.020997, -0.022997, -0.007000, 0.000000, -89.699981, 0.000000, 0.772996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 175:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002999, 0.133999, -0.008998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.020997, -0.022997, -0.007000, 0.000000, -89.699981, 0.000000, 0.772996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 176:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002999, 0.133999, -0.008998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.020997, -0.022997, -0.007000, 0.000000, -89.699981, 0.000000, 0.772996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 177:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002999, 0.133999, -0.008998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.020997, -0.022997, -0.007000, 0.000000, -89.699981, 0.000000, 0.772996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 178:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.006000, 0.133999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.014997, -0.022997, 0.000000, 0.000000, -89.699981, 0.000000, 0.772996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 179:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.003000, 0.133999, -0.007998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.014997, -0.019997, -0.008000, 0.000000, -89.699981, 0.000000, 0.772996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 180:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.003000, 0.133999, -0.007998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.014997, -0.019997, -0.008000, 0.000000, -89.699981, 0.000000, 0.772996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 181:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.037999, 0.129999, -0.007998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.057997, -0.026997, -0.002999, 0.000000, -89.699981, 0.000000, 0.772996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 182:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.007000, 0.113999, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.013997, -0.038997, -0.002999, 0.000000, -89.699981, 0.000000, 0.772996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 183:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010999, 0.113999, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.029997, -0.038997, -0.002999, 0.000000, -89.699981, 0.000000, 0.772996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 184:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010999, 0.137999, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.029997, -0.019997, -0.002999, 0.000000, -89.699981, 0.000000, 0.772996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 185:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010999, 0.137999, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.029997, -0.019997, -0.002999, 0.000000, -89.699981, 0.000000, 0.772996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 186:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010999, 0.137999, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.029997, -0.019997, -0.002999, 0.000000, -89.699981, 0.000000, 0.772996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 187:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.003999, 0.135999, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.023997, -0.022997, 0.006000, 0.000000, -89.699981, 0.000000, 0.772996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 188:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.009999, 0.135999, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028997, -0.022997, 0.006000, 0.000000, -89.699981, 0.000000, 0.772996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 189:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.009999, 0.135999, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028997, -0.022997, 0.006000, 0.000000, -89.699981, 0.000000, 0.772996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 190:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.009999, 0.135999, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028997, -0.022997, 0.000999, 0.000000, -89.699981, 0.000000, 0.691996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 191:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.009999, 0.135999, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028997, -0.022997, 0.000999, 0.000000, -89.699981, 0.000000, 0.691996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 192:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.009999, 0.135999, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028997, -0.022997, 0.000999, 0.000000, -89.699981, 0.000000, 0.691996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 193:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.009999, 0.135999, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028997, -0.022997, 0.000999, 0.000000, -89.699981, 0.000000, 0.691996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 194:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012999, 0.111999, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028997, -0.036997, 0.000999, 0.000000, -89.699981, 0.000000, 0.691996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 195:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012999, 0.127998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028997, -0.023997, 0.000999, 0.000000, -89.699981, 0.000000, 0.726996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 196:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012999, 0.106998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028997, -0.023997, 0.000999, 0.000000, -89.699981, 0.000000, 0.620996, 0.850998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 197:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012999, 0.106998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.033997, -0.019997, 0.000999, 0.000000, -89.699981, 0.000000, 0.722996, 0.850998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 198:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.002000, 0.122998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.014997, -0.006997, 0.000999, 0.000000, -89.699981, 0.000000, 0.722996, 0.850998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 199:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.006999, 0.122998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027997, -0.006997, 0.000999, 0.000000, -89.699981, 0.000000, 0.722996, 0.850998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 200:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.006999, 0.122998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027997, -0.006997, 0.000999, 0.000000, -89.699981, 0.000000, 0.722996, 0.850998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 201:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.008000, 0.122998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.009997, -0.007997, 0.000999, 0.000000, -89.699981, 0.000000, 0.670996, 0.850998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 202:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.113999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.403000, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.029000, -0.038999, 0.000000, 0.000000, -89.699981, 0.000000, 0.728999, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 203:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.014999, 0.137998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.035997, 0.009002, -0.003000, 0.000000, -89.699981, 0.000000, 0.780996, 0.850998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 204:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.014999, 0.137998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.035997, 0.009002, -0.003000, 0.000000, -89.699981, 0.000000, 0.780996, 0.850998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 205:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.014999, 0.137998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.035997, 0.009002, -0.003000, 0.000000, -89.699981, 0.000000, 0.780996, 0.850998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 206:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.014999, 0.137998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.035997, 0.009002, -0.003000, 0.000000, -89.699981, 0.000000, 0.780996, 0.850998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 207:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.010000, 0.114998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.005997, -0.017997, -0.003000, 0.000000, -89.699981, 0.000000, 0.780996, 0.850998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 208:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.000000, 0.120998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.014997, -0.008997, -0.003000, 0.000000, -89.699981, 0.000000, 0.780996, 0.850998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 209:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.011999, 0.094998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.030997, -0.027997, -0.003000, 0.000000, -89.699981, 0.000000, 0.694996, 0.850998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 210:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.011999, 0.094998, -0.002998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.030997, -0.027997, -0.003000, 0.000000, -89.699981, 0.000000, 0.694996, 0.850998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 211:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.014000, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.003998, -0.030996, 0.000000, 0.000000, -89.699981, 0.000000, 0.718997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 212:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.024000, 0.112998, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.041997, -0.042997, 0.000000, 0.000000, -89.699981, 0.000000, 0.661997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 213:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.024000, 0.130998, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.041997, -0.019997, 0.000000, 0.000000, -89.699981, 0.000000, 0.772997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 214:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.015000, 0.119998, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.002998, -0.036997, 0.000000, 0.000000, -89.699981, 0.000000, 0.772997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 215:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.015000, 0.119998, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.002998, -0.036997, 0.000000, 0.000000, -89.699981, 0.000000, 0.772997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 216:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.015000, 0.119998, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.002998, -0.036997, 0.000000, 0.000000, -89.699981, 0.000000, 0.772997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 217:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.011000, 0.121998, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028998, -0.027997, 0.000000, 0.000000, -89.699981, 0.000000, 0.772997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 218:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.011000, 0.121998, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028998, -0.027997, 0.000000, 0.000000, -89.699981, 0.000000, 0.772997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 219:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.014000, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.003998, -0.023996, 0.000000, 0.000000, -89.699981, 0.000000, 0.689997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 220:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.031999, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, -0.017000, -0.017997, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 221:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.004000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.023999, -0.017997, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 222:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.004000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.023999, -0.017997, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 223:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.004000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.023999, -0.017997, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 224:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.014000, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.003998, -0.023996, 0.000000, 0.000000, -89.699981, 0.000000, 0.689997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 225:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.014000, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.003998, -0.023996, 0.000000, 0.000000, -89.699981, 0.000000, 0.689997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 226:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.014000, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.003998, -0.023996, 0.000000, 0.000000, -89.699981, 0.000000, 0.689997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 227:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.023000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.040998, -0.017997, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 228:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.017000, 0.134999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.035999, -0.009996, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 229:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.013000, 0.112999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.029999, -0.037996, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 230:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.023000, 0.112999, -0.004999, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, -0.004000, -0.042996, 0.000000, 0.000000, -89.699981, 0.000000, 0.821998, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 231:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.016000, 0.130999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.032998, -0.017996, 0.000000, 0.000000, -89.699981, 0.000000, 0.689997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 232:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.016000, 0.130999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.032998, -0.017996, 0.000000, 0.000000, -89.699981, 0.000000, 0.689997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 233:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.014000, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.003998, -0.023996, 0.000000, 0.000000, -89.699981, 0.000000, 0.689997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 234:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.106999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.023998, -0.023996, 0.000000, 0.000000, -89.699981, 0.000000, 0.689997, 0.830999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 235:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.016999, 0.106999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.001998, -0.023996, 0.000000, 0.000000, -89.699981, 0.000000, 0.689997, 0.830999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 236:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.008000, 0.116999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028998, -0.004996, 0.000000, 0.000000, -89.699981, 0.000000, 0.689997, 0.830999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 237:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002999, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.022998, -0.023996, 0.000000, 0.000000, -89.699981, 0.000000, 0.689997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 238:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002999, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.022998, -0.023996, 0.000000, 0.000000, -89.699981, 0.000000, 0.689997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 239:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.041000, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.061997, -0.023996, 0.000000, 0.000000, -89.699981, 0.000000, 0.689997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 240:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.009999, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.008998, -0.023996, 0.000000, 0.000000, -89.699981, 0.000000, 0.721997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 241:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.009999, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.008998, -0.023996, 0.000000, 0.000000, -89.699981, 0.000000, 0.721997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 242:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.009999, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.008998, -0.023996, 0.000000, 0.000000, -89.699981, 0.000000, 0.721997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 243:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.009999, 0.125999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.008998, -0.023996, 0.000000, 0.000000, -89.699981, 0.000000, 0.721997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 244:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.026999, 0.132999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, -0.009002, -0.015996, 0.000000, 0.000000, -89.699981, 0.000000, 0.721997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 245:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.007999, 0.132999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.012997, -0.024997, 0.000000, 0.000000, -89.699981, 0.000000, 0.721997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 246:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.007999, 0.132999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.012997, -0.024997, 0.000000, 0.000000, -89.699981, 0.000000, 0.721997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 247:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.018000, 0.120999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.036997, -0.030997, -0.003000, 0.000000, -89.699981, 0.000000, 0.836997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 248:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.003000, 0.120999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.017998, -0.030997, -0.003000, 0.000000, -89.699981, 0.000000, 0.836997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 249:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010000, 0.130999, 0.003002, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.029998, -0.018996, 0.003000, 0.000000, -89.699981, 0.000000, 0.836997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 250:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010000, 0.130999, -0.001997, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.029998, -0.018996, 0.003000, 0.000000, -89.699981, 0.000000, 0.836997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 251:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.006999, 0.130999, -0.001997, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.009998, -0.018996, 0.003000, 0.000000, -89.699981, 0.000000, 0.836997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 252:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.011000, 0.109999, 0.001002, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027998, -0.038996, 0.003000, 0.000000, -89.699981, 0.000000, 0.836997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 253:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.011000, 0.109999, 0.001002, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027998, -0.038996, 0.003000, 0.000000, -89.699981, 0.000000, 0.688997, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 254:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.011000, 0.111999, 0.000002, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027998, -0.038996, 0.003000, 0.000000, -89.699981, 0.000000, 0.879997, 1.027999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 255:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.011000, 0.111999, 0.000002, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027998, -0.038996, 0.003000, 0.000000, -89.699981, 0.000000, 0.879997, 1.027999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 256:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.011999, 0.122999, 0.000002, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.007998, -0.032997, 0.003000, 0.000000, -89.699981, 0.000000, 0.879997, 1.027999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 257:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.011999, 0.122999, 0.000002, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.012998, -0.032997, 0.003000, 0.000000, -89.699981, 0.000000, 0.715997, 1.027999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 258:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.011000, 0.135999, 0.000002, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028998, -0.018997, 0.003000, 0.000000, -89.699981, 0.000000, 0.838997, 1.027999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 259:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.011000, 0.135999, 0.000002, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028998, -0.018997, 0.003000, 0.000000, -89.699981, 0.000000, 0.838997, 1.027999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 260:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.011000, 0.135999, 0.000002, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028998, -0.018997, 0.003000, 0.000000, -89.699981, 0.000000, 0.838997, 1.027999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 261:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.011000, 0.102998, 0.000002, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028998, -0.018997, 0.003000, 0.000000, -89.699981, 0.000000, 0.600997, 0.823999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 262:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.002999, 0.117998, 0.000002, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.015998, -0.002996, 0.000000, 0.000000, -89.699981, 0.000000, 0.763996, 0.823999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 263:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.007999, 0.117998, 0.000002, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.008998, -0.013997, 0.000000, 0.000000, -89.699981, 0.000000, 0.763996, 0.823999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 264:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.007999, 0.146999, 0.000002, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.008998, 0.012002, 0.000000, 0.000000, -89.699981, 0.000000, 0.763996, 0.823999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 265:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.009000, 0.103998, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.027998, -0.014997, 0.000000, 0.000000, -89.699981, 0.000000, 0.763996, 0.823999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 266:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.019000, 0.100998, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.035998, -0.023997, 0.000000, 0.000000, -89.699981, 0.000000, 0.888996, 0.823999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 267:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.019000, 0.100998, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.035998, -0.023997, 0.000000, 0.000000, -89.699981, 0.000000, 0.888996, 0.823999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 268:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.012000, 0.116999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.035998, -0.012997, 0.000000, 0.000000, -89.699981, 0.000000, 0.773996, 0.823999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 269:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.018000, 0.113999, -0.003998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.035998, -0.014997, 0.000000, 0.000000, -89.699981, 0.000000, 0.861996, 0.823999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 270:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.018000, 0.113999, 0.000001, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.035998, -0.014997, 0.000000, 0.000000, -89.699981, 0.000000, 0.747996, 0.823999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 271:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.033000, 0.113999, 0.000001, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.049998, -0.004997, 0.000000, 0.000000, -89.699981, 0.000000, 0.747996, 0.823999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 272:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.020000, 0.122999, 0.000001, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.040997, 0.000002, 0.000000, 0.000000, -89.699981, 0.000000, 0.821996, 0.823999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 273:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.002999, 0.122999, 0.000001, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.017998, 0.000002, 0.000000, 0.000000, -89.699981, 0.000000, 0.775996, 0.823999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 274:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.018000, 0.101999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.035997, -0.022996, 0.000000, 0.000000, -89.699981, 0.000000, 0.775996, 0.823999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 275:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.018000, 0.101999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.035997, -0.022996, 0.000000, 0.000000, -89.699981, 0.000000, 0.775996, 0.823999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 276:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.018000, 0.101999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.035997, -0.022996, 0.000000, 0.000000, -89.699981, 0.000000, 0.775996, 0.823999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 277:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.134999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.020998, 0.017003, 0.000000, 0.000000, -89.699981, 0.000000, 0.775996, 0.823999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 278:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.134999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.020998, 0.017003, 0.000000, 0.000000, -89.699981, 0.000000, 0.775996, 0.823999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 279:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.134999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.020998, 0.017003, 0.000000, 0.000000, -89.699981, 0.000000, 0.775996, 0.823999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 280:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.102999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.020998, -0.026997, 0.000000, 0.000000, -89.699981, 0.000000, 0.750996, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 281:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.102999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.020998, -0.026997, 0.000000, 0.000000, -89.699981, 0.000000, 0.750996, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 282:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.102999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.020998, -0.026997, 0.000000, 0.000000, -89.699981, 0.000000, 0.750996, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 283:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.102999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.020998, -0.026997, 0.000000, 0.000000, -89.699981, 0.000000, 0.750996, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 284:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.010000, 0.134999, 0.002002, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.028998, 0.005002, 0.005000, 0.000000, -89.699981, 0.000000, 0.750996, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 285:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.023000, 0.134999, 0.002002, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.037998, 0.003002, 0.005000, 0.000000, -89.699981, 0.000000, 0.750996, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 286:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.016000, 0.105999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.037998, -0.024996, 0.000000, 0.000000, -89.699981, 0.000000, 0.750996, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 287:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.004999, 0.119999, 0.005002, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.011998, -0.007996, 0.007000, 0.000000, -89.699981, 0.000000, 0.750996, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 288:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.102999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.020997, -0.026996, 0.000000, 0.000000, -89.699981, 0.000000, 0.750995, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 289:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.013000, 0.104998, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.030998, -0.018996, 0.002000, 0.000000, -89.699981, 0.000000, 0.750995, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 290:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.013000, 0.121999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.030998, -0.002996, 0.002000, 0.000000, -89.699981, 0.000000, 0.750995, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 291:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.013000, 0.121999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.030998, -0.002996, 0.002000, 0.000000, -89.699981, 0.000000, 0.750995, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 292:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.013000, 0.121999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.030998, -0.002996, 0.002000, 0.000000, -89.699981, 0.000000, 0.750995, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 293:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.013000, 0.121999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.030998, -0.002996, -0.002999, 0.000000, -89.699981, 0.000000, 0.750995, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 294:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.013000, 0.121999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.030998, -0.002996, -0.002999, 0.000000, -89.699981, 0.000000, 0.750995, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 295:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.007000, 0.121999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.011998, -0.011997, 0.002000, 0.000000, -89.699981, 0.000000, 0.750995, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 296:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.019000, 0.134998, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.036998, 0.008003, 0.001000, 0.000000, -89.699981, 0.000000, 0.805995, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 297:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.019000, 0.134998, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.036998, 0.008003, 0.001000, 0.000000, -89.699981, 0.000000, 0.805995, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 298:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.019000, 0.134998, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.036998, 0.008003, 0.001000, 0.000000, -89.699981, 0.000000, 0.805995, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 299:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.019000, 0.134998, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.036998, 0.008003, 0.001000, 0.000000, -89.699981, 0.000000, 0.805995, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 300:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.102999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.020997, -0.026996, 0.000000, 0.000000, -89.699981, 0.000000, 0.750995, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 301:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.102999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.020997, -0.026996, 0.000000, 0.000000, -89.699981, 0.000000, 0.750995, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 302:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.102999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.020997, -0.026996, 0.000000, 0.000000, -89.699981, 0.000000, 0.750995, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 303:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.102999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.020997, -0.026996, 0.000000, 0.000000, -89.699981, 0.000000, 0.750995, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 304:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.102999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.020997, -0.026996, 0.000000, 0.000000, -89.699981, 0.000000, 0.750995, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 305:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.002000, 0.102999, -0.000998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.020997, -0.026996, 0.000000, 0.000000, -89.699981, 0.000000, 0.750995, 0.858998, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 306:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.014000, 0.119999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.003998, -0.032996, 0.000000, 0.000000, -89.699981, 0.000000, 0.725996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 307:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.014000, 0.119999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.003998, -0.032996, 0.000000, 0.000000, -89.699981, 0.000000, 0.725996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 308:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.014000, 0.119999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.003998, -0.032996, 0.000000, 0.000000, -89.699981, 0.000000, 0.725996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 309:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, -0.014000, 0.119999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.003998, -0.032996, 0.000000, 0.000000, -89.699981, 0.000000, 0.725996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 310:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.005000, 0.096999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.022998, -0.050996, 0.000000, 0.000000, -89.699981, 0.000000, 0.725996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
		case 311:
		{
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX1, 1974, 2, 0.005000, 0.096999, -0.001998, 0.000000, 0.000000, -51.399990, 0.453000, 0.391000, 0.402999, 0xFF7F0000, 0x0);
			SetPlayerAttachedObject(playerid, GAG_OBJ_INDEX2, 19422, 2, 0.022998, -0.050996, 0.000000, 0.000000, -89.699981, 0.000000, 0.725996, 1.004999, 1.550999, 0xFF000000, 0x0);
			return 1;
		}
	}
	return 1;
}

stock RemoveGagObject(playerid) {
	RemovePlayerAttachedObject(playerid, GAG_OBJ_INDEX1);
	RemovePlayerAttachedObject(playerid, GAG_OBJ_INDEX2);
	return 1;
}