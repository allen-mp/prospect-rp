/*

  _____                       _      _____           _                   ____             _____                                      _ 
 |  __ \                     | |    / ____|         | |                 |  _ \           |  __ \                                    | |
 | |__) |___ _ __   ___  _ __| |_  | (___  _   _ ___| |_ ___ _ __ ___   | |_) |_   _     | |  | | ___  ___ _ __ ___   ___  _ __   __| |
 |  _  // _ \ '_ \ / _ \| '__| __|  \___ \| | | / __| __/ _ \ '_ ` _ \  |  _ <| | | |    | |  | |/ _ \/ __| '_ ` _ \ / _ \| '_ \ / _` |
 | | \ \  __/ |_) | (_) | |  | |_   ____) | |_| \__ \ ||  __/ | | | | | | |_) | |_| |    | |__| |  __/\__ \ | | | | | (_) | | | | (_| |
 |_|  \_\___| .__/ \___/|_|   \__| |_____/ \__, |___/\__\___|_| |_| |_| |____/ \__, |    |_____/ \___||___/_| |_| |_|\___/|_| |_|\__,_|
            | |                             __/ |                               __/ |                                                  
            |_|                            |___/                               |___/                                                

*/

#define MAX_REPORTS (200)
#define DIALOG_REPORTS 14879
#define DIALOG_REPORTS_ACCEPT 14880

enum reportsinfo
{
	rReporterName[MAX_PLAYER_NAME], // name of the reporter
	rReporterID, // id of the reporter
	bool:rEnabled, // if that report is active or not
	rText[128] // text of the report

};

new reportscount = 0;
new Report[MAX_REPORTS][reportsinfo];
new viewingreport[MAX_PLAYERS];
new speakingtoadmin[MAX_PLAYERS], speakingtoplayer[MAX_PLAYERS], Reported[MAX_PLAYERS], bool:areported[MAX_PLAYERS], tstr[128];


Hook:reports_OnGameModeInit()
{
	for(new i = 0; i < MAX_REPORTS; i++)
	{
		Report[i][rReporterID] = INVALID_PLAYER_ID;
		Report[i][rEnabled] = false;
	}
	return 1;
}

Hook:reports_OnPlayerConnect(playerid)
{
	speakingtoadmin[playerid] = INVALID_PLAYER_ID;
	speakingtoplayer[playerid] = INVALID_PLAYER_ID;
	return 1;
}

Hook:reports_OnPlayerDisconnect(playerid, reason)
{
	for(new i = 0; i < MAX_REPORTS; i ++)
	{
		if(Report[i][rReporterID] == playerid && Report[i][rEnabled] == true)
		{
			Report[i][rReporterID] = INVALID_PLAYER_ID;
			if(0 <= speakingtoadmin[playerid] < MAX_PLAYERS) 
			{
				speakingtoplayer[speakingtoadmin[playerid]] = INVALID_PLAYER_ID;
				speakingtoadmin[playerid] = INVALID_PLAYER_ID;
			}
			if(0 <= speakingtoplayer[playerid] < MAX_PLAYERS) 
			{
				speakingtoadmin[speakingtoplayer[playerid]] = INVALID_PLAYER_ID;
				speakingtoplayer[playerid] = INVALID_PLAYER_ID;
			}
			Report[i][rEnabled] = false;
			reportscount--;
		}
	}

	return 1;
}

Hook:reports_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_REPORTS)
	{
		if(response)
		{
			new fakeListitem = 0, selectedreport;
            for(new i = 0; i < MAX_REPORTS; i++) 
            {
           	 	if(Report[i][rEnabled] == true) 
           	 	{
            		if(fakeListitem == listitem) selectedreport = i;
                    fakeListitem++;
                }
            }
            format(tstr, sizeof(tstr), "%s's Report %d", Report[selectedreport][rReporterName], selectedreport);
            new rinfostr[128];
            viewingreport[playerid] = selectedreport;
            format(rinfostr, sizeof(rinfostr), "%s", Report[selectedreport][rText]);
            ShowPlayerDialog(playerid, DIALOG_REPORTS_ACCEPT, DIALOG_STYLE_MSGBOX, tstr, rinfostr, "Accept", "Back");
		}
	}
	if(dialogid == DIALOG_REPORTS_ACCEPT)
	{
		new reportid = viewingreport[playerid];
		if(response)
		{
			if(Report[reportid][rEnabled] == false)
			{
				SendClientMessage(playerid, COLOR_GREY, "This report is no longer active or another administrator is current dealing with it.");
				viewingreport[playerid] = -1;
			}
			else if(speakingtoadmin[Report[reportid][rReporterID]] != INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_GREY, "That player is currently speaking to an administrator.");
			else
			{
				format(tstr, sizeof(tstr), "%s has accepted %s's report (reportid: %d)", PlayerName(playerid), Report[reportid][rReporterName], reportid);
				ABroadCast(COLOR_LIGHTYELLOW, tstr, 1);
				SendClientMessage(playerid, COLOR_LIGHTYELLOW, "Use /ac to communicate with the player, /endac to end the chat session.");

				if(PlayerInfo[playerid][pAdmin] >= 2 && PlayerInfo[playerid][pStealthed] != 1)
				{
					format(tstr, sizeof(tstr), "%s has accepted your report, /ac to communicate.", PlayerName(playerid));
					SendClientMessage(Report[reportid][rReporterID], COLOR_GREY, tstr);
				}

				else if(PlayerInfo[playerid][pAdmin] == 1 || PlayerInfo[playerid][pStealthed] == 1)
				{
					SendClientMessage(Report[reportid][rReporterID], COLOR_GREY, "A silent admin has accepted your report, /ac to communicate.");
				}
				Report[reportid][rEnabled] = false;
				speakingtoplayer[playerid] = Report[reportid][rReporterID];
				speakingtoadmin[Report[reportid][rReporterID]] = playerid;
				Report[reportid][rReporterID] = INVALID_PLAYER_ID;
				reportscount--;
			}
		}
		if(!response)
		{
			new finalreportstr[8000];
			for(new i = 0; i < MAX_REPORTS; i++)
			{
				if(Report[i][rEnabled] == true)
				{
					format(finalreportstr, sizeof(finalreportstr), "%s%d. %s (%d): %s\n", finalreportstr, i, Report[i][rReporterName], Report[i][rReporterID], Report[i][rText]);
				}	
			}	
			ShowPlayerDialog(playerid, DIALOG_REPORTS, DIALOG_STYLE_LIST, "Active Reports", finalreportstr, "View", "Cancel");
		}
	}
	return 1;
}

CMD:report(playerid, params[])
{
	new now = gettime();
	if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /report [text]");
	if(PlayerInfo[playerid][pAdmin] >= 1) return SendClientMessage(playerid, COLOR_GREY, "Please do not /report as an administrator, thank you.");
	for(new i = 0; i < MAX_REPORTS; i++)
	{
		if(Report[i][rReporterID] == playerid && Report[i][rEnabled] == true) return SendClientMessage(playerid, COLOR_GREY, "You already have an on-going report, /cancelreport to cancel it.");
	}
	new text[128];
	if(sscanf(params, "s[128]", text)) return SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} /report [text]");

	if(now < (Reported[playerid] + 20))
	{
		format(tstr, sizeof(tstr), "You have to wait %d seconds before you can use /report again!", (Reported[playerid] + 20) - now);
		SendClientMessage(playerid, COLOR_GREY, tstr);
	}
	else
	{
		for(new i = 0; i < MAX_REPORTS; i++)
		{
			if(Report[i][rEnabled] == true) continue;

			Report[i][rEnabled] = true;
			format(Report[i][rText], 128, "%s", params);
			Report[i][rReporterID] = playerid;
			GetPlayerName(playerid, Report[i][rReporterName], MAX_PLAYER_NAME);
			
			format(tstr, sizeof(tstr), "Report (%d) from [%d]%s: %s", i, playerid, PlayerName(playerid), text);
			ABroadCast(COLOR_LIGHTYELLOW, tstr, 1);
			SendClientMessage(playerid, COLOR_YELLOW, "Your report message was sent to online administrators, thank you.");
			Reported[playerid] = gettime();
			areported[playerid] = true;
			reportscount++;
			break;
		}
	}	

	return 1;
}

CMD:reports(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] == 1 && ADuty[playerid] == false) return SendClientMessage(playerid, COLOR_GREY, "You are not on duty.");
	if(PlayerInfo[playerid][pAdmin] >= 1)
	{
		new finalreportstr[10000];
		for(new i = 0; i < MAX_REPORTS; i++)
		{
			if(Report[i][rEnabled] == true)
			{
				format(finalreportstr, sizeof(finalreportstr), "%s%d. %s (%d): %s\n", finalreportstr, i, Report[i][rReporterName], Report[i][rReporterID], Report[i][rText]);
			}	
		}	
		ShowPlayerDialog(playerid, DIALOG_REPORTS, DIALOG_STYLE_LIST, "Active Reports", finalreportstr, "View", "Cancel");
	}
	return 1;
}

CMD:ar(playerid, params[]) return cmd_acceptreport(playerid, params);

CMD:acceptreport(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 1) return AdmErrorMsg;
	if(PlayerInfo[playerid][pAdmin] == 1 && ADuty[playerid] == false) return SendClientMessage(playerid, COLOR_GREY, "You are not on duty.");
	new reportid;
	if(sscanf(params, "i", reportid)) return SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} /acceptreport [reportid]");
	if(Report[reportid][rEnabled] == false) return SendClientMessage(playerid, COLOR_GREY, "Invalid report ID.");
	else if(speakingtoadmin[Report[reportid][rReporterID]] != INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_GREY, "That player is currently speaking to an administrator.");

	format(tstr, sizeof(tstr), "%s has accepted %s's report (reportid: %d)", PlayerName(playerid), Report[reportid][rReporterName], reportid);
	ABroadCast(COLOR_LIGHTYELLOW, tstr, 1);
	SendClientMessage(playerid, COLOR_LIGHTYELLOW, "Use /ac to communicate with the player, /endac to end the chat session.");

	if(PlayerInfo[playerid][pAdmin] >= 2 && PlayerInfo[playerid][pStealthed] != 1)
	{
		format(tstr, sizeof(tstr), "%s has accepted your report, /ac to communicate.", PlayerName(playerid));
		SendClientMessage(Report[reportid][rReporterID], COLOR_GREY, tstr);
	}

	else if(PlayerInfo[playerid][pAdmin] == 1 || PlayerInfo[playerid][pStealthed] == 1)
	{
		SendClientMessage(Report[reportid][rReporterID], COLOR_GREY, "A silent admin has accepted your report, /ac to communicate.");
	}

	Report[reportid][rEnabled] = false;
	speakingtoplayer[playerid] = Report[reportid][rReporterID];
	speakingtoadmin[Report[reportid][rReporterID]] = playerid;
	Report[reportid][rReporterID] = INVALID_PLAYER_ID;
	reportscount--;

	return 1;
}

CMD:pr(playerid, params[]) return cmd_processreport(playerid, params);

CMD:processreport(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 1) return AdmErrorMsg;
	if(PlayerInfo[playerid][pAdmin] == 1 && ADuty[playerid] == false) return SendClientMessage(playerid, COLOR_GREY, "You are not on duty.");
	new reportid;
	if(sscanf(params, "i", reportid)) return SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} /processreport [reportid]");
	if(Report[reportid][rEnabled] == false) return SendClientMessage(playerid, COLOR_GREY, "Invalid report ID.");

	format(tstr, sizeof(tstr), "%s has processed %s's report (reportid: %d)", PlayerName(playerid), Report[reportid][rReporterName], reportid);
	ABroadCast(COLOR_LIGHTYELLOW, tstr, 1);

	Report[reportid][rEnabled] = false;
	if(0 <= speakingtoplayer[playerid] < MAX_PLAYERS) speakingtoadmin[speakingtoplayer[playerid]] = INVALID_PLAYER_ID;
	speakingtoplayer[playerid] = INVALID_PLAYER_ID;
	Report[reportid][rReporterID] = INVALID_PLAYER_ID;
	reportscount--;

	return 1;
}

CMD:ac(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] == 1 && ADuty[playerid] == false) return SendClientMessage(playerid, COLOR_GREY, "You are not on duty.");
	if(speakingtoplayer[playerid] == INVALID_PLAYER_ID && PlayerInfo[playerid][pAdmin] >= 1) return SendClientMessage(playerid, COLOR_GREY, "You are not speaking to any player.");
	if(speakingtoadmin[playerid] == INVALID_PLAYER_ID && PlayerInfo[playerid][pAdmin] < 1) return SendClientMessage(playerid, COLOR_GREY, "You are not speaking to any admin.");

	new text[128];
	if(sscanf(params, "s[128]", text)) return SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} /ac [message]");

	if(PlayerInfo[playerid][pAdmin] < 1)
	{
		for(new i = 0; i <= GetPlayerPoolSize(); i++)
		{
			if(speakingtoplayer[i] == playerid && PlayerInfo[i][pAdmin] >= 1 && speakingtoadmin[playerid] == i)
			{
				format(tstr, sizeof(tstr), "{008000}%s: %s", PlayerName(playerid), text);
				SendClientMessage(playerid, -1, tstr);
				SendClientMessage(i, -1, tstr);
			}
		}
	}
	if(PlayerInfo[playerid][pAdmin] == 1 || PlayerInfo[playerid][pStealthed] == 1)
	{
		for(new i = 0; i <= GetPlayerPoolSize(); i++)
		{
			if(speakingtoadmin[i] == playerid && speakingtoplayer[playerid] == i)
			{
				format(tstr, sizeof(tstr), "{008000}Silent Admin: %s", text);
				SendClientMessage(playerid, -1, tstr);
				SendClientMessage(i, -1, tstr);
			}
		}
	}
	if(PlayerInfo[playerid][pAdmin] > 1 && PlayerInfo[playerid][pStealthed] != 1)
	{
		for(new i = 0; i <= GetPlayerPoolSize(); i++)
		{
			if(speakingtoadmin[i] == playerid && speakingtoplayer[playerid] == i)
			{
				format(tstr, sizeof(tstr), "{008000}Admin %s: %s", PlayerName(playerid), text);
				SendClientMessage(playerid, -1, tstr);
				SendClientMessage(i, -1, tstr);
			}
		}
	}
	return 1;
}

CMD:startac(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 1) return AdmErrorMsg;
	if(PlayerInfo[playerid][pAdmin] == 1 && ADuty[playerid] == false) return SendClientMessage(playerid, COLOR_GREY, "You are not on duty.");
	if(speakingtoplayer[playerid] != INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_GREY, "You are already speaking to a player, /endac to end the chat session.");

	new targetid;
	if(sscanf(params, "i", targetid)) return SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} /startac [playerid]");
	if(PlayerInfo[targetid][pAdmin] >= 1) return SendClientMessage(playerid, COLOR_GREY, "Use /a for that.");
	if(speakingtoadmin[targetid] != INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_GREY, "That person is already speaking to an administrator.");
	speakingtoadmin[targetid] = playerid;
	speakingtoplayer[playerid] = targetid;
	format(tstr, sizeof(tstr), "You have started a chat session with %s, /ac to communicate, /endac to end the chat session.", PlayerName(targetid));
	SendClientMessage(playerid, COLOR_GREY, tstr);

	if(PlayerInfo[playerid][pAdmin] == 1 || PlayerInfo[playerid][pStealthed] == 1) SendClientMessage(targetid, COLOR_GREY, "A silent admin has started a chat session with you, /ac to communicate.");
	else if(PlayerInfo[playerid][pAdmin] > 1) 
	{
		format(tstr, sizeof(tstr), "Admin %s has started a chat session with you, /ac to communicate.", PlayerName(playerid));
		SendClientMessage(targetid, COLOR_GREY, tstr);
	}
	return 1;
}

CMD:endac(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 1) return AdmErrorMsg;
	if(PlayerInfo[playerid][pAdmin] == 1 && ADuty[playerid] == false) return SendClientMessage(playerid, COLOR_GREY, "You are not on duty.");
	if(speakingtoplayer[playerid] == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_GREY, "You are not speaking to any player.");

	format(tstr, sizeof(tstr), "You have ended the chat session with %s.", PlayerName(speakingtoplayer[playerid]));
	SendClientMessage(playerid, COLOR_GREY, tstr);
	if(PlayerInfo[playerid][pAdmin] == 1 || PlayerInfo[playerid][pStealthed] == 1) SendClientMessage(speakingtoplayer[playerid], COLOR_GREY, "The silent admin has ended the chat session with you.");
	else if(PlayerInfo[playerid][pAdmin] > 1) 
	{
		format(tstr, sizeof(tstr), "Admin %s has ended the chat session with you.", PlayerName(playerid));
		SendClientMessage(speakingtoplayer[playerid], COLOR_GREY, tstr);
	}

	if(0 <= speakingtoplayer[playerid] < MAX_PLAYERS) speakingtoadmin[speakingtoplayer[playerid]] = INVALID_PLAYER_ID;
	speakingtoplayer[playerid] = INVALID_PLAYER_ID;

	return 1;
}

CMD:cancelreport(playerid, params[])
{
	for(new i = 0; i < MAX_REPORTS; i++)
	{
		if(Report[i][rEnabled] == true && Report[i][rReporterID] == playerid)
		{
			SendClientMessage(playerid, COLOR_GREY, "You have cancelled your /report request.");
			Report[i][rEnabled] = false;
			Report[i][rReporterID] = INVALID_PLAYER_ID;
			reportscount--;
			break;
		}
		else if(i == 199 && Report[i][rEnabled] == false)
		{
			SendClientMessage(playerid, COLOR_GREY, "You got no on-going reports.");
			break;
		}
	}
	return 1;
}