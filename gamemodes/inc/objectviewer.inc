static str[128];

#define _SELECTION_ITEMS 	21
#define _ITEMS_PER_LINE  	7

#define HEADER_TEXT "Objects"

#define _DIALOG_BASE_X   	81.0
#define _DIALOG_BASE_Y   	84.0
#define _DIALOG_WIDTH    	550.0
#define _DIALOG_HEIGHT   	180.0
#define _SPRITE_DIM_X    	60.0
#define _SPRITE_DIM_Y    	70.0

#define BUY_FURNITURE_CATEGORY_DECO 1
#define BUY_FURNITURE_CATEGORY_OFFICE 2
#define BUY_FURNITURE_CATEGORY_HOBBY 3
#define BUY_FURNITURE_CATEGORY_LOUNGE 4
#define BUY_FURNITURE_CATEGORY_BEDROOM 5
#define BUY_FURNITURE_CATEGORY_KITCHEN 6
#define BUY_FURNITURE_CATEGORY_DINING 7
#define BUY_FURNITURE_CATEGORY_BATHROOM 8
#define BUY_FURNITURE_CATEGORY_LAUNDRY 9
#define BUY_FURNITURE_CATEGORY_ELECTRONICS 10
#define BUY_FURNITURE_CATEGORY_LIGHTS 11
#define BUY_FURNITURE_CATEGORY_OUTDOOR 12
#define BUY_FURNITURE_CATEGORY_STRUCTURE 13

new MAXIMUM_OBJECTS = 904;

new PlayerText:_gBackgroundTextDrawId[MAX_PLAYERS];
new PlayerText:prev_topbar[MAX_PLAYERS];
new PlayerText:prev_bottombar[MAX_PLAYERS];
new PlayerText:prev_topbar_line[MAX_PLAYERS];
new PlayerText:prev_houseicon[MAX_PLAYERS];
new PlayerText:prev_bottombar_line[MAX_PLAYERS];
new PlayerText:_gNextButtonTextDrawId[MAX_PLAYERS];
new PlayerText:_gPrevButtonTextDrawId[MAX_PLAYERS];
new PlayerText:_gSelectionItems[MAX_PLAYERS][_SELECTION_ITEMS];
new PlayerText:_gSelectionItemName[MAX_PLAYERS][_SELECTION_ITEMS];
new PlayerText:_gSelectionItemPrice[MAX_PLAYERS][_SELECTION_ITEMS];
new PlayerText:_gCurrentPageTextDrawId[MAX_PLAYERS];
new PlayerText:_gHeaderTextDrawId[MAX_PLAYERS];
new _gSelectionItemsTag[MAX_PLAYERS][_SELECTION_ITEMS];
new _gItemAt[MAX_PLAYERS];

//------------------------------------------------

_GetNumberOfPages(category)
{
	new total_pages = 0;
	
	switch(category) {
			case BUY_FURNITURE_CATEGORY_DECO: //Decorations
			{
				total_pages = 5;
			}
			case BUY_FURNITURE_CATEGORY_OFFICE: //Office
			{
				total_pages = 3;
			}
			case BUY_FURNITURE_CATEGORY_HOBBY: //Hobby
			{
				total_pages = 3;
			}
			case BUY_FURNITURE_CATEGORY_LOUNGE: //Lounge
			{
				total_pages = 3;
			}
			case BUY_FURNITURE_CATEGORY_BEDROOM: //Bedroom
			{
				total_pages = 3;
			}
			case BUY_FURNITURE_CATEGORY_KITCHEN: //Kitchen
			{
				total_pages = 7;
			}
			case BUY_FURNITURE_CATEGORY_DINING: //Dining
			{
				total_pages = 2;
			}
			case BUY_FURNITURE_CATEGORY_BATHROOM: //Bathroom
			{
				total_pages = 2;
			}
			case BUY_FURNITURE_CATEGORY_LAUNDRY: //Laundry
			{
				total_pages = 1;
			}
			case BUY_FURNITURE_CATEGORY_ELECTRONICS: //Electronics
			{
				total_pages = 3;
			}
			case BUY_FURNITURE_CATEGORY_LIGHTS: //Lights
			{
				total_pages = 1;
			}
			case BUY_FURNITURE_CATEGORY_OUTDOOR: //Outdoor
			{
				total_pages = 4;
			}
			case BUY_FURNITURE_CATEGORY_STRUCTURE: //Structure
			{
				total_pages = 4;
			}
		}
	return total_pages;
}

//------------------------------------------------
// Creates a model preview sprite

PlayerText:CreateModelPreviewTextDraw(playerid, modelindex, Float:Xpos, Float:Ypos, Float:width, Float:height, Float:rotX, Float:rotY)
{
    new PlayerText:txtPlayerSprite = CreatePlayerTextDraw(playerid, Xpos, Ypos, ""); // it has to be set with SetText later
    PlayerTextDrawFont(playerid, txtPlayerSprite, TEXT_DRAW_FONT_MODEL_PREVIEW);
    PlayerTextDrawColor(playerid, txtPlayerSprite, 0xFFFFFFFF);
    PlayerTextDrawBackgroundColor(playerid, txtPlayerSprite, 0x2E2E2E70);
    PlayerTextDrawTextSize(playerid, txtPlayerSprite, width, height); // Text size is the Width:Height
    PlayerTextDrawSetPreviewModel(playerid, txtPlayerSprite, modelindex);
    PlayerTextDrawSetSelectable(playerid, txtPlayerSprite, 1);
	PlayerTextDrawSetPreviewRot(playerid, txtPlayerSprite, rotX, rotY, 0.0, 1.0);
    PlayerTextDrawShow(playerid,txtPlayerSprite);
    return txtPlayerSprite;
}

PlayerText:CreateModelPreviewNameTextDraw(playerid, Float:Xpos, Float:Ypos, Float:Width, Float:Height, button_text[])
{
 	new PlayerText:txtInit;
   	txtInit = CreatePlayerTextDraw(playerid, Xpos, Ypos, button_text);
   	PlayerTextDrawUseBox(playerid, txtInit, 1);
   	PlayerTextDrawBoxColor(playerid, txtInit, 0x00000000);
   	//PlayerTextDrawBackgroundColor(playerid, txtInit, 0x000000FF);
	PlayerTextDrawLetterSize(playerid, txtInit, 0.15, 0.65);
	PlayerTextDrawFont(playerid, txtInit, 1);
	PlayerTextDrawSetShadow(playerid, txtInit, 0); // no shadow
    PlayerTextDrawSetOutline(playerid, txtInit, 0);
    PlayerTextDrawColor(playerid, txtInit, 0xD3AA29FF);
    PlayerTextDrawAlignment(playerid, txtInit, 2);
    PlayerTextDrawTextSize(playerid, txtInit, Height, Width); // The width and height are reversed for centering.. something the game does <g>
    PlayerTextDrawShow(playerid, txtInit);
    return txtInit;
}

PlayerText:CreateModelPreviewPriceTextDraw(playerid, Float:Xpos, Float:Ypos, Float:Width, Float:Height, button_text[])
{
 	new PlayerText:txtInit;
   	txtInit = CreatePlayerTextDraw(playerid, Xpos, Ypos, button_text);
   	PlayerTextDrawUseBox(playerid, txtInit, 1);
   	PlayerTextDrawBoxColor(playerid, txtInit, 0x00000000);
   	//PlayerTextDrawBackgroundColor(playerid, txtInit, 0x000000FF);
	PlayerTextDrawLetterSize(playerid, txtInit, 0.15, 0.65);
	PlayerTextDrawFont(playerid, txtInit, 1);
	PlayerTextDrawSetShadow(playerid, txtInit, 0); // no shadow
    PlayerTextDrawSetOutline(playerid, txtInit, 0);
    PlayerTextDrawColor(playerid, txtInit, 0x1A7E1CFF);
    PlayerTextDrawAlignment(playerid, txtInit, 2);
    PlayerTextDrawTextSize(playerid, txtInit, Height, Width); // The width and height are reversed for centering.. something the game does <g>
    PlayerTextDrawShow(playerid, txtInit);
    return txtInit;
}

//------------------------------------------------

_DestroyPlayerModelPreviews(playerid)
{
	new x=0;
	while(x != _SELECTION_ITEMS) {
	    if(_gSelectionItems[playerid][x] != PlayerText:INVALID_TEXT_DRAW) {
			PlayerTextDrawDestroy(playerid, _gSelectionItems[playerid][x]);
			_gSelectionItems[playerid][x] = PlayerText:INVALID_TEXT_DRAW;
			
			//destroy nametag
			PlayerTextDrawDestroy(playerid, _gSelectionItemName[playerid][x]);
			_gSelectionItemName[playerid][x] = PlayerText:INVALID_TEXT_DRAW;
			//destroy price
			PlayerTextDrawDestroy(playerid, _gSelectionItemPrice[playerid][x]);
			_gSelectionItemPrice[playerid][x] = PlayerText:INVALID_TEXT_DRAW;
		}
		x++;
	}
}

//------------------------------------------------

_ShowPlayerModelPreviews(playerid, category)
{
    new x=0;
	new Float:BaseX = _DIALOG_BASE_X;
	new Float:BaseY = _DIALOG_BASE_Y - (_SPRITE_DIM_Y * 0.33); // down a bit
	new linetracker = 0;
	
	new itemat = GetPVarInt(playerid, "ospawner_page") * _SELECTION_ITEMS;
	
	/*
	//DEBUG
	new dbstr[128];
	format(dbstr, sizeof(dbstr), "itemat is %d", itemat);
	SendClientMessage(playerid, -1, dbstr);
	
	format(dbstr, sizeof(dbstr), "_SELECTION_ITEMS is %d", _SELECTION_ITEMS);
	SendClientMessage(playerid, -1, dbstr);
	
	format(dbstr, sizeof(dbstr), "MAXIMUM_OBJECTS is %d", MAXIMUM_OBJECTS);
	SendClientMessage(playerid, -1, dbstr);
	*/
	
	// Destroy any previous ones created
	_DestroyPlayerModelPreviews(playerid);

	switch(category)
	{
		case BUY_FURNITURE_CATEGORY_DECO: //Decor
		{
			itemat = 0 + GetPVarInt(playerid, "ospawner_page") * _SELECTION_ITEMS;
			MAXIMUM_OBJECTS = 88;
		}
		case BUY_FURNITURE_CATEGORY_OFFICE: //Office
		{
			itemat = 88 + GetPVarInt(playerid, "ospawner_page") * _SELECTION_ITEMS;
			MAXIMUM_OBJECTS = 132;
		}
		case BUY_FURNITURE_CATEGORY_HOBBY: //Hobby
		{
			itemat = 132 + GetPVarInt(playerid, "ospawner_page") * _SELECTION_ITEMS;
			MAXIMUM_OBJECTS = 184;
		}
		case BUY_FURNITURE_CATEGORY_LOUNGE: //Lounge
		{
			itemat = 184 + GetPVarInt(playerid, "ospawner_page") * _SELECTION_ITEMS;
			MAXIMUM_OBJECTS = 231;
		}
		case BUY_FURNITURE_CATEGORY_BEDROOM: //Bedroom
		{
			itemat = 231 + GetPVarInt(playerid, "ospawner_page") * _SELECTION_ITEMS;
			MAXIMUM_OBJECTS = 281;
		}
		case BUY_FURNITURE_CATEGORY_KITCHEN: //Kitchen
		{
			itemat = 281 + GetPVarInt(playerid, "ospawner_page") * _SELECTION_ITEMS;
			MAXIMUM_OBJECTS = 417;
		}
		case BUY_FURNITURE_CATEGORY_DINING: //Dining
		{
			itemat = 417 + GetPVarInt(playerid, "ospawner_page") * _SELECTION_ITEMS;
			MAXIMUM_OBJECTS = 449;
		}
		case BUY_FURNITURE_CATEGORY_BATHROOM: //Bathroom
		{
			itemat = 449 + GetPVarInt(playerid, "ospawner_page") * _SELECTION_ITEMS;
			MAXIMUM_OBJECTS = 473;
		}
			case BUY_FURNITURE_CATEGORY_LAUNDRY: //Laundry
		{
			itemat = 473 + GetPVarInt(playerid, "ospawner_page") * _SELECTION_ITEMS;
			MAXIMUM_OBJECTS = 483;
		}
		case BUY_FURNITURE_CATEGORY_ELECTRONICS: //Eletronics
		{
			itemat = 483 + GetPVarInt(playerid, "ospawner_page") * _SELECTION_ITEMS;
			MAXIMUM_OBJECTS = 538;
		}
		case BUY_FURNITURE_CATEGORY_LIGHTS: //Lights
		{
			itemat = 538 + GetPVarInt(playerid, "ospawner_page") * _SELECTION_ITEMS;
			MAXIMUM_OBJECTS = 559;
		}
		case BUY_FURNITURE_CATEGORY_OUTDOOR: //Outdoor
		{
			itemat = 559 + GetPVarInt( playerid, "ospawner_page") * _SELECTION_ITEMS;
			MAXIMUM_OBJECTS = 626;
		}
			case BUY_FURNITURE_CATEGORY_STRUCTURE: //Structural Changes
		{
			itemat = 626 + GetPVarInt( playerid, "ospawner_page") * _SELECTION_ITEMS;
			MAXIMUM_OBJECTS = 693;
		}
	}
	
	while(x != _SELECTION_ITEMS && itemat < MAXIMUM_OBJECTS) {
		if(linetracker == 0) {
			BaseX = _DIALOG_BASE_X + 25.0; // in a bit from the box
			BaseY += _SPRITE_DIM_Y + 1.0; // move on the Y for the next line
		}
		
		_gSelectionItems[playerid][x] = CreateModelPreviewTextDraw(playerid, FurnitureObjectList[itemat][fD_model], BaseX, BaseY, _SPRITE_DIM_X, _SPRITE_DIM_Y, FurnitureObjectList[itemat][fD_rotX], FurnitureObjectList[itemat][fD_rotY]);
		_gSelectionItemsTag[playerid][x] = FurnitureObjectList[itemat][fD_model];
		_gSelectionItemName[playerid][x] = CreateModelPreviewNameTextDraw(playerid, BaseX + _SPRITE_DIM_X / 2, BaseY + _SPRITE_DIM_Y / 2 + 15, 50.0, 30.0, FurnitureObjectList[itemat][fD_name]);
		
		new pricestr[10];
		format(pricestr, sizeof(pricestr), "$%d", FurnitureObjectList[itemat][fD_price]);
		
		_gSelectionItemPrice[playerid][x] = CreateModelPreviewPriceTextDraw(playerid, BaseX + _SPRITE_DIM_X / 2, BaseY + _SPRITE_DIM_Y / 2 + 23, 50.0, 30.0, pricestr);
		
		BaseX += _SPRITE_DIM_X + 1.0; // move on the X for the next sprite
		linetracker++;
		if(linetracker == _ITEMS_PER_LINE) linetracker = 0;
		itemat++;
		x++;
	}
}

//------------------------------------------------

_UpdatePageTextDraw(playerid, category)
{
	new PageText[64+1];
	format(PageText, 64, "%d/%d", GetPVarInt(playerid,"ospawner_page") + 1, _GetNumberOfPages(category));
	PlayerTextDrawSetString(playerid, _gCurrentPageTextDrawId[playerid], PageText);
}

//------------------------------------------------

_CreateSelectionMenu(playerid, category)
{
	//background
	_gBackgroundTextDrawId[playerid] = CreatePlayerTextDraw(playerid, 92.000000, 125.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, _gBackgroundTextDrawId[playerid], 255);
	PlayerTextDrawFont(playerid,_gBackgroundTextDrawId[playerid], 1);
	PlayerTextDrawLetterSize(playerid, _gBackgroundTextDrawId[playerid], 0.500000, 25.000003);
	PlayerTextDrawColor(playerid, _gBackgroundTextDrawId[playerid], -1);
	PlayerTextDrawSetOutline(playerid, _gBackgroundTextDrawId[playerid], 0);
	PlayerTextDrawSetShadow(playerid, _gBackgroundTextDrawId[playerid], 1);
	PlayerTextDrawUseBox(playerid, _gBackgroundTextDrawId[playerid], 1);
    PlayerTextDrawBoxColor(playerid, _gBackgroundTextDrawId[playerid], 150);
	PlayerTextDrawTextSize(playerid, _gBackgroundTextDrawId[playerid], 547.000000, 106.000000);
    PlayerTextDrawShow(playerid, _gBackgroundTextDrawId[playerid]);
	
	//bottom bar
	prev_bottombar[playerid] = CreatePlayerTextDraw(playerid, 92.000000, 354.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, prev_bottombar[playerid], 255);
	PlayerTextDrawFont(playerid, prev_bottombar[playerid], 1);
	PlayerTextDrawLetterSize(playerid, prev_bottombar[playerid], 0.500000, 1.000002);
	PlayerTextDrawColor(playerid, prev_bottombar[playerid], -1);
	PlayerTextDrawSetOutline(playerid, prev_bottombar[playerid], 0);
	PlayerTextDrawSetShadow(playerid, prev_bottombar[playerid], 1);
	PlayerTextDrawUseBox(playerid, prev_bottombar[playerid], 1);
    PlayerTextDrawBoxColor(playerid, prev_bottombar[playerid], 180);
	PlayerTextDrawTextSize(playerid, prev_bottombar[playerid], 547.000000, 111.000000);
    PlayerTextDrawShow(playerid, prev_bottombar[playerid]);
	
	//top bar
	prev_topbar[playerid] = CreatePlayerTextDraw(playerid, 92.000000, 112.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, prev_topbar[playerid], 255);
	PlayerTextDrawFont(playerid, prev_topbar[playerid], 1);
	PlayerTextDrawLetterSize(playerid, prev_topbar[playerid], 0.500000, 1.000002);
	PlayerTextDrawColor(playerid, prev_topbar[playerid], -1);
	PlayerTextDrawSetOutline(playerid, prev_topbar[playerid], 0);
	PlayerTextDrawSetShadow(playerid, prev_topbar[playerid], 1);
	PlayerTextDrawUseBox(playerid, prev_topbar[playerid], 1);
    PlayerTextDrawBoxColor(playerid, prev_topbar[playerid], 180);
	PlayerTextDrawTextSize(playerid, prev_topbar[playerid], 547.000000, 111.000000);
    PlayerTextDrawShow(playerid, prev_topbar[playerid]);
	
	//bottom bar line
	prev_bottombar_line[playerid] = CreatePlayerTextDraw(playerid, 92.000000, 353.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, prev_bottombar_line[playerid], 255);
	PlayerTextDrawFont(playerid, prev_bottombar_line[playerid], 1);
	PlayerTextDrawLetterSize(playerid, prev_bottombar_line[playerid], 0.500000, -0.199996);
	PlayerTextDrawColor(playerid, prev_bottombar_line[playerid], -1);
	PlayerTextDrawSetOutline(playerid, prev_bottombar_line[playerid], 0);
	PlayerTextDrawSetShadow(playerid, prev_bottombar_line[playerid], 1);
	PlayerTextDrawUseBox(playerid, prev_bottombar_line[playerid], 1);
    PlayerTextDrawBoxColor(playerid, prev_bottombar_line[playerid], 336860310);
	PlayerTextDrawTextSize(playerid, prev_bottombar_line[playerid], 547.000000, 290.000000);
    PlayerTextDrawShow(playerid, prev_bottombar_line[playerid]);
	
	//top bar line
	prev_topbar_line[playerid] = CreatePlayerTextDraw(playerid, 92.000000, 124.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, prev_topbar_line[playerid], 255);
	PlayerTextDrawFont(playerid, prev_topbar_line[playerid], 1);
	PlayerTextDrawLetterSize(playerid, prev_topbar_line[playerid], 0.500000, -0.199995);
	PlayerTextDrawColor(playerid, prev_topbar_line[playerid], -1);
	PlayerTextDrawSetOutline(playerid, prev_topbar_line[playerid], 0);
	PlayerTextDrawSetShadow(playerid, prev_topbar_line[playerid], 1);
	PlayerTextDrawUseBox(playerid, prev_topbar_line[playerid], 1);
    PlayerTextDrawBoxColor(playerid, prev_topbar_line[playerid], 336860310);
	PlayerTextDrawTextSize(playerid, prev_topbar_line[playerid], 547.000000, 290.000000);
    PlayerTextDrawShow(playerid, prev_topbar_line[playerid]);
	
	//tshirt icon
	prev_houseicon[playerid] = CreatePlayerTextDraw(playerid, 94.000000, 112.000000, "hud:radar_propertyG");
	PlayerTextDrawBackgroundColor(playerid, prev_houseicon[playerid], 255);
	PlayerTextDrawFont(playerid, prev_houseicon[playerid], 4);
	PlayerTextDrawLetterSize(playerid, prev_houseicon[playerid], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, prev_houseicon[playerid], -1);
	PlayerTextDrawSetOutline(playerid, prev_houseicon[playerid], 0);
	PlayerTextDrawSetShadow(playerid, prev_houseicon[playerid], 1);
	PlayerTextDrawUseBox(playerid, prev_houseicon[playerid], 1);
    PlayerTextDrawBoxColor(playerid, prev_houseicon[playerid], -1);
	PlayerTextDrawTextSize(playerid, prev_houseicon[playerid], 9.000000, 10.000000);
    PlayerTextDrawShow(playerid, prev_houseicon[playerid]);
	
	//page number
	_gCurrentPageTextDrawId[playerid] = CreatePlayerTextDraw(playerid, 545.000000, 112.000000, "0/0");
	PlayerTextDrawAlignment(playerid, _gCurrentPageTextDrawId[playerid], 3);
	PlayerTextDrawBackgroundColor(playerid, _gCurrentPageTextDrawId[playerid], 255);
	PlayerTextDrawFont(playerid, _gCurrentPageTextDrawId[playerid], 1);
	PlayerTextDrawLetterSize(playerid, _gCurrentPageTextDrawId[playerid], 0.149999, 0.899999);
	PlayerTextDrawColor(playerid, _gCurrentPageTextDrawId[playerid], -2105376106);
	PlayerTextDrawSetOutline(playerid, _gCurrentPageTextDrawId[playerid], 0);
	PlayerTextDrawSetShadow(playerid, _gCurrentPageTextDrawId[playerid], 0);
	PlayerTextDrawUseBox(playerid, _gCurrentPageTextDrawId[playerid], 0);
	PlayerTextDrawSetProportional(playerid, _gCurrentPageTextDrawId[playerid], 1);
	PlayerTextDrawShow(playerid, _gCurrentPageTextDrawId[playerid]);
	
	//arrowright
	_gNextButtonTextDrawId[playerid] = CreatePlayerTextDraw(playerid, 537.000000, 354.000000, "LD_BEAT:right");
	PlayerTextDrawBackgroundColor(playerid, _gNextButtonTextDrawId[playerid], 255);
	PlayerTextDrawFont(playerid, _gNextButtonTextDrawId[playerid], 4);
	PlayerTextDrawLetterSize(playerid, _gNextButtonTextDrawId[playerid], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, _gNextButtonTextDrawId[playerid], -1);
	PlayerTextDrawSetOutline(playerid, _gNextButtonTextDrawId[playerid], 0);
	PlayerTextDrawSetShadow(playerid, _gNextButtonTextDrawId[playerid], 1);
	PlayerTextDrawUseBox(playerid, _gNextButtonTextDrawId[playerid], 1);
    PlayerTextDrawBoxColor(playerid, _gNextButtonTextDrawId[playerid], 255);
	PlayerTextDrawTextSize(playerid, _gNextButtonTextDrawId[playerid], 8.000000, 9.000000);
	PlayerTextDrawSetSelectable(playerid, _gNextButtonTextDrawId[playerid], 1);
    PlayerTextDrawShow(playerid, _gNextButtonTextDrawId[playerid]);
	
	//arrowleft
	_gPrevButtonTextDrawId[playerid] = CreatePlayerTextDraw(playerid, 524.000000, 354.000000, "LD_BEAT:left");
	PlayerTextDrawBackgroundColor(playerid, _gPrevButtonTextDrawId[playerid], 255);
	PlayerTextDrawFont(playerid, _gPrevButtonTextDrawId[playerid], 4);
	PlayerTextDrawLetterSize(playerid, _gPrevButtonTextDrawId[playerid], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, _gPrevButtonTextDrawId[playerid], -1);
	PlayerTextDrawSetOutline(playerid, _gPrevButtonTextDrawId[playerid], 0);
	PlayerTextDrawSetShadow(playerid, _gPrevButtonTextDrawId[playerid], 1);
	PlayerTextDrawUseBox(playerid, _gPrevButtonTextDrawId[playerid], 1);
    PlayerTextDrawBoxColor(playerid, _gPrevButtonTextDrawId[playerid], 255);
	PlayerTextDrawTextSize(playerid, _gPrevButtonTextDrawId[playerid], 8.000000, 9.000000);
	PlayerTextDrawSetSelectable(playerid, _gPrevButtonTextDrawId[playerid], 1);
    PlayerTextDrawShow(playerid, _gPrevButtonTextDrawId[playerid]);

    _ShowPlayerModelPreviews(playerid, category);
    _UpdatePageTextDraw(playerid, category);
}

//------------------------------------------------

_DestroySelectionMenu(playerid)
{
	_DestroyPlayerModelPreviews(playerid);

	PlayerTextDrawDestroy(playerid, _gHeaderTextDrawId[playerid]);
	PlayerTextDrawDestroy(playerid, _gBackgroundTextDrawId[playerid]);
	PlayerTextDrawDestroy(playerid, prev_bottombar[playerid]); //bottombar
	PlayerTextDrawDestroy(playerid, prev_topbar[playerid]); //topbar
	PlayerTextDrawDestroy(playerid, prev_topbar_line[playerid]); //topbar line
	PlayerTextDrawDestroy(playerid, prev_bottombar_line[playerid]);
	PlayerTextDrawDestroy(playerid, prev_houseicon[playerid]);
	PlayerTextDrawDestroy(playerid, _gCurrentPageTextDrawId[playerid]);
	PlayerTextDrawDestroy(playerid, _gNextButtonTextDrawId[playerid]);
	PlayerTextDrawDestroy(playerid, _gPrevButtonTextDrawId[playerid]);

	_gHeaderTextDrawId[playerid] = PlayerText:INVALID_TEXT_DRAW;
    _gBackgroundTextDrawId[playerid] = PlayerText:INVALID_TEXT_DRAW;
	prev_bottombar[playerid] = PlayerText:INVALID_TEXT_DRAW; //buttombar
	prev_topbar[playerid] = PlayerText:INVALID_TEXT_DRAW; //topbar
	prev_topbar_line[playerid] = PlayerText:INVALID_TEXT_DRAW; //topbar line
	prev_bottombar_line[playerid] = PlayerText:INVALID_TEXT_DRAW; //bottombar line
	prev_houseicon[playerid] = PlayerText:INVALID_TEXT_DRAW; //bottombar line
    _gCurrentPageTextDrawId[playerid] = PlayerText:INVALID_TEXT_DRAW;
    _gNextButtonTextDrawId[playerid] = PlayerText:INVALID_TEXT_DRAW;
    _gPrevButtonTextDrawId[playerid] = PlayerText:INVALID_TEXT_DRAW;
}

stock getFurnitureIndexFromModel(modelid) {
	for(new i; i < sizeof(FurnitureObjectList); i++) {
		if(modelid == FurnitureObjectList[i][fD_model]) {
			return i;
		}
	}
	return -1;
}

HandleObjectItemSelection(playerid, selecteditem) {
	
	if(PlayerInfo[playerid][pCash] < FurnitureObjectList[selecteditem][fD_price]) {
		format(str, sizeof(str), "You cant afford the %s.", FurnitureObjectList[selecteditem][fD_name]);
		SendClientMessage(playerid, COLOR_GREY, str);
		return 1;
	}
	
	//add furniture to players house array
	new houseid = PlayerInfo[playerid][pHouseID];
	new modelid = FurnitureObjectList[selecteditem][fD_model];
	
	if(PlayerInfo[playerid][pHouseID] == 0) return SendClientMessage(playerid, COLOR_GREY, "You do not own a house.");
	if(HouseInfo[houseid][hFurnitureAmount] >= MAX_FURNITURE) return SendClientMessage(playerid, COLOR_GREY, "You have reached the max amount of furniture slots.");
	
	CreateHouseFurniture(playerid, houseid, modelid, selecteditem); //ugly
	return 1;
}

//------------------------------------------------
stock clearObjectViewerTD(playerid) {
	// Init all of the textdraw related globals
    _gHeaderTextDrawId[playerid] = PlayerText:INVALID_TEXT_DRAW;
    _gBackgroundTextDrawId[playerid] = PlayerText:INVALID_TEXT_DRAW;
	prev_bottombar[playerid] = PlayerText:INVALID_TEXT_DRAW;
	prev_topbar[playerid] = PlayerText:INVALID_TEXT_DRAW;
	prev_topbar_line[playerid] = PlayerText:INVALID_TEXT_DRAW;
	prev_bottombar_line[playerid] = PlayerText:INVALID_TEXT_DRAW;
	prev_houseicon[playerid] = PlayerText:INVALID_TEXT_DRAW;
    _gCurrentPageTextDrawId[playerid] = PlayerText:INVALID_TEXT_DRAW;
    _gNextButtonTextDrawId[playerid] = PlayerText:INVALID_TEXT_DRAW;
    _gPrevButtonTextDrawId[playerid] = PlayerText:INVALID_TEXT_DRAW;
    
	new x=0;
	while(x != _SELECTION_ITEMS) {
        _gSelectionItems[playerid][x] = PlayerText:INVALID_TEXT_DRAW;
		_gSelectionItemName[playerid][x] = PlayerText:INVALID_TEXT_DRAW;
		_gSelectionItemPrice[playerid][x] = PlayerText:INVALID_TEXT_DRAW;
        x++;
	}

	_gItemAt[playerid] = 0;
	return 1;
}

stock playerClickOTextDraw(playerid, Text:clickedid) {

	if(GetPVarInt(playerid, "ospawner_active") == 0) return 0;

	// Handle: They cancelled (with ESC)
	if(clickedid == Text:INVALID_TEXT_DRAW) {
        _DestroySelectionMenu(playerid);
        SetPVarInt(playerid, "ospawner_active", 0);
        PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
        return 1;
	}
	return 1;
}

//------------------------------------------------
stock playerClickObjectTextDraw(playerid, PlayerText:playertextid) {
	if(GetPVarInt(playerid, "ospawner_active") == 0) return 0;

	new curpage = GetPVarInt(playerid, "ospawner_page");
	new category = GetPVarInt(playerid, "ospawner_category");
	
	// Handle: next button
	if(playertextid == _gNextButtonTextDrawId[playerid]) {
	    if(curpage < (_GetNumberOfPages(category) - 1)) {
	        SetPVarInt(playerid, "ospawner_page", curpage + 1);
	        _ShowPlayerModelPreviews(playerid, category);
         	_UpdatePageTextDraw(playerid, category);
         	PlayerPlaySound(playerid, 1083, 0.0, 0.0, 0.0);
		} else {
		    PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
		}
		return 1;
	}
	
	// Handle: previous button
	if(playertextid == _gPrevButtonTextDrawId[playerid]) {
	    if(curpage > 0) {
	    	SetPVarInt(playerid, "ospawner_page", curpage - 1);
	    	_ShowPlayerModelPreviews(playerid, category);
	    	_UpdatePageTextDraw(playerid, category);
	    	PlayerPlaySound(playerid, 1084, 0.0, 0.0, 0.0);
		} else {
		    PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
		}
		return 1;
	}
	
	// Search in the array of textdraws used for the items
	new x=0;
	while(x != _SELECTION_ITEMS) {
	    if(playertextid == _gSelectionItems[playerid][x]) {
			
			/*
			format(str, sizeof(str), "x is %d.", x);
			SendClientMessage(playerid, COLOR_GREY, str);
			
			format(str, sizeof(str), "_gSelectionItemsTag[playerid][x] is %d.", _gSelectionItemsTag[playerid][x]);
			SendClientMessage(playerid, COLOR_GREY, str);
			*/
			
			new i = getFurnitureIndexFromModel(_gSelectionItemsTag[playerid][x]);
			if(i != -1) {
				
				/*
				format(str, sizeof(str), "found match at index %d.", i);
				SendClientMessage(playerid, COLOR_GREY, str);
				*/
				
				format(str, sizeof(str), "Are you sure you want to pay $%d for a %s?", FurnitureObjectList[i][fD_price], FurnitureObjectList[i][fD_name]);
				ShowPlayerDialog(playerid, DIALOG_FURNITURE_BUY_CONFIRM, DIALOG_STYLE_MSGBOX, "Confirm", str, "Buy", "Back");
				
				SetPVarInt(playerid, "buying_furniture_id", i);
				PlayerPlaySound(playerid, 1083, 0.0, 0.0, 0.0);
				_DestroySelectionMenu(playerid);
				CancelSelectTextDraw(playerid);
				SetPVarInt(playerid, "ospawner_active", 0);
			}
        	return 1;
		}
		x++;
	}
	return 1;
}