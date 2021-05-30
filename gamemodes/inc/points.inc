/*
  _____      _       _           ____              _____                                      _ 
 |  __ \    (_)     | |         |  _ \            |  __ \                                    | |
 | |__) |__  _ _ __ | |_ ___    | |_) |_   _      | |  | | ___  ___ _ __ ___   ___  _ __   __| |
 |  ___/ _ \| | '_ \| __/ __|   |  _ <| | | |     | |  | |/ _ \/ __| '_ ` _ \ / _ \| '_ \ / _` |
 | |  | (_) | | | | | |_\__ \   | |_) | |_| |     | |__| |  __/\__ \ | | | | | (_) | | | | (_| |
 |_|   \___/|_|_| |_|\__|___/   |____/ \__, |     |_____/ \___||___/_| |_| |_|\___/|_| |_|\__,_|
                                        __/ |                                                   
                                       |___/                                                    
*/

// THE SYSTEM IS A MESS, TO BE IMPROVED - DESMOND

#define MAX_POINTS 15
#define DIALOG_EDIT_POINT 14822
#define DIALOG_EDIT_POINT_NAME 14823
#define DIALOG_EDIT_POINT_TIME 14824

/*	format(teststr, sizeof(teststr), "%s\n{FFFFFF}/capture", Points[idx][PointName]);
if(idx != 0 && idx != 1) Points[idx][Point3D] = Create3DTextLabel(teststr, 0x43527DFF, Points[idx][PointPosition][0], Points[idx][PointPosition][1], Points[idx][PointPosition][2], 15, 0, 1);

Keeping this here in case more points will be added and we will want to keep that style ^
*/


// easier to know each id's point
#define PMATSPICKUP1 		0
#define PMATSPICKUP2 		1
#define PAUTOEXPORT 		2
#define PSPRUNKFACTORY		3
#define PDRUGSFACTORY		4

enum pointsinfo
{
	PointName[36],
	PointTime, 						// in seconds, time left for people to be able to /capture
	PointCaptureTime, 				// in seconds, time left for the people who last did /capture to get the point
	bool:PointDeployed, 			// a bool to check if the point exists
	bool:PointEnabled, 				// a bool to check if the point is currently available to capture
	Float:PointPosition[3],
	Text3D:Point3D, 				// the /capture 3D
	PointPickup, 					// the /capture pickup

	bool:PointDeathMessage, 		// false or true - whenever a message should be sent to a player who dies in range of a point or not
	bool:PointDrugs, 				// a bool to check if the point has drugs to claim (only for drugs factory)

	PointFamily, 					// family slot
	PointOwner[MAX_PLAYER_NAME+1],

	PointBorder, 					// the gangzone id
	Float:PointBorders[4] 			// 4 gangzone positions, X, Y, X, Y	

};

new Points[MAX_POINTS][pointsinfo];
new PointsCount = 0;
new capturingpointtime[MAX_PLAYERS], 
Float:capturingposition[MAX_PLAYERS][3], 
capturingpoint[MAX_PLAYERS],
editingpoint[MAX_PLAYERS],
borderseditcount[MAX_PLAYERS], Float:bordersedit[MAX_PLAYERS][4],
bool:viewingpointborders[MAX_PLAYERS][15];

new teststr[128];

forward PointsTimer();
forward IsDefaultPoint(pointid);
forward SetupPoint(pointid, pointname[], hours, Float:x, Float:y, Float:z); // id, name of the point, hours till the point can be captured on start, x of the point, y of the point, z of the point

public SetupPoint(pointid, pointname[], hours, Float:x, Float:y, Float:z)
{
	format(Points[pointid][PointName], 36, pointname);
	Points[pointid][PointDeployed] = true;
	Points[pointid][PointEnabled] = false;

	Points[pointid][PointTime] = hours*3600;

	Points[pointid][PointPosition][0] = x;
	Points[pointid][PointPosition][1] = y;
	Points[pointid][PointPosition][2] = z;
	return 1;
}

Hook:points_OnGameModeInit()
{	
	LoadPoints();

	SetupPoint(PMATSPICKUP1, "Materials Pickup 1", 9, 1423.5864, -1320.5120, 13.5547);
	SetupPoint(PMATSPICKUP2, "Materials Pickup 2", 10, 2390.38114, -2008.3320, 13.5537);
	SetupPoint(PAUTOEXPORT, "Auto Export", 7, 2610.0503, -2239.3501, 13.5392);
	SetupPoint(PSPRUNKFACTORY, "Sprunk Factory", 11, 1324.2122, 286.5997, 20.0452);
	SetupPoint(PDRUGSFACTORY, "Drugs Factory", 12, -159.9184, -287.2171, 3.9053);

	SetTimer("PointsTimer", 1000, true);

	return 1;
}

Hook:points_OnGameModeExit()
{
	SavePoints();
	return 1;
}

LoadPoints()
{
	new arrCoords[13][64];
	new strFromFile[3000];
	new File: file = fopen("pointss2.cfg", io_read);
	if(file)
	{
		new idx, deathmsgconvert, deployedconvert;
		while(idx < sizeof(Points))
		{
			fread(file, strFromFile);
			split(strFromFile, arrCoords, '|');
			format(Points[idx][PointOwner], MAX_PLAYER_NAME, arrCoords[1]);
			Points[idx][PointTime] = strvalEx(arrCoords[2]);

			deathmsgconvert = strvalEx(arrCoords[3]);
			if(deathmsgconvert == 1) Points[idx][PointDeathMessage] = true;
			else if(deathmsgconvert == 0) Points[idx][PointDeathMessage] = false;

			Points[idx][PointFamily] = strvalEx(arrCoords[4]);

			Points[idx][PointCaptureTime] = 0;

			if(Points[idx][PointTime] == 0) Points[idx][PointEnabled] = true;
			else Points[idx][PointEnabled] = false;

			deployedconvert = strvalEx(arrCoords[8]);
			if(deployedconvert == 1) Points[idx][PointDeployed] = true && PointsCount++;
			else if(deployedconvert == 0) Points[idx][PointDeployed] = false;

			Points[idx][PointBorders][0] = floatstr(arrCoords[9]);
			Points[idx][PointBorders][1] = floatstr(arrCoords[10]);
			Points[idx][PointBorders][2] = floatstr(arrCoords[11]);
			Points[idx][PointBorders][3] = floatstr(arrCoords[12]);
			Points[idx][PointBorder] = GangZoneCreate(Points[idx][PointBorders][0], Points[idx][PointBorders][1], Points[idx][PointBorders][2], Points[idx][PointBorders][3]);

			idx++;
		}
		fclose(file);
	} else {
		printf("[error] Failed to find pointss2.cfg");
	}
	return 1;
}

SavePoints()
{
	new idx;
	new File:file;
	file = fopen("pointss2.cfg", io_write);
	fclose(file);
	file = fopen("pointss2.cfg", io_append);
	new coordsstring[2000];
	while(idx < sizeof(Points))
	{
		new deathmsgconvert, deployedconvert;
		if(Points[idx][PointDeathMessage] == true) deathmsgconvert = 1;
		else deathmsgconvert = 0;
		if(Points[idx][PointDeployed] == true) deployedconvert = 1;
		else deployedconvert = 0;
		format(coordsstring, sizeof(coordsstring), "%s|%s|%d|%d|%d|%f|%f|%f|%d|%f|%f|%f|%f\n",
			   Points[idx][PointName],
			   Points[idx][PointOwner],
			   Points[idx][PointTime],
			   deathmsgconvert,
			   Points[idx][PointFamily],
			   Points[idx][PointPosition][0],
			   Points[idx][PointPosition][1],
			   Points[idx][PointPosition][2],
			   deployedconvert,
			   Points[idx][PointBorders][0],
			   Points[idx][PointBorders][1],
			   Points[idx][PointBorders][2],
			   Points[idx][PointBorders][3]);		
		fwrite(file, coordsstring);
		idx++;
	}
	fclose(file);
	return 1;
}

public IsDefaultPoint(pointid)
{
	if(pointid == PMATSPICKUP1 || pointid == PMATSPICKUP2 || pointid == PAUTOEXPORT || pointid == PSPRUNKFACTORY || pointid == PDRUGSFACTORY) return true;
	else return false;
}

Hook:points_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_EDIT_POINT)
	{
		if(response)
		{
			switch(listitem)
			{
				case 0: 
				{
					if(IsDefaultPoint(editingpoint[playerid])) return SendClientMessage(playerid, COLOR_GREY, "You can not edit a default point's name.");
					format(teststr, sizeof(teststr), "Enter a new name for point: %s", Points[editingpoint[playerid]][PointName]);
					ShowPlayerDialog(playerid, DIALOG_EDIT_POINT_NAME, DIALOG_STYLE_INPUT, "Point Name", teststr, "Set", "Cancel");
				}
				case 1: ShowPlayerDialog(playerid, DIALOG_EDIT_POINT_TIME, DIALOG_STYLE_INPUT, "Point Time", "Enter a new time for the point (in hours, time till /capture for that point is available)", "Set", "Cancel");
				case 2:
				{
					if(IsDefaultPoint(editingpoint[playerid])) return SendClientMessage(playerid, COLOR_GREY, "You can not edit a default point's position.");
					new Float: temppos[3], pointid = editingpoint[playerid];
					GetPlayerPos(playerid, temppos[0], temppos[1], temppos[2]);

					Points[pointid][PointPosition][0] = temppos[0];
					Points[pointid][PointPosition][1] = temppos[1];
					Points[pointid][PointPosition][2] = temppos[2];

					DestroyDynamicPickup(Points[pointid][PointPickup]);
					Points[pointid][PointPickup] = CreateDynamicPickup(1254, 1, Points[pointid][PointPosition][0], Points[pointid][PointPosition][1], Points[pointid][PointPosition][2]);

					Delete3DTextLabel(Points[pointid][Point3D]);
					format(teststr, sizeof(teststr), "%s\n{FFFFFF}/capture", Points[pointid][PointName]);
					Points[pointid][Point3D] = Create3DTextLabel(teststr, 0x43527DFF, Points[pointid][PointPosition][0], Points[pointid][PointPosition][1], Points[pointid][PointPosition][2], 15, 0, 1);

					SendClientMessage(playerid, COLOR_GREY, "That point's position has been changed to your current position.");
				}
				case 3:
				{
					new pointid = editingpoint[playerid];
					if(Points[pointid][PointDeathMessage] == true)
					{
						Points[pointid][PointDeathMessage] = false;
						SendClientMessage(playerid, COLOR_GREY, "Players will no longer be informed if they died near a point.");	
					}
					else
					{
						Points[pointid][PointDeathMessage] = true;
						SendClientMessage(playerid, COLOR_GREY, "Players will now be informed if they died near a point.");
					}
				}
				case 4:
				{
					borderseditcount[playerid] = 4;
					SendClientMessage(playerid, COLOR_GREY, "You are now editing the borders of the point. Type /setpborders 4 times (make a square) to complete.");
				}	
			}
		}
	}
	if(dialogid == DIALOG_EDIT_POINT_NAME)
	{
		if(response)
		{
			if(strlen(inputtext) <= 0 || strlen(inputtext) > 35)
			{
				SendClientMessage(playerid, COLOR_GREY, "The point's name must be between 1-35 characters.");
				format(teststr, sizeof(teststr), "Enter a new name for point: %s", Points[editingpoint[playerid]][PointName]);
				ShowPlayerDialog(playerid, DIALOG_EDIT_POINT_NAME, DIALOG_STYLE_INPUT, "Point Name", teststr, "Set", "Cancel");
			}
			else
			{
				new pointid = editingpoint[playerid];
				format(Points[pointid][PointName], 36, "%s", inputtext);
				format(teststr, sizeof(teststr), "You have set that point's name to %s", Points[pointid][PointName]);
				SendClientMessage(playerid, COLOR_GREY, teststr);

				Delete3DTextLabel(Points[pointid][Point3D]);
				format(teststr, sizeof(teststr), "%s\n{FFFFFF}/capture", Points[pointid][PointName]);
				Points[pointid][Point3D] = Create3DTextLabel(teststr, 0x43527DFF, Points[pointid][PointPosition][0], Points[pointid][PointPosition][1], Points[pointid][PointPosition][2], 15, 0, 1);
			}
		}
	}
	if(dialogid == DIALOG_EDIT_POINT_TIME)
	{
		if(response)
		{
			if(!IsNumeric(inputtext) || strval(inputtext) < 0)
			{
				SendClientMessage(playerid, COLOR_GREY, "Please enter a valid number.");
				ShowPlayerDialog(playerid, DIALOG_EDIT_POINT_TIME, DIALOG_STYLE_INPUT, "Point Time", "Enter a new time for the point (in hours, time till /capture for that point is available)", "Set", "Cancel");
			}
			else
			{
				new pointid = editingpoint[playerid];
				Points[pointid][PointTime] = strval(inputtext)*3600;
				
				if(Points[pointid][PointTime] == 0)
				{
					format(teststr, sizeof(teststr), "You have set that point's time to %i hours causing it to be available to capture.", strval(inputtext));
					SendClientMessage(playerid, COLOR_GREY, teststr);
					foreach(new i: Player)
					{
						format(teststr, sizeof(teststr), "The %s is now available to capture.", Points[pointid][PointName]);
						SendClientMessage(i, COLOR_YELLOW, teststr);
						GangZoneShowForAll(Points[pointid][PointBorder], COLOR_RED);
						Points[pointid][PointEnabled] = true;
						if(PlayerInfo[i][pFMember] != 255)
						{
							SetPlayerCheckpointEx(i, Points[pointid][PointPosition][0], Points[pointid][PointPosition][1], Points[pointid][PointPosition][2], 2.0);
							CP[i] = 35;
							SendClientMessage(i, COLOR_GREY, "A checkpoint has been set for you to the available point as you're a family member.");
						}
						
					}
				}
				else
				{	
					format(teststr, sizeof(teststr), "You have set that point's time to %i hours.", strval(inputtext));
					Points[pointid][PointEnabled] = false;
					SendClientMessage(playerid, COLOR_GREY, teststr);
					GangZoneHideForAll(Points[pointid][PointBorder]);
				}
			}
		}
	}
	return 1;
}


Hook:points_OnPlayerDeath(playerid, killerid, reason)
{
	for(new p = 0; p < MAX_POINTS; p++)
	{
		if(Points[p][PointFamily] == PlayerInfo[playerid][pFMember] && Points[p][PointEnabled] == true && IsPlayerInRangeOfPoint(playerid, 30, Points[p][PointPosition][0], Points[p][PointPosition][1], Points[p][PointPosition][2]) && !strcmp(PlayerName(playerid), Points[p][PointOwner]))
		{
			Points[p][PointFamily] = 255;
			Points[p][PointCaptureTime] = 0;
			GiveMoney(killerid, 5000);
			foreach(new i: Player)
			{
				if(PlayerInfo[i][pFMember] != 255)
				{
					format(teststr, sizeof(teststr), "%s has died during the capturing of %s and no longer holds it.", PlayerICName(playerid), Points[p][PointName]);
					SendClientMessage(i, COLOR_YELLOW, teststr);
					format(teststr, sizeof(teststr), "%s has killed the point's conqueror %s and claimed $5000.", PlayerICName(killerid), PlayerICName(playerid));
					SendClientMessage(i, COLOR_YELLOW, teststr);
				}
			}
			break;
		}
		if(Points[p][PointEnabled] == true)
		{
			if(Points[p][PointDeathMessage] == true && IsPlayerInRangeOfPoint(playerid, 30.0, Points[p][PointPosition][0], Points[p][PointPosition][1], Points[p][PointPosition][2]))
			{
				format(teststr, sizeof(teststr), "You have died in range of point: %s", Points[p][PointName]);
				SendClientMessage(playerid, COLOR_RED, teststr);
				break;
			}
		}
	}
	return 1;
}

Hook:points_OnPlayerDisconnect(playerid, reason)
{
	for(new p = 0; p < MAX_POINTS; p++)
	{
		if(Points[p][PointFamily] == PlayerInfo[playerid][pFMember] && Points[p][PointEnabled] == true && PlayerInfo[playerid][pFMember] != 255 && !strcmp(PlayerName(playerid), Points[p][PointOwner]))
		{
			Points[p][PointFamily] = 255;
			Points[p][PointCaptureTime] = 0;		
		}
	}	
	return 1;
}

Hook:ps_OnPlayerEnterCheckpoint(playerid)
{
	if(CP[playerid] == 35)
	{
		DisablePlayerCheckpointEx(playerid);
	}
	if(CP[playerid] == CHECKPOINT_MATERIALS_1)
	{
		if(Points[PMATSPICKUP1][PointFamily] == PlayerInfo[playerid][pFMember] && PlayerInfo[playerid][pFMember] != 255)
		{
			PlayerInfo[playerid][pMats] += 100;
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "The factory has given you a bonus of 100 materials due to your family holding Materials Pickup 1.");
		}
	}
	if(CP[playerid] == CHECKPOINT_MATERIALS_2)
	{
		if(Points[PMATSPICKUP2][PointFamily] == PlayerInfo[playerid][pFMember] && PlayerInfo[playerid][pFMember] != 255)
		{
			PlayerInfo[playerid][pMats] += 100;
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "The factory has given you a bonus of 100 materials due to your family holding Materials Pickup 2.");
		}
	}
	return 1;
}


public PointsTimer()
{
	for(new p = 0; p < MAX_POINTS; p++)
	{
		if(Points[p][PointDeployed] == true)
		{
			if(Points[p][PointTime] > 0)
			{
				Points[p][PointTime] --;
				if(Points[p][PointTime] == 0)
				{
					
					format(teststr, sizeof(teststr), "The %s is now available to capture.", Points[p][PointName]);
					SendClientMessageToAll(COLOR_YELLOW, teststr);
					Points[p][PointEnabled] = true;
					GangZoneShowForAll(Points[p][PointBorder], COLOR_RED);
					SavePoints();
					foreach(new i: Player)
					{
						if(PlayerInfo[i][pFMember] != 255)
						{
							SetPlayerCheckpointEx(i, Points[p][PointPosition][0], Points[p][PointPosition][1], Points[p][PointPosition][2], 2.0);
							CP[i] = 35;
							SendClientMessage(i, COLOR_GREY, "A checkpoint has been set for you to the available point as you're a family member.");
						}
					}	
				}
			}
		}	
	}

	foreach(new i: Player)
	{
		if(capturingpointtime[i] > 0)
		{
			capturingpointtime[i] --;
			if(capturingpointtime[i] == 0)
			{
				new Float:X, Float:Y, Float:Z;
				GetPlayerPos(i, X, Y, Z);
				if(capturingposition[i][0] == X && capturingposition[i][1] == Y && capturingposition[i][2] == Z)
				{
					format(teststr, sizeof(teststr), "%s is attempting to capture the %s, 10 minutes left.", PlayerICName(i), Points[capturingpoint[i]][PointName]);
					SendClientMessageToAll(COLOR_YELLOW, teststr);
					Points[capturingpoint[i]][PointCaptureTime] = 600;
					Points[capturingpoint[i]][PointFamily] = PlayerInfo[i][pFMember];
					GetPlayerName(i, Points[capturingpoint[i]][PointOwner], MAX_PLAYER_NAME);
				}
				else
				{
					SendClientMessage(i, COLOR_RED, "You have moved during the capturing time and failed.");
				}
			}
		}
	}

	for(new p = 0; p < MAX_POINTS; p++)
	{
		if(Points[p][PointEnabled] == true)
		{
			if(Points[p][PointCaptureTime] > 0)
			{
				Points[p][PointCaptureTime] --;
				new capturer = GetPlayerIDFromName(Points[p][PointOwner]);
				if(IsPlayerConnected(capturer)) 
				{
					new ptime = Points[p][PointCaptureTime];
					if(ptime%60 == 0)
					{
						if(ptime >= 60)
						{
							format(teststr, sizeof(teststr), "~r~%d minutes left", ptime/60);
							displayCenterHUDInfo(capturer, teststr, 5);
						}
					}
					else if(ptime < 60)
					{
						format(teststr, sizeof(teststr), "~r~%d seconds left", ptime);
						displayCenterHUDInfo(capturer, teststr, 5);	
					}
				}
				if(Points[p][PointCaptureTime] == 0)
				{
					format(teststr, sizeof(teststr), "%s has successfully captured the %s point, it is theirs for the next 8 hours.", Points[p][PointOwner], Points[p][PointName]);
					SendClientMessageToAll(COLOR_YELLOW, teststr);


					Points[p][PointTime] = 8*3600; // convert hours to seconds. 1 hour = 3600 seconds
					if(p == PDRUGSFACTORY) // drugs factory point id
					{
						new capturedby = GetPlayerIDFromName(Points[p][PointOwner]);
						Points[PDRUGSFACTORY][PointDrugs] = true;
						Points[PDRUGSFACTORY][PointTime] = 12*3600; // extended time due to drugs factory point

						new randompdrug = random(3);
						switch(randompdrug)
						{
							case 0:
							{
								FamilyInfo[Points[PDRUGSFACTORY][PointFamily]][FamilyCannabis] += 50;
								SendClientMessage(capturedby, COLOR_LIGHTBLUE, "50 grams of Cannabis have been added to your family's safe.");
								Points[PDRUGSFACTORY][PointDrugs] = false;
							}
							case 1:
							{
								FamilyInfo[Points[PDRUGSFACTORY][PointFamily]][FamilyCocaine] += 50;
								SendClientMessage(capturedby, COLOR_LIGHTBLUE, "50 grams of Cocaine have been added to your family's safe.");
								Points[PDRUGSFACTORY][PointDrugs] = false;
								}
							case 2:
							{
								FamilyInfo[Points[PDRUGSFACTORY][PointFamily]][FamilyMeth] += 50;
								SendClientMessage(capturedby, COLOR_LIGHTBLUE, "50 grams of Meth have been added to your family's safe.");
								Points[PDRUGSFACTORY][PointDrugs] = false;
							}
						}
					}
					new capturedby = GetPlayerIDFromName(Points[p][PointOwner]);
					SendClientMessage(capturedby, COLOR_LIGHTBLUE, "Your family has received $50,000 for capturing the point.");
					FamilyInfo[PlayerInfo[capturedby][pFMember]][FamilyBank] += 50000;
					Points[p][PointEnabled] = false;
					GangZoneHideForAll(Points[p][PointBorder]);
					SavePoints();
				}
			}
		}
	}
	return 1;
}

CMD:capture(playerid, params[])
{
	if(PlayerInfo[playerid][pFMember] == 255) return SendClientMessage(playerid, COLOR_GREY, "You are not a member of a family.");
	if(PlayerInfo[playerid][pRank] < 4) return SendClientMessage(playerid, COLOR_GREY, "Only R4+ can use this command.");
	if(pDeathState[playerid] == PLAYER_STATE_REVIVE) return SendClientMessage(playerid, COLOR_GREY, "You can not use this command while injured.");

	for(new p = 0; p < MAX_POINTS; p++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 1, Points[p][PointPosition][0], Points[p][PointPosition][1], Points[p][PointPosition][2]))
		{
			if(Points[p][PointEnabled] == false) continue;
			if(Points[p][PointDeployed] == false) continue;
			format(teststr, sizeof(teststr), "You are attempting to capture the %s, please do not move for 10 seconds.", Points[p][PointName]);
			SendClientMessage(playerid, COLOR_RED, teststr);

			capturingpointtime[playerid] = 10;
			capturingpoint[playerid] = p;

			GetPlayerPos(playerid, capturingposition[playerid][0], capturingposition[playerid][1], capturingposition[playerid][2]);

			break;
		}
	}
	return 1;
}

CMD:points(playerid, params[])
{
	new bigstr[3000], headers[] = "Point\tOwner (Family)\tTime\tStatus\n";
	new timemsg[60], capturedmsg[17];
	for(new p = 0; p < MAX_POINTS; p++)
	{
		if(Points[p][PointDeployed] == true)
		{
			new minutes, hours, family = Points[p][PointFamily];
			minutes = Points[p][PointTime]/60;
			hours = Points[p][PointTime]/3600;
			
			if(Points[p][PointTime] >= 3600) format(timemsg, sizeof(timemsg), "%d hours", hours);
			else format(timemsg, sizeof(timemsg), "%d minutes", minutes);

			if(Points[p][PointCaptureTime] != 0) format(capturedmsg, sizeof(capturedmsg), "BEING CAPTURED");
			else format(capturedmsg, sizeof(capturedmsg), "AVAILABLE");

			if(Points[p][PointTime] > 0) format(capturedmsg, sizeof(capturedmsg), "Not Ready");

			if(Points[p][PointFamily] == 255) format(bigstr, sizeof(bigstr), "{FFFFFF}%s%i. %s\tNone\t%s\t%s\n", bigstr, p, Points[p][PointName], timemsg, capturedmsg);
			else format(bigstr, sizeof(bigstr), "{FFFFFF}%s%i. %s\t%s (%s)\t%s\t%s\n", bigstr, p, Points[p][PointName], Points[p][PointOwner], FamilyInfo[family][FamilyName], timemsg, capturedmsg);
		}
	}

	format(bigstr, sizeof(bigstr), "%s%s", headers, bigstr);
	ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_TABLIST_HEADERS, "Points", bigstr, "Close", "");
	return 1;
}

CMD:editpoint(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 2 && PlayerInfo[playerid][pDev] < 2) return AdmErrorMsg;
	new pointid;
	if(sscanf(params, "i", pointid)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /editpoint [pointid]");
	if(pointid < 0 || pointid > 14) return SendClientMessage(playerid, COLOR_GREY, "Enter a point id between 0-14.");
	if(Points[pointid][PointDeployed] == false) return SendClientMessage(playerid, COLOR_GREY, "That point is not created.");

	format(teststr, sizeof(teststr), "Editing Point %i", pointid);
	editingpoint[playerid] = pointid;
	new bigstr[1000];
	format(bigstr, sizeof(bigstr), "Name:\t %s\nPoint Time (hours):\t %d\nPoint Position\nDeath Message\nBorders",
								Points[pointid][PointName], Points[pointid][PointTime]/3600);
	ShowPlayerDialog(playerid, DIALOG_EDIT_POINT, DIALOG_STYLE_LIST, teststr, bigstr, "Edit", "Cancel");

	return 1;
}

CMD:gotopoint(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 2 && PlayerInfo[playerid][pDev] < 2) return AdmErrorMsg;
	new pointid;
	if(sscanf(params, "i", pointid)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /gotopoint [pointid]");
	if(pointid < 0 || pointid > 14) return SendClientMessage(playerid, COLOR_GREY, "Enter a point id between 0-14");
	if(Points[pointid][PointDeployed] == false) return SendClientMessage(playerid, COLOR_GREY, "That point is not created.");

	format(teststr, sizeof(teststr), "You have teleported to point ID %d", pointid);
	SendClientMessage(playerid, COLOR_GREY, teststr);
	SetPlayerPos(playerid, Points[pointid][PointPosition][0], Points[pointid][PointPosition][1], Points[pointid][PointPosition][2]);
	SetPlayerInterior(playerid, 0);
	return 1;
}

CMD:findpoint(playerid, params[])
{
	new pointid;
	if(sscanf(params, "i", pointid)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /findpoint [pointid]");
	if(pointid < 0 || pointid > 14) return SendClientMessage(playerid, COLOR_GREY, "Enter a point ID between 0-14");
	if(Points[pointid][PointDeployed] == false) return SendClientMessage(playerid, COLOR_GREY, "That point is not created.");

	SetPlayerCheckpointEx(playerid, Points[pointid][PointPosition][0], Points[pointid][PointPosition][1], Points[pointid][PointPosition][2], 2.0);
	CP[playerid] = 35;

	format(teststr, sizeof(teststr), "A checkpoint to the %s point has been set on your minimap.", Points[pointid][PointName]);
	SendClientMessage(playerid, COLOR_GREY, teststr);

	return 1;
}

CMD:setpborders(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 2 && PlayerInfo[playerid][pDev] < 2) return AdmErrorMsg;
	new calc = borderseditcount[playerid] - 1;
	if(calc < 0) return SendClientMessage(playerid, COLOR_GREY, "You are not editing any border.");

	borderseditcount[playerid] --;
	new Float:tempposition[3];
	GetPlayerPos(playerid, tempposition[0], tempposition[1], tempposition[2]);
	if(borderseditcount[playerid] == 3)
	{
		bordersedit[playerid][0] = tempposition[0];
		SendClientMessage(playerid, COLOR_GREY, "Position 1/4");
	}
	else if(borderseditcount[playerid] == 2)
	{
		bordersedit[playerid][1] = tempposition[1];
		SendClientMessage(playerid, COLOR_GREY, "Position 2/4");
	}
	else if(borderseditcount[playerid] == 1)
	{
		bordersedit[playerid][2] = tempposition[0];
		SendClientMessage(playerid, COLOR_GREY, "Position 3/4");
	}
	else if(borderseditcount[playerid] == 0)
	{
		bordersedit[playerid][3] = tempposition[1];
		SendClientMessage(playerid, COLOR_GREY, "Position 4/4");
		new pointid = editingpoint[playerid];
		Points[pointid][PointBorders][0] = bordersedit[playerid][0];
		Points[pointid][PointBorders][1] = bordersedit[playerid][1];
		Points[pointid][PointBorders][2] = bordersedit[playerid][2];
		Points[pointid][PointBorders][3] = bordersedit[playerid][3];
		format(teststr, sizeof(teststr), "You have edited the %s point's borders (ID: %d), /pointborders to display it.", Points[pointid][PointName], pointid);
		SendClientMessage(playerid, COLOR_GREY, teststr);
		GangZoneDestroy(Points[pointid][PointBorder]);
		Points[pointid][PointBorder] = GangZoneCreate(bordersedit[playerid][0], bordersedit[playerid][1], bordersedit[playerid][2], bordersedit[playerid][3]);
		if(Points[pointid][PointEnabled])
		{
			GangZoneHideForAll(Points[pointid][PointBorder]);
			GangZoneShowForAll(Points[pointid][PointBorder], COLOR_RED);
		}
	}

	return 1;
}

CMD:pointborders(playerid, params[])
{
	new pointid;
	if(sscanf(params, "i", pointid)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /pointborders [pointid]");
	if(!Points[pointid][PointDeployed]) return SendClientMessage(playerid, COLOR_GREY, "That point is not active.");

	if(viewingpointborders[playerid][pointid])
	{
		viewingpointborders[playerid][pointid] = false;
		GangZoneHideForPlayer(playerid, Points[pointid][PointBorder]);
		format(teststr, sizeof(teststr), "The %s point's borders no longer display for you.", Points[pointid][PointName]);
		SendClientMessage(playerid, COLOR_GREY, teststr);
	}
	else
	{
		viewingpointborders[playerid][pointid] = true;
		format(teststr, sizeof(teststr), "The %s point's borders now display for you.", Points[pointid][PointName]);
		SendClientMessage(playerid, COLOR_GREY, teststr);
		GangZoneShowForPlayer(playerid, Points[pointid][PointBorder], COLOR_RED);
	}
	return 1;
}


// Add/edit/delete/clear point rules in-game			CREDITS TO JACOB


new pointsSpam[MAX_PLAYERS];
CMD:pointrules( playerid, params[] ) {
	if( pointsSpam[playerid] != 0 && gettime() < ( pointsSpam[playerid] + 3 ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "Please wait 3 seconds before using this command again." );
	}
	pointsSpam[playerid] = gettime();
	mysql_pquery( sqlGameConnection, "SELECT * FROM pointsrules;", "fetchPointsInfo", "i", playerid );
	return 1;
}
forward fetchPointsInfo( playerid );
public fetchPointsInfo( playerid ) {
	new verstr[4000], id, admin[MAX_PLAYER_NAME+1], text[128];
	for( new i; i < cache_get_row_count(); i++ ) {
		id = cache_get_field_content_int( i, "ID" );
		cache_get_field_content( i, "admin", admin, sqlGameConnection, 25 );
		cache_get_field_content( i, "text", text, sqlGameConnection, 128 );
		format( verstr, sizeof( verstr ), "%s{FFFFFF}%d.\t%s{c5c1b5} (%s)\n", verstr, id, text, admin );
	}
	if( strlen( verstr ) > 1 ) {
		ShowPlayerDialog( playerid, DIALOG_VERSION, DIALOG_STYLE_MSGBOX, "{FFC300}Current Point Rules {FFFFFF}", verstr, "Okay", "" );
	}
	return 1;
}

CMD:insertpointrule( playerid, params[] ) {
	if( PlayerInfo[playerid][pDev] < 2 && PlayerInfo[playerid][pAdmin] < 2){
		return 1;
	}
	
	new text[128];
	if( sscanf( params, "s[128]", text ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF}/insertpointrule [text]" );		
	}
	
	insertPointRule( PlayerICName( playerid ), text, playerid );
	return 1;
}

insertPointRule( admin[], text[], playerid ) {
	new query[200];
	mysql_format( sqlGameConnection, query, sizeof( query ), "INSERT INTO pointsrules SET admin = '%e', text = '%e';", admin, text );
	mysql_pquery( sqlGameConnection, query );
	SendClientMessage( playerid, COLOR_GREY, "The point rule has been added to the rules list." );
	return 1;
}

CMD:deletepointrule( playerid, params[] ) {
	if( PlayerInfo[playerid][pDev] < 2 && PlayerInfo[playerid][pAdmin] < 2 ){
		return 1;
	}
	
	new id;
	if( sscanf( params, "i", id ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF}/deletepointrule [id]" );
	}
	
	deletePointRule( id, playerid );
	return 1;
}

deletePointRule( id, playerid ) {
	new query[200];
	mysql_format( sqlGameConnection, query, sizeof( query ), "DELETE FROM pointsrules WHERE ID = %d;", id );
	mysql_pquery( sqlGameConnection, query );
	SendClientMessage( playerid, COLOR_GREY, "The point rule has been deleted." );
	return 1;
}

CMD:updatepointrule( playerid, params[] ) {
	if( PlayerInfo[playerid][pDev] < 2 && PlayerInfo[playerid][pAdmin] < 2 ){
		return 1;
	}
	
	new id, text[128];
	if( sscanf( params, "is[128]", id, text ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF}/updatepointrule [id] [text]" );
	}
	
	updatePointRule( id, PlayerICName( playerid ), text, playerid );
	return 1;
}
updatePointRule( id, admin[], text[], playerid ) {
	new query[200];
	mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE pointsrules SET admin = '%e', text = '%e' WHERE ID = %d;", admin, text, id );
	mysql_pquery( sqlGameConnection, query );
	SendClientMessage( playerid, COLOR_GREY, "The point rule has been updated." );
	return 1;
}

CMD:clearpointrules( playerid, params[] ) {
	if( PlayerInfo[playerid][pDev] < 2 && PlayerInfo[playerid][pAdmin] < 2 ){ 
		return 1;
	}
	
	resetPointRule();
	SendClientMessage( playerid, COLOR_GREY, "The points rules have been cleared." );
	return 1;
}
resetPointRule() {
	mysql_pquery( sqlGameConnection, "TRUNCATE TABLE pointsrules;" );
	return 1;
}