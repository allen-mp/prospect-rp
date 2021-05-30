/*

	FMD Monthly call count reset system. ~$kylar

*/

// A variable that checks whether or not the callcounts should be reset //
static bool:_FMDCallResetFlag = true;

// A function that checks if it's the first day of the month //
AutoResetCallCount(){

	// Variable to store the day, month and year. Most of them are redundant except for day //
	new 
		day,
		month, 
		year;

	// Actually fetching the data //
	getdate(year, month, day);

	// If day is the first of the month //
	if(day == 1){

		// If the flag is true then the counts are reset //
		if(_FMDCallResetFlag){

			// Setting the flag to false so that it does not reset it again in that very same day if it's checked again //
			_FMDCallResetFlag = false;

			// looping though all the players and resetting the monthly call count variable //
			foreach(new i: Player)
				PlayerInfo[i][pMonthlyCallCount] = 0;

			// Sending a query to reset all monthly call count coloumn in the database //
			mysql_pquery(sqlGameConnection, "UPDATE `players` SET `MonthlyMedicCount` = 0");
		}
	}
	// If it's not the first of the month
	else{

		// If the flag is set to false
		if(!_FMDCallResetFlag){

			// Set it to true so that if the first day of the month happens again then it triggers the reset //
			_FMDCallResetFlag = true;
		}
	}

	return 1;
}