  /*
  _____ _                     _       _       ____          _____                                      _ 
 |  __ (_)                   | |     | |     |  _ \        |  __ \                                    | |
 | |__) | __________ _       | | ___ | |__   | |_) |_   _  | |  | | ___  ___ _ __ ___   ___  _ __   __| |
 |  ___/ |_  /_  / _` |  _   | |/ _ \| '_ \  |  _ <| | | | | |  | |/ _ \/ __| '_ ` _ \ / _ \| '_ \ / _` |
 | |   | |/ / / / (_| | | |__| | (_) | |_) | | |_) | |_| | | |__| |  __/\__ \ | | | | | (_) | | | | (_| |
 |_|   |_/___/___\__,_|  \____/ \___/|_.__/  |____/ \__, | |_____/ \___||___/_| |_| |_|\___/|_| |_|\__,_|
                                                     __/ |                                               
                                                    |___/                                               
*/


#define DIALOG_GETPIZZAJOB 16738

static housedeliveryid[MAX_PLAYERS];
static haspizza[MAX_PLAYERS];

new Float:RandomHouse[][3] =
{
	{2441.6282,-1356.9805,24.0000},
	{2441.7490,-1339.3866,24.0000},
	{2782.9695,-1925.3589,13.5469},
	{2753.0330,-1962.4550,13.5469},
	{2136.6016,-1229.8315,23.9766},
	{2192.3796,-1237.8867,23.9766},
	{2209.7964,-1237.3121,23.9263},
	{2144.9907,-1085.6957,24.7004},
	{2140.3267,-1083.1752,24.4701},
	{2109.4468,-1081.5615,24.8858},
	{2102.5120,-1074.7393,25.8022},
	{2104.7888,-1056.4807,27.1996},
	{1906.1940,-1116.4855,25.6641},
	{2230.0771,-1159.1823,25.8170},
	{2230.3896,-1395.5856,24.0000}

};

forward cancelpizzadelivery(playerid);
forward startpizzadelivery(playerid);
forward GetPlayerPizzaPay(playerid);
forward GetPlayerPizzaLevel(playerid);
forward IncreasePlayerPizzaSkill(playerid);
forward GetPlayerPizzaSkillLevel(playerid);

public cancelpizzadelivery(playerid)
{
	DisablePlayerCheckpointEx(playerid);
	CP[playerid] = CHECKPOINT_NONE;
	onpizzadelivery[playerid] = 0;
	HidePlayerProgressBar(playerid, pizzajobdeliveryid[playerid]);
	haspizza[playerid] = 0;
	return 1;

}
public startpizzadelivery(playerid)
{
	onpizzadelivery[playerid] = 1;
	housedeliveryid[playerid] = random(sizeof(RandomHouse));
	haspizza[playerid] = 0; // no longer need to check if the player has a pizza tray to place using /placepizza.
	SetPlayerProgressBarValue(playerid, pizzajobdeliveryid[playerid], 100.0);
	ShowPlayerProgressBar(playerid, pizzajobdeliveryid[playerid]);
	DisablePlayerCheckpointEx(playerid);
	SetPlayerCheckpointEx(playerid, RandomHouse[housedeliveryid[playerid]][0], RandomHouse[housedeliveryid[playerid]][1], RandomHouse[housedeliveryid[playerid]][2], 4.0);
	CP[playerid] = CHECKPOINT_PIZZAJOB;
	return 1;
}

public GetPlayerPizzaLevel(playerid)
{
	new skill = PlayerInfo[playerid][pPizzaSkill];
	if(skill >= 0 && skill <= 49) return 1;
	else if(skill >= 50 && skill <= 119) return 2;
	else if(skill >= 120 && skill <= 199) return 3;
	else if(skill >= 200 && skill <= 269) return 4;
	else if(skill >= 270) return 5;
	return 1;
}

public GetPlayerPizzaPay(playerid)
{
	new level = GetPlayerPizzaLevel(playerid);
	switch(level)
	{
		case 1: return 0;
		case 2: return 300;
		case 3: return 500;
		case 4: return 700;
		case 5: return 1000;
	}
	return 1;
}

public IncreasePlayerPizzaSkill(playerid)
{
	new skill = PlayerInfo[playerid][pPizzaSkill];

	if(skill+1 == 50)
	{
		SendClientMessage(playerid, COLOR_YELLOW, "* Your Pizza Skill is now Level 2, you will earn more money per delivery.");
	}
	else if(skill+1 == 120)
	{
		SendClientMessage(playerid, COLOR_YELLOW, "* Your Pizza Skill is now Level 3, you will earn more money per delivery.");
	}
	else if(skill+1 == 200)
	{
		SendClientMessage(playerid, COLOR_YELLOW, "* Your Pizza Skill is now Level 4, you will earn more money per delivery.");
	}
	else if(skill+1 == 270)
	{
		SendClientMessage(playerid, COLOR_YELLOW, "* Your Pizza Skill is now Level 5, you will earn more money per delivery.");
	}

	PlayerInfo[playerid][pPizzaSkill]++;
	return 1;
}

public GetPlayerPizzaSkillLevel(playerid)
{
	new skill = PlayerInfo[playerid][pPizzaSkill];
	new reqleft;
	if(skill >= 0 && skill <= 49) reqleft = 50 -skill;
	else if(skill >= 50 && skill <= 119) reqleft = 120 -skill;
	else if(skill >= 120 && skill <= 199) reqleft = 200 -skill;
	else if(skill >= 200 && skill <= 269) reqleft = 270 -skill;

	return reqleft;
}

Hook:PJob_OnGameModeInit()
{
	//CreateActor(155, 2104.9565, -1803.1753, 13.5547, 83.6858);
	//Create3DTextLabel("Pizza Delivery Job\n{FFFFFF}Press Y to become a pizza boy.", 0x43527DFF, 2104.9565, -1803.1753, 14.5247, 20.0, 0, 1);
	CreatePickup(1582, 1, 2104.7837,-1804.7845,13.5547, 0); // /getpizza pickup
	iPizza = CreateDynamicPickup(1239, 23, 2104.51, -1803.17, 13.55);
	new Text3D:textPizza = CreateDynamic3DTextLabel("Pizza Delivery Job\n{FFFFFF}/getjob", 0xFFFF91FF, 2104.51, -1803.17, 13.55 +0.88, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
	Create3DTextLabel("Pizza Delivery Job\n{FFFFFF}/getpizza", 0xFFFF91FF, 2104.7837,-1804.7845,13.5547 + 0.60, 20.0, 0, 1);

	#pragma unused textPizza
	return 1;
}

Hook:PJob_OnPlayerConnect(playerid)
{
	pizzajobdeliveryid[playerid] = CreatePlayerProgressBar(playerid,  290.0, 340.00, 65.0, 5.0, 0xFFFF00AA, 100.0, BAR_DIRECTION_RIGHT);
	return 1;
}

CMD:getpizza(playerid, params[])
{
	if(PlayerInfo[playerid][pCash] < 300) return SendClientMessage(playerid, COLOR_GREY, "You must have 300$ to get pizza.");
	if(PlayerInfo[playerid][pJob] != 21) return SendClientMessage(playerid, COLOR_GREY, "You must have the pizza job to use this command.");
	if(haspizza[playerid] == 1) return SendClientMessage(playerid, COLOR_GREY, "You already have a pizza! Place it in your pizza faggio using /placepizza.");
	if(!IsPlayerInRangeOfPoint(playerid, 4.0, 2104.7837,-1804.7845,13.5547)) return SendClientMessage(playerid, COLOR_GREY, "You must be by the pizza stack's entrance to do this.");
	haspizza[playerid] = 1;
	SendClientMessage(playerid, COLOR_YELLOW, "You were given a pizza tray ($300), place it in a pizza faggio (/placepizza) to continue the delivery.");
	GiveMoney(playerid, -300);
	return 1;
}
CMD:placepizza(playerid, params[])
{
	if(PlayerInfo[playerid][pJob] != 21) return SendClientMessage(playerid, COLOR_GREY, "You must have the pizza job to use this command.");
	if(onpizzadelivery[playerid] == 1) return SendClientMessage(playerid, COLOR_GREY, "You are already on a delivery.");
	new currentvehicleid = GetPlayerVehicleID(playerid);
	if(GetVehicleModel(currentvehicleid) != 448) return SendClientMessage(playerid, COLOR_GREY, "You must be in a pizza faggio to do this.");
	if(CP[playerid] != 0 && CP[playerid] != CHECKPOINT_PIZZAJOB) return SendClientMessage(playerid, COLOR_GREY, "You already have an active checkpoint, /killcp.");
	if(haspizza[playerid] != 1) return SendClientMessage(playerid, COLOR_GREY, "You must have a pizza tray to do this.");

	startpizzadelivery(playerid);

	SendClientMessage(playerid, COLOR_YELLOW, "You are now on a delivery. If you exit your faggio the delivery will be cancelled (/cpizzadelivery to cancel it manually).");
	SendClientMessage(playerid, COLOR_YELLOW, "A checkpoint has been set for you to find the location of the house you're going to deliver the pizza to.");

	return 1;
}

CMD:cpizzadelivery(playerid, params[])
{
	if(CP[playerid] == CHECKPOINT_PIZZAJOB && onpizzadelivery[playerid] == 1 && PlayerInfo[playerid][pJob] == 21)
	{
		cancelpizzadelivery(playerid);
		SendClientMessage(playerid, -1, "Your delivery has been cancelled.");
	}
	return 1;
}

Hook:PJB_OnPlayerEnterCheckpoint(playerid)
{
	if(CP[playerid] == CHECKPOINT_PIZZAJOB && onpizzadelivery[playerid] != 0)
	{
		// note - progress bar value is set to 100 when the delivery sets. every second it's minus 0.5, which means 2 seconds = 1 value, so if currentprogbarvalue is 90, it means 20 seconds passed and not 10.
		new Float:currentprogbarvalue = GetPlayerProgressBarValue(playerid, pizzajobdeliveryid[playerid]);
		new msg[80], randomdeliverymoney = GetPlayerPizzaPay(playerid); // bonus per level
		if(currentprogbarvalue < 70) // more than 60 seconds
		{
			randomdeliverymoney += random(600-300) + 300;
			format(msg, sizeof(msg), "You have delivered the pizza very late ($%i received).", randomdeliverymoney);
			SendClientMessage(playerid, COLOR_GREY, msg);
			GiveMoney(playerid, randomdeliverymoney);
		}
		else if(currentprogbarvalue >= 70 && currentprogbarvalue < 85) // done within 31-60 seconds
		{
			randomdeliverymoney += random(800-600) + 600;
			format(msg, sizeof(msg), "You have delivered the pizza in a respected time ($%i received).", randomdeliverymoney);
			SendClientMessage(playerid, COLOR_GREY, msg);
			GiveMoney(playerid, randomdeliverymoney);
		}
		else if(currentprogbarvalue >= 85 && currentprogbarvalue <= 100) // done within 1-30 seconds
		{
			randomdeliverymoney += random(1200-1000) + 1000;
			format(msg, sizeof(msg), "You have delivered the pizza in a very respected time (received: $%i).", randomdeliverymoney);
			SendClientMessage(playerid, COLOR_GREY, msg);
			GiveMoney(playerid, randomdeliverymoney);
		}
		else // somehow managed to get a number which is not defined within the if's above
		{
			randomdeliverymoney += random(500-300) + 300;
			format(msg, sizeof(msg), "You have delivered the pizza in an unknown time (received: $%i).", randomdeliverymoney);
			SendClientMessage(playerid, COLOR_GREY, msg);
			GiveMoney(playerid, randomdeliverymoney);
		}
		cancelpizzadelivery(playerid);
		IncreasePlayerPizzaSkill(playerid);
	}
	return 1;
}

Hook:PJob_OnPlayerExitVehicle(playerid, vehicleid)
{
	if(CP[playerid] == CHECKPOINT_PIZZAJOB && onpizzadelivery[playerid] == 1 && GetVehicleModel(vehicleid) == 448)
	{
		SendClientMessage(playerid, COLOR_GREY, "You have left your pizza faggio during a delivery and therefore you failed.");
		cancelpizzadelivery(playerid);
	}
	return 1;
}

Hook:PJob_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	if(GetVehicleModel(vehicleid) == 448 && PlayerInfo[playerid][pJob] != 21)
	{
		ClearAnimations(playerid);
		SendClientMessage(playerid, COLOR_GREY, "You must have the pizza deliver job to drive a pizza faggio.");
	}
	return 1;
}