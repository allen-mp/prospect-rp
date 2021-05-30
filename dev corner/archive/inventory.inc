/* /give & /drop & related functions reworked by Dwayne. */

enum WeaponDataEx
{
	wep_Slot,
	wep_Name[32],
	wep_Model
}

new g_WeaponInfo[46][WeaponDataEx] = {
	{0, "Fist", -1}, {0, "Brass Knuckles", 331},
	{1, "Golfclub", 333}, {1, "Nightstick", 334}, {1, "Knife", 335}, {1, "Bat", 336}, {1, "Shovel", 337}, {1, "Poolstick", 338}, {1, "Katana", 339}, {1, "Chainsaw", 341},
	{10, "Purple Dildo", 321}, {10, "White Dildo", 322}, {10, "Vibrator", 323}, {10, "Silver Vibrator", 324}, {10, "Flowers", 325}, {10, "Cane", 326},
	{8, "Grenade", 342}, {8, "Tear Gas", 343}, {8, "Molotov Cocktail", 344},
	{0, "Vehicle Missle", -1}, {0, "Hydra Flare", -1}, {0, "Jetpack", 370},
	{2, "Colt 45", 346}, {2, "Silenced Pistol", 347}, {2, "Desert Eagle", 348},
	{3, "Shotgun", 349}, {3, "Sawnoff Shotgun", 350}, {3, "Combat Shotgun", 351},
	{4, "Micro Uzi", 352}, {4, "MP5", 353},
	{5, "AK-47", 355}, {5, "M4", 356},
	{4, "Tec-9", 372},
	{6, "Rifle", 357}, {6, "Sniper Rifle", 358},
	{7, "Rocket Launcher", 359}, {7, "Heatseeking Rocket Launcher", 360}, {7, "Flamethrower", 361}, {7, "Minigun", 362},
	{8, "Satchel Charge", 363},
	{12, "Detonator", 364},
	{9, "Spraycan", 365}, {9, "Fire Extinguisher", 366}, {9, "Camera", 367},
	{11, "Nightvision Goggles", 368}, {11, "Thermal Goggles", 369}
};

new LastDrop[MAX_PLAYERS];
CMD:give(playerid, params[])
{
    if(inPaintball[playerid] || PlayerTazed[playerid] || PlayerCuffed[playerid] || PlayerFrozen[playerid] || PlayerInfo[playerid][pAdminJailed] == 1) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You cannot do this right now.");

	new giveplayerid = INVALID_PLAYER_ID, item[10], amount;
	if(sscanf(params, "us[10]I(0)", giveplayerid, item, amount))
	{
	    SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /give [playerid/PartOfName] [name] [amount]");
		SendClientMessage(playerid, COLOR_GRAD1, "Available names: Pot, Crack, Materials, Seeds, Weapon");
	}

	else
	{
	    if(playerid == giveplayerid) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You cannot give things to yourself.");
	    if(!IsPlayerConnected(giveplayerid)) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}That player isn't connected.");
		if(strmatch(item, "weapon"))
	    {
	        if(IsAtEvent[playerid] == 1) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You cannot give weapons while at an event.");
	        if(IsACop(playerid) || IsAFireman(playerid) || IsANG(playerid) || IsASenate(playerid) || PlayerInfo[playerid][pMember] == 10) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}Cops/FMD/NG/Senate/Taxi cannot use this command.");
     		if(!IsPlayerInRangeOfPlayer(2.0, playerid, giveplayerid)) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You must be near the player you're trying to give your weapon to.");
            if(PlayerInfo[giveplayerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}That player must play for 2 hours before using a weapon.");
			if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || GetPlayerState(giveplayerid) != PLAYER_STATE_ONFOOT) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You and the other player must be on foot.");
			if(PlayerTazed[giveplayerid] || PlayerCuffed[giveplayerid]) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}That player cannot get weapons right now.");
			if(GetPlayerWeapon(playerid) == 0) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You are not holding a weapon.");

			if(g_pGunInfo[playerid][GetWeaponIndex(GetPlayerWeapon(playerid))][wo_Gun] == 0)
			{
			    format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has attempted to give a fake weapon.", PlayerICName(playerid));
                ABroadCast(COLOR_WHITE, str, 1);

                return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You are not holding a weapon.");
			}
			else
			{
			    if(IsValidWeapon(playerid, GetPlayerWeapon(playerid)) == GUN_TYPE_ADMIN) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You cannot give this weapon.");
			    OfferedGunTo[playerid] = giveplayerid;
			    OfferedGunFrom[giveplayerid] = playerid;
			    GunOffered[giveplayerid] = GetPlayerWeapon(playerid);

			    format(str, sizeof str, "You have offered %s your %s, please wait for them to react to your offer.", PlayerICName(giveplayerid), g_WeaponInfo[GetPlayerWeapon(playerid)][wep_Name]);
			    SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

			    format(str, sizeof str, "%s has offered you their %s, /accept weapon to take the weapon.", PlayerICName(playerid), g_WeaponInfo[GetPlayerWeapon(playerid)][wep_Name]);
			    SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);

			    format(str, sizeof str, "{FF0000}AdmWarning: {FFFFFF}%s[%d] has offered their %s to %s[%d].", PlayerICName(playerid), playerid, g_WeaponInfo[GetPlayerWeapon(playerid)][wep_Name], PlayerICName(giveplayerid), giveplayerid);
				ABroadCast(COLOR_LIGHTRED, str, 1);
			}
		}


		else if(strmatch(item, "pot"))
		{
		    if(IsPlayerConnected(PotOffer[playerid])) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You cannot give pot/crack while selling an item.");
		    if(PlayerInfo[playerid][pPot] < amount) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You do not have that much pot on your person.");
  			if(!IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid)) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You must be near the player you're trying to give your pot to.");
			if(amount <= 0 || PlayerInfo[giveplayerid][pPot]+amount >= 51) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}Invalid pot amount, or that player cannot hold that much pot.");

			format(str, sizeof str, "* %s has given %s some pot.", PlayerICName(playerid), PlayerICName(giveplayerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

			format(str, sizeof str, "You have given %s %d grams of pot.", PlayerICName(giveplayerid), amount);
			SendClientMessage(playerid, COLOR_GREY, str);

			format(str, sizeof str, "You have recieved %d grams of pot from %s.", amount, PlayerICName(giveplayerid));
			SendClientMessage(giveplayerid, COLOR_GREY, str);

			PlayerInfo[giveplayerid][pPot] += amount;
			PlayerInfo[playerid][pPot] -= amount;

			format(str, sizeof str, "%s has given %s %d pot.", PlayerICName(playerid), PlayerICName(giveplayerid), amount);
			PayLog(str);
		}

		else if(strmatch(item, "crack"))
		{
		    if(IsPlayerConnected(CrackOffer[playerid])) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You cannot give pot/crack while selling an item.");
		    if(amount <= 0 || PlayerInfo[playerid][pCrack] < amount) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You do not have that much crack.");
		    if(!IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid)) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You are not near that player.");
			if(PlayerInfo[giveplayerid][pCrack] + 50 >= 50) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}That player cannot hold that much crack.");

			format(str, sizeof str, "* %s has given %s some crack.", PlayerICName(playerid), PlayerICName(giveplayerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

			format(str, sizeof str, "You have given %s %d grams of crack.", PlayerICName(giveplayerid), amount);
			SendClientMessage(playerid, COLOR_GREY, str);

			format(str, sizeof str, "You have recieved %d grams of crack from %s.", amount, PlayerICName(giveplayerid));
			SendClientMessage(giveplayerid, COLOR_GREY, str);

			PlayerInfo[giveplayerid][pCrack] += amount;
			PlayerInfo[playerid][pCrack] -= amount;

			format(str, sizeof str, "%s has given %s %d crack.", PlayerICName(playerid), PlayerICName(giveplayerid), amount);
			PayLog(str);
		}

		else if(strmatch(item, "materials"))
		{
		    if(amount <= 0 || PlayerInfo[playerid][pMats] < amount) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You do not have enough materials.");
		    if(amount >= 500001) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You cannot give more than 500,000 materials.");
  			if(!IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid)) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You are not near that player.");

            format(str, sizeof str, "* %s has given %s some materials.", PlayerICName(playerid), PlayerICName(giveplayerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

			format(str, sizeof str, "You have given %s %d materials.", PlayerICName(giveplayerid), amount);
			SendClientMessage(playerid, COLOR_GREY, str);

			format(str, sizeof str, "You have recieved %d materials from %s.", amount, PlayerICName(giveplayerid));
			SendClientMessage(giveplayerid, COLOR_GREY, str);

			PlayerInfo[giveplayerid][pMats] += amount;
			PlayerInfo[playerid][pMats] -= amount;

			format(str, sizeof str, "%s has given %s %d mats.", PlayerICName(playerid), PlayerICName(giveplayerid), amount);
			PayLog(str);
		}

		else if(strmatch(item, "seeds"))
		{
		    if(!IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid)) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You are not near that player.");
		    if(amount <= 0 || PlayerInfo[playerid][pPotSeeds] < amount) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You do not have enough pot seeds.");
			if(PlayerInfo[giveplayerid][pPotSeeds] + amount >= 51) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}That player can only hold 50 seeds on their person.");

            format(str, sizeof str, "* %s has given %s some seeds.", PlayerICName(playerid), PlayerICName(giveplayerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

			format(str, sizeof str, "You have given %s %d seeds.", PlayerICName(giveplayerid), amount);
			SendClientMessage(playerid, COLOR_GREY, str);

			format(str, sizeof str, "You have recieved %d seeds from %s.", amount, PlayerICName(giveplayerid));
			SendClientMessage(giveplayerid, COLOR_GREY, str);

			PlayerInfo[giveplayerid][pPotSeeds] += amount;
			PlayerInfo[playerid][pPotSeeds] -= amount;

			format(str, sizeof str, "%s has given %s %d seeds.", PlayerICName(playerid), PlayerICName(giveplayerid), amount);
			PayLog(str);
		}
		else SendClientMessage(playerid, COLOR_GREY, "Invalid item.");
	}

	return true;
}

// Item Dropping system by Dwayne Dutch Schons, 4/28/2018 - 5/3/2015
/* Notes:
	* Use the Item_Clear function to completely remove an item.
	* Use the Item_SetData function to modify a current object, or create a new one. To generate a vacant slot for creation, use Item_GetVacantSlot().
	* When accessing ItemInfo, do not go above MAX_ACTIVE_ITEMS - 1 and do not go below 0(obviously).
	* Item_GetVacantSlot returns -1 if no vacant item slots are available.
	* You can use Item_GetType to return pot, materials, weapons, etc as an item type ID.
		Usage example:
			Item_GetClosest(playerid, 5.0, Item_GetType("pot"));
*/
#define MAX_ACTIVE_ITEMS         (100)

#define ITEMTYPE_NONE            (0)
#define ITEMTYPE_WEAPON          (1)
#define ITEMTYPE_CRACK           (2)
#define ITEMTYPE_POT        	 (3)
#define ITEMTYPE_MATERIALS       (4)
#define ITEMTYPE_CLIPS           (6)

enum ItemInfo
{
	bool: iActive,
	iItemType,
	iItemValue,
	iItemAmount,
	Float: iPos[3],
	iWorld,
	iInterior,
	iTimer,
	iObject,
	Text3D:iLabel
}

new ItemData[MAX_ACTIVE_ITEMS][ItemInfo];
Item_GetVacantSlot()
{
	for(new i; i < MAX_ACTIVE_ITEMS; i++)
	{
	    if(ItemData[i][iActive] == false)
	        return i;
	}

	return -1;
}

Item_GetType(type[])
{
	new itemtype = ITEMTYPE_NONE;
	if(strcmp(type, "weapon", true) == 0)
	    itemtype = ITEMTYPE_WEAPON;

	else if(strcmp(type, "crack", true) == 0)
	    itemtype = ITEMTYPE_CRACK;

	else if(strcmp(type, "pot", true) == 0)
	    itemtype = ITEMTYPE_POT;

	else if(strcmp(type, "materials", true) == 0)
	    itemtype = ITEMTYPE_MATERIALS;


	else if(strcmp(type, "clips", true) == 0)
	    itemtype = ITEMTYPE_CLIPS;

	return itemtype;
}

Item_SetData(itemid, bool: active, itemtype, value, amount, Float: x, Float: y, Float: z, world, interior, bool: settimer = false, bool: createobject = false, time = 30000)
{
	if(itemid >= 0 && itemid < MAX_ACTIVE_ITEMS && !(itemtype <= ITEMTYPE_NONE || itemtype > ITEMTYPE_CLIPS))
	{
		ItemData[itemid][iActive] = active;
		ItemData[itemid][iItemType] = itemtype;
		ItemData[itemid][iItemValue] = value;
		ItemData[itemid][iItemAmount] = amount;
		ItemData[itemid][iPos][0] = x, ItemData[itemid][iPos][1] = y, ItemData[itemid][iPos][2] = z;
		ItemData[itemid][iWorld] = world;
		ItemData[itemid][iInterior] = interior;

		if(createobject)
		{
		    if(IsValidDynamicObject(ItemData[itemid][iObject]))
		        DestroyDynamicObject(ItemData[itemid][iObject]);

			switch(itemtype)
			{
			    case ITEMTYPE_WEAPON:
				{
					ItemData[itemid][iObject] = CreateDynamicObject(g_WeaponInfo[value][wep_Model], x, y, z -1, 93.7, 120.0, 120.0, world, interior, -1, 100.0);

					format(str, sizeof str, "Dropped %s", g_WeaponInfo[value][wep_Name]);
					ItemData[itemid][iLabel] = CreateDynamic3DTextLabel(str, COLOR_YELLOW, x, y, z - 0.8, 30.0);
				}
				case ITEMTYPE_POT:
				{
					ItemData[itemid][iObject] = CreateDynamicObject(1578, x, y, z -1, 180.0, 180.0, 120.0, world, interior, -1, 100.0);
					ItemData[itemid][iLabel] = CreateDynamic3DTextLabel("Dropped bag of pot", COLOR_YELLOW, x, y, z - 0.8, 30.0);
				}
				case ITEMTYPE_CRACK:
				{
					ItemData[itemid][iObject] = CreateDynamicObject(1575, x, y, z-1, 180.0, 180.0, 120.0, world, interior, -1, 100.0);
					ItemData[itemid][iLabel] = CreateDynamic3DTextLabel("Dropped bag of crack", COLOR_YELLOW, x, y, z - 0.8, 30.0);
				}
				case ITEMTYPE_MATERIALS:
				{
					ItemData[itemid][iObject] = CreateDynamicObject(1580, x, y, z -1, 180.0, 180.0, 120.0, world, interior, -1, 100.0);
					ItemData[itemid][iLabel] = CreateDynamic3DTextLabel("Dropped bag of materials", COLOR_YELLOW, x, y, z - 0.8, 30.0);
				}
				case ITEMTYPE_CLIPS:
				{
				    ItemData[itemid][iObject] = CreateDynamicObject(2040, x, y, z- 0.9, 0.0, 0.0, 0.0, world, interior, -1, 100.0);

				    format(str, sizeof str, "Dropped bag of %s clips", g_WeaponInfo[amount][wep_Name]);
			        ItemData[itemid][iLabel] = CreateDynamic3DTextLabel(str, COLOR_YELLOW, x, y, z - 0.8, 30.0);
				}
			}
		}

		if(settimer && active == true)
			ItemData[itemid][iTimer] = SetTimerEx("Item_Destroy", time, false, "i", itemid);

		return 1;
	}

	return 0;
}

forward Item_Destroy(itemid);
public Item_Destroy(itemid)
	return Item_Clear(itemid);

Item_Clear(itemid)
{
    if(itemid >= 0 && itemid < MAX_ACTIVE_ITEMS)
	{
	    ItemData[itemid][iActive] = false;
	    ItemData[itemid][iItemType] = ITEMTYPE_NONE;
	    ItemData[itemid][iItemValue] = 0;
	    ItemData[itemid][iItemAmount] = 0;
	    ItemData[itemid][iPos][0] = 0.0, ItemData[itemid][iPos][1] = 0.0, ItemData[itemid][iPos][2] = 0.0;

	    ItemData[itemid][iWorld] = 0;
	    ItemData[itemid][iInterior] = 0;

        if(IsValidDynamicObject(ItemData[itemid][iObject]))
        	DestroyDynamicObject(ItemData[itemid][iObject]);

		if(ItemData[itemid][iTimer] != 0)
		{
		    KillTimer(ItemData[itemid][iTimer]);
		    ItemData[itemid][iTimer] = 0;
		}

		if(IsValidDynamic3DTextLabel(ItemData[itemid][iLabel]))
		{
		    DestroyDynamic3DTextLabel(ItemData[itemid][iLabel]);
		    ItemData[itemid][iLabel] = Text3D:INVALID_3DTEXT_ID;
		}

		return 1;
	}

	return 0;
}

Item_GetClosest(playerid, Float: distance = 5.0, itemtype=-1)
{
	new Float: iCurDistance, Float: iLastDistance = distance, iCurrent = -1;
	for(new i; i < MAX_ACTIVE_ITEMS; i++)
	{
	    if(ItemData[i][iActive] == true)
	    {
	        if(itemtype != -1 && ItemData[i][iItemType] != itemtype)
	            continue;

			if(GetPlayerVirtualWorld(playerid) != ItemData[i][iWorld])
				continue;

			iCurDistance = GetPlayerDistanceFromPoint(playerid, ItemData[i][iPos][0], ItemData[i][iPos][1], ItemData[i][iPos][2]);
			if(iCurDistance < iLastDistance)
			{
			    iCurrent = i;
			    iLastDistance = iCurDistance;
			}
		}
	}

	return iCurrent;
}

CMD:itemhelp(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 4)
	{
		SendClientMessage(playerid, COLOR_GREY, "Admin Item Commands: ");
		SendClientMessage(playerid, COLOR_GRAD1, "/iteminfo /gotoitem /createitem /deleteitem");
	}
	else SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You're not authorized to use this command.");
	return true;
}

CMD:iteminfo(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 4)
	{
	    new iItem = Item_GetClosest(playerid, 5.0);
	    if(iItem == -1)
	        return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You are not near any items.");

		format(str, sizeof str, "(ID %i) (Type %i) (x, %f), (y, %f) (z, %f) (value, %i) (amount, %i)", iItem, ItemData[iItem][iItemType], ItemData[iItem][iPos][0], ItemData[iItem][iPos][1], ItemData[iItem][iPos][2], ItemData[iItem][iItemValue], ItemData[iItem][iItemAmount]);
		ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "Item Info", str, "Close", "");
	}

	return true;
}

CMD:gotoitem(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 4)
	{
		new itemid;
		if(sscanf(params, "i", itemid)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /gotoitem [itemid]");

		if(itemid >= 0 && itemid < MAX_ACTIVE_ITEMS)
		{
		    if(ItemData[itemid][iActive] == false)
		        return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}Provided item ID is not in use.");

			SetPlayerPos(playerid, ItemData[itemid][iPos][0], ItemData[itemid][iPos][1], ItemData[itemid][iPos][2]);
			SetPlayerVirtualWorld(playerid, ItemData[itemid][iWorld]);
			SetPlayerInterior(playerid, ItemData[itemid][iInterior]);

			Streamer_Update(playerid);
			format(str, sizeof str, "Teleporting to ~b~item %d~w~.", itemid);
			displayCenterHUDInfo(playerid,  str, 8);
		}
		else SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}Invalid item ID provided.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You're not authorized to use this command.");
	return true;
}

CMD:createitem(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 4)
	{
	    new itemtype, value, amount, hidden, time;
	    if(sscanf(params, "iiiiI(30)", itemtype, value, amount, hidden, time))
	    {
	        SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /createitem [itemtype] [value] [amount] [show object] [seconds(by default 30)]");
	        SendClientMessage(playerid, COLOR_GRAD1, "Item Types: (1 - Weapon ID) (2 - Crack) (3 - Pot) (4 - Materials) (6 - Clips)");
	        SendClientMessage(playerid, COLOR_GRAD2, "Values: (1 - Weapon ID) (2 - Amount of Crack) (3 - Amount of Pot) (4 - Amount of Materials) (6 - Amount of Clips)");
	        SendClientMessage(playerid, COLOR_GRAD2, "Amounts: (1 - Ammo) (6 - Weapon ID)");
	        SendClientMessage(playerid, COLOR_GRAD3, "Note: The optional second/time value allows you to set the amount of time(in seconds) before the item is automatically deleted.");
			return true;
		}

		if(itemtype <= ITEMTYPE_NONE || itemtype > ITEMTYPE_CLIPS)
		    return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}Invalid item type(weapon, crack, pot, materials or clips).");

		if(value < 0 || itemtype == ITEMTYPE_WEAPON && value >= 47)
			return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}Invalid value provided.");

		new item = Item_GetVacantSlot(), Float: x, Float: y, Float: z;
		GetPlayerPos(playerid, x, y, z);

		if(Item_SetData(item, true, itemtype, value, amount, x, y, z, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), true, true, time*1000) == 1)
		{
		    format(str, sizeof str, "{FF0000}AdmWarning: {FFFFFF}%s has spawned item %d at their location.", PlayerICName(playerid), item);
			ABroadCast(COLOR_LIGHTRED, str, 3);

			format(str, sizeof str, "You have spawned item %d at your location.", item);
			SendClientMessage(playerid, COLOR_YELLOW, str);

			Streamer_Update(playerid);
		}
		else SendClientMessage(playerid, COLOR_GREY, "Unable to create the requested item, Item_SetData failed.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You're not authorized to use this command.");
	return true;
}

CMD:deleteitem(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] >= 4)
	{
		new itemid;
		if(sscanf(params, "i", itemid)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /deleteitem [itemid]");

		if(itemid >= 0 && itemid < MAX_ACTIVE_ITEMS)
		{
		    if(ItemData[itemid][iActive] == false)
		        return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}Provided item ID is not in use.");

			Item_Clear(itemid);
			format(str, sizeof str, "You have deleted item %d.", itemid);
			SendClientMessage(playerid, COLOR_WHITE, str);
			format(str, sizeof str, "{FF0000}AdmWarning: {FFFFFF}%s has deleted item %d.", PlayerICName(playerid), itemid);
		}
		else SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}Invalid item ID provided.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You're not authorized to use this command.");
	return true;
}

CMD:inspect(playerid, params[])
{
	if(GetPlayerSpecialAction(playerid) != SPECIAL_ACTION_DUCK)
	    return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You must be crouching to inspect an item.");

	new item = Item_GetClosest(playerid, 5.0);
	if(item == -1)
	    return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You are not near any items.");

	switch(ItemData[item][iItemType])
	{
	    case ITEMTYPE_WEAPON:
	    {
	        format(str, sizeof str, "Loaded %s(%d rounds)", g_WeaponInfo[ItemData[item][iItemValue]][wep_Name], ItemData[item][iItemAmount]);
	        SendClientMessage(playerid, COLOR_WHITE, str);
		}

		case ITEMTYPE_POT:
		{
		    format(str, sizeof str, "Bag containing %d grams of pot.", ItemData[item][iItemValue]);
		    SendClientMessage(playerid, COLOR_WHITE, str);
		}

		case ITEMTYPE_CRACK:
		{
		    format(str, sizeof str, "Bag containing %d grams of crack.", ItemData[item][iItemValue]);
		    SendClientMessage(playerid, COLOR_WHITE, str);
		}

		case ITEMTYPE_MATERIALS:
		{
		    format(str, sizeof str, "Bag containing %d materials.", ItemData[item][iItemValue]);
		    SendClientMessage(playerid, COLOR_WHITE, str);
		}


		case ITEMTYPE_CLIPS:
		{
		    format(str, sizeof str, "%d Weapon Clips - %s", ItemData[item][iItemAmount], g_WeaponInfo[ItemData[item][iItemValue]][wep_Name]);
		    SendClientMessage(playerid, COLOR_WHITE, str);
		}
	}

	format(str, sizeof str, "* %s leans down, inspecting the item infront of them.", PlayerICName(playerid));
	ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	return true;
}

CMD:get(playerid, params[])
{
	if(PlayerTazed[playerid] || PlayerCuffed[playerid] || PlayerFrozen[playerid] || PlayerInfo[playerid][pAdminJailed] || inPaintball[playerid])
	    return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You can't do this right now.");

	new item = isnull(params) ? Item_GetClosest(playerid, 4.0, -1) : Item_GetClosest(playerid, 4.0, Item_GetType(params));
	if(item == -1)
	    return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You are not near any items.");

	else
	{
	    if(GetPlayerSpecialAction(playerid) != SPECIAL_ACTION_DUCK)
	        return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You must be crouching to grab an item.");

	    switch(ItemData[item][iItemType])
	    {
	        case ITEMTYPE_NONE: return SendClientMessage(playerid, COLOR_GREY, "(( Please submit a bug report mentioning bug ID 39. ))");
	        case ITEMTYPE_WEAPON:
	        {
	            if(PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You cannot pickup a weapon with less than 2 playing hours.");

	            GivePlayerGun(playerid, ItemData[item][iItemValue], ItemData[item][iItemAmount]);

	            format(str, sizeof str, "* %s has picked up a %s.", PlayerICName(playerid), g_WeaponInfo[ItemData[item][iItemValue]][wep_Name]);
	            ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

	            format(str, sizeof str, "You have picked up a %s.", g_WeaponInfo[ItemData[item][iItemValue]][wep_Name]);
	            SendClientMessage(playerid, COLOR_GREEN, str);
			}
			case ITEMTYPE_POT:
			{
				if(PlayerInfo[playerid][pPot] + ItemData[item][iItemValue] >= 51)
				    return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You cannot carry that much pot.");

				PlayerInfo[playerid][pPot] += ItemData[item][iItemValue];

				format(str, sizeof str, "* %s has picked up a bag of pot.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

				SendClientMessage(playerid, COLOR_GREEN, "You have picked up a bag of pot.");
			}

			case ITEMTYPE_CRACK:
			{
                if(PlayerInfo[playerid][pCrack] + ItemData[item][iItemValue] >= 51)
				    return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You cannot carry that much crack.");

				PlayerInfo[playerid][pCrack] += ItemData[item][iItemValue];

				format(str, sizeof str, "* %s has picked up a bag of crack.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

				SendClientMessage(playerid, COLOR_GREEN, "You have picked up a bag of crack.");
			}

			case ITEMTYPE_MATERIALS:
			{
			    if(PlayerInfo[playerid][pMats] + ItemData[item][iItemValue] >= 51)
				    return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You cannot carry that many materials.");

				PlayerInfo[playerid][pMats] += ItemData[item][iItemValue];

				format(str, sizeof str, "* %s has picked up a bag of materials.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

				SendClientMessage(playerid, COLOR_GREEN, "You have picked up a bag of materials.");
			}

			case ITEMTYPE_CLIPS:
			{
				if(PlayerInfo[playerid][pConnectTime] < 2)
				    return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You must play for 2 hours before using weapons/clips.");

				GivePlayerClips(playerid, ItemData[item][iItemAmount], ItemData[item][iItemValue]);

				format(str, sizeof str, "* %s has picked up a box of weapon clips.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

				SendClientMessage(playerid, COLOR_GREEN, "You have picked up a box of weapon clips.");
			}

			default: return true;
		}

		ApplyAnimationEx(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
		Item_Clear(item);
		Streamer_Update(playerid);
	}

	return 1;
}

CMD:drop(playerid, params[])
{
	if(pInHospital[playerid] || PlayerTazed[playerid] || PlayerCuffed[playerid] || PlayerFrozen[playerid] || IsAtEvent[playerid] || IsPlayerInAnyVehicle(playerid) || inPaintball[playerid]) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You can't do this right now.");
	if(isnull(params))
	{
	    SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /drop [name]");
		SendClientMessage(playerid, COLOR_GRAD1, "Available names: Weapon, Pot, Crack, Materials, Packages, Crates, Tikis, Seeds, Meal, Boombox");
		SendClientMessage(playerid, COLOR_GRAD1, "Available names: Briefcase, Cellphone, Products, Screwdrivers, Ropes, Jerrycan, Trash, Clips");
		if(IsACop(playerid) || IsANG(playerid) || IsAFireman(playerid) || IsASenate(playerid) || PlayerInfo[playerid][pMember] == 10)
		    SendClientMessage(playerid, COLOR_GRAD2, "Note: Dropped weapons/weapon clips will not be able to be picked up by players due to your faction affiliation.");

		return true;
	}

	else
	{
	    if(LastDrop[playerid] >= gettime()-2) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You must wait 2 seconds before using this command again.");
		LastDrop[playerid] = gettime();
	    if(strmatch(params, "weapon"))
	    {
	        if(playerHoldingTazer[playerid] != 0) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You cannot drop a tazer.");
	        if(GetPlayerWeapon(playerid) == 0) return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You are not holding a weapon.");
			if(g_pGunInfo[playerid][GetWeaponIndex(GetPlayerWeapon(playerid))][wo_Gun] == 0)
			{
   				format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has attempted to drop a fake/desynced weapon.", PlayerICName(playerid));
       			ABroadCast(COLOR_WHITE, str, 1);
                return SendClientMessage(playerid, COLOR_LIGHTRED, "{FF0000}Error: {FFFFFF}You are not holding a weapon.");
			}

			if(IsValidWeapon(playerid, GetPlayerWeapon(playerid)) == GUN_TYPE_NORMAL)
			{
			    if(IsACop(playerid) || IsANG(playerid) || IsASenate(playerid) || IsAFireman(playerid) || PlayerInfo[playerid][pMember] == 10)
			    {
			        format(str, sizeof str, "* %s has dropped their %s.", PlayerICName(playerid), g_WeaponInfo[GetPlayerWeapon(playerid)][wep_Name]);
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					TakePlayerWeapon(playerid, GetPlayerWeapon(playerid));
				}
				else
				{
				    new Float: x, Float: y, Float: z;
				    GetPlayerPos(playerid, x, y, z);

				    if(Item_SetData(Item_GetVacantSlot(), true, ITEMTYPE_WEAPON, GetPlayerWeapon(playerid), g_pGunInfo[playerid][GetWeaponIndex(GetPlayerWeapon(playerid))][wo_Ammo], x, y, z, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), true, true) == 1)
				    {
						format(str, sizeof str, "* %s has dropped their %s(/get to pick it up).", PlayerICName(playerid), g_WeaponInfo[GetPlayerWeapon(playerid)][wep_Name]);
                        ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

						if(GetPlayerWeapon(playerid) == 40 && BombID[playerid] != 0)
						{
						    DestroyDynamicObject(BombID[playerid]);
						    BombID[playerid] = -1;

						    SendClientMessage(playerid, COLOR_LIGHTBLUE, "The assosciated bomb has been removed due to you dropping the detonator.");
						}

						TakePlayerWeapon(playerid, GetPlayerWeapon(playerid));
                        SetPlayerArmedWeapon(playerid, 0);

						Streamer_Update(playerid);
					}
				}
			}
			else
			{
			    format(str, sizeof str, "* %s has dropped their %s.", PlayerICName(playerid), g_WeaponInfo[GetPlayerWeapon(playerid)][wep_Name]);
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				TakePlayerWeapon(playerid, GetPlayerWeapon(playerid));
			}

			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}

		else if(strmatch(params, "pot"))
		{
		    if(PlayerInfo[playerid][pPot] < 1)
		        return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You do not have any pot on your person.");

			if(PlayerInfo[playerid][pPot] > 50)
			    return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You cannot drop that much pot.");

			else
			{
			    new Float: x, Float: y, Float: z;
			    GetPlayerPos(playerid, x, y, z);

			    if(Item_SetData(Item_GetVacantSlot(), true, ITEMTYPE_POT, PlayerInfo[playerid][pPot], 0, x, y, z, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), true, true) == 1)
				{
				    format(str, sizeof str, "* %s has dropped a bag of pot(/get to pick it up).", PlayerICName(playerid));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

					PlayerInfo[playerid][pPot] = 0;
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				}
			}
		}

		else if(strmatch(params, "crack"))
		{
		    if(PlayerInfo[playerid][pCrack] < 1)
		        return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You do not have any crack on your person.");

			if(PlayerInfo[playerid][pCrack] > 50)
			    return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You cannot drop that much crack.");

			else
			{
			    new Float: x, Float: y, Float: z;
			    GetPlayerPos(playerid, x, y, z);

			    if(Item_SetData(Item_GetVacantSlot(), true, ITEMTYPE_CRACK, PlayerInfo[playerid][pCrack], 0, x, y, z, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), true, true) == 1)
				{
				    format(str, sizeof str, "* %s has dropped a bag of crack(/get to pick it up).", PlayerICName(playerid));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

					PlayerInfo[playerid][pCrack] = 0;
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				}
			}
		}

		else if(strmatch(params, "materials"))
		{
		    if(PlayerInfo[playerid][pMats] < 1)
		        return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You do not have any materials on your person.");

			new Float: x, Float: y, Float: z;
			if(Item_SetData(Item_GetVacantSlot(), true, ITEMTYPE_MATERIALS, PlayerInfo[playerid][pMats], 0, x, y, z, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), true, true) == 1)
			{
				format(str, sizeof str, "* %s has dropped a bag of materials(/get to pick it up).", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

				PlayerInfo[playerid][pMats] = 0;
				PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
			}
		}

		else if(strmatch(params, "packages"))
		{
			if(Packages[playerid] <= 0)
			    return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You are not carrying any packages!");

			Packages[playerid] = 0;
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);

			format(str, sizeof str, "* %s has dropped their packages.", PlayerICName(playerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		}

		else if(strmatch(params, "crates"))
		{
            if(Crates[playerid] <= 0)
			    return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You are not carrying any crates!");

			Crates[playerid] = 0;
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);

			format(str, sizeof str, "* %s has dropped their drug crates.", PlayerICName(playerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		}

		else if(strmatch(params, "tikis"))
		{
		    if(PlayerInfo[playerid][pTikis] == 0)
                return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You do not have any tikis!");

			PlayerInfo[playerid][pTikis] = 0;
			format(str, sizeof str, "* %s has thrown away their tikis.", PlayerICName(playerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}

		else if(strmatch(params, "seeds"))
		{
		    if(PlayerInfo[playerid][pPotSeeds] == 0)
                return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You do not have any pot seeds!");

			PlayerInfo[playerid][pPotSeeds] = 0;
			format(str, sizeof str, "* %s has dropped a bag of pot seeds.", PlayerICName(playerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}

		else if(strmatch(params, "meal"))
		{
		    if(!hasFoodTray[playerid])
                return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You are not carrying a food tray.");

			format(str, sizeof str, "* %s has thrown away their meal.", PlayerICName(playerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			throwAwayFoodTray(playerid);

			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}


		else if(strmatch(params, "boombox"))
		{
		    if(PlayerInfo[playerid][pStereo] == 0)
		        return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You do not have a boombox.");

			PlayerInfo[playerid][pStereo] = 0;
			format(str, sizeof str, "* %s has dropped their boombox.", PlayerICName(playerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}

		else if(strmatch(params, "briefcase"))
		{
			if(PlayerInfo[playerid][pSuitcase] == 0)
			    return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You do not have a boombox.");

 			PlayerInfo[playerid][pSuitcase] = 0;
            PlayerInfo[playerid][sMoney] = 0;
            PlayerInfo[playerid][sMats] = 0;
            PlayerInfo[playerid][sCrack] = 0;
            PlayerInfo[playerid][sPot] = 0;
            WearingSuitcase[playerid] = 0;
            RemovePlayerAttachedObject(playerid, 9);

            format(str, sizeof str, "* %s has dropped their briefcase.", PlayerICName(playerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}

		else if(strmatch(params, "cellphone"))
		{
		    if(PlayerInfo[playerid][pPnumber] == 0)
                return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You do not have a cellphone.");

			if(IsPlayerConnected(Mobile[playerid]))
			    return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You must hang up the current call before dropping your cellphone.");

			PhoneOffline[playerid] = 0;
			PlayerInfo[playerid][pPnumber] = 0;

			format(str, sizeof str, "* %s has dropped their cellphone.", PlayerICName(playerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}

		else if(strmatch(params, "products"))
		{
		    if(PlayerInfo[playerid][pProducts] == 0)
		        return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You do not have any products.");

			PlayerInfo[playerid][pProducts] = 0;
			format(str, sizeof str, "* %s has dropped all of their products.", PlayerICName(playerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}

		else if(strmatch(params, "screwdrivers"))
		{
		    if(PlayerInfo[playerid][pScrew] == 0)
		        return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You do not have any screwdrivers.");

			PlayerInfo[playerid][pScrew] = 0;
			format(str, sizeof str, "* %s has dropped all of their screwdrivers.", PlayerICName(playerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}

		else if(strmatch(params, "rope"))
		{
		    if(PlayerInfo[playerid][pRope] == 0)
		        return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You do not have any rope.");

			PlayerInfo[playerid][pRope] = 0;
			format(str, sizeof str, "* %s has dropped their rope.", PlayerICName(playerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}

		else if(strmatch(params, "jerrycan"))
		{
		    if(PlayerInfo[playerid][pJerrycan] == 0)
		        return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You do not have a jerrycan.");

			PlayerInfo[playerid][pJerrycan] = 0;
			format(str, sizeof str, "* %s has dropped their Jerrycan.", PlayerICName(playerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}

		else if(strmatch(params, "trash"))
		{
		    if(CP[playerid] != CHECKPOINT_GARBAGE)
		        return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You are not transporting any garbage.");

	        CP[playerid] = CHECKPOINT_NONE;
			DisablePlayerCheckpointEx(playerid);

		    format(str, sizeof(str), "* %s has thrown away their Trash.", PlayerICName(playerid));
            ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

            if(IsPlayerAttachedObjectSlotUsed(playerid, INDEX_8))
            {
                RemovePlayerAttachedObject(playerid, INDEX_8);
                SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
            }

            hasTruckCheckpoint[playerid] = -1;
            PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}

		else if(strmatch(params, "clips"))
		{
		    if(GetPlayerWeapon(playerid) == 0) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You are not holding a weapon.");
            if(g_pGunInfo[playerid][GetWeaponIndex(GetPlayerWeapon(playerid))][wo_Mags] == 0) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error: {FFFFFF}You do not have any clips for this weapon.");
			if(IsValidWeapon(playerid, GetPlayerWeapon(playerid)) != GUN_TYPE_NORMAL || IsACop(playerid) || IsAFireman(playerid) || IsASenate(playerid) || IsANG(playerid) || PlayerInfo[playerid][pMember] == 10)
		    {
                g_pGunInfo[playerid][GetWeaponIndex(GetPlayerWeapon(playerid))][wo_Mags] = 0;
                format(str, sizeof str, "* %s has dropped a bag of clips.", PlayerICName(playerid));
                ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
                PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
			}
			else
			{
			    new Float: x, Float: y, Float: z;
		        GetPlayerPos(playerid, x, y, z);

		        if(Item_SetData(Item_GetVacantSlot(), true, ITEMTYPE_CLIPS, g_pGunInfo[playerid][GetWeaponIndex(GetPlayerWeapon(playerid))][wo_Mags], GetPlayerWeapon(playerid), x, y, z, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), true, true) == 1)
				{
				    g_pGunInfo[playerid][GetWeaponIndex(GetPlayerWeapon(playerid))][wo_Mags] = 0;
				    format(str, sizeof str, "* %s has dropped a bag of clips(/get to pick it up).", PlayerICName(playerid));
				    ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				    Streamer_Update(playerid);
				    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				}
			}
		}
		else
		{
	    	SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /drop [name]");
			SendClientMessage(playerid, COLOR_GRAD1, "Available names: Weapon, Pot, Crack, Materials, Packages, Crates, Tikis, Seeds, Meal, Boombox");
			SendClientMessage(playerid, COLOR_GRAD1, "Available names: Briefcase, Cellphone, Products, Screwdrivers, Ropes, Jerrycan, Trash, Clips");
		}
	}
	return true;
}
