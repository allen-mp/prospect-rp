/*
  TODO:
  •	 Saving/Loading of pPizzaSkill,
  •	Job rewardsSbased on pPizzaSkill:
  		selling pizza to players, cooking pizza in own house.


*/

#define COLLECTIONPOINT     2109.9875, -1822.7603, 13.5577
#define COLLECTIONPOINTX    2109.9875
#define COLLECTIONPOINTY    -1822.7603
#define COLLECTIONPOINTZ    13.5577
#define JOBPOINT 			2102.0754, -1810.4479, 13.5547
#define CHECKPOINT_JOB_PIZZA 101 //using new enum and starting at previous is less effieicent.


enum PizzaVehicleInfo {
	pvid,
	pvmodel,
	Float: pvx,
	Float: pvy,
	Float: pvz,
	Float: pvrz,
	pvcolor1,
	pvcolor2

}
new PizzaVehicles[5][PizzaVehicleInfo] = {
	{-1, 448, 2097.6240, -1812.2375, 12.8705, 90.0000, -1, -1},
	{-1, 448, 2097.5808, -1813.7677, 12.8705, 90.0000, -1, -1},
	{-1, 436, 2096.7617, -1820.7731, 13.0700, 270.0000, 161, 6},
	{-1, 422, 2096.6328, -1817.4115, 13.2733, 90.0000, 161, 6},
	{-1, 448, 2097.6218, -1815.2719, 12.8705, 90.0000, -1, -1}
};


new PlayerHasPizza[MAX_PLAYERS];
new Float: PlayerPizzaPayment[MAX_PLAYERS];

//==============================[ Functions ]===================================
PizzaInit()
{
	//create icons, labels
	CreateDynamic3DTextLabel("Pizza Collection Point\n", 0xFFFF91FF, COLLECTIONPOINT +0.88, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);


	//create pizza vehicles
	for( new i; i < sizeof( PizzaVehicles ); i++ ) {
	    PizzaVehicles[i][pvid] = CreateVehicle( PizzaVehicles[i][pvmodel], PizzaVehicles[i][pvx], PizzaVehicles[i][pvy], PizzaVehicles[i][pvz], PizzaVehicles[i][pvrz], PizzaVehicles[i][pvcolor1], PizzaVehicles[i][pvcolor2], 100 );
	}
	return 1;
}

//When the player delivers the pizza...
PizzaCheckpoints( playerid ) {
	if(CP[playerid] == CHECKPOINT_JOB_PIZZA ) {
	    if( PlayerHasPizza[playerid] == -1 ) {
			PlayerHasPizza[playerid] = 0;
			
			new int_payment = floatround( PlayerPizzaPayment[playerid] );
	        GiveMoney( playerid, int_payment );
	        
	        PlayerPizzaPayment[playerid] = 0;
	        RemovePlayerAttachedObject(playerid, INDEX_8);
			SetPlayerSpecialAction( playerid, 0 );
			
	        new pizzastr[128];
			format( pizzastr, 128, "You have delivered the pizza and have earned $%d.", int_payment );
	        SendClientMessage( playerid, COLOR_YELLOW, pizzastr );
	        PlayerInfo[playerid][pPizzaSkill]++;
	        
	        //skill unlocks
	        if( PlayerInfo[playerid][pPizzaSkill] == 50 ) {
	            SendClientMessage( playerid, COLOR_YELLOW, "You have completed 50 pizza deliveries." );
	            SendClientMessage( playerid, COLOR_WHITE, "You may now use the pizza cars to deliver pizza." );
	        }
	        
	        if( PlayerInfo[playerid][pPizzaSkill] == 100 ) {
	            SendClientMessage( playerid, COLOR_YELLOW, "You have completed 100 pizza deliveries." );
	            SendClientMessage( playerid, COLOR_WHITE, "You may now sell pizza to other players using a pizza vehicle." );
			}
			
   			if( PlayerInfo[playerid][pPizzaSkill] == 150 ) {
	            SendClientMessage( playerid, COLOR_YELLOW, "You have completed 150 pizza deliveries." );
	            SendClientMessage( playerid, COLOR_WHITE, "You may now cook and serve pizza inside your house, if you have a stove." );
	        }
	        
	        CP[playerid] = CHECKPOINT_NONE;
	    	DisablePlayerCheckpointEx( playerid );
	    }
	    else {
			displayCenterHUDInfo(playerid,  "You must be holding the pizza", 8);
		}
	    return 1;
	}
	return 0;
}
//Is player within range of a pizza vehicle, if so, which is the closest?
IsPlayerNearPizzaVehicleID( playerid ) {
	new Float:vpos[3], closest = -1, Float: distance = 4.1, Float: tmpdist;
	new Float:ppos[3];
	GetPlayerPos( playerid, ppos[0], ppos[1], ppos[2] );
	for( new i; i < sizeof( PizzaVehicles ); i++ ) {
	    GetVehiclePos( PizzaVehicles[i][pvid], vpos[0], vpos[1], vpos[2] );
		tmpdist = VectorSize( vpos[0]-ppos[0], vpos[1]-ppos[1], vpos[2]-ppos[2] );
	    if( tmpdist < distance ) {
	        distance = tmpdist;
	        closest = i;
		}
	}
	return closest;
}

EnterPizzaVehicle( playerid, vehicleid, ispassenger ) {
	for( new i; i < sizeof( PizzaVehicles ); i++ ) {
	    if( vehicleid == PizzaVehicles[i][pvid] && PlayerInfo[playerid][pJob] == JOB_PIZZA ) {
	        if( PizzaVehicles[i][pvmodel] != 448 && PlayerInfo[playerid][pPizzaSkill] < 50 ) {
                displayCenterHUDInfo(playerid,  "You must first complete 50 pizza deliveries before using this vehicle.", 12 );
				ClearAnimations( playerid );
				return 1;
			}
			else if( PlayerInfo[playerid][pJob] != JOB_PIZZA ) {
				SendClientMessage( playerid, COLOR_GREY, "You are not a pizzaman." );
				ClearAnimations( playerid );
				return 1;
			}
		}
	}
	return 0;
}

PizzaQuitJob( playerid ) { //called when player quits the pizza job.

	if( PlayerHasPizza[playerid] == -1 ) {

        RemovePlayerAttachedObject(playerid, INDEX_8);
		SetPlayerSpecialAction( playerid, 0 );
	}
	
	PlayerHasPizza[playerid] = 0;
 PlayerPizzaPayment[playerid] = 0;
	return 1;
}

RandomNumber( min, max )
    return random( max-min ) + min;

//Get a random address for the pizza delivery
GenerateRandomHouse() {
	new ownedHouses = 0, housespool = GetHousePool();
	for( new i; i < housespool; i++ ) { //assure there's enough data to randomize.
	    if( HouseInfo[i][hOwned] ) {
			ownedHouses++;
		}
		if( ownedHouses == 2 ) { break; }
	}
	if( ownedHouses < 2 ) {
	    return -1;
	}
	
	new houseid = RandomNumber( 0, housespool );
	while( !HouseInfo[houseid][hOwned] ) {
	    houseid = RandomNumber( 0, housespool );
	}
	return houseid;
}
//================================[ Commands ]==================================

//For getting pizza from the Pizza Stack window, to deliver.
CMD:getpizza( playerid, params[] ) {
	if( PlayerHasPizza[playerid] != 0 ) {
	    return SendClientMessage( playerid, COLOR_GREY, "You already have someone's pizza order." );
	}
	
	if( GetPlayerState( playerid ) != 1 ) {
	    return SendClientMessage( playerid, COLOR_GREY, "You must hop out of your vehicle first." );
	}
	
	if( PlayerInfo[playerid][pJob] == JOB_PIZZA ) {
		if( IsPlayerInRangeOfPoint( playerid, 2.0, COLLECTIONPOINT ) ) {

			new houseid = GenerateRandomHouse();
			if( houseid == -1 ) {
			    return SendClientMessage( playerid, COLOR_GREY, "Worker: There are no customers at the moment." );
			}
			
			SetPlayerAttachedObject( playerid, INDEX_8, 2814, 6, 0.05, -0.04, -0.20, 250, 0, 0, 1.0, 1.0, 1.0 );
	        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
			SetPlayerArmedWeapon(playerid, 0); // disables weapons
			PlayerHasPizza[playerid] = -1;
			
			CP[playerid] = CHECKPOINT_JOB_PIZZA;
			new Float: displacement = floatsqroot(floatpower(floatabs(floatsub(HouseInfo[houseid][hOutsideX], COLLECTIONPOINTX)), 2) +floatpower(floatabs(floatsub(HouseInfo[houseid][hOutsideY], COLLECTIONPOINTY)), 2) +floatpower(floatabs(floatsub(HouseInfo[houseid][hOutsideZ], COLLECTIONPOINTZ)), 2));
			PlayerPizzaPayment[playerid] = displacement;
			SetPlayerCheckpointEx( playerid, HouseInfo[houseid][hOutsideX], HouseInfo[houseid][hOutsideY], HouseInfo[houseid][hOutsideZ], 2.5);
			displayCenterHUDInfo(playerid,  "~r~/loadpizza~ ~w~to load the pizza into a pizza vehicle", 8); //display for 8 seconds
			SendClientMessage( playerid, COLOR_WHITE, "Pizza Stack Worker says: Here you go, deliver this pizza, get out of here!" );
		}
	}
	else {
		return SendClientMessage( playerid, COLOR_GREY, "You must be within range of the pizza collection point." );
	}
	return 1;
}
//Loading the pizza into a vehicle, inorder to drive.
CMD:loadpizza( playerid, params[] ) {
	new pid = IsPlayerNearPizzaVehicleID( playerid );
    if( pid > -1 ) {
		PlayerHasPizza[playerid] = PizzaVehicles[pid][pvid];
		RemovePlayerAttachedObject(playerid, INDEX_8);
		SetPlayerSpecialAction( playerid, 0 );
		OnePlayAnim(playerid, "GANGS", "shake_cara", 4.1, 0, 0, 0, 0, 0);
		displayCenterHUDInfo(playerid,  "You load the pizza into the vehicle", 8);
		SendClientMessage( playerid, COLOR_GREY, "Get to the marked location, and unload the pizza (/unloadpizza)." );
		return 1;
    }
	SendClientMessage( playerid, COLOR_GREY, "You can only use Pizza Stack vehicles when delivering pizza." );
	return 1;
}

//Getting the pizza from the vehicle, to deliver it.
CMD:unloadpizza( playerid, params[] ) {
	if( PlayerHasPizza[playerid] > 0 ) {
		new Float: vpos[3];
		if( !IsValidVehicle( PlayerHasPizza[playerid] ) ) {
		    PlayerHasPizza[playerid] = -1;
			return SendClientMessage( playerid, COLOR_GREY, "The pizza has gone cold, get a new one." );
		}
		GetVehiclePos( PlayerHasPizza[playerid], vpos[0], vpos[1], vpos[2] );
		if( IsPlayerInRangeOfPoint( playerid, 4.0, vpos[0], vpos[1], vpos[2] ) ) {
		    OnePlayAnim(playerid, "GANGS", "shake_cara", 4.1, 0, 0, 0, 0, 0);
			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
			SetPlayerAttachedObject( playerid, INDEX_8, 2814, 6, 0.05, -0.04, -0.20, 250, 0, 0, 1.0, 1.0, 1.0 );
			SetPlayerArmedWeapon(playerid, 0);
			PlayerHasPizza[playerid] = -1;
			displayCenterHUDInfo( playerid,  "Deliver the pizza", 8 );
		}
		else {
			SendClientMessage( playerid, COLOR_GREY, "You are not by the vehicle that has the pizza." );
		}
		
	}
	return 1;
}

//No longer need these, it slows down compile time.
#undef COLLECTIONPOINT
#undef COLLECTIONPOINTX
#undef COLLECTIONPOINTY
#undef COLLECTIONPOINTZ
#undef JOBPOINT
#undef CHECKPOINT_JOB_PIZZA
