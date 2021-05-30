Hook:F_OnGameModeInit()
{
    CreateDynamicActor(106, 2516.3745,-1676.1100,13.9699, 75.0);
    CreateDynamicActor(104, 1995.1755,-1115.8999,26.7737, 180.0);
    Create3DTextLabel("{006f00}Grove Street\n{ffffff}Press Y to join.", 0x008080FF, 675.1188,-2294.3250,5.8098, 40.0, 0, 1);
    Create3DTextLabel("{b01ac6}Front Yard Ballas\n{ffffff}Press Y to join.", 0x008080FF, 675.1188,-2294.3250,5.8098, 40.0, 0, 1);
    return 1;
}


Hook:F_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(PRESSED(KEY_YES))
    {
       if(IsPlayerInRangeOfPoint(playerid, 5.0, 2516.3745, -1676.1100, 13.9699))
       {
             /*if(PlayerInfo[playerid][pMember] != 0 || PlayerInfo[playerid][pLeader] != 0) return SendClientMessage(playerid, COLOR_GREY, "You're already in a faction (/quitfaction) first.");
             if(PlayerInfo[playerid][pFMember] != 255) return SendClientMessage(playerid, COLOR_GREY, "You're already in a family (/quitfamily) first.");
             if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can not be in a vehicle to do this.");
             PlayerInfo[playerid][pFMember] = 0;
             PlayerInfo[playerid][pRank] =1;
             FamilyInfo[PlayerInfo[playerid][pFMember]][FamilyMembers] ++;
             SaveFamilies();
             SendClientMessage(playerid, COLOR_LIGHTBLUE, "You've agreed to the family rules and joined Grove Street as Rank#1!");
             SendClientMessage(playerid, COLOR_GREY, "SERVER: Make sure you've read rules(/rules and /pointrules). Good Luck!");*/
             ShowPlayerDialog(playerid, 3214, DIALOG_STYLE_MSGBOX, "Do you really want to join Grove Street?", "Are you sure? If so, press the ''Yes'' button, if no, press the ''N'' button.", "Yes", "No");
       }
       else if(IsPlayerInRangeOfPoint(playerid, 5.0, 1995.1755,-1115.8999,26.7737))
       {
             /*if(PlayerInfo[playerid][pMember] != 0 || PlayerInfo[playerid][pLeader] != 0) return SendClientMessage(playerid, COLOR_GREY, "You're already in a faction (/quitfaction) first.");
             if(PlayerInfo[playerid][pFMember] != 255) return SendClientMessage(playerid, COLOR_GREY, "You're already in a family (/quitfamily) first.");
             if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can not be in a vehicle to do this.");
             PlayerInfo[playerid][pFMember] = 1;
             PlayerInfo[playerid][pRank] = 1;
             FamilyInfo[PlayerInfo[playerid][pFMember]][FamilyMembers] ++;
             SaveFamilies();
             SendClientMessage(playerid, COLOR_LIGHTBLUE, "You've agreed to the family rules and joined Front Yard Ballas as Rank#1!");
             SendClientMessage(playerid, COLOR_GREY, "SERVER: Make sure you've read rules(/rules and /pointrules). Good Luck!");*/
             ShowPlayerDialog(playerid, 3215, DIALOG_STYLE_MSGBOX, "Do you really want to join Balls?", "Are you sure? If so, press the ''Yes'' button, if no, press the ''N'' button.", "Yes", "No");
       }
       return 1;
    }
    return 1;
}
Hook:fa_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
if(dialogid == 3214)
{
        if(!response)
        {
            SendClientMessage(playerid, COLOR_GREY, "Alright, so.. You do not want to join Grove Street.");
            return 1;
        }
        else
        {
                if(PlayerInfo[playerid][pMember] != 0 || PlayerInfo[playerid][pLeader] != 0) return SendClientMessage(playerid, COLOR_GREY, "You're already in a faction (/quitfaction) first.");
                if(PlayerInfo[playerid][pFMember] != 255) return SendClientMessage(playerid, COLOR_GREY, "You're already in a family (/quitfamily) first.");
                if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can not be in a vehicle to do this.");
                PlayerInfo[playerid][pFMember] = 2;
                PlayerInfo[playerid][pRank] =1;
                FamilyInfo[PlayerInfo[playerid][pFMember]][FamilyMembers] ++;
                SaveFamilies();
                SendClientMessage(playerid, COLOR_LIGHTBLUE, "You've agreed to the family rules and joined Grove Street as Rank#1!");
                SendClientMessage(playerid, COLOR_GREY, "SERVER: Make sure you've read rules(/rules and /pointrules). Good Luck!");
                return 1;
       }
}
if(dialogid == 3215)
{
          if(!response)
          {
              SendClientMessage(playerid, COLOR_GREY, "Alright, so.. You do not want to join Ballas..");
              return 1;
          }
          else
          {
                  if(PlayerInfo[playerid][pMember] != 0 || PlayerInfo[playerid][pLeader] != 0) return SendClientMessage(playerid, COLOR_GREY, "You're already in a faction (/quitfaction) first.");
                  if(PlayerInfo[playerid][pFMember] != 255) return SendClientMessage(playerid, COLOR_GREY, "You're already in a family (/quitfamily) first.");
                  if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can not be in a vehicle to do this.");
                  PlayerInfo[playerid][pFMember] = 1;
                  PlayerInfo[playerid][pRank] = 1;
                  FamilyInfo[PlayerInfo[playerid][pFMember]][FamilyMembers] ++;
                  SaveFamilies();
                  SendClientMessage(playerid, COLOR_LIGHTBLUE, "You've agreed to the family rules and joined Front Yard Ballas as Rank#1!");
                  SendClientMessage(playerid, COLOR_GREY, "SERVER: Make sure you've read rules(/rules and /pointrules). Good Luck!");
                  return 1;
          }
}
return 1;
}