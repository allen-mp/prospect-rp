/* Dynamic PNS system by Hank James. 
Version 1.0. 
MYSQL will be by Shadow. 
SARP(San Andreas Roleplay)*/

#define MAX_PNS 20
new Iterator:PNS<MAX_PNS>;
enum pnsInfo
{
  Float:pnsPos[3],
  Float:pnsIPos[3],
  pnsInUse,
  Text3D:pnsLabel,
  pnsPickup
}
new PNSINFO[MAX_PNS][pnsInfo];


// START OF COMMANDS.

CMD:createpns(playerid, params[])
{
   new Float:x, Float:y, Float:z, id = Iter_Free( PNS ), query[ 450 ], ssr[ 500 ];
   new str[128];
   if(PlayerInfo[playerid][pAdmin] < 1337) return SendClientMessage(playerid, COLOR_RED, "Error: {FFFFFF}You are not authorized to use this command.");
   GetPlayerPos(playerid, x, y, z);
   format(str, sizeof(str), "[PNS SYSTEM]: Icon created under ID: %d. Use /editpns to edit the PNS location.", id);
   SendClientMessage(playerid, COLOR_YELLOW, str);
   PNSINFO[id][pnsPos][0] = x;
   PNSINFO[id][pnsPos][1] = y;
   PNSINFO[id][pnsPos][2] = z;
   format( ssr, sizeof( ssr ), "To use the PNS:\n{FFFFFF}/usepns\nFee: $10,000\nID: %d", id );
   PNSINFO[id][pnsPickup] = CreateDynamicPickup(1239, 29, PNSINFO[id][pnsPos][0], PNSINFO[id][pnsPos][1], PNSINFO[id][pnsPos][2], 0);
   PNSINFO[id][pnsLabel] = PNSINFO[id][pnsLabel] = CreateDynamic3DTextLabel(ssr, COLOR_YELLOW, PNSINFO[id][pnsPos][0], PNSINFO[id][pnsPos][1], PNSINFO[id][pnsPos][2], 30.0);
   Iter_Add( PNS, id );
   mysql_format( sqlGameConnection, query, sizeof( query ), "INSERT INTO `PNS`(`ID`,`pnsX`, `pnsY`, `pnsZ`)VALUES('%d','%f', '%f', '%f')",  id, PNSINFO[id][pnsPos][0], PNSINFO[id][pnsPos][1], PNSINFO[id][pnsPos][2] );
   mysql_pquery( sqlGameConnection, query );
   return 1;
}

CMD:editpns(playerid, params[])
{
   new str[128], query[ 350 ];
   new id;
   new Float:x, Float:y, Float:z;
   if(PlayerInfo[playerid][pAdmin] < 1337) return SendClientMessage(playerid, COLOR_RED, "Error: {FFFFFF}You are not authorized to use this command.");
   GetPlayerPos(playerid, x, y, z);
   if(sscanf(params, "d", id)) return SendClientMessage(playerid, COLOR_LIGHTBLUE, "Usage: {FFFFFF}/editpns [ID]");
   if( !Iter_Contains( PNS, id ) ) return SendClientMessage( playerid, COLOR_GREY, "That PNS does not exist!" );
   PNSINFO[id][pnsIPos][0] = x;
   PNSINFO[id][pnsIPos][1] = y;
   PNSINFO[id][pnsIPos][2] = z;
   format(str, sizeof(str), "[PNS SYSTEM]: PNS ID (%d) interior position was set. It's ready to be used now.", id);
   SendClientMessage(playerid, COLOR_YELLOW, str);
   mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `PNS` SET `pnsIX`='%f', `pnsIY`='%f', `pnsIZ`='%f' WHERE `ID`='%d'",  PNSINFO[id][pnsIPos][0], PNSINFO[id][pnsIPos][1], PNSINFO[id][pnsIPos][2], id );
   mysql_pquery( sqlGameConnection, query );
   return 1;
}

CMD:usepns(playerid, params[])
{
   if(IsPlayerInRangeOfPNS(playerid) == 0) return SendClientMessage(playerid, COLOR_GREY, "You are not in range of any PNS icon.");
   if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_RED, "You are not in any vehicle.");
   if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendClientMessage(playerid, COLOR_GREY, "You are not driving any vehicle.");
   if(PlayerInfo[playerid][pCash] < 10000) return SendClientMessage(playerid, COLOR_GREY, "You need at least $10,000 for repairing your vehicle at this PNS.");
   new id = GetNearestPNS(playerid);
   if(PNSINFO[id][pnsInUse] == 1) return SendClientMessage(playerid, COLOR_YELLOW, "Please wait, the PNS is in use at the moment...");
   SetVehiclePos(GetPlayerVehicleID(playerid), PNSINFO[id][pnsIPos][0], PNSINFO[id][pnsIPos][1], PNSINFO[id][pnsIPos][2]);
   PNSINFO[id][pnsInUse] = 1;
   GiveMoney(playerid, -10000);
   SendClientMessage(playerid, COLOR_LIGHTBLUE, "You will be moved out in 10 seconds, please wait...");
   TogglePlayerControllable(playerid, 0);
   SetTimerEx("OutPNS", 10000, false, "ii", id, playerid);
   return 1;
}

CMD:deletepns(playerid, params[])
{
   new str[128], id, query[ 350 ];
   if(PlayerInfo[playerid][pAdmin] < 1337) return SendClientMessage(playerid, COLOR_RED, "Error: {FFFFFF}You are not authorized to use this command.");
   if(IsPlayerInRangeOfPNS(playerid) == 0) return SendClientMessage(playerid, COLOR_GREY, "You are not in range of any PNS icon.");
   if(sscanf(params, "d", id)) return SendClientMessage(playerid, COLOR_LIGHTBLUE, "Usage: {FFFFFF}/deletepns [ID]");
   if( !Iter_Contains( PNS, id ) ) return SendClientMessage( playerid, COLOR_GREY, "That PNS does not exist!" );
   PNSINFO[id][pnsPos][0] = 0.0;
   PNSINFO[id][pnsPos][1] = 0.0;
   PNSINFO[id][pnsPos][2] = 0.0;
   DestroyDynamicPickup(PNSINFO[id][pnsPickup]);
   DestroyDynamic3DTextLabel(Text3D:PNSINFO[id][pnsLabel]);
   PNSINFO[id][pnsIPos][0] = 0.0;
   PNSINFO[id][pnsIPos][1] = 0.0;
   PNSINFO[id][pnsIPos][2] = 0.0;
   PNSINFO[id][pnsInUse] = 0;
   format(str, sizeof(str), "[PNS SYSTEM] PNS icon (%d) has been sucessfully deleted.", id);
   SendClientMessage(playerid, COLOR_YELLOW, str);
   Iter_Remove( PNS, id );
   mysql_format( sqlGameConnection, query, sizeof( query ), "DELETE FROM `PNS` WHERE `ID`='%d'",  id );
   mysql_pquery( sqlGameConnection, query );
   return 1;
}

// END OF COMMANDS.

// START OF FUNCTIONS.

forward OutPNS(id, playerid);
public OutPNS(id, playerid)
{
   PNSINFO[id][pnsInUse] = 0;
   SetVehiclePos(GetPlayerVehicleID(playerid), PNSINFO[id][pnsPos][0], PNSINFO[id][pnsPos][1], PNSINFO[id][pnsPos][2]);
   SendClientMessage(playerid, COLOR_YELLOW, "Thanks for using our PNS, your fee was $10,000. Drive safe!");
   TogglePlayerControllable(playerid, 1);
   return 1;
}
   

stock IsPlayerInRangeOfPNS(playerid)
{
	for(new i = 0; i < MAX_PNS; i++)
	{
	  if(IsPlayerInRangeOfPoint(playerid, 5.0, PNSINFO[i][pnsPos][0], PNSINFO[i][pnsPos][1], PNSINFO[i][pnsPos][2]))
	  {
		 return 1;
      }
    }
    return 0;
}

stock GetNearestPNS(playerid)
{
	for(new i = 0; i < MAX_PNS; i++)
	{
	  if(IsPlayerInRangeOfPoint(playerid, 5.0, PNSINFO[i][pnsPos][0], PNSINFO[i][pnsPos][1], PNSINFO[i][pnsPos][2]))
	  {
		 return i;
      }
    }
    return 0;
}

forward LoadPNS();
public LoadPNS()
{
  new str[ 500 ];
  new rows = cache_num_rows( );
  if( rows )
  {
      new id, loaded;
      while( loaded < rows )
      {
            id = cache_get_field_content_int( loaded, "ID" );
            PNSINFO[ id ] [ pnsPos ] [ 0 ] = cache_get_field_content_float( loaded, "pnsX" );
            PNSINFO[ id ] [ pnsPos ] [ 1 ] = cache_get_field_content_float( loaded, "pnsY" );
            PNSINFO[ id ] [ pnsPos ] [ 2 ] = cache_get_field_content_float( loaded, "pnsZ" );
            PNSINFO[ id ] [ pnsIPos ] [ 0 ] = cache_get_field_content_float( loaded, "pnsIX" );
            PNSINFO[ id ] [ pnsIPos ] [ 1 ] = cache_get_field_content_float( loaded, "pnsIY" );
            PNSINFO[ id ] [ pnsIPos ] [ 2 ] = cache_get_field_content_float( loaded, "pnsIZ" );
            PNSINFO[ id ] [ pnsPickup ] = CreateDynamicPickup(1239, 29, PNSINFO[ id ] [ pnsPos ] [ 0 ], PNSINFO[ id ] [ pnsPos ] [ 1 ], PNSINFO[ id ] [ pnsPos ] [ 2 ], 0);
            format( str, sizeof( str ), "To use the PNS:\n{FFFFFF}/usepns\nFee: $10,000\nID: %d", id );
            PNSINFO[ id ] [ pnsLabel ] = PNSINFO[ id ] [ pnsLabel ] = CreateDynamic3DTextLabel(str, COLOR_YELLOW, PNSINFO[ id ] [ pnsPos ] [ 0 ], PNSINFO[ id ] [ pnsPos ] [ 1 ], 
              PNSINFO[ id ] [ pnsPos ] [ 2 ], 30.0);
            Iter_Add( PNS, id );
            loaded ++;
      }
      printf( "Loaded %d PNS's.", loaded );
  }
  else
  {
    printf("Couldn't find any PNS's");
  } 
  return 1;
}

// END OF FUNCTIONS.
