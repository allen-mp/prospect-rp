//======================[ INI find existing mySQL user ]========================
stock INI_FindHouseUserID(name[], hid)
{
  new query[120];
  mysql_format(sqlGameConnection, query, sizeof(query), "SELECT `ID` FROM `players` WHERE `name` = '%e' LIMIT 1", name);
  mysql_tquery(sqlGameConnection, query, "INI_OnFindHouseUserID", "si", name, hid);
  return 1;
}
forward INI_OnFindHouseUserID(name[], idx); //limited to one row
public INI_OnFindHouseUserID(name[], idx)
{
  HouseInfo[idx][hOwnerID] = cache_get_field_content_int(0, "ID");
  
  new saveQuery[5000];
  mysql_format(sqlGameConnection, saveQuery, sizeof(saveQuery), "INSERT INTO `houses` (`OwnerID`, `Created`, `Owned`, `Value`,");
  mysql_format(sqlGameConnection, saveQuery, sizeof(saveQuery), "%s `OutsideX`, `OutsideY`, `OutsideZ`, `OutsideA`, `InsideX`,", saveQuery);
  mysql_format(sqlGameConnection, saveQuery, sizeof(saveQuery), "%s `InsideY`, `InsideZ`, `InsideA`, `Interior`, `Locked`,", saveQuery);
  mysql_format(sqlGameConnection, saveQuery, sizeof(saveQuery), "%s `Alarm`, `AlarmStatus`, `Cash`, `Materials`, `Pot`,", saveQuery);
  mysql_format(sqlGameConnection, saveQuery, sizeof(saveQuery), "%s `Crack`, `Gun1`, `Gun2`, `Gun3`, `Gun4`, `Tag`, `Level`)", saveQuery);

  mysql_format(sqlGameConnection, saveQuery, sizeof(saveQuery), "%s VALUES(%d, %d, %d, %d, %f, %f, %f, %f, %f, %f, %f, %f,", saveQuery,
  HouseInfo[idx][hOwnerID], HouseInfo[idx][hCreated], HouseInfo[idx][hOwned], HouseInfo[idx][hValue], HouseInfo[idx][hOutsideX],
  HouseInfo[idx][hOutsideY], HouseInfo[idx][hOutsideZ], HouseInfo[idx][hOutsideA], HouseInfo[idx][hInsideX], HouseInfo[idx][hInsideY],
  HouseInfo[idx][hInsideZ], HouseInfo[idx][hInsideA]);

  mysql_format(sqlGameConnection, saveQuery, sizeof(saveQuery), "%s %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%e', %d)", saveQuery,
  HouseInfo[idx][hInterior], HouseInfo[idx][hLocked], HouseInfo[idx][hAlarm], HouseInfo[idx][hAlarmStatus],
  HouseInfo[idx][hCash], HouseInfo[idx][hMaterials], HouseInfo[idx][hPot], HouseInfo[idx][hCrack],
  HouseInfo[idx][hGun1], HouseInfo[idx][hGun2], HouseInfo[idx][hGun3], HouseInfo[idx][hGun4],
  HouseInfo[idx][hTag], HouseInfo[idx][hLevel]);
  mysql_tquery(sqlGameConnection, saveQuery);
  return 1;
}

stock convertHouses()
{
  new
    arrCoords[26][126], strFromFile2[256], File: file = fopen("houses.cfg", io_read);
  
  if(file)
  {
    new idx = 1;
    while(idx < sizeof(HouseInfo))
    {
      fread(file, strFromFile2);
      split(strFromFile2, arrCoords, '|');
      HouseInfo[idx][hCreated] = strval(arrCoords[1]);
      strmid(HouseInfo[idx][hOwner], arrCoords[0], 0, strlen(arrCoords[0]), 255);
      HouseInfo[idx][hOwned] = strval(arrCoords[2]);
      HouseInfo[idx][hValue] = strval(arrCoords[3]);
      HouseInfo[idx][hOutsideX] = floatstr(arrCoords[4]);
      HouseInfo[idx][hOutsideY] = floatstr(arrCoords[5]);
      HouseInfo[idx][hOutsideZ] = floatstr(arrCoords[6]);
      HouseInfo[idx][hOutsideA] = floatstr(arrCoords[7]);
      HouseInfo[idx][hInsideX] = floatstr(arrCoords[8]);
      HouseInfo[idx][hInsideY] = floatstr(arrCoords[9]);
      HouseInfo[idx][hInsideZ] = floatstr(arrCoords[10]);
      HouseInfo[idx][hInsideA] = floatstr(arrCoords[11]);
      HouseInfo[idx][hInterior] = strval(arrCoords[12]);
      HouseInfo[idx][hLocked] = strval(arrCoords[13]);
      HouseInfo[idx][hAlarm] = strval(arrCoords[14]);
      HouseInfo[idx][hAlarmStatus] = strval(arrCoords[15]);
      HouseInfo[idx][hCash] = strval(arrCoords[16]);
      HouseInfo[idx][hMaterials] = strval(arrCoords[17]);
      HouseInfo[idx][hPot] = strval(arrCoords[18]);
      HouseInfo[idx][hCrack] = strval(arrCoords[19]);
      HouseInfo[idx][hGun1] = strval(arrCoords[20]);
      HouseInfo[idx][hGun2] = strval(arrCoords[21]);
      HouseInfo[idx][hGun3] = strval(arrCoords[22]);
      HouseInfo[idx][hGun4] = strval(arrCoords[23]);
      strmid(HouseInfo[idx][hTag], arrCoords[24], 0, strlen(arrCoords[24]), 255);
      new rpos = strfind(HouseInfo[idx][hTag], "\r", true);
      new npos = strfind(HouseInfo[idx][hTag], "\n", true);
      if(rpos != -1)
      {
        strdel(HouseInfo[idx][hTag], rpos, rpos +2);
      }
      if(npos != -1)
      {
        strdel(HouseInfo[idx][hTag], npos, npos +2);
      }
      HouseInfo[idx][hLevel] = strval(arrCoords[25]);
      
      INI_FindHouseUserID(arrCoords[0], idx); //find ownerID (user ID in mysql), insert into mysql
      
      idx++;
    }
    fclose(file);
    printf("INSERT INTO `houses` conversion completed.");
  } else {
    printf("[error] Failed to find houses.cfg");
  }
}

//============================[ House conversion ]==============================
stock LoadHousesFromINI()
{
  new
    arrCoords[26][126], strFromFile2[256], File: file = fopen("houses.cfg", io_read);
  if(file)
  {
    HouseInfo[0][hOutsidePickup] = CreateDynamicPickup(1273, 1, 0.0, 0.0, 0.0);
    new idx = 1;
    while(idx < sizeof(HouseInfo))
    {
      fread(file, strFromFile2);
      split(strFromFile2, arrCoords, '|');
      HouseInfo[idx][hCreated] = strval(arrCoords[1]);
      if(HouseInfo[idx][hCreated])
      {
        strmid(HouseInfo[idx][hOwner], arrCoords[0], 0, strlen(arrCoords[0]), 255);
        HouseInfo[idx][hOwned] = strval(arrCoords[2]);
        HouseInfo[idx][hValue] = strval(arrCoords[3]);
        HouseInfo[idx][hOutsideX] = floatstr(arrCoords[4]);
        HouseInfo[idx][hOutsideY] = floatstr(arrCoords[5]);
        HouseInfo[idx][hOutsideZ] = floatstr(arrCoords[6]);
        HouseInfo[idx][hOutsideA] = floatstr(arrCoords[7]);
        HouseInfo[idx][hInsideX] = floatstr(arrCoords[8]);
        HouseInfo[idx][hInsideY] = floatstr(arrCoords[9]);
        HouseInfo[idx][hInsideZ] = floatstr(arrCoords[10]);
        HouseInfo[idx][hInsideA] = floatstr(arrCoords[11]);
        HouseInfo[idx][hInterior] = strval(arrCoords[12]);
        HouseInfo[idx][hLocked] = strval(arrCoords[13]);
        HouseInfo[idx][hAlarm] = strval(arrCoords[14]);
        HouseInfo[idx][hAlarmStatus] = strval(arrCoords[15]);
        HouseInfo[idx][hCash] = strval(arrCoords[16]);
        HouseInfo[idx][hMaterials] = strval(arrCoords[17]);
        HouseInfo[idx][hPot] = strval(arrCoords[18]);
        HouseInfo[idx][hCrack] = strval(arrCoords[19]);
        HouseInfo[idx][hGun1] = strval(arrCoords[20]);
        HouseInfo[idx][hGun2] = strval(arrCoords[21]);
        HouseInfo[idx][hGun3] = strval(arrCoords[22]);
        HouseInfo[idx][hGun4] = strval(arrCoords[23]);
        strmid(HouseInfo[idx][hTag], arrCoords[24], 0, strlen(arrCoords[24]), 255);
        new rpos = strfind(HouseInfo[idx][hTag], "\r", true);
        new npos = strfind(HouseInfo[idx][hTag], "\n", true);
        if(rpos != -1)
        {
          strdel(HouseInfo[idx][hTag], rpos, rpos +2);
        }
        if(npos != -1)
        {
          strdel(HouseInfo[idx][hTag], npos, npos +2);
        }
        HouseInfo[idx][hLevel] = strval(arrCoords[25]);
        INI_FindHouseUserID(arrCoords[0], idx); //find ownerID (user ID in mysql), insert into mysql


        if(HouseInfo[idx][hOutsidePickup])
        {
          DestroyDynamicPickup(HouseInfo[idx][hOutsidePickup]);
        }
        HouseInfo[idx][hOutsidePickup] = CreateDynamicPickup(1273, 1, HouseInfo[idx][hOutsideX], HouseInfo[idx][hOutsideY], HouseInfo[idx][hOutsideZ]);
        new location[MAX_ZONE_NAME], string3[250];
        Get2DZone(location, MAX_ZONE_NAME, HouseInfo[idx][hOutsideX], HouseInfo[idx][hOutsideY], HouseInfo[idx][hOutsideZ]);
        if(HouseInfo[idx][hOwned] == 1)
        {
          format(string3, sizeof(string3), "{5BC435}%s{FFFFFF}\nOwner: %s\nAddress: %d %s\nType /enter to go inside.", HouseInfo[idx][hTag], HouseInfo[idx][hOwner], idx, location);
          HouseInfoText3D[idx] = CreateDynamic3DTextLabel(string3, COLOR_HOUSETEXT, HouseInfo[idx][hOutsideX], HouseInfo[idx][hOutsideY], HouseInfo[idx][hOutsideZ] +0.88, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
          UpdateDynamic3DTextLabelText(HouseInfoText3D[idx], COLOR_HOUSETEXT, string3);
        }
        else
        {
          format(string3, sizeof(string3), "{5BC435}This property is for sale!{FFFFFF}\nAddress: %d %s\nType /buyhouse to buy it for $%d", idx, location, HouseInfo[idx][hValue]);
          HouseInfoText3D[idx] = CreateDynamic3DTextLabel(string3, COLOR_HOUSETEXT, HouseInfo[idx][hOutsideX], HouseInfo[idx][hOutsideY], HouseInfo[idx][hOutsideZ] +0.88, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100);
          UpdateDynamic3DTextLabelText(HouseInfoText3D[idx], COLOR_HOUSETEXT, string3);
        }
        format(str, sizeof(str), "%0.2f,%0.2f,%0.2f", HouseInfo[idx][hOutsideX], HouseInfo[idx][hOutsideY], HouseInfo[idx][hOutsideZ]);
        if(strcmp(str, "0.00,0.00,0.00", true) == 0)
        {
          DestroyDynamicPickup(HouseInfo[idx][hOutsidePickup]);
          DestroyDynamic3DTextLabel(HouseInfoText3D[idx]);
          HouseInfo[idx][hOutsidePickup] = 0;
          HouseInfoText3D[idx] = Text3D:INVALID_3DTEXT_ID;
        }

      }
      idx++;
    }
    fclose(file);
    printf("INSERT INTO `houses` conversion completed.");
  } else {
    printf("[error] Failed to find houses.cfg");
  }
  return 1;
}
//==============================================================================