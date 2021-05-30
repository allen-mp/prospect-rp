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
	printf("entering GetPlayerFreeCarslot(%d)", playerid );
	for( new i; i < PlayerInfo[playerid][pMaxCarSlots]; i++ ) {
		if( PlayerVehicles[playerid][i][pvSQLID] < 1 ) {
			printf("exiting GetPlayerFreeCarslot(%d), found free slot: %d", playerid, i);
			return i;
		}
	}
	printf( "exiting GetPlayerFreeCarslot(%d), free car slot not found", playerid );
	return -1;
}

IsVehicleOccupied( vehicleid ) {
	for( new p; p <= GetPlayerPoolSize(); p++ ) {
		if( IsPlayerConnected( p ) ) {
			new pstate = GetPlayerState( p );
			if( pstate == PLAYER_STATE_DRIVER || pstate == PLAYER_STATE_PASSENGER ) {
				if( GetPlayerVehicleID( p ) == vehicleid ) {
					return 1;
				}
			}
		}
	}
	return 0;
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
	return 1;
}

SetVehicleLockState( vehicleid, lockstate ) {
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx( vehicleid, engine, lights, alarm, doors, bonnet, boot, objective );
	SetVehicleParamsEx( vehicleid, engine, lights, alarm, lockstate, bonnet, boot, objective );
	gCarLock[vehicleid] = lockstate;
	return 1;
}
/*new vehAlarm[MAX_VEHICLES]
SetVehicleAlarmState( vehicleid, toggle = 1 ) {
	if( toggle == 0 ) { 
		vehAlarm[vehicleid] = 0;
		return 1;
	}
	vehAlarm[vehicleid] = gettime();
	return 1
}*/

/*new vehicleLights[MAX_VEHICLES]
SetVehicleLightsState( vehicleid, lightstate ) {
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx( vehicleid, engine, lights, alarm, doors, bonnet, boot, objective )
	SetVehicleParamsEx( vehicleid, engine, lightstate, alarm, doors, bonnet, boot, objective )
	vehicleLights[vehicleid] = lightstate
	return 1
}*/

SetVehicleEngineState( vehicleid, enginestate ) {
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx( vehicleid, engine, lights, alarm, doors, bonnet, boot, objective );
	SetVehicleParamsEx( vehicleid, enginestate, lights, alarm, doors, bonnet, boot, objective );
	engineOn[vehicleid] = enginestate;
	return 1;
}