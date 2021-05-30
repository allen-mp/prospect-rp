/*
	Definitions
*/
#define PUNISHMENT_KICK		(0)
#define PUNISHMENT_BAN 		(1)
#define PUNISHMENT_PRISON	(2)
#define PUNISHMENT_WARN		(3)

forward LoadPunishments(playerid);

/*
	Functions
*/

public LoadPunishments(playerid)
{
	if(cache_num_rows() == 0) return SendClientMessage(playerid, COLOR_GREY, "No punishments were found.");

	new dialogstring[5000], player, admin[24], punishment, reason[124], date[64]; //length;

	format(dialogstring, 5000, "Date\tAdmin\tPunishment\tReason\n");

	for(new i; i < cache_num_rows(); i++)
	{
		cache_get_field_content(i, "date", date);
		cache_get_field_content(i, "admin", admin);
		cache_get_field_content(i, "reason", reason);

		player 		= cache_get_field_content_int(i, "player");
		punishment 	= cache_get_field_content_int(i, "punishment");
		//length 		= cache_get_field_content_int(i, "length");

		if(punishment == PUNISHMENT_KICK)
		{
			format(dialogstring, sizeof(dialogstring), "%s%s\t%s\tKick\t%s\n", dialogstring, date, admin, reason);
		}
		else if(punishment == PUNISHMENT_BAN)
		{
			format(dialogstring, sizeof(dialogstring), "%s%s\t%s\tBan\t%s\n", dialogstring, date, admin, reason);
		}
		else if(punishment == PUNISHMENT_PRISON)
		{
			format(dialogstring, sizeof(dialogstring), "%s%s\t%s\tPrison\t%s\n", dialogstring, date, admin, reason);
		}
		else if(punishment == PUNISHMENT_WARN)
		{
			format(dialogstring, sizeof(dialogstring), "%s%s\t%s\tWarn\t%s\n", dialogstring, date, admin, reason);
		}
	}
	new DialogHeader[42];
	format(DialogHeader, sizeof(DialogHeader), "%s's admin record", GetNameFomSQLID(player));
	ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_TABLIST_HEADERS, DialogHeader, dialogstring, "Close", "");

	return 1;
}

CMD:adminrecord(playerid, params[])
{
	new adminquery[250], sqlid;
	if(sscanf(params, "i", sqlid))
	{
		mysql_format(sqlGameConnection, adminquery, sizeof adminquery, "SELECT * FROM punishmentlogs WHERE player = %d ORDER BY DATE(date) DESC", PlayerInfo[playerid][pID]);
		mysql_tquery(sqlGameConnection, adminquery, "LoadPunishments", "i", playerid);
	} else {
		mysql_format(sqlGameConnection, adminquery, sizeof adminquery, "SELECT * FROM punishmentlogs WHERE player = %d", sqlid);
		mysql_tquery(sqlGameConnection, adminquery, "LoadPunishments", "i", playerid);
	}
	return 1;
}
