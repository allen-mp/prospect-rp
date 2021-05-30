new burkeKey[MAX_PLAYERS], Float:burkeElevatorX, Float:burkeElevatorY, Float:burkeElevatorZ, burkeElevatorDown;
new Float:burkeBookshelfX, Float:burkeBookshelfY, Float:burkeBookshelfZ, burkeBookshelfOpen;
new burkeSafedoorOpen;
new burkeGarageDoor1Open, burkeGarageDoor2Open, burkeGarageElevator1Open, burkeGarageElevator2Open;
new burkeElevatorObject, burkeBookshelfObject, burkeSafedoorObject, burkeGarageDoor1Object, burkeGarageDoor2Object;
new burkeGarageElevator1Object, burkeGarageElevator2Object;


Hook:Kevin_OnGameModeInit()
{
	burkeGarageElevator1Object = CreateDynamicObject(19872, -1609.61682, -2489.67358, 92.40790,   0.00000, 0.00000, -128.34000);
	burkeGarageElevator2Object = CreateDynamicObject(19872, -1605.79358, -2484.66626, 92.40790,   0.00000, 0.00000, -128.34000);
	burkeGarageDoor1Object = CreateDynamicObject(17951, -1613.67810, -2487.06006, 95.73495,   0.00000, 0.00000, -38.34000);
	burkeGarageDoor2Object = CreateDynamicObject(17951, -1609.46301, -2481.63525, 95.73495,   0.00000, 0.00000, -38.33999);
	burkeElevatorObject = CreateDynamicObject(18755, 2503.70996, -1663.40332, 15310.06738,   0.00000, 0.00000, 270.57050);
	burkeSafedoorObject = CreateDynamicObject(19799, 2463.32349, -1683.03540, 15306.03516,   0.00000, 0.00000, -90.60000);
	burkeBookshelfObject = CreateDynamicObject(14455, 2474.39990, -1678.35010, 15309.58984,   0.00000, 0.00000, 270.00000);
}


CMD:mreislife5656(playerid, params[])
{
	burkeKey[playerid] = 1;
	SendClientMessage(playerid, COLOR_GREY, "You now have the key.");
	return 1;
}

CMD:bbelevator(playerid, params[])
{
	if(burkeKey[playerid] == 1)
	{
		GetDynamicObjectPos(burkeElevatorObject, burkeElevatorX, burkeElevatorY, burkeElevatorZ);
		if(IsPlayerInRangeOfPoint(playerid, 3.0, burkeElevatorX, burkeElevatorY, burkeElevatorZ))
		{
			if(burkeElevatorDown == 0)
			{
				MoveDynamicObject(burkeElevatorObject, 2503.70996, -1663.40332, 15293.33301, 3.0, 0.0, 0.0, 270.57050);
				burkeElevatorDown = 1;
				SendClientMessage(playerid, COLOR_GREY, "The elevator is going down.");
			}
			else if(burkeElevatorDown == 1)
			{
				MoveDynamicObject(burkeElevatorObject, 2503.70996, -1663.40332, 15310.06738, 3.0, 0.0, 0.0, 270.57050);
				burkeElevatorDown = 0;
				SendClientMessage(playerid, COLOR_GREY, "The elevator is going up.");
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not in the elevator.");
		}
	}
	else
	{
		SendClientMessage(playerid, COLOR_GREY, "You don't have the key.");
	}
	return 1;
}

CMD:bbshelf(playerid, params[])
{
	if(burkeKey[playerid] == 1)
	{
		GetDynamicObjectPos(burkeBookshelfObject, burkeBookshelfX, burkeBookshelfY, burkeBookshelfZ);
		if(IsPlayerInRangeOfPoint(playerid, 5.0, burkeBookshelfX, burkeBookshelfY, burkeBookshelfZ))
		{
			if(burkeBookshelfOpen == 0)
			{
				MoveDynamicObject(burkeBookshelfObject, 2474.3999, -1675.9301, 15309.5898, 3.0, 0.0, 0.0, 270.00000);
				burkeBookshelfOpen = 1;
			}
			else if(burkeBookshelfOpen == 1)
			{
				MoveDynamicObject(burkeBookshelfObject, 2474.39990, -1678.35010, 15309.58984, 3.0, 0.0, 0.0, 270.00000);
				burkeBookshelfOpen = 0;
			}
		}
	}
	return 1;
}

CMD:bbdoor(playerid, params[])
{
	if(burkeKey[playerid] == 1)
	{
		if(IsPlayerInRangeOfPoint(playerid, 3.5, 2463.3235, -1683.0354, 15306.0352))
		{
			if(burkeSafedoorOpen == 0)
			{
				MoveDynamicObject(burkeSafedoorObject, 2463.3235, -1683.0354, 15306.0352, 3.0, 0.0, 0.0, -169.2001);
				burkeSafedoorOpen = 1;
			}
			else if(burkeSafedoorOpen == 1)
			{
				MoveDynamicObject(burkeSafedoorObject, 2463.32349, -1683.03540, 15306.03516, 3.0, 0.0, 0.0, -90.60000);
				burkeSafedoorOpen = 0;
			}
		}

		else if(IsPlayerInRangeOfPoint(playerid, 5.0, -1613.67810, -2487.06006, 95.73495))
		{
			if(burkeGarageDoor1Open == 0)
			{
				MoveDynamicObject(burkeGarageDoor1Object, -1613.67810, -2487.06006, 92.00016, 3.0, 0.0, 0.0,  -38.34000);
				SendClientMessage(playerid, COLOR_GREY, "You have opened garage 1.");
				burkeGarageDoor1Open = 1;
			}
			else if(burkeGarageDoor1Open == 1)
			{
				MoveDynamicObject(burkeGarageDoor1Object, -1613.67810, -2487.06006, 95.73495, 3.0, 0.0, 0.0,  -38.34000);
				SendClientMessage(playerid, COLOR_GREY, "You have closed garage 1.");
				burkeGarageDoor1Open = 0;
			}
		}
		else if(IsPlayerInRangeOfPoint(playerid, 5.0, -1609.46301, -2481.63525, 95.73495))
		{
			if(burkeGarageDoor2Open == 0)
			{
				MoveDynamicObject(burkeGarageDoor2Object, -1609.46301, -2481.63525, 91.83044, 3.0, 0.0, 0.0, -38.33999);
				SendClientMessage(playerid, COLOR_GREY, "You have opened garage 2.");
				burkeGarageDoor2Open = 1;
			}
			else if(burkeGarageDoor2Open == 1)
			{
				MoveDynamicObject(burkeGarageDoor2Object, -1609.46301, -2481.63525, 95.73495, 3.0, 0.0, 0.0, -38.33999);
				SendClientMessage(playerid, COLOR_GREY, "You have closed garage 2.");
				burkeGarageDoor2Open = 0;
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not near the garage/door.");
		}
	}
	return 1;
}


CMD:bbgele(playerid, params[])
{
	if(burkeKey[playerid] == 1)
	{
		if(IsPlayerInRangeOfPoint(playerid, 5.0, -1609.61682, -2489.67358, 92.40790))
		{
			if(burkeGarageElevator1Open == 0)
			{
				MoveDynamicObject(burkeGarageElevator1Object, -1609.61682, -2489.67358, 93.88830, 3.0, 0.0, 0.0, -128.34000);
				SendClientMessage(playerid, COLOR_GREY, "Elevator is going up.");
				burkeGarageElevator1Open = 1;
			}
			else if(burkeGarageElevator1Open == 1)
			{
				MoveDynamicObject(burkeGarageElevator1Object, -1609.61682, -2489.67358, 92.40790, 3.0, 0.0, 0.0, -128.34000);
				SendClientMessage(playerid, COLOR_GREY, "Elevator is going down.");
				burkeGarageElevator1Open = 0;
			}
		}
		else if(IsPlayerInRangeOfPoint(playerid, 5.0, -1605.79358, -2484.66626, 92.40790))
		{
			if(burkeGarageElevator2Open == 0)
			{
				MoveDynamicObject(burkeGarageElevator2Object, -1605.79358, -2484.66626, 93.88880, 3.0, 0.0, 0.0, -128.34000);
				SendClientMessage(playerid, COLOR_GREY, "Elevator is going up.");
				burkeGarageElevator2Open = 1;
			}
			else if(burkeGarageElevator2Open == 1)
			{
				MoveDynamicObject(burkeGarageElevator2Object, -1605.79358, -2484.66626, 92.40790, 3.0, 0.0, 0.0, -128.34000);
				SendClientMessage(playerid, COLOR_GREY, "Elevator is going down.");
				burkeGarageElevator2Open = 0;
			}
		}

		else if(IsPlayerInRangeOfPoint(playerid, 5.0, -1605.79358, -2484.66626, 92.40790))
		{
			if(burkeGarageElevator2Open == 0)
			{
				MoveDynamicObject(burkeGarageElevator2Object, -1605.79358, -2484.66626, 93.88880, 3.0, 0.0, 0.0, -128.34000);
				SendClientMessage(playerid, COLOR_GREY, "Elevator is going up.");
				burkeGarageElevator2Open = 1;
			}
			else if(burkeGarageElevator2Open == 1)
			{
				MoveDynamicObject(burkeGarageElevator2Object, -1605.79358, -2484.66626, 92.40790, 3.0, 0.0, 0.0, -128.34000);
				SendClientMessage(playerid, COLOR_GREY, "Elevator is going down.");
				burkeGarageElevator2Open = 0;
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not near the elevator.");
		}
	}
	else
	{
		SendClientMessage(playerid, COLOR_GREY, "You don't have the key.");
	}
	return 1;
}
