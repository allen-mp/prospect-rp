/*=================

	Christmas Event 2017
	by Jay Cortez

=================*/

#define CHRISTMAS_FILE 			"christmas.ini"
#define CHRISTMAS_SOUND			"http://k007.kiwi6.com/hotlink/s8lgc8cam9/Christmas_Shadow2.mp3"
#define CHRISTMAS_SONG			"http://k007.kiwi6.com/hotlink/ruihyflkl6/Christmas_Shadow.mp3"
#define INVALID_PRESENT_ID		-1
#define MAX_PRESENTS			10
#define PRESENTS_TO_FIND		15

new christmasEnabled = 0;

new christmasAuthorized[] = {
	7378
};

new presentTypes[][] = {
	{19054, "Red Christmas Present"},
	{19055, "Yellow & Red Christmas Present"},
	{19056, "Rainbow Christmas Present"},
	{19057, "Red & Yellow Christmas Present"},
	{19058, "Blue Christmas Present"}
};

new Float:presentLocations[][] = {
	{1923.0643,-1762.3568,13.5391},
	{2109.9890,-1777.3737,13.3907},
	{1705.5219,-1723.8135,13.5469},
	{1516.0220,-1722.7667,13.5469},
	{1541.5997,-1671.5204,13.5529},
	{1903.6632,-1442.9767,13.5117},
	{1920.2618,-1375.3816,13.6091},
	{1531.3605,-1027.1975,24.0147},
	{1353.8766,-1096.8927,24.0971},
	{1135.6216,-1413.0117,13.6376},
	{369.8156,-1849.8510,7.6719},
	{290.6794,-1630.2468,33.3401},
	{347.7054,-1514.2289,33.0747},
	{708.8488,-1415.4913,13.5358},
	{1037.6407,-1843.1703,13.5818},
	{1282.5315,-1844.9580,13.5508},
	{1355.9678,-1751.0450,13.3735},
	{2133.3059,-1497.4581,23.9849},
	{2414.8562,-1537.9738,24.0000},
	{2713.3225,-1648.8042,13.2279},
	{1968.4695,-2174.6074,13.5469},
	{1936.4542,-1596.8789,13.5499},
	{2410.6375,-1248.8066,23.8125},
	{2117.5510,-1732.9569,18.0628},
	{2391.9080,-1742.1482,13.5469},
	{2487.1521,-1666.0365,13.3438},
	{2031.2340,-1416.0886,16.9922},
	{2059.5017,-1889.0714,20.0000},
	{1778.4739,-1775.4453,52.4688},
	{1515.2535,-1360.3254,190.8017},
	{899.8443,-1034.9996,95.0234},
	{167.7834,-1752.6432,5.2969},
	{260.8490,-1278.7782,85.3526},
	{1332.1227,-639.5985,114.0349},
	{1501.0984,-692.8601,94.7500},
	{1492.1064,-691.5677,94.7500},
	{1482.3835,-692.0007,94.7500},
	{1490.2380,-688.5726,94.7500},
	{1496.2780,-692.0362,94.7500},
	{1711.0038,-1100.2721,24.0781},
	{2765.8777,-387.3180,24.0012},
	{2766.7366,745.2954,10.8984},
	{2509.4263,1138.4695,14.2705},
	{2402.1475,1436.4952,15.8785},
	{2003.0099,1544.2897,13.5859},
	{1549.8978,1638.7437,10.8203},
	{1332.2155,1732.2617,10.8203},
	{1352.4668,1456.9900,10.8203},
	{1468.9412,1233.1780,10.8203},
	{1008.7559,1715.4512,10.7812},
	{982.1673,2565.6033,10.7362},
	{1050.7916,2914.4824,51.6622},
	{526.8539,2889.4993,6.8025},
	{297.2083,2597.2588,16.4766},
	{406.4179,2444.6936,16.5000},
	{-38.2261,2341.3547,24.1347},
	{-85.8060,2265.3003,125.1710},
	{-531.4526,1819.6948,138.2234},
	{-656.6006,1443.8218,13.6172},
	{-888.7077,1566.5927,25.9626},
	{-1536.8787,1001.0336,7.1875},
	{-1598.9923,659.5249,7.1875},
	{-1981.5541,266.9110,35.1719},
	{-2126.2427,148.9365,41.3019},
	{-2314.5317,101.1132,35.3984},
	{-2377.8179,-292.0763,59.3936},
	{-2126.6611,-275.7707,35.3203},
	{-1974.6483,-527.6316,35.3359},
	{-1620.1541,-665.3140,14.1440},
	{-1308.3027,-559.6769,14.1484},
	{-1154.6984,-208.9685,14.1484},
	{-1313.3345,102.6179,14.1484},
	{-596.4524,68.4913,43.9777},
	{-224.5625,-249.6571,1.4219},
	{171.7638,-14.3514,1.5781},
	{157.5365,-22.1955,1.5781},
	{324.5787,-198.7598,1.4920},
	{1122.4186,-287.2077,73.8129},
	{1559.1276,10.8809,23.9632},
	{1625.1815,581.4196,1.7578},
	{2155.0894,-107.1400,2.6539},
	{2265.7678,-72.5365,31.6016},
	{2326.8777,-80.2703,30.4834},
	{2541.8303,112.2299,26.4902},
	{2662.3743,-296.2203,14.1665},
	{1807.0404,-616.0289,77.0387},
	{1550.8274,-498.7066,54.6592},
	{1386.5234,-1114.2797,58.3991},
	{1301.4193,-1225.7799,37.9531},
	{-1054.5088, -603.6898, 96.7427},
	{-2666.5193, 244.3681, 14.3594},
	{-2809.4402, 1306.9690, 7.0988},
	{-2642.2285, 1414.5215, 23.8926},
	{-2383.6694, 1552.5946, 31.8533},
	{-2226.6655, 2348.1104, 4.9922},
	{-2444.4158, 2281.2566, 14.8993},
	{-2588.8020, 2363.0974, 20.1265},
	{-2276.7622, 2657.8733, 73.5156},
	{-1936.8396, 2381.2119, 49.5000},
	{-1803.7517, 2052.3096, 8.9615},
	{-1451.9252, 1868.9146, 32.6328},
	{-1056.9927, 1520.1509, 34.1801},
	{-7.4227, 1110.3088, 19.8172},
	{45.7702, 1213.4840, 18.9785},
	{162.4819, 1172.7228, 14.7661},
	{593.2874, 1225.0688, 11.7188},
	{623.0258, 893.0629, -35.3267},
	{1555.3420, 657.4982, 10.6719},
	{2603.6555, 728.9846, 10.8203},
	{2690.8113, 2786.4495, 59.0212},
	{2006.7072, 2911.8855, 51.6622},
	{-54.6163, 109.0044, 25.9801},
	{-460.3113, -51.1667, 59.9477},
	{-965.6066, -335.0024, 36.2742},
	{2169.8899, -1999.0215, 35.6163},
	{2390.2244, -2251.1938, 20.0573},
	{2598.4265, -1531.8612, 13.1181},
	{1775.3646, -1317.3688, 120.2004},
	{1834.1492, -1281.2981, 131.7344},
	{1526.5842, -1359.1451, 329.4609},
	{1535.2523, -1348.1117, 329.4544},
	{897.1519, -1257.6948, 49.3380},
	{1094.0325, -2037.0026, 82.7541},
	{2347.3511, -2109.9158, 39.0501},
	{2515.3931, -2073.1558, 49.2500},
	{1347.9032, -1556.3813, 85.5469},
	{387.3426, -2024.7762, 8.3496},
	{454.6707, -2271.9153, 12.1031},
	{306.4088, -1511.5515, 76.5362}
};

enum presentEnum {
	bool:presentUsed = false,
	presentModel,
	Float:presentX,
	Float:presentY,
	Float:presentZ,
	presentPickup,
	Text3D:presentLabel
};
new PresentInfo[MAX_PRESENTS][presentEnum];
new bool:tempPresent[MAX_PLAYERS];
new christmasTrack[MAX_PLAYERS];

Hook:chr_OnGameModeInit() {
	loadChristmasEvent();
	return 1;
}

Hook:chr_OnGameModeExit() {
	saveChristmasEvent();
	return 1;
}

Hook:chr_OnPlayerConnect(playerid) {
	tempPresent[playerid] = false;
	christmasTrack[playerid] = gettime();
	return 1;
}

CMD:christmascredits(playerid, params[]) {
	if(!christmasEnabled)
		return 1;

	SendClientMessage(playerid, COLOR_WHITE, "The christmas event 2018 was made by:");
	SendClientMessage(playerid, 0xF6FC4DFF, "Not Shadow :P");
	return 1;
}

CMD:christmashelp(playerid, params[]) {
	if(!christmasEnabled)
		return 1;

	SendClientMessage(playerid, 0xFF0000, "______________________{4D8906}______________________");

	new chrStr[128];
	format(chrStr, sizeof(chrStr), "** Find all %i presents for an awesome reward! **", PRESENTS_TO_FIND);
	SendClientMessage(playerid, COLOR_WHITE, chrStr);
	SendClientMessage(playerid, COLOR_WHITE, "** The presents are hidden all around the map! You can use /trackpresent for a hint! **");

	SendClientMessage(playerid, COLOR_WHITE, "Christmas commands: /christmashelp /getpresent /trackpresent /givepresent /christmascredits");

	if(isChristmasMod(playerid)) {
		SendClientMessage(playerid, COLOR_WHITE, "Christmas mod: /togglechristmas /gotopresent /allowreward /resetchristmas /christmasad");
	}
	return 1;
}

CMD:claimreward(playerid, params[]) {
	if(!christmasEnabled)
		return 1;

	if(PlayerInfo[playerid][pPresentCount] < PRESENTS_TO_FIND)
		return SendClientMessage(playerid, COLOR_GREY, "You have not completed the present hunt.");

	if(PlayerInfo[playerid][pPresentCount] == 99)
		return SendClientMessage(playerid, COLOR_GREY, "You have already claimed a reward!");

	new rewardID;
	if(sscanf(params, "i", rewardID)) {
		SendClientMessage(playerid, COLOR_GREY, "Usage: {FFFFFF}/claimreward (reward number)");
		ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Available rewards",
			"To claim your reward, use /claimreward (reward number)\n\n\
			{FFFFFF}[1] 20 Tikis\n\
			{FFFFFF}[2] 50g Cocaine & 10 tikis\n\
			{FFFFFF}[3] 100 Meth & 5 tikis\n\
			{FFFFFF}[4] 20k Materials\n\
			{FFFFFF}[5] Random stuff[100g cocaine or 200g cannabis or 100k mats or 500 meth]",
		"Close", "");
		return 1;
	}

	if(rewardID < 1 || rewardID > 5)
		return SendClientMessage(playerid, COLOR_GREY, "You picked an invalid reward!");

	PlayAudioStreamForPlayer(playerid, CHRISTMAS_SONG);

	switch(rewardID) {
		case 1: {
			PlayerInfo[playerid][pTikis] += 20;
			SendClientMessage(playerid, COLOR_PURPLE, "Reward: 20 tikis.");
		}
		case 2: {
			PlayerInfo[playerid][pTikis] += 10;
			PlayerInfo[playerid][pCocaine] += 50;
			SendClientMessage(playerid, COLOR_PURPLE, "Reward: 50g Cocaine and 10 tikis.");
		}
		case 3: {
			PlayerInfo[playerid][pTikis] += 5;
			PlayerInfo[playerid][pMeth] += 100;
			SendClientMessage(playerid, COLOR_PURPLE, "Reward: 100 Meth and 5 tikis.");
		}
		case 4: {
			PlayerInfo[playerid][pMats] += 20000;
			SendClientMessage(playerid, COLOR_PURPLE, "Reward: 20,000 Materials.");
		}
		case 5: {
			new randomWep = random(4);

			switch(randomWep) {
				case 0: {
					PlayerInfo[playerid][pCocaine] += 100;
					SendClientMessage(playerid, COLOR_PURPLE, "Reward: 100g Cocaine");
				}
				case 1: {
					PlayerInfo[playerid][pCannabis] += 200;
					SendClientMessage(playerid, COLOR_PURPLE, "Reward: 200g Cannabis");
				}
				case 2: {
					PlayerInfo[playerid][pMats] += 100000;
					SendClientMessage(playerid, COLOR_PURPLE, "Reward: 100,000 Materials");
				}
				case 3: {
					PlayerInfo[playerid][pMeth] += 500;
					SendClientMessage(playerid, COLOR_PURPLE, "Reward: 500 Meth");
				}
			}
		}
	}

	SendClientMessage(playerid, COLOR_PURPLE, "Thanks for participating in the Christmas Event 2017. :)");
	SendClientMessage(playerid, COLOR_PURPLE, "Created by Not Shadow :P");

	PlayerInfo[playerid][pPresentCount] = 99;

	ShowMerryChristmas(playerid, 1);
	return 1;
}

CMD:trackpresent(playerid, params[]) {
	if(!christmasEnabled)
		return 1;

	if(gettime() - christmasTrack[playerid] < 120)
		return SendClientMessage(playerid, COLOR_GREY, "You can use this once every two minutes.");

	new presID = random(MAX_PRESENTS), presLoc[MAX_ZONE_NAME];

	Get2DZone(presLoc, MAX_ZONE_NAME, PresentInfo[presID][presentX], PresentInfo[presID][presentY], PresentInfo[presID][presentZ]);
	new Float:presDist = GetPlayerDistanceFromPoint(playerid, PresentInfo[presID][presentX], PresentInfo[presID][presentY], PresentInfo[presID][presentZ]);

	new giftStr[128];
	format(giftStr, sizeof(giftStr), "A present was spotted in %s! (%im distance from you)", presLoc, floatround(presDist, floatround_ceil));
	SendClientMessage(playerid, COLOR_GREY, giftStr);

	christmasTrack[playerid] = gettime();
	return 1;
}

CMD:getpresent(playerid, params[]) {
	if(!christmasEnabled)
		return 1;

	if(tempPresent[playerid])
		return SendClientMessage(playerid, COLOR_GREY, "You are already holding a present. (/givepresent)");

	if(PlayerInfo[playerid][pPresentCount] >= PRESENTS_TO_FIND && PlayerInfo[playerid][pPresentCount] != 99)
		return SendClientMessage(playerid, COLOR_GREY, "Claim your reward first! /claimreward");

	new nearbyPres = getNearbyPresent(playerid, 3.0);
	if(nearbyPres == INVALID_PRESENT_ID)
		return SendClientMessage(playerid, COLOR_GREY, "You aren't near any present.");

	randomizePresent(nearbyPres);

	// Visual/sound
	OnePlayAnim(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
	PlayAudioStreamForPlayer(playerid, CHRISTMAS_SOUND);

	// Chat
	SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have found a present!");
	if(PlayerInfo[playerid][pPresentCount] <= PRESENTS_TO_FIND) {
		PlayerInfo[playerid][pPresentCount]++;
		new chrStr[128];
		format(chrStr, sizeof(chrStr), " [%i/%i] presents found. (/christmashelp)", PlayerInfo[playerid][pPresentCount], PRESENTS_TO_FIND);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, chrStr);

		if(PlayerInfo[playerid][pPresentCount] == PRESENTS_TO_FIND) {
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can now claim your Christmas reward! /claimreward");
		}
	}
	else {
		tempPresent[playerid] = true;
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have already claimed your reward, you can now do other players a favor!");
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You can gift this present to players who have not claimed their reward yet! (/givepresent)");
	}
	return 1;
}

CMD:givepresent(playerid, params[]) {
	if(!christmasEnabled)
		return 1;

	if(PlayerInfo[playerid][pPresentCount] <= PRESENTS_TO_FIND)
		return SendClientMessage(playerid, COLOR_GREY, "You can only do this after claiming your reward. (/claimreward)");

	if(!tempPresent[playerid])
		return SendClientMessage(playerid, COLOR_GREY, "You don't have any present on you.");

	new targetPl;
	if(sscanf(params, "u", targetPl))
		return SendClientMessage(playerid, COLOR_GREY, "Usage: /givepresent (player)");

	if(!IsPlayerConnected(targetPl))
		return SendClientMessage(playerid, COLOR_GREY, "Unknown player!");

	if(targetPl == playerid)
		return cmd_ame(playerid, "gives themselves a present.");

	if(!IsPlayerInRangeOfPlayer(2.0, playerid, targetPl))
		return SendClientMessage(playerid, COLOR_GREY, "You are not near this player.");

	if(PlayerInfo[targetPl][pPresentCount] >= PRESENTS_TO_FIND)
		return SendClientMessage(playerid, COLOR_GREY, "This player doesn't need your gift.");

	if(IsPlayerInAnyVehicle(playerid) || IsPlayerInAnyVehicle(targetPl))
		return SendClientMessage(playerid, COLOR_GREY, "You and the player should not be in a vehicle.");

	SetPlayerFacingPlayer(playerid, targetPl);
	SetPlayerFacingPlayer(targetPl, playerid);
	OnePlayAnim(playerid, "KISSING", "gift_give", 4.0, 0, 0, 0, 0, 0);

	SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have gifted that player a present! +$15,000 for karma!");
	SendClientMessage(targetPl, COLOR_LIGHTBLUE, "* You have received a present from a player!");

	new giftStr[128];
	format(giftStr, sizeof(giftStr), "* %s hides something behind their back before they reveal a gift, hands it over to %s.", PlayerICName(playerid), PlayerICName(targetPl));
	ProxDetector(30.0, playerid, giftStr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    tempPresent[playerid] = false;
    PlayerInfo[playerid][pPresentCount] = 99;
	PlayerInfo[targetPl][pPresentCount]++;

	if(PlayerInfo[targetPl][pPresentCount] == PRESENTS_TO_FIND) {
		SendClientMessage(targetPl, COLOR_LIGHTBLUE, "* You have completed the Christmas event, thanks to the gift! /claimreward");
	}
	return 1;
}

CMD:christmasad(playerid, params[]) {
	if(!isChristmasMod(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to do this.");

	SendClientMessageToAll(TEAM_GROVE_COLOR, "Advertisement [Ph: 555] Ho ho ho! Merry Christmas! Have you found all presents yet?");
	SendClientMessageToAll(TEAM_GROVE_COLOR, "(( /christmashelp ))");
	return 1;
}

CMD:togglechristmas(playerid, params[]) {
	if(!isChristmasMod(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to do this.");

	if(christmasEnabled) {
		christmasEnabled = 0;
		destroyAllPresents();
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "Christmas event disabled!");
	}
	else {
		christmasEnabled = 1;
		randomizeAllPresents();
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "Christmas event enabled!");
	}
	return 1;
}

CMD:allowreward(playerid, params[]) {
	if(!isChristmasMod(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to do this.");

	if(!christmasEnabled)
		return SendClientMessage(playerid, COLOR_GREY, "Christmas event is disabled. (/togglechristmas)");

	new targetPl;
	if(sscanf(params, "u", targetPl))
		return SendClientMessage(playerid, COLOR_GREY, "Usage: /allowreward (player)");

	if(!IsPlayerConnected(targetPl))
		return SendClientMessage(playerid, COLOR_GREY, "Unknown player.");

	PlayerInfo[targetPl][pPresentCount] = PRESENTS_TO_FIND;
	SendClientMessage(playerid, COLOR_LIGHTBLUE, "That player can now /claimreward.");
	SendClientMessage(targetPl, COLOR_LIGHTBLUE, "A christmas mod has refunded your reward. (/claimreward)");
	return 1;
}

CMD:resetchristmas(playerid, params[]) {
	if(!isChristmasMod(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to do this.");

	if(!christmasEnabled)
		return SendClientMessage(playerid, COLOR_GREY, "Christmas event is disabled. (/togglechristmas)");

	new targetPl;
	if(sscanf(params, "u", targetPl))
		return SendClientMessage(playerid, COLOR_GREY, "Usage: /resetchristmas (player)");

	if(!IsPlayerConnected(targetPl))
		return SendClientMessage(playerid, COLOR_GREY, "Unknown player.");

	PlayerInfo[targetPl][pPresentCount] = 0;
	SendClientMessage(playerid, COLOR_LIGHTBLUE, "The christmas event progress of this player has been reset!");
	return 1;
}

CMD:gotopresent(playerid, params[]) {
	if(!isChristmasMod(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to do this.");

	if(!christmasEnabled)
		return SendClientMessage(playerid, COLOR_GREY, "Christmas event is disabled. (/togglechristmas)");

	new presID;
	if(sscanf(params, "i", presID))
		return SendClientMessage(playerid, COLOR_GREY, "Usage: /gotopresent (id)");

	if(presID < 0 || presID > MAX_PRESENTS - 1)
		return SendClientMessage(playerid, COLOR_GREY, "Invalid present ID!");

	if(!PresentInfo[presID][presentUsed])
		return SendClientMessage(playerid, COLOR_GREY, "This present ID is not occupied at the moment.");

	SetPlayerPos(playerid, PresentInfo[presID][presentX], PresentInfo[presID][presentY], PresentInfo[presID][presentZ]);

	new chrStr[128];
	format(chrStr, sizeof(chrStr), "Teleported to present: [%i]", presID);
	SendClientMessage(playerid, -1, chrStr);
	return 1;
}

forward ShowMerryChristmas(playerid, stage);
public ShowMerryChristmas(playerid, stage) {
	switch(stage) {
		case 1:
			GameTextForPlayer(playerid, "~g~M", 1000, 4);
		case 2:
			GameTextForPlayer(playerid, "~g~M~r~E", 1000, 4);
		case 3:
			GameTextForPlayer(playerid, "~g~M~r~E~g~R", 1000, 4);
		case 4:
			GameTextForPlayer(playerid, "~g~M~r~E~g~R~r~R", 1000, 4);
		case 5:
			GameTextForPlayer(playerid, "~g~M~r~E~g~R~r~R~g~Y", 1000, 4);
		case 6:
			GameTextForPlayer(playerid, "~g~M~r~E~g~R~r~R~g~Y ~r~C", 1000, 4);
		case 7:
			GameTextForPlayer(playerid, "~g~M~r~E~g~R~r~R~g~Y ~r~C~g~H", 1000, 4);
		case 8:
			GameTextForPlayer(playerid, "~g~M~r~E~g~R~r~R~g~Y ~r~C~g~H~r~R", 1000, 4);
		case 9:
			GameTextForPlayer(playerid, "~g~M~r~E~g~R~r~R~g~Y ~r~C~g~H~r~R~g~I", 1000, 4);
		case 10:
			GameTextForPlayer(playerid, "~g~M~r~E~g~R~r~R~g~Y ~r~C~g~H~r~R~g~I~r~S", 1000, 4);
		case 11:
			GameTextForPlayer(playerid, "~g~M~r~E~g~R~r~R~g~Y ~r~C~g~H~r~R~g~I~r~S~g~T", 1000, 4);
		case 12:
			GameTextForPlayer(playerid, "~g~M~r~E~g~R~r~R~g~Y ~r~C~g~H~r~R~g~I~r~S~g~T~r~M", 1000, 4);
		case 13:
			GameTextForPlayer(playerid, "~g~M~r~E~g~R~r~R~g~Y ~r~C~g~H~r~R~g~I~r~S~g~T~r~M~g~A", 1000, 4);
		case 14:
			GameTextForPlayer(playerid, "~g~M~r~E~g~R~r~R~g~Y ~r~C~g~H~r~R~g~I~r~S~g~T~r~M~g~A~r~S", 1000, 4);
		case 15:
			GameTextForPlayer(playerid, "", 1000, 4);
		case 16:
			GameTextForPlayer(playerid, "~g~M~r~E~g~R~r~R~g~Y ~r~C~g~H~r~R~g~I~r~S~g~T~r~M~g~A~r~S", 1000, 4);
		case 17:
			GameTextForPlayer(playerid, "", 1000, 4);
		case 18:
			GameTextForPlayer(playerid, "~g~M~r~E~g~R~r~R~g~Y ~r~C~g~H~r~R~g~I~r~S~g~T~r~M~g~A~r~S", 1000, 4);
		case 19:
			GameTextForPlayer(playerid, "", 1000, 4);
		case 20:
			GameTextForPlayer(playerid, "~g~M~r~E~g~R~r~R~g~Y ~r~C~g~H~r~R~g~I~r~S~g~T~r~M~g~A~r~S", 1000, 4);
		case 21:
			GameTextForPlayer(playerid, "~g~M~r~E~g~R~r~R~g~Y ~r~C~g~H~r~R~g~I~r~S~g~T~r~M~g~A~r~S~n~~w~from Shadow!", 3000, 4);
	}

	if(stage < 21) {
		SetTimerEx("ShowMerryChristmas", 500, false, "ii", playerid, stage + 1);
	}
	return 1;
}

getNearbyPresent(playerid, Float:range) {
	if(GetPlayerVirtualWorld(playerid) != 0 || GetPlayerInterior(playerid) != 0)
		return INVALID_PRESENT_ID;

	for(new i = 0; i < MAX_PRESENTS; i++) {
		if(PresentInfo[i][presentUsed]) {
			if(IsPlayerInRangeOfPoint(playerid, range, PresentInfo[i][presentX], PresentInfo[i][presentY], PresentInfo[i][presentZ]))
				return i;
		}
	}
	return INVALID_PRESENT_ID;
}

loadChristmasEvent() {
	if(fexist(CHRISTMAS_FILE)) {
		christmasEnabled = dini_Int(CHRISTMAS_FILE, "christmasEnabled");
	}

	if(christmasEnabled) randomizeAllPresents();
	return 1;
}

saveChristmasEvent() {
	if(!fexist(CHRISTMAS_FILE))
		dini_Create(CHRISTMAS_FILE);

	dini_IntSet(CHRISTMAS_FILE, "christmasEnabled", christmasEnabled);
	return 1;
}

isChristmasMod(playerid) {
	for(new i = 0; i < sizeof(christmasAuthorized); i++) {
		if(PlayerInfo[playerid][pID] == christmasAuthorized[i])
			return 1;
	}
	return 0;
}

randomizeAllPresents() {
	for(new i = 0; i < MAX_PRESENTS; i++) {
		randomizePresent(i);
	}
	return 1;
}

destroyAllPresents() {
	for(new i = 0; i < MAX_PRESENTS; i++) {
		destroyPresent(i);
	}
	return 1;
}

destroyPresent(presentID) {
	PresentInfo[presentID][presentUsed] = false;
	DestroyDynamicPickup(PresentInfo[presentID][presentPickup]);
	DestroyDynamic3DTextLabel(PresentInfo[presentID][presentLabel]);
	return 1;
}

randomizePresent(presentID) {
	if(PresentInfo[presentID][presentUsed]) {
		destroyPresent(presentID);
	}

	PresentInfo[presentID][presentUsed] = true;

	// Get a random present type
	new pickedType = random(sizeof(presentTypes));

	// Set the location
	new pickedLocation = random(sizeof(presentLocations));

	while(locationHasPresent(pickedLocation)) {
		pickedLocation = random(sizeof(presentLocations));
	}

	PresentInfo[presentID][presentX] = presentLocations[pickedLocation][0];
	PresentInfo[presentID][presentY] = presentLocations[pickedLocation][1];
	PresentInfo[presentID][presentZ] = presentLocations[pickedLocation][2];

	// Set the model and create pickup
	PresentInfo[presentID][presentModel] = presentTypes[pickedType][0];
	PresentInfo[presentID][presentPickup] = CreateDynamicPickup(PresentInfo[presentID][presentModel], 1, PresentInfo[presentID][presentX], PresentInfo[presentID][presentY], PresentInfo[presentID][presentZ]);

	// Create the label
	new presentStr[300];
	format(presentStr, sizeof(presentStr), "%s\n{FFFFFF}(( /getpresent ))", presentTypes[pickedType][1]);
	PresentInfo[presentID][presentLabel] = CreateDynamic3DTextLabel(presentStr, 0x00EEFFFF, PresentInfo[presentID][presentX], PresentInfo[presentID][presentY], PresentInfo[presentID][presentZ], 20.0);

	return presentID;
}

locationHasPresent(locationID) {
	for(new i = 0; i < MAX_PRESENTS; i++) {
		if(PresentInfo[i][presentUsed]) {
			if(
				PresentInfo[i][presentX] == presentLocations[locationID][0] &&
				PresentInfo[i][presentY] == presentLocations[locationID][1] &&
				PresentInfo[i][presentZ] == presentLocations[locationID][2]
			) return 1;
		}
	}
	return 0;
}