#include <socket>

#define SOCKET_IP       "0.0.0.0"
#define SOCKET_PORT     5000
#define MAX_CONNECTIONS 1

new Socket:g_socket;
new bool:g_alive;


static discord_connect() {
    g_socket = socket_create(TCP);
    socket_set_max_connections(g_socket, MAX_CONNECTIONS);
    socket_connect(g_socket, SOCKET_IP, SOCKET_PORT);
}

stock Discord_GetTimeString() {
    new string[10];
    new h, m, s;
    gettime(h, m, s);
    h = FixHour(h);
    format(string, sizeof string, "%d:%d:%d", h, m, s);
    return string;
}

Hook:discord_OnGameModeInit() {

    printf("connecting to discord...");
    discord_connect();

    return 1;
}

Hook:discord_OnGameModeExit() {

    if(g_alive) socket_destroy(g_socket);
    return 1;
}

CMD:rediscord(playerid, params[]) {
    if(PlayerInfo[playerid][pDev] > 2) {
        if(!g_alive) {
            discord_connect();
        }
    }
    return;
}

public onSocketAnswer(Socket:id, data[], data_len) {
    if(id == g_socket) {
        print(data);

        if(strcmp(data, "!AMSG!", true, 6) == 0) {
            print("entering AMSG recieved");
            new msgtype[7], author[15], msg[128], string[144];
            if(!sscanf(data, "p<&>s[7]s[15]s[128]", msgtype, author, msg)) { //not a blank msg
                printf("msgtype: %s, author: %s, msg: %s", msgtype, author, msg);
                format(string, sizeof(string), "** [Discord] %s: %s **", author, msg);
			    SendAdminChatMessage(0xFEB918FF, string);
            }
        }
        else if(strcmp(data, "!ADMINS!", true) == 0) {

            new string[200];
            format(string, sizeof string, "!ADMINS!!MSG!");
            new bool:first = true;
            for(new i; i < GetPlayerPoolSize()+1; i++) {

                if(PlayerInfo[i][pAdmin] > 0) {
                    if(first == true) {
                        format(string, sizeof(string), "%s%s (%d)", string, PlayerICName(i), PlayerInfo[i][pAdmin]);
                        first = false;
                    }
                    else {
                        format(string, sizeof string, "%s, %s (%d)", string, PlayerICName(i), PlayerInfo[i][pAdmin]);
                    }
                }
            }
            format(string, sizeof string, "%s!MSG!", string);
            socket_send(g_socket, string, strlen(string));
        }

        else if(strcmp(data, "!SERVERTIME!", true) == 0) {
            new string[60];
            format(string, sizeof string, "!SERVERTIME!!MSG!%s!MSG!", Discord_GetTimeString());
            socket_send(g_socket, string, strlen(string));
        }

        else if(strcmp(data, "!GMX!", true) == 0) {
            #if DEVMODE
            SaveChars();
            new string[100];
            format(string, sizeof string, "!ANN!!MSG!I have restarted the server for you, boss.!MSG!");
            socket_send(g_socket, string, strlen(string));
            SendRconCommand("gmx");
            #else
            new string[100];
            format(string, sizeof(string), "!ANN!!MSG!Faggot, the server isn't in development mode.!MSG!");
            socket_send(g_socket, string, strlen(string));
            #endif

        }

    }
    return 1;
}

public onSocketClose(Socket:id) {
    g_alive = false;
    return 1;
}

#include "inc\discord\discord_cmds.inc"