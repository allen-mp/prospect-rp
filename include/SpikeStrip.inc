/*
 * SA-MP Spike Strips
 *
 * Copyright (c) 2013, Stylock
 * This file is provided as is with no warranties of any kind.
 */

/*
 * Changelog
 *
 *		13/8/2013
 *			~ shortened the prefix from "SpikeStrip" to "Stinger"
 *			~ added a config option to enable/disable the OnSpikeStripPopTire callback
 *			~ added a config option to enable/disable the Streamer plugin support
 * 
 *		5/7/2013:
 *			~ changed parameter order in OnSpikeStripPopTire callback
 *			~ fixed a bug where OnSpikeStripPopTire callback wasn't getting called
 *			~ fixed incorrect arguments being passed to SetTimer function
 *			~ added a function named SpikeStrip_DeleteAll that deletes all spike strips
 *
 * 		4/7/2013:
 *			~ removed the auto-adjustment of the Z coordinate when creating a spike strip
 *			~ added a callback named OnSpikeStripPopTire that is called when a tire is popped
 *			~ added a function named SpikeStrip_IsValid that checks the validity of a spike strip
 *			~ added a function named SpikeStrip_SetGhost that toggles a player's ghost mode
 *			~ added a function named SpikeStrip_IsGhost that checks a player's ghost mode
 *
 *		3/7/2013:
 *			~ initial release
 */

#include <a_samp>
// Alternatively use foreach
#include <YSI\y_iterate>

#define MAX_SPIKE_STRIPS	(100)
#define SS_TIME_INTERVAL	(250)
#define SS_LINE_SEGMENTS	(2) // 1, 2 or 4

#define SS_USE_CALLBACK		(true)	// OnSpikeStripPopTire
#define SS_USE_STREAMER		(false)	// Streamer plugin support

// Spike strip models
#define SPIKE_STRIP_LONG	(2892)
#define SPIKE_STRIP_SHORT	(2899)

static stock
	g_SpikeStrip[MAX_SPIKE_STRIPS],
	Iterator:g_Spike<MAX_SPIKE_STRIPS>,
	Float:g_Spike_x1[MAX_SPIKE_STRIPS],
	Float:g_Spike_y1[MAX_SPIKE_STRIPS],
	Float:g_Spike_x2[MAX_SPIKE_STRIPS],
	Float:g_Spike_y2[MAX_SPIKE_STRIPS],
	Float:g_Spike_x3[MAX_SPIKE_STRIPS],
	Float:g_Spike_y3[MAX_SPIKE_STRIPS],
	Float:g_Spike_x4[MAX_SPIKE_STRIPS],
	Float:g_Spike_y4[MAX_SPIKE_STRIPS],
	Float:g_Spike_zA[MAX_SPIKE_STRIPS],
	Float:g_Tire1_x1[MAX_PLAYERS],
	Float:g_Tire1_y1[MAX_PLAYERS],
	Float:g_Tire1_z1[MAX_PLAYERS],
	Float:g_Tire1_x2[MAX_PLAYERS],
	Float:g_Tire1_y2[MAX_PLAYERS],
	Float:g_Tire1_z2[MAX_PLAYERS],
	Float:g_Tire1_x3[MAX_PLAYERS],
	Float:g_Tire1_y3[MAX_PLAYERS],
	Float:g_Tire1_z3[MAX_PLAYERS],
	Float:g_Tire1_x4[MAX_PLAYERS],
	Float:g_Tire1_y4[MAX_PLAYERS],
	Float:g_Tire1_z4[MAX_PLAYERS],
	Float:g_Tire2_x1[MAX_PLAYERS],
	Float:g_Tire2_y1[MAX_PLAYERS],
	Float:g_Tire2_x2[MAX_PLAYERS],
	Float:g_Tire2_z1[MAX_PLAYERS],
	Float:g_Tire2_y2[MAX_PLAYERS],
	Float:g_Tire2_z2[MAX_PLAYERS],
	g_GetGhostMode[MAX_PLAYERS char],
	g_GetTireState[MAX_PLAYERS char],
	g_GetTireCount[MAX_PLAYERS char],
	g_GetVehicleID[MAX_PLAYERS],
	g_GetTickCount[MAX_PLAYERS],
	g_GetServerTC;

enum (<<= 1)
{
	e_TIRE_RR = 0b0001,
	e_TIRE_FR,
	e_TIRE_RL,
	e_TIRE_FL,
}

// Forwards
forward SS_GetTickCount();
forward OnSpikeStripPopTire(spikeid, vehicleid, playerid, tire);

public SS_GetTickCount()
{
	// Store the tick count for fast access. Better than
	// calling the function 20 times per second per player.
 	g_GetServerTC = GetTickCount();
 	return 1;
}

public OnGameModeInit()
{
	SetTimer("SS_GetTickCount", 50, 1);
	#if defined SS_OnGameModeInit 
        return SS_OnGameModeInit(); 
    #else 
        return 1; 
    #endif 
}

#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit SS_OnGameModeInit
#if defined SS_OnGameModeInit 
	forward SS_OnGameModeInit();
#endif 

public OnPlayerUpdate(playerid)
{
	static
		Float:matrix3x3[9],
		Float:x1, Float:y1, Float:z1,
		Float:x2, Float:y2, Float:z2,
		Float:x3, Float:y3, Float:z3,
		Float:x4, Float:y4, Float:z4;
	if (g_GetTickCount[playerid] < g_GetServerTC)
	{
		if (GetPlayerState(playerid) == 2) // Driver
		{
			if (IsPlayerNPC(playerid))
			{
				return 1;
			}
			if (!g_GetGhostMode{playerid})
			{
				new
					vid = GetPlayerVehicleID(playerid);
				if (vid != g_GetVehicleID[playerid])
				{
					new
						mid = GetVehicleModel(vid);
					g_GetTireCount{playerid} = SS_GetTireCount(mid);
					g_GetTireState{playerid} = SS_GetTireState(vid);
					GetVehicleModelInfo(mid, 6, x1, y1, z1);
					SS_GetTireSize(mid, e_TIRE_RR, z2);
					// Save rear wheel offsets
					g_Tire2_x1[playerid] = x1,
					g_Tire2_y1[playerid] = y1,
					g_Tire2_z1[playerid] = z1 - z2 / 2;
					GetVehicleModelInfo(mid, 5, x1, y1, z1);
					SS_GetTireSize(mid, e_TIRE_FR, z2);
					// Save front wheel offsets
					g_Tire2_x2[playerid] = x1,
					g_Tire2_y2[playerid] = y1,
					g_Tire2_z2[playerid] = z1 - z2 / 2;
				}
				if (g_GetTireCount{playerid}) // Check if the player's vehicle has any tires
				{
					if (g_GetTireState{playerid} < 0b1111) // Check if at least one tire is not flat (0b1111 = all tires are flat)
					{
						SS_GetVehicleRotationMatrix(vid, matrix3x3);
						// If the tire is not flat, then get its position
						!(g_GetTireState{playerid} & e_TIRE_RR) && SS_GetTirePos(vid, x1, y1, z1, g_Tire2_x1[playerid], g_Tire2_y1[playerid], g_Tire2_z1[playerid], matrix3x3), // RR
						!(g_GetTireState{playerid} & e_TIRE_FR) && SS_GetTirePos(vid, x2, y2, z2, g_Tire2_x2[playerid], g_Tire2_y2[playerid], g_Tire2_z2[playerid], matrix3x3); // FR
						if (g_GetTireCount{playerid} > 2)
						{
							!(g_GetTireState{playerid} & e_TIRE_RL) && SS_GetTirePos(vid, x3, y3, z3, -g_Tire2_x1[playerid], g_Tire2_y1[playerid], g_Tire2_z1[playerid], matrix3x3), // RL
							!(g_GetTireState{playerid} & e_TIRE_FL) && SS_GetTirePos(vid, x4, y4, z4, -g_Tire2_x2[playerid], g_Tire2_y2[playerid], g_Tire2_z2[playerid], matrix3x3); // FL
						}
						foreach (new i : g_Spike)
						{
							if (IsPlayerInRangeOfPoint(playerid, 60.0, g_Spike_x1[i], g_Spike_y1[i], g_Spike_zA[i]))
							{
								// If the tire is not flat, then monitor it
								!(g_GetTireState{playerid} & e_TIRE_RR) && SS_MonitorTire(vid, e_TIRE_RR, i, playerid, x1, y1, z1, g_Tire1_x1[playerid], g_Tire1_y1[playerid], g_Tire1_z1[playerid]), // RR
								!(g_GetTireState{playerid} & e_TIRE_FR) && SS_MonitorTire(vid, e_TIRE_FR, i, playerid, x2, y2, z2, g_Tire1_x2[playerid], g_Tire1_y2[playerid], g_Tire1_z2[playerid]); // FR
								if (g_GetTireCount{playerid} > 2)
								{
									!(g_GetTireState{playerid} & e_TIRE_RL) && SS_MonitorTire(vid, e_TIRE_RL, i, playerid, x3, y3, z3, g_Tire1_x3[playerid], g_Tire1_y3[playerid], g_Tire1_z3[playerid]), // RL
									!(g_GetTireState{playerid} & e_TIRE_FL) && SS_MonitorTire(vid, e_TIRE_FL, i, playerid, x4, y4, z4, g_Tire1_x4[playerid], g_Tire1_y4[playerid], g_Tire1_z4[playerid]); // FL
								}
							}
						}
						// If the tire is not flat, then save its position
						!(g_GetTireState{playerid} & e_TIRE_RR) && (g_Tire1_x1[playerid] = x1, g_Tire1_y1[playerid] = y1, g_Tire1_z1[playerid] = z1),
						!(g_GetTireState{playerid} & e_TIRE_FR) && (g_Tire1_x2[playerid] = x2, g_Tire1_y2[playerid] = y2, g_Tire1_z2[playerid] = z2);
						if (g_GetTireCount{playerid} > 2)
						{
							!(g_GetTireState{playerid} & e_TIRE_RL) && (g_Tire1_x3[playerid] = x3, g_Tire1_y3[playerid] = y3, g_Tire1_z3[playerid] = z3),
							!(g_GetTireState{playerid} & e_TIRE_FL) && (g_Tire1_x4[playerid] = x4, g_Tire1_y4[playerid] = y4, g_Tire1_z4[playerid] = z4);
						}
					}
				}
				g_GetVehicleID[playerid] = vid;
			}
		}
	    g_GetTickCount[playerid] = g_GetServerTC + SS_TIME_INTERVAL;
	}
	#if defined SS_OnPlayerUpdate 
        return SS_OnPlayerUpdate(playerid); 
    #else 
        return 1; 
    #endif 	
}

#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate SS_OnPlayerUpdate
#if defined SS_OnPlayerUpdate 
	forward SS_OnPlayerUpdate(playerid);
#endif

public OnVehicleDamageStatusUpdate(vehicleid, playerid)
{
	new
		tire_state = SS_GetTireState(vehicleid);
	if (tire_state != g_GetTireState{playerid})
	{
		if (g_GetTireState{playerid} > tire_state) // Check if at least one tire is fixed
		{
			new
				Float:matrix3x3[9];
			SS_GetVehicleRotationMatrix(vehicleid, matrix3x3);
			if (e_TIRE_RR & (g_GetTireState{playerid} & ~tire_state)) // If a flat tire is fixed
			{
				SS_GetTirePos(vehicleid, g_Tire1_x1[playerid], g_Tire1_y1[playerid], g_Tire1_z1[playerid], g_Tire2_x1[playerid], g_Tire2_y1[playerid], g_Tire2_z1[playerid], matrix3x3);
			}
			if (e_TIRE_FR & (g_GetTireState{playerid} & ~tire_state)) // If a flat tire is fixed
			{
				SS_GetTirePos(vehicleid, g_Tire1_x2[playerid], g_Tire1_y2[playerid], g_Tire1_z2[playerid], g_Tire2_x2[playerid], g_Tire2_y2[playerid], g_Tire2_z2[playerid], matrix3x3);
			}
			if (e_TIRE_RL & (g_GetTireState{playerid} & ~tire_state)) // If a flat tire is fixed
			{
				SS_GetTirePos(vehicleid, g_Tire1_x3[playerid], g_Tire1_y3[playerid], g_Tire1_z3[playerid], -g_Tire2_x1[playerid], g_Tire2_y1[playerid], g_Tire2_z1[playerid], matrix3x3);
			}
			if (e_TIRE_FL & (g_GetTireState{playerid} & ~tire_state)) // If a flat tire is fixed
			{
				SS_GetTirePos(vehicleid, g_Tire1_x4[playerid], g_Tire1_y4[playerid], g_Tire1_z4[playerid], -g_Tire2_x2[playerid], g_Tire2_y2[playerid], g_Tire2_z2[playerid], matrix3x3);
			}
		}
		g_GetTireState{playerid} = tire_state;
	}
	#if defined SS_OnVehicleDamageStatusUpdate
        return SS_OnVehicleDamageStatusUpdate(vehicleid, playerid); 
    #else 
        return 1; 
    #endif 
}

#if defined _ALS_OnVehicleDamageStatusUpd
	#undef OnVehicleDamageStatusUpdate
#else
	#define _ALS_OnVehicleDamageStatusUpd
#endif
#define OnVehicleDamageStatusUpdate SS_OnVehicleDamageStatusUpdate
#if defined SS_OnVehicleDamageStatusUpdate
	forward SS_OnVehicleDamageStatusUpdate(vehicleid, playerid);
#endif
	
stock SS_MonitorTire(vid, tire, sid, pid, Float:x1, Float:y1, Float:z1, Float:x2, Float:y2, Float:z2)
{
	new
		Float:distance = 1.0;
	if (SS_SegmentsIntersect_2D(g_Spike_x1[sid], g_Spike_y1[sid], g_Spike_x2[sid], g_Spike_y2[sid], x1, y1, x2, y2))
	{
		SS_DistanceLineToLine_3D(g_Spike_x1[sid], g_Spike_y1[sid], g_Spike_zA[sid], g_Spike_x2[sid], g_Spike_y2[sid], g_Spike_zA[sid], x1, y1, z1, x2, y2, z2, distance);
	}

	#if SS_LINE_SEGMENTS >= 2

	else if (SS_SegmentsIntersect_2D(g_Spike_x3[sid], g_Spike_y3[sid], g_Spike_x4[sid], g_Spike_y4[sid], x1, y1, x2, y2))
	{
		SS_DistanceLineToLine_3D(g_Spike_x3[sid], g_Spike_y3[sid], g_Spike_zA[sid], g_Spike_x4[sid], g_Spike_y4[sid], g_Spike_zA[sid], x1, y1, z1, x2, y2, z2, distance);
	}

	#endif

	#if SS_LINE_SEGMENTS == 4

	else if (SS_SegmentsIntersect_2D(g_Spike_x1[sid], g_Spike_y1[sid], g_Spike_x3[sid], g_Spike_y3[sid], x1, y1, x2, y2))
	{
		SS_DistanceLineToLine_3D(g_Spike_x1[sid], g_Spike_y1[sid], g_Spike_zA[sid], g_Spike_x3[sid], g_Spike_y3[sid], g_Spike_zA[sid], x1, y1, z1, x2, y2, z2, distance);
	}
	else if (SS_SegmentsIntersect_2D(g_Spike_x2[sid], g_Spike_y2[sid], g_Spike_x4[sid], g_Spike_y4[sid], x1, y1, x2, y2))
	{
		SS_DistanceLineToLine_3D(g_Spike_x2[sid], g_Spike_y2[sid], g_Spike_zA[sid], g_Spike_x4[sid], g_Spike_y4[sid], g_Spike_zA[sid], x1, y1, z1, x2, y2, z2, distance);
	}

	#endif

	if (distance < 0.24)
	{
		if (vid == g_GetVehicleID[pid]) // Make sure surprise tire pops don't occur
		{
			new
				data1,
				data2,
				data3,
				data4;
			GetVehicleDamageStatus(vid, data1, data2, data3, data4);
			UpdateVehicleDamageStatus(vid, data1, data2, data3, (data4 | tire)); // Pop
			#if SS_USE_CALLBACK
			
			CallRemoteFunction("OnSpikeStripPopTire", "iiii", sid, vid, pid, tire);
			
			#endif
		}
	}
	return 1;
}

stock SS_GetTireState(vid)
{
	new
		data;
	GetVehicleDamageStatus(vid, data, data, data, data);
	return data;
}

stock SS_GetTireCount(mid)
{
	static const
		tire_data[] =
	{
		4, 4, 4, 6, 4, 4, 4, 4, 6, 4,
		4, 4, 4, 4, 4, 4, 4, 0, 4, 4,
		4, 4, 4, 4, 4, 0, 4, 4, 4, 4,
		0, 6, 4, 6, 4, 4, 4, 6, 4, 4,
		4, 0, 4, 6, 4, 4, 0, 0, 2, 0,
		4, 4, 0, 0, 0, 6, 4, 4, 4, 4,
		0, 2, 2, 2, 0, 0, 4, 4, 2, 0,
		4, 4, 0, 0, 4, 4, 0, 4, 4, 4,
		4, 0, 4, 4, 0, 4, 4, 0, 0, 4,
		4, 4, 4, 0, 4, 4, 4, 0, 4, 4,
		4, 0, 4, 4, 4, 4, 4, 4, 4, 0,
		0, 0, 0, 0, 6, 6, 4, 4, 4, 0,
		0, 2, 2, 2, 6, 4, 4, 4, 4, 4,
		4, 4, 6, 4, 4, 4, 4, 0, 0, 0,
		4, 4, 4, 4, 4, 4, 4, 4, 0, 4,
		4, 4, 4, 0, 4, 4, 4, 4, 4, 4,
		4, 4, 4, 0, 0, 4, 4, 4, 4, 0,
		0, 4, 4, 4, 4, 4, 4, 0, 6, 4,
		4, 2, 4, 4, 4, 4, 2, 4, 4, 4,
		0, 4, 0, 0, 0, 0, 4, 4, 4, 4,
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
		4, 4
	};
	// Aircraft, boats, bicycles, trains
	// and RC vehicles return zero tires
	return tire_data[mid - 400];
}

stock SS_GetTireSize(mid, tire, &Float:size)
{
	static const
		Float:tire_size_R[] =
	{
		// Rear wheel size (from vehicles.ide)
		0.7680, 0.7749, 0.6999, 1.1000, 0.6600, 0.6999, 2.2799, 1.0000, 1.0599, 0.7500,
		0.8000, 0.6999, 0.6999, 0.7200, 0.7599, 0.6800, 0.8640, 0.0000, 0.6999, 0.6399,
		0.7879, 0.6499, 0.6999, 0.6999, 0.9200, 0.0000, 0.6999, 0.9359, 0.9139, 0.6999,
		0.0000, 1.0000, 1.2999, 1.2000, 0.8000, 1.1000, 0.6999, 1.0000, 0.6999, 0.6999,
		0.6999, 0.0000, 0.6800, 1.0820, 1.5000, 0.6800, 0.0000, 0.0000, 0.4639, 0.0000,
		1.1000, 0.7500, 0.0000, 0.0000, 0.0000, 1.2000, 0.8399, 0.5000, 0.7200, 0.6999,
		0.0000, 0.6700, 0.4639, 0.5600, 0.0000, 0.0000, 0.6999, 0.6999, 0.6200, 0.0000,
		0.8939, 0.6000, 0.0000, 0.0000, 0.6999, 0.6999, 0.0000, 0.7599, 0.6999, 0.6999,
		0.6999, 0.0000, 0.6999, 0.6600, 0.0000, 0.6000, 1.5000, 0.0000, 0.0000, 0.8999,
		0.9200, 0.6499, 0.6999, 0.0000, 0.8199, 0.9720, 0.6999, 0.0000, 0.7599, 0.8000,
		0.8000, 0.0000, 0.8199, 0.8199, 0.6999, 0.8999, 0.7730, 0.6999, 0.8000, 0.0000,
		0.0000, 0.0000, 0.0000, 0.0000, 1.1059, 1.1799, 0.7500, 0.7500, 0.6600, 0.0000,
		0.0000, 0.6800, 0.6800, 0.6700, 1.0000, 0.9200, 0.6999, 0.6999, 0.8500, 0.6999,
		0.4499, 1.2999, 1.0000, 0.6999, 0.6999, 0.7400, 0.6999, 0.0000, 0.0000, 0.0000,
		0.6999, 1.0000, 0.7400, 0.6999, 1.0000, 0.6999, 0.6999, 0.6999, 0.0000, 0.6840,
		0.7599, 0.7500, 0.8399, 0.0000, 0.8399, 0.6999, 1.5000, 1.5000, 0.7879, 0.6999,
		0.8700, 0.6999, 0.6800, 0.0000, 0.0000, 0.6399, 0.6999, 0.6999, 0.6999, 0.0000,
		0.0000, 0.2599, 0.5600, 1.1399, 0.5000, 0.6999, 0.6999, 0.0000, 1.0000, 0.8999,
		0.7799, 0.6800, 0.7699, 0.7500, 1.1200, 0.7400, 0.6539, 0.6999, 0.8600, 0.7400,
		0.0000, 1.1000, 0.0000, 0.0000, 0.0000, 0.0000, 0.6999, 0.6999, 0.6999, 0.9499,
		0.6999, 1.3660, 0.6999, 0.6999, 0.6999, 0.6999, 0.6000, 0.6000, 0.6000, 0.7599,
		0.3199, 0.6800
	};
	if (tire & (e_TIRE_RR | e_TIRE_RL))
	{
		size = tire_size_R[mid - 400];
		return 1;
	}
	static const
		Float:tire_size_F[] =
	{
		// Front wheel size (from vehicles.ide)
		0.7680, 0.7749, 0.6999, 1.1000, 0.6600, 0.6999, 2.2799, 1.0000, 1.0599, 0.7500,
		0.8000, 0.6999, 0.6999, 0.7200, 0.7599, 0.6800, 0.8640, 0.0000, 0.6999, 0.6399,
		0.7879, 0.6499, 0.6999, 0.6999, 0.8399, 0.0000, 0.6999, 0.9359, 0.9139, 0.6999,
		0.0000, 1.0000, 1.2999, 1.2000, 0.7200, 1.1000, 0.6999, 1.0000, 0.6999, 0.6999,
		0.6999, 0.0000, 0.6800, 1.0820, 1.5000, 0.6800, 0.0000, 0.0000, 0.4639, 0.0000,
		1.1000, 0.6999, 0.0000, 0.0000, 0.0000, 1.2000, 0.8399, 0.5000, 0.7200, 0.6999,
		0.0000, 0.6700, 0.4639, 0.7799, 0.0000, 0.0000, 0.6999, 0.6999, 0.6800, 0.0000,
		0.8939, 0.6000, 0.0000, 0.0000, 0.6999, 0.6999, 0.0000, 0.7599, 0.6999, 0.6999,
		0.6999, 0.0000, 0.6999, 0.6600, 0.0000, 0.6000, 1.5000, 0.0000, 0.0000, 0.8999,
		0.9200, 0.6499, 0.6999, 0.0000, 0.8199, 0.9720, 0.6999, 0.0000, 0.7599, 0.8000,
		0.8000, 0.0000, 0.8199, 0.8199, 0.6999, 0.8999, 0.7730, 0.6999, 0.8000, 0.0000,
		0.0000, 0.0000, 0.0000, 0.0000, 1.1059, 1.1799, 0.7500, 0.7500, 0.6600, 0.0000,
		0.0000, 0.6800, 0.6800, 0.6700, 1.1200, 0.9200, 0.6999, 0.6999, 0.8500, 0.6999,
		0.4499, 0.6800, 0.5879, 0.6999, 0.6999, 0.7400, 0.6999, 0.0000, 0.0000, 0.0000,
		0.6999, 1.0000, 0.7400, 0.6999, 1.0000, 0.6999, 0.6999, 0.6999, 0.0000, 0.6840,
		0.7599, 0.7500, 0.8399, 0.0000, 0.8399, 0.6999, 1.5000, 1.5000, 0.7879, 0.6999,
		0.8700, 0.6999, 0.6800, 0.0000, 0.0000, 0.6399, 0.6999, 0.6999, 0.5500, 0.0000,
		0.0000, 0.2599, 0.4799, 1.1399, 0.5000, 0.6999, 0.6999, 0.0000, 1.0000, 0.8999,
		0.7799, 0.6800, 0.7699, 0.6600, 1.1200, 0.7400, 0.6539, 0.6999, 0.8600, 0.7400,
		0.0000, 1.1000, 0.0000, 0.0000, 0.0000, 0.0000, 0.6999, 0.6999, 0.6999, 0.9499,
		0.6999, 1.3660, 0.6999, 0.6999, 0.6999, 0.6999, 0.6000, 0.6000, 0.6000, 0.7599,
		0.3199, 0.6800
	};
	if (tire & (e_TIRE_FR | e_TIRE_FL))
	{
		size = tire_size_F[mid - 400];
		return 1;
	}
	return 0;
}

stock SS_GetTirePos(vid, &Float:px, &Float:py, &Float:pz, Float:ox, Float:oy, Float:oz, Float:matrix3x3[])
{
	GetVehiclePos(vid, px, py, pz);
	px = px + ox * (1 - 2 * (matrix3x3[4] + matrix3x3[7])) + oy * (2 * (matrix3x3[1] + matrix3x3[8])) + oz * (2 * (matrix3x3[2] - matrix3x3[6])),
	py = py + ox * (2 * (matrix3x3[1] - matrix3x3[8])) + oy * (1 - 2 * (matrix3x3[0] + matrix3x3[7])) + oz * (2 * (matrix3x3[5] + matrix3x3[3])),
	pz = pz + ox * (2 * (matrix3x3[2] + matrix3x3[6])) + oy * (2 * (matrix3x3[5] - matrix3x3[3])) + oz * (1 - 2 * (matrix3x3[0] + matrix3x3[4]));
	return 1;
}

stock SS_GetVehicleRotationMatrix(vid, Float:matrix3x3[])
{
	new
		Float:qw,
		Float:qx,
		Float:qy,
		Float:qz;
	GetVehicleRotationQuat(vid, qw, qx, qy, qz);
	matrix3x3[0] = qx * qx, matrix3x3[1] = qx * qy, matrix3x3[2] = qx * qz,
	matrix3x3[3] = qx * qw, matrix3x3[4] = qy * qy, matrix3x3[5] = qy * qz,
	matrix3x3[6] = qy * qw, matrix3x3[7] = qz * qz, matrix3x3[8] = qz * qw;
	return 1;
}

/*
This function checks if two line segments intersect (2D space)
*/
stock SS_SegmentsIntersect_2D(Float:x1, Float:y1, Float:x2, Float:y2, Float:x3, Float:y3, Float:x4, Float:y4)
{
	new
		Float:xA = x2 - x1,
		Float:yA = y2 - y1,
		Float:xB = x4 - x3,
		Float:yB = y4 - y3,
		Float:d  = xA * yB - yA * xB;
	if (!d)
	{
		// Lines are parallel, or one or
		// both segments are zero-length
		return 0;
	}
	new
		Float:xC = x3 - x1,
		Float:yC = y3 - y1,
		Float:pA = (xC * yB - yC * xB) / d,
		Float:pB = (xC * yA - yC * xA) / d;
	if (pA < 0 || pA > 1 || pB < 0 || pB > 1)
	{
		return 0;
	}
	// Compute the intersection point
	// xi = x1 + pA * xA
	// yi = y1 + pA * yA
	return 1;
}

/*
This function computes the shortest distance between two lines (3D space)
*/
stock SS_DistanceLineToLine_3D(Float:x1, Float:y1, Float:z1, Float:x2, Float:y2, Float:z2, Float:x3, Float:y3, Float:z3, Float:x4, Float:y4, Float:z4, &Float:distance)
{
	new
		Float:ux = x2 - x1,
		Float:uy = y2 - y1,
		Float:uz = z2 - z1,
		Float:vx = x4 - x3,
		Float:vy = y4 - y3,
		Float:vz = z4 - z3,
		Float:wx = x1 - x3,
		Float:wy = y1 - y3,
		Float:wz = z1 - z3,
		Float:uu = ux * ux + uy * uy + uz * uz,
		Float:uv = ux * vx + uy * vy + uz * vz,
		Float:uw = ux * wx + uy * wy + uz * wz,
		Float:vv = vx * vx + vy * vy + vz * vz,
		Float:vw = vx * wx + vy * wy + vz * wz,
		Float:d  = uu * vv - uv * uv,
		Float:pA = (uv * vw - vv * uw) / d,
		Float:pB = (uu * vw - uv * uw) / d,
		// The difference of the two closest points
		Float:dx = wx + pA * ux - pB * vx,
		Float:dy = wy + pA * uy - pB * vy,
		Float:dz = wz + pA * uz - pB * vz;
	distance = floatsqroot(dx * dx + dy * dy + dz * dz);
	return 1;
}

#if !SS_USE_STREAMER
	#define SS_CreateObject(%0) \
		CreateObject(%0, 200.0)
	#define SS_DestroyObject \
		DestroyObject
#else
	#include <streamer>
	#define SS_CreateObject \
		CreateDynamicObject
	#define SS_DestroyObject \
		DestroyDynamicObject
#endif

/*
native Stinger_Create(mid, Float:x, Float:y, Float:z, Float:a);
*/

stock Stinger_Create(mid, Float:x, Float:y, Float:z, Float:a)
{
	new
		idx = Iter_Free(g_Spike);
	if (idx != -1)
	{
		new
			Float:adjust,
			Float:length;
		switch (mid)
		{
			case 2892:
			{
				length = 5.0;
			}
			case 2899:
			{
				length = 2.5;
				adjust = 0.118;
			}
			default:
			{
				return INVALID_OBJECT_ID;
			}
		}
		new
			id = SS_CreateObject(mid, x, y, z + adjust, 0.0, 0.0, a - 90.0);
		if (id != INVALID_OBJECT_ID)
		{
			a = -a;
			Iter_Add(g_Spike, idx);
			g_SpikeStrip[idx] = id;

			#if SS_LINE_SEGMENTS == 1

			g_Spike_x1[idx] = x + length * floatsin(a - 90.0, degrees);
			g_Spike_y1[idx] = y + length * floatcos(a - 90.0, degrees);
			g_Spike_x2[idx] = x - length * floatsin(a - 90.0, degrees);
			g_Spike_y2[idx] = y - length * floatcos(a - 90.0, degrees);

			#endif

			#if SS_LINE_SEGMENTS >= 2

			new
				value;
			mid != 2892 && (value = 10) || (value = 5);
			g_Spike_x1[idx] = x + length * floatsin(a - (90.0 + value), degrees);
			g_Spike_y1[idx] = y + length * floatcos(a - (90.0 + value), degrees);
			g_Spike_x3[idx] = x + length * floatsin(a - (90.0 - value), degrees);
			g_Spike_y3[idx] = y + length * floatcos(a - (90.0 - value), degrees);

			#if SS_LINE_SEGMENTS == 2

			g_Spike_x2[idx] = x - length * floatsin(a - (90.0 + value), degrees);
			g_Spike_y2[idx] = y - length * floatcos(a - (90.0 + value), degrees);
			g_Spike_x4[idx] = x - length * floatsin(a - (90.0 - value), degrees);
			g_Spike_y4[idx] = y - length * floatcos(a - (90.0 - value), degrees);

			#endif

			#if SS_LINE_SEGMENTS == 4

			g_Spike_x2[idx] = x - length * floatsin(a - (90.0 - value), degrees);
			g_Spike_y2[idx] = y - length * floatcos(a - (90.0 - value), degrees);
			g_Spike_x4[idx] = x - length * floatsin(a - (90.0 + value), degrees);
			g_Spike_y4[idx] = y - length * floatcos(a - (90.0 + value), degrees);

			#endif

			#endif

			g_Spike_zA[idx] = z;
			return idx;
		}
	}
  	return INVALID_OBJECT_ID;
}

#define SpikeStrip_Create Stinger_Create

/*
native Stinger_Delete(sid);
*/

stock Stinger_Delete(sid)
{
	if (Iter_Contains(g_Spike, sid))
	{
		SS_DestroyObject(g_SpikeStrip[sid]);
		Iter_Remove(g_Spike, sid);
	}
	return 1;
}

#define SpikeStrip_Delete Stinger_Delete

/*
native Stinger_DeleteAll();
*/

stock Stinger_DeleteAll()
{
	foreach (new i : g_Spike)
	{
		SS_DestroyObject(g_SpikeStrip[i]);
		Iter_SafeRemove(g_Spike, i, i);
	}
	return 1;
}

#define SpikeStrip_DeleteAll Stinger_DeleteAll

/*
native Stinger_IsValid(sid);
*/

stock Stinger_IsValid(sid)
{
	return Iter_Contains(g_Spike, sid);
}

#define SpikeStrip_IsValid Stinger_IsValid

/*
native Stinger_SetGhost(pid, bool:toggle);
*/

stock Stinger_SetGhost(pid, bool:toggle)
{
	g_GetVehicleID[pid] = 0;
	g_GetGhostMode{pid} = toggle;
	return 1;
}

#define SpikeStrip_SetGhost Stinger_SetGhost

/*
native Stinger_IsGhost(pid);
*/

stock Stinger_IsGhost(pid)
{
	return g_GetGhostMode{pid};
}

#define SpikeStrip_IsGhost Stinger_IsGhost