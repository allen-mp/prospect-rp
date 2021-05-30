/*
	@Author; Calvin Catt
	
	*** Developer Commands ***
	/createsafe [playerSQL] (will link a safe to a player's account)
	/deletesafe (will delete the safe you're beside)
	/safeconents (will get contents of the safe you're beside and display sqlID & arrayID)
	
	*** Admin Commands ***
	/safeconents (will get contents of the safe you're beside and display sqlID & arrayID)
	/safesystem (will allow admins to access the safe as the owner would - can be useful for removing items, etc.)
	
	*** Player Commands ***
	/safesystem [put/get] [mats/weapon/meth/cocaine/cannabis/cash] [amount/weapon slot]
	/safeconents (will get contents of the safe you're beside)
	
	*** Functions ***
	SaveSafe(safe);
	SaveSafes();
	LoadSafes();
*/


#define MAX_SAFES 			100
#define MAX_SAFE_WEAPONS	10

enum iSafeInfo
{
	owner,
	Float:posX,
	Float:posY,
	Float:posZ,
	virtualWorld,
	interior,
	cash,
	materials,
	meth,
	cocaine,
	cannabis,
	sqlID,
	pickupID
}
new itemSafeData[MAX_SAFES][iSafeInfo];
new weaponSafeData[MAX_SAFES][MAX_SAFE_WEAPONS];
new safescript[128];

stock SaveSafe(safe, print = true)
{
	if(print) printf("[SAFE] Saving itemSafeData[%d] linked to player SQL: %d", safe, itemSafeData[safe][owner]);
	if(itemSafeData[safe][sqlID] == 0) return 1; // We don't save unused safes...
	
	new query[512], weapQuery[256];
	
	for(new i = 0; i < MAX_SAFE_WEAPONS; i++)
	{
		mysql_format(sqlGameConnection, weapQuery, sizeof(weapQuery), "%s`weapon%d` = %d,", weapQuery, i, weaponSafeData[safe][i]);
	}
	
	mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `safes` SET `cash`= %d,`mats`= %d,`meth`= %d,`cocaine`= %d, %s `cannabis`= %d WHERE `safeID` = %d",
		itemSafeData[safe][cash], itemSafeData[safe][materials], itemSafeData[safe][meth], itemSafeData[safe][cocaine], weapQuery, itemSafeData[safe][cannabis], itemSafeData[safe][sqlID]);
	mysql_pquery(sqlGameConnection, query);
	printf(query);
	return 1;
}

stock SaveSafes()
{
	printf("[SAFE] Saving all safes.");
	for(new i = 0; i < MAX_SAFES; i++)
	{
		SaveSafe(i, false);
	}
	return 1;
}

stock LoadSafes()
{
	new query[128];
	mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `safes`");
	mysql_pquery(sqlGameConnection, query, "OnLoadSafes");
	return 1;
}

forward OnDeleteSafe(safe, playerid);
public OnDeleteSafe(safe, playerid)
{
	printf("[SAFE] itemSafeData[%d] has been deleted by %s.", safe, itemSafeData[safe][sqlID], PlayerName(playerid));
	
	DestroyDynamicPickup(itemSafeData[safe][pickupID]);
	itemSafeData[safe][posX] = 0.00;
	itemSafeData[safe][posY] = 0.00;
	itemSafeData[safe][posZ] = 0.00;
	itemSafeData[safe][interior] = 0;
	itemSafeData[safe][virtualWorld] = 0;
	itemSafeData[safe][owner] = 0;
	itemSafeData[safe][cash] = 0;
	itemSafeData[safe][materials] = 0;
	itemSafeData[safe][meth] = 0;
	itemSafeData[safe][cocaine] = 0;
	itemSafeData[safe][cannabis] = 0;
	itemSafeData[safe][sqlID] = 0;
	SendClientMessage(playerid, COLOR_GRAD1, "Successfully deleted the safe.");
	return 1;
}

forward OnLoadSafes();
public OnLoadSafes()
{
	new rows, fields;
	cache_get_data(rows, fields);
	
	if(rows)
	{
		new value[8];
		for(new safe = 0; safe < rows; safe++)
		{
			if(safe == MAX_SAFES - 1) return printf("Max safe limit reached.");
			itemSafeData[safe][sqlID] = cache_get_field_content_int(safe, "safeID");
			cache_get_field_content(safe, "posX", value);
			itemSafeData[safe][posX] = floatstr(value);
			cache_get_field_content(safe, "posY", value);
			itemSafeData[safe][posY] = floatstr(value);
			cache_get_field_content(safe, "posZ", value);
			itemSafeData[safe][posZ] = floatstr(value);
			itemSafeData[safe][interior] = cache_get_field_content_int(safe, "interior");
			itemSafeData[safe][virtualWorld] = cache_get_field_content_int(safe, "VW");
			itemSafeData[safe][owner] = cache_get_field_content_int(safe, "OwnerID");
			itemSafeData[safe][cash] = cache_get_field_content_int(safe, "cash");
			itemSafeData[safe][materials] = cache_get_field_content_int(safe, "mats");
			itemSafeData[safe][meth] = cache_get_field_content_int(safe, "meth");
			itemSafeData[safe][cocaine] = cache_get_field_content_int(safe, "cocaine");
			itemSafeData[safe][cannabis] = cache_get_field_content_int(safe, "cannabis");
			
			DestroyDynamicPickup(itemSafeData[safe][pickupID]);
			itemSafeData[safe][pickupID] = CreateDynamicPickup(1210, 23, itemSafeData[safe][posX], itemSafeData[safe][posY], itemSafeData[safe][posZ], itemSafeData[safe][virtualWorld], itemSafeData[safe][interior], -1, 100.0);		
			
			for(new i = 0; i < MAX_SAFE_WEAPONS; i++)
			{
				new weapstr[16];
				format(weapstr, sizeof(weapstr), "weapon%d", i);
				weaponSafeData[safe][i] = cache_get_field_content_int(safe, weapstr);
			}
		}
		printf("[SAFE] %d safes loaded successfully.", rows);
	}
	else printf("[SAFE] No safes to load.");
	return 1;
}

forward GetSafeSQL(safe);
public GetSafeSQL(safe) //get ID after insert query has complete
{

	new saveQuery[300];
	mysql_format(sqlGameConnection, saveQuery, sizeof(saveQuery), "SELECT `safeID` FROM `safes` ORDER BY `safeID` DESC LIMIT 1");
	mysql_pquery(sqlGameConnection, saveQuery, "SetSafeSQL", "i", safe);
	return 1;
}

forward SetSafeSQL(safe);
public SetSafeSQL(safe)
{
	itemSafeData[safe][sqlID] = cache_get_field_content_int(0, "safeID");
	printf("[SAFE] itemSafeData[%d] has been assigned sqlID %d.", safe, itemSafeData[safe][sqlID]);
	return 1;
}

CMD:createsafe(playerid, params[])
{
	if(PlayerInfo[playerid][pDev] >= 2)
	{
		new sqlid, unusedid = -1;
		if(sscanf(params, "i", sqlid)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /createsafe [sqlID]");
		new searchstr[64];
		for(new i = 0; i < MAX_SAFES; i++)
		{
			format(searchstr, sizeof(searchstr), "%0.2f,%0.2f,%0.2f", itemSafeData[i][posX], itemSafeData[i][posY],itemSafeData[i][posZ]);
			if(strcmp(searchstr, "0.00,0.00,0.00", true) == 0)
			{
				unusedid = i;
				break;
			}
		}
		
		if(unusedid == -1) return SendClientMessage(playerid, COLOR_GRAD1, "MAX_SAFES has been reached, unable to create another safe!");
		
		itemSafeData[unusedid][owner] = sqlid;
		GetPlayerPos(playerid, itemSafeData[unusedid][posX], itemSafeData[unusedid][posY], itemSafeData[unusedid][posZ]);
		itemSafeData[unusedid][interior] = GetPlayerInterior(playerid);
		itemSafeData[unusedid][virtualWorld] = GetPlayerVirtualWorld(playerid);
		
		DestroyDynamicPickup(itemSafeData[unusedid][pickupID]);
		itemSafeData[unusedid][pickupID] = CreateDynamicPickup(1210, 23, itemSafeData[unusedid][posX], itemSafeData[unusedid][posY], itemSafeData[unusedid][posZ], itemSafeData[unusedid][virtualWorld], itemSafeData[unusedid][interior], -1, 100.0);			
		
		new query[512];
		mysql_format( sqlGameConnection, query, sizeof(query), "INSERT INTO `safes` SET `OwnerID` = %d, `posX`= %f,`posY`= %f,`posZ`= %f,`VW`= %d,`interior`= %d",
			itemSafeData[unusedid][owner], itemSafeData[unusedid][posX], itemSafeData[unusedid][posY], itemSafeData[unusedid][posZ], itemSafeData[unusedid][virtualWorld], itemSafeData[unusedid][interior]);
		mysql_pquery( sqlGameConnection, query, "GetSafeSQL", "i", unusedid);
		printf(query);
		SaveSafe(unusedid);
		SendClientMessage(playerid, COLOR_GRAD1, "Successfully created a safe.");
		return 1;
	}
	return 1;
}

CMD:deletesafe(playerid, params[])
{
	new safe = -1;
	for(new i = 0; i < MAX_SAFES; i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 5.0, itemSafeData[i][posX], itemSafeData[i][posY], itemSafeData[i][posZ]))
		{
			if((GetPlayerVirtualWorld(playerid) == itemSafeData[i][virtualWorld]) && (GetPlayerInterior(playerid) == itemSafeData[i][interior]))
			{
				if(itemSafeData[i][owner] != 0) // Prevent deleting a blank one then trying to delete from SQL
				{
					safe = i;
				}
			}
		}
	}
	
	if(safe == -1) return SendClientMessage(playerid, COLOR_GRAD1, "You are not within range of a safe!");
	if(PlayerInfo[playerid][pDev] < 2) return AdmErrorMsg;
	
	new query[256];
	// Shouldn't need LIMIT 1 but we'll keep it in, just in case...
	mysql_format(sqlGameConnection, query, sizeof(query), "DELETE FROM `safes` WHERE `safeID`= %d LIMIT 1", itemSafeData[safe][sqlID]);
	mysql_pquery(sqlGameConnection, query, "OnDeleteSafe", "ii", safe, playerid);
	return 1;
}
	

CMD:safecontents(playerid, params[])
{
	new safe = -1;
	for(new i = 0; i < MAX_SAFES; i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 5.0, itemSafeData[i][posX], itemSafeData[i][posY], itemSafeData[i][posZ]))
		{
			if((GetPlayerVirtualWorld(playerid) == itemSafeData[i][virtualWorld]) && (GetPlayerInterior(playerid) == itemSafeData[i][interior]))
			{
				safe = i;
			}
		}
	}
	
	if(safe == -1) return SendClientMessage(playerid, COLOR_GRAD1, "You are not within range of a safe!");
	// We'll let admins use this to view safes
	if( ((itemSafeData[safe][owner] != PlayerInfo[playerid][pID]) && (PlayerInfo[playerid][pAdmin] < 4) && (PlayerInfo[playerid][pDev] < 2)) ) return SendClientMessage(playerid, COLOR_GREY, "You do not have access to that safe!");
	
	new clientMsg[512], titlestr[32];
	format(titlestr, sizeof(titlestr), "** SAFE CONTENTS **");
	format(clientMsg, sizeof(clientMsg), "{007BD0}Materials:{FFFFFF} %d\n", itemSafeData[safe][materials]);
	format(clientMsg, sizeof(clientMsg), "%s{007BD0}Cannabis:{FFFFFF} %d\n", clientMsg, itemSafeData[safe][cannabis]);
	format(clientMsg, sizeof(clientMsg), "%s{007BD0}Cocaine:{FFFFFF} %d\n", clientMsg, itemSafeData[safe][cocaine]);
	format(clientMsg, sizeof(clientMsg), "%s{007BD0}Meth:{FFFFFF} %d\n", clientMsg, itemSafeData[safe][meth]);
	
	new weapstr[256];
	for(new i = 0; i < MAX_SAFE_WEAPONS; i++)
	{
		new WeaponName[32];
		GetWeaponName(weaponSafeData[safe][i], WeaponName, 32);
		if(i == 0) format(weapstr, sizeof(weapstr), "{007BD0}Weapon %d: %s\n", i, WeaponName);
		else format(weapstr, sizeof(weapstr), "%s{007BD0}Weapon %d: %s\n", weapstr, i, WeaponName);
	}
	format(clientMsg, sizeof(clientMsg), "%s{007BD0}Cash:{FFFFFF} %d\n%s", clientMsg, itemSafeData[safe][cash], weapstr); // Add safe + weapon contents strings together
	
	if(PlayerInfo[playerid][pAdmin] > 3 || PlayerInfo[playerid][pDev] >= 1) {
		format(clientMsg, sizeof(clientMsg), "%s\n** %d'S SAFE CONTENTS (sqlID: %d | arrayID: %d) **", clientMsg, itemSafeData[safe][owner], itemSafeData[safe][sqlID], safe);
	}
	
	ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, titlestr, clientMsg, "Okay", "");
	return 1;
}


CMD:safesystem(playerid, params[]) // cmd name will be changed soon
{
	new safe = -1;
	for(new i = 0; i < MAX_SAFES; i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 5.0, itemSafeData[i][posX], itemSafeData[i][posY], itemSafeData[i][posZ]))
		{
			if((GetPlayerVirtualWorld(playerid) == itemSafeData[i][virtualWorld]) && (GetPlayerInterior(playerid) == itemSafeData[i][interior]))
			{
				safe = i;
			}
		}
	}
	
	if(safe == -1) return SendClientMessage(playerid, COLOR_GRAD1, "You are not within range of a safe!");
	// We'll let admins use the safes to remove items etc.
	if( (itemSafeData[safe][owner] != PlayerInfo[playerid][pID]) && (PlayerInfo[playerid][pAdmin] < 4) ) return SendClientMessage(playerid, COLOR_GREY, "You do not have access to that safe!");
	new sec[24], thing[24], amount[24], str[128];
	if(sscanf(params, "s[24]s[24]s[24]", sec, thing, amount)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /safe [put/get] [mats/weapon/meth/cocaine/cannabis/cash] [amount/weapon slot]");
	{
		if(strcmp(sec, "put", true) == 0)
		{
			// put mats
			if(strcmp(thing, "mats", true) == 0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(PlayerInfo[playerid][pMats] < strval(amount)) return SendClientMessage(playerid, COLOR_GREY, "You don't have that much materials.");
				if(strval(amount) < 1 || strval(amount) > 50000) return SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 50000.");
				if(itemSafeData[safe][materials] + strval(amount) > 100000) return SendClientMessage(playerid, COLOR_GREY, "The safe can't hold more than 100,000 materials.");
				PlayerInfo[playerid][pMats] -= strval(amount);
				itemSafeData[safe][materials] += strval(amount);
				SaveSafe(safe);
				format(str, sizeof(str), "* %s deposits materials into their safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				printf("%s has %s %s %s", PlayerName(playerid), sec, amount, thing); // log what the player is doing
				return 1;
			}
			// put cannabis
			if(strcmp(thing, "cannabis", true) == 0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(PlayerInfo[playerid][pCannabis] < strval(amount)) return SendClientMessage(playerid, COLOR_GREY, "You don't have that much cannabis.");
				if(strval(amount) < 1 || strval(amount) > 1000) return SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 1000.");
				if(itemSafeData[safe][cannabis] + strval(amount) > 1000) return SendClientMessage(playerid, COLOR_GREY, "The safe can't hold more than 1000 cannabis.");
				PlayerInfo[playerid][pCannabis] -= strval(amount);
				itemSafeData[safe][cannabis] += strval(amount);
				SaveSafe(safe);
				format(str, sizeof(str), "* %s deposits some pot into a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				printf("%s has %s %s %s", PlayerName(playerid), sec, amount, thing); // log what the player is doing
				return 1;
			}
			// put meth
			if(strcmp(thing, "meth", true) == 0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(PlayerInfo[playerid][pMeth] < strval(amount)) return SendClientMessage(playerid, COLOR_GREY, "You don't have that much meth.");
				if(strval(amount) < 1 || strval(amount) > 1000) return SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 1000.");
				if(itemSafeData[safe][meth] + strval(amount) > 1000) return SendClientMessage(playerid, COLOR_GREY, "The safe can't hold more than 1000 meth.");
				PlayerInfo[playerid][pMeth] -= strval(amount);
				itemSafeData[safe][meth] += strval(amount);
				SaveSafe(safe);
				format(str, sizeof(str), "* %s deposits some pot into a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				printf("%s has %s %s %s", PlayerName(playerid), sec, amount, thing); // log what the player is doing
				return 1;
			}
			// put cocaine
			if(strcmp(thing, "cocaine", true) == 0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(PlayerInfo[playerid][pCocaine] < strval(amount)) return SendClientMessage(playerid, COLOR_GREY, "You don't have that much cocaine.");
				if(strval(amount) < 1 || strval(amount) > 1000) return SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 1000.");
				if(itemSafeData[safe][cocaine] + strval(amount) > 1000) return SendClientMessage(playerid, COLOR_GREY, "The safe can't hold more than 1000 cocaine.");
				PlayerInfo[playerid][pCocaine] -= strval(amount);
				itemSafeData[safe][cocaine] += strval(amount);
				SaveSafe(safe);
				format(str, sizeof(str), "* %s deposits some cocaine into a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				printf("%s has %s %s %s", PlayerName(playerid), sec, amount, thing); // log what the player is doing
				return 1;
			}
			if(strcmp(thing, "weapon", true) == 0)
			{
				
				if(strval(amount) < 0 || strval(amount) > 9) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /safe put weapon [0-9]");
		        if(weaponSafeData[safe][strval(amount)] != 0) return SendClientMessage(playerid, COLOR_GREY, "You already have a weapon stored in that slot.");
		        if(GetPlayerWeapon(playerid) == 0) return SendClientMessage(playerid, COLOR_GREY, "You are not holding a weapon.");
		        if(HaveAdminWeapon(playerid, GetPlayerWeapon(playerid))  == GetPlayerWeapon(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can not store admin weapons.");

				new weaponName[64];
				GetWeaponName(GetPlayerWeapon(playerid), weaponName, sizeof(weaponName));
		        TakePlayerWeapon(playerid, GetPlayerWeapon(playerid));
				weaponSafeData[safe][strval(amount)] = GetPlayerWeapon(playerid);
    			format(safescript, sizeof safescript, "* %s places a %s in the safe.", PlayerICName(playerid), weaponName);
				ProxDetector(10.0, playerid, safescript, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				printf("%s has %s a %s in slot %s", PlayerName(playerid), sec, weaponName, amount); // log what the player is doing
				return 1;
			}
		}
		if(strcmp(sec, "get", true) == 0)
		{
			if(strcmp(thing, "mats", true) == 0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(strval(amount) > itemSafeData[safe][materials]) return SendClientMessage(playerid, COLOR_GREY, "The safe doesn't have that much materials.");
				if(strval(amount) < 1 || strval(amount) > 50000) return SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 50000.");
				PlayerInfo[playerid][pMats] += strval(amount);
				itemSafeData[safe][materials] -= strval(amount);
				SaveSafe(safe);
				format(str, sizeof(str), "* %s takes some materials from a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				printf("%s has %s %s %s", PlayerName(playerid), sec, amount, thing); // log what the player is doing
				return 1;
			}
			// fget pot
			if(strcmp(thing, "cannabis", true) == 0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(strval(amount) > itemSafeData[safe][cannabis]) return SendClientMessage(playerid, COLOR_GREY, "Family safe doesn't have that much cannabis.");
				if(strval(amount) < 1 || strval(amount) > 50) return SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 50.");
				if(strval(amount) + PlayerInfo[playerid][pCannabis] > 50) return SendClientMessage(playerid, COLOR_GREY, "You can only hold a max of 50 cannabis.");
				PlayerInfo[playerid][pCannabis] += strval(amount);
				itemSafeData[safe][cannabis] -= strval(amount);
				SaveSafe(safe);
				format(str, sizeof(str), "* %s takes some pot from a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				printf("%s has %s %s %s", PlayerName(playerid), sec, amount, thing); // log what the player is doing
				return 1;
			}
			// fget crack
			if(strcmp(thing, "cocaine", true) == 0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(strval(amount) > itemSafeData[safe][cocaine]) return SendClientMessage(playerid, COLOR_GREY, "Family safe doesn't have that much cocaine.");
				if(strval(amount) < 1 || strval(amount) > 25)SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 25.");
				if(strval(amount) + PlayerInfo[playerid][pCocaine] > 25) return SendClientMessage(playerid, COLOR_GREY, "You can only hold a max of 25 cocaine.");
				PlayerInfo[playerid][pCocaine] += strval(amount);
				itemSafeData[safe][cocaine] -= strval(amount);
				SaveSafe(safe);
				format(str, sizeof(str), "* %s takes some crack from a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				printf("%s has %s %s %s", PlayerName(playerid), sec, amount, thing); // log what the player is doing
				return 1;
			}
			if(strcmp(thing, "meth", true) == 0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(strval(amount) > itemSafeData[safe][meth]) return SendClientMessage(playerid, COLOR_GREY, "Family safe doesn't have that much meth.");
				if(strval(amount) < 1 || strval(amount) > 25)SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 25.");
				if(strval(amount) + PlayerInfo[playerid][pMeth] > 25) return SendClientMessage(playerid, COLOR_GREY, "You can only hold a max of 25 meth.");
				PlayerInfo[playerid][pMeth] += strval(amount);
				itemSafeData[safe][meth] -= strval(amount);
				SaveSafe(safe);
				format(str, sizeof(str), "* %s takes some crack from a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				printf("%s has %s %s %s", PlayerName(playerid), sec, amount, thing); // log what the player is doing
				return 1;
			}
			if(strcmp(thing, "weapon", true) == 0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The slot must be a number.");
				if(strval(amount) < 0 || strval(amount) > 9) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /safe get weapon [0-9]");
			    if(PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GREY, "You must play for 2 hours before using a weapon.");
			    if(weaponSafeData[safe][strval(amount)] == 0) return SendClientMessage(playerid, COLOR_GREY, "You do not have a weapon stored in the safe.");
				new WeaponName[32];
				GetWeaponName(weaponSafeData[safe][strval(amount)], WeaponName, 32);
                format(safescript, sizeof safescript, "* %s takes a %s from the safe.", PlayerICName(playerid), WeaponName);
				ProxDetector(10.0, playerid, safescript, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				
				GivePlayerGun(playerid, weaponSafeData[safe][strval(amount)]);
				weaponSafeData[safe][strval(amount)] = 0;
				printf("%s has taken a %s from slot %d", PlayerName(playerid), WeaponName, strval(amount)); // log what the player is doing
			}
		}
		return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /safe [put/get] [mats/weapon/meth/cocaine/cannabis/cash]");
	}
}

