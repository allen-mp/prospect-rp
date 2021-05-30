/************************

	File:
	- core.inc

	Description:
	- New and improved backup system by Jay Cortez

*************************/

#define MAX_BACKUP_CALLS	100
#define INVALID_BACKUP_ID	-1
#define BACKUP_SOUND		1083
#define DIALOG_BACKUPS		9382

// Store backup information
enum backupEnum {
	bool:bActive,
	bCaller,
	bFactionID,
	bTimestamp,
	bFromFaction
};
new BackupInfo[MAX_BACKUP_CALLS][backupEnum];

// Player bound variables
new backupFaction[MAX_PLAYERS], respondingToBackup[MAX_PLAYERS], backupTimer[MAX_PLAYERS];

// Reasons for cancelling a backup request
enum {
	BK_CANCEL_REASON_SELF,			// The player cancelled
	BK_CANCEL_REASON_DEATH,			// The player died
	BK_CANCEL_REASON_QUIT,			// The player quit the server
	BK_CANCEL_REASON_ARRIVE			// The player arrives at backup call
};

Hook:backup_OnPlayerDisconnect(playerid, reason) {
	KillTimer(backupTimer[playerid]);
	backupFaction[playerid] = -1;
	respondingToBackup[playerid] = INVALID_BACKUP_ID;

	// Cancel backup on disconnect
	if(BackupAllowed(playerid)) {
		if(IsPlayerCallingBackup(playerid))
			CancelAllBackupRequests(playerid, BK_CANCEL_REASON_QUIT);

		if(IsPlayerRespondingToBackup(playerid)) 
			CancelBackupResponding(playerid, BK_CANCEL_REASON_QUIT);
	}
}

/* Backups dialog */
Hook:backup_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) {
	if(dialogid == DIALOG_BACKUPS && response && BackupAllowed(playerid))
		OnPlayerRespondToBackup(playerid, listitem);
}

/* Cancel backup on death */
Hook:backup_OnPlayerDeath(playerid, killerid, reason) {
	if(BackupAllowed(playerid) && pDeathState[playerid] == PLAYER_STATE_REVIVE) {
		if(IsPlayerCallingBackup(playerid))
			CancelAllBackupRequests(playerid, BK_CANCEL_REASON_DEATH);

		if(IsPlayerRespondingToBackup(playerid)) 
			CancelBackupResponding(playerid, BK_CANCEL_REASON_DEATH);
	}
}

#include "inc/factions/backup_system/functions.inc"
#include "inc/factions/backup_system/commands.inc"