/*
	Drug System

	-> UseDrug(playerid, drugid);
	-> DropDrug(playerid, amount, object, Float:x, Float:y, Float:z);
	-> CheckPlayerDrugs(playerid);
	-> GiveDrug(playerid, targetid, drugid, amount);
	-> MixDrugs(playerid, drug1, drug2, amount1, amount2)
 */

#define 	MAX_DROPPED_DRUGS	(100)

forward UseDrug(playerid, drugid);
forward DropDrug(playerid, dobject, drugid, amount, Float:x, Float:y, Float:z);
forward PickUpDrug(playerid);
forward CheckPlayerDrugs(playerid);
forward GiveDrug(playerid, targetid, drugid, amount);
forward GetDrugInfo(playerid);
forward UseDrugs(playerid); // Timer
forward DrugEffects(playerid); // Timer
forward MixDrugs(playerid, drug1, drug2, amount1, amount2);
forward OfferDrug(playerid, target, drugid, amount);

enum { // Drug IDs
	DRUG_CANNABIS,
	DRUG_COCAINE,
	DRUG_METH,
	DRUG_XANAX,
	DRUG_CODEINE,
	DRUG_PROMETHAZINE,
	DRUG_LEAN
};

enum { // Object IDs
	DRUG_SHOE_OBJECT = 2694,
	DRUG_BOX_OBJECT = 2912,
	DRUG_DUFFLE_BAG = 11745
}

enum droppedDrugs {
	droppedID,
	bool:droppedDropped, // Check to see if the drug ID is dropped or not
	droppedDrug,
	Float:droppedPosition[3],
	Float:droppedRotPosition[3],
	droppedDropper[MAX_PLAYER_NAME],
	droppedDrugName[64],
	droppedObjName[64],
	droppedObject,
	droppedGrams, // how many grams in each object
	droppedMaxGrams // max grams an obj can hold

};
new DrugInfo[MAX_DROPPED_DRUGS][droppedDrugs];

new drugofferplayer[MAX_PLAYERS], drugofferid[MAX_PLAYERS], drugofferamount[MAX_PLAYERS];

GetDrugName(drugid)
{
	new drugname[64];
	switch(drugid)
	{
		case DRUG_CANNABIS: drugname = "Cannabis";
		case DRUG_COCAINE: drugname = "Cocaine";
		case DRUG_METH: drugname = "Meth";
		case DRUG_XANAX: drugname = "Xanax";
		case DRUG_PROMETHAZINE: drugname = "Promethazine";
		case DRUG_CODEINE: drugname = "Codeine";
		case DRUG_LEAN: drugname = "Lean";
	}
	return drugname;
}

GetDrugEffectString(drugid)
{
	new drugline[128];
	switch(drugid)
	{
		case DRUG_CANNABIS: drugline = "You've smoked some cannabis.";
		case DRUG_COCAINE: drugline = "You've snorted some cocaine";
		case DRUG_METH: drugline = "You've smoked some meth.";
		case DRUG_XANAX: drugline = "You've taken some xanax.";
		case DRUG_PROMETHAZINE: drugline = "You've taken promethazine.";
		case DRUG_CODEINE: drugline = "You've taken codeine.";
		case DRUG_LEAN: drugline = "You've drank some lean.";
	}
	return drugline;
}

GetPlayerDrugBubbleString(playerid, drugid)
{
	new drugline[128];
	switch(drugid)
	{
		case DRUG_CANNABIS:
		{
			format(drugline, sizeof drugline, "%s's eyes would look blood shot and tired.", PlayerICName(playerid));
		}
		case DRUG_COCAINE:
		{
			format(drugline, sizeof drugline, "%s's eyes would look glazed over, and would seem clueless to surroundings.", PlayerICName(playerid));
		}
		case DRUG_METH:
		{
			format(drugline, sizeof drugline, "%s would look lost, and very fatigued.", PlayerICName(playerid));
		}
		case DRUG_XANAX:
		{
			format(drugline, sizeof drugline, "%s would begin to look drowsy and tired.", PlayerICName(playerid));
		}
		case DRUG_PROMETHAZINE:
		{
			format(drugline, sizeof drugline, "%s's pupils begin to dialate.", PlayerICName(playerid));
		}
		case DRUG_CODEINE:
		{
			format(drugline, sizeof drugline, "%s slowly begins look fatigued as time goes on.", PlayerICName(playerid));
		}
		case DRUG_LEAN:
		{
			format(drugline, sizeof drugline, "%s reacts very sluggishly and slurrs a lot.", PlayerICName(playerid));
		}
	}
	return drugline;
}

public DrugEffects(playerid)
{
	SetPlayerWeather(playerid, g_WeatherID);

	new hour, minute, second;

	gettime(hour, minute, second);

	hour = FixHour(hour);

	SetPlayerTime(playerid, hour, minute);
	return 1;
}
public UseDrugs(playerid)
{
	if(!IsPlayerConnected(playerid)) return 0;

	if(UseDrugsTimer[playerid])
	{
		UseDrugsTimer[playerid] = 0;
	}
	return 1;
}

new PlayerMixingDrug[MAX_PLAYERS],
	MixingDrugTimerID[MAX_PLAYERS];

public MixDrugs(playerid, drug1, drug2, amount1, amount2)
{
	if( (drug1 == DRUG_PROMETHAZINE && drug2 == DRUG_CODEINE) || (drug1 == DRUG_CODEINE && drug2 == DRUG_PROMETHAZINE) )
	{
		if(PlayerInfo[playerid][pCodeine] < 5)
			return SendClientMessage(playerid, COLOR_GREY, "You don't have enough drugs to perform this action.");

		if(PlayerInfo[playerid][pPromethazine] < 5)
			return SendClientMessage(playerid, COLOR_GREY, "You don't have enough drugs to perform this action.");

		if(PlayerMixingDrug[playerid] != 0)
			return SendClientMessage(playerid, COLOR_GREY, "You're already mixing drugs.");

		PlayerInfo[playerid][pPromethazine] -= amount1;
		PlayerInfo[playerid][pCodeine] -= amount2;

		MixingDrugMessage(playerid);
		TogglePlayerControllable(playerid, 0);
	}
	return 1;
}

forward MixingDrugMessage(playerid);
public MixingDrugMessage(playerid)
{
  PlayerMixingDrug[playerid] = 5;
  GameTextForPlayer(playerid, "~g~MIXING DRUG~n~~w~(5 SECONDS LEFT)", 1000, 3);
  MixingDrugTimerID[playerid] = SetTimerEx("PlayerMixingDrugEx", 1000, true, "i", playerid);

  return 1;
}

forward PlayerMixingDrugEx(playerid);
public PlayerMixingDrugEx(playerid)
{
  new drugmsg[128];
  if(PlayerMixingDrug[playerid] > 0) PlayerMixingDrug[playerid]--;
  if(PlayerMixingDrug[playerid] == 0)
  {
  	TogglePlayerControllable(playerid, 1);
  	PlayerInfo[playerid][pLean]++;
  	SendClientMessage(playerid, COLOR_LIGHTBLUE, "You've used 5 fluid ounces of promethazine and codeine to make 1 fluid ounce of lean.");
 	KillTimer(MixingDrugTimerID[playerid]);
  }
  format(drugmsg, sizeof(drugmsg), "~g~MIXING DRUG~n~~w~(%d SECONDS LEFT)", PlayerMixingDrug[playerid]);
  GameTextForPlayer(playerid, drugmsg, 1000, 3);
  return 1;
}

public UseDrug(playerid, drugid)
{
	if(UseDrugsTimer[playerid] == 1)
		return SendClientMessage(playerid, COLOR_GREY, "You need to wait prior to consuming more drugs.");

	if(IsNearBank(playerid) && bankactive) 
		return SendClientMessage(playerid, COLOR_GREY, "You can not use this inside/outside the bank during an active bank robbery.");

	if(drugid == DRUG_CANNABIS)
	{
		if(PlayerInfo[playerid][pCannabis] == 0)
			return SendClientMessage(playerid, COLOR_LIGHTBLUE, "ERROR: {FFFFFF}You do not have enough cannabis!");

		SetPlayerChatBubble(playerid, GetPlayerDrugBubbleString(playerid, DRUG_CANNABIS), COLOR_PURPLE, 5.0000, 5 * 1000);

		new Float:dHP;
		GetPlayerHealth(playerid, dHP);

		if(dHP > 85)
			SetPlayerHealth(playerid, 100);
		else SetPlayerHealth(playerid, dHP + 10.0);

		PlayerInfo[playerid][pCannabis] -= 1;

		SetPlayerDrunkLevel(playerid, 5000);
		SetPlayerTime(playerid, 22, 0);
		SetPlayerWeather(playerid, 10004);
	}
	else if(drugid == DRUG_COCAINE)
	{
		if(PlayerInfo[playerid][pCocaine] == 0)
			return SendClientMessage(playerid, COLOR_LIGHTBLUE, "ERROR: {FFFFFF}You do not have enough cocaine!");

		SetPlayerChatBubble(playerid, GetPlayerDrugBubbleString(playerid, DRUG_COCAINE), COLOR_PURPLE, 5.0000, 5 * 1000);

		new Float:dHP, Float:dAP;
		GetPlayerHealth(playerid, dHP);
		GetPlayerArmour(playerid, dAP);
		
		if(dAP > 85)
			SetPlayerArmour(playerid, 100);
		else SetPlayerArmour(playerid, dAP + 8);

		PlayerInfo[playerid][pCocaine] -= 1;

		SetPlayerDrunkLevel(playerid, 5000);
		SetPlayerTime(playerid, 22, 0);
		SetPlayerWeather(playerid, 467);
	}
	else if(drugid == DRUG_METH)
	{
		if(PlayerInfo[playerid][pMeth] == 0)
			return SendClientMessage(playerid, COLOR_LIGHTBLUE, "ERROR: {FFFFFF}You do not have enough meth!");
	
		SetPlayerChatBubble(playerid, GetPlayerDrugBubbleString(playerid, DRUG_METH), COLOR_PURPLE, 5.0000, 5 * 1000);

		new Float:dHP, Float:dAP;
		PlayerInfo[playerid][pMeth] -= 1;

		GetPlayerHealth(playerid, dHP);

		if(dHP > 85)
			SetPlayerHealth(playerid, 100);
		else SetPlayerHealth(playerid, dHP + 10);

		GetPlayerArmour(playerid, dAP);

		if(dAP > 85)
			SetPlayerArmour(playerid, 100);
		else SetPlayerArmour(playerid, dAP + 5);

		SetPlayerDrunkLevel(playerid, 5000);
		SetPlayerTime(playerid, 23, 0);
		SetPlayerWeather(playerid, 493);
	}
	else if(drugid == DRUG_XANAX)
	{
		if(PlayerInfo[playerid][pXanax] == 0)
			return SendClientMessage(playerid, COLOR_LIGHTBLUE, "ERROR: {FFFFFF}You do not have enough xanax!");

		SetPlayerChatBubble(playerid, GetPlayerDrugBubbleString(playerid, DRUG_XANAX), COLOR_PURPLE, 5.0000, 5 * 1000);

		new Float:dHP;
		GetPlayerHealth(playerid, dHP);

		if(dHP > 85)
			SetPlayerHealth(playerid, 100);
		else SetPlayerHealth(playerid, dHP + 23);

		PlayerInfo[playerid][pXanax] -= 1;

		SetPlayerDrunkLevel(playerid, 5000);
		SetPlayerTime(playerid, 22, 0);
		SetPlayerWeather(playerid, 2009);
	}
	else if(drugid == DRUG_PROMETHAZINE)
	{
		if(PlayerInfo[playerid][pPromethazine] == 0)
			return SendClientMessage(playerid, COLOR_LIGHTBLUE, "ERROR: {FFFFFF}You do not have enough promethazine!");

		PlayerInfo[playerid][pPromethazine] -= 1;
		SetPlayerChatBubble(playerid, GetPlayerDrugBubbleString(playerid, DRUG_XANAX), COLOR_PURPLE, 5.0000, 5 * 1000);
	}
	else if(drugid == DRUG_CODEINE)
	{
		if(PlayerInfo[playerid][pCodeine] == 0)
			return SendClientMessage(playerid, COLOR_LIGHTBLUE, "ERROR: {FFFFFF}You do not have enough codeine!");

		PlayerInfo[playerid][pCodeine] -= 1;
		SetPlayerChatBubble(playerid, GetPlayerDrugBubbleString(playerid, DRUG_XANAX), COLOR_PURPLE, 5.0000, 5 * 1000);
	}
	else if(drugid == DRUG_LEAN)
	{
		if(PlayerInfo[playerid][pLean] == 0)
			return SendClientMessage(playerid, COLOR_LIGHTBLUE, "ERROR: {FFFFFF}You do not have enough lean!");

		SetPlayerChatBubble(playerid, GetPlayerDrugBubbleString(playerid, DRUG_XANAX), COLOR_PURPLE, 5.0000, 5 * 1000);

		PlayerInfo[playerid][pLean] -= 1;

		new Float:dAP;

		GetPlayerArmour(playerid, dAP);

		if(dAP > 85)
			SetPlayerArmour(playerid, 100);
		else SetPlayerArmour(playerid, dAP + 19);

		SetPlayerDrunkLevel(playerid, 5000);
		SetPlayerTime(playerid, 23, 0);
		SetPlayerWeather(playerid, 39);
	}

	SetTimerEx("DrugEffects", 30 * 1000, 0, "i", playerid); // 30 seconds

	UseDrugsTimer[playerid] = 1;
	SetTimerEx("UseDrugs", 5 * 1000, 0, "i", playerid); // 5 seconds

	if(!IsPlayerInAnyVehicle(playerid))
		OnePlayAnim(playerid, "SMOKING", "M_smk_in", 4.0, 0, 0, 0, 0, 0);

	SendClientMessage(playerid, COLOR_LIGHTBLUE, GetDrugEffectString(drugid));
	return 1;
}

public DropDrug(playerid, dobject, drugid, amount, Float:x, Float:y, Float:z)
{
	for(new i = 1; i < MAX_DROPPED_DRUGS; i++)
	{
		if(DrugInfo[i][droppedDropped] == false)
		{
			DrugInfo[i][droppedID] = i;

			GetPlayerName(playerid, DrugInfo[i][droppedDropper], MAX_PLAYER_NAME);

			// Setting variables for the object size
			if(dobject == DRUG_SHOE_OBJECT)
			{
				new str[64] = "Bag";
				DrugInfo[i][droppedObjName] = str;
				DrugInfo[i][droppedMaxGrams] = 15; // 15 grams
			}
			else if(dobject == DRUG_BOX_OBJECT)
			{
				new str[64] = "Box";
				DrugInfo[i][droppedObjName] = str;
				DrugInfo[i][droppedMaxGrams] = 50; // 5 grams
			}
			else if(dobject == DRUG_DUFFLE_BAG)
			{
				new str[64] = "Duffle Bag";
				DrugInfo[i][droppedObjName] = str;
				DrugInfo[i][droppedMaxGrams] = 100; // 100 grams
			}
			else {
				new str[64] = "Unspecified Object";
				DrugInfo[i][droppedObjName] = str;
				DrugInfo[i][droppedMaxGrams] = 150; // Admin Object
			}

			if(amount > DrugInfo[i][droppedMaxGrams])
				return SendClientMessage(playerid, COLOR_LIGHTBLUE, "ERROR: {FFFFFF}This item cannot hold that many grams!");

			// Taking drugs from the player
			DrugInfo[i][droppedGrams] = amount;

			if(drugid == DRUG_CANNABIS) // Cannabis
			{
				DrugInfo[i][droppedDrugName] = GetDrugName(DRUG_CANNABIS);
				DrugInfo[i][droppedDrug] = DRUG_CANNABIS;
				PlayerInfo[playerid][pCannabis] -= amount;
			}
			else if(drugid == DRUG_COCAINE) // Cocaine
			{
				DrugInfo[i][droppedDrugName] = GetDrugName(DRUG_COCAINE);
				DrugInfo[i][droppedDrug] = DRUG_COCAINE;
				PlayerInfo[playerid][pCocaine] -= amount;
			}
			else if(drugid == DRUG_METH) // Meth
			{
				DrugInfo[i][droppedDrugName] = GetDrugName(DRUG_METH);
				DrugInfo[i][droppedDrug] = DRUG_METH;
				PlayerInfo[playerid][pMeth] -= amount;
			}
			else if(drugid == DRUG_XANAX) // Xanax
			{
				DrugInfo[i][droppedDrugName] = GetDrugName(DRUG_XANAX);
				DrugInfo[i][droppedDrug] = DRUG_XANAX;
				PlayerInfo[playerid][pXanax] -= amount;
			}
			else if(drugid == DRUG_PROMETHAZINE) // Promethazine
			{
				DrugInfo[i][droppedDrugName] = GetDrugName(DRUG_PROMETHAZINE);
				DrugInfo[i][droppedDrug] = DRUG_PROMETHAZINE;
				PlayerInfo[playerid][pPromethazine] -= amount;
			}
			else if(drugid == DRUG_CODEINE) // Codine
			{
				DrugInfo[i][droppedDrugName] = GetDrugName(DRUG_CODEINE);
				DrugInfo[i][droppedDrug] = DRUG_CODEINE;
				PlayerInfo[playerid][pCodeine] -= amount;
			}
			else if(drugid == DRUG_LEAN) // Lean
			{
				DrugInfo[i][droppedDrugName] = GetDrugName(DRUG_PROMETHAZINE);
				DrugInfo[i][droppedDrug] = DRUG_LEAN;
				PlayerInfo[playerid][pLean] -= amount;
			}
			// Creating the object
			DrugInfo[i][droppedObject] = CreateDynamicObject(dobject, x, y, z -= 0.9, 0, 0, 0);
			Streamer_Update(playerid);

			DrugInfo[i][droppedPosition][0] = x;
			DrugInfo[i][droppedPosition][1] = y;
			DrugInfo[i][droppedPosition][2] = z;

			DrugInfo[i][droppedDropped] = true;
			break;
		}
	}
	return 1;
}

public GetDrugInfo(playerid)
{
	new str[244], Float:Position[3];
	GetPlayerPos(playerid, Position[0], Position[1], Position[2]);

	for(new i = 1; i < MAX_DROPPED_DRUGS; i++)
	{
		if(DrugInfo[i][droppedDropped] == false)
			continue;

		if(IsPlayerInRangeOfPoint(playerid, 3.0, Position[0], Position[1], Position[2]))
		{
			format(str, sizeof str, "Drug ID: {FFFFFF}%i{33ccff} | Drug Type: {FFFFFF}%s {33ccff}| Drug Grams: {FFFFFF}%i{33ccff} | Placer: {FFFFFF}%s", DrugInfo[i][droppedID], DrugInfo[i][droppedDrugName], DrugInfo[i][droppedGrams], DrugInfo[i][droppedDropper]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		}
	}
	return 1;
}

public PickUpDrug(playerid)
{
	new str[128], bool:founddrug = false;
	for(new i = 1; i < MAX_DROPPED_DRUGS; i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 2.0, DrugInfo[i][droppedPosition][0], DrugInfo[i][droppedPosition][1], DrugInfo[i][droppedPosition][2]))
		{
			if(DrugInfo[i][droppedDrug] == DRUG_CANNABIS)
			{
				format(str, sizeof(str), "You've picked up %i.0g of cannabis!", DrugInfo[i][droppedGrams]);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

				PlayerInfo[playerid][pCannabis] += DrugInfo[i][droppedGrams];
			}
			else if(DrugInfo[i][droppedDrug] == DRUG_COCAINE)
			{
				format(str, sizeof(str), "You've picked up %i.0g of cocaine!", DrugInfo[i][droppedGrams]);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

				PlayerInfo[playerid][pCocaine] += DrugInfo[i][droppedGrams];
			}
			else if(DrugInfo[i][droppedDrug] == DRUG_METH)
			{
				format(str, sizeof(str), "You've picked up %i.0g of meth!", DrugInfo[i][droppedGrams]);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

				PlayerInfo[playerid][pMeth] += DrugInfo[i][droppedGrams];
			}
			else if(DrugInfo[i][droppedDrug] == DRUG_XANAX)
			{
				format(str, sizeof(str), "You've picked up %i.0 tabs of xanax!", DrugInfo[i][droppedGrams]);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

				PlayerInfo[playerid][pXanax] += DrugInfo[i][droppedGrams];
			}
			else if(DrugInfo[i][droppedDrug] == DRUG_PROMETHAZINE)
			{
				format(str, sizeof(str), "You've picked up %i.0 tabs of promethazine!", DrugInfo[i][droppedGrams]);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

				PlayerInfo[playerid][pPromethazine] += DrugInfo[i][droppedGrams];
			}
			else if(DrugInfo[i][droppedDrug] == DRUG_CODEINE)
			{
				format(str, sizeof(str), "You've picked up %i.0 tabs of codeine!", DrugInfo[i][droppedGrams]);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

				PlayerInfo[playerid][pCodeine] += DrugInfo[i][droppedGrams];
			}
			else if(DrugInfo[i][droppedDrug] == DRUG_LEAN)
			{
				format(str, sizeof(str), "You've picked up %i.0 tabs of lean!", DrugInfo[i][droppedGrams]);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

				PlayerInfo[playerid][pLean] += DrugInfo[i][droppedGrams];
			}

			DestroyDynamicObjectEx(DrugInfo[i][droppedObject]);
			Streamer_Update(playerid);

			DrugInfo[i][droppedPosition][0] = 0;
			DrugInfo[i][droppedPosition][1] = 0;
			DrugInfo[i][droppedPosition][2] = 0;

			DrugInfo[i][droppedMaxGrams] = 0;
			DrugInfo[i][droppedGrams] = 0;
			DrugInfo[i][droppedDrug] = 0;

			DrugInfo[i][droppedDropped] = false;

			founddrug = true;

			break;
		}
	}
	if(founddrug != true) SendClientMessage(playerid, COLOR_GREY, "You are not by any pickable drugs.");
	return 1;
}

CMD:takedrug(playerid, params[])
{
	PickUpDrug(playerid);
	return 1;
}

public CheckPlayerDrugs(playerid)
{
	new str[128];
	format(str, sizeof(str), "[ {FFFFFF} ( 1. Cannabis (%i.0g) 2. Cocaine (%i.0g) 3. Meth (%i.0g) 4. Xanax (%d.0 tabs) 5. Promethazine (%d.0) ) {33ccff}]",
	 PlayerInfo[playerid][pCannabis], PlayerInfo[playerid][pCocaine], PlayerInfo[playerid][pMeth], PlayerInfo[playerid][pXanax], PlayerInfo[playerid][pPromethazine]);
	SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

	format(str, sizeof(str), "[ {FFFFFF} ( 6. Codeine (%d.0 fl oz) 7. Lean (%d.0 fl oz) ) {33ccff}]",
	 PlayerInfo[playerid][pCodeine], PlayerInfo[playerid][pLean]);
	SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
	return 1;
}

public GiveDrug(playerid, targetid, drugid, amount)
{
	if(targetid == playerid) return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}You can not give drugs to yourself.");
	if(!IsPlayerConnected(targetid) || targetid == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}Invalid player ID.");
	new Float:Position[3];
	GetPlayerPos(targetid, Position[0], Position[1], Position[2]);

	if(!IsPlayerInRangeOfPoint(playerid, 5.0, Position[0], Position[1], Position[2]))
		return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}You're not near the specified person!");

	if(drugid == DRUG_CANNABIS)
	{
		if(amount > PlayerInfo[playerid][pCannabis] || amount < 1)
			return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}You don't have enough cannabis!");
	}
	else if(drugid == DRUG_COCAINE)
	{
		if(amount > PlayerInfo[playerid][pCocaine] || amount < 1)
			return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}You don't have enough cocaine!");
	}
	else if(drugid == DRUG_METH)
	{
		if(amount > PlayerInfo[playerid][pMeth] || amount < 1)
			return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}You don't have enough meth!");
	}
	else if(drugid == DRUG_XANAX)
	{
		if(amount > PlayerInfo[playerid][pXanax] || amount < 1)
			return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}You don't have enough xanax!");
	}
	else if(drugid == DRUG_PROMETHAZINE)
	{
		if(amount > PlayerInfo[playerid][pPromethazine] || amount < 1)
			return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}You don't have enough promethazine!");
	}
	else if(drugid == DRUG_CODEINE)
	{
		if(amount > PlayerInfo[playerid][pCodeine] || amount < 1)
			return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}You don't have enough codeine!");
	}
	else if(drugid == DRUG_LEAN)
	{
		if(amount > PlayerInfo[playerid][pLean] || amount < 1)
			return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}You don't have enough lean!");
	}
	OfferDrug(playerid, targetid, drugid, amount);
	return 1;
}

public OfferDrug(playerid, target, drugid, amount)
{
	new str[128];
	if(drugid == DRUG_CANNABIS)
	{
		format(str, sizeof str, "%s has offered you %i.0 grams of cannabis (/acceptdrug).", PlayerICName(playerid), amount);
		SendClientMessage(target, COLOR_LIGHTBLUE, str);

		format(str, sizeof str, "You have offered %s %i.0 grams of cannabis.", PlayerICName(target), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
	}
	else if(drugid == DRUG_COCAINE)
	{
		format(str, sizeof str, "%s has offered you %i.0 grams of cocaine (/acceptdrug).", PlayerICName(playerid), amount);
		SendClientMessage(target, COLOR_LIGHTBLUE, str);

		format(str, sizeof str, "You have offered %s %i.0 grams of cocaine.", PlayerICName(target), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
	}
	else if(drugid == DRUG_METH)
	{
		format(str, sizeof str, "%s has offered you %i.0 grams of meth (/acceptdrug).", PlayerICName(playerid), amount);
		SendClientMessage(target, COLOR_LIGHTBLUE, str);

		format(str, sizeof str, "You have offered %s %i.0 grams of meth.", PlayerICName(target), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

	}
	else if(drugid == DRUG_XANAX)
	{
		format(str, sizeof str, "%s has offered you %i.0 tabs of xanax (/acceptdrug).", PlayerICName(playerid), amount);
		SendClientMessage(target, COLOR_LIGHTBLUE, str);

		format(str, sizeof str, "You have offered %s %i.0 tabs of xanax.", PlayerICName(target), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
	}
	else if(drugid == DRUG_PROMETHAZINE)
	{
		format(str, sizeof str, "%s has offered you %i.0 tabs of promethazine (/acceptdrug).", PlayerICName(playerid), amount);
		SendClientMessage(target, COLOR_LIGHTBLUE, str);

		format(str, sizeof str, "You have offered %s %i.0 tabs of promethazine.", PlayerICName(target), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
	}
	else if(drugid == DRUG_CODEINE)
	{
		format(str, sizeof str, "%s has offered you %i.0 tabs of codeine (/acceptdrug).", PlayerICName(playerid), amount);
		SendClientMessage(target, COLOR_LIGHTBLUE, str);

		format(str, sizeof str, "You have offered %s %i.0 tabs of codeine.", PlayerICName(target), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
	}
	else if(drugid == DRUG_LEAN)
	{
		format(str, sizeof str, "%s has offered you %i.0 tabs of lean (/acceptdrug).", PlayerICName(playerid), amount);
		SendClientMessage(target, COLOR_LIGHTBLUE, str);

		format(str, sizeof str, "You have offered %s %i.0 tabs of lean.", PlayerICName(target), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
	}


	drugofferplayer[target] = playerid;
	drugofferid[target] = drugid;
	drugofferamount[target] = amount;

	return 1;
}

/*
	Commands

	Notes: dcmd_ = Prefix for drug commands
 */

CMD:acceptdrug(playerid, params[])
{
	new target = drugofferplayer[playerid];
	if(target == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}You were not offered drugs.");
	if(!IsPlayerInRangeOfPlayer(5, playerid, target)) return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}That player is not near you.");

	new drugid = drugofferid[playerid];
	new amount = drugofferamount[playerid];
	if(drugid == DRUG_CANNABIS)
	{
		if(amount > PlayerInfo[target][pCannabis] || amount < 1)
			return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}That player no longer has the specified amount of Cannabis!");
	}
	else if(drugid == DRUG_COCAINE)
	{
		if(amount > PlayerInfo[target][pCocaine] || amount < 1)
			return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}That player no longer has the specified amount of Cocaine!");
	}
	else if(drugid == DRUG_METH)
	{
		if(amount > PlayerInfo[target][pMeth] || amount < 1)
			return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}That player no longer has the specified amount of Meth!");
	}
	else if(drugid == DRUG_XANAX)
	{
		if(amount > PlayerInfo[target][pXanax] || amount < 1)
			return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}That player no longer has the specified amount of Xanax!");
	}
	else if(drugid == DRUG_PROMETHAZINE)
	{
		if(amount > PlayerInfo[target][pPromethazine] || amount < 1)
			return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}That player no longer has the specified amount of promethazine!");
	}
	else if(drugid == DRUG_CODEINE)
	{
		if(amount > PlayerInfo[target][pCodeine] || amount < 1)
			return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}That player no longer has the specified amount of codeine!");
	}
	else if(drugid == DRUG_LEAN)
	{
		if(amount > PlayerInfo[target][pLean] || amount < 1)
			return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}That player no longer has the specified amount of lean!");
	}

	new str[128];
	if(drugid == DRUG_CANNABIS)
	{
		format(str, sizeof str, "%s has given you %i.0 grams of cannabis.", PlayerICName(target), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

		format(str, sizeof str, "You have given %s %i.0 grams of cannabis.", PlayerICName(playerid), amount);
		SendClientMessage(target, COLOR_LIGHTBLUE, str);

		PlayerInfo[playerid][pCannabis] += amount;
		PlayerInfo[target][pCannabis] -= amount;
	}
	else if(drugid == DRUG_COCAINE)
	{
		format(str, sizeof str, "%s has given you %i.0 grams of cocaine.", PlayerICName(target), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

		format(str, sizeof str, "You have given %s %i.0 grams of cocaine.", PlayerICName(playerid), amount);
		SendClientMessage(target, COLOR_LIGHTBLUE, str);

		PlayerInfo[playerid][pCocaine] += amount;
		PlayerInfo[target][pCocaine] -= amount;
	}
	else if(drugid == DRUG_METH)
	{
		format(str, sizeof str, "%s has given you %i.0 grams of meth.", PlayerICName(target), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

		format(str, sizeof str, "You have given %s %i.0 grams of meth.", PlayerICName(playerid), amount);
		SendClientMessage(target, COLOR_LIGHTBLUE, str);
		
		PlayerInfo[playerid][pMeth] += amount;
		PlayerInfo[target][pMeth] -= amount;
	}
	else if(drugid == DRUG_XANAX)
	{
		format(str, sizeof str, "%s has given you %i.0 tabs of xanax.", PlayerICName(target), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

		format(str, sizeof str, "You have given %s %i.0 tabs of xanax.", PlayerICName(playerid), amount);
		SendClientMessage(target, COLOR_LIGHTBLUE, str);
		
		PlayerInfo[playerid][pXanax] += amount;
		PlayerInfo[target][pXanax] -= amount;
	}
	else if(drugid == DRUG_PROMETHAZINE)
	{
		format(str, sizeof str, "%s has given you %i.0 FL OZ of promethazine.", PlayerICName(target), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

		format(str, sizeof str, "You have given %s %i.0 FL OZ of promethazine.", PlayerICName(playerid), amount);
		SendClientMessage(target, COLOR_LIGHTBLUE, str);
		
		PlayerInfo[playerid][pPromethazine] += amount;
		PlayerInfo[target][pPromethazine] -= amount;
	}
	else if(drugid == DRUG_CODEINE)
	{
		format(str, sizeof str, "%s has given you %i.0 FL OZ of codeine.", PlayerICName(target), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

		format(str, sizeof str, "You have given %s %i.0 FL OZ of codeine.", PlayerICName(playerid), amount);
		SendClientMessage(target, COLOR_LIGHTBLUE, str);
		
		PlayerInfo[playerid][pCodeine] += amount;
		PlayerInfo[target][pCodeine] -= amount;
	}
	else if(drugid == DRUG_LEAN)
	{
		format(str, sizeof str, "%s has given you %i.0 FL OZ of lean.", PlayerICName(target), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

		format(str, sizeof str, "You have given %s %i.0 FL OZ of lean.", PlayerICName(playerid), amount);
		SendClientMessage(target, COLOR_LIGHTBLUE, str);
		
		PlayerInfo[playerid][pLean] += amount;
		PlayerInfo[target][pLean] -= amount;
	}

	drugofferplayer[playerid] = INVALID_PLAYER_ID;
	drugofferamount[playerid] = 0;
	drugofferid[playerid] = 0;	
	return 1;
}

CMD:usedrug(playerid, params[])
{
	if(isnull(params))
		return SendClientMessage(playerid, COLOR_LIGHTBLUE, "USAGE: {FFFFFF}/usedrug [drug] (/drugs)");

	if(strcmp(params, "Cannabis", true) == 0)
	{
		UseDrug(playerid, DRUG_CANNABIS);
	}
	else if(strcmp(params, "Cocaine", true) == 0)
	{
		UseDrug(playerid, DRUG_COCAINE);
	}
	else if(strcmp(params, "Meth", true) == 0)
	{
		UseDrug(playerid, DRUG_METH);
	}
	else if(strcmp(params, "Xanax", true) == 0)
	{
		UseDrug(playerid, DRUG_XANAX);
	}
	else if(strcmp(params, "Promethazine", true) == 0)
	{
		UseDrug(playerid, DRUG_PROMETHAZINE);
	}
	else if(strcmp(params, "Codeine", true) == 0)
	{
		UseDrug(playerid, DRUG_CODEINE);
	}
	else if(strcmp(params, "Lean", true) == 0)
	{
		UseDrug(playerid, DRUG_LEAN);
	}
	return 1;
}

CMD:givedrug(playerid, params[])
{
	new targetid, drug[24], amount;
	if(sscanf(params, "us[24]i", targetid, drug, amount))
	{
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "USAGE: {FFFFFF}/givedrug [ID/PartOfName] [Drug] [Amount]");
		SendClientMessage(playerid, COLOR_GREY, "Avaliable drugs: cannabis, cocaine, meth, xanax, promethazine, codeine, lean");
	}
	if(strcmp(drug, "cannabis", true) == 0)
	{
		GiveDrug(playerid, targetid, DRUG_CANNABIS, amount);
	}
	else if(strcmp(drug, "cocaine", true) == 0)
	{
		GiveDrug(playerid, targetid, DRUG_COCAINE, amount);
	}
	else if(strcmp(drug, "meth", true) == 0)
	{
		GiveDrug(playerid, targetid, DRUG_METH, amount);
	}
	else if(strcmp(drug, "Xanax", true) == 0)
	{
		GiveDrug(playerid, targetid, DRUG_XANAX, amount);
	}
	else if(strcmp(drug, "promethazine", true) == 0)
	{
		GiveDrug(playerid, targetid, DRUG_PROMETHAZINE, amount);
	}
	else if(strcmp(drug, "codeine", true) == 0)
	{
		GiveDrug(playerid, targetid, DRUG_CODEINE, amount);
	}
	else if(strcmp(drug, "lean", true) == 0)
	{
		GiveDrug(playerid, targetid, DRUG_LEAN, amount);
	}
	return 1;
}

CMD:agivedrugs(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 99998)
		return SendClientMessage(playerid, COLOR_RED, "Error:{FFFFFF} You're not authorized to use this command.");

	new TargetPlayer, DrugName[24], amount;
	if(sscanf(params, "us[24]i", TargetPlayer, DrugName, amount))
		return SendClientMessage(playerid, COLOR_LIGHTBLUE, "USAGE: {FFFFFF}/agivedrugs [PlayerID/Part of Name] [Drug Name] [Amount]");

	if(strcmp(DrugName, "cannabis", true) == 0)
	{
		new GiveDrugsStr[64];
		format(GiveDrugsStr, sizeof GiveDrugsStr, "Admin %s has set your cannabis to %i.", PlayerOOCName(playerid), amount);
		SendClientMessage(TargetPlayer, COLOR_LIGHTBLUE, GiveDrugsStr);

		format(GiveDrugsStr, sizeof GiveDrugsStr, "You've set %s's cannabis to %i.", PlayerOOCName(TargetPlayer), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, GiveDrugsStr);

		PlayerInfo[TargetPlayer][pCannabis] = amount;
	}
	else if(strcmp(DrugName, "cocaine", true) == 0)
	{
		new GiveDrugsStr[64];
		format(GiveDrugsStr, sizeof GiveDrugsStr, "Admin %s has set your cocaine to %i.", PlayerOOCName(playerid), amount);
		SendClientMessage(TargetPlayer, COLOR_LIGHTBLUE, GiveDrugsStr);

		format(GiveDrugsStr, sizeof GiveDrugsStr, "You've set %s's cocaine to %i.", PlayerOOCName(TargetPlayer), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, GiveDrugsStr);
		PlayerInfo[TargetPlayer][pCocaine] = amount;
	}
	else if(strcmp(DrugName, "meth", true) == 0)
	{
		new GiveDrugsStr[64];
		format(GiveDrugsStr, sizeof GiveDrugsStr, "Admin %s has set your meth to %i.", PlayerOOCName(playerid), amount);
		SendClientMessage(TargetPlayer, COLOR_LIGHTBLUE, GiveDrugsStr);

		format(GiveDrugsStr, sizeof GiveDrugsStr, "You've set %s's meth to %i.", PlayerOOCName(TargetPlayer), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, GiveDrugsStr);

		PlayerInfo[TargetPlayer][pMeth] = amount;
	}
	else if(strcmp(DrugName, "xanax", true) == 0)
	{
		new GiveDrugsStr[64];
		format(GiveDrugsStr, sizeof GiveDrugsStr, "Admin %s has set your xanax to %i.", PlayerOOCName(playerid), amount);
		SendClientMessage(TargetPlayer, COLOR_LIGHTBLUE, GiveDrugsStr);

		format(GiveDrugsStr, sizeof GiveDrugsStr, "You've set %s's xanax to %i.", PlayerOOCName(TargetPlayer), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, GiveDrugsStr);

		PlayerInfo[TargetPlayer][pXanax] = amount;
	}
	else if(strcmp(DrugName, "promethazine", true) == 0)
	{
		new GiveDrugsStr[64];
		format(GiveDrugsStr, sizeof GiveDrugsStr, "Admin %s has set your promethazine to %i.", PlayerOOCName(playerid), amount);
		SendClientMessage(TargetPlayer, COLOR_LIGHTBLUE, GiveDrugsStr);

		format(GiveDrugsStr, sizeof GiveDrugsStr, "You've set %s's promethazine to %i.", PlayerOOCName(TargetPlayer), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, GiveDrugsStr);

		PlayerInfo[TargetPlayer][pPromethazine] = amount;
	}
	else if(strcmp(DrugName, "codeine", true) == 0)
	{
		new GiveDrugsStr[64];
		format(GiveDrugsStr, sizeof GiveDrugsStr, "Admin %s has set your codeine to %i.", PlayerOOCName(playerid), amount);
		SendClientMessage(TargetPlayer, COLOR_LIGHTBLUE, GiveDrugsStr);

		format(GiveDrugsStr, sizeof GiveDrugsStr, "You've set %s's codeine to %i.", PlayerOOCName(TargetPlayer), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, GiveDrugsStr);

		PlayerInfo[TargetPlayer][pCodeine] = amount;
	}
	else if(strcmp(DrugName, "lean", true) == 0)
	{
		new GiveDrugsStr[64];
		format(GiveDrugsStr, sizeof GiveDrugsStr, "Admin %s has set your lean to %i.", PlayerOOCName(playerid), amount);
		SendClientMessage(TargetPlayer, COLOR_LIGHTBLUE, GiveDrugsStr);

		format(GiveDrugsStr, sizeof GiveDrugsStr, "You've set %s's lean to %i.", PlayerOOCName(TargetPlayer), amount);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, GiveDrugsStr);

		PlayerInfo[TargetPlayer][pLean] = amount;
	}
	return 1;
}
CMD:dropdrug(playerid, params[])
{
	new drug[14], amount, dobject[50];
	if(sscanf(params, "s[24]is[24]", drug, amount, dobject))
	{
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "USAGE: {FFFFFF}/dropdrug [drug] [amount (grams)] [Shoe Box / Box / Duffle Bag]");
		SendClientMessage(playerid, COLOR_GREY, "Avaliable Choices: cannabis, cocaine, meth, xanax, promethazine, codeine, lean");
	}
	new Float:DropDrugPos[3];
	GetPlayerPos(playerid, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);

	if(strcmp(drug, "cannabis", true) == 0)
	{
		if(amount > PlayerInfo[playerid][pCannabis] || amount < 1)
			return SendClientMessage(playerid, COLOR_LIGHTBLUE, "ERROR: {FFFFFF}You don't enough cannabis!");

		if(strcmp(dobject, "Shoe Box", true) == 0)
		{
			DropDrug(playerid, DRUG_SHOE_OBJECT, DRUG_CANNABIS, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
		else if(strcmp(dobject, "Box", true) == 0)
		{
			DropDrug(playerid, DRUG_BOX_OBJECT, DRUG_CANNABIS, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
		else if(strcmp(dobject, "Duffle Bag", true) == 0)
		{
			DropDrug(playerid, DRUG_DUFFLE_BAG, DRUG_CANNABIS, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
	}
	else if(strcmp(drug, "cocaine", true) == 0)
	{		
		if(amount > PlayerInfo[playerid][pCocaine] || amount < 1)
			return SendClientMessage(playerid, COLOR_LIGHTBLUE, "ERROR: {FFFFFF}You don't enough cocaine!");

		if(strcmp(dobject, "Shoe Box", true) == 0)
		{
			DropDrug(playerid, DRUG_SHOE_OBJECT, DRUG_COCAINE, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
		else if(strcmp(dobject, "Box", true) == 0)
		{
			DropDrug(playerid, DRUG_BOX_OBJECT, DRUG_COCAINE, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
		else if(strcmp(dobject, "Duffle Bag", true) == 0)
		{
			DropDrug(playerid, DRUG_DUFFLE_BAG, DRUG_COCAINE, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
	}
	else if(strcmp(drug, "meth", true) == 0)
	{
		if(amount > PlayerInfo[playerid][pMeth] || amount < 1)
			return SendClientMessage(playerid, COLOR_LIGHTBLUE, "ERROR: {FFFFFF}You don't enough meth!");	
	
		if(strcmp(dobject, "Shoe Box", true) == 0)
		{
			DropDrug(playerid, DRUG_SHOE_OBJECT, DRUG_METH, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
		else if(strcmp(dobject, "Box", true) == 0)
		{
			DropDrug(playerid, DRUG_BOX_OBJECT, DRUG_METH, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
		else if(strcmp(dobject, "Duffle Bag", true) == 0)
		{
			DropDrug(playerid, DRUG_DUFFLE_BAG, DRUG_METH, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
	}
	else if(strcmp(drug, "xanax", true) == 0)
	{
		if(amount > PlayerInfo[playerid][pXanax] || amount < 1)
			return SendClientMessage(playerid, COLOR_LIGHTBLUE, "ERROR: {FFFFFF}You don't enough xanax!");	
	
		if(strcmp(dobject, "Shoe Box", true) == 0)
		{
			DropDrug(playerid, DRUG_SHOE_OBJECT, DRUG_XANAX, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
		else if(strcmp(dobject, "Box", true) == 0)
		{
			DropDrug(playerid, DRUG_BOX_OBJECT, DRUG_XANAX, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
		else if(strcmp(dobject, "Duffle Bag", true) == 0)
		{
			DropDrug(playerid, DRUG_DUFFLE_BAG, DRUG_XANAX, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
	}
	else if(strcmp(drug, "promethazine", true) == 0)
	{
		if(amount > PlayerInfo[playerid][pPromethazine] || amount < 1)
			return SendClientMessage(playerid, COLOR_LIGHTBLUE, "ERROR: {FFFFFF}You don't enough promethazine!");	
	
		if(strcmp(dobject, "Shoe Box", true) == 0)
		{
			DropDrug(playerid, DRUG_SHOE_OBJECT, DRUG_PROMETHAZINE, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
		else if(strcmp(dobject, "Box", true) == 0)
		{
			DropDrug(playerid, DRUG_BOX_OBJECT, DRUG_PROMETHAZINE, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
		else if(strcmp(dobject, "Duffle Bag", true) == 0)
		{
			DropDrug(playerid, DRUG_DUFFLE_BAG, DRUG_PROMETHAZINE, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
	}
	else if(strcmp(drug, "codeine", true) == 0)
	{
		if(amount > PlayerInfo[playerid][pCodeine] || amount < 1)
			return SendClientMessage(playerid, COLOR_LIGHTBLUE, "ERROR: {FFFFFF}You don't enough codeine!");	
	
		if(strcmp(dobject, "Shoe Box", true) == 0)
		{
			DropDrug(playerid, DRUG_SHOE_OBJECT, DRUG_CODEINE, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
		else if(strcmp(dobject, "Box", true) == 0)
		{
			DropDrug(playerid, DRUG_BOX_OBJECT, DRUG_CODEINE, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
		else if(strcmp(dobject, "Duffle Bag", true) == 0)
		{
			DropDrug(playerid, DRUG_DUFFLE_BAG, DRUG_CODEINE, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
	}
	else if(strcmp(drug, "lean", true) == 0)
	{
		if(amount > PlayerInfo[playerid][pLean] || amount < 1)
			return SendClientMessage(playerid, COLOR_LIGHTBLUE, "ERROR: {FFFFFF}You don't enough lean!");	
	
		if(strcmp(dobject, "Shoe Box", true) == 0)
		{
			DropDrug(playerid, DRUG_SHOE_OBJECT, DRUG_LEAN, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
		else if(strcmp(dobject, "Box", true) == 0)
		{
			DropDrug(playerid, DRUG_BOX_OBJECT, DRUG_LEAN, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
		else if(strcmp(dobject, "Duffle Bag", true) == 0)
		{
			DropDrug(playerid, DRUG_DUFFLE_BAG, DRUG_LEAN, amount, DropDrugPos[0], DropDrugPos[1], DropDrugPos[2]);
		}
	}
	return 1;
}

CMD:checkdrugs(playerid, params[])
{
	new str[128];
	for(new i = 1; i < MAX_DROPPED_DRUGS; i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 2.0, DrugInfo[i][droppedPosition][0], DrugInfo[i][droppedPosition][1], DrugInfo[i][droppedPosition][2]))
		{
			format(str, sizeof(str), "[ {FFFFFF}%s: %s ( %i.0g / %i.0g ) {33ccff}]", DrugInfo[i][droppedObjName], DrugInfo[i][droppedDrugName], DrugInfo[i][droppedGrams], DrugInfo[i][droppedMaxGrams]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			break;
		}

		if(i == MAX_DROPPED_DRUGS)
		{
			SendClientMessage(playerid, playerid, "ERROR: {FFFFFF}You're not near any dropped drugs!");
		}
	}
	return 1;
}

CMD:listdrugs(playerid, params[])
{
	GetDrugInfo(playerid);
	return 1;
}

CMD:createdrugs(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] != 1999999999)
		return SendClientMessage(playerid, COLOR_LIGHTBLUE, "ERROR: {FFFFFF}You're not a high enough level admin");

	new drugtype[64], amount, dobject;
	if(sscanf(params, "s[24]ii", drugtype, amount, dobject))
		return SendClientMessage(playerid, COLOR_LIGHTBLUE, "USAGE: {FFFFFF} /createdrugs [Drug] [Amount] [Object]");

	if(amount < 1)
		return SendClientMessage(playerid, COLOR_LIGHTBLUE, "ERROR: {FFFFFF}You can't go below 0.");

	new Float:Position[3];
	GetPlayerPos(playerid, Position[0], Position[1], Position[2]);

	if(strcmp(drugtype, "cannabis", true) == 0)
	{
		DropDrug(playerid, dobject, DRUG_CANNABIS, amount, Position[0], Position[1], Position[2]);
	}
	else if(strcmp(drugtype, "cocaine", true) == 0)
	{
		DropDrug(playerid, dobject, DRUG_COCAINE, amount, Position[0], Position[1], Position[2]);
	}
	else if(strcmp(drugtype, "meth", true) == 0)
	{
		DropDrug(playerid, dobject, DRUG_METH, amount, Position[0], Position[1], Position[2]);
	}
	else if(strcmp(drugtype, "xanax", true) == 0)
	{
		DropDrug(playerid, dobject, DRUG_XANAX, amount, Position[0], Position[1], Position[2]);
	}
	else if(strcmp(drugtype, "lean", true) == 0)
	{
		DropDrug(playerid, dobject, DRUG_LEAN, amount, Position[0], Position[1], Position[2]);
	}
	else if(strcmp(drugtype, "promethazine", true) == 0)
	{
		DropDrug(playerid, dobject, DRUG_PROMETHAZINE, amount, Position[0], Position[1], Position[2]);
	}
	else if(strcmp(drugtype, "codeine", true) == 0)
	{
		DropDrug(playerid, dobject, DRUG_CODEINE, amount, Position[0], Position[1], Position[2]);
	}
	return 1;
}

CMD:destroydrugs(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 99999) return SendClientMessage(playerid, COLOR_GRAD2, "{FF0000}You're not authorized to use that command.");

	new i;
	if(sscanf(params, "i", i))
		return SendClientMessage(playerid, COLOR_RED, "USAGE: {FFFFFF}/destroydrugs [ID] (/alistdrugs)");

	if(DrugInfo[i][droppedDropped] == false)
		return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}This ID isn't dropped.");

	DrugInfo[i][droppedPosition][0] = 0;
	DrugInfo[i][droppedPosition][1] = 0;
	DrugInfo[i][droppedPosition][2] = 0;

	DrugInfo[i][droppedMaxGrams] = 0;
	DrugInfo[i][droppedGrams] = 0;
	DrugInfo[i][droppedDrug] = 0;

	DrugInfo[i][droppedDropped] = false;

	DestroyDynamicObjectEx(DrugInfo[i][droppedObject]);

	new str[128];
	format(str, sizeof str, "AdmWarning{FFFFFF}: %s has deleted drug package ID %i.", PlayerOOCName(playerid), i);
	SendAdminMessage(COLOR_RED, str);
	return 1;
}

CMD:drughelp(playerid, params[])
{
	new hstr[564];
	format(hstr, sizeof(hstr), "{007BD0}Drug Help{B4B5B7}\n", hstr);
	format(hstr, sizeof(hstr), "%s\n{007BD0}/inv{B4B5B7} - List all the drugs in your inventory.", hstr);
	format(hstr, sizeof(hstr), "%s\n{007BD0}/dropdrug{B4B5B7} - Allows you to drop drugs on the floor.", hstr);
	format(hstr, sizeof(hstr), "%s\n{007BD0}/takedrug{B4B5B7} - Picks up nearby drugs off of the floor.", hstr);
	format(hstr, sizeof(hstr), "%s\n{007BD0}/givedrug{B4B5B7} - Gives a drug to a specified player.", hstr);
	format(hstr, sizeof(hstr), "%s\n{007BD0}/usedrug{B4B5B7} - Uses a specific drug.", hstr);
	format(hstr, sizeof(hstr), "%s\n{007BD0}/mixdrug{B4B5B7} - Mixes two drugs together.", hstr);
	ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Drugs", hstr, "Ok", "Close");
	return 1;
}

CMD:drugs(playerid, params[])
{
	CheckPlayerDrugs(playerid);
	return 1;
}

CMD:mixdrug(playerid, params[])
{
	new drug1[24], drug2[24];
	if(sscanf(params, "s[24]s[24]ii", drug1, drug2))
		return SendClientMessage(playerid, COLOR_GREY, "USAGE: /mixdrug [drug 1] [drug 2]");

	if(strcmp(drug1, "promethazine", true) == 0 && strcmp(drug2, "codeine", true) == 0)
	{
		MixDrugs(playerid, DRUG_PROMETHAZINE, DRUG_CODEINE, 5, 5);
	} else{
		SendClientMessage(playerid, COLOR_GREY, "Invalid drug mix.");
	}
	return 1;
}

Hook:drugs_OnPlayerConnect(playerid)
{
	drugofferplayer[playerid] = INVALID_PLAYER_ID;
	drugofferamount[playerid] = 0;
	drugofferid[playerid] = 0;
	return 1;
}

#include "../gamemodes/inc/drugs/growing.inc"