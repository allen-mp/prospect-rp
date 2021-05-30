# Documentation
Welcome to the Discord-SARP connector documentation. First, this addon uses JSON, PAWN and Python. This document covers the following areas...

Questions? Ask Dy1zan.

## Index
* Setup
* Sending data to Discord
* Adding Discord commands
* Customizing Discord output


## Setup
The following steps are required to have a runnable gamemode...
1. Assure SAMPSON.so or SAMPSON.dll is located in server.cfg under the plugins line.
2. Likewise, assure the socket.so or socket.dll plugin is also in server.cfg.

The following steps are required to make the bot work.

3. Copy the settings.py from the private repository (in `inc/discord`) and place it in the same directory as main.py.
4. Run ./start.sh, or in Windows simply use python3 ./{PATH TO ASYNCBOT}/main.py. Please assure you have Discord.PY installed for Python3. **The bot must be running before the server is started. This will be fixed later.**


## Sending data to Discord
You may need to send messages to Discord, such as /a messages. There are pre-defined functions for you. But first, assure you have `#include <discord>`

`stock Discord:sendBasicRequest(channel[], title[], message[], color)`

OR

`stock Discord:sendPlayerRequest(channel[], playerid, title[], message[], color)`

If you need anymore functionality to this, please contact Dy1zan. I will be implementing `stock Discord:sendPlayerPMRequest(user[], playerid, title[], message[], color)`. Sorry, I guess the 'Request' part in these functions do not make sense, and will be changed. Consider these three functions 'messages' which are sent to Discord, and formatted in a particular way.

## Adding Discord commands
### 1. Python
This part requires some Python. It's not too difficult and not much knowledge is required. Now, rather than confuse you, I'll show you some code. You'll just have to accept what you read below for the time being...
```Python
@bot.command(pass_context=True)
async def admins(ctx):
    channel = ctx.message.channel
    out = json.dumps({
        "type":"admins",
        "channel":str(channel.id)
    })

    global s
    await s.write(out)
```

First, `@bot.command` defines this as a new Discord command. In this case, the  function name: `admins` is used as the command name. e.g, !admins. `pass_context=True` passes some 'data' to the command, as the variable `ctx`. `ctx` can be used to get the author, channel, or other message data.

Next, JSON: this is just the data we're going to send to the SA:MP server. In this case, we say the type is 'admins', and then we also send the channel so we know what channel to reply to.

```Python
global s
await s.write(out)
```
This just sends the JSON data to the server. This is going to be changed to make it more simple.

### 2. PAWN
Once we have sent the JSON data to the SA:MP server, we then need to tell the SA:MP server what to do with it.

In this case, we declare a new `DiscordResponse`. Consider the following an example for `!admins`:
```C
//Define a callback for the response type, 'admins'
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
    Discord:sendBasicRequest(channel, "Server administrators", string, 0x58aecb);
}
```
There are four steps here,

1. Define `DiscordResponse:admins`, this means this will be invoked once the JSON data has been sent (remember, the 'type' was admins in the JSON data that was sent).
2. Get the JSON data sent. In this case, the channel.
3. Do stuff (in this case, get a list of online admins).
4. Send the list of online admins back to Discord.

## Customizing Discord output
This bit is optional learning, and allows you to customize the way messages are formatted and displayed in Discord. This bit also requires a bit of Python.

Here is a default output type... the basic type. This has an embedded message as you would see when using the !admins command.

```Python
@request_type
async def basic(server, data):
    print("Invoking basic request output to Discord...")
    embed = discord.Embed(title=data['title'], description=data['message'], color=discord.Colour(int(data['color'])))
    embed.set_footer(text=data['time'])
    await server.bot.send_message(discord.Object(id=data['channel']), embed=embed)
```

`@request_type` defines a new output type, and note that the name of the function is yet again used. As you can see below, JSON is sent to the Python server... and type = 'basic'.

```C
/*
    This sends a basic request to Discord. This will output the content
    as a basic formatted embedded message. e.g !admins
*/
stock Discord:sendBasicRequest(channel[], title[], message[], color) {
	new output[450];
	format(output, sizeof output, "{\"type\":\"basic\",\"color\":\"%i\", \"channel\":\"%s\", \"title\":\"%s\",\"message\":\"%s\", \"time\":\"%s\"}\r\n", color, channel, title, message, Discord:GetTimeString());
	socket_send(g_socket, output, strlen(output));
	return 1;
}
```

**Important Notice:** All user input should be escaped, in particular the characters: \\, ". That is, the escape character and a quote mark.