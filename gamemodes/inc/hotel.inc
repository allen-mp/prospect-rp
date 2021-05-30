#define MAX_HOTELS 50

new lastSQLhotelID, ownedHotel, Text3D:HotelText3D[MAX_HOUSES], hotelcount;

enum hotelInfo
{
	hotelID,
	bool: hotelCreated,
	Float: hotelPos[ 3 ],
	Float: hotelOPos[ 3 ],
	hotelSQLID,
	hotelPrice,
	hotelPickup
}
new HotelInfo[MAX_HOTELS][hotelInfo];

CMD:createhotel(playerid, params[])
{
	new price, Float:x, Float:y, Float:z, hotelid, string[50], query[500];
	if(!(PlayerInfo[playerid][pAdmin] > 3)) SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You're not authorized to use this command!");
	else{
		if(sscanf(params, "i", price)) SendClientMessage(playerid, COLOR_GREY, "Usage: /createhotel <Price>");
		else{
			for(new i = 1; i < sizeof(HotelInfo); i++)
			{
				if(HotelInfo[i][hotelCreated] == false)
				{
					hotelid = i;
					break;
				}
			}
			GetPlayerPos(playerid, Float:x, Float:y, Float:z);
			lastSQLhotelID++;
			HotelInfo[hotelid][hotelSQLID] = lastSQLhotelID;
			HotelInfo[hotelid][hotelPos][0] = x;
			HotelInfo[hotelid][hotelPos][1] = y;
			HotelInfo[hotelid][hotelPos][2] = z;
			HotelInfo[hotelid][hotelPrice] = price;
			HotelInfo[hotelid][hotelCreated] = true;
			ownedHotel++;
			format(string, sizeof(string), "\nHotel\nPrice: %d", HotelInfo[hotelid][hotelPrice]);
			HotelText3D[hotelid] = CreateDynamic3DTextLabel(string, COLOR_YELLOW, Float:x, Float:y, Float:z, 50, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1,STREAMER_3D_TEXT_LABEL_SD, -1, 0);
			HotelInfo[hotelid][hotelPickup] = CreateDynamicPickup(1318, 23, Float:x, Float:y, Float:z, -1, -1, -1, 50, -1, 0);
			mysql_format(sqlGameConnection, query, sizeof(query), "INSERT INTO `hotels` (`ID`, `HotelX`, `HotelY`, `HotelZ`, `HotelPrice`) VALUES('%d', '%f', '%f', '%f', '%d')", 
				HotelInfo[hotelid][hotelSQLID], HotelInfo[hotelid][hotelPos][0], HotelInfo[hotelid][hotelPos][1], HotelInfo[hotelid][hotelPos][2], HotelInfo[hotelid][hotelPrice]);
			mysql_pquery(sqlGameConnection, query);
		}
	}
	return 1;
}

stock LoadHotel()
{
	mysql_tquery(sqlGameConnection, "SELECT * FROM `Hotels`", "OnLoadHotels", "");
	return 1;
}

stock GetHotelPool()
{
	new pool, Cache:hotelpool;
	hotelpool = mysql_query(sqlGameConnection, "SELECT ID FROM hotels ORDER BY ID DESC LIMIT 1;");
	pool = cache_get_field_content_int(0, "ID");
	cache_delete(hotelpool);
	return pool;
}

forward OnLoadHotels();
public OnLoadHotels()
{
	new
		value[100], string[50];
		
	for(new id = 1; id < cache_num_rows(); id++)
	{
	    if(!(cache_get_field_content_int(id, "HotelCreated"))) continue;
	    HotelInfo[id][hotelSQLID] = cache_get_field_content_int(id, "ID");

		cache_get_field_content(id, "HotelX", value);
		HotelInfo[id][hotelPos][0] = floatstr(value);
		cache_get_field_content(id, "HotelY", value);
		HotelInfo[id][hotelPos][1] = floatstr(value);
		cache_get_field_content(id, "HotelZ", value);
		HotelInfo[id][hotelPos][2] = floatstr(value);
		cache_get_field_content(id, "IHotelX", value);
		HotelInfo[id][hotelOPos][0] = floatstr(value);
		cache_get_field_content(id, "IHotelY", value);
		HotelInfo[id][hotelOPos][1] = floatstr(value);
		cache_get_field_content(id, "IHotelZ", value);
		HotelInfo[id][hotelOPos][2] = floatstr(value);
		HotelInfo[id][hotelPrice] = cache_get_field_content_int(id, "HotelPrice");
		HotelInfo[id][hotelCreated] = true;
		format(string, sizeof(string), "\nHotel\nPrice: %d", HotelInfo[id][hotelPrice]);
		HotelText3D[id] = CreateDynamic3DTextLabel(string, COLOR_YELLOW, HotelInfo[id][hotelPos][0], HotelInfo[id][hotelPos][1], HotelInfo[id][hotelPos][2], 50, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1,STREAMER_3D_TEXT_LABEL_SD, -1, 0);
		HotelInfo[id][hotelPickup] = CreateDynamicPickup(1318, 23, HotelInfo[id][hotelPos][0], HotelInfo[id][hotelPos][1], HotelInfo[id][hotelPos][2], -1, -1, -1, 50, -1, 0);
		hotelcount++;
	}
	ownedHotel = hotelcount;
	lastSQLhotelID = GetHotelPool();
	return 1;
}