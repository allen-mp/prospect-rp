#define MAX_DUELS 20
new Iterator:Duel < MAX_DUELS >, DuelPickup[ MAX_DUELS ], Text3D:DuelLabel[ MAX_DUELS ], Float:DuelPos[ MAX_DUELS ] [ 3 ], DuelFamily[ MAX_DUELS ], DuelWorld[ MAX_DUELS ], DuelInterior[ MAX_DUELS ];
CMD:createduel( playerid, params[ ] )
{
	new familyid, id = Iter_Free( Duel ), Float:x, Float:y, Float:z, DuelStr[ 128 ], query[ 128 ];
	if( PlayerInfo[ playerid ] [ pAdmin ] < 1337 ) SendClientMessageEx( playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF} Your admin level is %d but you need 1337!", PlayerInfo[ playerid ] [ pAdmin ] );
	else if( sscanf( params, "i", familyid ) ) SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /createduel [Family ID]" );
	else if( id > 20 ) SendClientMessageEx( playerid, COLOR_GREY, "Max Duel Entrances: %d", MAX_DUELS );
	else
	{
		new vw = GetPlayerVirtualWorld( playerid );
		new int = GetPlayerInterior( playerid );
		GetPlayerPos( playerid, x, y, z );
		DuelPos[ id ] [ 0 ] = x;
		DuelPos[ id ] [ 1 ] = y;
		DuelPos[ id ] [ 2 ] = z;
		DuelFamily[ id ] = familyid;
		DuelWorld[ id ] = vw;
		DuelInterior[ id ] = int;
		DuelPickup[ id ] = CreateDynamicPickup( 1239, 29, DuelPos[ id ] [ 0 ], DuelPos[ id ] [ 1 ], DuelPos[ id ] [ 2 ], DuelWorld[ id ], DuelInterior[ id ] );
		format( DuelStr, sizeof( DuelStr ), "Duel Entrance\nID: %d\nFamily ID: %d", id, DuelFamily[ id ] );
		DuelLabel[ id ] = CreateDynamic3DTextLabel( DuelStr, COLOR_LIGHTBLUE, DuelPos[ id ] [ 0 ], DuelPos[ id ] [ 1 ], DuelPos[ id ] [ 2 ], 30.0, -1, -1, 1, DuelWorld[ id ], DuelInterior[ id ] );
		SendClientMessageEx( playerid, COLOR_GREY, "You've successfully created duel entrance for family ID: %d", DuelFamily[ id ] );
		Iter_Add( Duel, id );
		mysql_format( sqlGameConnection, query, sizeof( query ), "INSERT INTO `duel`( `ID`, `X`, `Y`, `Z`, `FamilyID` ) VALUES( '%d', '%f', '%f', '%f', '%d' )", id, DuelPos[ id ] [ 0 ], 
			DuelPos[ id ] [ 1 ], DuelPos[ id ] [ 2 ], DuelFamily[ id ] );
		mysql_pquery( sqlGameConnection, query );
	}
	return 1;
}
CMD:deleteduel( playerid, params[ ] )
{
	new id, query[ 128 ];
	if( PlayerInfo[ playerid ] [ pAdmin ] < 1337 ) SendClientMessageEx( playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF} Your admin level is %d but you need 1337!", PlayerInfo[ playerid ] [ pAdmin ] );
	else if( sscanf( params, "i", id ) ) SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /deleteduel [ID]" );
	else if( !Iter_Contains( Duel, id ) ) SendClientMessageEx( playerid, COLOR_GREY, "Duel ID: %d does not exist!", id );
	else
	{
		DestroyDynamic3DTextLabel( DuelLabel[ id ] );
		DestroyDynamicPickup( DuelPickup[ id ] );
		Iter_Remove( Duel, id );
		mysql_format( sqlGameConnection, query, sizeof( query ), "DELETE FROM `duel` WHERE `ID`='%d'", id );
		mysql_pquery( sqlGameConnection, query );
		SendClientMessageEx( playerid, COLOR_GREY, "Successfully deleted duel ID: %d", id );
	}
	return 1;
}