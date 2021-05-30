/*

	Everything related to putting out fires belongs here !
	
*/

// Destroying

CMD:destroyfire( playerid, params[] ) {
	if( PlayerInfo[playerid][pAdmin] >= 3 )
	{
		new fid;
		if( sscanf( params, "d", fid ) ) return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /destroyfire [ID]" );
		
		if( !( 0 < fid < MAX_FIRES ) ) {
			return SendClientMessage( playerid, COLOR_GREY, "You have specified a fire ID out of range. Try again." );
		}
		
		DestroyFire( fid );
		SendClientMessage( playerid, COLOR_GREY, "The fire has been destroyed." );
 	}
	return 1;
}


CMD:getfireid(playerid, params[]) {
	if(PlayerInfo[playerid][pAdmin] >= 3) {
		new fid = GetPlayerClosestFire(playerid);
		new string[30];
		format(string, sizeof(string), "The fireid is %d.", fid);
		SendClientMessage(playerid, COLOR_GREY, string);

	}
	return 1;
	
}

CMD:destroyallfires(playerid, params[]) {
	if(PlayerInfo[playerid][pAdmin] >= 3) {
		
		for(new i; i < GetFirePoolSize(); i++) {
			if(IsValidFire(i)) {
				DestroyFire(i);
			}
		}
		
	}
	return 1;
}

CMD:destroyfiresinarea(playerid, params[]) {
	if(PlayerInfo[playerid][pAdmin] >= 3) {

		new Float:radius;
		if(sscanf(params, "f", radius)) {
			return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /destroyfiresinarea [radius]");
		}

		for(new i; i < MAX_FIRES; i++) {
			if(!IsValidFire(i)) continue;

			new Float:fx, Float:fy, Float:fz;
			GetFirePos(i, fx, fy, fz);
			if(IsPlayerInRangeOfPoint(playerid, radius, fx, fy, fz)) {
				DestroyFire(i);
			}
		}
		SendClientMessage(playerid, COLOR_GREY, "You have destroyed all fires within the specified radius.");
	}

	return 1;
}