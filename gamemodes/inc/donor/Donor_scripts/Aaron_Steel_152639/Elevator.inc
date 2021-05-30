// Every file which uses y_hooks must have a seperate include for it //
#include <YSI\y_hooks>

// An array which holds all elevator point //
static Float:_elevatorPoints[][] = {

	{823.63, 58.31, 1901.08},
	{820.46, 58.27, 1915.87},
	{1512.06, -1130.68, 71.18}
};

// A function which returns if a player is near the elevator spot or not //
GetSteelElevatorID(playerid){
	for(new i = 0; i < sizeof(_elevatorPoints); i++){
		if(IsPlayerInRangeOfPoint(playerid, 2.0, _elevatorPoints[i][0], _elevatorPoints[i][1], _elevatorPoints[i][2]))
			return i;
	}
	return -1;
}


// Creating 3D text labels for the elevator points //
hook OnGameModeInit(){

	for(new i = 0, text[50]; i < sizeof(_elevatorPoints); i++){
		format(text, sizeof(text), "Floor %d", i+1);
		CreateDynamic3DTextLabel(text, -1, _elevatorPoints[i][0], _elevatorPoints[i][1], _elevatorPoints[i][2], 5.0, .testlos = 1, .worldid = 0);
	}

	return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys){

	if(newkeys & KEY_YES){
		new elevatorID = GetSteelElevatorID(playerid);
		if(elevatorID != -1){


			new dialogString[128];
			for(new i = 0; i < sizeof(_elevatorPoints); i++){
				if(elevatorID == i)
					continue;
				else
					format(dialogString, sizeof(dialogString), "%sFloor %d\n", dialogString, i+1);
			}
			Dialog_ShowCallback(playerid, using public SteelElevatorResponse, DIALOG_STYLE_LIST, "Elevator", dialogString, "Ok", "Exit");
		}
	}
	return 1;
}

forward SteelElevatorResponse(playerid, dialogid, response, listitem, inputtext[]);
public SteelElevatorResponse(playerid, dialogid, response, listitem, inputtext[]){

	new elevatorID = GetSteelElevatorID(playerid);
	if(elevatorID == -1)
		return SendClientMessage(playerid, COLOR_GREY, "You are not near any elevator");
	
	if(listitem >= elevatorID){
		listitem++;
	}

	switch(elevatorID){
		case 0: displayCenterHUDInfo(playerid, "~r~Floor 1~w~ Lobby", 5);
		case 1: 
			{
				displayCenterHUDInfo(playerid, "~r~Floor 2~w~ Meeting Room", 5);
				SetPlayerInterior(playerid, 1);
				SetPlayerVirtualWorld(playerid, 5);
			}


		case 2: 
			{
				displayCenterHUDInfo(playerid, "~r~Floor 3~w~ Office", 5);
				SetPlayerInterior(playerid, 0);
				SetPlayerVirtualWorld(playerid, 5);
				Timer_FreezePlayer(playerid, FREEZE, ENTER_FREEZE);
			}

		default: displayCenterHUDInfo(playerid, "UNKNOWN", 5);
	}

	SetPlayerPos(playerid, _elevatorPoints[listitem][0], _elevatorPoints[listitem][1], _elevatorPoints[listitem][2]);


	return 1;
}