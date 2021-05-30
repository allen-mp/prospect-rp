
/*
 *      Player chat
 *
 */

forward Discord_OnPlayerSendReport(playerid, text[]);
forward Discord_OnAdminSendAMessage(playerid, alevel[], text[]);
forward Discord_OnPayLog(text[]);
forward Discord_OnPlayerGov(color, text[]);
//stock Discord_GetTimeString() for returning string


CMD:linkdiscord(playerid, text[]) {
    //update later to allow them to change
    if(strcmp(PlayerInfo[playerid][pDiscord], "Not Linked", true) == 0) {
        return SendClientMessage(playerid, COLOR_GREY, "You have already connected your Discord account.");
    }
    SendClientMessage(playerid, COLOR_GREY, "Unfortuately, this is still being worked on. Check back later!");
    return 1;
}




//Report messages
public Discord_OnPlayerSendReport(playerid, text[]) {
    new string[300];

    format(string, sizeof(string), 
"!PREPORT!\
!TIME!%s!TIME!\
!PLAYER!%s!PLAYER!\
!MSG!%s!MSG!\r\n",
    Discord_GetTimeString(), PlayerICName(playerid), text
    );

    socket_send(g_socket, string, strlen(string));
}

public Discord_OnPlayerGov(color, text[]) {
    new string[128+100];

    format(string, sizeof(string),
"!GOVMSG!\
!TIME!%s!TIME\
!COLOR!%h!COLOR!\
!MSG!%s!MSG!\r\n",
    Discord_GetTimeString(), color, text
    );

    socket_send(g_socket, string, strlen(string));

}



//Admin messages
public Discord_OnAdminSendAMessage(playerid, alevel[], text[]) {
    new string[300];

    format(string, sizeof(string), 
"!AMSG!\
!TIME!%s!TIME!\
!PLAYER!%s %s!PLAYER!\
!MSG!%s!MSG!\r\n",
    Discord_GetTimeString(), alevel, PlayerICName(playerid), text
    );

    socket_send(g_socket, string, strlen(string));
}




//Pay log
public Discord_OnPayLog(text[]) {
    new string[300];

    format(string, sizeof(string), 
"!PAYLOG!\
!TIME!%s!TIME!\
!MSG!%s!MSG!", 
    Discord_GetTimeString(), text
    );

    socket_send(g_socket, string, strlen(string));
}




//Player chat
/*
Hook:discord_OnPlayerText(playerid, text[]) {
    new string[300];

    format(string, sizeof(string), 
"!PMSG!\
!TIME!%s!TIME!\
!PLAYER!%s!PLAYER!\
!MSG!%s!MSG!\r\n", 
    Discord_GetTimeString(), PlayerICName(playerid), text
    );

    socket_send(g_socket, string, strlen(string));
    return 1;
}*/