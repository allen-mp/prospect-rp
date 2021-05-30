/*

	Beginning the organization of families


	NOTE: I want SQL saving when data changes since family data won't change frequently...
	Likewise with family items: we will load the item that is required when it is required and
	update when there is a new item removed or added.
	


	THINGS PENDING (add more here):
		1. Leveling system for families -- how will they gain 'XP' to level?

	THINGS IN PROGRESS:
		Octet: Inventories


	This file should only contain saving, giving of strikes, or anything that may alter or fetchh fiSID, fiName, fiOwner, fiStrikes, fiLevel
	or anything else that should belong in FamilyInfo that is considered a CORE component to families.
	Example functions may include GiveFamilyStrike(fid), RemoveFamilyStrike(fid), AddFamily(fname, fowner), RemoveFamily(fid)
*/

//	Definitions
#define MAX_FAMILIES 12
#define MAX_FAMILY_SKINS	6
#define MAX_FAMILY_NAME		30

enum FamiliesInfo {
	fiSID,
	bool:fiTaken,
	fiName[MAX_FAMILY_NAME+1],
	fiOwner,
	fiMotd,
	fiStrikes,
	fiLevel,
	fiSkins[MAX_FAMILY_SKINS+1]
}

new Families[MAX_FAMILIES][FamiliesInfo];

/*
	Function prototypes
*/
//public
forward FamilyLog(text[]);
forward CreateFamily(ownerid, name[]);


//protected
forward OnLoadFamily();

//inc/inventories.inc
//forward FetchFamilyInventory(fid);

/*
	Includes/Extensions
*/
//#include "inc/families/inc/inventories.inc"

/*
	Logging
*/
static GetTimeString() {
    new string[10];
    new h, m, s;
    gettime(h, m, s);
    h = FixHour(h);
    format(string, sizeof string, "%d:%d:%d", h, m, s);
    return string;
}

public FamilyLog(text[]) {
	new File:logfile;
	logfile = fopen("../logs/FamilyLog.log", io_append);

	new string[144];
	
	format(string, sizeof string, "<FLOG> %s: %s", GetTimeString(), text);
	fwrite(logfile, string);
	fclose(logfile);
	return 1;
}
/*	
	Loading
*/
Hook:Families_OnGameModeInit() {
	RequestFamilyInfo();
	return 1;
}

//Note, there's no saving function since data is saved when it's modified.
RequestFamilyInfo() {
	inline LoadFamilyInfo() {
		if(cache_num_rows() < 1) {
			FamilyLog("There are no families in the database.");
			return;
		}
		
		for(new i; i < cache_num_rows(); i++) {
			// % Loop invariant: for all rows till i, the data has been imported into an array, Families
			
			if(i == MAX_FAMILIES) {
				FamilyLog("max families (12) has been reached.");
				break;
			}
			
			Families[i][fiSID] = cache_get_field_content_int(i, "ID");
			cache_get_field_content(i, "Name", Families[i][fiName]);
			Families[i][fiOwner] = cache_get_field_content_int(i, "Owner");
			cache_get_field_content(i, "Motd", Families[i][fiMotd]);
			Families[i][fiStrikes] = cache_get_field_content_int(i, "Strikes");
			Families[i][fiLevel] = cache_get_field_content_int(i, "Level");
			Families[i][fiTaken] = true;
		}
	}
	mysql_pquery_inline(sqlGameConnection, "SELECT * FROM Families", using inline LoadFamilyInfo, "");
	return;
}


public CreateFamily(ownerid, name[]) {
	//get free slot
	new slot = MAX_FAMILIES;
	for(new i; i < MAX_FAMILIES; i++) {
		if(!Families[i][fiTaken]) {
			slot = i;
			break;
		}
	}

	//max families?
	if(slot == MAX_FAMILIES) {
		return false;
	}

	Families[slot][fiOwner] = PlayerInfo[ownerid][pID];
	format(Families[slot][fiName], MAX_FAMILY_NAME, "%s", name);
	Families[slot][fiTaken] = true;

	new query[200];
	mysql_format(sqlGameConnection, query, sizeof query, "INSERT INTO Families SET Owner = %d, Name = '%e'", Families[slot][fiOwner], name);
	mysql_pquery(sqlGameConnection, query);
	return true;
}
/*
	CORE COMMANDS: only important commands here

*/
CMD:createfamily(playerid, params[]) {
	//get params
	new name[MAX_FAMILY_NAME+1], owner;
	if(sscanf(params, "us[MAX_FAMILY_NAME+1]", owner, name)) {
		return SendClientMessage(playerid, 0, "{449b41}Usage:{FFFFFF}/createfamily [partOfName/playerID] [familyName]");
	}

	if(!IsPlayerConnected(owner)) {
		return SendClientMessage(playerid, COLOR_LIGHTRED, "Error: That player is not connected.");
	}

	if(CreateFamily(owner, name)) {
		return SendClientMessage(playerid, COLOR_LIGHTGREEN, "Success: The family has been created.");
	}
	SendClientMessage(playerid, COLOR_LIGHTRED, "Error: I couldn't create the family. Are there free slots?");
	return 1;
}


