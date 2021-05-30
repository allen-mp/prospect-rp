#define MAX_DONATED_GROUPS 50
#define DIALOG_DONATED_GROUPS 9361
new Iterator: DonatedGroups< MAX_DONATED_GROUPS >, DonatedGroupID[ MAX_DONATED_GROUPS ], GroupName[ MAX_DONATED_GROUPS ] [ 150 ], GroupMaxRanks[ MAX_DONATED_GROUPS ], RankName1[ MAX_DONATED_GROUPS ] [ 25 ],
RankName2[ MAX_DONATED_GROUPS ] [ 25 ], RankName3[ MAX_DONATED_GROUPS ] [ 25 ], RankName4[ MAX_DONATED_GROUPS ] [ 25 ], RankName5[ MAX_DONATED_GROUPS ] [ 25 ], RankName6[ MAX_DONATED_GROUPS ] [ 25 ],
RankName7[ MAX_DONATED_GROUPS ] [ 25 ], RankName8[ MAX_DONATED_GROUPS ][ 25 ],Rank1Active[ MAX_DONATED_GROUPS ], Rank2Active[ MAX_DONATED_GROUPS ], Rank3Active[ MAX_DONATED_GROUPS ],
Rank4Active[ MAX_DONATED_GROUPS ], Rank5Active[ MAX_DONATED_GROUPS ], Rank6Active[ MAX_DONATED_GROUPS ], Rank7Active[ MAX_DONATED_GROUPS ], Rank8Active[ MAX_DONATED_GROUPS ], bool:GroupActive[ MAX_DONATED_GROUPS ], 
GroupOwner[ MAX_DONATED_GROUPS ] [ 69 ], DonorGroup[ MAX_PLAYERS ] = {-1, ...}, DonorName[ MAX_PLAYERS ] [ 150 ], PlayerIG[ MAX_PLAYERS ], MaskPerm[ MAX_DONATED_GROUPS ], SetUcNamePerm[ MAX_DONATED_GROUPS ],
DisguisePerm[ MAX_DONATED_GROUPS ], CallerIDPerm[ MAX_DONATED_GROUPS ];

static DonatedGroupStr[ 350 ];
CMD:adh( playerid )
{
	if( PlayerInfo[ playerid ] [ pAdmin ] < 1337 ) SendClientMessageEx( playerid, COLOR_GREY, "{ff0000}Error:{ffffff} Your admin level is %d but you need 1337 or above!", PlayerInfo[ playerid ] [ pAdmin ] );
	else
	{
		SendClientMessage( playerid, COLOR_GREY, "Commands{ffffff} /createdonatedgroup, /editdonatedgroup, /makedonatedgroupleader, /donatedgroups");
	}
	return 1;
}
CMD:dh( playerid, params[ ] )
{
	SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Commands:{FFFFFF} /quitdoantedgroup, /dkick, /dgiverank, /dinvite, /daccept, /dmembers, /dr, /dor, /mygroupperms, /editrank" );
	return 1;
}
/*												PLAYER-CMDS																*/
CMD:quitdonatedgroup( playerid, params[ ] )
{
	new confirmation[50], gid = PlayerInfo[ playerid ] [ pDonatedGroup ]/*, query[ 350 ]*/;
	if( PlayerInfo[ playerid ] [ pDonatedGroup ] == 0 ) SendClientMessage( playerid, COLOR_GREY, "You're not in any group!" );
	else if( sscanf( params, "s[50]", confirmation ) ) SendClientMessageEx( playerid, COLOR_GREY, "Are you sure you want to quit %s, type /quitdonatedgroup <CONFIRM>.", GroupName[ gid ] );
	else if( !strcmp( confirmation, "confirm", true ) )
	{
		SendClientMessageEx( playerid, COLOR_GREY, "You've left group %s!", GroupName[ gid ] [ 0 ] );
		PlayerInfo[ playerid ] [ pDonatedGroup ] = 0;
		PlayerInfo[ playerid ] [ pDonatedRank ] = 0;
	}
	return 1;
}
CMD:dkick( playerid, params[ ] )
{
	new giveplayerid;
	if( PlayerInfo[ playerid ] [ pDonatedGroup ] == 0 ) SendClientMessage( playerid, COLOR_GREY, "You're not in any group!" );
	else if( PlayerInfo[ playerid ] [ pDonatedRank ] != GroupMaxRanks[ PlayerInfo[ playerid ] [ pDonatedGroup ] ] ) SendClientMessageEx( playerid, COLOR_GREY, "Your rank is %d, but you must be rank %d!", PlayerInfo[ playerid ] [ pDonatedRank ], GroupMaxRanks[ PlayerInfo[ playerid ] [ pDonatedGroup ] ] );
	else if( sscanf( params, "u", giveplayerid ) ) SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dkick <Player>" );
	else if( PlayerInfo[ giveplayerid ] [ pDonatedGroup ] != PlayerInfo[ playerid ] [ pDonatedGroup ] ) SendClientMessage( playerid, COLOR_GREY, "That player is not a member of your group!" );
	else if( PlayerInfo[ giveplayerid ] [ pDonatedRank ] == PlayerInfo[ playerid ] [ pDonatedRank ] ) SendClientMessage( playerid, COLOR_GREY, "That player is the same rank as you, contact an admin." );
	else
	{
		SendClientMessageEx( giveplayerid, COLOR_GREY, "%s has kicked you out of %s!", PlayerOOCName( playerid ), GroupName[ PlayerInfo[ giveplayerid ] [ pDonatedGroup ] ] );
		SendClientMessageEx( playerid, COLOR_GREY, "You've successfully kicked %s out of %s", PlayerOOCName( giveplayerid ), GroupName[ PlayerInfo[ playerid ] [ pDonatedGroup ] ] );
		PlayerInfo[ giveplayerid ] [ pDonatedGroup ] = 0;
		PlayerInfo[ giveplayerid ] [ pDonatedRank ] = 0;
		DonorGroup[ giveplayerid ] = -1;
		PlayerIG[ giveplayerid ] = INVALID_PLAYER_ID;
	}
	return 1;
}
CMD:dgiverank( playerid, params[ ] )
{
	new giveplayerid, rank;
	if( PlayerInfo[ playerid ] [ pDonatedGroup ] == 0 ) SendClientMessage( playerid, COLOR_GREY, "You're not in any group!" );
	else if( PlayerInfo[ playerid ] [ pDonatedRank ] != GroupMaxRanks[ PlayerInfo[ playerid ] [ pDonatedGroup ] ] ) SendClientMessageEx( playerid, COLOR_GREY, "Your rank is %d, but you must be rank %d!", PlayerInfo[ playerid ] [ pDonatedRank ], GroupMaxRanks[ PlayerInfo[ playerid ] [ pDonatedGroup ] ] );
	else if( sscanf( params, "ui", giveplayerid, rank ) ) SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dgiverank <Player> <Rank>" );
	else if( PlayerInfo[ giveplayerid ] [ pDonatedGroup ] != PlayerInfo[ playerid ] [ pDonatedGroup ] ) SendClientMessage( playerid, COLOR_GREY, "That player is not a member of your group!" );
	else if( PlayerInfo[ giveplayerid ] [ pDonatedRank ] == PlayerInfo[ playerid ] [ pDonatedRank ] ) SendClientMessage( playerid, COLOR_GREY, "That player is the same rank as you, contact an admin." );
	else if( rank > GroupMaxRanks[ PlayerInfo[ playerid ] [ pDonatedGroup ] ] ) SendClientMessageEx( playerid, COLOR_GREY, "Your group max ranks are: %d, and you can't promote %s to rank %d!", 
		GroupMaxRanks[ PlayerInfo[ playerid ] [ pDonatedGroup ] ], PlayerOOCName( giveplayerid ), rank );
	else
	{
		SendClientMessageEx( giveplayerid, COLOR_GREY, "You've been promoted by %s to %d", PlayerOOCName( playerid ), rank );
		SendClientMessageEx( playerid, COLOR_GREY, "You've promoted %s to %d", PlayerOOCName( giveplayerid ), rank );
		PlayerInfo[ giveplayerid ] [ pDonatedRank ] = rank;
	}
	return 1;
}
CMD:dinvite( playerid, params[ ] )
{
	new gid = PlayerInfo[ playerid ] [ pDonatedGroup ], giveplayerid;
	if( PlayerInfo[ playerid ] [ pDonatedGroup ] == 0 ) SendClientMessage( playerid, COLOR_GREY, "You're not in any group!" );
	else if( !( PlayerInfo[ playerid ] [ pDonatedRank ] == GroupMaxRanks[ gid ] ) ) SendClientMessageEx( playerid, COLOR_GREY, "Your rank is %d, but you must be rank %d!", PlayerInfo[ playerid ] [ pDonatedRank ], GroupMaxRanks[ gid ] );
	else if( sscanf( params, "u", giveplayerid ) ) SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dinvite <Player>" );
	else
	{
		DonorGroup[ giveplayerid ] = gid;
		PlayerIG[ giveplayerid ] = playerid;
		DonorName[ giveplayerid ] = GroupName[ gid ];
		SendClientMessageEx( playerid, COLOR_GREY, "You've invited %s to join %s!", PlayerOOCName( giveplayerid ), GroupName[ gid ] );
		SendClientMessageEx( giveplayerid, COLOR_GREY, "You've been invited by %s to join %s, type /daccept to accept it!", PlayerOOCName( playerid ), GroupName[ gid ] );
	}
	return 1;
}
CMD:daccept( playerid )
{
	if( DonorGroup[ playerid ] == -1 ) SendClientMessage( playerid, COLOR_GREY, "You were not invited by anyone!" );
	else if( PlayerInfo[ playerid ] [ pDonatedGroup ] != 0 ) SendClientMessage( playerid, COLOR_GREY, "You're already in a group!" );
	else if( PlayerInfo[ playerid ] [ pGroup ] != 0 ) SendClientMessage( playerid, COLOR_GREY, "You must quit normal group then you can join donated one!" );
	else if( !IsPlayerConnected( PlayerIG[ playerid ] ) ) 
	{
		SendClientMessage( playerid, COLOR_GREY, "That player is not connected!" );
		PlayerIG[ playerid ] = INVALID_PLAYER_ID;
		DonorGroup[ playerid ] = -1;
	}
	else
	{
		SendClientMessageEx( playerid, COLOR_GREY, "You've successfully joined %s under %s!",  DonorName[ playerid ], PlayerOOCName( PlayerIG[ playerid ] ) );
		SendClientMessageEx( PlayerIG[ playerid ], COLOR_GREY, "%s has accepted your invitation and joined %s!", PlayerOOCName( playerid ), DonorName[ playerid ] );
		PlayerInfo[ playerid ] [ pDonatedGroup ] = DonorGroup[ playerid ];
		PlayerInfo[ playerid ] [ pDonatedRank ] = 1;
		PlayerIG[ playerid ] = INVALID_PLAYER_ID;
		DonorGroup[ playerid ] = -1;
	}
	return 1;
}

CMD:editrank( playerid, params[ ] )
{
	new group = PlayerInfo[ playerid ] [ pDonatedGroup ], rank, rankname[ 25 ], query[ 350 ];
	if( group == 0 ) SendClientMessage( playerid, COLOR_GREY, "You're not in any donated group so you can't use this command!" );
	else if( GroupMaxRanks[ group ] != PlayerInfo[ playerid ] [ pDonatedRank ] ) SendClientMessage( playerid, COLOR_GREY, "You are not the highest rank in your group so you can't use this command!" );
	else if( sscanf( params, "is[25]", rank, rankname ) ) SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /editrank [Rank] [Rank Name]" );
	else if( rank > GroupMaxRanks[ group ] || rank < 1 ) SendClientMessageEx( playerid, COLOR_GREY, "This group has max %d ranks starting from 1!", GroupMaxRanks[ group ] );
	else
	{
		if( rank == 1 ) 
		{ 
			RankName1[ group ] = rankname; 
			SendClientMessageEx( playerid, COLOR_GREY, "You've successfully edited rank 1 to %s", rankname ); 
			mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `RankName1`='%s', `Rank1Active`=1 WHERE `ID`='%d'", rankname, group ); 
		}
		else if( rank == 2 ) 
		{ 
			RankName2[ group ] = rankname; 
			SendClientMessageEx( playerid, COLOR_GREY, "You've successfully edited rank 2 to %s", rankname ); 
			mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `RankName2`='%s', `Rank1Active`=1 WHERE `ID`='%d'", rankname, group ); 
		}
		else if( rank == 3 ) 
		{ 
			RankName3[ group ] = rankname; 
			SendClientMessageEx( playerid, COLOR_GREY, "You've successfully edited rank 3 to %s", rankname ); 
			mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `RankName3`='%s', `Rank1Active`=1 WHERE `ID`='%d'", rankname, group ); 
		}
		else if( rank == 4 ) 
		{ 
			RankName4[ group ] = rankname; 
			SendClientMessageEx( playerid, COLOR_GREY, "You've successfully edited rank 4 to %s", rankname ); 
			mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `RankName4`='%s', `Rank1Active`=1 WHERE `ID`='%d'", rankname, group ); 
		}
		else if( rank == 5 ) 
		{ 
			RankName5[ group ] = rankname; 
			SendClientMessageEx( playerid, COLOR_GREY, "You've successfully edited rank 5 to %s", rankname ); 
			mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `RankName5`='%s', `Rank1Active`=1 WHERE `ID`='%d'", rankname, group ); 
		}
		else if( rank == 6 ) 
		{ 
			RankName6[ group ] = rankname; 
			SendClientMessageEx( playerid, COLOR_GREY, "You've successfully edited rank 6 to %s", rankname ); 
			mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `RankName6`='%s', `Rank1Active`=1 WHERE `ID`='%d'", rankname, group ); 
		}
		else if( rank == 7 ) 
		{ 
			RankName7[ group ] = rankname; 
			SendClientMessageEx( playerid, COLOR_GREY, "You've successfully edited rank 7 to %s", rankname ); 
			mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `RankName7`='%s', `Rank1Active`=1 WHERE `ID`='%d'", rankname, group ); 
		}
		else if( rank == 8 ) 
		{ 
			RankName8[ group ] = rankname; 
			SendClientMessageEx( playerid, COLOR_GREY, "You've successfully edited rank 8 to %s", rankname ); 
			mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `RankName8`='%s', `Rank1Active`=1 WHERE `ID`='%d'", rankname, group ); 
		}
		mysql_tquery( sqlGameConnection, query );
	}
	return 1;
}

CMD:dmembers( playerid )
{
	new gid = PlayerInfo[ playerid ] [ pDonatedGroup ];
	new dMembers[ 2500 ];
	if( PlayerInfo[ playerid ] [ pDonatedGroup ] == 0 ) SendClientMessage( playerid, COLOR_GREY, "You're not in a group!" );
	else
	{
		for( new i = 0; i <MAX_PLAYERS; i++ )
		{
			if( PlayerInfo[ i ] [ pDonatedGroup ] == gid )
			{
				if(PlayerInfo[ i ] [ pDonatedRank ] == 1)
					format( dMembers, sizeof( dMembers ), "%s\nName: %s, Rank: %d (%s)\n", dMembers, PlayerOOCName( i ), PlayerInfo[ i ] [ pDonatedRank ], RankName1[ PlayerInfo[ i ] [ pDonatedGroup ] ] [ 0 ] );
				if(PlayerInfo[ i ] [ pDonatedRank ] == 2)
					format( dMembers, sizeof( dMembers ), "%s\nName: %s, Rank: %d (%s)\n", dMembers, PlayerOOCName( i ), PlayerInfo[ i ] [ pDonatedRank ], RankName2[ PlayerInfo[ i ] [ pDonatedGroup ] ] [ 0 ] );
				if(PlayerInfo[ i ] [ pDonatedRank ] == 3)
					format( dMembers, sizeof( dMembers ), "%s\nName: %s, Rank: %d (%s)\n", dMembers, PlayerOOCName( i ), PlayerInfo[ i ] [ pDonatedRank ], RankName3[ PlayerInfo[ i ] [ pDonatedGroup ] ] [ 0 ] );
				if(PlayerInfo[ i ] [ pDonatedRank ] == 4)
					format( dMembers, sizeof( dMembers ), "%s\nName: %s, Rank: %d (%s)\n", dMembers, PlayerOOCName( i ), PlayerInfo[ i ] [ pDonatedRank ], RankName4[ PlayerInfo[ i ] [ pDonatedGroup ] ] [ 0 ] );
				if(PlayerInfo[ i ] [ pDonatedRank ] == 5)
					format( dMembers, sizeof( dMembers ), "%s\nName: %s, Rank: %d (%s)\n", dMembers, PlayerOOCName( i ), PlayerInfo[ i ] [ pDonatedRank ], RankName5[ PlayerInfo[ i ] [ pDonatedGroup ] ] [ 0 ] );
				if(PlayerInfo[ i ] [ pDonatedRank ] == 6)
					format( dMembers, sizeof( dMembers ), "%s\nName: %s, Rank: %d (%s)\n", dMembers, PlayerOOCName( i ), PlayerInfo[ i ] [ pDonatedRank ], RankName6[ PlayerInfo[ i ] [ pDonatedGroup ] ] [ 0 ] );
				if(PlayerInfo[ i ] [ pDonatedRank ] == 7)
					format( dMembers, sizeof( dMembers ), "%s\nName: %s, Rank: %d (%s)\n", dMembers, PlayerOOCName( i ), PlayerInfo[ i ] [ pDonatedRank ], RankName7[ PlayerInfo[ i ] [ pDonatedGroup ] ] [ 0 ] );
				if(PlayerInfo[ i ] [ pDonatedRank ] == 8)
					format( dMembers, sizeof( dMembers ), "%s\nName: %s, Rank: %d (%s)\n", dMembers, PlayerOOCName( i ), PlayerInfo[ i ] [ pDonatedRank ], RankName8[ PlayerInfo[ i ] [ pDonatedGroup ] ] [ 0 ] );
				ShowPlayerDialog( playerid, 4432, DIALOG_STYLE_LIST, "Donated-Group Members", dMembers, "Close", "" );
			}
		}
	}
	return 1;
}
CMD:dr( playerid, params[ ] )
{
	new gid = PlayerInfo[ playerid ] [ pDonatedGroup ];
	new rank = PlayerInfo[ playerid ] [ pDonatedRank ];
	new txt[ 350 ];
	if( PlayerInfo[ playerid ] [ pDonatedGroup ] == 0 ) SendClientMessage( playerid, COLOR_GREY, "You're not in any group!" );
	else if( sscanf( params, "s[350]", txt ) ) SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dr <TXT>");
	else
	{
		if( rank == 1 )		format( DonatedGroupStr, sizeof( DonatedGroupStr ), "(%d) %s %s: %s", rank, RankName1[ gid ] [ 0 ], PlayerICName( playerid ), txt );
		else if( rank == 2 )format( DonatedGroupStr, sizeof( DonatedGroupStr ), "(%d) %s %s: %s", rank, RankName2[ gid ] [ 0 ], PlayerICName( playerid ), txt );
		else if( rank == 3 )format( DonatedGroupStr, sizeof( DonatedGroupStr ), "(%d) %s %s: %s", rank, RankName3[ gid ] [ 0 ], PlayerICName( playerid ), txt );
		else if( rank == 4 )format( DonatedGroupStr, sizeof( DonatedGroupStr ), "(%d) %s %s: %s", rank, RankName4[ gid ] [ 0 ], PlayerICName( playerid ), txt );
		else if( rank == 5 )format( DonatedGroupStr, sizeof( DonatedGroupStr ), "(%d) %s %s: %s", rank, RankName5[ gid ] [ 0 ], PlayerICName( playerid ), txt );
		else if( rank == 6 )format( DonatedGroupStr, sizeof( DonatedGroupStr ), "(%d) %s %s: %s", rank, RankName6[ gid ] [ 0 ], PlayerICName( playerid ), txt );
		else if( rank == 7 )format( DonatedGroupStr, sizeof( DonatedGroupStr ), "(%d) %s %s: %s", rank, RankName7[ gid ] [ 0 ], PlayerICName( playerid ), txt );
		else if( rank == 8 )format( DonatedGroupStr, sizeof( DonatedGroupStr ), "(%d) %s %s: %s", rank, RankName8[ gid ] [ 0 ], PlayerICName( playerid ), txt );
		SendDonorMsg( gid, COLOR_HITMAN, DonatedGroupStr );
	}
	return 1;
}
CMD:dor( playerid, params[ ] )
{
	new gid = PlayerInfo[ playerid ] [ pDonatedGroup ];
	new rank = PlayerInfo[ playerid ] [ pDonatedRank ];
	new txt[ 350 ];
	if( PlayerInfo[ playerid ] [ pDonatedGroup ] == 0 ) SendClientMessage( playerid, COLOR_GREY, "You're not in any group!" );
	else if( sscanf( params, "s[350]", txt ) ) SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /dor <TXT>");
	//else if( !GroupActive[ gid ] == false ) 
	else
	{
		if( rank == 1 )		format( DonatedGroupStr, sizeof( DonatedGroupStr ), "(%d) %s %s: (( %s ))", rank, RankName1[ gid ] [ 0 ], PlayerICName( playerid ), txt );
		else if( rank == 2 )format( DonatedGroupStr, sizeof( DonatedGroupStr ), "(%d) %s %s: (( %s ))", rank, RankName2[ gid ] [ 0 ], PlayerICName( playerid ), txt );
		else if( rank == 3 )format( DonatedGroupStr, sizeof( DonatedGroupStr ), "(%d) %s %s: (( %s ))", rank, RankName3[ gid ] [ 0 ], PlayerICName( playerid ), txt );
		else if( rank == 4 )format( DonatedGroupStr, sizeof( DonatedGroupStr ), "(%d) %s %s: (( %s ))", rank, RankName4[ gid ] [ 0 ], PlayerICName( playerid ), txt );
		else if( rank == 5 )format( DonatedGroupStr, sizeof( DonatedGroupStr ), "(%d) %s %s: (( %s ))", rank, RankName5[ gid ] [ 0 ], PlayerICName( playerid ), txt );
		else if( rank == 6 )format( DonatedGroupStr, sizeof( DonatedGroupStr ), "(%d) %s %s: (( %s ))", rank, RankName6[ gid ] [ 0 ], PlayerICName( playerid ), txt );
		else if( rank == 7 )format( DonatedGroupStr, sizeof( DonatedGroupStr ), "(%d) %s %s: (( %s ))", rank, RankName7[ gid ] [ 0 ], PlayerICName( playerid ), txt );
		else if( rank == 8 )format( DonatedGroupStr, sizeof( DonatedGroupStr ), "(%d) %s %s: (( %s ))", rank, RankName8[ gid ] [ 0 ], PlayerICName( playerid ), txt );
		SendDonorMsg( gid, COLOR_HITMAN, DonatedGroupStr );
	}
	return 1;
}
CMD:mygroupperms( playerid )
{
	new gid = PlayerInfo[ playerid ] [ pDonatedGroup ], MaskStr[ 20 ], DisguiseStr[ 20 ], SetUcNameStr[ 20 ], CallerIDStr[ 20 ];
	if( PlayerInfo[ playerid ] [ pDonatedGroup ] == 0 ) SendClientMessage( playerid, COLOR_GREY, "You're not in any group!" );
	else
	{
		if( MaskPerm[ gid ] == 1 ) MaskStr = "{FF00FF}Yes"; if( MaskPerm[ gid ] == 0 ) MaskStr = "{FF0000}No";
		if( DisguisePerm[ gid ] == 1 ) DisguiseStr = "{FF00FF}Yes"; if( DisguisePerm[ gid ] == 0 ) DisguiseStr = "{FF0000}No";
		if( SetUcNamePerm[ gid ] == 1 ) SetUcNameStr = "{FF00FF}Yes"; if( SetUcNamePerm[ gid ] == 0 ) SetUcNameStr = "{FF0000}No";
		if( CallerIDPerm[ gid ] == 1 ) CallerIDStr = "{FF00FF}Yes"; if( CallerIDPerm[ gid ] == 0 ) CallerIDStr = "{FF0000}No";
		SendClientMessageEx( playerid, COLOR_GREY, "{00FFFF}Mask Permission: %s", MaskStr );
		SendClientMessageEx( playerid, COLOR_GREY, "{00FFFF}Disguise Permission: %s", DisguiseStr );
		SendClientMessageEx( playerid, COLOR_GREY, "{00FFFF}Setucname Permission: %s", SetUcNameStr );
		SendClientMessageEx( playerid, COLOR_GREY, "{00FFFF}Callerid Permission: %s", CallerIDStr );
	}
	return 1;
}
/*												PLAYER-CMDS																*/
/*												===========																*/
/*												ADMIN-CMDS																*/
CMD:createdonatedgroup( playerid, params[ ] )
{
	new giveplayerid, groupid = Iter_Free( DonatedGroups ), rank, gname[ 150 ], query[ 300 ];
	if( PlayerInfo[ playerid ] [ pAdmin ] < 1337 ) SendClientMessageEx( playerid, COLOR_GREY, "{ff0000}Error:{ffffff} Your admin level is %d but you need 1337 or above!", PlayerInfo[ playerid ] [ pAdmin ] );
	else if( sscanf( params, "uiis[150]", giveplayerid, groupid, rank, gname ) ) SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /createdonatedgroup <Playerid> <GroupID> <Max_Ranks> <Group Name>" );
	else if( Iter_Contains( DonatedGroups, groupid ) ) SendClientMessage( playerid, -1, "The group ID you've entered already exists!" );
	else if( rank > 8 || rank < 1 ) SendClientMessage( playerid, COLOR_GREY, "Ranks can not be above 8 and below 1!" );
	else if( PlayerInfo[ giveplayerid ] [ pGroup ] >= 1 ) SendClientMessage( playerid, -1, "That player is already in a normal group!" );
	else if( !IsPlayerConnected( giveplayerid ) ) SendClientMessage( playerid, COLOR_GREY, "That player is not connected!" );
	else if(!(strfind( gname, "'", true) == -1)) SendClientMessage( playerid, -1, "< ' > is not allowed character!" );
	else
	{
		DonatedGroupID[ groupid ] = groupid;
		GroupName[ groupid ] = gname;
		GroupMaxRanks[ groupid ] = rank;
		PlayerInfo[ giveplayerid ] [ pDonatedGroup ] = groupid;
		PlayerInfo[ giveplayerid ] [ pDonatedRank ] = rank;
		new name[69];
		name = PlayerName(giveplayerid);
		GroupOwner[ groupid ] = name;
		Iter_Add( DonatedGroups, groupid );
		SendClientMessageEx( playerid, COLOR_GREY, "You've successfully created a group(GID: %d, GNAME: %s, MAX_RANKS:%d for %s)", DonatedGroupID[ groupid ], GroupName[ groupid ][ 0 ], GroupMaxRanks[ groupid ], PlayerOOCName( giveplayerid ) );
		mysql_format( sqlGameConnection, query, sizeof( query ), "INSERT INTO `donatedgroups`(`ID`, `GroupName`, `GroupMaxRanks`, `GroupOwner`) VALUES('%d', '%s', '%d', '%s')", DonatedGroupID[ groupid ], GroupName[ groupid ][ 0 ], GroupMaxRanks[ groupid ], PlayerName( giveplayerid ) );
		mysql_tquery( sqlGameConnection, query );
		GroupActive[ groupid ] = true;
		RankName1[ groupid ] = "None"; 
		Rank1Active [ groupid ] = 1;
		RankName2[ groupid ] = "None"; 
		Rank2Active [ groupid ] = 1;
		RankName3[ groupid ] = "None"; 
		Rank3Active [ groupid ] = 1;
		RankName4[ groupid ] = "None"; 
		Rank4Active [ groupid ] = 1;
		RankName5[ groupid ] = "None"; 
		Rank5Active [ groupid ] = 1;
		RankName6[ groupid ] = "None"; 
		Rank6Active [ groupid ] = 1;
		RankName7[ groupid ] = "None"; 
		Rank7Active [ groupid ] = 1;
		RankName8[ groupid ] = "None"; 
		Rank8Active [ groupid ] = 1;
		MaskPerm[ groupid ] = 0;
		SetUcNamePerm[ groupid ] = 0;
		DisguisePerm[ groupid ] = 0;
		CallerIDPerm[ groupid ] = 0;
	}
	return 1;
}
CMD:makedonatedgroupleader( playerid, params[ ] )
{
	new giveplayerid, groupid;
	if( PlayerInfo[ playerid ] [ pAdmin ] < 1337 ) SendClientMessageEx( playerid, COLOR_GREY, "{ff0000}Error:{ffffff} Your admin level is %d but you need 1337 or above!", PlayerInfo[ playerid ] [ pAdmin ] );
	else if( sscanf( params, "ui", giveplayerid, groupid ) ) SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /makedonatedgroupleader <Playerid> <Group ID>" );
	else if( groupid == 0 )
	{
		SendClientMessageEx( playerid, COLOR_GREY, "You've removed %s from %s!", PlayerOOCName( giveplayerid ), GroupName[ groupid ] );
		SendClientMessageEx( giveplayerid, COLOR_GREY, "%s has removed you from %s!", PlayerOOCName( playerid ), GroupName[ groupid ] );
		PlayerInfo[ giveplayerid ] [ pDonatedGroup ] = 0;
		PlayerInfo[ giveplayerid ] [ pDonatedRank ] = 0;
	}
	else if( !Iter_Contains( DonatedGroups, groupid ) ) SendClientMessage( playerid, -1, "The group ID you've entered does not exist!" );
	else if( PlayerInfo[ giveplayerid ] [ pGroup ] >= 1 ) SendClientMessage( playerid, COLOR_GREY, "That player is already in normal group!" ); 
	else
	{
		SendClientMessageEx( playerid, COLOR_GREY, "You've successfully made %s leader of group id %d|%s", PlayerOOCName( giveplayerid ), groupid, GroupName[ groupid ] );
		SendClientMessageEx( giveplayerid, COLOR_GREY, "%s has made you leader of %s (GID: %d)", PlayerOOCName( playerid ), GroupName[ groupid ], groupid );
		PlayerInfo[ giveplayerid ] [ pDonatedGroup ] = groupid;
		PlayerInfo[ giveplayerid ] [ pDonatedRank ] = GroupMaxRanks[ groupid ];
	}
	return 1;
}

CMD:editdonatedgroup( playerid, params[ ] )
{
	new option[ 40 ], groupid, query[ 300 ], name[ 150 ];
	if( PlayerInfo[ playerid ] [ pAdmin ] < 1337 ) SendClientMessageEx( playerid, COLOR_GREY, "{ff0000}Error:{ffffff} Your admin level is %d but you need 1337 or above!", PlayerInfo[ playerid ] [ pAdmin ] );
	else if(sscanf(params, "ds[200] ", groupid, option))
	{
		SendClientMessage( playerid, COLOR_GREY, " {00BFFF}Usage:{FFFFFF}/editdonatedgroup [Group ID] [Option]" );
		SendClientMessage( playerid, COLOR_GREY, " Option: RName(rank names), GName(group name), Delete, Maskperm, Setucnameperm, Disguiseperm, Calleridperm" );
	}
	else if( !Iter_Contains( DonatedGroups, groupid ) ) SendClientMessage( playerid, -1, "{FF0000}Error:{FFFFFF} The group id you written does not exist!" );
	else if( strcmp( option, "gname", true ) == 0 )
    {
    	if( sscanf( params, "ds[250]s[150]", groupid, option, name ) ) SendClientMessage( playerid, COLOR_GREY, " {00BFFF}Usage:{FFFFFF}/editdonatedgroup [Group ID] [Group Name]" );
    	{
    		if(!(strfind( name, "'", true) == -1)) SendClientMessage( playerid, -1, "< ' > is not allowed character!" );
    		else
    		{
	    		SendClientMessageEx( playerid, COLOR_GREY, "GID: %s, Old GName: %s | New GName: %s", groupid, GroupName[ groupid ] [ 0 ], name );
	    		GroupName[ groupid ] = name;
	    		mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `GroupName`='%s' WHERE `ID`='%d'", name, groupid );
				mysql_tquery( sqlGameConnection, query );
			}
    	}
    }
    else if( strcmp( option, "delete", true ) == 0 )
    {
    	if( sscanf( params, "ds[250]", groupid, option ) ) SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF}/editdonatedgroup [Group ID] [Delete]" );
    	{
	    		for(new id = 0; id < MAX_PLAYERS; id++)
				{
					if( (PlayerInfo[ id ] [ pDonatedGroup ] == groupid ) )
					{
						PlayerInfo[ id ] [ pDonatedGroup ] = 0;
						PlayerInfo[ id ] [ pDonatedRank ] = 0;
						SendClientMessageEx( playerid, COLOR_GREY, "An admin %s has removed your group %s and you're now kicked out of it!", PlayerOOCName( playerid ), GroupName[ groupid ] );
					}
					break;
				}
				SendClientMessageEx( playerid, COLOR_GREY, "Successfully deleted group ID: %d(%s)", groupid, GroupName[ groupid ] [ 0 ] );
				mysql_format( sqlGameConnection, query, sizeof( query ), "DELETE FROM `donatedgroups` WHERE `ID`='%d'", groupid );
				mysql_tquery( sqlGameConnection, query );
				mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `players` SET `DonatedGroup`='0', `DonatedRank`='0' WHERE `DonatedGroup`='%d'", groupid );
				mysql_tquery( sqlGameConnection, query );
				GroupMaxRanks[ groupid ] = 0;
				GroupName[ groupid ] = "None";
				Rank1Active[ groupid ] = 0;
				RankName1[ groupid ] = "None";
				Rank2Active[ groupid ] = 0;
				RankName2[ groupid ] = "None";
				Rank3Active[ groupid ] = 0;
				RankName3[ groupid ] = "None";
				Rank4Active[ groupid ] = 0;
				RankName4[ groupid ] = "None";
				Rank5Active[ groupid ] = 0;
				RankName5[ groupid ] = "None";
				Rank6Active[ groupid ] = 0;
				RankName6[ groupid ] = "None";
				Rank7Active[ groupid ] = 0;
				RankName7[ groupid ] = "None";
				Rank8Active[ groupid ] = 0;
				RankName8[ groupid ] = "None";
				GroupActive[ groupid ] = false;
				Iter_Remove( DonatedGroups, groupid );
    	}
    }
    else if( strcmp( option, "maskperm", true ) == 0 ) //SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF}/editdonatedgroup [Group ID] [Maskperm]" );
    {
    	if( sscanf( params, "ds[250]", groupid, option ) ) SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF}/editdonatedgroup [Group ID] [Maskperm]" );
    	{
    		if( MaskPerm[ groupid ] == 0 )
    		{
				SendClientMessageEx( playerid, COLOR_GREY, "Successfully enabled mask permission for group ID %d", groupid );
				MaskPerm[ groupid ] = 1;
				mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `MaskPerm`=1 WHERE `ID`='%d'", groupid );
				mysql_tquery( sqlGameConnection, query );
			}
			else if( MaskPerm[ groupid ] == 1 )
			{
				SendClientMessageEx( playerid, COLOR_GREY, "Successfully disabled mask permission for group ID %d", groupid );
				MaskPerm[ groupid ] = 0;
				mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `MaskPerm`=0 WHERE `ID`='%d'", groupid );
				mysql_tquery( sqlGameConnection, query );
			}
			else
			{
				SendClientMessage( playerid, -1, "SYNTAX ERROR CODE: 69, CONTACT SHADOW!" );
			}
		}
    }
    else if( strcmp( option, "setucnameperm", true ) == 0 ) //SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF}/editdonatedgroup [Group ID] [Setucnameperm]" );
    {
    	if( sscanf( params, "ds[250]", groupid, option ) ) SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF}/editdonatedgroup [Group ID] [SetUcNamePerm]" );
    	{
    		if( SetUcNamePerm[ groupid ] == 0 )
    		{
				SendClientMessageEx( playerid, COLOR_GREY, "Successfully enabled setucname permission for group ID %d", groupid );
				SetUcNamePerm[ groupid ] = 1;
				mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `SetUcNamePerm`=1 WHERE `ID`='%d'", groupid );
				mysql_tquery( sqlGameConnection, query );
			}
			else if( SetUcNamePerm[ groupid ] == 1 )
			{
				SendClientMessageEx( playerid, COLOR_GREY, "Successfully disabled setucname permission for group ID %d", groupid );
				SetUcNamePerm[ groupid ] = 0;
				mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `SetUcNamePerm`=0 WHERE `ID`='%d'", groupid );
				mysql_tquery( sqlGameConnection, query );
			}
			else
			{
				SendClientMessage( playerid, -1, "SYNTAX ERROR CODE: 69, CONTACT SHADOW!" );
			}
		}
    }
    else if( strcmp( option, "disguiseperm", true ) == 0 ) //SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF}/editdonatedgroup [Group ID] [Disguiseperm]" );
    {
    	if( sscanf( params, "ds[250]", groupid, option ) ) SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF}/editdonatedgroup [Group ID] [DisguisePerm]" );
    	{
    		if( DisguisePerm[ groupid ] == 0 )
    		{
				SendClientMessageEx( playerid, COLOR_GREY, "Successfully enabled disguiseperm permission for group ID %d", groupid );
				DisguisePerm[ groupid ] = 1;
				mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `DisguisePerm`=1 WHERE `ID`='%d'", groupid );
				mysql_tquery( sqlGameConnection, query );
			}
			else if( DisguisePerm[ groupid ] == 1 )
			{
				SendClientMessageEx( playerid, COLOR_GREY, "Successfully disabled disguiseperm permission for group ID %d", groupid );
				DisguisePerm[ groupid ] = 0;
				mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `DisguisePerm`=0 WHERE `ID`='%d'", groupid );
				mysql_tquery( sqlGameConnection, query );
			}
			else
			{
				SendClientMessage( playerid, -1, "SYNTAX ERROR CODE: 69, CONTACT SHADOW!" );
			}
		}
    }
    else if( strcmp( option, "calleridperm", true ) == 0 ) //SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF}/editdonatedgroup [Group ID] [Calleridperm]" );
    {
    	if( sscanf( params, "ds[250]", groupid, option ) ) SendClientMessage( playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF}/editdonatedgroup [Group ID] [CallerIDPerm]" );
    	{
    		if( CallerIDPerm[ groupid ] == 0 )
    		{
				SendClientMessageEx( playerid, COLOR_GREY, "Successfully enabled calleridperm permission for group ID %d", groupid );
				CallerIDPerm[ groupid ] = 1;
				mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `CallerIDPerm`=1 WHERE `ID`='%d'", groupid );
				mysql_tquery( sqlGameConnection, query );
			}
			else if( CallerIDPerm[ groupid ] == 1 )
			{
				SendClientMessageEx( playerid, COLOR_GREY, "Successfully disabled calleridpem permission for group ID %d", groupid );
				CallerIDPerm[ groupid ] = 0;
				mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `CallerIDPerm`=0 WHERE `ID`='%d'", groupid );
				mysql_tquery( sqlGameConnection, query );
			}
			else
			{
				SendClientMessage( playerid, -1, "SYNTAX ERROR CODE: 69, CONTACT SHADOW!" );
			}
		}
    }
	else if( strcmp( option, "rname", true ) == 0 )
    {
    	new rankname[ 25 ], rank;
    	if( sscanf( params, "ds[200]ds[25]", groupid, option, rank, rankname ) ) SendClientMessage( playerid, COLOR_GREY, " {00BFFF}Usage:{FFFFFF}/editdonatedgroup [Group ID] [Rank ID] [Rank Name]" );
    	{
    		if( rank > GroupMaxRanks[ groupid ] || rank < GroupMaxRanks[ groupid ]+1-GroupMaxRanks[ groupid ] ) SendClientMessageEx( playerid, COLOR_GREY, "This group has max %d ranks starting from 1!", GroupMaxRanks[ groupid ] );
    		else
    		{
    			if( rank == 1 )
    			{
    				RankName1[ groupid ] = rankname;
    				Rank1Active[ groupid ] = 1;
    				SendClientMessageEx( playerid, COLOR_GREY, "You adjusted rank 1 name of groupid %d to %s", groupid, RankName1[ groupid ] );
    				mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `RankName1`='%s', `Rank1Active`=1 WHERE `ID`='%d'", rankname, groupid );
					mysql_tquery( sqlGameConnection, query );
    			}
    			else if( rank == 2 )
    			{
    				RankName2[ groupid ] = rankname;
    				Rank2Active[ groupid ] = 1;
    				SendClientMessageEx( playerid, COLOR_GREY, "You adjusted rank 2 name of groupid %d to %s", groupid, RankName2[ groupid ] );
    				mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `RankName2`='%s', `Rank2Active`=1 WHERE `ID`='%d'", rankname, groupid );
					mysql_tquery( sqlGameConnection, query );
    			}
    			else if( rank == 3 )
    			{
    				RankName3[ groupid ] = rankname;
    				Rank3Active[ groupid ] = 1;
    				SendClientMessageEx( playerid, COLOR_GREY, "You adjusted rank 3 name of groupid %d to %s", groupid, RankName3[ groupid ] );
    				mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `RankName3`='%s', `Rank3Active`=1 WHERE `ID`='%d'", rankname, groupid );
					mysql_tquery( sqlGameConnection, query );
    			}
    			else if( rank == 4 )
    			{
    				RankName4[ groupid ] = rankname;
    				Rank4Active[ groupid ] = 1;
    				SendClientMessageEx( playerid, COLOR_GREY, "You adjusted rank 4 name of groupid %d to %s", groupid, RankName4[ groupid ] );
    				mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `RankName4`='%s', `Rank4Active`=1 WHERE `ID`='%d'", rankname, groupid );
					mysql_tquery( sqlGameConnection, query );
    			}
    			else if( rank == 5 )
    			{
    				RankName5[ groupid ] = rankname;
    				Rank5Active[ groupid ] = 1;
    				SendClientMessageEx( playerid, COLOR_GREY, "You adjusted rank 5 name of groupid %d to %s", groupid, RankName5[ groupid ] );
    				mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `RankName5`='%s', `Rank5Active`=1 WHERE `ID`='%d'", rankname, groupid );
					mysql_tquery( sqlGameConnection, query );
    			}
    			else if( rank == 6 )
    			{
    				RankName6[ groupid ] = rankname;
    				Rank6Active[ groupid ] = 1;
    				SendClientMessageEx( playerid, COLOR_GREY, "You adjusted rank 6 name of groupid %d to %s", groupid, RankName6[ groupid ] );
    				mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `RankName6`='%s', `Rank6Active`=1 WHERE `ID`='%d'", rankname, groupid );
					mysql_tquery( sqlGameConnection, query );
    			}
    			else if( rank == 7 )
    			{
    				RankName7[ groupid ] = rankname;
    				Rank7Active[ groupid ] = 1;
    				SendClientMessageEx( playerid, COLOR_GREY, "You adjusted rank 7 name of groupid %d to %s", groupid, RankName7[ groupid ] );
    				mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `RankName7`='%s', `Rank7Active`=1 WHERE `ID`='%d'", rankname, groupid );
					mysql_tquery( sqlGameConnection, query );
    			}
    			else if( rank == 8 )
    			{
    				RankName8[ groupid ] = rankname;
    				Rank8Active[ groupid ] = 1;
    				SendClientMessageEx( playerid, COLOR_GREY, "You adjusted rank 8 name of groupid %d to %s", groupid, RankName8[ groupid ] );
    				mysql_format( sqlGameConnection, query, sizeof( query ), "UPDATE `donatedgroups` SET `RankName8`='%s', `Rank8Active`=1 WHERE `ID`='%d'", rankname, groupid );
					mysql_tquery( sqlGameConnection, query );
    			}
    		}
    	}   
    }
	return 1;
}
CMD:donatedgroups( playerid )
{
	new dest[2500];
	if( PlayerInfo[ playerid ] [ pAdmin ] < 1337 ) SendClientMessageEx( playerid, COLOR_GREY, "{ff0000}Error:{ffffff} Your admin level is %d but you need 1337 or above!", PlayerInfo[ playerid ] [ pAdmin ] );
	else
	{
		for( new i = 0; i <MAX_DONATED_GROUPS; i++ )
		{
			if(GroupActive[ i ] == true)
			{
				format( dest,sizeof( dest ),"%s\n{00FF00}Group ID: %d, Group Name: %s, Owner: %s\n", dest, DonatedGroupID[ i ], GroupName[ i ] [ 0 ], GroupOwner[ i ] [ 0 ] );
				ShowPlayerDialog( playerid, DIALOG_DONATED_GROUPS, DIALOG_STYLE_LIST, "Donated Groups", dest, "Close", "" );
	        }
		}
		SendClientMessage( playerid, COLOR_RED, "(Do not forget to /namechanges if account doesn't exist)" );
	}
	return 1;
}
/*												ADMIN-CMDS																*/
forward LoadDonatedGroups();
public LoadDonatedGroups()
{
	new value[150];
	new rows = cache_num_rows( );
 	if( rows )
  	{
   		new id, loaded;
		while( loaded < rows )
		{
  			id = cache_get_field_content_int( loaded, "ID" );
  			DonatedGroupID[ id ] = id;
  			GroupMaxRanks[ id ] = cache_get_field_content_int( loaded, "GroupMaxRanks" );
  			cache_get_field_content(loaded, "GroupName", value);
        	format(GroupName[ id ] [ 0 ], MAX_DONATED_GROUPS + 1, "%s", value);
        	Rank1Active[ id ] = cache_get_field_content_int( loaded, "Rank1Active" );
        	cache_get_field_content(loaded, "RankName1", value);
        	format(RankName1[ id ] [ 0 ], MAX_DONATED_GROUPS + 1, "%s", value);
        	Rank2Active[ id ] = cache_get_field_content_int( loaded, "Rank2Active" );
        	cache_get_field_content(loaded, "RankName2", value);
        	format(RankName2[ id ] [ 0 ], MAX_DONATED_GROUPS + 1, "%s", value);
        	Rank3Active[ id ] = cache_get_field_content_int( loaded, "Rank3Active" );
       		cache_get_field_content(loaded, "RankName3", value);
        	format(RankName3[ id ] [ 0 ], MAX_DONATED_GROUPS + 1, "%s", value);
       		Rank4Active[ id ] = cache_get_field_content_int( loaded, "Rank4Active" );
        	cache_get_field_content(loaded, "RankName4", value);
        	format(RankName4[ id ] [ 0 ], MAX_DONATED_GROUPS + 1, "%s", value);
        	Rank5Active[ id ] = cache_get_field_content_int( loaded, "Rank5Active" );
        	cache_get_field_content(loaded, "RankName5", value);
        	format(RankName5[ id ] [ 0 ], MAX_DONATED_GROUPS + 1, "%s", value);
        	Rank6Active[ id ] = cache_get_field_content_int( loaded, "Rank6Active" );
        	cache_get_field_content(loaded, "RankName6", value);
        	format(RankName6[ id ] [ 0 ], MAX_DONATED_GROUPS + 1, "%s", value);
        	Rank7Active[ id ] = cache_get_field_content_int( loaded, "Rank7Active" );
        	cache_get_field_content(loaded, "RankName7", value);
        	format(RankName7[ id ] [ 0 ], MAX_DONATED_GROUPS + 1, "%s", value);
        	Rank8Active[ id ] = cache_get_field_content_int( loaded, "Rank8Active" );
        	cache_get_field_content(loaded, "RankName8", value);
        	format(RankName8[ id ] [ 0 ], MAX_DONATED_GROUPS + 1, "%s", value);
        	cache_get_field_content(loaded, "GroupOwner", value);
        	format(GroupOwner[ id ] [ 0 ], MAX_DONATED_GROUPS + 1, "%s", value);
        	MaskPerm[ id ] = cache_get_field_content_int( loaded, "MaskPerm" );
        	SetUcNamePerm[ id ] = cache_get_field_content_int( loaded, "SetUcNamePerm" );
        	DisguisePerm[ id ] = cache_get_field_content_int( loaded, "DisguisePerm" );
        	CallerIDPerm[ id ] = cache_get_field_content_int( loaded, "CallerIDPerm" );
     	    Iter_Add( DonatedGroups, id );
		    loaded ++;
		    GroupActive[ id ] = true;
	    }
	    printf( "Loaded %d donated groups.", loaded );
	}	
	return 1;
}
SendDonorMsg( member, color, string2[] )
{
	foreach( new i: Player )
	{
		if( PlayerInfo[ i ] [ pDonatedGroup ] == member )
		{
			SendClientMessage( i, color, string2 );
		}
	}
	return 1;
}

Hook:grp_OnPlayerConnect( playerid )
{
	PlayerIG[ playerid ] = INVALID_PLAYER_ID;
	DonorGroup[ playerid ] = -1;
	return 1;
}