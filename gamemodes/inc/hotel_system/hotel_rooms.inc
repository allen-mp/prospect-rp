// This function begins to load all hotel rooms
forward LoadHotelRooms();
public LoadHotelRooms() {
	printf("[Hotel System] Query made to fetch hotel rooms...");

	new hotelQuery[100];
	mysql_format(sqlGameConnection, hotelQuery, sizeof(hotelQuery), "SELECT * FROM `hotelrooms`");
	mysql_pquery(sqlGameConnection, hotelQuery, "OnHotelRoomsLoaded");
	return 1;
}

// Called when all hotel room data has been loaded
forward OnHotelRoomsLoaded();
public OnHotelRoomsLoaded() {
	new hotelRows = cache_num_rows();

	if(hotelRows < 1) {
		printf("[Hotel System] No hotel rooms found in the database.");
		return 1;
	}
	
	printf("[Hotel System] %i hotel rooms found. Start loading...", hotelRows);

	for(new i = 0; i < hotelRows; i++) {
		new foundhID = cache_get_field_content_int(i, "ID");
		new foundhHotelID = cache_get_field_content_int(i, "HotelID");
		new foundhOwnerID = cache_get_field_content_int(i, "OwnerID");
		new foundhVW = cache_get_field_content_int(i, "VW");
		new foundhInt = cache_get_field_content_int(i, "Interior");
		new Float:foundhPos[3];
		foundhPos[0] = cache_get_field_content_float(i, "X");
		foundhPos[1] = cache_get_field_content_float(i, "Y");
		foundhPos[2] = cache_get_field_content_float(i, "Z");
		new foundCreatedBy[MAX_PLAYER_NAME + 1];
		cache_get_field_content(i, "CreatedBy", foundCreatedBy);

		CreateHotelRoom(foundhID, foundhHotelID, foundhOwnerID, foundhPos[0], foundhPos[1], foundhPos[2], foundhVW, foundhInt, foundCreatedBy);
	}
	return 1;
}

// This function saves all hotel rooms
// this should be called when the server exits
SaveHotelRooms() {
	for(new i = 0; i < MAX_HOTELROOMS; i++) {
		if(HotelRoomInfo[i][hUsed]) SaveHotelRoom(i);
	}
	return 1;
}

// This function saves a hotel room
SaveHotelRoom(roomID) {
	if(!HotelRoomInfo[roomID][hUsed])
		return false;

	new hotelSaveQuery[500];
	format(hotelSaveQuery, sizeof(hotelSaveQuery),
		"REPLACE INTO `hotelrooms`\
			(ID,\
			OwnerID,\
			X,\
			Y,\
			Z,\
			VW,\
			Interior,\
			CreatedBy)\
		VALUES(%d, %d, %f, %f, %f, %d, %d, %e)",
		HotelRoomInfo[roomID][hSQLID],
		HotelRoomInfo[roomID][hOwnerID],
		HotelRoomInfo[roomID][hX],
		HotelRoomInfo[roomID][hY],
		HotelRoomInfo[roomID][hZ],
		HotelRoomInfo[roomID][hVW],
		HotelRoomInfo[roomID][hInt],
		HotelRoomInfo[roomID][hCreatedBy]
	);
	mysql_pquery(sqlGameConnection, hotelSaveQuery);

	printf("[Hotel System] Hotel room ID %i saved. (SQL %i)", roomID, HotelRoomInfo[roomID][hSQLID]);
	return 1;
}

// Show information related to the nearest hotel room
CMD:hroominfo(playerid, params[]) {
	if(PlayerInfo[playerid][pDev] < 2)
		return AdmErrorMsg;

	new nearRoom = GetNearbyHotelRoom(playerid, 2.0);
	if(nearRoom == INVALID_HOTEL_ROOM)
		return SendClientMessage(playerid, COLOR_GREY, "You are not near any hotel room.");

	new hInfoStr[128];
	format(hInfoStr, sizeof(hInfoStr), "[Hotel Room ID %i]", nearRoom);
	SendClientMessage(playerid, SAMP_COLOR, hInfoStr);

	format(hInfoStr, sizeof(hInfoStr), "Owner: %s (SQL %i), Created by: %s, Hotel: %s",
		HotelRoomInfo[nearRoom][hOwnerName],
		HotelRoomInfo[nearRoom][hOwnerID],
		HotelRoomInfo[nearRoom][hCreatedBy],
		GetHotelName(HotelRoomInfo[nearRoom][hHotelID])
	);
	SendClientMessage(playerid, SAMP_COLOR, hInfoStr);
	return 1;
}

// Command to teleport to a hotel room
CMD:gotohroom(playerid, params[]) {
	if(PlayerInfo[playerid][pDev] < 2)
		return AdmErrorMsg;

	new hroomID;
	if(sscanf(params, "i", hroomID))
		return SendClientMessage(playerid, COLOR_WHITE, "{00BFFF}USAGE:{FFFFFF} /gotohroom [hotel room ID]");
	
	if(hroomID < 0 || hroomID > MAX_HOTELROOMS)
		return SendClientMessage(playerid, COLOR_GREY, "You entered an invalid hotel room ID.");

	if(!HotelRoomInfo[hroomID][hUsed])
		return SendClientMessage(playerid, COLOR_GREY, "This hotel room ID does not exist.");

	// Teleport the player
	Timer_FreezePlayer(playerid, FREEZE, ENTER_FREEZE);
	SetPlayerPos(playerid, HotelRoomInfo[hroomID][hX], HotelRoomInfo[hroomID][hY], HotelRoomInfo[hroomID][hZ]);
	SetPlayerVirtualWorld(playerid, HotelRoomInfo[hroomID][hVW]);
	SetPlayerInterior(playerid, HotelRoomInfo[hroomID][hInt]);

	// Show center HUD
	new hotelStr[128];
	format(hotelStr, sizeof(hotelStr), "~w~Teleported to hotel room ~r~ID %i~w~.", hroomID);
	displayCenterHUDInfo(playerid, hotelStr, 8);
	return 1;
}

// Create a new hotel room in-game
CreateHotelRoom(sethID, sethHotelID, sethOwnerID, Float:sethX, Float:sethY, Float:sethZ, sethVW, sethInt, sethCreatedBy[]) {
	// Find an available hotel room
	new availableHotel = -1;
	for(new i = 0; i < MAX_HOTELROOMS; i++) {
		if(!HotelRoomInfo[i][hUsed]) {
			availableHotel = i;
			break;
		}
	}

	if(availableHotel == -1) {
		printf("Max amount of hotel rooms reached! (%i)", MAX_HOTELROOMS);
		return 1;
	}

	HotelRoomInfo[availableHotel][hUsed] = true;
	HotelRoomInfo[availableHotel][hSQLID] = sethID;
	HotelRoomInfo[availableHotel][hHotelID] = sethHotelID;
	HotelRoomInfo[availableHotel][hOwnerID] = sethOwnerID;
	HotelRoomInfo[availableHotel][hVW] = sethVW;
	HotelRoomInfo[availableHotel][hInt] = sethInt;
	HotelRoomInfo[availableHotel][hX] = sethX;
	HotelRoomInfo[availableHotel][hY] = sethY;
	HotelRoomInfo[availableHotel][hZ] = sethZ;
	format(HotelRoomInfo[availableHotel][hCreatedBy], MAX_PLAYER_NAME + 1, "%s", sethCreatedBy);

	RefreshHotelVisual(availableHotel, true);
	printf("[Hotel System] Hotel room %i created. (SQL %i)", availableHotel, sethID);
	return 1;
}

// Refresh the "visuals" of a hotel room (pickup, 3D text)
RefreshHotelVisual(hotelID, bool:findName = false) {
	DestroyDynamic3DTextLabel(HotelRoomInfo[hotelID][hText3D]);
	DestroyDynamicPickup(HotelRoomInfo[hotelID][hPickupID]);

	if(HotelRoomInfo[hotelID][hUsed]) {
		new displayOwner[MAX_PLAYER_NAME + 1];

		if(HotelRoomInfo[hotelID][hOwnerID] != 0) {
			if(!findName) {
				format(displayOwner, sizeof(displayOwner), "%s", HotelRoomInfo[hotelID][hOwnerName]);
			}
			else {
				new nameQuery[100];
				mysql_format(sqlGameConnection, nameQuery, sizeof(nameQuery), "SELECT Name FROM players WHERE ID = %d", HotelRoomInfo[hotelID][hOwnerID]);
				mysql_pquery(sqlGameConnection, nameQuery, "OnRoomOwnerFetched", "i", hotelID);
				return 1;
			}		
		}
		else {
			format(displayOwner, sizeof(displayOwner), "Nobody");
			format(HotelRoomInfo[hotelID][hOwnerName], MAX_PLAYER_NAME + 1, "Nobody");
		}

		new hotelStr[150];
		format(hotelStr, sizeof(hotelStr), "Hotel Room{FFFFFF}\nOwner: %s\nRoom number: %i\nType /enter to go inside.", displayOwner, hotelID);
		HotelRoomInfo[hotelID][hText3D] = CreateDynamic3DTextLabel(hotelStr, COLOR_HOTELTEXT, HotelRoomInfo[hotelID][hX], HotelRoomInfo[hotelID][hY], HotelRoomInfo[hotelID][hZ] +0.88, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, HotelRoomInfo[hotelID][hVW], HotelRoomInfo[hotelID][hInt], -1, 100);
		HotelRoomInfo[hotelID][hPickupID] = CreateDynamicPickup(HOTEL_PICKUP, 1, HotelRoomInfo[hotelID][hX], HotelRoomInfo[hotelID][hY], HotelRoomInfo[hotelID][hZ], HotelRoomInfo[hotelID][hVW]);
	}
	return 1;
}

// Called when the name of a hotel room owner is found
forward OnRoomOwnerFetched(roomID);
public OnRoomOwnerFetched(roomID) {
	new rows, fields, displayOwner[MAX_PLAYER_NAME + 1];

	cache_get_data(rows, fields);

	if(!rows) {
		format(displayOwner, sizeof(displayOwner), "Unknown");
	}
	else {
		cache_get_field_content(0, "Name", displayOwner);
	}

	format(HotelRoomInfo[roomID][hOwnerName], MAX_PLAYER_NAME + 1, "%s", displayOwner);
	printf("[Hotel System] Owner found for room ID %i (%s)", roomID, displayOwner);

	RefreshHotelVisual(roomID);
	return 1;
}

// Get the ID of the nearest hotel room to a player
GetNearbyHotelRoom(playerid, Float:hotelRange = 5.0) {
	for(new i = 0; i < MAX_HOTELROOMS; i++) {
		if(!HotelRoomInfo[i][hUsed]) continue;

		if(IsPlayerInRangeOfPoint(playerid, hotelRange, HotelRoomInfo[i][hX], HotelRoomInfo[i][hY], HotelRoomInfo[i][hZ])) {
			if(GetPlayerVirtualWorld(playerid) == HotelRoomInfo[i][hVW]) {
				if(GetPlayerInterior(playerid) == HotelRoomInfo[i][hInt])
					return i;
			}
		}
	}

	return INVALID_HOTEL_ROOM;
}
