



#define COLOR_PURPLE 		0xC2A2DAAA
#define COLOR_YELLOW 		0xFFFF00AA
#define COLOR_ADVERT 		0x00AA00FF
#define COLOR_WHITE 		0xFFFFFFAA
#define COLOR_INVALID       0xAA3333AA
#define COLOR_GREY			0xAFAFAFAA

/*
  _____                      _                    
 |  __ \                    | |                   
 | |  | | ___    _ __   ___ | |_   _   _ ___  ___ 
 | |  | |/ _ \  | '_ \ / _ \| __| | | | / __|/ _ \
 | |__| | (_) | | | | | (_) | |_  | |_| \__ \  __/
 |_____/ \___/  |_| |_|\___/ \__|  \__,_|___/\___|
                                                 
	Old DonorScripts, contains messy script, to be sorted.

*/

new donorstr[1000];
// Donor Dialog ID's
enum
{	
	//============= [Desmond DeSantos ] ===========
	DIALOG_DONOR_DESMOND,
	DIALOG_DONOR_DESMOND_EDIT,
	
	
};
	
//================ [Gunnar Watsone] ===============
#define GUNNARWATSONE_ID    21900
//================ [Desmond DeSantos] ===================
enum DesmondDrinkData
{
	DesmondDrinkName[7],
	DesmondDrinkAction
}

new DesmondDrinks[4][DesmondDrinkData] = {
	{"Wine", SPECIAL_ACTION_DRINK_WINE},
	{"Beer", SPECIAL_ACTION_DRINK_BEER},
	{"Whisky", SPECIAL_ACTION_DRINK_BEER},
	{"Sprunk", SPECIAL_ACTION_DRINK_SPRUNK}
};

new Float: DesmondCameras[5][6] = {
	{1135.598266, -13.055255, 1002.148559, 1132.390502, -16.832738, 1001.484375},
	{1115.694335, -11.405779, 1004.237731, 1119.960449, -8.962922, 1003.325195},
	{1135.258911, 9.924472, 1004.576416, 1130.951049, 7.988495, 1002.935058},
	{1113.584350, -9.115205, 1003.608276, 1118.416503, -9.451298, 1002.368225},
	{1991.4371, -1983.2247, 17.8492, 1991.1522, -1983.4623, 17.8492}
};

new DesmondPrices[4];
new DesmondPassword[64];
new DesmondSafe[4];
new DesmondSafeWeapons[10];
new Text3D: DesmondEntrance;
new bool: DesmondLocked, bool: DesmondOfficeLocked;
new DesmondEntryFee;
new bool: DesmondLoggedIn[MAX_PLAYERS];
new DesmondArea;
new DesmondURL[128];

enum
{
	DESMOND_SAFE_MONEY,
	DESMOND_SAFE_POT,
	DESMOND_SAFE_CRACK
}
;

//================ [Gunnar Watsone] =================
new gunnarsafeWeapon[10];
new gunnarsafeCash;
new gunnarsafePot;
new gunnarsafeCrack;
new gunnarsafeMats;
new gunnarNote1[200];
new gunnarNote2[200];
new gunnarNote3[200];
new gunnarNote4[200];
new gunnarNote5[200];
new gunnarKey[MAX_PLAYERS];
new gunnarOldSkin[MAX_PLAYERS];

//================ [Tiger Woods] =================
new Float:CamPos[][] = { //Use this for any cameras!
	//campos x, camposy, camposz,     lookat x, lookaty, lookatz
	{145.0163, 1713.7521, 10018.9102, 144.5235, 1712.8848, 10018.6309},//1
	{145.0163, 1713.7521, 10018.9102, 144.0274, 1713.6217, 10018.7773},//2
	{145.0163, 1713.7521, 10018.9102, 145.3284, 1712.8058, 10018.2354},//3
	{133.9780, 1689.6569, 10016.7910, 133.3040, 1690.3929, 10014.9434},//4
	{133.9780, 1689.6569, 10016.7910, 133.1417, 1690.2018, 10016.7578},//5
	{133.9780, 1689.6569, 10016.7910, 133.7589, 1690.6305, 10016.6045},//6
	{143.7413, 1715.5237, 10015.3135, 142.9722, 1714.8876, 10014.9990},//7
	{124.4656, 1715.4156, 10015.8457, 125.1684, 1714.7078, 10015.4580},//8
	{119.8937, 1705.6431, 10015.9287, 120.4830, 1704.8304, 10015.4355},//9
	{149.5217, 1705.3497, 10018.4609, 148.9465, 1706.1708, 10017.8438},//10
	{149.5351, 1724.7557, 10018.5000, 149.0394, 1723.8851, 10018.2207},//11
	{149.5351, 1724.7557, 10018.5000, 148.5678, 1724.4951, 10018.0928},//12
	{1493.2236, 2860.3435, 16.3206, 1514.9635 ,2864.2524, 12.0862},//13
	{1460.5276, 2860.5461, 16.1168, 1477.8832, 2867.5828, 10.8431},//14#
	{1458.0160, 2831.5244, 19.5481, 1454.6670, 2818.0222, 11.2895},//15
	{1420.2155, 2797.7837, 19.6450, 1301.0417, 2783.8418, 10.8203},//16
	{1524.7358, 2762.8127, 17.6711, 1492.7655, 2773.2468, 10.8203},//17
	{1524.9965, 2783.9773, 17.5221, 1540.1783, 2754.3232, 10.6719},//18
	{1482.4075, 2777.2642, 20.5517, 1458.5457, 2773.4531, 10.8203},//19
	{1413.9020, 2777.9248, 17.8448, 1424.7291, 2771.9312, 14.8203},//20
	{1461.2344, 2847.0186, 13.1862, 1477.2556, 2854.8833, 10.8459}//21
};
new bool:tbss = false, tEChair = INVALID_PLAYER_ID;
new bool:inchair[MAX_PLAYERS], twShelf, pressTimer, twCircle;


new Text:blood;
forward BloodHide(playerid);
public BloodHide(playerid) {
    TextDrawHideForPlayer(playerid, blood);
	return 1;
}

//================ [Keven Ruthless] ================
new kevruth[128], ruthlessvehicle, bool:KevListen[MAX_PLAYERS] = false;
//================ [Jacob Malcom] ================
new jacob_factoryveh;

//================ [Devin Gideon] ================
new devin_boxville;

//================ [Lloyd Christmas] ================
new knives = 0, xmas[5], bool:llcmas[5] = false, secretstorage[4], secretdoor, knife[5], towerdoor[4], towerdoorstatus = 0, LloydSS = 0, LloydSD = 0;

//================ [Aydino Mazzerelli] ================
new aydinosafeWeapon[10];
new aydinosafeCash;
new aydinosafePot;
new aydinosafeCrack;
new aydinosafeMats;
new aydinokey[MAX_PLAYERS];

//================ [Dino Cicarelli] ================
/*new dinosafeWeapon[10];
new dinosafeCash;
new dinosafePot;
new dinosafeCrack;
new dinosafeMats;*/

//================ [Ace Johnson] ================
new AceGate[4], bool:AceGateOpen[4] = false;

//================ [Skarr Torino] ===============
new SquadPassword[128], SquadAdmPassword[128], bool: SquadLogged[MAX_PLAYERS], bool: SquadAdmLogged[MAX_PLAYERS],
	SquadSafe[3], SquadArmory[13][2], bool: SquadElevatorState, bool: SquadElevatorMoving, bool: SquadLockdown = false, SquadElevator, SquadDoor, bool: SquadDoorState,
	bool: SquadDoorMoving, bool: SquadCamera[MAX_PLAYERS], SquadElevatorDoor1, SquadElevatorDoor2, SquadElevatorDoor3, SquadElevatorDoor4;
	
enum
{
	SQUAD_SAFE_CRACK,
	SQUAD_SAFE_POT,
	SQUAD_SAFE_MONEY
}

//================ [Eric Daxono] ===============
/*enum
{
	SPEAKER_LISTEN_NONE,
	SPEAKER_LISTEN_INDOOR,
	SPEAKER_LISTEN_OUTDOOR
}

new bool: outdoorspeaker = true,
	bool: indoorspeaker = true,
	Text3D: SpeakerStatus,
	Text3D: SpeakerFrequency,
	Text3D: OutpostMOTD,
	SpeakerListening[MAX_PLAYERS] = SPEAKER_LISTEN_NONE,
	// bool: NewsMute[MAX_PLAYERS],
	IndoorSpeakerChannel,
	OutpostOutdoorSpeaker,
	OutpostIndoorSpeaker,
	OutpostAegisArea,
	CurrentOutpostMOTD[64];*/

//================ [Nick Fletcher] ===============
/*new bool: fCasinoLocked = false, bool: fCasinoKey[MAX_PLAYERS] = false, bool: fCasinoCCTV[MAX_PLAYERS] = false, LastCasinoCall,
Text3D: fCasinoText;
*/


//===============[ Freddie Valdemar ]==========
new JacobUtilityTruck, jacobsiren, JacobTrailer, JacobSweeper, jacobobjects[3];


stock dOnGameModeInit()
{
	print("[debug] dOnGameModeInit");
	//===============[ Donor Related ]=====================
	//================[ Freddie Valdemar ]==========
	JacobUtilityTruck = CreateVehicle( 552, 2511.6226,-2438.4219,13.3145, 44.6, 1, 161, -1 );
	JacobSweeper = CreateVehicle( 574,2506.2109,-2418.6033,13.3450,134.8071, 1, 161, -1 );
	JacobTrailer = CreateVehicle( 611,2515.8647,-2436.7354,13.2719,45.0640, 1, 161, -1 );
	jacobsiren = CreateObject(11701,0,0,-1000,0,0,0,100);
	SetVehicleLockState( JacobSweeper, 1 );
	SetVehicleLockState( JacobUtilityTruck, 1 );
	AttachObjectToVehicle(jacobsiren, JacobUtilityTruck, 0.000000,0.674999,1.350000,0.000000,0.000000,0.000000);
	

	
	//================ [Gunnar Watsone] =================
	LoadGunnar();
	CreateDynamic3DTextLabel("{90C3D4}/chalkboard", -1, 2213.5015,550.0428,9096.8848, 10, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1);
	CreateDynamic3DTextLabel("{90C3D4}/gunnarsafe", -1, 2248.0896,556.7626,9097.7354, 5, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1);
	CreateDynamic3DTextLabel("{90C3D4}/cagefighter", -1, 2218.1853,604.1145,9097.7393, 5, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1);
	CreateDynamic3DTextLabel("{90C3D4}/gymwater", -1, 2214.8909,596.8708,9097.7402, 5, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1);

	//============== [Aydino Mazzerelli] ==================
	CreateDynamic3DTextLabel("{FF0000}Vehicle repair\n{FF0000}(/aydinofix)", COLOR_LIGHTBLUE, 2258.4585,1464.5507,9002.3848, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1);
	CreateDynamic3DTextLabel("(( /buysoda ))", -1, 1293.9017,-1318.8225,18.9866, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1);

	//LoadDino(); //================= [Dino Cicarelli] ==================
	LoadAydino(); //================ [Aydino Mazzerelli] ======================
	

	//================= [Skarr Torino] =====================
	LoadSquad();
	SquadElevator = CreateDynamicObject(18755, -943.96771, -55.71810, 1001.21130,   0.00000, 0.00000, 90.00000);
	SquadDoor = CreateDynamicObject(1569, -961.67230, -57.70300, 1004.36530,   0.00000, 0.00000, 0.0000);
	SquadElevatorDoor1 = CreateDynamicObject(18757, -944.410034, -55.688213, 1006.076171, 0.00000, 0.0000, 90.0000);
	SquadElevatorDoor2 = CreateDynamicObject(18757, -942.416015, -55.799083, 1006.076049, 0.00000, 0.0000, 90.0000);
	SquadElevatorDoor3 = CreateDynamicObject(18757, -946.029602, -55.722320, 1001.216247, 0.00000, 0.0000, 90.0000);
	SquadElevatorDoor4 = CreateDynamicObject(18757, -939.736389, -55.684394, 1001.225952, 0.00000, 0.0000, 90.0000);
	
	//================ [Keven Ruthless] ================
	ruthlessvehicle = AddStaticVehicleEx(560, 1963.3165, -1719.4249, 15.5500, 0.0000, 201, 201, -1);
	AddVehicleComponent(ruthlessvehicle, 1010);
	AddVehicleComponent(ruthlessvehicle, 1080);
	AddVehicleComponent(ruthlessvehicle, 1029);
	AddVehicleComponent(ruthlessvehicle, 1030);
	AddVehicleComponent(ruthlessvehicle, 1031);
	AddVehicleComponent(ruthlessvehicle, 1138);
	AddVehicleComponent(ruthlessvehicle, 1033);
	SetVehicleNumberPlate(ruthlessvehicle, "Ruthless");
	SetVehicleToRespawn(ruthlessvehicle);


	//================ [Jacob Malcom] ================
	jacob_factoryveh = CreateVehicle(498,-2289.7437000,-175.6069000,35.3082000,91.0000000,-1,-1,900); //Boxville


	//================ [Devin Wheatley] ================
	devin_boxville = AddStaticVehicleEx(498, 1865.6602, -1318.0272, 13.5430, 263.2390, 53, 53, -1); // Boxville


	//================ [Lloyd Christmas] ================
	xmas[0] = CreateDynamicObject(19426,1526.8753700,-1364.8575400,35.0587000,0.0000000,0.0000000,90.0000000);
	SetDynamicObjectMaterial(xmas[0], 0, 3924, "rc_warhoose", "white", 0);

	xmas[1] =  CreateDynamicObject(19429,1525.3353300,-1364.8554700,40.8157000,0.0000000,0.0000000,90.0000000);
	SetDynamicObjectMaterial(xmas[1], 0, 8567, "vgsebuild02", "ws_tunnelwall2", 0);

	xmas[2] =CreateDynamicObject(19430,1525.3293500,-1364.8584000,46.6117000,0.0000000,0.0000000,90.0000000);
	SetDynamicObjectMaterial(xmas[2], 0, 8678, "wddngchplgrnd01", "vgschurchwall05_128", 0);

	xmas[3] = CreateDynamicObject(19325,1520.8291000,-1358.6717500,40.4553000,0.0000000,0.0000000,90.0000000);

	secretstorage[0] = CreateDynamicObject(1742, 1539.89795, -1354.99585, 311.92120,   0.00000, 0.00000, 48.88000);
	secretstorage[1] = CreateDynamicObject(1742, 1538.94763, -1356.08105, 311.91919,   0.00000, 0.00000, 48.88000);
	secretstorage[2] = CreateDynamicObject(19440, 1539.06030, -1355.94641, 313.61191,   0.00000, 0.00000, -40.98000);
	secretstorage[3] = CreateDynamicObject(19440, 1540.10693, -1354.74292, 313.61389,   0.00000, 0.00000, -40.98000);
	secretdoor = CreateDynamicObject(19440, 1543.56836, -1358.80444, 313.61789,   0.00000, 0.00000, 48.88000);
	knife[0] = CreateDynamicObject(335, 1538.14270, -1349.07190, 312.69495,   0.00000, 90.00000, 43.44000);
	knife[1] = CreateDynamicObject(335, 1538.21948, -1349.21375, 312.72070,   0.00000, 100.00000, 43.44000);
	knife[2] = CreateDynamicObject(335, 1538.28870, -1349.29175, 312.71960,   0.00000, 100.00000, 43.44000);
	knife[3] = CreateDynamicObject(335, 1538.32544, -1349.40173, 312.69818,   0.00000, 90.00000, 43.44000);
	knife[4] = CreateDynamicObject(335, 1538.08618, -1348.90894, 312.74545,   0.00000, 110.00000, 43.44000);
	new towersign1 = CreateDynamicObject(19329, 1524.34729, -1358.74084, 32.12194,   0.00000, 0.00000, 90.00000);
	SetDynamicObjectMaterialText(towersign1, 0, "Lloyd's", 50, "Gill Sans", 30, 1,0xFFF00000,0,1);
	new towersign2 = CreateDynamicObject(19329, 1524.33691, -1355.75195, 32.12194,   0.00000, 0.00000, 90.00000);
	SetDynamicObjectMaterialText(towersign2, 0, "Tower", 50, "Gill Sans", 30, 1,0xFFF00000,0,1);
	towerdoor[0] = CreateDynamicObject(1504, 1547.88977, -1358.90906, 311.87177,   0.00000, 0.00000, 49.44001);
	towerdoor[1] = CreateDynamicObject(1504, 1543.84399, -1358.47437, 323.46375,   0.00000, 0.00000, -41.22001);


	//================ [Desmond DeSantos] ================
	LoadDesmond();
	CreateDynamic3DTextLabel("DeSantos Casino Bar\n((/dsdrink))", COLOR_LIGHTBLUE, 1139.47, -5.51, 1000.67, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 23, 12);
	CreateDynamic3DTextLabel("Sprunk Machine((/dssoda))", COLOR_LIGHTBLUE, 1982, -1998.73, 13.54, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
	DesmondArea = CreateDynamicCircle(1130.80, -3.11, 130.0, 23, 12);
	
    CreateDynamicObject(955, 1983.02686, -1999.80713, 12.92352,   0.00000, 0.00000, 180.00000); // Sprunk machine
    CreateDynamicObject(2942, 1124.53015, 4.01472, 1000.14783,   0.00000, 0.00000, 0.00000);
    
	
    //===============[Kevin Ruthless]=====================
	CreateDynamic3DTextLabel("{FF0000}Ruthless's Safe!\n{FF0000}(/ruthlesssafe)", COLOR_LIGHTBLUE, 1984.2822, -1736.9333, 16.6930, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
    SetTimer("KevRuthStream", 1000, true);


   /* //================ [Dino Cicarelli] ================
    // Added as of Thursday, February 11, 2016 - Dora
    AddStaticVehicleEx(579, 1004.6097, -2130.2266, 13.0265, 175.8602, -1, -1, -1);
    AddStaticVehicleEx(579, 1000.6507, -2130.3005, 13.0265, 179.1001, -1, -1, -1);
    AddStaticVehicleEx(487, 958.3521, -2133.7524, 13.5474, -52.3200, -1, -1, -1);*/


	//================ [Ace Johnson] ================
	AceGate[0] = CreateDynamicObject(7930,316.7000000,-1136.8199000,81.3500000,0.0000000,0.0000000,133.3800000); //garagedoor
	AceGate[1] = CreateDynamicObject(7930,316.7000000,-1136.8199000,81.3500000,0.0000000,180.0000000,133.3800000); //garagedoor
	AceGate[2] = CreateDynamicObject(3061,308.6900000,-1139.8300000,80.8600000,0.0000000,0.0000000,-46.5000000); //door on garage
	AceGate[3] = CreateDynamicObject(1498,302.0600000,-1128.5300000,79.5100000,0.0000000,0.0000000,-135.6600000); //door on garage, backside

	AddStaticVehicleEx(409, 293.0240, -1175.1696, 80.6512, -46.8600, -1, -1, -1); //limo
   	AddStaticVehicleEx(487, 288.1877, -1190.0089, 81.0110, -136.0800, -1, -1, -1); //helicopter



	//============== [Tiger Woods] =================
 	twCircle = CreateDynamicCircle(1477.7290,2853.6323, 55.0);
	blood = TextDrawCreate(306.000000, -2.000000, ".");
	TextDrawAlignment(blood, 2);
	TextDrawBackgroundColor(blood, 255);
	TextDrawFont(blood, 1);
	TextDrawLetterSize(blood, 0.889999, 51.199993);
	TextDrawColor(blood, -1);
	TextDrawSetOutline(blood, 0);
	TextDrawSetProportional(blood, 1);
	TextDrawSetShadow(blood, 1);
	TextDrawUseBox(blood, 1);
	TextDrawBoxColor(blood, -1207959526);
	TextDrawTextSize(blood, 29.000000, 664.000000);

    //    BOOKSHELF TIGER
   	twShelf = CreateDynamicObject(1742,145.973022,1712.458984,10016.094726,0.000000,0.000000,-90.700004); // closed
   	

   	// ============== [Eric Daxono] ================
   	/*CreateDynamic3DTextLabel("Aegis\n{00FF00}Online", 0xB22222FF, 210.3190,1820.3220,8.0141, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 3, 0);
	CreateDynamic3DTextLabel("Solace\n{00FF00}Online", 0xFF82ABFF, 210.3760,1825.2704,8.0141, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 3, 0);
	OutpostMOTD = CreateDynamic3DTextLabel("Eric's Message: Welcome to the Outpost Radio Observatory", 0x9999FFFF, 210.6471,1822.9026,9.4063, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 3, 0);
    CreateDynamic3DTextLabel("Outpost Speaker", 0xB22222FF, -87.4484, 1088.91, 21.0531, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
	SpeakerFrequency  = CreateDynamic3DTextLabel("Outpost Radio Observatory\nTuned Station\n{CCCC00}OFF", 0xFF0000FF, 211.4125,1822.7621,6.4141, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 3, 0);
	
	if(indoorspeaker == true)
		SpeakerStatus = CreateDynamic3DTextLabel("ON", 0x00FF00FF, -87.4484, 1088.91, 20.8031, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);

	else
	    SpeakerStatus = CreateDynamic3DTextLabel("OFF", 0xFF0000FF, -87.4484, 1088.91, 20.8031, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);

	OutpostOutdoorSpeaker = CreateDynamicCircle(-87.4484, 1088.91, 30.0, -1, -1);
	OutpostIndoorSpeaker  = CreateDynamicCircle(212.4125, 1822.7621, 40.0, 3, -1);
	OutpostAegisArea      = CreateDynamicCircle(1254.7643, -804.2196, 30.0, 0, 0);
	
	Load_EricDaxono();*/


	//================ [Nick Fletcher] ===============
	//Load_NickFletcher();
 	//fCasinoText = CreateDynamic3DTextLabel("Fletchers Casino\n{00ff00}Unlocked", COLOR_LIGHTBLUE, 1491.98, -1450.51, 13.53, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
 	
	return 1;
}

new jacobcones[6] = 0, jacobconecount;
stock dOnGameModeExit()
{
    print("[debug] dOnGameModeExit()");
    //===============[ Donor Related ]=====================
	//=================[ Freddie Valdmar ]===========
	
	for( new i; i < sizeof( jacobcones ); i++ ) {
		if( jacobcones[i] != INVALID_OBJECT_ID ) {
			DestroyDynamicObjectEx( jacobcones[i] );
			jacobcones[i] = 0;
		}
	}
	DestroyObject( jacobsiren );
	DestroyVehicle( JacobUtilityTruck );
	DestroyVehicle( JacobTrailer );
	DestroyVehicle( JacobSweeper );

    //================ [ Gunnar Watsone ] ================
    SaveGunnar();

	// ========== [Aydino Mazzerelli] ===============
	SaveAydino();
		
	return 1;
}

stock dOnPlayerConnect(playerid)
{
    printf("[debug] dOnPlayerConnect(%s)", PlayerName(playerid));

    //================ [ Gunnar Watsone ] ================
    gunnarKey[playerid] = 0;
	gunnarOldSkin[playerid] = 0;


	//================ [Aydino Mazzerelli] ================
    aydinokey[playerid] = 0;


	//================ [Desmond DeSantos] ================
	DesmondLoggedIn[playerid] = false;
	SetPVarInt(playerid, "DesmondCameraWatching", -1);
	
	// ============== [Tiger Woods] ================
	inchair[playerid] = false;


	// ================ [Skarr Torino] ===============
	SquadLogged[playerid] = false;
	SquadAdmLogged[playerid] = false;
	SquadCamera[playerid] = false;

	return 1;
}

stock dOnPlayerDisconnect(playerid, reason) //called after OnPlayerSave
{
	printf("[debug] dOnPlayerDisconnect(%s, %d)", PlayerName(playerid), reason);
	
	//============== [Eric Daxono] ========================
	//SpeakerListening[playerid] = SPEAKER_LISTEN_NONE;


	//============== [Nick Fletcher] ========================
	//fCasinoKey[playerid] = false;
	return 1;
}

stock dOnPlayerSpawn(playerid) //wip
{
	printf("[debug] dOnPlayerSpawn(%s)", PlayerName(playerid));
	return 1;
}

stock dOnPlayerDeath(playerid, killerid, reason)
{
    printf("[debug] dOnPlayerDeath(%s, %s, %d)", PlayerName(playerid), PlayerName(killerid), reason);

    /*if(tEChair == playerid)
    {
	 	TextDrawHideForPlayer(tEChair, tFlash[0]);
		TextDrawHideForPlayer(tEChair, tFlash[1]);
		TextDrawHideForPlayer(tEChair, tFlash[2]);
		TextDrawHideForPlayer(tEChair, tFlash[3]);
		tEChair = INVALID_PLAYER_ID;
		
  		if(twTimer) {
			KillTimer(twTimer);
			twTimer = 0;
		}
	}

	inChair[playerid] = false;*/
	
	return 1;
}

stock dOnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	#pragma unused ispassenger, playerid, vehicleid
	return 1;
}

stock dOnPlayerExitVehicle(playerid, vehicleid)
{
	#pragma unused playerid, vehicleid //remove if you use these vars
    //printf("[debug] dOnPlayerExitVehicle(%s, %d)", PlayerName(playerid), vehicleid);
	return 1;
}

stock dOnPlayerPickUpDynamicPickup(playerid, pickupid) //this is called on PickUpPickup at the beginning
{
	//printf("[debug] dOnPlayerPickUpDynamicPickup(%s, %d)", PlayerName(playerid), pickupid);
	
	if(pickupid == WaterPickup)
	{
	    GameTextForPlayer(playerid, "~w~Type ~r~/getwater ~w~to drink some water", 3000, 3);
	    return 1;
	}
	/*if(pickupid == tGC)
	{
		GivePlayerGun(playerid, 2);
		return 1;
	}*/
	if(pickupid == ClothesPickup)
	{
	    if(!InBoxingGear[playerid]) GameTextForPlayer(playerid, "~w~Type ~r~/boxclothes ~w~to change into~n~your boxing clothes", 3000, 3);
	    else GameTextForPlayer(playerid, "~w~Type ~r~/boxclothes ~w~to change into~n~your street clothes", 3000, 3);
	}
	
	if(pickupid == OutfitPickup)
	{
	    if(!InOutfit[playerid])
	    {
		    GameTextForPlayer(playerid, "~w~Type ~r~/outfit ~w~to change into~n~your security uniform", 3000, 3);
		    return 1;
		}
		
		else
	    {
		    GameTextForPlayer(playerid, "~w~Type ~r~/outfit ~w~to change into~n~your street clothes", 3000, 3);
		    return 1;
		}
	}

	if(pickupid == VestPickup) //crash
	{
 		GameTextForPlayer(playerid, (pArmour[playerid] > 0.0) ? ("~w~Type ~r~/lockervest ~w~to get your~n~vest into the locker") : ("~w~Type ~r~/lockervest ~w~to put your~n~vest into the locker"), 3000, 3);
   		return 1;
	}

	if(pickupid == FighterPickup)
	{
        GameTextForPlayer(playerid, "~w~Type ~r~/fightme ~w~to fight other players", 3000, 3);
        return 1;
	}
	if(pickupid == FFAPickup)
	{
        GameTextForPlayer(playerid, "~w~Type ~r~/ffa ~w~to fight in the free for all ring", 3000, 3);
        return 1;
	}
	if(pickupid == CagePickup)
	{
        GameTextForPlayer(playerid, "~w~Type ~r~/cage ~w~to fight in the cage match", 3000, 3);
        return 1;
	}
	return 1;
}

stock dOnPlayerStateChange(playerid, newstate, oldstate)
{
	#pragma unused playerid, newstate, oldstate //remove if you use these vars
	//printf("[debug] dOnPlayerStateChange(%s, %d, %d)", PlayerName(playerid), newstate, oldstate);
	return 1;
}

stock dOnDynamicObjectMoved(objectid)
{
	//=============== [Skarr Torino] ===============
	if(objectid == SquadElevator)
	{
        SquadElevatorState = SquadElevatorMoving;
	  	SquadElevatorMoving = (SquadElevatorMoving == true) ? (false) : (true);
	  	
	  	if(SquadElevatorState != false)
	  	{
	  	    MoveDynamicObject(SquadElevatorDoor1, -945.949340, -55.701606, 1006.076171, 5.0);
	  	    MoveDynamicObject(SquadElevatorDoor2, -940.425842, -55.795616, 1006.076049, 5.0);
		}
		
		else
		{
		    MoveDynamicObject(SquadElevatorDoor3, -946.029602, -55.722320, 1001.216247, 5.0);
		    MoveDynamicObject(SquadElevatorDoor4, -939.736389, -55.684394, 1001.225952, 5.0);
		}
	}
	
	else if(objectid == SquadDoor)
	{
	    if(SquadDoorMoving == true)
	    {
	        SquadDoorState = true;
		}
		else
		{
		    SquadDoorState = false;
		}
	}
		
	return 1;
}

stock dOnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	#pragma unused playerid, newkeys, oldkeys
	//printf("[debug] dOnPlayerStateChange(%s, %d, %d)", PlayerName(playerid), newkeys, oldkeys);
	/*if(newkeys & KEY_YES)
	{
		if(IsPlayerInRangeOfPoint(playerid, 3.0, 145.6084, 1711.9868, 10016.0996))
		{
			if(twperm[playerid] == 1)
			{
				format(donorstr, sizeof(donorstr), "%s searches the books shelf for his favorite book.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				if(tbss == false)
				{
					MoveDynamicObject(tbs, 146.7643, 1711.3840, 10016.0996, 2.0, 0.0000, 0.0000, 180.0000);
					tbss = true;
				}
				else if(tbss == true)
				{
					MoveDynamicObject(tbs, 146.2070, 1712.4497, 10016.0996, 2.0,  0.00000, 0.00000, 270.00000);
					tbss = false;
				}
			}
		}
		else if(IsPlayerInRangeOfPoint(playerid, 2.0, 144.1367, 1721.3069, 10017.6279) && twTimC == false)
		{
		    if(IsPlayerConnected(tEChair))
		    {
	    		ApplyAnimation(playerid, "HEIST9", "Use_SwipeCard", 10.0, 0, 0, 0, 0, 0);
				twTimer = SetTimerEx("WoodsFlashFunc", 1000, false, "i", tEChair);

				new Float: health = 100.0;
				GetPlayerHealth(playerid, health);
				SetPlayerHealth(playerid, health-20);

				twTimC = true;
				format(donorstr, sizeof(donorstr), "%s presses the switch.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
		}
		else if(IsPlayerInRangeOfPoint(playerid, 2.0, 145.7216, 1723.7817, 10016.6826))
		{
			ApplyAnimation(playerid, "PED","SEAT_IDLE", 10.0, 0, 0, 0, 0, 0);
			tEChair = playerid;
		}
	}*/
	return 1;
}

stock dOnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{	
	if(dialogid == DIALOG_DONOR_DESMOND)
	{
	    if(!response) return SendClientMessage(playerid, COLOR_GREY, "You have closed the drink menu.");

	    if(GetPVarInt(playerid, "DesmondEditing") == 1)
	    {
	        SetPVarInt(playerid, "DesmondEditDrink", listitem);

			format(donorstr, sizeof donorstr, "You are now editing %s. The price of this drink is currently $%i.", DesmondDrinks[listitem][DesmondDrinkName], DesmondPrices[listitem]);
			ShowPlayerDialog(playerid, DIALOG_DONOR_DESMOND_EDIT, DIALOG_STYLE_INPUT, "Casino Drink - Edit Price", donorstr, "Edit", "Close");
		}
	    else if(listitem >= 0 && listitem <= sizeof DesmondDrinks)
		{
		    if(DesmondPrices[listitem] <= 0)
		    {
		        format(donorstr, sizeof donorstr, "%s is not currently available.", DesmondDrinks[listitem][DesmondDrinkAction]);
		        SendClientMessage(playerid, COLOR_GREY, donorstr);
			}
			
	        else
			{
				SetPlayerSpecialAction(playerid, DesmondDrinks[listitem][DesmondDrinkAction]);
				GiveMoney(playerid, -DesmondPrices[listitem]);
	        	DesmondSafe[DESMOND_SAFE_MONEY] += DesmondPrices[listitem];
	        	SaveDesmond(2); // save the safe
	        
	        	format(donorstr, sizeof donorstr, "You have purchased a %s for $%i.", DesmondDrinks[listitem][DesmondDrinkName], DesmondPrices[listitem]);
	        	SendClientMessage(playerid, COLOR_LIGHTBLUE, donorstr);
			}
		}
	}
	
	else if(dialogid == DIALOG_DONOR_DESMOND_EDIT)
	{
		listitem = GetPVarInt(playerid, "DesmondEditDrink");
		if(strval(inputtext) < 0 || strval(inputtext) >= 1000000)
		{
			format(donorstr, sizeof donorstr, "You are now editing %s. The price of this drink is currently $%i.", DesmondDrinks[listitem][DesmondDrinkName], DesmondPrices[listitem]);
			ShowPlayerDialog(playerid, DIALOG_DONOR_DESMOND_EDIT, DIALOG_STYLE_INPUT, "Casino Drink - Edit Price", donorstr, "Edit", "Close");
		}
		
		else
		{
			DesmondPrices[listitem] = strval(inputtext);
			format(donorstr, sizeof donorstr, "You have changed the price of %s to $%i.", DesmondDrinks[listitem][DesmondDrinkName], strval(inputtext));
			SendClientMessage(playerid, COLOR_WHITE, donorstr);
			SaveDesmond(1); // save the prices
			
			DeletePVar(playerid, "DesmondEditing");
			DeletePVar(playerid, "DesmondEditDrink");
		}
	}
	return 1;
}

stock dOnPlayerTakeDamage(playerid, issuerid, Float: amount, weaponid, bodypart)
{
	#pragma unused weaponid, bodypart //remove if you use these vars
	//printf("[debug] dOnPlayerTakeDamage(%s, %d, %f, %d, %d)", PlayerName(playerid), issuerid, amount, weaponid, bodypart);
	if(issuerid == INVALID_PLAYER_ID || playerid == issuerid) return 1;

	//================== [Skarr Torino] ==================
	if(SquadCamera[playerid] == true)
	{
		SendClientMessage(playerid, COLOR_GREEN, "(( You have been forced out of camera mode due to taking damage. ))");
		SetCameraBehindPlayer(playerid);
	    SquadCamera[playerid] = false;

	    TogglePlayerControllable(playerid, 1);
	    DeletePVar(playerid, "PlayerFrozen");
	}
	return 1;
}
stock hook_DestroyVehicle( vehicleid ) {
	if( vehicleid == JacobUtilityTruck ) {
		for( new i; i < sizeof( jacobcones ); i++ ) {
			if( jacobcones[i] != INVALID_OBJECT_ID ) {
				DestroyDynamicObjectEx( jacobcones[i] );
				jacobcones[i] = 0;
			}
		}
		DestroyObject( jacobsiren );
	}
	
	DestroyVehicle( vehicleid );
	return 1;
}
#if defined _ALS_DestroyVehicle
    #undef DestroyVehicle
#else
    #define _ALS_DestroyVehicle
#endif
#define DestroyVehicle hook_DestroyVehicle


//=================[ Freddie Valdemar ]============

CMD:jmfredhelp( playerid, params[] ) {
	return SendClientMessage( playerid, COLOR_GREY, "FRED'S HELP: /freds, /jmfredplaceroaditem, /jmfredclearroaditems, /fredlock" );
}
loadjmtruckitems() {
	jacobobjects[0] = CreateObject(2661,0,0,-1000,0,0,0,100);
	AttachObjectToVehicle(jacobobjects[0], JacobUtilityTruck, -0.014999,-3.060001,0.369999,0.000000,0.000000,360.000305);
	SetObjectMaterialText(jacobobjects[0], "FRED'S\nSANITARY\n555-(3387)", 0, OBJECT_MATERIAL_SIZE_256x128, "Tahoma", 40, 1, 0xFFB5B5B5, 0xFF9D2915, OBJECT_MATERIAL_TEXT_ALIGN_CENTER);
	

	jacobobjects[1] = CreateObject(2719,0,0,-1000,0,0,0,100);
	SetObjectMaterialText(jacobobjects[1], "FRED'S\nSANITARY\n555-(3387)", 0, OBJECT_MATERIAL_SIZE_256x128, "Tahoma", 40, 1, 0xFFB5B5B5, 0xFF9D2915, OBJECT_MATERIAL_TEXT_ALIGN_CENTER);
	AttachObjectToVehicle(jacobobjects[1], JacobUtilityTruck, -1.285000,-0.275000,0.294999,0.000000,0.000000,-90.0);

	jacobobjects[2] = CreateObject(2719,0,0,-1000,0,0,0,100);
	SetObjectMaterialText(jacobobjects[2], "FRED'S\nSANITARY\n555-(3387)", 0, OBJECT_MATERIAL_SIZE_256x128, "Tahoma", 40, 1, 0xFFB5B5B5, 0xFF9D2915, OBJECT_MATERIAL_TEXT_ALIGN_CENTER);
	AttachObjectToVehicle(jacobobjects[2], JacobUtilityTruck, 1.299999,-0.320000,0.275000,0.000000,0.000000,90.0);

	jacobsiren = CreateObject(11702,0,0,-1000,0,0,0,100);
	AttachObjectToVehicle(jacobsiren, JacobUtilityTruck, 0.000000,0.674999,1.350000,0.000000,0.000000,0.000000);
	return 1;
}
CMD:jmreloadtruckitems( playerid, params[] ) {
	for( new i; i < sizeof( jacobobjects ); i++ ) {
		DestroyObject( jacobobjects[i] );
	}
	DestroyObject( jacobsiren );
	loadjmtruckitems();
	return 1;
}
new jacobsirentoggle;
CMD:freds( playerid, params[] ) {
	if( GetPlayerVehicleID( playerid ) == JacobUtilityTruck ) {
		if( jacobsirentoggle ) {
			DestroyObject( jacobsiren );
			jacobsiren = CreateObject(11702,0,0,-1000,0,0,0,100);
			AttachObjectToVehicle(jacobsiren, JacobUtilityTruck, 0.000000,0.674999,1.350000,0.000000,0.000000,0.000000);
		} else {
			DestroyObject( jacobsiren );
			jacobsiren = CreateObject(11701,0,0,-1000,0,0,0,100);
			AttachObjectToVehicle(jacobsiren, JacobUtilityTruck, 0.000000,0.674999,1.350000,0.000000,0.000000,0.000000);
		}
		jacobsirentoggle = !jacobsirentoggle;
	}
	return 1;
}


CMD:jmfredplaceroaditem( playerid, params[] ) {
	if( jacobconecount >= 5 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You do not have anymore road items to place." );
	}

	new type, text[128];
	if( sscanf( params, "i", type ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /jmfredplaceroaditem [type (0:cone, 1:barrier)]" );
	}
	
	new Float: vpos[3];
	GetVehiclePos( JacobUtilityTruck, vpos[0], vpos[1], vpos[2] );
	if( !IsPlayerInRangeOfPoint( playerid, 20.0, vpos[0], vpos[1], vpos[2] ) ) {
		return SendClientMessage( playerid, COLOR_GREY, "You are not in range of the Utility Truck." );
		
	}
	
	new Float: ppos[4];
	GetPlayerPos( playerid, ppos[0], ppos[1], ppos[2] );
	GetPlayerFacingAngle( playerid, ppos[3] );
	for( new i; i < sizeof( jacobcones ); i++ ) {
		if( jacobcones[i] == 0 ) {
			jacobcones[i] = CreateDynamicObject( (type == 0) ? (1238) : (1459), ppos[0], ppos[1], ppos[2]-0.65, 0.0, 0.0, ppos[3] );
			format( text, sizeof( text ), "* %s places a road item on the ground by their feet.", PlayerICName(playerid) );
			ProxDetector( 30.0, playerid, text, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE );
			jacobconecount++;
			break;
		}
	}
	return 1;
}

CMD:jmfredclearroaditems( playerid, params[] ) {
	new Float:vpos[3];
	GetVehiclePos( JacobUtilityTruck, vpos[0], vpos[1], vpos[2] );
	if( !IsPlayerInRangeOfPoint( playerid, 20.0, vpos[0], vpos[1], vpos[2] ) && !PlayerInfo[playerid][pAdmin] ) {
		return SendClientMessage( playerid, COLOR_GREY, "You are not by the Utility Truck." ); 
	}
	for( new i; i < sizeof( jacobcones ); i++ ) {
		if( jacobcones[i] != 0 ) {
			DestroyDynamicObjectEx( jacobcones[i] );
			jacobcones[i] = 0;
		}
	}
	jacobconecount = 0;
	SendClientMessage( playerid, COLOR_GREY, "You pick up all of the road items." );
	return 1;
}

CMD:fredlock( playerid, params[] ) {
	new Float:vpos[3], text[128];
	GetVehiclePos( JacobUtilityTruck, vpos[0], vpos[1], vpos[2] );
	if( IsPlayerInRangeOfPoint( playerid, 5.0, vpos[0], vpos[1], vpos[2] )) {
		SetVehicleLockState( JacobUtilityTruck, !gCarLock[JacobUtilityTruck] );
		format( text, sizeof( text ), "* %s %s their vehicle.", PlayerICName(playerid), ( gCarLock[JacobUtilityTruck] == 1 ) ? ("locks") : ("unlocks") );
		ProxDetector( 30.0, playerid, text, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE );
	}
	else {
		GetVehiclePos( JacobSweeper, vpos[0], vpos[1], vpos[2] );
		if( IsPlayerInRangeOfPoint( playerid, 5.0, vpos[0], vpos[1], vpos[2] ) ) {
			SetVehicleLockState( JacobSweeper, !gCarLock[JacobSweeper] );
			format( text, sizeof( text ), "* %s %s their vehicle.", PlayerICName(playerid), ( gCarLock[JacobSweeper] == 1 ) ? ("locks") : ("unlocks") );
			ProxDetector( 30.0, playerid, text, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE );
			return 1;
		}
	}
	return 1;
}


//================ [Jacob Malcom] ================
/*          UNKNOWN DEVELOPMENT RERQUEST          */

CMD:jacobmalcomcmds(playerid,params[])
{
	SendClientMessage(playerid, -1, "----------[ Jacob Malcom's CMDs ]----------");
	SendClientMessage(playerid, 0xAFAFAFAA, "> /jboxville, enter/exit Boxville.");
	SendClientMessage(playerid, 0xAFAFAFAA, "> /jbc, Boxville vehicle chat (with passengers/driver).");
	return 1;
}

CMD:jboxville(playerid, params[]) //Entering/exiting the boxville
{
	if(IsPlayerInRangeOfPoint(playerid, 2.0, -2302.9670,-106.8171,999.8542))
	{
	    new
			Float:jacfacvpos[4];
	    GetVehiclePos(jacob_factoryveh, jacfacvpos[0], jacfacvpos[1], jacfacvpos[2]);
	    GetVehicleZAngle(jacob_factoryveh, jacfacvpos[3]);
	    new
			Float:jacfacvinfo[3];
	    GetVehicleModelInfo(GetVehicleModel(jacob_factoryveh), VEHICLE_MODEL_INFO_SIZE, jacfacvinfo[0], jacfacvinfo[1], jacfacvinfo[2]);
	    new
			Float:distance = (jacfacvinfo[1]/2 + 0.5);
     	jacfacvpos[0] += (distance * -floatsin(-jacfacvpos[3], degrees));
    	jacfacvpos[1] += (distance * -floatcos(-jacfacvpos[3], degrees));
    	SetPlayerPos(playerid, jacfacvpos[0], jacfacvpos[1], jacfacvpos[2]);
    	SetPlayerInterior(playerid, 0);
	}
	else
	{
		new
			Float:jacfacvpos[4];
	    GetVehiclePos(jacob_factoryveh, jacfacvpos[0], jacfacvpos[1], jacfacvpos[2]);
	    GetVehicleZAngle(jacob_factoryveh, jacfacvpos[3]);
	    new
			Float:jacfacvinfo[3];
	    GetVehicleModelInfo(GetVehicleModel(jacob_factoryveh), VEHICLE_MODEL_INFO_SIZE, jacfacvinfo[0], jacfacvinfo[1], jacfacvinfo[2]);
	    new
			Float:distance = (jacfacvinfo[1]/2 + 0.5);
     	jacfacvpos[0] += (distance * -floatsin(-jacfacvpos[3], degrees));
    	jacfacvpos[1] += (distance * -floatcos(-jacfacvpos[3], degrees));
		if(IsPlayerInRangeOfPoint(playerid, 1.5, jacfacvpos[0], jacfacvpos[1], jacfacvpos[2]))
    	{

    	    SetPlayerPos(playerid, -2302.9670,-106.8171,999.8542);
    	    SetPlayerFacingAngle(playerid, 180);
    		SetPlayerInterior(playerid, 0);
		}
	}
	return 1;
}

CMD:jbc(playerid, params[]) //For talking to people driving/inside the Boxville.
{
	new
 	   	jac_drivertext[128 - MAX_PLAYER_NAME];
	if (sscanf(params, "s[128]", jac_drivertext)) return 1;
	if(GetPlayerVehicleID(playerid) == jacob_factoryveh || IsPlayerInRangeOfPoint(playerid, 10.0, -2302.9670,-106.8171,999.8542))
	{
		new
			jac_name[MAX_PLAYER_NAME];
 		GetPlayerName(playerid, jac_name, sizeof(jac_name));
    	strmid(jac_name, str_replace('_', ' ', jac_name), 0, MAX_PLAYER_NAME);
	    foreach(new i: Player)
	    {
        	if(IsPlayerInRangeOfPoint(i, 10.0, -2302.9670,-106.8171,999.8542) || GetPlayerVehicleID(i) == jacob_factoryveh)
        	{
        		format(donorstr, sizeof(donorstr), "[Boxville] %s says: %s", jac_name, jac_drivertext);
        		SendClientMessage(i, -1, donorstr);
			}
	    }
	}
	return 1;
}

//================ [Devin Gideon] ================
/*	http://www.gta-sarp.com/forums/showthread.php?182818-Development-Request-Devin-Gideon-s-Request */
CMD:dboxville(playerid, params[]) //Entering/exiting the boxville (Credits to Jacob Malcom)
{
	if(GetPlayerVirtualWorld(playerid) == 1 && IsPlayerInRangeOfPoint(playerid, 2.0, -2302.9670, -106.8171, 999.8542))
	{
	    new
			Float:jacfacvpos[4];
	    GetVehiclePos(devin_boxville, jacfacvpos[0], jacfacvpos[1], jacfacvpos[2]);
	    GetVehicleZAngle(devin_boxville, jacfacvpos[3]);
	    new
			Float:jacfacvinfo[3];
	    GetVehicleModelInfo(GetVehicleModel(devin_boxville), VEHICLE_MODEL_INFO_SIZE, jacfacvinfo[0], jacfacvinfo[1], jacfacvinfo[2]);
	    new
			Float:distance = (jacfacvinfo[1]/2 + 0.5);
     	jacfacvpos[0] += (distance * -floatsin(-jacfacvpos[3], degrees));
    	jacfacvpos[1] += (distance * -floatcos(-jacfacvpos[3], degrees));
    	SetPlayerPos(playerid, jacfacvpos[0], jacfacvpos[1], jacfacvpos[2]);
    	SetPlayerInterior(playerid, 0);
    	SetPlayerVirtualWorld(playerid, 0);
    	PlayerInfo[playerid][pVirtualWorld] = 0;
	}
	else
	{
		new
			Float:jacfacvpos[4];
	    GetVehiclePos(devin_boxville, jacfacvpos[0], jacfacvpos[1], jacfacvpos[2]);
	    GetVehicleZAngle(devin_boxville, jacfacvpos[3]);
	    new
			Float:jacfacvinfo[3];
	    GetVehicleModelInfo(GetVehicleModel(devin_boxville), VEHICLE_MODEL_INFO_SIZE, jacfacvinfo[0], jacfacvinfo[1], jacfacvinfo[2]);
	    new
			Float:distance = (jacfacvinfo[1]/2 + 0.5);
     	jacfacvpos[0] += (distance * -floatsin(-jacfacvpos[3], degrees));
    	jacfacvpos[1] += (distance * -floatcos(-jacfacvpos[3], degrees));
		if(IsPlayerInRangeOfPoint(playerid, 1.5, jacfacvpos[0], jacfacvpos[1], jacfacvpos[2]))
    	{
    	    SetPlayerPos(playerid, -2302.9670,-106.8171,999.8542);
    	    SetPlayerFacingAngle(playerid, 180);
    		SetPlayerInterior(playerid, 0);
    		SetPlayerVirtualWorld(playerid, 1);
    		PlayerInfo[playerid][pVirtualWorld] = 1;
		}
	}
	return 1;
}
//================ [Keven Ruthless] ================
/*          UNKNOWN DEVELOPMENT RERQUEST          */
CMD:ruthless(playerid, params[])
{
	if(CP[playerid] != CHECKPOINT_NONE) return SendClientMessage(playerid, -1, "You already have a checkpoint set, use (/killcp) to kill it.");
	if(isnull(params)) return SendClientMessage(playerid, -1, "/ruthless [House | HQ]");
    if(!strcmp(params, "House", true, 7))
    {
	    SendClientMessage(playerid, -1, "Ruthless House has been marked on your map.");
		SetPlayerCheckpointEx(playerid, 1972.5854,-1722.2434,16.5636, 3.0);
		CP[playerid] = CHECKPOINT_RUTHLESS;
	}
	else if(!strcmp(params, "HQ", true, 7))
    {
    	SendClientMessage(playerid, -1, "Ruthless HQ has been marked on your map.");
		SetPlayerCheckpointEx(playerid, 1982.6362,-1682.8397,16.9317, 3.0);
		CP[playerid] = CHECKPOINT_RUTHLESS;
	}
	return 1;
}


CMD:krstream(playerid, params[])
{
	if(IsPlayerInRangeOfPoint(playerid, 2.0, 1968.04236, -1693.60791, 8023.81055))
	{
		if(isnull(params)) return SendClientMessage(playerid, -1, "/krstream [link]");
		format(kevruth, sizeof(kevruth), "%s", params);
		foreach(new i: Player)
		{
			KevListen[i] = false;
		}
	}
	return 1;
}

forward KevRuthStream();
public KevRuthStream()
{
	foreach(new i: Player)
	{
		if(IsPlayerInRangeOfPoint(i, 30, 1968.04236, -1693.60791, 8023.81055) && KevListen[i] == false)
		{
			PlayAudioStreamForPlayer(i, kevruth);
			KevListen[i] = true;
		}
	}
	return 1;
}

//================ [Desmond DeSantos] ================
CMD:dssoda(playerid, params[])
{
	if(IsPlayerInRangeOfPoint(playerid, 3.0, 1983.1195, -1999.4825, 13.5469) && GetPlayerVirtualWorld(playerid) == 0)
	{
	    if(PlayerInfo[playerid][pCash] < 5) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough money to purchase a sprunk can.");
		if(PlayerTazed[playerid] || PlayerCuffed[playerid] || PlayerFrozen[playerid]) return SendClientMessage(playerid, COLOR_GREY, "You cannot do this right now.");
		
		GiveMoney(playerid, -5);
		DesmondSafe[DESMOND_SAFE_MONEY] += 5;
		
		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);
		SendClientMessage(playerid, COLOR_WHITE, "You have purchased a sprunk for $5.");
	}
	
	else SendClientMessage(playerid, COLOR_GREY, "You are not near the sprunk machine.");
	return 1;
}

CMD:dsdrink(playerid, params[])
{
	if(PlayerTazed[playerid] || PlayerCuffed[playerid] || PlayerFrozen[playerid]) return SendClientMessage(playerid, COLOR_GREY, "You can't do this right now.");
	if(IsPlayerInRangeOfPoint(playerid, 3.0, 1139.47, -5.51, 1000.67) && GetPlayerVirtualWorld(playerid) == 23)
	{
		format(donorstr, sizeof donorstr, "%s($%i)\n%s($%i)\n%s($%i)\n%s($%i)", DesmondDrinks[0][DesmondDrinkName], DesmondPrices[0], DesmondDrinks[1][DesmondDrinkName], DesmondPrices[1],
		DesmondDrinks[2][DesmondDrinkName], DesmondPrices[2], DesmondDrinks[3][DesmondDrinkName], DesmondPrices[3]);
		ShowPlayerDialog(playerid, DIALOG_DONOR_DESMOND, DIALOG_STYLE_LIST, "Casino Drinks", donorstr, "Buy", "Close");
		
		if(GetPVarInt(playerid, "DesmondEditing") != 0)
		{
		    DeletePVar(playerid, "DesmondEditing");
		    DeletePVar(playerid, "DesmondEditDrink");
		}
	}
	
	else SendClientMessage(playerid, COLOR_GREY, "You are not near the bar!");
	return 1;
}

CMD:dscasino(playerid, params[])
{
	if(CP[playerid] != 0 && CP[playerid] != CHECKPOINT_DESMOND) return SendClientMessage(playerid, COLOR_GREY, "You already have an active checkpoint, /killcp.");

	DisablePlayerCheckpointEx(playerid);
	SetPlayerCheckpointEx(playerid, 1985.85, -1999.56, 13.54, 3.0);
	CP[playerid] = CHECKPOINT_DESMOND;
	
	SendClientMessage(playerid, COLOR_GREY, "You have set a checkpoint to the casino.");
	return 1;
}

CMD:dslogin(playerid, params[])
{
	if(PlayerInfo[playerid][pID] != 59148 && isnull(params)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dslogin [password]");
	if(PlayerInfo[playerid][pID] == 59148 || strmatch(params, DesmondPassword) || PlayerInfo[playerid][pAdmin] == 99999)
	{
	    if(DesmondLoggedIn[playerid] == true) return SendClientMessage(playerid, COLOR_GREY, "You are already logged in.");
	    DesmondLoggedIn[playerid] = true;
		SendClientMessage(playerid, COLOR_YELLOW, "You have logged in to the casino system.");
	}
	else SendClientMessage(playerid, -1, "Invalid password.");
	return 1;
}

CMD:dslogout(playerid, params[])
{
	if(DesmondLoggedIn[playerid] == true)
	{
	    DesmondLoggedIn[playerid] = false;
	    SendClientMessage(playerid, COLOR_YELLOW, "You have logged out of the casino system.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not allowed to use this command.");
}

CMD:dschangepass(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 59148 || DesmondLoggedIn[playerid] == true)
	{
		if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dschangepass [new password]");
		if(strlen(params) >= 2 && strlen(params) <= 64)
		{
		    strdel(DesmondPassword, 0, strlen(DesmondPassword));
		    strins(DesmondPassword, params, 0, strlen(params));

			format(donorstr, sizeof donorstr, "You have changed the casino password to: %s", params);
			SendClientMessage(playerid, COLOR_WHITE, donorstr);
			
			SaveDesmond(3); // save the password only
		}
		else SendClientMessage(playerid, COLOR_GREY, "Password length must be more than 2 and below 65.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not allowed to use this.");
	return 1;
}

CMD:dsofficelock(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 59148 || DesmondLoggedIn[playerid] == true)
	{
	    if(IsPlayerInRangeOfPoint(playerid, 3.5, 1143.2092, 1.6622, 1000.6797) || IsPlayerInRangeOfPoint(playerid, 3.5, 1153.4292, -2.7271, 997.6765))
	    {
			if(DesmondOfficeLocked == true)
			{
			    DesmondOfficeLocked = false;
			    SendClientMessage(playerid, COLOR_GREY, "You have unlocked the casino office.");
			}
			else
			{
			    DesmondOfficeLocked = true;
			    SendClientMessage(playerid, COLOR_GREY, "You have locked the casino office.");
			}
			
			SaveDesmond(4);
		}
		else SendClientMessage(playerid, COLOR_GREY, "You are not near the office.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You do not have the key.");
}

CMD:dslock(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 59148 || DesmondLoggedIn[playerid] == true)
	{
	    if(DesmondLocked == true)
	    {
	        DesmondLocked = false;
			format(donorstr, sizeof donorstr, "DeSantos Casino\n{00FF00}OPEN\n{FFFFFF}Entry Fee: $%i", DesmondEntryFee);
            UpdateDynamic3DTextLabelText(DesmondEntrance, COLOR_LIGHTBLUE, donorstr);
		}
		else
		{
		    DesmondLocked = true;
            UpdateDynamic3DTextLabelText(DesmondEntrance, COLOR_LIGHTBLUE, "DeSantos Casino\n{FF0000}CLOSED");
		}
		
		SaveDesmond(4);
	}
	else SendClientMessage(playerid, COLOR_GREY, "You do not have the key!");
	return 1;
}

CMD:dsprices(playerid, params[])
{
    if(PlayerInfo[playerid][pID] == 59148 || DesmondLoggedIn[playerid] == true)
	{
	    format(donorstr, sizeof donorstr, "%s($%i)\n%s($%i)\n%s($%i)\n%s($%i)", DesmondDrinks[0][DesmondDrinkName], DesmondPrices[0], DesmondDrinks[1][DesmondDrinkName], DesmondPrices[1],
		DesmondDrinks[2][DesmondDrinkName], DesmondPrices[2], DesmondDrinks[3][DesmondDrinkName], DesmondPrices[3]);
		ShowPlayerDialog(playerid, DIALOG_DONOR_DESMOND, DIALOG_STYLE_LIST, "Casino Drinks - Edit", donorstr, "Edit", "Close");

		SetPVarInt(playerid, "DesmondEditing", 1);
	}
	else SendClientMessage(playerid, COLOR_GREY, "You do not have the key.");
}

CMD:dsbroadcast(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 59148 || DesmondLoggedIn[playerid] == true)
	{
		if(!IsPlayerInDynamicArea(playerid, DesmondArea)) return SendClientMessage(playerid, COLOR_GREY, "You are not in the Casino.");
	    if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dsbroadcast [text]");
	    if(strlen(params) >= 2 || strlen(params) <= 100)
	    {
	        format(donorstr, sizeof donorstr, "[Microphone - %s]: %s", PlayerICName(playerid), params);
			ProxDetector(30.0, playerid, donorstr, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW);
		}
		else SendClientMessage(playerid, COLOR_GREY, "The text length cannot be less than 2 or more than 100.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You do not have the key.");
	return 1;
}

CMD:dsentryfee(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 59148 || DesmondLoggedIn[playerid] == true)
	{
		if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dsentryfee [fee amount]");
		if(strval(params) <= 0 || strval(params) >= 100000) return SendClientMessage(playerid, COLOR_GREY, "Invalid fee amount.");
		
		DesmondEntryFee = strval(params);
		SaveDesmond(4);
		if(DesmondLocked == false)
		{
		    format(donorstr, sizeof donorstr, "DeSantos Casino\n{00FF00}OPEN\n{FFFFFF}Entry Fee: $%i", DesmondEntryFee);
            UpdateDynamic3DTextLabelText(DesmondEntrance, COLOR_WHITE, donorstr);
		}

		// We don't need to update it if it's closed because the entry fee doesn't show anyways.
		format(donorstr, sizeof donorstr, "You have changed the casino update fee to $%i.", DesmondEntryFee);
		SendClientMessage(playerid, COLOR_YELLOW, donorstr);
	}
	else SendClientMessage(playerid, COLOR_GREY, "You do not have the key.");
	return 1;
}


CMD:dssafe(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 59148 || DesmondLoggedIn[playerid] == true)
	{
	    if(GetPlayerVirtualWorld(playerid) != 23 || !IsPlayerInRangeOfPoint(playerid, 5.0, 1159.3403, 5.5914, 996.8517)) return SendClientMessage(playerid, COLOR_GREY, "You are not near the safe.");
	    if(IsAtEvent[playerid] || inPaintball[playerid] || PlayerTazed[playerid] || PlayerCuffed[playerid]) return SendClientMessage(playerid, COLOR_GREY, "You cannot do this right now.");
		new option[10], option2[12], amount;
		if(sscanf(params, "s[10]s[12]D", option, option2, amount)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe [take/put] [money/weapon/crack/pot]");
		
		if(strmatch(option2, "weapon") == 1)
		{
		    if(strmatch(option, "put"))
		    {
		        if(sscanf(params, "s[120]s[12]d", option, option2, amount)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe [put] [weapon] [slot]");
		        if(amount < 0 || amount > 9) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe put weapon [0-9]");
		        if(DesmondSafeWeapons[amount] != 0) return SendClientMessage(playerid, COLOR_GREY, "You already have a weapon stored.");
		        if(GetPlayerWeapon(playerid) == 0) return SendClientMessage(playerid, COLOR_GREY, "You are not holding a weapon.");
		        if(HaveAdminWeapon(playerid, GetPlayerWeapon(playerid))  == GetPlayerWeapon(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can not store admin weapons.");

		        TakePlayerWeapon(playerid, GetPlayerWeapon(playerid));
				DesmondSafeWeapons[amount] = GetPlayerWeapon(playerid);

    			format(donorstr, sizeof donorstr, "* %s places a %s in the safe.", PlayerICName(playerid), dGetWeaponName(GetPlayerWeapon(playerid)));
				ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			
			else if(strmatch(option, "take"))
			{
                if(sscanf(params, "s[120]s[12]d", option, option2, amount)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe [take] [weapon] [slot]");
		        if(amount < 0 || amount > 9) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe take weapon [0-9]");
			    if(PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GREY, "You must play for 2 hours before using a weapon.");
			    if(DesmondSafeWeapons[amount] == 0) return SendClientMessage(playerid, COLOR_GREY, "You do not have a weapon stored in the safe.");

                format(donorstr, sizeof donorstr, "* %s takes a %s from the safe.", PlayerICName(playerid), dGetWeaponName(DesmondSafeWeapons[amount]));
				ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				
				GivePlayerGun(playerid, DesmondSafeWeapons[amount]);
				DesmondSafeWeapons[amount] = 0;
			}
		}
		
		else if(strmatch(option2, "money") == 1)
		{
		    if(strmatch(option, "put"))
		    {
    			if(sscanf(params, "s[10]s[12]d", option, option2, amount)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe put money [amount]");
				if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
				if(PlayerInfo[playerid][pCash] < amount) return SendClientMessage(playerid, COLOR_GREY, "You do not have that much cash on you.");
				
				GiveMoney(playerid, -amount);
				DesmondSafe[DESMOND_SAFE_MONEY] += amount;
				
				format(donorstr, sizeof donorstr, "* %s puts some money in the safe.", PlayerICName(playerid));
				ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			
			else if(strmatch(option, "take"))
			{
			    if(sscanf(params, "s[10]s[12]d", option, option2, amount)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe take money [amount]");
		        if(DesmondSafe[DESMOND_SAFE_MONEY] < amount) return SendClientMessage(playerid, COLOR_GREY, "You do not have that much in your safe.");
		        if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
		        
				GiveMoney(playerid, amount);
				DesmondSafe[DESMOND_SAFE_MONEY] -= amount;
				
				format(donorstr, sizeof donorstr, "* %s takes some money from the safe.", PlayerICName(playerid));
				ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
		}
		
		else if(strmatch(option2, "pot") == 1)
		{
		    if(strmatch(option, "put"))
		    {
		        if(sscanf(params, "s[10]s[12]d", option, option2, amount)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe put pot [amount]");
				if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
				if(PlayerInfo[playerid][pPot] < amount) return SendClientMessage(playerid, COLOR_GREY, "You do not have that much pot.");
				
				if(PlayerInfo[playerid][pPot]+DesmondSafe[DESMOND_SAFE_POT] >= 10000) return SendClientMessage(playerid, COLOR_GREY, "Your safe cannot store that much pot.");
				PlayerInfo[playerid][pPot] -= amount;
				DesmondSafe[DESMOND_SAFE_POT] += amount;
				
				format(donorstr, sizeof donorstr, "* %s puts some pot in the safe.", PlayerICName(playerid));
				ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			
			else if(strmatch(option, "take"))
			{
			    if(sscanf(params, "s[10]s[12]d", option, option2, amount)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe take pot [amount]");
				if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
				if(DesmondSafe[DESMOND_SAFE_POT] < amount) return SendClientMessage(playerid, COLOR_GREY, "Your safe doesn't have that much pot stored.");
				
				PlayerInfo[playerid][pPot] += amount;
				DesmondSafe[DESMOND_SAFE_POT] -= amount;
				
				format(donorstr, sizeof donorstr, "* %s takes some pot from the safe.", PlayerICName(playerid));
				ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
		}
		
        else if(strmatch(option2, "crack") == 1)
		{
		    if(strmatch(option, "put"))
		    {
		        if(sscanf(params, "s[10]s[12]d", option, option2, amount)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe put crack [amount]");
				if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
				if(PlayerInfo[playerid][pCrack] < amount) return SendClientMessage(playerid, COLOR_GREY, "You do not have that much crack.");

				if(PlayerInfo[playerid][pCrack]+DesmondSafe[DESMOND_SAFE_CRACK] >= 10000) return SendClientMessage(playerid, COLOR_GREY, "Your safe cannot store that much crack.");
				PlayerInfo[playerid][pCrack] -= amount;
				DesmondSafe[DESMOND_SAFE_CRACK] += amount;

				format(donorstr, sizeof donorstr, "* %s puts some crack in the safe.", PlayerICName(playerid));
				ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}

			else if(strmatch(option, "take"))
			{
			    if(sscanf(params, "s[10]s[12]d", option, option2, amount)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dssafe take crack [amount]");
				if(amount <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
				if(DesmondSafe[DESMOND_SAFE_CRACK] < amount) return SendClientMessage(playerid, COLOR_GREY, "Your safe doesn't have that much crack stored.");

				PlayerInfo[playerid][pCrack] += amount;
				DesmondSafe[DESMOND_SAFE_CRACK] -= amount;

				format(donorstr, sizeof donorstr, "* %s takes some crack in the safe.", PlayerICName(playerid));
				ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
		}
		else return SendClientMessage(playerid, COLOR_GREY, "Invalid safe option.");
		SaveDesmond(2);
	}
	else SendClientMessage(playerid, COLOR_GREY, "You do not have the key.");
	return 1;
}

CMD:dshelp(playerid, params[])
{
    SendClientMessage(playerid, 0xFFFFFFFF, "_______________DeSantos Casino Commands_____________");
    SendClientMessage(playerid, 0xFFFFFFFF, "/dscasino  /dslogin  /dslogout  /dschangepass  /dsofficelock");
    SendClientMessage(playerid, 0xFFFFFFFF, "/dslock  /dsprices  /dsbroadcast /dsentryfee /dsmusic");
    SendClientMessage(playerid, 0xFFFFFFFF, "/dssafe /dssafecheck /dscamera");
    return 1;
}

CMD:dssafecheck(playerid, params[])
{
    if(PlayerInfo[playerid][pID] == 59148 || DesmondLoggedIn[playerid] == true)
	{
	    if(GetPlayerVirtualWorld(playerid) == 23 && IsPlayerInRangeOfPoint(playerid, 5.0, 1159.3403, 5.5914, 996.8517))
	    {
		    format(donorstr, sizeof donorstr, "Weapon 1: %s Weapon 2: %s Weapon 3: %s", dGetWeaponName(DesmondSafeWeapons[0]), dGetWeaponName(DesmondSafeWeapons[1]), dGetWeaponName(DesmondSafeWeapons[2]));
			SendClientMessage(playerid, COLOR_YELLOW, donorstr);
			
			format(donorstr, sizeof donorstr, "Weapon 4: %s Weapon 5: %s Weapon 6: %s", dGetWeaponName(DesmondSafeWeapons[3]), dGetWeaponName(DesmondSafeWeapons[4]), dGetWeaponName(DesmondSafeWeapons[5]));
			SendClientMessage(playerid, COLOR_YELLOW, donorstr);
			
			format(donorstr, sizeof donorstr, "Weapon 7: %s Weapon 8: %s Weapon 9: %s Weapon 10: %s", dGetWeaponName(DesmondSafeWeapons[6]), dGetWeaponName(DesmondSafeWeapons[7]), dGetWeaponName(DesmondSafeWeapons[8]), dGetWeaponName(DesmondSafeWeapons[9]));
			SendClientMessage(playerid, COLOR_YELLOW, donorstr);
			
			format(donorstr, sizeof donorstr, "Money: %i", DesmondSafe[DESMOND_SAFE_MONEY]);
			SendClientMessage(playerid, COLOR_YELLOW, donorstr);

			format(donorstr, sizeof donorstr, "Pot: %i", DesmondSafe[DESMOND_SAFE_POT]);
			SendClientMessage(playerid, COLOR_YELLOW, donorstr);

			format(donorstr, sizeof donorstr, "Crack: %i", DesmondSafe[DESMOND_SAFE_CRACK]);
			SendClientMessage(playerid, COLOR_YELLOW, donorstr);
		}
		else SendClientMessage(playerid, COLOR_GREY, "You are not near the safe.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You do not have the key.");
}

new Float:deslastpos[MAX_PLAYERS][3];
CMD:dscamera(playerid, params[])
{
	if(PlayerInfo[playerid][pID] != 59148 && DesmondLoggedIn[playerid] == false) return SendClientMessage(playerid, COLOR_GREY, "You do not have the key.");
	if(!IsPlayerInRangeOfPoint(playerid, 5.0, 1157.6342, 4.1783, 996.5691) && !IsPlayerInRangeOfPoint(playerid, 20, 2244.9270, 2398.9609, 1207.6519)) return SendClientMessage(playerid, COLOR_GREY, "You are not in the office/hideout."); // office, hideout
	if(isnull(params))
	{
	    format(donorstr, sizeof donorstr, "/dscamera [1-%d]", sizeof DesmondCameras);
	    SendClientMessage(playerid, COLOR_GREY, donorstr);
	    
	    SendClientMessage(playerid, COLOR_GREY, "You may additionally turn off the camera using off.");
	}
		
	else
	{
		if(strmatch(params, "off"))
		{
		    if(GetPVarInt(playerid, "DesmondCameraWatching") != -1)
		    {
		    	if(GetPVarInt(playerid, "DesmondCameraWatching") == 4)
		    	{
		    		SetPlayerPos(playerid, deslastpos[playerid][0], deslastpos[playerid][1], deslastpos[playerid][2]);
					SetPlayerInterior(playerid, 12);
					SetPlayerVirtualWorld(playerid, 23);
		    	}
		        SetCameraBehindPlayer(playerid);
		        SendClientMessage(playerid, COLOR_WHITE, "You have stopped watching the camera.");
		        SetPVarInt(playerid, "DesmondCameraWatching", -1);

		        TogglePlayerControllable(playerid, 1);
		        PlayerFrozen[playerid] = false;
			}
			else SendClientMessage(playerid, COLOR_GREY, "You are not watching the camera.");
			return 1;
		}

		new iCamera = strval(params);
		if(iCamera >= 1 && iCamera <= sizeof DesmondCameras)
		{
		    iCamera--;
			format(donorstr, sizeof donorstr, "You are now watching camera %d.", iCamera+1);
			SendClientMessage(playerid, COLOR_WHITE, donorstr);
			
			SetPlayerCameraPos(playerid, DesmondCameras[iCamera][0], DesmondCameras[iCamera][1], DesmondCameras[iCamera][2]);
			SetPlayerCameraLookAt(playerid, DesmondCameras[iCamera][3], DesmondCameras[iCamera][4], DesmondCameras[iCamera][5]);

			TogglePlayerControllable(playerid, 0);
			PlayerFrozen[playerid] = true;
			if(iCamera == 4)
			{
				if(GetPVarInt(playerid, "DesmondCameraWatching") == -1) GetPlayerPos(playerid, deslastpos[playerid][0], deslastpos[playerid][1], deslastpos[playerid][2]);
				SetPlayerInterior(playerid, 0);
				SetPlayerVirtualWorld(playerid, 0);
				SetPlayerPos(playerid, 1993.8058, -1983.8760, 17.4402);
			}
			SetPVarInt(playerid, "DesmondCameraWatching", iCamera);
		}
		else SendClientMessage(playerid, COLOR_GREY, "Invalid camera.");
	}
	return 1;
}
				
//================ [Tiger Woods] ================
CMD:twmagic(playerid, params[])
{
	new playername[MAX_PLAYER_NAME];
	GetPlayerName(playerid, playername, MAX_PLAYER_NAME);
	if(IsPlayerInRangeOfPoint(playerid, 3.0, 145.6084, 1711.9868, 10016.0996))
	{
		if(tbss == false) // opened
		{
			MoveDynamicObject(twShelf, 145.955444, 1711.018798,10016.094726, 2.0);
			tbss = true;
			format(donorstr, sizeof(donorstr), "%s searches the books shelf for his favorite book.", playername);
			ProxDetector(30.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		}
		else if(tbss == true) // closed
		{
			MoveDynamicObject(twShelf, 145.973022,1712.458984,10016.094726, 2.0);
			tbss = false;
		}
		return 1;
	}
	if(IsPlayerInRangeOfPoint(playerid, 2.0, 144.1367, 1721.3069, 10017.6279) && ((gettime() - pressTimer) >= 5))
  	{
   		ApplyAnimation(playerid, "HEIST9", "Use_SwipeCard", 10.0, 0, 0, 0, 0, 0);
	 	new Float:pH;
	  	GetPlayerHealth(tEChair, pH);
	  	SetPlayerHealth(tEChair, pH-20);
	  	TextDrawShowForPlayer(tEChair, blood);
	  	SetTimerEx("BloodHide", 4999, false, "i", tEChair);
	  	pressTimer = gettime();
	  	format(donorstr, sizeof(donorstr), "%s presses the switch.", playername);
	 	ProxDetector(30.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
   	}
   	else return SendClientMessage(playerid, -1, "It looks like you didn't wait 5 seconds to use the button again or you are not near it!");
	return 1;
}
CMD:twchair(playerid, params[])
{
	new playerb;
	if(sscanf(params, "u", playerb)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /twchair [playerid]");
	if(!IsPlayerConnected(playerb)) return SendClientMessage(playerid, COLOR_GREY, "That player isn't connected.");
	if(inchair[playerb]) return SendClientMessage(playerid, -1, "	He is already in the chair!");
	if(tEChair != INVALID_PLAYER_ID) return SendClientMessage(playerid, -1, "Release the guy that is in the chair already!");
	if(!IsPlayerInRangeOfPoint(playerid, 3.0, 145.7005, 1723.3237, 10017.4072)) return SendClientMessage(playerid, -1, "	You are not near the chair !");
	if(playerb == playerid) return SendClientMessage(playerid, COLOR_WHITE, "You really trying to kill yourself? Use /kill");
	if(IsPlayerInRangeOfPlayer(2.0, playerid, playerb))
	{
		format(donorstr, sizeof(donorstr), "You have forced %s into the chair !", PlayerICName(playerb));
		SendClientMessage(playerid, COLOR_GREY, donorstr);
		format(donorstr, sizeof(donorstr), "You have been forced into the chair by %s!", PlayerICName(playerid));
		SendClientMessage(playerb, COLOR_GREY, donorstr);
		format(donorstr, sizeof(donorstr), "* %s forces %s into the electric chair. *", PlayerICName(playerid), PlayerICName(playerb));
		ProxDetector(30.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		ClearAnimations(playerb);
		SetPlayerPos(playerb, 145.7005, 1723.3237, 10017.4072);
		inchair[playerb] = true;
		tEChair = playerb;
		TogglePlayerControllable(playerb, 0);
		ApplyAnimation(playerb, "PED","SEAT_down",4.0,0,0,0,1,0);
		SetCameraBehindPlayer(playerb);
		SetPlayerFacingAngle(playerb, 174.1728);
	}
	else
	{
		SendClientMessage(playerid, COLOR_GREY, "	That player is not near you or the chair!");
		return 1;
	}
	return 1;
}
CMD:twexecute(playerid, params[])
{
	new playerb;
	if(sscanf(params, "u", playerb)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /twexecute [playerid]");
	if(!IsPlayerConnected(playerb)) return SendClientMessage(playerid, COLOR_GREY, "That player isn't connected.");
	if(!inchair[playerb]) return SendClientMessage(playerid, -1, "	He is not in the chair!");
	if(!IsPlayerInRangeOfPoint(playerid, 3.0, 144.4691, 1721.3744, 10017.0996)) return SendClientMessage(playerid, -1, "	You are not near the power switch !");
	if(playerb == playerid) return SendClientMessage(playerid, COLOR_WHITE, "You really trying to kill yourself? Use /kill");
	new Float:hp;
 	GetPlayerHealth(playerb, hp);
	if(hp < 21)
	{
		SetPlayerFacingAngle(playerid, 96.0777);
		format(donorstr, sizeof(donorstr), "* %s pulls a leaver, electrocuting %s with 2,000 volts. *", PlayerICName(playerid), PlayerICName(playerb));
		ProxDetector(30.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		new Float:health;
  		GetPlayerHealth(playerb, health);
		SetPlayerHealth(playerb, health-20);
		SendClientMessage(playerb, COLOR_RED, "You are now dead !");
		ClearAnimations(playerb);
		inchair[playerb] = false;
		tEChair = INVALID_PLAYER_ID;
		TogglePlayerControllable(playerb, 1);
		format(donorstr, sizeof donorstr, "You have killed %s", PlayerICName(playerb));
		SendClientMessage(playerid, COLOR_GREY, donorstr);
		return 1;
	}
	else
	{
		SetPlayerFacingAngle(playerid, 96.0777);
		format(donorstr, sizeof(donorstr), "* %s pulls a leaver, electrocuting %s with 2,000 volts. *", PlayerICName(playerid), PlayerICName(playerb));
		ProxDetector(30.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		new Float:health;
  		GetPlayerHealth(playerb, health);
		SetPlayerHealth(playerb, health-20);
		SendClientMessage(playerb, COLOR_RED, "You have been injured and lose 20 health !");
		return 1;
	}
}

CMD:twrelease(playerid, params[])
{
	new playerb;
	if(sscanf(params, "u", playerb)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /twrelease [playerid]");
	if(!IsPlayerConnected(playerb)) return SendClientMessage(playerid, COLOR_GREY, "That player isn't connected.");
	if(!inchair[playerb]) return SendClientMessage(playerid, -1, "	That player is not in the chair !");
	if(playerb == playerid) return SendClientMessage(playerid, COLOR_WHITE, "You should not be seeing this message");
	if(!IsPlayerInRangeOfPoint(playerid, 3.0, 145.7005, 1723.3237, 10017.4072)) return SendClientMessage(playerid, -1, "	You are not near the chair!");
	format(donorstr, sizeof(donorstr), "You released %s from the chair !", PlayerICName(playerb));
	SendClientMessage(playerid, COLOR_GREY, donorstr);
	format(donorstr, sizeof(donorstr), "You were released by %s!", PlayerICName(playerid));
	SendClientMessage(playerb, COLOR_GREY, donorstr);
	format(donorstr, sizeof(donorstr), "* %s released %s from the electric chair. *", PlayerICName(playerid), PlayerICName(playerb));
	ProxDetector(30.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	ClearAnimations(playerb);
	SetPlayerPos(playerb, 145.7005, 1723.3237, 10017.4072);
	inchair[playerb] = false;
	tEChair = INVALID_PLAYER_ID;
	TogglePlayerControllable(playerb, 1);
	SetPlayerFacingAngle(playerb, 174.1728);
	return 1;
}
CMD:twcam(playerid, params[])
{
	new playername[MAX_PLAYER_NAME], id;
 	GetPlayerName(playerid, playername, sizeof(playername));
  	if(IsPlayerInRangeOfPoint(playerid, 5.0, 148.1596, 1707.5944, 10017.0996) || IsPlayerInRangeOfPoint(playerid, 5.0, 1460, 2862, -5))
   	{
    	if(sscanf(params, "i", id)) return SendClientMessage(playerid, -1, "Usage:/twcam [0-21]");
    	if(id == 0)
    	{
  			SetCameraBehindPlayer(playerid);
		  	SetPlayerPos(playerid, 148.1596, 1707.5944, 10017.099);
		  	TogglePlayerControllable(playerid, 1);
	    }
		if(id > 0 && id < 12)
		{
		    TogglePlayerControllable(playerid, true);
		  	SetPlayerPos(playerid, 148.1596, 1707.5944, 10017.099);
		  	TogglePlayerControllable(playerid, false);
			SetPlayerCameraPos(playerid, CamPos[id][0], CamPos[id][1], CamPos[id][2]);
			SetPlayerCameraLookAt(playerid, CamPos[id][3], CamPos[id][4], CamPos[id][5]);

		}
		if(id >= 12 && id < 22)
		{
			SetPlayerCameraPos(playerid, CamPos[id][0], CamPos[id][1], CamPos[id][2]);
			SetPlayerCameraLookAt(playerid, CamPos[id][3], CamPos[id][4], CamPos[id][5]);
  			SetPlayerPos(playerid, 1460, 2862, -5);
    		TogglePlayerControllable(playerid, 0);
		}
	}
	else SendClientMessage(playerid, -1, "You are not in range of the camera room");
    return 1;
}
CMD:twmic(playerid, params[]) {
	if(isnull(params)) return SendClientMessage(playerid, -1, "Usage: /twmic [text]");
	format(donorstr,sizeof(donorstr), "[CAMERA-CONTROL]%s <o: %s", PlayerICName(playerid), params);
	if(IsPlayerInRangeOfPoint(playerid, 5.0, 148.1596, 1707.5944, 10017.0996) || IsPlayerInRangeOfPoint(playerid, 5.0, 1460, 2862, -5))
	{
		for(new i = 0; i < MAX_PLAYERS; i++) {
		    if(IsPlayerInRangeOfPoint(i, 35.0, 148.1596, 1707.5944, 10017.0996) ||
				IsPlayerInDynamicArea(i, twCircle, 0))
		    {
		        SendClientMessage(i, COLOR_YELLOW, donorstr);
		    }
		}
	}
	else SendClientMessage(playerid, -1, "You are not in range of the camera room");
	return 1;
}
CMD:twcmds(playerid, params[]) {
	SendClientMessage(playerid, -1, "<Tiger Woods Commands>:");
	SendClientMessage(playerid, -1, "/twmagic, /twcam, /twchair, /twrelease, /twexecute /twmic");
	return 1;
}

//================ [Lloyd Christmas] ================
/*          UNKNOWN DEVELOPMENT RERQUEST          */

CMD:throwknife(playerid, params[])
{
	if(IsPlayerInRangeOfPoint(playerid,2,1538.4371, -1349.1216, 312.0913))
	{
	    ApplyAnimation(playerid,"PED","fucku",4.0,0,0,0,0,0);
	    if(knives == 0)
   		{
    		knives = 1;
     		MoveDynamicObject(knife[0], 1541.19958, -1343.50024, 313.28717,20);
   		}
    	else if(knives==1)
	   	{
	       	knives = 2;
	       	MoveDynamicObject(knife[1], 1541.61938, -1343.89941, 313.57169,20);
	   	}
	   	else if(knives==2)
	   	{
	       	knives = 3;
	       	MoveDynamicObject(knife[2], 1541.31763, -1343.65881, 313.68930,20);
	   	}
	   	else if(knives==3)
	   	{
	       	knives = 4;
	       	MoveDynamicObject(knife[3], 1541.00818, -1343.36218, 313.52777,20);
	   	}
	   	else if(knives==4)
	   	{
	       	knives = 5;
	       	MoveDynamicObject(knife[4], 1541.20789, -1343.76306, 312.95313,20);
	   	}
	   	else if(knives==5)
	   	{
	       	knives = 0;
	       	MoveDynamicObject(knife[0], 1538.14270, -1349.07190, 312.69495,20);
	       	MoveDynamicObject(knife[1], 1538.21948, -1349.21375, 312.72070,20);
	       	MoveDynamicObject(knife[2], 1538.28870, -1349.29175, 312.71960,20);
	       	MoveDynamicObject(knife[3], 1538.32544, -1349.40173, 312.69818,20);
	       	MoveDynamicObject(knife[4], 1538.08618, -1348.90894, 312.74545,20);
	   	}
	}
	return 1;
}

CMD:llxmas1(playerid, params[])
{
	if(IsPlayerInRangeOfPoint(playerid, 15.0, 1525.3603500, -1364.8575400, 35.0587000))
	{
		if(llcmas[0] == false)
		{
	    	MoveDynamicObject(xmas[0], 1525.3603500,-1364.8575400,35.0587000, 2.0);
	    	llcmas[0] = true;

		}
		else if(llcmas[0] == true)
		{
	    	MoveDynamicObject(xmas[0], 1526.8753700,-1364.8575400,35.0587000,2.0);
	    	llcmas[0] = false;
		}
	}
	return 1;
}

CMD:llxmas2(playerid, params[])
{
	if(IsPlayerInRangeOfPoint(playerid, 15.0, 1526.8292200, -1364.8554700, 40.8157000))
	{
		if(llcmas[1] == false)
		{
	    	MoveDynamicObject(xmas[1], 1526.8292200,-1364.8554700,40.8157000, 2.0);
	    	llcmas[1] = true;

		}
		else if(llcmas[1] == true)
		{
			MoveDynamicObject(xmas[1], 1525.3353300,-1364.8554700,40.8157000, 2.0);
			llcmas[1] = false;
		}
	}
	return 1;
}

CMD:llxmas3(playerid, params[])
{
	if(IsPlayerInRangeOfPoint(playerid, 15.0, 1526.8043200, -1364.8584000, 46.6117000))
	{
		if(llcmas[2] == false)
		{
	    	MoveDynamicObject(xmas[2],1526.8043200,-1364.8584000,46.6117000, 2.0);
	    	llcmas[2] = true;

		}
		else if(llcmas[2] == true)
		{
	    	MoveDynamicObject(xmas[2], 1525.3293500,-1364.8584000,46.6117000, 2.0);
	    	llcmas[2] = false;
		}
	}
	return 1;
}

CMD:llcfreedom(playerid, params[])
{
	if(IsPlayerInRangeOfPoint(playerid, 15.0, 1520.8291000,-1358.6717500,40.4553000))
	{
		if(llcmas[3] == false)
		{
	    	MoveDynamicObject(xmas[3], 1519.5900900,-1358.6717500,40.4553000, 2.0);
	    	llcmas[3] = true;

		}
		else if(llcmas[3] == true)
		{
	    	MoveDynamicObject(xmas[3], 1520.8291000,-1358.6717500,40.4553000,2.0);
	    	llcmas[3] = false;
		}
	}

	return 1;
}

CMD:llfinalsolution(playerid, params[])
{
	if(llcmas[4] == false)
	{
	    xmas[4] = CreateDynamicObject(2780,1525.4133300,-1352.7124000,40.3539000,0.0000000,0.0000000,0.0000000);
        llcmas[4] = true;
	}
	else if(llcmas[4] == true)
	{
		DestroyDynamicObjectEx(xmas[4]);
        llcmas[4] = false;
	}
	return 1;
}

CMD:christmasdoors(playerid, params[])
{
    if(towerdoorstatus == 0)
    {
        DestroyDynamicObjectEx(towerdoor[0]);
        DestroyDynamicObjectEx(towerdoor[1]);
   		towerdoor[2] = CreateDynamicObject(1491, 1547.88977, -1358.90906, 311.87177,   0.00000, 0.00000, 49.44001);
		towerdoor[3] = CreateDynamicObject(1491, 1543.84399, -1358.47437, 323.46375,   0.00000, 0.00000, -41.22001);
		towerdoorstatus = 1;
	}
	else
	{
	    DestroyDynamicObjectEx(towerdoor[2]);
	    DestroyDynamicObjectEx(towerdoor[3]);
	    towerdoor[0] = CreateDynamicObject(1504, 1547.88977, -1358.90906, 311.87177,   0.00000, 0.00000, 49.44001);
	    towerdoor[1] = CreateDynamicObject(1504, 1543.84399, -1358.47437, 323.46375,   0.00000, 0.00000, -41.22001);
	    towerdoorstatus = 0;
	}
	return 1;
}

CMD:lloyd1(playerid, params[])
{
    if(IsPlayerInRangeOfPoint(playerid,6,1541.9911, -1358.6097, 311.9182))
	{
	    if(LloydSD == 0)
   		{
  			LloydSD = 1;
   			MoveDynamicObject(secretdoor, 1544.6873, -1359.8224, 313.6179,0.8);
   		}
    	else if(LloydSD == 1)
	   	{
	       	LloydSD = 0;
	       	MoveDynamicObject(secretdoor, 1543.56836, -1358.80444, 313.61789,0.8);
	   	}
	}
	return 1;
}

CMD:lloyd2(playerid, params[])
{
    if(IsPlayerInRangeOfPoint(playerid,3,1539.8979, -1354.9958, 311.9212))
	{
	    if(LloydSS == 0)
   		{
   			LloydSS = 1;
   			MoveDynamicObject(secretstorage[0], 1540.8979, -1353.9958, 311.9212,0.8);
  			MoveDynamicObject(secretstorage[1], 1537.9476, -1357.0811, 311.9192,0.8);
       		MoveDynamicObject(secretstorage[2], 1538.0603, -1356.9464, 313.6119,0.8);
       		MoveDynamicObject(secretstorage[3], 1541.1069, -1353.7429, 313.6139,0.8);
       	}
	    else if(LloydSS == 1)
		{
		   	LloydSS = 0;
			MoveDynamicObject(secretstorage[0], 1539.89795, -1354.99585, 311.92120,0.8);
	       	MoveDynamicObject(secretstorage[1], 1538.94763, -1356.08105, 311.91919,0.8);
	       	MoveDynamicObject(secretstorage[2], 1539.06030, -1355.94641, 313.61191,0.8);
	       	MoveDynamicObject(secretstorage[3], 1540.10693, -1354.74292, 313.61389,0.8);
	   	}
	}
	return 1;
}

//================ [Dino Cicarelli] ================
/*CMD:dinosafe(playerid, params[])
{
    if(PlayerInfo[playerid][pID] == 93337)
    {
   		new option[32], amount;
    	if(!IsPlayerInRangeOfPoint(playerid, 2.0, 986.92847, -2151.49316, 13.59967)) return SendClientMessage(playerid, -1, "You're not near the safe.");
        if(sscanf(params,"s[32]", params))
        {
            SendClientMessage(playerid, -1, "/dinosafe [type]");
            SendClientMessage(playerid, 0xAFAFAFAA, "Type: balance | withdraw | deposit");
            return 1;
        }
        if(!strcmp(params, "balance",true,7))
        {
            new string2[500];
            format(donorstr, sizeof(donorstr), "{FFFFFF}Cash: {FFCA05}%d \n{FFFFFF}Pot: {FFCA05}%d \n{FFFFFF}Crack: {FFCA05}%d \n{FFFFFF}Materials: {FFCA05}%d \n", dinosafeCash, dinosafePot, dinosafeCrack, dinosafeMats);
            strcat(donorstr, "{FFFFFF}Weapon Slot 0: {FFCA05}%s\n{FFFFFF}Weapon Slot 1: {FFCA05}%s\n{FFFFFF}Weapon Slot 2: {FFCA05}%s\n");
            strcat(donorstr, "{FFFFFF}Weapon Slot 3: {FFCA05}%s\n{FFFFFF}Weapon Slot 4: {FFCA05}%s\n{FFFFFF}Weapon Slot 5:{FFCA05} %s \n");
            strcat(donorstr, "{FFFFFF}Weapon Slot 6: {FFCA05}%s\n{FFFFFF}Weapon Slot 7: {FFCA05}%s\n{FFFFFF}Weapon Slot 8: {FFCA05}%s \n{FFFFFF}Weapon Slot 9: {FFCA05}%s");
            format(string2, sizeof(string2), donorstr,
            dGetWeaponName(dinosafeWeapon[0]), dGetWeaponName(dinosafeWeapon[1]), dGetWeaponName(dinosafeWeapon[2]), dGetWeaponName(dinosafeWeapon[3]),
            dGetWeaponName(dinosafeWeapon[4]), dGetWeaponName(dinosafeWeapon[5]), dGetWeaponName(dinosafeWeapon[6]), dGetWeaponName(dinosafeWeapon[7]),
            dGetWeaponName(dinosafeWeapon[8]), dGetWeaponName(dinosafeWeapon[9]));

            ShowPlayerDialog(playerid, 21351, DIALOG_STYLE_MSGBOX, "                {FF00A2} The Safe", string2, "Close", "");
            return 1;
        }
        else if(!strcmp(params, "withdraw",true,8)) //negative values okay, makes safe positive and player negative
        {
            if(sscanf(params,"s[32]s[32]i", params, option, amount))
            {
                SendClientMessage(playerid, -1, "/dinosafe withdraw [option] [amount]");
                SendClientMessage(playerid, 0xAFAFAFAA, "Option: cash | pot | crack | mats | weapon(amount == slot || 0 - 9 slots)");
                return 1;
            }
            if(!strcmp(option, "cash",true,4))
            {
                if(amount > dinosafeCash) return SendClientMessage(playerid, 0xAA3333AA, "The safe doesn't have that amount of money.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "You withdraw no money.");
				dinosafeCash -= amount;
                GiveMoney(playerid, amount);
                SaveDino();
            }
            else if(!strcmp(option, "pot",true,3))
            {
                if(amount > dinosafePot) return SendClientMessage(playerid, 0xAA3333AA, "The safe doesn't have that amount of pot.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "You withdraw no grams of pot.");
				dinosafePot -= amount;
                PlayerInfo[playerid][pPot] += amount;
				SaveDino();
            }
            else if(!strcmp(option, "crack",true, 5))
            {
                if(amount > dinosafeCrack) return SendClientMessage(playerid, 0xAA3333AA, "The safe doesn't have that amount of crack.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "You withdraw no grams of crack.");
				dinosafeCrack -= amount;
                PlayerInfo[playerid][pCrack] += amount;
                SaveDino();
            }
            else if(!strcmp(option, "mats",true, 5))
            {
                if(amount > dinosafeMats) return SendClientMessage(playerid, 0xAA3333AA, "The safe doesn't have that amount of materials.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "You withdraw no materials.");
				dinosafeMats -= amount;
                PlayerInfo[playerid][pMats] += amount;
                SaveDino();
            }
            else if(!strcmp(option, "weapon",true, 6))
            {
                new slot = amount, weaponid = dinosafeWeapon[slot];
                if(dinosafeWeapon[slot] == 0) return SendClientMessage(playerid, 0xAA3333AA, "There is no weapon in that slot.");
                GivePlayerGun(playerid, weaponid);
                dinosafeWeapon[slot] = 0;
                SaveDino();
            }
        }
        else if(!strcmp(params, "deposit",true,7))
        {
            if(sscanf(params,"s[32]s[32]i", params, option, amount))
            {
                SendClientMessage(playerid, -1, "/dinosafe deposit [option] [amount]");
                SendClientMessage(playerid, 0xAFAFAFAA, "Option: cash | pot | crack | materials | weapon(amount == slot || 0 - 9 slots)");
                return 1;
            }
            if(!strcmp(option, "cash",true,4))
            {
                if(dinosafeCash > 10000000 ||  amount > PlayerInfo[playerid][pCash]) return SendClientMessage(playerid, 0xAA3333AA, "You either don't have the amount of cash or the safe is full.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "You deposit no money.");
				dinosafeCash += amount;
                GiveMoney(playerid, -amount);
				SaveDino();
            }
            else if(!strcmp(option, "pot",true,3))
            {
                if(amount > PlayerInfo[playerid][pPot]) return SendClientMessage(playerid, 0xAA3333AA, "You don't have that amount of pot.");
                if(amount >= 1000) return SendClientMessage(playerid, 0xAA3333AA, "This safe can only hold 1000 grams of pot.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "You deposit no grams of pot.");
				dinosafePot += amount;
                PlayerInfo[playerid][pPot] -= amount;
    			SaveDino();
            }
            else if(!strcmp(option, "crack",true,5))
            {
                if(amount > PlayerInfo[playerid][pCrack]) return SendClientMessage(playerid, 0xAA3333AA, "You don't have that amount of crack.");
                if(amount >= 1000) return SendClientMessage(playerid, 0xAA3333AA, "This safe can only hold 1000 grams of crack.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "You deposit no grams of crack.");
				dinosafeCrack += amount;
                PlayerInfo[playerid][pCrack] -= amount;
                SaveDino();
            }
            else if(!strcmp(option, "mats",true,5))
            {
                if(amount > PlayerInfo[playerid][pMats]) return SendClientMessage(playerid, 0xAA3333AA, "You don't have that amount of materials.");
                if(amount >= 500000) return SendClientMessage(playerid, 0xAA3333AA, "This safe can only hold 500,000 materials.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "You deposit no materials.");
				dinosafeMats += amount;
                PlayerInfo[playerid][pMats] -= amount;
                SaveDino();
            }
            else if(!strcmp(option, "weapon",true,6))
            {
                new slot = amount, gunid = GetPlayerWeapon(playerid);
                if(gunid == 0) return SendClientMessage(playerid, 0xAA3333A, "You're not holding a weapon");
                if(HaveAdminWeapon(playerid, gunid) == gunid) return SendClientMessage(playerid, COLOR_GREY, "You cannot store this weapon.");
                if(dinosafeWeapon[slot] != 0) return SendClientMessage(playerid, 0xAA3333AA, "There is a weapon in this slot.");
                dinosafeWeapon[slot] = GetPlayerWeapon(playerid);
                TakePlayerWeapon(playerid, gunid);
                SaveDino();
            }
        }
    }
	return 1;
}
*/
//================ [Ace Johnson] ================
CMD:mansion(playerid, params[])
{
	if(isnull(params)) return SendClientMessage(playerid, COLOR_GRAD1, "USAGE: /mansion [garage/garagedoor]");
	if(!strcmp(params, "garage", true, 6))
	{
		if(IsPlayerInRangeOfPoint(playerid, 20.0, 316.7000, -1136.8199, 81.3500))
		{
			if(AceGateOpen[0] == true && AceGateOpen[1] == true)
			{
	            MoveDynamicObject(AceGate[0], 316.7000, -1136.8199, 81.3500, 0.3, 0.0000, 0.0000, 133.3800);
	            MoveDynamicObject(AceGate[1], 316.7000, -1136.8199, 81.3500, 0.3, 0.0000, 180.0000, 133.3800);
	            PlayerPlaySound(playerid, 1153, 316.7000, -1136.8199, 81.3500);
	            AceGateOpen[0] = false, AceGateOpen[1] = false;
			}
			else
			{
	            MoveDynamicObject(AceGate[0], 317.4825, -1137.6481, 82.3940, 0.3, 0.0000, 90.0000, 133.3800);
	            MoveDynamicObject(AceGate[1], 317.4825, -1137.6481, 82.3940, 0.3, 0.0000, 270.0000, 133.3800);
	            PlayerPlaySound(playerid, 1153, 316.7000, -1136.8199, 81.3500);
	            AceGateOpen[0] = true, AceGateOpen[1] = true;		
			}
		}
	}
	else if(!strcmp(params, "garagedoor", true, 10))
	{
		if(IsPlayerInRangeOfPoint(playerid, 3, 308.69, -1139.83, 80.86))
		{
			if(AceGateOpen[2] == true)
			{
				MoveDynamicObject(AceGate[2], 308.6859, -1139.8292, 80.8638, 0.02, 0.0000, 0.0000, 84.5400);
				PlayerPlaySound(playerid, 6400, 311.2500, -1140.5000, 81.2000);
				AceGateOpen[2] = false;
			}
			else MoveDynamicObject(AceGate[2], 308.69, -1139.83, 80.86, 0.02, 0.0000, 0.0000, -46.5000), AceGateOpen[2] = true;
		}
		if(IsPlayerInRangeOfPoint(playerid, 3, 302.06, -1128.53, 79.51))
		{
			if(AceGateOpen[3] == true)
			{
				MoveDynamicObject(AceGate[3], 302.0622, -1128.5250, 79.5102, 0.02, 0.0000, 0.0000, -227.5199);
				AceGateOpen[3] = false;
			}
			else MoveDynamicObject(AceGate[3], 302.06, -1128.53, 79.51, 0.02, 0.00, 0.00, -135.66), AceGateOpen[3] = true;
		}
	}
	return 1;
}

//================ [Gunnar Watsone] ================
stock CanGunnar(playerid)
{
	if(gunnarKey[playerid] == 1 || PlayerInfo[playerid][pID] == GUNNARWATSONE_ID) return 1;
	return 0;
}

stock GetGunnarWeaponName(weaponid)
{
    new wepname[32];
    GetWeaponName(weaponid, wepname, sizeof(wepname));
    if(weaponid == 0) format(wepname, sizeof(wepname), "Empty");
    return wepname;
}

CMD:gymwater(playerid, params[])
{
	if(!IsPlayerInRangeOfPoint(playerid, 5.0, 2214.8909,596.8708,9097.7402)) return SendClientMessage(playerid, COLOR_GREY, "You are not in the gym.");
	
	new Float:gunnar_HP;
	GetPlayerHealth(playerid, gunnar_HP);
	if(gunnar_HP + 25 < 101){
	    SetPlayerHealth(playerid, gunnar_HP + 25);
	}
	else
	{
	    SetPlayerHealth(playerid, 100);
	}
	GameTextForPlayer(playerid, "~w~You drink some ~b~water.", 3000, 3);
	return 1;
}

CMD:cagefighter(playerid, params[])
{
	if(!IsPlayerInRangeOfPoint(playerid, 5.0, 2218.1853,604.1145,9097.7393)) return SendClientMessage(playerid, COLOR_GREY, "You are not near the lockers.");
	if(GetPlayerSkin(playerid) == 80 || GetPlayerSkin(playerid) == 81)
	{
	    SetPlayerSkin(playerid, gunnarOldSkin[playerid]);
	}
	else
	{
	    gunnarOldSkin[playerid] = GetPlayerSkin(playerid);
	    new skinPick = random(2);
	    if(skinPick == 0)
	    {
	    	SetPlayerSkin(playerid, 80);
		}
		else
		{
		    SetPlayerSkin(playerid, 81);
		}
	}
	return 1;
}

CMD:gunnarsafe(playerid, params[])
{
	if(!CanGunnar(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You cannot do this.");
	if(!IsPlayerInRangeOfPoint(playerid, 5.0, 2248.0896,556.7626,9097.7354)) return SendClientMessage(playerid, COLOR_GREY, "You are not near the safe.");
    new board_Op[10];
	if(sscanf(params, "s[10]", board_Op)) return SendClientMessage(playerid, COLOR_GREY, "USAGE: /gunnarsafe [check/withdraw/deposit]");
	
	new gunnar_Item[10], gunnar_Amount;
	if(!strcmp(board_Op, "check",true,5))
 	{
 	    new gunnar_Str[440];
 	    format(gunnar_Str, sizeof(gunnar_Str), "{90C3D4}Cash:{FFFFFF} $%i\n{90C3D4}Crack:{FFFFFF} %i\n{90C3D4}Pot:{FFFFFF} %i\n{90C3D4}Materials:{FFFFFF} %i", gunnarsafeCash, gunnarsafeCrack, gunnarsafePot, gunnarsafeMats);
 	    format(gunnar_Str, sizeof(gunnar_Str), "%s\n{90C3D4}Weapon 0:{FFFFFF} %s\n{90C3D4}Weapon 1:{FFFFFF} %s\n{90C3D4}Weapon 2:{FFFFFF} %s\n{90C3D4}Weapon 3:{FFFFFF} %s\n{90C3D4}Weapon 4:{FFFFFF} %s", gunnar_Str, GetGunnarWeaponName(gunnarsafeWeapon[0]), GetGunnarWeaponName(gunnarsafeWeapon[1]), GetGunnarWeaponName(gunnarsafeWeapon[2]), GetGunnarWeaponName(gunnarsafeWeapon[3]), GetGunnarWeaponName(gunnarsafeWeapon[4]));
        format(gunnar_Str, sizeof(gunnar_Str), "%s\n{90C3D4}Weapon 5:{FFFFFF} %s\n{90C3D4}Weapon 6:{FFFFFF} %s\n{90C3D4}Weapon 7:{FFFFFF} %s\n{90C3D4}Weapon 8:{FFFFFF} %s\n{90C3D4}Weapon 9:{FFFFFF} %s", gunnar_Str, GetGunnarWeaponName(gunnarsafeWeapon[5]), GetGunnarWeaponName(gunnarsafeWeapon[6]), GetGunnarWeaponName(gunnarsafeWeapon[7]), GetGunnarWeaponName(gunnarsafeWeapon[8]), GetGunnarWeaponName(gunnarsafeWeapon[9]));
		ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Safe", gunnar_Str, "Close", "");
 	}
 	else if(!strcmp(board_Op, "withdraw",true,8))
 	{
 	    if(PlayerInfo[playerid][pID] != GUNNARWATSONE_ID) return SendClientMessage(playerid, COLOR_GREY, "You can't do this.");
 	    
 	    if(sscanf(params,"s[10]s[10]i", board_Op, gunnar_Item, gunnar_Amount))
        {
            SendClientMessage(playerid, COLOR_GREY, "USAGE: /gunnarsafe withdraw [item] [amount (weapon slot)]");
            SendClientMessage(playerid, COLOR_GREY, "Items: cash, crack, pot, materials, weapon");
            return 1;
        }
        if(gunnar_Amount < 1) return SendClientMessage(playerid, COLOR_GREY, "Withdraw a valid amount.");
        
        if(!strcmp(gunnar_Item, "cash",true,4))
 		{
 		    if(gunnar_Amount > gunnarsafeCash) return SendClientMessage(playerid, COLOR_GREY, "The safe doesn't have enough.");

 		    GiveMoney(playerid, gunnar_Amount);
 		    gunnarsafeCash -= gunnar_Amount;
 		    SendClientMessage(playerid, -1, "{90C3D4}* You have withdrawn some cash.");
 		}
 		else if(!strcmp(gunnar_Item, "crack",true,5))
 		{
 		    if(gunnar_Amount > gunnarsafeCrack) return SendClientMessage(playerid, COLOR_GREY, "The safe doesn't have enough.");

 		    PlayerInfo[playerid][pCrack] += gunnar_Amount;
 		    gunnarsafeCrack -= gunnar_Amount;
 		    SendClientMessage(playerid, -1, "{90C3D4}* You have withdrawn some crack.");
 		}
 		else if(!strcmp(gunnar_Item, "pot",true,3))
 		{
 		    if(gunnar_Amount > gunnarsafePot) return SendClientMessage(playerid, COLOR_GREY, "The safe doesn't have enough.");

 		    PlayerInfo[playerid][pPot] += gunnar_Amount;
 		    gunnarsafePot -= gunnar_Amount;
 		    SendClientMessage(playerid, -1, "{90C3D4}* You have withdrawn some pot.");
 		}
 		else if(!strcmp(gunnar_Item, "materials",true,9))
 		{
 		    if(gunnar_Amount > gunnarsafeMats) return SendClientMessage(playerid, COLOR_GREY, "The safe doesn't have enough.");

 		    PlayerInfo[playerid][pMats] += gunnar_Amount;
 		    gunnarsafeMats -= gunnar_Amount;
 		    SendClientMessage(playerid, -1, "{90C3D4}* You have withdrawn some materials.");
 		}
 		else if(!strcmp(gunnar_Item, "weapon",true,6))
 		{
            new slot = gunnar_Amount, weaponid = gunnarsafeWeapon[slot];
            if(weaponid == 0) return SendClientMessage(playerid, COLOR_GREY, "That weapon slot is empty.");
            GivePlayerGun(playerid, weaponid);
            gunnarsafeWeapon[slot] = 0;

            SendClientMessage(playerid, -1, "{90C3D4}* You have withdrawn a weapon.");
 		}
 		else return SendClientMessage(playerid, COLOR_GREY, "Invalid item.");
 		SaveGunnar();
 	}
 	else if(!strcmp(board_Op, "deposit",true,7))
 	{
 	    if(sscanf(params,"s[10]s[10]", board_Op, gunnar_Item, gunnar_Amount))
        {
            SendClientMessage(playerid, COLOR_GREY, "USAGE: /gunnarsafe deposit [item] [amount (weapon slot)]");
            SendClientMessage(playerid, COLOR_GREY, "Items: cash, crack, pot, materials, weapon");
            return 1;
        }
        if(gunnar_Amount < 1) return SendClientMessage(playerid, COLOR_GREY, "Deposit a valid amount.");
        
        if(!strcmp(gunnar_Item, "cash",true,4))
 		{
 		    if(gunnar_Amount > PlayerInfo[playerid][pCash]) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough.");
 		    
 		    GiveMoney(playerid, -gunnar_Amount);
 		    gunnarsafeCash += gunnar_Amount;
 		    SendClientMessage(playerid, -1, "{90C3D4}* You have deposited some cash.");
 		}
 		else if(!strcmp(gunnar_Item, "crack",true,5))
 		{
 		    if(gunnar_Amount > PlayerInfo[playerid][pCrack]) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough.");

 		    PlayerInfo[playerid][pCrack] -= gunnar_Amount;
 		    gunnarsafeCrack += gunnar_Amount;
 		    SendClientMessage(playerid, -1, "{90C3D4}* You have deposited some crack.");
 		}
 		else if(!strcmp(gunnar_Item, "pot",true,3))
 		{
 		    if(gunnar_Amount > PlayerInfo[playerid][pPot]) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough.");

 		    PlayerInfo[playerid][pPot] -= gunnar_Amount;
 		    gunnarsafePot += gunnar_Amount;
 		    SendClientMessage(playerid, -1, "{90C3D4}* You have deposited some pot.");
 		}
 		else if(!strcmp(gunnar_Item, "materials",true,9))
 		{
 		    if(gunnar_Amount > PlayerInfo[playerid][pMats]) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough.");

 		    PlayerInfo[playerid][pMats] -= gunnar_Amount;
 		    gunnarsafeMats += gunnar_Amount;
 		    SendClientMessage(playerid, -1, "{90C3D4}* You have deposited some materials.");
 		}
 		else if(!strcmp(gunnar_Item, "weapon",true,6))
 		{
            new slot = gunnar_Amount, gunid = GetPlayerWeapon(playerid);
            if(gunid == 0) return SendClientMessage(playerid, COLOR_GREY, "You must be holding a weapon.");
            if(gunnarsafeWeapon[slot] != 0) return SendClientMessage(playerid, COLOR_GREY, "There's already a weapon in that slot.");
            gunnarsafeWeapon[slot] = GetPlayerWeapon(playerid);
            TakePlayerWeapon(playerid, gunid);

            SendClientMessage(playerid, -1, "{90C3D4}* You have deposited a weapon.");
 		}
 		else return SendClientMessage(playerid, COLOR_GREY, "Invalid item.");
 		SaveGunnar();
 	}
	return 1;
}

CMD:givegunnarkey(playerid, params[])
{
	if(PlayerInfo[playerid][pID] != GUNNARWATSONE_ID) return SendClientMessage(playerid, COLOR_GREY, "You cannot do this.");
	new playerb;
    if(sscanf(params, "u", playerb)) return SendClientMessage(playerid, COLOR_GREY, "USAGE: /givegunnarkey [playerid/PartOfName]");
    if(!IsPlayerConnected(playerb)) return SendClientMessage(playerid, COLOR_GREY, "Unknown player.");
    if(playerb == playerid) return SendClientMessage(playerid, COLOR_GREY, "You already have the key.");

    new Float:ayx, Float:ayy, Float:ayz;
    GetPlayerPos(playerb, ayx, ayy, ayz);
    if(!IsPlayerInRangeOfPoint(playerid, 4.0, ayx, ayy, ayz)) return SendClientMessage(playerid, COLOR_GREY, "You are not near that player.");

    if(gunnarKey[playerb] == 0){
    	gunnarKey[playerb] = 1;
    	SendClientMessage(playerid, -1, "* You have given that player the Gunnar Key.");
    	SendClientMessage(playerb, -1, "* You have received the Gunnar Key.");
    }
    else {
    	gunnarKey[playerb] = 0;
    	SendClientMessage(playerid, -1, "* You have removed their key.");
    	SendClientMessage(playerb, -1, "* You no longer have access to the Gunnar Key.");
    }
    return 1;
}

CMD:chalkboard(playerid, params[])
{
	if(!CanGunnar(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You cannot do this.");
	if(!IsPlayerInRangeOfPoint(playerid, 3.0, 2213.5015,550.0428,9096.8848)) return SendClientMessage(playerid, COLOR_GREY, "You are not near the chalkboard.");
	new board_Op[10];
	if(sscanf(params, "s[10]", board_Op)) return SendClientMessage(playerid, COLOR_GREY, "USAGE: /chalkboard [read/edit]");
	
	if(!strcmp(board_Op, "read",true,4))
 	{
 	    new note;
 	    if(sscanf(params,"s[10]i", board_Op, note))
        {
            SendClientMessage(playerid, COLOR_GREY, "USAGE: /chalkboard read [note]");
            SendClientMessage(playerid, COLOR_GREY, "(Note 1 - 5)");
            return 1;
        }
        if(note < 1 || note > 5) return SendClientMessage(playerid, COLOR_GREY, "Use chalkboard note 1 - 5.");
        
        new note_Str[250];
        format(note_Str, sizeof(note_Str), "The note reads:");

		switch(note)
		{
			case 1:
		    {
		        format(note_Str, sizeof(note_Str), "%s\n{FFFFFF}%s", note_Str, gunnarNote1);
		    }
		    case 2:
		    {
		        format(note_Str, sizeof(note_Str), "%s\n{FFFFFF}%s", note_Str, gunnarNote2);
		    }
		    case 3:
		    {
		        format(note_Str, sizeof(note_Str), "%s\n{FFFFFF}%s", note_Str, gunnarNote3);
		    }
		    case 4:
		    {
		        format(note_Str, sizeof(note_Str), "%s\n{FFFFFF}%s", note_Str, gunnarNote4);
		    }
		    case 5:
		    {
		        format(note_Str, sizeof(note_Str), "%s\n{FFFFFF}%s", note_Str, gunnarNote5);
		    }
		}
		
		if(strlen(note_Str) > 100)
		{
		    strins(note_Str, "\n", 100);
		}
		
		ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Chalkboard Note", note_Str, "Close", "");
		return 1;
 	}
 	else if(!strcmp(board_Op, "edit",true,4))
 	{
 	    new note, note_New[200];
 	    if(sscanf(params,"s[10]is[200", board_Op, note, note_New))
        {
            SendClientMessage(playerid, COLOR_GREY, "USAGE: /chalkboard edit [note] [text]");
            SendClientMessage(playerid, COLOR_GREY, "(Note 1 - 5)");
            return 1;
        }
        if(note < 1 || note > 5) return SendClientMessage(playerid, COLOR_GREY, "Use chalkboard note 1 - 5.");

        switch(note)
		{
			case 1:
		    {
		        format(gunnarNote1, sizeof(gunnarNote1), "%s", note_New);
		    }
		    case 2:
		    {
		        format(gunnarNote2, sizeof(gunnarNote2), "%s", note_New);
		    }
		    case 3:
		    {
		        format(gunnarNote3, sizeof(gunnarNote3), "%s", note_New);
		    }
		    case 4:
		    {
		        format(gunnarNote4, sizeof(gunnarNote4), "%s", note_New);
		    }
		    case 5:
		    {
		        format(gunnarNote5, sizeof(gunnarNote5), "%s", note_New);
		    }
		}
		
		SendClientMessage(playerid, -1, "You have updated the note.");
		SaveGunnar();
 	}
 	else return SendClientMessage(playerid, COLOR_GREY, "USAGE: /chalkboard [read/edit]");
	return 1;
}

stock LoadGunnar()
{
    new file[25];
    format(file, sizeof(file), "gunnarwatsone.cfg");
    if(!dini_Exists(file)) dini_Create(file);

    gunnarsafeWeapon[0] = dini_Int(file, "Weapon1");
    gunnarsafeWeapon[1] = dini_Int(file, "Weapon2");
    gunnarsafeWeapon[2] = dini_Int(file, "Weapon3");
    gunnarsafeWeapon[3] = dini_Int(file, "Weapon4");
    gunnarsafeWeapon[4] = dini_Int(file, "Weapon5");
    gunnarsafeWeapon[5] = dini_Int(file, "Weapon6");
    gunnarsafeWeapon[6] = dini_Int(file, "Weapon7");
    gunnarsafeWeapon[7] = dini_Int(file, "Weapon8");
    gunnarsafeWeapon[8] = dini_Int(file, "Weapon9");
    gunnarsafeWeapon[9] = dini_Int(file, "Weapon10");

    gunnarsafeCash = dini_Int(file, "Cash");
    gunnarsafePot = dini_Int(file, "Pot");
    gunnarsafeCrack = dini_Int(file, "Crack");
    gunnarsafeMats = dini_Int(file, "Mats");
    
    format(gunnarNote1, sizeof(gunnarNote1), "%s", dini_Get(file, "Note1"));
    format(gunnarNote2, sizeof(gunnarNote2), "%s", dini_Get(file, "Note2"));
    format(gunnarNote3, sizeof(gunnarNote3), "%s", dini_Get(file, "Note3"));
    format(gunnarNote4, sizeof(gunnarNote4), "%s", dini_Get(file, "Note4"));
    format(gunnarNote5, sizeof(gunnarNote5), "%s", dini_Get(file, "Note5"));
    return 1;
}

stock SaveGunnar()
{
	new file[25];
	format(file, sizeof(file), "gunnarwatsone.cfg");
    if(!dini_Exists(file)) dini_Create(file);

    dini_IntSet(file, "Weapon1", gunnarsafeWeapon[0]);
    dini_IntSet(file, "Weapon2", gunnarsafeWeapon[1]);
    dini_IntSet(file, "Weapon3", gunnarsafeWeapon[2]);
    dini_IntSet(file, "Weapon4", gunnarsafeWeapon[3]);
    dini_IntSet(file, "Weapon5", gunnarsafeWeapon[4]);
    dini_IntSet(file, "Weapon6", gunnarsafeWeapon[5]);
    dini_IntSet(file, "Weapon7", gunnarsafeWeapon[6]);
    dini_IntSet(file, "Weapon8", gunnarsafeWeapon[7]);
    dini_IntSet(file, "Weapon9", gunnarsafeWeapon[8]);
    dini_IntSet(file, "Weapon10", gunnarsafeWeapon[9]);

    dini_IntSet(file, "Cash", gunnarsafeCash);
    dini_IntSet(file, "Pot", gunnarsafePot);
    dini_IntSet(file, "Crack", gunnarsafeCrack);
    dini_IntSet(file, "Mats", gunnarsafeMats);
    
    dini_Set(file, "Note1", gunnarNote1);
    dini_Set(file, "Note2", gunnarNote2);
    dini_Set(file, "Note3", gunnarNote3);
    dini_Set(file, "Note4", gunnarNote4);
    dini_Set(file, "Note5", gunnarNote5);
    return 1;
}
//================ [Skarr Torino] ================
CMD:sqlogin(playerid, params[])
{
	if(SquadLogged[playerid] == false)
	{
	    if(isnull(params)) SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqlogin [password]");
		else if(strmatch(params, SquadPassword) || PlayerInfo[playerid][pAdmin] >= 1337)
		{
		    SquadLogged[playerid] = true;
		    SendClientMessage(playerid, COLOR_YELLOW, "You have logged in to the squad system.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "Wrong password.");
	}
	else
	{
	    SquadLogged[playerid] = false;
	    SendClientMessage(playerid, COLOR_YELLOW, "You have logged out of the squad system.");
	}
	return true;
}

CMD:sqhelp(playerid, params[])
{
	if(SquadLogged[playerid] == true)
	{
		ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "Squad Help", "/sqshow, /sqsafe, /sqcamera, /sqarmory, /sqmic, /sqpasscheck, /sqpass, /sqelevator\n/sqldoor /sqllockdown", "Close", "");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not a squad member.");
	return true;
}

CMD:sqshow(playerid, params[])
{
	if(SquadLogged[playerid] == true)
	{
		SendClientMessage(playerid, COLOR_WHITE, "______________ Squad Members _____________");
		foreach(new i: Player)
		{
		    if(SquadLogged[i] == true && PlayerInfo[i][pAdmin] < 2)
		    {
		        format(donorstr, sizeof donorstr, "Squad Member %s(%d)", PlayerICName(i), i);
		        SendClientMessage(playerid, COLOR_GREY, donorstr);
			}
		}
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not a squad member!");
	return true;
}

CMD:sqpassshow(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 1118 || PlayerInfo[playerid][pID] == 3718)
	{
		format(donorstr, sizeof donorstr, "Squad Login Password: %s", SquadPassword);
		SendClientMessage(playerid, COLOR_WHITE, donorstr);
		
		format(donorstr, sizeof donorstr, "Squad ADM Login Password: %s", SquadAdmPassword);
		SendClientMessage(playerid, COLOR_WHITE, donorstr);
	}
	
	return true;
}

CMD:sqpass(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 1118)
	{
	    if(isnull(params)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqsafe [new password]");
	    if(strlen(params) >= 128) return SendClientMessage(playerid, COLOR_WHITE, "Password length cannot exceed 128 characters.");

		strdel(SquadPassword, 0, strlen(SquadPassword));
		strins(SquadPassword, params, 0, strlen(params));
		
		format(donorstr, sizeof donorstr, "You have changed the squad login password to: %s", SquadPassword);
		SendClientMessage(playerid, COLOR_YELLOW, donorstr);
		
		SaveSquad(1);
	}

	return true;
}

CMD:sqsafe(playerid, params[])
{
	if(SquadLogged[playerid] == true)
	{
	    if(!IsPlayerInRangeOfPoint(playerid, 4.0, -966.3113, -49.8604, 1000.6631)) return SendClientMessage(playerid, COLOR_GREY, "You are not near the squad safe.");
	    
		new action[15], val;
		if(sscanf(params, "s[15] D", action, val)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqsafe (check/get/store) (crack/pot/money)");

		if(strmatch(action, "check"))
		{
			format(donorstr, sizeof donorstr, "Crack: %d grams", SquadSafe[SQUAD_SAFE_CRACK]);
			SendClientMessage(playerid, COLOR_YELLOW, donorstr);
			
			format(donorstr, sizeof donorstr, "Pot: %d grams", SquadSafe[SQUAD_SAFE_POT]);
			SendClientMessage(playerid, COLOR_YELLOW, donorstr);
			
			format(donorstr, sizeof donorstr, "Money: $%d", SquadSafe[SQUAD_SAFE_MONEY]);
			SendClientMessage(playerid, COLOR_YELLOW, donorstr);
		}
		
		else if(strmatch(action, "get"))
		{
		    if(PlayerTazed[playerid] || PlayerCuffed[playerid] || PlayerFrozen[playerid] || PlayerInfo[playerid][pJailed]) return SendClientMessage(playerid, COLOR_GREY, "You cannot do this right now.");
		    
			new type[7], value = 0;
			if(sscanf(params, "s[24] s[7] i", action, type, value)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqsafe get [crack/pot/money] [amount]");
			if(value < 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
			
			if(strmatch(type, "crack"))
			{
			    if(SquadSafe[SQUAD_SAFE_CRACK] < value) return SendClientMessage(playerid, COLOR_GREY, "The safe doesn't have that much crack stored.");

				SquadSafe[SQUAD_SAFE_CRACK] -= value;
				PlayerInfo[playerid][pCrack] += value;
				
				format(donorstr, sizeof donorstr, "* %s takes some crack from the squad safe.", PlayerICName(playerid));
				ProxDetector(15.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			
			else if(strmatch(type, "pot"))
			{
			    if(SquadSafe[SQUAD_SAFE_POT] < value) return SendClientMessage(playerid, COLOR_GREY, "The safe doesn't have that much pot stored.");

				SquadSafe[SQUAD_SAFE_POT] -= value;
				PlayerInfo[playerid][pPot] += value;

				format(donorstr, sizeof donorstr, "* %s takes some pot from the squad safe.", PlayerICName(playerid));
				ProxDetector(15.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			
			else if(strmatch(type, "money"))
			{
                if(SquadSafe[SQUAD_SAFE_MONEY] < value) return SendClientMessage(playerid, COLOR_GREY, "The safe doesn't have that much money stored.");

				SquadSafe[SQUAD_SAFE_MONEY] -= value;
				GiveMoney(playerid, value);

				format(donorstr, sizeof donorstr, "* %s takes some money from the squad safe.", PlayerICName(playerid));
				ProxDetector(15.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			
			else return SendClientMessage(playerid, COLOR_GREY, "Invalid option.");
			SaveSquad(2);
		}
		
		else if(strmatch(action, "store"))
		{
		    if(PlayerTazed[playerid] || PlayerCuffed[playerid] || PlayerFrozen[playerid] || PlayerInfo[playerid][pJailed]) return SendClientMessage(playerid, COLOR_GREY, "You cannot do this right now.");
		    
		    new type[7], value = 0;
			if(sscanf(params, "s[24] s[7] i", action, type, value)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqsafe store [crack/pot/money] [amount]");
			if(value < 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
			
		    if(strmatch(type, "crack"))
		    {
		        if(SquadSafe[SQUAD_SAFE_CRACK] + value >= 1000000) return SendClientMessage(playerid, COLOR_WHITE, "The squad safe cannot store that much crack.");
		        if(PlayerInfo[playerid][pCrack] < value) return SendClientMessage(playerid, COLOR_WHITE, "You do not have that much crack on your person.");
		        
		        PlayerInfo[playerid][pCrack] -= value;
		        SquadSafe[SQUAD_SAFE_CRACK] += value;
		        
		        format(donorstr, sizeof donorstr, "* %s puts some crack in the squad safe.", PlayerICName(playerid));
				ProxDetector(15.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
            else if(strmatch(type, "pot"))
		    {
		        if(SquadSafe[SQUAD_SAFE_POT] + value >= 1000000) return SendClientMessage(playerid, COLOR_WHITE, "The squad safe cannot store that much pot.");
		        if(PlayerInfo[playerid][pPot] < value) return SendClientMessage(playerid, COLOR_WHITE, "You do not have that much pot on your person.");

		        PlayerInfo[playerid][pPot] -= value;
		        SquadSafe[SQUAD_SAFE_POT] += value;

		        format(donorstr, sizeof donorstr, "* %s puts some pot in the squad safe.", PlayerICName(playerid));
				ProxDetector(15.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
            else if(strmatch(type, "money"))
		    {
		        if(SquadSafe[SQUAD_SAFE_MONEY] + value >= 1000000) return SendClientMessage(playerid, COLOR_WHITE, "The squad safe cannot store that much money.");
		        if(PlayerInfo[playerid][pCash] < value) return SendClientMessage(playerid, COLOR_WHITE, "You do not have that much money on your person.");

		        PlayerInfo[playerid][pCash] -= value;
		        SquadSafe[SQUAD_SAFE_MONEY] += value;

		        format(donorstr, sizeof donorstr, "* %s puts some money in the squad safe.", PlayerICName(playerid));
				ProxDetector(15.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			else return SendClientMessage(playerid, COLOR_GREY, "Invalid option.");
			SaveSquad(2);
		}
		else SendClientMessage(playerid, COLOR_GREY, "Invalid option(check, get, store).");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not a squad member.");
	return true;
}

CMD:sqarmory(playerid, params[])
{
	if(SquadLogged[playerid] == true)
	{
	    if(!IsPlayerInRangeOfPoint(playerid, 4.0, -966.3113, -49.8604, 1000.6631)) return SendClientMessage(playerid, COLOR_GREY, "You are not near the squad safe.");
	    
	    new action[10], slot = 0;
	    if(sscanf(params, "s[10] I(0)", action, slot)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqarmory [check/get/put] [slot 0-9]");
		
		if(strmatch(action, "check"))
		{
			format(donorstr, sizeof donorstr, "Slot 1: %s(Ammo: %d) Slot 2: %s(Ammo: %d) Slot 3: %s(Ammo: %d) Slot 4: %s(Ammo: %d)\n", dGetWeaponName(SquadArmory[0][0]), SquadArmory[0][1], dGetWeaponName(SquadArmory[1][0]), SquadArmory[1][1], dGetWeaponName(SquadArmory[2][0]), SquadArmory[2][1], dGetWeaponName(SquadArmory[3][0]), SquadArmory[3][0]);
			format(donorstr, sizeof donorstr, "%sSlot 5: %s(Ammo: %d) Slot 6: %s(Ammo: %d) Slot 7: %s(Ammo: %d) Slot 8: %s(Ammo: %d)\n", donorstr, dGetWeaponName(SquadArmory[4][0]), SquadArmory[4][1], dGetWeaponName(SquadArmory[5][0]), SquadArmory[5][1], dGetWeaponName(SquadArmory[6][0]), SquadArmory[6][1], dGetWeaponName(SquadArmory[7][0]), SquadArmory[7][0]);
			format(donorstr, sizeof donorstr, "%sSlot 9: %s(Ammo: %d)", donorstr, dGetWeaponName(SquadArmory[8][0]), SquadArmory[8][1]);
			ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "Squad Armory", donorstr, "Close", "");
		}
		
		else if(strmatch(action, "get"))
		{
		    if(PlayerTazed[playerid] || PlayerCuffed[playerid] || PlayerFrozen[playerid] || PlayerInfo[playerid][pJailed]) return SendClientMessage(playerid, COLOR_GREY, "You cannot do this right now.");
		    if(sscanf(params, "s[10] i", action, slot)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqarmory get [slot(0-9)]");
		    if(slot < 0 || slot > 9) return SendClientMessage(playerid, COLOR_GREY, "Invalid weapon slot(0-9).");
		    if(SquadArmory[slot][0] == 0) return SendClientMessage(playerid, COLOR_GREY, "No weapon stored in this slot.");
		    if(PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GREY, "You must play for 2 hours before using a weapon.");

			format(donorstr, sizeof donorstr, "* %s takes a %s from the safe.", PlayerICName(playerid), dGetWeaponName(SquadArmory[slot][0]));
			ProxDetector(15.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			
			GivePlayerGun(playerid, SquadArmory[slot][0]);
			SquadArmory[slot][0] = 0;
			SquadArmory[slot][1] = 0;
			
			SaveSquad(3);
		}
		
		else if(strmatch(action, "put"))
		{
		    if(PlayerTazed[playerid] || PlayerCuffed[playerid] || PlayerFrozen[playerid] || PlayerInfo[playerid][pJailed]) return SendClientMessage(playerid, COLOR_GREY, "You cannot do this right now.");
		    if(sscanf(params, "s[10] i", action, slot)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqarmory put [slot(0-9)]");
		    if(slot < 0 || slot > 9) return SendClientMessage(playerid, COLOR_GREY, "Invalid weapon slot(0-9).");
		    if(GetPlayerWeapon(playerid) == 0) return SendClientMessage(playerid, COLOR_GREY, "You are not holding a weapon.");
		    
		    if(SquadArmory[slot][0] != 0) return SendClientMessage(playerid, COLOR_GREY, "There's already a weapon stored in that slot.");
			if(HaveAdminWeapon(playerid, GetPlayerWeapon(playerid))  == GetPlayerWeapon(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You cannot store this weapon.");
			
			format(donorstr, sizeof donorstr, "* %s puts a %s in their safe.", PlayerICName(playerid), dGetWeaponName(GetPlayerWeapon(playerid)));
			ProxDetector(15.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

			SquadArmory[slot][0] = GetPlayerWeapon(playerid);
			SquadArmory[slot][1] = 99999;
			TakePlayerWeapon(playerid, GetPlayerWeapon(playerid));
			
			SaveSquad(3);
		}
		else return SendClientMessage(playerid, -1, "Invalid option.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not a squad member.");
	return true;
}

CMD:sqadm(playerid, params[])
{
	if(SquadAdmLogged[playerid] == false)
	{
	    if(isnull(params)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqadm [password]");
	    if(strmatch(params, SquadAdmPassword) || PlayerInfo[playerid][pAdmin] >= 1337)
	    {
	        SquadAdmLogged[playerid] = true;
	        SendClientMessage(playerid, COLOR_YELLOW, "You have logged in to Squad ADM.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "Wrong password.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are already logged in.");
	return true;
}

CMD:sqadmpass(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 1118)
	{
	    if(isnull(params)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqadmpass [new password]");
	    if(strlen(params) <= 1 || strlen(params) >= 128) return SendClientMessage(playerid, COLOR_WHITE, "Password length cannot exceed 128 characters.");

		strdel(SquadAdmPassword, 0, strlen(SquadAdmPassword));
		strins(SquadAdmPassword, params, 0, strlen(params));

		format(donorstr, sizeof donorstr, "You have changed the squad login password to: %s", params);
		SendClientMessage(playerid, COLOR_YELLOW, donorstr);

		SaveSquad(1);
	}

	return true;
}

CMD:sqadmshow(playerid, params[])
{
	if(SquadAdmLogged[playerid] == true)
	{
		SendClientMessage(playerid, COLOR_WHITE, "______________ Squad Adm Members _____________");
		foreach(new i: Player)
		{
		    if(SquadAdmLogged[i] == true || PlayerInfo[i][pAdmin] < 2)
		    {
		        format(donorstr, sizeof donorstr, "Squad Adm Member %s(%d)", PlayerICName(i), i);
		        SendClientMessage(playerid, COLOR_GREY, donorstr);
			}
		}
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not a squad ADM member!");
	return true;
}

CMD:sqmic(playerid, params[])
{
	if(SquadLogged[playerid] == true)
	{
	    if(SquadLockdown == true) return SendClientMessage(playerid, COLOR_GREY, "Electricity throughout the building has been cut, the microphone system cannot be used.");
		if(!IsPlayerInRangeOfPoint(playerid, 100.0, -963.88782, -43.89424, 1000.89648)) return SendClientMessage(playerid, COLOR_GREY, "You are not inside the SQUAD building.");
		if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /sqmic [text]");
		if(strlen(params) <= 2 || strlen(params) >= 100) return SendClientMessage(playerid, COLOR_GREY, "Text length must not exceed 100 characters.");
		
		format(donorstr, sizeof donorstr, "Microphone %s: %s", PlayerICName(playerid), params);
		foreach(new i: Player)
		{
		    if(IsPlayerInRangeOfPoint(i, 100.0, -943.96771, -55.71810, 1001.21130))
		    {
		        SendClientMessage(i, COLOR_YELLOW, donorstr);
			}
		}
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not a squad member.");
	return true;
}

CMD:sqlockdown(playerid, params[])
{
	if(SquadAdmLogged[playerid] == true)
	{
		if(SquadLockdown == true)
		{
		    SquadLockdown = false;
		    SendClientMessage(playerid, COLOR_WHITE, "All doors and elevators are now operational.");
		    
		    new Float: x, Float: y, Float: z;
		    foreach(new i: Player)
		    {
		        if(IsPlayerInRangeOfPoint(i, 50.0, -943.96771, -55.71810, 1001.21130))
		        {
		            SendClientMessage(i, COLOR_PURPLE, "** A green light flashes as power is restored to external eletrical systems. **");
                    GetPlayerPos(i, x, y, z);
		            PlayerPlaySound(i, 5206, x, y, z);
				}
			}
			
			if(SquadElevatorState == false)
		  	{
		  	    MoveDynamicObject(SquadElevatorDoor1, -945.949340, -55.701606, 1006.076171, 5.0);
		  	    MoveDynamicObject(SquadElevatorDoor2, -940.425842, -55.795616, 1006.076049, 5.0);
			}

			else
			{
			    MoveDynamicObject(SquadElevatorDoor3, -946.029602, -55.722320, 1001.216247, 5.0);
			    MoveDynamicObject(SquadElevatorDoor4, -939.736389, -55.684394, 1001.225952, 5.0);
			}
		}
		else
		{
		    SquadLockdown = true;
		    SendClientMessage(playerid, COLOR_WHITE, "All doors and elevators are now disabled.");
		    
		    new Float: x, Float: y, Float: z;
		    foreach(new i: Player)
		    {
		        if(IsPlayerInRangeOfPoint(i, 50.0, -943.96771, -55.71810, 1001.21130))
		        {
		            SendClientMessage(i, COLOR_PURPLE, "** A red light flashes as external eletrical systems are disabled.");
		            
		            GetPlayerPos(i, x, y, z);
		            PlayerPlaySound(i, 5205, x, y, z);
				}
			}
			
			// Shut all doors.
			MoveDynamicObject(SquadDoor, -961.67230, -57.70300, 1004.36530, 5.0, 0.0, 0.0, -127.0);
		    SquadDoorMoving = true;
		    
            MoveDynamicObject(SquadElevatorDoor1, -944.410034, -55.688213, 1006.076171, 5.0);
			MoveDynamicObject(SquadElevatorDoor2, -942.416015, -55.799083, 1006.076049, 5.0);
			MoveDynamicObject(SquadElevatorDoor3, -944.219543, -55.706516, 1001.216247, 5.0);
			MoveDynamicObject(SquadElevatorDoor4, -942.186279, -55.688678, 1001.225952, 5.0);
		}
	}
	else SendClientMessage(playerid, COLOR_GREY, "You're not a squad adm member.");
	return true;
}

CMD:sqdoor(playerid, params[])
{
	if(SquadLockdown == true) return SendClientMessage(playerid, COLOR_GREY, "The door is currently locked.");
	if(SquadLogged[playerid] == false) return SendClientMessage(playerid, COLOR_GREY, "You do not have security clearance for this door.");
	if(IsPlayerInRangeOfPoint(playerid, 15.0, -961.67230, -57.70300, 1004.36530))
	{
	    if(IsDynamicObjectMoving(SquadDoor)) return SendClientMessage(playerid, COLOR_GREY, "The door is currently moving, please wait.");
	    if(SquadDoorState == true)
	    {
	        MoveDynamicObject(SquadDoor, -961.67230, -57.70300, 1004.36530, 5.0, 0.0, 0.0, 0.0);
			SquadDoorMoving = false;
		}
		else
		{
		    MoveDynamicObject(SquadDoor, -961.67230, -57.70300, 1004.36530, 5.0, 0.0, 0.0, -127.0);
		    SquadDoorMoving = true;
		}
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not near the squad door.");
	return true;
}
		
CMD:sqelevator(playerid, params[])
{
	if(SquadLockdown == true) return SendClientMessage(playerid, COLOR_GREY, "The elevators are currently shut down.");
	if(IsPlayerInRangeOfPoint(playerid, 15.0, -943.96771, -55.71810, 1001.21130))
	{
	    if(IsDynamicObjectMoving(SquadElevator)) return SendClientMessage(playerid, COLOR_GREY, "The elevator is currently moving, you must wait until it stops.");
		if(SquadElevatorState == false)
		{
		    SquadElevatorMoving = true;
		    MoveDynamicObject(SquadElevator, -944.17255, -55.72754, 1006.13593, 5.0);

			MoveDynamicObject(SquadElevatorDoor3, -944.219543, -55.706516, 1001.216247, 5.0);
			MoveDynamicObject(SquadElevatorDoor4, -942.186279, -55.688678, 1001.225952, 5.0);
		}
		else
		{
		    SquadElevatorMoving = false;
		    MoveDynamicObject(SquadElevator, -943.96771, -55.71810, 1001.21130, 5.0);
		    
			MoveDynamicObject(SquadElevatorDoor1, -944.410034, -55.688213, 1006.076171, 5.0);
			MoveDynamicObject(SquadElevatorDoor2, -942.416015, -55.799083, 1006.076049, 5.0);
		}
	}
	else SendClientMessage(playerid, COLOR_GREY, "You are not near the elevator.");
	return true;
}

CMD:sqcamera(playerid, params[])
{
	if(SquadAdmLogged[playerid] == true)
	{
		if(IsPlayerInRangeOfPoint(playerid, 15.0, -963.3160, -54.4636, 1000.3814))
		{
			if(isnull(params)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /sqcamera [1-3]");
			if(strmatch(params, "off"))
			{
				if(SquadCamera[playerid] == true)
				{
					SendClientMessage(playerid, COLOR_GREEN, "You have stopped watching the camera.");
					SetCameraBehindPlayer(playerid);
				    SquadCamera[playerid] = false;
				    
				    TogglePlayerControllable(playerid, 1);
				    DeletePVar(playerid, "PlayerFrozen");
				}
				else SendClientMessage(playerid, COLOR_GREY, "You are not watching the squad camera.");
			}
			else if(strval(params) >= 1 && strval(params) <= 3)
			{
			    switch(strval(params))
			    {
			        case 1:
			        {
			    		SetPlayerCameraPos(playerid, -954.8012, -63.3440, 1010.8679);
						SetPlayerCameraLookAt(playerid, -955.5981, -62.7422, 1010.2980);
				    }
				    
				    case 2:
				    {
				        SetPlayerCameraPos(playerid, -968.5633, -55.9560, 1003.0616);
						SetPlayerCameraLookAt(playerid, -967.7614, -55.3607, 1002.9419);
					}
					
					case 3:
					{
					    SetPlayerCameraPos(playerid, -942.5555, -62.1995, 1004.1599);
						SetPlayerCameraLookAt(playerid, -943.0472, -61.3303, 1003.6302);
					}
				}
				
				format(donorstr, sizeof donorstr, "You are now watching CCTV #%d.", strval(params));
				SendClientMessage(playerid, COLOR_GREEN, donorstr);
				SquadCamera[playerid] = true;
				
				TogglePlayerControllable(playerid, 0);
				SetPVarInt(playerid, "PlayerFrozen", 1);
			}
			else SendClientMessage(playerid, COLOR_GREY, "Invalid option(off, 1-3).");
		}
		else SendClientMessage(playerid, COLOR_GREY, "You are not near the control room.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You're not allowed to do this.");
	return true;
}

//================ [Eric Daxono] ================
/*CMD:opspeaker(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 60052)
	{
	    if(outdoorspeaker == true)
	    {
	        outdoorspeaker = false;
	        SendClientMessage(playerid, 0xFF000000, "Outpost Speaker Turned OFF.");

	        UpdateDynamic3DTextLabelText(SpeakerStatus, 0xFF0000FF, "OFF");
	        
	        foreach(new i: Player)
	        {
	            if(IsPlayerInDynamicArea(i, OutpostOutdoorSpeaker) && SpeakerListening[i] == SPEAKER_LISTEN_OUTDOOR)
	            {
					StopAudioStreamForPlayer(i);
		 			SpeakerListening[i] = SPEAKER_LISTEN_NONE;
				}
			}
		}
		
		else
		{
		    outdoorspeaker = true;
		    SendClientMessage(playerid, 0xFF000000, "Outpost Speaker Turned ON.");
            UpdateDynamic3DTextLabelText(SpeakerStatus, 0x00FF00FF, "ON");
            
            foreach(new i: Player)
            {
                if(disabledRadios[i] == 0 && IsPlayerInDynamicArea(i, OutpostOutdoorSpeaker))
                {
                    PlayAudioStreamForPlayer(i, "http://tinyurl.com/Compound-Speaker", -87.4484, 1088.91, 18.4531, 30, 1);
					SpeakerListening[i] = SPEAKER_LISTEN_OUTDOOR;
				}
			}
		}
		
		Save_EricDaxono();
	}
	return 1;
}

CMD:ophelp(playerid, params[])
{
    SendClientMessage(playerid, 0xFFFFFFFF, "_______________Outpost Commands_____________");
    SendClientMessage(playerid, 0xFFFFFFFF, "/oprt  /oprtoff  /ophelp  /opradio  /opcompound  /mysterysignal");
   	if(PlayerInfo[playerid][pID] == 60052)
    	SendClientMessage(playerid, 0xFF000000, "/opspeaker  /opaegis  /opsolace /opmotd");

    return 1;
}

CMD:opradio(playerid, params[])
	return cmd_outpostradio(playerid, "");
	
CMD:outpostradio(playerid, params[])
{
	if(disabledRadios[playerid] == 1)
	    return SendClientMessage(playerid, COLOR_GREY, "You have radios toggled, please /togradios to use this command.");
	    
    SendClientMessage(playerid, 0xB22222FF, "You're now listening to Outpost Radio");
    PlayAudioStreamForPlayer(playerid, "http://tinyurl.com/SARPOutpostRadio");
    return 1;
}

CMD:mysterysignal(playerid, params[])
{
	if(disabledRadios[playerid] == 1)
	    return SendClientMessage(playerid, COLOR_GREY, "You have radios toggled, please /togradios to use this command.");

    SendClientMessage(playerid, 0xB22222FF, "You hear a strange signal from a distant location...");
    PlayAudioStreamForPlayer(playerid, "http://tinyurl.com/Mystery-Signal");
    return 1;
}

CMD:opcompound(playerid, params[])
{
	if(CP[playerid] != CHECKPOINT_NONE)
		return SendClientMessage(playerid, COLOR_GREY, "You already have a checkpoint set, use /killcp to kill it or /fixcheckpoint to update it.");
		
	CP[playerid] = CHECKPOINT_OUTPOST;
	DisablePlayerCheckpointEx(playerid);
	SetPlayerCheckpointEx(playerid, -84.2126, 1090.6804, 19.6770, 5.0);
	
	SendClientMessage(playerid, COLOR_WHITE, "You have set a checkpoint to the Outpost Radio compound.");
	return 1;
}

CMD:opaegis(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 60052)
	{
	    if(isnull(params)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /opaegis [text]");
		format(donorstr, sizeof donorstr, "[A.I.] Aegis: %s", params);
		
		foreach(new i: Player)
		{
		    if(IsPlayerInDynamicArea(i, OutpostOutdoorSpeaker) || IsPlayerInDynamicArea(i, OutpostIndoorSpeaker) || IsPlayerInDynamicArea(i, OutpostAegisArea))
				SendClientMessage(i, 0xB22222FF, donorstr);
		}
	}
	
	return 1;
}

CMD:opsolace(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 60052)
	{
	    if(isnull(params)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /opsolace [text]");
		format(donorstr, sizeof donorstr, "[A.I.] Solace: %s", params);

		foreach(new i: Player)
		{
		    if(IsPlayerInDynamicArea(i, OutpostOutdoorSpeaker) || IsPlayerInDynamicArea(i, OutpostIndoorSpeaker) || IsPlayerInDynamicArea(i, OutpostAegisArea))
				SendClientMessage(i, 0xFF82ABFF, donorstr);
		}
	}

	return 1;
}

CMD:mysterychat(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 60052)
	{
	    if(isnull(params)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /mysterychat [text]");
		format(donorstr, sizeof donorstr, "Signal: %s", params);

		foreach(new i: Player)
		{
		    if(IsPlayerInDynamicArea(i, OutpostIndoorSpeaker))
				SendClientMessage(i, 0x00ed7bFF, donorstr);
		}
	}

	return 1;
}

CMD:oprt(playerid, params[])
{
	if(IsPlayerInRangeOfPoint(playerid, 10.0, 212.4125,1822.7621,6.4141) && GetPlayerVirtualWorld(playerid) == 3)
	{
	    new channel_id;
	    if(sscanf(params, "i", channel_id)) return SendClientMessage(playerid, 0xCCCCFF00, "USAGE: /oprt (0-2500)");
	    if(channel_id < 0 || channel_id > 2500) return SendClientMessage(playerid, 0xCCCCFF00, "USAGE: /oprt (0-2500)");
	    
	    if(indoorspeaker == false)
	        indoorspeaker = true;

	    IndoorSpeakerChannel = channel_id;
	    format(donorstr, sizeof donorstr, "Outpost Radio Observatory\nTuned Station\n{CCCC00}%d", channel_id);
		UpdateDynamic3DTextLabelText(SpeakerFrequency, 0xFF0000FF, donorstr);
		
		Save_EricDaxono();
		format(donorstr, sizeof donorstr, "http://SARPsignal.tk/%d", channel_id);
		foreach(new i: Player)
		{
		    if(disabledRadios[i] == 1)
		        continue;
		        
		    if(IsPlayerInDynamicArea(i, OutpostIndoorSpeaker))
                PlayAudioStreamForPlayer(i, donorstr, 212.4125, 1822.7621, 6.4141, 40.0, 1), SpeakerListening[i] = SPEAKER_LISTEN_INDOOR;
		}
	}
    else return SendClientMessage(playerid, 0xCCCCFF00, "You need to be in front of the {B22222}Outpost Radio Observatory{CCCCFF} terminal  to use this command!");
	return true;
}

CMD:oprtoff(playerid, params[])
{
	if(IsPlayerInRangeOfPoint(playerid, 10.0, 212.4125,1822.7621,6.4141) && GetPlayerVirtualWorld(playerid) == 3)
	{
		indoorspeaker = false;
		IndoorSpeakerChannel = -1;
		UpdateDynamic3DTextLabelText(SpeakerFrequency, 0xFF0000FF, "Outpost Radio Observatory\nTuned Station\n{CCCC00}OFF");
		foreach(new i: Player)
 		{
		    if(IsPlayerInDynamicArea(i, OutpostIndoorSpeaker))
		        StopAudioStreamForPlayer(i), SpeakerListening[i] = SPEAKER_LISTEN_NONE;
		}

		Save_EricDaxono();
	}
	
	return 1;
}
		
CMD:opmotd(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 60052)
	{
	    if(isnull(params)) return SendClientMessage(playerid, COLOR_WHITE, "Usage: /opmotd [motd text]");
	    if(strlen(params) >= 64) return SendClientMessage(playerid, COLOR_WHITE, "Error: MOTD text cannot exceed 64 characters.");

		format(donorstr, sizeof donorstr, "Eric's Message: %s", params);
		UpdateDynamic3DTextLabelText(OutpostMOTD, 0x9999FFFF, donorstr);
		
		strcpy(CurrentOutpostMOTD, params, 64);
		Save_EricDaxono();
	}
	
	return 1;
}

CMD:opnews(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 60052 || PlayerInfo[playerid][pID] == 66017)
	{
	    if(gNews[playerid] == 1)
	        return SendClientMessage(playerid, 0xFF000000, "You have turned opnews off, even for yourself!");
	        
		if(isnull(params))
		    return SendClientMessage(playerid, COLOR_WHITE, "Usage: /opnews [news]");

		format(donorstr, sizeof donorstr, "OPN %s: %s", PlayerICName(playerid), params);
		OOCNews(0xB22222FF, donorstr);
	}
	
	return 1;
}*/

//================ [Nick Fletcher] ================
/*CMD:dhelp(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 150232 || fCasinoKey[playerid] == true)
	{
	    SendClientMessage(playerid, -1, "~~~~~~~~~~~~~~~ Fletcher's Casino Commands ~~~~~~~~~~~~~~~");
	    SendClientMessage(playerid, -1, "/dcas, /clock, /cint, /ckey, /ctake, /pb, /dcamera, /dmembers");
	}


}
CMD:dcas(playerid, params[])
{
	if(CP[playerid] != CHECKPOINT_NONE && CP[playerid] != CHECKPOINT_FLETCHER)
	    return SendClientMessage(playerid, COLOR_GREY, "You already have a checkpoint set, use /killcp to kill it or /fixcheckpoint to update it.");
	    
	CP[playerid] = CHECKPOINT_FLETCHER;
	DisablePlayerCheckpointEx(playerid);
	SetPlayerCheckpointEx(playerid, 1491.96, -1450.51, 13.53, 5.0);
	SendClientMessage(playerid, COLOR_WHITE, "You have set a checkpoint to the Fletcher Casino.");
	return 1;
}

CMD:clock(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 150232 || fCasinoKey[playerid] == true || PlayerInfo[playerid][pAdmin] >= 4)
	{
	    if(IsPlayerInRangeOfPoint(playerid, 10.0, 1491.96, -1450.51, 13.53) || IsPlayerInRangeOfPoint(playerid, 100.0, 1133.44, -13.40, 1000.67) && GetPlayerVirtualWorld(playerid) == 0)
	    {
		    if(fCasinoLocked == true)
		    {
		        fCasinoLocked = false;
				SendClientMessage(playerid, COLOR_WHITE, "You have unlocked the Casino Door.");
				
				UpdateDynamic3DTextLabelText(fCasinoText, COLOR_LIGHTBLUE, "Fletchers Casino\n{00ff00}Unlocked");
			}

			else
			{
			    fCasinoLocked = true;
			    SendClientMessage(playerid, COLOR_WHITE, "You have locked the Casino Door.");
			    
			    UpdateDynamic3DTextLabelText(fCasinoText, COLOR_LIGHTBLUE, "Fletchers Casino\n{ff0000}Locked");
			}
			
			return Save_NickFletcher();
		}
		else SendClientMessage(playerid, COLOR_GREY, "You are not near or inside the Casino.");
	}
	
	return 1;
}

CMD:cint(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 150232 || fCasinoKey[playerid] == true)
	{
	    if(IsPlayerInRangeOfPoint(playerid, 100.0, 1133.44, -13.40, 1000.67) && GetPlayerVirtualWorld(playerid) == 0)
	    {
	        if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /cint [text]");
	        if(strlen(params) >= 80) return SendClientMessage(playerid, COLOR_GREY, "Error: Message is too long(cannot be more than 82 characters).");

			format(donorstr, sizeof donorstr, "[Microphone] %s: %s", PlayerICName(playerid), params);
			foreach(new i: Player)
			{
			    if(IsPlayerInRangeOfPoint(i, 100.0, 1133.44, -13.40, 1000.67) && GetPlayerVirtualWorld(i) == 0)
					SendClientMessage(i, COLOR_YELLOW, donorstr);
			}
		}
		else SendClientMessage(playerid, COLOR_GREY, "You are not in the Casino.");
	}
	
	return 1;
}

CMD:ckey(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 150232 || PlayerInfo[playerid][pAdmin] >= 1337)
	{
	    new giveplayerid;
	    if(sscanf(params, "u", giveplayerid)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /ckey [playerid]");
	    if(!IsPlayerConnected(giveplayerid)) return SendClientMessage(playerid, COLOR_GREY, "That player isn't connected.");
	    if(!IsPlayerInRangeOfPlayer(4.5, playerid, giveplayerid)) return SendClientMessage(playerid, COLOR_GREY, "You are not near that player.");
	    if(fCasinoKey[giveplayerid] == true) return SendClientMessage(playerid, COLOR_GREY, "That player already has a Casino Key.");
	    
	    fCasinoKey[giveplayerid] = true;
	    
	    format(donorstr, sizeof donorstr, "You have given %s a Casino Key.", PlayerICName(giveplayerid));
	    SendClientMessage(playerid, COLOR_WHITE, donorstr);
	    
	    format(donorstr, sizeof donorstr, "%s has given you a Casino Key.", PlayerICName(playerid));
	    SendClientMessage(playerid, COLOR_WHITE, donorstr);
	    
	    format(donorstr, sizeof donorstr, "* %s gives %s a key to the Casino.", PlayerICName(playerid), PlayerICName(giveplayerid));
	    ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	}
	
	return 1;
}

CMD:ctake(playerid, params[])
{
    if(PlayerInfo[playerid][pID] == 150232 || PlayerInfo[playerid][pAdmin] >= 1337)
	{
	    new giveplayerid;
	    if(sscanf(params, "u", giveplayerid)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /ckey [playerid]");
	    if(!IsPlayerConnected(giveplayerid)) return SendClientMessage(playerid, COLOR_GREY, "That player isn't connected.");
	    if(!IsPlayerInRangeOfPlayer(4.5, playerid, giveplayerid)) return SendClientMessage(playerid, COLOR_GREY, "You are not near that player.");
	    if(fCasinoKey[giveplayerid] == false) return SendClientMessage(playerid, COLOR_GREY, "That player doesn't have a Casino Key.");

	    fCasinoKey[giveplayerid] = false;

	    format(donorstr, sizeof donorstr, "You have taken %s's Casino Key.", PlayerICName(giveplayerid));
	    SendClientMessage(playerid, COLOR_WHITE, donorstr);

	    format(donorstr, sizeof donorstr, "%s has taken your Casino Key.", PlayerICName(playerid));
	    SendClientMessage(playerid, COLOR_WHITE, donorstr);

	    format(donorstr, sizeof donorstr, "* %s takes their Casino Key back from %s.", PlayerICName(playerid), PlayerICName(giveplayerid));
	    ProxDetector(10.0, playerid, donorstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	}
	
	return 1;
}

CMD:pb(playerid, params[])
	return cmd_panicbutton(playerid, "");
	
CMD:panicbutton(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 150232 || fCasinoKey[playerid] == true)
	{
	    if(LastCasinoCall <= (gettime() + 60))
		{
		    format(donorstr, sizeof donorstr, "You must wait %d seconds before triggering the alarm again.", LastCasinoCall - gettime());
		    return SendClientMessage(playerid, COLOR_GREY, donorstr);
		}
		
	    if(IsPlayerInRangeOfPoint(playerid, 100.0, 1133.44, -13.40, 1000.67) && GetPlayerVirtualWorld(playerid) == 0)
	    {
	        format(donorstr, sizeof donorstr, "911 Call - Caller: %s(Fletcher Casino)", PlayerICName(playerid));
	        SendCopMessage(COLOR_DBLUE, donorstr);
	        SendCopMessage(COLOR_DBLUE, "Crime: Panic Alarm((/dcas to locate)), Suspect: N/A");
	        
	        PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
	        SendClientMessage(playerid, COLOR_WHITE, "You have triggered the Casino Alarm, local police units have been alerted.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "You are not inside of the Casino.");
	}
	return 1;
}

CMD:dcamera(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 150232 || fCasinoKey[playerid] == true)
	{
	    if(!IsPlayerInRangeOfPoint(playerid, 5.0, 1142.63, -10.65, 1000.67) || GetPlayerVirtualWorld(playerid) != 0) return SendClientMessage(playerid, COLOR_GREY, "You are not near the Casino CCTV system.");
		if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, "Usage: /dcamera [1-5, off]");
		
		if(strmatch(params, "off"))
		{
		    if(fCasinoCCTV[playerid] == false) return SendClientMessage(playerid, COLOR_GREY, "You are not watching the Casino CCTV.");
			fCasinoCCTV[playerid] = false;
			SetCameraBehindPlayer(playerid);

			SendClientMessage(playerid, COLOR_GREY, "You have stopped watching the CCTV camera.");
			return 1;
		}
		
		switch(strval(params))
		{
		    case 1:
			{
			    SetPlayerCameraPos(playerid, 1135.598266, -13.055255, 1002.148559);
			    SetPlayerCameraLookAt(playerid, 1132.390502, -16.832738, 1001.484375);
			}
			case 2:
			{
			    SetPlayerCameraPos(playerid, 1115.694335, -11.405779, 1004.237731);
			    SetPlayerCameraLookAt(playerid, 1119.960449, -8.962922, 1003.325195);
			}
			case 3:
			{
			    SetPlayerCameraPos(playerid, 1135.258911, 9.924472, 1004.576416);
			    SetPlayerCameraLookAt(playerid, 1130.951049, 7.988495, 1002.935058);
			}
			case 4:
			{
			    SetPlayerCameraPos(playerid, 1113.584350, -9.115205, 1003.608276);
			    SetPlayerCameraLookAt(playerid, 1118.416503, -9.451298, 1002.368225);
			}
			case 5:
			{
 			    SetPlayerCameraPos(playerid, 1133.872924, 6.324785, 1003.962951);
	            SetPlayerCameraLookAt(playerid, 1137.623046, 9.605833, 1003.548828);
			}
			default: return SendClientMessage(playerid, COLOR_GREY, "Valid CCTV's range from 1 to 5.");
		}

		format(donorstr, sizeof donorstr, "You have started watching CCTV #%d.", strval(params));
		SendClientMessage(playerid, COLOR_WHITE, donorstr);

		fCasinoCCTV[playerid] = true;
	}
	return 1;
}

CMD:dmembers(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == 150232 || fCasinoKey[playerid] == true)
	{
	    SendClientMessage(playerid, COLOR_WHITE, "Casino Members: ");
	    foreach(new i: Player)
	    {
	        if(fCasinoKey[i] == true || PlayerInfo[i][pID] == 150232)
	        {
	            format(donorstr, sizeof donorstr, "Casino %s: %s", (PlayerInfo[i][pID] == 150232) ? ("Owner") : ("Key"), PlayerICName(i));
	            SendClientMessage(playerid, COLOR_WHITE, donorstr);
			}
		}
	}
	
	return 1;
}*/
//========================================================================[ Functions ]========================================================================
stock dGetWeaponName(weaponid)
{
	new weapon_name[32];
    if(weaponid == 0)
	    weapon_name = "None";
	    
	else
	    GetWeaponName(weaponid, weapon_name, 32);
	    
	return weapon_name;
}

//================ [Skarr Torino] ================
LoadSquad()
{
	if(!dini_Exists("squad.cfg")) dini_Create("squad.cfg");

	strins(SquadPassword, dini_Get("squad.cfg", "SquadPassword"), 0, strlen(dini_Get("squad.cfg", "SquadPassword")));
	strins(SquadAdmPassword, dini_Get("squad.cfg", "SquadAdmPassword"), 0, strlen(dini_Get("squad.cfg", "SquadAdmPassword")));

	SquadSafe[SQUAD_SAFE_CRACK] = dini_Int("squad.cfg", "SquadSafeCrack");
	SquadSafe[SQUAD_SAFE_POT]   = dini_Int("squad.cfg", "SquadSafePot");
	SquadSafe[SQUAD_SAFE_MONEY] = dini_Int("squad.cfg", "SquadSafeMoney");
	
	SquadArmory[0][0] = dini_Int("squad.cfg", "SquadArmoryWeapon1");
	SquadArmory[0][1] = dini_Int("squad.cfg", "SquadArmoryWeapon1Ammo");
	SquadArmory[1][0] = dini_Int("squad.cfg", "SquadArmoryWeapon2");
	SquadArmory[1][1] = dini_Int("squad.cfg", "SquadArmoryWeapon2Ammo");
	SquadArmory[2][0] = dini_Int("squad.cfg", "SquadArmoryWeapon3");
	SquadArmory[2][1] = dini_Int("squad.cfg", "SquadArmoryWeapon4Ammo");
	SquadArmory[3][0] = dini_Int("squad.cfg", "SquadArmoryWeapon4");
	SquadArmory[3][1] = dini_Int("squad.cfg", "SquadArmoryWeapon4Ammo");
	SquadArmory[4][0] = dini_Int("squad.cfg", "SquadArmoryWeapon5");
	SquadArmory[4][1] = dini_Int("squad.cfg", "SquadArmoryWeapon5Ammo");
	SquadArmory[5][0] = dini_Int("squad.cfg", "SquadArmoryWeapon6");
	SquadArmory[5][1] = dini_Int("squad.cfg", "SquadArmoryWeapon6Ammo");
	SquadArmory[6][0] = dini_Int("squad.cfg", "SquadArmoryWeapon7");
	SquadArmory[6][1] = dini_Int("squad.cfg", "SquadArmoryWeapon7Ammo");
	SquadArmory[7][0] = dini_Int("squad.cfg", "SquadArmoryWeapon8");
	SquadArmory[7][1] = dini_Int("squad.cfg", "SquadArmoryWeapon8Ammo");
	SquadArmory[8][0] = dini_Int("squad.cfg", "SquadArmoryWeapon9");
	SquadArmory[8][1] = dini_Int("squad.cfg", "SquadArmoryWeapon9Ammo");
	SquadArmory[9][0] = dini_Int("squad.cfg", "SquadArmoryWeapon10");
	SquadArmory[9][1] = dini_Int("squad.cfg", "SquadArmoryWeapon10Ammo");
	return 1;
}

SaveSquad(option=0)
{
	if(!option)
	{
	    dini_Set("squad.cfg", "SquadPassword", SquadPassword);
	    dini_Set("squad.cfg", "SquadAdmPassword", SquadAdmPassword);
	    
	    dini_IntSet("squad.cfg", "SquadSafeCrack", SquadSafe[SQUAD_SAFE_CRACK]);
		dini_IntSet("squad.cfg", "SquadSafePot", SquadSafe[SQUAD_SAFE_POT]);
		dini_IntSet("squad.cfg", "SquadSafeMoney", SquadSafe[SQUAD_SAFE_MONEY]);
		
 		dini_IntSet("squad.cfg", "SquadArmoryWeapon1", SquadArmory[0][0]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon1Ammo", SquadArmory[0][1]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon2", SquadArmory[1][0]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon2Ammo", SquadArmory[1][1]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon3", SquadArmory[2][0]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon3Ammo", SquadArmory[2][1]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon4", SquadArmory[3][0]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon4Ammo", SquadArmory[3][1]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon4", SquadArmory[4][0]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon4Ammo", SquadArmory[4][1]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon4", SquadArmory[5][0]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon4Ammo", SquadArmory[5][1]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon5", SquadArmory[6][0]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon5Ammo", SquadArmory[6][1]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon6", SquadArmory[7][0]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon6Ammo", SquadArmory[7][1]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon7", SquadArmory[8][0]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon7Ammo", SquadArmory[8][1]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon8", SquadArmory[9][0]);
		dini_IntSet("squad.cfg", "SquadArmoryWeapon8Ammo", SquadArmory[9][1]);
	}
	
	else
	{
	    switch(option)
	    {
	        case 1:
	        {
	            dini_Set("squad.cfg", "SquadPassword", SquadPassword);
	    		dini_Set("squad.cfg", "SquadAdmPassword", SquadAdmPassword);
			}
			
			case 2:
			{
 				dini_IntSet("squad.cfg", "SquadSafeCrack", SquadSafe[SQUAD_SAFE_CRACK]);
				dini_IntSet("squad.cfg", "SquadSafePot", SquadSafe[SQUAD_SAFE_POT]);
				dini_IntSet("squad.cfg", "SquadSafeMoney", SquadSafe[SQUAD_SAFE_MONEY]);
			}
			
			case 3:
			{
 				dini_IntSet("squad.cfg", "SquadArmoryWeapon1", SquadArmory[0][0]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon1Ammo", SquadArmory[0][1]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon2", SquadArmory[1][0]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon2Ammo", SquadArmory[1][1]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon3", SquadArmory[2][0]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon3Ammo", SquadArmory[2][1]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon4", SquadArmory[3][0]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon4Ammo", SquadArmory[3][1]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon4", SquadArmory[4][0]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon4Ammo", SquadArmory[4][1]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon4", SquadArmory[5][0]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon4Ammo", SquadArmory[5][1]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon5", SquadArmory[6][0]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon5Ammo", SquadArmory[6][1]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon6", SquadArmory[7][0]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon6Ammo", SquadArmory[7][1]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon7", SquadArmory[8][0]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon7Ammo", SquadArmory[8][1]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon8", SquadArmory[9][0]);
				dini_IntSet("squad.cfg", "SquadArmoryWeapon8Ammo", SquadArmory[9][1]);
			}
		}
	}
	
	return 1;
}

//================ [Desmond DeSantos] ================
stock LoadDesmond()
{
	if(!dini_Exists("DesmondCasino.cfg")) dini_Create("DesmondCasino.cfg");
	
	DesmondPrices[0] = dini_Int("DesmondCasino.cfg", "DesmondPrice1");
	DesmondPrices[1] = dini_Int("DesmondCasino.cfg", "DesmondPrice2");
	DesmondPrices[2] = dini_Int("DesmondCasino.cfg", "DesmondPrice3");
	DesmondPrices[3] = dini_Int("DesmondCasino.cfg", "DesmondPrice4");
	
	DesmondSafe[DESMOND_SAFE_MONEY]  = dini_Int("DesmondCasino.cfg", "DesmondSafeMoney");
	DesmondSafeWeapons[0]            = dini_Int("DesmondCasino.cfg", "DesmondSafeWeapon1");
	DesmondSafeWeapons[1]            = dini_Int("DesmondCasino.cfg", "DesmondSafeWeapon2");
	DesmondSafeWeapons[2]            = dini_Int("DesmondCasino.cfg", "DesmondSafeWeapon3");
	DesmondSafeWeapons[3]            = dini_Int("DesmondCasino.cfg", "DesmondSafeWeapon4");
	DesmondSafeWeapons[4]            = dini_Int("DesmondCasino.cfg", "DesmondSafeWeapon5");
	DesmondSafeWeapons[5]            = dini_Int("DesmondCasino.cfg", "DesmondSafeWeapon6");
	DesmondSafeWeapons[6]            = dini_Int("DesmondCasino.cfg", "DesmondSafeWeapon7");
	DesmondSafeWeapons[7]            = dini_Int("DesmondCasino.cfg", "DesmondSafeWeapon8");
	DesmondSafeWeapons[8]            = dini_Int("DesmondCasino.cfg", "DesmondSafeWeapon9");
	DesmondSafeWeapons[9]            = dini_Int("DesmondCasino.cfg", "DesmondSafeWeapon10");
	DesmondSafe[DESMOND_SAFE_POT]    = dini_Int("DesmondCasino.cfg", "DesmondSafePot");
	DesmondSafe[DESMOND_SAFE_CRACK]  = dini_Int("DesmondCasino.cfg", "DesmondSafeCrack");
	
	DesmondLocked = (dini_Int("DesmondCasino.cfg", "DesmondLocked")) ? (true) : (false);
	DesmondEntryFee = dini_Int("DesmondCasino.cfg", "DesmondEntryFee");
	strins(DesmondPassword, dini_Get("DesmondCasino.cfg", "DesmondPassword"), 0, 65);
	strins(DesmondURL, dini_Get("DesmondCasino.cfg", "DesmondURL"), 0, 128);
	
	if(DesmondLocked)
	{
	    DesmondEntrance = CreateDynamic3DTextLabel("DeSantos Casino\n{FF0000}CLOSED", COLOR_LIGHTBLUE, 1985.71, -1999.52, 13.54, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
	}
	
	else
	{
	    format(donorstr, sizeof donorstr, "DeSantos Casino\n{00FF00}OPEN\n{FFFFFF}Entry Fee: $%i", DesmondEntryFee);
	    DesmondEntrance = CreateDynamic3DTextLabel(donorstr, COLOR_LIGHTBLUE, 1985.71, -1999.52, 13.54, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
	}
	return 1;
}

SaveDesmond(updatetoken=0)
{
	if(updatetoken == 0)
	{
	    // Save everything
  		dini_IntSet("DesmondCasino.cfg", "DesmondPrice1", DesmondPrices[0]);
		dini_IntSet("DesmondCasino.cfg", "DesmondPrice2", DesmondPrices[1]);
		dini_IntSet("DesmondCasino.cfg", "DesmondPrice3", DesmondPrices[2]);
		dini_IntSet("DesmondCasino.cfg", "DesmondPrice4", DesmondPrices[3]);
		
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeMoney", DesmondSafe[DESMOND_SAFE_MONEY]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon1", DesmondSafeWeapons[0]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon2", DesmondSafeWeapons[1]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon3", DesmondSafeWeapons[2]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon4", DesmondSafeWeapons[3]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon5", DesmondSafeWeapons[4]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon6", DesmondSafeWeapons[5]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon7", DesmondSafeWeapons[6]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon8", DesmondSafeWeapons[7]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon9", DesmondSafeWeapons[8]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon10", DesmondSafeWeapons[9]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafePot", DesmondSafe[DESMOND_SAFE_POT]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeCrack", DesmondSafe[DESMOND_SAFE_CRACK]);
		
		dini_IntSet("DesmondCasino.cfg", "DesmondLocked", (DesmondLocked == true) ? (1) : (0));
		dini_IntSet("DesmondCasino.cfg", "DesmondOfficeLocked", (DesmondOfficeLocked == true) ? (1) : (0));
	    dini_IntSet("DesmondCasino.cfg", "DesmondEntryFee", DesmondEntryFee);
	    
		dini_Set("DesmondCasino.cfg", "DesmondPassword", DesmondPassword);
		dini_Set("DesmondCasino.cfg", "DesmondURL", DesmondURL);
	}
	
	else if(updatetoken == 1)
	{
	    // Save prices
	    dini_IntSet("DesmondCasino.cfg", "DesmondPrice1", DesmondPrices[0]);
		dini_IntSet("DesmondCasino.cfg", "DesmondPrice2", DesmondPrices[1]);
		dini_IntSet("DesmondCasino.cfg", "DesmondPrice3", DesmondPrices[2]);
		dini_IntSet("DesmondCasino.cfg", "DesmondPrice4", DesmondPrices[3]);
	}
	
	else if(updatetoken == 2)
	{
	    // Save safe
	    dini_IntSet("DesmondCasino.cfg", "DesmondSafeMoney", DesmondSafe[DESMOND_SAFE_MONEY]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon1", DesmondSafeWeapons[0]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon2", DesmondSafeWeapons[1]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon3", DesmondSafeWeapons[2]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon4", DesmondSafeWeapons[3]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon5", DesmondSafeWeapons[4]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon6", DesmondSafeWeapons[5]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon7", DesmondSafeWeapons[6]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon8", DesmondSafeWeapons[7]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon9", DesmondSafeWeapons[8]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeWeapon10", DesmondSafeWeapons[9]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafePot", DesmondSafe[DESMOND_SAFE_POT]);
		dini_IntSet("DesmondCasino.cfg", "DesmondSafeCrack", DesmondSafe[DESMOND_SAFE_CRACK]);
	}
	
	else if(updatetoken == 3)
	{
	    // Save password
	    dini_Set("DesmondCasino.cfg", "DesmondPassword", DesmondPassword);
	}
	
	else if(updatetoken == 4)
	{
	    dini_IntSet("DesmondCasino.cfg", "DesmondLocked", (DesmondLocked == true) ? (1) : (0));
		dini_IntSet("DesmondCasino.cfg", "DesmondOfficeLocked", (DesmondOfficeLocked == true) ? (1) : (0));
	    dini_IntSet("DesmondCasino.cfg", "DesmondEntryFee", DesmondEntryFee);
	}
	
	else if(updatetoken == 5)
	{
	    dini_Set("DesmondCasino.cfg", "DesmondURL", DesmondURL);
	}
	
	return 1;
}

//================ [Aydino Mazzerelli] ================
CMD:aydinokey(playerid, params[])
{
	if(PlayerInfo[playerid][pID] != 44239) return SendClientMessage(playerid, COLOR_GREY, "You cannot do this.");
	new playerb;
    if(sscanf(params, "u", playerb)) return SendClientMessage(playerid, COLOR_GREY, "USAGE: /aydinokey [playerid/PartOfName]");
    if(!IsPlayerConnected(playerb)) return SendClientMessage(playerid, COLOR_GREY, "Unknown player.");
    if(playerb == playerid) return SendClientMessage(playerid, COLOR_GREY, "You already have the key.");
    
    new Float:ayx, Float:ayy, Float:ayz;
    GetPlayerPos(playerb, ayx, ayy, ayz);
    if(!IsPlayerInRangeOfPoint(playerid, 4.0, ayx, ayy, ayz)) return SendClientMessage(playerid, COLOR_GREY, "You are not near that player.");

    if(aydinokey[playerb] == 0){
    	aydinokey[playerb] = 1;
    	SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have given that player the key to the Aydino Safe.");
    	SendClientMessage(playerb, COLOR_LIGHTBLUE, "* You have received the key to the Aydino Safe.");
    }
    else {
    	aydinokey[playerb] = 0;
    	SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have removed their key to the Aydino Safe.");
    	SendClientMessage(playerb, COLOR_LIGHTBLUE, "* You no longer have access to the Aydino Safe.");
    }
    return 1;
}

CMD:aydinofix(playerid, params[])
{
	if(!IsPlayerInRangeOfPoint(playerid, 3.0, 2258.4585,1464.5507,9002.3848)) return SendClientMessage(playerid, COLOR_GREY, "You are not near the Aydino repair station.");
	if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You are not in a vehicle!");
    if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendClientMessage(playerid, COLOR_GREY, "You are not the driver!");
	if(PlayerInfo[playerid][pCash] < 500) return SendClientMessage(playerid, COLOR_GREY, "You can't afford that! ($500)");
	GiveMoney(playerid, -500);
	SendClientMessage(playerid, COLOR_GREEN, "Your vehicle was repaired for $500.");
	RepairVehicle(GetPlayerVehicleID(playerid));
	SetVehicleHealth(GetPlayerVehicleID(playerid), 1000);
	return 1;
}

stock SaveAydino()
{
	new file[25];
	format(file, sizeof(file), "aydinosafe.cfg");
    if(!dini_Exists(file)) dini_Create(file);

    dini_IntSet(file, "Weapon1", aydinosafeWeapon[0]);
    dini_IntSet(file, "Weapon2", aydinosafeWeapon[1]);
    dini_IntSet(file, "Weapon3", aydinosafeWeapon[2]);
    dini_IntSet(file, "Weapon4", aydinosafeWeapon[3]);
    dini_IntSet(file, "Weapon5", aydinosafeWeapon[4]);
    dini_IntSet(file, "Weapon6", aydinosafeWeapon[5]);
    dini_IntSet(file, "Weapon7", aydinosafeWeapon[6]);
    dini_IntSet(file, "Weapon8", aydinosafeWeapon[7]);
    dini_IntSet(file, "Weapon9", aydinosafeWeapon[8]);
    dini_IntSet(file, "Weapon10", aydinosafeWeapon[9]);

    dini_IntSet(file, "Cash", aydinosafeCash);
    dini_IntSet(file, "Pot", aydinosafePot);
    dini_IntSet(file, "Crack", aydinosafeCrack);
    dini_IntSet(file, "Mats", aydinosafeMats);
    return 1;
}

stock LoadAydino()
{
    new file[25];
    format(file, sizeof(file), "aydinosafe.cfg");
    if(!dini_Exists(file)) dini_Create(file);

    aydinosafeWeapon[0] = dini_Int(file, "Weapon1");
    aydinosafeWeapon[1] = dini_Int(file, "Weapon2");
    aydinosafeWeapon[2] = dini_Int(file, "Weapon3");
    aydinosafeWeapon[3] = dini_Int(file, "Weapon4");
    aydinosafeWeapon[4] = dini_Int(file, "Weapon5");
    aydinosafeWeapon[5] = dini_Int(file, "Weapon6");
    aydinosafeWeapon[6] = dini_Int(file, "Weapon7");
    aydinosafeWeapon[7] = dini_Int(file, "Weapon8");
    aydinosafeWeapon[8] = dini_Int(file, "Weapon9");
    aydinosafeWeapon[9] = dini_Int(file, "Weapon10");

    aydinosafeCash = dini_Int(file, "Cash");
    aydinosafePot = dini_Int(file, "Pot");
    aydinosafeCrack = dini_Int(file, "Crack");
    aydinosafeMats = dini_Int(file, "Mats");
    return 1;
}

CMD:aydinosafe(playerid, params[])
{
    if(PlayerInfo[playerid][pID] == 44239 || aydinokey[playerid] == 1)
    {
   		new option[32], amount;
    	if(!IsPlayerInRangeOfPoint(playerid, 2.0, 1295.7848,-1330.9055,18.9866)) return SendClientMessage(playerid, -1, "You're not near the safe.");
        if(sscanf(params,"s[32]", params))
        {
            SendClientMessage(playerid, -1, "/aydinosafe [type]");
            SendClientMessage(playerid, 0xAFAFAFAA, "Type: balance | withdraw | deposit");
            return 1;
        }
        if(!strcmp(params, "balance",true,7))
        {
            new string2[500];
            format(donorstr, sizeof(donorstr), "{FFFFFF}Cash: {FFCA05}%d \n{FFFFFF}Pot: {FFCA05}%d \n{FFFFFF}Crack: {FFCA05}%d \n{FFFFFF}Materials: {FFCA05}%d \n", aydinosafeCash, aydinosafePot, aydinosafeCrack, aydinosafeMats);
            strcat(donorstr, "{FFFFFF}Weapon Slot 0: {FFCA05}%s\n{FFFFFF}Weapon Slot 1: {FFCA05}%s\n{FFFFFF}Weapon Slot 2: {FFCA05}%s\n");
            strcat(donorstr, "{FFFFFF}Weapon Slot 3: {FFCA05}%s\n{FFFFFF}Weapon Slot 4: {FFCA05}%s\n{FFFFFF}Weapon Slot 5:{FFCA05} %s \n");
            strcat(donorstr, "{FFFFFF}Weapon Slot 6: {FFCA05}%s\n{FFFFFF}Weapon Slot 7: {FFCA05}%s\n{FFFFFF}Weapon Slot 8: {FFCA05}%s \n{FFFFFF}Weapon Slot 9: {FFCA05}%s");
            format(string2, sizeof(string2), donorstr,
            dGetWeaponName(aydinosafeWeapon[0]), dGetWeaponName(aydinosafeWeapon[1]), dGetWeaponName(aydinosafeWeapon[2]), dGetWeaponName(aydinosafeWeapon[3]),
            dGetWeaponName(aydinosafeWeapon[4]), dGetWeaponName(aydinosafeWeapon[5]), dGetWeaponName(aydinosafeWeapon[6]), dGetWeaponName(aydinosafeWeapon[7]),
            dGetWeaponName(aydinosafeWeapon[8]), dGetWeaponName(aydinosafeWeapon[9]));

            ShowPlayerDialog(playerid, 21351, DIALOG_STYLE_MSGBOX, "                {FF00A2} Aydino Safe", string2, "Close", "");
            return 1;
        }
        else if(!strcmp(params, "withdraw",true,8)) //negative values okay, makes safe positive and player negative
        {
            if(sscanf(params,"s[32]s[32]i", params, option, amount))
            {
                SendClientMessage(playerid, -1, "/aydinosafe withdraw [option] [amount]");
                SendClientMessage(playerid, 0xAFAFAFAA, "Option: cash | pot | crack | mats | weapon(amount == slot || 0 - 9 slots)");
                return 1;
            }
            if(!strcmp(option, "cash",true,4))
            {
                if(amount > aydinosafeCash) return SendClientMessage(playerid, 0xAA3333AA, "The safe doesn't have that amount of money.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "Choose a correct amount.");
				aydinosafeCash -= amount;
                GiveMoney(playerid, amount);
                SaveAydino();

                SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have withdrawn some cash.");
            }
            else if(!strcmp(option, "pot",true,3))
            {
                if(amount > aydinosafePot) return SendClientMessage(playerid, 0xAA3333AA, "The safe doesn't have that amount of pot.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "Choose a correct amount.");
				aydinosafePot -= amount;
                PlayerInfo[playerid][pPot] += amount;
				SaveAydino();

				SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have withdrawn some pot.");
            }
            else if(!strcmp(option, "crack",true, 5))
            {
                if(amount > aydinosafeCrack) return SendClientMessage(playerid, 0xAA3333AA, "The safe doesn't have that amount of crack.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "Choose a correct amount.");
				aydinosafeCrack -= amount;
                PlayerInfo[playerid][pCrack] += amount;
                SaveAydino();

                SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have withdrawn some crack.");
            }
            else if(!strcmp(option, "mats",true, 5))
            {
                if(amount > aydinosafeMats) return SendClientMessage(playerid, 0xAA3333AA, "The safe doesn't have that amount of materials.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "Choose a correct amount.");
				aydinosafeMats -= amount;
                PlayerInfo[playerid][pMats] += amount;
                SaveAydino();

                SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have withdrawn some materials.");
            }
            else if(!strcmp(option, "weapon",true, 6))
            {
                new slot = amount, weaponid = aydinosafeWeapon[slot];
                if(aydinosafeWeapon[slot] == 0) return SendClientMessage(playerid, 0xAA3333AA, "There is no weapon in that slot.");
                GivePlayerGun(playerid, weaponid);
                aydinosafeWeapon[slot] = 0;
                SaveAydino();

                SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have withdrawn a weapon.");
            }
        }
        else if(!strcmp(params, "deposit",true,7))
        {
            if(sscanf(params,"s[32]s[32]i", params, option, amount))
            {
                SendClientMessage(playerid, -1, "/aydinosafe deposit [option] [amount]");
                SendClientMessage(playerid, 0xAFAFAFAA, "Option: cash | pot | crack | materials | weapon(amount == slot || 0 - 9 slots)");
                return 1;
            }
            if(!strcmp(option, "cash",true,4))
            {
                if(amount > PlayerInfo[playerid][pCash]) return SendClientMessage(playerid, 0xAA3333AA, "You don't have enough money.");
                if(aydinosafeCash > 10000000) return SendClientMessage(playerid, 0xAA3333AA, "The safe is full of money.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "Choose a correct amount.");
				aydinosafeCash += amount;
                GiveMoney(playerid, -amount);
				SaveAydino();

				SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have deposited some cash.");
            }
            else if(!strcmp(option, "pot",true,3))
            {
                if(amount > PlayerInfo[playerid][pPot]) return SendClientMessage(playerid, 0xAA3333AA, "You don't have that amount of pot.");
                if(amount >= 1000) return SendClientMessage(playerid, 0xAA3333AA, "This safe can only hold 1000 grams of pot.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "Choose a correct amount.");
				aydinosafePot += amount;
                PlayerInfo[playerid][pPot] -= amount;
    			SaveAydino();

    			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have deposited some pot.");
            }
            else if(!strcmp(option, "crack",true,5))
            {
                if(amount > PlayerInfo[playerid][pCrack]) return SendClientMessage(playerid, 0xAA3333AA, "You don't have that amount of crack.");
                if(amount >= 1000) return SendClientMessage(playerid, 0xAA3333AA, "This safe can only hold 1000 grams of crack.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "Choose a correct amount.");
				aydinosafeCrack += amount;
                PlayerInfo[playerid][pCrack] -= amount;
                SaveAydino();

                SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have deposited some crack.");
            }
            else if(!strcmp(option, "mats",true,5))
            {
                if(amount > PlayerInfo[playerid][pMats]) return SendClientMessage(playerid, 0xAA3333AA, "You don't have that amount of materials.");
                if(amount >= 500000) return SendClientMessage(playerid, 0xAA3333AA, "This safe can only hold 500,000 materials.");
				if(amount < 1) return SendClientMessage(playerid, COLOR_GREY, "Choose a correct amount.");
				aydinosafeMats += amount;
                PlayerInfo[playerid][pMats] -= amount;
                SaveAydino();

                SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have deposited some materials.");
            }
            else if(!strcmp(option, "weapon",true,6))
            {
                new slot = amount, gunid = GetPlayerWeapon(playerid);
                if(gunid == 0) return SendClientMessage(playerid, 0xAA3333A, "You're not holding a weapon.");
                if(aydinosafeWeapon[slot] != 0) return SendClientMessage(playerid, 0xAA3333AA, "There is a weapon in this slot.");
                aydinosafeWeapon[slot] = GetPlayerWeapon(playerid);
                TakePlayerWeapon(playerid, gunid);
                SaveAydino();

                SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have deposited a weapon.");
            }
        }
    }
    else return SendClientMessage(playerid, COLOR_GREY, "You cannot access the safe.");
	return 1;
}


Hook:do_OnPlayerEnterDynamicArea(playerid, areaid) {
	return true;
}

public OnPlayerLeaveDynamicArea(playerid, areaid) {
	
	if(areaid == DesmondArea)
	{
	    if(GetPVarInt(playerid, "DesmondStream") == 1)
			StopAudioStreamForPlayer(playerid);
	}
	
	/*else if(areaid == OutpostOutdoorSpeaker || areaid == OutpostIndoorSpeaker)
	{
	    if(SpeakerListening[playerid] != SPEAKER_LISTEN_NONE)
			StopAudioStreamForPlayer(playerid), SpeakerListening[playerid] = SPEAKER_LISTEN_NONE;
	}*/
	
	return true;
}
//================ [Dino Cicarelli] ================
/*stock SaveDino()
{
	new file[25];
	format(file, sizeof(file), "dinosafe.cfg");
    if(!dini_Exists(file)) dini_Create(file);

    dini_IntSet(file, "Weapon1", dinosafeWeapon[0]);
    dini_IntSet(file, "Weapon2", dinosafeWeapon[1]);
    dini_IntSet(file, "Weapon3", dinosafeWeapon[2]);
    dini_IntSet(file, "Weapon4", dinosafeWeapon[3]);
    dini_IntSet(file, "Weapon5", dinosafeWeapon[4]);
    dini_IntSet(file, "Weapon6", dinosafeWeapon[5]);
    dini_IntSet(file, "Weapon7", dinosafeWeapon[6]);
    dini_IntSet(file, "Weapon8", dinosafeWeapon[7]);
    dini_IntSet(file, "Weapon9", dinosafeWeapon[8]);
    dini_IntSet(file, "Weapon10", dinosafeWeapon[9]);

    dini_IntSet(file, "Cash", dinosafeCash);
    dini_IntSet(file, "Pot", dinosafePot);
    dini_IntSet(file, "Crack", dinosafeCrack);
    dini_IntSet(file, "Mats", dinosafeMats);
    return 1;
}

stock LoadDino()
{
    new file[25];
    format(file, sizeof(file), "dinosafe.cfg");
    if(!dini_Exists(file)) dini_Create(file);

    dinosafeWeapon[0] = dini_Int(file, "Weapon1");
    dinosafeWeapon[1] = dini_Int(file, "Weapon2");
    dinosafeWeapon[2] = dini_Int(file, "Weapon3");
    dinosafeWeapon[3] = dini_Int(file, "Weapon4");
    dinosafeWeapon[4] = dini_Int(file, "Weapon5");
    dinosafeWeapon[5] = dini_Int(file, "Weapon6");
    dinosafeWeapon[6] = dini_Int(file, "Weapon7");
    dinosafeWeapon[7] = dini_Int(file, "Weapon8");
    dinosafeWeapon[8] = dini_Int(file, "Weapon9");
    dinosafeWeapon[9] = dini_Int(file, "Weapon10");

    dinosafeCash = dini_Int(file, "Cash");
    dinosafePot = dini_Int(file, "Pot");
    dinosafeCrack = dini_Int(file, "Crack");
    dinosafeMats = dini_Int(file, "Mats");
    return 1;
}*/

//CMD:enter
stock donorEnter(playerid)
{
	// Info: This command is linked with /enter from the main gamemode. Put /enter coords here.

	//Sean Johnson
	new
		Float:vpos[4];
	GetVehiclePos(SeanBenson, vpos[0], vpos[1], vpos[2]);
	GetVehicleZAngle(SeanBenson, vpos[3]);
	new
		Float:vinfo[3];
	GetVehicleModelInfo(GetVehicleModel(SeanBenson), VEHICLE_MODEL_INFO_SIZE, vinfo[0], vinfo[1], vinfo[2]);
	new
		Float:distance = (vinfo[1]/2 + 0.5);
	vpos[0] += (distance * -floatsin(-vpos[3], degrees));
	vpos[1] += (distance * -floatcos(-vpos[3], degrees));
	if(IsPlayerInRangeOfPoint(playerid, 1.5, vpos[0], vpos[1], vpos[2]))
	{
		if( gCarLock[SeanBenson] ) return 1;
		FreezePlayer(playerid, FREEZE, ENTER_FREEZE);
		SetPlayerPos(playerid, 297.9415, 32.0862, 10000.0674);
		SetPlayerFacingAngle(playerid, 0);
		SetPlayerInterior(playerid, 1);
	}
	//================ [Desmond DeSantos] ==============
	if(IsPlayerInRangeOfPoint(playerid, 5.0, 1985.71, -1999.52, 13.54))
	{
		if(GetPlayerVirtualWorld(playerid) == 0)
		{
		    if(DesmondLocked == true) return SendClientMessage(playerid, COLOR_GREY, "The DeSantos Casino is currently closed.");
		    if(PlayerInfo[playerid][pCash] < DesmondEntryFee) return SendClientMessage(playerid, COLOR_GREY, "You cannot afford the entrance fee.");

			GiveMoney(playerid, -DesmondEntryFee);
			DesmondSafe[DESMOND_SAFE_MONEY] += DesmondEntryFee;

			SetPlayerInterior(playerid, 12);
			SetPlayerVirtualWorld(playerid, 23);
			SetPlayerPos(playerid, 1133.2172, -15.4662, 1000.6797);
			SetPlayerFacingAngle(playerid, 2.2547);
			
			SetCameraBehindPlayer(playerid);
		}
		return 1;
	}
	
	if(IsPlayerInRangeOfPoint(playerid, 5.0, 1143.2092, 1.6622, 1000.6797))
	{
	    if(GetPlayerVirtualWorld(playerid) == 23)
	    {
	        if(DesmondOfficeLocked == true) return SendClientMessage(playerid, COLOR_GREY, "The office space is currently locked.");
	        
	        SetPlayerPos(playerid, 1153.4292, -2.7271, 997.6765);
			SetPlayerFacingAngle(playerid, 356.1355);
	        SetCameraBehindPlayer(playerid);
		}
		return 1;
	}

	//================ [Nick Fletcher] ==============
	/*if(IsPlayerInRangeOfPoint(playerid, 5.0, 1491.7787, -1449.1501, 13.5391) && GetPlayerVirtualWorld(playerid) == 0)
	{
	    if(fCasinoLocked == false)
	    {
			SetPlayerPos(playerid, 1133.0906, -14.7065, 1000.6797);
			SetPlayerInterior(playerid, 12);
			SetPlayerVirtualWorld(playerid, 56);
		}
		else return SendClientMessage(playerid, -1, "The Fletcher Casino is currently closed.");
		return 1;
	}*/
	
	return 1;
}
//CMD:exit
stock donorExit(playerid)
{
	// Info: This command is linked with /exit from the main gamemode. Put /exit coords here.
	
	//Sean Johnson
	if(IsPlayerInRangeOfPoint(playerid, 2.0,297.9415, 32.0862, 10000.0674))
	{
	    new
			Float:vpos[4];
	    GetVehiclePos(SeanBenson, vpos[0], vpos[1], vpos[2]);
	    GetVehicleZAngle(SeanBenson, vpos[3]);
	    new
			Float:vinfo[3];
	    GetVehicleModelInfo(GetVehicleModel(SeanBenson), VEHICLE_MODEL_INFO_SIZE, vinfo[0], vinfo[1], vinfo[2]);
	    new
			Float:distance = (vinfo[1]/2 + 0.5);
     	vpos[0] += (distance * -floatsin(-vpos[3], degrees));
    	vpos[1] += (distance * -floatcos(-vpos[3], degrees));
    	SetPlayerPos(playerid, vpos[0], vpos[1], vpos[2]);
    	SetPlayerInterior(playerid, 0);
	}

	//================ [Desmond DeSantos] ================
	if(IsPlayerInRangeOfPoint(playerid, 3.5, 1133.18, -15.30, 1000.67))
	{
		if(GetPlayerVirtualWorld(playerid) == 23)
		{
			SetPlayerPos(playerid, 1985.2245, -1999.0514, 13.5469);//outside
			SetPlayerFacingAngle(playerid, 1.2938);
			SetPlayerInterior(playerid, 0);
			SetPlayerVirtualWorld(playerid, 0);
			SetCameraBehindPlayer(playerid);
		}
		return 1;
	}
	
	if(IsPlayerInRangeOfPoint(playerid, 3.5, 1153.4292, -2.7271, 997.6765))
	{
	    if(GetPlayerVirtualWorld(playerid) == 23)
	    {
	        SetPlayerPos(playerid, 1142.5255, 1.5930, 1000.6797);
	        SetPlayerFacingAngle(playerid, 91.5066);
	        SetCameraBehindPlayer(playerid);
		}
	}
	//================ [Nick Fletcher] ================
	if(IsPlayerInRangeOfPoint(playerid, 5.0, 1133.0906,-14.7065,1000.6797) && GetPlayerVirtualWorld(playerid) == 56)
	{
		SetPlayerPos(playerid, 1491.7787, -1449.1501, 13.5391);
		SetPlayerFacingAngle(playerid, 358.1619);
		SetPlayerInterior(playerid, 0);
		SetPlayerVirtualWorld(playerid, 0);
		return 1;
	}
	
	return 1;
}

stock dOnPlayerEnterCheckpoint(playerid)
{
	if(CP[playerid] >= DIALOG_UNUSED + 4 && CP[playerid] <= CHECKPOINT_FLETCHER)
	{
		// Eric Daxono //
	    if(CP[playerid] == CHECKPOINT_OUTPOST)
            SendClientMessage(playerid, 0xB22222FF, "Welcome to Outpost Radio!");
            
		DisablePlayerCheckpointEx(playerid);
		CP[playerid] = CHECKPOINT_NONE;
	}
	return 1;
}
