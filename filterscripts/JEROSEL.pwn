#include <a_samp>
#include <streamer>


#define MAX_STATIC_OBJECTS 200
new StaticObject[MAX_STATIC_OBJECTS],
	TotalStaticObjects;

stock CreateStaticObject(modelid, Float:X, Float:Y, Float:Z, Float:rX, Float:rY, Float:rZ)
{
	new Object = CreateObject(modelid, X, Y, Z, rX, rY, rZ);
	StaticObject[Object]++;
	TotalStaticObjects++;
	return 1;
}

public OnPlayerDisconnect( playerid, reason ) {

	return 1;
}
//============================
public OnFilterScriptInit() {
	LoadJessaErosel();
	return 1;
}

public OnFilterScriptExit() {
	return 1;
}
LoadJessaErosel()
{
  // Jessica Erosel Custom Map

new TempObject;

TempObject = CreateDynamicObject(18981, 2354.42334, 259.44592, 17.25471,   0.00000, 0.00000, 0.00000);
SetDynamicObjectMaterial(TempObject, 0, 8839, "vgsecarshow", "sw_wallbrick_06", 0);
TempObject = CreateDynamicObject(18981, 2354.48413, 224.75653, 17.25471,   0.00000, 0.00000, 0.00000);
SetDynamicObjectMaterial(TempObject, 0, 8839, "vgsecarshow", "sw_wallbrick_06", 0);
TempObject = CreateDynamicObject(18981, 2366.46289, 212.31561, 17.25471,   0.00000, 0.00000, -90.00014);
SetDynamicObjectMaterial(TempObject, 0, 8839, "vgsecarshow", "sw_wallbrick_06", 0);
TempObject = CreateDynamicObject(18981, 2366.43604, 271.27505, 17.17471,   0.00000, 0.00000, -90.84011);
SetDynamicObjectMaterial(TempObject, 0, 8839, "vgsecarshow", "sw_wallbrick_06", 0);
TempObject = CreateDynamicObject(18981, 2379.46069, 271.05652, 17.17471,   0.00000, 0.00000, -90.78011);
SetDynamicObjectMaterial(TempObject, 0, 8839, "vgsecarshow", "sw_wallbrick_06", 0);
TempObject = CreateDynamicObject(18981, 2379.24634, 212.28941, 17.26472,   0.00000, 0.00000, -90.16014);
SetDynamicObjectMaterial(TempObject, 0, 8839, "vgsecarshow", "sw_wallbrick_06", 0);
TempObject = CreateDynamicObject(18981, 2391.35498, 258.17413, 17.16471,   0.00000, 0.00000, 0.00000);
SetDynamicObjectMaterial(TempObject, 0, 8839, "vgsecarshow", "sw_wallbrick_06", 0);
TempObject = CreateDynamicObject(18981, 2391.37427, 241.82768, 17.16471,   0.00000, 0.00000, 0.00000);
SetDynamicObjectMaterial(TempObject, 0, 8839, "vgsecarshow", "sw_wallbrick_06", 0);
TempObject = CreateDynamicObject(18981, 2391.35229, 224.22284, 17.16471,   0.00000, 0.00000, 0.00000);
SetDynamicObjectMaterial(TempObject, 0, 8839, "vgsecarshow", "sw_wallbrick_06", 0);
TempObject = CreateDynamicObject(18981, 2366.62207, 224.88843, 24.94717,   0.00000, -89.59997, -0.24000);
SetDynamicObjectMaterial(TempObject, 0, 9495, "vict_sfw", "Grass_128HV", 0);
TempObject = CreateDynamicObject(18981, 2366.44189, 249.59822, 24.94204,   0.00000, -89.59997, 0.00000);
SetDynamicObjectMaterial(TempObject, 0, 9495, "vict_sfw", "Grass_128HV", 0);
TempObject = CreateDynamicObject(18981, 2366.56006, 259.02039, 24.95204,   0.00000, -89.59997, 0.00000);
SetDynamicObjectMaterial(TempObject, 0, 9495, "vict_sfw", "Grass_128HV", 0);
TempObject = CreateDynamicObject(18981, 2378.20068, 258.27267, 24.86837,   0.00000, -89.59997, 0.00000);
SetDynamicObjectMaterial(TempObject, 0, 9495, "vict_sfw", "Grass_128HV", 0);
TempObject = CreateDynamicObject(18981, 2378.45410, 234.29892, 24.86837,   0.00000, -89.59997, 0.00000);
SetDynamicObjectMaterial(TempObject, 0, 9495, "vict_sfw", "Grass_128HV", 0);
TempObject = CreateDynamicObject(18981, 2378.42749, 225.13890, 24.86837,   0.00000, -89.59997, 0.00000);
SetDynamicObjectMaterial(TempObject, 0, 9495, "vict_sfw", "Grass_128HV", 0);

CreateDynamicObject(3605, 2374.862304, 243.046005, 31.556993, 0.000000, 0.000000, 90.300071);
CreateDynamicObject(6965, 2376.498779, 217.793167, 23.804220, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(1482, 2358.303955, 269.509674, 26.506855, 0.000000, 0.000000, 89.199890);
CreateDynamicObject(3802, 2366.993896, 244.981674, 28.206487, 0.000000, 0.000000, 179.999984);
CreateDynamicObject(3802, 2366.993896, 239.411834, 28.206487, 0.000000, 0.000000, 179.999984);
CreateDynamicObject(870, 2380.314453, 268.432189, 25.590810, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(870, 2380.314453, 265.532135, 25.590810, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(870, 2380.314453, 262.501892, 25.590810, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(870, 2377.742187, 263.661834, 25.590810, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(869, 2377.646240, 267.656219, 25.830167, 0.000000, 0.000000, 81.099998);
CreateDynamicObject(869, 2374.137939, 268.205444, 25.830167, 0.000000, 0.000000, 81.099998);
CreateDynamicObject(869, 2374.804199, 263.981628, 25.830167, 0.000000, 0.000000, 81.099998);
CreateDynamicObject(1825, 2363.572509, 226.661392, 26.215431, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(1825, 2363.572509, 230.081436, 26.215431, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(1825, 2363.572509, 251.681213, 26.215431, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(1825, 2363.572509, 256.111206, 26.215431, 0.000000, 0.000000, 0.000000);

TempObject = CreateDynamicObject(19379, 2323.433349, 1286.204956, 3006.939697, 0.000000, -89.799972, 0.000000);
SetDynamicObjectMaterial(TempObject, 0, 13007, "sw_bankint", "woodfloor1", 0);
TempObject = CreateDynamicObject(19464, 2323.627685, 1293.718383, 3006.894287, 0.000000, -89.799964, 0.000000);
SetDynamicObjectMaterial(TempObject, 0, 16150, "ufo_bar", "dinerfloor01_128", 0);
TempObject = CreateDynamicObject(19464, 2331.199951, 1286.026977, 3006.865478, 0.000000, -89.799964, 0.000000);
SetDynamicObjectMaterial(TempObject, 0, 16150, "ufo_bar", "dinerfloor01_128", 0);
CreateDynamicObject(14720, 2331.750244, 1283.530761, 3006.976806, 0.000000, 0.000000, 89.899978);
CreateDynamicObject(2344, 2362.827392, 1283.301635, 3008.010253, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(19455, 2319.178710, 1286.058593, 3008.656005, 0.000000, 0.000000, 0.200000);
SetDynamicObjectMaterial(TempObject, 0, 8503, "vgs_shop5", "vegaspawnwall_128", 0);
TempObject = CreateDynamicObject(19379, 2323.439941, 1286.204956, 3010.322998, 0.000000, -89.799972, 0.000000);
SetDynamicObjectMaterial(TempObject, 0, 10969, "scum_sfse", "ws_fancywallpink", 0);
TempObject = CreateDynamicObject(19398, 2328.678955, 1285.939697, 3008.728759, 0.000000, 0.000000, 0.000000);
SetDynamicObjectMaterial(TempObject, 0, 8401, "vgshpground", "vegaspawnwall_128", 0);
TempObject = CreateDynamicObject(19398, 2323.741455, 1291.039062, 3008.748779, 0.000000, 0.000000, 89.899932);
SetDynamicObjectMaterial(TempObject, 0, 8401, "vgshpground", "vegaspawnwall_128", 0);
TempObject = CreateDynamicObject(19455, 2333.464843, 1282.997192, 3008.565917, 0.000000, 0.000000, 90.000015);
SetDynamicObjectMaterial(TempObject, 0, 8503, "vgs_shop5", "vegaspawnwall_128", 0);
TempObject = CreateDynamicObject(19455, 2333.464843, 1289.027343, 3008.565917, 0.000000, 0.000000, 90.000015);
SetDynamicObjectMaterial(TempObject, 0, 8503, "vgs_shop5", "vegaspawnwall_128", 0);
TempObject = CreateDynamicObject(19455, 2323.521240, 1296.707885, 3008.645996, 0.000000, 0.000000, 90.000015);
SetDynamicObjectMaterial(TempObject, 0, 8503, "vgs_shop5", "vegaspawnwall_128", 0);
TempObject = CreateDynamicObject(19455, 2317.439941, 1290.948120, 3008.645996, 0.000000, 0.000000, 90.000015);
SetDynamicObjectMaterial(TempObject, 0, 8503, "vgs_shop5", "vegaspawnwall_128", 0);
TempObject = CreateDynamicObject(19455, 2330.071044, 1290.948120, 3008.645996, 0.000000, 0.000000, 90.000015);
SetDynamicObjectMaterial(TempObject, 0, 8503, "vgs_shop5", "vegaspawnwall_128", 0);
TempObject = CreateDynamicObject(19455, 2328.696777, 1279.843872, 3008.656005, 0.000000, 0.000000, 0.200000);
SetDynamicObjectMaterial(TempObject, 0, 8503, "vgs_shop5", "vegaspawnwall_128", 0);
TempObject = CreateDynamicObject(19455, 2328.651855, 1292.303222, 3008.656005, 0.000000, 0.000000, 0.200000);
SetDynamicObjectMaterial(TempObject, 0, 8503, "vgs_shop5", "vegaspawnwall_128", 0);
TempObject = CreateDynamicObject(19455, 2333.301757, 1286.107543, 3008.656005, 0.000000, 0.000000, 0.200000);
SetDynamicObjectMaterial(TempObject, 0, 8503, "vgs_shop5", "vegaspawnwall_128", 0);
TempObject = CreateDynamicObject(19455, 2321.054199, 1295.863281, 3008.656005, 0.000000, 0.000000, 0.200000);
SetDynamicObjectMaterial(TempObject, 0, 8503, "vgs_shop5", "vegaspawnwall_128", 0);
TempObject = CreateDynamicObject(19455, 2326.146484, 1295.881225, 3008.656005, 0.000000, 0.000000, 0.200000);
SetDynamicObjectMaterial(TempObject, 0, 8503, "vgs_shop5", "vegaspawnwall_128", 0);
TempObject = CreateDynamicObject(19379, 2334.011230, 1286.204956, 3010.285156, 0.000000, -89.799972, 0.000000);
SetDynamicObjectMaterial(TempObject, 0, 10969, "scum_sfse", "ws_fancywallpink", 0);
TempObject = CreateDynamicObject(19379, 2324.318847, 1295.724243, 3010.318359, 0.000000, -89.799972, 0.000000);
SetDynamicObjectMaterial(TempObject, 0, 10969, "scum_sfse", "ws_fancywallpink", 0);
TempObject = CreateDynamicObject(19426, 2319.233886, 1286.327026, 3008.806640, 89.899978, 0.000000, 0.000000);
SetDynamicObjectMaterial(TempObject, 0, 1676, "wshxrefpump", "black64", 0);

CreateDynamicObject(2229, 2318.873291, 1284.698364, 3008.114257, 0.000000, 0.000000, 91.799964);
CreateDynamicObject(2229, 2318.880859, 1288.552978, 3008.094238, 0.000000, 0.000000, 91.799964);
CreateDynamicObject(1502, 2322.955322, 1290.909545, 3006.973632, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(1502, 2328.616699, 1286.728637, 3006.984863, 0.000000, 0.000000, -89.300041);
CreateDynamicObject(1569, 2323.104003, 1281.351684, 3006.992675, 0.000000, 0.000000, 0.000000);
TempObject = CreateDynamicObject(2370, 2320.043945, 1285.968505, 3006.860595, 0.000000, 0.000000, 0.000000);
SetDynamicObjectMaterial(TempObject, 0, 16150, "ufo_bar", "sa_wood07_128", 0);
CreateDynamicObject(1723, 2322.470947, 1287.194946, 3007.029541, 0.000000, 0.000000, -90.199958);
TempObject = CreateDynamicObject(11683, 2320.562988, 1283.805541, 3007.001708, 0.000000, 0.000000, 179.299835);
SetDynamicObjectMaterial(TempObject, 0, 8839, "vgsecarshow", "lightred2_32", 0);
TempObject = CreateDynamicObject(11683, 2320.626220, 1288.456298, 3007.001708, 0.000000, 0.000000, 1.099817);
SetDynamicObjectMaterial(TempObject, 0, 8839, "vgsecarshow", "lightred2_32", 0);
CreateDynamicObject(1893, 2321.362304, 1285.691650, 3010.450927, 0.000000, 0.000000, 90.899948);
CreateDynamicObject(16779, 2331.111083, 1285.942382, 3010.256103, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(1893, 2323.715332, 1293.490600, 3010.450927, 0.000000, 0.000000, 90.899948);
CreateDynamicObject(2072, 2319.924804, 1282.243408, 3008.480957, 0.000000, 0.000000, -47.099960);
CreateDynamicObject(2072, 2319.638671, 1290.515014, 3008.480957, 0.000000, 0.000000, -134.299987);
CreateDynamicObject(19175, 2321.146484, 1290.857543, 3008.680664, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(19174, 2328.550048, 1288.883544, 3008.586914, 0.000000, 0.000000, -88.599990);
CreateDynamicObject(19173, 2321.393554, 1281.436523, 3008.822265, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(19172, 2330.394042, 1288.938232, 3008.712158, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(2141, 2329.550537, 1283.783447, 3006.963867, 0.000000, 0.000000, 179.300079);
CreateDynamicObject(2008, 2326.239746, 1290.337402, 3006.955078, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(1715, 2327.129638, 1288.947631, 3006.983886, 0.000000, 0.000000, 179.000122);
CreateDynamicObject(19893, 2326.354248, 1290.262329, 3007.764404, 0.000000, 0.000000, 26.099996);
CreateDynamicObject(2028, 2320.815673, 1285.895996, 3007.205078, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(1787, 2320.874267, 1286.582031, 3007.215332, 0.000000, 0.000000, 89.499954);
CreateDynamicObject(2828, 2320.353027, 1286.376586, 3007.715820, 0.000000, 0.000000, -111.500015);
CreateDynamicObject(2855, 2319.818359, 1286.258300, 3007.715820, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(11737, 2323.837890, 1281.840698, 3007.022460, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(1828, 2320.583496, 1286.220947, 3007.058837, 0.000000, 0.000000, 90.200012);
CreateDynamicObject(14651, 2327.177001, 1283.250244, 3009.200195, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(2517, 2321.629638, 1295.159423, 3007.009033, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(2528, 2322.009033, 1291.451904, 3007.029541, 0.000000, 0.000000, -179.000183);
CreateDynamicObject(19873, 2321.847167, 1291.103881, 3008.020996, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(2522, 2325.533447, 1294.793579, 3006.972167, 0.000000, 0.000000, 89.199996);
CreateDynamicObject(11707, 2324.226806, 1296.555541, 3008.019531, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(1643, 2324.435302, 1295.466796, 3007.025634, 0.000000, 0.000000, 0.000000);
CreateDynamicObject(11706, 2332.862792, 1288.515625, 3007.000488, 0.000000, 0.000000, -88.899978);
TempObject = CreateDynamicObject(19455, 2324.080810, 1281.316040, 3008.605957, 0.000000, 0.000000, 90.000015);
SetDynamicObjectMaterial(TempObject, 0, 8503, "vgs_shop5", "vegaspawnwall_128", 0);
TempObject = CreateDynamicObject(19398, 2323.741455, 1290.948974, 3008.748779, 0.000000, 0.000000, 89.899932);
SetDynamicObjectMaterial(TempObject, 0, 8401, "vgshpground", "vegaspawnwall_128", 0);


 // End of Jes Erosel custom map

return 1;
}


