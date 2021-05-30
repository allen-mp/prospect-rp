/*
.---.  _              .-.     .-. _ 
: .; ::_;             : :     : ::_;
:   .'.-.,-.,-. .--.  : :   .-' :.-.
: :.`.: :: ,. :' .; ; : :_ ' .; :: :
:_;:_;:_;:_;:_;`.__,_;`.__;`.__.':_;
                                    
                                                     

@Creator of this script: Golden aka Troy Bradshaw.

THREAD LINK ( DRR ) :   http://www.gta-sarp.com/forums/showthread.php?249856                           
*/


/*
DEFINES FOR THE SCRIPTS
*/
#define RinaldiFILE "Rinaldi.data"

#define DIALOGID_DEPOSITMONEY 78601
#define DIALOGID_DEPOSIPOT 78602
#define DIALOGID_DEPOSITCRACK 78603
#define DIALOGID_DEPOSITMATS 78604
#define DIALOGID_DEPOSITWEPS 78605

#define DIALOGID_WITHDRAWMONEY 78606
#define DIALOGID_WITHDRAWPOT 78607
#define DIALOGID_WITHDRAWCRACK 78608
#define DIALOGID_WITHDRAWMATS 78609
#define DIALOGID_WITHDRAWWEPS 78610


new RinaldiDebug[128],
	RSafeACCESS[MAX_PLAYERS];
/* 
ESSENTIAL INCLUDS FOR THE SCRIPT
*/

#include "./Rinaldi/core.inc"
#include "./Rinaldi/commands.inc"



Hook:OnGameModeInit(){
	RinaldiLoad();
	return 1;
}

Hook:OnGameModeExit(){
	RinaldiSave();
	return 1;
}

Hook:OnPlayerConnect(playerid)
{
	RSafeACCESS[playerid] = 0;

	return 1;
}
Hook:OnPlayerDisconnect(playerid, reason)
{
	RSafeACCESS[playerid] = 0;
	return 1;
	
}
