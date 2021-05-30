// Made by Desmond DeSantos

#define MCCLAINDIALOG1 18621
#define MCCLAINDIALOG2 18622
#define MCCLAINDIALOG3 18623
#define MCCLAINDIALOG4 18624
#define MCCLAINDIALOG5 18625
#define MCCLAINDIALOG6 18626
#define MCCLAINDIALOG7 18627
#define MCCLAINDIALOG8 18628


new mcstr[128];

enum glInfo
{
    lWeapon1,
    lWeapon2,
    lWeapon3
}
new GunLockerInfo[glInfo];


enum vlInfo
{
    Float:lVest1,
    Float:lVest2,
    Float:lVest3
}
new VestLockerInfo[vlInfo];


Hook:McClain_OnGameModeInit()
{
    Create3DTextLabel("/mcdrinkwater",COLOR_YELLOW ,-895.5306,1514.3572,2503.4697, 1.0, 0, 0);
    Create3DTextLabel("/mcdrinkwater",COLOR_YELLOW ,-881.4675,1514.2550,2503.4697, 1.0, 0, 0);
    Create3DTextLabel("/mcdrinkwater",COLOR_YELLOW ,-895.1724,1492.3184,2503.4697, 1.0, 0, 0);
    return 1;
}


//Commands

//LOCKERS
CMD:mclockers(playerid,params[])
{
    if(!IsPlayerInRangeOfPoint(playerid, 3.0, -879.4963,1502.2184,2503.4697)) return SendClientMessage(playerid, COLOR_RED, "You are not in range of the McClain Company lockers.");
    ShowPlayerDialog(playerid, MCCLAINDIALOG1, DIALOG_STYLE_LIST, "McClain Company Lockers", "Weapon Storage \n Kevlar Storage \n First aid kit \n Check locker", "Select", "Exit");
    return 1;
}


//Heli Rails
CMD:mcrails(playerid,params[]) // This CMD was approved by Smo. We have this CMD in the old script
{
    SendClientMessage(playerid, COLOR_RED, "this command has been disabled."); 
    return 1;
}

//Misc
CMD:mcuniforms(playerid, params[])
{
    if(IsPlayerInRangeOfPoint(playerid, 3.0, -885.0938,1481.8187,2503.4697))
    {
        ShowPlayerDialog(playerid,MCCLAINDIALOG8,DIALOG_STYLE_LIST,"McClain Comapany Wardrobe","Casual 1 (Boy)\nDuty Suit\nCasual 2 (Boy)\nGirl Staff Uniform","Select","Cancel");
    }
    return 1;
}

CMD:mchelp(playerid,params[])
{
    SendClientMessage(playerid, COLOR_WHITE, "|______________McClain Company Commands______________|");
    SendClientMessage(playerid, COLOR_WHITE, "/mclockers /mcrails /mcuniforms");
}

Hook:MC_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == MCCLAINDIALOG1)
    {
        if(response)
        {
            switch(listitem)
            {
                case 0: 
                {
                    ShowPlayerDialog(playerid, MCCLAINDIALOG2, DIALOG_STYLE_LIST, "McClain Company Weapon Storage", "Store Weapon \n Get Weapon", "Enter", "Exit");
                }
                case 1:
                {
                    ShowPlayerDialog(playerid, MCCLAINDIALOG3, DIALOG_STYLE_LIST, "McClain Company Kevlar Storage", "Store Kevlar \n Take Kevlar", "Enter", "Exit");
                }
                case 2: 
                {
                    SetPlayerHealth(playerid, 100);
                }
                case 3:
                {
                    new gunname[32];
                    new name[MAX_PLAYER_NAME];
                    GetPlayerName(playerid, name, sizeof(name));
                    SendClientMessage(playerid, COLOR_WHITE, "|_________McClain Company Lockers_________|");
                    if(GunLockerInfo[lWeapon1] != 0)
                    {
                        GetWeaponName(GunLockerInfo[lWeapon1],gunname,sizeof(gunname));
                        format(mcstr,sizeof(mcstr),"Weapon Slot 1: %s", gunname,GunLockerInfo[lWeapon1]);
                        SendClientMessage(playerid,COLOR_WHITE,mcstr);
                    }
                    if(GunLockerInfo[lWeapon1] == 0)
                    {
                        SendClientMessage(playerid, COLOR_WHITE, "Weapon Slot 1: None");
                    }
                    if(GunLockerInfo[lWeapon2] != 0)
                    {
                        GetWeaponName(GunLockerInfo[lWeapon2],gunname,sizeof(gunname));
                        format(mcstr,sizeof(mcstr),"Weapon Slot 2: %s", gunname,GunLockerInfo[lWeapon2]);
                        SendClientMessage(playerid,COLOR_WHITE,mcstr);
                    }
                    if(GunLockerInfo[lWeapon2] == 0)
                    {
                        SendClientMessage(playerid, COLOR_WHITE, "Weapon Slot 2: None");
                    }
                    if(GunLockerInfo[lWeapon3] != 0)
                    {
                        GetWeaponName(GunLockerInfo[lWeapon3],gunname,sizeof(gunname));
                        format(mcstr,sizeof(mcstr),"Weapon Slot 3: %s", gunname,GunLockerInfo[lWeapon3]);
                        SendClientMessage(playerid,COLOR_WHITE,mcstr);
                    }
                    if(GunLockerInfo[lWeapon3] == 0)
                    {
                        SendClientMessage(playerid, COLOR_WHITE, "Weapon Slot 3: None");
                    }
                    if(VestLockerInfo[lVest1] != 0)
                    {
                        format(mcstr,sizeof(mcstr),"Kevlar Slot 1: %f",VestLockerInfo[lVest1]);
                        SendClientMessage(playerid, COLOR_WHITE, mcstr);
                    }
                    if(VestLockerInfo[lVest1] == 0)
                    {
                        SendClientMessage(playerid, COLOR_WHITE, "Kevlar Slot 1: None");
                    }
                    if(VestLockerInfo[lVest2] != 0)
                    {
                        format(mcstr,sizeof(mcstr),"Kevlar Slot 2: %f",VestLockerInfo[lVest2]);
                        SendClientMessage(playerid, COLOR_WHITE, mcstr);
                    }
                    if(VestLockerInfo[lVest2] == 0)
                    {
                        SendClientMessage(playerid, COLOR_WHITE, "Kevlar Slot 2: None");
                    }
                    if(VestLockerInfo[lVest3] != 0)
                    {
                        format(mcstr,sizeof(mcstr),"Kevlar Slot 3: %f",VestLockerInfo[lVest3]);
                        SendClientMessage(playerid, COLOR_WHITE, mcstr);
                    }
                    if(VestLockerInfo[lVest3] == 0)
                    {
                        SendClientMessage(playerid, COLOR_WHITE, "Kevlar Slot 3: None");
                    }
                }
            }
        }
    }
    if(dialogid == MCCLAINDIALOG2)
    {
        if(response)
        {
            switch(listitem)
            {
                case 0: 
                {
                    ShowPlayerDialog(playerid, MCCLAINDIALOG4, DIALOG_STYLE_LIST, "Store Weapon", "Slot 1\n Slot2\n Slot3", "Enter", "Exit");
                }
                case 1: 
                {
                    ShowPlayerDialog(playerid, MCCLAINDIALOG5, DIALOG_STYLE_LIST, "Take Weapon", "Slot 1 \n Slot 2 \n Slot 3", "Enter", "Exit");
                }
            }
            
        }
    }
    if(dialogid == MCCLAINDIALOG3)
    {
        if(response)
        {
            switch(listitem)
            {
                case 0:
                {
                    ShowPlayerDialog(playerid, MCCLAINDIALOG6, DIALOG_STYLE_LIST, "Store Kevlar", "Slot 1 \n Slot 2 \n Slot 3", "Enter", "Exit");
                }
                case 1:
                {
                    ShowPlayerDialog(playerid, MCCLAINDIALOG7, DIALOG_STYLE_LIST, "Get Kevlar", "Slot 1 \n Slot 2 \n Slot 3", "Enter", "Exit");
                }
            }
        }
    }
    if(dialogid == MCCLAINDIALOG4)
       {
        if(response)
        {
            if (listitem == 0)
            {
                if(GunLockerInfo[lWeapon1] != 0)
                {
                    SendClientMessage(playerid,COLOR_RED,"That slot has already filled up.");
                }
                new WeapID = GetPlayerWeapon(playerid);
                if(HaveAdminWeapon(playerid, WeapID) == WeapID) return SendClientMessage(playerid, COLOR_GREY, "You cannot store admin given weapons.");
                if(!WeapID)
                {
                    SendClientMessage(playerid,COLOR_RED,"You don't have any weapon on your hand to store.");
                    return 1;
                }
                GunLockerInfo[lWeapon1] = WeapID;
                TakePlayerWeapon(playerid, WeapID);
                SendClientMessage(playerid,COLOR_WHITE,"You have successfully put your weapon in the locker");
            }
            if (listitem == 1)
            {
                if(GunLockerInfo[lWeapon2] != 0)
                {
                    SendClientMessage(playerid,COLOR_RED,"That slot has already filled up.");
                }
                new WeapID = GetPlayerWeapon(playerid);
                if(HaveAdminWeapon(playerid, WeapID) == WeapID) return SendClientMessage(playerid, COLOR_GREY, "You cannot store admin given weapons.");
                if(!WeapID)
                {
                    SendClientMessage(playerid,COLOR_RED,"You don't have any weapon on your hand to store.");
                    return 1;
                }
                GunLockerInfo[lWeapon2] = WeapID;
                TakePlayerWeapon(playerid, WeapID);
                SendClientMessage(playerid,COLOR_WHITE,"You have successfully put your weapon in the locker");
            }
            if (listitem == 2)
            {
                if(GunLockerInfo[lWeapon3] != 0)
                {
                    SendClientMessage(playerid,COLOR_RED,"That slot has already filled up.");
                }
                new WeapID = GetPlayerWeapon(playerid);
                if(HaveAdminWeapon(playerid, WeapID) == WeapID) return SendClientMessage(playerid, COLOR_GREY, "You cannot store admin given weapons.");
                if(!WeapID)
                {
                    SendClientMessage(playerid,COLOR_RED,"You don't have any weapon on your hand to store.");
                    return 1;
                }
                GunLockerInfo[lWeapon3] = WeapID;
                TakePlayerWeapon(playerid, WeapID);
                SendClientMessage(playerid,COLOR_WHITE,"You have successfully put your weapon in the locker");
            }
        }
    }
    if(dialogid == MCCLAINDIALOG5)
    {
        if(response)
        {
            if (listitem == 0)
            {
                if(GunLockerInfo[lWeapon1] == 0)
                {
                    SendClientMessage(playerid,COLOR_RED,"That slot is empty.");
                    return 1;
                }
                GivePlayerGun(playerid,GunLockerInfo[lWeapon1]);
                SendClientMessage(playerid,COLOR_WHITE,"You took the weapon from the locker");
                GunLockerInfo[lWeapon1] = 0;
                return 0;
            }
            if (listitem == 1)
            {
                if(GunLockerInfo[lWeapon2] == 0)
                {
                    SendClientMessage(playerid,COLOR_RED,"That slot is empty.");
                    return 1;
                }
                GivePlayerGun(playerid,GunLockerInfo[lWeapon2]);
                SendClientMessage(playerid,COLOR_WHITE,"You took the weapon from the locker");
                GunLockerInfo[lWeapon2] = 0;
                return 0;
            }
            if (listitem == 2)
            {
                if(GunLockerInfo[lWeapon3] == 0)
                {
                    SendClientMessage(playerid,COLOR_RED,"That slot is empty.");
                    return 1;
                }
                GivePlayerGun(playerid,GunLockerInfo[lWeapon3]);
                SendClientMessage(playerid,COLOR_WHITE,"You took the weapon from the locker");
                GunLockerInfo[lWeapon3] = 0;
            }
        }
    }
    if(dialogid == MCCLAINDIALOG6)
       {
        if(response)
        {
            switch(listitem)
            {
                case 0:
                {
                    if(VestLockerInfo[lVest1] != 0)
                        {
                        SendClientMessage(playerid,COLOR_RED,"That slot has already filled up.");
                        return 1;
                    }
                    new Float:armour;
                    GetPlayerArmour(playerid, armour);
                    if(armour == 0)
                    {
                        SendClientMessage(playerid, COLOR_RED, "You don't have a kevlar vest to store.");
                        return 1;
                    }
                    if(armour != 0)
                    {
                        SetPlayerArmour(playerid, 0);
                        VestLockerInfo[lVest1] = armour;
                        SendClientMessage(playerid, COLOR_WHITE, "You can successfully stored your kevlar vest to the lockers.");
                        return 1;
                    }
                }
                case 1:
                {
                    if(VestLockerInfo[lVest2] != 0)
                    {
                        SendClientMessage(playerid,COLOR_RED,"That slot has already filled up.");
                        return 1;
                    }
                    new Float:armour;
                    GetPlayerArmour(playerid, armour);
                    if(armour == 0)
                    {
                        SendClientMessage(playerid, COLOR_RED, "You don't have a kevlar vest to store.");
                        return 1;
                    }
                    if(armour != 0)
                    {
                        SetPlayerArmour(playerid, 0);
                        VestLockerInfo[lVest2] = armour;
                        SendClientMessage(playerid, COLOR_WHITE, "You can successfully stored your kevlar vest to the lockers.");
                        return 1;
                    }
                }
                case 2:
                {
                    if(VestLockerInfo[lVest3] != 0)
                        {
                        SendClientMessage(playerid,COLOR_RED,"That slot has already filled up.");
                        return 1;
                    }
                    new Float:armour;
                    GetPlayerArmour(playerid, armour);
                    if(armour == 0)
                    {
                        SendClientMessage(playerid, COLOR_RED, "You don't have a kevlar vest to store.");
                        return 1;
                    }
                    if(armour != 0)
                    {
                        SetPlayerArmour(playerid, 0);
                        VestLockerInfo[lVest3] = armour;
                        SendClientMessage(playerid, COLOR_WHITE, "You can successfully stored your kevlar vest to the lockers.");
                        return 1;
                    }
                }
            }
        }
        return 0;
    }
    if(dialogid == MCCLAINDIALOG7)
   {
       if(response)
       {
            if(listitem == 0)
            {
                if(VestLockerInfo[lVest1] == 0) return SendClientMessage(playerid,COLOR_WHITE,"That slot is empty.");
                SetPlayerArmour(playerid, VestLockerInfo[lVest1]);
                VestLockerInfo[lVest1] = 0;
                SendClientMessage(playerid,COLOR_WHITE,"You have successfully took a kevlar vest from the locker");
            }    
            if(listitem == 1)
            {
                if(VestLockerInfo[lVest2] == 0) return SendClientMessage(playerid,COLOR_WHITE,"That slot is empty.");
                SetPlayerArmour(playerid, VestLockerInfo[lVest2]);
                VestLockerInfo[lVest2] = 0;
                SendClientMessage(playerid,COLOR_WHITE,"You have successfully took a kevlar vest from the locker");
            } 
            if(listitem == 2)
            {
                if(VestLockerInfo[lVest3] == 0) return SendClientMessage(playerid,COLOR_WHITE,"That slot is empty.");
                SetPlayerArmour(playerid, VestLockerInfo[lVest3]);
                VestLockerInfo[lVest3] = 0;
                SendClientMessage(playerid,COLOR_WHITE,"You have successfully took a kevlar vest from the locker");
            } 
       }
    }
    if(dialogid == MCCLAINDIALOG8) // Uniforms
    {
        if(response)
        {
            switch(listitem)
            {
                case 0: SetPlayerSkin(playerid, 223);            
                case 1: SetPlayerSkin(playerid, 50);
                case 2: SetPlayerSkin(playerid, 101);
                case 3: SetPlayerSkin(playerid, 233);
            }
        }
    }
    return 1;
}