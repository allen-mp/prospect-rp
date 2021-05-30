/*
	
  ______              _ _         _                    _       ____          _____                                      _ 
 |  ____|            (_) |       | |                  | |     |  _ \        |  __ \                                    | |
 | |__ __ _ _ __ ___  _| |_   _  | |     _____   _____| |___  | |_) |_   _  | |  | | ___  ___ _ __ ___   ___  _ __   __| |
 |  __/ _` | '_ ` _ \| | | | | | | |    / _ \ \ / / _ \ / __| |  _ <| | | | | |  | |/ _ \/ __| '_ ` _ \ / _ \| '_ \ / _` |
 | | | (_| | | | | | | | | |_| | | |___|  __/\ V /  __/ \__ \ | |_) | |_| | | |__| |  __/\__ \ | | | | | (_) | | | | (_| |
 |_|  \__,_|_| |_| |_|_|_|\__, | |______\___| \_/ \___|_|___/ |____/ \__, | |_____/ \___||___/_| |_| |_|\___/|_| |_|\__,_|
                           __/ |                                      __/ |                                               
                          |___/                                      |___/                                               

	http://www.gta-sarp.com/forums/showthread.php?298108 
	Read post:
	http://www.gta-sarp.com/forums/showthread.php?298108-Family-system&p=2169479&viewfull=1#post2169479

	Families get XP by activity, roleplay and shootouts activities.

	1. Roleplay - /famrp - RP given by gang mods
	2. Activity - if you have 5+ members in your family online, on /signcheck you get XP
	3. Shootouts - get XP for making a successful bank robbery

	8-10 levels

*/


GetFamilyLevel(familyid) // simply check what level the family is, as it's based on XP
{
	new xp = FamilyInfo[familyid][fXP];
	switch(xp)
	{
		case 0..49: return 1;
		case 50..149: return 2;
		case 150..299: return 3;
		case 300..499: return 4;
		case 500..749: return 5;
		case 750..1049: return 6;
		case 1050..1399: return 7;
		case 1400..1799: return 8;
	}
	if(xp >= 1799) return 8;
	
	return 1;
}


GetFamilyNextXP(familyid) // couldn't think of a better name. This function returns the amount of XP the family needs to level up
{
	new xp = FamilyInfo[familyid][fXP];
	switch(xp)
	{
		case 0..49: return 50;
		case 50..149: return 150;
		case 150..299: return 300;
		case 300..499: return 500;
		case 500..749: return 750;
		case 750..1049: return 1050;
		case 1050..1399: return 1400;
		case 1400..1799: return 1800;
	}
	if(xp >= 1799) return -1;
	return 1;
}

GrantFamXP(family, amount)
{
	new xp = FamilyInfo[family][fXP];
	new nextxp = GetFamilyNextXP(family);

	if(xp+amount > nextxp) // if family leveled up
	{
		if(GetFamilyLevel(family) == 8) return 1; // max level
		FamilyInfo[family][fXP] += amount;
		new msgstr[50];
		format(msgstr, sizeof(msgstr), "Your family has leveled up! New level: %d", GetFamilyLevel(family));
		foreach(new i: Player)
		{
			if(PlayerInfo[i][pFMember] == family)
			{
				SendClientMessage(i, COLOR_LIGHTBLUE, msgstr);
			}
		}
	}
	else FamilyInfo[family][fXP] += amount;	

	SaveFamilies();
	return 1;
}

