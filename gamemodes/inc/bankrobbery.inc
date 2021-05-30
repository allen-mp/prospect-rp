/*
  ____              _      _____       _     _                        _____           _                   ____          _____                                      _ 
 |  _ \            | |    |  __ \     | |   | |                      / ____|         | |                 |  _ \        |  __ \                                    | |
 | |_) | __ _ _ __ | | __ | |__) |___ | |__ | |__   ___ _ __ _   _  | (___  _   _ ___| |_ ___ _ __ ___   | |_) |_   _  | |  | | ___  ___ _ __ ___   ___  _ __   __| |
 |  _ < / _` | '_ \| |/ / |  _  // _ \| '_ \| '_ \ / _ \ '__| | | |  \___ \| | | / __| __/ _ \ '_ ` _ \  |  _ <| | | | | |  | |/ _ \/ __| '_ ` _ \ / _ \| '_ \ / _` |
 | |_) | (_| | | | |   <  | | \ \ (_) | |_) | |_) |  __/ |  | |_| |  ____) | |_| \__ \ ||  __/ | | | | | | |_) | |_| | | |__| |  __/\__ \ | | | | | (_) | | | | (_| |
 |____/ \__,_|_| |_|_|\_\ |_|  \_\___/|_.__/|_.__/ \___|_|   \__, | |_____/ \__, |___/\__\___|_| |_| |_| |____/ \__, | |_____/ \___||___/_| |_| |_|\___/|_| |_|\__,_|
                                                              __/ |          __/ |                               __/ |                                               
                                                             |___/          |___/                               |___/                                                

Author: Desmond
Refer to this thread:
http://www.gta-sarp.com/forums/showthread.php?295417-Bank-Robbery-System

Big vault - 10 minutes to break into, need drill, level 4+, at least 7 LEOs on (count PD and DEA)
Once the big vault opens, it says "Head to the safes to start gathering money.\nNOTE: you may use /stopbankrob to finish the robbery however this will end the robbery and you may not gather more money."

Inside the vault - 6 safes, 3Ds that say "/usedrill\nX seconds left" in green. When the safe is opened, change to "$250,000, /loadsafemoney to load the money."

Each safe takes 90 seconds to break into and give $250,000 each.
While breaking into the safe, the player will get 2 seconds skill checks, they will be required to press "SPACE" before the check ends or else they fail to drill into the safe.
Script counts 5 seconds and randomizes a number - either 0 or 1. If 1, show the skill check, if 0, don't show.

Once all safes are cleared or the robber did /stopbankrob, the script sets "bankmoneygathered" to 250,000 multiplied by how many safes the robbers broke into.
They will then have to load the money in a random location around SF.

Additionals:
Interactable stuff - blocking doors, PD have to use C2s to get through.
*/

#define MAX_VAULTS	6
#define BANK_VAULT_COORDINATES	1640.82, 2907.77, 988.54
#define BANK_VAULT_OUTSIDE	1633.3864, 2907.5081, 988.5403
#define BANK_VAULT_INSIDE	1648.8212, 2907.6978, 988.4282
#define CHECKPOINT_BANKROB 65

/*
	Bank variables
*/

new bankmoneygathered, 		// total money collected by robbers
	bool:bankactive, 		// is a bank robbery active?
	bool:breakingvault,		// used to see if the robbers are currently breaking the big vualt
	bigvault,				// big vault's door object
	bool:goinglaundering,	// used to see if the robbers finished robbing the  vaults (so if they left early no1 can continue breaking into vaults)
	bankrobber;				// the main robber - the person that can collect the money, determined by who did /usedrill on the vault's door


enum vaultinfo
{
	vMoney, 			// how much money left in the safe?
	bool:vBreaking, 	// is someone currently breaking into the safe?
	bool:vOpened,		// is the safe opened?
	Text3D:v3DText, 	// the 3D text above the safe
	Float:vPosX,		// coordinates X of the safe
	Float:vPosY,		// coordinates Y of the safe
	Float:vPosZ, 		// coordinates Z of the safe
	Float:vPosRX,		// coordinates X rotation of the safe
	Float:vPosRY,		// coordinates Y rotation of the safe
	Float:vPosRZ,		// coordinates Z rotation of the safe
	vObject 			// object id of the safe
};

new Vaults[MAX_VAULTS][vaultinfo];

new Float:VaultsPositions[][6] =
{
	{1645.54370, 2910.59302, 987.88550, 0.00000, 0.00000, 0.00000},
	{1649.48596, 2910.59302, 987.88550, 0.00000, 0.00000, 0.00000},
	{1653.44202, 2910.59302, 987.88550, 0.00000, 0.00000, 0.00000},
	{1653.40417, 2904.88403, 987.88550, 0.00000, 0.00000, -178.07994},
	{1649.48755, 2904.88916, 987.88550, 0.00000, 0.00000, -178.07990},
	{1645.62744, 2904.88916, 987.88550, 0.00000, 0.00000, -178.07990}
};

new Float:LaunderingPositions[][3] =
{
	{-2150.8677,-103.1125,35.3203},
	{-1815.8945,-73.5078,15.1094},
	{-1741.9487,1424.6788,7.1875}
};

/*
	Player variables
*/

new bankrobtimer[MAX_PLAYERS], skillchecktimer[MAX_PLAYERS], safebreaktime[MAX_PLAYERS], skillcheckrespond[MAX_PLAYERS];
new bool:playerperformskillcheck[MAX_PLAYERS];
new breakingintovault[MAX_PLAYERS];

GetLEOCount()
{
	new counter;
	foreach(new i: Player)
	{
		if(PlayerInfo[i][pAdmin] < 2 && (PlayerInfo[i][pMember] == FACTION_SAPD || PlayerInfo[i][pMember] == FACTION_FBI || PlayerInfo[i][pMember] == FACTION_SANG)) counter++;
	}
	return counter;
}

IsNearBank(playerid) // used to determine if the player is in the no healing zone
{
	// this if checks both inside and outside
	if(IsPlayerInRangeOfPoint(playerid, 200, 1633.3864, 2907.5081, 988.5403) || IsPlayerInRangeOfPoint(playerid, 200, 1450.8630, -1012.5521, 26.8438)) return 1;
	return 0;
}

StartBankRobbery(playerid)
{
	SendClientMessageToAll(COLOR_LIGHTBLUE, "City Hall: The LS Bank is being robbed - 10 minutes left to break into the vault.");
	SendClientMessage(playerid, COLOR_LIGHTBLUE, "Note: If you move from the vault door the robbery will fail but you may look around and shoot.");
	bankactive = true;
	breakingvault = true;

	bankrobtimer[playerid] = SetTimerEx("BankRobberyTimer", 1000*600, false, "d", playerid); // 10 minutes
	bankrobber = playerid;
	return 1;
}

SetupMoneyLaundering(playerid)
{
	foreach(new i: Player)
	{
		safebreaktime[i] = 0;
		KillTimer(bankrobtimer[i]);
		KillTimer(skillchecktimer[i]);
		KillTimer(safebreaktime[i]);
		KillTimer(skillcheckrespond[i]);
	}
	for(new i = 0; i < MAX_VAULTS; i++) CancelVaultBreak(i);
	goinglaundering = true;
	new rand = random(3);
	SetPlayerCheckpointEx(playerid, LaunderingPositions[rand][0], LaunderingPositions[rand][1], LaunderingPositions[rand][2], 3.0);
	CP[playerid] = CHECKPOINT_BANKROB;
	SendClientMessage(playerid, COLOR_LIGHTBLUE, "Head to the checkpoint marked on your map to receive the laundered money.");
	return 1;
}

forward BankRobberyTimer(playerid);
public BankRobberyTimer(playerid)
{
	breakingvault = false;
	MoveDynamicObject(bigvault, 1641.00696, 2905.82178, 989.11761, 3, 0.00000, 0.00000, 91.36140);
	TimeAfterBankJob = gettime()+172800; // 48 hours
	SendClientMessageToAll(COLOR_LIGHTBLUE, "City Hall: The LS Bank's vault has been opened.");
	SetupVaults();
	SendClientMessage(playerid, COLOR_LIGHTBLUE, "Tip: Head to the safes to start gathering money.");
	SendClientMessage(playerid, -1, "{FF0000}NOTE: {FFFFFF}You may use /stopbankrob to finish the robbery however this will end the robbery and you may not gather more money.");
	return 1;
}

BreakVault(playerid, safeid)
{
	Vaults[safeid][vBreaking] = true;
	breakingintovault[playerid] = safeid;
	safebreaktime[playerid] = 90;	

	skillchecktimer[playerid] = SetTimerEx("SkillCheckTimer", 5000, true, "dd", playerid, safeid); // 5 seconds, playerid and the safeid
	UpdateDynamic3DTextLabelText(Vaults[safeid][v3DText], -1, "{FF0000}Player Drilling\n{FFFFFF}90 seconds left");
	SendClientMessage(playerid, COLOR_LIGHTBLUE, "Note: If you move from the safe the breakin will fail but you may look around and shoot.");
	return 1;
}

forward SkillCheckTimer(playerid, safeid);
public SkillCheckTimer(playerid, safeid)
{
	new msg[60];
	safebreaktime[playerid] -= 5; // no skill check
	format(msg, sizeof(msg), "{FF0000}Player Drilling\n{FFFFFF}%d seconds left", safebreaktime[playerid]);
	UpdateDynamic3DTextLabelText(Vaults[safeid][v3DText], -1, msg);

	if(safebreaktime[playerid] == 0) // finished breaking
	{
		Vaults[safeid][vOpened] = true;
		UpdateDynamic3DTextLabelText(Vaults[safeid][v3DText], -1, "{00FF00}Opened\n{FFFFFF}/loadsafemoney ({00FF00}$250,000 {FFFFFF}inside)");		
		DestroyDynamicObjectEx(Vaults[safeid][vObject]); // destroy safe object
		Vaults[safeid][vObject] = CreateDynamicObject(1829, Vaults[safeid][vPosX], Vaults[safeid][vPosY], Vaults[safeid][vPosZ], Vaults[safeid][vPosRX], Vaults[safeid][vPosRY], Vaults[safeid][vPosRZ]); // place opened safe object
		KillTimer(skillchecktimer[playerid]);
		KillTimer(skillcheckrespond[playerid]);
		return 1;
	}

	new rand = random(2);
	if(rand) // skill check
	{
		displayCenterHUDInfo(playerid,  "~w~SKILL CHECK~n~~r~PRESS SPACE", 2); //display for 2 seconds
		skillcheckrespond[playerid] = SetTimerEx("SkillCheckRespond", 3000, false, "dd", playerid, safeid);
		playerperformskillcheck[playerid] = false;
	}
	return 1;
}

forward SkillCheckRespond(playerid, safeid);
public SkillCheckRespond(playerid, safeid)
{
	if(!playerperformskillcheck[playerid]) // failed to do skill check
	{
		CancelVaultBreak(safeid);
		SendClientMessage(playerid, COLOR_GREY, "You have failed to perform your skill check - safe breakin failed.");
		safebreaktime[playerid] = 0;
		KillTimer(skillchecktimer[playerid]);
		KillTimer(skillcheckrespond[playerid]);
	}
	else KillTimer(skillcheckrespond[playerid]);
	return 1;
}

Hook:bank_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(PRESSED(KEY_SPRINT))
	{
		if(safebreaktime[playerid] != 0 && bankactive) playerperformskillcheck[playerid] = true; // if is breaking into a vault, did the skill check!
	}
	return 1;
}

Hook:br_OnPlayerEnterCheckpoint(playerid)
{
	if(CP[playerid] == CHECKPOINT_BANKROB)
	{
		new msg[128];
		CP[playerid] = CHECKPOINT_NONE;
		DisablePlayerCheckpointEx(playerid);

		format(msg, sizeof(msg), "You have received $%d money for robbing the bank.", bankmoneygathered);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, msg);
		GameTextForPlayer(playerid, "~w~Good job!", 4000, 1);

		format(msg, sizeof(msg), "City Hall: The laundered money from the bank has been successfully robbed and gathered - $%d.", bankmoneygathered);
		SendClientMessageToAll(COLOR_LIGHTBLUE, msg);

		GiveMoney(playerid, bankmoneygathered);
		GOV -= bankmoneygathered;

		CancelBankRobbery();

		new family = PlayerInfo[playerid][pFMember];
		if(family != 255)
		{
			FamilyInfo[family][fXP] += 15;
			foreach(new i: Player)
			{
				if(PlayerInfo[i][pFMember] == family) 
				{
					SendClientMessage(i, COLOR_LIGHTBLUE, "Your family has been given 15 XP for successfully robbing the bank!");
					GrantFamXP(family, 15);
				}	
			}
		}
	}
	return 1;
}

Hook:bank_OnPlayerDisconnect(playerid, reason)
{
	if(bankrobber == playerid && bankactive)
	{
		SendClientMessageToAll(COLOR_LIGHTBLUE, "City Hall: Robber disconnected, bank robbery failed.");
		CancelBankRobbery();
	}
	KillTimer(bankrobtimer[playerid]);
	KillTimer(skillchecktimer[playerid]);
	KillTimer(safebreaktime[playerid]);
	KillTimer(skillcheckrespond[playerid]);
	return 1;
}

Hook:bank_OnPlayerUpdate(playerid)
{
	if(safebreaktime[playerid] != 0)
	{
		new safeid = breakingintovault[playerid];
		if(!IsPlayerInRangeOfPoint(playerid, 1.5, Vaults[safeid][vPosX], Vaults[safeid][vPosY], Vaults[safeid][vPosZ]))
		{
			SendClientMessage(playerid, COLOR_GREY, "You have moved too much from the safe, you have failed the breakin.");
			KillTimer(skillchecktimer[playerid]);
			KillTimer(safebreaktime[playerid]);
			KillTimer(skillcheckrespond[playerid]);
			CancelVaultBreak(safeid);
			safebreaktime[playerid] = 0;
			return 1;
		}
	}
	if(breakingvault && bankactive)
	{
		if(bankrobber == playerid)
		{
			if(!IsPlayerInRangeOfPoint(playerid, 1.5, BANK_VAULT_COORDINATES))
			{
				SendClientMessage(playerid, COLOR_GREY, "You have moved too much from the vault, bank robbery failed.");
				CancelBankRobbery();
			}
		}
	}
	return 1;
}

Hook:bankrobbery_OnPlayerDeath(playerid, killerid, reason)
{
	if(bankrobber == playerid && bankactive)
	{
		PlayerInfo[playerid][pDrill] = 0;
		CP[playerid] = CHECKPOINT_NONE;
		DisablePlayerCheckpointEx(playerid);
		SendClientMessageToAll(COLOR_LIGHTBLUE, "City Hall: Robber eliminated, bank robbery failed.");
		CancelBankRobbery();

		foreach(new i: Player)
		{
			if(safebreaktime[i] != 0)
			{
				CancelVaultBreak(i);
				KillTimer(skillchecktimer[i]);
				KillTimer(safebreaktime[i]);
				KillTimer(skillcheckrespond[i]);
			}
		}
	}
	KillTimer(bankrobtimer[playerid]);
	KillTimer(skillchecktimer[playerid]);
	KillTimer(safebreaktime[playerid]);
	KillTimer(skillcheckrespond[playerid]);
	return 1;
}

Hook:br_OnGameModeInit()
{
	bigvault = CreateDynamicObject(2634, 1641.35510, 2907.80640, 989.11761,	0.00000, 0.00000, 91.36140);
	return 1;
}

CancelBankRobbery()
{
	for(new i = 0; i < MAX_VAULTS; i++)
	{
		DestroyDynamicObjectEx(Vaults[i][vObject]);
		DestroyDynamic3DTextLabel(Vaults[i][v3DText]);
		Vaults[i][vMoney] = 0;
	}

	bankrobber = -1;
	bankactive = false;
	breakingvault = false;
	bankmoneygathered = 0;
	goinglaundering = false;

	foreach(new i: Player)
	{
		if(IsPlayerInRangeOfPoint(i, 10, BANK_VAULT_INSIDE)) SetPlayerPos(i, BANK_VAULT_OUTSIDE); // tp players that are inside the vault to avoid them getting stuck
		KillTimer(bankrobtimer[i]);
		KillTimer(skillchecktimer[i]);
		KillTimer(safebreaktime[i]);
		KillTimer(skillcheckrespond[i]);
	}
	MoveDynamicObject(bigvault, 1641.35510, 2907.80640, 989.11761, 3, 0.00000, 0.00000, 91.36140);
	return 1;
}

CancelVaultBreak(safeid)
{
	Vaults[safeid][vBreaking] = false;
	UpdateDynamic3DTextLabelText(Vaults[safeid][v3DText], -1, "{FFFFFF}/usedrill\n{FF0000}No one drilling");
	return 1;
}

IsByVaultDoor(playerid)
{
	if(IsPlayerInRangeOfPoint(playerid, 1.5, BANK_VAULT_COORDINATES)) return 1;
	return 0;
}

IsBySafe(playerid)
{
	new safeid = -1;

	for(new i = 0; i < MAX_VAULTS; i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 1.5, Vaults[i][vPosX], Vaults[i][vPosY], Vaults[i][vPosZ]))
		{
			safeid = i;
			break;
		}
	}
	return safeid;
}

SetupVaults()
{
	for(new i = 0; i < MAX_VAULTS; i++)
	{
		Vaults[i][vMoney] = 250000;
		Vaults[i][vBreaking] = false;
		Vaults[i][vOpened] = false;
		Vaults[i][vPosX] = VaultsPositions[i][0];
		Vaults[i][vPosY] = VaultsPositions[i][1];
		Vaults[i][vPosZ] = VaultsPositions[i][2];
		Vaults[i][vPosRX] = VaultsPositions[i][3];
		Vaults[i][vPosRY] = VaultsPositions[i][4];
		Vaults[i][vPosRZ] = VaultsPositions[i][5];

		Vaults[i][vObject] = CreateDynamicObject(2332, Vaults[i][vPosX], Vaults[i][vPosY], Vaults[i][vPosZ], Vaults[i][vPosRX], Vaults[i][vPosRY], Vaults[i][vPosRZ]);
		Vaults[i][v3DText] = CreateDynamic3DTextLabel("{FFFFFF}/usedrill\n{FF0000}No one drilling", -1, Vaults[i][vPosX], Vaults[i][vPosY], Vaults[i][vPosZ]+0.8, 5);
	}
	return 1;
}

CMD:usedrill(playerid, params[])
{
	if(PlayerInfo[playerid][pLevel] < 4) return SendClientMessage(playerid, COLOR_GREY, "You need to be level 4+ to use this.");
	if(PlayerInfo[playerid][pDrill] != 1) return SendClientMessage(playerid, COLOR_GREY, "You don't have a drill");
	if(!IsByVaultDoor(playerid) && !bankactive) return SendClientMessage(playerid, COLOR_GREY, "You are not near the vault's door.");
	if(bankactive && IsBySafe(playerid) == -1) return SendClientMessage(playerid, COLOR_GREY, "You are not near a safe.");
	if(GetLEOCount() < 7 && !bankactive) return SendClientMessage(playerid, COLOR_GREY, "There must be 7 LEOs on (SAPD, FBI or SANG, admins excluded)");
	if(gettime() < TimeAfterBankJob && !bankactive) return SendClientMessage(playerid, COLOR_GREY, "The bank robbery system is currently on cooldown.");
	if(goinglaundering) return SendClientMessage(playerid, COLOR_GREY, "You may not launder more money as the robber stopped robbing.");

	if(!bankactive) StartBankRobbery(playerid); // break into the vault
	else if(bankactive) // break into a safe
	{
		new safeid = IsBySafe(playerid);
		if(Vaults[safeid][vOpened]) return SendClientMessage(playerid, COLOR_GREY, "This safe is already opened.");
		if(Vaults[safeid][vBreaking]) return SendClientMessage(playerid, COLOR_GREY, "Someone is already breaking into this safe");
		BreakVault(playerid, safeid);
	}
	return 1;
}

CMD:loadsafemoney(playerid, params[])
{
	if(!bankactive) return SendClientMessage(playerid, COLOR_GREY, "A bank robbery is not active.");
	new safeid = IsBySafe(playerid);
	if(safeid == -1) return SendClientMessage(playerid, COLOR_GREY, "You are not by a safe.");
	if(!Vaults[safeid][vOpened]) return SendClientMessage(playerid, COLOR_GREY, "This safe is not opened (/usedrill to start breaking into it).");
	if(Vaults[safeid][vMoney] == 0) return SendClientMessage(playerid, COLOR_GREY, "Someone already loaded the money from this safe.");

	bankmoneygathered += 250000;
	Vaults[safeid][vMoney] = 0;

	if(bankmoneygathered == 1500000) SetupMoneyLaundering(bankrobber);

	UpdateDynamic3DTextLabelText(Vaults[safeid][v3DText], -1, "{00FF00}Opened\n{FFFFFF}Loaded ({00FF00}$0 {FFFFFF}inside)");
	new msg[128];
	format(msg, sizeof(msg), "You have loaded $250,000 from the safe (total: %d), the main robber may use /stopbankrob to stop robbing.", bankmoneygathered);
	SendClientMessage(playerid, COLOR_LIGHTBLUE, msg);
	SendClientMessage(playerid, COLOR_LIGHTBLUE, "NOTE: This will prevent you from gathering more money in this robbery.");
	return 1;
}

CMD:stopbankrob(playerid, params[])
{
	if(!bankactive) return SendClientMessage(playerid, COLOR_GREY, "There is currently no active bank robbery.");
	if(bankrobber != playerid) return SendClientMessage(playerid, COLOR_GREY, "Only the main robber may use this.");
	if(bankmoneygathered < 750000) return SendClientMessage(playerid, COLOR_GREY, "There must be at least $750,000 money laundered before stopping the robbery.");

	SetupMoneyLaundering(playerid);
	return 1;
}


CMD:checkbanktime(playerid, params[])
{
    new msg[128];

    new 
        diff = TimeAfterBankJob-gettime(),
        hours, minutes, seconds;

    if(diff < 0) return SendClientMessage(playerid, COLOR_GREY, "Bank robbery is available.");

    hours = diff / 3600;
    diff %= 3600;

    minutes = diff/60, 
    diff %= 60;

    seconds = diff;

    format(msg, sizeof(msg), "Time left: %d:%d:%d", hours, minutes, seconds);
    SendClientMessage(playerid, COLOR_GREY, msg);
    return 1;    
}

CMD:allowrobbank(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 3) return AdmErrorMsg;

	SendClientMessage(playerid, COLOR_LIGHTBLUE, "Bank robbery is now allowed.");
	TimeAfterBankJob = gettime();
	return 1;
}

CMD:bankhelp(playerid, params[])
{
	SendClientMessage(playerid, COLOR_YELLOW, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	SendClientMessage(playerid, COLOR_WHITE, "/checkbanktime - check when you can rob the bank again (24 hours cooldown since last robbery)");
	SendClientMessage(playerid, COLOR_WHITE, "/usedrill - drill into the big vault or into a safe");
	SendClientMessage(playerid, COLOR_WHITE, "/stopbankrob - can only be used by the main robber, stops the robbery ($750,000 loaded required)");
	SendClientMessage(playerid, COLOR_WHITE, "/loadsafemoney - launder money (does not give you money) which can be collected by the main robber at the checkpoint");
	SendClientMessage(playerid, COLOR_WHITE, "Notes - drill is unlimited but lost on death. 7 LEOs+ (PD/FBI/SANG) are required to rob the bank (admins do not count).");
	SendClientMessage(playerid, COLOR_WHITE, "You have a 50 percents chance to get a skill check when breaking into a safe.");
	SendClientMessage(playerid, COLOR_WHITE, "You have 3 seconds to respond to it (press SPACE or your custom sprint key). Only the main robber may collect the money.");
	SendClientMessage(playerid, COLOR_YELLOW, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	return 1;
}