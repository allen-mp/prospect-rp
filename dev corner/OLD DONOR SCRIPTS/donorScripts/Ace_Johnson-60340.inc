/*
 ______                   
/\  _  \                  
\ \ \L\ \    ___     __   
 \ \  __ \  /'___\ /'__`\ 
  \ \ \/\ \/\ \__//\  __/ 
   \ \_\ \_\ \____\ \____\
    \/_/\/_/\/____/\/____/
                          
                          
 _____          __                                         
/\___ \        /\ \                                        
\/__/\ \    ___\ \ \___     ___     ____    ___     ___    
   _\ \ \  / __`\ \  _ `\ /' _ `\  /',__\  / __`\ /' _ `\  
  /\ \_\ \/\ \L\ \ \ \ \ \/\ \/\ \/\__, `\/\ \L\ \/\ \/\ \ 
  \ \____/\ \____/\ \_\ \_\ \_\ \_\/\____/\ \____/\ \_\ \_\
   \/___/  \/___/  \/_/\/_/\/_/\/_/\/___/  \/___/  \/_/\/_/

   
	http://www.gta-sarp.com/forums/showthread.php?182667-Ace_Johnson-s-Mapping-Project-Request
*/

new AceGate[4];
new bool:AceGateOpen[4] = false;

Hook:AceJohnson_OnGameModeInit() {
	AceGate[0] = CreateDynamicObject(7930,316.7000000,-1136.8199000,81.3500000,0.0000000,0.0000000,133.3800000); //garagedoor
	AceGate[1] = CreateDynamicObject(7930,316.7000000,-1136.8199000,81.3500000,0.0000000,180.0000000,133.3800000); //garagedoor
	AceGate[2] = CreateDynamicObject(3061,308.6900000,-1139.8300000,80.8600000,0.0000000,0.0000000,-46.5000000); //door on garage
	AceGate[3] = CreateDynamicObject(1498,302.0600000,-1128.5300000,79.5100000,0.0000000,0.0000000,-135.6600000); //door on garage, backside

	AddStaticVehicleEx(409, 293.0240, -1175.1696, 80.6512, -46.8600, -1, -1, -1); //limo
   	AddStaticVehicleEx(487, 288.1877, -1190.0089, 81.0110, -136.0800, -1, -1, -1); //helicopter
	return 1;
}

CMD:mansion(playerid, params[]) {
	if(isnull(params)) return SendClientMessage(playerid, COLOR_GRAD1, "USAGE: /mansion [garage/garagedoor]");
	if(!strcmp(params, "garage", true, 6)) {
		if(IsPlayerInRangeOfPoint(playerid, 20.0, 316.7000, -1136.8199, 81.3500)) {
			if(AceGateOpen[0] == true && AceGateOpen[1] == true) {
	            MoveDynamicObject(AceGate[0], 316.7000, -1136.8199, 81.3500, 0.3, 0.0000, 0.0000, 133.3800);
	            MoveDynamicObject(AceGate[1], 316.7000, -1136.8199, 81.3500, 0.3, 0.0000, 180.0000, 133.3800);
	            PlayerPlaySound(playerid, 1153, 316.7000, -1136.8199, 81.3500);
	            AceGateOpen[0] = false, AceGateOpen[1] = false;
			}
			else {
	            MoveDynamicObject(AceGate[0], 317.4825, -1137.6481, 82.3940, 0.3, 0.0000, 90.0000, 133.3800);
	            MoveDynamicObject(AceGate[1], 317.4825, -1137.6481, 82.3940, 0.3, 0.0000, 270.0000, 133.3800);
	            PlayerPlaySound(playerid, 1153, 316.7000, -1136.8199, 81.3500);
	            AceGateOpen[0] = true, AceGateOpen[1] = true;		
			}
		}
	}
	else if(!strcmp(params, "garagedoor", true, 10)) {
		if(IsPlayerInRangeOfPoint(playerid, 3, 308.69, -1139.83, 80.86)) {
			if(AceGateOpen[2] == true) {
				MoveDynamicObject(AceGate[2], 308.6859, -1139.8292, 80.8638, 0.02, 0.0000, 0.0000, 84.5400);
				PlayerPlaySound(playerid, 6400, 311.2500, -1140.5000, 81.2000);
				AceGateOpen[2] = false;
			}
			else MoveDynamicObject(AceGate[2], 308.69, -1139.83, 80.86, 0.02, 0.0000, 0.0000, -46.5000), AceGateOpen[2] = true;
		}
		if(IsPlayerInRangeOfPoint(playerid, 3, 302.06, -1128.53, 79.51)) {
			if(AceGateOpen[3] == true) {
				MoveDynamicObject(AceGate[3], 302.0622, -1128.5250, 79.5102, 0.02, 0.0000, 0.0000, -227.5199);
				AceGateOpen[3] = false;
			}
			else MoveDynamicObject(AceGate[3], 302.06, -1128.53, 79.51, 0.02, 0.00, 0.00, -135.66), AceGateOpen[3] = true;
		}
	}
	return 1;
}