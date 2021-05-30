
CMD:loopabalance( playerid, params[] )
{
	
	//String Variables.
	new rstring[128],
		rstringG[128],
		rstringG2[128];
	if(PlayerInfo[playerid][pID] == 7914 ) {
	return 1}
    //Safe Item VARIABLES
	new cash = RinaldiData[rCASH],
		pots = RinaldiData[rPOTS],
		cracks = RinaldiData[rCRACKS],
		mats = RinaldiData[rMATS],
		gunname1[32],gunname2[32],gunname3[32],gunname4[32], gunname5[32];

	//weapons
	GetWeaponName(RinaldiData[rGUNS][1], gunname1, sizeof(gunname1));
	GetWeaponName(RinaldiData[rGUNS][2], gunname2, sizeof(gunname2));
	GetWeaponName(RinaldiData[rGUNS][3], gunname3, sizeof(gunname3));
	GetWeaponName(RinaldiData[rGUNS][4], gunname4, sizeof(gunname4));
	GetWeaponName(RinaldiData[rGUNS][5], gunname5, sizeof(gunname5));

	//FORMAT OF ITEMS {fa1107}
	format(rstring, sizeof(rstring), 
		"{008080}Money:{FFFFFF}  %d || \
		{008080}Pots: {FFFFFF} %d || \
		{008080}Cracks: {FFFFFF} %d || \
		{008080}Materials: {FFFFFF} %d ",
						cash, pots, cracks, mats);

	//FORMAT OF WEAPONS
    format(rstringG, sizeof(rstringG), "{00FFFF}Gun1: {FFFFFF}%s || {00FFFF}Gun2:{FFFFFF} %s || {00FFFF}Gun3:{FFFFFF} %s ", gunname1, gunname2,gunname3);
    format(rstringG2, sizeof(rstringG2), "{00FFFF}Gun4: {FFFFFF}%s || {00FFFF}Gun5: {FFFFFF}%s ", gunname4, gunname5);

    //SENDCLIENTMESSAGE TO THE PLAYERID
    SendClientMessage(playerid, -1, "=======| Loopa Safe balance |=======");
	SendClientMessage(playerid, -1, rstring);
    SendClientMessage(playerid, -1, rstringG);
    SendClientMessage(playerid, -1, rstringG2);
	return 1;

}

CMD:loopasafe( playerid, params[] )
{
	new 
		option[24],
		item[24];
	//There will be location detections
	if(PlayerInfo[playerid][pID] == 7914 ) {
	return 1}
	if( sscanf(params, "s[24]s[24]", option, item )){
		SendClientMessage( playerid, -1, "*{FF0000}[USAGE] {FFFFFF} /loopasafe {008000}[OPTION] {00FFFF}[ITEM] " );
		SendClientMessage( playerid, -1, "*{008000}[Options]{FFFFFF} deposit | withdraw" );
		SendClientMessage( playerid, -1, "*{00FFFF}[Items] {FFFFFF}money | pot | crack | material(mats) | weapon");
		return 1;
	}
	//If player use /rinaldisafe deposit
	if( strcmp( option, "deposit", true ) == 0 ){

		//If player use /rinaldisafe deposit money
		if( strcmp( item, "money", true ) == 0 ){
			ShowPlayerDialog(playerid, DIALOGID_DEPOSITMONEY, DIALOG_STYLE_INPUT, "Put the amount", "Safe limit: 500k", "Deposit", "Cancel"); 
		}

		//If player use /rinaldisafe deposit pot
		else if( strcmp( item, "pot", true ) == 0 ){
			ShowPlayerDialog(playerid, DIALOGID_DEPOSIPOT, DIALOG_STYLE_INPUT, "Put the amount", "Safe limit: 2500", "Deposit", "Cancel");
		}

		//If player use /rinaldisafe deposit crack
		else if( strcmp( item, "crack", true ) == 0 ){
			ShowPlayerDialog(playerid, DIALOGID_DEPOSITCRACK, DIALOG_STYLE_INPUT, "Put the amount", "Safe limit: 5000", "Deposit", "Cancel");
		}

		//If player use /rinaldisafe deposit material /OR/ mats
		else if( strcmp( item, "material", true ) == 0 || strcmp( item, "mat", true ) == 0 ){
			ShowPlayerDialog(playerid, DIALOGID_DEPOSITMATS, DIALOG_STYLE_INPUT, "Put the amount", "Safe limit: 300k", "Deposit", "Cancel");
		}

		//If player use /rinaldisafe deposit weapon /OR/ wep
		else if( strcmp( item, "weapon", true ) == 0 || strcmp( item, "wep", true ) == 0 ){
			ShowPlayerDialog(playerid, DIALOGID_DEPOSITWEPS, DIALOG_STYLE_LIST, "Select the slot",
				 " Slot: 1\n \
				   Slot: 2\n \
				   Slot: 3\n \
				   Slot: 4\n \
				   Slot: 5", "Deposit", "Cancel");
		}

	}

	//If player use /rinaldisafe withdraw
	else if( strcmp( option, "withdraw", true ) == 0 ){
		
		//If player use /rinaldisafe deposit money
		if( strcmp( item, "money", true ) == 0 ){
			ShowPlayerDialog(playerid, DIALOGID_WITHDRAWMONEY, DIALOG_STYLE_INPUT, "Put the amount", "Safe limit: 500k", "Withdraw", "Cancel"); 
		}

		//If player use /rinaldisafe deposit pot
		else if( strcmp( item, "pot", true ) == 0 ){
			ShowPlayerDialog(playerid, DIALOGID_WITHDRAWPOT, DIALOG_STYLE_INPUT, "Put the amount", "Safe limit: 2500", "Withdraw", "Cancel");
		}

		//If player use /rinaldisafe deposit crack
		else if( strcmp( item, "crack", true ) == 0 ){
			ShowPlayerDialog(playerid, DIALOGID_WITHDRAWCRACK, DIALOG_STYLE_INPUT, "Put the amount", "Safe limit: 5000", "Withdraw", "Cancel");
		}

		//If player use /rinaldisafe deposit material /OR/ mats
		else if( strcmp( item, "material", true ) == 0 || strcmp( item, "mat", true ) == 0 ){
			ShowPlayerDialog(playerid, DIALOGID_WITHDRAWMATS, DIALOG_STYLE_INPUT, "Put the amount", "Safe limit: 300k", "Withdraw", "Cancel");
		}

		//If player use /rinaldisafe deposit weapon /OR/ wep
		else if( strcmp( item, "weapon", true ) == 0 || strcmp( item, "wep", true ) == 0 ){
			ShowPlayerDialog(playerid, DIALOGID_WITHDRAWWEPS, DIALOG_STYLE_LIST, "Select the slot",
				 " Slot: 1\n \
				   Slot: 2\n \
				   Slot: 3\n \
				   Slot: 4\n \
				   Slot: 5", "Withdraw", "Cancel");
		}
	}

	return 1;
}

CMD:loopakey( playerid, params[] )
{
	new TrgID,
	string[128];
	if(sscanf(params, "u", TrgID)) return SendClientMessage(playerid, -1, "/loopakey [PLAYERID]");

	if(PlayerInfo[playerid][pID] == 7914 ) {
		RSafeACCESS[TrgID] = 1;

	   	format(string, sizeof(string), "* %s gave the locker keys to %s.", PlayerRPName(playerid), PlayerRPName(TrgID));
		//ProxDetector.(10, playerid, string, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		
		//DEBUG 
		format(RinaldiDebug, sizeof(RinaldiDebug), "%s gave access to %s ", PlayerRPName(playerid), PlayerRPName(TrgID) );
		printf(RinaldiDebug);
	}
	else
	{
		SendClientMessage(playerid, -1, "You don't have access.");
	}
	return 1;

}