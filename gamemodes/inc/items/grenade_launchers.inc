/*

	file:			items/grenade_launchers.inc

	description:	Custom grenade launcher script for special factions and groups.

	author:			Jay Cortez

	date created:	31st March 2018

*/

// If this script needs to be disabled, simply set the define below to true
#define GRENADE_LAUNCHERS_DISABLED	true

#if GRENADE_LAUNCHERS_DISABLED == true
	#endinput
#endif

#include <projectile>

#define GRENADE_SPEED	40.0
#define GRENADE_OBJECT	342

new bool:grenadeLoaded[MAX_PLAYERS],
	grenadeObject[MAX_PLAYERS],
	bool:grenadeLaunched[MAX_PLAYERS];

Hook:GrLaunch_OnPlayerConnect(playerid) {
	grenadeLoaded[playerid] = false;
	grenadeLaunched[playerid] = false;
	grenadeObject[playerid] = INVALID_OBJECT_ID;
}

// Command to load grenades into the grenade launcher attachment
CMD:loadgrenades(playerid, params[])
	return cmd_loadgrenade(playerid, params);

CMD:grenadelauncher(playerid, params[])
	return cmd_loadgrenade(playerid, params);

CMD:loadgrenade(playerid, params[]) {
	if(!CanUseGrenadeLauncher(playerid))
		return 1;

	if(grenadeLaunched[playerid])
		return SendClientMessage(playerid, COLOR_GREY, "You have already fired a grenade, let it explode first.");

	if(grenadeLoaded[playerid])
		return SendClientMessage(playerid, COLOR_GREY, "There is already a grenade in your grenade launcher.");

	if(!PlayerHasWeapon(playerid, WEAPON_GRENADE))
		return SendClientMessage(playerid, COLOR_GREY, "You do not have any grenades on you.");

	if(!PlayerHasWeapon(playerid, WEAPON_M4))
		return SendClientMessage(playerid, COLOR_GREY, "You do not have a M4 carbine on you.");

	ApplyAnimationEx(playerid, "COLT45", "sawnoff_reload", 4.0, 0, 0, 0, 0, 0);
	grenadeLoaded[playerid] = true;
	SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have loaded a grenade into the grenade launcher attachment.");
	return 1;
}

// Determines who is allowed to use the grenade launchers
CanUseGrenadeLauncher(playerid) {
	if(
		PlayerInfo[playerid][pGroup] == GROUP_S9 ||
		PlayerInfo[playerid][pGroup] == GROUP_HITMAN
	) return true;

	return false;
}

// Called when the player fires a shot
Hook:GrLaunch_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ) {
	if(CanUseGrenadeLauncher(playerid) && grenadeLoaded[playerid]) {
		if(weaponid == WEAPON_M4) {
			// Set variables
			grenadeLoaded[playerid] = false;
			grenadeLaunched[playerid] = true;

			// Get vector
			new Float:fPX,
				Float:fPY,
				Float:fPZ;
			GetPlayerCameraPos(playerid, fPX, fPY, fPZ);

			new Float:fVX,
				Float:fVY,
				Float:fVZ;
			GetPlayerCameraFrontVector(playerid, fVX, fVY, fVZ);

			const Float:fScale = 5.0;
			new Float:object_x = fPX + floatmul(fVX, fScale);
			new Float:object_y = fPY + floatmul(fVY, fScale);
			new Float:object_z = fPZ + floatmul(fVZ, fScale);

			// Create the projectile
			new proj = Projectile(object_x, object_y, object_z, GRENADE_SPEED * fVX, GRENADE_SPEED * fVY, (GRENADE_SPEED * fVZ) + 5.0, .air_resistance = 1.0, .sphere_radius = 0.010, .gravity = 15.0);
			
			// Create the object
			new obj = CreateDynamicObject(GRENADE_OBJECT, object_x, object_y, object_z + 0.5, 0, 0, 0);
			grenadeObject[playerid] = obj;
			Streamer_SetIntData(STREAMER_TYPE_OBJECT, obj, E_STREAMER_EXTRA_ID, proj);

			// Update streamer for all players
			for(new i, j = GetPlayerPoolSize(); i <= j; i++)
			{
				Streamer_UpdateEx(i, object_x, object_y, object_z, .type = STREAMER_TYPE_OBJECT);
			}

			displayCenterHUDInfo(playerid, "~r~~k~~SNEAK_ABOUT~ ~w~to explode grenade.", 4);
			return 0;
		}
	}
	return 1;
}

public OnProjectileUpdate(projid)
{
	new Float:projx,
		Float:projy,
		Float:projz;

	for (new i; i < MAX_PLAYERS; i++)
	{
		if(	
			!IsPlayerConnected(i)		||
			!CanUseGrenadeLauncher(i)	||
			grenadeObject[i] == INVALID_OBJECT_ID
		) continue;

	    if(Streamer_GetIntData(STREAMER_TYPE_OBJECT, grenadeObject[i], E_STREAMER_EXTRA_ID) == projid)
	    {
			GetProjectilePos(projid, projx, projy, projz);
			SetDynamicObjectPos(grenadeObject[i], projx, projy, projz);

			GetProjectileRot(projid, projx, projy, projz);
			SetDynamicObjectRot(grenadeObject[i], projx, projy, projz);
	        break;
	    }
	}
	return 1;
}

public OnProjectileCollide(projid, type, Float:x, Float:y, Float:z, extraid)
{
	for(new i; i < MAX_PLAYERS; i++)
	{
		if(	
			!IsPlayerConnected(i)		||
			!CanUseGrenadeLauncher(i)	||
			!grenadeLaunched[i]
		) continue;

		if(Streamer_GetIntData(STREAMER_TYPE_OBJECT, grenadeObject[i], E_STREAMER_EXTRA_ID) == projid)
		{
			CreateExplosion(x, y, z, 2, 10.0);
			StopProjectile(projid);
			DestroyDynamicObjectEx(grenadeObject[i]);
			grenadeLaunched[i] = false;
			break;
		}
	}
	return 1;
}

// Called when key state changes
Hook:GrLa_OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	// Explode the grenade
	if(CanUseGrenadeLauncher(playerid) && grenadeLaunched[playerid]) {
		if(PRESSED(KEY_WALK)) {
			new projid = Streamer_GetIntData(STREAMER_TYPE_OBJECT, grenadeObject[playerid], E_STREAMER_EXTRA_ID);
			new Float:x, Float:y, Float:z;
			GetDynamicObjectPos(grenadeObject[playerid], x, y, z);  

			CreateExplosion(x, y, z, 2, 10.0);
			StopProjectile(projid);
			DestroyDynamicObjectEx(grenadeObject[playerid]);
			grenadeLaunched[playerid] = false;
			OnePlayAnim(playerid, "PED", "bomber", 4.1, 0, 0, 0, 0, 0);
		}
	}
}