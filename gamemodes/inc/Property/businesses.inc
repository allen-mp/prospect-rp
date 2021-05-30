#if defined BUSINESSES_INCLUDED
    #endinput
#endif

#define BUSINESSES_INCLUDED
#define DIALOG_BUZINES 5531
#include "..\gamemodes\inc\Property\bars.inc"

static str[144];
new TempBizID[ MAX_PLAYERS ][ MAX_BIZ ];
enum bInfo
{
	bName[64],
	bOwned,
	bOwner[MAX_PLAYER_NAME],
	Float:bOutsideX,
	Float:bOutsideY,
	Float:bOutsideZ,
	Float:bOutsideAng,
	bOutsideWorld,
	bOutsideInt,
	bOutsidePickup,
	Float:bInsideX,
	Float:bInsideY,
	Float:bInsideZ,
	Float:bInsideAng,
	bInsideInt,
	bInsidePickup,
	bEntryFee,
	bValue,
	bTill,
	bLocked,
	bType,
	bProducts,
	bSeized,
	bNoProdsDate[128],
	bDonated,
	bool:bBuyingProd,
	bProdAmount,
	bProdPrice,
	bTempPrice

};
new BizInfo[MAX_BIZ][bInfo];

CMD:buyprod( playerid,params[ ] )
{
	new ProdString[ 850 ];
	for( new i = 0; i < sizeof( BizInfo ); i++ )
	{
		if( IsPlayerInRangeOfPoint(playerid, 25.0, BizInfo[ i ] [ bInsideX ], BizInfo[ i ] [ bInsideY ], BizInfo[ i ] [ bInsideZ ] ) )
		{
			if( GetPlayerVirtualWorld( playerid ) == i )
			{
				new id = i;
				new amount, price;
				if( PlayerInfo[ playerid ] [ pBizKey ] != id ) SendClientMessage( playerid, -1, "You do not own this business!" );
				else if( sscanf( params, "ii", amount, price ) ) SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /buyprod <Amount> <Price>" );
				else
				{
					if( price > 25 || price < 20 ) SendClientMessage( playerid, COLOR_GREY, "Price must be between 20 and 25!" );
					else if( price > BizInfo[ i ] [ bTill ] ) 
					{	
						format( ProdString, sizeof( ProdString ), "Your biz till has insufficient funds (REQUIRED: $%d) (BIZ TILL: $%d)", price, BizInfo[ i ] [ bTill ] );
						SendClientMessage( playerid, COLOR_GREY, ProdString );
					}
					else
					{
						if( BizInfo[ i ] [ bBuyingProd ] == true ) SendClientMessage( playerid, COLOR_GREY, "You already requested products, please be patient." );
						else
						{
							BizInfo[ i ] [ bTempPrice ] = price;
							price = price*amount;
							format( ProdString, sizeof( ProdString ),	"You've requested %d materials and they will be bought for $%d", amount, price );
							SendClientMessage( playerid, COLOR_GREY, ProdString );
							BizInfo[ i ] [ bBuyingProd ] = true;
							BizInfo[ i ] [ bProdAmount ] = amount;
							BizInfo[ i ] [ bProdPrice ] = price;
							format( ProdString, sizeof( ProdString ), "%s has requested products for his business, type /bizproduct for more info.", PlayerOOCName( playerid ) );
							SendClientMessageToAll( COLOR_YELLOW, ProdString );
						}
					}
				}
				return 1;
			}
		}		
	}
	return 1;
}

CMD:sellbizmat( playerid, params[ ] )
{
	new amount;
	for( new i = 0; i < sizeof( BizInfo ); i++ )
    {
    	if( IsPlayerInRangeOfPoint(playerid, 25.0, BizInfo[ i ] [ bOutsideX ], BizInfo[ i ] [ bOutsideY ], BizInfo[ i ] [ bOutsideZ ] ) )
		{
			new id = i;
			if( BizInfo[ i ] [ bBuyingProd ] == false ) SendClientMessage( playerid, COLOR_GREY, "This business hasn't requested any materials!" );
			else if( PlayerInfo[ playerid ] [ pBizKey ] == id ) SendClientMessage( playerid, -1, "You're owner of this business, you can't sell products to yourself!" );
			else if( sscanf( params, "i", amount ) ) SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /sellbizmat <Amount>" );
			else
			{
				if( BizInfo[ i ] [ bProdAmount ] < amount ) SendClientMessage( playerid, COLOR_GREY, "This business does not ned that much materials!" );
				else
				{
					if( BizInfo[ i ] [ bTill ] < amount*BizInfo[ i ] [ bTempPrice ] ) SendClientMessage( playerid, COLOR_GREY, "This business has insufficient funds" );
					else
					{
						if( PlayerInfo[ playerid ] [ pMats ] < amount ) SendClientMessage( playerid, COLOR_GREY, "You do not have that much materials!" );
						else
						{
							BizInfo[ i ] [ bTill ] = BizInfo[ i ] [ bTill ]-amount*BizInfo[ i ] [ bTempPrice ];
							BizInfo[ i ] [ bProdAmount ] = BizInfo[ i ] [ bProdAmount ]-amount;
							PlayerInfo[ playerid ] [ pMats ] = PlayerInfo[ playerid ] [ pMats ]-amount;
							BizInfo[ i ] [ bProducts ] = BizInfo[ i ] [ bProducts ]+amount;
							BizInfo[ i ] [ bProdPrice ] = BizInfo[ i ] [ bProdPrice ]-amount*BizInfo[ i ] [ bTempPrice ];
							GiveMoney( playerid, amount*BizInfo[ i ] [ bTempPrice ] );
							if( BizInfo[ i ] [ bProdAmount ] < 1 ) BizInfo[ i ] [ bBuyingProd ] = false;
						}
					}
				}
			}
			return 1;
		}
    }
    return 1;
}
new dick;
CMD:bizproduct( playerid, params[ ] )
{
    new BizString[ 5000 ];
    for( new i = 0; i < sizeof( BizInfo ); i++ )
    {
        if( BizInfo[ i ] [ bBuyingProd ] == true )
        {
            format( BizString, sizeof( BizString ), "%s\nBusiness: %s | Owner: %s | Requesting: %d materials | Price: $%d\n", BizString, BizInfo[ i ] [ bName ], BizInfo[ i ] [ bOwner ], BizInfo[ i ] [ bProdAmount ], BizInfo[ i ] [ bProdPrice ] );
            ShowPlayerDialog(playerid, DIALOG_BUZINES, DIALOG_STYLE_LIST, "Business", BizString, "Track Business", "Close");
            TempBizID[ playerid ][ dick ] = i;
            dick++;

        }
    }
}  

/*Hook:biz_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_BUZINES)
    {
        if(response) 
        {
        	if(TempBizID[ playerid ][ dick ] == listitem)
        	{
	        	new string[ 500] ;
	            format( string, sizeof( string ), "Check your minimap, go there and sell your materials to that business." );
	            SendClientMessage( playerid, COLOR_GREY, string );
	            SetPlayerCheckpoint( playerid, BizInfo[ TempBizID[ playerid ][ dick ] ] [ bOutsideX ], BizInfo[ TempBizID[ playerid ][ dick ] ] [ bOutsideY ], BizInfo[ TempBizID[ playerid ][ dick ] ] [ bOutsideZ ], 5.0 );
	            CP[playerid] = CHECKPOINT_BIZSHIT;
       		}
        }
        return 1; 
    }
	return 1;
}*/

/*Hook:zib_OnPlayerEnterCheckpoint(playerid)
{
	new string[ 500 ];
	if(IsPlayerInRangeOfPoint( playerid, 5.0, BizInfo[ TempBizID[ playerid ][ dick ] ] [ bOutsideX ], BizInfo[ TempBizID[ playerid ][ dick ] ] [ bOutsideY ], BizInfo[ TempBizID[ playerid ][ dick ] ] [ bOutsideZ ] ) )
	{
		format( string, sizeof( string ), "Type /sellbizmat to sell your materials to this business!" );
		SendClientMessage( playerid, COLOR_GREY, string );
		DisablePlayerCheckpoint( playerid );
		CP[playerid] = 0;
		TempBizID[ playerid ][ dick ] = -1;
	}
    return 1;
}*/

Hook:zib_OnPlayerConnect(playerid)
{
	TempBizID[ playerid ][ dick ] = -1;
	return 1;
}

CMD:biztypes(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 99999 || PlayerInfo[playerid][pBizMod] == 1)
	{
		new ammucount, ffcount, itemcount, barcount, sexshopcount, clothescount;
		for(new i = 0; i < 50; i++)
		{
			switch(BizInfo[i][bType])
			{
				case 1:
					ammucount++;
				case 2:
					ffcount++;
				case 3:
					itemcount++;
				case 4:
					barcount++;
				case 5:
					sexshopcount++;
				case 6:
					clothescount++;
			}
		}
		format(str, sizeof(str), "Ammunations: %d, Fast-Food Restaurants: %d, 24/7s: %d, Bars & Clubs: %d, Sex Shops: %d, Clothing Shops: %d", ammucount, ffcount, itemcount, barcount, sexshopcount, clothescount);
		SendClientMessage(playerid, COLOR_YELLOW, str);
	}
	return 1;
}

CMD:bizinfo(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 99999 || PlayerInfo[playerid][pBizMod] == 1)
	{
		new biz;
		if(sscanf(params, "i", biz)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /bizinfo [id]");
		{
			format(str, sizeof(str), "Business ID: %d, Name: %s, Owner: %s, Till: $%d, Products: %d, Type: %d, Value: $%d, Seized: %d",
				   biz, BizInfo[biz][bName], BizInfo[biz][bOwner], BizInfo[biz][bTill], BizInfo[biz][bProducts], BizInfo[biz][bType], BizInfo[biz][bValue], BizInfo[biz][bSeized]);
			SendClientMessage(playerid, COLOR_YELLOW2, str);
		}
	}
	else return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this command.");
	return 1;
}

CMD:checkbizstock(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 99999 || PlayerInfo[playerid][pBizMod] == 1)
	{
		SearchBizProducts(playerid);
	}
	else
	{
		AdmErrorMsg;
		return 1;
	}
	return 1;
}

CMD:govbizinfo(playerid, params[])
{
	if(PlayerInfo[playerid][pMember] != 6) return SendClientMessage(playerid, COLOR_GREY, "You are not a part of the government");

	new biz = IsPlayerNearBiz(playerid);
	if(biz == -1) return SendClientMessage(playerid, COLOR_GREY, "You must be near the business to get the biz ID.");

	SendClientMessage(playerid, COLOR_GREY, "Available Types: 1: gunshop, 2: burgershot, 3: 247, 4: bar/club, 5: sexshop, 6: clothes store, 7: donut store");
	SendClientMessage(playerid, COLOR_GREY, "Available Types: 8: clucknbell, 9: pizzastack, 10: bank, 11: electronics store, 12: furniture store 13: fishing");
	if(BizInfo[biz][bSeized]) format(str, sizeof(str), "Name: %s, Owner: %s, Type: %d - SEIZED", BizInfo[biz][bName], BizInfo[biz][bOwner], BizInfo[biz][bType]);
	else format(str, sizeof(str), "Name: %s, Owner: %s, Type: %d - NOT SEIZED", BizInfo[biz][bName], BizInfo[biz][bOwner], BizInfo[biz][bType]);
		
	SendClientMessage(playerid, COLOR_GREY, str);
	return 1;
}

CMD:goventer(playerid, params[])
{
	if(PlayerInfo[playerid][pMember] != 6) return SendClientMessage(playerid, COLOR_GREY, "You are not a part of the government");

	new biz = IsPlayerNearBiz(playerid);
	if(biz == -1) return SendClientMessage(playerid, COLOR_GREY, "You are not by a business to enter.");
	SetPlayerVirtualWorld(playerid, biz);
	PlayerInfo[playerid][pVirtualWorld] = biz;
	SetPlayerInterior(playerid, BizInfo[biz][bInsideInt]);
	PlayerInfo[playerid][pInt] = BizInfo[biz][bInsideInt];
	SetPlayerPos(playerid, BizInfo[biz][bInsideX], BizInfo[biz][bInsideY], BizInfo[biz][bInsideZ]);
	SetPlayerFacingAngle(playerid, BizInfo[biz][bInsideAng]);
	SetCameraBehindPlayer(playerid);
	return 1;
}

//==============================================================================
stock IsAtClothShop(playerid)
{
	for(new i = 0; i < sizeof(BizInfo); i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 25.0, BizInfo[i][bInsideX], BizInfo[i][bInsideY], BizInfo[i][bInsideZ]))
		{
			if(GetPlayerVirtualWorld(playerid) == i)
			{
				if(BizInfo[i][bType] == 6)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

stock displayMapIcons(playerid)
{
	for(new i = 0; i < MAX_BIZ; i++)   //businesses
	{
		if(i != PlayerInfo[playerid][pBizKey])   //not the business i own
		{
			if(BizInfo[i][bType] == 1)   //ammunation
			{
				SetPlayerMapIcon(playerid, iconCount[playerid], BizInfo[i][bOutsideX], BizInfo[i][bOutsideY], BizInfo[i][bOutsideZ], 18, 0, MAPICON_LOCAL);
				iconCount[playerid]++;
			}
			else if(BizInfo[i][bType] == 2)	 //burgershot
			{
				SetPlayerMapIcon(playerid, iconCount[playerid], BizInfo[i][bOutsideX], BizInfo[i][bOutsideY], BizInfo[i][bOutsideZ], 10, 0, MAPICON_LOCAL);
				iconCount[playerid]++;
			}
			else if(BizInfo[i][bType] == 3)	 //247
			{
				SetPlayerMapIcon(playerid, iconCount[playerid], BizInfo[i][bOutsideX], BizInfo[i][bOutsideY], BizInfo[i][bOutsideZ], 17, 0, MAPICON_LOCAL);
				iconCount[playerid]++;
			}
			else if(BizInfo[i][bType] == 4)	 //club
			{
				SetPlayerMapIcon(playerid, iconCount[playerid], BizInfo[i][bOutsideX], BizInfo[i][bOutsideY], BizInfo[i][bOutsideZ], 49, 0, MAPICON_LOCAL);
				iconCount[playerid]++;
			}
			else if(BizInfo[i][bType] == 5)	 //sexshop
			{
				SetPlayerMapIcon(playerid, iconCount[playerid], BizInfo[i][bOutsideX], BizInfo[i][bOutsideY], BizInfo[i][bOutsideZ], 38, 0, MAPICON_LOCAL);
				iconCount[playerid]++;
			}
			else if(BizInfo[i][bType] == 6)	 //clothes store
			{
				SetPlayerMapIcon(playerid, iconCount[playerid], BizInfo[i][bOutsideX], BizInfo[i][bOutsideY], BizInfo[i][bOutsideZ], 45, 0, MAPICON_LOCAL);
				iconCount[playerid]++;
			}
			else if(BizInfo[i][bType] == 7)	 //donut store
			{
				SetPlayerMapIcon(playerid, iconCount[playerid], BizInfo[i][bOutsideX], BizInfo[i][bOutsideY], BizInfo[i][bOutsideZ], 50, 0, MAPICON_LOCAL);
				iconCount[playerid]++;
			}
			else if(BizInfo[i][bType] == 8)	 //clucknbell
			{
				SetPlayerMapIcon(playerid, iconCount[playerid], BizInfo[i][bOutsideX], BizInfo[i][bOutsideY], BizInfo[i][bOutsideZ], 14, 0, MAPICON_LOCAL);
				iconCount[playerid]++;
			}
			else if(BizInfo[i][bType] == 9)	 //pizza stack
			{
				SetPlayerMapIcon(playerid, iconCount[playerid], BizInfo[i][bOutsideX], BizInfo[i][bOutsideY], BizInfo[i][bOutsideZ], 29, 0, MAPICON_LOCAL);
				iconCount[playerid]++;
			}
			else if(BizInfo[i][bType] == 10)	 //bank
			{
				SetPlayerMapIcon(playerid, iconCount[playerid], BizInfo[i][bOutsideX], BizInfo[i][bOutsideY], BizInfo[i][bOutsideZ], 52, 0, MAPICON_LOCAL);
				iconCount[playerid]++;
			}
			else if(BizInfo[i][bType] == 11)	 //electronics store
			{
				SetPlayerMapIcon(playerid, iconCount[playerid], BizInfo[i][bOutsideX], BizInfo[i][bOutsideY], BizInfo[i][bOutsideZ], 38, 0, MAPICON_LOCAL);
				iconCount[playerid]++;
			}
			else if(BizInfo[i][bType] == 12)	 //electronics store
			{
				SetPlayerMapIcon(playerid, iconCount[playerid], BizInfo[i][bOutsideX], BizInfo[i][bOutsideY], BizInfo[i][bOutsideZ], 38, 0, MAPICON_LOCAL);
				iconCount[playerid]++;
			}
			else if(BizInfo[i][bType] == 13)	 //fishing store
			{
				SetPlayerMapIcon(playerid, iconCount[playerid], BizInfo[i][bOutsideX], BizInfo[i][bOutsideY], BizInfo[i][bOutsideZ], 8, 0, MAPICON_LOCAL);
				iconCount[playerid]++;
			}
			else	 //unknown biz type
			{
				SetPlayerMapIcon(playerid, iconCount[playerid], BizInfo[i][bOutsideX], BizInfo[i][bOutsideY], BizInfo[i][bOutsideZ], 37, 0, MAPICON_LOCAL);
				iconCount[playerid]++;
			}
		}
	}
	SetPlayerMapIcon(playerid, iconCount[playerid], 2143.5302, -2259.8565, 26.5776, 51, 0, MAPICON_LOCAL); //matfactory 01
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 2166.3771, -1675.383, 15.0859, 24, 0, MAPICON_LOCAL); //drughouse
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 2337.8855, -1185.4025, 35.5452, 16, 0, MAPICON_LOCAL); //crackhouse
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 411.8294, -2050.2601, -0.4745, 9, 0, MAPICON_LOCAL); //pier
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 385.6571, -283.4588, 2.5919, 9, 0, MAPICON_LOCAL); //pier
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 2329.4089, -2316.0997, 13.5468, 56, 0, MAPICON_LOCAL); //job mechanic
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 165.7221, -280.6324, 1.9747, 56, 0, MAPICON_LOCAL); //job garbage
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 2243.0278,-1715.1628,13.6365, 56, 0, MAPICON_LOCAL); //job bodyguard
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 1741.7062, -1863.6664, 13.5748, 56, 0, MAPICON_LOCAL); //job taxi (ls)
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], -1982.0994, 131.8526, 27.6875, 56, 0, MAPICON_LOCAL); //job taxi (sf)
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 1381.0413, -1088.8511, 27.3906, 56, 0, MAPICON_LOCAL); //job lawyer
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 2048.4106, -1409.1826, 75.0372, 22, 0, MAPICON_LOCAL); //hospital 01
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 1147.9742, -1344.0618, 49.2668, 22, 0, MAPICON_LOCAL); //hospital 02
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], -2714.6499, 210.9217, 4.3281, 56, 0, MAPICON_LOCAL); //mechanic (sf)
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], -2764.7473, 384.7114, 6.3281, 56, 0, MAPICON_LOCAL); //lawyer (sf)
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], -1551.8896, 1154.8134, 7.1875, 56, 0, MAPICON_LOCAL); //bodyguard (sf)
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 859.6895, -590.0837, 17.5327, 22, 0, MAPICON_LOCAL); //hospital 03
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 1498.4185, -2182.5278, 14.1682, 20, 0, MAPICON_LOCAL); //fire department
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 1318.8472, 392.6770, 19.5547, 11, 0, MAPICON_LOCAL); //drug factory
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 612.8910, -564.9972, 26.1432, 30, 0, MAPICON_LOCAL); //PD
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 972.0325, -1385.7533, 13.5476, 56, 0, MAPICON_LOCAL); //job product dealer
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 1244.7333, 205.3294, 19.6454, 50, 0, MAPICON_LOCAL); //montgomery cafe
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 1564.5457, -1675.3245, 61.8977, 30, 0, MAPICON_LOCAL); //SAPD
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 1461.8095, -1010.9515, 61.1702, 52, 0, MAPICON_LOCAL); //bank
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 1025.05, -1024.23, 32.1, 63, 0, MAPICON_LOCAL); //payandspray
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 487.68, -1740.87, 11.13, 63, 0, MAPICON_LOCAL); //payandspray
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], -1420.73, 2583.37, 55.56, 63, 0, MAPICON_LOCAL); //payandspray
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], -1904.39, 284.97, 40.75, 63, 0, MAPICON_LOCAL); //payandspray
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], -2425.91, 1022.33, 50.10, 63, 0, MAPICON_LOCAL); //payandspray
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 1975.60, 2162.16, 10.77, 63, 0, MAPICON_LOCAL); //payandspray
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 2065.38, -1831.51, 13.25, 63, 0, MAPICON_LOCAL); //payandspray
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], -99.55, 1118.36, 19.44, 63, 0, MAPICON_LOCAL); //payandspray
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 721.07, -455.94, 16.04, 63, 0, MAPICON_LOCAL); //payandspray
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 2393.74, 1493.01, 10.52, 63, 0, MAPICON_LOCAL); //payandspray
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 2288.1233,-1104.7793,38.4565, 51, 0, MAPICON_LOCAL); //matfactory dillimore
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 249.8837, -56.2413, 5.8828, 49, 0, MAPICON_LOCAL); //liquor store in blueberry
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 1995.8113, -1115.6873, 26.7737, 59, 0, MAPICON_LOCAL); //ballas joingang icon
	iconCount[playerid]++;
	SetPlayerMapIcon(playerid, iconCount[playerid], 2521.3630, -1678.5033, 15.3839, 62, 0, MAPICON_LOCAL); //grove joingang icon
	iconCount[playerid]++;
	//SetPlayerMapIcon(playerid, iconCount[playerid], 1015.9748, -311.7639, 74.5135, 56, 0, MAPICON_LOCAL); //job miner
	//iconCount[playerid]++;
	if(PlayerInfo[playerid][pBizKey] != -1)   //biz owner
	{
		new icontype;
		new pbizid = PlayerInfo[playerid][pBizKey];
		switch(BizInfo[pbizid][bType])
		{
			case 1:
				icontype = 18;
			case 2:
				icontype = 10;
			case 3:
				icontype = 17;
			case 4:
				icontype = 49;
			case 5:
				icontype = 38;
			case 6:
				icontype = 45;
			case 7:
				icontype = 50;
			case 8:
				icontype = 14;
			case 9:
				icontype = 29;
			case 10:
				icontype = 52;
			case 11:
				icontype = 38;
			case 12:
				icontype = 11;
			case 13:
				icontype = 37;
		}
		new bizid = PlayerInfo[playerid][pBizKey];
		SetPlayerMapIcon(playerid, iconCount[playerid], BizInfo[bizid][bOutsideX], BizInfo[bizid][bOutsideY], BizInfo[bizid][bOutsideZ], icontype, 0, MAPICON_GLOBAL);
		bizIconID[playerid] = iconCount[playerid];
		iconCount[playerid]++;
	}
	//printf("map icons created: %d", iconCount[playerid]);
}
SaveBizProductDates()
{
	new file[20], idx, string[64];
	file = "bizz-products.ini";
	if(dini_Exists(file))
	{
		while(idx < sizeof(BizInfo))
		{
			format(string, sizeof(string), "biz%d", idx);
			dini_Set("bizz-products.ini", string, BizInfo[idx][bNoProdsDate]);
			idx++;
		}
		return 1;
	}
	dini_Create(file);
	while(idx < sizeof(BizInfo))
	{
		format(string, sizeof(string), "biz%d", idx);
		dini_Set("bizz-products.ini", string, BizInfo[idx][bNoProdsDate]);
		idx++;
	}
	return 1;
}

LoadBizProductDates()
{
	new file[20], idx;
	file = "bizz-products.ini";
	if(dini_Exists(file))
	{
		while(idx < sizeof(BizInfo))
		{
			new string[64];
			format(string, sizeof(string), "biz%d", idx);
			strmid(BizInfo[idx][bNoProdsDate], dini_Get(file, string), false, strlen(dini_Get(file, string)), 64);
			if(strmatch(BizInfo[idx][bNoProdsDate], ""))
			{
				SetNoProductsDate(idx);
			}
			idx++;
		}
		return 1;
	}
	while(idx < sizeof(BizInfo)) // if there's no file set all the dates
	{
		SetNoProductsDate(idx);
	}
	return 1;
}

//==============================================================================
SaveBusinesses()
{
	SaveBizProductDates();
	new idx;
	new File:file;
	file = fopen("bizz.cfg", io_write);
	fclose(file);
	file = fopen("bizz.cfg", io_append);
	new coordsstring[512];
	if(file) {
		while(idx < sizeof(BizInfo))
		{
			format(coordsstring, sizeof(coordsstring), "%s|%d|%s|%f|%f|%f|%f|%d|%d|%f|%f|%f|%f|%d|%d|%d|%d|%d|%d|%d|%d\n",
				   BizInfo[idx][bName],
				   BizInfo[idx][bOwned],
				   BizInfo[idx][bOwner],
				   BizInfo[idx][bOutsideX],
				   BizInfo[idx][bOutsideY],
				   BizInfo[idx][bOutsideZ],
				   BizInfo[idx][bOutsideAng],
				   BizInfo[idx][bOutsideWorld],
				   BizInfo[idx][bOutsideInt],
				   BizInfo[idx][bInsideX],
				   BizInfo[idx][bInsideY],
				   BizInfo[idx][bInsideZ],
				   BizInfo[idx][bInsideAng],
				   BizInfo[idx][bInsideInt],
				   BizInfo[idx][bEntryFee],
				   BizInfo[idx][bValue],
				   BizInfo[idx][bTill],
				   BizInfo[idx][bLocked],
				   BizInfo[idx][bType],
				   BizInfo[idx][bProducts],
				   BizInfo[idx][bSeized]);
			fwrite(file, coordsstring);
			idx++;
		}
		fclose(file);
	} else {
		printf("[error] Failed to find bizz.cfg");
	}
	return 1;
}

//==============================================================================
LoadBusinesses()
{
	LoadBizProductDates();
	new arrCoords[21][64];
	new strFromFile2[270];
	new File: file = fopen("bizz.cfg", io_read);
	if(file)
	{
		new idx;
		while(idx < sizeof(BizInfo))
		{
			fread(file, strFromFile2);
			split(strFromFile2, arrCoords, '|');
			strmid(BizInfo[idx][bName], arrCoords[0], 0, strlen(arrCoords[0]), 255);
			BizInfo[idx][bOwned] = strval(arrCoords[1]);
			strmid(BizInfo[idx][bOwner], arrCoords[2], 0, strlen(arrCoords[2]), 255);
			BizInfo[idx][bOutsideX] = floatstr(arrCoords[3]);
			BizInfo[idx][bOutsideY] = floatstr(arrCoords[4]);
			BizInfo[idx][bOutsideZ] = floatstr(arrCoords[5]);
			BizInfo[idx][bOutsideAng] = floatstr(arrCoords[6]);
			BizInfo[idx][bOutsideWorld] = strval(arrCoords[7]);
			BizInfo[idx][bOutsideInt] = strval(arrCoords[8]);
			BizInfo[idx][bInsideX] = floatstr(arrCoords[9]);
			BizInfo[idx][bInsideY] = floatstr(arrCoords[10]);
			BizInfo[idx][bInsideZ] = floatstr(arrCoords[11]);
			BizInfo[idx][bInsideAng] = floatstr(arrCoords[12]);
			BizInfo[idx][bInsideInt] = strval(arrCoords[13]);
			BizInfo[idx][bEntryFee] = strval(arrCoords[14]);
			BizInfo[idx][bValue] = strval(arrCoords[15]);
			BizInfo[idx][bTill] = strval(arrCoords[16]);
			BizInfo[idx][bLocked] = strval(arrCoords[17]);
			BizInfo[idx][bType] = strval(arrCoords[18]);
			BizInfo[idx][bProducts] = strval(arrCoords[19]);
			BizInfo[idx][bSeized] = strval(arrCoords[20]);
			//load pickups
			if(BizInfo[idx][bOutsidePickup]) DestroyDynamicPickup(BizInfo[idx][bOutsidePickup]);
			if(BizInfo[idx][bInsidePickup]) DestroyDynamicPickup(BizInfo[idx][bInsidePickup]);
			BizInfo[idx][bOutsidePickup] = CreateDynamicPickup(1272, 23, BizInfo[idx][bOutsideX], BizInfo[idx][bOutsideY], BizInfo[idx][bOutsideZ]);
			BizInfo[idx][bInsidePickup] = CreateDynamicPickup(1272, 23, BizInfo[idx][bInsideX], BizInfo[idx][bInsideY], BizInfo[idx][bInsideZ], idx);
			//load 3dtext
			if(BizText[idx]) DestroyDynamic3DTextLabel(BizText[idx]);
			if(BizInfo[idx][bOwned] == 0)
			{
				new pricestring[32];
				new price = BizInfo[idx][bValue];
				new j = valstr(pricestring, price);
				while(j >= 4)
				{
					j -= 3;
					strins(pricestring, ",", j);
				}
				strins(pricestring, "$", 0);
				format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nStatus: For Sale\nPrice: %s", BizInfo[idx][bName], pricestring);
			}
			else if(BizInfo[idx][bOwned] == 1)
			{
				// Government seized
				if(BizInfo[idx][bSeized] == 1)
				{
					format(str, sizeof(str), "{AFAFAF}%s{AFAFAF}\n{FFFFFF}This business has been seized by the\n{696969}Government", BizInfo[idx][bName]);
				}
				// Judicial Department seized
				else if(BizInfo[idx][bSeized] == 2) {
					format(str, sizeof(str), "{AFAFAF}%s{AFAFAF}\n{FFFFFF}This business has been seized by the\n{BA6103}Judicial Department", BizInfo[idx][bName]);
				}
				else
				{
					if(BizInfo[idx][bLocked] == 1)
					{
						format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s", BizInfo[idx][bName], BizInfo[idx][bOwner]);
					}
					else
					{
						format(str, sizeof(str), "{3D97E0}%s{FFFFFF}\nOwner: %s", BizInfo[idx][bName], BizInfo[idx][bOwner]);
					}
				}
			}
			BizText[idx] = CreateDynamic3DTextLabel(str, COLOR_BIZTEXT, BizInfo[idx][bOutsideX], BizInfo[idx][bOutsideY], BizInfo[idx][bOutsideZ] +0.88, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
			idx++;
			bizcount++;
		}
		fclose(file);
	} else {
		printf("[error] Failed to find bizz.cfg");
	}
	return 1;
}

CMD:getdrink(playerid, params[])
{
	for(new i = 0; i < sizeof(BizInfo); i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 25.0, BizInfo[i][bInsideX], BizInfo[i][bInsideY], BizInfo[i][bInsideZ]))
		{
			if(GetPlayerVirtualWorld(playerid) == i)
			{
				if(BizInfo[i][bType] == 4) //bar
				{
					if(BizInfo[i][bProducts] > 0)
					{
						ShowPlayerDialog(playerid, DIALOG_BAR_DYN, DIALOG_STYLE_LIST, "Bar Dialog", "Beer\nVodka\nWhiskey\nWine\nSprunk", "Ok", "Cancel");
					}
				}
			}
		}
	}
	new count = 0;
	for(new i = 0; i < MAX_BARS; i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 3, DynBars[i][BarX], DynBars[i][BarY], DynBars[i][BarZ]))
		{
			count++;
		}
	}
	if(count == 0) return SendClientMessage(playerid, COLOR_GREY, "You're not near a bar.");
	else
	{
		ShowPlayerDialog(playerid, DIALOG_BAR_DYN, DIALOG_STYLE_LIST, "Bar Dialog", "Beer\nVodka\nWhiskey\nWine\nSprunk", "Ok", "Cancel");
	}
	return 1;
}

// Command to buy clothing in a clothes store
CMD:buyclothes(playerid, params[])
{
	for(new i = 0; i < sizeof(BizInfo); i++)
	{
		if(GetPlayerVirtualWorld(playerid) != i)
			continue;

		if(BizInfo[i][bType] != 6)
			continue;

		if(!IsPlayerInRangeOfPoint(playerid, 25.0, BizInfo[i][bInsideX], BizInfo[i][bInsideY], BizInfo[i][bInsideZ]))
			continue;

		if(BizInfo[i][bProducts] <= 0)
			return SendClientMessage(playerid, COLOR_GREY, "This business doesn't have any products left.");
		
		ShowSkinShop(playerid);
		return 1;
	}

	SendClientMessage(playerid, COLOR_GREY, "You are not in a clothing store.");
	return 1;
}

CMD:buygear(playerid, params[]) {
	for(new i = 0; i < sizeof(BizInfo); i++)
	{
		if(IsPlayerInRangeOfPoint(playerid,25.0,BizInfo[i][bInsideX],BizInfo[i][bInsideY],BizInfo[i][bInsideZ]))
		{
			if(GetPlayerVirtualWorld(playerid) == i)
			{
				if(BizInfo[i][bType] == 6)
				{
					if(BizInfo[i][bProducts] <= 0)
					{
						SendClientMessage(playerid, COLOR_GREY, "This business doesn't have any products left.");
						return 1;
					}
					ShowPlayerDialog(playerid, DIALOG_BUYACCESSOR_MAIN, DIALOG_STYLE_LIST, "Gear", "Clothing Items 1\nClothing Items 2\nToys 1\nToys 2\nSpecial", "Select", "Cancel");
					return 1;
				} else {
					return SendClientMessage(playerid, COLOR_GREY, "You are not in a Clothing Store !");
				}
			}
		}
	}
	return 1;
}

CMD:buy(playerid, params[])
{
	for(new i = 0; i < sizeof(BizInfo); i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 25.0, BizInfo[i][bInsideX], BizInfo[i][bInsideY], BizInfo[i][bInsideZ]))
		{
			if(GetPlayerVirtualWorld(playerid) == i)
			{
				switch(BizInfo[i][bType])
				{
					case 2: // Fast-Food
					{
						if(BizInfo[i][bProducts] <= 0) return SendClientMessage(playerid, COLOR_GREY, "This business doesn't have any products left.");
						DisplayDialogForPlayer(playerid, 70); //burger shot
					}
					case 3: // 24-7
					{
						if(BizInfo[i][bProducts] <= 0) return SendClientMessage(playerid, COLOR_GREY, "This business doesn't have any products left.");
						new finalstring[5000];
						format(finalstring, sizeof(finalstring), "Item Name\tPrice\n\
																 Cellphone\t\t{4a8e4a}$2,500\n\
																 Phonebook\t\t{4a8e4a}$5,000\n\
																 Dice\t\t\t{4a8e4a}$500\n\
																 iPod\t\t\t{4a8e4a}$4,500\n\
																 Spray Can\t\t{4a8e4a}$2,000\n\
																 Ropes\t\t\t{4a8e4a}$2,000\n\
																 Cigars\t\t\t{4a8e4a}$500\n\
																 Sprunk\t\t\t{4a8e4a}$500\n\
																 Walkie Talkie\t\t{4a8e4a}$1,000\n\
																 Screw Driver\t\t{4a8e4a}$1,000\n\
																 Blindfolds\t\t{4a8e4a}$4,500\n\
																 Cards\t\t\t{4a8e4a}$2,000\n\
																 Address Book\t\t{4a8e4a}$5,000\n\
																 Calculator\t\t{4a8e4a}$1,250\n\
																 Boombox\t\t{4a8e4a}$6,000\n\
																 Map\t\t\t{4a8e4a}$1,000\n\
																 Box of Matches\t{4a8e4a}$850\n\
																 Jerry Can\t\t{4a8e4a}$1,000\n\
																 Case of beer\t\t{4a8e4a}$2,000\n\
																 Drill\t\t\t{4a8e4a}$250,000\n\
																 Duffle Bag\t\t{4a8e4a}$10,000\n\
																 Briefcase\t\t{4a8e4a}$10,000");

						ShowPlayerDialog(playerid, DIALOG_BUSSINESS_BUY, DIALOG_STYLE_TABLIST_HEADERS, "24/7", finalstring, "Buy", "Cancel");
					}
					case 5: // Sex Shop
					{
						if(BizInfo[i][bProducts] <= 0) return SendClientMessage(playerid, COLOR_GREY, "This business doesn't have any products left.");
						DisplayDialogForPlayer(playerid, 76);
					}
					case 7: // Donut Store
					{
						if(BizInfo[i][bProducts] <= 0) return SendClientMessage(playerid, COLOR_GREY, "This business doesn't have any products left.");
						DisplayDialogForPlayer(playerid, 107);
					}
					case 8: // Cluck n Bell
					{
						if(BizInfo[i][bProducts] <= 0) return SendClientMessage(playerid, COLOR_GREY, "This business doesn't have any products left.");
						DisplayDialogForPlayer(playerid, 71);
					}
					case 9: // Pizza Stack
					{
						if(BizInfo[i][bProducts] <= 0) return SendClientMessage(playerid, COLOR_GREY, "This business doesn't have any products left.");
						DisplayDialogForPlayer(playerid, 72);
					}
					case 13: //Fishing
					{
						if(BizInfo[i][bProducts] <= 0) return SendClientMessage(playerid, COLOR_GREY, "This business doesn't have any products left.");
						DisplayDialogForPlayer(playerid, 102);
					}
					default:
						SendClientMessage(playerid, COLOR_GREY, "You are not in a business that can sell products.");
				}
			}
		}
	}
	return 1;
}

stock SetNoProductsDate(biz)
{
	new string[64];
	new year, month, day;
	getdate(year, month, day);
	format(string, sizeof(string), "%d/%d/%d", day, month, year);
	strmid(BizInfo[biz][bNoProdsDate], string, 0, strlen(string), 255);
	SaveBizProductDates();
	return 1;
}

CMD:buygun(playerid, params[])
{
	for(new i = 0; i < sizeof(BizInfo); i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 25.0, BizInfo[i][bInsideX], BizInfo[i][bInsideY], BizInfo[i][bInsideZ]))
		{
			if(GetPlayerVirtualWorld(playerid) == i)
			{
				if(BizInfo[i][bType] == 1) //gunshop
				{
					if(BizInfo[i][bProducts] > 0)
					{
						new x_info[24], wstring[128];
						if(sscanf(params, "s[24]", x_info))
						{
							if(PlayerInfo[playerid][pWeaponLicense] != 1)
							{
								format(wstring, sizeof(wstring), "{FF0000}%s:", BizInfo[i][bName]);
								SendClientMessage(playerid, COLOR_WHITE, wstring);
								SendClientMessage(playerid, COLOR_GREY, "Vest ($12,000)");	
							}
							else
							{
								format(wstring, sizeof(wstring), "{FF0000}%s:", BizInfo[i][bName]);
								SendClientMessage(playerid, COLOR_WHITE, wstring);
								SendClientMessage(playerid, COLOR_GREY, "BrassKnuckles ($500), Katana ($1,000), Bat ($500)");
								SendClientMessage(playerid, COLOR_GREY, "Colt45 ($3,500), Deagle ($80,000) Shotgun ($12,000), Rifle ($10,000) Vest ($12,000)");
							}
							return 1;
						}
						else
						{
							if(strcmp(x_info, "colt45", true) == 0)
							{
								if(PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GREY, "You must have 2 playing hours to buy firearms from ammunation.");
								if(PlayerInfo[playerid][pWeaponLicense] != 1) return SendClientMessage(playerid, COLOR_GRAD2, "You do not have a weapons license.");
								if(GetMoney(playerid) >= 3500)
								{
									GivePlayerGun(playerid, 22);
									GiveMoney(playerid, -3500);
									SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have bought a Colt45 from the gunshop.");
									BizInfo[i][bProducts] --;
									BizInfo[i][bTill] += 7000;
									SaveBusinesses();
									format(str, sizeof(str), "* %s hands the clerk some money and receives their weapon.", PlayerICName(playerid));
									ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

								}
								else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money.");
							}
							if(strcmp(x_info, "deagle", true) == 0)
							{
								if(PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GREY, "You must have 2 playing hours to buy firearms from ammunation.");
								if(PlayerInfo[playerid][pWeaponLicense] != 1) return SendClientMessage(playerid, COLOR_GRAD2, "You do not have a weapons license.");
								if(GetMoney(playerid) >= 80000)
								{
									GivePlayerGun(playerid, 24);
									GiveMoney(playerid, -80000);
									SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have bought a Deagle from the gunshop.");
									BizInfo[i][bProducts] --;
									BizInfo[i][bTill] += 120000;
									SaveBusinesses();
									format(str, sizeof(str), "* %s hands the clerk some money and receives their weapon.", PlayerICName(playerid));
									ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

								}
								else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money.");
							}
							else if(strcmp(x_info, "shotgun", true) == 0)
							{
								if(PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GREY, "You must have 2 playing hours to buy firearms from ammunation.");
								if(PlayerInfo[playerid][pWeaponLicense] != 1) return SendClientMessage(playerid, COLOR_GRAD2, "You do not have a weapons license.");
								if(GetMoney(playerid) >= 12000)
								{
									GivePlayerGun(playerid, 25);
									GiveMoney(playerid, -12000);
									SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have bought a Shotgun from the gunshop.");
									BizInfo[i][bProducts] --;
									BizInfo[i][bTill] += 24000;
									SaveBusinesses();
									format(str, sizeof(str), "* %s hands the clerk some money and receives their weapon.", PlayerICName(playerid));
									ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
								}
								else
								{
									SendClientMessage(playerid, COLOR_GREY, "You don't have enough money.");
									return 1;
								}
							}
							if(strcmp(x_info, "rifle", true) == 0)
							{
								if(PlayerInfo[playerid][pConnectTime] < 2) return SendClientMessage(playerid, COLOR_GREY, "You must have 2 playing hours to buy firearms from ammunation.");
								if(PlayerInfo[playerid][pWeaponLicense] == 1)
								{
									if(GetMoney(playerid) >= 10000)
									{
										GiveMoney(playerid, -10000);
										GivePlayerGun(playerid, 33);
										SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have bought a Country Rifle from the gunshop.");
										BizInfo[i][bProducts] --;
										BizInfo[i][bTill] += 20000;
										SaveBusinesses();
										format(str, sizeof(str), "* %s hands the clerk some money and receives their rifle.", PlayerICName(playerid));
										ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
									}
									else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money.");
								}
								else return SendClientMessage(playerid, COLOR_GREY, "You must have a weapon license in order to purchase this.");
							}
							else if(strcmp(x_info, "BrassKnuckles", true) == 0)
							{
								if(PlayerInfo[playerid][pWeaponLicense] != 1) return SendClientMessage(playerid, COLOR_GRAD2, "You do not have a weapons license.");
								if(GetMoney(playerid) >= 500)
								{
									GiveMoney(playerid, -500);
									GivePlayerGun(playerid, 1);
									SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have bought some Brass Knuckles from the gunshop.");
									BizInfo[i][bProducts] --;
									BizInfo[i][bTill] += 1000; // let's let the biz owners at least break even on this
									SaveBusinesses();
									format(str, sizeof(str), "* %s hands the clerk some money and receives their Brass Knuckles.", PlayerICName(playerid));
									ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
								}
								else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money.");
							}
							else if(strcmp(x_info, "katana", true) == 0)
							{
								if(PlayerInfo[playerid][pWeaponLicense] != 1) return SendClientMessage(playerid, COLOR_GRAD2, "You do not have a weapons license.");
								if(GetMoney(playerid) >= 1000)
								{
									GiveMoney(playerid, -1000);
									GivePlayerGun(playerid, 8);
									SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have bought a Katana from the gunshop.");
									BizInfo[i][bProducts] --;
									BizInfo[i][bTill] += 2000;
									SaveBusinesses();
									format(str, sizeof(str), "* %s hands the clerk some money and receives their Katana.", PlayerICName(playerid));
									ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
								}
								else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money.");
							}
							else if(strcmp(x_info, "bat", true) == 0)
							{
								if(PlayerInfo[playerid][pWeaponLicense] != 1) return SendClientMessage(playerid, COLOR_GRAD2, "You do not have a weapons license.");
								if(GetMoney(playerid) >= 500)
								{
									GiveMoney(playerid, -500);
									GivePlayerGun(playerid, 5);
									SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have bought a Baseball Bat from the gunshop.");
									BizInfo[i][bProducts] --;
									BizInfo[i][bTill] += 1000; // let's let the biz owners at least break even on this
									SaveBusinesses();
									format(str, sizeof(str), "* %s hands the clerk some money and receives their Baseball Bat.", PlayerICName(playerid));
									ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
								}
								else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money.");
							}
							else if(strcmp(x_info, "vest", true) == 0)
							{
								if(GetMoney(playerid) >= 12000)
								{
									SetPlayerArmour(playerid, 50);
									GiveMoney(playerid, -12000);
									SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have bought a Kevlar Vest from the gunshop.");
									BizInfo[i][bProducts] --;
									BizInfo[i][bTill] += 24000;
									SaveBusinesses();
									format(str, sizeof(str), "* %s hands the clerk some money and receives their kevlar vest.", PlayerICName(playerid));
									ProxDetector(30.0, playerid, str, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
								}
								else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money.");
							}
							if(BizInfo[i][bProducts] <= 0) SetNoProductsDate(i);
						}
					}
					else
					{
						SendClientMessage(playerid, COLOR_GREY, "This business doesn't have any products left.");
						return 1;
					}
				}
				return 1;
			}
		}
	}
	return 1;
}

CMD:donated(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 1337 && !PlayerInfo[playerid][pBizMod]) return AdmErrorMsg;
	new bizid;
	if(sscanf(params, "i", bizid)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /setbizdonated [bizID]");
	if(!BizInfo[bizid][bOwned]) return SendClientMessage(playerid, COLOR_GREY, "You cannot edit this business as it is not owned by someone.");

	BizInfo[bizid][bDonated] = !BizInfo[bizid][bDonated];

	new msg[100];
	if(BizInfo[bizid][bDonated])
	{
		format(msg, sizeof(msg), "Business ID %d is now donated and can not be sold.", bizid);
		SendClientMessage(playerid, COLOR_GREY, msg);
	}
	else
	{
		format(msg, sizeof(msg), "Business ID %d is no longer donated and can be sold.", bizid);
		SendClientMessage(playerid, COLOR_GREY, msg);
	}
	return 1;
}

IsPlayerNearBiz(playerid)
{
	for(new i = 0; i < MAX_BIZ; i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 1.0, BizInfo[i][bOutsideX], BizInfo[i][bOutsideY], BizInfo[i][bOutsideZ]))
		{
			return i;
		}
	}
	return -1;
}
