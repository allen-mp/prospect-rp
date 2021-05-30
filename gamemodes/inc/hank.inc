#define DIALOG_ISLAND 2453
#define COLOR_LIMEGREEN 0x10F441AA
// Variables
 
new HankDJ[MAX_PLAYERS];
new HankCurrentSong[128];
new IClose;
new DonationIsland;
new IslandLogin[MAX_PLAYERS];
new adtimerIs;
new FishingTimeIS[MAX_PLAYERS];
new Fish[MAX_PLAYERS];
 
Hook:eh_OnGameModeInit()
{
 
CreateDynamicPickup(1239, 2, 675.1188,-2294.3250,5.8098, -1);
Create3DTextLabel("~Tia's Island General Store~", COLOR_LIMEGREEN, 675.1188,-2294.3250,5.8098, 40.0, 0, 1);
CreateDynamicPickup(1239, 2, 737.5669,-2332.7700,1.5416 , -1);
Create3DTextLabel("~Fishing Range~\n Press Y", COLOR_LIMEGREEN, 737.5669,-2332.7700,1.5416, 40.0, 0, 1);
CreateDynamicPickup(1239, 2, 708.9966,-2330.2515,8.0751 , -1);
Create3DTextLabel("~Fish Box~", COLOR_LIMEGREEN, 708.9966,-2330.2515,8.0751, 40.0, 0, 1);
CreateDynamicPickup(1239, 2, 705.9294,-2341.3477,6.4736, -1);
Create3DTextLabel("~Fixing Point~", COLOR_LIMEGREEN, 705.9294, -2341.3477, 6.4736, 40.0, 0, 1);
LoadHankdata();
return 1;
}
 
CMD:imusic(playerid, params[]) {
    if(HankDJ[playerid] == 0) return SendClientMessage(playerid, COLOR_GREY, "You are not a DJ!");
    if(!IsPlayerInRangeOfPoint(playerid, 100.0, 700.2119,-2306.7632,10.1280)) return SendClientMessage(playerid, COLOR_GREY, "You are not at the Island!");
 
    if(sscanf(params, "s[128]", HankCurrentSong)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /imusic [URL]");
 
    foreach(new p : Player) {
        if(IsPlayerInRangeOfPoint(playerid, 100.0, 700.2119,-2306.7632,10.1280)) {
            // Play music and display song name for each player.
            PlayAudioStreamForPlayer(p, HankCurrentSong, 700.2119,-2306.7632,10.1280, 100, 1);
            SendClientMessage(p, COLOR_ORANGE, "[Island] The song was changed by the DJ.");
        }
    }
    return 1;
}
 
CMD:ilocate(playerid) {
    SetPlayerCheckpoint(playerid, 684.5220,-2257.2024,2.2046, 3.0);
    SendClientMessage(playerid, COLOR_YELLOW, "Map: {ffffff}Tia's Island has been marked on your minimap. Use (/killcp) to remove checkpoint.");
    return 1;
}
 
SendIslandMessage(color, string2[])
{
    foreach( new i: Player )
    {
        if(IsPlayerConnected(i))
        {
            if(IslandLogin[i] == 1)
            {
                SendClientMessage(i, color, string2);
            }
        }
    }
    return 1;
}
 
CMD:imessage(playerid, params[]) {
    new Text[128];
    new line[200];
    if(IClose == 1) return SendClientMessage(playerid, COLOR_GREY, "The hotline is closed at the moment.");
    if(sscanf(params, "s[128]", Text)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /imessage [Text]");
    format(line, sizeof(line), "Phone Number [%d]: %s", PlayerInfo[playerid][pPnumber], Text);
    SendIslandMessage(COLOR_GEAR_BLUE, line);
    SendClientMessage(playerid, COLOR_GEAR_YELLOW, "Your message was delivered to the owners of Tia's Island, thank you.");
    GiveMoney(playerid, -25);
    return 1;
}
 
CMD:ifix(playerid, params[]) {
    if(!IsPlayerInRangeOfPoint(playerid, 5.0, 705.9294,-2341.3477,6.4736)) return SendClientMessage(playerid, 0xC92C00FF, "You are not near the vehicle fix point.");
    if(IClose == 1) return SendClientMessage(playerid, 0xC92C00FF, "Fix service is closed right now, come back later.");
    if(!IsPlayerInAnyVehicle(playerid) || GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendClientMessage(playerid, 0xC92C00FF, "You are not the driver of a vehicle.");
    if(PlayerInfo[playerid][pCash] < 750) {
       
        SendClientMessage(playerid, 0xC92C00FF, "You don't have enough money. Required ($750).");
        return 1;
    }
 
    GivePlayerMoney(playerid, -750);
    PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
    GameTextForPlayer(playerid, "~g~Vehicle fixed", 2000, 3);
    RepairVehicle(GetPlayerVehicleID(playerid));
 
    DonationIsland += 750;
    SendClientMessage(playerid, COLOR_WHITE, "[Island Message]: Thank you, visit again.");
    SaveHankdata();
    return 1;
}
 
CMD:ic(playerid, params[]) {
    new Text[128];
    new line[200];
    if(IslandLogin[playerid] == 1)  
    {
       if(sscanf(params, "s[128]", Text)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /ichat [Text]");
       format(line, sizeof(line), "[Island Chat] %s: %s", PlayerICName(playerid), Text);
       SendIslandMessage(COLOR_GEAR_BLUE, line);
    }
    else SendClientMessage(playerid, COLOR_GREY, "You're not authorized to use this command.");
    return 1;
}
 
CMD:ilogin(playerid, params[]) {
    new login;
    new line[200];
    if(IslandLogin[playerid] == 1) return SendClientMessage(playerid, COLOR_GREY, "You're already logged into the Tia's Island System.");  // to be replaced by mine and tia's id
    if(sscanf(params, "d", login)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /ilogin [password]");
    if(login == 2)
    {
        IslandLogin[playerid] = 1;
        format(line, sizeof(line), "Tommy Update: {ffffff}%s has logged into the Tia's Island System.", PlayerICName(playerid));
        SendIslandMessage(COLOR_GEAR_RED, line);
    }
    else SendClientMessage(playerid, COLOR_GREY, "The password you've entered is wrong, sorry.");
    return 1;
}
 
CMD:islandinfo(playerid, params[]) {
    SendClientMessage(playerid, COLOR_YELLOW, "____________________Tia James' Island Information____________________");
    SendClientMessage(playerid, -1, "How to get there? You can get to Tia's Island by roadway, seaway and airway.");
    SendClientMessage(playerid, -1, "Who found Tia's Island? Hank James found Tia's Island, it's located in the middle of LS sea.");
    SendClientMessage(playerid, -1, "Services available at Tia's Island? Fishing n' Selling services, cheap shops for general items, mechanic, music, etc. etc!");
    return 1;
}
 
CMD:iclose(playerid, params[]) {
    if(IslandLogin[playerid] != 1) return SendClientMessage(playerid, COLOR_GREY, "You're not allowed to use this command.");
    if(IClose == 0)
    {
      IClose = 1;
      SendIslandMessage(COLOR_GEAR_RED, "Tommy Update: {ffffff}The services of Tia's Island were pulled down.");
      SaveHankdata();
    }
    else if(IClose == 1)
    {
      IClose = 0;
      SendIslandMessage(COLOR_GEAR_RED, "Tommy Update: {ffffff}The services of Tia's Island were pulled up.");
      SaveHankdata();
    }
    return 1;
}
 
CMD:imic(playerid, params[]) {
    new micsays[128];
    new line[200];
    if(HankDJ[playerid] == 0) return SendClientMessage(playerid, COLOR_GREY, "You're not a DJ to use mic, sorry.");
    if(!IsPlayerInRangeOfPoint(playerid, 100.0, 700.2119,-2306.7632,10.1280)) return SendClientMessage(playerid, COLOR_GREY, "You are not at the Island!");
 
    if(sscanf(params, "s[128]", micsays)) return SendClientMessage(playerid, COLOR_GEAR_PURPLE, "{00BFFF}Usage:{FFFFFF} /imic [Text]");
 
    format(line, sizeof(line), "%s [MIC]: %s", PlayerICName(playerid), micsays);
 
    foreach(new p : Player) {
        if(IsPlayerInRangeOfPoint(p, 100.0, 700.2119,-2306.7632,10.1280)) {
            SendClientMessage(p, COLOR_GEAR_RED, line);
        }
    }
    return 1;
}
 
CMD:ibuy(playerid, params[]) {
    if(IClose == 1) return SendClientMessage(playerid, COLOR_GREY, "The services are closed right now, come back later.");
    if(!IsPlayerInRangeOfPoint(playerid, 3.0, 675.1188,-2294.3250,5.8098)) return SendClientMessage(playerid, COLOR_GREY, "You are not at the Island!");
    ShowPlayerDialog(playerid, DIALOG_ISLAND, DIALOG_STYLE_TABLIST, "Island General Shop", "Dice\t$500\nSpraycan\t$200\nRope\t$1000\nCigars\t$50\nSprunk\t$10\nWalkie Talkie\t$200\nScrewdriver\t$50\nBlindfolds\t$2000\nCards\t$1500\nFlower\t$50\nStereo\t$3000\nMatches\t$250\nBeer\t$500", "Select", "Cancel");
 
    return 1;
}
 
CMD:isell(playerid, params[]) {
 
    new fish1 = Fish[playerid];
    new price;
    new woah[128];
    if(!IsPlayerInRangeOfPoint(playerid, 2.0, 708.9966,-2330.2515,8.0751)) return SendClientMessage(playerid, COLOR_GREY, "You're not at the fish box on Island.");
    if(IClose == 1) return SendClientMessage(playerid, COLOR_GREY, "Fishing service is closed right now, come back later.");
    if(fish1 >= 1)
    {
       if(fish1 == 1 && fish1 < 55)
       {
          Fish[playerid] = 0;
          price = random(150) + 300;
          GiveMoney(playerid, price);
          format(woah, sizeof(woah), "You sold off the fish for ($%d).", price);
          SendClientMessage(playerid, COLOR_LIGHTBLUE, woah);
         
       }
       if(fish1 > 55 && fish1 < 75)
       {
          Fish[playerid] = 0;
          price = random(150) + 400;
          GiveMoney(playerid, price);
          format(woah, sizeof(woah), "You sold off the fish for ($%d).", price);
          SendClientMessage(playerid, COLOR_LIGHTBLUE, woah);
       
       }
       if(fish1 > 75 && fish1 < 85)
       {
          Fish[playerid] = 0;
          price = random(150) + 600;
          GiveMoney(playerid, price);
          format(woah, sizeof(woah), "You sold off the fish for ($%d).", price);
          SendClientMessage(playerid, COLOR_LIGHTBLUE, woah);
       }
    }
    else SendClientMessage(playerid, COLOR_GREY, "You don't have a fish to sell off.");
    return 1;
}
 
forward Timer_Fishsearched(playerid);
public Timer_Fishsearched(playerid)
{
    FishingTimeIS[playerid] = 0;
    return 1;
}
 
CMD:imakedj(playerid, params[])
{
      if(IslandLogin[playerid] == 1)
      {
         new targetid;
         if(sscanf(params, "u", targetid)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /imakedj [playerid]");
         if(HankDJ[targetid] == 0)
         {
             SendClientMessage(playerid, COLOR_GEAR_GREEN, "You've given them the status of DJ.");
             SendClientMessage(targetid, COLOR_GEAR_GREEN, "You've been given the status of DJ.");
             HankDJ[targetid] = 1;
         }
         else
         {
             SendClientMessage(targetid, COLOR_GEAR_GREEN, "Your DJ status was taken away.");
             SendClientMessage(playerid, COLOR_GEAR_GREEN, "You've taken away their DJ status.");
             HankDJ[targetid] = 0;
         }
       }
       else SendClientMessage(playerid, COLOR_GREY, "You're not authorized to use this command.");
      return 1;
}
 
CMD:ipullout(playerid)
{
   if(IslandLogin[playerid] == 1)
   {
      if(DonationIsland == 0) return SendClientMessage(playerid, COLOR_GREY, "There's no donation to withdraw.");
      GiveMoney(playerid, DonationIsland);
      DonationIsland = 0;
      SendClientMessage(playerid, COLOR_GREY, "You've pulled out whatever your Island's treasury has collected.");
      SendIslandMessage(COLOR_GEAR_RED, "Tommy Update: {ffffff}Treasury's money was withdrawn by someone.");
      SaveHankdata();
   }
   else SendClientMessage(playerid, COLOR_GREY, "You're not authorized to use this command.");
   return 1;
}
 
CMD:iad(playerid)
{
   if(IslandLogin[playerid] == 1)
   {
      if(IClose == 1) return SendClientMessage(playerid, COLOR_GREY, "The Island is closed right now, you cannot make advertisement.");
      if(adtimerIs == 1) return SendClientMessage(playerid, COLOR_GREY, "You've made an advertisement, please wait a minute to make one more.");
      SendClientMessageToAll(COLOR_GREEN, "Advertisement [2019]: Tia's Island is opened now! You all are invited. (/ilocate and /ihelp) to locate and know about the Island.");
      adtimerIs = 1;
      SetTimer("Timer_Isearched", 60000, 0);
   }
   else SendClientMessage(playerid, COLOR_GREY, "You're not authorized to use this command.");
   return 1;
}
 
forward Timer_Isearched(playerid);
public Timer_Isearched(playerid)
{
    adtimerIs = 0;
    return 1;
}
 
CMD:idonate(playerid, params[])
{
   new donate;
   new woah[122];
   if(!IsPlayerInRangeOfPoint(playerid, 2.0, 658.7969,-2304.2676,4.6387)) return SendClientMessage(playerid, COLOR_GREY, "You are not at the Tommy's Hut to donate.");
   if(IClose == 1) return SendClientMessage(playerid, COLOR_GREY, "Tommy says: Come back later, donation service is closed.");
   if(sscanf(params, "d", donate)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /idonate [amount]");
   if(PlayerInfo[playerid][pCash] > donate)
   {
     if(donate < 1) return SendClientMessage(playerid, COLOR_RED, "You can't donate below $0 or $0!");
     GiveMoney(playerid, -donate);
     DonationIsland += donate;
     SendClientMessage(playerid, COLOR_GREY, "Tommy says: Thanks for your donation, we're glad to have you as a visitor.");
     SendIslandMessage(COLOR_GEAR_RED, "Tommy Update: {ffffff} Donation was added to the treasury.");
     format(woah, sizeof(woah), "[ISLAND]: {ffffff}%s has donated $%d to Tia James' Island! Thank You.", PlayerICName(playerid), donate);
     if(donate > 1000000) return SendClientMessageToAll(COLOR_ORANGE, woah);
     SaveHankdata();
   }
   else SendClientMessage(playerid, COLOR_GREY, "Tommy says: Looks like you don't have much. It's fine we don't need donation.");
   return 1;
}
 
 
 
CMD:ihelp(playerid, params[])
{
    new wooh[100];
    SendClientMessage(playerid, COLOR_GREY, "____________________Tia James' Island Commands____________________");
    SendClientMessage(playerid, COLOR_GEAR_GREEN, "Commands(All): /ilocate, /isell, /idonate, /imessage, /islandinfo, /ifix, /ibuy");
    if(IslandLogin[playerid] == 1)
    {
       SendClientMessage(playerid, COLOR_GEAR_GREEN, "Commands(Owner): /imusic, /imakedj, /imic, /iclose, /ic, /ipullout, /iad, /ilogin");
       format(wooh, sizeof(wooh), "Total Island Treasury: $%d", DonationIsland);
       SendClientMessage(playerid, COLOR_GEAR_GREEN, wooh);
    }
    return 1;
}
 
stock SaveHankdata()
{
    new file[25];
    format(file, sizeof(file), "HankJames.cfg");
    if(!dini_Exists(file)) dini_Create(file);
 
    dini_IntSet(file, "Donation", DonationIsland);
    dini_IntSet(file, "Closed", IClose);
    return 1;
}
stock LoadHankdata()
{
    new file[25];
    format(file, sizeof(file), "HankJames.cfg");
    if(!dini_Exists(file)) dini_Create(file);
 
    DonationIsland = dini_Int(file, "Donation");
    IClose = dini_Int(file, "Closed");
    return 1;
}
 
Hook:eh_OnPlayerDisconnect(playerid, reason)
{
    IslandLogin[playerid] = 0;
    HankDJ[playerid] = 0;
    Fish[playerid] = 0;
    FishingTimeIS[playerid] = 0;    
    return 1;
}
 
Hook:eh_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(PRESSED(KEY_YES))
        {
           if(IsPlayerInRangeOfPoint(playerid, 30.0, 737.5669,-2332.7700,1.5416))
           {
              if(IClose == 1) return SendClientMessage(playerid, COLOR_GREY, "Fishing service is closed right now, come back later.");
              //if(PlayerHasRod[playerid] == 0) return SendClientMessage(playerid, COLOR_GREY, "Use your rod in order to fish (/userod).");
              if(Fish[playerid] >= 1) return SendClientMessage(playerid, COLOR_GREY, "You already have a fish, sell it first {ffffff}(/isell).");
              new fish = random(100) - 15;
              if(FishingTimeIS[playerid] == 1) return SendClientMessage(playerid, COLOR_GREY, "You will have to wait 60 seconds before catching another fish.");
              if(fish < 1) return SendClientMessage(playerid, COLOR_RED, "You didn't catch any fish.");
              if(fish == 1 && fish < 55)
              {
                 Fish[playerid] = fish;
                 SendClientMessage(playerid, COLOR_LIGHTBLUE, "You've cought a small size fish.");
                 FishingTimeIS[playerid] = 1;
                 SetTimerEx("Timer_Fishsearched", 60000, 0, "i", playerid);
              }
              if(fish > 55 && fish < 75)
              {
                 Fish[playerid] = fish;
                 SendClientMessage(playerid, COLOR_LIGHTBLUE, "You've cought a medium size fish.");
                 FishingTimeIS[playerid] = 1;
                 SetTimerEx("Timer_Fishsearched", 60000, 0, "i", playerid);
              }
              if(fish > 75 && fish < 85)
              {
                 Fish[playerid] = fish;
                 SendClientMessage(playerid, COLOR_LIGHTBLUE, "You've cought a large size fish.");
                 FishingTimeIS[playerid] = 1;
                 SetTimerEx("Timer_Fishsearched", 60000, 0, "i", playerid);
              }
              return 1;
           }
        }
       
       
    return 1;
}
 
Hook:eh_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_ISLAND)
    {
        if(response)
        {
            new str[245];
            if(listitem == 0) //dice
            {
                if(PlayerInfo[playerid][pDice] == 1)
                {
                    SendClientMessage(playerid, COLOR_GREY, "You already have dice.");
                    return 1;
                }
                if(PlayerInfo[playerid][pCash] > 499)
                {
                    GiveMoney(playerid, -500);
                    DonationIsland += 500;
 
                    PlayerInfo[playerid][pDice] = 1;
                    format(str, sizeof(str), "~r~-$%d", 500);
                    GameTextForPlayer(playerid, str, 5000, 1);
                    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
                    SaveHankdata();
                }
                else
                {
                    return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
                }
            }
            if(listitem == 1)
            {
                if(PlayerInfo[playerid][pSpraycan] >= 10)
                {
                    SendClientMessage(playerid, COLOR_GREY, "You already have 10 or more spray cans.");
                    return 1;
                }
                if(PlayerInfo[playerid][pCash] >= 200)
                {
                    GiveMoney(playerid, -200);
                    DonationIsland += 200;
 
                    PlayerInfo[playerid][pSpraycan] += 10;
                    format(str, sizeof(str), "~r~-$%d", 200);
                    GameTextForPlayer(playerid, str, 5000, 1);
                    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
                    SaveHankdata();
                }
                else
                {
                    return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
                }
            }
            if(listitem == 2)
            {
                if(PlayerInfo[playerid][pRope] >= 9)
                {
                    SendClientMessage(playerid, COLOR_GREY, "You already have 9 or more ropes.");
                    return 1;
                }
                if(PlayerInfo[playerid][pCash] > 999)
                {
                    GiveMoney(playerid, -1000);
                    DonationIsland += 1000;
 
                    PlayerInfo[playerid][pRope] += 3;
                    format(str, sizeof(str), "~r~-$%d", 1000);
                    GameTextForPlayer(playerid, str, 5000, 1);
                    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
                    SaveHankdata();
                }
                else
                {
                    return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
                }
            }
            if(listitem == 3)
            {
                if(PlayerInfo[playerid][pCigars] >= 10)
                {
                    SendClientMessage(playerid, COLOR_GREY, "You already have 10 or more cigars.");
                    return 1;
                }
                if(PlayerInfo[playerid][pCash] > 50)
                {
                    GiveMoney(playerid, -50);
                    DonationIsland += 50;
 
                    PlayerInfo[playerid][pCigars] += 10;
                    format(str, sizeof(str), "~r~-$%d", 50);
                    GameTextForPlayer(playerid, str, 5000, 1);
                    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
                    SaveHankdata();
                }
                else
                {
                    return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
                }
            }
            if(listitem == 4)
            {
                if(PlayerInfo[playerid][pSprunk] >= 3)
                {
                    SendClientMessage(playerid, COLOR_GREY, "You already have 3 or more cans of sprunk.");
                    return 1;
                }
                if(PlayerInfo[playerid][pCash] > 9)
                {
                    GiveMoney(playerid, -10);
                    DonationIsland += 10;
 
                    PlayerInfo[playerid][pSprunk] += 1;
                    format(str, sizeof(str), "~r~-$%d", 10);
                    GameTextForPlayer(playerid, str, 5000, 1);
                    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
                    SaveHankdata();
                }
                else
                {
                    return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
                }
            }
            if(listitem == 5) //walkie talkie
            {
                if(PlayerInfo[playerid][pWT] == 1)
                {
                    SendClientMessage(playerid, COLOR_GREY, "You already have a Walkie Talkie.");
                    return 1;
                }
                if(PlayerInfo[playerid][pCash] > 199)
                {
                    GiveMoney(playerid, -200);
                    DonationIsland += 200;
 
                    PlayerInfo[playerid][pWT] = 1;
                    format(str, sizeof(str), "~r~-$%d", 200);
                    GameTextForPlayer(playerid, str, 5000, 1);
                    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
                    SaveHankdata();
                }
                else
                {
                    return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
                }
            }
            if(listitem == 6) //screw drivers
            {
                if(PlayerInfo[playerid][pScrew] >= 5)
                {
                    SendClientMessage(playerid, COLOR_GREY, "You already have 5 or more screw drivers.");
                    return 1;
                }
                if(PlayerInfo[playerid][pCash] > 49)
                {
                    GiveMoney(playerid, -50);
                    DonationIsland += 50;
 
                    PlayerInfo[playerid][pScrew] += 5;
                    format(str, sizeof(str), "~r~-$%d", 50);
                    GameTextForPlayer(playerid, str, 5000, 1);
                    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
                    SaveHankdata();
                }
                else
                {
                    return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
                }
            }
            if(listitem == 7) //blindfolds
            {
                if(PlayerInfo[playerid][pBlindfolds] >= 6)
                {
                    SendClientMessage(playerid, COLOR_GREY, "You already have 6 or more blindfolds.");
                    return 1;
                }
                if(PlayerInfo[playerid][pCash] >= 2000)
                {
                    GiveMoney(playerid, -2000);
                    DonationIsland += 2000;
 
                    PlayerInfo[playerid][pBlindfolds] += 2;
                    format(str, sizeof(str), "~r~-$%d", 2000);
                    GameTextForPlayer(playerid, str, 5000, 1);
                    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
                    SaveHankdata();
                }
                else
                {
                    return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
                }
            }
            if(listitem == 8) //cards
            {
                if(PlayerInfo[playerid][pCash] >= 1500)
                {
                    GiveMoney(playerid, -1500);
                    DonationIsland += 1500;
 
                    format(str, sizeof(str), "~r~-$%d", 1500);
                    GameTextForPlayer(playerid, str, 5000, 1);
                    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
                    PlayerInfo[playerid][pDeck] = 1;
                    SaveHankdata();
                }
                else
                {
                    return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
                }
            }
            if(listitem == 9)
            {
                if(PlayerInfo[playerid][pCash] >= 50)
                {
                    GiveMoney(playerid, -50);
                    DonationIsland += 50;
 
                    //GivePlayerGun(playerid, 14); // to be enabled.
                    format(str, sizeof(str), "~r~-$%d", 50);
                    GameTextForPlayer(playerid, str, 5000, 1);
                    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
                    SaveHankdata();
                }
                else
                {
                    return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
                }
            }
            if(listitem == 10) // Stero
            {
                if(PlayerInfo[playerid][pStereo] > 0) return SendClientMessage(playerid, COLOR_GREY, "You already have a Boombox.");
                if(PlayerInfo[playerid][pCash] >= 3000)
                {
                    GiveMoney(playerid, -3000);
                    DonationIsland += 3000;
 
                    PlayerInfo[playerid][pStereo] = 2;
                    format(str, sizeof(str), "~r~-$%d", 3500);
                    GameTextForPlayer(playerid, str, 5000, 1);
                    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
                    SaveHankdata();
                }
                else
                {
                    return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
                }
            }
            if(listitem == 11) //Box of Matches
            {
                if(PlayerInfo[playerid][pMatches] > 0) return SendClientMessage(playerid, COLOR_GREY, "You can only carry one box of matches.");
                if(PlayerInfo[playerid][pCash] >= 250)
                {
                    GiveMoney(playerid, -250);
                    DonationIsland += 250;
 
                    PlayerInfo[playerid][pMatches] = 3;
                    format(str, sizeof(str), "~r~-$%d", 250);
                    GameTextForPlayer(playerid, str, 5000, 1);
                    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
                    SaveHankdata();
                }
                else
                {
                    return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
                }
            }
            if(listitem == 12) //case of beer
            {
                if(PlayerInfo[playerid][pCash] >= 500)
                {
                    GiveMoney(playerid, -500);
                    DonationIsland += 500;
 
                    PlayerInfo[playerid][pBeer] += 5;
                    format(str, sizeof(str), "~r~-$%d", 500);
                    GameTextForPlayer(playerid, str, 5000, 1);
                    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
                    SaveHankdata();
                }
                else
                {
                    return SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
                }
            }
        }
        return 1;
    }
    return 1;
}