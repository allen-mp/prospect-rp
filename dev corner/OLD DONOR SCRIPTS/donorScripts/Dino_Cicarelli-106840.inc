/*
 ____                          
/\  _`\   __                   
\ \ \/\ \/\_\    ___     ___   
 \ \ \ \ \/\ \ /' _ `\  / __`\ 
  \ \ \_\ \ \ \/\ \/\ \/\ \L\ \
   \ \____/\ \_\ \_\ \_\ \____/
    \/___/  \/_/\/_/\/_/\/___/ 
                               
                               
 ____                                       ___    ___           
/\  _`\    __                              /\_ \  /\_ \    __    
\ \ \/\_\ /\_\    ___     __     _ __    __\//\ \ \//\ \  /\_\   
 \ \ \/_/_\/\ \  /'___\ /'__`\  /\`'__\/'__`\\ \ \  \ \ \ \/\ \  
  \ \ \L\ \\ \ \/\ \__//\ \L\.\_\ \ \//\  __/ \_\ \_ \_\ \_\ \ \ 
   \ \____/ \ \_\ \____\ \__/.\_\\ \_\\ \____\/\____\/\____\\ \_\
    \/___/   \/_/\/____/\/__/\/_/ \/_/ \/____/\/____/\/____/ \/_/


	UNKNOWN DEVELOPMENT RERQUEST
*/

new dinosafeWeapon[10];
new dinosafeCash;
new dinosafePot;
new dinosafeCrack;
new dinosafeMats;

stock SaveDino() {
	new file[25];
	format(file, sizeof(file), "dinosafe.cfg");
    if(!dini_Exists(file)) dini_Create(file);

    dini_IntSet(file, "Weapon1", dinosafeWeapon[0]);
    dini_IntSet(file, "Weapon2", dinosafeWeapon[1]);
    dini_IntSet(file, "Weapon3", dinosafeWeapon[2]);
    dini_IntSet(file, "Weapon4", dinosafeWeapon[3]);
    dini_IntSet(file, "Weapon5", dinosafeWeapon[4]);
    dini_IntSet(file, "Weapon6", dinosafeWeapon[5]);
    dini_IntSet(file, "Weapon7", dinosafeWeapon[6]);
    dini_IntSet(file, "Weapon8", dinosafeWeapon[7]);
    dini_IntSet(file, "Weapon9", dinosafeWeapon[8]);
    dini_IntSet(file, "Weapon10", dinosafeWeapon[9]);

    dini_IntSet(file, "Cash", dinosafeCash);
    dini_IntSet(file, "Pot", dinosafePot);
    dini_IntSet(file, "Crack", dinosafeCrack);
    dini_IntSet(file, "Mats", dinosafeMats);
    return 1;
}

stock LoadDino() {
    new file[25];
    format(file, sizeof(file), "dinosafe.cfg");
    if(!dini_Exists(file)) dini_Create(file);

    dinosafeWeapon[0] = dini_Int(file, "Weapon1");
    dinosafeWeapon[1] = dini_Int(file, "Weapon2");
    dinosafeWeapon[2] = dini_Int(file, "Weapon3");
    dinosafeWeapon[3] = dini_Int(file, "Weapon4");
    dinosafeWeapon[4] = dini_Int(file, "Weapon5");
    dinosafeWeapon[5] = dini_Int(file, "Weapon6");
    dinosafeWeapon[6] = dini_Int(file, "Weapon7");
    dinosafeWeapon[7] = dini_Int(file, "Weapon8");
    dinosafeWeapon[8] = dini_Int(file, "Weapon9");
    dinosafeWeapon[9] = dini_Int(file, "Weapon10");

    dinosafeCash = dini_Int(file, "Cash");
    dinosafePot = dini_Int(file, "Pot");
    dinosafeCrack = dini_Int(file, "Crack");
    dinosafeMats = dini_Int(file, "Mats");
    return 1;
}

Hook:DinoCicarelli_OnGameModeInit() {
	LoadDino();
    AddStaticVehicleEx(579, 1004.6097, -2130.2266, 13.0265, 175.8602, -1, -1, -1);
    AddStaticVehicleEx(579, 1000.6507, -2130.3005, 13.0265, 179.1001, -1, -1, -1);
    AddStaticVehicleEx(487, 958.3521, -2133.7524, 13.5474, -52.3200, -1, -1, -1);
	return 1;
}

CMD:dinosafe(playerid, params[]) {
    if(PlayerInfo[playerid][pID] == 93337) {
   		new option[32], amount;
    	if(!IsPlayerInRangeOfPoint(playerid, 2.0, 986.92847, -2151.49316, 13.59967)) return SendClientMessage(playerid, -1, "You're not near the safe.");
        if(sscanf(params,"s[32]", params)) {
            SendClientMessage(playerid, -1, "/dinosafe [type]");
            SendClientMessage(playerid, 0xAFAFAFAA, "Type: balance | withdraw | deposit");
            return 1;
        }
        if(!strcmp(params, "balance",true,7)) {
            new string2[500];
            format(donorstr, sizeof(donorstr), "{FFFFFF}Cash: {FFCA05}%d \n{FFFFFF}Pot: {FFCA05}%d \n{FFFFFF}Crack: {FFCA05}%d \n{FFFFFF}Materials: {FFCA05}%d \n", dinosafeCash, dinosafePot, dinosafeCrack, dinosafeMats);
            strcat(donorstr, "{FFFFFF}Weapon Slot 0: {FFCA05}%s\n{FFFFFF}Weapon Slot 1: {FFCA05}%s\n{FFFFFF}Weapon Slot 2: {FFCA05}%s\n");
            strcat(donorstr, "{FFFFFF}Weapon Slot 3: {FFCA05}%s\n{FFFFFF}Weapon Slot 4: {FFCA05}%s\n{FFFFFF}Weapon Slot 5:{FFCA05} %s \n");
            strcat(donorstr, "{FFFFFF}Weapon Slot 6: {FFCA05}%s\n{FFFFFF}Weapon Slot 7: {FFCA05}%s\n{FFFFFF}Weapon Slot 8: {FFCA05}%s \n{FFFFFF}Weapon Slot 9: {FFCA05}%s");
            format(string2, sizeof(string2), donorstr,
            dGetWeaponName(dinosafeWeapon[0]), dGetWeaponName(dinosafeWeapon[1]), dGetWeaponName(dinosafeWeapon[2]), dGetWeaponName(dinosafeWeapon[3]),
            dGetWeaponName(dinosafeWeapon[4]), dGetWeaponName(dinosafeWeapon[5]), dGetWeaponName(dinosafeWeapon[6]), dGetWeaponName(dinosafeWeapon[7]),
            dGetWeaponName(dinosafeWeapon[8]), dGetWeaponName(dinosafeWeapon[9]));

            ShowPlayerDialog(playerid, 21351, DIALOG_STYLE_MSGBOX, "                {FF00A2} The Safe", string2, "Close", "");
            return 1;
        }
        else if(!strcmp(params, "withdraw",true,8)) {//negative values okay, makes safe positive and player negative
            if(sscanf(params,"s[32]s[32]i", params, option, amount)) {
                SendClientMessage(playerid, -1, "/dinosafe withdraw [option] [amount]");
                SendClientMessage(playerid, 0xAFAFAFAA, "Option: cash | pot | crack | mats | weapon(amount == slot || 0 - 9 slots)");
                return 1;
            }
            if(!strcmp(option, "cash",true,4)) {
                if(amount > dinosafeCash) return SendClientMessage(playerid, 0xAA3333AA, "The safe doesn't have that amount of money.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "You withdraw no money.");
				dinosafeCash -= amount;
                GiveMoney(playerid, amount);
                SaveDino();
            }
            else if(!strcmp(option, "pot",true,3)) {
                if(amount > dinosafePot) return SendClientMessage(playerid, 0xAA3333AA, "The safe doesn't have that amount of pot.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "You withdraw no grams of pot.");
				dinosafePot -= amount;
                PlayerInfo[playerid][pPot] += amount;
				SaveDino();
            }
            else if(!strcmp(option, "crack",true, 5)) {
                if(amount > dinosafeCrack) return SendClientMessage(playerid, 0xAA3333AA, "The safe doesn't have that amount of crack.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "You withdraw no grams of crack.");
				dinosafeCrack -= amount;
                PlayerInfo[playerid][pCrack] += amount;
                SaveDino();
            }
            else if(!strcmp(option, "mats",true, 5)) {
                if(amount > dinosafeMats) return SendClientMessage(playerid, 0xAA3333AA, "The safe doesn't have that amount of materials.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "You withdraw no materials.");
				dinosafeMats -= amount;
                PlayerInfo[playerid][pMats] += amount;
                SaveDino();
            }
            else if(!strcmp(option, "weapon",true, 6)) {
                new slot = amount, weaponid = dinosafeWeapon[slot];
                if(dinosafeWeapon[slot] == 0) return SendClientMessage(playerid, 0xAA3333AA, "There is no weapon in that slot.");
                GivePlayerGun(playerid, weaponid);
                dinosafeWeapon[slot] = 0;
                SaveDino();
            }
        }
        else if(!strcmp(params, "deposit",true,7)) {
            if(sscanf(params,"s[32]s[32]i", params, option, amount)) {
                SendClientMessage(playerid, -1, "/dinosafe deposit [option] [amount]");
                SendClientMessage(playerid, 0xAFAFAFAA, "Option: cash | pot | crack | materials | weapon(amount == slot || 0 - 9 slots)");
                return 1;
            }
            if(!strcmp(option, "cash",true,4)) {
                if(dinosafeCash > 10000000 ||  amount > PlayerInfo[playerid][pCash]) return SendClientMessage(playerid, 0xAA3333AA, "You either don't have the amount of cash or the safe is full.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "You deposit no money.");
				dinosafeCash += amount;
                GiveMoney(playerid, -amount);
				SaveDino();
            }
            else if(!strcmp(option, "pot",true,3)) {
                if(amount > PlayerInfo[playerid][pPot]) return SendClientMessage(playerid, 0xAA3333AA, "You don't have that amount of pot.");
                if(amount >= 1000) return SendClientMessage(playerid, 0xAA3333AA, "This safe can only hold 1000 grams of pot.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "You deposit no grams of pot.");
				dinosafePot += amount;
                PlayerInfo[playerid][pPot] -= amount;
    			SaveDino();
            }
            else if(!strcmp(option, "crack",true,5)) {
                if(amount > PlayerInfo[playerid][pCrack]) return SendClientMessage(playerid, 0xAA3333AA, "You don't have that amount of crack.");
                if(amount >= 1000) return SendClientMessage(playerid, 0xAA3333AA, "This safe can only hold 1000 grams of crack.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "You deposit no grams of crack.");
				dinosafeCrack += amount;
                PlayerInfo[playerid][pCrack] -= amount;
                SaveDino();
            }
            else if(!strcmp(option, "mats",true,5)) {
                if(amount > PlayerInfo[playerid][pMats]) return SendClientMessage(playerid, 0xAA3333AA, "You don't have that amount of materials.");
                if(amount >= 500000) return SendClientMessage(playerid, 0xAA3333AA, "This safe can only hold 500,000 materials.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "You deposit no materials.");
				dinosafeMats += amount;
                PlayerInfo[playerid][pMats] -= amount;
                SaveDino();
            }
            else if(!strcmp(option, "weapon",true,6)) {
                new slot = amount, gunid = GetPlayerWeapon(playerid);
                if(gunid == 0) return SendClientMessage(playerid, 0xAA3333A, "You're not holding a weapon");
                if(HaveAdminWeapon(playerid, gunid) == gunid) return SendClientMessage(playerid, COLOR_GREY, "You cannot store this weapon.");
                if(dinosafeWeapon[slot] != 0) return SendClientMessage(playerid, 0xAA3333AA, "There is a weapon in this slot.");
                dinosafeWeapon[slot] = GetPlayerWeapon(playerid);
                TakePlayerWeapon(playerid, gunid);
                SaveDino();
            }
        }
    }
	return 1;
}