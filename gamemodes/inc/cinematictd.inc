//new Text:background_bgcolor;
new Text:background_topslide;
new Text:background_topslide_shadow;
new Text:background_bottomslide;
new Text:background_bottomslide_shadow;

new Text:facebook_iconbg_shadow;
new Text:facebook_txtbg_shadow;
new Text:facebook_iconbg;
new Text:facebook_icon;
new Text:facebook_txt;

new Text:gtasarp_iconbg_shadow;
new Text:gtasarp_txtbg_shadow;
new Text:gtasarp_iconbg;
new Text:gtasarp_icon;
new Text:gtasarp_txt;

new Text:betaserver_txt;
new Text:sanrp_txt;

stock createCinematicTextDraw() {
	/*background_bgcolor = TextDrawCreate(1.000000 , 48.000000 , "_");
	TextDrawBackgroundColor(background_bgcolor, 255);
	TextDrawFont(background_bgcolor, 1);
	TextDrawLetterSize(background_bgcolor, 0.500000, 39.099948);
	TextDrawColor(background_bgcolor, -1);
	TextDrawSetOutline(background_bgcolor, 0);
	TextDrawSetProportional(background_bgcolor, 1);
	TextDrawSetShadow(background_bgcolor, 1);
	TextDrawUseBox(background_bgcolor, 1);
	TextDrawBoxColor(background_bgcolor, 65305);
	TextDrawTextSize(background_bgcolor, 639.000000, 2.000000);
	TextDrawSetSelectable(background_bgcolor, 0);*/
	
	background_topslide = TextDrawCreate(1.000000, 1.000000, "_");
	TextDrawBackgroundColor(background_topslide, 255);
	TextDrawFont(background_topslide, 1);
	TextDrawLetterSize(background_topslide, 0.500000, 4.899999);
	TextDrawColor(background_topslide, -1);
	TextDrawSetOutline(background_topslide, 0);
	TextDrawSetProportional(background_topslide, 1);
	TextDrawSetShadow(background_topslide, 1);
	TextDrawUseBox(background_topslide, 1);
	TextDrawBoxColor(background_topslide, 255);
	TextDrawTextSize(background_topslide, 639.000000, 0.000000);
	TextDrawSetSelectable(background_topslide, 0);
	
	background_topslide_shadow = TextDrawCreate(1.000000, 48.000000, "_");
	TextDrawBackgroundColor(background_topslide_shadow, 255);
	TextDrawFont(background_topslide_shadow, 1);
	TextDrawLetterSize(background_topslide_shadow, 0.500000, 0.400000);
	TextDrawColor(background_topslide_shadow, -1);
	TextDrawSetOutline(background_topslide_shadow, 0);
	TextDrawSetProportional(background_topslide_shadow, 1);
	TextDrawSetShadow(background_topslide_shadow, 1);
	TextDrawUseBox(background_topslide_shadow, 1);
	TextDrawBoxColor(background_topslide_shadow, 50);
	TextDrawTextSize(background_topslide_shadow, 639.000000, -1.000000);
	TextDrawSetSelectable(background_topslide_shadow, 0);
	
	background_bottomslide = TextDrawCreate(1.000000, 403.000000, "_");
	TextDrawBackgroundColor(background_bottomslide, 255);
	TextDrawFont(background_bottomslide, 1);
	TextDrawLetterSize(background_bottomslide, 0.500000, 4.899999);
	TextDrawColor(background_bottomslide, -1);
	TextDrawSetOutline(background_bottomslide, 0);
	TextDrawSetProportional(background_bottomslide, 1);
	TextDrawSetShadow(background_bottomslide, 1);
	TextDrawUseBox(background_bottomslide, 1);
	TextDrawBoxColor(background_bottomslide, 255);
	TextDrawTextSize(background_bottomslide, 639.000000, 0.000000);
	TextDrawSetSelectable(background_bottomslide, 0);
	
	background_bottomslide_shadow = TextDrawCreate(1.000000, 396.000000, "_");
	TextDrawBackgroundColor(background_bottomslide_shadow, 255);
	TextDrawFont(background_bottomslide_shadow, 1);
	TextDrawLetterSize(background_bottomslide_shadow, 0.500000, 0.400000);
	TextDrawColor(background_bottomslide_shadow, -1);
	TextDrawSetOutline(background_bottomslide_shadow, 0);
	TextDrawSetProportional(background_bottomslide_shadow, 1);
	TextDrawSetShadow(background_bottomslide_shadow, 1);
	TextDrawUseBox(background_bottomslide_shadow, 1);
	TextDrawBoxColor(background_bottomslide_shadow, 50);
	TextDrawTextSize(background_bottomslide_shadow, 639.000000, -1.000000);
	TextDrawSetSelectable(background_bottomslide_shadow, 0);
}

stock createAdvertismentTextDraw() {

	new Float: xoffset = 311.0;
	//new Float: yoffset = 88.0;
	new Float: yoffset = 35.0;
	
	facebook_iconbg_shadow = TextDrawCreate(219.000000 + xoffset, 339.000000 + yoffset, "_");
	TextDrawBackgroundColor(facebook_iconbg_shadow, 255);
	TextDrawFont(facebook_iconbg_shadow, 1);
	TextDrawLetterSize(facebook_iconbg_shadow, 0.219999, 1.200000);
	TextDrawColor(facebook_iconbg_shadow, -1);
	TextDrawSetOutline(facebook_iconbg_shadow, 0);
	TextDrawSetProportional(facebook_iconbg_shadow, 1);
	TextDrawSetShadow(facebook_iconbg_shadow, 1);
	TextDrawUseBox(facebook_iconbg_shadow, 1);
	TextDrawBoxColor(facebook_iconbg_shadow, 50);
	TextDrawTextSize(facebook_iconbg_shadow, 228.000000 + xoffset, 0.000000);
	TextDrawSetSelectable(facebook_iconbg_shadow, 0);
	
	facebook_txtbg_shadow = TextDrawCreate(230.000000 + xoffset, 339.000000 + yoffset, "_");
	TextDrawBackgroundColor(facebook_txtbg_shadow, 255);
	TextDrawFont(facebook_txtbg_shadow, 1);
	TextDrawLetterSize(facebook_txtbg_shadow, 0.219999, 1.200000);
	TextDrawColor(facebook_txtbg_shadow, -1);
	TextDrawSetOutline(facebook_txtbg_shadow, 0);
	TextDrawSetProportional(facebook_txtbg_shadow, 1);
	TextDrawSetShadow(facebook_txtbg_shadow, 1);
	TextDrawUseBox(facebook_txtbg_shadow, 1);
	TextDrawBoxColor(facebook_txtbg_shadow, 50);
	TextDrawTextSize(facebook_txtbg_shadow, 321.000000 + xoffset, 0.000000);
	TextDrawSetSelectable(facebook_txtbg_shadow, 0);
	
	facebook_iconbg = TextDrawCreate(220.000000 + xoffset, 340.000000 + yoffset, "_");
	TextDrawBackgroundColor(facebook_iconbg, 255);
	TextDrawFont(facebook_iconbg, 1);
	TextDrawLetterSize(facebook_iconbg, 0.219999, 1.000000);
	TextDrawColor(facebook_iconbg, -1);
	TextDrawSetOutline(facebook_iconbg, 0);
	TextDrawSetProportional(facebook_iconbg, 1);
	TextDrawSetShadow(facebook_iconbg, 1);
	TextDrawUseBox(facebook_iconbg, 1);
	TextDrawBoxColor(facebook_iconbg, 995727615);
	TextDrawTextSize(facebook_iconbg, 227.000000 + xoffset, 0.000000);
	TextDrawSetSelectable(facebook_iconbg, 0);
	
	facebook_icon = TextDrawCreate(223.000000 + xoffset, 338.000000 + yoffset, "f");
	TextDrawBackgroundColor(facebook_icon, 255);
	TextDrawFont(facebook_icon, 1);
	TextDrawLetterSize(facebook_icon, 0.289999, 1.500000);
	TextDrawColor(facebook_icon, -1);
	TextDrawSetOutline(facebook_icon, 0);
	TextDrawSetProportional(facebook_icon, 1);
	TextDrawSetShadow(facebook_icon, 0);
	TextDrawSetSelectable(facebook_icon, 0);
	
	//facebook_txt = TextDrawCreate(231.000000 + xoffset, 340.000000 + yoffset, "None");
	facebook_txt = TextDrawCreate(231.000000 + xoffset, 340.000000 + yoffset, "None");
	TextDrawBackgroundColor(facebook_txt, 255);
	TextDrawFont(facebook_txt, 1);
	TextDrawLetterSize(facebook_txt, 0.209999, 1.000000);
	TextDrawColor(facebook_txt, -1);
	TextDrawSetOutline(facebook_txt, 0);
	TextDrawSetProportional(facebook_txt, 1);
	TextDrawSetShadow(facebook_txt, 0);
	TextDrawUseBox(facebook_txt, 1);
	TextDrawBoxColor(facebook_txt, 995727615);
	TextDrawTextSize(facebook_txt, 320.000000 + xoffset, 4.000000);
	TextDrawSetSelectable(facebook_txt, 0);
	
	gtasarp_iconbg_shadow = TextDrawCreate(219.000000 + xoffset, 324.000000 + yoffset, "_");
	TextDrawBackgroundColor(gtasarp_iconbg_shadow, 255);
	TextDrawFont(gtasarp_iconbg_shadow, 1);
	TextDrawLetterSize(gtasarp_iconbg_shadow, 0.219999, 1.200000);
	TextDrawColor(gtasarp_iconbg_shadow, -1);
	TextDrawSetOutline(gtasarp_iconbg_shadow, 0);
	TextDrawSetProportional(gtasarp_iconbg_shadow, 1);
	TextDrawSetShadow(gtasarp_iconbg_shadow, 1);
	TextDrawUseBox(gtasarp_iconbg_shadow, 1);
	TextDrawBoxColor(gtasarp_iconbg_shadow, 50);
	TextDrawTextSize(gtasarp_iconbg_shadow, 228.000000 + xoffset, 0.000000);
	TextDrawSetSelectable(gtasarp_iconbg_shadow, 0);
	
	gtasarp_txtbg_shadow = TextDrawCreate(230.000000 + xoffset, 324.000000 + yoffset, "_");
	TextDrawBackgroundColor(gtasarp_txtbg_shadow, 255);
	TextDrawFont(gtasarp_txtbg_shadow, 1);
	TextDrawLetterSize(gtasarp_txtbg_shadow, 0.219999, 1.200000);
	TextDrawColor(gtasarp_txtbg_shadow, -1);
	TextDrawSetOutline(gtasarp_txtbg_shadow, 0);
	TextDrawSetProportional(gtasarp_txtbg_shadow, 1);
	TextDrawSetShadow(gtasarp_txtbg_shadow, 1);
	TextDrawUseBox(gtasarp_txtbg_shadow, 1);
	TextDrawBoxColor(gtasarp_txtbg_shadow, 50);
	TextDrawTextSize(gtasarp_txtbg_shadow, 321.000000 + xoffset, 0.000000);
	TextDrawSetSelectable(gtasarp_txtbg_shadow, 0);
	
	gtasarp_iconbg = TextDrawCreate(220.000000 + xoffset, 325.000000 + yoffset, "_");
	TextDrawBackgroundColor(gtasarp_iconbg, 255);
	TextDrawFont(gtasarp_iconbg, 1);
	TextDrawLetterSize(gtasarp_iconbg, 0.219999, 1.000000);
	TextDrawColor(gtasarp_iconbg, 200);
	TextDrawSetOutline(gtasarp_iconbg, 0);
	TextDrawSetProportional(gtasarp_iconbg, 1);
	TextDrawSetShadow(gtasarp_iconbg, 1);
	TextDrawUseBox(gtasarp_iconbg, 1);
	TextDrawBoxColor(gtasarp_iconbg, 1684300950);
	TextDrawTextSize(gtasarp_iconbg, 227.000000 + xoffset, 0.000000);
	TextDrawSetSelectable(gtasarp_iconbg, 0);
	
	gtasarp_icon = TextDrawCreate(220.000000 + xoffset, 324.000000 + yoffset, "LD_BEAT:upl");
	TextDrawAlignment(gtasarp_icon, 2);
	TextDrawBackgroundColor(gtasarp_icon, 255);
	TextDrawFont(gtasarp_icon, 4);
	TextDrawLetterSize(gtasarp_icon, 0.000000, 0.000000);
	TextDrawColor(gtasarp_icon, -1);
	TextDrawSetOutline(gtasarp_icon, 0);
	TextDrawSetProportional(gtasarp_icon, 1);
	TextDrawSetShadow(gtasarp_icon, 1);
	TextDrawUseBox(gtasarp_icon, 1);
	TextDrawBoxColor(gtasarp_icon, 995727615);
	TextDrawTextSize(gtasarp_icon, 8.000000, 11.000000);
	TextDrawSetSelectable(gtasarp_icon, 0);
	
	gtasarp_txt = TextDrawCreate(231.000000 + xoffset, 325.000000 + yoffset, "district-rp.xyz");
	TextDrawBackgroundColor(gtasarp_txt, 255);
	TextDrawFont(gtasarp_txt, 1);
	TextDrawLetterSize(gtasarp_txt, 0.209998, 1.000000);
	TextDrawColor(gtasarp_txt, -1);
	TextDrawSetOutline(gtasarp_txt, 0);
	TextDrawSetProportional(gtasarp_txt, 1);
	TextDrawSetShadow(gtasarp_txt, 0);
	TextDrawUseBox(gtasarp_txt, 1);
	TextDrawBoxColor(gtasarp_txt, 1684300950);
	TextDrawTextSize(gtasarp_txt, 320.000000 + xoffset, 4.000000);
	TextDrawSetSelectable(gtasarp_txt, 0);

}

stock createBetaServerTextDraw() {
	betaserver_txt = TextDrawCreate(10.000000, 377.000000, "BETA SERVER");
	TextDrawBackgroundColor(betaserver_txt, 20530);
	TextDrawFont(betaserver_txt, 1);
	TextDrawLetterSize(betaserver_txt, 0.300000, 1.299999);
	TextDrawColor(betaserver_txt, -1);
	TextDrawSetOutline(betaserver_txt, 0);
	TextDrawSetProportional(betaserver_txt, 1);
	TextDrawSetShadow(betaserver_txt, 0);
	TextDrawSetSelectable(betaserver_txt, 0);
	
	sanrp_txt = TextDrawCreate(10.000000, 367.000000, "District Roleplay");
	TextDrawBackgroundColor(sanrp_txt, 20530);
	TextDrawFont(sanrp_txt, 1);
	TextDrawLetterSize(sanrp_txt, 0.300000, 1.299999);
	TextDrawColor(sanrp_txt, -65281);
	TextDrawSetOutline(sanrp_txt, 0);
	TextDrawSetProportional(sanrp_txt, 1);
	TextDrawSetShadow(sanrp_txt, 0);
	TextDrawSetSelectable(sanrp_txt, 0);
}

stock showBetaServerTextDraw(playerid) {
	TextDrawShowForPlayer(playerid, betaserver_txt);
	TextDrawShowForPlayer(playerid, sanrp_txt);
}

stock hideBetaServerTextDraw(playerid) {
	TextDrawHideForPlayer(playerid, betaserver_txt);
	TextDrawHideForPlayer(playerid, sanrp_txt);
}

stock destroyBetaServerTextDraw() {
	TextDrawHideForAll(betaserver_txt);
	TextDrawHideForAll(sanrp_txt);
	TextDrawDestroy(betaserver_txt);
	TextDrawDestroy(sanrp_txt);
}

stock showCinematicTextDraw(playerid) {
	//TextDrawShowForPlayer(playerid, background_bgcolor);
	TextDrawShowForPlayer(playerid, background_topslide);
	TextDrawShowForPlayer(playerid, background_topslide_shadow);
	TextDrawShowForPlayer(playerid, background_bottomslide);
	TextDrawShowForPlayer(playerid, background_bottomslide_shadow);
}

stock hideCinematicTextDraw(playerid) {
//	TextDrawHideForPlayer(playerid, background_bgcolor);
	TextDrawHideForPlayer(playerid, background_topslide);
	TextDrawHideForPlayer(playerid, background_topslide_shadow);
	TextDrawHideForPlayer(playerid, background_bottomslide);
	TextDrawHideForPlayer(playerid, background_bottomslide_shadow);
}

stock hideAdvertismentTextDraw(playerid) {
	TextDrawHideForPlayer(playerid, facebook_iconbg_shadow);
	TextDrawHideForPlayer(playerid, facebook_txtbg_shadow);
	TextDrawHideForPlayer(playerid, facebook_iconbg);
	TextDrawHideForPlayer(playerid, facebook_icon);
	TextDrawHideForPlayer(playerid, facebook_txt);
	
	TextDrawHideForPlayer(playerid, gtasarp_iconbg_shadow);
	TextDrawHideForPlayer(playerid, gtasarp_txtbg_shadow);
	TextDrawHideForPlayer(playerid, gtasarp_iconbg);
	TextDrawHideForPlayer(playerid, gtasarp_icon);
	TextDrawHideForPlayer(playerid, gtasarp_txt);
}

stock showAdvertismentTextDraw(playerid) {
	TextDrawShowForPlayer(playerid, facebook_iconbg_shadow);
	TextDrawShowForPlayer(playerid, facebook_txtbg_shadow);
	TextDrawShowForPlayer(playerid, facebook_iconbg);
	TextDrawShowForPlayer(playerid, facebook_icon);
	TextDrawShowForPlayer(playerid, facebook_txt);
	
	TextDrawShowForPlayer(playerid, gtasarp_iconbg_shadow);
	TextDrawShowForPlayer(playerid, gtasarp_txtbg_shadow);
	TextDrawShowForPlayer(playerid, gtasarp_iconbg);
	TextDrawShowForPlayer(playerid, gtasarp_icon);
	TextDrawShowForPlayer(playerid, gtasarp_txt);
}

stock destroyCinematicTextDraw() {
	//TextDrawHideForAll(background_bgcolor);
	TextDrawHideForAll(background_topslide);
	TextDrawHideForAll(background_topslide_shadow);
	TextDrawHideForAll(background_bottomslide);
	TextDrawHideForAll(background_bottomslide_shadow);
	//TextDrawDestroy(background_bgcolor);
	TextDrawDestroy(background_topslide);
	TextDrawDestroy(background_topslide_shadow);
	TextDrawDestroy(background_bottomslide);
	TextDrawDestroy(background_bottomslide_shadow);
}

stock destroyAdvertismentTextDraw() {
	TextDrawHideForAll(facebook_iconbg_shadow);
	TextDrawHideForAll(facebook_txtbg_shadow);
	TextDrawHideForAll(facebook_iconbg);
	TextDrawHideForAll(facebook_icon);
	TextDrawHideForAll(facebook_txt);
	
	TextDrawDestroy(facebook_iconbg_shadow);
	TextDrawDestroy(facebook_txtbg_shadow);
	TextDrawDestroy(facebook_iconbg);
	TextDrawDestroy(facebook_icon);
	TextDrawDestroy(facebook_txt);
	
	TextDrawHideForAll(gtasarp_iconbg_shadow);
	TextDrawHideForAll(gtasarp_txtbg_shadow);
	TextDrawHideForAll(gtasarp_iconbg);
	TextDrawHideForAll(gtasarp_icon);
	TextDrawHideForAll(gtasarp_txt);
	
	TextDrawDestroy(gtasarp_iconbg_shadow);
	TextDrawDestroy(gtasarp_txtbg_shadow);
	TextDrawDestroy(gtasarp_iconbg);
	TextDrawDestroy(gtasarp_icon);
	TextDrawDestroy(gtasarp_txt);
}
