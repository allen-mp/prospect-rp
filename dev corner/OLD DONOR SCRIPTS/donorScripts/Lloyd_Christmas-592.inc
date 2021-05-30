/*
 __      ___                       __     
/\ \    /\_ \                     /\ \    
\ \ \   \//\ \     ___   __  __   \_\ \   
 \ \ \  __\ \ \   / __`\/\ \/\ \  /'_` \  
  \ \ \L\ \\_\ \_/\ \L\ \ \ \_\ \/\ \L\ \ 
   \ \____//\____\ \____/\/`____ \ \___,_\
    \/___/ \/____/\/___/  `/___/> \/__,_ /
                             /\___/       
                             \/__/        
 ____     __                          __                                  
/\  _`\  /\ \             __         /\ \__                               
\ \ \/\_\\ \ \___   _ __ /\_\    ____\ \ ,_\   ___ ___      __      ____  
 \ \ \/_/_\ \  _ `\/\`'__\/\ \  /',__\\ \ \/ /' __` __`\  /'__`\   /',__\ 
  \ \ \L\ \\ \ \ \ \ \ \/ \ \ \/\__, `\\ \ \_/\ \/\ \/\ \/\ \L\.\_/\__, `\
   \ \____/ \ \_\ \_\ \_\  \ \_\/\____/ \ \__\ \_\ \_\ \_\ \__/.\_\/\____/
    \/___/   \/_/\/_/\/_/   \/_/\/___/   \/__/\/_/\/_/\/_/\/__/\/_/\/___/ 

	
	UNKNOWN DEVELOPMENT RERQUEST
*/
	
new knives = 0;
new xmas[5];
new bool:llcmas[5] = false,;
new secretstorage[4];
new secretdoor;
new knife[5];
new towerdoor[4];
new towerdoorstatus = 0;
new LloydSS = 0;
new LloydSD = 0;

Hook:LloydChristmas_OnGameModeInit() {
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
	return 1;
}

CMD:throwknife(playerid, params[]) {
	if(IsPlayerInRangeOfPoint(playerid,2,1538.4371, -1349.1216, 312.0913)) {
	    ApplyAnimation(playerid,"PED","fucku",4.0,0,0,0,0,0);
	    if(knives == 0) {
    		knives = 1;
     		MoveDynamicObject(knife[0], 1541.19958, -1343.50024, 313.28717,20);
   		}
    	else if(knives == 1) {
	       	knives = 2;
	       	MoveDynamicObject(knife[1], 1541.61938, -1343.89941, 313.57169,20);
	   	}
	   	else if(knives == 2) {
	       	knives = 3;
	       	MoveDynamicObject(knife[2], 1541.31763, -1343.65881, 313.68930,20);
	   	}
	   	else if(knives == 3) {
	       	knives = 4;
	       	MoveDynamicObject(knife[3], 1541.00818, -1343.36218, 313.52777,20);
	   	}
	   	else if(knives == 4) {
	       	knives = 5;
	       	MoveDynamicObject(knife[4], 1541.20789, -1343.76306, 312.95313,20);
	   	}
	   	else if(knives == 5) {
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

CMD:llxmas1(playerid, params[]) {
	if(IsPlayerInRangeOfPoint(playerid, 15.0, 1525.3603500, -1364.8575400, 35.0587000)) {
		if(llcmas[0] == false) {
	    	MoveDynamicObject(xmas[0], 1525.3603500,-1364.8575400,35.0587000, 2.0);
	    	llcmas[0] = true;

		}
		else if(llcmas[0] == true) {
	    	MoveDynamicObject(xmas[0], 1526.8753700,-1364.8575400,35.0587000,2.0);
	    	llcmas[0] = false;
		}
	}
	return 1;
}

CMD:llxmas2(playerid, params[]) {
	if(IsPlayerInRangeOfPoint(playerid, 15.0, 1526.8292200, -1364.8554700, 40.8157000)) {
		if(llcmas[1] == false) {
	    	MoveDynamicObject(xmas[1], 1526.8292200,-1364.8554700,40.8157000, 2.0);
	    	llcmas[1] = true;

		}
		else if(llcmas[1] == true) {
			MoveDynamicObject(xmas[1], 1525.3353300,-1364.8554700,40.8157000, 2.0);
			llcmas[1] = false;
		}
	}
	return 1;
}

CMD:llxmas3(playerid, params[]) {
	if(IsPlayerInRangeOfPoint(playerid, 15.0, 1526.8043200, -1364.8584000, 46.6117000)) {
		if(llcmas[2] == false) {
	    	MoveDynamicObject(xmas[2],1526.8043200,-1364.8584000,46.6117000, 2.0);
	    	llcmas[2] = true;

		}
		else if(llcmas[2] == true) {
	    	MoveDynamicObject(xmas[2], 1525.3293500,-1364.8584000,46.6117000, 2.0);
	    	llcmas[2] = false;
		}
	}
	return 1;
}

CMD:llcfreedom(playerid, params[]) {
	if(IsPlayerInRangeOfPoint(playerid, 15.0, 1520.8291000,-1358.6717500,40.4553000)) {
		if(llcmas[3] == false) {
	    	MoveDynamicObject(xmas[3], 1519.5900900,-1358.6717500,40.4553000, 2.0);
	    	llcmas[3] = true;

		}
		else if(llcmas[3] == true) {
	    	MoveDynamicObject(xmas[3], 1520.8291000,-1358.6717500,40.4553000,2.0);
	    	llcmas[3] = false;
		}
	}
	return 1;
}

CMD:llfinalsolution(playerid, params[]) {
	if(llcmas[4] == false) {
	    xmas[4] = CreateDynamicObject(2780,1525.4133300,-1352.7124000,40.3539000,0.0000000,0.0000000,0.0000000);
        llcmas[4] = true;
	}
	else if(llcmas[4] == true) {
		DestroyDynamicObjectEx(xmas[4]);
        llcmas[4] = false;
	}
	return 1;
}

CMD:christmasdoors(playerid, params[]) {
    if(towerdoorstatus == 0) {
        DestroyDynamicObjectEx(towerdoor[0]);
        DestroyDynamicObjectEx(towerdoor[1]);
   		towerdoor[2] = CreateDynamicObject(1491, 1547.88977, -1358.90906, 311.87177,   0.00000, 0.00000, 49.44001);
		towerdoor[3] = CreateDynamicObject(1491, 1543.84399, -1358.47437, 323.46375,   0.00000, 0.00000, -41.22001);
		towerdoorstatus = 1;
	}
	else {
	    DestroyDynamicObjectEx(towerdoor[2]);
	    DestroyDynamicObjectEx(towerdoor[3]);
	    towerdoor[0] = CreateDynamicObject(1504, 1547.88977, -1358.90906, 311.87177,   0.00000, 0.00000, 49.44001);
	    towerdoor[1] = CreateDynamicObject(1504, 1543.84399, -1358.47437, 323.46375,   0.00000, 0.00000, -41.22001);
	    towerdoorstatus = 0;
	}
	return 1;
}

CMD:lloyd1(playerid, params[]) {
    if(IsPlayerInRangeOfPoint(playerid,6,1541.9911, -1358.6097, 311.9182)) {
	    if(LloydSD == 0) {
  			LloydSD = 1;
   			MoveDynamicObject(secretdoor, 1544.6873, -1359.8224, 313.6179,0.8);
   		}
    	else if(LloydSD == 1) {
	       	LloydSD = 0;
	       	MoveDynamicObject(secretdoor, 1543.56836, -1358.80444, 313.61789,0.8);
	   	}
	}
	return 1;
}

CMD:lloyd2(playerid, params[]) {
    if(IsPlayerInRangeOfPoint(playerid,3,1539.8979, -1354.9958, 311.9212)) {
	    if(LloydSS == 0) {
   			LloydSS = 1;
   			MoveDynamicObject(secretstorage[0], 1540.8979, -1353.9958, 311.9212,0.8);
  			MoveDynamicObject(secretstorage[1], 1537.9476, -1357.0811, 311.9192,0.8);
       		MoveDynamicObject(secretstorage[2], 1538.0603, -1356.9464, 313.6119,0.8);
       		MoveDynamicObject(secretstorage[3], 1541.1069, -1353.7429, 313.6139,0.8);
       	}
	    else if(LloydSS == 1) {
		   	LloydSS = 0;
			MoveDynamicObject(secretstorage[0], 1539.89795, -1354.99585, 311.92120,0.8);
	       	MoveDynamicObject(secretstorage[1], 1538.94763, -1356.08105, 311.91919,0.8);
	       	MoveDynamicObject(secretstorage[2], 1539.06030, -1355.94641, 313.61191,0.8);
	       	MoveDynamicObject(secretstorage[3], 1540.10693, -1354.74292, 313.61389,0.8);
	   	}
	}
	return 1;
}