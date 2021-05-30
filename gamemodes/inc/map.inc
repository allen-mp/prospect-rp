new Text:mapBorder;
new Text:mapImage;
new Text:waypointText;
new Text:waypointFlag;
new Text:playerMarker;

stock LoadMapTextDraw() {

	mapBorder = TextDrawCreate(191.000000, 96.000000, "_");
	TextDrawBackgroundColor(mapBorder, 255);
	TextDrawFont(mapBorder, 0);
	TextDrawLetterSize(mapBorder, 0.569998, 36.500015);
	TextDrawColor(mapBorder, -251);
	TextDrawSetOutline(mapBorder, 0);
	TextDrawSetProportional(mapBorder, 1);
	TextDrawSetShadow(mapBorder, 0);
	TextDrawUseBox(mapBorder, 1);
	TextDrawBoxColor(mapBorder, 255);
	TextDrawTextSize(mapBorder, 450.000000, 339.000000);
	TextDrawSetSelectable(mapBorder, 0);

	mapImage = TextDrawCreate(191.000000, 96.000000, "samaps:gtasamapbit4"); //426-97, 192-452
	TextDrawBackgroundColor(mapImage, 255);
	TextDrawFont(mapImage, 4);
	TextDrawLetterSize(mapImage, 0.500000, 1.000000);
	TextDrawColor(mapImage, -251);
	TextDrawSetOutline(mapImage, 0);
	TextDrawSetProportional(mapImage, 1);
	TextDrawSetShadow(mapImage, 0);
	TextDrawUseBox(mapImage, 1);
	TextDrawBoxColor(mapImage, -201);
	TextDrawTextSize(mapImage, 259.000000, 329.000000);
	TextDrawSetSelectable(mapImage, 0);
	
	waypointText = TextDrawCreate(193.000000, 412.000000, "Waypoint:");
	TextDrawBackgroundColor(waypointText, 255);
	TextDrawFont(waypointText, 1);
	TextDrawLetterSize(waypointText, 0.200000, 1.100000);
	TextDrawColor(waypointText, -1);
	TextDrawSetOutline(waypointText, 0);
	TextDrawSetProportional(waypointText, 1);
	TextDrawSetShadow(waypointText, 1);
	TextDrawSetSelectable(waypointText, 0);
	
	waypointFlag = TextDrawCreate(424.000000, 337.000000, "hud:radar_enemyAttack");
	TextDrawBackgroundColor(waypointFlag, 255);
	TextDrawFont(waypointFlag, 4);
	TextDrawLetterSize(waypointFlag, 0.549998, 2.499999);
	TextDrawColor(waypointFlag, -1);
	TextDrawSetOutline(waypointFlag, 0);
	TextDrawSetProportional(waypointFlag, 1);
	TextDrawSetShadow(waypointFlag, 1);
	TextDrawUseBox(waypointFlag, 1);
	TextDrawBoxColor(waypointFlag, -1);
	TextDrawTextSize(waypointFlag, -6.000000, 8.000000);
	TextDrawSetSelectable(waypointFlag, 0);
	
	playerMarker = TextDrawCreate(319.000000, 246.000000, "hud:arrow");
	TextDrawBackgroundColor(playerMarker, 255);
	TextDrawFont(playerMarker, 4);
	TextDrawLetterSize(playerMarker, 0.549997, 2.499999);
	TextDrawColor(playerMarker, -1);
	TextDrawSetOutline(playerMarker, 0);
	TextDrawSetProportional(playerMarker, 1);
	TextDrawSetShadow(playerMarker, 1);
	TextDrawUseBox(playerMarker, 1);
	TextDrawBoxColor(playerMarker, -1);
	TextDrawTextSize(playerMarker, -8.000000, 10.000000);
	TextDrawSetSelectable(playerMarker, 0);
	return 1;
}