// IMPORTANT NOTICE; HouseInfo[houseid][hModified] = 1; enables saving to that house
// set hModified to 1, only when the house is modified and requires saving.
// this is for improved performance.
//============================[ Loading/Saving ]================================

enum _houseInteriorInfo
{
	hInteriorID,
    Float:hX,
    Float:hY,
    Float:hZ,
	Float:hA,
	hName[32]
}

#define MAX_HOUSE_INTERIORS 44

new houseInteriorInfo[MAX_HOUSE_INTERIORS][_houseInteriorInfo] = {
	{0, 1.7592,12.9488,999.4284,89.8906, "Angel Pine Trailer"}, //index 0
	{1, 244.5000, 305.0000, 999.1484, 270.0000, "Small Room 1"},
	{2, 267.1000, 305.0000, 999.1484, 270.0000, "Small Room 2"},
	{10, 2259.3948, -1135.8015, 1050.6403, 270.0000, "Motel Room"},
	{15, 386.8663, 1471.6508, 1080.1949, 86.6616, "Small House 1"},
	{5, 22.7071, 1403.3888, 1084.4370, 359.5774, "Small House 2"},
	{4, 301.3000, 306.3000, 1003.5391, 270.0000, "Wood House"},
	{15, 295.0853, 1472.4978, 1080.2578, 357.6974, "Med House 1"},
	{15, 386.8132, 1471.5809, 1080.1949, 87.9148, "Med House 2"},
	{2, 446.9661, 1397.6705, 1084.3047, 3.1456, "Med House 3"},
	{15, 376.3000, 1417.2000, 1081.3281, 84.5970, "Rich House 1"},
	{15, 295.0820, 1472.5895, 1080.2578, 357.9638, "Rich House 2"},
	{6, 234.1893, 1064.2260, 1084.2115, 357.8933, "Rich House 3"},
	{7, 225.7264, 1021.7980, 1084.0167, 0.4233, "Rich House 4"},
	{12, 2324.4729, -1148.9298, 1050.7101, 359.4833, "Rich House 5"},
	{3, 235.2827, 1187.0928, 1080.2578, 358.8566, "Rich House 6"},
	{6, 344.4200, 304.9100, 999.1484, 280.0000, "BDSM Lair"},
	{5, 1226.5758, -812.2072, 1084.0078, 0.0000, "Office"},
	{6, 0744.4441, 1437.6198, 1102.7031, 360.0000, "Stripclub"},
	{3, 963.4101, -53.1195, 1001.1246, 87.6137, "Brothel"},
	{2, 2549.1807, -1294.7899, 1060.9844, 262.7453, "Big Smokes"},
	{5, 226.54, 1114.31, 1080.99, 270.0000, "VIP House 1"},
	{5, 140.26, 1366.78, 1083.86, 0.0000, "VIP House 2"},
	{10, 23.9781, 1340.4080, 1084.3750, 357.0715, "New House"},
	{4, -260.9349,1456.6975,1084.3672,86.3724, "New House"},
	{9, 83.0348,1322.7543,1083.8662,359.8916, "New House"},
	{9, 2317.8899,-1026.7600,1050.2178,359.8916, "New House"},
	{3, 2495.9409,-1692.5228,1014.7422,178.8066, "New House"},
	{8, 2807.6604,-1174.4706,1025.5703,358.0116, "New House"},
	{6, 2196.5779,-1204.4008,1049.0234,93.5792, "New House"},
	{10, 2270.1311,-1210.4797,1047.5625,89.1925, "New House"},
	{8, 2365.2231,-1135.3877,1050.8750,0.5182, "New House"},
	{2, 2237.5620,-1081.2277,1049.0234,358.0116, "New House"},
	{4, 261.0469,1284.5985,1080.2578,358.9516, "New House"},
	{4, 221.8893,1140.6204,1082.6094,358.0116, "New House"},
	{6, -68.8100,1351.2100,1080.2109,358.0116, "New House"},
	{9, 260.7921,1237.5183,1084.2578,359.5782, "New House"},
	{2, 2468.5461,-1698.2461,1013.5078,87.9392, "New House"},
	{1, 223.1488,1287.3376,1082.1406,359.2650, "New House"},
	{11, 2282.9709,-1140.0293,1050.8984,357.3849, "New House"},
	{15, 327.9431,1477.9525,1084.4375,359.5781, "New House"},
	{8, -42.5900,1405.4700,1084.4297,359.5781, "New House"},
	{6, 2308.7700,-1212.9399,1049.0234,359.5781, "New House"},
	{1, 2524.4788,-1679.4126,1015.4986,268.7106, "New House"}
};

new HouseFiresRemaining[MAX_HOUSES]; //fires remaining at house, fire system
stock GetHousePool()
{
	new pool, Cache:housesPool;
	housesPool = mysql_query(sqlGameConnection, "SELECT HouseID FROM houses ORDER BY HouseID DESC LIMIT 1;");
	pool = cache_get_field_content_int(0, "HouseID");
	cache_delete(housesPool);
	return pool;
}
stock LoadOwnedHouses()
{
	mysql_tquery(sqlGameConnection, "SELECT houses.*, players.Name FROM houses INNER JOIN players ON houses.OwnerID = players.ID", "OnLoadOwnedHouses", "");
	printf("mysql_tquery OnLoadOwnedHouses");
	return 1;
}

new
	ownedHousesPool = 0;
	
//static str[128], tmp[128]; //this is already defined in the 
forward OnLoadOwnedHouses();
public OnLoadOwnedHouses()
{
	new
		value[100], houseCount = 0;
		
	for(new id = 1; id < cache_num_rows(); id++)
	{
	    if(id == MAX_HOUSES)
			return print("[HouseInfo] WARNING: maximum houses reached.");
	    //important variables/general variables
	    if(!(cache_get_field_content_int(id, "Created"))) continue;
	    HouseInfo[id][hSQLID] = cache_get_field_content_int(id, "HouseID");
        HouseInfo[id][hOwnerID] = cache_get_field_content_int(id, "OwnerID");
        cache_get_field_content(id, "Name", value);
        format(HouseInfo[id][hOwner], MAX_PLAYER_NAME + 1, "%s", value);

		HouseInfo[id][hOwned] = cache_get_field_content_int(id, "Owned");
		HouseInfo[id][hValue] = cache_get_field_content_int(id, "Value");

		cache_get_field_content(id, "Tag", value);
		strmid(HouseInfo[id][hTag], value, 0, strlen(value), 255);
		HouseInfo[id][hLevel] = cache_get_field_content_int(id, "Level");

		//icon location variables
		cache_get_field_content(id, "OutsideX", value);
		HouseInfo[id][hOutsideX] = floatstr(value);
		cache_get_field_content(id, "OutsideY", value);
		HouseInfo[id][hOutsideY] = floatstr(value);
		cache_get_field_content(id, "OutsideZ", value);
		HouseInfo[id][hOutsideZ] = floatstr(value);
		cache_get_field_content(id, "OutsideA", value);
		HouseInfo[id][hOutsideA] = floatstr(value);
		cache_get_field_content(id, "InsideX", value);
		HouseInfo[id][hInsideX] = floatstr(value);
		cache_get_field_content(id, "InsideY", value);
		HouseInfo[id][hInsideY] = floatstr(value);
		cache_get_field_content(id, "InsideZ", value);
		HouseInfo[id][hInsideZ] = floatstr(value);
		cache_get_field_content(id, "InsideA", value);
		HouseInfo[id][hInsideA] = floatstr(value);
		HouseInfo[id][hInterior] = cache_get_field_content_int(id, "Interior");

		//upgrades/storage
		HouseInfo[id][hLocked] = cache_get_field_content_int(id, "Locked");
		HouseInfo[id][hAlarm] = cache_get_field_content_int(id, "Alarm");
		HouseInfo[id][hAlarmStatus] = cache_get_field_content_int(id, "AlarmStatus");
		HouseInfo[id][hCash] = cache_get_field_content_int(id, "Cash");
		HouseInfo[id][hMaterials] = cache_get_field_content_int(id, "Materials");
		HouseInfo[id][hPot] = cache_get_field_content_int(id, "Pot");
		HouseInfo[id][hCrack] = cache_get_field_content_int(id, "Crack");
		HouseInfo[id][hGun1] = cache_get_field_content_int(id, "Gun1");
		HouseInfo[id][hGun2] = cache_get_field_content_int(id, "Gun2");
		HouseInfo[id][hGun3] = cache_get_field_content_int(id, "Gun3");
		HouseInfo[id][hGun4] = cache_get_field_content_int(id, "Gun4");
		HouseInfo[id][hModified] = 0;

		HouseInfo[id][hOutsidePickup] = CreateDynamicPickup(HOUSE_OFFLINE, 1, HouseInfo[id][hOutsideX], HouseInfo[id][hOutsideY], HouseInfo[id][hOutsideZ]);

		HouseInfoText3D[id] = CreateDynamic3DTextLabel("", COLOR_HOUSETEXT, HouseInfo[id][hOutsideX], HouseInfo[id][hOutsideY], HouseInfo[id][hOutsideZ] +0.88, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
		SetHouseTag(id);
		HouseInfo[id][hCreated] = 1;
		houseCount++;
	}
	ownedHousesPool = houseCount;
	lastSQLhouseID = GetHousePool();
	printf("[HouseInfo] %d owned houses were loaded.", ownedHousesPool);
    mysql_tquery(sqlGameConnection, "SELECT * FROM houses WHERE OwnerID = 0", "OnLoadUnownedHouses", "");
	printf("mysql_tquery OnLoadUnownedHouses");
	return 1;
}

forward OnLoadUnownedHouses();
public OnLoadUnownedHouses()
{
	new
		value[100], houseCount = 0;

	for(new id = ownedHousesPool + 1; id < cache_num_rows() + ownedHousesPool; id++) //starts at 1
	{
	    if(id == MAX_HOUSES)
			return print("[HouseInfo] WARNING: maximum houses reached.");
	    //important variables/general variables
	    if(!(cache_get_field_content_int(id, "Created"))) continue;
	    HouseInfo[id][hSQLID] = cache_get_field_content_int(id, "HouseID");
        HouseInfo[id][hOwnerID] = 0;

		HouseInfo[id][hOwned] = 0;
		HouseInfo[id][hValue] = cache_get_field_content_int(id, "Value");

		cache_get_field_content(id, "Tag", value);
		strmid(HouseInfo[id][hTag], value, 0, strlen(value), 255);
		HouseInfo[id][hLevel] = cache_get_field_content_int(id, "Level");

		//icon location variables
		cache_get_field_content(id, "OutsideX", value);
		HouseInfo[id][hOutsideX] = floatstr(value);
		cache_get_field_content(id, "OutsideY", value);
		HouseInfo[id][hOutsideY] = floatstr(value);
		cache_get_field_content(id, "OutsideZ", value);
		HouseInfo[id][hOutsideZ] = floatstr(value);
		cache_get_field_content(id, "OutsideA", value);
		HouseInfo[id][hOutsideA] = floatstr(value);
		cache_get_field_content(id, "InsideX", value);
		HouseInfo[id][hInsideX] = floatstr(value);
		cache_get_field_content(id, "InsideY", value);
		HouseInfo[id][hInsideY] = floatstr(value);
		cache_get_field_content(id, "InsideZ", value);
		HouseInfo[id][hInsideZ] = floatstr(value);
		cache_get_field_content(id, "InsideA", value);
		HouseInfo[id][hInsideA] = floatstr(value);
		HouseInfo[id][hInterior] = cache_get_field_content_int(id, "Interior");
		HouseInfo[id][hModified] = 0;

		HouseInfo[id][hOutsidePickup] = CreateDynamicPickup(HOUSE_UNOWNED, 1, HouseInfo[id][hOutsideX], HouseInfo[id][hOutsideY], HouseInfo[id][hOutsideZ]);

		HouseInfoText3D[id] = CreateDynamic3DTextLabel("", COLOR_HOUSETEXT, HouseInfo[id][hOutsideX], HouseInfo[id][hOutsideY], HouseInfo[id][hOutsideZ] +0.88, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
		SetHouseTag(id);
		HouseInfo[id][hCreated] = 1;
		houseCount++;
	}
	printf("[HouseInfo] %d unowned houses were loaded. The last SQL ID in the table is; %d.", houseCount, lastSQLhouseID);
	return 1;
}
public SaveHouses()
{
	new
	    houseCount = 0;
	for(new id = 1; id < sizeof(HouseInfo); id++)
	{
	    if(!HouseInfo[id][hCreated]) continue;
        if(!HouseInfo[id][hModified]) continue;
		new query[2000];
		mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `houses` SET `OwnerID` = %d, `Created` = %d, `Owned` = %d,",
		HouseInfo[id][hOwnerID], HouseInfo[id][hCreated], HouseInfo[id][hOwned]);

		mysql_format(sqlGameConnection, query, sizeof(query), "%s `Value` = %d, `OutsideX` = %f, `OutsideY` = %f, `OutsideZ` = %f,",
		query, HouseInfo[id][hValue], HouseInfo[id][hOutsideX], HouseInfo[id][hOutsideY], HouseInfo[id][hOutsideZ]);

		mysql_format(sqlGameConnection, query, sizeof(query), "%s `OutsideA` = %f, `InsideX` = %f, `InsideY` = %f, `InsideZ` = %f,",
		query, HouseInfo[id][hOutsideA], HouseInfo[id][hInsideX], HouseInfo[id][hInsideY], HouseInfo[id][hInsideZ]);

		mysql_format(sqlGameConnection, query, sizeof(query), "%s `InsideA` = %f, `Interior` = %d, `Locked` = %d, `Alarm` = %d,",
		query, HouseInfo[id][hInsideA], HouseInfo[id][hInterior], HouseInfo[id][hLocked], HouseInfo[id][hAlarm]);

		mysql_format(sqlGameConnection, query, sizeof(query), "%s `AlarmStatus` = %d, `Cash` = %d, `Materials` = %d, `Pot` = %d,",
		query, HouseInfo[id][hAlarmStatus], HouseInfo[id][hCash], HouseInfo[id][hMaterials], HouseInfo[id][hPot]);

		mysql_format(sqlGameConnection, query, sizeof(query), "%s `Crack` = %d, `Gun1` = %d, `Gun2` = %d, `Gun3` = %d,",
		query, HouseInfo[id][hCrack], HouseInfo[id][hGun1], HouseInfo[id][hGun2], HouseInfo[id][hGun3]);

		mysql_format(sqlGameConnection, query, sizeof(query), "%s `Gun4` = '%d', `Tag` = '%e', `Level` = %d WHERE `HouseID` = %d",
		query, HouseInfo[id][hGun4], HouseInfo[id][hTag], HouseInfo[id][hLevel], HouseInfo[id][hSQLID]);
		mysql_tquery(sqlGameConnection, query);
		HouseInfo[id][hModified] = 0;
		houseCount++;
	}
	printf("[HouseInfo] %d houses were saved.", houseCount);
	return 1;
}
//===========================[ Useful functions ]===============================
stock SetHouseTag(hid)
{
	new
		newhousetag[180], location[60];
	Get2DZone(location, MAX_ZONE_NAME, HouseInfo[hid][hOutsideX], HouseInfo[hid][hOutsideY], HouseInfo[hid][hOutsideZ]);
	if(HouseInfo[hid][hOwned] == 1)
	{
		format(newhousetag, sizeof(newhousetag), "{5BC435}%s{FFFFFF}\nOwner: %s\nAddress: %d %s\nPost code: %d\nType /enter to go inside.", HouseInfo[hid][hTag], HouseInfo[hid][hOwner], hid, location, HouseInfo[hid][hSQLID]);
	}
	else
	{
		format(newhousetag, sizeof(newhousetag), "{5BC435}This property is for sale!{FFFFFF}\nAddress: %d %s\nPost code: %d\nType /buyhouse to buy it for $%d", hid, location, HouseInfo[hid][hSQLID], HouseInfo[hid][hValue]);
	}
	UpdateDynamic3DTextLabelText(HouseInfoText3D[hid], COLOR_HOUSETEXT, newhousetag);
	return 1;
}
stock GetHouseOwnerIDFromSQLID(sqlid)
{
	for(new id = 1; id < MAX_HOUSES; id++)
 	{
   		if(HouseInfo[id][hOwnerID] == sqlid)
  		{
  		    print("house ID obtained.");
      		return id;
		}
 	}
	return 0;
}
stock GetHouseIDFromSQLID(sqlid)
{
	for(new id = 0; id < MAX_HOUSES; id++)
 	{
   		if(HouseInfo[id][hSQLID] == sqlid)
  		{
      		return id;
		}
 	}
	return 0;
}
stock GetHouseOwner(sqlid, playerid){
	new query[128];
	mysql_format(sqlGameConnection, query, sizeof(query), "SELECT `HouseID` FROM `houses` WHERE `OwnerID`=%d", sqlid);
	mysql_tquery(sqlGameConnection, query, "OnGetHouseOwner", "i", playerid);
	return 1;
}
forward OnGetHouseOwner(playerid);
public OnGetHouseOwner(playerid)
{
	new ID;
	if(cache_num_rows() != 0)
	{
		ID = cache_get_field_content_int(0, "HouseID");
	}
	else
		ID = 0;

	PlayerInfo[playerid][pHouseID] = ID;
}
stock GetPlayerNearHouseID(playerid)
{
    for(new id = 1; id < sizeof(HouseInfo); id++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 1.0, HouseInfo[id][hOutsideX], HouseInfo[id][hOutsideY], HouseInfo[id][hOutsideZ]))
		{
		    return id;
		}
	}
	return 0;
}
stock GetHouseLocation(houseid)
{
	new location[MAX_ZONE_NAME];
	Get2DZone(location, MAX_ZONE_NAME, HouseInfo[houseid][hOutsideX], HouseInfo[houseid][hOutsideY], HouseInfo[houseid][hOutsideZ]);
	return location;
}
stock DeleteHouse(houseid)
{
    DestroyDynamic3DTextLabel(HouseInfoText3D[houseid]);
	HouseInfoText3D[houseid] = Text3D:INVALID_3DTEXT_ID;
	HouseInfo[houseid][hCreated] = 0;
	HouseInfo[houseid][hOwned] = 0;
	HouseInfo[houseid][hValue] = 0;
	HouseInfo[houseid][hOutsideX] = 0;
	HouseInfo[houseid][hOutsideY] = 0;
	HouseInfo[houseid][hOutsideZ] = 0;
	HouseInfo[houseid][hOutsideA] = 0;
	HouseInfo[houseid][hInsideX] = 0;
	HouseInfo[houseid][hInsideY] = 0;
	HouseInfo[houseid][hInsideZ] = 0;
	HouseInfo[houseid][hInsideA] = 0;
	HouseInfo[houseid][hInterior] = 0;
	HouseInfo[houseid][hLocked] = 0;
	HouseInfo[houseid][hAlarm] = 0;
	HouseInfo[houseid][hAlarmStatus] = 0;
	HouseInfo[houseid][hCash] = 0;
	HouseInfo[houseid][hMaterials] = 0;
	HouseInfo[houseid][hPot] = 0;
	HouseInfo[houseid][hCrack] = 0;
	HouseInfo[houseid][hGun1] = 0;
	HouseInfo[houseid][hGun2] = 0;
	HouseInfo[houseid][hGun3] = 0;
	HouseInfo[houseid][hGun4] = 0;
	HouseInfo[houseid][hOwnerID] = -1;
	strdel(HouseInfo[houseid][hTag], 0, strlen(HouseInfo[houseid][hTag]));
	strins(HouseInfo[houseid][hTag], "House", 0);
	HouseInfo[houseid][hLevel] = 0;
	if(HouseInfo[houseid][hOutsidePickup])
	{
		DestroyDynamicPickup(HouseInfo[houseid][hOutsidePickup]);
	}
	HouseInfo[houseid][hSQLID] = 0;
	return 1;
}
//==============================[ Admin commands ]==============================

CMD:cleanhouses(playerid, params[]) //data cleanups
{
	if(PlayerInfo[playerid][pAdmin] < 99999)
        return AdmErrorMsg;

	new confirm[128];
	if(sscanf(params, "s[128]", confirm))
	{
	    if(strcmp(confirm, "confirm", true) == 0)
	    {
		    SendClientMessage(playerid, COLOR_LIGHTBLUE, "This action will delete all houses at coordinates 0.0, 0.0, 0.0 and houses not created.");
			return SendClientMessage(playerid, COLOR_GREY, "Type /cleanhouses confirm to clean up the houses table.");
		}
	}
	for(new id = 1; id < MAX_HOUSES; id++)
	{
	    if(HouseInfo[id][hOutsideX] == 0.0 && HouseInfo[id][hOutsideY] == 0.0)
	    {
	        DeleteHouse(id);
	    }
	}
	mysql_tquery(sqlGameConnection, "DELETE FROM houses WHERE OutsideX = 0.0 AND OutsideY = 0.0", "", "");
	mysql_tquery(sqlGameConnection, "DELETE FROM houses WHERE Created = 0", "", "");
	SendClientMessage(playerid, COLOR_GREY, "You have cleaned out the houses table.");
	return 1;
}
CMD:createhouse(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 99999 || PlayerInfo[playerid][pBizMod] == 1)
	{
		new price, interiorid, houseid, Float:X, Float:Y, Float:Z, Float:A, check = 0;
		if(sscanf(params, "dd", price, interiorid))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /createhouse [price] [interior id]");
			format(str, sizeof(str), "{00BFFF}Usage:{FFFFFF} /viewhouseint [0-%d] or /gotoint", MAX_HOUSE_INTERIORS);
			SendClientMessage(playerid, COLOR_GRAD2, str);
			return 1;
		}
		GetPlayerPos(playerid, X, Y, Z);
		GetPlayerFacingAngle(playerid, A);
		for(new i = 1; i < sizeof(HouseInfo); i++)
		{
			if(HouseInfo[i][hCreated] == 0)
			{
				check = 1;
				houseid = i;
				break;
			}
		}
		if(!check) return SendClientMessage(playerid, COLOR_GRAD2, "1000/1000 houses have already been created.");
		lastSQLhouseID++;
		HouseInfo[houseid][hSQLID] = lastSQLhouseID;
		HouseInfo[houseid][hOutsideX] = X;
		HouseInfo[houseid][hOutsideY] = Y;
		HouseInfo[houseid][hOutsideZ] = Z;
		HouseInfo[houseid][hOutsideA] = A;
		HouseInfo[houseid][hCreated] = 1;
		HouseInfo[houseid][hValue] = price;
		strdel(HouseInfo[houseid][hTag], 0, strlen(HouseInfo[houseid][hTag]));
		strins(HouseInfo[houseid][hTag], "House", 0);
		SetHouseInterior(houseid, interiorid);
		ownedHousesPool++;
		if(HouseInfo[houseid][hOutsidePickup])
		{
			DestroyDynamicPickup(HouseInfo[houseid][hOutsidePickup]);
		}
		HouseInfo[houseid][hOutsidePickup] = CreateDynamicPickup(1273, 1, HouseInfo[houseid][hOutsideX], HouseInfo[houseid][hOutsideY], HouseInfo[houseid][hOutsideZ]);

		if(HouseInfoText3D[houseid])
		{
			DestroyDynamic3DTextLabel(HouseInfoText3D[houseid]);
		}
		HouseInfoText3D[houseid] = CreateDynamic3DTextLabel("", COLOR_HOUSETEXT, HouseInfo[houseid][hOutsideX], HouseInfo[houseid][hOutsideY], HouseInfo[houseid][hOutsideZ] +0.88, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
		SetHouseTag(houseid);
		format(str, sizeof(str), "You have created houseid %d with the price of $%d and interiorid %d", houseid, price, interiorid);
		SendClientMessage(playerid, COLOR_GRAD1, str);

		new query[500];
		format(query, sizeof(query), "INSERT INTO `houses` (`OwnerID`, `Created`, `Owned`, `Value`, `OutsideX`, `OutsideY`, `OutsideZ`, `OutsideA`, `InsideX`, `InsideY`, `InsideZ`, `InsideA`, `Interior`, `Locked`, `Alarm`, `AlarmStatus`, `Cash`, `Materials`, `Pot`, `Crack`, `Gun1`, `Gun2`, `Gun3`, `Gun4`, `Tag`, `Level`)");
		format(query, sizeof(query), "%s VALUES(-1, 1, 0, %d, %f, %f, %f, %f, %f, %f, %f, %f, %d, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'House', 2)", query, price, X, Y, Z, A, HouseInfo[houseid][hInsideX], HouseInfo[houseid][hInsideY], HouseInfo[houseid][hInsideZ], HouseInfo[houseid][hInsideA], HouseInfo[houseid][hInterior]);
		mysql_tquery(sqlGameConnection, query, "sqlAddHouse", "i", houseid);
	}
	else return AdmErrorMsg;
	return 1;
}

CMD:deletehouse(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 4 || PlayerInfo[playerid][pBizMod] == 1)
	{
		new houseid;
		if(sscanf(params, "ds[128]", houseid, str))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /deletehouse [houseid] [confirm]");
			return 1;
		}
		if(HouseInfo[houseid][hCreated] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That house has not even been created yet.");
		if(strcmp(str, "confirm", true) == 0)
		{
			foreach(Player, i)
			{
				if(IsPlayerConnected(i))
				{
					if(PlayerInfo[i][pHouseID] == houseid)
					{
						SendClientMessage(i, COLOR_LIGHTBLUE, "* Your house has been deleted by an admin, you are no longer the owner.");
						PlayerInfo[i][pHouseID] = 0;
					}
				}
			}
			new query[100];
			format(query, sizeof(query), "DELETE FROM `houses` WHERE `HouseID` = %d", HouseInfo[houseid][hSQLID]);
			mysql_tquery(sqlGameConnection, query);
			format(str, sizeof(str), "You have fully deleted houseid %d.", houseid);
			SendClientMessage(playerid, COLOR_GRAD1, str);
			DeleteHouse(houseid);
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /deletehouse [houseid] [confirm]");
			format(str, sizeof(str), "Are you sure you want to delete house id %d?", houseid);
			SendClientMessage(playerid, COLOR_GRAD2, str);
			return 1;
		}
	}
	else return AdmErrorMsg;
	return 1;
}

CMD:resethouse(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 4 || PlayerInfo[playerid][pBizMod] == 1)
	{
		new houseid;
		if(sscanf(params, "ds[128]", houseid, str))return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /resethouse [houseid] [confirm]");
		if(HouseInfo[houseid][hCreated] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That house has not even been created yet! ");
		if(strcmp(str, "confirm", true) == 0)
		{
			foreach(Player, i)
			{
				if(IsPlayerConnected(i))
				{
					if(PlayerInfo[i][pHouseID] == houseid)
					{
						SendClientMessage(i, COLOR_LIGHTBLUE, "* Your house has been reset by an admin, you are no longer the owner.");
						PlayerInfo[i][pHouseID] = 0;
					}
				}
			}
			HouseInfo[houseid][hOwned] = 0;
			HouseInfo[houseid][hLocked] = 0;
			HouseInfo[houseid][hCash] = 0;
			HouseInfo[houseid][hMaterials] = 0;
			HouseInfo[houseid][hPot] = 0;
			HouseInfo[houseid][hCrack] = 0;
			HouseInfo[houseid][hAlarm] = 0;
			HouseInfo[houseid][hAlarmStatus] = 0;
			HouseInfo[houseid][hGun1] = 0;
			HouseInfo[houseid][hGun2] = 0;
			HouseInfo[houseid][hGun3] = 0;
			HouseInfo[houseid][hGun4] = 0;
			HouseInfo[houseid][hOwnerID] = -1;

			strdel(HouseInfo[houseid][hTag], 0, strlen(HouseInfo[houseid][hTag]));
			strins(HouseInfo[houseid][hTag], "House", 0);
			SetHouseTag(houseid);
			format(str, sizeof(str), "You have reset houseid %d.", houseid);
			SendClientMessage(playerid, COLOR_GRAD1, str);
			HouseInfo[houseid][hModified] = 1;
			return 1;
		}
	}
	else return AdmErrorMsg;
	return 1;
}

CMD:gotohouse(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 4 || PlayerInfo[playerid][pBizMod] == 1)
	{
		new houseid;
		if(sscanf(params, "d", houseid)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /gotohouse [id]");
		if(HouseInfo[houseid][hCreated] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That house hasn't been created yet.");
		if(GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
		{
			SendClientMessage(playerid, COLOR_GREY, "You can not do that while spectating.");
			return 1;
		}
		SetPlayerPos(playerid, HouseInfo[houseid][hOutsideX], HouseInfo[houseid][hOutsideY], HouseInfo[houseid][hOutsideZ]);
		SetPlayerInterior(playerid, 0);
		PlayerInfo[playerid][pInt] = 0;
		SetPlayerVirtualWorld(playerid, 0);
		PlayerInfo[playerid][pVirtualWorld] = 0;
		format(str, sizeof(str), "~w~Teleporting to ~b~\"%s\"~w~.", HouseInfo[houseid][hTag]);
		displayCenterHUDInfo(playerid,  str, 8); //display for 8 seconds
	}
	else
	{
		AdmErrorMsg;
	}
	return 1;
}
CMD:gotohousesqlid(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 4 || PlayerInfo[playerid][pBizMod] == 1)
	{
		new houseid;
		if(sscanf(params, "d", houseid))
			return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /gotohousesqlid [post code/SQL ID]");
			
		houseid = GetHouseIDFromSQLID(houseid);
		
		if(HouseInfo[houseid][hCreated] == 0)
			return SendClientMessage(playerid, COLOR_GRAD1, "That house hasn't been created yet.");
			
		if(GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
			return SendClientMessage(playerid, COLOR_GREY, "You can not do that while spectating.");
			
		SetPlayerPos(playerid, HouseInfo[houseid][hOutsideX], HouseInfo[houseid][hOutsideY], HouseInfo[houseid][hOutsideZ]);
		SetPlayerInterior(playerid, 0);
		PlayerInfo[playerid][pInt] = 0;
		SetPlayerVirtualWorld(playerid, 0);
		PlayerInfo[playerid][pVirtualWorld] = 0;
		format(str, sizeof(str), "~w~Teleporting to ~b~\"%s\"~w~.", HouseInfo[houseid][hTag]);
		displayCenterHUDInfo(playerid,  str, 8); //display for 8 seconds
	}
	else
	{
		AdmErrorMsg;
	}
	return 1;
}
CMD:asethousetag(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 4 || PlayerInfo[playerid][pBizMod] == 1)
	{
		new houseid;
		if(sscanf(params, "ds[128]", houseid, tmp)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /asethousetag [House ID] [New Tag]");
  		if(HouseInfo[houseid][hCreated] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That house hasn't been created yet.");
		if(HouseInfo[houseid][hOwned] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That house isn't owned yet.");
		if(strlen(tmp) < 4 || strlen(tmp) > 25) return SendClientMessage(playerid, COLOR_GRAD1, "The tag must be greater than 4 and lesser than 26.");

		strdel(HouseInfo[houseid][hTag], 0, 25);
		strmid(HouseInfo[houseid][hTag], tmp, 0, sizeof(tmp), 25);
        SetHouseTag(houseid);
		format(str, sizeof(str), "That house's tag has been set to \"%s\"", HouseInfo[houseid][hTag]);
		SendClientMessage(playerid, COLOR_GRAD1, str);
		HouseInfo[houseid][hModified] = 1;
	}
	else
	{
		AdmErrorMsg;
	}
	return 1;
}
CMD:ogethouseowner(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1 && PlayerInfo[playerid][pBizMod] != 1)
		return AdmErrorMsg;
		
	new name[MAX_PLAYER_NAME + 1];
	if(sscanf(params, "s[25]", name))
	    return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /ogethouseowner [full name (case matters)]");

	new query[300];
	mysql_format(sqlGameConnection, query, sizeof(query), "SELECT HouseID FROM houses INNER JOIN players ON houses.OwnerID = players.ID WHERE players.Name = '%e' LIMIT 1", name);
	mysql_tquery(sqlGameConnection, query, "Onogethouseowner", "is", playerid, name);
    return 1;
}
forward Onogethouseowner(playerid, name[]);
public Onogethouseowner(playerid, name[])
{
	if(cache_num_rows() != 0)
	{
		new
			ownerstr[128], sqlid = cache_get_field_content_int(0, "HouseID");
		format(ownerstr, 128, "Owner: %s, SQL house ID/post code: %d", name, sqlid);
		SendClientMessage(playerid, COLOR_GREY, ownerstr);
		return 1;
	}
	SendClientMessage(playerid, COLOR_GREY, "That player doesn't own a house.");
	return 1;
}
CMD:houseinfo(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 1 || PlayerInfo[playerid][pBizMod] == 1)
	{
		for(new i = 0; i < MAX_HOUSES; i++)
		{
			if(HouseInfo[i][hCreated] == 1)
			{
				if(IsPlayerInRangeOfPoint(playerid, 1.0, HouseInfo[i][hOutsideX], HouseInfo[i][hOutsideY], HouseInfo[i][hOutsideZ]))
				{
					format(str, sizeof(str), "Owner: %s, ID: %d, Owner SQL ID: %d", HouseInfo[i][hOwner], i, HouseInfo[i][hOwnerID]);
					SendClientMessage(playerid, COLOR_GRAD1, str);
				}
			}
		}
	}
	return 1;
}

CMD:edithouse(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 4 || PlayerInfo[playerid][pBizMod] == 1)
	{
		new tmp2[128];
		new houseid;
		if(sscanf(params, "s[128] ", tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /edithouse [action] [houseid]");
			SendClientMessage(playerid, COLOR_GRAD2, "ACTIONS: entrance, interior, value");
			return 1;
		}
		if(strcmp(tmp, "entrance", true) == 0)
		{
			if(sscanf(params, "s[8] d", tmp2, houseid))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /edithouse entrance [houseid]");
				return 1;
			}
			new Float:X, Float:Y, Float:Z, Float:A;
			GetPlayerPos(playerid, X, Y, Z);
			GetPlayerFacingAngle(playerid, A);
			if(HouseInfo[houseid][hCreated] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That house has not even been created yet.");
			HouseInfo[houseid][hOutsideX] = X;
			HouseInfo[houseid][hOutsideY] = Y;
			HouseInfo[houseid][hOutsideZ] = Z;
			HouseInfo[houseid][hOutsideA] = A;
			if(HouseInfo[houseid][hOutsidePickup])
			{
				DestroyDynamicPickup(HouseInfo[houseid][hOutsidePickup]);
			}
			HouseInfo[houseid][hOutsidePickup] = CreateDynamicPickup(1273, 1, HouseInfo[houseid][hOutsideX], HouseInfo[houseid][hOutsideY], HouseInfo[houseid][hOutsideZ]);
			DestroyDynamic3DTextLabel(HouseInfoText3D[houseid]);
			HouseInfoText3D[houseid] = CreateDynamic3DTextLabel("", COLOR_HOUSETEXT, HouseInfo[houseid][hOutsideX], HouseInfo[houseid][hOutsideY], HouseInfo[houseid][hOutsideZ] +0.88, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
			SetHouseTag(houseid);
			format(str, sizeof(str), "You have moved houseid %d's entrance.", houseid);
			SendClientMessage(playerid, COLOR_GRAD1, str);
			HouseInfo[houseid][hModified] = 1;
			return 1;
		}
		else if(strcmp(tmp, "interior", true) == 0)
		{
			new interiorid;
			if(sscanf(params, "s[128] dd", tmp2, houseid, interiorid))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /edithouse interior [houseid] [interiorid]");
				format(str, sizeof(str), "{00BFFF}Usage:{FFFFFF} /viewhouseint [0-%d] or /gotoint", MAX_HOUSE_INTERIORS);
				SendClientMessage(playerid, COLOR_GRAD2, str);
				return 1;
			}
			if(HouseInfo[houseid][hCreated] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That house has not even been created yet.");
			
			if(interiorid < 0 || interiorid > MAX_HOUSE_INTERIORS)
			{
				format(str, sizeof(str), "Number can't be below 0 or above %d.", MAX_HOUSE_INTERIORS);
				SendClientMessage(playerid, COLOR_GREY, str);
				return 1;
			}
			
			SetHouseInterior(houseid, interiorid);
			format(str, sizeof(str), "You have edited houseid %d's interiorid to %d.", houseid, interiorid);
			SendClientMessage(playerid, COLOR_GRAD1, str);
			HouseInfo[houseid][hModified] = 1;
			return 1;
		}
		else if(strcmp(tmp, "value", true) == 0)
		{
			new value;
			if(sscanf(params, "s[128] dd", tmp2, houseid, value))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /edithouse value [houseid] [new value]");
				return 1;
			}
			if(HouseInfo[houseid][hCreated] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That house has not even been created yet.");
			HouseInfo[houseid][hValue] = value;
            SetHouseTag(houseid);
			format(str, sizeof(str), "You have edited houseid %d's value to $%d.", houseid, value);
			SendClientMessage(playerid, COLOR_GRAD1, str);
			HouseInfo[houseid][hModified] = 1;
			return 1;
	}
		else
		{
			format(str, sizeof(str), "Invalid action \"%s\".", tmp);
			SendClientMessage(playerid, COLOR_GRAD1, str);
			return 1;
		}
	}
	else
	{
		AdmErrorMsg;
	}
	return 1;
}

CMD:viewhouseint(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >=4 || PlayerInfo[playerid][pBizMod] == 1)
	{
		new interiorid;
		if(sscanf(params, "i", interiorid)) {
			format(str, sizeof(str), "{00BFFF}Usage:{FFFFFF} /viewhouseint [0-%d]", MAX_HOUSE_INTERIORS);
			SendClientMessage(playerid, COLOR_GREY, str);
			return 1;
		}
		if(interiorid < 0 || interiorid > MAX_HOUSE_INTERIORS)
		{
			format(str, sizeof(str), "Number can't be below 0 or above %d.", MAX_HOUSE_INTERIORS);
			SendClientMessage(playerid, COLOR_GREY, str);
			return 1;
		}
		SetPlayerInHouseInterior(playerid, interiorid);
	}
	return 1;
}
//=============================[ Player Commands ]==============================
CMD:sellhousetomarket(playerid, params[])
{
	//Conditions
	if(PlayerInfo[playerid][pHouseID] == 0)
		return SendClientMessage(playerid, COLOR_GRAD1, "You do not own a house to sell.");
		
	if(HouseFiresRemaining[PlayerInfo[playerid][pHouseID]] > 0)
	    return SendClientMessage(playerid, COLOR_GRAD1, "You cannot sell your house while its on fire!");
		
	new
	    houseid = PlayerInfo[playerid][pHouseID], location[60],
 	   value = HouseInfo[houseid][hValue] / 4;
	    
 	Get2DZone(location, MAX_ZONE_NAME, HouseInfo[houseid][hOutsideX], HouseInfo[houseid][hOutsideY], HouseInfo[houseid][hOutsideZ]);

	if(sscanf(params, "s[128]", str))
	{
		format(str, sizeof(str), "Your property is currently worth {FFFF00}$%d{FAFAFA}, type {FF0000}/sellhousetomarket confirm{FAFAFA} to sell {FFFF00}%d %s.", value, houseid, location);
		return SendClientMessage(playerid, COLOR_WHITE, str);
	}
	
	//Sell the house
	if(strcmp(str, "confirm", true) == 0)
	{
		GiveMoney(playerid, value);
		HouseInfo[houseid][hOwned] = 0;
		HouseInfo[houseid][hLocked] = 0;
		HouseInfo[houseid][hCash] = 0;
		HouseInfo[houseid][hMaterials] = 0;
		HouseInfo[houseid][hPot] = 0;
		HouseInfo[houseid][hCrack] = 0;
		HouseInfo[houseid][hAlarm] = 0;
		HouseInfo[houseid][hAlarmStatus] = 0;
		HouseInfo[houseid][hGun1] = 0;
		HouseInfo[houseid][hGun2] = 0;
		HouseInfo[houseid][hGun3] = 0;
		HouseInfo[houseid][hGun4] = 0;
		HouseInfo[houseid][hOwnerID] = -1;
		format(HouseInfo[houseid][hOwner], 24, "Nobody");
		PlayerInfo[playerid][pHouseID] = 0;
		format(str, sizeof(str), "Congratulations, you have sold {FFFF00}%d %s{FAFAFA} for $%d.", houseid, location, value);
		SendClientMessage(playerid, COLOR_WHITE, str);
		strdel(HouseInfo[houseid][hTag], 0, strlen(HouseInfo[houseid][hTag]));
		strins(HouseInfo[houseid][hTag], "House", 0);
		DestroyDynamicPickup(HouseInfo[houseid][hOutsidePickup]);
    	HouseInfo[houseid][hOutsidePickup] = CreateDynamicPickup(HOUSE_UNOWNED, 1, HouseInfo[houseid][hOutsideX], HouseInfo[houseid][hOutsideY], HouseInfo[houseid][hOutsideZ]);
        SetHouseTag(houseid);
        HouseInfo[houseid][hModified] = 1;
	}
	else
	{
		format(str, sizeof(str), "Your property is currently worth {FFFF00}$%d{FAFAFA}, type {FF0000}/sellhousetomarket confirm{FAFAFA} to sell {FFFF00}%d %s.", value, houseid, location);
		SendClientMessage(playerid, COLOR_WHITE, str);
	}
	return 1;
}

CMD:sethousetag(playerid, params[])
{
	if(PlayerInfo[playerid][pHouseID] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "You do not own a house.");
	new houseid = PlayerInfo[playerid][pHouseID];
	if(sscanf(params, "s[128]", str)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /sethousetag [Text]");
	if(strlen(str) < 4 || strlen(str) > 25) return SendClientMessage(playerid, COLOR_GRAD1, "The tag must be greater than 4 and lesser than 26.");
	if(!AlphaNumeric(tmp))
	{
		SendClientMessage(playerid, COLOR_RED, "** Invalid characters.");
		return 1;
	}
	strdel(HouseInfo[houseid][hTag], 0, 25);
	strmid(HouseInfo[houseid][hTag], str, 0, sizeof(str), 25);
    SetHouseTag(houseid);
	format(str, sizeof(str), "Your house's tag has been set to \"%s\"", HouseInfo[houseid][hTag]);
	SendClientMessage(playerid, COLOR_GRAD1, str);
	HouseInfo[houseid][hModified] = 1;
	return 1;
}

CMD:house(playerid, params[])
{
	if(PlayerInfo[playerid][pHouseID] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "You do not own a house.");
	new houseid = PlayerInfo[playerid][pHouseID];

	if(HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])
		return SendClientMessage(playerid, COLOR_GREY, "You don't own this house.");

	if(!IsPlayerInRangeOfPoint(playerid, 100.0, HouseInfo[houseid][hInsideX], HouseInfo[houseid][hInsideY], HouseInfo[houseid][hInsideZ]) || GetPlayerVirtualWorld(playerid) != HouseInfo[houseid][hSQLID])
	{
		SendClientMessage(playerid, COLOR_GRAD1, "You are not inside your house.");
		return 1;
	}
	new action[2][32];
	if(sscanf(params, "s[32] ", action[0]))
	{
		SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house [action]");
		SendClientMessage(playerid, COLOR_GRAD2, "ACTIONS: store, get, check.");
		return 1;
	}
	if(strcmp(action[0], "store", true) == 0)
	{
		if(sscanf(params, "s[32] s[32] ", action[0], action[1]))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house store [item]");
			SendClientMessage(playerid, COLOR_GRAD2, "Items: gun, cash, materials, pot, crack");
			return 1;
		}
		if(strcmp(action[1], "gun", true) == 0)
		{
			new weaponid = GetPlayerWeapon(playerid);
			if(HaveWeapon(playerid, weaponid) == weaponid && (weaponid == 22 || weaponid == 24 || weaponid == 25 || weaponid == 27 || weaponid == 29 || weaponid == 31 || weaponid == 30 || weaponid == 33 || weaponid == 34))
			{
				new weaponname[64];
				GetWeaponName(weaponid, weaponname, 64);
				if(HouseInfo[houseid][hGun1] == 0)
				{
					if(PlayerInfo[playerid][pMask] == 0)
					{
						// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
						format(str, sizeof(str), "* %s stores a %s into their safe.", PlayerICName(playerid), weaponname);
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					}
					format(str, sizeof(str), "* You have stored a %s.", weaponname);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					HouseInfo[houseid][hGun1] = weaponid;
					TakePlayerWeapon(playerid, weaponid);
					SetPlayerArmedWeapon(playerid, 0);
					HouseInfo[houseid][hModified] = 1;
					
					return 1;
				}
				else if(HouseInfo[houseid][hGun2] == 0)
				{
					if(PlayerInfo[playerid][pMask] == 0)
					{
						// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
						format(str, sizeof(str), "* %s stores a %s into their safe.", PlayerICName(playerid), weaponname);
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					}
					format(str, sizeof(str), "* You have stored a %s.", weaponname);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					HouseInfo[houseid][hGun2] = weaponid;
					TakePlayerWeapon(playerid, weaponid);
					SetPlayerArmedWeapon(playerid, 0);
					HouseInfo[houseid][hModified] = 1;
					return 1;
				}
				else if(HouseInfo[houseid][hGun3] == 0)
				{
					if(PlayerInfo[playerid][pMask] == 0)
					{
						// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
						format(str, sizeof(str), "* %s stores a %s into their safe.", PlayerICName(playerid), weaponname);
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					}
					format(str, sizeof(str), "* You have stored a %s.", weaponname);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					HouseInfo[houseid][hGun3] = weaponid;
					TakePlayerWeapon(playerid, weaponid);
					SetPlayerArmedWeapon(playerid, 0);
					HouseInfo[houseid][hModified] = 1;
					return 1;
				}
				else if(HouseInfo[houseid][hGun4] == 0 && PlayerInfo[playerid][pDonateRank] >= 2)
				{
					if(PlayerInfo[playerid][pMask] == 0)
					{
						// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
						format(str, sizeof(str), "* %s stores a %s into their safe.", PlayerICName(playerid), weaponname);
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					}
					format(str, sizeof(str), "* You have stored a %s.", weaponname);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					HouseInfo[houseid][hGun4] = weaponid;
					TakePlayerWeapon(playerid, weaponid);
					SetPlayerArmedWeapon(playerid, 0);
					HouseInfo[houseid][hModified] = 1;
					return 1;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GRAD1, "None of your gun slots are empty.");
				}
				return 1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GRAD1, "You can not store this.");
			}
		}
		if(strcmp(action[1], "pot", true) == 0 || strcmp(action[1], "weed", true) == 0)
		{
			new potam = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], potam)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house store pot [amount]");
			if(potam < 1 || potam > 1000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(potam > PlayerInfo[playerid][pPot]) return SendClientMessage(playerid, COLOR_GRAD1, "You don't have that much pot.");
			if(HouseInfo[houseid][hPot] +potam > 1000) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe is at or greater than it's pot limit.");
			HouseInfo[houseid][hPot] += potam;
			PlayerInfo[playerid][pPot] -= potam;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				format(str, sizeof(str), "* %s takes out some pot and puts it into their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have stored %d pot into your safe.", potam);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		if(strcmp(action[1], "crack", true) == 0)
		{
			new crackam = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], crackam)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house store crack [amount]");
			if(crackam < 1 || crackam > 1000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(crackam > PlayerInfo[playerid][pCrack]) return SendClientMessage(playerid, COLOR_GRAD1, "You don't have that much crack.");
			if(HouseInfo[houseid][hCrack] +crackam > 1000) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe is at or greater than it's crack limit.");
			HouseInfo[houseid][hCrack] += crackam;
			PlayerInfo[playerid][pCrack] -= crackam;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
				format(str, sizeof(str), "* %s takes out some crack and puts it into their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have stored %d crack into your safe.", crackam);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		if(strcmp(action[1], "materials", true) == 0 || strcmp(action[1], "mats", true) == 0)
		{
			new matam = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], matam)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house store mats/materials [amount]");
			if(matam < 1 || matam > 500000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(matam > PlayerInfo[playerid][pMats]) return SendClientMessage(playerid, COLOR_GRAD1, "You don't have that many materials.");
			if(HouseInfo[houseid][hMaterials] +matam > 500000) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe is at or greater than it's materials limit.");
			HouseInfo[houseid][hMaterials] += matam;
			PlayerInfo[playerid][pMats] -= matam;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
				format(str, sizeof(str), "* %s takes out some materials and puts it into their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have stored %d materials into your safe.", matam);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		if(strcmp(action[1], "money", true) == 0 || strcmp(action[1], "cash", true) == 0)
		{
			new casham;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], casham)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house store money/cash [amount]");
			if(casham < 1 || casham > 1000000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(casham > PlayerInfo[playerid][pCash]) return SendClientMessage(playerid, COLOR_GRAD1, "You don't have that much cash.");
			if(HouseInfo[houseid][hCash] +casham > 1000000) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe is at or greater than it's cash limit.");
			HouseInfo[houseid][hCash] += casham;
			PlayerInfo[playerid][pCash] -= casham;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
				format(str, sizeof(str), "* %s takes out some cash and puts it into their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have stored $%d cash into your safe.", casham);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		return 1;
	}
	if(strcmp(action[0], "get", true) == 0)
	{
		if(sscanf(params, "s[32] s[32] ", action[0], action[1]))
		{
			if(PlayerInfo[playerid][pDonateRank] == 3)
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house get [item]");
				SendClientMessage(playerid, COLOR_GRAD2, "Items: gun, cash, materials, pot, crack");
				return 1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house get [item]");
				SendClientMessage(playerid, COLOR_GRAD2, "Items: gun, cash, materials, pot, crack");
				return 1;
			}
		}
		if(strcmp(action[1], "gun", true) == 0)
		{
			new slot = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], slot))
			{
				if(PlayerInfo[playerid][pDonateRank] >= 2) SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house get gun [1-4]");
				else SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house get gun [1-3]");
				return 1;
			}
			new weaponname[64];
			if(slot == 1)
			{
				if(HouseInfo[houseid][hGun1] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That slot is empty.");
				new weaponid = HouseInfo[houseid][hGun1];
				GivePlayerGun(playerid, weaponid);
				GetWeaponName(weaponid, weaponname, 64);
				if(PlayerInfo[playerid][pMask] == 0)
				{
					// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
					format(str, sizeof(str), "* %s takes a weapon from their safe.", PlayerICName(playerid));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				}
				format(str, sizeof(str), "* You have taken a %s from your house.", weaponname);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
				HouseInfo[houseid][hGun1] = 0;
				HouseInfo[houseid][hModified] = 1;
				return 1;
			}
			if(slot == 2)
			{
				if(HouseInfo[houseid][hGun2] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That slot is empty.");
				new weaponid = HouseInfo[houseid][hGun2];
				GivePlayerGun(playerid, weaponid);
				GetWeaponName(weaponid, weaponname, 64);
				if(PlayerInfo[playerid][pMask] == 0)
				{
					// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
					format(str, sizeof(str), "* %s takes a weapon from their safe.", PlayerICName(playerid));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				}
				format(str, sizeof(str), "* You have taken a %s from your house.", weaponname);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
				HouseInfo[houseid][hGun2] = 0;
				HouseInfo[houseid][hModified] = 1;
				return 1;
			}
			if(slot == 3)
			{
				if(HouseInfo[houseid][hGun3] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That slot is empty.");
				new weaponid = HouseInfo[houseid][hGun3];
				GivePlayerGun(playerid, weaponid);
				GetWeaponName(weaponid, weaponname, 64);
				if(PlayerInfo[playerid][pMask] == 0)
				{
					// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
					format(str, sizeof(str), "* %s takes a weapon from their safe.", PlayerICName(playerid));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				}
				format(str, sizeof(str), "* You have taken a %s from your house.", weaponname);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
				HouseInfo[houseid][hGun3] = 0;
				HouseInfo[houseid][hModified] = 1;
				return 1;
			}
			if(slot == 4)
			{
				if(PlayerInfo[playerid][pDonateRank] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You're not a silver VIP or above.");
				if(HouseInfo[houseid][hGun4] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That slot is empty.");
				new weaponid = HouseInfo[houseid][hGun4];
				GivePlayerGun(playerid, weaponid);
				GetWeaponName(weaponid, weaponname, 64);
				if(PlayerInfo[playerid][pMask] == 0)
				{
					// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
					format(str, sizeof(str), "* %s takes a weapon from their safe.", PlayerICName(playerid));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				}
				format(str, sizeof(str), "* You have taken a %s from your house.", weaponname);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
				HouseInfo[houseid][hGun4] = 0;
				HouseInfo[houseid][hModified] = 1;
				return 1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GRAD1, "Invalid weapon slot.");
				return 1;
			}
		}
		if(strcmp(action[1], "pot", true) == 0 || strcmp(action[1], "weed", true) == 0)
		{
			new potam = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], potam))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house get pot [amount]");
				return 1;
			}
			if(potam < 1 || potam > 1000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(potam > HouseInfo[houseid][hPot]) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe doesn't have that much pot.");
			HouseInfo[houseid][hPot] -= potam;
			PlayerInfo[playerid][pPot] += potam;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
				format(str, sizeof(str), "* %s takes some pot from their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have taken %d pot from your safe.", potam);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		if(strcmp(action[1], "crack", true) == 0)
		{
			new crackam = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], crackam))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house get crack [amount]");
				return 1;
			}
			if(crackam < 1 || crackam > 1000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(crackam > HouseInfo[houseid][hCrack]) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe doesn't have that much crack.");
			HouseInfo[houseid][hCrack] -= crackam;
			PlayerInfo[playerid][pCrack] += crackam;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
				format(str, sizeof(str), "* %s takes some crack from their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have taken %d crack from your safe.", crackam);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		if(strcmp(action[1], "materials", true) == 0 || strcmp(action[1], "mats", true) == 0)
		{
			new matam = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], matam))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house get materials [amount]");
				return 1;
			}
			if(matam < 1 || matam > 500000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(matam > HouseInfo[houseid][hMaterials]) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe doesn't have that many materials.");
			HouseInfo[houseid][hMaterials] -= matam;
			PlayerInfo[playerid][pMats] += matam;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
				format(str, sizeof(str), "* %s takes some materials from their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have taken %d materials from your safe.", matam);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		if(strcmp(action[1], "money", true) == 0 || strcmp(action[1], "cash", true) == 0)
		{
			new casham = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], casham))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house get money [amount]");
				return 1;
			}
			if(casham < 1 || casham > 1000000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(casham > HouseInfo[houseid][hCash]) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe doesn't have that much cash.");
			HouseInfo[houseid][hCash] -= casham;
			PlayerInfo[playerid][pCash] += casham;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
				format(str, sizeof(str), "* %s takes some cash from their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have taken $%d cash from your safe.", casham);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		return 1;
	}
	if(strcmp(action[0], "check", true) == 0)
	{
		DisplaySafeInfo(houseid, playerid);
		return 1;
	}
	return 1;
}

CMD:setsale(playerid, params[])
{
	if(PlayerInfo[playerid][pHouseID] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "You do not own a house.");
	new confirmed[24];
	if(!sscanf(params, "s[24]", confirmed))
	{
		if(!(strcmp(confirmed, "confirm", true) != 0))
		{
			if(!(gForSales[playerid][PlayerHasASaleBoard]))
			{
				new houseid = PlayerInfo[playerid][pHouseID];

				if(HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])
					return SendClientMessage(playerid, COLOR_GREY, "You don't own this house.");

				if(IsPlayerInRangeOfPoint(playerid, 15.0, HouseInfo[houseid][hOutsideX], HouseInfo[houseid][hOutsideY], HouseInfo[houseid][hOutsideZ]))
				{
					new pcash = GetPlayerMoney(playerid);
					if(pcash > 5000)
					{
						GiveMoney(playerid, -5000);
						new Float:x, Float:y, Float:z, Float:Angle;
						GetPlayerPos(playerid, x, y, z);
						GetPlayerFacingAngle(playerid, Angle);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "For-Sale was succesfully added, '/destroysale' in order to remove it.");
						gForSales[playerid][Saleobject] = CreateDynamicObject(19471, (x -0.1), (y -0.1), (z -1), 0, 0, (Angle -270));
						gForSales[playerid][PlayerHasASaleBoard] = true;
					}
					else SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
				else SendClientMessage(playerid, COLOR_GREY, "You are not in range of your house.");
			}
			else SendClientMessage(playerid, COLOR_GREY, "You already have a For-Sale.");
		}
	}
	else
	{
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "** This costs 5000$, Point youself in the position you want it to be pointed at.");
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "** add the parameter \"confirm\" to confirm the sale-board.");
	}
	return 1;
}
CMD:destroysale(playerid, params[])
{
	if(gForSales[playerid][PlayerHasASaleBoard])
	{
		new Float:x, Float:y, Float:z;
		GetDynamicObjectPos(gForSales[playerid][Saleobject], x, y, z);
		if(IsPlayerInRangeOfPoint(playerid, 3.0, x, y, z))
		{
			DestroyDynamicObject(gForSales[playerid][Saleobject]);
			gForSales[playerid][Saleobject] = 0;
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "For-Sale was succesfully destroyed");
			gForSales[playerid][PlayerHasASaleBoard] = false;
		}
		else SendClientMessage(playerid, COLOR_GREY, "You have to be in range of your For-Sale in order to destroy it.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You do not have a For-Sale.");
	return 1;
}

CMD:lockhouse(playerid, params[])
{
	new houseid = PlayerInfo[playerid][pHouseID];
	if(houseid !=0)
	{
	    if(HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])return SendClientMessage(playerid, COLOR_GREY, "You don't own this house.");
		if(IsPlayerInRangeOfPoint(playerid, 1.0, HouseInfo[houseid][hOutsideX], HouseInfo[houseid][hOutsideY], HouseInfo[houseid][hOutsideZ]) || IsPlayerInRangeOfPoint(playerid, 3.0, HouseInfo[houseid][hInsideX], HouseInfo[houseid][hInsideY], HouseInfo[houseid][hInsideZ]) && GetPlayerVirtualWorld(playerid) == HouseInfo[houseid][hSQLID])
		{
			if(HouseInfo[houseid][hLocked])
			{
				format(str, sizeof(str), "* %s has unlocked the door to their property.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				HouseInfo[houseid][hLocked] = 0;
				HouseInfo[houseid][hModified] = 1;
				return 1;
			}
			else
			{
				format(str, sizeof(str), "* %s has locked the door to their property.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				HouseInfo[houseid][hLocked] = 1;
				HouseInfo[houseid][hModified] = 1;
				return 1;
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GRAD1, "You are not near / inside the property you own.");
			return 1;
		}
	}
	return 1;
}
CMD:buyhouse(playerid, params[])
{
	if(PlayerInfo[playerid][pHouseID] != 0)
		return SendClientMessage(playerid, COLOR_GRAD1, "You already own a house.");
		
	if(PlayerInfo[playerid][pLevel] == 1)
		return SendClientMessage(playerid, COLOR_GRAD1, "You must be level 2+ to buy a house.");
		
	new houseid = GetPlayerNearHouseID(playerid);
	
	if(!houseid)
		return SendClientMessage(playerid, COLOR_GRAD1, "You are not near a house.");
	
	if(HouseInfo[houseid][hOwned] == 1)
		return SendClientMessage(playerid, COLOR_GRAD1, "This property is already owned.");
		
	if(PlayerInfo[playerid][pCash] < HouseInfo[houseid][hValue] && PlayerInfo[playerid][hTicket] == 0)
		return SendClientMessage(playerid, COLOR_GRAD1, "You have incufficent funds to purchase this property.");
		
    new location[MAX_ZONE_NAME];
	location = GetHouseLocation(houseid);
    
	if(sscanf(params, "s[128] ", str))
	{
		format(str, sizeof(str), "This property will cost you $%d, type {FF0000}/buyhouse confirm{F8ECE0} to purchase {FFFF00}%d %s", HouseInfo[houseid][hValue], houseid, location);
		SendClientMessage(playerid, COLOR_WHITE, str);
		return 1;
	}
	if(strcmp(str, "confirm", true) == 0)
	{
		HouseInfo[houseid][hOwned] = 1;
		PlayerInfo[playerid][pHouseID] = houseid;
		HouseInfo[houseid][hOwnerID] = PlayerInfo[playerid][pID];
		strmid(HouseInfo[houseid][hOwner], PlayerName(playerid), 0, strlen(PlayerName(playerid)), 255);
		GiveMoney(playerid, -HouseInfo[houseid][hValue]);
		format(str, sizeof(str), "Congratulations, you just purchased {FFFF00}%d %s{F8ECE0} for $%d.", houseid, location, HouseInfo[houseid][hValue]);
		SendClientMessage(playerid, COLOR_WHITE, str);
		SendClientMessage(playerid, COLOR_WHITE, "Type {FF0000}/househelp{F8ECE0} for assistance.");
		strdel(HouseInfo[houseid][hTag], 0, strlen(HouseInfo[houseid][hTag]));
		strins(HouseInfo[houseid][hTag], "House", 0);
  		DestroyDynamicPickup(HouseInfo[houseid][hOutsidePickup]);
    	HouseInfo[houseid][hOutsidePickup] = CreateDynamicPickup(1273, 1, HouseInfo[houseid][hOutsideX], HouseInfo[houseid][hOutsideY], HouseInfo[houseid][hOutsideZ]);
		SetHouseTag(houseid);

		format(str, sizeof(str), "%s has purchased a house in %s.", PlayerICName(playerid), location);
		UpdateGameFeed(str, GAMEFEED_COLOR_GREEN);
		
		HouseInfo[houseid][hModified] = 1;
		return 1;
	}
	return 1;
}

CMD:sellhouse(playerid, params[])
{
	new houseid = PlayerInfo[playerid][pHouseID];
	
	if(HouseFiresRemaining[houseid] > 0)
	    return SendClientMessage(playerid, COLOR_GRAD1, "You cannot sell your house while its on fire!");
	    
	if(PlayerInfo[playerid][pHouseID] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "You do not own a house to sell.");
	if(!IsPlayerInRangeOfPoint(playerid, 20.0, HouseInfo[houseid][hOutsideX], HouseInfo[houseid][hOutsideY], HouseInfo[houseid][hOutsideZ])) return SendClientMessage(playerid, COLOR_GRAD1, "You are not near your house.");
	new location[MAX_ZONE_NAME];
	Get2DZone(location, MAX_ZONE_NAME, HouseInfo[houseid][hOutsideX], HouseInfo[houseid][hOutsideY], HouseInfo[houseid][hOutsideZ]);
	new selltoid, hoprice = 0;
	if(sscanf(params, "ud", selltoid, hoprice))
	{
		SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /sellhouse [PlayerID/PartOfName] [Price]");
		return 1;
	}
	if(IsPlayerConnected(selltoid))
	{
		new Float:pgx, Float:pgy, Float:pgz;
		GetPlayerPos(selltoid, pgx, pgy, pgz);
		if(!IsPlayerInRangeOfPoint(playerid, 3.0, pgx, pgy, pgz)) return SendClientMessage(playerid, COLOR_GREY, "You are not near the player! ");
		if(hoprice < 10000 || hoprice > 5000000) return SendClientMessage(playerid, COLOR_GRAD1, "Price must be in between $10,000 and $5,000,000.");
		if(PlayerInfo[selltoid][pHouseID] != 0) return SendClientMessage(playerid, COLOR_GRAD1, "This player already owns a house.");
		if(selltoid == playerid) return SendClientMessage(playerid, COLOR_GRAD1, "You can not sell your house to yourself.");
		OfferedSellHouse[selltoid][0] = playerid;
		OfferedSellHouse[selltoid][1] = hoprice;
		OfferedSellHouse[selltoid][2] = houseid;
		format(str, sizeof(str), "** %s has offered to sell you their house (%d %s) for $%d! /accept house to accept.", PlayerICName(playerid), houseid, location, hoprice);
		SendClientMessage(selltoid, COLOR_LIGHTBLUE, str);
		format(str, sizeof(str), "** You have offered to sell your house to %s (%d %s) for $%d.", PlayerICName(selltoid), houseid, location, hoprice);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		HouseInfo[houseid][hModified] = 1;
		return 1;
	}
	return 1;
}