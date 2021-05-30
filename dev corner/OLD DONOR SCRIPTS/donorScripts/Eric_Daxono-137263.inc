/*
 ____                         
/\  _`\          __           
\ \ \L\_\  _ __ /\_\    ___   
 \ \  _\L /\`'__\/\ \  /'___\ 
  \ \ \L\ \ \ \/ \ \ \/\ \__/ 
   \ \____/\ \_\  \ \_\ \____\
    \/___/  \/_/   \/_/\/____/
                              
                              
 ____                                              
/\  _`\                                            
\ \ \/\ \     __     __  _   ___     ___     ___   
 \ \ \ \ \  /'__`\  /\ \/'\ / __`\ /' _ `\  / __`\ 
  \ \ \_\ \/\ \L\.\_\/>  <//\ \L\ \/\ \/\ \/\ \L\ \
   \ \____/\ \__/.\_\/\_/\_\ \____/\ \_\ \_\ \____/
    \/___/  \/__/\/_/\//\/_/\/___/  \/_/\/_/\/___/ 

	
	UNKNOWN DEVELOPMENT RERQUEST
*/

enum {
	SPEAKER_LISTEN_NONE,
	SPEAKER_LISTEN_INDOOR,
	SPEAKER_LISTEN_OUTDOOR
}

new bool: outdoorspeaker = true;
new bool: indoorspeaker = true;
new Text3D: SpeakerStatus;
new Text3D: SpeakerFrequency;
new Text3D: OutpostMOTD;
new SpeakerListening[MAX_PLAYERS] = SPEAKER_LISTEN_NONE;
new IndoorSpeakerChannel;
new OutpostOutdoorSpeaker;
new OutpostIndoorSpeaker;
new OutpostAegisArea;
new CurrentOutpostMOTD[64];

Load_EricDaxono() {
	if(!dini_Exists("outpost.cfg")) {
	    dini_Create("outpost.cfg");
	}
	
	indoorspeaker = bool: dini_Int("outpost.cfg", "indoorspeaker_enabled");
	outdoorspeaker = bool: dini_Int("outpost.cfg", "outdoorspeaker_enabled");
	IndoorSpeakerChannel = dini_Int("outpost.cfg", "indoor_speaker_channel");
	
	//strins(CurrentOutpostMOTD, dini_Get("outpost.cfg", "motd"), 64); //this crashes
	format(CurrentOutpostMOTD, sizeof(CurrentOutpostMOTD), "%s", dini_Get("outpost.cfg", "motd"));
	
	if(outdoorspeaker == true) {
		UpdateDynamic3DTextLabelText(SpeakerStatus, 0x00FF00FF, "ON");
	}
	else {
		UpdateDynamic3DTextLabelText(SpeakerStatus, 0xFF0000FF, "OFF");
	}
	if(indoorspeaker == true) {
	    format(donorstr, sizeof donorstr, "Outpost Radio Observatory\nTuned Station\n{CCCC00}%d", IndoorSpeakerChannel);
	    UpdateDynamic3DTextLabelText(SpeakerFrequency, 0xFF0000FF, donorstr);
	}
	return 1;
}

Save_EricDaxono() {
	dini_IntSet("outpost.cfg", "indoorspeaker_enabled", indoorspeaker);
	dini_IntSet("outpost.cfg", "outdoorspeaker_enabled", outdoorspeaker);
	dini_IntSet("outpost.cfg", "indoor_speaker_channel", IndoorSpeakerChannel);
	
	dini_Set("outpost.cfg", "motd", CurrentOutpostMOTD);
	return 1;
}

Hook:EricDaxono_OnGameModeInit() {
	CreateDynamic3DTextLabel("Aegis\n{00FF00}Online", 0xB22222FF, 210.3190,1820.3220,8.0141, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 3, 0);
	CreateDynamic3DTextLabel("Solace\n{00FF00}Online", 0xFF82ABFF, 210.3760,1825.2704,8.0141, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 3, 0);
	OutpostMOTD = CreateDynamic3DTextLabel("Eric's Message: Welcome to the Outpost Radio Observatory", 0x9999FFFF, 210.6471,1822.9026,9.4063, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 3, 0);
    CreateDynamic3DTextLabel("Outpost Speaker", 0xB22222FF, -87.4484, 1088.91, 21.0531, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
	SpeakerFrequency  = CreateDynamic3DTextLabel("Outpost Radio Observatory\nTuned Station\n{CCCC00}OFF", 0xFF0000FF, 211.4125,1822.7621,6.4141, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 3, 0);
	
	if(indoorspeaker == true)
		SpeakerStatus = CreateDynamic3DTextLabel("ON", 0x00FF00FF, -87.4484, 1088.91, 20.8031, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
	else
	    SpeakerStatus = CreateDynamic3DTextLabel("OFF", 0xFF0000FF, -87.4484, 1088.91, 20.8031, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);

	OutpostOutdoorSpeaker = CreateDynamicCircle(-87.4484, 1088.91, 30.0, -1, -1);
	OutpostIndoorSpeaker  = CreateDynamicCircle(212.4125, 1822.7621, 40.0, 3, -1);
	OutpostAegisArea      = CreateDynamicCircle(1254.7643, -804.2196, 30.0, 0, 0);
	Load_EricDaxono();
	return 1;
}

Hook:EricDaxono_OnPlayerDisconnect(playerid, reason) {
	SpeakerListening[playerid] = SPEAKER_LISTEN_NONE;
	return 1;
}

Hook:EricDaxono_OnPlayerEnterDynamicArea(playerid, areaid) {
	if(areaid == OutpostOutdoorSpeaker) {
	    if(outdoorspeaker == false)
	        return 1;
	    if(disabledRadios[playerid] == 0) {
            PlayAudioStreamForPlayer(playerid, "http://tinyurl.com/Compound-Speaker", -87.4484, 1088.91, 18.4531, 30, 1);
            SpeakerListening[playerid] = SPEAKER_LISTEN_OUTDOOR;
		}
	}
	else if(areaid == OutpostIndoorSpeaker) {
		if(indoorspeaker == false)
		    return 1;
	    if(IndoorSpeakerChannel != -1 && disabledRadios[playerid] == 0) {
			format(donorstr, sizeof donorstr, "http://SARPsignal.tk/%d", IndoorSpeakerChannel);
			PlayAudioStreamForPlayer(playerid, donorstr, 212.4125,1822.7621,6.4141, 40, 1);

			SpeakerListening[playerid] = SPEAKER_LISTEN_INDOOR;
		}
	}
	return 1;
}

Hook:EricDaxono_OnPlayerLeaveDynamicArea(playerid, areaid) {
	if(areaid == OutpostOutdoorSpeaker || areaid == OutpostIndoorSpeaker) {
	    if(SpeakerListening[playerid] != SPEAKER_LISTEN_NONE)
			StopAudioStreamForPlayer(playerid), SpeakerListening[playerid] = SPEAKER_LISTEN_NONE;
	}
	return 1;
}

CMD:opspeaker(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 60052) {
	    if(outdoorspeaker == true) {
	        outdoorspeaker = false;
	        SendClientMessage(playerid, 0xFF000000, "Outpost Speaker Turned OFF.");
	        UpdateDynamic3DTextLabelText(SpeakerStatus, 0xFF0000FF, "OFF");
	        
	        foreach(Player, i) {
	            if(IsPlayerInDynamicArea(i, OutpostOutdoorSpeaker) && SpeakerListening[i] == SPEAKER_LISTEN_OUTDOOR) {
					StopAudioStreamForPlayer(i);
		 			SpeakerListening[i] = SPEAKER_LISTEN_NONE;
				}
			}
		}
		else {
		    outdoorspeaker = true;
		    SendClientMessage(playerid, 0xFF000000, "Outpost Speaker Turned ON.");
            UpdateDynamic3DTextLabelText(SpeakerStatus, 0x00FF00FF, "ON");
            
            foreach(Player, i) {
                if(disabledRadios[i] == 0 && IsPlayerInDynamicArea(i, OutpostOutdoorSpeaker)) {
                    PlayAudioStreamForPlayer(i, "http://tinyurl.com/Compound-Speaker", -87.4484, 1088.91, 18.4531, 30, 1);
					SpeakerListening[i] = SPEAKER_LISTEN_OUTDOOR;
				}
			}
		}
		
		Save_EricDaxono();
	}
	return 1;
}

CMD:ophelp(playerid, params[]) {
    SendClientMessage(playerid, 0xFFFFFFFF, "_______________Outpost Commands_____________");
    SendClientMessage(playerid, 0xFFFFFFFF, "/oprt  /oprtoff  /ophelp  /opradio  /opcompound  /mysterysignal");
   	if(PlayerInfo[playerid][pID] == 60052)
    	SendClientMessage(playerid, 0xFF000000, "/opspeaker  /opaegis  /opsolace /opmotd");
    return 1;
}

CMD:opradio(playerid, params[])
	return cmd_outpostradio(playerid, "");
	
CMD:outpostradio(playerid, params[]) {
	if(disabledRadios[playerid] == 1)
	    return SendClientMessage(playerid, COLOR_GREY, "You have radios toggled, please /togradios to use this command.");
	    
    SendClientMessage(playerid, 0xB22222FF, "You're now listening to Outpost Radio");
    PlayAudioStreamForPlayer(playerid, "http://tinyurl.com/SARPOutpostRadio");
    return 1;
}

CMD:mysterysignal(playerid, params[]) {
	if(disabledRadios[playerid] == 1)
	    return SendClientMessage(playerid, COLOR_GREY, "You have radios toggled, please /togradios to use this command.");

    SendClientMessage(playerid, 0xB22222FF, "You hear a strange signal from a distant location...");
    PlayAudioStreamForPlayer(playerid, "http://tinyurl.com/Mystery-Signal");
    return 1;
}

CMD:opcompound(playerid, params[]) {
	if(CP[playerid] != CHECKPOINT_NONE)
		return SendClientMessage(playerid, COLOR_GREY, "You already have a checkpoint set, use /killcp to kill it or /fixcheckpoint to update it.");
		
	CP[playerid] = CHECKPOINT_OUTPOST;
	DisablePlayerCheckpointEx(playerid);
	SetPlayerCheckpointEx(playerid, -84.2126, 1090.6804, 19.6770, 5.0);
	
	SendClientMessage(playerid, COLOR_WHITE, "You have set a checkpoint to the Outpost Radio compound.");
	return 1;
}

CMD:opaegis(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 60052) {
	    if(isnull(params)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /opaegis [text]");
		format(donorstr, sizeof donorstr, "[A.I.] Aegis: %s", params);
		
		foreach(Player, i) {
		    if(IsPlayerInDynamicArea(i, OutpostOutdoorSpeaker) || IsPlayerInDynamicArea(i, OutpostIndoorSpeaker) || IsPlayerInDynamicArea(i, OutpostAegisArea))
				SendClientMessage(i, 0xB22222FF, donorstr);
		}
	}
	return 1;
}

CMD:opsolace(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 60052) {
	    if(isnull(params)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /opsolace [text]");
		format(donorstr, sizeof donorstr, "[A.I.] Solace: %s", params);

		foreach(Player, i) {
		    if(IsPlayerInDynamicArea(i, OutpostOutdoorSpeaker) || IsPlayerInDynamicArea(i, OutpostIndoorSpeaker) || IsPlayerInDynamicArea(i, OutpostAegisArea))
				SendClientMessage(i, 0xFF82ABFF, donorstr);
		}
	}
	return 1;
}

CMD:mysterychat(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 60052) {
	    if(isnull(params)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /mysterychat [text]");
		format(donorstr, sizeof donorstr, "Signal: %s", params);

		foreach(Player, i) {
		    if(IsPlayerInDynamicArea(i, OutpostIndoorSpeaker))
				SendClientMessage(i, 0x00ed7bFF, donorstr);
		}
	}
	return 1;
}

CMD:oprt(playerid, params[]) {
	if(IsPlayerInRangeOfPoint(playerid, 10.0, 212.4125,1822.7621,6.4141) && GetPlayerVirtualWorld(playerid) == 3) {
	    new channel_id;
	    if(sscanf(params, "i", channel_id)) return SendClientMessage(playerid, 0xCCCCFF00, "USAGE: /oprt (0-2500)");
	    if(channel_id < 0 || channel_id > 2500) return SendClientMessage(playerid, 0xCCCCFF00, "USAGE: /oprt (0-2500)");
	    
	    if(indoorspeaker == false)
	        indoorspeaker = true;

	    IndoorSpeakerChannel = channel_id;
	    format(donorstr, sizeof donorstr, "Outpost Radio Observatory\nTuned Station\n{CCCC00}%d", channel_id);
		UpdateDynamic3DTextLabelText(SpeakerFrequency, 0xFF0000FF, donorstr);
		
		Save_EricDaxono();
		format(donorstr, sizeof donorstr, "http://SARPsignal.tk/%d", channel_id);
		foreach(Player, i) {
		    if(disabledRadios[i] == 1)
		        continue;
		        
		    if(IsPlayerInDynamicArea(i, OutpostIndoorSpeaker))
                PlayAudioStreamForPlayer(i, donorstr, 212.4125, 1822.7621, 6.4141, 40.0, 1), SpeakerListening[i] = SPEAKER_LISTEN_INDOOR;
		}
	}
    else return SendClientMessage(playerid, 0xCCCCFF00, "You need to be in front of the {B22222}Outpost Radio Observatory{CCCCFF} terminal  to use this command!");
	return 1;
}

CMD:oprtoff(playerid, params[]) {
	if(IsPlayerInRangeOfPoint(playerid, 10.0, 212.4125,1822.7621,6.4141) && GetPlayerVirtualWorld(playerid) == 3) {
		indoorspeaker = false;
		IndoorSpeakerChannel = -1;
		UpdateDynamic3DTextLabelText(SpeakerFrequency, 0xFF0000FF, "Outpost Radio Observatory\nTuned Station\n{CCCC00}OFF");
		foreach(Player, i) {
		    if(IsPlayerInDynamicArea(i, OutpostIndoorSpeaker))
		        StopAudioStreamForPlayer(i), SpeakerListening[i] = SPEAKER_LISTEN_NONE;
		}

		Save_EricDaxono();
	}
	return 1;
}
		
CMD:opmotd(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 60052) {
	    if(isnull(params)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /opmotd [motd text]");
	    if(strlen(params) >= 64) return SendClientMessage(playerid, COLOR_WHITE, "Error: MOTD text cannot exceed 64 characters.");

		format(donorstr, sizeof donorstr, "Eric's Message: %s", params);
		UpdateDynamic3DTextLabelText(OutpostMOTD, 0x9999FFFF, donorstr);
		
		strcpy(CurrentOutpostMOTD, params, 64);
		Save_EricDaxono();
	}
	return 1;
}

CMD:opnews(playerid, params[]) {
	if(PlayerInfo[playerid][pID] == 60052 || PlayerInfo[playerid][pID] == 66017) {
	    if(gNews[playerid] == 1)
	        return SendClientMessage(playerid, 0xFF000000, "You have turned opnews off, even for yourself!");
	        
		if(isnull(params))
		    return SendClientMessage(playerid, COLOR_WHITE, "Usage: /opnews [news]");

		format(donorstr, sizeof donorstr, "OPN %s: %s", PlayerICName(playerid), params);
		OOCNews(0xB22222FF, donorstr);
	}
	return 1;
}