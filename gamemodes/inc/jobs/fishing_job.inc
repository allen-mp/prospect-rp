/*

	file:			jobs/fishing_job.inc

	description:	The fishing job for SARP.

	author:			Unknown

	improved by:	Jay Cortez

	date created:	29th March 2018

*/

forward FishingAnimation(playerid);
forward Fishing(playerid);
forward Catching(playerid, weight);
forward FishCost(fish);
forward UpdateTextDraw(playerid);

// Fish names
new FishNamesNumber = 17;
new FishNames[17][0] =
{
	{"Jacket"},
	{"Amberjack"},
	{"Grouper"},
	{"Red Snapper"},
	{"Trout"},
	{"Blue Marlin"},
	{"Mackeral"},
	{"Sea Bass"},
	{"Pike"},
	{"Sail Fish"},
	{"Tuna"},
	{"Eel"},
	{"Dolphin"},
	{"Shark"},
	{"Turtle"},
	{"Catfish"},
	{"Swordfish"}
};

// Command to display fishing related help
CMD:fishhelp(playerid, params[])
{
	DisplayDialogForPlayer(playerid, 106);
	return 1;
}

// Command to open the fishing store dialog
CMD:store(playerid, params[]) {
	if(!IsPlayerInRangeOfPoint(playerid, 3, 369.20, -2065.96, 979.80))
		return SendClientMessage(playerid, COLOR_GREY, "You are not at the fishing store counter.");

	DisplayDialogForPlayer(playerid, 102);
	return 1;
}

// Command to take out/put away fishing rod
CMD:userod(playerid, params[]) {
	if(!PlayerInfo[playerid][pFishRod])
		return SendClientMessage(playerid, COLOR_GREY, "You do not own a fishing rod.");

	if(!PlayerInfo[playerid][pBait])
		return SendClientMessage(playerid, COLOR_GREY, "You do not have any bait on you.");

	if(playerHoldingTazer[playerid])
		return SendClientMessage(playerid, COLOR_GREY, "You must put your tazer away first.");

	new fishStr[128];

	if(!PlayerHasRod[playerid])
	{
		PlayerHasRod[playerid] = 1;
		SetPlayerAttachedObject(playerid, INDEX_7, 18632, 6, 0.06, 0.04, 0.0, 180.0, 0.0, 0.0);
		SetPlayerArmedWeapon(playerid, 0);
		format(fishStr, sizeof(fishStr), "* %s takes out their fishing rod.", PlayerICName(playerid));
		ProxDetector(30.0, playerid, fishStr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		showNewbieTipMessage(playerid, 10);
	}
	else
	{
		if(IsFishing[playerid] == 1)
			return SendClientMessage(playerid, COLOR_GREY, "You can't put your fishing rod away while you're fishing.");

		PlayerHasRod[playerid] = 0;
		RemovePlayerAttachedObject(playerid, INDEX_7);
		format(fishStr, sizeof(fishStr), "* %s puts away their fishing rod.", PlayerICName(playerid));
		ProxDetector(30.0, playerid, fishStr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	}
	return 1;
}

// Command to see a list of fish the player currently has
CMD:listfish(playerid, params[]) {
	new fishStr[128];

	SendClientMessage(playerid, COLOR_WHITE, "Your current fishes:");

	if(Fishes[playerid][pWeight1]) {
		format(fishStr, sizeof(fishStr), "** (1) Fish: %s (%i LBS).", Fishes[playerid][pFish1], Fishes[playerid][pWeight1]);
		SendClientMessage(playerid, COLOR_GREY, fishStr);
	}
	else SendClientMessage(playerid, COLOR_GREY, "** (1) Empty.");

	if(Fishes[playerid][pWeight2]) {
		format(fishStr, sizeof(fishStr), "** (2) Fish: %s (%i LBS).", Fishes[playerid][pFish2], Fishes[playerid][pWeight2]);
		SendClientMessage(playerid, COLOR_GREY, fishStr);
	}
	else SendClientMessage(playerid, COLOR_GREY, "** (2) Empty.");

	if(Fishes[playerid][pWeight3]) {
		format(fishStr, sizeof(fishStr), "** (3) Fish: %s (%i LBS).", Fishes[playerid][pFish3], Fishes[playerid][pWeight3]);
		SendClientMessage(playerid, COLOR_GREY, fishStr);
	}
	else SendClientMessage(playerid, COLOR_GREY, "** (3) Empty.");

	if(Fishes[playerid][pWeight4]) {
		format(fishStr, sizeof(fishStr), "** (4) Fish: %s (%i LBS).", Fishes[playerid][pFish4], Fishes[playerid][pWeight4]);
		SendClientMessage(playerid, COLOR_GREY, fishStr);
	}
	else SendClientMessage(playerid, COLOR_GREY, "** (4) Empty.");

	if(Fishes[playerid][pWeight5]) {
		format(fishStr, sizeof(fishStr), "** (5) Fish: %s (%i LBS).", Fishes[playerid][pFish5], Fishes[playerid][pWeight5]);
		SendClientMessage(playerid, COLOR_GREY, fishStr);
	}
	else SendClientMessage(playerid, COLOR_GREY, "** (5) Empty.");

	// Action
	new actionStr[128];
	format(actionStr, sizeof(actionStr), "* %s takes a look at their fishes.", PlayerICName(playerid));
	SetPlayerChatBubble(playerid, actionStr, COLOR_PURPLE, 20.0, 15000);
	return 1;
}

// Command to sell fish in a 24/7
CMD:sellfish(playerid, params[]) {
	// This should be made more dynamic
	if(!IsPlayerInRangeOfPoint(playerid, 100, -30.875, -88.9609, 1004.53))
		return SendClientMessage(playerid, COLOR_GREY, "You are not in a 24/7.");

	new fishid;
	if(sscanf(params, "i", fishid))
		return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /sellfish [number]");

	if(fishid < 1 || fishid > 5)
		return SendClientMessage(playerid, COLOR_GREY, "Fish can't be below 1 or above 5.");

	// Get the name of the fish and the weight
	new fishName[50], fishWeight, fishFid;

	switch(fishid) {
		case 1: {
			format(fishName, sizeof(fishName), "%s", Fishes[playerid][pFish1]);
			fishWeight = Fishes[playerid][pWeight1];
			fishFid = Fishes[playerid][pFid1];
		}
		case 2: {
			format(fishName, sizeof(fishName), "%s", Fishes[playerid][pFish2]);
			fishWeight = Fishes[playerid][pWeight2];
			fishFid = Fishes[playerid][pFid2];
		}
		case 3: {
			format(fishName, sizeof(fishName), "%s", Fishes[playerid][pFish3]);
			fishWeight = Fishes[playerid][pWeight3];
			fishFid = Fishes[playerid][pFid3];
		}
		case 4: {
			format(fishName, sizeof(fishName), "%s", Fishes[playerid][pFish4]);
			fishWeight = Fishes[playerid][pWeight4];
			fishFid = Fishes[playerid][pFid4];
		}
		case 5: {
			format(fishName, sizeof(fishName), "%s", Fishes[playerid][pFish5]);
			fishWeight = Fishes[playerid][pWeight5];
			fishFid = Fishes[playerid][pFid5];
		}
	}

	if(!fishWeight)
		return SendClientMessage(playerid, COLOR_GREY, "You don't have a fish in that slot.");

	if(fishWeight < 20)
		return SendClientMessage(playerid, COLOR_GREY, "We are only interested in fish weighting 20 LBS or more.");

	// Calculate the price
	new fishPrice = FishCost(fishFid) * fishWeight;

	// Send a message
	new fishStr[128];
	format(fishStr, sizeof(fishStr), "* You have sold your %s (%i LBS) for $%i.", fishName, fishWeight, fishPrice);
	SendClientMessage(playerid, COLOR_LIGHTBLUE, fishStr);

	// Give money and get rid of the fish
	GiveMoney(playerid, fishPrice);
	ClearFishID(playerid, fishid);
	Fishes[playerid][pLastFish] = 0;
	return 1;
}

// Command to throw all fishes back into the sea
CMD:throwbackall(playerid, params[]) {
	if(	
		!Fishes[playerid][pWeight1] &&
		!Fishes[playerid][pWeight2] &&
		!Fishes[playerid][pWeight3] &&
		!Fishes[playerid][pWeight4] &&
		!Fishes[playerid][pWeight5]
	) return SendClientMessage(playerid, COLOR_GREY, "You are not carrying any fish.");

	ClearFishes(playerid);
	Fishes[playerid][pLastFish] = 0;
	SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have thrown back all of your fish.");
	return 1;
}

// Command to throw last fish back into the sea
CMD:throwback(playerid, params[]) {
	if(!Fishes[playerid][pLastFish])
		return SendClientMessage(playerid, COLOR_GREY, "You don't have any fish to throw back.");
	
	ClearFishID(playerid, Fishes[playerid][pLastFish]);
	Fishes[playerid][pLastFish] = 0;
	SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have thrown back your last fish.");
	return 1;
}

// Command to release (throw away) a fish
CMD:releasefish(playerid, params[]) {
	new fishid;
	if(sscanf(params, "i", fishid))
		return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /releasefish [number]");

	if(fishid < 1 || fishid > 5)
		return SendClientMessage(playerid, COLOR_GREY, "Fish can't be below 1 or above 5.");

	// Get the name of the fish and the weight
	new fishName[50], fishWeight;

	switch(fishid) {
		case 1: {
			format(fishName, sizeof(fishName), "%s", Fishes[playerid][pFish1]);
			fishWeight = Fishes[playerid][pWeight1];
		}
		case 2: {
			format(fishName, sizeof(fishName), "%s", Fishes[playerid][pFish2]);
			fishWeight = Fishes[playerid][pWeight2];
		}
		case 3: {
			format(fishName, sizeof(fishName), "%s", Fishes[playerid][pFish3]);
			fishWeight = Fishes[playerid][pWeight3];
		}
		case 4: {
			format(fishName, sizeof(fishName), "%s", Fishes[playerid][pFish4]);
			fishWeight = Fishes[playerid][pWeight4];
		}
		case 5: {
			format(fishName, sizeof(fishName), "%s", Fishes[playerid][pFish5]);
			fishWeight = Fishes[playerid][pWeight5];
		}
	}

	if(!fishWeight)
		return SendClientMessage(playerid, COLOR_GREY, "You don't have a fish in that slot.");

	// Send a message
	new fishStr[128];
	format(fishStr, sizeof(fishStr), "* You have released the %s (%i LBS).", fishName, fishWeight);
	SendClientMessage(playerid, COLOR_LIGHTBLUE, fishStr);

	// Get rid of the fish
	ClearFishID(playerid, fishid);
	Fishes[playerid][pLastFish] = 0;
	return 1;
}


// OnPlayerKeyStateChange hook for fishing related code
Hook:fish_OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
	{
		if(PRESSED(KEY_FIRE))
		{
			// Player is fishing
			if(TextDrawTimer[playerid])
			{
				LineTension[playerid] += 5;
				FishDist[playerid] -= 1;
			}
		}
		else if(PRESSED(KEY_AIM))
		{
			if(PlayerHasRod[playerid])
			{
				if(CatchingTimer[playerid]) return 1;

				new fishStr[128];

				if(!IsFishing[playerid])
				{
					if(IsNotAtFishPlace(playerid)) return SendClientMessage(playerid, COLOR_GRAD1, "You are not at the fishing pier or at the wharf in blueberry.");
					if(PlayerInfo[playerid][pLine] == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't own a fishing line.");
					format(fishStr, sizeof(fishStr), "* %s throws their fishing line out to sea.", PlayerICName(playerid));
					ProxDetector(30.0, playerid, fishStr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					//TogglePlayerControllable(playerid, false);
					//SetPlayerFacingAngle(playerid, 180.0);
					OnePlayAnim(playerid, "CAMERA", "piccrch_in", 4.0, 0, 0, 0, 0, 0);
					IsFishing[playerid] = 1;
					FishingTimer[playerid] = SetTimerEx("Fishing", 7 *1000, 1, "i", playerid);
					SetTimerEx("FishingAnimation", 1000, 0, "i", playerid);
				}
				else
				{
					format(fishStr, sizeof(fishStr), "* %s reels in their fishing line.", PlayerICName(playerid));
					ProxDetector(30.0, playerid, fishStr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					//TogglePlayerControllable(playerid, true);
					IsFishing[playerid] = 0;
					OnePlayAnim(playerid, "BD_FIRE", "wash_up", 4.0, 0, 0, 0, 0, 0);
					if(CatchingTimer[playerid])
					{
						KillTimer(CatchingTimer[playerid]);
						CatchingTimer[playerid] = 0;
					}
					if(FishingTimer[playerid])
					{
						KillTimer(FishingTimer[playerid]);
						FishingTimer[playerid] = 0;
					}
				}
				return 1;
			}
		}
	}
	return 1;
}

// This function calculates fish price based on the type of fish
stock FishCost(fish)
{
	switch(fish)
	{
		case 0:		return 10;
		case 1:		return 10;
		case 2:		return 30;
		case 3:		return 30;
		case 4:		return 25;
		case 5:		return 50;
		case 6:		return 20;
		case 7:		return 65;
		case 8:		return 80;
		case 9:		return 120;
		case 10:	return 60;
		case 11:	return 90;
		case 12:	return 70;
		case 13:	return 150;
		case 14:	return 12;
		case 15:	return 90;
		case 16:	return 70;
		case 17:	return 70;
		default:	return 10;
	}
	return 0;
}

// This function clears all the player's fishes
stock ClearFishes(playerid)
{
	if(!IsPlayerConnected(playerid))
		return false;
	
	Fishes[playerid][pFid1] = 0;
	Fishes[playerid][pFid2] = 0;
	Fishes[playerid][pFid3] = 0;
	Fishes[playerid][pFid4] = 0;
	Fishes[playerid][pFid5] = 0;
	Fishes[playerid][pWeight1] = 0;
	Fishes[playerid][pWeight2] = 0;
	Fishes[playerid][pWeight3] = 0;
	Fishes[playerid][pWeight4] = 0;
	Fishes[playerid][pWeight5] = 0;
	return true;
}

// This functions clears a fish slot of the player
stock ClearFishID(playerid, fishSlot)
{
	if(!IsPlayerConnected(playerid))
		return false;

	switch(fishSlot)
	{
		case 1:
		{
			Fishes[playerid][pWeight1] = 0;
			Fishes[playerid][pFid1] = 0;
		}
		case 2:
		{
			Fishes[playerid][pWeight2] = 0;
			Fishes[playerid][pFid2] = 0;
		}
		case 3:
		{
			Fishes[playerid][pWeight3] = 0;
			Fishes[playerid][pFid3] = 0;
		}
		case 4:
		{
			Fishes[playerid][pWeight4] = 0;
			Fishes[playerid][pFid4] = 0;
		}
		case 5:
		{
			Fishes[playerid][pWeight5] = 0;
			Fishes[playerid][pFid5] = 0;
		}
	}
	return true;
}

// OnPlayerDisconnect hook for fishing related code
Hook:fish_OnPlayerDisconnect(playerid, reason) {
	if(FishingTimer[playerid])
		KillTimer(FishingTimer[playerid]);

	if(PlayerHasRod[playerid])
		RemovePlayerAttachedObject(playerid, INDEX_3);

	if(CatchingTimer[playerid])
		KillTimer(CatchingTimer[playerid]);
}

// Called every 5 minutes
fish_OnFiveMinuteSync(playerid) {
	if(PlayerInfo[playerid][pFishes] > 4)   //reset fishing timer
	{
		if(FishCount[playerid] > 2)
		{
			PlayerInfo[playerid][pFishes] = 0;
		}
		else
		{
			FishCount[playerid] += 1;
		}
	}
}

// Gets the rod name from an integer
GetFishingRodName(rodID) {
	new rod[20];
	switch(rodID)
	{
		case 0: rod = "None";
		case 1: rod = "Old Rod";
		case 2: rod = "Beginners Rod";
		case 3: rod = "Fishermans Rod";
		case 4: rod = "Elite Rod";
	}
	return rod;
}

// Gets the bait name from an integer
GetFishingBaitName(baitID) {
	new bait[20];
	switch(baitID)
	{
		case 0: bait = "None";
		case 1: bait = "Weak Bait";
		case 2: bait = "Regular Bait";
		case 3: bait = "Strong Bait";
		case 4: bait = "Elite Bait";
	}
	return bait;
}

// Gets the fishing line name from an integer
GetFishingLineName(lineID) {
	new line[20];
	switch(lineID)
	{
		case 0: line = "None";
		case 1: line = "Regular Line";
	}
	return line;
}

// Performs the fishing animation
public FishingAnimation(playerid)
{
	if(IsPlayerConnected(playerid))
	{
		if(IsFishing[playerid])
			ApplyAnimationEx(playerid, "CAMERA", "camstnd_idleloop", 4.1, 1, 0, 0, 0, 0);
	}
	return 1;
}

// Fishing timer
public Fishing(playerid)
{
	new fishStr[128];

	if(IsPlayerConnected(playerid))
	{
		if(IsFishing[playerid] == 1)
		{
			if(IsNotAtFishPlace(playerid))
			{
				ClearAnimations(playerid);
				KillTimer(FishingTimer[playerid]);
				TogglePlayerControllable(playerid, true);
				return SendClientMessage(playerid, COLOR_GRAD1, "You are no longer fishing.");
			}
			if(PlayerInfo[playerid][pFishes] > 5)
			{
				return SendClientMessage(playerid, COLOR_GREY, "You caught too many fish recently, please wait.");
			}
			if(Fishes[playerid][pWeight1] > 0 && Fishes[playerid][pWeight2] > 0 && Fishes[playerid][pWeight3] > 0 && Fishes[playerid][pWeight4] > 0 && Fishes[playerid][pWeight5] > 0)
			{
				return SendClientMessage(playerid, COLOR_GREY, "You can't carry more than 5 fish, sell or release them first.");
			}
			if(!IsNotAtFishPlace(playerid))
			{
				new Rod, Bait, Level;
				ApplyAnimationEx(playerid, "CAMERA", "camstnd_idleloop", 4.1, 1, 0, 0, 0, 0);
				Level = PlayerInfo[playerid][pFishSkill];
				if(Level >= 0 && Level <= 50)
				{
					Caught[playerid] = random(35) -7;
				}
				else if(Level >= 51 && Level <= 100)
				{
					Caught[playerid] = random(55) -20;
				}
				else if(Level >= 101 && Level <= 200)
				{
					Caught[playerid] = random(105) -50;
				}
				else if(Level >= 201 && Level <= 400)
				{
					Caught[playerid] = random(165) -60;
				}
				else if(Level >= 401)
				{
					Caught[playerid] = random(185) -70;
				}
				//
				Rod = PlayerInfo[playerid][pFishRod];
				switch(Rod)
				{
					case 1:
					{
						Caught[playerid] += random(5) -5;
					}
					case 2:
					{
						Caught[playerid] += random(10) -5;
					}
					case 3:
					{
						Caught[playerid] += random(15) -5;
					}
					case 4:
					{
						Caught[playerid] += random(20) -5;
					}
				}
				//
				Bait = PlayerInfo[playerid][pBait];
				if(Bait > 0)
				{
					switch(Bait)
					{
						case 1:
						{
							Caught[playerid] += random(2);
						}
						case 2:
						{
							Caught[playerid] += random(4);
						}
						case 3:
						{
							Caught[playerid] += random(10);
						}
						case 4:
						{
							Caught[playerid] += random(20);
						}
					}
				}
				//
				switch(Caught[playerid])
				{
					case -200..0:
						return SendClientMessage(playerid, COLOR_GREY, "Nothing..");
					case 1:
						return SendClientMessage(playerid, COLOR_GREY, "You reeled in a jacket and threw it away.");
					case 2:
						return SendClientMessage(playerid, COLOR_GREY, "You reeled in some pants and threw them away.");
					case 3:
					{
						Condom[playerid] ++;
						return SendClientMessage(playerid, COLOR_LIGHTBLUE, "You reeled in a used condom and decided to keep it.");
					}
					case 4:
					{
						PlayerInfo[playerid][pScrew] ++;
						return SendClientMessage(playerid, COLOR_LIGHTBLUE, "You reeled in a rusty screwdriver and decided to keep it.");
					}
					case 5:
						return SendClientMessage(playerid, COLOR_GREY, "You reeled in a pair of old trousers and threw them away.");
					case 6:
					{
						new mrand = random(2000);
						format(fishStr, sizeof(fishStr), "You reeled in a bag of money containing $%d, looked around and decided to keep it.", mrand);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, fishStr);
						GiveMoney(playerid, mrand);
						return 1;
					}
				}
				CatchingTimer[playerid] = SetTimerEx("Catching", 600, 1, "ii", playerid, Caught[playerid]);
				TextDrawTimer[playerid] = SetTimerEx("UpdateTextDraw", 100, 1, "i", playerid);
				TextDrawShowForPlayer(playerid, Text:FishingOutline);
				TextDrawShowForPlayer(playerid, Text:FishingBackground);
				TextDrawTextSize(FishingBar, 549.000000, 0.000000);
				TextDrawShowForPlayer(playerid, Text:FishingBar);
				FishDist[playerid] = 30 +random(30);
				LineTension[playerid] = 60;
				SendClientMessage(playerid, COLOR_GREY, "Line tugs, reel it in quickly! (Left mouse button)");
				showNewbieTipMessage(playerid, 11); //fishing bar
				if(PlayerInfo[playerid][pBaitAmount] == 1)
				{
					SendClientMessage(playerid, COLOR_GREY, "You have used up all your bait.");
				}
				PlayerInfo[playerid][pBaitAmount]--;
				if(PlayerInfo[playerid][pBaitAmount] <= 0)
				{
					PlayerInfo[playerid][pBaitAmount] = 0;
				}
				if(FishingTimer[playerid])
				{
					KillTimer(FishingTimer[playerid]);
					FishingTimer[playerid] = 0;
				}
			}
			else
			{
				ClearAnimations(playerid);
				if(FishingTimer[playerid])
				{
					KillTimer(FishingTimer[playerid]);
					FishingTimer[playerid] = 0;
				}
				TogglePlayerControllable(playerid, true);
				return SendClientMessage(playerid, COLOR_GRAD1, "* You are no longer fishing.");
			}
		}
	}
	return 1;
}

// Catching fish timer
public Catching(playerid, weight)
{
	new fishStr[128];

	if(IsPlayerConnected(playerid))
	{
		if(!IsNotAtFishPlace(playerid))
		{
			LineTension[playerid] = LineTension[playerid] -5;
			if(LineTension[playerid] <= 0)
			{
				if(TextDrawTimer[playerid])
				{
					KillTimer(TextDrawTimer[playerid]);
					TextDrawTimer[playerid] = 0;
				}
				if(CatchingTimer[playerid])
				{
					KillTimer(CatchingTimer[playerid]);
					CatchingTimer[playerid] = 0;
				}
				if(FishingTimer[playerid])
				{
					KillTimer(FishingTimer[playerid]);
					FishingTimer[playerid] = 0;
				}
				KillTimer(FishingTimer[playerid]);
				TogglePlayerControllable(playerid, true);
				LineTension[playerid] = 0;
				IsFishing[playerid] = 0;
				TextDrawHideForPlayer(playerid, Text:FishingBar);
				TextDrawHideForPlayer(playerid, Text:FishingBackground);
				TextDrawHideForPlayer(playerid, Text:FishingOutline);
				ClearAnimations(playerid);
				return SendClientMessage(playerid, COLOR_GREY, "The fish got away..");
			}
			if(LineTension[playerid] >= 98)
			{
				if(TextDrawTimer[playerid])
				{
					KillTimer(TextDrawTimer[playerid]);
					TextDrawTimer[playerid] = 0;
				}
				if(CatchingTimer[playerid])
				{
					KillTimer(CatchingTimer[playerid]);
					CatchingTimer[playerid] = 0;
				}
				if(FishingTimer[playerid])
				{
					KillTimer(FishingTimer[playerid]);
					FishingTimer[playerid] = 0;
				}
				ClearAnimations(playerid);
				IsFishing[playerid] = 0;
				TogglePlayerControllable(playerid, true);
				LineTension[playerid] = 0;
				TextDrawHideForPlayer(playerid, Text:FishingBar);
				TextDrawHideForPlayer(playerid, Text:FishingBackground);
				TextDrawHideForPlayer(playerid, Text:FishingOutline);
				PlayerInfo[playerid][pLine] = 0;
				return SendClientMessage(playerid, COLOR_GREY, "Line snapped.");
			}
			if(FishDist[playerid] <= 0)
			{
				if(TextDrawTimer[playerid])
				{
					KillTimer(TextDrawTimer[playerid]);
					TextDrawTimer[playerid] = 0;
				}
				if(CatchingTimer[playerid])
				{
					KillTimer(CatchingTimer[playerid]);
					CatchingTimer[playerid] = 0;
				}
				LineTension[playerid] = 0;
				TextDrawHideForPlayer(playerid, Text:FishingBar);
				TextDrawHideForPlayer(playerid, Text:FishingBackground);
				TextDrawHideForPlayer(playerid, Text:FishingOutline);
				PlayerInfo[playerid][pFishes] += 1;
				PlayerInfo[playerid][pFishSkill] += 1;
				new fstring[MAX_PLAYER_NAME], rand;
				rand = random(FishNamesNumber);
				format(fstring, sizeof(fstring), "%s", FishNames[rand]);
				if(Fishes[playerid][pWeight1] == 0)
				{
					strmid(Fishes[playerid][pFish1], fstring, 0, strlen(fstring), 255);
					Fishes[playerid][pWeight1] = Caught[playerid];
					format(fishStr, sizeof(fishStr), "You have caught a %s, which weighs %d pounds.", Fishes[playerid][pFish1], Caught[playerid]);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, fishStr);
					Fishes[playerid][pLastFish] = 1;
					Fishes[playerid][pFid1] = rand;
				}
				else if(Fishes[playerid][pWeight2] == 0)
				{
					strmid(Fishes[playerid][pFish2], fstring, 0, strlen(fstring), 255);
					Fishes[playerid][pWeight2] = Caught[playerid];
					format(fishStr, sizeof(fishStr), "You have caught a %s, which weighs %d pounds.", Fishes[playerid][pFish2], Caught[playerid]);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, fishStr);
					Fishes[playerid][pLastFish] = 2;
					Fishes[playerid][pFid2] = rand;
				}
				else if(Fishes[playerid][pWeight3] == 0)
				{
					strmid(Fishes[playerid][pFish3], fstring, 0, strlen(fstring), 255);
					Fishes[playerid][pWeight3] = Caught[playerid];
					format(fishStr, sizeof(fishStr), "You have caught a %s, which weighs %d pounds.", Fishes[playerid][pFish3], Caught[playerid]);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, fishStr);
					Fishes[playerid][pLastFish] = 3;
					Fishes[playerid][pFid3] = rand;
				}
				else if(Fishes[playerid][pWeight4] == 0)
				{
					strmid(Fishes[playerid][pFish4], fstring, 0, strlen(fstring), 255);
					Fishes[playerid][pWeight4] = Caught[playerid];
					format(fishStr, sizeof(fishStr), "You have caught a %s, which weighs %d pounds.", Fishes[playerid][pFish4], Caught[playerid]);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, fishStr);
					Fishes[playerid][pLastFish] = 4;
					Fishes[playerid][pFid4] = rand;
				}
				else if(Fishes[playerid][pWeight5] == 0)
				{
					strmid(Fishes[playerid][pFish5], fstring, 0, strlen(fstring), 255);
					Fishes[playerid][pWeight5] = Caught[playerid];
					format(fishStr, sizeof(fishStr), "You have caught a %s, which weighs %d pounds.", Fishes[playerid][pFish5], Caught[playerid]);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, fishStr);
					Fishes[playerid][pLastFish] = 5;
					Fishes[playerid][pFid5] = rand;
				}
				//
				if(Caught[playerid] > PlayerInfo[playerid][pBiggestFish])
				{
					format(fishStr, sizeof(fishStr), "You beat your old record of %d pounds! Your new biggest fish is %d pounds.", PlayerInfo[playerid][pBiggestFish], Caught[playerid]);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, fishStr);
					PlayerInfo[playerid][pBiggestFish] = Caught[playerid];
					OnePlayAnim(playerid, "CASINO", "manwind", 4.0, 0, 0, 0, 0, 0);
				}
				//
				if(PlayerInfo[playerid][pFishSkill] == 50)
				{
					SendClientMessage(playerid, COLOR_YELLOW, "* Your Fishing Skill is now Level 2, you can now catch Heavier Fish.");
				}
				else if(PlayerInfo[playerid][pFishSkill] == 250)
				{
					SendClientMessage(playerid, COLOR_YELLOW, "* Your Fishing Skill is now Level 3, you can now catch Heavier Fish.");
				}
				else if(PlayerInfo[playerid][pFishSkill] == 500)
				{
					SendClientMessage(playerid, COLOR_YELLOW, "* Your Fishing Skill is now Level 4, you can now catch Heavier Fish.");
				}
				else if(PlayerInfo[playerid][pFishSkill] == 1000)
				{
					SendClientMessage(playerid, COLOR_YELLOW, "* Your Fishing Skill is now Level 5, you can now catch Heavier Fish.");
				}
				//
				FishingTimer[playerid] = SetTimerEx("Fishing", 7 *1000, 1, "i", playerid);
				ApplyAnimationEx(playerid, "CAMERA", "camstnd_idleloop", 4.1, 1, 0, 0, 0, 0);
				showNewbieTipMessage(playerid, 12); //sellfish
			}
		}
		else
		{
			TextDrawHideForPlayer(playerid, Text:FishingBar);
			TextDrawHideForPlayer(playerid, Text:FishingBackground);
			TextDrawHideForPlayer(playerid, Text:FishingOutline);
			TogglePlayerControllable(playerid, true);
			IsFishing[playerid] = 0;
			LineTension[playerid] = 0;
			KillTimer(TextDrawTimer[playerid]);
			TextDrawTimer[playerid] = 0;
			KillTimer(CatchingTimer[playerid]);
			CatchingTimer[playerid] = 0;
			KillTimer(FishingTimer[playerid]);
			FishingTimer[playerid] = 0;
			return SendClientMessage(playerid, COLOR_GREY, "You are no longer fishing.");
		}
	}
	return 0;
}

// Update the fish textdraw
public UpdateTextDraw(playerid)
{
	if(IsPlayerConnected(playerid))
	{
		TextDrawHideForPlayer(playerid, Text:FishingBar);
		if(LineTension[playerid] >= 0 && LineTension[playerid] <= 2)
		{
			TextDrawTextSize(FishingBar, 549.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 3 && LineTension[playerid] <= 5)
		{
			TextDrawTextSize(FishingBar, 550.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 6 && LineTension[playerid] <= 8)
		{
			TextDrawTextSize(FishingBar, 552.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 9 && LineTension[playerid] <= 11)
		{
			TextDrawTextSize(FishingBar, 553.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 12 && LineTension[playerid] <= 14)
		{
			TextDrawTextSize(FishingBar, 554.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 15 && LineTension[playerid] <= 17)
		{
			TextDrawTextSize(FishingBar, 556.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 18 && LineTension[playerid] <= 20)
		{
			TextDrawTextSize(FishingBar, 557.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 21 && LineTension[playerid] <= 23)
		{
			TextDrawTextSize(FishingBar, 559.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 24 && LineTension[playerid] <= 26)
		{
			TextDrawTextSize(FishingBar, 560.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 27 && LineTension[playerid] <= 29)
		{
			TextDrawTextSize(FishingBar, 562.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 30 && LineTension[playerid] <= 32)
		{
			TextDrawTextSize(FishingBar, 563.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 33 && LineTension[playerid] <= 35)
		{
			TextDrawTextSize(FishingBar, 565.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 36 && LineTension[playerid] <= 38)
		{
			TextDrawTextSize(FishingBar, 566.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 39 && LineTension[playerid] <= 41)
		{
			TextDrawTextSize(FishingBar, 568.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 42 && LineTension[playerid] <= 44)
		{
			TextDrawTextSize(FishingBar, 569.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 45 && LineTension[playerid] <= 47)
		{
			TextDrawTextSize(FishingBar, 571.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 48 && LineTension[playerid] <= 50)
		{
			TextDrawTextSize(FishingBar, 572.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 51 && LineTension[playerid] <= 53)
		{
			TextDrawTextSize(FishingBar, 574.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 54 && LineTension[playerid] <= 56)
		{
			TextDrawTextSize(FishingBar, 576.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 57 && LineTension[playerid] <= 59)
		{
			TextDrawTextSize(FishingBar, 578.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 60 && LineTension[playerid] <= 62)
		{
			TextDrawTextSize(FishingBar, 580.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 63 && LineTension[playerid] <= 65)
		{
			TextDrawTextSize(FishingBar, 582.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 66 && LineTension[playerid] <= 68)
		{
			TextDrawTextSize(FishingBar, 584.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 69 && LineTension[playerid] <= 71)
		{
			TextDrawTextSize(FishingBar, 586.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 72 && LineTension[playerid] <= 74)
		{
			TextDrawTextSize(FishingBar, 587.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 75 && LineTension[playerid] <= 77)
		{
			TextDrawTextSize(FishingBar, 588.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 78 && LineTension[playerid] <= 80)
		{
			TextDrawTextSize(FishingBar, 590.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 81 && LineTension[playerid] <= 83)
		{
			TextDrawTextSize(FishingBar, 592.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 84 && LineTension[playerid] <= 86)
		{
			TextDrawTextSize(FishingBar, 594.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 87 && LineTension[playerid] <= 89)
		{
			TextDrawTextSize(FishingBar, 596.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 90 && LineTension[playerid] <= 92)
		{
			TextDrawTextSize(FishingBar, 598.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 93 && LineTension[playerid] <= 95)
		{
			TextDrawTextSize(FishingBar, 600.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 96 && LineTension[playerid] <= 98)
		{
			TextDrawTextSize(FishingBar, 602.000000, 0.000000);
		}
		else if(LineTension[playerid] >= 99 && LineTension[playerid] <= 100)
		{
			TextDrawTextSize(FishingBar, 604.000000, 0.000000);
		}
		TextDrawShowForPlayer(playerid, Text:FishingBar);
	}
	return 1;
}