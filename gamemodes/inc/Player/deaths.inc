/*
  _____  ______       _______ _    _     _______  ______ ______ _    _ _   _ _____                            
 |  __ \|  ____|   /\|__   __| |  | |   / /  __ \|  ____|  ____| |  | | \ | |  __ \                           
 | |  | | |__     /  \  | |  | |__| |  / /| |__) | |__  | |__  | |  | |  \| | |  | |                          
 | |  | |  __|   / /\ \ | |  |  __  | / / |  _  /|  __| |  __| | |  | | . ` | |  | |                          
 | |__| | |____ / ____ \| |  | |  | |/ /  | | \ \| |____| |    | |__| | |\  | |__| |                          
 |_____/|______/_/____\_\_|__|_|__|_/_/_  |_|  \_\______|_|_  __\____/|_|_\_|_____/__  __  ____  _   _ _____  
  / ____\ \   / / ____|__   __|  ____|  \/  | |  _ \ \   / / |  __ \|  ____|/ ____|  \/  |/ __ \| \ | |  __ \ 
 | (___  \ \_/ / (___    | |  | |__  | \  / | | |_) \ \_/ /  | |  | | |__  | (___ | \  / | |  | |  \| | |  | |
  \___ \  \   / \___ \   | |  |  __| | |\/| | |  _ < \   /   | |  | |  __|  \___ \| |\/| | |  | | . ` | |  | |
  ____) |  | |  ____) |  | |  | |____| |  | | | |_) | | |    | |__| | |____ ____) | |  | | |__| | |\  | |__| |
 |_____/   |_| |_____/   |_|  |______|_|  |_| |____/  |_|    |_____/|______|_____/|_|  |_|\____/|_| \_|_____/ 
                                                                                                              
                                                                                                              
                                                                                                              */

#define DIALOG_DEATHS_SHOW 				(14521)
#define DIALOG_DEATHS_ADMINSHOW 		(14522)
#define DIALOG_DEATHS_ADMIN 			(14523)
#define DIALOG_DEATHS_REFUND 			(14524)
#define DIALOG_DEATHS_REFUND_COLLECT 	(14525)

new deathscmdtimer[MAX_PLAYERS], viewingdeathid[MAX_PLAYERS];

forward SaveDeath(playerid, killerid);
public SaveDeath(playerid, killerid)
{
	new weapons[13];	

	weapons[12] = PlayerInfo[playerid][pGun12];
	weapons[11] = PlayerInfo[playerid][pGun11];
	weapons[10] = PlayerInfo[playerid][pGun10];
	weapons[9] = PlayerInfo[playerid][pGun9];
	weapons[8] = PlayerInfo[playerid][pGun8];
	weapons[7] = PlayerInfo[playerid][pGun7];
	weapons[6] = PlayerInfo[playerid][pGun6];
	weapons[5] = PlayerInfo[playerid][pGun5];
	weapons[4] = PlayerInfo[playerid][pGun4];
	weapons[3] = PlayerInfo[playerid][pGun3];
	weapons[2] = PlayerInfo[playerid][pGun2];
	weapons[1] = PlayerInfo[playerid][pGun1];
	weapons[0] = PlayerInfo[playerid][pGun0];

	new Year, Month, Day, datestr[32];
	getdate(Year, Month, Day);
	format(datestr, 32, "%02d/%02d/%d", Month, Day, Year);

	new Float:pX, Float:pY, Float:pZ; // player positions
	GetPlayerPos(playerid, pX, pY, pZ);

	new query[700];

	if(killerid != INVALID_PLAYER_ID)
	{
		new kwepid[24], weapon[24]; // kwepid is a string we're using, weapon is the string we will store the weapon's name in
		if(IsPlayerInAnyVehicle(killerid)) format(kwepid, 24, "%s", GetVehicleModel( GetPlayerVehicleID(killerid) ) );
		else 
		{
			GetWeaponName(GetPlayerWeapon(killerid), weapon, 24);
			format(kwepid, 24, "%s", weapon);
		}	
		new Float:kX, Float:kY, Float:kZ;
		GetPlayerPos(killerid, kX, kY, kZ);
		mysql_format(sqlGameConnection, query, sizeof(query), "INSERT INTO `deaths` (`KilledName`, `KillerName`, `Gun0`, `Gun1`, `Gun2`, `Gun3`, `Gun4`, `Gun5`, `Gun6`, `Gun7`, `Gun8`, `Gun9`, `Gun10`, `Gun11`, `Gun12`, `Date`, `KilledX`, `KilledY`, `KilledZ`, `KillerX`, `KillerY`, `KillerZ`, `Refunded`, `KilledSQL`, `KillerSQL`, `KillerWeapon`, `RefundedBy`) VALUES('%e', '%e', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%e', %f, %f, %f, %f, %f, %f, 0, %d, %d, '%e', 'None')", 
																																																																																			PlayerICName(playerid), PlayerICName(killerid), weapons[0], weapons[1], weapons[2], weapons[3], weapons[4], weapons[5], weapons[6], weapons[7], weapons[8], weapons[9], weapons[10], weapons[11], weapons[12], datestr, pX, pY, pZ, kX, kY, kZ, PlayerInfo[playerid][pID], PlayerInfo[killerid][pID], kwepid);
		mysql_pquery( sqlGameConnection, query, "DeathSQL", "sssffffffsdd", PlayerICName(playerid), PlayerICName(killerid), datestr, pX, pY, pZ, kX, kY, kZ, kwepid, PlayerInfo[playerid][pID], PlayerInfo[killerid][pID]);
	}
	else // if the player wasn't finished by a proper player the data should be "None"
	{
		mysql_format(sqlGameConnection, query, sizeof(query), "INSERT INTO `deaths` (`KilledName`, `KillerName`, `Gun0`, `Gun1`, `Gun2`, `Gun3`, `Gun4`, `Gun5`, `Gun6`, `Gun7`, `Gun8`, `Gun9`, `Gun10`, `Gun11`, `Gun12`, `Date`, `KilledX`, `KilledY`, `KilledZ`, `KillerX`, `KillerY`, `KillerZ`, `Refunded`, `KilledSQL`, `KillerSQL`, `KillerWeapon`, `RefundedBy`) VALUES('%e', 'None', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%e', %f, %f, %f, 0, 0, 0, 0, %d, -1, 'None', 'None')", PlayerICName(playerid), weapons[0], weapons[1], weapons[2], weapons[3], weapons[4], weapons[5], weapons[6], weapons[7], weapons[8], weapons[9], weapons[10], weapons[11], weapons[12], datestr, pX, pY, pZ, PlayerInfo[playerid][pID]);
		mysql_pquery( sqlGameConnection, query, "DeathSQL", "sssffffffsdd", PlayerICName(playerid), "None", datestr, pX, pY, pZ, 0, 0, 0, "None", PlayerInfo[playerid][pID], -1);		
	}

	return 1;
}

forward DeathSQL(pname[24], kname[24], date[32], Float:px, Float:py, Float:pz, Float:kx, Float:ky, Float:kz, weapon[24], psql, ksql); // p = killed player, k = killer, weapon = the weapon the killer used
public DeathSQL(pname[24], kname[24], date[32], Float:px, Float:py, Float:pz, Float:kx, Float:ky, Float:kz, weapon[24], psql, ksql)
{
	printf("[DEATH DEBUG] %s (SQL %d) killed by %s (SQL %d) using a %s, %s, Killer's position: %f, %f, %f, Player's position: %f, %f, %f", pname, psql, kname, ksql, weapon, date, kx, ky, kz, px, py, pz);
	return 1;
}

CMD:mydeaths(playerid, params[])
{
	if(gettime() < deathscmdtimer[playerid] + 5) return SendClientMessage(playerid, COLOR_GREY, "You must wait 5 seconds before using this command again.");

	deathscmdtimer[playerid] = gettime();
	new query[128];
	mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `deaths` WHERE `KilledSQL`=%d ORDER BY `ID` DESC LIMIT 20", PlayerInfo[playerid][pID]);
	mysql_pquery(sqlGameConnection, query, "MyDeathsSQL", "d", playerid);
	return 1;
}

forward MyDeathsSQL(playerid);
public MyDeathsSQL(playerid)
{
	new rows = cache_num_rows();
	if(rows == 0) return SendClientMessage(playerid, COLOR_GREY, "No deaths to display.");

	new deathstr[7000], headers[] = "ID\tKiller\tDate\tRefundedBy\n", ddstr[30];
	for(new i = 0; i < rows; i++)
	{
		new ID, killername[24], date[32], refundedby[24];
		ID = cache_get_field_content_int(i, "ID");
		cache_get_field_content(i, "KillerName", killername);
		cache_get_field_content(i, "Date", date);
		cache_get_field_content(i, "RefundedBy", refundedby);

		format(deathstr, sizeof(deathstr), "%s%d\t%s\t%s\t%s\n", deathstr, ID, killername, date, refundedby);
	}

	format(deathstr, sizeof(deathstr), "%s%s", headers, deathstr);
	format(ddstr, sizeof(ddstr), "My Deaths (SQL: %d)", PlayerInfo[playerid][pID]);
	ShowPlayerDialog(playerid, DIALOG_DEATHS_SHOW, DIALOG_STYLE_TABLIST_HEADERS, ddstr, deathstr, "Select", "Cancel");
	return 1;
}

CMD:deaths(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 2) return AdmErrorMsg;
	if(gettime() < deathscmdtimer[playerid] + 5) return SendClientMessage(playerid, COLOR_GREY, "You must wait 5 seconds before using this command again.");

	deathscmdtimer[playerid] = gettime();

	new query[128];
	mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `deaths` ORDER BY `ID` DESC LIMIT 20");
	mysql_pquery(sqlGameConnection, query, "ADeathsSQL", "d", playerid);

	return 1;
}

forward ADeathsSQL(playerid);
public ADeathsSQL(playerid)
{
	new rows = cache_num_rows();
	if(rows == 0) return SendClientMessage(playerid, COLOR_GREY, "No deaths to display.");

	new deathstr[7000], headers[] = "ID & Killed\tKiller\tDate\tRefundedBy\n";
	for(new i = 0; i < rows; i++)
	{
		new ID, killedname[24], killername[24], date[32], refundedby[24];
		ID = cache_get_field_content_int(i, "ID");
		cache_get_field_content(i, "KilledName", killedname);
		cache_get_field_content(i, "KillerName", killername);
		cache_get_field_content(i, "Date", date);
		cache_get_field_content(i, "RefundedBy", refundedby);

		format(deathstr, sizeof(deathstr), "%s%d | %s\t%s\t%s\t%s\n", deathstr, ID, killedname, killername, date, refundedby);
	}

	format(deathstr, sizeof(deathstr), "%s%s", headers, deathstr);
	ShowPlayerDialog(playerid, DIALOG_DEATHS_ADMINSHOW, DIALOG_STYLE_TABLIST_HEADERS, "Recent 20 deaths", deathstr, "Select", "Cancel");
	return 1;
}

CMD:checkdeath(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 2) return AdmErrorMsg;
	if(gettime() < deathscmdtimer[playerid] + 5) return SendClientMessage(playerid, COLOR_GREY, "You must wait 5 seconds before using this command again.");
	new deathid;
	if(sscanf(params, "d", deathid)) return SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} /checkdeath [deathid]");

	new query[128];
	mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `deaths` WHERE `ID`=%d", deathid);
	mysql_pquery(sqlGameConnection, query, "ACheckDeathsSQL", "dd", playerid, deathid);
	return 1;
}

forward ACheckDeathsSQL(playerid, deathid);
public ACheckDeathsSQL(playerid, deathid)
{
	if(cache_num_rows() == 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid death ID.");
	new ID, killedname[24], killername[24], date[32], refundedby[24], refunded, weapons[13], kweapon[24], bigdstr[1800], sstr[60], weaponslost[200], weaponname[24], killedsql, killersql;

	viewingdeathid[playerid] = deathid;

	ID = cache_get_field_content_int(0, "ID");
	killedsql = cache_get_field_content_int(0, "KilledSQL");
	killersql = cache_get_field_content_int(0, "KillerSQL");
	refunded = cache_get_field_content_int(0, "Refunded");
	cache_get_field_content(0, "KillerWeapon", kweapon);
	cache_get_field_content(0, "KilledName", killedname);
	cache_get_field_content(0, "KillerName", killername);
	cache_get_field_content(0, "Date", date);
	cache_get_field_content(0, "RefundedBy", refundedby);

	weapons[0] = cache_get_field_content_int(0, "Gun0");
	weapons[1] = cache_get_field_content_int(0, "Gun1");
	weapons[2] = cache_get_field_content_int(0, "Gun2");
	weapons[3] = cache_get_field_content_int(0, "Gun3");
	weapons[4] = cache_get_field_content_int(0, "Gun4");
	weapons[5] = cache_get_field_content_int(0, "Gun5");
	weapons[6] = cache_get_field_content_int(0, "Gun6");
	weapons[7] = cache_get_field_content_int(0, "Gun7");
	weapons[8] = cache_get_field_content_int(0, "Gun8");
	weapons[9] = cache_get_field_content_int(0, "Gun9");
	weapons[10] = cache_get_field_content_int(0, "Gun10");
	weapons[11] = cache_get_field_content_int(0, "Gun11");
	weapons[12] = cache_get_field_content_int(0, "Gun12");

	for(new i = 0; i < 13; i++)
	{
		if(weapons[i] == 0) continue;
		GetWeaponName(weapons[i], weaponname, sizeof(weaponname));
		format(weaponslost, sizeof(weaponslost), "%s%s\n", weaponslost, weaponname);
	}

	if(refunded == 2) format(bigdstr, sizeof(bigdstr), "{FF0000}Killed:\t{FFFFFF}%s (SQL %d)\t{FF0000}Killer:\t{FFFFFF}%s (SQL %d)\n\n{FF0000}Killed with a\t{FFFFFF}%s \nNOTE: if this is blank the killer used fists/grenades, or the player finished himself/bled out\n\n{FF0000}Date:\t{FFFFFF}%s\n\n{FF0000}Weapons lost{FFFFFF}\n%s\n\n{FF0000}Refunded by:\t{FFFFFF}%s\n\n{FF0000}Refund Status:\t{33AA33}Collected", killedname, killedsql, killername, killersql, kweapon, date, weaponslost, refundedby);
	else if(refunded == 1) format(bigdstr, sizeof(bigdstr), "{FF0000}Killed:\t{FFFFFF}%s (SQL %d)\t{FF0000}Killer:\t{FFFFFF}%s (SQL %d)\n\n{FF0000}Killed with a\t{FFFFFF}%s \nNOTE: if this is blank the killer used fists/grenades, or the player finished himself/bled out\n\n{FF0000}Date:\t{FFFFFF}%s\n\n{FF0000}Weapons lost{FFFFFF}\n%s\n\n{FF0000}Refunded by:\t{FFFFFF}%s\n\n{FF0000}Refund Status:\t{FFFF00}Approved", killedname, killedsql, killername, killersql, kweapon, date, weaponslost, refundedby);
	else format(bigdstr, sizeof(bigdstr), "{FF0000}Killed:\t{FFFFFF}%s (SQL %d)\t{FF0000}Killer:\t{FFFFFF}%s (SQL %d)\n\n{FF0000}Killed with a\t{FFFFFF}%s \nNOTE: if this is blank the killer used fists/grenades, or the player finished himself/bled out\n\n{FF0000}Date:\t{FFFFFF}%s\n\n{FF0000}Weapons lost{FFFFFF}\n%s\n\n{FF0000}Refunded by:\t{FFFFFF}%s\n\n{FF0000}Refund Status:\t{FFFFFF}None", killedname, killedsql, killername, killersql, kweapon, date, weaponslost, refundedby);
	format(sstr, sizeof(sstr), "Death ID #%d - %s", ID, killedname);
	ShowPlayerDialog(playerid, DIALOG_DEATHS_ADMIN, DIALOG_STYLE_MSGBOX, sstr, bigdstr, "Refund", "Cancel");
	return 1;
}

CMD:myrefunds(playerid, params[])
{
	if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can not use this whilst in a vehicle.");
	if(IsPlayerRestricted(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can not do this right now.");
	if(gettime() < deathscmdtimer[playerid] + 5) return SendClientMessage(playerid, COLOR_GREY, "You must wait 5 seconds before using this command again.");

	deathscmdtimer[playerid] = gettime();
	new query[128];
	mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `deaths` WHERE `KilledSQL`=%d AND `Refunded`=1 ORDER BY `ID` DESC LIMIT 20", PlayerInfo[playerid][pID]);
	mysql_pquery(sqlGameConnection, query, "MyRefundsSQL", "d", playerid);
	return 1;
}

forward MyRefundsSQL(playerid);
public MyRefundsSQL(playerid)
{
	new rows = cache_num_rows();
	if(rows == 0) return SendClientMessage(playerid, COLOR_GREY, "No refunds to display.");

	new deathstr[7000], headers[] = "ID\tKiller\tDate\tRefundedBy\n", ddstr[30];
	for(new i = 0; i < rows; i++)
	{
		new ID, killername[24], date[32], refundedby[24];
		ID = cache_get_field_content_int(i, "ID");
		cache_get_field_content(i, "KillerName", killername);
		cache_get_field_content(i, "Date", date);
		cache_get_field_content(i, "RefundedBy", refundedby);

		format(deathstr, sizeof(deathstr), "%s%d\t%s\t%s\t%s\n", deathstr, ID, killername, date, refundedby);
	}

	format(deathstr, sizeof(deathstr), "%s%s", headers, deathstr);
	format(ddstr, sizeof(ddstr), "My Refunds (SQL: %d)", PlayerInfo[playerid][pID]);
	ShowPlayerDialog(playerid, DIALOG_DEATHS_REFUND, DIALOG_STYLE_TABLIST_HEADERS, ddstr, deathstr, "Select", "Cancel");
	return 1;
}

Hook:deaths_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_DEATHS_SHOW)
	{
		if(response)
		{
			new Cache:result, ID, killedname[24], killername[24], date[32], refundedby[24], refunded, weapons[13], kweapon[24], bigdstr[1800], sstr[60], weaponslost[200], weaponname[24], query[128], killedsql, killersql;
			mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `deaths` WHERE `KilledSQL`=%d ORDER BY `ID` DESC LIMIT 20", PlayerInfo[playerid][pID]);
			result = mysql_query(sqlGameConnection, query);

			ID = cache_get_field_content_int(listitem, "ID");
			killedsql = cache_get_field_content_int(listitem, "KilledSQL");
			killersql = cache_get_field_content_int(listitem, "KillerSQL");
			refunded = cache_get_field_content_int(listitem, "Refunded");
			cache_get_field_content(listitem, "KillerWeapon", kweapon);
			cache_get_field_content(listitem, "KilledName", killedname);
			cache_get_field_content(listitem, "KillerName", killername);
			cache_get_field_content(listitem, "Date", date);
			cache_get_field_content(listitem, "RefundedBy", refundedby);

			weapons[0] = cache_get_field_content_int(listitem, "Gun0");
			weapons[1] = cache_get_field_content_int(listitem, "Gun1");
			weapons[2] = cache_get_field_content_int(listitem, "Gun2");
			weapons[3] = cache_get_field_content_int(listitem, "Gun3");
			weapons[4] = cache_get_field_content_int(listitem, "Gun4");
			weapons[5] = cache_get_field_content_int(listitem, "Gun5");
			weapons[6] = cache_get_field_content_int(listitem, "Gun6");
			weapons[7] = cache_get_field_content_int(listitem, "Gun7");
			weapons[8] = cache_get_field_content_int(listitem, "Gun8");
			weapons[9] = cache_get_field_content_int(listitem, "Gun9");
			weapons[10] = cache_get_field_content_int(listitem, "Gun10");
			weapons[11] = cache_get_field_content_int(listitem, "Gun11");
			weapons[12] = cache_get_field_content_int(listitem, "Gun12");

			for(new i = 0; i < 13; i++)
			{
				if(weapons[i] == 0) continue;
				GetWeaponName(weapons[i], weaponname, sizeof(weaponname));
				format(weaponslost, sizeof(weaponslost), "%s%s\n", weaponslost, weaponname);
			}

			if(refunded == 2) format(bigdstr, sizeof(bigdstr), "{FF0000}Killed:\t{FFFFFF}%s (SQL %d)\t{FF0000}Killer:\t{FFFFFF}%s (SQL %d)\n\n{FF0000}Killed with a\t{FFFFFF}%s \nNOTE: if this is blank the killer used fists/grenades, or the player finished himself/bled out\n\n{FF0000}Date:\t{FFFFFF}%s\n\n{FF0000}Weapons lost{FFFFFF}\n%s\n\n{FF0000}Refunded by:\t{FFFFFF}%s\n\n{FF0000}Refund Status:\t{33AA33}Collected", killedname, killedsql, killername, killersql, kweapon, date, weaponslost, refundedby);
			else if(refunded == 1) format(bigdstr, sizeof(bigdstr), "{FF0000}Killed:\t{FFFFFF}%s (SQL %d)\t{FF0000}Killer:\t{FFFFFF}%s (SQL %d)\n\n{FF0000}Killed with a\t{FFFFFF}%s \nNOTE: if this is blank the killer used fists/grenades, or the player finished himself/bled out\n\n{FF0000}Date:\t{FFFFFF}%s\n\n{FF0000}Weapons lost{FFFFFF}\n%s\n\n{FF0000}Refunded by:\t{FFFFFF}%s\n\n{FF0000}Refund Status:\t{FFFF00}Approved", killedname, killedsql, killername, killersql, kweapon, date, weaponslost, refundedby);
			else format(bigdstr, sizeof(bigdstr), "{FF0000}Killed:\t{FFFFFF}%s (SQL %d)\t{FF0000}Killer:\t{FFFFFF}%s (SQL %d)\n\n{FF0000}Killed with a\t{FFFFFF}%s \nNOTE: if this is blank the killer used fists/grenades, or the player finished himself/bled out\n\n{FF0000}Date:\t{FFFFFF}%s\n\n{FF0000}Weapons lost{FFFFFF}\n%s\n\n{FF0000}Refunded by:\t{FFFFFF}%s\n\n{FF0000}Refund Status:\t{FFFFFF}None", killedname, killedsql, killername, killersql, kweapon, date, weaponslost, refundedby);
			format(sstr, sizeof(sstr), "Death ID #%d - %s", ID, killedname);
			ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, sstr, bigdstr, "Done", "");
			cache_delete(Cache:result, sqlGameConnection);
		}
	}
	if(dialogid == DIALOG_DEATHS_ADMINSHOW)
	{
		if(response)
		{
			new Cache:result, ID, killedname[24], killername[24], date[32], refundedby[24], refunded, weapons[13], kweapon[24], bigdstr[1800], sstr[60], weaponslost[200], weaponname[24], query[128], killedsql, killersql;
			mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `deaths` ORDER BY `ID` DESC LIMIT 20", PlayerInfo[playerid][pID]);
			result = mysql_query(sqlGameConnection, query);

			ID = cache_get_field_content_int(listitem, "ID");
			viewingdeathid[playerid] = ID; // when clicking "refund" this will be used to detect what death the admin is viewing
			killedsql = cache_get_field_content_int(listitem, "KilledSQL");
			killersql = cache_get_field_content_int(listitem, "KillerSQL");
			refunded = cache_get_field_content_int(listitem, "Refunded");
			cache_get_field_content(listitem, "KillerWeapon", kweapon);
			cache_get_field_content(listitem, "KilledName", killedname);
			cache_get_field_content(listitem, "KillerName", killername);
			cache_get_field_content(listitem, "Date", date);
			cache_get_field_content(listitem, "RefundedBy", refundedby);

			weapons[0] = cache_get_field_content_int(listitem, "Gun0");
			weapons[1] = cache_get_field_content_int(listitem, "Gun1");
			weapons[2] = cache_get_field_content_int(listitem, "Gun2");
			weapons[3] = cache_get_field_content_int(listitem, "Gun3");
			weapons[4] = cache_get_field_content_int(listitem, "Gun4");
			weapons[5] = cache_get_field_content_int(listitem, "Gun5");
			weapons[6] = cache_get_field_content_int(listitem, "Gun6");
			weapons[7] = cache_get_field_content_int(listitem, "Gun7");
			weapons[8] = cache_get_field_content_int(listitem, "Gun8");
			weapons[9] = cache_get_field_content_int(listitem, "Gun9");
			weapons[10] = cache_get_field_content_int(listitem, "Gun10");
			weapons[11] = cache_get_field_content_int(listitem, "Gun11");
			weapons[12] = cache_get_field_content_int(listitem, "Gun12");

			for(new i = 0; i < 13; i++)
			{
				if(weapons[i] == 0) continue;
				GetWeaponName(weapons[i], weaponname, sizeof(weaponname));
				format(weaponslost, sizeof(weaponslost), "%s%s\n", weaponslost, weaponname);
			}

			if(refunded == 2) format(bigdstr, sizeof(bigdstr), "{FF0000}Killed:\t{FFFFFF}%s (SQL %d)\t{FF0000}Killer:\t{FFFFFF}%s (SQL %d)\n\n{FF0000}Killed with a\t{FFFFFF}%s \nNOTE: if this is blank the killer used fists/grenades, or the player finished himself/bled out\n\n{FF0000}Date:\t{FFFFFF}%s\n\n{FF0000}Weapons lost{FFFFFF}\n%s\n\n{FF0000}Refunded by:\t{FFFFFF}%s\n\n{FF0000}Refund Status:\t{33AA33}Collected", killedname, killedsql, killername, killersql, kweapon, date, weaponslost, refundedby);
			else if(refunded == 1) format(bigdstr, sizeof(bigdstr), "{FF0000}Killed:\t{FFFFFF}%s (SQL %d)\t{FF0000}Killer:\t{FFFFFF}%s (SQL %d)\n\n{FF0000}Killed with a\t{FFFFFF}%s \nNOTE: if this is blank the killer used fists/grenades, or the player finished himself/bled out\n\n{FF0000}Date:\t{FFFFFF}%s\n\n{FF0000}Weapons lost{FFFFFF}\n%s\n\n{FF0000}Refunded by:\t{FFFFFF}%s\n\n{FF0000}Refund Status:\t{FFFF00}Approved", killedname, killedsql, killername, killersql, kweapon, date, weaponslost, refundedby);
			else format(bigdstr, sizeof(bigdstr), "{FF0000}Killed:\t{FFFFFF}%s (SQL %d)\t{FF0000}Killer:\t{FFFFFF}%s (SQL %d)\n\n{FF0000}Killed with a\t{FFFFFF}%s \nNOTE: if this is blank the killer used fists/grenades, or the player finished himself/bled out\n\n{FF0000}Date:\t{FFFFFF}%s\n\n{FF0000}Weapons lost{FFFFFF}\n%s\n\n{FF0000}Refunded by:\t{FFFFFF}%s\n\n{FF0000}Refund Status:\t{FFFFFF}None", killedname, killedsql, killername, killersql, kweapon, date, weaponslost, refundedby);
			format(sstr, sizeof(sstr), "Death ID #%d - %s", ID, killedname);
			ShowPlayerDialog(playerid, DIALOG_DEATHS_ADMIN, DIALOG_STYLE_MSGBOX, sstr, bigdstr, "Refund", "Cancel");
			cache_delete(Cache:result, sqlGameConnection);
		}
	}
	if(dialogid == DIALOG_DEATHS_ADMIN)
	{
		if(response)
		{
			new Cache:result, query[128], refunded, refundedby[24], dmsg[100];
			mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `deaths` WHERE `ID`=%d", viewingdeathid[playerid]);
			result = mysql_query(sqlGameConnection, query);

			refunded = cache_get_field_content_int(0, "Refunded");
			cache_get_field_content(0, "RefundedBy", refundedby);
			if(refunded == 1 || refunded == 2) // 1 is approved for refund, 2 is approved & collected refund
			{
				format(dmsg, sizeof(dmsg), "Death ID #%d was already refunded by %s.", viewingdeathid[playerid], refundedby);
				SendClientMessage(playerid, COLOR_GREY, dmsg);
				return 1;
			}
			mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `deaths` SET `Refunded`=1, `RefundedBy`='%e' WHERE `ID`=%d", PlayerName(playerid), viewingdeathid[playerid]);
			mysql_query(sqlGameConnection, query);
			format(dmsg, sizeof(dmsg), "You have successfully approved DeathID #%d a refund.", viewingdeathid[playerid]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, dmsg);
			cache_delete(Cache:result, sqlGameConnection);
		}
	}
	if(dialogid == DIALOG_DEATHS_REFUND)
	{
		if(response)
		{
			new Cache:result, ID, killedname[24], killername[24], date[32], refundedby[24], weapons[13], kweapon[24], bigdstr[1800], sstr[60], weaponslost[200], weaponname[24], query[128], killedsql, killersql;
			mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `deaths` WHERE `KilledSQL`=%d AND `Refunded`=1 ORDER BY `ID` DESC LIMIT 20", PlayerInfo[playerid][pID]);
			result = mysql_query(sqlGameConnection, query);

			ID = cache_get_field_content_int(listitem, "ID");
			viewingdeathid[playerid] = ID;
			killedsql = cache_get_field_content_int(listitem, "KilledSQL");
			killersql = cache_get_field_content_int(listitem, "KillerSQL");
			cache_get_field_content(listitem, "KillerWeapon", kweapon);
			cache_get_field_content(listitem, "KilledName", killedname);
			cache_get_field_content(listitem, "KillerName", killername);
			cache_get_field_content(listitem, "Date", date);
			cache_get_field_content(listitem, "RefundedBy", refundedby);

			weapons[0] = cache_get_field_content_int(listitem, "Gun0");
			weapons[1] = cache_get_field_content_int(listitem, "Gun1");
			weapons[2] = cache_get_field_content_int(listitem, "Gun2");
			weapons[3] = cache_get_field_content_int(listitem, "Gun3");
			weapons[4] = cache_get_field_content_int(listitem, "Gun4");
			weapons[5] = cache_get_field_content_int(listitem, "Gun5");
			weapons[6] = cache_get_field_content_int(listitem, "Gun6");
			weapons[7] = cache_get_field_content_int(listitem, "Gun7");
			weapons[8] = cache_get_field_content_int(listitem, "Gun8");
			weapons[9] = cache_get_field_content_int(listitem, "Gun9");
			weapons[10] = cache_get_field_content_int(listitem, "Gun10");
			weapons[11] = cache_get_field_content_int(listitem, "Gun11");
			weapons[12] = cache_get_field_content_int(listitem, "Gun12");

			for(new i = 0; i < 13; i++)
			{
				if(weapons[i] == 0) continue;
				GetWeaponName(weapons[i], weaponname, sizeof(weaponname));
				format(weaponslost, sizeof(weaponslost), "%s%s\n", weaponslost, weaponname);
			}

			format(bigdstr, sizeof(bigdstr), "{FF0000}Killed:\t{FFFFFF}%s (SQL %d)\t{FF0000}Killer:\t{FFFFFF}%s (SQL %d)\n\n{FF0000}Killed with a\t{FFFFFF}%s \nNOTE: if this is blank the killer used fists/grenades, or the player finished himself/bled out\n\n{FF0000}Date:\t{FFFFFF}%s\n\n{FF0000}Weapons lost{FFFFFF}\n%s\n\n{FF0000}Refunded by:\t{FFFFFF}%s\n\n{FF0000}NOTE: Collecting your refund will set your current loadout\nDO NOT use this if you have any weapons on you or you risk losing them!", killedname, killedsql, killername, killersql, kweapon, date, weaponslost, refundedby);
			format(sstr, sizeof(sstr), "Death ID #%d - %s", ID, killedname);
			ShowPlayerDialog(playerid, DIALOG_DEATHS_REFUND_COLLECT, DIALOG_STYLE_MSGBOX, sstr, bigdstr, "Collect", "Cancel");
			cache_delete(Cache:result, sqlGameConnection);
		}
	}
	if(dialogid == DIALOG_DEATHS_REFUND_COLLECT)
	{
		if(response)
		{
			if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can not use this whilst in a vehicle.");
			new Cache:result, weapons[13], deathid = viewingdeathid[playerid], dmsg[100], query[128];
			mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `deaths` WHERE `ID`=%d", deathid);
			result = mysql_query(sqlGameConnection, query);

			weapons[0] = cache_get_field_content_int(0, "Gun0");
			weapons[1] = cache_get_field_content_int(0, "Gun1");
			weapons[2] = cache_get_field_content_int(0, "Gun2");
			weapons[3] = cache_get_field_content_int(0, "Gun3");
			weapons[4] = cache_get_field_content_int(0, "Gun4");
			weapons[5] = cache_get_field_content_int(0, "Gun5");
			weapons[6] = cache_get_field_content_int(0, "Gun6");
			weapons[7] = cache_get_field_content_int(0, "Gun7");
			weapons[8] = cache_get_field_content_int(0, "Gun8");
			weapons[9] = cache_get_field_content_int(0, "Gun9");
			weapons[10] = cache_get_field_content_int(0, "Gun10");
			weapons[11] = cache_get_field_content_int(0, "Gun11");
			weapons[12] = cache_get_field_content_int(0, "Gun12");

			for(new i = 0; i < 13; i++)
			{
				if(weapons[i] == 0) continue;
				GivePlayerGun(playerid, weapons[i]);
			}

			format(dmsg, sizeof(dmsg), "You have successfully collected your weapons from DeathID #%d", deathid);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, dmsg);
			printf("%s has collected his refund for DeathID #%d.", PlayerName(playerid), deathid);

			format(dmsg, sizeof(dmsg), "{FF0000}AdmWarning{FFFFFF}: %s has collected his refund for DeathID #%d.", PlayerName(playerid), deathid);
			SendAdminMessage(COLOR_WHITE, dmsg);

			cache_delete(Cache:result);
			mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `deaths` SET `Refunded`=2 WHERE `ID`=%d", deathid);
			mysql_query(sqlGameConnection, query);
		}
	}
	return 1;
}