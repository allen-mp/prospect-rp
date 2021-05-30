/*
 ____                                    
/\  _`\                    __            
\ \ \/\ \     __   __  __ /\_\    ___    
 \ \ \ \ \  /'__`\/\ \/\ \\/\ \ /' _ `\  
  \ \ \_\ \/\  __/\ \ \_/ |\ \ \/\ \/\ \ 
   \ \____/\ \____\\ \___/  \ \_\ \_\ \_\
    \/___/  \/____/ \/__/    \/_/\/_/\/_/
                                         
                                         
 ____            __                            
/\  _`\   __    /\ \                           
\ \ \L\_\/\_\   \_\ \     __    ___     ___    
 \ \ \L_L\/\ \  /'_` \  /'__`\ / __`\ /' _ `\  
  \ \ \/, \ \ \/\ \L\ \/\  __//\ \L\ \/\ \/\ \ 
   \ \____/\ \_\ \___,_\ \____\ \____/\ \_\ \_\
    \/___/  \/_/\/__,_ /\/____/\/___/  \/_/\/_/

	
	http://www.gta-sarp.com/forums/showthread.php?182818-Development-Request-Devin-Gideon-s-Request
*/

new devin_boxville;

Hook:DevinGideon_OnGameModeInit() {
	devin_boxville = AddStaticVehicleEx(498, 1865.6602, -1318.0272, 13.5430, 263.2390, 53, 53, -1); // Boxville
	return 1;
}

CMD:dboxville(playerid, params[]) {//Entering/exiting the boxville (Credits to Jacob Malcom)
	if(GetPlayerVirtualWorld(playerid) == 1 && IsPlayerInRangeOfPoint(playerid, 2.0, -2302.9670, -106.8171, 999.8542)) {
	    new Float:jacfacvpos[4];
	    GetVehiclePos(devin_boxville, jacfacvpos[0], jacfacvpos[1], jacfacvpos[2]);
	    GetVehicleZAngle(devin_boxville, jacfacvpos[3]);
	    
		new Float:jacfacvinfo[3];
	    GetVehicleModelInfo(GetVehicleModel(devin_boxville), VEHICLE_MODEL_INFO_SIZE, jacfacvinfo[0], jacfacvinfo[1], jacfacvinfo[2]);
		
	    new Float:distance = (jacfacvinfo[1]/2 + 0.5);
     	jacfacvpos[0] += (distance * -floatsin(-jacfacvpos[3], degrees));
    	jacfacvpos[1] += (distance * -floatcos(-jacfacvpos[3], degrees));
    	SetPlayerPos(playerid, jacfacvpos[0], jacfacvpos[1], jacfacvpos[2]);
    	SetPlayerInterior(playerid, 0);
    	SetPlayerVirtualWorld(playerid, 0);
    	PlayerInfo[playerid][pVirtualWorld] = 0;
	}
	else {
		new Float:jacfacvpos[4];
	    GetVehiclePos(devin_boxville, jacfacvpos[0], jacfacvpos[1], jacfacvpos[2]);
	    GetVehicleZAngle(devin_boxville, jacfacvpos[3]);
		
	    new Float:jacfacvinfo[3];
	    GetVehicleModelInfo(GetVehicleModel(devin_boxville), VEHICLE_MODEL_INFO_SIZE, jacfacvinfo[0], jacfacvinfo[1], jacfacvinfo[2]);
	    
		new Float:distance = (jacfacvinfo[1]/2 + 0.5);
     	jacfacvpos[0] += (distance * -floatsin(-jacfacvpos[3], degrees));
    	jacfacvpos[1] += (distance * -floatcos(-jacfacvpos[3], degrees));
		if(IsPlayerInRangeOfPoint(playerid, 1.5, jacfacvpos[0], jacfacvpos[1], jacfacvpos[2])) {
    	    SetPlayerPos(playerid, -2302.9670,-106.8171,999.8542);
    	    SetPlayerFacingAngle(playerid, 180);
    		SetPlayerInterior(playerid, 0);
    		SetPlayerVirtualWorld(playerid, 1);
    		PlayerInfo[playerid][pVirtualWorld] = 1;
		}
	}
	return 1;
}