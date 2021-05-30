new AntiSpamCMDS[MAX_PLAYERS];
CMD:version( playerid, params[] ) {
	if( AntiSpamCMDS[playerid] != 0 && gettime() < ( AntiSpamCMD[playerid] + 3 ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "Please wait 3 seconds before using this command again." );
	}
	AntiSpamCMD[playerid] = gettime();
	mysql_pquery( sqlGameConnection, "SELECT * FROM version;", "fetchVersion", "i", playerid );
	return 1;
}
forward fetchVersion( playerid );
public fetchVersion( playerid ) {
	new verstr[4000], id, admin[MAX_PLAYER_NAME+1], text[128];
	for( new i; i < cache_get_row_count(); i++ ) {
		id = cache_get_field_content_int( i, "ID" );
		cache_get_field_content( i, "developer", admin, sqlGameConnection, 25 );
		cache_get_field_content( i, "text", text, sqlGameConnection, 128 );
		format( verstr, sizeof( verstr ), "%s%d.\t%s (%s)\n", verstr, id, text, admin );
	}
	if( strlen( verstr ) > 1 ) {
		ShowPlayerDialog( playerid, DIALOG_VERSION, DIALOG_STYLE_MSGBOX, "Version {FFFFFF}", verstr, "Okay", "" );
	}
	return 1;
}

CMD:insertversion( playerid, params[] ) {
	if( PlayerInfo[playerid][pAdmin] < 1999999999 || PlayerInfo[playerid][pDev] < 3){
		return 1;
	}

	new text[128];
	if( sscanf( params, "s[128]", text ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF}/insertversion [text]" );
	}

	insertVersion( PlayerOOCName( playerid ), text, playerid );
	return 1;
}

insertVersion( admin[], text[], playerid ) {
	new query[200];
	mysql_format( sqlGameConnection, query, sizeof( query ), "INSERT INTO version SET developer = '%e', text = '%e';", admin, text );
	mysql_pquery( sqlGameConnection, query );
	SendClientMessage( playerid, COLOR_GREY, "The version has been added to the version list." );
	return 1;
}

CMD:deleteversion( playerid, params[] ) {
	if( PlayerInfo[playerid][pAdmin] < 1999999999 || PlayerInfo[playerid][pDev] < 3){
		return 1;
	}

	new id;
	if( sscanf( params, "i", id ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF}/deleteversion [id]" );
	}

	deleteVersion( id, playerid );
	return 1;
}

deleteVersion( id, playerid ) {
	new query[200];
	mysql_format( sqlGameConnection, query, sizeof( query ), "DELETE FROM version WHERE ID = %d;", id );
	mysql_pquery( sqlGameConnection, query );
	SendClientMessage( playerid, COLOR_GREY, "The version has been deleted." );
	return 1;
}

CMD:updateversion( playerid, params[] ) {
	if( PlayerInfo[playerid][pAdmin] < 1999999999 || PlayerInfo[playerid][pDev] < 3){
		return 1;
	}

	new id, text[128];
	if( sscanf( params, "is[128]", id, text ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF}/updateversion [id] [text]" );
	}

	updateVersion( id, PlayerICName( playerid ), text, playerid );
	return 1;
}
updateVersion( id, admin[], text[], playerid ) {
	new query[200];
	mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE version SET developer = '%e', text = '%e' WHERE ID = %d;", admin, text, id );
	mysql_pquery( sqlGameConnection, query );
	SendClientMessage( playerid, COLOR_GREY, "The version has been updated." );
	return 1;
}

CMD:clearversion( playerid, params[] ) {
	if( PlayerInfo[playerid][pAdmin] < 1999999999 || PlayerInfo[playerid][pDev] < 3){
		return 1;
	}

	resetVersion();
	SendClientMessage( playerid, COLOR_GREY, "All versions have been cleared." );
	return 1;
}
resetVersion() {
	mysql_pquery( sqlGameConnection, "TRUNCATE TABLE version;" );
	return 1;
}