/*
  _____         __  __ _      _                   ____          _____                                      _ 
 |  __ \       / _|/ _| |    | |                 |  _ \        |  __ \                                    | |
 | |  | |_   _| |_| |_| | ___| |__   __ _  __ _  | |_) |_   _  | |  | | ___  ___ _ __ ___   ___  _ __   __| |
 | |  | | | | |  _|  _| |/ _ \ '_ \ / _` |/ _` | |  _ <| | | | | |  | |/ _ \/ __| '_ ` _ \ / _ \| '_ \ / _` |
 | |__| | |_| | | | | | |  __/ |_) | (_| | (_| | | |_) | |_| | | |__| |  __/\__ \ | | | | | (_) | | | | (_| |
 |_____/ \__,_|_| |_| |_|\___|_.__/ \__,_|\__, | |____/ \__, | |_____/ \___||___/_| |_| |_|\___/|_| |_|\__,_|
                                           __/ |         __/ |                                               
                                          |___/         |___/                                                

Replaces briefcase, not a "hidden" storage place.
Can not be unequipped - only dropped. Does not save if the user logged out.
It will instead drop on the ground with the items (restart wipes them all completely).

The system will track every dufflebag ID and have variables to determine whenever it is:
Currently equipped by a player
Dropped on the ground

Instead of assigning it to players.
*/

#define MAX_DUFFLEBAGS 200

#define MAX_DUFFLEBAG_WEAPONS		7
#define MAX_DUFFLEBAG_MATERIALS		25000	// 25k
#define MAX_DUFFLEBAG_MONEY			1000000 // 1 million	
#define MAX_DUFFLEBAG_DRUGS			750		// note that this is not for all of the drugs in the dufflebag - it's per each one
#define DUFFLEBAG_DISAPPEAR_TIME	600		// in seconds, 600 seconds = 10 minutes
#define DUFFLEBAG_COOLDOWN			1200	// in seconds, 1200 seconds = 20 minutes, cooldown of /buy duffle bag (saves)

enum dufflebaginfo
{
	bool:dfCreated, // is the dufflebag ID in use?
	bool:dfDropped, // is the dufflebag currently on the ground?
	bool:dfHold, 	// is the dufflebag currently being held?
	Float:dfPos[3], // position X, Y and Z if on the ground
	Text3D:dfText,	// 3d label of the duffle
	dfPlayer,		// playerid of the duffle (it's possible that there is not)
	dfObject,		// objectid of the duffle
	dfTimer,		// timer to auto remove the dufflebag if it's removed

	/* Dufflebag Items */
	dfWeapon[MAX_DUFFLEBAG_WEAPONS],	// 7 slots for weapons
	dfMaterials, 	// materials storage
	dfMoney,		// money storage

	/* Drugs */
	dfCocaine,
	dfMeth,
	dfCannabis,
	dfXanax,
	dfPromethazine,
	dfCodeine,
	dfLean,
};

new DuffleBag[MAX_DUFFLEBAGS][dufflebaginfo];

/* Player Variables */

new bool:holdingDufflebag[MAX_PLAYERS];		// is a player holding a dufflebag
new holdingDufflebagID[MAX_PLAYERS]; 		// the ID of the dufflebag that a player is holding

CreateDuffleBag()
{
	new duffleid = -1;
	for(new i = 0; i < sizeof(DuffleBag); i++)
	{
		if(DuffleBag[i][dfCreated]) continue;

		DuffleBag[i][dfCreated] = true;
		duffleid = i;
		break;
	}
	return duffleid;
}

DeleteDuffleBag(duffleid)
{
	/* We're resetting ALL of the dufflebag's variables since the ID might be
		used again and therefore we wouldn't want it to keep previous data.	*/

	DestroyDynamicObject(DuffleBag[duffleid][dfObject]);

	DuffleBag[duffleid][dfCreated] = false;
	DuffleBag[duffleid][dfDropped] = false;
	DuffleBag[duffleid][dfHold] = false;
	DuffleBag[duffleid][dfPos][0] = 0;
	DuffleBag[duffleid][dfPos][1] = 0;
	DuffleBag[duffleid][dfPos][2] = 0;
	DuffleBag[duffleid][dfPlayer] = -1;
	DuffleBag[duffleid][dfObject] = INVALID_OBJECT_ID;
	KillTimer(DuffleBag[duffleid][dfTimer]);
	DestroyDynamic3DTextLabel(DuffleBag[duffleid][dfText]);
	for(new i = 0; i < MAX_DUFFLEBAG_WEAPONS; i++)
	{
		DuffleBag[duffleid][dfWeapon][i] = 0; 
	}
	DuffleBag[duffleid][dfMaterials] = 0;
	DuffleBag[duffleid][dfMoney] = 0;
	DuffleBag[duffleid][dfCocaine] = 0;
	DuffleBag[duffleid][dfMeth] = 0;
	DuffleBag[duffleid][dfCannabis] = 0;
	DuffleBag[duffleid][dfXanax] = 0;
	DuffleBag[duffleid][dfPromethazine] = 0;
	DuffleBag[duffleid][dfCodeine] = 0;
	DuffleBag[duffleid][dfLean] = 0;

	return 1;
}

AttachDufflebagToPlayer(playerid, duffleid)
{
	holdingDufflebag[playerid] = true;
	holdingDufflebagID[playerid] = duffleid;
	SetPlayerAttachedObject(playerid, 9, 11745, 5, 0.175999, -0.038000, 0.020999, 0, -96.099967, 98.399986, 1.0, 1.0, 1.0);

	DuffleBag[duffleid][dfHold] = true;
	DuffleBag[duffleid][dfPlayer] = playerid;
	DuffleBag[duffleid][dfDropped] = false;
	if(IsValidDynamic3DTextLabel(DuffleBag[duffleid][dfText])) DestroyDynamic3DTextLabel(DuffleBag[duffleid][dfText]);

	return 1;
}

RemoveDufflebagFromPlayer(playerid, duffleid)
{
	holdingDufflebag[playerid] = false;
	holdingDufflebagID[playerid] = -1;
	RemovePlayerAttachedObject(playerid, 9);

	DuffleBag[duffleid][dfHold] = false;
	DuffleBag[duffleid][dfPlayer] = -1;
	return 1;
}

DropDufflebag(playerid, duffleid)
{
	if(pDeathState[playerid] == PLAYER_STATE_REVIVE || pDeathState[playerid] == PLAYER_STATE_INHOSP)
	{
		DuffleBag[duffleid][dfPos][0] = DeathPos[playerid][DeathX];
		DuffleBag[duffleid][dfPos][1] = DeathPos[playerid][DeathY];
		DuffleBag[duffleid][dfPos][2] = DeathPos[playerid][DeathZ]-1;
	}
	else
	{
		new Float: newpos[3];
		GetPlayerPos(playerid, newpos[0], newpos[1], newpos[2]);
		DuffleBag[duffleid][dfPos][0] = newpos[0];
		DuffleBag[duffleid][dfPos][1] = newpos[1];
		DuffleBag[duffleid][dfPos][2] = newpos[2]-0.78; // -0.78 to make sure it's on the ground
	}

	RemoveDufflebagFromPlayer(playerid, duffleid);

	DuffleBag[duffleid][dfDropped] = true;

	DuffleBag[duffleid][dfObject] = CreateDynamicObject(11745, DuffleBag[duffleid][dfPos][0], DuffleBag[duffleid][dfPos][1], DuffleBag[duffleid][dfPos][2], 0, 0, 5.22000, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), -1, 100.0);
	DuffleBag[duffleid][dfTimer] = SetTimerEx("DufflebagTimer", DUFFLEBAG_DISAPPEAR_TIME * 1000, false, "i", duffleid);	// 1 seconds (1000 miliseconds) multiplied by the seconds defined to remove a dufflebag.
	DuffleBag[duffleid][dfText] = CreateDynamic3DTextLabel("{FF5E5A}Duffle Bag\n{FFFFFF}/dufflebag pickup", -1, DuffleBag[duffleid][dfPos][0], DuffleBag[duffleid][dfPos][1], DuffleBag[duffleid][dfPos][2]+0.3, 5);

	return 1;
}

IsPlayerNearDuffle(playerid)
{
	new duffleid = -1;
	for(new i = 0; i < sizeof(DuffleBag); i++)
	{
		// dfDropped is checked as well, just in case, to make sure it's on the ground.
		if(IsPlayerInRangeOfPoint(playerid, 3, DuffleBag[i][dfPos][0], DuffleBag[i][dfPos][1], DuffleBag[i][dfPos][2]) && DuffleBag[i][dfDropped])
		{
			duffleid = i;
			break;
		}
	}
	return duffleid;
}

ShowPlayerDuffleContents(playerid, duffleid)
{
	SendClientMessage(playerid, COLOR_WHITE, "Duffle bag Contents:");

	new msg[128], weapname[40];
	for(new i = 0; i < MAX_DUFFLEBAG_WEAPONS; i++)
	{
		if(DuffleBag[duffleid][dfWeapon][i] == 0) // if the weapon id in that slot is 0, which means it's not used
		{
			format(msg, sizeof(msg), "Slot %d: None", i); // display the slot ID of that weapon slot
			SendClientMessage(playerid, COLOR_BITEM, msg);
		}
		else // if there's a weapon, display its slot and name
		{
			GetWeaponName(DuffleBag[duffleid][dfWeapon][i], weapname, sizeof(weapname)); // get that weapon's name
			format(msg, sizeof(msg), "Slot %d: %s", i, weapname); // display the slot ID of that weapon and tell the player what weapon it is
			SendClientMessage(playerid, COLOR_BITEM, msg);
		}
	}

	format(msg, sizeof(msg), "Materials: %d", DuffleBag[duffleid][dfMaterials]);
	SendClientMessage(playerid, COLOR_BITEM, msg);
	format(msg, sizeof(msg), "Money: $%d", DuffleBag[duffleid][dfMoney]);
	SendClientMessage(playerid, COLOR_BITEM, msg);
	format(msg, sizeof(msg), "Cocaine: %d.0g", DuffleBag[duffleid][dfCocaine]);
	SendClientMessage(playerid, COLOR_BITEM, msg);
	format(msg, sizeof(msg), "Meth: %d.0g", DuffleBag[duffleid][dfMeth]);
	SendClientMessage(playerid, COLOR_BITEM, msg);
	format(msg, sizeof(msg), "Cannabis: %d.0g", DuffleBag[duffleid][dfCannabis]);
	SendClientMessage(playerid, COLOR_BITEM, msg);
	format(msg, sizeof(msg), "Xanax: %d.0g", DuffleBag[duffleid][dfXanax]);
	SendClientMessage(playerid, COLOR_BITEM, msg);
	format(msg, sizeof(msg), "Promethazine: %d.0 fl oz", DuffleBag[duffleid][dfPromethazine]);
	SendClientMessage(playerid, COLOR_BITEM, msg);
	format(msg, sizeof(msg), "Codeine: %d.0 fl oz", DuffleBag[duffleid][dfCodeine]);
	SendClientMessage(playerid, COLOR_BITEM, msg);
	format(msg, sizeof(msg), "Lean: %d.0 fl oz", DuffleBag[duffleid][dfLean]);
	SendClientMessage(playerid, COLOR_BITEM, msg);
	return 1;
}

IsPlayerHoldingDuffleBag(playerid) // is the player holding a duffle bag?
{
	if(holdingDufflebag[playerid] != false) return 1;
	return 0;
}

HoldingDufflebagID(playerid) // the ID of the duffle bag that the player is holding
{
	return holdingDufflebagID[playerid];
}

IsAllowedDuffleFaction(playerid) // this makes sure the player is in PD, FBI or SANG, in order to forcefully take a duffle bag from another player (must be restrained)
{
	if(IsACop(playerid) || IsASANG(playerid)) return 1;
	return 0;
}

forward DufflebagTimer(duffleid);

public DufflebagTimer(duffleid)
{
	DeleteDuffleBag(duffleid);
	return 1;
}

forward IsDisallowedWeapon(playerid);

// make sure to check if they are holding a weapon first
public IsDisallowedWeapon(playerid)
{
	new weaponid = GetPlayerWeapon(playerid);
	switch(weaponid)
	{
		case WEAPON_CHAINSAW: 			return 1; // chainsaw
		case WEAPON_GRENADE: 			return 1; //grenade
		case WEAPON_TEARGAS: 			return 1; // tear gas
		case WEAPON_MOLTOV: 			return 1; // molotov
		case WEAPON_FLAMETHROWER: 		return 1; // flamethrower
		case WEAPON_SATCHEL: 			return 1; // satchel charge (C4)
		case WEAPON_BOMB:				return 1; // detonator of C4
		case WEAPON_SPRAYCAN: 			return 1; // spraycan
		case WEAPON_FIREEXTINGUISHER: 	return 1; // fire extinguisher
	}
	return 0;
}

Hook:df_OnPlayerConnect(playerid)
{
	holdingDufflebag[playerid] = false;
	holdingDufflebagID[playerid] = -1;

	RemovePlayerAttachedObject(playerid, 9);
	return 1;
}

Hook:df_OnPlayerDisconnect(playerid, reason)
{
	if(IsPlayerHoldingDuffleBag(playerid)) DropDufflebag(playerid, HoldingDufflebagID(playerid));
	RemovePlayerAttachedObject(playerid, 9);
	return 1;
}

Hook:df_OnPlayerDeath(playerid)
{
	if(IsPlayerHoldingDuffleBag(playerid))
	{
		if(pDeathState[playerid] == PLAYER_STATE_NORMAL || PLAYER_STATE_REVIVE)
		{
			DropDufflebag(playerid, HoldingDufflebagID(playerid));
		}
	}	
	return 1;
}

Hook:df_OnGameModeExit()
{
	foreach(new i: Player)
	{
		RemovePlayerAttachedObject(i, 9);
	}
	return 1;
}

CMD:dufflebag(playerid, params[])
{
	if(pDeathState[playerid] != PLAYER_STATE_NORMAL) return SendClientMessage(playerid, COLOR_GRAD1, "You can't use your duffle bag at this time.");
	if(PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can get a duffle bag.");
	if(PlayerCuffed[playerid] > 0) return SendClientMessage(playerid, COLOR_GRAD1, "You can't use your duffle bag when cuffed.");
	if(inPaintball[playerid] > 0) return SendClientMessage(playerid, COLOR_GRAD1, "You cannot use your duffle bag at this time.");
	if(PlayerInfo[playerid][pJailed] == 1 || PlayerInfo[playerid][pJailed] == 2 || PlayerInfo[playerid][pAdminJailed] == 1) return SendClientMessage(playerid, COLOR_GREY, "You can't use this while you're in prison.");

	new action[2][32], slot, amount;

	if(sscanf(params, "s[32] ", action[0]))
	{
		SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag [name]");
		if(IsAllowedDuffleFaction(playerid)) SendClientMessage(playerid, COLOR_GRAD1, "Available names: pickup, check, deposit, withdraw, show, drop, give, take");
		else SendClientMessage(playerid, COLOR_GRAD1, "Available names: pickup, check, deposit, withdraw, show, drop, give");
		return 1;
	}
	
	if(!strcmp(action[0], "pickup", true))
	{
		if(IsPlayerHoldingDuffleBag(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You already have a duffle bag in hand.");
		if(pDeathState[playerid] == PLAYER_STATE_REVIVE || IsPlayerRestricted(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can not do this right now.");
		new duffleid = IsPlayerNearDuffle(playerid);
		if(duffleid == -1) return SendClientMessage(playerid, COLOR_GREY, "You are not near a duffle bag that you can pick up.");
		DestroyDynamicObject(DuffleBag[duffleid][dfObject]);
		AttachDufflebagToPlayer(playerid, duffleid);
		DuffleBag[duffleid][dfDropped] = false;
		KillTimer(DuffleBag[duffleid][dfTimer]);

		new str[128];
		format(str, sizeof(str), "* %s picks up a duffle bag.", PlayerICName(playerid));
		ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
	}
	else if(!strcmp(action[0], "check", true))
	{
		// duffleid is a dufflebag that's on ground near the player, however we prioritize the dufflebag that the player is holding
		new duffleid = IsPlayerNearDuffle(playerid);

		if(IsPlayerHoldingDuffleBag(playerid)) ShowPlayerDuffleContents(playerid, HoldingDufflebagID(playerid));
		else if(duffleid != -1) ShowPlayerDuffleContents(playerid, duffleid);
		else SendClientMessage(playerid, COLOR_GREY, "You are not near a duffle bag, or do not have one in hand.");
	}
	else if(!strcmp(action[0], "deposit", true))
	{
		if(!IsPlayerHoldingDuffleBag(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You are not holding a duffle bag.");
		if(sscanf(params, "s[32] s[32] ", action[0], action[1]))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag deposit [item]");
			SendClientMessage(playerid, COLOR_GRAD1, "Available Items: weapon, materials, money, cocaine, meth, cannabis, xanax, promethazine, codeine, lean");
			return 1;
		}

		new duffleid = HoldingDufflebagID(playerid);

		if(!strcmp(action[1], "weapon", true))
		{
			if(sscanf(params, "s[32] s[32] i", action[0], action[1], slot)) 
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag deposit weapon [slot]");
			}		
			if(PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GREY, "You must play for at least 2 hours before you can use this.");
			if(GetPlayerWeapon(playerid) == 0) return SendClientMessage(playerid, COLOR_GREY, "You are not holding any weapon.");
			if(IsDisallowedWeapon(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You may not store this weapon.");
			if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can't do this in a vehicle.");
			if(IsAtEvent[playerid] == 1) return SendClientMessage(playerid, COLOR_GREY, "You cannot store a weapon during an event.");
			if(IsACop(playerid) || IsANG(playerid) || IsAMedic(playerid) || IsASenate(playerid) || PlayerInfo[playerid][pMember] == 10 || PlayerInfo[playerid][pMember] == 9 || IsASANG(playerid)) return SendClientMessage(playerid, COLOR_GRAD1, "Cops/DEA/DoC/SAN/Taxi Company can not use this command.");
			if(slot >= MAX_DUFFLEBAG_WEAPONS || slot < 0) 
			{
				new msg[70];
				format(msg, sizeof(msg), "Invalid slot (slots 0-%d).", MAX_DUFFLEBAG_WEAPONS-1);
				return SendClientMessage(playerid, COLOR_GREY, msg);
			}	

			new str[128];
			new getweap = GetPlayerWeapon(playerid);
			if(HaveAdminWeapon(playerid, getweap) == getweap) return SendClientMessage(playerid, COLOR_GREY, "You cannot store admin given weapons in your duffle bag.");
			if(HaveWeapon(playerid, getweap) != getweap)
			{
				format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has attempted to duffle bag store a fake weapon.", PlayerOOCName(playerid));
				ABroadCast(COLOR_WHITE, str, 1);
				return SendClientMessage(playerid, COLOR_GREY, "You are not carrying a weapon.");
			}

			if(DuffleBag[duffleid][dfWeapon][slot] != 0) return SendClientMessage(playerid, COLOR_GREY, "There is already a weapon in that slot.");

			DuffleBag[duffleid][dfWeapon][slot] = GetPlayerWeapon(playerid);
			TakePlayerWeapon(playerid, getweap);
			SetPlayerArmedWeapon(playerid, 0);
			new weapname[40];
			GetWeaponName(getweap, weapname, sizeof(weapname));
			format(str, sizeof(str), "* %s puts their %s inside of their duffle bag.", PlayerICName(playerid), weapname);
			ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}
		else if(!strcmp(action[1], "materials", true))
		{
			if(sscanf(params, "s[32] s[32] i", action[0], action[1], amount)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag deposit materials [amount]");
			if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount.");
			if(amount > PlayerInfo[playerid][pMats]) return SendClientMessage(playerid, COLOR_GRAD1, "You don't have that many materials on you.");
			new mats = DuffleBag[duffleid][dfMaterials];
			new calc = mats + amount;
			new str[128];
			if(calc > MAX_DUFFLEBAG_MATERIALS) 
			{
				format(str, sizeof(str), "The duffle bag can only carry %d materials.", MAX_DUFFLEBAG_MATERIALS);
				return SendClientMessage(playerid, COLOR_GREY, str);
			}	

			DuffleBag[duffleid][dfMaterials] += amount;
			PlayerInfo[playerid][pMats] -= amount;
			format(str, sizeof(str), "* %s puts some materials inside of their duffle bag.", PlayerICName(playerid));
			ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}	
		else if(!strcmp(action[1], "money", true))
		{
			if(sscanf(params, "s[32] s[32] i", action[0], action[1], amount)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag deposit money [amount]");
			if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount.");
			if(amount > PlayerInfo[playerid][pCash]) return SendClientMessage(playerid, COLOR_GREY, "You don't have that much money on you.");

			new cash = DuffleBag[duffleid][dfMoney];
			new calc = cash + amount;
			
			new str[128];
			if(calc > MAX_DUFFLEBAG_MONEY) 
			{
				format(str, sizeof(str), "The duffle bag can only carry $%d money.", MAX_DUFFLEBAG_MONEY);
				return SendClientMessage(playerid, COLOR_GREY, str);
			}	
			DuffleBag[duffleid][dfMoney] += amount;
			GiveMoney(playerid, -amount);
			format(str, sizeof(str), "* %s puts some money inside of their duffle bag.", PlayerICName(playerid));
			ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}
		else if(!strcmp(action[1], "cocaine", true))
		{
			if(sscanf(params, "s[32] s[32] i", action[0], action[1], amount)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag deposit cocaine [amount]");
			if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount.");
			if(amount > PlayerInfo[playerid][pCocaine]) return SendClientMessage(playerid, COLOR_GREY, "You don't have that much cocaine on you.");

			new cocaine = DuffleBag[duffleid][dfCocaine];
			new calc = cocaine + amount;
			
			new str[128];
			if(calc > MAX_DUFFLEBAG_DRUGS) 
			{
				format(str, sizeof(str), "The duffle bag can only carry %d.0g cocaine.", MAX_DUFFLEBAG_DRUGS);
				return SendClientMessage(playerid, COLOR_GREY, str);
			}	
			DuffleBag[duffleid][dfCocaine] += amount;
			PlayerInfo[playerid][pCocaine] -= amount;
			format(str, sizeof(str), "* %s puts some cocaine inside of their duffle bag.", PlayerICName(playerid));
			ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}
		else if(!strcmp(action[1], "meth", true))
		{
			if(sscanf(params, "s[32] s[32] i", action[0], action[1], amount)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag deposit meth [amount]");
			if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount.");
			if(amount > PlayerInfo[playerid][pMeth]) return SendClientMessage(playerid, COLOR_GREY, "You don't have that much meth on you.");

			new meth = DuffleBag[duffleid][dfMeth];
			new calc = meth + amount;
			
			new str[128];
			if(calc > MAX_DUFFLEBAG_DRUGS) 
			{
				format(str, sizeof(str), "The duffle bag can only carry %d.0g meth.", MAX_DUFFLEBAG_DRUGS);
				return SendClientMessage(playerid, COLOR_GREY, str);
			}	
			DuffleBag[duffleid][dfMeth] += amount;
			PlayerInfo[playerid][pMeth] -= amount;
			format(str, sizeof(str), "* %s puts some meth inside of their duffle bag.", PlayerICName(playerid));
			ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}
		else if(!strcmp(action[1], "cannabis", true))
		{
			if(sscanf(params, "s[32] s[32] i", action[0], action[1], amount)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag deposit cannabis [amount]");
			if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount.");
			if(amount > PlayerInfo[playerid][pCannabis]) return SendClientMessage(playerid, COLOR_GREY, "You don't have that much cannabis on you.");

			new cannabis = DuffleBag[duffleid][dfCannabis];
			new calc = cannabis + amount;
			
			new str[128];
			if(calc > MAX_DUFFLEBAG_DRUGS) 
			{
				format(str, sizeof(str), "The duffle bag can only carry %d.0g cannabis.", MAX_DUFFLEBAG_DRUGS);
				return SendClientMessage(playerid, COLOR_GREY, str);
			}	
			DuffleBag[duffleid][dfCannabis] += amount;
			PlayerInfo[playerid][pCannabis] -= amount;
			format(str, sizeof(str), "* %s puts some cannabis inside of their duffle bag.", PlayerICName(playerid));
			ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}
		else if(!strcmp(action[1], "xanax", true))
		{
			if(sscanf(params, "s[32] s[32] i", action[0], action[1], amount)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag deposit xanax [amount]");
			if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount.");
			if(amount > PlayerInfo[playerid][pXanax]) return SendClientMessage(playerid, COLOR_GREY, "You don't have that much xanax on you.");

			new xanax = DuffleBag[duffleid][dfXanax];
			new calc = xanax + amount;
			
			new str[128];
			if(calc > MAX_DUFFLEBAG_DRUGS) 
			{
				format(str, sizeof(str), "The duffle bag can only carry %d.0g xanax.", MAX_DUFFLEBAG_DRUGS);
				return SendClientMessage(playerid, COLOR_GREY, str);
			}	
			DuffleBag[duffleid][dfXanax] += amount;
			PlayerInfo[playerid][pXanax] -= amount;
			format(str, sizeof(str), "* %s puts some xanax inside of their duffle bag.", PlayerICName(playerid));
			ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}
		else if(!strcmp(action[1], "promethazine", true))
		{
			if(sscanf(params, "s[32] s[32] i", action[0], action[1], amount)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag deposit promethazine [amount]");
			if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount.");
			if(amount > PlayerInfo[playerid][pPromethazine]) return SendClientMessage(playerid, COLOR_GREY, "You don't have that much promethazine on you.");

			new promethazine = DuffleBag[duffleid][dfPromethazine];
			new calc = promethazine + amount;
			
			new str[128];
			if(calc > MAX_DUFFLEBAG_DRUGS) 
			{
				format(str, sizeof(str), "The duffle bag can only carry %d.0 fl oz promethazine.", MAX_DUFFLEBAG_DRUGS);
				return SendClientMessage(playerid, COLOR_GREY, str);
			}	
			DuffleBag[duffleid][dfPromethazine] += amount;
			PlayerInfo[playerid][pPromethazine] -= amount;
			format(str, sizeof(str), "* %s puts some promethazine inside of their duffle bag.", PlayerICName(playerid));
			ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}
		else if(!strcmp(action[1], "codeine", true))
		{
			if(sscanf(params, "s[32] s[32] i", action[0], action[1], amount)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag deposit codeine [amount]");
			if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount.");
			if(amount > PlayerInfo[playerid][pCodeine]) return SendClientMessage(playerid, COLOR_GREY, "You don't have that much codeine on you.");

			new codeine = DuffleBag[duffleid][dfCodeine];
			new calc = codeine + amount;
			
			new str[128];
			if(calc > MAX_DUFFLEBAG_DRUGS) 
			{
				format(str, sizeof(str), "The duffle bag can only carry %d.0 fl oz codeine.", MAX_DUFFLEBAG_DRUGS);
				return SendClientMessage(playerid, COLOR_GREY, str);
			}	
			DuffleBag[duffleid][dfCodeine] += amount;
			PlayerInfo[playerid][pCodeine] -= amount;
			format(str, sizeof(str), "* %s puts some codeine inside of their duffle bag.", PlayerICName(playerid));
			ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}
		else if(!strcmp(action[1], "lean", true))
		{
			if(sscanf(params, "s[32] s[32] i", action[0], action[1], amount)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag deposit lean [amount]");
			if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount.");
			if(amount > PlayerInfo[playerid][pLean]) return SendClientMessage(playerid, COLOR_GREY, "You don't have that much lean on you.");

			new lean = DuffleBag[duffleid][dfLean];
			new calc = lean + amount;
			
			new str[128];
			if(calc > MAX_DUFFLEBAG_DRUGS) 
			{
				format(str, sizeof(str), "The duffle bag can only carry %d.0 fl oz codeine.", MAX_DUFFLEBAG_DRUGS);
				return SendClientMessage(playerid, COLOR_GREY, str);
			}	
			DuffleBag[duffleid][dfLean] += amount;
			PlayerInfo[playerid][pLean] -= amount;
			format(str, sizeof(str), "* %s puts some codeine inside of their duffle bag.", PlayerICName(playerid));
			ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}
	}
	else if(!strcmp(action[0], "withdraw", true))
	{
		if(!IsPlayerHoldingDuffleBag(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You are not holding a duffle bag.");
		if(sscanf(params, "s[32] s[32] ", action[0], action[1]))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag withdraw [item]");
			SendClientMessage(playerid, COLOR_GRAD1, "Available Items: weapon, materials, money, cocaine, meth, cannabis, xanax, promethazine, codeine, lean");
			return 1;
		}

		new duffleid = HoldingDufflebagID(playerid);

		if(!strcmp(action[1], "weapon", true))
		{
			if(sscanf(params, "s[32] s[32] i", action[0], action[1], slot)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag withdraw weapon [slot]");
			if(PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can use this.");
			if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GRAD2, "You can't do this in a vehicle.");
			if(IsAtEvent[playerid] == 1) return SendClientMessage(playerid, COLOR_GRAD1, "You cannot withdraw a weapon during an event.");
			if(IsACop(playerid) == 1 || IsANG(playerid) == 1 || IsAMedic(playerid) == 1 || IsASenate(playerid) || PlayerInfo[playerid][pMember] == 10 || PlayerInfo[playerid][pMember] == 9 || PlayerInfo[playerid][pMember] == FACTION_SANG) return SendClientMessage(playerid, COLOR_GRAD1, "Cops/DEA/DoC/SAN/Taxi Company can not use this command.");
			if(slot >= MAX_DUFFLEBAG_WEAPONS || slot < 0) 
			{
				new msg[70];
				format(msg, sizeof(msg), "Invalid slot (slots 0-%d).", MAX_DUFFLEBAG_WEAPONS-1);
				return SendClientMessage(playerid, COLOR_GREY, msg);
			}

			new str[128];
			if(DuffleBag[duffleid][dfWeapon][slot] == 0) return SendClientMessage(playerid, COLOR_GREY, "There is no weapon in that slot.");

			GivePlayerGun(playerid, DuffleBag[duffleid][dfWeapon][slot]);
			SetPlayerArmedWeapon(playerid, DuffleBag[duffleid][dfWeapon][slot]);

			new weapname[40];
			GetWeaponName(DuffleBag[duffleid][dfWeapon][slot], weapname, sizeof(weapname));

			DuffleBag[duffleid][dfWeapon][slot] = 0;

			format(str, sizeof(str), "* %s takes a %s from their duffle bag.", PlayerICName(playerid), weapname);
			ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}
		else if(!strcmp(action[1], "materials", true))
		{
			if(sscanf(params, "s[32] s[32] i", action[0], action[1], amount)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag withdraw materials [amount]");
			if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount.");
			if(amount > DuffleBag[duffleid][dfMaterials]) return SendClientMessage(playerid, COLOR_GRAD1, "The duffle bag doesn't have that much materials.");
			new str[128];
			DuffleBag[duffleid][dfMaterials] -= amount;
			PlayerInfo[playerid][pMats] += amount;
			format(str, sizeof(str), "* %s withdraws some materials from the dufflebag.", PlayerICName(playerid));
			ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}	
		else if(!strcmp(action[1], "money", true))
		{
			if(sscanf(params, "s[32] s[32] i", action[0], action[1], amount)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag deposit money [amount]");
			if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount.");
			if(amount > DuffleBag[duffleid][dfMoney]) return SendClientMessage(playerid, COLOR_GREY, "The duffle bag doesn't have that much money.");
			
			new str[128];	
			DuffleBag[duffleid][dfMoney] -= amount;
			GiveMoney(playerid, amount);
			format(str, sizeof(str), "* %s takes some money from their duffle bag.", PlayerICName(playerid));
			ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}
		else if(!strcmp(action[1], "cocaine", true))
		{
			if(sscanf(params, "s[32] s[32] i", action[0], action[1], amount)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag withdraw cocaine [amount]");
			if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount.");
			if(amount > DuffleBag[duffleid][dfCocaine]) return SendClientMessage(playerid, COLOR_GREY, "The duffle bag doesn't have that much cocaine.");
			
			new str[128];
			
			DuffleBag[duffleid][dfCocaine] -= amount;
			PlayerInfo[playerid][pCocaine] += amount;
			format(str, sizeof(str), "* %s takes some cocaine from their duffle bag.", PlayerICName(playerid));
			ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}
		else if(!strcmp(action[1], "meth", true))
		{
			if(sscanf(params, "s[32] s[32] i", action[0], action[1], amount)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag withdraw meth [amount]");
			if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount.");
			if(amount > DuffleBag[duffleid][dfMeth]) return SendClientMessage(playerid, COLOR_GREY, "The duffle bag doesn't have that much meth.");
			
			new str[128];
			
			DuffleBag[duffleid][dfMeth] -= amount;
			PlayerInfo[playerid][pMeth] += amount;
			format(str, sizeof(str), "* %s takes some meth from their duffle bag.", PlayerICName(playerid));
			ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}
		else if(!strcmp(action[1], "cannabis", true))
		{
			if(sscanf(params, "s[32] s[32] i", action[0], action[1], amount)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag withdraw cannabis [amount]");
			if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount.");
			if(amount > DuffleBag[duffleid][dfCannabis]) return SendClientMessage(playerid, COLOR_GREY, "The duffle bag doesn't have that much cannabis.");
			
			new str[128];
			
			DuffleBag[duffleid][dfCannabis] -= amount;
			PlayerInfo[playerid][pCannabis] += amount;
			format(str, sizeof(str), "* %s takes some cannabis from their duffle bag.", PlayerICName(playerid));
			ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}
		else if(!strcmp(action[1], "xanax", true))
		{
			if(sscanf(params, "s[32] s[32] i", action[0], action[1], amount)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag withdraw xanax [amount]");
			if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount.");
			if(amount > DuffleBag[duffleid][dfXanax]) return SendClientMessage(playerid, COLOR_GREY, "The duffle bag doesn't have that much xanax.");
			
			new str[128];
			
			DuffleBag[duffleid][dfXanax] -= amount;
			PlayerInfo[playerid][pXanax] += amount;
			format(str, sizeof(str), "* %s takes some xanax from their duffle bag.", PlayerICName(playerid));
			ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}
		else if(!strcmp(action[1], "promethazine", true))
		{
			if(sscanf(params, "s[32] s[32] i", action[0], action[1], amount)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag withdraw promethazine [amount]");
			if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount.");
			if(amount > DuffleBag[duffleid][dfPromethazine]) return SendClientMessage(playerid, COLOR_GREY, "The duffle bag doesn't have that much promethazine.");
			
			new str[128];
			
			DuffleBag[duffleid][dfPromethazine] -= amount;
			PlayerInfo[playerid][pPromethazine] += amount;
			format(str, sizeof(str), "* %s takes some promethazine from their duffle bag.", PlayerICName(playerid));
			ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}
		else if(!strcmp(action[1], "codeine", true))
		{
			if(sscanf(params, "s[32] s[32] i", action[0], action[1], amount)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag withdraw codeine [amount]");
			if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount.");
			if(amount > DuffleBag[duffleid][dfCodeine]) return SendClientMessage(playerid, COLOR_GREY, "The duffle bag doesn't have that much codeine.");
			
			new str[128];
			
			DuffleBag[duffleid][dfCodeine] -= amount;
			PlayerInfo[playerid][pCodeine] += amount;
			format(str, sizeof(str), "* %s takes some codeine from their duffle bag.", PlayerICName(playerid));
			ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}
		else if(!strcmp(action[1], "lean", true))
		{
			if(sscanf(params, "s[32] s[32] i", action[0], action[1], amount)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag withdraw lean [amount]");
			if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount.");
			if(amount > DuffleBag[duffleid][dfLean]) return SendClientMessage(playerid, COLOR_GREY, "The duffle bag doesn't have that much lean.");
			
			new str[128];
			
			DuffleBag[duffleid][dfLean] -= amount;
			PlayerInfo[playerid][pLean] += amount;
			format(str, sizeof(str), "* %s takes some lean from their duffle bag.", PlayerICName(playerid));
			ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}

	}
	else if(!strcmp(action[0], "show", true))
	{
		if(!IsPlayerHoldingDuffleBag(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You do not have a duffle bag in hand.");
		new target;
		if(sscanf(params, "s[32] u", action[0], target)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag show [PartOfName/playerid]");
		if(!IsPlayerConnected(target) || target == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_GREY, "Invalid player.");
		if(!IsPlayerInRangeOfPlayer(3, playerid, target)) return SendClientMessage(playerid, COLOR_GREY, "You are not near that player.");
		
		new str[128];
		ShowPlayerDuffleContents(target, HoldingDufflebagID(playerid));
		format(str, sizeof(str), "* %s shows %s their duffle bag content.", PlayerICName(playerid), PlayerICName(target));
		ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	}
	else if(!strcmp(action[0], "drop", true))
	{
		if(!IsPlayerHoldingDuffleBag(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You do not have a duffle bag in hand.");
		if(IsPlayerRestricted(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can not do this right now.");

		DropDufflebag(playerid, HoldingDufflebagID(playerid));
		new str[128];
		format(str, sizeof(str), "* %s drops a duffle bag.", PlayerICName(playerid));
		ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
	}
	else if(!strcmp(action[0], "give", true))
	{
		if(!IsPlayerHoldingDuffleBag(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You do not have a duffle bag in hand.");
		if(IsPlayerRestricted(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can not do this right now.");
		new target;
		if(sscanf(params, "s[32] u", action[0], target)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag give [PartOfName/playerid]");
		if(!IsPlayerConnected(target) || target == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_GREY, "Invalid player.");
		if(!IsPlayerInRangeOfPlayer(3, playerid, target)) return SendClientMessage(playerid, COLOR_GREY, "You are not near that player.");
		if(IsPlayerHoldingDuffleBag(target)) return SendClientMessage(playerid, COLOR_GREY, "That player already has a duffle bag in their hand.");

		new duffleid = HoldingDufflebagID(playerid); // get the id of the duffle bag that is given
		RemoveDufflebagFromPlayer(playerid, duffleid); // remove it from the player's hand, this will not reset anything but the player's variables, hence why we stored the id above
		AttachDufflebagToPlayer(target, duffleid); // give it to the target

		new str[128];
		format(str, sizeof(str), "* %s hands %s their duffle bag.", PlayerICName(playerid), PlayerICName(target));
		ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	}
	else if(!strcmp(action[0], "take", true))
	{
		if(!IsAllowedDuffleFaction(playerid)) return 1;
		if(IsPlayerRestricted(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can not do this right now.");
		new target;
		if(sscanf(params, "s[32] u", action[0], target)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dufflebag take [PartOfName/playerid]");
		if(!IsPlayerConnected(target) || target == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_GREY, "Invalid player.");
		if(!IsPlayerInRangeOfPlayer(3, playerid, target)) return SendClientMessage(playerid, COLOR_GREY, "You are not near that player.");
		if(!IsPlayerHoldingDuffleBag(target)) return SendClientMessage(playerid, COLOR_GREY, "That player doesn't have a duffle bag in their hand.");
		if(!PlayerCuffed[target]) return SendClientMessage(playerid, COLOR_GREY, "The player must be restrained first.");
		if(IsPlayerHoldingDuffleBag(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You already have a duffle bag in hand.");


		new duffleid = HoldingDufflebagID(target); // get the id of the duffle bag that is given
		RemoveDufflebagFromPlayer(target, duffleid); // remove it from the player's hand, this will not reset anything but the player's variables, hence why we stored the id above
		AttachDufflebagToPlayer(playerid, duffleid); // give it to the target

		new str[128];
		format(str, sizeof(str), "* %s forcefully takes %s's duffle bag.", PlayerICName(playerid), PlayerICName(target));
		ProxDetector(10.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	}
	return 1;
}
