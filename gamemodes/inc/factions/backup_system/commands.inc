/************************

	File:
	- commands.inc

	Description:
	- New and improved backup system by Jay Cortez

*************************/

/* Command to call backup */
CMD:backuptest(playerid, params[]) {
	if(!BackupAllowed(playerid))
		return AdmErrorMsg;

	if(IsPlayerRestricted(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You cannot call backup at this moment.");

	if(CheckForJammer(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "Backup signal has been jammed.");

	new bkType[10];
	if(sscanf(params, "s[10]", bkType)) {

		if(PlayerInfo[playerid][pGroup] == GROUP_S9) 
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /backup [SAPD, DoC, SAFMD, S9]");
		else
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /backup [SAPD, DoC, SAFMD]");
		
		return 1;
	}

	new bkFaction;

	if(strmatch(bkType, "sapd")) {
		bkFaction = FACTION_SAPD;
	}
	else if(strmatch(bkType, "doc")) {
		bkFaction = FACTION_ARMY;
	}
	else if(strmatch(bkType, "safmd")) {
		bkFaction = FACTION_FMD;
	}
	else if(strmatch(bkType, "s9") && PlayerInfo[playerid][pGroup] == GROUP_S9) {
		bkFaction = FACTION_S9;
	}
	else return cmd_backuptest(playerid, "");

	// Check if already called backup for this fac
	if(IsPlayerCallingBackup(playerid, bkFaction))
		return SendClientMessage(playerid, COLOR_GREY, "You have already sent a backup request to this faction, be patient.");

	// S9 anti reveal
	if(PlayerInfo[playerid][pGroup] == GROUP_S9 && bkFaction != FACTION_S9 && !PlayerInfo[playerid][pMask])
		return SendClientMessage(playerid, COLOR_GREY, "You should put on a mask or be in a fake faction to prevent revealing yourself.");

	// Send notification to faction
	new backupStr[128];
	format(backupStr, sizeof(backupStr), "HQ: Incoming backup request from %s. (/backups)", GetPlayerBackupName(playerid, bkFaction));

	foreach(new i:Player)
	{
		if(PlayerInfo[i][pMember] != bkFaction && PlayerInfo[i][pLeader] != bkFaction && FakeFaction[i] != bkFaction && RadioFrequency[i] != bkFaction)
			continue;	

		if(CheckForJammer(i))
			continue;

		SendClientMessage(i, GetFactionColour(bkFaction), backupStr);
		DoBackupSound(i, 1);
	}

	// Create the backup request
	CreateBackupRequest(playerid, bkFaction);

	// Show action above head
	if(bkFaction != FACTION_S9) {
		format(backupStr, sizeof(backupStr), "* %s reaches down and presses a button on their waistbelt.", PlayerICName(playerid));
		SetPlayerChatBubble(playerid, backupStr, COLOR_PURPLE, 20.0, 15000);
	}

	// Show HUD info
	format(backupStr, sizeof(backupStr), "~b~%s ~w~backup called.", GetFactionName(bkFaction));
	displayCenterHUDInfo(playerid, backupStr, 4);

	// Show notification in chat
	format(backupStr, sizeof(backupStr), "* You have called for {%06x}%s{33CCFF} backup.", GetFactionColour(bkFaction) >>> 8, GetFactionName(bkFaction));
	SendClientMessage(playerid, COLOR_LIGHTBLUE, backupStr);
	return 1;
}

/* Command to see backup calls */
CMD:backups(playerid, params[]) {
	if(!BackupAllowed(playerid))
		return AdmErrorMsg;

	if(IsPlayerRestricted(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You cannot see backup calls at this moment.");

	if(CheckForJammer(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "Signal has been jammed.");

	new facToDisplay = PlayerInfo[playerid][pMember];

	// Check fake faction backups
	new fakeCheck[5];
	if(!sscanf(params, "s[5]", fakeCheck)) {
		if(	PlayerInfo[playerid][pMember] == FACTION_S9 &&
			strmatch(fakeCheck, "fake")
		) {
			if(!FakeFaction[playerid])
				return SendClientMessage(playerid, COLOR_GREY, "You are not undercover in a faction at the moment.");

			facToDisplay = FakeFaction[playerid];
		}
	}

	// The faction currently has no open backup requests
	if(!FactionBackupCallsCount(facToDisplay)) {
		if(facToDisplay == PlayerInfo[playerid][pMember])
			return SendClientMessage(playerid, COLOR_GREY, "There are currently no backup calls for your faction.");
		else
			return SendClientMessage(playerid, COLOR_GREY, "There are currently no backup calls for your fake faction.");
	}

	// Display the list of backups
	ShowFactionBackups(playerid, facToDisplay);
	return 1;
}

/* Command to see backup calls for fake faction (Sector 9) */
CMD:fakebackups(playerid, params[])
	return cmd_backups(playerid, "fake");

CMD:fbackups(playerid, params[])
	return cmd_backups(playerid, "fake");

/* Command to cancel responding to current backup call */
CMD:bkctest(playerid, params[]) {
	if(!BackupAllowed(playerid))
		return AdmErrorMsg;

	if(IsPlayerRestricted(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You cannot do this at this moment.");

	if(!IsPlayerCallingBackup(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You don't have any backup requests out.");

	CancelAllBackupRequests(playerid, BK_CANCEL_REASON_SELF);
	SendClientMessage(playerid, COLOR_LIGHTBLUE, "* Your backup requests have been cancelled.");
	return 1;
}