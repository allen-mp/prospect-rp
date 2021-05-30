/*
 __  __            __             
/\ \/\ \          /\ \__          
\ \ `\\ \     __  \ \ ,_\    __   
 \ \ , ` \  /'__`\ \ \ \/  /'__`\ 
  \ \ \`\ \/\ \L\.\_\ \ \_/\  __/ 
   \ \_\ \_\ \__/.\_\\ \__\ \____\
    \/_/\/_/\/__/\/_/ \/__/\/____/
                                  
                                  
 ____                         __                
/\  _`\                      /\ \               
\ \ \L\ \  _ __   ___     ___\ \ \/'\     ____  
 \ \  _ <'/\`'__\/ __`\  / __`\ \ , <    /',__\ 
  \ \ \L\ \ \ \//\ \L\ \/\ \L\ \ \ \\`\ /\__, `\
   \ \____/\ \_\\ \____/\ \____/\ \_\ \_\/\____/
    \/___/  \/_/ \/___/  \/___/  \/_/\/_/\/___/ 
	

	http://www.gta-sarp.com/forums/showthread.php?247211-Development-Related-Nate_J_Brooks-s-Request
	
	Credits to Ettore Morano (MoKoLayLay)
*/

#define PIZZAWORKERSKIN				(155)
#define NATEBROOKS_SQLID			(95363)
#define CREATEPIZZA_PICKUP_COORDS	1, 1, 1 // TODO: Pickup coordinates
#define KKPCLOTHES_PICKUP_COORDS	1, 1, 1 // TODO: Pickup coordinates
#define KKP_ANTISPAM_SECONDS		(60)

#define COLOR_INVALID       0xAA3333AA
#define TODO_COLOR			-1 // TODO: colors for the SendClientMessage

enum eKkpInfo {
	KKP_MEMBER,
	KKP_HAS_KEY,
	KKP_ANTISPAM,
	KKP_ORDER_PIZZA,
	KKP_ON_A_CALL,
	KKP_ACCEPTED_CALL
}

new kkp[MAX_PLAYERS][eKkpInfo];
new kkpVehicle;

forward kkpAntiSpamTimer(playerid);
public kkpAntiSpamTimer(playerid) {
    kkp[playerid][KKP_ANTISPAM] = 1;
    return 1;
}

IsKkpMember(playerid) {
	return ((PlayerInfo[playerid][pID] == NATEBROOKS_SQLID) || (kkp[playerid][KKP_MEMBER]));
}

Hook:NateBrooks_OnGameModeInit() {
	// TODO: Model ID, coordinates, colors
	kkpVehicle = CreateVehicle(448, 1, 1, 1, 1, 1, 1, -1);
	SetVehicleLockState(kkpVehicle, 1);
	return 1;
}

Hook:NateBrooks_OnPlayerConnect(playerid) {
	kkp[playerid][KKP_MEMBER] =	(PlayerInfo[playerid][pID] == NATEBROOKS_SQLID) ? 1 : 0;
	kkp[playerid][KKP_HAS_KEY] = (PlayerInfo[playerid][pID] == NATEBROOKS_SQLID) ? 1 : 0;
	kkp[playerid][KKP_ANTISPAM] = 0;
	kkp[playerid][KKP_ORDER_PIZZA] = 0;
	kkp[playerid][KKP_ON_A_CALL] = -1;
	kkp[playerid][KKP_ACCEPTED_CALL] = -1;
	return 1;
}

Hook:NateBrooks_OnPlayerDisconnect(playerid, reason) {
	kkp[playerid][KKP_ORDER_PIZZA] = 0;
	
	if (kkp[playerid][KKP_ON_A_CALL]) {
		kkp[playerid][KKP_ON_A_CALL] = -1;
		kkp[kkp[playerid][KKP_ON_A_CALL]][KKP_ACCEPTED_CALL] = -1
		SendClientMessage(kkp[playerid][KKP_ON_A_CALL], TODO_COLOR, "Your call has been cancelled, someone else would take your call.");
	}
	
	if (kkp[playerid][KKP_ACCEPTED_CALL]) {
		kkp[playerid][KKP_ACCEPTED_CALL] = -1;
		kkp[kkp[playerid][KKP_ACCEPTED_CALL]][KKP_ON_A_CALL] = -1;
		SendClientMessage(kkp[playerid][KKP_ACCEPTED_CALL], TODO_COLOR, "The customer has cancelled his call.");
	}
	return 1;
}

CMD:kkphelp(playerid) {
	SendClientMessage(playerid, TODO_COLOR, "_________________________________________________________");
	if(PlayerInfo[playerid][pID] == NATEBROOKS_SQLID)
		SendClientMessage(playerid, TODO_COLOR, "{FF3C3C}Nate Brooks: /makekkpmember, /removekkpmember, /givekkpkey, /takekkpkey");
	if (IsKkpMember(playerid)) {
		SendClientMessage(playerid, TODO_COLOR, "{FF3C3C}KKP Member: /kkpclothes, /kkpacceptcall, /kkpcancelcall");
		SendClientMessage(playerid, TODO_COLOR, "{FF3C3C}KKP Member: /kkpcreatepizza, /kkplock, /kkpcalls");
	}
    SendClientMessage(playerid, TODO_COLOR, "{FF3C3C}KKP Customer: /kkporderpizza, /kkpcancelorder");
	return 1;
}

CMD:makekkpmember(playerid, params[]) {
	if(PlayerInfo[playerid][pID] != NATEBROOKS_SQLID)
		return SendClientMessage(playerid, COLOR_INVALID, "You are not authorized to use this command.");
	
	new kkpMemberToAdd;
	if(sscanf(params, "i", kkpMemberToAdd))
		return SendClientMessage(playerid, COLOR_INVALID, "USAGE: /makekkpmember <ID>");
	
	if (playerid == kkpMemberToAdd)
		return SendClientMessage(playerid, COLOR_INVALID, "You can't use this command on yourself.");
	
	if (!IsPlayerConnected(kkpMemberToAdd))
		return SendClientMessage(playerid, COLOR_INVALID, "Invalid player ID.");
	
	new messageToOwner[128], messageToNewMember[128];
	format(messageToOwner, sizeof(messageToOwner), "You've added %s to KKP.", PlayerICName(kkpMemberToAdd));
	format(messageToNewMember, sizeof(messageToNewMember), "%s has made you a KKP member.", PlayerICName(playerid));
	kkp[kkpMemberToAdd][KKP_MEMBER] = 1;
	SendClientMessage(playerid, TODO_COLOR, messageToOwner);
	SendClientMessage(kkpMemberToAdd, TODO_COLOR, messageToNewMember);
	return 1;
}

CMD:removekkpmember(playerid, params[]) {
	if(PlayerInfo[playerid][pID] != NATEBROOKS_SQLID)
		return SendClientMessage(playerid, COLOR_INVALID, "You are not authorized to use this command.");
	
	new kkpMemberToRemove;
	if(sscanf(params, "i", kkpMemberToRemove))
		return SendClientMessage(playerid, COLOR_INVALID, "USAGE: /removekkpmember <ID>");
	
	if (playerid == kkpMemberToRemove)
		return SendClientMessage(playerid, COLOR_INVALID, "You can't use this command on yourself.");
	
	if (!IsPlayerConnected(kkpMemberToRemove))
		return SendClientMessage(playerid, COLOR_INVALID, "Invalid player ID.");
	
	new messageToOwner[128], messageToOldMember[128];
	format(messageToOwner, sizeof(messageToOwner), "You've removed %s from KKP.", PlayerICName(kkpMemberToRemove));
	format(messageToOldMember, sizeof(messageToOldMember), "%s has kick you from KKP.", PlayerICName(playerid));
	kkp[kkpMemberToRemove][KKP_MEMBER] = 0;
	SendClientMessage(playerid, TODO_COLOR, messageToOwner);
	SendClientMessage(kkpMemberToRemove, TODO_COLOR, messageToOldMember);
	return 1;
}

CMD:givekkpkey(playerid, params[]) {
	if(PlayerInfo[playerid][pID] != NATEBROOKS_SQLID)
		return SendClientMessage(playerid, COLOR_INVALID, "You are not authorized to use this command.");
	
	new kkpGiveKey;
	if(sscanf(params, "i", kkpGiveKey))
		return SendClientMessage(playerid, COLOR_INVALID, "USAGE: /givekkpkey <ID>");
	
	if (playerid == kkpGiveKey)
		return SendClientMessage(playerid, COLOR_INVALID, "You can't use this command on yourself.");
	
	if (!IsPlayerConnected(kkpGiveKey))
		return SendClientMessage(playerid, COLOR_INVALID, "Invalid player ID.");
	
	new Float:targetX, Float:targetY, Float:targetZ;
	GetPlayerPos(kkpGiveKey, targetX, targetY, targetZ);
	if (!IsPlayerInRangeOfPoint(playerid, 4.0, targetX, targetY, targetZ))
		return SendClientMessage(playerid, COLOR_INVALID, "You are not near the player.");

	if (kkp[kkpGiveKey][KKP_HAS_KEY])
		return SendClientMessage(playerid, COLOR_INVALID, "The player already have a key to KKP vehicle.");
		
	new giveKeyMessage[128], acceptKeyMessage[128];
	format(giveKeyMessage, sizeof(giveKeyMessage), "You've gave %s a key to KKP vehicle.", PlayerICName(kkpGiveKey));
	format(acceptKeyMessage, sizeof(acceptKeyMessage), "%s gave you a key to KKP vehicle.", PlayerICName(playerid));
	
	kkp[kkpGiveKey][KKP_HAS_KEY] = 1;
	SendClientMessage(playerid, TODO_COLOR, giveKeyMessage);
	SendClientMessage(kkpGiveKey, TODO_COLOR, acceptKeyMessage);
	return 1;
}

CMD:takekkpkey(playerid, params[]) {
	if(PlayerInfo[playerid][pID] != NATEBROOKS_SQLID)
		return SendClientMessage(playerid, COLOR_INVALID, "You are not authorized to use this command.");
	
	new kkpTakeKey;
	if(sscanf(params, "i", kkpTakeKey))
		return SendClientMessage(playerid, COLOR_INVALID, "USAGE: /takekkpkey <ID>");
	
	if (playerid == kkpTakeKey)
		return SendClientMessage(playerid, COLOR_INVALID, "You can't use this command on yourself.");
	
	if (!IsPlayerConnected(kkpTakeKey))
		return SendClientMessage(playerid, COLOR_INVALID, "Invalid player ID.");
	
	new Float:targetX, Float:targetY, Float:targetZ;
	GetPlayerPos(kkpTakeKey, targetX, targetY, targetZ);
	if (!IsPlayerInRangeOfPoint(playerid, 4.0, targetX, targetY, targetZ))
		return SendClientMessage(playerid, COLOR_INVALID, "You are not near the player.");
		
	if (!kkp[kkpTakeKey][KKP_HAS_KEY])
		return SendClientMessage(playerid, COLOR_INVALID, "The player doesn't have a key to KKP vehicle.");

	new takeKeyMessage[128], takenFromKeyMessage[128];
	format(takeKeyMessage, sizeof(takeKeyMessage), "You took the key to KKP vehicle from %s.", PlayerICName(kkpTakeKey));
	format(takenFromKeyMessage, sizeof(takenFromKeyMessage), "%s took your key to KKP vehicle.", PlayerICName(playerid));
	
	kkp[kkpTakeKey][KKP_HAS_KEY] = 0;
	SendClientMessage(playerid, TODO_COLOR, takeKeyMessage);
	SendClientMessage(kkpTakeKey, TODO_COLOR, takenFromKeyMessage);
	return 1;
}

CMD:kkplock(playerid) {
	if (!kkp[playerid][KKP_HAS_KEY])
		return SendClientMessage(playerid, COLOR_INVALID, "You don't have key to KKP vehicle.");
	
	new Float:vehicleX, Float:vehicleY, Float:vehicleZ;
	GetVehiclePos(kkpVehicle, vehicleX, vehicleY, vehicleZ);
	if (!IsPlayerInRangeOfPoint(playerid, 7.0, vehicleX, vehicleY, vehicleZ))
		return SendClientMessage(playerid, COLOR_INVALID, "You are not near KKP vehicle.");
	
	new lockVehicle[128];
	SetVehicleLockState(kkpVehicle, !gCarLock[kkpVehicle]);
	format(lockVehicle, sizeof(lockVehicle), "* %s %s their vehicle.", PlayerICName(playerid), gCarLock[kkpVehicle] ? "locks" : "unlocks");
	return 1;
}

CMD:kkpclothes(playerid) {
	if (!IsKkpMember(playerid))
		return SendClientMessage(playerid, COLOR_INVALID, "You are not a KKP member.");
	
	if(!IsPlayerInRangeOfPoint(playerid, 4.0, KKPCLOTHES_PICKUP_COORDS))
		return SendClientMessage(playerid, COLOR_INVALID, "You are not near KKP lockers.");
	
	SetPlayerSkin(playerid, PIZZAWORKERSKIN);
	return 1;
}

CMD:kkporderpizza(playerid) {
	if(kkp[playerid][KKP_ANTISPAM]) {
		new string_anti_spam[128];
		format(string_anti_spam, sizeof(string_anti_spam), "You must wait %i seconds before using this command again!", KKP_ANTISPAM_SECONDS);
		return SendClientMessage(playerid, COLOR_INVALID, string_anti_spam);
	}

	new orderedPizza[128];
	format(orderedPizza, sizeof(orderedPizza), "%s has ordered a pizza, type /kkpacceptcall %i to accept his call.", PlayerICName(playerid), playerid);
	
	new Float:playerX, Float:playerY, Float:playerZ;
	GetPlayerPos(playerid, playerX, playerY, playerZ);
	
	new kkpMembersOnline = 0;	
	for(new i = 0; i < GetPlayerPoolSize(); i++) {
		if (kkp[i][KKP_MEMBER]) {
			SendClientMessage(i, TODO_COLOR, orderedPizza);
			kkpMembersOnline = 1;
		}
	}
	
	if(!kkpMembersOnline)
		return SendClientMessage(playerid, TODO_COLOR, "[KKP] There's no one available to take your order, please try again later.");

	SetTimerEx("kkpAntiSpamTimer", (KKP_ANTISPAM_SECONDS * 1000), false, "i", playerid);
	kkp[playerid][KKP_ANTISPAM] = 1;
		
	kkp[playerid][KKP_ORDER_PIZZA] = 1;
	SendClientMessage(playerid, TODO_COLOR, "[KKP] You've ordered a pizza.");
	return 1;
}

CMD:kkpcancelorder(playerid) {
	if (!kkp[playerid][KKP_ORDER_PIZZA])
		return SendClientMessage(playerid, COLOR_INVALID, "You did not order a pizza.");
	
	if (kkp[playerid][KKP_ACCEPTED_CALL]) {
		new cancelOrder[128];
		format(cancelOrder, sizeof(cancelOrder), "%s cancelled his order.", PlayerICName(playerid));
		kkp[kkp[playerid][KKP_ACCEPTED_CALL]][KKP_ON_A_CALL] = -1;
		SendClientMessage(kkp[playerid][KKP_ACCEPTED_CALL], TODO_COLOR, cancelOrder);
	}
	
	kkp[playerid][KKP_ORDER_PIZZA] = 0;
	kkp[playerid][KKP_ACCEPTED_CALL] = -1;
	return 1;
}

CMD:kkpacceptcall(playerid, params[]) {
	if (!IsKkpMember(playerid))
		return SendClientMessage(playerid, COLOR_INVALID, "You are not a KKP member.");
	
	if (kkp[playerid][KKP_ON_A_CALL] != -1)
		return SendClientMessage(playerid, COLOR_INVALID, "You are already on a call.");
	
	new targetid;
	if(sscanf(params, "i", targetid))
		return SendClientMessage(playerid, COLOR_INVALID, "USAGE: /kkpacceptcall <ID>");
	
	if (!IsPlayerConnected(targetid))
		return SendClientMessage(playerid, COLOR_INVALID, "Invalid player ID.");
		
	if (!kkp[targetid][KKP_ORDER_PIZZA])
		return SendClientMessage(playerid, COLOR_INVALID, "This player didn't order a pizza.");
	
	if (kkp[targetid][KKP_ACCEPTED_CALL] != -1)
		return SendClientMessage(playerid, COLOR_INVALID, "Someone already accepted the call.");
	
	SendClientMessage(targetid, COLOR_INVALID, "[KKP] Your call has been accepted, please stay where you are.");
	kkp[targetid][KKP_ACCEPTED_CALL] = playerid;
	kkp[playerid][KKP_ON_A_CALL] = targetid;
	
	new acceptedCall[128];
	format(acceptedCall, sizeof(acceptedCall), "%s accepted %s's call.", PlayerICName(playerid), PlayerICName(targetid));
	
	for(new i = 0; i < GetPlayerPoolSize(); i++) {
		if (kkp[i][KKP_MEMBER])
			SendClientMessage(i, TODO_COLOR, acceptedCall);
	}
	return 1;
}

CMD:kkpcancelcall(playerid) {
	if (!IsKkpMember(playerid))
		return SendClientMessage(playerid, COLOR_INVALID, "You are not a KKP member.");
	
	if (kkp[playerid][KKP_ON_A_CALL] == -1)
		return SendClientMessage(playerid, COLOR_INVALID, "You are not on a call.");
	
	new cancelCall[128];
	format(cancelCall, sizeof(cancelCall), "You have cancelled %'s call.", PlayerICName(kkp[playerid][KKP_ON_A_CALL]));
	
	kkp[kkp[playerid][KKP_ON_A_CALL]][KKP_ACCEPTED_CALL] = -1;
	SendClientMessage(kkp[playerid][KKP_ON_A_CALL], TODO_COLOR, "Your call has been cancelled, someone else would take your call.");
	
	kkp[playerid][KKP_ON_A_CALL] = -1;
	SendClientMessage(playerid, TODO_COLOR, cancelCall);
	return 1;
}

CMD:kkpcalls(playerid) {
	if (!IsKkpMember(playerid))
		return SendClientMessage(playerid, COLOR_INVALID, "You are not a KKP member.");
	
	SendClientMessage(playerid, TODO_COLOR, "KKP Calls:");
	for(new i = 0; i < GetPlayerPoolSize(); i++) {
		if (kkp[i][KKP_ORDER_PIZZA]) {
			new callString[128];
			format(callString, sizeof(callString), "%s (%s)", PlayerICName(i), kkp[i][KKP_ACCEPTED_CALL] != -1 ? PlayerICName(kkp[i][KKP_ACCEPTED_CALL]) : "N/A");
			SendClientMessage(playerid, TODO_COLOR, callString);
		}
	}
	return 1;
}

CMD:kkpcreatepizza(playerid) {
	if (!IsKkpMember(playerid))
		return SendClientMessage(playerid, COLOR_INVALID, "You are not a KKP member.");
		
	new Float:playerX, Float:playerY, Float:playerZ;
	GetPlayerPos(playerid, playerX, playerY, playerZ);
	if (!IsPlayerInRangeOfPoint(playerid, 4.0, CREATEPIZZA_PICKUP_COORDS))
		return SendClientMessage(playerid, COLOR_INVALID, "You are not near the oven.");

	givePlayerFoodTray(playerid, 1); // 1 == pizza box
	return 1;
}

// TODO: Save the members and the people who has key

// FINISHED SINCE LAST PUSH: kkpcreatepizza