
#if defined P_VEHICLES_INC
#else

#define P_VEHICLES_INC

//===
#define P_VEHICLES_TEST 1

//custom color for new systems
#define COLOR_VEHICLES 0x449b41FF
#define MAX_SPAWN_SLOTS 3 //3: VIP 2: normal player

//TODO: /dropcar restriction, dealerships

//NOTE: ONLY, AND I MEAN ONLY KEEP VARIABLES IN HERE WHICH WILL BE USED MORE THAN ONCE
enum pvehicleDatav {
    pvID,       //vehicle ID from createvehicle(..)
	pvSID,		//SQL ID of the vehicle in the DB.
	pvmodel,
	pvcolor1,
	pvcolor2,
	pvpaintjob,
	pvlocked,
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
	
	pvvw
}

//Stores all vehicle data.
new PlayerVehicles[MAX_PLAYERS][MAX_SPAWN_SLOTS][pvehicleDatav];

//Vehicle Destroy Reason
enum VD_REASON {
    VDPLAYER_DISCONNECT,
    VDPLAYER_ISSUE,
}

forward CreatePlayerVehicle(playerid, model, Float:x, Float:y, Float:z, Float:rz, color1, color2);
forward SpawnPlayerVehicle(playerid, vsql);
forward DestroyPlayerVehicle(playerid, slot, VD_REASON:reason);





//junk
SetVehicleLockState( vehicleid, lockstate ) {
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx( vehicleid, engine, lights, alarm, doors, bonnet, boot, objective );
	SetVehicleParamsEx( vehicleid, engine, lights, alarm, lockstate, bonnet, boot, objective );
	gCarLock[vehicleid] = lockstate;
	return 1;
}
SetVehicleEngineState( vehicleid, enginestate ) {
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx( vehicleid, engine, lights, alarm, doors, bonnet, boot, objective );
	SetVehicleParamsEx( vehicleid, enginestate, lights, alarm, doors, bonnet, boot, objective );
	engineOn[vehicleid] = enginestate;
	return 1;
}
IsVehicleOccupied( vehicleid ) {
    #pragma unused vehicleid
	/*for( new p; p <= GetPlayerPoolSize(); p++ ) {
		if( IsPlayerConnected( p ) ) {
			new pstate = GetPlayerState( p );
			if( pstate == PLAYER_STATE_DRIVER || pstate == PLAYER_STATE_PASSENGER ) {
				if( GetPlayerVehicleID( p ) == vehicleid ) {
					return 1;
				}
			}
		}
	}*/
	return 0;
}




//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
/*  CREATING PLAYER VEHICLE
    INSERT, then create the vehicle if there's a free slot otherwise notify the player.
*/
public CreatePlayerVehicle(playerid, model, Float:x, Float:y, Float:z, Float:rz, color1, color2) {

    //todo: generate random plate
    new plate[20];
    new query[500];

    //once vehicle is inserted into the database, get SID and try spawn
    inline OnInsertPlayerVehicle(pid, vmodel) {
        SendClientMessage(playerid, COLOR_VEHICLES, "Vehicle has been added to the database.");
        CreatePlayerVehicleEx(pid, vmodel);
    }

    mysql_format(sqlGameConnection, query, sizeof query,
"INSERT INTO playervehicles \
SET pID = %d, model = %d, parkx = %f, parky = %f, parkz = %f, parkrz = %f, locked = 0, color1 = 0, color2 = 0, damage=1000, gas=50, plate = '%e'",
    PlayerInfo[playerid][pID], model, x, y, z, rz, plate
    );

    mysql_pquery_inline(sqlGameConnection, query, using inline OnInsertPlayerVehicle, "ii", playerid, model);
    return 1;
}


//helper function
static CreatePlayerVehicleEx(playerid, model) {

    SendClientMessage(playerid, COLOR_VEHICLES, "Checking database...");
    inline OnGetPlayerVehicleSID(pid) {
        new string[144];
        format(string, sizeof string, "Found %d vehicles belonging to player with model.", cache_get_row_count());
        SendClientMessage(playerid, COLOR_VEHICLES, string);

        if(cache_get_row_count()) {
            new id = cache_get_field_content_int(0, "ID");

            SendClientMessage(pid, COLOR_VEHICLES, "We have located your vehicle. Please wait while we prepare your vehicle. . .");
            SpawnPlayerVehicle(pid, id);
        }
        else {
            //log
        }
    }
    
    new query[120];
    mysql_format(sqlGameConnection, query, sizeof query, "SELECT ID FROM playervehicles WHERE model = %d LIMIT 1;", model);
    mysql_pquery_inline(sqlGameConnection, query, using inline OnGetPlayerVehicleSID, "i", playerid);
    return 1;
}














//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
/*  SPAWNING VEHICLES
    1. Get a free slot
    2. Get data from MySQL & store to pVehicles ONLY IF it'll be reused later.
    3. Setup the vehicle
*/

public SpawnPlayerVehicle(playerid, vsql) {
    //where to spawn the vehicle?
    new Float:x, Float:y, Float:z, Float:rz;
    //keep damage as local vars: reduces size of playervehicles enum + it's useless after first spawn
    new dpanels, ddoors, dlights, dtires, gas;


    //get a free slot
    new slot = MAX_SPAWN_SLOTS+1;
    for(new i = 0; i < MAX_SPAWN_SLOTS; i++) {
        if(!PlayerVehicles[playerid][i][pvSID]) { //invalid SID = 0
            slot = i;
        }
    }

    if(slot == MAX_SPAWN_SLOTS+1) {
        SendClientMessage(playerid, COLOR_VEHICLES, "I'm sorry, you're going to have to park one of your vehicles in order to spawn this vehicle (/v park).");
    }
    SendClientMessage(playerid, COLOR_VEHICLES, "Found available vehicle slot. Fetching vehicle...");

    //Get the data from MySQL: playervehicles table
    inline SpawnPlayerVehicleEx(pid) {
        if(cache_get_row_count()) {

            #define veh(%1) PlayerVehicles[pid][slot][pv%1]
            veh(SID) = cache_get_field_content_int(0, "ID");
            x = cache_get_field_content_int(0, "x");
            y = cache_get_field_content_int(0, "y");
            z = cache_get_field_content_int(0, "z");
            rz = cache_get_field_content_int(0, "rz");
            veh(model) = cache_get_field_content_int(0, "model");
            veh(locked) = cache_get_field_content_int(0, "locked");
            veh(color1) = cache_get_field_content_int(0, "color1");
            veh(color2) = cache_get_field_content_int(0, "color2");
            veh(paintjob) = cache_get_field_content_int(0, "paintjob");
            gas = cache_get_field_content_int(0, "gas");
            veh(damage) = cache_get_field_content_float(0, "damage");
            
            //parking & towing;
            veh(parkx) = cache_get_field_content_float(0, "parkx");
            veh(parky) = cache_get_field_content_float(0, "parky");
            veh(parkz) = cache_get_field_content_float(0, "parkz");
            veh(parkrz) = cache_get_field_content_float(0, "parkrz");
            
            //upgrades
            veh(gps) = cache_get_field_content_int(0, "gps");
            veh(insurance) = cache_get_field_content_int( 0, "insurance");
            veh(alarm) = cache_get_field_content_int(0, "alarm");
            veh(sparekey) = cache_get_field_content_int(0, "sparekey");
            cache_get_field_content(0, "plate", veh(plate), sqlGameConnection, 9);
            
            
            dpanels = cache_get_field_content_int(0, "dpanels");
            ddoors = cache_get_field_content_int(0, "ddoors");
            dlights = cache_get_field_content_int(0, "dlights");
            dtires = cache_get_field_content_int(0, "dtires");
            
            veh(impounded) = cache_get_field_content_int(0, "impounded");

            //get vehicle component data
            new compstr[50];
            new components[14];
            for(new c; c < sizeof components; c++) {
                
                format( compstr, 50, "component%d", c );
                components[c] = cache_get_field_content_int(0, compstr);
            }

            if(x == 0.0) {
                x = veh(parkx);
                y = veh(parky);
                z = veh(parkz);
                rz = veh(parkrz);
            }

            #undef veh
            SendClientMessage(playerid, COLOR_VEHICLES, "Fetched vehicle. Spawning vehicle...");
            SetupPlayerVehicle(pid, slot, x, y, z, rz, dpanels, ddoors, dlights, dtires, gas, components);
            return 1;

        }
    }

    new query[100];
    mysql_format(sqlGameConnection, query, sizeof query, "SELECT * FROM playervehicles WHERE ID=%d", vsql);
    mysql_pquery_inline(sqlGameConnection, query, using inline SpawnPlayerVehicleEx, "i", playerid);
    return 1;
}


//PROTECTED (THIS MEANS: DO NOT TOUCH) --sets up the vehicle
static SetupPlayerVehicle(playerid, slot, Float:x, Float:y, Float:z, Float:rz, dpanels, ddoors, dlights, dtires, gas, components[]) {

    #define veh(%1) PlayerVehicles[playerid][slot][pv%1]
    veh(ID) = CreateVehicle(veh(model), x, y, z, rz, veh(color1), veh(color2), -1);
    SendClientMessage(playerid, COLOR_VEHICLES, "Spawned vehicle. Setting attributes...");

    SetVehicleVirtualWorld(veh(ID), veh(vw));
    SetVehicleNumberPlate(veh(ID), veh(plate));
    //SetVehicleLockState(veh(ID), veh(locked));
    
    if( IsABicycle( veh(ID) ) ) engineOn[veh(ID)] = 1; //set engine vars off
    SetVehicleEngineState(veh(ID), engineOn[veh(ID)]);
    playerDisabledEngine[veh(ID)] = 1;
    
    //set component info
    //get vehicle component data
    for( new c; c < 14; c++ ) {
        if( components[c] ) {
            AddVehicleComponent(veh(ID), components[c]);
        }
    }
    
    
    //totalled state
    if( veh(damage) < 300.0 ) {
        SetVehicleEngineState(veh(ID), 0);
        veh(damage) = 305.0;
    }
    
    //set the damage/gas
    UpdateVehicleDamageStatus(veh(ID), dpanels, ddoors, dlights, dtires);
    SetVehicleHealth(veh(ID), veh(damage));
    ChangeVehiclePaintjob(veh(ID), veh(paintjob));
    Gas[veh(ID)] = gas;

    SendClientMessage(playerid, COLOR_VEHICLES, "Your vehicle is ready to go! It's located where you last parked it.");
    #undef veh
}









//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
/*  DESTROY THE PLAYER VEHICLE
    Only save data which is being constantly updated by the server
    other data should be saved within commands etc.

    It's important we keep track of the reason the vehicle is being
    destroyed since if the player dropped connection we should
    respawn the vehicle back at its previous position
*/





public DestroyPlayerVehicle(playerid, slot, VD_REASON:reason) {
    new query[500];
    #define veh(%1) PlayerVehicles[playerid][slot][pv%1]
    new Float:x, Float:y, Float:z, Float:rz, vw;

    //get data
    new damage = GetVehicleHealth(veh(ID), veh(damage));
    new dpanels, ddoors, dlights, dtires, gas;
    GetVehicleDamageStatus(veh(ID), dpanels, ddoors, dlights, dtires);
	gas = Gas[veh(ID)];

    //did the player drop connection? if so, keep track of current vehicle pos
    if(reason == VD_REASON:VDPLAYER_ISSUE) {
        GetVehiclePos(veh(ID), x, y, z);
        GetVehicleZAngle(veh(ID), rz);
        vw = GetVehicleVirtualWorld( veh(ID) );


        mysql_format(sqlGameConnection, query, sizeof query,
"UPDATE playervehicles \
SET x = %f, y = %f, z = %f, rz = %f, vw = %d",
        PlayerInfo[playerid][pID], x, y, z, rz, vw
        );
    }

    //else just update normal shit
    else {
        mysql_format(sqlGameConnection, query, sizeof query,
"UPDATE playervehicles \
SET x = 0.0"
        
        );
    }
    mysql_format(sqlGameConnection, query, sizeof query, "%s\
    , damage = %d, dpanels = %d, ddoors = %d, dlights = %d, dtires = %d, gas = %d",
    damage, dpanels, ddoors, dlights, dtires, gas
    );

    //get vehicle component data
    new components[14];
	for( new c; c < sizeof components; c++ ) {
		components[c] = GetVehicleComponentInSlot(veh(ID), c);
        mysql_format(sqlGameConnection, query, sizeof query, "%s, component%d = %d", c, components[c]);
	}

    //finally, commit & destroy
    mysql_pquery(sqlGameConnection, query);
    DestroyVehicle(veh(ID));
    veh(SID) = 0;
    #undef veh
    return 1;
}


/*  TEST COMMANDS ONLY

*/
#if P_VEHICLES_TEST == 1
Hook:vehicles_OnPlayerSpawn(playerid) {
    SendClientMessage(playerid, COLOR_VEHICLES, "Good evening testers.");
    SendClientMessage(playerid, COLOR_VEHICLES, "Please test vehicles using commands: /cars, /c(reate)v(ehicle), /d(estroy)v(ehicle), /s(pawn)v(ehicle).");
    SendClientMessage(playerid, COLOR_VEHICLES, "It should be noted that these commands are going to be used for testing only.");
    SendClientMessage(playerid, COLOR_VEHICLES, "You should note, that this is only testing the backend, not the frontend (i.e user side).");
    return 1;
}
//Get player vehicles
CMD:cars(playerid, params[]) {

    inline OnGetPlayerVehicles(pid) {
        new rows = cache_num_rows();
        new string[144];
        if(rows) {
            for(new row = 0; row < rows; rows++) {


                format(string, sizeof(string), "%s : ID is %d", GetVehicleFriendlyNameFromModel(cache_get_field_content_int(0, "model")), cache_get_field_content_int(row, "ID"));
                SendClientMessage(pid, COLOR_VEHICLES, string);
            }
        }
        else {
            SendClientMessage(playerid, COLOR_VEHICLES, "You have no vehicles, /cv");
        }
    }

    new query[300];
    mysql_format(sqlGameConnection, query, sizeof query, "SELECT ID, model FROM playervehicles WHERE pID = %d", PlayerInfo[playerid][pID]);
    mysql_pquery_inline(sqlGameConnection, query, using inline OnGetPlayerVehicles, "i", playerid);
    return 1;
}
CMD:cv(playerid, params[]) {
    new model;
    if(sscanf(params, "i", model)) {
        SendClientMessage(playerid, COLOR_GREY, "You must enter a MODEL NUMBER of a vehicle.");
        return 1;
    }

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    SendClientMessage(playerid, COLOR_VEHICLES, "We're creating a vehicle at your position. . .");
    CreatePlayerVehicle(playerid, model, x, y, z, 0.0, -1, -1);
    return 1;
}

CMD:dv(playerid, params[]) {
    new slot;
    if(sscanf(params, "i", slot)) {
        SendClientMessage(playerid, COLOR_GREY, "You must enter a SLOT for which your vehicle is spawned in (try 0-2).");
        return 1;
    }

    DestroyPlayerVehicle(playerid, slot, VDPLAYER_DISCONNECT);
    return 1;
}

CMD:sv(playerid, params[]) {
    new id;
    if(sscanf(params, "i", id)) {
        SendClientMessage(playerid, COLOR_GREY, "You must enter a ID for which your vehicle specified in /cars.");
        return 1;
    }

    SpawnPlayerVehicle(playerid, id);
    return 1;
}


#endif

//EOF
#endif
