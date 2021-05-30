/*
 __  __                                    
/\ \/\ \                                   
\ \ \/'/'     __   __  __     __    ___    
 \ \ , <    /'__`\/\ \/\ \  /'__`\/' _ `\  
  \ \ \\`\ /\  __/\ \ \_/ |/\  __//\ \/\ \ 
   \ \_\ \_\ \____\\ \___/ \ \____\ \_\ \_\
    \/_/\/_/\/____/ \/__/   \/____/\/_/\/_/
                                           
                                           
 ____             __    __       ___                            
/\  _`\          /\ \__/\ \     /\_ \                           
\ \ \L\ \  __  __\ \ ,_\ \ \___ \//\ \      __    ____    ____  
 \ \ ,  / /\ \/\ \\ \ \/\ \  _ `\ \ \ \   /'__`\ /',__\  /',__\ 
  \ \ \\ \\ \ \_\ \\ \ \_\ \ \ \ \ \_\ \_/\  __//\__, `\/\__, `\
   \ \_\ \_\ \____/ \ \__\\ \_\ \_\/\____\ \____\/\____/\/\____/
    \/_/\/ /\/___/   \/__/ \/_/\/_/\/____/\/____/\/___/  \/___/ 

	
	UNKNOWN DEVELOPMENT RERQUEST
*/
	
new kevruth[128];
new ruthlessvehicle;
new bool:KevListen[MAX_PLAYERS] = false;

Hook:KevenRuthless_OnGameModeInit() {
	ruthlessvehicle = AddStaticVehicleEx(560, 1963.3165, -1719.4249, 15.5500, 0.0000, 201, 201, -1);
	AddVehicleComponent(ruthlessvehicle, 1010);
	AddVehicleComponent(ruthlessvehicle, 1080);
	AddVehicleComponent(ruthlessvehicle, 1029);
	AddVehicleComponent(ruthlessvehicle, 1030);
	AddVehicleComponent(ruthlessvehicle, 1031);
	AddVehicleComponent(ruthlessvehicle, 1138);
	AddVehicleComponent(ruthlessvehicle, 1033);
	SetVehicleNumberPlate(ruthlessvehicle, "Ruthless");
	SetVehicleToRespawn(ruthlessvehicle);
	CreateDynamic3DTextLabel("{FF0000}Ruthless's Safe!\n{FF0000}(/ruthlesssafe)", COLOR_LIGHTBLUE, 1984.2822, -1736.9333, 16.6930, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
    SetTimer("KevRuthStream", 1000, true);
	return 1;
}

CMD:ruthless(playerid, params[]) {
	if(CP[playerid] != CHECKPOINT_NONE) return SendClientMessage(playerid, -1, "You already have a checkpoint set, use (/killcp) to kill it.");
	if(isnull(params)) return SendClientMessage(playerid, -1, "/ruthless [House | HQ]");
    if(!strcmp(params, "House", true, 7)) {
	    SendClientMessage(playerid, -1, "Ruthless House has been marked on your map.");
		SetPlayerCheckpointEx(playerid, 1972.5854,-1722.2434,16.5636, 3.0);
		CP[playerid] = CHECKPOINT_RUTHLESS;
	}
	else if(!strcmp(params, "HQ", true, 7)) {
    	SendClientMessage(playerid, -1, "Ruthless HQ has been marked on your map.");
		SetPlayerCheckpointEx(playerid, 1982.6362,-1682.8397,16.9317, 3.0);
		CP[playerid] = CHECKPOINT_RUTHLESS;
	}
	return 1;
}


CMD:krstream(playerid, params[]) {
	if(IsPlayerInRangeOfPoint(playerid, 2.0, 1968.04236, -1693.60791, 8023.81055)) {
		if(isnull(params)) return SendClientMessage(playerid, -1, "/krstream [link]");
		format(kevruth, sizeof(kevruth), "%s", params);
		foreach(Player, i) KevListen[i] = false;
	}
	return 1;
}

forward KevRuthStream();
public KevRuthStream() {
	foreach(Player, i) {
		if(IsPlayerInRangeOfPoint(i, 30, 1968.04236, -1693.60791, 8023.81055) && KevListen[i] == false) {
			PlayAudioStreamForPlayer(i, kevruth);
			KevListen[i] = true;
		}
	}
	return 1;
}
