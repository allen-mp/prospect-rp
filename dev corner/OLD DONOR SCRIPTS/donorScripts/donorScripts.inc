#define COLOR_PURPLE 		0xC2A2DAAA
#define COLOR_YELLOW 		0xFFFF00AA
#define COLOR_ADVERT 		0x00AA00FF
#define COLOR_WHITE 		0xFFFFFFAA
#define COLOR_INVALID       0xAA3333AA
#define COLOR_GREY			0xAFAFAFAA

/*
  _____                      _                    
 |  __ \                    | |                   
 | |  | | ___    _ __   ___ | |_   _   _ ___  ___ 
 | |  | |/ _ \  | '_ \ / _ \| __| | | | / __|/ _ \
 | |__| | (_) | | | | | (_) | |_  | |_| \__ \  __/
 |_____/ \___/  |_| |_|\___/ \__|  \__,_|___/\___|
                                                 
	Old DonorScripts, contains messy script, to be sorted.
*/

new donorstr[1000];

stock dOnPlayerPickUpDynamicPickup(playerid, pickupid) { //this is called on PickUpPickup at the beginning
	if(pickupid == WaterPickup) {
	    GameTextForPlayer(playerid, "~w~Type ~r~/getwater ~w~to drink some water", 3000, 3);
	    return 1;
	}
	/*if(pickupid == tGC) {
		GivePlayerGun(playerid, 2);
		return 1;
	}*/
	if(pickupid == ClothesPickup) {
	    if(!InBoxingGear[playerid]) GameTextForPlayer(playerid, "~w~Type ~r~/boxclothes ~w~to change into~n~your boxing clothes", 3000, 3);
	    else GameTextForPlayer(playerid, "~w~Type ~r~/boxclothes ~w~to change into~n~your street clothes", 3000, 3);
	}
	if(pickupid == OutfitPickup) {
	    if(!InOutfit[playerid]) {
		    GameTextForPlayer(playerid, "~w~Type ~r~/outfit ~w~to change into~n~your security uniform", 3000, 3);
		    return 1;
		}
		else {
		    GameTextForPlayer(playerid, "~w~Type ~r~/outfit ~w~to change into~n~your street clothes", 3000, 3);
		    return 1;
		}
	}
	if(pickupid == VestPickup) { //crash
 		GameTextForPlayer(playerid, (pArmour[playerid] > 0.0) ? ("~w~Type ~r~/lockervest ~w~to get your~n~vest into the locker") : ("~w~Type ~r~/lockervest ~w~to put your~n~vest into the locker"), 3000, 3);
   		return 1;
	}
	if(pickupid == FighterPickup) {
        GameTextForPlayer(playerid, "~w~Type ~r~/fightme ~w~to fight other players", 3000, 3);
        return 1;
	}
	if(pickupid == FFAPickup) {
        GameTextForPlayer(playerid, "~w~Type ~r~/ffa ~w~to fight in the free for all ring", 3000, 3);
        return 1;
	}
	if(pickupid == CagePickup) {
        GameTextForPlayer(playerid, "~w~Type ~r~/cage ~w~to fight in the cage match", 3000, 3);
        return 1;
	}
	return 1;
}

//========================================================================[ Functions ]========================================================================
stock dGetWeaponName(weaponid) {
	new weapon_name[32];
    if(weaponid == 0)
	    weapon_name = "None";
	else
	    GetWeaponName(weaponid, weapon_name, 32);
	return weapon_name;
}

stock donorEnter(playerid) {
	// Info: This command is linked with /enter from the main gamemode. Put /enter coords here.

	//================ [Sean Johnson] ================
	new Float:vpos[4];
	GetVehiclePos(SeanBenson, vpos[0], vpos[1], vpos[2]);
	GetVehicleZAngle(SeanBenson, vpos[3]);
	
	new Float:vinfo[3];
	GetVehicleModelInfo(GetVehicleModel(SeanBenson), VEHICLE_MODEL_INFO_SIZE, vinfo[0], vinfo[1], vinfo[2]);
	
	new Float:distance = (vinfo[1]/2 + 0.5);
	vpos[0] += (distance * -floatsin(-vpos[3], degrees));
	vpos[1] += (distance * -floatcos(-vpos[3], degrees));
	if(IsPlayerInRangeOfPoint(playerid, 1.5, vpos[0], vpos[1], vpos[2])) {
		if( gCarLock[SeanBenson] ) return 1;
		FreezePlayer(playerid, FREEZE, ENTER_FREEZE);
		SetPlayerPos(playerid, 297.9415, 32.0862, 10000.0674);
		SetPlayerFacingAngle(playerid, 0);
		SetPlayerInterior(playerid, 1);
	}
	
	//================ [Robert Henderson] ================
	if(IsPlayerInRangeOfPoint(playerid, 5.0, 1260.7622, -1264.3942, 19380.3145))
	{
		if(rhVipLock == 0) return SendClientMessage(playerid, COLOR_GREY, "   The Kush VIP is closed!");
		if(PlayerInfo[playerid][pCash] < rhVipPrice) return SendClientMessage(playerid, COLOR_GREY, "   You can't afford that (/kushentryfee)!");
		rhSafeCash += rhVipPrice;
		GiveMoney(playerid, -rhVipPrice);
		format(donorstr, sizeof(donorstr), "   You have been charged ($%i)!", rhVipPrice);
		SendClientMessage(playerid, COLOR_GREY, donorstr);
		SetPlayerPos(playerid, 1260.7622, -1267.3262, 19381.6914);
		SaverhClub();
		return 1;
	}
	
	if(IsPlayerInRangeOfPoint(playerid, 5.0, 1282.2648, -1262.0641, 13.6107)) {
		if(rhDoorLock == 0) return SendClientMessage(playerid, COLOR_GREY, "   The Kush Lounge is closed!");
		if(PlayerInfo[playerid][pCash] < rhDoorPrice) return SendClientMessage(playerid, COLOR_GREY, "   You can't afford that!");
		FreezePlayer(playerid, FREEZE, ENTER_FREEZE);
		SetCameraBehindPlayer(playerid);
		rhSafeCash += rhDoorPrice;
		GiveMoney(playerid, -rhDoorPrice);
		format(donorstr, sizeof(donorstr), "   You have been charged ($%i)!", rhDoorPrice);
		SetPlayerInterior(playerid, 5);
		SetPlayerVirtualWorld(playerid, 0);
		SetPlayerPos(playerid, 1249.6672, -1254.5811, 19376.3457);
		SetPlayerFacingAngle(playerid, 270);
		SetPlayerTime(playerid, 0, 0);
		return 1;
	}

	//================ [Desmond DeSantos] ==============
	if(IsPlayerInRangeOfPoint(playerid, 5.0, 1985.71, -1999.52, 13.54)) {
		if(GetPlayerVirtualWorld(playerid) == 0) {
		    if(DesmondLocked == true) return SendClientMessage(playerid, COLOR_GREY, "The DeSantos Casino is currently closed.");
		    if(PlayerInfo[playerid][pCash] < DesmondEntryFee) return SendClientMessage(playerid, COLOR_GREY, "You cannot afford the entrance fee.");

			GiveMoney(playerid, -DesmondEntryFee);
			DesmondSafe[DESMOND_SAFE_MONEY] += DesmondEntryFee;

			SetPlayerInterior(playerid, 12);
			SetPlayerVirtualWorld(playerid, 23);
			SetPlayerPos(playerid, 1133.2172, -15.4662, 1000.6797);
			SetPlayerFacingAngle(playerid, 2.2547);
			
			SetCameraBehindPlayer(playerid);
		}
		return 1;
	}
	
	if(IsPlayerInRangeOfPoint(playerid, 5.0, 1143.2092, 1.6622, 1000.6797)) {
	    if(GetPlayerVirtualWorld(playerid) == 23) {
	        if(DesmondOfficeLocked == true) return SendClientMessage(playerid, COLOR_GREY, "The office space is currently locked.");
	        
	        SetPlayerPos(playerid, 1153.4292, -2.7271, 997.6765);
			SetPlayerFacingAngle(playerid, 356.1355);
	        SetCameraBehindPlayer(playerid);
		}
		return 1;
	}

	//================ [Nick Fletcher] ==============
	if(IsPlayerInRangeOfPoint(playerid, 5.0, 1491.7787, -1449.1501, 13.5391) && GetPlayerVirtualWorld(playerid) == 0) {
	    if(fCasinoLocked == false) {
			SetPlayerPos(playerid, 1133.0906, -14.7065, 1000.6797);
			SetPlayerInterior(playerid, 12);
			SetPlayerVirtualWorld(playerid, 5);
		}
		else return SendClientMessage(playerid, -1, "The Fletcher Casino is currently closed.");
		return 1;
	}
	return 1;
}

stock donorExit(playerid) {
	// Info: This command is linked with /exit from the main gamemode. Put /exit coords here.
	
	//================ [Sean Johnson] ================
	if(IsPlayerInRangeOfPoint(playerid, 2.0,297.9415, 32.0862, 10000.0674)) {
	    new Float:vpos[4];
	    GetVehiclePos(SeanBenson, vpos[0], vpos[1], vpos[2]);
	    GetVehicleZAngle(SeanBenson, vpos[3]);
	    
		new Float:vinfo[3];
	    GetVehicleModelInfo(GetVehicleModel(SeanBenson), VEHICLE_MODEL_INFO_SIZE, vinfo[0], vinfo[1], vinfo[2]);
	    
		new Float:distance = (vinfo[1]/2 + 0.5);
     	vpos[0] += (distance * -floatsin(-vpos[3], degrees));
    	vpos[1] += (distance * -floatcos(-vpos[3], degrees));
    	SetPlayerPos(playerid, vpos[0], vpos[1], vpos[2]);
    	SetPlayerInterior(playerid, 0);
	}
	
	//================ [Robert Henderson] ================
	if(IsPlayerInRangeOfPoint(playerid, 5.0, 1260.7622, -1267.3262, 19381.6914)) return SetPlayerPos(playerid, 1260.7622, -1264.3942, 19380.3145);
	
	if(IsPlayerInRangeOfPoint(playerid, 5.0, 1249.6672, -1254.5811, 19376.3457)) {
		FreezePlayer(playerid, FREEZE, ENTER_FREEZE);
		SetCameraBehindPlayer(playerid);
		SetPlayerInterior(playerid, 0);
		SetPlayerVirtualWorld(playerid, 0);
		SetPlayerPos(playerid, 1282.2648, -1262.0641, 13.6107);
		SetPlayerFacingAngle(playerid, 90);
		SetPlayerTime(playerid, 12, 0);
		return 1;
	}

	if(IsPlayerInRangeOfPoint(playerid, 5.0, 157.2526, 975.3065, 1954.4091)) {
		if(PlayerInfo[playerid][pLeader] == 2) {
			FreezePlayer(playerid, FREEZE, ENTER_FREEZE);
			SetPlayerPos(playerid, 2919.1328,2118.0205,17.8955);
		}
		return 1;
	}

	//================ [Desmond DeSantos] ================
	if(IsPlayerInRangeOfPoint(playerid, 3.5, 1133.18, -15.30, 1000.67)) {
		if(GetPlayerVirtualWorld(playerid) == 23) {
			SetPlayerPos(playerid, 1985.2245, -1999.0514, 13.5469);//outside
			SetPlayerFacingAngle(playerid, 1.2938);
			SetPlayerInterior(playerid, 0);
			SetPlayerVirtualWorld(playerid, 0);
			SetCameraBehindPlayer(playerid);
		}
		return 1;
	}
	
	if(IsPlayerInRangeOfPoint(playerid, 3.5, 1153.4292, -2.7271, 997.6765)) {
	    if(GetPlayerVirtualWorld(playerid) == 23) {
	        SetPlayerPos(playerid, 1142.5255, 1.5930, 1000.6797);
	        SetPlayerFacingAngle(playerid, 91.5066);
	        SetCameraBehindPlayer(playerid);
		}
	}

	//================ [Nick Fletcher] ================
	if(IsPlayerInRangeOfPoint(playerid, 5.0, 1133.0906,-14.7065,1000.6797) && GetPlayerVirtualWorld(playerid) == 5) {
		SetPlayerPos(playerid, 1491.7787, -1449.1501, 13.5391);
		SetPlayerFacingAngle(playerid, 358.1619);
		SetPlayerInterior(playerid, 0);
		SetPlayerVirtualWorld(playerid, 0);
		return 1;
	}
	return 1;
}

stock dOnPlayerEnterCheckpoint(playerid) {
	if(CP[playerid] >= DIALOG_UNUSED + 4 && CP[playerid] <= CHECKPOINT_FLETCHER) {
		//================ [Eric Daxono] ================
	    if(CP[playerid] == CHECKPOINT_OUTPOST)
            SendClientMessage(playerid, 0xB22222FF, "Welcome to Outpost Radio!");
            
		DisablePlayerCheckpointEx(playerid);
		CP[playerid] = CHECKPOINT_NONE;
	}
	return 1;
}