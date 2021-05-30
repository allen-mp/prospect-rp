
/*
	Isn't finished just uploading to GitHub to back it up. I plan to finish it tommorow from upload date.

	also wrote half of this half asleep so expect it to have bad naming conventions and readability to be ass
*/


#define DIALOG_CARTEL_SAFE			5000
#define DIALOG_CARTEL_SAFE_WITHDRAW 5001
#define DIALOG_CARTEL_SAFE_GUN_1	5002
#define DIALOG_CARTEL_SAFE_GUN_2	5003
#define DIALOG_CARTEL_SAFE_GUN_3	5004
#define DIALOG_CARTEL_SAFE_GUN_4	5005
#define DIALOG_CARTEL_SAFE_GUN_5	5006
#define DIALOG_CARTEL_SAFE_GUN_6	5007
#define DIALOG_CARTEL_SAFE_GUN_7	5008
#define DIALOG_CARTEL_SAFE_GUN_8	5009
#define DIALOG_CARTEL_SAFE_GUN_9	5010
#define DIALOG_CARTEL_SAFE_GUN_10	5011
#define DIALOG_CARTEL_SAFE_CRACK	5012
#define DIALOG_CARTEL_SAFE_WEED		5013


IntComma(number, delimiter[2] = ",")
{
	new int_string[10 + 3 + 1 + 1];
 
	format(int_string, sizeof(int_string), "%d", number >= 0 ? number : -number);
 
	new value = strlen(int_string);
 
	switch(value)
	{
		case 4..6:
					strins(int_string, delimiter, value-3,1);
		case 7..9:
					strins(int_string, delimiter, value-3,1), 
					strins(int_string, delimiter, value-6,1);
		case 10..12:
					strins(int_string, delimiter, value-3,1), 
					strins(int_string, delimiter, value-6,1), 
					strins(int_string, delimiter, value-9,1);
	}	
	if(number < 0) strins(int_string, "-", 0); 
	return int_string;
}

new gSafeGuns[11],
	gSafeMoney,
	gSafeCrack,
	gSafeWeed;

Hook:GroupSafe_OnGameModeInit()
{
	INI_ParseFile("cartelsafe.ini", "LoadGroupSafe", true, false, 1);
	return 1;
}

Hook:GroupSafe_OnGameModeExit()
{
	SaveGroupSafe();
	return 1;
}

SaveGroupSafe()
{
	new INI:cartelsafe; 

	INI_Open("cartelsafe.ini");

	INI_SetTag(cartelsafe, "Misc");
	INI_WriteInt(cartelsafe, "Money", gSafeMoney);
	INI_WriteInt(cartelsafe, "Weed", gSafeWeed);
	INI_WriteInt(cartelsafe, "Crack", gSafeCrack);

	INI_SetTag(cartelsafe, "Weapons");
	INI_WriteInt(cartelsafe, "Weapon 0", gSafeGuns[0]);
	INI_WriteInt(cartelsafe, "Weapon 1", gSafeGuns[1]);
	INI_WriteInt(cartelsafe, "Weapon 2", gSafeGuns[2]);
	INI_WriteInt(cartelsafe, "Weapon 3", gSafeGuns[0]);
	INI_WriteInt(cartelsafe, "Weapon 4", gSafeGuns[1]);
	INI_WriteInt(cartelsafe, "Weapon 5", gSafeGuns[2]);
	INI_WriteInt(cartelsafe, "Weapon 6", gSafeGuns[0]);
	INI_WriteInt(cartelsafe, "Weapon 7", gSafeGuns[1]);
	INI_WriteInt(cartelsafe, "Weapon 8", gSafeGuns[2]);
	INI_WriteInt(cartelsafe, "Weapon 9", gSafeGuns[0]);
	INI_WriteInt(cartelsafe, "Weapon 10", gSafeGuns[1]);

	INI_Close(cartelsafe);
}


forward LoadGroupSafe(name[], value[]);
public LoadGroupSafe(name[], value[])
{
	INI_Int("Money", gSafeMoney);
	INI_Int("Crack", gSafeCrack);
	INI_Int("Weed",  gSafeWeed);

	INI_Int("Weapon 0", gSafeGuns[0]);
	INI_Int("Weapon 1", gSafeGuns[1]);
	INI_Int("Weapon 2", gSafeGuns[2]);
	INI_Int("Weapon 3", gSafeGuns[3]);
	INI_Int("Weapon 4", gSafeGuns[4]);
	INI_Int("Weapon 5", gSafeGuns[5]);
	INI_Int("Weapon 6", gSafeGuns[6]);
	INI_Int("Weapon 7", gSafeGuns[7]);
	INI_Int("Weapon 8", gSafeGuns[8]);
	INI_Int("Weapon 9", gSafeGuns[9]);
	INI_Int("Weapon 10", gSafeGuns[10]);
	return 1;
}

CMD:gsafe(playerid, params[])
{
	new safeContents[600];
	format(safeContents, sizeof(safeContents), "Money: {429122}$%s{FFFFFF}\nCrack: %i\nWeed: %i\nWeapon: %i\nWeapon: %i\nWeapon: %i\nWeapon: %i\nWeapon: %i\nWeapon: %i\nWeapon: %i\nWeapon: %i\nWeapon: %i\nWeapon: %i",
		IntComma(gSafeMoney), gSafeCrack, gSafeWeed, gSafeGuns[0], gSafeGuns[1], gSafeGuns[2], gSafeGuns[3], gSafeGuns[4], gSafeGuns[5], gSafeGuns[6], gSafeGuns[7],
		gSafeGuns[8], gSafeGuns[9], gSafeGuns[10]);

	ShowPlayerDialog(playerid, DIALOG_CARTEL_SAFE, DIALOG_STYLE_LIST, "Group Safe", safeContents, "Select", "Close");
	return 1;
}

Hook:GroupSafe_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_CARTEL_SAFE)
	{
		if(!response)
			return ShowPlayerDialog(playerid, -1, 0, "", "", "", "");

		if(listitem == 0)
		{
			ShowPlayerDialog(playerid, DIALOG_CARTEL_SAFE_WITHDRAW, DIALOG_STYLE_INPUT, "Group Safe", "Enter an amount you'd like to deposit or withdraw:", "Deposit", "Withdraw");
		}
		else if(listitem == 1)
		{
			ShowPlayerDialog(playerid, DIALOG_CARTEL_SAFE_CRACK, DIALOG_STYLE_INPUT, "Group Safe", "Enter an amount of crack you'd like to deposit or withdraw:", "Deposit", "Withdraw");
		}
		else if(listitem == 2)
		{
			ShowPlayerDialog(playerid, DIALOG_CARTEL_SAFE_WEED, DIALOG_STYLE_INPUT, "Group Safe", "Enter an amount of weed you'd like to deposit or withdraw:", "Deposit", "Withdraw");			
		}
		else if(listitem == 3)
		{
			ShowPlayerDialog(playerid, DIALOG_CARTEL_SAFE_GUN_1, DIALOG_STYLE_LIST, "Group Safe", "Would you like to store or withdraw this weapon?", "Store", "Withdraw");
		}
		else if(listitem == 4)
		{
			ShowPlayerDialog(playerid, DIALOG_CARTEL_SAFE_GUN_2, DIALOG_STYLE_LIST, "Group Safe", "Would you like to store or withdraw this weapon?", "Store", "Withdraw");
		}
		else if(listitem == 5)
		{
			ShowPlayerDialog(playerid, DIALOG_CARTEL_SAFE_GUN_3, DIALOG_STYLE_LIST, "Group Safe", "Would you like to store or withdraw this weapon?", "Store", "Withdraw");
		}
		else if(listitem == 6)
		{
			ShowPlayerDialog(playerid, DIALOG_CARTEL_SAFE_GUN_4, DIALOG_STYLE_LIST, "Group Safe", "Would you like to store or withdraw this weapon?", "Store", "Withdraw");
		}
		else if(listitem == 7)
		{
			ShowPlayerDialog(playerid, DIALOG_CARTEL_SAFE_GUN_5, DIALOG_STYLE_LIST, "Group Safe", "Would you like to store or withdraw this weapon?", "Store", "Withdraw");
		}
		else if(listitem == 8)
		{
			ShowPlayerDialog(playerid, DIALOG_CARTEL_SAFE_GUN_6, DIALOG_STYLE_LIST, "Group Safe", "Would you like to store or withdraw this weapon?", "Store", "Withdraw");
		}
		else if(listitem == 9)
		{
			ShowPlayerDialog(playerid, DIALOG_CARTEL_SAFE_GUN_7, DIALOG_STYLE_LIST, "Group Safe", "Would you like to store or withdraw this weapon?", "Store", "Withdraw");
		}
		else if(listitem == 10)
		{
			ShowPlayerDialog(playerid, DIALOG_CARTEL_SAFE_GUN_8, DIALOG_STYLE_LIST, "Group Safe", "Would you like to store or withdraw this weapon?", "Store", "Withdraw");
		}
		else if(listitem == 11)
		{
			ShowPlayerDialog(playerid, DIALOG_CARTEL_SAFE_GUN_9, DIALOG_STYLE_LIST, "Group Safe", "Would you like to store or withdraw this weapon?", "Store", "Withdraw");
		}
		else if(listitem == 12)
		{
			ShowPlayerDialog(playerid, DIALOG_CARTEL_SAFE_GUN_10, DIALOG_STYLE_LIST, "Group Safe", "Would you like to store or withdraw this weapon?", "Store", "Withdraw");
		}
	}
	if(dialogid == DIALOG_CARTEL_SAFE_WITHDRAW)
	{
		if(response)
		{
			if(!isnumeric(inputtext))
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}You must insert a numeric amount!");

			new amount = strval(inputtext);
			if(GetPlayerMoney(playerid) < amount)
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}You don't have enough money to deposit the desired amount!");

			if(amount >= 1000000)
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}You cannot deposit more then a million at a time.");

			if(amount <= 0)
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}You cannot deposit less then $1.");

			gSafeMoney += amount;
			GivePlayerMoney(playerid, -amount);

			new str[128];
			format(str, sizeof(str), "SERVER: {FFFFFF}You've deposited $%s into the safe making the total balance: $%s", IntComma(amount), IntComma(gSafeMoney));
			SendClientMessage(playerid, COLOR_BLUE, str);
		} 
		else {
			if(!isnumeric(inputtext))
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}You must insert a numeric amount!");

			new amount = strval(inputtext);
			if(amount > gSafeMoney)
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}The safe doesn't have that much money in it!");
			
			if(amount >= 1000000)
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}You cannot withdraw more then a million at a time.");

			if(amount <= 0)
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}You cannot withdraw less then $1.");

			gSafeMoney -= amount;
			GivePlayerMoney(playerid, amount);
			
			new str[128];
			format(str, sizeof(str), "SERVER: {FFFFFF}You've withdrawn $%s into the safe making the total balance: $%s", IntComma(amount), IntComma(gSafeMoney));
			SendClientMessage(playerid, COLOR_BLUE, str);
		}
	}
	if(dialogid == DIALOG_CARTEL_SAFE_WEED)
	{
		if(response)
		{
			if(!isnumeric(inputtext))
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}You must insert a numeric amount!");

			new amount = strval(inputtext);
			if(PlayerInfo[playerid][pPot] < amount)
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}You don't have enough pot to deposit the amount desired.");

			if(amount <= 0)
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}You cannot deposit less then 1 gram of pot.");

			PlayerInfo[playerid][pPot] -= amount;
			gSafeWeed += amount;

			new str[128];
			format(str, sizeof(str), "SERVER: {FFFFFF}You've deposited %i grams of pot into the safe making the total grams: %i.", amount, gSafeWeed);
			SendClientMessage(playerid, COLOR_BLUE, str);

		}
		else {
			if(!isnumeric(inputtext))
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}You must insert a numeric amount!");

			new amount = strval(inputtext);
			if(amount > gSafeWeed)
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}The safe doesn't have that much pot in it!");

			if(amount <= 0)
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}You cannot withdraw less then 1 gram of pot.");

			PlayerInfo[playerid][pPot] += amount;
			gSafeWeed -= amount;

			new str[128];
			format(str, sizeof(str), "SERVER: {FFFFFF}You've withdrawn %i grams of pot into the safe making the total grams: %i.", amount, gSafeWeed);
			SendClientMessage(playerid, COLOR_BLUE, str);
		}
	}
	if(dialogid == DIALOG_CARTEL_SAFE_CRACK)
	{
		if(response)
		{
			if(!isnumeric(inputtext))
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}You must insert a numeric amount!");

			new amount = strval(inputtext);
			if(PlayerInfo[playerid][pPot] < amount)
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}You don't have enough crack to deposit the amount desired.");

			if(amount <= 0)
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}You cannot deposit less then 1 gram of crack.");

			PlayerInfo[playerid][pCrack] -= amount;
			gSafeCrack += amount;

			new str[128];
			format(str, sizeof(str), "SERVER: {FFFFFF}You've deposited %i grams of crack into the safe making the total grams: %i.", amount, gSafeCrack);
			SendClientMessage(playerid, COLOR_BLUE, str);

		}
		else {
			if(!isnumeric(inputtext))
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}You must insert a numeric amount!");

			new amount = strval(inputtext);
			if(amount > gSafeCrack)
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}The safe doesn't have that much crack in it!");

			if(amount <= 0)
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}You cannot withdraw less then 1 gram of crack.");

			PlayerInfo[playerid][pCrack] += amount;
			gSafeCrack -= amount;

			new str[128];
			format(str, sizeof(str), "SERVER: {FFFFFF}You've withdrawn %i grams of crack into the safe making the total grams: %i.", amount, gSafeCrack);
			SendClientMessage(playerid, COLOR_BLUE, str);
		}
	}
	if(dialogid == DIALOG_CARTEL_SAFE_GUN_1)
	{
		if(response)
		{
			if(gSafeGuns[0] != 0)
				return SendClientMessage(playerid, COLOR_BLUE, "NOTICE: {FFFFFF}There is already a gun in this slot!");

			gSafeGuns[0] = GetPlayerWeapon(playerid);

			new str[128], gunstr[24];
			GetWeaponName(GetPlayerWeapon(playerid), gunstr, 24);
			format(str, sizeof(str), "NOTICE: {FFFFFF}You've stored a %s in the safe!", gunstr);
			SendClientMessage(playerid, COLOR_BLUE, str);
		} else {
			if(GetPlayerWeapon(playerid) == gSafeGuns[0])
				return SendClientMessage(playerid, COLOR_BLUE, "ERROR: {FFFFFF}You already have this gun!");
			
			GivePlayerGun(playerid, gSafeGuns[0]);
		}
	}
	return 1;
}
