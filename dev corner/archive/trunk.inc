/* Trunk System
	Functions:
			IsAVan( vehicleid ) 			- is this vehicle a van?
			IsATruck( vehicleid ) 			- is this vehicle a truck?
			GetClosestTrunk( playerid ) 	- get trunk closest to player.
			ShowTrunkResponse( playerid, listitem )	- response from the shown trunk dialog.
			ShowTrunkResponse_PUT( playerid, listitem )	- ShowTrunkResponse option was to put stuff in the trunk.
*/
//======================[ Trunk System Header ]=====================
enum trInfo
{
	cTrunkOpened,
	cGun1,
	cGun2,
	cGun1Ammo,
	cGun2Ammo,
	cPot,
	cCrack,
	Float:cArmor
};
new TrunkInfo[MAX_VEHICLES][trInfo];
new CreatedCars[MAX_VEHICLES] = {INVALID_VEHICLE_ID, ...};

stock IsAVan( vehicleid )
{
	switch( GetVehicleModel( vehicleid ) )
	{
	    case 413, 482, 440, 459:
	        return 1;
	}
	return 0;
}
stock IsATruck( vehicleid )
{
	switch( GetVehicleModel( vehicleid ) )
	{
	    case 414, 433, 427, 456, 455, 482, 498, 499, 609:
	        return 1;
	}
	return 0;
}
static str[128];
//==================[ Trunk System Body - Functions ]=====================
GetClosestTrunk( playerid )
{
	IsByVehTrunk[playerid] = -1;
	
	new
		Float:vtpos[4], Float:vtinfo[3], vehcount = 0;
	for(new i = 0; i < MAX_VEHICLES; i++) //check if player by any trunk - jacob
	{
	    GetVehiclePos(i, vtpos[0], vtpos[1], vtpos[2]);
	    GetVehicleZAngle(i, vtpos[3]);

	    GetVehicleModelInfo(GetVehicleModel(i), VEHICLE_MODEL_INFO_SIZE, vtinfo[0], vtinfo[1], vtinfo[2]);
	    new Float:distance = (vtinfo[1]/2 + 0.5);
     	vtpos[0] += (distance * -floatsin(-vtpos[3], degrees));
    	vtpos[1] += (distance * -floatcos(-vtpos[3], degrees));
    	if(IsPlayerInRangeOfPoint(playerid, 1.5, vtpos[0], vtpos[1], vtpos[2]))
    	{
    	    IsByVehTrunk[playerid] = i;
    	    vehcount++;
		}
	}
	
	new check;
	for(new i = 0; i < sizeof(CreatedCars); i++)
	{
		if(CreatedCars[i] == IsByVehTrunk[playerid])
		{
			check = 1;
			CreatedCars[i] = INVALID_VEHICLE_ID;
			break;
		}
	}
	
	if( check )
	    return -1;
	    
	if( vehcount > 1 )
	    return -2;

	if( !IsByVehTrunk[playerid] )
		return -1;

	return IsByVehTrunk[playerid];
}
ShowTrunkResponse( playerid, listitem, vid ) {
	if(listitem == 0) //Open / Close
	{
		new engine, lights, alarm, doors, bonnet, boot, objective;
		new sendernames[MAX_PLAYER_NAME + 1];
		strmid(sendernames, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
		if( !TrunkInfo[vid][cTrunkOpened] )
		{
			GetVehicleParamsEx(vid, engine, lights, alarm, doors, bonnet, boot, objective);
			SetVehicleParamsEx(vid, engine, lights, alarm, doors, bonnet, VEHICLE_PARAMS_ON, objective);
			TrunkInfo[vid][cTrunkOpened] = 1;
			SendClientMessage(playerid, COLOR_WHITE, "Trunk {2F991A}open.");
			format(str, sizeof(str), "* %s has opened the trunk.", PlayerICName(playerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			return 1;
		}
		else if( TrunkInfo[vid][cTrunkOpened] )
		{
			GetVehicleParamsEx(vid, engine, lights, alarm, doors, bonnet, boot, objective);
			SetVehicleParamsEx(vid, engine, lights, alarm, doors, bonnet, VEHICLE_PARAMS_OFF, objective);
			TrunkInfo[vid][cTrunkOpened] = 0;
			SendClientMessage(playerid, COLOR_WHITE, "Trunk {E31919}closed.");
			format(str, sizeof(str), "* %s has closed the trunk.", PlayerICName(playerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			return 1;
		}
		return 1;
	}
	else if( !TrunkInfo[vid][cTrunkOpened] )
		return SendClientMessage( playerid, COLOR_GREY, "The trunk isn't opened." );

	//=============[ Trunk inventory; finding the slots ]===============
	//This will sort through all list items and sort them into their categories.
	new trunk_invent[12] = 0, //11 = max trunk items: gun: 0-1, armor: 2, pot 3, crack 4, crate 5-11
		trunk_count = 2;

	for( new i = 1; trInfo:i < trInfo; i++ ) //skip open state.
	{
		if( float( TrunkInfo[vid][trInfo:i] ) == 0.0 ) continue;
		if( i == 3 || i == 4 ) continue; //ammo data

		trunk_invent[trunk_count] = i;
		trunk_count++;
	}
	for( new i; i < sizeof( g_Crates ); i++ )
	{
		if( g_Crates[i][crate_trunk] == vid )
		{
			trunk_invent[trunk_count] = 8;
			trunk_count++;
		}
	}
	//=============[ Sorting through trunk inventory list ]=============
	//Each category of list items put into a nutshell.
	if( listitem > 0 && listitem < 12 )
	{
		if( trunk_invent[listitem] == 1 || trunk_invent[listitem] == 2 ) //1-2 gun
		{
			if(PlayerInfo[playerid][pConnectTime] < 2)
				return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can get a weapon.");

			new gunid, msg_str[128], gunammo;
			if( trunk_invent[listitem] == 1 && TrunkInfo[vid][cGun1] )
			{
				gunid = TrunkInfo[vid][cGun1];
				TrunkInfo[vid][cGun1] = 0;
				gunammo = TrunkInfo[vid][cGun1Ammo];
				TrunkInfo[vid][cGun1Ammo] = 0;
			}
			else if( trunk_invent[listitem] == 2 && TrunkInfo[vid][cGun2] )
			{
				gunid = TrunkInfo[vid][cGun2];
				TrunkInfo[vid][cGun2] = 0;
				gunammo = TrunkInfo[vid][cGun2Ammo];
				TrunkInfo[vid][cGun2Ammo] = 0;
			}
			else return 1; //error; no gun inside, stops exploits.

			new gunName[64];
			GivePlayerGun( playerid, gunid, gunammo );

			GetWeaponName( gunid, gunName, sizeof( gunName ) );
			format( msg_str, 128, "You've taken a %s from the vehicle.", gunName );
			SendClientMessage( playerid, COLOR_WHITE, msg_str );

			format( msg_str, 128, "* %s takes a %s from the trunk.", PlayerICName(playerid), gunName );
			ProxDetector(30.0, playerid, msg_str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			return 1;
		}
		else if( trunk_invent[listitem] == 7 )
		{
			if(TrunkInfo[vid][cArmor] != 0)
			{
				SetPlayerArmour(playerid, TrunkInfo[vid][cArmor]);
				PlayerInfo[playerid][pArmor] = TrunkInfo[vid][cArmor];
				TrunkInfo[vid][cArmor] = 0;
				format(str, sizeof(str), "* %s takes a vest from the trunk.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				return 1;
			}
			else
				return SendClientMessage( playerid, COLOR_GREY, "There is no vest inside of this trunk." );
		}
		else if( trunk_invent[listitem] == 6 )
		{
			ShowPlayerDialog(playerid, TRUNKTAKECRACK, DIALOG_STYLE_INPUT, "Take crack", "How many grams of crack do you want to take ?", "Take", "Cancel");
			return 1;
		}
		else if( trunk_invent[listitem] == 5 )
		{
			ShowPlayerDialog(playerid, TRUNKTAKEPOT, DIALOG_STYLE_INPUT, "Take pot", "How many grams of pot do you want to take ?", "Take", "Cancel"); //Trunk take pot
			return 1;
		}
		else if( trunk_invent[listitem] == 8 )
		{
			for( new i; i < sizeof( g_Crates ); i++ )
			{
				if( g_Crates[i][crate_trunk] < 1 ) continue;

				if( playerHasCrateID[playerid] >= 0 )
					return SendClientMessage( playerid, COLOR_GREY, "You are already holding a crate." );

				ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Crouch_In", 3.0, 0, 0, 0, 0, 0, 0);

				new crateid = i;
				//set the vars suggesting player has crate, and reference variable
				g_Crates[crateid][crate_player] = playerid;
				g_Crates[crateid][crate_trunk] = 0;
				playerHasCrateID[playerid] = crateid;

				switch( g_Crates[crateid][crate_type] )
				{
					case 3: SetPlayerAttachedObject( playerid, 8, g_Crates[crateid][crate_modelid], 5, -0.050999, 0.109999, 0.202000, -83.899963, -1.600038, 2.699998 );
					case 2: SetPlayerAttachedObject( playerid, 8, g_Crates[crateid][crate_modelid], 5, -0.050999, 0.174999, 0.202000, -83.899963, -1.600038, 2.699998 );
					case 1: SetPlayerAttachedObject( playerid, 8, g_Crates[crateid][crate_modelid], 5, 0.000000, 0.109999, 0.202000, -83.899963, -1.600038, 111.100006 );
				}
				SetPlayerSpecialAction(playerid, 25);
				return 1;
			}
			return SendClientMessage( playerid, COLOR_GREY, "There are no crates inside this trunk." );
		}
		else
		{
			ShowPlayerDialog(playerid, TRUNKPUTSTUFF, DIALOG_STYLE_LIST, "Put a gun in this trunk", "Put gun\nPut vest\nPut crack\nPut pot\nPut crate", "Put", "Cancel"); //Trunk put stuff
		}
	}
	return 1;

}
ShowTrunkResponse_PUT( playerid, listitem, vid ) {
	if( TrunkInfo[vid][cTrunkOpened] != 1 ) {
		return SendClientMessage( playerid, COLOR_GREY, "The trunk must be opened first.");
	}
	
	if(listitem == 0) //Put gun
	{
		if(PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can use this."); //selling to another
		if(IsACop(playerid) == 1 || IsANG(playerid) == 1 || IsAFireman(playerid) == 1 || IsASenate(playerid)) return SendClientMessage(playerid, COLOR_GRAD1, "Cops/FBI/SANG/SAFMD can not use this.");
		new gunID = GetPlayerWeapon(playerid),
			guntype = IsValidWeapon( playerid, gunID );

		if( !( gunID > 0 ) )
			return SendClientMessage( playerid, COLOR_GREY, "You are not holding a gun." );

		if( guntype == GUN_TYPE_ADMIN )
		{
			SendClientMessage(playerid, COLOR_GRAD1, "You can't place admin given weapons into a trunk.");
			return 1;
		}
		else if( guntype == GUN_TYPE_HACKED )
		{
			new admin_str[128];
			format( admin_str, 128, "{FF0000}AdmWarning{FFFFFF}: %s has attempted to put a desynced or hacked weapon inside of a trunk.", PlayerICName( playerid ) );
			SendAdminMessage( COLOR_WHITE, admin_str );
			SendClientMessage(playerid, COLOR_GRAD1, "You can't place admin given weapons into a trunk.");
			return 1;
		}
		else
		{
			new gunslot, ammoslot, gunName[64], msg_str[128],
				w_index = GetWeaponIndex( gunID );

			if( !TrunkInfo[vid][cGun1] ) gunslot = trInfo:cGun1, ammoslot = trInfo:cGun1Ammo;
			else if( !TrunkInfo[vid][cGun2] ) gunslot = trInfo:cGun2, ammoslot = trInfo:cGun2Ammo;
			else return SendClientMessage( playerid, COLOR_GREY, "There is no more room for guns inside this trunk." );

			GetWeaponName( gunID, gunName, sizeof( gunName ) );
			TrunkInfo[vid][trInfo:gunslot] = gunID;
			TrunkInfo[vid][trInfo:ammoslot] = g_pGunInfo[playerid][w_index][wo_Ammo];

			format( msg_str, 128, "You have put your %s in the car's trunk.", gunName );
			SendClientMessage( playerid, COLOR_WHITE, msg_str );
			TakePlayerWeapon( playerid, gunID );

			format( msg_str, 128, "* %s puts a %s in the trunk.", PlayerICName(playerid), gunName);
			ProxDetector(30.0, playerid, msg_str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			return 1;
		}
	}
	if(listitem == 1) //Put armor
	{
		if(IsACop(playerid) == 1 || IsANG(playerid) == 1 || IsAFireman(playerid) == 1 || IsASenate(playerid))
			return SendClientMessage(playerid, COLOR_GRAD1, "Cops/FBI/SANG/SAFMD can not use this.");

		GetPlayerArmour(playerid, PlayerInfo[playerid][pArmor]);
		if(PlayerInfo[playerid][pArmor] != 0)
		{
			TrunkInfo[vid][cArmor] = PlayerInfo[playerid][pArmor];
			SetPlayerArmour(playerid, 0);
			PlayerInfo[playerid][pArmor] = 0;
			format(str, sizeof(str), "* %s puts a vest in the trunk.", PlayerICName(playerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You haven't got any armour ");
		}
		return 1;
	}
	if(listitem == 2) //Put crack
	{
		format( str, sizeof( str ), "{FFFFFF}You have: %d grams of crack. How many grams do you want to put in?", PlayerInfo[playerid][pCrack] );
		ShowPlayerDialog(playerid, TRUNKPUTCRACK, DIALOG_STYLE_INPUT, "Put crack", str, "Put", "Cancel"); //Trunk put crack
	}
	if(listitem == 3) //Put pot
	{
		format( str, sizeof( str ), "{FFFFFF}You have: %d grams of pot. How many grams do you want to put in?", PlayerInfo[playerid][pPot] );
		ShowPlayerDialog(playerid, TRUNKPUTPOT, DIALOG_STYLE_INPUT, "Put pot", str, "Put", "Cancel"); //Trunk put pot
	}
	if( listitem == 4 ) //crate
	{

		new crateid = playerHasCrateID[playerid];

		if( crateid < 0 )
			return SendClientMessage( playerid, COLOR_GREY, "You are not holding a crate." );

		if( g_Crates[crateid][crate_player] != playerid )
			return SendClientMessage( playerid, COLOR_GREY, "You are not holding a crate." );

		new trunk_crate_cap = 0;
		for( new i; i < sizeof( g_Crates ); i++ ) //calc amount of crates inside the vehicle
		{
			if( g_Crates[i][crate_trunk] == vid )
				trunk_crate_cap++;
		}

		if( trunk_crate_cap > 4 && IsAVan( vid ) || trunk_crate_cap > 9 && IsATruck( vid ) )
			return SendClientMessage( playerid, COLOR_GREY, "There is no room left in this vehicle." );

		else if( trunk_crate_cap > 1 && !IsAVan( vid ) && !IsATruck( vid ) )
			return SendClientMessage( playerid, COLOR_GREY, "There is no room left in this vehicle." );

		for( new i; i < sizeof( g_Crates ); i++ )
		{
			if( g_Crates[i][crate_trunk] < 1 )
			{
				g_Crates[crateid][crate_trunk] = vid;
				RemovePlayerAttachedObject(playerid, 8);
				g_Crates[crateid][crate_player] = 9999;
				SetPlayerSpecialAction( playerid, 0 );
				ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Crouch_In", 3.0, 0, 0, 0, 0, 0, 0);

				playerHasCrateID[playerid] = -1;
				return 1;
			}
		}
	}
	return 1;
}