/*
 ____                                              
/\  _`\                                            
\ \ \L\_\  __  __    ___     ___      __     _ __  
 \ \ \L_L /\ \/\ \ /' _ `\ /' _ `\  /'__`\  /\`'__\
  \ \ \/, \ \ \_\ \/\ \/\ \/\ \/\ \/\ \L\.\_\ \ \/ 
   \ \____/\ \____/\ \_\ \_\ \_\ \_\ \__/.\_\\ \_\ 
    \/___/  \/___/  \/_/\/_/\/_/\/_/\/__/\/_/ \/_/ 
                                                   
                                                   
 __      __            __                                     
/\ \  __/\ \          /\ \__                                  
\ \ \/\ \ \ \     __  \ \ ,_\   ____    ___     ___      __   
 \ \ \ \ \ \ \  /'__`\ \ \ \/  /',__\  / __`\ /' _ `\  /'__`\ 
  \ \ \_/ \_\ \/\ \L\.\_\ \ \_/\__, `\/\ \L\ \/\ \/\ \/\  __/ 
   \ `\___x___/\ \__/.\_\\ \__\/\____/\ \____/\ \_\ \_\ \____\
    '\/__//__/  \/__/\/_/ \/__/\/___/  \/___/  \/_/\/_/\/____/

	
	http://www.gta-sarp.com/forums/showthread.php?227561-Development-Related-Gunnar_Watsone-s-Request&highlight=gunnar_Watsone
*/
	
#define GUNNARWATSONE_ID    21900

new gunnarsafeWeapon[10];
new gunnarsafeCash;
new gunnarsafePot;
new gunnarsafeCrack;
new gunnarsafeMats;
new gunnarNote1[200];
new gunnarNote2[200];
new gunnarNote3[200];
new gunnarNote4[200];
new gunnarNote5[200];
new gunnarKey[MAX_PLAYERS];
new gunnarOldSkin[MAX_PLAYERS];

stock CanGunnar(playerid) {
	if(gunnarKey[playerid] == 1 || PlayerInfo[playerid][pID] == GUNNARWATSONE_ID) return 1;
	return 0;
}

stock GetGunnarWeaponName(weaponid) {
    new wepname[32];
    GetWeaponName(weaponid, wepname, sizeof(wepname));
    if(weaponid == 0) format(wepname, sizeof(wepname), "Empty");
    return wepname;
}

stock LoadGunnar() {
    new file[25];
    format(file, sizeof(file), "gunnarwatsone.cfg");
    if(!dini_Exists(file)) dini_Create(file);

    gunnarsafeWeapon[0] = dini_Int(file, "Weapon1");
    gunnarsafeWeapon[1] = dini_Int(file, "Weapon2");
    gunnarsafeWeapon[2] = dini_Int(file, "Weapon3");
    gunnarsafeWeapon[3] = dini_Int(file, "Weapon4");
    gunnarsafeWeapon[4] = dini_Int(file, "Weapon5");
    gunnarsafeWeapon[5] = dini_Int(file, "Weapon6");
    gunnarsafeWeapon[6] = dini_Int(file, "Weapon7");
    gunnarsafeWeapon[7] = dini_Int(file, "Weapon8");
    gunnarsafeWeapon[8] = dini_Int(file, "Weapon9");
    gunnarsafeWeapon[9] = dini_Int(file, "Weapon10");

    gunnarsafeCash = dini_Int(file, "Cash");
    gunnarsafePot = dini_Int(file, "Pot");
    gunnarsafeCrack = dini_Int(file, "Crack");
    gunnarsafeMats = dini_Int(file, "Mats");
    
    format(gunnarNote1, sizeof(gunnarNote1), "%s", dini_Get(file, "Note1"));
    format(gunnarNote2, sizeof(gunnarNote2), "%s", dini_Get(file, "Note2"));
    format(gunnarNote3, sizeof(gunnarNote3), "%s", dini_Get(file, "Note3"));
    format(gunnarNote4, sizeof(gunnarNote4), "%s", dini_Get(file, "Note4"));
    format(gunnarNote5, sizeof(gunnarNote5), "%s", dini_Get(file, "Note5"));
    return 1;
}

stock SaveGunnar() {
	new file[25];
	format(file, sizeof(file), "gunnarwatsone.cfg");
    if(!dini_Exists(file)) dini_Create(file);

    dini_IntSet(file, "Weapon1", gunnarsafeWeapon[0]);
    dini_IntSet(file, "Weapon2", gunnarsafeWeapon[1]);
    dini_IntSet(file, "Weapon3", gunnarsafeWeapon[2]);
    dini_IntSet(file, "Weapon4", gunnarsafeWeapon[3]);
    dini_IntSet(file, "Weapon5", gunnarsafeWeapon[4]);
    dini_IntSet(file, "Weapon6", gunnarsafeWeapon[5]);
    dini_IntSet(file, "Weapon7", gunnarsafeWeapon[6]);
    dini_IntSet(file, "Weapon8", gunnarsafeWeapon[7]);
    dini_IntSet(file, "Weapon9", gunnarsafeWeapon[8]);
    dini_IntSet(file, "Weapon10", gunnarsafeWeapon[9]);

    dini_IntSet(file, "Cash", gunnarsafeCash);
    dini_IntSet(file, "Pot", gunnarsafePot);
    dini_IntSet(file, "Crack", gunnarsafeCrack);
    dini_IntSet(file, "Mats", gunnarsafeMats);
    
    dini_Set(file, "Note1", gunnarNote1);
    dini_Set(file, "Note2", gunnarNote2);
    dini_Set(file, "Note3", gunnarNote3);
    dini_Set(file, "Note4", gunnarNote4);
    dini_Set(file, "Note5", gunnarNote5);
    return 1;
}

Hook:GunnarWatsone_OnGameModeInit() {
	LoadGunnar();
	CreateDynamic3DTextLabel("{90C3D4}/chalkboard", -1, 2213.5015,550.0428,9096.8848, 10, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1);
	CreateDynamic3DTextLabel("{90C3D4}/gunnarsafe", -1, 2248.0896,556.7626,9097.7354, 5, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1);
	CreateDynamic3DTextLabel("{90C3D4}/cagefighter", -1, 2218.1853,604.1145,9097.7393, 5, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1);
	CreateDynamic3DTextLabel("{90C3D4}/gymwater", -1, 2214.8909,596.8708,9097.7402, 5, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1);
	return 1;
}

Hook:GunnarWatsone_OnGameModeExit() {
	SaveGunnar();
	return 1;
}

Hook:GunnarWatsone_OnPlayerConnect(playerid) {
	gunnarKey[playerid] = 0;
	gunnarOldSkin[playerid] = 0;
	return 1;
}

CMD:gymwater(playerid, params[]) {
	if(!IsPlayerInRangeOfPoint(playerid, 5.0, 2214.8909,596.8708,9097.7402)) return SendClientMessage(playerid, COLOR_GREY, "You are not in the gym.");
	
	new Float:gunnar_HP;
	GetPlayerHealth(playerid, gunnar_HP);
	if(gunnar_HP + 25 < 101) {
	    SetPlayerHealth(playerid, gunnar_HP + 25);
	}
	else {
	    SetPlayerHealth(playerid, 100);
	}
	GameTextForPlayer(playerid, "~w~You drink some ~b~water.", 3000, 3);
	return 1;
}

CMD:cagefighter(playerid, params[]) {
	if(!IsPlayerInRangeOfPoint(playerid, 5.0, 2218.1853,604.1145,9097.7393)) return SendClientMessage(playerid, COLOR_GREY, "You are not near the lockers.");
	if(GetPlayerSkin(playerid) == 80 || GetPlayerSkin(playerid) == 81) {
	    SetPlayerSkin(playerid, gunnarOldSkin[playerid]);
	}
	else {
	    gunnarOldSkin[playerid] = GetPlayerSkin(playerid);
	    new skinPick = random(2);
	    if(skinPick == 0) {
	    	SetPlayerSkin(playerid, 80);
		}
		else {
		    SetPlayerSkin(playerid, 81);
		}
	}
	return 1;
}

CMD:gunnarsafe(playerid, params[]) {
	if(!CanGunnar(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You cannot do this.");
	if(!IsPlayerInRangeOfPoint(playerid, 5.0, 2248.0896,556.7626,9097.7354)) return SendClientMessage(playerid, COLOR_GREY, "You are not near the safe.");
    new board_Op[10];
	if(sscanf(params, "s[10]", board_Op)) return SendClientMessage(playerid, COLOR_GREY, "USAGE: /gunnarsafe [check/withdraw/deposit]");
	
	new gunnar_Item[10], gunnar_Amount;
	if(!strcmp(board_Op, "check",true,5)) {
 	    new gunnar_Str[440];
 	    format(gunnar_Str, sizeof(gunnar_Str), "{90C3D4}Cash:{FFFFFF} $%i\n{90C3D4}Crack:{FFFFFF} %i\n{90C3D4}Pot:{FFFFFF} %i\n{90C3D4}Materials:{FFFFFF} %i", gunnarsafeCash, gunnarsafeCrack, gunnarsafePot, gunnarsafeMats);
 	    format(gunnar_Str, sizeof(gunnar_Str), "%s\n{90C3D4}Weapon 0:{FFFFFF} %s\n{90C3D4}Weapon 1:{FFFFFF} %s\n{90C3D4}Weapon 2:{FFFFFF} %s\n{90C3D4}Weapon 3:{FFFFFF} %s\n{90C3D4}Weapon 4:{FFFFFF} %s", gunnar_Str, GetGunnarWeaponName(gunnarsafeWeapon[0]), GetGunnarWeaponName(gunnarsafeWeapon[1]), GetGunnarWeaponName(gunnarsafeWeapon[2]), GetGunnarWeaponName(gunnarsafeWeapon[3]), GetGunnarWeaponName(gunnarsafeWeapon[4]));
        format(gunnar_Str, sizeof(gunnar_Str), "%s\n{90C3D4}Weapon 5:{FFFFFF} %s\n{90C3D4}Weapon 6:{FFFFFF} %s\n{90C3D4}Weapon 7:{FFFFFF} %s\n{90C3D4}Weapon 8:{FFFFFF} %s\n{90C3D4}Weapon 9:{FFFFFF} %s", gunnar_Str, GetGunnarWeaponName(gunnarsafeWeapon[5]), GetGunnarWeaponName(gunnarsafeWeapon[6]), GetGunnarWeaponName(gunnarsafeWeapon[7]), GetGunnarWeaponName(gunnarsafeWeapon[8]), GetGunnarWeaponName(gunnarsafeWeapon[9]));
		ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Safe", gunnar_Str, "Close", "");
 	}
 	else if(!strcmp(board_Op, "withdraw",true,8)) {
 	    if(PlayerInfo[playerid][pID] != GUNNARWATSONE_ID) return SendClientMessage(playerid, COLOR_GREY, "You can't do this.");
 	    
 	    if(sscanf(params,"s[10]s[10]i", board_Op, gunnar_Item, gunnar_Amount)) {
            SendClientMessage(playerid, COLOR_GREY, "USAGE: /gunnarsafe withdraw [item] [amount (weapon slot)]");
            SendClientMessage(playerid, COLOR_GREY, "Items: cash, crack, pot, materials, weapon");
            return 1;
        }
        if(gunnar_Amount < 1) return SendClientMessage(playerid, COLOR_GREY, "Withdraw a valid amount.");
        
        if(!strcmp(gunnar_Item, "cash",true,4)) {
 		    if(gunnar_Amount > gunnarsafeCash) return SendClientMessage(playerid, COLOR_GREY, "The safe doesn't have enough.");

 		    GiveMoney(playerid, gunnar_Amount);
 		    gunnarsafeCash -= gunnar_Amount;
 		    SendClientMessage(playerid, -1, "{90C3D4}* You have withdrawn some cash.");
 		}
 		else if(!strcmp(gunnar_Item, "crack",true,5)) {
 		    if(gunnar_Amount > gunnarsafeCrack) return SendClientMessage(playerid, COLOR_GREY, "The safe doesn't have enough.");

 		    PlayerInfo[playerid][pCrack] += gunnar_Amount;
 		    gunnarsafeCrack -= gunnar_Amount;
 		    SendClientMessage(playerid, -1, "{90C3D4}* You have withdrawn some crack.");
 		}
 		else if(!strcmp(gunnar_Item, "pot",true,3)) {
 		    if(gunnar_Amount > gunnarsafePot) return SendClientMessage(playerid, COLOR_GREY, "The safe doesn't have enough.");

 		    PlayerInfo[playerid][pPot] += gunnar_Amount;
 		    gunnarsafePot -= gunnar_Amount;
 		    SendClientMessage(playerid, -1, "{90C3D4}* You have withdrawn some pot.");
 		}
 		else if(!strcmp(gunnar_Item, "materials",true,9)) {
 		    if(gunnar_Amount > gunnarsafeMats) return SendClientMessage(playerid, COLOR_GREY, "The safe doesn't have enough.");

 		    PlayerInfo[playerid][pMats] += gunnar_Amount;
 		    gunnarsafeMats -= gunnar_Amount;
 		    SendClientMessage(playerid, -1, "{90C3D4}* You have withdrawn some materials.");
 		}
 		else if(!strcmp(gunnar_Item, "weapon",true,6)) {
            new slot = gunnar_Amount, weaponid = gunnarsafeWeapon[slot];
            if(weaponid == 0) return SendClientMessage(playerid, COLOR_GREY, "That weapon slot is empty.");
            GivePlayerGun(playerid, weaponid);
            gunnarsafeWeapon[slot] = 0;

            SendClientMessage(playerid, -1, "{90C3D4}* You have withdrawn a weapon.");
 		}
 		else return SendClientMessage(playerid, COLOR_GREY, "Invalid item.");
 		SaveGunnar();
 	}
 	else if(!strcmp(board_Op, "deposit",true,7)) {
 	    if(sscanf(params,"s[10]s[10]", board_Op, gunnar_Item, gunnar_Amount)) {
            SendClientMessage(playerid, COLOR_GREY, "USAGE: /gunnarsafe deposit [item] [amount (weapon slot)]");
            SendClientMessage(playerid, COLOR_GREY, "Items: cash, crack, pot, materials, weapon");
            return 1;
        }
        if(gunnar_Amount < 1) return SendClientMessage(playerid, COLOR_GREY, "Deposit a valid amount.");
        
        if(!strcmp(gunnar_Item, "cash",true,4)) {
 		    if(gunnar_Amount > PlayerInfo[playerid][pCash]) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough.");
 		    
 		    GiveMoney(playerid, -gunnar_Amount);
 		    gunnarsafeCash += gunnar_Amount;
 		    SendClientMessage(playerid, -1, "{90C3D4}* You have deposited some cash.");
 		}
 		else if(!strcmp(gunnar_Item, "crack",true,5)) {
 		    if(gunnar_Amount > PlayerInfo[playerid][pCrack]) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough.");

 		    PlayerInfo[playerid][pCrack] -= gunnar_Amount;
 		    gunnarsafeCrack += gunnar_Amount;
 		    SendClientMessage(playerid, -1, "{90C3D4}* You have deposited some crack.");
 		}
 		else if(!strcmp(gunnar_Item, "pot",true,3)) {
 		    if(gunnar_Amount > PlayerInfo[playerid][pPot]) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough.");

 		    PlayerInfo[playerid][pPot] -= gunnar_Amount;
 		    gunnarsafePot += gunnar_Amount;
 		    SendClientMessage(playerid, -1, "{90C3D4}* You have deposited some pot.");
 		}
 		else if(!strcmp(gunnar_Item, "materials",true,9)) {
 		    if(gunnar_Amount > PlayerInfo[playerid][pMats]) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough.");

 		    PlayerInfo[playerid][pMats] -= gunnar_Amount;
 		    gunnarsafeMats += gunnar_Amount;
 		    SendClientMessage(playerid, -1, "{90C3D4}* You have deposited some materials.");
 		}
 		else if(!strcmp(gunnar_Item, "weapon",true,6)) {
            new slot = gunnar_Amount, gunid = GetPlayerWeapon(playerid);
            if(gunid == 0) return SendClientMessage(playerid, COLOR_GREY, "You must be holding a weapon.");
            if(gunnarsafeWeapon[slot] != 0) return SendClientMessage(playerid, COLOR_GREY, "There's already a weapon in that slot.");
            gunnarsafeWeapon[slot] = GetPlayerWeapon(playerid);
            TakePlayerWeapon(playerid, gunid);

            SendClientMessage(playerid, -1, "{90C3D4}* You have deposited a weapon.");
 		}
 		else return SendClientMessage(playerid, COLOR_GREY, "Invalid item.");
 		SaveGunnar();
 	}
	return 1;
}

CMD:givegunnarkey(playerid, params[]) {
	if(PlayerInfo[playerid][pID] != GUNNARWATSONE_ID) return SendClientMessage(playerid, COLOR_GREY, "You cannot do this.");
	new playerb;
    if(sscanf(params, "u", playerb)) return SendClientMessage(playerid, COLOR_GREY, "USAGE: /givegunnarkey [playerid/PartOfName]");
    if(!IsPlayerConnected(playerb)) return SendClientMessage(playerid, COLOR_GREY, "Unknown player.");
    if(playerb == playerid) return SendClientMessage(playerid, COLOR_GREY, "You already have the key.");

    new Float:ayx, Float:ayy, Float:ayz;
    GetPlayerPos(playerb, ayx, ayy, ayz);
    if(!IsPlayerInRangeOfPoint(playerid, 4.0, ayx, ayy, ayz)) return SendClientMessage(playerid, COLOR_GREY, "You are not near that player.");

    if(gunnarKey[playerb] == 0) {
    	gunnarKey[playerb] = 1;
    	SendClientMessage(playerid, -1, "* You have given that player the Gunnar Key.");
    	SendClientMessage(playerb, -1, "* You have received the Gunnar Key.");
    }
    else {
    	gunnarKey[playerb] = 0;
    	SendClientMessage(playerid, -1, "* You have removed their key.");
    	SendClientMessage(playerb, -1, "* You no longer have access to the Gunnar Key.");
    }
    return 1;
}

CMD:chalkboard(playerid, params[]) {
	if(!CanGunnar(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You cannot do this.");
	if(!IsPlayerInRangeOfPoint(playerid, 3.0, 2213.5015,550.0428,9096.8848)) return SendClientMessage(playerid, COLOR_GREY, "You are not near the chalkboard.");
	new board_Op[10];
	if(sscanf(params, "s[10]", board_Op)) return SendClientMessage(playerid, COLOR_GREY, "USAGE: /chalkboard [read/edit]");
	
	if(!strcmp(board_Op, "read",true,4)) {
 	    new note;
 	    if(sscanf(params,"s[10]i", board_Op, note)) {
            SendClientMessage(playerid, COLOR_GREY, "USAGE: /chalkboard read [note]");
            SendClientMessage(playerid, COLOR_GREY, "(Note 1 - 5)");
            return 1;
        }
        if(note < 1 || note > 5) return SendClientMessage(playerid, COLOR_GREY, "Use chalkboard note 1 - 5.");
        
        new note_Str[250];
        format(note_Str, sizeof(note_Str), "The note reads:");

		switch(note) {
			case 1: {
		        format(note_Str, sizeof(note_Str), "%s\n{FFFFFF}%s", note_Str, gunnarNote1);
		    }
		    case 2: {
		        format(note_Str, sizeof(note_Str), "%s\n{FFFFFF}%s", note_Str, gunnarNote2);
		    }
		    case 3: {
		        format(note_Str, sizeof(note_Str), "%s\n{FFFFFF}%s", note_Str, gunnarNote3);
		    }
		    case 4: {
		        format(note_Str, sizeof(note_Str), "%s\n{FFFFFF}%s", note_Str, gunnarNote4);
		    }
		    case 5: {
		        format(note_Str, sizeof(note_Str), "%s\n{FFFFFF}%s", note_Str, gunnarNote5);
		    }
		}
		
		if(strlen(note_Str) > 100) {
		    strins(note_Str, "\n", 100);
		}
		
		ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Chalkboard Note", note_Str, "Close", "");
		return 1;
 	}
 	else if(!strcmp(board_Op, "edit",true,4)) {
 	    new note, note_New[200];
 	    if(sscanf(params,"s[10]is[200", board_Op, note, note_New)) {
            SendClientMessage(playerid, COLOR_GREY, "USAGE: /chalkboard edit [note] [text]");
            SendClientMessage(playerid, COLOR_GREY, "(Note 1 - 5)");
            return 1;
        }
        if(note < 1 || note > 5) return SendClientMessage(playerid, COLOR_GREY, "Use chalkboard note 1 - 5.");

        switch(note) {
			case 1: {
		        format(gunnarNote1, sizeof(gunnarNote1), "%s", note_New);
		    }
		    case 2: {
		        format(gunnarNote2, sizeof(gunnarNote2), "%s", note_New);
		    }
		    case 3: {
		        format(gunnarNote3, sizeof(gunnarNote3), "%s", note_New);
		    }
		    case 4: {
		        format(gunnarNote4, sizeof(gunnarNote4), "%s", note_New);
		    }
		    case 5: {
		        format(gunnarNote5, sizeof(gunnarNote5), "%s", note_New);
		    }
		}
		
		SendClientMessage(playerid, -1, "You have updated the note.");
		SaveGunnar();
 	}
 	else return SendClientMessage(playerid, COLOR_GREY, "USAGE: /chalkboard [read/edit]");
	return 1;
}