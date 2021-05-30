new bool:AlreadyRefunded[ MAX_PLAYERS ];
CMD:refundme( playerid )
{
	if( AlreadyRefunded[ playerid ] == true ) SendClientMessage( playerid, COLOR_GREY, "You're already refunded, what are you trying to do, mhm..?" );
	else
	{
		PlayerInfo[playerid][pLevel] = PlayerInfo[playerid][pLevel]+3;
		GiveMoney(playerid, 2500000);
		PlayerInfo[playerid][pDonateRank] = 2;
		format(str, 32, "%02d/%02d/%d", Month, Day, Year);
		strmid(PlayerInfo[giveplayerid][pVIPJoinDate], str, 0, strlen(str), 255);
		format(str, 32, "%02d/%02d/%d", Month +1, Day, Year);
		strmid(PlayerInfo[giveplayerid][pVIPExpDate], str, 0, strlen(str), 255);
		if(Month == 12)
		{
			format(PlayerInfo[giveplayerid][pVIPExpDate], 32, "%02d/%02d/%d", 1, Day, Year +1);
		}
		format(str, sizeof(str), "Join Date: %s, Expire Date: %s", PlayerInfo[giveplayerid][pVIPJoinDate], PlayerInfo[giveplayerid][pVIPExpDate]);
		SendClientMessage(giveplayerid, COLOR_GRAD2, str);
	}
	return 1;
}