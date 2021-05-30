
//Will adjust this later - Fred
//ATM functionality is dizziness with decreased HP as textdraw sprite: blood, didn't work with transparency

//functions
forward ShowBloodForPlayer(playerid, time, Float:loss);
forward HideBloodForPlayer(playerid);
forward IsPlayerBleeding(playerid);

//callbacks
forward OnPlayerBleeding(playerid, Float:loss);

static timer[MAX_PLAYERS];


//Check the status of the player
public IsPlayerBleeding(playerid) {
    if( timer[playerid] ) return true;
    else return false;
}

new TimeRemaining[MAX_PLAYERS];
//Enable the hurt effects for player, playerid
public ShowBloodForPlayer(playerid, time, Float:loss) {

    TimeRemaining[playerid] = time*2;
    new drunktime = time*2*50;
    SetPlayerDrunkLevel(playerid, 2000 + drunktime );
    timer[playerid] = SetTimerEx("OnPlayerBleeding", 500, true, "if", playerid, loss);
    SetPlayerTime(playerid, 22, 0);
    SetPlayerWeather(playerid, 202);
    
    return 1;
}

//What to do when the player is under hurting effects
public OnPlayerBleeding(playerid, Float:loss) {

    if( TimeRemaining[playerid] <= 0 || !IsPlayerConnected(playerid) ) {
        HideBloodForPlayer(playerid);
        RemovePlayerFire(playerid);
        
        KillTimer(timer[playerid]);
    }

    new Float:health;
    GetPlayerHealth(playerid, health);

    SetPlayerHealth(playerid, health - loss );
    TimeRemaining[playerid]--;

    
    return 1;
}

//Stop the hurting effects
public HideBloodForPlayer(playerid) {
    SetPlayerWeather(playerid, g_WeatherID);
	new hour,minute,second;
	new day,month,year;
	gettime(hour,minute,second);
	getdate(year,month,day);
	hour = FixHour(hour);
	SetPlayerTime(playerid, hour, minute);
    KillTimer(timer[playerid]);
    return 1;
}
