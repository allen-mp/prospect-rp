
/*
	Mariela Tucci's Donor Script

	Base: Avery
	Contributed/whole fix : Golden

	Last check: 26 may 2018
*/



#define MARIELA_SQL_ID 91115

#define MARIELA_ENTR_POS 1975.8326,-1922.9746,13.5469
#define MARIELA_EXIT_POS -2873.9021, 198.4312, 9995.8467
#define MARIELA_SAFE_POS -2844.1934, 220.6485, 9991.8096
#define MARIELA_CLUB_POS -2881.6914, 215.6439, 9991.8105

new 
	ClubStatus = 0,

	MarielaCurrentSong[128],
	MarielaSongTitle[32],

	MarielaUsingMic[MAX_PLAYERS],
	MarielaSmokeMachine,

	Text3D:MarielaEnterLabel,
	Text3D:MarielaSafeLabel,
	Text3D:MarielaExitLabel,

	MarielaIcons[3],
	MarielaIsDJ[MAX_PLAYERS],
	MarielaSmokeObject[5]; // Smoke objects


Hook:tucci_OnGameModeInit()
{

	MarielaSafeLabel = Create3DTextLabel("Mariela's Safe\n{FF0000}/marielasafe", -1, MARIELA_SAFE_POS, 20.0, 0, 0);
	MarielaExitLabel = Create3DTextLabel("Exit{FFFFFF}\n/clubexit", -1, MARIELA_EXIT_POS, 20.0, 0, 0);

	MarielaIcons[0] = CreatePickup(1239, 1, MARIELA_ENTR_POS, 0);	// Entrance Icon
	MarielaIcons[1] = CreatePickup(1239, 1, MARIELA_EXIT_POS, 0);	// Exit Icon
	
	MarielaEnterLabel = Create3DTextLabel("{FFFFFF}Mariela's Bar\n\
								 		   Status: {8B0000}Closed\n\
								 		   {FFFFFF}/clubenter", -1, MARIELA_ENTR_POS, 20.0, 0, 0);
	printf("Loaded Tucci");
	return 1;
}
Hook:tucci_OnGameModeExit()
{
		// Clears the text labels and icons.
	Delete3DTextLabel(MarielaEnterLabel);
	Delete3DTextLabel(MarielaExitLabel);
	Delete3DTextLabel(MarielaSafeLabel);

	// Clears all pickup icons
	for(new i=0; i < sizeof(MarielaIcons); i++)
		DestroyPickup(MarielaIcons[i]);

	return 1;
}

Hook:Tucci_OnPlayerText(playerid, text[]) 
{

	// If the player speaks regularly while using the mic, broadcast to everyone in the club.
	if(MarielaUsingMic[playerid] == 1) 
	{
		if(!IsPlayerInRangeOfPoint(playerid, 10.0, -2883.11, 226.72, 9994.52)) return SendClientMessage(playerid, COLOR_GREY, "You are not at the DJ table!");
		new micstr[80];
		foreach(new p : Player)
		{
			// If player is in the radius (100) of the club's center
			if(IsPlayerInRangeOfPoint(p, 100.0, MARIELA_CLUB_POS)) 
			{
				format(micstr, sizeof(micstr), "[DJ %s]: {FFFFFF}%s", PlayerICName(playerid), text);
				SendClientMessage(p, COLOR_BITEM, micstr);
			}
		}
	}
	return 0;
}



//========COMMANDS========
CMD:clubhelp(playerid) {
	if(PlayerInfo[playerid][pID] != MARIELA_SQL_ID) return SendClientMessage(playerid, COLOR_GREY, "You are not the club owner!");
	SendClientMessage(playerid, COLOR_ORANGE, "Mariela's Club Commands");
	SendClientMessage(playerid, COLOR_GREY, "/clubenter, /clubexit, /msmoke, /mmic /clublock");
	SendClientMessage(playerid, COLOR_GREY, "/mmakedj, /mremovedj, /mclubmusic /mstopmusic");
	return 1;
}
CMD:clublock(playerid)
{
	if(PlayerInfo[playerid][pID] != MARIELA_SQL_ID) return SendClientMessage(playerid, COLOR_GREY, "You are not the club owner!");

	if(ClubStatus == 0) 
	{ 
		SendClientMessage(playerid, COLOR_WHITE, "The club is now {008000}open.");
		Update3DTextLabelText(MarielaEnterLabel, COLOR_WHITE, "{FFFFFF}Mariela's Bar\n\
								 		   Status: {008000}Open\n\
								 		   {FFFFFF}/clubenter");
		ClubStatus = 1;
	}
	else {
		SendClientMessage(playerid, COLOR_WHITE, "The club is now {008000}open.");
		Update3DTextLabelText(MarielaEnterLabel, COLOR_WHITE, "{FFFFFF}Mariela's Bar\n\
								 		   Status: {8B0000}Closed\n\
								 		   {FFFFFF}/clubenter");
		ClubStatus = 0;
	}

	return 1;
}

CMD:clubenter(playerid) 
{
	new title[64];
	if(ClubStatus == 0) return SendClientMessage(playerid, COLOR_RED, "Club is currently locked.");
	if(IsPlayerInRangeOfPoint(playerid, 3.0, MARIELA_ENTR_POS))
	{
		SetPlayerPos(playerid, MARIELA_EXIT_POS);
		SetPlayerFacingAngle(playerid, 358.8926); 
		SetPlayerInterior(playerid, 1);

		SetPlayerTime(playerid, 2, 0);

		FreezePlayer(playerid, FREEZE, ENTER_FREEZE);
		
		// Shows player which song is playing
		if(strlen(MarielaCurrentSong) > 0) {
			// Plays the Current Song upon entry if one is set and shows info.
			PlayAudioStreamForPlayer(playerid, MarielaCurrentSong);
			format(title, sizeof(title), "Now playing: %s", MarielaSongTitle);
			SendClientMessage(playerid, COLOR_ORANGE, title);
		}
	}
	return 1;
}

CMD:clubexit(playerid) 
{
	new hour, mini, sec;
	gettime(hour, mini, sec);

	if(IsPlayerInRangeOfPoint(playerid, 3.0, MARIELA_EXIT_POS)) {

		SetPlayerPos(playerid, MARIELA_ENTR_POS);
		SetPlayerFacingAngle(playerid, 180.5);
		SetPlayerInterior(playerid, 0);

		SetPlayerTime(playerid, hour, mini);

		StopAudioStreamForPlayer(playerid);
		MarielaUsingMic[playerid] = 0;
	}
	return 1;
}

CMD:mmakedj(playerid, params[]) {
	new toplayer, str[64];

	if(PlayerInfo[playerid][pID] != MARIELA_SQL_ID) return SendClientMessage(playerid, COLOR_GREY, "You are not the club owner!");

	if(sscanf(params, "u", toplayer)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /marielamakedj [playerid / part of name]");

	if(!IsPlayerConnected(toplayer)) return SendClientMessage(playerid, COLOR_GREY, "That player isn't online.");

	if(MarielaIsDJ[toplayer] == 1) return SendClientMessage(playerid, COLOR_GREY, "That player is already a DJ!");

	format(str, sizeof(str), "%s has made you a DJ.", PlayerICName(playerid));
	SendClientMessage(toplayer, COLOR_BITEM, str);

	format(str, sizeof(str), "You have made %s a DJ.", PlayerICName(toplayer));
	SendClientMessage(playerid, COLOR_BITEM, str);

	MarielaIsDJ[toplayer] = 1;
	MarielaUsingMic[toplayer] = 1;

	return 1;
}

CMD:mremovedj(playerid, params[]) {
	new toplayer, str[64];

	if(PlayerInfo[playerid][pID] != MARIELA_SQL_ID) return SendClientMessage(playerid, COLOR_GREY, "You are not the club owner!");

	if(sscanf(params, "u", toplayer)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /marielaremovedj [playerid / part of name]");

	if(!IsPlayerConnected(toplayer)) return SendClientMessage(playerid, COLOR_GREY, "That player isn't online.");

	format(str, sizeof(str), "%s has revoked your DJ permissions.", PlayerICName(playerid));
	SendClientMessage(toplayer, COLOR_BITEM, str);

	format(str, sizeof(str), "You have revoked %s's DJ permissions.", PlayerICName(toplayer));
	SendClientMessage(playerid, COLOR_BITEM, str);

	MarielaIsDJ[toplayer] = 0;
	MarielaUsingMic[toplayer] = 0;

	return 1;
}


CMD:mclubmusic(playerid, params[]) {
	if(MarielaIsDJ[playerid] == 0) return SendClientMessage(playerid, COLOR_GREY, "You are not a DJ!");
	if(!IsPlayerInRangeOfPoint(playerid, 10.0, -2883.11, 226.72, 9994.52)) return SendClientMessage(playerid, COLOR_GREY, "You are not at the DJ table!");
	
	if(sscanf(params, "s[128]s[32]", MarielaCurrentSong, MarielaSongTitle)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /clubmusic [URL] [title]");

	// Don't format in loop. No need.
	format(MarielaSongTitle, sizeof(MarielaSongTitle), "Now playing: %s", MarielaSongTitle);

	foreach(new p : Player) {
		if(IsPlayerInRangeOfPoint(playerid, 100.0, MARIELA_CLUB_POS)) {
			// Play music and display song name for each player.
			PlayAudioStreamForPlayer(p, MarielaCurrentSong);
			SendClientMessage(p, COLOR_ORANGE, MarielaSongTitle);
		}
	}
	return 1;
}

CMD:mstopmusic(playerid) {
	new str[64];

	if(MarielaIsDJ[playerid] == 0) return SendClientMessage(playerid, COLOR_GREY, "You are not a DJ!");

	format(str, sizeof(str), "%s has stopped the music!", PlayerICName(playerid));
	foreach(new p : Player)
	{
		if(IsPlayerInRangeOfPoint(p, 100.0, MARIELA_CLUB_POS))
		{
			SendClientMessage(p, COLOR_ORANGE, str);
			StopAudioStreamForPlayer(p);
		}
	}

	MarielaCurrentSong[0] = '\0';
	MarielaSongTitle[0] = '\0';
	return 1;
}


CMD:msmoke(playerid) {
	if(!IsPlayerInRangeOfPoint(playerid, 100.0, MARIELA_CLUB_POS)) return SendClientMessage(playerid, COLOR_GREY, "You are not at the club!");
	if(PlayerInfo[playerid][pID] != MARIELA_SQL_ID) return SendClientMessage(playerid, COLOR_GREY, "You are not the club owner!");

	if(MarielaSmokeMachine == 0) {
		MarielaSmokeMachine = 1;
		SendClientMessage(playerid, COLOR_GREY, "You have turned the smoke machine on.");
	} else {
		MarielaSmokeMachine = 0;
		SendClientMessage(playerid, COLOR_GREY, "You have turned the smoke machine off.");
	}

	RenderSmoke();

	return 1;
}


CMD:mmic(playerid) {

	if(MarielaIsDJ[playerid] == 0) return SendClientMessage(playerid, COLOR_GREY, "You are not a DJ!");
	if(!IsPlayerInRangeOfPoint(playerid, 10.0, -2883.11, 226.72, 9994.52)) return SendClientMessage(playerid, COLOR_GREY, "You are not at the DJ table!");
	
	if(MarielaUsingMic[playerid] == 0) {
		SendClientMessage(playerid, COLOR_BITEM, "You are now using the mic.");
		MarielaUsingMic[playerid] = 1;
	} else {
		SendClientMessage(playerid, COLOR_BITEM, "You are no longer using the mic.");
		MarielaUsingMic[playerid] = 0;
	}
	return 1;
}



RenderSmoke() {
	// if the smoke machine is on, the script will generate smoke particles
	if(MarielaSmokeMachine == 1) {
		MarielaSmokeObject[0] = CreateDynamicObject(18716, -2885.16284, 225.14879, 9990.00586, 0.00000, 0.00000, 0.00000); // Smoke
		MarielaSmokeObject[1] = CreateDynamicObject(18716, -2879.80884, 224.26920, 9990.00586, 0.00000, 0.00000, 0.00000); // Smoke
		MarielaSmokeObject[2] = CreateDynamicObject(18716, -2887.62256, 225.54800, 9990.00586, 0.00000, 0.00000, 0.00000); // Smoke
		MarielaSmokeObject[3] = CreateDynamicObject(18716, -2874.94116, 224.78200, 9990.00586, 0.00000, 0.00000, 0.00000); // Smoke
		MarielaSmokeObject[4] = CreateDynamicObject(18716, -2872.49487, 225.53140, 9990.00586, 0.00000, 0.00000, 0.00000); // Smoke
	} else {
		for(new i=0; i < sizeof(MarielaSmokeObject); i++) {
			DestroyDynamicObjectEx(MarielaSmokeObject[i]);
		}
	}
	return 1;
}
