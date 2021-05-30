//Needs cleaning up

static str[144], tmp[144], sendername[MAX_PLAYER_NAME+1], giveplayer[MAX_PLAYER_NAME+1];

public OnPlayerText(playerid, text[])
{
	if(gPlayerLogged[playerid] == 0)
	{
		SendClientMessage(playerid, COLOR_GREY, "You are not logged in.");
		return 0;
	}
	if(PlayerInfo[playerid][pMuted] == 1)
	{
		SendClientMessage(playerid, COLOR_GREY, "You can't speak while muted.");
		return 0;
	}
	if(pDeathState[playerid] == PLAYER_STATE_INHOSP || WatchingTV[playerid] == true || OnCCTV[playerid] != -1) return 0;
	// Gagged
	if(IsPlayerGagged(playerid)) {
		SendClientMessage(playerid, COLOR_GREY, "The gag in your mouth makes you unable to speak...");
		
		new gagStr[128];
		format(gagStr, sizeof(gagStr), "* %s tries to mumble something.", PlayerICName(playerid));
		SetPlayerChatBubble(playerid, gagStr, COLOR_PURPLE, 20.0, 15000);
		return 0;
	}
	// Watching PD CCTV
	if(watchingPDCCTV[playerid]) {
		SendClientMessage(playerid, COLOR_GREY, "You can't speak while watching the PD CCTV.");
		return 0;
	}
	//anti server ad
	if(PlayerInfo[playerid][pAdmin] < 1)
	{
		if(strfind(text, ":", true) != -1)
		{
			new i_numcount, i_period, i_pos;
			while(text[i_pos])
			{
				if('0' <= text[i_pos] <= '9') i_numcount++;
				else if(text[i_pos] == '.') i_period++;
				i_pos++;
			}
			if(i_numcount >= 8 && i_period >= 3)
			{
				format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s [%d] has attempted to advertise another server.", PlayerICName(playerid), playerid);
				ABroadCast(COLOR_WHITE, str, 1);
				format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: '%s'", text);
				ABroadCast(COLOR_GRAD3, str, 1);
				return 0;
			}
		}
	}
	if(MarriageCeremoney[playerid] > 0)
	{
		new idx;
		tmp = strtok(text, idx);
		if((strcmp("yes", tmp, true, strlen(tmp)) == 0) && (strlen(tmp) == strlen("yes")))
		{
			if(GotProposedBy[playerid] < 999)
			{
				if(IsPlayerConnected(GotProposedBy[playerid]))
				{
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					GetPlayerICName(GotProposedBy[playerid], giveplayer, sizeof(giveplayer));
					format(str, sizeof(str), "Priest: %s do you take %s as your lovely Wife? (type 'yes', anything else will reject the Marriage).", giveplayer, sendername);
					SendClientMessage(GotProposedBy[playerid], COLOR_WHITE, str);
					MarriageCeremoney[GotProposedBy[playerid]] = 1;
					MarriageCeremoney[playerid] = 0;
					GotProposedBy[playerid] = 999;
				}
				else
				{
					MarriageCeremoney[playerid] = 0;
					GotProposedBy[playerid] = 999;
					return 1;
				}
			}
			else if(ProposedTo[playerid] < 999)
			{
				if(IsPlayerConnected(ProposedTo[playerid]))
				{
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					GetPlayerICName(ProposedTo[playerid], giveplayer, sizeof(giveplayer));
					if(PlayerInfo[playerid][pSex] == 1 && PlayerInfo[ProposedTo[playerid]][pSex] == 2)
					{
						format(str, sizeof(str), "Priest: %s and %s i pronounce you now...Husband & Wife, you may kiss the Bride.", sendername, giveplayer);
						SendClientMessage(playerid, COLOR_WHITE, str);
						format(str, sizeof(str), "Priest: %s and %s i pronounce you now...Husband & Wife, you may kiss the Groom.", giveplayer, sendername);
						SendClientMessage(ProposedTo[playerid], COLOR_WHITE, str);
						format(str, sizeof(str), "Marriage News: We have a new lovely couple, %s & %s have been married at the church in Jefferson.", sendername, giveplayer);
						OOCNews(COLOR_WHITE, str);
					}
					else if(PlayerInfo[playerid][pSex] == 1 && PlayerInfo[ProposedTo[playerid]][pSex] == 1)
					{
						format(str, sizeof(str), "Priest: %s and %s i pronounce you now...Husband & Husband, you may kiss the Groom.", sendername, giveplayer);
						SendClientMessage(playerid, COLOR_WHITE, str);
						format(str, sizeof(str), "Priest: %s and %s i pronounce you now...Husband & Husband, you may kiss the Groom.", giveplayer, sendername);
						SendClientMessage(ProposedTo[playerid], COLOR_WHITE, str);
						format(str, sizeof(str), "Marriage News: We have a new Gay couple, %s & %s have been married at the church in Jefferson.", sendername, giveplayer);
						OOCNews(COLOR_WHITE, str);
						Accent[playerid] = "Gay";
						Accent[ProposedTo[playerid]] = "Gay";
					}
					else if(PlayerInfo[playerid][pSex] == 2 && PlayerInfo[ProposedTo[playerid]][pSex] == 2)
					{
						format(str, sizeof(str), "Priest: %s and %s i pronounce you now...Wife & Wife, you may kiss the Bride.", sendername, giveplayer);
						SendClientMessage(playerid, COLOR_WHITE, str);
						format(str, sizeof(str), "Priest: %s and %s i pronounce you now...Wife & Wife, you may kiss the Bride.", giveplayer, sendername);
						SendClientMessage(ProposedTo[playerid], COLOR_WHITE, str);
						format(str, sizeof(str), "Marriage News: We have a new Lesbian couple, %s & %s have been married at the church in Jefferson.", sendername, giveplayer);
						OOCNews(COLOR_WHITE, str);
					}
					MarriageCeremoney[ProposedTo[playerid]] = 0;
					MarriageCeremoney[playerid] = 0;
					format(str, sizeof(str), "%s", sendername);
					strmid(PlayerInfo[ProposedTo[playerid]][pMarriedTo], str, 0, strlen(str), 255);
					format(str, sizeof(str), "%s", giveplayer);
					strmid(PlayerInfo[playerid][pMarriedTo], str, 0, strlen(str), 255);
					GiveMoney(playerid, -100000);
					PlayerInfo[playerid][pMarried] = 1;
					PlayerInfo[ProposedTo[playerid]][pMarried] = 1;
					ProposedTo[playerid] = 999;
					MarriageCeremoney[playerid] = 0;
					return 1;
				}
				else
				{
					MarriageCeremoney[playerid] = 0;
					ProposedTo[playerid] = 999;
					return 1;
				}
			}
		}
		else
		{
			if(GotProposedBy[playerid] < 999)
			{
				if(IsPlayerConnected(GotProposedBy[playerid]))
				{
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					GetPlayerICName(GotProposedBy[playerid], giveplayer, sizeof(giveplayer));
					format(str, sizeof(str), "* You didn't want to Marry %s, no 'yes' was said.", giveplayer);
					SendClientMessage(playerid, COLOR_YELLOW, str);
					format(str, sizeof(str), "* %s did't want to Marry you, no 'yes' was said.", sendername);
					SendClientMessage(GotProposedBy[playerid], COLOR_YELLOW, str);
					return 1;
				}
				else
				{
					MarriageCeremoney[playerid] = 0;
					GotProposedBy[playerid] = 999;
					return 1;
				}
			}
			else if(ProposedTo[playerid] < 999)
			{
				if(IsPlayerConnected(ProposedTo[playerid]))
				{
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					GetPlayerICName(ProposedTo[playerid], giveplayer, sizeof(giveplayer));
					format(str, sizeof(str), "* You didn't want to Marry %s, no 'yes' was said.", giveplayer);
					SendClientMessage(playerid, COLOR_YELLOW, str);
					format(str, sizeof(str), "* %s didn't want to Marry you, no 'yes' was said.", sendername);
					SendClientMessage(ProposedTo[playerid], COLOR_YELLOW, str);
					return 1;
				}
				else
				{
					MarriageCeremoney[playerid] = 0;
					ProposedTo[playerid] = 999;
					return 1;
				}
			}
		}
		return 1;
	}
	if(SAPDClearing[playerid] != 0)
	{
		if(SAPDClearing[playerid] == 1)
		{
			new idx;
			tmp = strtok(text, idx);
			new playa;
			playa = ReturnUser(tmp);
			if(IsPlayerConnected(playa))
			{
				if(playa != INVALID_PLAYER_ID)
				{
					if(PlayerInfo[playa][pWantedLevel] > 0)
					{
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						strmid(giveplayer, PlayerICName(playa), 0, MAX_PLAYER_NAME);
						PlayerInfo[playa][pWantedLevel] = 0;
						SetPlayerToTeamColor(playa);
						SAPDClearing[playerid] = 0;
						TogglePlayerControllable(playerid, true);
						format(str, sizeof(str), "* You cleared the records and wanted points of %s.", giveplayer);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* %s %s has cleared your records and wanted points.", GetPlayerRank(playerid), sendername);
						SendClientMessage(playa, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "HQ: %s %s has cleared %s's records and wanted points.", GetPlayerRank(playerid), sendername, giveplayer);
						SendDepartmentMessage(TEAM_BLUE_COLOR, str);
						isBOLO[playa] = 0;
						new query[128];
						mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `newcrimes` SET `CrimeEnabled` = 0 WHERE `CrimePID` = '%d'", PlayerInfo[playa][pID]);
						mysql_pquery(sqlGameConnection, query);
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "That player is currently not wanted.");
						SAPDClearing[playerid] = 0;
						TogglePlayerControllable(playerid, true);
						return 0;
					}
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Invalid ID.");
				SAPDClearing[playerid] = 0;
				TogglePlayerControllable(playerid, true);
				return 0;
			}
			return 0;
		}
		if(SAPDClearing[playerid] == 2)
		{
			new idx;
			tmp = strtok(text, idx);
			new playa;
			playa = ReturnUser(tmp);
			if(IsPlayerConnected(playa))
			{
				if(playa != INVALID_PLAYER_ID)
				{
					if(PlayerInfo[playa][pJailed] > 0)
					{
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						strmid(giveplayer, PlayerICName(playa), 0, MAX_PLAYER_NAME);
						PlayerInfo[playa][pWantedLevel] = 0;
						SetPlayerToTeamColor(playa);
						SAPDClearing[playerid] = 0;
						isBOLO[playa] = 0;
						new query[128];
						mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `newcrimes` SET `CrimeEnabled` = 0 WHERE `CrimePID` = '%d'", PlayerInfo[playa][pID]);
						mysql_pquery(sqlGameConnection, query);
						if(PlayerInfo[playa][pAdminJailed] == 1)
						{
							if(PlayerInfo[playa][pJailed] >= 1)
							{
								format(str, sizeof(str), "This player was jailed by an admin, you may not release them.");
								SendClientMessage(playerid, COLOR_GREY, str);
								format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has attempted to release %s from admin jail/prison.", sendername, giveplayer);
								ABroadCast(COLOR_WHITE, str, 1);
								SAPDClearing[playerid] = 0;
								TogglePlayerControllable(playerid, true);
								return 0;
							}
						}
						else if(PlayerInfo[playa][pJailed] == 1)
						{
							SetPlayerPos(playa, -2204.19, 842.05, 9999.26);
							SetPlayerInterior(playa, 0);
							SetPlayerVirtualWorld(playa, 0);
							format(str, sizeof(str), "* You have cleared %s's Wanted Points and released them from Jail.", giveplayer);
							SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
							format(str, sizeof(str), "* %s %s has cleared your Wanted Points and released you from Jail.", GetPlayerRank(playerid), sendername);
							SendClientMessage(playa, COLOR_LIGHTBLUE, str);
							format(str, sizeof(str), "HQ: %s %s has cleared %s's Wanted Points and released them from Jail.", GetPlayerRank(playerid), sendername, giveplayer);
							SendDepartmentMessage(TEAM_BLUE_COLOR, str);
							displayCenterHUDInfo(playa,  "~g~Released~n~~w~Try to be a better citizen.", 8); //display for 8 seconds

						}
						else if(PlayerInfo[playa][pJailed] == 2) //Adjusted - prison system
						{
							SetPlayerPos(playa, 202.2571, 1865.2544, 13.1406);
							SetPlayerFacingAngle(playa, 265.4817);
							Timer_FreezePlayer(playa, FREEZE, ENTER_FREEZE);
							SetPlayerSkin(playa, PlayerInfo[playa][pModel]);
							PlayerInfo[playa][pJailnumber] = 0;
							UpdateDynamic3DTextLabelText(PNumber3D[playa], COLOR_WHITE, "");
							DestroyDynamic3DTextLabel(PNumber3D[playa]);
							format(str, sizeof(str), "* You have cleared %s's Wanted Points and released them from Prison.", giveplayer);
							SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
							format(str, sizeof(str), "* %s %s has cleared your Wanted Points and released you from Prison.", GetPlayerRank(playerid), sendername);
							SendClientMessage(playa, COLOR_LIGHTBLUE, str);
							format(str, sizeof(str), "HQ: %s %s has cleared %s's Wanted Points and released them from Prison.", GetPlayerRank(playerid), sendername, giveplayer);
							SendDepartmentMessage(TEAM_BLUE_COLOR, str);
							displayCenterHUDInfo(playa,  "~g~Released~n~~w~Try to be a better citizen.", 8); //display for 8 seconds
							SetPlayerPos(playa, -2204.19, 842.05, 9999.26);
							SetPlayerInterior(playa, 0);
							SetPlayerVirtualWorld(playa, 0);
						}
						PlayerInfo[playa][pJailed] = 0;
						PlayerInfo[playa][pJailTime] = 0;
						TogglePlayerControllable(playerid, true);
						SetPlayerToTeamColor(playa);
						SetCameraBehindPlayer(playerid);
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "That player is currently not in jail.");
						SAPDClearing[playerid] = 0;
						TogglePlayerControllable(playerid, true);
						return 0;
					}
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Invalid ID.");
				SAPDClearing[playerid] = 0;
				TogglePlayerControllable(playerid, true);
				return 0;
			}
			return 0;
		}
	}
	if(CallLawyer[playerid] == 111)
	{
		new idx;
		tmp = strtok(text, idx);
		if((strcmp("yes", tmp, true, strlen(tmp)) == 0) && (strlen(tmp) == strlen("yes")))
		{
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			format(str, sizeof(str), "** %s is in Jail, and needs a Lawyer. Go to the Police Station.", sendername);
			SendJobMessage(2, TEAM_AZTECAS_COLOR, str);
			SendClientMessage(playerid, COLOR_LIGHTRED, "A message has been sent to all available Lawyers, please wait.");
			WantLawyer[playerid] = 0;
			CallLawyer[playerid] = 0;
			return 0;
		}
		else
		{
			SendClientMessage(playerid, COLOR_LIGHTRED, "There is no Lawyer available to you anymore, Jail Time started.");
			WantLawyer[playerid] = 0;
			CallLawyer[playerid] = 0;
			return 0;
		}
	}
	if(TalkingLive[playerid] != 255)
	{
		strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
		foreach( new i: Player )
		{
			if(PlayerInfo[i][pWire] == 1 && !CheckForJammer(playerid) && !CheckForJammer(i))
			{
				new Float: x[3];
				GetPlayerPos(playerid, x[0], x[1], x[2]);
				if(IsPlayerInRangeOfPoint(i, 20.0, x[0], x[1], x[2]))
				{
					if(i == playerid)
					{
						format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s: %s", PlayerOOCName(i), text);
					}
					else
					{
						format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s's surroundings: %s", PlayerOOCName(i), text);
					}
					if(PlayerInfo[i][pMember] != 2) // if player isn't FBI, send to both FBI and S9
					{
						SendBuggedMessage(COLOR_ORANGE, str);
						SendS9BuggedMessage(COLOR_ORANGE, str);
					}	
					else SendS9BuggedMessage(COLOR_ORANGE, str); // if player is FBI, send to S9 only, this is to avoid FBI figuring out
					// 																							that s9 bugged them
				}
			}
		}
		if(PlayerInfo[playerid][pMember] == 9 || PlayerInfo[playerid][pLeader] == 9)
		{
			format(str, sizeof(str), "Reporter %s: %s", sendername, text);
			OOCNews(COLOR_LIGHTGREEN, str);
		}
		else
		{
			format(str, sizeof(str), "%s: %s", LiveName[playerid], text);
			OOCNews(COLOR_LIGHTGREEN, str);
		}
		return 0;
	}
	if(Mobile[playerid] != 999)
	{
		new idx;
		tmp = strtok(text, idx);
		foreach( new i: Player )
		{
			if(PlayerInfo[i][pWire] == 1 && !CheckForJammer(playerid) && !CheckForJammer(i))
			{
				new Float: x[3];
				GetPlayerPos(playerid, x[0], x[1], x[2]);
				if(IsPlayerInRangeOfPoint(i, 20.0, x[0], x[1], x[2]))
				{
					if(i == playerid)
					{
						if(strcmp(Accent[i], "none", true, 3)) format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s (cellphone): [%s Accent] %s", PlayerOOCName(i), Accent[i], text);
						else format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s (cellphone): %s", PlayerOOCName(i), text);
					}
					else
					{
						if(strcmp(Accent[playerid], "none", true, 3)) format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s's surroundings: [%s Accent] %s", PlayerOOCName(i), Accent[playerid], text);
						else format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s's surroundings: %s", PlayerOOCName(i), text);
					}
					if(PlayerInfo[i][pMember] != 2) // if player isn't FBI, send to both FBI and S9
					{
						SendBuggedMessage(COLOR_ORANGE, str);
						SendS9BuggedMessage(COLOR_ORANGE, str);
					}	
					else SendS9BuggedMessage(COLOR_ORANGE, str); // if player is FBI, send to S9 only, this is to avoid FBI figuring out
					// 																							that s9 bugged them
				}
			}
		}
		format(str, sizeof(str), "%s says (cellphone): ", PlayerICName(playerid));
		if(!IsPlayerInAnyVehicle(playerid))
		{
			NewProxDetector(playerid, 15.0, str, text, COLOR_FADE1);
		}
		else
		{
			if(VehicleHasWindows(GetPlayerVehicleID(playerid)))
			{
				if(windowsDown[GetPlayerVehicleID(playerid)] == false)
				{
					if(strcmp(Accent[playerid], "none", true, 3))format(str, sizeof(str), "(vehicle) %s says (cellphone): [%s Accent] %s", PlayerICName(playerid), Accent[playerid], text);
					else format(str, sizeof(str), "(vehicle) %s says (cellphone): %s", PlayerICName(playerid), text);
					CarMessage(playerid, COLOR_GREY, str);
				}
				else
				{
					NewProxDetector(playerid, 15.0, str, text, COLOR_FADE1);
				}
			}
			else
			{
				NewProxDetector(playerid, 15.0, str, text, COLOR_FADE1);
			}
		}
		if(Mobile[playerid] == 914)
		{
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Paramedic Dispatch: Sorry I don't understand.");
				return 0;
			}
			SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Paramedic Dispatch: We have alerted all units in the area.");
			if(callerID[playerid] == false)format(str, sizeof(str), "911 Call; Paramedic - Caller: %s", PlayerICName(playerid));
			else format(str, sizeof(str), "911 Call; Paramedic - Caller: Unknown (Blocked Caller ID)");
			SendParaMessage(COLOR_DOCTOR, str);
			format(str, sizeof(str), "Situation: %s", text);
			SendParaMessage(COLOR_DOCTOR, str);
			SendClientMessage(playerid, COLOR_GRAD2, "They hung up...");
			Mobile[playerid] = 999;
			SetAllFiremanCheckpoint(playerid);
			detachPhoneFromPlayer(playerid);
			return 0;
		}
		if(Mobile[playerid] == 916)
		{
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Firemen: Hello? Are you there? I couldn't catch that, could you repeat?");
				return 0;
			}
			SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Firemen: We'll send someone out immediately.");
			if(callerID[playerid] == false)format(str, sizeof(str), "911 Call; Fire - Caller: %s", PlayerICName(playerid));
			else format(str, sizeof(str), "911 Call; Fire - Caller: Unknown (Blocked Caller ID)");
			SendParaMessage(COLOR_DOCTOR, str);
			format(str, sizeof(str), "Situation: %s", text);
			SendParaMessage(COLOR_DOCTOR, str);
			SendClientMessage(playerid, COLOR_GRAD2, "They hung up...");
			Mobile[playerid] = 999;
			SetAllFiremanCheckpoint(playerid);
			detachPhoneFromPlayer(playerid);
			return 0;
		}
		if(Mobile[playerid] == 913)
		{
			new zone[MAX_ZONE_NAME];
			GetPlayer2DZone(playerid, zone, MAX_ZONE_NAME);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Police Dispatch: Sorry I don't understand.");
				return 0;
			}
			if((strcmp("no", tmp, true, strlen(tmp)) == 0) && (strlen(tmp) == strlen("no")))
			{
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Police Dispatch: We have alerted all units in the area, a police officer should be with you shortly.");
				format(str, sizeof(str), "|____________Emergency call____________|");
				SendCopMessage(TCOLOR_NAVYBLUE, str);
				if(callerID[playerid] == true)format(str, sizeof(str), "Caller: Unknown, Phone: Unknown, Trace: %s", zone);
				else format(str, sizeof(str), "Caller: %s, Phone: %d, Trace: %s", PlayerICName(playerid), PlayerInfo[playerid][pPnumber], zone);
				SendCopMessage(TCOLOR_NAVYBLUE, str);
				format(str, sizeof(str), "Services Required: Police");
				SendCopMessage(TCOLOR_NAVYBLUE, str);
				format(str, sizeof(str), "Location: %s", text);
				SendCopMessage(TCOLOR_NAVYBLUE, str);
				format(str, sizeof(str), "Situation: %s", CallDescription[playerid]);
				SendCopMessage(TCOLOR_NAVYBLUE, str);
				SendClientMessage(playerid, COLOR_GRAD2, "They hung up...");
				Mobile[playerid] = 999;
				if(PlayerInfo[playerid][pWantedLevel] < 6)
				{
					Called911[playerid] = 1;
					SetAllCopCheckpoint(playerid);
				}
				detachPhoneFromPlayer(playerid);
				return 0;
			}
			SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Police Dispatch: We have alerted all units in the area, a police officer should be with you shortly.");
			format(str, sizeof(str), "|____________Emergency call____________|");
			SendCopMessage(TCOLOR_NAVYBLUE, str);
			if(callerID[playerid] == true)	format(str, sizeof(str), "Caller: Unknown, Phone: Unknown, Trace: %s", zone);
			else format(str, sizeof(str), "Caller: %s, Phone: %d, Trace: %s", PlayerICName(playerid), PlayerInfo[playerid][pPnumber], zone);
			SendCopMessage(TCOLOR_NAVYBLUE, str);
			format(str, sizeof(str), "Services Required: Police");
			SendCopMessage(TCOLOR_NAVYBLUE, str);
			format(str, sizeof(str), "Location: %s", text);
			SendCopMessage(TCOLOR_NAVYBLUE, str);
			format(str, sizeof(str), "Situation: %s", CallDescription[playerid]);
			SendCopMessage(TCOLOR_NAVYBLUE, str);
			SendClientMessage(playerid, COLOR_GRAD2, "They hung up...");
			Mobile[playerid] = 999;
			PlayerCalledPD[playerid] = 1;
			if(PlayerInfo[playerid][pWantedLevel] != 6)
			{
				Called911[playerid] = 1;
				SetAllCopCheckpoint(playerid);
			}
			detachPhoneFromPlayer(playerid);
			return 0;
		}
		if(Mobile[playerid] == 912)
		{
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Police Dispatch: Sorry I don't understand.");
				return 0;
			}
			strmid(CallDescription[playerid], text, 0, strlen(text), 255);
			SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Police Dispatch: What's your location?");
			Mobile[playerid] = 913;
			return 0;
		}
		if(Mobile[playerid] == 63971)
		{
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Operator: Sorry, I didn't quite understand that. Please state your query.");
				return 0;
			}
			else
			{
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Operator: Thank you, we'll be in contact as soon as possible!");
				format(str, sizeof(str), "[SAN Hotline - Ph: %d (( %s ))] %s", PlayerInfo[playerid][pPnumber], PlayerICName(playerid), text);
				SendHotlineMessage(COLOR_YELLOW, str);
				SendClientMessage(playerid, COLOR_GRAD2, "They hung up...");
				Mobile[playerid] = 999;
				detachPhoneFromPlayer(playerid);
			}
		}
		if(Mobile[playerid] == 911)
		{
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) 911 Operator: Sorry I don't understand, police, paramedic or fire?");
				return 0;
			}
			else if(strfind(tmp, "police", true, 0) != -1)
			{
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) 911 Operator: I am patching you to Police HQ, please hold.");
				Mobile[playerid] = 912;
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Police Dispatch: Please give me a short description of the crime.");
				return 0;
			}
			else if(strfind(tmp, "paramedic", true, 0) != -1)
			{
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) 911 Operator: I am patching you to Paramedic HQ, please hold.");
				Mobile[playerid] = 914;
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Paramedic Dispatch: Please give me a short description of the Incident.");
				return 0;
			}
			else if(strfind(tmp, "fire", true, 0) != -1)
			{
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) 911 Operator: I am patching you to the Fire Department, please hold.");
				Mobile[playerid] = 916;
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Firemen: Please provide a short description of the incident, is there a fire? Where is it?");
				return 0;
			}
			else
			{
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) 911 Operator: Sorry I don't understand, police, paramedic or fire?");
				return 0;
			}
		}

		if(Mobile[playerid] == 420)
		{
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Taxi Company: Sorry I don't understand, what type of service you'd like? premium, normal or fast?");
				return 0;
			}
			else if(strfind(tmp, "premium", true, 0) != -1)
			{
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Taxi Company: Premium services, all right. Please tell us your location?");
				Mobile[playerid] = 421;
				return 0;
			}
			else if(strfind(tmp, "normal", true, 0) != -1)
			{
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Taxi Company: Normal services, all right. Please tell us your location?");
				Mobile[playerid] = 422;
				return 0;
			}
			else if(strfind(tmp, "fast", true, 0) != -1)
			{
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Taxi Company: Fast services, all right. Please tell us your location?");
				Mobile[playerid] = 423;
				return 0;
			}
			else
			{
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Taxi Company: Sorry I don't understand, premium, normal or fast??");
				return 0;
			}
		}
        if(Mobile[playerid] == 421 || Mobile[playerid] ==  422 || Mobile[playerid] == 423)
		{
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Taxi Company: Sorry, I didn't understand. What's your location again?");
				return 0;
			}
			else
			{
				SendClientMessage(playerid, COLOR_YELLOW, "(cellphone) Taxi Company: Thank you, a unit will be sent as soon as possible!");
				new service[120];
				switch(Mobile[playerid])
				{
					case 421: service = "Premium";
					case 422: service = "Normal";
					case 423: service = "Fast";
				}
				format(str, sizeof(str), "[Taxi Hotline] %s [Ph: %d] is requesting a %s service, location: %s.", PlayerICName(playerid), PlayerInfo[playerid][pPnumber], service, text);
				SendTaxiFactionMessage(COLOR_YELLOW, str);
				SendClientMessage(playerid, COLOR_GRAD2, "They hung up...");
				Mobile[playerid] = 999;
				detachPhoneFromPlayer(playerid);
				HangupCall( playerid );
			}
		}	
		
		if(IsPlayerConnected(Mobile[playerid]))
		{
			if(Mobile[Mobile[playerid]] == playerid)
			{
				if(!strmatch(Accent[playerid], "none"))
				{
					if(usePayphone[playerid] == -1)format(str, sizeof(str), "(cellphone) %s says: [%s Accent] %s", CallerIDName(playerid, callerID[playerid]), Accent[playerid], text);
					else format(str, sizeof(str), "Stranger says (cellphone): [%s Accent] %s", Accent[playerid], text);
				}
				else
				{
					if(usePayphone[playerid] == -1)format(str, sizeof(str), "(cellphone) %s says: %s", CallerIDName(playerid, callerID[playerid]), text);
					else format(str, sizeof(str), "Stranger says (cellphone): %s", text);
				}
				if(PlayerInfo[Mobile[playerid]][pSpeakPhone] == 1)
				{
					if(windowsDown[GetPlayerVehicleID(playerid)] == false)
					{
						ProxDetector(20.0, Mobile[playerid], str, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW);
					}
					else
					{
						CarMessage(playerid, COLOR_GREY, str);
					}
				}
				else
				{
					SendClientMessage(Mobile[playerid], COLOR_YELLOW, str);
				}
			}
		}
		else
		{
			if(Mobile[playerid] != 999)
			{
				SendClientMessage(playerid, COLOR_GREY, "There's nobody there...");
			}
		}
		return 0;
	}
	else
	{
		foreach( new i: Player )
		{
			if(PlayerInfo[i][pWire] == 1 && !CheckForJammer(playerid) && !CheckForJammer(i))
			{
				new Float: x[3];
				GetPlayerPos(playerid, x[0], x[1], x[2]);
				if(IsPlayerInRangeOfPoint(i, 20.0, x[0], x[1], x[2]))
				{
					if(i == playerid)
					{
						if(strcmp(Accent[i], "none", true, 3)) format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s: [%s Accent] %s", PlayerOOCName(i), Accent[i], text);
						else format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s: %s", PlayerOOCName(i), text);
					}
					else
					{
						if(strcmp(Accent[playerid], "none", true, 3)) format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s's surroundings: [%s Accent] %s", PlayerOOCName(i), Accent[playerid], text);
						else format(str, sizeof(str), "{8D8DFF}(BUGGED){AFAFAF} %s's surroundings: %s", PlayerOOCName(i), text);
					}
					if(PlayerInfo[i][pMember] != 2) // if player isn't FBI, send to both FBI and S9
					{
						SendBuggedMessage(COLOR_ORANGE, str);
						SendS9BuggedMessage(COLOR_ORANGE, str);
					}	
					else SendS9BuggedMessage(COLOR_ORANGE, str); // if player is FBI, send to S9 only, this is to avoid FBI figuring out
					// 																							that s9 bugged them
				}
			}
		}
		if(!IsPlayerInAnyVehicle(playerid))
		{
			if(strcmp(Accent[playerid], "none", true, 3)) format(str, sizeof(str), "%s says: [%s Accent] ", PlayerICName(playerid), Accent[playerid]);
			else format(str, sizeof(str), "%s says: ", PlayerICName(playerid));
		}
		else
		{
			if(VehicleHasWindows(GetPlayerVehicleID(playerid)))
			{
				if(windowsDown[GetPlayerVehicleID(playerid)] == false)
				{
					if(strcmp(Accent[playerid], "none", true, 3))format(str, sizeof(str), "{FFFF66}(vehicle) %s says: [%s Accent] %s", PlayerICName(playerid), Accent[playerid], text);
					else format(str, sizeof(str), "{FFFF66}(vehicle) %s says: %s", PlayerICName(playerid), text);
				}
				else
				{
					if(strcmp(Accent[playerid], "none", true, 3))format(str, sizeof(str), "%s says: [%s Accent] ", PlayerICName(playerid), Accent[playerid]);
					else format(str, sizeof(str), "%s says: ", PlayerICName(playerid));
				}
			}
			else
			{
				if(strcmp(Accent[playerid], "none", true, 3))format(str, sizeof(str), "%s says: [%s Accent] ", PlayerICName(playerid), Accent[playerid]);
				else format(str, sizeof(str), "%s says: ", PlayerICName(playerid));
			}
		}
	}
	if(!IsPlayerInAnyVehicle(playerid))
	{
		NewProxDetector(playerid, 15.0, str, text, COLOR_FADE1);
	}
	else
	{
		if(windowsDown[GetPlayerVehicleID(playerid)] == false)
		{
			if(!VehicleHasWindows(GetPlayerVehicleID(playerid)))
			{
				 NewProxDetector(playerid, 15.0, str, text, COLOR_FADE1);
			}
			else
			{

				CarMessage(playerid, COLOR_GREY, str);
			}
		}
		else
		{
			 NewProxDetector(playerid, 15.0, str, text, COLOR_FADE1);
		}
	}
	//random animation
	if(autoChat[playerid] > 0)
	{
		//printf("anim is %d", GetPlayerAnimationIndex(playerid));
		if(GetPlayerAnimationIndex(playerid) == 1189 || GetPlayerAnimationIndex(playerid) == 1186 || GetPlayerAnimationIndex(playerid) == 1275)   //idle animation
		{
			new strlength = strlen(text);
			new animLength = strlength; //calculate time based on str input length
			if(animLength < 5)   //short word
			{
				autoChatTime[playerid] = 2;
			}
			else if(animLength > 10 && animLength < 15)	 //medium word
			{
				autoChatTime[playerid] = 4;
			}
			else	 //long words
			{
				autoChatTime[playerid] = 5;
			}
			new rand = random(6);
			switch(rand)
			{
				case 0:
				{
					ApplyAnimationEx(playerid, "PED", "IDLE_CHAT", 4.1, 1, 0, 0, 1, 1, 0);
				}
				case 1:
				{
					ApplyAnimationEx(playerid, "GANGS", "prtial_gngtlkB", 4.1, 1, 0, 0, 1, 1, 0);
				}
				case 2:
				{
					ApplyAnimationEx(playerid, "GANGS", "prtial_gngtlkE", 4.1, 1, 0, 0, 1, 1, 0);
				}
				case 3:
				{
					ApplyAnimationEx(playerid, "GANGS", "prtial_gngtlkF", 4.1, 1, 0, 0, 1, 1, 0);
				}
				case 4:
				{
					ApplyAnimationEx(playerid, "GANGS", "prtial_gngtlkG", 4.1, 1, 0, 0, 1, 1, 0);
				}
				case 5:
				{
					ApplyAnimationEx(playerid, "GANGS", "prtial_gngtlkH", 4.1, 1, 0, 0, 1, 1, 0);
				}
			}
		}
	}
	if(pCommandUsed[playerid] == false)
	{
		KillTimer(AFK_Timer[playerid]);
		pCommandUsed[playerid] = true;
		AFK_Timer[playerid] = SetTimerEx("Timer_stopCMD", 600000, false, "i", playerid); // 10 minutes
	}
	if(checkingBackground[playerid] == 1) {
		if(!IsPlayerInRangeOfPoint(playerid, 5.0, -2206.77, 835.14, 9999.28)) {
			checkingBackground[playerid] = 0;
			return 0;
		}

		// Cancel the background check
		if(!strcmp(text, "no", true) || !strcmp(text, "no.", true)) {
			SendClientMessage(playerid, -1, "Receptionist says: Have a nice day.");
			checkingBackground[playerid] = 0;
			return 0;
		}
		// Start background check (text = name)
		else {
			// Format name
			new checkName[MAX_PLAYER_NAME + 1];
			format(checkName, sizeof(checkName), "%s", text);

			if(strfind(checkName, " ", true) != -1) {
				strmid(checkName, str_replace(' ', '_', checkName), 0, MAX_PLAYER_NAME);
			}

			// Do query for SQL ID
			new query[128], rows, fields, Cache:result;
			mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `players` WHERE name = '%e' LIMIT 1", checkName);
			result = mysql_query(sqlGameConnection, query);
			cache_get_data(rows, fields, sqlGameConnection);
			
			if(!rows) {
				SendClientMessage(playerid, -1, "Receptionist says: Sorry, I have no criminal records of this person.");
				checkingBackground[playerid] = 0;
				cache_delete(Cache:result, sqlGameConnection);
				return 0;
			}
			else
			{
				if(GetMoney(playerid) < 100000) {
					SendClientMessage(playerid, COLOR_GREY, "You cannot afford to pay for the public background check.");
					cache_delete(Cache:result, sqlGameConnection);
					return 0;
				}

				cache_get_field_content(0, "Name", checkingName[playerid]);
				checkingID[playerid] = cache_get_field_content_int(0, "ID");
				checkingBackground[playerid] = 0;

				GiveMoney(playerid, -100000);

				new bgStr[128];
				format(bgStr, sizeof(bgStr), "* The receptionist hands you a copy of %s's criminal record. (/checkrecord)", checkingName[playerid]);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, bgStr);

				cache_delete(Cache:result, sqlGameConnection);
				return 0;
			}
		}

	}
	return 0;
}