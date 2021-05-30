/*
 _____       _                 _            _   _             
|_   _|     | |               | |          | | (_)            
  | |  _ __ | |_ _ __ ___   __| |_   _  ___| |_ _  ___  _ __  
  | | | '_ \| __| '__/ _ \ / _` | | | |/ __| __| |/ _ \| '_ \ 
 _| |_| | | | |_| | | (_) | (_| | |_| | (__| |_| | (_) | | | |
|_____|_| |_|\__|_|  \___/ \__,_|\__,_|\___|\__|_|\___/|_| |_|
                                                              
	This is the system for player owned vehicles.
	
	Related files are in /gamemodes/inc/vehicles.
		-> ./commands/players.inc ~ has all player owned vehicle commands.
							command related variables are here too!
		-> ./commands/admin.inc ~ has all administrator commands
		-> ./commands/impound.inc ~ impound related commands.
		
	It's important to note: only the player's spawned vehicle will be saved.
	
	What's in this file?:
	
		Structure,
			• definitions & variables
		Saving,
			• savePlayerVehicle( index )
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


//custom color for new systems
#define COLOR_VEHICLES 0xFFB76FFF

#define MAX_SPAWNED_VEHICLES 1

//Structure of each vehicle.
enum pvehicleDatav {
	pvID, 			//Array index
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
new PlayerVehicles[MAX_PLAYERS][MAX_VEHICLE_SLOTS][pvehicleDatav];

//Stores the last SQL ID (auto increment)
new lastvehSQLID;


//stores current spawned vehicle
new playerSpawnedVehicle[MAX_PLAYERS] = {-1,...};

//vupgrade misc
new spareKeys[MAX_PLAYERS] = -1;
new spareKeysSlot[MAX_PLAYERS] = -1;
new hasgivenKey[MAX_PLAYERS];


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

//fetch the last "ID" of auto increment from playervehicles on GamemodeInit
fetchLastSQLID() {
	mysql_pquery( sqlGameConnection, "SELECT ID FROM playervehicles ORDER BY ID DESC LIMIT 1;", "SetLastSQLID" );
	return 1;
}

forward SetLastSQLID();
public SetLastSQLID() {
	if( cache_num_rows() < 1 ) lastvehSQLID = 0; //this assumes the table's auto increment starts at 0
	lastvehSQLID = cache_get_field_content_int( 0, "ID" );
	return 1;
}

//Request the data
loadPlayerVehicles( playerid ) {
	new query[130];
	mysql_format( sqlGameConnection, query, sizeof(query), "SELECT * FROM playervehicles WHERE pID = %d;", PlayerInfo[playerid][pID] );
	mysql_pquery( sqlGameConnection, query, "setupPlayerVehicles", "i", playerid );
	return 1;
}


//Collect & process the data
forward setupPlayerVehicles(playerid);

public setupPlayerVehicles(playerid) {

	if( cache_get_row_count() < 1 ) {
		return 1;
	}
	
	for( new slot; slot < cache_get_row_count(); slot++ ) {
	
		//player has no more car slots
		if( slot >= PlayerInfo[playerid][pMaxCarSlots] ) {
			break;
		}
		#define veh(%1) PlayerVehicles[playerid][slot][pv%1]
		
		veh(SQLID) = cache_get_field_content_int( slot, "ID" );
		veh(pID) = cache_get_field_content_int( slot, "pID" );
		veh(model) = cache_get_field_content_int( slot, "model" );
		veh(x) = cache_get_field_content_float( slot, "x" );
		veh(y) = cache_get_field_content_float( slot, "y" );
		veh(z) = cache_get_field_content_float( slot, "z" );
		veh(rz) = cache_get_field_content_float( slot, "rz" );
		veh(locked) = cache_get_field_content_int( slot, "locked" );
		veh(color1) = cache_get_field_content_int( slot, "color1" );
		veh(color2) = cache_get_field_content_int( slot, "color2" );
		veh(paintjob) = cache_get_field_content_int( slot, "paintjob" );
		veh(gas) = cache_get_field_content_int( slot, "gas" );
		veh(damage) = cache_get_field_content_float( slot, "damage" );
		
		//parking & towing;
		veh(parkx) = cache_get_field_content_float( slot, "parkx" );
		veh(parky) = cache_get_field_content_float( slot, "parky" );
		veh(parkz) = cache_get_field_content_float( slot, "parkz" );
		veh(parkrz) = cache_get_field_content_float( slot, "parkrz" );
		
		//upgrades
		veh(gps) = cache_get_field_content_int( slot, "gps" );
		veh(insurance) = cache_get_field_content_int( slot, "insurance" );
		veh(alarm) = cache_get_field_content_int( slot, "alarm" );
		veh(sparekey) = cache_get_field_content_int( slot, "sparekey" );
		cache_get_field_content( slot, "plate", veh(plate), sqlGameConnection, 9 );
		
		//trunk
		veh(Gun1) = cache_get_field_content_int( slot, "Gun1" );
		veh(Gun2) = cache_get_field_content_int( slot, "Gun2" );
		veh(Pot) = cache_get_field_content_int( slot, "Pot" );
		veh(Crack) = cache_get_field_content_int( slot, "Crack" );
		veh(Armor) = cache_get_field_content_int( slot, "Armor" );
		
		veh(dpanels) = cache_get_field_content_int( slot, "dpanels" );
		veh(ddoors) = cache_get_field_content_int( slot, "ddoors" );
		veh(dlights) = cache_get_field_content_int( slot, "dlights" );
		veh(dtires) = cache_get_field_content_int( slot, "dtires" );
		
		veh(impounded) = cache_get_field_content_int( slot, "impounded" );
		
		//get vehicle component data
		new compstr[50];
		for( new c; c < 14; c++ ) {
			
			format( compstr, 50, "component%d", c );
			veh(components)[c] = cache_get_field_content_int( slot, compstr );
		}
		
		
		//spawn the last vehicle the player had
		if( PlayerInfo[playerid][lastCarID] > -1 ) {
			if( PlayerInfo[playerid][lastCarID] == veh(SQLID) ) {
				spawnPlayerVehicle( playerid, slot, 1 ); //1: spawns in last position, than last parked
			}
		}
		#undef veh
	}
	return 1;
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

//Save a single vehicle
savePlayerVehicle( playerid, slot ) {
	new query[1000];
	#define veh(%1) PlayerVehicles[playerid][slot][pv%1]
	if( !veh(spawned) || !IsValidVehicle( veh(ID) ) ) return 1; //else negative arrays
	
	//get trunk data to save
	GetVehicleHealth( veh(ID), veh(damage) );
	GetVehiclePos( veh(ID), veh(x), veh(y), veh(z) );
	GetVehicleZAngle( veh(ID), veh(rz) );
	veh(vw) = GetVehicleVirtualWorld( veh(ID) );
	
	veh(Gun1) = TrunkInfo[veh(ID)][cGun1];
	veh(Gun2) = TrunkInfo[veh(ID)][cGun2];
	veh(Pot) = TrunkInfo[veh(ID)][cCannabis];
	veh(Crack) = TrunkInfo[veh(ID)][cCocaine];
	veh(Armor) = TrunkInfo[veh(ID)][cArmor];
	
	GetVehicleDamageStatus( veh(ID), veh(dpanels), veh(ddoors), veh(dlights), veh(dtires) );
	veh(gas) = Gas[veh(ID)];
	
	
	mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE playervehicles SET pID = %d, model = %d, x = %f, y = %f, z = %f, rz = %f, locked = %d,",
		veh(pID), veh(model), veh(x), veh(y), veh(z), veh(rz), veh(locked) );
	mysql_format( sqlGameConnection, query, sizeof( query ), "%s color1 = %d, color2 = %d, paintjob = %d, gas = %d, damage = %f, gps = %d, alarm = %d, sparekey = %d, insurance = %d, plate = '%e',",
		query, veh(color1), veh(color2), veh(paintjob), veh(gas), veh(damage), veh(gps), veh(alarm), veh(sparekey), veh(insurance), veh(plate) );
	mysql_format( sqlGameConnection, query, sizeof( query ), "%s Gun1 = %d, Gun2 = %d, Pot = %d, Crack = %d, Armor = %f, parkx = %f, parky = %f, parkz = %f, parkrz = %f,",
		query, veh(Gun1), veh(Gun2), veh(Pot), veh(Crack), veh(Armor), veh(parkx), veh(parky), veh(parkz), veh(parkrz) );
	mysql_format( sqlGameConnection, query, sizeof( query ), "%s dpanels = %d, ddoors = %d, dlights = %d, dtires = %d, vw = %d, impounded = %d",
		query, veh(dpanels), veh(ddoors), veh(dlights), veh(dtires), veh(vw), veh(impounded) );
		
	//get vehicle component data
	for( new c; c < 14; c++ ) {
		PlayerVehicles[playerid][slot][pvcomponents][c] = GetVehicleComponentInSlot( veh(ID), c );
		mysql_format( sqlGameConnection, query, sizeof( query ), "%s, component%d = %d", query, c, PlayerVehicles[playerid][slot][pvcomponents][c] );
	}
	mysql_format( sqlGameConnection, query, sizeof( query ), "%s WHERE ID = %d;", query, veh(SQLID) );
	mysql_pquery( sqlGameConnection, query ); //Want to save this in the main thread before the client closes connection
	#undef veh
	return 1;
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
	
	if( veh(SQLID) < 1 ) return 1; //no vehicle exists in slot 
	
	
	//If it hasn't already been spawned...
	if( !veh(spawned) && !veh(impounded) ) {
		playerSpawnedVehicle[playerid] = slot;

		if( !lastpos ) {
			veh(ID) = CreateVehicle( veh(model), veh(parkx), veh(parky), veh(parkz), veh(parkrz), veh(color1), veh(color2), -1 );
		}
		else {
			veh(ID) = CreateVehicle( veh(model), veh(x), veh(y), veh(z), veh(rz), veh(color1), veh(color2), -1 );
		}
		SetVehicleVirtualWorld( veh(ID), veh(vw) );
		SetVehicleNumberPlate( veh(ID), veh(plate) );
		SetVehicleLockState( veh(ID), veh(locked) );
		
		if( IsABicycle( veh(ID) ) ) engineOn[veh(ID)] = 1; //set engine vars off
		SetVehicleEngineState( veh(ID), engineOn[veh(ID)] );
		playerDisabledEngine[veh(ID)] = 1;
		
		//set the trunk items
		TrunkInfo[veh(ID)][cGun1] = veh(Gun1);
		TrunkInfo[veh(ID)][cGun2] = veh(Gun2);
		TrunkInfo[veh(ID)][cCannabis] = veh(Pot);
		TrunkInfo[veh(ID)][cCocaine] = veh(Crack);
		TrunkInfo[veh(ID)][cArmor] = veh(Armor);
		
		//set component info
		//get vehicle component data
		for( new c; c < 14; c++ ) {
			if( veh(components)[c] ) {
				AddVehicleComponent( veh(ID), veh(components)[c] );
			}
		}
		
		veh(spawned) = 1;
		
		//totalled state
		if( veh(damage) < 300.0 ) {
			SetVehicleEngineState( veh(ID), 0 );
			veh(damage) = 305.0;
		}
		
		//set the damage/gas
		UpdateVehicleDamageStatus( veh(ID), veh(dpanels), veh(ddoors), veh(dlights), veh(dtires) );
		SetVehicleHealth( veh(ID), veh(damage) );
		ChangeVehiclePaintjob( veh(ID), veh(paintjob) );
		Gas[veh(ID)] = veh(gas);
	}
	#undef veh
	return 1;
}

GetPlayerSpawnedVehicleSlot( playerid ) {
	return playerSpawnedVehicle[playerid];
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
		//save the vehicle first
		savePlayerVehicle( playerid, slot );
		
		TrunkInfo[veh(ID)][cGun1] = 0;
		TrunkInfo[veh(ID)][cGun2] = 0;
		TrunkInfo[veh(ID)][cCannabis] = 0;
		TrunkInfo[veh(ID)][cCocaine] = 0;
		TrunkInfo[veh(ID)][cArmor] = 0;
		TrunkInfo[veh(ID)][cFoodtray] = 0;

		if(Neon[veh(ID)]) { DestroyDynamicObjectEx(Neon[veh(ID)]); Neon[veh(ID)] = 0; } //destroy neons
		if(Neon2[veh(ID)]) { DestroyDynamicObjectEx(Neon2[veh(ID)]); Neon2[veh(ID)] = 0; }
		if(Neon3[veh(ID)]) { DestroyDynamicObjectEx(Neon3[veh(ID)]); Neon3[veh(ID)] = 0; }
		if(Neon4[veh(ID)]) { DestroyDynamicObjectEx(Neon4[veh(ID)]); Neon4[veh(ID)] = 0; }
		if(TaxiSign[veh(ID)]) { DestroyDynamicObjectEx(TaxiSign[veh(ID)]); TaxiSign[veh(ID)] = 0; } //destroy taxi sign
		if(SirenObject[veh(ID)])	//destroy siren
		{
			DestroyDynamicObjectEx(SirenObject[veh(ID)]);
			Siren[veh(ID)] = 0;
			SirenObject[veh(ID)] = 0;
		}
		
		//get damage states
		GetVehicleDamageStatus( veh(ID), veh(dpanels), veh(ddoors), veh(dlights), veh(dtires) );
		
		//Get last position
		GetVehiclePos( veh(ID), veh(x), veh(y), veh(z) );
		GetVehicleZAngle( veh(ID), veh(rz) );
		
		//reset params for this vehicle ID
		SetVehicleParamsEx( veh(ID), 0, 0, 0, 0, 0, 0, 0 );
		
		DestroyVehicle( veh(ID) );
		veh(spawned) = 0;
		veh(ID) = INVALID_VEHICLE_ID;
		playerSpawnedVehicle[playerid] = -1;
		
		for( new i; i < GetPlayerPoolSize(); i++ ) {
			if( spareKeys[i] == playerid ) {
				spareKeys[i] = -1;
				spareKeysSlot[i] = -1;
			}
		}
		hasgivenKey[playerid] = 0;
		
		return 1;
	}
	#undef veh
	return 0;
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
createPlayerVehicle( playerid, slot, model, Float:x, Float:y, Float:z, Float:rz, col1 = -1, col2 = -1 ) {
	lastvehSQLID++;
	
	#define veh(%1) PlayerVehicles[playerid][slot][pv%1]
	
	//Reset the variables first
	ResetPlayerVehicleSlot( playerid, slot );
	
	//These are most important
	//veh(SQLID) = lastvehSQLID;
	veh(pID) = PlayerInfo[playerid][pID];
	
	veh(model) = model;
	veh(parkx) = x;
	veh(parky) = y;
	veh(parkz) = z;
	veh(parkrz) = rz;
	veh(gas) = 100;
	veh(damage) = 1000;
	
	if( col1 == -1 ) {
		col1 = random(255);
	}
	if( col2 == -1 ) {
		col2 = random(255);
	}
	veh(color1) = col1;
	veh(color2) = col2;
	veh(paintjob) = 3;
	GenerateRandomPlate( playerid, slot );
	
	new 
		query[500];
	mysql_format( sqlGameConnection, query, sizeof( query ), "INSERT INTO playervehicles SET pID = %d, model = %d, parkx = %f, parky = %f, parkz = %f, parkrz = %f, locked = 0, color1 = 0, color2 = 0, damage=1000, gas=100, plate = '%e'", PlayerInfo[playerid][pID], model, x, y, z, rz, veh(plate) );

	mysql_query(sqlGameConnection, query);

	mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM playervehicles WHERE pID = %d ORDER BY ID DESC LIMIT 1", PlayerInfo[playerid][pID]);
	
	new Cache:result;
	result = mysql_query(sqlGameConnection, query);	

	if( cache_num_rows() < 1 ) {
		printf("Deleted car slot %i from %s", slot, PlayerOOCName(playerid));
		ResetPlayerVehicleSlot( playerid, slot );
		cache_delete(result);
		return 1;
	}
	printf("assigned car to %s in slot %i", PlayerOOCName(playerid), slot);
	veh(SQLID) = cache_get_field_content_int( 0, "ID" );
	
	#undef veh
	
	cache_delete(result);
	
	return 1;
}

//=======[ PERMANENTLY DELETING ]=======
ResetPlayerVehicleSlot( playerid, slot ) {
	#define veh(%0) PlayerVehicles[playerid][slot][pv%0]
	
	if( veh(spawned) ) {
		destroyPlayerVehicle( playerid, slot );
	}
	
	if( veh(SQLID) > 0 ) {
		new query[100];
		mysql_format( sqlGameConnection, query, sizeof( query ), "DELETE FROM playervehicles WHERE ID = %d;", veh(SQLID) );
		mysql_pquery( sqlGameConnection, query );
	}
	
	for( new i; pvehicleDatav:i < pvehicleDatav; i++ ) {
		PlayerVehicles[playerid][slot][pvehicleDatav:i] = 0;
	}
	
	
	#undef veh
	return 1;
}

//Trunk info
/*
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
*/
#include "inc\vehicles\useful.inc"
#include "inc\vehicles\commands\player.inc"
#include "inc\vehicles\commands\admin.inc"
#include "inc\vehicles\commands\impound.inc"
#include "inc\vehicles\hooks.inc"
//#include "../gamemodes/inc/trunk.inc"