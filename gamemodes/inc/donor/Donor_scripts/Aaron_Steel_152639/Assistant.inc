// Every file which uses y_hooks must have a seperate include for it //
#include <YSI\y_hooks>

// Path where the assistant's data will be saved //
#define STEELMESSAGES_PATH "aaronsteel_messages.ini"
#define STEELPAYLOAD_PATH "aaronsteel_payload.ini"

// The assistant's position data //
#define STEELASSISTANT_POSX 834.25
#define STEELASSISTANT_POSY 67.54
#define STEELASSISTANT_POSZ 1901.26
#define STEELASSISTANT_ROT 248.3176

// The assistant's functionality limit //
#define STEELASSISTANT_MAXMESSAGES 10
#define STEELASSISTANT_MESSAGE_SIZE 50
#define STEELASSISTANT_MAXPAYLOAD 5

enum steelsActorAttributes
{
	SteelsActor,
	Text3D:SteelsActor_3DText,
	bool:SteelsActor_IsBusy,
	SteelsAssistantPayLoad[STEELASSISTANT_MAXPAYLOAD],
}
static g_SteelsActorAttributes[steelsActorAttributes];

static g_SteelsAssistantMessages[STEELASSISTANT_MAXMESSAGES][STEELASSISTANT_MESSAGE_SIZE + 1];

static _foundEmptySlot = -1;

hook OnGameModeInit()
{
	g_SteelsActorAttributes[SteelsActor] = CreateActor(141, STEELASSISTANT_POSX, STEELASSISTANT_POSY, STEELASSISTANT_POSZ, STEELASSISTANT_ROT);

	g_SteelsActorAttributes[SteelsActor_3DText] = CreateDynamic3DTextLabel("{43527D}Steel's assistant\n\n\n\n\n{FFFFFF}Press Y to interact with her", -1, STEELASSISTANT_POSX, STEELASSISTANT_POSY, STEELASSISTANT_POSZ + 1.0, 5.0, .testlos = 1, .worldid = -1, .interiorid = -1);

	static 
		diniFormat[20],
		i;

	format(diniFormat, sizeof(diniFormat), "");

	if(!dini_Exists(STEELMESSAGES_PATH))
	{
		dini_Create(STEELMESSAGES_PATH);		

		for(i = 0; i < STEELASSISTANT_MAXMESSAGES; i++)
		{
			format(g_SteelsAssistantMessages[i], STEELASSISTANT_MESSAGE_SIZE + 1, "");
			format(diniFormat, sizeof(diniFormat), "Message%d", i);
			dini_Set(STEELMESSAGES_PATH, diniFormat, g_SteelsAssistantMessages[i]);
		}
	}
	else
	{
		for(i = 0; i < STEELASSISTANT_MAXMESSAGES; i++)
		{
			format(diniFormat, sizeof(diniFormat), "Message%d", i);			

			if(!dini_Isset(STEELMESSAGES_PATH, diniFormat)){
				dini_Set(STEELMESSAGES_PATH, diniFormat, "");
				format(g_SteelsAssistantMessages[i], STEELASSISTANT_MESSAGE_SIZE, "");
			}
			else
				format(g_SteelsAssistantMessages[i], STEELASSISTANT_MESSAGE_SIZE + 1, "%s", dini_Get(STEELMESSAGES_PATH, diniFormat));
		}
	}
	if(!dini_Exists(STEELPAYLOAD_PATH))
	{
		dini_Create(STEELPAYLOAD_PATH);
		for(i = 0; i < STEELASSISTANT_MAXPAYLOAD; i++)
		{
			g_SteelsActorAttributes[SteelsAssistantPayLoad][i] = 0;
			format(diniFormat, sizeof(diniFormat), "Payload%d", i);
			dini_IntSet(STEELPAYLOAD_PATH, diniFormat, g_SteelsActorAttributes[SteelsAssistantPayLoad][i]);
		}
		
	}
	else
	{		
		for(i = 0; i < STEELASSISTANT_MAXPAYLOAD; i++)
		{
			format(diniFormat, sizeof(diniFormat), "Payload%d", i);

			if(!dini_Isset(STEELPAYLOAD_PATH, diniFormat)){
				dini_IntSet(STEELPAYLOAD_PATH, diniFormat, 0);
				g_SteelsActorAttributes[SteelsAssistantPayLoad][i] = 0;
			}
			else
				g_SteelsActorAttributes[SteelsAssistantPayLoad][i] = dini_Int(STEELPAYLOAD_PATH, diniFormat);
		}		
	}

	g_SteelsActorAttributes[SteelsActor_IsBusy] = false;

	return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(newkeys & KEY_YES)	
	{
		if(IsPlayerInRangeOfPoint(playerid, 5.0, STEELASSISTANT_POSX, STEELASSISTANT_POSY, STEELASSISTANT_POSZ))
		{
			if(g_SteelsActorAttributes[SteelsActor_IsBusy])
				SendClientMessage(playerid, -1, "{FF6347}Info:{FFFFFF} The assistant is busy.");
			else
			{
				g_SteelsActorAttributes[SteelsActor_IsBusy] = true;
				_foundEmptySlot = -1;

				if(PlayerInfo[playerid][pID] == AARON_STEEL_ID)
					ShowSteelMainDialog(playerid);
				else
					ShowPlayerSteelMainDialog(playerid);
			}
		}
	}

	return 1;
}

forward STEEL_ResponsePlayerMain(playerid, dialogid, response, listitem, inputtext[]);
public STEEL_ResponsePlayerMain(playerid, dialogid, response, listitem, inputtext[]){
	#pragma unused dialogid, inputtext	

	if(!response){
		_foundEmptySlot = -1;
		g_SteelsActorAttributes[SteelsActor_IsBusy] = false;
		return 1;
	}

	inline Response(_pid, _dialogid, _response, _listitem, _inputtext[]){
		#pragma unused _dialogid, _listitem, _inputtext
		if(_response){
			ShowPlayerSteelMainDialog(_pid);
		}
	}


	new i, found = false;

	switch(listitem){
		case 0:{

			for(i = 0, found = false; i < STEELASSISTANT_MAXMESSAGES; i++){
				if(!strlen(g_SteelsAssistantMessages[i])){
					found = true;
					_foundEmptySlot = i;
					break;
				}										
			}

			if(found){
				Dialog_ShowCallback(playerid, using public STEEL_ResponseMSGInput, DIALOG_STYLE_INPUT, 
					"{43527D}Assistant:{FFFFFF}", 
					"\"What message do you want me to give him?\n\nDon't forget to tell me your name too!\"", 
					"Ok", "No");
			}
			else{

				Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_MSGBOX, 
					"{43527D}Assistant:{FFFFFF}", 
					"\"Sorry I cannot take any more messages for anyone.\"", 
					"Ok");

				return 1;
			}
		}
		case 1:{

			new aaronID = ReturnAaronSteelID();

			if(aaronID != INVALID_PLAYER_ID){

				if(!PlayerInfo[aaronID][pPnumber] || PhoneOffline[aaronID]){
					Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_MSGBOX,
						"{43527D}Assistant:{FFFFFF}", 
						"\"Sorry. He seems to be busy.\"", "Ok");
					return 1;
				}

				new str[128];
				format(str, sizeof(str), "SMS: You have a guest here at the lobby waiting for you, Sender: Assistant (N/A)");
				SendClientMessage(aaronID, COLOR_YELLOW, str);
			}
			else{
				Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_MSGBOX,
					"{43527D}Assistant:{FFFFFF}", 
					"\"Sorry. He seems to be out of town.\"", "Ok");
				return 1;
			}
		}
		case 2:{
			for(i = 0, found = false; i < STEELASSISTANT_MAXPAYLOAD; i++){
				if(!g_SteelsActorAttributes[SteelsAssistantPayLoad][i]){
					found = true;
					_foundEmptySlot = i;
					break;
				}
			}

			if(found){
				ShowPlayerSteelPayDialog(playerid);
			}
			else{

				Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_MSGBOX, 
					"{43527D}Assistant:{FFFFFF}", 
					"\"Sorry I cannot take any more payments for anyone.\"", 
					"Ok");
			}
		}		
	}

	return 1;
}

forward STEEL_ResponseMSGInput(playerid, dialogid, response, listitem, inputtext[]);
public STEEL_ResponseMSGInput(playerid, dialogid, response, listitem, inputtext[]){
	#pragma unused dialogid, listitem

	if(!response){
		_foundEmptySlot = -1;
		ShowPlayerSteelMainDialog(playerid);
		return 1;
	}

	inline Response(_playerid, _dialogid, _response, _listitem, _inputtext[]){
		#pragma unused _dialogid, _listitem, _inputtext

		if(_response)
			ShowPlayerSteelMainDialog(_playerid);
	}

	if(!strlen(inputtext)){		

		Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_MSGBOX, 
			"{43527D}Assistant:{FFFFFF}", 
			"\"You just stood there like a dummy. So I guess you don't want to say anything.\"", 
			"Ok");

		return 1;
	}

	if(strlen(inputtext) > 50){		

		Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_MSGBOX,
			"{43527D}Assistant:{FFFFFF}", 
			"\"That is a long message. Try to keep it short, around 50 letters.\"", 
			"Ok");

		return 1;	
	}

	format(g_SteelsAssistantMessages[_foundEmptySlot], STEELASSISTANT_MESSAGE_SIZE + 1, "%s", inputtext);

	new diniFormat[20];
	format(diniFormat, sizeof(diniFormat), "Message%d", _foundEmptySlot);
	dini_Set(STEELMESSAGES_PATH, diniFormat, g_SteelsAssistantMessages[_foundEmptySlot]);

	Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_MSGBOX, 
		"{43527D}Assistant:{FFFFFF}", 
		"\"I will inform him of your message.\"", 
		"Ok");

	new aaronID = ReturnAaronSteelID();

	if(aaronID != INVALID_PLAYER_ID){

		if(!PlayerInfo[aaronID][pPnumber] || PhoneOffline[aaronID])
			return 1;

		new str[128];
		format(str, sizeof(str), "SMS: You have a new message, Sender: Assistant (N/A)");
		SendClientMessage(aaronID, COLOR_YELLOW, str);
	}
	return 1;
}

forward STEEL_ResponsePlayerPay(playerid, dialogid, response, listitem, inputtext[]);
public STEEL_ResponsePlayerPay(playerid, dialogid, response, listitem, inputtext[]){

	#pragma unused dialogid, listitem

	if(response){

		new _deposit;

		if(sscanf(inputtext, "d", _deposit)){
			SendClientMessage(playerid, COLOR_GRAD2, "You must input numbers");
			ShowPlayerSteelPayDialog(playerid);
			return 1;
		}
		else{

			if(_deposit < 1 || _deposit > 100000){
				SendClientMessage(playerid, COLOR_GRAD2, "You cannot give less than $1 or more than $100000");
				ShowPlayerSteelPayDialog(playerid);
				return 1;
			}

			if(_deposit > PlayerInfo[playerid][pCash]){
				SendClientMessage(playerid, COLOR_GRAD2, "You do not have that much cash");
				ShowPlayerSteelPayDialog(playerid);
				return 1;
			}

			PlayerInfo[playerid][pCash] -= _deposit;
			GivePlayerMoney(playerid, -_deposit);

			g_SteelsActorAttributes[SteelsAssistantPayLoad][_foundEmptySlot] = _deposit;

			new diniFormat[20];

			format(diniFormat, sizeof(diniFormat), "Payload%d", _foundEmptySlot);
			dini_IntSet(STEELPAYLOAD_PATH, diniFormat, _deposit);

			inline Response(_playerid, _dialogid, _response, _listitem, _inputtext[]){
				#pragma unused _dialogid, _listitem, _inputtext
				if(_response){
					ShowPlayerSteelMainDialog(_playerid);
				}
			}

			Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_MSGBOX, 
				"{43527D}Assistant:{FFFFFF}", 
				"\"I will inform him of your payment.\"", "Ok");

			new aaronID = ReturnAaronSteelID();

			if(aaronID != INVALID_PLAYER_ID){

				if(!PlayerInfo[aaronID][pPnumber] || PhoneOffline[aaronID])
					return 1;

				new str[128];
				format(str, sizeof(str), "SMS: You have a new payment waiting to be collected, Sender: Assistant (N/A)");
				SendClientMessage(aaronID, COLOR_YELLOW, str);
			}
		}
	}
	else{

		ShowPlayerSteelMainDialog(playerid);
	}

	return 1;
}

forward Steel_ResponseMain(playerid, dialogid, response, listitem, inputtext[]);
public Steel_ResponseMain(playerid, dialogid, response, listitem, inputtext[]){

	#pragma unused dialogid, inputtext

	if(!response){
		_foundEmptySlot = -1;
		g_SteelsActorAttributes[SteelsActor_IsBusy] = false;
		return 1;
	}
	else{

		inline Response_MSG(_playerid, _dialogid, _response, _listitem, _inputtext[]){
			#pragma unused _dialogid, _inputtext

			if(_response){

				new i = 0, count = _listitem, diniFormat[20];

				for(i = 0; i < STEELASSISTANT_MAXMESSAGES; i++){

					if(strlen(g_SteelsAssistantMessages[i])){
						if(count <= 0){
							format(g_SteelsAssistantMessages[i], STEELASSISTANT_MESSAGE_SIZE + 1, "");
							format(diniFormat, sizeof(diniFormat), "Message%d", i);
							dini_Set(STEELMESSAGES_PATH, diniFormat, g_SteelsAssistantMessages[i]);
							break;		
						}
						else
							count--;
					}
					else
						continue;
				}

				Steel_ResponseMain(_playerid, 0, true, 0, "");
			}
			else
				ShowSteelMainDialog(_playerid);
		}

		inline Response_PAY(_playerid, _dialogid, _response, _listitem, _inputtext[]){
			#pragma unused _dialogid, _inputtext

			if(_response){

				new i = 0, count = _listitem, diniFormat[20];

				for(i = 0; i < STEELASSISTANT_MAXPAYLOAD; i++){

					if(g_SteelsActorAttributes[SteelsAssistantPayLoad][i]){
						if(count <= 0){
							g_SteelsActorAttributes[SteelsAssistantPayLoad][i] = 0;
							format(diniFormat, sizeof(diniFormat), "Payload%d", i);
							dini_IntSet(STEELPAYLOAD_PATH, diniFormat, g_SteelsActorAttributes[SteelsAssistantPayLoad][i]);
							break;
						}
						else
							count--;
					}
					else
						continue;
				}

				Steel_ResponseMain(_playerid, 0, true, 1, "");
			}
			else
				ShowSteelMainDialog(_playerid);
		}
		new i, found = 0;
		switch(listitem){
			case 0:{
				new str[(STEELASSISTANT_MESSAGE_SIZE * STEELASSISTANT_MAXMESSAGES) + (STEELASSISTANT_MAXMESSAGES * 5)];

				for(i = 0; i < STEELASSISTANT_MAXMESSAGES; i++){
					if(strlen(g_SteelsAssistantMessages[i])){
						if(!found){
							found = 1;
							format(str, sizeof(str), "%s", g_SteelsAssistantMessages[i]);
						}
						else
							format(str, sizeof(str), "%s\n%s", str, g_SteelsAssistantMessages[i]);
					}
				}

				if(!found){
					SendClientMessage(playerid, COLOR_GRAD2, "There are no messages for you");
					ShowSteelMainDialog(playerid);
				}
				else{
					Dialog_ShowCallback(playerid, using inline Response_MSG, DIALOG_STYLE_LIST, 
						"{43527D}Assistant:{FFFFFF} \"Here are your messages.\"", 
						str, "Delete", "Back");
				}					
			}
			case 1:{
				new str[(STEELASSISTANT_MAXPAYLOAD * 10)];

				for(i = 0; i < STEELASSISTANT_MAXPAYLOAD; i++){
					if(g_SteelsActorAttributes[SteelsAssistantPayLoad][i]){
						if(!found){
							found = 1;
							format(str, sizeof(str), "$%d", g_SteelsActorAttributes[SteelsAssistantPayLoad][i]);
						}
						else
							format(str, sizeof(str), "%s\n$%d", str, g_SteelsActorAttributes[SteelsAssistantPayLoad][i]);
					}
				}

				if(!found){
					SendClientMessage(playerid, COLOR_GRAD2, "There are no more payments for you");
					ShowSteelMainDialog(playerid);
				}
				else{
					Dialog_ShowCallback(playerid, using inline Response_PAY, DIALOG_STYLE_LIST, 
						"{43527D}Assistant:{FFFFFF} \"Here are your payments.\"", 
						str, "Take", "Back");
				}
			}
		}
	}
	return 1;
}

ReturnAaronSteelID(){

	foreach(new i: Player){

		if(PlayerInfo[i][pID] == AARON_STEEL_ID)
			return i;
	}

	return INVALID_PLAYER_ID;
}

ShowPlayerSteelMainDialog(playerid){

	Dialog_ShowCallback(playerid, using public STEEL_ResponsePlayerMain, DIALOG_STYLE_LIST, "{43527D}Assistant:{FFFFFF} \"Hello. How may I help you?\"", 
		"Give him my message.\n\
		Inform him I'm here.\n\
		Give him this payment.", "Select", "Exit");

	return 1;
}

ShowPlayerSteelPayDialog(playerid){

	Dialog_ShowCallback(playerid, using public STEEL_ResponsePlayerPay, DIALOG_STYLE_INPUT, 
		"{43527D}Assistant:{FFFFFF}", 
		"How much do you want to pay? Don't give me any more than $100,000!", 
		"Yes", "No");

	return 1;
}

ShowSteelMainDialog(playerid){

	Dialog_ShowCallback(playerid, using public Steel_ResponseMain, DIALOG_STYLE_LIST, 
		"{43527D}Assistant:{FFFFFF} \"Hello Mr. Steel. How may I help you today?\"", 
		"Give me my messages\n\
		Give me my payments", 
		"Ok", "No");
	return 1;
}

// A command for senior dev only so that they can reset the actor just in case something goes wrong //
CMD:resetsteelactor(playerid){

	if(PlayerInfo[playerid][pDev] < 2)
		return SendClientMessage(playerid, COLOR_GREY, "You are not allowed to use this command");

	g_SteelsActorAttributes[SteelsActor_IsBusy] = false;	

	return SendClientMessage(playerid, -1, "You resetted the actor");
}