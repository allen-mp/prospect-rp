/*
				Blindfolds by Jay Cortez
*/

#define BFOLD_OBJ_INDEX		5

CMD:blindfoldhelp(playerid, params[])
{
	SendClientMessage(playerid, COLOR_YELLOW, "____________________________________________");
	SendClientMessage(playerid, COLOR_WHITE, "COMMANDS: /blindfold, /unblindfold");
	return 1;
}

CMD:blindfold(playerid, params[])
{
	if(!PlayerInfo[playerid][pBlindfolds])
		return SendClientMessage(playerid, COLOR_GREY, "You don't have any blindfolds.");

	new targetPlayer;
	if(sscanf(params, "u", targetPlayer))
		return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /blindfold [playerid/partOfName]");
	
	if(!IsPlayerConnected(targetPlayer))
		return SendClientMessage(playerid, COLOR_GREY, "Unknown player.");

	if(	GetDistanceBetweenPlayers(playerid, targetPlayer) > 4 ||
		GetPlayerState(targetPlayer) == PLAYER_STATE_SPECTATING ||
		PlayerInfo[targetPlayer][pMask]
	)
		return SendClientMessage(playerid, COLOR_GREY, "You are not close enough to this person.");	

	if(IsPlayerBlindfolded(targetPlayer))
		return SendClientMessage(playerid, COLOR_GREY, "This person is already blindfolded.");	

	if(	PlayerTied[targetPlayer] ||
		PlayerCuffed[targetPlayer] ||
		PlayerTazed[targetPlayer] ||
		(targetPlayer == playerid)
		) {
		ApplyBlindfold(playerid, targetPlayer);
	}
	else {
		new blindfoldAction[128];
		format(blindfoldAction, sizeof(blindfoldAction), "* %s attempts to tie a blindfold around %s's head, but fails.", PlayerICName(playerid), PlayerICName(targetPlayer));
		ProxDetector(30.0, playerid, blindfoldAction, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	}
	return 1;
}

CMD:unblindfold(playerid, params[])
{
	new targetPlayer;
	if(sscanf(params, "u", targetPlayer))
		return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /unblindfold [playerid/partOfName]");
	
	if(!IsPlayerConnected(targetPlayer))
		return SendClientMessage(playerid, COLOR_GREY, "Unknown player.");

	if(	GetDistanceBetweenPlayers(playerid, targetPlayer) > 4 ||
		GetPlayerState(targetPlayer) == PLAYER_STATE_SPECTATING ||
		PlayerInfo[targetPlayer][pMask]
	)
		return SendClientMessage(playerid, COLOR_GREY, "You are not close enough to this person.");	

	if(	PlayerTied[playerid] ||
		PlayerCuffed[playerid] ||
		PlayerTazed[playerid]
	)
		return SendClientMessage(playerid, COLOR_GREY, "You cannot do this while restrained.");	

	if(!IsPlayerBlindfolded(targetPlayer))
		return SendClientMessage(playerid, COLOR_GREY, "This person is not blindfolded.");

	// Remove it
	RemoveBlindfold(targetPlayer);

	// Action
	new blindfoldAction[128];

	if(targetPlayer == playerid) {
		format(blindfoldAction, sizeof(blindfoldAction), "* %s rips the blindfold off of their head.", PlayerICName(playerid));
	}
	else {
		format(blindfoldAction, sizeof(blindfoldAction), "* %s rips the blindfold off of %s's head.", PlayerICName(playerid), PlayerICName(targetPlayer));
	}
	
	ProxDetector(30.0, playerid, blindfoldAction, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	return 1;
}

stock IsPlayerBlindfolded(playerid) {
	return (PlayerBlinded[playerid] == 1);
}

stock ApplyBlindfold(blinder, blindedPlayer) {
	PlayerInfo[blinder][pBlindfolds]--;

	// Blindfold effect
	PlayerBlinded[blindedPlayer] = 1;
	GangZoneShowForPlayer(blindedPlayer, blackblindfoldzone, 255);
	SetPlayerCameraPos(blindedPlayer, -833.5241, -1358.8575, 86.9054);
	SetPlayerCameraLookAt(blindedPlayer, -833.5241, -1358.8575, 0);

	// Animation
	if(!IsPlayerInAnyVehicle(blinder) && blinder != blindedPlayer) {
		SetPlayerFacingPlayer(blinder, blindedPlayer);
		OnePlayAnim(blinder, "BD_FIRE", "wash_up", 4.0, 0, 0, 0, 0, 0);
	}

	// Inform messages
	new blindfoldAction[128];
	if(blinder != blindedPlayer) {
		format(blindfoldAction, sizeof(blindfoldAction), "* You were blindfolded by %s. (/blindfoldhelp)", PlayerICName(blinder));
		SendClientMessage(blindedPlayer, COLOR_LIGHTBLUE, blindfoldAction);

		format(blindfoldAction, sizeof(blindfoldAction), "* You have blindfolded %s. (/blindfoldhelp)", PlayerICName(blindedPlayer));
		SendClientMessage(blinder, COLOR_LIGHTBLUE, blindfoldAction);
	}
	else {
		SendClientMessage(blindedPlayer, COLOR_LIGHTBLUE, "* You have blindfolded yourself. (/blindfoldhelp)");
	}

	// Action line
	if(blinder != blindedPlayer) {
		format(blindfoldAction, sizeof(blindfoldAction), "* %s ties a blindfold around %s's head.", PlayerICName(blinder), PlayerICName(blindedPlayer));
	}
	else {
		format(blindfoldAction, sizeof(blindfoldAction), "* %s ties a blindfold around their own head.", PlayerICName(blinder));
	}
	ProxDetector(30.0, blinder, blindfoldAction, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

	// HUD
	displayCenterHUDInfo(blindedPlayer,  "~r~Blindfolded.", 8);

	// Objects
	AttachBlindfoldObject(blindedPlayer);
	return 1;
}

stock RemoveBlindfold(playerid) {
	GangZoneHideForPlayer(playerid, blackblindfoldzone);
	SetCameraBehindPlayer(playerid);
	PlayerBlinded[playerid] = 0;
	RemovePlayerAttachedObject(playerid, BFOLD_OBJ_INDEX);
	return 1;
}

Hook:bfold_OnPlayerDeath(playerid, killerid, reason) {
	// Remove blindfold on death
	if(IsPlayerBlindfolded(playerid)) {
		RemoveBlindfold(playerid);
	}
	return 1;
}

stock AttachBlindfoldObject(playerid) {
	switch(GetPlayerSkin(playerid)) {
		case 0:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.107997, -0.038998, 0.000000, 0.000000, -89.699981, 0.000000, 0.831999, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 1:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.107997, -0.038998, 0.000000, 0.000000, -89.699981, 0.000000, 0.831999, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 2:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.090998, -0.038998, 0.000000, 0.000000, -89.699981, 0.000000, 0.831999, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 3:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.099998, -0.024998, 0.000000, 0.000000, -89.699981, 0.000000, 0.831999, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 4:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.145998, -0.040998, 0.000000, 0.000000, -89.699981, 0.000000, 0.831999, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 5:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.121997, -0.035998, 0.000000, 0.000000, -89.699981, 0.000000, 0.831999, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 6:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.105997, -0.047998, 0.000000, 0.000000, -89.699981, 0.000000, 0.831999, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 7:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.101997, -0.010998, 0.000000, 0.000000, -89.699981, 0.000000, 0.831999, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 8:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.086997, -0.028998, 0.000000, 0.000000, -89.699981, 0.000000, 0.831999, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 9:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.070997, -0.033998, 0.000000, 0.000000, -89.699981, 0.000000, 0.831999, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 10:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.070997, -0.033998, 0.000000, 0.000000, -89.699981, 0.000000, 0.831999, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 11:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.070997, -0.033998, 0.000000, 0.000000, -89.699981, 0.000000, 0.831999, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 12:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.070997, -0.033998, 0.000000, 0.000000, -89.699981, 0.000000, 0.831999, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 13:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.085997, -0.042998, 0.000000, 0.000000, -89.699981, 0.000000, 0.831999, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 14:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.098997, -0.009998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 15:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.079997, -0.055998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 16:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.101997, -0.022998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 17:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.101997, -0.038998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 18:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.068997, -0.038998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 19:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.090997, -0.037998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 20:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.101998, -0.042998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 21:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.095998, -0.024998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 22:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.095998, -0.024998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 23:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.095998, -0.024998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 24:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.095998, -0.024998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 25:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.095998, -0.024998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 26:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.095998, -0.024998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 27:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.095998, -0.024998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 28:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.095998, -0.024998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 29:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.095998, -0.024998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 30:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.090998, -0.004998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 31:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.090998, -0.004998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 32:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.070998, -0.078998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 33:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.041997, -0.049998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 34:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.110997, -0.035998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 35:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.035998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 36:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.035998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 37:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.035998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 38:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.035998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 39:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.073997, -0.035998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 40:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.073997, -0.035998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 41:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.073997, -0.035998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 42:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.092997, -0.034998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 43:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.039997, -0.040998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 44:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.094997, -0.040998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 45:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.063997, -0.040998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 46:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.093997, -0.016998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 47:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.093997, -0.016998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 48:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.093997, -0.016998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 49:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.093997, -0.016998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 50:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.100997, -0.058998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 51:
		{
		   SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.100997, -0.038998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 52:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.100997, -0.038998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 53:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.084997, -0.043998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 54:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.084997, -0.024998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 55:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.065997, -0.024998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 56:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.075997, -0.039998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 57:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.119997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 58:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.091997, -0.053998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 59:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.091997, -0.035998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 60:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.091997, -0.035998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 61:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.091997, -0.035998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 62:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.091997, -0.035998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 63:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.076997, -0.035998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 64:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.076997, -0.035998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 65:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.084997, -0.037998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 66:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.099997, -0.041998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 67:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.099997, -0.041998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 68:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.083997, -0.032998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 69:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.083997, -0.032998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 70:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.083997, -0.032998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 71:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.083997, -0.032998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 72:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.083997, -0.032998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 73:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.083997, -0.032998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 74:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.083997, -0.032998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 75:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.083997, -0.032998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 76:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.070997, -0.032998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 77:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.070997, -0.032998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 78:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.114997, -0.046998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 79:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.090997, -0.045998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 80:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.090997, -0.045998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 81:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.090997, -0.045998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 82:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.094997, -0.019998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 83:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.094997, -0.019998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 84:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.094997, -0.019998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 85:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.094997, -0.019998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 86:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.101997, -0.042998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 87:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.076997, -0.015998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 88:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.076997, -0.015998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 89:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.100997, -0.043998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 90:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.074997, -0.027998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 91:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.074997, -0.027998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 92:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.074997, -0.027998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 93:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.074997, -0.027998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 94:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.074997, -0.027998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 95:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.092997, -0.067998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 96:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.110997, -0.047998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 97:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.071997, -0.047998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 98:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.091997, -0.017998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 99:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.091997, -0.017998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 100:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.008998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 101:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.046998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 102:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 103:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 104:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 105:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 106:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 107:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 108:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 109:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 110:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 111:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 112:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 113:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 114:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 115:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 116:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 117:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 118:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 119:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 120:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 121:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 122:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 123:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 124:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 125:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 126:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 127:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 128:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 129:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 130:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.051997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 131:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.076997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 132:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.038997, -0.061998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 133:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.106997, -0.035998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 134:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.106997, -0.035998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 135:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.074997, -0.035998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 136:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.084997, -0.064998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 137:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.084997, -0.064998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 138:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.069997, -0.032998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 139:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.069997, -0.032998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 140:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.075997, -0.020998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.550999, 0xFF000000, 0x0);
		}
		case 141:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.064997, -0.017998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 142:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.109997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 143:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.088997, -0.016998, -0.007000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 144:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.088997, -0.016998, -0.007000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 145:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.088997, -0.016998, -0.007000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 146:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.088997, -0.016998, -0.007000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 147:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.088997, -0.016998, -0.007000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 148:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.088997, -0.030998, 0.005000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 149:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.107997, -0.030998, 0.005000, 0.000000, -89.699981, 0.000000, 0.967998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 150:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.074997, -0.040998, 0.005000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 151:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.083997, -0.025998, 0.005000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 152:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.083997, -0.025998, 0.005000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 153:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.083997, -0.025998, 0.005000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 154:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.083997, -0.025998, 0.005000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 155:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.102997, -0.025998, 0.005000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 156:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.102997, -0.025998, 0.005000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 157:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.073997, -0.025998, 0.005000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 158:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.025998, 0.005000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 159:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.025998, 0.005000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 160:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.025998, 0.005000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 161:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.025998, 0.005000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 162:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.025998, 0.005000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 163:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.025998, 0.005000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 164:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.025998, 0.005000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 165:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.025998, 0.005000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 166:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.025998, 0.005000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 167:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.025998, 0.005000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 168:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.025998, 0.005000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 169:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.083997, -0.045998, 0.005000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 170:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.094997, -0.032998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 171:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.094997, -0.032998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 172:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.069997, -0.032998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 173:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.095997, -0.032998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 174:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.095997, -0.010998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 175:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.095997, -0.010998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 176:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.105997, -0.034998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 177:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.105997, -0.034998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 178:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.083997, -0.014998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 179:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.096997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 180:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.096997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 181:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.129997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 182:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.082997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 183:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 184:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 185:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 186:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 187:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 188:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 189:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 190:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 191:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 192:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 193:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 194:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 195:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 196:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 197:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.104997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 198:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.083997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 199:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.100997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 200:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.100997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 201:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.085997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 202:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.112997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 203:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 204:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 205:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.097997, -0.031998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 206:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.110997, -0.037998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 207:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.066997, -0.037998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 208:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.089997, -0.027998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 209:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.089997, -0.063998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 210:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.089997, -0.063998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 211:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.079997, -0.036998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 212:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.091997, -0.051998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 213:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.091997, -0.051998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 214:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.071997, -0.043998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 215:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.071997, -0.043998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 216:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.071997, -0.043998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 217:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.099997, -0.043998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 218:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.086997, -0.035998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 219:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.070997, -0.040998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 220:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.051997, -0.029998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 221:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.108997, -0.029998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 222:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.108997, -0.029998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 223:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.093997, -0.012998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 224:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.077997, -0.043998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 225:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.077997, -0.043998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 226:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.077997, -0.043998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 227:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.114997, -0.028998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 228:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.106997, -0.010998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 229:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.106997, -0.010998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 230:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.068997, -0.038998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 231:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.101997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 232:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.101997, -0.026998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 233:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.074997, -0.037998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 234:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.074997, -0.037998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 235:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.074997, -0.037998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 236:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.074997, -0.037998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 237:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.074997, -0.037998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 238:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.074997, -0.037998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 239:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.127997, -0.037998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 240:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.092997, -0.037998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 241:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.070997, -0.016998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 242:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.070997, -0.016998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 243:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.070997, -0.016998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 244:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.070997, -0.016998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 245:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.070997, -0.016998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 246:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.070997, -0.016998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 247:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.109997, -0.033998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 248:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.095997, -0.024998, 0.000000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 249:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.095997, -0.018998, 0.007000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 250:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.095997, -0.018998, 0.007000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 251:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.080997, -0.018998, 0.007000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 252:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.096997, -0.040998, 0.007000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 253:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.096997, -0.040998, 0.007000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 254:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.096997, -0.040998, 0.007000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 255:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.096997, -0.040998, 0.007000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 256:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.082997, -0.040998, 0.007000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 257:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.082997, -0.015998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 258:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.090997, -0.035998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 259:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.090997, -0.035998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 260:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.090997, -0.035998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 261:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.090997, -0.064998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 262:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.090997, -0.034998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 263:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.090997, -0.034998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 264:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.072997, -0.024998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 265:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.093997, -0.047998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 266:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.093997, -0.047998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 267:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.093997, -0.047998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 268:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.093997, -0.047998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 269:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.093997, -0.047998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 270:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.093997, -0.047998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 271:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.093997, -0.047998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 272:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.113997, -0.041998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 273:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.084997, -0.041998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 274:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.098997, -0.043998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 275:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.098997, -0.043998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 276:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.098997, -0.043998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 277:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.098997, -0.015998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 278:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.098997, -0.015998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 279:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.098997, -0.015998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 280:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.098997, -0.045998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 281:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.098997, -0.045998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 282:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.098997, -0.045998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 283:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.098997, -0.045998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 284:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.098997, -0.007998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 285:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.098997, -0.037998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 286:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.098997, -0.037998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 287:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.068998, -0.026998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 288:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.092998, -0.051998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 289:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.092998, -0.051998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 290:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.092998, -0.035998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 291:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.092998, -0.035998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 292:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.092998, -0.035998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 293:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.092998, -0.035998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 294:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.092998, -0.035998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 295:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.092998, -0.035998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 296:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.092998, -0.035998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 297:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.092998, -0.035998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 298:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.109998, -0.049998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 299:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.109998, -0.022998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 300:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.088998, -0.047998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 301:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.088998, -0.047998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 302:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.088998, -0.047998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 303:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.088998, -0.047998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 304:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.088998, -0.047998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 305:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.088998, -0.047998, 0.004000, 0.000000, -89.699981, 0.000000, 0.998998, 1.004999, 1.852999, 0xFF000000, 0x0);
		}
		case 306:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.065997, -0.019998, -0.001000, 0.000000, -89.699981, 0.000000, 0.843998, 1.004999, 1.852998, 0xFF000000, 0x0);
		}
		case 307:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.065997, -0.019998, -0.001000, 0.000000, -89.699981, 0.000000, 0.843998, 1.004999, 1.852998, 0xFF000000, 0x0);
		}
		case 308:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.065997, -0.019998, -0.001000, 0.000000, -89.699981, 0.000000, 0.843998, 1.004999, 1.852998, 0xFF000000, 0x0);
		}
		case 309:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.065997, -0.019998, -0.001000, 0.000000, -89.699981, 0.000000, 0.843998, 1.004999, 1.852998, 0xFF000000, 0x0);
		}
		case 310:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.095998, -0.045998, -0.001000, 0.000000, -89.699981, 0.000000, 0.843998, 1.004999, 1.852998, 0xFF000000, 0x0);
		}
		case 311:
		{
			SetPlayerAttachedObject(playerid, BFOLD_OBJ_INDEX, 19422, 2, 0.095998, -0.045998, -0.001000, 0.000000, -89.699981, 0.000000, 0.843998, 1.004999, 1.852998, 0xFF000000, 0x0);
		}
	}
	return 1;
}