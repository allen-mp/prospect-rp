#define MAX_LOCKERS 100
#define DIALOG_LOCKER 6932
new Iterator: Lockers< MAX_LOCKERS >;
enum Locker
{
	Float:LockerX,
	Float:LockerY,
	Float:LockerZ,
	FactionID,
	GroupID,
	dGID,
	LockerPickup,
	Text3D:LockerText,
	WeaponSlot[ 31 ],
	Price[ 31 ]
}
new LockerInfo[ MAX_LOCKERS ] [ Locker ];
new LockerID[ MAX_LOCKERS ];
CMD:createlocker( playerid, params[ ] )
{
	new id = Iter_Free( Lockers ), factionid, groupid, Float:x, Float:y, Float:z, str[1000], query[500], donatedgroupid;
	if( PlayerInfo[ playerid ] [ pAdmin ] < 1337 ) SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You're not enough high admin to use this command!");
	else if( id == -1 ) SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} MAX_LOCKERS reached");
	else if( sscanf ( params, "ii", factionid, groupid, donatedgroupid ) ) SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} /createlocker [FactionID] [GroupID] [Donated-GID]");
	else
	{
		GetPlayerPos( playerid, x, y, z );
		LockerInfo[ id ] [ LockerX ] = x;
		LockerInfo[ id ] [ LockerY ] = y;
		LockerInfo[ id ] [ LockerZ ] = z;
		LockerInfo[ id ] [ FactionID ] = factionid;
		LockerInfo[ id ] [ GroupID ] = groupid;
		LockerInfo[ id ] [ dGID ] = donatedgroupid;
		LockerInfo[ id ] [ LockerPickup ] = CreateDynamicPickup( 1239, 23, LockerInfo[ id ] [ LockerX ], LockerInfo[ id ] [ LockerY ], LockerInfo[ id ] [ LockerZ ] );
		LockerInfo[ id ] [ LockerText ] = CreateDynamic3DTextLabel("Locker", COLOR_HITMAN, LockerInfo[ id ] [ LockerX ], LockerInfo[ id ] [ LockerY ], LockerInfo[ id ] [ LockerZ ], 40.0);
		Iter_Add( Lockers, id );
		LockerID[ id ] = id;
		format( str, sizeof( str ), "You've successfully created locker id: %d, for faction id: %d and group id: %d dgid: %d", id, LockerInfo[ id ] [ FactionID ], LockerInfo[ id ] [ GroupID ], LockerInfo[ id ] [ dGID ] );
		SendClientMessage( playerid, COLOR_GREY, str );
		mysql_format( sqlGameConnection, query, sizeof( query ), "INSERT INTO `locker`(`ID`, `LockerX`, `LockerY`, `LockerZ`, `FactionID`, `GroupID`, `DGID`) \
			VALUES('%d', '%f', '%f', '%f', '%d', '%d', '%d')", id, LockerInfo[ id ] [ LockerX ], LockerInfo[ id ] [ LockerY ], LockerInfo[ id ] [ LockerZ ], 
			LockerInfo[ id ] [ FactionID ], LockerInfo[ id ] [ GroupID ], donatedgroupid );
		mysql_tquery( sqlGameConnection, query, "", "" );
		format( str, sizeof( str ), "[debug] ID: %d, LockerX: %f, LockerY: %f, LockerZ: %f, FactionID:%d, GroupID:%d DGID:%d", id, LockerInfo[ id ] [ LockerX ], LockerInfo[ id ] [ LockerY ], LockerInfo[ id ] [ LockerZ ], 
			LockerInfo[ id ] [ FactionID ], LockerInfo[ id ] [ GroupID ], donatedgroupid );
		SendClientMessage( playerid, -1, str );
	}
	return 1;
}
CMD:editlocker( playerid, params[] )
{
	new option[ 500 ], id, str[ 500 ], query[ 750 ];
	if( PlayerInfo[playerid][pAdmin] < 1337 ) SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You're not enough high admin to use this command!");
	else if(sscanf(params, "ds[200] ", id, option))
	{
		SendClientMessage( playerid, COLOR_GREY, " {00BFFF}Usage:{FFFFFF}/editlocker [Locker ID] [Option]" );
		SendClientMessage( playerid, COLOR_GREY, " Option: Position, Weapon, Price. Delete, ID" );
	}
	else if( !Iter_Contains( Lockers, id ) ) SendClientMessage( playerid, -1, "{FF0000}Error:{FFFFFF} The locker id you written does not exist!" );
	else if( strcmp( option, "position", true ) == 0 )
    {
    	new Float:x, Float:y, Float:z;
    	GetPlayerPos( playerid, Float:x, Float:y, Float:z );
        format( str, sizeof( str ), "You've successfully adjusted position of the locker ID: %d", id );
        SendClientMessage( playerid, COLOR_GREY, str );
        LockerInfo[ id ] [ LockerX ] = x;
		LockerInfo[ id ] [ LockerY ] = y;
		LockerInfo[ id ] [ LockerZ ] = z;
		DestroyDynamicPickup( LockerInfo[ id ] [ LockerPickup ] );
		DestroyDynamic3DTextLabel( LockerInfo[ id ] [ LockerText ] );
		LockerInfo[ id ] [ LockerPickup ] = CreateDynamicPickup( 1239, 23, LockerInfo[ id ] [ LockerX ], LockerInfo[ id ] [ LockerY ], LockerInfo[ id ] [ LockerZ ] );
		LockerInfo[ id ] [ LockerText ] = CreateDynamic3DTextLabel("Locker", COLOR_HITMAN, LockerInfo[ id ] [ LockerX ], LockerInfo[ id ] [ LockerY ], LockerInfo[ id ] [ LockerZ ], 40.0);
		mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `LockerX`='%f', `LockerY`='%f', `LockerZ`='%f' WHERE `ID`='%d'", LockerInfo[ id ] [ LockerX ], LockerInfo[ id ] [ LockerY ], LockerInfo[ id ] [ LockerZ ], id);
		mysql_tquery( sqlGameConnection, query, "", "" );
    }
    else if( strcmp( option, "weapon", true ) == 0 )
    {
    	new weaponslot, weaponid;
    	if( sscanf( params, "ds[200]dd", id, option, weaponslot, weaponid ) ) SendClientMessage( playerid, COLOR_GREY, " {00BFFF}Usage:{FFFFFF}/editlocker [Locker ID] [Weapon] [Weapon slot] [Weapon ID]" );
    	else if( weaponslot > 10 || weaponslot < 1 ) SendClientMessage( playerid, COLOR_GREY, "Weapon slot ID can not be above 10!" );
    	else
    	{
    		if( weaponslot == 1 ){ LockerInfo[ id ] [ WeaponSlot ] [ 0 ] = weaponid; SendClientMessage( playerid, COLOR_GREY, "Successfully changed gun ID for slot ID 1");
						    	   mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `Weapon1`='%d' WHERE `ID`='%d'", weaponid, id ); mysql_tquery( sqlGameConnection, query, "", "" );}
    		else if( weaponslot == 2 ){ LockerInfo[ id ] [ WeaponSlot ] [ 1 ] = weaponid; SendClientMessage( playerid, COLOR_GREY, "Successfully changed gun ID for slot ID 2");
    					    	  		mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `Weapon2`='%d' WHERE `ID`='%d'", weaponid, id ); mysql_tquery( sqlGameConnection, query, "", "" );}
    		else if( weaponslot == 3 ){ LockerInfo[ id ] [ WeaponSlot ] [ 2 ] = weaponid; SendClientMessage( playerid, COLOR_GREY, "Successfully changed gun ID for slot ID 3");
    					    	        mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `Weapon3`='%d' WHERE `ID`='%d'", weaponid, id ); mysql_tquery( sqlGameConnection, query, "", "" );}
    		else if( weaponslot == 4 ){ LockerInfo[ id ] [ WeaponSlot ] [ 3 ] = weaponid; SendClientMessage( playerid, COLOR_GREY, "Successfully changed gun ID for slot ID 4");
    					    	        mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `Weapon4`='%d' WHERE `ID`='%d'", weaponid, id ); mysql_tquery( sqlGameConnection, query, "", "" );}
    		else if( weaponslot == 5 ){ LockerInfo[ id ] [ WeaponSlot ] [ 4 ] = weaponid; SendClientMessage( playerid, COLOR_GREY, "Successfully changed gun ID for slot ID 5");
    					    	        mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `Weapon5`='%d' WHERE `ID`='%d'", weaponid, id ); mysql_tquery( sqlGameConnection, query, "", "" );}
    		else if( weaponslot == 6 ){ LockerInfo[ id ] [ WeaponSlot ] [ 5 ] = weaponid; SendClientMessage( playerid, COLOR_GREY, "Successfully changed gun ID for slot ID 6");
    					    	        mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `Weapon6`='%d' WHERE `ID`='%d'", weaponid, id ); mysql_tquery( sqlGameConnection, query, "", "" );}
    		else if( weaponslot == 7 ){ LockerInfo[ id ] [ WeaponSlot ] [ 6 ] = weaponid; SendClientMessage( playerid, COLOR_GREY, "Successfully changed gun ID for slot ID 7");
    					    	        mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `Weapon7`='%d' WHERE `ID`='%d'", weaponid, id ); mysql_tquery( sqlGameConnection, query, "", "" );}
    		else if( weaponslot == 8 ){ LockerInfo[ id ] [ WeaponSlot ] [ 7 ] = weaponid; SendClientMessage( playerid, COLOR_GREY, "Successfully changed gun ID for slot ID 8");
    					    	        mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `Weapon8`='%d' WHERE `ID`='%d'", weaponid, id ); mysql_tquery( sqlGameConnection, query, "", "" );}
    		else if( weaponslot == 9 ){ LockerInfo[ id ] [ WeaponSlot ] [ 8 ] = weaponid; SendClientMessage( playerid, COLOR_GREY, "Successfully changed gun ID for slot ID 9");
    					    	        mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `Weapon9`='%d' WHERE `ID`='%d'", weaponid, id ); mysql_tquery( sqlGameConnection, query, "", "" );}
    		else if( weaponslot == 10 ){ LockerInfo[ id ] [ WeaponSlot ] [ 9 ] = weaponid; SendClientMessage( playerid, COLOR_GREY, "Successfully changed gun ID for slot ID 10");
    				    	   	         mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `Weapon10`='%d' WHERE `ID`='%d'", weaponid, id ); mysql_tquery( sqlGameConnection, query, "", "" );}
    	} 
    }
    else if( strcmp( option, "price", true ) == 0 )
    {
    	new weaponslot, cripe;
    	if( sscanf( params, "ds[200]dd", id, option, weaponslot, cripe ) ) SendClientMessage( playerid, COLOR_GREY, " {00BFFF}Usage:{FFFFFF}/editlocker [Locker ID] [Price] [Weapon slot] [Amount]" );
    	else
    	{
    		if( weaponslot == 1 ){ LockerInfo[ id ] [ Price ] [ 0 ] = cripe; SendClientMessage( playerid, COLOR_GREY, "Successfully changed price for slot ID 1");
    					    	   mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `WeaponPrice1`='%d' WHERE `ID`='%d'", cripe, id ); mysql_tquery( sqlGameConnection, query, "", "" );}
    		else if( weaponslot == 2 ){ LockerInfo[ id ] [ Price ] [ 1 ] = cripe; SendClientMessage( playerid, COLOR_GREY, "Successfully changed price for slot ID 2");
    					    	        mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `WeaponPrice2`='%d' WHERE `ID`='%d'", cripe, id ); mysql_tquery( sqlGameConnection, query, "", "" );}
    		else if( weaponslot == 3 ){ LockerInfo[ id ] [ Price ] [ 2 ] = cripe; SendClientMessage( playerid, COLOR_GREY, "Successfully changed price for slot ID 3");
    					    	        mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `WeaponPrice3`='%d' WHERE `ID`='%d'", cripe, id ); mysql_tquery( sqlGameConnection, query, "", "" );}
    		else if( weaponslot == 4 ){ LockerInfo[ id ] [ Price ] [ 3 ] = cripe; SendClientMessage( playerid, COLOR_GREY, "Successfully changed price for slot ID 4");
    					    	        mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `WeaponPrice4`='%d' WHERE `ID`='%d'", cripe, id ); mysql_tquery( sqlGameConnection, query, "", "" );}
    		else if( weaponslot == 5 ){ LockerInfo[ id ] [ Price ] [ 4 ] = cripe; SendClientMessage( playerid, COLOR_GREY, "Successfully changed price for slot ID 5");
    					    	        mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `WeaponPrice5`='%d' WHERE `ID`='%d'", cripe, id ); mysql_tquery( sqlGameConnection, query, "", "" );}
    		else if( weaponslot == 6 ){ LockerInfo[ id ] [ Price ] [ 5 ] = cripe; SendClientMessage( playerid, COLOR_GREY, "Successfully changed price for slot ID 6");
    					    	        mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `WeaponPrice6`='%d' WHERE `ID`='%d'", cripe, id ); mysql_tquery( sqlGameConnection, query, "", "" );}
    		else if( weaponslot == 7 ){ LockerInfo[ id ] [ Price ] [ 6 ] = cripe; SendClientMessage( playerid, COLOR_GREY, "Successfully changed price for slot ID 7");
    					    	        mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `WeaponPrice7`='%d' WHERE `ID`='%d'", cripe, id ); mysql_tquery( sqlGameConnection, query, "", "" );}
    		else if( weaponslot == 8 ){ LockerInfo[ id ] [ Price ] [ 7 ] = cripe; SendClientMessage( playerid, COLOR_GREY, "Successfully changed price for slot ID 8");
    					    	        mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `WeaponPrice8`='%d' WHERE `ID`='%d'", cripe, id ); mysql_tquery( sqlGameConnection, query, "", "" );}
    		else if( weaponslot == 9 ){ LockerInfo[ id ] [ Price ] [ 8 ] = cripe; SendClientMessage( playerid, COLOR_GREY, "Successfully changed price for slot ID 9");
    					    	        mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `WeaponPrice9`='%d' WHERE `ID`='%d'", cripe, id ); mysql_tquery( sqlGameConnection, query, "", "" );}
    		else if( weaponslot == 10 ){ LockerInfo[ id ] [ Price ] [ 9 ] = cripe; SendClientMessage( playerid, COLOR_GREY, "Successfully changed price for slot ID 10");
    					    	         mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `WeaponPrice10`='%d' WHERE `ID`='%d'", cripe, id ); mysql_tquery( sqlGameConnection, query, "", "" );}
    	}
    }
    else if( strcmp( option, "delete", true ) == 0 )
    {
    	LockerInfo[ id ] [ LockerX ] = 0;
		LockerInfo[ id ] [ LockerY ] = 0;
		LockerInfo[ id ] [ LockerZ ] = 0;
		LockerInfo[ id ] [ FactionID ] = 0;
		LockerInfo[ id ] [ dGID ] = 0;
		LockerInfo[ id ] [ GroupID ] = 0;
		LockerInfo[ id ] [ WeaponSlot ] [ 0 ] = 0;
		LockerInfo[ id ] [ WeaponSlot ] [ 1 ] = 0;
		LockerInfo[ id ] [ WeaponSlot ] [ 2 ] = 0;
	    LockerInfo[ id ] [ WeaponSlot ] [ 3 ] = 0;
		LockerInfo[ id ] [ WeaponSlot ] [ 4 ] = 0;
		LockerInfo[ id ] [ WeaponSlot ] [ 5 ] = 0;
		LockerInfo[ id ] [ WeaponSlot ] [ 6 ] = 0;
		LockerInfo[ id ] [ WeaponSlot ] [ 7 ] = 0;
		LockerInfo[ id ] [ WeaponSlot ] [ 8 ] = 0;
		LockerInfo[ id ] [ WeaponSlot ] [ 9 ] = 0;
		LockerInfo[ id ] [ Price ] [ 0 ] = 0;
		LockerInfo[ id ] [ Price ] [ 1 ] = 0;
		LockerInfo[ id ] [ Price ] [ 2 ] = 0;
	    LockerInfo[ id ] [ Price ] [ 3 ] = 0;
		LockerInfo[ id ] [ Price ] [ 4 ] = 0;
		LockerInfo[ id ] [ Price ] [ 5 ] = 0;
		LockerInfo[ id ] [ Price ] [ 6 ] = 0;
		LockerInfo[ id ] [ Price ] [ 7 ] = 0;
		LockerInfo[ id ] [ Price ] [ 8 ] = 0;
		LockerInfo[ id ] [ Price ] [ 9 ] = 0;
		DestroyDynamicPickup( LockerInfo[ id ] [ LockerPickup ] );
		DestroyDynamic3DTextLabel( LockerInfo[ id ] [ LockerText ] );
		Iter_Remove( Lockers, id );
		mysql_format( sqlGameConnection, query, sizeof( query ), "DELETE FROM `locker` WHERE `ID`='%d'", id);
		mysql_tquery( sqlGameConnection, query, "", "" );
		format( str, sizeof( str ), "You've successfully deleted locker id: %d", id );
		SendClientMessage( playerid, COLOR_GREY, str );
    }
    else if( strcmp( option, "factionid", true ) == 0 )
    {
    	new factionid;
    	if( sscanf( params, "ds[200]dd", id, option, factionid ) ) {SendClientMessage( playerid, COLOR_GREY, " {00BFFF}Usage:{FFFFFF}/editlocker [Locker ID] [Factionid]" );
    															    SendClientMessage( playerid, COLOR_GREY, "If you want to set this locker only for exact faction id/not to public, make sure to set groupid, donatedgroup -1!");}
    	else
    	{
    		LockerInfo[ id ] [ FactionID ] = factionid;
    		mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `FactionID`='%d' WHERE `ID`='%d'", factionid, id ); 
    		mysql_tquery( sqlGameConnection, query, "", "" );
    		format (str, sizeof( str ), "Successfully adjusted faction id %d for locker id %d", factionid, id );
    		SendClientMessage( playerid, COLOR_GREY, str );
    	}
    }
    else if( strcmp( option, "groupid", true ) == 0 )
    {
    	new groupid;
    	if( sscanf( params, "ds[200]dd", id, option, groupid ) ) {SendClientMessage( playerid, COLOR_GREY, " {00BFFF}Usage:{FFFFFF}/editlocker [Locker ID] [Groupid]" );
    															  SendClientMessage( playerid, COLOR_GREY, "If you want to set this locker only for exact group id/not to public, make sure to set factionid, donatedgroup -1!");}
    	else
    	{
    		LockerInfo[ id ] [ GroupID ] = groupid;
    		mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `GroupID`='%d' WHERE `ID`='%d'", groupid, id ); 
    		mysql_tquery( sqlGameConnection, query, "", "" );
    		format (str, sizeof( str ), "Successfully adjusted groupid id %d for locker id %d", groupid, id );
    		SendClientMessage( playerid, COLOR_GREY, str );
    	}
    }
    else if( strcmp( option, "donatedgroup", true ) == 0 )
    {
    	new groupid;
    	if( sscanf( params, "ds[200]dd", id, option, groupid ) ) {SendClientMessage( playerid, COLOR_GREY, " {00BFFF}Usage:{FFFFFF}/editlocker [Locker ID] [donated group id]" );
    															  SendClientMessage( playerid, COLOR_GREY, "If you want to set this locker only for exact group id/not to public, make sure to set faction id, groupid -1!");}
    	else
    	{
    		LockerInfo[ id ] [ GroupID ] = groupid;
    		mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `DGID`='%d' WHERE `ID`='%d'", groupid, id ); 
    		mysql_tquery( sqlGameConnection, query, "", "" );
    		format (str, sizeof( str ), "Successfully adjusted donatedgroup id %d for locker id %d", groupid, id );
    		SendClientMessage( playerid, COLOR_GREY, str );
    	}
    }
    else if( strcmp( option, "reset", true ) == 0 )
    {
    		LockerInfo[ id ] [ WeaponSlot ] [ 0 ] = 0;
		    LockerInfo[ id ] [ WeaponSlot ] [ 1 ] = 0;
		    LockerInfo[ id ] [ WeaponSlot ] [ 2 ] = 0;
		    LockerInfo[ id ] [ WeaponSlot ] [ 3 ] = 0;
		    LockerInfo[ id ] [ WeaponSlot ] [ 4 ] = 0;
		    LockerInfo[ id ] [ WeaponSlot ] [ 5 ] = 0;
		    LockerInfo[ id ] [ WeaponSlot ] [ 6 ] = 0;
		    LockerInfo[ id ] [ WeaponSlot ] [ 7 ] = 0;
		    LockerInfo[ id ] [ WeaponSlot ] [ 8 ] = 0;
		    LockerInfo[ id ] [ WeaponSlot ] [ 9 ] = 0;
			LockerInfo[ id ] [ Price ] [ 0 ] = 0;
			LockerInfo[ id ] [ Price ] [ 1 ] = 0;
			LockerInfo[ id ] [ Price ] [ 2 ] = 0;
		    LockerInfo[ id ] [ Price ] [ 3 ] = 0;
			LockerInfo[ id ] [ Price ] [ 4 ] = 0;
			LockerInfo[ id ] [ Price ] [ 5 ] = 0;
			LockerInfo[ id ] [ Price ] [ 6 ] = 0;
			LockerInfo[ id ] [ Price ] [ 7 ] = 0;
			LockerInfo[ id ] [ Price ] [ 8 ] = 0;
			LockerInfo[ id ] [ Price ] [ 9 ] = 0;
		    mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `locker` SET `Weapon1`=0, `Weapon2`=0, `Weapon3`=0, `Weapon4`=0, `Weapon5`=0, `Weapon6`=0, `Weapon7`=0, \
		    `Weapon8`=0, `Weapon9`=0, `Weapon10`=0,	WHERE `ID`='%d'", id);
			mysql_tquery( sqlGameConnection, query, "", "" );
			format (str, sizeof( str ), "Successfully resetted locker id %d", id );
    		SendClientMessage( playerid, COLOR_GREY, str );
    }
    else if( strcmp( option, "id", true ) == 0 )
    {
    	for( new ids; ids < MAX_LOCKERS; ids++ )
    	{
	        if(IsPlayerInRangeOfPoint( playerid, 2.0, LockerInfo[ ids ] [ LockerX ], LockerInfo[ ids ] [ LockerY ], LockerInfo[ ids ] [ LockerZ ] ) )
	        {
	            format( str, sizeof str, "Locker ID is %d", LockerID [ ids ] ); 
	            SendClientMessage( playerid, -1, str );
	            return 1;
	        }
	    }
   	    SendClientMessage( playerid, -1, "You're not near any locker!" );	
    }
	return 1;
}
new BID[ MAX_PLAYERS ];
CMD:locker( playerid )
{
	new lockerdialog[1000], WeaponName1[100], WeaponName2[100], WeaponName3[100], WeaponName4[100], WeaponName5[100], WeaponName6[100], WeaponName7[100], WeaponName8[100],
	WeaponName9[100], WeaponName10[100];
	for( new id; id < MAX_LOCKERS; id++ )
    {
    	if(IsPlayerInRangeOfPoint( playerid, 2.0, LockerInfo[ id ] [ LockerX ], LockerInfo[ id ] [ LockerY ], LockerInfo[ id ] [ LockerZ ] ) )
        {
        	if( LockerInfo[ id ] [ FactionID ] == 0 || 
        		LockerInfo[ id ] [ GroupID ] == 0 || 
        		LockerInfo[ id ] [ dGID ] == 0 || 
        		LockerInfo[ id ] [ GroupID ] == PlayerInfo[ playerid ] [ pGroup ] || 
        		LockerInfo[ id ] [ FactionID ] == PlayerInfo[ playerid ] [ pMember ] ||
        		LockerInfo[ id ] [ dGID ] == PlayerInfo[ playerid ] [ pDonatedGroup ])
        	{
	    		GetWeaponName( LockerInfo[ id ] [ WeaponSlot ] [ 0 ], WeaponName1, sizeof( WeaponName1 ) );
	    		GetWeaponName( LockerInfo[ id ] [ WeaponSlot ] [ 1 ], WeaponName2, sizeof( WeaponName2 ) );
	    		GetWeaponName( LockerInfo[ id ] [ WeaponSlot ] [ 2 ], WeaponName3, sizeof( WeaponName3 ) );
	    		GetWeaponName( LockerInfo[ id ] [ WeaponSlot ] [ 3 ], WeaponName4, sizeof( WeaponName4 ) );
	    		GetWeaponName( LockerInfo[ id ] [ WeaponSlot ] [ 4 ], WeaponName5, sizeof( WeaponName5 ) );
	    		GetWeaponName( LockerInfo[ id ] [ WeaponSlot ] [ 5 ], WeaponName6, sizeof( WeaponName6 ) );
	    		GetWeaponName( LockerInfo[ id ] [ WeaponSlot ] [ 6 ], WeaponName7, sizeof( WeaponName7 ) );
	    		GetWeaponName( LockerInfo[ id ] [ WeaponSlot ] [ 7 ], WeaponName8, sizeof( WeaponName8 ) );
	    		GetWeaponName( LockerInfo[ id ] [ WeaponSlot ] [ 8 ], WeaponName9, sizeof( WeaponName9 ) );
	    		GetWeaponName( LockerInfo[ id ] [ WeaponSlot ] [ 9 ], WeaponName10, sizeof( WeaponName10 ) );
	    		if( LockerInfo[ id ] [ WeaponSlot ] [ 0 ] == 0 ) WeaponName1 = "None";
	    		if( LockerInfo[ id ] [ WeaponSlot ] [ 1 ] == 0 ) WeaponName2 = "None";
	    		if( LockerInfo[ id ] [ WeaponSlot ] [ 2 ] == 0 ) WeaponName3 = "None";
	    		if( LockerInfo[ id ] [ WeaponSlot ] [ 3 ] == 0 ) WeaponName4 = "None";
	    		if( LockerInfo[ id ] [ WeaponSlot ] [ 4 ] == 0 ) WeaponName5 = "None";
	    		if( LockerInfo[ id ] [ WeaponSlot ] [ 5 ] == 0 ) WeaponName6 = "None";
	    		if( LockerInfo[ id ] [ WeaponSlot ] [ 6 ] == 0 ) WeaponName7 = "None";
	    		if( LockerInfo[ id ] [ WeaponSlot ] [ 7 ] == 0 ) WeaponName8 = "None";
	    		if( LockerInfo[ id ] [ WeaponSlot ] [ 8 ] == 0 ) WeaponName9 = "None";
	    		if( LockerInfo[ id ] [ WeaponSlot ] [ 9 ] == 0 ) WeaponName10 = "None";
	    		BID[ playerid ] = id;
	    		format( lockerdialog, sizeof( lockerdialog ), "%s\t\t$%d\n%s\t\t$%d\n%s\t\t$%d\n%s\t\t$%d\n%s\t\t$%d\n%s\t\t$%d\n%s\t\t$%d\n%s\t\t$%d\n%s\t\t$%d\n%s\t\t$%d", 
	    			WeaponName1, LockerInfo[ id ] [ Price ] [ 0 ], WeaponName2, LockerInfo[ id ] [ Price ] [ 1 ], WeaponName3, LockerInfo[ id ] [ Price ] [ 2 ], WeaponName4, LockerInfo[ id ] [ Price ] [ 3 ],
	    			 WeaponName5, LockerInfo[ id ] [ Price ] [ 4 ],  WeaponName6,LockerInfo[ id ] [ Price ] [ 5 ], WeaponName7,LockerInfo[ id ] [ Price ] [ 6 ],
	    		WeaponName8, LockerInfo[ id ] [ Price ] [ 7 ], WeaponName9,LockerInfo[ id ] [ Price ] [ 8 ], WeaponName10, LockerInfo[ id ] [ Price ] [ 9 ] );
	    		ShowPlayerDialog( playerid, DIALOG_LOCKER, DIALOG_STYLE_LIST, "Locker", lockerdialog, "Select", "Cancel" );
	    		return 1;
	    	}
	    	else
	    	{
	    		SendClientMessage(playerid, -1, "NOT AUTHORIZED!");
	    		return 1;
	    	}
    	}
    }
	return 1;
}

CMD:lockerhelp( playerid, params[ ] )
{
	if( PlayerInfo[ playerid ] [ pAdmin ] >= 1337 ) SendClientMessage( playerid, COLOR_GREY, "Locker Help: /locker, /createlocker, /editlocker" );
	else SendClientMessage( playerid, COLOR_GREY, "Locker Help: /locker" );
	return 1;
}

Hook:lck_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_LOCKER)
    {
        if(response)
        {
        	new WeaponName1[100], WeaponName2[100], WeaponName3[100], WeaponName4[100], WeaponName5[100], WeaponName6[100], WeaponName7[100], WeaponName8[100],
			WeaponName9[100], WeaponName10[100], str[500];
			GetWeaponName( LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 0 ], WeaponName1, sizeof( WeaponName1 ) );
    		GetWeaponName( LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 1 ], WeaponName2, sizeof( WeaponName2 ) );
    		GetWeaponName( LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 2 ], WeaponName3, sizeof( WeaponName3 ) );
    		GetWeaponName( LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 3 ], WeaponName4, sizeof( WeaponName4 ) );
    		GetWeaponName( LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 4 ], WeaponName5, sizeof( WeaponName5 ) );
    		GetWeaponName( LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 5 ], WeaponName6, sizeof( WeaponName6 ) );
    		GetWeaponName( LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 6 ], WeaponName7, sizeof( WeaponName7 ) );
    		GetWeaponName( LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 7 ], WeaponName8, sizeof( WeaponName8 ) );
    		GetWeaponName( LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 8 ], WeaponName9, sizeof( WeaponName9 ) );
    		GetWeaponName( LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 9 ], WeaponName10, sizeof( WeaponName10 ) );
    		if( LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 0 ] == 0 ) WeaponName1 = "None";
    		if( LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 1 ] == 0 ) WeaponName2 = "None";
    		if( LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 2 ] == 0 ) WeaponName3 = "None";
    		if( LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 3 ] == 0 ) WeaponName4 = "None";
    		if( LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 4 ] == 0 ) WeaponName5 = "None";
    		if( LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 5 ] == 0 ) WeaponName6 = "None";
    		if( LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 6 ] == 0 ) WeaponName7 = "None";
    		if( LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 7 ] == 0 ) WeaponName8 = "None";
    		if( LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 8 ] == 0 ) WeaponName9 = "None";
    		if( LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 9 ] == 0 ) WeaponName10 = "None";
            switch(listitem)
            {
		            case 0: 
		            {
		            	if( PlayerInfo[ playerid ] [ pCash ] < LockerInfo[ BID[ playerid ] ] [ Price ] [ 0 ] ) SendClientMessage( playerid, COLOR_GREY, "You do not have enough cash!" );
		            	else
		            	{
			            	format( str, sizeof( str ), "You've taken %s from the locker for $%d", WeaponName1, LockerInfo[ BID[ playerid ] ] [ Price ] [ 0 ] );
			            	SendClientMessage( playerid, COLOR_GREY, str );
			                GivePlayerGun(playerid, LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 0 ]);
			                GiveMoney( playerid, -LockerInfo[ BID[ playerid ] ] [ Price ] [ 0 ] );
			                BID[ playerid ] = -1;
			            }
		                return 1;
		            }
		            case 1: 
		            {
		            	if( PlayerInfo[ playerid ] [ pCash ] < LockerInfo[ BID[ playerid ] ] [ Price ] [ 1 ] ) SendClientMessage( playerid, COLOR_GREY, "You do not have enough cash!" );
		            	else
		            	{
			            	format( str, sizeof( str ), "You've taken %s from the locker for $%d", WeaponName2, LockerInfo[ BID[ playerid ] ] [ Price ] [ 1 ] );
			            	SendClientMessage( playerid, COLOR_GREY, str );
			                GivePlayerGun(playerid, LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 1 ]);
			                GiveMoney( playerid, -LockerInfo[ BID[ playerid ] ] [ Price ] [ 1 ] );
			                BID[ playerid ] = -1;
			            }
		                return 1;
		            }
		            case 2:
		            {
		            	if( PlayerInfo[ playerid ] [ pCash ] < LockerInfo[ BID[ playerid ] ] [ Price ] [ 2 ] ) SendClientMessage( playerid, COLOR_GREY, "You do not have enough cash!" );
		            	else
		            	{
			            	format( str, sizeof( str ), "You've taken %s from the locker for $%d", WeaponName3,LockerInfo[ BID[ playerid ] ] [ Price ] [ 2 ] );
			            	SendClientMessage( playerid, COLOR_GREY, str );
			                GivePlayerGun(playerid, LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 2 ]);
			                GiveMoney( playerid, -LockerInfo[ BID[ playerid ] ] [ Price ] [ 2 ] );
			                BID[ playerid ] = -1;
			            }
		                return 1;
		            }
		            case 3:
		            {
		            	if( PlayerInfo[ playerid ] [ pCash ] < LockerInfo[ BID[ playerid ] ] [ Price ] [ 3 ] ) SendClientMessage( playerid, COLOR_GREY, "You do not have enough cash!" );
		            	else
		            	{
			            	format( str, sizeof( str ), "You've taken %s from the locker for $%d", WeaponName4,LockerInfo[ BID[ playerid ] ] [ Price ] [ 3 ] );
			            	SendClientMessage( playerid, COLOR_GREY, str );
			                GivePlayerGun(playerid, LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 3 ]);
			                GiveMoney( playerid, -LockerInfo[ BID[ playerid ] ] [ Price ] [ 3 ] );
			                BID[ playerid ] = -1;
			            }
		                return 1;
		            }
		            case 4:
		            {
		            	if( PlayerInfo[ playerid ] [ pCash ] < LockerInfo[ BID[ playerid ] ] [ Price ] [ 4 ] ) SendClientMessage( playerid, COLOR_GREY, "You do not have enough cash!" );
		            	else
		            	{
			            	format( str, sizeof( str ), "You've taken %s from the locker for $%d", WeaponName5,LockerInfo[ BID[ playerid ] ] [ Price ] [ 4 ] );
			            	SendClientMessage( playerid, COLOR_GREY, str );
			                GivePlayerGun(playerid, LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 4 ]);
			                GiveMoney( playerid, -LockerInfo[ BID[ playerid ] ] [ Price ] [ 4 ] );
			                BID[ playerid ] = -1;
			            }
		                return 1;
		            }
		            case 5:
		            {
		            	if( PlayerInfo[ playerid ] [ pCash ] < LockerInfo[ BID[ playerid ] ] [ Price ] [ 5 ] ) SendClientMessage( playerid, COLOR_GREY, "You do not have enough cash!" );
		            	else
		            	{
			            	format( str, sizeof( str ), "You've taken %s from the locker for $%d", WeaponName6,LockerInfo[ BID[ playerid ] ] [ Price ] [ 5 ] );
			            	SendClientMessage( playerid, COLOR_GREY, str );
			                GivePlayerGun(playerid, LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 5 ]);
			                GiveMoney( playerid, -LockerInfo[ BID[ playerid ] ] [ Price ] [ 5 ] );
			                BID[ playerid ] = -1;
			            }
		                return 1;
		            }
		            case 6:
		            {
		            	if( PlayerInfo[ playerid ] [ pCash ] < LockerInfo[ BID[ playerid ] ] [ Price ] [ 6 ] ) SendClientMessage( playerid, COLOR_GREY, "You do not have enough cash!" );
		            	else
		            	{
			            	format( str, sizeof( str ), "You've taken %s from the locker for $%d", WeaponName7,LockerInfo[ BID[ playerid ] ] [ Price ] [ 6 ] );
			            	SendClientMessage( playerid, COLOR_GREY, str );
			                GivePlayerGun(playerid, LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 6 ]);
			                GiveMoney( playerid, -LockerInfo[ BID[ playerid ] ] [ Price ] [ 6 ] );
			                BID[ playerid ] = -1;
			            }
		                return 1;
		            }
		            case 7:
		            {
		            	if( PlayerInfo[ playerid ] [ pCash ] < LockerInfo[ BID[ playerid ] ] [ Price ] [ 7 ] ) SendClientMessage( playerid, COLOR_GREY, "You do not have enough cash!" );
		            	else
		            	{
			            	format( str, sizeof( str ), "You've taken %s from the locker for $%d", WeaponName8,LockerInfo[ BID[ playerid ] ] [ Price ] [ 7 ] );
			            	SendClientMessage( playerid, COLOR_GREY, str );
			                GivePlayerGun(playerid, LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 7 ]);
			                GiveMoney( playerid, -LockerInfo[ BID[ playerid ] ] [ Price ] [ 7 ] );
			                BID[ playerid ] = -1;
			            }
		                return 1;
		            }
		            case 8:
		            {
		            	if( PlayerInfo[ playerid ] [ pCash ] < LockerInfo[ BID[ playerid ] ] [ Price ] [ 8 ] ) SendClientMessage( playerid, COLOR_GREY, "You do not have enough cash!" );
		            	else
		            	{
			            	format( str, sizeof( str ), "You've taken %s from the locker for $%d", WeaponName9,LockerInfo[ BID[ playerid ] ] [ Price ] [ 8 ] );
			            	SendClientMessage( playerid, COLOR_GREY, str );
			                GivePlayerGun(playerid, LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 8 ]);
			                GiveMoney( playerid, -LockerInfo[ BID[ playerid ] ] [ Price ] [ 8 ] );
			                BID[ playerid ] = -1;
			            }
		                return 1;
		            }
		            case 9: 
		            {
		            	if( PlayerInfo[ playerid ] [ pCash ] < LockerInfo[ BID[ playerid ] ] [ Price ] [ 9 ] ) SendClientMessage( playerid, COLOR_GREY, "You do not have enough cash!" );
		            	else
		            	{
			            	format( str, sizeof( str ), "You've taken %s from the locker for $%d", WeaponName10,LockerInfo[ BID[ playerid ] ] [ Price ] [ 9 ] );
			            	SendClientMessage( playerid, COLOR_GREY, str );
			                GivePlayerGun(playerid, LockerInfo[ BID[ playerid ] ] [ WeaponSlot ] [ 9 ]);
			                GiveMoney( playerid, -LockerInfo[ BID[ playerid ] ] [ Price ] [ 9 ] );
			                BID[ playerid ] = -1;
			            }
		                return 1;
		            }  
            }
        }
        return 1; 
    }
	return 1;
}
Hook:locker_OnPlayerConnect(playerid)
{
	BID[ playerid ] = -1;
	return 1;
}
forward LoadLockers();
public LoadLockers()
{
	new rows = cache_num_rows( );
 	if( rows )
  	{
   		new id, loaded;
		while( loaded < rows )
		{
  			id = cache_get_field_content_int( loaded, "ID" );
		    LockerInfo[ id ] [ LockerX ] = cache_get_field_content_float( loaded, "LockerX" );
		    LockerInfo[ id ] [ LockerY ] = cache_get_field_content_float( loaded, "LockerY" );
		    LockerInfo[ id ] [ LockerZ ] = cache_get_field_content_float( loaded, "LockerZ" );
		    LockerInfo[ id ] [ FactionID ] = cache_get_field_content_int( loaded, "FactionID" );
		    LockerInfo[ id ] [ dGID ] = cache_get_field_content_int( loaded, "DGID" );
		    LockerInfo[ id ] [ GroupID ] = cache_get_field_content_int( loaded, "GroupID" );
		    LockerInfo[ id ] [ WeaponSlot ] [ 0 ] = cache_get_field_content_int( loaded, "Weapon1" );
		    LockerInfo[ id ] [ WeaponSlot ] [ 1 ] = cache_get_field_content_int( loaded, "Weapon2" );
		    LockerInfo[ id ] [ WeaponSlot ] [ 2 ] = cache_get_field_content_int( loaded, "Weapon3" );
		    LockerInfo[ id ] [ WeaponSlot ] [ 3 ] = cache_get_field_content_int( loaded, "Weapon4" );
		    LockerInfo[ id ] [ WeaponSlot ] [ 4 ] = cache_get_field_content_int( loaded, "Weapon5" );
		    LockerInfo[ id ] [ WeaponSlot ] [ 5 ] = cache_get_field_content_int( loaded, "Weapon6" );
		    LockerInfo[ id ] [ WeaponSlot ] [ 6 ] = cache_get_field_content_int( loaded, "Weapon7" );
		    LockerInfo[ id ] [ WeaponSlot ] [ 7 ] = cache_get_field_content_int( loaded, "Weapon8" );
		    LockerInfo[ id ] [ WeaponSlot ] [ 8 ] = cache_get_field_content_int( loaded, "Weapon9" );
		    LockerInfo[ id ] [ WeaponSlot ] [ 9 ] = cache_get_field_content_int( loaded, "Weapon10" );
		    LockerInfo[ id ] [ Price ] [ 0 ] = cache_get_field_content_int( loaded, "WeaponPrice1" );
			LockerInfo[ id ] [ Price ] [ 1 ] = cache_get_field_content_int( loaded, "WeaponPrice2" );
			LockerInfo[ id ] [ Price ] [ 2 ] = cache_get_field_content_int( loaded, "WeaponPrice3" );
		    LockerInfo[ id ] [ Price ] [ 3 ] = cache_get_field_content_int( loaded, "WeaponPrice4" );
			LockerInfo[ id ] [ Price ] [ 4 ] = cache_get_field_content_int( loaded, "WeaponPrice5" );
			LockerInfo[ id ] [ Price ] [ 5 ] = cache_get_field_content_int( loaded, "WeaponPrice6" );
			LockerInfo[ id ] [ Price ] [ 6 ] = cache_get_field_content_int( loaded, "WeaponPrice7" );
			LockerInfo[ id ] [ Price ] [ 7 ] = cache_get_field_content_int( loaded, "WeaponPrice8" );
			LockerInfo[ id ] [ Price ] [ 8 ] = cache_get_field_content_int( loaded, "WeaponPrice9" );
			LockerInfo[ id ] [ Price ] [ 9 ] = cache_get_field_content_int( loaded, "WeaponPrice10" );
		    LockerInfo[ id ] [ LockerPickup ] = CreateDynamicPickup( 1239, 23, LockerInfo[ id ] [ LockerX ], LockerInfo[ id ] [ LockerY ], LockerInfo[ id ] [ LockerZ ] );
			LockerInfo[ id ] [ LockerText ] = CreateDynamic3DTextLabel("Locker", COLOR_HITMAN, LockerInfo[ id ] [ LockerX ], LockerInfo[ id ] [ LockerY ], LockerInfo[ id ] [ LockerZ ], 40.0);
   			Iter_Add( Lockers, id );
   			LockerID [ id ] = id;
		    loaded ++;
	    }
	    printf( "Loaded %d lockers.", loaded );
	}	
	return 1;
}