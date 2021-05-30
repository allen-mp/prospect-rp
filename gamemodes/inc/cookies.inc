new addss;
CMD:givecookie(playerid, params[])
{
	new giveplayerid, reason[128];
	if(PlayerInfo[playerid][pAdmin] >= 4)
	{
		if(!sscanf(params, "us[128]", giveplayerid, reason))
		{
			new str[128];
			if(IsPlayerConnected(giveplayerid))
			{
				if(giveplayerid != INVALID_PLAYER_ID)
				{
					PlayerInfo[giveplayerid][pCookies] += 1;
					if(PlayerInfo[playerid][pStealthed] == 1)
					{
						format(str, sizeof(str), "{FF6347}SERVER: %s has been given a cookie by an administrator, reason: %s", PlayerICName(giveplayerid), (reason));
					}
					else 
					{
						format(str, sizeof(str), "{FF6347}SERVER: %s has been given a cookie by %s, reason: %s", PlayerICName(giveplayerid), PlayerICName(playerid), (reason));
					}
					SendClientMessageToAll(COLOR_WHITE, str);
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /givecookie [playerid/PartOfName] [reason]");
		}
	}
	else
	{
		AdmErrorMsg;
	}
	return 1;
}

CMD:givecookieall(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 4)
	{
		AdmErrorMsg;
		return 1;
	}
	new str[128];
	new reason[128];
	if(!sscanf(params, "s[128]", reason))
	{
		//for(new i = 0;i<MAX_PLAYERS;i++)
		foreach( new i: Player )
		{
			if(IsPlayerConnected(i))
			{
				if(i != INVALID_PLAYER_ID)
				{
					PlayerInfo[i][pCookies] += 1;
				}
			}
		}
		if(PlayerInfo[playerid][pStealthed] == 1)
		{
			format(str, sizeof(str), "{FF6347}SERVER: Everyone was given a cookie by %s, reason: %s", PlayerICName(playerid), reason);
		}
		else
		{
			format(str, sizeof(str), "{FF6347}SERVER: Everyone was given a cookie by an administrator, reason: %s", reason);
		}
		SendClientMessageToAll(COLOR_WHITE, str);
	}
	else
	{
		SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /givecookieall [Reason]");
	}
	return 1;
}

CMD:cookies(playerid, params[])
{
	new str[285];
	format(str, sizeof(str), "You have %d cookies.", PlayerInfo[playerid][pCookies]);
	SendClientMessage(playerid, COLOR_GREY, str);
	return 1;
}

CMD:usecookies(playerid, params[])
{
	new str[860];
	if((pDeathState[playerid] == PLAYER_STATE_REVIVE || PlayerCuffed[playerid] > 0 || PlayerTazed[playerid] > 0)) return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can't use this whilst inside of a vehicle.");
	else
	{
		format(str, sizeof(str), "Shoutout\t\t5 Cookie\nFull Armor\t\t10 Cookies\nFWS\t\t\t35 Cookies\n30k Materials\t\t40 Cookies\n$350,000\t\t20 Cookies\n50g Cannabis\t\t20 Cookies\n50g Cocaine\t\t50 Cookies\n50 Tabs Xanax\t\t40 Cookies\n50g Meth\t\t50 Cookies");
		ShowPlayerDialog(playerid, 5523, DIALOG_STYLE_LIST, "Cookie Shop", str, "Select", "Cancle");
	}

	return 1;
}

Hook:cuk_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	new str[5042];
	if(dialogid == 5523)
	{
		if(response)
		{
			if(listitem == 0)  // Shoutout
			{
				if(PlayerInfo[playerid][pCookies] < 5) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough cookies.");
				if(gettime() < (addss + 60) && (PlayerInfo[playerid][pAdmin] < 3))
				{
					format(str, sizeof(str), "Please wait %d seconds to post your shoutout.", (addss + 60) - gettime());
					SendClientMessage(playerid, COLOR_GRAD2, str);
					return 1;
				}
				ShowPlayerDialog(playerid, 5524, DIALOG_STYLE_INPUT, "Shoutout", "Enter text out the shoutout, it will show for every player:", "Submit", "Cancel");
				return 1;
			}
			if(listitem == 1)  // Full Armor
			{
				if(PlayerInfo[playerid][pCookies] < 10) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough cookies.");
				SetPlayerArmour(playerid, 100);
				PlayerInfo[playerid][pCookies] -=10;
				SendClientMessage(playerid, COLOR_LIGHTRED, "You have redeemed 2 cookis for a Full Armor to the entire server.");
				return 1;
			}
			if(listitem == 2)  // FWS
			{
				if(PlayerInfo[playerid][pCookies] < 35) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough cookies.");
				GivePlayerGun(playerid, 30);
				GivePlayerGun(playerid, 32);
				GivePlayerGun(playerid, 24);
				GivePlayerGun(playerid, 33);
				PlayerInfo[playerid][pCookies] -=35;
				SendClientMessage(playerid, COLOR_LIGHTRED, "You have redeemed 6 cookies for a Full Weapon Set.");
				return 1;
			}
			if(listitem == 3)  // 30k mats
			{
				if(PlayerInfo[playerid][pCookies] < 40) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough cookies.");
				PlayerInfo[playerid][pMats] +=30000;
				PlayerInfo[playerid][pCookies] -=40;
				SendClientMessage(playerid, COLOR_LIGHTRED, "You have redeemed 15 cookies for 30,000 materials.");
				return 1;
			}
			if(listitem == 4)  // $350k
			{
				if(PlayerInfo[playerid][pCookies] < 20) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough cookies.");
				GiveMoney(playerid, 350000);
				PlayerInfo[playerid][pCookies] -=20;
				SendClientMessage(playerid, COLOR_LIGHTRED, "You have redeemed 20 cookies for $350,000.");
				return 1;
			}
			if(listitem == 5)  // cannabis
			{
				if(PlayerInfo[playerid][pCookies] < 20) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough cookies.");
				PlayerInfo[playerid][pCannabis] +=50;
				PlayerInfo[playerid][pCookies] -=20;
				SendClientMessage(playerid, COLOR_LIGHTRED, "You have redeemed 20 cookies for 50g Cannabis.");
				return 1;
			}
			if(listitem == 6)  // coke
			{
				if(PlayerInfo[playerid][pCookies] < 50) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough cookies.");
				PlayerInfo[playerid][pCocaine] +=50;
				PlayerInfo[playerid][pCookies] -=50;
				SendClientMessage(playerid, COLOR_LIGHTRED, "You have redeemed 30 cookies for 50g Cocaine.");
				return 1;
			}
			if(listitem == 7)  // xanax
			{
				if(PlayerInfo[playerid][pCookies] < 40) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough cookies.");
				PlayerInfo[playerid][pXanax] +=50;
				PlayerInfo[playerid][pCookies] -=40;
				SendClientMessage(playerid, COLOR_LIGHTRED, "You have redeemed 40 cookies for 50tabs of Xanax.");
				return 1;
			}
			if(listitem == 8)  // xanax
			{
				if(PlayerInfo[playerid][pCookies] < 50) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough cookies.");
				PlayerInfo[playerid][pMeth] +=50;
				PlayerInfo[playerid][pCookies] -=50;
				SendClientMessage(playerid, COLOR_LIGHTRED, "You have redeemed 50 cookies for 50g Meth.");
				return 1;
			}
		}
		return 1;
	}
	if(dialogid == 5524)
	{
		if(response)
		    {
		    	if(gettime() < (addss + 60) && (PlayerInfo[playerid][pAdmin] < 3))
				{
					format(str, sizeof(str), "Please wait %d seconds to post your shoutout.", (addss + 60) - gettime());
					SendClientMessage(playerid, COLOR_GRAD2, str);
					return 1;
				}
		        if(!(3 <= strlen(inputtext) <= 64))
		        {
		            SendClientMessage(playerid, COLOR_GREY, "Your shoutout may only contain 3 to 64 characters.");
		            return ShowPlayerDialog(playerid, 5524, DIALOG_STYLE_INPUT, "Shoutout", "Enter text out the shoutout, it will show for every player:", "Submit", "Cancel");
				}
				if(PlayerInfo[playerid][pCookies] < 5)
                {
                    return SendClientMessage(playerid, COLOR_GREY, "You don't have enough cookies. You can't redeem this item.");
                }

                PlayerInfo[playerid][pCookies] -= 5;
                format(str, sizeof(str), "{ff0083}Shoutout from %s: %s", PlayerOOCName(playerid), inputtext);
				SendClientMessageToAll(COLOR_GREY, str);
				SendClientMessage(playerid, COLOR_LIGHTRED, "You have redeemed 1 cookie for a Shoutout to the entire server.");
				if(PlayerInfo[playerid][pAdmin] < 4) adds = gettime();
			}
		return 1;
	}
	return 1;
}