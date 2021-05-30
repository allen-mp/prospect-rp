/*	        
									 /$$$$       /$$$$$$$$  /$$$$$$        /$$$$$$$  /$$$$$$$        /$$$$
									| $$_/      | $$_____/ /$$__  $$      | $$__  $$| $$__  $$      |_  $$
									| $$        | $$      | $$  \__/      | $$  \ $$| $$  \ $$        | $$
									| $$        | $$$$$   | $$            | $$$$$$$/| $$$$$$$/        | $$
									| $$        | $$__/   | $$            | $$__  $$| $$____/         | $$
									| $$        | $$      | $$    $$      | $$  \ $$| $$              | $$
									| $$$$      | $$$$$$$$|  $$$$$$/      | $$  | $$| $$             /$$$$
									|____/      |________/ \______/       |__/  |__/|__/            |____/

									 /$$$$$$$$ /$$ /$$   /$$                      /$$$$$$  /$$   /$$              
									| $$_____/| $$|__/  | $$                     /$$__  $$|__/  | $$              
									| $$      | $$ /$$ /$$$$$$    /$$$$$$       | $$  \__/ /$$ /$$$$$$   /$$   /$$
									| $$$$$   | $$| $$|_  $$_/   /$$__  $$      | $$      | $$|_  $$_/  | $$  | $$
									| $$__/   | $$| $$  | $$    | $$$$$$$$      | $$      | $$  | $$    | $$  | $$
									| $$      | $$| $$  | $$ /$$| $$_____/      | $$    $$| $$  | $$ /$$| $$  | $$
									| $$$$$$$$| $$| $$  |  $$$$/|  $$$$$$$      |  $$$$$$/| $$  |  $$$$/|  $$$$$$$
									|________/|__/|__/   \___/   \_______/       \______/ |__/   \___/   \____  $$
									                                                                     /$$  | $$
									                                                                    |  $$$$$$/
									                                                                     \______/ 
									 /$$$$$$$            /$$                 /$$$$$$$  /$$                    
									| $$__  $$          | $$                | $$__  $$| $$                    
									| $$  \ $$  /$$$$$$ | $$  /$$$$$$       | $$  \ $$| $$  /$$$$$$  /$$   /$$
									| $$$$$$$/ /$$__  $$| $$ /$$__  $$      | $$$$$$$/| $$ |____  $$| $$  | $$
									| $$__  $$| $$  \ $$| $$| $$$$$$$$      | $$____/ | $$  /$$$$$$$| $$  | $$
									| $$  \ $$| $$  | $$| $$| $$_____/      | $$      | $$ /$$__  $$| $$  | $$
									| $$  | $$|  $$$$$$/| $$|  $$$$$$$      | $$      | $$|  $$$$$$$|  $$$$$$$
									|__/  |__/ \______/ |__/ \_______/      |__/      |__/ \_______/ \____  $$
									                                                                 /$$  | $$
									                                                                |  $$$$$$/
									                                                                 \______/ 	

						 /$$   /$$                  /$$$$$$                                      /$$$$$$$  /$$           /$$         /$$    
						| $$$ | $$                 /$$__  $$                                    | $$__  $$|__/          | $$        | $$    
						| $$$$| $$  /$$$$$$       | $$  \__/  /$$$$$$   /$$$$$$  /$$   /$$      | $$  \ $$ /$$  /$$$$$$ | $$$$$$$  /$$$$$$  
						| $$ $$ $$ /$$__  $$      | $$       /$$__  $$ /$$__  $$| $$  | $$      | $$$$$$$/| $$ /$$__  $$| $$__  $$|_  $$_/  
						| $$  $$$$| $$  \ $$      | $$      | $$  \ $$| $$  \ $$| $$  | $$      | $$__  $$| $$| $$  \ $$| $$  \ $$  | $$    
						| $$\  $$$| $$  | $$      | $$    $$| $$  | $$| $$  | $$| $$  | $$      | $$  \ $$| $$| $$  | $$| $$  | $$  | $$ /$$
						| $$ \  $$|  $$$$$$/      |  $$$$$$/|  $$$$$$/| $$$$$$$/|  $$$$$$$      | $$  | $$| $$|  $$$$$$$| $$  | $$  |  $$$$/
						|__/  \__/ \______/        \______/  \______/ | $$____/  \____  $$      |__/  |__/|__/ \____  $$|__/  |__/   \___/  
						                                              | $$       /$$  | $$                     /$$  \ $$                    
						                                              | $$      |  $$$$$$/                    |  $$$$$$/                    
						                                              |__/       \______/                      \______/                                     							  							
*/
/*	Function list;
		•	IsPlayerFacingPoint
		•	GetPointDistanceToPointExMorph
		•	IsPointInRangeOfPoint
		•	AddThousandsSeparators
		• 	GetNameFomSQLID
*/

/*	Checks if the player is in range of a given point by distance,
	and checks if the player is facing the point.
*/

stock IsPlayerFacingPoint( playerid, Float: distance, Float: X, Float: Y, Float: Z, Float: error ) {
	if( !IsPlayerInRangeOfPoint( playerid, distance, X, Y, Z ) ) {
		return false;
	}
	
	new Float: angle, Float: misc = 5.0, Float: playerX, Float: playerY, Float: playerZ, Float: tarangle;
		
	GetPlayerFacingAngle( playerid, angle );
	GetPlayerPos( playerid, playerX, playerY, playerZ );
	
	tarangle = 180.0 -atan2( playerX - X, playerY - Y ) + misc;
	
	if( angle < tarangle + error && angle > tarangle - error ) {
		return true;
	}
	return false;
}	


Float:GetPointDistanceToPointExMorph(Float:x1, Float:y1, Float:z1, Float:x2, Float:y2, Float:z2)
{
	new Float:x, Float:y, Float:z;
	x = x1 -x2;
	y = y1 -y2;
	z = z1 -z2;
	return floatsqroot(x *x +y *y +z *z);
}

IsPointInRangeOfPoint(Float:x, Float:y, Float:z, Float:x2, Float:y2, Float:z2, Float:range)
{
	x2 -= x;
	y2 -= y;
	z2 -= z;
	return ((x2 * x2) + (y2 * y2) + (z2 * z2)) < (range * range);
}

/*
	Seperates numbers larger then 1,000 with a comma
*/
AddThousandsSeparators(number, const separator[] = ",")
{
	new output[15]; // longest possible output given 32 bit integers: -2,147,483,648
	format(output, sizeof(output), "%d", number);
 
	for(new i = strlen(output) - 3; i > 0 && output[i-1] != '-'; i -= 3)
	{
		strins(output, separator, i);
	}
 
	return output;
}

stock GetPosAheadVehicle(vehicleid, &Float:x, &Float:y, &Float:z, Float:offset =0.5)
{
	new Float:vehicleSize[3], Float:vehiclePos[3];
	GetVehiclePos(vehicleid, vehiclePos[0], vehiclePos[1], vehiclePos[2]);
	GetVehicleModelInfo(GetVehicleModel(vehicleid), VEHICLE_MODEL_INFO_SIZE, vehicleSize[0], vehicleSize[1], vehicleSize[2]);
	GetXYAheadVehicle(vehicleid, vehiclePos[0], vehiclePos[1], (vehicleSize[1] /2) +offset);
	x = vehiclePos[0];
	y = vehiclePos[1];
	z = vehiclePos[2];
	return 1;
}

GetXYAheadVehicle(vehicleid, &Float:q, &Float:w, Float:distance)
{
	new Float:a;
	GetVehiclePos(vehicleid, q, w, a);
	GetVehicleZAngle(vehicleid, a);
	q -= (distance * -floatsin(-a, degrees));
	w -= (distance * -floatcos(-a, degrees));
}
/*
	Gets a players name from their SQL ID

GetNameFomSQLID(sqlid) 
{ 
    new query[128], name[MAX_PLAYER_NAME]; 
    mysql_format(sqlGameConnection, query, sizeof(query), "SELECT name FROM `players` WHERE ID = %d LIMIT 1", sqlid); 

    new Cache:result = mysql_query(sqlGameConnection, query); 
   
    cache_get_field_content(0, "name", name, sqlGameConnection, 24);
    
    cache_delete(result); 
    return name; 
}*/
