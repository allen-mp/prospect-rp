/*
  _    _       _ _         ____            _____                                      _ 
 | |  | |     (_) |       |  _ \          |  __ \                                    | |
 | |  | |_ __  _| |_ ___  | |_) |_   _    | |  | | ___  ___ _ __ ___   ___  _ __   __| |
 | |  | | '_ \| | __/ __| |  _ <| | | |   | |  | |/ _ \/ __| '_ ` _ \ / _ \| '_ \ / _` |
 | |__| | | | | | |_\__ \ | |_) | |_| |   | |__| |  __/\__ \ | | | | | (_) | | | | (_| |
  \____/|_| |_|_|\__|___/ |____/ \__, |   |_____/ \___||___/_| |_| |_|\___/|_| |_|\__,_|
                                  __/ |                                                 
                                 |___/                                                  


*/


#define MAX_UNITS 				40
#define MAX_UNIT_MEMBERS 		4
#define MAX_UNITS_PER_FACTION 	20

#define DIALOG_UNITS_MAIN 		16758
#define DIALOG_UNITS_SHOW 		16759
#define DIALOG_UNITS_CREATE 	16760
#define DIALOG_UNITS_JOIN	 	16761
#define DIALOG_UNITS_DISBAND	16762
#define DIALOG_UNITS_FIND		16763

enum unitinfo
{
	bool:unitActive,
	unitCallSign[20],
	unitLeader,
	unitFaction
};

new Units[MAX_UNITS][unitinfo];
new pdunits, fmdunits; // counter for how many units there are currently in each faction, maximum 20 per faction!
new unitsmsg[200];
new joiningunit[MAX_PLAYERS];
new Text3D:UnitsCallSign[MAX_VEHICLES];
new findingunit[MAX_PLAYERS];

forward UnitSystemAccess(playerid);
forward ShowPlayerUnits(playerid);
forward ShowPlayerUnitInfo(playerid, unitid, join = 0);
forward GetUnitMembers(unitid);
forward GetUnitLeader(unitid);
forward CreateUnit(playerid, name[]);
forward DisbandUnit(unitid);

public UnitSystemAccess(playerid)
{
	if(PlayerInfo[playerid][pMember] == FACTION_SAPD || PlayerInfo[playerid][pMember] == FACTION_FMD) return 1;
	else if(PlayerInfo[playerid][pMember] == 11 && FakeFaction[playerid] == FACTION_SAPD) return 1;
	else if(PlayerInfo[playerid][pMember] == 11 && FakeFaction[playerid] == FACTION_FMD) return 1;
	else return 0;
}

public GetUnitLeader(unitid)
{
	return Units[unitid][unitLeader];
}

public GetUnitMembers(unitid)
{
	new counter = 0;
	foreach( new i: Player )
	{
		if(!UnitSystemAccess(i)) continue;
		if(PlayerInfo[i][pUnit] == unitid) counter++;
	}
	return counter;
}

public ShowPlayerUnits(playerid)
{
	new facid = PlayerInfo[playerid][pMember];
	new unitsstr[1500], headers[] = "Callsign\tLeader\tMembers\n";
	new unitexists;
	for(new i = 0; i < MAX_UNITS; i++)
	{
		if(!Units[i][unitActive]) continue;
		if(Units[i][unitFaction] != facid && FakeFaction[playerid] != Units[i][unitFaction]) continue;

		format(unitsstr, sizeof(unitsstr), "%s%s\t%s\t%d\n", unitsstr, Units[i][unitCallSign], PlayerICName(Units[i][unitLeader]), GetUnitMembers(i));
		unitexists = 1;
	}
	format(unitsstr, sizeof(unitsstr), "%s%s", headers, unitsstr);
	if(unitexists == 1) ShowPlayerDialog(playerid, DIALOG_UNITS_SHOW, DIALOG_STYLE_TABLIST_HEADERS, "Available Units", unitsstr, "Select", "Cancel");
	return 1;
}

public CreateUnit(playerid, name[])
{
	for(new i = 0; i < MAX_UNITS; i++)
	{	
		if(Units[i][unitActive]) continue;
		if(PlayerInfo[playerid][pMember] == 11) Units[i][unitFaction] = FakeFaction[playerid];
		else Units[i][unitFaction] = PlayerInfo[playerid][pMember];
		Units[i][unitActive] = true;
		Units[i][unitLeader] = playerid;
		format(Units[i][unitCallSign], 20, "%s", name);
		PlayerInfo[playerid][pUnit] = i;
		break;
	}
	if(PlayerInfo[playerid][pMember] == FACTION_SAPD || FakeFaction[playerid] == FACTION_SAPD) pdunits++;
	else if(PlayerInfo[playerid][pMember] == FACTION_FMD || FakeFaction[playerid] == FACTION_FMD) fmdunits++;
	return 1;
}

public DisbandUnit(unitid)
{
	foreach( new i: Player )
	{
		if(PlayerInfo[i][pMember] != Units[unitid][unitFaction] && FakeFaction[i] != Units[unitid][unitFaction]) continue;
		if(PlayerInfo[i][pUnit] != unitid) continue;
		PlayerInfo[i][pUnit] = -1;
	}

	format(unitsmsg, sizeof(unitsmsg), "HQ: Unit '%s' has been disbanded.", Units[unitid][unitCallSign]);
	SendRadioMessage(Units[unitid][unitFaction], GetFactionColour(Units[unitid][unitFaction]), unitsmsg);
	if(Units[unitid][unitFaction] == FACTION_SAPD) pdunits--;
	else if(Units[unitid][unitFaction] == FACTION_FMD) fmdunits--;
	Units[unitid][unitActive] = false;
	Units[unitid][unitFaction] = -1;
	Units[unitid][unitLeader] = -1;
	format(Units[unitid][unitCallSign], 20, "");
	return 1;
}

ShowPlayerUnitInfo(playerid, unitid, join = 0) // join is only 1 when the player is watching other units' stats and not his own.
{
	new unitinfostr[400];
	format(unitinfostr, sizeof(unitinfostr), "Unit Callsign: %s\n\nMembers:\n", Units[unitid][unitCallSign]);

	foreach( new i: Player )
	{
		if(!UnitSystemAccess(i)) continue;
		if(PlayerInfo[i][pUnit] != unitid) continue;
		if(GetUnitLeader(unitid) == i) format(unitinfostr, sizeof(unitinfostr), "%s%s (LEADER)\n", unitinfostr, PlayerICName(i));
		else format(unitinfostr, sizeof(unitinfostr), "%s%s\n", unitinfostr, PlayerICName(i));
	}

	if(!join) ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "My Unit", unitinfostr, "Done", "");
	else
	{
		format(unitsmsg, sizeof(unitsmsg), "%s Unit", Units[unitid][unitCallSign]);
		ShowPlayerDialog(playerid, DIALOG_UNITS_JOIN, DIALOG_STYLE_MSGBOX, unitsmsg, unitinfostr, "Join", "Cancel");
	}
	
	return 1;
}

CMD:units(playerid, params[])
{
	if(!UnitSystemAccess(playerid)) return AdmErrorMsg;

	if(PlayerInfo[playerid][pMember] == FACTION_SAPD || FakeFaction[playerid] == FACTION_SAPD) format(unitsmsg, sizeof(unitsmsg), "Units Database - SAPD Units %d/%d", pdunits, MAX_UNITS_PER_FACTION);
	else if(PlayerInfo[playerid][pMember] == FACTION_FMD || FakeFaction[playerid] == FACTION_FMD) format(unitsmsg, sizeof(unitsmsg), "Units Database - SAFMD Units %d/%d", fmdunits, MAX_UNITS_PER_FACTION);

	if(findingunit[playerid] == -1)
	{
		ShowPlayerDialog(playerid, DIALOG_UNITS_MAIN, DIALOG_STYLE_LIST, unitsmsg, "Show All Units\nCreate Unit\nDisband My Unit\nMy Unit\nCallsign\nDisband Unit (High Commands)\nLeave Unit\nFind Unit", "Select", "Cancel");
	}
	else
	{
		ShowPlayerDialog(playerid, DIALOG_UNITS_MAIN, DIALOG_STYLE_LIST, unitsmsg, "Show All Units\nCreate Unit\nDisband My Unit\nMy Unit\nCallsign\nDisband Unit (High Commands)\nLeave Unit\nStop Tracking", "Select", "Cancel");
	}
	return 1;
}

Hook:units_OnPlayerDisconnect(playerid, reason)
{
	new unitid = PlayerInfo[playerid][pUnit];
	if(unitid != -1)
	{
		if(Units[unitid][unitLeader] == playerid) // if leader left, find new one
		{
			new newleader = -1;
			foreach( new i: Player )
			{
				if(PlayerInfo[i][pUnit] != unitid) continue;
				if(i == playerid) continue;
				newleader = i;
			}
			if(newleader == -1) DisbandUnit(unitid); // if no new leader was found, disband unit
			else Units[unitid][unitLeader] = newleader; // if new leader was found, make him the leader
		}
	}
	return 1;
}

Hook:units_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_UNITS_MAIN)
	{
		if(response)
		{
			switch(listitem)
			{
				case 0:	ShowPlayerUnits(playerid); // show all units
				case 1: // show create dialog of a unit (ask him of the name/the callsign of the unit)
				{
					if(PlayerInfo[playerid][pUnit] != -1) return SendClientMessage(playerid, COLOR_GREY, "You are already in a unit.");
					if((PlayerInfo[playerid][pMember] == FACTION_SAPD || FakeFaction[playerid] == FACTION_SAPD) && pdunits == MAX_UNITS_PER_FACTION) return SendClientMessage(playerid, COLOR_GREY, "Your faction has reached the maximum units.");
					else if((PlayerInfo[playerid][pMember] == FACTION_FMD || FakeFaction[playerid] == FACTION_FMD) && fmdunits == MAX_UNITS_PER_FACTION) return SendClientMessage(playerid, COLOR_GREY, "Your faction has reached the maximum units.");
					ShowPlayerDialog(playerid, DIALOG_UNITS_CREATE, DIALOG_STYLE_INPUT, "Unit Creation - Callsign", "Please enter a name (callsign) for your unit", "Create", "Cancel");
				}
				case 2: // disband his own unit
				{
					if(PlayerInfo[playerid][pUnit] == -1) return SendClientMessage(playerid, COLOR_GREY, "You are not in a unit.");
					if(GetUnitLeader(PlayerInfo[playerid][pUnit]) != playerid) return SendClientMessage(playerid, COLOR_GREY, "You are not the leader of your unit.");

					DisbandUnit(PlayerInfo[playerid][pUnit]);
				}
				case 3: // display the player's unit info
				{
					if(PlayerInfo[playerid][pUnit] == -1) return SendClientMessage(playerid, COLOR_GREY, "You are not in a unit.");
					ShowPlayerUnitInfo(playerid, PlayerInfo[playerid][pUnit]);

				}
				case 4: // attach callsign
				{
					if(PlayerInfo[playerid][pUnit] == -1) SendClientMessage(playerid, COLOR_GREY, "You are not in a unit.");
					new vehicleid = GetPlayerVehicleID(playerid);
					new dynvehID = GetDynamicVehicleID(vehicleid);

					if( sVehicleInfo[dynvehID][v_faction] < 1 ) return SendClientMessage( playerid, COLOR_GREY, "This isn't a faction owned vehicle." );
					
					Delete3DTextLabel(UnitsCallSign[vehicleid]);

					UnitsCallSign[vehicleid] = Create3DTextLabel(Units[PlayerInfo[playerid][pUnit]][unitCallSign], 0x33CCFFFF, 0.0, 0.0, 0.0, 10.0, 0, 1 );
					Attach3DTextLabelToVehicle(UnitsCallSign[vehicleid], vehicleid, 0, -2.1, -0.3);
					SendClientMessage(playerid, COLOR_WHITE, "Callsign attached. Use /dcallsign to remove it. (( Any abusive words will result to OOC punishment. ))");
				}
				case 5: // disband a unit (only hcs, can choose any unit)
				{
					if(PlayerInfo[playerid][pRank] < 3 || PlayerInfo[playerid][pMember] == 11) return SendClientMessage(playerid, COLOR_GREY, "Only R3+ can use this."); // both pd and fmd rank restrictions is 3, so here we only check for the rank and not the faction. Added S9 as they can't be R3s and therefore this feature is disabled for them
					new facid = PlayerInfo[playerid][pMember];
					new unitsstr[1500], headers[] = "Callsign\tLeader\tMembers\n";
					new unitexists;
					for(new i = 0; i < MAX_UNITS; i++)
					{
						if(!Units[i][unitActive]) continue;
						if(Units[i][unitFaction] != facid && Units[i][unitFaction] != FakeFaction[playerid]) continue;

						format(unitsstr, sizeof(unitsstr), "%s%s\t%s\t%d\n", unitsstr, Units[i][unitCallSign], PlayerICName(Units[i][unitLeader]), GetUnitMembers(i));
						unitexists = 1;
					}
					format(unitsstr, sizeof(unitsstr), "%s%s", headers, unitsstr);
					if(unitexists == 1) ShowPlayerDialog(playerid, DIALOG_UNITS_DISBAND, DIALOG_STYLE_TABLIST_HEADERS, "Available Units", unitsstr, "Disband", "Cancel"); // make sure theres units
				}
				case 6: // leave unit
				{
					new unitid = PlayerInfo[playerid][pUnit];
					if(unitid == -1) return SendClientMessage(playerid, COLOR_GREY, "You are not in a unit.");
					format(unitsmsg, sizeof(unitsmsg), "You have left unit '%s'.", Units[unitid][unitCallSign]);
					SendClientMessage(playerid, COLOR_GREY, unitsmsg);

					if(Units[unitid][unitLeader] == playerid) // if leader left, find new one
					{
						new newleader = -1;
						foreach( new i: Player )
						{
							if(PlayerInfo[i][pUnit] != unitid) continue;
							if(i == playerid) continue;
							newleader = i;
						}
						if(newleader == -1) DisbandUnit(unitid); // if no new leader was found, disband unit
						else Units[unitid][unitLeader] = newleader; // if new leader was found, make him the leader
					}
					PlayerInfo[playerid][pUnit] = -1;
				}
				case 7: // find unit
				{
					if(findingunit[playerid] == -1)
					{
						new facid = PlayerInfo[playerid][pMember];
						new unitsstr[1500], headers[] = "Callsign\tLeader\tMembers\n";
						new unitexists;
						for(new i = 0; i < MAX_UNITS; i++)
						{
							if(!Units[i][unitActive]) continue;
							if(Units[i][unitFaction] != facid && Units[i][unitFaction] != FakeFaction[playerid]) continue;

							format(unitsstr, sizeof(unitsstr), "%s%s\t%s\t%d\n", unitsstr, Units[i][unitCallSign], PlayerICName(Units[i][unitLeader]), GetUnitMembers(i));
							unitexists = 1;
						}
						format(unitsstr, sizeof(unitsstr), "%s%s", headers, unitsstr);
						if(unitexists == 1) ShowPlayerDialog(playerid, DIALOG_UNITS_FIND, DIALOG_STYLE_TABLIST_HEADERS, "Available Units", unitsstr, "Find", "Cancel");
					}
					else
					{
						foreach( new i: Player )
						{
							if(PlayerInfo[i][pUnit] != findingunit[playerid]) continue;

							if(PlayerInfo[i][pMember] == FACTION_SAPD || FakeFaction[playerid] == FACTION_SAPD) SetPlayerMarkerForPlayer(playerid, i, TCOLOR_BLUE);
							else if(PlayerInfo[i][pMember] == FACTION_FMD || FakeFaction[playerid] == FACTION_FMD) SetPlayerMarkerForPlayer(playerid, i, TCOLOR_PARAMEDIC);
							SetPlayerToTeamColor(i);
						}
						format(unitsmsg, sizeof(unitsmsg), "You have stopped tracking unit '%s'.", Units[findingunit[playerid]][unitCallSign]);
						SendClientMessage(playerid, COLOR_GREY, unitsmsg);
						findingunit[playerid] = -1;
					}
				}
			}
		}
	}

	if(dialogid == DIALOG_UNITS_CREATE)
	{
		if(response)
		{
			if(strlen(inputtext) < 5 || strlen(inputtext) > 20)
			{
				SendClientMessage(playerid, COLOR_GREY, "The length of the name/callsign must be between 5 and 20 characters");
				ShowPlayerDialog(playerid, DIALOG_UNITS_CREATE, DIALOG_STYLE_INPUT, "Unit Creation - Callsign", "Please enter a name (callsign) for your unit", "Create", "Cancel");
				return 1;
			}
			else
			{
				new input[20];
				format(input, 20, "%s", inputtext);
				CreateUnit(playerid, input);
				format(unitsmsg, sizeof(unitsmsg), "You have successfully created the unit '%s'.", input);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, unitsmsg);
			}
		}
	}
	if(dialogid == DIALOG_UNITS_SHOW)
	{
		if(response)
		{
			new counter, unitid;
			for(new i = 0; i < MAX_UNITS; i++)
			{
				if(!Units[i][unitActive]) continue;
				if(Units[i][unitFaction] != PlayerInfo[playerid][pMember] && Units[i][unitFaction] != FakeFaction[playerid]) continue;
				if(counter == listitem)
				{
					unitid = i;
					joiningunit[playerid] = unitid;
					break;
				}

				counter++;
			}
			ShowPlayerUnitInfo(playerid, unitid, 1);
		}
	}
	if(dialogid == DIALOG_UNITS_JOIN)
	{
		if(response)
		{
			new unitid = joiningunit[playerid];
			if(PlayerInfo[playerid][pUnit] != -1) return SendClientMessage(playerid, COLOR_GREY, "You are already in a unit.");
			if(GetUnitMembers(unitid) == 4) return SendClientMessage(playerid, COLOR_GREY, "This unit already reached the maximum members (4).");
			PlayerInfo[playerid][pUnit] = unitid;
			format(unitsmsg, sizeof(unitsmsg), "You have joined the unit '%s'.", Units[unitid][unitCallSign]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, unitsmsg);	
		}
	}
	if(dialogid == DIALOG_UNITS_DISBAND)
	{
		if(response)
		{
			new counter, unitid;
			for(new i = 0; i < MAX_UNITS; i++)
			{
				if(!Units[i][unitActive]) continue;
				if(Units[i][unitFaction] != PlayerInfo[playerid][pMember] && Units[i][unitFaction] != FakeFaction[playerid]) continue;
				if(counter == listitem)
				{
					unitid = i;
					break;
				}

				counter++;
			}
			DisbandUnit(unitid);
		}
	}
	if(dialogid == DIALOG_UNITS_FIND)
	{
		if(response)
		{
			new counter, unitid;
			for(new i = 0; i < MAX_UNITS; i++)
			{
				if(!Units[i][unitActive]) continue;
				if(Units[i][unitFaction] != PlayerInfo[playerid][pMember] && Units[i][unitFaction] != FakeFaction[playerid]) continue;
				if(counter == listitem)
				{
					unitid = i;
					break;
				}

				counter++;
			}

			foreach( new i: Player )
			{
				if(PlayerInfo[i][pMember] != PlayerInfo[playerid][pMember] && PlayerInfo[i][pMember] != FakeFaction[playerid]) continue;
				if(PlayerInfo[i][pUnit] != unitid) continue;
				if(toggedRadio[i]) continue;

				if(PlayerInfo[i][pMember] == FACTION_SAPD || FakeFaction[i] == FACTION_SAPD) SetPlayerMarkerForPlayer(playerid, i, GPSCOLOR_SAPD);
				else if(PlayerInfo[i][pMember] == FACTION_FMD || FakeFaction[i] == FACTION_FMD) SetPlayerMarkerForPlayer(playerid, i, GPSCOLOR_LSFMD);
			}
			findingunit[playerid] = unitid;
			format(unitsmsg, sizeof(unitsmsg), "You are now tracking unit '%s'", Units[findingunit[playerid]][unitCallSign]);
			SendClientMessage(playerid, COLOR_GREY, unitsmsg);
		}
	}
	return 1;
}