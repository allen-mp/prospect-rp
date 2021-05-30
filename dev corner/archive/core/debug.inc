#define DEBUG_NAME	 	"0"
#define COLOR_DEBUG 	0xB8ECB0FF

#if DEVMODE == 0
#define pdebugf(%0,%1,%2) 							\
do 													\
{													\
	new string[strlen(%0)+%1];							\
	format( string, sizeof( string ), %0, %2 ); \
}													\
while( false )

#define pdebug(%0) \
do	\
{	\
	if( strlen(%0) > 1 ) {	\
		new string[sizeof(%0)+20];	\
		strpack( string, %0, sizeof( string ) );	\
		debugout( string );	\
	}	\
}	\
while( false )


//Developer has debugging enabled ingame
new DebugEnabled[MAX_PLAYERS];


static debugout( output[], size_t = sizeof output ) {
	//Send message to developers with debugging enabled
	if( strlen( output ) < 128 ) {
	
		for( new i; i < GetPlayerPoolSize(); i++ ) {
			if( PlayerInfo[i][pDev] > 1 && DebugEnabled[i] ) {
			
				SendClientMessage( i, COLOR_DEBUG, output );
			}
		}
	}
	
	//Open logging file and output the debug msg
	new path[80];
	format( path, 80, "../logs/%s_debug.log", DEBUG_NAME );
	new File:file = fopen( path, io_write );
	
	
	if( file ) {
		
		new hour, minute, second;
		gettime( hour, minute, second );
		
		format( output, size_t, "[%d:%d:%d] %s\n", hour, minute, second, output );
		fwrite( file, output );
	}

	return 1;
}

#pragma unused debugout

public OnPlayerDisconnect( playerid, reason ) {
	//Reset debugging; (showing to what developers?) var
	DebugEnabled[playerid] = 0;
	#if defined debug_OnPlayerDisconnect
		debug_OnPlayerDisconnect( playerid, reason )
	#endif
	return 1
}

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect debug_OnPlayerDisconnect
#if defined debug_OnPlayerDisconnect
    forward debug_OnPlayerDisconnect( playerid, reason )
#endif

public OnGameModeExit() {
	//pdebug("SERVER EXITING OnGameModeExit()")
	#if defined debug_OnPlayerDisconnect
		debug_OnPlayerDisconnect( playerid, reason )
	#endif
	return 1
}

#if defined _ALS_OnGameModeExit
    #undef OnGameModeExit
#else
    #define _ALS_OnGameModeExit
#endif
#define OnGameModeExit debug_OnGameModeExit
#if defined debug_OnGameModeExit
    forward debug_OnGameModeExit()
#endif

//Command to enable in-game debugging
CMD:debug( playerid, params[] ) {
	if( PlayerInfo[playerid][pDev] < 1 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You are not authorized to use this command." );
	}

	DebugEnabled[playerid] = !DebugEnabled[playerid];
	SendClientMessage( playerid, COLOR_DEBUG, "You have toggled debugging." );
	return 1;
}
#else
#define debugf(%0,%1)
#endif
