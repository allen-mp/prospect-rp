
#define JONAS_BLUE_MYSQL	1423				// Jonas BLue's MySQL row ID
#define JONAS_BLUE_FILE		"jonasblue.ini"		// File for jonas blue's scripting
#define JONAS_BLUE_LIMIT	50000				// Save cash limit
#define JONAS_BLUE_FIXPRICE	1000				// Fix price for Jonas Blue fix cmd


#define DIALOG_JONASCLOTHES 13580

new J_safeBalance;


new JonasBlueFriends[7] = {
	135288,
	135533,
	148675,
	147389,
	1423,
	721,
	135289
};

new JonasBlueClothes[10] = {
	7,
	21,
	29,
	115,
	177,
	185,
	223,
	294,
	292,
	293
};

Hook:Jonas_OnGameModeInit() {
	loadJonasBlueData(); 

	CreateDynamic3DTextLabel("{24FF00}/jonasfixcar", -1, 1226.4174, -1671.0725, 11.7969, 50, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, STREAMER_3D_TEXT_LABEL_SD, -1);
	CreateDynamic3DTextLabel("{24FF00}/jonasclothes", -1, 78.8361, 2459.1733, 2504.9702, 10, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, STREAMER_3D_TEXT_LABEL_SD, -1);
	CreateDynamic3DTextLabel("{24FF00}/jonassafe", -1, 75.5258, 2438.0713, 2498.6101, 10, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, STREAMER_3D_TEXT_LABEL_SD, -1);
	DonorDebug("Jonus Blue 1423");

	return 1;
}

Hook:Jonas_OnGameModeExit() {
	saveJonasBlueData();
	return 1; 
}

CMD:jonassafe(playerid, params[]) {
	if(!isJonasBlue(playerid) && !isJonasBlueFriend(playerid)) return SendClientMessage(playerid, 0xC92C00FF, "You are not allowed to do this.");
	if(!IsPlayerInRangeOfPoint(playerid, 5.0, 75.5258, 2438.0713, 2498.6101)) return SendClientMessage(playerid, 0xC92C00FF, "You are not near the safe.");

	new briefcaseOp[5], briefcaseAm;
	if(sscanf(params, "s[5]i", briefcaseOp, briefcaseAm)) {
		SendClientMessage(playerid, -1, "USAGE: /jonassafe [put/take] [amount]");
		
		new curAmount[128];
		format(curAmount, sizeof(curAmount), "There is $%i in the safe.", J_safeBalance);
		SendClientMessage(playerid, -1, curAmount);
		return 1;
	}
	if(briefcaseAm < 1) return SendClientMessage(playerid, 0xC92C00FF, "Please enter a valid amount.");

	if(!strcmp(briefcaseOp, "put")) {
		if(PlayerInfo[playerid][pCash] < briefcaseAm) return SendClientMessage(playerid, 0xC92C00FF, "You do not have enough money.");
		if(J_safeBalance + briefcaseAm > JONAS_BLUE_LIMIT) return SendClientMessage(playerid, 0xC92C00FF, "The briefcase can not hold this much money.");

		J_safeBalance += briefcaseAm;
		GiveMoney(playerid, -briefcaseAm); 

		new briefcaseUpd[128];
		format(briefcaseUpd, sizeof(briefcaseUpd), "You put $%i in the safe.", briefcaseAm);
		SendClientMessage(playerid, 0x8CFD97FF, briefcaseUpd);
	}
	else if(!strcmp(briefcaseOp, "take")) {
		if(briefcaseAm > J_safeBalance) return SendClientMessage(playerid, 0xC92C00FF, "There is not enough money in the briefcase.");

		J_safeBalance -= briefcaseAm;
		GiveMoney(playerid, briefcaseAm);

		new briefcaseUpd[128];
		format(briefcaseUpd, sizeof(briefcaseUpd), "You take $%i from the safe.", briefcaseAm);
		SendClientMessage(playerid, 0x8CFD97FF, briefcaseUpd);
	}
	else return SendClientMessage(playerid, -1, "USAGE: /jonassafe [put/take] [amount]");
	return 1;
}

CMD:jonasclothes(playerid, params[]) {
	if(!isJonasBlue(playerid) && !isJonasBlueFriend(playerid)) return SendClientMessage(playerid, 0xC92C00FF, "You are not allowed to do this.");
	if(!IsPlayerInRangeOfPoint(playerid, 5.0, 78.8361, 2459.1733, 2504.9702)) return SendClientMessage(playerid, 0xC92C00FF, "You are not near the clothes rack.");
	
	ShowModelSelectionMenuEx(playerid, JonasBlueClothes, 10, "Select your clothes", Jonas_Blue_Clothes_ID,  16.0, 0.0, -55.0);
	return 1;
}

CMD:jonasfixcar(playerid, params[]) {
	if(!IsPlayerInRangeOfPoint(playerid, 5.0, 1226.4174, -1671.0725, 11.7969)) return SendClientMessage(playerid, 0xC92C00FF, "You are not near the vehicle fix point.");
	if(!IsPlayerInAnyVehicle(playerid) || GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendClientMessage(playerid, 0xC92C00FF, "You are not the driver of a vehicle.");
	if(PlayerInfo[playerid][pCash] < JONAS_BLUE_FIXPRICE) { 
		new notEnough[128];
		format(notEnough, sizeof(notEnough), "You cannot afford this. ($%i)", JONAS_BLUE_FIXPRICE);
		SendClientMessage(playerid, 0xC92C00FF, notEnough);
		return 1;
	}

	GivePlayerMoney(playerid, -JONAS_BLUE_FIXPRICE);
	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
	GameTextForPlayer(playerid, "~g~Vehicle fixed", 2000, 3);
	RepairVehicle(GetPlayerVehicleID(playerid));

	// Prevent the safe/briefcase from overflowing
	if(J_safeBalance + JONAS_BLUE_FIXPRICE <= JONAS_BLUE_LIMIT) {
		J_safeBalance += JONAS_BLUE_FIXPRICE;
	}
	return 1;
}

stock isJonasBlue(playerid) { 
	return (PlayerInfo[playerid][pID] == JONAS_BLUE_MYSQL);
}

stock isJonasBlueFriend(playerid) {
	for(new i = 0; i < sizeof(JonasBlueFriends); i++) {
		if(PlayerInfo[playerid][pID] == JonasBlueFriends[i]) return true; 
	}
	return false;
}

stock loadJonasBlueData() {
	if(!dini_Exists(JONAS_BLUE_FILE)) dini_Create(JONAS_BLUE_FILE);

	J_safeBalance = dini_Int(JONAS_BLUE_FILE, "safeBalance");
	return 1;
}

stock saveJonasBlueData() {
	if(!dini_Exists(JONAS_BLUE_FILE)) dini_Create(JONAS_BLUE_FILE);

	dini_IntSet(JONAS_BLUE_FILE, "safeBalance", J_safeBalance);
	return 1;
}


Hook:J_OnPlayerModelSelectionEx(playerid, response, extraid, modelid)
{
	if(extraid == Jonas_Blue_Clothes_ID)
	{
	    if(response)
	    {
		    SendClientMessage(playerid, -1, "Clothes changed");
		    
		    SetPlayerSkin(playerid, modelid);
		}
	    else SendClientMessage(playerid, 0xFF0000FF, "Canceled clothes menu.");
	}
	return 1;
}
