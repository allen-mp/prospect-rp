/*
 _____       _                 _            _   _             
|_   _|     | |               | |          | | (_)            
  | |  _ __ | |_ _ __ ___   __| |_   _  ___| |_ _  ___  _ __  
  | | | '_ \| __| '__/ _ \ / _` | | | |/ __| __| |/ _ \| '_ \ 
 _| |_| | | | |_| | | (_) | (_| | |_| | (__| |_| | (_) | | | |
|_____|_| |_|\__|_|  \___/ \__,_|\__,_|\___|\__|_|\___/|_| |_|
                                                              
	This is the system for player owned vehicles.
	
	Related files are in /gamemodes/inc/vehicles.
		-> commands.inc ~ has all player owned vehicle commands.
							command related variables are here too!
		-> impounds.inc ~ is the impound system.
		
	It's important to note: only the player's spawned vehicle will be saved.
	
	What's in this file?:
	
		Structure,
			• definitions & variables
		Saving,
			• savePlayerVehicle( index )
			• savePlayerVehicles()
		Loading,
			• loadPlayerVehicles()
			• setupPlayerVehicles()
		Spawning,
			• spawnPlayerVehicle( index )
		Despawning,
			• destroyPlayerVehicle( index )
		Creating,
			• createPlayerVehicle( playerid, slot, model, x, y, z, rz )
		Useful functions
			• SetVehicleAlarmState ( vehicleid ), SetVehicleEngineState( vehicleid ), SetVehicleLockState( vehicleid )

*/

/*
===================================================
  _____ _                   _                  
 / ____| |                 | |                 
| (___ | |_ _ __ _   _  ___| |_ _   _ _ __ ___ 
 \___ \| __| '__| | | |/ __| __| | | | '__/ _ \
 ____) | |_| |  | |_| | (__| |_| |_| | | |  __/
|_____/ \__|_|   \__,_|\___|\__|\__,_|_|  \___|

	Definitions & variables
===================================================
*/

#define MAX_OWNED_VEHICLES 4000

//custom color for new systems
#define COLOR_VEHICLES 0xFFB76FFF

#define MAX_SPAWNED_VEHICLES 1

//Structure of each vehicle.
enum pvehicleDatav {
	pvID, 			//Array index
	pvplayerID,
	pvspawned, 		//Vehicles can be loaded, but not spawned
	pvSQLID,		//SQL ID of the vehicle in the DB.
	pvpID,			//Related player SQL ID.
	pvmodel,
	Float:pvx,
	Float:pvy,
	Float:pvz,
	Float:pvrz,
	pvcolor1,
	pvcolor2,
	pvpaintjob,
	pvlocked,
	pvgas,
	Float:pvdamage,
	
	//parking & towing
	Float:pvparkx,
	Float:pvparky,
	Float:pvparkz,
	Float:pvparkrz,
	
	//impound system
	pvimpounded,
	
	//upgrades
	pvgps,
	pvalarm,
	pvinsurance,
	pvsparekey,
	pvplate[9],
	
	//trunk stuff
	pvGun1,
	pvGun2,
	pvPot,
	pvCrack,
	Float:pvArmor,
	pvtow, //add towbar too!
	
	//damage
	pvdpanels,
	pvddoors,
	pvdlights,
	pvdtires,
	
	//mod shop stuff
	pvcomponents[14],
	pvvw
}

//Stores all vehicle data.
new PlayerVehicles[MAX_PLAYERS][MAX_VEHICLE_SLOTS][pvehicleDatav]

//Stores the number of vehicle in the DB.
new pVehiclePool

//Stores the last SQL ID (auto increment)
new lastvehSQLID


//stores current spawned vehicle
new playerSpawnedVehicle[MAX_PLAYERS] = {-1,...}

//vupgrade misc
new spareKeys[MAX_PLAYERS] = -1
new spareKeysSlot[MAX_PLAYERS] = -1
new hasgivenKey[MAX_PLAYERS]


/*
======================================================
 _                     _ _             
| |                   | (_)            
| |     ___   __ _  __| |_ _ __   __ _ 
| |    / _ \ / _` |/ _` | | '_ \ / _` |
| |___| (_) | (_| | (_| | | | | | (_| |
|______\___/ \__,_|\__,_|_|_| |_|\__, |
                                  __/ |
                                 |___/ 
								 
	Loads all vehicle data at once.
=====================================================
*/
//Request the data
loadPlayerVehicles( playerid ) {
	new query[130];
	mysql_format( sqlGameConnection, query, sizeof(query), "SELECT * FROM playervehicles WHERE pID = %d", PlayerInfo[playerid][pID] )
	mysql_pquery( sqlGameConnection, query, "setupPlayerVehicles", "i", playerid )
	return 1
}


//Collect & process the data
forward setupPlayerVehicles(playerid)

public setupPlayerVehicles(playerid) {

	if( cache_get_row_count() < 1 ) {
		return 1
	}
	
	for( new slot; slot < cache_get_row_count(); slot++ ) {
		if( slot >= PlayerInfo[playerid][pMaxCarSlots] ) {
			break
		}
	
		#define veh(%1) PlayerVehicles[playerid][slot][pv%1]
		
		veh(SQLID) = cache_get_field_content_int( slot, "ID" )
		PlayerInfo[playerid][pCarID][slot] = veh(SQLID)
		
		veh(pID) = cache_get_field_content_int( slot, "pID" )
		veh(model) = cache_get_field_content_int( slot, "model" )
		veh(x) = cache_get_field_content_float( slot, "x" )
		veh(y) = cache_get_field_content_float( slot, "y" )
		veh(z) = cache_get_field_content_float( slot, "z" )
		veh(rz) = cache_get_field_content_float( slot, "rz" )
		veh(locked) = cache_get_field_content_int( slot, "locked" )
		veh(color1) = cache_get_field_content_int( slot, "color1" )
		veh(color2) = cache_get_field_content_int( slot, "color2" )
		veh(paintjob) = cache_get_field_content_int( slot, "paintjob" )
		veh(gas) = cache_get_field_content_int( slot, "gas" )
		veh(damage) = cache_get_field_content_float( slot, "damage" )
		
		//parking & towing
		veh(parkx) = cache_get_field_content_float( slot, "parkx" )
		veh(parky) = cache_get_field_content_float( slot, "parky" )
		veh(parkz) = cache_get_field_content_float( slot, "parkz" )
		veh(parkrz) = cache_get_field_content_float( slot, "parkrz" )
		
		//upgrades
		veh(gps) = cache_get_field_content_int( slot, "gps" )
		veh(insurance) = cache_get_field_content_int( slot, "insurance" )
		veh(alarm) = cache_get_field_content_int( slot, "alarm" )
		veh(sparekey) = cache_get_field_content_int( slot, "sparekey" )
		cache_get_field_content( slot, "plate", veh(plate), sqlGameConnection, 9 )
		
		//trunk
		veh(Gun1) = cache_get_field_content_int( slot, "Gun1" )
		veh(Gun2) = cache_get_field_content_int( slot, "Gun2" )
		veh(Pot) = cache_get_field_content_int( slot, "Pot" )
		veh(Crack) = cache_get_field_content_int( slot, "Crack" )
		veh(Armor) = cache_get_field_content_int( slot, "Armor" )
		
		veh(dpanels) = cache_get_field_content_int( slot, "dpanels" )
		veh(ddoors) = cache_get_field_content_int( slot, "ddoors" )
		veh(dlights) = cache_get_field_content_int( slot, "dlights" )
		veh(dtires) = cache_get_field_content_int( slot, "dtires" )
		
		veh(impounded) = cache_get_field_content_int( slot, "impounded" )
		
		//get vehicle component data
		new compstr[50];
		for( new c; c < 14; c++ ) {
			
			format( compstr, 50, "component%d", c );
			veh(components)[c] = cache_get_field_content_int( slot, compstr );
		}
		
		lastvehSQLID = veh(SQLID)
		
		pVehiclePool++
		
		//spawn the last vehicle the player had
		if( PlayerInfo[playerid][lastCarID] > -1 ) {
			if( PlayerInfo[playerid][lastCarID] == veh(SQLID) ) {
				spawnPlayerVehicle( playerid, slot, 1 ) //1: spawns in last position, than last parked
			}
		}
		#undef veh
	}
	printf( "DEBUG: loaded playerid's %d owned vehicles, vehicle pool is: %d.", playerid, pVehiclePool )
	return 1
}
/*
====================================
  _____             _             
 / ____|           (_)            
| (___   __ ___   ___ _ __   __ _ 
 \___ \ / _` \ \ / / | '_ \ / _` |
 ____) | (_| |\ V /| | | | | (_| |
|_____/ \__,_| \_/ |_|_| |_|\__, |
                             __/ |
                            |___/
							
	Save all vehicles, or save one vehicle.
=====================================
*/

//Save all vehicles
/*savePlayerVehicles() {
	new query[450]
	for( new i; i <= GetPlayerPoolSize(); i++ ) {
		for( new slot; slot < MAX_VEHICLE_SLOTS; slot++ ) {
			#define veh(%1) PlayerVehicles[i][slot][pv%1]
			if( PlayerInfo[i][pCarID][slot] < 1 ) continue
			
			GetVehicleHealth( veh(ID), veh(damage) )
			GetVehiclePos( veh(ID), veh(x), veh(y), veh(z) )
			GetVehicleZAngle( veh(ID), veh(rz) )
			veh(vw) = GetVehicleVirtualWorld( veh(ID) )
			
			//get trunk data to save
			if( veh(spawned) ) {
				veh(Gun1) = TrunkInfo[veh(ID)][cGun1]
				veh(Gun2) = TrunkInfo[veh(ID)][cGun2]
				veh(Pot) = TrunkInfo[veh(ID)][cPot]
				veh(Crack) = TrunkInfo[veh(ID)][cCrack]
				veh(Armor) = TrunkInfo[veh(ID)][cArmor]
			}
			

			
			GetVehicleDamageStatus( veh(ID), veh(dpanels), veh(ddoors), veh(dlights), veh(dtires) )
			mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE playervehicles SET pID = %d, model = %d, x = %f, y = %f, z = %f, rz = %f, locked = %d,",
				veh(pID), veh(model), veh(x), veh(y), veh(z), veh(rz), veh(locked) )
			mysql_format( sqlGameConnection, query, sizeof( query ), "%s color1 = %d, color2 = %d, paintjob = %d, gas = %d, damage = %f, gps = %d, alarm = %d, sparekey = %d, plate = '%e',",
				query, veh(color1), veh(color2), veh(paintjob), Gas[veh(ID)], veh(damage), veh(gps), veh(alarm), veh(sparekey), veh(plate) )
			mysql_format( sqlGameConnection, query, sizeof( query ), "%s Gun1 = %d, Gun2 = %d, Pot = %d, Crack = %d, Armor = %f, parkx = %f, parky = %f, parkz = %f, parkrz = %f,",
				query, veh(Gun1), veh(Gun2), veh(Pot), veh(Crack), veh(Armor), veh(parkx), veh(parky), veh(parkz), veh(parkrz) )
			mysql_format( sqlGameConnection, query, sizeof( query ), "%s dpanels = %d, ddoors = %d, dlights = %d, dtires = %d, vw = %d, ",
				query, veh(dpanels), veh(ddoors), veh(dlights), veh(dtires), veh(vw) )
				
			//get vehicle component data
			new compstr[50];
			for( new c; c < 14; c++ ) {
				veh(components)[c] = GetVehicleComponentInSlot( veh(ID), c );
				format( compstr, 50, "component%d, ", c );
				mysql_format( sqlGameConnection, query, sizeof( query ), "%s %s = %d", query, compstr, veh(components)[c] );
			}
			mysql_format( sqlGameConnection, query, sizeof( query ), "%s WHERE ID = %d;", query, veh(SQLID) );
			
			mysql_pquery( sqlGameConnection, query ) //Want to save this in the main thread before the client closes connection
			#undef veh
		}
	}
	return 1
}*/

//Save a single vehicle
savePlayerVehicle( playerid, slot ) {
	new query[1000]
	#define veh(%1) PlayerVehicles[playerid][slot][pv%1]
	//if( PlayerInfo[playerid][pCarID][slot] != veh(pID) ) return 1
	if( !veh(spawned) ) return 1; //else negative arrays
	
	//get trunk data to save
	GetVehicleHealth( veh(ID), veh(damage) )
	GetVehiclePos( veh(ID), veh(x), veh(y), veh(z) )
	GetVehicleZAngle( veh(ID), veh(rz) )
	veh(vw) = GetVehicleVirtualWorld( veh(ID) )
	
	veh(Gun1) = TrunkInfo[veh(ID)][cGun1]
	veh(Gun2) = TrunkInfo[veh(ID)][cGun2]
	veh(Pot) = TrunkInfo[veh(ID)][cPot]
	veh(Crack) = TrunkInfo[veh(ID)][cCrack]
	veh(Armor) = TrunkInfo[veh(ID)][cArmor]
	
	GetVehicleDamageStatus( veh(ID), veh(dpanels), veh(ddoors), veh(dlights), veh(dtires) )
	veh(gas) = Gas[veh(ID)]
	
	
	mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE playervehicles SET pID = %d, model = %d, x = %f, y = %f, z = %f, rz = %f, locked = %d,",
		veh(pID), veh(model), veh(x), veh(y), veh(z), veh(rz), veh(locked) )
	mysql_format( sqlGameConnection, query, sizeof( query ), "%s color1 = %d, color2 = %d, paintjob = %d, gas = %d, damage = %f, gps = %d, alarm = %d, sparekey = %d, plate = '%e',",
		query, veh(color1), veh(color2), veh(paintjob), veh(gas), veh(damage), veh(gps), veh(alarm), veh(sparekey), veh(plate) )
	mysql_format( sqlGameConnection, query, sizeof( query ), "%s Gun1 = %d, Gun2 = %d, Pot = %d, Crack = %d, Armor = %f, parkx = %f, parky = %f, parkz = %f, parkrz = %f,",
		query, veh(Gun1), veh(Gun2), veh(Pot), veh(Crack), veh(Armor), veh(parkx), veh(parky), veh(parkz), veh(parkrz) )
	mysql_format( sqlGameConnection, query, sizeof( query ), "%s dpanels = %d, ddoors = %d, dlights = %d, dtires = %d, vw = %d, impounded = %d",
		query, veh(dpanels), veh(ddoors), veh(dlights), veh(dtires), veh(vw), veh(impounded) )
		
	//get vehicle component data
	for( new c; c < 14; c++ ) {
		PlayerVehicles[playerid][slot][pvcomponents][c] = GetVehicleComponentInSlot( veh(ID), c );
		mysql_format( sqlGameConnection, query, sizeof( query ), "%s, component%d = %d", query, c, PlayerVehicles[playerid][slot][pvcomponents][c] );
	}
	mysql_format( sqlGameConnection, query, sizeof( query ), "%s WHERE ID = %d;", query, veh(SQLID) );
	mysql_pquery( sqlGameConnection, query ) //Want to save this in the main thread before the client closes connection
	#undef veh
	return 1
}
/*
====================================================
  _____                            _             
 / ____|                          (_)            
| (___  _ __   __ ___      ___ __  _ _ __   __ _ 
 \___ \| '_ \ / _` \ \ /\ / / '_ \| | '_ \ / _` |
 ____) | |_) | (_| |\ V  V /| | | | | | | | (_| |
|_____/| .__/ \__,_| \_/\_/ |_| |_|_|_| |_|\__, |
       | |                                  __/ |
       |_|                                 |___/ 
	   
	Spawn both of the player's vehicle, or spawn one
	based on its ~index~.
====================================================
*/




//Spawn a single player vehicle
spawnPlayerVehicle( playerid, slot, lastpos = 0 ) {
		
	#define veh(%1) PlayerVehicles[playerid][slot][pv%1] //This is just a marco to reduce repetition!
	
	if( PlayerInfo[playerid][pCarID][slot] < 1 ) return 1;
	
	//If it hasn't already been spawned...
	if( !veh(spawned) && !veh(impounded) ) {
		playerSpawnedVehicle[playerid] = slot

		if( !lastpos ) {
			veh(ID) = CreateVehicle( veh(model), veh(parkx), veh(parky), veh(parkz), veh(parkrz), veh(color1), veh(color2), -1 )
		}
		else {
			veh(ID) = CreateVehicle( veh(model), veh(x), veh(y), veh(z), veh(rz), veh(color1), veh(color2), -1 )
		}
		SetVehicleVirtualWorld( veh(ID), veh(vw) )
		SetVehicleNumberPlate( veh(ID), veh(plate) )
		SetVehicleLockState( veh(ID), veh(locked) )
		engineOn[veh(ID)] = 0 //set engine vars off
		playerDisabledEngine[veh(ID)] = 1
		
		//set the trunk items
		TrunkInfo[veh(ID)][cGun1] = veh(Gun1)
		TrunkInfo[veh(ID)][cGun2] = veh(Gun2)
		TrunkInfo[veh(ID)][cPot] = veh(Pot)
		TrunkInfo[veh(ID)][cCrack] = veh(Crack)
		TrunkInfo[veh(ID)][cArmor] = veh(Armor)
		
		//set component info
		//get vehicle component data
		for( new c; c < 14; c++ ) {
			if( veh(components)[c] ) {
				AddVehicleComponent( veh(ID), veh(components)[c] );
			}
		}
		
		veh(playerID) = playerid
		veh(spawned) = 1
		
		//totalled state
		if( veh(damage) < 300.0 ) {
			veh(damage) = 305.0
		}
		
		//set the damage/gas
		UpdateVehicleDamageStatus( veh(ID), veh(dpanels), veh(ddoors), veh(dlights), veh(dtires) )
		SetVehicleHealth( veh(ID), veh(damage) )
		Gas[veh(ID)] = veh(gas)
		printf("DEBUG: Spawned playerid's %d vehicle at last position: %f, %f, %f.", playerid, veh(x), veh(y), veh(z) )
	}
	#undef veh
	return 1
}

/*
==============================================================
 _____                                       _             
|  __ \                                     (_)            
| |  | | ___  ___ _ __   __ ___      ___ __  _ _ __   __ _ 
| |  | |/ _ \/ __| '_ \ / _` \ \ /\ / / '_ \| | '_ \ / _` |
| |__| |  __/\__ \ |_) | (_| |\ V  V /| | | | | | | | (_| |
|_____/ \___||___/ .__/ \__,_| \_/\_/ |_| |_|_|_| |_|\__, |
                 | |                                  __/ |
                 |_|                                 |___/ 
	Saves, resets trunk data for the vehicle ID,
	and destroys the vehicle.
==============================================================
*/
destroyPlayerVehicle( playerid, slot ) {
	#define veh(%1) PlayerVehicles[playerid][slot][pv%1]
	if( veh(spawned) ) {
		savePlayerVehicle( playerid, slot )
		TrunkInfo[veh(ID)][cGun1] = 0
		TrunkInfo[veh(ID)][cGun2] = 0
		TrunkInfo[veh(ID)][cPot] = 0
		TrunkInfo[veh(ID)][cCrack] = 0
		TrunkInfo[veh(ID)][cArmor] = 0
		veh(spawned) = 0
		//veh(playerID) = INVALID_PLAYER_ID
		
		//get damage states
		GetVehicleDamageStatus( veh(ID), veh(dpanels), veh(ddoors), veh(dlights), veh(dtires) )
		
		//Get last position
		GetVehiclePos( veh(ID), veh(x), veh(y), veh(z) )
		GetVehicleZAngle( veh(ID), veh(rz) )
		
		//reset params for this vehicle ID
		SetVehicleParamsEx( veh(ID), 0, 0, 0, 0, 0, 0, 0 )
		DestroyVehicle( veh(ID) )
		
		for( new i; i < GetPlayerPoolSize(); i++ ) {
			if( spareKeys[i] == playerid ) {
				spareKeys[i] = -1
				spareKeysSlot[i] = -1
			}
		}
		hasgivenKey[playerid] = 0
		veh(ID) = INVALID_VEHICLE_ID
		playerSpawnedVehicle[playerid] = -1
		
		return 1
	}
	#undef veh
	return 0
}
/*
========================================================
  _____                _   _             
 / ____|              | | (_)            
| |     _ __ ___  __ _| |_ _ _ __   __ _ 
| |    | '__/ _ \/ _` | __| | '_ \ / _` |
| |____| | |  __/ (_| | |_| | | | | (_| |
 \_____|_|  \___|\__,_|\__|_|_| |_|\__, |
                                    __/ |
                                   |___/ 
	Creates a vehicle given the player it's being
	created for, and the player's vehicle slot (
	given the vehicle slot is free).
=======================================================
*/
//Give the player a vehicle.
createPlayerVehicle( playerid, slot, model, Float:x, Float:y, Float:z, Float:rz ) {
	if( pVehiclePool == MAX_OWNED_VEHICLES-1 ) return 1
	
	lastvehSQLID++
	pVehiclePool++ //increase this after - start at index 0
	#define veh(%1) PlayerVehicles[playerid][slot][pv%1]
	
	veh(pID) = PlayerInfo[playerid][pID]
	veh(model) = model
	veh(parkx) = x
	veh(parky) = y
	veh(parkz) = z
	veh(parkrz) = rz
	veh(SQLID) = lastvehSQLID
	veh(gas) = 100
	veh(gps) = 0
	veh(sparekey) = 0
	veh(insurance) = 0
	veh(alarm) = 0
	veh(damage) = 1000
	veh(playerID) = playerid
	veh(color1) = random(255);
	veh(color2) = random(255);
	GenerateRandomPlate( playerid, slot )
	PlayerInfo[playerid][pCarID][slot] = veh(SQLID)
	
	//Don't spawn it, it'll show in /myvehicles
	
	
	new query[500]
	mysql_format( sqlGameConnection, query, sizeof( query ), "INSERT INTO playervehicles SET pID = %d, model = %d, parkx = %f, parky = %f, parkz = %f, parkrz = %f, locked = 0, color1 = 0, color2 = 0, damage=1000, gas=100, plate = '%e'", PlayerInfo[playerid][pID], model, x, y, z, rz, veh(plate) )
	mysql_pquery( sqlGameConnection, query )
	
	#undef veh
	
	return 1
}

/*
========================================
 _    _           __       _ 
| |  | |         / _|     | |
| |  | |___  ___| |_ _   _| |
| |  | / __|/ _ \  _| | | | |
| |__| \__ \  __/ | | |_| | |
 \____/|___/\___|_|  \__,_|_|
	
	• getVehicleIndex returns the array index
		of PlayerVehicles[][], the more information
		that is given, the quicker it runs.
	
	• SetVehicleLockState, SetVehicleAlarmState,
		SetVehicleEngineState - toggles the vehicle's x,
		given the vehicleid.
=========================================
*/

GetPlayerFreeCarslot( playerid ) {

	for( new i; i < PlayerInfo[playerid][pMaxCarSlots]; i++ ) {
		if( PlayerInfo[playerid][pCarID][i] < 1 ) {
			return i
		}
	}
	return -1
}

IsVehicleOccupied( vehicleid ) {
	for( new p; p <= GetPlayerPoolSize(); p++ ) {
		if( IsPlayerConnected( p ) ) {
			new pstate = GetPlayerState( p )
			if( pstate == PLAYER_STATE_DRIVER || pstate == PLAYER_STATE_PASSENGER ) {
				if( GetPlayerVehicleID( p ) == vehicleid ) {
					return 1
				}
			}
		}
	}
	return 0
}

RandomChars( characters[], size = sizeof( characters ) ) {
	new const charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	for( new c; c < size; c++ ) {
		characters[c] = charset[random(sizeof(charset) - 1)];
	}
	return 1;
}

GenerateRandomPlate( playerid, slot ) {
	RandomChars( PlayerVehicles[playerid][slot][pvplate], 9 );
	return 1
}

SetVehicleLockState( vehicleid, lockstate ) {
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx( vehicleid, engine, lights, alarm, doors, bonnet, boot, objective )
	SetVehicleParamsEx( vehicleid, engine, lights, alarm, lockstate, bonnet, boot, objective )
	gCarLock[vehicleid] = lockstate
	return 1
}
new vehAlarm[MAX_VEHICLES]
SetVehicleAlarmState( vehicleid, toggle = 1 ) {
	if( toggle == 0 ) { 
		vehAlarm[vehicleid] = 0;
		return 1;
	}
	vehAlarm[vehicleid] = gettime();
	return 1
}
new vehicleLights[MAX_VEHICLES]
SetVehicleLightsState( vehicleid, lightstate ) {
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx( vehicleid, engine, lights, alarm, doors, bonnet, boot, objective )
	SetVehicleParamsEx( vehicleid, engine, lightstate, alarm, doors, bonnet, boot, objective )
	vehicleLights[vehicleid] = lightstate
	return 1
}

SetVehicleEngineState( vehicleid, enginestate ) {
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx( vehicleid, engine, lights, alarm, doors, bonnet, boot, objective )
	SetVehicleParamsEx( vehicleid, enginestate, lights, alarm, doors, bonnet, boot, objective )
	engineOn[vehicleid] = enginestate
	return 1
}

public OnVehiclePaintjob( playerid, vehicleid, paintjobid ) {
	new pvid = playerSpawnedVehicle[playerid]
	if( pvid > -1 ) {
		if( PlayerVehicles[playerid][pvid][pvID] == vehicleid ) {
			printf("paintjobtest");
			PlayerVehicles[playerid][pvid][pvpaintjob] = paintjobid;
		}
	}
	return 1;
}
public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	for( new i; i <= GetPlayerPoolSize(); i++ ) {
		new slot = playerSpawnedVehicle[i]
		if( PlayerVehicles[i][slot][pvID] == vehicleid ) {
			PlayerVehicles[i][slot][pvcolor1] = color1
			PlayerVehicles[i][slot][pvcolor2] = color2
			break
		}
	}
	return 1
}


enum ca_Info
{
	c_ID,
	c_Model,
	Float:c_ParkPos[4],
	Float:c_LastPos[4],
	c_Component[14],
	c_Color[2],
	c_PaintJob,
	c_Alarm,
	c_Insurance,
	c_Plate[10],
	c_GPS,
	c_Tow,
	c_Keys,
	c_Lock,
	c_Price,
	c_Trunk,
	c_Gun[2],
	c_Pot,
	c_Crack,
	Float:c_Armor,
	c_VW,
	c_Impounded
};


/*CheckVehicleTable() {
	mysql_pquery( sqlGameConnection, "SELECT ID FROM playervehicles;", "GetPlayerOldvehicleDatav", "" );
	return 1;
}
forward GetPlayerOldvehicleDatav();
public GetPlayerOldvehicleDatav() {
	if( cache_get_row_count() ) return 1; //don't convert if there already exists vehicles
	mysql_pquery( sqlGameConnection, "SELECT CKey1, CKey2, Name, ID FROM players;", "ConvertPlayerVehicles", "" );
	return 1;
}

		
//vehicle data
new vehicleDatav[ca_Info];
		
forward ConvertPlayerVehicles();
public ConvertPlayerVehicles() {
	new rows, fields;
	cache_get_data( rows, fields );
	printf( "Converting %d player vehicle rows." );
	if( !rows ) return 1;
	
	new key[2], Name[MAX_PLAYER_NAME+1], id;
	for( new i; i < rows; i++ ) {
		
		id = cache_get_field_content_int( i, "ID" );
		key[0] = cache_get_field_content_int( i, "CKey1" );
		key[1] = cache_get_field_content_int( i, "CKey2" );
		cache_get_field_content( i, "Name", Name, sqlGameConnection, MAX_PLAYER_NAME+1 );

		
		new query[1000];
		for( new d; d < sizeof( key ); d++ ) {
			if( key[d] ) {
				new strFile[48];
				format(strFile, sizeof(strFile), "cars/%d.ini", key[d] );
				if(!fexist(strFile)) {
					printf("ConvertPlayerVehicles(SQL;%d) > Car %d file doesn't exist (carkeyID: %d)", id, d, key[d]);
					continue;
				}
				
				new File:fCar = fopen(strFile, io_read);
				new strData[128], strKey[128];
				while(fread(fCar, strData, sizeof(strData)))
				{
					strKey = ini_GetKey(strData);
					if(strcmp(strKey, "Model") == 0) vehicleDatav[c_Model] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "X") == 0) vehicleDatav[c_ParkPos][0] = floatstr(ini_GetValue(strData));
					else if(strcmp(strKey, "Y") == 0) vehicleDatav[c_ParkPos][1] = floatstr(ini_GetValue(strData));
					else if(strcmp(strKey, "Z") == 0) vehicleDatav[c_ParkPos][2] = floatstr(ini_GetValue(strData));
					else if(strcmp(strKey, "A") == 0) vehicleDatav[c_ParkPos][3] = floatstr(ini_GetValue(strData));
					else if(strcmp(strKey, "LX") == 0) vehicleDatav[c_LastPos][0] = floatstr(ini_GetValue(strData));
					else if(strcmp(strKey, "LY") == 0) vehicleDatav[c_LastPos][1] = floatstr(ini_GetValue(strData));
					else if(strcmp(strKey, "LZ") == 0) vehicleDatav[c_LastPos][2] = floatstr(ini_GetValue(strData));
					else if(strcmp(strKey, "LA") == 0) vehicleDatav[c_LastPos][3] = floatstr(ini_GetValue(strData));
					else if(strcmp(strKey, "C1") == 0) vehicleDatav[c_Color][0] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "C2") == 0) vehicleDatav[c_Color][1] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "PJ") == 0) vehicleDatav[c_PaintJob] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "comp0") == 0) vehicleDatav[c_Component][0] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "comp1") == 0) vehicleDatav[c_Component][1] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "comp2") == 0) vehicleDatav[c_Component][2] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "comp3") == 0) vehicleDatav[c_Component][3] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "comp4") == 0) vehicleDatav[c_Component][4] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "comp5") == 0) vehicleDatav[c_Component][5] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "comp6") == 0) vehicleDatav[c_Component][6] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "comp7") == 0) vehicleDatav[c_Component][7] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "comp8") == 0) vehicleDatav[c_Component][8] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "comp9") == 0) vehicleDatav[c_Component][9] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "comp10") == 0) vehicleDatav[c_Component][10] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "comp11") == 0) vehicleDatav[c_Component][11] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "comp12") == 0) vehicleDatav[c_Component][12] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "comp13") == 0) vehicleDatav[c_Component][13] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "alarm") == 0) vehicleDatav[c_Alarm] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "ins") == 0) vehicleDatav[c_Insurance] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "plate") == 0) format(vehicleDatav[c_Plate], 10, ini_GetValue(strData));
					else if(strcmp(strKey, "gps") == 0) vehicleDatav[c_GPS] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "tow") == 0) vehicleDatav[c_Tow] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "keys") == 0) vehicleDatav[c_Keys] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "lock") == 0) vehicleDatav[c_Lock] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "price") == 0) vehicleDatav[c_Price] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "virtualworld") == 0) vehicleDatav[c_VW] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "trunk_opened") == 0) vehicleDatav[c_Trunk] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "trunk_gun1") == 0) vehicleDatav[c_Gun][0] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "trunk_gun2") == 0) vehicleDatav[c_Gun][1] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "trunk_pot") == 0) vehicleDatav[c_Pot] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "trunk_crack") == 0) vehicleDatav[c_Crack] = strvalEx(ini_GetValue(strData));
					else if(strcmp(strKey, "trunk_armor") == 0) vehicleDatav[c_Armor] = floatstr(ini_GetValue(strData));
					else if(strcmp(strKey, "impounded") == 0) vehicleDatav[c_Impounded] = strvalEx(ini_GetValue(strData));
				}
				fclose(fCar);
				
				if( vehicleDatav[c_Model] < 0 ) {
					printf("Unable to convert player: %s's vehicle, model: %d", Name, vehicleDatav[c_Model] );
					continue;
				}
				
				if( vehicleDatav[c_Alarm] < 0 ) vehicleDatav[c_Alarm] = 0;
				if( vehicleDatav[c_GPS] < 0 ) vehicleDatav[c_GPS] = 0;
				if( vehicleDatav[c_Insurance] < 0 ) vehicleDatav[c_Insurance] = 0;
				if( vehicleDatav[c_Keys] < 0 ) vehicleDatav[c_Keys] = 0;
				if( vehicleDatav[c_Lock] < 0 ) vehicleDatav[c_Lock] = 0;
				if( vehicleDatav[c_Gun][0] < 0 ) vehicleDatav[c_Gun][0] = 0;
				if( vehicleDatav[c_Gun][1] < 0 ) vehicleDatav[c_Gun][1] = 0;
				if( vehicleDatav[c_Pot] < 0 ) vehicleDatav[c_Pot] = 0;
				if( vehicleDatav[c_Crack] < 0 ) vehicleDatav[c_Crack] = 0;
				if( vehicleDatav[c_Armor] < 0.0 ) vehicleDatav[c_Armor] = 0.0;
				if( vehicleDatav[c_Color][0] < 0 ) vehicleDatav[c_Color][0] = 0;
				if( vehicleDatav[c_Color][1] < 0 ) vehicleDatav[c_Color][1] = 0;
				if( vehicleDatav[c_PaintJob] < 0 ) vehicleDatav[c_PaintJob] = 0;
				if( vehicleDatav[c_VW] < 0 ) vehicleDatav[c_VW] = 0;
				if( strlen( vehicleDatav[c_Plate] ) < 2 ) {
					RandomChars( vehicleDatav[c_Plate] );
				}
				
				mysql_format( sqlGameConnection, query, sizeof( query ), "INSERT INTO playervehicles SET pID = %d, model = %d, parkx = %f, parky = %f, parkz = %f, parkrz = %f,",
					id, vehicleDatav[c_Model], vehicleDatav[c_ParkPos][0], vehicleDatav[c_ParkPos][1], vehicleDatav[c_ParkPos][2], vehicleDatav[c_ParkPos][3] );
				mysql_format( sqlGameConnection, query, sizeof( query ), "%s alarm = %d, gps = %d, plate = '%e', insurance = %d, sparekey = %d,", 
					query, vehicleDatav[c_Alarm], vehicleDatav[c_GPS], vehicleDatav[c_Plate], vehicleDatav[c_Insurance], vehicleDatav[c_Keys] );
				mysql_format( sqlGameConnection, query, sizeof( query ), "%s locked = %d, Gun1 = %d, Gun2 = %d, Pot = %d, Crack = %d, Armor = %f,",
					query, vehicleDatav[c_Lock], vehicleDatav[c_Gun][0], vehicleDatav[c_Gun][1], vehicleDatav[c_Pot], vehicleDatav[c_Crack], vehicleDatav[c_Armor] );
				mysql_format( sqlGameConnection, query, sizeof( query ), "%s impounded = 0, color1 = %d, color2 = %d, paintjob = %d, vw = %d, damage=1000, gas=100", 
					query, vehicleDatav[c_Color][0], vehicleDatav[c_Color][1], vehicleDatav[c_PaintJob], vehicleDatav[c_VW] );
				new compstr[100];
				for( new c; c < 14; c++ ) {
					if( vehicleDatav[c_Component][c] ) {
						format( compstr, 100, " component%d", c );
						mysql_format( sqlGameConnection, query, sizeof( query ), "%s, %s = %d", query, compstr, vehicleDatav[c_Component][c] );
					}
				}
				mysql_format( sqlGameConnection, query, sizeof( query ), "%s;", query );
				mysql_pquery( sqlGameConnection, query );
				printf("Converting %s's cars::%s", Name, query );
				mysql_format( sqlGameConnection, query, sizeof( query), "SELECT ID FROM playervehicles WHERE pID = %d;", id );
				mysql_pquery( sqlGameConnection, query, "CheckPlayerCars", "i", id );

			}
		}
	}
	mysql_pquery( sqlGameConnection, "UPDATE players SET maxcarslots = 3;" );
	printf( "Vehicle conversion finished." )
	return 1;
}
forward CheckPlayerCars( id );
public CheckPlayerCars( id ) {
	new rows, fields;
	cache_get_data( rows, fields );
	if( !rows ) {
		printf("Couldn't insert converted player vehicle player SQL ID %d", id );
		return 1;
	}
	return 1;
}*/
#include "inc\vehicles\commands.inc"
#include "inc\vehicles\impounds.inc"
#include "inc\vehicles\hooks.inc"
//#include "../gamemodes/inc/trunk.inc"