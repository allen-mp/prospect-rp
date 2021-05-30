
#if defined SARPUTIL_INCLUDED
    #endinput
#endif

#define SARPUTIL_INCLUDED

// Define the namespace
#define Util:	Util_

#define Timer:%1(%2)	forward Timer_%1(%2);public Timer_%1(%2)
#define SQL:%1(%2)		forward SQL_%1(%2);public SQL_%1(%2)

stock Util:PlayerName(playerid)
{
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	return name;
}

stock Util:PlayerICName(playerid)
{
	new name[MAX_PLAYER_NAME];
    strmid(name, Util:str_replace('_', ' ', Util:PlayerName(playerid)), 0, MAX_PLAYER_NAME);
	return name;
}

stock Util:str_replace(sSearch, sReplace, const sSubject[], &iCount = 0)
{
	#pragma unused iCount
	new sReturn[128];
	format(sReturn, sizeof(sReturn), sSubject);
	for(new i = 0; i < sizeof(sReturn); i++)
	{
		if(sReturn[i] == sSearch)
		{
			sReturn[i] = sReplace;
		}
	}
	return sReturn;
}