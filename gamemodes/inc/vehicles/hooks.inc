/*
 _    _             _        
| |  | |           | |       
| |__| | ___   ___ | | _____ 
|  __  |/ _ \ / _ \| |/ / __|
| |  | | (_) | (_) |   <\__ \
|_|  |_|\___/ \___/|_|\_\___/

	This file contains hooks for callbacks
	that the player owned vehicle system needs.
	
	I encourage hooking. It is preprocessing.
	It doesn't slow down execution time in-game!
                             
*/


/*=========================================================
OnPlayerSecondSync -
	contains: /breakin - detect if player left vehicle
===========================================================
*/
OnVehiclePlayerSecondSync( playerid ) {
	new Float:vpos[3];
	
	if( LockPickingCar[playerid] == -1 ) {
			return 1;
	}
	
	new player = LockPickingCar[playerid];
	new slot = GetPlayerSpawnedVehicleSlot( player );
	if( slot < 0 ) return 1;
	
	#define veh(%1) PlayerVehicles[player][slot][pv%1]

	GetVehiclePos( veh(ID), vpos[0], vpos[1], vpos[2] );
	if( !IsPlayerInRangeOfPoint( playerid, 3.0, vpos[0], vpos[1], vpos[2] ) ) {
		vehiclePickCount[playerid] = 0;
		
		LockPickingCar[playerid] = -1;
		HidePlayerProgressBar( playerid, workingProgress[playerid] );
		ClearAnimations( playerid );
		
		SendClientMessage( playerid, COLOR_GREY, "Your attempt to breakin has failed as you have walked away." );
		return 1;
	}
	#undef veh
	return 1;
}

/*=========================================================
OnPlayerKeyStateChange -
	contains: /breakin; tapping Y rapidly.
===========================================================
*/

#define PRESSING(%0,%1) \
	(%0 & (%1))

public OnPlayerKeyStateChange( playerid, newkeys, oldkeys ) {
	#if defined hook_OnPlayerKeyStateChange
		hook_OnPlayerKeyStateChange( playerid, newkeys, oldkeys );
	#endif
	
	if( PRESSING( newkeys, KEY_YES ) ) {
		cmd_lock( playerid, "\0" );
	}
	
	//The user must tap "Y" 100 times to unlock the car.
	if( PRESSING(newkeys, KEY_YES) ) {
		if( LockPickingCar[playerid] == -1 ) {
			return 1;
		}
		vehiclePickCount[playerid] += 1;
		SetPlayerProgressBarValue( playerid, workingProgress[playerid], float( vehiclePickCount[playerid] ) );
		
		new player = LockPickingCar[playerid];
		#define veh(%1) PlayerVehicles[player][playerSpawnedVehicle[player]][pv%1]

		//Check if timer is up, successful breakin
		if( vehiclePickCount[playerid] == 100 ) {
		
			displayCenterHUDInfo( playerid, "You have broken the vehicle's lock.", 4 );
			
			veh(locked) = 0;
			SetVehicleLockState( veh(ID), veh(locked) ); //unlock the car
			vehiclePickCount[playerid] = 0;
			LockPickingCar[playerid] = -1;
			HidePlayerProgressBar( playerid, workingProgress[playerid] );
			ClearAnimations( playerid );
			ApplyAnimationEx( playerid, "CARRY", "crry_prtial", 4.0, 0, 0, 0, 0, 0 );
		}
		#undef veh
	}
	return 1;
}
#if defined _ALS_OnPlayerKeyStateChange
    #undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange hook_OnPlayerKeyStateChange
#if defined hook_OnPlayerKeyStateChange
    forward hook_OnPlayerKeyStateChange( playerid, newkeys, oldkeys );
#endif

/*=========================================================
OnVehicleDeath -
	contains: vehicle insurance
===========================================================
*/
public OnVehicleDeath( vehicleid, killerid ) {
	for( new i; i <= GetPlayerPoolSize(); i++ ) {
		new slot = GetPlayerSpawnedVehicleSlot( i );
		if( slot < 0 ) continue;
		
		#define veh(%0) PlayerVehicles[i][slot][pv%0]
		if( veh(ID) == vehicleid ) {
		
			if( !veh(insurance) ) {
				TrunkInfo[vehicleid][cTrunkOpened] = 0;
				TrunkInfo[vehicleid][cGun1] = 0;
				TrunkInfo[vehicleid][cGun2] = 0;
				TrunkInfo[vehicleid][cCannabis] = 0;
				TrunkInfo[vehicleid][cCocaine] = 0;
				TrunkInfo[vehicleid][cArmor] = 0.0;
				break;
			}
			
			//Set vehicle to destroy -> don't respawn, let the player do it.
			destroyPlayerVehicle( i, slot );
			//spawnPlayerVehicle( i, slot );
		}
		#undef veh
	}
	printf("exiting /vehicles/hooks: OnVehicleDeath( %d, %d ), entering SARP_current callback", vehicleid, killerid );
	#if defined hook_OnVehicleDeath
		hook_OnVehicleDeath( vehicleid );
	#endif
	return 1;
}

#if defined _ALS_OnVehicleDeath
    #undef OnVehicleDeath
#else
    #define _ALS_OnVehicleDeath
#endif
#define OnVehicleDeath hook_OnVehicleDeath
#if defined hook_OnVehicleDeath
    forward hook_OnVehicleDeath( vehicleid );
#endif


/*=========================================================
OnVehicleRespawn
	fixes seat position sync issues with SAMP; just cancel respawn in last pos,
	change pos to parked position
===========================================================
*/
public OnVehicleSpawn( vehicleid ) {
	#if defined veh_OnVehicleSpawn
		veh_OnVehicleSpawn( vehicleid );
	#endif
	
	//unlock the car by default
	SetVehicleLockState( vehicleid, 0 );
	
	for( new i; i <= GetPlayerPoolSize(); i++ ) {
		new slot = GetPlayerSpawnedVehicleSlot( i );
		if( slot < 0 ) continue;
		
		#define veh(%0) PlayerVehicles[i][slot][pv%0]
		if( veh(ID) == vehicleid ) {
			//Set vehicle to respawn in last parked position.
			destroyPlayerVehicle( i, slot );
			spawnPlayerVehicle( i, slot );
		}
		#undef veh
	}
	printf("exiting /vehicles/hooks: OnVehicleSpawn( %d )", vehicleid );
	
	return 1;
}

#if defined _ALS_OnVehicleSpawn
    #undef OnVehicleSpawn
#else
    #define _ALS_OnVehicleSpawn
#endif
#define OnVehicleSpawn veh_OnVehicleSpawn
#if defined veh_OnVehicleSpawn
    forward veh_OnVehicleSpawn( vehicleid );
#endif

/*=========================================================
OnPlayerDisconnect -
	contains: Reset all player vars
===========================================================
*/

public OnPlayerDisconnect( playerid, reason ) {

	SQLSave( playerid );
	
	//destroy the player's vehicles
	new slot = GetPlayerSpawnedVehicleSlot( playerid );
	if( slot > -1 ) {
		PlayerInfo[playerid][lastCarID] = PlayerVehicles[playerid][slot][pvSQLID];
		destroyPlayerVehicle( playerid, slot );
	} else {
		PlayerInfo[playerid][lastCarID] = -1;
	}
	
	#if defined hook_OnPlayerDisconnect
		hook_OnPlayerDisconnect( playerid, reason );
	#endif
	
	//reset slots, don't rid the vehicle...
	for( new s; s < PlayerInfo[playerid][pMaxCarSlots]; s++ ) {
		for( new i; pvehicleDatav:i < pvehicleDatav; i++ ) {
			PlayerVehicles[playerid][s][pvehicleDatav:i] = 0;
		}
	}
	
	//sellcar info
	playeridcarOffered[playerid] = -1;
	carOfferedSlot[playerid] = -1;
	carOfferedPrice[playerid] = -1;
	
	spareKeys[playerid] = -1;
	spareKeysSlot[playerid] = -1;
	
	// /breakin time remaining
	vehiclePickCount[playerid] = 0;
	
	//player is upgrading vehicle index x
	vupgrading[playerid] = -1;
	
	//spawn timer
	vSpawnTimer[playerid] = 0;

	DestroyPlayerProgressBar( playerid, workingProgress[playerid] );
	printf("exiting /vehicles/hooks: OnPlayerDisconnect( %d, %d )", playerid, reason );
	return 1;
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect hook_OnPlayerDisconnect
#if defined hook_OnPlayerDisconnect
    forward hook_OnPlayerDisconnect( playerid );
#endif

/*=========================================================
OnDialogResponse
	contains: /vupgrade
===========================================================
*/

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) {
	#if defined hook_OnDialogResponse
		hook_OnDialogResponse( playerid, dialogid, response, listitem, inputtext );
	#endif
	
	//**	Upgrade menu, on response: take the money, proceed the upgrade.		**
	
	if( dialogid == DIALOG_VEHICLES_UP2 ) {
		if( !response ) return 1;
		
		new upslot = vupgrading[playerid];
		
		//No car slot is selected to be upgraded.
		if( upslot < 0 ) return 1;
		
		#define veh(%1) PlayerVehicles[playerid][upslot][pv%1]
		if( veh(SQLID) < 1 ) {
			return 1;
		}
		
		if( !IsValidVehicle( veh(ID) ) ) {
			return SendClientMessage( playerid, COLOR_GREY, "You have no vehicle spawned to upgrade, try using /v spawn." );
		}
		
		switch( listitem ) {
			//GPS
			case 0: {
				if( veh(gps) ) {
					return SendClientMessage( playerid, COLOR_GREY, "This vehicle already has a GPS." );
				}
				if( PlayerInfo[playerid][pCash] < 350000 ) {
					return SendClientMessage( playerid, COLOR_GREY, "You cannot afford this." );
				}
				GiveMoney( playerid, -350000 );
				veh(gps) = 1;
				vupgrading[playerid] = -1;
				SendClientMessage( playerid, COLOR_WHITE, "Congratulations on your purchase! You can use /trackcar to locate your vehicle." );
			}
			//Alarm
			case 1: {
				if( veh(alarm) ) {
					return SendClientMessage( playerid, COLOR_GREY, "This vehicle already has an alarm." );
				}
				if( PlayerInfo[playerid][pCash] < 200000 ) {
					return SendClientMessage( playerid, COLOR_GREY, "You cannot afford this." );
				}
				GiveMoney( playerid, -200000 );
				veh(alarm) = 1;
				vupgrading[playerid] = -1;
				SendClientMessage( playerid, COLOR_WHITE, "Congratulations on your purchase! Your vehicle now has an alarm." );
			}
			//Insurance
			case 2: {
				if( veh(insurance) ) {
					return SendClientMessage( playerid, COLOR_GREY, "This vehicle is already insured." );
				}
				if( PlayerInfo[playerid][pCash] < 250000 ) {
					return SendClientMessage( playerid, COLOR_GREY, "You cannot afford this." );
				}
				GiveMoney( playerid, -250000 );
				veh(insurance) = 1;
				vupgrading[playerid] = -1;
				SendClientMessage( playerid, COLOR_WHITE, "Congratulations on your purchase! Your vehicle is now insured." );
			}
			//Spare keys
			case 3: {
				if( veh(sparekey) ) {
					return SendClientMessage( playerid, COLOR_GREY, "This vehicle already has a set of spare keys." );
				}
				if( PlayerInfo[playerid][pCash] < 250000 ) {
					return SendClientMessage( playerid, COLOR_GREY, "You cannot afford this." );
				}
				GiveMoney( playerid, -250000 );
				veh(sparekey) = 1;
				vupgrading[playerid] = -1;
				SendClientMessage( playerid, COLOR_WHITE, "Congratulations on your purchase! You now have a set of spare keys for your vehicle (/v givekeys).");
			}
			//Custom vehicle plate
			case 4: {
				if( PlayerInfo[playerid][pCash] < 100000 ) {
					return SendClientMessage( playerid, COLOR_GREY, "You cannot afford this." );
				}
				ShowPlayerDialog( playerid, DIALOG_CUSTOM_PLATE, DIALOG_STYLE_INPUT, "Custom Vehicle Plate", "Enter a new custom license plate for your vehicle.", "Purchase", "Cancel" );
			}
			case 5: {
				if( PlayerInfo[playerid][pMaxCarSlots] == MAX_VEHICLE_SLOTS ) {
					return SendClientMessage( playerid, COLOR_GREY, "You cannot buy anymore vehicle slots." );
				}
				if( PlayerInfo[playerid][pCash] < 15000000 ) {
					return SendClientMessage( playerid, COLOR_GREY, "You cannot afford this." );
				}
				GiveMoney( playerid, -15000000 );
				PlayerInfo[playerid][pMaxCarSlots]++;
				SendClientMessage( playerid, COLOR_WHITE, "Congratulations on your purchase! You can now purchase an extra vehicle." );
			}
		
		}
		#undef veh
		
		return 1;
	}
	
	//**	Upgrading vehicle slot's: custom license plate: MSGBOX.		**
	
	if( dialogid == DIALOG_CUSTOM_PLATE ) {
		if( !response ) return 1;
		
		if( vupgrading[playerid] < 0 ) {
			return 1;
		}
		
		#define veh(%1) PlayerVehicles[playerid][vupgrading[playerid]][pv%1]
		
		if( PlayerInfo[playerid][pCash] < 100000 ) {
					return SendClientMessage( playerid, COLOR_GREY, "You cannot afford this." );
		}
		//Acceptable characters: A-Z, a-z, numbers 0-9 max length 8
		if( !regex_match( inputtext, "^[A-Za-z0-9]{0,}" ) || strlen( inputtext ) > 8 ) {
			return SendClientMessage( playerid, COLOR_GREY, "You can only enter characters A-Z and/or numbers 0-9, and a maximum of 8 characters.");
		}
		
		GiveMoney( playerid, -100000 );
		format( veh(plate), 9, "%s", inputtext );
		SetVehicleNumberPlate( veh(ID), veh(plate) );
		
		#undef veh
		vupgrading[playerid] = -1;
		SendClientMessage( playerid, COLOR_LIGHTBLUE, "Congratulations on your purchase! Your vehicle now has a custom number plate." );
		return 1;
	
	}
	return 1;
}
#if defined _ALS_OnDialogResponse
    #undef OnDialogResponse
#else
    #define _ALS_OnDialogResponse
#endif
#define OnDialogResponse hook_OnDialogResponse
#if defined hook_OnDialogResponse
    forward hook_OnDialogResponse( playerid );
#endif



public OnVehiclePaintjob( playerid, vehicleid, paintjobid ) {
	new pvid = GetPlayerSpawnedVehicleSlot( playerid );
	if( pvid > -1 ) {
		if( PlayerVehicles[playerid][pvid][pvID] == vehicleid ) {
			PlayerVehicles[playerid][pvid][pvpaintjob] = paintjobid;
		}
	}
	return 1;
}
public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	new slot = GetPlayerSpawnedVehicleSlot( playerid );
	if( slot < 0 ) return 1;
	if( PlayerVehicles[playerid][slot][pvID] == vehicleid ) {
		PlayerVehicles[playerid][slot][pvcolor1] = color1;
		PlayerVehicles[playerid][slot][pvcolor2] = color2;
	}
	return 1;
}