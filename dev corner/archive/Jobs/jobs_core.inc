/*
	TODO:
	    • Change job IDs in gamemode: so all job cmds work with new IDs.



*/

//Assign an ID to each job
enum {
	JOB_NONE = 0,
	JOB_LAWYER,
	JOB_DRUG_DEALER,
	JOB_MECHANIC,
	JOB_WASTE_COLLECTOR,
	JOB_PRODUCT_DEALER,
	JOB_ARMS_DEALER,
	JOB_BOXER,
	JOB_TAXI_DRIVER,
	JOB_DRUG_SMUGGLER,
    JOB_PIZZA
}

//Job info/point
enum JobPointsInfo {
	Float: jobsX,
	Float: jobsY,
	Float: jobsZ,
	jobName[30],
	jobID,
	jobHint[128]
}

new JobPoints[][JobPointsInfo] = {
	{1378.1117, -1096.1830, 25.1200, "Lawyer", JOB_LAWYER, ""},
	{2164.1377, -1675.4172, 15.0859, "Drug Dealer", JOB_DRUG_DEALER, ""},
	{2329.4089, -2316.0996, 13.5469, "Mechanic", JOB_MECHANIC, ""},
	{165.7221,-280.6324,1.9747, "Waste Collector", JOB_WASTE_COLLECTOR, "HINT: You can get in a garbage truck to start your a job or ride on someone elses."},
	{972.0325, -1385.7533, 13.5476, "Product Dealer", JOB_PRODUCT_DEALER, "HINT: You can find products in front of the discount warehouse."},
	{1366.4325, -1275.2096, 13.5469, "Arms Dealer", JOB_ARMS_DEALER, "HINT: You can find materials packages behind ammunations."},
	{756.7493, 6.3762, 1000.7001, "Boxer", JOB_BOXER, ""},
	{-1982.0994, 131.8526, 27.6875, "Taxi Driver", JOB_TAXI_DRIVER, "HINT: You can find taxis in the carpark behind the building."},
	{2354.2703, -1169.3293, 28.0083, "Drug Smuggler", JOB_DRUG_SMUGGLER, "HINT: You can find drug crates at the drug factory in Montgomery."},
	{2102.0754, -1810.4479, 13.5547, "Pizzaman", JOB_PIZZA, "HINT: You can get a pizza (/getpizza) from the side window of the Pizza Stack."}
};

new GettingJob[MAX_PLAYERS] = -1; //for accepting jobs.


//Load the job includes
#include "../gamemodes/inc/Jobs/Pizza.inc"


//==============================[ Job functions ]===============================
JobsInit() {

	//create all the job points
	new jobtext[80];
	for( new i; i < sizeof( JobPoints ); i++ ) {
	    format( jobtext, 80, "%s Job\n{FFFFFF}/getjob", JobPoints[i][jobName] );
	    CreateDynamic3DTextLabel( jobtext, 0xFFFF91FF, JobPoints[i][jobsX], JobPoints[i][jobsY], JobPoints[i][jobsZ] + 0.88, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100 );
	    CreateDynamicPickup( 1239, 1, JobPoints[i][jobsX], JobPoints[i][jobsY], JobPoints[i][jobsZ] );
	}
	
	
	//load pizza intinalization things
	PizzaInit();
	return 1;
}

JobCheckpoints( playerid ) {

	if( PizzaCheckpoints( playerid ) == 1 ) return 1; //don't continue through other checkpoints...
	
	//...
	
	return 0;
}

JobOnPlayerEnterVehicle( playerid, vehicleid, ispassenger ) {

	#pragma unused ispassenger
	
	if( EnterPizzaVehicle( playerid, vehicleid, ispassenger ) == 1 ) return 1;
	
	//...

	return 0;
}

//==============================[ Job commands ]================================

CMD:getjob( playerid, params[] ) {

	if( PlayerInfo[playerid][pJob] != 0 ) {
	    return SendClientMessage( playerid, COLOR_GREY, "You already have a job, use /quitjob first." );
	}
	
	if( GetPlayerState( playerid ) != 1 ) {
	    return SendClientMessage( playerid, COLOR_GREY, "You must exit your vehicle before getting a job." );
	}
	
	new jobString[128];
	for( new i; i < sizeof( JobPoints ); i++ ) {
	    if( IsPlayerInRangeOfPoint( playerid, 3.0, JobPoints[i][jobsX], JobPoints[i][jobsY], JobPoints[i][jobsZ] ) ) {
			GettingJob[playerid] = i;
			
			format( jobString, 128, "*If you are sure you want to become a %s, type /accept job.", JobPoints[i][jobName] );
			SendClientMessage( playerid, COLOR_LIGHTBLUE, jobString );
			return 1;
		}
	}
	
	SendClientMessage( playerid, COLOR_GREY, "You are not near a place you can be employed." );
	return 1;
}

CMD:acceptjob( playerid, params[] ) {
	if( GettingJob[playerid] > -1 ) {
	    SendClientMessage(playerid, COLOR_LIGHTBLUE, "* Congratulations with your new Job, type /help to see your job commands.");
		PlayerInfo[playerid][pJob] = JobPoints[GettingJob[playerid]][jobID];

		if( strlen( JobPoints[GettingJob[playerid]][jobHint] ) > 0 ) {
		    SendClientMessage( playerid, COLOR_WHITE, JobPoints[GettingJob[playerid]][jobHint] );
		}
		
		if( PlayerInfo[playerid][pJob] == JOB_PIZZA ) {
		    PlayerInfo[playerid][pClothes] = 155;
		    SetPlayerSkin( playerid, 155 );
		}
		
		GettingJob[playerid] = -1;
	}
	return 1;
}

CMD:quitjob( playerid, params[] ) {
	if( PlayerInfo[playerid][pJob] != JOB_NONE ) {
	    if( CP[playerid] != CHECKPOINT_NONE ) {
	        CP[playerid] = CHECKPOINT_NONE;
	        DisablePlayerCheckpointEx( playerid );
		}
		
		SendClientMessage( playerid, COLOR_LIGHTBLUE, "* You quit your job." );
		
		if( PlayerInfo[playerid][pJob] == JOB_PIZZA ) PizzaQuitJob( playerid );
		PlayerInfo[playerid][pJob] = JOB_NONE;
	
	}
	else {
	    SendClientMessage( playerid, COLOR_GREY, "You do not have a job to quit." );
	}
	return 1;
}
