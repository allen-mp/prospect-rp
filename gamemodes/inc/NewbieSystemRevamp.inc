stock GetNewbieTag(playerid)
{
	new str[128];
	if( PlayerInfo[ playerid ] [ pTag ] == 1 && PlayerInfo[playerid][pLevel] < 2 ) str = "Newbie";
	else if( PlayerInfo[ playerid ] [ pTag ] == 2 && PlayerInfo[playerid][pLevel] > 1 ) str = "Player";
	else if( PlayerInfo[ playerid ] [ pTag ] == 3 && PlayerInfo[playerid][pHelper] == 1 ) str = "Trial Helper";
	else if( PlayerInfo[ playerid ] [ pTag ] == 4 && PlayerInfo[playerid][pHelper] == 2 ) str = "Helper";
	else if( PlayerInfo[ playerid ] [ pTag ] == 5 && PlayerInfo[playerid][pHelper] == 3 ) str = "Senior Helper";
	else if( PlayerInfo[ playerid ] [ pTag ] == 6 && PlayerInfo[playerid][pHelper] == 4 ) str = "Head Helper";
	else if( PlayerInfo[ playerid ] [ pTag ] == 7 && PlayerInfo[playerid][pHelper] == 5 ) str = "Director Of The Helper Team";
	else if( PlayerInfo[ playerid ] [ pTag ] == 8 && PlayerInfo[playerid][pAdmin] == 1) str = "Probationary Administrator";
	else if( PlayerInfo[ playerid ] [ pTag ] == 9 && PlayerInfo[playerid][pAdmin] == 2) str = "Junior Administrator";
	else if( PlayerInfo[ playerid ] [ pTag ] == 10 && PlayerInfo[playerid][pAdmin] == 3) str = "General Administrator";
	else if( PlayerInfo[ playerid ] [ pTag ] == 11 && PlayerInfo[playerid][pAdmin] == 4) str = "Senior Administrator";
	else if( PlayerInfo[ playerid ] [ pTag ] == 12 && PlayerInfo[playerid][pAdmin] >= 1337 && PlayerInfo[playerid][pAdmin] >= 1337 && PlayerInfo[playerid][pAdmin] <= 99998 ) str = "Head Administrator";
	else if( PlayerInfo[ playerid ] [ pTag ] == 13 && PlayerInfo[playerid][pAdmin] >= 99999 ) str = "Executive Administrator";
	else if( PlayerInfo[ playerid ] [ pTag ] == 14 && PlayerInfo[playerid][pAdmin] == 1999999998 ) str = "Co-Owner";
	else if( PlayerInfo[ playerid ] [ pTag ] == 15 && PlayerInfo[playerid][pAdmin] == 133777 ) str = "Server Management";
	else if( PlayerInfo[ playerid ] [ pTag ] == 16 && PlayerInfo[playerid][pTester] == TESTERRANK_REGULAR ) str = "Regular Tester";
	else if( PlayerInfo[ playerid ] [ pTag ] == 17 && PlayerInfo[playerid][pTester] == TESTERRANK_SENIOR ) str = "Senior Tester";
	else if( PlayerInfo[ playerid ] [ pTag ] == 18 && PlayerInfo[playerid][pTester] == TESTERRANK_MANAGER ) str = "Tester Manager";
	else if( PlayerInfo[ playerid ] [ pTag ] == 19 && PlayerInfo[playerid][pDev] == 1 ) str = "Junior Developer";
	else if( PlayerInfo[ playerid ] [ pTag ] == 20 && PlayerInfo[playerid][pDev] == 2 ) str = "Senior Developer";
	else if( PlayerInfo[ playerid ] [ pTag ] == 21 && PlayerInfo[playerid][pDev] == 3 ) str = "Developer Manager";
	else if( PlayerInfo[ playerid ] [ pTag ] == 22 && PlayerInfo[playerid][pDev] == 4 ) str = "Developer Leader";
	else if( PlayerInfo[ playerid ] [ pTag ] == 23 && PlayerInfo[playerid][pMapper] == 1 ) str = "Regular Mapper";
	else if( PlayerInfo[ playerid ] [ pTag ] == 24 && PlayerInfo[playerid][pMapper] == 2 ) str = "Senior Mapper";
	else if( PlayerInfo[ playerid ] [ pTag ] == 25 && PlayerInfo[playerid][pMapper] == 3 ) str = "Mapping Manager";
	else if( PlayerInfo[ playerid ] [ pTag ] == 26 && PlayerInfo[ playerid ] [ pAdmin ] > 1) str = CustomTag[playerid];
	else if( PlayerInfo[ playerid ] [ pTag ] == 27 && PlayerInfo[playerid][pFamed] == 1 ) str = "Famed";
	else if( PlayerInfo[ playerid ] [ pTag ] == 28 && PlayerInfo[playerid][pFamed] == 2 ) str = "Famed Manager";
	else if( PlayerInfo[ playerid ] [ pTag ] == 29 && PlayerInfo[playerid][pOldSchool] == 1 ) str = "Old School Member";
	else if( PlayerInfo[ playerid ] [ pTag ] == 30 && PlayerInfo[playerid][pOldSchool] == 2 ) str = "Old School Moderator";
	else if( PlayerInfo[ playerid ] [ pTag ] == 31 && PlayerInfo[playerid][pGFX] == 1 ) str = "Designer";
	else if( PlayerInfo[ playerid ] [ pTag ] == 32 && PlayerInfo[playerid][pGFX] == 2 ) str = "Video Editor";
	else if( PlayerInfo[ playerid ] [ pTag ] == 33 && PlayerInfo[playerid][pGFX] == 3 ) str = "GFX Moderator";
	else if( PlayerInfo[ playerid ] [ pTag ] == 34 && PlayerInfo[playerid][pGFX] == 4 ) str = "GFX Leader";
	else str ="Player";
	return str;
}

CMD:newbietag(playerid, params[])
{
	new str[200];
	if(isnull(params))
	{
		SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /newbietag <Tag>");
		SendClientMessage(playerid, COLOR_GREY, "Tags: Newbie, Player, Helper, Admin, Tester, Developer, Mapper, Famed, OldSchool, Designer");
	}
	else if(!strcmp(params,"newbie",true))
	{
		if(PlayerInfo[playerid][pLevel] > 1)
		{
			format(str, sizeof(str), "You're not a newbie, your level is %d", PlayerInfo[playerid][pLevel]);
			SendClientMessage(playerid, COLOR_GREY, str);
		}
		else
		{
			PlayerInfo[playerid][pTag] = 1;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Newbie");
		}
	}
	else if(!strcmp(params,"player",true))
	{
		PlayerInfo[playerid][pTag] = 2;
		SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Player");
	}
	else if(!strcmp(params,"helper",true))
	{
		if(PlayerInfo[playerid][pHelper] < 1) SendClientMessage(playerid, COLOR_GREY, "You're not a helper!");
		else if(PlayerInfo[playerid][pHelper] == 1)
		{
			PlayerInfo[playerid][pTag] = 3;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Trial Helper");
		}
		else if(PlayerInfo[playerid][pHelper] == 2)
		{
			PlayerInfo[playerid][pTag] = 4;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Helper");
		}
		else if(PlayerInfo[playerid][pHelper] == 3)
		{
			PlayerInfo[playerid][pTag] = 5;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Senior Helper");
		}
		else if(PlayerInfo[playerid][pHelper] == 4)
		{
			PlayerInfo[playerid][pTag] = 6;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Head Helper");
		}
		else if(PlayerInfo[playerid][pHelper] == 5)
		{
			PlayerInfo[playerid][pTag] = 7;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Director Of The Helper Team");
		}
	}
	else if(!strcmp(params,"admin",true))
	{
		if(PlayerInfo[playerid][pAdmin] < 1) SendClientMessage(playerid, COLOR_GREY, "You're not an admin!");
		else if(PlayerInfo[playerid][pAdmin] == 1)
		{
			PlayerInfo[playerid][pTag] = 8;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Probationary Administrator");
		}
		else if(PlayerInfo[playerid][pAdmin] == 2)
		{
			PlayerInfo[playerid][pTag] = 9;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Junior Administrator");	
		}
		else if(PlayerInfo[playerid][pAdmin] == 3)
		{
			PlayerInfo[playerid][pTag] = 10;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to General Administrator");
		}
		else if(PlayerInfo[playerid][pAdmin] > 3 && PlayerInfo[playerid][pAdmin] < 1337)
		{
			PlayerInfo[playerid][pTag] = 11;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Senior Administrator");
		}
		else if(PlayerInfo[playerid][pAdmin] > 1336 && PlayerInfo[playerid][pAdmin] < 99999)
		{
			PlayerInfo[playerid][pTag] = 12;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Head Administrator");
		}
		else if(PlayerInfo[playerid][pAdmin] > 99998 && PlayerInfo[playerid][pAdmin] < 199999999)
		{
			PlayerInfo[playerid][pTag] = 13;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Executive Administrator");	
		}
		else if(PlayerInfo[playerid][pAdmin] > 199999998 && PlayerInfo[playerid][pAdmin] < 1999999999)
		{
			PlayerInfo[playerid][pTag] = 14;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Co-Owner");	
		}
		else if(PlayerInfo[playerid][pAdmin] >= 1999999999)
		{
			PlayerInfo[playerid][pTag] = 15;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Owner");	
		}
	}
	else if(!strcmp(params,"tester",true))
	{
		if(PlayerInfo[playerid][pTester] < TESTERRANK_REGULAR) SendClientMessage(playerid, COLOR_GREY, "You are not a tester!");
		else if(PlayerInfo[playerid][pTester] == TESTERRANK_REGULAR)
		{
			PlayerInfo[playerid][pTag] = 16;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Regular Tester");	
		}
		else if(PlayerInfo[playerid][pTester] == TESTERRANK_SENIOR)
		{
			PlayerInfo[playerid][pTag] = 17;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Senior Tester");	
		}
		else if(PlayerInfo[playerid][pTester] == TESTERRANK_MANAGER)
		{
			PlayerInfo[playerid][pTag] = 18;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Tester Manager");	
		}
	}
	else if(!strcmp(params,"developer",true))
	{
		if(PlayerInfo[playerid][pDev] < 1) SendClientMessage(playerid, COLOR_GREY, "You are not a developer!");
		else if(PlayerInfo[playerid][pDev] == 1)
		{
			PlayerInfo[playerid][pTag] = 19;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Junior Developer");	
		}
		else if(PlayerInfo[playerid][pDev] == 2)
		{
			PlayerInfo[playerid][pTag] = 20;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Senior Developer");
		}
		else if(PlayerInfo[playerid][pDev] == 3)
		{
			PlayerInfo[playerid][pTag] = 21;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Developer Manager");	
		}
		else if(PlayerInfo[playerid][pDev] == 4)
		{
			PlayerInfo[playerid][pTag] = 22;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Developer Leader");
		}
	}
	else if(!strcmp(params,"mapper",true))
	{
		if(PlayerInfo[playerid][pMapper] < 1) SendClientMessage(playerid, COLOR_GREY, "You are not a Mapper!");
		else if(PlayerInfo[playerid][pMapper] == 1)
		{
			PlayerInfo[playerid][pTag] = 23;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Regular Mapper");
		}
		else if(PlayerInfo[playerid][pMapper] == 2)
		{
			PlayerInfo[playerid][pTag] = 24;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Senior Mapper");	
		}
		else if(PlayerInfo[playerid][pMapper] == 3)
		{
			PlayerInfo[playerid][pTag] = 25;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Mapping Manager");
		}
	}
	else if(!strcmp(params,"famed",true))
	{
		if(PlayerInfo[playerid][pFamed] < 1) SendClientMessage(playerid, COLOR_GREY, "You are not Famed!");
		else if(PlayerInfo[playerid][pFamed] == 1)
		{
			PlayerInfo[playerid][pTag] = 27;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Famed");
		}
		else if(PlayerInfo[playerid][pFamed] == 2)
		{
			PlayerInfo[playerid][pTag] = 28;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Famed Manager");
		}
	}
	else if(!strcmp(params,"oldschool",true))
	{
		if(PlayerInfo[playerid][pOldSchool] < 1) SendClientMessage(playerid, COLOR_GREY, "You are not Old School!");
		else if(PlayerInfo[playerid][pOldSchool] == 1)
		{
			PlayerInfo[playerid][pTag] = 29;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Old School Member");
		}
		else if(PlayerInfo[playerid][pOldSchool] == 2)
		{
			PlayerInfo[playerid][pTag] = 30;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Old School Moderator");
		}
	}
	else if(!strcmp(params,"designer",true))
	{
		if(PlayerInfo[playerid][pGFX] < 1) SendClientMessage(playerid, COLOR_GREY, "You are not Old School!");
		else if(PlayerInfo[playerid][pGFX] == 1)
		{
			PlayerInfo[playerid][pTag] = 31;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Designer");
		}
		else if(PlayerInfo[playerid][pGFX] == 2)
		{
			PlayerInfo[playerid][pTag] = 32;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Video Editor");
		}
		else if(PlayerInfo[playerid][pGFX] == 3)
		{
			PlayerInfo[playerid][pTag] = 33;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Moderator");
		}
		else if(PlayerInfo[playerid][pGFX] == 4)
		{
			PlayerInfo[playerid][pTag] = 34;
			SendClientMessage(playerid, COLOR_GREY, "You changed your newbie tag to Leader");
		}
	}
	else
	{
		SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /newbietag <Tag>");
		SendClientMessage(playerid, COLOR_GREY, "Tags: Newbie, Player, Helper, Admin, Tester, Developer, Mapper, Famed, OldSchool, Designer");
	}
	return 1;
}
CMD:newbie(playerid, params[])
{
	new msg[128], name[24], string[500];
	if(gPlayerLogged[playerid] == 0) SendClientMessage(playerid, COLOR_GREY, "You havent logged in yet.");
	else if(PlayerInfo[playerid][pTut] == 0) return 1;
	else if((nonewbie) && PlayerInfo[playerid][pAdmin] < 3) SendClientMessage(playerid, COLOR_GREY, "The Newbie channel has been disabled by an Admin.");
	else if(PlayerInfo[playerid][pNewbieMuted] == 1) SendClientMessage(playerid, COLOR_GREY, "You are banned from speaking in that channel.");
	else if(PlayerInfo[playerid][pTogNewbie]) SendClientMessage(playerid, COLOR_GREY, "Newbie chat channel is currently turned off, use /tognewbie.");
	else if(PlayerInfo[playerid][pTag] == 0) SendClientMessage(playerid, COLOR_GREY, "The newbie system was revamped, use /newbietag");
	else if(gettime() < (UseNewbieTimer[playerid] + 50))
	{
		format(string, sizeof(string), "You have to wait %d seconds before you can use /newbie again!", (UseNewbieTimer[playerid] + 50) - gettime());
		SendClientMessage(playerid, COLOR_GREY, string);
	}
	else
	{
		strmid(name, str_replace('_', ' ', PlayerName(playerid)), 0, MAX_PLAYER_NAME);
		if(sscanf(params, "s[174]", msg)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} (/newb)ie [message]");
		{
			if(strlen(msg) > MAX_SPLIT_LENGTH)
			{
				format(string, sizeof(string), "** %s %s [%d]: %.*s...", GetNewbieTag(playerid), PlayerOOCName(playerid),playerid, MAX_SPLIT_LENGTH, msg);
				OOCNewbie(NEWBIE_COLOR, string);
				format(string, sizeof(string), "* ...%s", msg[MAX_SPLIT_LENGTH]);
				OOCNewbie(NEWBIE_COLOR, string);
			}
			else
			{
				format(string, sizeof(string), "** %s %s [%d]: %s **", GetNewbieTag(playerid), PlayerOOCName(playerid), playerid, msg);
				OOCNewbie(NEWBIE_COLOR, string);
			}
			if(PlayerInfo[playerid][pAdmin] <= 1 && PlayerInfo[playerid][pHelper] <= 0) UseNewbieTimer[playerid] = gettime();
		}
	}
	return 1;
}
