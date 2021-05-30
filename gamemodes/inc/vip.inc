#define VIP_WELCOME 6652

//CHAT COLOR DEFINES
#define COL_WHITE "{FFFFFF}" //default
#define COL_YELL "{FFFF00}"

#define COL_GVIP "{FFC600}" //gold vip
#define COL_SVIP "{ACAEB0}" //silver vip
#define COL_BVIP "{BB7900}" //bronze vip
#define COL_LOC "{8D8DFF}" //location
#define COL_JOB "{FFFF91}" //job


stock displayVIPWelcomeMessage(playerid, level, expires[]) {
	switch(level) {
		case 1: { //BRONZE
			new vipType[] = "Bronze";
			new vipstr[2048];
			
			format(vipstr, sizeof(vipstr), ""#COL_WHITE"Thank you for supporting "#COL_YELL"Elite City Role Play"#COL_WHITE"!\nYour "#COL_BVIP"%s"#COL_WHITE" VIP will allow you to take advantage of the following features:\n\n", vipType);
			new perk1[] = "   * 15% increase in Materials when using the "#COL_JOB"Arms Dealer Job"#COL_WHITE"\n";
			new perk2[] = "   * 15% increase in Cash when using the "#COL_JOB"Drug Smuggler Job"#COL_WHITE"\n";
			new perk3[] = "   * 20% off when purchasing items from "#COL_LOC"24/7"#COL_WHITE" stores\n";
			new perk4[] = "   * 20% time reduction when getting sent to "#COL_LOC"Hospital"#COL_WHITE"\n";
			new perk5[] = "   * 15% increase in Cash when selling vehicles at the dropcar crane\n";
			new perk6[] = "   * Full access to the "#COL_LOC"VIP lounge"#COL_WHITE" and Bronze VIP restricted vehicles\n";
			new perk7[] = "   * Access to VIP commands and the "#COL_LOC"Black Market"#COL_WHITE"\n";
			new perk8[] = "\n   * Unlocked "#COL_LOC"24/7"#COL_WHITE" items:\n";
			new perk9[] = "     - Medium Jerrycan\n";
			new perk10[] = "     - Medium Boombox\n";
			format(vipstr, sizeof(vipstr), "%s%s%s%s%s%s%s%s%s%s%s", vipstr, perk1, perk2, perk3, perk4, perk5, perk6, perk7, perk8, perk9, perk10);
			format(vipstr, sizeof(vipstr), "%s\nYour VIP will expire in one month on the %s.", vipstr, expires);
			format(vipstr, sizeof(vipstr), "%s\nIf you are interested in contributing further, check out the online store at {800080}donate.gta-ecrp.com"#COL_WHITE".", vipstr);
			
			ShowPlayerDialog(playerid, VIP_WELCOME, DIALOG_STYLE_MSGBOX, "Bronze VIP", vipstr, "Ok", "Cancel");
		}
		case 2: { // SILVER
			new vipType[] = "Silver";
			new vipstr[2048];
			
			format(vipstr, sizeof(vipstr), ""#COL_WHITE"Thank you for supporting "#COL_YELL"Elite City Role Play"#COL_WHITE"!\nYour "#COL_SVIP"%s"#COL_WHITE" VIP will allow you to take advantage of the following features:\n\n", vipType);
			new perk1[] = "   * 20% increase in Materials when using the "#COL_JOB"Arms Dealer Job"#COL_WHITE"\n";
			new perk2[] = "   * 20% increase in Cash when using the "#COL_JOB"Drug Smuggler Job"#COL_WHITE"\n";
			new perk3[] = "   * 25% off when purchasing items from "#COL_LOC"24/7"#COL_WHITE" stores\n";
			new perk4[] = "   * 20% time reduction when getting sent to "#COL_LOC"Hospital"#COL_WHITE"\n";
			new perk5[] = "   * 20% increase in Cash when selling vehicles at the dropcar crane\n";
			new perk6[] = "   * Full access to the "#COL_LOC"VIP lounge"#COL_WHITE" and Silver/Bronze VIP restricted vehicles\n";
			new perk7[] = "   * Access to VIP commands and the "#COL_LOC"Black Market"#COL_WHITE"\n";
			new perk8[] = "\n   * Unlocked "#COL_LOC"24/7"#COL_WHITE" items:\n";
			new perk9[] = "     - Medium Jerrycan\n";
			new perk10[] = "     - Medium Boombox\n";
			format(vipstr, sizeof(vipstr), "%s%s%s%s%s%s%s%s%s%s%s", vipstr, perk1, perk2, perk3, perk4, perk5, perk6, perk7, perk8, perk9, perk10);
			format(vipstr, sizeof(vipstr), "%s\nYour VIP will expire in one month on the %s.", vipstr, expires);
			format(vipstr, sizeof(vipstr), "%s\nIf you are interested in contributing further, check out the online store at {800080}donate.gta-ecrp.com"#COL_WHITE".", vipstr);
			
			ShowPlayerDialog(playerid, VIP_WELCOME, DIALOG_STYLE_MSGBOX, "Silver VIP", vipstr, "Ok", "Cancel");
		
		}
		case 3: { //GOLD
			new vipType[] = "Gold";
			new vipstr[2048];
			
			format(vipstr, sizeof(vipstr), ""#COL_WHITE"Thank you for supporting "#COL_YELL"Elite City Role Play"#COL_WHITE"!\nYour "#COL_GVIP"%s"#COL_WHITE" VIP will allow you to take advantage of the following features:\n\n", vipType);
			new perk1[] = "   * 30% increase in Materials when using the "#COL_JOB"Arms Dealer Job"#COL_WHITE"\n";
			new perk2[] = "   * 30% increase in Cash when using the "#COL_JOB"Drug Smuggler Job"#COL_WHITE"\n";
			new perk3[] = "   * 30% off when purchasing items from "#COL_LOC"24/7"#COL_WHITE" stores\n";
			new perk4[] = "   * 30% time reduction when getting sent to "#COL_LOC"Hospital"#COL_WHITE"\n";
			new perk5[] = "   * 30% increase in Cash when selling vehicles at the dropcar crane\n";
			new perk6[] = "   * Full access to the "#COL_LOC"VIP lounge"#COL_WHITE" and all VIP restricted vehicles\n";
			new perk7[] = "   * Access to VIP commands and the "#COL_LOC"Black Market"#COL_WHITE"\n";
			new perk8[] = "\n   * Unlocked "#COL_LOC"24/7"#COL_WHITE" items:\n";
			new perk9[] = "     - Large Jerrycan\n";
			new perk10[] = "     - Large Boombox\n";
			
			format(vipstr, sizeof(vipstr), "%s%s%s%s%s%s%s%s%s%s%s", vipstr, perk1, perk2, perk3, perk4, perk5, perk6, perk7, perk8, perk9, perk10);
			format(vipstr, sizeof(vipstr), "%s\nYour VIP will expire in one month on the %s. If you choose to renew it before then you\nwill recieve a free {800080}Bronze VIP package"#COL_WHITE" that you can give to a friend!", vipstr, expires);
			format(vipstr, sizeof(vipstr), "%s\nIf you are interested in contributing further, check out the online store at {800080}donate.gta-ecrp.com"#COL_WHITE".", vipstr);
			
			ShowPlayerDialog(playerid, VIP_WELCOME, DIALOG_STYLE_MSGBOX, "Gold VIP", vipstr, "Ok", "Cancel");
		
		}
	}
}