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
    Player Inventories
    Base system by oneOCT3T

    To add an inventory item,
    1. add to e_pInventory enumerator
    2. add to GetInventoryName()
    3. add (make it same name as in e_pInventory), as a MySQL field in `inventories`

*/
enum e_pInventory {
    inMaterials,
    inCannabis,
    inCocaine,
    inMeth,
    inPackage,
    inCrates,
    inSeeds,
    inProducts,
    inRopes,
    inBlindfolds,
    inCigars,
    inSprunk,
    inSpraycan,
    inScrewdriver,
    inDeckOfCards,
    inWTC,
    inFishingRod,
    inBait,
    inBaitleft,
    inFishingLine,
    inStereo,
    inBoxOfMatches,
    inJerrycan,
    inCellphone,
    inCellPhoneN
}

//Vars

static Inventory[MAX_PLAYERS][e_pInventory];
static bool:ModifiedInventory[MAX_PLAYERS];



//Function Prototypes

forward GetInventoryName(invid);
//&: SetInventory(playerid, e_pInventory:item, oper, value);
forward GetInventory(playerid, e_pInventory:item);
static SaveInventoryQueue();
public ShowInventory(playerid, target);


//Function bodies

//Only shows what the player has, in a dialog
public ShowInventory(playerid, target) {
    new string[200];

    for(new item; e_pInventory:item < e_pInventory; item++) {

        if(Inventory[playerid][item]) {
            format(string, sizeof(string), "%s\n%d %s", string, Inventory[playerid][item], GetInventoryName(item));
        }
    }

    return 1;
}

//Saves the current modified inventories
static SaveInventoryQueue() {

    new query[400];
    foreach(new playerid: Player) {
        if(ModifiedInventory[playerid]) {
            mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE inventories SET");

            for(new item; e_pInventory:item < e_pInventory; item++) {

                mysql_format(sqlGameConnection, query, sizeof(query), " \
                    %s, `%s` = %d",
                    query, item, Inventory[playerid][item]);

            }

            mysql_format(sqlGameConnection, query, sizeof(query), "%s WHERE ID = %d", query, PlayerInfo[playerid][ID]);
            mysql_pquery(sqlGameConnection, query);
            ModifiedInventory[playerid] = false;
        }
    }
    return 1;
}

public GetInventory(playerid, e_pInventory:item) {
    return Inventory[playerid][item];
}

//Mainly like 'overloading operators', perform things when inventory changes
SetInventory(playerid, e_pInventory:item, oper = '\0', value = 0) {
    switch(oper) {
        case '+': {
            Inventory[playerid][item] += value;
            
        }
        case '-': {
            Inventory[playerid][item] -= value;
        }
        case '++': {
            Inventory[playerid][item]++;
        }
        case '--': {
            Inventory[playerid][item]--;
        }
        case '=': {
            Inventory[playerid][item] = value;
        }
        case default: {
            Inventory[playerid][item] = 0;
        }
    }

    ModifiedInventory[playerid] = true;
    printf("InventoryItemChange - %s (%d): %s is now %d (was %d)", PlayerName(playerid), playerid, GetInventoryName(item), Inventory[playerid][item], Inventory[playerid][item] - value);
    return Inventory[playerid][item];
}




//enum ID to inventory name
public GetInventoryName(invid) {

    new string[20];
    switch(invid) {

        case inMaterials: return "Material(s)";
        case inCannabis: return "Cannabis";
        case inCocaine: return "Cocaine";
        case inMeth: return "Meth";
        case inPackage: return "Package(s)";
        case inCrates: return "Crate(s)";
        case inSeeds: return "Seed(s)";
        case inProducts: return "Product(s)";
        case inRopes: return "Rope(s)";
        case inBlindfolds: return "Blindfold(s)";
        case inCigars: return "Cigar(s)";
        case inSprunk: return "Sprunk";
        case inSpraycan: return "Spraycan(s)";
        case inScrewdriver: return "Screwdriver(s)";
        case inDeckOfCards: return "Deck of Card(s)";
        case inWTC: return "wTC";
        case inFishingRod: return "Fishing Rod";
        case inBait: return "Bait";
        case inBaitleft: return "Bait left";
        case inFishingLine: return "Fishing Line";
        case inStereo: return "Portable Speaker";
        case inBoxOfMatches: return "Matches";
        case inJerrycan: return "Jerrycan";
        case inCellphone: format(string, sizeof(string), "Phone (%d)", inCellphoneN); return string;
        default: "Unknown";
    }
}