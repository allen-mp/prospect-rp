/*
  _________                     
 /   _____/ ____ _____    ____  
 \_____  \_/ __ \\__  \  /    \ 
 /        \  ___/ / __ \|   |  \
/_______  /\___  >____  /___|  /
        \/     \/     \/     \/ 
     ____.      .__                                
    |    | ____ |  |__   ____   __________   ____  
    |    |/  _ \|  |  \ /    \ /  ___/  _ \ /    \ 
/\__|    (  <_> )   Y  \   |  \\___ (  <_> )   |  \
\________|\____/|___|  /___|  /____  >____/|___|  /
                     \/     \/     \/           \/ 
    Changes by Golden.
*/
new SeanBenson;
new GiveBensonKey[MAX_PLAYERS], Btltimer[MAX_PLAYERS];
new BtlMatch, BtlMS;

static donorstr[128];

Hook:Sean_OnGameModeInit(){
	//================[ Sean Johnson ]=============
	SeanBenson = CreateVehicle(499, 2515.9604, -1717.5089, 13.5014, 270.0000, 1, 1, 60*30);
	SetVehicleLockState( SeanBenson, 1 );
	//New things by Troy Bradshaw.
	new Btlsign = CreateDynamicObject(18659, 2536.2629, -1722.0729, 14.0110, 0.0000, 0.0000, -90.0000); //SprayTag1
	SetDynamicObjectMaterialText(Btlsign, 0, "~Bottle Range~", 90, "Arial", 24, 1, 0xFF656A79, 0x0, 1);
	BtlMS = CreateDynamicObject(18659, 2536.2829, -1722.0729, 13.5110, 0.0000, 0.0000, -90.0000); //SprayTag1
	SetDynamicObjectMaterialText(BtlMS, 0, "Closed", 90, "Arial", 24, 1, 0xFF840410, 0x0, 1);
	BtlMatch = 0;//By default it is closed.
	return 1;
}
Hook:Sean_OnPlayerConnect(playerid){
		
	GiveBensonKey[playerid] = 0;
	Btltimer[playerid] = 0;
	return 1;
}
Hook:Sean_OnPlayerDisconnect(playerid){
	
	GiveBensonKey[playerid] = 0;
	Btltimer[playerid] = 0;
	return 1;
}
	

/* Start of SQL Legend
859 = Sean Johnson
48977 = Rajj Patel
3732 = Dominic Bradley
17542 = Carl Johnson 
53647 = Stefano_Mancini*/

//New commands by Troy Bradshaw
CMD:bensonkeys(playerid, params[]) //This command will give a playerID the keys / Access of using /lockbenson ONLY.
{
	if( PlayerInfo[playerid][pID] == 859 || PlayerInfo[playerid][pID] == 48977 || PlayerInfo[playerid][pID] == 3732 || PlayerInfo[playerid][pID] == 17542 || PlayerInfo[playerid][pID] == 53647 || PlayerInfo[playerid][pDev] > 0  )
	{
		new target,strg[128],strgRP[128]; //Variables
		if(sscanf(params,"u",target)) return SendClientMessage(playerid, COLOR_WHITE, "USAGE: /bensonkeys [ID]"); //SSCANF detection
		if( target == playerid ) return SendClientMessage(playerid, COLOR_RED, "You already have keys.");
		if(!IsPlayerInRangeOfPlayer(5.0, playerid, target)) return SendClientMessage(playerid, COLOR_GREY, "That player isn't near you.");//If target is near pID
		format(strg,sizeof(strg), "You have given %s the keys of the Benson.", PlayerICName(target));//Format of the string
		SendClientMessage(playerid, COLOR_WHITE, strg);
		GiveBensonKey[target] = 1;//It gives player the access.
		format(strgRP, sizeof(strgRP), "* %s withdraws a pair of keys and tosses them to %s.", PlayerICName(playerid), PlayerICName(target));//Forgot of RP string
	    ProxDetector(10.0, playerid, strgRP, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	}
	return 1;
}
CMD:bottlematch( playerid, params[] )//This is the command to close or open the /bottles command. 
{
	if( PlayerInfo[playerid][pID] == 859 || PlayerInfo[playerid][pID] == 48977 || PlayerInfo[playerid][pID] == 3732 || PlayerInfo[playerid][pID] == 17542 || PlayerInfo[playerid][pID] == 53647 || PlayerInfo[playerid][pDev] > 0  )
	{
		if(BtlMatch == 0)
		{
			BtlMatch = 1;//It opens the /bottles Command.
			SendClientMessage(playerid, -1, "You have enabled the bottle system.");
			SetDynamicObjectMaterialText(BtlMS, 0, "Open", 90, "Arial", 24, 1, 0xFF008000, 0x0, 1);//It updates the sign of Open.
		}
		else //If Btl is open, it will close it.
		{
		    BtlMatch = 0;//It closed the system.
		    SendClientMessage(playerid, -1, "You have disabled the bottle system.");
		    SetDynamicObjectMaterialText(BtlMS, 0, "Closed", 90, "Arial", 24, 1, 0xFF840410, 0x0, 1);//Updates the Sign to Closed.
		}

	}
	return 1;
}


new bottles[7];
new bottlesset;
CMD:bottles( playerid, params[] )
{

		if( !IsPlayerInRangeOfPoint( playerid, 10.0, 2532.32324, -1720.97070, 14.26210 ) ) {
			return SendClientMessage( playerid, COLOR_GREY, "You are not by the area to setup bottles." );
		}
		if( !bottlesset ) {
		    if(BtlMatch == 0) return SendClientMessage(playerid, -1, "ERROR: The bottle system is disabled."); //Added this detection
        	if(GetPlayerMoney(playerid) <= 999) return SendClientMessage(playerid, COLOR_GREY, "ERROR: You don't have enough cash. You need $1000.");//Added money detection
        	if(Btltimer[playerid] == 1) return SendClientMessage(playerid, COLOR_GREY, "ERROR: You must wait 1 minute until your next round!");//This is the timer thing.
			bottles[0] = CreateDynamicObject(1517, 2532.32324, -1720.97070, 14.23210,   0.00000, 0.00000, 0.00000);
			bottles[1] = CreateDynamicObject(1517, 2525.21362, -1720.91797, 14.22210,   0.00000, 0.00000, 0.00000);
			bottles[2] = CreateDynamicObject(1517, 2529.17236, -1721.28064, 13.94610,   0.00000, 0.00000, 0.00000);
			bottles[3] = CreateDynamicObject(1517, 2540.67944, -1716.20935, 14.88310,   0.00000, 0.00000, 0.00000);
			bottles[4] = CreateDynamicObject(1517, 2521.94751, -1708.27332, 13.80210,   0.00000, 0.00000, 0.00000);
			bottles[5] = CreateDynamicObject(1517, 2530.68872, -1707.68225, 13.68210,   0.00000, 0.00000, 0.00000);
			bottles[6] = CreateDynamicObject(1517, 2539.59888, -1707.15344, 13.78710,   0.00000, 0.00000, 0.00000);
			format(donorstr, sizeof donorstr, "* %s sets up a row of bottles.", PlayerICName(playerid));
   			Btltimer[playerid] = 1;//Calling the timer.
		    SetTimerEx("bottletimer", 60000, 0, "%d", playerid);//1minute timer for use /bottles again.
		    GiveMoney(playerid, -1000);
			ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			bottlesset = !bottlesset;
		}
		else {
			for( new i; i < sizeof( bottles ); i++ ) {
				DestroyDynamicObjectEx( bottles[i] );
			}
			SendClientMessage( playerid, COLOR_GREY, "You have cleared the bottles." );
			bottlesset = !bottlesset;
		}
		return 1;
}
forward bottletimer(playerid);//for Timer
public bottletimer(playerid)
{
  Btltimer[playerid] = 0;
}

CMD:lockbenson( playerid, params[] ) {
	if( PlayerInfo[playerid][pID] == 859 || PlayerInfo[playerid][pID] == 48977 || PlayerInfo[playerid][pID] == 3732 || PlayerInfo[playerid][pID] == 17542 || PlayerInfo[playerid][pID] == 53647 || PlayerInfo[playerid][pDev] > 0 || GiveBensonKey[playerid] == 1 )//Added GiveBensonKeys
	{
		new Float:vpos[3];
		GetVehiclePos( SeanBenson, vpos[0], vpos[1], vpos[2] );
		if( IsPlayerInRangeOfPoint( playerid, 5.0, vpos[0], vpos[1], vpos[2] ) || IsPlayerInRangeOfPoint( playerid, 5.0, 298.0026, 31.5620, 10000.6494 ) ) //The coords are for use /lockbenson inside the interior.
		{
			SetVehicleLockState( SeanBenson, !gCarLock[SeanBenson] );
			format(donorstr, sizeof donorstr, "* %s %s the Benson.", PlayerICName(playerid), (gCarLock[SeanBenson] == 1) ? ("locks") : ("unlocks") );
			ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			
		}
	}
	else { 
		SendClientMessage(playerid, COLOR_RED, "You don't have the keys"); 
		return 1; 
	}
	return 1;
}
CMD:bc(playerid, params[]) //For talking to people driving/inside the benson.
{
	new
 	   	text[128 - MAX_PLAYER_NAME];
	if (sscanf(params, "s[128]", text)) return 1;
	if(GetPlayerVehicleID(playerid) == SeanBenson || IsPlayerInRangeOfPoint(playerid, 15.0, 297.9415, 32.0862,10000.0674))
	{
		new
			name[MAX_PLAYER_NAME+1];
 		format( name, sizeof( name ), "%s", PlayerICName( playerid ) );
	    foreach( new i: Player )
	    {
        	if(IsPlayerInRangeOfPoint(i, 15.0, 297.9415, 32.0862,10000.0674) || GetPlayerVehicleID(i) == SeanBenson)
        	{
        		format(donorstr, sizeof(donorstr), "%s says: %s", name, text);
        		SendClientMessage(i, COLOR_GREY, donorstr);
			}
	    }
	}
	return 1;
}