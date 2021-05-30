#include <a_samp>

//Author: DraGoN FiRe
//Version 1.1

public OnFilterScriptInit()
{
        print("\n\n\n|----------------------------------|\n");//Credits :P
        print("      Missing Bracket Finder");
        print("                by");
        print("           DraGoN FiRe\n");
        print("|----------------------------------|\n\n\n");
        if(!fexist("mbFile.lst"))
        {//Checking if file exist.
                print("Missing file \"mbFile.lst\", you must put preprocessed script file which \nyou want to check in scriptfiles folder and to name it \"mbFile.lst\".");//Checking if file exists in certain folder
                print("To get .lst file you need to compile your script with following params in \npawn.cfg \"-l -;+ -(+ -\\\", without quotes.");
                return 1;
        }
    new time = GetTickCount();//Getting current server time so we can calculate execution time
        print("Working...\n\n\n");
        new File:mbFile = fopen("mbFile.lst", io_read);//Opening File...
        new OpenedBrackets, ClosedBrackets, LastGoodLine, line, string[512], CountChecks, bool:Delete = false;//Vars
        while(fread(mbFile, string))
        {//Reading File
            line++;
                if(strfind(string, "\\", false) != -1)
                {//Removing Special simbols
                        for(new i = 0; string[i] != '\0'; i++)
                        {
                                if(string[i] == '\\')
                                {
                                        strdel(string, i, i+2);
                                        i--;
                                }
                        }
                }
                if(Delete && strfind(string, "\"", false) == -1)
                {//If there is
                        continue;
                }
                else if(strfind(string, "\"", false) != -1)
                {//Removing Text Areas
                        new posX, posY;
                        for(new i = 0; string[i] != '\0'; i++)
                        {
                                if(string[i] == '"')
                                {
                                    if(Delete)
                                        {
                                            strdel(string, 0, i+1);
                                            Delete = false;
                                            i = 0;
                                            continue;
                                        }
                                        if(!posX) posX = i;
                                        else if(!posY) posY = i;
                                }
                                if(posX && posY)
                                {
                                    posY++;
                                        strdel(string, posX, posY);
                                        i -= (posY-posX);
                                        posX = posY = 0;
                                }
                        }
                        if(posX && !posY)
                        {//If there is opened quote (text area with break line)
                            strdel(string, posX, strlen(string));
                            Delete = true;
                        }
                }
            if(strfind(string, "public ", false) != -1 || strfind(string, "forward ", false) != -1 || strfind(string, "stock ", false) != -1)
                {//Searching for valid checkpoints, where brackets must match
                    if(OpenedBrackets != ClosedBrackets)
                    {
                        break;
                    }
                    else
                        {//If number of brackets match we can save this as last good line...
                                LastGoodLine = line;
                                CountChecks++;
                        }
                }
            if(strfind(string, "{", true) != -1)
            {//Searching for open brackets and counting them
                for(new i = 0; string[i] != '\0'; i++)
                {
                    if(string[i] == '{')
                    {
                        OpenedBrackets++;
                    }
                }
            }
            if(strfind(string, "}", true) != -1)
                {//Searching for closed brackets and counting them
                    for(new i = 0; string[i] != '\0'; i++)
                {
                    if(string[i] == '}')
                    {
                        ClosedBrackets++;
                    }
                }
                }
        }
        if(OpenedBrackets == ClosedBrackets) print("Everything is fine! ;) Enjoy!\n\n\n");
        if(OpenedBrackets != ClosedBrackets)
        {
                if(OpenedBrackets > ClosedBrackets) printf("Some bracket isn't closed on lines between %d and %d.\n\n\n", LastGoodLine, line);
                if(OpenedBrackets < ClosedBrackets) printf("Some bracket isn't opened on lines between %d and %d.\n\n\n", LastGoodLine, line);
        }
        print("|---------------Stats---------------|");
        printf("Opened Brackets: %d\nClosed Brackets: %d\nTotal lines: %d\nTime to execute: %.2fsek\nTotal CheckPoints: %d", OpenedBrackets, ClosedBrackets, line, float(GetTickCount() - time)/1000, CountChecks);
    print("|-----------------------------------|");
        return 1;
}

public OnFilterScriptExit()
{
        return 1;
}
