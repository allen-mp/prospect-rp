/*
 ____                  __      __                
/\  _`\               /\ \    /\ \  __           
\ \ \L\_\_ __    __   \_\ \   \_\ \/\_\     __   
 \ \  _\/\`'__\/'__`\ /'_` \  /'_` \/\ \  /'__`\ 
  \ \ \/\ \ \//\  __//\ \L\ \/\ \L\ \ \ \/\  __/ 
   \ \_\ \ \_\\ \____\ \___,_\ \___,_\ \_\ \____\
    \/_/  \/_/ \/____/\/__,_ /\/__,_ /\/_/\/____/
                                                 
                                                 
 __  __            ___       __                                       
/\ \/\ \          /\_ \     /\ \                                      
\ \ \ \ \     __  \//\ \    \_\ \     __    ___ ___      __     _ __  
 \ \ \ \ \  /'__`\  \ \ \   /'_` \  /'__`\/' __` __`\  /'__`\  /\`'__\
  \ \ \_/ \/\ \L\.\_ \_\ \_/\ \L\ \/\  __//\ \/\ \/\ \/\ \L\.\_\ \ \/ 
   \ `\___/\ \__/.\_\/\____\ \___,_\ \____\ \_\ \_\ \_\ \__/.\_\\ \_\ 
    `\/__/  \/__/\/_/\/____/\/__,_ /\/____/\/_/\/_/\/_/\/__/\/_/ \/_/ 

	
	UNKNOWN DEVELOPMENT RERQUEST
*/
	
new JacobUtilityTruck;
new jacobsiren;
new jacobsirentoggle;
new JacobTrailer;
new JacobSweeper;
new jacobobjects[3];
new jacobcones[6] = 0;
new jacobconecount;

loadjmtruckitems() {
	jacobobjects[0] = CreateObject(2661,0,0,-1000,0,0,0,100);
	AttachObjectToVehicle(jacobobjects[0], JacobUtilityTruck, -0.014999,-3.060001,0.369999,0.000000,0.000000,360.000305);
	SetObjectMaterialText(jacobobjects[0], "FRED'S\nSANITARY\n555-(3387)", 0, OBJECT_MATERIAL_SIZE_256x128, "Tahoma", 40, 1, 0xFFB5B5B5, 0xFF9D2915, OBJECT_MATERIAL_TEXT_ALIGN_CENTER);

	jacobobjects[1] = CreateObject(2719,0,0,-1000,0,0,0,100);
	SetObjectMaterialText(jacobobjects[1], "FRED'S\nSANITARY\n555-(3387)", 0, OBJECT_MATERIAL_SIZE_256x128, "Tahoma", 40, 1, 0xFFB5B5B5, 0xFF9D2915, OBJECT_MATERIAL_TEXT_ALIGN_CENTER);
	AttachObjectToVehicle(jacobobjects[1], JacobUtilityTruck, -1.285000,-0.275000,0.294999,0.000000,0.000000,-90.0);

	jacobobjects[2] = CreateObject(2719,0,0,-1000,0,0,0,100);
	SetObjectMaterialText(jacobobjects[2], "FRED'S\nSANITARY\n555-(3387)", 0, OBJECT_MATERIAL_SIZE_256x128, "Tahoma", 40, 1, 0xFFB5B5B5, 0xFF9D2915, OBJECT_MATERIAL_TEXT_ALIGN_CENTER);
	AttachObjectToVehicle(jacobobjects[2], JacobUtilityTruck, 1.299999,-0.320000,0.275000,0.000000,0.000000,90.0);

	jacobsiren = CreateObject(11702,0,0,-1000,0,0,0,100);
	AttachObjectToVehicle(jacobsiren, JacobUtilityTruck, 0.000000,0.674999,1.350000,0.000000,0.000000,0.000000);
	return 1;
}

stock hook_DestroyVehicle( vehicleid ) {
	if( vehicleid == JacobUtilityTruck ) {
		for( new i; i < sizeof( jacobcones ); i++ ) {
			if( jacobcones[i] != INVALID_OBJECT_ID ) {
				DestroyDynamicObjectEx( jacobcones[i] );
				jacobcones[i] = 0;
			}
		}
		DestroyObject( jacobsiren );
	}
	DestroyVehicle( vehicleid );
	return 1;
}
#if defined _ALS_DestroyVehicle
    #undef DestroyVehicle
#else
    #define _ALS_DestroyVehicle
#endif
#define DestroyVehicle hook_DestroyVehicle

Hook:FreddieValdemar_OnGameModeInit() {
	JacobUtilityTruck = CreateVehicle( 552, 2511.6226,-2438.4219,13.3145, 44.6, 1, 161, -1 );
	JacobSweeper = CreateVehicle( 574,2506.2109,-2418.6033,13.3450,134.8071, 1, 161, -1 );
	JacobTrailer = CreateVehicle( 611,2515.8647,-2436.7354,13.2719,45.0640, 1, 161, -1 );
	jacobsiren = CreateObject(11701,0,0,-1000,0,0,0,100);
	SetVehicleLockState( JacobSweeper, 1 );
	SetVehicleLockState( JacobUtilityTruck, 1 );
	AttachObjectToVehicle(jacobsiren, JacobUtilityTruck, 0.000000,0.674999,1.350000,0.000000,0.000000,0.000000);
	return 1;
}

Hook:FreddieValdemar_OnGameModeExit() {
	for( new i; i < sizeof( jacobcones ); i++ ) {
		if( jacobcones[i] != INVALID_OBJECT_ID ) {
			DestroyDynamicObjectEx( jacobcones[i] );
			jacobcones[i] = 0;
		}
	}
	DestroyObject( jacobsiren );
	DestroyVehicle( JacobUtilityTruck );
	DestroyVehicle( JacobTrailer );
	DestroyVehicle( JacobSweeper );
	return 1;
}

CMD:jmfredhelp( playerid, params[] ) {
	return SendClientMessage( playerid, COLOR_GREY, "FRED'S HELP: /freds, /jmfredplaceroaditem, /jmfredclearroaditems, /fredlock" );
}

CMD:jmreloadtruckitems( playerid, params[] ) {
	for( new i; i < sizeof( jacobobjects ); i++ ) {
		DestroyObject( jacobobjects[i] );
	}
	DestroyObject( jacobsiren );
	loadjmtruckitems();
	return 1;
}

CMD:freds( playerid, params[] ) {
	if( GetPlayerVehicleID( playerid ) == JacobUtilityTruck ) {
		if( jacobsirentoggle ) {
			DestroyObject( jacobsiren );
			jacobsiren = CreateObject(11702,0,0,-1000,0,0,0,100);
			AttachObjectToVehicle(jacobsiren, JacobUtilityTruck, 0.000000,0.674999,1.350000,0.000000,0.000000,0.000000);
		} else {
			DestroyObject( jacobsiren );
			jacobsiren = CreateObject(11701,0,0,-1000,0,0,0,100);
			AttachObjectToVehicle(jacobsiren, JacobUtilityTruck, 0.000000,0.674999,1.350000,0.000000,0.000000,0.000000);
		}
		jacobsirentoggle = !jacobsirentoggle;
	}
	return 1;
}

CMD:jmfredplaceroaditem( playerid, params[] ) {
	if( jacobconecount >= 5 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You do not have anymore road items to place." );
	}

	new type, text[128];
	if( sscanf( params, "i", type ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /jmfredplaceroaditem [type (0:cone, 1:barrier)]" );
	}
	
	new Float: vpos[3];
	GetVehiclePos( JacobUtilityTruck, vpos[0], vpos[1], vpos[2] );
	if( !IsPlayerInRangeOfPoint( playerid, 20.0, vpos[0], vpos[1], vpos[2] ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "You are not in range of the Utility Truck." );
		
	}
	
	new Float: ppos[4];
	GetPlayerPos( playerid, ppos[0], ppos[1], ppos[2] );
	GetPlayerFacingAngle( playerid, ppos[3] );
	for( new i; i < sizeof( jacobcones ); i++ ) {
		if( jacobcones[i] == 0 ) {
			jacobcones[i] = CreateDynamicObject( (type == 0) ? (1238) : (1459), ppos[0], ppos[1], ppos[2]-0.65, 0.0, 0.0, ppos[3] );
			format( text, sizeof( text ), "* %s places a road item on the ground by their feet.", PlayerICName(playerid) );
			ProxDetector( 30.0, playerid, text, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE );
			jacobconecount++;
			break;
		}
	}
	return 1;
}

CMD:jmfredclearroaditems( playerid, params[] ) {
	new Float:vpos[3];
	GetVehiclePos( JacobUtilityTruck, vpos[0], vpos[1], vpos[2] );
	if( !IsPlayerInRangeOfPoint( playerid, 20.0, vpos[0], vpos[1], vpos[2] ) && !PlayerInfo[playerid][pAdmin] ) {
		return SendClientMessage( playerid, COLOR_GREY, "You are not by the Utility Truck." ); 
	}
	for( new i; i < sizeof( jacobcones ); i++ ) {
		if( jacobcones[i] != 0 ) {
			DestroyDynamicObjectEx( jacobcones[i] );
			jacobcones[i] = 0;
		}
	}
	jacobconecount = 0;
	SendClientMessage( playerid, COLOR_GREY, "You pick up all of the road items." );
	return 1;
}

CMD:fredlock( playerid, params[] ) {
	new Float:vpos[3], text[128];
	GetVehiclePos( JacobUtilityTruck, vpos[0], vpos[1], vpos[2] );
	if( IsPlayerInRangeOfPoint( playerid, 5.0, vpos[0], vpos[1], vpos[2] )) {
		SetVehicleLockState( JacobUtilityTruck, !gCarLock[JacobUtilityTruck] );
		format( text, sizeof( text ), "* %s %s their vehicle.", PlayerICName(playerid), ( gCarLock[JacobUtilityTruck] == 1 ) ? ("locks") : ("unlocks") );
		ProxDetector( 30.0, playerid, text, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE );
	}
	else {
		GetVehiclePos( JacobSweeper, vpos[0], vpos[1], vpos[2] );
		if( IsPlayerInRangeOfPoint( playerid, 5.0, vpos[0], vpos[1], vpos[2] ) ) {
			SetVehicleLockState( JacobSweeper, !gCarLock[JacobSweeper] );
			format( text, sizeof( text ), "* %s %s their vehicle.", PlayerICName(playerid), ( gCarLock[JacobSweeper] == 1 ) ? ("locks") : ("unlocks") );
			ProxDetector( 30.0, playerid, text, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE );
			return 1;
		}
	}
	return 1;
}