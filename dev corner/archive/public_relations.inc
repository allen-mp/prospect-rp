/*
	SARP Public Relations Script
	By: Avery Booker
	03/04/2018
*/

/*========================
	Defines and Globals
=========================*/

#define PR_RANK_NONE 0
#define PR_RANK_MEMBER 1
#define PR_RANK_MOD 2

#define COLOR_PR 0x8AF393AA

new bool:toggedPRChat[MAX_PLAYERS];

/*========================
	Commands
=========================*/

/*
	CMD: /makepr
	Params: none, member, mod
	Description: Makes a member part of the PR team.
*/
CMD:makepr(playerid, params[]) {
	// If the player is below head admin or not a PR mod
	if(PlayerInfo[playerid][pAdmin] < 1337 && PlayerInfo[playerid][pPR] != 2) return SendClientMessage(playerid, COLOR_GRAD2, "{FF0000}You're not authorized to use that command.");

	new targetid, rank[8], str2[256];
	if(sscanf(params, "us[8]", targetid, rank)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /makepr [player] [none / member / mod]");

	if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, COLOR_GREY, "That player isn't connected.");

	// Removal from the PR team.
	if(strcmp(rank, "none", true) == 0) {
		format(str2, sizeof(str2), "{FF0000}AdmWarning{FFFFFF}: %s has removed %s's status as a PR representative.", PlayerOOCName(playerid), PlayerOOCName(targetid));
		ABroadCast(COLOR_WHITE, str2, 1);

		format(str2, sizeof(str2), "You were removed from the PR team by %s.", PlayerOOCName(playerid));
		SendClientMessage(targetid, COLOR_LIGHTBLUE, str2);

		format(str2, sizeof(str2), "You kicked %s from the PR team.", PlayerOOCName(targetid));
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str2);

		PlayerInfo[targetid][pPR] = PR_RANK_NONE;
		return 1;
	}
	else if(strcmp(rank, "member", true) == 0) {
		PlayerInfo[targetid][pPR] = PR_RANK_MEMBER;
	}
	else if(strcmp(rank, "mod", true) == 0) {
		PlayerInfo[targetid][pPR] = PR_RANK_MOD;
	}
	else return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /makepr [player] [none / member / mod]");

	format(str2, sizeof(str2), "%s has made you a Public Relations %s", PlayerOOCName(playerid), GetPRRankName(PlayerInfo[targetid][pPR]));
	SendClientMessage(targetid, COLOR_LIGHTBLUE, str2);

	format(str2, sizeof(str2), "{FF0000}AdmWarning{FFFFFF}: %s has made %s a PR %s.", PlayerOOCName(playerid), PlayerOOCName(playerid), GetPRRankName(PlayerInfo[targetid][pPR]));
	ABroadCast(COLOR_WHITE, str2, 1);
	return 1;
}

/*
	CMD: /prchat, /pc
	Params: string
	Description: Sends a PR chat message
*/
CMD:prchat(playerid, params[]) {
	if(PlayerInfo[playerid][pPR] < PR_RANK_MEMBER) return AdmErrorMsg;
	if(toggedPRChat[playerid]) return SendClientMessage(playerid, COLOR_PR, "Please re-enable PR chat with /togpc");

	new str2[350];
	if(sscanf(params, "s[350]", str2)) return 1;

	format(str2, sizeof(str2), "%s %s: %s", GetPRRankName(PlayerInfo[playerid][pPR]), PlayerOOCName(playerid), str2);
	SendPRMessage(COLOR_PR, str2);
	return 1;
}
CMD:pc(playerid, params[]) return cmd_prchat(playerid, params);

/*
	CMD: /prhelp
	Params: None
	Description: Shows PR help dialog
*/
CMD:prhelp(playerid, params[]) {
	if(PlayerInfo[playerid][pPR] < PR_RANK_MEMBER) return AdmErrorMsg;

	SendClientMessage(playerid, -1, "{8AF393}Representative:{FFFFFF} /(p)r(c)hat");
	if(PlayerInfo[playerid][pPR] >= PR_RANK_MOD)
		SendClientMessage(playerid, -1, "{8AF393}Moderator:{FFFFFF} /makepr");
	return 1;
}

/*========================
	Functions
=========================*/

SendPRMessage(color, string2[])
{
	foreach(new i: Player)
	{
		if(PlayerInfo[i][pPR] >= 1 && !toggedPRChat[i])
			SendClientMessage(i, color, string2);
	}
}

stock GetPRRankName(rankid) {
	new rank[18];

	switch(rankid)
	{
		case PR_RANK_MEMBER: rank = "Representative";
		case PR_RANK_MOD: rank = "Moderator";
		default: rank = "None";
	}
	return rank;
}