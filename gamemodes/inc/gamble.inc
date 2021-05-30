new GambleOffer[MAX_PLAYERS] = {-1, ...}; // variable to store the playerid who offered
new GambleAmount[MAX_PLAYERS]; // variable to store the amount info

CMD:gamble(playerid, params[])
{
    new targetid, str[128], amount, Float:x, Float:y, Float:z;
    if(!IsPlayerInRangeOfPoint(playerid, 100.0, -222.6756, -121.8232, 1901.0859)) 
    {
        CP[playerid] = 0;
        DisablePlayerCheckpoint(playerid);
        SendClientMessage(playerid, COLOR_GREY, "You're not inside the Dice Casino.");
        CP[playerid] = CHECKPOINT_DCARS; //set checkpoint ID
        SetPlayerCheckpointEx(playerid, 761.06, -1564.02, 13.92, 3.0);
        SendClientMessage(playerid, COLOR_LIGHTBLUE, "Dice Casino was marked on your minimap!");
        return 0;
    }

    if(sscanf(params, "ii", targetid, amount)) return SendClientMessage(playerid, -1, "Usage: /gamble [playerid] [amount]");
    if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, -1, "This player is not connected!");
    if(amount > 500000 || amount < 1000) return SendClientMessage(playerid, COLOR_GREY, "Amount cannot be more than $500,000 or less than $1,000.");
    if(amount > PlayerInfo[playerid][pCash]) return SendClientMessage(playerid, -1, "You do not have that much!");
    if(GambleOffer[playerid] != -1) return SendClientMessage(playerid, -1, "You're already gambling someone.");
    if(GambleOffer[targetid] != -1) return SendClientMessage(playerid, -1, "That player is already gambling someone!");
    GetPlayerPos(targetid, Float:x, Float:y, Float:z);
    if(!IsPlayerInRangeOfPoint(playerid, 5.0, x, y, z)) return SendClientMessage(playerid, -1, "You're not in range of that player!");
    if(targetid == playerid) return SendClientMessage(playerid, -1, "You cannot gamble yourself!");

    format(str, sizeof(str), "You've gambled %s for $%d, he has to type /agamble to accept it", PlayerOOCName(targetid), amount);
    SendClientMessage(playerid, -1, str);
    format(str, sizeof(str), "%s has gambled you for $%d, type /agamble to accept.", PlayerOOCName(playerid), amount);
    SendClientMessage(targetid, -1, str);

    GambleOffer[playerid] = targetid; // we store the playerid who we want to gamble against to our variable
    GambleOffer[targetid] = playerid; // we store the playerid who wants to offer to the targetid's variable
    GambleAmount[targetid] = amount; // we store the amount for the targetid to call upon later

    return 1;
}

CMD:agamble(playerid, params[])
{
    new enemy = GambleOffer[playerid], amount = GambleAmount[playerid];
    if(!IsPlayerInRangeOfPoint(playerid, 100.0, -222.6756, -121.8232, 1901.0859)) 
    {
        CP[playerid] = 0;
        DisablePlayerCheckpoint(playerid);
        SendClientMessage(playerid, COLOR_GREY, "You're not inside the Dice Casino.");
        CP[playerid] = CHECKPOINT_DCARS; //set checkpoint ID
        SetPlayerCheckpointEx(playerid, 761.06, -1564.02, 13.92, 3.0);
        SendClientMessage(playerid, COLOR_LIGHTBLUE, "Dice Casino was marked on your minimap!");
    }
    else if(GambleOffer[playerid] == -1) SendClientMessage(playerid, -1, "You were not offered to gamble!");
    else if(!IsPlayerConnected(enemy)) SendClientMessage(playerid, -1, "This player is not connected!");
    else if(PlayerInfo[playerid][pCash] < amount)
    {
        SendClientMessage(playerid, -1, "You do not have enough cash!");
    }
    else if(PlayerInfo[enemy][pCash] > amount)
    {
        new str[128];

        new your_roll = random(6);
        new enemy_roll = random(6);

        if(your_roll > enemy_roll)
        {
            format(str, sizeof(str), "%s (rolled %d) has won the gamble against %s (rolled %d) and won $%d", PlayerOOCName(playerid), your_roll, PlayerOOCName(enemy), enemy_roll, amount);
            ProxDetector( 7.0, playerid, str, COLOR_YELLOW,COLOR_YELLOW,COLOR_YELLOW,COLOR_YELLOW,COLOR_YELLOW );

            GiveMoney(playerid, amount);
            GiveMoney(enemy, -amount);
        }
        else if(your_roll < enemy_roll)
        {
            format(str, sizeof(str), "%s (rolled %d) has won the gamble against %s (rolled %d) and won $%d", PlayerOOCName(enemy), enemy_roll, PlayerOOCName(playerid), your_roll, amount);
            ProxDetector( 7.0, playerid, str, COLOR_YELLOW,COLOR_YELLOW,COLOR_YELLOW,COLOR_YELLOW,COLOR_YELLOW );

            GiveMoney(enemy, amount);
            GiveMoney(playerid, -amount);
        }
        else
        {
            format(str, sizeof(str), "%s (rolled %d) has won the gamble against %s (rolled %d) and nobody has won!", PlayerOOCName(enemy), enemy_roll, PlayerOOCName(playerid), your_roll);
            ProxDetector( 7.0, playerid, str, COLOR_YELLOW,COLOR_YELLOW,COLOR_YELLOW,COLOR_YELLOW,COLOR_YELLOW );
        }
    }
    else
    {
        SendClientMessage(playerid, -1, "The player who gambled you does not have enough cash!");
    }

    // Reset variables
    GambleOffer[enemy] = -1; // reset variables
    GambleOffer[playerid] = -1; // reset variables
    GambleAmount[playerid] = 0;

    return 1;
}