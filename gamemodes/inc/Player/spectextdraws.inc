//Global Textdraws:

new Text:spectextdraw1; // small black box
new Text:spectextdraw2; // white line

//Player Textdraws:

new PlayerText:specnametxd[MAX_PLAYERS]; // name textdraw
new PlayerText:spechptxd[MAX_PLAYERS]; // hp textdraw
new PlayerText:specarmortxd[MAX_PLAYERS]; // armor testdraw
new PlayerText:specleveltxd[MAX_PLAYERS]; // level textdraw
new PlayerText:specpingtxd[MAX_PLAYERS]; // level textdraw

forward ShowSpectateTextDraws(playerid, giveplayerid);
forward HideSpectateTextDraws(playerid);

Hook:spec_OnGameModeInit()
{
	spectextdraw1 = TextDrawCreate(242.665985, 379.396331, "usebox");
	TextDrawLetterSize(spectextdraw1, 0.000000, 6.282086);
	TextDrawTextSize(spectextdraw1, 380.000122, 0.000000);
	TextDrawAlignment(spectextdraw1, 1);
	TextDrawColor(spectextdraw1, 255);
	TextDrawUseBox(spectextdraw1, true);
	TextDrawBoxColor(spectextdraw1, 255);
	TextDrawSetShadow(spectextdraw1, 0);
	TextDrawSetOutline(spectextdraw1, 0);
	TextDrawFont(spectextdraw1, 0);

	spectextdraw2 = TextDrawCreate(243.000411, 395.574066, "usebox");
	TextDrawLetterSize(spectextdraw2, 0.000000, -0.910901);
	TextDrawTextSize(spectextdraw2, 376.666778, 0.000000);
	TextDrawAlignment(spectextdraw2, 1);
	TextDrawColor(spectextdraw2, 0);
	TextDrawUseBox(spectextdraw2, true);
	TextDrawBoxColor(spectextdraw2, -1);
	TextDrawSetShadow(spectextdraw2, 0);
	TextDrawSetOutline(spectextdraw2, 0);
	TextDrawFont(spectextdraw2, 0);

	return 1;
}

Hook:spec_OnPlayerConnect(playerid)
{
	specnametxd[playerid] = CreatePlayerTextDraw(playerid, 242.667083, 380.800109, "name");
	PlayerTextDrawLetterSize(playerid, specnametxd[playerid], 0.160329, 0.840888);
	PlayerTextDrawAlignment(playerid, specnametxd[playerid], 1);
	PlayerTextDrawColor(playerid, specnametxd[playerid], -1);
	PlayerTextDrawSetShadow(playerid, specnametxd[playerid], 0);
	PlayerTextDrawSetOutline(playerid, specnametxd[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, specnametxd[playerid], 51);
	PlayerTextDrawFont(playerid, specnametxd[playerid], 2);
	PlayerTextDrawSetProportional(playerid, specnametxd[playerid], 1);

	spechptxd[playerid] = CreatePlayerTextDraw(playerid, 242.333343, 399.881378, "Health: 100");
	PlayerTextDrawLetterSize(playerid, spechptxd[playerid], 0.189333, 1.039999);
	PlayerTextDrawAlignment(playerid, spechptxd[playerid], 1);
	PlayerTextDrawColor(playerid, spechptxd[playerid], -1);
	PlayerTextDrawSetShadow(playerid, spechptxd[playerid], 0);
	PlayerTextDrawSetOutline(playerid, spechptxd[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, spechptxd[playerid], 51);
	PlayerTextDrawFont(playerid, spechptxd[playerid], 2);
	PlayerTextDrawSetProportional(playerid, spechptxd[playerid], 1);

	specarmortxd[playerid] = CreatePlayerTextDraw(playerid, 241.666625, 419.377716, "Armor: 101");
	PlayerTextDrawLetterSize(playerid, specarmortxd[playerid], 0.202996, 1.127109);
	PlayerTextDrawAlignment(playerid, specarmortxd[playerid], 1);
	PlayerTextDrawColor(playerid, specarmortxd[playerid], -1);
	PlayerTextDrawSetShadow(playerid, specarmortxd[playerid], 0);
	PlayerTextDrawSetOutline(playerid, specarmortxd[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, specarmortxd[playerid], 51);
	PlayerTextDrawFont(playerid, specarmortxd[playerid], 2);
	PlayerTextDrawSetProportional(playerid, specarmortxd[playerid], 1);

	specleveltxd[playerid] = CreatePlayerTextDraw(playerid, 326.999725, 399.466583, "Level: 50");
	PlayerTextDrawLetterSize(playerid, specleveltxd[playerid], 0.192332, 0.998517);
	PlayerTextDrawAlignment(playerid, specleveltxd[playerid], 1);
	PlayerTextDrawColor(playerid, specleveltxd[playerid], -1);
	PlayerTextDrawSetShadow(playerid, specleveltxd[playerid], 0);
	PlayerTextDrawSetOutline(playerid, specleveltxd[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, specleveltxd[playerid], 51);
	PlayerTextDrawFont(playerid, specleveltxd[playerid], 2);
	PlayerTextDrawSetProportional(playerid, specleveltxd[playerid], 1);

	specpingtxd[playerid] = CreatePlayerTextDraw(playerid, 326.999725, 419.377716, "Ping: 69");
	PlayerTextDrawLetterSize(playerid, specpingtxd[playerid], 0.202996, 1.127109);
	PlayerTextDrawAlignment(playerid, specpingtxd[playerid], 1);
	PlayerTextDrawColor(playerid, specpingtxd[playerid], -1);
	PlayerTextDrawSetShadow(playerid, specpingtxd[playerid], 0);
	PlayerTextDrawSetOutline(playerid, specpingtxd[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, specpingtxd[playerid], 51);
	PlayerTextDrawFont(playerid, specpingtxd[playerid], 2);
	PlayerTextDrawSetProportional(playerid, specpingtxd[playerid], 1);

	return 1;
}

Hook:spec_OnPlayerDisconnect(playerid, reason)
{
	PlayerTextDrawDestroy(playerid, specnametxd[playerid]);
	PlayerTextDrawDestroy(playerid, specleveltxd[playerid]);
	PlayerTextDrawDestroy(playerid, spechptxd[playerid]);
	PlayerTextDrawDestroy(playerid, specarmortxd[playerid]);
	PlayerTextDrawDestroy(playerid, specpingtxd[playerid]);
	return 1;
}

Hook:spec_OnGameModeExit()
{
	TextDrawDestroy(spectextdraw1);
	TextDrawDestroy(spectextdraw2);
	return 1;
}

Hook:spec_OnPlayerUpdate(playerid)
{
	new txdstr[50];
	
	new Float:health, spectator = beingSpectatedID[playerid], Float:armor;

	if(IsPlayerConnected(spectator) && IsPlayerConnected(playerid))
	{
		if(Spectated[playerid])
		{

			GetPlayerArmour(playerid, armor);
			GetPlayerHealth(playerid, health);
			
			format(txdstr, sizeof(txdstr), "Health: %.1f", health);
			PlayerTextDrawSetString(spectator, spechptxd[spectator], txdstr);
			format(txdstr, sizeof(txdstr), "Armor: %.1f", armor);
			PlayerTextDrawSetString(spectator, specarmortxd[spectator], txdstr);
			format(txdstr, sizeof(txdstr), "Ping: %d", GetPlayerPing(playerid));
			PlayerTextDrawSetString(spectator, specpingtxd[spectator], txdstr);
		}
	}
	return 1;
}

public ShowSpectateTextDraws(playerid, giveplayerid)
{
	new txdstr[50];
	GetPlayerArmour(giveplayerid, PlayerInfo[giveplayerid][pArmor]);
	GetPlayerHealth(giveplayerid, PlayerInfo[giveplayerid][pHealth]);

	format(txdstr, sizeof(txdstr), "%s (ID %d)", PlayerName(giveplayerid), giveplayerid);
	PlayerTextDrawSetString(playerid, specnametxd[playerid], txdstr);

	format(txdstr, sizeof(txdstr), "Level: %d", PlayerInfo[giveplayerid][pLevel]);
	PlayerTextDrawSetString(playerid, specleveltxd[playerid], txdstr);

	format(txdstr, sizeof(txdstr), "Armor: %.1f", PlayerInfo[giveplayerid][pArmor]);
	PlayerTextDrawSetString(playerid, specarmortxd[playerid], txdstr);

	format(txdstr, sizeof(txdstr), "Health: %.1f", PlayerInfo[giveplayerid][pHealth]);
	PlayerTextDrawSetString(playerid, spechptxd[playerid], txdstr);

	format(txdstr, sizeof(txdstr), "Ping: %d", GetPlayerPing(giveplayerid));
	PlayerTextDrawSetString(playerid, specpingtxd[playerid], txdstr);

	TextDrawShowForPlayer(playerid, spectextdraw1);
	TextDrawShowForPlayer(playerid, spectextdraw2);
	PlayerTextDrawShow(playerid, specnametxd[playerid]);
	PlayerTextDrawShow(playerid, specleveltxd[playerid]);
	PlayerTextDrawShow(playerid, specarmortxd[playerid]);
	PlayerTextDrawShow(playerid, spechptxd[playerid]);
	PlayerTextDrawShow(playerid, specpingtxd[playerid]);
	return 1;
}

public HideSpectateTextDraws(playerid)
{
	TextDrawHideForPlayer(playerid, spectextdraw1);
	TextDrawHideForPlayer(playerid, spectextdraw2);
	PlayerTextDrawHide(playerid, specnametxd[playerid]);
	PlayerTextDrawHide(playerid, specleveltxd[playerid]);
	PlayerTextDrawHide(playerid, specarmortxd[playerid]);
	PlayerTextDrawHide(playerid, spechptxd[playerid]);
	PlayerTextDrawHide(playerid, specpingtxd[playerid]);
	return 1;
}