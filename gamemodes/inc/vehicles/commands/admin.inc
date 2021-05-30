static carstr[128];

/*
	******	Administrator Vehicle Commands		*******
	===================================================
*/

CMD:checkcars( playerid, params[] ) { 
	if( PlayerInfo[playerid][pAdmin] < 1 && PlayerInfo[playerid][pDev] < 3 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You are not authorized to use this command." );
	}

	new player;
	if( sscanf( params, "u", player ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /checkvehicles [playerid/playerName]" );
	}
	if( !IsPlayerConnected(player) ) {
		return SendClientMessage( playerid, COLOR_GREY, "That player isn't connected." );
	}
	
	format( carstr, sizeof( carstr ), "_______[ %s's vehicles ]_______", PlayerICName( player ) );
	SendClientMessage( playerid, COLOR_LIGHTBLUE, carstr );
	
	new count;
	for( new i; i < PlayerInfo[player][pMaxCarSlots]; i++ ) {
		
		#define veh(%1) PlayerVehicles[player][i][pv%1]
		if( veh(SQLID) < 1 ) continue;
		
		//veh name
		format( carstr, sizeof( carstr ), "Car slot %d: %s (%s) [vehicleid: %d] %s", i+1, GetVehicleFriendlyNameFromModel( veh(model) ), veh(plate), veh(ID), ( veh(impounded) == 1 ) ? ( ( "- IMPOUNDED" ) ) : ( "" ) );
		SendClientMessage( playerid, COLOR_WHITE, carstr );
		//veh info
		format( carstr, sizeof( carstr ), "	Upgrades: %s %s %s %s", 
			( veh(gps) == 1 ) ? ( ( "GPS" ) ) : ( "" ), ( veh(alarm) == 1 ) ? ( ( "Alarm" ) ) : ( "" ), ( veh(insurance) == 1 ) ? ( ( "Insurance" ) ) : ( "" ),
			( veh(sparekey) == 1 ) ? ( ( "Spare key" ) ) : ( "" ) );
		SendClientMessage( playerid, COLOR_GREY, carstr );
		count++;
		#undef veh
	}
	if( count < 1 ) {
		SendClientMessage( playerid, COLOR_GREY, "This player doesn't own any vehicles." );
	}
	SendClientMessage( playerid, COLOR_GREY, "________________________________" );
	return 1;
}

//========================[ Managing player vehicles ]=========================
CMD:givecar( playerid, params[] ) {
	if( PlayerInfo[playerid][pAdmin] < 4 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You are not authorized to use this." );
	}
	
	new player, model[128];
	if( sscanf( params, "us[128]", player, model ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /givecar [playerid/name] [vehicleName/ID]" );
	}
	
	if( !IsPlayerConnected( player ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "That player isn't connected." );
	}
	
	new modelid = ReturnVehicleModelID( model );
	
	new slot = GetPlayerFreeCarslot( player );
	if( slot == -1 ) {
		return SendClientMessage( playerid, COLOR_GREY, "That player has no more room for more cars." );
	}
	new Float:pos[4];
	GetPlayerPos( playerid, pos[0], pos[1], pos[2] );
	GetPlayerFacingAngle( playerid, pos[3] );
	createPlayerVehicle( player, slot, modelid, pos[0] + 5, pos[1], pos[2], pos[3] );
	
	new isspawned = GetPlayerSpawnedVehicleSlot( playerid );
	
	if( isspawned > -1 ) {
		destroyPlayerVehicle( player, isspawned );
		
	}
	
	spawnPlayerVehicle( player, slot );
	
	
	new vehstr[128];
	format( vehstr, sizeof( vehstr ), "You have given %s a %s.", PlayerICName( player ), GetVehicleFriendlyNameFromModel( modelid ) );
	SendClientMessage( playerid, COLOR_GREY, vehstr );
	SendClientMessage( playerid, COLOR_GREY, "Notify the player of the location of their parked car (here). It will spawn when they tow it." );
	format( vehstr, sizeof( vehstr ), "%s has given you a %s. It's now accessable in /v, it will spawn at the admin's position." , PlayerICName( playerid ), GetVehicleFriendlyNameFromModel( modelid ) );
	SendClientMessage( player, COLOR_LIGHTBLUE, vehstr );
	return 1;
}
CMD:removecar( playerid, params[] ) {
	if( PlayerInfo[playerid][pAdmin] < 4 ) {
		return 1;
	}
	
	new player, slot;
	if( sscanf( params, "ui", player, slot ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /removecar [playerID/playerName] [slot]" );
	}
	
	if( slot > PlayerInfo[player][pMaxCarSlots] || slot < 1 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You must enter a valid car slot (between 1 and 3-4). HINT: see /checkcars." );
	}
	
	if( !IsPlayerConnected( player ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "That player is not connected." );
	}
	
	new car_str[128];
	slot--;
	
	#define veh(%0) PlayerVehicles[player][slot][pv%0]
	
	if( veh(SQLID) < 1 ) {
		return SendClientMessage( playerid, COLOR_GREY, "This player doesn't have a vehicle in this slot." );
	}
	
	format( car_str, 128, "You have destroyed %s's %s.", PlayerICName( player ), GetVehicleFriendlyNameFromModel( veh(model) ) );
	SendClientMessage( playerid, COLOR_GREY, car_str );
	
	
	//Destroy the vehicle completely & despawn it
	ResetPlayerVehicleSlot( player, slot );
	
	#undef veh
	return 1;
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