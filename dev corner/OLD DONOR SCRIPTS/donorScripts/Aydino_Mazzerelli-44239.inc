/*
 ______              __                        
/\  _  \            /\ \  __                   
\ \ \L\ \  __  __   \_\ \/\_\    ___     ___   
 \ \  __ \/\ \/\ \  /'_` \/\ \ /' _ `\  / __`\ 
  \ \ \/\ \ \ \_\ \/\ \L\ \ \ \/\ \/\ \/\ \L\ \
   \ \_\ \_\/`____ \ \___,_\ \_\ \_\ \_\ \____/
    \/_/\/_/`/___/> \/__,_ /\/_/\/_/\/_/\/___/ 
               /\___/                          
               \/__/                           
                                                       ___    ___           
 /'\_/`\                                              /\_ \  /\_ \    __    
/\      \     __     ____    ____      __   _ __    __\//\ \ \//\ \  /\_\   
\ \ \__\ \  /'__`\  /\_ ,`\ /\_ ,`\  /'__`\/\`'__\/'__`\\ \ \  \ \ \ \/\ \  
 \ \ \_/\ \/\ \L\.\_\/_/  /_\/_/  /_/\  __/\ \ \//\  __/ \_\ \_ \_\ \_\ \ \ 
  \ \_\\ \_\ \__/.\_\ /\____\ /\____\ \____\\ \_\\ \____\/\____\/\____\\ \_\
   \/_/ \/_/\/__/\/_/ \/____/ \/____/\/____/ \/_/ \/____/\/____/\/____/ \/_/

   
	UNKNOWN DEVELOPMENT RERQUEST
*/

new aydinosafeWeapon[10];
new aydinosafeCash;
new aydinosafePot;
new aydinosafeCrack;
new aydinosafeMats;
new aydinokey[MAX_PLAYERS];

stock SaveAydino() {
	new file[25];
	format(file, sizeof(file), "aydinosafe.cfg");
    if(!dini_Exists(file)) dini_Create(file);

    dini_IntSet(file, "Weapon1", aydinosafeWeapon[0]);
    dini_IntSet(file, "Weapon2", aydinosafeWeapon[1]);
    dini_IntSet(file, "Weapon3", aydinosafeWeapon[2]);
    dini_IntSet(file, "Weapon4", aydinosafeWeapon[3]);
    dini_IntSet(file, "Weapon5", aydinosafeWeapon[4]);
    dini_IntSet(file, "Weapon6", aydinosafeWeapon[5]);
    dini_IntSet(file, "Weapon7", aydinosafeWeapon[6]);
    dini_IntSet(file, "Weapon8", aydinosafeWeapon[7]);
    dini_IntSet(file, "Weapon9", aydinosafeWeapon[8]);
    dini_IntSet(file, "Weapon10", aydinosafeWeapon[9]);

    dini_IntSet(file, "Cash", aydinosafeCash);
    dini_IntSet(file, "Pot", aydinosafePot);
    dini_IntSet(file, "Crack", aydinosafeCrack);
    dini_IntSet(file, "Mats", aydinosafeMats);
    return 1;
}

stock LoadAydino() {
    new file[25];
    format(file, sizeof(file), "aydinosafe.cfg");
    if(!dini_Exists(file)) dini_Create(file);

    aydinosafeWeapon[0] = dini_Int(file, "Weapon1");
    aydinosafeWeapon[1] = dini_Int(file, "Weapon2");
    aydinosafeWeapon[2] = dini_Int(file, "Weapon3");
    aydinosafeWeapon[3] = dini_Int(file, "Weapon4");
    aydinosafeWeapon[4] = dini_Int(file, "Weapon5");
    aydinosafeWeapon[5] = dini_Int(file, "Weapon6");
    aydinosafeWeapon[6] = dini_Int(file, "Weapon7");
    aydinosafeWeapon[7] = dini_Int(file, "Weapon8");
    aydinosafeWeapon[8] = dini_Int(file, "Weapon9");
    aydinosafeWeapon[9] = dini_Int(file, "Weapon10");

    aydinosafeCash = dini_Int(file, "Cash");
    aydinosafePot = dini_Int(file, "Pot");
    aydinosafeCrack = dini_Int(file, "Crack");
    aydinosafeMats = dini_Int(file, "Mats");
    return 1;
}

Hook:AydinoMazzerelli_OnGameModeInit() {
	LoadAydino();
	CreateDynamic3DTextLabel("{FF0000}Vehicle repair\n{FF0000}(/aydinofix)", COLOR_LIGHTBLUE, 2258.4585,1464.5507,9002.3848, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1);
	CreateDynamic3DTextLabel("(( /buysoda ))", -1, 1293.9017,-1318.8225,18.9866, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1);
	return 1;
}

Hook:AydinoMazzerelli_OnGameModeExit() {
	SaveAydino();
	return 1;
}

Hook:AydinoMazzerelli_OnPlayerConnect(playerid) {
	aydinokey[playerid] = 0;
	return 1;
}

CMD:aydinokey(playerid, params[]) {
	if(PlayerInfo[playerid][pID] != 44239) return SendClientMessage(playerid, COLOR_GREY, "You cannot do this.");
	new playerb;
    if(sscanf(params, "u", playerb)) return SendClientMessage(playerid, COLOR_GREY, "USAGE: /aydinokey [playerid/PartOfName]");
    if(!IsPlayerConnected(playerb)) return SendClientMessage(playerid, COLOR_GREY, "Unknown player.");
    if(playerb == playerid) return SendClientMessage(playerid, COLOR_GREY, "You already have the key.");
    
    new Float:ayx, Float:ayy, Float:ayz;
    GetPlayerPos(playerb, ayx, ayy, ayz);
    if(!IsPlayerInRangeOfPoint(playerid, 4.0, ayx, ayy, ayz)) return SendClientMessage(playerid, COLOR_GREY, "You are not near that player.");

    if(aydinokey[playerb] == 0) {
    	aydinokey[playerb] = 1;
    	SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have given that player the key to the Aydino Safe.");
    	SendClientMessage(playerb, COLOR_LIGHTBLUE, "* You have received the key to the Aydino Safe.");
    }
    else {
    	aydinokey[playerb] = 0;
    	SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have removed their key to the Aydino Safe.");
    	SendClientMessage(playerb, COLOR_LIGHTBLUE, "* You no longer have access to the Aydino Safe.");
    }
    return 1;
}

CMD:aydinofix(playerid, params[]) {
	if(!IsPlayerInRangeOfPoint(playerid, 3.0, 2258.4585,1464.5507,9002.3848)) return SendClientMessage(playerid, COLOR_GREY, "You are not near the Aydino repair station.");
	if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You are not in a vehicle!");
    if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendClientMessage(playerid, COLOR_GREY, "You are not the driver!");
	if(PlayerInfo[playerid][pCash] < 500) return SendClientMessage(playerid, COLOR_GREY, "You can't afford that! ($500)");
	GiveMoney(playerid, -500);
	SendClientMessage(playerid, COLOR_GREEN, "Your vehicle was repaired for $500.");
	RepairVehicle(GetPlayerVehicleID(playerid));
	SetVehicleHealth(GetPlayerVehicleID(playerid), 1000);
	return 1;
}

CMD:aydinosafe(playerid, params[]) {
    if(PlayerInfo[playerid][pID] == 44239 || aydinokey[playerid] == 1) {
   		new option[32], amount;
    	if(!IsPlayerInRangeOfPoint(playerid, 2.0, 1295.7848,-1330.9055,18.9866)) return SendClientMessage(playerid, -1, "You're not near the safe.");
        if(sscanf(params,"s[32]", params)) {
            SendClientMessage(playerid, -1, "/aydinosafe [type]");
            SendClientMessage(playerid, 0xAFAFAFAA, "Type: balance | withdraw | deposit");
            return 1;
        }
        if(!strcmp(params, "balance",true,7)) {
            new string2[500];
            format(donorstr, sizeof(donorstr), "{FFFFFF}Cash: {FFCA05}%d \n{FFFFFF}Pot: {FFCA05}%d \n{FFFFFF}Crack: {FFCA05}%d \n{FFFFFF}Materials: {FFCA05}%d \n", aydinosafeCash, aydinosafePot, aydinosafeCrack, aydinosafeMats);
            strcat(donorstr, "{FFFFFF}Weapon Slot 0: {FFCA05}%s\n{FFFFFF}Weapon Slot 1: {FFCA05}%s\n{FFFFFF}Weapon Slot 2: {FFCA05}%s\n");
            strcat(donorstr, "{FFFFFF}Weapon Slot 3: {FFCA05}%s\n{FFFFFF}Weapon Slot 4: {FFCA05}%s\n{FFFFFF}Weapon Slot 5:{FFCA05} %s \n");
            strcat(donorstr, "{FFFFFF}Weapon Slot 6: {FFCA05}%s\n{FFFFFF}Weapon Slot 7: {FFCA05}%s\n{FFFFFF}Weapon Slot 8: {FFCA05}%s \n{FFFFFF}Weapon Slot 9: {FFCA05}%s");
            format(string2, sizeof(string2), donorstr,
            dGetWeaponName(aydinosafeWeapon[0]), dGetWeaponName(aydinosafeWeapon[1]), dGetWeaponName(aydinosafeWeapon[2]), dGetWeaponName(aydinosafeWeapon[3]),
            dGetWeaponName(aydinosafeWeapon[4]), dGetWeaponName(aydinosafeWeapon[5]), dGetWeaponName(aydinosafeWeapon[6]), dGetWeaponName(aydinosafeWeapon[7]),
            dGetWeaponName(aydinosafeWeapon[8]), dGetWeaponName(aydinosafeWeapon[9]));

            ShowPlayerDialog(playerid, 21351, DIALOG_STYLE_MSGBOX, "                {FF00A2} Aydino Safe", string2, "Close", "");
            return 1;
        }
        else if(!strcmp(params, "withdraw",true,8)) { //negative values okay, makes safe positive and player negative
            if(sscanf(params,"s[32]s[32]i", params, option, amount)) {
                SendClientMessage(playerid, -1, "/aydinosafe withdraw [option] [amount]");
                SendClientMessage(playerid, 0xAFAFAFAA, "Option: cash | pot | crack | mats | weapon(amount == slot || 0 - 9 slots)");
                return 1;
            }
            if(!strcmp(option, "cash",true,4)) {
                if(amount > aydinosafeCash) return SendClientMessage(playerid, 0xAA3333AA, "The safe doesn't have that amount of money.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "Choose a correct amount.");
				aydinosafeCash -= amount;
                GiveMoney(playerid, amount);
                SaveAydino();

                SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have withdrawn some cash.");
            }
            else if(!strcmp(option, "pot",true,3)) {
                if(amount > aydinosafePot) return SendClientMessage(playerid, 0xAA3333AA, "The safe doesn't have that amount of pot.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "Choose a correct amount.");
				aydinosafePot -= amount;
                PlayerInfo[playerid][pPot] += amount;
				SaveAydino();

				SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have withdrawn some pot.");
            }
            else if(!strcmp(option, "crack",true, 5)) {
                if(amount > aydinosafeCrack) return SendClientMessage(playerid, 0xAA3333AA, "The safe doesn't have that amount of crack.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "Choose a correct amount.");
				aydinosafeCrack -= amount;
                PlayerInfo[playerid][pCrack] += amount;
                SaveAydino();

                SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have withdrawn some crack.");
            }
            else if(!strcmp(option, "mats",true, 5)) {
                if(amount > aydinosafeMats) return SendClientMessage(playerid, 0xAA3333AA, "The safe doesn't have that amount of materials.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "Choose a correct amount.");
				aydinosafeMats -= amount;
                PlayerInfo[playerid][pMats] += amount;
                SaveAydino();

                SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have withdrawn some materials.");
            }
            else if(!strcmp(option, "weapon",true, 6)) {
                new slot = amount, weaponid = aydinosafeWeapon[slot];
                if(aydinosafeWeapon[slot] == 0) return SendClientMessage(playerid, 0xAA3333AA, "There is no weapon in that slot.");
                GivePlayerGun(playerid, weaponid);
                aydinosafeWeapon[slot] = 0;
                SaveAydino();

                SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have withdrawn a weapon.");
            }
        }
        else if(!strcmp(params, "deposit",true,7)) {
            if(sscanf(params,"s[32]s[32]i", params, option, amount)) {
                SendClientMessage(playerid, -1, "/aydinosafe deposit [option] [amount]");
                SendClientMessage(playerid, 0xAFAFAFAA, "Option: cash | pot | crack | materials | weapon(amount == slot || 0 - 9 slots)");
                return 1;
            }
            if(!strcmp(option, "cash",true,4)) {
                if(amount > PlayerInfo[playerid][pCash]) return SendClientMessage(playerid, 0xAA3333AA, "You don't have enough money.");
                if(aydinosafeCash > 10000000) return SendClientMessage(playerid, 0xAA3333AA, "The safe is full of money.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "Choose a correct amount.");
				aydinosafeCash += amount;
                GiveMoney(playerid, -amount);
				SaveAydino();

				SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have deposited some cash.");
            }
            else if(!strcmp(option, "pot",true,3)) {
                if(amount > PlayerInfo[playerid][pPot]) return SendClientMessage(playerid, 0xAA3333AA, "You don't have that amount of pot.");
                if(amount >= 1000) return SendClientMessage(playerid, 0xAA3333AA, "This safe can only hold 1000 grams of pot.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "Choose a correct amount.");
				aydinosafePot += amount;
                PlayerInfo[playerid][pPot] -= amount;
    			SaveAydino();

    			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have deposited some pot.");
            }
            else if(!strcmp(option, "crack",true,5)) {
                if(amount > PlayerInfo[playerid][pCrack]) return SendClientMessage(playerid, 0xAA3333AA, "You don't have that amount of crack.");
                if(amount >= 1000) return SendClientMessage(playerid, 0xAA3333AA, "This safe can only hold 1000 grams of crack.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "Choose a correct amount.");
				aydinosafeCrack += amount;
                PlayerInfo[playerid][pCrack] -= amount;
                SaveAydino();

                SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have deposited some crack.");
            }
            else if(!strcmp(option, "mats",true,5)) {
                if(amount > PlayerInfo[playerid][pMats]) return SendClientMessage(playerid, 0xAA3333AA, "You don't have that amount of materials.");
                if(amount >= 500000) return SendClientMessage(playerid, 0xAA3333AA, "This safe can only hold 500,000 materials.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "Choose a correct amount.");
				aydinosafeMats += amount;
                PlayerInfo[playerid][pMats] -= amount;
                SaveAydino();

                SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have deposited some materials.");
            }
            else if(!strcmp(option, "weapon",true,6)) {
                new slot = amount, gunid = GetPlayerWeapon(playerid);
                if(gunid == 0) return SendClientMessage(playerid, 0xAA3333A, "You're not holding a weapon.");
                if(aydinosafeWeapon[slot] != 0) return SendClientMessage(playerid, 0xAA3333AA, "There is a weapon in this slot.");
                aydinosafeWeapon[slot] = GetPlayerWeapon(playerid);
                TakePlayerWeapon(playerid, gunid);
                SaveAydino();

                SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have deposited a weapon.");
            }
        }
    }
    else return SendClientMessage(playerid, COLOR_GREY, "You cannot access the safe.");
	return 1;
}