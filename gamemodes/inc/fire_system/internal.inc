/*

 _    _                _           
| |  | |              | |          
| |__| | ___  __ _  __| | ___ _ __ 
|  __  |/ _ \/ _` |/ _` |/ _ \ '__|
| |  | |  __/ (_| | (_| |  __/ |   
|_|  |_|\___|\__,_|\__,_|\___|_|   

	Fire system by Fred Heinrich (oneOCT3T)

	WARNING: you should not have to edit this file.
	Instead, you should be editing the files that use
	the functions defined below.
	This file contains the internal components of the
	fire system.

	External files should only be using the above
	fire type declarations, or the public functions
	defined below.

	If there's anything missing, feel free to add it
	to the list below.

	The aim of this structure is to create a system
	such that developers may extend without
	editing this file.

	Elite City Role Play 2017

*/

#define FIRE_RADIUS_IN_VEHICLE 15.0
#define FIRE_RADIUS_ON_FOOT 4.0
#define MAX_FIRES 200
#define INVALID_FIRE_SLOT MAX_FIRES+1

#define FIRE_SMALL_HEALTH 5
#define FIRE_MED_HEALTH 15
#define FIRE_LARGE_HEALTH 30

enum {
	FIRE_CAMP,
	FIRE_SMALL,
	FIRE_MED,
	FIRE_LARGE
}

/*	Functions
*/
forward IsValidFireType(type);
forward IsValidFire(fid);
forward IsPlayerOnFire(playerid);
forward IsPlayerPuttingOutFire(playerid);
forward IsPlayerInRangeOfFire(playerid, fid, Float:range);
forward IsAFiremen(playerid); //firemen skin?

forward CreateFire(type, Float:x, Float:y, Float:z, Float:offset, vw, int);
forward DestroyFire(fid);

forward GetFireHealth(fid);
forward GetFireVirtualWorld(fid);
forward GetFireInterior(fid);
forward GetFirePos(fid, &Float:x, &Float:y, &Float:z);
forward GetFireOwner(fid);
forward GetFireType(fid);
forward GetFirePoolSize();

forward SetFireHealth(fid, health);
forward SetFireVirtualWorld(fid, vw);
forward SetFireOwner(fid, playerid);

forward SetPlayerOnFire(playerid);
forward RemovePlayerFire(playerid);
forward GetPlayerClosestFire(playerid);



/*	Global variables
*/


enum FireInfo {
	fireObject,
	fireWoodObject,
	fireHealth,
	fireType,
	fireVW,
	fireInt,
	fireOwner
}

/*		fire internal core related vars */
static Fires[MAX_FIRES][FireInfo];

/* 		player putting out fires vars */

//max fires that may be put out at once by a single player
#define MAX_PUTOUT 5
static PlayerPuttingOutFire[MAX_PLAYERS][MAX_PUTOUT+1];
static FireTimer[MAX_PLAYERS][MAX_PUTOUT+1];
//how many fires player is putting out
static puttingoutSlots[MAX_PLAYERS];
//aim assist
new Text:aim_hud;
new Text:aim_hud_green;

/*		player on fire vars */
//remaining time on fire
new pFireTime[MAX_PLAYERS];
new pFireObject[MAX_PLAYERS] = {INVALID_OBJECT_ID, ...};





/*

 ______                _   _                 
|  ____|              | | (_)                
| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
| |  | |_| | | | | (__| |_| | (_) | | | \__ \
|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
                                             

*/

//Initalize some arrays, aim assist
Hook:Fire_OnGameModeInit() {
	for(new i; i < MAX_PLAYERS; i++) {
		for(new j; j < MAX_PUTOUT; j++) {
			PlayerPuttingOutFire[i][j] = INVALID_FIRE_SLOT;
		}
	}

	//Putting out HUD
	aim_hud = TextDrawCreate(314.000000, 207.000000, "X");
	TextDrawBackgroundColor(aim_hud, 255);
	TextDrawFont(aim_hud, 1);
	TextDrawLetterSize(aim_hud, 0.500000, 1.399999);
	TextDrawColor(aim_hud, 0xFF000044);
	TextDrawSetOutline(aim_hud, 0);
	TextDrawSetProportional(aim_hud, 1);
	TextDrawSetShadow(aim_hud, 0);
	TextDrawSetSelectable(aim_hud, 0);

	aim_hud_green = TextDrawCreate(314.000000, 207.000000, "X");
	TextDrawBackgroundColor(aim_hud_green, 255);
	TextDrawFont(aim_hud_green, 1);
	TextDrawLetterSize(aim_hud_green, 0.500000, 1.399999);
	TextDrawColor(aim_hud_green, 0x00FF0044);
	TextDrawSetOutline(aim_hud_green, 0);
	TextDrawSetProportional(aim_hud_green, 1);
	TextDrawSetShadow(aim_hud_green, 0);
	TextDrawSetSelectable(aim_hud_green, 0);
	return 1;
}

/*
	GETTERS:
	--------
	only store getters here.
	Getters are functions that
	allows a developer to access
	internal variables.
*/

public GetFireOwner(fid) {
	if(!IsValidFire(fid))
		return INVALID_PLAYER_ID;
	return Fires[fid][fireOwner];
}

public GetFireType(fid) {
	return Fires[fid][fireType];
}

public GetFireHealth(fid) {
	return Fires[fid][fireHealth];
}

public GetFireVirtualWorld(fid) {
	return Fires[fid][fireVW];
}

public GetFireInterior(fid) {
	return Fires[fid][fireInt];
}

public GetFirePos(fid, &Float:x, &Float:y, &Float:z) {
	GetDynamicObjectPos(Fires[fid][fireObject], x, y, z);
	return;
}

public GetFirePoolSize() {
	return MAX_FIRES;
}

/*
	SETTERS:
	--------
	Setters allows a developer
	to set internal variables.

*/
public SetFireOwner(fid, playerid) {
	Fires[fid][fireOwner] = playerid;
}

public SetFireHealth(fid, health) {
	if(IsValidFire(fid)) {
		if( health <= 0 ) {
			DestroyFire(fid);
		}
		else
			Fires[fid][fireHealth] = health;
		
		return 1;
	}
	return 0;
}


/*
	Checking Related Functions
	--------------------------
	Only store checking related functions here,
	that is to be accessiable outside of this file.
*/

public IsValidFireType(type) {
	if(type == FIRE_SMALL || type == FIRE_MED || type == FIRE_LARGE) return true;
	return false;
}


public IsValidFire(fid) {
	if(fid == INVALID_FIRE_SLOT) return false;
	if(!Fires[fid][fireObject]) return false;
	return true;
}

public IsPlayerPuttingOutFire(playerid) {
	return puttingoutSlots[playerid];
}

public IsPlayerOnFire(playerid) {
	return IsValidObject(pFireObject[playerid]) && pFireTime[playerid];
}

public IsPlayerInRangeOfFire(playerid, fid, Float:range) {
	new vw = GetPlayerVirtualWorld(playerid);
	new int = GetPlayerInterior(playerid);
	new Float: fx, Float:fy, Float:fz;
	GetFirePos(fid, fx, fy, fz);

	if( vw == GetFireVirtualWorld(fid) ) {
		if( int == GetFireInterior(fid) ) {
			if( IsPlayerInRangeOfPoint(playerid, range, fx, fy, fz ) ) {
				return true;
			}
		}
	}
	return false;
}

/*
	Creating/Destroying fires
	-------------------------
	Only related functions please.

*/
public CreateFire(type, Float:x, Float:y, Float:z, Float:offset, vw, int) {
	new i = GetFreeFireSlot();

	if( i != INVALID_FIRE_SLOT ) {
		switch(type) {
			case FIRE_CAMP: {
				Fires[i][fireObject] = CreateDynamicObject( 18688, x, y, (z + offset), 0.0, 0.0, 0.0, vw, int );
				Fires[i][fireWoodObject] = CreateDynamicObject(841, x, y, (z + offset + 1.5), 0.0, 0.0, 0.0);
				Fires[i][fireHealth] = FIRE_SMALL_HEALTH;
				Fires[i][fireType] = FIRE_SMALL;
			}
			case FIRE_SMALL: {
				Fires[i][fireObject] = CreateDynamicObject( 18688, x, y, (z + offset), 0.0, 0.0, 0.0, vw, int );
				Fires[i][fireHealth] = FIRE_SMALL_HEALTH;
				Fires[i][fireType] = FIRE_SMALL;
			}
			case FIRE_MED: {
				Fires[i][fireObject] = CreateDynamicObject( 18692, x, y, (z + offset), 0.0, 0.0, 0.0, vw, int );
				Fires[i][fireHealth] = FIRE_MED_HEALTH;
				Fires[i][fireType] = FIRE_MED;
			}
			case FIRE_LARGE: {
				Fires[i][fireObject] = CreateDynamicObject( 18691, x, y, (z + offset), 0.0, 0.0, 0.0, vw, int );
				Fires[i][fireHealth] = FIRE_LARGE_HEALTH;
				Fires[i][fireType] = FIRE_LARGE;
			}
		}
		//Invalid fire type provided.
		if(!IsValidFire(i)) return INVALID_FIRE_SLOT;

		Fires[i][fireVW] = vw;
		Fires[i][fireInt] = int;
		Fires[i][fireOwner] = INVALID_PLAYER_ID;

		return i;
	}
	printf("FIRE WARNING: max fires has been reached !");
	return INVALID_FIRE_SLOT;
}


public DestroyFire(fid) {

	if( !( IsValidFire(fid) ) ) {
		printf("FIRE WARNING: destroying invalid fire ID.");
		return 0;
	}
	
	if( Fires[fid][fireObject] ) DestroyDynamicObjectEx( Fires[fid][fireObject] );
	if( Fires[fid][fireWoodObject] ) DestroyDynamicObjectEx( Fires[fid][fireWoodObject] );

	for( new i; FireInfo:i < FireInfo; i++ ) {
		Fires[fid][FireInfo:i] = 0;
	}

	return 1;
}



/*
	DANGER:
	-------
	You are entering the deep internal area.
	Only adjust things here, if you know
	what you're doing!

*/
static Float:Get3DDistance( Float:x, Float:y, Float:z, Float:x2, Float:y2, Float:z2 ) {
	return floatsqroot( floatpower(x - x2, 2) + floatpower(y - y2, 2) + floatpower(z - z2, 2) );
}
public GetPlayerClosestFire( playerid ) {
	new 
		Float:x, Float:y, Float:z, Float:dist = 99999.0,
		Float:px, Float:py, Float:pz, fid;
	
	GetPlayerPos( playerid, px, py, pz );
	
	for( new i; i < GetFirePoolSize(); i++ ) {
		if( i >= MAX_FIRES - 1 ) break;
		
		if( IsValidFire(i) ) {
		
			GetFirePos(i, x, y, z);
			
			new Float: cmpDist = Get3DDistance( x, y, z, px, py, pz );
			if( cmpDist < dist || i == INVALID_FIRE_SLOT ) {
				dist = cmpDist;
				fid = i;
			}
		}
	}
	return fid;
}

GetFreeFireSlot() {
	for(new id; id < MAX_FIRES; id++)
	{
	    if(!IsValidFire(id)) {
	        return id;
		}
	}
	return INVALID_FIRE_SLOT;
}

/*	
		If the player is burning (on fire)...
		
*/

//is player inside a fire or is on fire?
OnFirePlayerSecondSync( playerid ) {

	new
	    Float:fx, Float:fy, Float:fz, Float:radius;
		
	if(IsAFiremen(playerid)) {
		if(IsPlayerOnFire(playerid)) RemovePlayerFire(playerid);
		return 1;
	}

	//bikes or onfoot may catch fire
	new biking = IsABike(GetPlayerVehicleID(playerid));
	if(IsPlayerInAnyVehicle(playerid) && !biking) {
		if(IsPlayerOnFire(playerid)) RemovePlayerFire(playerid);
		return 1;
	}

    if(IsPlayerOnFire(playerid)) {
		OnPlayerBurn( playerid );
		return 1;
	}
		
	//set the player on fire
	for( new i; GetFirePoolSize() > i; i++ ) {
		if( IsValidFire(i) ) {
		
			//radius player must be within
			new type = GetFireType(i);
		    if( type == FIRE_SMALL ) continue; //cancel camp fires from lighting players on fire
		    if( type == FIRE_MED ) radius = 1.5;
			if( type == FIRE_LARGE ) radius = 2.5;
			
			GetFirePos(i, fx, fy, fz);
			if( GetFireVirtualWorld(i) != GetPlayerVirtualWorld( playerid ) ) continue;
			if( GetFireInterior(i) != GetPlayerInterior(playerid) ) continue;
			
			if( IsPlayerInRangeOfPoint( playerid, radius, fx, fy, fz + 1 ) ) {
			    SetPlayerOnFire(playerid); //set the player on fire
				return 1;
			}
		}
	}
	return 1;
}

/*

	Setting players on fire...
	------------------------
	Again, Internal.

*/

public RemovePlayerFire(playerid) {

	DestroyObject( pFireObject[playerid] );
	HideBloodForPlayer(playerid);
	pFireObject[playerid] = INVALID_OBJECT_ID;
	pFireTime[playerid] = 0;
	return 1;
}

//set the player on fire
public SetPlayerOnFire(playerid) {

	if(PlayerInfo[playerid][pAdmin] > 1 && PlayerInfo[playerid][pStealthed] != 1) return 1;
	if(IsPlayerOnFire(playerid)) return 1;
	
	pFireObject[playerid] = CreateObject(18688, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	AttachObjectToPlayer( pFireObject[playerid], playerid, 0.0, 0.0, -1.5, 0.0, 0.0, 0.0 );
	pFireTime[playerid] = 10;
	ShowBloodForPlayer(playerid, 4, 2.0);


	SetPlayerArmedWeapon( playerid, 0 );
	return 1;
}

//fire damage effect on players
//Only called if the player is on fire
static OnPlayerBurn( playerid ) {
	if( gPlayerLogged[playerid] != 1 ) return 1;

	//is swimming?
	if( GetPlayerAnimationIndex( playerid ) ) {

		new animlib[40], animname[40];
		GetAnimationName( GetPlayerAnimationIndex(playerid), animlib, 40, animname, 40 );
		if( strcmp( animlib, "SWIM", true ) == 0) {
			pFireTime[playerid] = 0;
			RemovePlayerFire(playerid);
			HideBloodForPlayer(playerid);
		}
	}
	
	//effects
	pFireTime[playerid] -= 1;

	//new Float:health;
	//GetPlayerHealth(playerid, health);
	//SetPlayerHealth(playerid, health - 2 );

	if(pFireTime[playerid] <= 0) {
		RemovePlayerFire(playerid);
	}
	return 1;
}

//Illegal actions while burning...

Hook:Burn_OnPlayerChangeWeapon( playerid, oldweapon, newweapon ) {
	if(IsPlayerOnFire(playerid))
	    SetPlayerArmedWeapon( playerid, 0 );
	return 1;
}


Hook:Burn_OnPlayerEnterVehicle( playerid, vehicleid, ispassenger ) {
	if(IsPlayerOnFire(playerid))
		ClearAnimations( playerid );
	return 1;
}

//Putting the player out?
Hook:Burn_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart) {
	if( !IsPlayerOnFire(playerid) ) return 1;
	if( weaponid == 42 ) { //fire ext

	    RemovePlayerFire(playerid);
		return 0; //return 0 to stop Fire Est issuing damage
	}
	return 1;
}

Hook:Burn_OnPlayerDisconnect(playerid, reason) {
	if(IsPlayerOnFire(playerid)) {
		RemovePlayerFire(playerid);
	}
	return 1;
}

Hook:Burn_OnPlayerDeath(playerid, killerid, reason) {
	if(IsPlayerOnFire(playerid)) {
		RemovePlayerFire(playerid);
	}
	return 1;
}
/*

	Ways of putting out fire
	--------------------------
	Again, Internal.
*/

// Using fire truck/estingisher to put it out



//Checks for an empty slot in the queue then sets a timer function for the helper function, OnPlayerPutOutFireEx
//	puttingOutSlots is already checked in its parent callee.
static OnPlayerPutOutFire(playerid, fid, Float:offset, time, Float:fx, Float:fy, Float:fz) {
	//It's important the queue is maintained at all times.

	if(!IsValidFire(fid)) return 1;

	for(new i; i < MAX_PUTOUT; i++) {
		if(PlayerPuttingOutFire[playerid][i] == fid) {
			return 0;
		}
	}

	for(new i; i < MAX_PUTOUT; i++) {
		if(!FireTimer[playerid][i]) {

			FireTimer[playerid][i] = SetTimerEx("OnPlayerPutOutFireEx", time, true, "ifffii", playerid, fx, fy, fz+offset, fid, i);
			PlayerPuttingOutFire[playerid][i] = fid;
			puttingoutSlots[playerid]++;
			break;
		}
	}
	return 1;
}

//Helper function - INTERNAL, highly recommended not touching this unless you know mathematics.
forward OnPlayerPutOutFireEx(playerid, Float:fx, Float:fy, Float:fz, fid, queueid);
public OnPlayerPutOutFireEx(playerid, Float:fx, Float:fy, Float:fz, fid, queueid) {
	new
		Float:camx, Float:camy, Float:camz,
  		Float:camvx, Float:camvy, Float:camvz,
		Float:fcx, Float:fcy, Float:fcz,
		Float:fire_distance,
		keys, updown, leftright;
		
    GetPlayerKeys(playerid, keys, updown, leftright);
    if(keys & KEY_FIRE) {
	
		GetPlayerCameraFrontVector(playerid, camvx, camvy, camvz); //third dimensional camera look at vectors
		GetPlayerCameraPos(playerid, camx, camy, camz); //existing camera position to use with vectors
		
		fire_distance = Get3DDistance( camx, camy, camz, fx, fy, fz );
		
		fcx = camvx * fire_distance + camx;
		fcy = camvy * fire_distance + camy;
		fcz = camvz * fire_distance + camz;
		
		new Float: camDistToFire = Get3DDistance( fcx, fcy, fcz, fx, fy, fz );
		if( camDistToFire < 1.3) {
			

			new firehealth = GetFireHealth(fid) -5;
			TextDrawShowForPlayer(playerid, aim_hud_green);
			SetFireHealth(fid, firehealth);

			if(firehealth <= 0) {
				
				KillTimer( FireTimer[playerid][queueid] );
				FireTimer[playerid][queueid] = 0;
				PlayerPuttingOutFire[playerid][queueid] = INVALID_FIRE_SLOT;
				puttingoutSlots[playerid]--;
				//no longer aiming at destroyed fire
				if(!puttingoutSlots[playerid]) {
					TextDrawHideForPlayer(playerid, aim_hud_green);
				}
			}
			return 1;
		}
	}
	else {
		KillTimer( FireTimer[playerid][queueid] );
		FireTimer[playerid][queueid] = 0;
		PlayerPuttingOutFire[playerid][queueid] = INVALID_FIRE_SLOT;
		puttingoutSlots[playerid]--;
		//no longer firing
		TextDrawHideForPlayer(playerid, aim_hud);
	}

	//player didn't manage to reduce fire health
	TextDrawHideForPlayer(playerid, aim_hud_green);

	return 1;
}

stock IsAFiremen(playerid) {
	new skin = GetPlayerSkin(playerid);
	if( skin == 277 || skin == 278 || skin == 279) return 1;
	return 0;
}

//The trigger for putting out fire
Hook:Fire_OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	if(HOLDING(KEY_FIRE)) {


		if(puttingoutSlots[playerid] == MAX_PUTOUT) return 1;

		new Float:fire_radius;
		new Float:offset = 2.0;
		new time = 2000;

		if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) {
			new weapon = GetPlayerWeapon(playerid);
			
			if(weapon != 42) return 1;

			fire_radius = FIRE_RADIUS_ON_FOOT;


		}
		else if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {

			if(GetVehicleModel(GetPlayerVehicleID(playerid)) != 407) return 1;

			fire_radius = FIRE_RADIUS_IN_VEHICLE;
			offset = 2.5; //vector fix
			time = 1000; //puts out fire quicker
		}
		else return 1;

		if(!puttingoutSlots[playerid]) {
			TextDrawShowForPlayer(playerid, aim_hud);
		}


		new Float:fx, Float:fy, Float:fz;
		for(new i; i < GetFirePoolSize(); i++) {

			//if(GetFireType(i) == FIRE_SMALL) continue;
			if(GetFireVirtualWorld(i) != GetPlayerVirtualWorld(playerid)) continue;
			if(GetFireInterior(i) != GetPlayerInterior(playerid)) continue;

			GetFirePos(i, fx, fy, fz);
			if(IsPlayerInRangeOfPoint(playerid, fire_radius, fx, fy, fz) ) {
				OnPlayerPutOutFire(playerid, i, offset, time, fx, fy, fz);
			}
		}
	}
	else {
		TextDrawHideForPlayer(playerid, aim_hud);
	}
	return 1;
}


#include "inc\fire_system\commands\putout.inc"
#include "inc\fire_system\commands\create.inc"
#include "inc\fire_system\camp.inc"
