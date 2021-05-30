/*	        
									 /$$$$       /$$$$$$$$  /$$$$$$        /$$$$$$$  /$$$$$$$        /$$$$
									| $$_/      | $$_____/ /$$__  $$      | $$__  $$| $$__  $$      |_  $$
									| $$        | $$      | $$  \__/      | $$  \ $$| $$  \ $$        | $$
									| $$        | $$$$$   | $$            | $$$$$$$/| $$$$$$$/        | $$
									| $$        | $$__/   | $$            | $$__  $$| $$____/         | $$
									| $$        | $$      | $$    $$      | $$  \ $$| $$              | $$
									| $$$$      | $$$$$$$$|  $$$$$$/      | $$  | $$| $$             /$$$$
									|____/      |________/ \______/       |__/  |__/|__/            |____/

									 /$$$$$$$$ /$$ /$$   /$$                      /$$$$$$  /$$   /$$              
									| $$_____/| $$|__/  | $$                     /$$__  $$|__/  | $$              
									| $$      | $$ /$$ /$$$$$$    /$$$$$$       | $$  \__/ /$$ /$$$$$$   /$$   /$$
									| $$$$$   | $$| $$|_  $$_/   /$$__  $$      | $$      | $$|_  $$_/  | $$  | $$
									| $$__/   | $$| $$  | $$    | $$$$$$$$      | $$      | $$  | $$    | $$  | $$
									| $$      | $$| $$  | $$ /$$| $$_____/      | $$    $$| $$  | $$ /$$| $$  | $$
									| $$$$$$$$| $$| $$  |  $$$$/|  $$$$$$$      |  $$$$$$/| $$  |  $$$$/|  $$$$$$$
									|________/|__/|__/   \___/   \_______/       \______/ |__/   \___/   \____  $$
									                                                                     /$$  | $$
									                                                                    |  $$$$$$/
									                                                                     \______/ 
									 /$$$$$$$            /$$                 /$$$$$$$  /$$                    
									| $$__  $$          | $$                | $$__  $$| $$                    
									| $$  \ $$  /$$$$$$ | $$  /$$$$$$       | $$  \ $$| $$  /$$$$$$  /$$   /$$
									| $$$$$$$/ /$$__  $$| $$ /$$__  $$      | $$$$$$$/| $$ |____  $$| $$  | $$
									| $$__  $$| $$  \ $$| $$| $$$$$$$$      | $$____/ | $$  /$$$$$$$| $$  | $$
									| $$  \ $$| $$  | $$| $$| $$_____/      | $$      | $$ /$$__  $$| $$  | $$
									| $$  | $$|  $$$$$$/| $$|  $$$$$$$      | $$      | $$|  $$$$$$$|  $$$$$$$
									|__/  |__/ \______/ |__/ \_______/      |__/      |__/ \_______/ \____  $$
									                                                                 /$$  | $$
									                                                                |  $$$$$$/
									                                                                 \______/ 	

						 /$$   /$$                  /$$$$$$                                      /$$$$$$$  /$$           /$$         /$$    
						| $$$ | $$                 /$$__  $$                                    | $$__  $$|__/          | $$        | $$    
						| $$$$| $$  /$$$$$$       | $$  \__/  /$$$$$$   /$$$$$$  /$$   /$$      | $$  \ $$ /$$  /$$$$$$ | $$$$$$$  /$$$$$$  
						| $$ $$ $$ /$$__  $$      | $$       /$$__  $$ /$$__  $$| $$  | $$      | $$$$$$$/| $$ /$$__  $$| $$__  $$|_  $$_/  
						| $$  $$$$| $$  \ $$      | $$      | $$  \ $$| $$  \ $$| $$  | $$      | $$__  $$| $$| $$  \ $$| $$  \ $$  | $$    
						| $$\  $$$| $$  | $$      | $$    $$| $$  | $$| $$  | $$| $$  | $$      | $$  \ $$| $$| $$  | $$| $$  | $$  | $$ /$$
						| $$ \  $$|  $$$$$$/      |  $$$$$$/|  $$$$$$/| $$$$$$$/|  $$$$$$$      | $$  | $$| $$|  $$$$$$$| $$  | $$  |  $$$$/
						|__/  \__/ \______/        \______/  \______/ | $$____/  \____  $$      |__/  |__/|__/ \____  $$|__/  |__/   \___/  
						                                              | $$       /$$  | $$                     /$$  \ $$                    
						                                              | $$      |  $$$$$$/                    |  $$$$$$/                    
						                                              |__/       \______/                      \______/                                     							  							
*/
/*
				LOGIN ANNOUNCEMENTS
					by Fred (Jacob_Swift)

*/



//TextDraw related variables
#include "../gamemodes/inc/cinematictd.inc"

//login textdraws
static Text:sarpHeading;
static Text:version;

//anouncement textdraws
static Text:Textdraw0;
static Text:Textdraw1;
static Text:model;
static Text:text_announcement_t;
static Text:text_announcement;
static Text:author;

//Announcement types
enum {
	ANNOUNCE_NONE,
	ANNOUNCE_FACTION,
	ANNOUNCE_DEV,
	ANNOUNCE_ADMIN
}

//When modifying the above, adjust this too
#define MAX_ANNOUNCE_TYPES 4
#define DIALOG_SETANNOUNCEMENT 9301

//Current announcement type
static stock AnnounceType;


/*
	Visibility functions
*/
ShowLoginTextDraw( playerid ) {
	TextDrawShowForPlayer(playerid, sarpHeading);
	TextDrawShowForPlayer(playerid, version);
	
	//ShowAnnouncementTextDraw( playerid );
	return;
}

ShowAnnouncementTextDraw( playerid ) {
	TextDrawShowForPlayer(playerid, Textdraw0);
	TextDrawShowForPlayer(playerid, Textdraw1);
	TextDrawShowForPlayer(playerid, model);
	TextDrawShowForPlayer(playerid, text_announcement_t);
	TextDrawShowForPlayer(playerid, text_announcement);
	TextDrawShowForPlayer(playerid, author);
	return;

}
HideLoginTextDraw( playerid ) {
	TextDrawHideForPlayer(playerid, sarpHeading);
	TextDrawHideForPlayer(playerid, version);

	//HideAnnouncementTextDraw( playerid );
	
	return;
}

HideAnnouncementTextDraw( playerid ) {
	TextDrawHideForPlayer(playerid, Textdraw0);
	TextDrawHideForPlayer(playerid, Textdraw1);
	TextDrawHideForPlayer(playerid, model);
	TextDrawHideForPlayer(playerid, text_announcement_t);
	TextDrawHideForPlayer(playerid, text_announcement);
	TextDrawHideForPlayer(playerid, author);
	return;
}

/*
	Updating TextDraw related functions
*/

SetLoginAnnouncement( type, text[] ) {
	SetAnnouncementType( type );
	TextDrawSetString( text_announcement, text );
	return;
}

SetAnnouncementChar( skinid ) {
	TextDrawSetPreviewModel( model, skinid );
	return;
}

SetAnnouncementAuthor( playerid ) {
	new string[128];
	format( string, 128, "%s", PlayerICName( playerid ) );
	TextDrawSetString( author, string );
	return;
}

SetAnnouncementType( type ) {
	AnnounceType = type;
	switch( type ) {
		case ANNOUNCE_FACTION: TextDrawSetString( text_announcement_t, "Public Service Announcement" );
		case ANNOUNCE_DEV: TextDrawSetString( text_announcement_t, "Development Announcement" );
		case ANNOUNCE_ADMIN: TextDrawSetString( text_announcement_t, "Administration Announcement" );
		default: TextDrawSetString( text_announcement_t, "Server Announcement" );
	}
	return;
}


/*
	Commands: 
		/lockannouncement (disables factions using it)
		/setannouncement (sets a new announcement, 2 min reload for factions)
		/announcement (allows players to view the announcement)
		
*/
static tempAnnounceType[MAX_PLAYERS];
static bool:lockAnnounce = false;

CMD:lockannouncement( playerid, params[] ) {
	if( PlayerInfo[playerid][pDev] > 2 || PlayerInfo[playerid][pAdmin] >= 4 ) {
		new msg[144];
		
		lockAnnounce = !lockAnnounce;
		format( msg, 144, "You have %s the ability for faction leaders to set login announcements." , ( lockAnnounce ) ? ( "disabled" ) : ( "enabled" ) );
		SendClientMessage( playerid, COLOR_YELLOW, msg );
	}
	return 1;

}

static lastAnnounce = 0;
CMD:setannouncement( playerid, params[] ) {

	if( PlayerInfo[playerid][pLeader] || PlayerInfo[playerid][pDev] > 2 || PlayerInfo[playerid][pAdmin] > 2 ) {
	
		//if disabled for faction leaders
		
		if( PlayerInfo[playerid][pDev] <= 2 || PlayerInfo[playerid][pAdmin] <= 2 ) {
			if( lockAnnounce ) {
				return SendClientMessage( playerid, COLOR_GREY, "Faction leaders currently cannot set announcements." );
			}
			if( gettime() < lastAnnounce + 120 ) {
				return SendClientMessage( playerid, COLOR_GREY, "You must wait two minutes between setting announcements, try again in two minutes." );
			}
		}
		
		new type;
		if( sscanf( params, "i", type ) ) {
			return SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /setannouncement [type: faction (1), dev (2), admin (3)]" );
		}
		
		if( type == ANNOUNCE_DEV && PlayerInfo[playerid][pDev] <= 2 ) {
			return SendClientMessage( playerid, COLOR_GREY, "You are not a Development Manager." ); 
		}
		
		if( type == ANNOUNCE_ADMIN && PlayerInfo[playerid][pAdmin] <= 2 ) {
			return SendClientMessage( playerid, COLOR_GREY, "You are not a General Administrator." ); 
		}
		
		if( type == ANNOUNCE_FACTION && !PlayerInfo[playerid][pLeader] ) {
			return SendClientMessage( playerid, COLOR_GREY, "You are not a leader of a faction." ); 
		}
		
		//Exclude 0 (type ANNOUNCE_NONE)
		if( !( 0 < type < MAX_ANNOUNCE_TYPES ) ) {
			return SendClientMessage( playerid, COLOR_GREY, "You have entered an invalid announcement type." );
		}
		
		tempAnnounceType[playerid] = type;
		ShowPlayerDialog(playerid, DIALOG_SETANNOUNCEMENT, DIALOG_STYLE_INPUT, "Set Login Announcement", "Enter an announcement that'll appear on the login screen.", "Save", "Cancel" );
	}
	return 1;
}

new togAnnouncement[MAX_PLAYERS];
CMD:announcement( playerid, params[] ) { 

	if( !togAnnouncement[playerid] ) {
	
		ShowAnnouncementTextDraw( playerid );
		SendClientMessage( playerid, COLOR_YELLOW, "You have enabled the announcement. Use /announcement again to disable it." );
	}
	else {
		HideAnnouncementTextDraw( playerid );
	}
		
	togAnnouncement[playerid] = !togAnnouncement[playerid];
	return 1;
}

/*
	Hooks: TextDraw related
*/

Hook:loginTD_OnPlayerDisconnect( playerid, reason ) {
	if( togAnnouncement[playerid] ) {
		HideAnnouncementTextDraw( playerid );
	}
	return 1;
}

Hook:loginTD_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) {
	if( dialogid == DIALOG_SETANNOUNCEMENT ) {
		if( !response ) return 1;
		
		
		if( PlayerInfo[playerid][pLeader] || PlayerInfo[playerid][pDev] > 2 || PlayerInfo[playerid][pAdmin] > 2 ) {
			if( strlen( inputtext ) >= 1023 ) {
				return SendClientMessage( playerid, COLOR_GREY, "The announcement you have entered is too long, try shorten it." );
			}
			
			SetLoginAnnouncement( tempAnnounceType[playerid], inputtext );
			SetAnnouncementChar( GetPlayerSkin( playerid ) );
			SetAnnouncementAuthor( playerid );
			
			tempAnnounceType[playerid] = ANNOUNCE_NONE;
			
			SendClientMessage( playerid, COLOR_YELLOW, "You have set the login announcement. To view your changes use /announcement." );
			
			new report[144];
			format( report, 144, "[Announce] (%s): %s", PlayerICName( playerid ), inputtext );
			SendAdminMessage( COLOR_YELLOW, report );
			
			lastAnnounce = gettime();
			
		}
	}
	return 1;
}


Hook:loginTD_OnGameModeInit() {

	// Create the textdraws:

	
	
	Textdraw0 = TextDrawCreate(461.000000, 158.000000, "headbg");
	TextDrawBackgroundColor(Textdraw0, 255);
	TextDrawFont(Textdraw0, 1);
	TextDrawLetterSize(Textdraw0, 0.000000, 6.000000);
	TextDrawColor(Textdraw0, -1);
	TextDrawSetOutline(Textdraw0, 0);
	TextDrawSetProportional(Textdraw0, 1);
	TextDrawSetShadow(Textdraw0, 1);
	TextDrawUseBox(Textdraw0, 1);
	TextDrawBoxColor(Textdraw0, 192652880);
	TextDrawTextSize(Textdraw0, 181.000000, 0.000000);
	TextDrawSetSelectable(Textdraw0, 0);

	Textdraw1 = TextDrawCreate(461.000000, 163.000000, "bg");
	TextDrawBackgroundColor(Textdraw1, 255);
	TextDrawFont(Textdraw1, 1);
	TextDrawLetterSize(Textdraw1, 0.000000, 5.199999);
	TextDrawColor(Textdraw1, -1);
	TextDrawSetOutline(Textdraw1, 0);
	TextDrawSetProportional(Textdraw1, 1);
	TextDrawSetShadow(Textdraw1, 1);
	TextDrawUseBox(Textdraw1, 1);
	TextDrawBoxColor(Textdraw1, 255);
	TextDrawTextSize(Textdraw1, 181.000000, 0.000000);
	TextDrawSetSelectable(Textdraw1, 0);

	text_announcement_t = TextDrawCreate(206.000000, 152.000000, "Server Announcement");
	TextDrawBackgroundColor(text_announcement_t, 255);
	TextDrawFont(text_announcement_t, 2);
	TextDrawLetterSize(text_announcement_t, 0.210000, 1.200000);
	TextDrawColor(text_announcement_t, -1);
	TextDrawSetOutline(text_announcement_t, 0);
	TextDrawSetProportional(text_announcement_t, 1);
	TextDrawSetShadow(text_announcement_t, 1);
	TextDrawSetSelectable(text_announcement_t, 0);

	model = TextDrawCreate(238.000000, 141.000000, "model");
	TextDrawBackgroundColor(model, 192652800);
	TextDrawFont(model, 5);
	TextDrawLetterSize(model, 0.500000, 4.000000);
	TextDrawColor(model, -1);
	TextDrawSetOutline(model, 0);
	TextDrawSetProportional(model, 1);
	TextDrawSetShadow(model, 1);
	TextDrawUseBox(model, 1);
	TextDrawBoxColor(model, 192652800);
	TextDrawTextSize(model, -90.000000, 100.000000);
	TextDrawSetSelectable(model, 0);
	TextDrawSetPreviewModel( model, 299 );
	TextDrawSetPreviewRot(model, 0.0, 0.0, 45.0, 1.0);

	text_announcement = TextDrawCreate(210.000000, 171.000000, "I have nothing for you at the moment, check back later on! Although...you could press the Spawn button.");
	TextDrawBackgroundColor(text_announcement, 255);
	TextDrawFont(text_announcement, 1);
	TextDrawLetterSize(text_announcement, 0.170000, 0.899999);
	TextDrawColor(text_announcement, -1);
	TextDrawSetOutline(text_announcement, 0);
	TextDrawSetProportional(text_announcement, 1);
	TextDrawSetShadow(text_announcement, 1);
	TextDrawUseBox(text_announcement, 1);
	TextDrawBoxColor(text_announcement, 192652800);
	TextDrawTextSize(text_announcement, 450.000000, 0.000000);
	TextDrawSetSelectable(text_announcement, 0);

	author = TextDrawCreate(210.000000, 200.000000, "");
	TextDrawBackgroundColor(author, 255);
	TextDrawFont(author, 1);
	TextDrawLetterSize(author, 0.200000, 1.000000);
	TextDrawColor(author, -1);
	TextDrawSetOutline(author, 0);
	TextDrawSetProportional(author, 1);
	TextDrawSetShadow(author, 1);
	TextDrawSetSelectable(author, 0);
	
	//login & other stuff
	createCinematicTextDraw();
	createAdvertismentTextDraw();
	
	sarpHeading = TextDrawCreate(250.000000, 10.000000, "District Roleplay");
	TextDrawBackgroundColor(sarpHeading, 255);
	TextDrawFont(sarpHeading, 0);
	TextDrawLetterSize(sarpHeading, 0.500000, 1.000000);
	TextDrawColor(sarpHeading, -1);
	TextDrawSetOutline(sarpHeading, 0);
	TextDrawSetProportional(sarpHeading, 1);
	TextDrawSetShadow(sarpHeading, 1);
	TextDrawSetSelectable(sarpHeading, 0);

	version = TextDrawCreate(294.000000, 21.000000, GAMEMODE_VERSION);
	TextDrawBackgroundColor(version, 255);
	TextDrawFont(version, 1);
	TextDrawLetterSize(version, 0.180000, 0.799998);
	TextDrawColor(version, -1);
	TextDrawSetOutline(version, 0);
	TextDrawSetProportional(version, 1);
	TextDrawSetShadow(version, 1);
	TextDrawSetSelectable(version, 0);
	
	
	return 1;
}