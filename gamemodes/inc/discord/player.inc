

/*
    @Author: dy1zan / Golden
    @Command: !newb
    This callback is invoked whenever someone types !newb in Discord.
    This will send an a new  message in-game command /newb
*/

DiscordResponse:newb(data[]) {
    //Convert string into JSON object so we can extract data e.g sender, message
    new JSONNode:jsonData = json_parse_string(data);

    new sender[MAX_PLAYER_NAME+1], message[110], channel[30];
    json_get_string(jsonData, channel, sizeof channel, "channel");
    //Get the 'sender'/'message' contained within the JSON object
    json_get_string(jsonData, sender, sizeof sender, "sender");
    json_get_string(jsonData, message, sizeof message, "message");
    
    //Output the data
    new string[144];
    format(string, sizeof string, "** [Discord] %s: %s **", sender, message);
    printf(string);

    OOCNewbie(NEWBIE_COLOR, string);
    
    return 1;
}

/*
    Some util
*/
stock Discord:GetHelperRank(playerid) {
	new name[32];
    new level = PlayerInfo[playerid][pHelper];
	switch(level) {
		case 0: name = "None";
		case 1: name = "Trial Helper";
		case 2: name = "Helper";
		case 3: name = "Senior Helper";
		case 4: name = "Head Helper";
		case 5: name = "Director of The Helpers Team";
		default: name = "?";
	}
    return name;
}

DiscordResponse:id(data[]) {
    //Convert string into JSON object so we can extract data e.g sender, message
    new JSONNode:jsonData = json_parse_string(data);

    new pattern[110], channel[30], string[128], string2[128], count;
    json_get_string(jsonData, channel, sizeof channel, "channel");

    json_get_string(jsonData, pattern, sizeof pattern, "pattern");
    
    if(IsNumeric(pattern))
    {
        new player;
        sscanf(pattern, "u", player);
        if(IsPlayerConnected(player))
        {
            new name2[24], status[10];

            strmid(name2, str_replace('_', ' ', PlayerName(player)), 0, MAX_PLAYER_NAME);
            if(PlayerPaused[player] == 1)
                status = "tabbed";
            else
                status = "untabbed";

            format(string, sizeof(string), "Name: %s, ID: %d, Level: %d, Ping: %d, Status: %s", name2, player, PlayerInfo[player][pLevel], GetPlayerPing(player), status);
            Discord:sendBasicRequest(channel, "Players found", string, DISCORD_COLOR_INFO);
        }
        return 1;
    }
    else 
    {
        foreach( new i: Player )
        {
            if(strfind(PlayerName(i), pattern, true) != -1)
            {
                new name2[24], status[10];

                strmid(name2, str_replace('_', ' ', PlayerName(i)), 0, MAX_PLAYER_NAME);
                if(PlayerPaused[i] == 1)
                    status = "tabbed";
                else
                    status = "untabbed";
            
                if( count > 1 )
                {
                    format(string2, sizeof(string2), "Name: %s, ID: %d, Level: %d, Ping: %d, Status: %s\n", name2, i, PlayerInfo[i][pLevel], GetPlayerPing(i), status);
                    strins(string, string2, 0);

                }
                else 
                { 
                    format(string, sizeof(string), "Name: %s, ID: %d, Level: %d, Ping: %d, Status: %s", name2, i, PlayerInfo[i][pLevel], GetPlayerPing(i), status);
                    count++;
                }

                Discord:sendBasicRequest(channel, "Players found", string, DISCORD_COLOR_INFO);
            }
        }
    }
    return 1;
}

DiscordResponse:time(data[])
{
    new JSONNode:jsonData = json_parse_string(data);
    new channel[30];
    json_get_string(jsonData, channel, sizeof channel, "channel");

    new year, month, day;
    getdate(year, month, day);
    new hour, minute, second;
    gettime(hour, minute, second);
    hour = FixHour(hour);
    new string[128];

    format(string, sizeof(string), "%02d %s %02d:%02d:%02d", day, GetMonthFromInt(month), hour, minute, second);
    Discord:sendBasicRequest(channel, "Server time", string, DISCORD_COLOR_INFO);
    return 1;
}

DiscordResponse:admins(data[]) {
    //Get the channel the command was sent from
    new JSONNode:jsonData = json_parse_string(data);
    new channel[30];
    json_get_string(jsonData, channel, sizeof channel, "channel");

    //Get online admins as a string
    new string[144];
    foreach(new i : Player) {
        if(PlayerInfo[i][pAdmin] >= 1 && PlayerInfo[i][pStealthed] == 0) {
            if(strlen(string) > 1) {
                format(string, sizeof string, "%s, %s", string, PlayerICName(i));
            }
            else {
                format(string, sizeof string, "%s", PlayerICName(i));
            }
        }
    }
    if(strlen(string) < 1) {
        format(string, sizeof string, "There are no administrators online.");
    }

    //Send the list to Discord
    Discord:sendBasicRequest(channel, "Server administrators", string, DISCORD_COLOR_INFO);
}