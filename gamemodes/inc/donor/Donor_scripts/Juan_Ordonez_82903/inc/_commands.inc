CMD:netkey(playerid, params[]) 
{
  new targetid;
  if(PlayerInfo[playerid][pID] != Juan_ORDONEZ_ID) return AdmErrorMsg;
  if(!IsPlayerInRangeOfPoint(playerid, 5.0, 1464.1365,-618.7394,1994.6597)) return SendClientMessage(playerid, COLOR_GREY,"You're not at the office of the Network!");
  if(sscanf(params,"u", targetid)) return SendClientMessage(playerid, COLOR_WHITE, "{c0c0c0}Usage: {ffffff}/netkey [PlayerName/Playerid]");

  new bool:check = bool:PlayerNearPlayer(playerid, targetid);

  if(check == true)
  {
    if(netkey[targetid] == 1)
    {
      SendClientMessage(targetid, -1, "{808040}Your key has been revoked.");
      netkey[targetid] = 0;
      SendClientMessage(playerid, -1, "{808040}You have revoked their key.");
    }
    else if(netkey[targetid] == 0)
    {
      SendClientMessage(targetid, -1, "{808040}You have been given a key.");
      netkey[targetid] = 1;
      SendClientMessage(playerid, -1, "{808040}You have given them the key.");
    }
  }

  else SendClientMessage(playerid, COLOR_GREY, "Error: {ffffff}You're not near that player!");
  return 1;
}

CMD:netdonate(playerid, params[])
{
  new donation, string[123];
  
  if(PlayerInfo[playerid][pLevel] == 1 ) return SendClientMessage(playerid, COLOR_GREY,   "You are not allow to donate");
  if(!IsPlayerInRangeOfPoint(playerid, 2.0, 1452.5398,-620.8953,1901.2599)) return SendClientMessage(playerid, COLOR_GREY,"You're not at the counter of the Network!");
  if(sscanf(params,"d", donation)) return SendClientMessage(playerid, COLOR_WHITE, "{c0c0c0}Usage: {ffffff}/netdonate [amount]");
  if(PlayerInfo[playerid][pCash] < donation) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money!");
  if(gdonatevar >= 1000000) return SendClientMessage(playerid, COLOR_GREY, "The donation is closed right now!");
  if(donation < 1 || donation > 1000000) return SendClientMessage(playerid, COLOR_GREY, "You cannot donate less than $1 or more than $1,000,000!");
  {
    GiveMoney(playerid, -donation);
    gdonatevar += donation;
    format(string, sizeof(string), "Thanks for donating $%d to our company!", donation);
    SendClientMessage(playerid, COLOR_GREY, string);
    if(gdonatevar > 1000000) return gdonatevar = 1000000;
         
    SaveNETdata();
  }
  return 1;
}


CMD:netringbell(playerid)
{
  new Juan_virtualworld = GetPlayerVirtualWorld(playerid);
  new Juan_interior = GetPlayerInterior(playerid);
   
  if(!IsPlayerInRangeOfPoint(playerid, 2.0, 1452.5398,-620.8953,1901.2599)) return SendClientMessage(playerid, COLOR_GREY,"You're not at the counter of the Network!");
  {
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
      if(Juan_virtualworld == GetPlayerVirtualWorld(i))
      {
        if(Juan_interior == GetPlayerInterior(i))
        {
          if(IsPlayerInRangeOfPoint(i, 60.0, 1452.5398,-620.8953,1901.2599))
            SendClientMessage(i, COLOR_WHITE, "{ebcd21}A bell rang inside the lobby, jingle jingle.");
        }
      }
    }
  }
  return 1;
}

CMD:netlockdown(playerid)
{
  new Juan_virtualworld = GetPlayerVirtualWorld(playerid);
  new Juan_interior = GetPlayerInterior(playerid);

  if(netkey[playerid] != 1) return SendClientMessage(playerid, COLOR_GREY, "You don't have the Network keys!");
  if(!IsPlayerInRangeOfPoint(playerid, 2.0, 1449.7556,-628.7351,1950.3143)) return SendClientMessage(playerid, COLOR_GREY,"You're not at the CCTV room of the Network!");
  {
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
      if(Juan_virtualworld == GetPlayerVirtualWorld(i))
      {
        if(Juan_interior == GetPlayerInterior(i))
        {
          if(IsPlayerInRangeOfPoint(i, 80.0, 1452.5398,-620.8953,1901.2599) || IsPlayerInRangeOfPoint(i, 80.0, 1449.7556,-628.7351,1950.3143) || IsPlayerInRangeOfPoint(i, 80.0, 1461.2075,-630.1036,1994.6597))
            SendClientMessage(i, COLOR_WHITE, "{ebcd21}NETWORK COMPANY IS ON LOCKDOWN, NOBODY IS ALLOWED TO LEAVE/ENTER!!!");
        }
      }
    }
  }
  return 1;
}



CMD:netmessage(playerid, params[])
{
  new string[123];
    
  if(netkey[playerid] != 1) return SendClientMessage(playerid, COLOR_GREY, "You don't have the Network keys!");
  if(!IsPlayerInRangeOfPoint(playerid, 2.0, 1449.7556,-628.7351,1950.3143)) return SendClientMessage(playerid, COLOR_GREY,"You're not at the CCTV room of the Network!");

  if(sscanf(params,"s[123]", string)) return SendClientMessage(playerid, COLOR_WHITE, "{c0c0c0}Usage: {ffffff}/netmessage [announcement]");
  {
    format(string, sizeof(string), "{808040}Network Management: %s", string);
    SendNetworkMessage(COLOR_GREY, string);
  }

  return 1;
}

CMD:nethelp(playerid)
{
  if(!IsPlayerInRangeOfPoint(playerid, 80.0, 1452.5398,-620.8953,1901.2599)) return SendClientMessage(playerid, COLOR_GREY,"You're not at the lobby of the Network!");
  if(netkey[playerid] == 1)
  {
    SendClientMessage(playerid, -1, "{808040}_______The Network Help Menu_________");
    SendClientMessage(playerid, COLOR_GREY, "/netringbell /netlockdown /netmessage /netdonate /netbalance");
  }
  else
  {
    SendClientMessage(playerid, -1, "{808040}_______The Network Help Menu_________");
    SendClientMessage(playerid, COLOR_GREY, "/netdonate /netringbell");
  }

  return 1;
}

CMD:netbalance(playerid, params[])
{
  new string[123], amount;

  if(netkey[playerid] != 1) return SendClientMessage(playerid, COLOR_GREY, "You don't have the Network keys!");
  if(!IsPlayerInRangeOfPoint(playerid, 5.0, 1464.1365,-618.7394,1994.6597)) return SendClientMessage(playerid, COLOR_GREY,"You're not at the office of the Network!");

  if(sscanf(params, "i", amount))
  {
    SendClientMessage(playerid, COLOR_GREY, "{c0c0c0}Usage:{FFFFFF} /netbalance [amount]");
    format(string, sizeof(string), "There's $%d inside the safe.", gdonatevar);
    SendClientMessage(playerid, COLOR_GREY, string);
    return 1;
  }
  else
  {
    if(amount > gdonatevar || amount < 1) return SendClientMessage(playerid, COLOR_GREY, "You can't withdraw less than $1 or safe doesn't have that much yet!");
    GiveMoney(playerid, amount);
    gdonatevar -= amount;
    format(string, sizeof(string), "You have withdrawn $%d from the safe. New balance: $%d.", amount, gdonatevar);
    SendClientMessage(playerid, COLOR_GREY, string);
        
    SaveNETdata();
  }

  return 1;
}