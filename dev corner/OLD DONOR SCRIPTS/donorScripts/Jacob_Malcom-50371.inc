/*
 _____                          __        
/\___ \                        /\ \       
\/__/\ \     __      ___    ___\ \ \____  
   _\ \ \  /'__`\   /'___\ / __`\ \ '__`\ 
  /\ \_\ \/\ \L\.\_/\ \__//\ \L\ \ \ \L\ \
  \ \____/\ \__/.\_\ \____\ \____/\ \_,__/
   \/___/  \/__/\/_/\/____/\/___/  \/___/ 
                                          
                                          
                   ___                                 
 /'\_/`\          /\_ \                                
/\      \     __  \//\ \     ___    ___     ___ ___    
\ \ \__\ \  /'__`\  \ \ \   /'___\ / __`\ /' __` __`\  
 \ \ \_/\ \/\ \L\.\_ \_\ \_/\ \__//\ \L\ \/\ \/\ \/\ \ 
  \ \_\\ \_\ \__/.\_\/\____\ \____\ \____/\ \_\ \_\ \_\
   \/_/ \/_/\/__/\/_/\/____/\/____/\/___/  \/_/\/_/\/_/

   
	UNKNOWN DEVELOPMENT RERQUEST
*/
	
new jacob_factoryveh;

Hook:JacobMalcom_OnGameModeInit() {
	jacob_factoryveh = CreateVehicle(498,-2289.7437000,-175.6069000,35.3082000,91.0000000,-1,-1,900); //Boxville
	return 1;
}

CMD:jacobmalcomcmds(playerid,params[]) {
	SendClientMessage(playerid, -1, "----------[ Jacob Malcom's CMDs ]----------");
	SendClientMessage(playerid, 0xAFAFAFAA, "> /jboxville, enter/exit Boxville.");
	SendClientMessage(playerid, 0xAFAFAFAA, "> /jbc, Boxville vehicle chat (with passengers/driver).");
	return 1;
}

CMD:jboxville(playerid, params[]) { //Entering/exiting the boxville
	if(IsPlayerInRangeOfPoint(playerid, 2.0, -2302.9670,-106.8171,999.8542)) {
	    new Float:jacfacvpos[4];
	    GetVehiclePos(jacob_factoryveh, jacfacvpos[0], jacfacvpos[1], jacfacvpos[2]);
	    GetVehicleZAngle(jacob_factoryveh, jacfacvpos[3]);
	    
		new Float:jacfacvinfo[3];
	    GetVehicleModelInfo(GetVehicleModel(jacob_factoryveh), VEHICLE_MODEL_INFO_SIZE, jacfacvinfo[0], jacfacvinfo[1], jacfacvinfo[2]);
	    
		new Float:distance = (jacfacvinfo[1]/2 + 0.5);
     	jacfacvpos[0] += (distance * -floatsin(-jacfacvpos[3], degrees));
    	jacfacvpos[1] += (distance * -floatcos(-jacfacvpos[3], degrees));
    	SetPlayerPos(playerid, jacfacvpos[0], jacfacvpos[1], jacfacvpos[2]);
    	SetPlayerInterior(playerid, 0);
	}
	else {
		new Float:jacfacvpos[4];
	    GetVehiclePos(jacob_factoryveh, jacfacvpos[0], jacfacvpos[1], jacfacvpos[2]);
	    GetVehicleZAngle(jacob_factoryveh, jacfacvpos[3]);
	    
		new Float:jacfacvinfo[3];
	    GetVehicleModelInfo(GetVehicleModel(jacob_factoryveh), VEHICLE_MODEL_INFO_SIZE, jacfacvinfo[0], jacfacvinfo[1], jacfacvinfo[2]);
	    
		new Float:distance = (jacfacvinfo[1]/2 + 0.5);
     	jacfacvpos[0] += (distance * -floatsin(-jacfacvpos[3], degrees));
    	jacfacvpos[1] += (distance * -floatcos(-jacfacvpos[3], degrees));
		if(IsPlayerInRangeOfPoint(playerid, 1.5, jacfacvpos[0], jacfacvpos[1], jacfacvpos[2])) {
    	    SetPlayerPos(playerid, -2302.9670,-106.8171,999.8542);
    	    SetPlayerFacingAngle(playerid, 180);
    		SetPlayerInterior(playerid, 0);
		}
	}
	return 1;
}

CMD:jbc(playerid, params[]) { //For talking to people driving/inside the Boxville.
	new jac_drivertext[128 - MAX_PLAYER_NAME];
	if (sscanf(params, "s[128]", jac_drivertext)) return 1;
	if(GetPlayerVehicleID(playerid) == jacob_factoryveh || IsPlayerInRangeOfPoint(playerid, 10.0, -2302.9670,-106.8171,999.8542)) {
		new jac_name[MAX_PLAYER_NAME];
 		GetPlayerName(playerid, jac_name, sizeof(jac_name));
    	strmid(jac_name, str_replace('_', ' ', jac_name), 0, MAX_PLAYER_NAME);
	    foreach(Player, i) {
        	if(IsPlayerInRangeOfPoint(i, 10.0, -2302.9670,-106.8171,999.8542) || GetPlayerVehicleID(i) == jacob_factoryveh) {
        		format(donorstr, sizeof(donorstr), "[Boxville] %s says: %s", jac_name, jac_drivertext);
        		SendClientMessage(i, -1, donorstr);
			}
	    }
	}
	return 1;
}