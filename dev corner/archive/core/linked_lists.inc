#include <a_samp>
#include <zcmd>
#include <sscanf2>/*



===============================================================================
  _____       _                 _            _   _
 |_   _|     | |               | |          | | (_)
   | |  _ __ | |_ _ __ ___   __| |_   _  ___| |_ _  ___  _ __
   | | | '_ \| __| '__/ _ \ / _` | | | |/ __| __| |/ _ \| '_ \
  _| |_| | | | |_| | | (_) | (_| | |_| | (__| |_| | (_) | | | |
 |_____|_| |_|\__|_|  \___/ \__,_|\__,_|\___|\__|_|\___/|_| |_|
 
Elite City Role Play: Linked Lists implementation.
Credits:
•   Jacob Malcom (Swift) - producing an easy to understand script.
•	BigETI for memory access plugin and influence.

================================================================================
================================================================================
  _    _
 | |  | |
 | |  | |___  __ _  __ _  ___
 | |  | / __|/ _` |/ _` |/ _ \
 | |__| \__ \ (_| | (_| |  __/
  \____/|___/\__,_|\__, |\___|
                    __/ |
                   |___/
                   

Creating a new linked list:

	new list(my_list);
	
...where "my_list" is a specified name for the list.

Functions:
•   append( my_list, 2 ) => will add the number 2 (the cargo/node), to the end of the list.
•   pop( my_list ) => will remove the last element (node) from the list.
•   list_length( my_list ) => returns the list's length ( same as sizeof() )
•   get_value( get( mylist, i ) ) => get a value in the list stored at index i.



================================================================================
*/
#include <memory>
#define list(%0) list:%0[LINKED_LIST]
#define get_value(%0) get_value_core( Pointer:%0 )
#define get(%0,%1,%2) get_core( %0, %1, Pointer:%2 )

MEM::struct LINKED_LIST {
	LINKED_HEAD,
	LINKED_LAST,
	LINKED_SIZE
}
MEM::struct LINKED_NODE {
	NODE_NEXT,
	NODE_PREV,
	NODE_SIZE,
	NODE_CARGO
}

new list(my_list);

stock append( list:List[], cargo ) {
    //allocate enough space to fit all node based variables (structure).
	new Pointer:node = MEM::calloc( NODE_CARGO + 1 ); //returns 4 + 1.
	if( node ) {
	    //If the linked list is not empty, simply add the value to the end.
		if( List[LINKED_LAST] ) {
		    MEM::set_val( Pointer:List[LINKED_LAST], NODE_NEXT, _:node );
		    MEM::set_val( node, NODE_PREV, List[LINKED_LAST] );
		}
		//Else the list is empty, so set HEAD to reference node.
		else if( List[LINKED_HEAD] == NULL ) {
			List[LINKED_HEAD] = _:node;
		}
		//And of course, you must set the last element to node, now.
		List[LINKED_LAST] = _:node;
		//Increase the size of the linked list by 1.
		List[LINKED_SIZE]++;
		MEM::set_val( node, NODE_SIZE, 1 );
		//Assign the value to the node's cargo.
		MEM::set_val( node, NODE_CARGO, cargo );
		//Return the address of the last node.
		return List[LINKED_LAST];
	}
	return -1;
}
stock pop( list:List[] ) {
	//The list is empty
	if( List[LINKED_HEAD] == NULL ) {
	    return NULL;
	}
	new last = MEM_EX::get_val( Pointer: List[LINKED_LAST]->NODE_PREV );
	if( last ) {
	    MEM::set_val( Pointer: last, NODE_NEXT, NULL );
	    MEM::free( Pointer: List[LINKED_LAST] ); //free this memory address !!!
	    List[LINKED_SIZE]--;
	    return 1;
	}
	//The list is now empty. Get rid of it.
	List[LINKED_HEAD] = NULL;
	List[LINKED_LAST] = NULL;  //This needs reviewing !
	List[LINKED_SIZE] = 0;
	return NULL;
}
stock get_core( list:List[], index, Pointer: current_node = Pointer: NULL ) {
	//allow developers to specify a starting point, if not, start at HEAD.
	//this is to optimize performance.
	index++;
	if( _:current_node == NULL ) {
	    current_node = Pointer: List[LINKED_HEAD];
	}

	new
	    current_index = 0;
	    
	//loop through list until index specified is met.
	while( current_node != Pointer: NULL && current_index <= index ) {
	    current_index++;
	    if( current_index == index ) {
	        //return the node at this index.
	        return _:current_node;
	    }
	    //move to the next node.
	    current_node = Pointer: MEM_EX::get_val( current_node->NODE_NEXT );
	}
	return NULL;
}
stock get_value_core( Pointer: current_node ) {
	//assure the node exists.
	if( _:current_node == NULL ) {
	    return NULL;
	}
	//otherwise, we're good to get the value and return it.
	return MEM_EX::get_val( current_node->NODE_CARGO );
}
stock list_length( list:List[] ) {
	return List[LINKED_SIZE]; //this is why I used this variable.
}

/*
 ______                           _
 |  ____|                         | |
 | |__  __  ____ _ _ __ ___  _ __ | | ___
 |  __| \ \/ / _` | '_ ` _ \| '_ \| |/ _ \
 | |____ >  < (_| | | | | | | |_) | |  __/
 |______/_/\_\__,_|_| |_| |_| .__/|_|\___|
                            | |
                            |_|
*/


main()
{
	print( "Appending cargo '1'" );
	new
		node = append( my_list, 1 );
    new
		value = MEM_EX::get_val(Pointer:( node )->( NODE_CARGO ) );
		
    printf( "Successfully appended cargo: %d", value );
		
	print( "Appending two more values, making the linked list size 3." );
	append( my_list, 2 );
	append( my_list, 9 );
	append( my_list, 15 );
	append( my_list, 91 );
	append( my_list, 5 );
	new list(my_list2);
	new list(my_list3);
	new list(my_list4);
	print("Inserting 100,000 values");
	for( new i; i < 100000; i++ ) {
	    append( my_list, i );
	    append( my_list2, i );
	    append( my_list3, i );
	    append( my_list4, i );
	}
	print("Done inserting 100,000 values into 4 lists.");
	//print all the values in the list.
	new node2 = NULL, value2;
	for( new i = 0; i < list_length( my_list ); i++ ) {
  		value2 = get_value( get( my_list, i, node2 ) );
		#pragma unused value2
	    //printf( "Value at index %d is %d.", i, value2 );
	}
	printf( "The length of the linked list is now: %d", list_length( my_list ) );

	pop( my_list );
	printf( "The new length after popping is: %d", list_length( my_list ) );
	pop( my_list );
	pop( my_list );
}


new list(player_list);

public OnPlayerConnect(playerid)
{
	//if( PlayerInfo[playerid][pAdmin] >= 1 ) {
	append( player_list, playerid ); //all the players who have connected.
	//}
	return 1;
}
public OnPlayerDisconnect(playerid, reason)
{
	//of course I need a delete() method and search()/find() method,
	//before completing the player_list.
	
	return 1;
}


public OnPlayerSpawn(playerid)
{
	SetPlayerInterior(playerid,0);
	TogglePlayerClock(playerid,0);
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
   	return 1;
}

SetupPlayerForClassSelection(playerid)
{
 	SetPlayerInterior(playerid,14);
	SetPlayerPos(playerid,258.4893,-41.4008,1002.0234);
	SetPlayerFacingAngle(playerid, 270.0);
	SetPlayerCameraPos(playerid,256.0815,-43.0475,1004.0234);
	SetPlayerCameraLookAt(playerid,258.4893,-41.4008,1002.0234);
}

public OnPlayerRequestClass(playerid, classid)
{
	SetupPlayerForClassSelection(playerid);
	return 1;
}

public OnGameModeInit()
{
	SetGameModeText("Bare Script");
	ShowPlayerMarkers(1);
	ShowNameTags(1);
	AllowAdminTeleport(1);

	AddPlayerClass(265,1958.3783,1343.1572,15.3746,270.1425,0,0,0,0,-1,-1);

	return 1;
}

