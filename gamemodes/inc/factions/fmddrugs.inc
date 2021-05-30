/*
  ______ __  __ _____    _____  _____  _    _  _____  _____    _______     _______ _______ ______ __  __   ______     __  _____  ______  _____ __  __  ____  _   _ _____  
 |  ____|  \/  |  __ \  |  __ \|  __ \| |  | |/ ____|/ ____|  / ____\ \   / / ____|__   __|  ____|  \/  | |  _ \ \   / / |  __ \|  ____|/ ____|  \/  |/ __ \| \ | |  __ \ 
 | |__  | \  / | |  | | | |  | | |__) | |  | | |  __| (___   | (___  \ \_/ / (___    | |  | |__  | \  / | | |_) \ \_/ /  | |  | | |__  | (___ | \  / | |  | |  \| | |  | |
 |  __| | |\/| | |  | | | |  | |  _  /| |  | | | |_ |\___ \   \___ \  \   / \___ \   | |  |  __| | |\/| | |  _ < \   /   | |  | |  __|  \___ \| |\/| | |  | | . ` | |  | |
 | |    | |  | | |__| | | |__| | | \ \| |__| | |__| |____) |  ____) |  | |  ____) |  | |  | |____| |  | | | |_) | | |    | |__| | |____ ____) | |  | | |__| | |\  | |__| |
 |_|    |_|  |_|_____/  |_____/|_|  \_\\____/ \_____|_____/  |_____/   |_| |_____/   |_|  |______|_|  |_| |____/  |_|    |_____/|______|_____/|_|  |_|\____/|_| \_|_____/ 
                                                                                                                                                                          
                                                                                                                                                                          
*/

#define DIALOG_FMD_DRUGS 			17854
#define DIALOG_FMD_DRUGS_CHOOSE 	17855
#define DIALOG_FMD_DRUGS_PLAYER 	17856
#define DIALOG_FMD_DRUGS_AMOUNT	 	17857
#define DIALOG_FMD_DRUGS_PRICE	 	17858

#define DRUGS_CHOICE_XANAX		1
#define DRUGS_CHOICE_CANNABIS	2

new drugschoice[MAX_PLAYERS], playerchoice[MAX_PLAYERS], playeramount[MAX_PLAYERS], playerprice[MAX_PLAYERS];

GetOfferDrugsName(drug)
{
	new drugname[10];
	if(drug == DRUGS_CHOICE_XANAX) drugname = "Xanax";
	else if(drug == DRUGS_CHOICE_CANNABIS) drugname = "Cannabis";
	return drugname;
}

CMD:fmddrugs(playerid, params[])
{
	if(PlayerInfo[playerid][pMember] != FACTION_FMD) return AdmErrorMsg;
	if(PlayerInfo[playerid][pRank] < 4) return SendClientMessage(playerid, COLOR_GREY, "Only R4+ can use this.");
	if(!IsPlayerInRangeOfPoint(playerid, 10.0, 2142.3999, 1467.9122, 10001.0830)) return SendClientMessage(playerid, COLOR_GREY, "You must be by the pharmacy counter to do this."); //@developers replace x y z with the pharmacy's coordinates

	ShowPlayerDialog(playerid, DIALOG_FMD_DRUGS, DIALOG_STYLE_LIST, "{FF8282}FMD Drugs Database", "{FFFFFF}Give Drugs\nDrugs Logs", "Select", "Cancel");
	return 1;
}

CMD:buyfmddrugs(playerid, params[])
{
	if(playerchoice[playerid] == -1) return SendClientMessage(playerid, COLOR_GREY, "You do not have an offer from a FMD member to buy drugs.");
	if(!IsPlayerConnected(playerchoice[playerid]) || playerchoice[playerid] == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_GREY, "Seller disconnected.");
	if(!IsPlayerInRangeOfPlayer(4.0, playerid, playerchoice[playerid])) return SendClientMessage(playerid, COLOR_GREY, "You are not near the player.");
	if(PlayerInfo[playerid][pCash] < playerprice[playerid]) return SendClientMessage(playerid, COLOR_GREY, "You can not afford this.");

	GiveMoney(playerid, -playerprice[playerid]);
	FMD += playerprice[playerid];

	if(drugschoice[playerid] == DRUGS_CHOICE_XANAX) PlayerInfo[playerid][pXanax] += playeramount[playerid];
	else if(drugschoice[playerid] == DRUGS_CHOICE_CANNABIS) PlayerInfo[playerid][pCannabis] += playeramount[playerid];

	new msg[100];
	format(msg, sizeof(msg), "You have bought %d %s for %d from paramedic %s", playeramount[playerid], GetOfferDrugsName(drugschoice[playerid]), playerprice[playerid], PlayerICName(playerchoice[playerid]));
	SendClientMessage(playerid, COLOR_LIGHTBLUE, msg);
	format(msg, sizeof(msg), "%s has bought %d %s from you (FMD bank earning: %d).", PlayerICName(playerid), playeramount[playerid], GetOfferDrugsName(drugschoice[playerid]), playerprice[playerid]);
	SendClientMessage(playerchoice[playerid], COLOR_LIGHTBLUE, msg);

	new query[200], day, month, year, hour, minute, second, dstr[40];
	gettime(hour, minute, second);
	getdate(year, month, day);
	format(dstr, sizeof(dstr), "%d/%d/%d %d:%d:%d", day, month, year, hour, minute, second);
	mysql_format(sqlGameConnection, query, sizeof(query), "INSERT INTO `fmddrugs` (`Name`, `GivenBy`, `Date`, `Type`, `Amount`, `Price`) VALUES('%e', '%e', '%e', '%e', %d, %d)", PlayerOOCName(playerid), PlayerOOCName(playerchoice[playerid]), dstr, GetOfferDrugsName(drugschoice[playerid]), playeramount[playerid], playerprice[playerid]);
	mysql_pquery(sqlGameConnection, query, "FMDDrugsLog", "sssdd", PlayerOOCName(playerid), PlayerOOCName(playerchoice[playerid]), GetOfferDrugsName(drugschoice[playerid]), playeramount[playerid], playerprice[playerid]);

	playerchoice[playerid] = -1;
	return 1;
}

forward FMDDrugsLog(player[], givenby[], drug[], amount, price);
public FMDDrugsLog(player[], givenby[], drug[], amount, price)
{
	printf("%s has been given %d %s by %s for $%d (FMD Sale)", player, amount, drug, givenby, price);
	new msg[128];
	format(msg, sizeof(msg), "{FF0000}AdmWarning{FFFFFF}: %s has been given %d %s by %s for $%d (FMD Sale)", player, amount, drug, givenby, price);
	SendAdminMessage(-1, msg);
	return 1;
}

Hook:fmddrugs_OnPlayerConnect(playerid)
{
	playerchoice[playerid] = -1;
	return 1;
}

Hook:fmddrugs_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_FMD_DRUGS)
	{
		if(response)
		{
			switch(listitem)
			{
				// give drugs
				case 0: 
				{
					ShowPlayerDialog(playerid, DIALOG_FMD_DRUGS_CHOOSE, DIALOG_STYLE_LIST, "{FF8282}Which drugs would you like to give?", "{FFFFFF}Xanax\nCannabis", "Select", "Cancel");
				}	
				// check drugs logs
				case 1: 
				{
					new query[128], Name[24], GivenBy[24], rows, fields, cstr[5000], Date[24], Cache:result, amount, price;
					mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `fmddrugs` ORDER BY `ID` DESC LIMIT 50");
					result = mysql_query(sqlGameConnection, query);
					cache_get_data(rows, fields, sqlGameConnection);

					if(!rows) return SendMDCMessage(playerid, "No logs on record.");

					format(cstr, sizeof(cstr), "Date\t\tName\t\tGiven By\t\tAmount | Price\n");
					for(new i = 0; i < rows; i++)
					{
						cache_get_field_content(i, "Name", Name);
						cache_get_field_content(i, "GivenBy", GivenBy);
						cache_get_field_content(i, "Date", Date);
						amount = cache_get_field_content_int(i, "Amount");
						price = cache_get_field_content_int(i, "Price");
						format(cstr, sizeof(cstr), "%s{FF8282}(%s)\t{FF8282}%s\t{FF8282}%s\t{FF8282}%d | $%d\n", cstr, Date, Name, GivenBy, amount, price);
					}
					ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_TABLIST_HEADERS, "{FF8282}FMD Drugs Database", cstr, "Okay", "");
					cache_delete(Cache:result, sqlGameConnection);
				}
			}
		}
	}
	if(dialogid == DIALOG_FMD_DRUGS_CHOOSE)
	{
		if(response)
		{
			if(listitem == 0) drugschoice[playerid] = DRUGS_CHOICE_XANAX;
			else if(listitem == 1) drugschoice[playerid] = DRUGS_CHOICE_CANNABIS;
			ShowPlayerDialog(playerid, DIALOG_FMD_DRUGS_PLAYER, DIALOG_STYLE_INPUT, "Player ID", "{FFFFFF}Please enter the ID of the player", "Next", "Cancel");
		}
	}
	if(dialogid == DIALOG_FMD_DRUGS_PLAYER)
	{
		if(response)
		{
			new input = strval(inputtext);
			if(!IsNumeric(inputtext) || !IsPlayerConnected(input) || input == INVALID_PLAYER_ID)
			{
				SendClientMessage(playerid, COLOR_GREY, "Invalid player ID.");
				return ShowPlayerDialog(playerid, DIALOG_FMD_DRUGS_PLAYER, DIALOG_STYLE_INPUT, "Player ID", "{FFFFFF}Please enter the ID of the player", "Next", "Cancel");
			}
			else if(PlayerInfo[input][pMember] == FACTION_FMD)
			{
				SendClientMessage(playerid, COLOR_GREY, "You may not sell drugs to other FMD members.");
				return ShowPlayerDialog(playerid, DIALOG_FMD_DRUGS_PLAYER, DIALOG_STYLE_INPUT, "Player ID", "{FFFFFF}Please enter the ID of the player", "Next", "Cancel");	
			}
			else if(input == playerid)
			{
				SendClientMessage(playerid, COLOR_GREY, "You may not sell drugs to yourself.");
				return ShowPlayerDialog(playerid, DIALOG_FMD_DRUGS_PLAYER, DIALOG_STYLE_INPUT, "Player ID", "{FFFFFF}Please enter the ID of the player", "Next", "Cancel");		
			}
			else
			{
				playerchoice[playerid] = input;
				ShowPlayerDialog(playerid, DIALOG_FMD_DRUGS_AMOUNT, DIALOG_STYLE_INPUT, "Drugs Amount", "{FFFFFF}Please enter the amount of drugs", "Next", "Cancel");
			}
		}
	}
	if(dialogid == DIALOG_FMD_DRUGS_AMOUNT)
	{
		if(response)
		{
			new input = strval(inputtext);
			if(!IsNumeric(inputtext) || input < 1 || input > 20)
			{
				SendClientMessage(playerid, COLOR_GREY, "Invalid amount (amount must be between 1-20)");
				return ShowPlayerDialog(playerid, DIALOG_FMD_DRUGS_AMOUNT, DIALOG_STYLE_INPUT, "Drugs Amount", "{FFFFFF}Please enter the amount of drugs", "Next", "Cancel");
			}
			else
			{
				playeramount[playerid] = input;
				ShowPlayerDialog(playerid, DIALOG_FMD_DRUGS_PRICE, DIALOG_STYLE_INPUT, "Drugs Price", "{FFFFFF}Please enter the price for the drugs", "Done", "Cancel");
			}
		}
	}
	if(dialogid == DIALOG_FMD_DRUGS_PRICE)
	if(response)
	{
		new input = strval(inputtext);
		if(!IsNumeric(inputtext) || input < 5000 || input > 100000)
		{
			SendClientMessage(playerid, COLOR_GREY, "Invalid price (price must be between $5,000 and $100,000)");
			return ShowPlayerDialog(playerid, DIALOG_FMD_DRUGS_PRICE, DIALOG_STYLE_INPUT, "Drugs Price", "{FFFFFF}Please enter the price for the drugs", "Done", "Cancel");
		}
		else
		{
			new msg[100];
			playerprice[playerid] = input;
			if(!IsPlayerConnected(playerchoice[playerid]) || playerchoice[playerid] == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_GREY, "Player disconnected.");

			new target = playerchoice[playerid];
			format(msg, sizeof(msg), "%s has offered to sell you %d drugs (%s) for $%d (/buyfmddrugs to accept)", PlayerICName(playerid), playeramount[playerid], GetOfferDrugsName(drugschoice[playerid]), playerprice[playerid]);
			SendClientMessage(target, COLOR_LIGHTBLUE, msg);

			format(msg, sizeof(msg), "You have offered to sell %s %d %s for $%d.", PlayerICName(target), playeramount[playerid], GetOfferDrugsName(drugschoice[playerid]), playerprice[playerid]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, msg);

			playerchoice[target] = playerid;
			drugschoice[target] = drugschoice[playerid];
			playeramount[target] = playeramount[playerid];
			playerprice[target] = playerprice[playerid];
		}
	}
	return 1;
}