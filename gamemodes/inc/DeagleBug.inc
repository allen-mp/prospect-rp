new PlayerHasDeagle[MAX_PLAYERS];
forward public DeagleExploit( playerid );
public DeagleExploit( playerid )
{
	if( IsPlayerInAnyVehicle( playerid ) )
	{
		if( GetPlayerWeapon( playerid ) == 24 || PlayerHasDeagle[playerid] == 1)
		{
			SetPlayerArmedWeapon( playerid, 0 );
		}
	}
	return 1;
}