#define MAX_HOTELS			30
#define MAX_HOTELROOMS		500
#define HOTEL_PICKUP		19524
#define COLOR_HOTELTEXT 	0xFFD830FF
#define INVALID_HOTEL_ROOM	-1
#define MAX_HOTEL_NAME_SIZE	50

// Hotel rooms
enum hotelRoomEnum {
	bool:hUsed,
	hSQLID,
	hHotelID,
	hOwnerID,
	hOwnerName[MAX_PLAYER_NAME + 1],
	Float:hX,
	Float:hY,
	Float:hZ,
	hVW,
	hInt,
	hPickupID,
	Text3D:hText3D,
	hCreatedBy[MAX_PLAYER_NAME + 1]
};
new HotelRoomInfo[MAX_HOTELROOMS][hotelRoomEnum];

// Hotels
enum hotelEnum {
	bool:hUsed,
	hSQLID,
	hOwnerID,
	hName[MAX_HOTEL_NAME_SIZE]
};
new HotelInfo[MAX_HOTELS][hotelEnum];

// Calls when the gamemode has loaded
Hook:h_OnGamemodeInit() {
	LoadHotels();
	LoadHotelRooms();
	return 1;
}

// Save all hotel rooms once the server exits
Hook:h_OnGameModeExit() {
	SaveHotels();
	SaveHotelRooms();
	return 1;
}

// Show hotel management related commands
CMD:ahotelhelp(playerid, params[]) {
	if(PlayerInfo[playerid][pDev] < 2)
		return AdmErrorMsg;

	SendClientMessage(playerid, COLOR_HOTELTEXT, "____________________________________________");
	SendClientMessage(playerid, COLOR_WHITE, "** Hotels: /hotels, /createhotel, /edithotel");
	SendClientMessage(playerid, COLOR_WHITE, "** Hotel rooms: /hroominfo, /gotohroom");
	return 1;
}

#include "inc/hotel_system/hotels.inc"
#include "inc/hotel_system/hotel_rooms.inc"