#define MAX_GARAGES 500

static str[144], sendername[MAX_PLAYER_NAME+1];
new Text3D:GarageInfoText3D[MAX_GARAGES];
new EditingGarage[MAX_PLAYERS];

enum gInfo
{
	gCreatedBy[MAX_PLAYER_NAME+1],
	gCreated,
	gOwnerID,
	Float:gOutsideX,
	Float:gOutsideY,
	Float:gOutsideZ,
	Float:gOutsideA,
	Float:gOutsideCarX,
	Float:gOutsideCarY,
	Float:gOutsideCarZ,
	Float:gInsideX,
	Float:gInsideY,
	Float:gInsideZ,
	Float:gInsideA,
	gLocked,
	gOutsidePickup,
};
new GarageInfo[MAX_GARAGES][gInfo];

stock SetGarageType(gid, type) //more types might get added in the future..
{
	if(GarageInfo[gid][gCreated] != 0)
	{
		switch(type)
		{
			case 1:
			{
				GarageInfo[gid][gInsideX] = 2565.6995;
				GarageInfo[gid][gInsideY] = -832.8325;
				GarageInfo[gid][gInsideZ] = 68;
				GarageInfo[gid][gInsideA] = -90;
			}
			case 2:
			{
				GarageInfo[gid][gInsideX] = 2565.6995;
				GarageInfo[gid][gInsideY] = -845.6425;
				GarageInfo[gid][gInsideZ] = 68;
				GarageInfo[gid][gInsideA] = -90;
			}
		}
	}
}

CMD:editgarage(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 4 || PlayerInfo[playerid][pDev] >= 2 || PlayerInfo[playerid][pMapper] == MAPPERRANK_MANAGER || PlayerInfo[playerid][pBizMod] != 0 || PlayerInfo[playerid][pShopMod] != 0)
	{
		new action[24], nothing;
		if(!sscanf(params, "s[24]I(-1)", action, nothing))
		{
			if(!strcmp(action, "create", true))
			{
				new gType, confirmed[24];
				if(!sscanf(params, "s[24]iS()[24]", action, gType, confirmed))
				{
					if(gType < 1 || gType > 2) return SendClientMessage(playerid, COLOR_GRAD2, "Invalid garage type.");
					new garageid, bool:found =false;
					for(new i =1; i <MAX_GARAGES; i++)
					{
						if(GarageInfo[i][gCreated] == 0)
						{
							found = true;
							garageid = i;
							break;
						}
					}
					if(found)
					{
						new Float:x, Float:y, Float:z, Float:a, garagestring[64];
						GetPlayerFacingAngle(playerid, a);
						GetPlayerPos(playerid, x, y, z);
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						GarageInfo[garageid][gCreatedBy] = sendername;
						GarageInfo[garageid][gOutsideX] = x;
						GarageInfo[garageid][gOutsideY] = y;
						GarageInfo[garageid][gOutsideZ] = z;
						GarageInfo[garageid][gOutsideCarX] = x;
						GarageInfo[garageid][gOutsideCarY] = y;
						GarageInfo[garageid][gOutsideCarZ] = z;
						GarageInfo[garageid][gOutsideA] = (a -180);
						GarageInfo[garageid][gCreated] = 1;
						SetGarageType(garageid, gType);
						if(GarageInfo[garageid][gOutsidePickup])
						{
							DestroyDynamicPickup(GarageInfo[garageid][gOutsidePickup]);
						}
						GarageInfo[garageid][gOutsidePickup] = CreateDynamicPickup(2485, 1, GarageInfo[garageid][gOutsideX], GarageInfo[garageid][gOutsideY], (GarageInfo[garageid][gOutsideZ]));
						//format(garagestring, sizeof(garagestring), "HID: %d", GarageInfo[garageid][gOwnerID]);
						format(garagestring, sizeof(garagestring), "{E2C582}Garage{FFFFFF}\nOwner ID: %d", GarageInfo[garageid][gOwnerID]);
						GarageInfoText3D[garageid] = CreateDynamic3DTextLabel(garagestring, COLOR_GARAGETEXT, GarageInfo[garageid][gOutsideX], GarageInfo[garageid][gOutsideY], GarageInfo[garageid][gOutsideZ] +0.5, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
						format(str, sizeof(str), "You have succesfully spawned garage(%d) at your current position.", garageid);
						SendClientMessage(playerid, COLOR_GRAD1, str);
						format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has spawned garage(%d) at their position.", PlayerICName(playerid), garageid);
						ABroadCast(COLOR_WHITE, str, 1);
						EditingGarage[playerid] = garageid;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GRAD2, "Error: Maximum garages have been reached.");
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GRAD2, "{00BFFF}Usage:{FFFFFF} /editgarage create [garagetype (1-2)]");
				}
			}
			else if(!strcmp(action, "id", true))
			{
				new gid;
				if(!sscanf(params, "s[24]i", action, gid))
				{
					if(gid == 0 || gid > MAX_GARAGES) return SendClientMessage(playerid, COLOR_GRAD1, "Invalid garage ID.");
					if(GarageInfo[gid][gCreated] == 1)
					{
						EditingGarage[playerid] = gid;
						format(str, sizeof(str), "You are now editing garage(%d).", EditingGarage[playerid]);
						SendClientMessage(playerid, COLOR_GRAD1, str);
					}
					else
					{
						SendClientMessage(playerid, COLOR_GRAD1, "Invalid garage Id.");
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GRAD2, "{00BFFF}Usage:{FFFFFF} /editgarage id [GARAGEID)]");
				}
			}
			else if(!strcmp(action, "pickupposition", true))
			{
				if(EditingGarage[playerid] != 0)
				{
					if(!sscanf(params, "s[24]", action))
					{
						new Float:px, Float:py, Float:pz, Float:pa;
						GetPlayerPos(playerid, px, py, pz);
						GetPlayerFacingAngle(playerid, pa);
						DestroyDynamic3DTextLabel(GarageInfoText3D[EditingGarage[playerid]]);
						GarageInfo[EditingGarage[playerid]][gOutsideX] = px;
						GarageInfo[EditingGarage[playerid]][gOutsideY] = py;
						GarageInfo[EditingGarage[playerid]][gOutsideZ] = pz;
						GarageInfo[EditingGarage[playerid]][gOutsideA] = pa;
						GarageInfo[EditingGarage[playerid]][gOutsideCarX] = px;
						GarageInfo[EditingGarage[playerid]][gOutsideCarY] = py;
						GarageInfo[EditingGarage[playerid]][gOutsideCarZ] = pz;
						if(GarageInfo[EditingGarage[playerid]][gOutsidePickup])
						{
							DestroyDynamicPickup(GarageInfo[EditingGarage[playerid]][gOutsidePickup]);
						}
						GarageInfo[EditingGarage[playerid]][gOutsidePickup] = CreateDynamicPickup(2485, 1, GarageInfo[EditingGarage[playerid]][gOutsideX], GarageInfo[EditingGarage[playerid]][gOutsideY], GarageInfo[EditingGarage[playerid]][gOutsideZ]);
						new garagestring[64];
						//format(garagestring, sizeof(garagestring), "HID: %d", GarageInfo[EditingGarage[playerid]][gOwnerID]);
						format(garagestring, sizeof(garagestring), "{E2C582}Garage{FFFFFF}\nOwner ID: %d", GarageInfo[EditingGarage[playerid]][gOwnerID]);
						GarageInfoText3D[EditingGarage[playerid]] = CreateDynamic3DTextLabel(garagestring, COLOR_GARAGETEXT, GarageInfo[EditingGarage[playerid]][gOutsideX], GarageInfo[EditingGarage[playerid]][gOutsideY], GarageInfo[EditingGarage[playerid]][gOutsideZ] +0.5, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
						UpdateDynamic3DTextLabelText(GarageInfoText3D[EditingGarage[playerid]], COLOR_GARAGETEXT, garagestring);
						format(str, sizeof(str), "You have fully moved garage(%d) to your current position.", EditingGarage[playerid]);
						SendClientMessage(playerid, COLOR_GRAD1, str);
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GRAD1, "You are not editing any garage.");
				}
			}
			else if(!strcmp(action,"enterposition",true))
			{
				if(EditingGarage[playerid] != 0){
					new Float:px, Float:py, Float:pz, Float:pa;
					GetPlayerPos(playerid, px, py, pz);

					if(IsPlayerInAnyVehicle(playerid))
					{
						GetVehicleZAngle(GetPlayerVehicleID(playerid), pa);
					} else {
						GetPlayerFacingAngle(playerid, pa);
					}

					GarageInfo[EditingGarage[playerid]][gInsideX] = px;
					GarageInfo[EditingGarage[playerid]][gInsideY] = py;
					GarageInfo[EditingGarage[playerid]][gInsideZ] = pz;
					GarageInfo[EditingGarage[playerid]][gInsideA] = pa;
					format(str, sizeof(str), "Enter position of garage(%d) adjusted.", EditingGarage[playerid]);
					SendClientMessage(playerid, COLOR_GRAD1, str);
				}
				else{
					SendClientMessage(playerid, COLOR_GRAD1, "You are not editing any garage.");
				}
			}
			else if(!strcmp(action,"exitposition",true))
			{
				if(EditingGarage[playerid] != 0)
				{
					new Float:px, Float:py, Float:pz, Float:pa;
					GetPlayerPos(playerid, px, py, pz);

					if(IsPlayerInAnyVehicle(playerid))
					{
						GetVehicleZAngle(GetPlayerVehicleID(playerid), pa);
					} else {
						GetPlayerFacingAngle(playerid, pa);
					}

					GarageInfo[EditingGarage[playerid]][gOutsideCarX] = px;
					GarageInfo[EditingGarage[playerid]][gOutsideCarY] = py;
					GarageInfo[EditingGarage[playerid]][gOutsideCarZ] = pz;
					GarageInfo[EditingGarage[playerid]][gOutsideA] = pa;
					format(str, sizeof(str), "Exit position of garage(%d) adjusted.", EditingGarage[playerid]);
					SendClientMessage(playerid, COLOR_GRAD1, str);
				}
				else{
					SendClientMessage(playerid, COLOR_GRAD1, "	You are not editing any garage !");
				}
			}
			/*else if(!strcmp(action, "houseid", true))
			{
				if(EditingGarage[playerid] != 0)
				{
					new hid;
					if(!sscanf(params, "s[24]i", action, hid))
					{
						if(HouseInfo[hid][hCreated] == 1)
						{
							GarageInfo[EditingGarage[playerid]][gOwnerID] = hid;
							format(str, sizeof(str), "Garage(%d) was succesfully connected to house %i.", EditingGarage[playerid], hid);
							SendClientMessage(playerid, COLOR_GRAD1, str);
							new garagestring[64];
							//format(garagestring, sizeof(garagestring), "HID: %d", GarageInfo[EditingGarage[playerid]][gOwnerID]);
							format(garagestring, sizeof(garagestring), "{E2C582}Garage{FFFFFF}\nOwner ID: %d", GarageInfo[EditingGarage[playerid]][gOwnerID]);
							UpdateDynamic3DTextLabelText(GarageInfoText3D[EditingGarage[playerid]], COLOR_DARKRED, garagestring);
						}
						else
						{
							SendClientMessage(playerid, COLOR_GRAD1, "Invalid house ID.");
						}
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /editgarage houseid [HOUSEID(to connect the garage to)]");
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GRAD1, "You are not editing any garage.");
				}
			}*/
			else if(!strcmp(action, "ownerid", true))
			{
				if(EditingGarage[playerid] != 0)
				{
					new oid;
					if(!sscanf(params, "s[24]i", action, oid))
					{

						GarageInfo[EditingGarage[playerid]][gOwnerID] = oid;
						format(str, sizeof(str), "Garage(%d) was succesfully connected to Owner ID %i.", EditingGarage[playerid], oid);
						SendClientMessage(playerid, COLOR_GRAD1, str);
						new garagestring[64];
						//format(garagestring, sizeof(garagestring), "HID: %d", GarageInfo[EditingGarage[playerid]][gOwnerID]);
						format(garagestring, sizeof(garagestring), "{E2C582}Garage{FFFFFF}\nOwner ID: %d", GarageInfo[EditingGarage[playerid]][gOwnerID]);
						UpdateDynamic3DTextLabelText(GarageInfoText3D[EditingGarage[playerid]], COLOR_DARKRED, garagestring);

					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /editgarage ownerid [/nametosql for the SQL ID]");
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GRAD1, "You are not editing any garage.");
				}
			}
			else if(!strcmp(action, "delete", true))
			{
				if(EditingGarage[playerid] != 0)
				{
					strmid(GarageInfo[EditingGarage[playerid]][gCreatedBy], "", 0, strlen("None"), 255);
					DestroyDynamic3DTextLabel(GarageInfoText3D[EditingGarage[playerid]]);
					GarageInfoText3D[EditingGarage[playerid]] = Text3D:INVALID_3DTEXT_ID;
					GarageInfo[EditingGarage[playerid]][gOutsideX] = 0;
					GarageInfo[EditingGarage[playerid]][gOutsideY] = 0;
					GarageInfo[EditingGarage[playerid]][gOutsideZ] = 0;
					GarageInfo[EditingGarage[playerid]][gOutsideA] = 0;
					GarageInfo[EditingGarage[playerid]][gOutsideCarX] = 0;
					GarageInfo[EditingGarage[playerid]][gOutsideCarY] = 0;
					GarageInfo[EditingGarage[playerid]][gOutsideCarZ] = 0;
					GarageInfo[EditingGarage[playerid]][gInsideX] = 0;
					GarageInfo[EditingGarage[playerid]][gInsideY] = 0;
					GarageInfo[EditingGarage[playerid]][gInsideZ] = 0;
					GarageInfo[EditingGarage[playerid]][gInsideA] = 0;
					GarageInfo[EditingGarage[playerid]][gLocked] = 0;
					GarageInfo[EditingGarage[playerid]][gOwnerID] = 0;
					GarageInfo[EditingGarage[playerid]][gCreated] = 0;
					if(GarageInfo[EditingGarage[playerid]][gOutsidePickup])
					{
						DestroyDynamicPickup(GarageInfo[EditingGarage[playerid]][gOutsidePickup]);
					}
					format(str, sizeof(str), "You have fully deleted garage(%d).", EditingGarage[playerid]);
					SendClientMessage(playerid, COLOR_GRAD1, str);
					format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has deleted garage(%d).", PlayerICName(playerid), EditingGarage[playerid]);
					ABroadCast(COLOR_WHITE, str, 1);
					EditingGarage[playerid] = 0;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GRAD1, "You are not editing any garage.");
				}
			}
			else if(!strcmp(action, "stop", true))
			{
				if(EditingGarage[playerid] != 0)
				{
					SendClientMessage(playerid, COLOR_GRAD1, "Editing progress was ended.");
					EditingGarage[playerid] = 0;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GRAD1, "You are not editing any garage.");
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_WHITE, "Invalid action.");
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /editgarage [action]");
			SendClientMessage(playerid, COLOR_GREY, "Available actions: create, id, pickupposition, enterposition, exitposition, houseid, delete, stop");
		}
	}
	else
	{
		AdmErrorMsg;
	}
	return 1;
}

CMD:gotogarage(playerid, params[])
{
	new gid;
	if(PlayerInfo[playerid][pDev] >= 2 || PlayerInfo[playerid][pAdmin] >= 4 || PlayerInfo[playerid][pMapper] == MAPPERRANK_MANAGER)
	{
		if(GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
		{
			SendClientMessage(playerid, COLOR_GREY, "You can not do that while spectating.");
			return 1;
		}
		if(!sscanf(params, "d", gid))
		{
			if(gid > MAX_GARAGES)
			{
				return SendClientMessage(playerid, COLOR_GRAD1, "Garage ID was above the max garages.");
			}
			if(GarageInfo[gid][gCreated] == 1)
			{
				SetPlayerPos(playerid, GarageInfo[gid][gOutsideX], GarageInfo[gid][gOutsideY], GarageInfo[gid][gOutsideZ]);
				SetPlayerInterior(playerid, 0);
				PlayerInfo[playerid][pInt] = 0;
				SetPlayerVirtualWorld(playerid, 0);
				PlayerInfo[playerid][pVirtualWorld] = 0;
				format(str, sizeof(str), "~w~Teleporting to ~b~Garage~w~.");
				displayCenterHUDInfo(playerid,  str, 8); //display for 8 seconds
			}
			else
			{
				SendClientMessage(playerid, COLOR_GRAD1, "Invalid garage ID.");
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GRAD2, "{00BFFF}Usage:{FFFFFF} /gotogarage [Garageid]");
		}
	}
	else
	{
		AdmErrorMsg;
	}
	return 1;
}

CMD:garageinfo(playerid, params[])
{
	if(PlayerInfo[playerid][pDev] >= 2 || PlayerInfo[playerid][pAdmin] >= 4 || PlayerInfo[playerid][pMapper] == MAPPERRANK_MANAGER)
	{
		for(new gr =1; gr <MAX_GARAGES; gr++)
		{
			if(GarageInfo[gr][gCreated] == 1)
			{
				if(IsPlayerInRangeOfPoint(playerid, 4.0, GarageInfo[gr][gOutsideX], GarageInfo[gr][gOutsideY], GarageInfo[gr][gOutsideZ]))
				{
					SendClientMessage(playerid, COLOR_YELLOW, "____________________________________________");
					format(str, sizeof(str), "Garage ID : %i", gr);
					SendClientMessage(playerid, COLOR_WHITE, str);
					format(str, sizeof(str), "Created by: %s", GarageInfo[gr][gCreatedBy]);
					SendClientMessage(playerid, COLOR_WHITE, str);
					format(str, sizeof(str), "Owner ID: %i [/sqltoname for owner name]", GarageInfo[gr][gOwnerID]);
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

CMD:lockgarage(playerid, params[])
{
	new gcount = 0, chcount = 0;
	for(new gr =1; gr <MAX_GARAGES; gr++)
	{
		if(GarageInfo[gr][gCreated] == 1)
		{
			if(GarageInfo[gr][gOwnerID] == PlayerInfo[playerid][pID])
			{
				if(IsPlayerInRangeOfPoint(playerid, 4.0, GarageInfo[gr][gOutsideX], GarageInfo[gr][gOutsideY], GarageInfo[gr][gOutsideZ]))
				{

					if(GarageInfo[gr][gLocked] == 0)
					{
						GarageInfo[gr][gLocked] = 1;
						SendClientMessage(playerid, COLOR_GREY, "Garage {FF0000}locked{FFFFFF}.");
						format(str, sizeof(str), "* %s has locked their garage.", PlayerICName(playerid));
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					}
					else
					{
						GarageInfo[gr][gLocked] = 0;
						SendClientMessage(playerid, COLOR_GREY, "Garage {00FF00}unlocked{FFFFFF}.");
						format(str, sizeof(str), "* %s has unlocked their garage.", PlayerICName(playerid));
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					}
					chcount++;
				}
				gcount++;
			}
		}
	}
	if(gcount == 0)
	{
		SendClientMessage(playerid, COLOR_RED, "You do not own a garage	.");
	}
	else if(chcount == 0)
	{
		SendClientMessage(playerid, COLOR_RED, "You are not in range of your garage	.");
	}
	return 1;
}

//==================================[GARAGES]===================================
public SaveGarages()
{
	new idx = 1;
	new File:file;
	file = fopen("garages.cfg", io_write);
	fclose(file);
	file = fopen("garages.cfg", io_append);
	new coordsstring[512];
	if(file) {
		while(idx < sizeof(GarageInfo))
		{
			format(coordsstring, sizeof(coordsstring), "%s|%d|%d|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%d\r\n",
				   GarageInfo[idx][gCreatedBy],
				   GarageInfo[idx][gCreated],
				   GarageInfo[idx][gOwnerID],
				   GarageInfo[idx][gOutsideX],
				   GarageInfo[idx][gOutsideY],
				   GarageInfo[idx][gOutsideZ],
				   GarageInfo[idx][gOutsideA],
				   GarageInfo[idx][gOutsideCarX],
				   GarageInfo[idx][gOutsideCarY],
				   GarageInfo[idx][gOutsideCarZ],
				   GarageInfo[idx][gInsideX],
				   GarageInfo[idx][gInsideY],
				   GarageInfo[idx][gInsideZ],
				   GarageInfo[idx][gInsideA],
				   GarageInfo[idx][gLocked]);
			fwrite(file, coordsstring);
			idx++;
		}
		fclose(file);
	} else {
		printf("[error] Failed to find garages.cfg");
	}
	return 1;
}

public LoadGarages()
{
	new arrCoords[15][126], strFromFile2[512], File: file = fopen("garages.cfg", io_read);
	if(file)
	{
		new idx = 1;
		while(idx < sizeof(GarageInfo))
		{
			fread(file, strFromFile2);
			split(strFromFile2, arrCoords, '|');
			strmid(GarageInfo[idx][gCreatedBy], arrCoords[0], 0, strlen(arrCoords[0]), 255);
			GarageInfo[idx][gCreated] = strval(arrCoords[1]);
			GarageInfo[idx][gOwnerID] = strval(arrCoords[2]);
			GarageInfo[idx][gOutsideX] = floatstr(arrCoords[3]);
			GarageInfo[idx][gOutsideY] = floatstr(arrCoords[4]);
			GarageInfo[idx][gOutsideZ] = floatstr(arrCoords[5]);
			GarageInfo[idx][gOutsideA] = floatstr(arrCoords[6]);
			GarageInfo[idx][gOutsideCarX] = floatstr(arrCoords[7]);
			GarageInfo[idx][gOutsideCarY] = floatstr(arrCoords[8]);
			GarageInfo[idx][gOutsideCarZ] = floatstr(arrCoords[9]);
			GarageInfo[idx][gInsideX] = floatstr(arrCoords[10]);
			GarageInfo[idx][gInsideY] = floatstr(arrCoords[11]);
			GarageInfo[idx][gInsideZ] = floatstr(arrCoords[12]);
			GarageInfo[idx][gInsideA] = floatstr(arrCoords[13]);
			GarageInfo[idx][gLocked] = strval(arrCoords[14]);
			new string3[64];
			if(GarageInfo[idx][gCreated] == 1)
			{
				if(GarageInfo[idx][gOutsidePickup])
				{
					DestroyDynamicPickup(GarageInfo[idx][gOutsidePickup]);
				}
				GarageInfo[idx][gOutsidePickup] = CreateDynamicPickup(2485, 1, GarageInfo[idx][gOutsideX], GarageInfo[idx][gOutsideY], GarageInfo[idx][gOutsideZ]);
				//format(string3, sizeof(string3), "HID: %d", GarageInfo[idx][gOwnerID]);
				format(string3, sizeof(string3), "{E2C582}Garage{FFFFFF}\nOwner ID: %d", GarageInfo[idx][gOwnerID]);
				GarageInfoText3D[idx] = CreateDynamic3DTextLabel(string3, COLOR_GARAGETEXT, GarageInfo[idx][gOutsideX], GarageInfo[idx][gOutsideY], GarageInfo[idx][gOutsideZ] +0.5, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
				UpdateDynamic3DTextLabelText(GarageInfoText3D[idx], COLOR_GARAGETEXT, string3);
			}
			idx++;
		}
		fclose(file);
	} else {
		printf("[error] Failed to find garages.cfg");
	}
	return 1;
}

CMD:agaragehelp(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 4 || PlayerInfo[playerid][pMapper] == 3)
	{
		SendClientMessage(playerid, COLOR_YELLOW, "____________________________________________");
		SendClientMessage(playerid, COLOR_WHITE, "COMMANDS: /editgarage, /gotogarage, /garageinfo");
	}
	else
	{
		AdmErrorMsg;
	}
	return 1;
}