/*

	file:			syndicate_safe.inc

	description:	A safe to hold drugs for the Syndicate group (requested)

	author:			Jay Cortez

	date created:	27th February 2018

*/

#define SYNSAFE_FILE			"syndicate_safe.ini"
#define SYNSAFE_MAX_COKE		3000
#define SYNSAFE_MAX_METH		3000
#define SYNSAFE_MAX_CANNABIS	3000

enum syndicateSafeEnum {
	s_Coke,
	s_Meth,
	s_Cannabis,
	bool:s_Loaded
};
new SyndicateSafe[syndicateSafeEnum];

// Call the load function on gamemode init
Hook:syndsafe_OnGameModeInit() {
	SyndicateSafe[s_Loaded] = false;
	LoadSyndicateSafe();
}

// Command to interact with the safe

CMD:gsafe(playerid, params[]) {
	if(PlayerInfo[playerid][pGroup] != GROUP_CRIME)
		return AdmErrorMsg;

	if(!SyndicateSafe[s_Loaded])
		return SendClientMessage(playerid, COLOR_GREY, "Safe contents could not be loaded. Please contact a developer.");

	#if DEVMODE == 0
	if(!IsPlayerNearSyndicateHQ(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You are not at the safe.");
	#endif

	new actionStr[20], actionItem[20], actionItemAmount;
	if(sscanf(params, "s[20]s[20]i", actionStr, actionItem, actionItemAmount)) {
		SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /syndi(cate)safe [put/get] [coke/meth/cannabis] [amount]");
		
		new infoStr[128];
		format(infoStr, sizeof(infoStr), "Safe contents: %i.0g Coke, %i.0g Meth, %i.0g Cannabis.",
			SyndicateSafe[s_Coke],
			SyndicateSafe[s_Meth],
			SyndicateSafe[s_Cannabis]
		);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, infoStr);

		// Action
		format(infoStr, sizeof(infoStr), "* %s opens the safe and checks what's inside.", PlayerICName(playerid));
		SetPlayerChatBubble(playerid, infoStr, COLOR_PURPLE, 20.0, 15000);
		return 1;
	}

	if(actionItemAmount < 1)
		return SendClientMessage(playerid, COLOR_GREY, "Invalid amount.");

	new infoStr[128];

	// Store items
	if(strmatch(actionStr, "put")) {
		if(strmatch(actionItem, "coke")) {
			if(PlayerInfo[playerid][pCocaine] < actionItemAmount)
				return SendClientMessage(playerid, COLOR_GREY, "You do not have enough coke.");

			if(SyndicateSafe[s_Coke] + actionItemAmount > SYNSAFE_MAX_COKE)
				return SendClientMessage(playerid, COLOR_GREY, "The safe cannot hold this much coke.");

			PlayerInfo[playerid][pCocaine] -= actionItemAmount;
			SyndicateSafe[s_Coke] += actionItemAmount;
			format(actionItem, sizeof(actionItem), "coke");
		}
		else if(strmatch(actionItem, "meth")) {
			if(PlayerInfo[playerid][pMeth] < actionItemAmount)
				return SendClientMessage(playerid, COLOR_GREY, "You do not have enough meth.");

			if(SyndicateSafe[s_Meth] + actionItemAmount > SYNSAFE_MAX_METH)
				return SendClientMessage(playerid, COLOR_GREY, "The safe cannot hold this much meth.");

			PlayerInfo[playerid][pMeth] -= actionItemAmount;
			SyndicateSafe[s_Meth] += actionItemAmount;
			format(actionItem, sizeof(actionItem), "meth");
		}
		else if(strmatch(actionItem, "cannabis")) {
			if(PlayerInfo[playerid][pCannabis] < actionItemAmount)
				return SendClientMessage(playerid, COLOR_GREY, "You do not have enough cannabis.");

			if(SyndicateSafe[s_Cannabis] + actionItemAmount > SYNSAFE_MAX_CANNABIS)
				return SendClientMessage(playerid, COLOR_GREY, "The safe cannot hold this much cannabis.");

			PlayerInfo[playerid][pCannabis] -= actionItemAmount;
			SyndicateSafe[s_Cannabis] += actionItemAmount;
			format(actionItem, sizeof(actionItem), "cannabis");
		}
		else return SendClientMessage(playerid, COLOR_GREY, "Invalid item.");

		// Send info message
		format(infoStr, sizeof(infoStr), "You have deposited %i.0g of %s into the locker.",
			actionItemAmount,
			actionItem
		);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, infoStr);
	}
	// Retrieve items from safe
	else if(strmatch(actionStr, "get")) {
		if(strmatch(actionItem, "coke")) {
			if(SyndicateSafe[s_Coke] < actionItemAmount)
				return SendClientMessage(playerid, COLOR_GREY, "There is not enough coke in the safe.");

			PlayerInfo[playerid][pCocaine] += actionItemAmount;
			SyndicateSafe[s_Coke] -= actionItemAmount;
			format(actionItem, sizeof(actionItem), "coke");
		}
		else if(strmatch(actionItem, "meth")) {
			if(SyndicateSafe[s_Meth] < actionItemAmount)
				return SendClientMessage(playerid, COLOR_GREY, "There is not enough meth in safe.");

			PlayerInfo[playerid][pMeth] += actionItemAmount;
			SyndicateSafe[s_Meth] -= actionItemAmount;
			format(actionItem, sizeof(actionItem), "meth");
		}
		else if(strmatch(actionItem, "cannabis")) {
			if(SyndicateSafe[s_Cannabis] < actionItemAmount)
				return SendClientMessage(playerid, COLOR_GREY, "There is not enough cannabis in the safe.");

			PlayerInfo[playerid][pCannabis] += actionItemAmount;
			SyndicateSafe[s_Cannabis] -= actionItemAmount;
			format(actionItem, sizeof(actionItem), "cannabis");
		}
		else return SendClientMessage(playerid, COLOR_GREY, "Invalid item.");

		// Send info message
		format(infoStr, sizeof(infoStr), "You have taken %i.0g of %s from the locker.",
			actionItemAmount,
			actionItem
		);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, infoStr);
	}

	SaveSyndicateSafe();
	return 1;
}

// Loads the syndicate safe contents
LoadSyndicateSafe() {
	if(dini_Exists(SYNSAFE_FILE)) {
		SyndicateSafe[s_Coke] = dini_Int(SYNSAFE_FILE, "s_Coke");
		SyndicateSafe[s_Meth] = dini_Int(SYNSAFE_FILE, "s_Meth");
		SyndicateSafe[s_Cannabis] = dini_Int(SYNSAFE_FILE, "s_Cannabis");
	}

	SyndicateSafe[s_Loaded] = true;
}

// Saves the contents of the syndicate safe
SaveSyndicateSafe() {
	if(!dini_Exists(SYNSAFE_FILE)) dini_Create(SYNSAFE_FILE);

	dini_IntSet(SYNSAFE_FILE, "s_Coke", SyndicateSafe[s_Coke]);
	dini_IntSet(SYNSAFE_FILE, "s_Meth", SyndicateSafe[s_Meth]);
	dini_IntSet(SYNSAFE_FILE, "s_Cannabis", SyndicateSafe[s_Cannabis]);
}