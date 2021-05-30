// Every file which uses y_hooks must have a seperate include for it //
#include <YSI\y_hooks>

// Defining the path if it's not defined already //
#define STEELBIZCARD_PATH "aaronsteel_bizcard.ini"


// Variables that is needed //
enum _steelBizCard{
	steel_Name[MAX_PLAYER_NAME + 1], 			// Stores his custom name in the card //
	steel_Biz[30],								// Stores the name of his business in the card //
	steel_Pnumber								// Stores his phone number //
};

static SteelBizCard[_steelBizCard];

// Function to load the business card's data //
Steel_LoadBizCardData(){

	if(!dini_Exists(STEELBIZCARD_PATH)){

		dini_Create(STEELBIZCARD_PATH);

		format(SteelBizCard[steel_Name], MAX_PLAYER_NAME+1, "Aaron Steel");
		format(SteelBizCard[steel_Biz], 30, "N/A");
		SteelBizCard[steel_Pnumber] = 0;

		dini_Set(STEELBIZCARD_PATH, "ST_Name", SteelBizCard[steel_Name]);
		dini_Set(STEELBIZCARD_PATH, "ST_Biz", SteelBizCard[steel_Biz]);
		dini_IntSet(STEELBIZCARD_PATH, "ST_Number", SteelBizCard[steel_Pnumber]);
	}
	else{
		if(!dini_Isset(STEELBIZCARD_PATH, "ST_Name")){
			dini_Set(STEELBIZCARD_PATH, "ST_Name", "Aaron Steel");
			format(SteelBizCard[steel_Name], MAX_PLAYER_NAME+1, "Aaron Steel");
		}
		else
			format(SteelBizCard[steel_Name], MAX_PLAYER_NAME+1, dini_Get(STEELBIZCARD_PATH, "ST_Name"));	

		if(!dini_Isset(STEELBIZCARD_PATH, "ST_Biz")){
			dini_Set(STEELBIZCARD_PATH, "ST_Biz", "N/A");
			format(SteelBizCard[steel_Biz], 30, "N/A");
		}
		else
			format(SteelBizCard[steel_Biz], 30, dini_Get(STEELBIZCARD_PATH, "ST_Biz"));

		if(!dini_Isset(STEELBIZCARD_PATH, "ST_Number")){
			dini_IntSet(STEELBIZCARD_PATH, "ST_Number", 0);
			SteelBizCard[steel_Biz] = 0;
		}
		else
			SteelBizCard[steel_Biz] = dini_Int(STEELBIZCARD_PATH, "ST_Number");
	}
	return 1;
}

// Hooking OnGameModeInit //
hook OnGameModeInit(){

	Steel_LoadBizCardData();

	return 1;
}

CMD:steel(playerid, params[]){
	if(PlayerInfo[playerid][pID] != AARON_STEEL_ID)
		return 1;

	new choice[5], _params[64];

	if(sscanf(params, "s[5]S()[64]", choice, _params))
		return SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} /steel [edit/show]");

	if(!strcmp(choice, "edit", true)){
		return STEEL_editcard(playerid, _params);
	}
	else if(!strcmp(choice, "show", true)){

		new targetid;
		if(sscanf(_params, "u", targetid))
			return SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} /steel show [playerid/partOfName]");

		if(targetid == INVALID_PLAYER_ID)
			return SendClientMessage(playerid, -1, "{FF0000}Error:{FFFFFF} Could not find player");

		new 
			Float:targetPos[3];

		GetPlayerPos(targetid, targetPos[0], targetPos[1], targetPos[2]);

		if(!IsPlayerInRangeOfPoint(playerid, 3.0, targetPos[0], targetPos[1], targetPos[2]))
			return SendClientMessage(playerid, -1, "{FF0000}Error:{FFFFFF} You are not near the player");

		new msg[128];
		format(msg, sizeof(msg), "* %s reaches over towards %s and shows them their business card. *", PlayerICName(playerid), PlayerICName(targetid));
		ProxDetector(30.0, playerid, msg, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

		SendClientMessage(targetid, -1, "{33AA33}Business Card:");

		format(msg, sizeof(msg), "{33AA33}Name{FFFFFF}: %s", SteelBizCard[steel_Name]);
		SendClientMessage(targetid, -1, msg);

		format(msg, sizeof(msg), "{33AA33}Corp.{FFFFFF}: %s", SteelBizCard[steel_Biz]);
		SendClientMessage(targetid, -1, msg);

		format(msg, sizeof(msg), "{33AA33}Number{FFFFFF}: %d", SteelBizCard[steel_Pnumber]);
		SendClientMessage(targetid, -1, msg);
	}
	else
		return cmd_steel(playerid, "");

	return 1;
}

STEEL_editcard(playerid, params[]){

	new 
		option[10],
		value[35];

	if(sscanf(params, "s[10]S()[35]", option, value))
		return SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} /steel edit [name/corp/number]");

	if(!strcmp(option, "name", true)){
		if(!strlen(value))
			return SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} /steel edit name [name(1-24 chars)]");
		else if(!(0 < strlen(value) < (MAX_PLAYER_NAME+1)))
			return SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} Name must be more than 0 and less than "#MAX_PLAYER_NAME"");
		STEEL_editcardName(value);
		SendClientMessage(playerid, COLOR_GREY, "You changed the name in your business card");
	}
	else if(!strcmp(option, "corp", true)){
		if(!strlen(value))
			return SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} /steel edit corp [name(1-30 chars)]");
		else if(!(0 < strlen(value) < 30))
			return SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} Corp name must be more than 0 and less than 30");
		STEEL_editcardBiz(value);
		SendClientMessage(playerid, COLOR_GREY, "You changed the name of your corp in your business card");
	}
	else if(!strcmp(option, "number", true)){
		if(!strlen(value))
			return SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} /steel edit number [number]");
		new number;
		if(sscanf(value, "d", number))
			return SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} Phone number must be a number");
		STEEL_editcardNumber(number);
		SendClientMessage(playerid, COLOR_GREY, "You changed the number in your business card");
	}
	else
		return STEEL_editcard(playerid, "");

	return 1;
}

STEEL_editcardName(params[]){

	format(SteelBizCard[steel_Name], MAX_PLAYER_NAME+1, "%s", params);
	dini_Set(STEELBIZCARD_PATH, "ST_Name", params);

	return 1;
}

STEEL_editcardBiz(params[]){

	format(SteelBizCard[steel_Biz], 30, "%s", params);
	dini_Set(STEELBIZCARD_PATH, "ST_Biz", params);

	return 1;
}

STEEL_editcardNumber(params){

	SteelBizCard[steel_Pnumber] = params;
	dini_IntSet(STEELBIZCARD_PATH, "ST_Number", params);

	return 1;
}