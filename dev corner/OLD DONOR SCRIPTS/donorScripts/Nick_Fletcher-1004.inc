/*
 __  __              __         
/\ \/\ \  __        /\ \        
\ \ `\\ \/\_\    ___\ \ \/'\    
 \ \ , ` \/\ \  /'___\ \ , <    
  \ \ \`\ \ \ \/\ \__/\ \ \\`\  
   \ \_\ \_\ \_\ \____\\ \_\ \_\
    \/_/\/_/\/_/\/____/ \/_/\/_/
                                
                                
 ____    ___           __           __                      
/\  _`\ /\_ \         /\ \__       /\ \                     
\ \ \L\_\//\ \      __\ \ ,_\   ___\ \ \___      __   _ __  
 \ \  _\/ \ \ \   /'__`\ \ \/  /'___\ \  _ `\  /'__`\/\`'__\
  \ \ \/   \_\ \_/\  __/\ \ \_/\ \__/\ \ \ \ \/\  __/\ \ \/ 
   \ \_\   /\____\ \____\\ \__\ \____\\ \_\ \_\ \____\\ \_\ 
    \/_/   \/____/\/____/ \/__/\/____/ \/_/\/_/\/____/ \/_/ 

	
	UNKNOWN DEVELOPMENT RERQUEST
*/
	
new bool: fCasinoLocked = false;
new bool: fCasinoKey[MAX_PLAYERS] = false;
new bool: fCasinoCCTV[MAX_PLAYERS] = false;
new LastCasinoCall;
new Text3D: fCasinoText;


Load_NickFletcher() {
	if(!dini_Exists("fletcher.cfg"))
	    dini_Create("fletcher.cfg");
	    
	fCasinoLocked = bool: dini_Int("fletcher.cfg", "CasinoLocked");
	
	if(fCasinoLocked == true)
	    UpdateDynamic3DTextLabelText(fCasinoText, COLOR_LIGHTBLUE, "Fletchers Casino\n{ff0000}Locked");
	return 1;
}

Save_NickFletcher() {
	if(dini_Exists("fletcher.cfg")) {
	    dini_IntSet("fletcher.cfg", "CasinoLocked", fCasinoLocked);
	}
	return 1;
}

Hook:NickFletcher_OnGameModeInit() {
	Load_NickFletcher();
 	fCasinoText = CreateDynamic3DTextLabel("Fletchers Casino\n{00ff00}Unlocked", COLOR_LIGHTBLUE, 1491.98, -1450.51, 13.53, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
	return 1;
}

Hook:NickFletcher_OnPlayerDisconnect(playerid, reason) {
	fCasinoKey[playerid] = false;
	return 1;
}

CMD:dhelp(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 150232 || fCasinoKey[playerid] == true) {
	    SendClientMessage(playerid, -1, "~~~~~~~~~~~~~~~ Fletcher's Casino Commands ~~~~~~~~~~~~~~~");
	    SendClientMessage(playerid, -1, "/dcas, /clock, /cint, /ckey, /ctake, /pb, /dcamera, /dmembers");
	}
	return 1;
}

CMD:dcas(playerid, params[]) {
	if(CP[playerid] != CHECKPOINT_NONE && CP[playerid] != CHECKPOINT_FLETCHER)
	    return SendClientMessage(playerid, COLOR_GREY, "You already have a checkpoint set, use /killcp to kill it or /fixcheckpoint to update it.");
	    
	CP[playerid] = CHECKPOINT_FLETCHER;
	DisablePlayerCheckpointEx(playerid);
	SetPlayerCheckpointEx(playerid, 1491.96, -1450.51, 13.53, 5.0);
	SendClientMessage(playerid, COLOR_WHITE, "You have set a checkpoint to the Fletcher Casino.");
	return 1;
}

CMD:clock(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 150232 || fCasinoKey[playerid] == true || PlayerInfo[playerid][pAdmin] >= 4) {
	    if(IsPlayerInRangeOfPoint(playerid, 10.0, 1491.96, -1450.51, 13.53) || IsPlayerInRangeOfPoint(playerid, 100.0, 1133.44, -13.40, 1000.67) && GetPlayerVirtualWorld(playerid) == 5) {
		    if(fCasinoLocked == true) {
		        fCasinoLocked = false;
				SendClientMessage(playerid, COLOR_WHITE, "You have unlocked the Casino Door.");
				
				UpdateDynamic3DTextLabelText(fCasinoText, COLOR_LIGHTBLUE, "Fletchers Casino\n{00ff00}Unlocked");
			}
			else {
			    fCasinoLocked = true;
			    SendClientMessage(playerid, COLOR_WHITE, "You have locked the Casino Door.");
			    
			    UpdateDynamic3DTextLabelText(fCasinoText, COLOR_LIGHTBLUE, "Fletchers Casino\n{ff0000}Locked");
			}
			return Save_NickFletcher();
		}
		else SendClientMessage(playerid, COLOR_GREY, "You are not near or inside the Casino.");
	}
	return 1;
}

CMD:cint(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 150232 || fCasinoKey[playerid] == true) {
	    if(IsPlayerInRangeOfPoint(playerid, 100.0, 1133.44, -13.40, 1000.67) && GetPlayerVirtualWorld(playerid) == 5) {
	        if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /cint [text]");
	        if(strlen(params) >= 80) return SendClientMessage(playerid, COLOR_GREY, "Error: Message is too long(cannot be more than 82 characters).");

			format(donorstr, sizeof donorstr, "[Microphone] %s: %s", PlayerICName(playerid), params);
			foreach(Player, i) {
			    if(IsPlayerInRangeOfPoint(i, 100.0, 1133.44, -13.40, 1000.67) && GetPlayerVirtualWorld(i) == 5)
					SendClientMessage(i, COLOR_YELLOW, donorstr);
			}
		}
		else SendClientMessage(playerid, COLOR_GREY, "You are not in the Casino.");
	}
	return 1;
}

CMD:ckey(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 150232 || PlayerInfo[playerid][pAdmin] >= 1337) {
	    new giveplayerid;
	    if(sscanf(params, "u", giveplayerid)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /ckey [playerid]");
	    if(!IsPlayerConnected(giveplayerid)) return SendClientMessage(playerid, COLOR_GREY, "That player isn't connected.");
	    if(!IsPlayerInRangeOfPlayer(4.5, playerid, giveplayerid)) return SendClientMessage(playerid, COLOR_GREY, "You are not near that player.");
	    if(fCasinoKey[giveplayerid] == true) return SendClientMessage(playerid, COLOR_GREY, "That player already has a Casino Key.");
	    
	    fCasinoKey[giveplayerid] = true;
	    
	    format(donorstr, sizeof donorstr, "You have given %s a Casino Key.", PlayerICName(giveplayerid));
	    SendClientMessage(playerid, COLOR_WHITE, donorstr);
	    
	    format(donorstr, sizeof donorstr, "%s has given you a Casino Key.", PlayerICName(playerid));
	    SendClientMessage(playerid, COLOR_WHITE, donorstr);
	    
	    format(donorstr, sizeof donorstr, "* %s gives %s a key to the Casino.", PlayerICName(playerid), PlayerICName(giveplayerid));
	    ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	}
	return 1;
}

CMD:ctake(playerid, params[]) {
    if(PlayerInfo[playerid][pID] == 150232 || PlayerInfo[playerid][pAdmin] >= 1337) {
	    new giveplayerid;
	    if(sscanf(params, "u", giveplayerid)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /ckey [playerid]");
	    if(!IsPlayerConnected(giveplayerid)) return SendClientMessage(playerid, COLOR_GREY, "That player isn't connected.");
	    if(!IsPlayerInRangeOfPlayer(4.5, playerid, giveplayerid)) return SendClientMessage(playerid, COLOR_GREY, "You are not near that player.");
	    if(fCasinoKey[giveplayerid] == false) return SendClientMessage(playerid, COLOR_GREY, "That player doesn't have a Casino Key.");

	    fCasinoKey[giveplayerid] = false;

	    format(donorstr, sizeof donorstr, "You have taken %s's Casino Key.", PlayerICName(giveplayerid));
	    SendClientMessage(playerid, COLOR_WHITE, donorstr);

	    format(donorstr, sizeof donorstr, "%s has taken your Casino Key.", PlayerICName(playerid));
	    SendClientMessage(playerid, COLOR_WHITE, donorstr);

	    format(donorstr, sizeof donorstr, "* %s takes their Casino Key back from %s.", PlayerICName(playerid), PlayerICName(giveplayerid));
	    ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	}
	return 1;
}

CMD:pb(playerid, params[])
	return cmd_panicbutton(playerid, "");
	
CMD:panicbutton(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 150232 || fCasinoKey[playerid] == true) {
	    if(LastCasinoCall <= (gettime() + 60)) {
		    format(donorstr, sizeof donorstr, "You must wait %d seconds before triggering the alarm again.", LastCasinoCall - gettime());
		    return SendClientMessage(playerid, COLOR_GREY, donorstr);
		}
		
	    if(IsPlayerInRangeOfPoint(playerid, 100.0, 1133.44, -13.40, 1000.67) && GetPlayerVirtualWorld(playerid) == 5) {
	        format(donorstr, sizeof donorstr, "911 Call - Caller: %s(Fletcher Casino)", PlayerICName(playerid));
	        SendCopMessage(COLOR_DBLUE, donorstr);
	        SendCopMessage(COLOR_DBLUE, "Crime: Panic Alarm((/dcas to locate)), Suspect: N/A");
	        
	        PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
	        SendClientMessage(playerid, COLOR_WHITE, "You have triggered the Casino Alarm, local police units have been alerted.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "You are not inside of the Casino.");
	}
	return 1;
}

CMD:dcamera(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 150232 || fCasinoKey[playerid] == true) {
	    if(!IsPlayerInRangeOfPoint(playerid, 5.0, 1142.63, -10.65, 1000.67) && GetPlayerVirtualWorld(playerid) != 5) return SendClientMessage(playerid, COLOR_GREY, "You are not near the Casino CCTV system.");
		if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dcamera [1-5, off]");
		
		if(strmatch(params, "off")) {
		    if(fCasinoCCTV[playerid] == false) return SendClientMessage(playerid, COLOR_GREY, "You are not watching the Casino CCTV.");
			fCasinoCCTV[playerid] = false;
			SetCameraBehindPlayer(playerid);

			SendClientMessage(playerid, COLOR_GREY, "You have stopped watching the CCTV camera.");
			return 1;
		}
		
		switch(strval(params)) {
		    case 1: {
			    SetPlayerCameraPos(playerid, 1135.598266, -13.055255, 1002.148559);
			    SetPlayerCameraLookAt(playerid, 1132.390502, -16.832738, 1001.484375);
			}
			case 2: {
			    SetPlayerCameraPos(playerid, 1115.694335, -11.405779, 1004.237731);
			    SetPlayerCameraLookAt(playerid, 1119.960449, -8.962922, 1003.325195);
			}
			case 3: {
			    SetPlayerCameraPos(playerid, 1135.258911, 9.924472, 1004.576416);
			    SetPlayerCameraLookAt(playerid, 1130.951049, 7.988495, 1002.935058);
			}
			case 4: {
			    SetPlayerCameraPos(playerid, 1113.584350, -9.115205, 1003.608276);
			    SetPlayerCameraLookAt(playerid, 1118.416503, -9.451298, 1002.368225);
			}
			case 5: {
 			    SetPlayerCameraPos(playerid, 1133.872924, 6.324785, 1003.962951);
	            SetPlayerCameraLookAt(playerid, 1137.623046, 9.605833, 1003.548828);
			}
			default: return SendClientMessage(playerid, COLOR_GREY, "Valid CCTV's range from 1 to 5.");
		}

		format(donorstr, sizeof donorstr, "You have started watching CCTV #%d.", strval(params));
		SendClientMessage(playerid, COLOR_WHITE, donorstr);

		fCasinoCCTV[playerid] = true;
	}
	return 1;
}

CMD:dmembers(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 150232 || fCasinoKey[playerid] == true) {
	    SendClientMessage(playerid, COLOR_WHITE, "Casino Members: ");
	    foreach(Player, i) {
	        if(fCasinoKey[i] == true || PlayerInfo[i][pID] == 150232) {
	            format(donorstr, sizeof donorstr, "Casino %s: %s", (PlayerInfo[i][pID] == 150232) ? ("Owner") : ("Key"), PlayerICName(i));
	            SendClientMessage(playerid, COLOR_WHITE, donorstr);
			}
		}
	}
	return 1;
}