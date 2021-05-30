forward SaveDealershipsMySql();
public SaveDealershipsMySql()
{

	for(new i = 1; i < MAX_DEALERS; i++)
	{
		new query[2500];
		mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `dealerships`  WHERE `dID` = %d SET `d_name` = %s, `d_spawned` = %d,",
		Dealerships[i][dID], Dealerships[i][dName], Dealerships[i][dSpawned]);

		mysql_format(sqlGameConnection, query, sizeof(query), "%s `d_posx` = %f, `d_posy` = %f, `dposz` = %f,",
		query, Dealerships[i][dPos][0], Dealerships[i][dPos][1], Dealerships[i][dPos][2]);

		mysql_format(sqlGameConnection, query, sizeof(query), "%s `d_carx` = %f, `d_cary` = %f, `d_carz` = %f, `dcara` = %f,",
		query, Dealerships[i][dPos][3], Dealerships[i][dPos][4], Dealerships[i][dPos][5], Dealerships[i][dPos][6]);
	
		new ca = 0, str1 [10], str2[10];
			for(new c = 0; c < 10; c++)
			{
				if(Dealerships[i][dCars][c] >= 400)
				{
					format(str1, sizeof(str1), "`car_%d`", ca);
					format(str2, sizeof(str2), "`car_%d_price`", ca);
					mysql_format(sqlGameConnection, query, sizeof(query), "%s %s = %d, %s = %d,",
					query, str1, Dealerships[i][dCars][c], str2, Dealerships[i][dCarPrice][c]);
					ca++;
				}
		}
		mysql_tquery(sqlGameConnection, query);

	}
	return 1;
}

