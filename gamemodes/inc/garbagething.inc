new searched_ptimer[MAX_PLAYERS];

forward Timer_searched(playerid);
public Timer_searched(playerid)
{
    searched_ptimer[playerid] = 0;
    return 1;
}

CMD:lookin(playerid, params[])
{
	if(PlayerInfo[playerid][pLevel] < 4) return SendClientMessage(playerid, COLOR_GREY, "You should have level 4+ in order to use this command!");
	for(new i = 0; i < sizeof(trashInfo); i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 2.0, trashInfo[i][posX], trashInfo[i][posY], trashInfo[i][posZ]))
		{
			if(trashInfo[i][tAmount] > 0)
			{
               if(searched_ptimer[playerid] == 1) return SendClientMessage(playerid, COLOR_GREY, "You must wait 20 seconds before using this command again.");
			   new rand = random(10);
			   if(rand == 0)
			   {
				   SendClientMessage(playerid, COLOR_GREY, "You searched through the trash bin but found nothing!");
				   searched_ptimer[playerid] = 1;
				   SetTimerEx("Timer_searched", 20000, 0, "i", playerid);
				   /*format(str, sizeof(str), "* %s searches through the trash bin and finds nothing inside.", PlayerICName);
			       ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);*/
			   }
			   else if(rand == 1)
			   {
				   SendClientMessage(playerid, COLOR_GREY, "You searched through the trash bin and found Lockpick set!");
				   searched_ptimer[playerid] = 1;
			       PlayerInfo[playerid][pScrew] += 5;
				   trashInfo[i][tAmount] --;
				   SetTimerEx("Timer_searched", 20000, 0, "i", playerid);
				   /*format(str, sizeof(str), "* %s searches through the trash bin and finds nothing inside.", PlayerICName);
			       ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);*/
			   }
			   else if(rand == 2)
			   {
				   SendClientMessage(playerid, COLOR_GREY, "You searched through the trash bin and found nothing!");
				   searched_ptimer[playerid] = 1;
				   SetTimerEx("Timer_searched", 20000, 0, "i", playerid);
				   /*format(str, sizeof(str), "* %s searches through the trash bin and finds nothing inside.", PlayerICName);
			       ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);*/
			   }
			   else if(rand == 3)
			   {
				   SendClientMessage(playerid, COLOR_GREY, "You searched through the trash bin but found 5 ropes!");
				   searched_ptimer[playerid] = 1;
				   PlayerInfo[playerid][pRope] += 5;
				   trashInfo[i][tAmount] --;
				   SetTimerEx("Timer_searched", 20000, 0, "i", playerid);
				   /*format(str, sizeof(str), "* %s searches through the trash bin and finds nothing inside.", PlayerICName);
			       ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);*/
			   }
			   else if(rand == 4)
			   {
				   SendClientMessage(playerid, COLOR_GREY, "You searched through the trash bin but found nothing!");
				   searched_ptimer[playerid] = 1;
				   SetTimerEx("Timer_searched", 20000, 0, "i", playerid);
				   /*format(str, sizeof(str), "* %s searches through the trash bin and finds nothing inside.", PlayerICName);
			       ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);*/
			   }
			   else if(rand == 5)
			   {
				   SendClientMessage(playerid, COLOR_GREY, "You searched through the trash bin and found SD Pistol!");
				   searched_ptimer[playerid] = 1;
				   GivePlayerGun(playerid, 22);
				   trashInfo[i][tAmount] --;
				   SetTimerEx("Timer_searched", 20000, 0, "i", playerid);
				   /*format(str, sizeof(str), "* %s searches through the trash bin and finds nothing inside.", PlayerICName);
			       ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);*/
			   }
			   else if(rand == 6)
			   {
				   SendClientMessage(playerid, COLOR_GREY, "You searched through the trash bin but found colt 45!");
				   searched_ptimer[playerid] = 1;
				   GivePlayerGun(playerid, 22);
				   SetTimerEx("Timer_searched", 20000, 0, "i", playerid);
				   /*format(str, sizeof(str), "* %s searches through the trash bin and finds nothing inside.", PlayerICName);
			       ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);*/
			   }
			   else if(rand == 7)
			   {
				   SendClientMessage(playerid, COLOR_GREY, "You searched through the trash bin and found sprunk!");
				   searched_ptimer[playerid] = 1;
				   PlayerInfo[playerid][pSprunk] += 4;
				   trashInfo[i][tAmount] --;
				   SetTimerEx("Timer_searched", 20000, 0, "i", playerid);
				   /*format(str, sizeof(str), "* %s searches through the trash bin and finds nothing inside.", PlayerICName);
			       ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);*/
			   }
			   else if(rand == 8)
			   {
				   SendClientMessage(playerid, COLOR_GREY, "You searched through the trash bin and found $5000!");
				   searched_ptimer[playerid] = 1;
				   GiveMoney(playerid, 5000);
				   SetTimerEx("Timer_searched", 20000, 0, "i", playerid);
				   /*format(str, sizeof(str), "* %s searches through the trash bin and finds nothing inside.", PlayerICName);
			       ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);*/
			   }
			   else if(rand == 9)
			   {
				   SendClientMessage(playerid, COLOR_GREY, "You searched through the trash bin and found cigars!");
				   searched_ptimer[playerid] = 1;
				   PlayerInfo[playerid][pCigars] += 4;
				   trashInfo[i][tAmount] --;
				   SetTimerEx("Timer_searched", 20000, 0, "i", playerid);
				   /*format(str, sizeof(str), "* %s searches through the trash bin and finds nothing inside.", PlayerICName);
			       ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);*/
			   }
			}
			else
			{
				return SendClientMessage(playerid, COLOR_GREY, "The bin is empty.");
			}
		}
	}
	return 1;
}