#if defined BARS_INCLUDED
    #endinput
#endif

#define BARS_INCLUDED
static str[144];

enum _BarEnum
{
	Float:BarX,
	Float:BarY,
	Float:BarZ,
	BarCreated,
	BarVW,
	BarPickup,
	Text3D:BarLabel
}

new DynBars[MAX_BARS][_BarEnum];

SaveBars() {
	new idx = 0;
	new File:file;
	file = fopen("bars.cfg", io_write);
	fclose(file);
	file = fopen("bars.cfg", io_append);
	
	new writeformat[512];
	if( file ) {
		while(idx < MAX_BARS ) {
		
			format( writeformat, sizeof( writeformat ), "%f|%f|%f|%d|%d\n", DynBars[idx][BarX], DynBars[idx][BarY], DynBars[idx][BarZ], DynBars[idx][BarVW], DynBars[idx][BarCreated] );
			fwrite( file, writeformat );
			idx++;
		}
		fclose( file );
	} 
	else {
		print( "[error] Failed to find bars.cfg (saving)" );
	}
	
			
	return 1;
}

//returns free bar slot, else max = invalid
GetFreeBarID() {

	for( new i; i < MAX_BARS; i++ ) {
		if( !DynBars[i][BarCreated] ) {
			return i;
		}
	}
	return MAX_BARS;
}			
LoadBars()
{
	new output[5][30];
	new fileOutput[512];
	
	new File:file = fopen( "bars.cfg", io_read );
	
	if( file ) {
	
		new idx = 0;
		while( idx < MAX_BARS ) {
		
			fread( file, fileOutput );
			split( fileOutput, output, '|' );
			DynBars[idx][BarX] = floatstr( output[0] );
			DynBars[idx][BarY] = floatstr( output[1] );
			DynBars[idx][BarZ] = floatstr( output[2] );
			DynBars[idx][BarVW] = strvalEx( output[3] );
			DynBars[idx][BarCreated] = strvalEx( output[4] );
			
			if( DynBars[idx][BarCreated] ) {
				DynBars[idx][BarPickup] = CreateDynamicPickup( 1239, 1, DynBars[idx][BarX], DynBars[idx][BarY], DynBars[idx][BarZ], DynBars[idx][BarVW] );
				DynBars[idx][BarLabel] = CreateDynamic3DTextLabel("BAR\n{FFFFFF}/getdrink", 0x6E88FFFF, DynBars[idx][BarX], DynBars[idx][BarY], DynBars[idx][BarZ] +0.85, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, DynBars[idx][BarVW], -1, -1, 100);
			}
			
			idx++;
			
		}
		fclose( file );		
	} 
	else {
		printf( "[error] Failed to find bars.cfg (loading)" );
	}
	return 1;
}

//returns if bar is valid
IsValidBar( barid ) {
	return DynBars[barid][BarCreated];
}

stock IsAtBar(playerid)
{
	if(IsPlayerInRangeOfPoint(playerid, 4.0, 495.7801, -76.0305, 998.7578) || IsPlayerInRangeOfPoint(playerid, 4.0, 499.9654, -20.2515, 1000.6797)) return 1; //10GB
	if(IsPlayerInRangeOfPoint(playerid, 4.0, 681.4642, -455.4616, -25.6099) || IsPlayerInRangeOfPoint(playerid, 4.0, 2496.7192, -943.5505, 39.9100)) return 1; //Welcome pump / Comando Vermelho HQ
	if(IsPlayerInRangeOfPoint(playerid, 4.0, -787.3099, 498.3217, 1371.7422) || IsPlayerInRangeOfPoint(playerid, 4.0, -784.7496, 498.3208, 1371.7422)) return 1; //Mafia
	if(IsPlayerInRangeOfPoint(playerid, 4.0, 1215.9480, -13.3519, 1000.9219) || IsPlayerInRangeOfPoint(playerid, 10.0, -2658.9749, 1407.4136, 906.2734)) return 1; //Pigpen
	if(IsPlayerInRangeOfPoint(playerid, 6.0, 1144.7220, -4.0717, 1000.6719) || IsPlayerInRangeOfPoint(playerid, 4.0, 968.8607, -46.3676, 1001.1172)) return 1; //Casino & Tigerskin Brothel
	if(IsPlayerInRangeOfPoint(playerid, 6.0, 1258.2196, -794.0273, 1084.2344) || IsPlayerInRangeOfPoint(playerid, 6.0, 1557.1156, -1896.4205, 869.0624)) return 1; // Madd Doggs Mansion & Triads Club
	if(IsPlayerInRangeOfPoint(playerid, 15.0, 1950.8726, 1017.5034, 992.4745) || IsPlayerInRangeOfPoint(playerid, 6.0, 2537.5300, -1285.6221, 1054.6406)) return 1; //??? & Big Smoke House
	if(IsPlayerInRangeOfPoint(playerid, 2.0, 2814.4380, -1165.8096, 1025.5778) || IsPlayerInRangeOfPoint(playerid, 6.0, 1525.95, -1472.02, 1037.00)) return 1; //  Colonel Furnburger?
	return 0;
}

CMD:movebar(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 4) return AdmErrorMsg;
	new id;
	if(sscanf(params, "i", id)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /movebar [barid]");
	{
		if( !IsValidBar( id ) ) return SendClientMessage(playerid, COLOR_RED, "** Invalid bar ID.");
		new Float:x, Float:y, Float:z, vw = GetPlayerVirtualWorld( playerid );
		GetPlayerPos(playerid, x, y, z);
		DynBars[id][BarX] = x;
		DynBars[id][BarY] = y;
		DynBars[id][BarZ] = z;
		DynBars[id][BarVW] = vw;
		DestroyDynamicPickup(DynBars[id][BarPickup]);
		DynBars[id][BarPickup] = CreateDynamicPickup( 1239, 1, DynBars[id][BarX], DynBars[id][BarY], DynBars[id][BarZ], DynBars[id][BarVW] );
		DestroyDynamic3DTextLabel(DynBars[id][BarLabel]);
		DynBars[id][BarLabel] = CreateDynamic3DTextLabel("BAR\n{FFFFFF}/getdrink", 0x6E88FFFF, DynBars[id][BarX], DynBars[id][BarY], DynBars[id][BarZ] +0.85, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, DynBars[id][BarVW], -1, -1, 100);
		format(str, sizeof(str), "You have successfully moved Bar ID %d.", id);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
	}
	return 1;
}

CMD:deletebar(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 4) return AdmErrorMsg;
	new id;
	if(sscanf(params, "i", id)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /deletebar [barid]");
	{
		if( !IsValidBar( id ) ) return SendClientMessage(playerid, COLOR_RED, "** Invalid bar ID.");
		DynBars[id][BarCreated] = 0;
		DestroyDynamicPickup( DynBars[id][BarPickup] );
		DestroyDynamic3DTextLabel( DynBars[id][BarLabel] );
	}
	return 1;
}


CMD:getbarid(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 4) return AdmErrorMsg;

	for(new i = 0; i < MAX_BARS; i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 3, DynBars[i][BarX], DynBars[i][BarY], DynBars[i][BarZ]))
		{
			format(str, sizeof(str), "* Bar ID is %d.", i);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			return 1;
		}
	}
	SendClientMessage(playerid, COLOR_GREY, "You're not near a bar.");
	return 1;
}

CMD:addbar(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 4) return AdmErrorMsg;

	new Float:x, Float:y, Float:z, string[128], vw = GetPlayerVirtualWorld( playerid ), barID = GetFreeBarID();
	if( barID == MAX_BARS ) {
		return SendClientMessage( playerid, COLOR_GREY, "MAX_BARS has been reached." );
	}
	
	GetPlayerPos(playerid, x, y, z);
	
	format(string, sizeof(string), "[Dynamic Bar System] You have created a new bar with the ID of %d.", barID);
	SendClientMessage(playerid, COLOR_LIGHTBLUE, string);
	DynBars[barID][BarX] = x;
	DynBars[barID][BarY] = y;
	DynBars[barID][BarZ] = z;
	DynBars[barID][BarZ] = z;
	DynBars[barID][BarVW] = vw;
	DynBars[barID][BarCreated] = 1;
	DynBars[barID][BarPickup] = CreateDynamicPickup( 1239, 1, DynBars[barID][BarX], DynBars[barID][BarY], DynBars[barID][BarZ], DynBars[barID][BarVW] );
	DynBars[barID][BarLabel] = CreateDynamic3DTextLabel("BAR\n{FFFFFF}/getdrink", 0x6E88FFFF, DynBars[barID][BarX], DynBars[barID][BarY], DynBars[barID][BarZ] +0.85, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, DynBars[barID][BarVW], -1, -1, 100);
	
	return 1;
}

stock DeleteBar(id)
{
	new filename[50];
	format(filename, sizeof(filename), "BAR%i.ini", id);
	dini_Remove(filename);
	DynBars[id][BarX] = 0.0;
	DynBars[id][BarY] = 0.0;
	DynBars[id][BarZ] = 0.0;
	DestroyDynamicPickup(DynBars[id][BarPickup]);
	DestroyDynamic3DTextLabel(DynBars[id][BarLabel]);
	DynBars[id][BarLabel] = Text3D:INVALID_3DTEXT_ID;
	return 1;
}