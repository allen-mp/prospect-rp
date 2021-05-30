static carstr[128]; //for user output, static: only works in this file.
static str[128];
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
new vupgrading[MAX_PLAYERS] = -1;

CMD:carhelp(playerid, params[])
{
	SendClientMessage(playerid, COLOR_YELLOW, "____________________________________________");
	SendClientMessage(playerid, COLOR_WHITE, "COMMANDS: /v, /breakin, /dealerships, /clearallmods, /clearmods, /windows");
	SendClientMessage( playerid, COLOR_WHITE, "COMMANDS: /purchasecar, /spraycar, /lock, /engine, /trunk, /hood, /lights");
	return 1;
}

CMD:vehhelp( playerid, params[] ) {
	return cmd_carhelp( playerid, params );
}

/*
	********		Player Vehicle Commands		*********
	===================================================
*/

#define VCMD:%0(%1) cmdveh_%0(%1)
CMD:v( playerid, params[] ) { 
	new option[20], optional[80];
	if( sscanf( params, "s[20]S()[80]", option, optional ) ) {
		SendClientMessage( playerid, COLOR_GREY,		"{00BFFF}Usage:{FFFFFF} /v [option]" );
		SendClientMessage( playerid, COLOR_GREY,		"Available options:		/v cars, /v spawn, /v park, /v sell, /v dump" );
		SendClientMessage( playerid, COLOR_GREY, 		"Available options:		/v upgrade, /v track, /v givekeys");
		return 1;
	}


	//Commands that do not require the player to have a spawned car
	if( strcmp( option, "cars", true ) == 0 ) { return cmdveh_cars( playerid ); }
	if( strcmp( option, "spawn", true ) == 0 ) { return cmdveh_spawn( playerid, optional ); }
	
	
	//Commands that do require a spawned car
	new slot = GetPlayerSpawnedVehicleSlot( playerid );
	if( slot < 0 || slot > PlayerInfo[playerid][pMaxCarSlots] ) return SendClientMessage( playerid, COLOR_GREY, "You do not have any cars spawned. Use '/v spawn' to spawn one." );
	
	new model[80];
	format( model, 80, GetVehicleFriendlyNameFromModel( PlayerVehicles[playerid][slot][pvmodel] ) );
	
	if( strcmp( option, "track", true ) == 0 ) { return VCMD:track( playerid, slot, model ); }
	if( strcmp( option, "givekeys", true ) == 0 ) { return VCMD:givekeys( playerid, optional, slot, model );}
	if( strcmp( option, "park", true ) == 0 ) { return VCMD:park( playerid, slot, model ); }
	if( strcmp( option, "upgrade", true ) == 0 ) { return VCMD:upgrade( playerid, slot, model ); }
	if( strcmp( option, "sell", true ) == 0 ) { return VCMD:sell( playerid, optional, slot, model ); }
	if( strcmp( option, "dump", true ) == 0 ) { return VCMD:dump( playerid, optional, slot, model ); }
	
	return 1;
}

VCMD:dump( playerid, params[], slot, model[] ) {
	
	#define veh(%0) PlayerVehicles[playerid][slot][pv%0]
	
	new cartext[128];
	if( GetPlayerVehicleID( playerid ) != veh(ID) ) {
		format( cartext, sizeof( cartext ), "You must be inside your %s in order to dump it at the dump.", model );
		return SendClientMessage( playerid, COLOR_GREY, cartext );
	}
	
	if( !IsPlayerInRangeOfPoint( playerid, 5.0, 134.0741,-256.5139,1.3052 ) ) {
		SetPlayerCheckpointEx( playerid, 134.0522,-256.3802,1.3052, 5.0 );
		CP[playerid] = CHECKPOINT_CAR_DUMP;
		return SendClientMessage( playerid, COLOR_GREY, "You must be at the dump. A red marker has been marked on your map showing you the way." );
	}
	
	new moneyback = 0;
	
	if( veh(gps) ) moneyback += 300000;
	if( veh(insurance) ) moneyback += 200000;
	if( veh(alarm) ) moneyback += 100000;
	if( veh(sparekey) ) moneyback += 200000;
	
	new car_str[128];
	if( sscanf( params, "s[128]", car_str ) ) {	
		format( car_str, 128, "You will get a total of $%d for dumping your %s. Are you sure you want to permanently destroy this vehicle?", moneyback, model );
		SendClientMessage( playerid, COLOR_WHITE, car_str );
		SendClientMessage( playerid, COLOR_GREY, "/v dump [confirm], to proceed." );
		return 1;
	}
	
	ResetPlayerVehicleSlot( playerid, slot );
	
	format( car_str, 128, "You have dumped for %s for $%d.", model, moneyback );
	SendClientMessage( playerid, COLOR_GREY, car_str );
	GiveMoney( playerid, moneyback );
	
	#undef veh
	return 1;
}

new playeridcarOffered[MAX_PLAYERS] = {-1, ...};
new carOfferedSlot[MAX_PLAYERS] = {-1, ...};
new carOfferedPrice[MAX_PLAYERS] = {-1, ...};

VCMD:sell( playerid, params[], slot, model[] ) { 
	new player, price, cartext[128];
	if( sscanf( params, "ui", player, price ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /v sell [playerid/playerName] [price]" );
	}
	
	if( playerid == player ) {
		return SendClientMessage( playerid, COLOR_GREY, "You cannot sell this car to yourself, you already own it." );
	}
	if( price < 0 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You must enter a positive number for the price." );
	}
	
	#define veh(%1) PlayerVehicles[playerid][slot][pv%1]
	
	new Float:vpos[3];
	GetVehiclePos( veh(ID), vpos[0], vpos[1], vpos[2] );
	if( !IsPlayerInRangeOfPoint( playerid, 8.0, vpos[0], vpos[1], vpos[2] ) ) {
		format( cartext, sizeof( cartext ), "You must be near your %s in order to sell it.", model );
		return SendClientMessage( playerid, COLOR_GREY, cartext );
	}

	if( !IsPlayerConnected( player ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "ERROR: That player is not connected." );
	}
	
	new Float:ppos[3];
	GetPlayerPos( player, ppos[0], ppos[1], ppos[2] );
	if( IsPlayerInRangeOfPoint( playerid, 3.0, ppos[0], ppos[1], ppos[2] ) ) {
		format( carstr, sizeof( carstr ), "%s has offered you their %s for $%d, type /purchasecar to accept the vehicle.", PlayerICName( playerid ), model, price );
		SendClientMessage( player, COLOR_LIGHTBLUE, carstr );
		
		format( carstr, sizeof( carstr ), "You have offered %s your %s for $%d, please wait for the player to accept it.", PlayerICName( player ), model, price );
		SendClientMessage( playerid, COLOR_LIGHTBLUE, carstr );
		
		playeridcarOffered[player] = playerid;
		carOfferedSlot[player] = slot;
		carOfferedPrice[player] = price;
		return 1;
	
	}
	#undef veh
	SendClientMessage( playerid, COLOR_GREY, "You must be near this player in order to sell them this vehicle." );
	return 1;
}
CMD:purchasecar( playerid, params[] ) {
	if( playeridcarOffered[playerid] > -1 ) {
		new player = playeridcarOffered[playerid];
		new slot = carOfferedSlot[playerid];
		new price = carOfferedPrice[playerid];
		
		if( !IsValidVehicle( PlayerVehicles[player][slot][pvID] ) || PlayerVehicles[player][slot][pvSQLID] < 1 || PlayerVehicles[player][slot][pvspawned] < 1 ) {
			playeridcarOffered[playerid] = -1;
			carOfferedPrice[playerid] = -1;
			carOfferedSlot[playerid] = -1;
			return SendClientMessage( playerid, COLOR_GREY, "This player's vehicle is missing." );
		}
		if( PlayerInfo[playerid][pCash] < carOfferedPrice[playerid] ) {
			return SendClientMessage( playerid, COLOR_GREY, "You cannot afford this." );
		}
		
		new freeslot = GetPlayerFreeCarslot( playerid );
		if( freeslot == -1 ) {
			return SendClientMessage( playerid, COLOR_GREY, "You cannot own anymore vehicles." );
		}
		
		playeridcarOffered[playerid] = -1;
		carOfferedPrice[playerid] = -1;
		carOfferedSlot[playerid] = -1;
		
		GiveMoney( player, price );
		GiveMoney( playerid, -price );
		
		//Destroy the player's car if they got one spawned
		new isspawned = GetPlayerSpawnedVehicleSlot( playerid );
		if( isspawned > -1 ) {
			destroyPlayerVehicle( playerid, isspawned );
		}
		
		//>>>>[Fetch the seller's car info]<<<<<
		#define oldveh(%0) PlayerVehicles[player][slot][pv%0]
		
		//key info
		new modelid = oldveh(model), Float:x, Float:y, Float:z,
			Float:rz = oldveh(parkrz), vw = GetVehicleVirtualWorld( oldveh(ID) ), paintjob = oldveh(paintjob),
			color1 = oldveh(color1), color2 = oldveh(color2);
			
		GetVehiclePos( oldveh(ID), x, y, z );
		GetVehicleZAngle( oldveh(ID), rz );
			
		//vehicle trunk
		/*new Gun1 = TrunkInfo[oldveh(ID)][cGun1], Gun2 = TrunkInfo[oldveh(ID)][cGun1],
			Pot = TrunkInfo[oldveh(ID)][cCannabis], Crack = TrunkInfo[oldveh(ID)][cCocaine], Float:Armor = TrunkInfo[oldveh(ID)][cArmor];*/
			
		//upgrades
		new gps = oldveh(gps), alarm = oldveh(alarm), insurance = oldveh(insurance), sparekey = oldveh(sparekey), plate[9];
		format( plate, 9, "%s", oldveh(plate) );
			
		//vehicle state
		new gas = Gas[oldveh(ID)], dpanels, ddoors, dlights, dtyres, components[14], Float:health;
		GetVehicleHealth( oldveh(ID), health );

		for( new c; c < 14; c++ ) {
			components[c] = GetVehicleComponentInSlot( oldveh(ID), c );
		}
		GetVehicleDamageStatus( oldveh(ID), dpanels, ddoors, dlights, dtyres );
	
		//destroy the seller's car
		ResetPlayerVehicleSlot( player, slot );
		
		#undef oldveh
		
		//>>>>>[Set the buyer's car]<<<<<
		//create the player's new vehicle
		createPlayerVehicle( playerid, freeslot, modelid, x, y, z, rz, color1, color2 );
		
		#define veh(%0) PlayerVehicles[playerid][freeslot][pv%0]
		
		veh(paintjob) = paintjob;
		veh(vw) = vw;
		veh(x) = x;
		veh(y) = y;
		veh(z) = z;
		veh(rz) = rz;
		
		veh(gps) = gps;
		veh(alarm) = alarm;
		veh(insurance) = insurance;
		veh(sparekey) = sparekey;
		
		veh(gas) = gas;
		veh(ddoors) = ddoors;
		veh(dpanels) = dpanels;
		veh(dlights) = dlights;
		veh(dtires) = dtyres;
		veh(damage) = health;
		for( new i; i < 14; i++ ) {
			veh(components)[i] = components[i];
		}
		
		/*veh(Gun1) = Gun1;
		veh(Gun2) = Gun2;
		veh(Pot) = Pot;
		veh(Crack) = Crack;
		veh(Armor) = Armor;*/
		
		format( veh(plate), 9, "%s", plate );
		
		//>>>>>[Spawn, save and output success]<<<<<
		spawnPlayerVehicle( playerid, freeslot, 1 );
		savePlayerVehicle( playerid, freeslot );
		
		printf( "[DEBUG] %s (%d) has sold %s (%d) their %s (%d) for $%d", PlayerICName( player ), PlayerICName( playerid ), GetVehicleFriendlyNameFromModel( veh(model) ), veh(model), price );
		
		format( carstr, sizeof( carstr ), "You have bought the %s from %s for $%d.", GetVehicleFriendlyNameFromModel( veh(model) ), PlayerICName( player ), price );
		SendClientMessage( playerid, COLOR_LIGHTBLUE, carstr );
		
		format( carstr, sizeof( carstr ), "%s has bought the %s from you for $%d.", PlayerICName( playerid ), GetVehicleFriendlyNameFromModel( veh(model) ), price );
		SendClientMessage( player, COLOR_LIGHTBLUE, carstr );
		#undef veh
	
	}
	return 1;
}
VCMD:upgrade( playerid, slot, model[] ) {
	if( IsPlayerInRangeOfPoint( playerid, 5.0, 2324.2505,-2012.4784,13.6222 ) ) { //The new garage map
		
		new menuitems[150];
		#define veh(%1) PlayerVehicles[playerid][slot][pv%1]
		
		format( menuitems, 150, "GPS\t$350,000\nAlarm\t$200,000\nInsurance\t$250,000\nSpare keys\t$250000\nCustom license plate\t$100000\nAdd another vehicle slot\t$15,000,000" );
			
		vupgrading[playerid] = slot;
		new titlestr[100];
		format( titlestr, 100, "{FFB76F}Vehicle Upgrades for %s{FFFFFF}", model );
		ShowPlayerDialog( playerid, DIALOG_VEHICLES_UP2, DIALOG_STYLE_TABLIST, titlestr, menuitems, "Purchase", "Cancel" );
		#undef veh
		return 1;
	}
	SendClientMessage( playerid, COLOR_GREY, "You are not inside the Garage in Willowfield." );
	return 1;
}

new vSpawnTimer[MAX_PLAYERS];
VCMD:spawn( playerid, params[] ) {
	printf( "call ./vehicles/player.inc, /v spawn ( playerid: %d, params[]: %s )", playerid, params );
	
	if( vSpawnTimer[playerid] != 0 && gettime() < ( vSpawnTimer[playerid] + 120 ) ) {
		new _str[120];
		format(_str, sizeof(_str), "You must wait %d seconds before spawning your vehicles.", ( vSpawnTimer[playerid] + 120 ) - gettime());
		return SendClientMessage( playerid, COLOR_GREY, _str);
	}
	
	new cartext[128];
	
	new slot = GetPlayerSpawnedVehicleSlot( playerid );
	if(  slot > -1 ) {
		destroyPlayerVehicle( playerid, slot ); //destroy last spawned car
	}
	
	new model[128], carslot, location[MAX_ZONE_NAME + 1];
	
	if( sscanf( params, "d", carslot ) ) {
		if( !sscanf( params, "s[128]", model ) ) {
		
			//incase they enter the model name instead
			for( new s; s < PlayerInfo[playerid][pMaxCarSlots]; s++ ) {
			
				#define veh(%0) PlayerVehicles[playerid][s][pv%0]
				if( strcmp( model, GetVehicleFriendlyNameFromModel( veh(model) ), true ) == 0 && veh(SQLID) > 0 ) {
				
					if( veh(impounded) ) {
						return SendClientMessage( playerid, COLOR_GREY, "This vehicle is impounded, visit the impound yard to release it." );
					}
					
					vSpawnTimer[playerid] = gettime();
					spawnPlayerVehicle( playerid, s );
					
					GetPosition2DZone( location, veh(parkx), veh(parky), MAX_ZONE_NAME );
					format( cartext, sizeof( cartext ), "You have spawned your %s in its last parked position in %s.", GetVehicleFriendlyNameFromModel( veh(model) ), location );
					SendClientMessage( playerid, COLOR_GREY, cartext );
					return 1;
				}
				#undef veh
			}
		}
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /v spawn [car slot/car name]" );
	}
	
	if( carslot > PlayerInfo[playerid][pMaxCarSlots] || carslot < 1 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You didn't enter a correct car slot, check /v cars for a correct slot." );
	}
	
	carslot--;
	#define veh(%0) PlayerVehicles[playerid][carslot][pv%0]
	if( veh(SQLID) > 0 ) {
		
		if( veh(impounded) ) {
			return SendClientMessage( playerid, COLOR_GREY, "This vehicle is impounded, visit the impound yard to release it." );
		}
		
		vSpawnTimer[playerid] = gettime();
		spawnPlayerVehicle( playerid, carslot );
		GetPosition2DZone( location, veh(parkx), veh(parky), MAX_ZONE_NAME );
		format( cartext, sizeof( cartext ), "You have spawned your %s in its last parked position in %s.", GetVehicleFriendlyNameFromModel( veh(model) ), location );
		SendClientMessage( playerid, COLOR_GREY, cartext );
		return 1;
	
	}
	#undef veh
	
	format( cartext, sizeof( cartext ), "You do not own this vehicle, consider looking at /v cars." );
	SendClientMessage( playerid, COLOR_GREY, cartext );
	return 1;
}
VCMD:park( playerid, slot, model[] ) {
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
VCMD:givekeys( playerid, params[], slot, model[] ) {

	new player, cartext[128];
	if( sscanf( params, "u", player ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /givekeys [playerid/playerName]" );
	}
	
	#define veh(%0) PlayerVehicles[playerid][slot][pv%0]
	if( !PlayerVehicles[playerid][slot][pvsparekey] ) {
		format( cartext, sizeof( cartext ), "You do not have any spare keys for your %s.", model );
		return SendClientMessage( playerid, COLOR_GREY, cartext );
	}
	
	if( !IsPlayerConnected( playerid ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "ERROR: The player you provided is not connected." );
	}
	
	new Float:ppos[3];
	GetPlayerPos( player, ppos[0], ppos[1], ppos[2] );
	if( !IsPlayerInRangeOfPoint( playerid, 3.0, ppos[0], ppos[1], ppos[2] ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "ERROR: You are not near the player." );
	}
	
	if( spareKeys[player] == playerid && spareKeysSlot[player] == slot ) {
		format( cartext, sizeof( cartext ), "You have already given this player your spare key to your %s.", model );
		return SendClientMessage( playerid, COLOR_GREY, cartext );
	}
	
	spareKeys[player] = playerid;
	spareKeysSlot[player] = slot;
	hasgivenKey[playerid] = 1;
	format( carstr, sizeof( carstr ), "%s has given you their spare key to their %s.", PlayerICName( playerid ), model );
	SendClientMessage( player, COLOR_LIGHTBLUE, carstr );
	
	format( carstr, sizeof( carstr ), "You have given %s your spare key to your %s.", PlayerICName( player ), model );
	SendClientMessage( playerid, COLOR_LIGHTBLUE, carstr );
	#undef veh
	return 1;
}
VCMD:track( playerid, slot, model[] ) {
	new cartext[128];
	#define veh(%1) PlayerVehicles[playerid][slot][pv%1]

	if( veh(gps) ) {
		GetVehiclePos( veh(ID), veh(x), veh(y), veh(z) );
		SetPlayerCheckpointEx( playerid, veh(x), veh(y), veh(z), 5.0 );
		CP[playerid] = CHECKPOINT_CAR_TRACK;
		
		format( cartext, sizeof( cartext ), "A marker leading to your %s has been placed on the minimap.", model );
		SendClientMessage( playerid, COLOR_LIGHTBLUE, cartext );
	}
	else {
		format( cartext, sizeof( cartext ), "Your %s does not have a GPS, consider buying one at the Garage in Willowfield.", model );
		SendClientMessage( playerid, COLOR_GREY, cartext );
	}
	#undef veh
	return 1;
}
VCMD:cars( playerid ) {
	
	new day, month, year, hour, minute, second, headerstr[128];

	gettime(hour, minute, second);
	getdate(year, month, day);

	format(headerstr, sizeof(headerstr), "______________[ %s's vehicles (%s %d %d %d:%d:%d) ]______________", PlayerICName(playerid), GetMonthFromInt(month), day, year, hour, minute, second);
	SendClientMessage( playerid, COLOR_LIGHTBLUE, headerstr);

	new count;
	for( new i; i < PlayerInfo[playerid][pMaxCarSlots]; i++ ) {
		
		#define veh(%1) PlayerVehicles[playerid][i][pv%1]
		
		if( veh(SQLID) < 1 ) continue;
		
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
	new slot = GetPlayerSpawnedVehicleSlot( playerid );
	new playermsg[128];
	
	if( slot > -1 ) {
		
		#define veh(%1) PlayerVehicles[playerid][slot][pv%1] //This is just a marco to reduce repetition!
		
		if( veh(SQLID) > 0 && IsValidVehicle( veh(ID) ) ) {
	
			GetVehiclePos( veh(ID), veh(x), veh(y), veh(z) ); //update vehicle coords
			if( IsPlayerInRangeOfPoint( playerid, 5.0, veh(x), veh(y), veh(z) ) ) {
				
				if( veh(locked) ) {
					format( carstr, sizeof( carstr ), "* %s has unlocked their vehicle.", PlayerICName( playerid ) );
					format( playermsg, sizeof( playermsg ), "You've unlocked your vehicle.", PlayerICName( playerid ) );
				}
				else {
					format( carstr, sizeof( carstr ), "* %s has locked their vehicle.", PlayerICName( playerid ) );
					format( playermsg, sizeof( playermsg ), "You've locked your vehicle.", PlayerICName( playerid ) );
				}
				SetPlayerChatBubble(playerid, carstr, COLOR_PURPLE, 20.0, 15000);
				displayCenterHUDInfo( playerid,  playermsg, 8 );
				veh(locked) = !veh(locked);
				
				SetVehicleLockState( veh(ID), veh(locked) );
				//SetVehicleAlarmState( veh(ID), 0 )
				return 1;
			}
		}
		#undef veh
	}
	if( spareKeys[playerid] > -1 && spareKeysSlot[playerid] > -1 ) {
		new Float: vpos[3];
		#define veh(%0) PlayerVehicles[spareKeys[playerid]][spareKeysSlot[playerid]][pv%0]
		
		if( veh(spawned) && IsValidVehicle( veh(ID) ) && veh(SQLID) > 0 ) {
		
			GetVehiclePos( veh(ID), vpos[0], vpos[1], vpos[2] );
			if( IsPlayerInRangeOfPoint( playerid, 5.0, vpos[0], vpos[1], vpos[2] ) ) {
				if( veh(locked) ) {
					format( carstr, sizeof( carstr ), "* %s has unlocked their vehicle.", PlayerICName( playerid ) );
				}
				else {
					format( carstr, sizeof( carstr ), "* %s has locked their vehicle.", PlayerICName( playerid ) );
				}
				SetPlayerChatBubble(playerid, carstr, COLOR_PURPLE, 20.0, 15000);
				veh(locked) = !veh(locked);
				
				SetVehicleLockState( veh(ID), veh(locked) );
				//SetVehicleAlarmState( veh(ID), 0 )
				return 1;
			}
		}
		#undef veh
	}
	return 1;
}


//===================[ Player unowned vehicle commands ]=====================
CMD:paintcar( playerid, params[] ) {
	SendClientMessage( playerid, COLOR_GREY, "Visit a vehicle modification shop to style the paintjob of a vehicle." );
	return 1;
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
					if( PlayerVehicles[i][slot][pvID] == veh && PlayerVehicles[i][slot][pvSQLID] > 0 && PlayerVehicles[i][slot][pvspawned] ) {
						PlayerVehicles[i][slot][pvcolor1] = c1;
						PlayerVehicles[i][slot][pvcolor2] = c2;
						PlayerVehicles[i][slot][pvpaintjob] = 3;
						break;
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
new vehiclePickCount[MAX_PLAYERS];
new lastbrokenin[MAX_PLAYERS];
breakinPlayerVehicle( playerid ) {
	//restrict usage to 1/per30 seconds
	if( gettime() < lastbrokenin[playerid] + 30 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You must wait 30 seconds before breaking into another vehicle." );
	}
	new Float:vpos[3];
	for( new i; i < GetPlayerPoolSize()+1; i++ ) {
		new slot = GetPlayerSpawnedVehicleSlot( i );
		if( slot < 0 ) continue;
		
		#define veh(%1) PlayerVehicles[i][slot][pv%1]
		
		GetVehiclePos( veh(ID), vpos[0], vpos[1], vpos[2] );
		if( IsPlayerFacingPoint( playerid, 5.0, vpos[0], vpos[1], vpos[2], 50.0 ) ) {
			if( !veh(locked) ) {
				return SendClientMessage( playerid, COLOR_GREY, "This vehicle is already unlocked." );
			}
			//If vehicle has alarm notify owner, set the alarm to go off
			if( veh(alarm) ) {
				format( carstr, sizeof( carstr ), "SMS: This is an automated message informing you that your %s alarm has been activated.", GetVehicleFriendlyName( veh(ID) ) );
				SendClientMessage( i, COLOR_YELLOW, carstr );
				//SetVehicleAlarmState( veh(ID) ) //sound the alarm for 35secs
			}
			
			LockPickingCar[playerid] = i;
			vehiclePickCount[playerid] = 0;
			
			//command usage restriction
			lastbrokenin[playerid] = gettime();
			
			SetTimerEx( "OnBreakinExpire", 18*1000, false, "ii", playerid, veh(ID) );
			SetPlayerProgressBarValue( playerid, workingProgress[playerid], 1.0 );
			ShowPlayerProgressBar( playerid, workingProgress[playerid] );
			
			format( carstr, sizeof( carstr ), "* %s fiddles with the vehicle's lock using their screwdriver. *", PlayerICName( playerid ) );
			ProxDetector( 30.0, playerid, carstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE );
			//LoopingAnim( playerid, "POOL", "POOL_ChalkCue", 4.1, 0, 1, 1, 1, 1 )
			ApplyAnimation( playerid, "POOL", "POOL_ChalkCue", 4.1, 0, 1, 1, 1, 1, 1 );
			
			displayCenterHUDInfo( playerid,  "TIP: Rapidly tap ~r~Y~w~ to break in", 4);
			//SendClientMessage( playerid, COLOR_GREY, "TIP: tap Y rapidly to unlock the vehicle." );
			return 1;
		}
		#undef veh
	}
	SendClientMessage( playerid, COLOR_GREY, "You are not by any vehicle." );
	return 1;
}

//player taking too long to breakin
forward OnBreakinExpire( playerid, vehid );
public OnBreakinExpire( playerid, vehid ) {
	if( LockPickingCar[playerid] == -1 )  {
		return 1;
	}
	
	vehiclePickCount[playerid] = 0;
	LockPickingCar[playerid] = -1;
	HidePlayerProgressBar( playerid, workingProgress[playerid] );
	
	SendClientMessage( playerid, COLOR_GREY, "You have failed to break into the vehicle." );
	ClearAnimations( playerid );
	ApplyAnimationEx( playerid, "CARRY", "crry_prtial", 4.0, 0, 0, 0, 0, 0 );
	return 1;
}

new lastcheckplate[MAX_PLAYERS];

CMD:checkplate( playerid, params[] ) {
	if( gettime() < lastcheckplate[playerid] + 10 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You must wait 10 seconds to allow the system to cool down." );
	}
	if( PlayerInfo[playerid][pMember] != 1 && PlayerInfo[playerid][pMember] != 2 && PlayerInfo[playerid][pMember] != 5 && PlayerInfo[playerid][pMember] != 11) {
		return SendClientMessage( playerid, COLOR_GREY, "You do not have access to the correct systems to use this command." );
	}
	
	if( IsPlayerInAnyVehicle( playerid ) ) {
		if( sVehicleInfo[GetDynamicVehicleID( GetPlayerVehicleID( playerid ) )][v_faction] > 0 ) {
			new model[128], plate[128];
			if( sscanf( params, "s[128]s[128]", model, plate ) ) {
				return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /checkplates [vehicleName] [plate]" );
			}
			
			new modelid = ReturnVehicleModelID( model );
			
			lastcheckplate[playerid] = gettime();
			SendClientMessage( playerid, COLOR_LIGHTBLUE, "The following vehicles have matching vehicle plates,");
			
			new count;
			for( new i; i <= GetPlayerPoolSize(); i++ ) {
				for( new slot; slot < PlayerInfo[i][pMaxCarSlots]; slot++ ) {
					if( PlayerVehicles[i][slot][pvmodel] == modelid ) {
						if( strcmp( PlayerVehicles[i][slot][pvplate], plate, true ) == 0 ) {
							if( strlen( PlayerVehicles[i][slot][pvplate] ) > 1 ) {
								format( carstr, sizeof( carstr ), "Owner: %s | Vehicle model: %s | Plate: %s | Vehicle Plate ID: %d", PlayerICName( i ), GetVehicleFriendlyNameFromModel( modelid ), PlayerVehicles[i][slot][pvplate], i+78371 );
								SendClientMessage( playerid, COLOR_GREY, carstr );
								count++;
							}
						}
					}
				}
			}
			if( count == 0 ) {
				SendClientMessage( playerid, COLOR_GREY, "There were no results." );
			}
			return 1;
		}
	}
	SendClientMessage( playerid, COLOR_GREY, "You are not in a faction vehicle." );
	return 1;
}

CMD:clearmods(playerid)
{
    new slot = playerSpawnedVehicle[playerid];
    #define veh(%1) PlayerVehicles[playerid][slot][pv%1]
    if(GetPlayerVehicleID(playerid) != veh(ID)) return SendClientMessage( playerid, COLOR_GREY, "You are not in a vehicle that you own.");
    if(IsAHelicopter(veh(ID)) || IsABike(veh(ID)) || IsABoat(veh(ID)) || IsAPlane(veh(ID))) return SendClientMessage( playerid, COLOR_GREY, "This vehicle cannot have mods.");
    ShowPlayerDialog(playerid, DIALOG_CLEARMOD, DIALOG_STYLE_LIST, "Mod Removal", "Spoiler\nHood\nRoof\nSide Skirt\nNitro\nExhaust\nWheel\nStereo\nHydraulic\nF. Bumper\nR. Bumper", "Select", "Cancel");
    #undef veh
    return 1;
}

CMD:clearallmods(playerid, params[])
{
	if(IsPlayerInAnyVehicle(playerid))
	{
		if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
		{
			new slot = GetPlayerSpawnedVehicleSlot( playerid );
			if( slot < 0 ) return SendClientMessage(playerid, COLOR_GREY, "You do not own any vehicles that are spawned, use /v spawn.");
			new veh = GetPlayerVehicleID(playerid);
			if( PlayerVehicles[playerid][slot][pvID] == veh )
			{
			    new componentid = 0;
				for(new i = 0; i < 14; i++)
				{
					componentid = GetVehicleComponentInSlot(veh, i);
					if(componentid != 0)
					{
						RemoveVehicleComponent(veh, componentid);
						componentid = 0;
					}
				}
				if(IsValidDynamicObject(Neon[veh]))
				{
					DestroyDynamicObjectEx(Neon[veh]);
					Neon[veh] = 0;
				}
				if(IsValidDynamicObject(Neon2[veh]))
				{
					DestroyDynamicObjectEx(Neon2[veh]);
					Neon2[veh] = 0;
				}
				if(IsValidDynamicObject(Neon3[veh]))
				{
					DestroyDynamicObjectEx(Neon3[veh]);
					Neon3[veh] = 0;
				}
				if(IsValidDynamicObject(Neon4[veh]))
				{
					DestroyDynamicObjectEx(Neon4[veh]);
					Neon4[veh] = 0;
				}
				if(IsValidDynamicObject(SirenObject[veh]) ) {
					DestroyDynamicObjectEx(SirenObject[veh]);
					SirenObject[veh] = 0;
				}
				SendClientMessage(playerid, COLOR_WHITE, "Vehicle mods were cleared.");
			}
			else SendClientMessage(playerid, COLOR_GREY, "This is not your vehicle.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "You must be the driver.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not in any vehicle.");
	return 1;
}

CMD:showcarupgrades(playerid, params[])
{
	new vSlot,
		trgID,
		vString[128],
		vUpgrade[128];
	//Detect /showupgrade playername/ID vehicle slot.
	if(sscanf(params, "ud", trgID, vSlot)) 
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /showupgrade [playerid/name] [vehicle ID (1/2/3)]" );

	// Player cannot show himself the upgrades, they have /v cars
	if( trgID == playerid ) return SendClientMessage(playerid, COLOR_GREY, "Use /v cars for your own.");

	//detect if the target ID is online
	if(!IsPlayerConnected(trgID))
		return SendClientMessage(playerid, COLOR_GREY, "Unknown player.");

	//Detect the player is nearby to the target ID
	new Float:fX, Float:fY, Float:fZ;
	GetPlayerPos(trgID, fX, fY, fZ);
	if(!IsPlayerInRangeOfPoint(playerid, 5.0, fX, fY, fZ)) return SendClientMessage(playerid, COLOR_GRAD1, "You are not close enough to that player to offer him weapons.");
	
	//vSlot either it's 1 /or/ 2
	if( vSlot == 1 || vSlot == 2 || vSlot == 3) 
	{
		#define veh(%1) PlayerVehicles[playerid][vSlot-1][pv%1]
	}
	else return SendClientMessage(playerid, -1, "Slot must be 1 or 2");

	//Formats for the vehicle message print.
	format( vString, sizeof( vString ), "%s's vehicle: %s and their upgrades", PlayerICName(playerid), GetVehicleFriendlyNameFromModel( veh(model) ) );

	if( veh(alarm) || veh(insurance) || veh(gps) || veh(sparekey) ) 
	{
		format( vUpgrade, sizeof( vUpgrade ), "( %s%s%s%s)", 
				( veh(gps) == 1 ) ? ( ( "GPS " ) ) : ( "" ),\
				( veh(alarm) == 1 ) ? ( ( "Alarm " ) ) : ( "" ), \
				( veh(insurance) == 1 ) ? ( ( "Insurance " ) ) : ( "" ),\
				( veh(sparekey) == 1 ) ? ( ( "Spare key " ) ) : ( "" ) );
		
		format( vString, sizeof( vString ), "%s %s", vString, vUpgrade );
		SendClientMessage( trgID, COLOR_GREY, vString );
		
	}
	else 
	{
		format( vString, sizeof( vString ), "%s (No upgrades)", vString );
		SendClientMessage( trgID, COLOR_GREY, vString );
	}
	#undef veh
	return 1;
}