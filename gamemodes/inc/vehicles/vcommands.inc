static carstr[800] //for user output, static: only works in this file.

/*
	*****************		DIALOGS	  	***************
						DEFINITION & VARIABLES
		See hooks.inc for OnDialogResponse
	===================================================
*/	
		
//Using an enumerator in this case would be less efficient.
#define DIALOG_VEHICLES_UP		9000
#define DIALOG_VEHICLES_UP2		9001
#define DIALOG_CUSTOM_PLATE		9002
#define DIALOG_MYVEHICLES		9003

//the car being upgraded through /vupgrade
new vupgrading[MAX_PLAYERS] = -1

CMD:carhelp(playerid, params[])
{
	SendClientMessage(playerid, COLOR_YELLOW, "____________________________________________");
	SendClientMessage(playerid, COLOR_WHITE, "COMMANDS: /v, /breakin, /dealerships, /clearmods, /windows");
	SendClientMessage( playerid, COLOR_WHITE, "COMMANDS: /purchasecar, /spraycar, /lock, /engine, /trunk, /hood, /lights")
	return 1;
}

CMD:vehhelp( playerid, params[] ) {
	return cmd_carhelp( playerid, params )
}

/*
	********		Player Vehicle Commands		*********
	===================================================
*/

CMD:v( playerid, params[] ) { 
	new option[20], optional[80];
	if( sscanf( params, "s[20]z", option, optional ) ) {
		SendClientMessage( playerid, COLOR_GREY,		"{00BFFF}Usage:{FFFFFF} /v [option]" );
		SendClientMessage( playerid, COLOR_GREY,		"Available options:		/v cars, /v spawn, /v park, /v sell, /v dump" );
		SendClientMessage( playerid, COLOR_GREY, 		"Available options:		/v upgrade, /v track, /v givekeys")
		return 1;
	}

	//Commands that do not require the player to have a spawned car
	if( strcmp( option, "cars", true ) == 0 ) { return cmdveh_cars( playerid ); }
	if( strcmp( option, "spawn", true ) == 0 ) { return cmdveh_spawn( playerid, optional ); }
	
	
	//Commands that do require a spawned car
	new slot = playerSpawnedVehicle[playerid]
	if( slot < 0 ) return SendClientMessage( playerid, COLOR_GREY, "You do not have any cars spawned. Use '/v spawn' to spawn one." )
	new model[80];
	format( model, 80, GetVehicleFriendlyNameFromModel( PlayerVehicles[playerid][slot][pvmodel] ) );
	
	if( strcmp( option, "track", true ) == 0 ) { return cmdveh_track( playerid, slot, model ); }
	if( strcmp( option, "givekeys", true ) == 0 ) { return cmdveh_givekeys( playerid, optional, slot, model );}
	if( strcmp( option, "park", true ) == 0 ) { return cmdveh_park( playerid, slot, model ); }
	if( strcmp( option, "upgrade", true ) == 0 ) { return cmdveh_upgrade( playerid, slot, model ); }
	if( strcmp( option, "sell", true ) == 0 ) { return cmdveh_sell( playerid, optional, slot, model ); }
	if( strcmp( option, "dump", true ) == 0 ) { return cmdveh_dump( playerid, optional, slot, model ); }
	
	return 1;
}

cmdveh_dump( playerid, params[], slot, model[] ) {
	
	#define veh(%0) PlayerVehicles[playerid][slot][pv%0]
	
	new cartext[128];
	if( GetPlayerVehicleID( playerid ) != veh(ID) ) {
		format( cartext, sizeof( cartext ), "You must be inside your %s in order to dump it at the dump.", model );
		return SendClientMessage( playerid, COLOR_GREY, cartext );
	}
	
	if( !IsPlayerInRangeOfPoint( playerid, 5.0, 134.0741,-256.5139,1.3052 ) ) {
		SetPlayerCheckpointEx( playerid, 134.0522,-256.3802,1.3052, 5.0 )
		CP[playerid] = CHECKPOINT_CAR_DUMP
		return SendClientMessage( playerid, COLOR_GREY, "You must be at the dump. A red marker has been marked on your map showing you the way." )
	}
	
	new moneyback = 0
	
	if( veh(gps) ) moneyback += 300000
	if( veh(insurance) ) moneyback += 200000
	if( veh(alarm) ) moneyback += 100000
	if( veh(sparekey) ) moneyback += 200000
	
	new car_str[128]
	if( sscanf( params, "s[128]", car_str ) ) {	
		format( car_str, 128, "You will get a total of $%d for dumping your %s. Are you sure you want to do this?", moneyback, model )
		SendClientMessage( playerid, COLOR_WHITE, car_str )
		SendClientMessage( playerid, COLOR_GREY, "/v dump [confirm], to proceed." )
		return 1
	}
	
	destroyPlayerVehicle( playerid, slot )
	PlayerInfo[playerid][pCarID][slot] = 0
	veh(pID) = -1
	veh(model) = 0
	veh(impounded) = 0
	veh(gps) = 0
	veh(insurance) = 0
	veh(sparekey) = 0
	veh(alarm) = 0
	veh(Gun1) = 0
	veh(Gun2) = 0
	veh(Pot) = 0
	veh(Crack) = 0
	veh(Armor) = 0
	veh(damage) = 1000.00
	veh(playerID) = INVALID_PLAYER_ID
	mysql_format( sqlGameConnection, car_str, 128, "DELETE FROM playervehicles WHERE ID = %d;", veh(SQLID) )
	mysql_pquery( sqlGameConnection, car_str )
	
	format( car_str, 128, "You have dumped for %s for $%d.", model, moneyback )
	SendClientMessage( playerid, COLOR_GREY, car_str )
	GiveMoney( playerid, moneyback )
	SQLSave( playerid )
	
	#undef veh
	return 1;
}

new playeridcarOffered[MAX_PLAYERS] = {-1, ...}
new carOfferedSlot[MAX_PLAYERS] = {-1, ...}
new carOfferedPrice[MAX_PLAYERS] = {-1, ...}

cmdveh_sell( playerid, params[], slot, model[] ) { 
	new player, price, cartext[128];
	if( sscanf( params, "ui", player, price ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /v sell [playerid/playerName] [price]" )
	}
	
	if( price < 0 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You must enter a positive number for the price." )
	}
	
	#define veh(%1) PlayerVehicles[playerid][slot][pv%1]
	
	new Float:vpos[3]
	GetVehiclePos( veh(ID), vpos[0], vpos[1], vpos[2] )
	if( !IsPlayerInRangeOfPoint( playerid, 8.0, vpos[0], vpos[1], vpos[2] ) ) {
		format( cartext, sizeof( cartext ), "You must be near your %s in order to sell it.", model );
		return SendClientMessage( playerid, COLOR_GREY, cartext );
	}

	if( !IsPlayerConnected( player ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "ERROR: That player is not connected." )
	}
	
	new Float:ppos[3]
	GetPlayerPos( player, ppos[0], ppos[1], ppos[2] )
	if( IsPlayerInRangeOfPoint( playerid, 3.0, ppos[0], ppos[1], ppos[2] ) ) {
		format( carstr, sizeof( carstr ), "%s has offered you their %s for $%d, type /purchasecar to accept the vehicle.", PlayerICName( playerid ), model, price )
		SendClientMessage( player, COLOR_LIGHTBLUE, carstr )
		
		format( carstr, sizeof( carstr ), "You have offered %s your %s for $%d, please wait for the player to accept it.", PlayerICName( player ), model, price )
		SendClientMessage( playerid, COLOR_LIGHTBLUE, carstr )
		
		playeridcarOffered[player] = playerid
		carOfferedSlot[player] = slot
		carOfferedPrice[player] = price
		return 1
	
	}
	#undef veh
	SendClientMessage( playerid, COLOR_GREY, "You must be near this player in order to sell them this vehicle." )
	return 1;
}
CMD:purchasecar( playerid, params[] ) {
	if( playeridcarOffered[playerid] > -1 ) {
		new player = playeridcarOffered[playerid]
		new slot = carOfferedSlot[playerid]
		new price = carOfferedPrice[playerid]
		
		if( PlayerInfo[playerid][pCash] < carOfferedPrice[playerid] ) {
			return SendClientMessage( playerid, COLOR_GREY, "You cannot afford this." )
		}
		
		new freeslot = -1
		for( new i; i < PlayerInfo[playerid][pMaxCarSlots]; i++ ) {
			if( PlayerInfo[playerid][pCarID][i] < 1 ) {
				freeslot = i
			}
		}
		if( freeslot == -1 ) {
			return SendClientMessage( playerid, COLOR_GREY, "You cannot own anymore vehicles." )
		}
		
		playeridcarOffered[playerid] = -1
		carOfferedPrice[playerid] = -1
		carOfferedSlot[playerid] = -1
		PlayerInfo[player][pCarID][slot] = 0
		
		GiveMoney( player, price )
		GiveMoney( playerid, -price )
		
		PlayerInfo[player][pCarID][slot] = 0
		playerSpawnedVehicle[player] = -1
		
		#define veh(%0) PlayerVehicles[playerid][freeslot][pv%0]
		#define oldveh(%0) PlayerVehicles[player][slot][pv%0]
		
		
		for( new c; c < 14; c++ ) {
			veh(components)[c] = GetVehicleComponentInSlot( oldveh(ID), c );
		}
		GetVehicleHealth( oldveh(ID), veh(damage) )
		GetVehiclePos( oldveh(ID), veh(x), veh(y), veh(z) )
		GetVehicleZAngle( oldveh(ID), veh(rz) )
		veh(vw) = GetVehicleVirtualWorld( oldveh(ID) )
		
		//get trunk data to save
		if( veh(spawned) ) {
			veh(Gun1) = TrunkInfo[oldveh(ID)][cGun1]
			veh(Gun2) = TrunkInfo[oldveh(ID)][cGun2]
			veh(Pot) = TrunkInfo[oldveh(ID)][cPot]
			veh(Crack) = TrunkInfo[oldveh(ID)][cCrack]
			veh(Armor) = TrunkInfo[oldveh(ID)][cArmor]
		}
		
		GetVehicleDamageStatus( oldveh(ID), veh(dpanels), veh(ddoors), veh(dlights), veh(dtires) )
		destroyPlayerVehicle( player, slot );
		
		veh(pID) = PlayerInfo[playerid][pID]
		
		PlayerInfo[playerid][pCarID][freeslot] = PlayerVehicles[player][slot][pvSQLID]
		
		veh(model) = oldveh(model);
		veh(color1) = oldveh(color1);
		veh(color2) = oldveh(color2);
		veh(paintjob) = oldveh(paintjob);
		veh(locked) = oldveh(locked);
		veh(gps) = oldveh(gps);
		veh(alarm) = oldveh(alarm);
		veh(insurance) = oldveh(insurance);
		veh(sparekey) = oldveh(sparekey);
		veh(gas) = oldveh(gas)
		
		format( veh(plate), 9, "%s", oldveh(plate) );
		veh(SQLID) = oldveh(SQLID)
		if( playerSpawnedVehicle[playerid] > -1 ) {
			destroyPlayerVehicle( playerid, playerSpawnedVehicle[playerid] );
		}
		spawnPlayerVehicle( playerid, freeslot, 1 );
		savePlayerVehicle( playerid, freeslot ); //same veh sqlid
		
		oldveh(pID) = -1;
		oldveh(model) = 0;
		oldveh(gps) = 0;
		oldveh(insurance) = 0;
		oldveh(alarm) = 0;
		oldveh(sparekey) = 0;
		#undef oldveh
		
		format( carstr, sizeof( carstr ), "You have bought the %s from %s for $%d.", GetVehicleFriendlyNameFromModel( veh(model) ), PlayerICName( player ), price )
		SendClientMessage( playerid, COLOR_LIGHTBLUE, carstr )
		
		format( carstr, sizeof( carstr ), "%s has bought the %s from you for $%d.", PlayerICName( playerid ), GetVehicleFriendlyNameFromModel( veh(model) ), price )
		SendClientMessage( player, COLOR_LIGHTBLUE, carstr )
		#undef veh
	
	}
	return 1
}
cmdveh_upgrade( playerid, slot, model[] ) {
	if( IsPlayerInRangeOfPoint( playerid, 5.0, 2324.2505,-2012.4784,13.6222 ) ) { //The new garage map
		
		new menuitems[150]
		#define veh(%1) PlayerVehicles[playerid][slot][pv%1]
		
		format( menuitems, 150, "GPS\t$350,000\nAlarm\t$200,000\nInsurance\t$250,000\nSpare keys\t$250000\nCustom license plate\t$100000\nAdd another vehicle slot\t$15,000,000" )
			
		vupgrading[playerid] = slot
		new titlestr[100]
		format( titlestr, 100, "{FFB76F}Vehicle Upgrades for %s{FFFFFF}", model )
		ShowPlayerDialog( playerid, DIALOG_VEHICLES_UP2, DIALOG_STYLE_TABLIST, titlestr, menuitems, "Purchase", "Cancel" )
		#undef veh
		return 1
	}
	SendClientMessage( playerid, COLOR_GREY, "You are not inside the Garage in Willowfield, a marker leading to it will be set for you." )
	if(CP[playerid] != 0) return SendClientMessage(playerid, COLOR_GREY, "You already have an active checkpoint, /killcp.");
	DisablePlayerCheckpointEx(playerid);
	SetPlayerCheckpointEx(playerid, 2324.2505, -2012.4784, 13.6222, 3.0);
	CP[playerid] = 1;
	return 1;
}
cmdveh_spawn( playerid, params[] ) {
	new cartext[128];
	if( playerSpawnedVehicle[playerid] > -1 ) {
		format( cartext, sizeof( cartext ), "You can only have one car spawned at a time, /v park your %s first.", GetVehicleFriendlyNameFromModel( PlayerVehicles[playerid][playerSpawnedVehicle[playerid]][pvmodel] ) );
		return SendClientMessage( playerid, COLOR_GREY, cartext );
	}
	
	new model[128], carslot;
	if( sscanf( params, "d", carslot ) ) {
		if( !sscanf( params, "s[128]", model ) ) {
			//incase they enter the model name instead
			for( new s; s < PlayerInfo[playerid][pMaxCarSlots]; s++ ) {
				if( strcmp( model, GetVehicleFriendlyNameFromModel( PlayerVehicles[playerid][s][pvmodel] ), true ) == 0) {
					spawnPlayerVehicle( playerid, s );
					format( cartext, sizeof( cartext ), "You have spawned your %s in its last parked position.", GetVehicleFriendlyNameFromModel( PlayerVehicles[playerid][s][pvmodel] ) );
					SendClientMessage( playerid, COLOR_GREY, cartext );
					return 1;
				}
			}
		}
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /v spawn [car slot/car name]" );
	}
	
	if( carslot > PlayerInfo[playerid][pMaxCarSlots] || carslot < 1 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You didn't enter a correct car slot, check /v cars for a correct slot." );
	}
	
	if( PlayerInfo[playerid][pID] == PlayerVehicles[playerid][carslot][pvpID] ) {
		carslot--;
		spawnPlayerVehicle( playerid, carslot );
		format( cartext, sizeof( cartext ), "You have spawned your %s in its last parked position.", GetVehicleFriendlyNameFromModel( PlayerVehicles[playerid][carslot][pvmodel] ) );
		SendClientMessage( playerid, COLOR_GREY, cartext );
		return 1;
	
	}
	
	format( cartext, sizeof( cartext ), "You do not own this vehicle, consider looking at /v cars." );
	SendClientMessage( playerid, COLOR_GREY, cartext );
	return 1;
}
cmdveh_park( playerid, slot, model[] ) {
	#define veh(%0) PlayerVehicles[playerid][slot][pv%0]

	new cartext[128];
	if( GetPlayerVehicleID( playerid ) != veh(ID) ) {
		format( cartext, sizeof( cartext ), "You must be in your %s in order to park it.", model );
		SendClientMessage( playerid, COLOR_GREY, cartext );
		return 1;
	}
	
	GetVehiclePos( veh(ID), veh(parkx), veh(parky), veh(parkz) );
	GetVehicleZAngle( veh(ID), veh(parkrz) );
	destroyPlayerVehicle( playerid, slot );
	
	format( cartext, sizeof( cartext ), "You have parked your %s. It will spawn here next time you use /v spawn.", model );
	SendClientMessage( playerid, COLOR_LIGHTBLUE, cartext );
	
	#undef veh
	return 1;
}
cmdveh_givekeys( playerid, params[], slot, model[] ) {

	new player, cartext[128];
	if( sscanf( params, "u", player ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /givekeys [playerid/playerName]" )
	}
	
	#define veh(%0) PlayerVehicles[playerid][slot][pv%0]
	if( !PlayerVehicles[playerid][slot][pvsparekey] ) {
		format( cartext, sizeof( cartext ), "You do not have any spare keys for your %s.", model );
		return SendClientMessage( playerid, COLOR_GREY, cartext );
	}
	
	if( !IsPlayerConnected( playerid ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "ERROR: The player you provided is not connected." )
	}
	
	new Float:ppos[3]
	GetPlayerPos( player, ppos[0], ppos[1], ppos[2] )
	if( !IsPlayerInRangeOfPoint( playerid, 3.0, ppos[0], ppos[1], ppos[2] ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "ERROR: You are not near the player." );
	}
	
	if( spareKeys[player] == playerid && spareKeysSlot[player] == slot ) {
		format( cartext, sizeof( cartext ), "You have already given this player your spare key to your %s.", model );
		return SendClientMessage( playerid, COLOR_GREY, cartext )
	}
	
	spareKeys[player] = playerid;
	spareKeysSlot[player] = slot;
	hasgivenKey[playerid] = 1;
	format( carstr, sizeof( carstr ), "%s has given you their spare key to their %s.", PlayerICName( playerid ), model )
	SendClientMessage( player, COLOR_LIGHTBLUE, carstr )
	
	format( carstr, sizeof( carstr ), "You have given %s your spare key to your %s.", PlayerICName( player ), model )
	SendClientMessage( playerid, COLOR_LIGHTBLUE, carstr )
	#undef veh
	return 1;
}
cmdveh_track( playerid, slot, model[] ) {
	new cartext[128];
	#define veh(%1) PlayerVehicles[playerid][slot][pv%1]

	if( veh(gps) ) {
		GetVehiclePos( veh(ID), veh(x), veh(y), veh(z) )
		SetPlayerCheckpointEx( playerid, veh(x), veh(y), veh(z), 5.0 )
		CP[playerid] = CHECKPOINT_CAR_TRACK
		
		format( cartext, sizeof( cartext ), "A marker leading to your %s has been placed on the minimap.", model );
		SendClientMessage( playerid, COLOR_LIGHTBLUE, cartext );
	}
	else {
		format( cartext, sizeof( cartext ), "Your %s does not have a GPS, consider buying one at the Garage in Willowfield.", model );
		SendClientMessage( playerid, COLOR_GREY, cartext )
	}
	#undef veh
	return 1;
}
cmdveh_cars( playerid ) {

	SendClientMessage( playerid, COLOR_LIGHTBLUE, "______________[ Your vehicles ]______________")

	new count;
	for( new i; i < PlayerInfo[playerid][pMaxCarSlots]; i++ ) {
		
		#define veh(%1) PlayerVehicles[playerid][i][pv%1]
		
		if( PlayerInfo[playerid][pCarID][i] < 1 ) continue;
		
		count++;
		new outputcars[128], outputupgrades[128];
		format( outputcars, sizeof( outputcars ), "Slot %d: %s", i+1, GetVehicleFriendlyNameFromModel( veh(model) ) );
		
		if( veh(alarm) || veh(insurance) || veh(gps) || veh(sparekey) ) {
			format( outputupgrades, sizeof( outputupgrades ), "( %s%s%s%s)", 
					( veh(gps) == 1 ) ? ( ( "GPS " ) ) : ( "" ), ( veh(alarm) == 1 ) ? ( ( "Alarm " ) ) : ( "" ), ( veh(insurance) == 1 ) ? ( ( "Insurance " ) ) : ( "" ),
					( veh(sparekey) == 1 ) ? ( ( "Spare key " ) ) : ( "" ) );
			
			format( outputcars, sizeof( outputcars ), "%s %s", outputcars, outputupgrades );
			SendClientMessage( playerid, COLOR_GREY, outputcars );
			continue;
		}
		else {
			format( outputcars, sizeof( outputcars ), "%s (No upgrades)", outputcars );
			SendClientMessage( playerid, COLOR_GREY, outputcars );
			continue;
		}
		#undef veh
	}
	if( count == 0 ) {
		SendClientMessage( playerid, COLOR_GREY, "You do not have any owned vehicles." );
	}

	return 1;
}

CMD:lock( playerid, params[] ) {
	if( playerSpawnedVehicle[playerid] > -1 ) {
		
		new slot = playerSpawnedVehicle[playerid]
		
		#define veh(%1) PlayerVehicles[playerid][slot][pv%1] //This is just a marco to reduce repetition!
	
		GetVehiclePos( veh(ID), veh(x), veh(y), veh(z) ) //update vehicle coords
		if( !IsPlayerInRangeOfPoint( playerid, 5.0, veh(x), veh(y), veh(z) ) ) {
			
			SendClientMessage( playerid, COLOR_GREY, "You are not by any vehicles that you have the keys for." )
			return 1
		}
			
		if( veh(locked) ) {
			format( carstr, sizeof( carstr ), "* %s has unlocked their vehicle.", PlayerICName( playerid ) )
		}
		else {
			format( carstr, sizeof( carstr ), "* %s has locked their vehicle.", PlayerICName( playerid ) )
		}
		ProxDetector( 30.0, playerid, carstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE )
		veh(locked) = !veh(locked)
		
		SetVehicleLockState( veh(ID), veh(locked) )
		SetVehicleAlarmState( veh(ID), 0 )
		#undef veh
		return 1
	}
	if( spareKeys[playerid] > -1 && spareKeysSlot[playerid] > -1 ) {
		new Float: vpos[3];
		#define veh(%0) PlayerVehicles[spareKeys[playerid]][spareKeysSlot[playerid]][pv%0]
		if( veh(spawned) && veh(ID) != INVALID_VEHICLE_ID ) {
			GetVehiclePos( veh(ID), vpos[0], vpos[1], vpos[2] )
			if( IsPlayerInRangeOfPoint( playerid, 5.0, vpos[0], vpos[1], vpos[2] ) ) {
				if( veh(locked) ) {
					format( carstr, sizeof( carstr ), "* %s has unlocked their vehicle.", PlayerICName( playerid ) )
				}
				else {
					format( carstr, sizeof( carstr ), "* %s has locked their vehicle.", PlayerICName( playerid ) )
				}
				ProxDetector( 30.0, playerid, carstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE )
				veh(locked) = !veh(locked)
				
				SetVehicleLockState( veh(ID), veh(locked) )
				SetVehicleAlarmState( veh(ID), 0 )
				return 1
			}
		}
		#undef veh
	}
	SendClientMessage( playerid, COLOR_GREY, "You do not have any vehicle spawned that you have keys for." )
	return 1
}


//===================[ Player unowned vehicle commands ]=====================
CMD:paintcar( playerid, params[] ) {
	SendClientMessage( playerid, COLOR_GREY, "Visit a vehicle modification shop to style the paintjob of a vehicle." )
	return 1
}

//This was just copied and pasted from GM.
CMD:spraycar(playerid, params[])
{
	if(PlayerInfo[playerid][pSpraycan] == 0) return SendClientMessage(playerid, COLOR_GREY, "Your spraycan is empty.");
	if(IsPlayerInAnyVehicle(playerid))
	{
	    new c1, c2;
		if(sscanf(params, "iI(-1)", c1, c2)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /colorcar [0-255] [0-255(optional)]");
		{
		    new veh = GetPlayerVehicleID(playerid);
		    if(c2 == -1) c2 = c1;
			if(c1 < 0 || c1 > 255 || c2 < 0 || c2 > 255) return SendClientMessage(playerid, COLOR_GREY, "The colors are between 0 and 255.");

			ChangeVehiclePaintjob(veh, 3);
			ChangeVehicleColor(veh, c1, c2);
			if(c2 == c1) format(str, sizeof(str), "You have sprayed your vehicle to be %s (%d).", GetVehicleColorName(c1), c1);
	 		else format(str, sizeof(str), "You have sprayed the vehicle to be %s (%d) and %s (%d).", GetVehicleColorName(c1), c1, GetVehicleColorName(c2), c2);
			SendClientMessage(playerid, COLOR_GREY, str);

			if(c2 == c1)format(str, sizeof(str), "* %s uses a spraycan to spray the %s to be %s. *", PlayerICName(playerid), GetVehicleFriendlyName(veh), GetVehicleColorName(c1));
			else format(str, sizeof(str), "* %s uses a spraycan to spray the %s to be %s and %s. *", PlayerICName(playerid), GetVehicleFriendlyName(veh), GetVehicleColorName(c1), GetVehicleColorName(c2));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

			PlayerInfo[playerid][pSpraycan]--;
			PlayerPlaySound(playerid, 1134, 0.0, 0.0, 0.0);

			for( new i; i <= GetPlayerPoolSize(); i++ ) {
				for( new slot; slot < MAX_VEHICLE_SLOTS; slot++ ) {
					if( PlayerVehicles[i][slot][pvID] == veh ) {
						PlayerVehicles[i][slot][pvcolor1] = c1
						PlayerVehicles[i][slot][pvcolor2] = c2
						PlayerVehicles[i][slot][pvpaintjob] = 3
						break
					}
				}
			}
		}
	} else return SendClientMessage(playerid, COLOR_GREY, "You must be in a vehicle.");
	return 1;
}

CMD:colorcar( playerid, params[] ) {
	return cmd_spraycar( playerid, params ); //make players happy...
}


//CMD:breakin
new vehiclePickCount[MAX_PLAYERS]
new lastbrokenin[MAX_PLAYERS]
breakinPlayerVehicle( playerid ) {

	//restrict usage to 1/per30 seconds
	if( gettime() < lastbrokenin[playerid] + 30 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You must wait 30 seconds before breaking into another vehicle." )
	}
	new Float:vpos[3]
	for( new i; i < GetPlayerPoolSize()+1; i++ ) {
		new slot = playerSpawnedVehicle[i]
		if( slot < 0 ) continue
		
		#define veh(%1) PlayerVehicles[i][slot][pv%1]
		
		GetVehiclePos( veh(ID), vpos[0], vpos[1], vpos[2] )
		if( IsPlayerFacingPoint( playerid, 5.0, vpos[0], vpos[1], vpos[2], 50.0 ) ) {
			if( !veh(locked) ) {
				return SendClientMessage( playerid, COLOR_GREY, "This vehicle is already unlocked." )
			}
			//If vehicle has alarm notify owner, set the alarm to go off
			if( veh(alarm) ) {
				if( playerSpawnedVehicle[i] > -1 ) {
					format( carstr, sizeof( carstr ), "SMS: This is an automated message informing you that your %s alarm has been activated.", GetVehicleFriendlyName( veh(ID) ) )
					SendClientMessage( i, COLOR_YELLOW, carstr )
					
					SetVehicleAlarmState( veh(ID) ) //sound the alarm for 35secs
				}
			}
			
			LockPickingCar[playerid] = i
			vehiclePickCount[playerid] = 0
			
			//command usage restriction
			lastbrokenin[playerid] = gettime()
			
			SetTimerEx( "OnBreakinExpire", 18*1000, false, "ii", playerid, veh(ID) )
			SetPlayerProgressBarValue( playerid, workingProgress[playerid], 1.0 )
			ShowPlayerProgressBar( playerid, workingProgress[playerid] )
			
			format( carstr, sizeof( carstr ), "* %s fiddles with the vehicle's lock using their screwdriver. *", PlayerICName( playerid ) )
			ProxDetector( 30.0, playerid, carstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE )
			//LoopingAnim( playerid, "POOL", "POOL_ChalkCue", 4.1, 0, 1, 1, 1, 1 )
			ApplyAnimation( playerid, "POOL", "POOL_ChalkCue", 4.1, 0, 1, 1, 1, 1, 1 )
			
			displayCenterHUDInfo( playerid,  "TIP: Rapidly tap ~r~Y~w~ to break in", 4);
			//SendClientMessage( playerid, COLOR_GREY, "TIP: tap Y rapidly to unlock the vehicle." )
			return 1
		}
		#undef veh
	}
	SendClientMessage( playerid, COLOR_GREY, "You are not by any vehicle." )
	return 1
}

//player taking too long to breakin
forward OnBreakinExpire( playerid, vehid );
public OnBreakinExpire( playerid, vehid ) {

	if( LockPickingCar[playerid] == -1 )  {
		return 1
	}
	
	vehiclePickCount[playerid] = 0
	LockPickingCar[playerid] = -1
	HidePlayerProgressBar( playerid, workingProgress[playerid] )
	
	SendClientMessage( playerid, COLOR_GREY, "You have failed to break into the vehicle." )
	ClearAnimations( playerid )
	ApplyAnimationEx( playerid, "CARRY", "crry_prtial", 4.0, 0, 0, 0, 0, 0 )
	return 1
}

new lastcheckplate[MAX_PLAYERS]

CMD:checkplate( playerid, params[] ) {
	if( gettime() < lastcheckplate[playerid] + 10 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You must wait 10 seconds to allow the system to cool down." )
	}
	if( PlayerInfo[playerid][pMember] != 1 && PlayerInfo[playerid][pMember] != 2 && PlayerInfo[playerid][pMember] != 5 && PlayerInfo[playerid][pMember] != 11) {
		return SendClientMessage( playerid, COLOR_GREY, "You do not have access to the correct systems to use this command." )
	}
	
	if( IsPlayerInAnyVehicle( playerid ) ) {
		if( sVehicleInfo[GetDynamicVehicleID( GetPlayerVehicleID( playerid ) )][v_faction] > 0 ) {
			new model[128], plate[128]
			if( sscanf( params, "s[128]s[128]", model, plate ) ) {
				return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /checkplates [vehicleName] [plate]" )
			}
			
			new modelid = ReturnVehicleModelID( model )
			
			lastcheckplate[playerid] = gettime()
			SendClientMessage( playerid, COLOR_LIGHTBLUE, "The following vehicles have matching vehicle plates,")
			
			new count
			for( new i; i <= GetPlayerPoolSize(); i++ ) {
				for( new slot; slot < PlayerInfo[i][pMaxCarSlots]; slot++ ) {
					if( PlayerVehicles[i][slot][pvmodel] == modelid ) {
						if( strcmp( PlayerVehicles[i][slot][pvplate], plate, true ) == 0 ) {
							if( strlen( PlayerVehicles[i][slot][pvplate] ) > 1 ) {
								format( carstr, sizeof( carstr ), "Owner: %s | Vehicle model: %s | Plate: %s | Vehicle Plate ID: %d", PlayerICName( PlayerVehicles[i][slot][pvplayerID] ), GetVehicleFriendlyNameFromModel( modelid ), PlayerVehicles[i][slot][pvplate],
								PlayerVehicles[i][slot][pvplayerID]+78371 );
								SendClientMessage( playerid, COLOR_GREY, carstr );
								count++
							}
						}
					}
				}
			}
			if( count == 0 ) {
				SendClientMessage( playerid, COLOR_GREY, "There were no results." )
			}
			return 1
		}
	}
	SendClientMessage( playerid, COLOR_GREY, "You are not in a faction vehicle." )
	return 1
}

/*
	******	Administrator Vehicle Commands		*******
	===================================================
*/

CMD:checkcars( playerid, params[] ) { 
	if( PlayerInfo[playerid][pAdmin] < 1 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You are not authorized to use this command." )
	}

	new player
	if( sscanf( params, "u", player ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /checkvehicles [playerid/playerName]" )
	}
	if( !IsPlayerConnected(player) ) {
		return SendClientMessage( playerid, COLOR_GREY, "That player isn't connected." )
	}
	
	format( carstr, sizeof( carstr ), "_______[ %s's vehicles ]_______", PlayerICName( player ) )
	SendClientMessage( playerid, COLOR_LIGHTBLUE, carstr )
	
	new count
	for( new i; i < PlayerInfo[player][pMaxCarSlots]; i++ ) {
		if( PlayerInfo[player][pCarID][i] < 1 ) continue
		
		#define veh(%1) PlayerVehicles[player][i][pv%1]
		
		//veh name
		format( carstr, sizeof( carstr ), "Car slot %d: %s (%s) [vehicleid: %d] %s", i+1, GetVehicleFriendlyNameFromModel( veh(model) ), veh(plate), veh(ID), ( veh(impounded) == 1 ) ? ( ( "- IMPOUNDED" ) ) : ( "" ) )
		SendClientMessage( playerid, COLOR_WHITE, carstr )
		//veh info
		format( carstr, sizeof( carstr ), "	Upgrades: %s %s %s %s", 
			( veh(gps) == 1 ) ? ( ( "GPS" ) ) : ( "" ), ( veh(alarm) == 1 ) ? ( ( "Alarm" ) ) : ( "" ), ( veh(insurance) == 1 ) ? ( ( "Insurance" ) ) : ( "" ),
			( veh(sparekey) == 1 ) ? ( ( "Spare key" ) ) : ( "" ) )
		SendClientMessage( playerid, COLOR_GREY, carstr )
		count++
		#undef veh
	}
	if( count < 1 ) {
		SendClientMessage( playerid, COLOR_GREY, "This player doesn't own any vehicles." )
	}
	SendClientMessage( playerid, COLOR_GREY, "________________________________" )
	return 1
}
CMD:givecar( playerid, params[] ) {
	if( PlayerInfo[playerid][pAdmin] < 4 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You are not authorized to use this." )
	}
	

	if( pVehiclePool == MAX_OWNED_VEHICLES-1 ) {
		return SendClientMessage( playerid, COLOR_GREY, "Maximum vehicle treshold has been reached. Contact a developer." )
	}
	
	if( pVehiclePool + 199 > MAX_OWNED_VEHICLES ) {
		SendClientMessage( playerid, COLOR_GREY, "Maximum vehicle treshold has less than 200 slots free. Contact a developer immediately." )
	}
	
	new player, model[128]
	if( sscanf( params, "us[128]", player, model ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /givecar [playerid/name] [vehicleName/ID]" )
	}
	
	if( !IsPlayerConnected( player ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "That player isn't connected." );
	}
	
	new modelid = ReturnVehicleModelID( model );
	if(IsFactionVehicle(modelid)) return SendClientMessage(playerid, COLOR_GREY, "Players cannot own faction vehicles.");
	
	new slot = GetPlayerFreeCarslot( player )
	if( slot == -1 ) {
		return SendClientMessage( playerid, COLOR_GREY, "That player has no more room for more cars." )
	}
	new Float:pos[4]
	GetPlayerPos( playerid, pos[0], pos[1], pos[2] )
	GetPlayerFacingAngle( playerid, pos[3] )
	createPlayerVehicle( player, slot, modelid, pos[0] + 5, pos[1], pos[2], pos[3] )
	
	new vehstr[128]
	format( vehstr, sizeof( vehstr ), "You have given %s a %s.", PlayerICName( player ), GetVehicleFriendlyNameFromModel( modelid ) )
	SendClientMessage( playerid, COLOR_GREY, vehstr )
	SendClientMessage( playerid, COLOR_GREY, "Notify the player of the location of their parked car (here). It will spawn when they tow it." )
	format( vehstr, sizeof( vehstr ), "%s has given you a %s. It's now accessable in /v, it will spawn at the admin's position." , PlayerICName( playerid ), GetVehicleFriendlyNameFromModel( modelid ) )
	SendClientMessage( player, COLOR_LIGHTBLUE, vehstr )
	return 1
}
CMD:removecar( playerid, params[] ) {
	if( PlayerInfo[playerid][pAdmin] < 4 ) {
		return 1
	}
	
	new player, slot
	if( sscanf( params, "ui", player, slot ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /removecar [playerID/playerName] [slot]" )
	}
	
	if( slot > PlayerInfo[player][pMaxCarSlots] || slot < 1 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You must enter a valid car slot (between 1 and 3-4). HINT: see /checkcars." )
	}
	
	if( !IsPlayerConnected( player ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "That player is not connected." )
	}
	
	new car_str[128]
	slot--
	
	#define veh(%0) PlayerVehicles[player][slot][pv%0]
	
	
	if( PlayerInfo[player][pCarID][slot] < 1 ) {
		return SendClientMessage( playerid, COLOR_GREY, "This player doesn't have a vehicle in this slot." )
	}
	
	format( car_str, 128, "You have destroyed %s's %s.", PlayerICName( player ), GetVehicleFriendlyNameFromModel( veh(model) ) )
	SendClientMessage( playerid, COLOR_GREY, car_str )
	
	
	
	if( veh(pID) != PlayerInfo[player][pID] ) {
		PlayerInfo[player][pCarID][slot] = 0
		return 1;
	}
	PlayerInfo[player][pCarID][slot] = 0
	
	destroyPlayerVehicle( player, slot )
	veh(pID) = -1
	veh(model) = 0
	veh(impounded) = 0
	veh(gps) = 0
	veh(insurance) = 0
	veh(sparekey) = 0
	veh(alarm) = 0
	veh(Gun1) = 0
	veh(Gun2) = 0
	veh(Pot) = 0
	veh(Crack) = 0
	veh(Armor) = 0
	veh(damage) = 1000.00
	veh(playerID) = INVALID_PLAYER_ID
	mysql_format( sqlGameConnection, car_str, 128, "DELETE FROM playervehicles WHERE ID = %d;", veh(SQLID) )
	mysql_pquery( sqlGameConnection, car_str )
	SQLSave( playerid )
	
	#undef veh
	return 1
}

CMD:oldvehicle( playerid, params[] ) {
	if( PlayerInfo[playerid][pAdmin] < 1 && PlayerInfo[playerid][pDev] < 1 ) {
		return 1;
	}
	
	new id;
	if( sscanf( params, "i", id ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "USAGE:/oldvehicle [car key(ID)]" );
	}
	
	if( !IsPlayerConnected( id ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "Invalid player ID." );
	}
	
	new query[300];
	mysql_format( sqlGameConnection, query, sizeof( query ), "SELECT CKey1, CKey2 FROM players2 WHERE ID = %d", PlayerInfo[id][pID] );
	mysql_pquery( sqlGameConnection, query, "VehicleRefund", "ii", playerid, id );
	return 1;
}
forward VehicleRefund( playerid, id );
public VehicleRefund( playerid, id ) {
	new rows, fields, CKey[2];
	cache_get_data( rows, fields );
	if( !rows ) { 
	
		return SendClientMessage( playerid, COLOR_GREY, "Provided playerid not found." );
	}
	
	CKey[0] = cache_get_field_content_int( 0, "CKey1" );
	CKey[1] = cache_get_field_content_int( 0, "CKey2" );
	
	printf("oldvehicles: Ckey1: %d, CKey2: %d", CKey[0], CKey[1] );
	
	for( new i; i < sizeof( CKey ); i++ ) {
		new strFile[48];
		format(strFile, sizeof(strFile), "cars/%d.ini", CKey[i] );
		if(!fexist(strFile)) {
			return SendClientMessage( playerid, COLOR_GREY, "The old vehicle does not exist." );
		}

		new model;
		
		new File:fCar = fopen(strFile, io_read);
		new strData[128], strKey[128];
		while(fread(fCar, strData, sizeof(strData)))
		{
			strKey = ini_GetKey(strData);
			if(strcmp(strKey, "Model") == 0) model = strvalEx(ini_GetValue(strData));
		}
		fclose(fCar);
		
		new output[128];
		format( output, 128, "The old vehicle model of slot %d was: %d", i, model );
		SendClientMessage( playerid, COLOR_GREY, output );
	}
	return 1;
}
