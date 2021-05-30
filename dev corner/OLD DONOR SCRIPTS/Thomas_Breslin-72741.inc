/*

	Thomas Breslin scripting

 */


static bool:clubkey[MAX_PLAYERS], bool:gasmask[MAX_PLAYERS], bool:CanUseVaultAd[MAX_PLAYERS];

#define THOMAS_INTERIOR	-2055.7698, 333.2030, 1023.3812
#define THOMAS_EXTERIOR	942.9832, -1742.7632, 13.5546

#define THOMAS_SQL 72741

hook Th_OnPlayerDisconnect(playerid, reason)
{
	clubkey[playerid] = false;
	gasmask[playerid] = false;

	CanUseVaultAd[playerid] = true;
	return 1;
}

CMD:vgaschamber(playerid, params[])
{
	if(PlayerInfo[playerid][pID] != THOMAS_SQL)
		return SendClientMessage(playerid, -1, "You are not Thomas!");

	if(!IsPlayerInRangeOfPoint(playerid, 3.0, -2051.4673,363.2951,1028.1366))
		return SendClientMessage(playerid, COLOR_GREY, "You're not near the activation point for the gas chamber.");

	foreach(new i : Player)
	{
		if(IsPlayerInRangeOfPoint(i, 5.0, -2051.4819, 352.7324, 1028.1554))
		{
			if(gasmask[i] == false)
			{
				SetPlayerHealth(i, 0);
				SendClientMessage(i, COLOR_PURPLE, "* Your lungs would start to fill with toxins, and you'd eventually pass out");
			}
		}
	}

	SendClientMessage(playerid, -1, "You've used the gas chamber!");
	return 1;
}

CMD:vgasmask(playerid, params[])
{
	if(clubkey[playerid] == true || PlayerInfo[playerid][pID] == THOMAS_SQL)
	{
		if(gasmask[playerid] == true)
		{
			gasmask[playerid] = false;
			SendClientMessage(playerid, -1, "* You've taken off your gas mask!");
		} else {
			gasmask[playerid] = true;
			SendClientMessage(playerid, -1, "* You've put on your gas mask!");
		}
	} else {
		SendClientMessage(playerid, -1, "You're not authorised to use this command!");
	}
	return 1;
}

CMD:vaultad(playerid, params[])
{
	if(PlayerInfo[playerid][pID] == THOMAS_SQL)
	{
		if(CanUseVaultAd[playerid] == true)
		{
			SetTimerEx("vaultadtimer", 60 * 1000, 0, "i", playerid);
			SendClientMessageToAll(TEAM_GROVE_COLOR, "The Vault is open now! Come down to enjoy the party! (( /vaultgps ))");
		}	else {
			SendClientMessage(playerid, COLOR_GREY, "You must wait one minute before sending another vault ad.");
		}
	}
	return 1;
}

CMD:vaultgps(playerid, params[])
{
	SendClientMessage(playerid, COLOR_GREY, "A checkpoint has been marked on your map The Vault.");
	SetPlayerCheckpointEx(playerid, THOMAS_EXTERIOR, 3.0);
	return 1;
}

CMD:vhelp(playerid, params[])
{
	SendClientMessage(playerid, COLOR_ORANGE, "                       [ Thomas Breslin's Commands ]");
	SendClientMessage(playerid, COLOR_ORANGE, "/vgasmask /vgaschamber /vaultad /vaultgps");
	return 1;
}

CMD:vgivekey(playerid, params[])
{
	new target;
	if(sscanf(params, "u", target))
		return SendClientMessage(playerid, COLOR_LIGHTBLUE, "USAGE:{FFFFFF} /vgivekey [ID/PartOfName]");

	if(PlayerInfo[playerid][pID] != THOMAS_SQL)
		return SendClientMessage(playerid, COLOR_GREY, "You are not Thomas.");

	if(clubkey[target] == true)
		return SendClientMessage(playerid, COLOR_GREY, "This player already has a club key.");

	clubkey[target] = true;

	SendClientMessage(playerid, COLOR_LIGHTBLUE, "You've given the specified player a key to The Vault.");

	SendClientMessage(target, COLOR_LIGHTBLUE, "You've been given a key to The Vault.");
	return 1;
}

forward vaultadtimer(playerid);
public vaultadtimer(playerid)
{
	CanUseVaultAd[playerid] = true;
	SendClientMessage(playerid, COLOR_GREY, "You can use the vault ad again.");
	return 1;
}