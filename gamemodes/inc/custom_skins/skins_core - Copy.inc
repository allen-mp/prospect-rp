/*	        
									 /$$$$       /$$$$$$$$  /$$$$$$        /$$$$$$$  /$$$$$$$        /$$$$
									| $$_/      | $$_____/ /$$__  $$      | $$__  $$| $$__  $$      |_  $$
									| $$        | $$      | $$  \__/      | $$  \ $$| $$  \ $$        | $$
									| $$        | $$$$$   | $$            | $$$$$$$/| $$$$$$$/        | $$
									| $$        | $$__/   | $$            | $$__  $$| $$____/         | $$
									| $$        | $$      | $$    $$      | $$  \ $$| $$              | $$
									| $$$$      | $$$$$$$$|  $$$$$$/      | $$  | $$| $$             /$$$$
									|____/      |________/ \______/       |__/  |__/|__/            |____/

									 /$$$$$$$$ /$$ /$$   /$$                      /$$$$$$  /$$   /$$              
									| $$_____/| $$|__/  | $$                     /$$__  $$|__/  | $$              
									| $$      | $$ /$$ /$$$$$$    /$$$$$$       | $$  \__/ /$$ /$$$$$$   /$$   /$$
									| $$$$$   | $$| $$|_  $$_/   /$$__  $$      | $$      | $$|_  $$_/  | $$  | $$
									| $$__/   | $$| $$  | $$    | $$$$$$$$      | $$      | $$  | $$    | $$  | $$
									| $$      | $$| $$  | $$ /$$| $$_____/      | $$    $$| $$  | $$ /$$| $$  | $$
									| $$$$$$$$| $$| $$  |  $$$$/|  $$$$$$$      |  $$$$$$/| $$  |  $$$$/|  $$$$$$$
									|________/|__/|__/   \___/   \_______/       \______/ |__/   \___/   \____  $$
									                                                                     /$$  | $$
									                                                                    |  $$$$$$/
									                                                                     \______/ 
									 /$$$$$$$            /$$                 /$$$$$$$  /$$                    
									| $$__  $$          | $$                | $$__  $$| $$                    
									| $$  \ $$  /$$$$$$ | $$  /$$$$$$       | $$  \ $$| $$  /$$$$$$  /$$   /$$
									| $$$$$$$/ /$$__  $$| $$ /$$__  $$      | $$$$$$$/| $$ |____  $$| $$  | $$
									| $$__  $$| $$  \ $$| $$| $$$$$$$$      | $$____/ | $$  /$$$$$$$| $$  | $$
									| $$  \ $$| $$  | $$| $$| $$_____/      | $$      | $$ /$$__  $$| $$  | $$
									| $$  | $$|  $$$$$$/| $$|  $$$$$$$      | $$      | $$|  $$$$$$$|  $$$$$$$
									|__/  |__/ \______/ |__/ \_______/      |__/      |__/ \_______/ \____  $$
									                                                                 /$$  | $$
									                                                                |  $$$$$$/
									                                                                 \______/ 	

						 /$$   /$$                  /$$$$$$                                      /$$$$$$$  /$$           /$$         /$$    
						| $$$ | $$                 /$$__  $$                                    | $$__  $$|__/          | $$        | $$    
						| $$$$| $$  /$$$$$$       | $$  \__/  /$$$$$$   /$$$$$$  /$$   /$$      | $$  \ $$ /$$  /$$$$$$ | $$$$$$$  /$$$$$$  
						| $$ $$ $$ /$$__  $$      | $$       /$$__  $$ /$$__  $$| $$  | $$      | $$$$$$$/| $$ /$$__  $$| $$__  $$|_  $$_/  
						| $$  $$$$| $$  \ $$      | $$      | $$  \ $$| $$  \ $$| $$  | $$      | $$__  $$| $$| $$  \ $$| $$  \ $$  | $$    
						| $$\  $$$| $$  | $$      | $$    $$| $$  | $$| $$  | $$| $$  | $$      | $$  \ $$| $$| $$  | $$| $$  | $$  | $$ /$$
						| $$ \  $$|  $$$$$$/      |  $$$$$$/|  $$$$$$/| $$$$$$$/|  $$$$$$$      | $$  | $$| $$|  $$$$$$$| $$  | $$  |  $$$$/
						|__/  \__/ \______/        \______/  \______/ | $$____/  \____  $$      |__/  |__/|__/ \____  $$|__/  |__/   \___/  
						                                              | $$       /$$  | $$                     /$$  \ $$                    
						                                              | $$      |  $$$$$$/                    |  $$$$$$/                    
						                                              |__/       \______/                      \______/                                     							  							
*/
/*

	file:			customs_skins/core.inc

	description:	The custom skin registration system.

	author:			Jay Cortez

	date created:	28th March 2018

*/

/*

	NOTE TO THE PERSON WHO MAKES ANY CHANGE IN THIS FILE.
	If you add or remove a RegisterCustomSkin(); Please make sure to keep note of the next free ID.

	In-between Free IDs: 20021, 20022, 20023
	Next free ID in series: 20068 >

*/

#include "inc/custom_skins/array.inc"

#define SKIN_SHOP_MENU			20
#define SKIN_SHOP_PRICE			500

// Types of custom skins
enum {
	CUSTOM_SKIN_TYPE_PUBLIC,
	CUSTOM_SKIN_TYPE_FACTION,
	CUSTOM_SKIN_TYPE_GROUP,
	CUSTOM_SKIN_TYPE_DONOR
}

/*
	Register all skins you add to the server here.
	Please keep this as organized as possible.

	Examples:
	> RegisterCustomSkin(20001, CUSTOM_SKIN_TYPE_PUBLIC);
	// Registers skin ID 20001 to the public

	> RegisterCustomSkin(20080, CUSTOM_SKIN_TYPE_FACTION, FACTION_FMD);
	// Registers skin ID 20080 to the SAFMD faction (all ranks can use it)

	> RegisterCustomSkin(20003, CUSTOM_SKIN_TYPE_FACTION, FACTION_SAPD, 3);
	// Registers skin ID 20003 to the SAPD faction (rank 3 and above can use it)

	> RegisterCustomSkin(20006, CUSTOM_SKIN_TYPE_GROUP, GROUP_HITMAN);
	// Registers skin ID 20006 to the Hitman group

	> RegisterCustomSkin(20009, CUSTOM_SKIN_TYPE_GROUP, GROUP_CRIME, 2);
	// Registers skin ID 20009 to the crime group (rank 2 and above can use it)

	> RegisterCustomSkin(20011, CUSTOM_SKIN_TYPE_DONOR, 7379);
	// Registers skin ID 20011 to the player with SQL ID 7379
*/
/* 
20063 unused
65 70 71 72 too
*/

Hook:skins_OnGameModeInit() {
	// Public skins
	RegisterCustomSkin(20001, CUSTOM_SKIN_TYPE_PUBLIC);
	RegisterCustomSkin(20002, CUSTOM_SKIN_TYPE_PUBLIC);
	RegisterCustomSkin(20003, CUSTOM_SKIN_TYPE_PUBLIC);
	RegisterCustomSkin(20004, CUSTOM_SKIN_TYPE_PUBLIC);
	RegisterCustomSkin(20005, CUSTOM_SKIN_TYPE_PUBLIC);
	RegisterCustomSkin(20006, CUSTOM_SKIN_TYPE_PUBLIC);
	RegisterCustomSkin(20007, CUSTOM_SKIN_TYPE_PUBLIC);
	RegisterCustomSkin(20008, CUSTOM_SKIN_TYPE_PUBLIC);
	RegisterCustomSkin(20093, CUSTOM_SKIN_TYPE_PUBLIC); 
	RegisterCustomSkin(20103, CUSTOM_SKIN_TYPE_PUBLIC);
	RegisterCustomSkin(20104, CUSTOM_SKIN_TYPE_PUBLIC);
	RegisterCustomSkin(20105, CUSTOM_SKIN_TYPE_PUBLIC);
	RegisterCustomSkin(20106, CUSTOM_SKIN_TYPE_PUBLIC);
	RegisterCustomSkin(20107, CUSTOM_SKIN_TYPE_PUBLIC);
	RegisterCustomSkin(20108, CUSTOM_SKIN_TYPE_PUBLIC);
	RegisterCustomSkin(20109, CUSTOM_SKIN_TYPE_PUBLIC);

	// SAFMD skins
	RegisterCustomSkin(20009, CUSTOM_SKIN_TYPE_FACTION, FACTION_FMD);
	RegisterCustomSkin(20010, CUSTOM_SKIN_TYPE_FACTION, FACTION_FMD);
	RegisterCustomSkin(20011, CUSTOM_SKIN_TYPE_FACTION, FACTION_FMD);
	RegisterCustomSkin(20097, CUSTOM_SKIN_TYPE_FACTION, FACTION_FMD);

	// SAPD skins
	RegisterCustomSkin(20012, CUSTOM_SKIN_TYPE_FACTION, FACTION_SAPD);
	RegisterCustomSkin(20013, CUSTOM_SKIN_TYPE_FACTION, FACTION_SAPD);
	RegisterCustomSkin(20014, CUSTOM_SKIN_TYPE_FACTION, FACTION_SAPD);
	RegisterCustomSkin(20015, CUSTOM_SKIN_TYPE_FACTION, FACTION_SAPD);
	RegisterCustomSkin(20016, CUSTOM_SKIN_TYPE_FACTION, FACTION_SAPD);
	RegisterCustomSkin(20017, CUSTOM_SKIN_TYPE_FACTION, FACTION_SAPD);
	//RegisterCustomSkin(20063, CUSTOM_SKIN_TYPE_FACTION, FACTION_SAPD);


	// FBI skins
	RegisterCustomSkin(20018, CUSTOM_SKIN_TYPE_FACTION, FACTION_FBI);
	RegisterCustomSkin(20044, CUSTOM_SKIN_TYPE_FACTION, FACTION_FBI);
	RegisterCustomSkin(20045, CUSTOM_SKIN_TYPE_FACTION, FACTION_FBI);
	RegisterCustomSkin(20046, CUSTOM_SKIN_TYPE_FACTION, FACTION_FBI);
	RegisterCustomSkin(20088, CUSTOM_SKIN_TYPE_FACTION, FACTION_FBI);
	
	// CJNG Skins
	RegisterCustomSkin(20098, CUSTOM_SKIN_TYPE_GROUP, GROUP_CARTEL);

	// Tijuana Cartel group skins (donated)
	/* Female skin - http://www.gta-sarp.com/forums/showthread.php?296001 */
	RegisterCustomSkin(20020, CUSTOM_SKIN_TYPE_GROUP, GROUP_TIJUANA);
	/* Tijuana Cartel Group Skin - http://www.gta-sarp.com/forums/showthread.php?292157-Game-Related-Juan_Ordonez-s-Request */
	RegisterCustomSkin(20061, CUSTOM_SKIN_TYPE_GROUP, GROUP_TIJUANA);

	/* Caesar Bruetti - http://www.gta-sarp.com/forums/showthread.php?282690 */
	RegisterCustomSkin(20024, CUSTOM_SKIN_TYPE_DONOR, 170751); // His player account
	RegisterCustomSkin(20024, CUSTOM_SKIN_TYPE_DONOR, 186548); // His admin account

	/* Chris Ta - http://www.gta-sarp.com/forums/showthread.php?289574 */
	RegisterCustomSkin(20025, CUSTOM_SKIN_TYPE_DONOR, 13843); 

	/* Chris Ta - http://www.gta-sarp.com/forums/showthread.php?288630 */
	RegisterCustomSkin(20026, CUSTOM_SKIN_TYPE_DONOR, 8075); 

	/* Roscoe Jarvis - Got it for being an official modder */
	RegisterCustomSkin(20027, CUSTOM_SKIN_TYPE_DONOR, 60274); 

	/* Casper Holstein - http://www.gta-sarp.com/forums/showthread.php?290760 */
	RegisterCustomSkin(20028, CUSTOM_SKIN_TYPE_DONOR, 19791); 

	/* Jimmy Ta - http://www.gta-sarp.com/forums/showthread.php?292129 */
	RegisterCustomSkin(20029, CUSTOM_SKIN_TYPE_DONOR, 312); 

	/* Carlos Alcazar - http://www.gta-sarp.com/forums/showthread.php?292069 */
	RegisterCustomSkin(20030, CUSTOM_SKIN_TYPE_DONOR, 82903); 

	/* Mary Oswald's skin */
	RegisterCustomSkin(20031, CUSTOM_SKIN_TYPE_DONOR, 7381); 
	RegisterCustomSkin(20096, CUSTOM_SKIN_TYPE_DONOR, 7381);

	/* Chris Sanchez - http://www.gta-sarp.com/forums/showthread.php?292281 */
	RegisterCustomSkin(20032, CUSTOM_SKIN_TYPE_DONOR, 181598); 

	/* Dixie Banton - http://www.gta-sarp.com/forums/showthread.php?292458 */
	RegisterCustomSkin(20033, CUSTOM_SKIN_TYPE_DONOR, 5920); 

	/* Rodrigo Sosa - http://www.gta-sarp.com/forums/showthread.php?293673 */
	RegisterCustomSkin(20035, CUSTOM_SKIN_TYPE_DONOR, 185225);

	/* Armando Sosa - http://www.gta-sarp.com/forums/showthread.php?293770 */
	RegisterCustomSkin(20036, CUSTOM_SKIN_TYPE_DONOR, 8075);

	/* Tatyana Nowak - http://www.gta-sarp.com/forums/showthread.php?292834-Development-Related-Tatyana_Nowak-s-Request */
	RegisterCustomSkin(20037, CUSTOM_SKIN_TYPE_DONOR, 191172);

	/* Donald Crooks - http://www.gta-sarp.com/forums/showthread.php?293922 */
	RegisterCustomSkin(20038, CUSTOM_SKIN_TYPE_DONOR, 475);

	/* Claude West - http://www.gta-sarp.com/forums/showthread.php?294789 */
	RegisterCustomSkin(20039, CUSTOM_SKIN_TYPE_DONOR, 178970);

	/* Reece Sawano - http://www.gta-sarp.com/forums/showthread.php?295519 */
	RegisterCustomSkin(20040, CUSTOM_SKIN_TYPE_DONOR, 92920);

	/* Sean Johnson - http://www.gta-sarp.com/forums/showthread.php?295495 */
	RegisterCustomSkin(20041, CUSTOM_SKIN_TYPE_DONOR, 859);

	/* Chris Ta - http://www.gta-sarp.com/forums/showthread.php?295072 */
	RegisterCustomSkin(20042, CUSTOM_SKIN_TYPE_DONOR, 13843);

	/* Aaron Steel - (Will add link)*/
	RegisterCustomSkin(20043, CUSTOM_SKIN_TYPE_DONOR, 152639);
	/* Pooya - Given for all his hard work */
	RegisterCustomSkin(20047, CUSTOM_SKIN_TYPE_DONOR, 140405);
	/* Skarr Torino - http://www.gta-sarp.com/forums/showthread.php?294594 */
	RegisterCustomSkin(20048, CUSTOM_SKIN_TYPE_DONOR, 1118);
	/* Jerrelle_Mackenzie - http://www.gta-sarp.com/forums/showthread.php?295611-Game-Related-Jerrelle_Mackenzie-s-Request */
	RegisterCustomSkin(20049, CUSTOM_SKIN_TYPE_DONOR, 26660);
	/* Jesus_Salonga - http://www.gta-sarp.com/forums/showthread.php?295391-Game-Related-Jesus_Salonga-s-Request */
	RegisterCustomSkin(20050, CUSTOM_SKIN_TYPE_DONOR, 174017);
	/* Aaron_Steel - http://www.gta-sarp.com/forums/showthread.php?295652-Game-Related-Aaron_Steel-s-Request */
	RegisterCustomSkin(20051, CUSTOM_SKIN_TYPE_DONOR, 152639);
	/* Aaron Steel - http://www.gta-sarp.com/forums/showthread.php?294598 */
	RegisterCustomSkin(20052, CUSTOM_SKIN_TYPE_DONOR, 152639);
	/* Chris Ta - http://www.gta-sarp.com/forums/showthread.php?293378-Game-Related-Chris_Ta-s-Request */
	RegisterCustomSkin(20053, CUSTOM_SKIN_TYPE_DONOR, 13843);
	/* Chris Ta - http://www.gta-sarp.com/forums/showthread.php?293378-Game-Related-Chris_Ta-s-Request */
	RegisterCustomSkin(20054, CUSTOM_SKIN_TYPE_DONOR, 13843);
	/* Aaron Steel - http://www.gta-sarp.com/forums/showthread.php?294598 */
	RegisterCustomSkin(20055, CUSTOM_SKIN_TYPE_DONOR, 152639);
	/* Zherdev OPG (Rebels MC) - http://www.gta-sarp.com/forums/showthread.php?295626-Game-Related-Nikolai_Borodin-s-Request */
	RegisterCustomSkin(20056, CUSTOM_SKIN_TYPE_GROUP, GROUP_CARTEL);
	/* Thomas Teressi - http://www.gta-sarp.com/forums/showthread.php?292422-Game-Related-Thomas_Teressi-s-Request */
	RegisterCustomSkin(20057, CUSTOM_SKIN_TYPE_DONOR, 127908);
	/* Chris Ta - http://www.gta-sarp.com/forums/showthread.php?295961-Game-Related-Chris_Ta-s-Request */
	RegisterCustomSkin(20058, CUSTOM_SKIN_TYPE_DONOR, 13843);
	/* Tatyana_Nowak - http://www.gta-sarp.com/forums/showthread.php?292913-Shop-Related-Tatyana_Nowak-s-Request */
	RegisterCustomSkin(20059, CUSTOM_SKIN_TYPE_DONOR, 191172);
	RegisterCustomSkin(20060, CUSTOM_SKIN_TYPE_DONOR, 191172);
	RegisterCustomSkin(20062, CUSTOM_SKIN_TYPE_GROUP, GROUP_TIJUANA);
	//http://www.gta-sarp.com/forums/showthread.php?299095-Development-Related-Casper_Holstein-s-Request
	RegisterCustomSkin(20064, CUSTOM_SKIN_TYPE_DONOR, 19791);
	RegisterCustomSkin(20065, CUSTOM_SKIN_TYPE_DONOR, 19791);

	/* S9 Group skin - http://www.gta-sarp.com/forums/showthread.php?298172-Development-Related-Lance_Rizzi-s-Request */
	RegisterCustomSkin(20066, CUSTOM_SKIN_TYPE_GROUP, GROUP_S9);
	/* Emily Erosel - http://www.gta-sarp.com/forums/showthread.php?297592-Development-Related-Emily_Erosel-s-Request */
	RegisterCustomSkin(20067, CUSTOM_SKIN_TYPE_DONOR, 7516);
	RegisterCustomSkin(20068, CUSTOM_SKIN_TYPE_DONOR, 372); // Ren Rosetti donor skin 
	RegisterCustomSkin(20069, CUSTOM_SKIN_TYPE_DONOR, 30420); // Jay Destiny donor skin

	// SANG //
	RegisterCustomSkin(20073, CUSTOM_SKIN_TYPE_FACTION, FACTION_SANG);
	RegisterCustomSkin(20074, CUSTOM_SKIN_TYPE_FACTION, FACTION_SANG);
	RegisterCustomSkin(20075, CUSTOM_SKIN_TYPE_FACTION, FACTION_SANG);
	RegisterCustomSkin(20076, CUSTOM_SKIN_TYPE_FACTION, FACTION_SANG);
	RegisterCustomSkin(20077, CUSTOM_SKIN_TYPE_FACTION, FACTION_SANG);
	RegisterCustomSkin(20078, CUSTOM_SKIN_TYPE_FACTION, FACTION_SANG);
	RegisterCustomSkin(20079, CUSTOM_SKIN_TYPE_FACTION, FACTION_SANG);

	/* Manuel_Delgado - http://www.gta-sarp.com/forums/member.php?57715-Antwan_Brooks*/
	RegisterCustomSkin(20080, CUSTOM_SKIN_TYPE_DONOR, 48583);

	/* Aaron Steel Skin - check previous skins for link*/
	RegisterCustomSkin(20081, CUSTOM_SKIN_TYPE_DONOR, 152639);
	/* Aaron Steel - http://www.gta-sarp.com/forums/showthread.php?302639-Game-Related-Aaron_Steel-s-Request */
	RegisterCustomSkin(20082, CUSTOM_SKIN_TYPE_DONOR, 152639);
	RegisterCustomSkin(20083, CUSTOM_SKIN_TYPE_GROUP, GROUP_HITMAN);
	RegisterCustomSkin(20084, CUSTOM_SKIN_TYPE_GROUP, GROUP_HITMAN);

	/* Steven Royce */
	RegisterCustomSkin(20086, CUSTOM_SKIN_TYPE_DONOR, 7385); 
	RegisterCustomSkin(20087, CUSTOM_SKIN_TYPE_DONOR, 7385); 

	/* Christian Collins */
	RegisterCustomSkin(20089, CUSTOM_SKIN_TYPE_DONOR, 7397); 
	/* SQL 7439 */

	RegisterCustomSkin(20090, CUSTOM_SKIN_TYPE_DONOR, 7408);

	RegisterCustomSkin(20092, CUSTOM_SKIN_TYPE_DONOR, 7381);
	
	/* Dominic Heimdall's skin */
	RegisterCustomSkin(20099, CUSTOM_SKIN_TYPE_DONOR, 7388);
	RegisterCustomSkin(20110, CUSTOM_SKIN_TYPE_DONOR, 7388);
	
	/* Rose Cassdiy skin */
	RegisterCustomSkin(20100, CUSTOM_SKIN_TYPE_DONOR, 7582); 
	RegisterCustomSkin(20101, CUSTOM_SKIN_TYPE_DONOR, 7582);
	RegisterCustomSkin(20102, CUSTOM_SKIN_TYPE_DONOR, 7582);
	
	/* Millhouse skins */
	RegisterCustomSkin(20111, CUSTOM_SKIN_TYPE_DONOR, 7441);
	
	/* Richie Townes */
	RegisterCustomSkin(20112, CUSTOM_SKIN_TYPE_DONOR, 7408);

	/* Josh Steel's custom skin by Shadow */
	RegisterCustomSkin(20113, CUSTOM_SKIN_TYPE_DONOR, 7742);

	/* Rocco's custom skin by Shadow */
	RegisterCustomSkin(20114, CUSTOM_SKIN_TYPE_DONOR, 7762);
	RegisterCustomSkin(20115, CUSTOM_SKIN_TYPE_DONOR, 7762);


	RegisterCustomSkin(20117, CUSTOM_SKIN_TYPE_DONOR, 7747); //ANTHONY

	RegisterCustomSkin(20118, CUSTOM_SKIN_TYPE_GROUP, 9); //jaxy playa's group skin
	
	RegisterCustomSkin(20120, CUSTOM_SKIN_TYPE_FACTION, 7); //Judicial dept skin
	
	RegisterCustomSkin(20121, CUSTOM_SKIN_TYPE_GROUP, 10); //Wermacht
	RegisterCustomSkin(20122, CUSTOM_SKIN_TYPE_GROUP, 10); //Wermacht
	RegisterCustomSkin(20123, CUSTOM_SKIN_TYPE_GROUP, 10); //Wermacht
	RegisterCustomSkin(20124, CUSTOM_SKIN_TYPE_GROUP, 10); //Wermacht
	
	
	RegisterCustomSkin(20125, CUSTOM_SKIN_TYPE_DONOR, 7516);
	RegisterCustomSkin(20126, CUSTOM_SKIN_TYPE_DONOR, 7516);
	//RegisterCustomSkin(20127, CUSTOM_SKIN_TYPE_DONOR, 7742);
	RegisterCustomSkin(20130, CUSTOM_SKIN_TYPE_DONOR, 7541);
}

// Set variables on connect
new bool:skinShopFree[MAX_PLAYERS],
	lastEquippedCSkin[MAX_PLAYERS];

Hook:skins_OnPlayerConnect(playerid) {
	skinShopFree[playerid] = false;
	lastEquippedCSkin[playerid] = gettime();
}

// Checks if the player is allowed to wear a certain skin
IsAllowedSkin(playerid, id) {
	if(!CustomSkin[id][cs_Used])
		return false;

	new skinType = CustomSkin[id][cs_SkinType];

	// Public skin
	if(skinType == CUSTOM_SKIN_TYPE_PUBLIC) return true;
	// Faction restricted skin
	else if(skinType == CUSTOM_SKIN_TYPE_FACTION) {
		if(PlayerInfo[playerid][pMember] == FACTION_S9) return true;
		if(PlayerInfo[playerid][pMember] != CustomSkin[id][cs_Meta1])
			return false;

		if(PlayerInfo[playerid][pRank] < CustomSkin[id][cs_Meta2])
			return false;

		return true;
	}
	// Group restricted skin
	else if(skinType == CUSTOM_SKIN_TYPE_GROUP) {
		if(PlayerInfo[playerid][pGroup] != CustomSkin[id][cs_Meta1])
			return false;

		if(PlayerInfo[playerid][pGroupRank] < CustomSkin[id][cs_Meta2])
			return false;

		return true;
	}
	// Donor restricted skin
	else if(skinType == CUSTOM_SKIN_TYPE_DONOR) {
		if(PlayerInfo[playerid][pID] != CustomSkin[id][cs_Meta1])
			return false;

		return true;
	}

	return true;
}

// Registers a skin
RegisterCustomSkin(skinID, skinType, skinMeta1 = 0, skinMeta2 = 0) {
	new skID = -1;

	// Find a free ID
	for(new i = 0; i < MAX_CUSTOM_SKINS; i++) {
		if(!CustomSkin[i][cs_Used]) {
			skID = i;
			break;
		}
	}

	// No free ID was found
	if(skID == -1) {
		printf("[Custom Skins] A skin failed to register.");
		return false;
	}

	CustomSkin[skID][cs_Used] = true;
	CustomSkin[skID][cs_SkinID] = skinID;
	CustomSkin[skID][cs_SkinType] = skinType;
	CustomSkin[skID][cs_Meta1] = skinMeta1;
	CustomSkin[skID][cs_Meta2] = skinMeta2;

	/*printf("[Custom Skins] Registered: skin ID %i, type: %i.",
		skinID,
		skinType
	);*/
	return true;
}

// Checks if a SAMP default skin is a civilian skin or not
IsACivilianSkin(skinID) {
	switch(skinID) {
		case 71:			return false;
		case 74:			return false;
		case 165 .. 166:	return false;
		case 265 .. 267:	return false;
		case 274 .. 288:	return false;
		case 300 .. 302:	return false;
		case 306 .. 311:	return false;
	}

	return true;
}

// Checks if a SAMP default skin is a cop skin or not
IsACopSkin(skinID) {
	switch(skinID) {
		case 71:			return true;
		case 280 .. 285:	return true;
		case 288:			return true;
		case 300 .. 302:	return true;
		case 306 .. 307:	return true;
		case 309 .. 311:	return true;
	}

	return false;
}

// Checks if a SAMP default skin is a FBI skin or not
IsAFBISkin(skinID) {
	switch(skinID) {
		case 286:			return true;
	}

	return false;
}

// Checks if a SAMP default skin is a medic skin or not
IsAMedicSkin(skinID) {
	switch(skinID) {
		case 274 .. 279:	return true;
		case 308:			return true;
	}

	return false;
}

// Checks if a SAMP default skin is a government skin or not
IsAGovSkin(skinID) {
	switch(skinID) {
		case 165 .. 166:	return true;
		case 17: return true;
		case 111: return true;
		case 228: return true;
	}

	return false;
}

// Checks if a SAMP default skin is a JD skin or not
IsAJDSkin(skinID) {
	switch(skinID) {
		case 165 .. 166:	return true;
	}

	return false;
}

IsANGSkin(skinID) {
	switch(skinID) {
		case 287:	return true;
		case 191: 	return true;
	}

	return false;
}

// Checks if the given skin ID is a restricted skin
isRestrictedModel(skinID) {
	return (
		IsAGovSkin(skinID) ||
		IsAMedicSkin(skinID) ||
		IsAFBISkin(skinID) ||
		IsACopSkin(skinID)
	);
}

// Shows the skin shop to a player
ShowSkinShop(
	playerid,
	bool:incCivilianSkins = true,	// Should the skin shop include civilian skins?
	bool:incFactionSkins = false,	// Should the skin shop include faction skins?
	bool:incGroupSkins = false,		// Should the skin shop include group skins?
	bool:incDonorSkins = false,		// Should the skin shop include donor skins?
	bool:allFree = false			// Should the items in the skin shop be free?
) {
	new skinsArr[mS_CUSTOM_MAX_ITEMS], amountToshow = 0;

	// Default SAMP skins
	for(new i = 1; i < 311; i++) {
		// Civilian skins
		if(incCivilianSkins && IsACivilianSkin(i)) {
			skinsArr[amountToshow] = i;
			amountToshow++;
			continue;
		}
		// Faction skins
		if(incFactionSkins) {
			if(
				// Perform the check for S9 first. If the player isn't S9 then show only their faction skin.
				PlayerInfo[playerid][pMember] == FACTION_S9 ||

				((IsACop(playerid) && IsACopSkin(i)) ||
				(IsAFBI(playerid) && IsAFBISkin(i)) ||
				(IsAMedic(playerid) && IsAMedicSkin(i)) ||
				(IsASenate(playerid) && IsAGovSkin(i))) ||
				(IsAJudge(playerid) && IsAJDSkin(i)) ||
				(IsASANG(playerid) && IsANGSkin(i))
			) {
				skinsArr[amountToshow] = i;
				amountToshow++;
				continue;
			}
		}
	}

	// Add all the necessary custom skins
	for(new i = 0; i < MAX_CUSTOM_SKINS; i++) {
		if(	!CustomSkin[i][cs_Used] ||
			!IsAllowedSkin(playerid, i)
		) continue;

		// Add civilian skin
		if(incCivilianSkins && CustomSkin[i][cs_SkinType] == CUSTOM_SKIN_TYPE_PUBLIC)
			skinsArr[amountToshow] = CustomSkin[i][cs_SkinID];
		// Add faction skin
		else if(incFactionSkins && CustomSkin[i][cs_SkinType] == CUSTOM_SKIN_TYPE_FACTION)
			skinsArr[amountToshow] = CustomSkin[i][cs_SkinID];
		// Add group skin
		else if(incGroupSkins && CustomSkin[i][cs_SkinType] == CUSTOM_SKIN_TYPE_GROUP)
			skinsArr[amountToshow] = CustomSkin[i][cs_SkinID];
		// Add donor skin
		else if(incDonorSkins && CustomSkin[i][cs_SkinType] == CUSTOM_SKIN_TYPE_DONOR)
			skinsArr[amountToshow] = CustomSkin[i][cs_SkinID];
		else continue;

		amountToshow++;
	}

	skinShopFree[playerid] = allFree;

	if(!allFree) {
		new skinStr[128];
		format(skinStr, sizeof(skinStr), "* A new outfit will cost you $%i.", SKIN_SHOP_PRICE);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, skinStr);
	}
	
	ShowModelSelectionMenuEx(playerid, skinsArr, amountToshow, "", SKIN_SHOP_MENU, 0.0, 0.0, 0.0, 1.0, 0x404240BB, 0x6e726e99);
	//ShowModelSelectionMenuEx(playerid, skinsArr, amountToshow, "", SKIN_SHOP_MENU);

	return bool:(amountToshow > 0);
}

// Called on model selection response
skins_OnPlayerModelSelectionEx(playerid, response, extraid, modelid) {
	if(!response || extraid != SKIN_SHOP_MENU) return false;

	if(!skinShopFree[playerid]) {
		if(PlayerInfo[playerid][pCash] < SKIN_SHOP_PRICE)
			return SendClientMessage(playerid, COLOR_GREY, "You cannot afford a new outfit.");

		// Take the money
		GiveMoney(playerid, -SKIN_SHOP_PRICE);

		// Add money to the business
		new clStore = GetPlayerVirtualWorld(playerid);
		BizInfo[clStore][bTill] += SKIN_SHOP_PRICE;
		BizInfo[clStore][bProducts]--;

		// Send info message
		new skinStr[128];
		format(skinStr, sizeof(skinStr), "* You have paid $%i for your new outfit.", SKIN_SHOP_PRICE);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, skinStr);
	}
	else SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have put on a new outift.");

	// Set the new skin
	SetPlayerSkin(playerid, modelid);
	PlayerInfo[playerid][pModel] = modelid;
	PlayerInfo[playerid][pClothes] = 0;

	// If it's one of their custom skins
	for(new i = 0; i < MAX_CUSTOM_SKINS; i++) {
		if(	!CustomSkin[i][cs_Used] ||
			!IsAllowedSkin(playerid, i)
		) continue;

		if(CustomSkin[i][cs_SkinType] != CUSTOM_SKIN_TYPE_DONOR)
			continue;

		if(CustomSkin[i][cs_SkinID] == modelid) {
			// Send prox message to inform players
			new skinStr[128];
			format(skinStr, sizeof(skinStr), "(( %s has equipped a custom skin ))", PlayerICName(playerid));
			ProxDetector(10.0, playerid, skinStr, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5);
			
			// Set cooldown
			lastEquippedCSkin[playerid] = gettime();
			break;
		}
	}
	return true;
}

// Checks if the player has any donated custom skins
HasAnyDonorSkins(playerid) {
	for(new i = 0; i < MAX_CUSTOM_SKINS; i++) {
		if(	!CustomSkin[i][cs_Used] ||
			!IsAllowedSkin(playerid, i)
		) continue;

		if(CustomSkin[i][cs_SkinType] == CUSTOM_SKIN_TYPE_DONOR)
			return true;
	}

	return false;
}

// Command for developers to list the skins they have access to
CMD:skindebug(playerid, params[]) {
	if(PlayerInfo[playerid][pDev] < DEVRANK_SENIOR)
		return AdmErrorMsg;

	SendClientMessage(playerid, COLOR_LIGHTBLUE, "The following custom skins are available to you:");
	
	new skinStr[128];
	for(new i = 0; i < MAX_CUSTOM_SKINS; i++) {
		if(!IsAllowedSkin(playerid, i)) continue;

		format(skinStr, sizeof(skinStr), "Skin ID %i | Skin is", CustomSkin[i][cs_SkinID]);
		
		switch(CustomSkin[i][cs_SkinType]) {
			case CUSTOM_SKIN_TYPE_PUBLIC: {
				format(skinStr, sizeof(skinStr), "%s public", skinStr);
			}
			case CUSTOM_SKIN_TYPE_FACTION: {
				format(skinStr, sizeof(skinStr), "%s restricted to your faction", skinStr);
			}
			case CUSTOM_SKIN_TYPE_GROUP: {
				format(skinStr, sizeof(skinStr), "%s restricted to your group", skinStr);
			}
			case CUSTOM_SKIN_TYPE_DONOR: {
				format(skinStr, sizeof(skinStr), "%s restricted to your SQL ID", skinStr);
			}
		}

		SendClientMessage(playerid, COLOR_RED, skinStr);
	}
	return 1;
}

// Command for developers to list all skins
CMD:allskins(playerid, params[]) {
	if(PlayerInfo[playerid][pDev] < DEVRANK_SENIOR)
		return AdmErrorMsg;

	SendClientMessage(playerid, COLOR_LIGHTBLUE, "The following custom skins are in-game:");
	
	new skinStr[128];
	for(new i = 0; i < MAX_CUSTOM_SKINS; i++) {
		if(!CustomSkin[i][cs_Used]) continue;

		format(skinStr, sizeof(skinStr), "Skin ID %i | Skin is", CustomSkin[i][cs_SkinID]);
		
		switch(CustomSkin[i][cs_SkinType]) {
			case CUSTOM_SKIN_TYPE_PUBLIC: {
				format(skinStr, sizeof(skinStr), "%s public", skinStr);
			}
			case CUSTOM_SKIN_TYPE_FACTION: {
				format(skinStr, sizeof(skinStr), "%s restricted to faction: %s (rank %i and above)", skinStr, GetFactionName(CustomSkin[i][cs_Meta1]), CustomSkin[i][cs_Meta2]);
			}
			case CUSTOM_SKIN_TYPE_GROUP: {
				format(skinStr, sizeof(skinStr), "%s restricted to group: %s (rank %i and above)", skinStr, GetGroupName(CustomSkin[i][cs_Meta1]), CustomSkin[i][cs_Meta2]);
			}
			case CUSTOM_SKIN_TYPE_DONOR: {
				format(skinStr, sizeof(skinStr), "%s restricted to SQL ID: %i", skinStr, CustomSkin[i][cs_Meta1]);
			}
		}

		SendClientMessage(playerid, COLOR_RED, skinStr);
	}
	return 1;
}

// Command for developers to show a certain skin shop
CMD:skinshop(playerid, params[]) {
	if(PlayerInfo[playerid][pDev] < DEVRANK_SENIOR)
		return AdmErrorMsg;

	new skShopType;
	if(sscanf(params, "i", skShopType)) {
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "{00BFFF}USAGE:{FFFFFF} /skinshop [type]");
		SendClientMessage(playerid, COLOR_GREY, "Type 0: Civilian skins only");
		SendClientMessage(playerid, COLOR_GREY, "Type 1: Faction skins only");
		SendClientMessage(playerid, COLOR_GREY, "Type 2: Group skins only");
		SendClientMessage(playerid, COLOR_GREY, "Type 3: Civilian and faction skins");
		SendClientMessage(playerid, COLOR_GREY, "Type 4: Donor skins");
		SendClientMessage(playerid, COLOR_GREY, "Type 5: All skins");
		return 1;
	}

	if(skShopType < 0 || skShopType > 5)
		return SendClientMessage(playerid, COLOR_GREY, "Invalid skin shop type.");

	new anythingToShow;

	switch(skShopType) {
		case 0: anythingToShow = ShowSkinShop(playerid, true, false, false, false, true);
		case 1: anythingToShow = ShowSkinShop(playerid, false, true, false, false, true);
		case 2: anythingToShow = ShowSkinShop(playerid, false, false, true, false, true);
		case 3: anythingToShow = ShowSkinShop(playerid, true, true, false, false, true);
		case 4: anythingToShow = ShowSkinShop(playerid, false, false, false, true, true);
		case 5: anythingToShow = ShowSkinShop(playerid, true, true, true, true, true);
	}

	if(!anythingToShow)
		SendClientMessage(playerid, COLOR_GREY, "Nothing to show in this skin shop.");
	return 1;
}

// Command for donors to equip one of their skins
CMD:donorskin(playerid, params[])
	return cmd_myskins(playerid, params);

CMD:myskin(playerid, params[])
	return cmd_myskins(playerid, params);

CMD:myskins(playerid, params[]) {
	if(!HasAnyDonorSkins(playerid)) {
		SendClientMessage(playerid, COLOR_GREY, "Oops! Looks like you don't have any donor skins at the moment.");
		SendClientMessage(playerid, COLOR_GREY, "If you are interested in having your own exclusive skin, please follow the link below.");
		SendClientMessage(playerid, COLOR_GREY, "Donation link: http://donate.gta-ecrp.com/.");
		return 1;
	}

	if(IsPlayerRestricted(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You cannot do this right now.");

	if(IsPlayerInAnyVehicle(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You cannot do this while being in a vehicle.");

	if(IsPlayerMoving(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You must be standing still.");

	// 5 minute cooldown on this command
	//if(gettime() - lastEquippedCSkin[playerid] < (5 * 60))
	//	return SendClientMessage(playerid, COLOR_GREY, "You need to wait 5 minutes in between doing that.");

	ShowSkinShop(playerid, false, false, false, true, true);
	return 1;
}
