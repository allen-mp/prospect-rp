ShowStats(playerid, targetid, bool:bPrint = false)
{
	new line1[144], line2[144], line3[144], line4[144], line5[144], line6[144], line7[144], line8[144], line9[144], line10[144], line11[144], line12[144];

	// Business
	new btext[40];
	new biz = PlayerInfo[targetid][pBizKey];
	if(biz != -1) format(btext, sizeof(btext), "%s (%d)", BizInfo[biz][bName], biz);
	else btext = "None";

	// Gender
	new atext[8];
	if(PlayerInfo[targetid][pSex] == 1) atext = "Male";
	else if(PlayerInfo[targetid][pSex] == 2) atext = "Female";
	else atext = "Unknown";

	// Level cost
	new nxtlevel = PlayerInfo[targetid][pLevel] +1;
	new costlevel = nxtlevel * levelcost;
	new expamount = nxtlevel * levelexp;

	//Job name & Level
	new jtext[20];
	new jlevel;
	if(PlayerInfo[targetid][pJob] == 1)
	{
		jtext = "Detective";
		new level = PlayerInfo[targetid][pDetSkill];
		if(level >= 0 && level <= 49) jlevel = 1;
		else if(level >= 50 && level <= 99) jlevel = 2;
		else if(level >= 100 && level <= 199) jlevel = 3;
		else if(level >= 200 && level <= 399) jlevel = 4;
		else if(level >= 400) jlevel = 5;
	}
	else if(PlayerInfo[targetid][pJob] == 2)
	{
		jtext = "Lawyer";
		new level = PlayerInfo[targetid][pLawSkill];
		if(level >= 0 && level <= 49) jlevel = 1;
		else if(level >= 50 && level <= 99) jlevel = 2;
		else if(level >= 100 && level <= 199) jlevel = 3;
		else if(level >= 200 && level <= 399) jlevel = 4;
		else if(level >= 400) jlevel = 5;
	}
	else if(PlayerInfo[targetid][pJob] == 4)
	{
		jtext = "Drug Dealer";
		new level = PlayerInfo[targetid][pDrugsSkill];
		if(level >= 0 && level <= 49) jlevel = 1;
		else if(level >= 50 && level <= 99) jlevel = 2;
		else if(level >= 100 && level <= 199) jlevel = 3;
		else if(level >= 200 && level <= 399) jlevel = 4;
		else if(level >= 400) jlevel = 5;
	}
	else if(PlayerInfo[targetid][pJob] == 5)
	{
		jtext = "Product Dealer";
		jlevel = 1;
	}
	else if(PlayerInfo[targetid][pJob] == 7)
	{
		jtext = "Mechanic";
		new level = PlayerInfo[targetid][pMechSkill];
		if(level >= 0 && level <= 49) jlevel = 1;
		else if(level >= 50 && level <= 99) jlevel = 2;
		else if(level >= 100 && level <= 199) jlevel = 3;
		else if(level >= 200 && level <= 399) jlevel = 4;
		else if(level >= 400) jlevel = 5;

	}
	else if(PlayerInfo[targetid][pJob] == 8)
	{
		jtext = "Bodyguard";
		jlevel = 1;
	}
	else if(PlayerInfo[targetid][pJob] == 9)
	{
		jtext = "Arms Dealer";
		new level = PlayerInfo[targetid][pArmsSkill];
		if(level >= 0 && level <= 49) jlevel = 1;
		else if(level >= 50 && level <= 99) jlevel = 2;
		else if(level >= 100 && level <= 199) jlevel = 3;
		else if(level >= 200 && level <= 399) jlevel = 4;
		else if(level >= 400) jlevel = 5;
	}
	else if(PlayerInfo[targetid][pJob] == 12)
	{
		jtext = "Boxer";
		jlevel = 1;
	}
	else if(PlayerInfo[targetid][pJob] == 14)
	{
		jtext = "Taxi Driver";
		jlevel = 1;
	}
	else if(PlayerInfo[targetid][pJob] == 17)
	{
		jtext = "Drug Smuggler";
		new level = PlayerInfo[targetid][pSmugglerSkill];
		if(level >= 0 && level <= 19) jlevel = 1;
		else if(level >= 20 && level <= 39) jlevel = 2;
		else if(level >= 40 && level <= 59) jlevel = 3;
		else if(level >= 60 && level <= 119) jlevel = 4;
		else if(level >= 120) jlevel = 5;
	}
	else if(PlayerInfo[targetid][pJob] == 19)
	{
		jtext = "Waste Collector";
		new level = PlayerInfo[targetid][pWasteSkill];
		if(level >= 0 && level <= 49) jlevel = 1;
		else if(level >= 50 && level <= 99) jlevel = 2;
		else if(level >= 100 && level <= 199) level = 3;
		else if(level >= 200 && level <= 399) jlevel = 4;
		else if(level >= 400) jlevel = 5;
	}
	else if(PlayerInfo[targetid][pJob] == 21)
	{
		jtext = "Pizza Deliver";
		jlevel = GetPlayerPizzaLevel(playerid);
	}
	else
	{
		jtext = "None";
		jlevel = 0;
	}
	// VIP
	new vip[20];
	if(PlayerInfo[targetid][pDonateRank] == 0) vip = "None";
	else if(PlayerInfo[targetid][pDonateRank] == 1) vip = "Bronze";
	else if(PlayerInfo[targetid][pDonateRank] == 2) vip = "Silver";
	else if(PlayerInfo[targetid][pDonateRank] == 3) vip = "Gold";

	// House
	new htext[40]; // House
	if(PlayerInfo[targetid][pHouseID] != 0)
	{
		new location[MAX_ZONE_NAME];
		Get2DZone(location, MAX_ZONE_NAME, HouseInfo[PlayerInfo[targetid][pHouseID]][hOutsideX], HouseInfo[PlayerInfo[targetid][pHouseID]][hOutsideY], HouseInfo[PlayerInfo[targetid][pHouseID]][hOutsideZ]);
		format(htext, sizeof(htext), "%d %s", PlayerInfo[targetid][pHouseID], location);
	}
	else htext = "None";


	// Inventory
	new rod[20];
	switch(PlayerInfo[targetid][pFishRod])
	{
		case 0:
			rod = "None";
		case 1:
			rod = "Old Rod";
		case 2:
			rod = "Beginners Rod";
		case 3:
			rod = "Fishermans Rod";
		case 4:
			rod = "Elite Rod";
	}
	new bait[20];
	switch(PlayerInfo[targetid][pBait])
	{
		case 0:
			bait = "None";
		case 1:
			bait = "Weak Bait";
		case 2:
			bait = "Regular Bait";
		case 3:
			bait = "Strong Bait";
		case 4:
			bait = "Elite Bait";
	}
	new baita = PlayerInfo[targetid][pBaitAmount];
	if(baita == 0)
	{
		bait = "None";
	}
	new line[20];
	switch(PlayerInfo[targetid][pLine])
	{
		case 0:
			line = "None";
		case 1:
			line = "Regular Line";
	}

	new groupname[24], grouprank[24];
	if(PlayerInfo[targetid][pGroup] == GROUP_S9 || PlayerInfo[playerid][pGroup] == GROUP_S9) {
		groupname = "None";
		grouprank = "None";
	} else {
		groupname = GetPlayerGroupName(targetid);
		grouprank = GetPlayerGroupRankName(targetid);
	}
	
	//Formatting information into strings
	//phone check
	if(PlayerInfo[targetid][pPnumber] == 0) format(line1, sizeof(line1), "[Account] {B4B5B7}ID:[%d]  Name:[%s]  Sex:[%s]  Level:[%d]  Respect:[%d/%d]  Phone:[None]", PlayerInfo[targetid][pID], PlayerName(targetid), atext, PlayerInfo[targetid][pLevel], PlayerInfo[targetid][pExp], expamount);
	else format(line1, sizeof(line1), "[Account] {B4B5B7}ID:[%d]  Name:[%s]  Sex:[%s]  Level:[%d]  Respect:[%d/%d]  Phone:[%d]", PlayerInfo[targetid][pID], PlayerName(targetid), atext, PlayerInfo[targetid][pLevel], PlayerInfo[targetid][pExp], expamount, PlayerInfo[targetid][pPnumber]);
	format(line2, sizeof(line2), "[Details] {B4B5B7}Money:[$%s]  Bank:[$%s]  Next Level Cost:[$%s]  Job:[%s]  Job Level:[%d]", AddThousandsSeparators(PlayerInfo[targetid][pCash]), AddThousandsSeparators(PlayerInfo[targetid][pAccount]), AddThousandsSeparators(costlevel), jtext, jlevel);
	format(line3, sizeof(line3), "[Details] {BFC0C2}Married To:[%s]  Affiliation:[%s]  Rank:[%s (%d)] Group:[%s]  Group Rank:[%s (%d)]", PlayerInfo[targetid][pMarriedTo], GetPlayerAffiliationName(targetid), GetPlayerRank(targetid), PlayerInfo[targetid][pRank], groupname, grouprank, PlayerInfo[targetid][pGroupRank]);
	format(line4, sizeof(line4), "[Upgrades] {BFC0C2}Upgrade Points:[%d]  Armor Upgrade:[%d]  Health Upgrade:[%d] VIP:[%s] Gear Tickets[%d]",PlayerInfo[targetid][gPupgrade],PlayerInfo[targetid][pSHealth],PlayerInfo[targetid][pSHP], vip, PlayerInfo[targetid][gTicket]);
	format(line5, sizeof(line5), "[Property] {CBCCCE}House:[%s] Business:[%s]", htext, btext);
	format(line6, sizeof(line6), "[Records] {D8D8D8}Playing Hours:[%d]  Kills:[%d]  Deaths:[%d]", PlayerInfo[targetid][pConnectTime], PlayerInfo[targetid][pKills], PlayerInfo[targetid][pDeaths]);
	format(line7, sizeof(line7), "[Records] {D8D8D8}Biggest Fish:[%d]", PlayerInfo[targetid][pBiggestFish]);
	format(line8, sizeof(line8), "[Inventory] {CBCCCE}Materials:[%d]  Packages:[%d]  Crates:[%d]  Seeds:[%d]  Products:[%d]", PlayerInfo[targetid][pMats], Packages[targetid], Crates[targetid], PlayerInfo[targetid][pPotSeeds], PlayerInfo[targetid][pProducts]);
	format(line9, sizeof(line9), "[Inventory] {D8D8D8}Ropes:[%d]  Blindfolds:[%d]  Cigars:[%d]  Sprunk:[%d]  Spraycan:[%d]  Screw Driver:[%d]  Deck of Cards:[%d]  WTc:[%d]", PlayerInfo[targetid][pRope], PlayerInfo[targetid][pBlindfolds], PlayerInfo[targetid][pCigars], PlayerInfo[targetid][pSprunk], PlayerInfo[targetid][pSpraycan], PlayerInfo[targetid][pScrew], PlayerInfo[targetid][pDeck], PlayerInfo[targetid][pWTc]);
	format(line10, sizeof(line10), "[Inventory] {D8D8D8}Fishing Rod:[%s]  Bait:[%s]  Bait Left:[%d]  Fishing Line:[%s] Boombox:[%d] Box of Matches: [%d] Jerry Can: [%d]", rod, bait, baita, line, PlayerInfo[targetid][pStereo], PlayerInfo[targetid][pMatches], PlayerInfo[targetid][pJerrycan]);
	format(line11, sizeof(line11), "[Admin Info] {E3E3E3}Vehicle 1 ID:[%d]  Vehicle 1 TID:[%d]  Vehicle 2 ID:[%d]  Vehicle 2 TID:[%d]", 0, 0, 0, 0);
	format(line12, sizeof(line12), "[Admin Info] {E3E3E3}Int:[%d]  VW:[%d]  Warnings:[%d]  JailTime:[%d]  Tikis:[%d]  Skin:[%d]", GetPlayerInterior(targetid), GetPlayerVirtualWorld(targetid), PlayerInfo[targetid][pWarns], PlayerInfo[targetid][pJailTime], PlayerInfo[targetid][pTikis], GetPlayerSkin(targetid));

	if(bPrint)
	{
		SendClientMessage(playerid, 0x007BD0FF, line1);
		SendClientMessage(playerid, 0x007BD0FF, line2);
		SendClientMessage(playerid, 0x007BD0FF, line3);
		SendClientMessage(playerid, 0x007BD0FF, line4);
		SendClientMessage(playerid, 0x007BD0FF, line5);
		SendClientMessage(playerid, 0x007BD0FF, line6);
		SendClientMessage(playerid, 0x007BD0FF, line7);
		SendClientMessage(playerid, 0x007BD0FF, line8);
		SendClientMessage(playerid, 0x007BD0FF, line9);
		SendClientMessage(playerid, 0x007BD0FF, line10);

		if(PlayerInfo[playerid][pAdmin] >= 2)
		{
			SendClientMessage(playerid, 0x007BD0FF, line11);
			SendClientMessage(playerid, 0x007BD0FF, line12);
		}
	}
	else
	{
		new mstring[1536];
		if(PlayerInfo[playerid][pAdmin] <= 1) format(mstring, sizeof(mstring), "{007BD0}%s\n{007BD0}%s\n{007BD0}%s\n{007BD0}%s\n{007BD0}%s\n{007BD0}%s\n{007BD0}%s\n{007BD0}%s\n{007BD0}%s\n{007BD0}%s", line1, line2, line3, line4, line5, line6, line7, line8, line9, line10);
		else format(mstring, sizeof(mstring), "{007BD0}%s\n{007BD0}%s\n{007BD0}%s\n{007BD0}%s\n{007BD0}%s\n{007BD0}%s\n{007BD0}%s\n{007BD0}%s\n{007BD0}%s\n{007BD0}%s\n{007BD0}%s\n{007BD0}%s", line1, line2, line3, line4, line5, line6, line7, line8, line9, line10, line11, line12);

		new titlestr[64];
		format(titlestr, sizeof(titlestr), "{007BD0}Account Details: {B4B5B7}%s", PlayerName(targetid));
		ShowPlayerDialog(playerid, DIALOG_STATS, DIALOG_STYLE_MSGBOX, titlestr, mstring, "Done", "Print");
	}
	return 1;
}