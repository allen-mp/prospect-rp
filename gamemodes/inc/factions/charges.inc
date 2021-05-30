

#define MAX_CHARGES 		(120)
#define MAX_CHARGE_NAME		(70)
#define MAX_CHARGE_TYPES 	(10)
#define MAX_PRISON_TIME		(90)
#define MIN_BAIL_PRICE		(500000) // 500k
#define MAX_BAIL_PRICE		(10000000) // 10m


enum chargesInfo
{
	bool:chargeUsed, 	// active/used or not
	chargeType, 		// type - in what category
	chargeTime,  		// how much time you get in prison for the charge
	chargeName[MAX_CHARGE_NAME], 	// the charge's name

};

new Charge[MAX_CHARGES][chargesInfo];

GetChargeCategoryName(type)
{
	new name[70];
	switch(type)
	{
		case 1: name = "CRIMES AGAINST THE INDIVIDUAL";
		case 2: name = "CRIMES AGAINST PROPERTY";
		case 3: name = "CRIMES AGAINST PUBLIC DECENCY";
		case 4: name = "CRIMES AGAINST PUBLIC HEALTH AND SAFETY";
		case 5: name = "CRIMES AGAINST PUBLIC PEACE";
		case 6: name = "CRIMES AGAINST JUSTICE";
		case 7: name = "VEHICULAR OFFENSES";
		case 8: name = "CONTROL OF LETHAL EQUIPMENT";
		case 9: name = "CRIMES AGAINST THE STATE";
		default: name = "None";
	}
	return name;
}

RegisterNewCharge(type, time, name[])
{
	for(new i = 0; i < sizeof(Charge); i++)
	{
		if(Charge[i][chargeUsed]) continue;

		Charge[i][chargeUsed] = true;
		Charge[i][chargeType] = type;
		Charge[i][chargeTime] = time;
		format(Charge[i][chargeName], MAX_CHARGE_NAME, "%s", name);
		break;
	}
	return 1;
}

GetChargeIDFromName(name[])
{
	for(new i = 0; i < sizeof(Charge); i++)
	{
		if(Charge[i][chargeUsed])
		{
			if(!strcmp(name, Charge[i][chargeName])) return i;
		}
		
	}
	return -1;
}

forward onLoadCharges();
public onLoadCharges() 
{
	new rows = cache_num_rows();
	if(rows != 0) 
	{
		for(new i = 0; i < rows; i++)
		{
			Charge[i][chargeUsed] = true;
			Charge[i][chargeType] = cache_get_field_content_int(i, "Type");
			Charge[i][chargeTime] = cache_get_field_content_int(i, "Time");
			cache_get_field_content(i, "Name", Charge[i][chargeName]);
		}
	}
	return 1;
}

InsertCharge(playerid, type, time, name[]) 
{
	new query[200];
	mysql_format( sqlGameConnection, query, sizeof( query ), "INSERT INTO charges SET `Type`=%d, `Time`=%d, `Name`='%e';", type, time, name);
	mysql_pquery( sqlGameConnection, query );
	SendClientMessage( playerid, -1, "{FF0000}NOTICE:{FFFFFF} The charge has been added to the database successfully.");
	return 1;
}

CMD:addcharge(playerid, params[])
{
	new bool:addchargedisabled = true;
	if(addchargedisabled) return SendClientMessage(playerid, COLOR_GREY, "Disabled ~ Desmond");

	new type, time, name[MAX_CHARGE_NAME];
	new str[128];
	if(sscanf(params, "iis[70]", type, time, name))
	{
		SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} /addcharge [type] [minutes] [name]");
		SendClientMessage(playerid, COLOR_GREY, "Available types:");
		for(new i = 1; i < MAX_CHARGE_TYPES; i++)
		{
			format(str, sizeof(str), "(%d) %s", i, GetChargeCategoryName(i));
			SendClientMessage(playerid, COLOR_GREY, str);
		}
		return 1;
	}
	if(type < 1 || type > MAX_CHARGE_TYPES-1) 
	{
		format(str, sizeof(str), "{FF0000}Error:{FFFFFF} Please enter a type between 1-%d.", MAX_CHARGE_TYPES-1);
		return SendClientMessage(playerid, -1, str);
	}
	if(time < 5 || time > MAX_PRISON_TIME)
	{
		format(str, sizeof(str), "{FF0000}Error:{FFFFFF} Please enter time (minutes) between 5-%d.", MAX_PRISON_TIME);
		return SendClientMessage(playerid, -1, str);
	}
	if(strlen(name) < 5)
	{
		format(str, sizeof(str), "{FF0000}Error:{FFFFFF} Please enter a name between 5-%d.", MAX_CHARGE_NAME);
		return SendClientMessage(playerid, -1, str);
	}	

	RegisterNewCharge(type, time, name);
	InsertCharge(playerid, type, time, name);

	return 1;
}