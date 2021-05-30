
enum RData
{
	rCASH,
	rMATS,
	rCRACKS,
	rPOTS,
	rGUNS[6]
}
new RinaldiData[RData];

stock RinaldiSave()
{
	new 
		File: SaveRFile =fopen(RinaldiFILE, io_write),
		FileStrg[128];
	format(FileStrg, sizeof(FileStrg), "%d|%d|%d|%d|%d|%d|%d|%d|%d",
			RinaldiData[rCASH],
			RinaldiData[rCRACKS],
			RinaldiData[rPOTS],
			RinaldiData[rMATS],
			RinaldiData[rGUNS][1],
			RinaldiData[rGUNS][2],
			RinaldiData[rGUNS][3],
			RinaldiData[rGUNS][4],
			RinaldiData[rGUNS][5]
			);

	fwrite(SaveRFile, FileStrg);
	fclose(SaveRFile);

}

/*
	LOADING DATA
*/
stock RinaldiLoad()
{
	new FileStrg[128],
		File: LoadRFile = fopen(RinaldiFILE, io_read);
	if(!fexist(RinaldiFILE)){
		printf("%s isn't located properly.", RinaldiFILE);
		printf("%s has been created", RinaldiFILE);
		return 1; }
	while( fread( LoadRFile, FileStrg )){
	sscanf(FileStrg, "p<|>iiiiiiiii",
			RinaldiData[rCASH],
			RinaldiData[rCRACKS],
			RinaldiData[rPOTS],
			RinaldiData[rMATS],
			RinaldiData[rGUNS][1],
			RinaldiData[rGUNS][2],
			RinaldiData[rGUNS][3],
			RinaldiData[rGUNS][4],
			RinaldiData[rGUNS][5]);	
	printf("%s has been opened;",RinaldiFILE);
	return fclose(LoadRFile);
}
	return 1;	
}


RinadliSafeDetection(playerid)
{
	if(PlayerInfo[playerid][pID] == 148156 || \
		PlayerInfo[playerid][pID] == 99849 || \
		RSafeACCESS[playerid] == 1 \
		&& IsPlayerInRangeOfPoint(playerid, 5, 1254.9937, -721.9808, 1089.7581)
		) return 1;
	//if()
	return 0;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	#define rPlayerPots PlayerInfo[playerid][pPot]
	#define rPlayerCracks PlayerInfo[playerid][pCrack]
	#define rPlayerMats PlayerInfo[playerid][pMats]
	new gunID = GetPlayerWeapon(playerid),
		strg[128];

	//DIALOG FOR DEPOSIT MONEY
	if(dialogid == DIALOGID_DEPOSITMONEY){
		if(response){
			//If the input isn't text, it will give error.
			
			new amount = strval(inputtext),
				string[128];

			if(!IsNumeric(inputtext)) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}You must put number.");

			if(strlen(inputtext) <= 0) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}Invalid amount.");

   			if(GetPlayerMoney(playerid) < amount) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}You don't have that much money on you.");
   			
   			if( RinaldiData[rCASH] == 500001 ) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}You cannot deposit more than 500k");
   			
   			if( (RinaldiData[rCASH] + amount) >= 500001 ) return SendClientMessage(playerid, -1, "You cannot deposit more than 500k");

   			//MAIN DATA MODIFY
		    RinaldiData[rCASH] += amount;
			GivePlayerMoney(playerid, -amount);

			//RP LINE
		    format(string, sizeof(string), "* %s has deposited $%d $ in their safe.", PlayerRPName(playerid), amount);
			//ProxDetector.(10, playerid, string, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

			//DEBUG 
			format(RinaldiDebug, sizeof(RinaldiDebug), "%s deposit %d money in %s", PlayerRPName(playerid), amount, RinaldiFILE);
			printf(RinaldiDebug);

		}
	}
	
	//DIALOG FOR DEPOSIT POT
	if(dialogid == DIALOGID_DEPOSIPOT){
		if(response){
			
			new amount = strval(inputtext),
				string[128];

			if(!IsNumeric(inputtext)) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}You must put number.");
			
			if(strlen(inputtext) <= 0) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}Invalid amount.");
   			
   			if( rPlayerPots < amount) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}You don't have that much money on you.");
   			
   			if( RinaldiData[rPOTS] == 2501 ) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}You cannot deposit more than 2500pots");
   			
   			if( (RinaldiData[rPOTS] + amount) >= 2501 ) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}You cannot deposit more than 2500pots");
   			
   			//MAIN DATA MODIFY
		    RinaldiData[rPOTS] += amount;
			rPlayerPots -= amount;

			//RP LINE


			//DEBUG 
			format(RinaldiDebug, sizeof(RinaldiDebug), "%s deposit %d pot(s) in %s", PlayerRPName(playerid), amount, RinaldiFILE);
			printf(RinaldiDebug);
		}
	}

	//DIALOG FOR DPOSIT CRACK
	if(dialogid == DIALOGID_DEPOSITCRACK){
			if(response){

			new amount = strval(inputtext),
				string[128];

			if(!IsNumeric(inputtext)) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}You must put number.");

			if(strlen(inputtext) <= 0) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}Invalid amount.");

   			if( rPlayerCracks < amount) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}You don't have that much money on you.");

   			if( RinaldiData[rCRACKS] == 5001 ) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}You cannot deposit more than 5000cracks");

   			if( (RinaldiData[rCRACKS] + amount) >= 5001 ) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}You cannot deposit more than 5000cracks");
   			
		    RinaldiData[rCRACKS] += amount;
			rPlayerCracks -= amount;

			//DEBUG 
			format(RinaldiDebug, sizeof(RinaldiDebug), "%s deposit %d crack(s) in %s", PlayerRPName(playerid), amount, RinaldiFILE);
			printf(RinaldiDebug);
		}
	}

	//DIALOG FOR DEPOSIT MATS
	if(dialogid == DIALOGID_DEPOSITMATS){
			if(response){
			
			new amount = strval(inputtext);
			new string[128];

			//using define to define pPot into small name. I don't see any reason to use varriable for that.

			if(!IsNumeric(inputtext)) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}You must put number.");

			if(strlen(inputtext) <= 0) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}Invalid amount.");
   			
   			if( rPlayerMats < amount) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}You don't have that much mats on you.");
   			
   			if( (RinaldiData[rMATS] + amount) >= 200001 ) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}You cannot deposit more than 200k mats");
   			
   			//MODIFY
		    RinaldiData[rMATS] += amount;
			rPlayerMats -= amount;

			//RP LINE
			//DEBUG 
			format(RinaldiDebug, sizeof(RinaldiDebug), "%s deposit %d Material(s) in %s", PlayerRPName(playerid), amount, RinaldiFILE);
			printf(RinaldiDebug);
		}
	}

	if(dialogid == DIALOGID_DEPOSITWEPS){
		if(response){
			switch(listitem){

				case 0: //Slot1
				{
					if(RinaldiData[rGUNS][1] != 0) return SendClientMessage(playerid, -1, "*ERROR: The slot isn't free.");
					if(gunID != 0)
					{
				        {
							TakePlayerWeapon(playerid);
							RinaldiData[rGUNS][1] = gunID;

					      	//debug
					      	format(RinaldiDebug, sizeof(RinaldiDebug), "%s deposit %d weapon in slot 1", PlayerRPName(playerid), gunID);
							printf(RinaldiDebug);
						}
					}
					else
					{
						SendClientMessage(playerid, -1, "You must hold a weapon for deposit.");
					}
					

				}
				case 1: //Slot 2
				{
					if(RinaldiData[rGUNS][2] != 0) return SendClientMessage(playerid, -1, "*ERROR: The slot isn't free.");
					if(gunID != 0)
					{
				        {
							TakePlayerWeapon(playerid);
							RinaldiData[rGUNS][2] = gunID;
	
					      	//debug
					      	format(RinaldiDebug, sizeof(RinaldiDebug), "%s deposit %d weapon in slot 2", PlayerRPName(playerid), gunID);
							printf(RinaldiDebug);
						}
					}
					else
					{
						SendClientMessage(playerid, -1, "You must hold a weapon for deposit.");
					}

				}
				case 2: //Slot 3
				{
					if(RinaldiData[rGUNS][3] != 0) return SendClientMessage(playerid, -1, "*ERROR: The slot isn't free.");
					if(gunID != 0)
					{
				        {
							TakePlayerWeapon(playerid);
							RinaldiData[rGUNS][3] = gunID;
	
					      	//debug
					      	format(RinaldiDebug, sizeof(RinaldiDebug), "%s deposit %d weapon in slot 3", PlayerRPName(playerid), gunID);
							printf(RinaldiDebug);
						}
					}
					else
					{
						SendClientMessage(playerid, -1, "You must hold a weapon for deposit.");
					}
				}
				case 3: //Slot 4
				{
					if(RinaldiData[rGUNS][4] != 0) return SendClientMessage(playerid, -1, "*ERROR: The slot isn't free.");
					if(gunID != 0)
					{
				        {
							TakePlayerWeapon(playerid);
							RinaldiData[rGUNS][4] = gunID;
	
					      	//debug
					      	format(RinaldiDebug, sizeof(RinaldiDebug), "%s deposit %d weapon in slot 4", PlayerRPName(playerid), gunID);
							printf(RinaldiDebug);
						}
					}
					else
					{
						SendClientMessage(playerid, -1, "You must hold a weapon for deposit.");
					}
				}
				case 4: //Slot 5
				{
					if(RinaldiData[rGUNS][5] != 0) return SendClientMessage(playerid, -1, "*ERROR: The slot isn't free.");
					if(gunID != 0)
					{
				        {
							TakePlayerWeapon(playerid);
							RinaldiData[rGUNS][5] = gunID;
		
					      	//debug
					      	format(RinaldiDebug, sizeof(RinaldiDebug), "%s deposit %d weapon in slot 5", PlayerRPName(playerid), gunID);
							printf(RinaldiDebug);
						}
					}
					else
					{
						SendClientMessage(playerid, -1, "You must hold a weapon for deposit.");
					}
				}
			}
		}
		
	}

	if(dialogid == DIALOGID_WITHDRAWMONEY){
			if(response){

			if(!IsNumeric(inputtext)) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}You must put number.");
			new amount = strval(inputtext);

			new string[128];
			if(strlen(inputtext) <= 0) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}Invalid amount.");
   			if(RinaldiData[rCASH] < amount) return SendClientMessage(playerid, -1, "Safe doesn't have enough money.");

		    RinaldiData[rCASH] -= amount;
			GivePlayerMoney(playerid, amount);
			

			//DEBUG 
			format(RinaldiDebug, sizeof(RinaldiDebug), "%s withdraw %d money from %s", PlayerRPName(playerid), amount, RinaldiFILE);
			printf(RinaldiDebug);

		}
		
	}
	if(dialogid == DIALOGID_WITHDRAWPOT){
			if(response){

			if(!IsNumeric(inputtext)) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}You must put number.");
			new amount = strval(inputtext);

			new string[128];
			if(strlen(inputtext) <= 0) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}Invalid amount.");
   			if(RinaldiData[rPOTS] < amount) return SendClientMessage(playerid, -1, "Safe doesn't have enough money.");

		    RinaldiData[rPOTS] -= amount;
			rPlayerPots += amount;
			

			//DEBUG 
			format(RinaldiDebug, sizeof(RinaldiDebug), "%s withdraw %d pot(s) from %s", PlayerRPName(playerid), amount, RinaldiFILE);
			printf(RinaldiDebug);

		}
		
	}
	if(dialogid == DIALOGID_WITHDRAWCRACK){
			if(response){

			if(!IsNumeric(inputtext)) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}You must put number.");
			new amount = strval(inputtext);

			new string[128];
			if(strlen(inputtext) <= 0) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}Invalid amount.");
   			if(RinaldiData[rCRACKS] < amount) return SendClientMessage(playerid, -1, "Safe doesn't have enough money.");

		    RinaldiData[rCRACKS] -= amount;
			rPlayerCracks += amount;
			
		
			//DEBUG 
			format(RinaldiDebug, sizeof(RinaldiDebug), "%s withdraw %d crack(s) from %s", PlayerRPName(playerid), amount, RinaldiFILE);
			printf(RinaldiDebug);

		}		
	}
	if(dialogid == DIALOGID_WITHDRAWMATS){
			if(response){

			if(!IsNumeric(inputtext)) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}You must put number.");
			new amount = strval(inputtext);

			new string[128];
			if(strlen(inputtext) <= 0) return SendClientMessage(playerid, -1, "{140000}*[ERROR]{FFFFFF}Invalid amount.");
   			if(RinaldiData[rMATS] < amount) return SendClientMessage(playerid, -1, "Safe doesn't have enough money.");

		    RinaldiData[rMATS] -= amount;
			rPlayerMats += amount;
			

			//DEBUG 
			format(RinaldiDebug, sizeof(RinaldiDebug), "%s withdraw %d material(s) from %s", PlayerRPName(playerid), amount, RinaldiFILE);
			printf(RinaldiDebug);

		}		
	}
	if(dialogid == DIALOGID_WITHDRAWWEPS){
		if(response){
			switch(listitem){
				case 0: //Slot1
				{
					if(RinaldiData[rGUNS][1] == 0) return SendClientMessage(playerid, -1, "*ERROR: The slot is empty.");
					GivePlayerGun(playerid, RinaldiData[rGUNS][1]);
					RinaldiData[rGUNS][1] = 0;
					//debug
					format(RinaldiDebug, sizeof(RinaldiDebug), "%s withdrawn %d weapon from slot 1", PlayerRPName(playerid), RinaldiData[rGUNS][1]);
					printf(RinaldiDebug);
				}
				case 1: //Slot 2
				{
					if(RinaldiData[rGUNS][2] == 0) return SendClientMessage(playerid, -1, "*ERROR: The slot is empty.");
					GivePlayerGun(playerid, RinaldiData[rGUNS][1]);
					RinaldiData[rGUNS][2] = 0;
					//debug
					format(RinaldiDebug, sizeof(RinaldiDebug), "%s withdrawn %d weapon from slot 2", PlayerRPName(playerid), RinaldiData[rGUNS][2]);
					printf(RinaldiDebug);
				}
				case 2: //Slot 3
				{
					if(RinaldiData[rGUNS][3] == 0) return SendClientMessage(playerid, -1, "*ERROR: The slot is empty.");
					GivePlayerGun(playerid, RinaldiData[rGUNS][1]);
					RinaldiData[rGUNS][3] = 0;
					//debug
					format(RinaldiDebug, sizeof(RinaldiDebug), "%s withdrawn %d weapon from slot 3", PlayerRPName(playerid), RinaldiData[rGUNS][3]);
					printf(RinaldiDebug);
				}
				case 3: //Slot 4
				{
					if(RinaldiData[rGUNS][4] == 0) return SendClientMessage(playerid, -1, "*ERROR: The slot is empty.");
					GivePlayerGun(playerid, RinaldiData[rGUNS][1]);
					RinaldiData[rGUNS][4] = 0;
					//debug
					format(RinaldiDebug, sizeof(RinaldiDebug), "%s withdrawn %d weapon from slot 4", PlayerRPName(playerid), RinaldiData[rGUNS][4]);
					printf(RinaldiDebug);
				}
				case 4: //Slot 5
				{
					if(RinaldiData[rGUNS][5] == 0) return SendClientMessage(playerid, -1, "*ERROR: The slot is empty.");
					GivePlayerGun(playerid, RinaldiData[rGUNS][1]);
					RinaldiData[rGUNS][5] = 0;
					//debug
					format(RinaldiDebug, sizeof(RinaldiDebug), "%s withdrawn %d weapon from slot 5", PlayerRPName(playerid), RinaldiData[rGUNS][5]);
					printf(RinaldiDebug);
				}
			}
		}
		
	}

	return 1;
}

