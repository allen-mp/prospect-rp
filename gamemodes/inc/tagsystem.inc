/*
  _______             _____           _                   ____                _____                                      _ 
 |__   __|           / ____|         | |                 |  _ \              |  __ \                                    | |
    | | __ _  __ _  | (___  _   _ ___| |_ ___ _ __ ___   | |_) |_   _        | |  | | ___  ___ _ __ ___   ___  _ __   __| |
    | |/ _` |/ _` |  \___ \| | | / __| __/ _ \ '_ ` _ \  |  _ <| | | |       | |  | |/ _ \/ __| '_ ` _ \ / _ \| '_ \ / _` |
    | | (_| | (_| |  ____) | |_| \__ \ ||  __/ | | | | | | |_) | |_| |       | |__| |  __/\__ \ | | | | | (_) | | | | (_| |
    |_|\__,_|\__, | |_____/ \__, |___/\__\___|_| |_| |_| |____/ \__, |       |_____/ \___||___/_| |_| |_|\___/|_| |_|\__,_|
              __/ |          __/ |                               __/ |                                                     
             |___/          |___/                               |___/                                                     

Things that aren't here (couldn't place them here):
1. OnPlayerLogin sync the tag's position
2. Added Tagsplaced (counter for each family's current tags) to fInfo
3. The #include
4. OnPlayerEditObject (couldn't hook it for some reason)

Please note that if you wish to disable this system you will have to disable the items above as well (a part from commenting the #include or removing this file)
*/
// increase MAX_TAGS by 20 if you would like to have +1 tag for each family
// + you will have to increase the if statement in /createtag to the maximum tags (this line below):
// if(FamilyInfo[PlayerInfo[playerid][pFMember]][Tagsplaced] >= 4) return SendClientMessage(playerid, COLOR_GREY, "Your family already reached the maximum tag limit.");
#define MAX_TAGS	(100)
#define DIALOG_TAG_TEXT (16903)
#define DIALOG_TAG_BOLD (16904)
#define DIALOG_TAG_SIZE (16905)
#define DIALOG_TAG_COLOR (16906)
#define DIALOG_TAG_FACE (16907)
#define DIALOG_TAG_FINAL (16908)
#define DIALOG_TAG_SHOW (16909)


enum TagsInfo { 
	Float:TagPosition[3],
	Float:TagRotation[3],
	TagCreator[MAX_PLAYER_NAME + 1],
	TagFamily,
	bool:TagCreated,
	TagText[34],
	TagObject,
	TagColor[21],
	TagTime
};

new Tags[MAX_TAGS][TagsInfo];
new tagtext[MAX_PLAYERS][34], tagbold[MAX_PLAYERS], tagcolor[MAX_PLAYERS][21], tagfont[MAX_PLAYERS][30];
new tagdisappeartime = 48*3600; // 48 is the hours. 3600 seconds = 60 minutes = 1 hour
new tagspoll;

CMD:createtag(playerid, params[])
{
	if(PlayerInfo[playerid][pFMember] == 255) return SendClientMessage(playerid, COLOR_GREY, "You are not in a Family.");
	if(PlayerInfo[playerid][pRank] < 2) return SendClientMessage(playerid, COLOR_GREY, "You must be a R2+ to use this command.");
	if(FamilyInfo[PlayerInfo[playerid][pFMember]][Tagsplaced] >= 4) return SendClientMessage(playerid, COLOR_GREY, "Your family already reached the maximum tag limit.");
	if(PlayerTied[playerid] != 0 || PlayerCuffed[playerid] != 0 || PlayerFrozen[playerid] != 0) return SendClientMessage(playerid, COLOR_GREY, "You can't do that at this time.");
	if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can not use this while being in a vehicle.");
	if(PlayerInfo[playerid][pSpraycan] < 1) return SendClientMessage(playerid, COLOR_GREY, "You need a spraycan to do this.");
	if(tagspoll >= 80) return SendClientMessage(playerid, COLOR_GREY, "Tags amount have reached the maximum.");

	ShowPlayerDialog(playerid, DIALOG_TAG_TEXT, DIALOG_STYLE_INPUT, "Tag Text", "Please enter a text for your tag (min. 3 max. 19 characters long)", "Next", "Cancel");

	return 1;
}

CMD:tags(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 2 || PlayerInfo[playerid][pDev] >= 2)
	{
		new finaltagsstr[6000];
		for(new i = 0; i < MAX_TAGS; i++)
		{
			if(Tags[i][TagCreated] != true) continue;
			format(finaltagsstr, sizeof(finaltagsstr), "%s %i. %s{FFFFFF}  by %s for %s\n", finaltagsstr, i, Tags[i][TagText], Tags[i][TagCreator], FamilyInfo[Tags[i][TagFamily]][FamilyName]);
		}
		ShowPlayerDialog(playerid, DIALOG_TAG_SHOW, DIALOG_STYLE_LIST, "Active Tags", finaltagsstr, "Okay", "");
	}
	else if(PlayerInfo[playerid][pRank] >= 5 && PlayerInfo[playerid][pFMember] != 255)
	{
		new finaltagsstr[200];
		for(new i = 0; i < MAX_TAGS; i++)
		{
			if(Tags[i][TagCreated] != true) continue;
			if(Tags[i][TagFamily] != PlayerInfo[playerid][pFMember]) continue;
			format(finaltagsstr, sizeof(finaltagsstr), "%s %i. %s{FFFFFF}  by %s\n", finaltagsstr, i, Tags[i][TagText], Tags[i][TagCreator]);
		}
		ShowPlayerDialog(playerid, DIALOG_TAG_SHOW, DIALOG_STYLE_LIST, "Your family's tags.", finaltagsstr, "Okay", "");
	}
	return 1;
}

CMD:removetag(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 2 || PlayerInfo[playerid][pDev] >= 2)
	{
		new tempid;
		if(sscanf(params, "i", tempid)) return SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} /removetag [id]");
		if(Tags[tempid][TagCreated] != true) return SendClientMessage(playerid, COLOR_GREY, "That tag ID is not active.");
		FamilyInfo[Tags[tempid][TagFamily]][Tagsplaced] -= 1;
		SendClientMessage(playerid, COLOR_GREY, "You have successfully removed that tag ID.");
		Tags[tempid][TagFamily] = -1;
		Tags[tempid][TagCreated] = false;
		DestroyObject(Tags[tempid][TagObject]);
		tagspoll --;

	}
	else if(PlayerInfo[playerid][pFMember] != 255)
	{
		if(PlayerInfo[playerid][pRank] < 5 && PlayerInfo[playerid][pRank] > 1)
		{
			for(new i = 0; i < MAX_TAGS; i++)
			{
				if(IsPlayerInRangeOfPoint(playerid, 5.0, Tags[i][TagPosition][0], Tags[i][TagPosition][1], Tags[i][TagPosition][2]))
				{
					if(Tags[i][TagCreated] == true && Tags[i][TagFamily] == PlayerInfo[playerid][pFMember])
					{
						if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can not use this while in a vehicle.");
						FamilyInfo[Tags[i][TagFamily]][Tagsplaced] -= 1;
						SendClientMessage(playerid, COLOR_GREY, "You have successfully removed this family tag.");
						Tags[i][TagFamily] = -1;
						Tags[i][TagCreated] = false;
						DestroyObject(Tags[i][TagObject]);
						tagspoll --;
						break;
					}
				}
			}
		}	
		else if(PlayerInfo[playerid][pRank] >= 5)
		{
			new tempid;
			if(sscanf(params, "i", tempid)) return SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} /removetag [id]");
			if(Tags[tempid][TagFamily] != PlayerInfo[playerid][pFMember]) return SendClientMessage(playerid, COLOR_GREY, "That id is not your family's tag.");
			if(Tags[tempid][TagCreated] != true) return SendClientMessage(playerid, COLOR_GREY, "That tag ID is not active.");

			FamilyInfo[Tags[tempid][TagFamily]][Tagsplaced] -= 1;
			SendClientMessage(playerid, COLOR_GREY, "You have successfully removed this family tag.");
			Tags[tempid][TagFamily] = -1;
			Tags[tempid][TagCreated] = false;
			DestroyObject(Tags[tempid][TagObject]);		
			tagspoll --;	
		}
	}
	return 1;
}

Hook:Tags_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_TAG_TEXT)
	{
		if(response)
		{
			if(strlen(inputtext) < 3 || strlen(inputtext) > 19)
			{
				SendClientMessage(playerid, COLOR_GREY, "The tag text's length must be between 3 and 19 characters");
				ShowPlayerDialog(playerid, DIALOG_TAG_TEXT, DIALOG_STYLE_INPUT, "Tag Text", "Please enter a text for your tag (min. 3 max. 19 characters long)", "Next", "Cancel");
			}
			else
			{
				strdel(tagtext[playerid], 0, 33);
				strins(tagtext[playerid], inputtext, 0);
				ShowPlayerDialog(playerid, DIALOG_TAG_BOLD, DIALOG_STYLE_LIST, "Would you like to bold the text?", "Yes\nNo", "Next", "Cancel");
			}
		}
	}
	if(dialogid == DIALOG_TAG_BOLD)
	{
		if(response)
		{
			switch(listitem)
			{
				case 0: tagbold[playerid] = 1;
				case 1: tagbold[playerid] = 0;
			}
			ShowPlayerDialog(playerid, DIALOG_TAG_COLOR, DIALOG_STYLE_LIST, "Text Color", "Red\nGreen\nBlue\nPurple\nBlack\nWhite", "Set", "Cancel");
		}	
	}
	if(dialogid == DIALOG_TAG_COLOR)
	{
		if(response)
		{
			switch(listitem)
			{
				case 0: 
				{
					tagcolor[playerid] = "Red"; 
					format(tagtext[playerid], 35, "{ff0000}%s", tagtext[playerid]);
				}	
				case 1: 
				{
					tagcolor[playerid] = "Green"; 
					format(tagtext[playerid], 35, "{33cc33}%s", tagtext[playerid]);
				}	
				case 2: 
				{
					tagcolor[playerid] = "Blue"; 
					format(tagtext[playerid], 35, "{0000ff}%s", tagtext[playerid]);
				}	
				case 3: 
				{
					tagcolor[playerid] = "Purple"; 
					format(tagtext[playerid], 35, "{660066}%s", tagtext[playerid]);
				}	
				case 4: 
				{
					tagcolor[playerid] = "Black"; 
					format(tagtext[playerid], 35, "{000000}%s", tagtext[playerid]);
				}	
				case 5: 
				{
					tagcolor[playerid] = "White"; 
					format(tagtext[playerid], 35, "{ffffff}%s", tagtext[playerid]);
				}	
			}

			ShowPlayerDialog(playerid, DIALOG_TAG_FACE, DIALOG_STYLE_LIST, "Text Font (google them if you don't know what is what)", "Arial\nAldhabi\nEstrangelo Edessa", "Set", "Cancel");
		}
	}
	if(dialogid == DIALOG_TAG_FACE)
	{
		if(response)
		{
			switch(listitem)
			{
				case 0: tagfont[playerid] = "Arial";
				case 1: tagfont[playerid] = "Aldhabi";
				case 2: tagfont[playerid] = "Estrangelo Edessa";
			}

			new finalstr[500];

			if(tagbold[playerid] == 1) format(finalstr, sizeof(finalstr), "{FFFFFF}Tag Text: %s{FFFFFF}\n\nTag Bolded: Yes\n\nTag Color: %s\n\nTag Font: %s", tagtext[playerid], tagcolor[playerid], tagfont[playerid]);
			else format(finalstr, sizeof(finalstr), "{FFFFFF}Tag Text: %s{FFFFFF}\n\nTag Bolded: No\n\nTag Color: %s\n\nTag Font: %s", tagtext[playerid], tagcolor[playerid], tagfont[playerid]);
			ShowPlayerDialog(playerid, DIALOG_TAG_FINAL, DIALOG_STYLE_MSGBOX, "Tag Information", finalstr, "Tag", "Cancel");
		}
	}
	if(dialogid == DIALOG_TAG_FINAL)
	{
		if(response)
		{
			new Float:temppos[3];
			GetPlayerPos(playerid, temppos[0], temppos[1], temppos[2]);
			PlayerInfo[playerid][pSpraycan] --;
			for(new i = 0; i < MAX_TAGS; i++)
			{
				if(Tags[i][TagCreated] == false)
				{
					Tags[i][TagText] = tagtext[playerid];
					Tags[i][TagColor] = tagcolor[playerid];

					Tags[i][TagPosition][0] = temppos[0];
					Tags[i][TagPosition][1] = temppos[1];
					Tags[i][TagPosition][2] = temppos[2];

					Tags[i][TagRotation][0] = 0;		 		
					Tags[i][TagRotation][1] = 0;
					Tags[i][TagRotation][2] = 0;

					Tags[i][TagCreated] = true;

					GetPlayerName(playerid, Tags[i][TagCreator], MAX_PLAYER_NAME+1);
					Tags[i][TagFamily] = PlayerInfo[playerid][pFMember];
					FamilyInfo[PlayerInfo[playerid][pFMember]][Tagsplaced] ++;

					Tags[i][TagTime] = tagdisappeartime;

					Tags[i][TagObject] = CreateObject(19482, temppos[0], temppos[1], temppos[2], 0.0, 0.0, 0.0);
					SetObjectMaterialText(Tags[i][TagObject], tagtext[playerid], 0, OBJECT_MATERIAL_SIZE_256x128, tagfont[playerid], 25, tagbold[playerid], 0xFF0000FF, 0, 0);
					EditObject(playerid, Tags[i][TagObject]);
					tagspoll ++;
					break;
				}
			}

			for(new i = 0; i < MAX_TAGS; i++) // fixed syncing problem with tags
			{
				if(Tags[i][TagCreated] == true)
				{
					SetObjectPos(Tags[i][TagObject], Tags[i][TagPosition][0], Tags[i][TagPosition][1], Tags[i][TagPosition][2]);
					SetObjectRot(Tags[i][TagObject], Tags[i][TagRotation][0], Tags[i][TagRotation][1], Tags[i][TagRotation][2]);
				}
			}
		}
	}

	return 1;
}

forward TagsDisappear();

Hook:Tags_OnGameModeInit()
{
	SetTimer("TagsDisappear", 1000, true);
	return 1;
}

public TagsDisappear()
{
	for(new i = 0; i < MAX_TAGS; i++)
	{
		if(Tags[i][TagCreated] == true)
		{
			if(Tags[i][TagTime] > 0) Tags[i][TagTime] --;
			if(Tags[i][TagTime] <= 0)
			{
				Tags[i][TagCreated] = false;
				FamilyInfo[Tags[i][TagFamily]][Tagsplaced] --;
				DestroyObject(Tags[i][TagObject]);
				tagspoll --;
			}
		}
	}
	return 1;
}