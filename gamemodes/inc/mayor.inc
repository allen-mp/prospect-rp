#define MAYOR_OBJECT_LIMIT MAX_OBJECTS //this is the limit of objects the mayor can have

new bool:mayorObjects[MAYOR_OBJECT_LIMIT]; 
new totalObjects;

stock CreateMayorObject(modelid, Float:X, Float:Y, Float:Z, Float:rX, Float:rY, Float:rZ) {

	if(totalObjects == sizeof(mayorObjects)) {
		print("mayorObjects array is full, increase MAYOR_OBJECT_LIMIT");
		return;
	}
	
	new objectid = CreateDynamicObject(modelid, X, Y, Z, rX, rY, rZ);
	mayorObjects[objectid] = true;
	totalObjects++;
}

stock DestroyMayorObjects() {
	for(new i = 0; i < sizeof(mayorObjects); i ++) {
		if(mayorObjects[i]) {
			DestroyDynamicObjectEx(i);
			mayorObjects[i] = false;
		}
	}
	totalObjects = 0;
}

stock loadMayorObjects() {
	CreateMayorObject(970, 1500.72, -1749.97, 15.00,   0.00, 0.00, 270.00);
	CreateMayorObject(970, 1498.65, -1747.90, 15.00,   0.00, 0.00, 0.00);
	CreateMayorObject(970, 1494.49, -1747.90, 15.00,   0.00, 0.00, 0.00);
	CreateMayorObject(970, 1490.39, -1747.90, 15.00,   0.00, 0.00, 0.00);
	CreateMayorObject(970, 1486.26, -1747.90, 15.00,   0.00, 0.00, 0.00);
	CreateMayorObject(970, 1482.14, -1747.90, 15.00,   0.00, 0.00, 0.00);
	CreateMayorObject(970, 1478.02, -1747.90, 15.00,   0.00, 0.00, 0.00);
	CreateMayorObject(970, 1473.88, -1747.90, 15.00,   0.00, 0.00, 0.00);
	CreateMayorObject(970, 1469.74, -1747.90, 15.00,   0.00, 0.00, 0.00);
	CreateMayorObject(970, 1465.64, -1747.90, 15.00,   0.00, 0.00, 0.00);
	CreateMayorObject(970, 1461.39, -1749.96, 15.00,   0.00, 0.00, 270.00);
	CreateMayorObject(2491, 1481.35, -1748.19, 13.69,   0.00, 0.00, 0.00);
	CreateMayorObject(3877, 1503.63, -1750.69, 34.10,   0.00, 0.00, 0.00);
	CreateMayorObject(3877, 1458.73, -1750.66, 34.10,   0.00, 0.00, 0.00);
	CreateMayorObject(630, 1500.14, -1749.85, 15.42,   0.00, 0.00, 135.00);
	CreateMayorObject(630, 1489.43, -1750.46, 15.42,   0.00, 0.00, 135.00);
	CreateMayorObject(630, 1483.79, -1750.46, 15.42,   0.00, 0.00, 135.00);
	CreateMayorObject(630, 1478.47, -1750.46, 15.40,   0.00, 0.00, 135.00);
	CreateMayorObject(630, 1473.05, -1750.45, 15.45,   0.00, 0.00, 90.00);
	CreateMayorObject(630, 1461.73, -1749.91, 15.47,   0.00, 0.00, 90.00);
	CreateMayorObject(1892, 1580.41, -1720.85, 12.53,   0.00, 0.00, 90.00);
	CreateMayorObject(970, 1584.16, -1739.49, 13.03,   0.00, 0.00, 90.00);
	CreateMayorObject(4100, 1557.94, -1773.63, 14.08,   0.00, 0.00, -39.90);
	CreateMayorObject(4100, 1581.74, -1773.68, 14.08,   0.00, 0.00, -40.62);
	CreateMayorObject(1238, 1572.95, -1778.76, 12.73,   0.00, 0.00, 0.00);
	CreateMayorObject(1238, 1571.02, -1778.86, 12.73,   0.00, 0.00, 0.00);
	CreateMayorObject(1238, 1568.55, -1780.37, 12.73,   0.00, 0.00, 0.00);
	CreateMayorObject(1238, 1566.14, -1780.39, 12.73,   0.00, 0.00, 0.00);
	CreateMayorObject(1425, 1600.97, -1729.64, 12.81,   0.00, 0.00, 89.40);
	CreateMayorObject(1238, 1601.91, -1728.55, 12.68,   0.00, 0.00, 0.00);
	CreateMayorObject(1238, 1602.16, -1730.85, 12.68,   0.00, 0.00, 0.00);
	CreateMayorObject(1238, 1611.33, -1733.78, 12.68,   0.00, 0.00, 0.00);
	CreateMayorObject(1238, 1611.33, -1735.77, 12.68,   0.00, 0.00, 0.00);
	CreateMayorObject(1425, 1609.85, -1734.77, 12.83,   0.00, 0.00, 89.40);
	CreateMayorObject(970, 1584.16, -1724.91, 13.03,   0.00, 0.00, 90.00);
	CreateMayorObject(1892, 1580.47, -1717.89, 12.53,   0.00, 0.00, 90.00);
	CreateMayorObject(970, 1584.16, -1713.92, 13.03,   0.00, 0.00, 90.00);
	CreateMayorObject(3881, 1579.12, -1724.40, 14.37,   0.00, 0.00, 269.46);
	CreateMayorObject(1616, 1584.33, -1714.10, 17.62,   0.00, 0.00, 131.22);
	CreateMayorObject(2984, 1412.37, -1712.85, 13.81,   0.00, 0.00, -6.84);
	CreateMayorObject(2984, 1412.56, -1710.65, 13.81,   0.00, 0.00, -6.84);
	CreateMayorObject(2984, 1412.22, -1715.24, 13.81,   0.00, 0.00, -6.84);
	CreateMayorObject(2984, 1412.06, -1717.44, 13.81,   0.00, 0.00, -6.84);
	CreateMayorObject(2984, 1411.88, -1719.58, 13.81,   0.00, 0.00, -6.84);
	CreateMayorObject(1616, 1560.49, -1714.89, 21.69,   0.00, 0.00, 119.82);
	CreateMayorObject(1616, 1495.80, -1749.84, 20.98,   0.00, 0.00, 283.32);
	CreateMayorObject(1616, 1466.17, -1749.88, 20.98,   0.00, 0.00, 298.62);
	CreateMayorObject(2232, 1470.42, -1750.46, 15.01,   0.00, 0.00, 178.74);
	CreateMayorObject(2232, 1491.80, -1750.35, 15.03,   0.00, 0.00, 178.74);
	CreateMayorObject(1425, 1557.06, -1589.93, 12.79,   0.00, 0.00, 90.00);
	CreateMayorObject(1238, 1554.84, -1588.65, 12.96,   0.00, 0.00, 0.00);
	CreateMayorObject(1238, 1554.87, -1590.90, 12.96,   0.00, 0.00, 0.00);
	CreateMayorObject(1425, 1560.27, -1594.36, 12.83,   0.00, 0.00, 90.00);
	CreateMayorObject(1238, 1557.09, -1593.45, 12.96,   0.00, 0.00, 0.00);
	CreateMayorObject(1238, 1556.91, -1595.68, 12.96,   0.00, 0.00, 0.00);
	CreateMayorObject(970, 1547.22, -1599.62, 13.09,   0.00, 0.00, 90.00);
	CreateMayorObject(970, 1547.22, -1585.08, 13.09,   0.00, 0.00, 90.00);
	CreateMayorObject(3881, 1561.87, -1777.09, 14.35,   0.00, 0.00, 180.00);
	CreateMayorObject(1616, 1551.13, -1785.43, 21.41,   0.00, 0.00, 184.68);
	CreateMayorObject(1238, 1568.21, -1859.10, 12.73,   0.00, 0.00, 0.00);
	CreateMayorObject(1238, 1565.66, -1859.11, 12.73,   0.00, 0.00, 0.00);
	CreateMayorObject(1425, 1566.97, -1862.24, 12.80,   0.00, 0.00, 0.30);
	CreateMayorObject(1238, 1570.98, -1859.07, 12.68,   0.00, 0.00, 0.00);
	CreateMayorObject(1238, 1572.93, -1859.11, 12.73,   0.00, 0.00, 0.00);
	CreateMayorObject(1425, 1571.53, -1862.23, 12.82,   0.00, 0.00, 0.30);
	CreateMayorObject(1425, 1391.54, -1860.24, 12.80,   0.00, 0.00, 0.30);
	CreateMayorObject(1238, 1392.80, -1857.86, 12.66,   0.00, 0.00, 0.00);
	CreateMayorObject(1238, 1390.45, -1857.95, 12.69,   0.00, 0.00, 0.00);
	CreateMayorObject(1425, 1387.02, -1860.28, 12.83,   0.00, 0.00, 0.30);
	CreateMayorObject(1238, 1387.85, -1858.17, 12.66,   0.00, 0.00, 0.00);
	CreateMayorObject(1238, 1385.51, -1858.25, 12.66,   0.00, 0.00, 0.00);
	CreateMayorObject(1238, 1320.86, -1730.57, 12.69,   0.00, 0.00, 0.00);
	CreateMayorObject(1425, 1319.42, -1729.53, 12.81,   0.00, 0.00, -90.00);
	CreateMayorObject(1425, 1319.35, -1734.45, 12.79,   0.00, 0.00, -90.00);
	CreateMayorObject(1238, 1320.89, -1728.64, 12.67,   0.00, 0.00, 0.00);
	CreateMayorObject(1238, 1320.87, -1733.44, 12.69,   0.00, 0.00, 0.00);
	CreateMayorObject(1238, 1320.80, -1735.66, 12.69,   0.00, 0.00, 0.00);
	CreateMayorObject(970, 1322.84, -1724.37, 13.10,   0.00, 0.00, 90.00);
	CreateMayorObject(970, 1322.83, -1739.88, 13.10,   0.00, 0.00, 90.00);
	CreateMayorObject(979, 1324.06, -1732.17, 13.02,   0.00, 0.00, 90.00);
	CreateMayorObject(979, 1569.56, -1775.53, 13.13,   0.00, 0.00, 180.00);
	CreateMayorObject(979, 1584.16, -1732.13, 13.10,   0.00, 0.00, -90.00);
	CreateMayorObject(979, 1545.80, -1592.14, 13.15,   0.00, 0.00, -90.00);
	CreateMayorObject(979, 1429.40, -1578.79, 13.11,   0.00, 0.00, 0.00);
	CreateMayorObject(970, 1437.05, -1578.98, 13.06,   0.00, 0.00, 0.00);
	CreateMayorObject(970, 1422.01, -1578.97, 13.06,   0.00, 0.00, 0.00);
	CreateMayorObject(970, 1405.96, -1584.56, 13.06,   0.00, 0.00, 90.00);
	CreateMayorObject(970, 1406.04, -1600.36, 13.06,   0.00, 0.00, 90.00);
	CreateMayorObject(979, 1406.01, -1592.36, 13.11,   0.00, 0.00, 90.00);
	CreateMayorObject(1616, 1420.01, -1578.32, 21.33,   0.00, 0.00, 179.58);
	CreateMayorObject(1616, 1406.41, -1602.57, 20.86,   0.00, 0.00, 277.20);
	CreateMayorObject(1616, 1365.45, -1741.89, 18.84,   0.00, 0.00, -87.24);
	CreateMayorObject(979, 1389.12, -1854.65, 13.13,   0.00, 0.00, 180.00);
	CreateMayorObject(970, 1396.48, -1854.78, 13.15,   0.00, 0.00, 0.00);
	CreateMayorObject(970, 1381.65, -1855.02, 13.15,   0.00, 0.00, 0.00);
	CreateMayorObject(970, 1582.10, -1715.98, 13.03,   0.00, 0.00, 180.00);
}
