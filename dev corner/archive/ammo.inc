/*
	Limited ammo by Jacob,
		Functions: 	IsValidWeapon( playerid, weaponid )		- returns GUN_TYPE_NORMAL, GUN_TYPE_ADMIN, GUN_TYPE_HACKED.
					GetWeaponIndex( weaponid ) 		- returns the index that corresponds to the weapon dictionary & g_pGunInfo (player variable).
					ReloadPlayerWeapon( playerid ) 		- reloads the player's current weapon.
					UpdateAmmoText( playerid, weaponid ) 		- update the magazine textdraw for a specific player's weapon.
					GivePlayerClips( playerid, weaponid, clips ) 		- give a player x amount of clips for weaponid.
					GivePlayerGun( playerid, weaponid, ammo = -1, save = true ) 	- give an IC gun of weaponid, save refers to: save the gun on relog.
					GiveAdminWeapon( playerid, weaponid ) 		- give the player an OOC weapon, of weaponid (infinte ammo).
					TakePlayerWeapon( playerid, weaponid, del = true ) 	- take a weapon from the player. del = false for temp.
					RemoveAdminGuns( playerid )		- remove all player's OOC guns.
					
*/
//==========================[ Limited ammo header ]=============================
#define DIALOG_BUY_WEAPON 3394
#define DIALOG_GUNSTORE 3395

#define GUN_TYPE_NORMAL 1
#define GUN_TYPE_ADMIN 2
#define GUN_TYPE_HACKED 0

new iCurrentWeapon[MAX_PLAYERS];

new onelessBullet[MAX_PLAYERS];
new PlayerText: ammo_text[MAX_PLAYERS];
new AdminGuns[MAX_PLAYERS][12];
new weapCheat[MAX_PLAYERS];

enum w_Info //reason not linked: reduced array: less processing
{
	wo_Gun,
	wo_Ammo,
	wo_Mags
}
enum e_gunInfo
{
	w_Name[30],
	w_maxAmmo,
	w_storePrice,
	w_Store, //buyable at store
	w_ID,
	w_Slot,
	w_AnimLib[20],
	w_AnimName[20],
	w_dammo,
	Float:w_matsbullet,
	w_bull_level,
	w_mags_bullets,
	w_max_mags
}

//This is a dictionary of weapons.
new g_weapInfo[][e_gunInfo] =
{ // Name, max ammo, store price, in store, weapon ID, slot, default ammo, mats per bullet, ammo per clip
	{"Fist", 0, 0, 0, 0, 0, "", "", 0, 0.0, 0, 0, 0},
	{"Brass Knuckles", 1, 0, 0, 1, 0, "", "", 1, 0.0, 0, 0, 0},
	{"Golf Club", 1, 0, 0, 2, 1, "", "", 1, 0.0, 0, 0, 0},
	{"Nightstick", 1, 0, 0, 3, 1, "", "", 1, 0.0, 0, 0, 0},
	{"Knife", 1, 0, 0, 4, 1, "", "", 1, 0.0, 0, 0, 0},
	{"Baseball Bat", 1, 0, 0, 5, 1, "", "", 1, 0.0, 0, 0, 0},
	{"Shovel", 1, 0, 0, 6, 1, "", "", 1, 0.0, 0, 0, 0},
	{"Pool Cue", 1, 0, 0, 7, 1, "", "", 1, 0.0, 0, 0, 0},
	{"Katana", 1, 0, 0, 8, 1, "", "", 1, 0.0, 0, 0, 0},
	{"Chainsaw", 1, 0, 0, 9, 1, "", "", 1, 0.0, 0, 0, 0},
	{"Purple Dildo", 1, 0, 0, 10, 10, "", "", 1, 0.0, 0, 0, 0},
	{"Dildo", 1, 0, 0, 11, 10, "", "", 1, 0.0, 0, 0, 0},
	{"Vibrator", 1, 0, 0, 12, 10, "", "", 1, 0.0, 0, 0, 0},
	{"Silver Vibrator", 1, 0, 0, 13, 10, "", "", 1, 0.0, 0, 0, 0},
	{"Flowers", 1, 0, 0, 14, 10, "", "", 1, 0.0, 0, 0, 0},
	{"Cane", 1, 0, 0, 15, 10, "", "", 1, 0.0, 0, 0, 0},
	{"Grenade", 10, 0, 0, 16, 8, "", "", 2, 0.0, 0, 0, 0},
	{"Tear Gas", 10, 0, 0, 17, 8, "", "", 2, 0.0, 0, 0, 0},
	{"Molotov Cocktail", 10, 0, 0, 18, 8, "", "", 2, 0.0, 0, 0, 0},
	{"Colt45", 250, 1200, 1, 22, 2, "COLT45", "colt45_reload", 125, 1.0, 0, 12, 20},
	{"Silenced 9mm", 250, 0, 0, 23, 2, "COLT45", "colt45_reload", 125, 0.0, 0, 12, 20},
	{"Deagle", 250, 0, 0, 24, 2, "COLT45", "colt45_reload", 125, 3.0, 2, 12, 20},
	{"Shotgun", 100, 3400, 1, 25, 3, "", "", 80, 3.0, 1, 4, 10},
	{"Sawn Off Shotgun", 100, 0, 0, 26, 3, "COLT45", "sawnoff_reload", 50, 0.0, 0, 2, 20},
	{"Spas12", 80, 0, 0, 27, 3, "BUDDY", "buddy_reload", 50, 5.0, 3, 5, 10},
	{"SMG", 280, 0, 0, 28, 4, "COLT45", "colt45_reload", 200, 0.0, 0, 30, 10},
	{"MP5", 250, 5200, 1, 29, 4, "PYTHON", "python_reload", 150, 3.5, 2, 30, 10},
	{"AK47", 230, 0, 0, 30, 5, "RIFLE", "RIFLE_load", 350, 4.0, 3, 60, 8},
	{"M4", 200, 0, 0, 31, 5, "RIFLE", "RIFLE_load", 250, 5.0, 4, 40, 9},
	{"Tec9", 140, 0, 0, 32, 4, "RIFLE", "RIFLE_load", 180, 0.0, 0, 30, 10},
	{"Scopeless Rifle", 100, 3600, 1, 33, 6, "", "", 100, 2.0, 1, 2, 10},
	{"Sniper Rifle with Scope", 60, 0, 0, 34, 6, "", "", 10, 20.0, 5, 1, 10},
	{"RPG", 4, 0, 0, 35, 7, "", "", 2, 0.0, 0, 0, 0},
	{"Heatseeking Rocket", 4, 0, 0, 36, 7, "", "", 2, 0.0, 0, 0, 0},
	{"Flame Thrower", 1000, 0, 0, 37, 7, "", "", 2, 0.0, 0, 0, 0},
	{"Minigun", 5000, 0, 0, 38, 7, "", "", 500, 0.0, 0, 0, 0},
	{"C4", 1, 0, 0, 39, 8, "", "", 2, 0.0, 0, 0, 0},
	{"Detonator", 1, 0, 0, 40, 12, "", "", 1, 0.0, 0, 0, 0},
	{"Spraycan", 100, 0, 0, 41, 9, "", "", 200, 0.0, 0, 0, 0},
	{"Fire Extingisher", 1000, 3000, 1, 42, 9, "", "", 5000, 0.0, 0, 0, 0},
	{"Camera", 25, 0, 0, 43, 9, "", "", 10, 0.0, 0, 0, 0},
	{"Night Vision Goggles", 1, 0, 0, 44, 11, "", "", 1, 0.0, 0, 0, 0},
	{"Thermal Vision Goggles", 1, 0, 0, 44, 11, "", "", 1, 0.0, 0, 0, 0}
};
new g_pGunInfo[MAX_PLAYERS][sizeof( g_weapInfo )][w_Info];


//==========================[ Crate System Header ]===========================
#define MAX_CRATES 500
#define CRATE_SMALL 1
#define CRATE_MED 2
#define CRATE_LARGE 3
#define DIALOG_CRATE_INFO 3396
#define DIALOG_CRATE_RELOAD 3397
new lastCrateID;

enum e_crateInfo
{
	crate_cap,
	crate_modelid,
	crate_type,
	crate_trunk,
	crate_vw,
	crate_int,
	crate_wID[10],
	crate_ID,
	crate_spawned,
	crate_player,
	crate_obj
}
new playerHasCrateID[MAX_PLAYERS];
new bool:crate_save[MAX_CRATES];
new g_Crates[MAX_CRATES][e_crateInfo];

//===[ Trunk ]====
#include "inc\trunk.inc"
//====
//===========================[ Limited ammo modules ]===========================
IsValidWeapon( playerid, weaponid )
{
	for( new i; i < sizeof ( g_weapInfo ); i++ )
	{
		if( g_pGunInfo[playerid][i][wo_Gun] )
		{
		    if( g_weapInfo[i][w_ID] == weaponid )
		    {
      			return GUN_TYPE_NORMAL;
		    }
		}
	}
	for( new i; i < sizeof( AdminGuns[] ); i++ )
	{
	    if( weaponid == AdminGuns[playerid][i] )
	    {
	        return GUN_TYPE_ADMIN;
	    }
	}
	if( weaponid == 46 ) return GUN_TYPE_NORMAL;
	return GUN_TYPE_HACKED; //hacked/desynced
}
//This function will return the weapon's index.
GetWeaponIndex( weaponid )
{
	for( new i; i < sizeof( g_weapInfo ); i++ )
	{
        if( g_weapInfo[i][w_ID] == weaponid )
            return i;
	}
	return 255;
}
//This module checks the slot is taken (as in weapon inventory slot).
static CheckPlayerGunSlots( playerid, weaponindex )
{
	for( new i; i < sizeof( g_weapInfo ); i++ )
	{
	    if( g_pGunInfo[playerid][i][wo_Gun] )
	    {
	        if( g_weapInfo[i][w_Slot] == g_weapInfo[weaponindex][w_Slot] )
	        {
	            return i;
	        }
	    }
	}
	return false;
}
//This function forces the player to reload their current weapon if their ammo isn't already full.
ReloadPlayerWeapon( playerid )
{
	if( onelessBullet[playerid] == GetPlayerWeapon( playerid ) && onelessBullet[playerid] > 0 )
    {
		new
		    weapon = onelessBullet[playerid];

		new w_index = GetWeaponIndex( weapon );
		if( strlen( g_weapInfo[w_index][w_AnimLib] ) > 0 )
		{
		    if( g_pGunInfo[playerid][w_index][wo_Mags] )
		    {
		        g_pGunInfo[playerid][w_index][wo_Mags]--;
				g_pGunInfo[playerid][w_index][wo_Ammo] = g_weapInfo[w_index][w_mags_bullets];
				TakePlayerWeapon( playerid, weapon );
				//ClearAnimations( playerid );
				GivePlayerGun( playerid, weapon, g_weapInfo[w_index][w_mags_bullets] );
				ApplyAnimationEx(playerid, g_weapInfo[w_index][w_AnimLib], g_weapInfo[w_index][w_AnimName], 4.0, 0, 0, 0, 0, 0);
				UpdateAmmoText( playerid, g_weapInfo[w_index][w_ID] );
			}
		}
		onelessBullet[playerid] = 0;
	}
	return 1;
}
//This module updates the ammo textdraw: displaying the count of magazines above the gun.
UpdateAmmoText( playerid, weaponid )
{
	if( IsValidWeapon( playerid, weaponid ) )
	{
	    new wi = GetWeaponIndex( weaponid );
	    new ammo_str[100];

		if( g_pGunInfo[playerid][wi][wo_Mags] < 1 )
		{
		    format( ammo_str, sizeof( ammo_str ), "~r~%d", g_pGunInfo[playerid][wi][wo_Mags] );
		}
		else
		{
		    format( ammo_str, sizeof( ammo_str ), "%d", g_pGunInfo[playerid][wi][wo_Mags] );
		}
		PlayerTextDrawSetString( playerid, ammo_text[playerid], ammo_str );
	}
	return 1;
}
/*
	This function gives a defined amount of clips to a player.
*/
GivePlayerClips( playerid, weaponid, clips )
{
	if( IsValidWeapon( playerid, weaponid ) == GUN_TYPE_NORMAL )
	{
		new wi = GetWeaponIndex( weaponid );
		if( clips + g_pGunInfo[playerid][wi][wo_Mags] > g_weapInfo[wi][w_max_mags] )
			return 1;
		else
		    g_pGunInfo[playerid][wi][wo_Mags] += clips;
	}
	return 1;
}
//This module gives the weapon & sets the variables, overwriting weapon slots,
//as described in CheckPlayerGunSlots ( ... ).
GivePlayerGun( playerid, weaponid, ammo = -1, save = true )
{
	new wi;
	for(new i; i < sizeof( g_weapInfo ); i++ )
	{
	    if( g_weapInfo[i][w_ID] == weaponid )
	    {
	        wi = i;
			break;
		}
	}
	if( ammo == -1 ) ammo = g_weapInfo[wi][w_dammo];
	
    if( g_pGunInfo[playerid][wi][wo_Gun] < 1 ) //has a gun
    {
		//weapon is in existing slot.
		new weapTakingSlot = CheckPlayerGunSlots( playerid, wi );
		if( weapTakingSlot )
		{
			g_pGunInfo[playerid][weapTakingSlot][wo_Gun] = 0;
			g_pGunInfo[playerid][weapTakingSlot][wo_Ammo] = 0;
		}

		g_pGunInfo[playerid][wi][wo_Gun] = 1;
		g_pGunInfo[playerid][wi][wo_Ammo] = ammo;
	}
	else
	{
	    g_pGunInfo[playerid][wi][wo_Ammo] += ammo;
	}
	GivePlayerWeapon( playerid, g_weapInfo[wi][w_ID], ammo );
	
	if( save )
	{
	 	new query[100];
		mysql_format( sqlGameConnection, query, sizeof( query ), "INSERT INTO `ammo` SET `wo_index` = %d, `wo_Ammo` = %d, `pid` = %d", wi, ammo, PlayerInfo[playerid][pID] );
		mysql_pquery( sqlGameConnection, query );
	}
	
	SetPlayerArmedWeapon( playerid, g_weapInfo[wi][w_ID] );
	iCurrentWeapon[playerid] = GetPlayerWeapon( playerid );
	return 1;
}
/*
	Removes all admin guns player is holding
*/
RemoveAdminGuns( playerid, i = 0 ) //simple looping func
{
	if( i < sizeof( AdminGuns[] ) )
	{
		if( AdminGuns[playerid][i] )
	 		TakePlayerWeapon( playerid, g_weapInfo[i][w_ID], false );
	    
		return RemoveAdminGuns( playerid, i + 1 );
	}
	return 1;
}
//This will reset the player's weapons
ResetPlayerGuns( playerid )
{
	ResetPlayerWeapons( playerid );
	for( new i; i < sizeof( g_pGunInfo[] ); i++ )
	{
	    if( g_pGunInfo[playerid][i][wo_Gun] )
	    {
		    if( inPaintball[playerid] )
		    {
		        if( IsValidWeapon( playerid, g_weapInfo[i][w_ID] ) == GUN_TYPE_NORMAL )
		            continue;
		            
                GiveAdminWeapon( playerid, g_weapInfo[i][w_ID] );
                return 1;
			}
			
 			GivePlayerWeapon( playerid, g_weapInfo[i][w_ID], g_pGunInfo[playerid][i][wo_Ammo] );
		}
	}
	return 1;
}
//This function takes a weapon from the player, and resets its variables.
TakePlayerWeapon( playerid, weaponid, del = true )
{
	for( new i; i < sizeof( g_weapInfo ); i++ )
	{
	    if( g_weapInfo[i][w_ID] == weaponid )
	    {
	        g_pGunInfo[playerid][i][wo_Ammo] = 0;
	        g_pGunInfo[playerid][i][wo_Gun] = 0;
	        SetPlayerAmmo( playerid, g_weapInfo[i][w_Slot], 0 );
	        
	        if( del )
	        {
				//don't want big tables: slows searches, recall: its threaded
				new query[100];
				mysql_format( sqlGameConnection, query, sizeof( query ), "DELETE FROM `ammo` WHERE `pid` = %d AND `wo_index` = %d", PlayerInfo[playerid][pID], i );
				mysql_pquery( sqlGameConnection, query );
			}
	    }
	}
	ResetPlayerGuns( playerid );
	return 1;
}
//This function is for giving admin given weapons, and replaces any weapon in that weapon slot.
GiveAdminWeapon( playerid, weaponid )
{
	new slot = g_weapInfo[GetWeaponIndex( weaponid )][w_Slot];
    AdminGuns[playerid][slot] = weaponid;
    GivePlayerWeapon( playerid, weaponid, 99999 );
    return 1;
}
LoadAmmo( playerid )
{
 	new query[100];
	mysql_format( sqlGameConnection, query, sizeof( query ), "SELECT * FROM `ammo` WHERE `pid` = %d", PlayerInfo[playerid][pID] );
	mysql_pquery( sqlGameConnection, query, "OnLoadAmmo", "i", playerid );
}
forward OnLoadAmmo( playerid );
public OnLoadAmmo( playerid )
{
    new rows, fields;
	cache_get_data(	rows, fields );
	if( rows )
	{
		for( new i; i < rows; i++ )
		{
			new wi = cache_get_field_content_int( i, "wo_index" );
			if( !wi ) continue;
			g_pGunInfo[playerid][wi][wo_Ammo] = cache_get_field_content_int( i, "wo_ammo" );
			g_pGunInfo[playerid][wi][wo_Gun] = 1;
		}
	}
	return 1;
}
SaveAmmo( playerid )
{
	for( new wi; wi < sizeof( g_weapInfo ); wi++ )
	{
		new query[150];
	    if( !g_pGunInfo[playerid][wi][wo_Gun] ) continue; //do not save unowned weapons
	    if( IsValidWeapon( playerid, g_weapInfo[wi][w_ID] ) != GUN_TYPE_NORMAL ) continue; //do not save admin weapons/hacked
	    mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `ammo` SET `wo_ammo` = %d WHERE `pid` = %d AND `wo_index` = %d", g_pGunInfo[playerid][wi][wo_Ammo], PlayerInfo[playerid][pID], wi );
	}
	return 1;
}
//===========================[ Limited ammo commands ]==========================
static str[128]; //only applies within this file.
/*	CMD:sellclips is used to sell ammo between players */
new OfferedAmmo[MAX_PLAYERS] = -1;
new OfferedAmmoGun[MAX_PLAYERS];
new OfferedAmmoAmo[MAX_PLAYERS];
new OfferedAmmoAmmo[MAX_PLAYERS];
CMD:sellclips( playerid, params[] )
{
	if( PlayerInfo[playerid][pJob] != 9 )
		return SendClientMessage( playerid, COLOR_GREY, "You are not an Arms Dealer!" );
		
	new buyer, weaponname[40], ammo, amo, confirm[10];
 	if( sscanf( params, "us[30]ddS(string)[10]", buyer, weaponname, ammo, amo, confirm ) )
	{
	    SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /sellclips [playerid/name] [weapon] [clips] [price(?)]." );
	    
		new gun_string[128];
		format( gun_string, 128, "Available gun names: " );
	    for( new i; i < sizeof( g_weapInfo ); i++ )
	    {
	        if( g_weapInfo[i][w_matsbullet] > 0 )
	        {
	            if( g_weapInfo[i][w_bull_level] <= PlayerInfo[playerid][pArmsSkill] )
	            {
	                if( strlen( gun_string ) >= 90 )
					{
					    SendClientMessage( playerid, COLOR_GREY, gun_string );
						format( gun_string, 128, ">: " );
					}
	                format( gun_string, 128, "%s %s | ", gun_string, g_weapInfo[i][w_Name] );
	            }
	        }
	    }
	    if( strlen( gun_string ) > 3 ) return SendClientMessage( playerid, COLOR_GREY, gun_string );
	}
	else
	{
	    if( !( amo > 0 || ammo > 0 ) )
	        return SendClientMessage( playerid, COLOR_GREY, "You must specify a valid amount." );
	        
	    for( new i; i < sizeof( g_weapInfo ); i++ )
	    {
	        if( g_weapInfo[i][w_matsbullet] > 0 )
	        {
	            if( strcmp( g_weapInfo[i][w_Name], weaponname, true ) == 0 )
	            {
	                ammo = ammo * g_weapInfo[i][w_mags_bullets]; //converted to mags
	                if( g_weapInfo[i][w_matsbullet] * ammo <= PlayerInfo[playerid][pMats] )
	                {
	                    if( g_weapInfo[i][w_bull_level] <= PlayerInfo[playerid][pArmsSkill] )
		        		{
		        		    if( strcmp( confirm, "confirm", true ) == 0 )
		        		    {
				                if( IsPlayerConnected( playerid ) )
							    {
							        new Float:pos[3];
							        GetPlayerPos( buyer, pos[0], pos[1], pos[2] );
									if( !IsPlayerInRangeOfPoint( playerid, 2.0, pos[0], pos[1], pos[2] ) )
									    return SendClientMessage( playerid, COLOR_GREY, "You are too far away from the seller." );
									    
                                    new offeredclips = floatround(ammo/g_weapInfo[i][w_mags_bullets]);
							        format( str, sizeof( str ), "%s has offered you %d clips for a %s for $%d, /acceptclips to accept it.", PlayerICName( playerid ), offeredclips, g_weapInfo[i][w_Name], amo );
							        SendClientMessage( buyer, COLOR_LIGHTBLUE, str );
							        OfferedAmmo[buyer] = playerid;
							        OfferedAmmoGun[buyer] = i;
							        OfferedAmmoAmo[buyer] = amo;
									OfferedAmmoAmmo[buyer] = ammo;
									
							        format( str, sizeof( str ), "You have offered %s %d clips for their %s for $%d.", PlayerICName( buyer ), offeredclips, g_weapInfo[i][w_Name], amo );
							        SendClientMessage( playerid, COLOR_LIGHTBLUE, str );
							        return 1;
						        }
							}
							else
							{
							    format( str, sizeof( str ), "Are you sure you want to use %0.2f materials for this?", g_weapInfo[i][w_matsbullet] * ammo );
								SendClientMessage( playerid, COLOR_LIGHTBLUE, str );
								SendClientMessage( playerid, COLOR_GREY, " > Do: add \"confirm\" to the end of your /sellammo line." );
								return 1;
							}
						}
						else
						{
						    return SendClientMessage( playerid, COLOR_GREY, "You are not skilled enough to manufacture this." );
						}
					}
					else
					{
					    return SendClientMessage( playerid, COLOR_GREY, "You do not have enough materials for that." );
					}
	            }
	        }
	    }
		SendClientMessage( playerid, COLOR_GREY, "Invalid weapon name." );
	}
	return 1;
}
/*	CMD:acceptammo is linked to CMD:sellammo, accepting ammo. */
CMD:acceptclips( playerid, params[] )
{
	if( OfferedAmmo[playerid] >= 0 )
	{
		new wi = OfferedAmmoGun[playerid];
		new seller = OfferedAmmo[playerid];
		new ammo = OfferedAmmoAmmo[playerid];
		new amo = OfferedAmmoAmo[playerid];
		new Float:pos[3];
		if( IsPlayerConnected( seller ) )
		{
			GetPlayerPos( seller, pos[0], pos[1], pos[2] );
			if( !IsPlayerInRangeOfPoint( playerid, 2.0, pos[0], pos[1], pos[2] ) )
			    return SendClientMessage( playerid, COLOR_GREY, "You are too far away from the seller." );
			    
			if( PlayerInfo[seller][pMats] >= g_weapInfo[wi][w_matsbullet] * ammo )
			{
			    if( GetMoney( playerid ) >= amo )
			    {
					if( GetPlayerWeapon( playerid ) == g_weapInfo[wi][w_ID] )
					{
					    if( IsValidWeapon( playerid, g_weapInfo[wi][w_ID] ) )
					    {
		    				OfferedAmmoGun[playerid] = 0;
						    OfferedAmmo[playerid] = -1;
						    OfferedAmmoAmmo[playerid] = 0;
						    OfferedAmmoAmo[playerid] = 0;
						    GiveMoney( seller, amo );
						    GiveMoney( playerid, -amo );
						    ammo = floatround(ammo/g_weapInfo[wi][w_mags_bullets]);
						    GivePlayerClips( playerid, g_weapInfo[wi][w_ID], ammo );
						    if( g_pGunInfo[playerid][wi][wo_Ammo] < 2 )
						        ReloadPlayerWeapon( playerid );
							UpdateAmmoText( playerid, g_weapInfo[wi][w_ID] );

						    format( str, sizeof( str ), "You have brought %d clips from %s for your %s: for $%d.", ammo, PlayerICName( seller ), g_weapInfo[wi][w_Name], amo );
							SendClientMessage( playerid, COLOR_YELLOW, str );
							format( str, sizeof( str ), "You have sold %d clips for a %s: to %s for $%d.", ammo, g_weapInfo[wi][w_Name], PlayerICName( playerid ), amo );
							SendClientMessage( seller, COLOR_YELLOW, str );
						    return 1;
						}
						else
						    return SendClientMessage( playerid, COLOR_GREY, "That player doesn't have the right gun model.");
					}
					else
					{
					    format( str, sizeof( str ), "You must be holding the required gun: a %s.", g_weapInfo[wi][w_Name] );
					    return SendClientMessage( playerid, COLOR_GREY, str );
					}
			    }
			    else
			    {
			        return SendClientMessage( playerid, COLOR_GREY, "You do not have enough money." );
			    }
			}
		}
		return SendClientMessage( playerid, COLOR_GREY, "The supplier doesn't have enough supplies." );
	}
	return 1;
}
/*	CMD:buygun is used for buying weapons at an ammu-nation, only works inside the
	business, restricted level 4+ and to certain guns relying on g_gunInfo */
gunStoreDialog( playerid )
{
	for( new id = 0; id < sizeof(BizInfo); id++ )
	{
	    if( GetPlayerVirtualWorld( playerid ) == id )
		{
	    	if( !IsPlayerInRangeOfPoint(playerid, 25.0, BizInfo[id][bInsideX], BizInfo[id][bInsideY], BizInfo[id][bInsideZ] ) && BizInfo[id][bType] == 1 )
	    	{
	    	    break;
			}
		}
		if( id == sizeof( BizInfo ) ) return 1;
	}
	ShowPlayerDialog( playerid, DIALOG_GUNSTORE, DIALOG_STYLE_LIST, "Ammu-nation - what are you after?", "Weapons\nMagazines", "Go", "Cancel" );
	return 1;
}
buygun( playerid )
{
	if( PlayerInfo[playerid][pLevel] < 4 )
	    return SendClientMessage( playerid, COLOR_GREY, "You must be at least level 4 to buy guns." );

	for( new id = 0; id < sizeof(BizInfo); id++ )
	{
	    if( GetPlayerVirtualWorld( playerid ) == id )
		{
	    	if( !IsPlayerInRangeOfPoint(playerid, 25.0, BizInfo[id][bInsideX], BizInfo[id][bInsideY], BizInfo[id][bInsideZ] ) && BizInfo[id][bType] == 1 )
	    	{
	    	    break;
			}
		}

		if( id == sizeof( BizInfo ) ) return 1;
	}
	new str_dialog[500] = "Item\tPrice\tAmmo\n";
	for( new i; i < sizeof( g_weapInfo ); i++ ) //loop through possible guns, generate dialog's string.
	{
	    if( !g_weapInfo[i][w_Store] ) continue;
	    
	    new guntype = IsValidWeapon( playerid, g_weapInfo[i][w_ID] );
		//has the gun
		if( g_pGunInfo[playerid][i][wo_Gun] > 0 && guntype == GUN_TYPE_NORMAL )
   			format( str_dialog, sizeof( str_dialog ), "%s{00FF00}%s{FFFFFF}\t$%d\t%d\n",
				str_dialog, g_weapInfo[i][w_Name], g_weapInfo[i][w_storePrice]/3, g_weapInfo[i][w_mags_bullets] );

		//doesn't have the gun
		else
			format( str_dialog, sizeof( str_dialog ), "%s%s\t$%d\t0\n",
				str_dialog, g_weapInfo[i][w_Name], g_weapInfo[i][w_storePrice] );
	}
    ShowPlayerDialog( playerid, DIALOG_BUY_WEAPON, DIALOG_STYLE_TABLIST_HEADERS, "Ammu-nation - weapons", str_dialog, "Buy", "Cancel" );
	return 1;
}
buyammo( playerid )
{
 	#pragma unused playerid
	return 1;
}



CMD:givegun(playerid, params[])
{
	new playa, gun;
	if(PlayerInfo[playerid][pAdmin] >= 4 || PlayerInfo[playerid][pEventModd] > 0)
	{
		if(!sscanf(params, "ui", playa, gun))
		{

			if(gun < 1 ||gun > 46)
			{
				SendClientMessage(playerid, COLOR_GRAD1, "Invalid weapon ID.");
				return 1;
			}
			if(IsPlayerConnected(playa))
			{
				if(gun == 21)
				{
					SetPlayerSpecialAction(playa, SPECIAL_ACTION_USEJETPACK);
				}
				GiveAdminWeapon( playa, gun );
				format( str, sizeof( str ), "You have given gun %d to %s.", gun, PlayerICName(playa) );
				SendClientMessage( playerid, COLOR_ORANGE, str );
			}
		}
		else
		{
			SendClientMessage(playerid, COLOR_YELLOW, "_______________________________________");
			SendClientMessage(playerid, COLOR_WHITE, "1: Brass Knuckles 2: Golf Club 3: Nitestick 4: Knife 5: Baseball Bat 6: Shovel 7: Pool Cue 8: Katana 9: Chainsaw");
			SendClientMessage(playerid, COLOR_WHITE, "10: Purple Dildo 11: Small White Vibrator 12: Large White Vibrator 13: Silver Vibrator 14: Flowers 15: Cane 16: Frag Grenade");
			SendClientMessage(playerid, COLOR_WHITE, "17: Tear Gas 18: Molotov Cocktail 19: Vehicle Missile 20: Hydra Flare 21: Jetpack 22: 9mm 23: Silenced 9mm 24: Deagle 25: Shotgun");
			SendClientMessage(playerid, COLOR_WHITE, "26: Sawnoff Shotgun 27: Combat Shotgun 28: Micro SMG (Mac 10) 29: SMG (MP5) 30: AK-47 31: M4 32: Tec9 33: Country Rifle");
			SendClientMessage(playerid, COLOR_WHITE, "34: Sniper Rifle 35: Rocket Launcher 36: HS Rocket Launcher 37: Flamethrower 38: Minigun 39: Satchel Charge");
			SendClientMessage(playerid, COLOR_WHITE, "40: Detonator 41: Spraycan 42: Fire Extinguisher 43: Camera 44: Nightvision Goggles 45: Infared Goggles 46: Parachute");
			SendClientMessage(playerid, COLOR_YELLOW, "_______________________________________");
		}
	}
	else
	{
		AdmErrorMsg;
	}
	return 1;
}
//=====================[ Crate system - unfinished ]============================
//This module finds the closest crate to the player.
GetClosestCrate(playerid)
{
	new Float: opos[3], Float: ppos[3], crateid = -1,
		Float: olddistance = 0.0;

	GetPlayerPos( playerid, ppos[0], ppos[1], ppos[2] );
	for( new i; i < sizeof( g_Crates ); i++ )
	{
	    if( g_Crates[i][crate_spawned])
	    {
	        static Float: distance;

	        //restrictions
	        if( g_Crates[i][crate_trunk] > 0 ) continue; //exclude trunks from this
	        if( g_Crates[i][crate_vw] != GetPlayerVirtualWorld( playerid ) ) continue;
	        if( g_Crates[i][crate_int] != GetPlayerInterior( playerid ) ) continue;

	        if( g_Crates[i][crate_player] == playerid ) return i; //player has the crate in hands

	        if( g_Crates[i][crate_player] > MAX_PLAYERS || g_Crates[i][crate_player] < 0 )
		    	GetDynamicObjectPos( g_Crates[i][crate_obj], opos[0], opos[1], opos[2] );
			else
			    GetPlayerPos( g_Crates[i][crate_player], opos[0], opos[1], opos[2] );


			distance = floatsqroot( floatpower( ( opos[0] - ppos[0] ), 2 ) + floatpower( ( opos[1] - ppos[1] ), 2 ) + floatpower( ( opos[2] - ppos[2] ), 2 ) );
			if( olddistance == 0.0 )
			{
				crateid = i;
				olddistance = distance;
			}
			else if( distance < olddistance )
			{
			    crateid = i;
			    olddistance = distance;
			}
		}
	}
	if(olddistance < 2.0 ) return crateid;
	else return -1;
}
SaveCrates()
{
	new query[500];
	for( new i; i < MAX_CRATES; i++ )
	{
	    if( !crate_save[i] ) continue;

	    if( !( g_Crates[i][crate_spawned] && g_Crates[i][crate_trunk] ) ) continue;

	    new Float: cratepos[3];
	    if( g_Crates[i][crate_player] < MAX_PLAYERS && g_Crates[i][crate_player] > -1 )
	    {
	        GetPlayerPos( g_Crates[i][crate_player], cratepos[0], cratepos[1], cratepos[2] );
	    }
	    else if( !g_Crates[i][crate_trunk] )
	    {
	        GetDynamicObjectPos( g_Crates[i][crate_obj], cratepos[0], cratepos[1], cratepos[2] );
	    }
		mysql_format( sqlGameConnection, query, sizeof( query ),
			"UPDATE `crates` SET \
		 		`cap` = %d, `modelid` = %d, `type` = %d, `trunk` = %d, `vw` = %d, `int` = %d, \
					`xpos` = %f, `ypos` = %f, `zpos` = %f",
	    				g_Crates[i][crate_cap], g_Crates[i][crate_modelid], g_Crates[i][crate_type], g_Crates[i][crate_trunk],
	    					g_Crates[i][crate_vw], g_Crates[i][crate_int], cratepos[0], cratepos[1], cratepos[2] );

		for( new w; w < 9; w++ )
		{
			mysql_format( sqlGameConnection, query, sizeof( query ), "%s, `weapon%d` = %d", query, w, g_Crates[i][crate_wID][w] );
		}
		printf( "query: %s", query );
		mysql_format( sqlGameConnection, query, sizeof( query ), " %s WHERE `ID` = %d", query, g_Crates[i][crate_ID] );
	    mysql_pquery( sqlGameConnection, query );
	}
	printf( "saving: crates" );
	return 1;
}

//This command allows a General+ administrator to place ammo crates: small, med or large
//at their position.
CMD:placecrate( playerid, params[] )
{
	if( PlayerInfo[playerid][pAdmin] > 2 )
	{
	    new cratetype;
	    if( sscanf( params, "d", cratetype ) )
	        return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /placecrate [type: (small: 1, med: 2, large: 3)]" );

		if( cratetype < 1 || cratetype > 3 )
		    return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /placecrate [type: (small: 1, med: 2, large: 3)]" );

		new Float: pos[3];
	    GetPlayerPos( playerid, pos[0], pos[1], pos[2] );

	    new interior = GetPlayerInterior( playerid );
	    new vw = GetPlayerVirtualWorld( playerid );

	    for( new i; i < sizeof( g_Crates ); i++ )
	    {
	    	if( g_Crates[i][crate_spawned] ) continue; //find free slot

	    	#define GC(%1) g_Crates[i][crate_%1]
	    	//create crate object
	    	if( cratetype == CRATE_SMALL )
			{
				GC(obj) = CreateDynamicObject( 2969, pos[0], pos[1], pos[2] - 0.925, 0.0, 0.0, 0.0, vw, interior );
				GC(modelid) = 2969;
			}
	    	else if( cratetype == CRATE_MED )
			{
				GC(obj) = CreateDynamicObject( 3014, pos[0], pos[1], pos[2] - 0.8, 0.0, 0.0, 0.0, vw, interior );
				GC(modelid) = 3014;
			}
	    	else if( cratetype == CRATE_LARGE )
			{
				GC(obj) = CreateDynamicObject( 3015, pos[0], pos[1], pos[2] - 0.9, 0.0, 0.0, 0.0, vw, interior );
				GC(modelid) = 3015;
			}

			//set the vars of the crate
			GC(type) = cratetype;
	    	GC(cap) = cratetype * 300;
	    	GC(spawned) = 1;
	    	GC(player) = 9999;

	    	#undef GC
	    	mysql_pquery( sqlGameConnection, "INSERT INTO crates SET modelid = 0" );
	    	crate_save[i] = true; //save on queue
	    	g_Crates[i][crate_ID] = lastCrateID + 1;
	    	lastCrateID++;
	    	break;
		}
	}
	return 1;
}
//This command basically removes an ammo crate & resets the variables.
CMD:removecrate( playerid, params[] )
{
	if( PlayerInfo[playerid][pAdmin] > 2 )
	{
	    new crateid = GetClosestCrate( playerid );

	    if( crateid == -1 )
	        return SendClientMessage( playerid, COLOR_GREY, "You are not by a crate." );

		DestroyDynamicObject( g_Crates[crateid][crate_obj] );

		g_Crates[crateid][crate_spawned] = 0;
		g_Crates[crateid][crate_type] = 0;
		g_Crates[crateid][crate_obj] = 0;
		g_Crates[crateid][crate_cap] = 0;

		for( new i; i < sizeof( g_Crates[][] ); i++ )
		{
		    g_Crates[crateid][crate_wID] = 0;
		}

		SendClientMessage( playerid, COLOR_GREY, "You have removed the crate." );
	}
	return 1;
}
//This allows players to check information regarding the closest crates, providing
//information such as what weapon the ammo is suitable for, how much ammo, and the crate ID.
//Also allows the player to get ammo from the crate.
CMD:crate( playerid, params[] )
{
	if( PlayerInfo[playerid][pLevel] < 4 )
	    return SendClientMessage( playerid, COLOR_GREY, "You must be level four to use this feature." );

	new option[128];

	if( sscanf( params, "s[128]", option ) ) //shows crate's content & commands.
	{
	    new crateid = GetClosestCrate( playerid );
		if( crateid < 0 )
			return SendClientMessage(playerid, COLOR_GREY, "You are not by any crates.");

		new dialogTitle[100], dialogInfo[450];
		format( dialogTitle, 100, "{FFFF00}Crate: %d{FFFFFF}", crateid );
		format( dialogInfo, 450, "{FFFF00}You look inside the crate and find;\n\n{FFFF00}Contents:{FFFFFF} %d ammo\n{FFFF00}Suitable for:{FFFFFF}", g_Crates[crateid][crate_cap] );

		new weapcount;
		for( new i; i < g_Crates[crateid][crate_type]; i++ )
		{
		    if( g_Crates[crateid][crate_wID][i] > 0 )
		    {
		        weapcount++;
				format( dialogInfo, 450, "%s %s,", dialogInfo, g_weapInfo[GetWeaponIndex( g_Crates[crateid][crate_wID][i] )][w_Name] );
			}
		}
		if( weapcount > 0 )
		{
			strdel( dialogInfo, strlen( dialogInfo ) - 1, strlen( dialogInfo ) );
		}
		else
		{
		    format( dialogInfo, 450, "{FFFF00}%s nothing{FFFFFF}", dialogInfo );
		}
		format( dialogInfo, 450, "%s.\n\nTIP: use /crate ammo whilst holding the suited weapon(s) to refill your ammo.", dialogInfo );
		format( dialogInfo, 450, "%s\nCMDS: /pickupcrate, /dropcrate, /crate, /trunk.", dialogInfo );
		ShowPlayerDialog( playerid, DIALOG_CRATE_INFO, DIALOG_STYLE_MSGBOX, dialogTitle, dialogInfo, "Reload", "Okay" );
		return 1;
	}
	if( strcmp( option, "ammo", true ) == 0 ) //allows the player(s) to get ammo from the crate.
	{
	    new crateid = GetClosestCrate( playerid );
		if( crateid < 0 )
			return SendClientMessage(playerid, COLOR_GREY, "You are not by any crates.");

	    new weapon = GetPlayerWeapon( playerid );
		new w_index = GetWeaponIndex( weapon );

		if( w_index == 255 )
		    return SendClientMessage( playerid, COLOR_GREY, "You are not holding a valid weapon." );

		if( g_Crates[crateid][crate_player] > -1 && g_Crates[crateid][crate_player] < 9999 )
		    return SendClientMessage( playerid, COLOR_GREY, "You cannot do that while another player is holding the crate." );

		for( new i; i < g_Crates[crateid][crate_type]; i++ )
		{
			if( g_Crates[crateid][crate_wID][i] == g_weapInfo[w_index][w_ID] )
			{
				new take_amo = g_weapInfo[w_index][w_maxAmmo] - g_pGunInfo[playerid][w_index][wo_Ammo]; //max ammo amount

				if( take_amo < 1 )
				    return SendClientMessage( playerid, COLOR_GREY, "You cannot carry anymore ammo for this gun." );

				g_pGunInfo[playerid][w_index][wo_Ammo] += take_amo;
				g_Crates[crateid][crate_cap] -= take_amo;
				SetPlayerAmmo( playerid, weapon, g_pGunInfo[playerid][w_index][wo_Ammo] );

				format( option, 128, ( PlayerInfo[playerid][pMask] == 1 ) ? ( ( "* Stranger opens the crate's lid and takes enough ammo to fill their gun." ) )
											: ( "* %s opens the crate's lid and takes enough ammo to fill their gun." ), PlayerICName( playerid ) );

				ProxDetector( 30.0, playerid, option, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE );
				crate_save[crateid] = true;
				return 1;
			}
		}
	}
	return 1;
}
//This command enables players to pickup crates, still being able to use /crate.
CMD:pickupcrate( playerid, params[] )
{
    new crateid = GetClosestCrate( playerid );
	if( crateid < 0 )
		return SendClientMessage(playerid, COLOR_GREY, "You are not by any crates.");

    if( g_Crates[crateid][crate_player] == playerid )
        return SendClientMessage( playerid, COLOR_GREY, "You are already holding a crate." );

    new Float: opos[3], Float: ppos[3], Float: pangle;
    GetDynamicObjectPos( g_Crates[crateid][crate_obj], opos[0], opos[1], opos[2] );
    GetPlayerPos( playerid, ppos[0], ppos[1], ppos[2] );
    GetPlayerFacingAngle( playerid, pangle );

    //calc the angle between a player & an object
	new Float: crate_angle = 180.0 -atan2( ppos[0] - opos[0], ppos[1] - opos[1] ) + 5.0;
	if( !( pangle < crate_angle + 30 && pangle > crate_angle - 30) )
	    return SendClientMessage( playerid, COLOR_GREY, "You must be facing the crate." );

	ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Crouch_In", 3.0, 0, 0, 0, 0, 0, 0);
	DestroyDynamicObject( g_Crates[crateid][crate_obj] );
	g_Crates[crateid][crate_obj] = 0;

	//set the vars suggesting player has crate, and reference variable
	g_Crates[crateid][crate_player] = playerid;
	playerHasCrateID[playerid] = crateid;
	g_Crates[crateid][crate_vw] = GetPlayerVirtualWorld( playerid );
	g_Crates[crateid][crate_int] = GetPlayerInterior( playerid );

	switch( g_Crates[crateid][crate_type] )
	{
	    case 3: SetPlayerAttachedObject( playerid, 8, g_Crates[crateid][crate_modelid], 5, -0.050999, 0.109999, 0.202000, -83.899963, -1.600038, 2.699998 );
	    case 2: SetPlayerAttachedObject( playerid, 8, g_Crates[crateid][crate_modelid], 5, -0.050999, 0.174999, 0.202000, -83.899963, -1.600038, 2.699998 );
	    case 1: SetPlayerAttachedObject( playerid, 8, g_Crates[crateid][crate_modelid], 5, 0.000000, 0.109999, 0.202000, -83.899963, -1.600038, 111.100006 );
	}
	SetPlayerSpecialAction(playerid, 25);

	crate_save[crateid] = true;
	return 1;
}
//This command enables the ability for players to drop picked up crates, in front of them.
CMD:dropcrate( playerid, params[] )
{
    new crateid = playerHasCrateID[playerid];
    if( crateid < 0 )
        return SendClientMessage( playerid, COLOR_GREY, "You are not holding a crate." );

	if( g_Crates[crateid][crate_player] != playerid )
	    return SendClientMessage( playerid, COLOR_GREY, "You are not holding a crate." );

    RemovePlayerAttachedObject(playerid, 8);
	g_Crates[crateid][crate_player] = 9999;
	SetPlayerSpecialAction( playerid, 0 );
	ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Crouch_In", 3.0, 0, 0, 0, 0, 0, 0);

	new Float: pos[4];
    GetXYInFrontOfPlayer(playerid, pos[0], pos[1], pos[2], pos[3], 1.0);

	new Float: z_amo;
	switch( g_Crates[crateid][crate_type] )
	{
	    case 3: z_amo = 0.9;
	    case 2: z_amo = 0.8;
	    case 1: z_amo = 0.925;
	}

	g_Crates[crateid][crate_vw] = GetPlayerVirtualWorld( playerid );
	g_Crates[crateid][crate_int] = GetPlayerInterior( playerid );
	g_Crates[crateid][crate_obj] = CreateDynamicObject( g_Crates[crateid][crate_modelid], pos[0], pos[1], ( pos[2] - z_amo ), 0.0, 0.0, 0.0, g_Crates[crateid][crate_vw], g_Crates[crateid][crate_int] );
	Streamer_Update( playerid );

	playerHasCrateID[playerid] = -1;

	crate_save[crateid] = true;
	return 1;
}
//This allows administrators to add weapons to crates providing the weaponid, it'll
//automatically find the closest crate, and a free weapon slot to store that weapon.
//content inside ammo crates; the ammo, can only be used with the weapons set.
CMD:editcrate( playerid, params[] )
{
	if( PlayerInfo[playerid][pAdmin] > 2 )
	{
	    new crateid = GetClosestCrate( playerid );

		if( crateid == -1 )
		    return SendClientMessage( playerid, COLOR_GREY, "You are not near a crate." );

		new option[30], weaponid, client_str[128];
	    if( !sscanf( params, "s[30]D(0)", option, weaponid ) )
		{

			if( strcmp( option, "addweapon", true ) == 0 ) //add a weapon to the crate
			{
			    if( !sscanf( params, "s[30]i", option, weaponid ) )
				{
				    if( weaponid > 0 && weaponid < ( sizeof( g_weapInfo ) - 1 ) )
				    {
				        //find free weapon slot inside crate, small = 1 slot, med = 2 slots, large = 3
				        for( new i; i < g_Crates[crateid][crate_type]; i++ )
				        {
				            if( g_Crates[crateid][crate_wID][i] < 1 )
				            {
		      					g_Crates[crateid][crate_wID][i] = weaponid;

								format( client_str, 128, "You have added weapon id: %d to the crate: %d", weaponid, crateid );
                                crate_save[crateid] = true;
								return SendClientMessage( playerid, COLOR_GREY, client_str );
							}
						}
						return SendClientMessage( playerid, COLOR_GREY, "This crate is full." );
					}
					return SendClientMessage( playerid, COLOR_GREY, "Invalid weapon ID." );
				}
				else
				{
				    return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /editcrate addweapon [weaponid]" );
				}
			}
		}
		else
		{
		    return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /editcrate [addweapon]" );
		}
	}
	return 1;
}
//====================[ Dialogs belonging to ammo & trunk ]========================
ammo_OnDialogResponse( playerid, dialogid, response, listitem, inputtext[] ) {

	if( dialogid == DIALOG_GUNSTORE ) {
		if( listitem == 0 ) {
			return buygun( playerid );
		}
		else if( listitem == 1 ) {
			return buyammo( playerid );
		}
		return 1;
	}
	if( dialogid == DIALOG_CRATE_INFO )
	{
		if( !response ) return 1;
		new crateid = GetClosestCrate( playerid );
		if( crateid < 0 )
			return SendClientMessage(playerid, COLOR_GREY, "You are not by any crates.");

		new dialogTitle[50];
		format( dialogTitle, 100, "{FFFF00}Crate: %d{FFFFFF}", crateid );
		ShowPlayerDialog( playerid, DIALOG_CRATE_RELOAD, DIALOG_STYLE_INPUT, dialogTitle, "How many magazines do you want?", "Okay", "Cancel" );
		return 1;
	}
	if( dialogid == DIALOG_CRATE_RELOAD )
	{
		if( !response ) return 1;
		new amount = strval( inputtext );
	    new crateid = GetClosestCrate( playerid );
		if( crateid < 0 )
			return SendClientMessage(playerid, COLOR_GREY, "You are not by any crates.");

	    new weaponid = GetPlayerWeapon( playerid );
		if( weaponid == 0 ) return SendClientMessage( playerid, COLOR_GREY, "You must be holding the weapon." );
		
	    new windex = GetWeaponIndex( weaponid );
		
		new gunBelongs = 0;
		for( new i; i < g_Crates[crateid][crate_type]; i++ )
		{
		    if( GetWeaponIndex( g_Crates[crateid][crate_wID][i] ) == windex )
		    {
				gunBelongs = 1;
				break;
			}
		}
		if( !gunBelongs ) return SendClientMessage( playerid, COLOR_GREY, "This ammo doesn't fit." ); 
		
	    if( IsValidWeapon( playerid, weaponid ) == GUN_TYPE_NORMAL )
		{
		    if( amount > 0 )
		    {
			    if( amount*g_weapInfo[windex][w_mags_bullets] <= g_Crates[crateid][crate_cap] )
			    {
			        g_Crates[crateid][crate_cap] -= amount*g_weapInfo[windex][w_mags_bullets];
					g_pGunInfo[playerid][windex][wo_Mags] += amount;
					new take_str[128];
					format( take_str, 128, "You have taken %d magazines from the crate.", amount );
					UpdateAmmoText( playerid, weaponid );
					return SendClientMessage( playerid, COLOR_LIGHTBLUE, take_str );
				}
				else
				{
				    return SendClientMessage( playerid, COLOR_GREY, "There isn't enough capacity left inside this crate for that much magazines." );
				}
			}
			else
			{
				SendClientMessage( playerid, COLOR_GREY, "You cannot take that much magazines." );
			}
		}
		return 1;
	}
	if(dialogid == DIALOG_BUY_WEAPON)
	{
	    if( !response ) return 1;
	    else
	    {
	        new itemcount;
			for( new i; i < sizeof( g_weapInfo ); i++ ) //loop through all available guns in enumerator
			{
			    if( !g_weapInfo[i][w_Store] ) continue;

			    if( itemcount == listitem )
			    {
			        new insideBiz;
      				for( new id = 0; id < sizeof(BizInfo); id++ )
					{
					    if( GetPlayerVirtualWorld( playerid ) == id )
						{
					    	if( !IsPlayerInRangeOfPoint(playerid, 25.0, BizInfo[id][bInsideX], BizInfo[id][bInsideY], BizInfo[id][bInsideZ] ) )
					    	{
					    	    insideBiz = id;
					    	    break;
							}
						}
						if( id == sizeof( BizInfo ) ) return 1;
					}

			        //event of dialog spoofing to get more than max ammo
					if( g_pGunInfo[playerid][i][wo_Ammo] >= g_weapInfo[i][w_maxAmmo] )
					    return SendClientMessage( playerid, COLOR_GREY, "You cannot carry anymore ammo.");

			        //not enough money for the gun
			        if( GetPlayerMoney( playerid ) < g_weapInfo[i][w_storePrice] )
						return SendClientMessage( playerid, COLOR_GREY, "You can't afford that." );

					static str_dialog[128], str_paylog[128];
					if( g_pGunInfo[playerid][i][wo_Gun] < 1 ) //just a gun
					{
						GiveMoney( playerid, -g_weapInfo[i][w_storePrice] );
						format( str_dialog, 128, "Congratulations, you have brought a %s with %d starting ammo.", g_weapInfo[i][w_Name], g_weapInfo[i][w_mags_bullets] );
						format( str_paylog, 128, "%s has purchased a %s from an ammu-nation.", PlayerICName( playerid ), g_weapInfo[i][w_Name] );
						GivePlayerGun( playerid, g_weapInfo[i][w_ID], g_weapInfo[i][w_mags_bullets] ); //give the weapon
					}
					else //one clip
					{
						return displayCenterHUDInfo(playerid,  "You already own this weapon.", 8);
					}
					UpdateAmmoText( playerid, g_weapInfo[i][w_ID] );
					SendClientMessage( playerid, COLOR_LIGHTBLUE, str_dialog );
					BizInfo[insideBiz][bProducts] -= 1;
					PayLog(str_paylog);
					break;
			    }
				itemcount++;
			}
	    }
	    return 1;
	}
	//Trunk dialogs: trunk.inc
	if( dialogid == TRUNKDIALOG || dialogid == TRUNKPUTSTUFF )
	{
		new vid = GetClosestTrunk( playerid );
		if( vid == -2 ) return SendClientMessage( playerid, COLOR_GREY, "Found more than one trunk in range." );
		if( vid == -1 ) return SendClientMessage( playerid, COLOR_GREY, "You are not by any trunk." );

		if(IsAPlane(IsByVehTrunk[playerid]) || IsABike(IsByVehTrunk[playerid]) || IsAHelicopter(IsByVehTrunk[playerid]) || IsATrain(IsByVehTrunk[playerid]) || IsABoat(IsByVehTrunk[playerid]) || IsABus(IsByVehTrunk[playerid]) || GetVehicleModel(GetPlayerVehicleID(IsByVehTrunk[playerid])) == 523)
		{
			SendClientMessage(playerid, COLOR_GREY, "This vehicle doesn't have a trunk.");
			return 1;
		}

		
		if(dialogid == TRUNKDIALOG)
		{
			if( response ) {
				return ShowTrunkResponse( playerid, listitem, vid );
			}
		}
		if(dialogid == TRUNKPUTSTUFF) {
			if(response) {
				return ShowTrunkResponse_PUT( playerid, listitem, vid );
			}
			return 1;
		}
	}
	//===
	return 0;
}
