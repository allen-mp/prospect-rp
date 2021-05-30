#define SARP_BEACON_FIX

UpdateBeacons(){

	new i, j, color;

	foreach(i: Player){
		switch(backupType[i]){

			case 0:{continue;}

			case 1:{color = 0x2641FEFF;}

			case 2:{color = 0x8D8DFFFF;}

			case 3:{color = 0x2641FEFF;}

			case 4:{color = 0xFF8282FF;}

			case 5:{color = 0x9ACD32FF;}

			case FACTION_SANG:{color = TCOLOR_LIGHTGREEN;}

			case 666:{color = 0xFFFFFFFF;}
		}

		foreach(j: Player){
			if(backupType[i] != 666){
				if(backupType[i] == PlayerInfo[j][pMember])
					SetPlayerMarkerForPlayer(j, i, color);
			}
			else{
				if(PlayerInfo[j][pGroup] == GROUP_S9)
					SetPlayerMarkerForPlayer(j, i, color);
			}
		}
	}		
}