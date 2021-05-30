new bool:EarthQuakeEnabled, EarthCount, EarthTimer;
#define EarthQuakeBeep "https://tinyurl.com/BeepSARP"
#define DIALOG_EARTH_QUAKE 3967
CMD:earthquakehelp( playerid )
{
	new QuakeStr[ 350 ];
	if( PlayerInfo[ playerid ] [ pAdmin ] < 1337 ) SendClientMessage( playerid, COLOR_GREY, "{ff0000}Error:{ffffff} You're not authorized to use this command!");
	format( QuakeStr, sizeof( QuakeStr ), "{00FF00}/startearthquake --> Used to start an earthquake event {FF0000}(DO NOT USE IF HANK OR SHADOW IS NOT ONLINE){00FF00}\n\
										   /endearthquake --> Used to stop an earthquake event {FF0000}(DO NOT USE IF HANK OR SHADOW IS NOT ONLINE)");
	ShowPlayerDialog( playerid, DIALOG_EARTH_QUAKE, DIALOG_STYLE_MSGBOX, "Earth Quake Help", QuakeStr, "Okay", "" );
	return 1;
}

CMD:startearthquake( playerid, params[ ] )
{
	new confirmation[ 50 ];
	if( PlayerInfo[ playerid ] [ pAdmin ] < 1337 ) SendClientMessage( playerid, COLOR_GREY, "{ff0000}Error:{ffffff} You're not authorized to use this command!");
	if( sscanf( params, "s[50]", confirmation ) ) SendClientMessage( playerid, COLOR_RED, "Are you sure you want to start an earthquake event? Type /startearthquake <CONFIRM>" );
	else if( !strcmp( confirmation, "confirm", true ) )
	{
		for( new i = 0; i < MAX_PLAYERS; i++ )
		{
			SetPlayerDrunkLevel( i, 50000 );
			PlayAudioStreamForPlayer( i, EarthQuakeBeep );
			break;
		}
		EarthQuakeEnabled = true;
		SetWeather( -66 );
		EarthTimer = SetTimer( "StartEarthQuakeEvent", 60000*2, true );
		SendClientMessageToAll( COLOR_YELLOW, "BREAKING NEWS: An earthquake has started! Please STAY IN THE... *BEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEP*" );
	}
	else
	{
		SendClientMessage( playerid, COLOR_RED, "Are you sure you want to start an earthquake event? Type /startearthquake <CONFIRM>" );
	}
	return 1;
}

CMD:endearthquake( playerid, params[ ] )
{
	new confirmation[ 50 ];
	if( PlayerInfo[ playerid ] [ pAdmin ] < 1337 ) SendClientMessage( playerid, COLOR_GREY, "{ff0000}Error:{ffffff} You're not authorized to use this command!");
	if( sscanf( params, "s[50]", confirmation ) ) SendClientMessage( playerid, COLOR_RED, "Are you sure you want to start an earthquake event? Type /endearthquake <CONFIRM>" );
	else if( !strcmp( confirmation, "confirm", true ) )
	{
		for( new i = 0; i < MAX_PLAYERS; i++ )
		{
			SetPlayerDrunkLevel( i, 0 );
			break;
		}
		SetWeather( 1 );
		EarthQuakeEnabled = false;
		EarthCount = 0;
		KillTimer( EarthTimer );
		SendClientMessageToAll( COLOR_YELLOW, "The earthquake event has ended, thanks for roleplaying and participating!" );
		SendRconCommand( "unloadfs EarthQuakeMapping" );
	}
	else
	{
		SendClientMessage( playerid, COLOR_RED, "Are you sure you want to start an earthquake event? Type /endearthquake <CONFIRM>" );
	}
	return 1;
}

forward public StartEarthQuakeEvent( );
public StartEarthQuakeEvent(  )
{
	if( EarthQuakeEnabled == true )
	{
		EarthCount++;
		if( EarthCount == 1 )
		{
			for( new i = 0; i < MAX_PLAYERS; i++ )
			{
				SetPlayerDrunkLevel( i, 50000 );
				break;
			}
			SendClientMessageToAll( COLOR_YELLOW, "** Wind begins blowing faster than before, earth and buildings appear shaking**" );
			SendRconCommand( "loadfs EarthQuakeMapping" );
			return 1;
			
		}
		else if( EarthCount == 3 )
		{
			for( new i = 0; i < MAX_PLAYERS; i++ )
			{
				SetPlayerDrunkLevel( i, 0 );
				break;
			}
			SendClientMessageToAll( COLOR_LIGHTBLUE, "Public Service Announce: Alert! Earthquake! Run out of the buildings, stop wherever you are! Avoid using mobile phones, stay in open areas. Do not go on bridges!" );
			KillTimer( EarthTimer );
		}
	}
	return 1;
}