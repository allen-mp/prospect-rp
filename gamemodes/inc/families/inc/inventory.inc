/*
    

    Family inventories
    @Author: Octet

    
    NOTE: family safes are completely sep. from family data structure.
    They are obtained from the database when required using family ID.

    Database fields of table familyinv will include:
    1. ID,
    2. FID,
    3. ItemID,
    4. Name,
    5. Amount
    NO DUPLICATES for Name


*/

/*
    Definitions
*/

//46 weapons, pot, crack, meth, cash = 50 items
#define MAX_FAMILY_ITEMS 51
#define FAMILY_ITEM_NAME_LEN 20

//Inventory item IDs
#define F_INV_ITEM_GUN_MIN	1
#define F_INV_ITEM_GUN_MAX	46
#define F_INV_ITEM_CASH	47
#define F_INV_ITEM_CANNABIS	48
#define F_INV_ITEM_COCAINE	49
#define F_INV_ITEM_METH	50

/*
    Function prototypes
*/

//public
forward ShowFamilyItems(fid);
forward TakeFamilyItem(fid, lineid, amount); //lineid = sql row (in query)
forward StoreFamilyItem(fid, itemid); //itemid = look above in defines

//protected
//mainly helper functions
forward ShowFamilyItemsEx(fid);
forward TakeFamilyItemEx(fid, lineid, amount);


/*
    Showing Inventory items (in a dialog)
        fetches rows from database, prints out in dialog (no data store)
*/
public ShowFamilyItems(fid) {


    if(0 < fid < MAX_FAMILIES) {
        printf("FAMWARNING: ShowFamilyInventory(%d)", fid);
        return;
    }

    //As getting each item from database, format string and show in dialog (each line = line of SELECT * FROM familyinv WHERE FID=%d)
    inline OnShowFamilyItems() {
        if(cache_num_rows() < 1) {
            return SendClientMessage(playerid, COLOR_LIGHTRED, "No inventory items found.");
        }

        new items[FAMILY_ITEM_NAME_LEN*MAX_FAMILY_ITEMS]; //20*51

        for(new i; i < cache_num_rows(); i++) {
            if(i < MAX_FAMILY_ITEMS) {
                printf("FAMWARNING: FID %d has too many inventory slots in database", fid);
                break;
            }

            static itemname[20], amount;
            cache_get_field_content(i, "Name", itemname);
            amount = cache_get_field_content_int(i, "Amount", amount);
            //Item Name     |   AmountofItem
            format(items, sizeof items, "%s\t%d\n", itemname, amount);
        }

        ShowPlayerDialog(playerid, DIALOG_FAMILY_INVENTORY, DIALOG_STYLE_TABLIST, "Family Inventory", items, "Cancel", "Take");
    }

    //Query to select all items belonging to the family
    new query[150];
    mysql_format(sqlGameConnection, query, sizeof query, "SELECT Name, Amount FROM familyinv WHERE FID = %d", fid);
    mysql_pquery_inline(sqlGameConnection, query, using inline OnShowFamilyItems, "i", fid);

}


/*
    Taking family items
        Example usage: OnDialogResponse, admin take command... takes an inventory item from a player
        Reduces Amount by amount or deletes from table if Amount - amount = 0

        Returns 0 if unsuccessful and 1 if successful
*/
public TakeFamilyItem(fid, lineid, amount) {
    new query[150];
    mysql_format(sqlGameConnection, query, sizeof query, "SELECT ID, Amount FROM familyinv WHERE FID = %d", fid);
    mysql_pquery(sqlGameConnection, query, "TakeFamilyItemEx", "iii", fid, lineid, amount);
    return;
}

public TakeFamilyItemEx(fid, lineid, amount) {
    if(cache_num_rows() < lineid) { 
        printf("FAMWARNING: invalid lineid for famid %d", fid);
        return 0; 
    }

    new sqlid, dbamount;
    sqlid = cache_get_field_content_int(lineid, "ID");
    dbamount = cache_get_field_content_int(lineid, "Amount");
    

    //Update/Delete from database
    new query[100];

    if(dbamount - amount < 0) {
        return 0;
    }

    else if(dbamount - amount > 0) {
    
        mysql_format(sqlGameConnection, query, sizeof query, "UPDATE familyinv SET Amount = %d WHERE ID = %d", (dbamount - amount), sqlid);
        mysql_pquery(sqlGameConnection, query);
    }

    else {

        mysql_format(sqlGameConnection, query, sizeof query, "DELETE FROM familyinv WHERE ID = %d", sqlid);
        mysql_pquery(sqlGameConnection, query);
    }
    return 1;
}