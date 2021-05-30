/*
@Hank James - Junior Developer.

THREAD LINK ( DRR ) :   http://www.gta-sarp.com/forums/showthread.php?293220-Development-Related-Juan_Ordonez-s-Request                      
*/


#define Juan_ORDONEZ_ID 82903

/*
variables.
*/
new netkey[MAX_PLAYERS], gdonatevar;

/*
functions
*/

PlayerNearPlayer(playerid, targetid)
{
     new Float:x, Float:y, Float:z;
     {
        GetPlayerPos(targetid, x, y, z);

        if(IsPlayerInRangeOfPoint(playerid, 3.0, x, y, z))
        	return true;
     }
     return false;
}

SendNetworkMessage(color, string[])
{
    for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(IsPlayerInRangeOfPoint(i, 80.0, 1452.5398,-620.8953,1901.2599) || IsPlayerInRangeOfPoint(i, 80.0, 1449.7556,-628.7351,1950.3143) || IsPlayerInRangeOfPoint(i, 80.0, 1461.2075,-630.1036,1994.6597))
		{
		   SendClientMessage(i, color, string);
		}
	}
}

SaveNETdata()
{
	new file[25];
	format(file, sizeof(file), "NetworkIG.cfg");
    if(!dini_Exists(file)) dini_Create(file);

    dini_IntSet(file, "gdonatevar", gdonatevar);
    return 1;
}

LoadNETdata()
{
    new file[25];
    format(file, sizeof(file), "NetworkIG.cfg");
    if(!dini_Exists(file)) dini_Create(file);

    gdonatevar = dini_Int(file, "gdonatevar");
    return 1;
}

/*
hooks
*/

Hook:Juan_OnGameModeInit(){
	LoadNETdata();
	CreateDynamic3DTextLabel("{8080FF}/nethelp", -1, 1452.5398,-620.8953,1901.2599, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 5.0);
	
	DonorDebug("Juan_Ordonez_82903");
	return 1;
}

Hook:Juan_OnGameModeExit(){
	SaveNETdata();
	return 1;
}

Hook:Juan_OnPlayerDisconnect(playerid, reason){	
	netkey[playerid] = 0;
	return 1;	
}
/*
includes
*/
//#include "inc/donor/Donor_scripts/Juan_Ordonez_82903/inc/_commands.inc"
#include "inc/donor/Donor_scripts/Juan_Ordonez_82903/inc/_commands.inc"

