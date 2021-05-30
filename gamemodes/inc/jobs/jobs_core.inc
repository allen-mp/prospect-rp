/*
       _  ____  ____   _____ 
      | |/ __ \|  _ \ / ____|
      | | |  | | |_) | (___  
  _   | | |  | |  _ < \___ \ 
 | |__| | |__| | |_) |____) |
  \____/ \____/|____/|_____/ 
                             
                             
	
*/

enum // DONT USE - OUTDATED, NOT ACCURATE, USE DEFINES BELOW! still needed though, will fix & remove this in the future
{
	JOBID_DETECTIVE = 1,
	JOBID_LAWYER,
	JOBID_DRUGDEAL,
	JOBID_PRODDEAL,
	JOBID_CARJACK,
	JOBID_MECHANIC,
	JOBID_BODYGUARD,
	JOBID_ARMS,
	JOBID_THIEF,
	JOBID_FISHING,
	JOBID_BOXER,
	JOBID_TAXIDRIVER,
	JOBID_SMUGGLER,
	JOBID_MINER
};
#define DIALOG_JOBHELP_TRUCKER 3364
new jdialogid[MAX_PLAYERS];
new cargrass[7];
new CuttingGrass[MAX_PLAYERS];
forward GetPlayerJobLevel(playerid, job); // gets the player's job level
forward GetPlayerJobSkillLevel(playerid, job);  // gets the amount of X the player needs to do to level up, ex: needs to sell 3 guns to level up
forward GetPlayerJobSkill(playerid, job);

#define JOB_DETECTIVE 		1
#define JOB_LAWYER 			2
#define JOB_DRUGDEALER 		4
#define JOB_PRODUCTDEALER 	5
#define JOB_MECHANIC 		7
#define JOB_BODYGUARD 		8
#define JOB_ARMS 			9
#define JOB_BOXER 			12
#define JOB_TAXI 			14
#define JOB_SMUGGLER 		17
#define JOB_MINER 			18
#define JOB_WASTECOLLECTOR 	19
#define JOB_PIZZA			21
#define JOB_FARMER			22
#define JOB_GRASS			23
#define JOB_BUS				28
#define JOB_TRUCK			30

CMD:getjob(playerid, params[])
{
	if(PlayerInfo[playerid][pJob] != 0) return SendClientMessage(playerid, COLOR_GREY, "You already have a Job, use /quitjob first.");
	if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You must exit your vehicle before getting a Job.");

	if(IsPlayerInRangeOfPoint(playerid, 3.0, 256.6335, 64.0462, 1003.6406 +0.60) || IsPlayerInRangeOfPoint(playerid, 3.0, 1584.3629,-1668.6674,1106.1793))
	{
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* If you are sure you want to become a Detective, type /acceptjob.");
		GettingJob[playerid] = 1;
	}
	else if(IsPlayerInRangeOfPoint(playerid, 3.0, 1378.1117, -1096.1830, 25.1200) || IsPlayerInRangeOfPoint(playerid, 3.0, -2764.7473, 384.7114, 6.3281))
	{
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* If you are sure you want to become a Lawyer, type /acceptjob.");
		GettingJob[playerid] = 2;
	}
	else if(IsPlayerInRangeOfPoint(playerid, 3.0, 2164.1377, -1675.4172, 15.0859))
	{
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* If you are sure you want to become a Drug Dealer, type /acceptjob.");
		GettingJob[playerid] = 4;
	}
	else if(IsPlayerInRangeOfPoint(playerid, 3.0, 972.0325, -1385.7533, 13.5476))
	{
		if(PlayerInfo[playerid][pBizKey] != -1) //owns a business
			return SendClientMessage(playerid, COLOR_GREY, "Business owners cannot get this job, sorry.");
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* If you are sure you want to become a Product Dealer, type /acceptjob.");
		GettingJob[playerid] = 5;
	}
	else if(IsPlayerInRangeOfPoint(playerid, 3.0, 2329.4089, -2316.0996, 13.5469) || IsPlayerInRangeOfPoint(playerid, 3.0, -2714.6499, 210.9217, 4.3281))
	{
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* If you are sure you want to become a Mechanic, type /acceptjob.");
		GettingJob[playerid] = 7;
	}
	else if(IsPlayerInRangeOfPoint(playerid, 3.0, 2243.0278,-1715.1628,13.6365) || IsPlayerInRangeOfPoint(playerid, 3.0, -1551.8896, 1154.8134, 7.1875) || IsPlayerInRangeOfPoint(playerid, 3.0, 2227.0664, -1718.9178, 13.5245))
	{
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* If you are sure you want to become a Bodyguard, type /acceptjob.");
		GettingJob[playerid] = 8;
	}
	else if(IsPlayerInRangeOfPoint(playerid, 3.0, 1366.4325, -1275.2096, 13.5469))
	{
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* If you are sure you want to become a Arms Dealer, type /acceptjob.");
		GettingJob[playerid] = 9;
	}
	else if(IsPlayerInRangeOfPoint(playerid, 3.0, 241.8917, -184.6301, 1.5781)) //blueberry
	{
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* If you are sure you want to become a Arms Dealer, type /acceptjob.");
		GettingJob[playerid] = 9;
	}
	else if(IsPlayerInRangeOfPoint(playerid, 3.0, 756.7493, 6.3762, 1000.7001))
	{
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* If you are sure you want to become a Boxer, type /acceptjob.");
		GettingJob[playerid] = 12;
	}
	else if(IsPlayerInRangeOfPoint(playerid, 3.0, 1741.7062, -1863.6664, 13.5748) || IsPlayerInRangeOfPoint(playerid, 3.0, -1982.0994, 131.8526, 27.6875))
	{
		/*SendClientMessage(playerid, COLOR_LIGHTBLUE, "* If you are sure you want to become a Taxi Driver, type /acceptjob.");
		GettingJob[playerid] = 14;*/
		SendClientMessage(playerid, COLOR_GREY, "We are sorry but this job is currently disabled.");
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "Hint: {FFFFFF}If you wish to become a taxi driver please join Taxi Company.");
	}
	else if(IsPlayerInRangeOfPoint(playerid, 3.0, 2354.2703, -1169.3293, 28.0083))
	{
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* If you are sure you want to become a Drug Smuggler, type /acceptjob.");
		GettingJob[playerid] = 17;
	}
	else if(IsPlayerInRangeOfPoint(playerid, 3.0, 1015.9748, -311.7639, 74.5135))
	{
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* If you are sure you want to become a Miner, type /acceptjob.");
		GettingJob[playerid] = 18;
	}
	else if(IsPlayerInRangeOfPoint(playerid, 3.0, 165.7221,-280.6324,1.9747))
	{
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* If you are sure you want to become a Waste Collector, type /acceptjob.");
		GettingJob[playerid] = 19;
	}
	else if(IsPlayerInRangeOfPoint(playerid, 3.0, 2104.51, -1803.17, 13.55))
	{
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* If you are sure you want to become a pizza delivery boy, type /acceptjob.");
		GettingJob[playerid] = 21;
	}
	else if(IsPlayerInRangeOfPoint(playerid, 3, -40.0612, 26.5413, 3.1094))
	{
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* If you are sure you want to become a farmer, type /acceptjob.");
		GettingJob[playerid] = JOB_FARMER;	
	}
	else if(IsPlayerInRangeOfPoint(playerid, 3, 1961.1292,-1244.8138,19.9714))
	{
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* If you are sure you want to become a grass mower, type /acceptjob.");
		GettingJob[playerid] = JOB_GRASS;	
	}
    else if(IsPlayerInRangeOfPoint(playerid, 3, 2141.65, -1176.11, 23.99)) 
	{
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* If you are sure you want to become a bus driver, type /acceptjob.");
		GettingJob[playerid] = JOB_BUS;	
	}
    else if(IsPlayerInRangeOfPoint(playerid, 3, 1767.27, -2040.56, 13.52))
	{
		if(PlayerInfo[playerid][pTruckLicense] != 1) return SendClientMessage(playerid, COLOR_LIGHTBLUE, "You need a Trucker License to have this job.");
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* If you are sure you want to become a truck driver, type /acceptjob.");
		GettingJob[playerid] = JOB_TRUCK;	
	}
	else
	{
		SendClientMessage(playerid, COLOR_GREY, "You are not near a place to get a Job.");
	}

	return 1;
}

CMD:quitjob(playerid, params[])
{

	if(PlayerInfo[playerid][pJob] == 0) return SendClientMessage(playerid, COLOR_GREY, "You don't have a Job to quit.");

	if(PlayerInfo[playerid][pJob] == 17 && CP[playerid] != CHECKPOINT_NONE)
	{
		CP[playerid] = CHECKPOINT_NONE;
		DisablePlayerCheckpointEx(playerid);
	}

	SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You quit your Job.");
	PlayerInfo[playerid][pJob] = 0;

	return 1;

}

CMD:acceptjob(playerid, params[])
{

	if(GettingJob[playerid] < 1) return SendClientMessage(playerid, COLOR_GREY, "You haven't been offered a job.");
	
	SendClientMessage(playerid, COLOR_LIGHTBLUE, "* Congratulations with your new job!");
	PlayerInfo[playerid][pJob] = GettingJob[playerid];
	GettingJob[playerid] = 0;
	switch(PlayerInfo[playerid][pJob])
	{
		case 5: SendClientMessage(playerid, COLOR_WHITE, "HINT: You can find products in front of the discount warehouse.");
		case 9: SendClientMessage(playerid, COLOR_WHITE, "HINT: You can find materials packages behind ammunations.");
		case 17:
		{
			SendClientMessage(playerid, COLOR_WHITE, "HINT: You can find drug crates at the Drug Lab.");
			if(CP[playerid] != CHECKPOINT_NONE)
			{
				CP[playerid] = CHECKPOINT_NONE;
				DisablePlayerCheckpointEx(playerid);
			}
			SetPlayerCheckpointEx(playerid, 2411.9382, -2598.7937, 14.2131, 3.0); //drug factory
			CP[playerid] = CHECKPOINT_DRUGFACTORY;
		}
		case 19: SendClientMessage(playerid, COLOR_WHITE, "HINT: You can get in a garbage truck to start your a job or ride on someone elses.");
        case 28: SendClientMessage(playerid, COLOR_WHITE, "HINT: Get into a bus at bus depot, type /busfare inside it to start your job.");
        case 30: SendClientMessage(playerid, COLOR_WHITE, "HINT: You can use only Mule for deliveries. Anyone can rob your cargo, so be careful.");
	}
	return 1;
}

CMD:jobhelp(playerid, params[])
{
	if(goingJob[playerid] == true)
	{
		DisablePlayerCheckpointEx(playerid);
		goingJob[playerid] = false;
	}
	ShowPlayerDialog(playerid, DIALOG_JOBHELP, DIALOG_STYLE_LIST, "Job Help", "Detective\nLawyer\nDrug Dealer\nMechanic\n{00ff00}Bodyguard\n{FFFFFF}Arms Dealer\nTaxi Driver\n{00ff00}Drug Smuggler\n{FFFFFF}Waste Collector\n{00ff00}Pizza Deliver\n{00ff00}Farmer\n{00ff00}Grass Mower\n{00ff00}Bus Driver\n{00ff00}Miner\n{00ff00}Trucker", "Select", "Cancel");
	return 1;
}

CMD:jobinfo(playerid, params[])
{

	return 1;
}

Hook:Jobs_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch(dialogid) {
		case DIALOG_JOBHELP:
		{
			if(!response)
				return ShowHelpDialog(playerid);

			new mstring[1500],
				titlestring[32],
				line1[512],
				line2[512],
				line3[256];

			switch(listitem) {
				case 0: //Detective
				{
					format(titlestring, sizeof(titlestring), "Detective");
					format(line1, sizeof(line1), "{007BD0}Information:\n{B4B5B7}This job can be used to locate people anywhere around San Andreas.\nThis is helpful for the Government factions, and can be used to find criminals, it is a legal job and you cannot get busted for doing it.\n It will tell you the last known Location(area) they were found in and give you a beacon.");
					format(line2, sizeof(line2), "{007BD0}\nSkills:\n{B4B5B7}Level 1: You can find someone for 3 seconds, the reload time is 2 minutes.\nLevel 2: You can find someone for 5 seconds, the reload time is 1 minute, 20 seconds.\nLevel 3: You can find someone for 7 seconds, the reload time is 1 minute.\nLevel 4: You can find someone for 9 seconds, the reload time is 30 seconds.\nLevel 5: You can find someone for 11 seconds, the reload time is 20 seconds.");
					format(line3, sizeof(line3), "{007BD0}\nCommands:\n{B4B5B7}/find, /prisonlist\n/find is a command that can locate a player's position.\n\n{007BD0}Location Of Job:\n{B4B5B7}This job can be obtained inside the San Andreas Police Department.");
					jdialogid[playerid] = DIALOG_JOBHELP_DETECTIVE;
				}
				case 1: //Lawyer
				{
					format(titlestring, sizeof(titlestring), "Lawyer");
					format(line1, sizeof(line1), "{007BD0}Information:\n{B4B5B7}This job can be used to remove wanted stars, reduce jail time, and list all Criminals.\nThis is helpful for criminals and crooks, it is a legal job and you cannot get busted for doing it.\nIt will come in handy for your friends and families who got trouble with the cops.");
					format(line2, sizeof(line2), "{007BD0}\nSkills:\n{B4B5B7}Level 1: You can reduce inmates sentences by 1 minute.\nLevel 2: You can reduce inmates sentences by 2 minutes.\nLevel 3: You can reduce inmates sentences by 3 minutes.\nLevel 4: You can reduce inmates sentences by 4 minutes.\nLevel 5: You can reduce inmates sentences by 5 minutes.\nThe reload time is always 2 minutes no matter what level.");
					format(line3, sizeof(line3), "{007BD0}\nCommands:\n{B4B5B7}/defend, /free, /wanted, /duty, /prisonlist.\n\n{007BD0}Location of Job:\n{B4B5B7}This job can be located at the job icon(yellow circle) near the bank.");
					jdialogid[playerid] = DIALOG_JOBHELP_LAWYER;
				}
				case 2: //Drug Dealer
				{
					format(titlestring, sizeof(titlestring), "Drug Dealer");
					format(line1, sizeof(line1), "{007BD0}Information:\n{B4B5B7}This job can be used to sell cannabis to any customers you might find.\nThis is an illegal job and you can get busted for doing it.");
					//format(line2, sizeof(line2), "{007BD0}\nSkills:\n{B4B5B7}Level 1: You can hold 10 pot and 5 crack.\nLevel 2: You can hold 20 pot and 10 crack.\nLevel 3: You can hold 30 pot and 15 crack.\nLevel 4: You can hold 40 pot and 20 crack.\nLevel 5: You can hold 50 pot and 25 crack.\nThe reload time is always 1 minute no matter what level.");
					format(line2, sizeof(line2), "\n{B4B5B7}/drughelp for more commands and their usage");
					format(line3, sizeof(line3), "{007BD0}\nCommands:\n{B4B5B7}/drughelp, /getseeds, /plantweed, /checkweed, /pickweed.\n\n{007BD0}Location of Job:\n{B4B5B7}This job can be located outside the Drug Den opposite the Ganton Gym at the D icon.");
					jdialogid[playerid] = DIALOG_JOBHELP_DDEALER;
					return 1;
				}
				case 3: //Mechanic
				{
					format(titlestring, sizeof(titlestring), "Mechanic");
					format(line1, sizeof(line1), "{007BD0}Information:\n{B4B5B7}This job can be used to repair, add nos, and add hydraulics to vehicles.\nThis job can sometimes be rewarding but often people do not require assistance.\nThis is a legal job and you cannot be busted for doing it.");
					format(line2, sizeof(line2), "{007BD0}\nSkills:\n{B4B5B7}Level 1: You can refill a vehicle with 15% fuel.\nLevel 2: You can refill a vehicle with 40% fuel.\nLevel 3: You can refill a vehicle with 60% fuel.\nLevel 4: You can refill a vehicle with 80% fuel.\nLevel 5: You can refill a vehicle with 100% fuel.\nThe reload time for /repair depends on your mechanic level.");
					format(line3, sizeof(line3), "{007BD0}\nCommands:\n{B4B5B7}/repair, /hyd, /nos, /neon, /refill, /duty.\n\n{007BD0}Location of Job:\n{B4B5B7}This job can be obtained near /dropcar, at the job icon(yellow circle).");
					jdialogid[playerid] = DIALOG_JOBHELP_MECHANIC;
				}
				case 4: //Bodyguard
				{
					format(titlestring, sizeof(titlestring), "Bodyguard");
					format(line1, sizeof(line1), "{007BD0}Information:\n{B4B5B7}This job can be used to offer protection to people or give half armor.\nThis job is very profitable and the common spot for purchasing off bodyguards is the gym.\nThis is a legal job and you cannot be busted for doing it.");
					format(line2, sizeof(line2), "{007BD0}\nSkills:\n{B4B5B7}The reload time is always 1 minute, there are no job levels for this job so you don't need to level it up to earn the max money you can.");
					format(line3, sizeof(line3), "{007BD0}\nCommands:\n{B4B5B7}/guard, /sellvest.\n\n{007BD0}Location of Job:\n{B4B5B7}This job can be obtained outside the Ganton Gym, at the job icon(yellow circle).");
					jdialogid[playerid] = DIALOG_JOBHELP_BODYGUARD;
				}
				case 5: //Arms Dealer
				{
					format(titlestring, sizeof(titlestring), "Arms Dealer");
					format(line1, sizeof(line1), "{007BD0}Information:\n{B4B5B7}This job can be used to sell people weapons or materials which are used by businesses.\nThis job is very profitable and can earn you big cash at later levels.\nThis is a legal job and you cannot be busted for doing it.");
					format(line2, sizeof(line2), "{007BD0}\nSkills: (weapons)\n{B4B5B7}Level 1: Colt45 and all melee weapons.\nLevel 2: Shotgun and Rifle.\nLevel 3: MP5, UZI and TEC9.\nLevel 4: Deagle and Sawn-Off shotgun.\nLevel 5: AK47.\nMelee weapons: Flowers, Knuckles, Baseballbat, Poolcue, Parachute, Cane, Shovel, Golfclub, Katana and Dildo.\nThe reload time for selling weapons is always 10 seconds.");
					format(line3, sizeof(line3), "{007BD0}\nCommands:{B4B5B7}\n/getmats, /sellgun, /sellmats. NOTE: Materials cost $20 to $25 in game.\n\n{007BD0}Location of Job:\n{B4B5B7}This job can be obtained outside the large Los Santos Ammunation or in Blueberry.");
					jdialogid[playerid] = DIALOG_JOBHELP_ADEALER;
				}
				case 6: //Taxi Driver
				{
					format(titlestring, sizeof(titlestring), "Taxi Driver");
					format(line1, sizeof(line1), "{007BD0}Information:\n{B4B5B7}This job can be used to take passengers around the city for any price you desire($1-$500 per 16 seconds)\nThis job is not very profitable as people do not usually call taxis, and they sometimes try steal your taxi off you.\nThis is a legal job and you cannot be busted for doing it.");
					format(line2, sizeof(line2), "{007BD0}\nSkills:\n{B4B5B7}There is no reload time for taxi fares, there are no job levels for this job, in other words, you do not need to level it up to earn the max money you can.");
					format(line3, sizeof(line3), "{007BD0}\nCommands:\n{B4B5B7}/fare.\n\n{007BD0}Location of Job:\n{B4B5B7}This job can be obtained infront of Unity Station, at the job icon(yellow circle).");
					jdialogid[playerid] = DIALOG_JOBHELP_TAXIDR;
				}
				case 7: //Drug Smuggler
				{
					format(titlestring, sizeof(titlestring), "Drug Smuggler");
					format(line1, sizeof(line1), "{007BD0}Information:\n{B4B5B7}This job can be used to smuggle drugs into Los Santos.\nThis job can be very profitable at later levels but still can earn money at level 1.\nThis is an illegal job and you can be busted for doing it.");
					format(line2, sizeof(line2), "{007BD0}\nSkills:\n{B4B5B7}Level 1: You earn $5,000 when you deliver a crate.\nLevel 2: You earn $10,000 when you deliver a crate.\nLevel 3: You earn $15,000 when you deliver a crate.\nLevel 4: You earn $20,000 when you deliver a crate.\nLevel 5: You earn $25,000 when you deliver a crate.\nThere is no reload time for Drug Smuggling, it always costs $1000 for a drug crate.");
					format(line3, sizeof(line3), "{007BD0}\nCommands:\n{B4B5B7}/getcrate.\n\n{007BD0}Location of Job:\n{B4B5B7}This job can be obtained outside the Drugs Lab, near the Materials Factory 2.");
					jdialogid[playerid] = DIALOG_JOBHELP_DRUGSM;
				}
				case 8: // Waste Collector
				{
					format(titlestring, sizeof(titlestring), "Waste Collector");
					format(line1, sizeof(line1), "{007BD0}Information:\n{B4B5B7}This job can be used to pick up trash.\nThis job is not very profitable.\nThis is a legal job, meaning you won't get in trouble for picking up trash.");
					format(line2, sizeof(line2), "{007BD0}\nSkills:\n{B4B5B7}You can level up this job to earn more money each time you deliver trash.");
					format(line3, sizeof(line3), "{007BD0}\nCommands:\n{B4B5B7}/gettrash, /unloadtrash.\n\n{007BD0}Location of Job:\n{B4B5B7} This job can be obtained at the junkyard south of Blueberry.");
					jdialogid[playerid] = DIALOG_JOBHELP_WASTECOLLECTOR;
				}
				case 9: // Pizza Job
				{
					format(titlestring, sizeof(titlestring), "Pizza Deliver");
					format(line1, sizeof(line1), "{007BD0}Information:\n{B4B5B7}This job can be used to deliver pizzas, very profitable.\nThis is a legal job, meaning you won't get in trouble for delivering pizzas.");
					format(line2, sizeof(line2), "{007BD0}\nSkills:\n{B4B5B7}You can level up this job to earn more money as a bonus to your delivering time by delivering pizzas.\nLevel 1: $3,000 | Level 2: $6,000 | Level 3: $10,000 | level 4: $15,000 | Level 5: $20,000");
					format(line3, sizeof(line3), "{007BD0}\nCommands:\n{B4B5B7}/getpizza, /cpizzadelivery, /placepizza\n\n{007BD0}Location of Job:\n{B4B5B7} This job can be obtained at the Pizza Stacks.");
					jdialogid[playerid] = DIALOG_JOBHELP_PIZZADELIVER;
				}
				case 10: // Farmer Job
				{
					format(titlestring, sizeof(titlestring), "Farmer");
					format(line1, sizeof(line1), "{007BD0}Information:\n{B4B5B7}This job can be used to farm.\nThis is a legal job, meaning you won't get in trouble for farming.");
					format(line2, sizeof(line2), "{007BD0}\nSkills:\n{B4B5B7}You can level up this job to earn more money when farming by doing farm runs.\nLevel 1: $5,000 | Level 2: $10,000 | Level 3: $15,000 | level 4: $20,000 | Level 5: $25,000");
					format(line3, sizeof(line3), "{007BD0}\nCommands:\n{B4B5B7}/startfarming\n\n{007BD0}Location of Job:\n{B4B5B7} This job can be obtained at the Blueberry Farm.");
					jdialogid[playerid] = DIALOG_JOBHELP_FARMER;
				}
				case 11: // Grass Mower
				{
					format(titlestring, sizeof(titlestring), "Grass Mower");
					format(line1, sizeof(line1), "{007BD0}Information:\n{B4B5B7}This job can be used to mow the grass.\nThis is a legal job, meaning you won't get in trouble for mowing the grass.");
					format(line2, sizeof(line2), "{007BD0}\nSkills:\n{B4B5B7}No skills, but you get paid $10,000 and random bonus from $0-$10,000, good luck!");
					format(line3, sizeof(line3), "{007BD0}\nCommands:\n{B4B5B7}/grassmow\n\n{B4B5B7}/stopmowing\n\n{007BD0}Location of Job:\n{B4B5B7} This job can be obtained at the Glen Park.");
					jdialogid[playerid] = DIALOG_JOBHELP_GRASS;
				}
                                case 12: // Bus Driver
				{
					format(titlestring, sizeof(titlestring), "Bus Driver");
					format(line1, sizeof(line1), "{007BD0}Information:\n{B4B5B7}This is a very profitable job. Your motive is to drive the bus on a specific route.\nThis is a legal job, meaning you won't get arrested for doing this job.");
					format(line2, sizeof(line2), "{007BD0}\nSkills:\n{B4B5B7}You can level up in this job, /skills. Level 1: $20,000, Level 2: $30,000, Level 3: $40,000, Level 4: $50,000, Level 5: $60,000 per route.");
					format(line3, sizeof(line3), "{007BD0}\nCommands:\n{B4B5B7}/busfare\n\n{007BD0}Location of Job:\n{B4B5B7} This job can be obtained near Jefferson Motel.");
					jdialogid[playerid] = DIALOG_JOBHELP_BUS;
				}
                               case 13: // Miner
				{
					format(titlestring, sizeof(titlestring), "Miner");
					format(line1, sizeof(line1), "{007BD0}Information:\n{B4B5B7}This is a very profitable job, your main motive is to mine rocks and diamonds.\nThis is a legal job, meaning you won't get in trouble.");
					format(line2, sizeof(line2), "{007BD0}\nSkills:\n{B4B5B7}No skills, per rock you get $850 and per diamond you get $500,000. There is only one percent chance to get diamond.");
					format(line3, sizeof(line3), "{007BD0}\nCommands:\n{B4B5B7}/mine\n\n{007BD0}Location of Job:\n{B4B5B7} This job can be started at near construction site near All Saints. No need of (/getjob).");
					jdialogid[playerid] = DIALOG_JOBHELP_MINER;
				}
                                case 14: // TRUCKER
				{
					format(titlestring, sizeof(titlestring), "Trucker");
					format(line1, sizeof(line1), "{007BD0}Information:\n{B4B5B7}This is a very profitable job, you've to deliver different items from one place to other.\nIf you deliver illegal guns, you'd get in trouble.");
					format(line2, sizeof(line2), "{007BD0}\nSkills:\n{B4B5B7}Delivering illegal things pays the most but it's risky. Players are allowed to rob cargo. You need a trucker license to do this job.");
					format(line3, sizeof(line3), "{007BD0}\nCommands:\n{B4B5B7}/tpda\n\n/load\n\n/unload\n\n/checkcargo(Access: Everyone).{007BD0}Location of Job:\n{B4B5B7} This job can be obtained at El Corona.");
					jdialogid[playerid] = DIALOG_JOBHELP_TRUCKER;
				}
			}

			format(mstring, sizeof(mstring), "%s\n%s\n%s", line1, line2, line3);
			ShowPlayerDialog(playerid, jdialogid[playerid], DIALOG_STYLE_MSGBOX, titlestring, mstring, "Find job", "Back");
			return 1;
		}
                case DIALOG_JOBHELP_TRUCKER: //trucker
		{
			if(!response) return 1;

			goingJob[playerid] = true;
			displayCenterHUDInfo(playerid,  "Follow the ~r~red~w~ marker on your minimap to find the ~n~~y~Trucker Job~w~.", 8);
			SetPlayerCheckpointEx(playerid, 1767.27, -2040.56, 13.52, 5.0);
			return 1;
		}
                case DIALOG_JOBHELP_BUS: //Bus driver
		{
			if(!response) return 1;

			goingJob[playerid] = true;
			displayCenterHUDInfo(playerid,  "Follow the ~r~red~w~ marker on your minimap to find the ~n~~y~Bus Job~w~.", 8);
			SetPlayerCheckpointEx(playerid, 2141.65, -1176.11, 23.99, 5.0);
			return 1;
		}
                case DIALOG_JOBHELP_MINER: //Miner
		{
			if(!response) return 1;

			goingJob[playerid] = true;
			displayCenterHUDInfo(playerid,  "Follow the ~r~red~w~ marker on your minimap to find the ~n~~y~Miner Job~w~.", 8);
			SetPlayerCheckpointEx(playerid, 1282.43, -1265.65, 13.54, 5.0);
			return 1;
		}
		case DIALOG_JOBHELP_DETECTIVE: //Detective
		{
			if(!response) return 1;

			goingJob[playerid] = true;
			displayCenterHUDInfo(playerid,  "Follow the ~r~red~w~ marker on your minimap to find the ~n~~y~Detective Job~w~.", 8);
			SetPlayerCheckpointEx(playerid, 1553.5276, -1675.4719, 16.1953, 5.0);
			return 1;
		}
		case DIALOG_JOBHELP_LAWYER: // Lawyer
		{
			if(!response) return 1;

			goingJob[playerid] = true;
			displayCenterHUDInfo(playerid,  "Follow the ~r~red~w~ marker on your minimap to find the ~n~~y~Lawyer Job~w~.", 8);
			SetPlayerCheckpointEx(playerid, 1381.0413, -1088.8511, 27.3906, 5.0);
			return 1;
		}
		case DIALOG_JOBHELP_DDEALER: // Drug Dealer
		{
			if(!response) return 1;
			
			goingJob[playerid] = true;
			displayCenterHUDInfo(playerid,  "Follow the ~r~red~w~ marker on your minimap to find the ~n~~y~Drug Dealer Job~w~.", 8);
			SetPlayerCheckpointEx(playerid, 2166.3772, -1675.3829, 15.0859, 5.0);
			return 1;
		}
		case DIALOG_JOBHELP_MECHANIC: // Mechanic
		{
			if(!response) return 1;

			goingJob[playerid] = true;
			displayCenterHUDInfo(playerid,  "Follow the ~r~red~w~ marker on your minimap to find the ~n~~y~Mechanic Job~w~.", 8);
			SetPlayerCheckpointEx(playerid, 2329.4089, -2316.0996, 13.5469, 5.0);
			return 1;
		}
		case DIALOG_JOBHELP_BODYGUARD: // Bodyguard
		{
			if(!response) return 1;

			goingJob[playerid] = true;
			displayCenterHUDInfo(playerid,  "Follow the ~r~red~w~ marker on your minimap to find the ~n~~y~Bodyguard Job~w~.", 8);
			SetPlayerCheckpointEx(playerid, 2226.1716, -1718.1792, 13.5165, 5.0);
			return 1;
		}
		case DIALOG_JOBHELP_ADEALER: // Arms Dealer
		{
			if(!response) return 1;

			goingJob[playerid] = true;
			displayCenterHUDInfo(playerid,  "Follow the ~r~red~w~ marker on your minimap to find the ~n~~y~Arms Dealer Job~w~.", 8);
			SetPlayerCheckpointEx(playerid, 1366.4325, -1275.2096, 13.5469, 5.0);
			return 1;
		}
		case DIALOG_JOBHELP_BOXER: // Boxer
		{
			if(!response) return 1;
			
			goingJob[playerid] = true;
			displayCenterHUDInfo(playerid,  "Follow the ~r~red~w~ marker on your minimap to find the ~n~~y~Boxer Job~w~.", 8);
			SetPlayerCheckpointEx(playerid, 2227.9595, -1722.2257, 13.2633, 5.0);
			return 1;
		}
		case DIALOG_JOBHELP_TAXIDR: // Taxi Driver
		{
			if(!response) return 1;
			
			goingJob[playerid] = true;
			displayCenterHUDInfo(playerid,  "Follow the ~r~red~w~ marker on your minimap to find the ~n~~y~Taxi Driver Job~w~.", 8);
			SetPlayerCheckpointEx(playerid, 1741.7062, -1863.6664, 13.5748, 5.0);
			return 1;
		}
		case DIALOG_JOBHELP_DRUGSM: // Drug Smuggler
		{
			if(!response) return 1;

			goingJob[playerid] = true;
			displayCenterHUDInfo(playerid,  "Follow the ~r~red~w~ marker on your minimap to find the ~n~~y~Drug Smuggler Job~w~.", 8);
			SetPlayerCheckpointEx(playerid, 2354.2703, -1169.3293, 28.0083, 5.0);
			return 1;
		}
		case DIALOG_JOBHELP_PDEALER: // Product Dealer
		{
			if(!response) return 1;
			
			goingJob[playerid] = true;
			displayCenterHUDInfo(playerid,  "Follow the ~r~red~w~ marker on your minimap to find the ~n~~y~Product Dealer Job~w~.", 8);
			SetPlayerCheckpointEx(playerid, 972.0286, -1385.7682, 13.5477, 5.0);
			return 1;
		}
		case DIALOG_JOBHELP_WASTECOLLECTOR: // Waste Collector
		{
			if(!response) return 1;

			goingJob[playerid] = true;
			displayCenterHUDInfo(playerid,  "Follow the ~r~red~w~ ~r~red~w~ marker on your minimap to find the ~n~~y~Waste Collector Job~w~.", 8);
			SetPlayerCheckpointEx(playerid, 165.7221,-280.6324,1.9747, 5.0);
			return 1;
		}
		case DIALOG_JOBHELP_PIZZADELIVER: // Pizza job
		{
			if(!response) return 1;
			
			goingJob[playerid] = true;
			displayCenterHUDInfo(playerid,  "Follow the ~r~red~w~ ~r~red~w~ marker on your minimap to find the ~n~~y~Pizza Job~w~.", 8);
			SetPlayerCheckpointEx(playerid, 2104.9565, -1803.1753, 13.5547, 5.0);
			return 1;
		}
		case DIALOG_JOBHELP_FARMER: // Farmer Job
		{
			if(!response) return 1;
			goingJob[playerid] = true;
			displayCenterHUDInfo(playerid,  "Follow the ~r~red~w~ ~r~red~w~ marker on your minimap to find the ~n~~y~Farmer Job~w~.", 8);
			SetPlayerCheckpointEx(playerid, -40.0612, 26.5413, 3.1094, 5.0);
		}
		case DIALOG_JOBHELP_GRASS: // Grass Mower
		{
			if(!response) return 1;
			goingJob[playerid] = true;
			displayCenterHUDInfo(playerid,  "Follow the ~r~red~w~ ~r~red~w~ marker on your minimap to find the ~n~~y~Grass Mowing Job~w~.", 8);
			SetPlayerCheckpointEx(playerid, 1961.1292, -1244.8138, 19.9714, 5.0);
		}
	}
	return 1;
}


/* [=============	Useful Functions	=============]

	GetPlayerJobLevel(playerid, job) - gets the player's job level
	GetPlayerJobSkillLevel(playerid, job) - gets the amount of X the player needs to do to level up, ex: needs to sell 3 guns to level up
	GetPlayerJobSkill(playerid, job) - gets the player's job skill
*/

GetPlayerJobName(playerid)
{
	new jobname[20] = "None";
	switch(PlayerInfo[playerid][pJob])
	{
		case JOB_DETECTIVE:			jobname = "Detective";	
		case JOB_LAWYER: 			jobname = "Lawyer";	
		case JOB_DRUGDEALER: 		jobname = "Drug Dealer";	
		case JOB_PRODUCTDEALER: 	jobname = "Product Dealer";		
		case JOB_MECHANIC: 			jobname = "Mechanic";	
		case JOB_BODYGUARD: 		jobname = "Bodyguard";		
		case JOB_ARMS: 				jobname = "Arms Dealer";	
		case JOB_BOXER: 			jobname = "Boxer";		
		case JOB_TAXI: 				jobname = "Taxi Driver";	
		case JOB_SMUGGLER: 			jobname = "Drug Smuggler";	
		case JOB_MINER: 			jobname = "Miner";		
		case JOB_WASTECOLLECTOR: 	jobname = "Waste Collector";		
		case JOB_PIZZA:				jobname = "Pizza Boy";	
		case JOB_FARMER:			jobname = "Farmer";	
		case JOB_GRASS:				jobname = "Grass Mower";
        case JOB_BUS:				jobname = "Bus Driver";	
        case JOB_TRUCK:			    jobname = "Trucker";		
	}	
	return jobname;
}

public GetPlayerJobSkill(playerid, job)
{
	switch(job) {
		case JOB_DETECTIVE:			return PlayerInfo[playerid][pDetSkill];
		case JOB_LAWYER:			return PlayerInfo[playerid][pLawSkill];
		case JOB_DRUGDEALER:		return PlayerInfo[playerid][pDrugsSkill];
		case JOBID_CARJACK:			return PlayerInfo[playerid][pJackSkill];
		case JOB_MECHANIC:			return PlayerInfo[playerid][pMechSkill];
		case JOB_ARMS:				return PlayerInfo[playerid][pArmsSkill];
		case JOB_BOXER:				return PlayerInfo[playerid][pBoxSkill];
		case JOB_WASTECOLLECTOR:	return PlayerInfo[playerid][pWasteSkill];
		case JOB_SMUGGLER:			return PlayerInfo[playerid][pSmugglerSkill];
		case JOB_FARMER:			return PlayerInfo[playerid][pFarmerSkill];
        case JOB_BUS:				return PlayerInfo[playerid][pBusSkill];	
	}

	return 0;
}

public GetPlayerJobLevel(playerid, job)
{
	new joblevel1 = 0, joblevel2, joblevel3, joblevel4, joblevel5;
	new skill = GetPlayerJobSkill(playerid, job);

	joblevel2 = 50;
	joblevel3 = 100;
	joblevel4 = 200;
	joblevel5 = 400;	

	if(job == JOB_SMUGGLER)
	{
		joblevel2 = 20;
		joblevel3 = 40;
		joblevel4 = 60;
		joblevel5 = 80;
	}

    if(job == JOB_BUS)
	{
		joblevel2 = 10;
		joblevel3 = 20;
		joblevel4 = 30;
		joblevel5 = 50;
	}
	new lvl;
	if(skill >= joblevel1 && skill < joblevel2)
	{
		lvl = 1;
	}
	else if(skill >= joblevel2 && skill < joblevel3)
	{
		lvl = 2;
	}
	else if(skill >= joblevel3 && skill < joblevel4)
	{
		lvl = 3;
	}
	else if(skill >= joblevel4 && skill < joblevel5)
	{
		lvl = 4;
	}
	else if(skill >= joblevel5)
	{
		lvl = 5;
	}
	return lvl;
}

public GetPlayerJobSkillLevel(playerid, job)
{
	new skill = GetPlayerJobSkill(playerid, job); 
	new reqleft;
	
	if(job == JOB_DETECTIVE || job == JOB_LAWYER || job == JOB_DRUGDEALER ||
	job == JOBID_CARJACK || job == JOB_MECHANIC || job == JOB_ARMS || job == JOB_BOXER || job == JOB_WASTECOLLECTOR || job == JOB_FARMER)
	{		
		if(skill >= 0 && skill <= 49) reqleft = 50 -skill;
		else if(skill >= 50 && skill <= 99) reqleft = 100 -skill;
		else if(skill >= 100 && skill <= 199) reqleft = 200 -skill;
		else if(skill >= 200 && skill <= 399) reqleft = 400 -skill;

	}

	if(job == JOB_BUS)
	{
		if(skill >= 0 && skill <= 9) reqleft = 10 -skill;
		else if(skill >= 10 && skill <= 19) reqleft = 20 -skill;
		else if(skill >= 20 && skill <= 29) reqleft = 30 -skill;
		else if(skill >= 30 && skill <= 49) reqleft = 50 -skill;
	}
    if(job == JOB_SMUGGLER)
	{
		if(skill >= 0 && skill <= 19) reqleft = 20 -skill;
		else if(skill >= 20 && skill <= 39) reqleft = 40 -skill;
		else if(skill >= 40 && skill <= 59) reqleft = 60 -skill;
		else if(skill >= 60 && skill <= 119) reqleft = 120 -skill;
	}
	return reqleft;
}

new Float:RandomMowe[13][3] =
{
	{1928.0717,-1234.3527,18.0249},
	{1898.9652,-1222.1378,16.9995},
	{1868.4001,-1244.1985,13.9893},
	{1883.6132,-1192.0978,21.9133},
	{1907.1113,-1189.7694,22.0713},
	{1889.1681,-1158.8250,24.1529},
	{1967.3208,-1157.3369,20.9635},
	{1996.6298,-1165.6517,20.7865},
	{2034.3589,-1173.2274,22.6516},
	{2054.7466,-1156.8724,23.6783},
	{2032.5651,-1202.7784,23.0269},
	{2046.7028,-1240.0741,23.3066},
	{1976.8083,-1234.8224,20.0469}
};

GrassCar(carid)
{
	for(new i = 0; i < sizeof(cargrass); i++)
	{
		if(carid == cargrass[i])	return 1;
	}
	return 0;
}

#include "../gamemodes/inc/jobs/pizza_job.inc"
#include "../gamemodes/inc/jobs/fishing_job.inc"
#include "../gamemodes/inc/jobs/detective_job.inc"
#include "../gamemodes/inc/jobs/mechanic_job.inc"
#include "../gamemodes/inc/jobs/farmer_job.inc"
#include "../gamemodes/inc/jobs/grass_mower.inc"
#include "../gamemodes/inc/jobs/trucker.inc"