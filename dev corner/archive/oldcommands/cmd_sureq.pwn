// Command to RPly equip surveillance equipment for FBI. Saves some time.
CMD:sureq(playerid, params[]) {
	if(!IsAFBI(playerid))
		return 1;

	if(!IsPlayerNearFBIHQ(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You are not at your locker room.");

	new sureqType;
	if(sscanf(params, "i", sureqType)) {
		SendClientMessage(playerid, COLOR_GRAD2, "{00BFFF}Usage:{FFFFFF} /sureq [option]");
		SendClientMessage(playerid, COLOR_WHITE, "1: Silicone mask, microdot camera");
		SendClientMessage(playerid, COLOR_WHITE, "3: Microdot camera");
		return 1;
	}

	if(sureqType < 1 || sureqType > 3)
		return SendClientMessage(playerid, COLOR_GREY, "Invalid option.");

	switch(sureqType) {
		case 1: {
			cmd_sureq(playerid, "2");
			cmd_sureq(playerid, "3");
		}
		case 2: cmd_me(playerid, "retrieves a microdot camera, turns it on and attaches it to their attire.");
	}

	cmd_time(playerid, "");
	return 1;
}