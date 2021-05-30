stock HasPermission(playerid)
{
	if(PlayerInfo[playerid][pMember] < 9 || PlayerInfo[playerid][pMember] > 9) return SendClientMessage(playerid, COLOR_GREY, "Not authorized");
	return 1;
}
enum Newspaper
{
   Line1[255],
   Line2[255],
   Line3[255],
   Line4[255],
   Line5[255],
   NewsPublish,
}
new NewsInfo[Newspaper];


/*	
penum
	pNote1[255],
	pNote2[255],
	pNote3[255],
	pNote4[255],
	pNote5[255],*/


CMD:editnews(playerid, params[])
{
   new give[5], text[255];
   new string1[255];
   if(HasPermission(playerid))
   if(NewsInfo[NewsPublish] != 0) return SendClientMessage(playerid, COLOR_GREY, "There's already one newspaper released, wait an hour to release one more.");
   if(sscanf(params, "s[5]s[255]", give, text)) return SendClientMessage(playerid, COLOR_WHITE, "{00BFFF}Usage: {ffffff}/editnews [1 - 5] [TEXT]");
   if(!strcmp(give, "1", true))
   {
	  NewsInfo[Line1] = text;
	  format(string1, sizeof(string1), "* %s reaches for their pen and begins writing an article.", PlayerICName(playerid));
      ProxDetector(30.0, playerid, string1, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
   }
   else if(!strcmp(give, "2", true))
   {
     NewsInfo[Line2] = text;
     format(string1, sizeof(string1), "* %s reaches for their pen and begins writing an article.", PlayerICName(playerid));
     ProxDetector(30.0, playerid, string1, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
   }
   else if(!strcmp(give, "3", true))
   {
     NewsInfo[Line3] = text;
     format(string1, sizeof(string1), "* %s reaches for their pen and begins writing an article.", PlayerICName(playerid));
     ProxDetector(30.0, playerid, string1, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
   }
   else if(!strcmp(give, "4", true))
   {
     NewsInfo[Line4] = text;
     format(string1, sizeof(string1), "* %s reaches for their pen and begins writing an article.", PlayerICName(playerid));
     ProxDetector(30.0, playerid, string1, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
   }
   else if(!strcmp(give, "5", true))
   {
     NewsInfo[Line5] = text;
     format(string1, sizeof(string1), "* %s reaches for their pen and begins writing an article.", PlayerICName(playerid));
     ProxDetector(30.0, playerid, string1, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
   }
   return 1;
}
CMD:viewnews(playerid, params[])
{
   new dialogstring[1000];
   new string1[255];
   if(NewsInfo[NewsPublish] != 1) return SendClientMessage(playerid, COLOR_GREY, "Error: Newspaper is not published yet.");
   format(dialogstring, sizeof(dialogstring), "  {00BFFF}NEWS{ffffff}: %s\n\n\n {00BFFF}NEWS{ffffff}: %s\n\n\n {00BFFF}NEWS{ffffff}: %s\n\n\n {00BFFF}NEWS{ffffff}: %s\n\n\n {00BFFF}NEWS{ffffff}: %s", NewsInfo[Line1],
   NewsInfo[Line2],
   NewsInfo[Line3],
   NewsInfo[Line4],
   NewsInfo[Line5]);

   ShowPlayerDialog(playerid, 75, DIALOG_STYLE_MSGBOX, "{ffffff}NEWSPAPER", dialogstring, "OK", "");
   format(string1, sizeof(string1), "* %s reaches for their phone and begins checking the latest news on it.", PlayerICName(playerid));
   ProxDetector(30.0, playerid, string1, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
   return 1;
}
CMD:publishnews(playerid, params[])
{
   if(HasPermission(playerid))
   SendClientMessageToAll(-1, "Daily Newspaper has been published {808040}(/viewnews){ffffff} to check it!");
   NewsInfo[NewsPublish] = 1;
   return 1;
}

CMD:clearnews(playerid, params[])
{
   new string1[255];
   if(PlayerInfo[playerid][pAdmin] > 1)
   {
     format(string1, sizeof(string1), " ");
     NewsInfo[Line1] = string1;
     NewsInfo[Line2] = string1;
     NewsInfo[Line3] = string1;
     NewsInfo[Line4] = string1;
     NewsInfo[Line5] = string1;
     NewsInfo[NewsPublish] = 0;
     SendClientMessage(playerid, COLOR_GREY, "SANews newspaper was cleared!");
   }
   return 1;
}











CMD:editnote(playerid, params[])
{
   new give[5];
   new string[255];
   new string1[255];
   if(sscanf(params, "s[5]s[255]", give, string)) return SendClientMessage(playerid, COLOR_WHITE, "{00BFFF}Usage: {ffffff}/editnote [1 - 5] [NOTE]");
   if(!strcmp(give, "1", true))
   {
	  PlayerInfo[playerid][pNote1] = string;
      SendClientMessage(playerid, COLOR_GREY, "You've edited notepad page(1).");

      format(string1, sizeof(string1), "* %s pulls out their notepad and writes something on it.", PlayerICName(playerid));
      ProxDetector(30.0, playerid, string1, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
   }
   else if(!strcmp(give, "2", true))
   {
      PlayerInfo[playerid][pNote2] = string;
      SendClientMessage(playerid, COLOR_GREY, "You've edited notepad page(2).");

      format(string1, sizeof(string1), "* %s pulls out their notepad and writes something on it.", PlayerICName(playerid));
      ProxDetector(30.0, playerid, string1, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
   }
   else if(!strcmp(give, "3", true))
   {
      PlayerInfo[playerid][pNote3] = string;
      SendClientMessage(playerid, COLOR_GREY, "You've edited notepad page(3).");

      format(string1, sizeof(string1), "* %s pulls out their notepad and writes something on it.", PlayerICName(playerid));
      ProxDetector(30.0, playerid, string1, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
   }
   else if(!strcmp(give, "4", true))
   {
      PlayerInfo[playerid][pNote4] = string;
      SendClientMessage(playerid, COLOR_GREY, "You've edited notepad page(4).");

      format(string1, sizeof(string1), "* %s pulls out their notepad and writes something on it.", PlayerICName(playerid));
      ProxDetector(30.0, playerid, string1, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
   }
   else if(!strcmp(give, "5", true))
   {
      PlayerInfo[playerid][pNote5] = string;
      SendClientMessage(playerid, COLOR_GREY, "You've edited notepad page(5).");

      format(string1, sizeof(string1), "* %s pulls out their notepad and writes something on it.", PlayerICName(playerid));
      ProxDetector(30.0, playerid, string1, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
   }
   return 1;
}

CMD:notes(playerid, params[])
{
   new dialogstring[1000];
   new string1[255];
   format(dialogstring, sizeof(dialogstring), "{ffffff} Page 1: %s\n\n Page 2: %s\n\n Page 3: %s\n\n Page 4: %s\n\n Page 5: %s", PlayerInfo[playerid][pNote1],
   PlayerInfo[playerid][pNote2],
   PlayerInfo[playerid][pNote3],
   PlayerInfo[playerid][pNote4],
   PlayerInfo[playerid][pNote5]);

   ShowPlayerDialog(playerid, 75, DIALOG_STYLE_MSGBOX, "{ffffff}~~~~~~~~~~Notepad~~~~~~~~~", dialogstring, "OK", "");
   format(string1, sizeof(string1), "* %s pulls out their notepad and begins to read it.", PlayerICName(playerid));
   ProxDetector(30.0, playerid, string1, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
   return 1;
}

