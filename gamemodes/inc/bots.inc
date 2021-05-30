// BOTS
#include <a_samp>
#include <a_angles>

new npcsLoaded; //this is set to 1 when all the bots join the server

#define SOUND_RADIUS 		30

#define COLOR_YELLOW 		0xFFFF00AA
#define COLOR_WHITE 		0xFFFFFFAA
#define COLOR_FADE1 		0xE6E6E6E6
#define COLOR_FADE2 		0xC8C8C8C8
#define COLOR_FADE3 		0xAAAAAAAA
#define COLOR_FADE4 		0x8C8C8C8C
#define COLOR_FADE5 		0x6E6E6E6E

//dialog triggers
#define DIALOG_BUYGUN		2555 //emmet
#define DIALOG_BUYGUN2		2565 //emmet2
#define DIALOG_BUYLIQUOR	2566 //liquor
#define DIALOG_BUYSTRIP		2567 //strip
#define DIALOG_RENTRC		2600
#define DIALOG_PD_SEC		2601
#define DIALOG_PD_SEC_JOIN	2602

#define DIALOG_FBI	2603
#define DIALOG_FBI_BADGE	2604

//spawn delay
#define SPAWN_DELAY  2000

// MSG INDEXES
#define MSGIDX_VIKTOR	(0)
#define MSGIDX_RYDER1	(1)
#define MSGIDX_RYDER2	(2)
#define MSGIDX_SMOKE1	(3)
#define MSGIDX_REDNECK3	(4)
#define MSGIDX_VIKTOR2	(5)
#define MSGIDX_REFUGEE	(6)
#define MSGIDX_CARGO	(7)

//-----------------MISSION DIALOG IDS
#define DIALOG_MISSION_DROPCAR 2556 //bot_spawn
#define DIALOG_MISSION_DRUGPICKUP 2557 //bot_spawn
//#define DIALOG_MISSION_PDBREAKIN 2558 //bot_mafia (old + unused)
#define DIALOG_MISSION_TOWTAXI 2559 //bot_taxi
#define DIALOG_MISSION_RYDER1 2560 //bot_ryder
#define DIALOG_MISSION_RYDER2 2561 //bot_ryder
#define DIALOG_MISSION_SMOKE1 2562 //bot_smoke
#define DIALOG_MISSION_DROPCOUCH 2563 //bot_redneck3
#define DIALOG_MISSION_VISAS 2564 //bot_triad
#define DIALOG_MISSION_CARGO 2568 //bot_cargo
#define DIALOG_JOINBALLAS 2569 //og_kane
#define DIALOG_JOINGROVE 2570 //sweet
#define DIALOG_MISSION_MEDICINE 2571 //construction worker

//-----------------MISSION CHECKPOINT IDS
#define MISSION_NO_CP 0 //no checkpoint
#define MISSION_DROPCAR_CP1 1001
#define MISSION_DROPCAR_CP2 1002
#define MISSION_DRUGPICKUP_CP1 1003
#define MISSION_DRUGPICKUP_CP2 1004
#define MISSION_PDBREAKIN_CP1 1005
#define MISSION_DROPCOUCH_CP1 1106
#define MISSION_DROPCOUCH_CP2 1107
#define MISSION_DROPCOUCH_CP3 1108
#define MISSION_VISAS_CP 1109
#define MISSION_CARGO_CP1 1110
#define MISSION_CARGO_CP2 1111
#define MISSION_CARGO_CP3 1112
#define MISSION_CARGO_CP4 1113
#define MISSION_CARGO_CP5 1114
#define MISSION_CARGO_CP6 1115
#define MISSION_CARGO_CP7 1116
#define MISSION_CARGO_CP8 1117

#define MAX_MISSIONS 10

enum botInfo {
	bot_created,
	bot_name[MAX_PLAYER_NAME],
	bot_type,
	bot_skin,
    Float:bot_pos[4], //x, y, z, a
	virtualWorld,
	interiorID,
	Text3D:Label, //for displaying the bots name
	interactingWithID,
	playerInRange,
	beingRobbedBy,
	robbedTimerID,
	noRandChat,
	rSoundID[5], //5 sound ids to play after each interaction
	rText1[128],
	rText2[128],
	rText3[128],
	rText4[128],
	rText5[128],
	r_animLib1[24],
	r_animName1[24],
	r_animLib2[24],
	r_animName2[24],
	r_animLib3[24],
	r_animName3[24],
	r_animLib4[24],
	r_animName4[24],
	r_animLib5[24],
	r_animName5[24],
	animFreeze[5], 
	animLoop[5], 
	rTotal, //number of rand sounds used
};
new npcInfo[MAX_PLAYERS][botInfo];

//player vars
new interactingWithBotID[MAX_PLAYERS]; //which bot they have last spoken with
new botInRange[MAX_PLAYERS];
new robbingBotID[MAX_PLAYERS];
new robbingBotStage[MAX_PLAYERS];

//robbed vars
new hearingAlarm[MAX_PLAYERS];
new bot_liquor_robbedtime; //this variable is used so people can't rob the bot all the time

//stripper vars
new privateRoomTicket[MAX_PLAYERS];
new vanCargoAmount[MAX_PLAYERS]; //for cargo mission

new bjStage;
new bjTimerID;
new privateRooomOccupiedByID = -1;

//bot ids
new bot_homeless;
new bot_emmet;
new bot_clayton;
new bot_cesar;
new bot_cadet;
new bot_liquor;
new bot_construction;
new bot_construction2;
new bot_triad;
new bot_redneck;
new bot_refugee;
new bot_redneck2;
new bot_redneck3;
new bot_spawn;
new bot_court;
new bot_airport;
new bot_taxi;
new bot_needdrugs1;
new bot_needdrugs2;
new bot_mechanic;
new taxi_broken;
new bot_smoke;
new bot_ryder;
new bot_grove;
new bot_ballas;
new bot_zero;
new bot_pdreception;
new bot_freddy;
new bot_pizza;
//new bot_fishing;
new bot_stripbartender;
new bot_stripper1;
new bot_stripper2;
new bot_lapdance;
new bot_cargo;
new bot_caravan_dealer;
new bot_mine_exchange;
new bot_fbireception;
new bot_royal_guard;

//missions
new missionCP[MAX_PLAYERS]; //the checkpointid they have
new missionID[MAX_PLAYERS]; //the mission id they are on
new missionVehicle[MAX_PLAYERS];
new missionVehicleObj[MAX_PLAYERS];
new missionPickup[MAX_PLAYERS];
new missionFailedTimerID[MAX_PLAYERS];

//area triggers
new triggerID[MAX_PLAYERS];

//trigger ids
#define PLAYER_INAREA_NONE 0
#define PLAYER_INAREA_AIRPORTGATE1 1
#define PLAYER_INAREA_AIRPORTGATE2 2
#define PLAYER_INAREA_VISASTRIG 3
#define PLAYER_INAREA_CARGO 4
#define PLAYER_INAREA_COURTDETECT1 5
#define PLAYER_INAREA_COURTDETECT2 6

//dropcar tutorial vehicle spawns
new Float:dropcarSpawns[33][4] = {
{2148.3691,-1143.4242,24.6895,89.9030},
{2217.0466,-1161.8152,25.4665,89.8435},
{2410.5012,-1224.1042,24.4615,359.0143},
{1088.1655,-1192.8837,17.9074,91.1097},
{337.6074,-1788.7439,4.6709,0.4662},
{482.8692,-1765.5518,5.2807,270.0171},
{1989.1411,-2058.8337,13.1104,269.2782},
{2338.7241,-2073.0811,13.2921,266.9537},
{2854.2559,-2032.1626,10.6853,180.1589},
{1803.9011,-1933.2146,13.1910,179.0784},
{1631.3313,-1906.7238,13.3547,88.9337},
{1657.3303,-1806.6656,13.3520,267.5438},
{1276.9789,-2044.3879,58.8440,269.6181},
{1015.4036,-1369.5646,13.1753,269.9572},
{1991.9363,-2058.9910,13.1738,0.1781},
{2114.3176,-2159.0369,13.3600,88.5451},
{2078.3267,-2046.8698,13.3516,86.7094},
{2078.2559,-2033.3795,13.3508,90.9097},
{2078.5417,-2020.1810,13.3516,89.3973},
{2078.3918,-2006.7511,13.3508,89.5683},
{2020.2644,-1974.6487,13.3577,90.0238},
{2098.9761,-1982.2699,13.3496,89.6022},
{2229.7234,-2090.0798,13.3514,133.9385},
{2508.6257,-2118.2449,13.3498,179.4913},
{2460.8372,-2118.1199,13.3557,178.0187},
{2379.9463,-2084.8877,13.3180,90.5916},
{2445.2632,-1761.8135,13.3933,356.9626},
{2451.8215,-1789.0499,13.3541,179.2184},
{2451.8215,-1789.0499,13.3541,179.2184},
{2815.8005,-1539.7733,10.7266,359.7243},
{2796.6357,-1549.8243,10.7257,90.5832},
{2796.6182,-1571.7322,10.7312,90.7160},
{2796.6047,-1580.4495,10.7299,89.3349}
};


new dropcarTypes[5] = {
439, //stallion
421, //washington
419, //esperanto
413, //pony
575 //broadway
};

//pickup spawns
new Float:pickupSpawns[1][4] = {
{2324.3022,-1250.0573,27.9766,357.1621} //drugpickup mission pickup
};

//AIRPORT GATE
new airportGateOpen;
new airportGateTimerID;

new airportGateObjectID1;
new airportGateObjectID2;

new gateMoving;

stock airportGateTrigger() //function for opening/closing the airport gate
{
	if(!gateMoving) {
		if(airportGateOpen == 0) {
			MoveObject(airportGateObjectID1, 1968.90, -2189.93, 13.62, 1.50,  0.00, 0.00, 180.00);
			MoveObject(airportGateObjectID2, 1954.12, -2189.93, 13.62, 1.50,  0.00, 0.00, 180.00);
			airportGateOpen = 1;
			airportGateTimerID = SetTimer("airportGateClose", 10000, 0);
		}
		else {
			MoveObject(airportGateObjectID1, 1964.28, -2189.93, 13.62, 1.50, 0.00, 0.00, 180.00);
			MoveObject(airportGateObjectID2, 1958.80, -2189.93, 13.62, 1.50,  0.00, 0.00, 180.00);
			airportGateOpen = 0;
			KillTimer(airportGateTimerID);
		}
		gateMoving = 1;
	}
	
}

forward airportGateClose();
public airportGateClose() {
	if(!gateMoving && airportGateOpen) {
		airportGateTrigger();
	}
}

stock createAirportGate() {
	airportGateObjectID1 = CreateObject(988, 1964.28, -2189.93, 13.62,   0.00, 0.00, 180.00);
	airportGateObjectID2 = CreateObject(988, 1958.80, -2189.93, 13.62,   0.00, 0.00, 180.00);
}

stock destroyAirportGate() {
	DestroyObject(airportGateObjectID1);
	DestroyObject(airportGateObjectID2);
}

stock pauseRandChatForDuration(botid, seconds) {
	botPauseRandChat(botid);
	SetTimerEx("botResumeRandChat",seconds*1000,0,"i",botid);
}

stock toggleRandChat(botid, seconds, on) {
	if(on) {
		SetTimerEx("botResumeRandChat",seconds*1000,0,"i",botid);
	} else {
		SetTimerEx("botPauseRandChat",seconds*1000,0,"i",botid);
	}
}

forward botPauseRandChat(botid);
public botPauseRandChat(botid) {
	npcInfo[botid][noRandChat] = 1;
}

forward botResumeRandChat(botid);
public botResumeRandChat(botid) {
	npcInfo[botid][noRandChat] = 0;
}

forward AddActor(rpname[], btype, bskin, Float: x, Float: y, Float: z, Float: a, world, interid);
public AddActor(rpname[], btype, bskin, Float: x, Float: y, Float: z, Float: a, world, interid)
{
	new botid = CreateActor(bskin, x, y, z, a); //model x y z rotation
	
	strmid(npcInfo[botid][bot_name], rpname, 0, strlen(rpname), 128);
	
	npcInfo[botid][bot_created] = 1;
	npcInfo[botid][bot_type] = btype;
    npcInfo[botid][bot_skin] = bskin;
	
    npcInfo[botid][bot_pos][0] = x;
    npcInfo[botid][bot_pos][1] = y;
    npcInfo[botid][bot_pos][2] = z;
    npcInfo[botid][bot_pos][3] = a;
    npcInfo[botid][virtualWorld] = world;
	npcInfo[botid][interiorID] = interid;
	
	npcInfo[botid][interactingWithID] = -1; //not interacting
	npcInfo[botid][noRandChat] = 0;
	npcInfo[botid][playerInRange] = 0; //no player in range
	npcInfo[botid][beingRobbedBy] = -1; //no player robbing
	npcInfo[botid][robbedTimerID] = 0; //no timer
	
	SetActorVirtualWorld(botid, npcInfo[botid][virtualWorld]);
	
	/* these functions are not avaiable for actors
	//SetActorInterior(botid, npcInfo[botid][interiorID]);
	//wearable stuff
	if(botid == bot_stripper1) {
		SetPlayerAttachedObject(bot_stripper1,0,18639,2,0.156999,0.010998,0.000000,0.799996,-0.699998,-3.200000,1.000000,1.137999,1.000000);
		SetPlayerAttachedObject(bot_stripper1,1,3027,2,0.004999,0.086998,0.003000,-113.599975,-2.800003,0.000000,0.819998,0.642000,0.677999);
	}
	
	if(botid == bot_fishing) {
		SetPlayerAttachedObject(bot_fishing, 0, 18632, 6, 0.06, 0.04, 0.0, 180.0, 0.0, 0.0);
	}*/
	
	//label
	new namestr[32];
	format(namestr, sizeof(namestr), "%s (%d)", npcInfo[botid][bot_name], botid);
	npcInfo[botid][Label] = CreateDynamic3DTextLabel(namestr,0xFFFFFFFF,x, y, z +1.2, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100); //
	
    return botid;
}

forward createActors();
public createActors() {

	bot_homeless = AddActor("Bobby", 0, 79, 2390.7388, -1981.5107, 13.5469, 332.0231, 0, 0);
	bot_emmet = AddActor("Emmet", 0, 6, 2441.7942, -1964.3752, 13.5469, 228.2673, 0, 0);
	
	bot_cesar = AddActor("Cesar", 0, 292, 1688.1514, -2099.0596, 13.8343, 180.4563, 0, 0);
	bot_cadet = AddActor("Cadet", 0, 71, 1544.0502, -1631.9587, 13.3828, 91.8117, 0, 0);
	
	bot_construction = AddActor("Construction Worker", 0, 27, 1066.8724,-355.5412,73.9922,37.6136, 0, 0);
	addRandomSpeechText(bot_construction, "(Construction Worker nearly vomits)", 7868, "FOOD", "EAT_Vomit_P", 0, 0);
	addRandomSpeechText(bot_construction, "(Construction Worker nearly vomits)", 7866, "FAT", "IDLE_tired", 0, 1);
	
	bot_construction2 = AddActor("Ferry_Deepole", 0, 27, 1053.8528,-325.9841,-13.4725,37.4422, 0, 0);
	
	bot_triad = AddActor("Triad", 0, 122, 1036.5846,-1116.2133,23.8979,166.2730, 0, 0);
	bot_redneck = AddActor("Redneck", 0, 162, 687.5881, -472.5642, 16.5363,2.4122, 0, 0);
	bot_refugee = AddActor("Refugee", 0, 229, 410.9005, -2279.5581, 8.6423, 269.5727, 0, 0);
	bot_redneck2 = AddActor("Billy", 0, 159, 681.4788, -451.6862, -25.6172, 177.8065, 0, 0);
	bot_redneck3 = AddActor("Cletus", 0, 160, 690.4218, -586.9496, 16.3281, 92.2098, 0, 0);
	bot_spawn = AddActor("Viktor", 0, 112, 1827.3022, -1379.3441, 14.4219, 58.3930, 0, 0);
	bot_court = AddActor("Receptionist", 0, 194, 203.5712, 168.3158, 1003.0234, 90.5150, 0, 0);
	bot_airport = AddActor("Security", 0, 71, 1955.4437, -2181.5933, 13.5865, 270.5141, 0, 0);
	bot_taxi = AddActor("Abu", 0, 253, 1994.0986, -1944.6438, 13.5469, 315.2042, 0, 0);
	
	bot_needdrugs1 = AddActor("Oscar", 0, 8, 122.5568, -300.6841, 1.5781, 89.7805, 0, 0);
	
	bot_needdrugs2 = AddActor("Tyrone", 0, 28, 2160.2925, -1693.3661, 15.0859, 269.1805, 0, 0);
	addRandomSpeechText(bot_needdrugs2, "null", 0, "SMOKING", "M_smklean_loop", 0, 1);
	
	bot_mechanic = AddActor("Mechanic", 0, 50, 2280.1926, -2363.3188, 13.5469, 306.0264, 0, 0);
	
	bot_smoke = AddActor("Big_Smoke", 0, 269, 2492.9280,-1693.0078,14.7656,45.2915, 0, 0);
	addRandomSpeechText(bot_smoke, "null", 0, "DEALER", "DEALER_IDLE_01", 0, 1);
	
	bot_grove = AddActor("Sweet_Johnson", 0, 270, 2521.3630, -1678.5033, 15.3839, 86.3033, 0, 0);
	addRandomSpeechText(bot_grove, "null", 0, "DEALER", "DEALER_IDLE", 0, 1);
	
	bot_ballas = AddActor("OG_Kane", 0, 104, 1995.8113, -1115.6873, 26.7737, 182.6373, 0, 0);
	bot_ryder = AddActor("Ryder", 0, 271, 2456.6987 ,-1707.9587, 13.5893, 179.5085, 0, 0);
	addRandomSpeechText(bot_ryder, "null", 0, "SMOKING", "M_smklean_loop", 0, 1);
	
	bot_zero = AddActor("Zero", 0, 289, -2238.0530, 128.5867, 1035.4141, 0.0237, 0, 6);
	bot_pdreception = AddActor("Receptionist", 0, 141, -2202.40, 835.53, 9999.28, 0.5813, 0, 0);
	
	bot_freddy = AddActor("Freddy", 0, 108, 2720.8120, -2458.9160, 13.6459, 271.0722, 0, 0);
	addRandomSpeechText(bot_freddy, "null", 0, "SMOKING", "M_smklean_loop", 0, 1);
	
	bot_liquor = AddActor("Reggie", 0, 14, 252.2993, -54.8286, 1.5776, 181.3455, 0, 0);
	bot_clayton = AddActor("Clayton", 0, 133, 160.4093, -176.7875, 1.5781, 48.9583, 0, 0);
	bot_pizza = AddActor("Ryan", 0, 155, 215.0250, -182.4211, 1.5781, 45.5394, 0, 0);
	
	//bot_fishing = AddActor("Fred", 0, 35, 382.8322, -282.3294, 2.5416, 135.9302, 0, 0);
	//addRandomSpeechText(bot_fishing, "null", 0, "CAMERA", "piccrch_in", 1, 0);
	
	bot_stripbartender = AddActor("Barbara", 0, 31, 1206.1969, -29.8203, 1000.9531, 267.6797, 39, 0); //vw 10
	
	bot_stripper1 = AddActor("Candy", 0, 257, 1209.1958, -36.2388, 1001.4844, 9.5378, 39, 0); //vw 10
	addRandomSpeechText(bot_stripper1, "null", 0, "STRIP", "STR_Loop_A", 0, 1);
	
	bot_stripper2 = AddActor("Janice", 0, 246, 1215.5109, -33.5600, 1001.3904, 89.9202, 39, 0); //vw 10
	addRandomSpeechText(bot_stripper2, "null", 0, "STRIP", "STR_Loop_B", 0, 1);
	
	bot_lapdance = AddActor("Prostitute", 0, 75, 1203.6416, 15.9626, 1000.9219, 328.3460, 39, 2); //vw 10, int 2
	bot_cargo = AddActor("Worker", 0, 132, 303.6379, -226.6423, 1.5781, 268.1658, 0, 0);
	bot_fbireception = AddActor("Agent", 0, 286, 1046.2452, 1535.3804, 9011.0957, 90, 10, 10);

	SetTimer("botRandomTalk", 10000, 1); //10 second
	SetTimer("playerInRangeOfBotCheck", 1000, 1); //1 second
	print("SERVER: NPCs have successfully loaded.");
	npcsLoaded = 1;
	return 1;
}

forward LockTaxi();
public LockTaxi() {
	SetVehicleParamsEx(taxi_broken,0,1,0,1,1,0,0);
}
stock startInteracting(playerid) {
	if(botInRange[playerid] != -1) {
		npcInfo[botInRange[playerid]][interactingWithID] = playerid; //set for bot
		interactingWithBotID[playerid] = botInRange[playerid]; //set for player
	}
}

stock stopInteracting(playerid) {
	if(interactingWithBotID[playerid] != -1) {
		npcInfo[interactingWithBotID[playerid]][interactingWithID] = -1; //set for bot
		interactingWithBotID[playerid] = -1; //set for player
	}
}

stock botHandShakeOffer(botid, playerid, htype) {
	new string[128];
	format(string, sizeof(string), "* %s has offered to shake your hand (type /accept handshake), to accept it.", npcInfo[botid][bot_name]);
	SendClientMessage(playerid, COLOR_LIGHTBLUE, string);
	HandshakeOffer[playerid] = npcInfo[botid][id];
	HandshakeType[playerid] = htype;
}

stock addRandomSpeechText(botid, msgstring[], soundid, animLib[], animName[], freezes, loop) {
	
	if(npcInfo[botid][rTotal] < 5) {
		switch (npcInfo[botid][rTotal]) {
			case 0: {
				strmid(npcInfo[botid][rText1], msgstring, 0, strlen(msgstring), 128);
				npcInfo[botid][rSoundID][0] = soundid;
				strmid(npcInfo[botid][r_animLib1], animLib, 0, strlen(animLib), 128);
				strmid(npcInfo[botid][r_animName1], animName, 0, strlen(animName), 128);
				npcInfo[botid][animFreeze][0] = freezes;
				npcInfo[botid][animLoop][0] = loop;
			}
			case 1: {
				strmid(npcInfo[botid][rText2], msgstring, 0, strlen(msgstring), 128);
				npcInfo[botid][rSoundID][1] = soundid;
				strmid(npcInfo[botid][r_animLib2], animLib, 0, strlen(animLib), 128);
				strmid(npcInfo[botid][r_animName2], animName, 0, strlen(animName), 128);
				npcInfo[botid][animFreeze][1] = freezes;
				npcInfo[botid][animLoop][1] = loop;
			}
			case 2: {
				strmid(npcInfo[botid][rText3], msgstring, 0, strlen(msgstring), 128);
				npcInfo[botid][rSoundID][2] = soundid;
				strmid(npcInfo[botid][r_animLib3], animLib, 0, strlen(animLib), 128);
				strmid(npcInfo[botid][r_animName3], animName, 0, strlen(animName), 128);
				npcInfo[botid][animFreeze][2] = freezes;
				npcInfo[botid][animLoop][2] = loop;
			}
			case 3: {
				strmid(npcInfo[botid][rText4], msgstring, 0, strlen(msgstring), 128);
				npcInfo[botid][rSoundID][3] = soundid;
				strmid(npcInfo[botid][r_animLib4], animLib, 0, strlen(animLib), 128);
				strmid(npcInfo[botid][r_animName4], animName, 0, strlen(animName), 128);
				npcInfo[botid][animFreeze][3] = freezes;
				npcInfo[botid][animLoop][3] = loop;
			}
			case 4: {
				strmid(npcInfo[botid][rText5], msgstring, 0, strlen(msgstring), 128);
				npcInfo[botid][rSoundID][4] = soundid;
				strmid(npcInfo[botid][r_animLib5], animLib, 0, strlen(animLib), 128);
				strmid(npcInfo[botid][r_animName5], animName, 0, strlen(animName), 128);
				npcInfo[botid][animFreeze][4] = freezes;
				npcInfo[botid][animLoop][4] = loop;
			}
		}
		npcInfo[botid][rTotal]++;
	} else {
		printf("addRandomSpeechText: bot rTotal is %d, cannot add any more", npcInfo[botid][rTotal]);
	}
	
	return 1;
}

stock isNearTrigger(playerid) {	
	if(IsPlayerInRangeOfPoint(playerid, 5, 1961.4075, -2182.6155, 13.5469)) {
		if(triggerID[playerid]) return PLAYER_INAREA_AIRPORTGATE1;
	
		triggerID[playerid] = PLAYER_INAREA_AIRPORTGATE1;
		OnTriggerEvent(playerid, triggerID[playerid]);
		return PLAYER_INAREA_AIRPORTGATE1;
	}
	
	if(IsPlayerInRangeOfPoint(playerid, 5, 1961.7549, -2195.1021, 13.5469)) {
		if(triggerID[playerid]) return PLAYER_INAREA_AIRPORTGATE2;
		
		triggerID[playerid] = PLAYER_INAREA_AIRPORTGATE2;
		OnTriggerEvent(playerid, triggerID[playerid]);
		return PLAYER_INAREA_AIRPORTGATE2;
	}
	
	if(IsPlayerInRangeOfPoint(playerid, 12, 724.8891, -1744.5028, 0.2582)) { //for the fake visas mission
		if(triggerID[playerid]) return PLAYER_INAREA_VISASTRIG;
		
		triggerID[playerid] = PLAYER_INAREA_VISASTRIG;
		OnTriggerEvent(playerid, triggerID[playerid]);
		return PLAYER_INAREA_VISASTRIG;
	}
	
	if(IsPlayerInRangeOfPoint(playerid, 20, 990.1048, -1353.4719, 13.3725)) { //for the cargo mission
		if(triggerID[playerid]) return PLAYER_INAREA_CARGO;
		
		triggerID[playerid] = PLAYER_INAREA_CARGO;
		OnTriggerEvent(playerid, triggerID[playerid]);
		return PLAYER_INAREA_CARGO;
	}
	
	if(GetPlayerInterior(playerid) == 3) { //courthouse
	
		if(IsPlayerInRangeOfPoint(playerid, 0.5, 214.7873,159.8227,1003.0234)) {
			if(triggerID[playerid]) return PLAYER_INAREA_COURTDETECT1;
		
			triggerID[playerid] = PLAYER_INAREA_COURTDETECT1;
			OnTriggerEvent(playerid, triggerID[playerid]);
			return PLAYER_INAREA_COURTDETECT1;
		}
		
		if(IsPlayerInRangeOfPoint(playerid, 0.5, 214.7842,177.3051,1003.0313)) {
			if(triggerID[playerid]) return PLAYER_INAREA_COURTDETECT2;
		
			triggerID[playerid] = PLAYER_INAREA_COURTDETECT2;
			OnTriggerEvent(playerid, triggerID[playerid]);
			return PLAYER_INAREA_COURTDETECT2;
		}
	}
	//SendClientMessage(playerid, COLOR_YELLOW, "PLAYER_INAREA_NONE");
	triggerID[playerid] = PLAYER_INAREA_NONE;
	return PLAYER_INAREA_NONE;
}

forward endMissionDelay(playerid, msg_idx);
public endMissionDelay(playerid, msg_idx) {
	switch(msg_idx) {
	    case MSGIDX_VIKTOR: { // Viktor
	        GameTextForPlayer(playerid, "~y~Mission Passed!~n~~w~Respect+", 1000, 0);
	    }
		case MSGIDX_VIKTOR2: { // Viktor2
	        GameTextForPlayer(playerid, "~y~Mission Passed!~n~~w~$550", 1000, 0);
	    }
	    case MSGIDX_RYDER1: {
	        GameTextForPlayer(playerid, "~y~Mission Passed!~n~~w~$120", 1000, 0);
	    }
	    case MSGIDX_RYDER2: {
	        GameTextForPlayer(playerid, "~y~Mission Passed!~n~~w~$500", 1000, 0);
	    }
        case MSGIDX_SMOKE1: {
	        GameTextForPlayer(playerid, "~y~Mission Passed!~n~~w~$250", 1000, 0);
	    }
		case MSGIDX_REDNECK3: { //cletus couch deliver
	        GameTextForPlayer(playerid, "~y~Mission Passed!~n~~w~$700", 1000, 0);
	    }
		case MSGIDX_REFUGEE: {
	        GameTextForPlayer(playerid, "~y~Mission Passed!~n~~w~$650", 1000, 0);
	    }
		case MSGIDX_CARGO: {
	        GameTextForPlayer(playerid, "~y~Mission Passed!~n~~w~$950", 1000, 0);
	    }
	}
	//GameTextForPlayer(playerid, string, 1000, 0);
	
	
	
}

stock endMission(playerid) {
	missionID[playerid] = 0;
	missionCP[playerid] = MISSION_NO_CP;
	missionVehicle[playerid] = 0;
	missionVehicleObj[playerid] = 0;
	missionPickup[playerid] = 0;
	vanCargoAmount[playerid] = 0;
}

