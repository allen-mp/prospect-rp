/*	        
									 /$$$$       /$$$$$$$$  /$$$$$$        /$$$$$$$  /$$$$$$$        /$$$$
									| $$_/      | $$_____/ /$$__  $$      | $$__  $$| $$__  $$      |_  $$
									| $$        | $$      | $$  \__/      | $$  \ $$| $$  \ $$        | $$
									| $$        | $$$$$   | $$            | $$$$$$$/| $$$$$$$/        | $$
									| $$        | $$__/   | $$            | $$__  $$| $$____/         | $$
									| $$        | $$      | $$    $$      | $$  \ $$| $$              | $$
									| $$$$      | $$$$$$$$|  $$$$$$/      | $$  | $$| $$             /$$$$
									|____/      |________/ \______/       |__/  |__/|__/            |____/

									 /$$$$$$$$ /$$ /$$   /$$                      /$$$$$$  /$$   /$$              
									| $$_____/| $$|__/  | $$                     /$$__  $$|__/  | $$              
									| $$      | $$ /$$ /$$$$$$    /$$$$$$       | $$  \__/ /$$ /$$$$$$   /$$   /$$
									| $$$$$   | $$| $$|_  $$_/   /$$__  $$      | $$      | $$|_  $$_/  | $$  | $$
									| $$__/   | $$| $$  | $$    | $$$$$$$$      | $$      | $$  | $$    | $$  | $$
									| $$      | $$| $$  | $$ /$$| $$_____/      | $$    $$| $$  | $$ /$$| $$  | $$
									| $$$$$$$$| $$| $$  |  $$$$/|  $$$$$$$      |  $$$$$$/| $$  |  $$$$/|  $$$$$$$
									|________/|__/|__/   \___/   \_______/       \______/ |__/   \___/   \____  $$
									                                                                     /$$  | $$
									                                                                    |  $$$$$$/
									                                                                     \______/ 
									 /$$$$$$$            /$$                 /$$$$$$$  /$$                    
									| $$__  $$          | $$                | $$__  $$| $$                    
									| $$  \ $$  /$$$$$$ | $$  /$$$$$$       | $$  \ $$| $$  /$$$$$$  /$$   /$$
									| $$$$$$$/ /$$__  $$| $$ /$$__  $$      | $$$$$$$/| $$ |____  $$| $$  | $$
									| $$__  $$| $$  \ $$| $$| $$$$$$$$      | $$____/ | $$  /$$$$$$$| $$  | $$
									| $$  \ $$| $$  | $$| $$| $$_____/      | $$      | $$ /$$__  $$| $$  | $$
									| $$  | $$|  $$$$$$/| $$|  $$$$$$$      | $$      | $$|  $$$$$$$|  $$$$$$$
									|__/  |__/ \______/ |__/ \_______/      |__/      |__/ \_______/ \____  $$
									                                                                 /$$  | $$
									                                                                |  $$$$$$/
									                                                                 \______/ 	

						 /$$   /$$                  /$$$$$$                                      /$$$$$$$  /$$           /$$         /$$    
						| $$$ | $$                 /$$__  $$                                    | $$__  $$|__/          | $$        | $$    
						| $$$$| $$  /$$$$$$       | $$  \__/  /$$$$$$   /$$$$$$  /$$   /$$      | $$  \ $$ /$$  /$$$$$$ | $$$$$$$  /$$$$$$  
						| $$ $$ $$ /$$__  $$      | $$       /$$__  $$ /$$__  $$| $$  | $$      | $$$$$$$/| $$ /$$__  $$| $$__  $$|_  $$_/  
						| $$  $$$$| $$  \ $$      | $$      | $$  \ $$| $$  \ $$| $$  | $$      | $$__  $$| $$| $$  \ $$| $$  \ $$  | $$    
						| $$\  $$$| $$  | $$      | $$    $$| $$  | $$| $$  | $$| $$  | $$      | $$  \ $$| $$| $$  | $$| $$  | $$  | $$ /$$
						| $$ \  $$|  $$$$$$/      |  $$$$$$/|  $$$$$$/| $$$$$$$/|  $$$$$$$      | $$  | $$| $$|  $$$$$$$| $$  | $$  |  $$$$/
						|__/  \__/ \______/        \______/  \______/ | $$____/  \____  $$      |__/  |__/|__/ \____  $$|__/  |__/   \___/  
						                                              | $$       /$$  | $$                     /$$  \ $$                    
						                                              | $$      |  $$$$$$/                    |  $$$$$$/                    
						                                              |__/       \______/                      \______/                                     							  							
*/
forward NewProxDetector(playerid, Float:range, statictext[], msg[], colour);
forward GetColorValue(colour, mode);
forward SetRGB(colour, mode, value);


//used for testing
//#define coords  1555.08, -1675.55, 16.19
public NewProxDetector(playerid, Float:range, statictext[], msg[], colour) {

    //Get individual colour values
    new red = GetColorValue(colour, 'r');
    new green = GetColorValue(colour, 'g');
    new blue = GetColorValue(colour, 'b');

    //Keep original for chat bubble
    new original[144];
    format(original, sizeof(original), "%s%s", statictext, msg);

    //Add in accents, statictext is now neglected
    new staticstr[MAX_PLAYER_NAME+50];
    format(staticstr, sizeof(staticstr), "%s", statictext); // accent is already set
    //if(strcmp(Accent[playerid], "none", true, 3)) format(staticstr, sizeof(staticstr), "%s[%s Accent] ", statictext, Accent[playerid]);
    //else format(staticstr, sizeof(staticstr), "%s", statictext);

    new string[144];
    format(string, sizeof(string), "%s%s", staticstr, msg);
    SendClientMessage(playerid, colour, string);


    new Float:tarx, Float:tary, Float:tarz;
    GetPlayerPos(playerid, tarx, tary, tarz);
	new vw = GetPlayerVirtualWorld(playerid);
    
    foreach(new pid: Player) {

        if(pid == playerid) continue;
        if(!IsPlayerConnected(playerid)) continue;

		if(GetPlayerVirtualWorld(pid) != vw) continue;

        //Is player in range?
        new Float:x, Float:y, Float:z;
        GetPlayerPos(pid, x, y, z);
        new Float:dist = GetPointDistanceToPointExMorph(x, y, z, tarx, tary, tarz);
        if(dist <= range) {

            //Get individual colours, decrease by some dependent scalar
            new pr = red, pg = green, pb = blue;
            new scalar = floatround(140/range); //140 RGB values
            pr -= scalar*floatround(dist);
            pg -= scalar*floatround(dist);
            pb -= scalar*floatround(dist);

            //Jumble the text message up.
          /*  if(dist >= 0.8*floatround(range)) {
                new step = 4 + floatround(range-dist);

                new i;
                while(i < strlen(msg)) {
                    msg[i] = random(255);
                    if(msg[i] > 190) i++;
                    else i += step;
                }
            }
*/
            new col;

            //Set the colours
            col = SetRGB(colour, 'r', pr);
            col = SetRGB(col, 'g', pg);
            col = SetRGB(col, 'b', pb);

            //Send it
            format(string, sizeof(string), "%s%s", staticstr, msg);
            SendClientMessage(pid, col, string);


        }
    }
    //Finally, show chat bubble
    if(ThiefText[playerid] != Text3D:INVALID_3DTEXT_ID) return 1;
    if(nametagText[playerid] != Text3D:INVALID_3DTEXT_ID) return 1;
    
    SetPlayerChatBubble(playerid, original, colour, 0.3*floatround(range), 10000);
    return 1;
}

public GetColorValue(colour, mode) {
    switch(mode) {
        case 'r', 'R':
            return (colour & 0xFF000000) >>> 24;
        case 'g', 'G':
            return (colour & 0x00FF0000) >>> 16;
        case 'b', 'B':
            return (colour & 0x0000FF00) >>> 8;
    }
    return 0xFF;
}

public SetRGB(colour, mode, value) {

    switch(mode) {
        case 'r', 'R':
            return (colour & 0x00FFFFFF | (0xFF000000 & (value) << 24));
        
        case 'g', 'G':
            return (colour & 0xFF00FFFF | (0x00FF0000 & (value) << 16));

        case 'b', 'B':
            return (colour & 0xFFFF00FF | (0x0000FF00 & (value) << 8));
    }
    return colour;
}


//=============[ Commands ]=============
//To be optimized later

CMD:s(playerid, params[]) return cmd_shout(playerid, params);

CMD:shout(playerid, params[])
{
	if(gPlayerLogged[playerid] == 0) return SendClientMessage(playerid, COLOR_GREY, "You havent logged in yet.");
	if(pDeathState[playerid] == PLAYER_STATE_INHOSP || WatchingTV[playerid] == true) return 1;
	if(pDeathState[playerid] == PLAYER_STATE_REVIVE) //injured players cannot speak
	{
		SendClientMessage(playerid, -1, "You're not in the condition to shout!");
		new str[128];
		format(str, sizeof(str), "* %s tries to mumble something", PlayerICName(playerid));
		SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 15000);
		return 0;
	}
	if(pDeathState[playerid] == PLAYER_STATE_REVIVE){

	}
	if(PlayerInfo[playerid][pMuted] == 1) return SendClientMessage(playerid, COLOR_GREY, "You can't use this whilst muted.");
	new result[128];
    new str[144];
	if(sscanf(params, "s[128]", result)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} (/s)hout [local chat]");
	
	// Gagged
	if(IsPlayerGagged(playerid)) {
		SendClientMessage(playerid, COLOR_GREY, "The gag in your mouth makes you unable to speak...");
		
		new gagStr[128];
		format(gagStr, sizeof(gagStr), "* %s tries to mumble something loudly.", PlayerICName(playerid));
		SetPlayerChatBubble(playerid, gagStr, COLOR_PURPLE, 20.0, 15000);
		return 1;
	}

	/*foreach( new i: Player )
	{
		if(PlayerInfo[i][pWire] == 1 && !CheckForJammer(playerid) && !CheckForJammer(i))
		{
			new Float: x[3];
			GetPlayerPos(playerid, x[0], x[1], x[2]);
			if(IsPlayerInRangeOfPoint(i, 30.0, x[0], x[1], x[2]))
			{
				if(i == playerid)
				{
					if(strcmp(Accent[i], "none", true, 3)) format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s: [%s Accent] %s!", PlayerICName(i), Accent[i], result);
					else format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s: %s!", PlayerICName(i), result);
				}
				else
				{
					if(strcmp(Accent[playerid], "none", true, 3)) format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s's surroundings: [%s Accent] %s!", PlayerICName(i), Accent[playerid], result);
					else format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s's surroundings: %s!", PlayerICName(i), result);
				}
				foreach( new fbi: Player )
				{
					if(PlayerInfo[fbi][pMember] == 2 || PlayerInfo[fbi][pMember] == 11)
					{
						if(IsPlayerInAnyVehicle(fbi))
						{
							if(sVehicleInfo[GetDynamicVehicleID(GetPlayerVehicleID(fbi))][v_faction] == 2 || sVehicleInfo[GetDynamicVehicleID(GetPlayerVehicleID(fbi))][v_faction] == 11 && GetVehicleModel(GetPlayerVehicleID(fbi)) == 482)
							{
								SendClientMessage(fbi, COLOR_ORANGE, str);
							}
						}
					}
				} 
			}
		}
	}*/
	
	foreach( new i: Player )
	{
			if(PlayerInfo[i][pWire] == 1 && !CheckForJammer(playerid) && !CheckForJammer(i))
			{
				new Float: x[3];
				GetPlayerPos(playerid, x[0], x[1], x[2]);
				if(IsPlayerInRangeOfPoint(i, 20.0, x[0], x[1], x[2]))
				{
					if(i == playerid)
					{
						if(strcmp(Accent[i], "none", true, 3)) format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s: [%s Accent] %s!", PlayerICName(i), Accent[i], result);
						else format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s: %s!", PlayerICName(i), result);
					}
					else
					{
						if(strcmp(Accent[playerid], "none", true, 3)) format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s's surroundings: [%s Accent] %s!", PlayerICName(i), Accent[playerid], result);
						else format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s's surroundings: %s!", PlayerICName(i), result);
					}
					if(PlayerInfo[i][pMember] != 2) // if player isn't FBI, send to both FBI and S9
					{
						SendBuggedMessage(COLOR_ORANGE, str);
						SendS9BuggedMessage(COLOR_ORANGE, str);
					}	
					else SendS9BuggedMessage(COLOR_ORANGE, str); // if player is FBI, send to S9 only, this is to avoid FBI figuring out
					// 																							that s9 bugged them
				}
			}
	}

    new string[MAX_PLAYER_NAME+10];
    format(string, sizeof(string), "%s shouts: ", PlayerICName(playerid));
	NewProxDetector(playerid, 30.0, string, result, COLOR_FADE1);
	return 1;
}


CMD:l(playerid, params[]) return cmd_low(playerid, params);

CMD:low(playerid, params[])
{
	new msg[128];
    new str[144];
	if(!sscanf(params, "s[128]", msg))
	{
		if(gPlayerLogged[playerid] == 0)
		{
			SendClientMessage(playerid, COLOR_GREY, "You havent logged in yet.");
			return 1;
		}
		if(pDeathState[playerid] == PLAYER_STATE_INHOSP || ThiefText[playerid] != Text3D:INVALID_3DTEXT_ID || WatchingTV[playerid] == true) return 1;
		if(PlayerInfo[playerid][pMuted] == 1) return SendClientMessage(playerid, COLOR_GREY, "You can't use this whilst muted.");
		
		// Gagged
		if(IsPlayerGagged(playerid)) {
			SendClientMessage(playerid, COLOR_GREY, "The gag in your mouth makes you unable to speak...");
			
			new gagStr[128];
			format(gagStr, sizeof(gagStr), "* %s tries to mumble something quietly.", PlayerICName(playerid));
			SetPlayerChatBubble(playerid, gagStr, COLOR_PURPLE, 20.0, 15000);
			return 1;
		}

		/*foreach( new i: Player )
		{
			if(PlayerInfo[i][pWire] == 1 && !CheckForJammer(playerid) && !CheckForJammer(i))
			{
				new Float: x[3];
				GetPlayerPos(playerid, x[0], x[1], x[2]);
				if(IsPlayerInRangeOfPoint(i, 5.0, x[0], x[1], x[2]))
				{
					if(i == playerid)
					{
						format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s [low]: %s", PlayerOOCName(i), msg);
					}
					else
					{
						format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s's surroundings [low]: %s", PlayerOOCName(i), msg);
					}
					foreach( new fbi: Player )
					{
						if(PlayerInfo[fbi][pMember] == 2 || PlayerInfo[fbi][pMember] == 11)
						{
							if(IsPlayerInAnyVehicle(fbi))
							{
								if(sVehicleInfo[GetDynamicVehicleID(GetPlayerVehicleID(fbi))][v_faction] == 2 || sVehicleInfo[GetDynamicVehicleID(GetPlayerVehicleID(fbi))][v_faction] == 11 && GetVehicleModel(GetPlayerVehicleID(fbi)) == 482)
								{
									SendClientMessage(fbi, COLOR_ORANGE, str);
								}
							}
						}
					}
				}
			}
		}*/
		foreach( new i: Player )
		{
			if(PlayerInfo[i][pWire] == 1 && !CheckForJammer(playerid) && !CheckForJammer(i))
			{
				new Float: x[3];
				GetPlayerPos(playerid, x[0], x[1], x[2]);
				if(IsPlayerInRangeOfPoint(i, 20.0, x[0], x[1], x[2]))
				{
					if(i == playerid)
					{
						format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s [low]: %s", PlayerOOCName(i), msg);
					}
					else
					{
						format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s's surroundings [low]: %s", PlayerOOCName(i), msg);
					}
					if(PlayerInfo[i][pMember] != 2) // if player isn't FBI, send to both FBI and S9
					{
						SendBuggedMessage(COLOR_ORANGE, str);
						SendS9BuggedMessage(COLOR_ORANGE, str);
					}	
					else SendS9BuggedMessage(COLOR_ORANGE, str); // if player is FBI, send to S9 only, this is to avoid FBI figuring out
					// 																							that s9 bugged them
				}
			}
		}
		if(!IsPlayerInAnyVehicle(playerid))
		{
			format(str, sizeof(str), "%s [low]: ", PlayerICName(playerid));
			NewProxDetector(playerid, 5.0, str, msg, COLOR_GREY);
		}
		else
		{
			if(VehicleHasWindows(GetPlayerVehicleID(playerid)))
			{
				if(windowsDown[GetPlayerVehicleID(playerid)] == false)
				{
					format(str, sizeof(str), "(vehicle) %s [low]: %s", PlayerICName(playerid), msg);
					CarMessage(playerid, COLOR_GREY, str);
				}
				else
				{
					format(str, sizeof(str), "%s [low]: ", PlayerICName(playerid));
					NewProxDetector(playerid, 5.0, str, msg, COLOR_GREY);
				}
			}
			else
			{
				format(str, sizeof(str), "%s [low]: ", PlayerICName(playerid));
				NewProxDetector(playerid, 5.0, str, msg, COLOR_GREY);
			}
		}
	}
	else
	{
		SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} (/l)ow [local chat]");
	}
	return 1;
}
