#define MAX_ATMS ( 500 )

#define INVALID_ATM_ID ( -1 )

new Iterator: Atms< MAX_ATMS >;

new ATMID[ MAX_ATMS ];

new EditingAtm[ MAX_PLAYERS ];

enum ATM 
{
	Float: atm_X,
	Float: atm_Y,
	Float: atm_Z,
	Float: atm_fX,
	Float: atm_fY,
	Float: atm_fZ,
	AtmObject,
	bool:Save
}

new AtmInfo[ MAX_ATMS ] [ ATM ];

forward LoadATMS();
public LoadATMS()
{
	new rows = cache_num_rows( );
 	if( rows )
  	{
   		new id, loaded;
		while( loaded < rows )
		{
  			id = cache_get_field_content_int( loaded, "ID" );
		    AtmInfo [ id ] [ atm_X ] = cache_get_field_content_float( loaded, "atm_X" );
		    AtmInfo [ id ] [ atm_Y ] = cache_get_field_content_float( loaded, "atm_Y" );
		    AtmInfo [ id ] [ atm_Z ] = cache_get_field_content_float( loaded, "atm_Z" );
		    AtmInfo [ id ] [ atm_fX ] = cache_get_field_content_float( loaded, "atm_fX" );
		    AtmInfo [ id ] [ atm_fY ] = cache_get_field_content_float( loaded, "atm_fY" );
		    AtmInfo [ id ] [ atm_fZ ] = cache_get_field_content_float( loaded, "atm_fZ" );
		    AtmInfo[ id ] [ AtmObject ] = CreateObject( 19324, AtmInfo [ id ] [ atm_X ], AtmInfo [ id ] [ atm_Y ], AtmInfo [ id ] [ atm_Z ],  AtmInfo [ id ] [ atm_fX ], AtmInfo [ id ] [ atm_fY ], AtmInfo [ id ] [ atm_fZ ], 50 );
   			AtmInfo[ id ] [ Save ] = true;
   			Iter_Add( Atms, id );
   			ATMID [ id ] = id;
		    loaded ++;
	    }
	    printf( "Loaded %d ATMs.", loaded );
	}	
	return 1;
}

/*Hook:a_OnPlayerEditObject(playerid, playerobject, objectid, response, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ)
{
	new Float:oldX, Float:oldY, Float:oldZ, Float:oldRotX, Float:oldRotY, Float:oldRotZ, str[150];
	GetObjectPos(objectid, oldX, oldY, oldZ);
	GetObjectRot(objectid, oldRotX, oldRotY, oldRotZ);

	if(response == EDIT_RESPONSE_FINAL && EditingAtm[ playerid ])
	{
		new query[500];
		DestroyObject(AtmInfo[ EditingAtm[ playerid ] ] [ AtmObject ]);
		AtmInfo[EditingAtm[ playerid ] ] [atm_X] = fX;
		AtmInfo[EditingAtm[ playerid ] ] [atm_Y] = fY;
		AtmInfo[EditingAtm[ playerid ] ] [atm_Z] = fZ;
		AtmInfo[EditingAtm[ playerid ] ] [atm_fX] = fRotX;
		AtmInfo[EditingAtm[ playerid ] ] [atm_fY] = fRotY;
		AtmInfo[EditingAtm[ playerid ] ] [atm_fZ] = fRotZ;
		AtmInfo[ EditingAtm[ playerid ] ] [ AtmObject ] = CreateObject( 19324, AtmInfo [ EditingAtm[ playerid ] ] [ atm_X ], AtmInfo [ EditingAtm[ playerid ] ] [ atm_Y ], AtmInfo [ EditingAtm[ playerid ] ] [ atm_Z ],  
			AtmInfo [ EditingAtm[ playerid ] ] [ atm_fX ], AtmInfo [ EditingAtm[ playerid ] ] [ atm_fY ], AtmInfo [ EditingAtm[ playerid ] ] [ atm_fZ ], 50 );
		mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `atm` SET `atm_X`='%f', `atm_Y`='%f', `atm_Z`='%f', `atm_fX`='%f', `atm_fY`='%f', `atm_fZ`='%f' WHERE `ID`='%d'", AtmInfo [ EditingAtm[ playerid ] ] [ atm_X ], AtmInfo [ EditingAtm[ playerid ] ] [ atm_Y ], AtmInfo [ EditingAtm[ playerid ] ] [ atm_Z ],  
			AtmInfo [ EditingAtm[ playerid ] ] [ atm_fX ], AtmInfo [ EditingAtm[ playerid ] ] [ atm_fY ], AtmInfo [ EditingAtm[ playerid ] ] [ atm_fZ ], EditingAtm[ playerid ] );
		mysql_tquery( sqlGameConnection, query, "", "" );
		format( str, sizeof( str ), "You've adjusted position of ATM ID: %d", EditingAtm[ playerid ]);
		SendClientMessage(playerid, -1, str);
	}
	if(response == EDIT_RESPONSE_CANCEL && EditingAtm[ playerid ])
	{
		DestroyObject( AtmInfo[ EditingAtm[ playerid ] ] [ AtmObject ] );
		AtmInfo[ EditingAtm[ playerid ] ] [ AtmObject ] = CreateObject( 19324, AtmInfo [ EditingAtm[ playerid ] ] [ atm_X ], AtmInfo [ EditingAtm[ playerid ] ] [ atm_Y ], AtmInfo [ EditingAtm[ playerid ] ] [ atm_Z ],  
			AtmInfo [ EditingAtm[ playerid ] ] [ atm_fX ], AtmInfo [ EditingAtm[ playerid ] ] [ atm_fY ], AtmInfo [ EditingAtm[ playerid ] ] [ atm_fZ ], 50 );
		format( str, sizeof( str ), "You haven't adjusted the position of ATM ID: %d as you canceled it", EditingAtm[ playerid ]);
		SendClientMessage(playerid, -1, str);
	}
	return 1;
}*/

CMD:createatm( playerid, params[] )
{
	new id = Iter_Free( Atms ), Float: x, Float: y, Float: z, query[500], str[100];
	if( PlayerInfo[ playerid ] [ pAdmin ] < 3 ) SendClientMessage( playerid, -1, "Not authorized!" );
	else if( id == -1 ) SendClientMessage(playerid, -1, "MAX ATMS has reached");
	else
	{
		GetPlayerPos( playerid, x, y, z );
		AtmInfo[ id ] [ AtmObject ] = CreateObject( 19324, x, y, z, 0, 0, 0, 50 );
		SetPlayerPos(playerid, x+2, y+2, z+2);
		AtmInfo[ id ] [ Save ] = true;
		AtmInfo[ id ] [ atm_X ] = x;
		AtmInfo[ id ] [ atm_Y ] = y;
		AtmInfo[ id ] [ atm_Z ] = z;
		mysql_format( sqlGameConnection, query, sizeof( query ), "INSERT INTO `atm` SET `ID`='%d', `atm_X`='%f', `atm_Y`='%f', `atm_Z`='%f'", id, x, y, z );
		mysql_tquery( sqlGameConnection, query, "", "" );
		Iter_Add(Atms, id);
		ATMID [ id ] = id;
		format(str, sizeof(str), "You've successfully created an ATM ID: %d", id);
		SendClientMessage(playerid, -1, str);
	}
	return 1;
}

CMD:gotoatm( playerid, params[] )
{
	new id, str[100];
	if( PlayerInfo[ playerid ] [ pAdmin ] < 3 ) SendClientMessage( playerid, -1, "Not authorized!" );
	else if( sscanf( params, "i", id ) ) SendClientMessage( playerid, -1, "Usage: /gotoatm <ID>" );
	else if( !Iter_Contains( Atms, id ) ) SendClientMessage( playerid, -1, "The ATM ID you entered does not exist!" );
	else
	{
		SetPlayerPos(playerid, AtmInfo [ id ] [ atm_X ]+2, AtmInfo [ id ] [ atm_Y ]+2, AtmInfo [ id ] [ atm_Z ]+2);
		format( str, sizeof( str ), "You've successfully teleported to ATM ID: %d", id );
		SendClientMessage( playerid, -1, str );
	}
	return 1;
}

CMD:deleteatm( playerid, params[] )
{
	new id, query[100], str[100];
	if( PlayerInfo[ playerid ] [ pAdmin ] < 3 ) SendClientMessage( playerid, -1, "Not authorized!" );
	else if( sscanf( params, "i", id ) ) SendClientMessage( playerid, -1, "Usage: /deleteatm <ID>" );
	else if( !Iter_Contains( Atms, id ) ) SendClientMessage( playerid, -1, "The ATM ID you entered does not exist!" );
	else
	{
		DestroyObject( AtmInfo[ id ] [ AtmObject ] );
		AtmInfo [ id ] [ atm_X ] = 0;
		AtmInfo [ id ] [ atm_Y ] = 0;
		AtmInfo [ id ] [ atm_Z ] = 0;
		Iter_Remove( Atms, id );
		mysql_format( sqlGameConnection, query, sizeof( query ), "DELETE FROM `atm` WHERE `ID`='%d'", id);
		mysql_tquery( sqlGameConnection, query, "", "" );
		format( str, sizeof( str ), "You've successfully deleted ATM ID: %d", id );
		SendClientMessage( playerid, -1, str );
	}
	return 1;
}

CMD:getatmid( playerid )
{
    new str[ 32 ];
    if( PlayerInfo[ playerid ] [ pAdmin ] < 3 ) return SendClientMessage( playerid, -1, "Not authorized!" );
    for( new id; id < MAX_ATMS; id++ )
    {
        if(IsPlayerInRangeOfPoint( playerid, 2.0, AtmInfo[ id ] [ atm_X ], AtmInfo[ id ] [ atm_Y ], AtmInfo[ id ] [ atm_Z ] ) )
        {
            format( str, sizeof str, "ATM ID is %d", ATMID[ id ] ); 
            SendClientMessage( playerid, -1, str );
            return 1;
        }
    }
    SendClientMessage( playerid, -1, "You're not near any ATM!" );
    return 1;
}

CMD:editatm( playerid, params[] )
{
	new id, str[125];
	if( PlayerInfo[ playerid ] [ pAdmin ] < 3 ) SendClientMessage( playerid, -1, "Not authorized!" );
	else if( sscanf( params, "i", id ) ) SendClientMessage( playerid, -1, "Usage: /editatm <ID>" );
	else if( !Iter_Contains( Atms, id ) ) SendClientMessage( playerid, -1, "The ATM ID you entered does not exist!" );
	else
	{
		EditObject(playerid, AtmInfo[ id ] [ AtmObject] );
		format( str, sizeof( str ), "You're editing ATM ID: %d", id );
		SendClientMessage( playerid, -1, str );
		EditingAtm[ playerid ] = id;
	}
	return 1;
}