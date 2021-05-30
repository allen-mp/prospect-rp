/*  
 *  Version: MPL 1.1
 *  
 *  The contents of this file are subject to the Mozilla Public License Version
 *  1.1 (the "License"); you may not use this file except in compliance with
 *  the License. You may obtain a copy of the License at
 *  [url]http://www.mozilla.org/MPL/[/url]
 *  
 *  Software distributed under the License is distributed on an "AS IS" basis,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 *  for the specific language governing rights and limitations under the
 *  License.
 *  
 *  The Original Code is the sscanf 2.0 SA:MP plugin.
 *  
 *  The Initial Developer of the Original Code is Alex "Y_Less" Cole.
 *  Portions created by the Initial Developer are Copyright (C) 2010
 *  the Initial Developer. All Rights Reserved.
 *  
 *  Contributor(s):
 *  
 *  Special Thanks to:
 *  
 *      SA:MP Team past, present and future
 */

#define SSCANF:%0(%1) sscanf_%0(%1);public sscanf_%0(%1)

#if defined sscanf
	#error sscanf (possibly the PAWN version) already defined.
#endif

native sscanf(const data[], const format[], {Float,_}:...);
native unformat(const data[], const format[], {Float,_}:...) = sscanf;
native SSCANF_Init(players, invalid, len);
native SSCANF_Join(playerid, const name[], npc);
native SSCANF_Leave(playerid);

native SSCANF_Option(const name[], value);

stock const
	SSCANF_QUIET[] = "SSCANF_QUIET",
	OLD_DEFAULT_NAME[] = "OLD_DEFAULT_NAME",
	MATCH_NAME_PARTIAL[] = "MATCH_NAME_PARTIAL",
	CELLMIN_ON_MATCHES[] = "CELLMIN_ON_MATCHES",
	OLD_DEFAULT_KUSTOM[] = "OLD_DEFAULT_KUSTOM",
	OLD_DEFAULT_CUSTOM[] = "OLD_DEFAULT_CUSTOM";

static stock
	bool:SSCANF_gInit = false;

#if !defined _ALS_
	forward public _ALS_();
	
	_ALS_() <_ALS_:unhooked>
	{
	}
	
	_ALS_() <_ALS_:hooked>
	{
	}
	
	_ALS_() <>
	{
	}
#endif

/*
  OnFilterScriptInit

  Called when the script starts if it is a filterscript, sets up the system,
  then calls the "real" OnFilterScriptInit (using the new ALS 2 hook method).
*/

public OnFilterScriptInit()
{
	SSCANF_Init(GetMaxPlayers(), INVALID_PLAYER_ID, MAX_PLAYER_NAME);
	SSCANF_gInit = true;
	state _ALS_:hooked;
	return SSCANF_OnFilterScriptInit();
}

#if defined _ALS_OnFilterScriptInit
    #undef OnFilterScriptInit
#else
    #define _ALS_OnFilterScriptInit
#endif
#define OnFilterScriptInit(%0) SSCANF_OnFilterScriptInit(%0) <_ALS_:hooked>

forward SSCANF_OnFilterScriptInit();

public SSCANF_OnFilterScriptInit() <_ALS_:unhooked>
{
	return 1;
}

public SSCANF_OnFilterScriptInit() <>
{
	return 1;
}

/*
  OnGameModeInit

  Called when the script starts if it is a gamemode.  This callback is also
  called in filterscripts so we don't want to reinitialise the system in that
  case.
*/

public OnGameModeInit()
{
	if (!SSCANF_gInit)
	{
		SSCANF_Init(GetMaxPlayers(), INVALID_PLAYER_ID, MAX_PLAYER_NAME);
		SSCANF_gInit = true;
		state _ALS_:hooked;
	}
	return SSCANF_OnGameModeInit();
}

#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit(%0) SSCANF_OnGameModeInit(%0) <_ALS_:hooked>

forward SSCANF_OnGameModeInit();

public SSCANF_OnGameModeInit() <_ALS_:unhooked>
{
	return 1;
}

public SSCANF_OnGameModeInit() <>
{
	return 1;
}

/*
  OnPlayerConnect

  Called when a player connects.  Actually increments an internal count so that
  if a script ends and "OnPlayerDisconnect" is called then "sscanf" still knows
  that the player is really connected.  Also stores their name internally.
*/

public OnPlayerConnect(playerid)
{
	new
		name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof (name));
	SSCANF_Join(playerid, name, IsPlayerNPC(playerid));
	return SSCANF_OnPlayerConnect(playerid);
}

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect(%0) SSCANF_OnPlayerConnect(%0) <_ALS_:hooked>

forward SSCANF_OnPlayerConnect(playerid);

public SSCANF_OnPlayerConnect(playerid) <_ALS_:unhooked>
{
	return 1;
}

public SSCANF_OnPlayerConnect(playerid) <>
{
	return 1;
}

/*
  OnPlayerDisconnect

  Called when a player disconnects, or when a script is ended.
*/

public OnPlayerDisconnect(playerid, reason)
{
	new
		ret = SSCANF_OnPlayerDisconnect(playerid, reason);
	SSCANF_Leave(playerid);
	return ret;
}

#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect(%0) SSCANF_OnPlayerDisconnect(%0) <_ALS_:hooked>

forward SSCANF_OnPlayerDisconnect(playerid, reason);

public SSCANF_OnPlayerDisconnect(playerid, reason) <_ALS_:unhooked>
{
	return 1;
}

public SSCANF_OnPlayerDisconnect(playerid, reason) <>
{
	return 1;
}

#define SSCANF_Init
#define SSCANF_Join
#define SSCANF_Leave

#define extract%0->%1; EXTRN%1;unformat(_:EXTRZ:EXTRV:EXTRX:%0,##,%1,,);
#define unformat(_:EXTRZ:EXTRV:EXTRX:%0,##,%1);%2else if (unformat(_:EXTRV:EXTRX:%0,##,%1))

#define EXTRV:EXTRX:%0<%3>##,%9new%1,%2) EXTRY:%0##P<%3>,|||%1|||%2)
#define EXTRX:%0##,%9new%1,%2) EXTRY:%0##,|||%1|||%2)
#define EXTRY: EXTR8:EXTR9:EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:

#define EXTR8:EXTR9:EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:%0##%1,%2|||%6:%3=%9|||%4) %6_EXTRO:%0##%1,%2|||%3=%9|||%4)
#define EXTR9:EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:%0##%1,%2|||%3=%9|||%4) __EXTRO:%0##%1,%2|||%3=%9|||%4)
#define EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:%0##%1,%2|||%6:%3[%7]|||%4) %6_EXTRW:%0##%1,%2|||%3[%7]|||%4)
#define EXTR1:EXTR2:EXTR3:EXTR4:%0##%1,%2|||%3[%7]|||%4) __EXTRW:%0##%1,%2|||%3|||%4)
#define EXTR2:EXTR3:EXTR4:%0##%1,%2|||%6:%3|||%4) %6_EXTRN:%0##%1,%2|||%3|||%4)
#define EXTR3:EXTR4:%0##%1,,%2||||||%4) %0##%1,%2)
#define EXTR4:%0##%1,%2|||%3|||%4) __EXTRN:%0##%1,%2|||%3|||%4)

// Optional specifiers.
#define __EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1I"("#%9")"#,%2,%3|||%4|||%5)
#define Float_EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1F"("#%9")"#,%2,%3|||%4|||%5)
#define player_EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1U"("#%9")"#,%2,%3|||%4|||%5)
#define string2_EXTRO:%0##%1,%2|||%3[%7]=%9|||%4,%5) EXTRY:%0##%1S"("#%9")"#[%7],%2,%3|||%4|||%5)

// Normal specifiers (the double underscore is to work for "_:".
#define __EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1i,%2,%3|||%4|||%5)
#define Float_EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1f,%2,%3|||%4|||%5)
#define player_EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1u,%2,%3|||%4|||%5)
//#define string2_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1s[%7],%2,%3|||%4|||%5)

// Array versions of normal specifiers.
#define __EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<i>[%7],%2,%3|||%4|||%5)
#define Float_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<f>[%7],%2,%3|||%4|||%5)
#define player_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<u>[%7],%2,%3|||%4|||%5)
#define string2_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1s[%7],%2,%3|||%4|||%5)

// Get rid of excess leading space which causes warnings.
#define EXTRN%0new%1; new%1;

#if !defined string2
    #define string2:
#endif

#define player:

#define hex:
#define hex_EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1H"("#%9")"#,%2,%3|||%4|||%5)
#define hex_EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1h,%2,%3|||%4|||%5)
#define hex_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<h>[%7],%2,%3|||%4|||%5)

#define bin:
#define bin_EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1B"("#%9")"#,%2,%3|||%4|||%5)
#define bin_EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1b,%2,%3|||%4|||%5)
#define bin_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<b>[%7],%2,%3|||%4|||%5)

#define kustom:%0<%1> %0
#define kustom_EXTRO:%0##%1,%2|||%3<%8>=%9|||%4,%5) EXTRY:%0##%1K<%8>"("#%9")"#,%2,%3|||%4|||%5)
#define kustom_EXTRN:%0##%1,%2|||%3<%8>|||%4,%5) EXTRY:%0##%1k<%8>,%2,%3|||%4|||%5)
//#define bin_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<b>[%7],%2,%3|||%4|||%5)

SSCANF:weapon(string2[])
{
	// This function is VERY basic, needs VASTLY improving to detect variations.
	if ('0' <= string2[0] <= '9')
	{
		new
			ret = strval(string2);
		if (0 <= ret <= 18 || 22 <= ret <= 46)
		{
			return ret;
		}
	}
	else if (!strcmp(string2, "Unarmed")) return 0;
	else if (!strcmp(string2, "Brass Knuckles")) return 1;
	else if (!strcmp(string2, "Golf Club")) return 2;
	else if (!strcmp(string2, "Night Stick")) return 3;
	else if (!strcmp(string2, "Knife")) return 4;
	else if (!strcmp(string2, "Baseball Bat")) return 5;
	else if (!strcmp(string2, "Shovel")) return 6;
	else if (!strcmp(string2, "Pool cue")) return 7;
	else if (!strcmp(string2, "Katana")) return 8;
	else if (!strcmp(string2, "Chainsaw")) return 9;
	else if (!strcmp(string2, "Purple Dildo")) return 10;
	else if (!strcmp(string2, "White Dildo")) return 11;
	else if (!strcmp(string2, "Long White Dildo")) return 12;
	else if (!strcmp(string2, "White Dildo 2")) return 13;
	else if (!strcmp(string2, "Flowers")) return 14;
	else if (!strcmp(string2, "Cane")) return 15;
	else if (!strcmp(string2, "Grenades")) return 16;
	else if (!strcmp(string2, "Tear Gas")) return 17;
	else if (!strcmp(string2, "Molotovs")) return 18;
	else if (!strcmp(string2, "Pistol")) return 22;
	else if (!strcmp(string2, "Silenced Pistol")) return 23;
	else if (!strcmp(string2, "Desert Eagle")) return 24;
	else if (!strcmp(string2, "Shotgun")) return 25;
	else if (!strcmp(string2, "Sawn Off Shotgun")) return 26;
	else if (!strcmp(string2, "Combat Shotgun")) return 27;
	else if (!strcmp(string2, "Micro Uzi")) return 28;
	else if (!strcmp(string2, "Mac 10")) return 28;
	else if (!strcmp(string2, "MP5")) return 29;
	else if (!strcmp(string2, "AK47")) return 30;
	else if (!strcmp(string2, "M4")) return 31;
	else if (!strcmp(string2, "Tec9")) return 32;
	else if (!strcmp(string2, "Rifle")) return 33;
	else if (!strcmp(string2, "Sniper Rifle")) return 34;
	else if (!strcmp(string2, "RPG")) return 35;
	else if (!strcmp(string2, "Missile Launcher")) return 36;
	else if (!strcmp(string2, "Flame Thrower")) return 37;
	else if (!strcmp(string2, "Minigun")) return 38;
	else if (!strcmp(string2, "Sachel Charges")) return 39;
	else if (!strcmp(string2, "Detonator")) return 40;
	else if (!strcmp(string2, "Spray Paint")) return 41;
	else if (!strcmp(string2, "Fire Extinguisher")) return 42;
	else if (!strcmp(string2, "Camera")) return 43;
	else if (!strcmp(string2, "Nightvision Goggles")) return 44;
	else if (!strcmp(string2, "Thermal Goggles")) return 45;
	else if (!strcmp(string2, "Parachute")) return 46;
	return -1;
}

SSCANF:vehicle(string2[])
{
	// This function is VERY basic, needs VASTLY improving to detect variations.
	if ('0' <= string2[0] <= '9')
	{
		new
			ret = strval(string2);
		if (400 <= ret <= 611)
		{
			return ret;
		}
	}
	else if (!strcmp(string2, "Landstalker")) return 400;
	else if (!strcmp(string2, "Bravura")) return 401;
	else if (!strcmp(string2, "Buffalo")) return 402;
	else if (!strcmp(string2, "Linerunner")) return 403;
	else if (!strcmp(string2, "Perenniel")) return 404;
	else if (!strcmp(string2, "Sentinel")) return 405;
	else if (!strcmp(string2, "Dumper")) return 406;
	else if (!strcmp(string2, "Firetruck")) return 407;
	else if (!strcmp(string2, "Trashmaster")) return 408;
	else if (!strcmp(string2, "Stretch")) return 409;
	else if (!strcmp(string2, "Manana")) return 410;
	else if (!strcmp(string2, "Infernus")) return 411;
	else if (!strcmp(string2, "Voodoo")) return 412;
	else if (!strcmp(string2, "Pony")) return 413;
	else if (!strcmp(string2, "Mule")) return 414;
	else if (!strcmp(string2, "Cheetah")) return 415;
	else if (!strcmp(string2, "Ambulance")) return 416;
	else if (!strcmp(string2, "Leviathan")) return 417;
	else if (!strcmp(string2, "Moonbeam")) return 418;
	else if (!strcmp(string2, "Esperanto")) return 419;
	else if (!strcmp(string2, "Taxi")) return 420;
	else if (!strcmp(string2, "Washington")) return 421;
	else if (!strcmp(string2, "Bobcat")) return 422;
	else if (!strcmp(string2, "Mr Whoopee")) return 423;
	else if (!strcmp(string2, "BF Injection")) return 424;
	else if (!strcmp(string2, "Hunter")) return 425;
	else if (!strcmp(string2, "Premier")) return 426;
	else if (!strcmp(string2, "Enforcer")) return 427;
	else if (!strcmp(string2, "Securicar")) return 428;
	else if (!strcmp(string2, "Banshee")) return 429;
	else if (!strcmp(string2, "Predator")) return 430;
	else if (!strcmp(string2, "Bus")) return 431;
	else if (!strcmp(string2, "Rhino")) return 432;
	else if (!strcmp(string2, "Barracks")) return 433;
	else if (!strcmp(string2, "Hotknife")) return 434;
	else if (!strcmp(string2, "Article Trailer")) return 435;
	else if (!strcmp(string2, "Previon")) return 436;
	else if (!strcmp(string2, "Coach")) return 437;
	else if (!strcmp(string2, "Cabbie")) return 438;
	else if (!strcmp(string2, "Stallion")) return 439;
	else if (!strcmp(string2, "Rumpo")) return 440;
	else if (!strcmp(string2, "RC Bandit")) return 441;
	else if (!strcmp(string2, "Romero")) return 442;
	else if (!strcmp(string2, "Packer")) return 443;
	else if (!strcmp(string2, "Monster")) return 444;
	else if (!strcmp(string2, "Admiral")) return 445;
	else if (!strcmp(string2, "Squallo")) return 446;
	else if (!strcmp(string2, "Seasparrow")) return 447;
	else if (!strcmp(string2, "Pizzaboy")) return 448;
	else if (!strcmp(string2, "Tram")) return 449;
	else if (!strcmp(string2, "Article Trailer 2")) return 450;
	else if (!strcmp(string2, "Turismo")) return 451;
	else if (!strcmp(string2, "Speeder")) return 452;
	else if (!strcmp(string2, "Reefer")) return 453;
	else if (!strcmp(string2, "Tropic")) return 454;
	else if (!strcmp(string2, "Flatbed")) return 455;
	else if (!strcmp(string2, "Yankee")) return 456;
	else if (!strcmp(string2, "Caddy")) return 457;
	else if (!strcmp(string2, "Solair")) return 458;
	else if (!strcmp(string2, "Berkley's RC Van")) return 459;
	else if (!strcmp(string2, "Skimmer")) return 460;
	else if (!strcmp(string2, "PCJ-600")) return 461;
	else if (!strcmp(string2, "Faggio")) return 462;
	else if (!strcmp(string2, "Freeway")) return 463;
	else if (!strcmp(string2, "RC Baron")) return 464;
	else if (!strcmp(string2, "RC Raider")) return 465;
	else if (!strcmp(string2, "Glendale")) return 466;
	else if (!strcmp(string2, "Oceanic")) return 467;
	else if (!strcmp(string2, "Sanchez")) return 468;
	else if (!strcmp(string2, "Sparrow")) return 469;
	else if (!strcmp(string2, "Patriot")) return 470;
	else if (!strcmp(string2, "Quad")) return 471;
	else if (!strcmp(string2, "Coastguard")) return 472;
	else if (!strcmp(string2, "Dinghy")) return 473;
	else if (!strcmp(string2, "Hermes")) return 474;
	else if (!strcmp(string2, "Sabre")) return 475;
	else if (!strcmp(string2, "Rustler")) return 476;
	else if (!strcmp(string2, "ZR-350")) return 477;
	else if (!strcmp(string2, "Walton")) return 478;
	else if (!strcmp(string2, "Regina")) return 479;
	else if (!strcmp(string2, "Comet")) return 480;
	else if (!strcmp(string2, "BMX")) return 481;
	else if (!strcmp(string2, "Burrito")) return 482;
	else if (!strcmp(string2, "Camper")) return 483;
	else if (!strcmp(string2, "Marquis")) return 484;
	else if (!strcmp(string2, "Baggage")) return 485;
	else if (!strcmp(string2, "Dozer")) return 486;
	else if (!strcmp(string2, "Maverick")) return 487;
	else if (!strcmp(string2, "SAN News Maverick")) return 488;
	else if (!strcmp(string2, "Rancher")) return 489;
	else if (!strcmp(string2, "FBI Rancher")) return 490;
	else if (!strcmp(string2, "Virgo")) return 491;
	else if (!strcmp(string2, "Greenwood")) return 492;
	else if (!strcmp(string2, "Jetmax")) return 493;
	else if (!strcmp(string2, "Hotring Racer")) return 494;
	else if (!strcmp(string2, "Sandking")) return 495;
	else if (!strcmp(string2, "Blista Compact")) return 496;
	else if (!strcmp(string2, "Police Maverick")) return 497;
	else if (!strcmp(string2, "Boxville")) return 498;
	else if (!strcmp(string2, "Benson")) return 499;
	else if (!strcmp(string2, "Mesa")) return 500;
	else if (!strcmp(string2, "RC Goblin")) return 501;
	else if (!strcmp(string2, "Hotring Racer")) return 502;
	else if (!strcmp(string2, "Hotring Racer")) return 503;
	else if (!strcmp(string2, "Bloodring Banger")) return 504;
	else if (!strcmp(string2, "Rancher")) return 505;
	else if (!strcmp(string2, "Super GT")) return 506;
	else if (!strcmp(string2, "Elegant")) return 507;
	else if (!strcmp(string2, "Journey")) return 508;
	else if (!strcmp(string2, "Bike")) return 509;
	else if (!strcmp(string2, "Mountain Bike")) return 510;
	else if (!strcmp(string2, "Beagle")) return 511;
	else if (!strcmp(string2, "Cropduster")) return 512;
	else if (!strcmp(string2, "Stuntplane")) return 513;
	else if (!strcmp(string2, "Tanker")) return 514;
	else if (!strcmp(string2, "Roadtrain")) return 515;
	else if (!strcmp(string2, "Nebula")) return 516;
	else if (!strcmp(string2, "Majestic")) return 517;
	else if (!strcmp(string2, "Buccaneer")) return 518;
	else if (!strcmp(string2, "Shamal")) return 519;
	else if (!strcmp(string2, "Hydra")) return 520;
	else if (!strcmp(string2, "FCR-900")) return 521;
	else if (!strcmp(string2, "NRG-500")) return 522;
	else if (!strcmp(string2, "HPV1000")) return 523;
	else if (!strcmp(string2, "Cement Truck")) return 524;
	else if (!strcmp(string2, "Towtruck")) return 525;
	else if (!strcmp(string2, "Fortune")) return 526;
	else if (!strcmp(string2, "Cadrona")) return 527;
	else if (!strcmp(string2, "FBI Truck")) return 528;
	else if (!strcmp(string2, "Willard")) return 529;
	else if (!strcmp(string2, "Forklift")) return 530;
	else if (!strcmp(string2, "Tractor")) return 531;
	else if (!strcmp(string2, "Combine Harvester")) return 532;
	else if (!strcmp(string2, "Feltzer")) return 533;
	else if (!strcmp(string2, "Remington")) return 534;
	else if (!strcmp(string2, "Slamvan")) return 535;
	else if (!strcmp(string2, "Blade")) return 536;
	else if (!strcmp(string2, "Freight (Train)")) return 537;
	else if (!strcmp(string2, "Brownstreak (Train)")) return 538;
	else if (!strcmp(string2, "Vortex")) return 539;
	else if (!strcmp(string2, "Vincent")) return 540;
	else if (!strcmp(string2, "Bullet")) return 541;
	else if (!strcmp(string2, "Clover")) return 542;
	else if (!strcmp(string2, "Sadler")) return 543;
	else if (!strcmp(string2, "Firetruck LA")) return 544;
	else if (!strcmp(string2, "Hustler")) return 545;
	else if (!strcmp(string2, "Intruder")) return 546;
	else if (!strcmp(string2, "Primo")) return 547;
	else if (!strcmp(string2, "Cargobob")) return 548;
	else if (!strcmp(string2, "Tampa")) return 549;
	else if (!strcmp(string2, "Sunrise")) return 550;
	else if (!strcmp(string2, "Merit")) return 551;
	else if (!strcmp(string2, "Utility Van")) return 552;
	else if (!strcmp(string2, "Nevada")) return 553;
	else if (!strcmp(string2, "Yosemite")) return 554;
	else if (!strcmp(string2, "Windsor")) return 555;
	else if (!strcmp(string2, "Monster \"A\"")) return 556;
	else if (!strcmp(string2, "Monster \"B\"")) return 557;
	else if (!strcmp(string2, "Uranus")) return 558;
	else if (!strcmp(string2, "Jester")) return 559;
	else if (!strcmp(string2, "Sultan")) return 560;
	else if (!strcmp(string2, "Stratum")) return 561;
	else if (!strcmp(string2, "Elegy")) return 562;
	else if (!strcmp(string2, "Raindance")) return 563;
	else if (!strcmp(string2, "RC Tiger")) return 564;
	else if (!strcmp(string2, "Flash")) return 565;
	else if (!strcmp(string2, "Tahoma")) return 566;
	else if (!strcmp(string2, "Savanna")) return 567;
	else if (!strcmp(string2, "Bandito")) return 568;
	else if (!strcmp(string2, "Freight Flat Trailer (Train)")) return 569;
	else if (!strcmp(string2, "Streak Trailer (Train)")) return 570;
	else if (!strcmp(string2, "Kart")) return 571;
	else if (!strcmp(string2, "Mower")) return 572;
	else if (!strcmp(string2, "Dune")) return 573;
	else if (!strcmp(string2, "Sweeper")) return 574;
	else if (!strcmp(string2, "Broadway")) return 575;
	else if (!strcmp(string2, "Tornado")) return 576;
	else if (!strcmp(string2, "AT400")) return 577;
	else if (!strcmp(string2, "DFT-30")) return 578;
	else if (!strcmp(string2, "Huntley")) return 579;
	else if (!strcmp(string2, "Stafford")) return 580;
	else if (!strcmp(string2, "BF-400")) return 581;
	else if (!strcmp(string2, "Newsvan")) return 582;
	else if (!strcmp(string2, "Tug")) return 583;
	else if (!strcmp(string2, "Petrol Trailer")) return 584;
	else if (!strcmp(string2, "Emperor")) return 585;
	else if (!strcmp(string2, "Wayfarer")) return 586;
	else if (!strcmp(string2, "Euros")) return 587;
	else if (!strcmp(string2, "Hotdog")) return 588;
	else if (!strcmp(string2, "Club")) return 589;
	else if (!strcmp(string2, "Freight Box Trailer (Train)")) return 590;
	else if (!strcmp(string2, "Article Trailer 3")) return 591;
	else if (!strcmp(string2, "Andromada")) return 592;
	else if (!strcmp(string2, "Dodo")) return 593;
	else if (!strcmp(string2, "RC Cam")) return 594;
	else if (!strcmp(string2, "Launch")) return 595;
	else if (!strcmp(string2, "Police Car (LSPD)")) return 596;
	else if (!strcmp(string2, "Police Car (SFPD)")) return 597;
	else if (!strcmp(string2, "Police Car (LVPD)")) return 598;
	else if (!strcmp(string2, "Police Ranger")) return 599;
	else if (!strcmp(string2, "Picador")) return 600;
	else if (!strcmp(string2, "S.W.A.T.")) return 601;
	else if (!strcmp(string2, "Alpha")) return 602;
	else if (!strcmp(string2, "Phoenix")) return 603;
	else if (!strcmp(string2, "Glendale Shit")) return 604;
	else if (!strcmp(string2, "Sadler Shit")) return 605;
	else if (!strcmp(string2, "Baggage Trailer \"A\"")) return 606;
	else if (!strcmp(string2, "Baggage Trailer \"B\"")) return 607;
	else if (!strcmp(string2, "Tug Stairs Trailer")) return 608;
	else if (!strcmp(string2, "Boxville")) return 609;
	else if (!strcmp(string2, "Farm Trailer")) return 610;
	else if (!strcmp(string2, "Utility Trailer")) return 611;
	return -1;
}

// Fix the compiler crash when both the PAWN and Plugin versions of sscanf are
// found by renaming the old version at declaration.  (fixes.inc compatible
// naming scheme: "BAD_Function()").
#define sscanf(%0:...) BAD_sscanf(%0:...)
