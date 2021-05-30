#define MAX_TOWERS ( 100 )

#define INVALID_TOWER_ID ( -1 )

new Iterator: Towers < MAX_TOWERS >;
enum Tower
{
	Float: tower_X,
	Float: tower_Y,
	Float: tower_Z,
	Float: tower_fX,
	Float: tower_fY,
	Float: tower_fZ,
	TowerObject
}
new EditingTower[ MAX_PLAYERS ];
new TowerInfo[ MAX_TOWERS ] [ Tower ];
new TOWERID[ MAX_TOWERS ];

CMD:createtower( playerid, params[ ] )
{
	new id = Iter_Free( Towers ), Float:x, Float:y, Float:z, str[ 150 ], query[ 500 ];
	if( PlayerInfo[ playerid ] [ pAdmin ] < 3 ) SendClientMessage( playerid, -1, "You're not authorized to use this command!" );
	else if( id == -1 ) SendClientMessage( playerid, -1, "MAX TOWERS has reached" );
	else
	{
		GetPlayerPos( playerid, x, y, z );
		TowerInfo[ id ] [ tower_X ] = x;
		TowerInfo[ id ] [ tower_Y ] = y;
		TowerInfo[ id ] [ tower_Z ] = z;
		TowerInfo[ id ] [ TowerObject ] = CreateObject( 3459, TowerInfo[ id ] [ tower_X ], TowerInfo[ id ] [ tower_Y ], TowerInfo[ id ] [ tower_Z ], 
		    	TowerInfo[ id ] [ tower_fX ], TowerInfo[ id ] [ tower_fY ], TowerInfo[ id ] [ tower_fZ ], 50 );
		SetPlayerPos( playerid, x+2, y+2, z+2 );
		Iter_Add( Towers, id );
		format( str, sizeof( str ), "Tower ID: %d has been successfully created", id );
		SendClientMessage( playerid, -1, str );
		mysql_format( sqlGameConnection, query, sizeof( query ), "INSERT INTO `tower` SET `ID`='%d', `tower_X`='%f', `tower_Y`='%f', `tower_Z`='%f'", id, x, y, z );
		mysql_tquery( sqlGameConnection, query, "", "" );
		TOWERID [ id ] = id;
	}
	return 1;
}

CMD:deletetower( playerid, params[] )
{
	new id, query[ 100 ], str[ 100 ];
	if( PlayerInfo[ playerid ] [ pAdmin ] < 3 ) SendClientMessage( playerid, -1, "Not authorized!" );
	else if( sscanf( params, "i", id ) ) SendClientMessage( playerid, -1, "Usage: /deletetower <ID>" );
	else if( !Iter_Contains( Towers, id ) ) SendClientMessage( playerid, -1, "The TOWER ID you entered does not exist!" );
	else
	{
		DestroyObject( TowerInfo[ id ] [ TowerObject ] );
		TowerInfo[ id ] [ tower_X ] = 0;
		TowerInfo[ id ] [ tower_Y ] = 0;
		TowerInfo[ id ] [ tower_Z ] = 0;
		Iter_Remove( Towers, id );
		mysql_format( sqlGameConnection, query, sizeof( query ), "DELETE FROM `tower` WHERE `ID`='%d'", id);
		mysql_tquery( sqlGameConnection, query, "", "" );
		format( str, sizeof( str ), "You've successfully deleted TOWER ID: %d", id );
		SendClientMessage( playerid, -1, str );
	}
	return 1;
}

CMD:edittower( playerid, params[] )
{
	new id, str[125];
	if( PlayerInfo[ playerid ] [ pAdmin ] < 3 ) SendClientMessage( playerid, -1, "Not authorized!" );
	else if( sscanf( params, "i", id ) ) SendClientMessage( playerid, -1, "Usage: /edittower <ID>" );
	else if( !Iter_Contains( Towers, id ) ) SendClientMessage( playerid, -1, "The TOWER ID you entered does not exist!" );
	else
	{
		EditObject(playerid, TowerInfo[ id ] [ TowerObject ] );
		format( str, sizeof( str ), "You're editing TOWER ID: %d", id );
		SendClientMessage( playerid, -1, str );
		EditingTower[ playerid ] = id;
	}
	return 1;
}

CMD:gettowerid( playerid )
{
    new str[ 32 ];
    if( PlayerInfo[ playerid ] [ pAdmin ] < 3 ) return SendClientMessage( playerid, -1, "Not authorized!" );
    for( new id; id < MAX_TOWERS; id++ )
    {
        if(IsPlayerInRangeOfPoint( playerid, 2.0, TowerInfo[ id ] [ tower_X ], TowerInfo[ id ] [ tower_Y ], TowerInfo[ id ] [ tower_Z ] ) )
        {
            format( str, sizeof str, "TOWER ID is %d", TOWERID[ id ] ); 
            SendClientMessage( playerid, -1, str );
            return 1;
        }
    }
    SendClientMessage( playerid, -1, "You're not near any TOWER!" );
    return 1;
}

CMD:gototower( playerid, params[] )
{
	new id, str[100];
	if( PlayerInfo[ playerid ] [ pAdmin ] < 3 ) SendClientMessage( playerid, -1, "Not authorized!" );
	else if( sscanf( params, "i", id ) ) SendClientMessage( playerid, -1, "Usage: /gototower <ID>" );
	else if( !Iter_Contains( Towers, id ) ) SendClientMessage( playerid, -1, "The TOWER ID you entered does not exist!" );
	else
	{
		SetPlayerPos(playerid, TowerInfo[ id ] [ tower_X ]+2, TowerInfo[ id ] [ tower_Y ]+2, TowerInfo[ id ] [ tower_Z ]+2 );
		format( str, sizeof( str ), "You've successfully teleported to TOWER ID: %d", id );
		SendClientMessage( playerid, -1, str );
	}
	return 1;
}

CMD:phone(playerid)
{
	if( PlayerInfo[ playerid ] [ pPnumber ] == 0 ) return SendClientMessage( playerid, -1, "You do not have a cellphone!" );
	new closest_id = -1, Float:distance;

	for(new id = 0; id < MAX_TOWERS; id ++)
	{
		new Float:tmpdistance = GetPlayerDistanceFromPoint( playerid, TowerInfo[ id ] [ tower_X ], TowerInfo[ id ] [ tower_Y ], TowerInfo[ id ] [ tower_Z ] );

		if( tmpdistance <= 650.0 && ( closest_id == -1 || tmpdistance < distance ) )
		{
			closest_id = id;
			distance = tmpdistance;
		}
	}

	if( closest_id == -1 )
	{
		SendClientMessage( playerid, -1, "You've no signal!" );
		return 1;
	}
	else
	{
		if( distance > 0.0 && distance < 401.0 ) return SendClientMessage( playerid, -1, "Your signal is 5/5" );
		else if( distance > 400.0 && distance < 451.0 ) return SendClientMessage( playerid, -1, "Your signal is 4/5" );
		else if( distance > 450.0 && distance < 501.0 ) return SendClientMessage( playerid, -1, "Your signal is 3/5" );
		else if( distance > 550.0 && distance < 601.0 ) return SendClientMessage( playerid, -1, "Your signal is 2/5" );
		else if( distance > 600.0 && distance < 651.0 ) return SendClientMessage( playerid, -1, "Your signal is 1/5" );
	}
	return 1;
}

forward LoadTowers();
public LoadTowers()
{
	new rows = cache_num_rows( );
 	if( rows )
  	{
   		new id, loaded;
		while( loaded < rows )
		{
  			id = cache_get_field_content_int( loaded, "ID" );
		    TowerInfo[ id ] [ tower_X ] = cache_get_field_content_float( loaded, "tower_X" );
		    TowerInfo[ id ] [ tower_Y ] = cache_get_field_content_float( loaded, "tower_Y" );
		    TowerInfo[ id ] [ tower_Z ] = cache_get_field_content_float( loaded, "tower_Z" );
		    TowerInfo[ id ] [ tower_fX ] = cache_get_field_content_float( loaded, "tower_fX" );
		    TowerInfo[ id ] [ tower_fY ] = cache_get_field_content_float( loaded, "tower_fY" );
		   	TowerInfo[ id ] [ tower_fZ ] = cache_get_field_content_float( loaded, "tower_fZ" );
		    TowerInfo[ id ] [ TowerObject ] = CreateObject( 3459, TowerInfo[ id ] [ tower_X ], TowerInfo[ id ] [ tower_Y ], TowerInfo[ id ] [ tower_Z ], 
		    	TowerInfo[ id ] [ tower_fX ], TowerInfo[ id ] [ tower_fY ], TowerInfo[ id ] [ tower_fZ ], 50 );
   			Iter_Add( Towers, id );
		    loaded ++;
		    TOWERID [ id ] = id;
	    }
	    printf( "Loaded %d Towers.", loaded );
	}	
	return 1;
}