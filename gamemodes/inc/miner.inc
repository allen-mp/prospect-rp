new bool:PlayerMining[ MAX_PLAYERS ];
new bool:HasAttachedRock[ MAX_PLAYERS ];
new RandomRock[ MAX_PLAYERS ];
new bool:MinerCP[ MAX_PLAYERS ];
CMD:mine( playerid, params[ ] )
{
	new vehicleid = GetPlayerVehicleID(playerid);
	if( PlayerMining[ playerid ] ) SendClientMessage( playerid, -1, "You're already mining!" );
	else if( HasAttachedRock[ playerid ] ) SendClientMessage( playerid, -1, "You're already holding a rock, deliver it first!" );
	else if( IsPlayerInVehicle(playerid, vehicleid) ) SendClientMessage(playerid, -1, "You can't do that while you're in a vehicle!");
	else if( IsPlayerInRangeOfPoint( playerid, 3.0, 1275.15, -1250.29, 14.34 ) || IsPlayerInRangeOfPoint( playerid, 3.0, 1267.51, -1246.17, 15.15 ) ||
	IsPlayerInRangeOfPoint( playerid, 3.0, 1265.68, -1238.76, 16.72 ) || IsPlayerInRangeOfPoint( playerid, 3.0, 1255.71, -1237.21, 17.97 )
	|| IsPlayerInRangeOfPoint( playerid, 3.0, 1243.11, -1246.73, 14.72 ))
	{
		RandomRock[ playerid ] = random( 100 );
		PlayerMining[ playerid ] = true;
		HasAttachedRock[ playerid ] = true;
		MinerCP[ playerid ] = true;
		GameTextForPlayer(playerid, "~b~You are now mining......", 12000, 3);
   		SetTimerEx("Mining", 12000, false, "c", playerid, ""); //called "function" when 10 seconds elapsed
   		TogglePlayerControllable( playerid, 0 );
   		ApplyAnimation(playerid, "BASEBALL", "BAT_4", 4.1, 1, 0, 0, 0, 0, 1);
   		SetPlayerAttachedObject( playerid, 1, 337, 6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0, 0 );
	}
	else
	{
		SendClientMessage(playerid, -1, "You're not near a mining place!");
	}
	return 1;
}

forward Mining( playerid );
public Mining( playerid )
{
	new vehicleid = GetPlayerVehicleID(playerid);
	if( IsPlayerInVehicle(playerid, vehicleid) )
	{
		if( PlayerMining[ playerid ] == true )
		{
			ClearAnimationsEx(playerid);
			RemovePlayerFromVehicle(playerid);
		}
	}
    SetPlayerAttachedObject( playerid, 9, 3929, 1, 0.242999, 0.324000, 0.012000, -17.200078, 20.699993, 9.800034, 0.579999, 0.617999, 0.676999 );
	SetPlayerSpecialAction( playerid, SPECIAL_ACTION_CARRY );
	ApplyAnimation( playerid, "CARRY", "liftup", 4.1, 0, 0, 0, 0, 0, 1);
	SetPlayerCheckpoint( playerid, 1282.43, -1265.65, 13.54, 5.0 );
    CP[playerid] = CHECKPOINT_MINER;
	MinerStopLooping( playerid );
	TogglePlayerControllable( playerid, 1 );
	RemovePlayerAttachedObject( playerid, 1 );
    return 1;
}

MinerStopLooping( playerid )
{
	ApplyAnimationEx(playerid, "CARRY", "crry_prtial", 4.0, 0, 0, 0, 0, 0);
}

Hook:mfp_OnPlayerEnterCheckpoint(playerid)
{
	if(IsPlayerInRangeOfPoint( playerid, 5.0, 1282.43, -1265.65, 13.54 ) )
	{
		if( !MinerCP[ playerid ] ) SendClientMessage(playerid, -1, "Something went wrong and you can't deliver the rock, please re-log");
		else
		{
			if( RandomRock[ playerid ] == 10 )
			{
				SendClientMessage(playerid, -1, "You've dug a diamond and earnt $500,000");
				GiveMoney( playerid, 500000 );
			}
			else
			{
				SendClientMessage( playerid, -1, "You've dug a normal rock and earnt $850" );
				GiveMoney( playerid, 850);
			}
			RandomRock[ playerid ] = -1;
			PlayerMining[ playerid ] = false;
			HasAttachedRock[ playerid ] = false;
			MinerCP[ playerid ] = false;
			DisablePlayerCheckpoint( playerid );
            CP[playerid] = 0;
			SetPlayerSpecialAction( playerid, SPECIAL_ACTION_NONE );
			RemovePlayerAttachedObject( playerid, 9);
			ApplyAnimation(playerid, "CARRY", "putdwn", 4.1, 0, 0, 0, 0, 0, 1);
		}
	}
    return 1;
}

Hook:mfp_OnGameModeInit()
{
    CreateDynamicPickup(1239,23, 1275.15, -1250.29, 14.34);
	CreateDynamic3DTextLabel("/mine", COLOR_BLUE, 1275.15, -1250.29, 14.34 +0.8, 40.0);
	CreateDynamicPickup(1239,23, 1267.51, -1246.17, 15.15);
	CreateDynamic3DTextLabel("/mine", COLOR_BLUE, 1267.51, -1246.17, 15.15 +0.8, 40.0);
	CreateDynamicPickup(1239,23, 1265.68, -1238.76, 16.72);
	CreateDynamic3DTextLabel("/mine", COLOR_BLUE, 1265.68, -1238.76, 16.72 +0.8, 40.0);
	CreateDynamicPickup(1239,23, 1255.71, -1237.21, 17.97);
	CreateDynamic3DTextLabel("/mine", COLOR_BLUE, 1255.71, -1237.21, 17.97 +0.8, 40.0);
	CreateDynamicPickup(1239,23, 1243.11, -1246.73, 14.72);
	CreateDynamic3DTextLabel("/mine", COLOR_BLUE, 1243.11, -1246.73, 14.72 +0.8, 40.0);
    return 1;
}

Hook:mfp_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	if( PlayerMining[ playerid ] == true )
	{
		ClearAnimationsEx(playerid);
		RemovePlayerFromVehicle(playerid);
		return 1;
	}
	return 1;
}