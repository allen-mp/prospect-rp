#define MAX_ATMS 1000

enum ATM
{
	ID,
	Float:Pos[3],
	AtmObj,
	bool: AtmCreated
}

new AtmInfo[MAX_ATMS][ATM];

CMD:createatm(playerid, params[])
{
	new Float:X, Float:Y, Float:Z, query[250], str[250];
	GetPlayerPos(playerid, Float:X, Float:Y, Float:Z);
	for(new i = 1; i < sizeof(AtmInfo); i++)
	{
		mysql_format(sqlGameConnection, query, sizeof(query), "INSERT INTO `atm`(`PosX`, `PosY`, `PosZ`)VALUES('%f', '%f', '%f')", AtmInfo[i][Pos][0], AtmInfo[i][Pos][1], AtmInfo[i][Pos][2]);
		mysql_pquery(sqlGameConnection, query);
		LoadAtms();
		break;
	}
	format(str, sizeof(str), "Successfully created ATM at : %f %f %f", X, Y, Z);
	SendClientMessage(playerid, -1, str);
	return 1;
}

CMD:deleteatm(playerid, params[])
{
	new atmid, query[250];
	if(sscanf(params, "u", atmid)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /deleteatm <atm_id>");
	if(AtmInfo[atmid][AtmCreated] == false) SendClientMessage(playerid, -1, "That ATM does not exist/Is not created.");
	else
	{
		DestroyDynamicObject(AtmInfo[atmid][AtmObj]);
		AtmInfo[atmid][Pos][0] = 0;
		AtmInfo[atmid][Pos][1] = 0;
		AtmInfo[atmid][Pos][2] = 0;
		AtmInfo[atmid][AtmCreated] = false;
		mysql_format(sqlGameConnection, query, sizeof(query), "DELETE FROM `atm` WHERE `ID`='%d'", atmid);
		mysql_pquery(sqlGameConnection, query);
		LoadAtms();
	}
	return 1;
}

CMD:getatmid(playerid, params[])
{
	new str[50];
	for(new i = 1; i < sizeof(AtmInfo); i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 2.0, AtmInfo[i][Pos][0], AtmInfo[i][Pos][1], AtmInfo[i][Pos][2]))
		{
			format(str, sizeof(str), "ATM ID: %d | I: %d", AtmInfo[i][ID], i);
			SendClientMessage(playerid, -1, str);
			return 1;
		}
	}
	return 1;
}

LoadAtms()
{
    mysql_tquery(sqlGameConnection, "SELECT * FROM `atm`", "QueryLoadATM");
    return 1;
}

forward QueryLoadATM();
public QueryLoadATM()
{
    new rows = cache_get_row_count(sqlGameConnection);
    for(new i = 1; i < rows && i < MAX_ATMS; i ++)
    {
    	DestroyDynamicObject(AtmInfo[i][AtmObj]);
    	AtmInfo[i][ID] = cache_get_field_content_int(i, "ID");
        AtmInfo[i][Pos][0] = cache_get_field_content_float(i, "PosX");
        AtmInfo[i][Pos][1] = cache_get_field_content_float(i, "PosY");
        AtmInfo[i][Pos][2] = cache_get_field_content_float(i, "PosZ");
        AtmInfo[i][AtmCreated] = true;
        AtmInfo[i][AtmObj] = CreateDynamicObject(19324, AtmInfo[i][Pos][0], AtmInfo[i][Pos][1], AtmInfo[i][Pos][2], 0, 0, 0, 0, 0, -1, 50, 50, -1, 0);
    }
    return 1;
}