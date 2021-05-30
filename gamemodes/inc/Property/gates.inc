
#if defined GATES_INCLUDED
    #endinput
#endif

#define GATES_INCLUDED

//Dependencies
#include "../gamemodes/inc/Property/houses.inc"

enum Gate_Info
{
	GateBy[MAX_PLAYER_NAME+1],
	GateObject,
	GateCreated,
	GateHouseID,
	GateObjectID,
	Float:GateX,
	Float:GateY,
	Float:GateZ,
	Float:GateRotX,
	Float:GateRotY,
	Float:GateRotZ,
	Float:MveGateX,
	Float:MveGateY,
	Float:MveGateZ,
	Float:MveGateRotX,
	Float:MveGateRotY,
	Float:MveGateRotZ,
	GateVW,
	GateINT,
	Float:GateRadius,
	GateStatus, //if opened or closed
	GatePW[24],
	Float:GateSpeed,
	Faction,
	Group
};
new Gates[MAX_GATES][Gate_Info];


//Vars
static str[144], sendername[MAX_PLAYER_NAME+1];

forward LoadGates();
forward SaveGates();

//==================================[GATES]=====================================
public SaveGates()
{
	new idx = 1;
	new File:file;
	file = fopen("gates.cfg", io_write);
	fclose(file);
	file = fopen("gates.cfg", io_append);
	new coordsstring[512];
	if(file) {
		while(idx < MAX_GATES)
		{

			format(coordsstring, sizeof(coordsstring), "%s|%d|%d|%d|%f|%f|%f|%f|%f|%f|%f|%f|%f|%d|%d|%f|%s|%f|%d|%f|%f|%f|%d\r\n",
				   Gates[idx][GateBy],
				   Gates[idx][GateCreated],
				   Gates[idx][GateHouseID],
				   Gates[idx][GateObjectID],
				   Gates[idx][GateX],
				   Gates[idx][GateY],
				   Gates[idx][GateZ],
				   Gates[idx][GateRotX],
				   Gates[idx][GateRotY],
				   Gates[idx][GateRotZ],
				   Gates[idx][MveGateX],
				   Gates[idx][MveGateY],
				   Gates[idx][MveGateZ],
				   Gates[idx][GateVW],
				   Gates[idx][GateINT],
				   Gates[idx][GateRadius],
				   Gates[idx][GatePW],
				   Gates[idx][GateSpeed],
				   Gates[idx][Faction],
				   Gates[idx][MveGateRotX],
				   Gates[idx][MveGateRotY],
				   Gates[idx][MveGateRotZ],
				   Gates[idx][Group]);
			fwrite(file, coordsstring);
			idx++;
		}
		fclose(file);
	} else {
		printf("[error] Failed to find gates.cfg");
	}
	return 1;
}

public LoadGates()
{
	new arrCoords[23][126], strFromFile2[512], File: file = fopen("gates.cfg", io_read);
	if(file)
	{
		new idx = 1;
		while(idx < MAX_GATES)
		{
			fread(file, strFromFile2);
			split(strFromFile2, arrCoords, '|');
			strmid(Gates[idx][GateBy], arrCoords[0], 0, strlen(arrCoords[0]), 255);
			Gates[idx][GateCreated] = strval(arrCoords[1]);
			if(Gates[idx][GateCreated] == 1)
			{
				Gates[idx][GateHouseID] = strval(arrCoords[2]);
				Gates[idx][GateObjectID] = strval(arrCoords[3]);
				Gates[idx][GateX] = floatstr(arrCoords[4]);
				Gates[idx][GateY] = floatstr(arrCoords[5]);
				Gates[idx][GateZ] = floatstr(arrCoords[6]);
				Gates[idx][GateRotX] = floatstr(arrCoords[7]);
				Gates[idx][GateRotY] = floatstr(arrCoords[8]);
				Gates[idx][GateRotZ] = floatstr(arrCoords[9]);
				Gates[idx][MveGateX] = floatstr(arrCoords[10]);
				Gates[idx][MveGateY] = floatstr(arrCoords[11]);
				Gates[idx][MveGateZ] = floatstr(arrCoords[12]);
				Gates[idx][GateVW] = strval(arrCoords[13]);
				Gates[idx][GateINT] = strval(arrCoords[14]);
				Gates[idx][GateRadius] = floatstr(arrCoords[15]);
				strmid(Gates[idx][GatePW], arrCoords[16], 0, strlen(arrCoords[15]), 255);
				Gates[idx][GateSpeed] = floatstr(arrCoords[17]);
				Gates[idx][Faction] = strval(arrCoords[18]);
				Gates[idx][MveGateRotX] = strval(arrCoords[19]);
				Gates[idx][MveGateRotY] = strval(arrCoords[20]);
				Gates[idx][MveGateRotZ] = strval(arrCoords[21]);
				Gates[idx][Group] = strval(arrCoords[22]);
				gatecount++;

				CreateGate(idx);
			}
			idx++;
		}
		fclose(file);
	}
	return 1;
}

CMD:gotogate(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 1337 || PlayerInfo[playerid][pDev] >= 2 || PlayerInfo[playerid][pMapper] == MAPPERRANK_MANAGER || PlayerInfo[playerid][pBizMod] == 1)
	{
		new gateid;
		if(sscanf(params, "i", gateid))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /gotogate [gate id]");
			return 1;
		}
		if(gateid < 0 || gateid > MAX_GATES) return SendClientMessage(playerid, COLOR_RED, "** Invalid amount.");

		new gatetogoto = gateid;
		format(str, sizeof(str), "%0.2f,%0.2f,%0.2f", Gates[gatetogoto][GateX], Gates[gatetogoto][GateY], Gates[gatetogoto][GateZ]);
		if(strcmp(str, "0.00,0.00,0.00", true) == 0) return SendClientMessage(playerid, COLOR_RED, "** Unused gate ID.");

		format(str, sizeof(str), "[debug] Teleporting to Gate ID: %d, ObjectID: %d, VW: %d, INT: %d", gatetogoto, Gates[gatetogoto][GateObjectID], Gates[gatetogoto][GateVW], Gates[gatetogoto][GateINT]);
		SendClientMessage(playerid, COLOR_GREY, str);

		SetPlayerPos(playerid, Gates[gatetogoto][GateX], Gates[gatetogoto][GateY], Gates[gatetogoto][GateZ]);
		SetPlayerInterior(playerid, Gates[gatetogoto][GateINT]);
		SetPlayerVirtualWorld(playerid, Gates[gatetogoto][GateVW]);
		displayCenterHUDInfo(playerid,  "~r~Teleporting.", 8); //display for 8 seconds
		format(str, sizeof(str), "You have teleported to gate id %d.", gatetogoto);
		SendClientMessage(playerid, COLOR_GRAD1, str);
		return 1;
	}
	else
	{
		AdmErrorMsg;
	}
	return 1;
}

CMD:editgate(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 4 || PlayerInfo[playerid][pDev] >= 2 || PlayerInfo[playerid][pMapper] == MAPPERRANK_MANAGER || PlayerInfo[playerid][pBizMod] != 0 || PlayerInfo[playerid][pShopMod] != 0)
	{
		new action[24], optional;
		if(!sscanf(params, "s[24]F(0.0)", action, optional))
		{
			if(!strcmp(action, "create", true))
			{
				new objid, tempgid, bool:found = false;
				if(!sscanf(params, "s[24]i", action, objid))
				{
					for(new i =1; i <=MAX_GATES; i++)
					{
						if(Gates[i][GateCreated] == 0)
						{
							tempgid = i;
							found = true;
							break;
						}
					}
					if(found)
					{
						new Float:x, Float:y, Float:z, vw, inter;
						GetPlayerPos(playerid, x, y, z);
						vw = GetPlayerVirtualWorld(playerid);
						inter = GetPlayerInterior(playerid);
						EditingGate[playerid] = tempgid;
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						Gates[tempgid][GateBy] = sendername;
						Gates[tempgid][GateObjectID] = objid;
						Gates[tempgid][GateX] = x +1;
						Gates[tempgid][GateY] = y +1;
						Gates[tempgid][GateZ] = z;
						Gates[tempgid][GateRotX] = 0;
						Gates[tempgid][GateRotY] = 0;
						Gates[tempgid][GateRotZ] = 0;
						Gates[tempgid][GateVW] = vw;
						Gates[tempgid][GateINT] = inter;
						Gates[tempgid][GateStatus] = 0;
						Gates[tempgid][Faction] = 0;
						Gates[tempgid][Group] = 0;
						Gates[tempgid][GateSpeed] = 3.0;
						Gates[tempgid][GateRadius] = 5.0;
						CreateGate(tempgid);
						Streamer_Update(playerid);
						format(str, sizeof(str), "You have succesfully spawned gate(%d) at your current position.", tempgid);
						SendClientMessage(playerid, COLOR_GRAD1, str);
						format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has spawned gate(%d) at their position.", PlayerICName(playerid), tempgid);
						ABroadCast(COLOR_WHITE, str, 1);
					}
					else
					{
						SendClientMessage(playerid, COLOR_DARKRED, "ERROR: Max gates reached.");
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /editgate create [OBJECTID]");
					SendClientMessage(playerid, COLOR_GRAD1, "Door object IDs: 2955 (regular), 3089 (with windows), 2949 ('keep locked'), 1498 (white door).");
					SendClientMessage(playerid, COLOR_GRAD1, "Door object IDs: 1536 (modern door; black, yellow), 1535 (pink apartment).");
					SendClientMessage(playerid, COLOR_GRAD2, "Garage object IDs: 10558 (old), 2957 (regular), 3037 (warehouse), 7709 (modern).");
					SendClientMessage(playerid, COLOR_GRAD3, "Gate object IDs: 3049 (red, barb wired), 3036 ('keep locked'), 2930 (door gate), 975 (long, rails).");
					SendClientMessage(playerid, COLOR_GRAD3, "Gate Object IDs: 989 (airport gate), 966 (barrier base), 968 (barrier).");
				}
			}
			else if(!strcmp(action, "id", true))
			{
				new gateid;
				if(!sscanf(params, "s[24]i", action, gateid))
				{
					if(gateid == 0 || gateid > MAX_GATES) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid gate ID.");
					if(Gates[gateid][GateCreated] == 1)
					{
						EditingGate[playerid] = gateid;
						format(str, sizeof(str), "You are now editing gate(%d).", EditingGate[playerid]);
						SendClientMessage(playerid, COLOR_GRAD1, str);
					}
					else
					{
						SendClientMessage(playerid, COLOR_GRAD1, "Invalid gate ID.");
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /editgate id [GATEID]");
				}
			}
			else if(!strcmp(action, "faction", true))
			{
				if(EditingGate[playerid] != 0)
				{
					new
						faction;
					if(sscanf(params, "s[24]i", action, faction))
						return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /editgate [faction id]");

					if(!(faction > -1))
						return SendClientMessage(playerid, COLOR_GREY, "Invalid faction ID.");

					Gates[EditingGate[playerid]][Faction] = faction;

					format(str, sizeof(str), "You have successfully changed gate(%d)'s faction to %d.", EditingGate[playerid], faction);
					SendClientMessage(playerid, COLOR_GREY, str);
				}
			}
			else if(!strcmp(action, "group", true))
			{
				if(EditingGate[playerid] != 0)
				{
					new
						group;
					if(sscanf(params, "s[24]i", action, group))
						return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /editgate [group id]");

					if(!(group > -1))
						return SendClientMessage(playerid, COLOR_GREY, "Invalid group ID.");

					Gates[EditingGate[playerid]][Group] = group;

					format(str, sizeof(str), "You have successfully changed gate(%d)'s group to %d.", EditingGate[playerid], group);
					SendClientMessage(playerid, COLOR_GREY, str);
				}
			}
			else if(!strcmp(action, "object", true))
			{
				if(EditingGate[playerid] != 0)
				{
					new objid;
					if(!sscanf(params, "s[24]i", action, objid))
					{
						if(Gates[EditingGate[playerid]][GateObject]) { DestroyDynamicObjectEx(Gates[EditingGate[playerid]][GateObject]); }
						Gates[EditingGate[playerid]][GateObject] = 0;
						Gates[EditingGate[playerid]][GateObjectID] = objid;
						CreateGate(EditingGate[playerid]);
						format(str, sizeof(str), "You have succesfully changed gate(%d)'s object to %d.", EditingGate[playerid], objid);
						SendClientMessage(playerid, COLOR_GRAD1, str);
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /editgate object [OBJECTID]");
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_WHITE, "You are not editing any gate.");
				}
			}
			else if(!strcmp(action, "position", true))
			{
				if(EditingGate[playerid] != 0)
				{
					GateAction[playerid] = EDITING_GATE_POSITION;
					EditDynamicObject(playerid, Gates[EditingGate[playerid]][GateObject]);
					format(str, sizeof(str), "You are now editing gate(%d)'s position.", EditingGate[playerid]);
					SendClientMessage(playerid, COLOR_GRAD1, str);
				}
				else
				{
					SendClientMessage(playerid, COLOR_GRAD1, "You are not editing any gate.");
				}
			}
			else if(!strcmp(action, "oposition", true))
			{
				if(EditingGate[playerid] != 0)
				{
					GateAction[playerid] = EDITING_GATE_OPOSITION;
					EditDynamicObject(playerid, Gates[EditingGate[playerid]][GateObject]);
					format(str, sizeof(str), "You are now editing gate(%d)'s opened position.", EditingGate[playerid]);
					SendClientMessage(playerid, COLOR_GRAD1, str);
				}
				else
				{
					SendClientMessage(playerid, COLOR_GRAD1, "You are not editing any gate.");
				}
			}
			else if(!strcmp(action, "speed", true))
			{
				if(EditingGate[playerid] != 0)
				{
					new Float:newspeed;
					if(!sscanf(params, "s[24]f", action, newspeed))
					{
						if(newspeed < 0.001 || newspeed > 10) return SendClientMessage(playerid, COLOR_GRAD1, "Speed must be between 0.001 or 10 seconds per unit.");
						Gates[EditingGate[playerid]][GateSpeed] = newspeed;
						format(str, sizeof(str), "Gate(%d)'s Speed was succesfully adjust to %f seconds/unit.", EditingGate[playerid], newspeed);
						SendClientMessage(playerid, COLOR_GRAD1, str);
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /editgate speed [NEWSPEED(Seconds/unit)]");
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GRAD1, "You are not editing any gate.");
				}
			}
			else if(!strcmp(action, "radius", true))
			{
				if(EditingGate[playerid] != 0)
				{
					new Float:newradii;
					if(!sscanf(params, "s[24]f", action, newradii))
					{
						if(newradii < 1 || newradii > 15) return SendClientMessage(playerid, COLOR_GRAD1, "Radius must be between 1 or 15 units.");
						Gates[EditingGate[playerid]][GateRadius] = newradii;
						format(str, sizeof(str), "Gate(%d)'s Radius was succesfully adjust to %f units.", EditingGate[playerid], newradii);
						SendClientMessage(playerid, COLOR_GRAD1, str);
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /editgate radius [NEWRADIUS(in units)]");
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GRAD1, "You are not editing any gate.");
				}
			}
			else if(!strcmp(action, "houseid", true))
			{
				if(EditingGate[playerid] != 0)
				{
					new hid;
					if(!sscanf(params, "s[24]i", action, hid))
					{
						if(HouseInfo[hid][hCreated] == 1)
						{
							Gates[EditingGate[playerid]][GateHouseID] = HouseInfo[hid][hSQLID];
							format(str, sizeof(str), "Gate(%d) was succesfully connected to house %i.", EditingGate[playerid], hid);
							SendClientMessage(playerid, COLOR_GRAD1, str);
						}
						else
						{
							SendClientMessage(playerid, COLOR_GRAD1, "Invalid house ID.");
						}
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /editgate houseid [HOUSEID(to connect the gate to)]");
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GRAD1, "You are not editing any gate.");
				}
			}
			else if(!strcmp(action, "delete", true))
			{
				if(EditingGate[playerid] != 0)
				{
					if(Gates[EditingGate[playerid]][GateObject]) { DestroyDynamicObjectEx(Gates[EditingGate[playerid]][GateObject]); }
					Gates[EditingGate[playerid]][GateObject] = 0;
					strmid(Gates[EditingGate[playerid]][GateBy], "", 0, strlen("None"), 255);
					Gates[EditingGate[playerid]][GateCreated] = 0;
					Gates[EditingGate[playerid]][GateObjectID] = 0;
					Gates[EditingGate[playerid]][GateX] = 0;
					Gates[EditingGate[playerid]][GateY] = 0;
					Gates[EditingGate[playerid]][GateZ] = 0;
					Gates[EditingGate[playerid]][GateRotX] = 0;
					Gates[EditingGate[playerid]][GateRotY] = 0;
					Gates[EditingGate[playerid]][GateRotZ] = 0;
					Gates[EditingGate[playerid]][GateVW] = 0;
					Gates[EditingGate[playerid]][GateINT] = 0;
					Gates[EditingGate[playerid]][MveGateX] = 0;
					Gates[EditingGate[playerid]][MveGateX] = 0;
					Gates[EditingGate[playerid]][MveGateX] = 0;
					Gates[EditingGate[playerid]][GateSpeed] = 0;
					Gates[EditingGate[playerid]][GateRadius] = 0;
					Gates[EditingGate[playerid]][GateHouseID] = 0;
					Gates[EditingGate[playerid]][GateStatus] = 0;
					format(str, sizeof(str), "You have deleted gate(%d).", EditingGate[playerid]);
					SendClientMessage(playerid, COLOR_GRAD1, str);
					format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has deleted gate(%d).", PlayerICName(playerid), EditingGate[playerid]);
					ABroadCast(COLOR_WHITE, str, 1);
					EditingGate[playerid] = 0;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GRAD1, "You are not editing any gate.");
				}
			}
			else if(!strcmp(action, "reload", true))
			{
				if(EditingGate[playerid] != 0)
				{
					new gid = EditingGate[playerid];
					if(Gates[gid][GateObject]) { DestroyDynamicObjectEx(Gates[gid][GateObject]); }
					Gates[gid][GateObject] = CreateDynamicObject(Gates[gid][GateObjectID], Gates[gid][GateX], Gates[gid][GateY], Gates[gid][GateZ], Gates[gid][GateRotX], Gates[gid][GateRotY], Gates[gid][GateRotZ], Gates[gid][GateVW], Gates[gid][GateINT]);
					format(str, sizeof(str), "You have reloaded gate(%d).", EditingGate[playerid]);
					SendClientMessage(playerid, COLOR_GRAD1, str);
					format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has reloaded gate(%d).", PlayerICName(playerid), EditingGate[playerid]);
					ABroadCast(COLOR_WHITE, str, 1);
					EditingGate[playerid] = 0;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GRAD1, "You are not editing any gate.");
				}
			}
			else if(!strcmp(action, "stop", true))
			{
				if(EditingGate[playerid] != 0)
				{
					SendClientMessage(playerid, COLOR_GRAD1, "Editing progress was ended.");
					EditingGate[playerid] = 0;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GRAD1, "You are not editing any gate.");
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_WHITE, "Invalid action.");
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /editgate [action]");
			SendClientMessage(playerid, COLOR_GREY, "Available actions: create, id, object, position, oposition, speed, houseid, radius, delete, stop, reload");
			SendClientMessage(playerid, COLOR_GREY, "Available actions: faction, group");
		}
	}
	else
	{
		AdmErrorMsg;
	}
	return 1;
}



CMD:amovegate(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 4 || PlayerInfo[playerid][pDev] >= 2 || PlayerInfo[playerid][pMapper] == MAPPERRANK_MANAGER || PlayerInfo[playerid][pBizMod] >= 1)
	{
		for(new gt =1; gt <MAX_GATES; gt++)
		{
			if(Gates[gt][GateCreated] == 1)
			{
				if(IsPlayerInRangeOfPoint(playerid, Gates[gt][GateRadius], Gates[gt][GateX], Gates[gt][GateY], Gates[gt][GateZ]))
				{
					if(Gates[gt][GateStatus] == 0)
					{
						MoveDynamicObject(Gates[gt][GateObject], Gates[gt][MveGateX], Gates[gt][MveGateY], Gates[gt][MveGateZ], Gates[gt][GateSpeed], Gates[gt][MveGateRotX], Gates[gt][MveGateRotY], Gates[gt][MveGateRotZ]);
						Gates[gt][GateStatus] = 1;
					}
					else
					{
						MoveDynamicObject(Gates[gt][GateObject], Gates[gt][GateX], Gates[gt][GateY], Gates[gt][GateZ], Gates[gt][GateSpeed], Gates[gt][GateRotX], Gates[gt][GateRotY], Gates[gt][GateRotZ]);
						Gates[gt][GateStatus] = 0;
					}
				}
			}
		}
	}
	else
	{
		AdmErrorMsg;
	}
	return 1;
}

//by Fred
CMD:closestgate( playerid, params[] ) {
	if(PlayerInfo[playerid][pAdmin] >= 3 || PlayerInfo[playerid][pDev] >= 2 || PlayerInfo[playerid][pBizMod] == 1 ||  PlayerInfo[playerid][pMapper] == MAPPERRANK_MANAGER)
	{
		new radius;
		
		if( sscanf( params, "I(5)", radius ) ) {
			return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /closestgate [radius(optional)]" );
		}
		
		//not inbetween
		if( !( 0 < radius <= 500 ) ) {
			return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /closestgate radius (must be between 0 and 500)" );
		}
		
		new closest = -1;
		new Float: distance = 0.0;
		
		new Float: ppos[3];
		GetPlayerPos( playerid, ppos[0], ppos[1], ppos[2] );
		
		for( new g; g < MAX_GATES; g++ )
		{
			if( GetPlayerVirtualWorld( playerid ) != Gates[g][GateVW] ) continue;
			if( GetPlayerInterior( playerid ) != Gates[g][GateINT] ) continue;
			
			if( IsPlayerInRangeOfPoint( playerid, radius, Gates[g][GateX], Gates[g][GateY], Gates[g][GateZ] ) ) {
				new Float: cmpDist = floatsqroot( floatpower( ( Gates[g][GateX] - ppos[0] ), 2 ) + floatpower( ( Gates[g][GateY] - ppos[1] ), 2 ) + floatpower( ( Gates[g][GateZ] - ppos[2] ), 2 ) );
				if( cmpDist < distance || closest == -1 ) {
					distance = cmpDist;
					closest = g;
					
				}
			}
		}
		
		if( closest == -1 ) {
			return SendClientMessage( playerid, COLOR_GREY, "You are not near any gates within the specified radius." );
		}
		
		SendClientMessage(playerid, COLOR_YELLOW, "____________________________________________");
		format(str, sizeof(str), "Gate ID : %i   Created by: %s   HouseID: %i   Speed: %f   ObjectID: %d", closest, Gates[closest][GateBy], Gates[closest][GateHouseID], Gates[closest][GateSpeed], Gates[closest][GateObjectID]);
		SendClientMessage(playerid, COLOR_WHITE, str);
		format(str, sizeof(str), "Radius: %f, Group: %d, Faction: %d", Gates[closest][GateRadius], Gates[closest][Group], Gates[closest][Faction]);
		SendClientMessage(playerid, COLOR_WHITE, str);
		format(str, sizeof(str), "X: %f, Y: %f, Z: %f", Gates[closest][GateX], Gates[closest][GateY], Gates[closest][GateZ]);
		SendClientMessage(playerid, COLOR_WHITE, str);
		format(str, sizeof(str), "VW: %d, INT: %d", Gates[closest][GateVW], Gates[closest][GateINT]);
		SendClientMessage(playerid, COLOR_WHITE, str);
		
	}
	return 1;
}
CMD:gateinfo(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 3 || PlayerInfo[playerid][pDev] >= 2 || PlayerInfo[playerid][pBizMod] == 1 ||  PlayerInfo[playerid][pMapper] == MAPPERRANK_MANAGER)
	{
		for(new gt = 0; gt < MAX_GATES; gt++)
		{
			if(Gates[gt][GateCreated] == 1)
			{
				if(IsPlayerInRangeOfPoint(playerid, 8, Gates[gt][GateX], Gates[gt][GateY], Gates[gt][GateZ]))
				{
					if(	
						GetPlayerVirtualWorld(playerid) != Gates[gt][GateVW] ||
						GetPlayerInterior(playerid) != Gates[gt][GateINT]
					) continue;

					SendClientMessage(playerid, COLOR_YELLOW, "____________________________________________");
					format(str, sizeof(str), "Gate ID : %i   Created by: %s   HouseID: %i   Speed: %f   ObjectID: %d", gt, Gates[gt][GateBy], Gates[gt][GateHouseID], Gates[gt][GateSpeed], Gates[gt][GateObjectID]);
					SendClientMessage(playerid, COLOR_WHITE, str);
					format(str, sizeof(str), "Radius: %f, Group: %d, Faction: %d", Gates[gt][GateRadius], Gates[gt][Group], Gates[gt][Faction]);
					SendClientMessage(playerid, COLOR_WHITE, str);
					format(str, sizeof(str), "X: %f, Y: %f, Z: %f", Gates[gt][GateX], Gates[gt][GateY], Gates[gt][GateZ]);
					SendClientMessage(playerid, COLOR_WHITE, str);
					format(str, sizeof(str), "VW: %d, INT: %d", Gates[gt][GateVW], Gates[gt][GateINT]);
					SendClientMessage(playerid, COLOR_WHITE, str);
				}
			}
		}
	}
	else
	{
		AdmErrorMsg;
	}
	return 1;
}

CMD:biggateinfo(playerid, params[])
{
	if(PlayerInfo[playerid][pDev] >= 2 || PlayerInfo[playerid][pAdmin] >= 4 || PlayerInfo[playerid][pMapper] == MAPPERRANK_MANAGER)
	{
		for(new gt = 0; gt < MAX_GATES; gt++)
		{
			if(Gates[gt][GateCreated] == 1)
			{
				if(IsPlayerInRangeOfPoint(playerid, 24, Gates[gt][GateX], Gates[gt][GateY], Gates[gt][GateZ]))
				{
					SendClientMessage(playerid, COLOR_YELLOW, "____________________________________________");
					format(str, sizeof(str), "Gate ID : %i   Created by: %s   HouseID: %i   Speed: %f   ObjectID: %d", gt, Gates[gt][GateBy], Gates[gt][GateHouseID], Gates[gt][GateSpeed], Gates[gt][GateObjectID]);
					SendClientMessage(playerid, COLOR_WHITE, str);
					format(str, sizeof(str), "Radius: %f", Gates[gt][GateRadius]);
					SendClientMessage(playerid, COLOR_WHITE, str);
					format(str, sizeof(str), "X: %f, Y: %f, Z: %f", Gates[gt][GateX], Gates[gt][GateY], Gates[gt][GateZ]);
					SendClientMessage(playerid, COLOR_WHITE, str);
					format(str, sizeof(str), "VW: %d, INT: %d", Gates[gt][GateVW], Gates[gt][GateINT]);
					SendClientMessage(playerid, COLOR_WHITE, str);
				}
			}
		}
	}
	else
	{
		AdmErrorMsg;
	}
	return 1;
}

stock CreateGate(gid)
{
	Gates[gid][GateObject] = CreateDynamicObject(Gates[gid][GateObjectID], Gates[gid][GateX], Gates[gid][GateY], Gates[gid][GateZ], Gates[gid][GateRotX], Gates[gid][GateRotY], Gates[gid][GateRotZ], Gates[gid][GateVW], Gates[gid][GateINT]);
	Gates[gid][GateCreated] = 1;
}


CMD:movegate(playerid, params[])
{
	new pw[64];
	if(!sscanf(params, "s[64]", pw))
	{
		if(strcmp(pw, "\\", true) == 0) return 1;
		for(new gt =1; gt <MAX_GATES; gt++)
		{
			if(Gates[gt][GateCreated] == 1)
			{
				if(IsPlayerInRangeOfPoint(playerid, Gates[gt][GateRadius], Gates[gt][GateX], Gates[gt][GateY], Gates[gt][GateZ]))
				{
					if(!strcmp(pw, Gates[gt][GatePW], false))
					{
						// Check if it's a faction/group gate before moving it
						if(Gates[gt][Faction] > 0 || Gates[gt][Group] > 0)
							continue;

						if(Gates[gt][GateStatus] == 0)
						{
							MoveDynamicObject(Gates[gt][GateObject], Gates[gt][MveGateX], Gates[gt][MveGateY], Gates[gt][MveGateZ], Gates[gt][GateSpeed], Gates[gt][MveGateRotX], Gates[gt][MveGateRotY], Gates[gt][MveGateRotZ]);
							Gates[gt][GateStatus] = 1;
						}
						else
						{
							MoveDynamicObject(Gates[gt][GateObject], Gates[gt][GateX], Gates[gt][GateY], Gates[gt][GateZ], Gates[gt][GateSpeed], Gates[gt][GateRotX], Gates[gt][GateRotY], Gates[gt][GateRotZ]);
							Gates[gt][GateStatus] = 0;
						}
					}
				}
			}
		}
	}
	else
	{
		SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /movegate [Password(Case Sensitive)]");
	}
	return 1;
}

CMD:gatepass(playerid, params[])
{
	if(PlayerInfo[playerid][pHouseID] != 0)
	{
		new newpw[64], changedcount = 0;
		if(!sscanf(params, "s[64]", newpw))
		{
			if(!ContainsValidCharacters(newpw)) return SendClientMessage(playerid, COLOR_GREY, "Invalid parameters.  Do not use /, \\, :, *, ?, \", <, >, [, ], or |.");//"
			for(new gt =1; gt <MAX_GATES; gt++)
			{
				if(Gates[gt][GateCreated] == 1)
				{
					if(Gates[gt][GateHouseID] == HouseInfo[PlayerInfo[playerid][pHouseID]][hSQLID])
					{
						strdel(Gates[gt][GatePW], 0, strlen(Gates[gt][GatePW]));
						strins(Gates[gt][GatePW], newpw, 0);
						changedcount++;
					}
				}
			}
			if(changedcount == 0)
			{
				SendClientMessage(playerid, COLOR_GREY, "No connected gates found.");
			}
			else
			{
				format(str, sizeof(str), "Password of %i gates were changed to '%s'.", changedcount, newpw);
				SendClientMessage(playerid, COLOR_WHITE, str);
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /gatepass [New Password(Case Sensitive)]");
		}
	}
	else
	{
		SendClientMessage(playerid, COLOR_RED, "ERROR: You do not own a house.");
	}
	return 1;
}

CMD:gate(playerid, params[])
{
	for(new gateid = 1; gateid < MAX_GATES; gateid++)
	{	
		if(PlayerInfo[playerid][pMember] > 0 || PlayerInfo[playerid][pGroup] == GROUP_S9 || PlayerInfo[playerid][pGroup] == GROUP_S99)
		{
			if(Gates[gateid][Faction] > 0) // If the gate is attached to a faction //
			{
				if(IsPlayerInRangeOfPoint(playerid, Gates[gateid][GateRadius], Gates[gateid][GateX], Gates[gateid][GateY], Gates[gateid][GateZ]))
				{
					if(
						(Gates[gateid][Faction] == PlayerInfo[playerid][pMember]) ||
						(PlayerInfo[playerid][pGroup] == GROUP_S9) || (PlayerInfo[playerid][pGroup] == GROUP_S99) ||
						(PlayerInfo[playerid][pMember] == FACTION_FBI && Gates[gateid][Faction] != FACTION_SAPD) 
					)	// If player is in the same faction as the gate is attached to, then
					{
						if(Gates[gateid][GateStatus] == 0) // If it's closed
						{
							// Open it //
							MoveDynamicObject(Gates[gateid][GateObject], Gates[gateid][MveGateX], Gates[gateid][MveGateY], Gates[gateid][MveGateZ], Gates[gateid][GateSpeed], Gates[gateid][MveGateRotX], Gates[gateid][MveGateRotY], Gates[gateid][MveGateRotZ]);
							Gates[gateid][GateStatus] = 1;
						}
						else // If it it's open
						{
							// Close it //
							MoveDynamicObject(Gates[gateid][GateObject], Gates[gateid][GateX], Gates[gateid][GateY], Gates[gateid][GateZ], Gates[gateid][GateSpeed], Gates[gateid][GateRotX], Gates[gateid][GateRotY], Gates[gateid][GateRotZ]);
							Gates[gateid][GateStatus] = 0;
						}
					}
				}
			}
		}
		if(PlayerInfo[playerid][pGroup] != GROUP_NONE) // If player is in any group //
		{
			if(Gates[gateid][Group] > 0) // If the gate is attached to a group //
			{
				if(IsPlayerInRangeOfPoint(playerid, Gates[gateid][GateRadius], Gates[gateid][GateX], Gates[gateid][GateY], Gates[gateid][GateZ]))
				{
					if(Gates[gateid][Group] == PlayerInfo[playerid][pGroup]) // If the player has the same group as the gate it's attached to //
					{
						if(Gates[gateid][GateStatus] == 0)	// If closed //
						{
							// open //
							MoveDynamicObject(Gates[gateid][GateObject], Gates[gateid][MveGateX], Gates[gateid][MveGateY], Gates[gateid][MveGateZ], Gates[gateid][GateSpeed], Gates[gateid][MveGateRotX], Gates[gateid][MveGateRotY], Gates[gateid][MveGateRotZ]);
							Gates[gateid][GateStatus] = 1;
						}
						else // If open //
						{
							// close //
							MoveDynamicObject(Gates[gateid][GateObject], Gates[gateid][GateX], Gates[gateid][GateY], Gates[gateid][GateZ], Gates[gateid][GateSpeed], Gates[gateid][GateRotX], Gates[gateid][GateRotY], Gates[gateid][GateRotZ]);
							Gates[gateid][GateStatus] = 0;
						}
					}
				}
			}
		}
	}
	return 1;
}