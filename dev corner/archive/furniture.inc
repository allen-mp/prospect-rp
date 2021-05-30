static str[128];
enum furnInfoDict {
	fD_type,
	fD_name[50],
	fD_model,
	fD_price
}
new FurnitureDict[99][furnInfoDict] = {
	{ 1, "800W Microwave", 2149, 50000 },
	{ 1, "Bar Fridge", 2147, 350000 },
	{ 1, "Desktop Computer", 2190, 200000 },
	{ 1, "Small Black TV", 1749, 50000 },
	{ 1, "Medium Wooden TV", 1429, 75000 },
	{ 1, "Large Modern Black TV", 1792, 100000 },
	{ 1, "Black Small Wall LCD TV", 19787, 125000 },
	{ 1, "Black Large Wall LCD TV", 19786, 150000 },
	{ 2, "Red flower pot", 2245, 2000 },
	{ 2, "Blue flower pot, fancy", 2251, 4000 },
	{ 2, "Budget Computer Desk", 2180, 15000 },
	{ 2, "Modern Computer Desk", 2169, 80000 },
	{ 2, "Business Computer Desk", 2169, 120000 },
	{ 2, "Blue Computer Chair", 2356, 15000 },
	{ 2, "Foldup Chair, Budget", 2121, 5000 },
	{ 2, "Wooden Chair", 2310, 8000 },
	{ 2, "Wooden & Leather Dining Chair", 2120, 12000 },
	{ 2, "Leather Computer Chair", 1714, 20000 },
	{ 2, "Black 1-seated Sofa", 1704, 30000 },
	{ 2, "Wooden Rocking Chair", 11734, 80000 },
	{ 2, "Luxary 1-seated Sofa", 1708, 100000 },
	{ 2, "Old-fashioned Chair", 1735, 80000 },
	{ 2, "Budget 1-seated Sofa", 1711, 40000 },
	{ 2, "Brown Ottomat", 2293, 20000 },
	{ 2, "Budget Moss 3-seated Sofa", 1764, 100000 },
	{ 2, "Budget 2-seated Sofa", 1728, 90000 },
	{ 2, "2-seated Dark Brown Sofa", 1757, 120000 },
	{ 2, "Country 3-seat and 2-seat", 14493, 450000 },
	{ 2, "Modern Black 2-seated Sofa", 1723, 250000 },
	{ 2, "Modern Brown 2-seated Sofa", 1702, 180000 },
	{ 2, "Budget 2-seated Couch", 1763, 80000 },
	{ 2, "4-seated Budget Couch", 1710, 350000 },
	{ 2, "Designer Trunk Couch", 1707, 400000 },
	{ 2, "Budget 3-seated Blue Couch", 1768, 280000 },
	{ 2, "Swank Couch, 3-seated", 1761, 150000 },
	{ 2, "Luxary Couch, 2-seated", 1713, 350000 },
	{ 2, "Coffee Table, Glass & Brown", 1817, 110000 },
	{ 2, "Budget Coffee Table", 1814, 50000 },
	{ 2, "Dark Brown Coffee Table", 2236, 90000 },
	{ 2, "Rounded Coffee Table", 1815, 80000 },
	{ 2, "Rounded Glass Coffee Table", 1822, 150000 },
	{ 2, "Budget Single Bed", 1771, 50000 },
	{ 2, "Budget Double Bed", 1794, 100000 },
	{ 2, "Modern Double Bed", 2302, 120000 },
	{ 2, "Modern Double Bed 2", 2299, 140000 },
	{ 2, "Luxary Queen Bed", 14866, 180000 },
	{ 2, "Luxary King Bed", 11720, 210000 },
	{ 2, "Luxary Double Bed & Tables", 2298, 300000 },
	{ 2, "Double Bed with Back Shelfing", 2301, 250000 },
	{ 2, "King Double Bed, Luxary", 14446, 500000 },
	{ 2, "Simple Double Bed", 1745, 120000 },
	{ 2, "Fancy Double Bed", 1701, 150000 },
	{ 2, "Cheap Double Bed", 1801, 100000 },
	{ 2, "Love/Heart Double Bed", 11731, 450000 },
	{ 2, "Modern Double Bed with Side Tables", 2566, 480000 },
	{ 2, "Modern x2 Single Beds, Tables Set", 2565, 550000 },
	{ 2, "Blue Round Rug", 2841, 15000 },
	{ 2, "Orange Rug", 2818, 15000 },
	{ 2, "Tiger Rug", 1828, 80000 },
	{ 2, "Pattern Brown Lounge Rug", 2833, 40000 },
	{ 2, "Green Rug", 2817, 15000 },
	{ 2, "Picture of Beach", 2280, 15000 },
	{ 2, "Picture of Sunset", 2281, 15000 },
	{ 2, "Picture of Bridge", 2276, 15000 },
	{ 2, "Picture of Ship", 2286, 15000 },
	{ 2, "Modern Art", 2257, 15000 },
	{ 2, "Picture of Palms", 2256, 15000 },
	{ 2, "Picture of Car", 2254, 15000 },
	{ 2, "Picture of Girl", 2255, 15000 },
	{ 2, "Picture of Offices", 2262, 15000 },
	{ 2, "Picture of City", 2258, 15000 },
	{ 2, "Pool Table", 2964, 350000 },
	{ 2, "Rounded Wooden Dining Table", 2111, 150000 },
	{ 2, "Long Dining Table", 2357, 190000 },
	{ 2, "Square Wooden Dining Table", 1433, 130000 },
	{ 2, "Small Wooden Dining Table", 1516, 100000 },
	{ 2, "Table and 3 Red Chairs", 1432, 200000 },
	{ 2, "Poker Table", 19474, 450000 },
	{ 2, "Green Table", 11690, 150000 },
	{ 2, "Double Green Table", 11691, 200000 },
	{ 2, "Red Twin Chair and Table", 2802, 200000 },
	{ 2, "Office Bookshelf", 2161, 80000 },
	{ 2, "Office Cabinet", 2167, 80000 },
	{ 2, "Photocopier/Printer", 2202, 220000 },
	{ 2, "Filing Cabinet", 2197, 60000 },
	{ 2, "Short Office Cabinet", 2163, 70000 },
	{ 2, "Rounded Office Desk", 2207, 350000 },
	{ 2, "Bookshelf", 1742, 100000 },
	{ 2, "Stack of Magazines", 2855, 15000 },
	{ 2, "Scattered Magazines", 2059, 15000 },
	{ 2, "Stack of Blue Poker Chips", 1880, 15000 },
	{ 2, "Stack of Green Poker Chips", 1902, 15000 },
	{ 2, "Stack of Yellow Poker Chips", 1878, 15000},
	{ 2, "Shoe Boxes", 2654, 15000 },
	{ 2, "Box of Beer", 19572, 15000 },
	{ 2, "Crate of Apples", 19636, 15000 },
	{ 2, "Pizza Box", 19571, 15000 },
	{ 2, "Toolbox", 19921, 15000 },
	{ 2, "Bottle of Juice", 19563, 15000 }
};
#define ELECTRONIC_COUNT 8
#define MAX_FURNITURE 150
#define FURNITURE_MENU 2994
enum furnInfo {
	f_SQLID, //sql id
	f_ID, //dictionary id
	f_spawned,
	f_owned,
	f_obj,
	f_vw,
	Float:f_X,
	Float:f_Y,
	Float:f_Z,
	Float:f_RX,
	Float:f_RY,
	Float:f_RZ
}
new Furniture[MAX_HOUSES][MAX_FURNITURE][furnInfo];

#define DIALOG_FURNITURE 2996
#define DIALOG_FURNITURE_SHOP 2998
CMD:placefurniture( playerid, params[] ) {
	if( PlayerInfo[playerid][pHouseID] == 0 ) 
		return SendClientMessage( playerid, COLOR_GRAD1, "You do not own a house." );
	new houseid = PlayerInfo[playerid][pHouseID];

	if( HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID] )
		return SendClientMessage( playerid, COLOR_GREY, "You don't own this house." );

	if( !IsPlayerInRangeOfPoint( playerid, 100.0, HouseInfo[houseid][hInsideX], HouseInfo[houseid][hInsideY], HouseInfo[houseid][hInsideZ] ) || GetPlayerVirtualWorld( playerid ) != HouseInfo[houseid][hSQLID] ) {
		return SendClientMessage(playerid, COLOR_GRAD1, "You are not inside your house.");
	}
	new furn_str[1500], count;
	for( new i; i < MAX_FURNITURE; i++ ) {
		if( !Furniture[houseid][i][f_spawned] ) {
			if( Furniture[houseid][i][f_owned] ) {
				if( count == 0 ) { 
					format( furn_str, 1500, "%s", FurnitureDict[Furniture[houseid][i][f_ID]][fD_name] );
				}
				else {
					format( furn_str, 1500, "%s\n%s", furn_str, FurnitureDict[Furniture[houseid][i][f_ID]][fD_name] );
				}
				count++;
			}
		}
	}
	ShowPlayerDialog( playerid, DIALOG_FURNITURE, DIALOG_STYLE_LIST, "Select a piece of furniture...", furn_str, "Place", "Cancel" );
	return 1;
}
new FurnitureEditID[MAX_PLAYERS] = -1;
furniture_edit( playerid, listitem ) {
	new Float: PosInfo[4], houseid = PlayerInfo[playerid][pHouseID], count, furnID;
	GetXYInFrontOfPlayer( playerid, PosInfo[0], PosInfo[1], PosInfo[2], PosInfo[3], 0.5 ); //X,Y,Z,Angle,Dist
	
	for( new i; i < MAX_FURNITURE; i++ ) {
		if( !Furniture[houseid][i][f_spawned] ) {
			if( Furniture[houseid][i][f_owned] ) {
				if( listitem == count ) {
					furnID = Furniture[houseid][i][f_ID];
					break;
				}
				count++;
			}
		}
	}
	#define furniture(%1) Furniture[houseid][furnID][f_%1]
	if( !furniture(spawned) ) {
		furniture(vw) = GetPlayerVirtualWorld( playerid );
		furniture(obj) = CreateDynamicObject( FurnitureDict[furnID][fD_model], PosInfo[0], PosInfo[1], PosInfo[2], 0.0, 0.0, 0.0, furniture(vw) );
		EditDynamicObject( playerid, furniture(obj) );
	}
	SendClientMessage( playerid, COLOR_GREY, "Type /storefurniture to remove the furniture from the room, or /removefurniture to delete it permanently." );
	FurnitureEditID[playerid] = furnID;
	#undef furniture
	return 1;
}
furniture_edit_response( playerid, objectid, response, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ ) {
	new houseid = PlayerInfo[playerid][pHouseID];
	for( new i; i < MAX_FURNITURE; i++ ) {
		if( objectid == Furniture[houseid][i][f_obj] ) {
			if( response == EDIT_RESPONSE_FINAL ) {
				#define furniture(%1) Furniture[houseid][i][f_%1]
				furniture(spawned) = 1;
				furniture(X) = fX;
				furniture(Y) = fY;
				furniture(Z) = fZ;
				furniture(RX) = fRotX;
				furniture(RY) = fRotY;
				furniture(RZ) = fRotZ;
				SetDynamicObjectPos( objectid, fX, fY, fZ );
				SetDynamicObjectRot( objectid, fRotX, fRotY, fRotZ );
				#undef furniture
			}
			else if( response == EDIT_RESPONSE_CANCEL ) { 
				new Float:oldPos[3], Float:oldRot[3];
				GetDynamicObjectPos( objectid, oldPos[0], oldPos[1], oldPos[2] );
				GetDynamicObjectRot( objectid, oldRot[0], oldRot[1], oldRot[2] ); 
				SetDynamicObjectPos( objectid, oldPos[0], oldPos[1], oldPos[2] );
				SetDynamicObjectRot( objectid, oldRot[0], oldRot[1], oldRot[2] );
			}
			return 1;
		}
	}
	return 0;
}
new BuyingFurnitureID[MAX_PLAYERS] = -1;
public OnPlayerModelSelectionEx(playerid, response, extraid, modelid) {
	if( extraid == FURNITURE_MENU ) {
		if( response ) {
			for( new i; i < sizeof( FurnitureDict ); i++ ) {
				if( modelid == FurnitureDict[i][fD_model] ) {
					BuyingFurnitureID[playerid] = i;
					break;
				}
			}
			if( BuyingFurnitureID[playerid] > -1 ) {
				new furnstr[200];
				format( furnstr, 200, "Are you sure you want to pay $%d for a %s?", FurnitureDict[BuyingFurnitureID[playerid]][fD_price], FurnitureDict[BuyingFurnitureID[playerid]][fD_name] );
				ShowPlayerDialog( playerid, DIALOG_FURNITURE_SHOP, DIALOG_STYLE_MSGBOX, "Confirmation", furnstr, "Buy", "Back" );
				return 1;
			}
		}
	}
	return 1;
}
//=============================[ Editing furniture commands: remove/delete ]=============================
CMD:storefurniture( playerid, params[] ) {
	if( FurnitureEditID[playerid] > -1 ) {
		new houseid = PlayerInfo[playerid][pHouseID], furnid = FurnitureEditID[playerid];
		DestroyDynamicObject( Furniture[houseid][furnid][f_obj] );
		Furniture[houseid][furnid][f_spawned] = 0;
		SendClientMessage( playerid, COLOR_GREY, "The furniture has been removed from the room." );
		FurnitureEditID[playerid] = -1;
	}
	else {
		return SendClientMessage( playerid, COLOR_GREY, "You are not editing/placing any furniture." );
	}
	return 1;
}
CMD:removefurniture( playerid, params[] ) {
	if( FurnitureEditID[playerid] > -1 ) {
		new houseid = PlayerInfo[playerid][pHouseID], furnid = FurnitureEditID[playerid];
		DestroyDynamicObject( Furniture[houseid][furnid][f_obj] );
		Furniture[houseid][furnid][f_owned] = 0;
		FurnitureEditID[playerid] = -1;
		SendClientMessage( playerid, COLOR_GREY, "You have permanently deleted this piece of furniture." );
	}
	else {
		SendClientMessage( playerid, COLOR_GREY, "You are not editing/placing any furniture." );
	}
	return 1;
}
#define DIALOG_EDIT_FURNITURE 2997
CMD:editfurniture( playerid, params[] ) {
	if( PlayerInfo[playerid][pHouseID] == 0 ) 
		return SendClientMessage( playerid, COLOR_GRAD1, "You do not own a house." );
	new houseid = PlayerInfo[playerid][pHouseID];

	if( HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID] )
		return SendClientMessage( playerid, COLOR_GREY, "You don't own this house." );

	if( !IsPlayerInRangeOfPoint( playerid, 100.0, HouseInfo[houseid][hInsideX], HouseInfo[houseid][hInsideY], HouseInfo[houseid][hInsideZ] ) || GetPlayerVirtualWorld( playerid ) != HouseInfo[houseid][hSQLID] ) {
		return SendClientMessage(playerid, COLOR_GRAD1, "You are not inside your house.");
	}
	new furn_str[400], count = 0;
	for( new i; i < MAX_FURNITURE; i++ ) {
		#define furniture(%1) Furniture[PlayerInfo[playerid][pHouseID]][i][f_%1]
		if( IsPlayerInRangeOfPoint( playerid, 3.0, furniture(X), furniture(Y), furniture(Z) ) ) {
			if( count == 0 ) {
				format( furn_str, 400, "%s", FurnitureDict[i][fD_name] );
			}
			else {
				format( furn_str, 400, "%s\n%s", furn_str, FurnitureDict[i][fD_name] );
			}
			count++;
		}
		#undef furniture
	}
	if( count == 0 ) {
		return SendClientMessage( playerid, COLOR_GREY, "You are not in range of any placed furniture that you own." );
	}
	ShowPlayerDialog( playerid, DIALOG_EDIT_FURNITURE, DIALOG_STYLE_LIST, "Select a piece of nearby furniture...", furn_str, "Edit", "Cancel" );
	return 1;
}
edit_placed_furniture( playerid, listitem ) {
	new houseid = PlayerInfo[playerid][pHouseID], count = 0;
	for( new i; i < MAX_FURNITURE; i++ ) { 
		#define furniture(%1) Furniture[houseid][i][f_%1]
		if( IsPlayerInRangeOfPoint( playerid, 3.0, furniture(X), furniture(Y), furniture(Z) ) ) {
			if( count == listitem ) {
				EditDynamicObject( playerid, furniture(obj) );
				FurnitureEditID[playerid] = i;
				SendClientMessage( playerid, COLOR_GREY, "/storefurniture to store this furniture, or /removefurniture to delete it permanently whilst editing it." );
			}
			count++;
		}
		#undef furniture
	}
	return 1;
}
//=============================[ Eletronic furniture commands ]==============================
new FoodTime[MAX_PLAYERS];
CMD:fridge( playerid, params[] ) {
	if( FoodTime[playerid] + 10 > gettime() ) {
		return SendClientMessage( playerid, COLOR_GREY, "You must wait ten seconds before using the fridge/microwave again." );
	}
	for( new i; i < MAX_HOUSES; i++ ) { 
		for( new f; f < MAX_FURNITURE; f++ ) {
			#define furniture(%1) Furniture[i][f][f_%1]
			if( furniture(ID) == 1 ) {
				if( furniture(spawned) ) {
					if( IsPlayerInRangeOfPoint( playerid, 2.0, furniture(X), furniture(Y), furniture(Z) ) ) {
						new foodtype[128];
						if( sscanf( params, "s[128]", foodtype ) ) {
							return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fridge [sprunk/beer]" );
						}
						if( strcmp( foodtype, "sprunk", true ) == 0 ) { 
							format( str, sizeof( str ), "* %s opens the fridge and grabs a sprunk.", PlayerICName(playerid) );
							ProxDetector( 30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE );
							FoodTime[playerid] = gettime();
							SetPlayerSpecialAction( playerid, SPECIAL_ACTION_DRINK_SPRUNK );
						}
						else if( strcmp( foodtype, "beer", true ) == 0 ) { 
							format( str, sizeof( str ), "* %s opens the fridge and grabs a beer.", PlayerICName( playerid ) );
							ProxDetector( 30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE );
							FoodTime[playerid] = gettime();
							SetPlayerSpecialAction( playerid, SPECIAL_ACTION_DRINK_BEER );
						}
					}
				}
			}
			#undef furniture
		}
	}
	return 1;
}
CMD:microwave( playerid, params[] ) {
	if( FoodTime[playerid] + 10 > gettime() ) { //limit usage to every 10secs for micro & fridge.
		return SendClientMessage( playerid, COLOR_GREY, "You must wait ten seconds before using the fridge/microwave again." );
	}
	for( new i; i < MAX_HOUSES; i++ ) {
		for( new f; f < MAX_FURNITURE; f++ ) {
			#define furniture(%1) Furniture[i][f][f_%1]
			if( furniture(ID) == 0 ) {
				if( furniture(spawned) ) {
					if( IsPlayerInRangeOfPoint( playerid, 2.0, furniture(X), furniture(Y), furniture(Z) ) ) {
						new foodtype[128], Float: health;
						if( sscanf( params, "s[128]", foodtype ) )
							return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /microwave [noodles/small pie]" );
						
						GetPlayerHealth( playerid, health );
						if( strcmp( foodtype, "noodles", true ) == 0 ) { //noodles
							format( str, sizeof( str ), "* %s opens the microwave and cooks some noodles", PlayerICName(playerid) );
							ProxDetector( 30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE );
							FoodTime[playerid] = gettime();
							if( health + 15 >= 100 ) {
								SetPlayerHealth( playerid, 100 );
							}
							else {
								SetPlayerHealth( playerid, health + 15 );
							}
						}
						if( strcmp( foodtype, "small pie", true ) == 0 ) { //small pie
							format( str, sizeof( str ), "* %s opens the microwave and cooks a small pie.", PlayerICName( playerid ) );
							ProxDetector( 30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE );
							FoodTime[playerid] = gettime();
							if( health + 20 >= 100 ) { 
								SetPlayerHealth( playerid, 100 );
							}
							else {
								SetPlayerHealth( playerid, health + 20 );
							}
						}
					}
				}
			}
			#undef furniture
		}
	}
	return 1;
}