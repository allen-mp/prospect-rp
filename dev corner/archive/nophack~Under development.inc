stock NOPHackCHECK(playerid)
{
	if( GetPlayerState(playerid) != PLAYER_STATE_DRIVER ) return 1;
	new nveh = GetPlayerVehicleID(playerid);
	if( PlayerInfo[playerid][pAdmin] > 1 && GetPlayerState(playerid) == PLAYER_STATE_DRIVER ) return 1;
	else if ( sVehicleInfo[nveh][v_faction] != PlayerInfo[playerid][pMember] ) ExecutiveNOPHACK(playerid);
	return 1;
}

stock ExecutiveNOPHACK(playerid)
{
	new nopstring[128];
	new nveh = GetPlayerVehicleID(playerid);
	RemovePlayerFromVehicle(playerid);
	new Float:X, Float:Y, Float:Z;
	GetPlayerPos(playerid, X, Y, Z);
	SetPlayerPos(playerid, X, Y, Z+2);
	NOPHackCHECK(playerid);
	format(nopstring, sizeof(nopstring), "{AA3333}A-Warning{FFFF00}: %s (ID %d) may be NOP hacking - Vehicle model %d.", PlayerOOCName(playerid), playerid, GetVehicleModel(nveh));
	ABroadCast(COLOR_YELLOW, nopstring, 2);
	return 1;
}

