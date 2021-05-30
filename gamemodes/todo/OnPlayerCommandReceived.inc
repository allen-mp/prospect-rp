// Convert to ZCMD
//==============================================================================
static str[144], tmp[144], sendername[MAX_PLAYER_NAME+1], giveplayer[MAX_PLAYER_NAME+1];
public OnPlayerCommandReceived(playerid, cmdtext[])
{
	if(!gamemodeLoaded) return SendClientMessage(playerid, COLOR_GREY, "Please wait until the gamemode finishes loading.");
	new cmd[64], idx;
	cmd = strtok(cmdtext, idx);
	if(gPlayerSpawned[playerid] == 0)
	{
		SendClientMessage(playerid, COLOR_GRAD1, "You are not logged in or have not spawned.");
		return 0;
	}
	if(PlayerInfo[playerid][pAdmin] > 0 && gAdminAuthorized[playerid] == 0)
	{
		format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s (ID %d) was kicked for attempting to use commands without authorizing.", PlayerICName(playerid), playerid);
		UpdateWarnings(str);
		KickEx(playerid);
		return 0;
	}
	if(strcmp(cmd, "/pdcctv", true, 6) && watchingPDCCTV[playerid]) {
		SendClientMessage(playerid, COLOR_GRAD1, "You can only use the /pdcctv command at the moment.");
		return 0;
	}
	if(strcmp(cmd, "/mute", true, 4) && strcmp(cmd, "/report", true, 6) && PlayerInfo[playerid][pMuted] == 1)
	{
		SendClientMessage(playerid, COLOR_GRAD1, "You can't speak, you're muted.");
		return 0;
	}
	
	//anti server ad
	if(PlayerInfo[playerid][pAdmin] < 1)
	{
		new i_numcount, i_period, i_pos;
		if(strfind(cmdtext, ":", true) != -1)
		{
			while(cmdtext[i_pos])
			{
				if('0' <= cmdtext[i_pos] <= '9') i_numcount++;
				else if(cmdtext[i_pos] == '.') i_period++;
				i_pos++;
			}
			if(i_numcount >= 8 && i_period >= 3)
			{
				format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s [%d] has attempted to advertise another server.", PlayerICName(playerid), playerid);
				ABroadCast(COLOR_WHITE, str, 1);
				format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: '%s'", cmdtext);
				ABroadCast(COLOR_GRAD3, str, 1);
				return 0;
			}
		}
		else // If there is no colon
		{
			if(!strcmp(cmd, "/newb", true, 4) || !strcmp(cmd, "/newbie", true, 6))
			{
				if(i_numcount >= 8 && PlayerInfo[playerid][pConnectTime] < 3)
				{
					format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s [%d] may have attempted to advertise another server.", PlayerICName(playerid), playerid);
					ABroadCast(COLOR_WHITE, str, 1);
					format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: '%s'", cmdtext);
					ABroadCast(COLOR_GRAD3, str, 1);
					return 0;
				}
			}
		}
	}
	return 1;
}


//////// Timer /////////////

Timer:stopCMD(playerid) return pCommandUsed[playerid] = false;

Timer:DropWepTimer(playerid)
{
	if(!IsPlayerConnected(playerid)) return 0;
	if(DropWeaponTimer[playerid] == 1)
	{
		DropWeaponTimer[playerid] = 0;
	}
	return 1;
}

Timer:UseFare(playerid)
{
	if(!IsPlayerConnected(playerid)) return 0;
	if(UseFareTimer[playerid])
	{
		UseFareTimer[playerid] = 0;
	}
	return 1;
}

Timer:SellGun(playerid)
{
	if(!IsPlayerConnected(playerid)) return 0;
	if(SellGunTimer[playerid])
	{
		SellGunTimer[playerid] = 0;
	}
	return 1;
}

Timer:Fillup(playerid)
{
	new FillUp;
	new vehicleid = GetPlayerVehicleID(playerid);
	FillUp = 100 - Gas[vehicleid];
	if(FillUp > 100)
	{
		SendClientMessage(playerid, COLOR_GREY, "The vehicle fuel tank is full.");
		Refueling[playerid] = 0;
		return 1;
	}
	if(Refueling[playerid] == 1)
	{
		if(PlayerInfo[playerid][pCash] >= FillUp +4)
		{
			Gas[vehicleid] += FillUp;
			FillUp = FillUp * 8;
			format(str, sizeof(str), "~w~ You filled up your vehicle~n~ for ~g~$%d~w~.", FillUp);
			displayCenterHUDInfo(playerid,  str, 8); //display for 8 seconds
			GiveMoney(playerid, - FillUp);
			Refueling[playerid] = 0;
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
			Refueling[playerid] = 0;
		}
	}
	return 1;
}

Timer:UseAccept(playerid)
{
	if(!IsPlayerConnected(playerid)) return 0;
	if(UseAcceptTimer[playerid])
	{
		UseAcceptTimer[playerid] = 0;
	}
	return 1;
}

Timer:WeedPick(playerid, weed)
{
	/*
	
		Pending re-work by Sparke

	 */
	if(!IsPlayerConnected(playerid)) return 0;
	if(WeedPickTimer[playerid])
	{
		new Float:X, Float:Y, Float:Z;
		GetPlayerPos(playerid, X, Y, Z);
		if(WeedPickPosition[playerid][PosX] == X && WeedPickPosition[playerid][PosY] == Y && WeedPickPosition[playerid][PosZ] == Z)
		{
			if(GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_DUCK)
			{
				if(WeedInfo[weed][WeedPlanted] == 1)
				{
					format(str, sizeof(str), "You have picked a weed plant and received %d grams of pot.", WeedInfo[weed][WeedGrams]);
					SendClientMessage(playerid, COLOR_GREEN, str);
					format(str, sizeof(str), "* %s picks a weed plant.", PlayerICName(playerid));
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					format(str, sizeof(str), "~w~Found %d grams of pot", WeedInfo[weed][WeedGrams]);
					displayCenterHUDInfo(playerid, str, 8);
					PlayerInfo[playerid][pCannabis] += WeedInfo[weed][WeedGrams];
					if(weedObject[weed]) { DestroyDynamicObjectEx(weedObject[weed]); }
					weedObject[weed] = 0;
					GotPlant[playerid] = 1;
					OldWeedX[playerid] = WeedInfo[weed][WeedX];
					OldWeedY[playerid] = WeedInfo[weed][WeedY];
					OldWeedZ[playerid] = WeedInfo[weed][WeedZ];
					WeedInfo[weed][WeedX] = 0.0;
					WeedInfo[weed][WeedY] = 0.0;
					WeedInfo[weed][WeedZ] = 0.0;
					WeedInfo[weed][WeedTime] = 0;
					WeedInfo[weed][WeedPlanted] = 0;
					WeedInfo[weed][WeedGrams] = 0;
					strmid(WeedInfo[weed][WeedPlanter], "[NONE]", 0, strlen("[NONE]"), 128);
					WeedPickTimer[playerid] = 0;
				}
				else
				{
					WeedPickTimer[playerid] = 0;
					SendClientMessage(playerid, COLOR_GREEN, "This weed does not exist anymore.");
				}
			}
			else
			{
				WeedPickTimer[playerid] = 0;
				SendClientMessage(playerid, COLOR_GREEN, "You are not crouched, picking failed.");
			}
		}
		else
		{
			WeedPickTimer[playerid] = 0;
			SendClientMessage(playerid, COLOR_GREEN, "You have moved from your weed picking position, picking failed.");
		}
	}
	return 1;
}
/////////////////////////////////


public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
	if(!gamemodeLoaded) return SendClientMessage(playerid, COLOR_GREY, "Please wait until the gamemode finishes loading.");
	
	new cmd[64];
	new giveplayerid, moneys, idx;
	cmd = strtok(cmdtext, idx);
	strmid(sendername, PlayerName(playerid), 0, MAX_PLAYER_NAME);
	if(strcmp(cmd, "/changepass", true) != 0 && strcmp(cmd, "/ochangepass", true) != 0) printf("[cmd] [%s] %s", sendername, cmdtext);
	if(success) return 1;
	if(pCommandUsed[playerid] == false)
	{
		KillTimer(AFK_Timer[playerid]);
		pCommandUsed[playerid] = true;
		AFK_Timer[playerid] = SetTimerEx("Timer_stopCMD", 600000, false, "i", playerid); // 10 minutes
	}
	if(strcmp(cmd, "/fedit", true) == 0)
	{
		if(PlayerInfo[playerid][pGangDirector] < 1 && PlayerInfo[playerid][pAdmin] < 1337)
		{
			SendClientMessage(playerid, COLOR_GRAD2, "You are not authorized to use this command.");
			return 1;
		}
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fedit [family] [name] [amount]");
			SendClientMessage(playerid, COLOR_GREY, "Available names: MaxSkins, Skin1, Skin2, Skin3, Skin4, Skin5, Skin6, Skin7, Skin8");
			return 1;
		}
		new family = strvalEx(tmp);
		if(family < 1 || family > 20)
		{
			SendClientMessage(playerid, COLOR_GREY, "Family can't be below 1 or above 20.");
			return 1;
		}
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fedit [family] [name] [amount]");
			SendClientMessage(playerid, COLOR_GREY, "Available names: MaxSkins, Skin1, Skin2, Skin3, Skin4, Skin5, Skin6, Skin7, Skin8");
			return 1;
		}
		if(strcmp(tmp, "maxskins", true) == 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fedit [family] [name] [amount]");
				SendClientMessage(playerid, COLOR_GREY, "Available names: MaxSkins, Skin1, Skin2, Skin3, Skin4, Skin5, Skin6, Skin7, Skin8");
				return 1;
			}
			new amount = strvalEx(tmp);
			if(amount < 0 || amount > 8)
			{
				SendClientMessage(playerid, COLOR_GREY, "MaxSkins can't be below 0 or above 8.");
				return 1;
			}
			FamilyInfo[family -1][FamilySkins] = amount;
			format(str, sizeof(str), "You have adjusted the MaxSkins of Family %d to %d", family, amount);
			SendClientMessage(playerid, COLOR_WHITE, str);
			SaveFamilies();
			return 1;
		}
		if(strcmp(tmp, "skin1", true) == 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fedit [family] [name] [amount]");
				SendClientMessage(playerid, COLOR_GREY, "Available names: MaxSkins, Skin1, Skin2, Skin3, Skin4, Skin5, Skin6, Skin7, Skin8");
				return 1;
			}
			new amount = strvalEx(tmp);
			if(amount < 0 || amount > 311)
			{
				SendClientMessage(playerid, COLOR_GREY, "Skin can't be below 0 or above 311.");
				return 1;
			}
			FamilyInfo[family -1][FamilySkin1] = amount;
			format(str, sizeof(str), "You have adjusted Family %d's Skin1 to Skin ID %d", family, amount);
			SendClientMessage(playerid, COLOR_WHITE, str);
			SaveFamilies();
			return 1;
		}
		if(strcmp(tmp, "skin2", true) == 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fedit [family] [name] [amount]");
				SendClientMessage(playerid, COLOR_GREY, "Available names: MaxSkins, Skin1, Skin2, Skin3, Skin4, Skin5, Skin6, Skin7, Skin8");
				return 1;
			}
			new amount = strvalEx(tmp);
			if(amount < 0 || amount > 311)
			{
				SendClientMessage(playerid, COLOR_GREY, "Skin can't be below 0 or above 311.");
				return 1;
			}
			FamilyInfo[family -1][FamilySkin2] = amount;
			format(str, sizeof(str), "You have adjusted Family %d's Skin2 to Skin ID %d", family, amount);
			SendClientMessage(playerid, COLOR_WHITE, str);
			SaveFamilies();
			return 1;
		}
		if(strcmp(tmp, "skin3", true) == 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fedit [family] [name] [amount]");
				SendClientMessage(playerid, COLOR_GREY, "Available names: MaxSkins, Skin1, Skin2, Skin3, Skin4, Skin5, Skin6, Skin7, Skin8");
				return 1;
			}
			new amount = strvalEx(tmp);
			if(amount < 0 || amount > 311)
			{
				SendClientMessage(playerid, COLOR_GREY, "Skin can't be below 0 or above 311.");
				return 1;
			}
			FamilyInfo[family -1][FamilySkin3] = amount;
			format(str, sizeof(str), "You have adjusted Family %d's Skin3 to Skin ID %d", family, amount);
			SendClientMessage(playerid, COLOR_WHITE, str);
			SaveFamilies();
			return 1;
		}
		if(strcmp(tmp, "skin4", true) == 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fedit [family] [name] [amount]");
				SendClientMessage(playerid, COLOR_GREY, "Available names: MaxSkins, Skin1, Skin2, Skin3, Skin4, Skin5, Skin6, Skin7, Skin8");
				return 1;
			}
			new amount = strvalEx(tmp);
			if(amount < 0 || amount > 311)
			{
				SendClientMessage(playerid, COLOR_GREY, "Skin can't be below 0 or above 311.");
				return 1;
			}
			FamilyInfo[family -1][FamilySkin4] = amount;
			format(str, sizeof(str), "You have adjusted Family %d's Skin4 to Skin ID %d", family, amount);
			SendClientMessage(playerid, COLOR_WHITE, str);
			SaveFamilies();
			return 1;
		}
		if(strcmp(tmp, "skin5", true) == 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fedit [family] [name] [amount]");
				SendClientMessage(playerid, COLOR_GREY, "Available names: MaxSkins, Skin1, Skin2, Skin3, Skin4, Skin5, Skin6, Skin7, Skin8");
				return 1;
			}
			new amount = strvalEx(tmp);
			if(amount < 0 || amount > 311)
			{
				SendClientMessage(playerid, COLOR_GREY, "Skin can't be below 0 or above 311.");
				return 1;
			}
			FamilyInfo[family -1][FamilySkin5] = amount;
			format(str, sizeof(str), "You have adjusted Family %d's Skin5 to Skin ID %d", family, amount);
			SendClientMessage(playerid, COLOR_WHITE, str);
			SaveFamilies();
			return 1;
		}
		if(strcmp(tmp, "skin6", true) == 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fedit [family] [name] [amount]");
				SendClientMessage(playerid, COLOR_GREY, "Available names: MaxSkins, Skin1, Skin2, Skin3, Skin4, Skin5, Skin6, Skin7, Skin8");
				return 1;
			}
			new amount = strvalEx(tmp);
			if(amount < 0 || amount > 311)
			{
				SendClientMessage(playerid, COLOR_GREY, "Skin can't be below 0 or above 311.");
				return 1;
			}
			FamilyInfo[family -1][FamilySkin6] = amount;
			format(str, sizeof(str), "You have adjusted Family %d's Skin6 to Skin ID %d", family, amount);
			SendClientMessage(playerid, COLOR_WHITE, str);
			SaveFamilies();
			return 1;
		}
		if(strcmp(tmp, "skin7", true) == 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fedit [family] [name] [amount]");
				SendClientMessage(playerid, COLOR_GREY, "Available names: MaxSkins, Skin1, Skin2, Skin3, Skin4, Skin5, Skin6, Skin7, Skin8");
				return 1;
			}
			new amount = strvalEx(tmp);
			if(amount < 0 || amount > 311)
			{
				SendClientMessage(playerid, COLOR_GREY, "Skin can't be below 0 or above 311.");
				return 1;
			}
			FamilyInfo[family -1][FamilySkin7] = amount;
			format(str, sizeof(str), "You have adjusted Family %d's Skin7 to Skin ID %d", family, amount);
			SendClientMessage(playerid, COLOR_WHITE, str);
			SaveFamilies();
			return 1;
		}
		if(strcmp(tmp, "skin8", true) == 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fedit [family] [name] [amount]");
				SendClientMessage(playerid, COLOR_GREY, "Available names: MaxSkins, Skin1, Skin2, Skin3, Skin4, Skin5, Skin6, Skin7, Skin8");
				return 1;
			}
			new amount = strvalEx(tmp);
			if(amount < 0 || amount > 311)
			{
				SendClientMessage(playerid, COLOR_GREY, "Skin can't be below 0 or above 311.");
				return 1;
			}
			FamilyInfo[family -1][FamilySkin8] = amount;
			format(str, sizeof(str), "You have adjusted Family %d's Skin8 to Skin ID %d", family, amount);
			SendClientMessage(playerid, COLOR_WHITE, str);
			SaveFamilies();
			return 1;
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "Unknown name.");
		}
		return 1;
	}
	if(strcmp(cmd, "/setbizname", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] >= 99999 || PlayerInfo[playerid][pBizMod] == 1)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /setbizname [id] [name]");
				return 1;
			}
			new business = strval(tmp);
			if(BizInfo[business][bSeized] == 1) return SendClientMessage(playerid, COLOR_GREY, "That business needs to be owned first.");
			// Fix to allow spaces in business names
			while(idx < strlen(cmdtext) && cmdtext[idx] <= ' ') idx++; // pass all spaces before the biz name
			strmid(tmp, cmdtext, idx, strlen(cmdtext));
			if(!strlen(tmp))
			{
				return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /biz name [newname]");
			}
			if(strlen(tmp) >= 64) return SendClientMessage(playerid, COLOR_GRAD1, "The name length must be lesser than 64 characters.");
			if(!AlphaNumeric(tmp)) return SendClientMessage(playerid, COLOR_GRAD1, "The name can only contain alphabetical/numerical characters.");
			strmid(BizInfo[business][bName], tmp, 0, strlen(tmp), 255);
			format(str, sizeof(str), "You have set business ID %d's name to '%s'.", business, BizInfo[business][bName]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			if(BizInfo[business][bOwned] == 0)
			{
				new pricestring[32];
				new price = BizInfo[business][bValue];
				new j = valstr(pricestring, price);
				while(j >= 4)
				{
					j -= 3;
					strins(pricestring, ",", j);
				}
				strins(pricestring, "$", 0);
				format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nStatus: For Sale\nPrice: %s", BizInfo[business][bName], pricestring);
			}
			else
			{
				// Government seized
				if(BizInfo[business][bSeized] == 1)
				{
					format(str, sizeof(str), "{AFAFAF}%s{AFAFAF}\n{FFFFFF}This business has been seized by the\n{696969}Government", BizInfo[business][bName]);
				}
				// Judicial Department seized 
				else if(BizInfo[business][bSeized] == 2) {
					format(str, sizeof(str), "{AFAFAF}%s{AFAFAF}\n{FFFFFF}This business has been seized by the\n{BA6103}Judicial Department", BizInfo[business][bName]);
				}
				else
				{
					if(BizInfo[business][bLocked] == 1)
					{
						format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s\nEntry Fee: Locked", BizInfo[business][bName], BizInfo[business][bOwner]);
					}
					else
					{
						format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s", BizInfo[business][bName], BizInfo[business][bOwner]);
					}
				}
			}
			UpdateDynamic3DTextLabelText(Text3D:BizText[business], COLOR_BIZTEXT, str);
		}
		else return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this command.");
		return 1;
	}
	if(strcmp(cmd, "/setbizowner", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] >= 99999 || PlayerInfo[playerid][pBizMod] == 1)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /setbizowner [id] [owner name]");
				return 1;
			}
			new business = strval(tmp);
			new length = strlen(cmdtext);
			while((idx < length) && (cmdtext[idx] <= ' '))
			{
				idx++;
			}
			new offset = idx;
			new result[24];
			while((idx < length) && ((idx - offset) < (sizeof(result) - 1)))
			{
				result[idx - offset] = cmdtext[idx];
				idx++;
			}
			result[idx - offset] = EOS;
			if(!strlen(result))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /setbizowner [id] [owner name]");
				return 1;
			}
			strmid(BizInfo[business][bOwner], result, 0, strlen(result), 255);
			format(str, sizeof(str), "You have set business ID %d's owner name to '%s'.", business, BizInfo[business][bName]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			if(BizInfo[business][bOwned] == 0)
			{
				new pricestring[32];
				new price = BizInfo[business][bValue];
				new j = valstr(pricestring, price);
				while(j >= 4)
				{
					j -= 3;
					strins(pricestring, ",", j);
				}
				strins(pricestring, "$", 0);
				format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nStatus: For Sale\nPrice: %s", BizInfo[business][bName], pricestring);
			}
			else
			{
				// Government seized
				if(BizInfo[business][bSeized] == 1)
				{
					format(str, sizeof(str), "{AFAFAF}%s{AFAFAF}\n{FFFFFF}This business has been seized by the\n{696969}Government", BizInfo[business][bName]);
				}
				// Judicial Department seized 
				else if(BizInfo[business][bSeized] == 2) {
					format(str, sizeof(str), "{AFAFAF}%s{AFAFAF}\n{FFFFFF}This business has been seized by the\n{BA6103}Judicial Department", BizInfo[business][bName]);
				}
				else
				{
					if(BizInfo[business][bLocked] == 1)
					{
						format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s\nEntry Fee: Locked", BizInfo[business][bName], BizInfo[business][bOwner]);
					}
					else
					{
						format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s", BizInfo[business][bName], BizInfo[business][bOwner]);
					}
				}
			}
			UpdateDynamic3DTextLabelText(Text3D:BizText[business], COLOR_BIZTEXT, str);
		}
		else return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this command.");
		return 1;
	}
	if(strcmp(cmd, "/setbizenter", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] >= 99999 || PlayerInfo[playerid][pBizMod] == 1)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /setbizenter [bizid]");
				return 1;
			}
			new id = strval(tmp);
			new Float:x, Float:y, Float:z, Float:a;
			GetPlayerPos(playerid, x, y, z);
			BizInfo[id][bOutsideX] = x;
			BizInfo[id][bOutsideY] = y;
			BizInfo[id][bOutsideZ] = z;
			BizInfo[id][bOutsideWorld] = GetPlayerVirtualWorld(playerid);
			BizInfo[id][bOutsideInt] = GetPlayerInterior(playerid);
			GetPlayerFacingAngle(playerid, a);
			BizInfo[id][bOutsideAng] = a -180.0;
			//load pickups
			if(BizInfo[id][bOutsidePickup]) DestroyDynamicPickup(BizInfo[id][bOutsidePickup]);
			BizInfo[id][bOutsidePickup] = CreateDynamicPickup(1272, 23, BizInfo[id][bOutsideX], BizInfo[id][bOutsideY], BizInfo[id][bOutsideZ]);
			//load 3dtext
			if(BizText[id]) DestroyDynamic3DTextLabel(BizText[id]);
			if(BizInfo[id][bOwned] == 0)
			{
				new pricestring[32];
				new price = BizInfo[id][bValue];
				new j = valstr(pricestring, price);
				while(j >= 4)
				{
					j -= 3;
					strins(pricestring, ",", j);
				}
				strins(pricestring, "$", 0);
				format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nStatus: For Sale\nPrice: %s", BizInfo[id][bName], pricestring);
			}
			else if(BizInfo[id][bOwned] == 1)
			{
				// Government seized
				if(BizInfo[id][bSeized] == 1)
				{
					format(str, sizeof(str), "{AFAFAF}%s{AFAFAF}\n{FFFFFF}This business has been seized by the\n{696969}Government", BizInfo[id][bName]);
				}
				// Judicial Department seized 
				else if(BizInfo[id][bSeized] == 2) {
					format(str, sizeof(str), "{AFAFAF}%s{AFAFAF}\n{FFFFFF}This business has been seized by the\n{BA6103}Judicial Department", BizInfo[id][bName]);
				}
				else
				{
					if(BizInfo[id][bLocked] == 1)
					{
						format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s\nEntry Fee: Locked", BizInfo[id][bName], BizInfo[id][bOwner]);
					}
					else
					{
						format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s", BizInfo[id][bName], BizInfo[id][bOwner]);
					}
				}
			}
			BizText[id] = CreateDynamic3DTextLabel(str, COLOR_BIZTEXT, BizInfo[id][bOutsideX], BizInfo[id][bOutsideY], BizInfo[id][bOutsideZ] +0.88, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
			SaveBusinesses();
			format(str, sizeof(str), "You have set business ID %d's enter location.", id);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		}
		else return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this command.");
		return 1;
	}
	if(strcmp(cmd, "/setbizexit", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] >= 99999 || PlayerInfo[playerid][pBizMod] == 1)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /setbizexit [bizid]");
				return 1;
			}
			new id = strval(tmp);
			new Float:x, Float:y, Float:z, Float:a;
			GetPlayerPos(playerid, x, y, z);
			BizInfo[id][bInsideX] = x;
			BizInfo[id][bInsideY] = y;
			BizInfo[id][bInsideZ] = z;
			BizInfo[id][bInsideInt] = GetPlayerInterior(playerid);
			GetPlayerFacingAngle(playerid, a);
			BizInfo[id][bInsideAng] = a;
			//load pickups
			if(BizInfo[id][bInsidePickup]) DestroyDynamicPickup(BizInfo[id][bInsidePickup]);
			BizInfo[id][bInsidePickup] = CreateDynamicPickup(1272, 23, BizInfo[id][bInsideX], BizInfo[id][bInsideY], BizInfo[id][bInsideZ], id);
			SaveBusinesses();
			format(str, sizeof(str), "You have set business ID %d's exit location.", id);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		}
		else return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this command.");
		return 1;
	}
	if(strcmp(cmd, "/setbizprod", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] >= 99999 || PlayerInfo[playerid][pBizMod] == 1)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /setbizprod [bizid] [products]");
				return 1;
			}
			new id = strval(tmp);
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /setbizprod [bizid] [products]");
				return 1;
			}
			new type = strval(tmp);
			BizInfo[id][bProducts] = type;
			SaveBusinesses();
			format(str, sizeof(str), "You have set business ID %d's products to %d.", id, type);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		}
		else return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this command.");
		return 1;
	}
	if(strcmp(cmd, "/aseizebiz", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] >= 99999 || PlayerInfo[playerid][pBizMod] == 1)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /aseizebiz [bizid]");
				return 1;
			}
			new id = strval(tmp);
			if(!BizInfo[id][bOwned]) return SendClientMessage(playerid, COLOR_GREY, "You cannot seize a business that is not owned by someone.");
			if(BizInfo[id][bSeized] == 0)
			{
				format(str, sizeof(str), "You have seized business ID %d.", id);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
				BizInfo[id][bSeized] = 1;
				format(str, sizeof(str), "{AFAFAF}%s{AFAFAF}\n{FFFFFF}This business has been seized by the\n{696969}Government", BizInfo[id][bName]);
				UpdateDynamic3DTextLabelText(Text3D:BizText[id], COLOR_BIZTEXT, str);
			}
			else
			{
				format(str, sizeof(str), "You have unseized business ID %d.", id);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
				BizInfo[id][bSeized] = 0;
				if(BizInfo[id][bLocked] == 1)
				{
					format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s\nEntry Fee: Locked", BizInfo[id][bName], BizInfo[id][bOwner]);
				}
				else
				{
					format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s", BizInfo[id][bName], BizInfo[id][bOwner]);
				}
				UpdateDynamic3DTextLabelText(Text3D:BizText[id], COLOR_BIZTEXT, str);
			}
			SaveBusinesses();
		}
		else return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this command.");
		return 1;
	}
	if(strcmp(cmd, "/setbiztill", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] >= 99999 || PlayerInfo[playerid][pBizMod] == 1)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /setbiztill [bizid] [amount]");
				return 1;
			}
			new id = strval(tmp);
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /setbiztill [bizid] [amount]");
				return 1;
			}
			new type = strval(tmp);
			BizInfo[id][bTill] = type;
			SaveBusinesses();
			format(str, sizeof(str), "You have set business ID %d's bank amount to $%d.", id, type);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		}
		else return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this command.");
		return 1;
	}
	/*if(strcmp(cmd, "/setbizentryfee", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] >= 99999 || PlayerInfo[playerid][pBizMod] == 1)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /setbizentryfee [bizid] [amount]");
				return 1;
			}
			new id = strval(tmp);
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /setbizentryfee [bizid] [amount]");
				return 1;
			}
			new type = strval(tmp);
			//BizInfo[id][bEntryFee] = type;
			SaveBusinesses();
			format(str, sizeof(str), "You have set business ID %d's entry fee to $%d.", id, type);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		}
		else return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this command.");
		return 1;
	}*/
	if(strcmp(cmd, "/setbiztype", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] >= 99999 || PlayerInfo[playerid][pBizMod] == 1)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /setbiztype [bizid] [type]");
				SendClientMessage(playerid, COLOR_GREY, "Available Types: 1: gunshop, 2: burgershot, 3: 247, 4: bar/club, 5: sexshop, 6: clothes store, 7: donut store");
				SendClientMessage(playerid, COLOR_GREY, "Available Types: 8: clucknbell, 9: pizzastack, 10: bank, 11: electronics store, 12: furniture store 13: fishing");
				return 1;
			}
			new id = strval(tmp);
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /setbiztype [bizid] [type]");
				SendClientMessage(playerid, COLOR_GREY, "Available Types: 1: gunshop, 2: burgershot, 3: 247, 4: bar/club, 5: sexshop, 6: clothes store, 7: donut store");
				SendClientMessage(playerid, COLOR_GREY, "Available Types: 8: clucknbell, 9: pizzastack, 10: bank, 11: electronics store, 12: furniture store 13: fishing");
				return 1;
			}
			new type = strval(tmp);
			if(type > 13 || type < 1)
			{
				SendClientMessage(playerid, COLOR_GREY, "Type can't be below 1 or above 13.");
				return 1;
			}
			BizInfo[id][bType] = type;
			SaveBusinesses();
			format(str, sizeof(str), "You have set business ID %d's type to %d.", id, type);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		}
		else return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this command.");
		return 1;
	}
	if(strcmp(cmd, "/setbizprice", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] >= 99999 || PlayerInfo[playerid][pBizMod] == 1)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /setbizprice [bizid] [amount]");
				return 1;
			}
			new id = strval(tmp);
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /setbizprice [bizid] [amount]");
				return 1;
			}
			new amount = strval(tmp);
			if(amount > 100000000 || amount < 1)
			{
				SendClientMessage(playerid, COLOR_GREY, "Type can't be below $1 or above $100,000,000.");
				return 1;
			}
			BizInfo[id][bValue] = amount;
			if(BizInfo[id][bOwned] == 0)
			{
				new pricestring[32];
				new price = BizInfo[id][bValue];
				new j = valstr(pricestring, price);
				while(j >= 4)
				{
					j -= 3;
					strins(pricestring, ",", j);
				}
				strins(pricestring, "$", 0);
				format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nStatus: For Sale\nPrice: %s", BizInfo[id][bName], pricestring);
			}
			else
			{
				// Government seized
				if(BizInfo[id][bSeized] == 1)
				{
					format(str, sizeof(str), "{AFAFAF}%s{AFAFAF}\n{FFFFFF}This business has been seized by the\n{696969}Government", BizInfo[id][bName]);
				}
				// Judicial Department seized 
				else if(BizInfo[id][bSeized] == 2) {
					format(str, sizeof(str), "{AFAFAF}%s{AFAFAF}\n{FFFFFF}This business has been seized by the\n{BA6103}Judicial Department", BizInfo[id][bName]);
				}
				else
				{
					if(BizInfo[id][bLocked] == 1)
					{
						format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s\nEntry Fee: Locked", BizInfo[id][bName], BizInfo[id][bOwner]);
					}
					else
					{
						format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s", BizInfo[id][bName], BizInfo[id][bOwner]);
					}
				}
			}
			UpdateDynamic3DTextLabelText(Text3D:BizText[id], COLOR_BIZTEXT, str);
			SaveBusinesses();
			format(str, sizeof(str), "You have set business ID %d's value to $%d.", id, amount);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		}
		else return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this command.");
		return 1;
	}
	if(strcmp(cmd, "/gotobiz", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] >= 99999 || PlayerInfo[playerid][pBizMod] == 1)
		{
			if(GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can not do that while spectating.");
				return 1;
			}
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /gotobiz [bizid]");
				return 1;
			}
			new id = strval(tmp);
			SetPlayerPos(playerid, BizInfo[id][bOutsideX], BizInfo[id][bOutsideY], BizInfo[id][bOutsideZ]);
			SetPlayerVirtualWorld(playerid, BizInfo[id][bOutsideWorld]);
			SetPlayerInterior(playerid, BizInfo[id][bOutsideInt]);
			displayCenterHUDInfo(playerid,  "~r~Teleporting.", 8); //display for 8 seconds
		}
		else return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this command.");
		return 1;
	}
	if(strcmp(cmd, "/asellbiz", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] >= 99999 || PlayerInfo[playerid][pBizMod] == 1)
		{
			new biz = IsPlayerNearBiz(playerid);
			if(biz == -1) return SendClientMessage(playerid, COLOR_GREY, "You must be near the business you wish to sell.");
			foreach( new i: Player )
			{
				if(PlayerInfo[i][pBizKey] == biz)
				{
					PlayerInfo[i][pBizKey] = -1;
				}
			}
			strmid(BizInfo[biz][bOwner], "None", 0, strlen("None"), 255);
			BizInfo[biz][bOwned] = 0;
			BizInfo[biz][bSeized] = 0;
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have successfully sold this business.");
			new pricestring[32];
			new price = BizInfo[biz][bValue];
			new j = valstr(pricestring, price);
			while(j >= 4)
			{
				j -= 3;
				strins(pricestring, ",", j);
			}
			strins(pricestring, "$", 0);
			format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nStatus: For Sale\nPrice: %s", BizInfo[biz][bName], pricestring);
			UpdateDynamic3DTextLabelText(Text3D:BizText[biz], COLOR_BIZTEXT, str);
		}
		else return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this command.");
		return 1;
	}
	if(strcmp(cmd, "/getbizid", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] >= 99999 || PlayerInfo[playerid][pBizMod] == 1)
		{
			new biz = IsPlayerNearBiz(playerid);
			if(biz == -1) return SendClientMessage(playerid, COLOR_GREY, "You must be near the business to get the biz ID.");
			format(str, sizeof(str), "Name: %s, Owner: %s, ID: %d", BizInfo[biz][bName], BizInfo[biz][bOwner], biz);
			SendClientMessage(playerid, COLOR_WHITE, str);
		}
		else return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this command.");
		return 1;
	}
	if(strcmp(cmd, "/purchase", true) == 0)
	{
		if(UseBMTimer[playerid]) return SendClientMessage(playerid, COLOR_GREY, "You must wait 30 seconds.");
		if(IsPlayerInRangeOfPoint(playerid, 30.0, 311.92, -137.58, 999.60) && PlayerInfo[playerid][pVirtualWorld] == 426)
		{
			if(PlayerInfo[playerid][pDonateRank] == 2)
			{
				ShowPlayerDialog(playerid, 99, DIALOG_STYLE_LIST, "Silver Blackmarket", "Deagle			$80,000\nAK-47			$180,000\nTEC9			$50,000\nMicro-UZI			$50,000", "Purchase", "Cancel");
			}
			if(PlayerInfo[playerid][pDonateRank] == 3)
			{
				ShowPlayerDialog(playerid, 100, DIALOG_STYLE_LIST, "Gold Blackmarket", "Deagle			$60,000\nKnife			$1,000,000\nAK-47			$100,000\nTEC9			$30,000\nMicro-UZI			$30,000", "Purchase", "Cancel");
			}
			if(PlayerInfo[playerid][pDonateRank] == 4)
			{
				ShowPlayerDialog(playerid, 3325, DIALOG_STYLE_LIST, "Diamond Blackmarket", "Deagle			$40,000\nKnife			$600,000\nAK-47			$60,000\nTEC9			$20,000\nMicro-UZI			$20,000\nSniper			$180,000", "Purchase", "Cancel");
			}
			return 1;
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not in the VIP Club/Black Market.");
		}
		return 1;
	}
	if(strcmp(cmd, "/biz", true) == 0)
	{
		if(PlayerInfo[playerid][pBizKey] == -1) return SendClientMessage(playerid, COLOR_GREY, "You do not own a business.");
		new business = PlayerInfo[playerid][pBizKey];
		if(BizInfo[business][bSeized] == 1) return SendClientMessage(playerid, COLOR_GREY, "That business is currently property of the government.");
		new x_nr[64];
		x_nr = strtok(cmdtext, idx);
		if(!strlen(x_nr))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /biz [name]");
			SendClientMessage(playerid, COLOR_GRAD1, "Available names: Lock, Till, Restock, Name");
			/*if(business == 1)
				SendClientMessage(playerid, COLOR_GRAD1, "Available names: Gate");*/
			return 1;
		}
		if(strcmp(x_nr, "name", true) == 0)
		{
			// Fix to allow spaces in business names
			while(idx < strlen(cmdtext) && cmdtext[idx] <= ' ') idx++; // pass all spaces before the biz name
			strmid(tmp, cmdtext, idx, strlen(cmdtext));
			if(!strlen(tmp))
			{
				return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /biz name [newname]");
			}
			if(strlen(tmp) < 5 || strlen(tmp) >= 64) return SendClientMessage(playerid, COLOR_GRAD1, "The name length must be greater than 5 and lesser than 64 characters.");
			if(!AlphaNumeric(tmp)) return SendClientMessage(playerid, COLOR_GRAD1, "The name can only contain alphabetical/numerical characters.");
			strmid(BizInfo[business][bName], tmp, 0, strlen(tmp), 255);
			format(str, sizeof(str), "You have set your business name to '%s'.", BizInfo[business][bName]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

			// Government seized
			if(BizInfo[business][bSeized] == 1)
			{
				format(str, sizeof(str), "{AFAFAF}%s{AFAFAF}\n{FFFFFF}This business has been seized by the\n{696969}Government", BizInfo[business][bName]);
			}
			// Judicial Department seized 
			else if(BizInfo[business][bSeized] == 2) {
				format(str, sizeof(str), "{AFAFAF}%s{AFAFAF}\n{FFFFFF}This business has been seized by the\n{BA6103}Judicial Department", BizInfo[business][bName]);
			}
			else
			{
				if(BizInfo[business][bLocked] == 1)
				{
					format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s\nEntry Fee: Locked", BizInfo[business][bName], BizInfo[business][bOwner]);
				}
				else
				{
					format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s", BizInfo[business][bName], BizInfo[business][bOwner]);
				}
			}
			UpdateDynamic3DTextLabelText(Text3D:BizText[business], COLOR_BIZTEXT, str);
			return 1;
		}
		/*if(strcmp(x_nr, "entryfee", true) == 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /biz entryfee [amount]");
				return 1;
			}
			new amount = strval(tmp);
			if(amount < 0 || amount > 5000)
			{
				SendClientMessage(playerid, COLOR_WHITE, "Entry fee cannot go below $0 or above $5,000.");
				return 1;
			}
			BizInfo[business][bEntryFee] = amount;
			format(str, sizeof(str), "Your business entry fee has been set to $%d.", amount);
			SendClientMessage(playerid, COLOR_GREY, str);

			// Government seized
			if(BizInfo[business][bSeized] == 1)
			{
				format(str, sizeof(str), "{AFAFAF}%s{AFAFAF}\n{FFFFFF}This business has been seized by the\n{696969}Government", BizInfo[business][bName]);
			}
			// Judicial Department seized 
			else if(BizInfo[business][bSeized] == 2) {
				format(str, sizeof(str), "{AFAFAF}%s{AFAFAF}\n{FFFFFF}This business has been seized by the\n{BA6103}Judicial Department", BizInfo[business][bName]);
			}
			else
			{
				if(BizInfo[business][bLocked] == 1)
				{
					format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s", BizInfo[business][bName], BizInfo[business][bOwner]);
				}
				else
				{
					format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s", BizInfo[business][bName], BizInfo[business][bOwner]);
				}
			}
			UpdateDynamic3DTextLabelText(Text3D:BizText[business], COLOR_BIZTEXT, str);
		}*/
		else if(strcmp(x_nr, "lock", true) == 0)
		{
			new inbusiness = 0;
			if(IsPlayerInRangeOfPoint(playerid, 5.0, BizInfo[business][bInsideX], BizInfo[business][bInsideY], BizInfo[business][bInsideZ]))
			{
				if(GetPlayerVirtualWorld(playerid) == playerid)
				{
					inbusiness = 1;
				}
			}
			if(!inbusiness && !IsPlayerInRangeOfPoint(playerid, 2.0, BizInfo[business][bOutsideX], BizInfo[business][bOutsideY], BizInfo[business][bOutsideZ]))
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not near/inside your business.");
				return 1;
			}
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			if(BizInfo[business][bLocked] == 1)
			{
				BizInfo[business][bLocked] = 0;
				format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s", BizInfo[business][bName], BizInfo[business][bOwner]);
				UpdateDynamic3DTextLabelText(Text3D:BizText[business], COLOR_BIZTEXT, str);
				PlayerPlaySound(playerid, 1145, 0.0, 0.0, 0.0);
				format(str, sizeof(str), "* %s has unlocked the door to their business.", sendername);
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				return 1;
			}
			else
			{
				BizInfo[business][bLocked] = 1;
				format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s", BizInfo[business][bName], BizInfo[business][bOwner]);
				UpdateDynamic3DTextLabelText(Text3D:BizText[business], COLOR_BIZTEXT, str);
				PlayerPlaySound(playerid, 1145, 0.0, 0.0, 0.0);
				format(str, sizeof(str), "* %s has locked the door to their business.", sendername);
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				return 1;
			}
		}
		else if(strcmp(x_nr, "till", true) == 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /biz till [amount]");
				format(str, sizeof(str), "You have $%d in your business till.", BizInfo[business][bTill]);
				SendClientMessage(playerid, COLOR_YELLOW, str);
				return 1;
			}
			new amount = strval(tmp);
			if(amount < 1 || amount > BizInfo[business][bTill]) return SendClientMessage(playerid, COLOR_GREY, "Invalid transaction amount.");
			new ebill = amount / 8;
			new profit = amount-ebill;
			BizInfo[business][bTill] -= amount;
			GiveMoney(playerid, profit);
			TaxValue += ebill;
			format(str, sizeof(str), "You have withdrawn $%d (Tax: -$%d. Profit: $%d) from your business till. Total business till: $%d", amount, ebill, profit, BizInfo[business][bTill]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		}
		else if(strcmp(x_nr, "restock", true) == 0)
		{
			SendClientMessage( playerid, COLOR_GREY, "Use /buyprod!" );
			/*if(IsPlayerInRangeOfPoint(playerid, 25.0, BizInfo[business][bInsideX], BizInfo[business][bInsideY], BizInfo[business][bInsideZ]))
			{
				if(GetPlayerVirtualWorld(playerid) == business)
				{
					tmp = strtok(cmdtext, idx);
					if(!strlen(tmp))
					{
						SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /biz restock [amount]");
						format(str, sizeof(str), "Your business has %d products.", BizInfo[business][bProducts]);
						SendClientMessage(playerid, COLOR_YELLOW, str);
						return 1;
					}
					new amount = strval(tmp);
					if(amount > PlayerInfo[playerid][pMats]) return SendClientMessage(playerid, COLOR_GREY, "You don't have that many materials.");
					if(amount < 1 || amount > 5000) return SendClientMessage(playerid, COLOR_GREY, "Cannot restock below 1 product or above 5000 products.");
					//if(amount + BizInfo[business][bProducts] > 500) return SendClientMessage(playerid, COLOR_GREY, "Your business can only hold 500 max products.");
					BizInfo[business][bProducts] += amount;
					PlayerInfo[playerid][pMats] -=amount;
					format(str, sizeof(str), "You have restocked %d products into your business. Total Products: %d", amount, BizInfo[business][bProducts]);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
				}
			}
			else
			{
				return SendClientMessage(playerid, COLOR_GREY, "You are not inside your business.");
			}*/
		}
		/*else if(strcmp(x_nr,"gate",true) == 0)
		{
			if(business!= 1) return 1;
			if(IsPlayerInRangeOfPoint(playerid,25.0,BizInfo[1][bInsideX],BizInfo[1][bInsideY],BizInfo[1][bInsideZ]))
			{
				if(GetPlayerVirtualWorld(playerid) == 1)
				{
					openSRGate();
				}
			}
			else
			{
				return SendClientMessage(playerid,COLOR_GREY, "You are not inside your business.");
			}
		}*/
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "Invalid action.");
			return 1;
		}
		return 1;
	}
	if(strcmp(cmd, "/famdeposit", true) == 0)
	{
		if(PlayerInfo[playerid][pFMember] == 255)
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not in a Family.");
			return 1;
		}
		if(!IsPlayerAtBankingPoint(playerid))		
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not at the bank counter.");
			return 1;
		}
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /famdeposit [amount]");
			format(str, sizeof(str), "You have $%d in your family bank account.", FamilyInfo[PlayerInfo[playerid][pFMember]][FamilyBank]);
			SendClientMessage(playerid, COLOR_GRAD3, str);
			return 1;
		}
		new cashdeposit = strvalEx(tmp);
		if(cashdeposit > PlayerInfo[playerid][pCash] || cashdeposit < 1)
		{
			SendClientMessage(playerid, COLOR_GRAD2, "You don't have that much.");
			return 1;
		}
		GiveMoney(playerid, -cashdeposit);
		FamilyInfo[PlayerInfo[playerid][pFMember]][FamilyBank] =FamilyInfo[PlayerInfo[playerid][pFMember]][FamilyBank] +cashdeposit;
		SendClientMessage(playerid, COLOR_WHITE, "Bank Statement:");
		format(str, sizeof(str), "Old Balance: $%d", FamilyInfo[PlayerInfo[playerid][pFMember]][FamilyBank]);
		SendClientMessage(playerid, COLOR_GRAD2, str);
		format(str, sizeof(str), "Deposit: $%d", cashdeposit);
		SendClientMessage(playerid, COLOR_GRAD4, str);
		format(str, sizeof(str), "New Balance: $%d", FamilyInfo[PlayerInfo[playerid][pFMember]][FamilyBank]);
		SendClientMessage(playerid, COLOR_WHITE, str);
		return 1;
	}
	if(strcmp(cmd, "/fwithdraw", true) == 0)
	{
		if(PlayerInfo[playerid][pFMember] == 255)
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not in a Family.");
			return 1;
		}
		
		if(!IsPlayerAtBankingPoint(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You are not at a bank counter.");

		if(PlayerInfo[playerid][pRank] < 4)
		{
			SendClientMessage(playerid, COLOR_GREY, "You must be rank 4 to withdraw money from your family bank account.");
			return 1;
		}
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fwithdraw [amount]");
			format(str, sizeof(str), "You have $%d in your family bank account.", FamilyInfo[PlayerInfo[playerid][pFMember]][FamilyBank]);
			SendClientMessage(playerid, COLOR_GRAD3, str);
			return 1;
		}
		new cashdeposit = strvalEx(tmp);
		if(cashdeposit > FamilyInfo[PlayerInfo[playerid][pFMember]][FamilyBank] || cashdeposit < 1)
		{
			SendClientMessage(playerid, COLOR_GRAD2, "You don't have that much.");
			return 1;
		}
		PlayerInfo[playerid][pCash] += cashdeposit;
		FamilyInfo[PlayerInfo[playerid][pFMember]][FamilyBank] =FamilyInfo[PlayerInfo[playerid][pFMember]][FamilyBank] -cashdeposit;
		format(str, sizeof(str), "You have withdrawn $%d from your family bank account, Total: $%d ", cashdeposit, FamilyInfo[PlayerInfo[playerid][pFMember]][FamilyBank]);
		SendClientMessage(playerid, COLOR_YELLOW, str);
		return 1;
	}
	if(strcmp(cmd, "/engine", true) == 0)
	{
		if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return 1;
		if(!IsPlayerInAnyVehicle(playerid))
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not in a vehicle.");
			return 1;
		}
		new vehid = GetPlayerVehicleID(playerid);
		if(vehid != INVALID_VEHICLE_ID)
		{
			if(turningengine[playerid]) return 1; // avoiding spam
			new Float:vhealth;
			GetVehicleHealth( vehid, vhealth );
			if( vhealth < 306.0 ) {
				return SendClientMessage( playerid, COLOR_GREY, "The vehicle will not start." );
			}
			
			if(IsABicycle(vehid)) return SendClientMessage(playerid, COLOR_GREY, "That vehicle doesn't have an engine.");
			if(Gas[vehid] < 1)
			{
				return SendClientMessage(playerid, COLOR_GREY, "That vehicle is out of fuel.");
			}
			if(Refueling[playerid] == 1) return SendClientMessage(playerid, COLOR_GREY, "You can't start your engine whilst re-fuelling.");
			if(engineOn[vehid] == 0)
			{
				if(GetVehNeedHW(vehid, playerid))
				{
					return SendClientMessage(playerid, COLOR_GRAD2, "You do not have the key for this vehicle! (/(h)ot(w)ire)");
				}
				turningenginetimer[playerid] = SetTimerEx("Timer_EngineStart", 2500, false, "ii", playerid, vehid);
				format(str, sizeof(str), "* %s turns the key in the ignition...", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				turningengine[playerid] = 1;
			}
			else
			{
				new engine, lights, alarm, doors, bonnet, boot, objective;
				GetVehicleParamsEx(vehid, engine, lights, alarm, doors, bonnet, boot, objective);
				SetVehicleParamsEx(vehid, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);
				engineOn[vehid] = 0;
				playerDisabledEngine[vehid] = 1;
				format(str, sizeof(str), "* %s has turned their engine off.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
		}
		return 1;
	}
	if(strcmp(cmd, "/lights", true) == 0)
	{
		if(!IsPlayerInAnyVehicle(playerid))
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not in a vehicle.");
			return 1;
		}
		if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
		{
			return 1;
		}
		new vehid = GetPlayerVehicleID(playerid);
		if(vehid != INVALID_VEHICLE_ID)
		{
			if(IsABicycle(vehid))
			{
				SendClientMessage(playerid, COLOR_GREY, "This vehicle doesn't have lights.");
				return 1;
			}
			new engine, lights, alarm, doors, bonnet, boot, objective;
			GetVehicleParamsEx(vehid, engine, lights, alarm, doors, bonnet, boot, objective);
			if(lightsOff[vehid] == 0)
			{
				SetVehicleParamsEx(vehid, engine, VEHICLE_PARAMS_ON, alarm, doors, bonnet, boot, objective);
				lightsOff[vehid] = 1;
				format(str, sizeof(str), "* %s has turned their lights on.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			else
			{
				SetVehicleParamsEx(vehid, engine, VEHICLE_PARAMS_OFF, alarm, doors, bonnet, boot, objective);
				lightsOff[vehid] = 0;
				format(str, sizeof(str), "* %s has turned their lights off.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
		}
		return 1;
	}
	if(strcmp(cmd, "/hood", true) == 0)
	{
		new vehid = gLastCar[playerid];
		if(vehid == INVALID_VEHICLE_ID)
		{
			return 1;
		}
		new Float:cx, Float:cy, Float:cz;
		GetVehiclePos(vehid, cx, cy, cz);
		if(IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER || IsPlayerInRangeOfPoint(playerid, 4, cx, cy, cz) && !IsPlayerInAnyVehicle(playerid))
		{
			if(IsABicycle(vehid))
			{
				SendClientMessage(playerid, COLOR_GREY, "This vehicle doesn't have a hood.");
				return 1;
			}
			new engine, lights, alarm, doors, bonnet, boot, objective;
			GetVehicleParamsEx(vehid, engine, lights, alarm, doors, bonnet, boot, objective);
			if(hoodOpen[vehid] == 0)
			{
				SetVehicleParamsEx(vehid, engine, lights, alarm, doors, VEHICLE_PARAMS_ON, boot, objective);
				hoodOpen[vehid] = 1;
				format(str, sizeof(str), "* %s has opened their hood.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			else
			{
				SetVehicleParamsEx(vehid, engine, lights, alarm, doors, VEHICLE_PARAMS_OFF, boot, objective);
				hoodOpen[vehid] = 0;
				format(str, sizeof(str), "* %s has closed their hood.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not in a vehicle/in range of your last vehicle.");
		}
		return 1;
	}
	if(strcmp(cmd, "/vips", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] >= 1337 || PlayerInfo[playerid][pShopMod] != 0)
		{
			new vips;
			new vipType[7];
			new vipColor[10];
			foreach( new i: Player )
			{
				if(IsPlayerConnected(i))
				{
					if(PlayerInfo[i][pDonateRank] > 0)
					{
						if(PlayerInfo[i][pDonateRank] == 1)
						{
							vipType = "Bronze";
							vipColor = "{BB7900}";
						}
						else if(PlayerInfo[i][pDonateRank] == 2)
						{
							vipType = "Silver";
							vipColor = "{ACAEB0}";
						}
						else if(PlayerInfo[i][pDonateRank] == 3)
						{
							vipType = "Gold";
							vipColor = "{FFC600}";
						}
						format(str, sizeof(str), "Name: %s | Type: %s%s{FFFFFF} | Purchased: %s | Expires: %s", PlayerICName(i), vipColor, vipType, PlayerInfo[i][pVIPJoinDate], PlayerInfo[i][pVIPExpDate]);
						SendClientMessage(playerid, COLOR_WHITE, str);
						vips++;
					}
				}
			}
			if(vips == 0)
			{
				SendClientMessage(playerid, COLOR_GREY, "There are currently no VIPs online.");
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GRAD2, "You are not authorized to use this command.");
		}
		return 1;
	}
	if(strcmp(cmdtext, "/vip", true) ==0)
	{//
		if(IsPlayerRestricted(playerid)) return SendClientMessage(playerid, COLOR_GRAD1, "You can not do this at this time.");
		if((pDeathState[playerid] == PLAYER_STATE_REVIVE || PlayerCuffed[playerid] > 0 || PlayerTazed[playerid] > 0))
			return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
		if(PlayerInfo[playerid][pJailed] != 0) return SendClientMessage(playerid, COLOR_RED, "You can not use this while being in prison.");
		if(PlayerInfo[playerid][pDonateRank] >= 1)
		{
			if(IsPlayerInAnyVehicle(playerid))
			{
				new tempid = GetPlayerVehicleID(playerid);
				if(IsAPlane(tempid) || IsAHelicopter(tempid) || IsABike(tempid) || IsATrain(tempid) || IsABoat(tempid))
				{
					ShowPlayerDialog(playerid, 3242, DIALOG_STYLE_LIST, "VIP", "Color Name\nRepair Car\nSkin", "Select", "Cancel");
				}
				else
				{
					ShowPlayerDialog(playerid, 62, DIALOG_STYLE_LIST, "VIP", "Color Name\nCar Tune\nSkin", "Select", "Cancel");
				}
			}
			else
			{
				ShowPlayerDialog(playerid, 62, DIALOG_STYLE_LIST, "VIP", "Color Name\nCar Tune\nSkin", "Select", "Cancel");
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not a VIP.");
		}
		return 1;
	}
	if(strcmp(cmd, "/famed", true) == 0) // This allows famed members to color their name the in-game famed color (scolor_cyan)
	{
		if(PlayerInfo[playerid][pFamed] >= 1)
		{
			if(usingFC[playerid] == 0)
			{
				usingFC[playerid] = 1;
				SendClientMessage(playerid, COLOR_FAMED, "Your name is now colored.");
				SetPlayerToTeamColor(playerid);
			}
			else
			{
				usingFC[playerid] = 0;
				SendClientMessage(playerid, COLOR_FAMED, "Your name is no longer colored.");
				SetPlayerToTeamColor(playerid);
			}
		}
		return 1;
	}
	if(strcmp(cmdtext, "/viptime", true) ==0)
	{
		if(PlayerInfo[playerid][pDonateRank] < 1)
		{
			return SendClientMessage(playerid, COLOR_GREY, "You are not a VIP.");
		}
		format(str, sizeof(str), "Join Date: %s, Expire Date: %s", PlayerInfo[playerid][pVIPJoinDate], PlayerInfo[playerid][pVIPExpDate]);
		SendClientMessage(playerid, COLOR_GRAD1, str);
		return 1;
	}
	/*if(strcmp(cmd, "/makevip", true) == 0)
	{
		strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
		if(PlayerInfo[playerid][pAdmin] >= 99999 || PlayerInfo[playerid][pShopMod] != 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /makevip [playerid/PartOfName] [rank]");
				return 1;
			}
			giveplayerid = ReturnUser(tmp);
			if(giveplayerid == INVALID_PLAYER_ID)
			{
				SendClientMessage(playerid, COLOR_GRAD2, "Invalid player.");
				return 1;
			}
			new x_string[64];
			x_string = strtok(cmdtext, idx);
			if(!strlen(x_string))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /makevip [playerid/PartOfName] [rank]");
				return 1;
			}
			new level;
			level = strvalEx(x_string);
			new Year, Month, Day;
			getdate(Year, Month, Day);
			if(level < 0 && level > 3)
			{
				return SendClientMessage(giveplayerid, COLOR_GREY, "Rank cannot be below 0 or above 3.");
			}
			if(level == 0)
			{
				PlayerInfo[giveplayerid][pDonateRank] = 0;
				if(PlayerInfo[giveplayerid][pLevel] == 1) PlayerInfo[giveplayerid][pTag] = NTAG_NEWBIE;
				else if(PlayerInfo[giveplayerid][pLevel] > 1) PlayerInfo[giveplayerid][pTag] = NTAG_PLAYER;
				format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s's VIP status has been revoked by %s.", PlayerICName(giveplayerid), PlayerICName(playerid));
				ABroadCast(COLOR_WHITE, str, 1);
				format(str, sizeof(str), "Admin %s has revoked you of your VIP status.", PlayerICName(playerid));
				SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
				ClearVIP(giveplayerid);
				return 1;
			}
			else if(level == 1)
			{
				PlayerInfo[giveplayerid][pDonateRank] = 1;
				PlayerInfo[giveplayerid][pTag] = NTAG_BVIP;
				format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has been made a bronze VIP by %s.", PlayerICName(giveplayerid), PlayerICName(playerid));
				ABroadCast(COLOR_WHITE, str, 1);
				format(str, sizeof(str), "Admin %s has made you a bronze VIP.", PlayerICName(playerid));
				SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
			}
			else if(level == 2)
			{
				PlayerInfo[giveplayerid][pDonateRank] = 2;
				PlayerInfo[giveplayerid][pTag] = NTAG_SVIP;
				format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has been made a silver VIP by %s.", PlayerICName(giveplayerid), PlayerICName(playerid));
				ABroadCast(COLOR_WHITE, str, 1);
				format(str, sizeof(str), "Admin %s has made you a silver VIP.", PlayerICName(playerid));
				SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
			}
			else if(level == 3)
			{
				PlayerInfo[giveplayerid][pDonateRank] = 3;
				PlayerInfo[giveplayerid][pTag] = NTAG_GVIP;
				format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has been made a gold VIP by %s.", PlayerICName(giveplayerid), PlayerICName(playerid));
				ABroadCast(COLOR_WHITE, str, 1);
				format(str, sizeof(str), "Admin %s has made you a gold VIP.", PlayerICName(playerid));
				SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
			}
			format(str, 32, "%02d/%02d/%d", Month, Day, Year);
			strmid(PlayerInfo[giveplayerid][pVIPJoinDate], str, 0, strlen(str), 255);

			new 
				rDay,
				rMonth,
				rYear;

			GetDateAfterInterval(Day, Month, Year, 30, rDay, rMonth, rYear);

			format(str, 32, "%02d/%02d/%d", rMonth, rDay, rYear);
			strmid(PlayerInfo[giveplayerid][pVIPExpDate], str, 0, strlen(str), 255);

			format(str, sizeof(str), "Join Date: %s, Expire Date: %s", PlayerInfo[giveplayerid][pVIPJoinDate], PlayerInfo[giveplayerid][pVIPExpDate]);
			SendClientMessage(giveplayerid, COLOR_GRAD2, str);
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this command.");
		}
		return 1;
	}*/
	if(strcmp(cmd, "/makevip", true) == 0)
	{
		strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
		if(PlayerInfo[playerid][pAdmin] >= 99999 || PlayerInfo[playerid][pShopMod] != 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /makevip [playerid/PartOfName] [rank]");
				return 1;
			}
			giveplayerid = ReturnUser(tmp);
			if(giveplayerid == INVALID_PLAYER_ID)
			{
				SendClientMessage(playerid, COLOR_GRAD2, "Invalid player.");
				return 1;
			}
			new x_string[64];
			x_string = strtok(cmdtext, idx);
			if(!strlen(x_string))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /makevip [playerid/PartOfName] [rank]");
				return 1;
			}
			new level;
			level = strvalEx(x_string);
			new Year, Month, Day;
			getdate(Year, Month, Day);
			if(level < 0 && level > 3)
			{
				return SendClientMessage(giveplayerid, COLOR_GREY, "Rank cannot be below 0 or above 3.");
			}
			if(Month == 1 && Day >= 29)
			{
				return SendClientMessage(giveplayerid, COLOR_GREY, "You cannot make vips today.");
			}
			if(level == 0)
			{
				PlayerInfo[giveplayerid][pDonateRank] = 0;
				if(PlayerInfo[giveplayerid][pLevel] == 1) PlayerInfo[giveplayerid][pTag] = NTAG_NEWBIE;
				else if(PlayerInfo[giveplayerid][pLevel] > 1) PlayerInfo[giveplayerid][pTag] = NTAG_PLAYER;
				format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s's VIP status has been revoked by %s.", PlayerICName(giveplayerid), PlayerICName(playerid));
				ABroadCast(COLOR_WHITE, str, 1);
				format(str, sizeof(str), "Admin %s has revoked you of your VIP status.", PlayerICName(playerid));
				SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
				ClearVIP(giveplayerid);
				return 1;
			}
			else if(level == 1)
			{
				PlayerInfo[giveplayerid][pDonateRank] = 1;
				PlayerInfo[giveplayerid][pTag] = NTAG_BVIP;
				format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has been made a bronze VIP by %s.", PlayerICName(giveplayerid), PlayerICName(playerid));
				ABroadCast(COLOR_WHITE, str, 1);
				format(str, sizeof(str), "Admin %s has made you a bronze VIP.", PlayerICName(playerid));
				SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
			}
			else if(level == 2)
			{
				PlayerInfo[giveplayerid][pDonateRank] = 2;
				PlayerInfo[giveplayerid][pTag] = NTAG_SVIP;
				format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has been made a silver VIP by %s.", PlayerICName(giveplayerid), PlayerICName(playerid));
				ABroadCast(COLOR_WHITE, str, 1);
				format(str, sizeof(str), "Admin %s has made you a silver VIP.", PlayerICName(playerid));
				SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
			}
			else if(level == 3)
			{
				PlayerInfo[giveplayerid][pDonateRank] = 3;
				PlayerInfo[giveplayerid][pTag] = NTAG_GVIP;
				format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has been made a gold VIP by %s.", PlayerICName(giveplayerid), PlayerICName(playerid));
				ABroadCast(COLOR_WHITE, str, 1);
				format(str, sizeof(str), "Admin %s has made you a gold VIP.", PlayerICName(playerid));
				SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
			}
			else if(level == 4)
			{
				PlayerInfo[giveplayerid][pDonateRank] = 4;
				PlayerInfo[giveplayerid][pTag] = NTAG_DVIP;
				format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has been made a diamond VIP by %s.", PlayerICName(giveplayerid), PlayerICName(playerid));
				ABroadCast(COLOR_WHITE, str, 1);
				format(str, sizeof(str), "Admin %s has made you a diamond VIP.", PlayerICName(playerid));
				SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
			}
			format(str, 32, "%02d/%02d/%d", Month, Day, Year);
			strmid(PlayerInfo[giveplayerid][pVIPJoinDate], str, 0, strlen(str), 255);
			format(str, 32, "%02d/%02d/%d", Month +1, Day, Year);
			strmid(PlayerInfo[giveplayerid][pVIPExpDate], str, 0, strlen(str), 255);
			if(Month == 12)
			{
				format(PlayerInfo[giveplayerid][pVIPExpDate], 32, "%02d/%02d/%d", 1, Day, Year +1);
			}
			format(str, sizeof(str), "Join Date: %s, Expire Date: %s", PlayerInfo[giveplayerid][pVIPJoinDate], PlayerInfo[giveplayerid][pVIPExpDate]);
			SendClientMessage(giveplayerid, COLOR_GRAD2, str);
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this command.");
		}
		return 1;
	}
	if(strcmp(cmd, "/agiverank", true) == 0)
	{
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /agiverank [playerid/PartOfName] [Number(0-8 (in some cases))]");
			return 1;
		}
		new para1;
		new level;
		para1 = ReturnUser(tmp);
		tmp = strtok(cmdtext, idx);
		level = strvalEx(tmp);
		if(level > 12 || level < 0)
		{
			SendClientMessage(playerid, COLOR_GREY, "Dont go below number 0, or above number 12.");
			return 1;
		}
		if(PlayerInfo[playerid][pAdmin] >= 3 || PlayerInfo[playerid][pFactionMod] == 1)
		{
			if(IsPlayerConnected(para1))
			{
				if(para1 != INVALID_PLAYER_ID)
				{
					if(PlayerInfo[para1][pMember] != 0)
					{
						strmid(giveplayer, PlayerICName(para1), 0, MAX_PLAYER_NAME);
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						PlayerInfo[para1][pRank] = level;
						format(str, sizeof(str), "* You have been promoted to a higher Rank by admin %s.", sendername);
						SendClientMessage(para1, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* You have given %s Rank %d.", giveplayer, level);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						customRankName[para1] = false;
						format(customRank[para1], 24, "None");
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "That player is not a member of a Faction.");
						return 1;
					}
				}
			}
		}
		else
		{
			AdmErrorMsg;
		}
		return 1;
	}
	if(strcmp(cmd, "/sendto", true) == 0 || strcmp(cmd, "/st", true) == 0)
	{
		if(!(PlayerInfo[playerid][pAdmin] >= 2))
			return SendClientMessage(playerid, COLOR_GRAD2, "You are not authorized to use this command.");
		
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_WHITE, "{00BFFF}Usage:{FFFFFF} /(s)end(t)o [area/playerid] [playerid]");
			SendClientMessage(playerid, COLOR_GREY, "AREAS: lsair sfair lvair SAPD sfhosp");
			SendClientMessage(playerid, COLOR_GREY, "AREAS: skyscraper chiliad area51 airstrip bayside quarry dam bank pier");
			SendClientMessage(playerid, COLOR_GREY, "AREAS: underwater jet battlefield 8track kickstart dirtbike bloodbowl gym");
			SendClientMessage(playerid, COLOR_GREY, "AREAS: SAN lsmod bigammu allsaints taxi docks loco countygen dillhosp tiki arch");
			SendClientMessage(playerid, COLOR_GREY, "AREAS: fmd trainsf sfbridge sfbank sfapart sffmd paintball pizza");
			return 1;
		}
		new
		p =1,
		Float:a = GetPlayerFacingAngle(playerid, a),
			  destination[64] = " ",
								lookupid = ReturnUser(tmp),
								tmp34[128];
		tmp34 = strtok(cmdtext, idx);
		if(!strlen(tmp34))
		{
			SendClientMessage(playerid, COLOR_WHITE, "{00BFFF}Usage:{FFFFFF} /sendto [area/playerid] [playerid]");
			SendClientMessage(playerid, COLOR_GREY, "AREAS: lsair sfair lvair SAPD");
			SendClientMessage(playerid, COLOR_GREY, "AREAS: skyscraper chiliad area51 airstrip bayside quarry dam bank pier");
			SendClientMessage(playerid, COLOR_GREY, "AREAS: underwater jet battlefield 8track kickstart dirtbike bloodbowl gym");
			SendClientMessage(playerid, COLOR_GREY, "AREAS: mine dillimore blueberry prison montgomery carrier grove glenpark skytower pizza");
			return 1;
		}
		giveplayerid = ReturnUser(tmp34);
		if(!IsPlayerConnected(giveplayerid)) return SendClientMessage(playerid, COLOR_GRAD1, "That player is not active.");
		if(IsPlayerNPC(giveplayerid)) return 1;
		if(PlayerInfo[giveplayerid][pAdmin] > PlayerInfo[playerid][pAdmin]) return SendClientMessage(playerid, COLOR_GREY, "You can't teleport higher admins.");
		if(GetPlayerState(playerid) == PLAYER_STATE_PASSENGER) p =0;
		if(lookupid != INVALID_PLAYER_ID)
		{
			if(lookupid != giveplayerid)
			{
				if(!gPlayerLogged[lookupid])
					return SendClientMessage(playerid, COLOR_GRAD2, "That player hasn't spawned yet.");
				if(GetPlayerState(lookupid) == PLAYER_STATE_SPECTATING)
					return SendClientMessage(playerid, COLOR_GRAD2, "That player is spectating someone.");
			}
			new
			Float:x,
				  Float:y,
				  Float:z,
				  i = GetPlayerInterior(lookupid),
				  vvv = GetPlayerVirtualWorld(lookupid);
			if(IsPlayerInAnyVehicle(lookupid))
			{
				GetVehicleZAngle(GetPlayerVehicleID(lookupid), a);
			}
			else
			{
				GetPlayerFacingAngle(lookupid, a);
			}
			GetPlayerPos(lookupid, x, y, z);
			Teleport(giveplayerid, x, y +2, z, a, i, vvv, p);
			destination = PlayerICName(lookupid);
		}
		else if(!strcmp(tmp, "SAPD", true) || !strcmp(tmp, "ls", true))
		{
			destination = "San Andreas Police Department";
			Teleport(giveplayerid, 1529.6, -1691.2, 13.3, a, 0, 0, p);
		}
		else if(!strcmp(tmp, "int1", true))
		{
			strmid(sendername, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
			if(strcmp(sendername, "Marcus", true) == 0)
			{
				destination = "Marcus' Room";
				Teleport(giveplayerid, 1492.91, -1635.93, 949.16, 270.0, 1, 0, p);
				return 1;
			}
			else
			{
				destination = "Interior 1";
				Teleport(giveplayerid, 1411.1451, -0.5155, 1000.9236, a, 1, 0, p);
			}
		}
		else if(!strcmp(tmp, "marcsroom", true))
		{
			destination = "Marcus' Room";
			Teleport(giveplayerid, 1492.91, -1635.93, 949.16, 270.0, 1, 0, p);
		}
		else if(!strcmp(tmp, "interior1", true))
		{
			destination = "Interior 1";
			Teleport(giveplayerid, 1411.1451, -0.5155, 1000.9236, a, 1, 0, p);
		}
		else if(!strcmp(tmp, "gym", true))
		{
			destination = "Los Santos GYM";
			Teleport(giveplayerid, 2225.4001, -1724.4464, 13.5632, a, 0, 0, p);
		}
		else if(!strcmp(tmp, "lsair", true))
		{
			destination = "Los Santos Airport";
			Teleport(giveplayerid, 1934.9127, -2290.6362, 13.5469, a, 0, 0, p);
		}
		else if(!strcmp(tmp, "sfair", true) || !strcmp(tmp, "sf", true))
		{
			destination = "San Fierro Airport";
			Teleport(giveplayerid, -1417.0, -295.8, 14.1, a, 0, 0, p);
		}
		else if(!strcmp(tmp, "lvair", true) || !strcmp(tmp, "lv", true))
		{
			destination = "Las Venturas Airport";
			Teleport(giveplayerid, 1699.2, 1435.1, 10.7, a, 0, 0, p);
		}
		else if(!strcmp(tmp, "jet", true))
		{
			destination = "private jet";
			Teleport(giveplayerid, 1.71875, 30.4062, 1200.34, a, 1, 0, 0);
		}
		else if(!strcmp(tmp, "chiliad", true))
		{
			destination = "Mt. Chiliad";
			Teleport(giveplayerid, -2317.5325, -1644.9664, 483.7031, a, 0, 0, p);
		}
		else if(!strcmp(tmp, "area51", true))
		{
			destination = "Area 51";
			Teleport(giveplayerid, 202.1886, 1881.4122, 17.2199, 37.8779, 0, 0, p);
		}
		else if(!strcmp(tmp, "airstrip", true))
		{
			destination = "Desert Airstrip";
			Teleport(giveplayerid, 357.5273, 2513.8701, 16.5856, a, 0, 0, p);
		}
		else if(!strcmp(tmp, "skyscraper", true) || !strcmp(tmp, "sky", true))
		{
			destination = "the Skyscraper";
			Teleport(giveplayerid, 1543.9886, -1353.7587, 329.4735, a, 0, 0, p);
		}
		else if(!strcmp(tmp, "bayside", true))
		{
			destination = "Bayside Heliport";
			Teleport(giveplayerid, -2252.2944, 2335.5396, 4.8125, a, 0, 0, p);
		}
		else if(!strcmp(tmp, "underwater", true) || !strcmp(tmp, "under", true))
		{
			destination = "under the sea";
			Teleport(giveplayerid, -1005.3044, 657.4559, -39.0847, a, 0, 0, p);
		}
		else if(!strcmp(tmp, "bank", true))
		{
			destination = "The Bank";
			Teleport(giveplayerid, 1462.395751, -1016.391174, 25.84375, a, 0, 0, p);
		}
		else if(!strcmp(tmp, "pier", true))
		{
			destination = "the fishing pier";
			Teleport(giveplayerid, 364.9920, -2059.7288, 15.3990, a, 0, 0, p);
		}
		else if(!strcmp(tmp, "battlefield", true) || !strcmp(tmp, "battle", true))
		{
			destination = "the battlefield";
			Teleport(giveplayerid, -972.4957, 1060.9830, 1345.6690, a, 10, 0, p);
		}
		else if(!strcmp(tmp, "dam", true))
		{
			destination = "the Sherman Dam";
			Teleport(giveplayerid, -715.0000, 2062.0000, 60.0000, a, 0, 0, p);
		}
		else if(!strcmp(tmp, "bloodbowl", true))
		{
			destination = "Bloodbowl Arena";
			Teleport(giveplayerid, -1394.5928, 996.4797, 1033.8864, a, 15, 0, p);
		}
		else if(!strcmp(tmp, "8track", true))
		{
			destination = "8-Track Stadium";
			Teleport(giveplayerid, -1406.3815, -262.7644, 1043.4290, 346.8336, 7, 0, p);
		}
		else if(!strcmp(tmp, "dirtbike", true))
		{
			destination = "Dirtbike Stadium";
			Teleport(giveplayerid, -1436.2065, -642.5217, 1049.5261, 167.3703, 4, 0, p);
		}
		else if(!strcmp(tmp, "kickstart", true))
		{
			destination = "Kickstart Stadium";
			Teleport(giveplayerid, -1447.2618, 1604.3374, 1052.5220, 263.7223, 14, 0, p);
		}
		else if(!strcmp(tmp, "quarry", true))
		{
			destination = "Hunter Quarry";
			Teleport(giveplayerid, 609.8776, 867.3369, -42.2692, a, 0, 0, p);
		}
		else if(!strcmp(tmp, "mine", true))
		{
			destination = "Mine";
			Teleport(giveplayerid, 1027.9011, -305.6943, 74.5135, a, 0, 0, p);
		}
		else if(!strcmp(tmp, "dillimore", true))
		{
			destination = "Dillimore";
			Teleport(giveplayerid, 641.7200, -564.9901, 16.1875, a, 0, 0, p);
		}
		else if(!strcmp(tmp, "blueberry", true))
		{
			destination = "Blueberry";
			Teleport(giveplayerid, 214.9969, -129.4305, 1.5779, a, 0, 0, p);
		}
		else if(!strcmp(tmp, "montgomery", true))
		{
			destination = "Montgomery";
			Teleport(giveplayerid, 1329.1864, 317.3034, 19.5469, 66.3917, 0, 0, p);
		}
		else if(!strcmp(tmp, "carrier", true))
		{
			destination = "Aircraft Carrier";
			Teleport(giveplayerid, 3352.1787, -2835.1282, 18.2344, 358.2253, 0, 0, p);
		}
		else if(!strcmp(tmp, "grove", true))
		{
			destination = "Grove Street";
			Teleport(giveplayerid, 2510.5432, -1687.5811, 13.5661, 49.4352, 0, 0, p);
		}
		else if(!strcmp(tmp, "glenpark", true))
		{
			destination = "Glen Park";
			Teleport(giveplayerid, 1977.5286, -1185.2473, 25.9960, 3.6421, 0, 0, p);
		}
		else if(!strcmp(tmp, "skytower", true))
		{
			destination = "Sky Tower";
			Teleport(giveplayerid, 1543.60132, -1355.82654, 4568.44873, 0.0, 0, 0, p);
		}
		else if(!strcmp(tmp, "sfhosp", true))
		{
			destination = "San Fierro Hospital";
			Teleport(giveplayerid, -2653.3828, 638.5444, 14.4531, 0.0, 0, 0, p);
		}
		else if(!strcmp(tmp, "paintball", true))
		{
			destination = "Paintball";
			Teleport(giveplayerid, 1310.0594, -1376.5634, 13.6561, 0.0, 0, 0, p);
		}
		else if(!strcmp(tmp, "SAN", true))
		{
			destination = "San Andreas Network";
			Teleport(giveplayerid, 785.2725, -1325.4803, 13.5469, 350.9720, 0, 0, p);
		}
		else if(!strcmp(tmp, "lsmod", true))
		{
			destination = "LS Transfender";
			Teleport(giveplayerid, 1035.7266, -1034.4908, 31.6142, 183.1505, 0, 0, p);
		}
		else if(!strcmp(tmp, "bigammu", true))
		{
			destination = "Ammunation";
			Teleport(giveplayerid, 1363.4006, -1279.7795, 13.5469, 87.1025, 0, 0, p);
		}
		else if(!strcmp(tmp, "allsaints", true))
		{
			destination = "All Saints";
			Teleport(giveplayerid, 1185.8871, -1323.7729, 13.5591, 269.2528, 0, 0, p);
		}
		else if(!strcmp(tmp, "taxi", true))
		{
			destination = "Taxi Company";
			Teleport(giveplayerid, 1773.9911, -1860.9225, 13.5781, 359.1532, 0, 0, p);
		}
		else if(!strcmp(tmp, "docks", true))
		{
			destination = "Ocean Docks";
			Teleport(giveplayerid, 2318.1357, -2318.7971, 13.5469, 141.9202, 0, 0, p);
		}
		else if(!strcmp(tmp, "loco", true))
		{
			destination = "Loco Low";
			Teleport(giveplayerid, 2645.3574, -2022.3029, 13.5469, 2.7214, 0, 0, p);
		}
		else if(!strcmp(tmp, "countygen", true))
		{
			destination = "County General";
			Teleport(giveplayerid, 2024.6085, -1423.7742, 16.9665, 134.2379, 0, 0, p);
		}
		else if(!strcmp(tmp, "dillhosp", true))
		{
			destination = "Dillimore Hospital";
			Teleport(giveplayerid, 844.7101, -576.0881, 16.5325, 359.3915, 0, 0, p);
		}
		else if(!strcmp(tmp, "tiki", true))
		{
			destination = "Tikistore";
			Teleport(giveplayerid, 2347.5042, -1465.6353, 23.8281, 87.9392, 0, 0, p);
		}
		else if(!strcmp(tmp, "arch", true))
		{
			destination = "Arch Angels";
			Teleport(giveplayerid, -2711.4683, 223.9344, 4.3281, 271.3685, 0, 0, p);
		}
		else if(!strcmp(tmp, "fmd", true))
		{
			destination = "Los Santos Fire Medical Dept.";
			Teleport(giveplayerid, 1108.6631, -1337.4138, 13.8500, 178.4724, 0, 0, p);
		}
		else if(!strcmp(tmp, "trainsf", true))
		{
			destination = "SF Train Station";
			Teleport(giveplayerid, -1986.8132, 137.4702, 27.5391, 91.4267, 0, 0, p);
		}
		else if(!strcmp(tmp, "sfbridge", true))
		{
			destination = "SF Bridge";
			Teleport(giveplayerid, -1631.1191, 582.4623, 40.5780, 315.5850, 0, 0, p);
		}
		else if(!strcmp(tmp, "sfbank", true))
		{
			destination = "San Fierro Bank";
			Teleport(giveplayerid, -1706.2723, 785.4876, 24.8906, 90.6131, 0, 0, p);
		}
		else if(!strcmp(tmp, "sfapart", true))
		{
			destination = "San Fierro Apartments";
			Teleport(giveplayerid, -1754.3157, 944.8129, 24.8828, 179.5380, 0, 0, p);
		}
		else if(!strcmp(tmp, "sffmd", true))
		{
			destination = "San Fierro Fire Medical Dept.";
			Teleport(giveplayerid, -2667.0950, 582.0782, 14.4531, 180.5395, 0, 0, p);
		}
		else if(!strcmp(tmp, "pizza", true))
		{
			destination = "Pizza Stacks";
			Teleport(giveplayerid, 2098.9524, -1806.1031, 13.5550, 88.4629, 0, 0, p);
		}
		else
		{
			return SendClientMessage(playerid, COLOR_FADE1, "Invalid destination.");
		}
		// format str and send to player
		format(str, sizeof(str), "Sent to ~b~%s~w~.", destination);
		displayCenterHUDInfo(giveplayerid,  str, 8); //display for 8 seconds
		format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has sent %s to \"%s\"", PlayerICName(playerid), PlayerICName(giveplayerid), destination);
		ABroadCast(COLOR_WHITE, str, 1);
		return 1;
	}
	if(strcmp(cmd, "/joinevent", true) == 0)
	{
		if(EventActive == 1)
		{
			if(EventLocked == 1)
			{
				SendClientMessage(playerid, COLOR_GREY, "The event is currently locked.");
				return 1;
			}
			if(IsAtEvent[playerid] == 1)
			{
				SendClientMessage(playerid, COLOR_GREY, "You have already joined the event.");
				return 1;
			}
			if(EventX == 0.0 && EventY == 0.0 && EventZ == 0.0)
			{
				SendClientMessage(playerid, COLOR_GREY, "The event spawn has not been set.");
				return 1;
			}
			if(PlayerInfo[playerid][pAdminJailed]) return SendClientMessage(playerid, COLOR_WHITE, "{FF0000}Error{FFFFFF}: You cannot join events whilst in admin-prison.");
			GetPlayerPos(playerid, StuffEvent[playerid][0], StuffEvent[playerid][1], StuffEvent[playerid][2]);
			GetPlayerFacingAngle(playerid, StuffEvent[playerid][3]);
			GetPlayerHealth(playerid, StuffEvent[playerid][4]);
			GetPlayerArmour(playerid, StuffEvent[playerid][5]);
			StuffEvent2[playerid][0] = GetPlayerInterior(playerid);
			StuffEvent2[playerid][1] = GetPlayerVirtualWorld(playerid);
			EventGuns[playerid][0] = PlayerInfo[playerid][pGun0];
			EventGuns[playerid][1] = PlayerInfo[playerid][pGun1];
			EventGuns[playerid][2] = PlayerInfo[playerid][pGun2];
			EventGuns[playerid][3] = PlayerInfo[playerid][pGun3];
			EventGuns[playerid][4] = PlayerInfo[playerid][pGun4];
			EventGuns[playerid][5] = PlayerInfo[playerid][pGun5];
			EventGuns[playerid][6] = PlayerInfo[playerid][pGun6];
			EventGuns[playerid][7] = PlayerInfo[playerid][pGun7];
			EventGuns[playerid][8] = PlayerInfo[playerid][pGun8];
			EventGuns[playerid][9] = PlayerInfo[playerid][pGun9];
			EventGuns[playerid][10] = PlayerInfo[playerid][pGun10];
			EventGuns[playerid][11] = PlayerInfo[playerid][pGun11];
			EventGuns[playerid][12] = PlayerInfo[playerid][pGun12];
			SetPlayerPos(playerid, EventX, EventY, EventZ);
			IsAtEvent[playerid] = 1;
			SetPlayerVirtualWorld(playerid, EventWorld);
			SetPlayerInterior(playerid, EventInt);
			SetPlayerHealth(playerid, EventHP);
			SetPlayerArmour(playerid, EventArmour);
			ResetPlayerAdminWeaponsEx(playerid);
			ResetPlayerWeapons(playerid);
			if(EventJoinText == 1)
			{
				GameTextForPlayer(playerid, EventText, 5000, 0);
			}
			displayCenterHUDInfo(playerid,  "~r~Teleporting.", 8); //display for 8 seconds
			SendClientMessage(playerid, COLOR_LIGHTRED, "You have been teleported to the event, please follow instructions given by the admin.");
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "There are currently no events active.");
		}
		return 1;
	}
	if(strcmp(cmdtext, "/quitevent", true) == 0)
	{
		if(IsAtEvent[playerid] != 0)
		{
			ResetPlayerAdminWeaponsEx(playerid);
			SetPlayerSpawn(playerid);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have quit the event.");
		}
		return 1;
	}
	if(strcmp(cmd, "/event", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] >= 4 || PlayerInfo[playerid][pEventModd] > 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /event [name]");
				SendClientMessage(playerid, COLOR_GRAD1, "Available names: Jointext, Health, Armor, Gun1, Gun2, Gun3, Gun4, Gun5");
				SendClientMessage(playerid, COLOR_GRAD1, "Available names: Pos, Open, Lock, Start, End");
				return 1;
			}
			if(strcmp(tmp, "jointext", true) == 0)
			{
				if(EventStarted)
				{
					SendClientMessage(playerid, COLOR_GREY, "There is already a started event.");
					return 1;
				}
				new length = strlen(cmdtext);
				while((idx < length) && (cmdtext[idx] <= ' '))
				{
					idx++;
				}
				new offset = idx;
				new result[128];
				while((idx < length) && ((idx - offset) < (sizeof(result) - 1)))
				{
					result[idx - offset] = cmdtext[idx];
					idx++;
				}
				result[idx - offset] = EOS;
				if(!strlen(result))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /event jointext [text]");
					return 1;
				}
				format(EventText, sizeof(EventText), "%s", result);
				SendClientMessage(playerid, COLOR_WHITE, "You have been set the event jointext to the following.");
				GameTextForPlayer(playerid, EventText, 5000, 0);
				EventJoinText = 1;
				return 1;
			}
			if(strcmp(tmp, "health", true) == 0)
			{
				if(EventStarted)
				{
					SendClientMessage(playerid, COLOR_GREY, "There is already a started event.");
					return 1;
				}
				tmp = strtok(cmdtext, idx);
				if(!strlen(tmp))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /event health [amount]");
					return 1;
				}
				new ehp = strvalEx(tmp);
				if(ehp < 1 || ehp > 100)
				{
					SendClientMessage(playerid, COLOR_GREY, "Health can't be below 1 or above 100.");
					return 1;
				}
				EventHP = ehp;
				format(str, sizeof(str), "You have set the event health to %d.", ehp);
				SendClientMessage(playerid, COLOR_WHITE, str);
				return 1;
			}
			if(strcmp(tmp, "armor", true) == 0)
			{
				if(EventStarted)
				{
					SendClientMessage(playerid, COLOR_GREY, "There is already a started event.");
					return 1;
				}
				tmp = strtok(cmdtext, idx);
				if(!strlen(tmp))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /event armor [amount]");
					return 1;
				}
				new earmor = strvalEx(tmp);
				if(earmor < 0 || earmor > 100)
				{
					SendClientMessage(playerid, COLOR_GREY, "Armor can't be below 0 or above 100.");
					return 1;
				}
				EventArmour = earmor;
				format(str, sizeof(str), "You have set the event armor to %d.", earmor);
				SendClientMessage(playerid, COLOR_WHITE, str);
				return 1;
			}
			if(strcmp(tmp, "gun1", true) == 0)
			{
				if(EventStarted)
				{
					SendClientMessage(playerid, COLOR_GREY, "There is already a started event.");
					return 1;
				}
				tmp = strtok(cmdtext, idx);
				if(!strlen(tmp))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /event gun1 [gunid]");
					return 1;
				}
				new egun = strvalEx(tmp);
				EventWeapon1 = egun;
				format(str, sizeof(str), "You have been set the event gun1 to %d.", egun);
				SendClientMessage(playerid, COLOR_WHITE, str);
				return 1;
			}
			if(strcmp(tmp, "gun2", true) == 0)
			{
				if(EventStarted)
				{
					SendClientMessage(playerid, COLOR_GREY, "There is already a started event.");
					return 1;
				}
				tmp = strtok(cmdtext, idx);
				if(!strlen(tmp))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /event gun2 [gunid]");
					return 1;
				}
				new egun = strvalEx(tmp);
				EventWeapon2 = egun;
				format(str, sizeof(str), "You have been set the event gun2 to %d.", egun);
				SendClientMessage(playerid, COLOR_WHITE, str);
				return 1;
			}
			if(strcmp(tmp, "gun3", true) == 0)
			{
				if(EventStarted)
				{
					SendClientMessage(playerid, COLOR_GREY, "There is already a started event.");
					return 1;
				}
				tmp = strtok(cmdtext, idx);
				if(!strlen(tmp))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /event gun3 [gunid]");
					return 1;
				}
				new egun = strvalEx(tmp);
				EventWeapon3 = egun;
				format(str, sizeof(str), "You have been set the event gun3 to %d.", egun);
				SendClientMessage(playerid, COLOR_WHITE, str);
				return 1;
			}
			if(strcmp(tmp, "gun4", true) == 0)
			{
				if(EventStarted)
				{
					SendClientMessage(playerid, COLOR_GREY, "There is already a started event.");
					return 1;
				}
				tmp = strtok(cmdtext, idx);
				if(!strlen(tmp))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /event gun4 [gunid]");
					return 1;
				}
				new egun = strvalEx(tmp);
				EventWeapon4 = egun;
				format(str, sizeof(str), "You have been set the event gun4 to %d.", egun);
				SendClientMessage(playerid, COLOR_WHITE, str);
				return 1;
			}
			if(strcmp(tmp, "gun5", true) == 0)
			{
				if(EventStarted)
				{
					SendClientMessage(playerid, COLOR_GREY, "There is already a started event.");
					return 1;
				}
				tmp = strtok(cmdtext, idx);
				if(!strlen(tmp))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /event gun5 [gunid]");
					return 1;
				}
				new egun = strvalEx(tmp);
				EventWeapon5 = egun;
				format(str, sizeof(str), "You have been set the event gun5 to %d.", egun);
				SendClientMessage(playerid, COLOR_WHITE, str);
				return 1;
			}
			if(strcmp(tmp, "pos", true) == 0)
			{
				if(EventStarted)
				{
					SendClientMessage(playerid, COLOR_GREY, "There is already a started event.");
					return 1;
				}
				GetPlayerPos(playerid, EventX, EventY, EventZ);
				EventInt = GetPlayerInterior(playerid);
				EventWorld = GetPlayerVirtualWorld(playerid);
				SendClientMessage(playerid, COLOR_WHITE, "You have successfully adjusted the event position.");
			}
			if(strcmp(tmp, "open", true) == 0)
			{
				if(EventX == 0.0 && EventY == 0.0 && EventZ == 0.0)
				{
					SendClientMessage(playerid, COLOR_GREY, "The event spawn has not been set.");
					return 1;
				}
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				SendClientMessageToAll(COLOR_BLUE, "Event: An event is about to start. Type /joinevent to participate.");
				EventActive = 1;
			}
			if(strcmp(tmp, "lock", true) == 0)
			{
				if(!EventActive)
				{
					SendClientMessage(playerid, COLOR_GREY, "There are currently no events active.");
					return 1;
				}
				if(EventLocked)
				{
					EventLocked = 0;
					SendClientMessageToAll(COLOR_BLUE, "Event: The event has been unlocked.");
				}
				else
				{
					EventLocked = 1;
					SendClientMessageToAll(COLOR_BLUE, "Event: The event has been locked.");
				}
			}
			if(strcmp(tmp, "start", true) == 0)
			{
				if(EventStarted)
				{
					SendClientMessage(playerid, COLOR_GREY, "There is already a started event.");
					return 1;
				}
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				SendClientMessageToAll(COLOR_BLUE, "Event: The event has been started.");
				EventStarted = 1;
//			        for(new i = 0; i < MAX_PLAYERS; i++)
				foreach( new i: Player )
				{
					if(IsPlayerConnected(i))
					{
						if(IsAtEvent[i] == 1)
						{
							GivePlayerAdminGun(i, EventWeapon1);
							GivePlayerAdminGun(i, EventWeapon2);
							GivePlayerAdminGun(i, EventWeapon3);
							GivePlayerAdminGun(i, EventWeapon4);
							GivePlayerAdminGun(i, EventWeapon5);
						}
					}
				}
			}
			if(strcmp(tmp, "end", true) == 0)
			{
				if(EventActive == 1 || EventStarted == 1)
				{
					EventX = 0.0;
					EventY = 0.0;
					EventZ = 0.0;
					EventInt = 0;
					EventWorld = 0;
					EventHP = 100;
					EventArmour = 0;
					EventActive = 0;
					EventStarted = 0;
					EventLocked = 0;
					EventWeapon1 = 0;
					EventWeapon2 = 0;
					EventWeapon3 = 0;
					EventWeapon4 = 0;
					EventWeapon5 = 0;
					EventJoinText = 0;
					format(EventText, sizeof(EventText), "None");
					SendClientMessageToAll(COLOR_BLUE, "Event: The event has been finished.");
//						for(new i = 0; i < MAX_PLAYERS; i++)
					foreach( new i: Player )
					{
						if(IsPlayerConnected(i))
						{
							if(IsAtEvent[i] == 1)
							{
								ResetPlayerAdminWeaponsEx(i);
								SetPlayerSpecialAction(i, SPECIAL_ACTION_NONE); //Reset the Jetpack if used in event.
								SetPlayerSpawn(i);
							}
						}
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "There are currently no events active.");
				}
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this command.");
		}
		return 1;
	}
	if(!strcmp(cmd, "/setvhp", true))
	{
		tmp = strtok(cmdtext, idx);
		if(!(PlayerInfo[playerid][pAdmin] >= 3))
			return SendClientMessage(playerid, COLOR_GRAD1, "{FF0000}You're not authorized to use that command.");
		if(!strlen(tmp))
			return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /sethp [vehicle id] [health]");
		new vid = strval(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
			return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /sethp [vehicle id] [health]");
		if(!GetVehicleModel(vid))
			return SendClientMessage(playerid, COLOR_GRAD2, "Invalid vehicle ID.  Make sure the vehicle ID exists.");
		new vhealth = strval(tmp);
		SetVehicleHealth(vid, vhealth);
		return 1;
	}
	
	if(strcmp(cmd, "/vsetweather", true) == 0)
	{
		if(VoodooSpell[playerid] == 0)
		{
			return 1;
		}
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /vsetweather [weatherid]");
			return 1;
		}
		new weather;
		weather = strvalEx(tmp);
		if(weather >= 0 && weather <= 19)
		{
			SetWeather(weather);
			g_WeatherID = weather;
			VoodooSpell[playerid] = 0;
			return SendClientMessageToAll(COLOR_PURPLE, "* The weather suddenly changes.");
		}
		SendClientMessage(playerid, COLOR_GREY, "* The weather ID must not be below 0 or above 19.");
		return 1;
	}
	if(strcmp(cmd, "/setgrav", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] < 1337)
		{
			return 1;
		}
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /setgrav [number]");
			return 1;
		}
		new Float:grav;
		grav = strvalEx(tmp);
		SetGravity(grav);
		SendClientMessage(playerid, COLOR_GREY, "Gravity has been set for everyone.");
		return 1;
	}
	if(strcmp(cmd, "/savechars", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] >= 4 || PlayerInfo[playerid][pDev] >= 3)
		{
			SaveChars();
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* Player/Server data has been saved.");
		}
		else
		{
			AdmErrorMsg;
		}
		return 1;
	}
	if(strcmp(cmd, "/load", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] < 1337)
		{
			AdmErrorMsg;
			return 1;
		}
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /load [name]");
			SendClientMessage(playerid, COLOR_GREY, "Available names: Families, Points, Entrances, Bizz, Surnames");
			return 1;
		}
		if(strcmp(tmp, "families", true) == 0)
		{
			LoadFamilies();
			SendClientMessage(playerid, COLOR_GRAD1, "Families.cfg has successfully been reloaded.");
			return 1;
		}
		if(strcmp(tmp, "entrances", true) == 0)
		{
			LoadEntrances();
			SendClientMessage(playerid, COLOR_GRAD1, "Entrances.cfg has successfully been reloaded.");
			return 1;
		}
		if(strcmp(tmp, "bizz", true) == 0)
		{
			LoadBusinesses();
			SendClientMessage(playerid, COLOR_GRAD1, "Bizz.cfg has successfully been reloaded.");
			return 1;
		}
		if(strcmp(tmp, "surnames", true) == 0)
		{
			LoadSurnames();
			SendClientMessage(playerid, COLOR_GRAD1, "Surnames.cfg has successfully been reloaded.");
			return 1;
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "Invalid load name.");
		}
		return 1;
	}
	if(strcmp(cmd, "/getpos", true) == 0)
	{
		new Float:x, Float:y, Float:z;
		GetPlayerPos(playerid, x, y, z);
		format(str, sizeof(str), "Current Position: %0.2f, %0.2f, %0.2f", x, y, z);
		SendClientMessage(playerid, SAMP_COLOR, str);
		return 1;
	}
	if(strcmp(cmd, "/getvw", true) == 0)
	{
		format(str, sizeof(str), "Current Virtual World: %d", GetPlayerVirtualWorld(playerid));
		SendClientMessage(playerid, SAMP_COLOR, str);
		return 1;
	}
	if(strcmp(cmd, "/seizebiz", true) == 0)
	{
		if(	PlayerInfo[playerid][pMember] != FACTION_GOV &&
			PlayerInfo[playerid][pMember] != FACTION_JD)
			return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this.");
		
		
		if(PlayerInfo[playerid][pMember] == FACTION_GOV && PlayerInfo[playerid][pRank] < 2)
			return SendClientMessage(playerid, COLOR_GREY, "You must be R2+ to use this.");

		new id = IsPlayerNearBiz(playerid);
		if(id == -1)
			return SendClientMessage(playerid, COLOR_GREY, "You are not near a business.");
		
		if(!BizInfo[id][bOwned])
			return SendClientMessage(playerid, COLOR_GREY, "You cannot seize a business that is not owned by someone.");
		
		// Business is not seized
		if(BizInfo[id][bSeized] == 0)
		{
			format(str, sizeof(str), "* You have seized %s's business.", BizInfo[id][bOwner]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

			// Seize for government
			if(PlayerInfo[playerid][pMember] == 6) {
				BizInfo[id][bSeized] = 1;
				format(str, sizeof(str), "{AFAFAF}%s{AFAFAF}\n{FFFFFF}This business has been seized by the\n{696969}Government", BizInfo[id][bName]);
			}
			// Seize for Judicial Department
			else if(PlayerInfo[playerid][pMember] == 7) {
				BizInfo[id][bSeized] = 2;
				format(str, sizeof(str), "{AFAFAF}%s{AFAFAF}\n{FFFFFF}This business has been seized by the\n{BA6103}Judicial Department", BizInfo[id][bName]);
			}
		
			UpdateDynamic3DTextLabelText(Text3D:BizText[id], COLOR_BIZTEXT, str);
		}
		// Business is seized
		else 
		{
			if(	BizInfo[id][bSeized] == 1 &&
				PlayerInfo[playerid][pMember] != 6)
				return SendClientMessage(playerid, COLOR_GREY, "You are not able to unseize this business.");

			if(	BizInfo[id][bSeized] == 2 &&
				PlayerInfo[playerid][pMember] != 7)
				return SendClientMessage(playerid, COLOR_GREY, "You are not able to unseize this business.");

			format(str, sizeof(str), "You have unseized %s's business.", BizInfo[id][bOwner]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);

			BizInfo[id][bSeized] = 0;

			if(BizInfo[id][bLocked] == 1)
			{
				format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s\nEntry Fee: Locked", BizInfo[id][bName], BizInfo[id][bOwner]);
			}
			else
			{
				format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s", BizInfo[id][bName], BizInfo[id][bOwner]);
			}
			UpdateDynamic3DTextLabelText(Text3D:BizText[id], COLOR_BIZTEXT, str);
		}
		SaveBusinesses();
		return 1;
	}
	if(strcmp(cmd, "/sate", true) == 0)
	{
		if(PlayerInfo[playerid][pLeader] == 6 || PlayerInfo[playerid][pLeader] == 5)
		{
			if(UsingSate[playerid] == 1)
			{
				SetCameraBehindPlayer(playerid);
				PutPlayerInVehicle(playerid, gLastCar[playerid], 2);
				TogglePlayerControllable(playerid, true);
				UsingSate[playerid] = 0;
				SetPlayerPos(playerid, oldsposx[playerid], oldsposy[playerid], oldsposz[playerid]);
				oldsposx[playerid] = 0.0;
				oldsposy[playerid] = 0.0;
				oldsposz[playerid] = 0.0;
				newsposx[playerid] = 0.0;
				newsposy[playerid] = 0.0;
				newsposz[playerid] = 0.0;
				ZOOM[playerid] = 0;
				SetCameraBehindPlayer(playerid);
				return 1;
			}
			if(IsPlayerInRangeOfPoint(playerid, 3.0, 213.5529,1818.7916,6.4216))
			{
				if(UsingSate[playerid] == 0)
				{
					SetPlayerFacingAngle(playerid, 0.0);
					GetPlayerPos(playerid, oldsposx[playerid], oldsposy[playerid], oldsposz[playerid]);
					SetPlayerPos(playerid, oldsposx[playerid], oldsposy[playerid], oldsposz[playerid] +500);
					gLastCar[playerid] = GetPlayerVehicleID(playerid);
					TogglePlayerControllable(playerid, false);
					GetPlayerPos(playerid, newsposx[playerid], newsposy[playerid], newsposz[playerid]);
					SetPlayerCameraPos(playerid, newsposx[playerid], newsposy[playerid], newsposz[playerid] +200);
					SendClientMessage(playerid, COLOR_GREY, "Use up, down, left, right to navigate and fire key to adjust the zoom !");
					SetPlayerCameraLookAt(playerid, newsposx[playerid], newsposy[playerid] +5, newsposz[playerid]);
					UsingSate[playerid] = 1;
					ZOOM[playerid] = 1;
					return 1;
				}
			}
			else return 1;
		}
		else if(PlayerInfo[playerid][pMember] == 2 || PlayerInfo[playerid][pGroup] == GROUP_S9)
		{
			if(UsingSate[playerid] == 1)
			{
				SetCameraBehindPlayer(playerid);
				PutPlayerInVehicle(playerid, gLastCar[playerid], 2);
				TogglePlayerControllable(playerid, true);
				UsingSate[playerid] = 0;
				SetPlayerPos(playerid, oldsposx[playerid], oldsposy[playerid], oldsposz[playerid]);
				oldsposx[playerid] = 0.0;
				oldsposy[playerid] = 0.0;
				oldsposz[playerid] = 0.0;
				newsposx[playerid] = 0.0;
				newsposy[playerid] = 0.0;
				newsposz[playerid] = 0.0;
				ZOOM[playerid] = 0;
				SetCameraBehindPlayer(playerid);
				return 1;
			}
			if(IsPlayerInAnyVehicle(playerid))
			{
				new playervehid = GetPlayerVehicleID(playerid);
				if(sVehicleInfo[GetDynamicVehicleID(playervehid)][v_faction] != 2 && sVehicleInfo[GetDynamicVehicleID(playervehid)][v_group] != GROUP_S9)
				{
					SendClientMessage(playerid, COLOR_GREY, "You are not in the back seat of a DEA/S9 vehicle !");
					return 1;
				}
				if(GetPlayerVehicleSeat(playerid) == 2 || GetPlayerVehicleSeat(playerid) == 3)
				{
					if(UsingSate[playerid] == 0)
					{
						SetPlayerFacingAngle(playerid, 0.0);
						GetPlayerPos(playerid, oldsposx[playerid], oldsposy[playerid], oldsposz[playerid]);
						SetPlayerPos(playerid, oldsposx[playerid], oldsposy[playerid], oldsposz[playerid] +500);
						gLastCar[playerid] = playervehid;
						TogglePlayerControllable(playerid, false);
						GetPlayerPos(playerid, newsposx[playerid], newsposy[playerid], newsposz[playerid]);
						SetPlayerCameraPos(playerid, newsposx[playerid], newsposy[playerid], newsposz[playerid] +200);
						SendClientMessage(playerid, COLOR_GREY, "Use up, down, left, right to navigate and fire key to adjust the zoom !");
						SetPlayerCameraLookAt(playerid, newsposx[playerid], newsposy[playerid] +5, newsposz[playerid]);
						UsingSate[playerid] = 1;
						ZOOM[playerid] = 1;
						return 1;
					}
				}
			}
			else return SendClientMessage(playerid, COLOR_GREY, "You are not in vehicle !");
		}
		else return SendClientMessage(playerid, COLOR_GREY, "You are not a member of the DEA !");
	}
	if(strcmp(cmd, "/mute", true) == 0)
	{
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /mute [playerid/PartOfName]");
			return 1;
		}
		new playa;
		playa = ReturnUser(tmp);
		if(PlayerInfo[playerid][pAdmin] >= 1)
		{
			if(IsPlayerConnected(playa))
			{
				if(playa != INVALID_PLAYER_ID)
				{
					strmid(giveplayer, PlayerICName(playa), 0, MAX_PLAYER_NAME);
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					if(PlayerInfo[playa][pMuted] == 0)
					{
						PlayerInfo[playa][pMuted] = 1;
						format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s was muted by %s.", giveplayer, sendername);
						ABroadCast(COLOR_WHITE, str, 1);
					}
					else
					{
						PlayerInfo[playa][pMuted] = 0;
						format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s was unmuted by %s.", giveplayer, sendername);
						ABroadCast(COLOR_WHITE, str, 1);
					}
				}
			}
		}
		else
		{
			AdmErrorMsg;
		}
		return 1;
	}
	if(strcmp(cmd, "/admute", true) == 0)
	{
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /admute [playerid/PartOfName]");
			return 1;
		}
		new playa;
		playa = ReturnUser(tmp);
		if(PlayerInfo[playerid][pAdmin] >= 1)
		{
			if(IsPlayerConnected(playa))
			{
				if(playa != INVALID_PLAYER_ID)
				{
					strmid(giveplayer, PlayerICName(playa), 0, MAX_PLAYER_NAME);
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					if(PlayerInfo[playa][pAdMuted] == 0)
					{
						PlayerInfo[playa][pAdMuted] = 1;
						format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s's ad permission was revoked by %s.", giveplayer, sendername);
						ABroadCast(COLOR_WHITE, str, 1);
					}
					else
					{
						PlayerInfo[playa][pAdMuted] = 0;
						format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s's ad permission was granted by %s.", giveplayer, sendername);
						ABroadCast(COLOR_WHITE, str, 1);
					}
				}
			}
		}
		else
		{
			AdmErrorMsg;
		}
		return 1;
	}
	if(strcmp(cmd, "/bugmute", true) == 0)
	{
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /bugmute [playerid/PartOfName]");
			return 1;
		}
		new playa;
		playa = ReturnUser(tmp);
		if(PlayerInfo[playerid][pAdmin] >= 1 || PlayerInfo[playerid][pTester] >= TESTERRANK_SENIOR)
		{
			if(IsPlayerConnected(playa))
			{
				if(playa != INVALID_PLAYER_ID)
				{
					strmid(giveplayer, PlayerICName(playa), 0, MAX_PLAYER_NAME);
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					if(PlayerInfo[playa][pBugMuted] == 0)
					{
						PlayerInfo[playa][pBugMuted] = 1;
						format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s's /reportbug permission was revoked by %s.", giveplayer, sendername);
						ABroadCast(COLOR_WHITE, str, 1);
						if(PlayerInfo[playerid][pTester] >= 1 && PlayerInfo[playerid][pAdmin] == 0)
						{
							return SendClientMessage(playerid, COLOR_WHITE, str);
						}
					}
					else
					{
						PlayerInfo[playa][pBugMuted] = 0;
						format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s's /reportbug permission was granted by %s.", giveplayer, sendername);
						ABroadCast(COLOR_WHITE, str, 1);
						if(PlayerInfo[playerid][pTester] >= 1 && PlayerInfo[playerid][pAdmin] == 0)
						{
							return SendClientMessage(playerid, COLOR_WHITE, str);
						}
					}
				}
			}
		}
		else
		{
			AdmErrorMsg;
		}
		return 1;
	}
	if(strcmp(cmd, "/banip", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] > 1 || PlayerInfo[playerid][pBanAppealer])
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_WHITE, "{00BFFF}Usage:{FFFFFF} /banip [ip]");
				return 1;
			}
			format(str, sizeof(str), "banip %s", tmp);
			SendRconCommand(str);
			SendRconCommand("reloadbans");
			strmid(sendername, PlayerOOCName(playerid), 0, MAX_PLAYER_NAME);
			format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has banned IP: %s", sendername, tmp);
			ABroadCast(COLOR_WHITE, str, 1);
		}
		else
		{
			SendClientMessage(playerid, COLOR_GRAD2, "{FF0000}You're not authorized to use that command.");
		}
		return 1;
	}
	if(strcmp(cmd, "/getprisontime", true) == 0 || strcmp(cmd, "/cp", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] >= 1)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /getprisontime [playerid/PartOfName]");
				return 1;
			}
			giveplayerid = ReturnUser(tmp);
			if(IsPlayerConnected(giveplayerid))
			{
				if(giveplayerid != INVALID_PLAYER_ID)
				{
					GetPlayerICName(giveplayerid, sendername, sizeof(sendername));
					format(str, sizeof(str), "Name: %s, Time: %d minutes Prisoned By %s Reason: %s", PlayerICName(giveplayerid), PlayerInfo[giveplayerid][pJailTime] / 60, PlayerInfo[giveplayerid][pPrisonedBy], PlayerInfo[giveplayerid][pPrisonReason]);
					SendClientMessage(playerid, COLOR_GREY, str);
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GRAD1, "Player does not exist.");
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GRAD1, "You are not authorized to use this command.");
		}
		return 1;
	}
	if(strcmp(cmd, "/shuffle", true) ==0)
	{
		if(PlayerInfo[playerid][pDeck] < 1)
		{
			SendClientMessage(playerid, COLOR_GRAD2, "You don't have a deck of cards.");
			return 1;
		}
		ShuffleDeck(playerid);
		PlayerPlaySound(playerid, 5600, 0.0, 0.0, 0.0);
		format(str, sizeof(str), "* %s has shuffled their deck of cards.", PlayerICName(playerid));
		ProxDetector(5.0, playerid, str, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW);
		return 1;
	}
	if(strcmp(cmd, "/dealcards", true) ==0)
	{
		if(PlayerInfo[playerid][pDeck] < 1)
		{
			SendClientMessage(playerid, COLOR_GRAD2, "You don't have a deck of cards.");
			return 1;
		}
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dealcards [playerid] [amount]");
			return 1;
		}
		new targetplayerid = ReturnUser(tmp);
		if(IsPlayerConnected(targetplayerid))
		{
			if(IsPlayerInRangeOfPlayer(8.0, playerid, targetplayerid))
			{
				tmp = strtok(cmdtext, idx);
				if(!strlen(tmp))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dealcards [playerid/PartOfName] [amount]");
					return 1;
				}
				new numcards = strval(tmp);
				if(numcards < 1 || numcards > 5)
				{
					SendClientMessage(playerid, COLOR_GREY, "You must deal between 1 and 5 cards.");
					return 1;
				}
				if((CardsHave[targetplayerid] + numcards) > 5)
				{
					SendClientMessage(playerid, COLOR_GREY, "That player doesn't have enough open card slots.");
					return 1;
				}
				if((CardsDealt[playerid] + numcards) > 52)
				{
					SendClientMessage(playerid, COLOR_GREY, "You don't have enough cards in the deck to deal that many.");
					return 1;
				}
				for(new i = 0; i < numcards; i++)
				{
					DealCard(playerid, targetplayerid);
				}
				format(str, sizeof(str), "* %s has dealt %d cards to %s.", PlayerICName(playerid), numcards, PlayerICName(targetplayerid));
				ProxDetector(5.0, playerid, str, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW);
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not near that player.");
			}
		}
		else
		{
			format(str, sizeof(str), "{FF0000}Error:{FFFFFF} That player isn't connected.", targetplayerid);
			SendClientMessage(playerid, COLOR_GRAD1, str);
		}
		return 1;
	}
	if(strcmp(cmd, "/discard", true) ==0)
	{
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /discard [card slot/all]");
			return 1;
		}
		if(strcmp(tmp, "all", true) ==0)
		{
			DiscardAll(playerid);
			return 1;
		}
		while(strlen(tmp))
		{
			new discardid = strval(tmp);
			tmp = strtok(cmdtext, idx);
			if(discardid < 1 || discardid > 5)
			{
				SendClientMessage(playerid, COLOR_GREY, "Card cannot be below 1 or above 5.");
				continue;
			}
			else if(discardid == 1 && Cards[playerid][0] < 1)
			{
				SendClientMessage(playerid, COLOR_GREY, "You don't have a card with that number.");
				continue;
			}
			else if(discardid == 2 && Cards[playerid][1] < 1)
			{
				SendClientMessage(playerid, COLOR_GREY, "You don't have a card with that number.");
				continue;
			}
			else if(discardid == 3 && Cards[playerid][2] < 1)
			{
				SendClientMessage(playerid, COLOR_GREY, "You don't have a card with that number.");
				continue;
			}
			else if(discardid == 4 && Cards[playerid][3] < 1)
			{
				SendClientMessage(playerid, COLOR_GREY, "You don't have a card with that number.");
				continue;
			}
			else if(discardid == 5 && Cards[playerid][4] < 1)
			{
				SendClientMessage(playerid, COLOR_GREY, "You don't have a card with that number.");
				continue;
			}
			Discard(playerid, discardid -1);
		}
		return 1;
	}
	if(strcmp(cmd, "/cards", true) ==0)
	{
		SendClientMessage(playerid, COLOR_WHITE, "Cards:");
		format(str, sizeof(str), "Card 1: %s.", CardNames[Cards[playerid][0]]);
		SendClientMessage(playerid, COLOR_GREY, str);
		format(str, sizeof(str), "Card 2: %s.", CardNames[Cards[playerid][1]]);
		SendClientMessage(playerid, COLOR_GREY, str);
		format(str, sizeof(str), "Card 3: %s.", CardNames[Cards[playerid][2]]);
		SendClientMessage(playerid, COLOR_GREY, str);
		format(str, sizeof(str), "Card 4: %s.", CardNames[Cards[playerid][3]]);
		SendClientMessage(playerid, COLOR_GREY, str);
		format(str, sizeof(str), "Card 5: %s.", CardNames[Cards[playerid][4]]);
		SendClientMessage(playerid, COLOR_GREY, str);
		return 1;
	}
	if(strcmp(cmd, "/showcard", true) ==0)
	{
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /showcard [card slot/all]");
			return 1;
		}
		if(strcmp(tmp, "all", true) ==0)
		{
			ShowHand(playerid);
			return 1;
		}
		while(strlen(tmp))
		{
			new discardid = strval(tmp);
			tmp = strtok(cmdtext, idx);
			if(discardid < 1 || discardid > 5)
			{
				SendClientMessage(playerid, COLOR_GREY, "Card cannot be below 1 or above 5.");
				continue;
			}
			else if(discardid == 1 && Cards[playerid][0] < 1)
			{
				SendClientMessage(playerid, COLOR_GREY, "You don't have a card with that number.");
				continue;
			}
			else if(discardid == 2 && Cards[playerid][1] < 1)
			{
				SendClientMessage(playerid, COLOR_GREY, "You don't have a card with that number.");
				continue;
			}
			else if(discardid == 3 && Cards[playerid][2] < 1)
			{
				SendClientMessage(playerid, COLOR_GREY, "You don't have a card with that number.");
				continue;
			}
			else if(discardid == 4 && Cards[playerid][3] < 1)
			{
				SendClientMessage(playerid, COLOR_GREY, "You don't have a card with that number.");
				continue;
			}
			else if(discardid == 5 && Cards[playerid][4] < 1)
			{
				SendClientMessage(playerid, COLOR_GREY, "You don't have a card with that number.");
				continue;
			}
			Showcard(playerid, discardid -1);
		}
		return 1;
	}
	if(strcmp(cmd, "/nametag", true) == 0)
	{
		new x_tag[64];
		x_tag = strtok(cmdtext, idx);
		if(ADuty[playerid] == true) return SendClientMessage(playerid, COLOR_GREY, "Unable to use whilst on admin duty!");
		if(!strlen(x_tag))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /nametag [tagname]");
			SendClientMessage(playerid, COLOR_GRAD1, "Tag names: VIP, Helper, Admin, Probie, Developer, Tester, Mapper, None");
			return 1;
		}
		
		if(strcmp(x_tag, "vip", true) == 0)
		{
			if(usingNameTag[playerid])
				return SendClientMessage(playerid, COLOR_GREY, "You already have your nametag enabled, use \"None\" to disable it.");
			
			if(PlayerInfo[playerid][pDonateRank] == 0)
				return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this nametag.");
			if(PlayerInfo[playerid][pDonateRank] == 4)
			{
			nametagText[playerid] = CreateDynamic3DTextLabel("\nDiamond VIP", COLOR_LIGHTBLUE, 0.0, 0.0, 0.0 +0.32, 20.0, playerid, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* Diamond VIP nametag enabled.");
			usingNameTag[playerid] = 1;
			}
			else
			{
				nametagText[playerid] = CreateDynamic3DTextLabel("\nVIP", 0x800080FF0, 0, 0.0, 0.0 +0.32, 20.0, playerid, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "* VIP nametag enabled.");
				usingNameTag[playerid] = 1;
			}
		}
		else if(strcmp(x_tag, "helper", true) == 0)
		{
			if(usingNameTag[playerid])
				return SendClientMessage(playerid, COLOR_GREY, "You already have your nametag enabled, use \"None\" to disable it.");
			
			if(PlayerInfo[playerid][pHelper] == 0)
				return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this nametag.");

			nametagText[playerid] = CreateDynamic3DTextLabel("\nHelper", NEWBIE_COLOR, 0.0, 0.0, 0.0 +0.32, 20.0, playerid, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* Helper nametag enabled.");
			usingNameTag[playerid] = 1;
			return 1;
		}
		else if(strcmp(x_tag, "admin", true) == 0)
		{
			if(usingNameTag[playerid])
				return SendClientMessage(playerid, COLOR_GREY, "You already have your nametag enabled, use \"None\" to disable it.");
			
			if(PlayerInfo[playerid][pAdmin] < 2)
				return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this nametag.");

			if(!ADuty[playerid] && PlayerInfo[playerid][pAdmin] == 1) return SendClientMessage(playerid, COLOR_GREY, "You are not on duty.");
			nametagText[playerid] = CreateDynamic3DTextLabel("\nAdministrator", 0xFEB918FF, 0.0, 0.0, 0.0 +0.32, 20.0, playerid, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* Administrator nametag enabled.");
			usingNameTag[playerid] = 1;
			return 1;
		}
		else if(strcmp(x_tag, "probie", true) == 0)
		{
			if(usingNameTag[playerid])
				return SendClientMessage(playerid, COLOR_GREY, "You already have your nametag enabled, use \"None\" to disable it.");
			
			if(PlayerInfo[playerid][pAdmin] != 1)
				return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this nametag.");
			nametagText[playerid] = CreateDynamic3DTextLabel("\nProbationary Administrator", 0xFEB918FF, 0.0, 0.0, 0.0 +0.32, 20.0, playerid, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* Probie Administrator nametag enabled.");
			usingNameTag[playerid] = 1;
			return 1;
		}
		else if(strcmp(x_tag, "developer", true) == 0)
		{
			if(usingNameTag[playerid])
				return SendClientMessage(playerid, COLOR_GREY, "You already have your nametag enabled, use \"None\" to disable it.");
			
			if(PlayerInfo[playerid][pDev] == DEVRANK_NONE)
				return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this nametag.");

			nametagText[playerid] = CreateDynamic3DTextLabel("\nDeveloper", 0xFEE349FF, 0.0, 0.0, 0.0 +0.32, 20.0, playerid, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* Developer nametag enabled.");
			usingNameTag[playerid] = 1;
			return 1;
		}
		else if(strcmp(x_tag, "tester", true) == 0)
		{
			if(usingNameTag[playerid])
				return SendClientMessage(playerid, COLOR_GREY, "You already have your nametag enabled, use \"None\" to disable it.");
			
			if(PlayerInfo[playerid][pTester] == TESTERRANK_NONE)
				return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this nametag.");

			nametagText[playerid] = CreateDynamic3DTextLabel("\nBeta Tester", 0x00CA00FF, 0.0, 0.0, 0.0 +0.32, 20.0, playerid, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* Beta Tester nametag enabled.");
			usingNameTag[playerid] = 1;
			return 1;
		}
		else if(strcmp(x_tag, "mapper", true) == 0)
		{
			if(usingNameTag[playerid])
				return SendClientMessage(playerid, COLOR_GREY, "You already have your nametag enabled, use \"None\" to disable it.");
			
			if(PlayerInfo[playerid][pMapper] == MAPPERRANK_NONE)
				return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this nametag.");

			nametagText[playerid] = CreateDynamic3DTextLabel("\nMapper", 0xFFFFFFAA, 0.0, 0.0, 0.0 +0.32, 20.0, playerid, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* Mapper nametag enabled.");
			usingNameTag[playerid] = 1;
			return 1;
		}
		else if(strcmp(x_tag, "none", true) == 0)
		{
			if(!usingNameTag[playerid])
				return SendClientMessage(playerid, COLOR_GREY, "You are not using a nametag.");
			
			DestroyDynamic3DTextLabel(nametagText[playerid]);
			nametagText[playerid] = Text3D:INVALID_3DTEXT_ID;
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* Nametag has been disabled.");
			usingNameTag[playerid] = 0;
			return 1;
		}
		return 1;
	}
	/*if(strcmp(cmd, "/newbietag", true) == 0)
	{
		new x_tag[64];
		x_tag = strtok(cmdtext, idx);
		if(!strlen(x_tag))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /newbietag [tagname]");
			SendClientMessage(playerid, COLOR_GRAD1, "Tag names: Newbie, Player, VIP, Helper, Admin, Developer, Famed, Tester, Mapper, Designer, Oldschool");
		}
		else if(strcmp(x_tag, "Newbie", true) == 0)
		{
			if(PlayerInfo[playerid][pLevel] == 1 || PlayerInfo[playerid][pAdmin] > 1)
			{
				PlayerInfo[playerid][pTag] = NTAG_NEWBIE;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Newbie");
				return 1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_WHITE, "You are not authorized to use this tag");
				return 1;
			}
		}
		else if(strcmp(x_tag, "player", true) == 0)
		{
			if(PlayerInfo[playerid][pLevel] >= 2 || PlayerInfo[playerid][pAdmin] >= 1)
			{
				PlayerInfo[playerid][pTag] = NTAG_PLAYER;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Player");
				return 1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_WHITE, "You are not authorized to use this tag");
				return 1;
			}
		}
		else if(strcmp(x_tag, "vip", true) == 0)
		{
			if(PlayerInfo[playerid][pDonateRank] == 0)
			{
				SendClientMessage(playerid, COLOR_WHITE, "You are not authorized to use this tag");
				return 1;
			}
			new viprank = PlayerInfo[playerid][pDonateRank];
			if(viprank == 1)
			{
				PlayerInfo[playerid][pTag] = NTAG_BVIP;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Bronze VIP");
				return 1;
			}
			else if(viprank == 2)
			{
				PlayerInfo[playerid][pTag] = NTAG_SVIP;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Silver VIP");
				return 1;
			}
			else if(viprank == 3)
			{
				PlayerInfo[playerid][pTag] = NTAG_GVIP;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Gold VIP");
				return 1;
			}
		}
		else if(strcmp(x_tag, "helper", true) == 0)
		{
			if(PlayerInfo[playerid][pHelper] == 0)
			{
				SendClientMessage(playerid, COLOR_WHITE, "You are not authorized to use this tag");
				return 1;
			}
			new helperrank = PlayerInfo[playerid][pHelper];
			if(helperrank == 1)
			{
				PlayerInfo[playerid][pTag] = NTAG_THELPER;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Trial Helper");
			}
			if(helperrank == 2)
			{
				PlayerInfo[playerid][pTag] = NTAG_HELPER;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Helper");
			}
			if(helperrank == 3)
			{
				PlayerInfo[playerid][pTag] = NTAG_SHELPER;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Senior Helper");
			}
			if(helperrank == 4)
			{
				PlayerInfo[playerid][pTag] = NTAG_HHELPER;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Head Helper");
			}
			if(helperrank == 5)
			{
				PlayerInfo[playerid][pTag] = NTAG_DOPR;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Director of The Helpers Team");
			}
			return 1;
		}
		else if(strcmp(x_tag, "admin", true) == 0)
		{
			if(PlayerInfo[playerid][pAdmin] <= 1)
			{
				SendClientMessage(playerid, COLOR_WHITE, "You are not authorized to use this tag");
				return 1;
			}
			PlayerInfo[playerid][pTag] = NTAG_ADMIN;
			SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Admin");
			return 1;
		}
		else if(strcmp(x_tag, "developer", true) == 0)
		{
			if(PlayerInfo[playerid][pDev] == DEVRANK_NONE)
			{
				SendClientMessage(playerid, COLOR_WHITE, "You are not authorized to use this tag");
				return 1;
			}
			new devrank = PlayerInfo[playerid][pDev];
			if(devrank == 1)
			{
				PlayerInfo[playerid][pTag] = NTAG_JUNIORDEV;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Junior Developer");
			}
			if(devrank == 2)
			{
				PlayerInfo[playerid][pTag] = NTAG_SENIORDEV;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Senior Developer");
			}
			if(devrank == 3)
			{
				PlayerInfo[playerid][pTag] = NTAG_MANAGERDEV;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Development Manager");
			}
			if(devrank == 4)
			{
				PlayerInfo[playerid][pTag] = NTAG_LEADDEV;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Lead Developer");
			}
			return 1;
		}
		else if(strcmp(x_tag, "famed", true) == 0)
		{
			if(PlayerInfo[playerid][pFamed] == 0)
			{
				SendClientMessage(playerid, COLOR_WHITE, "You are not authorized to use this tag");
				return 1;
			}
			new famrank = PlayerInfo[playerid][pFamed];
			if(famrank == 1)
			{
				PlayerInfo[playerid][pTag] = NTAG_FAMED;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Famed");
			}
			if(famrank == 2)
			{
				PlayerInfo[playerid][pTag] = NTAG_MANAGERFAMED;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Famed Manager");
			}
			return 1;
		}
		else if(strcmp(x_tag, "Oldschool", true) == 0)
        {
            if(PlayerInfo[playerid][pOldSchool] == 0)
            {
                SendClientMessage(playerid, COLOR_WHITE, "You are not authorized to use this tag");
                return 1;
            }
            new osrank = PlayerInfo[playerid][pOldSchool];
            if(osrank == 1)
            {
                PlayerInfo[playerid][pTag] = NTAG_OLDSCHOOL;
                SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Old School");
            }
            if(osrank == 2)
            {
                PlayerInfo[playerid][pTag] = NTAG_MODOLDSCHOOL;
                SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Old School Moderator");
            }
            return 1;
        }
		else if(strcmp(x_tag, "tester", true) == 0)
		{
			if(PlayerInfo[playerid][pTester] == TESTERRANK_NONE)
			{
				SendClientMessage(playerid, COLOR_WHITE, "You are not authorized to use this tag");
				return 1;
			}
			new testrank = PlayerInfo[playerid][pTester];
			if(testrank == 1)
			{
				PlayerInfo[playerid][pTag] = NTAG_TESTER;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Tester");
			}
			if(testrank == 2)
			{
				PlayerInfo[playerid][pTag] = NTAG_STESTER;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Tester");
			}
			if(testrank == 3)
			{
				PlayerInfo[playerid][pTag] = NTAG_MANAGERTESTER;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Tester Manager");
			}
			return 1;
		}
		else if(strcmp(x_tag, "mapper", true) == 0)
		{
			if(PlayerInfo[playerid][pMapper] == MAPPERRANK_NONE)
			{
				SendClientMessage(playerid, COLOR_WHITE, "You are not authorized to use this tag");
				return 1;
			}
			new testrank = PlayerInfo[playerid][pMapper];
			if(testrank == 1)
			{
				PlayerInfo[playerid][pTag] = NTAG_MAPPER;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Mapper");
			}
			if(testrank == 2)
			{
				PlayerInfo[playerid][pTag] = NTAG_SENIORMAPPER;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Senior Mapper");
			}
			if(testrank == 3)
			{
				PlayerInfo[playerid][pTag] = NTAG_MANAGERMAPPER;
				SendClientMessage(playerid, COLOR_WHITE, "Your tag in newbie chat was changed to Mapping Manager");
			}
			return 1;
		}
	}	*/
	if(strcmp(cmd, "/skills", true) ==0)
	{ // functions such as GetPlayerJobLevel are defined in core.inc in the jobs folder
		new tline[1200], line1[128], line2[128], line3[128], line4[128], line5[128], line6[128], line7[128], line8[128], line9[128], line10[128],
			line11[128], line12[128], line13[128], line14[128];
		new req1, req2, jlevel;

		new templine[128], templine2[128];

		new joblevel1 = GetPlayerJobLevel(playerid, JOB_DETECTIVE), joblevel2 = GetPlayerJobLevel(playerid, JOB_MECHANIC);

		format(line1, sizeof(line1), "{007BD0}Detective Skill Level: {B4B5B7}%d\t\t\t\t\t{007BD0}Mechanic Skill Level: {B4B5B7}%d", joblevel1, joblevel2);
		if(joblevel1 < 5) format(templine, sizeof(templine), "You need to find %d more people to level up.", GetPlayerJobSkillLevel(playerid, JOB_DETECTIVE));
		else if(joblevel1 == 5) format(templine, sizeof(templine), "You are the maximum level for this job.\t");
		if(joblevel2 < 5) format(templine2, sizeof(templine2), "You need to fix/fill %d more vehicles to level up.", GetPlayerJobSkillLevel(playerid, JOB_MECHANIC));
		else if(joblevel2 == 5) format(templine2, sizeof(templine2), "You are the maximum level for this job.");
		format(line2, sizeof(line2), "%s\t\t\t%s", templine, templine2);

		joblevel1 = GetPlayerJobLevel(playerid, JOBID_LAWYER), joblevel2 = GetPlayerJobLevel(playerid, JOB_BOXER);
		format(line3, sizeof(line3), "{007BD0}Lawyer Skill Level: {B4B5B7}%d\t\t\t\t\t\t{007BD0}Boxer Skill Level: {B4B5B7}%d", joblevel1, joblevel2);
		if(joblevel1 < 5) format(templine, sizeof(templine), "You need to free %d more people to level up.", GetPlayerJobSkillLevel(playerid, JOB_LAWYER));
		else if(joblevel1 == 5) format(templine, sizeof(templine), "You are the maximum level for this job.\t");
		if(joblevel2 < 5) format(templine2, sizeof(templine2), "You need to win %d more boxing matches to level up.", GetPlayerJobSkillLevel(playerid, JOB_BOXER));
		else if(joblevel2 == 5) format(templine2, sizeof(templine2), "You are the maximum level for this job.");
		format(line4, sizeof(line4), "%s\t\t\t%s", templine, templine2);
		//Sex & Fishing
		jlevel = PlayerInfo[playerid][pSexSkill];
		if(jlevel >= 0 && jlevel <= 49)
		{
			joblevel1 = 1;
			req1 = 50 -jlevel;
		}
		else if(jlevel >= 50 && jlevel <= 99)
		{
			joblevel1 = 2;
			req1 = 100 -jlevel;
		}
		else if(jlevel >= 100 && jlevel <= 199)
		{
			joblevel1 = 3;
			req1 = 200 -jlevel;
		}
		else if(jlevel >= 200 && jlevel <= 399)
		{
			joblevel1 = 4;
			req1 = 400 -jlevel;
		}
		else if(jlevel >= 400)
		{
			joblevel1 = 5;
		}
		jlevel = PlayerInfo[playerid][pFishSkill];
		if(jlevel >= 0 && jlevel <= 49)
		{
			joblevel2 = 1;
			req2 = 50 -jlevel;
		}
		else if(jlevel >= 50 && jlevel <= 249)
		{
			joblevel2 = 2;
			req2 = 250 -jlevel;
		}
		else if(jlevel >= 250 && jlevel <= 499)
		{
			joblevel2 = 3;
			req2 = 500 -jlevel;
		}
		else if(jlevel >= 500 && jlevel <= 999)
		{
			joblevel2 = 4;
			req2 = 1000 -jlevel;
		}
		else if(jlevel >= 1000)
		{
			joblevel2 = 5;
		}
		format(line5, sizeof(line5), "{007BD0}Sex Skill Level: {B4B5B7}%d\t\t\t\t\t\t{007BD0}Fishing Skill Level: {B4B5B7}%d", joblevel1, joblevel2);
		if(joblevel1 < 5) format(templine, sizeof(templine), "You need to have sex with %d more people to level up.", req1);
		else if(joblevel1 == 5) format(templine, sizeof(templine), "You are the maximum level for this job.\t\t");
		if(joblevel2 < 5) format(templine2, sizeof(templine2), "You need to catch %d more fish to level up.", req2);
		else if(joblevel2 == 5) format(templine2, sizeof(templine2), "You are the maximum level for this job.");
		format(line6, sizeof(line6), "%s\t%s", templine, templine2);

		joblevel1 = GetPlayerJobLevel(playerid, JOB_DRUGDEALER), joblevel2 = GetPlayerJobLevel(playerid, JOB_SMUGGLER);
		format(line7, sizeof(line7), "{007BD0}Drug Dealer Skill Level: {B4B5B7}%d\t\t\t\t\t{007BD0}Drug Smuggler Level: {B4B5B7}%d", joblevel1, joblevel2);
		if(joblevel1 < 5) format(templine, sizeof(templine), "You need to sell drugs %d more times to level up.", GetPlayerJobSkillLevel(playerid, JOB_DRUGDEALER));
		else if(joblevel1 == 5) format(templine, sizeof(templine), "You are the maximum level for this job.\t\t");
		if(joblevel2 < 5) format(templine2, sizeof(templine2), "You need to deliver %d more drug crates to level up.", GetPlayerJobSkillLevel(playerid, JOB_SMUGGLER));
		else if(joblevel2 == 5) format(templine2, sizeof(templine2), "You are the maximum level for this job.");
		format(line8, sizeof(line8), "%s\t\t%s", templine, templine2);

		// Car Jacker & pizza
		jlevel = PlayerInfo[playerid][pJackSkill];
		if(jlevel >= 0 && jlevel <= 49)
		{
			joblevel1 = 1;
			req1 = 50 -jlevel;
		}
		else if(jlevel >= 50 && jlevel <= 99)
		{
			joblevel1 = 2;
			req1 = 100 -jlevel;
		}
		else if(jlevel >= 100 && jlevel <= 199)
		{
			joblevel1 = 3;
			req1 = 200 -jlevel;
		}
		else if(jlevel >= 200 && jlevel <= 399)
		{
			joblevel1 = 4;
			req1 = 400 -jlevel;
		}
		else if(jlevel >= 400)
		{
			joblevel1 = 5;
			req1 = 999;
		}
		joblevel2 = GetPlayerPizzaLevel(playerid);
		format(line9, sizeof(line9), "{007BD0}Car Jacker Skill Level: {B4B5B7}%d\t\t\t\t\t{007BD0}Pizza Skill Level: {B4B5B7}%d", joblevel1, joblevel2);
		if(joblevel1 < 5) format(templine, sizeof(templine), "You need to drop %d more cars at the crane to level up.", req1);
		else if(joblevel1 == 5) format(templine, sizeof(templine), "You are the maximum level for this job.\t\t");
		if(joblevel2 < 5) format(templine2, sizeof(templine2), "You need to deliver %d more pizzas to level up.", GetPlayerPizzaSkillLevel(playerid));
		else if(joblevel2 == 5) format(templine2, sizeof(templine2), "You are the maximum level for this job.");
		if(req1 <= 99) format(line10, sizeof(line10), "%s\t%s", templine, templine2);
		else if(req1 >= 100 && req1 <= 500) format(line10, sizeof(line10), "%s\t%s", templine, templine2);
		else if(req1 == 999) format(line10, sizeof(line10), "%s\t\t%s", templine, templine2);

		// Arms Dealer & Waste Collector
		joblevel1 = GetPlayerJobLevel(playerid, JOB_ARMS), joblevel2 = GetPlayerJobLevel(playerid, JOB_WASTECOLLECTOR);
		format(line11, sizeof(line11), "{007BD0}Arms Dealer Skill Level: {B4B5B7}%d\t\t\t\t\t{007BD0}Waste Collector Skill Level: {B4B5B7}%d", joblevel1, joblevel2);
		if(joblevel1 < 5) format(templine, sizeof(templine), "You need to craft %d more guns to level up.", GetPlayerJobSkillLevel(playerid, JOB_ARMS));
		else if(joblevel1 == 5) format(templine, sizeof(templine), "You are the maximum level for this job.");
		if(joblevel2 < 5) format(templine2, sizeof(templine2), "You need to pickup %d more trashbags level up.", GetPlayerJobSkillLevel(playerid, JOB_WASTECOLLECTOR));
		else if(joblevel2 == 5) format(templine2, sizeof(templine2), "You are the maximum level for this job.");
		req1 = GetPlayerJobSkillLevel(playerid, JOB_ARMS);
		if(req1 <= 99) format(line12, sizeof(line12), "%s\t\t\t%s", templine, templine2);
		else if(req1 >= 100 && req1 <= 500) format(line12, sizeof(line12), "%s\t%s", templine, templine2);
		else if(req1 == 999) format(line12, sizeof(line12), "%s\t\t%s", templine, templine2);

		//BUS DRIVER
		joblevel1 = GetPlayerJobLevel(playerid, JOB_BUS);
		format(line13, sizeof(line13), "{007BD0}Bus Skill Level: {B4B5B7}%d", joblevel1);
		if(joblevel1 < 5) format(templine, sizeof(templine), "You need to drive bus %d more times to level up.", GetPlayerJobSkillLevel(playerid, JOB_BUS));
		else if(joblevel1 == 5) format(templine, sizeof(templine), "You are the maximum level for this job.\t\t");
		format(line14, sizeof(line14), "%s", templine);
		
		format(tline, sizeof(tline), "%s\n%s\n\n%s\n%s\n\n%s\n%s\n\n%s\n%s\n\n%s\n%s\n\n%s\n%s\n%s\n%s", line1, line2, line3, line4, line5, line6, line7, line8, line9, line10, line11, line12, line13, line14);
		ShowPlayerDialog(playerid, DIALOG_SKILLS, DIALOG_STYLE_MSGBOX, "Skill Levels", tline, "Done", "Cancel");
		return 1;
	}
	if(strcmp(cmd, "/adjust", true) == 0)
	{
		if(PlayerInfo[playerid][pFMember] == 255)
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not in a Family.");
			return 1;
		}
		new family = PlayerInfo[playerid][pFMember];
		new isleader = 0;
		if(strcmp(FamilyInfo[family][FamilyLeader], PlayerICName(playerid), true) == 0 || strcmp(FamilyInfo[family][FamilyLeader], PlayerName(playerid), true) == 0)
		{
			isleader = 1;
		}
		if(PlayerInfo[playerid][pRank] >= 5)
		{
			new x_nr[64];
			x_nr = strtok(cmdtext, idx);
			if(!strlen(x_nr))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /adjust [name]");
				SendClientMessage(playerid, COLOR_GRAD1, "Available names: Name, MOTD, Invite, Uninvite, Rank, Safe, Rankname, Leadername");
				SendClientMessage(playerid, COLOR_GRAD1, "Note: To kick an offline member use /ofuninvite [full name(case sensitive)]");
				return 1;
			}
			if(strcmp(x_nr, "name", true) == 0)
			{
				if(isleader == 0)
					return SendClientMessage(playerid, COLOR_GREY, "Only the family leader is allowed to change the family name.");
				new length = strlen(cmdtext);
				while((idx < length) && (cmdtext[idx] <= ' '))
				{
					idx++;
				}
				new offset = idx;
				new result[24];
				while((idx < length) && ((idx - offset) < (sizeof(result) - 1)))
				{
					result[idx - offset] = cmdtext[idx];
					idx++;
				}
				result[idx - offset] = EOS;
				if(!strlen(result))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /adjust name [name]");
					return 1;
				}
				if(InvalidChar(result) == 1)
				{
					SendClientMessage(playerid, COLOR_GREY, "Family name cannot contain invalid characters.");
					return 1;
				}
				strmid(FamilyInfo[family][FamilyName], result, 0, strlen(result), 255);
				format(str, sizeof(str), "* You have adjusted your Family's Name to '%s'.", FamilyInfo[family][FamilyName]);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
				SaveFamilies();
			}
			else if(strcmp(x_nr, "rankname", true) == 0)
			{
				if(PlayerInfo[playerid][pRank] < 6)
					return SendClientMessage(playerid, COLOR_GREY, "You must be rank 6 to change ranknames.");
				tmp = strtok(cmdtext, idx);
				if(!strlen(tmp))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /adjust rankname [rank] [name]");
					return 1;
				}
				new rank = strvalEx(tmp);
				if(rank < 1 || rank > 6)
				{
					SendClientMessage(playerid, COLOR_GREY, "Rank can't be below 1 or above 6.");
					return 1;
				}
				new length = strlen(cmdtext);
				while((idx < length) && (cmdtext[idx] <= ' '))
				{
					idx++;
				}
				new offset = idx;
				new result[24];
				while((idx < length) && ((idx - offset) < (sizeof(result) - 1)))
				{
					result[idx - offset] = cmdtext[idx];
					idx++;
				}
				result[idx - offset] = EOS;
				if(!strlen(result))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /adjust rankname [rank] [name]");
					return 1;
				}
				if(InvalidChar(result) == 1)
				{
					SendClientMessage(playerid, COLOR_GREY, "ERROR: cannot contain invalid characters .");
					return 1;
				}
				strmid(FamilyRank[family][rank -1], result, 0, strlen(result), 255);
				format(str, sizeof(str), "* You have adjusted the name of Rank %d to \"%s\".", rank, FamilyRank[family][rank -1]);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
				SaveFamilies();
				return 1;
			}
			else if(strcmp(x_nr, "motd", true) == 0)
			{
				new length = strlen(cmdtext);
				while((idx < length) && (cmdtext[idx] <= ' '))
				{
					idx++;
				}
				new offset = idx;
				new result[64];
				while((idx < length) && ((idx - offset) < (sizeof(result) - 1)))
				{
					result[idx - offset] = cmdtext[idx];
					idx++;
				}
				result[idx - offset] = EOS;
				if(!strlen(result))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /adjust MOTD [MOTD Text]");
					return 1;
				}
				if(InvalidChar(result) == 1)
				{
					SendClientMessage(playerid, COLOR_GREY, "ERROR: cannot contain invalid characters .");
					return 1;
				}
				strmid(FamilyInfo[family][FamilyMOTD], result, 0, strlen(result), 255);
				format(str, sizeof(str), "* You have adjusted your Family's MOTD to \"%s\".", FamilyInfo[family][FamilyMOTD]);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
				SaveFamilies();
			}
			else if(strcmp(x_nr, "safe", true) == 0)
			{
				x_nr = strtok(cmdtext, idx);
				if(!strlen(x_nr))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /adjust safe [confirm]");
					if(FamilyInfo[family][FamilySafe] != 0)
					{
						SendClientMessage(playerid, COLOR_GREY, "Adjusting the position of your Family's Safe will cost $50,000.");
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "Purchasing a Safe for your Family will cost $50,000.");
						SendClientMessage(playerid, COLOR_WHITE, "HINT: Purchasing a safe will give your family the ability to store/share drugs, materials, and guns.");
					}
					return 1;
				}
				if(strcmp(x_nr, "confirm", true) == 0)
				{
					if(PlayerInfo[playerid][pCash] < 49999)
					{
						SendClientMessage(playerid, COLOR_GRAD1, "You don't have that much money.");
						return 1;
					}
					if(FamilyInfo[family][FamilySafe] != 0)
					{
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have adjusted the position of your Family's Safe.");
					}
					else
					{
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have purchased a Safe.");
					}
					GiveMoney(playerid, -50000);
					new randworld = GetPlayerVirtualWorld(playerid);
					FamilyInfo[family][FamilySafeWorld] = randworld;
					GetPlayerPos(playerid, FamilyInfo[family][FamilySafePos][0], FamilyInfo[family][FamilySafePos][1], FamilyInfo[family][FamilySafePos][2]);
					DestroyDynamicPickup(FamilyInfo[family][PickupID]);
					FamilyInfo[family][PickupID] = CreateDynamicPickup(1210, 23, FamilyInfo[family][FamilySafePos][0], FamilyInfo[family][FamilySafePos][1], FamilyInfo[family][FamilySafePos][2], randworld, -1, -1, 100.0);
					FamilyInfo[family][FamilySafe] = 1;
					SaveFamilies();
					LogSafe(playerid, "Moved Safe");
					return 1;
				}
			}
			else if(strcmp(x_nr, "invite", true) == 0)
			{
				x_nr = strtok(cmdtext, idx);
				if(!strlen(x_nr))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /adjust invite [playerid/PartOfName]");
					return 1;
				}
				giveplayerid = ReturnUser(x_nr);
				if(IsPlayerConnected(giveplayerid))
				{
					if(giveplayerid != INVALID_PLAYER_ID)
					{
						if(PlayerInfo[giveplayerid][pMember] > 0 || PlayerInfo[giveplayerid][pLeader] > 0 || PlayerInfo[giveplayerid][pFMember] != 255)
						{
							SendClientMessage(playerid, COLOR_GREY, "That player is already in a Faction / Family.");
							return 1;
						}
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
						format(str, sizeof(str), "* You've invited %s to join %s.", giveplayer, FamilyInfo[family][FamilyName]);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* %s has invited you to join %s, (type /accept family) to accept.", sendername, FamilyInfo[family][FamilyName]);
						SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
						FamilyOffer[giveplayerid] = playerid;
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
					return 1;
				}
			}
			else if(strcmp(x_nr, "uninvite", true) == 0)
			{
				tmp = strtok(cmdtext, idx);
				if(!strlen(x_nr))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /adjust uninvite [playerid/PartOfName]");
					return 1;
				}
				giveplayerid = ReturnUser(tmp);
				if(!strlen(x_nr))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /adjust uninvite [playerid/PartOfName]");
					return 1;
				}
				if(IsPlayerConnected(giveplayerid))
				{
					if(giveplayerid != INVALID_PLAYER_ID)
					{
						if(PlayerInfo[giveplayerid][pFMember] != family)
						{
							SendClientMessage(playerid, COLOR_GREY, "That player is not in your Family.");
							return 1;
						}
						if(PlayerInfo[giveplayerid][pRank] > PlayerInfo[playerid][pRank] && !strmatch(FamilyInfo[family][FamilyLeader], PlayerICName(playerid)))
						{
							SendClientMessage(playerid, COLOR_GREY, "That player is a higher rank & you can't kick them unless you're the leader.");
							return 1;
						}
						if(playerid == giveplayerid) return SendClientMessage(playerid, COLOR_GREY, "You can't uninvite yourself, use /quitfamily.");
						new fam = PlayerInfo[giveplayerid][pFMember];
						if(strmatch(FamilyInfo[fam][FamilyLeader], PlayerICName(giveplayerid)))
						{
							strmid(FamilyInfo[fam][FamilyLeader], "None", 0, 100, 255);
							FamilyInfo[fam][FamilyMembers] -= 1;
						}
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
						format(str, sizeof(str), "* You have kicked %s out of your Family.", giveplayer);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* Family Leader %s has kicked you out of the Family.", sendername);
						SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
						PlayerInfo[giveplayerid][pFMember] = 255;
						PlayerInfo[giveplayerid][pRank] = 0;
						FamilyInfo[family][FamilyMembers] --;
						return 1;
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
					return 1;
				}
			}
			else if(strcmp(x_nr, "rank", true) == 0)
			{
				tmp = strtok(cmdtext, idx);
				if(!strlen(x_nr))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /adjust rank [rank] [playerid/PartOfName]");
					return 1;
				}
				new rank = strvalEx(tmp);
				if(!strlen(tmp))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /adjust rank [rank] [playerid/PartOfName]");
					return 1;
				}
				if(rank < 1 || rank > 6)
				{
					SendClientMessage(playerid, COLOR_GREY, "Rank can't be below 1 or above 6.");
					return 1;
				}
				tmp = strtok(cmdtext, idx);
				if(!strlen(x_nr))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /adjust rank [rank] [playerid/PartOfName]");
					return 1;
				}
				giveplayerid = ReturnUser(tmp);
				if(IsPlayerConnected(giveplayerid))
				{
					if(giveplayerid != INVALID_PLAYER_ID)
					{
						if(PlayerInfo[giveplayerid][pFMember] != family)
						{
							SendClientMessage(playerid, COLOR_GREY, "That player is not in your family.");
							return 1;
						}
						if(playerid == giveplayerid)
						{
							SendClientMessage(playerid, COLOR_GREY, "You can't adjust your own rank.");
							return 1;
						}
						if(rank < 0 || rank > 4 && PlayerInfo[playerid][pRank] == 5 && isleader == 0)
						{
							SendClientMessage(playerid, COLOR_GREY, "Rank 5 can only give family members ranks ranging from 1 through 4.");
							return 1;
						}
						if(PlayerInfo[giveplayerid][pRank] >= PlayerInfo[playerid][pRank])
						{
							SendClientMessage(playerid, COLOR_GREY, "That player is a higher or the same rank.");
							return 1;
						}
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
						format(str, sizeof(str), "* You have given %s Rank %d.", giveplayer, rank);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* Family Leader %s has given you Rank %d.", sendername, rank);
						SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
						PlayerInfo[giveplayerid][pRank] = rank;
						return 1;
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
					return 1;
				}
			}
			else if(strcmp(x_nr, "leadername", true) == 0)
			{
				if(isleader == 0) return SendClientMessage(playerid, COLOR_GREY, "Only the family leader is allowed to change this setting.");
				
				if(FamilyInfo[family][FamilyLeaderHide])
				{
					FamilyInfo[family][FamilyLeaderHide] = 0;	
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "You will no longer be hidden on /families.");
				}
				else
				{
					FamilyInfo[family][FamilyLeaderHide] = 1;
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "You are now hidden on /families.");
				}	
				SaveFamilies();
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Invalid adjust name.");
				return 1;
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "Your rank is not high enough.");
			return 1;
		}
		return 1;
	}
	if(strcmp(cmd, "/fcreate", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] >= 1337 || PlayerInfo[playerid][pGangDirector] > 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fcreate [family] [playerid/PartOfName]");
				return 1;
			}
			new family = strvalEx(tmp);
			if(family < 1 || family > 20)
			{
				SendClientMessage(playerid, COLOR_GREY, "Family can't be below 1 or above 20.");
				return 1;
			}
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fcreate [family] [playerid/PartOfName]");
				return 1;
			}
			giveplayerid = ReturnUser(tmp);
			if(IsPlayerNPC(giveplayerid)) return 1;
			if(IsPlayerConnected(giveplayerid))
			{
				if(giveplayerid != INVALID_PLAYER_ID)
				{
					family -= 1;
					if(FamilyInfo[family][FamilyTaken] == 1)
					{
						SendClientMessage(playerid, COLOR_GREY, "That Family is already taken.");
						return 1;
					}
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
					format(str, sizeof(str), "* You've made %s the Leader of a Family.", giveplayer);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* Admin %s has made you the Leader of a Family.", sendername);
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "%s", giveplayer);
					strmid(FamilyInfo[family][FamilyLeader], str, 0, strlen(str), 255);
					FamilyInfo[family][FamilyMembers] ++;
					FamilyInfo[family][FamilyTaken] = 1;
					PlayerInfo[giveplayerid][pFMember] = family;
					PlayerInfo[giveplayerid][pModel] = FamilyInfo[family][FamilySkin1];
					SetPlayerSkin(giveplayerid, FamilyInfo[family][FamilySkin1]);
					PlayerInfo[giveplayerid][pClothes] = 0;
					PlayerInfo[giveplayerid][pRank] = 6;
					PlayerInfo[giveplayerid][pMember] = 0;
					PlayerInfo[giveplayerid][pLeader] = 0;
					SaveFamilies();
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
				return 1;
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "{FF0000}You're not authorized to use that command.");
			return 1;
		}
		return 1;
	}
	if(strcmp(cmd, "/fdelete", true) == 0)
	{
		if(PlayerInfo[playerid][pAdmin] >= 1337 || PlayerInfo[playerid][pGangDirector] > 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fdelete [family]");
				return 1;
			}
			new family = strvalEx(tmp);
			if(family < 1 || family > 20)
			{
				SendClientMessage(playerid, COLOR_GREY, "Family can't be below 1 or above 20.");
				return 1;
			}
			family -= 1;
			if(FamilyInfo[family][FamilyTaken] != 1)
			{
				SendClientMessage(playerid, COLOR_GREY, "That Family isn't taken.");
				return 1;
			}
			if(UseAdmCmdTimer[playerid] > 2)
			{
				format(str, sizeof(str), "{FF6347}SERVER: %s was banned, reason: (autoban) /fdelete spam.", PlayerOOCName(playerid));
				SendClientMessageToAll(COLOR_WHITE, str);

				new reason[12];
				format(reason, sizeof(reason), "/fdelete spam.");
				BanAccount(playerid, INVALID_PLAYER_ID, reason); //log before banning the account
				return 1;
			}
			format(str, sizeof(str), "Family %d has been deleted.", family +1);
			SendClientMessage(playerid, COLOR_GREY, str);
			ClearFamily(family);
			UseAdmCmdTimer[playerid]++;
			SetTimerEx("Timer_UseAdmCmd", 3 *1000, 0, "i", playerid);
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "{FF0000}You're not authorized to use that command.");
			return 1;
		}
		return 1;
	}
	if(strcmp(cmd, "/divorce", true) ==0)
	{
		if(PlayerInfo[playerid][pMarried] < 1)
		{
			SendClientMessage(playerid, COLOR_GREY, "You aren't Married.");
			return 1;
		}
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /divorce [Playerid/PartOfName]");
			return 1;
		}
		giveplayerid = ReturnUser(tmp);
		if(IsPlayerConnected(giveplayerid))
		{
			if(giveplayerid != INVALID_PLAYER_ID)
			{
				if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
				{
					if(giveplayerid == playerid)
					{
						SendClientMessage(playerid, COLOR_GREY, "You cannot offer a divorce to yourself.");
						return 1;
					}
					new dstring[MAX_PLAYER_NAME];
					new wstring[MAX_PLAYER_NAME];
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
					format(str, sizeof(str), "%s", giveplayer);
					strmid(wstring, str, 0, strlen(str), 255);
					format(str, sizeof(str), "%s", PlayerInfo[playerid][pMarriedTo]);
					strmid(dstring, str, 0, strlen(str), 255);
					if(strcmp(dstring, wstring, true) == 0)
					{
						format(str, sizeof(str), "* You've sent Divorce Papers to %s.", giveplayer);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* %s just sent you their Divorce Papers (type /accept divorce) to accept.", sendername);
						SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
						DivorceOffer[giveplayerid] = playerid;
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "That player is not Married to you.");
						return 1;
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
					return 1;
				}
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
			return 1;
		}
		return 1;
	}
	if(strcmp(cmd, "/propose", true) ==0)
	{
		if(PlayerInfo[playerid][pMarried] > 0)
		{
			SendClientMessage(playerid, COLOR_GREY, "You are already Married.");
			return 1;
		}
		if(PlayerInfo[playerid][pCash] < 100000)
		{
			SendClientMessage(playerid, COLOR_GREY, "The Marriage & Reception costs $100,000.");
			return 1;
		}
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /propose [Playerid/PartOfName]");
			return 1;
		}
		giveplayerid = ReturnUser(tmp);
		if(IsPlayerConnected(giveplayerid))
		{
			if(giveplayerid != INVALID_PLAYER_ID)
			{
				if(PlayerInfo[giveplayerid][pMarried] > 0)
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is already Married.");
					return 1;
				}
				if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
				{
					if(giveplayerid == playerid)
					{
						SendClientMessage(playerid, COLOR_GREY, "You cannot offer to propose to yourself.");
						return 1;
					}
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
					format(str, sizeof(str), "* You proposed to %s.", giveplayer);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* %s just proposed to you (type /accept marriage) to accept.", sendername);
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					ProposeOffer[giveplayerid] = playerid;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
					return 1;
				}
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
			return 1;
		}
		return 1;
	}
	if(strcmp(cmd, "/witness", true) ==0)
	{
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /witness [Playerid/PartOfName]");
			return 1;
		}
		giveplayerid = ReturnUser(tmp);
		if(IsPlayerConnected(giveplayerid))
		{
			if(giveplayerid != INVALID_PLAYER_ID)
			{
				if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
				{
					if(giveplayerid == playerid)
					{
						SendClientMessage(playerid, COLOR_GREY, "You cannot offer to be your own witness.");
						return 1;
					}
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
					format(str, sizeof(str), "* You requested %s to be your Marriage Witness.", giveplayer);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* %s just requested you to be their Marriage Witness (type /accept witness) to accept.", sendername);
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					MarryWitnessOffer[giveplayerid] = playerid;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
					return 1;
				}
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
			return 1;
		}
		return 1;
	}
	if(strcmp(cmd, "/irc", true) ==0)
	{
		new x_nr[64];
		x_nr = strtok(cmdtext, idx);
		if(!strlen(x_nr))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} (/irc join [channelnr] or /irc join [channelnr] [password])  (/irc Leave)  (/irc Admins) (/irc members)");
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /irc [name] [channelnr]");
			SendClientMessage(playerid, COLOR_GRAD1, "Available names: MOTD, Password, NeedPass, Lock, Kick, Status");
			return 1;
		}
		if(strcmp(x_nr, "join", true) == 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /irc join [channelnr] or /irc join [channelnr] [password]");
				return 1;
			}
			new channel = strvalEx(tmp);
			if(channel < 1 || channel > 10)
			{
				SendClientMessage(playerid, COLOR_GREY, "Channel Number can't be below 1 or above 10.");
				return 1;
			}
			channel -= 1;
			if(IRCInfo[channel][iLock] == 0)
			{
				if(IRCInfo[channel][iNeedPass] == 0)
				{
					JoinChannelNr(playerid, channel);
				}
				else
				{
					tmp = strtok(cmdtext, idx);
					if(!strlen(tmp))
					{
						SendClientMessage(playerid, COLOR_WHITE, "There's a password required to join this Channel");
						SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /irc join [channelnr] [password]");
						return 1;
					}
					JoinChannel(playerid, channel, tmp);
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "That Channel is Locked, please choose a different one.");
				return 1;
			}
		}
		else if(strcmp(x_nr, "status", true) == 0)
		{
			for(new i = 0; i < sizeof(IRCInfo); i++)
			{
				format(str, sizeof(str), "Channel %d: %d Players Connected.", i + 1, IRCInfo[i][iPlayers]);
				SendClientMessage(playerid, COLOR_WHITE, str);
			}
			return 1;
		}
		else if(strcmp(x_nr, "password", true) == 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /irc password [channelnr] [password]");
				return 1;
			}
			new channel = strvalEx(tmp);
			if(channel < 1 || channel > 10)
			{
				SendClientMessage(playerid, COLOR_GREY, "Channel Number can't be below 1 or above 10.");
				return 1;
			}
			channel -= 1;
			new wstring[128];
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			format(str, sizeof(str), "%s", sendername);
			strmid(wstring, str, 0, strlen(str), 255);
			if(strcmp(IRCInfo[channel][iAdmin], wstring, true) == 0)
			{
				tmp = strtok(cmdtext, idx);
				if(!strlen(tmp))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /irc password [channelnr] [password]");
					return 1;
				}
				strmid(IRCInfo[channel][iPassword], tmp, 0, strlen(tmp), 255);
				format(str, sizeof(str), "You've changed the IRC Channel's Password to: %s.", IRCInfo[channel][iPassword]);
				SendClientMessage(playerid, COLOR_YELLOW, str);
				SaveIRC();
				return 1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not the Admin of that Channel.");
				return 1;
			}
		}
		else if(strcmp(x_nr, "needpass", true) == 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /irc needpass [channelnr]");
				return 1;
			}
			new channel = strvalEx(tmp);
			if(channel < 1 || channel > 10)
			{
				SendClientMessage(playerid, COLOR_GREY, "Channel Number can't be below 1 or above 10.");
				return 1;
			}
			channel -= 1;
			new wstring[128];
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			format(str, sizeof(str), "%s", sendername);
			strmid(wstring, str, 0, strlen(str), 255);
			if(strcmp(IRCInfo[channel][iAdmin], wstring, true) == 0)
			{
				if(IRCInfo[channel][iNeedPass] != 0)
				{
					IRCInfo[channel][iNeedPass] = 0;
					SendClientMessage(playerid, COLOR_YELLOW, "Players won't have to fill in a password in order to join the IRC Channel now.");
				}
				else
				{
					IRCInfo[channel][iNeedPass] = 1;
					SendClientMessage(playerid, COLOR_YELLOW, "Players must fill in a password in order to join the IRC Channel now.");
				}
				SaveIRC();
				return 1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not the Admin of that Channel.");
				return 1;
			}
		}
		else if(strcmp(x_nr, "lock", true) == 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /irc lock [channelnr]");
				return 1;
			}
			new channel = strvalEx(tmp);
			if(channel < 1 || channel > 10)
			{
				SendClientMessage(playerid, COLOR_GREY, "Channel Number can't be below 1 or above 10.");
				return 1;
			}
			channel -= 1;
			new wstring[128];
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			format(str, sizeof(str), "%s", sendername);
			strmid(wstring, str, 0, strlen(str), 255);
			if(strcmp(IRCInfo[channel][iAdmin], wstring, true) == 0)
			{
				if(IRCInfo[channel][iLock] != 0)
				{
					IRCInfo[channel][iLock] = 0;
					SendClientMessage(playerid, COLOR_YELLOW, "You've unlocked the IRC Channel.");
				}
				else
				{
					IRCInfo[channel][iLock] = 1;
					SendClientMessage(playerid, COLOR_YELLOW, "You've locked the IRC Channel.");
				}
				SaveIRC();
				return 1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not the Admin of that Channel.");
				return 1;
			}
		}
		else if(strcmp(x_nr, "motd", true) == 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /irc motd [channelnr] [motdtext]");
				return 1;
			}
			new channel = strvalEx(tmp);
			if(channel < 1 || channel > 10)
			{
				SendClientMessage(playerid, COLOR_GREY, "Channel Number can't be below 1 or above 10.");
				return 1;
			}
			channel -= 1;
			new wstring[128];
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			format(str, sizeof(str), "%s", sendername);
			strmid(wstring, str, 0, strlen(str), 255);
			if(strcmp(IRCInfo[channel][iAdmin], wstring, true) == 0)
			{
				new length = strlen(cmdtext);
				while((idx < length) && (cmdtext[idx] <= ' '))
				{
					idx++;
				}
				new offset = idx;
				new result[64];
				while((idx < length) && ((idx - offset) < (sizeof(result) - 1)))
				{
					result[idx - offset] = cmdtext[idx];
					idx++;
				}
				result[idx - offset] = EOS;
				if(!strlen(result))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /irc motd [motdtext]");
					return 1;
				}
				strmid(IRCInfo[channel][iMOTD], result, 0, strlen(result), 255);
				SendClientMessage(playerid, COLOR_YELLOW, "You've adjusted the IRC Channel's MOTD Text.");
				SaveIRC();
				return 1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not the Admin of that Channel.");
				return 1;
			}
		}
		else if(strcmp(x_nr, "leave", true) == 0)
		{
			if(PlayersChannel[playerid] < 999)
			{
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				format(str, sizeof(str), "* %s has left the Channel.", sendername);
				SendIRCMessage(PlayersChannel[playerid], COLOR_GREEN, str);
				IRCInfo[PlayersChannel[playerid]][iPlayers] --;
				PlayersChannel[playerid] = 999;
				return 1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not in an IRC Channel.");
				return 1;
			}
		}
		else if(strcmp(x_nr, "admins", true) == 0)
		{
			for(new i = 0; i < sizeof(IRCInfo); i++)
			{
				format(str, sizeof(str), "Channel %d: %s.", i + 1, IRCInfo[i][iAdmin]);
				SendClientMessage(playerid, COLOR_WHITE, str);
			}
			return 1;
		}
		else if(strcmp(x_nr, "kick", true) == 0)
		{
			if(PlayersChannel[playerid] == 999)
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not in an IRC Channel.");
				return 1;
			}
			new wstring[128];
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			format(str, sizeof(str), "%s", sendername);
			strmid(wstring, str, 0, strlen(str), 255);
			if(strcmp(IRCInfo[PlayersChannel[playerid]][iAdmin], wstring, true) == 0)
			{
				tmp = strtok(cmdtext, idx);
				if(!strlen(tmp))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /irc kick [playerid/PartOfName]");
					return 1;
				}
				giveplayerid = ReturnUser(tmp);
				if(IsPlayerConnected(giveplayerid))
				{
					if(giveplayerid != INVALID_PLAYER_ID)
					{
						if(PlayersChannel[giveplayerid] == PlayersChannel[playerid])
						{
							strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
							strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
							format(str, sizeof(str), "* You've kicked %s out of your IRC Channel.", giveplayer);
							SendClientMessage(playerid, COLOR_YELLOW, str);
							format(str, sizeof(str), "* You've been kicked out of the IRC Channel by Channel Admin: %s.", sendername);
							SendClientMessage(giveplayerid, COLOR_YELLOW, str);
							format(str, sizeof(str), "* %s has left the Channel (Kicked).", giveplayer);
							SendIRCMessage(PlayersChannel[playerid], COLOR_GREEN, str);
							IRCInfo[PlayersChannel[giveplayerid]][iPlayers] --;
							PlayersChannel[giveplayerid] = 999;
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "That player is not in your IRC Channel.");
							return 1;
						}
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
					return 1;
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not the Admin of the Channel.");
				return 1;
			}
		}
		else if(strcmp(x_nr, "members", true) == 0)
		{
			if(PlayersChannel[playerid] == 999)
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not in an IRC Channel.");
				return 1;
			}
			format(str, sizeof(str), "========| Players in IRC #%i |========", (PlayersChannel[playerid]-1));
			SendClientMessage(playerid, COLOR_WHITE, str);
			foreach( new i: Player )
			{
				if(PlayersChannel[i] == PlayersChannel[playerid])
				{
					format(str, sizeof(str), "- %s", PlayerName(i));
					SendClientMessage(playerid, COLOR_WHITE, str);
				}
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "Invalid IRC Channel Number! ");
			return 1;
		}
		return 1;
	}
	if(strcmp(cmd, "/channel", true) == 0)
	{
		if(PlayerInfo[playerid][pWT] != 1)
		{
			SendClientMessage(playerid, COLOR_GREY, "You don`t have a Walkie Talkie.");
			return 1;
		}
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /channel [channel number]");
			return 1;
		}
		new channel = strvalEx(tmp);
		if(channel < 1 || channel > 999999)
			return SendClientMessage(playerid, COLOR_GRAD1, "The channel number must not be below 1 or above 999999 .");
		PlayerInfo[playerid][pWTc] = channel;
		format(str, sizeof(str), "* You have set your Walkie Talkie to channel to %d.", channel);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		return 1;
	}
	if(strcmp(cmd, "/settax", true) == 0)
	{
		if(PlayerInfo[playerid][pMember] != 6) return SendClientMessage(playerid, COLOR_GREY, "You are not a part of the government.");
		if(PlayerInfo[playerid][pRank] < 5) return SendClientMessage(playerid, COLOR_GREY, "Only R5+ can use this.");
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /settax [percent]");
			return 1;
		}
		moneys = strvalEx(tmp);
		if(moneys < 1 || moneys > 50)
		{
			SendClientMessage(playerid, COLOR_GREY, "Tax rate may not be below 1 or above 50.");
			return 1;
		}
		Tax = moneys;
		SaveStuff();
		format(str, sizeof(str), "* The Income Tax has been set to %d percent per paycheck.", Tax);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		return 1;
	}
	if(strcmp(cmd, "/change", true) == 0)
	{
		if(!IsPlayerInRangeOfPoint(playerid, 2, 754.5243, -39.7095, 1000.5859) && !IsPlayerInRangeOfPoint(playerid, 2, 2200.5842, -1970.2686, 13.7841))
		{
			SendClientMessage(playerid, COLOR_GRAD2, "You are not at the locker.");
			return 1;
		}
		strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
		if(IsPlayerInRangeOfPoint(playerid, 2, 754.5243, -39.7095, 1000.5859))
		{
			if(PlayerInfo[playerid][pClothes] != 0)
			{
				SetPlayerSkin(playerid, PlayerInfo[playerid][pModel]);
				PlayerInfo[playerid][pClothes] = 0;
				PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
				ApplyAnimationEx(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
				format(str, sizeof(str), "* %s switches back to their clothes.", sendername);
				ProxDetector(20.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			else
			{
				new uniforms[2];
				uniforms[0] = 203;
				uniforms[1] = 204;
				new rand = random(sizeof(uniforms));
				SetPlayerSkin(playerid, uniforms[rand]);
				PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);
				ApplyAnimationEx(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
				PlayerInfo[playerid][pClothes] = uniforms[rand];
				format(str, sizeof(str), "* %s switches to a karate uniform.", sendername);
				ProxDetector(20.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
		}
		return 1;
	}
	if(strcmp(cmd, "/lotto", true) == 0)
	{
		if(PlayerInfo[playerid][pAdminJailed]) return SendClientMessage(playerid, COLOR_WHITE, "{FF0000}Error{FFFFFF}: You cannot participate in the lottery while in OOC prison.");
		if(PlayerInfo[playerid][pLottoNr] > 0)
		{
			format(str, sizeof(str), "Your lotto number is %d.", PlayerInfo[playerid][pLottoNr]);
			SendClientMessage(playerid, COLOR_GRAD3, str);
			return 1;
		}
		if(PlayerInfo[playerid][pCash] < 1500)
		{
			SendClientMessage(playerid, COLOR_GREY, "You need $1500 for a Lottery Ticket.");
			return 1;
		}
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_WHITE, "{00BFFF}Usage:{FFFFFF} /lotto [number]");
			return 1;
		}
		new lottonr = strvalEx(tmp);
		if(lottonr < 1 || lottonr > 80)
		{
			SendClientMessage(playerid, COLOR_GREY, "Your Lotto Number can't be below 1 or above 80.");
			return 1;
		}
		format(str, sizeof(str), "* You bought a Lottery Ticket with number: %d.", lottonr);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		GiveMoney(playerid, -1500);
		PlayerInfo[playerid][pLottoNr] = lottonr;
		return 1;
	}
	if(strcmp(cmd, "/get", true) == 0)
	{
		new x_nr[64];
		x_nr = strtok(cmdtext, idx);
		if(!strlen(x_nr))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /get [name]");
			SendClientMessage(playerid, COLOR_GRAD1, "Available names: Weapon, Pot, Crack, Materials, Kevlar, Briefcase");
			return 1;
		}
		if(strcmp(x_nr, "briefcase", true) == 0)
		{
			if(PlayerInfo[playerid][pSuitcase] == 1) return SendClientMessage(playerid, COLOR_GRAD1, "You already have a briefcase.");
			new f = 100 +1;
			for(new a = 0; a < sizeof(sObjCoords); a++)
			{
				if(IsPlayerInRangeOfPoint(playerid, 3.0, sObjCoords[a][0], sObjCoords[a][1], sObjCoords[a][2]))
				{
					f = a;
					break;
				}
			}
			if(f > 100) return SendClientMessage(playerid, COLOR_GREY, "You are not near any briefcases nearby.");
			if(GetPlayerState(playerid) != 1) return SendClientMessage(playerid, COLOR_GREY, "You must be on foot.");
			else
			{
				PlayerInfo[playerid][pSuitcase] = 1;
				PlayerInfo[playerid][sPot] += sObjectID[f][0];
				PlayerInfo[playerid][sMats] += sObjectID[f][3];
				PlayerInfo[playerid][sCrack] += sObjectID[f][4];
				PlayerInfo[playerid][sMoney] += sObjectID[f][5];
				sObjCoords[f][0] = 0.0;
				sObjCoords[f][1] = 0.0;
				sObjCoords[f][2] = 0.0;
				sObjectID[f][0] = 0;
				DestroyDynamicObject(sobject[f]);
				sobject[f] = 0;
				SetPlayerAttachedObject(playerid, 0, 1210, 6, 0.3, 0.1, 0.0, 0.0, 270.0, 0.0);
				WearingSuitcase[playerid] = 1;
				SendClientMessage(playerid, 0x33AA3300, "You picked up a briefcase");
				format(str, sizeof(str), "* %s has picked up a briefcase.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
		}
		if(strcmp(x_nr, "weapon", true) == 0)
		{
			if(inPaintball[playerid] > 0) return SendClientMessage(playerid, COLOR_GRAD1, "You cannot use this at this time.");
			if(PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can get a weapon.");
			if(GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK) return SendClientMessage(playerid, COLOR_GREY, "You cannot use this at this time");

			new f = 100 +1;
			for(new a = 0; a < sizeof(ObjCoords); a++)
			{
				if(IsPlayerInRangeOfPoint(playerid, 3.0, ObjCoords[a][0], ObjCoords[a][1], ObjCoords[a][2]))
				{
					f = a;
					break;
				}
			}
			if(f > 100) return SendClientMessage(playerid, COLOR_GREY, "You are not near the weapon which you can pick up.");
			if(GetPlayerState(playerid) != 1) return SendClientMessage(playerid, COLOR_GREY, "You must be on foot.");
			else
			{
				new gunname[25];
				new buffer[100];
				ObjCoords[f][0] = 0.0;
				ObjCoords[f][1] = 0.0;
				ObjCoords[f][2] = 0.0;
				if(object[f]) { DestroyDynamicObjectEx(object[f]); }
				object[f] = 0;
				GivePlayerGun(playerid, wObjectID[f][0]);
				GetWeaponName(wObjectID[f][0], gunname, sizeof(gunname));
				format(buffer, sizeof(buffer), "You picked up a %s", gunname);
				SendClientMessage(playerid, 0x33AA3300, buffer);
				format(str, sizeof(str), "* %s has picked up a %s.", PlayerICName(playerid), gunname);
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
		}
		if(strcmp(x_nr, "kevlar", true) == 0)
		{
			new f = 100 +1;
			for(new a = 0; a < sizeof(ObjCoords); a++)
			{
				if(IsPlayerInRangeOfPoint(playerid, 3.0, kObjCoords[a][0], kObjCoords[a][1], kObjCoords[a][2]))
				{
					f = a;
					break;
				}
			}
			if(f > 100) return SendClientMessage(playerid, COLOR_GREY, "You are not near any kevlar which you can pick up.");
			if(GetPlayerState(playerid) != 1) return SendClientMessage(playerid, COLOR_GREY, "You must be on foot.");
			kObjCoords[f][0] = 0.0;
			kObjCoords[f][1] = 0.0;
			kObjCoords[f][2] = 0.0;
			if(kobject[f]) { DestroyDynamicObjectEx(kobject[f]); }
			kobject[f] = 0;
			SetPlayerArmour(playerid, kObjectVal[f][0]);
			SendClientMessage(playerid, 0x33AA3300, "You picked up a kevlar");
			format(str, sizeof(str), "* %s has picked up a kevlar.", PlayerICName(playerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		}

		if(strcmp(x_nr, "materials", true) == 0)
		{
			new f = 100 +1;
			for(new a = 0; a < sizeof(mObjCoords); a++)
			{
				if(IsPlayerInRangeOfPoint(playerid, 3.0, mObjCoords[a][0], mObjCoords[a][1], mObjCoords[a][2]))
				{
					f = a;
					break;
				}
			}
			if(f > 100) return SendClientMessage(playerid, COLOR_GREY, "You are not near any materials which you can pick up.");
			if(GetPlayerState(playerid) != 1) return SendClientMessage(playerid, COLOR_GREY, "You must be on foot.");
			else
			{
				mObjCoords[f][0] = 0.0;
				mObjCoords[f][1] = 0.0;
				mObjCoords[f][2] = 0.0;
				if(mobject[f]) { DestroyDynamicObjectEx(mobject[f]); }
				mobject[f] = 0;
				PlayerInfo[playerid][pMats] += mObjectID[f][0];
				SendClientMessage(playerid, 0x33AA3300, "You picked up some materials");
				format(str, sizeof(str), "* %s has picked up some materials.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
		}
		return 1;
	}
	if(strcmp(cmd, "/drop", true) == 0)
	{
		if(IsPlayerInAnyVehicle(playerid))
			return SendClientMessage(playerid, COLOR_GREY, "You cannot do that while in a car!");
		
		if(pDeathState[playerid] != PLAYER_STATE_NORMAL)
			return SendClientMessage(playerid, COLOR_GREY, "You can't use this at the moment.");

		if(IsAtEvent[playerid] == 1)
			return SendClientMessage(playerid, COLOR_GREY, "You can't use this in events.");
		
		new curr_Time = gettime();

		if((curr_Time - CanDrop[playerid]) <= 2)
		{		
			return 1;
		}

		CanDrop[playerid] = curr_Time;

		new x_nr[64];
		x_nr = strtok(cmdtext, idx);
		if(!strlen(x_nr))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /drop [name]");
			SendClientMessage(playerid, COLOR_GRAD1, "Available names: Weapon, Materials, Packages, Crates, Tikis, Seeds, Meal, Kevlar, Boombox");
			SendClientMessage(playerid, COLOR_GRAD1, "Available names: Cellphone, Products, Screwdrivers, Ropes, Gags, Jerrycan, Trash, Briefcase");
			return 1;
		}
		if(strcmp(x_nr, "Jerrycan", true) == 0)
		{
			if(PlayerInfo[playerid][pJerrycan] < 1)
				return SendClientMessage(playerid, COLOR_GREY, "You do not have a Jerrycan to drop.");
			format(str, sizeof(str), "* %s has dropped a Jerrycan.", PlayerICName(playerid));
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerInfo[playerid][pJerrycan] = 0;
			return 1;
		}
		else if(strcmp(x_nr, "briefcase", true) == 0)
		{
			if(PlayerInfo[playerid][pSuitcase] > 0)
			{
				new f = 100 +1;
				for(new a = 0; a < sizeof(sObjCoords); a++)
				{
					if(sObjCoords[a][0] == 0.0)
					{
						f = a;
						break;
					}
				}
				sObjectID[f][0] = PlayerInfo[playerid][sPot];
				sObjectID[f][3] = PlayerInfo[playerid][sMats];
				sObjectID[f][4] = PlayerInfo[playerid][sCrack];
				sObjectID[f][5] = PlayerInfo[playerid][sMoney];
				PlayerInfo[playerid][pSuitcase] = 0;
				PlayerInfo[playerid][sMoney] = 0;
				PlayerInfo[playerid][sMats] = 0;
				PlayerInfo[playerid][sCrack] = 0;
				PlayerInfo[playerid][sPot] = 0;
				WearingSuitcase[playerid] = 0;
				GetPlayerPos(playerid, sObjCoords[f][0], sObjCoords[f][1], sObjCoords[f][2]);
				sobject[f] = CreateDynamicObject(1210, sObjCoords[f][0], sObjCoords[f][1], sObjCoords[f][2] -1, 270.0, 270.0, 120.0, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), -1, 100.0);
				Streamer_Update(playerid);
				SetTimerEx("DeleteSuitcase", 30000, 0, "dd", sobject[f], f);
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				RemovePlayerAttachedObject(playerid, 9);
				format(str, sizeof(str), "* %s has dropped their briefcase (/get to pick it up).", sendername);
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				return 1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "You don't have a Briefcase to throw away.");
				return 1;
			}
		}
		if(strcmp(x_nr, "weapon", true) == 0)
		{
			if(GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK) return SendClientMessage(playerid, COLOR_GREY, "You cannot use this at this time");
			new WeaponName[65];
			if(PlayerInfo[playerid][pMember] != 0) //faction drop
			{
				new gunid = GetPlayerWeapon(playerid);
				if(gunid)
				{
					if(gunid == 45)
					{
						WeaponName = "Infrared Goggles.";
					}
					else
					{
						GetWeaponName(gunid, WeaponName, 64);
					}
					format(str, sizeof(str), "* %s has dropped a %s.", PlayerICName(playerid), WeaponName);
					ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					TakePlayerWeapon(playerid, GetPlayerWeapon(playerid));
					SetPlayerArmedWeapon(playerid, 0);
				}
				else return SendClientMessage(playerid, COLOR_GREY, "{FF0000}Error:{FFFFFF} You're not holding a weapon.");
				return 1;
			}
			if(DropWeaponTimer[playerid] == 1)
				return SendClientMessage(playerid, COLOR_GREY, "You must wait 1 minute before dropping a weapon again!");
			if(playerHoldingTazer[playerid] == 1) return SendClientMessage(playerid, COLOR_GREY, "You can't drop a tazer.");
			if(inPaintball[playerid] > 0) return SendClientMessage(playerid, COLOR_GRAD1, "You cannot drop a weapon at this time.");
			if(OfferedGunTo[playerid] != -1)   //offering someone a gun
			{
				OfferedGunFrom[OfferedGunTo[playerid]] = -1;
				GunOffered[OfferedGunTo[playerid]] = 0;
				OfferedGunTo[playerid] = -1;
			}
			new gunid = GetPlayerWeapon(playerid);
			if(gunid)
			{
				//make sure it's a scripted gun to prevent people from dropping hacked weapons (people could pick them up)
				if(HaveWeapon(playerid, gunid) != gunid)
				{
					format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has attempted to drop a fake weapon.", PlayerICName(playerid));
					ABroadCast(COLOR_WHITE, str, 1);
					return SendClientMessage(playerid, COLOR_GREY, "You are not carrying a weapon!");
				}
				new f = 100 +1;
				for(new a = 0; a < sizeof(ObjCoords); a++)
				{
					if(ObjCoords[a][0] == 0.0)
					{
						f = a;
						break;
					}
				}
				if(f > 100) return SendClientMessage(playerid, COLOR_GREY, "You can not throw weapons at the moment, try back later!");
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				GetWeaponName(gunid, WeaponName, 64);
				if(gunid == 40 && BombID[playerid] != 0)
				{
					if(BombID[playerid]) { DestroyDynamicObjectEx(BombID[playerid]); }
					BombID[playerid] = -1;
				}
				if(gunid == 18)
				{
					WeaponName = "Molotovs";
				}
				if(gunid == 44)
				{
					WeaponName = "Nightvision Goggles";
				}
				if(gunid == 45)
				{
					WeaponName = "Infared Goggles";
				}
				new gunID = GetPlayerWeapon(playerid);
				new pgun0 = PlayerInfo[playerid][pGun0];
				new pgun1 = PlayerInfo[playerid][pGun1];
				new pgun2 = PlayerInfo[playerid][pGun2];
				new pgun3 = PlayerInfo[playerid][pGun3];
				new pgun4 = PlayerInfo[playerid][pGun4];
				new pgun5 = PlayerInfo[playerid][pGun5];
				new pgun6 = PlayerInfo[playerid][pGun6];
				new pgun7 = PlayerInfo[playerid][pGun7];
				new pgun8 = PlayerInfo[playerid][pGun8];
				new pgun9 = PlayerInfo[playerid][pGun9];
				new pgun10 = PlayerInfo[playerid][pGun10];
				new pgun11 = PlayerInfo[playerid][pGun11];
				new pgun12 = PlayerInfo[playerid][pGun12];
				new isadmingun = 0;
				if(pgun0 != gunID && pgun1 != gunID && pgun2 != gunID && pgun3 != gunID && pgun4 != gunID && pgun5 != gunID && pgun6 != gunID && pgun7 != gunID && pgun8 != gunID && pgun9 != gunID && pgun10 != gunID && pgun11 != gunID && pgun12 != gunID)
				{
					isadmingun = 1;
				}
				else
				{
					isadmingun = 0;
				}
				TakePlayerWeapon(playerid, gunid);
				if(IsACop(playerid) == 0 && IsANG(playerid) == 0 && IsAMedic(playerid) == 0 && isadmingun == 0 || PlayerInfo[playerid][pMember] != 6)
				{
					wObjectID[f][0] = gunid;
					GetPlayerPos(playerid, ObjCoords[f][0], ObjCoords[f][1], ObjCoords[f][2]);
					object[f] = CreateDynamicObject(GunObjects[gunid][0], ObjCoords[f][0], ObjCoords[f][1], ObjCoords[f][2] -1, 93.7, 120.0, 120.0, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), -1, 100.0);
					Streamer_Update(playerid);
					//300000 = 5minutes.
					SetTimerEx("DeleteGun", 300000, 0, "dd", object[f], f);
					DropWeaponTimer[playerid] = 1;
					SetTimerEx("Timer_DropWepTimer", 60*1000, 0, "i", playerid);
				}
				format(str, sizeof(str), "* %s has dropped a %s (/get to pick it up).", PlayerICName(playerid), WeaponName);
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				SetPlayerArmedWeapon(playerid, 0);
				return 1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not holding a weapon.");
				return 1;
			}
		}
		else if(strcmp(x_nr, "kevlar", true) == 0)
		{
			//cancel the kevlar offer
			if(KevlarOfferTo[playerid] != -1)   //offering someone a kevlar
			{
				KevlarOfferFrom[KevlarOfferTo[playerid]] = -1;
				KevlarOfferTo[playerid] = -1;
			}
			if(IsACop(playerid) == 1 || IsANG(playerid) == 1 || PlayerInfo[playerid][pMember] == 9 || IsAMedic(playerid) == 1 || IsASenate(playerid)) return SendClientMessage(playerid, COLOR_GRAD1, "Cops/DEA/DoC can not use this command.");
			new Float:armorpt;
			GetPlayerArmour(playerid, armorpt);
			if(!(armorpt == 0))
			{
				new f = 100 +1;
				for(new a = 0; a < sizeof(kObjCoords); a++)
				{
					if(ObjCoords[a][0] == 0.0)
					{
						f = a;
						break;
					}
				}
				if(FVestProtected[playerid] == 1) return SendClientMessage(playerid, COLOR_RED, "You cannot /drop kevlar after spawn, wait few minutes.");
				if(KevlarDropped[playerid] == 1) return SendClientMessage(playerid, COLOR_RED, "You cannot /drop kevlar within 45 seconds");
				if(f > 100) return SendClientMessage(playerid, COLOR_GREY, "You can not throw kevlars at the moment, try back later.");
				kObjectVal[f][0] =  armorpt;
				SetPlayerArmour(playerid, 0);
				GetPlayerPos(playerid, kObjCoords[f][0], kObjCoords[f][1], kObjCoords[f][2]);
				kobject[f] = CreateDynamicObject(1242, kObjCoords[f][0], kObjCoords[f][1], (kObjCoords[f][2] -1), 90.0, 90.0, 0.0, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), -1, 100.0);
				Streamer_Update(playerid);
				SetTimerEx("DeleteKevlar", 30000, 0, "dd", kobject[f], f);
				KevlarDropped[playerid] = 1; //It shows that player droppped the Kevlar
				SetTimerEx("DropKevlarTimer", 45000, 0, "d", playerid);//45seconds must be passed for use /drop kevlar again.
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				format(str, sizeof(str), "* %s has dropped their kevlar (/get to pick it up).", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				return 1;
			}
			else return SendClientMessage(playerid, COLOR_GREY, "You are not wearing a kevlar.");
		}
		/*else if(strcmp(x_nr, "products", true) == 0)
		{
			if(PlayerInfo[playerid][pProducts] != 0)
			{
				PlayerInfo[playerid][pProducts] = 0;
				format(str, sizeof(str), "* %s has dropped all of their products.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			else return SendClientMessage(playerid, COLOR_GREY, "You don't have any products to drop.");
		}*/
		else if(strcmp(x_nr, "screwdrivers", true) == 0)
		{
			if(PlayerInfo[playerid][pScrew] != 0)
			{
				PlayerInfo[playerid][pScrew] = 0;
				format(str, sizeof(str), "* %s has dropped all of their screwdrivers.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			else return SendClientMessage(playerid, COLOR_GREY, "You don't have any screwdrivers to drop.");
		}
		else if(strcmp(x_nr, "ropes", true) == 0)
		{
			if(PlayerInfo[playerid][pRope] != 0)
			{
				PlayerInfo[playerid][pRope] = 0;
				format(str, sizeof(str), "* %s has dropped all of their ropes.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			else return SendClientMessage(playerid, COLOR_GREY, "You don't have any ropes to drop.");
		}
		else if(strcmp(x_nr, "gags", true) == 0)
		{
			if(PlayerInfo[playerid][pGag] != 0)
			{
				PlayerInfo[playerid][pGag] = 0;
				format(str, sizeof(str), "* %s has dropped all of their gags.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			else return SendClientMessage(playerid, COLOR_GREY, "You don't have any gags to drop.");
		}
		else if(strcmp(x_nr, "tikis", true) == 0)
		{
			if(PlayerInfo[playerid][pTikis] == 0)return SendClientMessage(playerid, COLOR_GREY, "You don't have any Tikis.");

			PlayerInfo[playerid][pTikis] = 0;
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			format(str, sizeof(str), "* %s has dropped all of their tikis.", sendername);
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
			return 1;
		}
		else if(strcmp(x_nr, "meth", true) == 0)
		{
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "USAGE: {FFFFFF}/dropdrug [drug name (/drugs)] [amount] [bag/box]");
		}
		else if(strcmp(x_nr, "pot", true) == 0)
		{
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "USAGE: {FFFFFF}/dropdrug [drug name (/drugs)] [amount] [bag/box]");
		}
		else if(strcmp(x_nr, "seeds", true) == 0)
		{
			if(PlayerInfo[playerid][pPotSeeds] > 0)
			{
				PlayerInfo[playerid][pPotSeeds] = 0;
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				format(str, sizeof(str), "* %s has dropped a bag of seeds.", sendername);
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				return 1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not carrying any Seeds to throw away.");
				return 1;
			}
		}
		else if(strcmp(x_nr, "materials", true) == 0)
		{
			if(PlayerInfo[playerid][pMats] > 0)
			{
				new f = 100 +1;
				for(new a = 0; a < sizeof(mObjCoords); a++)
				{
					if(mObjCoords[a][0] == 0.0)
					{
						f = a;
						break;
					}
				}
				mObjectID[f][0] = PlayerInfo[playerid][pMats];
				PlayerInfo[playerid][pMats] = 0;
				GetPlayerPos(playerid, mObjCoords[f][0], mObjCoords[f][1], mObjCoords[f][2]);
				mobject[f] = CreateDynamicObject(1580, mObjCoords[f][0], mObjCoords[f][1], mObjCoords[f][2] -1, 180.0, 180.0, 120.0, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), -1, 100.0);
				Streamer_Update(playerid);
				SetTimerEx("DeleteMats", 30000, 0, "dd", mobject[f], f);
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				format(str, sizeof(str), "* %s has dropped a bag of materials (/get to pick it up).", sendername);
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				return 1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not carrying any Materials to throw away.");
				return 1;
			}
		}
		else if(strcmp(x_nr, "packages", true) == 0)
		{
			if(Packages[playerid] > 0)
			{
				Packages[playerid] = 0;
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				format(str, sizeof(str), "* %s has dropped their packages.", sendername);
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				if(CP[playerid] != CHECKPOINT_NONE)
				{
						CP[playerid] = CHECKPOINT_NONE;
						DisablePlayerCheckpointEx(playerid);
						SendClientMessage(playerid, COLOR_WHITE, "Checkpoint removed! ");
				}
				else if(goingJob[playerid] == true)
				{
						DisablePlayerCheckpointEx(playerid);
						goingJob[playerid] = false;
						SendClientMessage(playerid, COLOR_WHITE, "Checkpoint removed! ");
				}
					else if(isRepairing[playerid] == true)
				{
						isRepairing[playerid] = false;
						DisablePlayerCheckpointEx(playerid);
						SendClientMessage(playerid, COLOR_WHITE, "Checkpoint removed.");
				}
				return 1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not carrying any Packages to throw away.");
				return 1;
			}
		}
		else if(strcmp(x_nr, "crates", true) == 0)
		{
			if(Crates[playerid] > 0)
			{
				Crates[playerid] = 0;
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				format(str, sizeof(str), "* %s has dropped their drug crates.", sendername);
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				return 1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not carrying any drug crates.");
				return 1;
			}
		}
		else if(strcmp(x_nr, "meal", true) == 0)
		{
			if(hasFoodTray[playerid])
			{
				format(str, sizeof(str), "* %s has thrown away their Meal.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				throwAwayFoodTray(playerid);
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not carrying a Meal.");
				return 1;
			}
		}
		else if(strcmp(x_nr, "boombox", true) == 0)
		{
			if(PlayerInfo[playerid][pStereo] == 0)return SendClientMessage(playerid, COLOR_GREY, "You don't have a Boombox.");

			PlayerInfo[playerid][pStereo] = 0;
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			format(str, sizeof(str), "* %s has dropped their boombox.", sendername);
			ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
			return 1;
		}
		else if(strcmp(x_nr, "cellphone", true) == 0)
		{
			if(PlayerInfo[playerid][pPnumber])
			{
				if(Mobile[playerid] != 999) return SendClientMessage(playerid, COLOR_GRAD2, "You can't throw away your Cellphone while you are on a call.");
				format(str, sizeof(str), "* %s has thrown away their Cellphone.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				PlayerInfo[playerid][pPnumber] = 0;
				PhoneOffline[playerid] = 0;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GRAD2, "You don't have a cell phone.");
				return 1;
			}
		}
		/*else if(strcmp(x_nr, "trash", true) == 0)
		{
			if(CP[playerid] == CHECKPOINT_GARBAGE)
			{
				CP[playerid] = CHECKPOINT_NONE;
				DisablePlayerCheckpointEx(playerid);

				format(str, sizeof(str), "* %s has thrown away their Trash.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

				if(IsPlayerAttachedObjectSlotUsed(playerid, INDEX_8))
				{
					RemovePlayerAttachedObject(playerid, INDEX_8);
					SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
				}

				//reset variables
				hasTruckCheckpoint[playerid] = -1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GRAD2, "You are not holding any Trash.");
				return 1;
			}
		}*/
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "Invalid drop name.");
			return 1;
		}
		return 1;
	}
	if(strcmp(cmd, "/give", true) == 0)
	{
		if(pDeathState[playerid] != PLAYER_STATE_NORMAL)
			return SendClientMessage(playerid, COLOR_GREY, "You can't use this at the moment.");

		if(IsAtEvent[playerid] == 1)
			return SendClientMessage(playerid, COLOR_GREY, "You can't use this in events.");

		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /give [playerid/PartOfName] [name] [amount]");
			SendClientMessage(playerid, COLOR_GRAD1, "Available names: Cannabis, Cocaine, Materials, Seeds, Weapon, Kevlar, Beer, Tiki");
			return 1;
		}
		giveplayerid = ReturnUser(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /give [playerid/PartOfName] [name] [amount]");
			SendClientMessage(playerid, COLOR_GRAD1, "Available names: Cannabis, Cocaine, Materials, Seeds, Weapon, Kevlar, Tiki");
			return 1;
		}
		new givetoid = giveplayerid;
		if(strcmp(tmp, "weapon", true) == 0)
		{
			if(GetPlayerSpecialAction(givetoid) == SPECIAL_ACTION_USEJETPACK) return SendClientMessage(playerid, COLOR_GREY, "You cannot use this at this time");

			if(GetPlayerWeapon(playerid) == 0) return SendClientMessage(playerid, COLOR_GRAD1, "You are not holding any weapon.");
			if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GRAD2, "You can't do this at this time.");
			if(IsAtEvent[playerid] == 1) return SendClientMessage(playerid, COLOR_GRAD1, "You cannot give someone a weapon at this time.");
			if(inPaintball[playerid] > 0) return SendClientMessage(playerid, COLOR_GRAD1, "You cannot give someone a weapon at this time.");
			if(PlayerInfo[playerid][pGroup] != GROUP_CRIME)
			{
				if(IsACop(playerid) == 1 || IsANG(playerid) == 1 || IsAMedic(playerid) == 1 || IsASenate(playerid) || PlayerInfo[playerid][pMember] == 10 || PlayerInfo[playerid][pMember] == 9 || PlayerInfo[playerid][pMember] == FACTION_SANG)
				return SendClientMessage(playerid, COLOR_GRAD1, "Cops/DEA/SANG/DoC/SAN/Taxi Company can not use this command.");
			}
			if(PlayerInfo[giveplayerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "That player must play for at least 2 hours before they can get a weapon.");
			if(IsPlayerInAnyVehicle(givetoid)) return SendClientMessage(playerid, COLOR_GREY, "You can't give a weapon to players if they are inside of a vehicle.");
			//make sure it's a scripted gun to prevent people from dropping hacked weapons (people could pick them up)
			new gunid = GetPlayerWeapon(playerid);
			if(HaveWeapon(playerid, gunid) != gunid)
			{
				format(str, sizeof(str), "{FF0000}AdmWarning{FFFFFF}: %s has attempted to give a fake weapon.", PlayerICName(playerid));
				ABroadCast(COLOR_WHITE, str, 1);
				return SendClientMessage(playerid, COLOR_GREY, "You are not carrying a weapon!");
			}
			new Float:fX, Float:fY, Float:fZ;
			GetPlayerPos(givetoid, fX, fY, fZ);
			if(!IsPlayerInRangeOfPoint(playerid, 3.0, fX, fY, fZ)) return SendClientMessage(playerid, COLOR_GRAD1, "You are not close enough to that player to offer him weapons.");
			OfferedGunTo[playerid] = givetoid;
			OfferedGunFrom[givetoid] = playerid;
			GunOffered[givetoid] = GetPlayerWeapon(playerid);
			new weaponname[50];
			GetWeaponName(GetPlayerWeapon(playerid), weaponname, sizeof(weaponname));
			format(string5, sizeof(string5), "You have offered your %s to %s, wait to see if he will accept it.", weaponname, PlayerICName(givetoid));
			SendClientMessage(playerid, COLOR_LIGHTBLUE, string5);
			format(string5, sizeof(string5), "%s has offered you their %s, /accept weapon to accept it.", PlayerICName(playerid), weaponname);
			SendClientMessage(givetoid, COLOR_LIGHTBLUE, string5);
		}
		else if(strcmp(tmp, "kevlar", true) == 0)
		{
			if(IsACop(playerid) == 1 || IsANG(playerid) == 1 || IsAMedic(playerid) == 1 || IsASenate(playerid) || PlayerInfo[playerid][pMember] == 10 || PlayerInfo[playerid][pMember] == 9 || PlayerInfo[playerid][pMember] == FACTION_SANG) return SendClientMessage(playerid, COLOR_GRAD1, "Cops/DEA/DoC/SAN/Taxi Company can not use this command.");
			
			if(PlayerPaused[givetoid])
				return SendClientMessage(playerid, COLOR_RED, "ERROR: {FFFFFF}You cannot give kevlar to tabbed/afk players!");
			if(FVestProtected[playerid] == 1) return SendClientMessage(playerid, COLOR_RED, "You cannot /give kevlar after spawn, wait few minutes.");
			if(IsPlayerConnected(givetoid))
			{
				new Float:armorpt;
				GetPlayerArmour(playerid, armorpt);
				if(!(armorpt == 0))
				{
					new Float:x, Float:y, Float:z;
					GetPlayerPos(givetoid, x, y, z);
					if(IsPlayerInRangeOfPoint(playerid, 3.0, x, y, z))
					{
						format(string5, sizeof(string5), "You have offered your vest to %s, wait to see if he will accept it.", PlayerICName(givetoid));
						SendClientMessage(playerid, COLOR_LIGHTBLUE, string5);
						format(string5, sizeof(string5), "%s has offered you their kevlar, /accept kevlar to accept it.", PlayerICName(playerid));
						SendClientMessage(givetoid, COLOR_LIGHTBLUE, string5);
						KevlarOfferTo[playerid] = givetoid;
						KevlarOfferFrom[givetoid] = playerid;
					}
					else return SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
				}
				else return SendClientMessage(playerid, COLOR_GREY, "You are not wearing a kevlar.");
			}
			else return SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
		}
		else if(strcmp(tmp, "materials", true) == 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /give [playerid/PartOfName] [name] [amount]");
				SendClientMessage(playerid, COLOR_GRAD1, "Available names: Pot, Crack, Materials, Seeds");
				return 1;
			}
			new amount = strvalEx(tmp);
			if(IsPlayerConnected(giveplayerid))
			{
				if(giveplayerid != INVALID_PLAYER_ID)
				{
					if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
					{
						if(playerid == giveplayerid)
						{
							SendClientMessage(playerid, COLOR_GREY, "You can't give to yourself.");
							return 1;
						}
						if(amount > PlayerInfo[playerid][pMats] || amount < 1)
						{
							SendClientMessage(playerid, COLOR_GREY, "You don't have that much.");
							return 1;
						}
						if(amount > 50000)
						{
							SendClientMessage(playerid, COLOR_GREY, "You can't give more than 50,000 at a time.");
							return 1;
						}
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
						format(str, sizeof(str), "You have given %s %d Materials.", giveplayer, amount);
						SendClientMessage(playerid, COLOR_GRAD1, str);
						format(str, sizeof(str), "You have received %d Materials from %s.", amount, sendername);
						SendClientMessage(giveplayerid, COLOR_GRAD1, str);
						format(str, sizeof(str), "* %s has given %s some Materials.", sendername, giveplayer);
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						PlayerInfo[playerid][pMats] = PlayerInfo[playerid][pMats] -amount;
						PlayerInfo[giveplayerid][pMats] = PlayerInfo[giveplayerid][pMats] +amount;
						format(str, sizeof(str), "%s has given %s %d Materials.", sendername, giveplayer, amount);
						PayLog(str);
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
						return 1;
					}
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
				return 1;
			}
		}
		else if(strcmp(tmp, "seeds", true) == 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /give [playerid/PartOfName] [name] [amount]");
				SendClientMessage(playerid, COLOR_GRAD1, "Available names: Pot, Crack, Materials, Seeds");
				return 1;
			}
			new amount = strvalEx(tmp);
			if(IsPlayerConnected(giveplayerid))
			{
				if(giveplayerid != INVALID_PLAYER_ID)
				{
					if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
					{
						if(playerid == giveplayerid)
						{
							SendClientMessage(playerid, COLOR_GREY, "You can't give to yourself.");
							return 1;
						}
						if(amount > PlayerInfo[playerid][pPotSeeds] || amount < 1)
						{
							SendClientMessage(playerid, COLOR_GREY, "You don't have that much.");
							return 1;
						}
						if(amount + PlayerInfo[giveplayerid][pPotSeeds] > 50)
						{
							SendClientMessage(playerid, COLOR_GREY, "That player can only hold a maximum of 50 seeds.");
							return 1;
						}
						if(amount > 500)
						{
							SendClientMessage(playerid, COLOR_GREY, "You can't give more than 50 at a time.");
							return 1;
						}
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
						format(str, sizeof(str), "You have given %s %d Seeds.", giveplayer, amount);
						SendClientMessage(playerid, COLOR_GRAD1, str);
						format(str, sizeof(str), "You have received %d Seeds from %s.", amount, sendername);
						SendClientMessage(giveplayerid, COLOR_GRAD1, str);
						format(str, sizeof(str), "* %s has given %s some Seeds.", sendername, giveplayer);
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						PlayerInfo[playerid][pPotSeeds] = PlayerInfo[playerid][pPotSeeds] -amount;
						PlayerInfo[giveplayerid][pPotSeeds] = PlayerInfo[giveplayerid][pPotSeeds] +amount;
						format(str, sizeof(str), "%s has given %s %d Seeds.", sendername, giveplayer, amount);
						PayLog(str);
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
						return 1;
					}
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
				return 1;
			}
		}
		else if(strcmp(tmp, "tiki", true) == 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /give [playerid/PartOfName] [name] [amount]");
				SendClientMessage(playerid, COLOR_GRAD1, "Available names: Tiki");
				return 1;
			}
			new amount = strvalEx(tmp);
			if(IsPlayerConnected(giveplayerid))
			{
				if(giveplayerid != INVALID_PLAYER_ID)
				{
					if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
					{
						if(playerid == giveplayerid)
						{
							SendClientMessage(playerid, COLOR_GREY, "You can't give to yourself.");
							return 1;
						}
						if(amount > PlayerInfo[playerid][pTikis] || amount < 1)
						{
							SendClientMessage(playerid, COLOR_GREY, "You don't have that much.");
							return 1;
						}
						if(amount + PlayerInfo[giveplayerid][pTikis] > 100)
						{
							SendClientMessage(playerid, COLOR_GREY, "That player can only have 100 tikis from a player.");
							return 1;
						}
						if(amount > 100)
						{
							SendClientMessage(playerid, COLOR_GREY, "You can't give more than 100 at a time.");
							return 1;
						}
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
						format(str, sizeof(str), "You have given %s %d Tikis.", giveplayer, amount);
						SendClientMessage(playerid, COLOR_GRAD1, str);
						format(str, sizeof(str), "You have received %d Tikis from %s.", amount, sendername);
						SendClientMessage(giveplayerid, COLOR_GRAD1, str);
						PlayerInfo[playerid][pTikis] = PlayerInfo[playerid][pTikis] -amount;
						PlayerInfo[giveplayerid][pTikis] = PlayerInfo[giveplayerid][pTikis] +amount;
						format(str, sizeof(str), "%s has given %s %d Tikis.", sendername, giveplayer, amount);
						PayLog(str);
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
						return 1;
					}
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
				return 1;
			}
		}
		else if(strcmp(tmp, "beer", true) == 0)
		{
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /give [playerid/PartOfName] [name] [amount]");
				SendClientMessage(playerid, COLOR_GRAD1, "Available names: Pot, Crack, Materials, Seeds, Beer");
				return 1;
			}
			new amount = strvalEx(tmp);
			if(IsPlayerConnected(giveplayerid))
			{
				if(giveplayerid != INVALID_PLAYER_ID)
				{
					if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
					{
						if(playerid == giveplayerid)
						{
							SendClientMessage(playerid, COLOR_GREY, "You can't give to yourself.");
							return 1;
						}
						if(amount > PlayerInfo[playerid][pBeer] || amount < 1)
						{
							SendClientMessage(playerid, COLOR_GREY, "You don't have that much.");
							return 1;
						}
						if(amount > 5)
						{
							SendClientMessage(playerid, COLOR_GREY, "You can't give more than 5 at a time.");
							return 1;
						}
						if(PlayerInfo[giveplayerid][pBeer] + amount > 5)
						{
							SendClientMessage(playerid, COLOR_GREY, "That player can't carry more than 5 beer.");
							return 1;
						}
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
						format(str, sizeof(str), "You have given %s %d Beer.", giveplayer, amount);
						SendClientMessage(playerid, COLOR_GRAD1, str);
						format(str, sizeof(str), "You have received %d Beer from %s.", amount, sendername);
						SendClientMessage(giveplayerid, COLOR_GRAD1, str);
						format(str, sizeof(str), "* %s has given %s Beer.", sendername, giveplayer);
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						PlayerInfo[playerid][pBeer] = PlayerInfo[playerid][pBeer] -amount;
						PlayerInfo[giveplayerid][pBeer] = PlayerInfo[giveplayerid][pBeer] +amount;
						format(str, sizeof(str), "%s has given %s %d Beer.", sendername, giveplayer, amount);
						PayLog(str);
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
						return 1;
					}
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
				return 1;
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "Invalid give name.");
			return 1;
		}
		return 1;
	}
	if(strcmp(cmd, "/take", true) == 0)
	{
		if(IsACop(playerid) || IsANG(playerid) || IsASANG(playerid) || PlayerInfo[playerid][pMember] == 11)
		{
			if(PlayerInfo[playerid][pRank] < 1 && PlayerInfo[playerid][pMember] != 11)
			{
				SendClientMessage(playerid, COLOR_GREY, "Your rank is not high enough.");
				return 1;
			}
			new x_nr[64];
			x_nr = strtok(cmdtext, idx);
			if(!strlen(x_nr))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /take [name] [player]");
				SendClientMessage(playerid, COLOR_GRAD1, "Available names: Weapons, Drugs, Materials, Cellphone, Seeds");
				return 1;
			}
			if(strcmp(x_nr, "weapons", true) == 0)
			{
				tmp = strtok(cmdtext, idx);
				if(!strlen(tmp))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /take [name] [playerid/PartOfName]");
					return 1;
				}
				giveplayerid = ReturnUser(tmp);
				if(PlayerCuffed[giveplayerid] == 0 && PlayerPaused[giveplayerid] == 0 && PlayerTied[giveplayerid] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That player isn't restrained.");
				if(PlayerInfo[giveplayerid][pConnectTime] < 24) return SendClientMessage(playerid, COLOR_GRAD1, "That player needs to have 24 playing hours.");
				if(IsPlayerConnected(giveplayerid))
				{
					if(giveplayerid != INVALID_PLAYER_ID)
					{
						if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
						{
							strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
							strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
							format(str, sizeof(str), "You have taken %s's Weapons.", giveplayer);
							SendClientMessage(playerid, COLOR_GRAD1, str);
							format(str, sizeof(str), "%s has taken your Weapons.", sendername);
							SendClientMessage(giveplayerid, COLOR_GRAD1, str);
							format(str, sizeof(str), "* %s %s has taken %s's Weapons.", GetPlayerRank(playerid), sendername, giveplayer);
							ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
							ClearGuns(giveplayerid);
							ResetPlayerWeapons(giveplayerid);
							format(str, sizeof(str), "%s has taken %s's Weapons.", sendername, giveplayer);
							PayLog(str);
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
							return 1;
						}
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
					return 1;
				}
			}
			/*else if(strcmp(x_nr, "drugs", true) == 0)
			{
				tmp = strtok(cmdtext, idx);
				if(!strlen(tmp))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /take [name] [playerid/PartOfName]");
					return 1;
				}
				giveplayerid = ReturnUser(tmp);
				if(PlayerCuffed[giveplayerid] == 0 && PlayerPaused[giveplayerid] == 0 && PlayerTied[giveplayerid] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That player isn't restrained.");
				if(PlayerInfo[giveplayerid][pConnectTime] < 24) return SendClientMessage(playerid, COLOR_GRAD1, "That player needs to have 24 playing hours.");
				if(IsPlayerConnected(giveplayerid))
				{
					if(giveplayerid != INVALID_PLAYER_ID)
					{
						if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
						{
							format(str, sizeof(str), "You have taken %d grams of cannabis, %d grams of cocaine, and %d grams of meth from %s.", PlayerInfo[giveplayerid][pCannabis], PlayerInfo[giveplayerid][pCocaine], PlayerInfo[giveplayerid][pMeth], PlayerICName(giveplayerid));
							SendClientMessage(playerid, COLOR_GRAD1, str);
							
							format(str, sizeof(str), "%s has taken %d grams of cannabis, %d grams of cocaine, and %d grams of meth.", PlayerICName(playerid), PlayerInfo[giveplayerid][pCannabis], PlayerInfo[giveplayerid][pCocaine], PlayerInfo[giveplayerid][pMeth]);
							SendClientMessage(giveplayerid, COLOR_GRAD1, str);

							format(str, sizeof(str), "* %s %s has taken away %s's drugs.", GetPlayerRank(playerid), PlayerICName(playerid), PlayerICName(giveplayerid));
							ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

							PlayerInfo[giveplayerid][pCannabis] = 0;
							PlayerInfo[giveplayerid][pCocaine] = 0;
							PlayerInfo[giveplayerid][pMeth] = 0;
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
							return 1;
						}
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
					return 1;
				}
			}*/
			else if(strcmp(x_nr, "drugs", true) == 0)
			{
				tmp = strtok(cmdtext, idx);
				if(!strlen(tmp))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /take [name] [playerid/PartOfName]");
					return 1;
				}
				giveplayerid = ReturnUser(tmp);
				if(PlayerCuffed[giveplayerid] == 0 && PlayerPaused[giveplayerid] == 0 && PlayerTied[giveplayerid] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That player isn't restrained.");
				if(PlayerInfo[giveplayerid][pConnectTime] < 24) return SendClientMessage(playerid, COLOR_GRAD1, "That player needs to have 24 playing hours.");
				if(IsPlayerConnected(giveplayerid))
				{
					if(giveplayerid != INVALID_PLAYER_ID)
					{
						if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
						{
							format(str, sizeof(str), "You have taken %d grams of cannabis, %d grams of cocaine, %d grams of meth, %d grams of xanax, %d grams of promethazine, %d grams of codeine, and %d grams of lean from %s.", PlayerInfo[giveplayerid][pCannabis], PlayerInfo[giveplayerid][pCocaine], PlayerInfo[giveplayerid][pMeth], PlayerInfo[giveplayerid][pXanax], PlayerInfo[giveplayerid][pPromethazine], PlayerInfo[giveplayerid][pCodeine], PlayerInfo[giveplayerid][pLean],  PlayerICName(giveplayerid));
							SendClientMessage(playerid, COLOR_GRAD1, str);
							
							format(str, sizeof(str), "%s has taken %d grams of cannabis, %d grams of cocaine, %d grams of meth, %d grams of xanax, %d grams of promethazine, %d grams of codeine, and %d grams of lean.", PlayerICName(playerid), PlayerInfo[giveplayerid][pCannabis], PlayerInfo[giveplayerid][pCocaine], PlayerInfo[giveplayerid][pMeth], PlayerInfo[giveplayerid][pXanax], PlayerInfo[giveplayerid][pPromethazine], PlayerInfo[giveplayerid][pCodeine], PlayerInfo[giveplayerid][pLean]);
							SendClientMessage(giveplayerid, COLOR_GRAD1, str);

							format(str, sizeof(str), "* %s %s has taken away %s's drugs.", GetPlayerRank(playerid), PlayerICName(playerid), PlayerICName(giveplayerid));
							ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

							PlayerInfo[giveplayerid][pCannabis] = 0;
							PlayerInfo[giveplayerid][pCocaine] = 0;
							PlayerInfo[giveplayerid][pMeth] = 0;
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
							return 1;
						}
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
					return 1;
				}
			}
			else if(strcmp(x_nr, "seeds", true) == 0)
			{
				tmp = strtok(cmdtext, idx);
				if(!strlen(tmp))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /take [name] [playerid/PartOfName]");
					return 1;
				}
				giveplayerid = ReturnUser(tmp);
				if(PlayerCuffed[giveplayerid] == 0 && PlayerPaused[giveplayerid] == 0 && PlayerTied[giveplayerid] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That player isn't restrained.");
				if(PlayerInfo[giveplayerid][pConnectTime] < 24) return SendClientMessage(playerid, COLOR_GRAD1, "That player needs to have 24 playing hours.");
				if(IsPlayerConnected(giveplayerid))
				{
					if(giveplayerid != INVALID_PLAYER_ID)
					{
						if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
						{
							if(PlayerInfo[giveplayerid][pPotSeeds] < 1) return SendClientMessage(playerid, COLOR_GREY, "That player doesn't have any Seeds.");
							if(PlayerInfo[giveplayerid][pLevel] < 4)
							{
								format(str, sizeof(str), "You can't confisticate seeds from this player because their level is %d.", PlayerInfo[giveplayerid][pLevel]);
								SendClientMessage(playerid, COLOR_GRAD1, str);
								return 1;
							}
							strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
							strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
							format(str, sizeof(str), "You have taken %d Seeds from %s.", PlayerInfo[giveplayerid][pPotSeeds], giveplayer);
							SendClientMessage(playerid, COLOR_GRAD1, str);
							format(str, sizeof(str), "%s has taken your %d Seeds.", sendername, PlayerInfo[giveplayerid][pPotSeeds]);
							SendClientMessage(giveplayerid, COLOR_GRAD1, str);
							format(str, sizeof(str), "* %s %s has taken away %s's Seeds.", GetPlayerRank(playerid), sendername, giveplayer);
							ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
							//PlayerInfo[playerid][pPotSeeds] += PlayerInfo[giveplayerid][pPotSeeds];
							PlayerInfo[giveplayerid][pPotSeeds] = 0;
							format(str, sizeof(str), "%s has taken %d Seeds from %s.", sendername, PlayerInfo[giveplayerid][pPotSeeds], giveplayer);
							PayLog(str);
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
							return 1;
						}
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
					return 1;
				}
			}
			else if(strcmp(x_nr, "materials", true) == 0)
			{
				tmp = strtok(cmdtext, idx);
				if(!strlen(tmp))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /take [name] [playerid/PartOfName]");
					return 1;
				}
				giveplayerid = ReturnUser(tmp);
				if(PlayerCuffed[giveplayerid] == 0 && PlayerPaused[giveplayerid] == 0 && PlayerTied[giveplayerid] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That player isn't restrained.");
				if(PlayerInfo[giveplayerid][pConnectTime] < 24) return SendClientMessage(playerid, COLOR_GRAD1, "That player needs to have 24 playing hours.");
				if(IsPlayerConnected(giveplayerid))
				{
					if(giveplayerid != INVALID_PLAYER_ID)
					{
						if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
						{
							if(PlayerInfo[giveplayerid][pMats] < 1) return SendClientMessage(playerid, COLOR_GREY, "That player doesn't have any Materials.");
							if(PlayerInfo[giveplayerid][pLevel] < 4)
							{
								format(str, sizeof(str), "You can't confisticate materials from this player because their level is %d.", PlayerInfo[giveplayerid][pLevel]);
								SendClientMessage(playerid, COLOR_GRAD1, str);
								return 1;
							}
							strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
							strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
							format(str, sizeof(str), "You have taken %d Materials from %s.", PlayerInfo[giveplayerid][pMats], giveplayer);
							SendClientMessage(playerid, COLOR_GRAD1, str);
							format(str, sizeof(str), "%s has taken your %d Materials.", sendername, PlayerInfo[giveplayerid][pMats]);
							SendClientMessage(giveplayerid, COLOR_GRAD1, str);
							format(str, sizeof(str), "* %s %s has taken away %s's Materials.", GetPlayerRank(playerid), sendername, giveplayer);
							ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
							//PlayerInfo[playerid][pMats] += PlayerInfo[giveplayerid][pMats];
							PlayerInfo[giveplayerid][pMats] = 0;
							format(str, sizeof(str), "%s has taken %d Materials from %s.", sendername, PlayerInfo[giveplayerid][pMats], giveplayer);
							PayLog(str);
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
							return 1;
						}
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
					return 1;
				}
			}
			else if(strcmp(x_nr, "cellphone", true) == 0)
			{
				tmp = strtok(cmdtext, idx);
				if(!strlen(tmp))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /take [name] [playerid/PartOfName]");
					return 1;
				}
				giveplayerid = ReturnUser(tmp);
				if(PlayerCuffed[giveplayerid] == 0 && PlayerPaused[giveplayerid] == 0 && PlayerTied[giveplayerid] == 0) return SendClientMessage(playerid, COLOR_GRAD1, "That player isn't restrained.");
				if(PlayerInfo[giveplayerid][pConnectTime] < 24) return SendClientMessage(playerid, COLOR_GRAD1, "That player needs to have 24 playing hours.");
				if(IsPlayerConnected(giveplayerid))
				{
					if(giveplayerid != INVALID_PLAYER_ID)
					{
						if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
						{
							if(PlayerInfo[giveplayerid][pPnumber] == 0)
							{
								SendClientMessage(playerid, COLOR_GREY, "That player doesn't have a Cellphone.");
								return 1;
							}
							strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
							strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
							format(str, sizeof(str), "* You have taken away %s's Cellphone.", giveplayer);
							SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
							format(str, sizeof(str), "* %s %s as taken away your Cellphone.", GetPlayerRank(playerid), sendername);
							SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
							format(str, sizeof(str), "* %s %s has taken away %s's Cellphone.", GetPlayerRank(playerid), sendername, giveplayer);
							ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
							PlayerInfo[giveplayerid][pPnumber] = 0;
							format(str, sizeof(str), "%s has taken %s's Cellphone.", sendername, giveplayer);
							PayLog(str);
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
							return 1;
						}
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
					return 1;
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Invalid take name.");
				return 1;
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not a Cop / DEA / SASD / NG member.");
			return 1;
		}
		return 1;
	}
	if(strcmp(cmd, "/boxstats", true) == 0)
	{
		if(PlayerInfo[playerid][pJob] != 12)
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not a Boxer.");
			return 1;
		}
		new ttext[20];//Title
		new clevel = PlayerInfo[playerid][pBoxSkill];
		if(clevel >= 0 && clevel <= 49)
		{
			ttext = "Beginner";
		}
		else if(clevel >= 50 && clevel <= 199)
		{
			ttext = "Amateur";
		}
		else if(clevel >= 200 && clevel <= 399)
		{
			ttext = "Professional";
		}
		new ntext[20];//NickName
		new level = PlayerInfo[playerid][pWins];
		if(level > 0 && PlayerInfo[playerid][pLoses] == 0)
		{
			ntext = "Undefeated";
		}
		else
		{
			if(level >= 0 && level <= 10)
			{
				ntext = "Newcomer";
			}
			else if(level >= 11 && level <= 20)
			{
				ntext = "Touchy Fist";
			}
			else if(level >= 21 && level <= 30)
			{
				ntext = "Nut Cracker";
			}
			else if(level >= 31 && level <= 40)
			{
				ntext = "Tommygun";
			}
			else if(level >= 41 && level <= 50)
			{
				ntext = "Skull Breaker";
			}
			else if(level >= 51 && level <= 60)
			{
				ntext = "Light Speed";
			}
			else if(level >= 61 && level <= 70)
			{
				ntext = "Unbroken Warrior";
			}
			else if(level >= 71)
			{
				ntext = "Italian Stallion";
			}
		}
		SendClientMessage(playerid, COLOR_WHITE, "Boxing Records:");
		if(Titel[TitelWins] == 0)format(str, sizeof(str), "| Current Champion: Nobody");
		else format(str, sizeof(str), "| Current Champion: %s, with [%d] Winnings and [%d] Losses.", Titel[TitelName], Titel[TitelWins], Titel[TitelLoses]);
		SendClientMessage(playerid, COLOR_GREY, str);
		format(str, sizeof(str), "| Current Title: %s.", ttext);
		SendClientMessage(playerid, COLOR_GREY, str);
		format(str, sizeof(str), "| Current NickName: %s.", ntext);
		SendClientMessage(playerid, COLOR_GREY, str);
		format(str, sizeof(str), "| Total Wins: %d.", PlayerInfo[playerid][pWins]);
		SendClientMessage(playerid, COLOR_GREY, str);
		format(str, sizeof(str), "| Total Losses: %d.", PlayerInfo[playerid][pLoses]);
		SendClientMessage(playerid, COLOR_GREY, str);
		return 1;
	}
	if(strcmp(cmd, "/fight", true) == 0)
	{
		if(PlayerInfo[playerid][pJob] != 12)
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not a Boxer.");
			return 1;
		}
		if(InRing > 0)
		{
			SendClientMessage(playerid, COLOR_GREY, "There is already a Fight going on, wait for it to Finish.");
			return 1;
		}
		if(PlayerBoxing[playerid] > 0)
		{
			SendClientMessage(playerid, COLOR_GREY, "You are already Fighting.");
			return 1;
		}
		if(!IsPlayerInRangeOfPoint(playerid, 20.0, 765.9343, 0.2761, 1000.7173))
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not at the Ganton Gym.");
			return 1;
		}
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fight [Playerid/PartOfName]");
			return 1;
		}
		giveplayerid = ReturnUser(tmp);
		if(IsPlayerConnected(giveplayerid))
		{
			if(giveplayerid != INVALID_PLAYER_ID)
			{
				if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
				{
					if(giveplayerid == playerid)
					{
						SendClientMessage(playerid, COLOR_GREY, "You cannot Box with yourself.");
						return 1;
					}
					strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					format(str, sizeof(str), "* You offered a Boxing Challenge to %s.", giveplayer);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* Boxer %s wants to Fight with you (type /accept boxing) to accept.", sendername);
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					BoxOffer[giveplayerid] = playerid;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
					return 1;
				}
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
			return 1;
		}
		return 1;
	}
	if(strcmp(cmd, "/service", true) == 0)
	{

		new x_nr[64];
		x_nr = strtok(cmdtext, idx);
		if(!strlen(x_nr))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /service [name]");
			SendClientMessage(playerid, COLOR_GRAD1, "Available names: Medic, Mechanic, EMS, Lawyer");
			return 1;
		}
		else if(strcmp(x_nr, "medic", true) == 0)
		{
			if(pDeathState[playerid] != PLAYER_STATE_REVIVE) return SendClientMessage(playerid, COLOR_GREY, "You are not injured.");
			if(PlayerCalledEMS[playerid] == 1) return SendClientMessage(playerid, COLOR_GREY, "You are already awaiting the EMS.");
			new count =0;
			foreach( new i: Player )if(PlayerInfo[i][pMember] == 4)count++;
			if(count ==0) return SendClientMessage(playerid, COLOR_GREY, "There are currently no EMS online.");
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			format(str, sizeof(str), "Dispatch: %s is in need of a Paramedic (/EMScalls to list the calls).", PlayerICName(playerid));
			SendParaMessage(COLOR_DOCTOR, str);
			PlayerCalledEMS[playerid] = 1;
			displayCenterHUDInfo(playerid,  "~r~EMS Called.", 8); //display for 8 seconds
			TextDrawHideForPlayer(playerid, Text:DieText);
			TextDrawHideForPlayer(playerid, Text:InjuredText);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* The emergency services have been notified of your position.");
			return 1;
		}
		else if(strcmp(x_nr, "lawyer", true) == 0)
		{
			new Lawyers = 0;
			foreach( new i: Player )
			{
				if(IsPlayerConnected(i))
				{
					if(PlayerInfo[i][pJob] == 2 && JobDuty[i] == 1)
					{
						Lawyers += 1;
					}
				}
			}
			if(!(Lawyers >= 1))
			{
				SendClientMessage(playerid, COLOR_GREY, "There are no Lawyers on duty at the moment, try again later.");
				return 1;
			}
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			format(str, sizeof(str), "** %s is in need of a Lawyer. (use /accept lawyer to accept the call)", sendername);
			SendJobMessage(2, TEAM_AZTECAS_COLOR, str);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have called for a Lawyer, wait for a reply.");
			LawyerCall = playerid;
			return 1;
		}
		else if(strcmp(x_nr, "mechanic", true) == 0)
		{
			new Mechanics = 0;
			foreach( new i: Player )
			{
				if(IsPlayerConnected(i))
				{
					if(PlayerInfo[i][pJob] == 7 && JobDuty[i] == 1)
					{
						Mechanics += 1;
					}
				}
			}
			if(!(Mechanics >= 1))
			{
				SendClientMessage(playerid, COLOR_GREY, "There are no Mechanics on duty at the moment, try again later.");
				return 1;
			}
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			format(str, sizeof(str), "** %s is in need of a Mechanic. (use /accept mechanic to accept the call)", sendername);
			SendJobMessage(7, TEAM_AZTECAS_COLOR, str);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have called for a Mechanic, wait for a reply.");
			MechanicCall = playerid;
			return 1;
		}
		else if(strcmp(x_nr, "EMS", true) == 0)
		{
			if(pDeathState[playerid] != PLAYER_STATE_REVIVE) return SendClientMessage(playerid, COLOR_GREY, "You are not injured.");
			if(PlayerCalledEMS[playerid] == 1) return SendClientMessage(playerid, COLOR_GREY, "You are already awaiting the EMS.");
			new count = 0;
			foreach( new i: Player )if(PlayerInfo[i][pMember] == 4 || FakeFaction[i] == FACTION_FMD) count++;
			if(count ==0) return SendClientMessage(playerid, COLOR_GREY, "There are currently no EMS online.");
			strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
			format(str, sizeof(str), "Dispatch: %s is in need of a Paramedic (/EMScalls to list the calls).", PlayerICName(playerid));
			SendParaMessage(COLOR_DOCTOR, str);
			PlayerCalledEMS[playerid] = 1;
			displayCenterHUDInfo(playerid,  "~r~EMS Called.", 8); //display for 8 seconds
			TextDrawHideForPlayer(playerid, Text:DieText);
			TextDrawHideForPlayer(playerid, Text:InjuredText);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* The emergency services have been notified of your position.");
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "Unknown service name.");
			return 1;
		}
		return 1;
	}

	if(strcmp(cmd, "/tie", true) == 0)
	{
		if(PlayerInfo[playerid][pRope] < 1)
		{
			SendClientMessage(playerid, COLOR_GREY, "You don't have any rope, buy some from the 24/7.");
			return 1;
		}
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /tie [Playerid/PartOfName]");
			return 1;
		}
		giveplayerid = ReturnUser(tmp);
		if(IsPlayerConnected(giveplayerid))
		{
			if(giveplayerid != INVALID_PLAYER_ID)
			{
				if(giveplayerid == playerid)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't Tie yourself.");
					return 1;
				}
				if(PlayerCuffed[playerid] > 0 || PlayerTazed[playerid] > 0 || PlayerBlinded[playerid] > 0 || PlayerTied[playerid] > 0)
				{
					SendClientMessage(playerid, COLOR_GREY, "You can not tie someone up at this time.");
					return 1;
				}
				if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
				{
					new car = GetPlayerVehicleID(playerid);
					if(IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) == 2 && IsPlayerInVehicle(giveplayerid, car))
					{
						format(str, sizeof(str), "* You were tied up by %s.", PlayerICName(playerid));
						SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* You tied %s up.", PlayerICName(giveplayerid));
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* %s ties %s up, so he wont go anywhere.", PlayerICName(playerid), PlayerICName(giveplayerid));
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						displayCenterHUDInfo(giveplayerid,  "~r~Tied.", 8); //display for 8 seconds
						TogglePlayerControllable(giveplayerid, 0);
						PlayerTied[giveplayerid] = 1;
						PlayerTiedTime[giveplayerid] = 150;
						PlayerInfo[playerid][pRope]--;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "Either that player is not in your vehicle, or you're not the driver.");
						return 1;
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
					return 1;
				}
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
			return 1;
		}
		return 1;
	}
	if(strcmp(cmd, "/usecigar", true) == 0)
	{
		if(PlayerInfo[playerid][pCigars] < 1)
		{
			SendClientMessage(playerid, COLOR_GREY, "You don't have any cigars, buy some from the 24/7.");
			return 1;
		}
		if(PlayerTied[playerid] != 0 || PlayerCuffed[playerid] != 0 || PlayerTazed[playerid] != 0 || PlayerFrozen[playerid] != 0 || IsPlayerInAnyVehicle(playerid) || IsFishing[playerid] != 0 || GetPlayerAnimationIndex(playerid) == 1130)
		{
			SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
			return 1;
		}
		format(str, sizeof(str), "* %s takes out a cigar and lights it.", PlayerICName(playerid));
		ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_SMOKE_CIGGY);
		PlayerInfo[playerid][pCigars]--;
		return 1;
	}
	if(strcmp(cmd, "/usesprunk", true) == 0 || strcmp(cmd, "/usesoda", true) == 0)
	{
		if(PlayerBoxing[playerid] > 0)
		{
			SendClientMessage(playerid, COLOR_GREY, "You can't use soda whilst you're boxing.");
			return 1;
		}
		if(PlayerInfo[playerid][pSprunk] < 1)
		{
			SendClientMessage(playerid, COLOR_GREY, "You don't have any soda, buy some from a 24/7 store or a fast food business.");
			return 1;
		}
		if(PlayerTied[playerid] != 0 || PlayerCuffed[playerid] != 0 || PlayerTazed[playerid] != 0 || PlayerFrozen[playerid] != 0 || IsFishing[playerid] != 0 || GetPlayerAnimationIndex(playerid) == 1130)
		{
			SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
			return 1;
		}
		if(IsNearBank(playerid) && bankactive) return SendClientMessage(playerid, COLOR_GREY, "You can not use this inside/outside the bank during an active bank robbery.");
		format(str, sizeof(str), "* %s opens a can of soda.", PlayerICName(playerid));
		ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);
		PlayerInfo[playerid][pSprunk]--;
		return 1;
	}
	if(strcmp(cmd, "/untie", true) == 0)
	{
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /untie [Playerid/PartOfName]");
			return 1;
		}
		giveplayerid = ReturnUser(tmp);
		if(IsPlayerConnected(giveplayerid))
		{
			if(giveplayerid != INVALID_PLAYER_ID)
			{
				if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
				{
					if(giveplayerid == playerid)
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't untie yourself.");
						return 1;
					}
					if(PlayerCuffed[playerid] > 0 || PlayerTazed[playerid] > 0 || PlayerBlinded[playerid] > 0 || PlayerTied[playerid] > 0)
					{
						SendClientMessage(playerid, COLOR_GREY, "You can not untie someone at this time.");
						return 1;
					}
					if(PlayerTied[giveplayerid])
					{
						format(str, sizeof(str), "* You were untied by %s.", PlayerICName(playerid));
						SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* You untied %s.", PlayerICName(giveplayerid));
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* %s loosens the ropes on %s.", PlayerICName(playerid), PlayerICName(giveplayerid));
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						displayCenterHUDInfo(giveplayerid,  "~g~Untied.", 8); //display for 8 seconds
						TogglePlayerControllable(giveplayerid, 1);
						PlayerTied[giveplayerid] = 0;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "That player isn't Tied up.");
						return 1;
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
					return 1;
				}
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
			return 1;
		}
		return 1;
	}
	if(strcmp(cmd, "/fare", true) == 0)
	{
		if( PlayerInfo[playerid][pJailed] > 0 ) {
			return SendClientMessage( playerid, COLOR_GREY, "You cannot do this while you are imprisoned." );
		}
		if(PlayerInfo[playerid][pMember] == 10 || PlayerInfo[playerid][pLeader] == 10 || PlayerInfo[playerid][pJob] == 14)
		{
			if(UseFareTimer[playerid]) return SendClientMessage(playerid, COLOR_GREY, " You must wait 10 seconds before you can use /fare again.");
			if(TransportDuty[playerid] > 0)
			{
				if(TransportDuty[playerid] == 1)
				{
					TaxiDrivers -= 1;
				}
				else if(TransportDuty[playerid] == 2)
				{
					BusDrivers -= 1;
				}
				TransportDuty[playerid] = 0;
				format(str, sizeof(str), "* You are now off duty and earned $%d. 30 percent of your wage was given to the taxi company.", TaxiDriverMoney[playerid]);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
				GiveMoney(playerid, floatround((TaxiDriverMoney[playerid] * 70)/100));
				TAXI += floatround((TaxiDriverMoney[playerid] * 30)/100);
				SaveFactionBanks();
				TaxiDriverMoney[playerid] = 0;
				SetPlayerColor(playerid, TCOLOR_WHITE);
				return 1;
			}
			new Veh = GetPlayerVehicleID(playerid);
			if(IsATaxiCar(Veh) || PlayerInfo[playerid][pMember] == 10 && PlayerInfo[playerid][pRank] >= 4 || PlayerInfo[playerid][pLeader] == 10)
			{
				if(GetPlayerState(playerid) == 2)
				{
					tmp = strtok(cmdtext, idx);
					if(!strlen(tmp))
					{
						SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fare [price]");
						return 1;
					}
					moneys = strvalEx(tmp);
					if(moneys < 1 || moneys > 20)
					{
						SendClientMessage(playerid, COLOR_GREY, "Fare price must be between $1 and $20 per yard travelled.");
						return 1;
					}
					TaxiDrivers += 1;
					TransportDuty[playerid] = 1;
					TransportValue[playerid] = moneys;
					GetPlayerICName(playerid, sendername, sizeof(sendername));
					format(str, sizeof(str), "Taxi Driver %s is On Duty, Fare: $%d, If you need a Taxi, call 420.", sendername, TransportValue[playerid]);
					SetPlayerColor(playerid, TCOLOR_YELLOW);
					OOCNews(TEAM_GROVE_COLOR, str);
					UseFareTimer[playerid] = true;
					SetTimerEx("Timer_UseFare", 10000, 0, "i", playerid);
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You are not the Driver.");
					return 1;
				}
			}
			else if(IsABus(Veh) || PlayerInfo[playerid][pMember] == 10 && PlayerInfo[playerid][pRank] >= 4 || PlayerInfo[playerid][pLeader] == 10)
			{
				if(GetPlayerState(playerid) == 2)
				{
					tmp = strtok(cmdtext, idx);
					if(!strlen(tmp))
					{
						SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fare [price]");
						return 1;
					}
					moneys = strvalEx(tmp);
					if(moneys < 1 || moneys > 500)
					{
						SendClientMessage(playerid, COLOR_GREY, "Fare price must be between $1 and $500 for each yard travelled.");
						return 1;
					}
					BusDrivers += 1;
					TransportDuty[playerid] = 2;
					TransportValue[playerid] = moneys;
					GetPlayerICName(playerid, sendername, sizeof(sendername));
					format(str, sizeof(str), "Bus Driver %s is On Duty, fare: $%d, If you need a Bus, call 420.", sendername, TransportValue[playerid]);
					SetPlayerColor(playerid, TCOLOR_YELLOW);
					OOCNews(TEAM_GROVE_COLOR, str);
					UseFareTimer[playerid] = true;
					SetTimerEx("Timer_UseFare", 10000, 0, "i", playerid);
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You are not the Driver.");
					return 1;
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not in a Taxi / Bus, or a high enough Rank.");
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not a Taxi / Bus Driver.");
			return 1;
		}
		return 1;
	}
	/*if(strcmp(cmd, "/frisk", true) ==0)
	{
		new stringFrisk[5024];
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /frisk [playerid/PartOfName]");
			return 1;
		}
		giveplayerid = ReturnUser(tmp);
		if(IsPlayerConnected(giveplayerid))
		{
			if(giveplayerid != INVALID_PLAYER_ID)
			{
				if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
				{
					if(GetPlayerState(giveplayerid) == PLAYER_STATE_SPECTATING) return SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
					//if(playerid == giveplayerid) return SendClientMessage(playerid, COLOR_GREY, "You cannot frisk yourself.");
					if(PlayerTied[giveplayerid] || PlayerCuffed[giveplayerid] || PlayerTazed[giveplayerid])
					{
						new WeaponName[65];
						strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						SendClientMessage(playerid, COLOR_GREEN, "_______________________________________");
						format(stringFrisk, sizeof(stringFrisk), "*** %s's Items ***", giveplayer);
						SendClientMessage(playerid, COLOR_WHITE, stringFrisk);
						if(PlayerInfo[giveplayerid][pCash] > 0){
							format(stringFrisk, sizeof(stringFrisk), "Money (%d)", PlayerInfo[giveplayerid][pCash]);
							SendClientMessage(playerid, COLOR_GREY, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pCannabis] > 0) {
							format(stringFrisk, sizeof(stringFrisk), "Cannabis (%d.0g)", PlayerInfo[giveplayerid][pCannabis]);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pCocaine] > 0) {
							format(stringFrisk, sizeof(stringFrisk), "Cocaine (%d.0g)", PlayerInfo[giveplayerid][pCocaine]);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pMeth] > 0) {
							format(stringFrisk, sizeof(stringFrisk), "Meth (%d.0g)", PlayerInfo[giveplayerid][pMeth]);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pXanax] > 0) {
							format(stringFrisk, sizeof(stringFrisk), "Xanax (%d pills)", PlayerInfo[giveplayerid][pXanax]);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pPromethazine] > 0) {
							format(stringFrisk, sizeof(stringFrisk), "Promethazine (%d.0 fl oz)", PlayerInfo[giveplayerid][pPromethazine]);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pCodeine] > 0) {
							format(stringFrisk, sizeof(stringFrisk), "Codeine (%d.0 fl oz)", PlayerInfo[giveplayerid][pCodeine]);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pLean] > 0) {
							format(stringFrisk, sizeof(stringFrisk), "Mixed substance (Promethazine/Codeine) (%d.0g)", PlayerInfo[giveplayerid][pLean]);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pPotSeeds] > 0) {
							format(stringFrisk, sizeof(stringFrisk), "Seeds (%dg)", PlayerInfo[giveplayerid][pPotSeeds]);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pMats] > 0) {
							format(stringFrisk, sizeof(stringFrisk), "Materials (%d)", PlayerInfo[giveplayerid][pMats]);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						if(Packages[playerid] > 0) SendClientMessage(playerid, COLOR_BITEM, "Materials Packages");
						if(Crates[playerid] > 0) SendClientMessage(playerid, COLOR_BITEM, "Drug Crates");
						if(Fishes[giveplayerid][pWeight1] > 0 || Fishes[giveplayerid][pWeight2] > 0 || Fishes[giveplayerid][pWeight3] > 0 || Fishes[giveplayerid][pWeight4] > 0 || Fishes[giveplayerid][pWeight5] > 0) SendClientMessage(playerid, COLOR_GRAD3, "Fish");
						if(PlayerInfo[giveplayerid][pPhoneBook] > 0) SendClientMessage(playerid, COLOR_GRAD3, "Phone Book");
						if(PlayerInfo[giveplayerid][piPod] > 0) SendClientMessage(playerid, COLOR_GRAD3, "iPod");
						if(PlayerInfo[giveplayerid][pPnumber] > 0) SendClientMessage(playerid, COLOR_GRAD3, "Cellphone");
						if(PlayerInfo[giveplayerid][pSpraycan] > 0) {
							format(stringFrisk, sizeof(stringFrisk), "Spraycans (%d)", PlayerInfo[giveplayerid][pSpraycan]);
							SendClientMessage(playerid, COLOR_GREY, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pScrew] > 0) {
							format(stringFrisk, sizeof(stringFrisk), "Screwdrivers (%d)", PlayerInfo[giveplayerid][pScrew]);
							SendClientMessage(playerid, COLOR_GREY, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pDice] > 0) SendClientMessage(playerid, COLOR_GRAD3, "Dice");
						if(PlayerInfo[giveplayerid][pRope] > 0) {
							format(stringFrisk, sizeof(stringFrisk), "Rope (%d)", PlayerInfo[giveplayerid][pRope]);
							SendClientMessage(playerid, COLOR_GREY, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pGag] > 0) {
							format(stringFrisk, sizeof(stringFrisk), "Gag (%d)", PlayerInfo[giveplayerid][pGag]);
							SendClientMessage(playerid, COLOR_GREY, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pCigars] > 0) {
							format(stringFrisk, sizeof(stringFrisk), "Cigars (%d)", PlayerInfo[giveplayerid][pCigars]);
							SendClientMessage(playerid, COLOR_GREY, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pSprunk] > 0) {
							format(stringFrisk, sizeof(stringFrisk), "Sprunk (%d)", PlayerInfo[giveplayerid][pSprunk]);
							SendClientMessage(playerid, COLOR_GREY, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pBombs] > 0) SendClientMessage(playerid, COLOR_BITEM, "C4 Explosives");
						//if(PlayerInfo[giveplayerid][pPoison] > 0) SendClientMessage(playerid, COLOR_BITEM, "Poison");
						if(PlayerInfo[giveplayerid][pWire] > 0) SendClientMessage(playerid, COLOR_BITEM, "Wire");
						if(PlayerInfo[giveplayerid][pJammer] > 0) SendClientMessage(playerid, COLOR_BITEM, "Signal Jammer");
						if(PlayerInfo[giveplayerid][pBlindfolds] > 0) {
							format(stringFrisk, sizeof(stringFrisk), "Blindfolds (%d)", PlayerInfo[giveplayerid][pBlindfolds]);
							SendClientMessage(playerid, COLOR_GREY, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pGun0] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun0], WeaponName, 64);
							format(stringFrisk, sizeof(stringFrisk), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pPortMDC] != 0)
						{
							SendClientMessage(playerid, COLOR_BITEM, "Portable MDC");
						}
						if(PlayerInfo[giveplayerid][pGun1] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun1], WeaponName, 64);
							format(stringFrisk, sizeof(stringFrisk), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pGun2] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun2], WeaponName, 64);
							format(stringFrisk, sizeof(stringFrisk), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pGun3] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun3], WeaponName, 64);
							format(stringFrisk, sizeof(stringFrisk), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pGun4] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun4], WeaponName, 64);
							format(stringFrisk, sizeof(stringFrisk), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pGun5] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun5], WeaponName, 64);
							format(stringFrisk, sizeof(stringFrisk), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pGun6] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun6], WeaponName, 64);
							format(stringFrisk, sizeof(stringFrisk), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pGun7] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun7], WeaponName, 64);
							format(stringFrisk, sizeof(stringFrisk), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pGun8] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun8], WeaponName, 64);
							format(stringFrisk, sizeof(stringFrisk), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pGun9] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun9], WeaponName, 64);
							format(stringFrisk, sizeof(stringFrisk), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pGun10] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun10], WeaponName, 64);
							format(stringFrisk, sizeof(stringFrisk), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						if(PlayerInfo[giveplayerid][pGun11] == 44)
						{
							SendClientMessage(playerid, COLOR_GRAD3, "Nightvision Goggles");
						}
						if(PlayerInfo[giveplayerid][pGun11] == 45)
						{
							SendClientMessage(playerid, COLOR_GRAD3, "Infared Goggles");
						}
						if(PlayerInfo[giveplayerid][pGun12] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun12], WeaponName, 64);
							format(stringFrisk, sizeof(stringFrisk), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, stringFrisk);
						}
						for(new weap = 1; weap < 47; weap++)
						{
							if(HaveAdminWeapon(giveplayerid, weap) == weap)
							{
								GetWeaponName(weap, WeaponName, 64);
								format(stringFrisk, sizeof(stringFrisk), "{FF0000}Admin Given %s", WeaponName);
								SendClientMessage(playerid, COLOR_WHITE, stringFrisk);
							}
						}
						if(Crates[giveplayerid] == 1)
						{
							SendClientMessage(playerid, COLOR_BITEM, "Drugs Crate");
						}
						if(PlayerInfo[giveplayerid][pHasCBomb])
						{
							SendClientMessage(playerid, COLOR_BITEM, "C2 explosives");	
						}
						LastFriskedPlayer[playerid] = giveplayerid;
						format(stringFrisk, sizeof(stringFrisk), "* %s has frisked %s.", PlayerICName(playerid), PlayerICName(giveplayerid));
						ProxDetector(30.0, playerid, stringFrisk, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					}
					else
					{
						format(stringFrisk, sizeof(stringFrisk), "* You offered to frisk %s.", PlayerICName(giveplayerid));
						SendClientMessage(playerid, COLOR_LIGHTBLUE, stringFrisk);
						format(stringFrisk, sizeof(stringFrisk), "* %s wants to frisk you, (type /accept frisk) to accept.", PlayerICName(playerid));
						SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, stringFrisk);
						FriskOffer[giveplayerid] = playerid;
					}
				}
				else return SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
			}
		}
		else return SendClientMessage(playerid, COLOR_GREY, "That player is offline.");
		return 1;
	}*/
	if(strcmp(cmd, "/frisk", true) ==0)
	{
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /frisk [playerid/PartOfName]");
			return 1;
		}
		giveplayerid = ReturnUser(tmp);
		if(IsPlayerConnected(giveplayerid))
		{
			if(giveplayerid != INVALID_PLAYER_ID)
			{
				if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
				{
					if(GetPlayerState(giveplayerid) == PLAYER_STATE_SPECTATING) return SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
					//if(playerid == giveplayerid) return SendClientMessage(playerid, COLOR_GREY, "You cannot frisk yourself.");
					if(PlayerTied[giveplayerid] || PlayerCuffed[giveplayerid] || PlayerTazed[giveplayerid])
					{
						new WeaponName[65];
						strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						SendClientMessage(playerid, COLOR_GREEN, "_______________________________________");
						format(str, sizeof(str), "*** %s's Items ***", giveplayer);
						SendClientMessage(playerid, COLOR_WHITE, str);
						if(PlayerInfo[giveplayerid][pCash] > 0){
							format(str, sizeof(str), "Money (%d)", PlayerInfo[giveplayerid][pCash]);
							SendClientMessage(playerid, COLOR_GREY, str);
						}
						if(PlayerInfo[giveplayerid][pCannabis] > 0) {
							format(str, sizeof(str), "Cannabis (%d.0g)", PlayerInfo[giveplayerid][pCannabis]);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pCocaine] > 0) {
							format(str, sizeof(str), "Cocaine (%d.0g)", PlayerInfo[giveplayerid][pCocaine]);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pMeth] > 0) {
							format(str, sizeof(str), "Meth (%d.0g)", PlayerInfo[giveplayerid][pMeth]);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pXanax] > 0) {
							format(str, sizeof(str), "Xanax (%d pills)", PlayerInfo[giveplayerid][pXanax]);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pPromethazine] > 0) {
							format(str, sizeof(str), "Promethazine (%d.0 fl oz)", PlayerInfo[giveplayerid][pPromethazine]);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pCodeine] > 0) {
							format(str, sizeof(str), "Codeine (%d.0 fl oz)", PlayerInfo[giveplayerid][pCodeine]);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pLean] > 0) {
							format(str, sizeof(str), "Mixed substance (Promethazine/Codeine) (%d.0g)", PlayerInfo[giveplayerid][pLean]);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pPotSeeds] > 0) {
							format(str, sizeof(str), "Seeds (%dg)", PlayerInfo[giveplayerid][pPotSeeds]);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pMats] > 0) {
							format(str, sizeof(str), "Materials (%d)", PlayerInfo[giveplayerid][pMats]);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						if(Packages[giveplayerid] > 0) SendClientMessage(playerid, COLOR_BITEM, "Materials Packages");
						if(Crates[giveplayerid] > 0) SendClientMessage(playerid, COLOR_BITEM, "Drug Crates");
						if(Fishes[giveplayerid][pWeight1] > 0 || Fishes[giveplayerid][pWeight2] > 0 || Fishes[giveplayerid][pWeight3] > 0 || Fishes[giveplayerid][pWeight4] > 0 || Fishes[giveplayerid][pWeight5] > 0) SendClientMessage(playerid, COLOR_GRAD3, "Fish");
						if(PlayerInfo[giveplayerid][pPhoneBook] > 0) SendClientMessage(playerid, COLOR_GRAD3, "Phone Book");
						if(PlayerInfo[giveplayerid][piPod] > 0) SendClientMessage(playerid, COLOR_GRAD3, "iPod");
						if(PlayerInfo[giveplayerid][pPnumber] > 0) SendClientMessage(playerid, COLOR_GRAD3, "Cellphone");
						if(PlayerInfo[giveplayerid][pSpraycan] > 0) {
							format(str, sizeof(str), "Spraycans (%d)", PlayerInfo[giveplayerid][pSpraycan]);
							SendClientMessage(playerid, COLOR_GREY, str);
						}
						if(PlayerInfo[giveplayerid][pScrew] > 0) {
							format(str, sizeof(str), "Screwdrivers (%d)", PlayerInfo[giveplayerid][pScrew]);
							SendClientMessage(playerid, COLOR_GREY, str);
						}
						if(PlayerInfo[giveplayerid][pDice] > 0) SendClientMessage(playerid, COLOR_GRAD3, "Dice");
						if(PlayerInfo[giveplayerid][pRope] > 0) {
							format(str, sizeof(str), "Rope (%d)", PlayerInfo[giveplayerid][pRope]);
							SendClientMessage(playerid, COLOR_GREY, str);
						}
						if(PlayerInfo[giveplayerid][pGag] > 0) {
							format(str, sizeof(str), "Gag (%d)", PlayerInfo[giveplayerid][pGag]);
							SendClientMessage(playerid, COLOR_GREY, str);
						}
						if(PlayerInfo[giveplayerid][pCigars] > 0) {
							format(str, sizeof(str), "Cigars (%d)", PlayerInfo[giveplayerid][pCigars]);
							SendClientMessage(playerid, COLOR_GREY, str);
						}
						if(PlayerInfo[giveplayerid][pSprunk] > 0) {
							format(str, sizeof(str), "Sprunk (%d)", PlayerInfo[giveplayerid][pSprunk]);
							SendClientMessage(playerid, COLOR_GREY, str);
						}
						if(PlayerInfo[giveplayerid][pBombs] > 0) SendClientMessage(playerid, COLOR_BITEM, "C4 Explosives");
						//if(PlayerInfo[giveplayerid][pPoison] > 0) SendClientMessage(playerid, COLOR_BITEM, "Poison");
						if(PlayerInfo[giveplayerid][pWire] > 0) SendClientMessage(playerid, COLOR_BITEM, "Wire");
						if(PlayerInfo[giveplayerid][pJammer] > 0) SendClientMessage(playerid, COLOR_BITEM, "Signal Jammer");
						if(PlayerInfo[giveplayerid][pBlindfolds] > 0) {
							format(str, sizeof(str), "Blindfolds (%d)", PlayerInfo[giveplayerid][pBlindfolds]);
							SendClientMessage(playerid, COLOR_GREY, str);
						}
						if(PlayerInfo[giveplayerid][pGun0] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun0], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pPortMDC] != 0)
						{
							SendClientMessage(playerid, COLOR_BITEM, "Portable MDC");
						}
						if(PlayerInfo[giveplayerid][pGun1] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun1], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pGun2] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun2], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pGun3] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun3], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pGun4] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun4], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pGun5] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun5], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pGun6] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun6], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pGun7] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun7], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pGun8] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun8], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pGun9] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun9], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pGun10] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun10], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pGun11] == 44)
						{
							SendClientMessage(playerid, COLOR_GRAD3, "Nightvision Goggles");
						}
						if(PlayerInfo[giveplayerid][pGun11] == 45)
						{
							SendClientMessage(playerid, COLOR_GRAD3, "Infared Goggles");
						}
						if(PlayerInfo[giveplayerid][pGun12] != 0)
						{
							GetWeaponName(PlayerInfo[giveplayerid][pGun12], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(playerid, COLOR_BITEM, str);
						}
						for(new weap = 1; weap < 47; weap++)
						{
							if(HaveAdminWeapon(giveplayerid, weap) == weap)
							{
								GetWeaponName(weap, WeaponName, 64);
								format(str, sizeof(str), "{FF0000}Admin Given %s", WeaponName);
								SendClientMessage(playerid, COLOR_WHITE, str);
							}
						}
						if(Crates[giveplayerid] == 1)
						{
							SendClientMessage(playerid, COLOR_BITEM, "Drugs Crate");
						}
						if(PlayerInfo[giveplayerid][pHasCBomb])
						{
							SendClientMessage(playerid, COLOR_BITEM, "C2 explosives");	
						}
						LastFriskedPlayer[playerid] = giveplayerid;
						format(str, sizeof(str), "* %s has frisked %s.", PlayerICName(playerid), PlayerICName(giveplayerid));
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					}
					else
					{
						format(str, sizeof(str), "* You offered to frisk %s.", PlayerICName(giveplayerid));
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* %s wants to frisk you, (type /accept frisk) to accept.", PlayerICName(playerid));
						SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
						FriskOffer[giveplayerid] = playerid;
					}
				}
				else return SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
			}
		}
		else return SendClientMessage(playerid, COLOR_GREY, "That player is offline.");
		return 1;
	}
	if(strcmp(cmd, "/sellgun", true) == 0)
	{
		if(PlayerInfo[playerid][pJailed] != 0) return SendClientMessage(playerid, COLOR_GREY, "You can not sell weapons while in jail!");
		if(PlayerInfo[playerid][pJob] != 9 && PlayerInfo[playerid][pGroup] != GROUP_CRIME) return SendClientMessage(playerid, COLOR_GREY, "You are not a Arms Dealer!");
		if(PlayerCuffed[playerid] == 1)return SendClientMessage(playerid, COLOR_GREY, "You can't do this whilst cuffed.");
		if(GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK) return SendClientMessage(playerid, COLOR_GREY, "You cannot use this at this time");

		if(IsPlayerInAnyVehicle(playerid))return SendClientMessage(playerid, COLOR_GREY, "You can't use this whilst inside of a vehicle.");
		if(GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK) return SendClientMessage(playerid, COLOR_GREY, "You are not allowed to use the command now.");

		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendWeaponMessage(playerid);
			return 1;
		}
		giveplayerid = ReturnUser(tmp);
		if(IsPlayerConnected(giveplayerid))
		{
			if(giveplayerid != INVALID_PLAYER_ID)
			{
				tmp = strtok(cmdtext, idx);
				if(!strlen(tmp))
				{
					SendWeaponMessage(playerid);
					return 1;
				}
			}
			if(SellGunTimer[playerid] == 1 && PlayerInfo[playerid][pAdmin] < 2)
				return SendClientMessage(playerid, COLOR_GREY, "You must wait 1 minute before selling another weapon !");
			if(IsPlayerInRangeOfPlayer(5.0, playerid, giveplayerid))
			{
				if(IsPlayerInAnyVehicle(giveplayerid))return SendClientMessage(playerid, COLOR_GREY, "You can't sell a vehicle to them whilst they are driving.");

				/*if(GetPlayerVehicleSeat(giveplayerid) == 0) {
					return SendClientMessage(playerid, COLOR_GRAD2, "  You can not sell a weapon to someone while they are driving !");
				}*/
				new weapon, price, loggun;
				if(strcmp(tmp, "knuckles", true) == 0)
				{
					if(PlayerInfo[playerid][pMats] < 25) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					weapon = 1;
					price = 25;
				}
				else if(strcmp(tmp, "golfclub", true) == 0)
				{
					if(PlayerInfo[playerid][pMats] < 25) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					weapon = 2;
					price = 25;
				}
				else if(strcmp(tmp, "baseballbat", true) == 0)
				{
					if(PlayerInfo[playerid][pMats] < 25) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					weapon = 5;
					price = 25;
				}
				else if(strcmp(tmp, "shovel", true) == 0)
				{
					if(PlayerInfo[playerid][pMats] < 25) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					weapon = 6;
					price = 25;
				}
				else if(strcmp(tmp, "poolcue", true) == 0)
				{
					if(PlayerInfo[playerid][pMats] < 25) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					weapon = 7;
					price = 25;
				}
				else if(strcmp(tmp, "katana", true) == 0)
				{
					if(PlayerInfo[playerid][pMats] < 50) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					weapon = 8;
					price = 50;
				}
				else if(strcmp(tmp, "dildo", true) == 0)
				{
					if(PlayerInfo[playerid][pMats] < 25) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					weapon = 10;
					price = 25;
				}
				else if(strcmp(tmp, "flowers", true) == 0)
				{
					if(PlayerInfo[playerid][pMats] < 25) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					weapon = 14;
					price = 25;
				}
				else if(strcmp(tmp, "cane", true) == 0)
				{
					if(PlayerInfo[playerid][pMats] < 25) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					weapon = 15;
					price = 25;
				}
				else if(strcmp(tmp, "parachute", true) == 0)
				{
					if(PlayerInfo[playerid][pMats] < 25) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					weapon = 46;
					price = 25;
				}
				else if(strcmp(tmp, "colt45", true) == 0)
				{
					if(giveplayerid == playerid && PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can get a weapon."); //selling to yourself
					if(PlayerInfo[giveplayerid][pConnectTime] < 2 && PlayerInfo[playerid][pAdmin] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "That player must be playing for at least 2 hours before receiving a weapon."); //selling to another

					if(PlayerInfo[playerid][pMats] < 250) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					if(PlayerInfo[giveplayerid][pGun2] == 22 || PlayerInfo[giveplayerid][pGun2] == 24) return SendClientMessage(playerid, COLOR_GREY, "That player is already carrying a weapon in that slot !");
					weapon = 22;
					price = 250;
				}
				else if(strcmp(tmp, "shotgun", true) == 0)
				{
					if(giveplayerid == playerid && PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can get a weapon."); //selling to yourself
					if(PlayerInfo[giveplayerid][pConnectTime] < 2 && PlayerInfo[playerid][pAdmin] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "That player must be playing for at least 2 hours before receiving a weapon."); //selling to another

					new level = PlayerInfo[playerid][pArmsSkill];
					if(level < 50) return SendClientMessage(playerid, COLOR_GREY, "You are not the required level to create that !");
					if(PlayerInfo[playerid][pMats] < 500) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					if(PlayerInfo[giveplayerid][pGun3] == 27) return SendClientMessage(playerid, COLOR_GREY, "That player is already carrying a weapon in that slot !");
					weapon = 25;
					price = 500;
				}
				else if(strcmp(tmp, "rifle", true) == 0)
				{
					if(giveplayerid == playerid && PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can get a weapon."); //selling to yourself
					if(PlayerInfo[giveplayerid][pConnectTime] < 2 && PlayerInfo[playerid][pAdmin] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "That player must be playing for at least 2 hours before receiving a weapon."); //selling to another

					new level = PlayerInfo[playerid][pArmsSkill];
					if(level < 50) return SendClientMessage(playerid, COLOR_GREY, "You are not the required level to create that !");
					if(PlayerInfo[playerid][pMats] < 1000) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					if(PlayerInfo[giveplayerid][pGun6] == 34) return SendClientMessage(playerid, COLOR_GREY, "That player is already carrying a weapon in that slot !");
					weapon = 33;
					price = 1000;
				}
				else if(strcmp(tmp, "mp5", true) == 0)
				{
					if(giveplayerid == playerid && PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can get a weapon."); //selling to yourself
					if(PlayerInfo[giveplayerid][pConnectTime] < 2 && PlayerInfo[playerid][pAdmin] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "That player must be playing for at least 2 hours before receiving a weapon."); //selling to another

					new level = PlayerInfo[playerid][pArmsSkill];
					if(level < 100) return SendClientMessage(playerid, COLOR_GREY, "You are not the required level to create that !");
					if(PlayerInfo[playerid][pMats] < 2000) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					weapon = 29;
					price = 2000;
				}
				else if(strcmp(tmp, "uzi", true) == 0)
				{
					if(giveplayerid == playerid && PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can get a weapon."); //selling to yourself
					if(PlayerInfo[giveplayerid][pConnectTime] < 2 && PlayerInfo[playerid][pAdmin] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "That player must be playing for at least 2 hours before receiving a weapon."); //selling to another

					new level = PlayerInfo[playerid][pArmsSkill];
					if(level < 100) return SendClientMessage(playerid, COLOR_GREY, "You are not the required level to create that !");
					if(PlayerInfo[playerid][pMats] < 1500) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					weapon = 28;
					price = 1500;
				}
				else if(strcmp(tmp, "tec9", true) == 0)
				{
					if(giveplayerid == playerid && PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can get a weapon."); //selling to yourself
					if(PlayerInfo[giveplayerid][pConnectTime] < 2 && PlayerInfo[playerid][pAdmin] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "That player must be playing for at least 2 hours before receiving a weapon."); //selling to another

					new level = PlayerInfo[playerid][pArmsSkill];
					if(level < 100) return SendClientMessage(playerid, COLOR_GREY, "You are not the required level to create that !");
					if(PlayerInfo[playerid][pMats] < 1500) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					weapon = 32;
					price = 1500;
				}
				else if(strcmp(tmp, "deagle", true) == 0)
				{
					if(giveplayerid == playerid && PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can get a weapon."); //selling to yourself
					if(PlayerInfo[giveplayerid][pConnectTime] < 2 && PlayerInfo[playerid][pAdmin] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "That player must be playing for at least 2 hours before receiving a weapon."); //selling to another

					new level = PlayerInfo[playerid][pArmsSkill];
					if(level < 200) return SendClientMessage(playerid, COLOR_GREY, "You are not the required level to create that !");
					if(PlayerInfo[playerid][pMats] < 4000) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					weapon = 24;
					price = 4000;
				}
				else if(strcmp(tmp, "sdpistol", true) == 0)
				{
					if(giveplayerid == playerid && PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can get a weapon."); //selling to yourself
					if(PlayerInfo[giveplayerid][pConnectTime] < 2 && PlayerInfo[playerid][pAdmin] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "That player must be playing for at least 2 hours before receiving a weapon."); //selling to another

					new level = PlayerInfo[playerid][pArmsSkill];
					if(level < 200) return SendClientMessage(playerid, COLOR_GREY, "You are not the required level to create that !");
					if(PlayerInfo[playerid][pMats] < 2000) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					weapon = 23;
					price = 2000;
				}
				else if(strcmp(tmp, "sawnoff", true) == 0)
				{
					if(giveplayerid == playerid && PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can get a weapon."); //selling to yourself
					if(PlayerInfo[giveplayerid][pConnectTime] < 2 && PlayerInfo[playerid][pAdmin] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "That player must be playing for at least 2 hours before receiving a weapon."); //selling to another

					new level = PlayerInfo[playerid][pArmsSkill];
					if(level < 200) return SendClientMessage(playerid, COLOR_GREY, "You are not the required level to create that !");
					if(PlayerInfo[playerid][pMats] < 3500) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					weapon = 26;
					price = 3500;
				}
				else if(strcmp(tmp, "ak47", true) == 0)
				{
					if(giveplayerid == playerid && PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can get a weapon."); //selling to yourself
					if(PlayerInfo[giveplayerid][pConnectTime] < 2 && PlayerInfo[playerid][pAdmin] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "That player must be playing for at least 2 hours before receiving a weapon."); //selling to another

					new level = PlayerInfo[playerid][pArmsSkill];
					if(level < 400) return SendClientMessage(playerid, COLOR_GREY, "You are not the required level to create that !");
					if(PlayerInfo[playerid][pMats] < 8000) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					if(PlayerInfo[giveplayerid][pGun5] == 31) return SendClientMessage(playerid, COLOR_GREY, "That player is already carrying a weapon in that slot !");
					weapon = 30;
					price = 8000;
				}
				else if(PlayerInfo[playerid][pFMember] != 255) {
					if(strcmp(tmp, "m4", true) == 0)
					{
						if(giveplayerid == playerid && PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can get a weapon."); //selling to yourself
						if(PlayerInfo[giveplayerid][pConnectTime] < 2 && PlayerInfo[playerid][pAdmin] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "That player must be playing for at least 2 hours before receiving a weapon."); //selling to another

						new level = PlayerInfo[playerid][pArmsSkill];
						if(level < 400) return SendClientMessage(playerid, COLOR_GREY, "You are not the required level to create that !");
						if(PlayerInfo[playerid][pMats] < 8500) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
						weapon = 31;
						price = 8500;
					}
					else if(strcmp(tmp, "sniper", true) == 0)
					{
						if(giveplayerid == playerid && PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can get a weapon."); //selling to yourself
						if(PlayerInfo[giveplayerid][pConnectTime] < 2 && PlayerInfo[playerid][pAdmin] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "That player must be playing for at least 2 hours before receiving a weapon."); //selling to another

						// Still requires level 4 arms dealer to craft this
						new level = PlayerInfo[playerid][pArmsSkill];
						if(level < 400) return SendClientMessage(playerid, COLOR_GREY, "You are not the required level to create that !");
						if(PlayerInfo[playerid][pMats] < 10000) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
						weapon = 34;
						price = 10000;
					}
					else // bug fix for fam members being able to give any input. the script will check if they typed all of the weapons above and then if not, if they are in a family..
					{ // -.. then they could make m4 or sniper. however, where is the else for that? there is no else, so the code continues below and does the part of "You have received a WEAPONNAME from SELLER"
						SendClientMessage(playerid, COLOR_GREY, "Invalid weapon name !");
						return 1;
					}
				}
				/*
				else if(strcmp(tmp, "spas12", true) == 0)
				{
					if(giveplayerid == playerid && PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can get a weapon."); //selling to yourself
					if(PlayerInfo[giveplayerid][pConnectTime] < 2 && PlayerInfo[playerid][pAdmin] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "That player must be playing for at least 2 hours before receiving a weapon."); //selling to another

					new level = PlayerInfo[playerid][pArmsSkill];
					if(level < 400) return SendClientMessage(playerid, COLOR_GREY, "You are not the required level to create that !");
					if(PlayerInfo[playerid][pMats] < 10000) return SendClientMessage(playerid, COLOR_GREY, "Not enough Materials for that Weapon !");
					weapon = 27;
					price = 10000;
				}*/
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "Invalid weapon name !");
					return 1;
				}
				if(weapon != 0) //sell gun
				{
					GivePlayerGun(giveplayerid, weapon);
				}
				if(weapon ==27 ||weapon ==34 ||weapon ==31 ||weapon ==30 ||weapon ==24 ||weapon ==33 ||weapon ==29 ||weapon ==25 ||weapon ==22) //increase level
				{
					if(weapon != 23 || weapon != 25)
					{
						loggun = 1;
					}
					PlayerInfo[playerid][pArmsSkill]++;
				}
				strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
				strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
				PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
				PlayerPlaySound(giveplayerid, 1052, 0.0, 0.0, 0.0);
				format(str, sizeof(str), "You have received a %s from %s.", tmp, sendername);
				SendClientMessage(giveplayerid, COLOR_GRAD1, str);
				format(str, sizeof(str), "* %s creates a weapon from materials and hands it to %s.", sendername, giveplayer);
				ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				if(loggun == 1)
				{
					format(str, sizeof(str), "%s has sold a %s to %s", sendername, tmp, giveplayer);
					PayLog(str);
				}
				PlayerInfo[playerid][pMats] -= price;
				if(PlayerInfo[playerid][pAdmin] < 2)
				{
					SellGunTimer[playerid] = 1;
					SetTimerEx("Timer_SellGun", 60 *1000, 0, "i", playerid);
				}
				if(PlayerInfo[playerid][pArmsSkill] == 50)
				{
					SendClientMessage(playerid, COLOR_YELLOW, "* Your Arms Dealer Skill is now Level 2, you have unlocked the shotgun and the rifle.");
				}
				else if(PlayerInfo[playerid][pArmsSkill] == 100)
				{
					SendClientMessage(playerid, COLOR_YELLOW, "* Your Arms Dealer Skill is now Level 3, you have unlocked the TEC9, UZI and MP5.");
				}
				else if(PlayerInfo[playerid][pArmsSkill] == 200)
				{
					SendClientMessage(playerid, COLOR_YELLOW, "* Your Arms Dealer Skill is now Level 4, you have unlocked the Sawn-Off shotgun and the Desert Eagle.");
				}
				else if(PlayerInfo[playerid][pArmsSkill] == 400)
				{
					SendClientMessage(playerid, COLOR_YELLOW, "* Your Arms Dealer Skill is now Level 5, you have unlocked the AK47.");
				}
				// if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
				// {
				// SetPlayerArmedWeapon(playerid, 0);
				// }
				// if(GetPlayerVehicleSeat(giveplayerid) == 0 || GetPlayerVehicleSeat(giveplayerid) == 128)
				// {
				// SetPlayerArmedWeapon(giveplayerid, 0);
				// }
				if(IsPlayerInAnyVehicle(giveplayerid))
				{
					if(GetPlayerVehicleSeat(giveplayerid) == 0)
					{
						SetPlayerArmedWeapon(giveplayerid, 0);
						//return SendClientMessage(playerid, COLOR_GRAD2, "  You can not sell a weapon to someone while they are driving !");
					}
					if(weapon != 29 && weapon != 22)
					{
						SetPlayerArmedWeapon(giveplayerid, 0);
					}
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "You are too far away !");
				return 1;
			}
		}
		else
		{
			format(str, sizeof(str), "That player is offline.");
			SendClientMessage(playerid, COLOR_GRAD1, str);
		}
		return 1;
	}
	if(strcmp(cmd, "/getseeds", true) == 0)
	{
		if(PlayerInfo[playerid][pJob] != 4)
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not a Drug Dealer.");
			return 1;
		}
		if(!IsPlayerInRangeOfPoint(playerid, 2.0, 323.0342, 1118.5804, 1083.8828))
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not at the Drug House.");
			return 1;
		}
		if(PlayerInfo[playerid][pPotSeeds] > 49)
		{
			format(str, sizeof(str), "You still have %d seeds with you, sell or /drop them first.", PlayerInfo[playerid][pPotSeeds]);
			SendClientMessage(playerid, COLOR_GREY, str);
			return 1;
		}
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /getseeds [amount]");
			return 1;
		}
		new price;
		new ammount;
		new level = PlayerInfo[playerid][pDrugsSkill];
		new drugs = PlayerInfo[playerid][pPotSeeds];
		ammount = strvalEx(tmp);
		if(ammount > dhstock)
		{
			SendClientMessage(playerid, COLOR_GREY, "The Drug House doesn't have that much seeds at this time.");
			return 1;
		}
		if(level >= 0 && level <= 49)
		{
			if(drugs > 9)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't carry any more seeds.");
				return 1;
			}
			if(ammount < 1 || ammount > 10)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't purchase more than 10 at your current skill level.");
				return 1;
			}
			if(drugs + ammount > 10)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't carry more than 10 at your current skill level.");
				return 1;
			}
		}
		else if(level >= 50 && level <= 99)
		{
			if(drugs > 19)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't carry any more seeds.");
				return 1;
			}
			if(ammount < 1 || ammount > 20)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't purchase more than 20 at your current skill level.");
				return 1;
			}
			if(drugs + ammount > 20)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't carry more than 20 at your current skill level.");
				return 1;
			}
		}
		else if(level >= 100 && level <= 199)
		{
			if(drugs > 29)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't carry any more seeds.");
				return 1;
			}
			if(ammount < 1 || ammount > 30)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't purchase more than 30 at your current skill level.");
				return 1;
			}
			if(drugs + ammount > 30)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't carry more than 30 at your current skill level.");
				return 1;
			}
		}
		else if(level >= 200 && level <= 399)
		{
			if(drugs > 39)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't carry any more seeds.");
				return 1;
			}
			if(ammount < 1 || ammount > 40)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't purchase more than 40 at your current skill level.");
				return 1;
			}
			if(drugs + ammount > 40)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't carry more than 40 at your current skill level.");
				return 1;
			}
		}
		else if(level >= 400)
		{
			if(drugs > 49)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't carry any more seeds.");
				return 1;
			}
			if(ammount < 1 || ammount > 50)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't purchase more than 50 at your current skill level.");
				return 1;
			}
			if(drugs + ammount > 50)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't carry more than 50 at your current skill level.");
				return 1;
			}
		}
		price = ammount * seedprice;
		if(PlayerInfo[playerid][pCash] >= price)
		{
			format(str, sizeof(str), "* You bought %d seeds for $%d.", ammount, price);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			GiveMoney(playerid, -price);
			PlayerInfo[playerid][pPotSeeds] = ammount + drugs;
			dhstock = dhstock -ammount;
			PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
			return 1;
		}
		return 1;
	}
	if(strcmp(cmd, "/getcrack", true) == 0)
	{
		if(PlayerInfo[playerid][pJob] != 4)
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not a Drug Dealer.");
			return 1;
		}
		if(!IsPlayerInRangeOfPoint(playerid, 2.0, 2346.2937, -1185.2551, 1027.9766))
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not at the Crack Lab.");
			return 1;
		}
		if(PlayerInfo[playerid][pCocaine] > 24)
		{
			format(str, sizeof(str), "You still have %d grams of crack with you, sell or /drop them first.", PlayerInfo[playerid][pCocaine]);
			SendClientMessage(playerid, COLOR_GREY, str);
			return 1;
		}
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /getcrack [amount]");
			return 1;
		}
		new price;
		new ammount;
		new level = PlayerInfo[playerid][pDrugsSkill];
		new drugs = PlayerInfo[playerid][pCocaine];
		ammount = strvalEx(tmp);
		if(ammount > chstock)
		{
			SendClientMessage(playerid, COLOR_GREY, "The Crack Lab doesn't have that much crack at this time.");
			return 1;
		}
		if(level >= 0 && level <= 49)
		{
			if(drugs > 4)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't carry any more crack.");
				return 1;
			}
			if(ammount < 1 || ammount > 5)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't purchase more than 5 at your current skill level.");
				return 1;
			}
			if(drugs + ammount > 5)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't carry more than 5 at your current skill level.");
				return 1;
			}
		}
		else if(level >= 50 && level <= 99)
		{
			if(drugs > 9)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't carry any more crack.");
				return 1;
			}
			if(ammount < 1 || ammount > 10)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't purchase more than 10 at your current skill level.");
				return 1;
			}
			if(drugs + ammount > 10)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't carry more than 10 at your current skill level.");
				return 1;
			}
		}
		else if(level >= 100 && level <= 199)
		{
			if(drugs > 14)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't carry any more crack.");
				return 1;
			}
			if(ammount < 1 || ammount > 15)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't purchase more than 15 at your current skill level.");
				return 1;
			}
			if(drugs + ammount > 15)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't carry more than 15 at your current skill level.");
				return 1;
			}
		}
		else if(level >= 200 && level <= 399)
		{
			if(drugs > 19)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't carry any more crack.");
				return 1;
			}
			if(ammount < 1 || ammount > 20)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't purchase more than 20 at your current skill level.");
				return 1;
			}
			if(drugs + ammount > 20)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't carry more than 20 at your current skill level.");
				return 1;
			}
		}
		else if(level >= 400)
		{
			if(drugs > 24)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't carry any more crack.");
				return 1;
			}
			if(ammount < 1 || ammount > 25)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't purchase more than 25 at your current skill level.");
				return 1;
			}
			if(drugs + ammount > 25)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't carry more than 25 at your current skill level.");
				return 1;
			}
		}
		price = ammount * crackgprice;
		if(PlayerInfo[playerid][pCash] > price)
		{
			format(str, sizeof(str), "* You bought %d grams of crack for $%d.", ammount, price);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			GiveMoney(playerid, -price);
			PlayerInfo[playerid][pCocaine] = ammount + drugs;
			chstock = chstock -ammount;
			PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
			return 1;
		}
		return 1;
	}

	if(strcmp(cmd, "/fill", true) == 0)
	{
		if(IsAtGasStation(playerid))
		{
			if(engineOn[GetPlayerVehicleID(playerid)] == 0)
			{
				if(Gas[GetPlayerVehicleID(playerid)] >= 99) return SendClientMessage(playerid, COLOR_WHITE, "This vehicle doesn't need filling.");
				displayCenterHUDInfo(playerid,  "~w~Refueling vehicle,~n~~r~please wait...", 8); //display for 8 seconds
				SetTimerEx("Timer_Fillup", 5000, 0, "i", playerid);
				Refueling[playerid] = 1;
			}
			else return SendClientMessage(playerid, COLOR_GREY, "You must turn off your engine first ! (/engine)");
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not at a Gas Station.");
		}
		return 1;
	}
	if(strcmp(cmd, "/detain", true) == 0)
	{
		if(IsACop(playerid) || IsANG(playerid) || PlayerInfo[playerid][pMember] == FACTION_SANG)
		{
			if(IsPlayerInAnyVehicle(playerid))
			{
				SendClientMessage(playerid, COLOR_GREY, "Cannot use this while being in the Car.");
				return 1;
			}
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
				return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /detain [playerid/PartOfName] [seatid]");
			giveplayerid = ReturnUser(tmp);
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
				return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /detain [playerid/PartOfName] [seatid]");
			new seat = strvalEx(tmp);
			if(IsPlayerConnected(giveplayerid))
			{
				if(giveplayerid != INVALID_PLAYER_ID)
				{
					if(seat < 1 || seat > 3) return SendClientMessage(playerid, COLOR_GREY, "Seat cannot be below 1 or above 3.");
					if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
					{
						if(GetPlayerState(giveplayerid) == PLAYER_STATE_SPECTATING) return SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
						if(IsPlayerInAnyVehicle(giveplayerid)) return SendClientMessage(playerid, COLOR_GREY, "Suspect is in a Car, get him out first.");
						if(giveplayerid == playerid)
						{
							SendClientMessage(playerid, COLOR_GREY, "You cannot Detain yourself.");
							return 1;
						}
						if(PlayerCuffed[giveplayerid])
						{
							new carid = gLastCar[playerid];
							if(IsInvalidDetainVehicle(carid))
							{
								SendClientMessage(playerid, COLOR_GREY, "You can't Detain someone in that vehicle.");
								return 1;
							}
							foreach( new i: Player )
							{
								if(IsPlayerInAnyVehicle(i))
								{
									if(GetPlayerVehicleID(i) == gLastCar[playerid])
									{
										if(GetPlayerVehicleSeat(i) == seat)
										{
											format(str, sizeof(str), "That seat is occupied by %s.", PlayerICName(i));
											SendClientMessage(playerid, COLOR_GREY, str);
											return 1;
										}
									}
								}
							}
							format(str, sizeof(str), "* You were detained by %s.", PlayerICName(playerid));
							SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
							format(str, sizeof(str), "* You have detained %s.", PlayerICName(giveplayerid));
							SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
							format(str, sizeof(str), "* %s grabs %s and throws him in the car.", PlayerICName(playerid), PlayerICName(giveplayerid));
							ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
							displayCenterHUDInfo(giveplayerid,  "~r~Detained.", 8); //display for 8 seconds
							SetPlayerInterior(giveplayerid, 0);
							ClearAnimations(giveplayerid, 1);
							TogglePlayerControllable(giveplayerid, 0);
							TogglePlayerControllable(giveplayerid, 0);
							SetPlayerVirtualWorld(giveplayerid, 0);
							ClearAnimations(giveplayerid, 1);
							SetPlayerInterior(giveplayerid,0);

							if(IsInvalidDetainSeat(carid))
							{
								PutPlayerInVehicle(giveplayerid, carid, 1);
							}
							else
							{
								PutPlayerInVehicle(giveplayerid, carid, seat);
							}
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "That player needs to be restrained first.");
							return 1;
						}
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
						return 1;
					}
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
				return 1;
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not a Cop / DEA / SASD / NG Member.");
		}
		return 1;
	}
	if(strcmp(cmd, "/gps", true) == 0)
	{
		TogglePlayerGPS(playerid);
		return 1;
	}
	if(strcmp(cmd, "/stretcher", true) == 0)
	{
		if(IsAMedic(playerid))
		{
			if(IsPlayerInAnyVehicle(playerid))
			{
				SendClientMessage(playerid, COLOR_GREY, "Cannot use this while being in the Car.");
				return 1;
			}
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
				return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /stretcher [playerid/PartOfName] [seatid]");
			giveplayerid = ReturnUser(tmp);
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
				return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /stretcher [playerid/PartOfName] [seatid]");
			new seat = strvalEx(tmp);
			if(IsPlayerConnected(giveplayerid))
			{
				if(giveplayerid != INVALID_PLAYER_ID)
				{
					if(seat < 1 || seat > 3)
					{
						SendClientMessage(playerid, COLOR_GREY, "Seat cannot be below 1 or above 3.");
						return 1;
					}
					if(IsPlayerInAnyVehicle(giveplayerid))
					{
						SendClientMessage(playerid, COLOR_GREY, "Patient is in a Car, get him out first.");
						return 1;
					}
					if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
					{
						if(giveplayerid == playerid)
						{
							SendClientMessage(playerid, COLOR_GREY, "You cannot place yourself on a stretcher.");
							return 1;
						}
						{
							new carid = gLastCar[playerid];
							if(!(IsValidStretcherVehicle(carid)))
							{
								SendClientMessage(playerid, COLOR_GREY, "You can't fit a stretcher in that vehicle.");
								return 1;
							}
							foreach( new i: Player )
							{
								if(IsPlayerInAnyVehicle(i))
								{
									if(GetPlayerVehicleID(i) == gLastCar[playerid])
									{
										if(GetPlayerVehicleSeat(i) == seat)
										{
											format(str, sizeof(str), "That seat is occupied by %s.", PlayerICName(i));
											SendClientMessage(playerid, COLOR_GREY, str);
											return 1;
										}
									}
								}
							}
							format(str, sizeof(str), "* You were placed on a stretcher by %s.", PlayerICName(playerid));
							SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
							format(str, sizeof(str), "* You have placed %s on a stretcher.", PlayerICName(giveplayerid));
							SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
							format(str, sizeof(str), "* %s places %s on a stretcher and wheels him to the vehicle.", PlayerICName(playerid), PlayerICName(giveplayerid));
							ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
							displayCenterHUDInfo(giveplayerid,  "~r~You were placed on a stretcher.", 8); //display for 8 seconds
							ClearAnimations(giveplayerid, 1);
							PlayerCalledEMS[giveplayerid] = 0;
							TextDrawHideForPlayer(giveplayerid, DieText);
							TextDrawHideForPlayer(giveplayerid, InjuredText);
							TogglePlayerControllable(giveplayerid, false);
							pDeathState[giveplayerid] = PLAYER_STATE_TREAT;
							//ResetPlayerWeapons(giveplayerid);
							//ClearGuns(giveplayerid);
							ResetPlayerAdminWeaponsEx(giveplayerid);
							if(IsInvalidDetainSeat(carid)) PutPlayerInVehicle(giveplayerid, carid, 1);
							else PutPlayerInVehicle(giveplayerid, carid, seat);
						}
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
						return 1;
					}
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
				return 1;
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not a SAFMD member.");
		}
	}
	if(strcmp(cmd, "/guard", true) == 0)
	{
		if(PlayerInfo[playerid][pJob] != 8)
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not a Bodyguard.");
			return 1;
		}
		if(Guarding[playerid] != 999)
		{
			return SendClientMessage(playerid, COLOR_GRAD2, "You are already guarding someone.");
		}
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /guard [playerid/PartOfName] [price]");
			return 1;
		}
		new money;
		giveplayerid = ReturnUser(tmp);
		if(GuardedBy[giveplayerid] != 999)
		{
			SendClientMessage(playerid, COLOR_GRAD2, "That person is already being guarded.");
			return 1;
		}
		if(Guarding[giveplayerid] != 999)
		{
			return SendClientMessage(playerid, COLOR_GRAD2, "That person is already guarding someone.");
		}
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /guard [playerid/PartOfName] [price]");
			return 1;
		}
		money = strval(tmp);
		if(money < 2000 || money > 10000)
		{
			SendClientMessage(playerid, COLOR_GREY, "Price can't be lower than $2000, or above $10,000.");
			return 1;
		}
		if(IsPlayerConnected(giveplayerid))
		{
			if(giveplayerid != INVALID_PLAYER_ID)
			{
				if(IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
				{
					if(giveplayerid == playerid) return SendClientMessage(playerid, COLOR_GREY, "You can't offer protection to yourself.");
					//for(new i = 0;i < MAX_PLAYERS;i++)
					foreach( new i: Player )
					{
						if(GuardOffer[i] == playerid)
						{
							GuardOffer[i] = 999;
							GuardPrice[i] = 0;
						}
					}
					strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					format(str, sizeof(str), "* You offered protection to %s for $%d.", giveplayer, money);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* Bodyguard %s wants to protect you for $%d, (type /accept bodyguard) to accept.", sendername, money);
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					GuardOffer[giveplayerid] = playerid;
					GuardPrice[giveplayerid] = money;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
				}
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
		}
		return 1;
	}
	if(strcmp(cmd, "/sellbiztomarket", true) == 0)
	{
		if(PlayerInfo[playerid][pBizKey] == -1) return SendClientMessage(playerid, COLOR_GREY, "You do not own a business.");
		new biz = IsPlayerNearBiz(playerid);
		if(biz == -1) return SendClientMessage(playerid, COLOR_GREY, "You must be near your business.");
		if(PlayerInfo[playerid][pBizKey] != biz) return SendClientMessage(playerid, COLOR_GREY, "This is not your business.");
		if(BizInfo[biz][bSeized] == 1) return SendClientMessage(playerid, COLOR_GREY, "That business is currently property of the government.");
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			new pricestring[32];
			new price = BizInfo[biz][bValue] /4;
			new j = valstr(pricestring, price);
			while(j >= 4)
			{
				j -= 3;
				strins(pricestring, ",", j);
			}
			strins(pricestring, "$", 0);
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /sellbiztomarket [confirm]");
			format(str, sizeof(str), "Your business is worth %s.", pricestring);
			SendClientMessage(playerid, COLOR_GRAD3, str);
			return 1;
		}
		if(strcmp(tmp, "confirm", true) == 0)
		{
			GiveMoney(playerid, BizInfo[biz][bValue] /4);
			strmid(BizInfo[biz][bOwner], "None", 0, strlen("None"), 255);
			BizInfo[biz][bOwned] = 0;
			BizInfo[biz][bSeized] = 0;
			PlayerInfo[playerid][pBizKey] = -1;
			new pricestring[32];
			new price = BizInfo[biz][bValue] /4;
			new j = valstr(pricestring, price);
			while(j >= 4)
			{
				j -= 3;
				strins(pricestring, ",", j);
			}
			strins(pricestring, "$", 0);
			format(str, sizeof(str), "Congratulations, you have sold %s to the market and received 25 percent of the value, %s.", BizInfo[biz][bName], pricestring);
			SendClientMessage(playerid, COLOR_YELLOW, str);
			format(str, sizeof(str), "~w~Congratulations!~n~ You have sold your business for ~n~~g~%s.", pricestring);
			displayCenterHUDInfo(playerid,  str, 8); //display for 8 seconds
			new pricestring2[32];
			new price2 = BizInfo[biz][bValue];
			new j2 = valstr(pricestring2, price2);
			while(j2 >= 4)
			{
				j2 -= 3;
				strins(pricestring2, ",", j2);
			}
			strins(pricestring2, "$", 0);
			format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nStatus: For Sale\nPrice: %s", BizInfo[biz][bName], pricestring2);
			UpdateDynamic3DTextLabelText(Text3D:BizText[biz], COLOR_BIZTEXT, str);
		}
		return 1;
	}
	if(strcmp(cmd, "/sellbiz", true) == 0)
	{
		new biz = PlayerInfo[playerid][pBizKey];
		if(biz == -1) return SendClientMessage(playerid, COLOR_GREY, "You do not own a business.");
		if(BizInfo[biz][bSeized] == 1) return SendClientMessage(playerid, COLOR_GREY, "That business is currently property of the government.");
		if(BizInfo[biz][bDonated]) return SendClientMessage(playerid, COLOR_GREY, "You may not sell this business as it is donated.");
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /sellbusiness [playerid/PartOfName] [price]");
			return 1;
		}
		giveplayerid = ReturnUser(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /sellbusiness [playerid/PartOfName] [price]");
			return 1;
		}
		new price = strval(tmp);
		if(price < 50000 || price > 10000000) return SendClientMessage(playerid, COLOR_GREY, "Price cannot be lower than $50,000 or higher than $10,000,000.");
		if(playerid == giveplayerid)
		{
			return SendClientMessage(playerid, COLOR_GREY, "You cannot sell yourself a business.");
		}
		if(PlayerInfo[giveplayerid][pBizKey] != -1)
		{
			return SendClientMessage(playerid, COLOR_GREY, "That player already owns a business.");
		}
		if(!IsPlayerInRangeOfPlayer(8.0, playerid, giveplayerid))
		{
			return SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
		}
		format(str, sizeof(str), "* You offered %s to buy your business (%s) for $%d.", PlayerICName(giveplayerid), BizInfo[biz][bName], price);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		format(str, sizeof(str), "* %s wants to sell you a business (%s) for $%d, (type /accept business) to accept.", PlayerICName(playerid), BizInfo[biz][bName], price);
		SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
		format(str, sizeof(str), "* %s hands %s the contract to their business.", PlayerICName(playerid), PlayerICName(giveplayerid));
		ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		BizOffer[giveplayerid] = playerid;
		BizPrice[giveplayerid] = price;
		BizID[giveplayerid] = biz;
		return 1;
	}
	if(strcmp(cmd, "/buybiz", true) == 0)
	{
		new biz = IsPlayerNearBiz(playerid);
		if(biz == -1) return SendClientMessage(playerid, COLOR_GREY, "You aren't at a buyable business.");
		if(BizInfo[biz][bSeized] == 1) return SendClientMessage(playerid, COLOR_GREY, "That business is currently property of the government.");
		if(BizInfo[biz][bOwned] == 1) return SendClientMessage(playerid, COLOR_GREY, "This business is already owned.");
		if(PlayerInfo[playerid][pBizKey] != -1) return SendClientMessage(playerid, COLOR_GREY, "You already own a business.");
		if(GetMoney(playerid) >= BizInfo[biz][bValue])
		{
			format(str, sizeof(str), "* Congratulations! You bought the %s business. Type /help to view your commands.", BizInfo[biz][bName]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
			GiveMoney(playerid, -BizInfo[biz][bValue]);
			BizInfo[biz][bOwned] = 1;
			PlayerInfo[playerid][pBizKey] = biz;
			strmid(BizInfo[biz][bOwner], PlayerName(playerid), 0, strlen(PlayerName(playerid)), 255);
			if(BizInfo[biz][bLocked] == 1)
				format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s", BizInfo[biz][bName], BizInfo[biz][bOwner]);
			else
				format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s", BizInfo[biz][bName], BizInfo[biz][bOwner]);
			UpdateDynamic3DTextLabelText(Text3D:BizText[biz], COLOR_BIZTEXT, str);
			return 1;
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You do not have enough money to purchase this business.");
		}
		return 1;
	}
	if(strcmp(cmd, "/free", true) == 0)
	{
		if(PlayerInfo[playerid][pJob] != 2)
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not a Lawyer.");
			return 1;
		}
		if(PlayerInfo[playerid][pLawyerFreeTime] != 0)
		{
			SendClientMessage(playerid, COLOR_GREY, "You must wait 2 minutes before you can free someone again.");
			return 1;
		}
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /free [playerid/PartOfName]");
			return 1;
		}
		giveplayerid = ReturnUser(tmp);
		if(IsPlayerConnected(giveplayerid))
		{
			if(giveplayerid != INVALID_PLAYER_ID)
			{
				if(IsPlayerInRangeOfPlayer(5.0, playerid, giveplayerid))
				{
					if(giveplayerid == playerid)
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't free yourself.");
						return 1;
					}
					if(PlayerInfo[giveplayerid][pAdminJailed] == 1)
					{
						return SendClientMessage(playerid, COLOR_LIGHTBLUE, "This player was admin prisoned, you cannot decrease their time.");
					}
					if(PlayerInfo[giveplayerid][pJailed] == 1)
					{
						new minutes;
						new level = PlayerInfo[playerid][pLawSkill];
						if(level >= 0 && level <= 49)
						{
							minutes = 1;
						}
						else if(level >= 50 && level <= 99)
						{
							minutes = 2;
						}
						else if(level >= 100 && level <= 199)
						{
							minutes = 3;
						}
						else if(level >= 200 && level <= 399)
						{
							minutes = 4;
						}
						else if(level >= 400)
						{
							minutes = 5;
						}
						strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						format(str, sizeof(str), "* You have reduced %s's jail time by %d minutes.", giveplayer, minutes);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* Lawyer %s has reduced your jail time by %d minutes.", sendername, minutes);
						SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
						WantLawyer[giveplayerid] = 0;
						CallLawyer[giveplayerid] = 0;
						JailPrice[giveplayerid] = 0;
						PlayerInfo[giveplayerid][pJailTime] = PlayerInfo[giveplayerid][pJailTime] -minutes * 60;
						PlayerInfo[playerid][pLawSkill] ++;
						PlayerInfo[playerid][pLawyerFreeTime] = 120;
						if(PlayerInfo[playerid][pLawSkill] == 50)
						{
							SendClientMessage(playerid, COLOR_YELLOW, "* Your Lawyer Skill is now Level 2, you will now earn more Money and have a quicker Reload Time.");
						}
						else if(PlayerInfo[playerid][pLawSkill] == 100)
						{
							SendClientMessage(playerid, COLOR_YELLOW, "* Your Lawyer Skill is now Level 3, you will now earn more Money and have a quicker Reload Time.");
						}
						else if(PlayerInfo[playerid][pLawSkill] == 200)
						{
							SendClientMessage(playerid, COLOR_YELLOW, "* Your Lawyer Skill is now Level 4, you will now earn more Money and have a quicker Reload Time.");
						}
						else if(PlayerInfo[playerid][pLawSkill] == 400)
						{
							SendClientMessage(playerid, COLOR_YELLOW, "* Your Lawyer Skill is now Level 5, you will now earn more Money and have a quicker Reload Time.");
						}
					}
					else
					{
						SendClientMessage(playerid, COLOR_GRAD1, "Player is not Jailed.");
						return 1;
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GRAD1, "You're too far away.");
					return 1;
				}
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
		}
		return 1;
	}
	if(strcmp(cmd, "/cancel", true) ==0)
	{
		new x_job[64];
		x_job = strtok(cmdtext, idx);
		if(!strlen(x_job))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /cancel [name]");
			SendClientMessage(playerid, COLOR_GRAD1, "Available names: Pot, Crack, Repair, Lawyer, Defense, Vest, Live, Refill, Boxing, Gamble");
			SendClientMessage(playerid, COLOR_GRAD2, "Available names: Taxi, Bus, Medic, Mechanic, Ticket, Handshake, Kiss, Witness, Marriage, Divorce");
			SendClientMessage(playerid, COLOR_GRAD2, "Available names: Family, Faction, Group, Contract, Biz, Call");
			return 1;
		}
		else if(strcmp(x_job, "gamble", true) == 0)
		{
			GambleOffer[playerid] = -1; 
            GambleAmount[playerid] = 0;
		}
		else if(strcmp(x_job, "pot", true) == 0)
		{
			PotOffer[playerid] = 999;                        
			PotPrice[playerid] = 0;
			PotGram[playerid] = 0;
		}
		else if(strcmp(x_job, "crack", true) == 0)
		{
			CrackOffer[playerid] = 999;
			CrackPrice[playerid] = 0;
			CrackGram[playerid] = 0;
		}
		else if(strcmp(x_job, "materials", true) == 0)
		{
			MatsOffer[playerid] = 999;
			MatsPrice[playerid] = 0;
			MatsGram[playerid] = 0;
			MatsGram2[playerid] = 0;
		}
		else if(strcmp(x_job, "prod", true) == 0)
		{
			ProdOffer[playerid] = 999;
			ProdPrice[playerid] = 0;
			ProdAmount[playerid] = 0;
		}
		else if(strcmp(x_job, "repair", true) == 0)
		{
			RepairOffer[playerid] = 999;
			RepairPrice[playerid] = 0;
			RepairCar[playerid] = 0;
			repairVeh[playerid] = -1;
			CP[playerid] = CHECKPOINT_NONE;
			DisablePlayerCheckpointEx(playerid);
			isRepairing[playerid] = false;
		}
		else if(strcmp(x_job, "lawyer", true) == 0)
		{
			WantLawyer[playerid] = 0;
			CallLawyer[playerid] = 0;
		}
		else if(strcmp(x_job, "defense", true) == 0)
		{
			DefenseOffer[playerid] = 999;
			DefensePrice[playerid] = 0;
		}
		else if(strcmp(x_job, "vest", true) == 0)
		{
			VestOffer[playerid] = 999;
			VestPrice[playerid] = 0;
		}
		else if(strcmp(x_job, "bodyguard", true) == 0)
		{
			if(GuardedBy[playerid] == 999) return SendClientMessage(playerid, COLOR_GREY, "You aren't being guarded.");
			SetPlayerToTeamColor(playerid);
			SetPlayerToTeamColor(GuardedBy[playerid]);
			Guarding[GuardedBy[playerid]] = 999;
			GuardedBy[playerid] = 999;
			GuardingPrice[playerid] = 0;
			SetPlayerArmour(playerid, 0.0);
		}
		else if(strcmp(x_job, "biz", true) == 0)
		{
			BizID[playerid] = 999;
			BizOffer[playerid] = 999;
			BizPrice[playerid] = 0;
		}
		else if(strcmp(x_job, "live", true) == 0)
		{
			LiveOffer[playerid] = 999;
		}
		else if(strcmp(x_job, "refill", true) == 0)
		{
			RefillOffer[playerid] = INVALID_PLAYER_ID;
			RefillPrice[playerid] = 0;
		}
		else if(strcmp(x_job, "boxing", true) == 0)
		{
			BoxOffer[playerid] = 999;
		}
		else if(strcmp(x_job, "witness", true) == 0)
		{
			MarryWitnessOffer[playerid] = 999;
		}
		else if(strcmp(x_job, "marriage", true) == 0)
		{
			ProposeOffer[playerid] = 999;
		}
		else if(strcmp(x_job, "divorce", true) == 0)
		{
			DivorceOffer[playerid] = 999;
		}
		else if(strcmp(x_job, "family", true) == 0)
		{
			FamilyOffer[playerid] = 999;
		}
		else if(strcmp(x_job, "faction", true) == 0)
		{
			FactionOffer[playerid] = 999;
		}
		else if(strcmp(x_job, "group", true) == 0)
		{
			GroupOffer[playerid] = 999;
		}
		else if(strcmp(x_job, "frisk", true) == 0)
		{
			FriskOffer[playerid] = 999;
		}
		else if(strcmp(x_job, "handshake", true) == 0)
		{
			HSHAKEOFFER[playerid] = 999;
			HShakeType[playerid] = 0;
		}
		else if(strcmp(x_job, "kiss", true) == 0)
		{
			KissOffer[playerid] = 999;
			KissType[playerid] = 0;
		}
		else if(strcmp(x_job, "contract", true) == 0)
		{
			if(PlayerInfo[playerid][pGroup] != GROUP_HITMAN) return 1;
			if(GoChase[playerid] < 999)
			{
				format(str, sizeof(str), "Hitman %s has cancelled the contract on %s.", PlayerICName(playerid), PlayerICName(GoChase[playerid]));
				foreach( new i: Player )
				{
					if(PlayerInfo[i][pGroup] == GROUP_HITMAN)
					{
						SendClientMessage(i, COLOR_YELLOW, str);
					}
				}
			}
			ContractOffer[playerid] = 999;
			ContractID[playerid] = 999;
			GoChase[playerid] = 999;
		}
		else if(strcmp(x_job, "medic", true) == 0)
		{
			if(IsPlayerConnected(MedicCall))
			{
				if(MedicCall == playerid)
				{
					MedicCall = 999;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You are not the current Caller.");
					return 1;
				}
			}
		}
		else if(strcmp(x_job, "mechanic", true) == 0)
		{
			if(IsPlayerConnected(MechanicCall))
			{
				if(MechanicCall == playerid)
				{
					MechanicCall = 999;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You are not the current Caller.");
					return 1;
				}
			}
		}
		else if(strcmp(x_job, "taxi", true) == 0)
		{
			if(TaxiCall < 999)
			{
				if(TransportDuty[playerid] == 1 && TaxiCallTime[playerid] > 0)
				{
					TaxiAccepted[playerid] = 999;
					displayCenterHUDInfo(playerid,  "~w~You have canceled the~n~~r~call", 8); //display for 8 seconds
					TaxiCallTime[playerid] = 0;
					DisablePlayerCheckpointEx(playerid);
					TaxiCall = 999;
				}
				else
				{
					if(IsPlayerConnected(TaxiCall))
					{
						if(TaxiCall == playerid)
						{
							TaxiCall = 999;
						}
					}
					foreach( new i: Player )
					{
						if(IsPlayerConnected(i))
						{
							if(TaxiAccepted[i] < 999)
							{
								if(TaxiAccepted[i] == playerid)
								{
									TaxiAccepted[i] = 999;
									displayCenterHUDInfo(i,  "~w~Taxi Caller has canceled the~n~~r~call", 8); //display for 8 seconds
									TaxiCallTime[i] = 0;
									DisablePlayerCheckpointEx(i);
								}
							}
						}
					}
				}
			}
		}
		else if(strcmp(x_job, "call", true) == 0)
		{
			if(PlayerInfo[playerid][pMember] == FACTION_FMD || PlayerInfo[playerid][pLeader] == FACTION_FMD || FakeFaction[playerid] == FACTION_FMD)
			{
				if(ParaOnACall[playerid] == 1)
				{
					ParaOnACall[playerid] = 0;
					CallAccepted[PlayerVictim[playerid]] = 0;
					EMSArrived[PlayerVictim[playerid]] = 0;
					DisablePlayerCheckpointEx(playerid);
					format(str, sizeof(str), "* Your call was put back in line.");
					SendClientMessage(PlayerVictim[playerid], COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "Dispatch: Paramedic %s has canceled the call of %s.", PlayerICName(playerid), PlayerICName(PlayerVictim[playerid]));
					SendParaMessage(COLOR_DOCTOR, str);
					return 1;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You are not on a call.");
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not a Paramedic.");
			}
		}
		else if(strcmp(x_job, "bus", true) == 0)
		{
			if(BusCall < 999)
			{
				if(TransportDuty[playerid] == 2 && BusCallTime[playerid] > 0)
				{
					BusAccepted[playerid] = 999;
					displayCenterHUDInfo(playerid,  "~w~You have canceled the~n~~r~call", 8); //display for 8 seconds
					BusCallTime[playerid] = 0;
					DisablePlayerCheckpointEx(playerid);
					BusCall = 999;
				}
				else
				{
					if(IsPlayerConnected(BusCall))
					{
						if(BusCall == playerid)
						{
							BusCall = 999;
						}
					}
					foreach( new i: Player )
					{
						if(IsPlayerConnected(i))
						{
							if(BusAccepted[i] < 999)
							{
								if(BusAccepted[i] == playerid)
								{
									BusAccepted[i] = 999;
									displayCenterHUDInfo(i,  "~w~Bus caller has canceled the~n~~r~call", 8); //display for 8 seconds
									BusCallTime[i] = 0;
									DisablePlayerCheckpointEx(i);
								}
							}
						}
					}
				}
			}
		}
		else
		{
			return 1;
		}
		format(str, sizeof(str), "* You have canceled the %s.", x_job);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		return 1;
	}
	if(strcmp(cmd, "/accept", true) ==0)
	{
		new x_job[64];
		x_job = strtok(cmdtext, idx);
		if(!strlen(x_job))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /accept [name]");
			SendClientMessage(playerid, COLOR_GRAD1, "Available names: Sex, Pot, Crack, Repair, Lawyer, Defense, Vest, Job, Live, Refill, Weapon");
			SendClientMessage(playerid, COLOR_GRAD2, "Available names: Taxi, Boxing, Medic, Mechanic, Ticket, Family, Bodyguard, Heal, Business, Kevlar");
			SendClientMessage(playerid, COLOR_GRAD2, "Available names: Namechange, Gamble");
			return 1;
		}
		else if(strcmp(x_job, "keys", true) == 0)
		{
			if(KeysOfferFrom[playerid] != -1 && KeysOfferIndex[playerid] != 0)
			{
				if(!IsPlayerConnected(KeysOfferFrom[playerid])) return SendClientMessage(playerid, COLOR_GRAD1, "The offerer isn't connected to the server anymore.");
				PlayerInfo[playerid][pKeys] = KeysOfferIndex[playerid];
				format(x_job, sizeof(x_job), "** %s has accepted your keys offer.", PlayerICName(playerid));
				SendClientMessage(KeysOfferFrom[playerid], COLOR_LIGHTBLUE, x_job);
				format(x_job, sizeof(x_job), "** You have accepted %s's keys offer.", PlayerICName(KeysOfferFrom[playerid]));
				SendClientMessage(playerid, COLOR_LIGHTBLUE, x_job);
				KeysOfferFrom[playerid] = -1;
				KeysOfferIndex[playerid] = 0;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GRAD1, "Noone has offered you their vehicle keys.");
			}
			return 1;
		}
		else if(strcmp(x_job, "house", true) == 0)
		{
			new houseid = PlayerInfo[OfferedSellHouse[playerid][0]][pHouseID];
			new sellerid = OfferedSellHouse[playerid][0];
			new Float:pgx, Float:pgy, Float:pgz;
			GetPlayerPos(sellerid, pgx, pgy, pgz);
			if(!IsPlayerInRangeOfPoint(playerid, 3.0, pgx, pgy, pgz)) return SendClientMessage(playerid, COLOR_GREY, "You are not near the player! ");
			if(OfferedSellHouse[playerid][0] != -1 && OfferedSellHouse[playerid][1] != -1 && OfferedSellHouse[playerid][2] != -1)
			{
				if(houseid != 0 && houseid == OfferedSellHouse[playerid][2] && HouseInfo[houseid][hOwned] == 1)
				{
					if(GetMoney(playerid) >= OfferedSellHouse[playerid][1])
					{
						if(PlayerInfo[playerid][pHouseID] != 0) return SendClientMessage(playerid, COLOR_GRAD1, "You already own a house.");
						new location[MAX_ZONE_NAME];
						Get2DZone(location, MAX_ZONE_NAME, HouseInfo[houseid][hOutsideX], HouseInfo[houseid][hOutsideY], HouseInfo[houseid][hOutsideZ]);
						GiveMoney(OfferedSellHouse[playerid][0], OfferedSellHouse[playerid][1]);
						GiveMoney(playerid, -OfferedSellHouse[playerid][1]);
						strmid(HouseInfo[houseid][hOwner], PlayerName(playerid), 0, strlen(PlayerName(playerid)), 255);
						if(IsACop(sellerid) || IsANG(sellerid) || IsAMedic(sellerid) || IsASenate(sellerid))
						{
							HouseInfo[houseid][hGun1] = 0;
							HouseInfo[houseid][hGun2] = 0;
							HouseInfo[houseid][hGun3] = 0;
							HouseInfo[houseid][hGun4] = 0;
							HouseInfo[houseid][hGun5] = 0;
							HouseInfo[houseid][hGun6] = 0;
							HouseInfo[houseid][hGun7] = 0;
						}
						PlayerInfo[playerid][pHouseID] = houseid;
						HouseInfo[houseid][hOwnerID] = PlayerInfo[playerid][pID];
						PlayerInfo[OfferedSellHouse[playerid][0]][pHouseID] = 0;
						format(str, sizeof(str), "Congratulations, you have sold {FFFF00}%d %s{FAFAFA} for $%d to %s.", houseid, location, OfferedSellHouse[playerid][1], PlayerICName(playerid));
						SendClientMessage(OfferedSellHouse[playerid][0], COLOR_WHITE, str);
						format(str, sizeof(str), "Congratulations, you have bought {FFFF00}%d %s{FAFAFA} for $%d from %s.", houseid, location, OfferedSellHouse[playerid][1], PlayerICName(OfferedSellHouse[playerid][0]));
						SendClientMessage(playerid, COLOR_WHITE, str);
						SetHouseTag(houseid);
						OfferedSellHouse[playerid][0] = -1;
						OfferedSellHouse[playerid][1] = -1;
						OfferedSellHouse[playerid][2] = -1;

						HouseInfo[houseid][hModified] = 1;
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GRAD1, "You can not afford this.");
						return 1;
					}
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GRAD1, "No one offered to sell you a house.");
				return 1;
			}
			return 1;
		}
		else if(strcmp(x_job, "weapon", true) == 0)
		{
			if(OfferedGunFrom[playerid] == -1) return SendClientMessage(playerid, COLOR_GRAD1, "No one offered you a weapon.");
			if(PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GRAD1, "You must play for at least 2 hours before you can use this.");
			if(OfferedGunTo[OfferedGunFrom[playerid]] != playerid)
			{
				OfferedGunFrom[playerid] = -1;
				GunOffered[playerid] = 0;
				return SendClientMessage(playerid, COLOR_GRAD1, "Player is no longer offering you a weapon.");
			}
			if(GunOffered[playerid] != GetPlayerWeapon(OfferedGunFrom[playerid]))
				return SendClientMessage(playerid, COLOR_GREY, "That player doesn't have the right weapon in their hands.");
			if(GetPlayerWeapon(OfferedGunFrom[playerid]) != GunOffered[playerid]) return SendClientMessage(playerid, COLOR_GRAD1, "That player isn't holding the offered weapon.");
			new Float:X23, Float:Y23, Float:Z23;
			GetPlayerPos(OfferedGunFrom[playerid], X23, Y23, Z23);
			if(!IsPlayerInRangeOfPoint(playerid, 3.0, X23, Y23, Z23)) return SendClientMessage(playerid, COLOR_GRAD1, "You are not close enough to that player to accept his weapon.");
			new weaponname[50];
			GetWeaponName(GunOffered[playerid], weaponname, sizeof(weaponname));
			format(string5, sizeof(string5), "%s has accepted your %s.", PlayerICName(playerid), weaponname);
			SendClientMessage(OfferedGunFrom[playerid], COLOR_LIGHTBLUE, string5);
			format(string5, sizeof(string5), "You have accepted %s's %s.", PlayerICName(OfferedGunFrom[playerid]), weaponname);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, string5);
			format(string5, sizeof(string5), "%s has accepted a gun (%s) from %s.", PlayerOOCName(playerid), weaponname, PlayerOOCName(OfferedGunFrom[playerid]));
			PayLog(string5);

			TakePlayerWeapon(OfferedGunFrom[playerid], GunOffered[playerid]);
			GivePlayerGun(playerid, GunOffered[playerid]);

			OfferedGunTo[OfferedGunFrom[playerid]] = -1;
			OfferedGunFrom[playerid] = -1;
			GunOffered[playerid] = 0;
			if(IsPlayerInAnyVehicle(playerid))
			{
				if(GetPlayerVehicleSeat(playerid) == 0)
				{
					SetPlayerArmedWeapon(playerid, 0);
				}
				if((GetPlayerWeapon(playerid) != 38 && GetPlayerWeapon(playerid) != 29 && GetPlayerWeapon(playerid) != 22))
				{
					SetPlayerArmedWeapon(playerid, 0);
				}
			}
			if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER || GetPlayerState(playerid) == PLAYER_STATE_PASSENGER)
			{
				SetPlayerArmedWeapon(playerid, 0);
			}
			return 1;
		}
		else if(strcmp(x_job, "kevlar", true) == 0)
		{
			if(IsACop(playerid) == 1 || IsANG(playerid) == 1 || IsAMedic(playerid) == 1 || IsASenate(playerid)) return SendClientMessage(playerid, COLOR_GRAD1, "Cops/DEA/DoC can not use this command.");
			if(!(KevlarOfferFrom[playerid] == -1))
			{
				new Float:x, Float:y, Float:z;
				GetPlayerPos(KevlarOfferFrom[playerid], x, y, z);
				if(IsPlayerInRangeOfPoint(playerid, 3.0, x, y, z))
				{
					new Float:hasArmor;
					GetPlayerArmour(KevlarOfferFrom[playerid], hasArmor);
					if(hasArmor < 1) return SendClientMessage(playerid, COLOR_GRAD1, "That player doesn't have a kevlar vest.");
					SetPlayerArmour(KevlarOfferFrom[playerid], 0);
					SetPlayerArmour(playerid, hasArmor);
					format(string5, sizeof(string5), "%s has accepted your kevlar.", PlayerICName(playerid));
					SendClientMessage(KevlarOfferFrom[playerid], COLOR_LIGHTBLUE, string5);
					format(string5, sizeof(string5), "You have accepted %s's kevlar.", PlayerICName(KevlarOfferFrom[playerid]));
					SendClientMessage(playerid, COLOR_LIGHTBLUE, string5);
					KevlarOfferTo[KevlarOfferFrom[playerid]] = -1;
					KevlarOfferFrom[playerid] = -1;
					return 1;
				}
				else return SendClientMessage(playerid, COLOR_GRAD1, "You are not close enough to that player to accept his kevlar.");
			}
			else return SendClientMessage(playerid, COLOR_GREY, "No one offered you a kevlar.");
		}
		/*else if(strcmp(x_job, "frisk", true) == 0)
		{
			if(FriskOffer[playerid] < 999)
			{
				if(IsPlayerConnected(FriskOffer[playerid]))
				{
					if(IsPlayerInRangeOfPlayer(12.0, playerid, FriskOffer[playerid]))
					{
						format(str, sizeof(str), "* You accepted %s's offer to frisk you.", PlayerICName(FriskOffer[playerid]));
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* %s has accepted your offer to frisk them.", PlayerICName(playerid));
						SendClientMessage(FriskOffer[playerid], COLOR_LIGHTBLUE, str);
						new WeaponName[65];
						strmid(giveplayer, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						GetPlayerICName(FriskOffer[playerid], sendername, sizeof(sendername));
						SendClientMessage(FriskOffer[playerid], COLOR_GREEN, "_______________________________________");
						format(str, sizeof(str), "*** %s's Items ***", giveplayer);
						SendClientMessage(FriskOffer[playerid], COLOR_WHITE, str);
						
						if(PlayerInfo[playerid][pCash] > 0)
						{
							format(str, sizeof(str), "Money (%d)", PlayerInfo[playerid][pCash]);
							SendClientMessage(FriskOffer[playerid], COLOR_GREY, str);
						}
						if(PlayerInfo[playerid][pCannabis] > 0)
						{
							format(str, sizeof(str), "Cannabis (%dg)", PlayerInfo[playerid][pCannabis]);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pCocaine] > 0)
						{
							format(str, sizeof(str), "Cocaine (%dg)", PlayerInfo[playerid][pCocaine]);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pMeth] > 0)
						{
							format(str, sizeof(str), "Meth (%dg)", PlayerInfo[playerid][pMeth]);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pXanax] > 0) {
							format(str, sizeof(str), "Xanax (%d pills)", PlayerInfo[giveplayerid][pXanax]);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pPromethazine] > 0) {
							format(str, sizeof(str), "Promethazine (%d.0 fl oz)", PlayerInfo[giveplayerid][pPromethazine]);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pCodeine] > 0) {
							format(str, sizeof(str), "Codeine (%d.0 fl oz)", PlayerInfo[giveplayerid][pCodeine]);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[giveplayerid][pLean] > 0) {
							format(str, sizeof(str), "Mixed substance (Promethazine/Codeine) (%d.0g)", PlayerInfo[giveplayerid][pLean]);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pPotSeeds] > 0)
						{
							format(str, sizeof(str), "Seeds (%dg)", PlayerInfo[playerid][pPotSeeds]);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pMats] > 0)
						{
							format(str, sizeof(str), "Materials (%d)", PlayerInfo[playerid][pMats]);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(Packages[playerid] > 0)
						{
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, "Materials Packages");
						}
						if(Crates[playerid] > 0)
						{
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, "Drug Crates");
						}
						if(Fishes[playerid][pWeight1] > 0 || Fishes[playerid][pWeight2] > 0 || Fishes[playerid][pWeight3] > 0 || Fishes[playerid][pWeight4] > 0 || Fishes[playerid][pWeight5] > 0)
						{
							SendClientMessage(FriskOffer[playerid], COLOR_GRAD3, "Fish");
						}
						if(PlayerInfo[playerid][pPhoneBook] > 0)
						{
							SendClientMessage(FriskOffer[playerid], COLOR_GRAD3, "Phone Book");
						}
						if(PlayerInfo[playerid][piPod] > 0)
						{
							SendClientMessage(FriskOffer[playerid], COLOR_GRAD3, "iPod");
						}
						if(PlayerInfo[playerid][pPnumber] > 0)
						{
							SendClientMessage(FriskOffer[playerid], COLOR_GRAD3, "Cellphone");
						}
						if(PlayerInfo[playerid][pSpraycan] > 0)
						{
							format(str, sizeof(str), "Spraycans (%d)", PlayerInfo[playerid][pSpraycan]);
							SendClientMessage(FriskOffer[playerid], COLOR_GREY, str);
						}
						if(PlayerInfo[playerid][pScrew] > 0)
						{
							format(str, sizeof(str), "Screwdrivers (%d)", PlayerInfo[playerid][pScrew]);
							SendClientMessage(FriskOffer[playerid], COLOR_GREY, str);
						}
						if(PlayerInfo[playerid][pDice] > 0)
						{
							SendClientMessage(FriskOffer[playerid], COLOR_GRAD3, "Dice");
						}
						if(PlayerInfo[playerid][pRope] > 0)
						{
							format(str, sizeof(str), "Rope (%d)", PlayerInfo[playerid][pRope]);
							SendClientMessage(FriskOffer[playerid], COLOR_GREY, str);
						}
						if(PlayerInfo[playerid][pGag] > 0)
						{
							format(str, sizeof(str), "Gag (%d)", PlayerInfo[playerid][pGag]);
							SendClientMessage(FriskOffer[playerid], COLOR_GREY, str);
						}
						if(PlayerInfo[playerid][pCigars] > 0)
						{
							format(str, sizeof(str), "Cigars (%d)", PlayerInfo[playerid][pCigars]);
							SendClientMessage(FriskOffer[playerid], COLOR_GREY, str);
						}
						if(PlayerInfo[playerid][pSprunk] > 0)
						{
							format(str, sizeof(str), "Sprunk (%d)", PlayerInfo[playerid][pSprunk]);
							SendClientMessage(FriskOffer[playerid], COLOR_GREY, str);
						}
						if(PlayerInfo[playerid][pBombs] > 0)
						{
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, "C4 Explosives");
						}*/
								/*if(PlayerInfo[playerid][pPoison] > 0)
								{
									SendClientMessage(FriskOffer[playerid], COLOR_BITEM, "Poison");
								}*/
						/*if(PlayerInfo[playerid][pBlindfolds] > 0)
						{
							format(str, sizeof(str), "Blindfolds (%d)", PlayerInfo[playerid][pBlindfolds]);
							SendClientMessage(FriskOffer[playerid], COLOR_GREY, str);
						}
						if(PlayerInfo[playerid][pPortMDC] != 0)
						{
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, "Portable MDC");
						}
						if(PlayerInfo[playerid][pGun0] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun0], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun1] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun1], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun2] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun2], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun3] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun3], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun4] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun4], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun5] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun5], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun6] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun6], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun7] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun7], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun8] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun8], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun9] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun9], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun10] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun10], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun11] == 44)
						{
							SendClientMessage(playerid, COLOR_GRAD3, "Nightvision Goggles");
						}
						if(PlayerInfo[playerid][pGun11] == 45)
						{
							SendClientMessage(playerid, COLOR_GRAD3, "Infared Goggles");
						}
						if(PlayerInfo[playerid][pGun12] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun12], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						for(new weap = 1; weap < 47; weap++)
						{
							if(HaveAdminWeapon(playerid, weap) == weap)
							{
								GetWeaponName(weap, WeaponName, 64);
								format(str, sizeof(str), "{FF0000}Admin Given %s", WeaponName);
								SendClientMessage(FriskOffer[playerid], COLOR_WHITE, str);
							}
						}
						format(str, sizeof(str), "* %s has frisked %s.", PlayerICName(FriskOffer[playerid]), PlayerICName(playerid));
						ProxDetector(30.0, FriskOffer[playerid], str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						FriskOffer[playerid] = 999;
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "The frisker is too far away.");
						return 1;
					}
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody offered to frisk you.");
				return 1;
			}
		}*/
		else if(strcmp(x_job, "frisk", true) == 0)
		{
			if(FriskOffer[playerid] < 999)
			{
				if(IsPlayerConnected(FriskOffer[playerid]))
				{
					if(IsPlayerInRangeOfPlayer(12.0, playerid, FriskOffer[playerid]))
					{
						format(str, sizeof(str), "* You accepted %s's offer to frisk you.", PlayerICName(FriskOffer[playerid]));
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* %s has accepted your offer to frisk them.", PlayerICName(playerid));
						SendClientMessage(FriskOffer[playerid], COLOR_LIGHTBLUE, str);
						new WeaponName[65];
						strmid(giveplayer, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						GetPlayerICName(FriskOffer[playerid], sendername, sizeof(sendername));
						SendClientMessage(FriskOffer[playerid], COLOR_GREEN, "_______________________________________");
						format(str, sizeof(str), "*** %s's Items ***", giveplayer);
						SendClientMessage(FriskOffer[playerid], COLOR_WHITE, str);

						if(PlayerInfo[playerid][pCash] > 0)
						{
							format(str, sizeof(str), "Money (%d)", PlayerInfo[playerid][pCash]);
							SendClientMessage(FriskOffer[playerid], COLOR_GREY, str);
						}
						if(PlayerInfo[playerid][pCannabis] > 0)
						{
							format(str, sizeof(str), "Cannabis (%dg)", PlayerInfo[playerid][pCannabis]);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pCocaine] > 0)
						{
							format(str, sizeof(str), "Cocaine (%dg)", PlayerInfo[playerid][pCocaine]);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pMeth] > 0)
						{
							format(str, sizeof(str), "Meth (%dg)", PlayerInfo[playerid][pMeth]);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pXanax] > 0) {
							format(str, sizeof(str), "Xanax (%d pills)", PlayerInfo[playerid][pXanax]);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pPromethazine] > 0) {
							format(str, sizeof(str), "Promethazine (%d.0 fl oz)", PlayerInfo[playerid][pPromethazine]);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pCodeine] > 0) {
							format(str, sizeof(str), "Codeine (%d.0 fl oz)", PlayerInfo[playerid][pCodeine]);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pLean] > 0) {
							format(str, sizeof(str), "Mixed substance (Promethazine/Codeine) (%d.0g)", PlayerInfo[playerid][pLean]);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pPotSeeds] > 0)
						{
							format(str, sizeof(str), "Seeds (%dg)", PlayerInfo[playerid][pPotSeeds]);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pMats] > 0)
						{
							format(str, sizeof(str), "Materials (%d)", PlayerInfo[playerid][pMats]);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(Packages[playerid] > 0)
						{
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, "Materials Packages");
						}
						if(Crates[playerid] > 0)
						{
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, "Drug Crates");
						}
						if(Fishes[playerid][pWeight1] > 0 || Fishes[playerid][pWeight2] > 0 || Fishes[playerid][pWeight3] > 0 || Fishes[playerid][pWeight4] > 0 || Fishes[playerid][pWeight5] > 0)
						{
							SendClientMessage(FriskOffer[playerid], COLOR_GRAD3, "Fish");
						}
						if(PlayerInfo[playerid][pPhoneBook] > 0)
						{
							SendClientMessage(FriskOffer[playerid], COLOR_GRAD3, "Phone Book");
						}
						if(PlayerInfo[playerid][piPod] > 0)
						{
							SendClientMessage(FriskOffer[playerid], COLOR_GRAD3, "iPod");
						}
						if(PlayerInfo[playerid][pPnumber] > 0)
						{
							SendClientMessage(FriskOffer[playerid], COLOR_GRAD3, "Cellphone");
						}
						if(PlayerInfo[playerid][pSpraycan] > 0)
						{
							format(str, sizeof(str), "Spraycans (%d)", PlayerInfo[playerid][pSpraycan]);
							SendClientMessage(FriskOffer[playerid], COLOR_GREY, str);
						}
						if(PlayerInfo[playerid][pScrew] > 0)
						{
							format(str, sizeof(str), "Screwdrivers (%d)", PlayerInfo[playerid][pScrew]);
							SendClientMessage(FriskOffer[playerid], COLOR_GREY, str);
						}
						if(PlayerInfo[playerid][pDice] > 0)
						{
							SendClientMessage(FriskOffer[playerid], COLOR_GRAD3, "Dice");
						}
						if(PlayerInfo[playerid][pRope] > 0)
						{
							format(str, sizeof(str), "Rope (%d)", PlayerInfo[playerid][pRope]);
							SendClientMessage(FriskOffer[playerid], COLOR_GREY, str);
						}
						if(PlayerInfo[playerid][pGag] > 0)
						{
							format(str, sizeof(str), "Gag (%d)", PlayerInfo[playerid][pGag]);
							SendClientMessage(FriskOffer[playerid], COLOR_GREY, str);
						}
						if(PlayerInfo[playerid][pCigars] > 0)
						{
							format(str, sizeof(str), "Cigars (%d)", PlayerInfo[playerid][pCigars]);
							SendClientMessage(FriskOffer[playerid], COLOR_GREY, str);
						}
						if(PlayerInfo[playerid][pSprunk] > 0)
						{
							format(str, sizeof(str), "Sprunk (%d)", PlayerInfo[playerid][pSprunk]);
							SendClientMessage(FriskOffer[playerid], COLOR_GREY, str);
						}
						if(PlayerInfo[playerid][pBombs] > 0)
						{
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, "C4 Explosives");
						}
						/*if(PlayerInfo[playerid][pPoison] > 0)
						{
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, "Poison");
						}*/
						if(PlayerInfo[playerid][pBlindfolds] > 0)
						{
							format(str, sizeof(str), "Blindfolds (%d)", PlayerInfo[playerid][pBlindfolds]);
							SendClientMessage(FriskOffer[playerid], COLOR_GREY, str);
						}
						if(PlayerInfo[playerid][pPortMDC] != 0)
						{
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, "Portable MDC");
						}
						if(PlayerInfo[playerid][pGun0] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun0], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun1] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun1], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun2] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun2], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun3] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun3], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun4] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun4], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun5] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun5], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun6] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun6], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun7] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun7], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun8] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun8], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun9] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun9], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun10] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun10], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						if(PlayerInfo[playerid][pGun11] == 44)
						{
							SendClientMessage(playerid, COLOR_GRAD3, "Nightvision Goggles");
						}
						if(PlayerInfo[playerid][pGun11] == 45)
						{
							SendClientMessage(playerid, COLOR_GRAD3, "Infared Goggles");
						}
						if(PlayerInfo[playerid][pGun12] != 0)
						{
							GetWeaponName(PlayerInfo[playerid][pGun12], WeaponName, 64);
							format(str, sizeof(str), "%s", WeaponName);
							SendClientMessage(FriskOffer[playerid], COLOR_BITEM, str);
						}
						for(new weap = 1; weap < 47; weap++)
						{
							if(HaveAdminWeapon(playerid, weap) == weap)
							{
								GetWeaponName(weap, WeaponName, 64);
								format(str, sizeof(str), "{FF0000}Admin Given %s", WeaponName);
								SendClientMessage(FriskOffer[playerid], COLOR_WHITE, str);
							}
						}
						format(str, sizeof(str), "* %s has frisked %s.", PlayerICName(FriskOffer[playerid]), PlayerICName(playerid));
						ProxDetector(30.0, FriskOffer[playerid], str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						FriskOffer[playerid] = 999;
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "The frisker is too far away.");
						return 1;
					}
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody offered to frisk you.");
				return 1;
			}
		}
		else if(strcmp(x_job, "gag", true) == 0)
		{
			if(IsPlayerGagged(playerid))
				return SendClientMessage(playerid, COLOR_GREY, "You are already gagged.");

			if(gagRequester[playerid] == INVALID_PLAYER_ID)
				return SendClientMessage(playerid, COLOR_GREY, "Nobody offered to gag you.");

			new gagger = gagRequester[playerid];

			if(!IsPlayerConnected(gagger)) {
				gagRequester[playerid] = INVALID_PLAYER_ID;
				return SendClientMessage(playerid, COLOR_GREY, "The person who tried to gag has left the server.");
			}

			if(GetDistanceBetweenPlayers(gagger, playerid) > 4) {
				gagRequester[playerid] = INVALID_PLAYER_ID;
				return SendClientMessage(playerid, COLOR_GREY, "The person who tried to gag you is too far away.");
			}

			ApplyGag(gagger, playerid);
			gagRequester[playerid] = INVALID_PLAYER_ID;
			return 1;
		}
		else if(strcmp(x_job, "business", true) == 0)
		{
			if(BizOffer[playerid] < 999)
			{
				if(GetMoney(playerid) >= BizPrice[playerid])
				{
					if(IsPlayerConnected(BizOffer[playerid]))
					{
						if(IsPlayerInRangeOfPlayer(12.0, playerid, BizOffer[playerid]))
						{
							if(PlayerInfo[playerid][pBizKey] != -1)
							{
								SendClientMessage(playerid, COLOR_GREY, "You already own a business.");
								return 1;
							}
							new biz = BizID[playerid];
							format(str, sizeof(str), "* You sold the %s business to %s, and received $%d.", BizInfo[biz][bName], PlayerICName(playerid), BizPrice[playerid]);
							SendClientMessage(BizOffer[playerid], COLOR_LIGHTBLUE, str);
							strmid(BizInfo[biz][bOwner], PlayerName(playerid), 0, strlen(PlayerName(playerid)), 255);
							if(BizInfo[biz][bLocked] == 1)
								format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s", BizInfo[biz][bName]);
							else
								format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s", BizInfo[biz][bName], BizInfo[biz][bOwner]);
							UpdateDynamic3DTextLabelText(Text3D:BizText[biz], COLOR_BIZTEXT, str);
							GiveMoney(playerid, -BizPrice[playerid]);
							GiveMoney(BizOffer[playerid], BizPrice[playerid]);
							format(str, sizeof(str), "* %s signs the contract.", PlayerICName(playerid));
							ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
							format(str, sizeof(str), "* Congratulations! You bought the %s business from %s. Type /help to view your commands.", BizInfo[biz][bName], PlayerICName(BizOffer[playerid]));
							SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
							PlayerInfo[BizOffer[playerid]][pBizKey] = -1;
							PlayerInfo[playerid][pBizKey] = biz;
							BizInfo[biz][bOwned] = 1;
							BizOffer[playerid] = 999;
							BizPrice[playerid] = 0;
							BizID[playerid] = 0;
							return 1;
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "The business owner is too far away.");
							return 1;
						}
					}
					return 1;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford the business.");
					return 1;
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "No-one offered you a business.");
				return 1;
			}
		}
		else if(strcmp(x_job, "divorce", true) == 0)
		{
			if(DivorceOffer[playerid] < 999)
			{
				if(IsPlayerConnected(DivorceOffer[playerid]))
				{
					if(IsPlayerInRangeOfPlayer(10.0, playerid, DivorceOffer[playerid]))
					{
						GetPlayerICName(DivorceOffer[playerid], giveplayer, sizeof(giveplayer));
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						format(str, sizeof(str), "* You have accepted %s's request to divorce.", giveplayer);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* %s has accepted your request to be divorce.", sendername);
						SendClientMessage(DivorceOffer[playerid], COLOR_LIGHTBLUE, str);
						ClearMarriage(playerid);
						ClearMarriage(DivorceOffer[playerid]);
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "The player that sent you the Divorce Papers is not near you.");
						return 1;
					}
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody sent you any Divorce Papers.");
				return 1;
			}
		}
		//Player Position when /accept family isn't needed at all as there already Freeze system ~Troy
		else if(strcmp(x_job, "family", true) == 0)
		{
			if(FamilyOffer[playerid] < 999)
			{
				if(IsPlayerConnected(FamilyOffer[playerid]))
				{
					if(PlayerInfo[FamilyOffer[playerid]][pFMember] != 255)
					{
						if(PlayerInfo[playerid][pMember] != 0 || PlayerInfo[playerid][pLeader] != 0 || PlayerInfo[playerid][pFMember] != 255)
							return SendClientMessage(playerid, COLOR_GREY, "You are already in a Faction / Family.");
						if(IsPlayerInAnyVehicle(playerid))
							return SendClientMessage(playerid, COLOR_GREY, "You can not be in a vehicle to do this.");
/*
						new Float:x, Float:y, Float:z;
						GetPlayerPos(playerid, x, y, z);
*/
						new family = PlayerInfo[FamilyOffer[playerid]][pFMember];
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						GetPlayerICName(FamilyOffer[playerid], giveplayer, sizeof(giveplayer));
						format(str, sizeof(str), "* You have accepted %s's request to join %s, you are now a Member of it.", giveplayer, FamilyInfo[family][FamilyName]);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* %s has accepted your request and is now a Member of your Family.", sendername);
						SendClientMessage(FamilyOffer[playerid], COLOR_LIGHTBLUE, str);
						/*
						SendClientMessage(playerid, COLOR_LIGHTRED, "* Use 'next' to Select the char you want to use.");
						SendClientMessage(playerid, COLOR_LIGHTRED, "* If you've found the Char you want to use, type 'done'.");
						TogglePlayerControllable(playerid, false);
						*/
						PlayerInfo[playerid][pFMember] = family;
						PlayerInfo[playerid][pRank] = 1;
						PlayerInfo[playerid][pClothes] = 0;
						FamilyOffer[playerid] = 999;
						FamilyInfo[family][FamilyMembers] ++;

//						SetPlayerPos(playerid, x, y, z);
						SaveFamilies();
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "The Player who invited you is no longer a Member of a Family.");
						return 1;
					}
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody sent you a Family Invite.");
				return 1;
			}
		}
		else if(strcmp(x_job, "faction", true) == 0)
		{
			if(FactionOffer[playerid] < 999)
			{
				if(IsPlayerConnected(FactionOffer[playerid]))
				{
					if(PlayerInfo[FactionOffer[playerid]][pLeader] == 0 && PlayerInfo[FactionOffer[playerid]][pRecruiter] == 0) return SendClientMessage(playerid, COLOR_GREY, "The Player who invited you is no longer a Faction Leader.");
					if(PlayerInfo[playerid][pMember] != 0 || PlayerInfo[playerid][pLeader] != 0 || PlayerInfo[playerid][pFMember] != 255)
					{
						SendClientMessage(playerid, COLOR_GREY, "You are already in a Faction / Family.");
						return 1;
					}
					PlayerInfo[playerid][pMember] = PlayerInfo[FactionOffer[playerid]][pMember];
					PlayerInfo[playerid][pRank] = 0;

					format(str, sizeof(str), "* You have accepted %s's request to join the %s, you are now a Member of it.", PlayerICName(FactionOffer[playerid]), GetFactionName(PlayerInfo[playerid][pMember]));
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* %s has accepted your request and is now a Member of the %s.", PlayerICName(playerid), GetFactionName(PlayerInfo[playerid][pMember]));
					SendClientMessage(FactionOffer[playerid], COLOR_LIGHTBLUE, str);

					FactionOffer[playerid] = 999;
					customRankName[playerid] = false;
					format(customRank[playerid], 24, "None");

					new member = PlayerInfo[playerid][pMember]; // stop revealing :)
					if(member == 2 || member == 8 || member == 11) return 1;
					return 1;
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody sent you a Faction Invite.");
				return 1;
			}
		}
		else if(strcmp(x_job, "group", true) == 0)
		{
			if(GroupOffer[playerid] < 999)
			{
				if(IsPlayerConnected(GroupOffer[playerid]))
				{
					if(PlayerInfo[playerid][pGroup] != 0) return SendClientMessage(playerid, COLOR_GREY, "You are already in a group, use /quitgroup.");
					if(PlayerInfo[playerid][pDonatedGroup] != 0) return SendClientMessage(playerid, COLOR_GREY, "You are already in a donated group, use /quitdonatedgroup.");
					if(PlayerInfo[GroupOffer[playerid]][pGroup] == GROUP_NONE) return SendClientMessage(playerid, COLOR_GREY, "That player is no longer a member of the group.");

					format(str, sizeof(str), "* You have accepted %s's request to join the %s, you are now a Member of it.", PlayerOOCName(GroupOffer[playerid]), GetPlayerGroupName(GroupOffer[playerid]));
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* %s has accepted your request and is now a Member of the %s.", PlayerOOCName(playerid), GetPlayerGroupName(GroupOffer[playerid]));
					SendClientMessage(GroupOffer[playerid], COLOR_LIGHTBLUE, str);

					PlayerInfo[playerid][pGroup] = PlayerInfo[GroupOffer[playerid]][pGroup];
					PlayerInfo[playerid][pGroupRank] = 0;

					GroupOffer[playerid] = 999;
					return 1;
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody sent you a Group Invite.");
				return 1;
			}
		}
		else if(strcmp(x_job, "witness", true) == 0)
		{
			if(MarryWitnessOffer[playerid] < 999)
			{
				if(IsPlayerConnected(MarryWitnessOffer[playerid]))
				{
					if(IsPlayerInRangeOfPlayer(10.0, playerid, MarryWitnessOffer[playerid]))
					{
						GetPlayerICName(MarryWitnessOffer[playerid], giveplayer, sizeof(giveplayer));
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						format(str, sizeof(str), "* You have accepted %s's request to be their Marriage Witness.", giveplayer);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* %s has accepted your request to be your Marriage Witness.", sendername);
						SendClientMessage(MarryWitnessOffer[playerid], COLOR_LIGHTBLUE, str);
						MarryWitness[MarryWitnessOffer[playerid]] = playerid;
						MarryWitnessOffer[playerid] = 999;
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "The player that requested you to be their Marriage Witness is not near you.");
						return 1;
					}
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody has asked you to be their Marriage Witness.");
				return 1;
			}
		}
		else if(strcmp(x_job, "marriage", true) == 0)
		{
			if(ProposeOffer[playerid] < 999)
			{
				if(!IsPlayerInRangeOfPoint(playerid, 100.0, -1268.99, 56.59, 991.94))
				{
					SendClientMessage(playerid, COLOR_GREY, "You are not at the Church in Jefferson.");
					return 1;
				}
				if(IsPlayerConnected(ProposeOffer[playerid]))
				{
					if(IsPlayerInRangeOfPlayer(10.0, playerid, ProposeOffer[playerid]))
					{
						if(MarryWitness[ProposeOffer[playerid]] == 999)
						{
							SendClientMessage(playerid, COLOR_GREY, "The proposer doesn't have a Marriage Witness.");
							return 1;
						}
						if(IsPlayerConnected(MarryWitness[ProposeOffer[playerid]]))
						{
							if(IsPlayerInRangeOfPlayer(12.0, ProposeOffer[playerid], MarryWitness[ProposeOffer[playerid]]))
							{
								GetPlayerICName(ProposeOffer[playerid], giveplayer, sizeof(giveplayer));
								strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
								format(str, sizeof(str), "* You have accepted %s's request to be your Husband.", giveplayer);
								SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
								format(str, sizeof(str), "* %s has accepted your request to be your Wife.", sendername);
								SendClientMessage(ProposeOffer[playerid], COLOR_LIGHTBLUE, str);
								format(str, sizeof(str), "Priest: %s do you take %s as your lovely Husband? (type 'yes', anything else will reject the Marriage)", sendername, giveplayer);
								SendClientMessage(playerid, COLOR_WHITE, str);
								MarriageCeremoney[playerid] = 1;
								ProposedTo[ProposeOffer[playerid]] = playerid;
								GotProposedBy[playerid] = ProposeOffer[playerid];
								MarryWitness[ProposeOffer[playerid]] = 999;
								ProposeOffer[playerid] = 999;
								return 1;
							}
							else
							{
								SendClientMessage(playerid, COLOR_GREY, "The Marriage Witness is not near your proposer.");
								return 1;
							}
						}
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "The player that proposed to you is not near you.");
						return 1;
					}
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody Proposed to you.");
				return 1;
			}
		}
		else if(strcmp(x_job, "ticket", true) == 0)
		{
/*			if(TicketOffer[playerid] < 999)
			{
				if(IsPlayerConnected(TicketOffer[playerid]))
				{
					if(IsPlayerInRangeOfPlayer(5.0, playerid, TicketOffer[playerid]))
					{
						if(PlayerInfo[playerid][pCash] < TicketMoney[playerid])
						{
							SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
							return 1;
						}
						GetPlayerICName(TicketOffer[playerid], giveplayer, sizeof(giveplayer));
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						format(str, sizeof(str), "* You have paid the Ticket of $%d to %s %s.", TicketMoney[playerid], GetPlayerRank(TicketOffer[playerid]), giveplayer);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* %s has paid your Ticket of $%d.", sendername, TicketMoney[playerid]);
						SendClientMessage(TicketOffer[playerid], COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* %s has paid the Ticket.", sendername);
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						PlayerInfo[playerid][pCash] = PlayerInfo[playerid][pCash] -TicketMoney[playerid];
						GivePlayerMoney(playerid, -TicketMoney[playerid]);
						TaxValue += TicketMoney[playerid];

						SAPD += (TicketMoney[playerid] / 2);
						
						TicketOffer[playerid] = 999;
						TicketMoney[playerid] = 0;
						SaveStuff();
						return 1;
					}
					else return SendClientMessage(playerid, COLOR_GREY, "The Officer is not near you.");
				}
			}
			else return SendClientMessage(playerid, COLOR_GREY, "Nobody offered you a Ticket.");*/
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "* Type /mytickets to view your tickets.");
		}
		else if(strcmp(x_job, "handshake", true) == 0)
		{
			if(HSHAKEOFFER[playerid] < 999)
			{
				if(IsPlayerConnected(HSHAKEOFFER[playerid]))
				{
					if(!IsPlayerInAnyVehicle(playerid))
					{
						if(IsPlayerInRangeOfPlayer(2.0, playerid, HSHAKEOFFER[playerid]))
						{
							if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can't use this at this time.");
							if(IsPlayerInAnyVehicle(HSHAKEOFFER[playerid])) return SendClientMessage(playerid, COLOR_GREY, "You can't use this at this time.");
							format(str, sizeof(str), "* You have shaken %s's hand.", PlayerICName(HSHAKEOFFER[playerid]));
							SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
							format(str, sizeof(str), "* %s has accepted the Handshake.", PlayerICName(playerid));
							SendClientMessage(HSHAKEOFFER[playerid], COLOR_LIGHTBLUE, str);
							ClearAnimationsEx(playerid);
							ClearAnimationsEx(HSHAKEOFFER[playerid]);
							SetPlayerFacingPlayer(playerid, HSHAKEOFFER[playerid]);
							SetPlayerFacingPlayer(HSHAKEOFFER[playerid], playerid);
							if(HShakeType[playerid] == 1)
							{
								ApplyAnimationEx(playerid, "GANGS", "hndshkaa", 4.0, 0, 0, 0, 0, 0);
								ApplyAnimationEx(HSHAKEOFFER[playerid], "GANGS", "hndshkaa", 4.0, 0, 0, 0, 0, 0);
							}
							else if(HShakeType[playerid] == 2)
							{
								ApplyAnimationEx(playerid, "GANGS", "hndshkba", 4.0, 0, 0, 0, 0, 0);
								ApplyAnimationEx(HSHAKEOFFER[playerid], "GANGS", "hndshkba", 4.0, 0, 0, 0, 0, 0);
							}
							else if(HShakeType[playerid] == 3)
							{
								ApplyAnimationEx(playerid, "GANGS", "hndshkda", 4.0, 0, 0, 0, 0, 0);
								ApplyAnimationEx(HSHAKEOFFER[playerid], "GANGS", "hndshkda", 4.0, 0, 0, 0, 0, 0);
							}
							else if(HShakeType[playerid] == 4)
							{
								ApplyAnimationEx(playerid, "GANGS", "hndshkea", 4.0, 0, 0, 0, 0, 0);
								ApplyAnimationEx(HSHAKEOFFER[playerid], "GANGS", "hndshkea", 4.0, 0, 0, 0, 0, 0);
							}
							else if(HShakeType[playerid] == 5)
							{
								ApplyAnimationEx(playerid, "GANGS", "hndshkfa", 4.0, 0, 0, 0, 0, 0);
								ApplyAnimationEx(HSHAKEOFFER[playerid], "GANGS", "hndshkfa", 4.0, 0, 0, 0, 0, 0);
							}
							else if(HShakeType[playerid] == 6)
							{
								ApplyAnimationEx(playerid, "GANGS", "prtial_hndshk_biz_01", 4.0, 0, 0, 0, 0, 0);
								ApplyAnimationEx(HSHAKEOFFER[playerid], "GANGS", "prtial_hndshk_biz_01", 4.0, 0, 0, 0, 0, 0);
							}
							HSHAKEOFFER[playerid] = 999;
							HShakeType[playerid] = 0;
							return 1;
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "You must be standing closer.");
							return 1;
						}
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "You must exit the vehicle.");
						return 1;
					}
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody offered to shake your hand.");
				return 1;
			}
		}
		else if(strcmp(x_job, "kiss", true) == 0)
		{
			if(KissOffer[playerid] < 999)
			{
				if(IsPlayerConnected(KissOffer[playerid]))
				{
					if(!IsPlayerInAnyVehicle(playerid))
					{
						if(IsPlayerInRangeOfPlayer(2.0, playerid, KissOffer[playerid]))
						{
							if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can't use this at this time.");
							if(IsPlayerInAnyVehicle(KissOffer[playerid])) return SendClientMessage(playerid, COLOR_GREY, "You can't use this at this time.");
							GetPlayerICName(KissOffer[playerid], giveplayer, sizeof(giveplayer));
							strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
							format(str, sizeof(str), "* You have kissed %s.", giveplayer);
							SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
							format(str, sizeof(str), "* %s has accepted the Kiss.", sendername);
							SendClientMessage(KissOffer[playerid], COLOR_LIGHTBLUE, str);
							ClearAnimationsEx(playerid);
							ClearAnimationsEx(KissOffer[playerid]);
							/*new Float: fX, Float: fY, Float: fZ, Float: fA, Float: dist = 0.9;
							SetPlayerFacingPlayer(playerid, KissOffer[playerid]); //face the person
							GetXYInFrontOfPlayer(playerid, fX, fY, fZ, fA, dist); //get the position infront of me to setpos them
							SetPlayerPos(KissOffer[playerid], fX, fY, fZ); //set their pos
							SetPlayerFacingPlayer(KissOffer[playerid], playerid); //make them face me*/
							if(KissType[playerid] == 1)
							{
								ApplyAnimationEx(playerid, "KISSING", "Grlfrd_Kiss_01", 4.0, 0, 0, 0, 0, 0);
								ApplyAnimationEx(KissOffer[playerid], "KISSING", "Playa_Kiss_01", 4.0, 0, 0, 0, 0, 0);
							}
							else if(KissType[playerid] == 2)
							{
								ApplyAnimationEx(playerid, "KISSING", "Grlfrd_Kiss_02", 4.0, 0, 0, 0, 0, 0);
								ApplyAnimationEx(KissOffer[playerid], "KISSING", "Playa_Kiss_02", 4.0, 0, 0, 0, 0, 0);
							}
							else if(KissType[playerid] == 3)
							{
								ApplyAnimationEx(playerid, "KISSING", "Grlfrd_Kiss_03", 4.0, 0, 0, 0, 0, 0);
								ApplyAnimationEx(KissOffer[playerid], "KISSING", "Playa_Kiss_03", 4.0, 0, 0, 0, 0, 0);
							}
							KissOffer[playerid] = 999;
							KissType[playerid] = 0;
							return 1;
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "You must be standing closer.");
							return 1;
						}
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "You must exit the vehicle.");
						return 1;
					}
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody offered to kiss you.");
				return 1;
			}
		}
		else if(strcmp(x_job, "contract", true) == 0)
		{
			if(ContractOffer[playerid] < 999)
			{
				if(GoChase[playerid] == 999)
				{
					if(IsPlayerConnected(ContractOffer[playerid]))
					{
						if(PlayerInfo[ContractID[playerid]][pHeadValue] != 0)
						{

							new hitname[MAX_PLAYER_NAME];
							strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
							GetPlayerICName(ContractOffer[playerid], giveplayer, sizeof(giveplayer));
							GetPlayerICName(ContractID[playerid], hitname, sizeof(hitname));
							format(str, sizeof(str), "* You have accepted the contract to kill %s, you will receive $%d when completed.", hitname, PlayerInfo[ContractID[playerid]][pHeadValue] /2);
							SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
							format(str, sizeof(str), "Hitman %s has accepted the contract to kill %s.", sendername, hitname);
							foreach( new i: Player )
							{
								if(PlayerInfo[i][pGroup] == GROUP_HITMAN)
								{
									SendClientMessage(i, COLOR_YELLOW, str);
								}
							}
							GoChase[playerid] = ContractID[playerid];
							ContractOffer[playerid] = 999;
							ContractID[playerid] = 999;
							return 1;
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "The contract has expired.");
							return 1;
						}
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You are currently busy with a contract.");
					return 1;
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody offered you a contract.");
				return 1;
			}
		}
		else if(strcmp(x_job, "boxing", true) == 0)
		{
			if(BoxOffer[playerid] < 999)
			{
				if(IsPlayerConnected(BoxOffer[playerid]))
				{
					new points;
					new mypoints;
					GetPlayerICName(BoxOffer[playerid], giveplayer, sizeof(giveplayer));
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					new level = PlayerInfo[BoxOffer[playerid]][pBoxSkill];
					if(level >= 0 && level <= 49)
					{
						points = 40;
					}
					else if(level >= 50 && level <= 99)
					{
						points = 50;
					}
					else if(level >= 100 && level <= 199)
					{
						points = 60;
					}
					else if(level >= 200 && level <= 399)
					{
						points = 70;
					}
					else if(level >= 400)
					{
						points = 80;
					}
					if(PlayerInfo[playerid][pJob] == 12)
					{
						new clevel = PlayerInfo[playerid][pBoxSkill];
						if(clevel >= 0 && clevel <= 49)
						{
							mypoints = 40;
						}
						else if(clevel >= 50 && clevel <= 99)
						{
							mypoints = 50;
						}
						else if(clevel >= 100 && clevel <= 199)
						{
							mypoints = 60;
						}
						else if(clevel >= 200 && clevel <= 399)
						{
							mypoints = 70;
						}
						else if(clevel >= 400)
						{
							mypoints = 80;
						}
					}
					else
					{
						mypoints = 30;
					}
					format(str, sizeof(str), "* You have accepted the Boxing Challenge from %s, and will fight with %d Health.", giveplayer, mypoints);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* %s has accepted your Boxing Challenge Request, you will fight with %d Health.", sendername, points);
					GetPlayerArmour(playerid, PlayerKevBox[playerid]);
					GetPlayerArmour(BoxOffer[playerid], PlayerKevBox[BoxOffer[playerid]]);

					SendClientMessage(BoxOffer[playerid], COLOR_LIGHTBLUE, str);
					SetPlayerHealth(playerid, mypoints);
					SetPlayerHealth(BoxOffer[playerid], points);
					SetPlayerArmour(playerid, 0);
					SetPlayerArmour(BoxOffer[playerid], 0);
					SetPlayerInterior(playerid, 5);
					SetPlayerInterior(BoxOffer[playerid], 5);
					SetPlayerPos(playerid, 762.9852, 2.4439, 1001.5942);
					SetPlayerFacingAngle(playerid, 131.8632);
					SetPlayerPos(BoxOffer[playerid], 758.7064, -1.8038, 1001.5942);
					SetPlayerFacingAngle(BoxOffer[playerid], 313.1165);
					TogglePlayerControllable(playerid, false);
					TogglePlayerControllable(BoxOffer[playerid], 0);
					SetCameraBehindPlayer(playerid);
					SetCameraBehindPlayer(BoxOffer[playerid]);
					displayCenterHUDInfo(playerid,  "~r~Waiting.", 8); //display for 8 seconds
					displayCenterHUDInfo(BoxOffer[playerid],  "~r~Waiting.", 8); //display for 8 seconds
					new name[MAX_PLAYER_NAME];
					new dstring[MAX_PLAYER_NAME];
					new wstring[MAX_PLAYER_NAME];
					GetPlayerICName(playerid, name, sizeof(name));
					format(dstring, sizeof(dstring), "%s", name);
					strmid(wstring, dstring, 0, strlen(dstring), 255);
					if(strcmp(Titel[TitelName], wstring, true) == 0)
					{
						format(str, sizeof(str), "Boxing News: Boxing Champion %s will fight VS %s, in 60 seconds (Grove Street Gym).",  sendername, giveplayer);
						BoxingAnn(COLOR_WHITE, str);
						TBoxer = playerid;
						BoxDelay = 60;
					}
					GetPlayerICName(BoxOffer[playerid], name, sizeof(name));
					format(dstring, sizeof(dstring), "%s", name);
					strmid(wstring, dstring, 0, strlen(dstring), 255);
					if(strcmp(Titel[TitelName], wstring, true) == 0)
					{
						format(str, sizeof(str), "Boxing News: Boxing Champion %s will fight VS %s, in 60 seconds (Grove Street Gym).",  giveplayer, sendername);
						BoxingAnn(COLOR_WHITE, str);
						TBoxer = BoxOffer[playerid];
						BoxDelay = 60;
					}
					BoxWaitTime[playerid] = 1;
					BoxWaitTime[BoxOffer[playerid]] = 1;
					if(BoxDelay < 1)
					{
						BoxDelay = 20;
					}
					InRing = 1;
					Boxer1 = BoxOffer[playerid];
					Boxer2 = playerid;
					SetPlayerArmedWeapon(playerid, 0);
					SetPlayerArmedWeapon(BoxOffer[playerid], 0);
					PlayerBoxing[playerid] = 1;
					PlayerBoxing[BoxOffer[playerid]] = 1;
					BoxOffer[playerid] = 999;
					return 1;
				}
				return 1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody offered you a Boxing Challenge.");
				return 1;
			}
		}
		else if(strcmp(x_job, "taxi", true) == 0)
		{
			if(PlayerInfo[playerid][pJob] != 14 && PlayerInfo[playerid][pMember] != 10)
				return SendClientMessage(playerid, COLOR_GREY, "You are not a taxi driver.");
			if(TransportDuty[playerid] != 1)
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not on duty.");
				return 1;
			}
			new
				customerid = GetPlayerIDFromName(LastCaller);
			if(IsPlayerConnected(customerid))
			{
				new Float:X, Float:Y, Float:Z;
				for(new i = 0; i < MAX_TAXICALLS; i++)
				{
					if(strlen(LastCaller) > 3)
					{
						if(strcmp(TaxiCInfo[i][taxiname], PlayerICName(customerid), false) == 0)
						{
							format(str, sizeof(str), "%s in %s, has been marked on your GPS.", TaxiCInfo[i][taxiname], TaxiCInfo[i][taxilocation]);
							SendClientMessage(playerid, -1, str);
							TaxiCID[playerid] = TaxiCInfoID[i];
							TaxiCInfoID[i] = -1;
							TaxiCInfo[i][taxiname] = "0";
							break;
						}
					}
				}
				GetPlayerPos(TaxiCID[playerid], X, Y, Z);
				SetPlayerCheckpointEx(playerid, X, Y, Z, 1.0);
				CP[playerid] = CHECKPOINT_TAXI_CALLER;
				LastCaller = "0";
				return 1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody called for a Taxi yet.");
				return 1;
			}
		}
		else if(strcmp(x_job, "bus", true) == 0)
		{
			if(TransportDuty[playerid] != 2)
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not a Bus Driver.");
				return 1;
			}
			if(BusCallTime[playerid] > 0)
			{
				SendClientMessage(playerid, COLOR_GREY, "You have already accepted a Bus Call.");
				return 1;
			}
			if(BusCall < 999)
			{
				if(IsPlayerConnected(BusCall))
				{
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					GetPlayerICName(BusCall, giveplayer, sizeof(giveplayer));
					format(str, sizeof(str), "* You have accepted the Bus Call from %s, you will see the marker untill you have reached it.", giveplayer);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* Bus Driver %s has accepted your Bus Call please wait at your current Position.", sendername);
					SendClientMessage(BusCall, COLOR_LIGHTBLUE, str);
					new Float:X, Float:Y, Float:Z;
					GetPlayerPos(BusCall, X, Y, Z);
					SetPlayerCheckpointEx(playerid, X, Y, Z, 5);
					displayCenterHUDInfo(playerid,  "~w~Bus Caller drive to the~n~~r~redmarker", 8); //display for 8 seconds
					BusCallTime[playerid] = 1;
					BusAccepted[playerid] = BusCall;
					BusCall = 999;
					return 1;
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody called for a Bus yet.");
				return 1;
			}
		}
		else if(strcmp(x_job, "medic", true) == 0)
		{
			if(PlayerInfo[playerid][pMember] == 4 || PlayerInfo[playerid][pLeader] == 4)
			{
				if(MedicCallTime[playerid] > 0)
				{
					SendClientMessage(playerid, COLOR_GREY, "You have already accepted a Medic Call.");
					return 1;
				}
				if(MedicCall < 999)
				{
					if(IsPlayerConnected(MedicCall))
					{
						new Float:X, Float:Y, Float:Z;
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						GetPlayerICName(MedicCall, giveplayer, sizeof(giveplayer));
						format(str, sizeof(str), "* You have accepted the Medic Call from %s, you have 30 Seconds to get there.", giveplayer);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "* After the 30 Seconds the Red Marker will disappear.");
						format(str, sizeof(str), "* Medic %s has accepted your Medic Call please wait at your current Position.", sendername);
						SendClientMessage(MedicCall, COLOR_LIGHTBLUE, str);
						GetPlayerPos(MedicCall, X, Y, Z);
						SetPlayerCheckpointEx(playerid, X, Y, Z, 5);
						displayCenterHUDInfo(playerid,  "~w~Medic Caller drive to the~n~~r~redmarker", 8); //display for 8 seconds
						MedicCallTime[playerid] = 1;
						MedicCall = 999;
						return 1;
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "Nobody called for a Medic yet.");
					return 1;
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not a Medic.");
				return 1;
			}
		}
		else if(strcmp(x_job, "lawyer", true) == 0)
		{
			if(PlayerInfo[playerid][pJob] == 2)
			{
				if(LawyerCallTime[playerid] > 0)
				{
					SendClientMessage(playerid, COLOR_GREY, "You have already accepted a Lawyer Call.");
					return 1;
				}
				if(LawyerCall < 999)
				{
					if(IsPlayerConnected(LawyerCall))
					{
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						GetPlayerICName(LawyerCall, giveplayer, sizeof(giveplayer));
						format(str, sizeof(str), "* You have accepted the Lawyer Call from %s, you have 30 Seconds to get there.", giveplayer);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "* After the 30 Seconds the Red Marker will disappear.");
						format(str, sizeof(str), "* Lawyer %s has accepted your Lawyer Call please wait at your current Position.", sendername);
						SendClientMessage(LawyerCall, COLOR_LIGHTBLUE, str);
						new Float:X, Float:Y, Float:Z;
						GetPlayerPos(LawyerCall, X, Y, Z);
						SetPlayerCheckpointEx(playerid, X, Y, Z, 5);
						displayCenterHUDInfo(playerid,  "~w~Lawyer Caller drive to the~n~~r~redmarker", 8); //display for 8 seconds
						LawyerCallTime[playerid] = 1;
						LawyerCall = 999;
						return 1;
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "Nobody called for a Lawyer yet.");
					return 1;
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not a Lawyer.");
				return 1;
			}
		}
		else if(strcmp(x_job, "mechanic", true) == 0)
		{
			if(PlayerInfo[playerid][pJob] != 7)
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not a Mechanic.");
				return 1;
			}
			if(MechanicCallTime[playerid] > 0)
			{
				SendClientMessage(playerid, COLOR_GREY, "You have already accepted a Mechanic Call.");
				return 1;
			}
			if(MechanicCall < 999)
			{
				if(IsPlayerConnected(MechanicCall))
				{
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					GetPlayerICName(MechanicCall, giveplayer, sizeof(giveplayer));
					format(str, sizeof(str), "* You have accepted the Mechanic Call from %s, you have 30 Seconds to get there.", giveplayer);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, "* After the 30 Seconds the Red Marker will disappear.");
					format(str, sizeof(str), "* Mechanic %s has accepted your Mechanic Call please wait at your current Position.", sendername);
					SendClientMessage(MechanicCall, COLOR_LIGHTBLUE, str);
					new Float:X, Float:Y, Float:Z;
					GetPlayerPos(MechanicCall, X, Y, Z);
					SetPlayerCheckpointEx(playerid, X, Y, Z, 5);
					displayCenterHUDInfo(playerid,  "~w~Mechanic Caller drive to the~n~~r~redmarker", 8); //display for 8 seconds
					MechanicCallTime[playerid] = 1;
					MechanicCall = 999;
					return 1;
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody called for a Mechanic yet.");
				return 1;
			}
		}
		else if(strcmp(x_job, "refill", true) == 0)
		{
			OnRefillAccept(playerid);
		}
		else if(strcmp(x_job, "live", true) == 0)
		{
			if(LiveOffer[playerid] < 999)
			{
				if(IsPlayerConnected(LiveOffer[playerid]))
				{
					if(IsPlayerInRangeOfPlayer(5.0, playerid, LiveOffer[playerid]))
					{
						SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You are frozen until the live conversation ends.");
						SendClientMessage(LiveOffer[playerid], COLOR_LIGHTBLUE, "* You are frozen until the live conversation ends (use /live again).");
						TogglePlayerControllable(playerid, false);
						TogglePlayerControllable(LiveOffer[playerid], 0);
						TalkingLive[playerid] = LiveOffer[playerid];
						TalkingLive[LiveOffer[playerid]] = playerid;
						LiveOffer[playerid] = 999;
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "You are to far away from the News Reporter.");
						return 1;
					}
				}
				return 1;
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody gave you a Live Conversation offer.");
				return 1;
			}
		}
		else if(strcmp(x_job, "defense", true) == 0)
		{
			if(DefenseOffer[playerid] < 999)
			{
				if(PlayerInfo[playerid][pCash] > DefensePrice[playerid])
				{
					if(IsPlayerConnected(DefenseOffer[playerid]))
					{
						new points = PlayerInfo[playerid][pWantedLevel];
						if(points == 0)
						{
							SendClientMessage(playerid, COLOR_GREY, "You are not wanted.");
							DefenseOffer[playerid] = 999;
							DefensePrice[playerid] = 0;
							return 1;
						}
						GetPlayerICName(DefenseOffer[playerid], giveplayer, sizeof(giveplayer));
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						format(str, sizeof(str), "* Lawyer %s has defended you and lowered your wanted level for $%d.", giveplayer, DefensePrice[playerid]);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* %s accepted the Defense, the $%d was transferred to your Bank Account.", sendername, DefensePrice[playerid]);
						SendClientMessage(DefenseOffer[playerid], COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "%s has accepted the defense from %s for $%d", sendername, giveplayer, DefensePrice[playerid]);
						PayLog(str);
						PlayerInfo[DefenseOffer[playerid]][pAccount] += DefensePrice[playerid];
						GiveMoney(playerid, -DefensePrice[playerid]);
						PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);
						PlayerInfo[playerid][pWantedLevel] = points -1;
						if(points == 6) SetPlayerToTeamColor(playerid);
						DefenseOffer[playerid] = 999;
						DefensePrice[playerid] = 0;
						return 1;
					}
					return 1;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					return 1;
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody has offered to defend you.");
				return 1;
			}
		}
		else if(strcmp(x_job, "bodyguard", true) == 0)
		{
			if(GuardOffer[playerid] < 999)
			{
				if(GetMoney(playerid) > GuardPrice[playerid])
				{
					if(IsPlayerConnected(GuardOffer[playerid]))
					{
						if(IsPlayerInRangeOfPlayer(12.0, playerid, GuardOffer[playerid]))
						{
							if(UseAcceptTimer[playerid]) return SendClientMessage(playerid, COLOR_GREY, "You must wait 60 seconds.");
							GetPlayerICName(GuardOffer[playerid], giveplayer, sizeof(giveplayer));
							strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
							format(str, sizeof(str), "* You accepted the protection for $%d from bodyguard %s.", GuardPrice[playerid], giveplayer);
							SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
							format(str, sizeof(str), "%s accepted the protection for $%d from bodyguard %s.", sendername, GuardPrice[playerid], giveplayer);
							PayLog(str);
							format(str, sizeof(str), "* %s accepted your protection the $%d was added to your bank account.", sendername, GuardPrice[playerid]);
							SendClientMessage(GuardOffer[playerid], COLOR_LIGHTBLUE, str);
							PlayerInfo[GuardOffer[playerid]][pAccount] += GuardPrice[playerid];
							GuardingPrice[playerid] = GuardPrice[playerid];
							GiveMoney(playerid, -GuardPrice[playerid]);
							GuardedBy[playerid] = GuardOffer[playerid];
							Guarding[GuardedBy[playerid]] = playerid;
							SetPlayerMarkerForPlayer(GuardOffer[playerid], playerid, COLOR_LIGHTRED);
							SetPlayerMarkerForPlayer(playerid, GuardOffer[playerid], COLOR_LIGHTRED);
							GuardOffer[playerid] = 999;
							GuardPrice[playerid] = 0;
							UseAcceptTimer[playerid] = 1;
							SetTimerEx("Timer_UseAccept", 60 *1000, 0, "i", playerid);
							return 1;
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "Your bodyguard is too far away.");
						}
						return 1;
					}
					return 1;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford the protection.");
					return 1;
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody offered you protection.");
				return 1;
			}
		}
		else if(strcmp(x_job, "heal", true) == 0)
		{
			if(healOffer[playerid] < 999)
			{
				if(GetMoney(playerid) > healPrice[playerid])
				{
					if(IsPlayerConnected(healOffer[playerid]))
					{
						if(IsPlayerInRangeOfPlayer(12.0, playerid, healOffer[playerid]))
						{
							if(UseAcceptTimer[playerid]) return SendClientMessage(playerid, COLOR_GREY, "You must wait 60 seconds.");
							GetPlayerICName(healOffer[playerid], giveplayer, sizeof(giveplayer));
							strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
							format(str, sizeof(str), "* You accepted the medical treatment for $%d from paramedic %s.", healPrice[playerid], giveplayer);
							SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
							format(str, sizeof(str), "%s accepted the medical treatment for $%d from paramedic %s.", sendername, healPrice[playerid], giveplayer);
							PayLog(str);
							format(str, sizeof(str), "* %s accepted your medical treatment, the $%d was added to your bank account.", sendername, healPrice[playerid]);
							SendClientMessage(healOffer[playerid], COLOR_LIGHTBLUE, str);
							PlayerInfo[healOffer[playerid]][pAccount] += healPrice[playerid];
							GiveMoney(playerid, -healPrice[playerid]);
							SetPlayerHealth(playerid, 100);
							healOffer[playerid] = 999;
							healPrice[playerid] = 0;
							UseAcceptTimer[playerid] = 1;
							SetTimerEx("Timer_UseAccept", 60 *1000, 0, "i", playerid);
							return 1;
						}
						else
						{
							SendClientMessage(playerid, COLOR_GREY, "The paramedic is too far away.");
						}
						return 1;
					}
					return 1;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford the heal.");
					return 1;
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody offered to heal you.");
				return 1;
			}
		}
		else if(strcmp(x_job, "vest", true) == 0)
		{
			if(VestOffer[playerid] < 999)
			{
				if(PlayerInfo[playerid][pCash] > VestPrice[playerid])
				{
					if(IsPlayerConnected(VestOffer[playerid]))
					{
						if(IsPlayerInRangeOfPlayer(3.0, playerid, VestOffer[playerid]))
						{
							if(UseAcceptTimer[playerid]) return SendClientMessage(playerid, COLOR_GREY, "You must wait 60 seconds.");
							GetPlayerICName(VestOffer[playerid], giveplayer, sizeof(giveplayer));
							strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
							format(str, sizeof(str), "* You accepted the kevlar vest for $%d from bodyguard %s.", VestPrice[playerid], giveplayer);
							SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
							format(str, sizeof(str), "* %s accepted your vest, and the $%d was added to your bank account.", sendername, VestPrice[playerid]);
							SendClientMessage(VestOffer[playerid], COLOR_LIGHTBLUE, str);
							format(str, sizeof(str), "%s has accepted the kevlar vest from %s for $%d", sendername, giveplayer, VestPrice[playerid]);
							PayLog(str);
							new Float:armor;
							GetPlayerArmour(playerid, armor);
							if(armor > 0) format(str, sizeof(str), "* %s takes off their vest and straps on a new one.", PlayerICName(playerid));
							else format(str, sizeof(str), "* %s grabs the kevlar vest and straps it on.", PlayerICName(playerid));
							ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
							PlayerInfo[VestOffer[playerid]][pAccount] += VestPrice[playerid];
							GiveMoney(playerid, -VestPrice[playerid]);
							VestOffer[playerid] = 999;
							VestPrice[playerid] = 0;
							SetPlayerArmour(playerid, 50.0);
							UseAcceptTimer[playerid] = 1;
							SetTimerEx("Timer_UseAccept", 60 *1000, 0, "i", playerid);
							return 1;
						}
						else
						{
							SendClientMessage(playerid, COLOR_GRAD1, "You're too far away.");
							return 1;
						}
					}
					return 1;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					return 1;
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody offered you any Protection.");
				return 1;
			}
		}
		else if(strcmp(x_job, "pot", true) == 0)
		{
			if(PotOffer[playerid] < 999)
			{
				if(PlayerInfo[playerid][pCash] > PotPrice[playerid])
				{
					if(PlayerInfo[playerid][pCannabis] < 7)
					{
						if(IsPlayerConnected(PotOffer[playerid]))
						{
							if(PotGram[playerid] != PlayerInfo[PotOffer[playerid]][pCannabis]) SendClientMessage(playerid, COLOR_GREY, "That player doesn't have enough pot.");
							GetPlayerICName(PotOffer[playerid], giveplayer, sizeof(giveplayer));
							strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
							format(str, sizeof(str), "* You bought %d grams of pot for $%d from drug dealer %s.", PotGram[playerid], PotPrice[playerid], giveplayer);
							SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
							format(str, sizeof(str), "* %s has bought your %d grams, the $%d was added to your bank account.", sendername, PotGram[playerid], PotPrice[playerid]);
							SendClientMessage(PotOffer[playerid], COLOR_LIGHTBLUE, str);
							PlayerInfo[PotOffer[playerid]][pAccount] += PotPrice[playerid];
							PlayerInfo[PotOffer[playerid]][pDrugsSkill] ++;
							GiveMoney(playerid, -PotPrice[playerid]);
							PlayerInfo[playerid][pCannabis] += PotGram[playerid];
							PlayerInfo[PotOffer[playerid]][pCannabis] -= PotGram[playerid];
							if(PlayerInfo[PotOffer[playerid]][pDrugsSkill] == 50)
							{
								SendClientMessage(PotOffer[playerid], COLOR_YELLOW, "* Your Drug Dealer Skill is now Level 2, you can buy more Grams.");
							}
							else if(PlayerInfo[PotOffer[playerid]][pDrugsSkill] == 100)
							{
								SendClientMessage(PotOffer[playerid], COLOR_YELLOW, "* Your Drug Dealer Skill is now Level 3, you can buy more Grams.");
							}
							else if(PlayerInfo[PotOffer[playerid]][pDrugsSkill] == 200)
							{
								SendClientMessage(PotOffer[playerid], COLOR_YELLOW, "* Your Drug Dealer Skill is now Level 4, you can buy more Grams.");
							}
							else if(PlayerInfo[PotOffer[playerid]][pDrugsSkill] == 400)
							{
								SendClientMessage(PotOffer[playerid], COLOR_YELLOW, "* Your Drug Dealer Skill is now Level 5, you can buy more Grams.");
							}
							PotOffer[playerid] = 999;
							PotPrice[playerid] = 0;
							PotGram[playerid] = 0;
							return 1;
						}
						return 1;
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "You are fully loaded with Pot, /usepot or /drop it first.");
						return 1;
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					return 1;
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody offered you any Pot.");
				return 1;
			}
		}
		else if(strcmp(x_job, "materials", true) == 0)
		{
			if(MatsOffer[playerid] < 999)
			{
				if(PlayerInfo[playerid][pCash] > MatsPrice[playerid])
				{
					if(IsPlayerConnected(MatsOffer[playerid]))
					{
						giveplayerid = MatsOffer[playerid];
						if(MatsGram2[playerid] != PlayerInfo[giveplayerid][pMats])
						{
							SendClientMessage(playerid, COLOR_GREY, "Transaction failed! The deal has been changed.");
							SendClientMessage(giveplayerid, COLOR_GREY, "Transaction failed! The deal has been changed.");
							MatsOffer[playerid] = 999;
							MatsPrice[playerid] = 0;
							MatsGram[playerid] = 0;
							MatsGram2[playerid] = 0;
							return 1;
						}
						GetPlayerICName(MatsOffer[playerid], giveplayer, sizeof(giveplayer));
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						format(str, sizeof(str), "* You bought %d materials for $%d from arms dealer %s.", MatsGram[playerid], MatsPrice[playerid], giveplayer);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* %s has bought your %d materials, the $%d was added to your bank account.", sendername, MatsGram[playerid], MatsPrice[playerid]);
						SendClientMessage(MatsOffer[playerid], COLOR_LIGHTBLUE, str);
						PlayerInfo[MatsOffer[playerid]][pAccount] += MatsPrice[playerid];
						GiveMoney(playerid, -MatsPrice[playerid]);
						PlayerInfo[playerid][pMats] += MatsGram[playerid];
						PlayerInfo[MatsOffer[playerid]][pMats] -= MatsGram[playerid];
						MatsOffer[playerid] = 999;
						MatsPrice[playerid] = 0;
						MatsGram[playerid] = 0;
						MatsGram2[playerid] = 0;
						return 1;
					}
					return 1;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					return 1;
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody offered you any Materials.");
				return 1;
			}
		}
		/*else if(strcmp(x_job, "products", true) == 0)
		{
			if(ProdOffer[playerid] < 999)
			{
				if(PlayerInfo[playerid][pCash] > ProdPrice[playerid])
				{
					if(IsPlayerConnected(ProdOffer[playerid]))
					{
						GetPlayerICName(ProdOffer[playerid], giveplayer, sizeof(giveplayer));
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						format(str, sizeof(str), "* You bought %d products for $%d from product dealer %s.", ProdAmount[playerid], ProdPrice[playerid], giveplayer);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* %s has bought your %d products, the $%d was added to your bank account.", sendername, ProdAmount[playerid], ProdPrice[playerid]);
						SendClientMessage(ProdOffer[playerid], COLOR_LIGHTBLUE, str);
						PlayerInfo[ProdOffer[playerid]][pAccount] += ProdPrice[playerid];
						GiveMoney(playerid, -ProdPrice[playerid]);
						PlayerInfo[playerid][pProducts] += ProdAmount[playerid];
						PlayerInfo[ProdOffer[playerid]][pProducts] -= ProdAmount[playerid];
						ProdOffer[playerid] = 999;
						ProdPrice[playerid] = 0;
						ProdAmount[playerid] = 0;
						return 1;
					}
					return 1;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					return 1;
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody offered you any Products.");
				return 1;
			}
		}*/
		else if(strcmp(x_job, "repair", true) == 0)
		{
			if(RepairOffer[playerid] < 999)
			{
				if(PlayerInfo[playerid][pCash] > RepairPrice[playerid])
				{
					if(IsPlayerInAnyVehicle(playerid))
					{
						if(IsPlayerConnected(RepairOffer[playerid]))
						{
							new level = PlayerInfo[RepairOffer[playerid]][pMechSkill];
							if(UseAcceptTimer[playerid]) return SendClientMessage(playerid, COLOR_GREY, "You must wait 60 seconds.");
							RepairCar[playerid] = GetPlayerVehicleID(playerid);
							SetVehicleHealth(RepairCar[playerid], 999.0);
							format(str, sizeof(str), "* Mechanic %s has repaired your vehicle for %d.", PlayerICName(RepairOffer[playerid]), RepairPrice[playerid]);
							SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
							format(str, sizeof(str), "* You fixed %s's vehicle, the $%d has been added to your Bank Account.", PlayerICName(playerid), RepairPrice[playerid]);
							SendClientMessage(RepairOffer[playerid], COLOR_LIGHTBLUE, str);
							format(str, sizeof(str), "* %s has repaired %s's vehicle.", PlayerICName(RepairOffer[playerid]), PlayerICName(playerid));
							ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
							format(str, sizeof(str), "%s has accepted the repairs from %s for $%d", PlayerName(playerid), PlayerName(RepairOffer[playerid]), RepairPrice[playerid]);
							PayLog(str);
							PlayerInfo[RepairOffer[playerid]][pMechSkill]++;
							if(PlayerInfo[RepairOffer[playerid]][pMechSkill] == 50)
							{
								SendClientMessage(RepairOffer[playerid], COLOR_YELLOW, "* Your Mechanic Skill is now Level 2, you can add more Fuel to Players Cars.");
							}
							else if(PlayerInfo[RepairOffer[playerid]][pMechSkill] == 100)
							{
								SendClientMessage(RepairOffer[playerid], COLOR_YELLOW, "* Your Mechanic Skill is now Level 3, you can add more Fuel to Players Cars.");
							}
							else if(PlayerInfo[RepairOffer[playerid]][pMechSkill] == 200)
							{
								SendClientMessage(RepairOffer[playerid], COLOR_YELLOW, "* Your Mechanic Skill is now Level 4, you can add more Fuel to Players Cars.");
							}
							else if(PlayerInfo[RepairOffer[playerid]][pMechSkill] == 400)
							{
								SendClientMessage(RepairOffer[playerid], COLOR_YELLOW, "* Your Mechanic Skill is now Level 5, you can add more Fuel to Players Cars.");
							}
							PlayerInfo[RepairOffer[playerid]][pAccount] += RepairPrice[playerid];
							GiveMoney(playerid, -RepairPrice[playerid]);
							new rtf;
							if(level >= 0 && level <= 50) rtf = 180;//level 1
							else if(level >= 51 && level <= 100) rtf = 140;//level 2
							else if(level >= 101 && level <= 200) rtf = 100;//level 3
							else if(level >= 201 && level <= 400) rtf = 80;//level 4
							else if(level >= 401) rtf = 40;//level 5
							PlayerInfo[RepairOffer[playerid]][pRepairTime] = rtf;
							RepairOffer[playerid] = 999;
							RepairPrice[playerid] = 0;
							UseAcceptTimer[playerid] = 1;
							SetTimerEx("Timer_UseAccept", 60 *1000, 0, "i", playerid);
							return 1;
						}
						return 1;
					}
					return 1;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You can't afford that.");
					return 1;
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Nobody offered you to repair your vehicle.");
				return 1;
			}
		}
		else if(strcmp(x_job, "death", true) == 0)
		{
			if(pDeathState[playerid] != PLAYER_STATE_REVIVE) return SendClientMessage(playerid, COLOR_GREY, "You are not injured.");
			new Float:phealth;
			GetPlayerHealth(playerid, phealth);
			if(phealth > 50) return SendClientMessage(playerid, COLOR_GREY, "You can only use this with 50 HP or less.");
			SetPlayerHealth(playerid, 0);
			return 1;
		}
		else if(strcmp(x_job, "call", true) == 0)
		{
			if(PlayerInfo[playerid][pMember] == 4 || PlayerInfo[playerid][pLeader] == 4 || FakeFaction[playerid] == FACTION_FMD)
			{
				if(ParaOnACall[playerid] == 0)
				{
					tmp = strtok(cmdtext, idx);
					if(!strlen(tmp))
					{
						SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /accept call [playerid/PartOfName]");
						return 1;
					}
					new victim;
					victim = ReturnUser(tmp);
					if(IsPlayerConnected(victim))
					{
						if(victim != INVALID_PLAYER_ID)
						{
							if(victim == playerid)
								return SendClientMessage(playerid, COLOR_GREY, "You cannot accept your own calls.");
							if(pDeathState[victim] == PLAYER_STATE_REVIVE)
							{
								if(CallAccepted[victim] == 0)
								{
									PlayerParamedic[victim] = playerid;
									PlayerVictim[playerid] = victim;
									format(str, sizeof(str), "Dispatch: Paramedic %s has accepted the EMS call of %s.", PlayerICName(playerid), PlayerICName(victim));
									SendParaMessage(COLOR_DOCTOR, str);
									format(str, sizeof(str), "* You have accepted the call of %s, a checkpoint was set at their location.", PlayerICName(victim));
									SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
									SendClientMessage(playerid, COLOR_DOCTOR, "A beacon to your victim's location has been sent to the Police Department.");
									CP[playerid] = CHECKPOINT_INJURED;
									SetPlayerCheckpointEx(playerid, DeathPos[victim][DeathX], DeathPos[victim][DeathY], DeathPos[victim][DeathZ], 3);
									format(str, sizeof(str), "* Paramedic %s has accepted your EMS call.", PlayerICName(playerid));
									SendClientMessage(victim, COLOR_LIGHTBLUE, str);
									CallAccepted[victim] = 1;
									ParaOnACall[playerid] = 1;
									for(new i = 0; i <= GetPlayerPoolSize(); i++)
									{
										if(PlayerInfo[i][pMember] == 1)
										{
											SetPlayerMarkerForPlayer(i, victim, COLOR_DOCTOR);
										}
									}
									return 1;
								}
								else
								{
									SendClientMessage(playerid, COLOR_GREY, "That call was already accepted.");
									return 1;
								}
							}
							else
							{
								SendClientMessage(playerid, COLOR_GREY, "That player is not injured.");
								return 1;
							}
						}
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "That player is offline.");
						return 1;
					}
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "You are already on a call, \"/cancel call\" in order to cancel it.");
					return 1;
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "You are not a Paramedic.");
				return 1;
			}
		}
		else
		{
			return 1;
		}
		return 1;
	}
	if(strcmp(cmd, "/family", true) == 0 || strcmp(cmd, "/f", true) == 0)
	{
		if(famTog[PlayerInfo[playerid][pFMember]] == true) return SendClientMessage(playerid, COLOR_GREY, "The family chat has been toggled off.");
		if(PlayerInfo[playerid][pFMember] == 255) return SendClientMessage(playerid, COLOR_GREY, "You are not a member of a Family.");
		if(gFam[playerid]) return SendClientMessage(playerid, COLOR_GREY, "Family chat is currently turned off, use /togfam.");
		if(famMuted[playerid] == true) return SendClientMessage(playerid, COLOR_GREY, "You can't use this chat as you're currently muted.");
		new length = strlen(cmdtext);
		while((idx < length) && (cmdtext[idx] <= ' '))
		{
			idx++;
		}
		new offset = idx;
		new result[128];
		while((idx < length) && ((idx - offset) < (sizeof(result) - 1)))
		{
			result[idx - offset] = cmdtext[idx];
			idx++;
		}
		result[idx - offset] = EOS;
		if(!strlen(result))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} (/f)amily [family chat]");
			return 1;
		}
		format(str, sizeof(str), "(( ** (%d) %s %s: %s ** ))", PlayerInfo[playerid][pRank], GetPlayerRank(playerid), PlayerICName(playerid), result);
		SendNewFamilyMessage(PlayerInfo[playerid][pFMember], TEAM_AZTECAS_COLOR, str);
		format(str, sizeof(str), "(( ** (%d) %s %s: %s ** ))", PlayerInfo[playerid][pRank], GetPlayerRank(playerid), PlayerOOCName(playerid), result);
		SendShadowMsg(COLOR_GREEN, str);
		if(beingSpectatedID[playerid] != INVALID_PLAYER_ID)
		{
			if(IsPlayerConnected(beingSpectatedID[playerid]) && SpectatingID[beingSpectatedID[playerid]] == playerid)
			{
				format(str, sizeof(str), "[AdmWrn] %s used /f: %s", PlayerICName(playerid), result);
				SendClientMessage(beingSpectatedID[playerid], COLOR_YELLOW, str);
			}
		}
		return 1;
	}
	if(strcmp(cmd, "/news", true) == 0)
	{
		if(PlayerInfo[playerid][pMember] != 9 && PlayerInfo[playerid][pLeader] != 9)
			return SendClientMessage(playerid, COLOR_GREY, "You are not a news reporter.");
		if(PlayerInfo[playerid][pOnDuty] == 0) return SendClientMessage(playerid, COLOR_GREY, "You need to be on duty.");
		if(PlayerInfo[playerid][pRank] < 1)
			return SendClientMessage(playerid, COLOR_GREY, "You must be at least rank 1 to use this.");
		if(gNews[playerid]) return SendClientMessage(playerid, COLOR_GREY, "News chat is currently turned off, use /tognews.");
		if(!IsPlayerInAnyVehicle(playerid))
		{
			if(PlayerInfo[playerid][pRank] < 4)
				return SendClientMessage(playerid, COLOR_GREY, "You must be at least rank 4 to use this outside of a vehicle.");
			strmid(tmp, cmdtext, idx, strlen(cmdtext));
			new length = strlen(cmdtext);
			while((idx < length) && (cmdtext[idx] <= ' '))
			{
				idx++;
			}
			new offset = idx;
			new result[128];
			while((idx < length) && ((idx - offset) < (sizeof(result) - 1)))
			{
				result[idx - offset] = cmdtext[idx];
				idx++;
			}
			result[idx - offset] = EOS;
			if(!strlen(result))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /news [text]");
				return 1;
			}
			format(str, sizeof(str), "SAN %s: %s", PlayerICName(playerid), result);
			OOCNews(COLOR_NEWS, str);
		}
		else
		{
			new vehid = GetPlayerVehicleID(playerid);
			new newcar = GetVehicleModel(vehid);
			if(newcar == 488 || newcar == 582 || PlayerInfo[playerid][pRank] >= 3)
			{
				strmid(tmp, cmdtext, idx, strlen(cmdtext));
				new length = strlen(cmdtext);
				while((idx < length) && (cmdtext[idx] <= ' '))
				{
					idx++;
				}
				new offset = idx;
				new result[128];
				while((idx < length) && ((idx - offset) < (sizeof(result) - 1)))
				{
					result[idx - offset] = cmdtext[idx];
					idx++;
				}
				result[idx - offset] = EOS;
				if(!strlen(result))
				{
					SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /news [text]");
					return 1;
				}
				format(str, sizeof(str), "SAN %s: %s", PlayerICName(playerid), result);
				OOCNews(COLOR_NEWS, str);
			}
			else return SendClientMessage(playerid, COLOR_GREY, "You must be at least rank 3 to use this on foot/in any vehicle.");
		}
		return 1;
	}
	if(strcmp(cmd, "/live", true) == 0)
	{
		if(PlayerInfo[playerid][pMember] == 9 || PlayerInfo[playerid][pLeader] == 9)
		{
			if(TalkingLive[playerid] != 255)
			{
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "* Live conversation ended.");
				SendClientMessage(TalkingLive[playerid], COLOR_LIGHTBLUE, "* Live conversation ended.");
				TogglePlayerControllable(playerid, true);
				TogglePlayerControllable(TalkingLive[playerid], 1);
				TalkingLive[TalkingLive[playerid]] = 255;
				TalkingLive[playerid] = 255;
				return 1;
			}
			if(PlayerInfo[playerid][pRank] < 3)
			{
				SendClientMessage(playerid, COLOR_GREY, "You must be at least rank 3 to do live interviews.");
				return 1;
			}
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /live [playerid/PartOfName] [player's live name]");
				return 1;
			}
			giveplayerid = ReturnUser(tmp);
			if(IsPlayerConnected(giveplayerid))
			{
				if(giveplayerid != INVALID_PLAYER_ID)
				{
					if(IsPlayerInRangeOfPlayer(5.0, playerid, giveplayerid))
					{
						if(giveplayerid == playerid)
						{
							SendClientMessage(playerid, COLOR_GREY, "You can't talk live with yourself.");
							return 1;
						}
						strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
						strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
						format(str, sizeof(str), "* You offered %s to have a live conversation.", giveplayer);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* %s offered you to have a live conversation, type (/accept live) to accept.", sendername);
						SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
						LiveOffer[giveplayerid] = playerid;
						strmid(LiveName[giveplayerid], cmdtext, idx + 1, strlen(cmdtext));
						if(strlen(LiveName[giveplayerid]) < 1)
						{
							SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have not given that player a live prefix, their prefix has been autoset to 'Anonymous'");
							LiveName[giveplayerid] = "Anonymous";
						}
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "You are too far away from that player.");
						return 1;
					}
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Invalid ID/Name.");
				return 1;
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not a news reporter.");
		}
		return 1;
	}
	if(strcmp(cmd, "/sellmats", true) == 0)
	{
		if(PlayerInfo[playerid][pJob] != 9 && PlayerInfo[playerid][pGroup] != GROUP_CRIME)
		{
			SendClientMessage(playerid, COLOR_GREY, "You are not a Arms Dealer.");
			return 1;
		}
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /sellmats [playerid/PartOfName] [amount] [price]");
			return 1;
		}
		new playa;
		playa = ReturnUser(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /sellmats [playerid/PartOfName] [amount] [price]");
			return 1;
		}
		new needed;
		needed = strvalEx(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /sellmats [playerid/PartOfName] [amount] [price]");
			return 1;
		}
		new money;
		money = strvalEx(tmp);
		if(needed < 1 || needed > 100000)
		{
			SendClientMessage(playerid, COLOR_GREY, "Materials can't be lower than 1, or above 100000.");
			return 1;
		}
		if(money < 1 || money > 150000)
		{
			SendClientMessage(playerid, COLOR_GREY, "Price can't be lower than $1, or above $150,000.");
			return 1;
		}
		if(needed > PlayerInfo[playerid][pMats])
		{
			SendClientMessage(playerid, COLOR_GREY, "You don't have that much materials with you.");
			return 1;
		}
		if(IsPlayerConnected(playa))
		{
			if(playa != INVALID_PLAYER_ID)
			{
				if(IsPlayerInRangeOfPlayer(8.0, playerid, playa))
				{
					if(playa == playerid)
					{
						SendClientMessage(playerid, COLOR_GREY, "You can't sell to yourself.");
						return 1;
					}
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					strmid(giveplayer, PlayerICName(playa), 0, MAX_PLAYER_NAME);
					format(str, sizeof(str), "* You offered %s to buy %d materials for $%d .", giveplayer, needed, money);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* Arms Dealer %s wants to sell you %d materials for $%d, (type /accept materials) to buy.", sendername, needed, money);
					SendClientMessage(playa, COLOR_LIGHTBLUE, str);
					MatsOffer[playa] = playerid;
					MatsPrice[playa] = money;
					MatsGram[playa] = needed;
					MatsGram2[playa] = PlayerInfo[playerid][pMats];
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
				}
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
		}
		return 1;
	}
	if(strcmp(cmd, "/eject", true) == 0)
	{
		if(IsPlayerInAnyVehicle(playerid))
		{
			if(GetPlayerState(playerid) != 2)
			{
				SendClientMessage(playerid, COLOR_GREY, "You can only eject people as the driver.");
				return 1;
			}
			tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /eject [playerid/PartOfName]");
				return 1;
			}
			giveplayerid = ReturnUser(tmp);
			if(IsPlayerConnected(giveplayerid))
			{
				if(giveplayerid != INVALID_PLAYER_ID)
				{
					if(giveplayerid == playerid)
					{
						SendClientMessage(playerid, COLOR_GREY, "You cannot Eject yourself.");
						return 1;
					}
					new vehid;
					vehid = GetPlayerVehicleID(playerid);
					if(IsPlayerInVehicle(giveplayerid, vehid))
					{
						GetPlayerICName(playerid, sendername, sizeof(sendername));
						strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
						format(str, sizeof(str), "* You have thrown %s out of the car.", giveplayer);
						SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* You have been thrown out the car by %s.", sendername);
						SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
						format(str, sizeof(str), "* %s has thrown %s out of the vehicle.", sendername, giveplayer);
						ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
						if(GetVehicleModel(GetPlayerVehicleID(giveplayerid)) == 416 && pDeathState[giveplayerid] == PLAYER_STATE_TREAT) // fix to injured players getting /eject'd but still frozen/not in a normal state.
						{
							pDeathState[giveplayerid] = PLAYER_STATE_NORMAL;
							TogglePlayerControllable(giveplayerid, true);
						}
						RemovePlayerFromVehicle(giveplayerid);
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "That player is not in your Car.");
						return 1;
					}
				}
			}
			else
			{
				SendClientMessage(playerid, COLOR_GREY, "Invalid ID/Name.");
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "You need to be in a Vehicle to use this.");
		}
		return 1;
	}
	if(strcmp(cmd, "/kiss", true) == 0)
	{
		if(PlayerInfo[playerid][pJailed] != 0) return SendClientMessage(playerid, COLOR_GREY, "You can do that while in jail!");
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFF0000FF, "{00BFFF}Usage:{FFFFFF} /kiss [playerid/PartOfName] [1-3]");
			return 1;
		}
		giveplayerid = ReturnUser(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFF0000FF, "{00BFFF}Usage:{FFFFFF} /kiss [playerid/PartOfName] [1-3]");
			return 1;
		}
		new snumber;
		snumber = strval(tmp);
		if(snumber < 1 || snumber > 3)
		{
			SendClientMessage(playerid, 0xFF0000FF, "{00BFFF}Usage:{FFFFFF} /kiss [playerid/PartOfName] [1-3]");
			return 1;
		}
		if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can't use this at this time.");
		if(IsPlayerConnected(giveplayerid))
		{
			if(giveplayerid != INVALID_PLAYER_ID)
			{
				if(IsPlayerInRangeOfPlayer(2.0, playerid, giveplayerid))
				{
					if(IsPlayerInAnyVehicle(giveplayerid)) return SendClientMessage(playerid, COLOR_GREY, "You can't use this at this time.");
					if(giveplayerid == playerid)
					{
						SendClientMessage(playerid, COLOR_GREY, "You cannot offer to kiss yourself.");
						return 1;
					}
					strmid(giveplayer, PlayerICName(giveplayerid), 0, MAX_PLAYER_NAME);
					strmid(sendername, PlayerICName(playerid), 0, MAX_PLAYER_NAME);
					format(str, sizeof(str), "* You offered to kiss %s.", giveplayer);
					SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
					format(str, sizeof(str), "* %s has offered to kiss you (type /accept kiss), to accept it.", sendername);
					SendClientMessage(giveplayerid, COLOR_LIGHTBLUE, str);
					KissOffer[giveplayerid] = playerid;
					KissType[giveplayerid] = snumber;
				}
				else
				{
					SendClientMessage(playerid, COLOR_GREY, "That player is not near you.");
					return 1;
				}
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_GREY, "That player is Offline.");
			return 1;
		}
		return 1;
	}
	return 1;
}