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
/*
    This file contains functions & definitions
    assiocated to players and players only.
    This includes hashing, player data, loading,
    saving etc.
*/

/*
**              Hashing
**                  by Octet
*/

#define MAX_PASSWORD_LENGTH         64
#define MAX_SALT_LENGTH             32
#define MAX_ENCRYPTED_PASS_LENGTH   129

enum e_PassData {
    passEncrypt[MAX_ENCRYPTED_PASS_LENGTH],
    passSalt[MAX_SALT_LENGTH]
}


//temp vars for storing password
new tempPassword[MAX_PLAYERS][65];
new tempHash[MAX_PLAYERS][65];
new tempSalt[MAX_PLAYERS][33];




/*
    Whirlpool: http://forum.sa-mp.com/showthread.php?t=570945
*/
native WP_Hash(buffer[], len, const str[]);

//  Variables

//Variables used when fetching the values from the table
new NewPassword[MAX_PLAYERS][MAX_ENCRYPTED_PASS_LENGTH];
new NewSalt[MAX_PLAYERS][MAX_SALT_LENGTH];

//  Prototypes

forward HashPassword(password[], playerid);
forward HashPasswordEx(password[], encrypted[], salt[]);
forward CheckPassword(password[], hash[], salt[], playerid);


//  Functions

//Updates the password in the DB, as well as hashing
public HashPassword(password[], playerid) {

    printf("Hashing password for player %s (%d).", PlayerName(playerid), playerid);

    new string[MAX_ENCRYPTED_PASS_LENGTH];
    new salt[MAX_SALT_LENGTH];
    HashPasswordEx(password, string, salt);

    //save inside field NewPassword
    new query[300];
    mysql_format(sqlGameConnection, query, sizeof query, "UPDATE `players` SET `NewPassword` = '%e', `NewSalt` = '%e' WHERE `Name` = '%s'", string, salt, PlayerName(playerid));
    mysql_pquery(sqlGameConnection, query);
    return 1;
}

//Hashes the password referencing encrypted & salt & remember arrays are pointers so reference not required
public HashPasswordEx(password[], encrypted[], salt[]) {

    new part1[33];
    new part2[33];

    strmid(part1, password, 0, floatround(strlen(password)/2, floatround_floor));
    strmid(part2, password, floatround(strlen(password)/2, floatround_floor), strlen(password));

    format(salt, MAX_SALT_LENGTH, "%s", generateRandomSalt());
    format(encrypted, MAX_ENCRYPTED_PASS_LENGTH, "%s%s%s", part1, salt, part2);
    WP_Hash(encrypted, MAX_ENCRYPTED_PASS_LENGTH, encrypted);
    return;
}


public CheckPassword(password[], hash[], salt[], playerid) {

    new string[MAX_ENCRYPTED_PASS_LENGTH];
    new part1[33];
    new part2[33];
    strmid(part1, password, 0, floatround(strlen(password)/2, floatround_floor));
    strmid(part2, password, floatround(strlen(password)/2, floatround_floor), strlen(password));

    format(string, sizeof(string), "%s%s%s", part1, salt, part2);
    WP_Hash(string, sizeof string, string);

    if(strcmp(hash, string, true) == 0) {
        printf("The account, %s (%d) has been authorized using password.", PlayerName(playerid), playerid);
        return true;
    }
    return false;
}

/*
    Salt generation by Smo (0ms)
*/
stock generateRandomSalt() {
	new salt[32 + 1];
	for(new i; i < 32; i++) {
		salt[i] = random(79) + 47;

		if(salt[i] == 33 || salt[i] == 34 || salt[i] == 37 || salt[i] == 39 || salt[i] == 92) { //invalid SQL chars
			salt[i] = 33; //replace with !
		}
	}
	salt[32] = 0;
	return salt;
}