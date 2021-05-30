//====== INCLUDES & DEFINES =======//
#include <a_samp>
#include <zcmd>
#include <sscanf2>
#include <foreach>
#include <streamer>

#define COLOR_RED


#define SCM    SendClientMessage //defining SendClientMessage to be more easier to write it and to stop wasting time in writing it completely.
#define SCMTA  SendClientMessageToAll //same to SendClientMessage's story.
//======== EVENT VARIABLES =========//
new
InEvent[MAX_PLAYERS],  //Checking player if he is in/out event
EventOpened[MAX_PLAYERS], //Checking if Event is opened to join it.
EventStarted[MAX_PLAYERS],
Float:RandomEventSpawns[][] =
{
    {-3398.3511,1567.3708,16.0874},
    {-3398.4805,1569.2455,15.9844},
    {-3402.8857,1570.5845,15.8978}
}; //This is Random Event spawns.
//======== FORWARD =========//
forward EventStart(); //we did forwarded this as timer to event stats.
new CP_EVENT;
forward loser(playerid);
//======= COMMANDS AND MAP AND EVENT THINGYS GOES HERE! ========//
CMD:parkourevent(playerid, params[])
{
    if(IsPlayerAdmin(playerid))
    {
        if(EventOpened[playerid] == 0)
        {
            new string[128];
            if(sscanf(params,"i")) SCM(playerid,-1,"{fdfe1d}Usage: {FFFFFF}/startevent");
            EventOpened[playerid] = 1;
            EventStarted[playerid] = 0;
            SCMTA(-1,"{800080}[EVENT INFO] {F033FF}An Administrator has opened Parkour Event!");
            format(string,sizeof(string),"{800080}[EVENT INFO] {fef65b}Type {2faced}/joinparkour {fef65b}to join the Parkour Event!");
            SCMTA(-1,string);
            GameTextForAll("~R~Parkour ~W~EVENT ~G~OPENED!",3000,3);
            SetTimerEx("EventStart", 1000, false, "i", playerid);
        }
        else
        {
            if(EventOpened[playerid] == 1)
            {
                SCMTA(-1,"{800080}[EVENT INFO] {fef65b}An Administrator has closed the Parkour Event, and started it!");
                EventOpened[playerid] = 0;
                foreach( new i: Player )
				{
				if(IsPlayerConnected(i))
				{
				TogglePlayerControllable(i, 1);
				InEvent[playerid] = 1;
				EventStarted[playerid] = 1;
				GameTextForPlayer(i, "~W~EVENT ~Y~HAS ~G~BEEN ~B~STARTED!", 3000, 3);
				}
				}
            }
        }
    }
    else
    {
        SCM(playerid, -1,"{F00f00}[ERROR]: {FFFFFF}You aren't authorized to use this command!");
    }
    return 1;
}

CMD:startparkour(playerid, params[])
{
   if(EventOpened[playerid] == 1)
   {
   if(IsPlayerAdmin(playerid))
    {

for(new i = 0; i<MAX_PLAYERS; i++)
{
if(IsPlayerConnected(i))
{
TogglePlayerControllable(i, 1);
SCMTA(0xf8f8f8fff,"[EVENT STARTED] {F033FF}An Administrator has Started the Parkour Event!");
GameTextForPlayer(i, "~W~EVENT ~Y~HAS ~G~BEEN ~B~STARTED!",3000,3);
EventOpened[playerid] = 0;
InEvent[playerid] = 1;
}
}
   }
   else
   {
   SCM(playerid, 0xf8f8f8fff,"[ERROR]: {FFFFFF}You aren't authorized to use this command!");
   }
   }
   else
   {
   SendClientMessage(playerid, 0xf8f8f8fff,"ERROR: {FFFFFF}There isn't any opened event at moment to start!");
   }
   return 1;
}
CMD:fire(playerid,params[])
{
    new ID, string[128], tname[MAX_PLAYER_NAME];
    GetPlayerName(ID, tname,sizeof(tname));
    if(sscanf(params,"is",ID,params)) return SCM(playerid,-1,"{fdfe1d}Syntax: {FFFFFF}/fire <id> <reason>");
    if(!IsPlayerAdmin(playerid)) return  SCM(playerid, -1, "{F00f00}[ERROR]: {FFFFFF}You are not authorized to use this command!");
    if(!IsPlayerConnected(ID)) return SCM(playerid, -1, "{f00f00}[ERROR]: {FFFFFF}Player isn't connected!");
    if(InEvent[ID] == 0) return SCM(playerid, -1, "{f00f00}[ERROR]: {FFFFFF}Player isn't in the Event!");
    format(string,sizeof(string),"{800080}[EVENT FIRE] {Fef65b}%s has been fired from the  Event! {f00f00}[REASON: %s]",tname, params);
    SCMTA(-1,string);
    InEvent[ID] = 0;
    SpawnPlayer(ID);
    return 1;
}


CMD:joinparkour(playerid,params[])
{
    if(InEvent[playerid] == 1) return SendClientMessage(playerid,-1,"{f00f00}[ERROR]: {FFFFFF}You're already in the Parkour Event!");
    if(EventOpened[playerid] == 0) return SendClientMessage(playerid, -1,"{f00f00}[ERROR]: {FFFFFF}there isn't any Parkour Event at the moment!");
    new Rrandom = random(sizeof(RandomEventSpawns)), name[MAX_PLAYER_NAME], id, string[128];
    GetPlayerName(id, name,sizeof(name));
    SetPlayerPos(playerid,RandomEventSpawns[Rrandom][0], RandomEventSpawns[Rrandom][1], RandomEventSpawns[Rrandom][2]);
    SetPlayerVirtualWorld(playerid,1);
    TogglePlayerControllable(playerid, 0);
    SetPlayerArmour(playerid, 100.0);
    SetPlayerHealth(playerid, 100.0);
    InEvent[playerid] = 1;
	EventOpened[playerid] = 1;
    format(string,sizeof(string),"{2faced}Event: {fef65b}%s(%i) {800080}has joined the Parkour event!",name,id);
    SendClientMessageToAll(-1,string);
    GameTextForPlayer(playerid, "~W~Event will start in ~P~Minutes!",3000,3);
    return 1;
}
CMD:eventenablemotherfucker(playerid)
{
    SetTimerEx("EventStart", 1000, false, "i", playerid);
    return 1;
}
public EventStart()
{
    CreateObject(9741, -3395.04, 1575.80, -9.33,   356.86, 0.00, 3.14);
	CreateObject(8483, -3386.99, 1573.39, 20.26,   0.00, 0.00, 180.00);
	CreateObject(16113, -3393.07, 1541.06, 14.58,   0.00, 0.00, 55.00);
	CreateObject(16113, -3383.60, 1598.03, 14.58,   0.00, 0.00, 207.00);
	CreateObject(16113, -3370.63, 1568.23, 14.58,   0.00, 0.00, 134.00);
	CreateObject(7916, -3387.28, 1578.05, 28.07,   0.00, 0.00, 276.19);
	CreateObject(3374, -3418.60, 1571.03, 21.19,   0.00, 0.00, 0.00);
	CreateObject(3374, -3413.39, 1570.99, 15.75,   0.00, 0.00, 0.00);
	CreateObject(3374, -3415.64, 1571.01, 18.53,   0.00, 0.00, 0.00);
	CreateObject(3743, -3426.89, 1570.70, 26.31,   0.00, 0.00, -90.00);
	CreateObject(3578, -3438.25, 1570.69, 28.22,   0.00, 0.00, 0.00);
	CreateObject(3578, -3448.54, 1570.71, 28.22,   0.00, 0.00, 0.00);
	CreateObject(3578, -3476.65, 1570.79, 35.39,   0.00, 0.00, 0.00);
	CreateObject(3578, -3457.87, 1570.70, 29.90,   0.00, 22.00, 0.00);
	CreateObject(3578, -3467.12, 1570.73, 33.61,   0.00, 22.00, 0.00);
	CreateObject(13562, -3484.42, 1571.33, 28.83,   0.00, 0.00, 0.00);
	CreateObject(13562, -3489.39, 1571.13, 28.83,   0.00, 0.00, 0.00);
	CreateObject(13562, -3493.96, 1571.03, 28.83,   0.00, 0.00, 0.00);
	CreateObject(13562, -3499.09, 1570.69, 28.83,   0.00, 0.00, 0.00);
	CreateObject(13562, -3502.60, 1575.25, 28.83,   0.00, 0.00, 0.00);
	CreateObject(13562, -3504.43, 1568.19, 28.83,   0.00, 0.00, 0.00);
	CreateObject(13562, -3508.67, 1573.01, 28.83,   0.00, 0.00, 0.00);
	CreateObject(12990, -3525.56, 1572.63, 35.28,   0.00, 0.00, 90.00);
	CreateObject(3565, -3549.56, 1573.06, 46.74,   0.00, 0.00, 90.00);
	CreateObject(3565, -3540.14, 1573.04, 36.59,   0.00, 0.00, 90.00);
	CreateObject(3565, -3542.41, 1573.02, 39.09,   0.00, 0.00, 90.00);
	CreateObject(3565, -3544.67, 1573.06, 41.73,   0.00, 0.00, 90.00);
	CreateObject(3565, -3547.07, 1573.06, 44.32,   0.00, 0.00, 90.00);
	CreateObject(3502, -3555.02, 1572.58, 49.72,   0.00, 0.00, 90.00);
	CreateObject(3502, -3563.59, 1572.59, 49.72,   0.00, 0.00, 90.00);
	CreateObject(3502, -3571.99, 1572.60, 49.72,   0.00, 0.00, 90.00);
	CreateObject(3631, -3580.30, 1572.52, 47.57,   0.00, 0.00, 0.00);
	CreateObject(3631, -3588.83, 1572.58, 47.57,   0.00, 0.00, 0.00);
	CreateObject(16481, -3610.71, 1572.54, 52.65,   0.00, 0.00, 180.00);
	CreateObject(11496, -3636.76, 1572.38, 58.12,   0.00, 0.00, 90.00);
	CreateObject(18841, -3654.36, 1588.89, 63.18,   90.00, 0.00, 0.00);
	CreateObject(974, -3652.45, 1601.60, 58.77,   0.00, 0.00, 292.13);
	CreateObject(974, -3650.89, 1574.84, 58.27,   0.00, 0.00, 66.04);
	CreateObject(974, -3656.81, 1579.95, 58.27,   0.00, 0.00, 27.39);
	CreateObject(3578, -3659.34, 1585.86, 59.65,   0.00, -41.00, 97.00);
	CreateObject(974, -3658.97, 1589.33, 60.31,   0.00, 0.00, 348.48);
	CreateObject(13749, -3642.66, 1594.37, 63.44,   0.00, 0.00, 229.48);
	CreateObject(16644, -3630.29, 1582.77, 69.97,   0.00, 0.00, 358.73);
	CreateObject(16640, -3614.71, 1587.51, 73.33,   0.00, 0.00, 0.00);
	CreateObject(18984, -3672.57, 1592.72, 78.84,   0.00, 0.00, 90.00);
	CreateObject(18827, -3770.09, 1592.72, 78.84,   0.00, 0.00, 90.00);
	CreateObject(6052, -3828.30, 1586.32, 76.26,   0.00, 0.00, 276.12);
	CreateObject(13648, -3837.85, 1557.47, 78.45,   0.00, 0.00, 355.16);
	CreateObject(18566, -3839.71, 1534.08, 78.88,   0.00, 0.00, 178.00);
	CreateObject(18566, -3839.91, 1531.81, 78.88,   0.00, 0.00, 359.36);
	CreateObject(18367, -3840.03, 1529.99, 78.39,   0.00, 0.00, 0.00);
	CreateObject(13562, -3840.29, 1498.08, 75.10,   0.00, 0.00, 0.00);
	CreateObject(13562, -3843.33, 1493.76, 75.10,   0.00, 0.00, 0.00);
	CreateObject(13562, -3836.10, 1493.40, 75.10,   0.00, 0.00, 0.00);
	CreateObject(13562, -3833.32, 1487.82, 75.10,   0.00, 0.00, 0.00);
	CreateObject(13562, -3839.72, 1487.27, 75.10,   0.00, 0.00, 0.00);
	CreateObject(13562, -3845.92, 1486.28, 75.10,   0.00, 0.00, 0.00);
	CreateObject(13562, -3835.87, 1481.11, 75.10,   0.00, 0.00, 0.00);
	CreateObject(13562, -3842.48, 1480.49, 75.10,   0.00, 0.00, 0.00);
	CreateObject(13562, -3838.71, 1475.13, 75.10,   0.00, 0.00, 0.00);
	CreateObject(18367, -3838.07, 1472.80, 81.83,   0.00, 0.00, 0.00);
	CreateObject(18767, -3835.02, 1437.17, 90.32,   0.00, 0.00, 180.00);
	CreateObject(18762, -3832.03, 1442.79, 92.72,   0.00, 0.00, 0.00);
	CreateObject(16644, -3825.04, 1444.75, 95.22,   0.00, 0.00, 0.00);
	CreateObject(18783, -3807.29, 1397.97, 101.66,   0.00, 0.00, 0.00);
	CreateObject(18762, -3807.17, 1431.31, 96.30,   0.00, 0.00, 0.00);
	CreateObject(18762, -3805.81, 1432.81, 95.17,   0.00, 0.00, 0.00);
	CreateObject(18762, -3803.61, 1429.18, 97.22,   0.00, 0.00, 0.00);
	CreateObject(18762, -3804.59, 1431.22, 96.30,   0.00, 0.00, 0.00);
	CreateObject(18762, -3805.76, 1429.17, 97.22,   0.00, 0.00, 0.00);
	CreateObject(18762, -3808.08, 1429.01, 97.22,   0.00, 0.00, 0.00);
	CreateObject(18786, -3807.29, 1417.74, 101.66,   0.00, 0.00, 90.00);
	CreateObject(18783, -3805.26, 1443.27, 92.68,   0.00, 0.00, 0.00);
	CreateObject(3533, -3801.31, 1398.64, 103.54,   90.00, 0.00, -7.00);
	CreateObject(3533, -3811.28, 1398.35, 103.54,   90.00, 0.00, 0.00);
	CreateObject(3533, -3799.93, 1398.71, 103.54,   90.00, 0.00, 22.00);
	CreateObject(3533, -3813.19, 1398.15, 103.54,   90.00, 0.00, 22.00);
	CreateObject(3533, -3804.84, 1398.37, 103.54,   90.00, 0.00, -3.00);
	CreateObject(3533, -3814.73, 1398.18, 103.54,   90.00, 0.00, 0.00);
	CreateObject(3533, -3803.13, 1398.55, 103.54,   90.00, 0.00, 22.00);
	CreateObject(3533, -3807.82, 1398.53, 103.54,   90.00, 0.00, 0.00);
	CreateObject(3534, -3808.04, 1391.44, 104.95,   0.00, 0.00, 0.00);
}
public OnGameModeInit()
{
   // Create3DTextLabel("Winner", 0xFFFFFFFF, -3806.7693,1392.0258,105.1600, 30, 10, 1);
    return 1;
}

public OnPlayerConnect(playerid)
{
    SendClientMessage(playerid, -1, "{fef65b}Parkour Event System Loaded!");
    SendClientMessage(playerid, -1, "{95fa7f}it's Created by RxErT!");
    SendClientMessage(playerid, -1, "{0099cc}Type /eventcmds to get available commands!");
    return 1;
}


public OnPlayerSpawn(playerid)
{
    CP_EVENT = CreateDynamicCP(-3806.7693,1392.0258,105.1600, 2.5, .interiorid = 0); /// Creating Event Win CP
    InEvent[playerid] = 0;
    TogglePlayerControllable(playerid, 1);
    SetPlayerVirtualWorld(playerid, 0);
    return 1;
}
public OnPlayerEnterDynamicCP(playerid, checkpointid)
{
    new name[MAX_PLAYER_NAME], string[24+MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));
    if(checkpointid == CP_EVENT)
    {
        format(string, sizeof(string), "{fef65b}%s Has won the Parkour Event Rewarded $50,00!", name);
        SendClientMessageToAll(-1, string);
        SpawnPlayer(playerid);
        GivePlayerMoney(playerid, 50000);
        DestroyDynamicCP(checkpointid);
    }
    return 1;
}
CMD:eventcmds(playerid, params[])
{
    if(IsPlayerAdmin(playerid))
    {
        new string[500];
        strcat(string, "{fef65b}/parkourevent {1ffdc4}-> {fd1f58}to create the Parkour event And Close Event!\n");
        strcat(string, "{fef65b}/startparkour {1ffdc4}-> {fd1f58}To unfreeze All players in Event start the event!\n");
        strcat(string, "{fef65b}/fire{1ffdc4} -> {fd1f58}to fire/kick someone from the Parkour event!\n");
        strcat(string, "{bbbbee}This Event system was created By: {fdfe1d}RxErT!");
        ShowPlayerDialog(playerid, 1884, DIALOG_STYLE_MSGBOX,"Pakrour Event - Commands", string, "Ok", "");
    }
    else
    {
        SCM(playerid, -1, "{F00f00}[ERROR]: {FFFFFF}You are not authorized to use this command!");
    }
    return 1;
}
