/*
 ____                                                   __     
/\  _`\                                                /\ \    
\ \ \/\ \     __    ____    ___ ___     ___     ___    \_\ \   
 \ \ \ \ \  /'__`\ /',__\ /' __` __`\  / __`\ /' _ `\  /'_` \  
  \ \ \_\ \/\  __//\__, `\/\ \/\ \/\ \/\ \L\ \/\ \/\ \/\ \L\ \ 
   \ \____/\ \____\/\____/\ \_\ \_\ \_\ \____/\ \_\ \_\ \___,_\
    \/___/  \/____/\/___/  \/_/\/_/\/_/\/___/  \/_/\/_/\/__,_ /
                                                               
                                                               
 ____            ____                     __                     
/\  _`\         /\  _`\                  /\ \__                  
\ \ \/\ \     __\ \,\L\_\     __      ___\ \ ,_\   ___     ____  
 \ \ \ \ \  /'__`\/_\__ \   /'__`\  /' _ `\ \ \/  / __`\  /',__\ 
  \ \ \_\ \/\  __/ /\ \L\ \/\ \L\.\_/\ \/\ \ \ \_/\ \L\ \/\__, `\
   \ \____/\ \____\\ `\____\ \__/.\_\ \_\ \_\ \__\ \____/\/\____/
    \/___/  \/____/ \/_____/\/__/\/_/\/_/\/_/\/__/\/___/  \/___/ 
	
	
	http://www.gta-sarp.com/forums/showthread.php?193638-Development-Request-Desmond-DeSantos-s-Request
*/

enum DesmondDrinkData {
	DesmondDrinkName[7],
	DesmondDrinkAction
};

enum {
	DESMOND_SAFE_MONEY,
	DESMOND_SAFE_POT,
	DESMOND_SAFE_CRACK
};

new DesmondDrinks[4][DesmondDrinkData] = {
	{"Wine",	SPECIAL_ACTION_DRINK_WINE},
	{"Beer",	SPECIAL_ACTION_DRINK_BEER},
	{"Whisky",	SPECIAL_ACTION_DRINK_BEER},
	{"Sprunk",	SPECIAL_ACTION_DRINK_SPRUNK}
};

new Float: DesmondCameras[5][6] = {
	{1135.598266, -13.055255, 1002.148559, 1132.390502, -16.832738, 1001.484375},
	{1115.694335, -11.405779, 1004.237731, 1119.960449, -8.962922, 1003.325195},
	{1135.258911, 9.924472, 1004.576416, 1130.951049, 7.988495, 1002.935058},
	{1113.584350, -9.115205, 1003.608276, 1118.416503, -9.451298, 1002.368225},
	{1133.872924, 6.324785, 1003.962951, 1137.623046, 9.605833, 1003.548828}
};

new DesmondPrices[4];
new DesmondPassword[64];
new DesmondSafe[4];
new DesmondSafeWeapons[10];
new Text3D: DesmondEntrance;
new bool: DesmondLocked, bool: DesmondOfficeLocked;
new DesmondEntryFee;
new bool: DesmondLoggedIn[MAX_PLAYERS];
new DesmondArea;
new DesmondURL[128];

stock LoadDesmond() {
	if(!dini_Exists("DesmondCasino.cfg")) dini_Create("DesmondCasino.cfg");
	
	DesmondPrices[0] = dini_Int("DesmondCasino.cfg", "DesmondPrice1");
	DesmondPrices[1] = dini_Int("DesmondCasino.cfg", "DesmondPrice2");
	DesmondPrices[2] = dini_Int("DesmondCasino.cfg", "DesmondPrice3");
	DesmondPrices[3] = dini_Int("DesmondCasino.cfg", "DesmondPrice4");
	
	DesmondSafe[DESMOND_SAFE_MONEY]  = dini_Int("DesmondCasino.cfg", "DesmondSafeMoney");
	DesmondSafeWeapons[0]            = dini_Int("DesmondCasino.cfg", "DesmondSafeWeapon1");
	DesmondSafeWeapons[1]            = dini_Int("DesmondCasino.cfg", "DesmondSafeWeapon2");
	DesmondSafeWeapons[2]            = dini_Int("DesmondCasino.cfg", "DesmondSafeWeapon3");
	DesmondSafeWeapons[3]            = dini_Int("DesmondCasino.cfg", "DesmondSafeWeapon4");
	DesmondSafeWeapons[4]            = dini_Int("DesmondCasino.cfg", "DesmondSafeWeapon5");
	DesmondSafeWeapons[5]            = dini_Int("DesmondCasino.cfg", "DesmondSafeWeapon6");
	DesmondSafeWeapons[6]            = dini_Int("DesmondCasino.cfg", "DesmondSafeWeapon7");
	DesmondSafeWeapons[7]            = dini_Int("DesmondCasino.cfg", "DesmondSafeWeapon8");
	DesmondSafeWeapons[8]            = dini_Int("DesmondCasino.cfg", "DesmondSafeWeapon9");
	DesmondSafeWeapons[9]            = dini_Int("DesmondCasino.cfg", "DesmondSafeWeapon10");
	DesmondSafe[DESMOND_SAFE_POT]    = dini_Int("DesmondCasino.cfg", "DesmondSafePot");
	DesmondSafe[DESMOND_SAFE_CRACK]  = dini_Int("DesmondCasino.cfg", "DesmondSafeCrack");
	
	DesmondLocked = (dini_Int("DesmondCasino.cfg", "DesmondLocked")) ? (true) : (false);
	DesmondEntryFee = dini_Int("DesmondCasino.cfg", "DesmondEntryFee");
	strins(DesmondPassword, dini_Get("DesmondCasino.cfg", "DesmondPassword"), 0, 65);
	strins(DesmondURL, dini_Get("DesmondCasino.cfg", "DesmondURL"), 0, 128);
	
	if(DesmondLocked) {
	    DesmondEntrance = CreateDynamic3DTextLabel("DeSantos Casino\n{FF0000}CLOSED", COLOR_LIGHTBLUE, 1985.71, -1999.52, 13.54, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
	}
	else {
	    format(donorstr, sizeof donorstr, "DeSantos Casino\n{00FF00}OPEN\n{FFFFFF}Entry Fee: $%i", DesmondEntryFee);
	    DesmondEntrance = CreateDynamic3DTextLabel(donorstr, COLOR_LIGHTBLUE, 1985.71, -1999.52, 13.54, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
	}
	return 1;
}

SaveDesmond(updatetoken=0) {
	if(updatetoken == 0) {
	    // Save everything
  		dini_IntSet("DesmondCasino.cfg", "DesmondPrice1", DesmondPrices[0]);
		dini_IntSet("DesmondCasino.cfg", "DesmondPrice2", DesmondPrices[1]);
		dini_IntSet("DesmondCasino.cfg", "DesmondPrice3", DesmondPrices[2]);
		dini_IntSet("DesmondCasino.cfg", "DesmondPrice4", DesmondPrices[3]);
		
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeMoney", DesmondSafe[DESMOND_SAFE_MONEY]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon1", DesmondSafeWeapons[0]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon2", DesmondSafeWeapons[1]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon3", DesmondSafeWeapons[2]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon4", DesmondSafeWeapons[3]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon5", DesmondSafeWeapons[4]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon6", DesmondSafeWeapons[5]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon7", DesmondSafeWeapons[6]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon8", DesmondSafeWeapons[7]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon9", DesmondSafeWeapons[8]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon10", DesmondSafeWeapons[9]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafePot", DesmondSafe[DESMOND_SAFE_POT]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeCrack", DesmondSafe[DESMOND_SAFE_CRACK]);
		
		dini_IntSet("DesmondCasino.cfg", "DesmondLocked", (DesmondLocked == true) ? (1) : (0));
		dini_IntSet("DesmondCasino.cfg", "DesmondOfficeLocked", (DesmondOfficeLocked == true) ? (1) : (0));
	    dini_IntSet("DesmondCasino.cfg", "DesmondEntryFee", DesmondEntryFee);
	    
		dini_Set("DesmondCasino.cfg", "DesmondPassword", DesmondPassword);
		dini_Set("DesmondCasino.cfg", "DesmondURL", DesmondURL);
	}
	else if(updatetoken == 1) {
	    // Save prices
	    dini_IntSet("DesmondCasino.cfg", "DesmondPrice1", DesmondPrices[0]);
		dini_IntSet("DesmondCasino.cfg", "DesmondPrice2", DesmondPrices[1]);
		dini_IntSet("DesmondCasino.cfg", "DesmondPrice3", DesmondPrices[2]);
		dini_IntSet("DesmondCasino.cfg", "DesmondPrice4", DesmondPrices[3]);
	}
	else if(updatetoken == 2) {
	    // Save safe
	    dini_IntSet("DesmondCasino.cfg", "DesmondSafeMoney", DesmondSafe[DESMOND_SAFE_MONEY]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon1", DesmondSafeWeapons[0]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon2", DesmondSafeWeapons[1]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon3", DesmondSafeWeapons[2]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon4", DesmondSafeWeapons[3]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon5", DesmondSafeWeapons[4]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon6", DesmondSafeWeapons[5]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon7", DesmondSafeWeapons[6]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon8", DesmondSafeWeapons[7]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon9", DesmondSafeWeapons[8]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon10", DesmondSafeWeapons[9]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafePot", DesmondSafe[DESMOND_SAFE_POT]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeCrack", DesmondSafe[DESMOND_SAFE_CRACK]);
	}
	else if(updatetoken == 3) {
	    // Save password
	    dini_Set("DesmondCasino.cfg", "DesmondPassword", DesmondPassword);
	}
	else if(updatetoken == 4) {
	    dini_IntSet("DesmondCasino.cfg", "DesmondLocked", (DesmondLocked == true) ? (1) : (0));
		dini_IntSet("DesmondCasino.cfg", "DesmondOfficeLocked", (DesmondOfficeLocked == true) ? (1) : (0));
	    dini_IntSet("DesmondCasino.cfg", "DesmondEntryFee", DesmondEntryFee);
	}
	else if(updatetoken == 5) {
	    dini_Set("DesmondCasino.cfg", "DesmondURL", DesmondURL);
	}
	return 1;
}

Hook:DesmondDeSantos_OnGameModeInit() {
	LoadDesmond();
	CreateDynamic3DTextLabel("DeSantos Casino Bar\n((/dsdrink))", COLOR_LIGHTBLUE, 1139.47, -5.51, 1000.67, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 23, 12);
	CreateDynamic3DTextLabel("Sprunk Machine((/dssoda))", COLOR_LIGHTBLUE, 1982, -1998.73, 13.54, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
	DesmondArea = CreateDynamicCircle(1130.80, -3.11, 130.0, 23, 12);
	
    CreateDynamicObject(955, 1983.02686, -1999.80713, 12.92352,   0.00000, 0.00000, 180.00000); // Sprunk machine
    CreateDynamicObject(2942, 1124.53015, 4.01472, 1000.14783,   0.00000, 0.00000, 0.00000);
	return 1;
}

Hook:DesmondDeSantos_OnPlayerConnect(playerid) {
	DesmondLoggedIn[playerid] = false;
	SetPVarInt(playerid, "DesmondCameraWatching", -1);
	return 1;
}

Hook:DesmondDeSantos_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) {
	if(dialogid == DIALOG_DONOR_DESMOND) {
	    if(!response) return SendClientMessage(playerid, COLOR_GREY, "You have closed the drink menu.");

	    if(GetPVarInt(playerid, "DesmondEditing") == 1) {
	        SetPVarInt(playerid, "DesmondEditDrink", listitem);

			format(donorstr, sizeof donorstr, "You are now editing %s. The price of this drink is currently $%i.", DesmondDrinks[listitem][DesmondDrinkName], DesmondPrices[listitem]);
			ShowPlayerDialog(playerid, DIALOG_DONOR_DESMOND_EDIT, DIALOG_STYLE_INPUT, "Casino Drink - Edit Price", donorstr, "Edit", "Close");
		}
	    else if(listitem >= 0 && listitem <= sizeof DesmondDrinks) {
		    if(DesmondPrices[listitem] <= 0) {
		        format(donorstr, sizeof donorstr, "%s is not currently available.", DesmondDrinks[listitem][DesmondDrinkAction]);
		        SendClientMessage(playerid, COLOR_GREY, donorstr);
			}
	        else {
				SetPlayerSpecialAction(playerid, DesmondDrinks[listitem][DesmondDrinkAction]);
				GiveMoney(playerid, -DesmondPrices[listitem]);
	        	DesmondSafe[DESMOND_SAFE_MONEY] += DesmondPrices[listitem];
	        	SaveDesmond(2); // save the safe
	        
	        	format(donorstr, sizeof donorstr, "You have purchased a %s for $%i.", DesmondDrinks[listitem][DesmondDrinkName], DesmondPrices[listitem]);
	        	SendClientMessage(playerid, COLOR_LIGHTBLUE, donorstr);
			}
		}
	}
	else if(dialogid == DIALOG_DONOR_DESMOND_EDIT) {
		listitem = GetPVarInt(playerid, "DesmondEditDrink");
		if(strval(inputtext) < 0 || strval(inputtext) >= 1000000) {
			format(donorstr, sizeof donorstr, "You are now editing %s. The price of this drink is currently $%i.", DesmondDrinks[listitem][DesmondDrinkName], DesmondPrices[listitem]);
			ShowPlayerDialog(playerid, DIALOG_DONOR_DESMOND_EDIT, DIALOG_STYLE_INPUT, "Casino Drink - Edit Price", donorstr, "Edit", "Close");
		}
		else {
			DesmondPrices[listitem] = strval(inputtext);
			format(donorstr, sizeof donorstr, "You have changed the price of %s to $%i.", DesmondDrinks[listitem][DesmondDrinkName], strval(inputtext));
			SendClientMessage(playerid, COLOR_WHITE, donorstr);
			SaveDesmond(1); // save the prices
			
			DeletePVar(playerid, "DesmondEditing");
			DeletePVar(playerid, "DesmondEditDrink");
		}
	}
	return 1;
}

Hook:DesmondDeSantos_OnPlayerLeaveDynamicArea(playerid, areaid) {
	if(areaid == DesmondArea) {
	    if(GetPVarInt(playerid, "DesmondStream") == 1)
			StopAudioStreamForPlayer(playerid);
	}
	return 1;
}

CMD:dssoda(playerid, params[]) {
	if(IsPlayerInRangeOfPoint(playerid, 3.0, 1983.1195, -1999.4825, 13.5469) && GetPlayerVirtualWorld(playerid) == 0) {
	    if(PlayerInfo[playerid][pCash] < 5) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough money to purchase a sprunk can.");
		if(PlayerTazed[playerid] || PlayerCuffed[playerid] || PlayerFrozen[playerid]) return SendClientMessage(playerid, COLOR_GREY, "You cannot do this right now.");
		
		GiveMoney(playerid, -5);
		DesmondSafe[DESMOND_SAFE_MONEY] += 5;
		
		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);
		SendClientMessage(playerid, COLOR_WHITE, "You have purchased a sprunk for $5.");
	}
	
	else SendClientMessage(playerid, COLOR_GREY, "You are not near the sprunk machine.");
	return 1;
}

CMD:dsdrink(playerid, params[]) {
	if(PlayerTazed[playerid] || PlayerCuffed[playerid] || PlayerFrozen[playerid]) return SendClientMessage(playerid, COLOR_GREY, "You can't do this right now.");
	if(IsPlayerInRangeOfPoint(playerid, 3.0, 1139.47, -5.51, 1000.67) && GetPlayerVirtualWorld(playerid) == 23) {
		format(donorstr, sizeof donorstr, "%s($%i)\n%s($%i)\n%s($%i)\n%s($%i)", DesmondDrinks[0][DesmondDrinkName], DesmondPrices[0], DesmondDrinks[1][DesmondDrinkName], DesmondPrices[1],
		DesmondDrinks[2][DesmondDrinkName], DesmondPrices[2], DesmondDrinks[3][DesmondDrinkName], DesmondPrices[3]);
		ShowPlayerDialog(playerid, DIALOG_DONOR_DESMOND, DIALOG_STYLE_LIST, "Casino Drinks", donorstr, "Buy", "Close");
		
		if(GetPVarInt(playerid, "DesmondEditing") != 0) {
		    DeletePVar(playerid, "DesmondEditing");
		    DeletePVar(playerid, "DesmondEditDrink");
		}
	}
	
	else SendClientMessage(playerid, COLOR_GREY, "You are not near the bar!");
	return 1;
}

CMD:dscasino(playerid, params[]) {
	if(CP[playerid] != 0 && CP[playerid] != CHECKPOINT_DESMOND) return SendClientMessage(playerid, COLOR_GREY, "You already have an active checkpoint, /killcp.");

	DisablePlayerCheckpointEx(playerid);
	SetPlayerCheckpointEx(playerid, 1985.85, -1999.56, 13.54, 3.0);
	CP[playerid] = CHECKPOINT_DESMOND;
	
	SendClientMessage(playerid, COLOR_GREY, "You have set a checkpoint to the casino.");
	return 1;
}

CMD:dslogin(playerid, params[]) {
	if(PlayerInfo[playerid][pID] != 59148 && isnull(params)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dslogin [password]");
	if(PlayerInfo[playerid][pID] == 59148 || strmatch(params, DesmondPassword) || PlayerInfo[playerid][pAdmin] == 99999) {
	    if(DesmondLoggedIn[playerid] == true) return SendClientMessage(playerid, COLOR_GREY, "You are already logged in.");
	    DesmondLoggedIn[playerid] = true;
		SendClientMessage(playerid, COLOR_YELLOW, "You have logged in to the casino system.");
	}
	else SendClientMessage(playerid, -1, "Invalid password.");
	return 1;
}

CMD:dslogout(playerid, params[]) {
	if(DesmondLoggedIn[playerid] == true) {
	    DesmondLoggedIn[playerid] = false;
	    SendClientMessage(playerid, COLOR_YELLOW, "You have logged out of the casino system.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not allowed to use this command.");
}

CMD:dschangepass(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 59148 || DesmondLoggedIn[playerid] == true) {
		if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dschangepass [new password]");
		if(strlen(params) >= 2 && strlen(params) <= 64) {
		    strdel(DesmondPassword, 0, strlen(DesmondPassword));
		    strins(DesmondPassword, params, 0, strlen(params));

			format(donorstr, sizeof donorstr, "You have changed the casino password to: %s", params);
			SendClientMessage(playerid, COLOR_WHITE, donorstr);
			
			SaveDesmond(3); // save the password only
		}
		else SendClientMessage(playerid, COLOR_GREY, "Password length must be more than 2 and below 65.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not allowed to use this.");
	return 1;
}

CMD:dsofficelock(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 59148 || DesmondLoggedIn[playerid] == true) {
	    if(IsPlayerInRangeOfPoint(playerid, 3.5, 1143.2092, 1.6622, 1000.6797) || IsPlayerInRangeOfPoint(playerid, 3.5, 1153.4292, -2.7271, 997.6765)) {
			if(DesmondOfficeLocked == true) {
			    DesmondOfficeLocked = false;
			    SendClientMessage(playerid, COLOR_GREY, "You have unlocked the casino office.");
			}
			else {
			    DesmondOfficeLocked = true;
			    SendClientMessage(playerid, COLOR_GREY, "You have locked the casino office.");
			}
			
			SaveDesmond(4);
		}
		else SendClientMessage(playerid, COLOR_GREY, "You are not near the office.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You do not have the key.");
	return 1;
}

CMD:dslock(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 59148 || DesmondLoggedIn[playerid] == true) {
	    if(DesmondLocked == true) {
	        DesmondLocked = false;
			format(donorstr, sizeof donorstr, "DeSantos Casino\n{00FF00}OPEN\n{FFFFFF}Entry Fee: $%i", DesmondEntryFee);
            UpdateDynamic3DTextLabelText(DesmondEntrance, COLOR_LIGHTBLUE, donorstr);
		}
		else {
		    DesmondLocked = true;
            UpdateDynamic3DTextLabelText(DesmondEntrance, COLOR_LIGHTBLUE, "DeSantos Casino\n{FF0000}CLOSED");
		}
		
		SaveDesmond(4);
	}
	else SendClientMessage(playerid, COLOR_GREY, "You do not have the key!");
	return 1;
}

CMD:dsprices(playerid, params[]) {
    if(PlayerInfo[playerid][pID] == 59148 || DesmondLoggedIn[playerid] == true) {
	    format(donorstr, sizeof donorstr, "%s($%i)\n%s($%i)\n%s($%i)\n%s($%i)", DesmondDrinks[0][DesmondDrinkName], DesmondPrices[0], DesmondDrinks[1][DesmondDrinkName], DesmondPrices[1],
		DesmondDrinks[2][DesmondDrinkName], DesmondPrices[2], DesmondDrinks[3][DesmondDrinkName], DesmondPrices[3]);
		ShowPlayerDialog(playerid, DIALOG_DONOR_DESMOND, DIALOG_STYLE_LIST, "Casino Drinks - Edit", donorstr, "Edit", "Close");

		SetPVarInt(playerid, "DesmondEditing", 1);
	}
	else SendClientMessage(playerid, COLOR_GREY, "You do not have the key.");
	return 1;
}

CMD:dsbroadcast(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 59148 || DesmondLoggedIn[playerid] == true) {
		if(!IsPlayerInDynamicArea(playerid, DesmondArea)) return SendClientMessage(playerid, COLOR_GREY, "You are not in the Casino.");
	    if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dsbroadcast [text]");
	    if(strlen(params) >= 2 || strlen(params) <= 100) {
	        format(donorstr, sizeof donorstr, "[Microphone - %s]: %s", PlayerICName(playerid), params);
			ProxDetector(30.0, playerid, donorstr, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW);
		}
		else SendClientMessage(playerid, COLOR_GREY, "The text length cannot be less than 2 or more than 100.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You do not have the key.");
	return 1;
}

CMD:dsentryfee(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 59148 || DesmondLoggedIn[playerid] == true) {
		if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dsentryfee [fee amount]");
		if(strval(params) <= 0 || strval(params) >= 100000) return SendClientMessage(playerid, COLOR_GREY, "Invalid fee amount.");
		
		DesmondEntryFee = strval(params);
		SaveDesmond(4);
		if(DesmondLocked == false) {
		    format(donorstr, sizeof donorstr, "DeSantos Casino\n{00FF00}OPEN\n{FFFFFF}Entry Fee: $%i", DesmondEntryFee);
            UpdateDynamic3DTextLabelText(DesmondEntrance, COLOR_WHITE, donorstr);
		}

		// We don't need to update it if it's closed because the entry fee doesn't show anyways.
		format(donorstr, sizeof donorstr, "You have changed the casino update fee to $%i.", DesmondEntryFee);
		SendClientMessage(playerid, COLOR_YELLOW, donorstr);
	}
	else SendClientMessage(playerid, COLOR_GREY, "You do not have the key.");
	return 1;
}

CMD:dssafe(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 59148 || DesmondLoggedIn[playerid] == true) {
	    if(GetPlayerVirtualWorld(playerid) != 23 || !IsPlayerInRangeOfPoint(playerid, 5.0, 1159.3403, 5.5914, 996.8517)) return SendClientMessage(playerid, COLOR_GREY, "You are not near the safe.");
	    if(IsAtEvent[playerid] || inPaintball[playerid] || PlayerTazed[playerid] || PlayerCuffed[playerid]) return SendClientMessage(playerid, COLOR_GREY, "You cannot do this right now.");
		new option[10], option2[12], amount;
		if(sscanf(params, "s[10]s[12]D", option, option2, amount)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe [take/put] [money/weapon/crack/pot]");
		
		if(strmatch(option2, "weapon") == 1) {
		    if(strmatch(option, "put")) {
		        if(sscanf(params, "s[120]s[12]d", option, option2, amount)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe [put] [weapon] [slot]");
		        if(amount < 0 || amount > 9) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe put weapon [0-9]");
		        if(DesmondSafeWeapons[amount] != 0) return SendClientMessage(playerid, COLOR_GREY, "You already have a weapon stored.");
		        if(GetPlayerWeapon(playerid) == 0) return SendClientMessage(playerid, COLOR_GREY, "You are not holding a weapon.");
		        if(HaveAdminWeapon(playerid, GetPlayerWeapon(playerid))  == GetPlayerWeapon(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can not store admin weapons.");

		        TakePlayerWeapon(playerid, GetPlayerWeapon(playerid));
				DesmondSafeWeapons[amount] = GetPlayerWeapon(playerid);

    			format(donorstr, sizeof donorstr, "* %s places a %s in the safe.", PlayerICName(playerid), dGetWeaponName(GetPlayerWeapon(playerid)));
				ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			
			else if(strmatch(option, "take")) {
                if(sscanf(params, "s[120]s[12]d", option, option2, amount)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe [take] [weapon] [slot]");
		        if(amount < 0 || amount > 9) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe take weapon [0-9]");
			    if(PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GREY, "You must play for 2 hours before using a weapon.");
			    if(DesmondSafeWeapons[amount] == 0) return SendClientMessage(playerid, COLOR_GREY, "You do not have a weapon stored in the safe.");

                format(donorstr, sizeof donorstr, "* %s takes a %s from the safe.", PlayerICName(playerid), dGetWeaponName(DesmondSafeWeapons[amount]));
				ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				
				GivePlayerGun(playerid, DesmondSafeWeapons[amount]);
				DesmondSafeWeapons[amount] = 0;
			}
		}
		
		else if(strmatch(option2, "money") == 1) {
		    if(strmatch(option, "put")) {
    			if(sscanf(params, "s[10]s[12]d", option, option2, amount)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe put money [amount]");
				if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
				if(PlayerInfo[playerid][pCash] < amount) return SendClientMessage(playerid, COLOR_GREY, "You do not have that much cash on you.");
				
				GiveMoney(playerid, -amount);
				DesmondSafe[DESMOND_SAFE_MONEY] += amount;
				
				format(donorstr, sizeof donorstr, "* %s puts some money in the safe.", PlayerICName(playerid));
				ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			
			else if(strmatch(option, "take")) {
			    if(sscanf(params, "s[10]s[12]d", option, option2, amount)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe take money [amount]");
		        if(DesmondSafe[DESMOND_SAFE_MONEY] < amount) return SendClientMessage(playerid, COLOR_GREY, "You do not have that much in your safe.");
		        if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
		        
				GiveMoney(playerid, amount);
				DesmondSafe[DESMOND_SAFE_MONEY] -= amount;
				
				format(donorstr, sizeof donorstr, "* %s takes some money from the safe.", PlayerICName(playerid));
				ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
		}
		
		else if(strmatch(option2, "pot") == 1) {
		    if(strmatch(option, "put")) {
		        if(sscanf(params, "s[10]s[12]d", option, option2, amount)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe put pot [amount]");
				if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
				if(PlayerInfo[playerid][pPot] < amount) return SendClientMessage(playerid, COLOR_GREY, "You do not have that much pot.");
				
				if(PlayerInfo[playerid][pPot]+DesmondSafe[DESMOND_SAFE_POT] >= 10000) return SendClientMessage(playerid, COLOR_GREY, "Your safe cannot store that much pot.");
				PlayerInfo[playerid][pPot] -= amount;
				DesmondSafe[DESMOND_SAFE_POT] += amount;
				
				format(donorstr, sizeof donorstr, "* %s puts some pot in the safe.", PlayerICName(playerid));
				ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			
			else if(strmatch(option, "take")) {
			    if(sscanf(params, "s[10]s[12]d", option, option2, amount)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe take pot [amount]");
				if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
				if(DesmondSafe[DESMOND_SAFE_POT] < amount) return SendClientMessage(playerid, COLOR_GREY, "Your safe doesn't have that much pot stored.");
				
				PlayerInfo[playerid][pPot] += amount;
				DesmondSafe[DESMOND_SAFE_POT] -= amount;
				
				format(donorstr, sizeof donorstr, "* %s takes some pot from the safe.", PlayerICName(playerid));
				ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
		}
		
        else if(strmatch(option2, "crack") == 1) {
		    if(strmatch(option, "put")) {
		        if(sscanf(params, "s[10]s[12]d", option, option2, amount)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe put crack [amount]");
				if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
				if(PlayerInfo[playerid][pCrack] < amount) return SendClientMessage(playerid, COLOR_GREY, "You do not have that much crack.");

				if(PlayerInfo[playerid][pCrack]+DesmondSafe[DESMOND_SAFE_CRACK] >= 10000) return SendClientMessage(playerid, COLOR_GREY, "Your safe cannot store that much crack.");
				PlayerInfo[playerid][pCrack] -= amount;
				DesmondSafe[DESMOND_SAFE_CRACK] += amount;

				format(donorstr, sizeof donorstr, "* %s puts some crack in the safe.", PlayerICName(playerid));
				ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}

			else if(strmatch(option, "take")) {
			    if(sscanf(params, "s[10]s[12]d", option, option2, amount)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe take crack [amount]");
				if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
				if(DesmondSafe[DESMOND_SAFE_CRACK] < amount) return SendClientMessage(playerid, COLOR_GREY, "Your safe doesn't have that much crack stored.");

				PlayerInfo[playerid][pCrack] += amount;
				DesmondSafe[DESMOND_SAFE_CRACK] -= amount;

				format(donorstr, sizeof donorstr, "* %s takes some crack in the safe.", PlayerICName(playerid));
				ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
		}
		else return SendClientMessage(playerid, COLOR_GREY, "Invalid safe option.");
		SaveDesmond(2);
	}
	else SendClientMessage(playerid, COLOR_GREY, "You do not have the key.");
	return 1;
}

CMD:dshelp(playerid, params[]) {
    SendClientMessage(playerid, 0xFFFFFFFF, "_______________DeSantos Casino Commands_____________");
    SendClientMessage(playerid, 0xFFFFFFFF, "/dscasino  /dslogin  /dslogout  /dschangepass  /dsofficelock");
    SendClientMessage(playerid, 0xFFFFFFFF, "/dslock  /dsprices  /dsbroadcast /dsentryfee /dsmusic");
    SendClientMessage(playerid, 0xFFFFFFFF, "/dssafe /dssafecheck /dscamera");
    return 1;
}

CMD:dssafecheck(playerid, params[]) {
    if(PlayerInfo[playerid][pID] == 59148 || DesmondLoggedIn[playerid] == true) {
	    if(GetPlayerVirtualWorld(playerid) == 23 && IsPlayerInRangeOfPoint(playerid, 5.0, 1159.3403, 5.5914, 996.8517)) {
		    format(donorstr, sizeof donorstr, "Weapon 1: %s Weapon 2: %s Weapon 3: %s", dGetWeaponName(DesmondSafeWeapons[0]), dGetWeaponName(DesmondSafeWeapons[1]), dGetWeaponName(DesmondSafeWeapons[2]));
			SendClientMessage(playerid, COLOR_YELLOW, donorstr);
			
			format(donorstr, sizeof donorstr, "Weapon 4: %s Weapon 5: %s Weapon 6: %s", dGetWeaponName(DesmondSafeWeapons[3]), dGetWeaponName(DesmondSafeWeapons[4]), dGetWeaponName(DesmondSafeWeapons[5]));
			SendClientMessage(playerid, COLOR_YELLOW, donorstr);
			
			format(donorstr, sizeof donorstr, "Weapon 7: %s Weapon 8: %s Weapon 9: %s Weapon 10: %s", dGetWeaponName(DesmondSafeWeapons[6]), dGetWeaponName(DesmondSafeWeapons[7]), dGetWeaponName(DesmondSafeWeapons[8]), dGetWeaponName(DesmondSafeWeapons[9]));
			SendClientMessage(playerid, COLOR_YELLOW, donorstr);
			
			format(donorstr, sizeof donorstr, "Money: %i", DesmondSafe[DESMOND_SAFE_MONEY]);
			SendClientMessage(playerid, COLOR_YELLOW, donorstr);

			format(donorstr, sizeof donorstr, "Pot: %i", DesmondSafe[DESMOND_SAFE_POT]);
			SendClientMessage(playerid, COLOR_YELLOW, donorstr);

			format(donorstr, sizeof donorstr, "Crack: %i", DesmondSafe[DESMOND_SAFE_CRACK]);
			SendClientMessage(playerid, COLOR_YELLOW, donorstr);
		}
		else SendClientMessage(playerid, COLOR_GREY, "You are not near the safe.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You do not have the key.");
	return 1;
}

CMD:dscamera(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 59148 || DesmondLoggedIn[playerid] == true) {
	    if(GetPlayerVirtualWorld(playerid) != 23 || !IsPlayerInRangeOfPoint(playerid, 5.0, 1157.6342, 4.1783, 996.5691)) return SendClientMessage(playerid, COLOR_GREY, "You are not in the office.");
		if(isnull(params)) {
		    format(donorstr, sizeof donorstr, "/dscamera [1-%d]", sizeof DesmondCameras);
		    SendClientMessage(playerid, COLOR_GREY, donorstr);
		    
		    SendClientMessage(playerid, COLOR_GREY, "You may additionally turn off the camera using off.");
		}
		else {
			if(strmatch(params, "off")) {
			    if(GetPVarInt(playerid, "DesmondCameraWatching") != -1) {
			        SetCameraBehindPlayer(playerid);
			        SendClientMessage(playerid, COLOR_WHITE, "You have stopped watching the camera.");
			        SetPVarInt(playerid, "DesmondCameraWatching", -1);

			        TogglePlayerControllable(playerid, 1);
			        PlayerFrozen[playerid] = false;
				}
				else SendClientMessage(playerid, COLOR_GREY, "You are not watching the camera.");
				return 1;
			}

			new iCamera = strval(params);
			if(iCamera >= 1 && iCamera <= sizeof DesmondCameras) {
			    iCamera--;
				format(donorstr, sizeof donorstr, "You are now watching camera %d.", iCamera+1);
				SendClientMessage(playerid, COLOR_WHITE, donorstr);
				
				SetPlayerCameraPos(playerid, DesmondCameras[iCamera][0], DesmondCameras[iCamera][1], DesmondCameras[iCamera][2]);
				SetPlayerCameraLookAt(playerid, DesmondCameras[iCamera][3], DesmondCameras[iCamera][4], DesmondCameras[iCamera][5]);

				TogglePlayerControllable(playerid, 0);
				PlayerFrozen[playerid] = true;
				SetPVarInt(playerid, "DesmondCameraWatching", iCamera);
			}
			else SendClientMessage(playerid, COLOR_GREY, "Invalid camera.");
		}
	}
	else SendClientMessage(playerid, COLOR_GREY, "You do not have the key.");
	return 1;
}
