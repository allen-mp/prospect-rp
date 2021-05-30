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
	LoadWaffenSS();
	return 1;
}

public OnFilterScriptExit() {
	return 1;
}
LoadWaffenSS()
{
  // Jessica Erosel Custom Map

//Objects////////////////////////////////////////////////////////////////////////////////////////////////////////
new tmpobjid;
tmpobjid = CreateDynamicObject(987, -1250.386352, 2283.072021, 130.878860, 0.000000, 0.000000, 130.599884, -1, -1, -1, 300.00, 300.00);
tmpobjid = CreateDynamicObject(987, -1258.130493, 2292.106201, 130.878860, 0.000000, 0.000000, 130.599884, -1, -1, -1, 300.00, 300.00);
tmpobjid = CreateDynamicObject(3279, -1263.925537, 2286.872802, 130.786117, 0.000000, 0.000000, -144.700012, -1, -1, -1, 300.00, 300.00);
tmpobjid = CreateDynamicObject(987, -1273.652465, 2310.210205, 130.878860, 0.000000, 0.000000, -139.400207, -1, -1, -1, 300.00, 300.00);
tmpobjid = CreateDynamicObject(987, -1260.119018, 2275.947509, 130.878860, 0.000000, 0.000000, 36.899822, -1, -1, -1, 300.00, 300.00);
tmpobjid = CreateDynamicObject(987, -1269.634033, 2268.807373, 131.448959, 0.000000, 0.000000, 36.899822, -1, -1, -1, 300.00, 300.00);
tmpobjid = CreateDynamicObject(987, -1277.737915, 2262.723632, 131.448959, 0.000000, 0.000000, 36.899822, -1, -1, -1, 300.00, 300.00);
tmpobjid = CreateDynamicObject(987, -1284.991699, 2272.414306, 131.399398, 0.000000, 0.000000, -52.800159, -1, -1, -1, 300.00, 300.00);
tmpobjid = CreateDynamicObject(987, -1292.149536, 2281.845947, 130.399490, 0.000000, 0.000000, -52.800159, -1, -1, -1, 300.00, 300.00);
tmpobjid = CreateDynamicObject(987, -1297.675292, 2289.124755, 129.399581, 0.000000, 0.000000, -52.800159, -1, -1, -1, 300.00, 300.00);
tmpobjid = CreateDynamicObject(987, -1282.468750, 2302.651855, 130.658920, 0.000000, 0.000000, -139.400207, -1, -1, -1, 300.00, 300.00);
tmpobjid = CreateDynamicObject(987, -1288.809082, 2297.218261, 129.498992, 0.000000, 0.000000, -137.300109, -1, -1, -1, 300.00, 300.00);
tmpobjid = CreateDynamicObject(3279, -1290.022094, 2288.742919, 130.136230, 0.000000, 0.000000, 40.700019, -1, -1, -1, 300.00, 300.00);
tmpobjid = CreateDynamicObject(3362, -1278.675903, 2270.402832, 131.688369, 0.000000, 0.000000, 38.699993, -1, -1, -1, 300.00, 300.00);
tmpobjid = CreateDynamicObject(1684, -1267.762695, 2275.379882, 133.423202, 0.000000, 0.000000, -144.699935, -1, -1, -1, 300.00, 300.00);
tmpobjid = CreateDynamicObject(16095, -1283.310791, 2281.740478, 131.734130, 0.000000, 0.000000, -142.499923, -1, -1, -1, 300.00, 300.00);


 // End of Jes Erosel custom map

return 1;
}



