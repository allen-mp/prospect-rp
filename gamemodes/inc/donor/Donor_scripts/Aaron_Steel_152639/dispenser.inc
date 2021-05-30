// Every file which uses y_hooks must have a seperate include for it //
#include <YSI\y_hooks>

#define STEEL_WATER_DISPENSER_COORDS 1500.58, -1126.71, 71.18


hook OnGameModeInit(){
	CreateDynamic3DTextLabel("Water Dispenser\n\nPress Y to interact", 0x00e6e6FF, STEEL_WATER_DISPENSER_COORDS, 5.0, .testlos = 1, .worldid = 0);
	return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys){

	if(IsPlayerInRangeOfPoint(playerid, 2.0, STEEL_WATER_DISPENSER_COORDS)){		
		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);
		displayCenterHUDInfo(playerid,  "LMB to drink, press F to drop.", 8); //display for 8 seconds
	}

	return 1;
}
