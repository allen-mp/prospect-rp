
//Putting out camp fires
CMD:putoutfire(playerid, params[]) {
	for(new i = 0; i < MAX_FIRES; i++) {
	
	 	if( IsValidFire(i) ) {
		
	 	    if( ( GetFireOwner(i) == playerid ) && GetFireType(i) == FIRE_SMALL ) {
		 	    new
					Float:fx, Float:fy, Float:fz;
				GetFirePos(i, fx, fy, fz);
				
				if(IsPlayerInRangeOfPoint(playerid, 3.0, fx, fy, fz)) {
					DestroyFire( i );
					
					new string[144];
					format( string, sizeof(string), "* %s fiddles with the fire and puts it out.", PlayerICName( playerid ) );
					ProxDetector( 30.0, playerid, string, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE );
					return 1;
	 			}
			}
		}
	}
	SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} You're not by your fire, or haven't placed any.");
	return 1;
}

Hook:Camp_OnPlayerDisconnect(playerid) {
	for(new i = 0; i < MAX_FIRES; i++) //camp fire system
	{ 
	 	if(IsValidFire(i)) {
   			if(GetFireOwner(i) == playerid) {

				DestroyFire(i);
 				break;
			}
		}
	}
	return 1;
}