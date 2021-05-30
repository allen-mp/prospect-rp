



CMD:createfire( playerid, params[] ) {
	if( PlayerInfo[playerid][pAdmin] >= 3 ) {
		new
	 	   	Float:amo, type, Float:x, Float:y, Float:z;

			
		if( sscanf( params, "df", type, amo ) ) {
			return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /createfire [type (1 = small, 2 = medium, 3 = large)] [up/down amount]" );
		}
		if( !IsValidFireType(type) ) {
			return SendClientMessage(playerid, COLOR_GREY, "Uhoh! This isn't a valid fire type. Valid types include 1: small, 2: medium, 3: large." );
		}

		GetPlayerPos( playerid, x, y, z );
		new vw = GetPlayerVirtualWorld(playerid);
		new int = GetPlayerInterior(playerid);
		new slot = CreateFire(type, x, y, z, amo, vw, int);

			
		new string[144];
		format( string, 144, "You have successfully created fire, with fire ID: %d.", slot );
		SendClientMessage( playerid, COLOR_YELLOW, string );
		return 1;

	}
	return 1;
}
/*
CMD:createfirecircle(playerid, params[]) {
	if(PlayerInfo[playerid][pAdmin] >= 3) {

		new Float:radius;
		if(sscanf(params, "f", radius)) {
			return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /createfirecircle [radius]");
		}

		if(radius > 10.0) {
			return SendClientMessage(playerid, COLOR_GREY, "The radius cannot be bigger than 10.0 units.");
		}

		new Float:x, Float:y, Float:z;
		GetPlayerPos(playerid, x, y, z);
		x -= radius;
		y -= radius;

		new Float:angle = acos(
			(2*floatpower(radius,2) - (floatpower(2.4, 2)))/(2*(2*radius))
		);


		for(new i; i < 360; i += angle) {
			x += (radius * -floatsin((i), degrees));
			y += (radius * -floatcos((i), degrees));
			CreateFire(FIRE_LARGE, x, y, z, -2.5);
		}
		SendClientMessage(playerid, COLOR_GREY, "You light a circle of fire, if some aren't appearing, the fire limit may have been reached.");
	}
	return 1;
}*/

CMD:createfireline(playerid, params[]) {
	if(PlayerInfo[playerid][pAdmin] >= 3) {
		new Float:length;
		if(sscanf(params, "f", length)) {
			return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /createfireline [length]");
		}

		if( length > 10 ) {
			return SendClientMessage(playerid, COLOR_GREY, "The length cannot be greater than 10.0 units.");
		}

		new Float:angle;
		GetPlayerFacingAngle(playerid, angle);

		new Float:dir_x = (2.4 * -floatsin((angle), degrees));
		new Float:dir_y = (2.4 * -floatcos((angle), degrees));
		new Float:x, Float:y, Float:z;
		new vw = GetPlayerVirtualWorld(playerid);
		new int = GetPlayerInterior(playerid);
		GetPlayerPos(playerid, x, y, z);

		for(new i; i < length; i++) {
			x += dir_x;
			y += dir_y;
			CreateFire(FIRE_LARGE, x, y, z, -2.5, vw, int );
		}
		SendClientMessage(playerid, COLOR_GREY, "You light a line of fire. If some aren't appearing the fire limit may have been reached.");
	}
	return 1;
}



CMD:lightfire(playerid, params[]) //only source for players to place fires
{
	if(IsAtEvent[playerid])
		return SendClientMessage(playerid, COLOR_GREY, "You cannot do this right now.");
	if(PlayerInfo[playerid][pJailed])
		return SendClientMessage(playerid, COLOR_GREY, "You cannot do this right now.");
	if(pInHospital[playerid])
		return SendClientMessage(playerid, COLOR_GREY, "You cannot do this right now.");
	if(pDeathState[playerid] == PLAYER_STATE_REVIVE)
		return SendClientMessage(playerid, COLOR_GREY, "You cannot do this right now.");
	if(IsPlayerInAnyVehicle(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} You cannot light fires in vehicles.");
	if(!(PlayerCuffed[playerid] == 0 || PlayerInfo[playerid][pJailed] > 0 || inPaintball[playerid] != 0))
		return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} You cannot light a fire right now.");

	if(GetPlayerInterior(playerid) != 0)
		return SendClientMessage(playerid, COLOR_GREY, "You cannot place camp fires inside interiors.");

	if(GetPlayerVirtualWorld(playerid) != 0) {
		return SendClientMessage(playerid, COLOR_GREY, "You cannot place camp fires inside other virtual worlds.");
	}

	new firetype[10], string[128];
	if (sscanf(params, "s[10]", firetype)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /lightfire [camp]");
	new
		Float:player_x, Float:player_y, Float:player_z, Float:player_facing;
   	GetPlayerPos(playerid, player_x, player_y, player_z);
 	if(strmatch("camp", firetype))
	{
	    if(PlayerInfo[playerid][pMatches] > 0)
	    {
	        new Float:fpos[3];
			for(new i = 0; i < MAX_FIRES; i++)
			{
    			GetFirePos(i, fpos[0], fpos[1], fpos[2]);

			    if(GetFireOwner(i) == playerid)
					return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} You can only place one fire.");

			    if(IsPlayerInRangeOfPoint(playerid, 50.0, fpos[0], fpos[1], fpos[2]))
					return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} There is a fire already within 50 metres.");

				//otherwise create a new fire.
				if(!IsValidFire(i)) {
			    	SetPlayerSpecialAction(playerid, 0);
					ApplyAnimationEx(playerid, "CARRY", "putdwn", 3.0, 0, 0, 0, 0, 0, 0);
					GetPlayerFacingAngle(playerid, player_facing);
			    	player_x += (2.0 * -floatsin((-player_facing -180), degrees));
			    	player_y += (2.0 * -floatcos((-player_facing - 180), degrees));
					CreateFire(FIRE_CAMP, player_x, player_y, player_z, -2.5, 0, 0);
		            SetFireOwner(i, playerid);
		            PlayerInfo[playerid][pMatches]--;
					format(string, sizeof(string), "* %s kneels down and lights a fire.", PlayerICName(playerid));
					ProxDetector(30.0, playerid, string, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					SetTimerEx("OnPlayerPlaceFire", 3000, 0, "i", playerid);
					return 1;
				}
			}
			return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} max fires reached.");
		} else return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} You don't have any matches.");
	}
	/*else if(strmatch("big", firetype))
	{
	    //restrictions
	    if(PlayerInfo[playerid][pLevel] < 5)
	        return SendClientMessage(playerid, COLOR_GREY, "You must be at least level 5 to light out of control fires.");
	    if(PlayerInfo[playerid][pJerrycan] < 10 || PlayerInfo[playerid][pMatches] < 1)
			return SendClientMessage(playerid, COLOR_GREY, "You must have a Jerry Can with at least 10 litres of diesel, and matches.");

		//user stuff - animations ect
		new fire_count = 2;
		for(new i = 0; i < MAX_FIRES; i++)
		{
			if(!IsValidFire(i))
			{
			    GetPlayerFacingAngle(playerid, player_facing);
			    player_x += (2.9 + random(2) * -floatsin((-(player_facing - 180)), degrees));
			    player_y += (2.9 + random(2) * -floatcos((-(player_facing - 180)), degrees));
				CreateFire(FIRE_LARGE, player_x, player_y, player_z, -2.3);
		 		SetFireVirtualWorld(i, GetPlayerVirtualWorld(playerid));
		 		fire_count--;
		 		PlayerInfo[playerid][pMatches]--;
		 		if(fire_count == 0)
		 		{
   					format(str, sizeof(str), "* %s spreads diesel gas around using their Jerry Can and lights it.", PlayerICName(playerid));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			 		SetPlayerSpecialAction(playerid, 0);
					ApplyAnimationEx(playerid, "CARRY", "putdwn", 3.0, 0, 0, 0, 0, 0, 0);

					//placing fire, setting variables
				    PlayerInfo[playerid][pJerrycan] = PlayerInfo[playerid][pJerrycan] - 10;
				    return 1;
		 		}
			}
		}
		SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} Max fires has been reached !");
	}*/
	else
	    SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /lightfire [camp]");
	return 1;
}