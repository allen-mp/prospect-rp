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
// TextDraw developed using Zamaroht's Textdraw Editor 1.0

//functions
forward ShowBloodForPlayer(playerid, loss);
forward HideBloodForPlayer(playerid);
forward IsPlayerHurt(playerid);

//callbacks
forward OnPlayerHurt(playerid, loss);

static bool:shown[MAX_PLAYERS];
static timer[MAX_PLAYERS];

static Text:blood1;
static Text:blood2;

Hook:Hurt_OnGameModeInit() {
    blood1 = TextDrawCreate(-81.000000, -100.000000, "particle:bloodpool_64");
    TextDrawBackgroundColor(blood1, 255);
    TextDrawFont(blood1, 4);
    TextDrawLetterSize(blood1, 0.500000, 1.000000);
    TextDrawColor(blood1, -16776961);
    TextDrawSetOutline(blood1, 0);
    TextDrawSetProportional(blood1, 1);
    TextDrawSetShadow(blood1, 1);
    TextDrawUseBox(blood1, 1);
    TextDrawBoxColor(blood1, 0xFFFFFF70);
    TextDrawTextSize(blood1, 410.000000, 403.000000);
    

    blood2 = TextDrawCreate(199.000000, 210.000000, "particle:bloodpool_64");
    TextDrawBackgroundColor(blood2, 255);
    TextDrawFont(blood2, 4);
    TextDrawLetterSize(blood2, 0.500000, 1.000000);
    TextDrawColor(blood2, -16776961);
    TextDrawSetOutline(blood2, 0);
    TextDrawSetProportional(blood2, 1);
    TextDrawSetShadow(blood2, 1);
    TextDrawUseBox(blood2, 1);
    TextDrawBoxColor(blood2, 0xFFFFFF70);
    TextDrawTextSize(blood2, 410.000000, 403.000000);
    return 1;
}


//Check the status of the player
public IsPlayerHurt(playerid) {
    if( shown[playerid] ) return true;
    else return false;
}

//Enable the hurt effects for player, playerid
public ShowBloodForPlayer(playerid, Float:loss) {
    shown[playerid] = true;

    TextDrawShowForPlayer(playerid, blood1);
    TextDrawShowForPlayer(playerid, blood2);
    timer[playerid] = SetTimerEx("OnPlayerHurt", 500, true, "if", playerid, loss);
    return 1;
}

//What to do when the player is under hurting effects
public HideBloodForPlayer(playerid, Float:loss) {

    /*if(shown[playerd]) {
        HideTextDrawForPlayer(playerid, blood1);
        HideTextDrawForPlayer(playerid, blood2);
    }
    else {
        ShowTextDrawForPlayer(playerid, blood1);
        ShowTextDrawForPlayer(playerid, blood2);

    }
    shown[playerd] = !shown[playerd];
    */

    new Float:health;
    GetPlayerHealth(playerid, health);

    SetPlayerHealth(playerid, health - loss )
    return 1;
}

//Stop the hurting effects
public HidePlayerHurt(playerid) {
    shown[playerid] = false;
    KillTimer(timer[playerid]);
    TextDrawHideForPlayer(playerid, blood1);
    TextDrawHideForPlayer(playerid, blood2);
    return 1;
}
