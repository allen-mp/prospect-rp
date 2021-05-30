/*
  ______ _      ______ _____ _______ _____ ____  _   _  _____   ______     __  _____  ______  _____ __  __  ____  _   _ _____  
 |  ____| |    |  ____/ ____|__   __|_   _/ __ \| \ | |/ ____| |  _ \ \   / / |  __ \|  ____|/ ____|  \/  |/ __ \| \ | |  __ \ 
 | |__  | |    | |__ | |       | |    | || |  | |  \| | (___   | |_) \ \_/ /  | |  | | |__  | (___ | \  / | |  | |  \| | |  | |
 |  __| | |    |  __|| |       | |    | || |  | | . ` |\___ \  |  _ < \   /   | |  | |  __|  \___ \| |\/| | |  | | . ` | |  | |
 | |____| |____| |___| |____   | |   _| || |__| | |\  |____) | | |_) | | |    | |__| | |____ ____) | |  | | |__| | |\  | |__| |
 |______|______|______\_____|  |_|  |_____\____/|_| \_|_____/  |____/  |_|    |_____/|______|_____/|_|  |_|\____/|_| \_|_____/ 
                                                                                                                              

*/

#define MAX_CANDIDATES 4
#define DIALOG_ELECTIONS_ONE 11560
#define DIALOG_ELECTIONS_TWO 11561
#define DIALOG_ELECTIONS_THREE 11562
#define DIALOG_ELECTIONS_FOUR 11563
#define DIALOG_ELECTIONS_FIVE 11564
#define DIALOG_ELECTIONS_SIX 11565
#define DIALOG_ELECTIONS_SEVEN 11566
#define DIALOG_ELECTIONS_EIGHT 11567
#define DIALOG_ELECTIONS_NINE 11568

enum eleinfo
{
	cname[24], // name of the candidate
	votes, // votes counter
	eskinid, // skinid of the model txd
	PlayerText: vtxd[MAX_PLAYERS], // votes txd
	PlayerText: ntxd[MAX_PLAYERS] // candidate name txd

};

new Elections[MAX_CANDIDATES][eleinfo];
new bool: ElectionsActive, ElectionsCandidates, bool: ElectionsReadable = false;
new elestr[128];
new pcandidates[MAX_PLAYERS];
new pcandidate1name[MAX_PLAYERS][24], pcandidate1skin[MAX_PLAYERS];
new pcandidate2name[MAX_PLAYERS][24], pcandidate2skin[MAX_PLAYERS];
new pcandidate3name[MAX_PLAYERS][24], pcandidate3skin[MAX_PLAYERS];
new pcandidate4name[MAX_PLAYERS][24], pcandidate4skin[MAX_PLAYERS];

new bool: viewingelections[MAX_PLAYERS] = false;
new PlayerText: electionstextdraws[MAX_PLAYERS][26];

new VoteTimer[MAX_PLAYERS];

forward StartElections(playerid, candidates);
forward VoteIncrease(candidate);
forward OnVoteCheck(playerid);
forward LogVote(playerid, candidate);
forward HideElectionsAll(playerid);

public HideElectionsAll(playerid)
{
	PlayerTextDrawHide(playerid, electionstextdraws[playerid][0]);	
	PlayerTextDrawHide(playerid, electionstextdraws[playerid][1]);
	PlayerTextDrawHide(playerid, electionstextdraws[playerid][2]);
	PlayerTextDrawHide(playerid, Elections[0][ntxd]);
	PlayerTextDrawHide(playerid, Elections[1][ntxd]);
	PlayerTextDrawHide(playerid, Elections[2][ntxd]);
	PlayerTextDrawHide(playerid, Elections[3][ntxd]);
	PlayerTextDrawHide(playerid, Elections[0][vtxd]);
	PlayerTextDrawHide(playerid, Elections[1][vtxd]);
	PlayerTextDrawHide(playerid, Elections[2][vtxd]);
	PlayerTextDrawHide(playerid, Elections[3][vtxd]);
	PlayerTextDrawHide(playerid, electionstextdraws[playerid][7]);
	PlayerTextDrawHide(playerid, electionstextdraws[playerid][8]);
	PlayerTextDrawHide(playerid, electionstextdraws[playerid][9]);
	PlayerTextDrawHide(playerid, electionstextdraws[playerid][10]);
	PlayerTextDrawHide(playerid, electionstextdraws[playerid][13]);
	PlayerTextDrawHide(playerid, electionstextdraws[playerid][16]);
	PlayerTextDrawHide(playerid, electionstextdraws[playerid][21]);
	PlayerTextDrawHide(playerid, electionstextdraws[playerid][22]);
	PlayerTextDrawHide(playerid, electionstextdraws[playerid][23]);
	PlayerTextDrawHide(playerid, electionstextdraws[playerid][24]);
	PlayerTextDrawHide(playerid, electionstextdraws[playerid][25]);
	return 1;
}

public StartElections(playerid, candidates)
{
	mysql_query(sqlGameConnection, "TRUNCATE TABLE voters;");

	ElectionsReadable = true;
	format(elestr, sizeof(elestr), "%s has started the elections poll! (/viewelections)", PlayerOOCName(playerid));
	SendClientMessageToAll(COLOR_LIGHTBLUE, elestr);

	for(new i = 0; i < MAX_CANDIDATES; i++)
	{
		Elections[i][votes] = 0;
	}

	format(Elections[0][cname], 24, "%s", pcandidate1name[playerid]);
	Elections[0][eskinid] = pcandidate1skin[playerid];

	format(Elections[1][cname], 24, "%s", pcandidate2name[playerid]);
	Elections[1][eskinid] = pcandidate2skin[playerid];

	if(candidates > 2)
	{
		format(Elections[2][cname], 24, "%s", pcandidate3name[playerid]);
		Elections[2][eskinid] = pcandidate3skin[playerid];
	}

	if(candidates == 4)
	{
		format(Elections[3][cname], 24, "%s", pcandidate4name[playerid]);
		Elections[3][eskinid] = pcandidate4skin[playerid];
	}

	foreach( new i: Player )
	{
		PlayerTextDrawSetString(i, Elections[0][ntxd][i], Elections[0][cname]);
		PlayerTextDrawSetString(i, Elections[1][ntxd][i], Elections[1][cname]);
		PlayerTextDrawSetString(i, Elections[2][ntxd][i], Elections[2][cname]);
		PlayerTextDrawSetString(i, Elections[3][ntxd][i], Elections[3][cname]);

		PlayerTextDrawSetString(i, Elections[0][vtxd][i], "Votes: 0");
		PlayerTextDrawSetString(i, Elections[1][vtxd][i], "Votes: 0"); 
		PlayerTextDrawSetString(i, Elections[2][vtxd][i], "Votes: 0"); 
		PlayerTextDrawSetString(i, Elections[3][vtxd][i], "Votes: 0"); 

		PlayerTextDrawSetPreviewModel(i, electionstextdraws[i][13], Elections[0][eskinid]);
		PlayerTextDrawSetPreviewModel(i, electionstextdraws[i][16], Elections[1][eskinid]);
		PlayerTextDrawSetPreviewModel(i, electionstextdraws[i][1], Elections[2][eskinid]);		
		PlayerTextDrawSetPreviewModel(i, electionstextdraws[i][2], Elections[3][eskinid]);
	}
	

	return 1;
}

public VoteIncrease(candidate)
{
	Elections[candidate][votes] ++;

	foreach( new i: Player )
	{
		format(elestr, sizeof(elestr), "Votes: %d", Elections[candidate][votes]);
		PlayerTextDrawSetString(i, Elections[candidate][vtxd][i], elestr);
	}


	return 1;
}

Hook:Elections_OnPlayerConnect(playerid)
{
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][0]);	
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][1]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][2]);
	PlayerTextDrawDestroy(playerid, Elections[0][ntxd]);
	PlayerTextDrawDestroy(playerid, Elections[1][ntxd]);
	PlayerTextDrawDestroy(playerid, Elections[2][ntxd]);
	PlayerTextDrawDestroy(playerid, Elections[3][ntxd]);
	PlayerTextDrawDestroy(playerid, Elections[0][vtxd]);
	PlayerTextDrawDestroy(playerid, Elections[1][vtxd]);
	PlayerTextDrawDestroy(playerid, Elections[2][vtxd]);
	PlayerTextDrawDestroy(playerid, Elections[3][vtxd]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][7]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][8]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][9]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][10]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][13]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][16]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][21]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][22]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][23]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][24]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][25]);

	electionstextdraws[playerid][0] = CreatePlayerTextDraw(playerid, 188.999984, 60.000000, "box");
	PlayerTextDrawLetterSize(playerid, electionstextdraws[playerid][0], 0.000000, 32.033344);
	PlayerTextDrawTextSize(playerid, electionstextdraws[playerid][0], 449.000000, 0.000000);
	PlayerTextDrawAlignment(playerid, electionstextdraws[playerid][0], 1);
	PlayerTextDrawColor(playerid, electionstextdraws[playerid][0], 16777215);
	PlayerTextDrawUseBox(playerid, electionstextdraws[playerid][0], 1);
	PlayerTextDrawBoxColor(playerid, electionstextdraws[playerid][0], 196796);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][0], 289);
	PlayerTextDrawSetOutline(playerid, electionstextdraws[playerid][0], 0);
	PlayerTextDrawBackgroundColor(playerid, electionstextdraws[playerid][0], 255);
	PlayerTextDrawFont(playerid, electionstextdraws[playerid][0], 1);
	PlayerTextDrawSetProportional(playerid, electionstextdraws[playerid][0], 1);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][0], 289);

	electionstextdraws[playerid][1] = CreatePlayerTextDraw(playerid, 160.333297, 259.244445, "");
	PlayerTextDrawLetterSize(playerid, electionstextdraws[playerid][1], 0.000000, 0.000000);
	PlayerTextDrawTextSize(playerid, electionstextdraws[playerid][1], 90.000000, 90.000000);
	PlayerTextDrawAlignment(playerid, electionstextdraws[playerid][1], 1);
	PlayerTextDrawColor(playerid, electionstextdraws[playerid][1], -1);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][1], 0);
	PlayerTextDrawSetOutline(playerid, electionstextdraws[playerid][1], 0);
	PlayerTextDrawBackgroundColor(playerid, electionstextdraws[playerid][1], -256);
	PlayerTextDrawFont(playerid, electionstextdraws[playerid][1], 5);
	PlayerTextDrawSetProportional(playerid, electionstextdraws[playerid][1], 0);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][1], 0);
	PlayerTextDrawSetPreviewModel(playerid, electionstextdraws[playerid][1], Elections[2][eskinid]);
	PlayerTextDrawSetPreviewRot(playerid, electionstextdraws[playerid][1], 0.000000, 0.000000, 0.000000, 1.000000);

	electionstextdraws[playerid][2] = CreatePlayerTextDraw(playerid, 385.333343, 258.000000, "");
	PlayerTextDrawLetterSize(playerid, electionstextdraws[playerid][2], 0.000000, 0.000000);
	PlayerTextDrawTextSize(playerid, electionstextdraws[playerid][2], 90.000000, 90.000000);
	PlayerTextDrawAlignment(playerid, electionstextdraws[playerid][2], 1);
	PlayerTextDrawColor(playerid, electionstextdraws[playerid][2], -1);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][2], 0);
	PlayerTextDrawSetOutline(playerid, electionstextdraws[playerid][2], 0);
	PlayerTextDrawBackgroundColor(playerid, electionstextdraws[playerid][2], 0);
	PlayerTextDrawFont(playerid, electionstextdraws[playerid][2], 5);
	PlayerTextDrawSetProportional(playerid, electionstextdraws[playerid][2], 0);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][2], 0);
	PlayerTextDrawSetPreviewModel(playerid, electionstextdraws[playerid][2], Elections[3][eskinid]);
	PlayerTextDrawSetPreviewRot(playerid, electionstextdraws[playerid][2], 0.000000, 0.000000, 0.000000, 1.000000);

	Elections[2][ntxd][playerid] = CreatePlayerTextDraw(playerid, 226.000030, 253.074081, Elections[2][cname]);
	PlayerTextDrawLetterSize(playerid, Elections[2][ntxd][playerid], 0.158663, 1.309630);
	PlayerTextDrawAlignment(playerid, Elections[2][ntxd][playerid], 1);
	PlayerTextDrawColor(playerid, Elections[2][ntxd][playerid], -1);
	PlayerTextDrawSetShadow(playerid, Elections[2][ntxd][playerid], 0);
	PlayerTextDrawSetOutline(playerid, Elections[2][ntxd][playerid], -2);
	PlayerTextDrawBackgroundColor(playerid, Elections[2][ntxd][playerid], 255);
	PlayerTextDrawFont(playerid, Elections[2][ntxd][playerid], 2);
	PlayerTextDrawSetProportional(playerid, Elections[2][ntxd][playerid], 1);
	PlayerTextDrawSetShadow(playerid, Elections[2][ntxd][playerid], 0);

	Elections[3][ntxd][playerid] = CreatePlayerTextDraw(playerid, 340.333374, 251.829620, Elections[3][cname]);
	PlayerTextDrawLetterSize(playerid, Elections[3][ntxd][playerid], 0.148329, 1.417479);
	PlayerTextDrawAlignment(playerid, Elections[3][ntxd][playerid], 1);
	PlayerTextDrawColor(playerid, Elections[3][ntxd][playerid], -1);
	PlayerTextDrawSetShadow(playerid, Elections[3][ntxd][playerid], 0);
	PlayerTextDrawSetOutline(playerid, Elections[3][ntxd][playerid], -2);
	PlayerTextDrawBackgroundColor(playerid, Elections[3][ntxd][playerid], 255);
	PlayerTextDrawFont(playerid, Elections[3][ntxd][playerid], 2);
	PlayerTextDrawSetProportional(playerid, Elections[3][ntxd][playerid], 1);
	PlayerTextDrawSetShadow(playerid, Elections[3][ntxd][playerid], 0);

	format(elestr, sizeof(elestr), "Votes: %i", Elections[2][votes]);
	Elections[2][vtxd][playerid] = CreatePlayerTextDraw(playerid, 230.999954, 275.414855, elestr);
	PlayerTextDrawLetterSize(playerid, Elections[2][vtxd][playerid], 0.252999, 1.197630);
	PlayerTextDrawAlignment(playerid, Elections[2][vtxd][playerid], 1);
	PlayerTextDrawColor(playerid, Elections[2][vtxd][playerid], -1);
	PlayerTextDrawSetShadow(playerid, Elections[2][vtxd][playerid], 0);
	PlayerTextDrawSetOutline(playerid, Elections[2][vtxd][playerid], -2);
	PlayerTextDrawBackgroundColor(playerid, Elections[2][vtxd][playerid], 255);
	PlayerTextDrawFont(playerid, Elections[2][vtxd][playerid], 2);
	PlayerTextDrawSetProportional(playerid, Elections[2][vtxd][playerid], 1);
	PlayerTextDrawSetShadow(playerid, Elections[2][vtxd][playerid], 0);

	format(elestr, sizeof(elestr), "Votes: %i", Elections[3][votes]);
	Elections[3][vtxd][playerid] = CreatePlayerTextDraw(playerid, 340.666717, 276.281524, elestr);
	PlayerTextDrawLetterSize(playerid, Elections[3][vtxd][playerid], 0.259665, 1.077334);
	PlayerTextDrawAlignment(playerid, Elections[3][vtxd][playerid], 1);
	PlayerTextDrawColor(playerid, Elections[3][vtxd][playerid], -1);
	PlayerTextDrawSetShadow(playerid, Elections[3][vtxd][playerid], 0);
	PlayerTextDrawSetOutline(playerid, Elections[3][vtxd][playerid], -2);
	PlayerTextDrawBackgroundColor(playerid, Elections[3][vtxd][playerid], 255);
	PlayerTextDrawFont(playerid, Elections[3][vtxd][playerid], 2);
	PlayerTextDrawSetProportional(playerid, Elections[3][vtxd][playerid], 1);
	PlayerTextDrawSetShadow(playerid, Elections[3][vtxd][playerid], 0);

	electionstextdraws[playerid][7] = CreatePlayerTextDraw(playerid, 187.999908, 59.748138, "box");
	PlayerTextDrawLetterSize(playerid, electionstextdraws[playerid][7], 0.000000, 4.766664);
	PlayerTextDrawTextSize(playerid, electionstextdraws[playerid][7], 449.000000, 0.000000);
	PlayerTextDrawAlignment(playerid, electionstextdraws[playerid][7], 1);
	PlayerTextDrawColor(playerid, electionstextdraws[playerid][7], -1);
	PlayerTextDrawUseBox(playerid, electionstextdraws[playerid][7], 1);
	PlayerTextDrawBoxColor(playerid, electionstextdraws[playerid][7], 255);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][7], 0);
	PlayerTextDrawSetOutline(playerid, electionstextdraws[playerid][7], 0);
	PlayerTextDrawBackgroundColor(playerid, electionstextdraws[playerid][7], 255);
	PlayerTextDrawFont(playerid, electionstextdraws[playerid][7], 1);
	PlayerTextDrawSetProportional(playerid, electionstextdraws[playerid][7], 1);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][7], 0);

	electionstextdraws[playerid][8] = CreatePlayerTextDraw(playerid, 277.000000, 76.755561, "Elections");
	PlayerTextDrawLetterSize(playerid, electionstextdraws[playerid][8], 0.400000, 1.600000);
	PlayerTextDrawAlignment(playerid, electionstextdraws[playerid][8], 1);
	PlayerTextDrawColor(playerid, electionstextdraws[playerid][8], -1);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][8], 0);
	PlayerTextDrawSetOutline(playerid, electionstextdraws[playerid][8], 0);
	PlayerTextDrawBackgroundColor(playerid, electionstextdraws[playerid][8], 255);
	PlayerTextDrawFont(playerid, electionstextdraws[playerid][8], 2);
	PlayerTextDrawSetProportional(playerid, electionstextdraws[playerid][8], 1);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][8], 0);

	electionstextdraws[playerid][9] = CreatePlayerTextDraw(playerid, 237.999969, 327.414794, "VOTE");
	PlayerTextDrawLetterSize(playerid, electionstextdraws[playerid][9], 0.282332, 1.077329);
	PlayerTextDrawTextSize(playerid, electionstextdraws[playerid][9], 269.000000, 7.077329);
	PlayerTextDrawAlignment(playerid, electionstextdraws[playerid][9], 1);
	PlayerTextDrawColor(playerid, electionstextdraws[playerid][9], -1);
	PlayerTextDrawUseBox(playerid, electionstextdraws[playerid][9], 1);
	PlayerTextDrawBoxColor(playerid, electionstextdraws[playerid][9], 255);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][9], 0);
	PlayerTextDrawSetOutline(playerid, electionstextdraws[playerid][9], 0);
	PlayerTextDrawBackgroundColor(playerid, electionstextdraws[playerid][9], 255);
	PlayerTextDrawFont(playerid, electionstextdraws[playerid][9], 2);
	PlayerTextDrawSetProportional(playerid, electionstextdraws[playerid][9], 1);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][9], 0);
	PlayerTextDrawSetSelectable(playerid, electionstextdraws[playerid][9], true);

	electionstextdraws[playerid][10] = CreatePlayerTextDraw(playerid, 353.999969, 327.170349, "VOTE");
	PlayerTextDrawLetterSize(playerid, electionstextdraws[playerid][10], 0.282999, 1.056591);
	PlayerTextDrawTextSize(playerid, electionstextdraws[playerid][10], 383.000000, 7.056591);
	PlayerTextDrawAlignment(playerid, electionstextdraws[playerid][10], 1);
	PlayerTextDrawColor(playerid, electionstextdraws[playerid][10], -1);
	PlayerTextDrawUseBox(playerid, electionstextdraws[playerid][10], 1);
	PlayerTextDrawBoxColor(playerid, electionstextdraws[playerid][10], 255);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][10], 0);
	PlayerTextDrawSetOutline(playerid, electionstextdraws[playerid][10], 0);
	PlayerTextDrawBackgroundColor(playerid, electionstextdraws[playerid][10], 255);
	PlayerTextDrawFont(playerid, electionstextdraws[playerid][10], 2);
	PlayerTextDrawSetProportional(playerid, electionstextdraws[playerid][10], 1);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][10], 0);
	PlayerTextDrawSetSelectable(playerid, electionstextdraws[playerid][10], true);

	electionstextdraws[playerid][13] = CreatePlayerTextDraw(playerid, 159.666641, 124.414810, "");
	PlayerTextDrawLetterSize(playerid, electionstextdraws[playerid][13], 0.000000, 0.000000);
	PlayerTextDrawTextSize(playerid, electionstextdraws[playerid][13], 90.000000, 90.000000);
	PlayerTextDrawAlignment(playerid, electionstextdraws[playerid][13], 1);
	PlayerTextDrawColor(playerid, electionstextdraws[playerid][13], -1);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][13], 0);
	PlayerTextDrawSetOutline(playerid, electionstextdraws[playerid][13], 0);
	PlayerTextDrawBackgroundColor(playerid, electionstextdraws[playerid][13], -256);
	PlayerTextDrawFont(playerid, electionstextdraws[playerid][13], 5);
	PlayerTextDrawSetProportional(playerid, electionstextdraws[playerid][13], 0);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][13], 0);
	PlayerTextDrawSetPreviewModel(playerid, electionstextdraws[playerid][13], Elections[0][eskinid]);
	PlayerTextDrawSetPreviewRot(playerid, electionstextdraws[playerid][13], 0.000000, 0.000000, 0.000000, 1.000000);

	Elections[0][ntxd][playerid] = CreatePlayerTextDraw(playerid, 225.666717, 109.525848, Elections[0][cname]);
	PlayerTextDrawLetterSize(playerid, Elections[0][ntxd][playerid], 0.159996, 1.313776);
	PlayerTextDrawAlignment(playerid, Elections[0][ntxd][playerid], 1);
	PlayerTextDrawColor(playerid, Elections[0][ntxd][playerid], -1);
	PlayerTextDrawSetShadow(playerid, Elections[0][ntxd][playerid], 0);
	PlayerTextDrawSetOutline(playerid, Elections[0][ntxd][playerid], -2);
	PlayerTextDrawBackgroundColor(playerid, Elections[0][ntxd][playerid], 255);
	PlayerTextDrawFont(playerid, Elections[0][ntxd][playerid], 2);
	PlayerTextDrawSetProportional(playerid, Elections[0][ntxd][playerid], 1);
	PlayerTextDrawSetShadow(playerid, Elections[0][ntxd][playerid], 0);

	Elections[1][ntxd][playerid] = CreatePlayerTextDraw(playerid, 340.000030, 109.585189, Elections[1][cname]);
	PlayerTextDrawLetterSize(playerid, Elections[1][ntxd][playerid], 0.154661, 1.413333);
	PlayerTextDrawAlignment(playerid, Elections[1][ntxd][playerid], 1);
	PlayerTextDrawColor(playerid, Elections[1][ntxd][playerid], -1);
	PlayerTextDrawSetShadow(playerid, Elections[1][ntxd][playerid], 0);
	PlayerTextDrawSetOutline(playerid, Elections[1][ntxd][playerid], -2);
	PlayerTextDrawBackgroundColor(playerid, Elections[1][ntxd][playerid], 255);
	PlayerTextDrawFont(playerid, Elections[1][ntxd][playerid], 2);
	PlayerTextDrawSetProportional(playerid, Elections[1][ntxd][playerid], 1);
	PlayerTextDrawSetShadow(playerid, Elections[1][ntxd][playerid], 0);

	electionstextdraws[playerid][16] = CreatePlayerTextDraw(playerid, 384.666625, 121.170372, "");
	PlayerTextDrawLetterSize(playerid, electionstextdraws[playerid][16], 0.000000, 0.000000);
	PlayerTextDrawTextSize(playerid, electionstextdraws[playerid][16], 90.000000, 90.000000);
	PlayerTextDrawAlignment(playerid, electionstextdraws[playerid][16], 1);
	PlayerTextDrawColor(playerid, electionstextdraws[playerid][16], -1);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][16], 0);
	PlayerTextDrawSetOutline(playerid, electionstextdraws[playerid][16], 0);
	PlayerTextDrawBackgroundColor(playerid, electionstextdraws[playerid][16], 0);
	PlayerTextDrawFont(playerid, electionstextdraws[playerid][16], 5);
	PlayerTextDrawSetProportional(playerid, electionstextdraws[playerid][16], 0);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][16], 0);
	PlayerTextDrawSetPreviewModel(playerid, electionstextdraws[playerid][16], Elections[1][eskinid]);
	PlayerTextDrawSetPreviewRot(playerid, electionstextdraws[playerid][16], 0.000000, 0.000000, 0.000000, 1.000000);

	format(elestr, sizeof(elestr), "Votes: %i", Elections[0][votes]);
	Elections[0][vtxd][playerid] = CreatePlayerTextDraw(playerid, 232.999969, 142.244430, elestr);
	PlayerTextDrawLetterSize(playerid, Elections[0][vtxd][playerid], 0.275999, 1.210072);
	PlayerTextDrawAlignment(playerid, Elections[0][vtxd][playerid], 1);
	PlayerTextDrawColor(playerid, Elections[0][vtxd][playerid], -1);
	PlayerTextDrawSetShadow(playerid, Elections[0][vtxd][playerid], 0);
	PlayerTextDrawSetOutline(playerid, Elections[0][vtxd][playerid], -2);
	PlayerTextDrawBackgroundColor(playerid, Elections[0][vtxd][playerid], 255);
	PlayerTextDrawFont(playerid, Elections[0][vtxd][playerid], 2);
	PlayerTextDrawSetProportional(playerid, Elections[0][vtxd][playerid], 1);
	PlayerTextDrawSetShadow(playerid, Elections[0][vtxd][playerid], 0);

	format(elestr, sizeof(elestr), "Votes: %i", Elections[1][votes]);
	Elections[1][vtxd][playerid] = CreatePlayerTextDraw(playerid, 340.666687, 141.414825, elestr);
	PlayerTextDrawLetterSize(playerid, Elections[1][vtxd][playerid], 0.280665, 1.255704);
	PlayerTextDrawAlignment(playerid, Elections[1][vtxd][playerid], 1);
	PlayerTextDrawColor(playerid, Elections[1][vtxd][playerid], -1);
	PlayerTextDrawSetShadow(playerid, Elections[1][vtxd][playerid], 0);
	PlayerTextDrawSetOutline(playerid, Elections[1][vtxd][playerid], -2);
	PlayerTextDrawBackgroundColor(playerid, Elections[1][vtxd][playerid], 255);
	PlayerTextDrawFont(playerid, Elections[1][vtxd][playerid], 2);
	PlayerTextDrawSetProportional(playerid, Elections[1][vtxd][playerid], 1);
	PlayerTextDrawSetShadow(playerid, Elections[1][vtxd][playerid], 0);

	electionstextdraws[playerid][21] = CreatePlayerTextDraw(playerid, 354.000061, 201.451995, "VOTE");
	PlayerTextDrawLetterSize(playerid, electionstextdraws[playerid][21], 0.293332, 1.205924);
	PlayerTextDrawTextSize(playerid, electionstextdraws[playerid][21], 383.000000, 7.205924);
	PlayerTextDrawAlignment(playerid, electionstextdraws[playerid][21], 1);
	PlayerTextDrawColor(playerid, electionstextdraws[playerid][21], -1);
	PlayerTextDrawUseBox(playerid, electionstextdraws[playerid][21], 1);
	PlayerTextDrawBoxColor(playerid, electionstextdraws[playerid][21], 255);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][21], 0);
	PlayerTextDrawSetOutline(playerid, electionstextdraws[playerid][21], 0);
	PlayerTextDrawBackgroundColor(playerid, electionstextdraws[playerid][21], 255);
	PlayerTextDrawFont(playerid, electionstextdraws[playerid][21], 2);
	PlayerTextDrawSetProportional(playerid, electionstextdraws[playerid][21], 1);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][21], 0);
	PlayerTextDrawSetSelectable(playerid, electionstextdraws[playerid][21], true);

	electionstextdraws[playerid][22] = CreatePlayerTextDraw(playerid, 187.999954, 110.770492, "box");
	PlayerTextDrawLetterSize(playerid, electionstextdraws[playerid][22], 0.000000, 26.366670);
	PlayerTextDrawTextSize(playerid, electionstextdraws[playerid][22], 188.000000, 0.000000);
	PlayerTextDrawAlignment(playerid, electionstextdraws[playerid][22], 1);
	PlayerTextDrawColor(playerid, electionstextdraws[playerid][22], -1);
	PlayerTextDrawUseBox(playerid, electionstextdraws[playerid][22], 1);
	PlayerTextDrawBoxColor(playerid, electionstextdraws[playerid][22], 255);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][22], 0);
	PlayerTextDrawSetOutline(playerid, electionstextdraws[playerid][22], 0);
	PlayerTextDrawBackgroundColor(playerid, electionstextdraws[playerid][22], 255);
	PlayerTextDrawFont(playerid, electionstextdraws[playerid][22], 3);
	PlayerTextDrawSetProportional(playerid, electionstextdraws[playerid][22], 1);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][22], 0);

	electionstextdraws[playerid][23] = CreatePlayerTextDraw(playerid, 448.333282, 110.355613, "box");
	PlayerTextDrawLetterSize(playerid, electionstextdraws[playerid][23], 0.000000, 26.433334);
	PlayerTextDrawTextSize(playerid, electionstextdraws[playerid][23], 449.000000, 0.000000);
	PlayerTextDrawAlignment(playerid, electionstextdraws[playerid][23], 1);
	PlayerTextDrawColor(playerid, electionstextdraws[playerid][23], -1);
	PlayerTextDrawUseBox(playerid, electionstextdraws[playerid][23], 1);
	PlayerTextDrawBoxColor(playerid, electionstextdraws[playerid][23], 255);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][23], 0);
	PlayerTextDrawSetOutline(playerid, electionstextdraws[playerid][23], 0);
	PlayerTextDrawBackgroundColor(playerid, electionstextdraws[playerid][23], 255);
	PlayerTextDrawFont(playerid, electionstextdraws[playerid][23], 3);
	PlayerTextDrawSetProportional(playerid, electionstextdraws[playerid][23], 1);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][23], 0);

	electionstextdraws[playerid][24] = CreatePlayerTextDraw(playerid, 191.333419, 353.022277, "box");
	PlayerTextDrawLetterSize(playerid, electionstextdraws[playerid][24], 0.000000, -0.533334);
	PlayerTextDrawTextSize(playerid, electionstextdraws[playerid][24], 447.000000, 0.000000);
	PlayerTextDrawAlignment(playerid, electionstextdraws[playerid][24], 1);
	PlayerTextDrawColor(playerid, electionstextdraws[playerid][24], -1);
	PlayerTextDrawUseBox(playerid, electionstextdraws[playerid][24], 1);
	PlayerTextDrawBoxColor(playerid, electionstextdraws[playerid][24], 255);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][24], 0);
	PlayerTextDrawSetOutline(playerid, electionstextdraws[playerid][24], 0);
	PlayerTextDrawBackgroundColor(playerid, electionstextdraws[playerid][24], 255);
	PlayerTextDrawFont(playerid, electionstextdraws[playerid][24], 1);
	PlayerTextDrawSetProportional(playerid, electionstextdraws[playerid][24], 1);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][24], 0);

	electionstextdraws[playerid][25] = CreatePlayerTextDraw(playerid, 240.999588, 201.859283, "VOTE");
	PlayerTextDrawLetterSize(playerid, electionstextdraws[playerid][25], 0.293332, 1.205924);
	PlayerTextDrawTextSize(playerid, electionstextdraws[playerid][25], 270.000000, 7.205924);
	PlayerTextDrawAlignment(playerid, electionstextdraws[playerid][25], 1);
	PlayerTextDrawColor(playerid, electionstextdraws[playerid][25], -1);
	PlayerTextDrawUseBox(playerid, electionstextdraws[playerid][25], 1);
	PlayerTextDrawBoxColor(playerid, electionstextdraws[playerid][25], 255);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][25], 0);
	PlayerTextDrawSetOutline(playerid, electionstextdraws[playerid][25], 0);
	PlayerTextDrawBackgroundColor(playerid, electionstextdraws[playerid][25], 255);
	PlayerTextDrawFont(playerid, electionstextdraws[playerid][25], 2);
	PlayerTextDrawSetProportional(playerid, electionstextdraws[playerid][25], 1);
	PlayerTextDrawSetShadow(playerid, electionstextdraws[playerid][25], 0);
	PlayerTextDrawSetSelectable(playerid, electionstextdraws[playerid][25], true);


	viewingelections[playerid] = false;
	HideElectionsAll(playerid);
	
	return 1;
}


Hook:EL_OnPlayerDisconnect(playerid, reason)	
{
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][0]);	
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][1]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][2]);
	PlayerTextDrawDestroy(playerid, Elections[0][ntxd]);
	PlayerTextDrawDestroy(playerid, Elections[1][ntxd]);
	PlayerTextDrawDestroy(playerid, Elections[2][ntxd]);
	PlayerTextDrawDestroy(playerid, Elections[3][ntxd]);
	PlayerTextDrawDestroy(playerid, Elections[0][vtxd]);
	PlayerTextDrawDestroy(playerid, Elections[1][vtxd]);
	PlayerTextDrawDestroy(playerid, Elections[2][vtxd]);
	PlayerTextDrawDestroy(playerid, Elections[3][vtxd]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][7]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][8]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][9]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][10]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][13]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][16]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][21]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][22]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][23]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][24]);
	PlayerTextDrawDestroy(playerid, electionstextdraws[playerid][25]);
	return 1;
}

Hook:ELECTIONS_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_ELECTIONS_ONE)
	{
		if(response)
		{
			new candidates = strval(inputtext);

			if(candidates < 2 || candidates > 4)
			{
				ShowPlayerDialog(playerid, DIALOG_ELECTIONS_ONE, DIALOG_STYLE_INPUT, "{FFFFFF}Elections - Part One", "{FFFFFF}How many candidates would you like to have? (2-4)", "Next", "Cancel");
				SendClientMessage(playerid, COLOR_GREY, "The candidates number must not be less than 2 or more than 4.");
				return 1;
			}

			pcandidates[playerid] = candidates;
			ShowPlayerDialog(playerid, DIALOG_ELECTIONS_TWO, DIALOG_STYLE_INPUT, "{FFFFFF}Elections - Part Two", "{FFFFFF}Enter the name for your first candidate.", "Next", "Cancel");

		}
		
	}
	if(dialogid == DIALOG_ELECTIONS_TWO)
	{
		if(response)
		{
			if(strlen(inputtext) < 3 || strlen(inputtext) > 24)
			{
				ShowPlayerDialog(playerid, DIALOG_ELECTIONS_TWO, DIALOG_STYLE_INPUT, "{FFFFFF}Elections - Part Two", "{FFFFFF}Enter the name for your first candidate.", "Next", "Cancel");
				SendClientMessage(playerid, COLOR_GREY, "The candidate name must be between 3-24 characters.");
				return 1;
			}

			format(pcandidate1name[playerid], 24, "%s", inputtext);
			ShowPlayerDialog(playerid, DIALOG_ELECTIONS_THREE, DIALOG_STYLE_INPUT, "{FFFFFF}Elections - Part Three", "{FFFFFF}Enter the skin ID for your first candidate.", "Next", "Cancel");

		}
	}
	if(dialogid == DIALOG_ELECTIONS_THREE)
	{
		if(response)
		{
			new skinid = strval(inputtext);
			if(skinid < 1 || skinid > 311)
			{
				ShowPlayerDialog(playerid, DIALOG_ELECTIONS_THREE, DIALOG_STYLE_INPUT, "{FFFFFF}Elections - Part Three", "{FFFFFF}Enter the skin ID for your first candidate.", "Next", "Cancel");
				SendClientMessage(playerid, COLOR_GREY, "The candidat's skin ID must be between 1-311.");
				return 1;
			}

			pcandidate1skin[playerid] = skinid;
			ShowPlayerDialog(playerid, DIALOG_ELECTIONS_FOUR, DIALOG_STYLE_INPUT, "{FFFFFF}Elections - Part Four", "{FFFFFF}Enter the name for your second candidate.", "Next", "Cancel");
		}
	}
	if(dialogid == DIALOG_ELECTIONS_FOUR)
	{
		if(response)
		{
			if(strlen(inputtext) < 3 || strlen(inputtext) > 24)
			{
				ShowPlayerDialog(playerid, DIALOG_ELECTIONS_FOUR, DIALOG_STYLE_INPUT, "{FFFFFF}Elections - Part Four", "{FFFFFF}Enter the name for your second candidate.", "Next", "Cancel");
				SendClientMessage(playerid, COLOR_GREY, "The candidate name must be between 3-24 characters.");
				return 1;
			}

			format(pcandidate2name[playerid], 24, "%s", inputtext);
			ShowPlayerDialog(playerid, DIALOG_ELECTIONS_FIVE, DIALOG_STYLE_INPUT, "{FFFFFF}Elections - Part Five", "{FFFFFF}Enter the skin ID for your second candidate.", "Next", "Cancel");
		}
	}
	if(dialogid == DIALOG_ELECTIONS_FIVE)
	{
		if(response)
		{
			new skinid = strval(inputtext);
			if(skinid < 1 || skinid > 311)
			{
				ShowPlayerDialog(playerid, DIALOG_ELECTIONS_FIVE, DIALOG_STYLE_INPUT, "{FFFFFF}Elections - Part Five", "{FFFFFF}Enter the skin ID for your second candidate.", "Next", "Cancel");
				SendClientMessage(playerid, COLOR_GREY, "The candidat's skin ID must be between 1-311.");
				return 1;
			}

			pcandidate2skin[playerid] = skinid;

			if(pcandidates[playerid] == 2)
			{				
				ElectionsCandidates = pcandidates[playerid];
				ElectionsActive = true;
				StartElections(playerid, ElectionsCandidates);
			}

			if(pcandidates[playerid] > 2)
			{
				ShowPlayerDialog(playerid, DIALOG_ELECTIONS_SIX, DIALOG_STYLE_INPUT, "{FFFFFF}Elections - Part Six", "{FFFFFF}Enter the name for your third candidate.", "Next", "Cancel");
			}
		}
	}
	if(dialogid == DIALOG_ELECTIONS_SIX)
	{
		if(response)
		{
			if(strlen(inputtext) < 3 || strlen(inputtext) > 24)
			{
				ShowPlayerDialog(playerid, DIALOG_ELECTIONS_SIX, DIALOG_STYLE_INPUT, "{FFFFFF}Elections - Part Six", "{FFFFFF}Enter the name for your third candidate.", "Next", "Cancel");
				SendClientMessage(playerid, COLOR_GREY, "The candidate name must be between 3-24 characters.");
				return 1;
			}

			format(pcandidate3name[playerid], 24, "%s", inputtext);
			ShowPlayerDialog(playerid, DIALOG_ELECTIONS_SEVEN, DIALOG_STYLE_INPUT, "{FFFFFF}Elections - Part Seven", "{FFFFFF}Enter the skin ID for your third candidate.", "Next", "Cancel");
		}
	}
	if(dialogid == DIALOG_ELECTIONS_SEVEN)
	{
		if(response)
		{
			new skinid = strval(inputtext);
			if(skinid < 1 || skinid > 311)
			{
				ShowPlayerDialog(playerid, DIALOG_ELECTIONS_SEVEN, DIALOG_STYLE_INPUT, "{FFFFFF}Elections - Part Seven", "{FFFFFF}Enter the skin ID for your third candidate.", "Next", "Cancel");
				SendClientMessage(playerid, COLOR_GREY, "The candidat's skin ID must be between 1-311.");
				return 1;
			}

			pcandidate3skin[playerid] = skinid;

			if(pcandidates[playerid] == 3)
			{
				ElectionsCandidates = pcandidates[playerid];
				ElectionsActive = true;
				StartElections(playerid, ElectionsCandidates);
			}

			if(pcandidates[playerid] > 3)
			{
				ShowPlayerDialog(playerid, DIALOG_ELECTIONS_EIGHT, DIALOG_STYLE_INPUT, "{FFFFFF}Elections - Part Eight", "{FFFFFF}Enter the name for your fourth candidate.", "Next", "Cancel");	
			}
		}
	}
	if(dialogid == DIALOG_ELECTIONS_EIGHT)
	{
		if(response)
		{
			if(strlen(inputtext) < 3 || strlen(inputtext) > 24)
			{
				ShowPlayerDialog(playerid, DIALOG_ELECTIONS_EIGHT, DIALOG_STYLE_INPUT, "{FFFFFF}Elections - Part Eight", "{FFFFFF}Enter the name for your fourth candidate.", "Next", "Cancel");
				SendClientMessage(playerid, COLOR_GREY, "The candidate name must be between 3-24 characters.");
				return 1;
			}

			format(pcandidate4name[playerid], 24, "%s", inputtext);
			ShowPlayerDialog(playerid, DIALOG_ELECTIONS_NINE, DIALOG_STYLE_INPUT, "{FFFFFF}Elections - Part Nine", "{FFFFFF}Enter the skin ID for your fourth candidate.", "Done", "Cancel");
		}
	}
	if(dialogid == DIALOG_ELECTIONS_NINE)
	{
		if(response)
		{
			new skinid = strval(inputtext);
			if(skinid < 1 || skinid > 311)
			{
				ShowPlayerDialog(playerid, DIALOG_ELECTIONS_NINE, DIALOG_STYLE_INPUT, "{FFFFFF}Elections - Part Nine", "{FFFFFF}Enter the skin ID for your fourth candidate.", "Next", "Cancel");
				SendClientMessage(playerid, COLOR_GREY, "The candidat's skin ID must be between 1-311.");
				return 1;
			}

			ElectionsCandidates = pcandidates[playerid];
			ElectionsActive = true;
			pcandidate4skin[playerid] = skinid;
			StartElections(playerid, ElectionsCandidates);
		}
	}
	return 1;
}

CMD:startelections(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 2 && PlayerInfo[playerid][pDev] < 2) return AdmErrorMsg;
	if(ElectionsActive == true) return SendClientMessage(playerid, COLOR_GREY, "Elections are already active! /endelections to end them.");

	ShowPlayerDialog(playerid, DIALOG_ELECTIONS_ONE, DIALOG_STYLE_INPUT, "{FFFFFF}Elections - Part One", "{FFFFFF}How many candidates would you like to have? (2-4)", "Next", "Cancel");
	
	return 1;
}

CMD:endelections(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 2 && PlayerInfo[playerid][pDev] < 2) return AdmErrorMsg;
	if(ElectionsActive == false) return SendClientMessage(playerid, COLOR_GREY, "Elections are not active, /startelections to begin.");

	ElectionsActive = false;
	format(elestr, sizeof(elestr), "%s has ended the elections poll.", PlayerOOCName(playerid));
	SendClientMessageToAll(COLOR_LIGHTBLUE, elestr);

	foreach( new i: Player )
	{
		viewingelections[i] = false;
		HideElectionsAll(i);
		CancelSelectTextDraw(i);
	}
	return 1;
}

CMD:viewelections(playerid, params[]) // view only, can't  vote (vote textdraws don't display)
{
	if(!ElectionsReadable) return SendClientMessage(playerid, COLOR_GREY, "There are no active elections.");
	if(viewingelections[playerid] == false)
	{
		SendClientMessage(playerid, COLOR_GREY, "Use /viewelections again to turn off the elections box, use /evote to vote.");
		viewingelections[playerid] = true;

		PlayerTextDrawShow(playerid, electionstextdraws[playerid][0]);	
		PlayerTextDrawShow(playerid, electionstextdraws[playerid][16]);

		PlayerTextDrawShow(playerid, Elections[0][ntxd]);
		PlayerTextDrawShow(playerid, Elections[0][vtxd]);

		PlayerTextDrawShow(playerid, Elections[1][ntxd]);
		PlayerTextDrawShow(playerid, Elections[1][vtxd]);

		if(ElectionsCandidates > 2)
		{
			PlayerTextDrawShow(playerid, Elections[2][ntxd]);
			PlayerTextDrawShow(playerid, Elections[2][vtxd]);
			PlayerTextDrawShow(playerid, electionstextdraws[playerid][1]);		
		}

		if(ElectionsCandidates > 3)
		{
			PlayerTextDrawShow(playerid, Elections[3][ntxd]);
			PlayerTextDrawShow(playerid, Elections[3][vtxd]);
			PlayerTextDrawShow(playerid, electionstextdraws[playerid][2]);
		}


		PlayerTextDrawShow(playerid, electionstextdraws[playerid][7]);
		PlayerTextDrawShow(playerid, electionstextdraws[playerid][8]);
		PlayerTextDrawShow(playerid, electionstextdraws[playerid][13]);
		PlayerTextDrawShow(playerid, electionstextdraws[playerid][22]);
		PlayerTextDrawShow(playerid, electionstextdraws[playerid][23]);
		PlayerTextDrawShow(playerid, electionstextdraws[playerid][24]);
		CancelSelectTextDraw(playerid);
	}
	else if(viewingelections[playerid] == true)
	{
		viewingelections[playerid] = false;
		HideElectionsAll(playerid);
		CancelSelectTextDraw(playerid);
	}
	return 1;
}

CMD:evote(playerid, params[])
{
	if(ElectionsActive == false) return SendClientMessage(playerid, COLOR_GREY, "There are no active elections.");
	if(PlayerInfo[playerid][pLevel] < 5) return SendClientMessage(playerid, COLOR_GREY, "You may only vote as a level 5+");

	if(!IsPlayerInRangeOfPoint(playerid, 5.0, -2108.4446, 764.0786, 9006.0859) && !IsPlayerInRangeOfPoint(playerid, 5.0, -2205.0752, 837.4177, 9999.2656))
		return SendClientMessage(playerid, COLOR_GREY, "You're not at the LSPD front desk or the city hall service desk.");

	if(gettime() < VoteTimer[playerid] + 5) return SendClientMessage(playerid, COLOR_GREY, "Please wait 5 seconds before using /evote again.");

	new query[128];
	VoteTimer[playerid] = gettime();
	mysql_format(sqlGameConnection, query, sizeof query, "SELECT `ID` FROM voters WHERE `ID` = %d", PlayerInfo[playerid][pID]);
	mysql_pquery(sqlGameConnection, query, "OnVoteCheck", "i", playerid);

	return 1;
}

public OnVoteCheck(playerid)
{

	if(cache_num_rows() != 0) return SendClientMessage(playerid, COLOR_GREY, "You already voted on the elections!");

	viewingelections[playerid] = true;
	PlayerTextDrawShow(playerid, electionstextdraws[playerid][0]);	

	PlayerTextDrawShow(playerid, Elections[0][ntxd]);
	PlayerTextDrawShow(playerid, Elections[0][vtxd]);
	PlayerTextDrawShow(playerid, electionstextdraws[playerid][25]); // VOTE BUTTON

	PlayerTextDrawShow(playerid, Elections[1][ntxd]);
	PlayerTextDrawShow(playerid, Elections[1][vtxd]);
	PlayerTextDrawShow(playerid, electionstextdraws[playerid][21]); // VOTE BUTTON

	if(ElectionsCandidates > 2)
	{
		PlayerTextDrawShow(playerid, Elections[2][ntxd]);
		PlayerTextDrawShow(playerid, Elections[2][vtxd]);
		PlayerTextDrawShow(playerid, electionstextdraws[playerid][9]); // VOTE BUTTON
		PlayerTextDrawShow(playerid, electionstextdraws[playerid][1]);
	}	

	if(ElectionsCandidates > 3)
	{
		PlayerTextDrawShow(playerid, Elections[3][ntxd]);
		PlayerTextDrawShow(playerid, Elections[3][vtxd]);
		PlayerTextDrawShow(playerid, electionstextdraws[playerid][10]); // VOTE BUTTON
		PlayerTextDrawShow(playerid, electionstextdraws[playerid][2]);
	}

	PlayerTextDrawShow(playerid, electionstextdraws[playerid][7]);
	PlayerTextDrawShow(playerid, electionstextdraws[playerid][8]);

	PlayerTextDrawShow(playerid, electionstextdraws[playerid][13]);
	PlayerTextDrawShow(playerid, electionstextdraws[playerid][16]);

	PlayerTextDrawShow(playerid, electionstextdraws[playerid][22]);
	PlayerTextDrawShow(playerid, electionstextdraws[playerid][23]);
	PlayerTextDrawShow(playerid, electionstextdraws[playerid][24]);
	
	SendClientMessage(playerid, COLOR_GREY, "You may now vote for your candidate. Use /viewelections to turn off the elections box.");

	SelectTextDraw(playerid, 0x00FF00FF);

	return 1;
}

public LogVote(playerid, candidate)
{
	new query[128];
	mysql_format(sqlGameConnection, query, sizeof(query), "INSERT INTO `voters` (`ID`, `votedFor`) VALUES('%d', '%e')", PlayerInfo[playerid][pID], Elections[candidate][cname]);
	mysql_query(sqlGameConnection, query);
	return 1;
}