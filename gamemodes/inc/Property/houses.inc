#if defined HOUSES_INCLUDED
    #endinput
#endif

#define HOUSES_INCLUDED

#include "..\gamemodes\inc\Property\gates.inc"

// IMPORTANT NOTICE; HouseInfo[houseid][hModified] = 1; enables saving to that house
// set hModified to 1, only when the house is modified and requires saving.
// this is for improved performance.
//============================[ Loading/Saving ]================================
static str[128], tmp[128];
enum _houseInteriorInfo
{
	hInteriorID,
    Float:hX,
    Float:hY,
    Float:hZ,
	Float:hA,
	hName[32],
	hFreeze,
	hLevel
}



new houseInteriorInfo[][_houseInteriorInfo] = {
	{0, 1.7592,12.9488,999.4284,89.8906, "Angel Pine Trailer", 0, 1}, //index 0
	{1, 244.5000, 305.0000, 999.1484, 270.0000, "Small Room 1", 0, 1},
	{2, 267.1000, 305.0000, 999.1484, 270.0000, "Small Room 2", 0, 1},
	{10, 2259.3948, -1135.8015, 1050.6403, 270.0000, "Motel Room", 0, 2},
	{15, 386.8663, 1471.6508, 1080.1949, 86.6616, "Small House 1", 0, 3},
	{5, 22.7071, 1403.3888, 1084.4370, 359.5774, "Small House 2", 0,4},
	{4, 301.3000, 306.3000, 1003.5391, 270.0000, "Wood House", 0, 7},
	{15, 295.0853, 1472.4978, 1080.2578, 357.6974, "Med House 1", 0, 5},
	{15, 386.8132, 1471.5809, 1080.1949, 87.9148, "Med House 2", 0, 6},
	{2, 446.9661, 1397.6705, 1084.3047, 3.1456, "Med House 3", 0, 8},
	{15, 376.3000, 1417.2000, 1081.3281, 84.5970, "Rich House 1", 0, 9},
	{15, 295.0820, 1472.5895, 1080.2578, 357.9638, "Rich House 2", 0, 6},
	{6, 234.1893, 1064.2260, 1084.2115, 357.8933, "Rich House 3", 0, 11},
	{7, 225.7264, 1021.7980, 1084.0167, 0.4233, "Rich House 4", 0, 50},
	{12, 2324.4729, -1148.9298, 1050.7101, 359.4833, "Rich House 5", 0, 55},
	{3, 235.2827, 1187.0928, 1080.2578, 358.8566, "Rich House 6", 0, 45},
	{6, 344.4200, 304.9100, 999.1484, 280.0000, "BDSM Lair", 0, 20},
	{5, 1226.5758, -812.2072, 1084.0078, 0.0000, "Office", 0, 5},
	{6, 0744.4441, 1437.6198, 1102.7031, 360.0000, "Stripclub", 0, 5},
	{3, 963.4101, -53.1195, 1001.1246, 87.6137, "Brothel", 0, 10},
	{2, 2549.1807, -1294.7899, 1060.9844, 262.7453, "Big Smokes", 0, 40},
	{5, 226.54, 1114.31, 1080.99, 270.0000, "VIP House 1", 0, 60},
	{5, 140.26, 1366.78, 1083.86, 0.0000, "VIP House 2", 0, 45},
	{10, 23.9781, 1340.4080, 1084.3750, 357.0715, "New House", 0, 40},
	{4, -260.9349,1456.6975,1084.3672,86.3724, "New House", 0, 32},
	{9, 83.0348,1322.7543,1083.8662,359.8916, "New House", 0, 37},
	{9, 2317.8899,-1026.7600,1050.2178,359.8916, "New House", 0, 37},
	{3, 2495.9409,-1692.5228,1014.7422,178.8066, "New House", 0, 37},
	{8, 2807.6604,-1174.4706,1025.5703,358.0116, "New House", 0, 24},
	{6, 2196.5779,-1204.4008,1049.0234,93.5792, "New House", 0, 26},
	{10, 2270.1311,-1210.4797,1047.5625,89.1925, "New House", 0, 21},
	{8, 2365.2231,-1135.3877,1050.8750,0.5182, "New House", 0, 10},
	{2, 2237.5620,-1081.2277,1049.0234,358.0116, "New House", 0, 27},
	{4, 261.0469,1284.5985,1080.2578,358.9516, "New House", 0, 20},
	{4, 221.8893,1140.6204,1082.6094,358.0116, "New House", 0, 15},
	{6, -68.8100,1351.2100,1080.2109,358.0116, "New House", 0, 17},
	{9, 260.7921,1237.5183,1084.2578,359.5782, "New House", 0, 29},
	{2, 2468.5461,-1698.2461,1013.5078,87.9392, "New House", 0, 24},
	{1, 223.1488,1287.3376,1082.1406,359.2650, "New House", 0, 21},
	{11, 2282.9709,-1140.0293,1050.8984,357.3849, "New House", 0, 8},
	{15, 327.9431,1477.9525,1084.4375,359.5781, "New House", 0, 8},
	{8, -42.5900,1405.4700,1084.4297,359.5781, "New House", 0, 15},
	{6, 2308.7700,-1212.9399,1049.0234,359.5781, "New House", 0, 18},
	{1, 2524.4788,-1679.4126,1015.4986,268.7106, "New House", 0, 15}, //43
	{1, 243.9651,304.9588,1008.6194,272.0475, "Empty House", 1, 18},
	{1, 266.7557,304.9372,1009.5787,269.2274, "Empty House", 1, 15},
	{1, 2259.5396,-1135.8944,1061.3513,269.2040, "Empty House", 1, 22},
	{1, 446.9709,1397.0635,1096.2762,2.6000, "Empty House", 1, 10},
	{1, 376.9538,1417.4308,1090.5320,89.0590, "Empty House", 1, 11},
	{1, 295.2011,1472.5571,1288.4384,359.6507, "Empty House", 1, 20}, //needs new coords
	{1, 234.1711,1064.0579,1098.0194,1.3946, "Empty House", 1, 20},
	{1, 225.7832,1021.6218,1096.2322,1.0814, "Empty House", 1, 25},
	{1, 2324.5049,-1149.2339,1064.2856,358.9350, "Empty House", 1, 60},
	{1, 235.3277,1187.0463,1092.5670,357.8173, "Empty House", 1, 60}, //needs new coords
	{1, 343.9464,304.9431,1009.9575,266.2343, "Empty House", 1, 55},
	{1, 226.4931,1114.1849,1291.2213,271.0233, "Empty House", 1, 31}, //needs new coords
	{1, 140.3286,1366.1735,1096.6440,1.2222, "Empty House", 1, 15},
	{1, 24.0189,1340.3876,1099.2056,359.9922, "Empty House", 1, 28},
	{1, -260.8597,1456.7130,1097.5079,91.1497, "Empty House", 1, 41},
	{1, 83.0698,1322.4150,1097.1952,0.3056, "Empty House", 1, 37},
	{1, 2317.7932,-1026.7068,1065.2538,359.7022, "Empty House", 1, 39},
	{1, 2495.9241,-1692.2426,1029.1273,180.5206, "Empty House", 1, 37},
	{1, 2807.6565,-1174.5311,1037.6699,358.8089, "Empty House", 1, 28},
	{1, 2196.6406,-1204.3497,1259.0602,90.8549, "Empty House", 1, 25}, //needs new coords //
	{1, 2270.1367,-1210.4171,1256.5388,89.8916, "Empty House", 1, 25}, //needs new coords //
	{1, 2237.5422,-1081.2633,1257.1143,1.2175, "Empty House", 1, 21}, //needs new coords //
//	{0, 2365.2249,-1135.3840,1061.4880,359.5294, "Empty House", 1, 21}, //major transparent
	{1, 260.9997,1284.4519,1087.7015,357.0461, "Empty House", 1, 20},
	{1, 221.8969,1140.3651,1087.5343,358.0563, "Empty House", 1, 18},
	{1, -68.8098,1351.5551,1088.9017,1.1661, "Empty House", 1, 20},
	{1, 260.8188,1237.6218,1092.3868,359.3095, "Empty House", 1, 21},
	{1, 223.1048,1287.0809,1088.4839,5.8894, "Empty House", 1, 22},
	{1, 2282.9199,-1139.9753,1057.4589,2.7327, "Empty House", 1, 10},
	{1, 328.0080,1477.9692,1091.9458,358.9727, "Empty House", 1, 10},
	{1, -42.5948,1405.7189,1091.6273,4.2994, "Empty House", 1, 17},
	{1, 2253.7280,-1114.0651,850.8878,5.8827, "Empty Motel Room", 0, 10 },
	{1, 698.0511,338.8492,803.5403,269.6884, "Empty House", 0, 12 },
	{1, 424.7157,2519.7791,9.9994,94.2435, "Empty House", 0, 10 }
	
};
#define MAX_HOUSE_INTERIORS sizeof(houseInteriorInfo)

#define INVALID_HOUSE_ID MAX_HOUSES+1
forward GetHouseVirtualWorld(houseid);

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
		value[100];
		
	for(new i = 1; i < cache_num_rows(); i++)
	{
	    if(i == MAX_HOUSES)
			return print("[HouseInfo] WARNING: maximum houses reached.");
	    //important variables/general variables
	    if(!(cache_get_field_content_int(i, "Created"))) continue;
	    HouseInfo[i][hSQLID] = cache_get_field_content_int(i, "HouseID");
        HouseInfo[i][hOwnerID] = cache_get_field_content_int(i, "OwnerID");
        cache_get_field_content(i, "Name", value);
        format(HouseInfo[i][hOwner], MAX_PLAYER_NAME + 1, "%s", value);

		HouseInfo[i][hOwned] = cache_get_field_content_int(i, "Owned");
		HouseInfo[i][hValue] = cache_get_field_content_int(i, "Value");

		cache_get_field_content(i, "Tag", value);
		strmid(HouseInfo[i][hTag], value, 0, strlen(value), 255);
		HouseInfo[i][hLevel] = cache_get_field_content_int(i, "Level");
		HouseInfo[i][hDonated] = cache_get_field_content_int(i, "Donated");

		//icon location variables
		cache_get_field_content(i, "OutsideX", value);
		HouseInfo[i][hOutsideX] = floatstr(value);
		cache_get_field_content(i, "OutsideY", value);
		HouseInfo[i][hOutsideY] = floatstr(value);
		cache_get_field_content(i, "OutsideZ", value);
		HouseInfo[i][hOutsideZ] = floatstr(value);
		cache_get_field_content(i, "OutsideA", value);
		HouseInfo[i][hOutsideA] = floatstr(value);
		cache_get_field_content(i, "InsideX", value);
		HouseInfo[i][hInsideX] = floatstr(value);
		cache_get_field_content(i, "InsideY", value);
		HouseInfo[i][hInsideY] = floatstr(value);
		cache_get_field_content(i, "InsideZ", value);
		HouseInfo[i][hInsideZ] = floatstr(value);
		cache_get_field_content(i, "InsideA", value);
		HouseInfo[i][hInsideA] = floatstr(value);
		HouseInfo[i][hWorld]	= i;
		HouseInfo[i][hInterior] = cache_get_field_content_int(i, "Interior");

		//upgrades/storage
		HouseInfo[i][hLocked] 			= cache_get_field_content_int(i, "Locked");
		HouseInfo[i][hAlarm] 			= cache_get_field_content_int(i, "Alarm");
		HouseInfo[i][hAlarmStatus] 	= cache_get_field_content_int(i, "AlarmStatus");
		HouseInfo[i][hCash] 			= cache_get_field_content_int(i, "Cash");
		HouseInfo[i][hMaterials] 		= cache_get_field_content_int(i, "Materials");
		HouseInfo[i][hCannabis] 		= cache_get_field_content_int(i, "Cannabis");
		HouseInfo[i][hCocaine] 		= cache_get_field_content_int(i, "Cocaine");
		HouseInfo[i][hDuffleBag] 		= cache_get_field_content_int(i, "DuffleBag");
		HouseInfo[i][hMeth] 			= cache_get_field_content_int(i, "Meth");
		HouseInfo[i][hXanax] 			= cache_get_field_content_int(i, "Xanax");
		HouseInfo[i][hPromethazine] 	= cache_get_field_content_int(i, "Promethazine");
		HouseInfo[i][hCodeine] 		= cache_get_field_content_int(i, "Codeine");
		HouseInfo[i][hLean] 			= cache_get_field_content_int(i, "Lean");
		HouseInfo[i][hGun1] 			= cache_get_field_content_int(i, "Gun1");
		HouseInfo[i][hGun2] 			= cache_get_field_content_int(i, "Gun2");
		HouseInfo[i][hGun3] 			= cache_get_field_content_int(i, "Gun3");
		HouseInfo[i][hGun4] 			= cache_get_field_content_int(i, "Gun4");
		HouseInfo[i][hGun5] 			= cache_get_field_content_int(i, "Gun5");
		HouseInfo[i][hGun6] 			= cache_get_field_content_int(i, "Gun6");
		HouseInfo[i][hGun7] 			= cache_get_field_content_int(i, "Gun7");
		HouseInfo[i][hHasFurniture] 	= cache_get_field_content_int(i, "HasFurniture");
		HouseInfo[i][hFreeze] 			= cache_get_field_content_int(i, "Freeze");
		
		HouseInfo[i][hModified] = 0;

		HouseInfo[i][hOutsidePickup] = CreateDynamicPickup(HOUSE_OFFLINE, 1, HouseInfo[i][hOutsideX], HouseInfo[i][hOutsideY], HouseInfo[i][hOutsideZ]);

		HouseInfoText3D[i] = CreateDynamic3DTextLabel("", COLOR_HOUSETEXT, HouseInfo[i][hOutsideX], HouseInfo[i][hOutsideY], HouseInfo[i][hOutsideZ] +0.88, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
		SetHouseTag(i);
		
		//load the furniture if the house has furniture
		if(HouseInfo[i][hHasFurniture]) {
			printf("[Furniture] loading house furniture for houseid %d (sqlid %d)", i, HouseInfo[i][hSQLID]);
			LoadFurniture(i, HouseInfo[i][hSQLID]);
		}
		
		HouseInfo[i][hCreated] = 1;
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
		value[100], houseCountUnowned = 0;

	for(new i = ownedHousesPool + 1; i < cache_num_rows() + ownedHousesPool; i++) //starts at 1
	{
	    if(i == MAX_HOUSES)
			return print("[HouseInfo] WARNING: maximum houses reached.");
	    //important variables/general variables
	    if(!(cache_get_field_content_int(i, "Created"))) continue;
	    HouseInfo[i][hSQLID] = cache_get_field_content_int(i, "HouseID");
        HouseInfo[i][hOwnerID] = 0;

		HouseInfo[i][hOwned] = 0;
		HouseInfo[i][hValue] = cache_get_field_content_int(i, "Value");

		cache_get_field_content(i, "Tag", value);
		strmid(HouseInfo[i][hTag], value, 0, strlen(value), 255);
		HouseInfo[i][hLevel] = cache_get_field_content_int(i, "Level");
		HouseInfo[i][hDonated] = cache_get_field_content_int(i, "Donated");

		//icon location variables
		cache_get_field_content(i, "OutsideX", value);
		HouseInfo[i][hOutsideX] = floatstr(value);
		cache_get_field_content(i, "OutsideY", value);
		HouseInfo[i][hOutsideY] = floatstr(value);
		cache_get_field_content(i, "OutsideZ", value);
		HouseInfo[i][hOutsideZ] = floatstr(value);
		cache_get_field_content(i, "OutsideA", value);
		HouseInfo[i][hOutsideA] = floatstr(value);
		cache_get_field_content(i, "InsideX", value);
		HouseInfo[i][hInsideX] = floatstr(value);
		cache_get_field_content(i, "InsideY", value);
		HouseInfo[i][hInsideY] = floatstr(value);
		cache_get_field_content(i, "InsideZ", value);
		HouseInfo[i][hInsideZ] = floatstr(value);
		cache_get_field_content(i, "InsideA", value);
		HouseInfo[i][hInsideA] = floatstr(value);
		HouseInfo[i][hInterior] = cache_get_field_content_int(i, "Interior");
		HouseInfo[i][hModified] = 0;

		HouseInfo[i][hOutsidePickup] = CreateDynamicPickup(HOUSE_UNOWNED, 1, HouseInfo[i][hOutsideX], HouseInfo[i][hOutsideY], HouseInfo[i][hOutsideZ]);

		HouseInfoText3D[i] = CreateDynamic3DTextLabel("", COLOR_HOUSETEXT, HouseInfo[i][hOutsideX], HouseInfo[i][hOutsideY], HouseInfo[i][hOutsideZ] +0.88, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
		SetHouseTag(i);
		HouseInfo[i][hCreated] = 1;
		houseCountUnowned++;
	}
	printf("[HouseInfo] %d unowned houses were loaded. The last SQL ID in the table is; %d.", houseCountUnowned, lastSQLhouseID);
	return 1;
}

public SaveHouses()
{
	new
	    houseCountSaved = 0;
	for(new i = 1; i < sizeof(HouseInfo); i++)
	{
	    if(!HouseInfo[i][hCreated]) continue;
        if(!HouseInfo[i][hModified]) continue;
		new query[2500];
		mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `houses` SET `OwnerID` = %d, `Created` = %d, `Owned` = %d,",
		HouseInfo[i][hOwnerID], HouseInfo[i][hCreated], HouseInfo[i][hOwned]);

		mysql_format(sqlGameConnection, query, sizeof(query), "%s `Value` = %d, `OutsideX` = %f, `OutsideY` = %f, `OutsideZ` = %f,",
		query, HouseInfo[i][hValue], HouseInfo[i][hOutsideX], HouseInfo[i][hOutsideY], HouseInfo[i][hOutsideZ]);

		mysql_format(sqlGameConnection, query, sizeof(query), "%s `OutsideA` = %f, `InsideX` = %f, `InsideY` = %f, `InsideZ` = %f,",
		query, HouseInfo[i][hOutsideA], HouseInfo[i][hInsideX], HouseInfo[i][hInsideY], HouseInfo[i][hInsideZ]);

		mysql_format(sqlGameConnection, query, sizeof(query), "%s `InsideA` = %f, `Interior` = %d, `Locked` = %d, `Alarm` = %d,",
		query, HouseInfo[i][hInsideA], HouseInfo[i][hInterior], HouseInfo[i][hLocked], HouseInfo[i][hAlarm]);

		mysql_format(sqlGameConnection, query, sizeof(query), "%s `AlarmStatus` = %d, `Cash` = %d, `Materials` = %d, `Cannabis` = %d, `DuffleBag` = %d,",
		query, HouseInfo[i][hAlarmStatus], HouseInfo[i][hCash], HouseInfo[i][hMaterials], HouseInfo[i][hCannabis], HouseInfo[i][hDuffleBag]);

		mysql_format(sqlGameConnection, query, sizeof(query), "%s `Xanax` = %d, `Promethazine` = %d, `Codeine` = %d, `Lean` = %d,",
		query, HouseInfo[i][hXanax], HouseInfo[i][hPromethazine], HouseInfo[i][hCodeine], HouseInfo[i][hLean]);

		mysql_format(sqlGameConnection, query, sizeof(query), "%s `Cocaine` = %d, `Meth` = %d, `Gun1` = %d, `Gun2` = %d, `Gun3` = %d,",
		query, HouseInfo[i][hCocaine], HouseInfo[i][hMeth], HouseInfo[i][hGun1], HouseInfo[i][hGun2], HouseInfo[i][hGun3]);

		mysql_format(sqlGameConnection, query, sizeof(query), "%s `Gun4` = %d, `Gun5` = %d, `Gun6` = %d, `Gun7` = %d, `Tag` = '%e', `Level` = %d , `Freeze` = %d, `Donated`=%d, `HasFurniture`=%d WHERE `HouseID` = %d",
		query, HouseInfo[i][hGun4], HouseInfo[i][hGun5], HouseInfo[i][hGun6], HouseInfo[i][hGun7], HouseInfo[i][hTag], HouseInfo[i][hLevel], HouseInfo[i][hFreeze], HouseInfo[i][hDonated], HouseInfo[i][hHasFurniture], HouseInfo[i][hSQLID]);
		mysql_tquery(sqlGameConnection, query);
		HouseInfo[i][hModified] = 0;
		houseCountSaved++;
	}
	printf("[HouseInfo] %d houses were saved.", houseCountSaved);	
	return 1;
}
//===========================[ Useful functions ]===============================

//simple getter
public GetHouseVirtualWorld(houseid) {
	if(!(0 < houseid < MAX_HOUSES)) return INVALID_HOUSE_ID;
	return HouseInfo[houseid][hSQLID];
}

stock SetHouseTag(hid)
{
	new
		newhousetag[180], location[60];
	Get2DZone(location, MAX_ZONE_NAME, HouseInfo[hid][hOutsideX], HouseInfo[hid][hOutsideY], HouseInfo[hid][hOutsideZ]);
	if(HouseInfo[hid][hOwned] == 1)
	{
		format(newhousetag, sizeof(newhousetag), "{5BC435}%s{FFFFFF}\nAddress: %d %s\nPost code: %d\nType /enter to go inside.", HouseInfo[hid][hTag], hid, location, HouseInfo[hid][hSQLID]);
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
	for(new i = 1; i < MAX_HOUSES; i++)
 	{
   		if(HouseInfo[i][hOwnerID] == sqlid)
  		{
  		    print("house ID obtained.");
      		return i;
		}
 	}
	return 0;
}
stock GetHouseIDFromSQLID(sqlid)
{
	for(new i = 0; i < MAX_HOUSES; i++)
 	{
   		if(HouseInfo[i][hSQLID] == sqlid)
  		{
      		return i;
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
    for(new i = 1; i < sizeof(HouseInfo); i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 1.0, HouseInfo[i][hOutsideX], HouseInfo[i][hOutsideY], HouseInfo[i][hOutsideZ]))
		{
		    return i;
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
	HouseInfo[houseid][hCannabis] = 0;
	HouseInfo[houseid][hDuffleBag] = 0;
	HouseInfo[houseid][hCocaine] = 0;
	HouseInfo[houseid][hMeth] = 0;
	HouseInfo[houseid][hGun1] = 0;
	HouseInfo[houseid][hGun2] = 0;
	HouseInfo[houseid][hGun3] = 0;
	HouseInfo[houseid][hGun4] = 0;
	HouseInfo[houseid][hGun5] = 0;
	HouseInfo[houseid][hGun6] = 0;
	HouseInfo[houseid][hGun7] = 0;
	HouseInfo[houseid][hOwnerID] = -1;
	strdel(HouseInfo[houseid][hTag], 0, strlen(HouseInfo[houseid][hTag]));
	strins(HouseInfo[houseid][hTag], "House", 0);
	HouseInfo[houseid][hLevel] = 0;
	HouseInfo[houseid][hDonated] = 0;
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
	for(new i = 1; i < MAX_HOUSES; i++)
	{
	    if(HouseInfo[i][hOutsideX] == 0.0 && HouseInfo[i][hOutsideY] == 0.0)
	    {
	        DeleteHouse(i);
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
		format(query, sizeof(query), "INSERT INTO `houses` (`OwnerID`, `Created`, `Owned`, `Value`, `OutsideX`, `OutsideY`, `OutsideZ`, `OutsideA`, `InsideX`, `InsideY`, `InsideZ`, `InsideA`, `Interior`, `Tag`, `Level`)");
		format(query, sizeof(query), "%s VALUES(-1, 1, 0, %d, %f, %f, %f, %f, %f, %f, %f, %f, %d, 'House', 2)", query, price, X, Y, Z, A, HouseInfo[houseid][hInsideX], HouseInfo[houseid][hInsideY], HouseInfo[houseid][hInsideZ], HouseInfo[houseid][hInsideA], HouseInfo[houseid][hInterior]);
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
			foreach( new i: Player )
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
			foreach( new i: Player )
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

			SetHouseOnSale(houseid);
			format(str, sizeof(str), "You have reset houseid %d.", houseid);
			SendClientMessage(playerid, COLOR_GRAD1, str);
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

forward SetHouseOnSale(houseid);
public SetHouseOnSale(houseid)
{
	HouseInfo[houseid][hOwned] = 0;
	HouseInfo[houseid][hLocked] = 0;
	HouseInfo[houseid][hCash] = 0;
	HouseInfo[houseid][hMaterials] = 0;
	HouseInfo[houseid][hCannabis] = 0;
	HouseInfo[houseid][hDuffleBag] = 0;
	HouseInfo[houseid][hCocaine] = 0;
	HouseInfo[houseid][hMeth] = 0;
	HouseInfo[houseid][hAlarm] = 0;
	HouseInfo[houseid][hAlarmStatus] = 0;
	HouseInfo[houseid][hGun1] = 0;
	HouseInfo[houseid][hGun2] = 0;
	HouseInfo[houseid][hGun3] = 0;
	HouseInfo[houseid][hGun4] = 0;
	HouseInfo[houseid][hGun5] = 0;
	HouseInfo[houseid][hGun6] = 0;
	HouseInfo[houseid][hGun7] = 0;
	HouseInfo[houseid][hOwnerID] = -1;

	// if the value is less than 1m, set it to 1.5m
	// this must be before SetHouseTag so we can assure it displays the correct price
	if(HouseInfo[houseid][hValue] < 100000) HouseInfo[houseid][hValue] = 1500000;

	strdel(HouseInfo[houseid][hTag], 0, strlen(HouseInfo[houseid][hTag]));
	strins(HouseInfo[houseid][hTag], "House", 0);
	SetHouseTag(houseid);
	ClearHouseFurniture(houseid);

	HouseInfo[houseid][hHasFurniture] = 0;
	HouseInfo[houseid][hModified] = 1;
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
					
					new bool:gates = false;
					for(new gt =1; gt <MAX_GATES; gt++)
					{
						if(Gates[gt][GateCreated] == 1)
						{
							if(Gates[gt][GateHouseID] == HouseInfo[i][hSQLID])
							{
								format(str, sizeof(str), "Gate ID: %d", gt);
								SendClientMessage(playerid, COLOR_GRAD1, str);
								gates = true;
							}
						}
					}
					if(gates == false) SendClientMessage(playerid, COLOR_GRAD1, "There is no gates connected to this house.");
					return 1;
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
			SendClientMessage(playerid, COLOR_GRAD2, "ACTIONS: entrance, interior, value, freeze, exit");
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
			if(value < 0) return SendClientMessage(playerid, COLOR_GRAD1, "You can't set a house price lower than 0!");
			HouseInfo[houseid][hValue] = value;
            SetHouseTag(houseid);
			format(str, sizeof(str), "You have edited houseid %d's value to $%d.", houseid, value);
			SendClientMessage(playerid, COLOR_GRAD1, str);
			HouseInfo[houseid][hModified] = 1;
			return 1;
		}
		else if(strcmp(tmp, "freeze", true) == 0)
		{
			new frozen;
			if(sscanf(params, "s[128] dd", tmp2, houseid, frozen))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /edithouse freeze [houseid] [1/0]");
				return 1;
			}
			if(HouseInfo[houseid][hCreated] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That house has not even been created yet.");
			HouseInfo[houseid][hFreeze] = frozen;
			format(str, sizeof(str), "You have set houseid %d's frozen to %d.", houseid, frozen);
			SendClientMessage(playerid, COLOR_GRAD1, str);
			
			HouseInfo[houseid][hModified] = 1;
			return 1;
		}
		else if(strcmp(tmp, "exit", true) == 0)
		{
			if(sscanf(params, "s[8] d", tmp2, houseid))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /edithouse exit [houseid]");
				return 1;
			}
			new Float:X, Float:Y, Float:Z, Float:A;
			GetPlayerPos(playerid, X, Y, Z);
			GetPlayerFacingAngle(playerid, A);
			if(HouseInfo[houseid][hCreated] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That house has not even been created yet.");
			HouseInfo[houseid][hInsideX] = X;
			HouseInfo[houseid][hInsideY] = Y;
			HouseInfo[houseid][hInsideZ] = Z;
			HouseInfo[houseid][hInsideA] = A;
			format(str, sizeof(str), "You have moved houseid %d's exit position.", houseid);
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
	if(PlayerInfo[playerid][pAdmin] >=4 || PlayerInfo[playerid][pBizMod] == 1 || PlayerInfo[playerid][pDev] > 1)
	{
		new interiorid;
		if(sscanf(params, "i", interiorid)) {
			format(str, sizeof(str), "{00BFFF}Usage:{FFFFFF} /viewhouseint [0-%d]", MAX_HOUSE_INTERIORS);
			SendClientMessage(playerid, COLOR_GREY, str);
			return 1;
		}
		if(interiorid < 0 || interiorid > MAX_HOUSE_INTERIORS - 1)
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
		HouseInfo[houseid][hCannabis] = 0;
		HouseInfo[houseid][hDuffleBag] = 0;
		HouseInfo[houseid][hCocaine] = 0;
		HouseInfo[houseid][hMeth] = 0;
		HouseInfo[houseid][hAlarm] = 0;
		HouseInfo[houseid][hAlarmStatus] = 0;
		HouseInfo[houseid][hGun1] = 0;
		HouseInfo[houseid][hGun2] = 0;
		HouseInfo[houseid][hGun3] = 0;
		HouseInfo[houseid][hGun4] = 0;
		HouseInfo[houseid][hGun5] = 0;
		HouseInfo[houseid][hGun6] = 0;
		HouseInfo[houseid][hGun7] = 0;
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
			SendClientMessage(playerid, COLOR_GRAD2, "Items: dufflebag, gun, cash, materials, cannabis, cocaine, meth, xanax, promethazine, codeine, lean");
			return 1;
		}
		if(strcmp(action[1], "gun", true) == 0)
		{
			if(GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK) return SendClientMessage(playerid, COLOR_GREY, "You cannot use this at this time");
			if(IsACop(playerid) == 1 || IsANG(playerid) == 1 || IsAMedic(playerid) == 1 || IsASenate(playerid) || PlayerInfo[playerid][pMember] == 10 || PlayerInfo[playerid][pMember] == 9 || PlayerInfo[playerid][pMember] == FACTION_SANG)
			return SendClientMessage(playerid, COLOR_GRAD1, "Cops/FBI/SANG/SAN/Taxi Company can not use this command.");
			new weaponid = GetPlayerWeapon(playerid);
			if(HaveWeapon(playerid, weaponid) == weaponid && (weaponid == 22 || weaponid == 24 || weaponid == 25 || weaponid == 27 || weaponid == 29 || weaponid == 31 || weaponid == 30 || weaponid == 33 || weaponid == 34 || weaponid == 28 || weaponid == 32 || weaponid == 26 || weaponid == 4))
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
				else if(HouseInfo[houseid][hGun4] == 0)
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
				else if(HouseInfo[houseid][hGun5] == 0)
				{
					if(PlayerInfo[playerid][pMask] == 0)
					{
						// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
						format(str, sizeof(str), "* %s stores a %s into their safe.", PlayerICName(playerid), weaponname);
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					}
					format(str, sizeof(str), "* You have stored a %s.", weaponname);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					HouseInfo[houseid][hGun5] = weaponid;
					TakePlayerWeapon(playerid, weaponid);
					SetPlayerArmedWeapon(playerid, 0);
					HouseInfo[houseid][hModified] = 1;
					return 1;
				}
				else if(HouseInfo[houseid][hGun6] == 0)
				{
					if(PlayerInfo[playerid][pMask] == 0)
					{
						// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
						format(str, sizeof(str), "* %s stores a %s into their safe.", PlayerICName(playerid), weaponname);
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					}
					format(str, sizeof(str), "* You have stored a %s.", weaponname);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					HouseInfo[houseid][hGun6] = weaponid;
					TakePlayerWeapon(playerid, weaponid);
					SetPlayerArmedWeapon(playerid, 0);
					HouseInfo[houseid][hModified] = 1;
					return 1;
				}
				else if(HouseInfo[houseid][hGun7] == 0)
				{
					if(PlayerInfo[playerid][pMask] == 0)
					{
						// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
						format(str, sizeof(str), "* %s stores a %s into their safe.", PlayerICName(playerid), weaponname);
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					}
					format(str, sizeof(str), "* You have stored a %s.", weaponname);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					HouseInfo[houseid][hGun7] = weaponid;
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
		if(strcmp(action[1], "cannabis", true) == 0 || strcmp(action[1], "weed", true) == 0)
		{
			new cannabisam = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], cannabisam)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house store cannabis [amount]");
			if(cannabisam < 1 || cannabisam > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(cannabisam > PlayerInfo[playerid][pCannabis]) return SendClientMessage(playerid, COLOR_GRAD1, "You don't have that much cannabis.");
			if(HouseInfo[houseid][hCannabis] +cannabisam > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe is at or greater than it's cannabis limit.");
			HouseInfo[houseid][hCannabis] += cannabisam;
			PlayerInfo[playerid][pCannabis] -= cannabisam;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				format(str, sizeof(str), "* %s takes out some cannabis and puts it into their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have stored %d cannabis into your safe.", cannabisam);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		if(strcmp(action[1], "cocaine", true) == 0)
		{
			new cokeam = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], cokeam)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house store cocaine [amount]");
			if(cokeam < 1 || cokeam > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(cokeam > PlayerInfo[playerid][pCocaine]) return SendClientMessage(playerid, COLOR_GRAD1, "You don't have that much cocaine.");
			if(HouseInfo[houseid][hCocaine] +cokeam > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe is at or greater than it's cocaine limit.");
			HouseInfo[houseid][hCocaine] += cokeam;
			PlayerInfo[playerid][pCocaine] -= cokeam;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
				format(str, sizeof(str), "* %s takes out some cocaine and puts it into their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have stored %d cocaine into your safe.", cokeam);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		if(strcmp(action[1], "dufflebag", true) == 0)
		{
			if(!IsPlayerHoldingDuffleBag(playerid)) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You do not have a duffle bag in hand.");
			if(HouseInfo[houseid][hDuffleBag] != 0) return SendClientMessage(playerid, COLOR_GRAD1, "There is already a duffle bag in your house.");
			new duffleid = HoldingDufflebagID(playerid);
			HouseInfo[houseid][hDuffleBag] = 1;
		    RemoveDufflebagFromPlayer(playerid, duffleid); 
			if(PlayerInfo[playerid][pMask] == 0)
			{
				format(str, sizeof(str), "* %s puts his duffle bag into the safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have stored a duffle bag into your safe.");
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		if(strcmp(action[1], "meth", true) == 0)
		{
			new metham = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], metham)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house store meth [amount]");
			if(metham < 1 || metham > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(metham > PlayerInfo[playerid][pMeth]) return SendClientMessage(playerid, COLOR_GRAD1, "You don't have that much meth.");
			if(HouseInfo[houseid][hMeth] + metham > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe is at or greater than it's meth limit.");
			HouseInfo[houseid][hMeth] += metham;
			PlayerInfo[playerid][pMeth] -= metham;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
				format(str, sizeof(str), "* %s takes out some meth and puts it into their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have stored %d meth into your safe.", metham);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		if(strcmp(action[1], "xanax", true) == 0)
		{
			new xanam = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], xanam)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house store xanax [amount]");
			if(xanam < 1 || xanam > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(xanam > PlayerInfo[playerid][pXanax]) return SendClientMessage(playerid, COLOR_GRAD1, "You don't have that much xanax.");
			if(HouseInfo[houseid][hXanax] + xanam > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe is at or greater than it's xanax limit.");
			HouseInfo[houseid][hXanax] += xanam;
			PlayerInfo[playerid][pXanax] -= xanam;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
				format(str, sizeof(str), "* %s takes out some xanax and puts it into their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have stored %d xanax into your safe.", xanam);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		if(strcmp(action[1], "promethazine", true) == 0)
		{
			new prometham = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], prometham)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house store promethazine [amount]");
			if(prometham < 1 || prometham > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(prometham > PlayerInfo[playerid][pPromethazine]) return SendClientMessage(playerid, COLOR_GRAD1, "You don't have that much promethazine.");
			if(HouseInfo[houseid][hPromethazine] + prometham > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe is at or greater than it's promethazine limit.");
			HouseInfo[houseid][hPromethazine] += prometham;
			PlayerInfo[playerid][pPromethazine] -= prometham;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
				format(str, sizeof(str), "* %s takes out some promethazine and puts it into their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have stored %d promethazine into your safe.", prometham);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		if(strcmp(action[1], "codeine", true) == 0)
		{
			new codeineam = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], codeineam)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house store codeine [amount]");
			if(codeineam < 1 || codeineam > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(codeineam > PlayerInfo[playerid][pCodeine]) return SendClientMessage(playerid, COLOR_GRAD1, "You don't have that much codeine.");
			if(HouseInfo[houseid][hCodeine] + codeineam > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe is at or greater than it's codeine limit.");
			HouseInfo[houseid][hCodeine] += codeineam;
			PlayerInfo[playerid][pCodeine] -= codeineam;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
				format(str, sizeof(str), "* %s takes out some codeine and puts it into their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have stored %d codeine into your safe.", codeineam);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		if(strcmp(action[1], "lean", true) == 0)
		{
			new leanam = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], leanam)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house store lean [amount]");
			if(leanam < 1 || leanam > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(leanam > PlayerInfo[playerid][pLean]) return SendClientMessage(playerid, COLOR_GRAD1, "You don't have that much lean.");
			if(HouseInfo[houseid][hLean] + leanam > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe is at or greater than it's lean limit.");
			HouseInfo[houseid][hLean] += leanam;
			PlayerInfo[playerid][pLean] -= leanam;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
				format(str, sizeof(str), "* %s takes out some lean and puts it into their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have stored %d lean into your safe.", leanam);
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
			if(casham < 1 || casham > 2000000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(casham > PlayerInfo[playerid][pCash]) return SendClientMessage(playerid, COLOR_GRAD1, "You don't have that much cash.");
			if(HouseInfo[houseid][hCash] +casham > 2000000) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe is at or greater than it's cash limit.");
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
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house get [item]");
			SendClientMessage(playerid, COLOR_GRAD2, "Items: dufflebag, gun, cash, materials, cannabis, cocaine, xanax, promethazine, lean");
			return 1;
		}
		if(strcmp(action[1], "gun", true) == 0)
		{ 
		    if(PlayerInfo[playerid][pWeaponRestrict] == 1) SendClientMessage(playerid, COLOR_GRAD1, "{FF0000}Error: {FFFFFF}You're restricted from holding any weapon.");
			if(IsACop(playerid) == 1 || IsANG(playerid) == 1 || IsAMedic(playerid) == 1 || IsASenate(playerid) || PlayerInfo[playerid][pMember] == 10 || PlayerInfo[playerid][pMember] == 9)
			return SendClientMessage(playerid, COLOR_GRAD1, "Cops/FBI/SANG/SAN/Taxi Company can not use this command.");
			new slot = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], slot))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house get gun [1-7]");
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
			if(slot == 5)
			{
				if(HouseInfo[houseid][hGun5] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That slot is empty.");
				new weaponid = HouseInfo[houseid][hGun5];
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
				HouseInfo[houseid][hGun5] = 0;
				HouseInfo[houseid][hModified] = 1;
				return 1;
			}
			if(slot == 6)
			{
				if(HouseInfo[houseid][hGun6] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That slot is empty.");
				new weaponid = HouseInfo[houseid][hGun6];
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
				HouseInfo[houseid][hGun6] = 0;
				HouseInfo[houseid][hModified] = 1;
				return 1;
			}
			if(slot == 7)
			{
				if(HouseInfo[houseid][hGun7] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That slot is empty.");
				new weaponid = HouseInfo[houseid][hGun7];
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
				HouseInfo[houseid][hGun7] = 0;
				HouseInfo[houseid][hModified] = 1;
				return 1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GRAD1, "Invalid weapon slot.");
				return 1;
			}
		}
		if(strcmp(action[1], "cannabis", true) == 0 || strcmp(action[1], "weed", true) == 0)
		{
			new cannabisam = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], cannabisam))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house get cannabis [amount]");
				return 1;
			}
			if(cannabisam < 1 || cannabisam > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(cannabisam > HouseInfo[houseid][hCannabis]) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe doesn't have that much cannabis.");
			HouseInfo[houseid][hCannabis] -= cannabisam;
			PlayerInfo[playerid][pCannabis] += cannabisam;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
				format(str, sizeof(str), "* %s takes some cannabis from their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have taken %d cannabis from your safe.", cannabisam);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		if(strcmp(action[1], "cocaine", true) == 0)
		{
			new cokeam = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], cokeam))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house get cocaine [amount]");
				return 1;
			}
			if(cokeam < 1 || cokeam > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(cokeam > HouseInfo[houseid][hCocaine]) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe doesn't have that much cocaine.");
			HouseInfo[houseid][hCocaine] -= cokeam;
			PlayerInfo[playerid][pCocaine] += cokeam;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
				format(str, sizeof(str), "* %s takes some cocaine from their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have taken %d cocaine from your safe.", cokeam);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		if(strcmp(action[1], "dufflebag", true) == 0)
		{
			if(IsPlayerHoldingDuffleBag(playerid)) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You're currently holding a duffle bag in your hand.");
			new target;
		    new duffleid = HouseInfo[houseid][hDuffleBag]; // ID of the duffle bag in the house
		    if(HouseInfo[houseid][hDuffleBag] <= 0) return SendClientMessage(playerid, COLOR_GRAD1, "{FF0000}Error: {FFFFFF}There is no duffle bag in your safe.");
		    HouseInfo[houseid][hDuffleBag] = 0;
		    AttachDufflebagToPlayer(playerid, duffleid);
			if(PlayerInfo[playerid][pMask] == 0)
			{
				format(str, sizeof(str), "* %s takes the duffle bag from their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have taken the duffle bag from your safe.");
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		if(strcmp(action[1], "meth", true) == 0)
		{
			new metham = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], metham))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house get meth [amount]");
				return 1;
			}
			if(metham < 1 || metham > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(metham > HouseInfo[houseid][hMeth]) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe doesn't have that much meth.");
			HouseInfo[houseid][hMeth] -= metham;
			PlayerInfo[playerid][pMeth] += metham;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
				format(str, sizeof(str), "* %s takes some meth from their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have taken %d meth from your safe.", metham);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		if(strcmp(action[1], "xanax", true) == 0)
		{
			new xanam = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], xanam))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house get xanax [amount]");
				return 1;
			}
			if(xanam < 1 || xanam > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(xanam > HouseInfo[houseid][hXanax]) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe doesn't have that much xanax.");
			HouseInfo[houseid][hXanax] -= xanam;
			PlayerInfo[playerid][pXanax] += xanam;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
				format(str, sizeof(str), "* %s takes some xanax from their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have taken %d xanax from your safe.", xanam);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		if(strcmp(action[1], "promethazine", true) == 0)
		{
			new prometham = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], prometham))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house get promethazine [amount]");
				return 1;
			}
			if(prometham < 1 || prometham > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(prometham > HouseInfo[houseid][hPromethazine]) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe doesn't have that much promethazine.");
			HouseInfo[houseid][hPromethazine] -= prometham;
			PlayerInfo[playerid][pPromethazine] += prometham;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
				format(str, sizeof(str), "* %s takes some promethazine from their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have taken %d promethazine from your safe.", prometham);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		if(strcmp(action[1], "codeine", true) == 0)
		{
			new codeineam = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], codeineam))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house get codeine [amount]");
				return 1;
			}
			if(codeineam < 1 || codeineam > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(codeineam > HouseInfo[houseid][hCodeine]) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe doesn't have that much codeine.");
			HouseInfo[houseid][hCodeine] -= codeineam;
			PlayerInfo[playerid][pCodeine] += codeineam;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
				format(str, sizeof(str), "* %s takes some codeine from their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have taken %d codeine from your safe.", codeineam);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			HouseInfo[houseid][hModified] = 1;
		}
		if(strcmp(action[1], "lean", true) == 0)
		{
			new codeineam = 0;
			if(sscanf(params, "s[32] s[32] d", action[0], action[1], codeineam))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /house get lean [amount]");
				return 1;
			}
			if(codeineam < 1 || codeineam > 15000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
			if(codeineam > HouseInfo[houseid][hLean]) return SendClientMessage(playerid, COLOR_GRAD1, "Your safe doesn't have that much lean.");
			HouseInfo[houseid][hLean] -= codeineam;
			PlayerInfo[playerid][pLean] += codeineam;
			if(PlayerInfo[playerid][pMask] == 0)
			{
				// The str only gets sent if the player does NOT have a mask on due to the fact someone could reveal a hitman by knowing the owner of the house
				format(str, sizeof(str), "* %s takes some lean from their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			format(str, sizeof(str), "* You have taken %d lean from your safe.", codeineam);
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
			if(casham < 1 || casham > 2000000) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid amount.");
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

CMD:ahouse(playerid, params[])
{
	if(!(PlayerInfo[playerid][pAdmin] > 2 || PlayerInfo[playerid][pBizMod])) return AdmErrorMsg;
	new houseid;
	if(sscanf(params, "i", houseid)) return SendClientMessage(playerid, COLOR_GRAD1, "Usage: /ahouse [houseid]");
	if(HouseInfo[houseid][hCreated] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That house hasn't been created yet.");
	DisplaySafeInfo(houseid, playerid);
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
			DestroyDynamicObjectEx(gForSales[playerid][Saleobject]);
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
	new hdebug[256];
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

		/*format(str, sizeof(str), "%s has purchased a house in %s.", PlayerICName(playerid), location);
		UpdateGameFeed(str, GAMEFEED_COLOR_GREEN);*/
		format(hdebug, sizeof(hdebug), "%s has bought house ID %d [%s] for %d.", PlayerName(playerid), houseid, location, HouseInfo[houseid][hValue]);
		Log:debug("HOUSE", "%s", hdebug);

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
	if(HouseInfo[houseid][hDonated]) return SendClientMessage(playerid, COLOR_GRAD1, "You can not sell this house as it is donated.");
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

CMD:sethousedonated(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 1337 && !PlayerInfo[playerid][pBizMod]) return AdmErrorMsg;
	new houseid;
	if(sscanf(params, "d", houseid)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /sethousedonated [houseid]");
	if(!HouseInfo[houseid][hCreated]) return SendClientMessage(playerid, COLOR_GRAD1, "That house has not been created yet! ");

	HouseInfo[houseid][hDonated] = !HouseInfo[houseid][hDonated];

	new msg[100];
	if(HouseInfo[houseid][hDonated])
	{
		format(msg, sizeof(msg), "House ID %d is now donated and can not be sold.", houseid);
		SendClientMessage(playerid, COLOR_GREY, msg);
	}
	else
	{
		format(msg, sizeof(msg), "House ID %d is no longer donated and can be sold.", houseid);
		SendClientMessage(playerid, COLOR_GREY, msg);
	}

	return 1;
}

stock SetHouseInterior(houseid, level)
{
	HouseInfo[houseid][hInterior] = houseInteriorInfo[level][hInteriorID];
	HouseInfo[houseid][hLevel] = houseInteriorInfo[level][hLevel];
	HouseInfo[houseid][hInsideX] = houseInteriorInfo[level][hX];
	HouseInfo[houseid][hInsideY] = houseInteriorInfo[level][hY];
	HouseInfo[houseid][hInsideZ] = houseInteriorInfo[level][hZ];
	HouseInfo[houseid][hInsideA] = houseInteriorInfo[level][hA];
	HouseInfo[houseid][hFreeze] = houseInteriorInfo[level][hFreeze];
	HouseInfo[houseid][hModified] = 1;
	return 1;
}

stock SetPlayerInHouseInterior(playerid, level)
{
	PlayerInfo[playerid][pInt] = houseInteriorInfo[level][hInteriorID];
	SetPlayerInterior(playerid, PlayerInfo[playerid][pInt]);
	SetPlayerPos(playerid, houseInteriorInfo[level][hX], houseInteriorInfo[level][hY], houseInteriorInfo[level][hZ]);
	SetPlayerFacingAngle(playerid, houseInteriorInfo[level][hA]);
	return 1;
}

IsPlayerNearHouse(playerid)
{
	for(new i = 0; i < MAX_HOUSES; i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 1.0, HouseInfo[i][hOutsideX], HouseInfo[i][hOutsideY], HouseInfo[i][hOutsideZ]))
		{
			return i;
		}
	}
	return -1;
}