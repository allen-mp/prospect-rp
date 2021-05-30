/*

	file:			jobs/mechanic_job.inc

	description:	The mechanic job for SARP.

	author:			Unknown

	improved by:	Jay Cortez

	date created:	30th March 2018

*/

// Mechanic price
#define MECH_HYDRAULICS_PRICE		500
#define MECH_REPAIR_PRICE			5000
#define MECH_NOS_PRICE				1000
#define MECH_NEON_PRICE				5000

// Cooldowns
#define MECH_HYDRAULICS_COOLDOWN	(30000)

// Neon colors
#define NEON_RED					18647
#define NEON_BLUE					18648
#define NEON_GREEN					18649
#define NEON_YELLOW					18650
#define NEON_PINK					18651
#define NEON_WHITE					18652

// Command to repair the vehicle
CMD:repair(playerid, params[])
	return cmd_fix(playerid, params);

CMD:fix(playerid, params[])
{
	if(PlayerInfo[playerid][pJob] != 7)
		return SendClientMessage(playerid, COLOR_GREY, "You are not a mechanic.");

	if(IsPlayerRestricted(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You are unable to do that at this time.");

	if(!CanRepair[playerid])
		return SendClientMessage(playerid, COLOR_GREY, "You must wait before repairing your vehicle again.");
	
	if(!IsPlayerInAnyVehicle(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You are not in any vehicle.");

	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
		return SendClientMessage(playerid, COLOR_GREY, "You are not the driver.");

	if(GetVehicleSpeed(GetPlayerVehicleID(playerid)) > 5)
		return SendClientMessage(playerid, COLOR_WHITE, "You can't do that while the car is moving!");

	if(PlayerInfo[playerid][pCash] < MECH_REPAIR_PRICE)
		return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to do this.");

	// Open hood of vehicle
	new engine, lights, alarm, doors, bonnet, boot, objective, Float:x, Float:y, Float:z;
	GetVehicleParamsEx(GetPlayerVehicleID(playerid), engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(GetPlayerVehicleID(playerid), engine, lights, alarm, doors, 1, boot, objective);
	
	// Set checkpoint at hood
	GetPosAheadVehicle(GetPlayerVehicleID(playerid), x, y, z);
	SetPlayerCheckpointEx(playerid, x, y, z, 1.5);

	// Set variables
	repairVeh[playerid] = GetPlayerVehicleID(playerid);
	isRepairing[playerid] = true;

	// Action
	new mechanicStr[128];
	format(mechanicStr, sizeof(mechanicStr), "* %s pops the bonnet of the vehicle open, then opens the door and steps out.", PlayerICName(playerid));
	SetPlayerChatBubble(playerid, mechanicStr, COLOR_PURPLE, 20.0, 15000);

	format(mechanicStr, sizeof(mechanicStr), "> %s pops the bonnet of the vehicle open, then opens the door and steps out.", PlayerICName(playerid));
	SendClientMessage(playerid, COLOR_PURPLE, mechanicStr);

	// Remove and inform player
	RemovePlayerFromVehicle(playerid);
	SendClientMessage(playerid, COLOR_YELLOW, "Make your way to the bonnet to finish the repair of the vehicle.");
	return 1;
}

Timer:RepairTimer(playerid) return CanRepair[playerid] = true;

// Called when the player enter the repair checkpoint
OnEnterRepairCheckpoint(playerid) {
	// Game text and sound
	GameTextForPlayer(playerid, "~r~Vehicle repair ~g~completed.", 3000, 5);
	PlayerPlaySound(playerid, 1133, 0, 0, 0);

	ImproveMechanicSkill(playerid, 1);

	// Determine repair cooldown
	new minutesToWait;

	switch(PlayerInfo[playerid][pMechSkill]) {
		case 0 .. 49:		minutesToWait = 5;
		case 50 .. 99:		minutesToWait = 4;
		case 100 .. 199:	minutesToWait = 3;
		case 200 .. 399:	minutesToWait = 2;
		default:			minutesToWait = 1;
	}

	SetTimerEx("Timer_RepairTimer", 60000 * minutesToWait, false, "i", playerid);

	// Repair the vehicle
	RepairVehicle(repairVeh[playerid]);
	SetVehicleHealth(repairVeh[playerid], 1000);

	// charge money
	GiveMoney(playerid, -MECH_REPAIR_PRICE); 

	switch(GetVehicleModel(repairVeh[playerid])){
		case 427, 432, 601:SetVehicleHealth(repairVeh[playerid], 10000); //Enforcer, Tank, Splashy
		case 470, 433, 596, 598:SetVehicleHealth(repairVeh[playerid], 1350); //Humvee, Barracks, SAPD-Cruisers
		case 425, 520, 528:SetVehicleHealth(repairVeh[playerid], 1500); //Hunter, Hydra, smaller SWAT Van
	}
	if(sVehicleInfo[repairVeh[playerid]][v_faction] == 6 && GetVehicleModel(sVehicleInfo[repairVeh[playerid]][v_id]) == 409)
		{
			SetVehicleHealth(sVehicleInfo[repairVeh[playerid]][v_id], 10000);
		}
	
	new mechanicStr[128];

	// Bicycle fix
	if(IsABicycle(repairVeh[playerid])) {
		SetVehicleEngineState(repairVeh[playerid], 0);
		format(mechanicStr, sizeof(mechanicStr), "* %s fixes the bike's chain and tests it out.", PlayerICName(playerid));
		SetPlayerChatBubble(playerid, mechanicStr, COLOR_PURPLE, 20.0, 15000);

		format(mechanicStr, sizeof(mechanicStr), "> %s fixes the bike's chain and tests it out.", PlayerICName(playerid));
		SendClientMessage(playerid, COLOR_PURPLE, mechanicStr);
	}
	else {
		format(mechanicStr, sizeof(mechanicStr), "* %s moves their hands around inside of the engine bay, attempting to fix the vehicle.", PlayerICName(playerid));
		SetPlayerChatBubble(playerid, mechanicStr, COLOR_PURPLE, 20.0, 15000);

		format(mechanicStr, sizeof(mechanicStr), "> %s moves their hands around inside of the engine bay, attempting to fix the vehicle.", PlayerICName(playerid));
		SendClientMessage(playerid, COLOR_PURPLE, mechanicStr);
	}

	// Close the bonnet
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(repairVeh[playerid], engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(repairVeh[playerid], engine, lights, alarm, doors, 0, boot, objective);
	
	// Reset variables and remove checkpoint
	repairVeh[playerid] = INVALID_VEHICLE_ID;
	isRepairing[playerid] = false;
	CanRepair[playerid] = false;
	DisablePlayerCheckpointEx(playerid);
	return 1;
}

CMD:nos(playerid)
{
	if(PlayerInfo[playerid][pJob] != 7) return SendClientMessage(playerid, COLOR_GREY, "You are not a mechanic.");
	if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can use this command only when in vehicle.");
	if(PlayerInfo[playerid][pCash] < MECH_NOS_PRICE)
		return SendClientMessage(playerid, COLOR_GREY, "You cant afford the nos.");

	new vehicleid = GetPlayerVehicleID(playerid);

	if(!IsValidNosVehicle(vehicleid))
		return SendClientMessage(playerid, COLOR_GREY, "This vehicle does not support nos.");

	new Float:x, Float:y, Float:z;

	new vehspeed = GetVehicleSpeed(vehicleid);

	if(vehspeed > 5) return SendClientMessage(playerid, COLOR_GREY, "Please stop your vehicle in order to add nitro.");

	GetPosAheadVehicle(GetPlayerVehicleID(playerid), x, y, z);
	SetPlayerCheckpointEx(playerid, x, y, z, 1.5);

	// Open hood of vehicle
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(GetPlayerVehicleID(playerid), engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(GetPlayerVehicleID(playerid), engine, lights, alarm, doors, 1, boot, objective);

	RemovePlayerFromVehicle(playerid);
	SendClientMessage(playerid, COLOR_GREY, "Make your way to the hood and add nitro to your vehicle.");

	nosVehicle[playerid] = vehicleid;
	isAddingNos[playerid] = true;

	return 1;
}

OnEnterNosCheckpoint(playerid) {
	// Game text and sound
	GameTextForPlayer(playerid, "~r~Vehicle NOS addition ~g~completed.", 3000, 5);
	PlayerPlaySound(playerid, 1133, 0, 0, 0);

	// Add nos
	AddVehicleComponent(nosVehicle[playerid], 1009);	
	
	new mechanicStr[128];

	format(mechanicStr, sizeof(mechanicStr), "* %s moves their hands around inside of the engine bay, attempting to add nos to the vehicle.", PlayerICName(playerid));
	SetPlayerChatBubble(playerid, mechanicStr, COLOR_PURPLE, 20.0, 15000);

	format(mechanicStr, sizeof(mechanicStr), "> %s moves their hands around inside of the engine bay, attempting to add nos to the vehicle.", PlayerICName(playerid));
	SendClientMessage(playerid, COLOR_PURPLE, mechanicStr);

	// Close the bonnet
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(nosVehicle[playerid], engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(nosVehicle[playerid], engine, lights, alarm, doors, 0, boot, objective);
	
	// Reset variables and remove checkpoint
	nosVehicle[playerid] = INVALID_VEHICLE_ID;
	isAddingNos[playerid] = false;
	DisablePlayerCheckpointEx(playerid);
	return 1;
}

// Command to apply hydraulics to vehicle
CMD:hyd(playerid, params[])
	return cmd_hydraulics(playerid, params);

CMD:hydraulics(playerid, params[]) {
	if(PlayerInfo[playerid][pJob] != 7)
		return SendClientMessage(playerid, COLOR_GREY, "You are not a mechanic.");

	if(UseHYDTimer[playerid])
		return SendClientMessage(playerid, COLOR_GREY, "You must wait before doing that again.");

	if(!IsPlayerInAnyVehicle(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You are not in a vehicle.");

	if(PlayerInfo[playerid][pCash] < MECH_HYDRAULICS_PRICE)
		return SendClientMessage(playerid, COLOR_GREY, "You cant afford the hydraulics.");

	GiveMoney(playerid, -MECH_HYDRAULICS_PRICE);

	// Add hydraulics to vehicle
	AddVehicleComponent(GetPlayerVehicleID(playerid), 1087);

	// Play sound
	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);

	// Action
	new mechanicStr[128];

	format(mechanicStr, sizeof(mechanicStr), "* %s has modified the vehicle with hydraulics.", PlayerICName(playerid));
	SetPlayerChatBubble(playerid, mechanicStr, COLOR_PURPLE, 20.0, 15000);

	format(mechanicStr, sizeof(mechanicStr), "> %s has modified the vehicle with hydraulics.", PlayerICName(playerid));
	SendClientMessage(playerid, COLOR_PURPLE, mechanicStr);
	
	// Set the hydraulics timer
	UseHYDTimer[playerid] = 1;
	SetTimerEx("Timer_UseHYD", MECH_HYDRAULICS_COOLDOWN, false, "i", playerid);
	return 1;
}

Timer:UseHYD(playerid)
{
	if(!IsPlayerConnected(playerid)) return 0;
	if(UseHYDTimer[playerid])
	{
		UseHYDTimer[playerid] = 0;
	}
	return 1;
}

// Command to refill a player's vehicle
CMD:refill(playerid, params[]) {
	if(PlayerInfo[playerid][pJob] != 7)
		return SendClientMessage(playerid, COLOR_GREY, "You are not a mechanic.");

	if(PlayerInfo[playerid][pMechTime])
		return SendClientMessage(playerid, COLOR_GREY, "You must wait before you can offer another refill.");
	
	new target, money;
	if(sscanf(params, "ui", target, money))
		return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /refill [playerid/PartOfName] [price]");

	if(!IsPlayerConnected(target))
		return SendClientMessage(playerid, COLOR_GREY, "Unknown player.");

	if(target == playerid)
		return SendClientMessage(playerid, COLOR_GREY, "You cannot offer to refill your own vehicle.");

	if(money < 1000 || money > 5000)
		return SendClientMessage(playerid, COLOR_GREY, "The price cannot be lower then $1,000 or above $5,000.");
	
	if(!IsPlayerInRangeOfPlayer(8.0, playerid, target))
		return SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");

	if(!IsPlayerInAnyVehicle(target))
		return SendClientMessage(playerid, COLOR_GREY, "That player is not in a vehicle.");

	new mechanicStr[128];

	format(mechanicStr, sizeof(mechanicStr), "* You offered %s to refill their car for $%i .", PlayerICName(target), money);
	SendClientMessage(playerid, COLOR_LIGHTBLUE, mechanicStr);

	format(mechanicStr, sizeof(mechanicStr), "* Mechanic %s wants to refill your car for $%i. Type [/accept refill] to accept.", PlayerICName(playerid), money);
	SendClientMessage(target, COLOR_LIGHTBLUE, mechanicStr);

	// Set variables
	PlayerInfo[playerid][pMechTime] = 60;
	RefillOffer[target] = playerid;
	RefillPrice[target] = money;
	return 1;
}

// Called when a refill request is accepted
OnRefillAccept(playerid) {
	new refiller = RefillOffer[playerid];

	if(refiller == INVALID_PLAYER_ID)
		return SendClientMessage(playerid, COLOR_GREY, "Nobody offered to refill your vehicle.");
	
	if(!IsPlayerConnected(refiller)) {
		RefillOffer[playerid] = INVALID_PLAYER_ID;
		return SendClientMessage(playerid, COLOR_GREY, "The player that offered you a refill has disconnected.");
	}

	if(!IsPlayerInAnyVehicle(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You are not in any vehicle.");

	new refillPrice = RefillPrice[playerid];

	if(PlayerInfo[playerid][pCash] < refillPrice)
		return SendClientMessage(playerid, COLOR_GREY, "You cannot afford the refill.");

	if(UseAcceptTimer[playerid])
		return SendClientMessage(playerid, COLOR_GREY, "You must wait before doing that.");

	new vehid = GetPlayerVehicleID(playerid),
		fuelToAdd = 0;

	// Improve the refiller's mechanic skill by 1
	ImproveMechanicSkill(refiller, 1);
	
	// Determine the fuel to add
	switch(PlayerInfo[refiller][pMechSkill]) {
		case 0 .. 49:		fuelToAdd = 15;
		case 50 .. 99:		fuelToAdd = 40;
		case 100 .. 199:	fuelToAdd = 60;
		case 200 .. 399:	fuelToAdd = 80;
		default:			fuelToAdd = 100;
	}

	// Send messages
	new mechanicStr[128];

	format(mechanicStr, sizeof(mechanicStr), "* You refilled your vehicle with %i, for $%i by mechanic %s.", fuelToAdd, refillPrice, PlayerICName(refiller));
	SendClientMessage(playerid, COLOR_LIGHTBLUE, mechanicStr);

	format(mechanicStr, sizeof(mechanicStr), "* You refilled %s's vehicle with %i, the $%i has been added to your bank account.", PlayerICName(playerid), fuelToAdd, refillPrice);
	SendClientMessage(RefillOffer[playerid], COLOR_LIGHTBLUE, mechanicStr);

	// Take and add money
	PlayerInfo[refiller][pAccount] += refillPrice;
	GiveMoney(playerid, -refillPrice);

	// Set the fuel
	if(Gas[vehid] < 110)
		Gas[vehid] += fuelToAdd;

	// Reset refill offer
	RefillOffer[playerid] = INVALID_PLAYER_ID;
	RefillPrice[playerid] = 0;

	// Set accept timer
	UseAcceptTimer[playerid] = 1;
	SetTimerEx("Timer_UseAccept", 60 *1000, 0, "i", playerid);
	return 1;
}

// Command to add neon strips to a vehicle
CMD:neon(playerid, params[]) {

	if(PlayerInfo[playerid][pJob] != 7)
		return SendClientMessage(playerid, COLOR_GREY, "You are not a mechanic.");

	if(!IsPlayerInAnyVehicle(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You are not in any vehicle.");

	if(PlayerInfo[playerid][pCash] < MECH_NEON_PRICE)
		return SendClientMessage(playerid, COLOR_GREY, "You cannot afford the neon.");

	new vehid = GetPlayerVehicleID(playerid);
	if(IsInvalidNeonVehicle(vehid))
		return SendClientMessage(playerid, COLOR_GREY, "You cannot add neon to this vehicle.");

	ShowPlayerDialog(playerid, 101, DIALOG_STYLE_LIST, "Neon",
		"Red\n\
		Blue\n\
		Green\n\
		Yellow\n\
		Pink\n\
		White",
	"Mod", "Cancel");
	return 1;
}

// Called when a player has selected neon from the dialog
Dialog_NeonResponse(playerid, listitem) {
	// Set the neon
	new vehid = GetPlayerVehicleID(playerid);

	switch(listitem)
	{
		case 0: AddVehicleNeon(vehid, NEON_RED);
		case 1: AddVehicleNeon(vehid, NEON_BLUE);
		case 2: AddVehicleNeon(vehid, NEON_GREEN);
		case 3: AddVehicleNeon(vehid, NEON_YELLOW);
		case 4: AddVehicleNeon(vehid, NEON_PINK);
		case 5: AddVehicleNeon(vehid, NEON_WHITE);
	}

	// Take the money
	GiveMoney(playerid, -MECH_NEON_PRICE);

	// Play sound
	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);

	// Action
	new mechanicStr[128];

	format(mechanicStr, sizeof(mechanicStr), "* %s modifies the vehicle with neon.", PlayerICName(playerid));
	SetPlayerChatBubble(playerid, mechanicStr, COLOR_PURPLE, 20.0, 15000);

	format(mechanicStr, sizeof(mechanicStr), "> %s modifies the vehicle with neon.", PlayerICName(playerid));
	SendClientMessage(playerid, COLOR_PURPLE, mechanicStr);
	return 1;
}

// Adds neon to a vehicle
AddVehicleNeon(vehicleid, neon)
{
	// Destroy current neons
	if(Neon[vehicleid])		DestroyDynamicObjectEx(Neon[vehicleid]);
	if(Neon2[vehicleid])	DestroyDynamicObjectEx(Neon2[vehicleid]);
	if(Neon3[vehicleid])	DestroyDynamicObjectEx(Neon3[vehicleid]);
	if(Neon4[vehicleid])	DestroyDynamicObjectEx(Neon4[vehicleid]);

	Neon[vehicleid] = CreateDynamicObject(neon, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	Neon2[vehicleid] = CreateDynamicObject(neon, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	if(GetVehicleModel(vehicleid) == 409)
	{
		Neon3[vehicleid] = CreateDynamicObject(neon, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		Neon4[vehicleid] = CreateDynamicObject(neon, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	}
	switch(GetVehicleModel(vehicleid))
	{
		case 400:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.9, 0.0, -0.8, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.9, 0.0, -0.8, 0.0, 0.0, 0.0);
		}
		case 401:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.9, 0.0, -0.6, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.9, 0.0, -0.6, 0.0, 0.0, 0.0);
		}
		case 402:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.92, 0.0, -0.7, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.92, 0.0, -0.7, 0.0, 0.0, 0.0);
		}
		case 404, 466, 467:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 0.0, -0.55, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 0.0, -0.55, 0.0, 0.0, 0.0);
		}
		case 405, 442:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 0.0, -0.7, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 0.0, -0.7, 0.0, 0.0, 0.0);
		}
		case 409:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 1.0, -0.6, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 1.0, -0.6, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon3[vehicleid], vehicleid, 0.8, -1.0, -0.6, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon4[vehicleid], vehicleid, -0.8, -1.0, -0.6, 0.0, 0.0, 0.0);
		}
		case 410, 560:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 0.0, -0.5, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 0.0, -0.5, 0.0, 0.0, 0.0);
		}
		case 411, 412, 415, 419, 458, 474:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 0.0, -0.65, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 0.0, -0.65, 0.0, 0.0, 0.0);
		}
		case 413:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 0.0, -0.85, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 0.0, -0.85, 0.0, 0.0, 0.0);
		}
		case 418:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.9, 0.0, -0.95, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.9, 0.0, -0.95, 0.0, 0.0, 0.0);
		}
		case 420, 426, 429, 436:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.9, 0.0, -0.55, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.9, 0.0, -0.55, 0.0, 0.0, 0.0);
		}
		case 421, 422:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.9, 0.0, -0.7, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.9, 0.0, -0.7, 0.0, 0.0, 0.0);
		}
		case 424:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.85, 0.0, -0.5, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.85, 0.0, -0.5, 0.0, 0.0, 0.0);
		}
		case 434:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.6, 0.0, -0.75, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.6, 0.0, -0.75, 0.0, 0.0, 0.0);
		}
		case 438, 439:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.9, 0.0, -0.75, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.9, 0.0, -0.75, 0.0, 0.0, 0.0);
		}
		case 440:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 0.0, -1.0, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 0.0, -1.0, 0.0, 0.0, 0.0);
		}
		case 444:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.9, 0.0, 0.0, 0.0, 0.0, 0.0);
		}
		case 445, 451, 475:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 0.0, -0.6, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 0.0, -0.6, 0.0, 0.0, 0.0);
		}
		case 457:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.55, 0.0, -0.4, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.55, 0.0, -0.4, 0.0, 0.0, 0.0);
		}
		case 459:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 0.0, -0.8, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 0.0, -0.8, 0.0, 0.0, 0.0);
		}
		case 477, 480, 579, 580:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 0.0, -0.6, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 0.0, -0.6, 0.0, 0.0, 0.0);
		}
		case 479, 541, 559, 565, 576, 598:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 0.0, -0.5, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 0.0, -0.5, 0.0, 0.0, 0.0);
		}
		case 491, 534..536, 545, 587:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 0.0, -0.65, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 0.0, -0.65, 0.0, 0.0, 0.0);
		}
		case 494, 502, 503, 540, 603:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 0.0, -0.7, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 0.0, -0.7, 0.0, 0.0, 0.0);
		}
		case 495:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 0.0, -0.9, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 0.0, -0.9, 0.0, 0.0, 0.0);
		}
		case 496, 518, 533, 566:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 0.0, -0.55, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 0.0, -0.55, 0.0, 0.0, 0.0);
		}
		case 500:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 0.2, -0.6, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 0.2, -0.6, 0.0, 0.0, 0.0);
		}
		case 506:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, -0.2, -0.55, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, -0.2, -0.55, 0.0, 0.0, 0.0);
		}
		case 527:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 0.2, -0.5, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 0.2, -0.5, 0.0, 0.0, 0.0);
		}
		case 550:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, -0.2, -0.7, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, -0.2, -0.7, 0.0, 0.0, 0.0);
		}
		case 558, 589:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 0.0, -0.45, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 0.0, -0.45, 0.0, 0.0, 0.0);
		}
		case 562:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 0.1, -0.5, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 0.1, -0.5, 0.0, 0.0, 0.0);
		}
		case 567:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 0.2, -0.7, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 0.2, -0.7, 0.0, 0.0, 0.0);
		}
		case 575:
		{
			AttachDynamicObjectToVehicle(Neon[vehicleid], vehicleid, 0.8, 0.0, -0.4, 0.0, 0.0, 0.0);
			AttachDynamicObjectToVehicle(Neon2[vehicleid], vehicleid, -0.8, 0.0, -0.4, 0.0, 0.0, 0.0);
		}
	}
	return 1;
}

// This function adds to the mechanic skill of a player
ImproveMechanicSkill(playerid, amount) {
	PlayerInfo[playerid][pMechSkill] += amount;

	// Send notification if new level reached
	if(PlayerInfo[playerid][pMechSkill] == 50)
		SendClientMessage(playerid, COLOR_YELLOW, "* Your Mechanic Skill is now Level 2, you can now add more Fuel to Cars.");
	else if(PlayerInfo[playerid][pMechSkill] == 100)
		SendClientMessage(playerid, COLOR_YELLOW, "* Your Mechanic Skill is now Level 3, you can now add more Fuel to Cars.");
	else if(PlayerInfo[playerid][pMechSkill] == 200)
		SendClientMessage(playerid, COLOR_YELLOW, "* Your Mechanic Skill is now Level 4, you can now add more Fuel to Cars.");
	else if(PlayerInfo[playerid][pMechSkill] == 400)
		SendClientMessage(playerid, COLOR_YELLOW, "* Your Mechanic Skill is now Level 5, you can now add more Fuel to Cars.");

	return 1;
}