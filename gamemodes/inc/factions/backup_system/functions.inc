/************************

	File:
	- functions.inc

	Description:
	- New and improved backup system by Jay Cortez

*************************/

/* Creates a new backup request */
stock CreateBackupRequest(playerid, forFactionID) {
	// Find a free backup ID
	new freeBkID = INVALID_BACKUP_ID;

	for(new i = 0; i < MAX_BACKUP_CALLS; i++) {
		if(!BackupInfo[i][bActive]) {
			freeBkID = i;
			break;
		}
	}

	if(freeBkID == INVALID_BACKUP_ID)
		return false;

	// Setup the backup request
	BackupInfo[freeBkID][bActive] = true;
	BackupInfo[freeBkID][bCaller] = playerid;
	BackupInfo[freeBkID][bFactionID] = forFactionID;
	BackupInfo[freeBkID][bTimestamp] = gettime();

	if(PlayerInfo[playerid][pGroup] != GROUP_S9)
		BackupInfo[freeBkID][bFromFaction] = PlayerInfo[playerid][pMember];
	else {
		if(forFactionID == GROUP_S9 || PlayerInfo[playerid][pMask])
			BackupInfo[freeBkID][bFromFaction] = GROUP_S9;
		else
			BackupInfo[freeBkID][bFromFaction] = FakeFaction[playerid];
	}
	return true;
}

/* Is the player allowed to call backup */
stock BackupAllowed(playerid) {
	if(
		PlayerInfo[playerid][pMember] == 1 ||	// SAPD
		PlayerInfo[playerid][pMember] == 2 ||	// FBI
		PlayerInfo[playerid][pMember] == 3 ||	// SASD
		PlayerInfo[playerid][pMember] == 4 ||	// SAFMD
		PlayerInfo[playerid][pMember] == 5 ||	// DoC
		PlayerInfo[playerid][pGroup] == GROUP_S9 	// Sector 9
	) return true;

	return false;
}

/* Gets the count of backup calls for a faction */
stock FactionBackupCallsCount(factionID) {
	new bkCount = 0;

	for(new i = 0; i < MAX_BACKUP_CALLS; i++) {
		if(BackupInfo[i][bActive] && BackupInfo[i][bFactionID] == factionID) 
			bkCount++;
	}

	return bkCount;
}

/* Plays the backup sound */
forward DoBackupSound(playerid, stage);
public DoBackupSound(playerid, stage) {
	if(stage <= 3) {
		PlayerPlaySound(playerid, BACKUP_SOUND, 0.0, 0.0, 0.0);
		SetTimerEx("DoBackupSound", 80, false, "ii", playerid, stage + 1);
	}
	return 1;
}

/* Show a faction's backups to a player */
stock ShowFactionBackups(playerid, bkFaction) {
	// Keep track of which faction we're looking at
	backupFaction[playerid] = bkFaction;

	// Create title
	new backupDiaTitle[50];
	format(backupDiaTitle, sizeof(backupDiaTitle), "{%06x}%s backup calls [%i]", GetFactionColour(bkFaction) >>> 8, GetFactionName(bkFaction), FactionBackupCallsCount(bkFaction));

	// Create content
	new backupDiaContent[1024];
	format(backupDiaContent, sizeof(backupDiaContent), "Requested by\tLocation\tTime");

	for(new i = 0; i < MAX_BACKUP_CALLS; i++) {
		if(BackupInfo[i][bActive] && BackupInfo[i][bFactionID] == bkFaction) {
			// Format the time
			new bkCallerTime[20];
			format(bkCallerTime, sizeof(bkCallerTime), "%i sec. ago", gettime() - BackupInfo[i][bTimestamp]);

			// Add to the dialog content
			format(backupDiaContent, sizeof(backupDiaContent),
				"%s\n%s\t%s\t%s",
				backupDiaContent,
				GetPlayerBackupName(BackupInfo[i][bCaller], BackupInfo[i][bFactionID]),
				GetPlayerBackupLocation(BackupInfo[i][bCaller]),
				bkCallerTime
			);
		}
	}

	// Show the dialog
	ShowPlayerDialog(playerid, DIALOG_BACKUPS, DIALOG_STYLE_TABLIST_HEADERS, backupDiaTitle, backupDiaContent, "Respond", "Close");
	return 1;
}

/* Gets the location of a backup caller */
stock GetPlayerBackupLocation(playerid) {
	new bkCallerLocation[MAX_ZONE_NAME];

	if(CheckForJammer(playerid))
		format(bkCallerLocation, sizeof(bkCallerLocation), "No signal...");
	else if(GetPlayerVirtualWorld(playerid) || GetPlayerInterior(playerid))
		format(bkCallerLocation, sizeof(bkCallerLocation), "Inside");
	else
		GetPlayer2DZone(playerid, bkCallerLocation, sizeof(bkCallerLocation));

	return bkCallerLocation;
}

/* Gets the name of a backup caller */
stock GetPlayerBackupName(playerid, bkFaction) {
	new nameToReturn[50];

	// Sector 9, special backup format
	if(PlayerInfo[playerid][pGroup] == GROUP_S9) {
		if(bkFaction == FACTION_S9)
			format(nameToReturn, sizeof(nameToReturn), "%s %s %s", GetGroupName(GROUP_S9), RealS9Rank(playerid), PlayerICName(playerid));
		else if(PlayerInfo[playerid][pMask])
			format(nameToReturn, sizeof(nameToReturn), "%s %s", GetGroupName(GROUP_S9), RealS9Rank(playerid));
	}
	
	// Normal name format
	if(!strlen(nameToReturn))
		format(nameToReturn, sizeof(nameToReturn), "%s %s %s", GetPlayerFactionName(playerid), GetPlayerRank(playerid), PlayerICName(playerid));

	return nameToReturn;
}

/* Checks if a player is responding to a backup call */
stock IsPlayerRespondingToBackup(playerid) {
	return (BackupAllowed(playerid) && respondingToBackup[playerid] != INVALID_BACKUP_ID);
}

/* Check if a player currently has a backup request out (for a faction) */
stock IsPlayerCallingBackup(playerid, forFactionID = 0) {
	for(new i = 0; i < MAX_BACKUP_CALLS; i++) {
		if(BackupInfo[i][bActive] && BackupInfo[i][bCaller] == playerid) {
			if(forFactionID != 0 && BackupInfo[i][bFactionID] != forFactionID)
				continue;

			return true;
		}
	}
	return false;
}

/* Called when a player selects a backup item in the dialog */
stock OnPlayerRespondToBackup(playerid, listitem) {
	// Backups of which faction
	new bkFaction = backupFaction[playerid];

	// Try to find the backup ID from the listitem
	new backupID = INVALID_BACKUP_ID, backupCounter = 0;

	for(new i = 0; i < MAX_BACKUP_CALLS; i++) {
		if(BackupInfo[i][bActive] && BackupInfo[i][bFactionID] == bkFaction) {
			if(backupCounter == listitem) {
				backupID = i;
				break;
			}

			backupCounter++;
		}
	}

	// Check if the backup item was found
	if(backupID == INVALID_BACKUP_ID)
		return SendClientMessage(playerid, COLOR_GREY, "The backup call you were trying to respond to has expired.");

	if(BackupInfo[backupID][bCaller] == playerid)
		return SendClientMessage(playerid, COLOR_GREY, "You cannot respond to your own backup calls.");

	if(IsPlayerRespondingToBackup(playerid)) {
		if(respondingToBackup[playerid] == backupID)
			return SendClientMessage(playerid, COLOR_GREY, "You are already responding to this backup call.");

		CancelBackupResponding(playerid);
	}

	StartBackupResponding(playerid, backupID);
	return 1;
}

/* Starts responding to a backup call */
stock StartBackupResponding(playerid, backupID) {
	// Send a message
	new acceptStr[128];
	format(acceptStr, sizeof(acceptStr), "You are now responding to %s's backup call.", GetPlayerBackupName(BackupInfo[backupID][bCaller], BackupInfo[backupID][bFactionID]));
	SendClientMessage(playerid, GetFactionColour(PlayerInfo[playerid][pMember]), acceptStr);

	// Set responding variable
	respondingToBackup[playerid] = backupID;

	// Start timer
	backupTimer[playerid] = SetTimerEx("BackupRespondUpdate", 1000, true, "i", playerid);
	return 1;
}

/* Cancels the current backup request a player is responding to */
stock CancelBackupResponding(playerid, cancelReason = BK_CANCEL_REASON_SELF) {
	// Kill the update timer
	KillTimer(backupTimer[playerid]);

	// Get the backup ID of current responding backup
	new backupID = respondingToBackup[playerid];

	// Send the message in chat
	new cancelStr[128];

	if(cancelReason != BK_CANCEL_REASON_ARRIVE) {
		switch(cancelReason) {
			case BK_CANCEL_REASON_SELF:
				format(cancelStr, sizeof(cancelStr), "Cancelled");
			case BK_CANCEL_REASON_DEATH:
				format(cancelStr, sizeof(cancelStr), "Death");
			case BK_CANCEL_REASON_QUIT:
				format(cancelStr, sizeof(cancelStr), "They quit");
		}

		format(cancelStr, sizeof(cancelStr), "You have cancelled responding to %s's backup request. (( %s ))",
			GetPlayerBackupName(BackupInfo[backupID][bCaller], BackupInfo[backupID][bFactionID]),
			cancelStr
		);
	}
	else {
		format(cancelStr, sizeof(cancelStr), "You have arrived at the backup call.");
	}

	SendClientMessage(playerid, GetFactionColour(PlayerInfo[playerid][pMember]), cancelStr);
	
	// Reset responding variable
	respondingToBackup[playerid] = INVALID_BACKUP_ID;

	// Reset center HUD
	displayCenterHUDInfo(playerid, "", 1);

	// Reset player marker
	SetPlayerMarkerForPlayer(playerid, BackupInfo[backupID][bCaller], 0xFFFFFFAA);
	SetPlayerToTeamColor(BackupInfo[backupID][bCaller]);
	return 1;
}

/* Cancel all backup requests of a player */
stock CancelAllBackupRequests(playerid, cancelReason) {
	// Cancel all their backup requests
	for(new i = 0; i < MAX_BACKUP_CALLS; i++) {
		if(BackupInfo[i][bActive] && BackupInfo[i][bCaller] == playerid) {
			CancelBackupRequest(i, cancelReason);
		}
	}

	// Send message
	new cancelStr[128];

	switch(cancelReason) {
		case BK_CANCEL_REASON_SELF:
			format(cancelStr, sizeof(cancelStr), "Cancelled");
		case BK_CANCEL_REASON_DEATH:
			format(cancelStr, sizeof(cancelStr), "You died");
		case BK_CANCEL_REASON_QUIT:
			format(cancelStr, sizeof(cancelStr), "You quit the game");
	}

	format(cancelStr, sizeof(cancelStr), "You have cancelled your backup requests. (( %s ))",
		cancelStr
	);

	SendClientMessage(playerid, GetFactionColour(PlayerInfo[playerid][pMember]), cancelStr);
	return 1;
}

/* Cancel a backup request */
stock CancelBackupRequest(backupID, cancelReason) {
	BackupInfo[backupID][bActive] = false;

	foreach(new i:Player) {
		if(BackupAllowed(i) && respondingToBackup[i] == backupID) {
			CancelBackupResponding(i, cancelReason);
		}
	}
	return 1;
}

/* Get the player's distance to a backup call */
stock GetPlayerDistanceToBackup(playerid, backupID) {
	new backupCaller = BackupInfo[backupID][bCaller];
	new distanceStr[30];

	if(CheckForJammer(backupCaller))
		format(distanceStr, sizeof(distanceStr), "No signal...");
	else if(GetPlayerVirtualWorld(backupCaller) || GetPlayerInterior(backupCaller))
		format(distanceStr, sizeof(distanceStr), "Unknown");
	else
		format(distanceStr, sizeof(distanceStr), "%im", floatround(GetDistanceBetweenPlayers(playerid, backupCaller)));

	return distanceStr;
}

/* Called every second for players who are responding to a backup call */
forward BackupRespondUpdate(playerid);
public BackupRespondUpdate(playerid) {
	new backupID = respondingToBackup[playerid];

	// HUD info
	new hudStr[128];
	format(hudStr, sizeof(hudStr), "~w~Responding to ~b~%s's~w~ backup call.~n~Distance: ~r~%s",
		GetPlayerBackupName(BackupInfo[backupID][bCaller], BackupInfo[backupID][bFactionID]),
		GetPlayerDistanceToBackup(playerid, backupID)
	);
	displayCenterHUDInfo(playerid, hudStr, 5);

	// Set marker
	if(BackupInfo[backupID][bFromFaction] == FACTION_S9 && PlayerInfo[playerid][pMember] != FACTION_S9) {
		SetPlayerMarkerForPlayer(playerid, BackupInfo[backupID][bCaller], 0xFFFFFF);
	}
	else {
		SetPlayerMarkerForPlayer(playerid, BackupInfo[backupID][bCaller],
			(GetFactionColour(BackupInfo[backupID][bFromFaction]) | 0x000000FF)
		);
	}

	if(GetDistanceBetweenPlayers(playerid, BackupInfo[backupID][bCaller]) <= 15)
		OnPlayerArriveAtBackup(playerid, backupID);
	return 1;
}

/* Called when a player arrives at a backup call */
stock OnPlayerArriveAtBackup(playerid, backupID) {
	CancelBackupResponding(playerid, BK_CANCEL_REASON_ARRIVE);

	// Send message to caller
	new bkCaller = BackupInfo[backupID][bCaller];
	new bkStr[128];
	format(bkStr, sizeof(bkStr), "* %s has arrived at your backup call.", PlayerICName(playerid));
	SendClientMessage(bkCaller, GetFactionColour(BackupInfo[backupID][bFromFaction]), bkStr);
	return 1;
}