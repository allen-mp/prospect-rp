#include <discord>

/* ====================================================================
    Discord colour definitions here only please
=======================================================================*/
#define DISCORD_COLOR_INFO 0x58aecb

/* ====================================================================
    Channel definitions here only please
=======================================================================*/
#define DISCORD_ADMINCHAT_CHANNEL   "465873855672745985"
#define DISCORD_ADMINCMDS_CHANNEL   "465875438321795074"

#define DISCORD_NEWBIE_CHANNEL  "465874164960460800"
#define DISCORD_TESTER_CHANNEL "465874413267582986"
#define DISCORD_BUG_CHANNEL     "465879591656095754"

//=====================================================================

Hook:Discord_OnGameModeInit() {
    Discord:connect("127.0.0.1", 7778);
    return 1;
}

Hook:Discord_OnGameModeExit() {
    Discord:disconnect();
    return 1;
}

CMD:dconnect(playerid, params[]) {
    if(PlayerInfo[playerid][pDev] > 2) {
        SendClientMessage(playerid, -1, "You have requested the SA:MP server to reconnect to the Discord bot.");
        Discord:reconnect();
    }
    return 1;
}


/* ====================================================================
    Include any files containing message types e.g what should the
    server do when !admins is sent on Discord?
=======================================================================*/
#include "inc/discord/admin.inc"
#include "inc/discord/developer.inc"
#include "inc/discord/player.inc"

//=====================================================================