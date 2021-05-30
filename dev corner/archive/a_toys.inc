/**********************
	Alexanders' Toys
	Approved by Smo
***********************/

#define ATOYS_WORLD			777
#define ATOYS_INT			1

#define COLOR_ATOYS			0x78E2FCFF
#define COLOR_ATOYS_ERR		0x8C0606FF

#define DIA_ATOYS_SAFE		32720
#define DIA_ATOYS_WMEN		32721
#define DIA_ATOYS_WMEN_EQ	32722
#define DIA_ATOYS_WMEN_UC	32723
#define DIA_ATOYS_WMEN_UC2	32724
#define DIA_ATOYS_WMEN_UC3	32725
#define DIA_ATOYS_CRIME		32726
#define DIA_ATOYS_CRIME2	32727
#define DIA_ATOYS_CRIME3	32728
#define DIA_ATOYS_CRIME4	32729

#define MAX_CRIMES			20

#define WMEN_DINI 			"wmenkills.ini"

// Crime scene related
enum crimeEnum {
	bool:crimeUsed,
	bool:crimeUseActor,
	crimeActorSkin,
	crimeActor,
	crimeMainRP[128],
	crimeRP1[128],
	crimeRP2[128],
	crimeRP3[128],
	crimeRP4[128],
	crimeRP5[128],
	crimeRP6[128],
	crimeRP7[128],
	crimeRP8[128],
	crimeExpireTime,
	Float:crimeX,
	Float:crimeY,
	Float:crimeZ,
	crimeVW,
	Text3D:crimeTag
};
new CrimeInfo[MAX_CRIMES][crimeEnum];

// Objects
new atoysClown, atoysMovingPlate, atoysMovingDumpster;

// Other variables
new bool:atoysClownTurning = false, atoysClownTimer;
new Text3D:atoysEnterTag, Text3D:atoysExitTag, atoysEnterPickup;
new bool:atoysOpened = false;
new atoysLastWarn = 1;
new bool:atoysVentOpening = false, bool:atoysVentOpened = false;
new bool:atoysVictim[MAX_PLAYERS];
new bool:isTempKiller[MAX_PLAYERS];
new wmenMurders;
new wmenAuthCode;

new crimeOptionEdit[MAX_PLAYERS];
new crimeOptionSkin[MAX_PLAYERS];
new bool:crimeOptionActor[MAX_PLAYERS];
new crimeOption3D[MAX_PLAYERS][128];
new crimeOptionTime[MAX_PLAYERS];
new crimeOptionRP1[MAX_PLAYERS][128];
new crimeOptionRP2[MAX_PLAYERS][128];
new crimeOptionRP3[MAX_PLAYERS][128];
new crimeOptionRP4[MAX_PLAYERS][128];
new crimeOptionRP5[MAX_PLAYERS][128];
new crimeOptionRP6[MAX_PLAYERS][128];
new crimeOptionRP7[MAX_PLAYERS][128];
new crimeOptionRP8[MAX_PLAYERS][128];

new atoysMembers[] = {
	179889, // Alexander
	159542 // Accomplice
};

forward atoysUpdateClown();
forward atoysUnfreeze(playerid);
forward atoysOpenVent(stage);

Hook:at_OnGameModeInit() {
	atoysLoadMap();

	atoysEnterPickup = CreateDynamicPickup(1272, 23, 503.6803, -1406.5255, 16.1598);
	atoysEnterTag = CreateDynamic3DTextLabel("Alexanders' Toys\n{FFFFFF}Address: 64 Rodeo\n{FF0000}Locked", 0x3D97E0FF, 503.6803, -1406.5255, 16.1598, 6.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
	atoysExitTag = CreateDynamic3DTextLabel("{FF0000}Locked", -1, 2692.1194, 1713.2268, 9001.9336, 4.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
	CreateDynamic3DTextLabel("{FFFFFF}/toysafe", -1, 2710.4150, 1710.1219, 9003.2490, 2.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);

	atoysLoadData();
	return 1;
}

Hook:at_OnGameModeExit() {
	KillTimer(atoysClownTimer);

	DestroyDynamic3DTextLabel(atoysEnterTag);
	DestroyDynamic3DTextLabel(atoysExitTag);
	DestroyDynamicPickup(atoysEnterPickup);

	atoysSaveData();
	return 1;
}

Hook:at_OnPlayerConnect(playerid) {
	atoysVictim[playerid] = false;
	crimeOptionTime[playerid] = 10;
	isTempKiller[playerid] = false;
	return 1;
}

Hook:at_OnPlayerDeath(playerid, killerid, reason) {
	if(atoysVictim[playerid] && atoysIsMember(killerid)) {
		SendClientMessage(playerid, COLOR_YELLOW, "You were killed by a serial killer.");
		atoysVictim[playerid] = false;

		new notifyStr[128];

		format(notifyStr, sizeof(notifyStr), "Willowman %s has murdered %s.", PlayerICName(killerid), PlayerICName(playerid));
		wmenMurders++;
		atoysSaveData();

		for(new i = 0; i < MAX_PLAYERS; i++) {
			if(IsPlayerConnected(i) && atoysIsMember(i)) {
				SendClientMessage(i, COLOR_YELLOW, notifyStr);
			}
		}
	}
	return 1;
}

Hook:at_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch(dialogid) {
		// Configure crime scene
		case DIA_ATOYS_CRIME: {
			if(!response)
				return 0;

			switch(listitem) {
				case 0: { // Toggle actor
					crimeOptionActor[playerid] = !crimeOptionActor[playerid];
					atoysShowCrimeDialog(playerid);
				}
				case 1: { // Actor skin
					ShowPlayerDialog(playerid, DIA_ATOYS_CRIME2, DIALOG_STYLE_INPUT, "Change actor skin", "Please enter the skin ID of the crime scene actor.", "Set", "Back");
				}
				case 2: { // 3D text
					ShowPlayerDialog(playerid, DIA_ATOYS_CRIME3, DIALOG_STYLE_INPUT, "Change 3D text", "Please enter the 3D text for this crime scene.\n\
					This text should be a short description of the scene.", "Set", "Back");
				}
				case 3 .. 10: { // RP lines 1-8
					crimeOptionEdit[playerid] = listitem - 2;

					new atoysPreviewText[300];
					format(atoysPreviewText, sizeof(atoysPreviewText), "Current RP line for RP %i is:", crimeOptionEdit[playerid]);

					switch(crimeOptionEdit[playerid]) {
						case 1: format(atoysPreviewText, sizeof(atoysPreviewText), "%s\n%s", atoysPreviewText, crimeOptionRP1[playerid]);
						case 2: format(atoysPreviewText, sizeof(atoysPreviewText), "%s\n%s", atoysPreviewText, crimeOptionRP2[playerid]);
						case 3: format(atoysPreviewText, sizeof(atoysPreviewText), "%s\n%s", atoysPreviewText, crimeOptionRP3[playerid]);
						case 4: format(atoysPreviewText, sizeof(atoysPreviewText), "%s\n%s", atoysPreviewText, crimeOptionRP4[playerid]);
						case 5: format(atoysPreviewText, sizeof(atoysPreviewText), "%s\n%s", atoysPreviewText, crimeOptionRP5[playerid]);
						case 6: format(atoysPreviewText, sizeof(atoysPreviewText), "%s\n%s", atoysPreviewText, crimeOptionRP6[playerid]);
						case 7: format(atoysPreviewText, sizeof(atoysPreviewText), "%s\n%s", atoysPreviewText, crimeOptionRP7[playerid]);
						case 8: format(atoysPreviewText, sizeof(atoysPreviewText), "%s\n%s", atoysPreviewText, crimeOptionRP8[playerid]);
					}

					ShowPlayerDialog(playerid, DIA_ATOYS_CRIME4, DIALOG_STYLE_INPUT, "Change RP line", atoysPreviewText, "Set", "Back");
				}
				case 11: { // Print RP lines
					if(strlen(crimeOptionRP1[playerid]))
						SendClientMessage(playerid, COLOR_PURPLE, crimeOptionRP1[playerid]);

					if(strlen(crimeOptionRP2[playerid]))
						SendClientMessage(playerid, COLOR_PURPLE, crimeOptionRP2[playerid]);

					if(strlen(crimeOptionRP3[playerid]))
						SendClientMessage(playerid, COLOR_PURPLE, crimeOptionRP3[playerid]);

					if(strlen(crimeOptionRP4[playerid]))
						SendClientMessage(playerid, COLOR_PURPLE, crimeOptionRP4[playerid]);

					if(strlen(crimeOptionRP5[playerid]))
						SendClientMessage(playerid, COLOR_PURPLE, crimeOptionRP5[playerid]);

					if(strlen(crimeOptionRP6[playerid]))
						SendClientMessage(playerid, COLOR_PURPLE, crimeOptionRP6[playerid]);

					if(strlen(crimeOptionRP7[playerid]))
						SendClientMessage(playerid, COLOR_PURPLE, crimeOptionRP7[playerid]);

					if(strlen(crimeOptionRP8[playerid]))
						SendClientMessage(playerid, COLOR_PURPLE, crimeOptionRP8[playerid]);
				}
				case 12: { // Expire time
					crimeOptionTime[playerid] += 10;

					if(crimeOptionTime[playerid] > 120) {
						crimeOptionTime[playerid] = 10;
					}

					atoysShowCrimeDialog(playerid);
				}
				case 13: { // Create crime scene
					new Float:foundPos[3];
					GetPlayerPos(playerid, foundPos[0], foundPos[1], foundPos[2]);

					new newCrimeID = atoysCreateCrime(crimeOptionActor[playerid], crimeOptionSkin[playerid], crimeOption3D[playerid], crimeOptionRP1[playerid], crimeOptionRP2[playerid], crimeOptionRP3[playerid], crimeOptionRP4[playerid], crimeOptionRP5[playerid], crimeOptionRP6[playerid], crimeOptionRP7[playerid], crimeOptionRP8[playerid], crimeOptionTime[playerid], foundPos[0], foundPos[1], foundPos[2], GetPlayerVirtualWorld(playerid));

					new notifyStr[128];
					if(newCrimeID != -1) {
						format(notifyStr, sizeof(notifyStr), "Created new crime scene (ID %i)", newCrimeID);

						SetPlayerPos(playerid, foundPos[0]+0.5, foundPos[1], foundPos[2]);
					}
					else {
						format(notifyStr, sizeof(notifyStr), "Crime scene could not be created...");
					}

					SendClientMessage(playerid, COLOR_YELLOW, notifyStr);
					return 1;
				}
			}
		}
		// Set rp line
		case DIA_ATOYS_CRIME4: {
			if(!response)
				return atoysShowCrimeDialog(playerid);

			switch(crimeOptionEdit[playerid]) {
				case 1: format(crimeOptionRP1[playerid], 128, "%s", inputtext);
				case 2: format(crimeOptionRP2[playerid], 128, "%s", inputtext);
				case 3: format(crimeOptionRP3[playerid], 128, "%s", inputtext);
				case 4: format(crimeOptionRP4[playerid], 128, "%s", inputtext);
				case 5: format(crimeOptionRP5[playerid], 128, "%s", inputtext);
				case 6: format(crimeOptionRP6[playerid], 128, "%s", inputtext);
				case 7: format(crimeOptionRP7[playerid], 128, "%s", inputtext);
				case 8: format(crimeOptionRP8[playerid], 128, "%s", inputtext);
			}

			SendClientMessage(playerid, COLOR_ATOYS, "RP line set.");
			atoysShowCrimeDialog(playerid);
		}
		// Set 3D text
		case DIA_ATOYS_CRIME3: {
			if(!response)
				return atoysShowCrimeDialog(playerid);

			if(!strlen(inputtext))
				return 0;

			format(crimeOption3D[playerid], 128, "%s", inputtext);
			SendClientMessage(playerid, COLOR_ATOYS, "3D text set.");
			atoysShowCrimeDialog(playerid);
		}
		// Set actor skin
		case DIA_ATOYS_CRIME2: {
			if(!response)
				return atoysShowCrimeDialog(playerid);

			new atoysNewSkin = strval(inputtext);
			if(atoysNewSkin < 0 || atoysNewSkin > 311)
				return 0;

			crimeOptionSkin[playerid]= atoysNewSkin;
			SendClientMessage(playerid, COLOR_ATOYS, "Skin set.");
			atoysShowCrimeDialog(playerid);
		}
		case DIA_ATOYS_WMEN: {
			if(!response)
				return 0;

			switch(listitem) {
				case 0: { // Equipment
					if(!IsPlayerInRangeOfPoint(playerid, 2.0, 2743.9297, 2549.5410, 8367.0996))
						return SendClientMessage(playerid, COLOR_ATOYS_ERR, "You are not at the locker.");

					ShowPlayerDialog(playerid, DIA_ATOYS_WMEN_EQ, DIALOG_STYLE_LIST, "{333333}Willowmen equipment",
						"{333333}> {FFFFFF}Knife\n\
						{333333}> {FFFFFF}Chainsaw\n\
						{333333}> {FFFFFF}Shovel\n\
						{333333}> {FFFFFF}Desert Eagle\n\
						{333333}> {FFFFFF}Shotgun\n\
						{333333}> {FFFFFF}Rifle\n\
						{333333}> {FFFFFF}C4\n\
						{333333}> {FFFFFF}Rope\n\
						{333333}> {FFFFFF}Blindfold\n\
						{333333}> {FFFFFF}SIM card",
					"Take", "Back");
				}
				case 1: { // Undercover
					ShowPlayerDialog(playerid, DIA_ATOYS_WMEN_UC, DIALOG_STYLE_LIST, "{333333}Willowmen undercover",
						"{333333}> {FFFFFF}Change name\n\
						{333333}> {FFFFFF}Change level",
					"Select", "Back");
				}
				case 2: { // Commands
					cmd_wmen(playerid, "");
				}
			}
			return 1;
		}
		case DIA_ATOYS_WMEN_UC: {
			if(!response)
				return atoysShowWmenDialog(playerid);

			if(listitem == 0) {
				ShowPlayerDialog(playerid, DIA_ATOYS_WMEN_UC2, DIALOG_STYLE_INPUT, "{333333}Willowmen change name",
					"{FFFFFF}Enter the undercover name.\n\n{FFFFFF}(Including underscore, e.g: John_Doe)",
				"Change", "Back");
			} else {
				ShowPlayerDialog(playerid, DIA_ATOYS_WMEN_UC3, DIALOG_STYLE_INPUT, "{333333}Willowmen change level",
					"{FFFFFF}Enter the fake level.",
				"Change", "Back");
			}
		}
		case DIA_ATOYS_WMEN_UC2: {
			if(!response)
				return atoysShowWmenDialog(playerid);

			if(!strlen(inputtext))
				return 1;

			new query[128];
			mysql_format(sqlGameConnection, query, sizeof(query), "SELECT `Name` FROM `players` WHERE `Name` = '%e'", inputtext);
			mysql_pquery(sqlGameConnection, query, "OnSetName", "iis", playerid, playerid, inputtext);
		}
		case DIA_ATOYS_WMEN_UC3: {
			if(!response)
				return atoysShowWmenDialog(playerid);

			new atoysNewLvl = strval(inputtext);

			if(atoysNewLvl < 1 || atoysNewLvl > 100)
				return SendClientMessage(playerid, COLOR_ATOYS_ERR, "Enter a valid level.");

			SetPlayerScore(playerid, atoysNewLvl);
			PlayerInfo[playerid][pLevel] = atoysNewLvl;

			SendClientMessage(playerid, COLOR_ATOYS, "Fake level updated.");
		}
		case DIA_ATOYS_WMEN_EQ: {
			if(!response)
				return atoysShowWmenDialog(playerid);

			new atoysWep;

			switch(listitem) {
				case 0: atoysWep = WEAPON_KNIFE;
				case 1: atoysWep = WEAPON_CHAINSAW;
				case 2: atoysWep = WEAPON_SHOVEL;
				case 3: atoysWep = WEAPON_DEAGLE;
				case 4: atoysWep = WEAPON_SHOTGUN;
				case 5: atoysWep = WEAPON_RIFLE;
				case 6: atoysWep = -1;
				case 7: atoysWep = -2;
				case 8: atoysWep = -3;
				case 9: atoysWep = -4;
			}

			new atoysWepName[40], atoysRPStr[128];
			if(atoysWep == -1) { // C4
				format(atoysWepName, sizeof(atoysWepName), "C4");

				PlayerInfo[playerid][pBombs] = 1;
			}
			else if(atoysWep == -2) { // Rope
				format(atoysWepName, sizeof(atoysWepName), "rope");

				PlayerInfo[playerid][pRope]++;
			}
			else if(atoysWep == -3) { // Blindfold
				format(atoysWepName, sizeof(atoysWepName), "blindfold");

				PlayerInfo[playerid][pBlindfolds]++;
			}
			else if(atoysWep == -4) { // SIM
				if(PlayerInfo[playerid][pPnumber]) {
					format(atoysRPStr, sizeof(atoysRPStr), "* %s takes the SIM card out of their cellphone and crushes it.", PlayerICName(playerid));
					ProxDetector(30.0, playerid, atoysRPStr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				}

				format(atoysWepName, sizeof(atoysWepName), "new SIM card");

				PlayerInfo[playerid][pPnumber] = 1000 + random(8999);
			}
			else {
				GivePlayerGun(playerid, atoysWep);
				SetPlayerArmedWeapon(playerid, atoysWep);
				GetWeaponName(atoysWep, atoysWepName, sizeof(atoysWepName));
			}

			format(atoysRPStr, sizeof(atoysRPStr), "* %s retrieves a %s from the locker.", PlayerICName(playerid), atoysWepName);
			ProxDetector(30.0, playerid, atoysRPStr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		}
		case DIA_ATOYS_SAFE: {
			if(!response)
				return 0;

			new atoysEnteredCode = strval(inputtext);

			if(atoysEnteredCode == 1234) {
				SendClientMessage(playerid, COLOR_ATOYS, "Alexanders' Toys safe:");
				SendClientMessage(playerid, -1, "Cash: $0");
				SendClientMessage(playerid, -1, "Toy-parts: 0 parts");
			}
			else if(atoysEnteredCode == wmenAuthCode) {
				if(atoysVentOpening) return 0;

				GameTextForPlayer(playerid, "~g~Pin accepted.", 1000, 3);

				atoysVentOpening = true;
				atoysOpenVent(1);
			}
			else {
				SendClientMessage(playerid, COLOR_ATOYS_ERR, "Invalid pin!");

				if(gettime() - atoysLastWarn > 30) {
					for(new i = 0; i < MAX_PLAYERS; i++) {
						if(IsPlayerConnected(i) && atoysIsMember(i)) {
							if(!PhoneOffline[i] && PlayerInfo[playerid][pPnumber] != 0) {
								SendClientMessage(i, COLOR_YELLOW, "SMS: Invalid safe pin submitted, be aware!, Sender: MXC-Security (555)");
								PlayerPlaySound(playerid, SMS_RECIEVED, 0.0, 0.0, 0.0);
							}
						}
					}
					atoysLastWarn = gettime();
				}
			}
		}
	}
	return 1;
}

Hook:at_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(PRESSED(KEY_YES)) {
		if(atoysIsMember(playerid)) {
			// Toggle clown toy
			if(atoysNearObject(playerid, atoysClown, 4.0)) {
				atoysToggleClown();
			}

			// Lock/unlock toy store
			if(	IsPlayerInRangeOfPoint(playerid, 5.0, 2692.1194, 1713.2268, 9001.9336) ||
				IsPlayerInRangeOfPoint(playerid, 5.0, 503.6803, -1406.5255, 16.1598)) {
				atoysOpened = !atoysOpened;

				if(atoysOpened) {
					GameTextForPlayer(playerid, "~g~Unlocked.", 1000, 3);
					UpdateDynamic3DTextLabelText(atoysEnterTag, 0x3D97E0FF, "Alexanders' Toys\n{FFFFFF}Address: 64 Rodeo\n{FFFFFF}/entertoy");
					UpdateDynamic3DTextLabelText(atoysExitTag, -1, "{FFFFFF}/exittoy");
				}
				else {
					GameTextForPlayer(playerid, "~r~Locked.", 1000, 3);
					UpdateDynamic3DTextLabelText(atoysEnterTag, 0x3D97E0FF, "Alexanders' Toys\n{FFFFFF}Address: 64 Rodeo\n{FF0000}Locked");
					UpdateDynamic3DTextLabelText(atoysExitTag, -1, "{FF0000}Locked");
				}
			}
			// Move dumpster
			else if(IsPlayerInRangeOfPoint(playerid, 2.0, 2746.7451, 2551.8579, 8367.0996)) {
				if(!atoysVentOpened && !atoysVentOpening) {
					atoysVentOpening = true;
					atoysOpenVent(1);
				}
			}
			// Locker
			else if(IsPlayerInRangeOfPoint(playerid, 2.0, 2743.9297, 2549.5410, 8367.0996)) {
				atoysShowWmenDialog(playerid);
			}
			// Upstairs to downstairs
			else if(IsPlayerInRangeOfPoint(playerid, 4.0, 573.1486, -1570.6709, 31.8074)) {
				SetPlayerPos(playerid, 574.6300, -1571.6716, 16.1797);
			}
			// Downstairs to upstairs
			else if(IsPlayerInRangeOfPoint(playerid, 4.0, 574.6300, -1571.6716, 16.1797)) {
				SetPlayerPos(playerid, 573.1486, -1570.6709, 31.8074);
			}
		}
	}
	return 1;
}

CMD:inspectcrime(playerid, params[]) {
	for(new i = 0; i < MAX_CRIMES; i++) {
		if(!CrimeInfo[i][crimeUsed]) continue;

		if(!IsPlayerInRangeOfPoint(playerid, 4.0, CrimeInfo[i][crimeX], CrimeInfo[i][crimeY], CrimeInfo[i][crimeZ])) continue;

		if(GetPlayerVirtualWorld(playerid) != CrimeInfo[i][crimeVW]) continue;

		SendClientMessage(playerid, -1, "* Crime scene:");
		if(strlen(CrimeInfo[i][crimeRP1]))
			SendClientMessage(playerid, COLOR_PURPLE, CrimeInfo[i][crimeRP1]);

		if(strlen(CrimeInfo[i][crimeRP2]))
			SendClientMessage(playerid, COLOR_PURPLE, CrimeInfo[i][crimeRP2]);

		if(strlen(CrimeInfo[i][crimeRP3]))
			SendClientMessage(playerid, COLOR_PURPLE, CrimeInfo[i][crimeRP3]);

		if(strlen(CrimeInfo[i][crimeRP4]))
			SendClientMessage(playerid, COLOR_PURPLE, CrimeInfo[i][crimeRP4]);

		if(strlen(CrimeInfo[i][crimeRP5]))
			SendClientMessage(playerid, COLOR_PURPLE, CrimeInfo[i][crimeRP5]);

		if(strlen(CrimeInfo[i][crimeRP6]))
			SendClientMessage(playerid, COLOR_PURPLE, CrimeInfo[i][crimeRP6]);

		if(strlen(CrimeInfo[i][crimeRP7]))
			SendClientMessage(playerid, COLOR_PURPLE, CrimeInfo[i][crimeRP7]);

		if(strlen(CrimeInfo[i][crimeRP8]))
			SendClientMessage(playerid, COLOR_PURPLE, CrimeInfo[i][crimeRP8]);
		return 1;
	}
	SendClientMessage(playerid, COLOR_GREY, "You are not near any crime scene.");
	return 1;
}

// Checks how many kills the serial killers have.
CMD:wmenkills(playerid) {
	if(!atoysIsMember(playerid) || PlayerInfo[playerid][pAdmin] < 1337) return 1;

	new str[128];

	format(str, sizeof(str), "The willowmen have %i total murders.", wmenMurders);
	SendClientMessage(playerid, COLOR_YELLOW, str);
	return 1;
}

CMD:privacy(playerid, params[]) {
	if(!atoysIsMember(playerid)) return 1;

	ClearIP(playerid);
	SendClientMessage(playerid, -1, "Protected.");
	return 1;
}

CMD:wmen(playerid, params[]) {
	if(!atoysIsMember(playerid)) return 1;

	SendClientMessage(playerid, COLOR_YELLOW, "____________________________________________");
	SendClientMessage(playerid, -1, "Willowmen: /wmen /victim /wmendrag /entertoy /exittoy /crawlvent /toysafe");
	SendClientMessage(playerid, -1, "Willowmen: /wmenkills /wmencode /makecrime /wmencrimes /privacy /maketempwmen");
	return 1;
}

CMD:wmencrimes(playerid, params[]) {
	if(!atoysIsMember(playerid))
		return 1;

	SendClientMessage(playerid, -1, "Currently active crime scenes:");

	new notifyStr[128];
	for(new i = 0; i < MAX_CRIMES; i++) {
		if(!CrimeInfo[i][crimeUsed]) continue;

		format(notifyStr, sizeof(notifyStr), "- ID: %i, Expires in %i seconds", i, CrimeInfo[i][crimeExpireTime] - gettime());
		SendClientMessage(playerid, COLOR_YELLOW, notifyStr);
		format(notifyStr, sizeof(notifyStr), "- 3D text: %s", CrimeInfo[i][crimeMainRP]);
		SendClientMessage(playerid, COLOR_YELLOW, notifyStr);
		SendClientMessage(playerid, COLOR_YELLOW, "____________________________________");
	}

	SendClientMessage(playerid, COLOR_GREY, "HINT: /delcrime (id) to delete a crime scene.");
	return 1;
}

CMD:delcrime(playerid, params[]) {
	if(!atoysIsMember(playerid))
		return 1;

	new specifiedCrime;
	if(sscanf(params, "i", specifiedCrime))
		return SendClientMessage(playerid, COLOR_ATOYS_ERR, "USAGE: /delcrime (id)");

	if(!CrimeInfo[specifiedCrime][crimeUsed])
		return SendClientMessage(playerid, COLOR_ATOYS_ERR, "Unknown crime!");

	new attemptDeleteC = atoysDeleteCrime(specifiedCrime);

	if(attemptDeleteC) {
		SendClientMessage(playerid, COLOR_ATOYS, "The crime has been deleted.");
	}
	else {
		SendClientMessage(playerid, COLOR_ATOYS, "The crime has been deleted.");
	}
	return 1;
}

CMD:wmencode(playerid, params[]) {
	if(!atoysIsMember(playerid))
		return 1;

	new newCode[10], notifyStr[128];
	if(sscanf(params, "s[10]", newCode)) {
		SendClientMessage(playerid, COLOR_ATOYS_ERR, "USAGE: /wmencode (new code)");
		format(notifyStr, sizeof(notifyStr), "* Current code: %i", wmenAuthCode);
		SendClientMessage(playerid, COLOR_ATOYS_ERR, notifyStr);
		return 1;
	}

	wmenAuthCode = strval(newCode);

	format(notifyStr, sizeof(notifyStr), "* Code set to %i", wmenAuthCode);
	SendClientMessage(playerid, COLOR_ATOYS, notifyStr);

	atoysSaveData();
	return 1;
}

CMD:toysafe(playerid, params[]) {
	if(!IsPlayerInRangeOfPoint(playerid, 1.0, 2710.4153, 1710.5253, 9002.1523))
		return 1;

	ShowPlayerDialog(playerid, DIA_ATOYS_SAFE, DIALOG_STYLE_PASSWORD, "Alexanders' Toys Safe", "{FFFFFF}Enter the 4 digit pin.", "Enter", "Close");
	return 1;
}

CMD:makecrime(playerid, params[]) {
	if(!atoysIsMember(playerid))
		return 1;

	atoysShowCrimeDialog(playerid);
	return 1;
}

CMD:maketempwmen(playerid, params[]) {
	if(!atoysIsMember(playerid))
		return 1;

	new targetid;
	if(sscanf(params, "u", targetid))
		return SendClientMessage(playerid, COLOR_ATOYS_ERR, "USAGE: /maketempwmen (player)");

	if(!IsPlayerConnected(targetid))
		return SendClientMessage(playerid, COLOR_ATOYS_ERR, "Invalid player.");

	isTempKiller[targetid] = !isTempKiller[targetid];

	new notifyStr[128];

	if(isTempKiller[targetid]) {
		format(notifyStr, sizeof(notifyStr), "Willowman %s has made %s a serial killer.", PlayerICName(playerid), PlayerICName(targetid));
	}
	else {
		format(notifyStr, sizeof(notifyStr), "Willowman %s has removed %s's serial killer status.", PlayerICName(playerid), PlayerICName(targetid));
	}

	for(new i = 0; i < MAX_PLAYERS; i++) {
		if(IsPlayerConnected(i) && atoysIsMember(i)) {
			SendClientMessage(i, COLOR_YELLOW, notifyStr);
		}
	}
	return 1;
}

CMD:victim(playerid, params[]) {
	if(!atoysIsMember(playerid))
		return 1;

	new targetid;
	if(sscanf(params, "u", targetid))
		return SendClientMessage(playerid, COLOR_ATOYS_ERR, "USAGE: /victim (player)");

	if(!IsPlayerConnected(targetid))
		return SendClientMessage(playerid, COLOR_ATOYS_ERR, "Invalid player.");

	atoysVictim[targetid] = !atoysVictim[targetid];

	new notifyStr[128];

	if(atoysVictim[targetid]) {
		format(notifyStr, sizeof(notifyStr), "Willowman %s has tasked themselves with the murder of %s.", PlayerICName(playerid), PlayerICName(targetid));
	}
	else {
		format(notifyStr, sizeof(notifyStr), "Willowman %s cancelled the murder of %s.", PlayerICName(playerid), PlayerICName(targetid));
	}

	for(new i = 0; i < MAX_PLAYERS; i++) {
		if(IsPlayerConnected(i) && atoysIsMember(i)) {
			SendClientMessage(i, COLOR_YELLOW, notifyStr);
		}
	}
	return 1;
}

CMD:wmendrag(playerid, params[]) {
	if(!atoysIsMember(playerid))
		return 1;

	new targetid;
	if(sscanf(params, "u", targetid))
		return SendClientMessage(playerid, COLOR_ATOYS_ERR, "USAGE: /wmendrag (player)");

	if(!IsPlayerConnected(targetid))
		return SendClientMessage(playerid, COLOR_ATOYS_ERR, "Invalid player.");

	new atoysOldMember = PlayerInfo[playerid][pMember];
	new oldCuffedStatus = PlayerCuffed[targetid];

	PlayerInfo[playerid][pMember] = 1;
	PlayerCuffed[targetid] = 1;
	cmd_drag(playerid, params);
	PlayerCuffed[targetid] = oldCuffedStatus;
	PlayerInfo[playerid][pMember] = atoysOldMember;
	return 1;
}

CMD:crawlvent(playerid, params[]) {
	if(!atoysIsMember(playerid))
		return 1;

	if(!atoysVentOpened)
		return 1;

	// Store to room
	if(IsPlayerInRangeOfPoint(playerid, 1.0, 2713.6777, 1710.2201, 9002.1523)) {
		TogglePlayerControllable(playerid, false);
		SetTimerEx("atoysUnfreeze", 1000, false, "i", playerid);

		SetPlayerPos(playerid, 2746.7451, 2551.8579, 8367.0996);
		SetPlayerFacingAngle(playerid, 90);
		SetCameraBehindPlayer(playerid);
	}
	// Room to store
	else if(IsPlayerInRangeOfPoint(playerid, 1.0, 2747.7676, 2552.2476, 8367.0996)) {
		TogglePlayerControllable(playerid, false);
		SetTimerEx("atoysUnfreeze", 1000, false, "i", playerid);

		SetPlayerPos(playerid, 2713.6777, 1710.2201, 9002.1523);
		SetPlayerFacingAngle(playerid, 68);
		SetCameraBehindPlayer(playerid);
	}
	return 1;
}

CMD:entertoy(playerid, params[]) {
	// Enter toy
	if(IsPlayerInRangeOfPoint(playerid, 5.0, 503.6803, -1406.5255, 16.1598))
	{
		if(!atoysOpened)
			return 1;

		if(GetPlayerVirtualWorld(playerid) != 0 || GetPlayerInterior(playerid) != 0)
			return 1;

		TogglePlayerControllable(playerid, false);
		SetTimerEx("atoysUnfreeze", 2000, false, "i", playerid);

		SetPlayerVirtualWorld(playerid, ATOYS_WORLD);
		SetPlayerInterior(playerid, ATOYS_INT);
		SetPlayerPos(playerid, 2692.1194, 1713.2268, 9001.9336);
		SetPlayerFacingAngle(playerid, 270);
		SetCameraBehindPlayer(playerid);

		SendClientMessage(playerid, COLOR_ATOYS, "* Welcome to Alexanders' Toys!");
	}
	// Enter room from roof
	else if(IsPlayerInRangeOfPoint(playerid, 5.0, 575.5459, -1474.5901, 28.0703))
	{
		if(GetPlayerVirtualWorld(playerid) != 0 || GetPlayerInterior(playerid) != 0)
			return 1;

		if(!atoysIsMember(playerid))
			return 1;

		TogglePlayerControllable(playerid, false);
		SetTimerEx("atoysUnfreeze", 2000, false, "i", playerid);

		SetPlayerVirtualWorld(playerid, ATOYS_WORLD);
		SetPlayerInterior(playerid, ATOYS_INT);
		SetPlayerPos(playerid, 2745.8486, 2552.3496, 8367.0996);
		SetPlayerFacingAngle(playerid, 180);
		SetCameraBehindPlayer(playerid);
	}
	return 1;
}

CMD:exittoy(playerid, params[]) {
	// Exit toy
	if(IsPlayerInRangeOfPoint(playerid, 5.0, 2692.1194, 1713.2268, 9001.9336))
	{
		if(GetPlayerVirtualWorld(playerid) != ATOYS_WORLD || GetPlayerInterior(playerid) != ATOYS_INT)
			return 1;

		if(!atoysOpened)
			return 1;

		TogglePlayerControllable(playerid, true);

		SetPlayerVirtualWorld(playerid, 0);
		SetPlayerInterior(playerid, 0);
		SetPlayerPos(playerid, 503.6803, -1406.5255, 16.1598);
		SetPlayerFacingAngle(playerid, 180);
		SetCameraBehindPlayer(playerid);
	}
	// Exit room
	else if(IsPlayerInRangeOfPoint(playerid, 5.0, 2745.8486, 2552.3496, 8367.0996))
	{
		if(GetPlayerVirtualWorld(playerid) != ATOYS_WORLD || GetPlayerInterior(playerid) != ATOYS_INT)
			return 1;

		if(!atoysIsMember(playerid))
			return 1;

		TogglePlayerControllable(playerid, true);

		SetPlayerVirtualWorld(playerid, 0);
		SetPlayerInterior(playerid, 0);
		SetPlayerPos(playerid, 575.5386, -1474.3624, 28.0703);
		SetPlayerFacingAngle(playerid, 0);
		SetCameraBehindPlayer(playerid);
	}
	return 1;
}

public atoysOpenVent(stage) {
	switch(stage) {
		case 1: {
			// Move plate and dumpster
			new Float:atoysObjPos[3];
			GetDynamicObjectPos(atoysMovingPlate, atoysObjPos[0], atoysObjPos[1], atoysObjPos[2]);
			MoveDynamicObject(atoysMovingPlate, atoysObjPos[0] + 3.0, atoysObjPos[1], atoysObjPos[2], 5.0);

			GetDynamicObjectPos(atoysMovingDumpster, atoysObjPos[0], atoysObjPos[1], atoysObjPos[2]);
			MoveDynamicObject(atoysMovingDumpster, atoysObjPos[0], atoysObjPos[1] - 2.5, atoysObjPos[2], 5.0);
		}
		case 2: {
			atoysVentOpened = true;
			SetTimerEx("atoysOpenVent", 6000, false, "i", 3);
		}
		case 3: {
			// Move plate and dumpster
			new Float:atoysObjPos[3];
			GetDynamicObjectPos(atoysMovingPlate, atoysObjPos[0], atoysObjPos[1], atoysObjPos[2]);
			MoveDynamicObject(atoysMovingPlate, atoysObjPos[0] - 3.0, atoysObjPos[1], atoysObjPos[2], 5.0);

			GetDynamicObjectPos(atoysMovingDumpster, atoysObjPos[0], atoysObjPos[1], atoysObjPos[2]);
			MoveDynamicObject(atoysMovingDumpster, atoysObjPos[0], atoysObjPos[1] + 2.5, atoysObjPos[2], 5.0);
		}
	}
}

Hook:at_OnDynamicObjectMoved(objectid) {
	if(objectid == atoysMovingPlate && atoysVentOpened == false) {
		atoysOpenVent(2);
	}
	else if(objectid == atoysMovingPlate && atoysVentOpened == true) {
		atoysVentOpened = false;
		atoysVentOpening = false;
	}
	return 1;
}

atoysShowWmenDialog(playerid)
	return ShowPlayerDialog(playerid, DIA_ATOYS_WMEN, DIALOG_STYLE_LIST, "{333333}Willowmen menu", "{333333}> {FFFFFF}Equipment\n{333333}> {FFFFFF}Undercover\n{333333}> {FFFFFF}Commands", "Select", "Close");

atoysToggleClown() {
	if(atoysClownTurning) {
		KillTimer(atoysClownTimer);
		atoysClownTurning = false;
	}
	else {
		atoysClownTimer = SetTimer("atoysUpdateClown", 50, true);
		atoysClownTurning = true;
	}
}

atoysIsMember(playerid) {
	if(isTempKiller[playerid]) return 1;

	for(new i = 0; i < sizeof(atoysMembers); i++) {
		if(PlayerInfo[playerid][pID] == atoysMembers[i]) return 1;
	}
	return 0;
}

atoysLoadData() {
	if(!dini_Exists(WMEN_DINI)) dini_Create(WMEN_DINI);

	wmenMurders = dini_Int(WMEN_DINI, "wmenMurders");
	wmenAuthCode = dini_Int(WMEN_DINI, "wmenAuthCode");
	return 1;
}

atoysSaveData() {
	if(!dini_Exists(WMEN_DINI)) dini_Create(WMEN_DINI);

	dini_IntSet(WMEN_DINI, "wmenMurders", wmenMurders);
	dini_IntSet(WMEN_DINI, "wmenAuthCode", wmenAuthCode);
	return 1;
}

atoysShowCrimeDialog(playerid) {
	new crimeBuildStr[2000];

	// Use actor
	format(crimeBuildStr, sizeof(crimeBuildStr), "Use actor");
	if(crimeOptionActor[playerid]) {
		format(crimeBuildStr, sizeof(crimeBuildStr), "%s [Yes]", crimeBuildStr);
	}
	else {
		format(crimeBuildStr, sizeof(crimeBuildStr), "%s [No]", crimeBuildStr);
	}

	// Actor skin
	format(crimeBuildStr, sizeof(crimeBuildStr), "%s\nActor skin [%i]", crimeBuildStr, crimeOptionSkin[playerid]);

	// Crime 3D text
	format(crimeBuildStr, sizeof(crimeBuildStr), "%s\n3D Text", crimeBuildStr);

	// RP lines
	format(crimeBuildStr, sizeof(crimeBuildStr), "%s\n\
	1: %s\n\
	2: %s\n\
	3: %s\n\
	4: %s\n\
	5: %s\n\
	6: %s\n\
	7: %s\n\
	8: %s\n\
	-> Print all RP lines", crimeBuildStr, crimeOptionRP1[playerid], crimeOptionRP2[playerid], crimeOptionRP3[playerid], crimeOptionRP4[playerid], crimeOptionRP5[playerid], crimeOptionRP6[playerid], crimeOptionRP7[playerid], crimeOptionRP8[playerid]);

	// Expire time
	format(crimeBuildStr, sizeof(crimeBuildStr), "%s\nExpire time [%i minutes]", crimeBuildStr, crimeOptionTime);

	// Create crime scene
	format(crimeBuildStr, sizeof(crimeBuildStr), "%s\nCreate crime scene", crimeBuildStr);

	return ShowPlayerDialog(playerid, DIA_ATOYS_CRIME, DIALOG_STYLE_LIST, "Crime Scene", crimeBuildStr, "Select", "Close");
}

forward atoysDeleteCrime(crimeID);
public atoysDeleteCrime(crimeID) {
	if(!CrimeInfo[crimeID][crimeUsed]) return 0;

	CrimeInfo[crimeID][crimeUsed] = false;

	if(CrimeInfo[crimeID][crimeUseActor])
		DestroyActor(CrimeInfo[crimeID][crimeActor]);

	DestroyDynamic3DTextLabel(CrimeInfo[crimeID][crimeTag]);
	return 1;
}

atoysCreateCrime(
	bool:useActor_,
	actorSkin_,
	mainRP_[],
	lineRP1_[],
	lineRP2_[],
	lineRP3_[],
	lineRP4_[],
	lineRP5_[],
	lineRP6_[],
	lineRP7_[],
	lineRP8_[],
	timeExp_,
	Float:cPosx_,
	Float:cPosy_,
	Float:cPosz_,
	cPosvw_
	)
{
	for(new i = 0; i < MAX_CRIMES; i++) {
		if(!CrimeInfo[i][crimeUsed]) {
			CrimeInfo[i][crimeUsed] = true;
			CrimeInfo[i][crimeUseActor] = useActor_;
			CrimeInfo[i][crimeActorSkin] = actorSkin_;
			CrimeInfo[i][crimeExpireTime] = gettime() + (timeExp_ * 60);
			format(CrimeInfo[i][crimeMainRP], 128, "%s", mainRP_);
			format(CrimeInfo[i][crimeRP1], 128, "%s", lineRP1_);
			format(CrimeInfo[i][crimeRP2], 128, "%s", lineRP2_);
			format(CrimeInfo[i][crimeRP3], 128, "%s", lineRP3_);
			format(CrimeInfo[i][crimeRP4], 128, "%s", lineRP4_);
			format(CrimeInfo[i][crimeRP5], 128, "%s", lineRP5_);
			format(CrimeInfo[i][crimeRP6], 128, "%s", lineRP6_);
			format(CrimeInfo[i][crimeRP7], 128, "%s", lineRP7_);
			format(CrimeInfo[i][crimeRP8], 128, "%s", lineRP8_);
			CrimeInfo[i][crimeX] = cPosx_;
			CrimeInfo[i][crimeY] = cPosy_;
			CrimeInfo[i][crimeZ] = cPosz_;
			CrimeInfo[i][crimeVW] = cPosvw_;

			if(CrimeInfo[i][crimeUseActor]) {
				CrimeInfo[i][crimeActor] = CreateActor(CrimeInfo[i][crimeActorSkin], CrimeInfo[i][crimeX], CrimeInfo[i][crimeY], CrimeInfo[i][crimeZ], 0.0);

				ApplyActorAnimation(CrimeInfo[i][crimeActor], "KNIFE", "KILL_Knife_Ped_Die", 4.0, 0, 1, 1, 1, 0);
			}

			new text3dStr[300];
			format(text3dStr, sizeof(text3dStr), "%s\n{FFFFFF}(( /inspectcrime ))", CrimeInfo[i][crimeMainRP]);

			CrimeInfo[i][crimeTag] = CreateDynamic3DTextLabel(text3dStr, 0xC2A2DAFF, CrimeInfo[i][crimeX], CrimeInfo[i][crimeY], CrimeInfo[i][crimeZ], 6.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, CrimeInfo[i][crimeVW], -1, -1, 100);

			SetTimerEx("atoysDeleteCrime", timeExp_ * 60000, false, "i", i);
			return i;
		}
	}
	return -1;
}

atoysNearObject(playerid, anObj, Float:aRange) {
	new Float:atoysObjPos[3];
	GetDynamicObjectPos(anObj, atoysObjPos[0], atoysObjPos[1], atoysObjPos[2]);

	if(IsPlayerInRangeOfPoint(playerid, aRange, atoysObjPos[0], atoysObjPos[1], atoysObjPos[2]))
		return 1;

	return 0;
}

public atoysUpdateClown() {
	new Float:atoysClownRot[3];
	GetDynamicObjectRot(atoysClown, atoysClownRot[0], atoysClownRot[1], atoysClownRot[2]);

	if(atoysClownRot[2] >= 360) {
		SetDynamicObjectRot(atoysClown, atoysClownRot[0], atoysClownRot[1], 0.0);
	}
	else {
		SetDynamicObjectRot(atoysClown, atoysClownRot[0], atoysClownRot[1], atoysClownRot[2] + 1.0);
	}
	return 1;
}

public atoysUnfreeze(playerid) {
	TogglePlayerControllable(playerid, true);
	return 1;
}

atoysLoadMap() {
	// CreateDynamicObject(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, worldid = -1, interiorid = -1, playerid = -1, Float:streamdistance = STREAMER_OBJECT_SD, Float:drawdistance = STREAMER_OBJECT_DD, STREAMER_TAG_AREA areaid = STREAMER_TAG_AREA -1, priority = 0)

	new atoysObj;
	atoysClown = CreateDynamicObject(19353, 2701.574707, 1708.561645, 9003.016601, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	SetDynamicObjectMaterial(atoysClown, 0, 7231, "vgncircir2", "Clown_Pock_sign1c2", 0x00000000);
	atoysObj = CreateDynamicObject(1491, 2691.251708, 1703.606689, 9000.933593, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	SetDynamicObjectMaterial(atoysObj, 0, 14581, "ab_mafiasuitea", "kit_door1", 0x00000000);
	atoysObj = CreateDynamicObject(19482, 2693.355712, 1703.416015, 9002.394531, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	SetDynamicObjectMaterialText(atoysObj, 0, "Alexanders'", 90, "Ariel", 20, 1, 0xFFFFFFFF, 0x00000000, 0);
	atoysObj = CreateDynamicObject(19482, 2691.254150, 1703.416015, 9002.394531, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	SetDynamicObjectMaterialText(atoysObj, 0, "Toys", 90, "Ariel", 20, 1, 0xFFFFFFFF, 0x00000000, 0);
	atoysObj = CreateDynamicObject(19482, 2693.156005, 1703.416015, 9001.962890, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	SetDynamicObjectMaterialText(atoysObj, 0, "A happy place!", 90, "Ariel", 20, 1, 0xFFBD0000, 0x00000000, 0);
	atoysObj = CreateDynamicObject(19482, 2691.293945, 1715.207275, 9002.462890, 0.000000, 0.000000, 360.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	SetDynamicObjectMaterialText(atoysObj, 0, "Goodbye.", 90, "Ariel", 20, 1, 0xFFFFFFFF, 0x00000000, 0);
	atoysObj = CreateDynamicObject(19834, 2710.506591, 1708.674682, 9003.062500, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	SetDynamicObjectMaterial(atoysObj, 0, 10765, "airportgnd_sfse", "black64", 0x00000000);
	atoysObj = CreateDynamicObject(19834, 2711.637207, 1708.674682, 9001.965820, 0.000000, 90.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	SetDynamicObjectMaterial(atoysObj, 0, 10765, "airportgnd_sfse", "black64", 0x00000000);
	atoysObj = CreateDynamicObject(19378, 2747.741943, 2548.399658, 8366.013671, 0.000000, 90.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	SetDynamicObjectMaterial(atoysObj, 0, 16640, "a51", "concretegroundl1_256", 0x00000000);
	atoysObj = CreateDynamicObject(19378, 2747.741943, 2548.399658, 8369.318359, 0.000000, 90.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	SetDynamicObjectMaterial(atoysObj, 0, 16640, "a51", "concretegroundl1_256", 0x00000000);
	atoysObj = CreateDynamicObject(19482, 2748.146240, 2550.644287, 8366.647460, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	SetDynamicObjectMaterialText(atoysObj, 0, "IIII", 90, "Ariel", 20, 0, 0xFFFF0000, 0x00000000, 0);
	atoysObj = CreateDynamicObject(19482, 2748.146240, 2549.883544, 8366.735351, 4.399999, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	SetDynamicObjectMaterialText(atoysObj, 0, "IIII", 90, "Ariel", 20, 0, 0xFFFF0000, 0x00000000, 0);
	atoysObj = CreateDynamicObject(19482, 2748.156250, 2549.552001, 8369.378906, 55.999942, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	SetDynamicObjectMaterialText(atoysObj, 0, "I", 50, "Ariel", 20, 0, 0xFFFF0000, 0x00000000, 0);
	atoysObj = CreateDynamicObject(19482, 2748.156250, 2550.364501, 8369.493164, 55.999942, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	SetDynamicObjectMaterialText(atoysObj, 0, "I", 50, "Ariel", 20, 0, 0xFFFF0000, 0x00000000, 0);
	atoysObj = CreateDynamicObject(19482, 2742.515136, 2552.976806, 8369.288085, 55.999942, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	SetDynamicObjectMaterialText(atoysObj, 0, "X", 50, "Wingdings", 20, 0, 0xFFFF0000, 0x00000000, 0);

	CreateDynamicObject(1441, 2713.663818, 1711.881835, 9001.796875, 0.000000, 0.000000, -90.700012, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2464, 2712.875976, 1709.112182, 9002.142578, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(945, 2712.910644, 1710.725952, 9010.952148, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(3576, 2710.685791, 1709.426635, 9001.133789, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2332, 2710.050781, 1709.072021, 9002.962890, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2886, 2710.227294, 1709.321655, 9003.002929, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(1721, 2712.590332, 1712.765502, 9001.152343, 0.000000, 0.000000, 105.799987, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2002, 2710.092285, 1711.081665, 9001.128906, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2986, 2714.121826, 1708.689208, 9001.442382, 450.000000, 0.000000, 360.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(14531, 2702.917236, 1708.802856, 9004.550781, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(19810, 2709.269042, 1711.361206, 9002.906250, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2368, 2693.556396, 1705.565307, 9000.875000, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2368, 2696.398193, 1705.565307, 9000.875000, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(3498, 2701.488037, 1708.593261, 8996.892578, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(1886, 2691.747314, 1713.672729, 9004.954101, 13.599998, 0.000000, 39.400001, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(1886, 2691.790527, 1703.918579, 9005.019531, 13.599998, 0.000000, 138.799957, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(19355, 2689.486328, 1703.742553, 9000.852539, 0.000000, 90.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(19355, 2689.556396, 1705.153564, 9002.635742, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(19355, 2689.556396, 1703.552490, 9002.635742, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(19355, 2691.047607, 1703.552490, 9005.188476, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(19355, 2689.387207, 1703.552490, 9002.646484, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(19355, 2689.377197, 1703.552490, 9003.888671, 0.000000, 90.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(1496, 2689.545410, 1703.622192, 9000.933593, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(1514, 2693.634765, 1705.648315, 9002.143554, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(1514, 2696.417480, 1705.648315, 9002.143554, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(912, 2699.416259, 1703.707153, 9001.432617, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(912, 2700.837402, 1703.707153, 9001.432617, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(912, 2707.581298, 1703.707153, 9001.432617, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(912, 2706.160400, 1703.707153, 9001.432617, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(912, 2703.448974, 1703.707153, 9001.432617, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2473, 2703.616455, 1703.458007, 9002.019531, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2488, 2699.475097, 1703.448974, 9002.149414, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2488, 2700.886474, 1703.448974, 9002.149414, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2488, 2700.886474, 1703.709228, 9002.149414, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2488, 2699.475585, 1703.709228, 9002.149414, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2488, 2699.475585, 1703.939453, 9002.149414, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2488, 2700.886962, 1703.939453, 9002.149414, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(14860, 2705.831787, 1700.840087, 9002.916992, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2028, 2706.516357, 1703.717651, 9002.056640, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2028, 2705.825927, 1703.717651, 9002.056640, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(1719, 2707.288818, 1703.691528, 9002.036132, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(1719, 2707.919433, 1703.691528, 9002.036132, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2372, 2708.883056, 1706.532958, 9000.940429, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(912, 2700.837402, 1713.927978, 9001.432617, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(912, 2699.426025, 1713.927978, 9001.432617, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(912, 2697.024902, 1713.927978, 9001.432617, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2474, 2699.267333, 1714.163696, 9002.008789, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2474, 2700.657958, 1714.163696, 9002.008789, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2464, 2696.968994, 1713.934570, 9002.149414, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2464, 2695.762451, 1713.692871, 9001.061523, 0.000000, 0.000000, -43.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2593, 2691.448730, 1710.592285, 9001.792968, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2504, 2691.469238, 1710.977172, 9002.004882, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2504, 2691.759521, 1710.616821, 9002.004882, 0.000000, 0.000000, 450.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2504, 2691.489257, 1710.616821, 9002.004882, 0.000000, 0.000000, 630.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2504, 2691.469238, 1710.656860, 9002.004882, 0.000000, 0.000000, 360.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2504, 2691.469238, 1710.586791, 9002.004882, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2504, 2691.469238, 1710.337036, 9002.004882, 0.000000, 0.000000, 360.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2176, 2708.364257, 1709.841308, 9003.167968, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2394, 2708.660156, 1707.085815, 9001.683593, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(19320, 2708.311523, 1705.713256, 9001.164062, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2819, 2707.757812, 1706.312377, 9000.911132, 0.000000, 0.000000, 7.799999, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(11704, 2707.943603, 1706.254394, 9000.950195, 270.000000, 0.000000, 45.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(912, 2703.974609, 1708.916748, 9001.432617, 0.000000, 0.000000, 360.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(912, 2704.014648, 1708.316162, 9001.432617, 0.000000, 0.000000, 540.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2690, 2711.364990, 1708.651123, 9002.188476, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2961, 2695.728271, 1703.410644, 9002.526367, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(964, 2703.992675, 1708.614624, 9001.067382, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(3015, 2704.239501, 1708.797729, 9002.064453, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(3015, 2704.239501, 1708.337280, 9001.868164, 0.000000, 0.000000, 13.199997, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(3015, 2703.642089, 1708.886840, 9001.721679, 0.000000, 0.000000, 13.199997, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(3015, 2703.713867, 1708.307373, 9002.000000, 0.000000, 0.000000, 13.199997, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2854, 2694.468017, 1705.609375, 9001.923828, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(19993, 2691.377441, 1705.445068, 9001.895507, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(1742, 2706.647705, 1713.478759, 9000.933593, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(1742, 2705.216308, 1713.478759, 9000.933593, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(1742, 2703.784912, 1713.478759, 9000.933593, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2816, 2703.588134, 1712.621337, 9000.933593, 0.000000, 0.000000, 14.100000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2412, 2691.363525, 1714.439331, 9000.933593, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2412, 2691.363525, 1712.667968, 9000.933593, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2643, 2694.344482, 1714.205810, 9002.641601, 0.000000, 13.500000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(1885, 2691.489013, 1711.698486, 9000.933593, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2774, 2696.648437, 1709.183837, 9000.933593, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(912, 2696.655273, 1710.327270, 9001.432617, 0.000000, 0.000000, 360.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(912, 2695.534179, 1709.206542, 9001.432617, 0.000000, 0.000000, 450.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(3675, 2697.465332, 1710.014770, 9004.863281, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2649, 2699.125732, 1709.217407, 9004.986328, 180.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(19356, 2711.733642, 1707.960937, 9000.250000, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	atoysMovingPlate = CreateDynamicObject(19356, 2713.253173, 1709.522338, 9000.250000, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(14705, 2695.517333, 1709.222412, 9002.218750, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(325, 2695.471923, 1709.207031, 9002.345703, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2666, 2695.842285, 1709.172241, 9003.919921, 0.000000, -12.599996, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2114, 2697.015869, 1710.253662, 9002.135742, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2114, 2696.666259, 1710.463867, 9002.135742, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2114, 2696.275878, 1710.283691, 9002.135742, 0.000000, 0.000000, 30.900001, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(3497, 2696.651367, 1710.338989, 9003.679687, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(19449, 2742.798095, 2548.357177, 8367.669921, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2986, 2713.150878, 1708.649169, 9001.442382, 450.000000, 0.000000, 540.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(19449, 2743.428710, 2543.555664, 8367.669921, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(19449, 2748.248779, 2548.357177, 8367.669921, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(19449, 2743.428710, 2553.076416, 8367.669921, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2986, 2713.107177, 1708.279907, 9001.891601, 0.000000, 90.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2986, 2748.134277, 2552.233154, 8366.099609, 0.000000, 90.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(14578, 2746.753906, 2550.552246, 8369.556640, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(927, 2743.705322, 2543.621582, 8367.827148, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(1687, 2745.360839, 2544.305419, 8366.874023, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(1428, 2747.603515, 2544.054199, 8367.509765, 0.000000, 0.000000, 180.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2986, 2747.579345, 2544.716552, 8369.203125, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(19815, 2742.894531, 2547.435791, 8367.602539, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(19922, 2743.649414, 2547.406250, 8366.099609, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(1793, 2743.479003, 2549.474853, 8366.099609, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(938, 2743.851318, 2550.305175, 8369.092773, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(19121, 2743.933349, 2550.516845, 8369.854492, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2676, 2745.012207, 2546.997558, 8366.207031, 0.000000, 0.000000, -7.799999, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(3013, 2743.222412, 2546.195068, 8367.026367, 0.000000, 0.000000, -44.799995, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2949, 2745.183837, 2552.969726, 8366.099609, 0.000000, 0.000000, 450.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(11729, 2743.128662, 2549.503906, 8366.099609, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(2680, 2743.473876, 2549.287353, 8367.150390, 0.000000, 0.000000, 90.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(19088, 2743.677490, 2555.060791, 8366.850585, -32.600009, 90.199974, 93.399948, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(19836, 2744.458496, 2550.735595, 8366.764648, 0.000000, 0.000000, 0.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(19836, 2743.797851, 2551.666015, 8366.764648, 0.000000, 0.000000, 27.100000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	atoysMovingDumpster = CreateDynamicObject(910, 2747.625732, 2551.955078, 8367.333984, 0.000000, 0.000000, 270.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);
	CreateDynamicObject(3675, 2748.258789, 2548.061279, 8364.470703, 0.000000, 0.000000, 270.000000, ATOYS_WORLD, ATOYS_INT, -1, 300.00, 300.00);

	CreateDynamicObject(1688, 575.606201, -1476.777343, 28.030317, 0.000000, 0.000000, 90.000000, -1, -1, -1, 300.00, 300.00);
	CreateDynamicObject(2986, 575.547180, -1474.977783, 27.350317, 0.000000, 90.000000, 270.000000, -1, -1, -1, 300.00, 300.00);
	CreateDynamicObject(671, 593.085327, -1611.371704, 15.248072, 0.000000, 0.000000, 0.000000, -1, -1, -1, 300.00, 300.00);
	CreateDynamicObject(647, 591.055175, -1614.716796, 16.514669, 0.000000, 0.000000, 0.000000, -1, -1, -1, 300.00, 300.00);
	CreateDynamicObject(647, 587.615722, -1612.991577, 16.514669, 0.000000, 0.000000, -32.999996, -1, -1, -1, 300.00, 300.00);
	return 1;
}
