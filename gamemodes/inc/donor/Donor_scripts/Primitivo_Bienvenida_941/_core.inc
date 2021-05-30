/*
                                                  ______                          _   _        _____           _       _   _             
                                                 |  ____|                        | | ( )      / ____|         (_)     | | (_)            
                                                 | |__   _ __ ___  _ __ ___   ___| |_|/ ___  | (___   ___ _ __ _ _ __ | |_ _ _ __   __ _ 
                                                 |  __| | '_ ` _ \| '_ ` _ \ / _ \ __| / __|  \___ \ / __| '__| | '_ \| __| | '_ \ / _` |
                                                 | |____| | | | | | | | | | |  __/ |_  \__ \  ____) | (__| |  | | |_) | |_| | | | | (_| |
                                                 |______|_| |_| |_|_| |_| |_|\___|\__| |___/ |_____/ \___|_|  |_| .__/ \__|_|_| |_|\__, |
                                                                                                                | |                 __/ |
                                                                                                                |_|                |___/ 
												@Author: Sparke
*/

//#include <YSI\YSI\y_hooks>

static
	bool:InVan[MAX_PLAYERS],
	bool:EmmetVanLock,
//	bool:UsingCCTV,
	EnteredCar[MAX_PLAYERS],
	EmmetVan;

#define EMMET_SQLID (941) // Emmets SQL ID

#define EMMET_VAN_INTERIOR	-1513.7272, 985.0410, 1999.1707 // Van interior position
#define EMMET_VAN_SPAWN		 2501.6162,-1539.4716,23.7378 // Van spawn position

Hook:Emmet_OnPlayerConnect(playerid)
{
	print( "emmet onplayerconnect" );
	InVan[playerid] = false;
	EnteredCar[playerid] = -1;
	return 1;
}

Hook:Emmet_OnGameModeInit()
{
    EmmetVan = CreateVehicle(499, EMMET_VAN_SPAWN, 0, 00, 00, -1, 0);
    return 1;
}

Hook:Emmet_OnVehicleDeath(vehicleid, playerid)
{
    if(vehicleid == EmmetVan)
    {
        for( new i; i < GetPlayerPoolSize(); i++ )
        {
            if(InVan[i] == true)
            {
                new Float:vpos[3], Float:zangle, Float:ppos[3];

                GetVehiclePos(EmmetVan, ppos[0], ppos[1], ppos[2]);
                GetVehicleZAngle(EmmetVan, zangle);
                GetVehicleVelocity(EmmetVan, vpos[0], vpos[1], vpos[2]);

                ppos[0] -= (5 * floatsin(-zangle, degrees));
                ppos[1] -= (5 * floatcos(-zangle, degrees));

                SetPlayerPos(i, ppos[0], ppos[1], ppos[2]);
                SetPlayerFacingAngle(i, zangle);
                SetPlayerVelocity(i, vpos[0], vpos[1], vpos[2]);

                SetPlayerInterior(i, 0);
                SetPlayerVirtualWorld(i, 0);

                InVan[i] = false;
            }
        }
    }
    return 1;
}
CMD:entervan(playerid, params[])
{
    static Float:vpos[3];
    GetVehiclePos(EmmetVan, vpos[0], vpos[1], vpos[2]);

    if(!IsPlayerInRangeOfPoint(playerid, 8, vpos[0], vpos[1], vpos[2]))
        return SendClientMessage(playerid, COLOR_RED, "ERROR:{FFFFFF} You're not near the van.");

    if(EmmetVanLock == true)
        return SendClientMessage(playerid, COLOR_RED, "ERROR:{FFFFFF} The van is locked.");

	//stop the player from falling
	Timer_FreezePlayer(playerid, FREEZE, ENTER_FREEZE);
	
    SendClientMessage(playerid, COLOR_RED, "You entered the van.");
    
    SetPlayerVirtualWorld(playerid, 5);
    SetPlayerInterior(playerid, 1);
    SetPlayerPos(playerid, EMMET_VAN_INTERIOR);

    InVan[playerid] = true;
    EnteredCar[playerid] = EmmetVan;

    return 1;
}

CMD:exitvan(playerid, params[])
{
    if(!IsPlayerInRangeOfPoint(playerid, 5, EMMET_VAN_INTERIOR))
        return SendClientMessage(playerid, COLOR_RED, "You're not near the van exit.");

    static Float:vpos[3], Float:zangle, Float:ppos[3];

    GetVehiclePos(EmmetVan, ppos[0], ppos[1], ppos[2]);
    GetVehicleZAngle(EmmetVan, zangle);
    GetVehicleVelocity(EmmetVan, vpos[0], vpos[1], vpos[2]);

    ppos[0] -= (5 * floatsin(-zangle, degrees));
    ppos[1] -= (5 * floatcos(-zangle, degrees));

    SetPlayerPos(playerid, ppos[0], ppos[1], ppos[2]);
    SetPlayerFacingAngle(playerid, zangle);
    SetPlayerVelocity(playerid, vpos[0], vpos[1], vpos[2]);
    
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);

    InVan[playerid] = false;
    return 1;
}

CMD:lockvan(playerid, params[])
{
    if(PlayerInfo[playerid][pID] != EMMET_SQLID)
        return SendClientMessage(playerid, COLOR_RED, "ERROR:{FFFFFF} You're not Emmet!");

    static Float:vpos[3];

    GetVehiclePos(EmmetVan, vpos[0], vpos[1], vpos[2]);

    if(!IsPlayerInRangeOfPoint(playerid, 5, vpos[0], vpos[1], vpos[2]))
        return SendClientMessage(playerid, COLOR_RED, "ERROR:{FFFFFF} You're not near the van.");

    if(EmmetVanLock == false)
    {
        SendClientMessage(playerid, COLOR_RED, "NOTICE: {FFFFFF}You've locked the van.");

        EmmetVanLock = true;
    } else {
        SendClientMessage(playerid, COLOR_RED, "NOTICE: {FFFFFF}You've unlocked the van.");

        EmmetVanLock = false;           
    }
    return 1;
}

CMD:eastclub(playerid, params[])
{
    SetPlayerCheckpointEx(playerid, 2462.1147,-1494.4135,24.0000, 5);
    return 1;
}

CMD:emmetmusic(playerid, params[])
{
    if(PlayerInfo[playerid][pID] != EMMET_SQLID)
        return SendClientMessage(playerid, COLOR_RED, "ERROR:{FFFFFF} You're not Emmet!");

    new emmetmusicurl[164];
    if(sscanf(params, "s[164]", emmetmusicurl))
        return SendClientMessage(playerid, COLOR_LIGHTBLUE, "USAGE: /emmetmusic [URL]");

    SendClientMessage(playerid, COLOR_LIGHTBLUE, "You've chosen some music.");

    foreach(new i : Player)
    {
        if(IsPlayerInRangeOfPoint(i, 35.0, 1209.61, -1244.95, 3031.03)) // Replace with entrance coords
        {
            PlayAudioStreamForPlayer(i, emmetmusicurl, 1209.61, -1244.95, 3031.03, 35.0, 1); // Replace with entrance coords 
        }
    }
    return 1;
}