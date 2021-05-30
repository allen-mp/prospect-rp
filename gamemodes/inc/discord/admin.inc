/*
    This file adds functionality to admin commands sent
    via Discord, e.g !admins, !a
    Please continue using the format used below for each command.
*/

/*
    @Author: dy1zan
    @Command: !a
    This callback is invoked whenever someone types !a in Discord.
    This will send an admin chat message in-game, of the message sent from Discord.
*/
DiscordResponse:asay(data[]) {
    //Convert string into JSON object so we can extract data e.g sender, message
    new JSONNode:jsonData = json_parse_string(data);

    new sender[MAX_PLAYER_NAME+1], message[110];
    //Get the 'sender'/'message' contained within the JSON object
    json_get_string(jsonData, sender, sizeof sender, "sender");
    json_get_string(jsonData, message, sizeof message, "message");
    
    //Output the data
    new string[144];
    format(string, sizeof string, "** [Discord] %s: %s **", sender, message);
    printf(string);
    SendAdminChatMessage(0xFEB918FF, string);
    return 1;
}

/*
    @Author: dy1zan
    @Command: !admins
    Returns a list of online administrators to Discord (the same
    channel at which the !admins command was used)
*/


DiscordResponse:stats(data[]) {
    new JSONNode:jsonData = json_parse_string(data);
    new user[MAX_PLAYER_NAME+1];
    json_get_string(jsonData, user, sizeof user, "user");
    new player;
    new result = sscanf(user, "u", player);
    if(result || !IsPlayerConnected(player)) {
        Discord:sendBasicRequest(DISCORD_ADMINCMDS_CHANNEL, "Player Statistics...", "That player is not connected, or is invalid.", DISCORD_COLOR_INFO);
        return;
    }
    printf("%d", player);
    
    ShowStatsDiscord(player);
}


/*
    Some util stuff
*/
stock Discord:GetAdminRank(playerid) {
    new name[32];
    new rank = PlayerInfo[playerid][pAdmin];
	switch(rank) {
		case 0: name = "None";
		case 1: name = "Probationary Administrator";
		case 2: name = "Junior Administrator";
		case 3: name = "General Administrator";
		case 4 .. 1336: name = "Senior Administrator";
		case 1337: name = "Head Administrator";
		case 99999: name = "Executive Administrator";
		case 999999: name = "Executive Administrator";
		default: name = "?";
	}
    return name;
}


DiscordResponse:prison(data[])
{
    new JSONNode:jsonData = json_parse_string(data),
        sender[128],
        user[MAX_PLAYER_NAME+1],
        ptime,
        reason[20],
        Pid,
        str[128]
        ;

    json_get_string(jsonData, sender, sizeof sender, "sender");
    json_get_string(jsonData, user, sizeof user, "player");
    ptime = json_get_int(jsonData, "ptime");
    json_get_string(jsonData, reason, sizeof reason, "reason");

    new TrgID = sscanf(user, "u", Pid);

    if( TrgID || !IsPlayerConnected(Pid)){
        Discord:sendBasicRequest(DISCORD_ADMINCMDS_CHANNEL, "Prison system", "That player is not connected, or is invalid.", DISCORD_COLOR_INFO);
        return;
    }


    TogglePlayerControllable(Pid, true);

    if(WatchingTV[Pid])
        WatchingTV[Pid] = false;
        
    if(ptime != 0)
    {
        format(str, sizeof(str), "{FF6347}SERVER: %s has been prisoned by an administrator, reason: %s", PlayerOOCName(Pid), reason);
        SendClientMessageToAll(COLOR_WHITE, str);

        format(str, sizeof(str), "You have been prisoned for %d minutes. (/time)", ptime);
        SendClientMessage(Pid, COLOR_LIGHTBLUE, str);
    }

    displayCenterHUDInfo(Pid, "~w~Welcome to ~n~~r~admin prison.", 8);

    ClearGuns(Pid);
    ResetPlayerWeapons(Pid);
    PlayerInfo[Pid][pWantedLevel] = 0;
    SetPlayerVirtualWorld(Pid, 0);
    PlayerInfo[Pid][pVirtualWorld] = 0;
    SetPlayerInterior(Pid, 69);
    PlayerInfo[Pid][pInt] = 69;
    format(str, sizeof(str), "%s", sender);
    strmid(PlayerInfo[Pid][pPrisonedBy], str, 0, strlen(str), 255);
    format(str, sizeof(str), "%s", reason);
    strmid(PlayerInfo[Pid][pPrisonReason], str, 0, strlen(str), 255);
    SetPlayerColor(Pid, TCOLOR_PRISON);
    PlayerInfo[Pid][pJailed] = 3;
    PlayerInfo[Pid][pAdminJailed] = 1;
    PlayerInfo[Pid][pJailTime] = ptime * 60;
    SetPlayerSkin(Pid, 50);
    PlayerInfo[Pid][pModel] = 50;
    new rand = random(sizeof(PrisonSpawns));
    PrisonCell[Pid] = rand;
    SetPlayerPos(Pid, PrisonSpawns[rand][0], PrisonSpawns[rand][1], PrisonSpawns[rand][2]);
    SetPlayerFacingAngle(Pid, PrisonSpawns[rand][3]);
    Timer_FreezePlayer(Pid, FREEZE, ENTER_FREEZE);

    SavePrison(PlayerName(Pid), sender, reason, ptime);

}



DiscordResponse:kick(data[])
{
    new JSONNode:jsonData = json_parse_string(data),
        sender[128],
        user[MAX_PLAYER_NAME+1],
        reason[20],
        Pid,
        str[128],
        logstring[128],
        year, month, day
        ;

    json_get_string(jsonData, sender, sizeof sender, "sender");
    json_get_string(jsonData, user, sizeof user, "player");
    json_get_string(jsonData, reason, sizeof reason, "reason");

    new TrgID = sscanf(user, "u", Pid);

    if( TrgID || !IsPlayerConnected(Pid))
    {
        Discord:sendBasicRequest(DISCORD_ADMINCMDS_CHANNEL, "Kick system", "That player is not connected, or is invalid.", DISCORD_COLOR_INFO);
        return;
    }
    getdate(year, month, day);
    format(logstring, sizeof(logstring), "AdmCmd: %s was kicked by %s, reason: %s (%d-%d-%d).", Pid, sender, reason, month, day, year);
    format(str, sizeof(str), "{FF6347}SERVER: %s was kicked by %s, reason: %s", PlayerICName(Pid), sender, reason);
    SendClientMessageToAll(COLOR_WHITE, str);
    KickLog(logstring);
    KickEx(Pid);

}



DiscordResponse:w(data[])
{
    new JSONNode:jsonData = json_parse_string(data),
        user[MAX_PLAYER_NAME+1],
        message[128],
        Pid,
        str[128]
        ;

    json_get_string(jsonData, user, sizeof user, "player");
    json_get_string(jsonData, message, sizeof message, "message");

    new TrgID = sscanf(user, "u", Pid);

    if( TrgID || !IsPlayerConnected(Pid))
    {
        Discord:sendBasicRequest(DISCORD_ADMINCMDS_CHANNEL, "Whisper system", "That player is not connected, or is invalid.", DISCORD_COLOR_INFO);
        return;
    }

    format(str, sizeof(str), "A discord admin whispers: %s", message);
    SendClientMessage(Pid, COLOR_YELLOW, str);

}

DiscordResponse:getbanreason(data[])
{

    new JSONNode:jsonData = json_parse_string(data),
        name[MAX_PLAYER_NAME+1];

    json_get_string(jsonData, name, sizeof name, "name");

    new query[128];
    mysql_format(sqlGameConnection, query, sizeof(query), "SELECT `Reason`, `Date`, `BannedBy` FROM `banlist` WHERE `Name` = '%e'", name);
    mysql_pquery(sqlGameConnection, query, "OnDiscordGetBanReason", "s", name);    
}


forward OnDiscordGetBanReason(playername[]);
public OnDiscordGetBanReason(playername[])
{
    if(cache_num_rows() == 0) return Discord:sendBasicRequest(DISCORD_ADMINCMDS_CHANNEL, "Ban reason", "That account isn't banned via the sql banlist.", DISCORD_COLOR_INFO);

    new reason[128], date[128], Banby[128];
    cache_get_field_content(0, "Reason", reason);
    cache_get_field_content(0, "Date", date);
    cache_get_field_content(0, "BannedBy", Banby);

    new str[128];
    format(str, sizeof(str), "Banlist: %s \nReason: %s \nDate: %s \nBanned by %s", playername, reason, date, Banby);

    Discord:sendBasicRequest(DISCORD_ADMINCMDS_CHANNEL, "Ban reason", str, DISCORD_COLOR_INFO);

    return 1;
}


