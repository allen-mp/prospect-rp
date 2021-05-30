/*
 ____    __                               
/\  _`\ /\ \                              
\ \,\L\_\ \ \/'\      __     _ __   _ __  
 \/_\__ \\ \ , <    /'__`\  /\`'__\/\`'__\
   /\ \L\ \ \ \\`\ /\ \L\.\_\ \ \/ \ \ \/ 
   \ `\____\ \_\ \_\ \__/.\_\\ \_\  \ \_\ 
    \/_____/\/_/\/_/\/__/\/_/ \/_/   \/_/ 
                                          
                                          
 ______                                     
/\__  _\               __                   
\/_/\ \/   ___   _ __ /\_\    ___     ___   
   \ \ \  / __`\/\`'__\/\ \ /' _ `\  / __`\ 
    \ \ \/\ \L\ \ \ \/ \ \ \/\ \/\ \/\ \L\ \
     \ \_\ \____/\ \_\  \ \_\ \_\ \_\ \____/
      \/_/\/___/  \/_/   \/_/\/_/\/_/\/___/ 

	  
	http://www.gta-sarp.com/forums/showthread.php?182696-Development-Request-Skarr-Torino-s-Request
*/
	
new SquadPassword[128];
new SquadAdmPassword[128];
new bool: SquadLogged[MAX_PLAYERS];
new bool: SquadAdmLogged[MAX_PLAYERS];
new SquadSafe[3];
new SquadArmory[13][2];
new bool: SquadElevatorState;
new bool: SquadElevatorMoving;
new bool: SquadLockdown = false;
new SquadElevator;
new SquadDoor;
new bool: SquadDoorState;
new bool: SquadDoorMoving;
new bool: SquadCamera[MAX_PLAYERS];
new SquadElevatorDoor1;
new SquadElevatorDoor2;
new SquadElevatorDoor3
new SquadElevatorDoor4;

enum {
	SQUAD_SAFE_CRACK,
	SQUAD_SAFE_POT,
	SQUAD_SAFE_MONEY
}

LoadSquad() {
	if(!dini_Exists("squad.cfg")) dini_Create("squad.cfg");

	strins(SquadPassword, dini_Get("squad.cfg", "SquadPassword"), 0, strlen(dini_Get("squad.cfg", "SquadPassword")));
	strins(SquadAdmPassword, dini_Get("squad.cfg", "SquadAdmPassword"), 0, strlen(dini_Get("squad.cfg", "SquadAdmPassword")));

	SquadSafe[SQUAD_SAFE_CRACK] = dini_Int("squad.cfg", "SquadSafeCrack");
	SquadSafe[SQUAD_SAFE_POT]   = dini_Int("squad.cfg", "SquadSafePot");
	SquadSafe[SQUAD_SAFE_MONEY] = dini_Int("squad.cfg", "SquadSafeMoney");
	
	SquadArmory[0][0] = dini_Int("squad.cfg", "SquadArmoryWeapon1");
	SquadArmory[0][1] = dini_Int("squad.cfg", "SquadArmoryWeapon1Ammo");
	SquadArmory[1][0] = dini_Int("squad.cfg", "SquadArmoryWeapon2");
	SquadArmory[1][1] = dini_Int("squad.cfg", "SquadArmoryWeapon2Ammo");
	SquadArmory[2][0] = dini_Int("squad.cfg", "SquadArmoryWeapon3");
	SquadArmory[2][1] = dini_Int("squad.cfg", "SquadArmoryWeapon4Ammo");
	SquadArmory[3][0] = dini_Int("squad.cfg", "SquadArmoryWeapon4");
	SquadArmory[3][1] = dini_Int("squad.cfg", "SquadArmoryWeapon4Ammo");
	SquadArmory[4][0] = dini_Int("squad.cfg", "SquadArmoryWeapon5");
	SquadArmory[4][1] = dini_Int("squad.cfg", "SquadArmoryWeapon5Ammo");
	SquadArmory[5][0] = dini_Int("squad.cfg", "SquadArmoryWeapon6");
	SquadArmory[5][1] = dini_Int("squad.cfg", "SquadArmoryWeapon6Ammo");
	SquadArmory[6][0] = dini_Int("squad.cfg", "SquadArmoryWeapon7");
	SquadArmory[6][1] = dini_Int("squad.cfg", "SquadArmoryWeapon7Ammo");
	SquadArmory[7][0] = dini_Int("squad.cfg", "SquadArmoryWeapon8");
	SquadArmory[7][1] = dini_Int("squad.cfg", "SquadArmoryWeapon8Ammo");
	SquadArmory[8][0] = dini_Int("squad.cfg", "SquadArmoryWeapon9");
	SquadArmory[8][1] = dini_Int("squad.cfg", "SquadArmoryWeapon9Ammo");
	SquadArmory[9][0] = dini_Int("squad.cfg", "SquadArmoryWeapon10");
	SquadArmory[9][1] = dini_Int("squad.cfg", "SquadArmoryWeapon10Ammo");
	return 1;
}

SaveSquad(option=0) {
	if(!option) {
	    dini_Set("squad.cfg", "SquadPassword", SquadPassword);
	    dini_Set("squad.cfg", "SquadAdmPassword", SquadAdmPassword);
	    
	    dini_IntSet("squad.cfg", "SquadSafeCrack", SquadSafe[SQUAD_SAFE_CRACK]);
		dini_IntSet("squad.cfg", "SquadSafePot", SquadSafe[SQUAD_SAFE_POT]);
		dini_IntSet("squad.cfg", "SquadSafeMoney", SquadSafe[SQUAD_SAFE_MONEY]);
		
 		dini_IntSet("squad.cfg", "SquadArmoryWeapon1", SquadArmory[0][0]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon1Ammo", SquadArmory[0][1]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon2", SquadArmory[1][0]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon2Ammo", SquadArmory[1][1]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon3", SquadArmory[2][0]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon3Ammo", SquadArmory[2][1]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon4", SquadArmory[3][0]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon4Ammo", SquadArmory[3][1]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon4", SquadArmory[4][0]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon4Ammo", SquadArmory[4][1]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon4", SquadArmory[5][0]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon4Ammo", SquadArmory[5][1]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon5", SquadArmory[6][0]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon5Ammo", SquadArmory[6][1]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon6", SquadArmory[7][0]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon6Ammo", SquadArmory[7][1]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon7", SquadArmory[8][0]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon7Ammo", SquadArmory[8][1]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon8", SquadArmory[9][0]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon8Ammo", SquadArmory[9][1]);
	}
	else {
	    switch(option) {
	        case 1: {
	            dini_Set("squad.cfg", "SquadPassword", SquadPassword);
	    		dini_Set("squad.cfg", "SquadAdmPassword", SquadAdmPassword);
			}
			case 2: {
 				dini_IntSet("squad.cfg", "SquadSafeCrack", SquadSafe[SQUAD_SAFE_CRACK]);
				dini_IntSet("squad.cfg", "SquadSafePot", SquadSafe[SQUAD_SAFE_POT]);
				dini_IntSet("squad.cfg", "SquadSafeMoney", SquadSafe[SQUAD_SAFE_MONEY]);
			}
			case 3: {
 				dini_IntSet("squad.cfg", "SquadArmoryWeapon1", SquadArmory[0][0]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon1Ammo", SquadArmory[0][1]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon2", SquadArmory[1][0]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon2Ammo", SquadArmory[1][1]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon3", SquadArmory[2][0]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon3Ammo", SquadArmory[2][1]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon4", SquadArmory[3][0]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon4Ammo", SquadArmory[3][1]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon4", SquadArmory[4][0]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon4Ammo", SquadArmory[4][1]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon4", SquadArmory[5][0]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon4Ammo", SquadArmory[5][1]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon5", SquadArmory[6][0]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon5Ammo", SquadArmory[6][1]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon6", SquadArmory[7][0]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon6Ammo", SquadArmory[7][1]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon7", SquadArmory[8][0]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon7Ammo", SquadArmory[8][1]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon8", SquadArmory[9][0]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon8Ammo", SquadArmory[9][1]);
			}
		}
	}
	return 1;
}

Hook:SkarrTorino_OnGameModeInit() {
	LoadSquad();
	SquadElevator = CreateDynamicObject(18755, -943.96771, -55.71810, 1001.21130,   0.00000, 0.00000, 90.00000);
	SquadDoor = CreateDynamicObject(1569, -961.67230, -57.70300, 1004.36530,   0.00000, 0.00000, 0.0000);
	SquadElevatorDoor1 = CreateDynamicObject(18757, -944.410034, -55.688213, 1006.076171, 0.00000, 0.0000, 90.0000);
	SquadElevatorDoor2 = CreateDynamicObject(18757, -942.416015, -55.799083, 1006.076049, 0.00000, 0.0000, 90.0000);
	SquadElevatorDoor3 = CreateDynamicObject(18757, -946.029602, -55.722320, 1001.216247, 0.00000, 0.0000, 90.0000);
	SquadElevatorDoor4 = CreateDynamicObject(18757, -939.736389, -55.684394, 1001.225952, 0.00000, 0.0000, 90.0000);
	return 1;
}

Hook:SkarrTorino_OnPlayerConnect(playerid) {
	SquadLogged[playerid] = false;
	SquadAdmLogged[playerid] = false;
	SquadCamera[playerid] = false;
	return 1;
}

Hook:SkarrTorino_OnDynamicObjectMoved(objectid) {
	if(objectid == SquadElevator) {
        SquadElevatorState = SquadElevatorMoving;
	  	SquadElevatorMoving = (SquadElevatorMoving == true) ? (false) : (true);
	  	
	  	if(SquadElevatorState != false) {
	  	    MoveDynamicObject(SquadElevatorDoor1, -945.949340, -55.701606, 1006.076171, 5.0);
	  	    MoveDynamicObject(SquadElevatorDoor2, -940.425842, -55.795616, 1006.076049, 5.0);
		}
		else {
		    MoveDynamicObject(SquadElevatorDoor3, -946.029602, -55.722320, 1001.216247, 5.0);
		    MoveDynamicObject(SquadElevatorDoor4, -939.736389, -55.684394, 1001.225952, 5.0);
		}
	}
	else if(objectid == SquadDoor) {
	    if(SquadDoorMoving == true) {
	        SquadDoorState = true;
		}
		else {
		    SquadDoorState = false;
		}
	}
	return 1;
}

Hook:SkarrTorino_OnPlayerTakeDamage(playerid, issuerid, Float: amount, weaponid, bodypart) {
	if(SquadCamera[playerid] == true) {
		SendClientMessage(playerid, COLOR_GREEN, "(( You have been forced out of camera mode due to taking damage. ))");
		SetCameraBehindPlayer(playerid);
	    SquadCamera[playerid] = false;

	    TogglePlayerControllable(playerid, 1);
	    DeletePVar(playerid, "PlayerFrozen");
	}
	return 1;
}

CMD:sqlogin(playerid, params[]) {
	if(SquadLogged[playerid] == false) {
	    if(isnull(params)) SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqlogin [password]");
		else if(strmatch(params, SquadPassword) || PlayerInfo[playerid][pAdmin] >= 1337) {
		    SquadLogged[playerid] = true;
		    SendClientMessage(playerid, COLOR_YELLOW, "You have logged in to the squad system.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "Wrong password.");
	}
	else {
	    SquadLogged[playerid] = false;
	    SendClientMessage(playerid, COLOR_YELLOW, "You have logged out of the squad system.");
	}
	return 1;
}

CMD:sqhelp(playerid, params[]) {
	if(SquadLogged[playerid] == true) {
		ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "Squad Help", "/sqshow, /sqsafe, /sqcamera, /sqarmory, /sqmic, /sqpasscheck, /sqpass, /sqelevator\n/sqldoor /sqllockdown", "Close", "");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not a squad member.");
	return 1;
}

CMD:sqshow(playerid, params[]) {
	if(SquadLogged[playerid] == true) {
		SendClientMessage(playerid, COLOR_WHITE, "______________ Squad Members _____________");
		foreach(Player, i) {
		    if(SquadLogged[i] == true && PlayerInfo[i][pAdmin] < 2) {
		        format(donorstr, sizeof donorstr, "Squad Member %s(%d)", PlayerICName(i), i);
		        SendClientMessage(playerid, COLOR_GREY, donorstr);
			}
		}
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not a squad member!");
	return 1;
}

CMD:sqpassshow(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 1118 || PlayerInfo[playerid][pID] == 3718) {
		format(donorstr, sizeof donorstr, "Squad Login Password: %s", SquadPassword);
		SendClientMessage(playerid, COLOR_WHITE, donorstr);
		
		format(donorstr, sizeof donorstr, "Squad ADM Login Password: %s", SquadAdmPassword);
		SendClientMessage(playerid, COLOR_WHITE, donorstr);
	}
	return 1;
}

CMD:sqpass(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 1118) {
	    if(isnull(params)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqsafe [new password]");
	    if(strlen(params) >= 128) return SendClientMessage(playerid, COLOR_WHITE, "Password length cannot exceed 128 characters.");
		strdel(SquadPassword, 0, strlen(SquadPassword));
		strins(SquadPassword, params, 0, strlen(params));
		
		format(donorstr, sizeof donorstr, "You have changed the squad login password to: %s", SquadPassword);
		SendClientMessage(playerid, COLOR_YELLOW, donorstr);
		
		SaveSquad(1);
	}
	return 1;
}

CMD:sqsafe(playerid, params[]) {
	if(SquadLogged[playerid] == true) {
	    if(!IsPlayerInRangeOfPoint(playerid, 4.0, -966.3113, -49.8604, 1000.6631)) return SendClientMessage(playerid, COLOR_GREY, "You are not near the squad safe.");	    
		new action[15], val;
		if(sscanf(params, "s[15] D", action, val)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqsafe (check/get/store) (crack/pot/money)");
		if(strmatch(action, "check")) {
			format(donorstr, sizeof donorstr, "Crack: %d grams", SquadSafe[SQUAD_SAFE_CRACK]);
			SendClientMessage(playerid, COLOR_YELLOW, donorstr);
			
			format(donorstr, sizeof donorstr, "Pot: %d grams", SquadSafe[SQUAD_SAFE_POT]);
			SendClientMessage(playerid, COLOR_YELLOW, donorstr);
			
			format(donorstr, sizeof donorstr, "Money: $%d", SquadSafe[SQUAD_SAFE_MONEY]);
			SendClientMessage(playerid, COLOR_YELLOW, donorstr);
		}
		else if(strmatch(action, "get")) {
		    if(PlayerTazed[playerid] || PlayerCuffed[playerid] || PlayerFrozen[playerid] || PlayerInfo[playerid][pJailed]) return SendClientMessage(playerid, COLOR_GREY, "You cannot do this right now."); 
			new type[7], value = 0;
			if(sscanf(params, "s[24] s[7] i", action, type, value)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqsafe get [crack/pot/money] [amount]");
			if(value < 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
			if(strmatch(type, "crack")) {
			    if(SquadSafe[SQUAD_SAFE_CRACK] < value) return SendClientMessage(playerid, COLOR_GREY, "The safe doesn't have that much crack stored.");

				SquadSafe[SQUAD_SAFE_CRACK] -= value;
				PlayerInfo[playerid][pCrack] += value;
				
				format(donorstr, sizeof donorstr, "* %s takes some crack from the squad safe.", PlayerICName(playerid));
				ProxDetector(15.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			else if(strmatch(type, "pot")) {
			    if(SquadSafe[SQUAD_SAFE_POT] < value) return SendClientMessage(playerid, COLOR_GREY, "The safe doesn't have that much pot stored.");

				SquadSafe[SQUAD_SAFE_POT] -= value;
				PlayerInfo[playerid][pPot] += value;

				format(donorstr, sizeof donorstr, "* %s takes some pot from the squad safe.", PlayerICName(playerid));
				ProxDetector(15.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			else if(strmatch(type, "money")) {
                if(SquadSafe[SQUAD_SAFE_MONEY] < value) return SendClientMessage(playerid, COLOR_GREY, "The safe doesn't have that much money stored.");

				SquadSafe[SQUAD_SAFE_MONEY] -= value;
				GiveMoney(playerid, value);

				format(donorstr, sizeof donorstr, "* %s takes some money from the squad safe.", PlayerICName(playerid));
				ProxDetector(15.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			else return SendClientMessage(playerid, COLOR_GREY, "Invalid option.");
			SaveSquad(2);
		}
		else if(strmatch(action, "store")) {
		    if(PlayerTazed[playerid] || PlayerCuffed[playerid] || PlayerFrozen[playerid] || PlayerInfo[playerid][pJailed]) return SendClientMessage(playerid, COLOR_GREY, "You cannot do this right now.");
		    
		    new type[7], value = 0;
			if(sscanf(params, "s[24] s[7] i", action, type, value)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqsafe store [crack/pot/money] [amount]");
			if(value < 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
			
		    if(strmatch(type, "crack")) {
		        if(SquadSafe[SQUAD_SAFE_CRACK] + value >= 1000000) return SendClientMessage(playerid, COLOR_WHITE, "The squad safe cannot store that much crack.");
		        if(PlayerInfo[playerid][pCrack] < value) return SendClientMessage(playerid, COLOR_WHITE, "You do not have that much crack on your person.");
		        
		        PlayerInfo[playerid][pCrack] -= value;
		        SquadSafe[SQUAD_SAFE_CRACK] += value;
		        
		        format(donorstr, sizeof donorstr, "* %s puts some crack in the squad safe.", PlayerICName(playerid));
				ProxDetector(15.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
            else if(strmatch(type, "pot")) {
		        if(SquadSafe[SQUAD_SAFE_POT] + value >= 1000000) return SendClientMessage(playerid, COLOR_WHITE, "The squad safe cannot store that much pot.");
		        if(PlayerInfo[playerid][pPot] < value) return SendClientMessage(playerid, COLOR_WHITE, "You do not have that much pot on your person.");

		        PlayerInfo[playerid][pPot] -= value;
		        SquadSafe[SQUAD_SAFE_POT] += value;

		        format(donorstr, sizeof donorstr, "* %s puts some pot in the squad safe.", PlayerICName(playerid));
				ProxDetector(15.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
            else if(strmatch(type, "money")) {
		        if(SquadSafe[SQUAD_SAFE_MONEY] + value >= 1000000) return SendClientMessage(playerid, COLOR_WHITE, "The squad safe cannot store that much money.");
		        if(PlayerInfo[playerid][pCash] < value) return SendClientMessage(playerid, COLOR_WHITE, "You do not have that much money on your person.");

		        PlayerInfo[playerid][pCash] -= value;
		        SquadSafe[SQUAD_SAFE_MONEY] += value;

		        format(donorstr, sizeof donorstr, "* %s puts some money in the squad safe.", PlayerICName(playerid));
				ProxDetector(15.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			else return SendClientMessage(playerid, COLOR_GREY, "Invalid option.");
			SaveSquad(2);
		}
		else SendClientMessage(playerid, COLOR_GREY, "Invalid option(check, get, store).");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not a squad member.");
	return 1;
}

CMD:sqarmory(playerid, params[]) {
	if(SquadLogged[playerid] == true) {
	    if(!IsPlayerInRangeOfPoint(playerid, 4.0, -966.3113, -49.8604, 1000.6631)) return SendClientMessage(playerid, COLOR_GREY, "You are not near the squad safe.");
	    
	    new action[10], slot = 0;
	    if(sscanf(params, "s[10] I(0)", action, slot)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqarmory [check/get/put] [slot 0-9]");
		
		if(strmatch(action, "check")) {
			format(donorstr, sizeof donorstr, "Slot 1: %s(Ammo: %d) Slot 2: %s(Ammo: %d) Slot 3: %s(Ammo: %d) Slot 4: %s(Ammo: %d)\n", dGetWeaponName(SquadArmory[0][0]), SquadArmory[0][1], dGetWeaponName(SquadArmory[1][0]), SquadArmory[1][1], dGetWeaponName(SquadArmory[2][0]), SquadArmory[2][1], dGetWeaponName(SquadArmory[3][0]), SquadArmory[3][0]);
			format(donorstr, sizeof donorstr, "%sSlot 5: %s(Ammo: %d) Slot 6: %s(Ammo: %d) Slot 7: %s(Ammo: %d) Slot 8: %s(Ammo: %d)\n", donorstr, dGetWeaponName(SquadArmory[4][0]), SquadArmory[4][1], dGetWeaponName(SquadArmory[5][0]), SquadArmory[5][1], dGetWeaponName(SquadArmory[6][0]), SquadArmory[6][1], dGetWeaponName(SquadArmory[7][0]), SquadArmory[7][0]);
			format(donorstr, sizeof donorstr, "%sSlot 9: %s(Ammo: %d)", donorstr, dGetWeaponName(SquadArmory[8][0]), SquadArmory[8][1]);
			ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "Squad Armory", donorstr, "Close", "");
		}
		else if(strmatch(action, "get")) {
		    if(PlayerTazed[playerid] || PlayerCuffed[playerid] || PlayerFrozen[playerid] || PlayerInfo[playerid][pJailed]) return SendClientMessage(playerid, COLOR_GREY, "You cannot do this right now.");
		    if(sscanf(params, "s[10] i", action, slot)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqarmory get [slot(0-9)]");
		    if(slot < 0 || slot > 9) return SendClientMessage(playerid, COLOR_GREY, "Invalid weapon slot(0-9).");
		    if(SquadArmory[slot][0] == 0) return SendClientMessage(playerid, COLOR_GREY, "No weapon stored in this slot.");
		    if(PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GREY, "You must play for 2 hours before using a weapon.");

			format(donorstr, sizeof donorstr, "* %s takes a %s from the safe.", PlayerICName(playerid), dGetWeaponName(SquadArmory[slot][0]));
			ProxDetector(15.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			
			GivePlayerGun(playerid, SquadArmory[slot][0]);
			SquadArmory[slot][0] = 0;
			SquadArmory[slot][1] = 0;
			
			SaveSquad(3);
		}
		else if(strmatch(action, "put")) {
		    if(PlayerTazed[playerid] || PlayerCuffed[playerid] || PlayerFrozen[playerid] || PlayerInfo[playerid][pJailed]) return SendClientMessage(playerid, COLOR_GREY, "You cannot do this right now.");
		    if(sscanf(params, "s[10] i", action, slot)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqarmory put [slot(0-9)]");
		    if(slot < 0 || slot > 9) return SendClientMessage(playerid, COLOR_GREY, "Invalid weapon slot(0-9).");
		    if(GetPlayerWeapon(playerid) == 0) return SendClientMessage(playerid, COLOR_GREY, "You are not holding a weapon.");
		    
		    if(SquadArmory[slot][0] != 0) return SendClientMessage(playerid, COLOR_GREY, "There's already a weapon stored in that slot.");
			if(HaveAdminWeapon(playerid, GetPlayerWeapon(playerid))  == GetPlayerWeapon(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You cannot store this weapon.");
			
			format(donorstr, sizeof donorstr, "* %s puts a %s in their safe.", PlayerICName(playerid), dGetWeaponName(GetPlayerWeapon(playerid)));
			ProxDetector(15.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

			SquadArmory[slot][0] = GetPlayerWeapon(playerid);
			SquadArmory[slot][1] = 99999;
			TakePlayerWeapon(playerid, GetPlayerWeapon(playerid));
			
			SaveSquad(3);
		}
		else return SendClientMessage(playerid, -1, "Invalid option.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not a squad member.");
	return 1;
}

CMD:sqadm(playerid, params[]) {
	if(SquadAdmLogged[playerid] == false) {
	    if(isnull(params)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqadm [password]");
	    if(strmatch(params, SquadAdmPassword) || PlayerInfo[playerid][pAdmin] >= 1337) {
	        SquadAdmLogged[playerid] = true;
	        SendClientMessage(playerid, COLOR_YELLOW, "You have logged in to Squad ADM.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "Wrong password.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are already logged in.");
	return 1;
}

CMD:sqadmpass(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 1118) {
	    if(isnull(params)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqadmpass [new password]");
	    if(strlen(params) <= 1 || strlen(params) >= 128) return SendClientMessage(playerid, COLOR_WHITE, "Password length cannot exceed 128 characters.");

		strdel(SquadAdmPassword, 0, strlen(SquadAdmPassword));
		strins(SquadAdmPassword, params, 0, strlen(params));

		format(donorstr, sizeof donorstr, "You have changed the squad login password to: %s", params);
		SendClientMessage(playerid, COLOR_YELLOW, donorstr);

		SaveSquad(1);
	}
	return 1;
}

CMD:sqadmshow(playerid, params[]) {
	if(SquadAdmLogged[playerid] == true) {
		SendClientMessage(playerid, COLOR_WHITE, "______________ Squad Adm Members _____________");
		foreach(Player, i) {
		    if(SquadAdmLogged[i] == true || PlayerInfo[i][pAdmin] < 2) {
		        format(donorstr, sizeof donorstr, "Squad Adm Member %s(%d)", PlayerICName(i), i);
		        SendClientMessage(playerid, COLOR_GREY, donorstr);
			}
		}
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not a squad ADM member!");
	return 1;
}

CMD:sqmic(playerid, params[]) {
	if(SquadLogged[playerid] == true) {
	    if(SquadLockdown == true) return SendClientMessage(playerid, COLOR_GREY, "Electricity throughout the building has been cut, the microphone system cannot be used.");
		if(!IsPlayerInRangeOfPoint(playerid, 100.0, -963.88782, -43.89424, 1000.89648)) return SendClientMessage(playerid, COLOR_GREY, "You are not inside the SQUAD building.");
		if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /sqmic [text]");
		if(strlen(params) <= 2 || strlen(params) >= 100) return SendClientMessage(playerid, COLOR_GREY, "Text length must not exceed 100 characters.");
		
		format(donorstr, sizeof donorstr, "Microphone %s: %s", PlayerICName(playerid), params);
		foreach(Player, i) {
		    if(IsPlayerInRangeOfPoint(i, 100.0, -943.96771, -55.71810, 1001.21130)) {
		        SendClientMessage(i, COLOR_YELLOW, donorstr);
			}
		}
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not a squad member.");
	return 1;
}

CMD:sqlockdown(playerid, params[]) {
	if(SquadAdmLogged[playerid] == true) {
		if(SquadLockdown == true) {
		    SquadLockdown = false;
		    SendClientMessage(playerid, COLOR_WHITE, "All doors and elevators are now operational.");
		    
		    new Float: x, Float: y, Float: z;
		    foreach(Player, i) {
		        if(IsPlayerInRangeOfPoint(i, 50.0, -943.96771, -55.71810, 1001.21130)) {
		            SendClientMessage(i, COLOR_PURPLE, "** A green light flashes as power is restored to external eletrical systems. **");
                    GetPlayerPos(i, x, y, z);
		            PlayerPlaySound(i, 5206, x, y, z);
				}
			}
			
			if(SquadElevatorState == false) {
		  	    MoveDynamicObject(SquadElevatorDoor1, -945.949340, -55.701606, 1006.076171, 5.0);
		  	    MoveDynamicObject(SquadElevatorDoor2, -940.425842, -55.795616, 1006.076049, 5.0);
			}
			else {
			    MoveDynamicObject(SquadElevatorDoor3, -946.029602, -55.722320, 1001.216247, 5.0);
			    MoveDynamicObject(SquadElevatorDoor4, -939.736389, -55.684394, 1001.225952, 5.0);
			}
		}
		else {
		    SquadLockdown = true;
		    SendClientMessage(playerid, COLOR_WHITE, "All doors and elevators are now disabled.");
		    
		    new Float: x, Float: y, Float: z;
		    foreach(Player, i) {
		        if(IsPlayerInRangeOfPoint(i, 50.0, -943.96771, -55.71810, 1001.21130)) {
		            SendClientMessage(i, COLOR_PURPLE, "** A red light flashes as external eletrical systems are disabled.");
		            
		            GetPlayerPos(i, x, y, z);
		            PlayerPlaySound(i, 5205, x, y, z);
				}
			}
			
			// Shut all doors.
			MoveDynamicObject(SquadDoor, -961.67230, -57.70300, 1004.36530, 5.0, 0.0, 0.0, -127.0);
		    SquadDoorMoving = true;
		    
            MoveDynamicObject(SquadElevatorDoor1, -944.410034, -55.688213, 1006.076171, 5.0);
			MoveDynamicObject(SquadElevatorDoor2, -942.416015, -55.799083, 1006.076049, 5.0);
			MoveDynamicObject(SquadElevatorDoor3, -944.219543, -55.706516, 1001.216247, 5.0);
			MoveDynamicObject(SquadElevatorDoor4, -942.186279, -55.688678, 1001.225952, 5.0);
		}
	}
	else SendClientMessage(playerid, COLOR_GREY, "You're not a squad adm member.");
	return 1;
}

CMD:sqdoor(playerid, params[]) {
	if(SquadLockdown == true) return SendClientMessage(playerid, COLOR_GREY, "The door is currently locked.");
	if(SquadLogged[playerid] == false) return SendClientMessage(playerid, COLOR_GREY, "You do not have security clearance for this door.");
	if(IsPlayerInRangeOfPoint(playerid, 15.0, -961.67230, -57.70300, 1004.36530)) {
	    if(IsDynamicObjectMoving(SquadDoor)) return SendClientMessage(playerid, COLOR_GREY, "The door is currently moving, please wait.");
	    if(SquadDoorState == true) {
	        MoveDynamicObject(SquadDoor, -961.67230, -57.70300, 1004.36530, 5.0, 0.0, 0.0, 0.0);
			SquadDoorMoving = false;
		}
		else {
		    MoveDynamicObject(SquadDoor, -961.67230, -57.70300, 1004.36530, 5.0, 0.0, 0.0, -127.0);
		    SquadDoorMoving = true;
		}
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not near the squad door.");
	return 1;
}
		
CMD:sqelevator(playerid, params[]) {
	if(SquadLockdown == true) return SendClientMessage(playerid, COLOR_GREY, "The elevators are currently shut down.");
	if(IsPlayerInRangeOfPoint(playerid, 15.0, -943.96771, -55.71810, 1001.21130)) {
	    if(IsDynamicObjectMoving(SquadElevator)) return SendClientMessage(playerid, COLOR_GREY, "The elevator is currently moving, you must wait until it stops.");
		if(SquadElevatorState == false) {
		    SquadElevatorMoving = true;
		    MoveDynamicObject(SquadElevator, -944.17255, -55.72754, 1006.13593, 5.0);

			MoveDynamicObject(SquadElevatorDoor3, -944.219543, -55.706516, 1001.216247, 5.0);
			MoveDynamicObject(SquadElevatorDoor4, -942.186279, -55.688678, 1001.225952, 5.0);
		}
		else {
		    SquadElevatorMoving = false;
		    MoveDynamicObject(SquadElevator, -943.96771, -55.71810, 1001.21130, 5.0);
		    
			MoveDynamicObject(SquadElevatorDoor1, -944.410034, -55.688213, 1006.076171, 5.0);
			MoveDynamicObject(SquadElevatorDoor2, -942.416015, -55.799083, 1006.076049, 5.0);
		}
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not near the elevator.");
	return 1;
}

CMD:sqcamera(playerid, params[]) {
	if(SquadAdmLogged[playerid] == true) {
		if(IsPlayerInRangeOfPoint(playerid, 15.0, -963.3160, -54.4636, 1000.3814)) {
			if(isnull(params)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqcamera [1-3]");
			if(strmatch(params, "off")) {
				if(SquadCamera[playerid] == true) {
					SendClientMessage(playerid, COLOR_GREEN, "You have stopped watching the camera.");
					SetCameraBehindPlayer(playerid);
				    SquadCamera[playerid] = false;
				    
				    TogglePlayerControllable(playerid, 1);
				    DeletePVar(playerid, "PlayerFrozen");
				}
				else SendClientMessage(playerid, COLOR_GREY, "You are not watching the squad camera.");
			}
			else if(strval(params) >= 1 && strval(params) <= 3) {
			    switch(strval(params)) {
			        case 1: {
			    		SetPlayerCameraPos(playerid, -954.8012, -63.3440, 1010.8679);
						SetPlayerCameraLookAt(playerid, -955.5981, -62.7422, 1010.2980);
				    }
				    case 2: {
				        SetPlayerCameraPos(playerid, -968.5633, -55.9560, 1003.0616);
						SetPlayerCameraLookAt(playerid, -967.7614, -55.3607, 1002.9419);
					}
					case 3: {
					    SetPlayerCameraPos(playerid, -942.5555, -62.1995, 1004.1599);
						SetPlayerCameraLookAt(playerid, -943.0472, -61.3303, 1003.6302);
					}
				}
				
				format(donorstr, sizeof donorstr, "You are now watching CCTV #%d.", strval(params));
				SendClientMessage(playerid, COLOR_GREEN, donorstr);
				SquadCamera[playerid] = true;
				
				TogglePlayerControllable(playerid, 0);
				SetPVarInt(playerid, "PlayerFrozen", 1);
			}
			else SendClientMessage(playerid, COLOR_GREY, "Invalid option(off, 1-3).");
		}
		else SendClientMessage(playerid, COLOR_GREY, "You are not near the control room.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You're not allowed to do this.");
	return 1;
}