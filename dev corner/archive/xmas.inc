/*CMD:gotospawn(playerid)
{
     SetPlayerPos(playerid, 2192.4468, -1819.0150, 18.8672);
     return 1;
}
CMD:gotolv(playerid)
{
     SetPlayerPos(playerid, 2931.8774,2119.2739,20.3906);
     return 1;
}
CMD:gotosf(playerid)
{
     SetPlayerPos(playerid, -1749.6079,188.8713,34.9856);
     return 1;
}
CMD:gotorc(playerid)
{
     SetPlayerPos(playerid, 2127.2258,-364.0407,59.5466);
     return 1;
}
CMD:gotofc(playerid)
{
     SetPlayerPos(playerid, -709.9302,-1727.5803,47.6867);
     return 1;
}
CMD:gotobs(playerid)
{
     SetPlayerPos(playerid, -2225.3049,2316.4102,7.9918);
     return 1;
}
CMD:gotoaa(playerid)
{
     SetPlayerPos(playerid, 144.0808,2616.9910,17.7420);
     return 1;
}
CMD:gototbe(playerid)
{
     SetPlayerPos(playerid, -261.8146,1859.4540,43.8891);
     return 1;
}
CMD:gotochi(playerid)
{
     SetPlayerPos(playerid, -2431.3525,-1619.5646,528.2825);
     return 1;
}
 
CMD:gotols(playerid)
{
     SetPlayerPos(playerid, 2769.5032, -1823.0596, 32.2693);
     return 1;
}
*/
#define COLOR_DARKBLUE 0x0059FFFF
#define COLOR_DARKGREEN 0x2D9402FF
#define COLOR_GREY 0xAFAFAFAA
#define COLOR_RED 0xFF0000AA
#define COLOR_ICE 0x7becf2
#define COLOR_DARK_ICE 0x42ebf4
#define CHRISTMAS_INV_DIALOG 32766
#define CHRISTMAS_WELCOME_DIALOG 32764
#define CHRISTMAS_END_DIALOG 32764
#define CHRISTMAS_LV 32763
#define CHRISTMAS_SF 32762
#define CHRISTMAS_RC 32761
#define CHRISTMAS_FC 32760
#define CHRISTMAS_BS 32759
#define CHRISTMAS_AA 32758
#define CHRISTMAS_TBE 32757
#define CHRISTMAS_CHI 32756
#define CHRISTMAS_LS 32755
 
#define XmasTreePointX 2192.4468
#define XmasTreePointY -1819.0150
#define XmasTreePointZ 18.8672
 
enum xmasInv
{
    PLAYER_SACK,
    LV_PRESENT,
    SF_PRESENT,
    RC_PRESENT,
    FC_PRESENT,
    BS_PRESENT,
    AA_PRESENT,
    TBE_PRESENT,
    CHI_PRESENT,
    LS_PRESENT,
    FINISHED_EVENT
};

new PlayerXmas[MAX_PLAYERS][xmasInv]
xOnGameModeInit()
{
	MySQL_createtable();
	CreateObject(19057, 2931.8774,2119.2739,18.01, 0, 0, 0); //lv present
    CreateObject(19055, -1749.6079,188.8713,32.4856, 0, 0, 0); //sf present
    CreateObject(19057, 2127.2258,-364.0407,57.1466, 0, 0, 0); //rc present
    CreateObject(19054, -709.9302,-1727.5803,45.2867, 0, 0, 0); //fc present
    CreateObject(19055, -2225.3049,2316.4102,4.5918, 0, 0, 0); //bs present
    CreateObject(19056, 144.0808,2616.9910,16.2420, 0, 0, 0); //aa present
    CreateObject(19057, -261.8146,1859.4540,41.8891, 0, 0, 0); //tbe present
    CreateObject(19054, -2431.3525,-1619.5646,525.9825, -14, 7, -9); //chilliad present
    CreateObject(19056, 2769.5032, -1823.0596, 30.2693, 5, -10, 0); //ls present
    Create3DTextLabel("Type /christmas to begin!", COLOR_WHITE, 2192.4468, -1819.0150, 18.8672, 20, 0, 1);
  	print("xOnGameModeInit");
}

xOnPlayerDisconnect(playerid, reason)
{
	#pragma unused reason
	MySQL_Save_ChristmasEvent(playerid);
	PlayerXmas[playerid][PLAYER_SACK] = 0;
	PlayerXmas[playerid][LV_PRESENT] = 0;
	PlayerXmas[playerid][SF_PRESENT] = 0;
	PlayerXmas[playerid][RC_PRESENT] = 0;
	PlayerXmas[playerid][FC_PRESENT] = 0;
	PlayerXmas[playerid][BS_PRESENT] = 0;
	PlayerXmas[playerid][AA_PRESENT] = 0;
	PlayerXmas[playerid][TBE_PRESENT] = 0;
	PlayerXmas[playerid][CHI_PRESENT] = 0;
	PlayerXmas[playerid][LS_PRESENT] = 0;
	PlayerXmas[playerid][FINISHED_EVENT] = 0;
}

xOnPlayerLogin(playerid)
{
	new query[100];
	mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM xmasevent2016 WHERE ID = %d", PlayerInfo[playerid][pID]);
	mysql_pquery(sqlGameConnection, query, "MySQL_Load_ChristmasEvent", "d", playerid);
	print("xOnPlayerLogin");
}

stock xOnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	#pragma unused inputtext
    if(dialogid == CHRISTMAS_WELCOME_DIALOG)
    {
        if(response == 1)
        {
            if(PlayerXmas[playerid][PLAYER_SACK] == 1)
            {
                SendClientMessage(playerid, COLOR_GREY, "You've already got your Christmas sack!");
                return 1;
            } else {
                PlayerXmas[playerid][PLAYER_SACK] = 1;
                SendClientMessage(playerid, COLOR_ICE, "You have received your Christmas sack! Use /xmasinv to see your event progress.");
                SendClientMessage(playerid, COLOR_ICE, "Click on each present in /xmasinv to get a clue towards its location.");
                SendClientMessage(playerid, COLOR_ICE, "Use /getpresent to pick up a present when you find one.");
                SendClientMessage(playerid, COLOR_ICE, "Once you've found them all, check /xmasinv one last time.");
                SendClientMessage(playerid, COLOR_GREY, "(You'll find it easier if you don't follow the order of /xmasinv!)");
                return 1;
            }
           
        }
    } 
    else if(dialogid == CHRISTMAS_INV_DIALOG)
    {
        if(response)
        {
            switch(listitem)
            {
                case 0: ShowPlayerDialog(playerid, CHRISTMAS_LV, DIALOG_STYLE_MSGBOX, "Clue #1", "{FFFFFF}The clues for this present are as follows:\n\n\n{7becf2}1) The initials of the place the present is located at are the same as a\nfamous fashion designer.\n\n2)The word 'pipelines'.", "Done", "");
                case 1: ShowPlayerDialog(playerid, CHRISTMAS_SF, DIALOG_STYLE_MSGBOX, "Clue #2", "{FFFFFF}The clues for this present are as follows:\n\n\n{7becf2}1) The citys real-life counterpart is often associated with a large, red bridge.\n\n2) The location is a word with another word meaning the same thing. \n    This word is a type of bird.", "Done", "");
                case 2: ShowPlayerDialog(playerid, CHRISTMAS_RC, DIALOG_STYLE_MSGBOX, "Clue #3", "{FFFFFF}The clues for this present are as follows:\n\n\n{7becf2}1) The initials of the place the present is located at are the same as the\ntoy; __ Racers.\n\n2) Fill in the gap of the 1996 film - ______spotting.", "Done", "");
                case 3: ShowPlayerDialog(playerid, CHRISTMAS_FC, DIALOG_STYLE_MSGBOX, "Clue #4", "{FFFFFF}The clues for this present are as follows:\n\n\n{7becf2}1) The initials of the place the present is located at are the same as a\nsection of our forums, often used as a threat towards rulebreakers in-game.\n\n2)The word 'winding'.", "Done", "");
                case 4: ShowPlayerDialog(playerid, CHRISTMAS_BS, DIALOG_STYLE_MSGBOX, "Clue #5", "{FFFFFF}The clue for this present is that {7becf2}the location is the same as where you'd go if you were cashing in your money from a bank robbery.", "Done", "");
                case 5: ShowPlayerDialog(playerid, CHRISTMAS_AA, DIALOG_STYLE_MSGBOX, "Clue #6", "{FFFFFF}The clues for this present are as follows:\n\n\n{7becf2}1) The initials of the place the present is located at are the same as a\nBritish breakdown cover company.\n\n2) The word 'orphan'.", "Done", "");
                case 6: ShowPlayerDialog(playerid, CHRISTMAS_TBE, DIALOG_STYLE_MSGBOX, "Clue #7", "{FFFFFF}The clue for this present is that {7becf2}it is located near a geyser that exists in the San Andreas Single Player mode.", "Done", "");
                case 7: ShowPlayerDialog(playerid, CHRISTMAS_CHI, DIALOG_STYLE_MSGBOX, "Clue #8", "{FFFFFF}The clue for this present is that {7becf2}it exists at the highest point in San Andreas.", "Done", "");
                case 8: ShowPlayerDialog(playerid, CHRISTMAS_LS, DIALOG_STYLE_MSGBOX, "Clue #9", "{FFFFFF}The clue for this present is an anagram:\n\n\n{7becf2}'LIST SODA  AMOUNTS'", "Done", "");
                case 9: ShowPlayerDialog(playerid, CHRISTMAS_LS, DIALOG_STYLE_MSGBOX, "Found all of the presents?", "{7becf2}Return to the Christmas tree and use /foundall.", "Got it", "");
            }
        }
        return 1; // We handled a dialog, so return 1. Just like OnPlayerCommandText.
    } 
    return 0;
}

CMD:christmas(playerid)
{
    if(PlayerXmas[playerid][FINISHED_EVENT] == 1) return SendClientMessage(playerid, COLOR_ICE, "You've already completed the Christmas event this year, thanks for taking part!");
    if(IsPlayerInRangeOfPoint(playerid, 2, XmasTreePointX, XmasTreePointY, XmasTreePointZ))
    {
        new w_string[420] = "Welcome to this years Christmas Event!\n\n           It's slightly more compact compared to last year, so will take less time to complete. Have fun!:\n\n           {42b3f4}The presents for this Christmas have gone missing! Rumour has it that they have been spread across San Andreas - it's your job to find them!\n           Pick up a sack from here, and get collecting!\n\n{660033}Developed by Day";
        ShowPlayerDialog(playerid, CHRISTMAS_WELCOME_DIALOG, DIALOG_STYLE_MSGBOX, "Christmas Event, 2016", w_string, "Take a sack", "Leave");
        return 1;
    } return 1;
}
//Inventory
 
forward ViewSack(playerid);
public ViewSack(playerid)
{
    new present_count;
    new titlestr[64];
    new line1[50], line2[50], line3[50], line4[50], line5[50], line6[50], line7[50], line8[50], line9[50], line10[50];
 
    //LV present
    new lvtext[50];
    if(PlayerXmas[playerid][LV_PRESENT] == 0)
    {
            lvtext = "{ff0000}You haven't found this present yet!";
    } else
    {
        lvtext = "{42f44e}Present found in Las Venturas!";
        present_count++;
    }
 
    //SF present
    new sftext[50];
    if(PlayerXmas[playerid][SF_PRESENT] == 0)
    {
        sftext = "{ff0000}You haven't found this present yet!";
    } else
    {
        sftext = "{42f44e}Present found in San Fierro!";
        present_count++;
    }
           
    //RC present
    new rctext[50];
    if(PlayerXmas[playerid][RC_PRESENT] == 0)
    {
        rctext = "{ff0000}You haven't found this present yet!";
    } else
    {
        rctext = "{42f44e}Present found in Red County!";
        present_count++;
    }
 
    //FC present
    new fctext[50];
    if(PlayerXmas[playerid][FC_PRESENT] == 0)
    {
        fctext = "{ff0000}You haven't found this present yet!";
    } else
    {
        fctext = "{42f44e}Present found in Flint County!";
        present_count++;
    }
 
    //BS present
    new bstext[50];
    if(PlayerXmas[playerid][BS_PRESENT] == 0)
    {
        bstext = "{ff0000}You haven't found this present yet!";
    } else
    {
        bstext = "{42f44e}Present found at Bayside!";
        present_count++;
    }
 
    //AA present
    new aatext[50];
    if(PlayerXmas[playerid][AA_PRESENT] == 0)
    {
        aatext = "{ff0000}You haven't found this present yet!";
    } else
    {
        aatext = "{42f44e}Present found at the Abandoned Airfield!";
        present_count++;
    }
 
    //TBE present
    new tbetext[50];
    if(PlayerXmas[playerid][TBE_PRESENT] == 0)
    {
        tbetext = "{ff0000}You haven't found this present yet!";
    } else
    {
        tbetext = "{42f44e}Present found near Regular Tom!";
        present_count++;
    }
 
    //CHI present
    new chitext[50];
    if(PlayerXmas[playerid][CHI_PRESENT] == 0)
    {
        chitext = "{ff0000}You haven't found this present yet!";
    } else
    {
        chitext = "{42f44e}Present found up on Mount Chilliad!";
        present_count++;
    }
 
    //LS present
    new lstext[50];
    if(PlayerXmas[playerid][LS_PRESENT] == 0)
    {
        lstext = "{ff0000}You haven't found this present yet!";
    } else
    {
        lstext = "{42f44e}Present found in Los Santos!";
        present_count++;
    }
 
    //Title
    format(titlestr, sizeof(titlestr), "{FFFFFF}%s\'s Christmas Sack [%d/9]", PlayerName(playerid), present_count);
 
    //Hidden finished line
    new finishedtext[50] = "Found all of the presents?";
 
    //DIALOG
    format(line1, sizeof(line1), "- %s", lvtext);
    format(line2, sizeof(line2), "- %s", sftext);
    format(line3, sizeof(line3), "- %s", rctext);
    format(line4, sizeof(line4), "- %s", fctext);
    format(line5, sizeof(line5), "- %s", bstext);
    format(line6, sizeof(line6), "- %s", aatext);
    format(line7, sizeof(line7), "- %s", tbetext);
    format(line8, sizeof(line8), "- %s", chitext);
    format(line9, sizeof(line9), "- %s", lstext);
    format(line10, sizeof(line10), "- %s", finishedtext);
 
    new mainstring[600];
    if(PlayerXmas[playerid][LV_PRESENT] == 0 || PlayerXmas[playerid][SF_PRESENT] == 0 || PlayerXmas[playerid][RC_PRESENT] == 0 || PlayerXmas[playerid][FC_PRESENT] == 0 || PlayerXmas[playerid][BS_PRESENT] == 0 || PlayerXmas[playerid][AA_PRESENT] == 0 || PlayerXmas[playerid][TBE_PRESENT] == 0 || PlayerXmas[playerid][CHI_PRESENT] == 0 || PlayerXmas[playerid][LS_PRESENT] == 0)
        {
            format(mainstring, sizeof(mainstring), "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", lvtext, sftext, rctext, fctext, bstext, aatext, tbetext, chitext, lstext);
        } else {
            format(mainstring, sizeof(mainstring), "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s", lvtext, sftext, rctext, fctext, bstext, aatext, tbetext, chitext, lstext, finishedtext);
        }
 
    ShowPlayerDialog(playerid, CHRISTMAS_INV_DIALOG, DIALOG_STYLE_LIST, titlestr, mainstring, "Done", "");
}
 
CMD:xmasinv(playerid)
{
    if(PlayerXmas[playerid][FINISHED_EVENT] == 1) return SendClientMessage(playerid, COLOR_ICE, "You've already completed the Christmas event this year, thanks for taking part!");
    if(PlayerXmas[playerid][PLAYER_SACK] == 0)
    {
        SendClientMessage(playerid, COLOR_GREY, "You haven't picked up a Christmas sack yet! Go to the Christmas tree on the motel roof to get one.");
        return 1;
    } else {
        ViewSack(playerid);
        return 1;
    }
}
 
//Picking up presents
 
forward PickUpPresent(playerid, currpresent);
public PickUpPresent(playerid, currpresent)
{
    if(currpresent == 1)
    {
        PlayerXmas[playerid][LV_PRESENT] = 1;
        ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Crouch_In", 3.0, 0, 0, 0, 0, 0, 0);
        GameTextForPlayer(playerid,"Present found in Las Venturas!", 4000, 3);
        SendClientMessage(playerid, COLOR_ICE, "Present found in Las Venturas!");
    }
 
    if(currpresent == 2)
    {
        PlayerXmas[playerid][SF_PRESENT] = 1;
        ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Crouch_In", 3.0, 0, 0, 0, 0, 0, 0);
        GameTextForPlayer(playerid,"Present found in San Fierro!", 4000, 3);
        SendClientMessage(playerid, COLOR_ICE, "Present found in San Fierro!");
    }
 
    if(currpresent == 3)
    {
        PlayerXmas[playerid][RC_PRESENT] = 1;
        ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Crouch_In", 3.0, 0, 0, 0, 0, 0, 0);
        GameTextForPlayer(playerid,"Present found in Red County!", 4000, 3);
        SendClientMessage(playerid, COLOR_ICE, "Present found in Red County!");
    }
 
    if(currpresent == 4)
    {
        PlayerXmas[playerid][FC_PRESENT] = 1;
        ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Crouch_In", 3.0, 0, 0, 0, 0, 0, 0);
        GameTextForPlayer(playerid,"Present found in Flint County!", 4000, 3);
        SendClientMessage(playerid, COLOR_ICE, "Present found in Flint County!");
    }
 
    if(currpresent == 5)
    {
        PlayerXmas[playerid][BS_PRESENT] = 1;
        ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Crouch_In", 3.0, 0, 0, 0, 0, 0, 0);
        GameTextForPlayer(playerid,"Present found at Bayside!", 4000, 3);
        SendClientMessage(playerid, COLOR_ICE, "Present found at Bayside!");
    }
 
    if(currpresent == 6)
    {
        PlayerXmas[playerid][AA_PRESENT] = 1;
        ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Crouch_In", 3.0, 0, 0, 0, 0, 0, 0);
        GameTextForPlayer(playerid,"~g~Present found at the Abandonded Airfield!", 4000, 3);
        SendClientMessage(playerid, COLOR_ICE, "Present found at the Abandonded Airfield!");
    }
 
    if(currpresent == 7)
    {
        PlayerXmas[playerid][TBE_PRESENT] = 1;
        ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Crouch_In", 3.0, 0, 0, 0, 0, 0, 0);
        GameTextForPlayer(playerid,"~g~Present found near Regular Tom!", 4000, 3);
        SendClientMessage(playerid, COLOR_ICE, "Present found near Regular Tom!");
    }
 
    if(currpresent == 8)
    {
        PlayerXmas[playerid][CHI_PRESENT] = 1;
        ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Crouch_In", 3.0, 0, 0, 0, 0, 0, 0);
        GameTextForPlayer(playerid,"~g~Present found up on Mount Chilliad!", 4000, 3);
        SendClientMessage(playerid, COLOR_ICE, "Present found up on Mount Chilliad!");
    }
 
    if(currpresent == 9)
    {
        PlayerXmas[playerid][LS_PRESENT] = 1;
        ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Crouch_In", 3.0, 0, 0, 0, 0, 0, 0);
        GameTextForPlayer(playerid,"~g~Present found in Los Santos!", 4000, 3);
        SendClientMessage(playerid, COLOR_ICE, "Present found in Los Santos!");
    }
   
}
 
CMD:getpresent(playerid, params[])
{
    new currpresent = 0;
    if(PlayerXmas[playerid][FINISHED_EVENT] == 1) return SendClientMessage(playerid, COLOR_ICE, "You've already completed the Christmas event this year, thanks for taking part!");
    if(PlayerXmas[playerid][PLAYER_SACK] == 0) return SendClientMessage(playerid, COLOR_GREY, "You haven't picked up your Christmas sack.");
 
    //LV present
    if(IsPlayerInRangeOfPoint(playerid, 2, 2931.8774,2119.2739,18.3906))
    {
        if(PlayerXmas[playerid][LV_PRESENT] == 0)
        {
            currpresent = 1;                                                      
            PickUpPresent(playerid, currpresent);
            return 1;
        }
    }
 
    //SF present
    if(IsPlayerInRangeOfPoint(playerid, 2, -1749.6079,188.8713,32.9856))
    {
        if(PlayerXmas[playerid][SF_PRESENT] == 0)
        {
            currpresent = 2;                                                      
            PickUpPresent(playerid, currpresent);
            return 1;
        }
    }
 
    //RC present
    if(IsPlayerInRangeOfPoint(playerid, 2, 2127.2258,-364.0407,57.5466))
    {
        if(PlayerXmas[playerid][RC_PRESENT] == 0)
        {
            currpresent = 3;                                                      
            PickUpPresent(playerid, currpresent);
            return 1;
        }
    }
 
    //FC present
    if(IsPlayerInRangeOfPoint(playerid, 2, -709.9302,-1727.5803,45.2867))
    {
        if(PlayerXmas[playerid][FC_PRESENT] == 0)
        {
            currpresent = 4;                                                      
            PickUpPresent(playerid, currpresent);
            return 1;
        }
    }
 
    //BS present
    if(IsPlayerInRangeOfPoint(playerid, 2, -2225.3049,2316.4102,4.9918))
    {
        if(PlayerXmas[playerid][BS_PRESENT] == 0)
        {
            currpresent = 5;                                                      
            PickUpPresent(playerid, currpresent);
            return 1;
        }
    }
 
    //AA present
    if(IsPlayerInRangeOfPoint(playerid, 2, 144.0808,2616.9910,16.7420))
    {
        if(PlayerXmas[playerid][AA_PRESENT] == 0)
        {
            currpresent = 6;                                                      
            PickUpPresent(playerid, currpresent);
            return 1;
        }
    }
 
    //TBE present
    if(IsPlayerInRangeOfPoint(playerid, 2, -261.8146,1859.4540,41.8891))
    {
        if(PlayerXmas[playerid][TBE_PRESENT] == 0)
        {
            currpresent = 7;                                                      
            PickUpPresent(playerid, currpresent);
            return 1;
        }
    }
 
    //CHI present
    if(IsPlayerInRangeOfPoint(playerid, 2, -2431.3525,-1619.5646,526.2825))
    {
        if(PlayerXmas[playerid][CHI_PRESENT] == 0)
        {
            currpresent = 8;                                                      
            PickUpPresent(playerid, currpresent);
            return 1;
        }
    }
 
    //LS present
    if(IsPlayerInRangeOfPoint(playerid, 2, 2769.5032, -1823.0596, 30.2693))
    {
        if(PlayerXmas[playerid][LS_PRESENT] == 0)
        {
            currpresent = 9;                                                      
            PickUpPresent(playerid, currpresent);
            return 1;
        }
    }
    else
    {
        SendClientMessage(playerid, COLOR_RED, "There are no obtainable presents nearby."); //not in range of any present or already got nearby presents
        return 1;
    } return 0;
}
 
 
//Finished
 
forward FoundThemAll(playerid);
public FoundThemAll(playerid)
{
    new f_string[400];
    format(f_string, sizeof(f_string), "{42ebf4}Congratulations, you have found all of the Christmas presents!\n\n{FFFFFF}From the entire staff roster, we wish you all a Merry Christmas, and thank you for sticking with the community through a tough 2016. \nThough the server has changed and SA:MP may getting old for some, the thing that keeps SARP together is its community.\n\nYou have received your Christmas present from us, thank you.");
    ShowPlayerDialog(playerid, CHRISTMAS_END_DIALOG, DIALOG_STYLE_MSGBOX, "Christmas Event, 2016", f_string, "Done", "");
    if(IsPlayerConnected(playerid))
    {
    	if(PlayerXmas[playerid][FINISHED_EVENT] == 1) return 1;
        PlayerInfo[playerid][pTikis] += 5; //REWARDS
        SendClientMessage(playerid, COLOR_GREY, "You receive 5 tikis for completing the 2016 Christmas event.");
        PlayerXmas[playerid][FINISHED_EVENT] = 1;
    }
    return 1;
}
 
CMD:foundall(playerid)
{
    if(PlayerXmas[playerid][FINISHED_EVENT] == 1) return SendClientMessage(playerid, COLOR_ICE, "You've already completed the Christmas event this year, thanks for taking part!");
    if(!IsPlayerInRangeOfPoint(playerid, 2, XmasTreePointX, XmasTreePointY, XmasTreePointZ))
    {
        SendClientMessage(playerid, COLOR_GREY, "You are not in range of the Christmas tree.");
        return 1;
    } else if(PlayerXmas[playerid][PLAYER_SACK] == 0)
    {
        SendClientMessage(playerid, COLOR_GREY, "You haven't got a Christmas sack yet. Use /christmas at the Christmas Tree to get one.");
        return 1;
 
    } else if(PlayerXmas[playerid][LV_PRESENT] == 0 || PlayerXmas[playerid][SF_PRESENT] == 0 || PlayerXmas[playerid][RC_PRESENT] == 0 || PlayerXmas[playerid][FC_PRESENT] == 0 || PlayerXmas[playerid][BS_PRESENT] == 0 || PlayerXmas[playerid][AA_PRESENT] == 0 || PlayerXmas[playerid][TBE_PRESENT] == 0 || PlayerXmas[playerid][CHI_PRESENT] == 0 || PlayerXmas[playerid][LS_PRESENT] == 0)
    {
        SendClientMessage(playerid, COLOR_GREY, "You haven't found all of the presents yet (/xmasinv).");
        return 1;
    } else
    {
        FoundThemAll(playerid);
        return 1;
    }
}



//============================[ MYSQL functions ]===============================
forward MySQL_Load_ChristmasEvent(playerid);
public MySQL_Load_ChristmasEvent(playerid)
{
	new rows, fields;
	cache_get_data(rows, fields);
	if(!rows) return CreateNewRecord(playerid);
	PlayerXmas[playerid][PLAYER_SACK] = cache_get_field_content_int(0, "PLAYER_SACK");
	PlayerXmas[playerid][LV_PRESENT] = cache_get_field_content_int(0, "LV_PRESENT");
	PlayerXmas[playerid][SF_PRESENT] = cache_get_field_content_int(0, "SF_PRESENT");
	PlayerXmas[playerid][RC_PRESENT] = cache_get_field_content_int(0, "RC_PRESENT");
	PlayerXmas[playerid][FC_PRESENT] = cache_get_field_content_int(0, "FC_PRESENT");
	PlayerXmas[playerid][BS_PRESENT] = cache_get_field_content_int(0, "BS_PRESENT");
	PlayerXmas[playerid][AA_PRESENT] = cache_get_field_content_int(0, "AA_PRESENT");
	PlayerXmas[playerid][TBE_PRESENT] = cache_get_field_content_int(0, "TBE_PRESENT");
	PlayerXmas[playerid][CHI_PRESENT] = cache_get_field_content_int(0, "CHI_PRESENT");
	PlayerXmas[playerid][LS_PRESENT] = cache_get_field_content_int(0, "LS_PRESENT");
	PlayerXmas[playerid][FINISHED_EVENT] = cache_get_field_content_int(0, "FINISHED_EVENT");
	return 1;
}
forward CreateNewRecord(playerid);
public CreateNewRecord(playerid)
{
	new query[500];
    mysql_format(sqlGameConnection, query, 500, "INSERT INTO `xmasevent2016` SET `ID` = %d, `PLAYER_SACK` = 0, `LV_PRESENT` = 0, `SF_PRESENT` = 0, `RC_PRESENT` = 0, `FC_PRESENT` = 0, `BS_PRESENT` = 0, ", PlayerInfo[playerid][pID]);
    mysql_format(sqlGameConnection, query, 500, "%s `AA_PRESENT` = 0, `TBE_PRESENT` = 0, `CHI_PRESENT` = 0, `LS_PRESENT` = 0, `FINISHED_EVENT` = 0;", query);
    mysql_tquery(sqlGameConnection, query);
	return 1;
}

MySQL_Save_ChristmasEvent(playerid)
{
	new query[1000];
	mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE xmasevent2016 SET PLAYER_SACK = %d, LV_PRESENT = %d, SF_PRESENT = %d",
		PlayerXmas[playerid][PLAYER_SACK], PlayerXmas[playerid][LV_PRESENT], PlayerXmas[playerid][SF_PRESENT]);

	mysql_format(sqlGameConnection, query, sizeof(query), "%s, RC_PRESENT = %d, FC_PRESENT = %d, BS_PRESENT = %d, AA_PRESENT = %d",
		query, PlayerXmas[playerid][RC_PRESENT], PlayerXmas[playerid][FC_PRESENT], PlayerXmas[playerid][BS_PRESENT], PlayerXmas[playerid][AA_PRESENT]);

	mysql_format(sqlGameConnection, query, sizeof(query), "%s, TBE_PRESENT = %d, CHI_PRESENT = %d, LS_PRESENT = %d, FINISHED_EVENT = %d WHERE ID = %d;",
		query, PlayerXmas[playerid][TBE_PRESENT], PlayerXmas[playerid][CHI_PRESENT], PlayerXmas[playerid][LS_PRESENT], PlayerXmas[playerid][FINISHED_EVENT], PlayerInfo[playerid][pID]);

	mysql_tquery(sqlGameConnection, query, "");

	return 1;
}

MySQL_createtable()
{
	new query[600];
    mysql_format(sqlGameConnection, query, 600, "CREATE TABLE IF NOT EXISTS `xmasevent2016` (`ID` int(11), `PLAYER_SACK` int(1), `LV_PRESENT` int(1), `SF_PRESENT` int(1), `RC_PRESENT` int(1),");
	mysql_format(sqlGameConnection, query, 600, "%s `FC_PRESENT` int(1), `BS_PRESENT` int(1), `AA_PRESENT` int(1), `TBE_PRESENT` int(1), `CHI_PRESENT` int(1), `LS_PRESENT` int(1), `FINISHED_EVENT` int(1), PRIMARY KEY (`ID`)) ENGINE=InnoDB DEFAULT CHARSET=latin1;", query);
	mysql_tquery(sqlGameConnection, query);
	return 1;
}