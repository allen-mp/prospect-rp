/*
	Author: 			$kylar

	Title: 				Inactive property checker

	Description: 		This script checks all properties (houses & businesses) on payday. If the property is owned by a player then
						it checks their last login. If it exceeds more than 30 days. Then it resets it.
*/

// Every file where y_hooks is used must have this included. Do not remove! //
#include <YSI\y_hooks>

// Function that does the property activity checking //
CheckInactiveHouses(){

	// INLINE FUNCTIONS. Look below to know which is hooked to which. //
	//________________________________________________________________//
	
	inline SQLFetchHouses(){

		// Storing the number of rows //
		new rows = cache_num_rows();

		// If there were any rows fetched at all then do the next steps otherwise simply get out of the inline function //
		if(rows > 0){
			new 
				i,j,						// loop variables //
				count, 						// Stores total houses that were reset //
				houseid,					// Stores the SQL ID of the house //
				ownerid, 					// Stores the SQL ID of the owner of the house //
				name[MAX_PLAYER_NAME + 1],	// Stores the name of the player //
				lastlogin[30],				// Stores the last login of the player //
				day, month, year;			// Stores the date month year respectively //

			// Looping though all the rows //
			for(i = 0; i < rows; i++){

				// Storing the data in the variables
				houseid = cache_get_field_content_int(i, "HouseID");
				ownerid = cache_get_field_content_int(i, "OwnerID");
				cache_get_field_content(i, "LastLogin", lastlogin);
				cache_get_field_content(i, "Name", name);

				// If the last login is none, continue //
				if(!strcmp(lastlogin, "none", .ignorecase = true))
					continue;
				
				// If the SQL ID of the player is offline //
				if(!IsSQLPlayerOnline(ownerid)){

					new 
						dateString[15],		// Stores the date string which is sliced from 'LastLogin' column
						timeString[15];		// Stores the time string which is sliced from 'LastLogin' column

					// Seperating the date and time from the 'LastLogin' string //
					sscanf(lastlogin, "s[15]s[15]", dateString, timeString);

					// Seperating the date / month / year from the dateString //
					sscanf(dateString, "p</>iii", month, day, year);

					new 
						todayDay,			// Stores today's date
						todayMonth,			// Stores today's month
						todayYear;			// Stores today's year

					// Get todays date, month and year //
					getdate(todayYear, todayMonth, todayDay);

					// Store the difference in days from lastlogin to today's date //
					new daysDifference = GetDaysDifference(day, month, year, todayDay, todayMonth, todayYear);
					
					// If the difference is less than a month //
					if(daysDifference <= 31)
						continue;					
						
					// Getting the ID of the house //
					for(j = 1; j < MAX_HOUSES; j++){
						if(HouseInfo[j][hOwnerID] == ownerid){

							SetHouseOnSale(j);
							printf("[INACTIVE HOUSE] [%d]%s's house [%d] has been reset for $%d. Last login: %s", ownerid, name, houseid, HouseInfo[j][hValue], lastlogin);
							count++;
							break;
						}						
					}
				}				
			}

			// Sending the count of houses that were reset to every online Property Moderator //
			if(count > 0){
				new msg[128];
				format(msg, sizeof(msg), "[INACTIVE HOUSES] %d houses were reset due to their owners' inactivity", count);
				foreach(i:Player){
					if(PlayerInfo[i][pBizMod])
						SendClientMessage(i, COLOR_GRAD1, msg);
				}
				SaveHouses();
			}
		}
	}
	
	//_______________________________________________________________//
	// End of inline functions. Check functions below which call these inline functions //

	// First to deal with houses //
	// Fetching the houses data along with player SQL ID//
	mysql_pquery_inline(sqlGameConnection, "SELECT houses.HouseID, houses.OwnerID, players.ID, players.LastLogin, players.Name FROM houses INNER JOIN players ON houses.OwnerID = players.ID", using inline SQLFetchHouses, "");
	return 1;
}

// A function which returns true if the SQL ID player is online //
IsSQLPlayerOnline(SQLID){
	foreach(new i: Player){
		if(SQLID == PlayerInfo[i][pID])
			return true;
	}
	return false;
}