
//Make sure this include isn't already included...
#if defined DISCORD_INCLUDED
    #endinput
#endif

#define DISCORD_INCLUDED

//Declare the namespace
#define Discord:   Discord_

/*

    @Author Dylan/Dy1zan
    @Date 02/06/2018

    This file is an include adding discord functionality
    to the Python middle-man server.

    Dependencies:
    1.  a_samp
    2.  socket
    3.  SAMPSON (a_json)

*/


/* ====================================================================
    Definitions & Variables
=======================================================================*/

//Used to define new message types sent from Discord
#define DiscordResponse:%1(%2) \
            forward dresponse_%1(%2);    \
            public dresponse_%1(%2)

#define MAX_DISCORD_STRING  144
#define MAX_DISCORD_BUFFER  1024

static Socket:g_socket;
static bool:g_alive;
static g_ip[45];
static g_port;

forward Discord:reconnect();


/* ====================================================================
    Functions - Initalization & Deinitalization
=======================================================================*/

Discord:connect(ip[], port) {

    format(g_ip, sizeof g_ip, "%s", ip);
    g_port = port;

	g_socket = socket_create(TCP);
	Log:info("Discord", "Connecting to Discord...");
    if(!is_socket_valid(g_socket)) {
        Log:info("Discord", "An error has occured when creating the socket.");
        return;
    }

    socket_connect(g_socket, ip, port);

    /*
        socket_connect does not gurantee successful connection.
        It is also asynchronous.
        Therefore, onSocketAnswer needs to be used, where the server
        replies with a "hey, I am connected"
    */
    g_alive = true;
    Log:info("Discord", "Successfully connected to the Discord middle-man on %s:%d.", ip, port);
}

Discord:disconnect() {
    if(g_alive) socket_destroy(g_socket);
    return true;
}


/* ====================================================================
    SOCKET CALLBACKS
=======================================================================*/

public onSocketClose(Socket: id) {
	//Connection lost TODO: automatically reconnect (requires Timer)
    Log:info("Discord", "The connection to the Discord middle-man has been closed. Attempting to reconnect...");
    g_alive = false;
    SetTimer("Discord_reconnect", 8000, false);
	return true;
}

public Discord:reconnect() {

    if(!is_socket_valid(g_socket)) {
        g_socket = socket_create(TCP);
    }

    new connected = socket_connect(g_socket, g_ip, g_port);
    if(connected) Log:info("Discord", "Successfully reconnected to the Discord middle-man on %s:d", g_ip, g_port);
    g_alive = true;
}

/*
    Invoked when there is an incoming message from Discord.
    Reads the 'type' of Discord message, and then passes it to
    the implemented function e.g DiscordResponse:[type](...)
*/
public onSocketAnswer(Socket:id, data[], data_len) {
    new JSONNode:jsonData = json_parse_string(data);
    if(!jsonData) return false;
    
    new type[20], func[32];

    json_get_string(jsonData, type, sizeof type, "type");

    //invoke the function defined by the response type
    format(func, sizeof func, "dresponse_%s", type);
    CallLocalFunction(func, "s", data);
    
    Log:debug("Discord:onSocketAnswer", "%s", data);
    return true;
}

/* ====================================================================
    In addition to SAMPSON,
=======================================================================*/
Discord:json_escape(const string[], output[], len) {
    new pos, c, i;

    while((c = string[pos++]) > 0) {
        if(i == len) {
            i = 0;
            break;
        }

        if(c == '"' || c == '\\') {
            if((i + 2) > len) {
                i = 0;
                break;
            }

            output[i++] = '\\';
            output[i++] = c;
        }
        else {
            output[i++] = c;
        }
    }

    output[i] = EOS;
    return i;

}
/* ====================================================================
    BASIC REQUEST TYPES
=======================================================================*/
/*
    This sends a basic request to Discord. This will output the content
    as a basic formatted embedded message. e.g !admins
*/
stock Discord:sendBasicRequest(channel[], title[], message[], color) {
    //Escape the input...
    new 
        escapedTitle[MAX_DISCORD_STRING],
        escapedMessage[MAX_DISCORD_STRING];
    Discord:json_escape(message, escapedMessage, MAX_DISCORD_STRING);
    Discord:json_escape(title, escapedTitle, MAX_DISCORD_STRING);

    //Produce JSON and send it to the Python server.
	new output[MAX_DISCORD_BUFFER];
	format(output, sizeof output, "{\"type\":\"basic\",\"color\":\"%i\", \"channel\":\"%s\", \"title\":\"%s\",\"message\":\"%s\", \"time\":\"%s\"}\r\n", color, channel, escapedTitle, escapedMessage, Discord:GetTimeString());
	socket_send(g_socket, output, strlen(output));
	return 1;
}

/*
    This function outputs to Discord without an embed.
*/
stock Discord:sendTableMessage(channel[], title[], message[], color) {
    //Escape the input...
    new 
        escapedTitle[MAX_DISCORD_STRING],
        escapedMessage[MAX_DISCORD_STRING];
    Discord:json_escape(message, escapedMessage, MAX_DISCORD_STRING);
    Discord:json_escape(title, escapedTitle, MAX_DISCORD_STRING);

    //Produce JSON & send it to the Python server
	new output[MAX_DISCORD_BUFFER];
	format(output, sizeof output, "{\"type\":\"table\",\"color\":\"%i\", \"channel\":\"%s\", \"title\":\"%s\",\"message\":\"%s\", \"time\":\"%s\"}\r\n", color, channel, escapedTitle, escapedMessage, Discord:GetTimeString());
	socket_send(g_socket, output, strlen(output));
	return 1;
}

/*
    This function relates a player to the request sent to Discord. Similar to
    {@link sendBasicRequest}, except includes the player's name.
*/
stock Discord:sendPlayerRequest(channel[], playerid, title[], message[], color) {
    //Escape the input...
    new 
        escapedTitle[MAX_DISCORD_STRING],
        escapedMessage[MAX_DISCORD_STRING];
    Discord:json_escape(message, escapedMessage, MAX_DISCORD_STRING);
    Discord:json_escape(title, escapedTitle, MAX_DISCORD_STRING);

    //Produce JSON & send it to the Python server
	new output[MAX_DISCORD_BUFFER];
	format(output, sizeof output, "{\"type\":\"basic_player\",\"color\":\"%i\", \"channel\":\"%s\", \"title\":\"%s\", \"player\":\"%s\",\"message\":\"%s\", \"time\":\"%s\"}\r\n", color, channel, escapedTitle, PlayerICName(playerid), escapedMessage, Discord:GetTimeString());
	socket_send(g_socket, output, strlen(output));
	return 1;
}


/* ====================================================================
    UTIL
=======================================================================*/
/*
	Gets the current server time as a string.
    Dependency: FixHour (SA:RP gamemode)
*/
stock Discord:GetTimeString() {
    new string[10];
    new h, m, s;
    gettime(h, m, s);
    h = FixHour(h);
    format(string, sizeof(string), "%02d:%02d:%02d", h, m, s);
    return string;
}
