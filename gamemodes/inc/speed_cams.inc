/*
   _____                     _    _____                                        ____           
  / ____|                   | |  / ____|                                      |  _ \          
 | (___  _ __   ___  ___  __| | | |     __ _ _ __ ___   ___ _ __ __ _ ___     | |_) |_   _    
  \___ \| '_ \ / _ \/ _ \/ _` | | |    / _` | '_ ` _ \ / _ \ '__/ _` / __|    |  _ <| | | |   
  ____) | |_) |  __/  __/ (_| | | |___| (_| | | | | | |  __/ | | (_| \__ \    | |_) | |_| |   
 |_____/| .__/ \___|\___|\__,_|  \_____\__,_|_| |_| |_|\___|_|  \__,_|___/    |____/ \__, |   
        | |                                                                           __/ |   
  _____ |_|                                  _                _____                  |___/    
 |  __ \                                    | |     ___      / ____|                | |       
 | |  | | ___  ___ _ __ ___   ___  _ __   __| |    ( _ )    | (___  _ __   __ _ _ __| | _____ 
 | |  | |/ _ \/ __| '_ ` _ \ / _ \| '_ \ / _` |    / _ \/\   \___ \| '_ \ / _` | '__| |/ / _ \
 | |__| |  __/\__ \ | | | | | (_) | | | | (_| |   | (_>  <   ____) | |_) | (_| | |  |   <  __/
 |_____/ \___||___/_| |_| |_|\___/|_| |_|\__,_|    \___/\/  |_____/| .__/ \__,_|_|  |_|\_\___|
                                                                   | |                        
                                                                   |_| 
 */

#define MAX_CAMS 	(50)
#define CAM_OBJ		(18880)

enum SpeedCams {
	speedID,
	bool:speedPlaced, // Varible we used to see if the speed cam is created/placed
	Float:speedPosition[3],
	Float:speedRotation[3],
	speedSpeed, // Speed limit
	speedCreator[MAX_PLAYER_NAME + 1],
	speedObject,
	Text3D:speed3D
};
new Cams[MAX_CAMS][SpeedCams];

Hook:speedcams_OnGameModeInit()
{
	LoadSpeed();
	return 1;
}

stock SaveSpeed(creator[], speed, Float:x, Float:y, Float:z, Float: rotx, Float: roty, Float: rotz, id)
{
	new filename[50];
	format(filename, sizeof(filename), "Speed%i.ini", id);
	if(!dini_Exists(filename))
	{
		dini_Create(filename);
		dini_Set(filename, "Creator", creator);
		dini_FloatSet(filename, "X", x);
		dini_FloatSet(filename, "Y", y);
		dini_FloatSet(filename, "Z", z);
		dini_FloatSet(filename, "RX", rotx);
		dini_FloatSet(filename, "RY", roty);
		dini_FloatSet(filename, "RZ", rotz);
		dini_IntSet(filename, "speed", speed);
	}
	else
	{
		dini_Set(filename, "Creator", creator);
		dini_FloatSet(filename, "X", x);
		dini_FloatSet(filename, "Y", y);
		dini_FloatSet(filename, "Z", z);
		dini_FloatSet(filename, "RX", rotx);
		dini_FloatSet(filename, "RY", roty);
		dini_FloatSet(filename, "RZ", rotz);
		dini_IntSet(filename, "speed", speed);
	}
	return 1;
}

stock LoadSpeed()
{
	new filename[50];
	for(new i = 0; i < MAX_CAMS; i++)
	{
		format(filename, sizeof(filename), "Speed%i.ini", i);
		if(!dini_Exists(filename)) continue;
		if(dini_Exists(filename))
		{
			format(Cams[i][speedCreator], MAX_PLAYER_NAME+1, "%s", dini_Get(filename, "Creator"));

			Cams[i][speedPlaced] = true;

			Cams[i][speedPosition][0] = dini_Float(filename, "X");
			Cams[i][speedPosition][1] = dini_Float(filename, "Y");
			Cams[i][speedPosition][2] = dini_Float(filename, "Z");

			Cams[i][speedRotation][0] = dini_Float(filename, "RX");
			Cams[i][speedRotation][1] = dini_Float(filename, "RY");
			Cams[i][speedRotation][2] = dini_Float(filename, "RZ");

			Cams[i][speedSpeed] = dini_Int(filename, "speed");

			Cams[i][speedObject] = CreateDynamicObject(CAM_OBJ, Cams[i][speedPosition][0], Cams[i][speedPosition][1], Cams[i][speedPosition][2], Cams[i][speedRotation][0], Cams[i][speedRotation][1], Cams[i][speedRotation][2]);

			new strmsg[128];
			format(strmsg, sizeof(strmsg), "Speed Camera\nSpeed limit: %i MPH.", Cams[i][speedSpeed]);
			Cams[i][speed3D] = Create3DTextLabel(strmsg, COLOR_ENTRANCE, Cams[i][speedPosition][0], Cams[i][speedPosition][1], Cams[i][speedPosition][2]+4, 5, 0, 0);
		}
	}
	return 1;
}

CMD:createspeed(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] > 2 || PlayerInfo[playerid][pDev] >= 2)
	{
		new speed;
		if(sscanf(params, "i", speed)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /createspeed [speedlimit in MPH]");
		if(speed > 150 || speed < 50) return SendClientMessage(playerid, COLOR_GREY, "The speed limit must be between 50 and 150 MPH.");
		new strmsg[128], Float:Position[3];

		GetPlayerPos(playerid, Position[0], Position[1], Position[2]);

		for(new i = 0; i < MAX_CAMS; i++)
		{
			if(Cams[i][speedPlaced] == false)
			{
				format(strmsg, sizeof(strmsg), "You've created camera ID %i.", i);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, strmsg);

				GetPlayerName(playerid, Cams[i][speedCreator], 25);

				Cams[i][speedPlaced] = true;
				Cams[i][speedPosition][0] = Position[0];
				Cams[i][speedPosition][1] = Position[1];
				Cams[i][speedPosition][2] = Position[2];

				Cams[i][speedSpeed] = speed;

				Cams[i][speedObject] = CreateDynamicObject(CAM_OBJ, Position[0], Position[1], Position[2], 0, 0, 0);
				format(strmsg, sizeof(strmsg), "Speed Camera\nSpeed limit: %i MPH.", speed);
				Cams[i][speed3D] = Create3DTextLabel(strmsg, COLOR_ENTRANCE, Cams[i][speedPosition][0], Cams[i][speedPosition][1], Cams[i][speedPosition][2]+4, 5, 0, 0);
				EditDynamicObject(playerid, Cams[i][speedObject]);
				SaveSpeed(Cams[i][speedCreator], Cams[i][speedSpeed], Cams[i][speedPosition][0], Cams[i][speedPosition][1], Cams[i][speedPosition][2], Cams[i][speedRotation][0], Cams[i][speedRotation][1], Cams[i][speedRotation][2], i);

				new Float: temppos[3];
				GetPlayerPos(playerid, temppos[0], temppos[1], temppos[2]);
				SetPlayerPos(playerid, temppos[0]+2, temppos[1], temppos[2]);
				break;
			}
		}
	}
	return 1;
}

CMD:editspeed(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 3 && PlayerInfo[playerid][pDev] < 2) return AdmErrorMsg;

	new camid, action[30], valuevar;
	if(!sscanf(params, "is[30]i", camid, action, valuevar))
	{
		if(camid < 0 || camid > 49) return SendClientMessage(playerid, COLOR_GREY, "The camera id must be between 0 and 49.");
		new tempmsg[80];
		if(!strcmp(action, "pos", true))
		{
			format(tempmsg, sizeof(tempmsg), "You are now editing speed camera ID %i's position.", camid);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, tempmsg);
			EditDynamicObject(playerid, Cams[camid][speedObject]);
		}
		else if(!strcmp(action, "speed", true))
		{
			if(valuevar < 50 || valuevar > 150) return SendClientMessage(playerid, COLOR_GREY, "The speed limit must be between 50 and 150 MPH.");
			Cams[camid][speedSpeed] = valuevar;
			format(tempmsg, sizeof(tempmsg), "You have edited speed camera ID %i's speed limit to %i.", camid, valuevar);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, tempmsg);
			format(tempmsg, sizeof(tempmsg), "Speed Camera\nSpeed limit: %i MPH.", valuevar);
			Update3DTextLabelText(Cams[camid][speed3D], COLOR_ENTRANCE, tempmsg);
		}
		SaveSpeed(Cams[camid][speedCreator], Cams[camid][speedSpeed], Cams[camid][speedPosition][0], Cams[camid][speedPosition][1], Cams[camid][speedPosition][2], Cams[camid][speedRotation][0], Cams[camid][speedRotation][1], Cams[camid][speedRotation][2], camid);
	}
	else return SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} /editspeed [cameraid] [pos/speed] [0/value]");
	return 1;
}

CMD:deletespeed(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] > 2 || PlayerInfo[playerid][pDev] >= 2)
	{
		new camid;
		if(sscanf(params, "i", camid)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /deletespeed [camid]");
		if(Cams[camid][speedPlaced] == false) return SendClientMessage(playerid, COLOR_GREY, "That camera id is not used.");
		if(camid < 0 || camid > 49) return SendClientMessage(playerid, COLOR_GREY, "The camera id must be between 0 and 49.");

		Cams[camid][speedPlaced] = false;
		Cams[camid][speedPosition][0] = 0;
		Cams[camid][speedPosition][1] = 0;
		Cams[camid][speedPosition][2] = 0;

		Cams[camid][speedRotation][0] = 0;
		Cams[camid][speedRotation][1] = 0;
		Cams[camid][speedRotation][2] = 0;

		Cams[camid][speedSpeed] = 0;

		DestroyDynamicObjectEx(Cams[camid][speedObject]);
		Delete3DTextLabel(Cams[camid][speed3D]);

		new filename[50];
		format(filename, sizeof(filename), "Speed%i.ini", camid);
		dini_Remove(filename);

		new strmsg[50];
		format(strmsg, sizeof(strmsg), "You have deleted speed camera ID %i.", camid);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, strmsg);
	}
	return 1;
}

CMD:closeestcams(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 3 && PlayerInfo[playerid][pDev] < 2) return AdmErrorMsg;
	new strmsg[128];
	for(new i = 0; i < MAX_CAMS; i++)
	{
		if(Cams[i][speedPlaced] == true)
		{
			if(IsPlayerInRangeOfPoint(playerid, 10.0, Cams[i][speedPosition][0], Cams[i][speedPosition][1], Cams[i][speedPosition][2]))
			{
				format(strmsg, sizeof(strmsg), "Camera ID: %i | Camera Placer: %s | Max Speed: %i",
					i, Cams[i][speedCreator], Cams[i][speedSpeed]);

				SendClientMessage(playerid, COLOR_LIGHTBLUE, strmsg);
			}
		}
	}
	return 1;
}