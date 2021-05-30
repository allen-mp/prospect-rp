Timer:BPLgateclose(playerid)
{
	SetDynamicObjectRot(BPLG1, 0, 270, -295);
	SetDynamicObjectRot(BPLG2, 0, 90, 0);
	return 1;
}
Timer:UseBM(playerid)
{
	if(!IsPlayerConnected(playerid)) return 0;
	if(UseBMTimer[playerid])
	{
		UseBMTimer[playerid] = 0;
		BMPurchased[playerid] = 0;
	}
	return 1;
}
/*==============================================================================
This needs converting to y_dialogs, and CLEANING THE FUCK UP -- dy1zan
==============================================================================*/
static str[144], sendername[MAX_PLAYER_NAME+1], giveplayer[MAX_PLAYER_NAME+1];
public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_MDC_MAIN)
	{
		if(!response) {
			LogOffMDC(playerid);
			return 1;
		}

		if(GetMDCAccessType(playerid) == MDC_ACCESS_NONE)
			return SendMDCMessage(playerid, "This function is not available to you.");

		switch(listitem)
		{
			case 0: return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+1, DIALOG_STYLE_INPUT, "{2641FE}Mobile Data Computer - Lookup function", "{FFFFFF}Enter a name to run a search.\nPlease note that this will only display people who are registered on the police database.\n((Please not that you will have to use underscores '_' while searching for names.", "Search", "Return");
			case 1: {
				if(GetMDCAccessType(playerid) != MDC_ACCESS_FULL)
					return SendMDCMessage(playerid, "This function is not available to you.");

				return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+6, DIALOG_STYLE_LIST, "{2641FE}Mobile Data Computer - BOLO", "View BOLOs\nAdd BOLO\nRemove BOLO", "Okay", "Close");
			}
			case 2:
			{
				if(GetMDCAccessType(playerid) != MDC_ACCESS_FULL)
					return SendMDCMessage(playerid, "This function is not available to you.");

				if(PlayerInfo[playerid][pMember] == 5)
				{
					AdmErrorMsg;
					return MDC_MAIN;
				}
				new query[128], Name[26], GivenBy[26], rows, fields, cstr[5000], Date[50], Cache:result, IsTaken;
				mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `weaponslog` ORDER BY `ID` DESC LIMIT 50");
				result = mysql_query(sqlGameConnection, query);
				cache_get_data(rows, fields, sqlGameConnection);

				if(!rows) return SendMDCMessage(playerid, "No logs on record.");

				format(cstr, sizeof(cstr), "Date\t\tName\t\tGiven By\t\tTaken/Given\n");
				for(new i = 0; i < rows; i++)
				{
					cache_get_field_content(i, "Name", Name);
					cache_get_field_content(i, "GivenBy", GivenBy);
					cache_get_field_content(i, "Date", Date);
					IsTaken = cache_get_field_content_int(i, "Taken");
					if(IsTaken) format(cstr, sizeof(cstr), "%s{FF5F5F}(%s)\t{FF5F5F}%s\t{FF5F5F}%s\t{FF5F5F}Taken\n", cstr, Date, Name, GivenBy);
					else format(cstr, sizeof(cstr), "%s{1CC343}(%s)\t{1CC343}%s\t{1CC343}%s\t{1CC343}Given\n", cstr, Date, Name, GivenBy);
				}
				ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+7, DIALOG_STYLE_TABLIST_HEADERS, "Weapon License Record", cstr, "Ok", "");
				cache_delete(Cache:result, sqlGameConnection);
			}
			case 3:
			{
				if(GetMDCAccessType(playerid) != MDC_ACCESS_FULL)
					return SendMDCMessage(playerid, "This function is not available to you.");

				new wantedstr[1000], wantedAmount = 0;
				for(new i = 0; i <= GetPlayerPoolSize(); i++)
				{
					if(PlayerInfo[i][pWantedLevel] > 0)
					{
						wantedAmount++;
						format(wantedstr, sizeof(wantedstr), "%s%s\tWanted %i times.\n", wantedstr, PlayerICName(i), PlayerInfo[i][pWantedLevel]);
					}
				}

				if(wantedAmount) return ShowPlayerDialog(playerid, DIALOG_MDC_WANTED, DIALOG_STYLE_LIST, "Wanted", wantedstr, "Okay", "");
				else return SendMDCMessage(playerid, "No wanted people were found.");
			}
			case 4:
			{
				LogOffMDC(playerid);
				return 1;
			}
		}
		return 1;
	}
	if(dialogid == DIALOG_MDC_MAIN+1)
	{
		if(!response) return MDC_MAIN;
		else
		{
			new query[128], rows, fields, Cache:result, SearchName[26], MDCID;
			mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `players` WHERE name = '%e'", inputtext);
			result = mysql_query(sqlGameConnection, query);
			cache_get_data(rows, fields, sqlGameConnection);
			
			new bool:nofind;
			if(!rows) nofind = true;
			if(!nofind)
			{
				for(new i = 0; i < rows; i++)
				{
					cache_get_field_content(i, "Name", SearchName);
					MDCID = cache_get_field_content_int(i, "ID");
				}
			}
			if(nofind)
			{
				format(mdcstr, sizeof(mdcstr), "Your search query for '%s' has returned no results.", inputtext);
				return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+2, DIALOG_STYLE_MSGBOX, "{2641FE}Mobile Data Computer - Lookup function", mdcstr, "Search", "Return");
			}
			format(str, sizeof(str), "%s", (SearchName));
			strmid(PlayerInfo[playerid][pMDCSearchName], str, 0, strlen(str), 255);
			PlayerInfo[playerid][pMDCSearchID] = MDCID;
			format(mdcstr, sizeof(mdcstr), "{2641FE}Mobile Data Computer - %s", PlayerInfo[playerid][pMDCSearchName]);
			ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+3, DIALOG_STYLE_LIST, mdcstr, "Profile\nView History\n{FF6347}Add Warrant{FFFFFF}\nTrack Cellphone\nView Active Tickets\nRemove Ticket\nView Old Tickets\nAppearance", "Select", "Close");
			cache_delete(Cache:result, sqlGameConnection);
		}
		return 1;
	}
	if(dialogid == DIALOG_MDC_MAIN+2)
	{
		if(!response) return MDC_MAIN;
		else return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+1, DIALOG_STYLE_INPUT, "{2641FE}Mobile Data Computer - Lookup function", "{FFFFFF}Enter a name to run a search.\nPlease note that this will only display people who are registered on the police database.\n((Please not that you will have to use underscores '_' while searching for names.", "Search", "Return");
	}
	if(dialogid == DIALOG_MDC_MAIN+3)
	{
		if(!response) return MDC_MAIN;

		switch(listitem)
		{
			case 0://Profile
			{
				new query[128], profile[2000], rows, fields, Cache:result;
				mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `players` WHERE name = '%e'", PlayerInfo[playerid][pMDCSearchName]);
				result = mysql_query(sqlGameConnection, query);
				cache_get_data(rows, fields, sqlGameConnection);
				if(!rows) return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+2, DIALOG_STYLE_MSGBOX, "{2641FE}Mobile Data Computer - Lookup function", "{FFFFFF}An error has occured.", "Search", "Return");
				new level,// int
					Sex, //int
					Age,//int
					HouseID,//int
					BizKey,//int
					Pnumber,//int
					DriverPoints//int
				;
				for(new i = 0; i < rows; i++)
				{
					//cache_get_field_content(i, "Name", SearchName);
					cache_get_field_content_int(i, "Level", level);
					cache_get_field_content_int(i, "Sex", Sex);
					cache_get_field_content_int(i, "Age", Age);
					cache_get_field_content_int(i, "pHouseID", HouseID);
					cache_get_field_content_int(i, "BizKey", BizKey);
					cache_get_field_content_int(i, "PhoneNr", Pnumber);
					cache_get_field_content_int(i, "DriverPoints", DriverPoints);
				}
				format(str, sizeof(str), "{FFFF00}Name: %s\n", PlayerInfo[playerid][pMDCSearchName]);
				strcat(profile, str);
				
				if(IsPlayerConnected(GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])))
				{
					format(str, sizeof(str), "{FFFF00}Activity Status: {1CC343}Online\n");
				}
				else format(str, sizeof(str), "{FFFF00}Activity Status: {FF5F5F}Offline\n");
				strcat(profile, str);
				new genstr[15];
				if(IsPlayerConnected(GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName]))) Sex = PlayerInfo[GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])][pSex];
				switch(Sex)
				{
					case 1: genstr = "Male";
					case 2: genstr = "Female";
					default: genstr = "Not defined";
				}
				format(str, sizeof(str), "{FFFF00}Gender: %s\n", genstr);
				strcat(profile, str);
				
				if(IsPlayerConnected(GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName]))) Age = PlayerInfo[GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])][pAge];
				if(Age == 0) format(str, sizeof(str), "{FFFF00}Age: Not defined\n");
				else format(str, sizeof(str), "{FFFF00}Age: %d\n", Age);
				strcat(profile, str);
				
				new htext[40], location[MAX_ZONE_NAME];
				if(IsPlayerConnected(GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])))
				{
					if(PlayerInfo[GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])][pHouseID] != 0)
					{
						Get2DZone(location, MAX_ZONE_NAME, HouseInfo[PlayerInfo[GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])][pHouseID]][hOutsideX], HouseInfo[PlayerInfo[GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])][pHouseID]][hOutsideY], HouseInfo[PlayerInfo[GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])][pHouseID]][hOutsideZ]);
						format(htext, sizeof(htext), "%d %s", PlayerInfo[GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])][pHouseID], location);
					}
					else htext = "Homeless";
				}
				else//sql
				{
					if(HouseID != 0)
					{
						Get2DZone(location, MAX_ZONE_NAME, HouseInfo[HouseID][hOutsideX], HouseInfo[HouseID][hOutsideY], HouseInfo[HouseID][hOutsideZ]);
						format(htext, sizeof(htext), "%d %s", HouseID, location);
					}
					else htext = "Homeless";
				}
				format(str, sizeof(str), "{FFFF00}Address: %s\n", htext);
				strcat(profile, str);
				
				new btext[128];
				if(IsPlayerConnected(GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName]))) BizKey = PlayerInfo[GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])][pBizKey];
				if(BizKey == 0) BizKey = -1;//fix for bizid 0
				if(BizKey != -1) format(btext, sizeof(btext), "%s", BizInfo[BizKey][bName]);
				else btext = "None";
				
				format(str, sizeof(str), "{FFFF00}Business:  %s\n", btext);
				strcat(profile, str);
				
				if(IsPlayerConnected(GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName]))) Pnumber = PlayerInfo[GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])][pPnumber];
				format(str, sizeof(str), "{FFFF00}Phoner Number:  %d\n", Pnumber);
				strcat(profile, str);

				if(IsPlayerConnected(GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName]))) DriverPoints = PlayerInfo[GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])][pDriverPoints];
				format(str, sizeof(str), "{FFFF00}Driver Points:  %d\n", DriverPoints);
				strcat(profile, str);
				
				cache_delete(Cache:result, sqlGameConnection);

				// select * from playervehicles
				format(str, sizeof(str), "{FFFF00}Registered Vehicles:\nAn error has occured while searching the database ((Disabled - pending Jacob))");
				strcat(profile, str);

				
				format(mdcstr, sizeof(mdcstr), "{2641FE}Mobile Data Computer - %s", PlayerInfo[playerid][pMDCSearchName]);
				ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+4, DIALOG_STYLE_MSGBOX, mdcstr, profile, "Close", "");
				
			}
			case 1://History
			{
				new query[128], rows, fields, CrimeType, CrimeReason[128], cstr[5000], Date[50], copname[28], isEnabled, Cache:result;
				mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `newcrimes` WHERE `CrimePID` = %d ORDER BY `CrimeID` DESC LIMIT 20", PlayerInfo[playerid][pMDCSearchID]);
				result = mysql_query(sqlGameConnection, query);
				printf(query);
				cache_get_data(rows, fields, sqlGameConnection);

				// Get the player if they're online
				new mdcPlayer = GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName]);

				if(!rows || (IsPlayerConnected(mdcPlayer) && hitmanUndercover[mdcPlayer]))
				{
					format(mdcstr, sizeof(mdcstr), "Your search for '%s' has returned no warrants.", PlayerInfo[playerid][pMDCSearchName]);
					return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+2, DIALOG_STYLE_MSGBOX, "{2641FE}Mobile Data Computer - Lookup function", mdcstr, "Search", "Return");
				}
				format(cstr, sizeof(cstr), "Date\tType\tIssuer\tReason\n");
				for(new i = 0; i < rows; i++)
				{
					cache_get_field_content(i, "CrimeReason", CrimeReason);
					cache_get_field_content(i, "RealName", copname);
					cache_get_field_content(i, "Date", Date);
					CrimeType = cache_get_field_content_int(i, "CrimeType");
					isEnabled = cache_get_field_content_int(i, "CrimeEnabled");
					if(isEnabled) format(cstr, sizeof(cstr), "%s{1CC343}(%s)\t{1CC343}%s\t{1CC343}%s\t{1CC343}%s\n", cstr, Date, GetCrimeType(CrimeType), copname, CrimeReason);
					else format(cstr, sizeof(cstr), "%s{FF5F5F}(%s)\t{FF5F5F}%s\t{FF5F5F}%s\t{FF5F5F}%s{FFFFFF}\n", cstr, Date, GetCrimeType(CrimeType), copname, CrimeReason);
				}
				ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+4, DIALOG_STYLE_TABLIST_HEADERS, "{2641FE}Mobile Data Computer - Lookup Function", cstr, "Close", "");
				cache_delete(Cache:result, sqlGameConnection);
			}
			case 2:// Add Warrant
			{
				if(GetMDCAccessType(playerid) != MDC_ACCESS_FULL)
					return SendMDCMessage(playerid, "This function is not available to you.");

				format(mdcstr, sizeof(mdcstr), "{2641FE}Mobile Data Computer - %s", PlayerInfo[playerid][pMDCSearchName]);
				ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+5, DIALOG_STYLE_INPUT, mdcstr, "Please enter the warrant reason\t\t\t\t\t\t", "Confirm", "Close");
			}
			case 3://Track Cellhpone
			{
				if(GetMDCAccessType(playerid) != MDC_ACCESS_FULL)
					return SendMDCMessage(playerid, "This function is not available to you.");

				if(!IsPlayerConnected(GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])))
				{
					format(mdcstr, sizeof(mdcstr), "Unable to Track Cellphone (( %s is not online ))", PlayerInfo[playerid][pMDCSearchName]);
					return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+4, DIALOG_STYLE_MSGBOX, "{2641FE}Mobile Data Computer - Lookup function", mdcstr, "Search", "Return");
				}
				if(PlayerInfo[GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])][pPnumber] != 0 && PhoneOffline[GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])] == 0)
				{
					new plZone[MAX_ZONE_NAME];
					GetPlayer2DZone(GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName]), plZone, sizeof(plZone));
					format(str, sizeof(str), "That person was last seen in %s.", plZone);
					SendClientMessage(playerid, -1, str);
				}
				else
				{
					format(mdcstr, sizeof(mdcstr), "Track Cellphone - Unavailable", PlayerInfo[playerid][pMDCSearchName]);
					return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+4, DIALOG_STYLE_MSGBOX, "{2641FE}Mobile Data Computer - Lookup function", mdcstr, "Close", "");
				}
			}
			case 4://view tickets
			{
				new query[128], rows, fields, TicketReason[128], Amount, RealName[128], TicketID, cstr[5000], Cache:result;
				mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `tickets` WHERE `TicketPID` = %d AND `Active` = 1 ORDER BY `TicketID` DESC", PlayerInfo[playerid][pMDCSearchID]);
				result = mysql_query(sqlGameConnection, query);
				cache_get_data(rows, fields, sqlGameConnection);
				if(!rows)
				{
					format(str, sizeof(str), "%s has no active tickets.", PlayerInfo[playerid][pMDCSearchName]);
					SendMDCMessage(playerid, str);
					return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+3, DIALOG_STYLE_LIST, mdcstr, "Profile\nView History\n{FF6347}Add Warrant{FFFFFF}\nTrack Cellphone\nView Active Tickets\nRemove Ticket\nView Old Tickets\nAppearance", "Select", "Close");
				}
				format(cstr, sizeof(cstr), "Ticket ID\tIssuer\tAmount\tReason\n");
				for(new i = 0; i < rows; i++)
				{
					cache_get_field_content(i, "TicketReason", TicketReason);
					cache_get_field_content(i, "IssuerName", RealName);
					Amount = cache_get_field_content_int(i, "Amount");
					TicketID = cache_get_field_content_int(i, "TicketID");
					format(cstr, sizeof(cstr), "%s#%d\t%s\t$%d\t%s\n", cstr, TicketID, RealName, Amount, TicketReason);
				}
				cache_delete(Cache:result, sqlGameConnection);
				return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+4, DIALOG_STYLE_TABLIST_HEADERS, "{2641FE}Mobile Data Computer - Unpaid Tickets", cstr, "Close", "");
			}
			case 5://remove ticket
			{
				if(GetMDCAccessType(playerid) != MDC_ACCESS_FULL)
					return SendMDCMessage(playerid, "This function is not available to you.");

				format(str, sizeof(str), "Please type the ticket ID you would like to remove from %s.", PlayerInfo[playerid][pMDCSearchName]);
				return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+12, DIALOG_STYLE_INPUT, "{2641FE}Mobile Data Computer - Remove Ticket", str, "Remove", "Return");
			}
			case 6://old tickets
			{
				new query[128], rows, fields, TicketReason[128], Amount, RealName[128], TicketID, cstr[5000], Cache:result;
				mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `tickets` WHERE `TicketPID` = %d AND `Active` = 0 ORDER BY `TicketID` DESC", PlayerInfo[playerid][pMDCSearchID]);
				result = mysql_query(sqlGameConnection, query);
				cache_get_data(rows, fields, sqlGameConnection);
				if(!rows)
				{
					format(str, sizeof(str), "%s has no paid tickets.", PlayerInfo[playerid][pMDCSearchName]);
					SendMDCMessage(playerid, str);
					return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+3, DIALOG_STYLE_LIST, mdcstr, "Profile\nView History\n{FF6347}Add Warrant{FFFFFF}\nTrack Cellphone\nView Active Tickets\nRemove Ticket\nView Old Tickets\nAppearance", "Select", "Close");
				}
				format(cstr, sizeof(cstr), "Ticket ID\tIssuer\tAmount\tReason\n");
				for(new i = 0; i < rows; i++)
				{
					cache_get_field_content(i, "TicketReason", TicketReason);
					cache_get_field_content(i, "IssuerName", RealName);
					Amount = cache_get_field_content_int(i, "Amount");
					TicketID = cache_get_field_content_int(i, "TicketID");
					format(cstr, sizeof(cstr), "%s#%d\t%s\t$%d\t%s\n", cstr, TicketID, RealName, Amount, TicketReason);
				}
				cache_delete(Cache:result, sqlGameConnection);
				return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+4, DIALOG_STYLE_TABLIST_HEADERS, "{2641FE}Mobile Data Computer - Paid Tickets", cstr, "Close", "");
			}
			case 7: // Appearance
			{
				if(GetMDCAccessType(playerid) != MDC_ACCESS_FULL)
					return SendMDCMessage(playerid, "This function is not available to you.");

				new query[128], rows, fields, Cache:result;
				mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `players` WHERE name = '%e'", PlayerInfo[playerid][pMDCSearchName]);
				result = mysql_query(sqlGameConnection, query);
				cache_get_data(rows, fields, sqlGameConnection);
				
				new appearance[64], build, height, eyecolour, ethnicity, gender, age;
				

				build = cache_get_field_content_int(0, "Build");
				gender = cache_get_field_content_int(0, "Sex");
				age = cache_get_field_content_int(0, "Age");
				height = cache_get_field_content_int(0, "Height");
				eyecolour = cache_get_field_content_int(0, "EyeColour");
				ethnicity = cache_get_field_content_int(0, "Ethnicity");
				cache_get_field_content(0, "Appearance", appearance, sqlGameConnection, 64);

				new bstr[16], cstr[8], dstr[8], estr[9];
				switch(ethnicity)
				{
					case 0: bstr = "White";
					case 1: bstr = "Black";
					case 2: bstr = "Asian";
					case 3: bstr = "Hispanic";
					case 4: bstr = "Latino";
					case 5: bstr = "Native American";
					default: bstr = "White";
				}
				switch(eyecolour)
				{
					case 0: cstr = "Blue";
					case 1: cstr = "Brown";
					case 2: cstr = "Green";
					default: cstr = "Blue";
				}
				switch(gender)
				{
					case 0: dstr = "Unknown";
					case 1: dstr = "Male";
					case 2: dstr = "Female";
					default: dstr = "Unknown";
				}
				switch(build)
				{
					case 0: estr = "Slim";
					case 1: estr = "Average";
					case 2: estr = "Muscular";
					default: estr = "Average";
				}
				
				new astr[512];
				format(astr, sizeof(astr), "Gender:\t\t%s\nAge:\t\t\t%d\nEthnicity\t\t%s\nEye Colour:\t\t%s\nBody Build:\t\t%s\nHeight:\t\t\t%dcm\nUnique Features:\t%s",
				dstr, age, bstr, cstr, estr, height, appearance);
				
				
				cache_delete(Cache:result, sqlGameConnection);
				ShowPlayerDialog(playerid, DIALOG_APPEARANCE_DISPLAYMDC, DIALOG_STYLE_LIST, "Character Appearance", astr, "Back", "");
				
			}
		}
		return 1;
	}
	if(dialogid == DIALOG_MDC_MAIN+4)
	{
		format(mdcstr, sizeof(mdcstr), "{2641FE}Mobile Data Computer - %s", PlayerInfo[playerid][pMDCSearchName]);
		return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+3, DIALOG_STYLE_LIST, mdcstr, "Profile\nView History\n{FF6347}Add Warrant{FFFFFF}\nTrack Cellphone\nView Active Tickets\nRemove Ticket\nView Old Tickets\nAppearance", "Select", "Close");
	}
	if(dialogid == DIALOG_MDC_MAIN+5)
	{
		if(!response)
		{
			format(mdcstr, sizeof(mdcstr), "{2641FE}Mobile Data Computer - %s", PlayerInfo[playerid][pMDCSearchName]);
			return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+3, DIALOG_STYLE_LIST, mdcstr, "Profile\nView History\n{FF6347}Add Warrant{FFFFFF}\nTrack Cellphone\nView Active Tickets\nRemove Ticket\nView Old Tickets\nAppearance", "Select", "Close");
		}
		else
		{
			if(!IsPlayerConnected(GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName]))) return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+2, DIALOG_STYLE_MSGBOX, "{2641FE}Mobile Data Computer - Lookup function", "{FFFFFF}That player is not online.", "Search", "Return");
			if(UseAdmCmdTimer[playerid] > 8)
			{
				new reason[128];
				format(str, sizeof(str), "{FF6347}SERVER: %s was banned, reason: (autoban) /su spam.", PlayerOOCName(playerid));
				SendClientMessageToAll(COLOR_WHITE, str);

				format(reason, sizeof(reason), "/su spam.");
				BanAccount(playerid, INVALID_PLAYER_ID, reason); //log before banning the account
				return 1;
			}
			PlayerInfo[GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])][pWantedLevel]++;
			SetPlayerCriminal(GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName]), playerid, inputtext);
			AddCrime(playerid, GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName]), 2, inputtext);
			PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);
			UseAdmCmdTimer[playerid]++;
			SetTimerEx("Timer_UseAdmCmd", 3 *1000, 0, "i", playerid); //3 seconds
			format(mdcstr, sizeof(mdcstr), "{2641FE}Mobile Data Computer - %s", PlayerInfo[playerid][pMDCSearchName]);
			ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+3, DIALOG_STYLE_LIST, mdcstr, "Profile\nView History\n{FF6347}Add Warrant{FFFFFF}\nTrack Cellphone\nView Active Tickets\nRemove Ticket\nView Old Tickets\nAppearance", "Select", "Close");
		}
		return 1;
	}

	if(dialogid == DIALOG_MDC_MAIN+6)
	{
		if(!response) return MDC_MAIN;
		else
		{
			switch(listitem)
			{
				case 0:// ALL BOLO
				{
					new bstr[1500], count=0;
					for(new i = 0; i < MAX_PLAYERS; i++)
					{
						if(IsPlayerConnected(i))
						{
							if(isBOLO[i])
							{
								new query[128], CrimeReason[128], rows, fields, Cache:result;
								mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `newcrimes` WHERE `CrimePID` = %d AND CrimeType = 3 ORDER BY `CrimeID` DESC LIMIT 1", PlayerInfo[i][pID]);
								result = mysql_query(sqlGameConnection, query);
								printf(query);
								cache_get_data(rows, fields, sqlGameConnection);
								if(!rows)
								{
									SendClientMessage(playerid, COLOR_RED, "(MDC): An error has occured.");
									return MDC_MAIN;
								}
								cache_get_field_content(0, "CrimeReason", CrimeReason);
								count++;
								format(bstr, sizeof(bstr), "%s%s - %s\n", bstr, PlayerOOCName(i), CrimeReason);
								cache_delete(Cache:result, sqlGameConnection);
							}
						}
					}
					if(count==0){ format(bstr, sizeof(bstr), "There are no BOLO notices.\t\t"); }
					ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+7, DIALOG_STYLE_LIST, "{2641FE}Mobile Data Computer - Be On Look Out", bstr, "Ok", "");
				}
				case 1:// ADD BOLO
				{
					ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+8, DIALOG_STYLE_INPUT, "{2641FE}Mobile Data Computer - Be On Look Out", "{FFFFFF}Please enter a name to {1CC343}CREATE{FFFFFF} a BOLO\n((Please not that you will have to use underscores '_' while searching for names.", "Add", "Return");
				}
				case 2:// DELETE BOLO
				{
					ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+9, DIALOG_STYLE_INPUT, "{2641FE}Mobile Data Computer - Be On Look Out", "{FFFFFF}Please enter a name to {FF5F5F}REMOVE{FFFFFF} a BOLO\n((Please not that you will have to use underscores '_' while searching for names.", "Add", "Return");
				}
			}
		}
		return 1;
	}
	if(dialogid == DIALOG_MDC_MAIN+7) return MDC_MAIN;
	if(dialogid == DIALOG_MDC_MAIN+8)// add name
	{
		if(!response) return MDC_MAIN;
		format(str, sizeof(str), "%s", inputtext);
		strmid(PlayerInfo[playerid][pMDCSearchName], str, 0, strlen(str), 255);
		return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+10, DIALOG_STYLE_INPUT, "{2641FE}Mobile Data Computer - Be On Look Out", "{FFFFFF}Please add a BOLO reason\t\t\t\t\t\t\t", "Add", "Return");
	}
	if(dialogid == DIALOG_MDC_MAIN+10)//add reason
	{
		if(!response) return MDC_MAIN;
		new query[128], rows, fields, ISABOLO, realname[MAX_PLAYER_NAME+1], Cache:result;
		mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `players` WHERE `name` = '%e' LIMIT 1", PlayerInfo[playerid][pMDCSearchName]);
		result = mysql_query(sqlGameConnection, query);
		cache_get_data(rows, fields, sqlGameConnection);
		cache_get_field_content(0, "Name", realname);
		ISABOLO = cache_get_field_content_int(0, "isBOLO");
		cache_delete(Cache:result, sqlGameConnection);
		format(str, sizeof(str), "%s", realname);
		strmid(PlayerInfo[playerid][pMDCSearchName], str, 0, strlen(str), 255);
		if(IsPlayerConnected(GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])))
		{
			isBOLO[GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])] = 1;
			format(str, sizeof(str), "DISPATCH: %s %s has placed a new B.O.L.O alert.", GetPlayerRank(playerid), PlayerOOCName(playerid));
			SendRadioMessage(PlayerInfo[playerid][pMember], TEAM_BLUE_COLOR, str);
			format(str, sizeof(str), "%s", inputtext);
			AddCrime(playerid, GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName]), 3, str);
			return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+6, DIALOG_STYLE_LIST, "{2641FE}Mobile Data Computer - BOLO", "View BOLOs\nAdd BOLO\nRemove BOLO", "Okay", "Close");
		}
		else
		{
			if(!rows)
			{
				format(mdcstr, sizeof(mdcstr), "Your BOLO query for '%s' has returned no results.\nPlease type another name adjust the BOLO\n((Please not that you will have to use underscores '_' while searching for names.", PlayerInfo[playerid][pMDCSearchName]);
				return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+11, DIALOG_STYLE_MSGBOX, "{2641FE}Mobile Data Computer - Be On Look Out", mdcstr, "Search", "Return");
			}
			if(ISABOLO == 1) return SendClientMessage(playerid, COLOR_LIGHTRED, "(MDC): That person already has a BOLO on them.");
			else
			{
				format(str, sizeof(str), "%s", inputtext);
				AddOfflineCrime(playerid, PlayerInfo[playerid][pMDCSearchName], 3, str);
				mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `players` SET `isBOLO` = 1 WHERE `name` = '%e'", PlayerInfo[playerid][pMDCSearchName]);
				mysql_pquery(sqlGameConnection, query);
				format(mdcstr, sizeof(mdcstr), "(MDC): A BOLO has been placed on %s.", PlayerInfo[playerid][pMDCSearchName]);
				SendClientMessage(playerid, COLOR_LIGHTRED, mdcstr);

				
				return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+6, DIALOG_STYLE_LIST, "{2641FE}Mobile Data Computer - BOLO", "View BOLOs\nAdd BOLO\nRemove BOLO", "Okay", "Close");
			}
		}
	}
	if(dialogid == DIALOG_MDC_MAIN+9)//remove
	{
		if(!response) return MDC_MAIN;
		new query[128], rows, fields, ISABOLO, realname[MAX_PLAYER_NAME+1], Cache:result;
		mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `players` WHERE name = '%e' LIMIT 1", inputtext);
		result = mysql_query(sqlGameConnection, query);
		cache_get_data(rows, fields);
		cache_get_field_content(0, "Name", realname);
		ISABOLO = cache_get_field_content_int(0, "isBOLO");
		cache_delete(Cache:result, sqlGameConnection);
		format(str, sizeof(str), "%s", (realname));
		strmid(PlayerInfo[playerid][pMDCSearchName], str, 0, strlen(str), 255);
		if(IsPlayerConnected(GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])))
		{
			if(!isBOLO[GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])])
			{
				SendClientMessage(playerid, COLOR_LIGHTRED, "(MDC): That person doesn't have a BOLO on them.");
				return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+6, DIALOG_STYLE_LIST, "{2641FE}Mobile Data Computer - BOLO", "View BOLOs\nAdd BOLO\nRemove BOLO", "Okay", "Close");
			}
			isBOLO[GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])] = 0;
			format(mdcstr, sizeof(mdcstr), "(MDC): You have removed the BOLO on %s.", inputtext);
			SendClientMessage(playerid, COLOR_LIGHTRED, mdcstr);
			mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `newcrimes` SET `CrimeEnabled` = 0 WHERE (`CrimePID` = '%d' AND `CrimeType` = 3)", PlayerInfo[playerid][pMDCSearchID]);
			mysql_pquery(sqlGameConnection, query);
			return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+6, DIALOG_STYLE_LIST, "{2641FE}Mobile Data Computer - BOLO", "View BOLOs\nAdd BOLO\nRemove BOLO", "Okay", "Close");
		}
		else
		{
			if(!rows)
			{
				format(mdcstr, sizeof(mdcstr), "Your BOLO query for '%s' has returned no results.\nPlease type another name adjust the BOLO\n((Please not that you will have to use underscores '_' while searching for names.", inputtext);
				return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+11, DIALOG_STYLE_MSGBOX, "{2641FE}Mobile Data Computer - Be On Look Out", mdcstr, "Search", "Return");
			}
			if(ISABOLO == 0) return SendClientMessage(playerid, COLOR_LIGHTRED, "(MDC): That person doesn't have a BOLO on them.");
			else
			{
				mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `players` SET `isBOLO` = 0 WHERE `name` = '%e'", inputtext);
				mysql_pquery(sqlGameConnection, query);
				mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `newcrimes` SET `CrimeEnabled` = 0 WHERE (`CrimePID` = '%d' AND `CrimeType` = 3)", PlayerInfo[playerid][pMDCSearchID]);
				mysql_pquery(sqlGameConnection, query);
				format(mdcstr, sizeof(mdcstr), "(MDC): You have removed the BOLO on %s.", inputtext);
				SendClientMessage(playerid, COLOR_LIGHTRED, mdcstr);
				return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+6, DIALOG_STYLE_LIST, "{2641FE}Mobile Data Computer - BOLO", "View BOLOs\nAdd BOLO\nRemove BOLO", "Okay", "Close");
			}
		}
	}
	if(dialogid == DIALOG_MDC_MAIN+11) return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+6, DIALOG_STYLE_LIST, "{2641FE}Mobile Data Computer - BOLO", "View BOLOs\nAdd BOLO\nRemove BOLO", "Okay", "Close");
	if(dialogid == DIALOG_MDC_MAIN+12)//remove ticket
	{
		new realnum = strval(inputtext);
		if(!response)
		{
			format(mdcstr, sizeof(mdcstr), "{2641FE}Mobile Data Computer - %s", PlayerInfo[playerid][pMDCSearchName]);
			return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+3, DIALOG_STYLE_LIST, mdcstr, "Profile\nView History\n{FF6347}Add Warrant{FFFFFF}\nTrack Cellphone\nView Active Tickets\nRemove Ticket\nView Old Tickets\nAppearance", "Select", "Close");
		}
		if(!IsNumeric(inputtext))
		{
			SendClientMessage(playerid, COLOR_LIGHTRED, "(MDC): Invalid input.");
			format(mdcstr, sizeof(mdcstr), "{2641FE}Mobile Data Computer - %s", PlayerInfo[playerid][pMDCSearchName]);
			return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+3, DIALOG_STYLE_LIST, mdcstr, "Profile\nView History\n{FF6347}Add Warrant{FFFFFF}\nTrack Cellphone\nView Active Tickets\nRemove Ticket\nView Old Tickets\nAppearance", "Select", "Close");
		}
		if(!IsPlayerConnected(GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName])))
		{
			SendClientMessage(playerid, COLOR_LIGHTRED, "(MDC): That player is not online.");
			format(mdcstr, sizeof(mdcstr), "{2641FE}Mobile Data Computer - %s", PlayerInfo[playerid][pMDCSearchName]);
			return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+3, DIALOG_STYLE_LIST, mdcstr, "Profile\nView History\n{FF6347}Add Warrant{FFFFFF}\nTrack Cellphone\nView Active Tickets\nRemove Ticket\nView Old Tickets\nAppearance", "Select", "Close");
		}
		new rows, fields, query[128], pid, date[128], crimeid, active, Cache:result;
		mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `tickets` WHERE `TicketID` = %d LIMIT 1", realnum);
		result = mysql_query(sqlGameConnection, query);
		printf(query);
		cache_get_data(rows, fields, sqlGameConnection);
		if(!rows)
		{
			SendClientMessage(playerid, COLOR_LIGHTRED, "(MDC): Invalid ticket ID.");
			format(mdcstr, sizeof(mdcstr), "{2641FE}Mobile Data Computer - %s", PlayerInfo[playerid][pMDCSearchName]);
			return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+3, DIALOG_STYLE_LIST, mdcstr, "Profile\nView History\n{FF6347}Add Warrant{FFFFFF}\nTrack Cellphone\nView Active Tickets\nRemove Ticket\nView Old Tickets\nAppearance", "Select", "Close");
		}
		active = cache_get_field_content_int(0, "Active");
		pid = cache_get_field_content_int(0, "TicketPID");
		cache_get_field_content(0, "Date", date);
		if(pid != PlayerInfo[playerid][pMDCSearchID])
		{
			format(mdcstr, sizeof(mdcstr), "(MDC): That ticket number does not belong to %s.", PlayerInfo[playerid][pMDCSearchName]);
			SendClientMessage(playerid, COLOR_LIGHTRED, str);
			format(mdcstr, sizeof(mdcstr), "{2641FE}Mobile Data Computer - %s", PlayerInfo[playerid][pMDCSearchName]);
			return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+3, DIALOG_STYLE_LIST, mdcstr, "Profile\nView History\n{FF6347}Add Warrant{FFFFFF}\nTrack Cellphone\nView Active Tickets\nRemove Ticket\nView Old Tickets\nAppearance", "Select", "Close");
		}
		if(!active)
		{
			format(mdcstr, sizeof(mdcstr), "(MDC): That ticket number does not belong to %s.", PlayerInfo[playerid][pMDCSearchName]);
			SendClientMessage(playerid, COLOR_LIGHTRED, str);
			format(mdcstr, sizeof(mdcstr), "{2641FE}Mobile Data Computer - %s", PlayerInfo[playerid][pMDCSearchName]);
			return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+3, DIALOG_STYLE_LIST, mdcstr, "Profile\nView History\n{FF6347}Add Warrant{FFFFFF}\nTrack Cellphone\nView Active Tickets\nRemove Ticket\nView Old Tickets\nAppearance", "Select", "Close");
		}

		cache_delete(Cache:result, sqlGameConnection);

		new query2[128];
		mysql_format(sqlGameConnection, query2, sizeof(query2), "UPDATE `tickets` SET `Active` = 0 WHERE `TicketID` = '%d'", realnum);
		mysql_pquery(sqlGameConnection, query2);


		format(str, sizeof(str), "HQ: %s has deleted ticket number %d (%s).", PlayerICName(playerid), realnum, PlayerInfo[playerid][pMDCSearchName]);
		SendRadioMessage(1, TEAM_BLUE_COLOR, str);
		
		format(str, sizeof(str), "* %s has removed your ticket #%d", PlayerICName(playerid), realnum);
		SendClientMessage(GetPlayerIDFromName(PlayerInfo[playerid][pMDCSearchName]), COLOR_LIGHTRED, str);
		
		mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `newcrimes` WHERE `DATE` = '%e' LIMIT 1", date);
		result = mysql_query(sqlGameConnection, query);
		cache_get_data(rows, fields, sqlGameConnection);
		if(!rows) return SendClientMessage(playerid, COLOR_GREY, "An error has occured while updating the MDC.");
		crimeid = cache_get_field_content_int(0, "CrimeID");
		cache_delete(Cache:result, sqlGameConnection);
		mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `newcrimes` SET `CrimeEnabled` = 0 WHERE `CrimeID` = '%d'", crimeid);
		mysql_pquery(sqlGameConnection, query);
	}
	if(dialogid == DIALOG_NAMECHANGES)
	{
		if(!response) return 1;
		switch(listitem)
		{
			case 0://recent
			{
				new query[128], OldName[26], NewName[26], ChangedBy[26], rows, fields, cstr[5000], Date[50], Cache:result;
				mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `namechanges` ORDER BY `ID` DESC");
				result = mysql_query(sqlGameConnection, query);
				cache_get_data(rows, fields, sqlGameConnection);
				if(!rows) return SendClientMessage(playerid, COLOR_WHITE, "No namechanges on record.");
				format(cstr, sizeof(cstr), "Date\t\tOld Name\t\tNew Name\t\tAdmin\n");
				for(new i = 0; i < rows; i++)
				{
					cache_get_field_content(i, "OldName", OldName);
					cache_get_field_content(i, "NewName", NewName);
					cache_get_field_content(i, "ApprovedBy", ChangedBy);
					cache_get_field_content(i, "Date", Date);


					format(cstr, sizeof(cstr), "%s%s\t\t%s\t\t%s\t\t%s\n", cstr, Date, OldName, NewName, ChangedBy);
				}
				ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_TABLIST_HEADERS, "Namechange Record", cstr, "Ok", "");
				cache_delete(Cache:result, sqlGameConnection);
			}
			case 1: ShowPlayerDialog(playerid, DIALOG_NAMECHANGES_OLD, DIALOG_STYLE_INPUT, "Old Name Records", "Search By Old Name", "Ok", "");
			case 2: ShowPlayerDialog(playerid, DIALOG_NAMECHANGES_NEW, DIALOG_STYLE_INPUT, "New Name Records", "Search By New Name", "Ok", "");
			case 3: ShowPlayerDialog(playerid, DIALOG_NAMECHANGES_ADMIN, DIALOG_STYLE_INPUT, "Admin Name Record", "Search Changes By A Specific Admin", "Ok", "");
			case 4: ShowPlayerDialog(playerid, DIALOG_NAMECHANGES_ID, DIALOG_STYLE_INPUT, "Namechange Records", "Search By Account ID", "Ok", "");
		}
		return 1;
	}
	if(dialogid == DIALOG_NAMECHANGES_OLD)
	{
		if(!response) return 1;
		else
		{
			new query[128], OldName[26], NewName[26], ChangedBy[26], rows, fields, cstr[5000], Date[50], Cache:result;
			mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `namechanges` WHERE `OldName`='%e' ORDER BY `ID` DESC", inputtext);
			result = mysql_query(sqlGameConnection, query);
			cache_get_data(rows, fields, sqlGameConnection);
			if(!rows) return SendClientMessage(playerid, COLOR_WHITE, "No namechanges on record.");
			format(cstr, sizeof(cstr), "Date\t\tOld Name\t\tNew Name\t\tAdmin\n");
			for(new i = 0; i < rows; i++)
			{
				cache_get_field_content(i, "OldName", OldName);
				cache_get_field_content(i, "NewName", NewName);
				cache_get_field_content(i, "ApprovedBy", ChangedBy);
				cache_get_field_content(i, "Date", Date);

				format(cstr, sizeof(cstr), "%s%s\t\t%s\t\t%s\t\t%s\n", cstr, Date, OldName, NewName, ChangedBy);
			}
			ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_TABLIST_HEADERS, "Namechange Record", cstr, "Ok", "");
			cache_delete(Cache:result, sqlGameConnection);
		}
		return 1;
	}
	if(dialogid == DIALOG_NAMECHANGES_NEW)
	{
		if(!response) return 1;
		else
		{
			new query[128], OldName[26], NewName[26], ChangedBy[26], rows, fields, cstr[5000], Date[50], Cache:result;
			mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `namechanges` WHERE `NewName`='%e' ORDER BY `ID` DESC", inputtext);
			result = mysql_query(sqlGameConnection, query);
			cache_get_data(rows, fields, sqlGameConnection);
			if(!rows) return SendClientMessage(playerid, COLOR_WHITE, "No namechanges on record.");
			format(cstr, sizeof(cstr), "Date\t\tOld Name\t\tNew Name\t\tAdmin\n");
			for(new i = 0; i < rows; i++)
			{
				cache_get_field_content(i, "OldName", OldName);
				cache_get_field_content(i, "NewName", NewName);
				cache_get_field_content(i, "ApprovedBy", ChangedBy);
				cache_get_field_content(i, "Date", Date);

				format(cstr, sizeof(cstr), "%s%s\t\t%s\t\t%s\t\t%s\n", cstr, Date, OldName, NewName, ChangedBy);
			}
			ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_TABLIST_HEADERS, "Namechange Record", cstr, "Ok", "");
			cache_delete(Cache:result, sqlGameConnection);
		}
		return 1;
	}
	if(dialogid == DIALOG_NAMECHANGES_ADMIN)
	{
		if(!response) return 1;
		else
		{
			new query[128], OldName[26], NewName[26], ChangedBy[26], rows, fields, cstr[5000], Date[50], Cache:result;
			mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `namechanges` WHERE `ApprovedBy`='%e' ORDER BY `ID` DESC", inputtext);
			result = mysql_query(sqlGameConnection, query);
			cache_get_data(rows, fields, sqlGameConnection);
			if(!rows) return SendClientMessage(playerid, COLOR_WHITE, "No namechanges on record.");
			format(cstr, sizeof(cstr), "Date\t\tOld Name\t\tNew Name\t\tAdmin\n");
			for(new i = 0; i < rows; i++)
			{
				cache_get_field_content(i, "OldName", OldName);
				cache_get_field_content(i, "NewName", NewName);
				cache_get_field_content(i, "ApprovedBy", ChangedBy);
				cache_get_field_content(i, "Date", Date);

				format(cstr, sizeof(cstr), "%s%s\t\t%s\t\t%s\t\t%s\n", cstr, Date, OldName, NewName, ChangedBy);
			}
			ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_TABLIST_HEADERS, "Namechange Record", cstr, "Ok", "");
			cache_delete(Cache:result, sqlGameConnection);
		}
		return 1;
	}
	if(dialogid == DIALOG_NAMECHANGES_ID)
	{
		if(!response) return 1;
		else
		{
			new query[128], OldName[26], NewName[26], ChangedBy[26], rows, fields, cstr[5000], Date[50], Cache:result;
			mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `namechanges` WHERE `pID`='%e' ORDER BY `ID` DESC", inputtext);
			result = mysql_query(sqlGameConnection, query);
			cache_get_data(rows, fields, sqlGameConnection);
			if(!rows) return SendClientMessage(playerid, COLOR_WHITE, "No namechanges on record.");
			format(cstr, sizeof(cstr), "Date\t\tOld Name\t\tNew Name\t\tAdmin\n");
			for(new i = 0; i < rows; i++)
			{
				cache_get_field_content(i, "OldName", OldName);
				cache_get_field_content(i, "NewName", NewName);
				cache_get_field_content(i, "ApprovedBy", ChangedBy);
				cache_get_field_content(i, "Date", Date);

				format(cstr, sizeof(cstr), "%s%s\t\t%s\t\t%s\t\t%s\n", cstr, Date, OldName, NewName, ChangedBy);
			}
			ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_TABLIST_HEADERS, "Namechange Record", cstr, "Ok", "");
			cache_delete(Cache:result, sqlGameConnection);
		}
		return 1;
	}
	if(dialogid == DIALOG_HOUSEUPGRADE)
	{
		if(!response) return 1;
		else
		{
			switch(listitem)
			{
				case 0:
				{
					if(HouseInfo[PlayerInfo[playerid][pHouseID]][hAlarm] == 0)
					{
						if(PlayerInfo[playerid][pCash] >= 30000)
						{
							new location[MAX_ZONE_NAME];
							Get2DZone(location, MAX_ZONE_NAME, HouseInfo[PlayerInfo[playerid][pHouseID]][hOutsideX], HouseInfo[PlayerInfo[playerid][pHouseID]][hOutsideY], HouseInfo[PlayerInfo[playerid][pHouseID]][hOutsideZ]);
							HouseInfo[PlayerInfo[playerid][pHouseID]][hAlarm] = 1;
							HouseInfo[PlayerInfo[playerid][pHouseID]][hModified] = 1;
							PlayerInfo[playerid][pCash] -= 30000;
							format(str, sizeof(str), "[House Upgrade] You have purchased an alarm for your house (%d %s) for $30,000.", PlayerInfo[playerid][pHouseID], location);
							SendClientMessage(playerid, COLOR_YELLOW, str);
						}
						else return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					}
					else return SendClientMessage(playerid, COLOR_GREY, "Your house already has an alarm fitted.");
				}
			}
		}
	}
	if(dialogid == TAXICALL_DIALOG)
	{
		if(response == 1)
		{
			if(strval(inputtext) > 0 && strval(inputtext) < 20)
			{
				new Float:X, Float:Y, Float:Z;
				GetPlayerPos(TaxiCInfoID[strval(inputtext)], X, Y, Z);
				SetPlayerCheckpointEx(playerid, X, Y, Z, 1.0);
				CP[playerid] = CHECKPOINT_TAXI_CALLER;
				new string[128];
				format(string, sizeof(string), "%s in %s, has been marked on your GPS.", TaxiCInfo[strval(inputtext)][taxiname], TaxiCInfo[strval(inputtext)][taxilocation]);
				SendClientMessage(playerid, -1, string);
				TaxiCID[playerid] = TaxiCInfoID[strval(inputtext)];
				TaxiCInfoID[strval(inputtext)] = -1;
				TaxiCInfo[strval(inputtext)][taxiname] = "0";
			} else return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} Invalid caller ID.");
			return 1;
		}
	}

	if(dialogid == DIALOG_MOD_EDIT)
	{
		if(response)
		{
			new vid = GetPlayerVehicleID(playerid);
			if(listitem == 0)
			{
				if(GetVehicleComponentInSlot(vid, 0) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that slot.");
				removeSlotComponent(vid, 0);
			}
			if(listitem == 1)
			{
				if(GetVehicleComponentInSlot(vid, 1) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that slot.");
				removeSlotComponent(vid, 1);
			}
			if(listitem == 2)
			{
				if(GetVehicleComponentInSlot(vid, 2) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that slot.");
				removeSlotComponent(vid, 2);
			}
			if(listitem == 3)
			{
				if(GetVehicleComponentInSlot(vid, 3) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that slot.");
				removeSlotComponent(vid, 3);
			}
			if(listitem == 4)
			{
				if(GetVehicleComponentInSlot(vid, 4) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that slot.");
				removeSlotComponent(vid, 4);
			}
			if(listitem == 5)
			{
				if(GetVehicleComponentInSlot(vid, 5) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that slot.");
				removeSlotComponent(vid, 5);
			}
			if(listitem == 6)
			{
				if(GetVehicleComponentInSlot(vid, 6) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that slot.");
				removeSlotComponent(vid, 6);
			}
			if(listitem == 7)
			{
				if(GetVehicleComponentInSlot(vid, 7) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that slot.");
				removeSlotComponent(vid, 7);
			}
			if(listitem == 8)
			{
				if(GetVehicleComponentInSlot(vid, 8) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that slot.");
				removeSlotComponent(vid, 8);
			}
			if(listitem == 9)
			{
				if(GetVehicleComponentInSlot(vid, 9) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that slot.");
				removeSlotComponent(vid, 9);
			}
			if(listitem == 10)
			{
				if(GetVehicleComponentInSlot(vid, 10) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that slot.");
				removeSlotComponent(vid, 10);
			}
			if(listitem == 11)
			{
				if(GetVehicleComponentInSlot(vid, 11) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that slot.");
				removeSlotComponent(vid, 11);
			}
			if(listitem == 12)
			{
				if(GetVehicleComponentInSlot(vid, 12) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that slot.");
				removeSlotComponent(vid, 12);
			}
			if(listitem == 13)
			{
				if(GetVehicleComponentInSlot(vid, 13) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that slot.");
				removeSlotComponent(vid, 13);
			}
		}
	}
	if(dialogid == DIALOG_PAINTBALL_WEAPONS1) //handguns
	{
		if(!response)
		{
			ClearPaintballGuns(playerid);
			return 1;
		}
		if(listitem == 0)
		{
			pPaintGuns[playerid][1] = 22;
			format(PaintBall[playerid][pbWeaponName1], 32, "{FF0000}Colt 45{FFFFFF}");
			ShowPlayerDialog(playerid, DIALOG_PAINTBALL_WEAPONS2, DIALOG_STYLE_LIST, "Shotgun Selection", "Shotgun\nSawnoff Shotgun\nSpas 12", "Select", "Quit");
		}
		if(listitem == 1)
		{
			format(PaintBall[playerid][pbWeaponName1], 32, "{FF0000}Desert Eagle{FFFFFF}");
			pPaintGuns[playerid][1] = 24;
			ShowPlayerDialog(playerid, DIALOG_PAINTBALL_WEAPONS2, DIALOG_STYLE_LIST, "Shotgun Selection", "Shotgun\nSawnoff Shotgun\nSpas 12", "Select", "Quit");
		}
	}
	if(dialogid == DIALOG_MAP)
	{
		if(!response) return 1;
		else
		{
			switch(listitem)
			{
				case 0:
				{
					SendClientMessage(playerid, COLOR_GREY, "{FF0000}[Job CP]{FFFFFF} Follow the marker on your minimap to find the Detective job.");
					goingJob[playerid] = true;
					SetPlayerCheckpointEx(playerid, 1553.5276, -1675.4719, 16.1953, 5.0);
				}
				case 1:
				{
					SendClientMessage(playerid, COLOR_GREY, "{FF0000}[Job CP]{FFFFFF} Follow the marker on your minimap to find the Lawyer job.");
					goingJob[playerid] = true;
					SetPlayerCheckpointEx(playerid, 1381.0413, -1088.8511, 27.3906, 5.0);
				}
				case 2:
				{
					SendClientMessage(playerid, COLOR_GREY, "{FF0000}[Job CP]{FFFFFF} Follow the marker on your minimap to find the Drug Dealer job.");
					goingJob[playerid] = true;
					SetPlayerCheckpointEx(playerid, 2166.3772, -1675.3829, 15.0859, 5.0);
				}
				case 3:
				{
					SendClientMessage(playerid, COLOR_GREY, "{FF0000}[Job CP]{FFFFFF} Follow the marker on your minimap to find the Product Dealer job.");
					goingJob[playerid] = true;
					SetPlayerCheckpointEx(playerid, 972.0286, -1385.7682, 13.5477, 5.0);
				}
				case 4:
				{
					SendClientMessage(playerid, COLOR_GREY, "{FF0000}[Job CP]{FFFFFF} Follow the marker on your minimap to find the Mechanic job.");
					goingJob[playerid] = true;
					SetPlayerCheckpointEx(playerid, 2329.4089, -2316.0996, 13.5469, 5.0);
				}
				case 5:
				{
					SendClientMessage(playerid, COLOR_GREY, "{FF0000}[Job CP]{FFFFFF} Follow the marker on your minimap to find the Bodyguard job.");
					goingJob[playerid] = true;
					SetPlayerCheckpointEx(playerid, 2226.1716, -1718.1792, 13.5165, 5.0);
				}
				case 6:
				{
					SendClientMessage(playerid, COLOR_GREY, "{FF0000}[Job CP]{FFFFFF} Follow the marker on your minimap to find the Arms Dealer job.");
					goingJob[playerid] = true;
					SetPlayerCheckpointEx(playerid, 1366.4325, -1275.2096, 13.5469, 5.0);
				}
				case 7:
				{
					SendClientMessage(playerid, COLOR_GREY, "{FF0000}[Job CP]{FFFFFF} Follow the marker on your minimap to find the Boxer job.");
					goingJob[playerid] = true;
					SetPlayerCheckpointEx(playerid, 2227.9595, -1722.2257, 13.2633, 5.0);
				}
				case 8:
				{
					SendClientMessage(playerid, COLOR_GREY, "{FF0000}[Job CP]{FFFFFF} Follow the marker on your minimap to find the Taxi Driver job.");
					goingJob[playerid] = true;
					SetPlayerCheckpointEx(playerid, 1741.7062, -1863.6664, 13.5748, 5.0);
				}
				case 9:
				{
					SendClientMessage(playerid, COLOR_GREY, "{FF0000}[Job CP]{FFFFFF} Follow the marker on your minimap to find the Drug Smuggler job.");
					goingJob[playerid] = true;
					SetPlayerCheckpointEx(playerid, 2354.2703, -1169.3293, 28.0083, 5.0);
				}
				case 10:
				{
					SendClientMessage(playerid, COLOR_GREY, "{FF0000}[Job CP]{FFFFFF} Follow the marker on your minimap to find the San Andreas Police Department.");
					goingJob[playerid] = true;
					SetPlayerCheckpointEx(playerid, 1552.9044, -1675.5406, 16.1953, 5.0);
				}
				case 11:
				{
					SendClientMessage(playerid, COLOR_GREY, "{FF0000}[Job CP]{FFFFFF} Follow the marker on your minimap to find the Los Santos Bank.");
					goingJob[playerid] = true;
					SetPlayerCheckpointEx(playerid, 1462.2672, -1012.0541, 26.8438, 5.0);
				}
				case 12:
				{
					SendClientMessage(playerid, COLOR_GREY, "{FF0000}[Job CP]{FFFFFF} Follow the marker on your minimap to find the Los Santos Gym.");
					goingJob[playerid] = true;
					SetPlayerCheckpointEx(playerid, 2228.8279, -1721.8065, 13.5659, 5.0);
				}
				case 13:
				{
					SendClientMessage(playerid, COLOR_GREY, "{FF0000}[Job CP]{FFFFFF} Follow the marker on your minimap to find the Fire & Medical Department.");
					goingJob[playerid] = true;
					SetPlayerCheckpointEx(playerid, 1108.6631, -1337.4138, 13.8500, 5.0);
				}
				case 14:
				{
					SendClientMessage(playerid, COLOR_GREY, "{FF0000}[Job CP]{FFFFFF} Follow the marker on your minimap to find the SAN Station.");
					goingJob[playerid] = true;
					SetPlayerCheckpointEx(playerid, 648.2065, -1353.3262, 13.12, 5.0);
				}
			}
		}
	}
	if(dialogid == DIALOG_PAINTBALL_WEAPONS2) //shotguns
	{
		if(!response)
		{
			ClearPaintballGuns(playerid);
			return 1;
		}
		if(listitem == 0)
		{
			pPaintGuns[playerid][2] = 25;
			format(PaintBall[playerid][pbWeaponName2], 32, "{FF0000}Shotgun{FFFFFF}");
			ShowPlayerDialog(playerid, DIALOG_PAINTBALL_WEAPONS3, DIALOG_STYLE_LIST, "SMG Selection", "Uzi\nMP5\nTec-9", "Select", "Quit");
		}
		if(listitem == 1)
		{
			pPaintGuns[playerid][2] = 26;
			format(PaintBall[playerid][pbWeaponName2], 32, "{FF0000}Sawnoff Shotgun{FFFFFF}");
			ShowPlayerDialog(playerid, DIALOG_PAINTBALL_WEAPONS3, DIALOG_STYLE_LIST, "SMG Selection", "Uzi\nMP5\nTec-9", "Select", "Quit");
		}
		if(listitem == 2)
		{
			pPaintGuns[playerid][2] = 27;
			format(PaintBall[playerid][pbWeaponName2], 32, "{FF0000}Spas 12{FFFFFF}");
			ShowPlayerDialog(playerid, DIALOG_PAINTBALL_WEAPONS3, DIALOG_STYLE_LIST, "SMG Selection", "Uzi\nMP5\nTec-9", "Select", "Quit");
		}
	}
	if(dialogid == DIALOG_PAINTBALL_WEAPONS3)
	{
		if(!response)
		{
			ClearPaintballGuns(playerid);
			return 1;
		}
		if(listitem == 0)
		{
			pPaintGuns[playerid][3] = 28;
			format(PaintBall[playerid][pbWeaponName3], 32, "{FF0000}UZI{FFFFFF}");
			ShowPlayerDialog(playerid, DIALOG_PAINTBALL_WEAPONS4, DIALOG_STYLE_LIST, "Assault Selection", "AK-47\nM4", "Select", "Quit");
		}
		if(listitem == 1)
		{
			pPaintGuns[playerid][3] = 29;
			format(PaintBall[playerid][pbWeaponName3], 32, "{FF0000}MP5{FFFFFF}");
			ShowPlayerDialog(playerid, DIALOG_PAINTBALL_WEAPONS4, DIALOG_STYLE_LIST, "Assault Selection", "AK-47\nM4", "Select", "Quit");
		}
		if(listitem == 2)
		{
			pPaintGuns[playerid][3] = 32;
			format(PaintBall[playerid][pbWeaponName3], 32, "{FF0000}Tec-9{FFFFFF}");
			ShowPlayerDialog(playerid, DIALOG_PAINTBALL_WEAPONS4, DIALOG_STYLE_LIST, "Assault Selection", "AK-47\nM4", "Select", "Quit");
		}
	}
	if(dialogid == DIALOG_PAINTBALL_WEAPONS4)
	{
		if(!response)
		{
			ClearPaintballGuns(playerid);
			return 1;
		}
		if(listitem == 0)
		{
			pPaintGuns[playerid][4] = 30;
			format(PaintBall[playerid][pbWeaponName4], 32, "{FF0000}AK-47{FFFFFF}");
			ShowPlayerDialog(playerid, DIALOG_PAINTBALL_WEAPONS5, DIALOG_STYLE_LIST, "Rifle Selection", "County Rifle\nSniper Rifle", "Select", "Quit");
		}
		if(listitem == 1)
		{
			pPaintGuns[playerid][4] = 31;
			format(PaintBall[playerid][pbWeaponName4], 32, "{FF0000}M4{FFFFFF}");
			ShowPlayerDialog(playerid, DIALOG_PAINTBALL_WEAPONS5, DIALOG_STYLE_LIST, "Rifle Selection", "County Rifle\nSniper Rifle", "Select", "Quit");
		}
	}
	if(dialogid == DIALOG_PAINTBALL_WEAPONS5)
	{
		if(!response)
		{
			ClearPaintballGuns(playerid);
			return 1;
		}
		if(listitem == 0)
		{
			new pbjoinstr[1000];
			pPaintGuns[playerid][5] = 33;
			format(PaintBall[playerid][pbWeaponName5], 32, "{FF0000}County Rifle{FFFFFF}");
			format(pbjoinstr, sizeof(pbjoinstr), "{FFFFFF}Handgun: %s\nShotgun: %s\nSMG: %s\nAssault Rifle: %s\nRifle: %s\n\n\n{FFFF00}If you're sure you want to join with these weapons, press 'Enter'.", PaintBall[playerid][pbWeaponName1], PaintBall[playerid][pbWeaponName2], PaintBall[playerid][pbWeaponName3], PaintBall[playerid][pbWeaponName4], PaintBall[playerid][pbWeaponName5]);
			ShowPlayerDialog(playerid, DIALOG_PAINTBALL_WEAPONS6, DIALOG_STYLE_MSGBOX, "Paintball Confirmation", pbjoinstr, "Enter", "Quit");
		}
		if(listitem == 1)
		{
			new pbjoinstr[1000];
			pPaintGuns[playerid][5] = 34;
			format(PaintBall[playerid][pbWeaponName5], 32, "{FF0000}Sniper Rifle{FFFFFF}");
			format(pbjoinstr, sizeof(pbjoinstr), "{FFFFFF}Handgun: %s\nShotgun: %s\nSMG: %s\nAssault Rifle: %s\nRifle: %s\n\n\n{FFFF00}If you're sure you want to join with these weapons, press 'Enter'.", PaintBall[playerid][pbWeaponName1], PaintBall[playerid][pbWeaponName2], PaintBall[playerid][pbWeaponName3], PaintBall[playerid][pbWeaponName4], PaintBall[playerid][pbWeaponName5]);
			ShowPlayerDialog(playerid, DIALOG_PAINTBALL_WEAPONS6, DIALOG_STYLE_MSGBOX, "Paintball Confirmation", pbjoinstr, "Enter", "Quit");
		}
	}
	if(dialogid == DIALOG_PAINTBALL_WEAPONS6)
	{
		if(!response)
		{
			ClearPaintballGuns(playerid);
			return 1;
		}
		else
		{
			enter_pb(playerid);
		}
	}
	if(dialogid == DIALOG_LOGIN)
	{
		if(response)
		{
			if(!gamemodeLoaded)
			{
				ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login", "{FFFFFF}Welcome back to {FFFF00}San Andreas Role Play{FFFFFF}.\n\nPlease enter your password below to login to your account:", "Login", "Quit");
				SendClientMessage(playerid, COLOR_WHITE, "{F00000}SERVER{FFFFFF}: Please wait until the gamemode has finished loading.");
				return 1;
			}
			if(!strlen(inputtext))
			{
				ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login", "{FFFFFF}Welcome back to {FFFF00}San Andreas Role Play{FFFFFF}.\n\nPlease enter your password below to login to your account:", "Login", "Quit");
				SendClientMessage(playerid, COLOR_WHITE, "{F00000}SERVER{FFFFFF}: You must enter a password.");
				return 1;
			}
			if(strlen(inputtext) > 64)
			{
				ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login", "{FFFFFF}Welcome back to {FFFF00}San Andreas Role Play{FFFFFF}.\n\nPlease enter your password below to login to your account:", "Login", "Quit");
				SendClientMessage(playerid, COLOR_WHITE, "{F00000}SERVER{FFFFFF}: Password is too long.");
				return 1;
			}
			if(gPlayerLogged[playerid])
			{
				SendClientMessage(playerid, COLOR_WHITE, "{F00000}SERVER{FFFFFF}: You are already logged in.");
				return 1;
			}


			//\\// Password & Verification
			//=============================
			new bool: correctpass = false;
			if(isnull(NewPassword[playerid])) {

				//password in old shitty method
				if(isnull(tempHash[playerid])) { 
					new encrypted_password[65];
					strmid(encrypted_password, inputtext, 0, strlen(inputtext), 255);

					Encrypt(encrypted_password);
					if(strcmp(encrypted_password, tempPassword[playerid], true) == 0) { //password matches Key_ field

						HashPassword(inputtext, playerid);
						correctpass = true;
					}

				//password in buggy SHA_256 -- warning remove this on SA:MP 0.3.8 update, replace with error message for manual password change
				//SHA_256 was buggy in SA:MP 0.3.7 RC-X and has been fixed hence will no longer match correct passwords
				} else { 
					new password[65];

					strmid(password, inputtext, 0, strlen(inputtext), 255);
					SHA256_PassHash(password, tempSalt[playerid], password, sizeof(password));

					//SHA_256 matches
					if(strcmp(password, tempHash[playerid], true) == 0) {

						HashPassword(inputtext, playerid);
						correctpass = true;
					}
				}
			}
			//Use the new password method
			else {
				if(CheckPassword(inputtext, NewPassword[playerid], NewSalt[playerid], playerid)) {
					correctpass = true;
				}
			}

			//Correct password?
			if(correctpass) {
				new query[128];
				mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `players` WHERE `name` = '%e' LIMIT 1", PlayerName(playerid));
				mysql_pquery(sqlGameConnection, query, "OnPlayerLoad", "dd", playerid, MySQLConnectionOrder[playerid]);
			}
			//If not, try again
			else {
				if(LoginAttempts[playerid] >= 2) {
					SendClientMessage(playerid, COLOR_WHITE, "{F00000}SERVER{FFFFFF}: Incorrect password, you have been kicked as a result.");
					KickEx(playerid);
					return 1;
				}

				new msgstring[128];
				format(msgstring, sizeof(msgstring), "{FFFFFF}Welcome back to {FFFF00}San Andreas Role Play{FFFFFF}.\n\nPlease enter your password below to login to your account:", msgstring);
				ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login", msgstring, "Login", "Quit");
				displayCenterHUDInfo(playerid,  "Incorrect password.", 8);
				LoginAttempts[playerid]++;
			}
			return 1;
		}
		else
		{
			SendClientMessage(playerid, COLOR_WHITE, "{F00000}SERVER{FFFFFF}: You have left the server.");
			KickEx(playerid);
		}
		return 1;
	}
	if(dialogid == DIALOG_TAXI)
	{
		if(response)
		{
			if(listitem == 0)
			{
				ShowPlayerDialog(playerid, DIALOG_TAXI_UNIFORMS, DIALOG_STYLE_LIST, "Taxi Uniforms", "Woman (ID 211)\nTrainee (ID 255)\nRookie (ID 261)\nTaxi Driver (ID 61)\nCabbie (ID 234)\nVeteran (ID 188)\nSupervisor (ID 253)\nCommissioner (ID 147)", "Select", "Cancel");
			}
			if(listitem == 1)
			{
				ShowPlayerDialog(playerid, DIALOG_TAXI_WEAPONS, DIALOG_STYLE_LIST, "Taxi Armory", "Deagle ($25,000)\nKevlar ($5,000)", "Select", "Cancel");
			}
		}
	}
	if(dialogid == DIALOG_TAXI_UNIFORMS)
	{
		if(!response) return 1;
		PlayerInfo[playerid][pClothes] = 0;
		if(listitem == 0)
		{
			PlayerInfo[playerid][pModel] = 211;
			SetPlayerSkin(playerid, PlayerInfo[playerid][pModel]);
		}
		if(listitem == 1)
		{
			PlayerInfo[playerid][pModel] = 255;
			SetPlayerSkin(playerid, PlayerInfo[playerid][pModel]);
		}
		if(listitem == 2)
		{
			PlayerInfo[playerid][pModel] = 261;
			SetPlayerSkin(playerid, PlayerInfo[playerid][pModel]);
		}
		if(listitem == 3)
		{
			PlayerInfo[playerid][pModel] = 61;
			SetPlayerSkin(playerid, PlayerInfo[playerid][pModel]);
		}
		if(listitem == 4)
		{
			PlayerInfo[playerid][pModel] = 234;
			SetPlayerSkin(playerid, PlayerInfo[playerid][pModel]);
		}
		if(listitem == 5)
		{
			PlayerInfo[playerid][pModel] = 188;
			SetPlayerSkin(playerid, PlayerInfo[playerid][pModel]);
		}
		if(listitem == 6)
		{
			PlayerInfo[playerid][pModel] = 253;
			SetPlayerSkin(playerid, PlayerInfo[playerid][pModel]);
		}
		if(listitem == 7)
		{
			PlayerInfo[playerid][pModel] = 147;
			SetPlayerSkin(playerid, PlayerInfo[playerid][pModel]);
		}
	}
	if(dialogid == DIALOG_TAXI_WEAPONS)
	{
		if(!response) return 1;
		if(listitem == 0)
		{
			if(PlayerInfo[playerid][pCash] >= 25000)
			{
				GivePlayerGun(playerid, 24);
				GiveMoney(playerid, -25000);
			}
			else return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
		}
		if(listitem == 1)
		{
			if(PlayerInfo[playerid][pCash] >= 5000)
			{
				SetPlayerArmour(playerid, 100.0);
				GiveMoney(playerid, -5000);
			}
			else return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
		}
	}
	if(dialogid == DIALOG_CRIME)
	{
		if(response)
		{
			new money = PlayerInfo[playerid][pCash];
			switch(listitem)
			{
				case 0: // Desert Eagle
				{
					if(money >= 10000) {
						GiveMoney(playerid, -10000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_DEAGLE);
						SendClientMessage(playerid, COLOR_CRIME, "You have purchased a Desert Eagle for $10,000.");
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}
				}
				case 1: // SD Pistol
				{
					if(money >= 5000) {
						GiveMoney(playerid, -5000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_SILENCED);
						SendClientMessage(playerid, COLOR_CRIME, "You have purchased an SD Pistol for $5,000.");
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}
				}
				case 2: // Break
				{
					showCrimeDialog(playerid);
				}
				case 3: // M4
				{
					if(money >= 25000) {
						GiveMoney(playerid, -25000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_M4);
						SendClientMessage(playerid, COLOR_CRIME, "You have purchased an M4 for $25,000.");
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}
				}
				case 4: // AK47
				{
					if(money >= 20000) {
						GiveMoney(playerid, -20000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_AK47);
						SendClientMessage(playerid, COLOR_CRIME, "You have purchased an AK47 for $20,000.");
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}
				}
				case 5: // Break
				{
					showCrimeDialog(playerid);
				}
				case 6: // Spas12
				{
					if(money >= 50000) {
						GiveMoney(playerid, -50000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_SHOTGSPA);
						SendClientMessage(playerid, COLOR_CRIME, "You have purchased a combat shotgun for $50,000.");
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}
				}
				case 7: // Shotgun
				{
					if(money >= 5000) {
						GiveMoney(playerid, -5000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_SHOTGUN);
						SendClientMessage(playerid, COLOR_CRIME, "You have purchased a shotgun for $5,000.");
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}
				}
				case 8: // Sawn off
				{
					if(money >= 8000) {
						GiveMoney(playerid, -8000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_SAWEDOFF);
						SendClientMessage(playerid, COLOR_CRIME, "You have purchased a sawn-off shotgun for $8,000.");
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}
				}
				case 9: // Break
				{
					showCrimeDialog(playerid);
				}
				case 10: // MP5
				{
					if(money >= 2500) {
						GiveMoney(playerid, -2500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_MP5);
						SendClientMessage(playerid, COLOR_CRIME, "You have purchased an MP5 for $2,500.");
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}
				}
				case 11: // TEC9
				{
					if(money >= 2500) {
						GiveMoney(playerid, -2500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_TEC9);
						SendClientMessage(playerid, COLOR_CRIME, "You have purchased a TEC9 for $2,500.");
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}
				}
				case 12: // Uzi
				{
					if(money >= 2500) {
						GiveMoney(playerid, -2500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_UZI);
						SendClientMessage(playerid, COLOR_CRIME, "You have purchased an Uzi for $2,500.");
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}
				}
				case 13: // Break
				{
					showCrimeDialog(playerid);
				}
				case 14: // Rifle
				{
					if(money >= 5000) {
						GiveMoney(playerid, -5000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_RIFLE);
						SendClientMessage(playerid, COLOR_CRIME, "You have purchased a country rifle for $2,500.");
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}
				}
				case 15: // Sniper Rifle
				{
					if(money >= 35000) {
						GiveMoney(playerid, -35000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_SNIPER);
						SendClientMessage(playerid, COLOR_CRIME, "You have purchased a sniper rifle for $35,000.");
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}
				}
				case 16: // Break
				{
					showCrimeDialog(playerid);
				}
				case 17: // Flamethrower
				{
					if(PlayerInfo[playerid][pGroupRank] < 3)
						return SendClientMessage(playerid, COLOR_CRIME, "You need to be R3+ to use this.");

					if(money >= 50000) {
						GiveMoney(playerid, -50000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_FLAMETHROWER);
						SendClientMessage(playerid, COLOR_CRIME, "You have purchased a flamethrower for $50,000.");
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}
				}
				case 18: // C4
				{
					if(PlayerInfo[playerid][pGroupRank] < 2)
						return SendClientMessage(playerid, COLOR_GREY, "You aren't high enough rank.");

					if(money >= 25000) {
						GiveMoney(playerid, -25000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						PlayerInfo[playerid][pBombs] += 1;

						SendClientMessage(playerid, COLOR_CRIME, "You have purchased a block of plastic explosive for $25,000.");
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}
				}
				case 19: // Break
				{
					showCrimeDialog(playerid);
				}
				case 20: // Chainsaw
				{
					if(PlayerInfo[playerid][pGroupRank] < 3)
						return SendClientMessage(playerid, COLOR_GREY, "You aren't high enough rank.");

					if(money >= 20000) {
						GiveMoney(playerid, -20000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_CHAINSAW);
						SendClientMessage(playerid, COLOR_CRIME, "You have purchased a chainsaw for $20,000.");
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}
				}
				case 21: // Knife
				{
					if(money >= 50000) {
						GiveMoney(playerid, -50000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_KNIFE);
						SendClientMessage(playerid, COLOR_CRIME, "You have purchased a knife for $50,000.");
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}
				}
				case 22: // Break
				{
					showCrimeDialog(playerid);
				}
				case 23: // Tear Gas
				{
					if(money >= 5000) {
						GiveMoney(playerid, -5000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_TEARGAS);
						SendClientMessage(playerid, COLOR_CRIME, "You have purchased two canisters of tear gas for $5,000.");
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}
				}
				case 24: // Grenades
				{
					SendClientMessage(playerid, COLOR_GREY, "Grenades have been disabled.");
					/*if(money >= 2500) {
						GiveMoney(playerid, -2500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_GRENADE);
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}*/
				}
				case 25: // Molotov Cocktail
				{
					if(PlayerInfo[playerid][pGroupRank] < 3)
						return SendClientMessage(playerid, COLOR_GREY, "You aren't high enough rank.");
					
					if(money >= 5000) {
						GiveMoney(playerid, -5000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_MOLTOV);
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}
				}
				case 26: // Break
				{
					showCrimeDialog(playerid);
				}
				case 27: // Health and Armour
				{
					if(money >= 4000) {
						GiveMoney(playerid, -4000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						SetPlayerHealth(playerid, 100.0);
						SetPlayerArmour(playerid, 100.0);
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}
				}
				case 28: // Half Vest
				{
					if(money >= 1000) {
						GiveMoney(playerid, -1000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						SetPlayerArmour(playerid, 50.0);
					} else {
						SendClientMessage(playerid, COLOR_CRIME, "You can't afford that.");
					}
				}
				case 29: // Break
				{
					showCrimeDialog(playerid);
				}
				case 30: // Jammer
				{
					if(money >= 20000)
					{
						GiveMoney(playerid, -20000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						PlayerInfo[playerid][pJammer] += 1;
						SendClientMessage(playerid, COLOR_CRIME, "* You have purchased a signal jammer for $20,000.");
					}
					else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money for that.");
				}
				case 31: // Dufflebag
				{
					SendClientMessage(playerid, COLOR_CRIME, "TODO: Script Dufflebags");
				}
				case 32: // Namechange
				{
					ShowPlayerDialog(playerid, DIALOG_SYNDICATE_NAMECHANGE, DIALOG_STYLE_INPUT, "{646495}Name Change", "Abuse will result in a ban and removal.\n{FF0000}Remember to use an underscore to seperate fore and surnames. Spaces will bug your account!", "Accept", "Back");
				}
				case 33: // Break
				{
					showCrimeDialog(playerid);
				}
				case 34: // Fully Armed
				{
					if(money >= 200000)
					{
						GiveMoney(playerid, -200000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);

						SetPlayerArmour(playerid, 100.0);
						SetPlayerHealth(playerid, 100.0);

						GivePlayerGun(playerid, WEAPON_DEAGLE);
						GivePlayerGun(playerid, WEAPON_M4);
						GivePlayerGun(playerid, WEAPON_SHOTGSPA);
						GivePlayerGun(playerid, WEAPON_MP5);
						GivePlayerGun(playerid, WEAPON_SNIPER);
						GivePlayerGun(playerid, WEAPON_KNIFE);

						SendClientMessage(playerid, COLOR_CRIME, "* You have purchased a full kit for $200,000.");
					}
					else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money for that.");
				}
			}
		}
	}
	if(dialogid == DIALOG_ARMORY) {
		if(!response)
			return 1;

		switch(listitem) {
			case 0: {
				if(GetMoney(playerid) < 500)
					return SendClientMessage(playerid, COLOR_GREY, "You cannot afford this.");

				GivePlayerGun(playerid, WEAPON_COLT45);
				GiveMoney(playerid, -500);
			}
			case 1: {
				if(GetMoney(playerid) < 1000)
					return SendClientMessage(playerid, COLOR_GREY, "You cannot afford this.");

				GivePlayerGun(playerid, WEAPON_MP5);
				GiveMoney(playerid, -1000);
			}
			case 2: {
				if(GetMoney(playerid) < 1000)
					return SendClientMessage(playerid, COLOR_GREY, "You cannot afford this.");

				GivePlayerGun(playerid, WEAPON_SHOTGUN);
				GiveMoney(playerid, -1000);
			}
			case 3: {
				if(GetMoney(playerid) < 10000)
					return SendClientMessage(playerid, COLOR_GREY, "You cannot afford this.");

				GivePlayerGun(playerid, WEAPON_DEAGLE);
				GiveMoney(playerid, -10000);
			}
			case 4: {
				if(GetMoney(playerid) < 10000)
					return SendClientMessage(playerid, COLOR_GREY, "You cannot afford this.");

				GivePlayerGun(playerid, WEAPON_RIFLE);
				GiveMoney(playerid, -10000);
			}
			case 5: {
				if(GetMoney(playerid) < 25000)
					return SendClientMessage(playerid, COLOR_GREY, "You cannot afford this.");

				GivePlayerGun(playerid, WEAPON_M4);
				GiveMoney(playerid, -25000);
			}
			case 6: {
				if(GetMoney(playerid) < 15000)
					return SendClientMessage(playerid, COLOR_GREY, "You cannot afford this.");

				GivePlayerGun(playerid, WEAPON_AK47);
				GiveMoney(playerid, -15000);
			}
			case 7: {
				if(GetMoney(playerid) < 35000)
					return SendClientMessage(playerid, COLOR_GREY, "You cannot afford this.");

				GivePlayerGun(playerid, WEAPON_SNIPER);
				GiveMoney(playerid, -35000);
			}
			case 8: {
				if(GetMoney(playerid) < 35000)
					return SendClientMessage(playerid, COLOR_GREY, "You cannot afford this.");

				GivePlayerGun(playerid, WEAPON_SHOTGSPA);
				GiveMoney(playerid, -35000);
			}
			case 9: {
				if(GetMoney(playerid) < 40000)
					return SendClientMessage(playerid, COLOR_GREY, "You cannot afford this.");

				GivePlayerGun(playerid, WEAPON_CHAINSAW);
				GiveMoney(playerid, -40000);
			}
			case 10: {
				if(GetMoney(playerid) < 10000)
					return SendClientMessage(playerid, COLOR_GREY, "You cannot afford this.");

				SetPlayerArmour(playerid, 100);
				GiveMoney(playerid, -10000);
			}
			case 11: {
				if(GetMoney(playerid) < 15)
					return SendClientMessage(playerid, COLOR_GREY, "You cannot afford this.");

				SetPlayerHealth(playerid, 100);
				GiveMoney(playerid, -15);
			}
			case 12: {
				if(GetMoney(playerid) < 10000)
					return SendClientMessage(playerid, COLOR_GREY, "You cannot afford this.");

				GivePlayerGun(playerid, WEAPON_SAWEDOFF);
				GiveMoney(playerid, -10000);
			}
			case 13: {
				if(GetMoney(playerid) < 10000)
					return SendClientMessage(playerid, COLOR_GREY, "You cannot afford this.");

				GivePlayerGun(playerid, WEAPON_SILENCED);
				GiveMoney(playerid, -10000);
			}
			case 14: {
				if(GetMoney(playerid) < 15000)
					return SendClientMessage(playerid, COLOR_GREY, "You cannot afford this.");

				GivePlayerGun(playerid, WEAPON_MOLTOV);
				GiveMoney(playerid, -15000);
			}
			case 15: {
				if(GetMoney(playerid) < 25000)
					return SendClientMessage(playerid, COLOR_GREY, "You cannot afford this.");

				GivePlayerGun(playerid, WEAPON_UZI);
				GiveMoney(playerid, -25000);
			}
			case 16: {
				if(GetMoney(playerid) < 25000)
					return SendClientMessage(playerid, COLOR_GREY, "You cannot afford this.");

				GivePlayerGun(playerid, WEAPON_TEC9);
				GiveMoney(playerid, -25000);
			}
			case 17: {
				if(GetMoney(playerid) < 10000)
					return SendClientMessage(playerid, COLOR_GREY, "You cannot afford this.");

				PlayerInfo[playerid][pJammer] = 1;
				GiveMoney(playerid, -10000);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "Signal jammer acquired.");
			}
			case 18: {
				if(GetMoney(playerid) < 10000)
					return SendClientMessage(playerid, COLOR_GREY, "You cannot afford this.");

				GivePlayerGun(playerid, WEAPON_TEARGAS);
				GiveMoney(playerid, -10000);
			}
			case 19: {
				SendClientMessage(playerid, COLOR_GREY, "Grenades have been disabled");
				/*
				if(GetMoney(playerid) < 10000)
					return SendClientMessage(playerid, COLOR_GREY, "You cannot afford this.");

				GivePlayerGun(playerid, WEAPON_GRENADE);
				GiveMoney(playerid, -10000);*/
			}
		}

		PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
	}
	if(dialogid == DIALOG_SYNDICATE_NAMECHANGE)
	{
		if(!response || !strlen(inputtext))
			return showCrimeDialog(playerid);

		new query[128];
		mysql_format(sqlGameConnection, query, sizeof(query), "SELECT `Name` FROM `players` WHERE `Name` = '%e'", inputtext);
		mysql_pquery(sqlGameConnection, query, "OnSetName", "iis", playerid, playerid, inputtext);
	}
	if(dialogid == DIALOG_HITMAN)
	{
		if(response)
		{
			if(!IsPlayerNearHitmenHQ(playerid))
				return SendClientMessage(playerid, COLOR_GREY, "You are not at your headquarters.");

			new money = PlayerInfo[playerid][pCash];
			switch(listitem)
			{
				case 0: // Desert Eagle
				{
					if(money >= 10000) {
						GiveMoney(playerid, -10000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_DEAGLE);
						SendClientMessage(playerid, COLOR_HITMAN, "You have purchased a Desert Eagle for $10,000.");
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 1: // SD Pistol
				{
					if(money >= 5000) {
						GiveMoney(playerid, -5000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_SILENCED);
						SendClientMessage(playerid, COLOR_HITMAN, "You have purchased an SD Pistol for $5,000.");
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 2: // Break
				{
					showHitmanDialog(playerid);
				}
				case 3: // M4
				{
					if(money >= 25000) {
						GiveMoney(playerid, -25000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_M4);
						SendClientMessage(playerid, COLOR_HITMAN, "You have purchased an M4 for $25,000.");
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 4: // AK47
				{
					if(money >= 20000) {
						GiveMoney(playerid, -20000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_AK47);
						SendClientMessage(playerid, COLOR_HITMAN, "You have purchased an AK47 for $20,000.");
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 5: // Break
				{
					showHitmanDialog(playerid);
				}
				case 6: // Spas12
				{
					if(money >= 50000) {
						GiveMoney(playerid, -50000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_SHOTGSPA);
						SendClientMessage(playerid, COLOR_HITMAN, "You have purchased a combat shotgun for $50,000.");
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 7: // Shotgun
				{
					if(money >= 2000) {
						GiveMoney(playerid, -2000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_SHOTGUN);
						SendClientMessage(playerid, COLOR_HITMAN, "You have purchased a shotgun for $2,000.");
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 8: // Sawn off
				{
					if(money >= 8000) {
						GiveMoney(playerid, -8000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_SAWEDOFF);
						SendClientMessage(playerid, COLOR_HITMAN, "You have purchased a sawn-off shotgun for $8,000.");
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 9: // Break
				{
					showHitmanDialog(playerid);
				}
				case 10: // MP5
				{
					if(money >= 2500) {
						GiveMoney(playerid, -2500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_MP5);
						SendClientMessage(playerid, COLOR_HITMAN, "You have purchased an MP5 for $2,500.");
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 11: // TEC9
				{
					if(money >= 2500) {
						GiveMoney(playerid, -2500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_TEC9);
						SendClientMessage(playerid, COLOR_HITMAN, "You have purchased a TEC9 for $2,500.");
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 12: // Uzi
				{
					if(money >= 2500) {
						GiveMoney(playerid, -2500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_UZI);
						SendClientMessage(playerid, COLOR_HITMAN, "You have purchased an Uzi for $2,500.");
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 13: // Break
				{
					showHitmanDialog(playerid);
				}
				case 14: // Rifle
				{
					if(money >= 5000) {
						GiveMoney(playerid, -5000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_RIFLE);
						SendClientMessage(playerid, COLOR_HITMAN, "You have purchased a country rifle for $5,000.");
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 15: // Sniper Rifle
				{
					if(PlayerInfo[playerid][pGroupRank] < 4) return SendClientMessage(playerid, COLOR_HITMAN, "You must be R4+ to purchase this.");
					if(money >= 45000) {
						GiveMoney(playerid, -45000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_SNIPER);
						SendClientMessage(playerid, COLOR_HITMAN, "You have purchased a sniper rifle for $45,000.");
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 16: // Break
				{
					showHitmanDialog(playerid);
				}
				case 17: // Flamethrower
				{
					if(PlayerInfo[playerid][pGroupRank] < 4) return SendClientMessage(playerid, COLOR_HITMAN, "You must be R4+ to purchase this.");
					if(PlayerInfo[playerid][pGroupRank] < 3)
						return SendClientMessage(playerid, COLOR_HITMAN, "You need to be R3+ to use this.");

					if(money >= 50000) {
						GiveMoney(playerid, -50000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_FLAMETHROWER);
						SendClientMessage(playerid, COLOR_HITMAN, "You have purchased a flamethrower for $50,000.");
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 18: // C4
				{
					if(PlayerInfo[playerid][pGroupRank] < 4) return SendClientMessage(playerid, COLOR_HITMAN, "You must be R4+ to purchase this.");
					if(money >= 20000) {
						GiveMoney(playerid, -20000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						PlayerInfo[playerid][pBombs] += 1;

						SendClientMessage(playerid, COLOR_HITMAN, "You have purchased a block of plastic explosive for $20,000.");
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 19: // Break
				{
					showHitmanDialog(playerid);
				}
				case 20: // Chainsaw
				{
					if(PlayerInfo[playerid][pGroupRank] < 4) return SendClientMessage(playerid, COLOR_HITMAN, "You must be R4+ to purchase this.");
					if(money >= 40000) {
						GiveMoney(playerid, -40000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_CHAINSAW);
						SendClientMessage(playerid, COLOR_HITMAN, "You have purchased a chainsaw for $40,000.");
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 21: // Knife
				{
					if(money >= 50000) {
						GiveMoney(playerid, -50000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_KNIFE);
						SendClientMessage(playerid, COLOR_HITMAN, "You have purchased a knife for $50,000.");
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 22: // Break
				{
					showHitmanDialog(playerid);
				}
				case 23: // Tear Gas
				{
					if(PlayerInfo[playerid][pGroupRank] < 4) return SendClientMessage(playerid, COLOR_HITMAN, "You must be R4+ to purchase this.");
					if(money >= 5000) {
						GiveMoney(playerid, -5000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_TEARGAS);
						SendClientMessage(playerid, COLOR_HITMAN, "You have purchased two canisters of tear gas for $5,000.");
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 24: // Grenades
				{
					SendClientMessage(playerid, COLOR_GREY, "Grenades have been disabled.");
					/*
					if(PlayerInfo[playerid][pGroupRank] < 4) return SendClientMessage(playerid, COLOR_HITMAN, "You must be R4+ to purchase this.");
					if(money >= 2500) {
						GiveMoney(playerid, -2500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_GRENADE);
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}*/
				}
				case 25: // Molotov Cocktail
				{
					if(PlayerInfo[playerid][pGroupRank] < 4) return SendClientMessage(playerid, COLOR_HITMAN, "You must be R4+ to purchase this.");
					if(money >= 5000) {
						GiveMoney(playerid, -5000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_MOLTOV);
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 26: // Break
				{
					showHitmanDialog(playerid);
				}
				case 27: // Health and Armour
				{
					if(money >= 4000) {
						GiveMoney(playerid, -4000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						SetPlayerHealth(playerid, 100.0);
						SetPlayerArmour(playerid, 100.0);
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 28: // First Aid Kit
				{
					if(money >= 2000) {
						GiveMoney(playerid, -2000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						SetPlayerHealth(playerid, 100.0);
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 29: // Half Vest
				{
					if(money >= 1000) {
						GiveMoney(playerid, -1000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						SetPlayerArmour(playerid, 50.0);
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 30: // Full Vest
				{
					if(money >= 2000) {
						GiveMoney(playerid, -2000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						SetPlayerArmour(playerid, 100.0);
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 31: // Break
				{
					showHitmanDialog(playerid);
				}
				case 32: // Jammer
				{
					if(money >= 20000)
					{
						GiveMoney(playerid, -20000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						PlayerInfo[playerid][pJammer] += 1;
						SendClientMessage(playerid, COLOR_HITMAN, "* You have purchased a signal jammer for $20,000.");
					}
					else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money for that.");
				}
				case 33: // Poison
				{
					if(money >= 5000)
					{
						PlayerInfo[playerid][pPoison] += 1;
						GiveMoney(playerid, -5000);
						SendClientMessage(playerid, COLOR_HITMAN, "You have purchased some cyanide.");
					} else {
						SendClientMessage(playerid, COLOR_HITMAN, "You can't afford that.");
					}
				}
				case 34: // Fake Badge
				{
					if(PlayerInfo[playerid][pGroupRank] < 4) return SendClientMessage(playerid, COLOR_HITMAN, "You must be R4+ to use fake badges.");
					new dialogstring[128];
 					format(dialogstring, sizeof(dialogstring), "[%d] SAPD\n[%d] DEA\n[%d] SASD\n[%d] SAFMD\n[%d] NG\n[%d] Government\n[%d] Judicial Department\n[%d] DEA\n[%d] ATF", HitmanBadgeSAPD, HitmanBadgeFBI, HitmanBadgeRCSD, HitmanBadgeLSFMD, HitmanBadgeNG, HitmanBadgeGOV, HitmanBadgeSCOTUS, HitmanBadgeDEA, HitmanBadgeATF);
					ShowPlayerDialog(playerid, 6493, DIALOG_STYLE_LIST, "Fake Badges", dialogstring, "Choose", "Cancel");
				}
				case 35: // Lock pick
				{
					if(money >= 10000)
					{
						if(PlayerInfo[playerid][pHasLockPick] == 1)
							return SendClientMessage(playerid, COLOR_GREY, "You already have a lock pick kit.");

						GiveMoney(playerid, -10000);
						PlayerInfo[playerid][pHasLockPick] = 1;

						SendClientMessage(playerid, COLOR_HITMAN, "* You have purchased a lock pick kit for $10,000.");
					}
					else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money for that.");
				}
			}
		}
	}
	if(dialogid == 9082)
	{
		if(response)
		{
			new money = PlayerInfo[playerid][pCash];
			if(listitem == 0) // Desert Eagle
			{
				if(money >= 500)
				{
					PlayerInfo[playerid][pCash] -= 10000;
					GivePlayerGun(playerid, 24);
				}
				else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money for that.");
			}
			if(listitem == 1) // M4
			{
				if(money >= 20000)
				{
					PlayerInfo[playerid][pCash] -= 20000;
					GivePlayerGun(playerid, 31);
				}
				else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money for that.");
			}
			if(listitem == 2) // AK47
			{
				if(money >= 20000)
				{
					PlayerInfo[playerid][pCash] -= 20000;
					GivePlayerGun(playerid, 30);
				}
				else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money for that.");
			}
			if(listitem == 3) // Sniper Rifle
			{
				if(money >= 50000)
				{
					PlayerInfo[playerid][pCash] -= 50000;
					GivePlayerGun(playerid, 34);
				}
				else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money for that.");
			}
			if(listitem == 4) // Health
			{
				SetPlayerHealth(playerid, 100);
			}
			if(listitem == 4) // Armor
			{
				if(money >= 10000)
				{
					PlayerInfo[playerid][pCash] -= 10000;
					SetPlayerArmour(playerid, 100);
				}
			}
		}
	}
	if(dialogid == DIALOG_BAR_DYN)
	{
		if(response)
		{
			new Float:playerHP;
			GetPlayerHealth(playerid, playerHP);
			new string[128];
			if(listitem == 0) //beer
			{
				if(GetPlayerMoney(playerid) > 59)
				{
					if(playerHP >=90)
					{
						SetPlayerHealth(playerid, 100.0);
					}
					else
					{
						SetPlayerHealth(playerid, playerHP + 10.0);
					}
					GivePlayerMoney(playerid, -60);
					SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_BEER);
					format(string, sizeof(string), "~r~-$%d", 60);
					GameTextForPlayer(playerid, string, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					SendClientMessage(playerid, 0xAFAFAFAA, "Beer purchased.");
					return 1;
				}
				else SendClientMessage(playerid, 0xAFAFAFAA, "Life isn't free, soda too, get 3USD then come back.");
			}
			if(listitem == 1) //vodka
			{
				if(GetPlayerMoney(playerid) > 99)
				{
					if(playerHP >=80)
					{
						SetPlayerHealth(playerid, 100.0);
					}
					else
					{
						SetPlayerHealth(playerid, playerHP + 20.0);
					}
					GivePlayerMoney(playerid, -100);
					SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_WINE);
					format(string, sizeof(string), "~r~-$%d", 100);
					GameTextForPlayer(playerid, string, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					SendClientMessage(playerid, 0xAFAFAFAA, "Vodka purchased.");
					return 1;
				}
				else SendClientMessage(playerid, 0xAFAFAFAA, "Life isn't free, soda too, get 3USD then come back.");
			}
			if(listitem == 2) //whiskey
			{
				if(GetPlayerMoney(playerid) > 99)
				{
					if(playerHP >=80)
					{
						SetPlayerHealth(playerid, 100.0);
					}
					else
					{
						SetPlayerHealth(playerid, playerHP + 20.0);
					}
					GivePlayerMoney(playerid, -100);
					SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_WINE);
					format(string, sizeof(string), "~r~-$%d", 100);
					GameTextForPlayer(playerid, string, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					SendClientMessage(playerid, 0xAFAFAFAA, "Whiskey purchased.");
					return 1;
				}
				else SendClientMessage(playerid, 0xAFAFAFAA, "Life isn't free, soda too, get 3USD then come back.");
			}
			if(listitem == 3) //wine
			{
				if(GetPlayerMoney(playerid) > 99)
				{
					if(playerHP >=80)
					{
						SetPlayerHealth(playerid, 100.0);
					}
					else
					{
						SetPlayerHealth(playerid, playerHP + 20.0);
					}
					GivePlayerMoney(playerid, -100);
					SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_WINE);
					format(string, sizeof(string), "~r~-$%d", 100);
					GameTextForPlayer(playerid, string, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					SendClientMessage(playerid, 0xAFAFAFAA, "Wine purchased.");
					return 1;
				}
				else SendClientMessage(playerid, 0xAFAFAFAA, "Life isn't free, soda too, get 3USD then come back.");
			}
			if(listitem == 4) //soda
			{
				if(GetPlayerMoney(playerid) > 49)
				{
					if(playerHP >=80)
					{
						SetPlayerHealth(playerid, 100.0);
					}
					else
					{
						SetPlayerHealth(playerid, playerHP + 20.0);
					}
					GivePlayerMoney(playerid, -50);
					SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);
					format(string, sizeof(string), "~r~-$%d", 50);
					GameTextForPlayer(playerid, string, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					SendClientMessage(playerid, 0xAFAFAFAA, "Soda purchased.");
					return 1;
				}
				else SendClientMessage(playerid, 0xAFAFAFAA, "Life isn't free, soda too, get 3USD then come back.");
			}
		}
	}
	if(dialogid == DIALOG_REGISTER) //REGISTER
	{
		if(response)
		{
			if(!gamemodeLoaded)
			{
				format(str, sizeof(str), "{FFFFFF}Welcome to {FFFF00}San Andreas Role Play{FFFFFF}.\n\nPlease register your account by typing the password below:", str);
				ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Register", str, "Register", "Quit");
				
				SendClientMessage(playerid, COLOR_WHITE, "{F00000}SERVER{FFFFFF}: Please wait until the gamemode has finished loading.");
				return 1;
			}
			
			if(strlen(inputtext) >= 50)
			{
				format(str, sizeof(str), "{FFFFFF}Welcome to {FFFF00}San Andreas Role Play{FFFFFF}.\n\nPlease register your account by typing the password below:", str);
				ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Register", str, "Register", "Quit");

				SendClientMessage(playerid, COLOR_WHITE, "{F00000}SERVER{FFFFFF}: Password is too long.");
				return 1;
			}
			if(!strlen(inputtext))
			{
				format(str, sizeof(str), "{FFFFFF}Welcome to {FFFF00}San Andreas Role Play{FFFFFF}.\n\nPlease register your account by typing the password below:", str);
				ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Register", str, "Register", "Quit");

				SendClientMessage(playerid, COLOR_WHITE, "{F00000}SERVER{FFFFFF}: You must enter a password.");
				return 1;
			}
			if(gPlayerLogged[playerid])
			{
				SendClientMessage(playerid, COLOR_WHITE, "{F00000}SERVER{FFFFFF}: You are already logged in.");
				return 1;
			}

			//surname is restricted
			for(new i = 0; i < sizeof(SurnameInfo); i++)
			{
				if(strcmp(SurnameInfo[i][Surname], GetPlayerSurname(playerid)) == 0 && SurnameInfo[i][Taken] == 1)
				{
					new string4[128];
					new pNames[24];
					GetPlayerICName(playerid, pNames, sizeof(pNames));
					format(string4, sizeof(string4), "{FF6347}SERVER: %s is a restricted surname. Please change your surname and come back.", GetPlayerSurname(playerid));
					SendClientMessage(playerid, COLOR_WHITE, string4);
					/*format(string4, sizeof(string4), "SERVER: %s has been kicked, reason: Restricted Surname.", pNames);
					SendClientMessageToAll(COLOR_LIGHTRED, string4);*/
					KickEx(playerid);
					return 1;
				}
			}

			if(!AlphaNumericName(PlayerName(playerid)) && strlen(PlayerName(playerid)) > 4 && strlen(PlayerName(playerid)) <= MAX_PLAYER_NAME)
			{
				SendClientMessage(playerid, COLOR_WHITE, "{FF6347}SERVER: You have been kicked for having a non-rp name, pick a roleplay name (eg. John_Smith).");
				KickEx(playerid);
				return 1;
			}

			new tmppass[64];
			strmid(tmppass, inputtext, 0, strlen(inputtext), 255);
			OnPlayerRegister(playerid, tmppass);
		}
		else
		{
			SendClientMessage(playerid, COLOR_RED, "You chose to leave the server.");
			KickEx(playerid);
		}
		return 1;
	}
	// Doors
	if(dialogid == DIALOG_EDITDOOR_MAIN)
	{
		if(!response) return 1;
		else
		{
			if(listitem == 0)
			{
				new unusedid = -1;
				for(new i = 0; i < sizeof(Entrances); i++)
				{
					format(str, sizeof(str), "%0.2f,%0.2f,%0.2f", Entrances[i][outside_x], Entrances[i][outside_y], Entrances[i][outside_z]);
					if(strcmp(str, "0.00,0.00,0.00", true) == 0)
					{
						unusedid = i;
						break;
					}
				}
				if(unusedid == -1) return SendClientMessage(playerid, COLOR_GRAD1, "All of the door ID's are already in use.");
				new longstring[256];
				format(longstring, sizeof(longstring), "Please enter the door ID you would like to create.\n\n{FF0000}NOTE #1:{FFFFFF} The next unused door ID is %d.\n{FF0000}NOTE #2:{FFFFFF} This will create an entrance door at your current position and rotation.\n", unusedid);
				ShowPlayerDialog(playerid,
								 DIALOG_EDITDOOR_CREATE,
								 DIALOG_STYLE_INPUT,
								 "Edit Door - Create Door",
								 longstring,
								 "Create",
								 "Cancel");
				return 1;
			}
			else if(listitem == 1)
			{
				ShowPlayerDialog(playerid,
								 DIALOG_EDITDOOR_NAME1,
								 DIALOG_STYLE_INPUT,
								 "Edit Door - Change Name Step 1",
								 "Please enter the ID of the door you would like to change the name of.\n",
								 "Done",
								 "Cancel");
				return 1;
			}
			else if(listitem == 2)
			{
				ShowPlayerDialog(playerid,
								 DIALOG_EDITDOOR_EPOS,
								 DIALOG_STYLE_INPUT,
								 "Edit Door - Change Entrance Position Step 1",
								 "Please enter the door ID you would like to change the entrance position of.\n",
								 "Done",
								 "Cancel");
				return 1;
			}
			else  if(listitem == 3)
			{
				ShowPlayerDialog(playerid,
								 DIALOG_EDITDOOR_IPOS,
								 DIALOG_STYLE_INPUT,
								 "Edit Door - Change Interior Position Step 1",
								 "Please enter the door ID you would like to change the interior position of.\n",
								 "Done",
								 "Cancel");
				return 1;
			}
			else if(listitem == 4)
			{
				ShowPlayerDialog(playerid,
								 DIALOG_EDITDOOR_ICON1,
								 DIALOG_STYLE_INPUT,
								 "Edit Door - Change Icon Step 1",
								 "Please enter the door ID you would like to change the icon of.\n",
								 "Done",
								 "Cancel");
				return 1;
			}
			else if(listitem == 5)
			{
				ShowPlayerDialog(playerid,
								 DIALOG_EDITDOOR_PASS1,
								 DIALOG_STYLE_INPUT,
								 "Edit Door - Change Password Step 1",
								 "Please enter the door ID you would like to change the password on.\n",
								 "Done",
								 "Cancel");
				return 1;
			}
			else if(listitem == 6)
			{
				ShowPlayerDialog(playerid,
								 DIALOG_EDITDOOR_FREEZE,
								 DIALOG_STYLE_INPUT,
								 "Edit Door - Freeze On Enter/Exit",
								 "Please enter the door ID you would like to toggle the freezing on.\n",
								 "Done",
								 "Cancel");
				return 1;
			}
			else if(listitem == 7)
			{
				ShowPlayerDialog(playerid,
								 DIALOG_EDITDOOR_OWNER1,
								 DIALOG_STYLE_INPUT,
								 "Edit Door - Owner",
								 "Please enter the ID of the door you want to change the owner for.\n",
								 "Done",
								 "Cancel");
				return 1;
			}
			else if(listitem == 8)
			{
				ShowPlayerDialog(playerid,
								 DIALOG_EDITDOOR_REM1,
								 DIALOG_STYLE_INPUT,
								 "Edit Door - Remove Door Step 1",
								 "Please enter the door ID you would like to remove.\n",
								 "Done",
								 "Cancel");
				return 1;
			}
			return 1;
		}
	}
	if(dialogid == DIALOG_EDITDOOR_CREATE)
	{
		if(PlayerInfo[playerid][pAdmin] < 3 && PlayerInfo[playerid][pDev] < 1 && PlayerInfo[playerid][pMapper] < 1 && PlayerInfo[playerid][pBizMod] < 1) return SendClientMessage(playerid, COLOR_GRAD1, "You are not authorized to use this.");
		if(!response) return 1;
		else
		{
			if(!IsNumeric(inputtext)) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			new ident = strval(inputtext);
			if(ident > MAX_DOORS || ident < 0) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			format(str, sizeof(str), "%0.2f,%0.2f,%0.2f", Entrances[ident][outside_x], Entrances[ident][outside_y], Entrances[ident][outside_z]);
			if(strcmp(str, "0.00,0.00,0.00", true) != 0)
				return SendClientMessage(playerid, COLOR_GRAD1, "That door ID is already in use.");
			new Float:dpX, Float:dpY, Float:dpZ, Float:dpA;
			GetPlayerPos(playerid, dpX, dpY, dpZ);
			GetPlayerFacingAngle(playerid, dpA);
			new intid = GetPlayerInterior(playerid);
			new worldid = GetPlayerVirtualWorld(playerid);
			Entrances[ident][outside_x] = dpX;
			Entrances[ident][outside_y] = dpY;
			Entrances[ident][outside_z] = dpZ;
			Entrances[ident][outside_a] = dpA;
			Entrances[ident][outside_i] = intid;
			Entrances[ident][outside_w] = worldid;
			strdel(Entrances[ident][createdby], 0, 50);
			format(str, sizeof(str), "%s", PlayerICName(playerid));
			strins(Entrances[ident][createdby], str, 0);
			if( Entrances[ident][outside_icon] == 0 ) {
				EntranceRef[ident] = CreateDynamicPickup(19198, 23, Entrances[ident][outside_x], Entrances[ident][outside_y], Entrances[ident][outside_z]+0.4, Entrances[ident][outside_w]);
				EntranceRefInt[ident] = CreateDynamicPickup(19198, 23, Entrances[ident][inside_x], Entrances[ident][inside_y], Entrances[ident][inside_z]+0.4, Entrances[ident][inside_w]);
			} else {
				EntranceRef[ident] = CreateDynamicPickup(19198, 23, Entrances[ident][outside_x], Entrances[ident][outside_y], Entrances[ident][outside_z], Entrances[ident][outside_w]);
				EntranceRefInt[ident] = CreateDynamicPickup(19198, 23, Entrances[ident][inside_x], Entrances[ident][inside_y], Entrances[ident][inside_z], Entrances[ident][inside_w]);
			}
			new doornametext[128];
			format(doornametext, sizeof(doornametext), "{43527D}%s{FFFFFF}\n\nType /enter to go inside.", Entrances[ident][doorname]);
			EntranceRefText[ident] = CreateDynamic3DTextLabel(doornametext, COLOR_ENTRANCE, Entrances[ident][outside_x], Entrances[ident][outside_y], Entrances[ident][outside_z] +0.75, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, Entrances[ident][outside_w], Entrances[ident][outside_i]);
			format(str, sizeof(str), "You have created door id %d.", ident);
			SendClientMessage(playerid, COLOR_GRAD1, str);
			format(str, sizeof(str), "%s has created door ID %d.", PlayerName(playerid), ident);
			printf(str);
		}
		return 1;
	}
	if(dialogid == DIALOG_GAMBLE_1)
	{
			if(response)
			{
				new string[450];
				if(listitem == 0)
				{
					format(string, sizeof(string), "Current President: Jennifer Blake\nJobs\nFactions\nFamilies");
					ShowPlayerDialog(playerid, DIALOG_GAMBLE_1, DIALOG_STYLE_LIST, "Computer", string, "Select", "Close");
					return 1;
				}
				if(listitem == 1)
				{
					ShowPlayerDialog(playerid, DIALOG_JOBHELP, DIALOG_STYLE_LIST, "Job Help", "Detective\nLawyer\nDrug Dealer\nMechanic\nBodyguard\nArms Dealer\nTaxi Driver\nDrug Smuggler\nWaste Collector\nPizza Deliver\nFarmer", "Select", "Cancel");
					return 1;
				}
				if(listitem == 2)
				{
					new count[MAX_FACTIONS] = 0, facStr[1024];
					foreach(new i:Player) {
					new playerFac = PlayerInfo[i][pMember];

					if(playerFac != 0 && playerFac < sizeof(count))
					count[playerFac]++;
					}

					format(facStr, sizeof(facStr), "{FFFFFF}San Andreas Police Department ({FF0000}%i{FFFFFF})", count[FACTION_SAPD]);
					format(facStr, sizeof(facStr), "%s\n{FFFFFF}Federal Bureau of Investigtion ({FF0000}%i{FFFFFF})", facStr, count[FACTION_FBI]);
					format(facStr, sizeof(facStr), "%s\n{FFFFFF}San Andreas Fire & Medical Dept. ({FF0000}%i{FFFFFF})", facStr, count[FACTION_FMD]);
					format(facStr, sizeof(facStr), "%s\n{FFFFFF}Government ({FF0000}%i{FFFFFF})", facStr, count[FACTION_GOV]);
					format(facStr, sizeof(facStr), "%s\n{FFFFFF}San Andreas Network ({FF0000}%i{FFFFFF})", facStr, count[FACTION_NEWS]);
					format(facStr, sizeof(facStr), "%s\n{FFFFFF}Judicial Department ({FF0000}%i{FFFFFF})", facStr, count[FACTION_JD]);
					format(facStr, sizeof(facStr), "%s\n{FFFFFF}San Andreas National Guard ({FF0000}%i{FFFFFF})", facStr, count[FACTION_SANG]);

					ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Factions", facStr, "Close", "");
					for(new i = 0; i < 12; i++)count[i] = 0;
					return 1;
				}
				if(listitem == 3)
				{
					if(PlayerInfo[playerid][pMember]  < 1 && PlayerInfo[playerid][pGroup] < 10)
					{
					new famstr[2048];
					for(new i = 0; i < sizeof(FamilyInfo); i++)
					{
						format(famstr, sizeof(famstr), "%s\n%d\t%s", famstr, i +1, FamilyInfo[i][FamilyName]);
					}
					ShowPlayerDialog(playerid, FAMILY_DIALOG, DIALOG_STYLE_LIST, "Families", famstr, "Ok", "Cancel");
					format(str, sizeof(str), "* %s logs into deep web and checks list of current gangs.", PlayerICName(playerid));
					SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 15000);
					format(str, sizeof(str), "%s logs into deep web and checks list of current gangs", PlayerICName(playerid));
					SendClientMessage(playerid, COLOR_PURPLE, str);
					}
					else if(PlayerInfo[playerid][pMember] > 0 || PlayerInfo[playerid][pGroup] == 10)
					{
						SendClientMessage(playerid, COLOR_GREY, "You tried to log into deep web and check all available gangs, but failed, your face is pictured!");
					}
					return 1;
				}
			}
			return 1;
	}
	if(dialogid == DIALOG_EDITDOOR_NAME1)
	{
		if(PlayerInfo[playerid][pAdmin] < 3 && PlayerInfo[playerid][pDev] < 1 && PlayerInfo[playerid][pMapper] < 1 && PlayerInfo[playerid][pBizMod] < 1) return SendClientMessage(playerid, COLOR_GRAD1, "You are not authorized to use this.");
		if(!response) return 1;
		else
		{
			if(!IsNumeric(inputtext)) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			new ident = strval(inputtext);
			if(ident > MAX_DOORS || ident < 0) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			format(str, sizeof(str), "%0.2f,%0.2f,%0.2f", Entrances[ident][outside_x], Entrances[ident][outside_y], Entrances[ident][outside_z]);
			if(strcmp(str, "0.00,0.00,0.00", true) == 0)
				return SendClientMessage(playerid, COLOR_GRAD1, "That door ID doesn't exist.");
			SetPVarInt(playerid, "door_editing_name", ident);
			ShowPlayerDialog(playerid,
							 DIALOG_EDITDOOR_NAME2,
							 DIALOG_STYLE_INPUT,
							 "Edit Door - Change Name Step 2",
							 "Please enter the new name for the door.",
							 "Done",
							 "Cancel");
		}
		return 1;
	}
	if(dialogid == DIALOG_EDITDOOR_NAME2)
	{
		if(PlayerInfo[playerid][pAdmin] < 3 && PlayerInfo[playerid][pDev] < 1 && PlayerInfo[playerid][pMapper] < 1 && PlayerInfo[playerid][pBizMod] < 1) return SendClientMessage(playerid, COLOR_GRAD1, "You are not authorized to use this.");
		new ident = GetPVarInt(playerid, "door_editing_name");
		if(ident < 0 || ident > MAX_DOORS) return 1;
		if(!response) return 1;
		else
		{
			format(str, sizeof(str), "%0.2f,%0.2f,%0.2f", Entrances[ident][outside_x], Entrances[ident][outside_y], Entrances[ident][outside_z]);
			if(strcmp(str, "0.00,0.00,0.00", true) == 0)
				return SendClientMessage(playerid, COLOR_GRAD1, "That door ID doesn't exist.");
			if(strlen(inputtext) > 50) return SendClientMessage(playerid, COLOR_GRAD1, "Name can not be longer than 50 characters!");
			strdel(Entrances[ident][doorname], 0, 50);
			strins(Entrances[ident][doorname], inputtext, 0);
			format(str, sizeof(str), "{43527D}%s{FFFFFF}\n\nType /enter to go inside.", Entrances[ident][doorname]);
			UpdateDynamic3DTextLabelText(EntranceRefText[ident], COLOR_ENTRANCE, str);
			format(str, sizeof(str), "You have edited door id %d's name to \"%s\".", ident, Entrances[ident][doorname]);
			SendClientMessage(playerid, COLOR_GRAD1, str);
			SetPVarInt(playerid, "door_editing_name", -1);
			format(str, sizeof(str), "%s has changed the name of door ID %d.", PlayerName(playerid), ident);
			printf(str);
		}
		return 1;
	}
	if(dialogid == DIALOG_EDITDOOR_EPOS)
	{
		if(PlayerInfo[playerid][pAdmin] < 3 && PlayerInfo[playerid][pDev] < 1 && PlayerInfo[playerid][pMapper] < 1 && PlayerInfo[playerid][pBizMod] < 1) return SendClientMessage(playerid, COLOR_GRAD1, "You are not authorized to use this.");
		if(!response) return 1;
		else
		{
			if(!IsNumeric(inputtext)) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			new ident = strval(inputtext);
			if(ident > MAX_DOORS || ident < 0) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			format(str, sizeof(str), "%0.2f,%0.2f,%0.2f", Entrances[ident][outside_x], Entrances[ident][outside_y], Entrances[ident][outside_z]);
			if(strcmp(str, "0.00,0.00,0.00", true) == 0)
				return SendClientMessage(playerid, COLOR_GRAD1, "That door ID doesn't exist.");
			SetPVarInt(playerid, "door_editing_id", ident);
			SetPVarInt(playerid, "door_editing_type", 1);
			SendClientMessage(playerid, COLOR_YELLOW, "You can now do /ddone in the position you want to move this door.");
		}
		return 1;
	}
	if(dialogid == DIALOG_EDITDOOR_IPOS)
	{
		if(PlayerInfo[playerid][pAdmin] < 3 && PlayerInfo[playerid][pDev] < 1 && PlayerInfo[playerid][pMapper] < 1 && PlayerInfo[playerid][pBizMod] < 1) return SendClientMessage(playerid, COLOR_GRAD1, "You are not authorized to use this.");
		if(!response) return 1;
		else
		{
			if(!IsNumeric(inputtext)) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			new ident = strval(inputtext);
			if(ident > MAX_DOORS || ident < 0) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			format(str, sizeof(str), "%0.2f,%0.2f,%0.2f", Entrances[ident][outside_x], Entrances[ident][outside_y], Entrances[ident][outside_z]);
			if(strcmp(str, "0.00,0.00,0.00", true) == 0)
				return SendClientMessage(playerid, COLOR_GRAD1, "That door ID doesn't exist.");
			SetPVarInt(playerid, "door_editing_id", ident);
			SetPVarInt(playerid, "door_editing_type", 2);
			SendClientMessage(playerid, COLOR_YELLOW, "You can now do /ddone in the position you want to move this door's interior.");
		}
		return 1;
	}
	if(dialogid == DIALOG_EDITDOOR_ICON1)
	{
		if(PlayerInfo[playerid][pAdmin] < 3 && PlayerInfo[playerid][pDev] < 1 && PlayerInfo[playerid][pMapper] < 1 && PlayerInfo[playerid][pBizMod] < 1) return 1;
		if(!response) return SetPVarInt(playerid, "door_editing_icon", -1);
		else
		{
			if(!IsNumeric(inputtext)) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			new ident = strval(inputtext);
			if(ident > MAX_DOORS || ident < 0) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			format(str, sizeof(str), "%0.2f,%0.2f,%0.2f", Entrances[ident][outside_x], Entrances[ident][outside_y], Entrances[ident][outside_z]);
			if(strcmp(str, "0.00,0.00,0.00", true) == 0)
				return SendClientMessage(playerid, COLOR_GRAD1, "That door ID doesn't exist.");
			SetPVarInt(playerid, "door_editing_icon", ident);
			ShowPlayerDialog(playerid,
							 DIALOG_EDITDOOR_ICON2,
							 DIALOG_STYLE_LIST,
							 "Edit Door - Change Icon Step 2",
							 "Entrance (\"i\" icon)\nInformation\nArrow #2\nArrow #3\nArrow #4\nArrow #5\nEnter/Exit Marker",
							 "Done",
							 "Cancel");
		}
		return 1;
	}
	if(dialogid == DIALOG_EDITDOOR_ICON2)
	{
		if(PlayerInfo[playerid][pAdmin] < 3 && PlayerInfo[playerid][pDev] < 1 && PlayerInfo[playerid][pMapper] < 1 && PlayerInfo[playerid][pBizMod] < 1) return 1;
		if(!response) return SetPVarInt(playerid, "door_editing_icon", -1);
		else
		{
			new did = GetPVarInt(playerid, "door_editing_icon");
			if(did < 1) return SetPVarInt(playerid, "door_editing_icon", -1);
			if(did > MAX_DOORS || did < 0) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			format(str, sizeof(str), "%0.2f,%0.2f,%0.2f", Entrances[did][outside_x], Entrances[did][outside_y], Entrances[did][outside_z]);
			if(strcmp(str, "0.00,0.00,0.00", true) == 0)
				return SetPVarInt(playerid, "door_editing_icon", -1);
			Entrances[did][outside_icon] = listitem;
			new actualpickupid = 0;
			if(Entrances[did][outside_icon] == 0) actualpickupid = 19198;
			else if(Entrances[did][outside_icon] == 1) actualpickupid = 1239;
			else if(Entrances[did][outside_icon] == 2) actualpickupid = 19131;
			else if(Entrances[did][outside_icon] == 3) actualpickupid = 19132;
			else if(Entrances[did][outside_icon] == 4) actualpickupid = 19133;
			else if(Entrances[did][outside_icon] == 5) actualpickupid = 19134;
			else if(Entrances[did][outside_icon] == 6) actualpickupid = 19135;
			if(EntranceRef[did]) DestroyDynamicPickup(EntranceRef[did]);
			if(EntranceRefInt[did]) DestroyDynamicPickup(EntranceRefInt[did]);
			if(EntranceRefText[did]) DestroyDynamic3DTextLabel(EntranceRefText[did]);
			if( Entrances[did][outside_icon] == 0 ) {
				EntranceRef[did] = CreateDynamicPickup(actualpickupid, 23, Entrances[did][outside_x], Entrances[did][outside_y], Entrances[did][outside_z]+0.4, Entrances[did][outside_w]);
				EntranceRefInt[did] = CreateDynamicPickup(actualpickupid, 23, Entrances[did][inside_x], Entrances[did][inside_y], Entrances[did][inside_z]+0.4, Entrances[did][inside_w]);
			} else {
				EntranceRef[did] = CreateDynamicPickup(actualpickupid, 23, Entrances[did][outside_x], Entrances[did][outside_y], Entrances[did][outside_z], Entrances[did][outside_w]);
				EntranceRefInt[did] = CreateDynamicPickup(actualpickupid, 23, Entrances[did][inside_x], Entrances[did][inside_y], Entrances[did][inside_z], Entrances[did][inside_w]);
			}
			new doornametext[128];
			format(doornametext, sizeof(doornametext), "{43527D}%s{FFFFFF}\n\nType /enter to go inside.", Entrances[did][doorname]);
			EntranceRefText[did] = CreateDynamic3DTextLabel(doornametext, COLOR_ENTRANCE, Entrances[did][outside_x], Entrances[did][outside_y], Entrances[did][outside_z] +0.75, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, Entrances[did][outside_w], Entrances[did][outside_i]);
			SaveEntrances();
			SendClientMessage(playerid, COLOR_YELLOW, "You have updated that door's icon.");
			format(str, sizeof(str), "%s has edited the icon of door ID %d.", PlayerName(playerid), did);
			printf(str);
		}
		return 1;
	}
	if(dialogid == DIALOG_EDITDOOR_PASS1)
	{
		if(PlayerInfo[playerid][pAdmin] < 3 && PlayerInfo[playerid][pDev] < 1 && PlayerInfo[playerid][pMapper] < 1 && PlayerInfo[playerid][pBizMod] < 1) return 1;
		if(!response) return SetPVarInt(playerid, "door_editing_pass", -1);
		else
		{
			if(!IsNumeric(inputtext)) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			new ident = strval(inputtext);
			if(ident > MAX_DOORS || ident < 0) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			format(str, sizeof(str), "%0.2f,%0.2f,%0.2f", Entrances[ident][outside_x], Entrances[ident][outside_y], Entrances[ident][outside_z]);
			if(strcmp(str, "0.00,0.00,0.00", true) == 0)
				return SendClientMessage(playerid, COLOR_GRAD1, "That door ID doesn't exist.");
			SetPVarInt(playerid, "door_editing_pass", ident);
			ShowPlayerDialog(playerid,
							 DIALOG_EDITDOOR_PASS2,
							 DIALOG_STYLE_INPUT,
							 "Edit Door - Change Password Step 2",
							 "Please enter the new password for the door, just enter \"none\" to remove the password.",
							 "Change",
							 "Cancel");
		}
		return 1;
	}
	if(dialogid == DIALOG_EDITDOOR_PASS2)
	{
		if(PlayerInfo[playerid][pAdmin] < 3 && PlayerInfo[playerid][pDev] < 1 && PlayerInfo[playerid][pMapper] < 1 && PlayerInfo[playerid][pBizMod] < 1) return 1;
		if(!response) return SetPVarInt(playerid, "door_editing_pass", -1);
		else
		{
			new did = GetPVarInt(playerid, "door_editing_pass");
			if(did > MAX_DOORS || did < 0) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			format(str, sizeof(str), "%0.2f,%0.2f,%0.2f", Entrances[did][outside_x], Entrances[did][outside_y], Entrances[did][outside_z]);
			if(strcmp(str, "0.00,0.00,0.00", true) == 0)
				return SendClientMessage(playerid, COLOR_GRAD1, "That door ID doesn't exist.");
			if(strlen(inputtext) > 50) return SendClientMessage(playerid, COLOR_GRAD1, "That password is too long!  Must be under 50 characters!");
			strdel(Entrances[did][dpassword], 0, 128);
			if(strcmp(inputtext, "none", true) != 0)
				strins(Entrances[did][dpassword], inputtext, 0);
			if(strcmp(inputtext, "none", true) != 0)
				format(str, sizeof(str), "You have changed door %d's password to %s.", did, inputtext);
			else
				format(str, sizeof(str), "You have removed door %d's password.", did);
			SendClientMessage(playerid, COLOR_GRAD1, str);
			SetPVarInt(playerid, "door_editing_pass", -1);
			format(str, sizeof(str), "%s has updated the password of door ID %d.", PlayerName(playerid), did);
			printf(str);
		}
		return 1;
	}
	if(dialogid == DIALOG_EDITDOOR_FREEZE)
	{
		if(PlayerInfo[playerid][pAdmin] < 3 && PlayerInfo[playerid][pDev] < 1 && PlayerInfo[playerid][pMapper] < 1 && PlayerInfo[playerid][pBizMod] < 1) return 1;
		if(!response) return 1;
		else
		{
			if(!IsNumeric(inputtext)) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			new ident = strval(inputtext);
			if(ident > MAX_DOORS || ident < 0) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			format(str, sizeof(str), "%0.2f,%0.2f,%0.2f", Entrances[ident][outside_x], Entrances[ident][outside_y], Entrances[ident][outside_z]);
			if(strcmp(str, "0.00,0.00,0.00", true) == 0)
				return SendClientMessage(playerid, COLOR_GRAD1, "That door ID doesn't exist.");
			Entrances[ident][freeze] = !Entrances[ident][freeze]; // Toggle, works in C++ not sure about PAWN.
			if(Entrances[ident][freeze])
				format(str, sizeof(str), "Players are now frozen when they enter or exit this door.");
			else
				format(str, sizeof(str), "Players are now NOT frozen when they enter or exit this door.");
			SendClientMessage(playerid, COLOR_YELLOW, str);
			format(str, sizeof(str), "%s has updated the freeze option of door ID %d.", PlayerName(playerid), ident);
			printf(str);
		}
		return 1;
	}
	if( dialogid == DIALOG_EDITDOOR_OWNER1 ) {
		if(PlayerInfo[playerid][pAdmin] < 3 && PlayerInfo[playerid][pDev] < 1 && PlayerInfo[playerid][pMapper] < 1 && PlayerInfo[playerid][pBizMod] < 1) return 1;
		if(!response) return SetPVarInt(playerid, "door_editing_owner", -1);
		else
		{
			if(!IsNumeric(inputtext)) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			new ident = strval(inputtext);
			if(ident > MAX_DOORS || ident < 0) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			format(str, sizeof(str), "%0.2f,%0.2f,%0.2f", Entrances[ident][outside_x], Entrances[ident][outside_y], Entrances[ident][outside_z]);
			if(strcmp(str, "0.00,0.00,0.00", true) == 0)
				return SendClientMessage(playerid, COLOR_GRAD1, "That door ID doesn't exist.");
			SetPVarInt(playerid, "door_editing_owner", ident);
			ShowPlayerDialog(playerid,
							 DIALOG_EDITDOOR_OWNER2,
							 DIALOG_STYLE_INPUT,
							 "Edit Door - Change Owner Step 2",
							 "Please enter the owner's SQL ID (/nametosql) \"0\" to remove the owner.",
							 "Change",
							 "Cancel");
		}
		return 1;
	}
		
	if( dialogid == DIALOG_EDITDOOR_OWNER2 ) {
		if(PlayerInfo[playerid][pAdmin] < 3 && PlayerInfo[playerid][pDev] < 1 && PlayerInfo[playerid][pMapper] < 1 && PlayerInfo[playerid][pBizMod] < 1) return 1;
		if(!response) return SetPVarInt(playerid, "door_editing_owner", -1);
		else
		{
			new did = GetPVarInt(playerid, "door_editing_owner");
			if(did > MAX_DOORS || did < 0) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			format(str, sizeof(str), "%0.2f,%0.2f,%0.2f", Entrances[did][outside_x], Entrances[did][outside_y], Entrances[did][outside_z]);
			if(strcmp(str, "0.00,0.00,0.00", true) == 0)
				return SendClientMessage(playerid, COLOR_GRAD1, "That door ID doesn't exist.");
				
			new ownerID = strval( inputtext );
			if( !IsNumeric( inputtext ) || ownerID < 0 ) {
				return SendClientMessage( playerid, COLOR_GREY, "You must enter some positive integer, as owner SQL IDs are represented as numbers." );
				
			}
			
			Entrances[did][eowner] = ownerID;
			SetPVarInt(playerid, "door_editing_owner", -1);
			
			if(Entrances[did][eowner])
				format(str, sizeof(str), "This door now has an owner.");
			else
				format(str, sizeof(str), "This door no longer has a set owner.");
			SendClientMessage(playerid, COLOR_YELLOW, str);
			format(str, sizeof(str), "%s has updated the owner option of door ID %d to owner SQL: %d", PlayerName(playerid), ownerID);
			printf(str);
			
		}
		return 1;
	}
	if(dialogid == DIALOG_EDITDOOR_REM1)
	{
		if(PlayerInfo[playerid][pAdmin] < 3 && PlayerInfo[playerid][pDev] < 1 && PlayerInfo[playerid][pMapper] < 1 && PlayerInfo[playerid][pBizMod] < 1) return 1;
		if(!response) return SetPVarInt(playerid, "door_editing_remove", -1);
		else
		{
			if(!IsNumeric(inputtext)) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			new ident = strval(inputtext);
			if(ident > MAX_DOORS || ident < 0) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			format(str, sizeof(str), "%0.2f,%0.2f,%0.2f", Entrances[ident][outside_x], Entrances[ident][outside_y], Entrances[ident][outside_z]);
			if(strcmp(str, "0.00,0.00,0.00", true) == 0)
				return SendClientMessage(playerid, COLOR_GRAD1, "That door ID doesn't exist.");
			SetPVarInt(playerid, "door_editing_remove", ident);
			ShowPlayerDialog(playerid,
							 DIALOG_EDITDOOR_REM2,
							 DIALOG_STYLE_MSGBOX,
							 "Edit Door - Remove Door Step 2",
							 "Are you sure you want to delete this door?",
							 "Yes",
							 "No");
		}
		return 1;
	}
	if(dialogid == DIALOG_EDITDOOR_REM2)
	{
		if(PlayerInfo[playerid][pAdmin] < 3 && PlayerInfo[playerid][pDev] < 1 && PlayerInfo[playerid][pMapper] < 1 && PlayerInfo[playerid][pBizMod] < 1) return 1;
		if(!response) return SetPVarInt(playerid, "door_editing_remove", -1);
		else
		{
			new did = GetPVarInt(playerid, "door_editing_remove");
			if(did > MAX_DOORS || did < 0) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid door ID.");
			format(str, sizeof(str), "%0.2f,%0.2f,%0.2f", Entrances[did][outside_x], Entrances[did][outside_y], Entrances[did][outside_z]);
			if(strcmp(str, "0.00,0.00,0.00", true) == 0)
				return SendClientMessage(playerid, COLOR_GRAD1, "That door ID doesn't exist.");
			Entrances[did][outside_icon] = 0;
			Entrances[did][outside_x] = 0.00;
			Entrances[did][outside_y] = 0.00;
			Entrances[did][outside_z] = 0.00;
			Entrances[did][outside_a] = 0.00;
			Entrances[did][outside_i] = 0;
			Entrances[did][outside_w] = 0;
			Entrances[did][inside_x] = 0.00;
			Entrances[did][inside_y] = 0.00;
			Entrances[did][inside_z] = 0.00;
			Entrances[did][inside_a] = 0.00;
			Entrances[did][inside_i] = 0;
			Entrances[did][inside_w] = 0;
			strdel(Entrances[did][doorname], 0, 50);
			strdel(Entrances[did][createdby], 0, 50);
			if(EntranceRef[did]) DestroyDynamicPickup(EntranceRef[did]);
			if(EntranceRefInt[did]) DestroyDynamicPickup(EntranceRefInt[did]);
			if(EntranceRefText[did]) DestroyDynamic3DTextLabel(EntranceRefText[did]);
			EntranceRef[did] = 0;
			EntranceRefInt[did] = 0;
			EntranceRefText[did] = Text3D:INVALID_3DTEXT_ID;
			format(str, sizeof(str), "You have removed door id %d.", did);
			SendClientMessage(playerid, COLOR_GRAD1, str);
			SetPVarInt(playerid, "door_editing_remove", -1);
			format(str, sizeof(str), "%s has removed door ID %d.", PlayerName(playerid), did);
			printf(str);
		}
		return 1;
	}
	if(dialogid == DIALOG_ID_PLAYER_PASS)
	{
		if(!response) return 1;
		else
		{
			for(new i = 0; i < sizeof(EntranceRef); i++)
			{
				if(
					IsPlayerInRangeOfPoint(playerid, 2, Entrances[i][outside_x], Entrances[i][outside_y], Entrances[i][outside_z]) &&
					GetPlayerVirtualWorld(playerid) == Entrances[i][outside_w] &&
					GetPlayerInterior(playerid) == Entrances[i][outside_i])
				{
					if(!strlen(Entrances[i][dpassword])) // the door they're at has no password. wtf
						return 1;
					if(strcmp(Entrances[i][dpassword], inputtext) != 0 || strlen(inputtext) == 0)
						return SendClientMessage(playerid, COLOR_GRAD1, "Incorrect password... Access Denied.");
					SendClientMessage(playerid, COLOR_GREEN, "Access granted.");
					SetPlayerPos(playerid, Entrances[i][inside_x], Entrances[i][inside_y], Entrances[i][inside_z]);
					SetPlayerFacingAngle(playerid, Entrances[i][inside_a]);
					SetPlayerInterior(playerid, Entrances[i][inside_i]);
					SetPlayerVirtualWorld(playerid, Entrances[i][inside_w]);
					SetCameraBehindPlayer(playerid);
					if(Entrances[i][freeze])
					{
						Timer_FreezePlayer(playerid, FREEZE, ENTER_FREEZE);
					}
					return 1;
				}
			}
		}
		return 1;
	}
	if(dialogid == DIALOG_INTERIOR_MENU)
	{
		if(response)
		{
			new arrayid = listitem, tpstr[128];
			SetPlayerPos(playerid, InteriorDialogData[arrayid][0], InteriorDialogData[arrayid][1], InteriorDialogData[arrayid][2]);
			SetPlayerInterior(playerid, InteriorDialogInteriors[arrayid][0]);
			SetPlayerFacingAngle(playerid, InteriorDialogData[arrayid][3]);
			format(tpstr, sizeof(tpstr), "* You have been teleported to interior %s.", InteriorDialogNames[arrayid][0]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, tpstr);
		}
		return 1;
	}
	//dealerships
	if(dialogid == 24325)
	{
		if(!response)
		{
			for(new i = 0; i < MAX_DEALERS; i++)
			{
				format(str, sizeof(str), "dealerlist_%d", i);
				DeletePVar(playerid, str);
			}
			return 1;
		}
		else
		{
			format(str, sizeof(str), "dealerlist_%d", listitem);
			new i = GetPVarInt(playerid, str);
			SetPlayerCheckpointEx(playerid, Dealerships[i][dPos][0], Dealerships[i][dPos][1], Dealerships[i][dPos][2], 5.0);
			CP[playerid] = CHECKPOINT_SHOWDEALERSHIP;
			format(str, sizeof(str), "%s has been marked on your map (red marker).", Dealerships[i][dName]);
			SendClientMessage(playerid, COLOR_YELLOW, str);
		}
		return 1;
	}
	if(dialogid == 12513)
	{
		if(response)
		{
			if(PhoneOffline[playerid]) return SendClientMessage(playerid, COLOR_GREY, "Your cellphone is currently turned off, use /togphone.");
			if(PlayerInfo[playerid][pCash] < 25) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to make a call. ($25)");
			if(CheckForJammer(playerid)) return SendClientMessage(playerid, COLOR_GREY, "Phone signal has been jammed.");

			if(	PlayerInfo[playerid][pAdminJailed] || PlayerInfo[playerid][pJailed]) return SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't use this whilst you're in prison.");
			
			if(PlayerInfo[playerid][pHospital] || IsPlayerRestricted(playerid) || pDeathState[playerid] != PLAYER_STATE_NORMAL) return SendClientMessage(playerid, COLOR_GRAD2, "You are unable to use a phone at this time.");
			if(strlen(inputtext))
			{
				if(IsNumeric(inputtext))
				{
					do_call(playerid, strval(inputtext));
				}
			}
		}
	}
	if(dialogid == 12460)
	{
		if(!response)
		{
			pModelState[playerid] = 999;
			DeletePVar(playerid, "buyingdealer");
			return 1;
		}
		else
		{
			new d = GetPVarInt(playerid, "buyingdealer");
			if(Dealerships[d][dCreated] == false)
			{
				pModelState[playerid] = 999;
				DeletePVar(playerid, "buyingdealer");
				return 1;
			}
			if(Dealerships[d][dCars][listitem] >= 400)
			{
				PlayerTextDrawDestroy(playerid, pModel_t[playerid]);
				pModelState[playerid] = 999;
				pModel_t[playerid] = CreatePlayerTextDraw(playerid, 228.0, 200.0, "_");
				PlayerTextDrawFont(playerid, pModel_t[playerid], TEXT_DRAW_FONT_MODEL_PREVIEW);
				PlayerTextDrawTextSize(playerid, pModel_t[playerid], 200.0, 200.0);
				PlayerTextDrawSetPreviewModel(playerid, pModel_t[playerid], Dealerships[d][dCars][listitem]);
				PlayerTextDrawSetPreviewRot(playerid, pModel_t[playerid], -16.0, 0.0, -55.0);
				PlayerTextDrawUseBox(playerid, pModel_t[playerid], 0);
				PlayerTextDrawBackgroundColor(playerid, pModel_t[playerid], 0x00000000);
				PlayerTextDrawShow(playerid, pModel_t[playerid]);
				pModelState[playerid] = 3;
				pModelRot[playerid][0] = 0;
				pModelRot[playerid][1] = 0;
				pModelRot[playerid][2] = 0;
				pModelTimer[playerid] = SetTimerEx("UpdatePreviewModel", 1000, 1, "i", playerid);
				format(str, sizeof(str), "Are you sure you would like to purchase this %s for $%d?\n\n{F5DA81}NOTE: Below is a preview of the vehicle.", GetVehicleFriendlyNameFromModel(Dealerships[d][dCars][listitem]), Dealerships[d][dCarPrice][listitem]);
				ShowPlayerDialog(playerid, 12461, DIALOG_STYLE_MSGBOX, "Confirm Purchase", str, "Purchase", "Cancel");
				SetPVarInt(playerid, "buyingcarid", listitem);
			}
		}
		return 1;
	}
	if(dialogid == 12461)
	{
		PlayerTextDrawDestroy(playerid, pModel_t[playerid]);
		pModelState[playerid] = 999;
		if(!response)
		{
			DeletePVar(playerid, "buyingdealer");
			DeletePVar(playerid, "buyingcarid");
			return 1;
		}
		else
		{
			new d = GetPVarInt(playerid, "buyingdealer");
			if(Dealerships[d][dCreated] == false)
			{
				DeletePVar(playerid, "buyingdealer");
				DeletePVar(playerid, "buyingcarid");
				return 1;
			}
			new cid = GetPVarInt(playerid, "buyingcarid");
			new model = Dealerships[d][dCars][cid];
			if(model < 400 || model > 600)
			{
				DeletePVar(playerid, "buyingdealer");
				DeletePVar(playerid, "buyingcarid");
				return 1;
			}
			new price = Dealerships[d][dCarPrice][cid];
			if(price < 1)
			{
				DeletePVar(playerid, "buyingdealer");
				DeletePVar(playerid, "buyingcarid");
				return 1;
			}
			if(GetMoney(playerid) < price)
			{
				DeletePVar(playerid, "buyingdealer");
				DeletePVar(playerid, "buyingcarid");
				SendClientMessage(playerid, COLOR_GRAD1, "You can not afford that.");
				return 1;
			}
			new slot;
			slot = GetPlayerFreeCarslot( playerid );
			if( slot < 0 ) {
				return SendClientMessage( playerid, COLOR_GREY, "You do not have any free slots to buy another car. Sell a car first, or dump one." );
			}
			
			GiveMoney(playerid, -price);
			
			
			createPlayerVehicle(playerid, slot, model, Dealerships[d][dPos][3], Dealerships[d][dPos][4], Dealerships[d][dPos][5], Dealerships[d][dPos][6]);
			if( playerSpawnedVehicle[playerid] > -1 ) {
				destroyPlayerVehicle( playerid, playerSpawnedVehicle[playerid] );
			}
			
			spawnPlayerVehicle( playerid, slot );
			format(str, sizeof(str), "You have bought a %s from %s for $%d.", GetVehicleFriendlyNameFromModel(Dealerships[d][dCars][cid]), Dealerships[d][dName], Dealerships[d][dCarPrice][cid]);
			SendClientMessage(playerid, COLOR_YELLOW, str);
		}
		return 1;
	}
	if(dialogid == 12400) // ID is dedit
	{
		if(!response)
		{
			DeletePVar(playerid, "edittingdealer");
			return 1;
		}
		if(listitem == 0)
		{
			new d = GetPVarInt(playerid, "edittingdealer");
			if(Dealerships[d][dCreated] == false)
			{
				DeletePVar(playerid, "edittingdealer");
				return 1;
			}
			ShowPlayerDialog(playerid, 12410, DIALOG_STYLE_INPUT, "Change Dealership Name", "Enter a new name for the dealership.", "Done", "Cancel");
		}
		else if(listitem == 1)
		{
			new d = GetPVarInt(playerid, "edittingdealer");
			if(Dealerships[d][dCreated] == false)
			{
				DeletePVar(playerid, "edittingdealer");
				return 1;
			}
			new carsstring[1024];
			format(carsstring, sizeof(carsstring), "Add Car\n");
			for(new i = 0; i < 64; i++)
			{
				if(Dealerships[d][dCars][i] >= 400)
				{
					format(carsstring, sizeof(carsstring), "%s%d. %s ($%d)\n", carsstring, i, GetVehicleFriendlyNameFromModel(Dealerships[d][dCars][i]), Dealerships[d][dCarPrice][i]);
				}
			}
			ShowPlayerDialog(playerid, 12420, DIALOG_STYLE_LIST, "Manage Dealership Cars", carsstring, "Select", "Cancel");
		}
		else if(listitem == 2)
		{
			new d = GetPVarInt(playerid, "edittingdealer");
			if(Dealerships[d][dCreated] == false)
			{
				DeletePVar(playerid, "edittingdealer");
				return 1;
			}
			SetPVarInt(playerid, "movedealerid", d);
			SetPVarInt(playerid, "movingdealerid", 1);
			SendClientMessage(playerid, COLOR_YELLOW, "Use /ddone in the new position to update the position.");
		}
		else if(listitem == 3)
		{
			new d = GetPVarInt(playerid, "edittingdealer");
			if(Dealerships[d][dCreated] == false)
			{
				DeletePVar(playerid, "edittingdealer");
				return 1;
			}
			SetPVarInt(playerid, "movedealerid", d);
			SetPVarInt(playerid, "movingdealerid", 2);
			SendClientMessage(playerid, COLOR_YELLOW, "Use /ddone in the new position to update the vehicle spawning position.");
		}
		else if(listitem == 4)
		{
			new d = GetPVarInt(playerid, "edittingdealer");
			if(Dealerships[d][dCreated] == false)
			{
				DeletePVar(playerid, "edittingdealer");
				return 1;
			}
			ShowPlayerDialog(playerid, 12430, DIALOG_STYLE_MSGBOX, "Remove Dealership", "Are you sure you want to remove this dealership? These changes can not be reversed.", "Yes", "No");
		}
		return 1;
	}
	if(dialogid == 12410) // id is changing dealership name
	{
		if(!response)
		{
			DeletePVar(playerid, "edittingdealer");
			return 1;
		}
		if(strlen(inputtext))
		{
			new d = GetPVarInt(playerid, "edittingdealer");
			if(Dealerships[d][dCreated] == false)
			{
				DeletePVar(playerid, "edittingdealer");
				return 1;
			}
			strmid(Dealerships[d][dName], inputtext, 0, 64, 64);
			format(str, sizeof(str), "Dealership\nName: %s\nID: %d\n/buycar to buy a car.", Dealerships[d][dName], d);
			UpdateDynamic3DTextLabelText(Dealerships[d][d3DText], 0xFE9A2EFF, str);
			SaveDealerships();
			format(str, sizeof(str), "You have changed Dealership ID %d's name to \"%s\".", d, Dealerships[d][dName]);
			SendClientMessage(playerid, COLOR_YELLOW, str);
		}
		return 1;
	}
	if(dialogid == 12420) // id is managing dealership cars
	{
		if(!response)
		{
			DeletePVar(playerid, "edittingdealer");
			return 1;
		}
		if(listitem == 0)
		{
			new d = GetPVarInt(playerid, "edittingdealer");
			if(Dealerships[d][dCreated] == false)
			{
				DeletePVar(playerid, "edittingdealer");
				return 1;
			}
			ShowPlayerDialog(playerid, 12421, DIALOG_STYLE_INPUT, "Enter Vehicle Name/ID", "Enter the vehicle model or ID that you want to add to this dealership.", "Okay", "Cancel");
		}
		else
		{
			new d = GetPVarInt(playerid, "edittingdealer");
			if(Dealerships[d][dCreated] == false)
			{
				DeletePVar(playerid, "edittingdealer");
				return 1;
			}
			new car = listitem - 1;
			if(Dealerships[d][dCars][car] >= 400)
			{
				Dealerships[d][dCars][car] = 0;
				Dealerships[d][dCarPrice][car] = 0;
				new ca = 0; // car id fix
				for(new i = 0; i < 64; i++)
				{
					if(Dealerships[d][dCars][i] >= 400)
					{
						new oldid, oldprice;
						oldid = Dealerships[d][dCars][i];
						oldprice = Dealerships[d][dCarPrice][i];
						Dealerships[d][dCars][i] = 0;
						Dealerships[d][dCarPrice][i] = 0;
						// here's the fix
						Dealerships[d][dCars][ca] = oldid;
						Dealerships[d][dCarPrice][ca] = oldprice;
						ca++;
					}
				}
				SaveDealerships();
				SendClientMessage(playerid, COLOR_YELLOW, "You have removed that vehicle from the dealership.");
			}
		}
		return 1;
	}
	if(dialogid == 12421) // id is adding car name dialog
	{
		if(!response)
		{
			DeletePVar(playerid, "edittingdealer");
			return 1;
		}
		else
		{
			new d = GetPVarInt(playerid, "edittingdealer");
			if(Dealerships[d][dCreated] == false)
			{
				DeletePVar(playerid, "edittingdealer");
				return 1;
			}
			new modelid = ReturnVehicleModelID(inputtext);
			if(modelid < 400 || modelid > 600) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid vehicle model.");
			SetPVarInt(playerid, "managedcarmodel", modelid);
			format(str, sizeof(str), "Enter the price you want for a %s.", GetVehicleFriendlyNameFromModel(GetPVarInt(playerid, "managedcarmodel")));
			ShowPlayerDialog(playerid, 12422, DIALOG_STYLE_INPUT, "Enter Price", str, "Okay", "Cancel");
		}
		return 1;
	}
	if(dialogid == 12422) // id is adding car price dialog
	{
		if(!response)
		{
			DeletePVar(playerid, "edittingdealer");
			return 1;
		}
		else
		{
			new d = GetPVarInt(playerid, "edittingdealer");
			if(Dealerships[d][dCreated] == false)
			{
				DeletePVar(playerid, "edittingdealer");
				return 1;
			}
			new price = strval(inputtext);
			if(price < 1) return SendClientMessage(playerid, COLOR_GRAD1, "The price can not be below 1.");
			new openindex = -1;
			for(new c = 0; c < 64; c++)
			{
				if(Dealerships[d][dCars][c] < 400)
				{
					openindex = c;
					break;
				}
			}
			if(openindex == -1) return SendClientMessage(playerid, COLOR_GRAD1, "Failed to add car (Error 104)");
			Dealerships[d][dCars][openindex] = GetPVarInt(playerid, "managedcarmodel");
			Dealerships[d][dCarPrice][openindex] = price;
			SaveDealerships();
			format(str, sizeof(str), "Added %s to dealership %s for price $%d.", GetVehicleFriendlyNameFromModel(GetPVarInt(playerid, "managedcarmodel")), Dealerships[d][dName], Dealerships[d][dCarPrice][openindex]);
			SendClientMessage(playerid, COLOR_YELLOW, str);
		}
		return 1;
	}
	if(dialogid == 12430) // id is removing dealership
	{
		if(!response)
		{
			DeletePVar(playerid, "edittingdealer");
			return 1;
		}
		else
		{
			new d = GetPVarInt(playerid, "edittingdealer");
			if(Dealerships[d][dCreated] == false)
			{
				DeletePVar(playerid, "edittingdealer");
				return 1;
			}
			strdel(Dealerships[d][dName], 0, 64);
			Dealerships[d][dPos][0] = 0.0;
			Dealerships[d][dPos][1] = 0.0;
			Dealerships[d][dPos][2] = 0.0;
			Dealerships[d][dPos][3] = 0.0;
			Dealerships[d][dPos][4] = 0.0;
			Dealerships[d][dPos][5] = 0.0;
			Dealerships[d][dPos][6] = 0.0;
			for(new c = 0; c < 64; c++)
			{
				Dealerships[d][dCars][c] = 0;
				Dealerships[d][dCarPrice][c] = 0;
			}
			DestroyDynamic3DTextLabel(Dealerships[d][d3DText]);
			DestroyDynamicPickup(Dealerships[d][dPickup]);
			Dealerships[d][d3DText] = Text3D:INVALID_3DTEXT_ID;
			Dealerships[d][dPickup] = 0;
			Dealerships[d][dCreated] = false;
			SaveDealerships();
			format(str, sizeof(str), "dealerships/%d.ini", d);
			if(fexist(str))
			{
				fremove(str);
			}
			SendClientMessage(playerid, COLOR_YELLOW, "You have successfully deleted the dealership.");
		}
		return 1;
	}
	if(dialogid == 24200) // ID is Giving Accessory From List
	{
		if(response)
		{
			new player = GetPVarInt(playerid, "ToRemoveAccessor");
			Accessories[player][listitem] = 0;
			AccessoriesX[player][listitem] = 0;
			AccessoriesY[player][listitem] = 0;
			AccessoriesZ[player][listitem] = 0;
			AccessoriesrX[player][listitem] = 0;
			AccessoriesrY[player][listitem] = 0;
			AccessoriesrZ[player][listitem] = 0;
			AccessoriessX[player][listitem] = 0;
			AccessoriessY[player][listitem] = 0;
			AccessoriessZ[player][listitem] = 0;
			Accessoriesc1[player][listitem] = 0;
			RemovePlayerAttachedObject(playerid, listitem);
			SendClientMessage(player, COLOR_LIGHTBLUE, "* An admin has removed one of your gear.");
		}
		return 1;
	}
	if(dialogid == 24207) // ID is Giving Accessory From List
	{
		if(response)
		{
			new freeslot = -1, player = GetPVarInt(playerid, "ToGiveAccessorTo");
			for(new i = 0; i < 7; i++)
			{
				if(Accessories[player][i] == 0)
				{
					freeslot = i;
					break;
				}
			}
			if(freeslot == -1) return SendClientMessage(playerid, COLOR_GRAD1, "That player does not have enough space for that.");
			Accessories[player][freeslot] = AdminAccessers[listitem][modid];
			AccessoriesB[player][freeslot] = 1;
			AccessoriessX[player][freeslot] = 1;
			AccessoriessY[player][freeslot] = 1;
			AccessoriessZ[player][freeslot] = 1;
			format(str, sizeof(str), "An admin has given you a \"%s\", Type /gear to equip it.", AdminAccessers[listitem][modname]);
			SendClientMessage(player, COLOR_LIGHTBLUE, str);
			format(str, sizeof(str), "You have given %s a \"%s\".", PlayerICName(player), AdminAccessers[listitem][modname]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		}
		return 1;
	}
	if(dialogid == 24208) // ID is Giving Accessory From Search
	{
		if(response)
		{
			new freeslot = -1, player = GetPVarInt(playerid, "ToGiveAccessorTo");
			for(new i = 0; i < 7; i++)
			{
				if(Accessories[player][i] == 0)
				{
					freeslot = i;
					break;
				}
			}
			if(freeslot == -1) return SendClientMessage(playerid, COLOR_GRAD1, "That player does not have enough space for that.");
			Accessories[player][freeslot] = AdminAccessers[accessorysr[playerid][listitem]][modid];
			AccessoriesB[player][freeslot] = 1;
			AccessoriessX[player][freeslot] = 1;
			AccessoriessY[player][freeslot] = 1;
			AccessoriessZ[player][freeslot] = 1;
			format(str, sizeof(str), "An admin has given you a \"%s\", Type /gear to equip it.", AdminAccessers[accessorysr[playerid][listitem]][modname]);
			SendClientMessage(player, COLOR_LIGHTBLUE, str);
			format(str, sizeof(str), "You have given %s a \"%s\".", PlayerICName(player), AdminAccessers[accessorysr[playerid][listitem]][modname]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		}
		return 1;
	}
	if(dialogid == 24209) // ID is Giving Accessory Search
	{
		if(response)
		{
			new hugestring[4096], listnum = 0;
			for(new i = 0; i < sizeof(AdminAccessers); i++)
			{
				if(AdminAccessers[i][modid] > 1 && strfind(AdminAccessers[i][modname], inputtext, true) != -1)
				{
					format(hugestring, sizeof(hugestring), "%s\n%s", hugestring, AdminAccessers[i][modname]);
					accessorysr[playerid][listnum] = i;
					listnum++;
				}
			}
			ShowPlayerDialog(playerid, 24208, DIALOG_STYLE_LIST, "Search results", hugestring, "Give", "Cancel");
		}
		return 1;
	}
	if(dialogid == 24210) // ID is Giving Accessory
	{
		if(response)
		{
			ShowPlayerDialog(playerid, 24209, DIALOG_STYLE_INPUT, "Enter model name...", "Please enter the model's name that you want to search for (e.g: SantaHat1)", "Search", "Cancel");
		}
		else
		{
			new hugestring[4096];
			for(new i = 0; i < sizeof(AdminAccessers); i++)
			{
				if(AdminAccessers[i][modid] > 1)
				{
					format(hugestring, sizeof(hugestring), "%s\n%s", hugestring, AdminAccessers[i][modname]);
				}
			}
			ShowPlayerDialog(playerid, 24207, DIALOG_STYLE_LIST, "List of models to give", hugestring, "Give", "Cancel");
		}
		return 1;
	}
	if(dialogid == DIALOG_BUYACCESSOR_MAIN) // ID is Buying Accessories
	{
		if(response)
		{
			new hugestring[3700];
			new pricestr[13];
			new pricestr2[13];
			switch(listitem)
			{
				case 0:   //Clothing Items 1
				{
					for(new a = 0; a < 127; a++)
					{
						if(AccessNames[a][modprice] == 1) { // item needs gear ticket
							format(pricestr, sizeof(pricestr), "[Ticket]");
						} else {
							valstr(pricestr2, AccessNames[a][modprice]);
							format(pricestr, sizeof(pricestr), "$%s", pricestr2);
						}

						format(hugestring, sizeof(hugestring), "%s\n%s  %s", hugestring, AccessNames[a][modname], pricestr);
					}
					listItemClothes[playerid] = 0;
				}
				case 1:   //Clothing Items 2
				{
					for(new a = 127; a < 244; a++)
					{
						if(AccessNames[a][modprice] == 1) { // item needs gear ticket
							format(pricestr, sizeof(pricestr), "[Ticket]");
						} else {
							valstr(pricestr2, AccessNames[a][modprice]);
							format(pricestr, sizeof(pricestr), "$%s", pricestr2);
						}

						format(hugestring, sizeof(hugestring), "%s\n%s  %s", hugestring, AccessNames[a][modname], pricestr);
					}
					listItemClothes[playerid] = 127;
				}
				case 2:   //Toys 1
				{
					for(new a = 244; a < 283; a++)
					{
						if(AccessNames[a][modprice] == 1) { // item needs gear ticket
							format(pricestr, sizeof(pricestr), "[Ticket]");
						} else {
							valstr(pricestr2, AccessNames[a][modprice]);
							format(pricestr, sizeof(pricestr), "$%s", pricestr2);
						}

						format(hugestring, sizeof(hugestring), "%s\n%s  %s", hugestring, AccessNames[a][modname], pricestr);
					}
					listItemClothes[playerid] = 244;
				}
				case 3:   //Toys 2
				{
					for(new a = 283; a < 345; a++)
					{
						if(AccessNames[a][modprice] == 1) { // item needs gear ticket
							format(pricestr, sizeof(pricestr), "[Ticket]");
						} else {
							valstr(pricestr2, AccessNames[a][modprice]);
							format(pricestr, sizeof(pricestr), "$%s", pricestr2);
						}

						format(hugestring, sizeof(hugestring), "%s\n%s  %s", hugestring, AccessNames[a][modname], pricestr);
					}
					listItemClothes[playerid] = 283;
				}
				case 4:	//VIP
				{
					for(new a = 345; a < sizeof(AccessNames); a++)
					{
						if(AccessNames[a][modprice] == 1) { // item needs gear ticket
							format(pricestr, sizeof(pricestr), "[Ticket]");
						} else {
							valstr(pricestr2, AccessNames[a][modprice]);
							format(pricestr, sizeof(pricestr), "$%s", pricestr2);
						}

						format(hugestring, sizeof(hugestring), "%s\n%s  %s", hugestring, AccessNames[a][modname], pricestr);
					}
					listItemClothes[playerid] = 345;
				}
			}
			ShowPlayerDialog(playerid, DIALOG_BUYACCESSOR, DIALOG_STYLE_LIST, "Gear", hugestring, "Purchase", "Back");
		}
		return 1;
	}
	if(dialogid == DIALOG_BUYACCESSOR) // ID is Buying Accessories
	{
		if(response)
		{
			//0 + 127-127+1
			listitem = listItemClothes[playerid] +listitem;
			if(!IsAtClothShop(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You are not at a clothes shop.");
			new biz = GetPlayerVirtualWorld(playerid);
			if(GetMoney(playerid) < AccessNames[listitem][modprice]) return SendClientMessage(playerid, COLOR_GRAD1, "You do not have enough money to pay for that.");
			new freeslot = -1;
			for(new i = 0; i < 7; i++)
			{
				if(Accessories[playerid][i] == 0)
				{
					freeslot = i;
					break;
				}
			}
			if(freeslot == -1) return SendClientMessage(playerid, COLOR_GRAD1, "You do not have enough space for that.");
			PlayerTextDrawDestroy(playerid, pModel_t[playerid]);
			pModelState[playerid] = 999;
			pModel_t[playerid] = CreatePlayerTextDraw(playerid, 220.0, 220.0, "_");
			PlayerTextDrawFont(playerid, pModel_t[playerid], TEXT_DRAW_FONT_MODEL_PREVIEW);
			PlayerTextDrawTextSize(playerid, pModel_t[playerid], 200.0, 200.0);
			PlayerTextDrawSetPreviewModel(playerid, pModel_t[playerid], AccessNames[listitem][modid]);
			PlayerTextDrawSetPreviewRot(playerid, pModel_t[playerid], -15.0, 0.0, 0.0);
			PlayerTextDrawUseBox(playerid, pModel_t[playerid], 0);
			PlayerTextDrawBackgroundColor(playerid, pModel_t[playerid], 0x00000000);
			PlayerTextDrawShow(playerid, pModel_t[playerid]);
			pModelState[playerid] = 1;
			pModelRot[playerid][0] = 0;
			pModelRot[playerid][1] = 0;
			pModelRot[playerid][2] = 0;
			pModelTimer[playerid] = SetTimerEx("UpdatePreviewModel", 1000, 1, "i", playerid);
			format(str, sizeof(str), "{B4B5B7}Are you sure you would like to purchase the %s for $%d?", AccessNames[listitem][modname], AccessNames[listitem][modprice]);
			ShowPlayerDialog(playerid, 772, DIALOG_STYLE_MSGBOX, "Are you sure?", str, "Purchase", "Back");
			SetPVarInt(playerid, "buyaccbincobiz", biz);
			SetPVarInt(playerid, "buyaccbinco", listitem);
		}
		else
		{
			listItemClothes[playerid] = -1;
			ShowPlayerDialog(playerid, DIALOG_BUYACCESSOR_MAIN, DIALOG_STYLE_LIST, "Gear", "Clothing Items 1\nClothing Items 2\nToys 1\nToys 2\nSpecial", "Select", "Cancel");
		}
		return 1;
	}
	if(dialogid == 772)
	{
		PlayerTextDrawDestroy(playerid, pModel_t[playerid]);
		pModelState[playerid] = 999;
		if(!response)
		{
			DeletePVar(playerid, "buyaccbincobiz");
			DeletePVar(playerid, "buyaccbinco");
			new hugestring[3700];
			new pricestr[13];
			new pricestr2[13];
			switch(listItemClothes[playerid])
			{

				case 0:
				{ //Clothing Items 1
					for(new a = 0; a < 127; a++)
					{
						if(AccessNames[a][modprice] == 1) { // item needs gear ticket
							format(pricestr, sizeof(pricestr), "[Ticket]");
						} else {
							valstr(pricestr2, AccessNames[a][modprice]);
							format(pricestr, sizeof(pricestr), "$%s", pricestr2);
						}
						format(hugestring, sizeof(hugestring), "%s\n%s  %s", hugestring, AccessNames[a][modname], pricestr);

						listItemClothes[playerid] = 0;
						ShowPlayerDialog(playerid, DIALOG_BUYACCESSOR, DIALOG_STYLE_LIST, "Gear", hugestring, "Purchase", "Back");
					}
				}
				case 127:
				{ //Clothing Items 2
					for(new a = 127; a < 244; a++)
					{
						if(AccessNames[a][modprice] == 1) { // item needs gear ticket
							format(pricestr, sizeof(pricestr), "[Ticket]");
						} else {
							valstr(pricestr2, AccessNames[a][modprice]);
							format(pricestr, sizeof(pricestr), "$%s", pricestr2);
						}
						format(hugestring, sizeof(hugestring), "%s\n%s  %s", hugestring, AccessNames[a][modname], pricestr);

						listItemClothes[playerid] = 127;
						ShowPlayerDialog(playerid, DIALOG_BUYACCESSOR, DIALOG_STYLE_LIST, "Gear", hugestring, "Purchase", "Back");
					}
				}
				case 244:
				{ //Toys 1
					for(new a = 244; a < 283; a++)
					{
						if(AccessNames[a][modprice] == 1) { // item needs gear ticket
							format(pricestr, sizeof(pricestr), "[Ticket]");
						} else {
							valstr(pricestr2, AccessNames[a][modprice]);
							format(pricestr, sizeof(pricestr), "$%s", pricestr2);
						}
						format(hugestring, sizeof(hugestring), "%s\n%s  %s", hugestring, AccessNames[a][modname], pricestr);

						listItemClothes[playerid] = 244;
						ShowPlayerDialog(playerid, DIALOG_BUYACCESSOR, DIALOG_STYLE_LIST, "Gear", hugestring, "Purchase", "Back");
					}
				}
				case 283:
				{ //Toys 2
					for(new a = 283; a < 345; a++)
					{
						if(AccessNames[a][modprice] == 1) { // item needs gear ticket
							format(pricestr, sizeof(pricestr), "[Ticket]");
						} else {
							valstr(pricestr2, AccessNames[a][modprice]);
							format(pricestr, sizeof(pricestr), "$%s", pricestr2);
						}
						format(hugestring, sizeof(hugestring), "%s\n%s  %s", hugestring, AccessNames[a][modname], pricestr);

						listItemClothes[playerid] = 283;
						ShowPlayerDialog(playerid, DIALOG_BUYACCESSOR, DIALOG_STYLE_LIST, "Gear", hugestring, "Purchase", "Back");
					}
				}
				case 345:
				{ //Special
					for(new a = 345; a < sizeof(AccessNames); a++)
					{
						if(AccessNames[a][modprice] == 1) { // item needs gear ticket
							format(pricestr, sizeof(pricestr), "[Ticket]");
						} else {
							valstr(pricestr2, AccessNames[a][modprice]);
							format(pricestr, sizeof(pricestr), "$%s", pricestr2);
						}
						format(hugestring, sizeof(hugestring), "%s\n%s  %s", hugestring, AccessNames[a][modname], pricestr);

						listItemClothes[playerid] = 345;
						ShowPlayerDialog(playerid, DIALOG_BUYACCESSOR, DIALOG_STYLE_LIST, "Gear", hugestring, "Purchase", "Back");
					}
				}
				default:
				{
					listItemClothes[playerid] = -1;
					ShowPlayerDialog(playerid, DIALOG_BUYACCESSOR_MAIN, DIALOG_STYLE_LIST, "Gear", "Clothing Items 1\nClothing Items 2\nToys 1\nToys 2\nSpecial", "Select", "Cancel");
				}
			}
			return 1;
		}
		else
		{
			new biz = GetPVarInt(playerid, "buyaccbincobiz");
			new accid = GetPVarInt(playerid, "buyaccbinco");
			new freeslot = -1;
			for(new i = 0; i < 7; i++)
			{
				if(Accessories[playerid][i] == 0)
				{
					freeslot = i;
					break;
				}
			}
			if(freeslot == -1)
			{
				listItemClothes[playerid] = -1;
				SendClientMessage(playerid, COLOR_GRAD1, "You do not have enough space for that.");
				return 1;
			}

			if(AccessNames[accid][modprice] == 1) //if the accessory modprice is 1 then it's a special gear
			{ //special menu
				if(PlayerInfo[playerid][gTicket] < 1)
				{
					listItemClothes[playerid] = -1;
					SendClientMessage(playerid, COLOR_GRAD1, "You do not have any gear tickets, you can purchase them at donate.gta-sarp.epizy.com.");
					return 1;
				}
				PlayerInfo[playerid][gTicket]--;
				format(str, sizeof(str), "* You have used a Gear Ticket, you have %d remaining.", PlayerInfo[playerid][gTicket]);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			}

			Accessories[playerid][freeslot] = AccessNames[accid][modid];
			AccessoriesB[playerid][freeslot] = 1;
			AccessoriessX[playerid][freeslot] = 1;
			AccessoriessY[playerid][freeslot] = 1;
			AccessoriessZ[playerid][freeslot] = 1;
			BizInfo[biz][bTill] += AccessNames[accid][modprice];
			BizInfo[biz][bProducts] -= 1;
			GiveMoney(playerid, -AccessNames[accid][modprice]);

			new buygearstr[128];
			format(buygearstr, sizeof(buygearstr), "~r~%s~w~ purchased! Type /gear to equip it.", AccessNames[accid][modname]);
			displayCenterHUDInfo(playerid,  buygearstr, 8); //display for 8 seconds
			listItemClothes[playerid] = -1;
			DeletePVar(playerid, "buyaccbincobiz");
			DeletePVar(playerid, "buyaccbinco");
		}
		return 1;
	}

	if(dialogid == 24215) // ID is Inventory Accessory List
	{
		if(response)
		{
			if(listitem <= 6)   //if it's a gear slot
			{
				if(Accessories[playerid][listitem] == 0) //if there is nothing in the slot
				{
					SendClientMessage(playerid, COLOR_GRAD1, "That slot is empty.");
					ShowAccessDialog(playerid, 0);
					return 1;
				}
				else
				{
					new titlestring[128];
					format(titlestring, sizeof(titlestring), "Gear %d", listitem +1);
					SetPVarInt(playerid, "ManageAccessID", listitem);
					if(!IsPlayerAttachedObjectSlotUsed(playerid, listitem))
					{
						ShowPlayerDialog(playerid, 24216, DIALOG_STYLE_LIST, titlestring, "Equip\nAdjust Position\nChange Bone\nChange Color\nDelete", "Select", "Back");
					}
					else
					{
						ShowPlayerDialog(playerid, 24216, DIALOG_STYLE_LIST, titlestring, "Unequip\nAdjust Position\nChange Bone\nChange Color\nDelete", "Select", "Back");
					}
				}
			}
			else if(listitem == 7)
			{
				equipAllGear(playerid);
			}
			else if(listitem == 8)
			{
				removeAllGear(playerid);
			}
		}
		return 1;
	}
	if(dialogid == 24216) // ID is Inventory Manage Item
	{
		if(response)
		{
			new manageid = GetPVarInt(playerid, "ManageAccessID");
			if(listitem == 0)
			{
				if(GetPVarInt(playerid, "ManageAccessID") < 0 || GetPVarInt(playerid, "ManageAccessID") > 6) return SendClientMessage(playerid, COLOR_RED, "ACCESS DENIED: Hacking attempt.");
				if(IsPlayerAttachedObjectSlotUsed(playerid, manageid))
				{
					RemovePlayerAttachedObject(playerid, manageid);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have unequipped the gear.");
				}
				else
				{
					//equipping for the first time, bone/position is default
					if(AccessoriesX[playerid][manageid] == 0.0 && AccessoriesY[playerid][manageid] == 0.0 && AccessoriesZ[playerid][manageid] == 0.0)
					{
						new bonestring[256 +1];
						for(new i; i < sizeof(AttachmentBones); i++)
						{
							format(bonestring, sizeof(bonestring), "%s\n%s", bonestring, AttachmentBones[i]);
						}
						ShowPlayerDialog(playerid, 24217, DIALOG_STYLE_LIST, "Select Bone", bonestring, "Select", "Back");
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "* Select the bodypart which you would like to equip the gear to.");
					}
					else
					{
						SetPlayerAttachedObject(playerid, manageid, Accessories[playerid][manageid], AccessoriesB[playerid][manageid], AccessoriesX[playerid][manageid], AccessoriesY[playerid][manageid], AccessoriesZ[playerid][manageid], AccessoriesrX[playerid][manageid], AccessoriesrY[playerid][manageid], AccessoriesrZ[playerid][manageid], AccessoriessX[playerid][manageid], AccessoriessY[playerid][manageid], AccessoriessZ[playerid][manageid], GetGearColour(Accessoriesc1[playerid][manageid]));
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have equipped the gear.");
						DeletePVar(playerid, "ManageAccessID");
					}
				}
			}
			else if(listitem == 1)
			{
				if(GetPVarInt(playerid, "ManageAccessID") < 0 || GetPVarInt(playerid, "ManageAccessID") > 6) return SendClientMessage(playerid, COLOR_RED, "ACCESS DENIED: Hacking attempt.");
				if(AccessoriesB[playerid][manageid] <= 0)
				{
					AccessoriesB[playerid][manageid] = 1;
				}
				if(AccessoriessX[playerid][manageid] <= 0 && AccessoriessY[playerid][manageid] <= 0 && AccessoriessZ[playerid][manageid] <= 0)
				{
					AccessoriessX[playerid][manageid] = 1;
					AccessoriessY[playerid][manageid] = 1;
					AccessoriessZ[playerid][manageid] = 1;
				}
				SetPlayerAttachedObject(playerid, manageid, Accessories[playerid][manageid], AccessoriesB[playerid][manageid], AccessoriesX[playerid][manageid], AccessoriesY[playerid][manageid], AccessoriesZ[playerid][manageid], AccessoriesrX[playerid][manageid], AccessoriesrY[playerid][manageid], AccessoriesrZ[playerid][manageid], AccessoriessX[playerid][manageid], AccessoriessY[playerid][manageid], AccessoriessZ[playerid][manageid], GetGearColour(Accessoriesc1[playerid][manageid]));
				EditAttachedObject(playerid, manageid);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "* Use the editor to adjust the position, rotation and scale. Press V to change camera zoom, hold space to adjust camera position.");
			}
			else if(listitem == 2)
			{
				if(GetPVarInt(playerid, "ManageAccessID") < 0 || GetPVarInt(playerid, "ManageAccessID") > 6) return SendClientMessage(playerid, COLOR_RED, "ACCESS DENIED: Hacking attempt.");
				new bonestring[256 +1];
				for(new i; i < sizeof(AttachmentBones); i++)
				{
					format(bonestring, sizeof(bonestring), "%s\n%s", bonestring, AttachmentBones[i]);
				}
				ShowPlayerDialog(playerid, 24217, DIALOG_STYLE_LIST, "Select Bone", bonestring, "Select", "Back");
			}
			else if(listitem == 3) //color
			{
				if(GetPVarInt(playerid, "ManageAccessID") < 0 || GetPVarInt(playerid, "ManageAccessID") > 6) return SendClientMessage(playerid, COLOR_RED, "ACCESS DENIED: Hacking attempt."); //dont know what this is for..
				ShowPlayerDialog(playerid, 24218, DIALOG_STYLE_LIST, "Select Color", "None\nRed\nLight Red\nDark Red\nGreen\nLight Green\nDark Green\nBlue\nLight Blue\nDark Blue\nPink\nLight Pink\nDark Pink\nPurple\nLight Purple\nDark Purple\nOrange\nLight Orange\nDark Orange\nTurquoise\nLight Turquoise\nDark Turquoise\nBrown\nLight Brown\nDark Brown\nGrey\nLight Grey\nDark Grey\nYellow\nLight Yellow\nDark Yellow\nBlack\nWhite", "Select", "Back");
			}
			else if(listitem == 4)
			{
				new msg[128];
				for(new check = 0; check < sizeof(AccessNames); check++)
				{
					if(AccessNames[check][modid] == Accessories[playerid][manageid])
					{							
						format(msg, sizeof(msg), "Are you sure you want to delete %s?", AccessNames[check][modname]);
						break;
					}
				}				
				ShowPlayerDialog(playerid, DIALOG_GEAR_DELETE, DIALOG_STYLE_MSGBOX, "Gear Delete Confirmation", msg, "Delete", "Cancel");
			}
		}
		else
		{
			ShowAccessDialog(playerid, 0);
		}
		return 1;
	}
	if(dialogid == DIALOG_GEAR_DELETE)
	{
		if(response)
		{
			new manageid = GetPVarInt(playerid, "ManageAccessID");
			if(manageid < 0 || manageid > 6) return SendClientMessage(playerid, COLOR_RED, "ACCESS DENIED: Hacking attempt.");
			RemovePlayerAttachedObject(playerid, manageid);
			Accessories[playerid][manageid] = 0;
			AccessoriesB[playerid][manageid] = 0;
			AccessoriesX[playerid][manageid] = 0;
			AccessoriesY[playerid][manageid] = 0;
			AccessoriesZ[playerid][manageid] = 0;
			AccessoriesrX[playerid][manageid] = 0;
			AccessoriesrY[playerid][manageid] = 0;
			AccessoriesrZ[playerid][manageid] = 0;
			AccessoriessX[playerid][manageid] = 0;
			AccessoriessY[playerid][manageid] = 0;
			AccessoriessZ[playerid][manageid] = 0;
			Accessoriesc1[playerid][manageid] = 0; //color1
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have deleted the gear.");
			DeletePVar(playerid, "ManageAccessID");
		}
	}
	if(dialogid == 24218)
	{
		if(response)
		{
			new manageid = GetPVarInt(playerid, "ManageAccessID");
			if(GetPVarInt(playerid, "ManageAccessID") < 0 || GetPVarInt(playerid, "ManageAccessID") > 6) return SendClientMessage(playerid, COLOR_RED, "ACCESS DENIED: Hacking attempt.");
			Accessoriesc1[playerid][manageid] = listitem;
			if(IsPlayerAttachedObjectSlotUsed(playerid, manageid))
			{
				SetPlayerAttachedObject(playerid, manageid, Accessories[playerid][manageid], AccessoriesB[playerid][manageid], AccessoriesX[playerid][manageid], AccessoriesY[playerid][manageid], AccessoriesZ[playerid][manageid], AccessoriesrX[playerid][manageid], AccessoriesrY[playerid][manageid], AccessoriesrZ[playerid][manageid], AccessoriessX[playerid][manageid], AccessoriessY[playerid][manageid], AccessoriessZ[playerid][manageid], GetGearColour(Accessoriesc1[playerid][manageid]));
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have adjusted the color of the gear.");
			}
		}
		else
		{
		}
		DeletePVar(playerid, "ManageAccessID");
		return 1;
	}
	if(dialogid == 24217) //edit accessory select bone
	{
		new manageid = GetPVarInt(playerid, "ManageAccessID");
		if(response)
		{
			AccessoriesB[playerid][manageid] = listitem +1;
			SetPlayerAttachedObject(playerid, manageid, Accessories[playerid][manageid], AccessoriesB[playerid][manageid], AccessoriesX[playerid][manageid], AccessoriesY[playerid][manageid], AccessoriesZ[playerid][manageid], AccessoriesrX[playerid][manageid], AccessoriesrY[playerid][manageid], AccessoriesrZ[playerid][manageid], AccessoriessX[playerid][manageid], AccessoriessY[playerid][manageid], AccessoriessZ[playerid][manageid], GetGearColour(Accessoriesc1[playerid][manageid]));
			EditAttachedObject(playerid, manageid);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* Use the editor to adjust the position, rotation and scale. Press V to change camera zoom, hold space to adjust camera position.");
			DeletePVar(playerid, "ManageAccessID"); //added
		}
		else
		{
			new titlestring[128], access;
			for(new check = 0; check < sizeof(AccessNames); check++)
			{
				if(AccessNames[check][modid] == Accessories[playerid][manageid])
				{
					access = check;
				}
			}
			format(titlestring, sizeof(titlestring), "%s", AccessNames[access][modname]);
			if(!IsPlayerAttachedObjectSlotUsed(playerid, listitem))
			{
				ShowPlayerDialog(playerid, 24216, DIALOG_STYLE_LIST, titlestring, "Equip\nAdjust Position\nChange Bone\nChange Color\nDelete", "Select", "Back");
			}
			else
			{
				ShowPlayerDialog(playerid, 24216, DIALOG_STYLE_LIST, titlestring, "Unequip\nAdjust Position\nChange Bone\nChange Color\nDelete", "Select", "Back");
			}
		}
		return 1;
	}
	if(dialogid == DIALOG_BUSSINESS_BUY) //247
	{
		if(response)
		{
			new biz = GetPlayerVirtualWorld(playerid);
			if(listitem == 0) //cellphone
			{
				if(PlayerInfo[playerid][pCash] > 2500)
				{
					GiveMoney(playerid, -2500);
					BizInfo[biz][bTill] += 5000;
					BizInfo[biz][bProducts] -= 1;
					buyCellphone(playerid, 0); //0 = default type
					format(str, sizeof(str), "~r~-$%d", 2500);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 1) //phonebook
			{
				if(PlayerInfo[playerid][pPhoneBook] == 1)
				{
					SendClientMessage(playerid, COLOR_GRAD2, "You already have a phonebook.");
					return 1;
				}
				if(PlayerInfo[playerid][pCash] > 5000)
				{
					GiveMoney(playerid, -5000);
					BizInfo[biz][bTill] += 10000;
					BizInfo[biz][bProducts] -= 1;
					PlayerInfo[playerid][pPhoneBook] = 1;
					format(str, sizeof(str), "~r~-$%d", 5000);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Phonebook~w~ purchased!~n~Type ~y~/number~w~ to look up other players numbers.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 2) //dice
			{
				if(PlayerInfo[playerid][pDice] == 1)
				{
					SendClientMessage(playerid, COLOR_GRAD2, "You already have dice.");
					return 1;
				}
				if(PlayerInfo[playerid][pCash] > 500)
				{
					GiveMoney(playerid, -500);
					BizInfo[biz][bTill] += 1000;
					BizInfo[biz][bProducts] -= 1;
					PlayerInfo[playerid][pDice] = 1;
					format(str, sizeof(str), "~r~-$%d", 500);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Dice~w~ purchased!~n~Type ~y~/dice~w~ to use.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 3)
			{
				if(PlayerInfo[playerid][piPod] > 0)
				{
					SendClientMessage(playerid, COLOR_GRAD2, "You already have a iPod.");
					return 1;
				}
				if(PlayerInfo[playerid][pCash] > 4500)
				{
					GiveMoney(playerid, -4500);
					BizInfo[biz][bTill] += 9000;
					BizInfo[biz][bProducts] -= 1;
					PlayerInfo[playerid][piPod] = 1;
					format(str, sizeof(str), "~r~-$%d", 4500);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~iPod~w~ purchased!~n~type ~y~/ipod~w~ to listen to music.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 4)
			{
				if(PlayerInfo[playerid][pSpraycan] >= 10)
				{
					SendClientMessage(playerid, COLOR_GRAD2, "You already have 10 or more spray cans.");
					return 1;
				}
				if(PlayerInfo[playerid][pCash] >= 2000)
				{
					GiveMoney(playerid, -2000);
					BizInfo[biz][bTill] += 4000;
					BizInfo[biz][bProducts] -= 1;
					PlayerInfo[playerid][pSpraycan] += 10;
					format(str, sizeof(str), "~r~-$%d", 2000);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~10 Spraycans~w~ purchased!~n~type ~y~/colorcar~w~ or ~y~/paintcar~w~ to paint vehicles.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 5)
			{
				if(PlayerInfo[playerid][pRope] >= 9)
				{
					SendClientMessage(playerid, COLOR_GRAD2, "You already have 9 or more ropes.");
					return 1;
				}
				if(PlayerInfo[playerid][pCash] > 2000)
				{
					GiveMoney(playerid, -2000);
					BizInfo[biz][bTill] += 4000;
					BizInfo[biz][bProducts] -= 1;
					PlayerInfo[playerid][pRope] += 3;
					format(str, sizeof(str), "~r~-$%d", 2000);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~3 Ropes~w~ purchased!~n~type ~y~/tie~w~ while driving a car to tie your passengers.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 6)
			{
				if(PlayerInfo[playerid][pCigars] >= 10)
				{
					SendClientMessage(playerid, COLOR_GRAD2, "You already have 10 or more cigars.");
					return 1;
				}
				if(PlayerInfo[playerid][pCash] > 500)
				{
					GiveMoney(playerid, -500);
					BizInfo[biz][bTill] += 1000;
					BizInfo[biz][bProducts] -= 1;
					PlayerInfo[playerid][pCigars] += 10;
					format(str, sizeof(str), "~r~-$%d", 500);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~10 Cigars~w~ purchased!~n~type ~y~/usecigar~w~ to use, LMB to smoke, press F to drop.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 7)
			{
				if(PlayerInfo[playerid][pSprunk] >= 3)
				{
					SendClientMessage(playerid, COLOR_GRAD2, "You already have 3 or more cans of sprunk.");
					return 1;
				}
				if(PlayerInfo[playerid][pCash] > 1000)
				{
					GiveMoney(playerid, -1000);
					BizInfo[biz][bTill] += 2000;
					BizInfo[biz][bProducts] -= 1;
					PlayerInfo[playerid][pSprunk] += 1;
					format(str, sizeof(str), "~r~-$%d", 1000);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Sprunk~w~ purchased!~n~type ~y~/usesoda~w~ to use, LMB to drink, press F to drop.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 8) //walkie talkie
			{
				if(PlayerInfo[playerid][pWT] == 1)
				{
					SendClientMessage(playerid, COLOR_GRAD2, "You already have a Walkie Talkie.");
					return 1;
				}
				if(PlayerInfo[playerid][pCash] > 1000)
				{
					GiveMoney(playerid, -1000);
					BizInfo[biz][bTill] += 2000;
					BizInfo[biz][bProducts] -= 1;
					PlayerInfo[playerid][pWT] = 1;
					format(str, sizeof(str), "~r~-$%d", 1000);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Walkie Talkie~w~ purchased!~n~~y~/channel~w~ to set your channel number, then use ~y~/wt~w~ to type.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 9) //screw drivers
			{
				if(PlayerInfo[playerid][pScrew] >= 5)
				{
					SendClientMessage(playerid, COLOR_GRAD2, "You already have 5 or more screw drivers.");
					return 1;
				}
				if(PlayerInfo[playerid][pCash] > 1000)
				{
					GiveMoney(playerid, -1000);
					BizInfo[biz][bTill] += 2000;
					BizInfo[biz][bProducts] -= 1;
					PlayerInfo[playerid][pScrew] += 5;
					format(str, sizeof(str), "~r~-$%d", 1000);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~5 Screw Drivers~w~ purchased!~n~type ~y~/breakin~w~ to pick the locks on someones ~g~houses~w~.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 10) //blindfolds
			{
				if(PlayerInfo[playerid][pBlindfolds] >= 6)
				{
					SendClientMessage(playerid, COLOR_GRAD2, "You already have 6 or more blindfolds.");
					return 1;
				}
				if(PlayerInfo[playerid][pCash] >= 4500)
				{
					GiveMoney(playerid, -4500);
					BizInfo[biz][bTill] += 9000;
					BizInfo[biz][bProducts] -= 1;
					PlayerInfo[playerid][pBlindfolds] += 2;
					format(str, sizeof(str), "~r~-$%d", 4500);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~2 Blindfolds~w~ purchased!~n~type ~y~/blindfold~w~ to blindfold someone who is tied.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 11) //cards
			{
				if(PlayerInfo[playerid][pCash] >= 2000)
				{
					GiveMoney(playerid, -2000);
					BizInfo[biz][bTill] += 4000;
					BizInfo[biz][bProducts] -= 1;
					format(str, sizeof(str), "~r~-$%d", 2000);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					PlayerInfo[playerid][pDeck] = 1;
					displayCenterHUDInfo(playerid,  "~r~Deck Of Cards~w~ purchased!~n~type ~y~/help~w~ view the gambling commands.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 12) //address book
			{
				if(PlayerInfo[playerid][pCash] >= 5000)
				{
					GiveMoney(playerid, -5000);
					BizInfo[biz][bTill] += 10000;
					BizInfo[biz][bProducts] -= 1;
					PlayerInfo[playerid][pAddressBook] = 1;
					format(str, sizeof(str), "~r~-$%d", 5000);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~2 Address Book~w~ purchased!~n~type ~y~/address~w~ to find out where someone lives.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 13)
			{
				if(PlayerInfo[playerid][pCash] >= 1250)
				{
					GiveMoney(playerid, -1250);
					BizInfo[biz][bTill] += 2500;
					BizInfo[biz][bProducts] -= 1;
					PlayerInfo[playerid][pCalculator] = 1;
					format(str, sizeof(str), "~r~-$%d", 1250);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Calculator~w~ purchased!~n~type ~y~/calculate~w~ to use it.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 14) // Stero
			{
				if(PlayerInfo[playerid][pStereo] > 0) return SendClientMessage(playerid, COLOR_GRAD2, "You already have a Boombox.");
				if(PlayerInfo[playerid][pCash] >= 6000)
				{
					GiveMoney(playerid, -6000);
					BizInfo[biz][bTill] += 12000;
					BizInfo[biz][bProducts] -= 1;
					PlayerInfo[playerid][pStereo] = 1;
					format(str, sizeof(str), "~r~-$%d", 6000);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Boombox~w~ purchased!~n~type ~y~/placeboombox~w~ to place it on the ground.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 15) //map
			{
				if(PlayerInfo[playerid][pMap] != 0) return SendClientMessage(playerid, COLOR_GREY, "You already have a map.");
				if(PlayerInfo[playerid][pCash] >= 1000)
				{
					GiveMoney(playerid, -1000);
					BizInfo[biz][bTill] += 2000;
					BizInfo[biz][bProducts] -= 1;
					format(str, sizeof(str), "~r~-%d", 1000);
					PlayerInfo[playerid][pMap] = 1;
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid, "~r~Map~w~ purchased!~n~type ~y~/map~w~ to use it", 8);
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You don't have the cash for that.");
				}
			}
			if(listitem == 16) //Box of Matches
			{
				if(PlayerInfo[playerid][pMatches] > 0) return SendClientMessage(playerid, COLOR_GRAD2, "You can only carry one box of matches.");
				if(PlayerInfo[playerid][pCash] >= 850)
				{
					GiveMoney(playerid, -850);
					BizInfo[biz][bTill] += 1700;
					BizInfo[biz][bProducts] -= 1;
					PlayerInfo[playerid][pMatches] = 3;
					format(str, sizeof(str), "~r~-$%d", 850);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Box of Matches~w~ purchased!~n~type ~y~/lightfire~w~ to light a fire.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 17) //Jerry Can
			{
				if(PlayerInfo[playerid][pJerrycan] > 0) return SendClientMessage(playerid, COLOR_GRAD2, "You can only carry one jerry can.");
				if(PlayerInfo[playerid][pCash] >= 1000)
				{
					GiveMoney(playerid, -1000);
					BizInfo[biz][bTill] += 2000;
					BizInfo[biz][bProducts] -= 1;
					PlayerInfo[playerid][pJerrycan] = 1;
					format(str, sizeof(str), "~r~-$%d", 2000);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Jerry Can~w~ purchased!~n~type ~y~/fill~w~ to fill the jerry can.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 18) //case of beer
			{
				if(PlayerInfo[playerid][pCash] >= 2000)
				{
					GiveMoney(playerid, -2000);
					BizInfo[biz][bTill] += 4000;
					BizInfo[biz][bProducts] -= 1;
					PlayerInfo[playerid][pBeer] = 5;
					format(str, sizeof(str), "~r~-$%d", 2000);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Case of beert~w~ purchased!~n~type ~y~/usebeer~w~ to get a beer in your hand.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 19) // drill
			{
				if(PlayerInfo[playerid][pLevel] < 4) return SendClientMessage(playerid, COLOR_GREY, "You need level 4+ to buy this.");
				if(PlayerInfo[playerid][pDrill] == 1) return SendClientMessage(playerid, COLOR_GREY, "You already have a drill, /usedrill near the vault to use it.");
				if(PlayerInfo[playerid][pCash] >= 250000)
				{
					GiveMoney(playerid, -250000);
					BizInfo[biz][bTill] += 500000; // 10 percents
					BizInfo[biz][bProducts] -= 1;
					PlayerInfo[playerid][pDrill] = 1;
					format(str, sizeof(str), "~r~-$%d", 250000);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Drillt~w~ purchased!~n~type ~y~/usedrill~w~ near the vault to start drilling.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 20) // duffle bag
			{
				if(holdingDufflebag[playerid]) return SendClientMessage(playerid, COLOR_GREY, "You already have a dufflebag, /dufflebag.");
				// cooldown is here to ensure players arent buying and dropping duffle bags constantly, therefore filling the world with objects
				// duffle bags disappear 10 minutes after they are dropped
				if(gettime() < PlayerInfo[playerid][pDuffleBagTime]) return SendClientMessage(playerid, COLOR_GREY, "You must wait 20 minutes before purchasing another duffle bag.");
				if(PlayerInfo[playerid][pCash] >= 10000)
				{
					new duffleid = CreateDuffleBag();
					// in case duffleid is -1, it means that dufflebags reached their maximum (200 on the server)
					// if we do get to such a case, i might adjust MAXIMUM_DUFFLEBAGS on dufflebag.inc
					if(duffleid == -1) return SendClientMessage(playerid, COLOR_GREY, "An error occured ~ contact Desmond.");

					AttachDufflebagToPlayer(playerid, duffleid);

					GiveMoney(playerid, -10000);
					BizInfo[biz][bTill] += 20000; 
					BizInfo[biz][bProducts] -= 1;

					PlayerInfo[playerid][pDuffleBagTime] = gettime()+DUFFLEBAG_COOLDOWN;
					
					format(str, sizeof(str), "~r~-$%d", 10000);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Dufflebagt~w~ purchased!~n~type ~y~/dufflebag.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}

			}
			if(listitem == 21) // briefcase
			{
				if(PlayerInfo[playerid][pCash] >= 10000)
				{
					GiveMoney(playerid, -10000);
					BizInfo[biz][bTill] += 20000;
					BizInfo[biz][bProducts] -= 1;
					PlayerInfo[playerid][pSuitcase] = 1;
					format(str, sizeof(str), "~r~-$%d", 10000);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Briefcase~w~ purchased!~n~type ~y~/briefcase~w~ to use it.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(BizInfo[biz][bProducts] <= 0) SetNoProductsDate(biz);
		}
		return 1;
	}
	if(dialogid == DIALOG_BUSINESS_DRINK) //bar
	{
		if(response)
		{
			new biz = GetPlayerVirtualWorld(playerid);
			if(BizInfo[biz][bProducts] <= 0 && GetPlayerVirtualWorld(playerid) != 0)
			{
				SendClientMessage(playerid, COLOR_GREY, "This business doesn't have any products left.");
				return 1;
			}
			if(listitem == 0) //beer
			{
				if(PlayerInfo[playerid][pCash] > 19)
				{
					GiveMoney(playerid, -20);
					BizInfo[biz][bTill] += 20;
					BizInfo[biz][bProducts] -=1;
					SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_BEER);
					format(str, sizeof(str), "~r~-$%d", 20);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Beer~w~ purchased!~n~press LMB to drink, press F to drop.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 1) //vodka
			{
				if(PlayerInfo[playerid][pCash] > 29)
				{
					GiveMoney(playerid, -30);
					BizInfo[biz][bTill] += 30;
					BizInfo[biz][bProducts] -=1;
					SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_WINE);
					format(str, sizeof(str), "~r~-$%d", 30);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Vodka~w~ purchased!~n~press LMB to drink, press F to drop.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 2) //whiskey
			{
				if(PlayerInfo[playerid][pCash] > 34)
				{
					GiveMoney(playerid, -35);
					BizInfo[biz][bTill] += 35;
					BizInfo[biz][bProducts] -=1;
					SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_WINE);
					format(str, sizeof(str), "~r~-$%d", 35);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Whiskey~w~ purchased!~n~press LMB to drink, press F to drop.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 3) //wine
			{
				if(PlayerInfo[playerid][pCash] > 24)
				{
					GiveMoney(playerid, -25);
					BizInfo[biz][bTill] += 25;
					BizInfo[biz][bProducts] -=1;
					SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_WINE);
					format(str, sizeof(str), "~r~-$%d", 25);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Wine~w~ purchased!~n~press LMB to drink, press F to drop.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 4) //soda
			{
				if(PlayerInfo[playerid][pCash] > 14)
				{
					GiveMoney(playerid, -15);
					BizInfo[biz][bTill] += 15;
					BizInfo[biz][bProducts] -=1;
					SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);
					format(str, sizeof(str), "~r~-$%d", 15);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Sprunk~w~ purchased!~n~press LMB to drink, press F to drop.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(BizInfo[biz][bProducts] <= 0)
			{
				SetNoProductsDate(biz);
			}
		}
		return 1;
	}
	if(dialogid == DIALOG_TRAIN) //gym
	{
		if(response)
		{
			if(listitem == 0) //normal
			{
				PlayerInfo[playerid][pFightingStyle] = FIGHT_STYLE_NORMAL;
				SetPlayerFightingStyle(playerid, PlayerInfo[playerid][pFightingStyle]);
				PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have learned a new fighting style.");
			}
			if(listitem == 1) //boxing
			{
				if(PlayerInfo[playerid][pCash] > 49999)
				{
					PlayerInfo[playerid][pFightingStyle] = FIGHT_STYLE_BOXING;
					SetPlayerFightingStyle(playerid, PlayerInfo[playerid][pFightingStyle]);
					GiveMoney(playerid, -50000);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have learned a new fighting style.");
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 2) //kung fu
			{
				if(PlayerInfo[playerid][pCash] > 49999)
				{
					PlayerInfo[playerid][pFightingStyle] = FIGHT_STYLE_KUNGFU;
					SetPlayerFightingStyle(playerid, PlayerInfo[playerid][pFightingStyle]);
					GiveMoney(playerid, -50000);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have learned a new fighting style.");
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 3) //kneehead
			{
				if(PlayerInfo[playerid][pCash] > 49999)
				{
					PlayerInfo[playerid][pFightingStyle] = FIGHT_STYLE_KNEEHEAD;
					SetPlayerFightingStyle(playerid, PlayerInfo[playerid][pFightingStyle]);
					GiveMoney(playerid, -50000);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have learned a new fighting style.");
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 4) //grabkick
			{
				if(PlayerInfo[playerid][pCash] > 49999)
				{
					PlayerInfo[playerid][pFightingStyle] = FIGHT_STYLE_GRABKICK;
					SetPlayerFightingStyle(playerid, PlayerInfo[playerid][pFightingStyle]);
					GiveMoney(playerid, -50000);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have learned a new fighting style.");
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 5) //elbow
			{
				if(PlayerInfo[playerid][pCash] > 49999)
				{
					PlayerInfo[playerid][pFightingStyle] = FIGHT_STYLE_ELBOW;
					SetPlayerFightingStyle(playerid, PlayerInfo[playerid][pFightingStyle]);
					GiveMoney(playerid, -50000);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have learned a new fighting style.");
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
		}
		return 1;
	}
	if(dialogid == DIALOG_BUYMEAL) //Cafeteria // COMMENTED
	{
		if(response)
		{
			new Float:playerHP;
			GetPlayerHealth(playerid, playerHP);
			if(listitem == 0) //Bread
			{
				if(PlayerInfo[playerid][pCash] > 2)
				{
					GiveMoney(playerid, -2);
					if(playerHP > 98)
					{
						SetPlayerHealth(playerid, 100.0);
					}
					else
					{
						SetPlayerHealth(playerid, playerHP + 2.0);
					}
					SendClientMessage(playerid, COLOR_GRAD4, "Bread purchased.");
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					return 1;
				}
			}
			if(listitem == 1) //Apple
			{
				if(PlayerInfo[playerid][pCash] > 3)
				{
					GiveMoney(playerid, -3);
					if(playerHP > 97)
					{
						SetPlayerHealth(playerid, 100.0);
					}
					else
					{
						SetPlayerHealth(playerid, playerHP + 3.0);
					}
					SendClientMessage(playerid, COLOR_GRAD4, "Apple purchased.");
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					return 1;
				}
			}
			if(listitem == 2) //Cok O Pops
			{
				if(PlayerInfo[playerid][pCash] > 5)
				{
					GiveMoney(playerid, -5);
					if(playerHP > 95)
					{
						SetPlayerHealth(playerid, 100.0);
					}
					else
					{
						SetPlayerHealth(playerid, playerHP + 5.0);
					}
					SendClientMessage(playerid, COLOR_GRAD4, "Cok O Pops purchased.");
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					return 1;
				}
			}
			if(listitem == 3) //Mashed Potatoes
			{
				if(PlayerInfo[playerid][pCash] > 5)
				{
					GiveMoney(playerid, -5);
					if(playerHP > 95)
					{
						SetPlayerHealth(playerid, 100.0);
					}
					else
					{
						SetPlayerHealth(playerid, playerHP + 5.0);
					}
					SendClientMessage(playerid, COLOR_GRAD4, "Mashed Potatoes purchased.");
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					return 1;
				}
			}
			if(listitem == 4) //Carrot
			{
				if(PlayerInfo[playerid][pCash] > 1)
				{
					GiveMoney(playerid, -1);
					if(playerHP > 99)
					{
						SetPlayerHealth(playerid, 100.0);
					}
					else
					{
						SetPlayerHealth(playerid, playerHP + 1.0);
					}
					SendClientMessage(playerid, COLOR_GRAD4, "Carrot purchased.");
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					return 1;
				}
			}
			if(listitem == 5) //Green Beans
			{
				if(PlayerInfo[playerid][pCash] > 4)
				{
					GiveMoney(playerid, -4);
					if(playerHP > 96)
					{
						SetPlayerHealth(playerid, 100.0);
					}
					else
					{
						SetPlayerHealth(playerid, playerHP + 4.0);
					}
					SendClientMessage(playerid, COLOR_GRAD4, "Green Beans purchased.");
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					return 1;
				}
			}
			if(listitem == 6) //Pork Roll
			{
				if(PlayerInfo[playerid][pCash] > 5)
				{
					GiveMoney(playerid, -5);
					if(playerHP > 95)
					{
						SetPlayerHealth(playerid, 100.0);
					}
					else
					{
						SetPlayerHealth(playerid, playerHP + 5.0);
					}
					SendClientMessage(playerid, COLOR_GRAD4, "Pork Roll purchased.");
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					return 1;
				}
			}
			if(listitem == 7) //Sausage
			{
				if(PlayerInfo[playerid][pCash] > 7)
				{
					GiveMoney(playerid, -7);
					if(playerHP > 93)
					{
						SetPlayerHealth(playerid, 100.0);
					}
					else
					{
						SetPlayerHealth(playerid, playerHP + 7.0);
					}
					SendClientMessage(playerid, COLOR_GRAD4, "Sausage purchased.");
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					return 1;
				}
			}
			if(listitem == 8) //Chicken
			{
				if(PlayerInfo[playerid][pCash] > 20)
				{
					GiveMoney(playerid, -20);
					if(playerHP > 80)
					{
						SetPlayerHealth(playerid, 100.0);
					}
					else
					{
						SetPlayerHealth(playerid, playerHP + 20.0);
					}
					SendClientMessage(playerid, COLOR_GRAD4, "Chicken purchased.");
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					return 1;
				}
			}
			ApplyAnimationEx(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		}
		return 1;
	}
	if(dialogid == DIALOG_PG_MAIN) // DoC lockers
	{
		if(response)
		{
			if(listitem == 0) // Duty
			{
				if(PlayerInfo[playerid][pOnDuty] == 0)
				{
					format(str, sizeof(str), "* %s takes their bade and equipment from the locker.", PlayerICName(playerid));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					
					if(PlayerInfo[playerid][pMember] == 11) FakeFaction[playerid] = 5;

					SetPlayerColor(playerid, COLOR_DOC_NAMETAG);
					GivePlayerGun(playerid, WEAPON_NITESTICK);
					GivePlayerGun(playerid, WEAPON_SPRAYCAN);
					SetPlayerArmour(playerid, 100.0);
					PlayerInfo[playerid][pOnDuty] = 1;
					PlayerInfo[playerid][pClothes] = 0;
				}
				else if(PlayerInfo[playerid][pOnDuty] == 1)
				{
					format(str, sizeof(str), "* %s places their badge and equipment back into thes locker.", PlayerICName(playerid));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					
					SetPlayerColor(playerid, TCOLOR_WHITE);
					
					ResetPlayerWeapons(playerid);
					ClearGuns(playerid);
					PlayerInfo[playerid][pGun1] = 0;
					PlayerInfo[playerid][pGun3] = 0;
					PlayerInfo[playerid][pGun5] = 0;

					SetPlayerArmour(playerid, 0.0);

					PlayerInfo[playerid][pClothes] = 0;
					PlayerInfo[playerid][pOnDuty] = 0;

					// Set off-duty skin
					new undercoverskins[10];
					undercoverskins[0] = 17; undercoverskins[5] = 303;
					undercoverskins[1] = 185; undercoverskins[6] = 304;
					undercoverskins[2] = 46; undercoverskins[7] = 305;
					undercoverskins[3] = 73; undercoverskins[8] = 2;
					undercoverskins[4] = 72; undercoverskins[9] = 7;
					new randomnumber = random(sizeof(undercoverskins));
					SetPlayerSkin(playerid, undercoverskins[randomnumber]);
					PlayerInfo[playerid][pModel] = undercoverskins[randomnumber];
				}
			}
			if(listitem == 1) // Equipment
			{
				ShowPlayerDialog(playerid, DIALOG_PG_WEAPONRY, DIALOG_STYLE_TABLIST_HEADERS, "Equipment", "Item\tPrice\nPortable MDC\tNo cost\nMace\t$20\nNight Stick\t$20\nDeagle\t$500\nShotgun\t$200\nMP5\t$400\nRifle\t$1,000\nM4\t$4,000\nFirst Aid Kit\tNo cost\nKevlar vest\t$1,000\nRubber Bullets\t$100", "Select", "Close");
			}
			if(listitem == 2) // Clearing
			{
				SendClientMessage(playerid, COLOR_YELLOW, "Please type the name of the player you want to clear.");
				SAPDClearing[playerid] = 1;
			}
			if(listitem == 3) // Uniform
			{
				ShowPlayerDialog(playerid, DIALOG_PG_UNIFORM, DIALOG_STYLE_LIST, "Uniform", "Male uniform 1\nMale uniform 2\nMale uniform 3\nMale uniform 4\nMale uniform 5\nFemale uniform", "Select", "Close");
			}
			if(listitem == 4) // SWAT
			{
				if(PlayerInfo[playerid][pRank] < 2)
					return SendClientMessage(playerid, COLOR_GREY, "This can only be used by R2+.");

				SetPlayerSkin(playerid, 285);
				SetPlayerArmour(playerid, 100);
			}
		}
		return 1;
	}
	if(dialogid == DIALOG_PG_UNIFORM) { // DoC uniforms
		if(!response)
			return 1;

		switch(listitem) {
			case 0: SetPlayerSkin(playerid, 311);
			case 1: SetPlayerSkin(playerid, 310);
			case 2: SetPlayerSkin(playerid, 302);
			case 3: SetPlayerSkin(playerid, 282);
			case 4: SetPlayerSkin(playerid, 283);
			case 5: SetPlayerSkin(playerid, 309);
		}

		PlayerInfo[playerid][pModel] = GetPlayerSkin(playerid);

		cmd_ame(playerid, "puts on their uniform.");
	}
	if(dialogid == DIALOG_PG_WEAPONRY) // DoC equipment
	{ 
		if(response)
		{
			if(listitem == 0) // Portable MDC
			{
				PlayerInfo[playerid][pPortMDC] = 1;
				SendClientMessage(playerid, COLOR_ORANGE, "You take a portable MDC unit from the locker.");
				SendClientMessage(playerid, COLOR_ORANGE, "(( This will be lost upon death. ))");
			}
			if(listitem == 1) // Mace
			{
				if(PlayerInfo[playerid][pCash] < 20)
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
	
				GiveMoney(playerid, -20);
				GivePlayerGun(playerid, 41);
			}
			if(listitem == 2) // Night Stick
			{
				if(PlayerInfo[playerid][pCash] < 20)
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
	
				GiveMoney(playerid, -20);
				GivePlayerGun(playerid, 3);
			}
			if(listitem == 3) // Deagle
			{
				if(PlayerInfo[playerid][pCash] < 500)
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");

				GiveMoney(playerid, -500);
				GivePlayerGun(playerid, 24);
			}
			if(listitem == 4) // Shotgun
			{
				if(PlayerInfo[playerid][pCash] < 200)
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
	
				GiveMoney(playerid, -200);
				GivePlayerGun(playerid, 25);
			}
			if(listitem == 5) // MP5
			{
				if(PlayerInfo[playerid][pCash] < 400)
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");

				GiveMoney(playerid, -400);
				GivePlayerGun(playerid, 29);
			}
			if(listitem == 6) // Rifle
			{
				if(PlayerInfo[playerid][pCash] < 1000)
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				
				GiveMoney(playerid, -1000);
				GivePlayerGun(playerid, 33);
			}
			if(listitem == 7) // M4
			{
				if(PlayerInfo[playerid][pCash] < 4000)
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");

				GiveMoney(playerid, -4000);
				GivePlayerGun(playerid, 31);
			} 
			if(listitem == 8) SetPlayerHealth(playerid, 100);
			if(listitem == 9) { // Kevlar
				if(PlayerInfo[playerid][pCash] < 1000)
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");

				GiveMoney(playerid, -1000);
				SetPlayerArmour(playerid, 100);
			}
			if(listitem == 9) // Rubber bullets
			{
				if(PlayerInfo[playerid][pCash] < 100)
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");

				hasRubberBullets[playerid] = 1;
				loadedRubberBullets[playerid] = false;
				SendClientMessage(playerid, COLOR_YELLOW, "To use your rubber bullets, use \"/loadrubber\", this only works with a Country Rifle.");
				GiveMoney(playerid, -100);
			}
		}
		return 1;
	}
	if(dialogid == DIALOG_SANG_MAIN) // SANG lockers
	{
		if(response)
		{
			if(listitem == 0) // Duty
			{
				if(PlayerInfo[playerid][pOnDuty] == 0)
				{
					format(str, sizeof(str), "* %s takes their badge and equipment from the locker.", PlayerICName(playerid));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					
					if(PlayerInfo[playerid][pMember] == 11) FakeFaction[playerid] = FACTION_SANG;

					SetPlayerColor(playerid, COLOR_SANG);
					GivePlayerGun(playerid, WEAPON_NITESTICK);
					GivePlayerGun(playerid, WEAPON_SPRAYCAN);
					SetPlayerArmour(playerid, 100.0);
					PlayerInfo[playerid][pOnDuty] = 1;
					PlayerInfo[playerid][pClothes] = 0;

					if(PlayerInfo[playerid][pGroup] == GROUP_S9 && PlayerInfo[playerid][pMember] != FACTION_SANG)
					{
						SetPlayerSkin(playerid, 287);
						PlayerInfo[playerid][pModel] = 287;

						PlayerInfo[playerid][pOnDuty] = 1;
						PlayerInfo[playerid][pMember] = FACTION_SANG;
						PlayerInfo[playerid][pRank] = 1;

						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have been given a badge automatically (SANG Corporal)");
					}
				}
				else if(PlayerInfo[playerid][pOnDuty] == 1)
				{
					format(str, sizeof(str), "* %s places their badge and equipment back into thes locker.", PlayerICName(playerid));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					
					SetPlayerColor(playerid, TCOLOR_WHITE);
					
					ResetPlayerWeapons(playerid);
					ClearGuns(playerid);

					SetPlayerArmour(playerid, 0.0);

					PlayerInfo[playerid][pClothes] = 0;
					PlayerInfo[playerid][pOnDuty] = 0;
				}
			}
			if(listitem == 1) // Equipment
			{
				ShowPlayerDialog(playerid, DIALOG_SANG_WEAPONRY, DIALOG_STYLE_TABLIST_HEADERS, "Equipment", "Item\tPrice\nPortable MDC\tNo cost\nMace\t$20\nNight Stick\t$20\nDeagle\t$500\nShotgun\t$200\nMP5\t$400\nRifle\t$1,000\nM4\t$6,000\nSpas-12\t$15,000\nFirst Aid Kit\tNo cost\nKevlar vest\t$1,000\nRubber Bullets\t$100\nSniper Rifle\t$12,000", "Select", "Close");
			}
			if(listitem == 2) // Clearing
			{
				SendClientMessage(playerid, COLOR_YELLOW, "Please type the name of the player you want to clear.");
				SAPDClearing[playerid] = 1;
			}
			if(listitem == 3) // Uniform
			{
				// Shows faction skin selector
				ShowSkinShop(playerid, false, true, false, false, true);
			}
			if(listitem == 4) // Shows civilian skin selector
			{
				ShowSkinShop(playerid, true, false, false, false, false);
				return 1;
			}
		}
		return 1;
	}
	/*if(dialogid == DIALOG_SANG_UNIFORM) { // SANG uniforms
		if(!response)
			return 1;

		switch(listitem) {
			case 0: SetPlayerSkin(playerid, 287);
			case 1: SetPlayerSkin(playerid, 179);
			case 2: SetPlayerSkin(playerid, 191);
			case 3: SetPlayerSkin(playerid, 73);
			case 4: SetPlayerSkin(playerid, 46);
		}

		PlayerInfo[playerid][pModel] = GetPlayerSkin(playerid);

		cmd_ame(playerid, "puts on their uniform.");
	}*/
	if(dialogid == DIALOG_SANG_WEAPONRY) // DoC equipment
	{ 
		if(response)
		{
			if(listitem == 0) // Portable MDC
			{
				PlayerInfo[playerid][pPortMDC] = 1;
				SendClientMessage(playerid, COLOR_ORANGE, "You take a portable MDC unit from the locker.");
				SendClientMessage(playerid, COLOR_ORANGE, "(( This will be lost upon death. ))");
			}
			if(listitem == 1) // Mace
			{
				if(PlayerInfo[playerid][pCash] < 20)
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
	
				GiveMoney(playerid, -20);
				GivePlayerGun(playerid, 41);
			}
			if(listitem == 2) // Night Stick
			{
				if(PlayerInfo[playerid][pCash] < 20)
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
	
				GiveMoney(playerid, -20);
				GivePlayerGun(playerid, 3);
			}
			if(listitem == 3) // Deagle
			{
				if(PlayerInfo[playerid][pCash] < 500)
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");

				GiveMoney(playerid, -500);
				GivePlayerGun(playerid, 24);
			}
			if(listitem == 4) // Shotgun
			{
				if(PlayerInfo[playerid][pCash] < 200)
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
	
				GiveMoney(playerid, -200);
				GivePlayerGun(playerid, 25);
			}
			if(listitem == 5) // MP5
			{
				if(PlayerInfo[playerid][pCash] < 400)
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");

				GiveMoney(playerid, -400);
				GivePlayerGun(playerid, 29);
			}
			if(listitem == 6) // Rifle
			{
				if(PlayerInfo[playerid][pCash] < 1000)
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				
				GiveMoney(playerid, -1000);
				GivePlayerGun(playerid, 33);
			}
			if(listitem == 7) // M4
			{
				if(PlayerInfo[playerid][pCash] < 4000)
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");

				GiveMoney(playerid, -4000);
				GivePlayerGun(playerid, 31);
			} 
			if(listitem == 8) // Spas-12
			{
				if(PlayerInfo[playerid][pCash] < 15000) // 15k
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");

				GiveMoney(playerid, -15000);
				GivePlayerGun(playerid, 27);
			}
			if(listitem == 9) SetPlayerHealth(playerid, 100);
			if(listitem == 10) { // Kevlar
				if(PlayerInfo[playerid][pCash] < 1000)
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");

				GiveMoney(playerid, -1000);
				SetPlayerArmour(playerid, 100);
			}
			if(listitem == 11) // Rubber bullets
			{
				if(PlayerInfo[playerid][pCash] < 100)
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");

				hasRubberBullets[playerid] = 1;
				loadedRubberBullets[playerid] = false;
				SendClientMessage(playerid, COLOR_YELLOW, "To use your rubber bullets, use \"/loadrubber\", this only works with a Country Rifle.");
				GiveMoney(playerid, -100);
			}
			if(listitem == 12) // sniper rifle
			{
				if(PlayerInfo[playerid][pCash] < 12000) // 12k
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");

				GiveMoney(playerid, -12000);
				GivePlayerGun(playerid, 34);
			}	
		}
		return 1;
	}
	if(dialogid == DIALOG_SAS_MAIN) //SAS
	{
		if(response)
		{
			if(listitem == 0)
			{
				if(PlayerInfo[playerid][pOnDuty] == 0) //if player is off duty, set them them on
				{
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);

					format(str, sizeof(str), "* %s %s takes a Badge from their locker.", GetPlayerRank(playerid), sendername);
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

					if(PlayerInfo[playerid][pMember] == 15 || PlayerInfo[playerid][pLeader] == 15)
					{
						SetPlayerColor(playerid, TCOLOR_SAS);
					}
					PlayerInfo[playerid][pOnDuty] = 1;
				}
				else if(PlayerInfo[playerid][pOnDuty] == 1) //if player is on duty, set them off
				{
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					if(PlayerInfo[playerid][pMember] == 15)
					{
						format(str, sizeof(str), "* %s %s places a Badge in their locker.", GetPlayerRank(playerid), sendername);
					}
					else
					{
						format(str, sizeof(str), "* %s %s places a Badge in their locker.", GetPlayerRank(playerid), sendername);
					}
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					SetPlayerColor(playerid, TCOLOR_WHITE);
					PlayerInfo[playerid][pOnDuty] = 0;
					TogglePlayerControllable(playerid, true);
				}
				return 1;
			}
			if(listitem == 1)
			{
				SendClientMessage(playerid, COLOR_GREY, "This feature is deprecated.");
				return 1;
			}
			if(listitem == 2)
			{
				if(PlayerInfo[playerid][pCash] < 5000)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					GiveMoney(playerid, -5000);
					SetPlayerHealth(playerid, 100);
					SetPlayerArmour(playerid, 100);
					SendClientMessage(playerid, COLOR_GREY, "Health and Kevlar replenished.");
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 3)
			{
				if(PlayerInfo[playerid][pCash] < 5000)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					GiveMoney(playerid, -1000);
					GivePlayerGun(playerid, 22);
					SendClientMessage(playerid, COLOR_GREY, "You've purchased a Colt45.");
					TogglePlayerControllable(playerid, true);
				}
			}
		}
		return 1;
	}
	if(dialogid == DIALOG_S9_MAIN) // Sector 9 locker
	{
		if(response)
		{
			if(listitem == 0) // Armory
				return showS9Dialog(playerid);
			if(listitem == 1) // Clothes
			{
				return SendClientMessage(playerid, COLOR_LIGHTRED, "Feature is disabled.");
				/* DestroySelectionMenu(playerid);
				SetPVarInt(playerid, "skinc_active", 1);
				CreateSelectionMenu(playerid);
				SelectTextDraw(playerid, 0xACCBF1FF); */
			}
			if(listitem == 2) // Restricted Clothes
			{
				new diastr[2000];
				strcat(diastr, "Agent\t(286)\t\n");
				strcat(diastr, "S.W.A.T Special Forces\t(285)\t\n");
				strcat(diastr, "San Andreas Army\t(287)\t\n");
				strcat(diastr, "Desert Sherrif (Chief)\t(288)\t\n");
				strcat(diastr, "County Sheriff\t(283)\t\n");
				strcat(diastr, "Las Venturas Police Officer\t(282)\t\n");
				strcat(diastr, "San Fierro Police Officer\t(281)\t\n");
				strcat(diastr, "Los Santos Police Officer\t(280)\t\n");
				strcat(diastr, "Officer Tenpenny\t(265)\t\n");
				strcat(diastr, "Officer Pulaski\t(266)\t\n");
				strcat(diastr, "Officer Hernandez\t(267)\t\n");
				strcat(diastr, "White MIB Agent\t(165)\t\n");
				strcat(diastr, "Black MIB Agent\t(166)\t\n");
				strcat(diastr, "Black (White Shirt) Medic\t(274)\t\n");
				strcat(diastr, "White (Blue Shirt) Medic\t(275)\t\n");
				strcat(diastr, "White (Green Shirt) Medic\t(276)\t\n");
				strcat(diastr, "Firefighter (White)\t(277)\t\n");
				strcat(diastr, "Firefighter (Black)\t(278)\t\n");
				strcat(diastr, "San Fierro Figherfighter\t(279)\t\n");
				strcat(diastr, "LSPD Motorbike Cop\t(284)\t");
				ShowPlayerDialog(playerid, DIALOG_S9_RESTRICTED, DIALOG_STYLE_TABLIST, "S9 Skins", diastr, "Select", "Back");
			}
			if(listitem == 3) // Badges
			{
				new diastr[2000];
				strcat(diastr, "Los Santos Police Department\t(1)\t{23c62e}(Enabled){ffffff}\n");
				strcat(diastr, "Federal Bureau of Investigation\t(2)\t{23c62e}(Disabled){ffffff}\n");
				strcat(diastr, "San Andreas Sheriffs Department\t(3)\t{ff4949}(Disabled){ffffff}\n");
				strcat(diastr, "San Andreas Fire & Medical Department\t(4)\t{23c62e}(Enabled){ffffff}\n");
				strcat(diastr, "Department of Corrections\t(5)\t{23c62e}(Enabled){ffffff}\n");
				strcat(diastr, "Government\t(6)\t{23c62e}(Enabled){ffffff}\n");
				strcat(diastr, "Judicial Department\t(7)\t{ff4949}(Disabled){ffffff}\n");
				strcat(diastr, "San Andreas National Guard\t(14)\t{23c62e}(Enabled){ffffff}\n");
				strcat(diastr, "Civilian\t(0)\t");
				ShowPlayerDialog(playerid, DIALOG_S9_BADGES, DIALOG_STYLE_TABLIST, "Faction Badges", diastr, "Select", "Back");
			}
		}
		return 1;
	}
	if(dialogid == DIALOG_S9_RESTRICTED)
	{
		if(!response)
		{
			new bool: Admin;
			foreach( new i: Player )
			{
				if(PlayerInfo[i][pAdmin] > 0)
				{
					Admin = true;
					break;
				}
			}
			if(Admin) ShowPlayerDialog(playerid, DIALOG_S9_MAIN, DIALOG_STYLE_LIST, GetPlayerFactionName(playerid), "Namechange ({33AA33}Available{FFFFFF})\nArmory\nWardrobe\nWardrobe ({ff7b00}Restricted{FFFFFF})\nLaw Enforcement Badges", "Select", "Cancel");
			else ShowPlayerDialog(playerid, DIALOG_S9_MAIN, DIALOG_STYLE_LIST, GetPlayerFactionName(playerid), "Namechange ({FF0000}Not available{FFFFFF})\nArmory\nWardrobe\nWardrobe ({ff7b00}Restricted{FFFFFF})\nLaw Enforcement Badges", "Select", "Cancel");
		}
		else
		{
			if(listitem == 0) return SetPlayerSkin(playerid, 286);//fbi
			if(listitem == 1) 
			{
				SetPlayerSkin(playerid, 285); //swat
				SetPlayerArmour(playerid, 100); // swat skins are granted 100 armor
			}
			if(listitem == 2) 
			{
				SetPlayerSkin(playerid, 287); //sang
				SetPlayerArmour(playerid, 100);
			}
			if(listitem == 3) return SetPlayerSkin(playerid, 288);//Chief
			if(listitem == 4) return SetPlayerSkin(playerid, 283);//County Sherrif
			if(listitem == 5) return SetPlayerSkin(playerid, 282);//lvpd cop
			if(listitem == 6) return SetPlayerSkin(playerid, 281);//sfpd cop
			if(listitem == 7) return SetPlayerSkin(playerid, 280);//lspd cop
			if(listitem == 8) return SetPlayerSkin(playerid, 265);//tenpenny
			if(listitem == 9) return SetPlayerSkin(playerid, 266);//pulaski
			if(listitem == 10) return SetPlayerSkin(playerid, 267);//hernandez
			if(listitem == 11) return SetPlayerSkin(playerid, 165);//white mib
			if(listitem == 12) return SetPlayerSkin(playerid, 166);//black mib
			if(listitem == 13) return SetPlayerSkin(playerid, 274);//Black (White Shirt) Medic
			if(listitem == 14) return SetPlayerSkin(playerid, 275);//White (Blue Shirt) Medic
			if(listitem == 15) return SetPlayerSkin(playerid, 267);//White (Green Shirt) Medic
			if(listitem == 16) return SetPlayerSkin(playerid, 277);//Firefighter (White)
			if(listitem == 17) return SetPlayerSkin(playerid, 278);//Firefighter (Black)
			if(listitem == 18) return SetPlayerSkin(playerid, 279);//San Fierro Figherfighter
			if(listitem == 19) return SetPlayerSkin(playerid, 284);//LSPD Motorbike Cop
			PlayerInfo[playerid][pClothes] = GetPlayerSkin(playerid);
		}
		return 1;
	}
	
	if(dialogid == DIALOG_S9_BADGES)
	{
		if(!response) return ShowS9Armory(playerid);
		ChosenFaction[playerid] = listitem + 1;
		if(listitem == 7) ChosenFaction[playerid] = 14; // SANG is ID 14, different from how this system works

		// If civillian
		if(listitem == 8) {
			SetPlayerColor(playerid, TCOLOR_WHITE);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* Tip: You can also use '/removebadge'.");
			ChosenFaction[playerid] = 0;
			PlayerInfo[playerid][pOnDuty] = 0;
			return 1;
		}

		// Disabled badges
		if(listitem == 6 || listitem == 2) {
			SendClientMessage(playerid, COLOR_GREY, "That badge is disabled.");
			return ShowS9Armory(playerid);
		}

		// Let the player know which faction he has selected.
		format(str, sizeof(str), "You have selected: %s", GetFactionName(ChosenFaction[playerid]));
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

		// Now to change the rank.
		new diastr[300];
		format(str, sizeof(str), "Change rank for %s.", GetFactionName(ChosenFaction[playerid]));
	
		// Just adds the rank name to the list item. (e.g. Rank 0 - Cadet)
		format(diastr, sizeof(diastr), "Rank 0 - %s\n", GetFactionRankName(ChosenFaction[playerid], 0));
		format(diastr, sizeof(diastr), "%sRank 1 - %s\n", diastr, GetFactionRankName(ChosenFaction[playerid], 1));
		format(diastr, sizeof(diastr), "%sRank 2 - %s\n", diastr, GetFactionRankName(ChosenFaction[playerid], 2));
		if(PlayerInfo[playerid][pGroupRank] >= GROUP_S9_RANKS-1) {
			format(diastr, sizeof(diastr), "%sRank 3 - %s\n", diastr, GetFactionRankName(ChosenFaction[playerid], 3));
			format(diastr, sizeof(diastr), "%sRank 4 - %s", diastr, GetFactionRankName(ChosenFaction[playerid], 4));
		} else {
			format(diastr, sizeof(diastr), "%sRank 3 - %s", diastr, GetFactionRankName(ChosenFaction[playerid], 3));
		}

		ShowPlayerDialog(playerid, DIALOG_S9_RANKS, DIALOG_STYLE_LIST, str, diastr, "Confirm", "Cancel");
		return 1;
	}
	if(dialogid == DIALOG_S9_RANKS) {
		if(!response) return ShowS9Armory(playerid);
		new skin;

		ChosenRank[playerid] = listitem;

		switch(ChosenFaction[playerid])
		{
			case FACTION_SAPD:
			{
				SetPlayerColor(playerid, TCOLOR_BLUE);

				// To change the player's skin.
				switch(ChosenRank[playerid])
				{
					case 0: skin = 71; // Cadet
					case 1: skin = 280; // Officer
					case 2: skin = 281; // Senior Officer
					case 3: skin = 282; // Sargent
				}
			}
			case FACTION_FBI:
			{
				SetPlayerColor(playerid, TCOLOR_NAVYBLUE);
				skin = 286; // FBI skin
			}
			case FACTION_FMD:
			{
				SetPlayerColor(playerid, TCOLOR_PARAMEDIC);
				switch(ChosenRank[playerid])
				{
					case 0: skin = 276; // Trainee
					case 1..3: skin = 275; // R1-R3
				}
			}
			case FACTION_ARMY:
			{
				SetPlayerColor(playerid, COLOR_DOC_NAMETAG);
				skin = 287; // Army skin
			}
			case 6: // Senate
			{
				SetPlayerColor(playerid, TCOLOR_SENATE);
				skin = 165; // White MIB
			}
			case FACTION_SANG:
			{
				SetPlayerColor(playerid, COLOR_SANG);
				skin = 287; // Army skin
			}
		}

		SetPlayerSkin(playerid, skin);
		PlayerInfo[playerid][pModel] = skin;

		PlayerInfo[playerid][pOnDuty] = 1;
		PlayerInfo[playerid][pMember] = ChosenFaction[playerid];
		PlayerInfo[playerid][pRank] = ChosenRank[playerid];

		format(str, sizeof(str), "You are now a %s %s.", GetFactionName(ChosenFaction[playerid]), GetFactionRankName(ChosenFaction[playerid], ChosenRank[playerid]));
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

		format(customRank[playerid], 24, "%s", GetFactionRankName(ChosenFaction[playerid], ChosenRank[playerid]));
		customRankName[playerid] = true;
		ChosenFaction[playerid] = 0;
		ChosenRank[playerid] = 0;

		return 1;
	}
	if(dialogid == DIALOG_S9_ARMORY)
	{
		if(!response) return ShowS9Armory(playerid);
		if(response)
		{
			if(!IsPlayerNearSector9HQ(playerid))
				return SendClientMessage(playerid, COLOR_GREY, "You are not at your headquarters.");

			new money = PlayerInfo[playerid][pCash];
			switch(listitem)
			{
				case 0: // Desert Eagle
				{
					if(money >= 2500) {
						GiveMoney(playerid, -2500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_DEAGLE);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have purchased a Desert Eagle for $2,500.");
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}
				}
				case 1: // SD Pistol
				{
					if(money >= 5000) {
						GiveMoney(playerid, -5000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_SILENCED);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have purchased an SD Pistol for $5,000.");
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}
				}
				case 2: // Break
				{
					showS9Dialog(playerid);
				}
				case 3: // M4
				{
					if(money >= 12500) {
						GiveMoney(playerid, -12500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_M4);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have purchased an M4 for $12,500.");
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}
				}
				case 4: // AK47
				{
					if(money >= 7500) {
						GiveMoney(playerid, -7500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_AK47);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have purchased an AK47 for $7,500.");
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}
				}
				case 5: // Break
				{
					showS9Dialog(playerid);
				}
				case 6: // Spas12
				{
					if(money >= 25000) {
						GiveMoney(playerid, -25000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_SHOTGSPA);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have purchased a combat shotgun for $25,000.");
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}
				}
				case 7: // Shotgun
				{
					if(money >= 2000) {
						GiveMoney(playerid, -2000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_SHOTGUN);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have purchased a shotgun for $2,000.");
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}
				}
				case 8: // Sawn off
				{
					if(money >= 7500) {
						GiveMoney(playerid, -7500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_SAWEDOFF);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have purchased a sawn-off shotgun for $7,500.");
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}
				}
				case 9: // Break
				{
					showS9Dialog(playerid);
				}
				case 10: // MP5
				{
					if(money >= 1000) {
						GiveMoney(playerid, -1000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_MP5);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have purchased an MP5 for $1,000.");
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}
				}
				case 11: // TEC9
				{
					if(money >= 2500) {
						GiveMoney(playerid, -2500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_TEC9);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have purchased a TEC9 for $2,500.");
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}
				}
				case 12: // Uzi
				{
					if(money >= 2500) {
						GiveMoney(playerid, -2500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_UZI);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have purchased an Uzi for $2,500.");
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}
				}
				case 13: // Break
				{
					showS9Dialog(playerid);
				}
				case 14: // Rifle
				{
					if(money >= 2500) {
						GiveMoney(playerid, -2500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_RIFLE);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have purchased a country rifle for $2,500.");
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}
				}
				case 15: // Sniper Rifle
				{
					if(money >= 15000) {
						GiveMoney(playerid, -15000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_SNIPER);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have purchased a sniper rifle for $15,000.");
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}
				}
				case 16: // Break
				{
					showS9Dialog(playerid);
				}
				case 17: // Flamethrower
				{
					if(money >= 20000) {
						GiveMoney(playerid, -20000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_FLAMETHROWER);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have purchased a flamethrower for $20,000.");
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}
				}
				case 18: // C4
				{
					if(money >= 10000) {
						GiveMoney(playerid, -25000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						PlayerInfo[playerid][pBombs] += 1;

						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have purchased a block of plastic explosive for $10,000.");
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}
				}
				case 19: // Break
				{
					showS9Dialog(playerid);
				}
				case 20: // Chainsaw
				{
					if(money >= 5000) {
						GiveMoney(playerid, -5000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_CHAINSAW);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have purchased a chainsaw for $5,000.");
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}
				}
				case 21: // Knife
				{
					if(money >= 1000) {
						GiveMoney(playerid, -1000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_KNIFE);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have purchased a knife for $1,000.");
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}
				}
				case 22: // Break
				{
					showS9Dialog(playerid);
				}
				case 23: // Tear Gas
				{
					if(money >= 2500) {
						GiveMoney(playerid, -2500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_TEARGAS);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have purchased two canisters of tear gas for $2,500.");
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}
				}
				case 24: // Grenades
				{
					SendClientMessage(playerid, COLOR_GREY, "Grenades have been disabled.");
					/*
					if(money >= 2500) {
						GiveMoney(playerid, -2500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_GRENADE);
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}*/
				}
				case 25: // Molotov Cocktail
				{
					if(money >= 2500) {
						GiveMoney(playerid, -2500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						GivePlayerGun(playerid, WEAPON_MOLTOV);
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}
				}
				case 26: // Break
				{
					showS9Dialog(playerid);
				}
				case 27: // Health and Armour
				{
					if(money >= 1000) {
						GiveMoney(playerid, -1000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						SetPlayerHealth(playerid, 100.0);
						SetPlayerArmour(playerid, 100.0);
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}
				}
				case 28: // Half Vest
				{
					if(money >= 500) {
						GiveMoney(playerid, -1000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						SetPlayerArmour(playerid, 50.0);
					} else {
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You can't afford that.");
					}
				}
				case 29: // Break
				{
					showS9Dialog(playerid);
				}
				case 30: // Jammer
				{
					if(money >= 1000)
					{
						GiveMoney(playerid, -1000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						PlayerInfo[playerid][pJammer] += 1;
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have purchased a signal jammer for $1,000.");
					}
					else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money for that.");
				}
				case 31: // Lock pick
				{
					if(money >= 5000)
					{
						if(PlayerInfo[playerid][pHasLockPick] == 1)
							return SendClientMessage(playerid, COLOR_GREY, "You already have a lock pick kit.");

						GiveMoney(playerid, -5000);
						PlayerInfo[playerid][pHasLockPick] = 1;

						SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have purchased a lock pick kit for $5,000.");
					}
					else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money for that.");
				}
			}
		}
	}
	if(dialogid == DIALOG_LEO_MAIN) //SAPD
	{
		if(response)
		{
			switch(listitem)
			{
				case 0:
				{
					if(PlayerInfo[playerid][pOnDuty] == 0) //if player is off duty, set them them on
					{
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);

						format(str, sizeof(str), "* %s %s retrieves his equipment from his locker and equips his badge & body camera.", GetPlayerRank(playerid), sendername);
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						if(PlayerInfo[playerid][pMember] == 1 || PlayerInfo[playerid][pLeader] == 1 || PlayerInfo[playerid][pMember] == 11)
						{
							if(PlayerInfo[playerid][pMember] == 11) FakeFaction[playerid] = 1;
							SetPlayerColor(playerid, TCOLOR_BLUE);
						}
						else if(PlayerInfo[playerid][pMember] == 2 || PlayerInfo[playerid][pLeader] == 2 || PlayerInfo[playerid][pMember] == 11)
						{
							if(PlayerInfo[playerid][pMember] == 11) FakeFaction[playerid] = 2;
							SetPlayerColor(playerid, TCOLOR_NAVYBLUE);
						}
						else if(PlayerInfo[playerid][pMember] == 3 || PlayerInfo[playerid][pLeader] == 3 || PlayerInfo[playerid][pMember] == 11)
						{
							if(PlayerInfo[playerid][pMember] == 11) FakeFaction[playerid] = 3;
							SetPlayerColor(playerid, TCOLOR_BEIGE);
						}
						GivePlayerGun(playerid, 24);
						GivePlayerGun(playerid, 41);
						GivePlayerGun(playerid, 3);
						SetPlayerArmour(playerid, 100.0);
						PlayerInfo[playerid][pOnDuty] = 1;
						TogglePlayerControllable(playerid, true);

						if(PlayerInfo[playerid][pGroup] == GROUP_S9 && PlayerInfo[playerid][pMember] != FACTION_SAPD && PlayerInfo[playerid][pMember] != FACTION_SASD && IsPlayerNearSAPDHQ(playerid))
						{
							SetPlayerSkin(playerid, 280);
							PlayerInfo[playerid][pModel] = 280;

							customRankName[playerid] = false;
							PlayerInfo[playerid][pOnDuty] = 1;
							PlayerInfo[playerid][pMember] = FACTION_SAPD;
							PlayerInfo[playerid][pRank] = 1;

							SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have been given a badge automatically (SAPD Officer)");
						}
						else if(PlayerInfo[playerid][pGroup] == GROUP_S9 && PlayerInfo[playerid][pMember] != FACTION_FBI && IsPlayerNearFBIHQ(playerid))
						{
							SetPlayerSkin(playerid, 286);
							PlayerInfo[playerid][pModel] = 286;

							customRankName[playerid] = false;
							PlayerInfo[playerid][pOnDuty] = 1;
							PlayerInfo[playerid][pMember] = FACTION_FBI;
							PlayerInfo[playerid][pRank] = 1;

							SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have been given a badge automatically (FBI Staff)");
						}
					}
					else if(PlayerInfo[playerid][pOnDuty] == 1) //if player is on duty, set them off
					{
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						if(PlayerInfo[playerid][pMember] == 2)
						{
							format(str, sizeof(str), "* %s %s places his equipment inside his locker and removes his badge & body camera.", GetPlayerRank(playerid), sendername);
						}
						else
						{
							format(str, sizeof(str), "* %s %s places his equipment inside his locker and removes his badge & body camera.", GetPlayerRank(playerid), sendername);
						}
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						SetPlayerColor(playerid, TCOLOR_WHITE);
						PlayerInfo[playerid][pGun2] = 0;
						PlayerInfo[playerid][pGun9] = 0;
						PlayerInfo[playerid][pGun1] = 0;
						ResetPlayerWeapons(playerid);
						ClearGuns(playerid);
						SetPlayerArmour(playerid, 0.0);
						PlayerInfo[playerid][pOnDuty] = 0;
						TogglePlayerControllable(playerid, true);
						PlayerInfo[playerid][pHandtazer] = 0;
						PlayerInfo[playerid][ptazerGun] = 0;
						GivePlayerGun(playerid, 24);
						if(PlayerInfo[playerid][pMember] == 5 || PlayerInfo[playerid][pLeader] == 5)
						{
							GivePlayerGun(playerid, 29);
						}
					}
					return 1;
				}
				case 1:
				{
					// Shows civilian skin selector
					ShowSkinShop(playerid, true, false, false, false, false);
					return 1;
				}
				case 2:
				{
					// Shows faction skin selector
					ShowSkinShop(playerid, false, true, false, false, true);
					return 1;
				}
				case 3:
				{
					showSAPDDialog(playerid);
				}
				case 4:
				{
					if(PlayerInfo[playerid][pMember] == FACTION_SAPD && PlayerInfo[playerid][pRank] < 2)
					{
						SendClientMessage(playerid, COLOR_GREY, "Only R2s+ are able to use this.");
						TogglePlayerControllable(playerid, true);
						return 1;
					}
				
					ShowPlayerDialog(playerid, DIALOG_SAPD_SWAT, DIALOG_STYLE_LIST, "SWAT", "Weapons\nAccessories\nSWAT Uniform", "Select", "Back");
				}
				case 5:
				{
					SendClientMessage(playerid, COLOR_YELLOW, "Please type the name of the player you want to clear.");
					SAPDClearing[playerid] = 1;
				}
				case 6:
				{
					SendClientMessage(playerid, COLOR_YELLOW, "Please type the name of the player you want to release.");
					SAPDClearing[playerid] = 2;
				}
				case 7: // Namechange
				{
					new bool: Admin;
					foreach( new i: Player )
					{
						if(PlayerInfo[i][pAdmin] > 1)
						{
							Admin = true;
							break;
						}
					}
					if(Admin) ShowPlayerDialog(playerid, DIALOG_FREE_NAMECHANGE, DIALOG_STYLE_INPUT, "Namechange", "Insert a new name no longer than 24 characters following the roleplay name format:\nRoleplay name format: Firstname_Lastname", "Request", "Back");
					else ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Namechange", "There are no administrators online that can handle your namechange request!", "Okay", "");
				}
			}
		}
		return 1;
	}
	if(dialogid == DIALOG_FREE_NAMECHANGE)
	{
		if(response)
		{
			if(strlen(inputtext) < 1) return ShowPlayerDialog(playerid, DIALOG_FREE_NAMECHANGE, DIALOG_STYLE_INPUT, "Namechange", "Insert a new name no longer than 24 characters following the roleplay name format:\nRoleplay name format: Firstname_Lastname\n{FF0000}This name can not be empty.{FFFFFF}", "Request", "Back");
			if(strlen(inputtext) > MAX_PLAYER_NAME) return ShowPlayerDialog(playerid, DIALOG_FREE_NAMECHANGE, DIALOG_STYLE_INPUT, "Namechange", "Insert a new name no longer than 24 characters following the roleplay name format:\nRoleplay name format: Firstname_Lastname\n{FF0000}This name exceeds more than twenty four characters.{FFFFFF}", "Request", "Back");
			if(!IsValidName(inputtext) && PlayerInfo[playerid][pAdmin] == 0) return ShowPlayerDialog(playerid, DIALOG_FREE_NAMECHANGE, DIALOG_STYLE_INPUT, "Namechange", "Insert a new name no longer than 24 characters following the roleplay name format:\nRoleplay name format: Firstname_Lastname\n{FF0000}This name does not follow the roleplay name format: Firstname_Lastname{FFFFFF}", "Request", "Back");
			new bool: Admin;
			foreach( new i: Player )
			{
				if(PlayerInfo[i][pAdmin] >= 1)
				{
					Admin = true;
					break;
				}
			}
			if(Admin)
			{
				format(str, sizeof(str), "{FFFFFF}Are you sure you want to change your name to %s for free?", inputtext);
				ShowPlayerDialog(playerid, DIALOG_FREE_NAMECHANGE1, DIALOG_STYLE_MSGBOX, "Namechange", str, "Confirm", "Back");
				strmid(NameChangeRequest[playerid], inputtext, 0, strlen(inputtext), 255);
			}
			else
			{
				ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Namechange", "There are no administrators online that can handle your namechange request anymore!", "Okay", "");
				strmid(NameChangeRequest[playerid], "0", 0, 255);
			}
		}
		else
		{
			if(IsPlayerInRangeOfPoint(playerid, 3, 226.7504, 122.7705, 999.0406))
			{
				new bool: Admin;
				foreach( new i: Player )
				{
					if(PlayerInfo[i][pAdmin] > 0)
					{
						Admin = true;
						break;
					}
				}
				if(Admin) ShowPlayerDialog(playerid, DIALOG_LEO_MAIN, DIALOG_STYLE_LIST, GetPlayerFactionName(playerid), "Duty\nUndercover\nChange Uniform\nArmory\nSWAT\nClear Suspect\nRelease Suspect\nNamechange ({33AA33}Available{FFFFFF})", "Select", "Cancel");
				else ShowPlayerDialog(playerid, DIALOG_LEO_MAIN, DIALOG_STYLE_LIST, GetPlayerFactionName(playerid), "Duty\nUndercover\nChange Uniform\nArmory\nSWAT\nClear Suspect\nRelease Suspect\nNamechange ({FF0000}Not available{FFFFFF})", "Select", "Cancel");
			}
		}
	}
	if(dialogid == DIALOG_FREE_NAMECHANGE1)
	{
		if(response)
		{
			new query[128];
			mysql_format(sqlGameConnection, query, sizeof(query), "SELECT `Name` FROM `players` WHERE `Name` = '%e' LIMIT 1", NameChangeRequest[playerid]);
			mysql_pquery(sqlGameConnection, query, "OnNamechange", "is", playerid, NameChangeRequest[playerid]);
			format(str, sizeof(str), "Your free namechange request to %s has been dispatched to Administrators.", NameChangeRequest[playerid]);
			ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Namechange", str, "Okay", "");
		}
		else
		{
			new bool: Admin;
			foreach( new i: Player )
			{
				if(PlayerInfo[i][pAdmin] > 1)
				{
					Admin = true;
					break;
				}
			}
			if(Admin) ShowPlayerDialog(playerid, DIALOG_FREE_NAMECHANGE, DIALOG_STYLE_INPUT, "Namechange", "Insert a new name no longer than 24 characters following the roleplay name format:\nRoleplay name format: Firstname_Lastname", "Request", "Back");
			else ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Namechange", "There are no administrators online that can handle your namechange request!", "Okay", "");
		}
	}
	if(dialogid == DIALOG_SAPD_WEAPONRY)
	{
		if(response)
		{
			if(listitem == 0) // Portable MDC
			{
				PlayerInfo[playerid][pPortMDC] = 1;
				SendClientMessage(playerid, COLOR_ORANGE, "You take a portable MDC unit from the locker.");
				SendClientMessage(playerid, COLOR_ORANGE, "(( This will be lost upon death. ))");
			}
			if(listitem == 1) // Mace
			{
				if(PlayerInfo[playerid][pCash] < 20)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					GiveMoney(playerid, -20);
					GivePlayerGun(playerid, 41);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 2) // Night Stick
			{
				if(PlayerInfo[playerid][pCash] < 20)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					GiveMoney(playerid, -20);
					GivePlayerGun(playerid, 3);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 3) // Deagle
			{
				if(PlayerInfo[playerid][pCash] < 500)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					GiveMoney(playerid, -500);
					GivePlayerGun(playerid, 24);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 4) //Shotgun
			{
				if(PlayerInfo[playerid][pCash] < 200)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					GiveMoney(playerid, -200);
					GivePlayerGun(playerid, 25);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 5) //MP5
			{
				if(PlayerInfo[playerid][pCash] < 400)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					GiveMoney(playerid, -400);
					GivePlayerGun(playerid, 29);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 6) //Rifle
			{
				if(PlayerInfo[playerid][pCash] < 1000)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					GiveMoney(playerid, -1000);
					GivePlayerGun(playerid, 33);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 7) //M4
			{
				if(PlayerInfo[playerid][pCash] < 6000)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					GiveMoney(playerid, -6000);
					GivePlayerGun(playerid, 31);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 8) //first aid
			{
				if(PlayerInfo[playerid][pCash] < 200)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					format(str, sizeof(str), "* %s %s has used a first aid kit.", GetPlayerRank(playerid), PlayerICName(playerid));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					GiveMoney(playerid, -200);
					SetPlayerHealth(playerid, 100.0);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 9) //kevlar
			{
				if(PlayerInfo[playerid][pCash] < 1000)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					SetPlayerArmour(playerid, 100.0);
					GiveMoney(playerid, -1000);
				}
			}
			if(listitem == 10) //rubber bullets
			{
				if(PlayerInfo[playerid][pRank] < 2 && IsACop(playerid)) return SendClientMessage(playerid, COLOR_GREY, "Only rank 2+ can use this.");
				if(PlayerInfo[playerid][pRank] < 1 && IsAFBI(playerid)) return SendClientMessage(playerid, COLOR_GREY, "Only rank 1+ can use this.");
				if(PlayerInfo[playerid][pCash] < 100)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					hasRubberBullets[playerid] = 1;
					loadedRubberBullets[playerid] = false;
					SendClientMessage(playerid, COLOR_YELLOW, "To use your rubber bullets, use \"/loadrubber\", this only works with a Country Rifle.");
					GiveMoney(playerid, -100);
				}
			}
			if(listitem == 11) //C2 explosives
			{
				if(PlayerInfo[playerid][pMember] == FACTION_SAPD && PlayerInfo[playerid][pRank] < 3) return SendClientMessage( playerid, COLOR_GREY, "You do not have access to this equipment ((R3+))." );
				if( PlayerInfo[playerid][pCash] < 80000 ) {
					SendClientMessage( playerid, COLOR_GREY, "You can't afford that." );
					TogglePlayerControllable( playerid, true );
					return 1;
				}
				else {
					GiveMoney( playerid, -80000 );
					PlayerInfo[playerid][pHasCBomb] = 1;
					SendClientMessage( playerid, COLOR_YELLOW, "To deploy this explosive use \"/deployexplosive\", this will allow you to breach houses, doors and gates." );
				}
			}
			if(listitem == 12) // Signal Jammer
			{
				if(PlayerInfo[playerid][pMember] != FACTION_FBI)
					return AdmErrorMsg;
				
				TogglePlayerControllable(playerid, true);
				if(PlayerInfo[playerid][pCash] < 1000) return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				GiveMoney(playerid, -1000);
				PlayerInfo[playerid][pJammer] = 1;
				SendClientMessage(playerid, COLOR_GREY, "Signal jammer purchased! (/placejammer)");
			}
			showSAPDDialog(playerid);
		}
		else
		{
			ShowPlayerDialog(playerid, DIALOG_LEO_MAIN, DIALOG_STYLE_LIST, GetPlayerFactionName(playerid), "Duty\nUndercover\nChange Uniform\nArmory\nSWAT\nClear Suspect\nRelease Suspect", "Select", "Cancel");
		}
		return 1;
	}
	if(dialogid == DIALOG_SAPD_SWAT) // SWAT MENU
	{
		if(response)
		{
			if(listitem == 0) //WEAPONS
			{
				ShowPlayerDialog(playerid, DIALOG_SAPD_SWAT_WEAPONS, DIALOG_STYLE_TABLIST, "SWAT Weapons",
					"Desert Eagle\t$500\n\
					Shotgun\t$200\n\
					MP5\t$400\n\
					Rifle\t$1,000\n\
					M4\t$6,000\n\
					Sniper Rifle\t$12,000\n\
					Spas-12\t$15,000\n\
					Infrared Goggles\t$1,500",
				"Select", "Back");
			}
			if(listitem == 1) //ACCESSORIES
			{
				ShowPlayerDialog(playerid, DIALOG_SAPD_SWAT_ACCESSORIES, DIALOG_STYLE_TABLIST, "SWAT Accessories",
					"Tear Gas\t$800\n\
					Grenades\t$800\n\
					Kevlar\t$2,000",
				"Select", "Back");
			}
			if(listitem == 2)
			{
				PlayerInfo[playerid][pClothes] = 0;
				SetPlayerSkin(playerid, 285);
				PlayerInfo[playerid][pModel] = 285;
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You equipped your SWAT Uniform *");
			}
		}
		else
		{
			ShowPlayerDialog(playerid, DIALOG_LEO_MAIN, DIALOG_STYLE_LIST, GetPlayerFactionName(playerid), "Duty\nUndercover\nChange Uniform\nArmory\nSWAT\nClear Suspect\nRelease Suspect", "Select", "Cancel");
		}
		return 1;
	}
	if(dialogid == DIALOG_SAPD_SWAT_WEAPONS) //WEAPONS
	{
		if(response)
		{
			new playercash = PlayerInfo[playerid][pCash];
			if(listitem == 0)
			{
				if(playercash < 500)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				GiveMoney(playerid, -500);
				GivePlayerGun(playerid, 24);
				TogglePlayerControllable(playerid, true);
			}
			if(listitem == 1)
			{
				if(playercash < 200)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				GiveMoney(playerid, -200);
				GivePlayerGun(playerid, 25);
				TogglePlayerControllable(playerid, true);
			}
			if(listitem == 2)
			{
				if(playercash < 400)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				GiveMoney(playerid, -400);
				GivePlayerGun(playerid, 29);
				TogglePlayerControllable(playerid, true);
			}
			if(listitem == 3)
			{
				if(playercash < 1000)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				GiveMoney(playerid, -1000);
				GivePlayerGun(playerid, 33);
				TogglePlayerControllable(playerid, true);
			}
			if(listitem == 4)
			{
				if(playercash < 6000)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				GiveMoney(playerid, -6000);
				GivePlayerGun(playerid, 31);
				TogglePlayerControllable(playerid, true);
			}
			if(listitem == 5)
			{
				if(playercash < 12000)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				GiveMoney(playerid, -12000);
				GivePlayerGun(playerid, 34);
				TogglePlayerControllable(playerid, true);
			}
			if(listitem == 6)
			{
				if(playercash < 15000)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				GiveMoney(playerid, -15000);
				GivePlayerGun(playerid, 27);
				TogglePlayerControllable(playerid, true);
			}
			if(listitem == 7) //Thermal goggles
			{
				if(PlayerInfo[playerid][pCash] < 1500)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					GiveMoney(playerid, -1500);
					GivePlayerGun(playerid, 45);
				}
			}
			ShowPlayerDialog(playerid, DIALOG_SAPD_SWAT_WEAPONS, DIALOG_STYLE_TABLIST, "SWAT Weapons",
					"Desert Eagle\t$500\n\
					Shotgun\t$200\n\
					MP5\t$400\n\
					Rifle\t$1,000\n\
					M4\t$6,000\n\
					Sniper Rifle\t$12,000\n\
					Spas-12\t$15,000\n\
					Infrared Goggles\t$1,500",
				"Select", "Back");
		}
		else
		{
			ShowPlayerDialog(playerid, DIALOG_SAPD_SWAT, DIALOG_STYLE_LIST, "SWAT", "Weapons\nAccessories\nSWAT Uniform", "Select", "Back");
		}
		return 1;
	}
	if(dialogid == DIALOG_SAPD_SWAT_ACCESSORIES) //ACCESSORIES
	{
		if(response)
		{
			new playercash = PlayerInfo[playerid][pCash];
			if(listitem == 0) // tear gas
			{
				if(playercash < 800)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				GiveMoney(playerid, -800);
				GivePlayerGun(playerid, 17);
				TogglePlayerControllable(playerid, true);
			}
			if(listitem == 1) // grenades
			{
				return SendClientMessage(playerid, COLOR_GREY, "Grenades have been disabled.");
				/*
				if(playercash < 1000)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				GiveMoney(playerid, -1000);
				GivePlayerGun(playerid, 16);
				if(GetPlayerAmmo(playerid) >= 2) //ammo fix
				{
					GivePlayerGun(playerid, 16);
				}
				TogglePlayerControllable(playerid, true);*/
			}
			if(listitem == 2) // armor
			{
				if(playercash < 2000)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				GiveMoney(playerid, -2000);
				SetPlayerArmour(playerid, 100.0);
				TogglePlayerControllable(playerid, true);
			}
			ShowPlayerDialog(playerid, DIALOG_SAPD_SWAT_ACCESSORIES, DIALOG_STYLE_TABLIST, "SWAT Accessories",
					"Tear Gas\t$800\n\
					Grenades\t$800\n\
					Kevlar\t$2,000",
				"Select", "Back");
		}
		else
		{
			ShowPlayerDialog(playerid, DIALOG_SAPD_SWAT, DIALOG_STYLE_LIST, "SWAT", "Weapons\nAccessories\nSWAT Uniform", "Select", "Back");
		}
		return 1;
	}
	if(dialogid == DIALOG_ADMIN_COLORCHANGER)
	{
		if(response)
		{
			if(listitem == 0) // DEFINE MACROS COLORS, BUG
			{
				SetPlayerColor(playerid, 0x80008000);
			}
			if(listitem == 1)
			{
				SetPlayerColor(playerid, 0xFFFF0000);
			}
			if(listitem == 2)
			{
				SetPlayerColor(playerid, 0xff000000);
			}
			if(listitem == 3)
			{
				SetPlayerColor(playerid, 0x33CCFF00);
			}
			if(listitem == 4)
			{
				SetPlayerColor(playerid, 0xFF990000);
			}
			if(listitem == 5)
			{
				SetPlayerColor(playerid, 0x33AA3300);
			}
			if(listitem == 6)
			{
				SetPlayerColor(playerid, 0x4B00B000);
			}
			if(listitem == 7)
			{
				SetPlayerColor(playerid, 0xFF66FF00);
			}
			if(listitem == 8)
			{
				SetPlayerColor(playerid, 0x00000000);
			}
		}
		return 1;
	}
	
	if(dialogid == DIALOG_APPEARANCE_GENDER)
	{
		if(response) // pressed male
		{
			if(PlayerInfo[playerid][pCash] < 25000) return SendClientMessage(playerid, COLOR_GRAD1, "You need $25,000 to change your gender!");
			if(listitem == 0)
			{
				SendClientMessage(playerid, COLOR_YELLOW, "You have successfully changed your sex to: Male.");
				PlayerInfo[playerid][pCash] -= 25000;
				PlayerInfo[playerid][pSex] = 1;
				ShowAppearanceMain(playerid);
				return OnPlayerSave(playerid);
			}
			if(listitem == 1) // pressed female
			{
				SendClientMessage(playerid, COLOR_YELLOW, "You have successfully changed your sex to: Female.");
				PlayerInfo[playerid][pCash] -= 25000;
				PlayerInfo[playerid][pSex] = 2;
				ShowAppearanceMain(playerid);
				return OnPlayerSave(playerid);
			}
			return 1;
		}
		return ShowAppearanceMain(playerid);
	}
	if(dialogid == DIALOG_APPEARANCE_DISPLAY) return 1;
	if(dialogid == DIALOG_APPEARANCE_DISPLAYMDC)
	{
		if(response)
		{
			return ShowPlayerDialog(playerid, DIALOG_MDC_MAIN+3, DIALOG_STYLE_LIST, "{2641FE}Mobile Data Computer - Appearance", "Profile\nView History\n{FF6347}Add Warrant{FFFFFF}\nTrack Cellphone\nView Active Tickets\nRemove Ticket\nView Old Tickets\nAppearance", "Select", "Close");
		}
		return 1;
	}
	if(dialogid == DIALOG_APPEARANCE_AGE)
	{
		if(response)
		{
			if(PlayerInfo[playerid][pCash] < 25000) return SendClientMessage(playerid, COLOR_GRAD1, "You need $25,000 to change your age!");
			new amount = strval(inputtext);
			if(amount < 16 || amount > 75) return SendClientMessage(playerid, COLOR_GREY, "Your age can't be above 75, or below 16.");
			
			format(str, sizeof(str), "You have successfully changed your age to: %d years old.", amount);
			SendClientMessage(playerid, COLOR_YELLOW, str);
			PlayerInfo[playerid][pCash] -= 25000;
			PlayerInfo[playerid][pAge] = amount;
			ShowAppearanceMain(playerid);
			return OnPlayerSave(playerid);
		}
		return ShowAppearanceMain(playerid);
	}
	if(dialogid == DIALOG_APPEARANCE_ETHNICITY)
	{
		if(response)
		{
			PlayerInfo[playerid][pEthnicity] = listitem;
			format(str, sizeof(str), "You have successfully changed your ethnicity to: %s.", GetPlayerEthnicity(playerid));
			SendClientMessage(playerid, COLOR_YELLOW, str);
			ShowAppearanceMain(playerid);
			return OnPlayerSave(playerid);
		}
		return ShowAppearanceMain(playerid);
	}
	if(dialogid == DIALOG_APPEARANCE_EYE)
	{
		if(response)
		{
			PlayerInfo[playerid][pEyeColour] = listitem;
			format(str, sizeof(str), "You have successfully changed your eye colour to: %s.", GetPlayerEyeColour(playerid));
			SendClientMessage(playerid, COLOR_YELLOW, str);
			ShowAppearanceMain(playerid);
			return OnPlayerSave(playerid);
		}
		return ShowAppearanceMain(playerid);
	}
	if(dialogid == DIALOG_APPEARANCE_BUILD)
	{
		if(response)
		{
			PlayerInfo[playerid][pBuild] = listitem;
			format(str, sizeof(str), "You have successfully changed your body build type to: %s.", GetPlayerBuild(playerid));
			SendClientMessage(playerid, COLOR_YELLOW, str);
			ShowAppearanceMain(playerid);
			return OnPlayerSave(playerid);
		}
		return ShowAppearanceMain(playerid);
	}
	if(dialogid == DIALOG_APPEARANCE_HEIGHT)
	{
		if(response)
		{
			new amount = strval(inputtext);
			if(amount < 140 || amount > 210) return SendClientMessage(playerid, COLOR_GRAD2, "Character height must be between 140cm and 210cm!");
			
			PlayerInfo[playerid][pHeight] = amount;
			format(str, sizeof(str), "You have successfully changed your height to: %dcm.", amount);
			SendClientMessage(playerid, COLOR_YELLOW, str);
			ShowAppearanceMain(playerid);
			return OnPlayerSave(playerid);
		}
		return ShowAppearanceMain(playerid);
	}
	if(dialogid == DIALOG_APPEARANCE_UNIQUE)
	{
		if(response)
		{
			if(strlen(inputtext) >= 64) return SendClientMessage(playerid, COLOR_GRAD1, "Too much text - please keep it within 64 letters.");
			strmid(PlayerInfo[playerid][pAppearance], inputtext, 0, strlen(inputtext), 255); // Don't want string length too long, 64 is good?
			format(str, sizeof(str), "You have successfully changed your unique features to: %s.", inputtext);
			SendClientMessage(playerid, COLOR_YELLOW, str);
			ShowAppearanceMain(playerid);
			return OnPlayerSave(playerid);
		}
		return ShowAppearanceMain(playerid);
	}
	if(dialogid == DIALOG_APPEARANCE_MAIN)
	{
		//ShowPlayerDialog(playerid, DIALOG_APPEARANCE_MAIN, DIALOG_STYLE_LIST, "Character Appearance", "Gender\nAge\nEthnicity\nEye Colour\nBuild\nHeight (feet)\nUnique Features", "Select", "Cancel");
		if(response)
		{
			if(listitem == 0) return ShowPlayerDialog(playerid, DIALOG_APPEARANCE_GENDER, DIALOG_STYLE_LIST, "Appearance: Gender ($25,000)", "Male\nFemale", "Select", "Back");
			if(listitem == 1) return ShowPlayerDialog(playerid, DIALOG_APPEARANCE_AGE, DIALOG_STYLE_INPUT, "Appearance: Age ($25,000)", "Please enter your character's age.", "Select", "Back");
			if(listitem == 2) return ShowPlayerDialog(playerid, DIALOG_APPEARANCE_ETHNICITY, DIALOG_STYLE_LIST, "Appearance: Ethnicity", "White\nBlack\nAsian\nHispanic\nLatino\nNative American", "Select", "Back");
			if(listitem == 3) return ShowPlayerDialog(playerid, DIALOG_APPEARANCE_EYE, DIALOG_STYLE_LIST, "Appearance: Eye Colour", "Blue\nBrown\nGreen", "Select", "Back");
			if(listitem == 4) return ShowPlayerDialog(playerid, DIALOG_APPEARANCE_BUILD, DIALOG_STYLE_LIST, "Appearance: Build", "Slim\nAverage\nMuscular", "Select", "Back");
			if(listitem == 5) return ShowPlayerDialog(playerid, DIALOG_APPEARANCE_HEIGHT, DIALOG_STYLE_INPUT, "Appearance: Height", "Please enter your character's height.", "Select", "Back");
			if(listitem == 6) return ShowPlayerDialog(playerid, DIALOG_APPEARANCE_UNIQUE, DIALOG_STYLE_INPUT, "Appearance: Unique Features", "Please enter your character's distinguishable remarks (e.g. specific tattoo).", "Select", "Back");			
		}
		return 1; // Pressed cancel
	}
	if(dialogid == AMSGS) // Admin Messages
	{
		if(!response)
		{
			return 1;
		}
		else
		{
			if(!strlen(inputtext))
			{
				displayAdminMessages(playerid);
				SendClientMessage(playerid, COLOR_GRAD1, "You must enter a message before clicking submit.");
				return 1;
			}
			else
			{
				//get time
				new hour, mins, sec;
				gettime(hour, mins, sec);
				hour = FixHour(hour);
				//get admin color based on rank
				new acolor[25];
				if(PlayerInfo[playerid][pAdmin] == 1)
				{
					acolor = C_SILENT_ADMIN;
				}
				if(PlayerInfo[playerid][pAdmin] == 2)
				{
					acolor = C_JUNIOR_ADMIN;
				}
				if(PlayerInfo[playerid][pAdmin] == 3)
				{
					acolor = C_GENERAL_ADMIN;
				}
				if(PlayerInfo[playerid][pAdmin] >= 4 && PlayerInfo[playerid][pAdmin] < 1337)
				{
					acolor = C_SENIOR_ADMIN;
				}
				if(PlayerInfo[playerid][pAdmin] >= 1337 && PlayerInfo[playerid][pAdmin] < 99999)
				{
					acolor = C_HEAD_ADMIN;
				}
				if(PlayerInfo[playerid][pAdmin] >= 99999)
				{
					acolor = C_EXEC_ADMIN;
				}
				format(str, sizeof(str), "[%d:%d]%s %s{BFBDBE}: %s", hour, mins, acolor, PlayerICName(playerid), inputtext);
				if(LINES_AMOUNT == sizeof(aMessageInfo))   //if at the end of the array
				{
					shiftArray(); //shift all msgs back 1 index
					strmid(aMessageInfo[LINES_AMOUNT -1], str, 0, 128);
				}
				else
				{
					strmid(aMessageInfo[LINES_AMOUNT], str, 0, 128);
					LINES_AMOUNT++;
				}
				SaveAdminMessages();
				displayAdminMessages(playerid);
			}
		}
		return 1;
	}
	if(dialogid == FAMILY_DIALOG)
	{
		if(!response) return 1;
		if(listitem != -1)
		{
			new famstring[512];
			new playersOnline = 0;
			foreach( new i: Player )
			{
				if(PlayerInfo[i][pFMember] == listitem)
				{
					playersOnline++;
				}
			}
			new nextxp = GetFamilyNextXP(listitem);
			new level = GetFamilyLevel(listitem);
			if(FamilyInfo[listitem][FamilyLeaderHide]) 
			{
				format(famstring, sizeof(famstring), "{FFFFFF}Name: %s\nLeader: Hidden\nMembers: %d/%d\nStrikes: %d/3\nXP: %d/%d (Level %d, %d XP left for level up)", FamilyInfo[listitem][FamilyName], playersOnline, FamilyInfo[listitem][FamilyMembers], FamilyInfo[listitem][FStrikes], FamilyInfo[listitem][fXP], nextxp, level, nextxp-FamilyInfo[listitem][fXP]);
				if(nextxp == -1) format(famstring, sizeof(famstring), "{FFFFFF}Name: %s\nLeader: Hidden\nMembers: %d/%d\nStrikes: %d/3\nLevel %d MAX", FamilyInfo[listitem][FamilyName], playersOnline, FamilyInfo[listitem][FamilyMembers], FamilyInfo[listitem][FStrikes], level);				
			}
			else 
			{
				format(famstring, sizeof(famstring), "{FFFFFF}Name: %s\nLeader: %s\nMembers: %d/%d\nStrikes: %d/3\nXP: %d/%d (Level %d, %d XP left for level up)", FamilyInfo[listitem][FamilyName], FamilyInfo[listitem][FamilyLeader], playersOnline, FamilyInfo[listitem][FamilyMembers], FamilyInfo[listitem][FStrikes], FamilyInfo[listitem][fXP], nextxp, level, nextxp-FamilyInfo[listitem][fXP]);
				if(nextxp == -1) format(famstring, sizeof(famstring), "{FFFFFF}Name: %s\nLeader: %s\nMembers: %d/%d\nStrikes: %d/3\nLevel %d MAX", FamilyInfo[listitem][FamilyName], FamilyInfo[listitem][FamilyLeader], playersOnline, FamilyInfo[listitem][FamilyMembers], FamilyInfo[listitem][FStrikes], level);				
			}
			ShowPlayerDialog(playerid, FAMILY_INFO, DIALOG_STYLE_MSGBOX, "Families", famstring, "Ok", "Cancel");
		}
		return 1;
	}
	if(dialogid == 48) //refund
	{
		if(!IsPlayerConnected(RefundingID[playerid]) || RefundingID[playerid] == INVALID_PLAYER_ID)
		{
			RefundingID[playerid] = 999;
			RefundingNumber[playerid] = 999;
			SendClientMessage(playerid, COLOR_GREY, "That player is Offline / Invalid ID.");
			return 1;
		}
		if(!response) //cancel
		{
			RefundingNumber[playerid] = 999;
			DisplayDialogForPlayer(playerid, 47); //refund
		}
		if(response)
		{
			if(!strlen(inputtext)) //no text inputted
			{
				RefundingNumber[playerid] = 999;
				DisplayDialogForPlayer(playerid, 47); //refund
				return 1;
			}
			new amount;
			amount = strval(inputtext);
			new year, month, day;
			getdate(year, month, day);
			new stat = RefundingNumber[playerid];
			new giveplayerid = RefundingID[playerid];
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
			switch(stat)
			{
				case 0: //Level
				{
					RefundingNumber[playerid] = 999;
					PlayerInfo[giveplayerid][pLevel] = amount; //edit
					format(str, sizeof(str), "* You have set %s's Level to %d.", giveplayer, amount); //edit
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* An Admin has set your Level to %d.", amount); //edit
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					DisplayDialogForPlayer(playerid, 47); //refund
					format(str, sizeof(str), "[%d/%d/%d] %s has set %s's Level to %d", day, month, year, sendername, giveplayer, amount); //edit
					StatLog(str);
				}
				case 1: //Cash
				{
					RefundingNumber[playerid] = 999;
					ResetMoney(giveplayerid);
					GiveMoney(giveplayerid, amount);
					format(str, sizeof(str), "* You have set %s's Cash to $%d.", giveplayer, amount); //edit
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* An Admin has set your Cash to $%d.", amount); //edit
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					DisplayDialogForPlayer(playerid, 47); //refund
					format(str, sizeof(str), "[%d/%d/%d] %s has set %s's Cash to $%d", day, month, year, sendername, giveplayer, amount); //edit
					StatLog(str);
				}
				case 2: //Bank
				{
					RefundingNumber[playerid] = 999;
					PlayerInfo[giveplayerid][pAccount] = amount; //edit
					format(str, sizeof(str), "* You have set %s's Bank Account to $%d.", giveplayer, amount); //edit
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* An Admin has set your Bank Account to $%d.", amount); //edit
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					DisplayDialogForPlayer(playerid, 47); //refund
					format(str, sizeof(str), "[%d/%d/%d] %s has set %s's Bank Account to $%d", day, month, year, sendername, giveplayer, amount); //edit
					StatLog(str);
				}
				case 3: //Materials
				{
					RefundingNumber[playerid] = 999;
					PlayerInfo[giveplayerid][pMats] = amount; //edit
					format(str, sizeof(str), "* You have set %s's Materials to %d.", giveplayer, amount); //edit
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* An Admin has set your Materials to %d.", amount); //edit
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					DisplayDialogForPlayer(playerid, 47); //refund
					format(str, sizeof(str), "[%d/%d/%d] %s has set %s's Materials to %d", day, month, year, sendername, giveplayer, amount); //edit
					StatLog(str);
				}
				case 4: //Crack
				{
					RefundingNumber[playerid] = 999;
					PlayerInfo[giveplayerid][pCocaine] = amount; //edit
					format(str, sizeof(str), "* You have set %s's Cocaine to %d.", giveplayer, amount); //edit
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* An Admin has set your Cocaine to %d.", amount); //edit
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					DisplayDialogForPlayer(playerid, 47); //refund
					format(str, sizeof(str), "[%d/%d/%d] %s has set %s's Cocaine to %d", day, month, year, sendername, giveplayer, amount); //edit
					StatLog(str);
				}
				case 5: //Pot
				{
					RefundingNumber[playerid] = 999;
					PlayerInfo[giveplayerid][pCannabis] = amount; //edit
					format(str, sizeof(str), "* You have set %s's Pot to %d.", giveplayer, amount); //edit
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* An Admin has set your Pot to %d.", amount); //edit
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					DisplayDialogForPlayer(playerid, 47); //refund
					format(str, sizeof(str), "[%d/%d/%d] %s has set %s's Pot to %d", day, month, year, sendername, giveplayer, amount); //edit
					StatLog(str);
				}
				case 6: //Arms Dealer
				{
					RefundingNumber[playerid] = 999;
					//
					new lvl;
					if(amount == 1)
					{
						lvl = 0;
					}
					else if(amount == 2)
					{
						lvl = 50;
					}
					else if(amount == 3)
					{
						lvl = 100;
					}
					else if(amount == 4)
					{
						lvl = 200;
					}
					else if(amount == 5)
					{
						lvl = 400;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "Amount can't be below 0 or above 5.");	//edit
						DisplayDialogForPlayer(playerid, 47);
						return 1;
					}
					PlayerInfo[giveplayerid][pArmsSkill] = lvl; //edit
					//
					format(str, sizeof(str), "* You have set %s's Arms Dealer Level to %d.", giveplayer, amount); //edit
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* An Admin has set your Arms Dealer Level to %d.", amount); //edit
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					DisplayDialogForPlayer(playerid, 47); //refund
					format(str, sizeof(str), "[%d/%d/%d] %s has set %s's Arms Dealer Level to %d", day, month, year, sendername, giveplayer, amount); //edit
					StatLog(str);
				}
				case 7: //Carjacker
				{
					RefundingNumber[playerid] = 999;
					//
					new lvl;
					if(amount == 1)
					{
						lvl = 0;
					}
					else if(amount == 2)
					{
						lvl = 50;
					}
					else if(amount == 3)
					{
						lvl = 100;
					}
					else if(amount == 4)
					{
						lvl = 200;
					}
					else if(amount == 5)
					{
						lvl = 400;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "Amount can't be below 0 or above 5.");	//edit
						DisplayDialogForPlayer(playerid, 47);
						return 1;
					}
					PlayerInfo[giveplayerid][pJackSkill] = lvl; //edit
					//
					format(str, sizeof(str), "* You have set %s's Car Jacker Level to %d.", giveplayer, amount); //edit
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* An Admin has set your Car Jacker Level to %d.", amount); //edit
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					DisplayDialogForPlayer(playerid, 47); //refund
					format(str, sizeof(str), "[%d/%d/%d] %s has set %s's Car Jacker Level to %d", day, month, year, sendername, giveplayer, amount); //edit
					StatLog(str);
				}
				case 8: //Detective
				{
					RefundingNumber[playerid] = 999;
					//
					new lvl;
					if(amount == 1)
					{
						lvl = 0;
					}
					else if(amount == 2)
					{
						lvl = 50;
					}
					else if(amount == 3)
					{
						lvl = 100;
					}
					else if(amount == 4)
					{
						lvl = 200;
					}
					else if(amount == 5)
					{
						lvl = 400;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "Amount can't be below 0 or above 5.");	//edit
						DisplayDialogForPlayer(playerid, 47);
						return 1;
					}
					PlayerInfo[giveplayerid][pDetSkill] = lvl; //edit
					//
					format(str, sizeof(str), "* You have set %s's Detective Level to %d.", giveplayer, amount); //edit
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* An Admin has set your Detective Level to %d.", amount); //edit
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					DisplayDialogForPlayer(playerid, 47); //refund
					format(str, sizeof(str), "[%d/%d/%d] %s has set %s's Detective Level to %d", day, month, year, sendername, giveplayer, amount); //edit
					StatLog(str);
				}
				case 9: //Lawyer
				{
					RefundingNumber[playerid] = 999;
					//
					new lvl;
					if(amount == 1)
					{
						lvl = 0;
					}
					else if(amount == 2)
					{
						lvl = 50;
					}
					else if(amount == 3)
					{
						lvl = 100;
					}
					else if(amount == 4)
					{
						lvl = 200;
					}
					else if(amount == 5)
					{
						lvl = 400;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "Amount can't be below 0 or above 5.");	//edit
						DisplayDialogForPlayer(playerid, 47);
						return 1;
					}
					PlayerInfo[giveplayerid][pLawSkill] = lvl; //edit
					//
					format(str, sizeof(str), "* You have set %s's Lawyer Level to %d.", giveplayer, amount); //edit
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* An Admin has set your Lawyer Level to %d.", amount); //edit
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					DisplayDialogForPlayer(playerid, 47); //refund
					format(str, sizeof(str), "[%d/%d/%d] %s has set %s's Lawyer Level to %d", day, month, year, sendername, giveplayer, amount); //edit
					StatLog(str);
				}
				case 10: //Mechanic
				{
					RefundingNumber[playerid] = 999;
					//
					new lvl;
					if(amount == 1)
					{
						lvl = 0;
					}
					else if(amount == 2)
					{
						lvl = 50;
					}
					else if(amount == 3)
					{
						lvl = 100;
					}
					else if(amount == 4)
					{
						lvl = 200;
					}
					else if(amount == 5)
					{
						lvl = 400;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "Amount can't be below 0 or above 5.");	//edit
						DisplayDialogForPlayer(playerid, 47);
						return 1;
					}
					PlayerInfo[giveplayerid][pMechSkill] = lvl; //edit
					//
					format(str, sizeof(str), "* You have set %s's Mechanic Level to %d.", giveplayer, amount); //edit
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* An Admin has set your Mechanic Level to %d.", amount); //edit
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					DisplayDialogForPlayer(playerid, 47); //refund
					format(str, sizeof(str), "[%d/%d/%d] %s has set %s's Mechanic Level to %d", day, month, year, sendername, giveplayer, amount); //edit
					StatLog(str);
				}
				case 11: //Drug Dealer
				{
					RefundingNumber[playerid] = 999;
					//
					new lvl;
					if(amount == 1)
					{
						lvl = 0;
					}
					else if(amount == 2)
					{
						lvl = 50;
					}
					else if(amount == 3)
					{
						lvl = 100;
					}
					else if(amount == 4)
					{
						lvl = 200;
					}
					else if(amount == 5)
					{
						lvl = 400;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "Amount can't be below 0 or above 5.");	//edit
						DisplayDialogForPlayer(playerid, 47);
						return 1;
					}
					PlayerInfo[giveplayerid][pDrugsSkill] = lvl; //edit
					//
					//PlayerInfo[giveplayerid][pDrugsSkill] = amount; //edit
					format(str, sizeof(str), "* You have set %s's Drug Dealer Level to %d.", giveplayer, amount); //edit
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* An Admin has set your Drug Dealer Level to %d.", amount); //edit
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					DisplayDialogForPlayer(playerid, 47); //refund
					format(str, sizeof(str), "[%d/%d/%d] %s has set %s's Drug Dealer Level to %d", day, month, year, sendername, giveplayer, amount); //edit
					StatLog(str);
				}
				case 12: //Sex
				{
					RefundingNumber[playerid] = 999;
					//
					new lvl;
					if(amount == 1)
					{
						lvl = 0;
					}
					else if(amount == 2)
					{
						lvl = 50;
					}
					else if(amount == 3)
					{
						lvl = 100;
					}
					else if(amount == 4)
					{
						lvl = 200;
					}
					else if(amount == 5)
					{
						lvl = 400;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "Amount can't be below 0 or above 5.");	//edit
						DisplayDialogForPlayer(playerid, 47);
						return 1;
					}
					PlayerInfo[giveplayerid][pSexSkill] = lvl; //edit
					format(str, sizeof(str), "* You have set %s's Sex Level to %d.", giveplayer, amount); //edit
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* An Admin has set your Sex Level to %d.", amount); //edit
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					DisplayDialogForPlayer(playerid, 47); //refund
					format(str, sizeof(str), "[%d/%d/%d] %s has set %s's Sex Level to %d", day, month, year, sendername, giveplayer, amount); //edit
					StatLog(str);
				}
				case 13: //Boxing
				{
					RefundingNumber[playerid] = 999;
					//
					new lvl;
					if(amount == 1)
					{
						lvl = 0;
					}
					else if(amount == 2)
					{
						lvl = 50;
					}
					else if(amount == 3)
					{
						lvl = 100;
					}
					else if(amount == 4)
					{
						lvl = 200;
					}
					else if(amount == 5)
					{
						lvl = 400;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "Amount can't be below 0 or above 5.");	//edit
						DisplayDialogForPlayer(playerid, 47);
						return 1;
					}
					PlayerInfo[giveplayerid][pBoxSkill] = lvl; //edit
					//
					format(str, sizeof(str), "* You have set %s's Boxing Level to %d.", giveplayer, amount); //edit
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* An Admin has set your Boxing Level to %d.", amount); //edit
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					DisplayDialogForPlayer(playerid, 47); //refund
					format(str, sizeof(str), "[%d/%d/%d] %s has set %s's Boxing Level to %d", day, month, year, sendername, giveplayer, amount); //edit
					StatLog(str);
				}
				case 14: //Fishing
				{
					RefundingNumber[playerid] = 999;
					//
					new lvl;
					if(amount == 1)
					{
						lvl = 0;
					}
					else if(amount == 2)
					{
						lvl = 50;
					}
					else if(amount == 3)
					{
						lvl = 100;
					}
					else if(amount == 4)
					{
						lvl = 200;
					}
					else if(amount == 5)
					{
						lvl = 400;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "Amount can't be below 0 or above 5.");	//edit
						DisplayDialogForPlayer(playerid, 47);
						return 1;
					}
					PlayerInfo[giveplayerid][pFishSkill] = lvl; //edit
					//
					format(str, sizeof(str), "* You have set %s's Fishing Level to %d.", giveplayer, amount); //edit
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* An Admin has set your Fishing Level to %d.", amount); //edit
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					DisplayDialogForPlayer(playerid, 47); //refund
					format(str, sizeof(str), "[%d/%d/%d] %s has set %s's Fishing Level to %d", day, month, year, sendername, giveplayer, amount); //edit
					StatLog(str);
				}
				case 15:
				{
					RefundingNumber[playerid] = 999;

					new lvl;
					switch(amount)
					{
						case 1: lvl = 0;
						case 2: lvl = 50;
						case 3: lvl = 120;
						case 4: lvl = 200;
						case 5: lvl = 270;
						default:
						{
							SendClientMessage(playerid, COLOR_GREY, "Amount can't be below 0 or above 5.");	//edit
							DisplayDialogForPlayer(playerid, 47);
							return 1;
						}
					}
					PlayerInfo[playerid][pPizzaSkill] = lvl;
					format(str, sizeof(str), "* You have set %s's Pizza Level to %d.", giveplayer, amount); //edit
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* An Admin has set your Pizza Level to %d.", amount); //edit
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					DisplayDialogForPlayer(playerid, 47); //refund
					format(str, sizeof(str), "[%d/%d/%d] %s has set %s's Pizza Level to %d", day, month, year, sendername, giveplayer, amount); //edit
					StatLog(str);

				}
case 16:
				{
					RefundingNumber[playerid] = 999;

					new lvl;
					switch(amount)
					{
						case 1: lvl = 0;
						case 2: lvl = 10;
						case 3: lvl = 20;
						case 4: lvl = 30;
						case 5: lvl = 50;
						default:
						{
							SendClientMessage(playerid, COLOR_GREY, "Amount can't be below 0 or above 5.");	//edit
							DisplayDialogForPlayer(playerid, 47);
							return 1;
						}
					}
					PlayerInfo[playerid][pBusSkill] = lvl;
					format(str, sizeof(str), "* You have set %s's Bus Driver Level to %d.", giveplayer, amount); //edit
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* An Admin has set your Bus Driver Level to %d.", amount); //edit
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					DisplayDialogForPlayer(playerid, 47); //refund
					format(str, sizeof(str), "[%d/%d/%d] %s has set %s's Bus Driver to %d", day, month, year, sendername, giveplayer, amount); //edit
					StatLog(str);

				}
				default:
				{
					DisplayDialogForPlayer(playerid, 47); //refund
					SendClientMessage(giveplayerid, COLOR_GREY, "Invalid stat code.");
					return 1;
				}
			}
		}
		return 1;
	}
	if(dialogid == 47) //refund
	{
		if(!response)
		{
			RefundingID[playerid] = 999;
		}
		if(response)
		{
			new statname[32];
			strmid(giveplayer, PlayerICName(RefundingID[playerid]), 0, MAX_PLAYER_NAME);
			switch(listitem)
			{
				case 0:
				{
					RefundingNumber[playerid] = 0;
					statname = "Level";
					format(str, sizeof(str), "Refunding [%d]%s's %s", RefundingID[playerid], giveplayer, statname);
					ShowPlayerDialog(playerid, 48, DIALOG_STYLE_INPUT, str, "Enter the amount below.", "Confirm", "Cancel");
				}
				case 1:
				{
					RefundingNumber[playerid] = 1;
					statname = "Cash";
					format(str, sizeof(str), "Refunding [%d]%s's %s", RefundingID[playerid], giveplayer, statname);
					ShowPlayerDialog(playerid, 48, DIALOG_STYLE_INPUT, str, "Enter the amount below.", "Confirm", "Cancel");
				}
				case 2:
				{
					RefundingNumber[playerid] = 2;
					statname = "Bank";
					format(str, sizeof(str), "Refunding [%d]%s's %s", RefundingID[playerid], giveplayer, statname);
					ShowPlayerDialog(playerid, 48, DIALOG_STYLE_INPUT, str, "Enter the amount below.", "Confirm", "Cancel");
					return 1;
				}
				case 3:
				{
					RefundingNumber[playerid] = 3;
					statname = "Materials";
					format(str, sizeof(str), "Refunding [%d]%s's %s", RefundingID[playerid], giveplayer, statname);
					ShowPlayerDialog(playerid, 48, DIALOG_STYLE_INPUT, str, "Enter the amount below.", "Confirm", "Cancel");
				}
				case 4:
				{
					RefundingNumber[playerid] = 6;
					statname = "Arms Dealer Skill";
					format(str, sizeof(str), "Refunding [%d]%s's %s", RefundingID[playerid], giveplayer, statname);
					ShowPlayerDialog(playerid, 48, DIALOG_STYLE_INPUT, str, "Enter the amount below.", "Confirm", "Cancel");
				}
				case 5:
				{
					RefundingNumber[playerid] = 7;
					statname = "Carjacker Skill";
					format(str, sizeof(str), "Refunding [%d]%s's %s", RefundingID[playerid], giveplayer, statname);
					ShowPlayerDialog(playerid, 48, DIALOG_STYLE_INPUT, str, "Enter the amount below.", "Confirm", "Cancel");
				}
				case 6:
				{
					RefundingNumber[playerid] = 8;
					statname = "Detective Skill";
					format(str, sizeof(str), "Refunding [%d]%s's %s", RefundingID[playerid], giveplayer, statname);
					ShowPlayerDialog(playerid, 48, DIALOG_STYLE_INPUT, str, "Enter the amount below.", "Confirm", "Cancel");
				}
				case 7:
				{
					RefundingNumber[playerid] = 9;
					statname = "Lawyer Skill";
					format(str, sizeof(str), "Refunding [%d]%s's %s", RefundingID[playerid], giveplayer, statname);
					ShowPlayerDialog(playerid, 48, DIALOG_STYLE_INPUT, str, "Enter the amount below.", "Confirm", "Cancel");
				}
				case 8:
				{
					RefundingNumber[playerid] = 10;
					statname = "Mechanic Skill";
					format(str, sizeof(str), "Refunding [%d]%s's %s", RefundingID[playerid], giveplayer, statname);
					ShowPlayerDialog(playerid, 48, DIALOG_STYLE_INPUT, str, "Enter the amount below.", "Confirm", "Cancel");
				}
				case 9:
				{
					RefundingNumber[playerid] = 11;
					statname = "Drug Dealer Skill";
					format(str, sizeof(str), "Refunding [%d]%s's %s", RefundingID[playerid], giveplayer, statname);
					ShowPlayerDialog(playerid, 48, DIALOG_STYLE_INPUT, str, "Enter the amount below.", "Confirm", "Cancel");
				}
				case 10:
				{
					RefundingNumber[playerid] = 12;
					statname = "Sex Skill";
					format(str, sizeof(str), "Refunding [%d]%s's %s", RefundingID[playerid], giveplayer, statname);
					ShowPlayerDialog(playerid, 48, DIALOG_STYLE_INPUT, str, "Enter the amount below.", "Confirm", "Cancel");
				}
				case 11:
				{
					RefundingNumber[playerid] = 13;
					statname = "Boxing Skill";
					format(str, sizeof(str), "Refunding [%d]%s's %s", RefundingID[playerid], giveplayer, statname);
					ShowPlayerDialog(playerid, 48, DIALOG_STYLE_INPUT, str, "Enter the amount below.", "Confirm", "Cancel");
				}
				case 12:
				{
					RefundingNumber[playerid] = 14;
					statname = "Fishing Skill";
					format(str, sizeof(str), "Refunding [%d]%s's %s", RefundingID[playerid], giveplayer, statname);
					ShowPlayerDialog(playerid, 48, DIALOG_STYLE_INPUT, str, "Enter the amount below.", "Confirm", "Cancel");
				}
				case 13:
				{
					RefundingNumber[playerid] = 15;
					statname = "Pizza Skill";
					format(str, sizeof(str), "Refunding [%d]%s's %s", RefundingID[playerid], giveplayer, statname);
					ShowPlayerDialog(playerid, 48, DIALOG_STYLE_INPUT, str, "Enter the amount below.", "Confirm", "Cancel");
				}
case 14:
				{
					RefundingNumber[playerid] = 16;
					statname = "Bus Skill";
					format(str, sizeof(str), "Refunding [%d]%s's %s", RefundingID[playerid], giveplayer, statname);
					ShowPlayerDialog(playerid, 48, DIALOG_STYLE_INPUT, str, "Enter the amount below.", "Confirm", "Cancel");
				}
			}
			return 1;
		}
		return 1;
	}
	if(dialogid == 59)
	{
		if(response)
		{
			if(strlen(inputtext))
			{
				new amount = strval(inputtext);
				if(amount < 1 || amount > 100000)
				{
					SendClientMessage(playerid, COLOR_GRAD1, "You can't withdraw under $1 or more than $100,000 at a time.");
					return 1;
				}
				if(PlayerInfo[playerid][pAccount] < amount)
				{
					format(str, sizeof(str), "The ATM has denied your transaction.\nThe amount of money ($%d), you want to withdraw, exceeds the amount in your account ($%d).", amount, PlayerInfo[playerid][pAccount]);
					ShowPlayerDialog(playerid, 60, DIALOG_STYLE_MSGBOX, "ATM", str, "Done", "Cancel");
					return 1;
				}
				PlayerInfo[playerid][pAccount] -= amount;
				GiveMoney(playerid, amount);
				format(str, sizeof(str), "You have withdrawn $%d from your bank acount.\nNew balance: $%d", amount, PlayerInfo[playerid][pAccount]);
				ShowPlayerDialog(playerid, 61, DIALOG_STYLE_MSGBOX, "ATM", str, "Done", "Cancel");
				format(str, sizeof(str), "* %s takes their cash out of the machine and puts their card in their pocket.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				return 1;
			}
		}
		return 1;
	}
	if(dialogid == 60)
	{
		if(response) { }
		return 1;
	}
	if(dialogid == 3242)
	{
		if(response)
		{
			if(listitem == 0)
			{
				if(PlayerInfo[playerid][pDonateRank] >= 1)
				{
					if(PlayerInfo[playerid][pVipColor] == 0)
					{
						PlayerInfo[playerid][pVipColor] = 1;
						SendClientMessage(playerid, 0x80008000, "Your name is now colored.");
						SetPlayerToTeamColor(playerid);
					}
					else if(PlayerInfo[playerid][pVipColor] == 1)
					{
						PlayerInfo[playerid][pVipColor] = 0;
						SendClientMessage(playerid, COLOR_GRAD2, "Your name is no longer colored.");
						SetPlayerToTeamColor(playerid);
					}
				}
			}
			if(listitem == 1) // car repair
			{
				/*if(!IsPlayerInAnyVehicle(playerid))
				{
					SendClientMessage(playerid, COLOR_GREY, "You are not in a vehicle.");
					return 1;
				}
				if(vipRepairTime[playerid] > 0)
				{
					format(str, sizeof(str), "Please wait %d seconds before repairing another vehicle.", vipRepairTime[playerid]);
					return SendClientMessage(playerid, COLOR_GREY, str);
				}
				format(str, sizeof(str), "* %s has repaired the vehicle.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				new carid = GetPlayerVehicleID(playerid);
				//RepairVehicle(carid);
				SetVehicleHealth(carid, 999.0);
				PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
				vipRepairTime[playerid] = 60; // 60 seconds*/
				SendClientMessage(playerid, COLOR_GREY, "This feature is currently disabled!");
			}
			if(listitem == 2) // skin
			{
				ShowPlayerDialog(playerid, 1246, DIALOG_STYLE_LIST, "VIP Skins", "Wu Zi Mu\nKent Paul\nMaccer\nKen Rosenberg\nWhite MIB agent\nMichael Toreno\nJizzy B.", "Choose", "Cancel");
			}
		}
		return 1;
	}
	if(dialogid == 62)
	{
		if(response)
		{
			if(listitem == 0)
			{
				if(PlayerInfo[playerid][pDonateRank] >= 1)
				{
					if(PlayerInfo[playerid][pVipColor] == 0)
					{
						PlayerInfo[playerid][pVipColor] = 1;
						SendClientMessage(playerid, 0x80008000, "Your name is now colored.");
						SetPlayerToTeamColor(playerid);
					}
					else if(PlayerInfo[playerid][pVipColor] == 1)
					{
						PlayerInfo[playerid][pVipColor] = 0;
						SendClientMessage(playerid, COLOR_GRAD2, "Your name is no longer colored.");
						SetPlayerToTeamColor(playerid);
					}
				}
			}
			if(listitem == 1) // car tune
			{
				new tempid = GetPlayerVehicleID(playerid);
				if(!IsPlayerInAnyVehicle(playerid))
				{
					SendClientMessage(playerid, COLOR_GREY, "You are not in a vehicle.");
					return 1;
				}
				if(IsAPlane(tempid) || IsAHelicopter(tempid) || IsABike(tempid) || IsATrain(tempid) || IsABoat(tempid))
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't mod this.");
					return 1;
				}
				ShowPlayerDialog(playerid, 63, DIALOG_STYLE_LIST, "Tune Menu", "Colors\nPaintjobs\nWheels\nHydraulics\nRepair", "Select", "Cancel");
			}
			if(listitem == 2) // skin
			{
				ShowPlayerDialog(playerid, 1246, DIALOG_STYLE_LIST, "VIP Skins", "Wu Zi Mu\nKent Paul\nMaccer\nKen Rosenberg\nWhite MIB agent\nMichael Toreno\nJizzy B.", "Choose", "Cancel");
			}
		}
		return 1;
	}
	if(dialogid == 1246)
	{
		if(response)
		{
			if(listitem == 0)
			{
				SendClientMessage(playerid, COLOR_GRAD1, "You have set your skin to \"Wu Zi Mu\" (ID: 294)");
				SetPlayerSkin(playerid, 294);
			}
			if(listitem == 1)
			{
				SendClientMessage(playerid, COLOR_GRAD1, "You have set your skin to \"Kent Paul\" (ID: 291)");
				SetPlayerSkin(playerid, 291);
			}
			if(listitem == 2)
			{
				SendClientMessage(playerid, COLOR_GRAD1, "You have set your skin to \"Maccer\" (ID: 2)");
				SetPlayerSkin(playerid, 2);
			}
			if(listitem == 3)
			{
				SendClientMessage(playerid, COLOR_GRAD1, "You have set your skin to \"Ken Rosenberg\" (ID: 290)");
				SetPlayerSkin(playerid, 290);
			}
			if(listitem == 4)
			{
				SendClientMessage(playerid, COLOR_GRAD1, "You have set your skin to \"White MIB agent\" (ID: 165)");
				SetPlayerSkin(playerid, 165);
			}
			if(listitem == 5)
			{
				SendClientMessage(playerid, COLOR_GRAD1, "You have set your skin to \"Michael Toreno\" (ID: 295)");
				SetPlayerSkin(playerid, 295);
			}
			if(listitem == 6)
			{
				SendClientMessage(playerid, COLOR_GRAD1, "You have set your skin to \"Jizzy B.\" (ID: 296)");
				SetPlayerSkin(playerid, 296);
			}
		}
		return 1;
	}

	if(dialogid == 63)
	{
		if(response)
		{
			if(listitem == 0) // Colors
			{
				ShowPlayerDialog(playerid, 64, DIALOG_STYLE_LIST, "Vehicle Color", "Black\nWhite\nDark Blue\nLight Blue\nGreen\nYellow\nPink\nRed", "Change", "Cancel");
			}
			if(listitem == 1) // Paintjobs
			{
				ShowPlayerDialog(playerid, 65, DIALOG_STYLE_LIST, "Paint Jobs", "Style1\nStyle2\nStyle3", "Change", "Cancel");
			}
			if(listitem == 2) // Wheels
			{
				ShowPlayerDialog(playerid, 66, DIALOG_STYLE_LIST, "Wheels", "Style 1 \nStyle 2 \nStyle 3 \nStyle 4 \nStyle 5 \nStyle 6", "Change", "Cancel");
			}
			if(listitem == 3) // Hydraulics
			{
				new vipcar = GetPlayerVehicleID(playerid);
				if(!IsPlayerInAnyVehicle(playerid))
				{
					SendClientMessage(playerid, COLOR_GREY, "You are not in a vehicle.");
					return 1;
				}
				if(IsAPlane(vipcar) || IsAHelicopter(vipcar) || IsABike(vipcar) || IsATrain(vipcar) || IsABoat(vipcar))
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't mod this.");
					return 1;
				}
				AddVehicleComponent(vipcar, 1087);
				PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
			}
			if(listitem == 4) // Repair
			{
				/*if(!IsPlayerInAnyVehicle(playerid))
				{
					SendClientMessage(playerid, COLOR_GREY, "You are not in a vehicle.");
					return 1;
				}
				if(vipRepairTime[playerid] > 0)
				{
					format(str, sizeof(str), "Please wait %d seconds before repairing another vehicle.", vipRepairTime[playerid]);
					return SendClientMessage(playerid, COLOR_GREY, str);
				}
				format(str, sizeof(str), "* %s has repaired the vehicle.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				new carid = GetPlayerVehicleID(playerid);
				//RepairVehicle(carid);
				SetVehicleHealth(carid, 999.0);
				PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
				vipRepairTime[playerid] = 60; // 60 seconds*/
				SendClientMessage(playerid, COLOR_GREY, "This feature is currently disabled!");
			}
		}
		return 1;
	}
	if(dialogid == 64)
	{
		if(response)
		{
			new vipcar = GetPlayerVehicleID(playerid);
			if(listitem == 0) // black
			{
				PlayerPlaySound(playerid, 1134, 0.0, 0.0, 0.0);
				ChangeVehicleColor(vipcar, 0, 0);
			}
			if(listitem == 1) //White
			{
				PlayerPlaySound(playerid, 1134, 0.0, 0.0, 0.0);
				ChangeVehicleColor(vipcar, 1, 1);
			}
			if(listitem == 2) //Dark Blue
			{
				PlayerPlaySound(playerid, 1134, 0.0, 0.0, 0.0);
				ChangeVehicleColor(vipcar, 425, 425);
			}
			if(listitem == 3) // Light Blue
			{
				PlayerPlaySound(playerid, 1134, 0.0, 0.0, 0.0);
				ChangeVehicleColor(vipcar, 147, 147);
			}
			if(listitem == 4) //Green
			{
				PlayerPlaySound(playerid, 1134, 0.0, 0.0, 0.0);
				ChangeVehicleColor(vipcar, 16, 16);
			}
			if(listitem == 5) //Yellow
			{
				PlayerPlaySound(playerid, 1134, 0.0, 0.0, 0.0);
				ChangeVehicleColor(vipcar, 6, 6);
			}
			if(listitem == 6) //Pink
			{
				PlayerPlaySound(playerid, 1134, 0.0, 0.0, 0.0);
				ChangeVehicleColor(vipcar, 146, 146);
			}
			if(listitem == 7) // Red
			{
				PlayerPlaySound(playerid, 1134, 0.0, 0.0, 0.0);
				ChangeVehicleColor(vipcar, 3, 3);
			}
		}
		return 1;
	}
	if(dialogid == 65)
	{
		if(response)
		{
			new vipcar = GetPlayerVehicleID(playerid);
			if(listitem == 0) // style 1
			{
				PlayerPlaySound(playerid, 1134, 0.0, 0.0, 0.0);
				ChangeVehiclePaintjob(vipcar, 0);
			}
			if(listitem == 1) //Style 2
			{
				PlayerPlaySound(playerid, 1134, 0.0, 0.0, 0.0);
				ChangeVehiclePaintjob(vipcar, 1);
			}
			if(listitem == 2) //style 3
			{
				PlayerPlaySound(playerid, 1134, 0.0, 0.0, 0.0);
				ChangeVehiclePaintjob(vipcar, 2);
			}
		}
		return 1;
	}
	if(dialogid == 66)
	{
		if(response)
		{
			new vipcar = GetPlayerVehicleID(playerid);
			if(!IsPlayerInAnyVehicle(playerid))
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not in a vehicle.");
				return 1;
			}
			if(IsAPlane(vipcar) || IsAHelicopter(vipcar) || IsABike(vipcar) || IsATrain(vipcar) || IsABoat(vipcar))
			{
				//in case they somehow bypass the /vip check
				SendClientMessage(playerid, COLOR_GREY, "You can't mod this.");
				return 1;
			}
			if(listitem == 0) // style 1
			{
				PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
				AddVehicleComponent(vipcar, 1084);
			}
			if(listitem == 1) //Style 2
			{
				PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
				AddVehicleComponent(vipcar, 1073);
			}
			if(listitem == 2) //Style 3
			{
				PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
				AddVehicleComponent(vipcar, 1075);
			}
			if(listitem == 3) //Style 4
			{
				PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
				AddVehicleComponent(vipcar, 1077);
			}
			if(listitem == 4) //Style 5
			{
				PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
				AddVehicleComponent(vipcar, 1079);
			}
			if(listitem == 5) //Style 6
			{
				PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
				AddVehicleComponent(vipcar, 1080);
			}
		}
		return 1;
	}
	if(dialogid == DIALOG_LSFMD_MAIN) //LSFMD
	{
		if(response)
		{
			if(listitem == 0)
			{
				if(PlayerInfo[playerid][pOnDuty] == 0)
				{
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					format(str, sizeof(str), "* Paramedic %s takes their Badge and medic kit from their locker.", sendername);
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					PlayerInfo[playerid][pOnDuty] = 1;
					if(PlayerInfo[playerid][pMember] == 11) FakeFaction[playerid] = 4;
					SetPlayerColor(playerid, TCOLOR_PARAMEDIC);
					SetPlayerHealth(playerid, 100);
					SetPlayerArmour(playerid, 100);
					if(PlayerInfo[playerid][pRank] != 0)
					{
						GivePlayerGun(playerid, WEAPON_SPRAYCAN); // Spray can
					}

					if(PlayerInfo[playerid][pGroup] == GROUP_S9 && PlayerInfo[playerid][pMember] != FACTION_FMD)
					{
						SetPlayerSkin(playerid, 275);
						PlayerInfo[playerid][pModel] = 275;

						PlayerInfo[playerid][pOnDuty] = 1;
						PlayerInfo[playerid][pMember] = FACTION_FMD;
						PlayerInfo[playerid][pRank] = 1;

						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have been given a badge automatically (SAFMD EMR)");
					}
				}
				else if(PlayerInfo[playerid][pOnDuty] == 1)
				{
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					format(str, sizeof(str), "* Paramedic %s puts their Badge and their medic kit in their locker.", sendername);
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					PlayerInfo[playerid][pOnDuty] = 0;
					SetPlayerColor(playerid, TCOLOR_WHITE);
				}
				return 1;
			}
			if(listitem == 1)
			{
				// Shows faction skin selector
				ShowSkinShop(playerid, false, true, false, false, true);
				return 1;
			}
			if(listitem == 2)
			{
				// Shows civilian skin selector
				ShowSkinShop(playerid, true, false, false, false, false);
				return 1;
			}
			if(listitem == 3)
			{
				ShowPlayerDialog(playerid, DIALOG_LSFMD_GEAR, DIALOG_STYLE_TABLIST, "Gear",
					"Fire Extinguisher\t$50\n\
					Desert eagle\t$1000\n\
					Kevlar\t$2000\n\
					Temporary RRK\tFree\n\
					Radio\t$150\n\
					First aid kit\t$150\n\
					Portable MDC\tFree",
				"Purchase", "Back");
				return 1;
			}
		}
		return 1;
	}
	if(dialogid == DIALOG_LSFMD_GEAR) //LSFMD GEAR
	{
		if(response)
		{
			if(listitem == 0) // Fire Extinguisher
			{
				if(PlayerInfo[playerid][pCash] < 50)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					GiveMoney(playerid, -50);
					GivePlayerGun(playerid, 42);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 1) // Deagle
			{
				if(PlayerInfo[playerid][pRank] < 1) return SendClientMessage(playerid, COLOR_GREY, "This may only be used by R1s+");
				if(PlayerInfo[playerid][pCash] < 1000)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					GiveMoney(playerid, -1000);
					GivePlayerGun(playerid, WEAPON_DEAGLE);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 2) //Kevlar
			{
				if(PlayerInfo[playerid][pCash] < 2000)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					GiveMoney(playerid, -2000);
					SetPlayerArmour(playerid, 100);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 3) // Temp RRK
			{
				if(FMD_RRK_DISABLED)
					return SendClientMessage(playerid, COLOR_GREY, "The RRK system is disabled.");

				if(hasTempRRK[playerid] == 0)
				{
					hasTempRRK[playerid] = 1;
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have taken a temporary RRK from the lockers.");
					TogglePlayerControllable(playerid, true);
				}
				else return SendClientMessage(playerid, COLOR_GREY, "You already have a temporary RRK.");
			}
			if(listitem == 4) //radio
			{
				if(PlayerInfo[playerid][pCash] < 150)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					PlayerInfo[playerid][pRadio] = 1;
					GiveMoney(playerid, -150);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 5)
			{
				if(PlayerInfo[playerid][pCash] < 150)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					format(str, sizeof(str), "* %s %s has used a first aid kit.", GetPlayerRank(playerid), PlayerICName(playerid));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					GiveMoney(playerid, -200);
					SetPlayerHealth(playerid, 100.0);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 6) // portable mdc
			{
				PlayerInfo[playerid][pPortMDC] = 1;
				SendClientMessage(playerid, COLOR_ORANGE, "You take a portable MDC unit from the locker.");
				SendClientMessage(playerid, COLOR_ORANGE, "(( This will be lost upon death. ))");
			}
		}
		else
		{
			ShowPlayerDialog(playerid, DIALOG_LSFMD_MAIN, DIALOG_STYLE_LIST, "SAFMD", "Duty\nChange Uniform\nOff Duty\nGear", "Select", "Cancel");
		}
		return 1;
	}
	if(dialogid == SANEWSLOCKER)
	{
		if(response)
		{
			if(listitem == 0)
			{
				if(PlayerInfo[playerid][pOnDuty] == 0)
				{
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					format(str, sizeof(str), "* %s %s takes their Press Pass and camera from their locker.", GetPlayerRank(playerid), sendername);
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					PlayerInfo[playerid][pOnDuty] = 1;
					SetPlayerColor(playerid, TCOLOR_LIGHTBLUE);
				}
				else if(PlayerInfo[playerid][pOnDuty] == 1)
				{
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					format(str, sizeof(str), "* %s %s places their Press Pass and Camera in their locker.", GetPlayerRank(playerid), sendername);
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					PlayerInfo[playerid][pOnDuty] = 0;
					SetPlayerColor(playerid, TCOLOR_WHITE);
					SetPlayerSkin(playerid, PlayerInfo[playerid][pModel]);
					PlayerInfo[playerid][pClothes] = 0;
				}
				return 1;
			}
			if(listitem == 1)
			{
				// Show civilian skin selector
				ShowSkinShop(playerid, true, false, false, false, false);
				return 1;
			}
			if(listitem == 2)
			{
				ShowPlayerDialog(playerid, SANEWSGUNS, DIALOG_STYLE_LIST, "Gear", "Camera\t\t$250\nKevlar Vest \t$500\nRadio\t\t$150\nMace\t\t$25\nFirst Aid Kit \t$150", "Select", "Back");
				return 1;
			}
		}
		return 1;
	}
	if(dialogid == SANEWSGUNS)
	{
		if(response)
		{
			if(listitem == 0) // CAMERA
			{
				if(PlayerInfo[playerid][pCash] < 250)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
				}
				else
				{
					GiveMoney(playerid, -250);
					GivePlayerGun(playerid, 43);
					TogglePlayerControllable(playerid, true);
				}
				return 1;
			}
			if(listitem == 1) // Kevlar Vest
			{
				if(PlayerInfo[playerid][pCash] < 500)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
				}
				else
				{
					GiveMoney(playerid, -500);
					SetPlayerArmour(playerid, 100);
					TogglePlayerControllable(playerid, true);
				}
				return 1;
			}
			if(listitem == 2) //radio
			{
				if(PlayerInfo[playerid][pCash] < 150)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					PlayerInfo[playerid][pRadio] = 1;
					GiveMoney(playerid, -150);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 3) //Mace
			{
				if(PlayerInfo[playerid][pCash] < 25)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					GiveMoney(playerid, -25);
					GivePlayerGun(playerid, 41);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 4) // Health
			{
				if(PlayerInfo[playerid][pCash] < 150)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
				}
				else
				{
					GiveMoney(playerid, -150);
					SetPlayerHealth(playerid, 100);
					TogglePlayerControllable(playerid, true);
				}
				return 1;
			}
		}
		else
		{
			ShowPlayerDialog(playerid, SANEWSLOCKER, DIALOG_STYLE_LIST, "SAN", "Duty\nChange Uniform\nGear\n", "Select", "Cancel");
		}
		return 1;
	}
	if(dialogid == 70) //burgershot
	{
		if(response)
		{
			new biz = GetPlayerVirtualWorld(playerid);
			if(strlen(inputtext))
			{
				if(!IsNumeric(inputtext))
				{
					return SendClientMessage(playerid, COLOR_GREY, "You must type a number.");
				}
				if(strcmp(inputtext, "1", true) == 0) //Sprunk
				{
					if(GetMoney(playerid) < 2)
					{
						return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money.");
					}
					SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);
					displayCenterHUDInfo(playerid,  "~r~Sprunk~w~ purchased!~n~type ~y~/usesoda~w~ to use, LMB to drink, press F to drop.", 8); //display for 8 seconds
					GiveMoney(playerid, -2);
					BizInfo[biz][bTill] += 2;
					BizInfo[biz][bProducts] -= 1;
				}
				else if(strcmp(inputtext, "2", true) == 0) //Koolaid
				{
					if(GetMoney(playerid) < 1)
					{
						return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money.");
					}
					SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);
					displayCenterHUDInfo(playerid,  "~r~Koolaid~w~ purchased!~n~type ~y~/usesoda~w~ to use, LMB to drink, press F to drop.", 8); //display for 8 seconds
					GiveMoney(playerid, -1);
					BizInfo[biz][bTill] += 2;
					BizInfo[biz][bProducts] -= 1;
				}
				else if(strcmp(inputtext, "3", true) == 0) //Largo Combo
				{
					if(boughtFoodTimer[playerid])
					{
						return SendClientMessage(playerid, COLOR_GREY, "You must wait 1 minute before buying another Meal.");
					}
					if(GetMoney(playerid) < 25)
					{
						return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money.");
					}
					if(hasFoodTray[playerid])
					{
						return SendClientMessage(playerid, COLOR_GREY, "You are already holding a meal, type /preparefood then type /eat to eat it.");
					}
					format(str, sizeof(str), "* %s has bought a Meat Stack Meal.", PlayerICName(playerid));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					displayCenterHUDInfo(playerid,  "~r~Meat Stack Meal~w~ purchased!~n~type ~y~/preparefood~w~ to prepare your meal~n~~w~then ~y~/eat~w~ to eat it.", 8); //display for 8 seconds
					hasFoodTray[playerid] = 3; //burgershot
					givePlayerFoodTray(playerid, hasFoodTray[playerid]);
					GiveMoney(playerid, -25);
					BizInfo[biz][bTill] += 2;
					BizInfo[biz][bProducts] -= 1;
					SetTimerEx("boughtFood", 60 *1000, 0, "i", playerid); //1 minute
					boughtFoodTimer[playerid] = true;
				}
				else if(strcmp(inputtext, "4", true) == 0) //Medium Combo
				{
					return SendClientMessage(playerid, COLOR_GREY, "This meal will be available soon.");
				}
				else if(strcmp(inputtext, "5", true) == 0) //Small Combo
				{
					return SendClientMessage(playerid, COLOR_GREY, "This meal will be available soon.");
				}
				else if(strcmp(inputtext, "6", true) == 0) //Salad Meal
				{
					return SendClientMessage(playerid, COLOR_GREY, "This meal will be available soon.");
				}
				if(BizInfo[biz][bProducts] <= 0) SetNoProductsDate(biz);
			}
		}
		return 1;
	}
	if(dialogid == 71) //cluckin bell
	{
		if(response)
		{
			new biz = GetPlayerVirtualWorld(playerid);
			if(strlen(inputtext))
			{
				if(!IsNumeric(inputtext))
				{
					return SendClientMessage(playerid, COLOR_GREY, "You must type a number.");
				}
				if(strcmp(inputtext, "1", true) == 0) //Sprunk
				{
					if(GetMoney(playerid) < 2)
					{
						return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money.");
					}
					SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);
					displayCenterHUDInfo(playerid,  "~r~Sprunk~w~ purchased!~n~type ~y~/usesoda~w~ to use, LMB to drink, press F to drop.", 8); //display for 8 seconds
					GiveMoney(playerid, -2);
					BizInfo[biz][bTill] += 2;
					BizInfo[biz][bProducts] -= 1;
				}
				else if(strcmp(inputtext, "2", true) == 0) //Koolaid
				{
					if(GetMoney(playerid) < 1)
					{
						return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money.");
					}
					SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);
					displayCenterHUDInfo(playerid,  "~r~Koolaid~w~ purchased!~n~type ~y~/usesoda~w~ to use, LMB to drink, press F to drop.", 8); //display for 8 seconds
					GiveMoney(playerid, -1);
					BizInfo[biz][bTill] += 2;
					BizInfo[biz][bProducts] -= 1;
				}
				else if(strcmp(inputtext, "3", true) == 0) //Largo Combo
				{
					if(boughtFoodTimer[playerid])
					{
						return SendClientMessage(playerid, COLOR_GREY, "You must wait 1 minute before buying another Meal.");
					}
					if(GetMoney(playerid) < 25)
					{
						return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money.");
					}
					if(hasFoodTray[playerid])
					{
						return SendClientMessage(playerid, COLOR_GREY, "You are already holding a meal, type /preparefood then type /eat to eat it.");
					}
					format(str, sizeof(str), "* %s has bought a Cluckin' Huge Meal.", PlayerICName(playerid));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					displayCenterHUDInfo(playerid,  "~r~Cluckin' Huge Meal~w~ purchased!~n~type ~y~/preparefood~w~ to prepare your meal~n~~w~then ~y~/eat~w~ to eat it.", 8); //display for 8 seconds
					hasFoodTray[playerid] = 2; //chicken
					givePlayerFoodTray(playerid, hasFoodTray[playerid]);
					GiveMoney(playerid, -25);
					BizInfo[biz][bTill] += 2;
					BizInfo[biz][bProducts] -= 1;
					SetTimerEx("boughtFood", 60 *1000, 0, "i", playerid); //1 minute
					boughtFoodTimer[playerid] = true;
				}
				else if(strcmp(inputtext, "4", true) == 0) //Medium Combo
				{
					return SendClientMessage(playerid, COLOR_GREY, "This meal will be available soon.");
				}
				else if(strcmp(inputtext, "5", true) == 0) //Small Combo
				{
					return SendClientMessage(playerid, COLOR_GREY, "This meal will be available soon.");
				}
				else if(strcmp(inputtext, "6", true) == 0) //Salad Meal
				{
					return SendClientMessage(playerid, COLOR_GREY, "This meal will be available soon.");
				}
				if(BizInfo[biz][bProducts] <= 0)
				{
					SetNoProductsDate(biz);
				}
			}
		}
		return 1;
	}
	if(dialogid == 72) //pizza stack
	{
		if(response)
		{
			new biz = GetPlayerVirtualWorld(playerid);
			if(strlen(inputtext))
			{
				if(!IsNumeric(inputtext))
				{
					return SendClientMessage(playerid, COLOR_GREY, "You must type a number.");
				}
				if(strcmp(inputtext, "1", true) == 0) //Sprunk
				{
					if(GetMoney(playerid) < 2)
					{
						return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money.");
					}
					SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);
					displayCenterHUDInfo(playerid,  "~r~Sprunk~w~ purchased!~n~type ~y~/usesoda~w~ to use, LMB to drink, press F to drop.", 8); //display for 8 seconds
					GiveMoney(playerid, -2);
					BizInfo[biz][bTill] += 2;
					BizInfo[biz][bProducts] -= 1;
				}
				else if(strcmp(inputtext, "2", true) == 0) //Lemonade
				{
					if(GetMoney(playerid) < 1)
					{
						return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money.");
					}
					SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);
					displayCenterHUDInfo(playerid,  "~r~Lemonade~w~ purchased!~n~type ~y~/usesoda~w~ to use, LMB to drink, press F to drop.", 8); //display for 8 seconds
					GiveMoney(playerid, -1);
					BizInfo[biz][bTill] += 2;
					BizInfo[biz][bProducts] -= 1;
				}
				else if(strcmp(inputtext, "3", true) == 0) //Pizza Take Out
				{
					if(boughtFoodTimer[playerid])
					{
						return SendClientMessage(playerid, COLOR_GREY, "You must wait 1 minute before buying another Meal.");
					}
					if(GetMoney(playerid) < 100)
					{
						return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money.");
					}
					if(hasFoodTray[playerid])
					{
						return SendClientMessage(playerid, COLOR_GREY, "You are already holding a meal, type /preparefood then type /eat to eat it.");
					}
					format(str, sizeof(str), "* %s has bought a Pizza Take Out Meal.", PlayerICName(playerid));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					displayCenterHUDInfo(playerid,  "~r~Pizza Take Out Meal~w~ purchased!~n~type ~y~/preparefood~w~ to prepare your meal~n~~w~then ~y~/eat~w~ to eat it.", 8); //display for 8 seconds
					hasFoodTray[playerid] = 1; //pizza
					givePlayerFoodTray(playerid, hasFoodTray[playerid]);
					GiveMoney(playerid, -100);
					BizInfo[biz][bTill] += 2;
					BizInfo[biz][bProducts] -= 1;
					SetTimerEx("boughtFood", 60 *1000, 0, "i", playerid); //1 minute
					boughtFoodTimer[playerid] = true;
				}
				else if(strcmp(inputtext, "4", true) == 0) //Full Rack
				{
					return SendClientMessage(playerid, COLOR_GREY, "This meal will be available soon.");
				}
				else if(strcmp(inputtext, "5", true) == 0) //Double D-Luxe
				{
					return SendClientMessage(playerid, COLOR_GREY, "This meal will be available soon.");
				}
				else if(strcmp(inputtext, "6", true) == 0) //Buster
				{
					return SendClientMessage(playerid, COLOR_GREY, "This meal will be available soon.");
				}
				else if(strcmp(inputtext, "7", true) == 0) //Salad Meal
				{
					return SendClientMessage(playerid, COLOR_GREY, "This meal will be available soon.");
				}
				if(BizInfo[biz][bProducts] <= 0)
				{
					SetNoProductsDate(biz);
				}
			}
		}
		return 1;
	}
	if(dialogid == 75) // Product Dealer
	{
		if(response)
		{
			DisplayDialogForPlayer(playerid, 74);
			return 1;
		}
		return 1;
	}
	if(dialogid == 74) // Product Dealer
	{
		if(response)
		{
			DisplayDialogForPlayer(playerid, 75);
			return 1;
		}
		return 1;
	}
	if(dialogid == 76) //sex shop
	{
		if(response)
		{
			new biz = GetPlayerVirtualWorld(playerid);

			if(BizInfo[biz][bProducts] <= 0)
				return SendClientMessage(playerid, COLOR_GREY, "This business doesn't have any products left.");

			if(listitem == 0) //nitestick
			{
				if(PlayerInfo[playerid][pCash] >= 7000)
				{
					GiveMoney(playerid, -7000);
					BizInfo[biz][bTill] += 14000;
					BizInfo[biz][bProducts] -= 1;
					GivePlayerGun(playerid, 3);
					format(str, sizeof(str), "~r~-$%d", 7000);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Nitestick~w~ purchased!~n~enjoy.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 1) //condom
			{
				if(PlayerInfo[playerid][pCash] > 500)
				{
					GiveMoney(playerid, -500);
					BizInfo[biz][bTill] += 500;
					BizInfo[biz][bProducts] -= 1;
					Condom[playerid] ++;
					format(str, sizeof(str), "~r~-$%d", 500);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Condom~w~ purchased!~n~~y~Condoms~w~ will automatically be used when you have sex.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 2) //dildo
			{
				if(PlayerInfo[playerid][pCash] >= 2000)
				{
					GiveMoney(playerid, -2000);
					BizInfo[biz][bTill] += 4000;
					BizInfo[biz][bProducts] -= 1;
					GivePlayerGun(playerid, 10);
					format(str, sizeof(str), "~r~-$%d", 2000);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Purple Dildo~w~ purchased!~n~enjoy.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 3) //vibrator
			{
				if(PlayerInfo[playerid][pCash] >= 4000)
				{
					GiveMoney(playerid, -4000);
					BizInfo[biz][bTill] += 8000;
					BizInfo[biz][bProducts] -= 1;
					GivePlayerGun(playerid, 13);
					format(str, sizeof(str), "~r~-$%d", 4000);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Vibrator~w~ purchased!~n~enjoy.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 4) //small white vibrator
			{
				if(PlayerInfo[playerid][pCash] >= 5000)
				{
					GiveMoney(playerid, -5000);
					BizInfo[biz][bTill] += 10000;
					BizInfo[biz][bProducts] -= 1;
					GivePlayerGun(playerid, 11);
					format(str, sizeof(str), "~r~-$%d", 5000);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Small White Vibrator~w~ purchased!~n~enjoy.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 5) //large white vibrator
			{
				if(PlayerInfo[playerid][pCash] >= 7500)
				{
					GiveMoney(playerid, -7500);
					BizInfo[biz][bTill] += 15000;
					BizInfo[biz][bProducts] -= 1;
					GivePlayerGun(playerid, 12);
					format(str, sizeof(str), "~r~-$%d", 7500);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Large White Vibrator~w~ purchased!~n~enjoy.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 6) //camera
			{
				if(PlayerInfo[playerid][pCash] >= 10000)
				{
					GiveMoney(playerid, -10000);
					BizInfo[biz][bTill] += 20000;
					BizInfo[biz][bProducts] -= 1;
					GivePlayerGun(playerid, 43);
					format(str, sizeof(str), "~r~-$%d", 10000);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Camera~w~ purchased!~n~enjoy.", 8); //display for 8 seconds
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(listitem == 7) //gag
			{
				if(PlayerInfo[playerid][pGag] >= 3)
					return SendClientMessage(playerid, COLOR_GREY, "You already have 3 gags.");

				if(PlayerInfo[playerid][pCash] >= 15000)
				{
					GiveMoney(playerid, -15000);
					BizInfo[biz][bTill] += 30000;
					BizInfo[biz][bProducts] -= 1;
					format(str, sizeof(str), "~r~-$%d", 15000);
					GameTextForPlayer(playerid, str, 5000, 1);
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					displayCenterHUDInfo(playerid,  "~r~Gag~w~ purchased!~n~enjoy.", 8); //display for 8 seconds
					PlayerInfo[playerid][pGag]++;
				}
				else
				{
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
			}
			if(BizInfo[biz][bProducts] <= 0) SetNoProductsDate(biz);
		}
		return 1;
	}
	if(dialogid == 77) //buy clothes
	{
		if(response)
		{
			if(!IsAtClothShop(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You are not at a clothes shop.");
			new biz = GetPlayerVirtualWorld(playerid);
			if(!IsNumeric(inputtext))
			{
				SendClientMessage(playerid, COLOR_GREY, "You must enter a number for the skin ID.");
				return 1;
			}
			if((strval(inputtext) == 294 && PlayerInfo[playerid][pDonateRank] == 0) ||
					(strval(inputtext) == 295 && PlayerInfo[playerid][pDonateRank] == 0) ||
					(strval(inputtext) == 167) ||
					(strval(inputtext) == 0) ||
					(strval(inputtext) >= 265 && strval(inputtext) <= 267) ||
					(strval(inputtext) == 165) ||
					(strval(inputtext) == 166) ||
					(strval(inputtext) >= 280 && strval(inputtext) <= 288) ||
					(strval(inputtext) >= 274 && strval(inputtext) <= 279) ||
					(strval(inputtext) >= 163 && strval(inputtext) <= 166) ||
					(strval(inputtext) >= 268 && strval(inputtext) <= 271) ||
					(strval(inputtext) == 296) ||
					(strval(inputtext) == 71)) return SendClientMessage(playerid, COLOR_GREY, "You are not allowed to use this skin.");
			new skin = strval(inputtext);
			if(skin < 0 || skin > 311) return SendClientMessage(playerid, COLOR_GREY, "Skin can't be below 0 or above 311.");
			PlayerTextDrawDestroy(playerid, pModel_t[playerid]);
			pModelState[playerid] = 999;
			pModel_t[playerid] = CreatePlayerTextDraw(playerid, 220.0, 250.0, "_");
			PlayerTextDrawFont(playerid, pModel_t[playerid], TEXT_DRAW_FONT_MODEL_PREVIEW);
			PlayerTextDrawTextSize(playerid, pModel_t[playerid], 200.0, 200.0);
			PlayerTextDrawSetPreviewModel(playerid, pModel_t[playerid], skin);
			PlayerTextDrawUseBox(playerid, pModel_t[playerid], 0);
			PlayerTextDrawBackgroundColor(playerid, pModel_t[playerid], 0x00000000);
			PlayerTextDrawShow(playerid, pModel_t[playerid]);
			pModelState[playerid] = 2;
			pModelRot[playerid][0] = 0;
			pModelRot[playerid][1] = 0;
			pModelRot[playerid][2] = 0;
			pModelTimer[playerid] = SetTimerEx("UpdatePreviewModel", 1000, 1, "i", playerid);
			ShowPlayerDialog(playerid, 771, DIALOG_STYLE_MSGBOX, "Are you sure?", "Are you sure you would like to buy this skin for $500?\n\n{F5DA81}NOTE: Below is a preview of the skin.", "Purchase", "Cancel");
			SetPVarInt(playerid, "buyskinbincobiz", biz);
			SetPVarInt(playerid, "buyskinbinco", skin);
			return 1;
		}
		return 1;
	}
	if(dialogid == 771)
	{
		new skin = GetPVarInt(playerid, "buyskinbinco");
		if(isRestrictedModel(skin))
		{
			SendClientMessage(playerid, COLOR_GREY, "* You have been caught attempting to exploit.");
			SendClientMessage(playerid, COLOR_GREY, "* Exploiting is against the server rules, please don't do it.");
			format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has attempted to buy a restricted skin.", PlayerName(playerid));
			ABroadCast(COLOR_WHITE, str, 1);
			return 1;
		} 
		new biz = GetPVarInt(playerid, "buyskinbincobiz");
		PlayerTextDrawDestroy(playerid, pModel_t[playerid]);
		pModelState[playerid] = 999;
		if(!response)
		{
			DeletePVar(playerid, "buyskinbinco");
			DeletePVar(playerid, "buyskinbincobiz");
			return 1;
		}
		else
		{
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You bought a new set of clothes for $1000.");
			GiveMoney(playerid, -1000);
			BizInfo[biz][bTill] += 1000;
			BizInfo[biz][bProducts] -= 1;
			SetPlayerSkin(playerid, skin);
			PlayerInfo[playerid][pClothes] = 0;
			PlayerInfo[playerid][pModel] = skin;
			DeletePVar(playerid, "buyskinbinco");
			DeletePVar(playerid, "buyskinbincobiz");
		}
		if(BizInfo[biz][bProducts] <= 0)
		{
			SetNoProductsDate(biz);
		}
		return 1;
	}
	if(dialogid == 78) //admin authorization
	{
		if(gAdminAuthorized[playerid] == 1)
		{
			SendClientMessage(playerid, COLOR_WHITE, "{F00000}SERVER{FFFFFF}: Your admin account has already been authorized.");
			return 1;
		}
		if(response)
		{
			if(!strlen(inputtext))
			{
				DisplayDialogForPlayer(playerid, 78); //admin authorization
				SendClientMessage(playerid, COLOR_WHITE, "{F00000}SERVER{FFFFFF}: You must enter your security code.");
				return 1;
			}
			if(strlen(inputtext) >= 50)
			{
				DisplayDialogForPlayer(playerid, 78); //admin authorization
				SendClientMessage(playerid, COLOR_WHITE, "{F00000}SERVER{FFFFFF}: Security code is too long.");
				return 0;
			}
			if(PlayerInfo[playerid][pSecKey] != strval(inputtext))
			{
				SendClientMessage(playerid, COLOR_WHITE, "{F00000}SERVER{FFFFFF}: Security Key does not match. You have been kicked as a result.");
				KickEx(playerid);
				return 1;
			}
			else
			{
				gAdminAuthorized[playerid] = 1;
				SendClientMessage(playerid, COLOR_WHITE, "{F00000}SERVER{FFFFFF}: Your admin account has successfully been authorized.");
				if(!strmatch(adminMOTD, "None"))
				{
					format(str, sizeof(str), "Admin MOTD: %s", adminMOTD);
					SendClientMessage(playerid, COLOR_YELLOW, str);
				}
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_WHITE, "{F00000}SERVER{FFFFFF}: You have left the server.");
			KickEx(playerid);
		}
		return 1;
	}
	if(dialogid == 79) // Help
	{
		if(response)
		{
			if(listitem == 0) // Account
			{
				new hstr[1024];
				format(hstr, sizeof(hstr), "{007BD0}Account Help:{B4B5B7}\n", hstr);
				format(hstr, sizeof(hstr), "%sThe following commands are related to your player account:\n", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/stats{B4B5B7} - Displays your player statistics, such as your level/money/job/faction.", hstr);				
				format(hstr, sizeof(hstr), "%s\n{007BD0}/skills{B4B5B7} - Displays your job skill levels, these are seperate from your main level and can be leveled by doing jobs.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/buylevel{B4B5B7} - Upgrades your main level, leveling up means higher paychecks and offers many perks.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/upgrade{B4B5B7} - Gives your account special perks such as higher spawn health/armor.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/changepass{B4B5B7} - Changes your login password, do not to use the same password on other servers.", hstr);
				ShowPlayerDialog(playerid, 80, DIALOG_STYLE_MSGBOX, "Account", hstr, "Ok", "Back");
				return 1;
			}
			if(listitem == 1) // General
			{
				new hstr[4000];
				format(hstr, sizeof(hstr), "{007BD0}General Help:{B4B5B7}\n", hstr);
				format(hstr, sizeof(hstr), "%sThe following commands are general commands:\n", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/pay{B4B5B7} - Pay money to another player.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/wiretransfer{B4B5B7} - Transfer money to another players bank account.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/(ad)vertise{B4B5B7} - Creates an advertisment for the whole server (useful for buying or selling something).", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/give{B4B5B7} - Gives something to another player.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/drop{B4B5B7} - Lets you drop things, such as weapons, drugs.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/get{B4B5B7} - Lets you pickup things which have been dropped.", hstr);
				//format(hstr, sizeof(hstr), "%s\n{007BD0}/pickpocket{B4B5B7} - Steal things from other players, useful if you are roleplaying a thief.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/tie{B4B5B7} - Ties up a player (you must be driving and they must be in your vehicle).", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/untie{B4B5B7} - Unties a player, a player will automatically untie after 5 minutes of using /tie.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/blindfold{B4B5B7} - Lets you blindfold another player, preventing them from seeing anything.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/unblindfold{B4B5B7} - Removes a blindfold from a player.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/frisk{B4B5B7} - Frisks a player to show you what they are carrying.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/time{B4B5B7} - Displays current server time, when used in jail it will display your remaining jail time.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/eat{B4B5B7} - Eats from a foodtray.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/animlist{B4B5B7} - Displays a list of animations (useful for roleplaying).", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/contract{B4B5B7} - Places a hit on someone, when a hitman kills them they lose half of the contract price.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/flipcoin{B4B5B7} - Flips a coin.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/rules{B4B5B7} - Shows the rules of the server.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/breakin{B4B5B7} - Used to break in to houses, businesses and player owned vehicles.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/id{B4B5B7} - Displays a players id, name, ping.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/cancel{B4B5B7} - Can be used to cancel offers or things that another player might be offering you.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/kill{B4B5B7} - Sends you on a trip to hospital.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/eject{B4B5B7} - Ejects a player from your vehicle if you are the driver.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/carradio{B4B5B7} - Changes the car radio.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/hood{B4B5B7} - Open/close the hood of a vehicle.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/trunk{B4B5B7} - Open/close the trunk of a vehicle.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/lights{B4B5B7} - Turns the lights off/on on any vehicle.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/admins{B4B5B7} - Displays a list of admins online.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/report{B4B5B7} - Sends a message to any admins.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/helpers{B4B5B7} - Displays a list of helpers online.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/helpme{B4B5B7} - Sends a message to any helpers.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/clearmychat{B4B5B7} - Clear your own chat.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/usedrug{B4B5B7} - Uses the specified drug..", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/sex{B4B5B7} - Offer to have sex with another player.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/service{B4B5B7} - Contact the local services (EMS, Mechanic, Police)", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/points{B4B5B7} - Displays a list of capturable businesses, gangs who control them get money when they're used.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/families{B4B5B7} - Displays a list of gangs online.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/pickweed{B4B5B7} - Pick from a weed plant.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/checkweed{B4B5B7} - Shows how much weed a weed plant has produced.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/getdrink{B4B5B7} - Allows you to purchase a drink.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/requestnewbieunmute{B4B5B7} - Request an unmute from the newbie chat channel.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/requestadunmute{B4B5B7} - Request an unmute from the advertisement system.", hstr);
				//format(hstr, sizeof(hstr), "%s\n{007BD0}/cancelreport{B4B5B7} - Cancel your current active report.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/inmates{B4B5B7} - Check who is in the IC prison.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/blocknumber{B4B5B7} - Block a number from calling/SMS'ing you.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/unblocknumber{B4B5B7} - Unblock a number you have previously stopped from calling/SMS'ing you.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/blocklist{B4B5B7} - View a list of all number you have blocked.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/factions{B4B5B7} - View a list of all factions, and how many members are online.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/lightfire{B4B5B7} - Light a camp fire.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/putoutfire{B4B5B7} - Put out your camp fire.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/removebug{B4B5B7} - Removes a surveillance bug from another player.", hstr);
				ShowPlayerDialog(playerid, 81, DIALOG_STYLE_MSGBOX, "General", hstr, "Ok", "Back");
				return 1;
			}
			if(listitem == 2) // Chat
			{
				new hstr[1024];
				format(hstr, sizeof(hstr), "{007BD0}Chat Help:{B4B5B7}\n", hstr);
				format(hstr, sizeof(hstr), "%sThe following commands are related to interaction or chat:\n", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/me{B4B5B7} - For performing actions in character.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/do{B4B5B7} - For performing actions in character.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/whisper{B4B5B7} - Whispers to other players in range.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}(/o)oc{B4B5B7} - Speaks in the global out of character chat channel.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}(/s)hout{B4B5B7} - Speaks at a bigger range.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}(/l)ow{B4B5B7} - Speaks at a smaller range.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/b{B4B5B7} - This is for speaking out of character locally.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/intercom{B4B5B7} - Speaks to passengers if you are using a vehicle with an intercom (shamal/cargobob).", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/speakerphone{B4B5B7} - Allows you to turn on the phones speaker while taking a call.", hstr);
				ShowPlayerDialog(playerid, 82, DIALOG_STYLE_MSGBOX, "Chat", hstr, "Ok", "Back");
				return 1;
			}
			if(listitem == 3) // Toggle
			{
				new hstr[1024];
				format(hstr, sizeof(hstr), "{007BD0}Toggle Help:{B4B5B7}\n", hstr);
				format(hstr, sizeof(hstr), "%sThe following commands are for toggling things on or off:\n", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/togooc{B4B5B7} - Toggles on/off the global out of character chat.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/tognewbie{B4B5B7} - Toggles on/off the newbie chat.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/togfam{B4B5B7} - Toggles on/off the family chat.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/togwhisper{B4B5B7} - Toggles on/off whisper chat.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/togphone{B4B5B7} - Toggles on/off your cellphone.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/togwt{B4B5B7} - Toggles on/off walkie talkie chat.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/toghelper{B4B5B7} - Toggles on/off helper chat.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/togfl{B4B5B7} - Toggles on/off faction leader chat.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/togtester{B4B5B7} - Toggles on/off the beta tester chat.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/togautochat{B4B5B7} - Toggles on/off automatic chat animations when typing on foot or in vehicles.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/tognames{B4B5B7} - Toggles on/off nametags.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/togradios{B4B5B7} - Toggles on/off radio streams from boomboxes and car radios.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/togfuelhud{B4B5B7} - Toggles on/off the vehicle fuel hud.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/togspeedhud{B4B5B7} - Toggles on/off the vehicle speed hud.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/togvhealthhud{B4B5B7} - Toggles on/off the vehicle health hud.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/togdept{B4B5B7} - Toggles on/off the department radio chat.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/togradio{B4B5B7} - Toggles on/off the faction radio chat.", hstr);
				ShowPlayerDialog(playerid, 83, DIALOG_STYLE_MSGBOX, "Toggle", hstr, "Ok", "Back");
				return 1;
			}
			if(listitem == 4) // Bank
			{
				new hstr[1024];
				format(hstr, sizeof(hstr), "{007BD0}Bank Help:{B4B5B7}\n", hstr);
				format(hstr, sizeof(hstr), "%sThe following commands are for banking:\n", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/withdraw{B4B5B7} - Withdraws money from your bank account.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/deposit{B4B5B7} - Deposits money to your bank account.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/atm{B4B5B7} - Lets you withdraw money from atm machines.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/wiretransfer{B4B5B7} - Transfers money to another persons bank account.", hstr);
				ShowPlayerDialog(playerid, 84, DIALOG_STYLE_MSGBOX, "Bank", hstr, "Ok", "Back");
				return 1;
			}
			if(listitem == 5) // Job
			{
				ShowPlayerDialog(playerid, DIALOG_JOBHELP, DIALOG_STYLE_LIST, "Job Help", "Detective\nLawyer\nDrug Dealer\nMechanic\nBodyguard\nArms Dealer\nTaxi Driver\nDrug Smuggler\nWaste Collector\nPizza Deliver", "Ok", "Back");
				return 1;
			}
			if(listitem == 6) // Faction
			{
				if(PlayerInfo[playerid][pMember] == 0)
				{
					ShowHelpDialog(playerid);
					SendClientMessage(playerid, COLOR_GREY, "You are not a member of a Faction.");
					return 1;
				}
				if(PlayerInfo[playerid][pMember] == 1 || PlayerInfo[playerid][pLeader] == 1) // SAPD
				{
					new hstr[1800];
					format(hstr, sizeof(hstr), "{007BD0}Faction Help:{B4B5B7}\n", hstr);
					format(hstr, sizeof(hstr), "%sThe following commands are specific to your faction:\n", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/r)adio{B4B5B7} - Types a message in radio chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/d)epartments{B4B5B7} - Types a message in departments chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/m)egaphone{B4B5B7} - Types a message in the megaphone.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/su)spect{B4B5B7} - Make a player a suspect.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/SAPD{B4B5B7} - Used to go on duty.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/cuff{B4B5B7} - Cuffs a player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/badge{B4B5B7} - Hide or show your badge.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/detain{B4B5B7} - Detains a player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/sdtazer{B4B5B7} - Equips tazer gun.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/handtazer{B4B5B7} - Equips stun weapon.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/showbadge{B4B5B7} - Shows your badge to another player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/take{B4B5B7} - Takes things from another player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/ticket{B4B5B7} - Offers another player a ticket.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/gov)ernment{B4B5B7} - Displays a government message to the server.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/deliver{B4B5B7} - Delivers a player to prison.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/clothes{B4B5B7} - Used to change clothes.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(un)invite{B4B5B7} - Uninvites a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/ouninvite{B4B5B7} - Uninvites a member of your faction that is offline.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/giverank{B4B5B7} - Sets the rank of a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/deployspikes{B4B5B7} - Deploys road spikes.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/deletespike(s){B4B5B7} - Deletes road spikes.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/deployroadblock{B4B5B7} - Deploys roadblock.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/deleteroadblock(s){B4B5B7} - Deletes roadblocks.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/opencell{B4B5B7} - Opens nearby jail cells.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(f)action(l)eaders{B4B5B7} - Communicate to other leaders and faction moderators.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/facwithdraw{B4B5B7} - Withdraws money from your factions bank account.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/pdcalls{B4B5B7} - Show a list of 911 calls that have occurred.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/flashlight{B4B5B7} - Give functionality for the flashlight system.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/dlock{B4B5B7} - Lock a dynamic vehicle with a lock enabled.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/towcars{B4B5B7} - Respawns all faction vehicles.", hstr);
					ShowPlayerDialog(playerid, 85, DIALOG_STYLE_MSGBOX, "Faction", hstr, "Ok", "Back");
					return 1;
				}
				if(PlayerInfo[playerid][pMember] == 2 || PlayerInfo[playerid][pLeader] == 2)   //fbi
				{
					new hstr[2500];
					format(hstr, sizeof(hstr), "{007BD0}Faction Help:{B4B5B7}\n", hstr);
					format(hstr, sizeof(hstr), "%sThe following commands are specific to your faction:\n", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/r)adio{B4B5B7} - Types a message in radio chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/d)epartments{B4B5B7} - Types a message in departments chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/m)egaphone{B4B5B7} - Types a message in the megaphone.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/fbi{B4B5B7} - Used to go on duty.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/fbiuc{B4B5B7} - Request a new undercover identity.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/cuff{B4B5B7} - Cuffs a player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/detain{B4B5B7} - Detains a player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/sdtazer{B4B5B7} - Equips tazer gun.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/handtazer{B4B5B7} - Equips stun weapon.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/showbadge{B4B5B7} - Shows your badge to another player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/take{B4B5B7} - Takes things from another player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/ticket{B4B5B7} - Offers another player a ticket.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/gov)ernment{B4B5B7} - Displays a government message to the server.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/clothes{B4B5B7} - Used to change clothes.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(un)invite{B4B5B7} - Uninvites a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/ouninvite{B4B5B7} - Uninvites a member of your faction that is offline.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/giverank{B4B5B7} - Sets the rank of a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/deployspikes{B4B5B7} - Deploys road spikes.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/deletespike(s){B4B5B7} - Deletes road spikes.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/deployroadblock{B4B5B7} - Deploys roadblock.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/deleteroadblock(s){B4B5B7} - Deletes roadblocks.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/sate{B4B5B7} - Enables satellite mode while in a surveillance van.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/badge{B4B5B7} - Hide or show your badge.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/seizebiz{B4B5B7} - Shuts down a business.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/opencell{B4B5B7} - Opens nearby jail cells.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(f)action(l)eaders{B4B5B7} - Communicate to other leaders and faction moderators.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/facwithdraw{B4B5B7} - Withdraws money from your factions bank account.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/dlock{B4B5B7} - Lock a dynamic vehicle with a lock enabled.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/plantbug{B4B5B7} - Plants a bug on a player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/removebug{B4B5B7} - Removes a surveillance bug from another player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/towcars{B4B5B7} - Respawns all faction vehicles.", hstr);
					ShowPlayerDialog(playerid, 85, DIALOG_STYLE_MSGBOX, "Faction", hstr, "Ok", "Back");
					return 1;
				}
				if(PlayerInfo[playerid][pMember] == 3 || PlayerInfo[playerid][pLeader] == 3)   //SASD
				{
					new hstr[1800];
					format(hstr, sizeof(hstr), "{007BD0}Faction Help:{B4B5B7}\n", hstr);
					format(hstr, sizeof(hstr), "%sThe following commands are specific to your faction:\n", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/r)adio{B4B5B7} - Types a message in radio chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/d)epartments{B4B5B7} - Types a message in departments chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/m)egaphone{B4B5B7} - Types a message in the megaphone.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/su)spect{B4B5B7} - Make a player a suspect.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/sasd{B4B5B7} - Used to go on duty.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/cuff{B4B5B7} - Cuffs a player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/detain{B4B5B7} - Detains a player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/sdtazer{B4B5B7} - Equips tazer gun.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/handtazer{B4B5B7} - Equips stun weapon.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/showbadge{B4B5B7} - Shows your badge to another player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/take{B4B5B7} - Takes things from another player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/ticket{B4B5B7} - Offers another player a ticket.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/gov)ernment{B4B5B7} - Displays a government message to the server.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/deliver{B4B5B7} - Delivers a player to prison.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/clothes{B4B5B7} - Used to change clothes.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(un)invite{B4B5B7} - Uninvites a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/ouninvite{B4B5B7} - Uninvites a member of your faction that is offline.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/giverank{B4B5B7} - Sets the rank of a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/deployspikes{B4B5B7} - Deploys road spikes.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/deletespike(s){B4B5B7} - Deletes road spikes.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/deployroadblock{B4B5B7} - Deploys roadblock.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/deleteroadblock(s){B4B5B7} - Deletes roadblocks.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/opencell{B4B5B7} - Opens nearby jail cells.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(f)action(l)eaders{B4B5B7} - Communicate to other leaders and faction moderators.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/facwithdraw{B4B5B7} - Withdraws money from your factions bank account.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/pdcalls{B4B5B7} - Show a list of 911 calls that have occurred.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/flashlight{B4B5B7} - Give functionality for the flashlight system.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/dlock{B4B5B7} - Lock a dynamic vehicle with a lock enabled.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/towcars{B4B5B7} - Respawns all faction vehicles.", hstr);
					ShowPlayerDialog(playerid, 85, DIALOG_STYLE_MSGBOX, "Faction", hstr, "Ok", "Back");
					return 1;
				}
				if(PlayerInfo[playerid][pMember] == 4 || PlayerInfo[playerid][pLeader] == 4) // LSFMD
				{
					new hstr[1500];
					format(hstr, sizeof(hstr), "{007BD0}Faction Help:{B4B5B7}\n", hstr);
					format(hstr, sizeof(hstr), "%sThe following commands are specific to your faction:\n", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/r)adio{B4B5B7} - Types a message in radio chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/d)epartments{B4B5B7} - Types a message in departments chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/m)egaphone{B4B5B7} - Types a message in the megaphone.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/SAFMD{B4B5B7} - Used to go on duty.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/EMScalls{B4B5B7} - Lists all injured people.", hstr);
					if(!FMD_RRK_DISABLED) {
						format(hstr, sizeof(hstr), "%s\n{007BD0}/getrrk{B4B5B7} - Gets a rapid response kit.", hstr);
						format(hstr, sizeof(hstr), "%s\n{007BD0}/droprrk{B4B5B7} - Drops a rapid response kit.", hstr);
						format(hstr, sizeof(hstr), "%s\n{007BD0}/pickrrk{B4B5B7} - Picks up a rapid response kit.", hstr);
						format(hstr, sizeof(hstr), "%s\n{007BD0}/userrk{B4B5B7} - Uses a rapid response kit.", hstr);
						format(hstr, sizeof(hstr), "%s\n{007BD0}/placerrk{B4B5B7} - Places a rapid response kit.", hstr);
						format(hstr, sizeof(hstr), "%s\n{007BD0}/usetemprrk{B4B5B7} - Uses a temporary RRK on a person (must have one first)", hstr);
					}
					else {
						format(hstr, sizeof(hstr), "%s\n{007BD0}/transferpatient{B4B5B7} - Transfers a patient into the hospital.", hstr);
						format(hstr, sizeof(hstr), "%s\n{007BD0}/releasepatient{B4B5B7} - Releases a patient from the hospital.", hstr);
						format(hstr, sizeof(hstr), "%s\n{007BD0}/morphine{B4B5B7} - Injects morphine into a patient.", hstr);
						format(hstr, sizeof(hstr), "%s\n{007BD0}/checkpatient{B4B5B7} - Checks the pulse of a patient.", hstr);
					}
					format(hstr, sizeof(hstr), "%s\n{007BD0}/stretcher{B4B5B7} - Places a person on a stretcher.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/clothes{B4B5B7} - Used to change clothes.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(un)invite{B4B5B7} - Uninvites a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/ouninvite{B4B5B7} - Uninvites a member of your faction that is offline.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/giverank{B4B5B7} - Sets the rank of a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/showbadge{B4B5B7} - Shows your badge to another player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(f)action(l)eaders{B4B5B7} - Communicate to other leaders and faction moderators.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/facwithdraw{B4B5B7} - Withdraws money from your factions bank account.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/dlock{B4B5B7} - Lock a dynamic vehicle with a lock enabled.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/gml{B4B5B7} - Issue a marijuana license.", hstr);
					ShowPlayerDialog(playerid, 85, DIALOG_STYLE_MSGBOX, "Faction", hstr, "Ok", "Back");
					format(hstr, sizeof(hstr), "%s\n{007BD0}/towcars{B4B5B7} - Respawns all faction vehicles.", hstr);
					return 1;
				}
				if(PlayerInfo[playerid][pMember] == 14 || PlayerInfo[playerid][pLeader] == 14) // SANG
				{
					new hstr[1800];
					format(hstr, sizeof(hstr), "{007BD0}Faction Help:{B4B5B7}\n", hstr);
					format(hstr, sizeof(hstr), "%sThe following commands are specific to your faction:\n", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/r)adio{B4B5B7} - Types a message in radio chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/d)epartments{B4B5B7} - Types a message in departments chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/m)egaphone{B4B5B7} - Types a message in the megaphone.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/gov)ernment{B4B5B7} - Displays a government message to the server.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/sang{B4B5B7} - SANG lockers.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/cuff{B4B5B7} - Cuffs a player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/detain{B4B5B7} - Detains a player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/sdtazer{B4B5B7} - Equips tazer gun.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/handtazer{B4B5B7} - Equips stun weapon.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/showbadge{B4B5B7} - Shows your badge to another player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(un)invite{B4B5B7} - Uninvites a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/ouninvite{B4B5B7} - Uninvites a member of your faction that is offline.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/giverank{B4B5B7} - Sets the rank of a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/deployspikes{B4B5B7} - Deploys road spikes.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/deletespike(s){B4B5B7} - Deletes road spikes.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/deployroadblock{B4B5B7} - Deploys roadblock.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/deleteroadblock(s){B4B5B7} - Deletes roadblocks.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(f)action(l)eaders{B4B5B7} - Communicate to other leaders and faction moderators.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/dlock{B4B5B7} - Lock a dynamic vehicle with a lock enabled.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/registerprisoner{B4B5B7} - Register prisoner.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/prisonlist{B4B5B7} - List prisoner.", hstr);
					ShowPlayerDialog(playerid, 85, DIALOG_STYLE_MSGBOX, "Faction", hstr, "Ok", "Back");
					return 1;
				}
				if(PlayerInfo[playerid][pMember] == 6 || PlayerInfo[playerid][pLeader] == 6) //Senate
				{
					new hstr[1536];
					format(hstr, sizeof(hstr), "{007BD0}Faction Help:{B4B5B7}\n", hstr);
					format(hstr, sizeof(hstr), "%sThe following commands are specific to your faction:\n", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/gov)ernment{B4B5B7} - Public Service Announcement.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/r)adio{B4B5B7} - Types a message in radio chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/d)epartments{B4B5B7} - Types a message in departments chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/setbadgenumber{B4B5B7} - Setup your badge number.", hstr);					
					format(hstr, sizeof(hstr), "%s\n{007BD0}/govlocker{B4B5B7} - Used to go on duty.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/settax{B4B5B7} - Sets the income tax.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/taxwithdraw{B4B5B7} - Withdraws money from the treasury.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/facdeposit{B4B5B7} - Deposits money into faction bank accounts.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(un)invite{B4B5B7} - Uninvites a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/ouninvite{B4B5B7} - Uninvites a member of your faction that is offline.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/giverank{B4B5B7} - Sets the rank of a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/showbadge{B4B5B7} - Shows your badge to another player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(f)action(l)eaders{B4B5B7} - Communicate to other leaders and faction moderators.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/l(ocal)r(adio){B4B5B7} - Speak on the faction's local radio.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/seizebiz{B4B5B7} - Seize down a business.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(g)ive(w)eapon(l)icense.", hstr);	
					format(hstr, sizeof(hstr), "%s\n{007BD0}/dlock{B4B5B7} - Lock a dynamic vehicle with a lock enabled.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/towcars{B4B5B7} - Respawns all faction vehicles.", hstr);
					ShowPlayerDialog(playerid, 85, DIALOG_STYLE_MSGBOX, "Faction", hstr, "Ok", "Back");
					return 1;
				}
				if(PlayerInfo[playerid][pMember] == 7 || PlayerInfo[playerid][pLeader] == 7) //High Court
				{
					new hstr[2000];
					format(hstr, sizeof(hstr), "{007BD0}Faction Help:{B4B5B7}\n", hstr);
					format(hstr, sizeof(hstr), "%sThe following commands are specific to your faction:\n", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/r)adio{B4B5B7} - Types a message in radio chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/d)epartments{B4B5B7} - Types a message in departments chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/clothes{B4B5B7} - Used to change clothes.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/invite{B4B5B7} - Invites a member into your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/giverank{B4B5B7} - Sets the rank of a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(f)action(l)eaders{B4B5B7} - Communicate to other leaders and faction moderators.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/facwithdraw{B4B5B7} - Withdraws money from your factions bank account.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/jd{B4B5B7} - Judicial Department locker.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/cuff{B4B5B7} - Cuffs a player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/dlock{B4B5B7} - Lock a dynamic vehicle with a lock enabled.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/gavel{B4B5B7} - Bang the gavel on the bench (in courtroom).", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/seizebiz{B4B5B7} - Seize a business.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/seizefunds{B4B5B7} - Seizes funds from a person's bank account.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/towcars{B4B5B7} - Respawns all faction vehicles.", hstr);
					ShowPlayerDialog(playerid, 85, DIALOG_STYLE_MSGBOX, "Faction", hstr, "Ok", "Cancel");
					return 1;
				}
				if(PlayerInfo[playerid][pMember] == 9 || PlayerInfo[playerid][pLeader] == 9) //News Agency
				{
					new hstr[3048];
					format(hstr, sizeof(hstr), "{007BD0}Faction Help:{B4B5B7}\n", hstr);
					format(hstr, sizeof(hstr), "%sThe following commands are specific to your faction:\n", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/san{B4B5B7} - Used to go on duty.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/news{B4B5B7} - Reports the news.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/live{B4B5B7} - Starts a live interview with another player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/clothes{B4B5B7} - Used to change clothes.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(un)invite{B4B5B7} - Uninvites a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/ouninvite{B4B5B7} - Uninvites a member of your faction that is offline.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/giverank{B4B5B7} - Sets the rank of a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(f)action(l)eaders{B4B5B7} - Communicate to other leaders and faction moderators.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/dlock{B4B5B7} - Lock a dynamic vehicle with a lock enabled.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/sanhotline{B4B5B7} - Toggle off the ability for others to use the hotline.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/showp(ress)card{B4B5B7} - Show your SAN press card to someone.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/blacklistad{B4B5B7} - Blacklist someone from using advertisements.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/unblacklistad{B4B5B7} - Unblackist someone from using advertisements.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/sanjingle{B4B5B7} - Play the SAN Jingle.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/placecamera{B4B5B7} - Place the SAN camera.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/startbroadcast{B4B5B7} - Start the broadcast for live news (camera must be present).", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/endbroadcast{B4B5B7} - End the broadcast for live news (camera must be present).", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/pickupcamera{B4B5B7} - Pick up the SAN camera.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/towcars{B4B5B7} - Respawns all faction vehicles.", hstr);
					ShowPlayerDialog(playerid, 85, DIALOG_STYLE_MSGBOX, "Faction", hstr, "Ok", "Back");
					return 1;
				}
				if(PlayerInfo[playerid][pMember] == 10 || PlayerInfo[playerid][pLeader] == 10) //San Andreas Taxi Company
				{
					new hstr[1024];
					format(hstr, sizeof(hstr), "{007BD0}Faction Help:{B4B5B7}\n", hstr);
					format(hstr, sizeof(hstr), "%sThe following commands are specific to your faction:\n", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/r)adio{B4B5B7} - Types a message in radio chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/fare{B4B5B7} - Sets your fare.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/clothes{B4B5B7} - Used to change clothes.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(un)invite{B4B5B7} - Uninvites a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/ouninvite{B4B5B7} - Uninvites a member of your faction that is offline.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/giverank{B4B5B7} - Sets the rank of a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(f)action(l)eaders{B4B5B7} - Communicate to other leaders and faction moderators.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/facwithdraw{B4B5B7} - Withdraws money from your factions bank account.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/taxisign{B4B5B7} - Puts a taxi sign on a vehicle.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/dlock{B4B5B7} - Lock a dynamic vehicle with a lock enabled.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/towcars{B4B5B7} - Respawns all faction vehicles.", hstr);
					ShowPlayerDialog(playerid, 85, DIALOG_STYLE_MSGBOX, "Faction", hstr, "Ok", "Back");
					return 1;
				}
				if(PlayerInfo[playerid][pGroup] == GROUP_S9) // Sector 9
				{
					new hstr[1024];
					format(hstr, sizeof(hstr), "{007BD0}Faction Help:{B4B5B7}\n", hstr);
					format(hstr, sizeof(hstr), "%sThe following commands are specific to your faction:\n", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/disguise{B4B5B7} - Used to change clothes.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/mask{B4B5B7} - Used to hide your name.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/sate{B4B5B7} - Only works in an S9 or FBI vehicle. Satellite.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/changefreq{B4B5B7} - Used to listen in to faction radios.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/profile{B4B5B7} - Used to view a player's profile.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/s9{B4B5B7} - Used to access S9 lockers at the HQ.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/shows9badge{B4B5B7} - Shows your S9 badge. Use with caution.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/plantbug{B4B5B7} - Used to plant a bug on a player. Listen in by sitting in an FBI or S9 vehicle.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(un)invite{B4B5B7} - (Un)invites a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/ouninvite{B4B5B7} - Uninvites a member of your faction that is offline.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/giverank{B4B5B7} - Sets the rank of a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(f)action(l)eaders{B4B5B7} - Communicate to other leaders and faction moderators.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/dlock{B4B5B7} - Lock a dynamic vehicle with a lock enabled.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/towcars{B4B5B7} - Respawns all faction vehicles.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/placejammer{B4B5B7} - Places a jammer on the ground.");
					format(hstr, sizeof(hstr), "%s\n{007BD0}/removejammer{B4B5B7} - Picks up a jammer from the ground.");
					format(hstr, sizeof(hstr), "%s\n{007BD0}/crushjammer{B4B5B7} - Crushes a jammer from the ground.");
					format(hstr, sizeof(hstr), "%s\n{007BD0}/removejammer{B4B5B7} - Picks up a jammer from the ground.");
					format(hstr, sizeof(hstr), "%s\n{007BD0}/jammer{B4B5B7} - Checks if a jammer is around you..");
					format(hstr, sizeof(hstr), "%s\n{007BD0}/s9uc{B4B5B7} - Hides your crimes and tickets from the MDC.");
					ShowPlayerDialog(playerid, 85, DIALOG_STYLE_MSGBOX, "Faction", hstr, "Ok", "Back");
					return 1;
				}
				if(PlayerInfo[playerid][pMember] == 15 || PlayerInfo[playerid][pLeader] == 15) //Drug Lords
				{
					new hstr[1024];
					format(hstr, sizeof(hstr), "{007BD0}Faction Help:{B4B5B7}\n", hstr);
					format(hstr, sizeof(hstr), "%sThe following commands are specific to your faction:\n", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/sas{B4B5B7} - Used to access your lockers.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/clothes{B4B5B7} - Used to change clothes.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(un)invite{B4B5B7} - Uninvites a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/ouninvite{B4B5B7} - Uninvites a member of your faction that is offline.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/giverank{B4B5B7} - Sets the rank of a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(f)action(l)eaders{B4B5B7} - Communicate to other leaders and faction moderators.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/dlock{B4B5B7} - Lock a dynamic vehicle with a lock enabled.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/towcars{B4B5B7} - Respawns all faction vehicles.", hstr);
					ShowPlayerDialog(playerid, 85, DIALOG_STYLE_MSGBOX, "Faction", hstr, "Ok", "Back");
					return 1;
				}
				if(PlayerInfo[playerid][pMember] == 12 || PlayerInfo[playerid][pLeader] == 12)   //DEA
				{
					new hstr[1800];
					format(hstr, sizeof(hstr), "{007BD0}Faction Help:{B4B5B7}\n", hstr);
					format(hstr, sizeof(hstr), "%sThe following commands are specific to your faction:\n", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/r)adio{B4B5B7} - Types a message in radio chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/d)epartments{B4B5B7} - Types a message in departments chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/m)egaphone{B4B5B7} - Types a message in the megaphone.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/su)spect{B4B5B7} - Make a player a suspect.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/dea{B4B5B7} - Used to go on duty.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/cuff{B4B5B7} - Cuffs a player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/detain{B4B5B7} - Detains a player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/sdtazer{B4B5B7} - Equips tazer gun.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/handtazer{B4B5B7} - Equips stun weapon.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/showbadge{B4B5B7} - Shows your badge to another player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/take{B4B5B7} - Takes things from another player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/gov)ernment{B4B5B7} - Displays a government message to the server.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/deliver{B4B5B7} - Delivers a player to prison.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/clothes{B4B5B7} - Used to change clothes.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(un)invite{B4B5B7} - Uninvites a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/ouninvite{B4B5B7} - Uninvites a member of your faction that is offline.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/giverank{B4B5B7} - Sets the rank of a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/sate{B4B5B7} - Enables satellite mode while in a surveillance van.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/opencell{B4B5B7} - Opens nearby jail cells.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(f)action(l)eaders{B4B5B7} - Communicate to other leaders and faction moderators.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/facwithdraw{B4B5B7} - Withdraws money from your factions bank account.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/dlock{B4B5B7} - Lock a dynamic vehicle with a lock enabled.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/towcars{B4B5B7} - Respawns all faction vehicles.", hstr);
					ShowPlayerDialog(playerid, 85, DIALOG_STYLE_MSGBOX, "Faction", hstr, "Ok", "Back");
					return 1;
				}
				if(PlayerInfo[playerid][pMember] == 13 || PlayerInfo[playerid][pLeader] == 13)   //ATF
				{
					new hstr[1800];
					format(hstr, sizeof(hstr), "{007BD0}Faction Help:{B4B5B7}\n", hstr);
					format(hstr, sizeof(hstr), "%sThe following commands are specific to your faction:\n", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/r)adio{B4B5B7} - Types a message in radio chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/d)epartments{B4B5B7} - Types a message in departments chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/m)egaphone{B4B5B7} - Types a message in the megaphone.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/su)spect{B4B5B7} - Make a player a suspect.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/atf{B4B5B7} - Used to go on duty.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/cuff{B4B5B7} - Cuffs a player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/detain{B4B5B7} - Detains a player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/sdtazer{B4B5B7} - Equips tazer gun.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/handtazer{B4B5B7} - Equips stun weapon.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/showbadge{B4B5B7} - Shows your badge to another player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/take{B4B5B7} - Takes things from another player.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(/gov)ernment{B4B5B7} - Displays a government message to the server.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/deliver{B4B5B7} - Delivers a player to prison.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/clothes{B4B5B7} - Used to change clothes.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(un)invite{B4B5B7} - Uninvites a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/ouninvite{B4B5B7} - Uninvites a member of your faction that is offline.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/giverank{B4B5B7} - Sets the rank of a member of your faction.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/sate{B4B5B7} - Enables satellite mode while in a surveillance van.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/opencell{B4B5B7} - Opens nearby jail cells.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/(f)action(l)eaders{B4B5B7} - Communicate to other leaders and faction moderators.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/facwithdraw{B4B5B7} - Withdraws money from your factions bank account.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/dlock{B4B5B7} - Lock a dynamic vehicle with a lock enabled.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/towcars{B4B5B7} - Respawns all faction vehicles.", hstr);
					ShowPlayerDialog(playerid, 85, DIALOG_STYLE_MSGBOX, "Faction", hstr, "Ok", "Back");
					return 1;
				}
				return 1;
			}
			if(listitem == 7) // Family
			{
				if(PlayerInfo[playerid][pFMember] == 255)
				{
					ShowHelpDialog(playerid);
					SendClientMessage(playerid, COLOR_GREY, "You are not a member of a Family.");
					return 1;
				}
				new hstr[1400];
				format(hstr, sizeof(hstr), "{007BD0}Family Help:{B4B5B7}\n", hstr);
				format(hstr, sizeof(hstr), "%sThe following commands are specific to your family:\n", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/(/f)amily{B4B5B7} - Displays a message in family chat.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/fwithdraw{B4B5B7} - Withdraws money from your family bank account.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/famdeposit{B4B5B7} - Deposits money into your families bank account.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/fsafe{B4B5B7} - Allows you to access your family safe.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/fstats{B4B5B7} - Displays the contents of your families safe.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/clothes{B4B5B7} - Used to change clothes.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/togfam{B4B5B7} - Used to toggle the family chat for yourself.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/dlock{B4B5B7} - Lock a dynamic vehicle with a lock enabled.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/createtag{B4B5B7} - Create a graffiti tag.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/removetag{B4B5B7} - remove a graffiti tag.", hstr);
				if(PlayerInfo[playerid][pRank] >= 5)
				{
					format(hstr, sizeof(hstr), "%s\n{007BD0}/fmute{B4B5B7} - Used to mute a player from the (/f) chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/funmute{B4B5B7} - Used to unmute a player from the (/f) chat.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/togfamchat{B4B5B7} - Used to toggle the family chat for all members.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/adjust{B4B5B7} - Used to adjust multiple things regarding your family.", hstr);
					format(hstr, sizeof(hstr), "%s\n{007BD0}/tags{B4B5B7} - Used to display your current family's tags.", hstr);
				}
				ShowPlayerDialog(playerid, 86, DIALOG_STYLE_MSGBOX, "Other", hstr, "Ok", "Back");
				return 1;
			}
			if(listitem == 8) // Other
			{
				new hstr[1024];
				format(hstr, sizeof(hstr), "{007BD0}Other Help:{B4B5B7}\n", hstr);
				format(hstr, sizeof(hstr), "%sThe following commands are miscellaneous:\n", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/cellphonehelp(s){B4B5B7} - Displays the cellphone commands.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/househelp{B4B5B7} - Displays the house commands.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/fishhelp{B4B5B7} - Displays the fishing commands.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/irchelp{B4B5B7} - Displays the irc commands.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/carhelp{B4B5B7} - Displays the car commands.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/radiohelp{B4B5B7} - Displays the radio station commands.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/grouphelp{B4B5B7} - Displays the group commands.", hstr);
				ShowPlayerDialog(playerid, 88, DIALOG_STYLE_MSGBOX, "Other", hstr, "Ok", "Back");
				return 1;
			}
			if(listitem == 9) // Business
			{
				new hstr[1024];
				format(hstr, sizeof(hstr), "{007BD0}Business Help:{B4B5B7}\n", hstr);
				format(hstr, sizeof(hstr), "%sThe following commands are for businesses owners:\n", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/sellbiz(s){B4B5B7} - Sells a business.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/sellbiztomarket{B4B5B7} - Sells a business to the market.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/biz{B4B5B7} - Allows you to set properties of your business.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/buybiz{B4B5B7} - Buys a business.", hstr);
				ShowPlayerDialog(playerid, 89, DIALOG_STYLE_MSGBOX, "Gambling", hstr, "Ok", "Back");
				return 1;
			}
			if(listitem == 10) // Gambling
			{
				new hstr[1024];
				format(hstr, sizeof(hstr), "{007BD0}Gambling Help:{B4B5B7}\n", hstr);
				format(hstr, sizeof(hstr), "%sThe following commands are for gambling:\n", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/showcard(s){B4B5B7} - Shows card to another player.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/cards{B4B5B7} - Shows you a list of your cards.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/discard{B4B5B7} - Discards a card.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/dealcards{B4B5B7} - Deals cards to another player.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/shuffle{B4B5B7} - Shuffles your deck of cards.", hstr);
				ShowPlayerDialog(playerid, 90, DIALOG_STYLE_MSGBOX, "Gambling", hstr, "Ok", "Back");
				return 1;
			}
			if(listitem == 11) // Admin
			{
				if(PlayerInfo[playerid][pAdmin] < 1) return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to view this.");
				new hstr[1024];
				format(hstr, sizeof(hstr), "{007BD0}Admin Help:{B4B5B7}\n", hstr);
				format(hstr, sizeof(hstr), "%sThe following commands are for admins:\n", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/(/a)dmin{B4B5B7} - Displays a message in the admin chat.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/(/ah)elp{B4B5B7} - Displays a list of admins commands.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/ahousehelp{B4B5B7} - Displays the admin house commands.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/apropertyhelp{B4B5B7} - Displays the admin property commands.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/agaragehelp{B4B5B7} - Displays the admin garage commands.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/agatehelp{B4B5B7} - Displays the admin gate commands.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/nametag{B4B5B7} - Set a colored nametag above your name.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/newbietag{B4B5B7} - Set your tag in the newbie chat.", hstr);
				ShowPlayerDialog(playerid, 87, DIALOG_STYLE_MSGBOX, "Admin", hstr, "Ok", "Back");
				return 1;
			}
			if(listitem == 12) // Helper
			{
				if(PlayerInfo[playerid][pHelper] < 1) return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to view this.");
				new hstr[1024];
				format(hstr, sizeof(hstr), "{007BD0}Helper Help:{B4B5B7}\n", hstr);
				format(hstr, sizeof(hstr), "%sThe following commands are for helpers:\n", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/helpers{B4B5B7} - Lists all helpers and their helper score.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/newbies{B4B5B7} - Lists all newbies.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/gotonewbie{B4B5B7} - Teleports to a newbie to see if they need help.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/hgoto{B4B5B7} - Teleports to areas.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/nmute{B4B5B7} - Mutes a player from the newbie chat.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/(/h)helper(c)hat{B4B5B7} - Displays a message in the helper chat.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/nwarn{B4B5B7} - Newbie chat warns a player.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/ncheck{B4B5B7} - Checks a players newbie warnings.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/(n)(r)emove(w)arnings{B4B5B7} - Removes a players newbie warnings.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/nonewbie{B4B5B7} - Enables or disables the newbie chat.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/makehelper{B4B5B7} - Makes a player a helper.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/omakehelper{B4B5B7} - Makes a player a helper.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/helperhelp{B4B5B7} - Displays all the helper commands.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/nametag{B4B5B7} - Set a colored nametag above your name.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/newbietag{B4B5B7} - Set your tag in the newbie chat.", hstr);
				ShowPlayerDialog(playerid, 87, DIALOG_STYLE_MSGBOX, "Helper", hstr, "Ok", "Back");
				return 1;
			}
			if(listitem == 13) // Developer
			{
				if(PlayerInfo[playerid][pDev] < 1) return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to view this.");
				new hstr[1024];
				format(hstr, sizeof(hstr), "{007BD0}Developer Help:{B4B5B7}\n", hstr);
				format(hstr, sizeof(hstr), "%sThe following commands are for developers:\n", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/(d)ev(c)hat{B4B5B7} - Displays a message in the developer chat.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/viewhouseint{B4B5B7} - View house interiors.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/devs{B4B5B7} - Lists all developers.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/makedev{B4B5B7} - Makes a player a developer.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/devhelp{B4B5B7} - Displays all the developer commands.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/nametag{B4B5B7} - Set a colored nametag above your name.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/newbietag{B4B5B7} - Set your tag in the newbie chat.", hstr);
				ShowPlayerDialog(playerid, 87, DIALOG_STYLE_MSGBOX, "Developer", hstr, "Ok", "Back");
				return 1;
			}
			if(listitem == 14) // Beta Tester
			{
				if(PlayerInfo[playerid][pTester] < 1) return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to view this.");
				new hstr[1024];
				format(hstr, sizeof(hstr), "{007BD0}Beta Tester Help:{B4B5B7}\n", hstr);
				format(hstr, sizeof(hstr), "%sThe following commands are for beta testers:\n", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/(t)est(c)hat{B4B5B7} - Displays a message in the beta tester chat.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/testers{B4B5B7} - Lists all beta testers.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/togtester{B4B5B7} - Enables or disables the beta tester chat.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/acceptbug{B4B5B7} - Accepts a bug report.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/treturn{B4B5B7} - Returns you to your original position.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/bugmute{B4B5B7} - Mutes a player from reporting bugs.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/maketester{B4B5B7} - Makes a player a beta tester.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/nametag{B4B5B7} - Set a colored nametag above your name.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/newbietag{B4B5B7} - Set your tag in the newbie chat.", hstr);
				ShowPlayerDialog(playerid, 87, DIALOG_STYLE_MSGBOX, "Beta Tester", hstr, "Ok", "Back");
				return 1;
			}
			if(listitem == 15) // VIP
			{
				if(PlayerInfo[playerid][pDonateRank] < 1) return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to view this.");
				new hstr[1024];
				format(hstr, sizeof(hstr), "{007BD0}VIP Help:{B4B5B7}\n", hstr);
				format(hstr, sizeof(hstr), "%sThe following commands are for vips:\n", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/purchase{B4B5B7} - Purchase Items from the VIP Club or Blackmarket.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/getgift{B4B5B7} - Gets a random gift from the VIP Club.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/(v)ip(c)hat{B4B5B7} - Displays a message in the VIP chat.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/togvc{B4B5B7} - Hides or shows the VIP chat.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/viptime{B4B5B7} - Displays your VIP join or expiry date.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/vip{B4B5B7} - Set VIP nametag and other things.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/nametag{B4B5B7} - Set a colored nametag above your name.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/newbietag{B4B5B7} - Set your tag in the newbie chat.", hstr);
				ShowPlayerDialog(playerid, 87, DIALOG_STYLE_MSGBOX, "VIP", hstr, "Ok", "Back");
				return 1;
			}
			if(listitem == 16) // Character
			{
				new hstr[1024];
				format(hstr, sizeof(hstr), "{007BD0}Character Help{B4B5B7}\n", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/appearance{B4B5B7} - sets and displays your own/others set appearances.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/walkstyle{B4B5B7} - sets your character's walk style.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/namechange{B4B5B7} - inside the city hall, this allows you to change your name.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/inventory{B4B5B7} - displays inventory items.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/animlist{B4B5B7} - displays all available animations.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/licenses{B4B5B7} - displays your licenses.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/showlicenses{B4B5B7} - lets you show your licenses to another player", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/gear{B4B5B7} - allows you to manage your character's gear, remember to /buygear first.", hstr);
				ShowPlayerDialog(playerid, 393, DIALOG_STYLE_MSGBOX, "Character", hstr, "Ok", "Back");
				return 1;
			}
			if(listitem == 17) // drugs
			{
				new hstr[1024];
				format(hstr, sizeof(hstr), "{007BD0}Drug Help{B4B5B7}\n", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/inv{B4B5B7} - List all the drugs in your inventory.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/dropdrug{B4B5B7} - Allows you to drop drugs on the floor.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/takedrug{B4B5B7} - Picks up nearby drugs off of the floor.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/givedrug{B4B5B7} - Gives a drug to a specified player.", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/usedrug{B4B5B7} - Uses a specific drug.", hstr);
				ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Drugs", hstr, "Ok", "Close");
			}
			if(listitem == 18) // insurance
			{
				new hstr[1024];
				format(hstr, sizeof(hstr), "{007BD0}Insurance Help{B4B5B7}\n", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/buyinsurance{B4B5B7} - Buy insurance (must be used in the All Saints Hospital)", hstr);
				format(hstr, sizeof(hstr), "%s\n{007BD0}/myinsurance{B4B5B7} - Displays your insurance uses.", hstr);
				format(hstr, sizeof(hstr), "%s\n{B4B5B7}Insurance makes your recovery time in the hospital faster and reduces your hospital bill by 25 percents.", hstr);
				ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Insurance", hstr, "Ok", "Close");
			}
		}
		return 1;
	}
	if(dialogid == 80) //Account
	{
		if(!response)
			ShowHelpDialog(playerid);
		return 1;
	}
	if(dialogid == 81) //General
	{
		if(!response)
			ShowHelpDialog(playerid);
		return 1;
	}
	if(dialogid == 82) //Chat
	{
		if(!response)
			ShowHelpDialog(playerid);
		return 1;
	}
	if(dialogid == 83) //Toggles
	{
		if(!response)
			ShowHelpDialog(playerid);
		return 1;
	}
	if(dialogid == 84) //Bank
	{
		if(!response)
			ShowHelpDialog(playerid);
		return 1;
	}
	if(dialogid == 85) //Faction
	{
		if(!response)
			ShowHelpDialog(playerid);
		return 1;
	}
	if(dialogid == 86) //Family
	{
		if(!response)
			ShowHelpDialog(playerid);
		return 1;
	}
	if(dialogid == 87) //Other
	{
		if(!response)
			ShowHelpDialog(playerid);
		return 1;
	}
	if(dialogid == 88) //Biz
	{
		if(!response)
			ShowHelpDialog(playerid);
		return 1;
	}
	if(dialogid == 89) //Gambling
	{
		if(!response)
			ShowHelpDialog(playerid);
		return 1;
	}
	if(dialogid == 90) //Admin
	{
		if(!response)
			ShowHelpDialog(playerid);
		return 1;
	}
	if(dialogid == 393) //Character
	{
		if(!response)
			ShowHelpDialog(playerid);
		return 1;
	}
	//guide
	if(dialogid == DIALOGID_GUIDE_MAIN) //main guide screen
	{
		if(response)
		{
			if(listitem == 0) // How to make money
			{
				new hstr[2048];
				format(hstr, sizeof(hstr), "%s{007BD0}Jobs{FFFFFF}\n", hstr);
				format(hstr, sizeof(hstr), "%sThere are multiple ways to make money on this server, the most common way of making money is getting a job.\n", hstr);
				format(hstr, sizeof(hstr), "%sThe most popular job for new players is the {FFFF91}Bodyguard Job{FFFFFF}, this job allows you to sell vests to other players for quick money.\n", hstr);
				format(hstr, sizeof(hstr), "%sMost jobs have a skill level which can be leveled up by doing the job, the higher your skill level the more money you can make while doing it.\n", hstr);
				format(hstr, sizeof(hstr), "%sYou can quit your job at any time, when you quit a job you don't lose the skill level.\n", hstr);
				format(hstr, sizeof(hstr), "%sYou can get a job by going to the job location and typing /getjob.\n", hstr);
				format(hstr, sizeof(hstr), "%sTo see a list of jobs and their locations, type {FFFF91}/jobhelp{FFFFFF}.\n\n", hstr);

				format(hstr, sizeof(hstr), "%s{007BD0}Activities & Missions{FFFFFF}\n", hstr);
				format(hstr, sizeof(hstr), "%sActivities are things you can do on the side while having a job such as fish at the Pier or sell cars at the Crane in Ocean Docks. Activities like\n", hstr);
				format(hstr, sizeof(hstr), "%sfishing or selling cars will also raise your skill level resulting in more money or less cooldown time the next time you do it.\n", hstr);
				format(hstr, sizeof(hstr), "%sTo sell cars at the crane type {FFFF91}/dropcar{FFFFFF}.\n", hstr);

				format(hstr, sizeof(hstr), "%s{007BD0}Paycheck{FFFFFF}\n", hstr);
				format(hstr, sizeof(hstr), "%sYou start with $10,000 on hand and $20,000 which you can {FFFF91}/withdraw{FFFFFF} from the bank. You can also withdraw money from ATMs.\n", hstr);
				format(hstr, sizeof(hstr), "%sEvery 1 hour you will get a paycheck and 1 respect point, the amount of money you get on paycheck depends on your main level.\n", hstr);
				format(hstr, sizeof(hstr), "%sWhen you have enough money to {FFFF91}/buylevel{FFFFFF} your paycheck money will increase.", hstr);

				ShowPlayerDialog(playerid, DIALOGID_GUIDE_MONEY, DIALOG_STYLE_MSGBOX, "How to make money", hstr, "Ok", "Back");
				return 1;
			}
			if(listitem == 1) // Getting guns
			{
				new hstr[512];
				format(hstr, sizeof(hstr), "%s{007BD0}Getting guns{FFFFFF}\n", hstr);
				format(hstr, sizeof(hstr), "%s{D14545}Warning: You must play for at least 2 hours before you can get a weapon, this was added to help minimize deathmatching.{FFFFFF}\n\n", hstr);

				format(hstr, sizeof(hstr), "%sYou can buy weapons from Ammunations or Arms Dealers. If you are an {FFFF91}Arms Dealer{FFFFFF} you can create your own weapons and sell them to\n", hstr);
				format(hstr, sizeof(hstr), "%syourself or players. You must be at least level 2 before buying legal from an Ammunation.", hstr);

				ShowPlayerDialog(playerid, DIALOGID_GUIDE_GUNS, DIALOG_STYLE_MSGBOX, "Getting guns", hstr, "Ok", "Back");
				return 1;
			}
			if(listitem == 2) // Joining a faction or gang
			{
				new hstr[512];
				format(hstr, sizeof(hstr), "{007BD0}Joining a faction or gang:{FFFFFF}\n", hstr);

				format(hstr, sizeof(hstr), "%sTo join a gang or faction you need to be invited by the leader, being friends with the gangs members will increase your chance of being invited.\n", hstr);
				format(hstr, sizeof(hstr), "%sGrove Street and Ballas are both public gangs and are good if you want to quickly join one.\n", hstr);
				format(hstr, sizeof(hstr), "%sYou can display a list of gangs by typing {FFFF91}/families{FFFFFF}.", hstr);

				format(hstr, sizeof(hstr), "%s\nJoining factions requires filling out an application on the forums at {8D8DFF}gta-sarp.epizy.com/forums.", hstr);
				ShowPlayerDialog(playerid, DIALOGID_GUIDE_FACTION, DIALOG_STYLE_MSGBOX, "Joining a faction or gang", hstr, "Ok", "Back");

				return 1;
			}
			if(listitem == 3) // How to roleplay
			{
				new hstr[612];
				format(hstr, sizeof(hstr), "{007BD0}How to roleplay:{FFFFFF}\n", hstr);
				format(hstr, sizeof(hstr), "%sThe fastest way to learn how to roleplay is to watch other people doing it.\n", hstr);
				format(hstr, sizeof(hstr), "%sYou will earn the respect of other players quickly if you are a good roleplayer. You will also find it easier to join factions and gangs.\n", hstr);
				format(hstr, sizeof(hstr), "%sThere are commands which can help improve your roleplay skills quickly that you should familiarize yourself with.\n", hstr);
				format(hstr, sizeof(hstr), "%s{FFFF91}/me{FFFFFF} - Can be used to do actions (example: /me rolls up on Smo with an AK47.).\n", hstr);
				format(hstr, sizeof(hstr), "%s{FFFF91}/do{FFFFFF} - For describing the situation. (example: /do The stick is brown.\n", hstr);
				format(hstr, sizeof(hstr), "%s{FFFF91}/animhelp{FFFFFF} - displays a list of animations.", hstr);
				ShowPlayerDialog(playerid, DIALOGID_GUIDE_ROLEPLAY, DIALOG_STYLE_MSGBOX, "How to roleplay", hstr, "Ok", "Back");
				return 1;
			}
			if(listitem == 4) // How to level up
			{
				new hstr[812];
				format(hstr, sizeof(hstr), "{007BD0}How to level up:{FFFFFF}\n", hstr);
				format(hstr, sizeof(hstr), "%sYou can level up by getting respect points and the amount of money it costs to {FFFF91}/buylevel{FFFFFF}.\n", hstr);
				format(hstr, sizeof(hstr), "%sEach paycheck you will be given 1 respect point, you can earn the money by doing jobs, missions, fishing or /dropcar.\n", hstr);
				format(hstr, sizeof(hstr), "%sLeveling up your account will allow you to make more money on paycheck, get upgrade points as well as unlock features like:\n", hstr);
				format(hstr, sizeof(hstr), "%sThe ability to buy weapons from Ammunations. (Level 2)\n", hstr);
				format(hstr, sizeof(hstr), "%sThe ability to place a {FFFF91}/contract{FFFFFF} on another player (when a player has contract on their head they become a target and will lose half the contract\n", hstr);
				format(hstr, sizeof(hstr), "%sprice if they are killed by a hitman).\n", hstr);
				format(hstr, sizeof(hstr), "%sUnlock more commands ({FFFF91}/wiretransfer{FFFFFF} etc).", hstr);
				format(hstr, sizeof(hstr), "%sYou can type {FFFF91}/stats{FFFFFF} to display your account stats, more commands are listed in {FFFF91}/help{FFFFFF}.\n", hstr);
				ShowPlayerDialog(playerid, DIALOGID_GUIDE_LEVEL, DIALOG_STYLE_MSGBOX, "How to level up", hstr, "Ok", "Back");
				return 1;
			}
		}
		viewingGuide[playerid] = 0;
		return 1;
	}
	if(dialogid == DIALOGID_GUIDE_MONEY)
	{
		if(!response)
		{
			ShowPlayerDialog(playerid, DIALOGID_GUIDE_MAIN, DIALOG_STYLE_LIST, "Guide", "How to make money\nGetting guns\nJoining a gang or faction\nHow to roleplay\nHow to level up", "Ok", "Cancel");
			return 1;
		}
		SendClientMessage(playerid, COLOR_WHITE, "Type {7DAEFF}/guide{FFFFFF} if you want to view this again, use ({7DAEFF}/newb{FFFFFF})ie to ask questions in the newbie chat channel.");
		viewingGuide[playerid] = 0;
	}
	if(dialogid == DIALOGID_GUIDE_GUNS)
	{
		if(!response)
		{
			ShowPlayerDialog(playerid, DIALOGID_GUIDE_MAIN, DIALOG_STYLE_LIST, "Guide", "How to make money\nGetting guns\nJoining a gang or faction\nHow to roleplay\nHow to level up", "Ok", "Cancel");
			return 1;
		}
		SendClientMessage(playerid, COLOR_WHITE, "Type {7DAEFF}/guide{FFFFFF} if you want to view this again, use ({7DAEFF}/newb{FFFFFF})ie to ask questions in the newbie chat channel.");
		viewingGuide[playerid] = 0;
	}
	if(dialogid == DIALOGID_GUIDE_FACTION)
	{
		if(!response)
		{
			ShowPlayerDialog(playerid, DIALOGID_GUIDE_MAIN, DIALOG_STYLE_LIST, "Guide", "How to make money\nGetting guns\nJoining a gang or faction\nHow to roleplay\nHow to level up", "Ok", "Cancel");
			return 1;
		}
		SendClientMessage(playerid, COLOR_WHITE, "Type {7DAEFF}/guide{FFFFFF} if you want to view this again, use ({7DAEFF}/newb{FFFFFF})ie to ask questions in the newbie chat channel.");
		viewingGuide[playerid] = 0;
	}
	if(dialogid == DIALOGID_GUIDE_ROLEPLAY)
	{
		if(!response)
		{
			ShowPlayerDialog(playerid, DIALOGID_GUIDE_MAIN, DIALOG_STYLE_LIST, "Guide", "How to make money\nGetting guns\nJoining a gang or faction\nHow to roleplay\nHow to level up", "Ok", "Cancel");
			return 1;
		}
		SendClientMessage(playerid, COLOR_WHITE, "Type {7DAEFF}/guide{FFFFFF} if you want to view this again, use ({7DAEFF}/newb{FFFFFF})ie to ask questions in the newbie chat channel.");
		viewingGuide[playerid] = 0;
	}
	if(dialogid == DIALOGID_GUIDE_LEVEL)
	{
		if(!response)
		{
			ShowPlayerDialog(playerid, DIALOGID_GUIDE_MAIN, DIALOG_STYLE_LIST, "Guide", "How to make money\nGetting guns\nJoining a gang or faction\nHow to roleplay\nHow to level up", "Ok", "Cancel");
			return 1;
		}
		SendClientMessage(playerid, COLOR_WHITE, "Type {7DAEFF}/guide{FFFFFF} if you want to view this again, use ({7DAEFF}/newb{FFFFFF})ie to ask questions in the newbie chat channel.");
		viewingGuide[playerid] = 0;
	}
	if(dialogid == DIALOG_SENATE_MAIN) //Senate
	{
		if(response)
		{
			if(listitem == 0)
			{
				if(PlayerInfo[playerid][pOnDuty] == 0) //if player is off duty, set them them on
				{
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					format(str, sizeof(str), "* %s retrieves his equipment from his locker and equips his badge & body camera.", sendername);
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					SetPlayerColor(playerid, TCOLOR_SENATE);
					if(PlayerInfo[playerid][pMember] == 11) FakeFaction[playerid] = 6;
					GivePlayerGun(playerid, 24);
					SetPlayerArmour(playerid, 100.0);
					PlayerInfo[playerid][pOnDuty] = 1;
					TogglePlayerControllable(playerid, true);

					if(PlayerInfo[playerid][pGroup] == GROUP_S9 && PlayerInfo[playerid][pMember] != FACTION_GOV)
					{
						SetPlayerSkin(playerid, 165);
						PlayerInfo[playerid][pModel] = 165;

						PlayerInfo[playerid][pOnDuty] = 1;
						PlayerInfo[playerid][pMember] = FACTION_GOV;
						PlayerInfo[playerid][pRank] = 1;

						SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have been given a badge automatically (GOV Security)");
					}
				}
				else if(PlayerInfo[playerid][pOnDuty] == 1) //if player is on duty, set them off
				{
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					format(str, sizeof(str), "* %s places his equipment inside his locker and removes his badge & body camera.", sendername);
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					SetPlayerColor(playerid, TCOLOR_WHITE);
					PlayerInfo[playerid][pGun2] = 0;
					ResetPlayerWeapons(playerid);
					ClearGuns(playerid);
					SetPlayerArmour(playerid, 0.0);
					PlayerInfo[playerid][pOnDuty] = 0;
					TogglePlayerControllable(playerid, true);
					// Shows civilian skin selector
					ShowSkinShop(playerid, true, false, false, false, false);
				}
			}
			if(listitem == 1)
			{
				// Shows faction skin selector
				ShowSkinShop(playerid, false, true, false, false, true);
			}
			if(listitem == 2)
			{
				ShowPlayerDialog(playerid, DIALOG_SENATE_ARMORY, DIALOG_STYLE_LIST, "Armory", "Deagle\t\t$500\nM4\t\t$6,000\nKevlar\t\t$150\nFirst Aid Kit\t$200", "Select", "Back"); // Senate Locker
			}
		}
		return 1;
	}
	if(dialogid == DIALOG_SENATE_ARMORY)
	{
		if(response)
		{

			if(listitem == 0) // Deagle
			{
				if(PlayerInfo[playerid][pCash] < 500)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					GiveMoney(playerid, -500);
					GivePlayerGun(playerid, 24);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 1) //M4
			{
				if(PlayerInfo[playerid][pCash] < 6000)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					GiveMoney(playerid, -6000);
					GivePlayerGun(playerid, 31);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 2) //kevlar
			{
				if(PlayerInfo[playerid][pCash] < 150)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					SetPlayerArmour(playerid, 100);
					GiveMoney(playerid, -150);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 3)
			{
				if(PlayerInfo[playerid][pCash] < 200)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					SetPlayerHealth(playerid, 100);
					GiveMoney(playerid, -200);
					TogglePlayerControllable(playerid, true);
				}
			}
		}
		else
		{
			ShowPlayerDialog(playerid, DIALOG_SENATE_MAIN, DIALOG_STYLE_LIST, "Government", "Duty\nChange Uniform\nArmory", "Select", "Cancel"); // Senate Locker
		}
		return 1;
	}
	if(dialogid == DIALOG_HC_MAIN) //High Court
	{
		if(response)
		{
			if(listitem == 0)
			{
				if(PlayerInfo[playerid][pOnDuty] == 0) //if player is off duty, set them them on
				{
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					format(str, sizeof(str), "* %s takes their equipment from the locker.", sendername);
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					SetPlayerColor(playerid, TCOLOR_HIGHCOURT);
					SetPlayerArmour(playerid, 100.0);
					PlayerInfo[playerid][pOnDuty] = 1;
					TogglePlayerControllable(playerid, true);
				}
				else if(PlayerInfo[playerid][pOnDuty] == 1) //if player is on duty, set them off
				{
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					format(str, sizeof(str), "* %s places their equipment in the locker.", sendername);
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					SetPlayerColor(playerid, TCOLOR_WHITE);
					ResetPlayerWeapons(playerid);
					ClearGuns(playerid);
					SetPlayerArmour(playerid, 0.0);
					PlayerInfo[playerid][pOnDuty] = 0;
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 1)
			{
				ShowSkinShop(playerid, false, true, false, false, true);
			}
			if(listitem == 2)
			{
				ShowPlayerDialog(playerid, DIALOG_HC_ARMORY, DIALOG_STYLE_TABLIST, "Armory", "First aid kit\tFree\nKevlar vest\tFree\nDesert Eagle\t$500\nMP5\t$1,000\nShotgun\t$1,000\nMace\t$25\nPortable MDC\tFREE", "Select", "Back"); //HC Locker
			}
		}
		return 1;
	}
	if(dialogid == DIALOG_HC_ARMORY)
	{
		if(response)
		{
			if(listitem == 0) // HP
			{
				SetPlayerHealth(playerid, 100);
			}
			if(listitem == 1) // Kevlar
			{
				if(PlayerInfo[playerid][pCash] < 1000)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					SetPlayerArmour(playerid, 100);
					GiveMoney(playerid, -1000);
				}
			}
			if(listitem == 2) //Deagle
			{
				GivePlayerGun(playerid, 24);
				TogglePlayerControllable(playerid, true);
			}
			if(listitem == 3) //MP5
			{
				if(PlayerInfo[playerid][pCash] < 1000)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					GiveMoney(playerid, -1000);
					GivePlayerGun(playerid, 29);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 4) //Shotgun
			{
				if(PlayerInfo[playerid][pCash] < 1000)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					GiveMoney(playerid, -1000);
					GivePlayerGun(playerid, 25);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 5) //Mace
			{
				if(PlayerInfo[playerid][pCash] < 25)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					TogglePlayerControllable(playerid, true);
					return 1;
				}
				else
				{
					GiveMoney(playerid, -25);
					GivePlayerGun(playerid, 41);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 6) // portable mdc
			{
				PlayerInfo[playerid][pPortMDC] = 1;
				SendClientMessage(playerid, COLOR_ORANGE, "You take a portable MDC unit from the locker.");
				SendClientMessage(playerid, COLOR_ORANGE, "(( This will be lost upon death. ))");
			}
		}
		else
		{
			ShowPlayerDialog(playerid, DIALOG_HC_MAIN, DIALOG_STYLE_LIST, "Judicial Department", "Duty\nChange Uniform\nArmory", "Select", "Cancel"); // HC Locker
		}
		return 1;
	}
	if(dialogid == 93) //Tiki Exchange
	{
		if(response)
		{
			if(listitem == 0) // SD Pistol
			{
				if(PlayerInfo[playerid][pConnectTime] < 5) return SendClientMessage(playerid, COLOR_GREY, "You are not allowed to use this exchange.");

				if(PlayerInfo[playerid][pTikis] < 1) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough tikis.");
				PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				PlayerInfo[playerid][pTikis] -= 5;
				GivePlayerGun(playerid, 23);
				SendClientMessage(playerid, COLOR_GREY, "You exchanged 5 tiki for a Silenced Pistol.");
				return 1;
			}
			if(listitem == 1) // Full Armor
			{
				if(PlayerInfo[playerid][pTikis] < 10) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough tikis.");
				PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				PlayerInfo[playerid][pTikis] -= 10;
				SetPlayerArmour(playerid, 100);
				SendClientMessage(playerid, COLOR_GREY, "You exchanged 10 tikis for Full Armor.");
				return 1;
			}
			if(listitem == 2) // 25 Pot
			{
				SendClientMessage(playerid, COLOR_GREY, "This has been disabled.");
				return 1;
			}
			if(listitem == 3) // 25 Crack
			{
				SendClientMessage(playerid, COLOR_GREY, "This has been disabled.");
				return 1;
			}
			if(listitem == 4) // 10,000 materials
			{
				if(PlayerInfo[playerid][pTikis] < 25) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough tikis.");
				PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				PlayerInfo[playerid][pTikis] -= 25;
				PlayerInfo[playerid][pMats] += 10000;
				SendClientMessage(playerid, COLOR_GREY, "You exchanged 25 tikis for 10,000 Materials.");
				return 1;
			}
			if(listitem == 5) // $100,000
			{
				if(PlayerInfo[playerid][pTikis] < 15) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough tikis.");
				PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				PlayerInfo[playerid][pTikis] -= 15;
				GiveMoney(playerid, 100000);
				SendClientMessage(playerid, COLOR_GREY, "You exchanged 15 tikis for $100,000.");
				return 1;
			}
			if(listitem == 6) // Gamble
			{
				if(PlayerInfo[playerid][pConnectTime] < 4) return SendClientMessage(playerid, COLOR_GREY, "You are not allowed to use this exchange.");
				if(PlayerInfo[playerid][pTikis] < 40) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough tikis.");

				new rand = 0 + random(14);
				PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				PlayerInfo[playerid][pTikis] -= 40;
				if(rand == 0)
				{
					GivePlayerGun(playerid, 28);
					return SendClientMessage(playerid, COLOR_GREY, "You exchanged 40 tikis for a Micro SMG.");
				}
				else if(rand == 1)
				{
					GivePlayerGun(playerid, 26);
					return SendClientMessage(playerid, COLOR_GREY, "You exchanged 40 tikis for a Sawnoff Shotgun.");
				}
				else if(rand == 2)
				{
					PlayerInfo[playerid][pCash] += 115000;
					return SendClientMessage(playerid, COLOR_GREY, "You exchanged 40 tikis for $115,000.");
				}
				else if(rand == 3)
				{
					PlayerInfo[playerid][pTikis] += 25;
					return SendClientMessage(playerid, COLOR_GREY, "You exchanged 40 tikis for 25 Tikis.");
				}
				else if(rand == 4)
				{
					/*PlayerInfo[playerid][pSHealth] = 100;
					return SendClientMessage(playerid, COLOR_GREY, "You exchanged 12 tikis for Full Armor on spawn.");*/
					return SendClientMessage(playerid, COLOR_GREY, "You exchanged 40 tikis for nothing! Try again, sorry for your luck.");
				}
				else if(rand == 5)
				{
					GivePlayerGun(playerid, 26);
					return SendClientMessage(playerid, COLOR_GREY, "You exchanged 40 tikis for a Sawnoff Shotgun.");
				}
				else if(rand == 6)
				{
					GivePlayerGun(playerid, 26);
					return SendClientMessage(playerid, COLOR_GREY, "You exchanged 40 tikis for a Sawnoff Shotgun.");
				}
				else if(rand == 7)
				{
					return SendClientMessage(playerid, COLOR_GREY, "You exchanged 40 tikis for nothing! Try again, sorry for your luck.");
				}
				else if(rand == 8)
				{
					return SendClientMessage(playerid, COLOR_GREY, "You exchanged 40 tikis for nothing! Try again, sorry for your luck.");
				}
				else if(rand == 9)
				{
					GivePlayerGun(playerid, 26);
					return SendClientMessage(playerid, COLOR_GREY, "You exchanged 40 tikis for a Sawnoff Shotgun.");
				}
				else if(rand == 10)
				{
					GivePlayerGun(playerid, 32);
					return SendClientMessage(playerid, COLOR_GREY, "You exchanged 40 tikis for a Tec 9.");
				}
				else if(rand == 11)
				{
					PlayerInfo[playerid][gTicket] = 5;
					return SendClientMessage(playerid, COLOR_GREY, "You exchanged 40 tikis for 5 Gear Tickets.");
				}
				else if(rand == 12)
				{
					PlayerInfo[playerid][pMats] += 50000;
					return SendClientMessage(playerid, COLOR_GREY, "You exchanged 40 tikis for 50,000 Materials.");
				}
				else if(rand == 13)
				{
					PlayerInfo[playerid][pSHP] = 100;
					return SendClientMessage(playerid, COLOR_GREY, "You exchanged 40 tikis for Full Health on spawn.");
				}
				else
				{
					PlayerInfo[playerid][pSHP] = 100;
					return SendClientMessage(playerid, COLOR_GREY, "You exchanged 40 tikis for Full Health on spawn.");
				}
			}
			if(listitem == 7) // Mace
			{
				if(PlayerInfo[playerid][pTikis] < 20) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough tikis.");
				PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				PlayerInfo[playerid][pTikis] -= 20;
				GivePlayerGun(playerid, 41);
				SendClientMessage(playerid, COLOR_GREY, "You exchanged 20 tikis for a mace.");
				return 1;
			}
			if(listitem == 8) // Voodoo Magic
			{
				if(PlayerInfo[playerid][pTikis] < 25) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough tikis.");
				PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				PlayerInfo[playerid][pTikis] -= 25;
				VoodooSpell[playerid] = 1;
				SendClientMessage(playerid, COLOR_GREY, "You exchanged 25 tikis for Voodoo Magic (/vsetweather).");
				return 1;
			}
			if(listitem == 9) // C4
			{
				SendClientMessage(playerid, COLOR_GREY, "This has been disabled.");
				return 1;
			}
			if(listitem == 10) // Gear Ticket
			{
				if(PlayerInfo[playerid][pTikis] < 10) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough tikis.");
				PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				PlayerInfo[playerid][pTikis] -= 10;

				PlayerInfo[playerid][gTicket] += 1;
				SendClientMessage(playerid, COLOR_GREY, "You exchanged 10 tikis for a Gear Ticket.");
				return 1;
			}
			if(listitem == 11) // Large Boombox
			{
				if(PlayerInfo[playerid][pTikis] < 20) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough tikis.");
				if(PlayerInfo[playerid][pStereo]) return SendClientMessage(playerid, COLOR_GREY, "You already have a Large Boombox.");
				PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				PlayerInfo[playerid][pTikis] -= 20;

				PlayerInfo[playerid][pStereo] = 4;
				SendClientMessage(playerid, COLOR_GREY, "You exchanged 20 tikis for a Large Boombox.");
				return 1;
			}
			if(listitem == 12) // TEC 9
			{
				if(PlayerInfo[playerid][pConnectTime] < 15) return SendClientMessage(playerid, COLOR_GREY, "You are not allowed to use this exchange.");

				if(PlayerInfo[playerid][pTikis] < 15) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough tikis.");
				PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				PlayerInfo[playerid][pTikis] -= 15;

				GivePlayerGun(playerid, 28);
				SendClientMessage(playerid, COLOR_GREY, "You exchanged 15 tikis for a TEC 9.");
				return 1;
			}
			if(listitem == 13) // Grenade
			{
				return SendClientMessage(playerid, COLOR_GREY, "Grenades are currently disabled!");
			}
			if(listitem == 14) // RPG
			{
				return SendClientMessage(playerid, COLOR_GREY, "RPG is currently disabled!");
			}
			if(listitem == 15) // Silver VIP
			{
				if(PlayerInfo[playerid][pTikis] < 60) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough tikis.");

				//give vip
				new Year, Month, Day;
				getdate(Year, Month, Day);
				if(Month == 1 && Day >= 29) return SendClientMessage(playerid, COLOR_GREY, "You cannot get Silver VIP today.");

				//take tikis
				PlayerInfo[playerid][pTikis] -= 60;
				PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				SendClientMessage(playerid, COLOR_GREY, "You exchanged 60 tikis for Silver VIP.");

				PlayerInfo[playerid][pDonateRank] = 2;
				PlayerInfo[playerid][pTag] = NTAG_SVIP;

				format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has purchased silver VIP from the Tiki Store.", PlayerName(playerid));
				ABroadCast(COLOR_WHITE, str, 1);

				format(str, 32, "%02d/%02d/%d", Month, Day, Year);
				strmid(PlayerInfo[playerid][pVIPJoinDate], str, 0, strlen(str), 255);
				format(str, 32, "%02d/%02d/%d", Month +1, Day, Year);
				strmid(PlayerInfo[playerid][pVIPExpDate], str, 0, strlen(str), 255);
				if(Month == 12)
				{
					format(PlayerInfo[playerid][pVIPExpDate], 32, "%02d/%02d/%d", 1, Day, Year +1);
				}
				format(str, sizeof(str), "Join Date: %s, Expire Date: %s", PlayerInfo[playerid][pVIPJoinDate], PlayerInfo[playerid][pVIPExpDate]);
				SendClientMessage(playerid, COLOR_GRAD2, str);
				return 1;
			}
			if(listitem == 16) // Knife
			{
				if(PlayerInfo[playerid][pConnectTime] < 4) return SendClientMessage(playerid, COLOR_GREY, "You are not allowed to use this exchange.");
				if(PlayerInfo[playerid][pTikis] < 65) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough tikis.");
				PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				PlayerInfo[playerid][pTikis] -= 65;

				GivePlayerGun(playerid, 4);
				SendClientMessage(playerid, COLOR_GREY, "You exchanged 65 tikis for a Knife.");
				return 1;
			}
		}
		return 1;
	}
	if(dialogid == 94) //Pickpocket
	{
		if(response)
		{
			new giveplayerid = Robbing[playerid];
			if(IsPlayerConnected(giveplayerid) && BeingRobbed[giveplayerid] == playerid)
			{
				if(IsPlayerInRangeOfPlayer(1.0, playerid, giveplayerid))
				{
					if(listitem == 0) // Cash
					{
						if(PlayerInfo[giveplayerid][pCash] > 0)
						{
							new lvl = convertToLevel(JOBID_THIEF, PlayerInfo[playerid][pThieveSkill]);
							new randmoney = lvl * random(PlayerInfo[giveplayerid][pCash] / 20);
							if(randmoney < 0 || randmoney > 500000)
							{
								SendClientMessage(playerid, COLOR_GREY, "That dropped the money.");
								Robbing[playerid] = 999;
								BeingRobbed[giveplayerid] = 999;
								return 1;
							}
							format(str, sizeof(str), "~w~Robbed %s~n~~g~$%d stolen.", PlayerICName(giveplayerid), randmoney);
							displayCenterHUDInfo(playerid,  str, 8);
							format(str, sizeof(str), "~w~Robbed by %s~n~~r~$%d lost.", PlayerICName(playerid), randmoney);
							displayCenterHUDInfo(giveplayerid,  str, 8);
							GiveMoney(playerid, randmoney);
							GiveMoney(giveplayerid, -randmoney);
						}
						else
						{
							DisplayDialogForPlayer(playerid, 94);
							return SendClientMessage(playerid, COLOR_GREY, "That player does not have any money on them.");
						}
					}
					if(listitem == 1) // Crack
					{
						if(PlayerInfo[giveplayerid][pCocaine] > 0)
						{
							new lvl = convertToLevel(JOBID_THIEF, PlayerInfo[playerid][pThieveSkill]);
							new randcrack = lvl * random(PlayerInfo[giveplayerid][pCocaine] / 5);
							if(randcrack < 0 || randcrack > 500000)
							{
								SendClientMessage(playerid, COLOR_GREY, "That dropped the crack.");
								Robbing[playerid] = 999;
								BeingRobbed[giveplayerid] = 999;
								return 1;
							}
							format(str, sizeof(str), "~w~Robbed %s~n~~g~%d grams of crack stolen.", PlayerICName(giveplayerid), randcrack);
							displayCenterHUDInfo(playerid,  str, 8);
							format(str, sizeof(str), "~w~Robbed by %s~n~~r~%d grams of crak lost.", PlayerICName(playerid), randcrack);
							displayCenterHUDInfo(giveplayerid,  str, 8);
							PlayerInfo[playerid][pCocaine] -= randcrack;
							PlayerInfo[giveplayerid][pCocaine] += randcrack;
						}
						else
						{
							DisplayDialogForPlayer(playerid, 94);
							return SendClientMessage(playerid, COLOR_GREY, "That player does not have any crack on them.");
						}
					}
					if(listitem == 2) // Pot
					{
						if(PlayerInfo[giveplayerid][pCannabis] > 0)
						{
							new lvl = convertToLevel(JOBID_THIEF, PlayerInfo[playerid][pThieveSkill]);
							new randpot = lvl * random(PlayerInfo[giveplayerid][pCannabis] / 5);
							if(randpot < 0 || randpot > 500000)
							{
								SendClientMessage(playerid, COLOR_GREY, "That dropped the pot.");
								Robbing[playerid] = 999;
								BeingRobbed[giveplayerid] = 999;
								return 1;
							}
							format(str, sizeof(str), "~w~Robbed %s~n~~g~%d grams of pot stolen.", PlayerICName(giveplayerid), randpot);
							displayCenterHUDInfo(playerid,  str, 8);
							format(str, sizeof(str), "~w~Robbed by %s~n~~r~%d grams of pot lost.", PlayerICName(playerid), randpot);
							displayCenterHUDInfo(giveplayerid,  str, 8);
							PlayerInfo[playerid][pCannabis] -= randpot;
							PlayerInfo[giveplayerid][pCannabis] += randpot;
						}
						else
						{
							DisplayDialogForPlayer(playerid, 94);
							return SendClientMessage(playerid, COLOR_GREY, "That player does not have any pot on them.");
						}
					}
					if(listitem == 3) // Materials
					{
						if(PlayerInfo[giveplayerid][pMats] > 0)
						{
							new lvl = convertToLevel(JOBID_THIEF, PlayerInfo[playerid][pThieveSkill]);
							new randmats = lvl * random(PlayerInfo[giveplayerid][pMats] / 20);
							if(randmats < 0 || randmats > 500000)
							{
								SendClientMessage(playerid, COLOR_GREY, "That dropped the materials.");
								Robbing[playerid] = 999;
								BeingRobbed[giveplayerid] = 999;
								return 1;
							}
							format(str, sizeof(str), "~w~Robbed %s~n~~g~%d materials stolen.", PlayerICName(giveplayerid), randmats);
							displayCenterHUDInfo(playerid,  str, 8);
							format(str, sizeof(str), "~w~Robbed by %s~n~~r~%d materials lost.", PlayerICName(playerid), randmats);
							displayCenterHUDInfo(giveplayerid,  str, 8);
							PlayerInfo[playerid][pMats] -= randmats;
							PlayerInfo[giveplayerid][pMats] += randmats;
						}
						else
						{
							DisplayDialogForPlayer(playerid, 94);
							return SendClientMessage(playerid, COLOR_GREY, "That player does not have any materials on them.");
						}
					}
					Robbing[playerid] = 999;
					BeingRobbed[giveplayerid] = 999;
					PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
					PlayerPlaySound(giveplayerid, 1145, 0.0, 0.0, 0.0);
					PlayerInfo[playerid][pThieveSkill] ++;
					if(PlayerInfo[playerid][pThieveSkill] == 50)
					{
						SendClientMessage(playerid, COLOR_YELLOW, "* Your Pick Pocket Skill is now Level 2, you will now earn more when stealing.");
					}
					else if(PlayerInfo[playerid][pThieveSkill] == 250)
					{
						SendClientMessage(playerid, COLOR_YELLOW, "* Your Pick Pocket Skill is now Level 3, you will now earn more when stealing.");
					}
					else if(PlayerInfo[playerid][pThieveSkill] == 500)
					{
						SendClientMessage(playerid, COLOR_YELLOW, "* Your Pick Pocket Skill is now Level 4, you will now earn more when stealing.");
					}
					else if(PlayerInfo[playerid][pThieveSkill] == 1000)
					{
						SendClientMessage(playerid, COLOR_YELLOW, "* Your Pick Pocket Skill is now Level 5, you will now earn more when stealing.");
					}
				}
				else
				{
					Robbing[playerid] = 999;
					BeingRobbed[giveplayerid] = 999;
					RobbingX[playerid] = 0.0;
					RobbingY[playerid] = 0.0;
					RobbingZ[playerid] = 0.0;
					SendClientMessage(playerid, COLOR_GREY, "That player is not in range.");
					return 1;
				}
			}
		}
		else
		{
			Robbing[playerid] = 999;
			RobbingX[playerid] = 0.0;
			RobbingY[playerid] = 0.0;
			RobbingZ[playerid] = 0.0;
		}
		return 1;
	}
	if(dialogid == DIALOG_STATS1)   //inv dialog
	{
		if(response)
		{
			return 0;
		}
		new rod[20], bait[20], baita, line[20], line1[1000], line2[1000], line3[1000];
		
		format(rod, sizeof(rod), "%s", GetFishingRodName(PlayerInfo[playerid][pFishRod]));
		format(bait, sizeof(bait), "%s", GetFishingBaitName(PlayerInfo[playerid][pBait]));
		format(line, sizeof(line), "%s", GetFishingLineName(PlayerInfo[playerid][pLine]));

		baita = PlayerInfo[playerid][pBaitAmount];
		
		if(baita == 0)
			bait = "None";

		new packages = Packages[playerid];
		new crates = Crates[playerid];
		//format(line1, sizeof(line1), "{007BD0}[Inventory] {CBCCCE}Materials:[%d]  Cannabis:[%d.0g]  Cocaine:[%d.0g] Meth[%d.0g]  Package:[%d]  Crates:[%d]  Seeds:[%d]  Products:[%d]", PlayerInfo[playerid][pMats], PlayerInfo[playerid][pCannabis], PlayerInfo[playerid][pCocaine], PlayerInfo[playerid][pMeth], packages, crates, PlayerInfo[playerid][pPotSeeds], PlayerInfo[playerid][pProducts]);
		format(line1, sizeof(line1), "{007BD0}[Inventory] {CBCCCE}Materials:[%d]  Cannabis:[%d.0g]  Cocaine:[%d.0g] Meth[%d.0g]  Package:[%d]  Crates:[%d]  Seeds:[%d]", PlayerInfo[playerid][pMats], PlayerInfo[playerid][pCannabis], PlayerInfo[playerid][pCocaine], PlayerInfo[playerid][pMeth], packages, crates, PlayerInfo[playerid][pPotSeeds]);
		format(line2, sizeof(line2), "{007BD0}[Inventory] {D8D8D8}Ropes:[%d]  Blindfolds:[%d]  Cigars:[%d]  Sprunk:[%d]  Spraycan:[%d]  Screw Driver:[%d]  Deck of Cards:[%d]  WTc:[%d]", PlayerInfo[playerid][pRope], PlayerInfo[playerid][pBlindfolds], PlayerInfo[playerid][pCigars], PlayerInfo[playerid][pSprunk], PlayerInfo[playerid][pSpraycan], PlayerInfo[playerid][pScrew], PlayerInfo[playerid][pDeck], PlayerInfo[playerid][pWTc]);
		format(line3, sizeof(line3), "{007BD0}[Inventory] {D8D8D8}Fishing Rod:[%s]  Bait:[%s]  Bait Left:[%d]  Fishing Line:[%s] Stereo:[%d] Box of Matches:[%d] Jerry Can:[%d]", rod, bait, baita, line, PlayerInfo[playerid][pStereo], PlayerInfo[playerid][pMatches], PlayerInfo[playerid][pJerrycan]);
		SendClientMessage(playerid, COLOR_WHITE, line1);
		SendClientMessage(playerid, COLOR_WHITE, line2);
		SendClientMessage(playerid, COLOR_WHITE, line3);
		return 1;
	}
	if(dialogid == DIALOG_STATS)   //stats dialog
	{
		if(response)
		{
			return 0;
		}
		ShowStats(playerid, viewingStatsOf[playerid], true);
		return 1;
	}
	if(dialogid == DIALOG_SKILLS)   //skills dialog
	{
		if(response)
		{
			return 0;
		}
		return 1;
	}
	
	if(dialogid == 99)
	{
		if(response)
		{
			SetTimerEx("Timer_UseBM", 30000, 0, "i", playerid);
			SendClientMessage(playerid, COLOR_GREY, "You must wait 30 seconds before purchasing another weapon.");
			UseBMTimer[playerid] = 1;
			if(listitem == 0) // Deagle
			{
				if(PlayerInfo[playerid][pCash] < 80000)
				{
					TogglePlayerControllable(playerid, true);
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
				else
				{
					GiveMoney(playerid, -80000);
					GivePlayerGun(playerid, 24);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 1) // AK
			{
				if(PlayerInfo[playerid][pCash] < 180000)
				{
					TogglePlayerControllable(playerid, true);
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
				else
				{
					GiveMoney(playerid, -180000);
					GivePlayerGun(playerid, 30);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 2) //TEC9
			{

				if(PlayerInfo[playerid][pCash] < 50000)
				{
					TogglePlayerControllable(playerid, true);
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
				else
				{
					GiveMoney(playerid, -50000);
					GivePlayerGun(playerid, 32);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 3) //UZI
			{

				if(PlayerInfo[playerid][pCash] < 50000)
				{
					TogglePlayerControllable(playerid, true);
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
				else
				{
					GiveMoney(playerid, -50000);
					GivePlayerGun(playerid, 28);
					TogglePlayerControllable(playerid, true);
				}
			}
		}
		return 1;
	}
	if(dialogid == 100)
	{
		if(response)
		{
			BMPurchased[playerid] += 1;
			if(BMPurchased[playerid] == 2)
			{
				SetTimerEx("Timer_UseBM", 30000, 0, "i", playerid);
				UseBMTimer[playerid] = 1;
				SendClientMessage(playerid, COLOR_GREY, "You must wait 30 seconds before purchasing another weapon.");
			}
			if(listitem == 0) // Deagle
			{
				if(PlayerInfo[playerid][pCash] < 60000)
				{
					TogglePlayerControllable(playerid, true);
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
				else
				{
					GiveMoney(playerid, -60000);
					GivePlayerGun(playerid, 24);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 1) // Knife
			{
				if(PlayerInfo[playerid][pCash] < 1000000)
				{
					TogglePlayerControllable(playerid, true);
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
				else
				{
					GiveMoney(playerid, -1000000);
					GivePlayerGun(playerid, 4);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 2) // AK
			{
				if(PlayerInfo[playerid][pCash] < 100000)
				{
					TogglePlayerControllable(playerid, true);
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
				else
				{
					GiveMoney(playerid, -100000);
					GivePlayerGun(playerid, 30);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 3) //TEC9
			{

				if(PlayerInfo[playerid][pCash] < 30000)
				{
					TogglePlayerControllable(playerid, true);
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
				else
				{
					GiveMoney(playerid, -30000);
					GivePlayerGun(playerid, 32);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 4) //UZI
			{

				if(PlayerInfo[playerid][pCash] < 30000)
				{
					TogglePlayerControllable(playerid, true);
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
				else
				{
					GiveMoney(playerid, -30000);
					GivePlayerGun(playerid, 28);
					TogglePlayerControllable(playerid, true);
				}
			}
		}
		return 1;
	}
	if(dialogid == 3325)
	{
		if(response)
		{
			BMPurchased[playerid] += 1;
			if(BMPurchased[playerid] == 2)
			{
				SetTimerEx("Timer_UseBM", 30000, 0, "i", playerid);
				UseBMTimer[playerid] = 1;
				SendClientMessage(playerid, COLOR_GREY, "You must wait 30 seconds before purchasing another weapon.");
			}
			if(listitem == 0) // Deagle
			{
				if(PlayerInfo[playerid][pCash] < 40000)
				{
					TogglePlayerControllable(playerid, true);
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
				else
				{
					GiveMoney(playerid, -40000);
					GivePlayerGun(playerid, 24);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 1) // Knife
			{
				if(PlayerInfo[playerid][pCash] < 600000)
				{
					TogglePlayerControllable(playerid, true);
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
				else
				{
					GiveMoney(playerid, -600000);
					GivePlayerGun(playerid, 4);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 2) // AK
			{
				if(PlayerInfo[playerid][pCash] < 60000)
				{
					TogglePlayerControllable(playerid, true);
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
				else
				{
					GiveMoney(playerid, -60000);
					GivePlayerGun(playerid, 30);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 3) //TEC9
			{

				if(PlayerInfo[playerid][pCash] < 20000)
				{
					TogglePlayerControllable(playerid, true);
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
				else
				{
					GiveMoney(playerid, -20000);
					GivePlayerGun(playerid, 32);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 4) //UZI
			{

				if(PlayerInfo[playerid][pCash] < 20000)
				{
					TogglePlayerControllable(playerid, true);
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
				else
				{
					GiveMoney(playerid, -20000);
					GivePlayerGun(playerid, 28);
					TogglePlayerControllable(playerid, true);
				}
			}
			if(listitem == 5) //Sniper
			{

				if(PlayerInfo[playerid][pCash] < 180000)
				{
					TogglePlayerControllable(playerid, true);
					return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
				}
				else
				{
					GiveMoney(playerid, -180000);
					GivePlayerGun(playerid, 34);
					TogglePlayerControllable(playerid, true);
				}
			}
		}
		return 1;
	}
	if(dialogid == 101)
	{
		if(response)
			Dialog_NeonResponse(playerid, listitem);

		return 1;
	}
	if(dialogid == 102)
	{
		if(response)
		{
			switch(listitem)
			{
				case 0: // Fishing Rod
				{
					DisplayDialogForPlayer(playerid, 103);
				}
				case 1: // Fishing Bait
				{
					DisplayDialogForPlayer(playerid, 104);
				}
				case 2: // Fishing Line
				{
					DisplayDialogForPlayer(playerid, 105);
				}
			}
		}
		return 1;
	}
	if(dialogid == 103)
	{
		if(response)
		{
			switch(listitem)
			{
				case 0: // Old Rod
				{
					if(PlayerInfo[playerid][pCash] < 200)
					{
						TogglePlayerControllable(playerid, true);
						return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					}
					else
					{
						GiveMoney(playerid, -200);
						PlayerInfo[playerid][pFishRod] = 1;
						TogglePlayerControllable(playerid, true);
						SendClientMessage(playerid, COLOR_GREY, "You purchased an old rod.");
					}
				}
				case 1: // Beginners Rod
				{
					if(PlayerInfo[playerid][pCash] < 1000)
					{
						TogglePlayerControllable(playerid, true);
						return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					}
					else
					{
						GiveMoney(playerid, -1000);
						PlayerInfo[playerid][pFishRod] = 2;
						TogglePlayerControllable(playerid, true);
						SendClientMessage(playerid, COLOR_GREY, "You purchased a beginners rod.");
					}
				}
				case 2: // Fisherman's Rod
				{
					if(PlayerInfo[playerid][pCash] < 5000)
					{
						TogglePlayerControllable(playerid, true);
						return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					}
					else
					{
						GiveMoney(playerid, -5000);
						PlayerInfo[playerid][pFishRod] = 3;
						TogglePlayerControllable(playerid, true);
						SendClientMessage(playerid, COLOR_GREY, "You purchased a fishermans rod.");
					}
				}
				case 3: // Elite Rod
				{
					if(PlayerInfo[playerid][pCash] < 15000)
					{
						TogglePlayerControllable(playerid, true);
						return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					}
					else
					{
						GiveMoney(playerid, -15000);
						PlayerInfo[playerid][pFishRod] = 4;
						TogglePlayerControllable(playerid, true);
						SendClientMessage(playerid, COLOR_GREY, "You purchased an elite rod.");
					}
				}
			}
		}
		return 1;
	}
	if(dialogid == 104)
	{
		if(response)
		{
			switch(listitem)
			{
				case 0: // Weak Bait
				{
					if(PlayerInfo[playerid][pCash] < 150)
					{
						TogglePlayerControllable(playerid, true);
						return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					}
					else
					{
						GiveMoney(playerid, -150);
						PlayerInfo[playerid][pBait] = 1;
						TogglePlayerControllable(playerid, true);
						SendClientMessage(playerid, COLOR_GREY, "You purchased 10 pieces of weak bait.");
						PlayerInfo[playerid][pBaitAmount] = 10;
					}
				}
				case 1: // Regular Bait
				{
					if(PlayerInfo[playerid][pCash] < 500)
					{
						TogglePlayerControllable(playerid, true);
						return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					}
					else
					{
						GiveMoney(playerid, -500);
						PlayerInfo[playerid][pBait] = 2;
						TogglePlayerControllable(playerid, true);
						SendClientMessage(playerid, COLOR_GREY, "You purchased 10 pieces of regular bait.");
						PlayerInfo[playerid][pBaitAmount] = 10;
					}
				}
				case 2: // Strong Bait
				{
					if(PlayerInfo[playerid][pCash] < 1500)
					{
						TogglePlayerControllable(playerid, true);
						return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					}
					else
					{
						GiveMoney(playerid, -1500);
						PlayerInfo[playerid][pBait] = 3;
						TogglePlayerControllable(playerid, true);
						SendClientMessage(playerid, COLOR_GREY, "You purchased 10 pieces of strong bait.");
						PlayerInfo[playerid][pBaitAmount] = 10;
					}
				}
				case 3: // Elite Bait
				{
					if(PlayerInfo[playerid][pCash] < 2500)
					{
						TogglePlayerControllable(playerid, true);
						return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					}
					else
					{
						GiveMoney(playerid, -2500);
						PlayerInfo[playerid][pBait] = 4;
						TogglePlayerControllable(playerid, true);
						SendClientMessage(playerid, COLOR_GREY, "You purchased 10 pieces of elite bait.");
						PlayerInfo[playerid][pBaitAmount] = 10;
					}
				}
			}
		}
		return 1;
	}
	if(dialogid == 105)
	{
		if(response)
		{
			switch(listitem)
			{
				case 0: // Fishing Line
				{
					if(PlayerInfo[playerid][pCash] < 1000)
					{
						TogglePlayerControllable(playerid, true);
						return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					}
					else
					{
						GiveMoney(playerid, -1000);
						PlayerInfo[playerid][pLine] = 1;
						TogglePlayerControllable(playerid, true);
						SendClientMessage(playerid, COLOR_GREY, "You purchased a fishing line.");
					}
				}
			}
		}
		return 1;
	}
	if(dialogid == 107) //donut store
	{
		if(response)
		{
			new biz = GetPlayerVirtualWorld(playerid);
			if(strlen(inputtext))
			{
				if(!IsNumeric(inputtext))
				{
					return SendClientMessage(playerid, COLOR_GREY, "You must type a number.");
				}
				if(strcmp(inputtext, "1", true) == 0) //Sprunk
				{
					if(GetMoney(playerid) < 2)
					{
						return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money.");
					}
					SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);
					displayCenterHUDInfo(playerid,  "~r~Sprunk~w~ purchased!~n~type ~y~/usesoda~w~ to use, LMB to drink, press F to drop.", 8); //display for 8 seconds
					GiveMoney(playerid, -2);
					BizInfo[biz][bTill] += 2;
					BizInfo[biz][bProducts] -= 1;
				}
				else if(strcmp(inputtext, "2", true) == 0) //Coffee
				{
					if(GetMoney(playerid) < 5)
					{
						return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money.");
					}
					SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);
					displayCenterHUDInfo(playerid,  "~r~Coffee~w~ purchased!~n~type ~y~/usesoda~w~ to use, LMB to drink, press F to drop.", 8); //display for 8 seconds
					GiveMoney(playerid, -1);
					BizInfo[biz][bTill] += 2;
					BizInfo[biz][bProducts] -= 1;
				}
				else if(strcmp(inputtext, "3", true) == 0) //Donut Tray
				{
					if(boughtFoodTimer[playerid])
					{
						return SendClientMessage(playerid, COLOR_GREY, "You must wait 1 minute before buying another Meal.");
					}
					if(GetMoney(playerid) < 100)
					{
						return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money.");
					}
					if(hasFoodTray[playerid])
					{
						return SendClientMessage(playerid, COLOR_GREY, "You are already holding a meal, type /preparefood then type /eat to eat it.");
					}
					if(IsACop(playerid))
					{
						displayCenterHUDInfo(playerid,  "~r~Donut Tray~w~ purchased!~n~type ~y~/preparefood~w~ to prepare your meal~n~~w~then ~y~/eat~w~ to eat it.", 8); //display for 8 seconds
						format(str, sizeof(str), "* %s has bought a Donut Tray.", PlayerICName(playerid));
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						GiveMoney(playerid, -70);
					}
					else
					{
						displayCenterHUDInfo(playerid,  "~r~Donut Tray~w~ purchased!~n~type ~y~/preparefood~w~ to prepare your meal~n~~w~then ~y~/eat~w~ to eat it.", 8); //display for 8 seconds
						format(str, sizeof(str), "* %s has bought a Donut Tray.", PlayerICName(playerid));
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						GiveMoney(playerid, -100);
					}
					hasFoodTray[playerid] = 4; //donuts
					givePlayerFoodTray(playerid, hasFoodTray[playerid]);
					BizInfo[biz][bTill] += 2;
					BizInfo[biz][bProducts] -= 1;
					SetTimerEx("boughtFood", 60 *1000, 0, "i", playerid); //1 minute
					boughtFoodTimer[playerid] = true;
				}
				if(BizInfo[biz][bProducts] <= 0) SetNoProductsDate(biz);
			}
		}
		return 1;
	}
	//radio stuff
	if(dialogid == 108)
	{
		if(response)
		{
			if(listitem == 0)
			{
				//di.fm
				DisplayDialogForPlayer(playerid, 109);
			}
			else if(listitem == 1)
			{
				//soma.fm
				DisplayDialogForPlayer(playerid, 110);
			}
			else if(listitem == 2)
			{
				//etn.fm
				DisplayDialogForPlayer(playerid, 111);
			}
			else if(listitem == 3)
			{
				//infowars radio
				DisplayDialogForPlayer(playerid, 112);
			}
			else if(listitem == 4)
			{
				// custom
				DisplayDialogForPlayer(playerid, 113);
			}
			else if(listitem == 5)
			{
				// turn off
				usingRadio[playerid] = 0;
				StopAudioStreamForPlayer(playerid);
			}
		}
		return 1;
	}
	if(dialogid == 109)
	{
		//di.fm
		if(response)
		{
			switch(listitem)
			{
				case 0:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/classicvocaltrance.pls");
					usingRadio[playerid] = 1;
				}
				case 1:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/ukgarage.pls");
					usingRadio[playerid] = 1;
				}
				case 2:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/trance.pls");
					usingRadio[playerid] = 1;
				}
				case 3:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/vocaltrance.pls");
					usingRadio[playerid] = 1;
				}
				case 4:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/techhouse.pls");
					usingRadio[playerid] = 1;
				}
				case 5:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/psychill.pls");
					usingRadio[playerid] = 1;
				}
				case 6:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/chilloutdreams.pls");
					usingRadio[playerid] = 1;
				}
				case 7:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/classictrance.pls");
					usingRadio[playerid] = 1;
				}
				case 8:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/eurodance.pls");
					usingRadio[playerid] = 1;
				}
				case 9:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/club.pls");
					usingRadio[playerid] = 1;
				}
				case 10:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/discohouse.pls");
					usingRadio[playerid] = 1;
				}
				case 11:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/dubstep.pls");
					usingRadio[playerid] = 1;
				}
				case 12:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/electro.pls");
					usingRadio[playerid] = 1;
				}
				case 13:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/chillout.pls");
					usingRadio[playerid] = 1;
				}
				case 14:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/classiceurodance.pls");
					usingRadio[playerid] = 1;
				}
				case 15:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/house.pls");
					usingRadio[playerid] = 1;
				}
				case 16:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/soulfulhouse.pls");
					usingRadio[playerid] = 1;
				}
				case 17:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/funkyhouse.pls");
					usingRadio[playerid] = 1;
				}
				case 18:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/tribalhouse.pls");
					usingRadio[playerid] = 1;
				}
				case 19:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/harddance.pls");
					usingRadio[playerid] = 1;
				}
				case 20:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/minimal.pls");
					usingRadio[playerid] = 1;
				}
				case 21:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/techno.pls");
					usingRadio[playerid] = 1;
				}
				case 22:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/progressive.pls");
					usingRadio[playerid] = 1;
				}
				case 23:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/goapsy.pls");
					usingRadio[playerid] = 1;
				}
				case 24:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/hardstyle.pls");
					usingRadio[playerid] = 1;
				}
				case 25:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/hardcore.pls");
					usingRadio[playerid] = 1;
				}
				case 26:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/lounge.pls");
					usingRadio[playerid] = 1;
				}
				case 27:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/exposurenyc.pls");
					usingRadio[playerid] = 1;
				}
				case 28:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/djmixes.pls");
					usingRadio[playerid] = 1;
				}
				case 29:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/drumandbass.pls");
					usingRadio[playerid] = 1;
				}
				case 30:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/liquiddnb.pls");
					usingRadio[playerid] = 1;
				}
				case 31:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/classictechno.pls");
					usingRadio[playerid] = 1;
				}
				case 32:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/breaks.pls");
					usingRadio[playerid] = 1;
				}
				case 33:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/ambient.pls");
					usingRadio[playerid] = 1;
				}
				case 34:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/spacemusic.pls");
					usingRadio[playerid] = 1;
				}
				case 35:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/futuresynthpop.pls");
					usingRadio[playerid] = 1;
				}
				case 36:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/oldschoolhouse.pls");
					usingRadio[playerid] = 1;
				}
				case 37:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/chiptunes.pls");
					usingRadio[playerid] = 1;
				}
				case 38:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/gabber.pls");
					usingRadio[playerid] = 1;
				}
				case 39:
				{
					PlayAudioStreamForPlayer(playerid, "http://listen.di.fm/public3/latinhouse.pls");
					usingRadio[playerid] = 1;
				}
			}
		}
		else
		{
			DisplayDialogForPlayer(playerid, 108); //radio main
		}
		return 1;
	}
	// Trunk start
	if(dialogid == TRUNK_PUT_STUFF) //Trunk put stuff
	{
		if(response)
		{
			if(listitem == 0) //Put gun
			{
				if(PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can use this."); //selling to another
				if(IsACop(playerid) == 1 || IsANG(playerid) == 1 || IsAMedic(playerid) == 1 || IsASenate(playerid) || PlayerInfo[playerid][pMember] == FACTION_SANG) return SendClientMessage(playerid, COLOR_GRAD1, "Cops/DEA/SANG/DoC/SAFMD can not use this.");
				ShowPlayerDialog(playerid, TRUNKPUTGUN, DIALOG_STYLE_LIST, "Put a gun in this trunk", "Slot 1\nSlot 2", "Put", "Cancel"); //Trunk put gun
				return 1;
			}
			if(listitem == 1) //Put armor
			{
				if(IsACop(playerid) == 1 || IsANG(playerid) == 1 || IsAMedic(playerid) == 1 || IsASenate(playerid) || PlayerInfo[playerid][pMember] == 9 || PlayerInfo[playerid][pMember] == FACTION_SANG) return SendClientMessage(playerid, COLOR_GRAD1, "Cops/DEA/SANG/DoC/SAFMD/News can not use this.");
				new counter = 0;
				new vid;
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				for(new i; i != MAX_VEHICLES; i++)
				{
					new dist = CheckPlayerDistanceToVehicle(4, playerid, i);
					if(dist)
					{
						vid = i;
						counter++;
					}
				}
				switch(counter)
				{
					case 0:
					{
						SendClientMessage(playerid, COLOR_GREY, "No cars with trunk near you");
						return 1;
					}
					case 1:
					{
						if(IsPlayerInAnyVehicle(playerid))
						{
							SendClientMessage(playerid, COLOR_GREY, "You can't use the trunk while you're in the car");
							return 1;
						}
						if(IsAPlane(vid) || IsABike(vid) || IsAHelicopter(vid) || IsATrain(vid) || IsABoat(vid) || IsABus(vid) || GetVehicleModel(GetPlayerVehicleID(vid)) == 523)
						{
							SendClientMessage(playerid, COLOR_GREY, "This vehicle doesn't have a trunk.");
							return 1;
						}
						if(TrunkInfo[vid][cTrunkOpened] != 1)
						{
							SendClientMessage(playerid, COLOR_RED, "You need to open the trunk first");
							return 1;
						}
						GetPlayerArmour(playerid, PlayerInfo[playerid][pArmor]);
						if(PlayerInfo[playerid][pArmor] != 0)
						{
							TrunkInfo[vid][cArmor] = PlayerInfo[playerid][pArmor];
							SetPlayerArmour(playerid, 0);
							PlayerInfo[playerid][pArmor] = 0;
							format(str, sizeof(str), "* %s puts a vest in the trunk.", PlayerICName(playerid));
							ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
							return 1;
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "You haven't got any armour ");
							return 1;
						}
					}
					default:
					{
						SendClientMessage(playerid, COLOR_GREY, "Found more then one car in range");
						return 1;
					}
				}
				return 1;
			}
			if(listitem == 2) // Put cocaine
			{
				ShowPlayerDialog(playerid, TRUNK_PUT_COCAINE, DIALOG_STYLE_INPUT, "Put cocaine", "How many grams of cocaine do you want to put?", "Put", "Cancel"); //Trunk put crack
			}
			if(listitem == 3) // Put cannabis
			{
				ShowPlayerDialog(playerid, TRUNK_PUT_CANNABIS, DIALOG_STYLE_INPUT, "Put cannabis", "How many grams of cannabis do you want to put?", "Put", "Cancel"); //Trunk put pot
			}
			if(listitem == 4) // Put meth
			{
				ShowPlayerDialog(playerid, TRUNK_PUT_METH, DIALOG_STYLE_INPUT, "Put meth", "How many grams of meth do you want to put?", "Put", "Cancel"); //Trunk put pot
			}
			if(listitem == 5) // Put xanax
			{
				ShowPlayerDialog(playerid, TRUNK_PUT_XANAX, DIALOG_STYLE_INPUT, "Put xanax", "How many tabs of xanax do you want to put?", "Put", "Cancel"); //Trunk put pot
			}
			if(listitem == 6) //Put food tray
			{
				new counter = 0;
				new vid;
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				for(new i; i != MAX_VEHICLES; i++)
				{
					new dist = CheckPlayerDistanceToVehicle(4, playerid, i);
					if(dist)
					{
						vid = i;
						counter++;
					}
				}
				switch(counter)
				{
					case 0:
					{
						SendClientMessage(playerid, COLOR_GREY, "No cars with trunk near you");
						return 1;
					}
					case 1:
					{
						if(IsPlayerInAnyVehicle(playerid))
						{
							SendClientMessage(playerid, COLOR_GREY, "You can't use the trunk while you're in the car.");
							return 1;
						}
						if(IsAPlane(vid) || IsABike(vid) || IsAHelicopter(vid) || IsATrain(vid) || IsABoat(vid) || IsABus(vid) || GetVehicleModel(GetPlayerVehicleID(vid)) == 523)
						{
							SendClientMessage(playerid, COLOR_GREY, "This vehicle doesn't have a trunk.");
							return 1;
						}
						if(TrunkInfo[vid][cTrunkOpened] != 1)
						{
							SendClientMessage(playerid, COLOR_RED, "You need to open the trunk first.");
							return 1;
						}
						if(!hasFoodTray[playerid])
						{
							SendClientMessage(playerid, COLOR_GREY, "You are not carrying a food tray.");
							return 1;
						}
						if(TrunkInfo[vid][cFoodtray])
						{
							SendClientMessage(playerid, COLOR_GREY, "There's already a food tray in the trunk.");
							return 1;
						}
						TrunkInfo[vid][cFoodtray] = hasFoodTray[playerid];
						throwAwayFoodTray(playerid);
						format(str, sizeof(str), "* %s puts a food tray in the trunk.", PlayerICName(playerid));
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						return 1;
					}
					default:
					{
						SendClientMessage(playerid, COLOR_GREY, "Found more then one car in range.");
						return 1;
					}
				}
				return 1;
			}
		}
		return 1;
	}
	if(dialogid == TRUNKPUTGUN) //Trunk put gun
	{
		if(response)
		{
			if(listitem == 0) //Put gun 1
			{
				new counter = 0;
				new vid;
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				for(new i; i != MAX_VEHICLES; i++)
				{
					new dist = CheckPlayerDistanceToVehicle(4, playerid, i);
					if(dist)
					{
						vid = i;
						counter++;
					}
				}
				switch(counter)
				{
					case 0:
					{
						SendClientMessage(playerid, COLOR_GREY, "No cars with trunk near you");
					}
					case 1:
					{
						if(TrunkInfo[vid][cGun1] == 0)
						{
							if(IsPlayerInAnyVehicle(playerid))
							{
								SendClientMessage(playerid, COLOR_GREY, "You can't use the trunk while you're in the car");
								return 1;
							}
							if(IsAPlane(vid) || IsABike(vid) || IsAHelicopter(vid) || IsATrain(vid) || IsABoat(vid) || IsABus(vid) || GetVehicleModel(GetPlayerVehicleID(vid)) == 523)
							{
								SendClientMessage(playerid, COLOR_GREY, "This vehicle doesn't have a trunk.");
								return 1;
							}
							if(TrunkInfo[vid][cTrunkOpened] != 1)
							{
								SendClientMessage(playerid, COLOR_RED, "You need to open the trunk first");
								return 1;
							}
							new buffer[512];
							new gunname[100];
							new gunID = GetPlayerWeapon(playerid);
							new pgun0 = PlayerInfo[playerid][pGun0];
							new pgun1 = PlayerInfo[playerid][pGun1];
							new pgun2 = PlayerInfo[playerid][pGun2];
							new pgun3 = PlayerInfo[playerid][pGun3];
							new pgun4 = PlayerInfo[playerid][pGun4];
							new pgun5 = PlayerInfo[playerid][pGun5];
							new pgun6 = PlayerInfo[playerid][pGun6];
							new pgun7 = PlayerInfo[playerid][pGun7];
							new pgun8 = PlayerInfo[playerid][pGun8];
							new pgun9 = PlayerInfo[playerid][pGun9];
							new pgun10 = PlayerInfo[playerid][pGun10];
							new pgun11 = PlayerInfo[playerid][pGun11];
							new pgun12 = PlayerInfo[playerid][pGun12];
							if(pgun0 != gunID && pgun1 != gunID && pgun2 != gunID && pgun3 != gunID && pgun4 != gunID && pgun5 != gunID && pgun6 != gunID && pgun7 != gunID && pgun8 != gunID && pgun9 != gunID && pgun10 != gunID && pgun11 != gunID && pgun12 != gunID)
							{
								SendClientMessage(playerid, COLOR_GRAD1, "You can't place admin given weapons into a trunk.");
								return 1;
							}
							if(gunID != 0)
							{
								if(gunID == 1)
								{
									PlayerInfo[playerid][pGun0] = 0;
								}
								if(gunID == 2)
								{
									PlayerInfo[playerid][pGun1] = 0;
								}
								if(gunID == 3)
								{
									PlayerInfo[playerid][pGun1] = 0;
								}
								if(gunID == 4)
								{
									PlayerInfo[playerid][pGun1] = 0;
								}
								if(gunID == 5)
								{
									PlayerInfo[playerid][pGun1] = 0;
								}
								if(gunID == 6)
								{
									PlayerInfo[playerid][pGun1] = 0;
								}
								if(gunID == 7)
								{
									PlayerInfo[playerid][pGun1] = 0;
								}
								if(gunID == 8)
								{
									PlayerInfo[playerid][pGun1] = 0;
								}
								if(gunID == 9)
								{
									PlayerInfo[playerid][pGun1] = 0;
								}
								if(gunID == 10)
								{
									PlayerInfo[playerid][pGun10] = 0;
								}
								if(gunID == 11)
								{
									PlayerInfo[playerid][pGun10] = 0;
								}
								if(gunID == 12)
								{
									PlayerInfo[playerid][pGun10] = 0;
								}
								if(gunID == 13)
								{
									PlayerInfo[playerid][pGun10] = 0;
								}
								if(gunID == 14)
								{
									PlayerInfo[playerid][pGun10] = 0;
								}
								if(gunID == 15)
								{
									PlayerInfo[playerid][pGun10] = 0;
								}
								if(gunID == 16)
								{
									PlayerInfo[playerid][pGun8] = 0;
								}
								if(gunID == 17)
								{
									PlayerInfo[playerid][pGun8] = 0;
								}
								if(gunID == 18)
								{
									PlayerInfo[playerid][pGun8] = 0;
								}
								if(gunID == 22)
								{
									PlayerInfo[playerid][pGun2] = 0;
								}
								if(gunID == 23)
								{
									PlayerInfo[playerid][pGun2] = 0;
								}
								if(gunID == 24)
								{
									PlayerInfo[playerid][pGun2] = 0;
								}
								if(gunID == 25)
								{
									PlayerInfo[playerid][pGun3] = 0;
								}
								if(gunID == 26)
								{
									PlayerInfo[playerid][pGun3] = 0;
								}
								if(gunID == 27)
								{
									PlayerInfo[playerid][pGun3] = 0;
								}
								if(gunID == 28)
								{
									PlayerInfo[playerid][pGun4] = 0;
								}
								if(gunID == 29)
								{
									PlayerInfo[playerid][pGun4] = 0;
								}
								if(gunID == 30)
								{
									PlayerInfo[playerid][pGun5] = 0;
								}
								if(gunID == 31)
								{
									PlayerInfo[playerid][pGun5] = 0;
								}
								if(gunID == 32)
								{
									PlayerInfo[playerid][pGun4] = 0;
								}
								if(gunID == 33)
								{
									PlayerInfo[playerid][pGun6] = 0;
								}
								if(gunID == 34)
								{
									PlayerInfo[playerid][pGun6] = 0;
								}
								if(gunID == 35)
								{
									PlayerInfo[playerid][pGun7] = 0;
								}
								if(gunID == 36)
								{
									PlayerInfo[playerid][pGun7] = 0;
								}
								if(gunID == 37)
								{
									PlayerInfo[playerid][pGun7] = 0;
								}
								if(gunID == 38)
								{
									PlayerInfo[playerid][pGun7] = 0;
								}
								if(gunID == 39)
								{
									PlayerInfo[playerid][pGun8] = 0;
								}
								if(gunID == 40)
								{
									PlayerInfo[playerid][pGun12] = 0;
								}
								if(gunID == 41)
								{
									PlayerInfo[playerid][pGun9] = 0;
								}
								if(gunID == 42)
								{
									PlayerInfo[playerid][pGun9] = 0;
								}
								if(gunID == 43)
								{
									PlayerInfo[playerid][pGun9] = 0;
								}
								if(gunID == 44)
								{
									PlayerInfo[playerid][pGun11] = 0;
								}
								if(gunID == 45)
								{
									PlayerInfo[playerid][pGun11] = 0;
								}
								if(gunID == 46)
								{
									PlayerInfo[playerid][pGun11] = 0;
								}
								GetWeaponName(gunID, gunname, sizeof(gunname));
								TrunkInfo[vid][cGun1] = gunID;
								format(buffer, sizeof(buffer), "You have put your %s in the car's trunk.", gunname);
								SendClientMessage(playerid, COLOR_WHITE, buffer);
								SetPlayerWeapons(playerid);
								format(str, sizeof(str), "* %s puts a %s in the trunk.", PlayerICName(playerid), gunname);
								ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
								return 1;
							}
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "This slot is already taken");
							return 1;
						}
					}
					default:
					{
						SendClientMessage(playerid, COLOR_GREY, "Found more then one car in range");
						return 1;
					}
				}
				return 1;
			}
			if(listitem == 1) //Put gun 2
			{
				new counter = 0;
				new vid;
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				for(new i; i != MAX_VEHICLES; i++)
				{
					new dist = CheckPlayerDistanceToVehicle(4, playerid, i);
					if(dist)
					{
						vid = i;
						counter++;
					}
				}
				switch(counter)
				{
					case 0:
					{
						SendClientMessage(playerid, COLOR_GREY, "No cars with trunk near you");
					}
					case 1:
					{
						if(TrunkInfo[vid][cGun2] == 0)
						{
							if(IsPlayerInAnyVehicle(playerid))
							{
								SendClientMessage(playerid, COLOR_GREY, "You can't use the trunk while you're in the car");
								return 1;
							}
							if(IsAPlane(vid) || IsABike(vid) || IsAHelicopter(vid) || IsATrain(vid) || IsABoat(vid) || IsABus(vid) || GetVehicleModel(GetPlayerVehicleID(vid)) == 523)
							{
								SendClientMessage(playerid, COLOR_GREY, "This vehicle doesn't have a trunk.");
								return 1;
							}
							if(TrunkInfo[vid][cTrunkOpened] != 1)
							{
								SendClientMessage(playerid, COLOR_RED, "You need to open the trunk first");
								return 1;
							}
							new buffer[512];
							new gunname[100];
							new gunID = GetPlayerWeapon(playerid);
							new pgun0 = PlayerInfo[playerid][pGun0];
							new pgun1 = PlayerInfo[playerid][pGun1];
							new pgun2 = PlayerInfo[playerid][pGun2];
							new pgun3 = PlayerInfo[playerid][pGun3];
							new pgun4 = PlayerInfo[playerid][pGun4];
							new pgun5 = PlayerInfo[playerid][pGun5];
							new pgun6 = PlayerInfo[playerid][pGun6];
							new pgun7 = PlayerInfo[playerid][pGun7];
							new pgun8 = PlayerInfo[playerid][pGun8];
							new pgun9 = PlayerInfo[playerid][pGun9];
							new pgun10 = PlayerInfo[playerid][pGun10];
							new pgun11 = PlayerInfo[playerid][pGun11];
							new pgun12 = PlayerInfo[playerid][pGun12];
							if(pgun0 != gunID && pgun1 != gunID && pgun2 != gunID && pgun3 != gunID && pgun4 != gunID && pgun5 != gunID && pgun6 != gunID && pgun7 != gunID && pgun8 != gunID && pgun9 != gunID && pgun10 != gunID && pgun11 != gunID && pgun12 != gunID)
							{
								SendClientMessage(playerid, COLOR_GRAD1, "You can't place admin given weapons into a trunk.");
								return 1;
							}
							if(gunID != 0)
							{
								if(gunID == 1)
								{
									PlayerInfo[playerid][pGun0] = 0;
								}
								if(gunID == 2)
								{
									PlayerInfo[playerid][pGun1] = 0;
								}
								if(gunID == 3)
								{
									PlayerInfo[playerid][pGun1] = 0;
								}
								if(gunID == 4)
								{
									PlayerInfo[playerid][pGun1] = 0;
								}
								if(gunID == 5)
								{
									PlayerInfo[playerid][pGun1] = 0;
								}
								if(gunID == 6)
								{
									PlayerInfo[playerid][pGun1] = 0;
								}
								if(gunID == 7)
								{
									PlayerInfo[playerid][pGun1] = 0;
								}
								if(gunID == 8)
								{
									PlayerInfo[playerid][pGun1] = 0;
								}
								if(gunID == 9)
								{
									PlayerInfo[playerid][pGun1] = 0;
								}
								if(gunID == 10)
								{
									PlayerInfo[playerid][pGun10] = 0;
								}
								if(gunID == 11)
								{
									PlayerInfo[playerid][pGun10] = 0;
								}
								if(gunID == 12)
								{
									PlayerInfo[playerid][pGun10] = 0;
								}
								if(gunID == 13)
								{
									PlayerInfo[playerid][pGun10] = 0;
								}
								if(gunID == 14)
								{
									PlayerInfo[playerid][pGun10] = 0;
								}
								if(gunID == 15)
								{
									PlayerInfo[playerid][pGun10] = 0;
								}
								if(gunID == 16)
								{
									PlayerInfo[playerid][pGun8] = 0;
								}
								if(gunID == 17)
								{
									PlayerInfo[playerid][pGun8] = 0;
								}
								if(gunID == 18)
								{
									PlayerInfo[playerid][pGun8] = 0;
								}
								if(gunID == 22)
								{
									PlayerInfo[playerid][pGun2] = 0;
								}
								if(gunID == 23)
								{
									PlayerInfo[playerid][pGun2] = 0;
								}
								if(gunID == 24)
								{
									PlayerInfo[playerid][pGun2] = 0;
								}
								if(gunID == 25)
								{
									PlayerInfo[playerid][pGun3] = 0;
								}
								if(gunID == 26)
								{
									PlayerInfo[playerid][pGun3] = 0;
								}
								if(gunID == 27)
								{
									PlayerInfo[playerid][pGun3] = 0;
								}
								if(gunID == 28)
								{
									PlayerInfo[playerid][pGun4] = 0;
								}
								if(gunID == 29)
								{
									PlayerInfo[playerid][pGun4] = 0;
								}
								if(gunID == 30)
								{
									PlayerInfo[playerid][pGun5] = 0;
								}
								if(gunID == 31)
								{
									PlayerInfo[playerid][pGun5] = 0;
								}
								if(gunID == 32)
								{
									PlayerInfo[playerid][pGun4] = 0;
								}
								if(gunID == 33)
								{
									PlayerInfo[playerid][pGun6] = 0;
								}
								if(gunID == 34)
								{
									PlayerInfo[playerid][pGun6] = 0;
								}
								if(gunID == 35)
								{
									PlayerInfo[playerid][pGun7] = 0;
								}
								if(gunID == 36)
								{
									PlayerInfo[playerid][pGun7] = 0;
								}
								if(gunID == 37)
								{
									PlayerInfo[playerid][pGun7] = 0;
								}
								if(gunID == 38)
								{
									PlayerInfo[playerid][pGun7] = 0;
								}
								if(gunID == 39)
								{
									PlayerInfo[playerid][pGun8] = 0;
								}
								if(gunID == 40)
								{
									PlayerInfo[playerid][pGun12] = 0;
								}
								if(gunID == 41)
								{
									PlayerInfo[playerid][pGun9] = 0;
								}
								if(gunID == 42)
								{
									PlayerInfo[playerid][pGun9] = 0;
								}
								if(gunID == 43)
								{
									PlayerInfo[playerid][pGun9] = 0;
								}
								if(gunID == 44)
								{
									PlayerInfo[playerid][pGun11] = 0;
								}
								if(gunID == 45)
								{
									PlayerInfo[playerid][pGun11] = 0;
								}
								if(gunID == 46)
								{
									PlayerInfo[playerid][pGun11] = 0;
								}
								GetWeaponName(gunID, gunname, sizeof(gunname));
								TrunkInfo[vid][cGun2] = gunID;
								format(buffer, sizeof(buffer), "You have put your %s in the car's trunk.", gunname);
								SendClientMessage(playerid, COLOR_WHITE, buffer);
								SetPlayerWeapons(playerid);
								format(str, sizeof(str), "* %s puts a %s in the trunk.", PlayerICName(playerid), gunname);
								ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
								return 1;
							}
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "This slot is already taken");
							return 1;
						}
					}
					default:
					{
						SendClientMessage(playerid, COLOR_GREY, "Found more then one car in range");
						return 1;
					}
				}
				return 1;
			}
		}
		return 1;
	}
	if(dialogid == TRUNKTAKEGUN) //Trunk take gun
	{
		if(response)
		{
			if(listitem == 0) //Take gun 1
			{
				new counter = 0;
				new vid;
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				for(new i; i != MAX_VEHICLES; i++)
				{
					new dist = CheckPlayerDistanceToVehicle(4, playerid, i);
					if(dist)
					{
						vid = i;
						counter++;
					}
				}
				switch(counter)
				{
					case 0:
					{
						SendClientMessage(playerid, COLOR_GREY, "No cars with trunk near you");
						return 1;
					}
					case 1:
					{
						if(IsPlayerInAnyVehicle(playerid))
						{
							SendClientMessage(playerid, COLOR_GREY, "You can't use the trunk while you're in the car");
							return 1;
						}
						if(IsAPlane(vid) || IsABike(vid) || IsAHelicopter(vid) || IsATrain(vid) || IsABoat(vid) || IsABus(vid) || GetVehicleModel(GetPlayerVehicleID(vid)) == 523)
						{
							SendClientMessage(playerid, COLOR_GREY, "This vehicle doesn't have a trunk.");
							return 1;
						}
						if(TrunkInfo[vid][cTrunkOpened] != 1)
						{
							SendClientMessage(playerid, COLOR_RED, "You need to open the trunk first");
							return 1;
						}
						if(TrunkInfo[vid][cGun1] != 0)
						{
							new buffer[512];
							new gunName[100];
							GivePlayerGun(playerid, TrunkInfo[vid][cGun1]);
							GetWeaponName(TrunkInfo[vid][cGun1], gunName, sizeof(gunName));
							format(buffer, sizeof(buffer), "You've taken a %s from the vehicle.", gunName);
							SendClientMessage(playerid, COLOR_WHITE, buffer);
							TrunkInfo[vid][cGun1] = 0;
							format(str, sizeof(str), "* %s takes a %s from the trunk.", PlayerICName(playerid), gunName);
							ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
							return 1;
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "This slot is empty.");
							return 1;
						}
					}
					default:
					{
						SendClientMessage(playerid, COLOR_GREY, "Found more then one car in range");
						return 1;
					}
				}
				return 1;
			}
			if(listitem == 1) //Take gun 2
			{
				new counter = 0;
				new vid;
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				for(new i; i != MAX_VEHICLES; i++)
				{
					new dist = CheckPlayerDistanceToVehicle(4, playerid, i);
					if(dist)
					{
						vid = i;
						counter++;
					}
				}
				switch(counter)
				{
					case 0:
					{
						SendClientMessage(playerid, COLOR_GREY, "No cars with trunk near you");
						return 1;
					}
					case 1:
					{
						if(IsPlayerInAnyVehicle(playerid))
						{
							SendClientMessage(playerid, COLOR_GREY, "You can't use the trunk while you're in the car");
							return 1;
						}
						if(IsAPlane(vid) || IsABike(vid) || IsAHelicopter(vid) || IsATrain(vid) || IsABoat(vid) || IsABus(vid) || GetVehicleModel(GetPlayerVehicleID(vid)) == 523)
						{
							SendClientMessage(playerid, COLOR_GREY, "This vehicle doesn't have a trunk.");
							return 1;
						}
						if(TrunkInfo[vid][cTrunkOpened] != 1)
						{
							SendClientMessage(playerid, COLOR_RED, "You need to open the trunk first");
							return 1;
						}
						if(TrunkInfo[vid][cGun2] != 0)
						{
							new buffer[512];
							new gunName[100];
							GivePlayerGun(playerid, TrunkInfo[vid][cGun2]);
							GetWeaponName(TrunkInfo[vid][cGun2], gunName, sizeof(gunName));
							format(buffer, sizeof(buffer), "You've taken a %s from the vehicle.", gunName);
							SendClientMessage(playerid, COLOR_WHITE, buffer);
							TrunkInfo[vid][cGun2] = 0;
							format(str, sizeof(str), "* %s takes a %s from the trunk.", PlayerICName(playerid), gunName);
							ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
							return 1;
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "This slot is empty.");
							return 1;
						}
					}
					default:
					{
						SendClientMessage(playerid, COLOR_GREY, "Found more then one car in range");
						return 1;
					}
				}
				return 1;
			}
		}
		return 1;
	}
	if(dialogid == TRUNK_TAKE_STUFF) //Trunk take stuff
	{
		if(response)
		{
			if(listitem == 0) //Take gun
			{
				if(PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can get a weapon.");
				ShowPlayerDialog(playerid, TRUNKTAKEGUN, DIALOG_STYLE_LIST, "Take a gun from this trunk", "Slot 1\nSlot 2", "Take", "Cancel"); //Trunk put gun
				return 1;
			}
			if(listitem == 1) //Take armor
			{
				new counter = 0;
				new vid;
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				for(new i; i != MAX_VEHICLES; i++)
				{
					new dist = CheckPlayerDistanceToVehicle(4, playerid, i);
					if(dist)
					{
						vid = i;
						counter++;
					}
				}
				switch(counter)
				{
					case 0:
					{
						SendClientMessage(playerid, COLOR_GREY, "No cars with trunk near you");
						return 1;
					}
					case 1:
					{
						if(IsPlayerInAnyVehicle(playerid))
						{
							SendClientMessage(playerid, COLOR_GREY, "You can't use the trunk while you're in the car.");
							return 1;
						}
						if(IsAPlane(vid) || IsABike(vid) || IsAHelicopter(vid) || IsATrain(vid) || IsABoat(vid) || IsABus(vid) || GetVehicleModel(GetPlayerVehicleID(vid)) == 523)
						{
							SendClientMessage(playerid, COLOR_GREY, "This vehicle doesn't have a trunk.");
							return 1;
						}
						if(TrunkInfo[vid][cTrunkOpened] != 1)
						{
							SendClientMessage(playerid, COLOR_RED, "You need to open the trunk first.");
							return 1;
						}
						if(TrunkInfo[vid][cArmor] != 0)
						{
							SetPlayerArmour(playerid, TrunkInfo[vid][cArmor]);
							PlayerInfo[playerid][pArmor] = TrunkInfo[vid][cArmor];
							TrunkInfo[vid][cArmor] = 0;
							format(str, sizeof(str), "* %s takes a vest from the trunk.", PlayerICName(playerid));
							ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
							return 1;
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "This trunk haven't got any vest inside ");
							return 1;
						}
					}
					default:
					{
						SendClientMessage(playerid, COLOR_GREY, "Found more then one car in range");
						return 1;
					}
				}
				return 1;
			}
			if(listitem == 2) //Take cocaine
			{
				ShowPlayerDialog(playerid, TRUNK_TAKE_COCAINE, DIALOG_STYLE_INPUT, "Take cocaine", "How many grams of crack do you want to take?", "Take", "Cancel"); //Trunk take crack
			}
			if(listitem == 3) //Take cannabis
			{
				ShowPlayerDialog(playerid, TRUNK_TAKE_CANNABIS, DIALOG_STYLE_INPUT, "Take cannabis", "How many grams of pot do you want to take?", "Take", "Cancel"); //Trunk take pot
			}
			if(listitem == 4) //Take meth
			{
				ShowPlayerDialog(playerid, TRUNK_TAKE_METH, DIALOG_STYLE_INPUT, "Take meth", "How many grams of  do you want to take?", "Take", "Cancel"); //Trunk take pot
			}
			if(listitem == 5) //Take xanax
			{
				ShowPlayerDialog(playerid, TRUNK_TAKE_XANAX, DIALOG_STYLE_INPUT, "Take xanax", "How many tabs of xanax do you want to take?", "Take", "Cancel"); //Trunk take pot
			}
			if(listitem == 6) //Take food tray
			{
				new counter = 0;
				new vid;
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				for(new i; i != MAX_VEHICLES; i++)
				{
					new dist = CheckPlayerDistanceToVehicle(4, playerid, i);
					if(dist)
					{
						vid = i;
						counter++;
					}
				}
				switch(counter)
				{
					case 0:
					{
						SendClientMessage(playerid, COLOR_GREY, "No cars with trunk near you");
						return 1;
					}
					case 1:
					{
						if(IsPlayerInAnyVehicle(playerid))
						{
							SendClientMessage(playerid, COLOR_GREY, "You can't use the trunk while you're in the car.");
							return 1;
						}
						if(IsAPlane(vid) || IsABike(vid) || IsAHelicopter(vid) || IsATrain(vid) || IsABoat(vid) || IsABus(vid) || GetVehicleModel(GetPlayerVehicleID(vid)) == 523)
						{
							SendClientMessage(playerid, COLOR_GREY, "This vehicle doesn't have a trunk.");
							return 1;
						}
						if(TrunkInfo[vid][cTrunkOpened] != 1)
						{
							SendClientMessage(playerid, COLOR_RED, "You need to open the trunk first.");
							return 1;
						}
						if(hasFoodTray[playerid])
						{
							SendClientMessage(playerid, COLOR_GREY, "You're already holding a food tray.");
							return 1;
						}
						if(TrunkInfo[vid][cFoodtray])
						{
							hasFoodTray[playerid] = TrunkInfo[vid][cFoodtray];
							givePlayerFoodTray(playerid, TrunkInfo[vid][cFoodtray]);
							TrunkInfo[vid][cFoodtray] = 0;
							format(str, sizeof(str), "* %s takes a food tray from the trunk.", PlayerICName(playerid));
							ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
							return 1;
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "There's no food tray in the trunk.");
							return 1;
						}
					}
					default:
					{
						SendClientMessage(playerid, COLOR_GREY, "Found more then one car in range");
						return 1;
					}
				}
				return 1;
			}
		}
		return 1;
	}
	if(dialogid == TRUNK_TAKE_COCAINE) // Withdraw cocaine
	{
		if(response)
		{
			new counter = 0;
			new vid;
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			new cocaine = strval(inputtext);
			for(new i; i != MAX_VEHICLES; i++)
			{
				new dist = CheckPlayerDistanceToVehicle(4, playerid, i);
				if(dist)
				{
					vid = i;
					counter++;
				}
			}
			switch(counter)
			{
				case 0:
				{
					SendClientMessage(playerid, COLOR_GREY, "No cars with trunk near you");
					return 1;
				}
				case 1:
				{
					if(IsPlayerInAnyVehicle(playerid))
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't use the trunk while you're in the car");
						return 1;
					}
					if(IsAPlane(vid) || IsABike(vid) || IsAHelicopter(vid) || IsATrain(vid) || IsABoat(vid) || IsABus(vid) || GetVehicleModel(GetPlayerVehicleID(vid)) == 523)
					{
						SendClientMessage(playerid, COLOR_GREY, "This vehicle doesn't have a trunk.");
						return 1;
					}
					if(TrunkInfo[vid][cTrunkOpened] != 1)
					{
						SendClientMessage(playerid, COLOR_RED, "You need to open the trunk first");
						return 1;
					}
					new pcocaine = PlayerInfo[playerid][pCocaine];
					new calc = cocaine + pcocaine;
					if(cocaine < 1)
					{
						SendClientMessage(playerid, COLOR_GREY, "Invalid amount");
						return 1;
					}
					if(calc > 25)
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't have that much cocaine on you");
						return 1;
					}
					if(cocaine > TrunkInfo[vid][cCocaine])
					{
						SendClientMessage(playerid, COLOR_GREY, "You don't have that much in the trunk");
						return 1;
					}
					if(TrunkInfo[vid][cCocaine] != 0)
					{
						PlayerInfo[playerid][pCocaine] += cocaine;
						TrunkInfo[vid][cCocaine] -= cocaine;
						format(str, sizeof(str), "* %s takes some cocaine from the trunk.", PlayerICName(playerid));
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "This trunk haven't got any cocaine inside ");
						return 1;
					}
				}
				default:
				{
					SendClientMessage(playerid, COLOR_GREY, "Found more then one car in range");
					return 1;
				}
			}
			return 1;
		}
		return 1;
	}
	if(dialogid == TRUNK_PUT_CANNABIS) // Deposit Pot
	{
		if(response)
		{
			new counter = 0;
			new vid;
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			new cannabis = strval(inputtext);
			for(new i; i != MAX_VEHICLES; i++)
			{
				new dist = CheckPlayerDistanceToVehicle(4, playerid, i);
				if(dist)
				{
					vid = i;
					counter++;
				}
			}
			switch(counter)
			{
				case 0:
				{
					SendClientMessage(playerid, COLOR_GREY, "No cars with trunk near you");
					return 1;
				}
				case 1:
				{
					if(IsPlayerInAnyVehicle(playerid))
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't use the trunk while you're in the car");
						return 1;
					}
					if(IsAPlane(vid) || IsABike(vid) || IsAHelicopter(vid) || IsATrain(vid) || IsABoat(vid) || IsABus(vid) || GetVehicleModel(GetPlayerVehicleID(vid)) == 523)
					{
						SendClientMessage(playerid, COLOR_GREY, "This vehicle doesn't have a trunk.");
						return 1;
					}
					if(TrunkInfo[vid][cTrunkOpened] != 1)
					{
						SendClientMessage(playerid, COLOR_RED, "You need to open the trunk first");
						return 1;
					}
					new tCannabis = TrunkInfo[vid][cCannabis];
					new calc = cannabis + tCannabis;
					if(cannabis < 1)
					{
						SendClientMessage(playerid, COLOR_GREY, "Invalid amount");
						return 1;
					}
					if(calc > 250)
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't have that much cannabis in your trunk");
						return 1;
					}
					if(cannabis > PlayerInfo[playerid][pCannabis])
					{
						SendClientMessage(playerid, COLOR_GREY, "You don't have that much on you");
						return 1;
					}
					if(PlayerInfo[playerid][pCannabis] != 0)
					{
						PlayerInfo[playerid][pCannabis] -= cannabis;
						TrunkInfo[vid][cCannabis] += cannabis;
						format(str, sizeof(str), "* %s puts some cannabis in the trunk.", PlayerICName(playerid));
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "You don't have any cannabis on you ");
						return 1;
					}
				}
				default:
				{
					SendClientMessage(playerid, COLOR_GREY, "Found more then one car in range");
					return 1;
				}
			}
			return 1;
		}
		return 1;
	}
	if(dialogid == TRUNK_PUT_METH) // Withdraw  meth
	{
		if(response)
		{
			new counter = 0, vid;

			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);

			new meth = strval(inputtext);

			for(new i; i != MAX_VEHICLES; i++)
			{
				new dist = CheckPlayerDistanceToVehicle(4, playerid, i);
				if(dist)
				{
					vid = i;
					counter++;
				}
			}
			switch(counter)
			{
				case 0:
				{
					SendClientMessage(playerid, COLOR_GREY, "No cars with trunk near you");
				}

				case 1:
				{
					if(IsPlayerInAnyVehicle(playerid))
							return SendClientMessage(playerid, COLOR_GREY, "You can't use the trunk while you're in the car");

					if(IsAPlane(vid) || IsABike(vid) || IsAHelicopter(vid) || IsATrain(vid) || IsABoat(vid) || IsABus(vid) || GetVehicleModel(GetPlayerVehicleID(vid)) == 523)
							return SendClientMessage(playerid, COLOR_GREY, "This vehicle doesn't have a trunk.");

					if(TrunkInfo[vid][cTrunkOpened] != 1)
							return SendClientMessage(playerid, COLOR_RED, "You need to open the trunk first");

					new tMeth = TrunkInfo[vid][cMeth];
					new calc = meth + tMeth;

					if(meth < 1)
							return SendClientMessage(playerid, COLOR_GREY, "Invalid amount");

					if(calc > 250)
							return SendClientMessage(playerid, COLOR_GREY, "You can't have that much meth in your trunk");

					if(meth > PlayerInfo[playerid][pMeth])
							return SendClientMessage(playerid, COLOR_GREY, "You don't have that much on you");

					if(PlayerInfo[playerid][pMeth] != 0)
					{
						PlayerInfo[playerid][pMeth] -= meth;
						TrunkInfo[vid][cMeth] += meth;

						format(str, sizeof(str), "* %s puts some meth in the trunk.", PlayerICName(playerid));
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "You don't have any cannabis on you ");
						return 1;
					}
				}
				default:
				{
					SendClientMessage(playerid, COLOR_GREY, "Found more then one car in range");
					return 1;
				}
			}
		}
		return 1;
	}
	if(dialogid == TRUNK_PUT_COCAINE) //Put Crack
	{
		if(response)
		{
			new counter = 0;
			new vid;
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			new crack = strval(inputtext);
			for(new i; i != MAX_VEHICLES; i++)
			{
				new dist = CheckPlayerDistanceToVehicle(4, playerid, i);
				if(dist)
				{
					vid = i;
					counter++;
				}
			}
			switch(counter)
			{
				case 0:
				{
					SendClientMessage(playerid, COLOR_GREY, "No cars with trunk near you");
					return 1;
				}
				case 1:
				{
					if(IsPlayerInAnyVehicle(playerid))
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't use the trunk while you're in the car");
						return 1;
					}
					if(IsAPlane(vid) || IsABike(vid) || IsAHelicopter(vid) || IsATrain(vid) || IsABoat(vid) || IsABus(vid) || GetVehicleModel(GetPlayerVehicleID(vid)) == 523)
					{
						SendClientMessage(playerid, COLOR_GREY, "This vehicle doesn't have a trunk.");
						return 1;
					}
					if(TrunkInfo[vid][cTrunkOpened] != 1)
					{
						SendClientMessage(playerid, COLOR_RED, "You need to open the trunk first");
						return 1;
					}
					new tcrack = TrunkInfo[vid][cCocaine];
					new calc = crack +tcrack;
					if(crack < 1)
					{
						SendClientMessage(playerid, COLOR_GREY, "Invalid amount");
						return 1;
					}
					if(calc > 250)
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't have that much cocaine in your trunk");
						return 1;
					}
					if(crack > PlayerInfo[playerid][pCocaine])
					{
						SendClientMessage(playerid, COLOR_GREY, "You don't have that much on you");
						return 1;
					}
					if(PlayerInfo[playerid][pCocaine] != 0)
					{
						PlayerInfo[playerid][pCocaine] -= crack;
						TrunkInfo[vid][cCocaine] += crack;
						format(str, sizeof(str), "* %s puts some cocaine in the trunk.", PlayerICName(playerid));
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "You don't have any crack on you ");
						return 1;
					}
				}
				default:
				{
					SendClientMessage(playerid, COLOR_GREY, "Found more then one car in range");
					return 1;
				}
			}
			return 1;
		}
		return 1;
	}
	if(dialogid == TRUNK_PUT_XANAX) //Put xanax
	{
		if(response)
		{
			new counter = 0;
			new vid;
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			new crack = strval(inputtext);
			for(new i; i != MAX_VEHICLES; i++)
			{
				new dist = CheckPlayerDistanceToVehicle(4, playerid, i);
				if(dist)
				{
					vid = i;
					counter++;
				}
			}
			switch(counter)
			{
				case 0:
				{
					SendClientMessage(playerid, COLOR_GREY, "No cars with trunk near you");
					return 1;
				}
				case 1:
				{
					if(IsPlayerInAnyVehicle(playerid))
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't use the trunk while you're in the car");
						return 1;
					}
					if(IsAPlane(vid) || IsABike(vid) || IsAHelicopter(vid) || IsATrain(vid) || IsABoat(vid) || IsABus(vid) || GetVehicleModel(GetPlayerVehicleID(vid)) == 523)
					{
						SendClientMessage(playerid, COLOR_GREY, "This vehicle doesn't have a trunk.");
						return 1;
					}
					if(TrunkInfo[vid][cTrunkOpened] != 1)
					{
						SendClientMessage(playerid, COLOR_RED, "You need to open the trunk first");
						return 1;
					}
					new tcrack = TrunkInfo[vid][cXanax];
					new calc = crack +tcrack;
					if(crack < 1)
					{
						SendClientMessage(playerid, COLOR_GREY, "Invalid amount");
						return 1;
					}
					if(calc > 250)
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't have that much xanax in your trunk");
						return 1;
					}
					if(crack > PlayerInfo[playerid][pXanax])
					{
						SendClientMessage(playerid, COLOR_GREY, "You don't have that much on you");
						return 1;
					}
					if(PlayerInfo[playerid][pXanax] != 0)
					{
						PlayerInfo[playerid][pXanax] -= crack;
						TrunkInfo[vid][cXanax] += crack;
						format(str, sizeof(str), "* %s puts some xanax in the trunk.", PlayerICName(playerid));
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "You don't have any crack on you ");
						return 1;
					}
				}
				default:
				{
					SendClientMessage(playerid, COLOR_GREY, "Found more then one car in range");
					return 1;
				}
			}
			return 1;
		}
		return 1;
	}
	if(dialogid == TRUNK_TAKE_CANNABIS) //Take pot
	{
		if(response)
		{
			new counter = 0;
			new vid;
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			new pot = strval(inputtext);
			for(new i; i != MAX_VEHICLES; i++)
			{
				new dist = CheckPlayerDistanceToVehicle(4, playerid, i);
				if(dist)
				{
					vid = i;
					counter++;
				}
			}
			switch(counter)
			{
				case 0:
				{
					SendClientMessage(playerid, COLOR_GREY, "No cars with trunk near you");
					return 1;
				}
				case 1:
				{
					if(IsPlayerInAnyVehicle(playerid))
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't use the trunk while you're in the car");
						return 1;
					}
					if(IsAPlane(vid) || IsABike(vid) || IsAHelicopter(vid) || IsATrain(vid) || IsABoat(vid) || IsABus(vid) || GetVehicleModel(GetPlayerVehicleID(vid)) == 523)
					{
						SendClientMessage(playerid, COLOR_GREY, "This vehicle doesn't have a trunk.");
						return 1;
					}
					if(TrunkInfo[vid][cTrunkOpened] != 1)
					{
						SendClientMessage(playerid, COLOR_RED, "You need to open the trunk first");
						return 1;
					}
					new ppot = PlayerInfo[playerid][pCannabis];
					new calc = pot +ppot;
					if(pot < 1)
					{
						SendClientMessage(playerid, COLOR_GREY, "Invalid amount");
						return 1;
					}
					if(calc > 50)
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't have that much cannabis on you");
						return 1;
					}
					if(pot > TrunkInfo[vid][cCannabis])
					{
						SendClientMessage(playerid, COLOR_GREY, "You don't have that much in the trunk");
						return 1;
					}
					if(TrunkInfo[vid][cCannabis] != 0)
					{
						PlayerInfo[playerid][pCannabis] += pot;
						TrunkInfo[vid][cCannabis] -= pot;
						format(str, sizeof(str), "* %s takes some pot from the trunk.", PlayerICName(playerid));
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "This trunk haven't got any pot inside ");
						return 1;
					}
				}
				default:
				{
					SendClientMessage(playerid, COLOR_GREY, "Found more then one car in range");
					return 1;
				}
			}
			return 1;
		}
		return 1;
	}
	if(dialogid == TRUNK_TAKE_METH) //Take Meth
	{
		if(response)
		{
			new counter = 0;
			new vid;
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			new pot = strval(inputtext);
			for(new i; i != MAX_VEHICLES; i++)
			{
				new dist = CheckPlayerDistanceToVehicle(4, playerid, i);
				if(dist)
				{
					vid = i;
					counter++;
				}
			}
			switch(counter)
			{
				case 0:
				{
					SendClientMessage(playerid, COLOR_GREY, "No cars with trunk near you");
					return 1;
				}
				case 1:
				{
					if(IsPlayerInAnyVehicle(playerid))
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't use the trunk while you're in the car");
						return 1;
					}
					if(IsAPlane(vid) || IsABike(vid) || IsAHelicopter(vid) || IsATrain(vid) || IsABoat(vid) || IsABus(vid) || GetVehicleModel(GetPlayerVehicleID(vid)) == 523)
					{
						SendClientMessage(playerid, COLOR_GREY, "This vehicle doesn't have a trunk.");
						return 1;
					}
					if(TrunkInfo[vid][cTrunkOpened] != 1)
					{
						SendClientMessage(playerid, COLOR_RED, "You need to open the trunk first");
						return 1;
					}
					new ppot = PlayerInfo[playerid][pMeth];
					new calc = pot +ppot;
					if(pot < 1)
					{
						SendClientMessage(playerid, COLOR_GREY, "Invalid amount");
						return 1;
					}
					if(calc > 50)
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't have that much meth on you");
						return 1;
					}
					if(pot > TrunkInfo[vid][cMeth])
					{
						SendClientMessage(playerid, COLOR_GREY, "You don't have that much in the trunk");
						return 1;
					}
					if(TrunkInfo[vid][cMeth] != 0)
					{
						PlayerInfo[playerid][pMeth] += pot;
						TrunkInfo[vid][cMeth] -= pot;
						format(str, sizeof(str), "* %s takes some meth from the trunk.", PlayerICName(playerid));
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "This trunk haven't got any meth inside ");
						return 1;
					}
				}
				default:
				{
					SendClientMessage(playerid, COLOR_GREY, "Found more then one car in range");
					return 1;
				}
			}
			return 1;
		}
		return 1;
	}
	if(dialogid == TRUNK_TAKE_XANAX) //Take XANAX
	{
		if(response)
		{
			new counter = 0;
			new vid;
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			new pot = strval(inputtext);
			for(new i; i != MAX_VEHICLES; i++)
			{
				new dist = CheckPlayerDistanceToVehicle(4, playerid, i);
				if(dist)
				{
					vid = i;
					counter++;
				}
			}
			switch(counter)
			{
				case 0:
				{
					SendClientMessage(playerid, COLOR_GREY, "No cars with trunk near you");
					return 1;
				}
				case 1:
				{
					if(IsPlayerInAnyVehicle(playerid))
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't use the trunk while you're in the car");
						return 1;
					}
					if(IsAPlane(vid) || IsABike(vid) || IsAHelicopter(vid) || IsATrain(vid) || IsABoat(vid) || IsABus(vid) || GetVehicleModel(GetPlayerVehicleID(vid)) == 523)
					{
						SendClientMessage(playerid, COLOR_GREY, "This vehicle doesn't have a trunk.");
						return 1;
					}
					if(TrunkInfo[vid][cTrunkOpened] != 1)
					{
						SendClientMessage(playerid, COLOR_RED, "You need to open the trunk first");
						return 1;
					}
					new ppot = PlayerInfo[playerid][pXanax];
					new calc = pot +ppot;
					if(pot < 1)
					{
						SendClientMessage(playerid, COLOR_GREY, "Invalid amount");
						return 1;
					}
					if(calc > 50)
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't have that much xanax on you");
						return 1;
					}
					if(pot > TrunkInfo[vid][cXanax])
					{
						SendClientMessage(playerid, COLOR_GREY, "You don't have that much in the trunk");
						return 1;
					}
					if(TrunkInfo[vid][cXanax] != 0)
					{
						PlayerInfo[playerid][pXanax] += pot;
						TrunkInfo[vid][cXanax] -= pot;
						format(str, sizeof(str), "* %s takes some xanax from the trunk.", PlayerICName(playerid));
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "This trunk haven't got any xanax inside ");
						return 1;
					}
				}
				default:
				{
					SendClientMessage(playerid, COLOR_GREY, "Found more then one car in range");
					return 1;
				}
			}
			return 1;
		}
		return 1;
	}
	if(dialogid == TRUNKDIALOG) //Trunk
	{
		if(response)
		{
			if(listitem == 0) //Open / Close
			{
				new engine, lights, alarm, doors, bonnet, boot, objective;
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				if(TrunkInfo[IsByVehTrunk[playerid]][cTrunkOpened] == 0)
				{
					GetVehicleParamsEx(IsByVehTrunk[playerid], engine, lights, alarm, doors, bonnet, boot, objective);
					SetVehicleParamsEx(IsByVehTrunk[playerid], engine, lights, alarm, doors, bonnet, VEHICLE_PARAMS_ON, objective);
					TrunkInfo[IsByVehTrunk[playerid]][cTrunkOpened] = 1;
					format(str, sizeof(str), "* %s has opened the trunk.", PlayerICName(playerid));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					return 1;
				}
				else if(TrunkInfo[IsByVehTrunk[playerid]][cTrunkOpened] == 1)
				{
					GetVehicleParamsEx(IsByVehTrunk[playerid], engine, lights, alarm, doors, bonnet, boot, objective);
					SetVehicleParamsEx(IsByVehTrunk[playerid], engine, lights, alarm, doors, bonnet, VEHICLE_PARAMS_OFF, objective);
					TrunkInfo[IsByVehTrunk[playerid]][cTrunkOpened] = 0;
					format(str, sizeof(str), "* %s has closed the trunk.", PlayerICName(playerid));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					return 1;
				}

				return 1;
			}
			if(listitem == 1) //Check
			{
				new counter = 0;
				new vid;
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				for(new i; i != MAX_VEHICLES; i++)
				{
					new dist = CheckPlayerDistanceToVehicle(4, playerid, i);
					if(dist)
					{
						vid = i;
						counter++;
					}
				}
				switch(counter)
				{
					case 0:
					{
						SendClientMessage(playerid, COLOR_GREY, "No cars with trunk near you");
					}
					case 1:
					{
						if(IsPlayerInAnyVehicle(playerid))
						{
							SendClientMessage(playerid, COLOR_GREY, "You can't use the trunk while you're in the car");
							return 1;
						}
						if(TrunkInfo[vid][cTrunkOpened] != 1)
						{
							SendClientMessage(playerid, COLOR_RED, "You need to open the trunk first");
							return 1;
						}
						if(IsAPlane(vid) || IsABike(vid) || IsAHelicopter(vid) || IsATrain(vid) || IsABoat(vid) || IsABus(vid) || GetVehicleModel(GetPlayerVehicleID(vid)) == 523)
						{
							SendClientMessage(playerid, COLOR_GREY, "This vehicle doesn't have a trunk.");
							return 1;
						}
						new gunname1[64];
						if(TrunkInfo[vid][cGun1] != 0)
						{
							GetWeaponName(TrunkInfo[vid][cGun1], gunname1, sizeof(gunname1));
						}
						else
						{
							gunname1 = "Empty";
						}
						new gunname2[64];
						if(TrunkInfo[vid][cGun2] != 0)
						{
							GetWeaponName(TrunkInfo[vid][cGun2], gunname2, sizeof(gunname2));
						}
						else
						{
							gunname2 = "Empty";
						}
						new coordsstring[128];
						SendClientMessage(playerid, COLOR_GREEN, "_______________________________________");
						format(coordsstring, sizeof(coordsstring), "Gun Slot 1: %s", gunname1);
						SendClientMessage(playerid, COLOR_WHITE, coordsstring);
						format(coordsstring, sizeof(coordsstring), "Gun Slot 2: %s", gunname2);
						SendClientMessage(playerid, COLOR_WHITE, coordsstring);
						format(coordsstring, sizeof(coordsstring), "Armor: %0.2f", TrunkInfo[vid][cArmor]);
						SendClientMessage(playerid, COLOR_WHITE, coordsstring);
						format(coordsstring, sizeof(coordsstring), "Cocaine: %d", TrunkInfo[vid][cCocaine]);
						SendClientMessage(playerid, COLOR_WHITE, coordsstring);
						format(coordsstring, sizeof(coordsstring), "Cannabis: %d", TrunkInfo[vid][cCannabis]);
						SendClientMessage(playerid, COLOR_WHITE, coordsstring);
						format(coordsstring, sizeof(coordsstring), "Meth: %d", TrunkInfo[vid][cMeth]);
						SendClientMessage(playerid, COLOR_WHITE, coordsstring);
						format(coordsstring, sizeof(coordsstring), "Xanax: %d", TrunkInfo[vid][cXanax]);
						SendClientMessage(playerid, COLOR_WHITE, coordsstring);
						format(coordsstring, sizeof(coordsstring), "Food Tray: %s", getFoodTypeById(TrunkInfo[vid][cFoodtray]));
						SendClientMessage(playerid, COLOR_WHITE, coordsstring);
						SendClientMessage(playerid, COLOR_GREEN, "_______________________________________");
						format(str, sizeof(str), "* %s looks inside the trunk and checks it.", PlayerICName(playerid));
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						return 1;
					}
					default:
					{
						SendClientMessage(playerid, COLOR_GREY, "Found more then one car in range");
						return 1;
					}
				}
				return 1;
			}
			if(listitem == 2) //Put stuff
			{
				ShowPlayerDialog(playerid, TRUNK_PUT_STUFF, DIALOG_STYLE_LIST, "Put a gun in this trunk", "Put gun\nPut vest\nPut cocaine\nPut cannabis\nPut meth\nPut xanax\nPut food tray", "Put", "Cancel"); //Trunk put stuff
			}
			if(listitem == 3) //Take stuff
			{

				ShowPlayerDialog(playerid, TRUNK_TAKE_STUFF, DIALOG_STYLE_LIST, "Take a gun from this trunk", "Take gun\nTake vest\nTake crack\nTake pot\nTake meth\nTake xanax\nTake food tray", "Take", "Cancel"); //Trunk take stuff
			}
		}
		return 1;
	}
	if(dialogid == 110)   //somafm
	{
		if(response)
		{
			switch(listitem)
			{
				case 0:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/groovesalad.pls");
					usingRadio[playerid] = 1;
				}
				case 1:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/dronezone.pls");
					usingRadio[playerid] = 1;
				}
				case 2:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/christmas.pls");
					usingRadio[playerid] = 1;
				}
				case 3:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/lush.pls");
					usingRadio[playerid] = 1;
				}
				case 4:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/indiepop.pls");
					usingRadio[playerid] = 1;
				}
				case 5:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/spacestation.pls");
					usingRadio[playerid] = 1;
				}
				case 6:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/secretagent.pls");
					usingRadio[playerid] = 1;
				}
				case 7:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/xmasinfrisko.pls");
					usingRadio[playerid] = 1;
				}
				case 8:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/beatblender.pls");
					usingRadio[playerid] = 1;
				}
				case 9:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/suburbsofgoa.pls");
					usingRadio[playerid] = 1;
				}
				case 10:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/bootliquor.pls");
					usingRadio[playerid] = 1;
				}
				case 11:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/u80s192.pls");
					usingRadio[playerid] = 1;
				}
				case 12:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/poptron.pls");
					usingRadio[playerid] = 1;
				}
				case 13:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/cliqhop.pls");
					usingRadio[playerid] = 1;
				}
				case 14:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/sonicuniverse192.pls");
					usingRadio[playerid] = 1;
				}
				case 15:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/missioncontrol.pls");
					usingRadio[playerid] = 1;
				}
				case 16:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/digitalis.pls");
					usingRadio[playerid] = 1;
				}
				case 17:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/illstreet.pls");
					usingRadio[playerid] = 1;
				}
				case 18:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/tags.pls");
					usingRadio[playerid] = 1;
				}
				case 19:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/doomed.pls");
					usingRadio[playerid] = 1;
				}
				case 20:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/covers.pls");
					usingRadio[playerid] = 1;
				}
				case 21:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/480min.pls");
					usingRadio[playerid] = 1;
				}
				case 22:
				{
					PlayAudioStreamForPlayer(playerid, "http://somafm.com/brfm.pls");
					usingRadio[playerid] = 1;
				}
			}
		}
		else
		{
			DisplayDialogForPlayer(playerid, 108); //radio main
		}
		return 1;
	}
	if(dialogid == 111)   //etn.fm
	{
		if(response)
		{
			switch(listitem)
			{
				case 0:
				{
					PlayAudioStreamForPlayer(playerid, "http://etn.fm/playlists/etn1beta.m3u");
					usingRadio[playerid] = 1;
				}
				case 1:
				{
					PlayAudioStreamForPlayer(playerid, "http://etn.fm/playlists/etn2beta.m3u");
					usingRadio[playerid] = 1;
				}
			}
		}
		else
		{
			DisplayDialogForPlayer(playerid, 108); //radio main
		}
		return 1;
	}
	if(dialogid == 112)   //talk radio
	{
		if(response)
		{
			switch(listitem)
			{
				case 0:
				{
					PlayAudioStreamForPlayer(playerid, "http://www.infowars.com/stream.pls");
					usingRadio[playerid] = 1;
				}
			}
		}
		else
		{
			DisplayDialogForPlayer(playerid, 108); //radio main
		}
		return 1;
	}
	if(dialogid == 113)
	{
		//custom radio
		if(response)
		{
			if(!strlen(inputtext))
			{
				SendClientMessage(playerid, COLOR_WHITE, "You must enter a stream url.");
				DisplayDialogForPlayer(playerid, 113);
				return 1;
			}
			else
			{
				if(strcmp(inputtext, "youtube", true) != -1) return SendClientMessage(playerid, COLOR_GRAD1, "Youtube links are disabled! Try mp3skull.com.");
				PlayAudioStreamForPlayer(playerid, inputtext); // New str seemed redundant.
				usingRadio[playerid] = 1;
			}
		}
		else
		{
			DisplayDialogForPlayer(playerid, 108); //radio main
		}
		return 1;
	}
	if(dialogid == 919)
	{
		if(response == 1)
		{
			if(IsPlayerInRangeOfPoint(playerid, 7.0, 1545.24, -1011.07, 23.65))
			{
				BParkedCar[playerid] = 1;
				SetDynamicObjectRot(BPLG1, 0, 0, -259);
				SetTimer("Timer_BPLgateclose", 3500, false);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "Watchman: Go ahead and park your car.");
			}
			else if(IsPlayerInRangeOfPoint(playerid, 7.0, 1634.00, -1138.31, 23.68))
			{
				BParkedCar[playerid] = 1;
				SetDynamicObjectRot(BPLG2, 0, 0, 0);
				SetTimer("Timer_BPLgateclose", 3500, false);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "Watchman: Go ahead and park your car.");
			}
		}
		return 1;
	}
	//calculator
	if(dialogid == DIALOG_CALC_1)
	{
		if(response)
		{
			switch(listitem)
			{
				case 0:
				{
					Calc_Option[playerid] = 1; // Multiply
					ShowPlayerDialog(playerid, DIALOG_CALC_2, DIALOG_STYLE_INPUT, "Calculator", "Please choose a number you would like to multiply.", "Times", "Cancel");
				}
				case 1:
				{
					Calc_Option[playerid] = 2; // Add
					ShowPlayerDialog(playerid, DIALOG_CALC_2, DIALOG_STYLE_INPUT, "Calculator", "Please choose a number you would like to add.", "Plus", "Cancel");
				}
				case 2:
				{
					Calc_Option[playerid] = 3; // Divide
					ShowPlayerDialog(playerid, DIALOG_CALC_2, DIALOG_STYLE_INPUT, "Calculator", "Please choose a number you would like to divide.", "Divided By", "Cancel");
				}
				case 3:
				{
					Calc_Option[playerid] = 4; // Substract
					ShowPlayerDialog(playerid, DIALOG_CALC_2, DIALOG_STYLE_INPUT, "Calculator", "Please choose a number you would like to substract.", "Minus", "Cancel");
				}
			}
		}
		return 1;
	}
	if(dialogid == DIALOG_CALC_2)
	{
		if(IsNumeric(inputtext))
		{
			Calc_Numb1[playerid] = strval(inputtext);
			ShowPlayerDialog(playerid, DIALOG_CALC_3, 1, "Calculator", "Please choose the second number", "Equals", "Cancel");
			return 1;
		}
	}
	if(dialogid == DIALOG_CALC_3)
	{
		if(IsNumeric(inputtext))
		{
			new sz_String[128];
			Calc_Numb2[playerid] = strval(inputtext);
			if(Calc_Option[playerid] == 1) // Multiply
			{
				Calc_Result[playerid] = Calc_Numb1[playerid] * Calc_Numb2[playerid];
				format(sz_String, 128, "Calculator: %d * %d = %d", Calc_Numb1[playerid], Calc_Numb2[playerid], Calc_Result[playerid]);
			}
			if(Calc_Option[playerid] == 2) // Plus
			{
				Calc_Result[playerid] = Calc_Numb1[playerid] + Calc_Numb2[playerid];
				format(sz_String, 128, "Calculator: %d + %d = %d", Calc_Numb1[playerid], Calc_Numb2[playerid], Calc_Result[playerid]);
			}
			if(Calc_Option[playerid] == 3) // Divide
			{
				Calc_Result[playerid] = Calc_Numb1[playerid] / Calc_Numb2[playerid];
				format(sz_String, 128, "Calculator: %d / %d = %d", Calc_Numb1[playerid], Calc_Numb2[playerid], Calc_Result[playerid]);
			}
			if(Calc_Option[playerid] == 4) // Substract
			{
				Calc_Result[playerid] = Calc_Numb1[playerid] - Calc_Numb2[playerid];
				format(sz_String, 128, "Calculator: %d - %d = %d", Calc_Numb1[playerid], Calc_Numb2[playerid], Calc_Result[playerid]);
			}
			SendClientMessage(playerid, COLOR_GREY, sz_String);
		}
		return 1;
	}
	//radio dialogs
	if(dialogid == DIALOG_EDIT_ON)
	{
		if(!response)
		{
			editingBoomboxID[playerid] = -1; //no longer editing
			EDITING_TYPE[playerid] = EDIT_RADIO_NONE;
			return 1;
		}
		if(listitem == 0)   //turn on
		{
			switch(BoomboxInfo[editingBoomboxID[playerid]][boomboxTurnedOn])
			{
				case 0:   //if off
				{
					//set radio url top default
					if(!BoomboxInfo[editingBoomboxID[playerid]][BoomboxURL])   //radio url is empty, play a random station
					{
						new defaultStation[128];
						new rand = random(3);
						switch(rand)
						{
							case 0:
							{
								format(defaultStation, 128, "http://ch1relay1.etn.fm:8130/listen.pls?sid=1");
							}
							case 1:
							{
								format(defaultStation, 128, "http://somafm.com/indiepop.pls");
							}
							case 2:
							{
								format(defaultStation, 128, "http://www.infowars.com/stream.pls");
							}
						}
						format(BoomboxInfo[editingBoomboxID[playerid]][BoomboxURL], 256, defaultStation);
						BoomboxInfo[editingBoomboxID[playerid]][boomboxTurnedOn] = 1;
						BoomboxInfo[editingBoomboxID[playerid]][BoomboxWorld] = GetPlayerVirtualWorld(playerid);
						editingBoomboxID[playerid] = -1; //no longer editing
						EDITING_TYPE[playerid] = EDIT_RADIO_NONE;
					}
					else	 //just turn it on
					{
						BoomboxInfo[editingBoomboxID[playerid]][boomboxTurnedOn] = 1;
						editingBoomboxID[playerid] = -1; //no longer editing
						EDITING_TYPE[playerid] = EDIT_RADIO_NONE;
					}
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have turned on the boombox.");
				}
				case 1:   //if on
				{
					BoomboxInfo[editingBoomboxID[playerid]][boomboxTurnedOn] = 0;
					BoomboxInfo[editingBoomboxID[playerid]][BoomboxWorld] = GetPlayerVirtualWorld(playerid);
					editingBoomboxID[playerid] = -1; //no longer editing
					EDITING_TYPE[playerid] = EDIT_RADIO_NONE;
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have turned off the boombox.");
				}
			}
		}
		if(listitem == 1)   //change station
		{
			ShowPlayerDialog(playerid, DIALOG_STATIONS, DIALOG_STYLE_LIST, "Radio stations", "SARP Official Radio\nRadio Stations\nCustom", "Select", "Cancel");
			return 1;
		}
		if(listitem == 2)   //lock
		{
			if(editingBoomboxID[playerid] != playerid)
			{
				SendClientMessage(playerid, COLOR_GREY, "You must be the owner to lock the boombox.");
				editingBoomboxID[playerid] = -1;
				EDITING_TYPE[playerid] = EDIT_RADIO_NONE;
				return 1;
			}
			new string[128];
			new playername[MAX_PLAYER_NAME];
			GetPlayerName(playerid, playername, sizeof(playername));
			new bbid = editingBoomboxID[playerid];
			if(BoomboxInfo[bbid][locked])
			{
				format(string, sizeof(string), "%s's Stereo\n{FFFFFF}/edit", playername);
				BoomboxInfo[bbid][locked] = 0;
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have unlocked the boombox.");
			}
			else
			{
				format(string, sizeof(string), "%s's Stereo\n{FFFFFF}Locked", playername);
				BoomboxInfo[bbid][locked] = 1;
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have locked the boombox.");
			}
			UpdateDynamic3DTextLabelText(BoomboxInfo[bbid][bbLabelID], 0x6E88FFFF, string);
			editingBoomboxID[playerid] = -1; //no longer editing
			EDITING_TYPE[playerid] = EDIT_RADIO_NONE;
			return 1;
		}
		if(listitem == 3)   //pickup
		{
			if(PlayerInfo[playerid][pStereo] != 0 && editingBoomboxID[playerid] != playerid)   //if trying to pickup another persons boombox and i already have own, don't let me
			{
				SendClientMessage(playerid, COLOR_GREY, "You already have a boombox.");
				editingBoomboxID[playerid] = -1; //no longer editing
				EDITING_TYPE[playerid] = EDIT_RADIO_NONE;
				return 1;
			}
			// http://www.gta-SARP.com/forums/showthread.php?114184
			if(PlayerInfo[editingBoomboxID[playerid]][pStereo] == 4 && PlayerInfo[playerid][pDonateRank] != 3 && editingBoomboxID[playerid] != playerid)
			{
				SendClientMessage(playerid, COLOR_GREY, "You must be a Gold VIP to carry a large boombox.");
				editingBoomboxID[playerid] = -1; //no longer editing
				EDITING_TYPE[playerid] = EDIT_RADIO_NONE;
				return 1;
			}
			pickupBoombox(playerid);
			editingBoomboxID[playerid] = -1; //no longer editing
			EDITING_TYPE[playerid] = EDIT_RADIO_NONE;
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have picked up the boombox.");
		}
		return 1;
	}
	if(dialogid == DIALOG_STATIONS)
	{
		if(!response)
		{
			if(EDITING_TYPE[playerid] == EDIT_RADIO_BOOMBOX)
			{
				displayBoomboxEditDialog(playerid);
			}
			else if(EDITING_TYPE[playerid] == EDIT_RADIO_VEHICLE)
			{
				EDITING_TYPE[playerid] = EDIT_RADIO_NONE;
			}
			return 1;
		}
		if(listitem == 0)   //ECRP Official Radio
		{
			new rstring[1000], name[32], rgenre[32];
			for(new i = 0; i < sizeof(radioInfo); i++)
			{
				format(rgenre, sizeof(rgenre), "%s", radioInfo[i][genreName]);
				format(name, sizeof(name), "%s", radioInfo[i][channelName]);
				format(rstring, sizeof(rstring), "%s %s - %s\n", rstring, rgenre, name);
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST1, DIALOG_STYLE_LIST, "SARP Official Radio", rstring, "Select", "Cancel");
			return 1;
		}
		if(listitem == 1)   //display genre
		{
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_GENRES, DIALOG_STYLE_LIST, "Radio Stations", "Alternative\nBlues\nClassical\nCountry\nDecades\nEasy Listening\nElectronic\nFolk\nInspirational\nInternational\nJazz\nLatin\nMetal\nMisc\nNew Age\nPop\nPublic Radio\nR&B/Urban\nRap\nReggae\nRock\nTalk", "Select", "Cancel");
			return 1;
		}
		if(listitem == 2)   //Custom
		{
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_CUSTOM, DIALOG_STYLE_INPUT, "Custom Stream", "Enter the url of the custom radio stream below:\nExamples:\n{7DAEFF}http://www.radiostation.com/stream.pls{FFFFFF}", "Ok", "Back");
			return 1;
		}
	}
	if(dialogid == DIALOG_SARP_STATIONS_LIST1)   //SARP OFFICIAL STATIONS LIST
	{
		if(!response)
		{
			ShowPlayerDialog(playerid, DIALOG_STATIONS, DIALOG_STYLE_LIST, "Radio stations", "SARP Official Radio\nRadio Stations\nCustom", "Select", "Cancel");
			return 1;
		}
		if(radioInfo[listitem][taken] == 0)   //radio station is not taken or url is blank
		{
			new rstring[1000], name[32], rgenre[32];
			for(new i = 0; i < sizeof(radioInfo); i++)
			{
				format(rgenre, sizeof(rgenre), "%s", radioInfo[i][genreName]);
				format(name, sizeof(name), "%s", radioInfo[i][channelName]);
				format(rstring, sizeof(rstring), "%s %s - %s\n", rstring, rgenre, name);
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST1, DIALOG_STYLE_LIST, "SARP Official Radio", rstring, "Select", "Cancel");
			SendClientMessage(playerid, COLOR_GREY, "That radio station hasn't been taken yet.");
			return 1;
		}
		if(!strcmp(radioInfo[listitem][url], "None", true))
		{
			new rstring[1000], name[32], rgenre[32];
			for(new i = 0; i < sizeof(radioInfo); i++)
			{
				format(rgenre, sizeof(rgenre), "%s", radioInfo[i][genreName]);
				format(name, sizeof(name), "%s", radioInfo[i][channelName]);
				format(rstring, sizeof(rstring), "%s %s - %s\n", rstring, rgenre, name);
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST1, DIALOG_STYLE_LIST, "SARP Official Radio", rstring, "Select", "Cancel");
			SendClientMessage(playerid, COLOR_GREY, "That radio station hasn't been set up yet.");
			return 1;
		}
		if(EDITING_TYPE[playerid] == EDIT_RADIO_BOOMBOX)
		{
			format(BoomboxInfo[editingBoomboxID[playerid]][BoomboxURL], 256, radioInfo[listitem][url]); //set BB to selected radio station
			changeBoomboxRadioStation(editingBoomboxID[playerid]); //update for all players streaming to this BB
			editingBoomboxID[playerid] = -1; //no longer editing
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have changed the radio station on the boombox.");
		}
		else if(EDITING_TYPE[playerid] == EDIT_RADIO_VEHICLE)
		{
			new vehid = GetPlayerVehicleID(playerid);
			if(vehid)
			{
				format(VehicleInfo[vehid][radioURL], 256, radioInfo[listitem][url]);
				VehicleInfo[vehid][radioStation] = 1;
				playVehicleStreamForAll(vehid);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have changed the radio station in the vehicle.");
			}
		}
		EDITING_TYPE[playerid] = EDIT_RADIO_NONE;
		return 1;
	}
	if(dialogid == DIALOG_SARP_STATIONS_LIST2)   //PLAY STATION
	{
		if(!response)
		{
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_GENRES, DIALOG_STYLE_LIST, "Radio Stations", "Alternative\nBlues\nClassical\nCountry\nDecades\nEasy Listening\nElectronic\nFolk\nInspirational\nInternational\nJazz\nLatin\nMetal\nMisc\nNew Age\nPop\nPublic Radio\nR&B/Urban\nRap\nReggae\nRock\nTalk", "Select", "Cancel");
			return 1;
		}
		new selected = listItemGenre[playerid] -9 +listitem;
		if(EDITING_TYPE[playerid] == EDIT_RADIO_BOOMBOX)
		{
			format(BoomboxInfo[editingBoomboxID[playerid]][BoomboxURL], 256, radioStations[selected][radiourl]); //set BB to selected radio station
			changeBoomboxRadioStation(editingBoomboxID[playerid]); //update for all players streaming to this BB
			editingBoomboxID[playerid] = -1; //no longer editing
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have changed the radio station on the boombox.");
		}
		else if(EDITING_TYPE[playerid] == EDIT_RADIO_VEHICLE)
		{
			new vehid = GetPlayerVehicleID(playerid);
			if(vehid)
			{
				format(VehicleInfo[vehid][radioURL], 256, radioStations[selected][radiourl]);
				VehicleInfo[vehid][radioStation] = 1;
				playVehicleStreamForAll(vehid);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have changed the radio station in the vehicle.");
			}
		}
		EDITING_TYPE[playerid] = EDIT_RADIO_NONE;
		listItemGenre[playerid] = -1; //reset the listitem
		return 1;
	}
	if(dialogid == DIALOG_SARP_STATIONS_GENRES)   //SARP GENRES
	{
		if(!response)
		{
			listItemGenre[playerid] = -1;
			ShowPlayerDialog(playerid, DIALOG_STATIONS, DIALOG_STYLE_LIST, "Radio stations", "SARP Official Radio\nRadio Stations\nCustom", "Select", "Cancel");
			return 1;
		}
		new rstring[1000], name[32];
		if(listitem == 0)
		{
			for(new r = 0; r < 10; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - Alternative", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 1)
		{
			for(new r = 10; r < 20; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - Blues", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 2)
		{
			for(new r = 20; r < 30; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - Classical", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 3)
		{
			for(new r = 30; r < 40; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - Country", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 4)
		{
			for(new r = 40; r < 50; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - Decades", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 5)
		{
			for(new r = 50; r < 60; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - Easy Listening", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 6)
		{
			for(new r = 60; r < 70; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - Electronic", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 7)
		{
			for(new r = 70; r < 80; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - Folk", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 8)
		{
			for(new r = 80; r < 90; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - Inspirational", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 9)
		{
			for(new r = 90; r < 100; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - International", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 10)
		{
			for(new r = 100; r < 110; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - Jazz", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 11)
		{
			for(new r = 110; r < 120; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - Latin", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 12)
		{
			for(new r = 120; r < 130; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - Metal", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 13)
		{
			for(new r = 130; r < 140; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - Misc", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 14)
		{
			for(new r = 140; r < 150; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - New Age", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 15)
		{
			for(new r = 150; r < 160; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - Pop", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 16)
		{
			for(new r = 160; r < 170; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - Public Radio", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 17)
		{
			for(new r = 170; r < 180; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - R&B/Urban", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 18)
		{
			for(new r = 180; r < 190; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - Rap", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 19)
		{
			for(new r = 190; r < 200; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - Reggae", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 20)
		{
			for(new r = 200; r < 210; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - Rock", rstring, "Play", "Cancel");
			return 1;
		}
		if(listitem == 21)
		{
			for(new r = 210; r < 220; ++r)
			{
				format(name, sizeof(name), "%s", radioStations[r][radioname]);
				format(rstring, sizeof(rstring), "%s %s (listeners: %d)\n", rstring, name, radioStations[r][listeners]);
				listItemGenre[playerid] = r;
			}
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_LIST2, DIALOG_STYLE_LIST, "Radio stations - Talk", rstring, "Play", "Cancel");
			return 1;
		}
	}
	if(dialogid == DIALOG_SARP_STATIONS_CUSTOM)   //SARP CUSTOM
	{
		if(!response)
		{
			ShowPlayerDialog(playerid, DIALOG_STATIONS, DIALOG_STYLE_LIST, "Radio stations", "SARP Official Radio\nRadio Stations\nCustom", "Select", "Cancel");
			return 1;
		}
		if(!strlen(inputtext))
		{
			SendClientMessage(playerid, COLOR_WHITE, "You must enter a stream url.");
			ShowPlayerDialog(playerid, DIALOG_SARP_STATIONS_CUSTOM, DIALOG_STYLE_INPUT, "Custom Stream", "Enter the url of the custom radio stream below:\nExamples:\n{7DAEFF}http://www.radiostation.com/stream.pls{FFFFFF}", "Ok", "Back");
			return 1;
		}
		else
		{
			new rstr[256];
			strmid(rstr, inputtext, 0, strlen(inputtext), 255);
			if(EDITING_TYPE[playerid] == EDIT_RADIO_BOOMBOX)
			{
				format(BoomboxInfo[editingBoomboxID[playerid]][BoomboxURL], 256, rstr); //set BB to selected radio station
				changeBoomboxRadioStation(editingBoomboxID[playerid]); //update for all players streaming to this BB
				editingBoomboxID[playerid] = -1; //no longer editing
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have changed the radio station on the boombox.");
			}
			else if(EDITING_TYPE[playerid] == EDIT_RADIO_VEHICLE)
			{
				new vehid = GetPlayerVehicleID(playerid);
				if(vehid)
				{
					format(VehicleInfo[vehid][radioURL], 256, rstr);
					VehicleInfo[vehid][radioStation] = 1;
					playVehicleStreamForAll(vehid);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have changed the radio station in the vehicle.");
				}
			}
			EDITING_TYPE[playerid] = EDIT_RADIO_NONE;
		}
	}
	//radio vehicle
	if(dialogid == DIALOG_CAR_MENU_MAIN)
	{
		if(!response) return 1;
		if(listitem == 0)   //turn off/on
		{
			new vehid = GetPlayerVehicleID(playerid);
			if(VehicleInfo[vehid][radioStation])   // if on
			{
				VehicleInfo[vehid][radioStation] = 0;
				stopVehicleStreamForAll(vehid);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have turned the vehicle radio off.");
			}
			else
			{
				if(!VehicleInfo[vehid][radioURL])   //radio url is empty, play a random station
				{
					new defaultStation[128];
					//random radio selector (based on car type)
					new vehicleid = GetPlayerVehicleID(playerid);
					new vehmodel = GetVehicleModel(vehicleid);
					new cartype = getCarType(vehmodel);
					switch(cartype)
					{
						case 1:   //industrial car
						{
							new rand = random(10);
							switch(rand)
							{
								case 0:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=212880"); //xliv rap
								}
								case 1:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=188714"); //art bell
								}
								case 2:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=1654897"); //reggae
								}
								case 3:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=164745"); //80's 90's
								}
								case 4:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=914897"); //idobi
								}
								case 5:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=326520"); //beatles
								}
								case 6:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=106750"); //rock heaven
								}
								case 7:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=1562093"); //calm radio solo piano
								}
								case 8:
								{
									format(defaultStation, 128, "http://ch1relay1.etn.fm:8130/listen.pls?sid=1"); //etn1
								}
								case 9:
								{
									format(defaultStation, 128, "http://ch1relay1.etn.fm:8000/listen.pls?sid=6"); //etn2
								}
							}
							format(VehicleInfo[vehid][radioURL], 256, defaultStation);
						}
						case 2:  //poor car
						{
							new rand = random(10);
							switch(rand)
							{
								case 0:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=212880"); //xliv rap
								}
								case 1:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=188714"); //art bell
								}
								case 2:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=1654897"); //reggae
								}
								case 3:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=164745"); //80's 90's
								}
								case 4:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=914897"); //idobi
								}
								case 5:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=326520"); //beatles
								}
								case 6:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=106750"); //rock heaven
								}
								case 7:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=1562093"); //calm radio solo piano
								}
								case 8:
								{
									format(defaultStation, 128, "http://ch1relay1.etn.fm:8130/listen.pls?sid=1"); //etn1
								}
								case 9:
								{
									format(defaultStation, 128, "http://ch1relay1.etn.fm:8000/listen.pls?sid=6"); //etn2
								}
							}
							format(VehicleInfo[vehid][radioURL], 256, defaultStation);
						}
						case 3:   //normal car
						{
							new rand = random(10);
							switch(rand)
							{
								case 0:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=212880"); //xliv rap
								}
								case 1:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=188714"); //art bell
								}
								case 2:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=1654897"); //reggae
								}
								case 3:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=164745"); //80's 90's
								}
								case 4:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=914897"); //idobi
								}
								case 5:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=326520"); //beatles
								}
								case 6:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=106750"); //rock heaven
								}
								case 7:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=1562093"); //calm radio solo piano
								}
								case 8:
								{
									format(defaultStation, 128, "http://ch1relay1.etn.fm:8130/listen.pls?sid=1"); //etn1
								}
								case 9:
								{
									format(defaultStation, 128, "http://ch1relay1.etn.fm:8000/listen.pls?sid=6"); //etn2
								}
							}
							format(VehicleInfo[vehid][radioURL], 256, defaultStation);
						}
						case 4:   //country car
						{
							new rand = random(3);
							switch(rand)
							{
								case 0:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=297243");
								}
								case 1:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=96698");
								}
								case 2:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=9494745");
								}
							}
							format(VehicleInfo[vehid][radioURL], 256, defaultStation);
						}
						default:   //unknown
						{
							new rand = random(10);
							switch(rand)
							{
								case 0:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=212880"); //xliv rap
								}
								case 1:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=188714"); //art bell
								}
								case 2:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=1654897"); //reggae
								}
								case 3:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=164745"); //80's 90's
								}
								case 4:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=914897"); //idobi
								}
								case 5:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=326520"); //beatles
								}
								case 6:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=106750"); //rock heaven
								}
								case 7:
								{
									format(defaultStation, 128, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=1562093"); //calm radio solo piano
								}
								case 8:
								{
									format(defaultStation, 128, "http://ch1relay1.etn.fm:8130/listen.pls?sid=1"); //etn1
								}
								case 9:
								{
									format(defaultStation, 128, "http://ch1relay1.etn.fm:8000/listen.pls?sid=6"); //etn2
								}
							}
							format(VehicleInfo[vehid][radioURL], 256, defaultStation);
						}
					}
				}
				VehicleInfo[vehid][radioStation] = 1;
				playVehicleStreamForAll(vehid);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have turned the vehicle radio on.");
				EDITING_TYPE[playerid] = EDIT_RADIO_NONE;
			}
			return 1;
		}
		if(listitem == 1)   //change station
		{
			ShowPlayerDialog(playerid, DIALOG_STATIONS, DIALOG_STYLE_LIST, "Radio stations", "SARP Official Radio\nRadio Stations\nCustom", "Select", "Cancel");
			EDITING_TYPE[playerid] = EDIT_RADIO_VEHICLE;
			return 1;
		}
		return 1;
	}


	//DONORSTUFF
	if(dialogid == DIALOG_MC_WEAPONS) // MC Weapons
	{
		if(response)
		{
			switch(listitem)
			{
				case 0:   //colt45
				{
					if(PlayerInfo[playerid][pCash] >= 1500)
					{
						GivePlayerGun(playerid, 23);
						GiveMoney(playerid, -1500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have taken a Silenced Pistol off the weapons rack.");
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					}
				}
				case 1:   //deagle
				{
					if(PlayerInfo[playerid][pCash] >= 4500)
					{
						GivePlayerGun(playerid, 24);
						GiveMoney(playerid, -4500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have taken a Deagle off the weapons rack.");
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					}
				}
				case 2:   //shotgun
				{
					if(PlayerInfo[playerid][pCash] >= 2500)
					{
						GivePlayerGun(playerid, 25);
						GiveMoney(playerid, -2500);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have taken a Shotgun off the weapons rack.");
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					}
				}
				case 3:   //rifle
				{
					if(PlayerInfo[playerid][pCash] >= 4000)
					{
						GivePlayerGun(playerid, 33);
						GiveMoney(playerid, -4000);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have taken a Rifle off the weapons rack.");
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					}
				}
				case 4:   //kevlar
				{
					if(PlayerInfo[playerid][pCash] >= 2600)
					{
						GiveMoney(playerid, -2600);
						SetPlayerArmour(playerid, 100);
						PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have taken some Kevlar off the weapons rack.");
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					}
				}
			}
		}
		return 1;
	}
	//Hitman stuff by Calvin Catt
	if(dialogid == 6493) // Fake Badges
	{
		if(response)
		{
			if(listitem == 0)
			{
				if(HitmanBadgeSAPD >= 1)
				{
					PlayerInfo[playerid][pHAFakeBadge] = 1;
					HitmanBadgeSAPD -= 1;
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have obtained a fake San Andreas Police Department badge for free.");
				}
				else return SendClientMessage(playerid, COLOR_GRAD1, "There are no fake San Andreas Police Department badges in stock.");
			}
			if(listitem == 1)
			{
				if(HitmanBadgeFBI >= 1)
				{
					PlayerInfo[playerid][pHAFakeBadge] = 2;
					HitmanBadgeFBI -= 1;
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have obtained a fake Federeal Bureau of Investigation badge for free.");
				}
				else return SendClientMessage(playerid, COLOR_GRAD1, "There are no fake Federeal Bureau of Investigation badges in stock.");
			}
			if(listitem == 2)
			{
				if(HitmanBadgeRCSD >= 1)
				{
					PlayerInfo[playerid][pHAFakeBadge] = 3;
					HitmanBadgeRCSD -= 1;
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have obtained a fake San Andreas Sheriff Department badge for free.");
				}
				else return SendClientMessage(playerid, COLOR_GRAD1, "There are no fake San Andreas Sheriff Department badges in stock.");
			}
			if(listitem == 3)
			{
				if(HitmanBadgeLSFMD >= 1)
				{
					PlayerInfo[playerid][pHAFakeBadge] = 4;
					HitmanBadgeLSFMD -= 1;
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have obtained a fake San Andreas Fire Medical Department badge for free.");
				}
				else return SendClientMessage(playerid, COLOR_GRAD1, "There are no fake San Andreas Fire Medical Department badges in stock.");
			}
			if(listitem == 4)
			{
				if(HitmanBadgeNG >= 1)
				{
					PlayerInfo[playerid][pHAFakeBadge] = 14;
					HitmanBadgeNG -= 1;
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have obtained a fake SANG badge for free.");
				}
				else return SendClientMessage(playerid, COLOR_GRAD1, "There are no fake SANG badges in stock.");
			}
			if(listitem == 5)
			{
				if(HitmanBadgeGOV >= 1)
				{
					PlayerInfo[playerid][pHAFakeBadge] = 6;
					HitmanBadgeGOV -= 1;
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have obtained a fake Government badge for free.");
				}
				else return SendClientMessage(playerid, COLOR_GRAD1, "There are no fake Government badges in stock.");
			}
			if(listitem == 6)
			{
				if(HitmanBadgeSCOTUS >= 1)
				{
					PlayerInfo[playerid][pHAFakeBadge] = 7;
					HitmanBadgeSCOTUS -= 1;
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have obtained a fake Judicial Department badge for free.");
				}
				else return SendClientMessage(playerid, COLOR_GRAD1, "There are no fake Judicial Department badges in stock.");
			}
			if(listitem == 7)
			{
				if(HitmanBadgeDEA >= 1)
				{
					PlayerInfo[playerid][pHAFakeBadge] = 12;
					HitmanBadgeDEA -= 1;
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have obtained a fake DEA badge for free.");
				}
				else return SendClientMessage(playerid, COLOR_GRAD1, "There are no fake DEA badges in stock.");
			}
			if(listitem == 8)
			{
				if(HitmanBadgeATF >= 1)
				{
					PlayerInfo[playerid][pHAFakeBadge] = 13;
					HitmanBadgeATF -= 1;
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have obtained a fake ATF badge for free.");
				}
				else return SendClientMessage(playerid, COLOR_GRAD1, "There are no fake ATF badges in stock.");
			}
		}
		return 1;
	}
	if(dialogid == 3942)
	{
		if(!response)
			return 1;
		if(strlen(inputtext) < 3 || strlen(inputtext) > (126 - MAX_PLAYER_NAME)) return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} you cannot exceed 126 characters.");
		strmid(PlayerInfo[playerid][pAppearance], inputtext, 0, strlen(inputtext), 255);
		//format(PlayerInfo[playerid][pAppearance], 128, "%s", inputtext);
		format(str, sizeof(str), "-> %s %s", PlayerICName(playerid), PlayerInfo[playerid][pAppearance]);
		SendClientMessage(playerid, COLOR_PURPLE, str);
		return 1;
	}

	//HOUSE FURNITURE
	if(dialogid == DIALOG_FURNITURE_MAIN)
	{
		if(!response) return 1;

		if(listitem == 0) //BUY FURNITURE
		{
			new houseid = PlayerInfo[playerid][pHouseID];
			if(GetTotalFurnitureOutdoor(houseid) > 10 && GetPlayerVirtualWorld(playerid) != HouseInfo[houseid][hSQLID]) return SendClientMessage(playerid, COLOR_GREY, "You cannot have more than 10 furniture objects placed outside your house.");
			ShowPlayerDialog(playerid, DIALOG_FURNITURE_BUY, DIALOG_STYLE_LIST, "Buy furniture", "Decorations\nOffice\nHobby\nLounge\nBedroom\nKitchen\nDining\nBathroom\nLaundry\nElectronics\nLights\nOutdoor\nStructural", "Select", "Cancel");
			return 1;
		}
		if(listitem == 1) //EDIT FURNITURE
		{
			ShowPlayerDialog(playerid, DIALOG_FURNITURE_EDIT, DIALOG_STYLE_LIST, "Edit furniture", "Edit\nShow/Hide\nDelete", "Select", "Back");
			return 1;
		}
	}
	if(dialogid == DIALOG_FURNITURE_BUY)
	{
		if(!response) return ShowPlayerDialog(playerid, DIALOG_FURNITURE_MAIN, DIALOG_STYLE_LIST, "Furniture", "Buy furniture\nEdit furniture", "Select", "Cancel");

		if(listitem == 0) //Decor
		{
			_DestroySelectionMenu(playerid);
			SetPVarInt(playerid, "ospawner_active", 1);
			SetPVarInt(playerid, "ospawner_page", 0);
			SetPVarInt(playerid, "ospawner_category", BUY_FURNITURE_CATEGORY_DECO);
			_CreateSelectionMenu(playerid, GetPVarInt(playerid, "ospawner_category"));
			SelectTextDraw(playerid, 0xACCBF1FF);
			return 1;
		}
		if(listitem == 1) //Office
		{
			_DestroySelectionMenu(playerid);
			SetPVarInt(playerid, "ospawner_active", 1);
			SetPVarInt(playerid, "ospawner_page", 0);
			SetPVarInt(playerid, "ospawner_category", BUY_FURNITURE_CATEGORY_OFFICE);
			_CreateSelectionMenu(playerid, GetPVarInt(playerid, "ospawner_category"));
			SelectTextDraw(playerid, 0xACCBF1FF);
			return 1;
		}
		if(listitem == 2) //Hobby
		{
			_DestroySelectionMenu(playerid);
			SetPVarInt(playerid, "ospawner_active", 1);
			SetPVarInt(playerid, "ospawner_page", 0);
			SetPVarInt(playerid, "ospawner_category", BUY_FURNITURE_CATEGORY_HOBBY);
			_CreateSelectionMenu(playerid, GetPVarInt(playerid, "ospawner_category"));
			SelectTextDraw(playerid, 0xACCBF1FF);
			return 1;
		}
		if(listitem == 3) //Lounge
		{
			_DestroySelectionMenu(playerid);
			SetPVarInt(playerid, "ospawner_active", 1);
			SetPVarInt(playerid, "ospawner_page", 0);
			SetPVarInt(playerid, "ospawner_category", BUY_FURNITURE_CATEGORY_LOUNGE);
			_CreateSelectionMenu(playerid, GetPVarInt(playerid, "ospawner_category"));
			SelectTextDraw(playerid, 0xACCBF1FF);
			return 1;
		}
		if(listitem == 4) //Bedroom
		{
			_DestroySelectionMenu(playerid);
			SetPVarInt(playerid, "ospawner_active", 1);
			SetPVarInt(playerid, "ospawner_page", 0);
			SetPVarInt(playerid, "ospawner_category", BUY_FURNITURE_CATEGORY_BEDROOM);
			_CreateSelectionMenu(playerid, GetPVarInt(playerid, "ospawner_category"));
			SelectTextDraw(playerid, 0xACCBF1FF);
			return 1;
		}
		if(listitem == 5) //Kitchen
		{
			_DestroySelectionMenu(playerid);
			SetPVarInt(playerid, "ospawner_active", 1);
			SetPVarInt(playerid, "ospawner_page", 0);
			SetPVarInt(playerid, "ospawner_category", BUY_FURNITURE_CATEGORY_KITCHEN);
			_CreateSelectionMenu(playerid, GetPVarInt(playerid, "ospawner_category"));
			SelectTextDraw(playerid, 0xACCBF1FF);
			return 1;
		}
		if(listitem == 6) //Dining
		{
			_DestroySelectionMenu(playerid);
			SetPVarInt(playerid, "ospawner_active", 1);
			SetPVarInt(playerid, "ospawner_page", 0);
			SetPVarInt(playerid, "ospawner_category", BUY_FURNITURE_CATEGORY_DINING);
			_CreateSelectionMenu(playerid, GetPVarInt(playerid, "ospawner_category"));
			SelectTextDraw(playerid, 0xACCBF1FF);
			return 1;
		}
		if(listitem == 7) //Bathroom
		{
			_DestroySelectionMenu(playerid);
			SetPVarInt(playerid, "ospawner_active", 1);
			SetPVarInt(playerid, "ospawner_page", 0);
			SetPVarInt(playerid, "ospawner_category", BUY_FURNITURE_CATEGORY_BATHROOM);
			_CreateSelectionMenu(playerid, GetPVarInt(playerid, "ospawner_category"));
			SelectTextDraw(playerid, 0xACCBF1FF);
			return 1;
		}
		if(listitem == 8) //Laundry
		{
			_DestroySelectionMenu(playerid);
			SetPVarInt(playerid, "ospawner_active", 1);
			SetPVarInt(playerid, "ospawner_page", 0);
			SetPVarInt(playerid, "ospawner_category", BUY_FURNITURE_CATEGORY_LAUNDRY);
			_CreateSelectionMenu(playerid, GetPVarInt(playerid, "ospawner_category"));
			SelectTextDraw(playerid, 0xACCBF1FF);
			return 1;
		}
		if(listitem == 9) //Electronics
		{
			_DestroySelectionMenu(playerid);
			SetPVarInt(playerid, "ospawner_active", 1);
			SetPVarInt(playerid, "ospawner_page", 0);
			SetPVarInt(playerid, "ospawner_category", BUY_FURNITURE_CATEGORY_ELECTRONICS);
			_CreateSelectionMenu(playerid, GetPVarInt(playerid, "ospawner_category"));
			SelectTextDraw(playerid, 0xACCBF1FF);
			return 1;
		}
		if(listitem == 10) //Lights
		{
			_DestroySelectionMenu(playerid);
			SetPVarInt(playerid, "ospawner_active", 1);
			SetPVarInt(playerid, "ospawner_page", 0);
			SetPVarInt(playerid, "ospawner_category", BUY_FURNITURE_CATEGORY_LIGHTS);
			_CreateSelectionMenu(playerid, GetPVarInt(playerid, "ospawner_category"));
			SelectTextDraw(playerid, 0xACCBF1FF);
			return 1;
		}
		if(listitem == 11) //Outdoor
		{
			_DestroySelectionMenu(playerid);
			SetPVarInt(playerid, "ospawner_active", 1);
			SetPVarInt(playerid, "ospawner_page", 0);
			SetPVarInt(playerid, "ospawner_category", BUY_FURNITURE_CATEGORY_OUTDOOR);
			_CreateSelectionMenu(playerid, GetPVarInt(playerid, "ospawner_category"));
			SelectTextDraw(playerid, 0xACCBF1FF);
			return 1;
		}
		if(listitem == 12) //Structure
		{
			_DestroySelectionMenu(playerid);
			SetPVarInt(playerid, "ospawner_active", 1);
			SetPVarInt(playerid, "ospawner_page", 0);
			SetPVarInt(playerid, "ospawner_category", BUY_FURNITURE_CATEGORY_STRUCTURE);
			_CreateSelectionMenu(playerid, GetPVarInt(playerid, "ospawner_category"));
			SelectTextDraw(playerid, 0xACCBF1FF);
			return 1;
		}
	}
	if(dialogid == DIALOG_FURNITURE_BUY_CONFIRM)
	{
		if(!response) return ShowPlayerDialog(playerid, DIALOG_FURNITURE_BUY, DIALOG_STYLE_LIST, "Buy furniture", "Decorations\nOffice\nHobby\nLounge\nBedroom\nKitchen\nDining\nBathroom\nLaundry\nElectronics\nLights\nOutdoor\nStructural", "Select", "Cancel");

		HandleObjectItemSelection(playerid, GetPVarInt(playerid, "buying_furniture_id"));
		return 1;
	}
	if(dialogid == DIALOG_FURNITURE_EDIT)
	{
		if(!response) return ShowPlayerDialog(playerid, DIALOG_FURNITURE_MAIN, DIALOG_STYLE_LIST, "Furniture", "Buy furniture\nEdit furniture", "Select", "Cancel");
		if(listitem == 0) //edit
		{
			new houseid = PlayerInfo[playerid][pHouseID];
			if(!houseid) return SendClientMessage( playerid, COLOR_GRAD1, "You do not own a house." ); //in case they sell while in menu

			new fstr[2500];

			//show my furniture
			for(new i = 0; i < MAX_FURNITURE; i++) {

				if(FurnitureInfo[houseid][i][f_ModelID]) { //contains a model id
					if(FurnitureInfo[houseid][i][f_Spawned]) {
						format(fstr, sizeof(fstr), "%s\n%s (ID %d)", fstr, FurnitureInfo[houseid][i][f_ModelName], i);
					} else { //hidden
						format(fstr, sizeof(fstr), "%s\n{AFAFAF}%s (ID %d){FFFFFF}", fstr, FurnitureInfo[houseid][i][f_ModelName], i);
					}
				}
				else {
					format(fstr, sizeof(fstr), "%s\n{AFAFAF}Empty{FFFFFF}", fstr);
				}
			}
			ShowPlayerDialog(playerid, DIALOG_FURNITURE_EDIT_SELECT, DIALOG_STYLE_LIST, "Edit furniture", fstr, "Edit", "Back");
		}
		if(listitem == 1) //hide
		{
			new houseid = PlayerInfo[playerid][pHouseID];
			if(!houseid) return SendClientMessage( playerid, COLOR_GRAD1, "You do not own a house." ); //in case they sell while in menu

			new fstr[2500];

			//show my furniture
			for(new i = 0; i < MAX_FURNITURE; i++) {

				if(FurnitureInfo[houseid][i][f_ModelID]) { //contains a model id
					if(FurnitureInfo[houseid][i][f_Spawned]) {
						format(fstr, sizeof(fstr), "%s\n%s (ID %d)", fstr, FurnitureInfo[houseid][i][f_ModelName], i);
					} else { //hidden
						format(fstr, sizeof(fstr), "%s\n{AFAFAF}%s (ID %d){FFFFFF}", fstr, FurnitureInfo[houseid][i][f_ModelName], i);
					}
				}
				else {
					format(fstr, sizeof(fstr), "%s\n{AFAFAF}Empty{FFFFFF}", fstr);
				}
			}
			ShowPlayerDialog(playerid, DIALOG_FURNITURE_SHOWHIDE_SELECT, DIALOG_STYLE_LIST, "Show/Hide furniture", fstr, "Select", "Back");
		}
		if(listitem == 2) //delete
		{
			new houseid = PlayerInfo[playerid][pHouseID];
			if(!houseid) return SendClientMessage( playerid, COLOR_GRAD1, "You do not own a house." ); //in case they sell while in menu

			new fstr[2500];

			//show my furniture
			for(new i = 0; i < MAX_FURNITURE; i++) {

				if(FurnitureInfo[houseid][i][f_ModelID]) { //contains a model id
					if(FurnitureInfo[houseid][i][f_Spawned]) {
						format(fstr, sizeof(fstr), "%s\n%s (ID %d)", fstr, FurnitureInfo[houseid][i][f_ModelName], i);
					} else { //hidden
						format(fstr, sizeof(fstr), "%s\n{AFAFAF}%s (ID %d){FFFFFF}", fstr, FurnitureInfo[houseid][i][f_ModelName], i);
					}
				}
				else {
					format(fstr, sizeof(fstr), "%s\n{AFAFAF}Empty{FFFFFF}", fstr);
				}
			}
			ShowPlayerDialog(playerid, DIALOG_FURNITURE_DELETE_SELECT, DIALOG_STYLE_LIST, "Delete furniture", fstr, "Edit", "Back");
		}
		return 1;
	}

	//furniture edit menu
	if(dialogid == DIALOG_FURNITURE_EDIT_SELECT)
	{
		if(!response) return ShowPlayerDialog(playerid, DIALOG_FURNITURE_EDIT, DIALOG_STYLE_LIST, "Edit furniture", "Edit\nShow/Hide\nDelete", "Select", "Back");

		new houseid = PlayerInfo[playerid][pHouseID];
		if(houseid == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't own a house.");
		if(!FurnitureInfo[houseid][listitem][f_ModelID]) return SendClientMessage(playerid, COLOR_GREY, "You do not have any furniture in this slot.");
		if(houseid != FurnitureInfo[houseid][listitem][f_HouseID]) return SendClientMessage(playerid, COLOR_GREY, "That furniture does not belong to your house.");
		if(!FurnitureInfo[houseid][listitem][f_Spawned]) return SendClientMessage(playerid, COLOR_GREY, "You cannot edit hidden furniture.");

		new furnitureObject = FurnitureInfo[houseid][listitem][f_ObjectID];
		if(!IsValidDynamicObject(furnitureObject)) return SendClientMessage(playerid, COLOR_GREY, "That furniture object is invalid (possibly de-spawned?).");

		//set vars and enable edit mode
		EditingFurnitureObjectID[playerid] = furnitureObject;
		EditingFurnitureHouseID[playerid] = houseid;
		EditingFurnitureID[playerid] = listitem; //the index
		EditDynamicObject(playerid, furnitureObject);

		return 1;
	}
	if(dialogid == DIALOG_FURNITURE_SHOWHIDE_SELECT)
	{
		if(!response) return ShowPlayerDialog(playerid, DIALOG_FURNITURE_EDIT, DIALOG_STYLE_LIST, "Edit furniture", "Edit\nShow/Hide\nDelete", "Select", "Back");

		new houseid = PlayerInfo[playerid][pHouseID];
		if(houseid == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't own a house.");
		if(!FurnitureInfo[houseid][listitem][f_ModelID]) return SendClientMessage(playerid, COLOR_GREY, "You do not have any furniture in this slot.");
		if(houseid != FurnitureInfo[houseid][listitem][f_HouseID]) return SendClientMessage(playerid, COLOR_GREY, "That furniture does not belong to your house.");

		if(FurnitureInfo[houseid][listitem][f_Spawned]) { //hide
			FurnitureInfo[houseid][listitem][f_Spawned] = 0;
			if(FurnitureInfo[houseid][listitem][f_ObjectID]) { DestroyDynamicObjectEx(FurnitureInfo[houseid][listitem][f_ObjectID]); }
			FurnitureInfo[houseid][listitem][f_ObjectID] = 0;

			format(str, sizeof(str), "* You have hidden the %s.", FurnitureInfo[houseid][listitem][f_ModelName]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

			//sql update
			new query[200];
			mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `furniture` SET `Spawned`=0 WHERE `ID`=%d", FurnitureInfo[houseid][listitem][f_ID]);
			mysql_pquery(sqlGameConnection, query);

		} else { //unhide
			FurnitureInfo[houseid][listitem][f_ObjectID] = CreateDynamicObject(FurnitureInfo[houseid][listitem][f_ModelID], FurnitureInfo[houseid][listitem][f_X], FurnitureInfo[houseid][listitem][f_Y], FurnitureInfo[houseid][listitem][f_Z], FurnitureInfo[houseid][listitem][f_RX], FurnitureInfo[houseid][listitem][f_RY], FurnitureInfo[houseid][listitem][f_RZ]);
			FurnitureInfo[houseid][listitem][f_Spawned] = 1;
			Streamer_Update(playerid);

			format(str, sizeof(str), "* You have unhidden the %s.", FurnitureInfo[houseid][listitem][f_ModelName]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

			//sql update
			new query[200];
			mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `furniture` SET `Spawned`=1 WHERE `ID`=%d", FurnitureInfo[houseid][listitem][f_ID]);
			mysql_pquery(sqlGameConnection, query);
		}
		return 1;
	}
	if(dialogid == DIALOG_FURNITURE_DELETE_SELECT)
	{
		if(!response) return ShowPlayerDialog(playerid, DIALOG_FURNITURE_EDIT, DIALOG_STYLE_LIST, "Edit furniture", "Edit\nShow/Hide\nDelete", "Select", "Back");

		new houseid = PlayerInfo[playerid][pHouseID];
		if(houseid == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't own a house.");
		if(!FurnitureInfo[houseid][listitem][f_ModelID]) return SendClientMessage(playerid, COLOR_GREY, "You do not have any furniture in this slot.");
		if(houseid != FurnitureInfo[houseid][listitem][f_HouseID]) return SendClientMessage(playerid, COLOR_GREY, "That furniture does not belong to your house.");

		format(str, sizeof(str), "* You have deleted the %s.", FurnitureInfo[houseid][listitem][f_ModelName]);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

		DeleteHouseFurniture(houseid, listitem);
		return 1;
	}

	if(dialogid == DIALOG_CHARGES_REMOVE)
	{
		if(response)
		{
			new query[128], query2[128], CrimeReason[128], psqlid = searchingsqlid[playerid], CrimeID, Cache:result;
			mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `newcrimes` WHERE `CrimePID` = %d ORDER BY `CrimeID` DESC LIMIT 20", psqlid);
			result = mysql_query(sqlGameConnection, query);

			CrimeID = cache_get_field_content_int(listitem, "CrimeID");
			cache_get_field_content(listitem, "CrimeReason", CrimeReason);

			format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has deleted the charge '%s' from SQLID %d.", PlayerOOCName(playerid), CrimeReason, psqlid);
			SendAdminMessage(-1, str);
			print(str);

			mysql_format(sqlGameConnection, query2, sizeof(query2), "DELETE FROM `newcrimes` WHERE `CrimeID`=%d", CrimeID);
			mysql_query(sqlGameConnection, query2);
			cache_delete(Cache:result, sqlGameConnection);
		}
	}

	// Edit badge main screen
	if(dialogid == DIALOG_S9_EDIT_BADGE )
	{
		if(response) {
			switch(listitem)
			{
				// Change player name
				case 0: {
					if(PlayerInfo[playerid][pGroup] != GROUP_S9 && PlayerInfo[playerid][pGroup] != GROUP_S99) return 1;

					return ShowPlayerDialog(playerid, DIALOG_S9_EDIT_BADGE_NAME, DIALOG_STYLE_INPUT, "Player Badge Name", "Changes for only you.", "Confirm", "Cancel");
				}
				// Change faction name
				case 1: {
					if(PlayerInfo[playerid][pGroup] != GROUP_S9 && PlayerInfo[playerid][pGroup] != GROUP_S99) return 1;

					return ShowPlayerDialog(playerid, DIALOG_S9_EDIT_BADGE_FACTION, DIALOG_STYLE_INPUT, "Faction Name", "Changes for everyone in the faction.", "Confirm", "Cancel");
				}
				// Change faction weapons
				case 2: {
					if(PlayerInfo[playerid][pGroup] != GROUP_S9 && PlayerInfo[playerid][pGroup] != GROUP_S99) return 1;

					return ShowPlayerDialog(playerid, DIALOG_S9_EDIT_BADGE_WEAPONS, DIALOG_STYLE_INPUT, "Faction Weapons", "Changes for everyone in the faction.", "Confirm", "Cancel");
				}
			}
		}
	}
	// Edit badge name
	if(dialogid == DIALOG_S9_EDIT_BADGE_NAME)
	{
		if(response) {
			if(PlayerInfo[playerid][pGroup] != GROUP_S9 && PlayerInfo[playerid][pGroup] != GROUP_S99) return 1;

			new str2[64];
			if(sscanf(inputtext, "s[24]", PlayerInfo[playerid][pS9BadgeName])) return ShowS9BadgeMenu(playerid);
			format(str2, sizeof(str2), "You have set your badge name to: %s", PlayerInfo[playerid][pS9BadgeName]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str2);
		} else {
			ShowS9BadgeMenu(playerid);
		}
		return 1;
	}
	// Edit badge faction
	if(dialogid == DIALOG_S9_EDIT_BADGE_FACTION)
	{
		if(response) {
			if(PlayerInfo[playerid][pGroup] != GROUP_S9 && PlayerInfo[playerid][pGroup] != GROUP_S99) return 1;

			new str2[64];
			if(PlayerInfo[playerid][pGroup] == GROUP_S9)
			{
				if(sscanf(inputtext, "s[64]", S9BadgeFaction)) return ShowS9BadgeMenu(playerid);
				format(str2, sizeof(str2), "You have set S9's badge faction to: %s", S9BadgeFaction);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str2);
			}
			else
			{
				if(sscanf(inputtext, "s[64]", S99BadgeFaction)) return ShowS9BadgeMenu(playerid);
				format(str2, sizeof(str2), "You have set S9's badge faction to: %s", S99BadgeFaction);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str2);
			}
		} else {
			ShowS9BadgeMenu(playerid);
		}
		return 1;
	}
	// Edit badge weapons
	if(dialogid == DIALOG_S9_EDIT_BADGE_WEAPONS)
	{
		if(response) {
			if(PlayerInfo[playerid][pGroup] != GROUP_S9 && PlayerInfo[playerid][pGroup] != GROUP_S99) return 1;

			new str2[64];
			if(PlayerInfo[playerid][pGroup] == GROUP_S9)
			{
				if(sscanf(inputtext, "s[64]", S9BadgeWeapons)) return ShowS9BadgeMenu(playerid);
				format(str2, sizeof(str2), "You have set S9's badge weapons to: %s", S9BadgeWeapons);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str2);
			}
			else
			{
				if(sscanf(inputtext, "s[64]", S99BadgeWeapons)) return ShowS9BadgeMenu(playerid);
				format(str2, sizeof(str2), "You have set S9's badge weapons to: %s", S99BadgeWeapons);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str2);
			}
		} else {
			ShowS9BadgeMenu(playerid);
		}
		return 1;
	}
	if(dialogid == DIALOG_CLEARMOD)
	{
		new carid = GetPlayerVehicleID(playerid);
          // Spoiler.
        if(listitem == 0)
        {
	        if(GetVehicleComponentInSlot(carid, 0) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that category.");
	        removeSlotComponent(carid, 0);
        } // Hood.
        if(listitem == 1)
        {
            if(GetVehicleComponentInSlot(carid, 1) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that category.");
            removeSlotComponent(carid, 1);
        } // Roof.
        if(listitem == 2)
        {
            if(GetVehicleComponentInSlot(carid, 2) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that category.");
            removeSlotComponent(carid, 2);
        } // Side Skirt.
        if(listitem == 3)
        {
            if(GetVehicleComponentInSlot(carid, 3) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that category.");
            removeSlotComponent(carid, 3);
        } // Nitro.
        if(listitem == 4)
        {
            if(GetVehicleComponentInSlot(carid, 5) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that category.");
            removeSlotComponent(carid, 5);
        } // Exhaust.
        if(listitem == 5)
        {
            if(GetVehicleComponentInSlot(carid, 6) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that category.");
            removeSlotComponent(carid, 6);
        } // Wheel.
        if(listitem == 6)
        {
            if(GetVehicleComponentInSlot(carid, 7) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that category.");
            removeSlotComponent(carid, 7);
        } // Stereo.
        if(listitem == 7)
        {
            if(GetVehicleComponentInSlot(carid, 8) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that category.");
            removeSlotComponent(carid, 8);
        } // Hydraulic.
        if(listitem == 8)
        {
            if(GetVehicleComponentInSlot(carid, 9) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that category.");
            removeSlotComponent(carid, 9);
        } // F. Bumper.
        if(listitem == 9)
        {
            if(GetVehicleComponentInSlot(carid, 10) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that category.");
            removeSlotComponent(carid, 10);
        } // R. Bumper.
        if(listitem == 10)
        {
            if(GetVehicleComponentInSlot(carid, 11) == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a vehicle mod in that category.");
            removeSlotComponent(carid, 11);
        }

	}
	return 0;
}
