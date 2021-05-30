
/*
 _____                            _____           _       _
|  __ \                          / ____|         (_)     | |
| |  | | ___  _ __   ___  _ __  | (___   ___ _ __ _ _ __ | |_ ___
| |  | |/ _ \| '_ \ / _ \| '__|  \___ \ / __| '__| | '_ \| __/ __|
| |__| | (_) | | | | (_) | |     ____) | (__| |  | | |_) | |_\__ \
|_____/ \___/|_| |_|\___/|_|    |_____/ \___|_|  |_| .__/ \__|___/
                                                   | |
                                                   |_|


	Donors have their own file and folder, where the file is named FIRSTNAME_LASTNAME_SQLID.inc

	Such as FIRST_LAST_123.inc
		FOLDER-NAME: FIRST_LAST_123/items( includes )

	Example line to include the script is
											 #include "/donor/First_Last_123.inc"

*/
stock DonorDebug(string[128])
{
	Log:debug("DONOR", "Loaded Donor script %s", string);
}


#include "inc/donor/donor_Scripts/Juan_Ordonez_82903\_core.inc" // Works

#include "inc/donor/donor_Scripts/Mariela_Tucci_91115/_core.inc" // Works

#include "inc/donor/donor_Scripts/Jonas_Blue_1423/_core.inc" // Works

#include "inc/donor/Donor_scripts/Primitivo_Bienvenida_941/_core.inc" // Works 

#include "inc/donor/donor_Scripts/Sean_Johnson_859/_core.inc" // Works

#include "inc/donor/donor_Scripts/Ricardo_Abalos-190549/_core.inc" // Works

#include "inc/donor/donor_Scripts/Aaron_Steel_152639/_core.inc" // Works
