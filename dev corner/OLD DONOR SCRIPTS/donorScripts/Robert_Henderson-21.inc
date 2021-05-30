/*
 ____            __                     __      
/\  _`\         /\ \                   /\ \__   
\ \ \L\ \    ___\ \ \____     __   _ __\ \ ,_\  
 \ \ ,  /   / __`\ \ '__`\  /'__`\/\`'__\ \ \/  
  \ \ \\ \ /\ \L\ \ \ \L\ \/\  __/\ \ \/ \ \ \_ 
   \ \_\ \_\ \____/\ \_,__/\ \____\\ \_\  \ \__\
    \/_/\/ /\/___/  \/___/  \/____/ \/_/   \/__/
                                                
                                                
 __  __                     __                                          
/\ \/\ \                   /\ \                                         
\ \ \_\ \     __    ___    \_\ \     __   _ __   ____    ___     ___    
 \ \  _  \  /'__`\/' _ `\  /'_` \  /'__`\/\`'__\/',__\  / __`\ /' _ `\  
  \ \ \ \ \/\  __//\ \/\ \/\ \L\ \/\  __/\ \ \//\__, `\/\ \L\ \/\ \/\ \ 
   \ \_\ \_\ \____\ \_\ \_\ \___,_\ \____\\ \_\\/\____/\ \____/\ \_\ \_\
    \/_/\/_/\/____/\/_/\/_/\/__,_ /\/____/ \/_/ \/___/  \/___/  \/_/\/_/

	
	UNKNOWN DEVELOPMENT RERQUEST
*/

#define CLUB_WORLD_ID 0

enum rhList {
    rhItem[32],
    rhPrice,
    rhDrink,
}

new KushDJEnabled = 0;
new bool: KushDJEnabledPlayer[MAX_PLAYERS] = false;
new cluburl[128];
new rHendersonMic[MAX_PLAYERS];
new rHendersonDJ[MAX_PLAYERS];
new KushDJArea;
new Text3D:VIPPrice;
new Text3D:DoorPrice;
new rhDj[MAX_PLAYERS];
new rhSmoke;
new rhDisco;
new rhVipLock;
new rhDoorLock;
new rhVipPrice;
new rhDoorPrice;
new rhSafeWeapon[10];
new rhSafeCash;
new rhSafePot;
new rhSafeCrack;
new rhSafeMats;
new rhSmokeObject[3];
new rhDiscoObject[4];
new rhListItems[10][rhList];

stock SaverhClub() {
	new file[25];
	format(file, sizeof(file), "RobertHendersonClubDocuments.cfg");
    if(!dini_Exists(file)) dini_Create(file);

    dini_IntSet(file, "VIP", rhVipPrice);
    dini_IntSet(file, "Door", rhDoorPrice);

    dini_IntSet(file, "Weapon1", rhSafeWeapon[0]);
    dini_IntSet(file, "Weapon2", rhSafeWeapon[1]);
    dini_IntSet(file, "Weapon3", rhSafeWeapon[2]);
    dini_IntSet(file, "Weapon4", rhSafeWeapon[3]);
    dini_IntSet(file, "Weapon5", rhSafeWeapon[4]);
    dini_IntSet(file, "Weapon6", rhSafeWeapon[5]);
    dini_IntSet(file, "Weapon7", rhSafeWeapon[6]);
    dini_IntSet(file, "Weapon8", rhSafeWeapon[7]);
    dini_IntSet(file, "Weapon9", rhSafeWeapon[8]);
    dini_IntSet(file, "Weapon10", rhSafeWeapon[9]);

    dini_IntSet(file, "Cash", rhSafeCash);
    dini_IntSet(file, "Pot", rhSafePot);
    dini_IntSet(file, "Crack", rhSafeCrack);
    dini_IntSet(file, "Mats", rhSafeMats);

    dini_Set(file, "ItemName1", rhListItems[0][rhItem]);
    dini_Set(file, "ItemName2", rhListItems[1][rhItem]);
    dini_Set(file, "ItemName3", rhListItems[2][rhItem]);
    dini_Set(file, "ItemName4", rhListItems[3][rhItem]);
    dini_Set(file, "ItemName5", rhListItems[4][rhItem]);
    dini_Set(file, "ItemName6", rhListItems[5][rhItem]);
    dini_Set(file, "ItemName7", rhListItems[6][rhItem]);
    dini_Set(file, "ItemName8", rhListItems[7][rhItem]);
    dini_Set(file, "ItemName9", rhListItems[8][rhItem]);
    dini_Set(file, "ItemName10", rhListItems[9][rhItem]);

    dini_IntSet(file, "ItemPrice1", rhListItems[0][rhPrice]);
    dini_IntSet(file, "ItemPrice2", rhListItems[1][rhPrice]);
    dini_IntSet(file, "ItemPrice3", rhListItems[2][rhPrice]);
    dini_IntSet(file, "ItemPrice4", rhListItems[3][rhPrice]);
    dini_IntSet(file, "ItemPrice5", rhListItems[4][rhPrice]);
    dini_IntSet(file, "ItemPrice6", rhListItems[5][rhPrice]);
    dini_IntSet(file, "ItemPrice7", rhListItems[6][rhPrice]);
    dini_IntSet(file, "ItemPrice8", rhListItems[7][rhPrice]);
    dini_IntSet(file, "ItemPrice9", rhListItems[8][rhPrice]);
    dini_IntSet(file, "ItemPrice10", rhListItems[9][rhPrice]);

    dini_IntSet(file, "ItemDrink1", rhListItems[0][rhDrink]);
    dini_IntSet(file, "ItemDrink2", rhListItems[1][rhDrink]);
    dini_IntSet(file, "ItemDrink3", rhListItems[2][rhDrink]);
    dini_IntSet(file, "ItemDrink4", rhListItems[3][rhDrink]);
    dini_IntSet(file, "ItemDrink5", rhListItems[4][rhDrink]);
    dini_IntSet(file, "ItemDrink6", rhListItems[5][rhDrink]);
    dini_IntSet(file, "ItemDrink7", rhListItems[6][rhDrink]);
    dini_IntSet(file, "ItemDrink8", rhListItems[7][rhDrink]);
    dini_IntSet(file, "ItemDrink9", rhListItems[8][rhDrink]);
    dini_IntSet(file, "ItemDrink10", rhListItems[9][rhDrink]);

    print("Robert Henderson data is saved.");
    return 1;
}

stock LoadrhClub() {
    new file[25];
    format(file, sizeof(file), "RobertHendersonClubDocuments.cfg");
    if(!dini_Exists(file)) dini_Create(file);

    rhVipPrice = dini_Int(file, "VIP");
    rhDoorPrice = dini_Int(file, "Door");

    rhSafeWeapon[0] = dini_Int(file, "Weapon1");
    rhSafeWeapon[1] = dini_Int(file, "Weapon2");
    rhSafeWeapon[2] = dini_Int(file, "Weapon3");
    rhSafeWeapon[3] = dini_Int(file, "Weapon4");
    rhSafeWeapon[4] = dini_Int(file, "Weapon5");
    rhSafeWeapon[5] = dini_Int(file, "Weapon6");
    rhSafeWeapon[6] = dini_Int(file, "Weapon7");
    rhSafeWeapon[7] = dini_Int(file, "Weapon8");
    rhSafeWeapon[8] = dini_Int(file, "Weapon9");
    rhSafeWeapon[9] = dini_Int(file, "Weapon10");

    rhSafeCash = dini_Int(file, "Cash");
    rhSafePot = dini_Int(file, "Pot");
    rhSafeCrack = dini_Int(file, "Crack");
    rhSafeMats = dini_Int(file, "Mats");

    format(rhListItems[0][rhItem], 32, "%s", dini_Get(file, "ItemName1"));
    format(rhListItems[1][rhItem], 32, "%s", dini_Get(file, "ItemName2"));
    format(rhListItems[2][rhItem], 32, "%s", dini_Get(file, "ItemName3"));
    format(rhListItems[3][rhItem], 32, "%s", dini_Get(file, "ItemName4"));
    format(rhListItems[4][rhItem], 32, "%s", dini_Get(file, "ItemName5"));
    format(rhListItems[5][rhItem], 32, "%s", dini_Get(file, "ItemName6"));
    format(rhListItems[6][rhItem], 32, "%s", dini_Get(file, "ItemName7"));
    format(rhListItems[7][rhItem], 32, "%s", dini_Get(file, "ItemName8"));
    format(rhListItems[8][rhItem], 32, "%s", dini_Get(file, "ItemName9"));
    format(rhListItems[9][rhItem], 32, "%s", dini_Get(file, "ItemName10"));

    rhListItems[0][rhPrice] =  dini_Int(file, "ItemPrice1");
    rhListItems[1][rhPrice] =  dini_Int(file, "ItemPrice2");
    rhListItems[2][rhPrice] =  dini_Int(file, "ItemPrice3");
    rhListItems[3][rhPrice] =  dini_Int(file, "ItemPrice4");
    rhListItems[4][rhPrice] =  dini_Int(file, "ItemPrice5");
    rhListItems[5][rhPrice] =  dini_Int(file, "ItemPrice6");
    rhListItems[6][rhPrice] =  dini_Int(file, "ItemPrice7");
    rhListItems[7][rhPrice] =  dini_Int(file, "ItemPrice8");
    rhListItems[8][rhPrice] =  dini_Int(file, "ItemPrice9");
    rhListItems[9][rhPrice] =  dini_Int(file, "ItemPrice10");

    rhListItems[0][rhDrink] =  dini_Int(file, "ItemDrink1");
    rhListItems[1][rhDrink] =  dini_Int(file, "ItemDrink2");
    rhListItems[2][rhDrink] =  dini_Int(file, "ItemDrink3");
    rhListItems[3][rhDrink] =  dini_Int(file, "ItemDrink4");
    rhListItems[4][rhDrink] =  dini_Int(file, "ItemDrink5");
    rhListItems[5][rhDrink] =  dini_Int(file, "ItemDrink6");
    rhListItems[6][rhDrink] =  dini_Int(file, "ItemDrink7");
    rhListItems[7][rhDrink] =  dini_Int(file, "ItemDrink8");
    rhListItems[8][rhDrink] =  dini_Int(file, "ItemDrink9");
    rhListItems[9][rhDrink] =  dini_Int(file, "ItemDrink10");

    print("Loaded");
    return 1;
}

Hook:RobertHenderson_OnGameModeInit() {
	LoadrhClub();
	KushDJArea = CreateDynamicCircle(1271.4037, -1254.4858, 40.0, -1, -1);
	CreateDynamic3DTextLabel("To purchase a drink\nType (/kushbar)!", COLOR_LIGHTBLUE, 1267.2490,-1262.1517,19380.3145, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 5);
	VIPPrice = CreateDynamic3DTextLabel("{FFFFFF}VIP Entrance\nEntry Fee:({FF0000}CLOSED{FFFFFF})", COLOR_WHITE, 1260.8263,-1264.8387,19380.3145, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 5);
	DoorPrice = CreateDynamic3DTextLabel("{FFFFFF}.:. {FF9900}Kush Lounge{FFFFFF} .:. \n{FF0000}CLOSED{FFFFFF}\nEntry Fee: (unknown)", COLOR_WHITE, 1282.2648, -1262.0641, 14.6107, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
	return 1;
}

Hook:RobertHenderson_OnPlayerConnect(playerid) {
	rHendersonMic[playerid] = 0;
    rHendersonDJ[playerid] = 0;
    rhDj[playerid] = 0;
	return 1;
}

Hook:RobertHenderson_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) {
	if(dialogid == DIALOG_KUSH_BAR) {
        if(!response) return 1;
        else {
            switch(listitem) {
                case 0: {
                    if(PlayerInfo[playerid][pCash] < rhListItems[listitem][rhPrice]) return SendClientMessage(playerid, -1, "You don't have enough cash");
                    format(donorstr, sizeof(donorstr), "You bought a %s from The Kush Lounge bar.", rhListItems[listitem][rhItem]);
                    SendClientMessage(playerid, 0xF5F5DCFF, donorstr);
					SetPlayerSpecialAction(playerid, rhListItems[listitem][rhDrink]);
                    GiveMoney(playerid, -rhListItems[listitem][rhPrice]);
                    rhSafeCash += rhListItems[listitem][rhPrice];
                    SaverhClub();
                }
                case 1: {
                    if(PlayerInfo[playerid][pCash] < rhListItems[listitem][rhPrice]) return SendClientMessage(playerid, -1, "You don't have enough cash");
                    format(donorstr, sizeof(donorstr), "You bought a %s from The Kush Lounge bar.", rhListItems[listitem][rhItem]);
                    SendClientMessage(playerid, 0xF5F5DCFF, donorstr);
					SetPlayerSpecialAction(playerid, rhListItems[listitem][rhDrink]);
                    GiveMoney(playerid, -rhListItems[listitem][rhPrice]);
                    rhSafeCash += rhListItems[listitem][rhPrice];
                    SaverhClub();
                }
                case 2: {
                    if(PlayerInfo[playerid][pCash] < rhListItems[listitem][rhPrice]) return SendClientMessage(playerid, -1, "You don't have enough cash");
                    format(donorstr, sizeof(donorstr), "You bought a %s from The Kush Lounge bar.", rhListItems[listitem][rhItem]);
                    SendClientMessage(playerid, 0xF5F5DCFF, donorstr);
					SetPlayerSpecialAction(playerid, rhListItems[listitem][rhDrink]);
                    GiveMoney(playerid, -rhListItems[listitem][rhPrice]);
                    rhSafeCash += rhListItems[listitem][rhPrice];
                    SaverhClub();
                }
                case 3: {
                    if(PlayerInfo[playerid][pCash] < rhListItems[listitem][rhPrice]) return SendClientMessage(playerid, -1, "You don't have enough cash");
                    format(donorstr, sizeof(donorstr), "You bought a %s from The Kush Lounge bar.", rhListItems[listitem][rhItem]);
                    SendClientMessage(playerid, 0xF5F5DCFF, donorstr);
					SetPlayerSpecialAction(playerid, rhListItems[listitem][rhDrink]);
                    GiveMoney(playerid, -rhListItems[listitem][rhPrice]);
                    rhSafeCash += rhListItems[listitem][rhPrice];
                    SaverhClub();
                }
                case 4: {
                    if(PlayerInfo[playerid][pCash] < rhListItems[listitem][rhPrice]) return SendClientMessage(playerid, -1, "You don't have enough cash");
                    format(donorstr, sizeof(donorstr), "You bought a %s from The Kush Lounge bar.", rhListItems[listitem][rhItem]);
                    SendClientMessage(playerid, 0xF5F5DCFF, donorstr);
					SetPlayerSpecialAction(playerid, rhListItems[listitem][rhDrink]);
                    GiveMoney(playerid, -rhListItems[listitem][rhPrice]);
                    rhSafeCash += rhListItems[listitem][rhPrice];
                    SaverhClub();
                }
                case 5: {
                    if(PlayerInfo[playerid][pCash] < rhListItems[listitem][rhPrice]) return SendClientMessage(playerid, -1, "You don't have enough cash");

                    format(donorstr, sizeof(donorstr), "You bought a %s from The Kush Lounge bar.", rhListItems[listitem][rhItem]);
                    SendClientMessage(playerid, 0xF5F5DCFF, donorstr);
					SetPlayerSpecialAction(playerid, rhListItems[listitem][rhDrink]);
                    GiveMoney(playerid, -rhListItems[listitem][rhPrice]);
                    rhSafeCash += rhListItems[listitem][rhPrice];
                    SaverhClub();
                }
                case 6: {
                    if(PlayerInfo[playerid][pCash] < rhListItems[listitem][rhPrice]) return SendClientMessage(playerid, -1, "You don't have enough cash");

                    format(donorstr, sizeof(donorstr), "You bought a %s from The Kush Lounge bar.", rhListItems[listitem][rhItem]);
                    SendClientMessage(playerid, 0xF5F5DCFF, donorstr);
					SetPlayerSpecialAction(playerid, rhListItems[listitem][rhDrink]);
                    GiveMoney(playerid, -rhListItems[listitem][rhPrice]);
                    rhSafeCash += rhListItems[listitem][rhPrice];
                    SaverhClub();
                }
                case 7: {
                    if(PlayerInfo[playerid][pCash] < rhListItems[listitem][rhPrice]) return SendClientMessage(playerid, -1, "You don't have enough cash");
                    format(donorstr, sizeof(donorstr), "You bought a %s from The Kush Lounge bar.", rhListItems[listitem][rhItem]);
                    SendClientMessage(playerid, 0xF5F5DCFF, donorstr);
					SetPlayerSpecialAction(playerid, rhListItems[listitem][rhDrink]);
                    GiveMoney(playerid, -rhListItems[listitem][rhPrice]);
                    rhSafeCash += rhListItems[listitem][rhPrice];
                    SaverhClub();
                }
                case 8: {
                    if(PlayerInfo[playerid][pCash] < rhListItems[listitem][rhPrice]) return SendClientMessage(playerid, -1, "You don't have enough cash");
                    format(donorstr, sizeof(donorstr), "You bought a %s from The Kush Lounge bar.", rhListItems[listitem][rhItem]);
                    SendClientMessage(playerid, 0xF5F5DCFF, donorstr);
					SetPlayerSpecialAction(playerid, rhListItems[listitem][rhDrink]);
                    GiveMoney(playerid, -rhListItems[listitem][rhPrice]);
                    rhSafeCash += rhListItems[listitem][rhPrice];
                    SaverhClub();
                }
                case 9: {
                    if(PlayerInfo[playerid][pCash] < rhListItems[listitem][rhPrice]) return SendClientMessage(playerid, -1, "You don't have enough cash");
                    format(donorstr, sizeof(donorstr), "You bought a %s from The Kush Lounge bar.", rhListItems[listitem][rhItem]);
                    SendClientMessage(playerid, 0xF5F5DCFF, donorstr);
					SetPlayerSpecialAction(playerid, rhListItems[listitem][rhDrink]);
                    GiveMoney(playerid, -rhListItems[listitem][rhPrice]);
                    rhSafeCash += rhListItems[listitem][rhPrice];
                    SaverhClub();
                }
            }
        }
    }
	return 1;
}

Hook:RobertHenderson_OnPlayerEnterDynamicArea(playerid, areaid) {
	if(areaid == KushDJArea) {
	    if(!disabledRadios[playerid] && KushDJEnabled) {
	        if(KushDJEnabledPlayer[playerid] == false) {
	            PlayAudioStreamForPlayer(playerid, cluburl, 1271.4037, -1254.4858, 19380.4199, 40.0, true);
	            usingRadio[playerid] = 0;
	            KushDJEnabledPlayer[playerid] = true;
			}
		}
	}
	return 1;
}

Hook:RobertHenderson_OnPlayerLeaveDynamicArea(playerid, areaid) {
	if(areaid == KushDJArea) {
	    if(KushDJEnabledPlayer[playerid] == true)
	        StopAudioStreamForPlayer(playerid), KushDJEnabledPlayer[playerid] = false;
	}
	return 1;
}

CMD:mic(playerid, params[]) {
	if(!IsPlayerInRangeOfPoint(playerid, 100.0, 1271.1643, -1254.8700, 19380.2266)) return SendClientMessage(playerid, 0xAA3333AA, "You are not in the Kush Lounge!");
    if(rHendersonMic[playerid] != 1) return SendClientMessage(playerid, 0xAA3333AA, "You don't have control of the microphone.");
	if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, "USAGE: /mic [text]");
    format(donorstr, sizeof(donorstr), "[{33CCFF}DJ{FFFF00}] %s: %s", PlayerICName(playerid), params);
    ProxDetector(100.0, playerid, donorstr, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW);
    return 1;
}

CMD:givemic(playerid, params[]) {
    new	name[MAX_PLAYER_NAME + 1], givename[MAX_PLAYER_NAME + 1], playerb, Float:kPos[3];
    GetPlayerPos(playerid, kPos[0], kPos[1], kPos[2]);

    if(PlayerInfo[playerid][pID] != 21 && PlayerInfo[playerid][pID] != 5 && PlayerInfo[playerid][pID] != 3393 && PlayerInfo[playerid][pID] != 425) return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this command.");
    if(sscanf(params, "u", playerb)) return SendClientMessage(playerid, COLOR_GREY, "USAGE: /givemic [playerid/PartOfName]");
    if(!IsPlayerConnected(playerb)) return SendClientMessage(playerid, COLOR_GREY, "Invalid playerid.");
    if(!IsPlayerInRangeOfPoint(playerb, 5.0, kPos[0], kPos[1], kPos[2])) return SendClientMessage(playerid, -1, "That person is not near you.");

    strmid(name, PlayerICName(playerid), 0, MAX_PLAYER_NAME + 1);
    strmid(givename, PlayerICName(playerb), 0, MAX_PLAYER_NAME + 1);
    if(rHendersonMic[playerb] == 0) {
        format(donorstr, sizeof(donorstr), "You have given %s a microphone.", givename);
        SendClientMessage(playerid, 0x33AA33AA, donorstr);

        rHendersonMic[playerb] = 1;

        format(donorstr, sizeof(donorstr), "%s has given you a microphone.", name);
        SendClientMessage(playerb, 0x33AA33AA, donorstr);
    }
    else if(rHendersonMic[playerb] == 1) {
		format(donorstr, sizeof(donorstr), "You have removed %s's microphone.", givename);
		SendClientMessage(playerid, 0xAA3333AA, donorstr);

		rHendersonMic[playerb] = 0;

		format(donorstr, sizeof(donorstr), "%s has removed your microphone.", name);
		SendClientMessage(playerb, 0xAA3333AA, donorstr);
    }
    return 1;
}

CMD:kushbaredit(playerid, params[]) {
    if(PlayerInfo[playerid][pID] == 21) {
        new slot, name[32], amount, select[5];
	    if(sscanf(params, "s[5]i", select, slot))
			return SendClientMessage(playerid, -1, "/kushbaredit [name/price/drink] slot[0-9]");
	    if(!strcmp(params, "name", true, 4)) {
		    if(sscanf(params,"s[5]is[32]", select, slot, name))
				return SendClientMessage(playerid, -1, "/kushbaredit name slot[0-9] [name]");

	        format(rhListItems[slot][rhItem], 32, "%s", name);
	        SendClientMessage(playerid, 0xFF00FFFF, "Item name changed");
	        SaverhClub();

	    }
	    else if(!strcmp(params, "price", true, 5)) {

	        if(sscanf(params,"s[5]ii", select, slot, amount))
				return SendClientMessage(playerid, -1, "/kushbaredit price slot[0-9] [amount]");

	        if(amount < 0 || amount > 10000)
				return SendClientMessage(playerid, COLOR_WHITE, "   You can't sell your drinks for less than 0 or more than 10k$");
	        else {
	            rhListItems[slot][rhPrice] = amount;
	            SendClientMessage(playerid, 0xFF00FFFF, "Item price changed");
	            SaverhClub();
	        }
	    }
	    else if(!strcmp(params, "drink", true, 5)) {
	        if(sscanf(params,"s[5]is[32]", select, slot, name))
	        	return SendClientMessage(playerid, -1, "/kushbaredit drink slot[0-9] [beer, sprunk, wine]");

	        if(!strcmp(name, "beer", true, 4)) {
	            rhListItems[slot][rhDrink] = 20;
	            SendClientMessage(playerid, 0xFF00FFFF, "Item drink changed");
	    		SaverhClub();
	        }
	        else if(!strcmp(name, "sprunk", true, 5)) {
	            rhListItems[slot][rhDrink] = 23;
	            SendClientMessage(playerid, 0xFF00FFFF, "Item drink changed");
	            SaverhClub();
	        }
	        else if(!strcmp(name, "wine", true, 4)) {
	            rhListItems[slot][rhDrink] = 22;
	            SendClientMessage(playerid, 0xFF00FFFF, "Item drink changed");
	            SaverhClub();
	        }
	    }
    }
	return 1;
}

CMD:kushsafe(playerid, params[]) {
    if(PlayerInfo[playerid][pID] == 21 || PlayerInfo[playerid][pID] == 425) {
    	if(!IsPlayerInRangeOfPoint(playerid, 2.0, 1275.4397, -1259.2340, 19382.9180)) {
			return SendClientMessage(playerid, -1, "You're not near the safe.");
		}
		new option[32], amount;
        if(sscanf(params,"s[32]", params)) {
            SendClientMessage(playerid, -1, "/kushsafe [type]");
            SendClientMessage(playerid, 0xAFAFAFAA, "Type: balance | withdraw | deposit");
            return 1;
        }
        if(!strcmp(params, "balance",true,7)) {
            new string2[1000];
            format(donorstr, sizeof(donorstr), "{FFFFFF}Cash: {FFCA05}%d \n{FFFFFF}Pot: {FFCA05}%d \n{FFFFFF}Crack: {FFCA05}%d \n{FFFFFF}Materials: {FFCA05}%d \n", rhSafeCash, rhSafePot, rhSafeCrack, rhSafeMats);
            strcat(donorstr, "{FFFFFF}Weapon Slot 0: {FFCA05}%s\n{FFFFFF}Weapon Slot 1: {FFCA05}%s\n{FFFFFF}Weapon Slot 2: {FFCA05}%s\n");
            strcat(donorstr, "{FFFFFF}Weapon Slot 3: {FFCA05}%s\n{FFFFFF}Weapon Slot 4: {FFCA05}%s\n{FFFFFF}Weapon Slot 5:{FFCA05} %s \n");
            strcat(donorstr, "{FFFFFF}Weapon Slot 6: {FFCA05}%s\n{FFFFFF}Weapon Slot 7: {FFCA05}%s\n{FFFFFF}Weapon Slot 8: {FFCA05}%s \n{FFFFFF}Weapon Slot 9: {FFCA05}%s");
            format(string2, sizeof(string2), donorstr,
            dGetWeaponName(rhSafeWeapon[0]), dGetWeaponName(rhSafeWeapon[1]), dGetWeaponName(rhSafeWeapon[2]), dGetWeaponName(rhSafeWeapon[3]),
            dGetWeaponName(rhSafeWeapon[4]), dGetWeaponName(rhSafeWeapon[5]), dGetWeaponName(rhSafeWeapon[6]), dGetWeaponName(rhSafeWeapon[7]),
            dGetWeaponName(rhSafeWeapon[8]), dGetWeaponName(rhSafeWeapon[9]));

            ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "                {FF00A2} The Safe", string2, "Close", "");
            return 1;
        }
        else if(!strcmp(params, "withdraw",true,8)) { //negative values okay, makes safe positive and player negative
            if(sscanf(params,"s[32]s[32]i", params, option, amount)) {
                SendClientMessage(playerid, -1, "/kushsafe withdraw [option] [amount]");
                SendClientMessage(playerid, 0xAFAFAFAA, "Option: cash | pot | crack | mats | weapon(amount == slot || 0 - 9 slots)");
                return 1;
            }
            if(!strcmp(option, "cash",true,4)) {
                if(amount > rhSafeCash) return SendClientMessage(playerid, 0xAA3333AA, "You can't withdraw more than the amount.");
				if(amount < 1)
				    return SendClientMessage(playerid, COLOR_GREY, "You withdraw no money.");
				rhSafeCash -= amount;
                GiveMoney(playerid, amount);
                SaverhClub();
                SendClientMessage(playerid, 0xAA3333AA, "You have withdrawn some wonga");
            }
            else if(!strcmp(option, "pot",true,3)) {
                if(amount > rhSafePot) return SendClientMessage(playerid, 0xAA3333AA, "You can't withdraw more than the amount.");
				if(amount < 1)
				    return SendClientMessage(playerid, COLOR_GREY, "You deposit no grams of pot.");
				rhSafePot -= amount;
                PlayerInfo[playerid][pPot] += amount;
                SaverhClub();
                SendClientMessage(playerid, 0xAA3333AA, "You have withdrawn some dope, thats dope man.");
            }
            else if(!strcmp(option, "crack",true, 5)) {
                if(amount > rhSafeCrack) return SendClientMessage(playerid, 0xAA3333AA, "You can't withdraw more than the amount.");
				if(amount < 1)
				    return SendClientMessage(playerid, COLOR_GREY, "You deposit no grams of crack.");
				rhSafeCrack -= amount;
                PlayerInfo[playerid][pCrack] += amount;
                SaverhClub();
                SendClientMessage(playerid, 0xAA3333AA, "You have withdrawn some crack");
            }
            else if(!strcmp(option, "mats",true, 5)) {
                if(amount > rhSafeMats) return SendClientMessage(playerid, 0xAA3333AA, "You can't withdraw more than the amount.");
				if(amount < 1)
				    return SendClientMessage(playerid, COLOR_GREY, "You deposit no materials.");
				rhSafeMats -= amount;
                PlayerInfo[playerid][pMats] += amount;
                SaverhClub();
                SendClientMessage(playerid, 0xAA3333AA, "You have withdrawn some mats");
            }
            else if(!strcmp(option, "weapon",true, 6)) {
                new slot = amount, weaponid = rhSafeWeapon[slot];
                if(rhSafeWeapon[slot] == 0) return SendClientMessage(playerid, 0xAA3333AA, "There is no weapon in that holder.");
                GivePlayerGun(playerid, weaponid);
                rhSafeWeapon[slot] = 0;
                SaverhClub();
                SendClientMessage(playerid, 0xAA3333AA, "You have withdrawn a gun.");
            }
        }
        else if(!strcmp(params, "deposit",true,7)) {
            if(sscanf(params,"s[32]s[32]i", params, option, amount)) {
                SendClientMessage(playerid, -1, "/kushsafe deposit [option] [amount]");
                SendClientMessage(playerid, 0xAFAFAFAA, "Option: cash | pot | crack | materials | weapon(amount == slot || 0 - 9 slots)");
                return 1;
            }
            if(!strcmp(option, "cash",true,4)) {
                if(rhSafeCash > 10000000 ||  amount > PlayerInfo[playerid][pCash])
					return SendClientMessage(playerid, 0xAA3333AA, "You either don't have the cash or the safe is full.");
				if(amount < 1)
				    return SendClientMessage(playerid, COLOR_GREY, "You deposit no money.");
				rhSafeCash += amount;
                GiveMoney(playerid, -amount);
                SaverhClub();
				SendClientMessage(playerid, 0xAA3333AA, "You have deposited some mad dolla");
            }
            else if(!strcmp(option, "pot",true,3)) {
                if(amount > PlayerInfo[playerid][pPot]) return SendClientMessage(playerid, 0xAA3333AA, "You can't deposit more than the amount.");
				if(amount < 1)
				    return SendClientMessage(playerid, COLOR_GREY, "You deposit no grams of pot.");
				rhSafePot += amount;
                PlayerInfo[playerid][pPot] -= amount;
    			SaverhClub();
    			SendClientMessage(playerid, 0xAA3333AA, "You have deposited some pot");
            }
            else if(!strcmp(option, "crack",true,5)) {
                if(amount > PlayerInfo[playerid][pCrack]) return SendClientMessage(playerid, 0xAA3333AA, "You can't deposit more than the amount.");
				if(amount < 1)
				    return SendClientMessage(playerid, COLOR_GREY, "You deposit no grams of crack.");
				rhSafeCrack += amount;
                PlayerInfo[playerid][pCrack] -= amount;
                SaverhClub();
                SendClientMessage(playerid, 0xAA3333AA, "You have deposited some crack");
            }
            else if(!strcmp(option, "mats",true,5)) {
                if(amount > PlayerInfo[playerid][pMats]) return SendClientMessage(playerid, 0xAA3333AA, "You can't deposit more than the amount.");
				if(amount < 1)
				    return SendClientMessage(playerid, COLOR_GREY, "You deposit no materials.");
				rhSafeMats += amount;
                PlayerInfo[playerid][pMats] -= amount;
                SaverhClub();
                SendClientMessage(playerid, 0xAA3333AA, "You have deposited some mats");
            }
            else if(!strcmp(option, "weapon",true,6)) {
                new slot = amount;
	            new gunid = GetPlayerWeapon(playerid);

                if(gunid == 0) return SendClientMessage(playerid, 0xAA3333A, "You're not holding a weapon");
                if(HaveAdminWeapon(playerid, gunid) == gunid) return SendClientMessage(playerid, COLOR_GREY, "You cannot store this weapon.");
                if(rhSafeWeapon[slot] != 0) return SendClientMessage(playerid, 0xAA3333AA, "There is a weapon in this slot.");
                rhSafeWeapon[slot] = GetPlayerWeapon(playerid);
                TakePlayerWeapon(playerid, gunid);
                SaverhClub();
                SendClientMessage(playerid, 0xAA3333AA, "You have deposited a weapon");
            }
        }
    }
	return 1;
}

CMD:kushsmoke(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 21 || PlayerInfo[playerid][pID] == 3393) {
        if(!IsPlayerInRangeOfPoint(playerid, 5.0, 1271.1643, -1254.8700, 19380.2266)) return SendClientMessage(playerid, COLOR_GREY, "You are not near the smoke machine.");
        if(rhSmoke == 0) {
            rhSmokeObject[0] = CreateDynamicObject(2780, 1265.93506, -1255.47717, 19378.00000,   0.00000, 0.00000, 0.00000);
            rhSmokeObject[1] = CreateDynamicObject(2780, 1268.05457, -1258.76318, 19378.00000,   0.00000, 0.00000, 0.00000);
            rhSmokeObject[2] = CreateDynamicObject(2780, 1267.84680, -1251.01160, 19378.00000,   0.00000, 0.00000, 0.00000);
            rhSmoke = 1;

            format(donorstr, sizeof(donorstr), "You have enabled the smoke machine.");
            SendClientMessage(playerid, 0x33AA33AA, donorstr);
        }
        else if(rhSmoke == 1) {
		    DestroyDynamicObjectEx(rhSmokeObject[0]);
		    DestroyDynamicObjectEx(rhSmokeObject[1]);
		    DestroyDynamicObjectEx(rhSmokeObject[2]);
		    rhSmoke = 0;
		    format(donorstr, sizeof(donorstr), "You have disabled he smoke machine.", playerid);
		    SendClientMessage(playerid, 0xAA3333AA, donorstr);
        }
    }
    return 1;
}

CMD:kushdisco(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 21 || PlayerInfo[playerid][pID] == 3393 || PlayerInfo[playerid][pID] == 425) {
        if(!IsPlayerInRangeOfPoint(playerid, 5.0, 1271.1643, -1254.8700, 19380.2266)) return SendClientMessage(playerid, COLOR_GREY, "You are not near the control panel.");
        if(rhDisco == 0) {
            rhDiscoObject[0] = CreateDynamicObject(18102, 1267.82336, -1255.33398, 19386.00000,   0.00000, 0.00000, 358.81076);
            rhDiscoObject[1] = CreateDynamicObject(18657, 1265.56458, -1268.72351, 19438.66211,   270.00000, 0.00000, 0.00000);
			rhDiscoObject[2] = CreateDynamicObject(18656, 1269.28247, -1252.81738, 19438.95117,   270.00000, 0.00000, 0.00000);
            rhDiscoObject[3] = CreateDynamicObject(18658, 1260.43469, -1258.53320, 19424.74219,   270.00000, 0.00000, 0.00000);
            rhDisco = 1;

            format(donorstr, sizeof(donorstr), "You have enabled the disco.", playerid);
            SendClientMessage(playerid, 0x33AA33AA, donorstr);
        }
        else if(rhDisco == 1) {
	        DestroyDynamicObjectEx(rhDiscoObject[0]);
	        DestroyDynamicObjectEx(rhDiscoObject[1]);
	        DestroyDynamicObjectEx(rhDiscoObject[2]);
	        DestroyDynamicObjectEx(rhDiscoObject[3]);
	        rhDisco = 0;
            format(donorstr, sizeof(donorstr), "You have disabled the disco.", playerid);
            SendClientMessage(playerid, 0xAA3333AA, donorstr);
        }
    }
    return 1;
}

CMD:kushdjperm(playerid, params[]) {
    if(PlayerInfo[playerid][pID] == 21 || PlayerInfo[playerid][pID] == 5 || PlayerInfo[playerid][pID] == 3393 || PlayerInfo[playerid][pID] == 425) {
    	new	name[MAX_PLAYER_NAME + 1], givename[MAX_PLAYER_NAME + 1], playerb, Float:kPos[3];
    	GetPlayerPos(playerid, kPos[0], kPos[1], kPos[2]);
        if(sscanf(params, "u", playerb)) return SendClientMessage(playerid, COLOR_GREY, "USAGE: /kushdjperm [playerid/PartOfName]");
        if(!IsPlayerConnected(playerb)) return SendClientMessage(playerid, COLOR_GREY, "Invalid playerid.");
        if(!IsPlayerInRangeOfPoint(playerb, 2.0, kPos[0], kPos[1], kPos[2])) return SendClientMessage(playerid, -1, "That person is not near you.");
    	strmid(name, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
    	strmid(givename, PlayerICName(playerb), 0, MAX_PLAYER_NAME + 1);
        if(rHendersonDJ[playerb] == 0) {
            format(donorstr, sizeof(donorstr), "You have given %s DJ permission", givename);
            SendClientMessage(playerid, 0x33AA33AA, donorstr);

            rHendersonDJ[playerb] = 1;

            format(donorstr, sizeof(donorstr), "%s has given you DJ permission", name);
            SendClientMessage(playerb, 0x33AA33AA, donorstr);
        }
        else if(rHendersonDJ[playerb] == 1) {
            format(donorstr, sizeof(donorstr), "You have removed %s's DJ permission.", givename);
            SendClientMessage(playerid, 0xAA3333AA, donorstr);

            rHendersonDJ[playerb] = 0;

            format(donorstr, sizeof(donorstr), "%s has removed DJ permission", name);
            SendClientMessage(playerb, 0xAA3333AA, donorstr);
        }
    }
    return 1;
}

CMD:kushdj(playerid, params[]) {
    if(rHendersonDJ[playerid] == 0) return SendClientMessage(playerid, COLOR_GREY, "You are not a DJ!");
    if(!IsPlayerInRangeOfPoint(playerid, 5.0, 1271.4037, -1254.4858, 19380.4199)) return SendClientMessage(playerid, COLOR_GREY, "You are not near the DJ booth.");
    if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}USAGE:{FFFFFF} /kushdj [streamurl]");
    format(donorstr, sizeof(donorstr), "* You have set the stream url to %s", params);
    SendClientMessage(playerid, COLOR_LIGHTBLUE, donorstr);
    foreach(Player, i) { //update url
		if(IsPlayerInDynamicArea(i, KushDJArea) && !disabledRadios[playerid]) {
	    	usingRadio[playerid] = 0;
	    	PlayAudioStreamForPlayer(i, params);
	    	KushDJEnabledPlayer[i] = true;
            KushDJEnabled = 1;
        }
    }
    return 1;
}

CMD:kushbar(playerid, params[]) {
    if(!IsPlayerInRangeOfPoint(playerid, 5.0, 1267.3003, -1262.9395, 19379.3125)) return SendClientMessage(playerid, 0xAA3333AA, "You are not near the bar.");
    new string1[500], string2[500];
    format(string1, sizeof(string1), "Drink\tPrice\n {FFFFFF}%s\t {FFCA05}$%d\n{FFFFFF}%s\t {FFCA05}$%d\n{FFFFFF}%s\t {FFCA05}$%d\n{FFFFFF}%s\t {FFCA05}$%d\n{FFFFFF}%s\t {FFCA05}$%d\n", rhListItems[0][rhItem], rhListItems[0][rhPrice], rhListItems[1][rhItem], rhListItems[1][rhPrice], rhListItems[2][rhItem], rhListItems[2][rhPrice], rhListItems[3][rhItem], rhListItems[3][rhPrice], rhListItems[4][rhItem], rhListItems[4][rhPrice]);
    format(string2, sizeof(string2), "{FFFFFF}%s\t {FFCA05}$%d\n{FFFFFF}%s\t {FFCA05}$%d\n{FFFFFF}%s\t {FFCA05}$%d\n{FFFFFF}%s\t {FFCA05}$%d\n{FFFFFF}%s\t {FFCA05}$%d", rhListItems[5][rhItem], rhListItems[5][rhPrice], rhListItems[6][rhItem], rhListItems[6][rhPrice], rhListItems[7][rhItem], rhListItems[7][rhPrice], rhListItems[8][rhItem], rhListItems[8][rhPrice], rhListItems[9][rhItem], rhListItems[9][rhPrice]);
    strcat(string1, string2);
	ShowPlayerDialog(playerid, DIALOG_KUSH_BAR, DIALOG_STYLE_TABLIST_HEADERS, "           {FF00A2}The Kush Lounge", string1, "Buy", "Cancel");
    return 1;
}

CMD:kushlockvip(playerid, params[]) {
    if(PlayerInfo[playerid][pID] == 21 || PlayerInfo[playerid][pID] == 425) {
        if(rhVipLock == 0) {
			UpdateDynamic3DTextLabelText(VIPPrice, COLOR_WHITE, "{FFFFFF}VIP Entrance\nEntry Fee:({33AA33}OPEN{FFFFFF})");
            format(donorstr, sizeof(donorstr), "You have unlocked the VIP doors.");
            SendClientMessage(playerid, 0x33AA33AA, donorstr);
            rhVipLock = 1;
        }
        else if(rhVipLock == 1) {
			UpdateDynamic3DTextLabelText(VIPPrice, COLOR_WHITE, "{FFFFFF}VIP Entrance\nEntry Fee:({FF0000}CLOSED{FFFFFF})");
            format(donorstr, sizeof(donorstr), "You have locked the VIP doors.");
            SendClientMessage(playerid, 0xAA3333AA, donorstr);
            rhVipLock = 0;
        }
    }
    return 1;
}

CMD:lockkush(playerid, params[]) {
    if(PlayerInfo[playerid][pID] == 21 || PlayerInfo[playerid][pID] == 5 || PlayerInfo[playerid][pID] == 425) {
        if(rhDoorLock == 0) {
        	format(donorstr, sizeof(donorstr), "{FFFFFF}.:. {FF9900}Kush Lounge{FFFFFF} .:. \n{33AA33}OPEN{FFFFFF}\nEntry Fee: ($%d)", rhDoorPrice);
			UpdateDynamic3DTextLabelText(DoorPrice, COLOR_WHITE, donorstr);
            format(donorstr, sizeof(donorstr), "The Kush Lounge has been unlocked!");
            SendClientMessage(playerid, 0x33AA33AA, donorstr);
            rhDoorLock = 1;
        }
        else if(rhDoorLock == 1) {
			UpdateDynamic3DTextLabelText(DoorPrice, COLOR_WHITE, "{FFFFFF}.:. Kush Lounge .:.\nEntry Fee: \n({FF0000}CLOSED{FFFFFF})");
            format(donorstr, sizeof(donorstr), "The Kush Lounge has been locked!");
            SendClientMessage(playerid, 0xAA3333AA, donorstr);
            rhDoorLock = 0;
        }
    }
    return 1;
}

CMD:kushdoorprice(playerid, params[]) {
    if(PlayerInfo[playerid][pID] == 21 || PlayerInfo[playerid][pID] == 5 || PlayerInfo[playerid][pID] == 425) {
        new price;
        if(sscanf(params, "i", price))
			return SendClientMessage(playerid, -1, "/kushdoorprice [price]");
        if(price < 0 || price > 10000)
			return SendClientMessage(playerid, COLOR_WHITE, "   You can't set the door price for less than 0 or more than $10,000");
        else {
			rhDoorPrice = price;
			format(donorstr, sizeof(donorstr), "{FFFFFF}.:. {FF9900}Kush Lounge{FFFFFF} .:. \n{33AA33}Open{FFFFFF}\nEntry Fee: ($%d)", rhDoorPrice);
			UpdateDynamic3DTextLabelText(DoorPrice, COLOR_WHITE, donorstr);

            SendClientMessage(playerid, 0x6495EDFF, "Price Changed.");
            SaverhClub();
        }
    }
    return 1;
}

CMD:vipprice(playerid, params[]) {
    if(PlayerInfo[playerid][pID] == 21 || PlayerInfo[playerid][pID] == 425) {
        new price;
        if(sscanf(params, "i", price))
			return SendClientMessage(playerid, -1, "/vipprice [price]");
        if(price < 0 || price > 10000)
			return SendClientMessage(playerid, COLOR_WHITE, "   You can't set the door price for less than 0 or more than $10,000");
        else {
			rhVipPrice = price;
			format(donorstr, sizeof(donorstr), "{FFFFFF}VIP Entrance\nEntry Fee: {00AA00}$%d", rhVipPrice);
			UpdateDynamic3DTextLabelText(VIPPrice, COLOR_WHITE, donorstr);

            SendClientMessage(playerid, 0x6495EDFF, "Price Changed.");
            SaverhClub();
        }
    }
    return 1;
}

CMD:kushhelp(playerid, params[]) {
    if(PlayerInfo[playerid][pID] == 21 || PlayerInfo[playerid][pID] == 5 || PlayerInfo[playerid][pID] == 425) { //Robert Henderson, Dan West
    	SendClientMessage(playerid, COLOR_YELLOW,"_________________________________________________________");
        SendClientMessage(playerid, -1, "{FF3C3C}Norms: /mic, /kushdj, /kushbar");
        SendClientMessage(playerid, -1, "{FF3C3C}Kush Admin: /kushdjperm, /kushlockvip, /vipprice, /kushdisco");
        SendClientMessage(playerid, -1, "{FF3C3C}Kush Admin: /kushsmoke, /kushsafe, /kushbaredit, /kushbaredit");
        SendClientMessage(playerid, -1, "{FF3C3C}Kush Admin: /kushdoorprice, /lockkush");

    }
    else {
    	SendClientMessage(playerid, COLOR_YELLOW,"_________________________________________________________");
        SendClientMessage(playerid, -1, "{FF3C3C}/mic, /kushdj, /kushbar, /kushentryfee");
    }
    return 1;
}