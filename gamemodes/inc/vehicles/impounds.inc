CMD:impound( playerid, params[] ) {
	if( PlayerInfo[playerid][pMember] != 1 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You must be a police officer to use this command." )
	}
	
	if( !IsPlayerInRangeOfPoint( playerid, 10.0, 2259.2510,-2138.4514,13.5469 ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "You are not by the impound point at the Los Santos Impound Yard." )
	}
	
	new Float: vpos[3], slot
	for( new i; i <= GetPlayerPoolSize(); i++ ) {
		slot = playerSpawnedVehicle[i]
		if( slot < 0 ) continue
		
		#define veh(%1) PlayerVehicles[i][slot][pv%1]
		GetVehiclePos( veh(ID), vpos[0], vpos[1], vpos[2] )
		
		//Is player in range of the vehicle, and impound point?
		if( IsPlayerInRangeOfPoint( playerid, 5.0, vpos[0], vpos[1], vpos[2] ) ) {
			//set the vehicle to impounded
			veh(impounded) = 1
			//reset trunk info
			veh(Gun1) = 0
			veh(Gun2) = 0
			veh(Pot) = 0
			veh(Crack) = 0
			veh(Armor) = 0.0
			destroyPlayerVehicle( i, slot ) //adjust this, playerid = owner playerid not this
			
			SendClientMessage( playerid, COLOR_VEHICLES, "You have successfully impounded the vehicle." )
			return 1
		}
		#undef veh
	}
	return 1
}
CMD:impoundradius( playerid, params[] ) {
	if( PlayerInfo[playerid][pAdmin] < 2 ) {
		return 1
	}
	
	new radius
	if( sscanf( params, "d", radius ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /impoundradius [radius(decimal)]")
	}
	if( radius > 15 || radius < 1 ) {
		return SendClientMessage( playerid, COLOR_GREY, "The radius must be less than 15, and greater than 0." )
	}
	new Float: vpos[3], slot
	for( new i; i <= GetPlayerPoolSize(); i++ ) {
		slot = playerSpawnedVehicle[i]
		if( slot < 0 ) continue
		#define veh(%1) PlayerVehicles[i][slot][pv%1]
		GetVehiclePos( veh(ID), vpos[0], vpos[1], vpos[2] )
		
		//Is player in range of the vehicle, and impound point?
		if( IsPlayerInRangeOfPoint( playerid, float(radius), vpos[0], vpos[1], vpos[2] ) ) {
			//set the vehicle to impounded
			veh(impounded) = 1
			//reset trunk info
			veh(Gun1) = 0
			veh(Gun2) = 0
			veh(Pot) = 0
			veh(Crack) = 0
			veh(Armor) = 0.0
			destroyPlayerVehicle( i, slot ) //adjust this, playerid = owner playerid not this
			
		}
		#undef veh
	}
	SendClientMessage( playerid, COLOR_VEHICLES, "You have successfully impounded the vehicles." )
	return 1
}
CMD:release( playerid, params[] ) {
	new slot, confirm[128]
	if( !IsPlayerInRangeOfPoint( playerid, 3.0, -2033.2562,-117.4913,1035.1719 ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "You must be by the release point at the Impound Yard." );
	}
	if( sscanf( params, "is[128]", slot, confirm ) ) {
		SendClientMessage( playerid, COLOR_WHITE, "It will cost $50,000 to release your vehicle." )
		return SendClientMessage( playerid, COLOR_VEHICLES, "{00BFFF}Usage:{FFFFFF} /release [car slot] [confirm] - to confirm and release your vehicle." )
	}
	
	if( slot > PlayerInfo[playerid][pMaxCarSlots] || slot < 1 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You must enter a valid car slot (between 1 and 3-4). HINT: see /myvehicles." )
	}
	
	if( PlayerInfo[playerid][pCash] < 50000 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You cannot afford to release your vehicle." )
	}
	
	slot--
	#define veh(%1) PlayerVehicles[playerid][slot][pv%1]
	
	if( veh(impounded ) ) {
		GiveMoney( playerid, -50000 )
		veh(impounded) = 0
		spawnPlayerVehicle( playerid, slot )
		SetVehiclePos( veh(ID), 2274.7224,-2127.9663,13.7202 )
		SetVehicleZAngle( veh(ID), 45.0 )
		
		SendClientMessage( playerid, COLOR_VEHICLES, "You have paid the pound to release your vehicle. Find your vehicle on the grass by the fence." )
		return 1
	}
	SendClientMessage( playerid, COLOR_GREY, "This vehicle is not impounded." )
	#undef veh
	return 1
}