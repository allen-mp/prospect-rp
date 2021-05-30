/*
    This file adds functionality to developer commands sent
    via Discord, e.g !dt and some others
    Please continue using the format used below for each command.
*/

/*
    @Author: dy1zan / Golden
    @Command: !dt
    This callback is invoked whenever someone types !dt in Discord.
    This will send an a new chat message in-game, of the message sent from Discord.
*/


SendDiscordDevTesterMessage(color, string2[])
{
	foreach( new i: Player )
	{
		if(PlayerInfo[i][pDev] >= 1 || PlayerInfo[i][pTester] >= 1)
		{
			SendClientMessage(i, color, string2);
		}
	}
}
DiscordResponse:dt(data[]) {
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

    SendDiscordDevTesterMessage(0x559F75FF, string);
    return 1;
}

CMD:dt(playerid, params[])
{
	new msg[128], str[128], rank[10], dstr[128];

	if(sscanf(params, "s[128]", msg)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dt [message]");
	if(PlayerInfo[playerid][pDev] < 1 && PlayerInfo[playerid][pTester] < 1) return AdmErrorMsg;

	if(PlayerInfo[playerid][pDev] >= 1)  format(rank, sizeof(rank), "Developer");
	else if (PlayerInfo[playerid][pTester] >= 1)  format(rank, sizeof(rank), "Tester");

	format(str, sizeof(str), "%s %s : %s", rank, PlayerICName(playerid), msg);

    format(dstr, sizeof(dstr), "%s : %s", PlayerICName(playerid), msg);

	SendDiscordDevTesterMessage(0x559F75FF, str);

	Discord:sendBasicRequest(DISCORD_TESTER_CHANNEL, rank, dstr, 0x3C9B93);
	return 1;
}