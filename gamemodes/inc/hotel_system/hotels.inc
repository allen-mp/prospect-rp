// This function begins to load all hotels
forward LoadHotels();
public LoadHotels() {
	printf("[Hotel System] Query made to fetch hotels...");

	new hotelQuery[100];
	mysql_format(sqlGameConnection, hotelQuery, sizeof(hotelQuery), "SELECT * FROM `hotels`");
	mysql_pquery(sqlGameConnection, hotelQuery, "OnHotelsLoaded");
	return 1;
}

// Called when all hotels have been loaded
forward OnHotelsLoaded();
public OnHotelsLoaded() {
	new hotelRows = cache_num_rows();

	if(hotelRows < 1) {
		printf("[Hotel System] No hotels found in the database.");
		return 1;
	}
	
	printf("[Hotel System] %i hotels found. Start loading...", hotelRows);

	for(new i = 0; i < hotelRows; i++) {
		new foundhID = cache_get_field_content_int(i, "ID");
		new foundhOwnerID = cache_get_field_content_int(i, "OwnerID");
		new foundhName[MAX_HOTEL_NAME_SIZE];
		cache_get_field_content(i, "Name", foundhName);

		CreateHotel(foundhID, foundhOwnerID, foundhName);
	}
	return 1;
}

// This function saves all hotels
SaveHotels() {
	for(new i = 0; i < MAX_HOTELS; i++) {
		if(HotelInfo[i][hUsed]) SaveHotel(i);
	}
	return 1;
}

// This function saves a hotel room
SaveHotel(hotelID) {
	if(!HotelInfo[hotelID][hUsed])
		return false;

	new hotelSaveQuery[500];
	format(hotelSaveQuery, sizeof(hotelSaveQuery),
		"REPLACE INTO `hotels`\
			(ID,\
			OwnerID,\
			Name)\
		VALUES(%d, %d, %e)",
		HotelInfo[hotelID][hSQLID],
		HotelInfo[hotelID][hOwnerID],
		HotelInfo[hotelID][hName]
	);
	mysql_pquery(sqlGameConnection, hotelSaveQuery);

	printf("[Hotel System] Hotel \"%s\" saved. (SQL %i)", HotelInfo[hotelID][hName], HotelInfo[hotelID][hSQLID]);
	return 1;
}

// Create a new hotel
CreateHotel(sethID, sethOwnerID, sethName[]) {
	// Find an available hotel
	new availableHotel = -1;
	for(new i = 0; i < MAX_HOTELS; i++) {
		if(!HotelInfo[i][hUsed]) {
			availableHotel = i;
			break;
		}
	}

	if(availableHotel == -1) {
		printf("Max amount of hotels reached! (%i)", MAX_HOTELS);
		return -1;
	}

	HotelInfo[availableHotel][hUsed] = true;
	HotelInfo[availableHotel][hSQLID] = sethID;
	HotelInfo[availableHotel][hOwnerID] = sethOwnerID;
	format(HotelInfo[availableHotel][hName], MAX_HOTEL_NAME_SIZE, "%s", sethName);

	printf("[Hotel System] Hotel %i created: %s. (SQL %i)", availableHotel, sethName, sethID);
	return availableHotel;
}

// Returns the amount of hotels
GetHotelCount() {
	new hotelCount = 0;

	for(new i = 0; i < MAX_HOTELS; i++) 
		if(HotelInfo[i][hUsed]) hotelCount++;

	return hotelCount;
}

// Returns the amount of hotel rooms that a hotel has
GetHotelRoomCount(hotelSQLID) {
	new roomCount = 0;

	for(new i = 0; i < MAX_HOTELROOMS; i++)
		if(HotelRoomInfo[i][hUsed] && HotelRoomInfo[i][hHotelID] == hotelSQLID) roomCount++;

	return roomCount;
}

// Returns the name of a hotel
GetHotelName(hotelSQLID) {
	new hotelReturnName[MAX_HOTEL_NAME_SIZE];

	for(new i = 0; i < MAX_HOTELS; i++) {
		if(!HotelInfo[i][hUsed] || HotelInfo[i][hSQLID] != hotelSQLID) continue;

		format(hotelReturnName, sizeof(hotelReturnName), "%s", HotelInfo[i][hName]);
		break;
	}

	if(!strlen(hotelReturnName))
		format(hotelReturnName, sizeof(hotelReturnName), "Invalid hotel");

	return hotelReturnName;
}

// Command to show all current hotels
CMD:hotels(playerid, params[]) {
	if(PlayerInfo[playerid][pDev] < 2)
		return AdmErrorMsg;

	if(!GetHotelCount())
		return SendClientMessage(playerid, COLOR_GREY, "There are currently no hotels.");

	// Format dialog string
	new hotelStr[2048];
	format(hotelStr, sizeof(hotelStr), "* %i Hotel(s) found:\n", GetHotelCount());

	for(new i = 0; i < MAX_HOTELS; i++) {
		if(!HotelInfo[i][hUsed]) continue;

		format(hotelStr, sizeof(hotelStr), "%s\n{FFD830}[%i] \"%s\", %i rooms.", hotelStr, i, HotelInfo[i][hName], GetHotelRoomCount(HotelInfo[i][hSQLID]));
	}

	// Show the dialog
	ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "{FFD830}List of hotels", hotelStr, "Close", "");
	return 1;
}

// Command to create a new hotel
CMD:createhotel(playerid, params[]) {
	if(PlayerInfo[playerid][pDev] < 2)
		return AdmErrorMsg;

	new newHotelName[MAX_HOTEL_NAME_SIZE];
	if(sscanf(params, "s[50]", newHotelName))
		return SendClientMessage(playerid, COLOR_WHITE, "{00BFFF}USAGE:{FFFFFF} /createhotel [hotel name]");

	if(!strlen(newHotelName))
		return SendClientMessage(playerid, COLOR_GREY, "Enter a valid hotel name.");

	// Check if we can make a hotel at this moment
	new freeHotelSlot;

	for(new i = 0; i < MAX_HOTELS; i++) {
		if(!HotelInfo[i][hUsed]) {
			freeHotelSlot = i;
			break;
		}
	}

	if(freeHotelSlot == -1)
		return SendClientMessage(playerid, COLOR_GREY, "The maximum number of hotels has been reached.");
	
	// Insert a new hotel into the DB
	new hotelQuery[128];
	mysql_format(sqlGameConnection, hotelQuery, sizeof(hotelQuery), "INSERT INTO `hotels`(`Name`) VALUES('%e');", newHotelName);
	mysql_pquery(sqlGameConnection, hotelQuery, "OnHotelCreated", "is", playerid, newHotelName);
	return 1;
}

// Called when a hotel is created
forward OnHotelCreated(playerid, hotelName[]);
public OnHotelCreated(playerid, hotelName[]) {
	// Create the new hotel in-game
	new theHSQLID = cache_insert_id(sqlGameConnection);
	new createdHotelID = CreateHotel(theHSQLID, 0, hotelName);

	// Send message
	new hotelStr[128];
	format(hotelStr, sizeof(hotelStr), "* You have successfully created the hotel: \"%s\". (ID %i)", hotelName, createdHotelID);
	SendClientMessage(playerid, COLOR_LIGHTBLUE, hotelStr);
	return 1;
}

// Command to edit a hotel
CMD:edithotel(playerid, params[]) {
	if(PlayerInfo[playerid][pDev] < 2)
		return AdmErrorMsg;

	new hotelID, editOption[20];
	if(sscanf(params, "is[20]", hotelID, editOption)) {
		SendClientMessage(playerid, COLOR_WHITE, "{00BFFF}USAGE:{FFFFFF} /edithotel [hotel ID] [option]");
		SendClientMessage(playerid, COLOR_GREY, "Options: owner");
		return 1;
	}

	if(hotelID < 0 || hotelID > MAX_HOTELS)
		return SendClientMessage(playerid, COLOR_GREY, "You entered an invalid hotel ID.");

	if(!HotelInfo[hotelID][hUsed])
		return SendClientMessage(playerid, COLOR_GREY, "This hotel ID is not in use.");

	// Edit the hotel owner
	if(strmatch(editOption, "owner")) {
		new newOwner[30];

		if(sscanf(params, "is[20]s[30]", hotelID, editOption, newOwner))
			return SendClientMessage(playerid, COLOR_WHITE, "{00BFFF}USAGE:{FFFFFF} /edithotel [hotel ID] owner [playerid/partOfName/none]");
	
		// Makes the hotel "server owned"
		new newOwnerID, notifyStr[128];

		if(strmatch(newOwner, "none")) {
			if(HotelInfo[hotelID][hOwnerID] != 0)
				return SendClientMessage(playerid, COLOR_GREY, "This hotel is already owned by the server.");

			newOwnerID = 0;

			format(notifyStr, sizeof(notifyStr), "* The hotel \"%s\" (%i) is now owned by the server.", HotelInfo[hotelID][hName], hotelID);
		}
		else { // Makes a player the owner of the hotel
			new foundHotelUser = ReturnUser(newOwner);

			if(foundHotelUser == INVALID_PLAYER_ID)
				return SendClientMessage(playerid, COLOR_GREY, "Unknown player.");

			newOwnerID = PlayerInfo[foundHotelUser][pID];

			if(HotelInfo[hotelID][hOwnerID] == newOwnerID)
				return SendClientMessage(playerid, COLOR_GREY, "This hotel is already owned by that player.");

			format(notifyStr, sizeof(notifyStr), "* The hotel \"%s\" (%i) is now owned by %s.", HotelInfo[hotelID][hName], hotelID, PlayerOOCName(foundHotelUser));
		}

		// Set the owner
		HotelInfo[hotelID][hOwnerID] = newOwnerID;

		// Send the message
		SendClientMessage(playerid, COLOR_LIGHTBLUE, notifyStr);
	}
	else return cmd_edithotel(playerid, "");
	return 1;
}