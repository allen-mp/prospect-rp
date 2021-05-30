new MechanicTrucks[4], bool:mechanicDuty[MAX_PLAYERS];

hook OnGameModeInit()
{
    MechanicTrucks[0] = CreateVehicle(525, 2352.6565, -1995.5439, 13.3312, 0.0000, 	 -1, -1, 100);
    MechanicTrucks[1] = CreateVehicle(525, 2389.5193, -2016.0085, 13.3612, 270.0000, -1, -1, 100);
    MechanicTrucks[2] = CreateVehicle(525, 2400.4277, -2016.0823, 13.3612, 270.0000, -1, -1, 100);
	return 1;
}

hook OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	if(vehicleid == MechanicTrucks[0] || vehicleid == MechanicTrucks[1] || vehicleid == MechanicTrucks[2])
	{
		if(!ispassenger)
		{
			if(PlayerInfo[playerid][pJob] != 7)
			{
				displayCenterHUDInfo(playerid, "You do not have keys for this vehicle.", 8);

				new Float:pos[4];
				GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
				SetPlayerPos(playerid, pos[0], pos[1], pos[2]);
			}
		}
	}
	return 1;
}

#define MECHANIC_SPOT 2347.5835, -1997.2177, 13.3754

new bool:NeedsMechanic[MAX_PLAYERS], HandlingRequest[MAX_PLAYERS];

CMD:mechanicduty(playerid, params[])
{
	if(PlayerInfo[playerid][pJob] != 7)
		return SendClientMessage(playerid, COLOR_RED, "You're not a mechanic!");

	if(mechanicDuty[playerid] == false)
	{
		SendClientMessage(playerid, COLOR_BLUE, "NOTICE: {FFFFFF}You've went on mechanic duty!");
		mechanicDuty[playerid] = true;
	} else {
		SendClientMessage(playerid, COLOR_BLUE, "NOTICE: {FFFFFF}You've went off mechanic duty!");
		mechanicDuty[playerid] = false;
	}
	return 1;
}

cmdmech_requestmechanic(playerid)
{
	new mechString[124];
	format(mechString, sizeof(mechString), "Player %s is in need of a mechanic! (/acceptrequest [%i])", PlayerOOCName(playerid), playerid);
	foreach(new i: Player)
	{
		if(PlayerInfo[i][pJob] == 7 || mechanicDuty[playerid] == true)
		{
			SendClientMessage(i, COLOR_LIGHTBLUE, mechString);
		}
	}
	NeedsMechanic[playerid] = true;
	SendClientMessage(playerid, COLOR_LIGHTBLUE, "You've requested a mechanic be patient.");
	return 1;
}
// else if(strcmp(x_job, "mechanic", true) == 0) return SendClientMessage(playerid, COLOR_GREY, "Use /acceptrequest");
/*

		if(strcmp(x_nr, "mechanic", false) == 0)
			return cmdmech_requestmechanic(playerid);
*/
CMD:acceptrequest(playerid, params[])
{
	new userID;
	if(sscanf(params, "i", userID))
		return SendClientMessage(playerid, COLOR_BLUE, "USAGE: {FFFFFF}/acceptrequest [Playerid/PartOfName]");

	if(NeedsMechanic[userID] == true)
	{
		HandlingRequest[playerid] = userID;
		NeedsMechanic[userID] = false;
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "You've accepted the mechanic request!");
	} else {
		SendClientMessage(playerid, COLOR_GREY, "This user doesn't need a mechanic!");
	}
	return 1;
}

CMD:repaircar(playerid, params[])
{
	new userID;
	if(sscanf(params, "i", userID))
		return SendClientMessage(playerid, COLOR_BLUE, "USAGE: {FFFFFF}/repaircar [Playerid/PartOfName]");

	if(HandlingRequest[playerid] != userID)
		return SendClientMessage(playerid, COLOR_GREY, "You're not handling this persons request!");

	if(!IsPlayerInRangeOfPoint(playerid, 15, MECHANIC_SPOT))
		return SendClientMessage(playerid, COLOR_GREY, "You're not near the mechanic spot!");

	if(!IsPlayerInRangeOfPoint(userID, 15, MECHANIC_SPOT))
		return SendClientMessage(playerid, COLOR_GREY, "The person specified is not near the mechanic spot!");

	if(!IsPlayerInAnyVehicle(userID))
		return SendClientMessage(playerid, COLOR_GREY, "The person specified is not in a vehicle!");

	if(GetPlayerMoney(userID) < 15000)
		return SendClientMessage(playerid, COLOR_GREY, "The person specified does not have $15,000!");

	new userVehicle = GetPlayerVehicleID(userID);
	RepairVehicle(userVehicle);

	SendClientMessage(playerid, COLOR_BLUE, "Mechanic: {ffffff}You've made $10,000 for repairing the vehicle!");
	SendClientMessage(userID, COLOR_BLUE, "Mechanic: {ffffff}You've paid $10,000 for repairing your vehicle!");

	GiveMoney(playerid, 15000);
	GiveMoney(userID, -15000);

	return 1;
}