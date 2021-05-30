new fsafestr[128];

#define DIALOG_FSAFES_SHOW	15212
#define SAFE_WEAPONS_LIMIT	20

forward LogSafe(playerid, action[128]);

public LogSafe(playerid, action[128])
{
	new query[200];
	new Year, Month, Day, datestr[32];
	getdate(Year, Month, Day);
	format(datestr, 32, "%02d/%02d/%d", Month, Day, Year);

	mysql_format(sqlGameConnection, query, sizeof(query), "INSERT INTO `familysafelogs`(`Player`, `PlayerSQL`, `Action`, `FamilyID`, `Date`) VALUES ('%e', %d, '%e', %d, '%e')", PlayerName(playerid), PlayerInfo[playerid][pID], action, PlayerInfo[playerid][pFMember], datestr);
	mysql_pquery(sqlGameConnection, query, "LogSafeSQL", "sdsds", PlayerName(playerid), PlayerInfo[playerid][pID], action, PlayerInfo[playerid][pFMember], datestr);
	return 1;
}

forward LogSafeSQL(name[24], sql, action[128], family, date[32]);
public LogSafeSQL(name[24], sql, action[128], family, date[32])
{
	printf("[FAMILY DEBUG] %s (SQL %d, Slot %d): %s", name, sql, family, action);
	return 1;
}

CMD:fsafe(playerid, params[])
{
	new fam = PlayerInfo[playerid][pFMember];
	if(fam == 255) return SendClientMessage(playerid, COLOR_GREY, "You are not a member of a family.");
	if(FamilyInfo[fam][FamilySafe] == 0) return SendClientMessage(playerid, COLOR_GRAD2, "Your family does not own a safe.");
	if(!IsPlayerInRangeOfPoint(playerid, 2, FamilyInfo[fam][FamilySafePos][0], FamilyInfo[PlayerInfo[playerid][pFMember]][FamilySafePos][1], FamilyInfo[PlayerInfo[playerid][pFMember]][FamilySafePos][2])) return SendClientMessage(playerid, COLOR_GRAD2, "You are not at your Family Safe.");
	if(GetPlayerVirtualWorld(playerid) != FamilyInfo[fam][FamilySafeWorld]) return SendClientMessage(playerid, COLOR_GREY, "You are not at your Family Safe.");
	new sec[24], thing[24], amount[24];
	if(sscanf(params, "s[24]s[24]s[24]", sec, thing, amount)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fsafe [put/get] [mats/cannabis/meth/cocaine/promethazine/lean/codeine/gun]");
	{
		if(strcmp(sec, "put", true) == 0)
		{
			// fput mats
			if(strcmp(thing, "mats", true) == 0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(PlayerInfo[playerid][pMats] < strval(amount)) return SendClientMessage(playerid, COLOR_GREY, "You don't have that much materials.");
				if(strval(amount) < 1 || strval(amount) > 200000) return SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 50000.");
				if(FamilyInfo[fam][FamilyMats] + strval(amount) > 200000) return SendClientMessage(playerid, COLOR_GREY, "Family safe can't hold more than 200,000 materials.");
				PlayerInfo[playerid][pMats] -= strval(amount);
				FamilyInfo[fam][FamilyMats] += strval(amount);
				SaveFamilies();
				format(fsafestr, sizeof(fsafestr), "Deposited %d Materials", strval(amount));
				LogSafe(playerid, fsafestr);
				format(fsafestr, sizeof(fsafestr), "* %s deposits materials into a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, fsafestr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			// fput pot
			if(strcmp(thing, "cannabis", true) == 0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(PlayerInfo[playerid][pCannabis] < strval(amount)) return SendClientMessage(playerid, COLOR_GREY, "You don't have that much cannabis.");
				if(strval(amount) < 1 || strval(amount) > 1000) return SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 1000.");
				if(FamilyInfo[fam][FamilyCannabis] + strval(amount) > 1000) return SendClientMessage(playerid, COLOR_GREY, "Family safe can't hold more than 1000 cannabis.");
				PlayerInfo[playerid][pCannabis] -= strval(amount);
				FamilyInfo[fam][FamilyCannabis] += strval(amount);
				SaveFamilies();
				format(fsafestr, sizeof(fsafestr), "Deposited %d Cannabis", strval(amount));
				LogSafe(playerid, fsafestr);
				format(fsafestr, sizeof(fsafestr), "* %s deposits some cannabis into a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, fsafestr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			// fput meth
			if(strcmp(thing, "meth", true) == 0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(PlayerInfo[playerid][pMeth] < strval(amount)) return SendClientMessage(playerid, COLOR_GREY, "You don't have that much meth.");
				if(strval(amount) < 1 || strval(amount) > 1000) return SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 1000.");
				if(FamilyInfo[fam][FamilyMeth] + strval(amount) > 1000) return SendClientMessage(playerid, COLOR_GREY, "Family safe can't hold more than 1000 meth.");
				PlayerInfo[playerid][pMeth] -= strval(amount);
				FamilyInfo[fam][FamilyMeth] += strval(amount);
				SaveFamilies();
				format(fsafestr, sizeof(fsafestr), "Deposited %d Meth", strval(amount));
				LogSafe(playerid, fsafestr);
				format(fsafestr, sizeof(fsafestr), "* %s deposits some meth into a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, fsafestr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			// fput crack
			if(strcmp(thing, "cocaine", true) ==0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(PlayerInfo[playerid][pCocaine] < strval(amount)) return SendClientMessage(playerid, COLOR_GREY, "You don't have that much cocaine.");
				if(strval(amount) < 1 || strval(amount) > 1000) return SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 1000.");
				if(FamilyInfo[fam][FamilyCocaine] + strval(amount) > 1000) return SendClientMessage(playerid, COLOR_GREY, "Family safe can't hold more than 1000 cocaine.");
				PlayerInfo[playerid][pCocaine] -= strval(amount);
				FamilyInfo[fam][FamilyCocaine] += strval(amount);
				SaveFamilies();
				format(fsafestr, sizeof(fsafestr), "Deposited %d Cocaine", strval(amount));
				LogSafe(playerid, fsafestr);
				format(fsafestr, sizeof(fsafestr), "* %s deposits some cocaine into a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, fsafestr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			// fput promethazine
			if(strcmp(thing, "promethazine", true) ==0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(PlayerInfo[playerid][pPromethazine] < strval(amount)) return SendClientMessage(playerid, COLOR_GREY, "You don't have that much promethazine.");
				if(strval(amount) < 1 || strval(amount) > 1000) return SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 1000.");
				if(FamilyInfo[fam][FamilyPromethazine] + strval(amount) > 1000) return SendClientMessage(playerid, COLOR_GREY, "Family safe can't hold more than 1000 promethazine.");
				PlayerInfo[playerid][pPromethazine] -= strval(amount);
				FamilyInfo[fam][FamilyPromethazine] += strval(amount);
				SaveFamilies();
				format(fsafestr, sizeof(fsafestr), "Deposited %d Promethazine", strval(amount));
				LogSafe(playerid, fsafestr);
				format(fsafestr, sizeof(fsafestr), "* %s deposits some promethazine into a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, fsafestr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			// fput codeine
			if(strcmp(thing, "codeine", true) ==0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(PlayerInfo[playerid][pCodeine] < strval(amount)) return SendClientMessage(playerid, COLOR_GREY, "You don't have that much codeine.");
				if(strval(amount) < 1 || strval(amount) > 1000) return SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 1000.");
				if(FamilyInfo[fam][FamilyCodeine] + strval(amount) > 1000) return SendClientMessage(playerid, COLOR_GREY, "Family safe can't hold more than 1000 codeine.");
				PlayerInfo[playerid][pCodeine] -= strval(amount);
				FamilyInfo[fam][FamilyCodeine] += strval(amount);
				SaveFamilies();
				format(fsafestr, sizeof(fsafestr), "Deposited %d Codeine", strval(amount));
				LogSafe(playerid, fsafestr);
				format(fsafestr, sizeof(fsafestr), "* %s deposits some codeine into a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, fsafestr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			// fput lean
			if(strcmp(thing, "lean", true) ==0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(PlayerInfo[playerid][pLean] < strval(amount)) return SendClientMessage(playerid, COLOR_GREY, "You don't have that much lean.");
				if(strval(amount) < 1 || strval(amount) > 1000) return SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 1000.");
				if(FamilyInfo[fam][FamilyLean] + strval(amount) > 1000) return SendClientMessage(playerid, COLOR_GREY, "Family safe can't hold more than 1000 lean.");
				PlayerInfo[playerid][pLean] -= strval(amount);
				FamilyInfo[fam][FamilyLean] += strval(amount);
				SaveFamilies();
				format(fsafestr, sizeof(fsafestr), "Deposited %d Lean", strval(amount));
				LogSafe(playerid, fsafestr);
				format(fsafestr, sizeof(fsafestr), "* %s deposits some lean into a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, fsafestr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
			}
			// fput gun
			if(strcmp(thing, "gun", true) ==0)
			{
				new stored = 0, weapon = GetPlayerWeapon(playerid);
				if(strcmp(amount, "colt45", true) == 0)
				{
					if(FamilyInfo[fam][Colt45] >= SAFE_WEAPONS_LIMIT) return SendClientMessage(playerid, COLOR_GREY, "Your family Colt45 locker is full.");
					if(weapon != 22 || HaveWeapon(playerid, weapon) != weapon) return SendClientMessage(playerid, COLOR_GREY, "You are not holding a Colt45.");
					FamilyInfo[fam][Colt45]++;
					TakePlayerWeapon(playerid, 22);
					stored = 1;
				}
				if(strcmp(amount, "shotgun", true) == 0)
				{
					if(FamilyInfo[fam][Shotgun] >= SAFE_WEAPONS_LIMIT) return SendClientMessage(playerid, COLOR_GREY, "Your family Shotgun locker is full.");
					if(weapon != 25 || HaveWeapon(playerid, weapon) != weapon) return SendClientMessage(playerid, COLOR_GREY, "You are not holding a Shotgun.");
					FamilyInfo[fam][Shotgun]++;
					TakePlayerWeapon(playerid, 25);
					stored = 1;
				}
				if(strcmp(amount, "mp5", true) == 0)
				{
					if(FamilyInfo[fam][MP5] >= SAFE_WEAPONS_LIMIT) return SendClientMessage(playerid, COLOR_GREY, "Your family MP5 locker is full.");
					if(weapon != 29 || HaveWeapon(playerid, weapon) != weapon) return SendClientMessage(playerid, COLOR_GREY, "You are not holding a MP5.");
					FamilyInfo[fam][MP5]++;
					TakePlayerWeapon(playerid, 29);
					stored = 1;
				}
				if(strcmp(amount, "ak47", true) == 0)
				{
					if(FamilyInfo[fam][AK47] >= SAFE_WEAPONS_LIMIT) return SendClientMessage(playerid, COLOR_GREY, "Your family AK47 locker is full.");
					if(weapon != 30 || HaveWeapon(playerid, weapon) != weapon) return SendClientMessage(playerid, COLOR_GREY, "You are not holding an AK47.");
					FamilyInfo[fam][AK47]++;
					TakePlayerWeapon(playerid, 30);
					stored = 1;
				}
				if(strcmp(amount, "m4", true) == 0)
				{
					if(FamilyInfo[fam][M4] >= SAFE_WEAPONS_LIMIT) return SendClientMessage(playerid, COLOR_GREY, "Your family M4 locker is full.");
					if(weapon != 31 || HaveWeapon(playerid, weapon) != weapon) return SendClientMessage(playerid, COLOR_GREY, "You are not holding an M4.");
					FamilyInfo[fam][M4]++;
					TakePlayerWeapon(playerid, 31);
					stored = 1;
				}
				if(strcmp(amount, "spas12", true) == 0)
				{
					if(FamilyInfo[fam][SPAS12] >= SAFE_WEAPONS_LIMIT) return SendClientMessage(playerid, COLOR_GREY, "Your family SPAS12 locker is full.");
					if(weapon != 27 || HaveWeapon(playerid, weapon) != weapon) return SendClientMessage(playerid, COLOR_GREY, "You are not holding a SPAS12.");
					FamilyInfo[fam][SPAS12]++;
					TakePlayerWeapon(playerid, 27);
					stored = 1;
				}
				if(strcmp(amount, "rifle", true) == 0)
				{
					if(FamilyInfo[fam][Rifle] >= SAFE_WEAPONS_LIMIT) return SendClientMessage(playerid, COLOR_GREY, "Your family Rifle locker is full.");
					if(weapon != 33 || HaveWeapon(playerid, weapon) != weapon) return SendClientMessage(playerid, COLOR_GREY, "You are not holding a Rifle.");
					FamilyInfo[fam][Rifle]++;
					TakePlayerWeapon(playerid, 33);
					stored = 1;
				}
				if(strcmp(amount, "deagle", true) == 0)
				{
					if(FamilyInfo[fam][Deagle] >= SAFE_WEAPONS_LIMIT) return SendClientMessage(playerid, COLOR_GREY, "You family Deagle locker is full.");
					if(weapon != 24 || HaveWeapon(playerid, weapon) != weapon) return SendClientMessage(playerid, COLOR_GREY, "You are not holding a Deagle.");
					FamilyInfo[fam][Deagle]++;
					TakePlayerWeapon(playerid, 24);
					stored = 1;
				}
				if(strcmp(amount, "sniper", true) == 0)
				{
					if(FamilyInfo[fam][Sniper] >= SAFE_WEAPONS_LIMIT) return SendClientMessage(playerid, COLOR_GREY, "Your family Sniper locker is full.");
					if(weapon != 34 || HaveWeapon(playerid, weapon) != weapon) return SendClientMessage(playerid, COLOR_GREY, "You are not holding a Sniper.");
					FamilyInfo[fam][Sniper]++;
					TakePlayerWeapon(playerid, 34);
					stored = 1;
				}
				if(strcmp(amount, "uzi", true) == 0)
				{
					if(FamilyInfo[fam][UZI] >= SAFE_WEAPONS_LIMIT) return SendClientMessage(playerid, COLOR_GREY, "Your family UZI locker is full.");
					if(weapon != 28 || HaveWeapon(playerid, weapon) != weapon) return SendClientMessage(playerid, COLOR_GREY, "You are not holding an UZI.");
					FamilyInfo[fam][UZI]++;
					TakePlayerWeapon(playerid, 28);
					stored = 1;
				}
				if(strcmp(amount, "tec9", true) == 0)
				{
					if(FamilyInfo[fam][TEC9] >= SAFE_WEAPONS_LIMIT) return SendClientMessage(playerid, COLOR_GREY, "Your family TEC9 locker is full.");
					if(weapon != 32 || HaveWeapon(playerid, weapon) != weapon) return SendClientMessage(playerid, COLOR_GREY, "You are not holding a TEC9.");
					FamilyInfo[fam][TEC9]++;
					TakePlayerWeapon(playerid, 32);
					stored = 1;
				}
				if(stored == 1)
				{
					format(fsafestr, sizeof(fsafestr), "* %s places a %s into a safe.", PlayerICName(playerid), amount);
					ProxDetector(30.0, playerid, fsafestr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					SaveFamilies();
					format(fsafestr, sizeof(fsafestr), "Deposited %s", amount);
					LogSafe(playerid, fsafestr);
				}
			}
		}
		if(strcmp(sec, "get", true) == 0)
		{
			if(strcmp(thing, "mats", true) == 0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(PlayerInfo[playerid][pRank] < 5) return SendClientMessage(playerid, COLOR_GREY, "Only rank 5+ is allowed to take family materials.");
				if(strval(amount) > FamilyInfo[fam][FamilyMats]) return SendClientMessage(playerid, COLOR_GREY, "Family safe doesn't have that much materials.");
				if(strval(amount) < 1 || strval(amount) > 50000) return SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 50000.");
				PlayerInfo[playerid][pMats] += strval(amount);
				FamilyInfo[fam][FamilyMats] -= strval(amount);
				SaveFamilies();
				format(fsafestr, sizeof(fsafestr), "Withdrew %d Materials", strval(amount));
				LogSafe(playerid, fsafestr);
				format(fsafestr, sizeof(fsafestr), "* %s takes some materials from a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, fsafestr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				return 1;
			}
			// fget pot
			if(strcmp(thing, "cannabis", true) == 0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(PlayerInfo[playerid][pRank] < 4) return SendClientMessage(playerid, COLOR_GREY, "Only rank 5+ is allowed to take family cannabis.");
				if(strval(amount) > FamilyInfo[fam][FamilyCannabis]) return SendClientMessage(playerid, COLOR_GREY, "Family safe doesn't have that much cannabis.");
				if(strval(amount) < 1 || strval(amount) > 50) return SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 50.");
				if(strval(amount) + PlayerInfo[playerid][pCannabis] > 50) return SendClientMessage(playerid, COLOR_GREY, "You can only hold a max of 50 cannabis.");
				PlayerInfo[playerid][pCannabis] += strval(amount);
				FamilyInfo[fam][FamilyCannabis] -= strval(amount);
				SaveFamilies();
				format(fsafestr, sizeof(fsafestr), "Withdrew %d Cannabis", strval(amount));
				LogSafe(playerid, fsafestr);
				format(fsafestr, sizeof(fsafestr), "* %s takes some cannabis from a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, fsafestr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				return 1;
			}
			// fget crack
			if(strcmp(thing, "cocaine", true) ==0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(PlayerInfo[playerid][pRank] < 4) return SendClientMessage(playerid, COLOR_GREY, "Only rank 5+ is allowed to take family cocaine.");
				if(strval(amount) > FamilyInfo[fam][FamilyCocaine]) return SendClientMessage(playerid, COLOR_GREY, "Family safe doesn't have that much cocaine.");
				if(strval(amount) < 1 || strval(amount) > 25)SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 25.");
				if(strval(amount) + PlayerInfo[playerid][pCocaine] > 25) return SendClientMessage(playerid, COLOR_GREY, "You can only hold a max of 25 cocaine.");
				PlayerInfo[playerid][pCocaine] += strval(amount);
				FamilyInfo[fam][FamilyCocaine] -= strval(amount);
				SaveFamilies();
				format(fsafestr, sizeof(fsafestr), "Withdrew %d Cocaine", strval(amount));
				LogSafe(playerid, fsafestr);
				format(fsafestr, sizeof(fsafestr), "* %s takes some cocaine from a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, fsafestr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				return 1;
			}
			if(strcmp(thing, "meth", true) ==0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(PlayerInfo[playerid][pRank] < 4) return SendClientMessage(playerid, COLOR_GREY, "Only rank 5+ is allowed to take family meth.");
				if(strval(amount) > FamilyInfo[fam][FamilyMeth]) return SendClientMessage(playerid, COLOR_GREY, "Family safe doesn't have that much meth.");
				if(strval(amount) < 1 || strval(amount) > 25)SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 25.");
				if(strval(amount) + PlayerInfo[playerid][pMeth] > 25) return SendClientMessage(playerid, COLOR_GREY, "You can only hold a max of 25 meth.");
				PlayerInfo[playerid][pMeth] += strval(amount);
				FamilyInfo[fam][FamilyMeth] -= strval(amount);
				SaveFamilies();
				format(fsafestr, sizeof(fsafestr), "Withdrew %d Meth", strval(amount));
				LogSafe(playerid, fsafestr);
				format(fsafestr, sizeof(fsafestr), "* %s takes some meth from a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, fsafestr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				return 1;
			}
			if(strcmp(thing, "promethazine", true) ==0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(PlayerInfo[playerid][pRank] < 4) return SendClientMessage(playerid, COLOR_GREY, "Only rank 5+ is allowed to take family promethazine.");
				if(strval(amount) > FamilyInfo[fam][FamilyPromethazine]) return SendClientMessage(playerid, COLOR_GREY, "Family safe doesn't have that much promethazine.");
				if(strval(amount) < 1 || strval(amount) > 25)SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 25.");
				if(strval(amount) + PlayerInfo[playerid][pPromethazine] > 25) return SendClientMessage(playerid, COLOR_GREY, "You can only hold a max of 25 promethazine.");
				PlayerInfo[playerid][pPromethazine] += strval(amount);
				FamilyInfo[fam][FamilyPromethazine] -= strval(amount);
				SaveFamilies();
				format(fsafestr, sizeof(fsafestr), "Withdrew %d Promethazine", strval(amount));
				LogSafe(playerid, fsafestr);
				format(fsafestr, sizeof(fsafestr), "* %s takes some promethazine from a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, fsafestr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				return 1;
			}
			if(strcmp(thing, "codeine", true) ==0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(PlayerInfo[playerid][pRank] < 4) return SendClientMessage(playerid, COLOR_GREY, "Only rank 5+ is allowed to take family codeine.");
				if(strval(amount) > FamilyInfo[fam][FamilyCodeine]) return SendClientMessage(playerid, COLOR_GREY, "Family safe doesn't have that much codeine.");
				if(strval(amount) < 1 || strval(amount) > 25)SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 25.");
				if(strval(amount) + PlayerInfo[playerid][pCodeine] > 25) return SendClientMessage(playerid, COLOR_GREY, "You can only hold a max of 25 codeine.");
				PlayerInfo[playerid][pCodeine] += strval(amount);
				FamilyInfo[fam][FamilyCodeine] -= strval(amount);
				SaveFamilies();
				format(fsafestr, sizeof(fsafestr), "Withdrew %d Codeine", strval(amount));
				LogSafe(playerid, fsafestr);
				format(fsafestr, sizeof(fsafestr), "* %s takes some codeine from a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, fsafestr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				return 1;
			}
			if(strcmp(thing, "lean", true) ==0)
			{
				if(!IsNumeric(amount)) return SendClientMessage(playerid, COLOR_GREY, "The amount must be a number.");
				if(PlayerInfo[playerid][pRank] < 4) return SendClientMessage(playerid, COLOR_GREY, "Only rank 5+ is allowed to take family lean.");
				if(strval(amount) > FamilyInfo[fam][FamilyLean]) return SendClientMessage(playerid, COLOR_GREY, "Family safe doesn't have that much lean.");
				if(strval(amount) < 1 || strval(amount) > 25)SendClientMessage(playerid, COLOR_GREY, "Amount must be between 1 and 25.");
				if(strval(amount) + PlayerInfo[playerid][pLean] > 25) return SendClientMessage(playerid, COLOR_GREY, "You can only hold a max of 25 lean.");
				PlayerInfo[playerid][pLean] += strval(amount);
				FamilyInfo[fam][FamilyLean] -= strval(amount);
				SaveFamilies();
				format(fsafestr, sizeof(fsafestr), "Withdrew %d Lean", strval(amount));
				LogSafe(playerid, fsafestr);
				format(fsafestr, sizeof(fsafestr), "* %s takes some lean from a safe.", PlayerICName(playerid));
				ProxDetector(30.0, playerid, fsafestr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				return 1;
			}
			// fget gun
			if(strcmp(thing, "gun", true) ==0)
			{
				if(PlayerInfo[playerid][pRank] < 2)
					return SendClientMessage(playerid, COLOR_GREY, "Only rank 2+ is allowed to take family guns.");
				new got = 0;
				if(strcmp(amount, "colt45", true) == 0)
				{
					if(FamilyInfo[fam][Colt45] <= 0) return SendClientMessage(playerid, COLOR_GREY, "Your family Colt45 locker is empty.");
					if(HaveWeapon(playerid, 22) == 22) return SendClientMessage(playerid, COLOR_GREY, "You are already holding a Colt45.");
					FamilyInfo[fam][Colt45]--;
					GivePlayerGun(playerid, 22);
					got = 1;
				}
				if(strcmp(amount, "shotgun", true) == 0)
				{
					if(FamilyInfo[fam][Shotgun] <= 0) return SendClientMessage(playerid, COLOR_GREY, "Your family Shotgun locker is empty.");
					if(HaveWeapon(playerid, 25) == 25) return SendClientMessage(playerid, COLOR_GREY, "You are already holding a Shotgun.");
					FamilyInfo[fam][Shotgun]--;
					GivePlayerGun(playerid, 25);
					got = 1;
				}
				if(strcmp(amount, "mp5", true) == 0)
				{
					if(FamilyInfo[fam][MP5] <= 0) return SendClientMessage(playerid, COLOR_GREY, "Your family MP5 locker is empty.");
					if(HaveWeapon(playerid, 29) == 29) return SendClientMessage(playerid, COLOR_GREY, "You are already holding a MP5.");
					FamilyInfo[fam][MP5]--;
					GivePlayerGun(playerid, 29);
					got = 1;
				}
				if(strcmp(amount, "ak47", true) == 0)
				{
					if(FamilyInfo[fam][AK47] <= 0) return SendClientMessage(playerid, COLOR_GREY, "Your family AK47 locker is empty.");
					if(HaveWeapon(playerid, 30) == 30) return SendClientMessage(playerid, COLOR_GREY, "You are already holding an AK47.");
					FamilyInfo[fam][AK47]--;
					GivePlayerGun(playerid, 30);
					got = 1;
				}
				if(strcmp(amount, "m4", true) == 0)
				{
					if(FamilyInfo[fam][M4] <= 0) return SendClientMessage(playerid, COLOR_GREY, "Your family M4 locker is empty.");
					if(HaveWeapon(playerid, 31) == 31) return SendClientMessage(playerid, COLOR_GREY, "You are already holding a M4.");
					FamilyInfo[fam][M4]--;
					GivePlayerGun(playerid, 31);
					got = 1;
				}
				if(strcmp(amount, "spas12", true) == 0)
				{
					if(FamilyInfo[fam][SPAS12] <= 0) return SendClientMessage(playerid, COLOR_GREY, "Your family SPAS12 locker is empty.");
					if(HaveWeapon(playerid, 27) == 27) return SendClientMessage(playerid, COLOR_GREY, "You are already holding a SPAS12.");
					FamilyInfo[fam][SPAS12]--;
					GivePlayerGun(playerid, 27);
					got = 1;
				}
				if(strcmp(amount, "rifle", true) == 0)
				{
					if(FamilyInfo[fam][Rifle] <= 0) return SendClientMessage(playerid, COLOR_GREY, "Your family Rifle locker is empty.");
					if(HaveWeapon(playerid, 33) == 33) return SendClientMessage(playerid, COLOR_GREY, "You are already holding a Rifle.");
					FamilyInfo[fam][Rifle]--;
					GivePlayerGun(playerid, 33);
					got = 1;
				}
				if(strcmp(amount, "sniper", true) == 0)
				{
					if(FamilyInfo[fam][Sniper] <= 0) return SendClientMessage(playerid, COLOR_GREY, "Your family Sniper locker is empty.");
					if(HaveWeapon(playerid, 34) == 34) return SendClientMessage(playerid, COLOR_GREY, "You are already holding a Sniper.");
					FamilyInfo[fam][Sniper]--;
					GivePlayerGun(playerid, 34);
					got = 1;
				}
				if(strcmp(amount, "deagle", true) == 0)
				{
					if(FamilyInfo[fam][Deagle] <= 0) return SendClientMessage(playerid, COLOR_GREY, "Your family Deagle locker is empty.");
					if(HaveWeapon(playerid, 24) == 24) return SendClientMessage(playerid, COLOR_GREY, "You are already holding a Deagle.");
					FamilyInfo[fam][Deagle]--;
					GivePlayerGun(playerid, 24);
					got = 1;
				}
				if(strcmp(amount, "uzi", true) == 0)
				{
					if(FamilyInfo[fam][UZI] <= 0) return SendClientMessage(playerid, COLOR_GREY, "Your family UZI locker is empty.");
					if(HaveWeapon(playerid, 28) == 28) return SendClientMessage(playerid, COLOR_GREY, "You are already holding an UZI.");
					FamilyInfo[fam][UZI]--;
					GivePlayerGun(playerid, 28);
					got = 1;
				}
				if(strcmp(amount, "tec9", true) == 0)
				{
					if(FamilyInfo[fam][TEC9] <= 0) return SendClientMessage(playerid, COLOR_GREY, "Your family TEC9 locker is empty.");
					if(HaveWeapon(playerid, 32) == 32) return SendClientMessage(playerid, COLOR_GREY, "You are already holding a TEC9.");
					FamilyInfo[fam][TEC9]--;
					GivePlayerGun(playerid, 32);
					got = 1;
				}
				if(got == 1)
				{
					format(fsafestr, sizeof(fsafestr), "* %s takes a %s from a safe.", PlayerICName(playerid), amount);
					ProxDetector(30.0, playerid, fsafestr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
					SaveFamilies();
					format(fsafestr, sizeof(fsafestr), "Withdrew %s", amount);
					LogSafe(playerid, fsafestr);
				}
				return 1;
			}
		}
	}
	return 1;
}

CMD:myfamlogs(playerid, params[])
{
	if(PlayerInfo[playerid][pFMember] == 255) return SendClientMessage(playerid, COLOR_GREY, "You are not in a family.");
	if(PlayerInfo[playerid][pRank] < 5) return SendClientMessage(playerid, COLOR_GREY, "Only R5+ can use this.");

	new query[128];
	mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `familysafelogs` WHERE `FamilyID`= %d ORDER BY `ID` DESC LIMIT 40", PlayerInfo[playerid][pFMember]);
	mysql_pquery(sqlGameConnection, query, "FamilySLogs", "d", playerid);
	return 1;
}

forward FamilySLogs(playerid);
public FamilySLogs(playerid)
{
	new rows = cache_num_rows();
	if(!rows) return SendClientMessage(playerid, COLOR_GREY, "No safe actions to display.");

	new familysstr[4096], headers[] = "Player\tAction\tDate\n";

	for(new i = 0; i < rows; i++)
	{
		new name[24], date[32], action[128];
		cache_get_field_content(i, "Player", name);
		cache_get_field_content(i, "Date", date);
		cache_get_field_content(i, "Action", action);

		format(familysstr, sizeof(familysstr), "%s%s\t%s\t%s\n", familysstr, name, action, date);
	}

	format(familysstr, sizeof(familysstr), "%s%s", headers, familysstr);
	ShowPlayerDialog(playerid, DIALOG_FSAFES_SHOW, DIALOG_STYLE_TABLIST_HEADERS, "My Family Safe Logs", familysstr, "Done", "");
	return 1;
}