//dialogs
#define DIALOG_FURNITURE_MAIN 2996
#define DIALOG_FURNITURE_BUY 2997
#define DIALOG_FURNITURE_EDIT 2998
#define DIALOG_FURNITURE_BUY3 2999
#define DIALOG_FURNITURE_BUY_CONFIRM 3000
#define DIALOG_FURNITURE_EDIT_SELECT 3001
#define DIALOG_FURNITURE_SHOWHIDE_SELECT 3002
#define DIALOG_FURNITURE_DELETE_SELECT 3003

static str[128];

//max limits
#define MAX_FURNITURE 100
#define MAX_OUTDOOR_FURNITURE 30

//validation
#define INVALID_FURNITURE_ID 0

//list of furniture objects
enum fFurnitureObjectList {
	fD_type,
	fD_model,
	fD_name[50],
	fD_price,
	Float:fD_rotX,
	Float:fD_rotY
}

new FurnitureObjectList[][fFurnitureObjectList] = {
	{1, 2274, "Picture Of Flower", 1000, 0.0, 0.0},//Decor 0
	{1, 2279, "Picture of Chilliad", 1000, 0.0, 0.0},
	{1, 2265, "Picture of Desert", 1000, 0.0, 0.0},
	{1, 2268, "Picture of Cat", 1000, 0.0, 0.0},
	{1, 2277, "Picture of Snowball", 1000, 0.0, 0.0},
	{1, 2263, "Picture of Night City", 7500, 0.0, 0.0},
	{1, 2269, "Picture of Lake", 4500, 0.0, 0.0},
	{1, 2275, "Picture of Fruit", 1000, 0.0, 0.0},
	{1, 2261, "Picture of Bridge", 3500, 0.0, 0.0},
	{1, 2264, "Picture of Beach", 6000, 0.0, 0.0},
	{1, 2282, "Picture of Sunset", 2500, 0.0, 0.0},
	{1, 2255, "Picture of Ass", 10000, 0.0, 0.0},
	{1, 2287, "Picture of Boats", 5400, 0.0, 0.0},
	{1, 2276, "Picture of Bridges", 7500, 0.0, 0.0},
	{1, 2284, "Picture of Church", 8900, 0.0, 0.0},
	{1, 2285, "Picture of Map", 4500, 0.0, 0.0},
	{1, 2271, "Pic. of something Special", 4500, 0.0, 0.0},
	{1, 2273, "Picture of Flowers", 4000, 0.0, 0.0},
	{1, 2280, "Picture of River", 2500, 0.0, 0.0},
	{1, 2286, "Picture of Ship", 6500, 0.0, 0.0},
	{1, 2254, "Picture of Car", 2500, 0.0, 0.0},
	{1, 2256, "Picture of Palms", 2400, 0.0, 0.0},
	{1, 2257, "Modern Art", 9000, 0.0, 0.0},
	{1, 3962, "Modern Art 2", 10000, 0.0, -90.0},
	{1, 2587, "Poster1", 6800, 0.0, 0.0},
	{1, 2588, "Poster2", 9000, 0.0, 0.0},
	{1, 2691, "Poster3", 8500, 0.0, 0.0},
	{1, 19825, "Clock", 6500, 0.0, 180.0},
	{1, 2828, "Desk Picture Frames", 4500, 0.0, 0.0},
	{1, 1736, "Deer's Head", 8000, 0.0, 0.0},
	{1, 11733, "Rocking Horse", 10000, 0.0, 0.0},
	{1, 1828, "Tiger Rug", 15000, -90.0, 0.0},
	{1, 2841, "Rug 1", 5000, 90.0, 0.0},
	{1, 2835, "Rug 2", 5000, 90.0, 0.0},
	{1, 2842, "Rug 3", 5000, 90.0, 0.0},
	{1, 2847, "Rug 4", 5000, 90.0, 0.0},
	{1, 2817, "Rug 5", 5000, 90.0, 0.0},
	{1, 2815, "Rug 6", 5000, 90.0, 0.0},
	{1, 2833, "Rug 7", 5000, 90.0, 0.0},
	{1, 2834, "Rug 8", 5000, 90.0, 0.0},
	{1, 2836, "Rug 9", 5000, 90.0, 0.0},
	{1, 2818, "Rug 10", 5000, 90.0, 0.0},
	{1, 11737, "Doormat", 3000, 90.0, 180.0},
	{1, 11712, "Cross", 2500, 0.0, 90.0},
	{1, 11710, "Exit Sign", 2500, 0.0, 180.0},
	{1, 18664, "Tag Sign 1", 4500, 0.0, 180.0},
	{1, 18665, "Tag Sign 2", 4500, 0.0, 180.0},
	{1, 18666, "Tag Sign 3", 4500, 0.0, 180.0},
	{1, 18667, "Tag Sign 4", 4500, 0.0, 180.0},
	{1, 18663, "Tag Sign 5", 4500, 0.0, 180.0},
	{1, 18662, "Tag Sign 6", 4500, 0.0, 180.0},
	{1, 18661, "Tag Sign 7", 4500, 0.0, 180.0},
	{1, 18660, "Tag Sign 8", 4500, 0.0, 180.0},
	{1, 18659, "Tag Sign 9", 4500, 0.0, 180.0},
	{1, 933, "Cable Roll", 6500, 0.0, 0.0},
	{1, 19836, "Blood Splatter", 11000, 90.0, 180.0},
	{1, 2068, "Cargo Net", 20000, 0.0, 0.0},
	{1, 2945, "Netting", 8000, 0.0, 0.0},
	{1, 948, "Pot plant", 5000, 0.0, 0.0},
	{1, 949, "Pot plant2", 5000, 0.0, 0.0},
	{1, 950, "Pot plant4", 5000, 0.0, 0.0},
	{1, 2010, "Plant", 5000, 0.0, 0.0},
	{1, 2011, "Plant 2",5000, 0.0, 0.0},
	{1, 2176, "Vase", 8000, 0.0, 0.0},
	{1, 2194, "Cactus", 5600, 0.0, 0.0},
	{1, 2195, "Bush", 9000, 0.0, 0.0},
	{1, 2240, "Pot plant", 5000, 0.0, 0.0},
	{1, 2241, "Pot plant", 5000, 0.0, 0.0},
	{1, 2242, "Pot plant", 5000, 0.0, 0.0},
	{1, 2243, "Pot plant", 5000, 0.0, 0.0},
	{1, 2244, "Pot plant", 5000, 0.0, 0.0},
	{1, 2245, "Pot plant", 5000, 0.0, 0.0},
	{1, 2246, "Vase", 5000, 0.0, 0.0},
	{1, 2247, "Vase 1", 5000, 0.0, 0.0},
	{1, 2248, "Pot", 2500, 0.0, 0.0},
	{1, 2249, "Vase 2", 4500, 0.0, 0.0},
	{1, 2250, "Vase 3", 4500, 0.0, 0.0},
	{1, 2251, "Vase 4", 5000, 0.0, 0.0},
	{1, 14705, "Vase 5", 5000, 0.0, 0.0},
	{1, 2252, "Pot plant", 4500, 0.0, 0.0},
	{1, 2253, "Pot plant", 4500, 0.0, 0.0},
	{1, 2868, "Candle", 7500, 0.0, 0.0},
	{1, 2869, "Candles", 7500, 0.0, 0.0},
	{1, 3802, "Hanging plant1", 6500, 0.0, 0.0},
	{1, 3809, "Hanging plant2", 6500, 0.0, 0.0},
	{1, 3810, "Hanging plant3", 6500, 0.0, 0.0},
	{1, 14804, "Pot plant1", 5000, 0.0, 0.0},
	{1, 14834, "Pot plant2", 5000, 0.0, 0.0},
	{1, 2067, "Filing Cabinet", 5000, 0.0, 0.0},//Office 90
	{1, 2609, "Filing Cabinet 2", 6000, 0.0, 0.0},
	{1, 2065, "Filing Cabinet 3", 10000, 0.0, 0.0},
	{1, 2066, "Filing Cabinet 4", 10000, 0.0, 0.0},
	{1, 2610, "Filing Cabinet 5", 10000, 0.0, 0.0},
	{1, 2167, "Office Cupboard", 11000, 180.0, 180.0},
	{1, 2161, "Office Bookshelf", 15000, 180.0, 180.0},
	{1, 2163, "Office Wall Cupboard", 25000, 180.0, 180.0},
	{1, 2737, "Blue Notice Board", 30000, 0.0, 0.0},
	{1, 2191, "Filing Case", 20000, 180.0, 180.0},
	{1, 2162, "Office Bookshelf", 15000, 180.0, 180.0},
	{1, 2608, "Office Bookshelf 2", 15000, 180.0, 180.0},
	{1, 2199, "Office BookShelf 3", 15000, 180.0, 180.0},
	{1, 2164, "Office Bookshelf 3", 15000, 180.0, 180.0},
	{1, 2204, "Brown Office Cabinet", 16000, 180.0, 180.0},
	{1, 2208, "Ellipse Office Unit", 17000, 0.0, 0.0},
	{1, 14455, "Bookshelves", 25000, 0.0, 0.0},
	{1, 2202, "Large Printer", 15000, 0.0, 0.0},
	{1, 19805, "Whiteboard", 10000, 0.0, 0.0},
	{1, 2616, "Noticeboard", 12000, 0.0, 0.0},
	{1, 3077, "Big Chalkboard", 15000, 0.0, 0.0},
	{1, 19999, "Office Chair", 7500, 180.0, 180.0},
	{1, 2356, "Office Chair 2", 8000, 0.0, 0.0},
	{1, 1671, "Office Chair 3", 10000, 180.0, 180.0},
	{1, 1714, "Office Chair 4", 12000, 180.0, 180.0},
	{1, 1663, "Office Chair 5", 15000, 180.0, 180.0},
	{1, 1715, "Office Chair 6", 15000, 180.0, 180.0},
	{1, 1721, "Office Chair 7", 15000, 0.0, 0.0},
	{1, 2310, "Office Chair 8", 15000, 0.0, 0.0},
	{1, 2308, "Desk 1", 20000, 0.0, 180.0},
	{1, 2185, "Desk 2", 20000, 0.0, 0.0},
	{1, 2184, "Desk 3", 25000, 0.0, 0.0},
	{1, 2206, "Desk 4", 25000, 0.0, 0.0},
	{1, 2008, "Desk 5", 25000, 0.0, 0.0},
	{1, 2207, "Desk 6", 25000, 0.0, 0.0},
	{1, 2205, "Desk 7", 25000, 0.0, 0.0},
	{1, 1963, "Desk 8", 25000, 0.0, 0.0},
	{1, 2174, "Desk 9", 25000, 0.0, 180.0},
	{1, 2173, "Desk 10", 30000, 0.0, 0.0},
	{1, 2166, "Desk 11", 30000, 0.0, 0.0},
	{1, 2165, "Desk 12", 30000, 0.0, 0.0},
	{1, 2605, "Desk 13", 30000, 0.0, 0.0},
	{1, 2607, "Desk 14", 35000, 0.0, 0.0},
	{1, 1999, "Desk 15", 35000, 0.0, 0.0},
	{1, 2894, "Opened Book", 5000, 0.0, 0.0},//Hobby 134
	{1, 19939, "Rectangle Shelf", 6000, 0.0, 0.0},
	{1, 19938, "Corner Shelf", 4000, 0.0, 0.0},
	{1, 1961, "Record Green", 2000, 0.0, 0.0},
	{1, 1962, "Record Purple", 2000, 0.0, 0.0},
	{1, 2114, "Basket Ball", 3000, 0.0, 0.0},
	{1, 3496, "Basket ball hoop", 5000, 0.0, 0.0},
	{1, 3497, "Basket ball stand", 10000, 0.0, 0.0},
	{1, 1974, "Golf Ball", 1500, 0.0, 0.0},
	{1, 3017, "Blueprints", 5000, 0.0, 0.0},
	{1, 3111, "Blueprints 1", 5000, 0.0, 0.0},
	{1, 2046, "Hobby Cupboard", 15000, 0.0, 0.0},
	{1, 14820, "DJ Stuff", 22000, 0.0, 0.0},
	{1, 1742, "Hobby Bookshelf", 16000, 0.0, 0.0},
	{1, 941, "Hobby Work Bench", 2500, 0.0, 0.0},
	{1, 14556, "Open Cupboard", 15000, 0.0, 0.0},
	{1, 2855, "Stack of Magazines", 4000, 0.0, 0.0},
	{1, 2854, "Pile of Books", 4000, 0.0, 0.0},
	{1, 2853, "Pile of Books 2", 4000, 0.0, 0.0},
	{1, 2852, "Pile of Magazines", 6000, 0.0, 0.0},
	{1, 2816, "Opened Magazines", 7000, 0.0, 0.0},
	{1, 19921, "Toolbox", 6000, 0.0, 0.0},
	{1, 19878, "Skateboard", 10000, 0.0, 0.0},
	{1, 2630, "Exercise Bike", 5000, 0.0, 0.0},
	{1, 2629, "Gym Bench", 15000, 0.0, 0.0},
	{1, 2628, "Leg Press", 15000, 0.0, 0.0},
	{1, 2627, "Treadmill", 15000, 0.0, 0.0},
	{1, 2916, "Dumbbell", 5000, 0.0, 0.0},
	{1, 2913, "Bar", 10000, 0.0, 0.0},
	{1, 1985, "Boxing bag", 8000, 0.0, 0.0},
	{1, 19815, "Workshop Tools", 8000, 0.0, 0.0},
	{1, 19621, "Oil Can", 600, 0.0, 0.0},
	{1, 19897, "Packet of Smokes", 200, 0.0, 0.0},
	{1, 19609, "Drum Kit", 40000, 0.0, 0.0},
	{1, 19317, "Electric Guitar", 10000, 0.0, 0.0},
	{1, 19610, "Microphone", 10000, 0.0, 0.0},
	{1, 19611, "Microphone Stand", 10000, 0.0, 0.0},
	{1, 19613, "Guitar Amp", 7000, 0.0, 0.0},
	{1, 19624, "Travelling Suitcase", 4000, 0.0, 0.0},
	{1, 11729, "Locker", 8000, 0.0, 0.0},
	{1, 11738, "Medical Box", 2000, 0.0, 0.0},
	{1, 11736, "Medical Pack", 2000, 0.0, 0.0},
	{1, 11745, "Big Black Dufflebag", 5000, 0.0, 0.0},
	{1, 1954, "Turn Table", 5000, 0.0, 0.0},
	{1, 2964, "Pool Table", 20000, 0.0, 0.0},
	{1, 14651, "Pool Table Set", 10000, 0.0, 0.0},
	{1, 2965, "Pool Triangle", 5000, 0.0, 0.0},
	{1, 3003, "Pool White Ball", 2000, 0.0, 0.0},
	{1, 3002, "Pool Orange Ball", 2000, 0.0, 0.0},
	{1, 3106, "Pool 8 Ball", 26000, 0.0, 0.0},
	{1, 3004, "Pool Cue", 10000, 0.0, 0.0},
	{1, 19918, "Magic Box", 5000, 0.0, 0.0},
	{1, 11725, "Black Fireplace", 15000, 0.0, 0.0},//Lounge 186
	{1, 11724, "Large Stone Fireplace", 20000, 0.0, 0.0},
	{1, 19632, "Wooden Logs", 5000, 0.0, 0.0},
	{1, 2313, "TV Cabinet", 20000, 0.0, 0.0},
	{1, 2311, "TV Cabinet 2", 20000, 0.0, 0.0},
	{1, 2078, "China Cabinet", 20000, 0.0, 0.0},
	{1, 2083, "Glass Coffee Table", 20000, 0.0, 0.0},
	{1, 1821, "Wooden Trolley", 15000, 0.0, 0.0},
	{1, 2082, "Dark Square Coffee", 20000, 0.0, 0.0},
	{1, 1823, "Dark Coffee Table", 20000, 0.0, 0.0},
	{1, 1822, "Round Coffee Table", 15000, 0.0, 0.0},
	{1, 1815, "Round Wood Coffee", 15000, 0.0, 0.0},
	{1, 2236, "Rectangle Coffee Tab", 20000, 0.0, 0.0},
	{1, 1814, "Wooden Coffee Table", 20000, 0.0, 0.0},
	{1, 2081, "Semi-circle Coffee", 15000, 0.0, 0.0},
	{1, 1819, "Round Coffee Table", 15000, 0.0, 0.0},
	{1, 2235, "Old Coffee Table", 8000, 0.0, 0.0},
	{1, 2084, "Glass China Cabinet", 15000, 0.0, 0.0},
	{1, 1759, "Weaved Old Chair", 15000, 180.0, 180.0},
	{1, 1765, "Beige Sofa Chair", 20000, 180.0, 180.0},
	{1, 1758, "Dark Sofa Chair", 20000, 180.0, 180.0},
	{1, 1755, "Blue Sofa Chair", 20000, 180.0, 180.0},
	{1, 1769, "Blue Wood Sofa Chair", 20000, 180.0, 180.0},
	{1, 1762, "Basic Sofa Chair", 20000, 180.0, 180.0},
	{1, 1767, "Basic Sofa Chair 2", 20000, 180.0, 180.0},
	{1, 11717, "Love Sofa", 30000, 180.0, 180.0},
	{1, 1707, "Future Sofa", 60000, 0.0, 0.0},
	{1, 1764, "Beige Sofa", 50000, 180.0, 180.0},
	{1, 1763, "Weaved Sofa", 50000, 180.0, 180.0},
	{1, 1712, "Old Worn Sofa", 30000, 180.0, 180.0},
	{1, 1702, "Brown Sofa", 45000, 180.0, 180.0},
	{1, 1706, "Chip Sofa", 25000, 180.0, 180.0},
	{1, 1768, "Blue Sofa", 50000, 180.0, 180.0},
	{1, 1766, "Budget Sofa", 25000, 180.0, 180.0},
	{1, 1757, "Budget Dark Sofa", 25000, 180.0, 180.0},
	{1, 1761, "Budget Dark Sofa 2", 25000, 180.0, 180.0},
	{1, 1713, "Business Sofa", 80000, 180.0, 180.0},
	{1, 1703, "Black Posh Sofa", 50000, 180.0, 180.0},
	{1, 1760, "Blue Sofa 2", 50000, 180.0, 180.0},
	{1, 1756, "Dark Budget Sofa", 35000, 180.0, 180.0},
	{1, 14491, "Sofa & Table Set", 45000, 0.0, 0.0},
	{1, 1710, "Large Old Sofa", 30000, 180.0, 180.0},
	{1, 14493, "Two Sofa Set", 30000, 0.0, 0.0},
	{1, 2293, "Ottomat", 10000, 0.0, 0.0},
	{1, 1735, "Chair", 10000, 180.0, 180.0},
	{1, 11734, "Rocking Chair", 10000, 180.0, 180.0},
	{1, 2096, "Rocking Chair 2", 10000, 180.0, 180.0},
	{1, 2306, "Cabinet", 10000, 180.0, 180.0},//Bedroom
	{1, 2021, "Pink Drawers", 12000, 180.0, 180.0},
	{1, 2020, "Pink Drawers 2", 14000, 180.0, 180.0},
	{1, 1740, "Wooden Bedside", 10000, 180.0, 180.0},
	{1, 2095, "Bedside Drawers", 11000, 180.0, 180.0},
	{1, 1417, "Ugly Wardroom", 10000, 180.0, 180.0},
	{1, 2323, "Wooden Drawers", 10000, 180.0, 180.0},
	{1, 1743, "Medium Cabinet", 10000, 180.0, 180.0},
	{1, 2145, "Clothes Cabinet", 10000, 180.0, 180.0},
	{1, 2025, "Posh Wardroom", 12000, 180.0, 180.0},
	{1, 2329, "Wooden Wardroom", 12000, 180.0, 180.0},
	{1, 2569, "White Dresser", 12000, 180.0, 180.0},
	{1, 2574, "Dresser & Table", 18000, 180.0, 180.0},
	{1, 2570, "Dresser & Utils", 15000, 180.0, 180.0},
	{1, 2576, "Wooden Dressers", 17000, 180.0, 180.0},
	{1, 2568, "Dark Table & Lamps", 14000, 180.0, 180.0},
	{1, 1812, "Foldup Bed", 10000, 180.0, 180.0},
	{1, 1771, "Mattress & Base Bed", 20000, 180.0, 180.0},
	{1, 1796, "Single Bed", 10000, 180.0, 180.0},
	{1, 11720, "King Single Bed", 60000, 180.0, 180.0},
	{1, 2302, "Queen Bed", 60000, 180.0, 180.0},
	{1, 1794, "Queen Bed 2", 60000, 180.0, 180.0},
	{1, 14866, "Queen Bed 3", 60000, 180.0, 180.0},
	{1, 1700, "Queen Bed 4", 60000, 180.0, 180.0},
	{1, 2301, "Queen Bed 5", 60000, 180.0, 180.0},
	{1, 2090, "Queen Bed 6", 60000, 180.0, 180.0},
	{1, 2299, "Queen Bed 7", 60000, 180.0, 180.0},
	{1, 1797, "Queen Bed 8", 60000, 180.0, 180.0},
	{1, 1701, "Queen Bed 9", 60000, 180.0, 180.0},
	{1, 1745, "Queen Bed 10", 60000, 180.0, 180.0},
	{1, 1795, "Queen Bed 11", 60000, 180.0, 180.0},
	{1, 1798, "Queen Bed 12", 60000, 180.0, 180.0},
	{1, 1799, "Queen Bed 13", 60000, 180.0, 180.0},
	{1, 1801, "Queen Bed 14", 60000, 180.0, 180.0},
	{1, 1802, "Queen Bed 15", 60000, 180.0, 180.0},
	{1, 1803, "Queen Bed 16", 60000, 180.0, 180.0},
	{1, 1793, "Queen Mattresses", 12000, 180.0, 180.0},
	{1, 14880, "Queen Mattresses 2", 12000, 180.0, 180.0},
	{1, 14861, "Queen Mattresses 3", 12000, 180.0, 180.0},
	{1, 2575, "Bedroom Set, Bed", 50000, 180.0, 180.0},
	{1, 11731, "Heart Bed", 80000, 180.0, 180.0},
	{1, 2399, "Rack of Clothes", 20000, 0.0, 0.0},
	{1, 11735, "Boot", 2000, 0.0, 0.0},
	{1, 2689, "Shirt", 4000, 0.0, 0.0},
	{1, 2704, "Shirt2", 4000, 0.0, 0.0},
	{1, 2706, "Shirt3", 4000, 0.0, 0.0},
	{1, 2694, "Shoebox", 5000, 0.0, 0.0},
	{1, 2003, "Safe1", 10000, 180.0, 180.0},
	{1, 2004, "Safe2", 10000, 180.0, 180.0},
	{1, 2005, "Safe3", 10000, 180.0, 180.0},
	{1, 19931, "Dark Counter", 10000, 180.0, 180.0},//Kitchen 283
	{1, 19928, "Dark Counter 2", 10000, 180.0, 180.0},
	{1, 19930, "Dark Counter 3", 10000, 180.0, 180.0},
	{1, 19932, "Dark Oven Cabinet", 10000, 180.0, 180.0},
	{1, 19935, "Dark Cupboard", 10000, 180.0, 180.0},
	{1, 19925, "Dark Corner Counter", 10000, 180.0, 180.0},
	{1, 19936, "Dark Cut Counter", 10000, 180.0, 180.0},
	{1, 19937, "Dark Cupboard 2", 10000, 180.0, 180.0},
	{1, 2089, "Dark Cupboard 3", 10000, 180.0, 180.0},
	{1, 19927, "Dark Sink", 10000, 180.0, 180.0},
	{1, 19923, "Dark Island", 30000, 180.0, 180.0},
	{1, 19929, "Dark Trip. Counter", 20000, 180.0, 180.0},
	{1, 2139, "Maple Counter", 12000, 180.0, 180.0},
	{1, 2303, "Maple Dishwasher", 12000, 180.0, 180.0},
	{1, 2133, "Steel Counter", 15000, 180.0, 180.0},
	{1, 2134, "Steel Counter 2", 15000, 180.0, 180.0},
	{1, 2341, "Steel Corner Counter", 15000, 180.0, 180.0},
	{1, 2335, "Green Counter", 12000, 180.0, 180.0},
	{1, 2334, "Green Counter 2", 12000, 180.0, 180.0},
	{1, 2338, "Green Corner Counter", 12000,180.0, 180.0},
	{1, 2156, "Green Doub. Counter", 12000, 180.0, 180.0},
	{1, 2157, "Green Doub Counter 2", 12000, 180.0, 180.0},
	{1, 2159, "Green Slot Counter", 12000, 180.0, 180.0},
	{1, 2160, "Green Sink", 12000, 180.0, 180.0},
	{1, 2155, "White Counter", 30000, 180.0, 180.0},
	{1, 2152, "White Long Counter", 30000, 180.0, 180.0},
	{1, 2154, "White Sink", 30000, 180.0, 180.0},
	{1, 2151, "White Top Bot Counter", 30000, 180.0, 180.0},
	{1, 2142, "Black/Wood Counter", 50000, 180.0, 180.0},
	{1, 2148, "Black/Wood Counter 2", 50000, 180.0, 180.0},
	{1, 2148, "Black/Wood Dishwasher", 50000, 180.0, 180.0},
	{1, 2136, "Counter with Sink", 1500, 180.0, 180.0},
	{1, 2013, "Orange Counter Sink", 20000, 180.0, 180.0},
	{1, 2015, "Orange Counter", 20000, 180.0, 180.0},
	{1, 2017, "Orange Oven Counter", 20000, 180.0, 180.0},
	{1, 2018, "Orange Wash Machine", 20000, 180.0, 180.0},
	{1, 2022, "Orange Corner Counter", 20000, 180.0, 180.0},
	{1, 2016, "Orange Draws Counter", 20000, 180.0, 180.0},
	{1, 2019, "Orange Fridge Counter", 20000, 180.0, 180.0},
	{1, 2127, "Kitchenset red", 45000, 180.0, 180.0},
	{1, 2128, "Kitchenset red", 45000, 180.0, 180.0},
	{1, 2129, "Kitchenset red", 45000, 180.0, 180.0},
	{1, 2130, "Kitchenset red", 45000, 180.0, 180.0},
	{1, 2092, "Circle Pantry", 18000, 180.0, 180.0},
	{1, 19915, "White Old Stove", 22000, 180.0, 180.0},
	{1, 936, "Work Bench 1", 15000, 180.0, 180.0},
	{1, 937, "Work Bench 2", 15000, 180.0, 180.0},
	{1, 2451, "Work Bench 3", 15000, 180.0, 180.0},
	{1, 2419, "Work Bench 4", 15000, 180.0, 180.0},
	{1, 2418, "Work Bench 5", 15000, 180.0, 180.0},
	{1, 19916, "Fridge", 80000, 180.0, 180.0},
	{1, 2532, "Bar Fridge", 80000, 180.0, 180.0},
	{1, 19933, "Oven", 50000, 180.0, 180.0},
	{1, 1773, "Oven 2", 50000, 180.0, 180.0},
	{1, 1777, "Oven 3", 50000, 180.0, 180.0},
	{1, 2144, "Oven 4", 50000, 180.0, 180.0},
	{1, 19924, "Extraction Hood", 22000, 0.0, 0.0},
	{1, 2150, "Sink", 8000, 180.0, 180.0},
	{1, 2427, "Sprunk Machine", 80000, 180.0, 180.0},
	{1, 11743, "Coffee Machine", 50000,180.0, 180.0},
	{1, 1808, "Water Dispenser", 50000, 180.0, 180.0},
	{1, 2832, "Dirty Dishes", 2000, 0.0, 0.0},
	{1, 2850, "Dirty Dishes 2", 2000, 0.0, 0.0},
	{1, 19993, "Bowl", 2000, 0.0, 0.0},
	{1, 11744, "Plate", 2000, 0.0, 0.0},
	{1, 2822, "Plate Set", 3000, 0.0, 0.0},
	{1, 2829, "Plate Set 2", 3000, 0.0, 0.0},
	{1, 2864, "Bowl Set", 3000, 0.0, 0.0},
	{1, 19835, "Coffee Cup", 2000, 0.0, 0.0},
	{1, 2437, "Cup Pile", 2000, 0.0, 0.0},
	{1, 11718, "Sauce Pan 1", 2000, 0.0, 0.0},
	{1, 11719, "Sauce Pan 2", 2000, 0.0, 0.0},
	{1, 19585, "Sauce Pan 3", 2000, 0.0, 0.0},
	{1, 19584, "Sauce Pan 4", 2000, 0.0, 0.0},
	{1, 19581, "Fry pan", 2000, 0.0, 0.0},
	{1, 19586, "Spatula", 2000, 0.0, 0.0},
	{1, 19583, "Sharp Knife", 2000, 0.0, 0.0},
	{1, 11715, "Fork", 2000, 0.0, 0.0},
	{1, 11716, "Butter Knife", 2000, 0.0, 0.0},
	{1, 19582, "Meat", 2000, 0.0, 0.0},
	{1, 19882, "Cooked Meat", 2000, 0.0, 0.0},
	{1, 19580, "Pizza", 2000, 0.0, 0.0},
	{1, 19571, "Pizza Box", 2000, 0.0, 0.0},
	{1, 2881, "Slice of Pizza", 2000, 0.0, 0.0},
	{1, 19576, "Green Apple", 2000, 0.0, 0.0},
	{1, 19575, "Red Apple", 2000, 0.0, 0.0},
	{1, 19578, "Banana", 2000, 0.0, 0.0},
	{1, 19579, "Loaf of Bread", 2000, 0.0, 0.0},
	{1, 19561, "Cereal Box 1", 2000, 0.0, 0.0},
	{1, 19562, "Cereal Box 2", 2000, 0.0, 0.0},
	{1, 19563, "Orange Juice", 2000, 0.0, 0.0},
	{1, 19564, "Apple Juice", 2000, 0.0, 0.0},
	{1, 19566, "Fish Fingers", 2000, 0.0, 0.0},
	{1, 19567, "Ice Cream", 2000, 0.0, 0.0},
	{1, 19570, "Bottle of Milk", 2000, 0.0, 0.0},
	{1, 19572, "Box of Beer", 2000, 0.0, 0.0},
	{1, 19573, "Bag of Coal", 2000, 0.0, 0.0},
	{1, 19574, "Orange", 2000, 0.0, 0.0},
	{1, 19630, "Fish", 2000, 0.0, 0.0},
	{1, 19818, "Wine Glass", 2000, 0.0, 0.0},
	{1, 19820, "Wine Bottle 1", 2000, 0.0, 0.0},
	{1, 19821, "Wine Bottle 2", 2000, 0.0, 0.0},
	{1, 19822, "Wine Bottle 3", 2000, 0.0, 0.0},
	{1, 19824, "Wine Bottle 4", 2000, 0.0, 0.0},
	{1, 19883, "Piece of Bread", 2000, 0.0, 0.0},
	{1, 19587, "Plastic Pallet", 2000, 0.0, 0.0},
	{1, 11722, "Ketchup", 2000, 0.0, 0.0},
	{1, 11723, "Mustard", 2000, 0.0, 0.0},
	{1, 11741, "Cake", 2000, 0.0, 0.0},
	{1, 11742, "Slice of Cake", 2000, 0.0, 0.0},
	{1, 11706, "Trashcan", 5000, 0.0, 0.0},
	{1, 1486, "Bottle 1", 1000, 0.0, 0.0},
	{1, 1487, "Bottle 2", 1000, 0.0, 0.0},
	{1, 1509, "Bottle 3", 1000, 0.0, 0.0},
	{1, 1520, "Bottle 4", 1000, 0.0, 0.0},
	{1, 1543, "Bottle 5", 1000, 0.0, 0.0},
	{1, 1544, "Bottle 6", 1000, 0.0, 0.0},
	{1, 1664, "Wine", 5000, 0.0, 0.0},
	{1, 1668, "Wine 2", 5000, 0.0, 0.0},
	{1, 1669, "Wine 3", 5000, 0.0, 0.0},
	{1, 1670, "Wine 4", 5000, 0.0, 0.0},
	{1, 1665, "Ashtray", 1000, 0.0, 0.0},
	{1, 1546, "Cup", 1000, 0.0, 0.0},
	{1, 1950, "Bottle 7", 1000, 0.0, 0.0},
	{1, 2829, "Dishes", 800, 0.0, 0.0},
	{1, 2822, "Dishes2", 800, 0.0, 0.0},
	{1, 2830, "Dishes3", 800, 0.0, 0.0},
	{1, 2831, "Dishes4", 800, 0.0, 0.0},
	{1, 2832, "Dishes5", 800, 0.0, 0.0},
	{1, 2848, "Dishes6", 80, 0.0, 0.0},
	{1, 2849, "Dishes7", 800, 0.0, 0.0},
	{1, 2856, "Milk", 1000, 0.0, 0.0},
	{1, 2857,"Empty Pizza",  1000, 0.0, 0.0},
	{1, 2858, "Empty Chinese", 1000, 0.0, 0.0},
	{1, 2860, "Pizza", 2000, 0.0, 0.0},
	{1, 2803, "Meat", 4000, 0.0, 0.0},
	{1, 1820, "Round Dining Table", 16000, 0.0, 0.0},//Dining 419
	{1, 2112, "Medium Wood Table", 16000, 0.0, 0.0},
	{1, 2080, "Ellipse Wood Table", 16000, 0.0, 0.0},
	{1, 2110, "Basic Dining Table", 16000, 0.0, 0.0},
	{1, 1737, "Basic Dining Table 2", 16000, 0.0, 0.0},
	{1, 1770, "Basic Dining Table 3", 16000, 0.0, 0.0},
	{1, 2032, "Basic Dining Table 4", 16000, 0.0, 0.0},
	{1, 2118, "Maple Dining Table", 18000, 0.0, 0.0},
	{1, 2030, "Round Maple Dining", 20000, 0.0, 0.0},
	{1, 15037, "Table with Chair & TV", 50000, 0.0, 0.0},
	{1, 19997, "Small Table", 5000, 0.0, 0.0},
	{1, 1516, "Small Table 2", 5000, 0.0, 0.0},
	{1, 2764, "Cloth Table", 8000, 0.0, 0.0},
	{1, 1811, "Dining Chair 1", 4000, 0.0, 0.0},
	{1, 2636, "Dining Chair 2", 4000, 0.0, 0.0},
	{1, 2121, "Dining Chair 3", 4000, 0.0, 0.0},
	{1, 2776, "Dining Chair 4", 5000, 0.0, 0.0},
	{1, 2120, "Dining Chair 5", 5000, 0.0, 0.0},
	{1, 1739, "Dining Chair 6", 10000, 0.0, 0.0},
	{1, 2807, "Dining Chair 7", 10000, 0.0, 0.0},
	{1, 1720, "Dining Chair 8", 10000, 0.0, 0.0},
	{1, 2123, "Dining Chair 9", 10000, 0.0, 0.0},
	{1, 2124, "Dining Chair 10", 10000, 0.0, 0.0},
	{1, 19996, "Dining Chair 11", 10000, 0.0, 0.0},
	{1, 1716, "Stool", 4000, 0.0, 0.0},
	{1, 2350, "Stool 2", 4000, 0.0, 0.0},
	{1, 2125, "Stool 3", 4000, 0.0, 0.0},
	{1, 1805, "Stool 4", 4000, 0.0, 0.0},
	{1, 2802, "Table & Chairs", 30000, 0.0, 0.0},
	{1, 2801, "Table Top 1", 15000, 0.0, 0.0},
	{1, 2800, "Table Top 2", 15000, 0.0, 0.0},
	{1, 1670, "Table Top 3", 15000, 0.0, 0.0},
	{1, 11732, "Heart Bath Tub", 69000, 0.0, 0.0},//Bathroom 451
	{1, 2526, "Bath Tub", 30000, 0.0, 0.0},
	{1, 2516, "Bath Tub 2", 30000, 0.0, 0.0},
	{1, 2519, "Bath Tub 3", 30000, 0.0, 0.0},
	{1, 2522, "Bath Tub 4", 30000, 0.0, 0.0},
	{1, 14494, "Shower Bath", 15000, 0.0, 0.0},
	{1, 2527, "Shower", 15000, 0.0, 0.0},
	{1, 2517, "Shower 2", 15000, 0.0, 0.0},
	{1, 2528, "Toilet", 5000, 0.0, 0.0},
	{1, 2521, "Toilet 2", 4000, 0.0, 0.0},
	{1, 2525, "Toilet 3", 5000, 0.0, 0.0},
	{1, 19873, "Toilet Paper", 2000, 0.0, 0.0},
	{1, 2515, "Sink", 6000, 0.0, 0.0},
	{1, 2518, "Sink 2", 8000, 0.0, 0.0},
	{1, 2523, "Sink 3", 10000, 0.0, 0.0},
	{1, 2524, "Sink 4", 8000, 0.0, 0.0},
	{1, 2739, "Sink 5", 9000, 0.0, 0.0},
	{1, 11707, "Towel Rack", 5000, 0.0, 0.0},
	{1, 19874, "Bar of Soap", 500, 0.0, 0.0},
	{1, 2742, "Hand dryer", 3000, 0.0, 0.0},
	{1, 2750, "Hair dryer", 3000, 0.0, 0.0},
	{1, 2752, "Hair product", 2000, 0.0, 0.0},
	{1, 2751, "Hair product2", 2000, 0.0, 0.0},
	{1, 2741, "Soap dispenser", 7000, 0.0, 0.0},
	{1, 11709, "Sink", 12000, 0.0, 0.0},//Laundry 475
	{1, 1208, "Washer", 15000, 0.0, 0.0},
	{1, 19622, "Broom", 2000, 0.0, 0.0},
	{1, 1778, "Mop & Bucket", 3000, 0.0, 0.0},
	{1, 2844, "Clothes 1", 2000, 0.0, 0.0},
	{1, 2845, "Clothes 2", 2000, 0.0, 0.0},
	{1, 2846, "Clothes 3", 2000, 0.0, 0.0},
	{1, 2819, "Clothes 4", 2000, 0.0, 0.0},
	{1, 2843, "Clothes 5", 2000, 0.0, 0.0},
	{1, 14401, "Benches", 9000, 0.0, 0.0},
	{1, 19920, "TV Remote", 2000, 0.0, 0.0},//Electronics 485
	{1, 19807, "Telephone", 40000, 0.0, 0.0},
	{1, 19808, "Keyboard", 5000, 0.0, 0.0},
	{1, 2149, "Microwave", 70000, 0.0, 0.0},
	{1, 2421, "Microwave 2", 80000, 0.0, 0.0},
	{1, 1719, "Console", 80000, 0.0, 0.0},
	{1, 2028, "Console 2", 80000, 0.0, 0.0},
	{1, 19830, "Blender", 8000, 0.0, 0.0},
	{1, 11705, "Old Telephone", 8000, 0.0, 0.0},
	{1, 2102, "Radio", 15000, 0.0, 0.0},
	{1, 14772, "Television", 45000, 0.0, 0.0},
	{1, 1790, "Video Player", 8000, 0.0, 0.0},
	{1, 1782, "Video Player 2", 8000, 0.0, 0.0},
	{1, 1747, "Old Television", 30000, 0.0, 0.0},
	{1, 1748, "Large Television", 50000, 0.0, 0.0},
	{1, 2320, "Large Television 2", 60000, 0.0, 0.0},
	{1, 2190, "Computer", 90000, 0.0, 0.0},
	{1, 1781, "Modern Television", 100000, 0.0, 0.0},
	{1, 2318, "Modern Television 2", 120000, 0.0, 0.0},
	{1, 2101, "Stero Set", 45000, 0.0, 0.0},
	{1, 19614, "Guitar Amp", 50000, 0.0, 0.0},
	{1, 2226, "Funk Stero", 35000, 0.0, 0.0},
	{1, 2322, "Grey Television", 40000, 0.0, 0.0},
	{1, 2595, "TV with Recorder", 30000, 0.0, 0.0},
	{1, 2596, "Wall TV", 150000, 0.0, 0.0},
	{1, 2231, "Speaker", 60000, 0.0, 0.0},
	{1, 2225, "Flash Stero", 50000, 0.0, 0.0},
	{1, 2227, "Flash Speaker", 50000, 0.0, 0.0},
	{1, 2426, "Pizza Oven", 12000, 0.0, 0.0},
	{1, 2233, "Speaker on Stand", 45000, 0.0, 0.0},
	{1, 2103, "Stero", 80000, 0.0, 0.0},
	{1, 2312, "Flash Television", 40000, 0.0, 0.0},
	{1, 1786, "Flash Television 2", 40000, 0.0, 0.0},
	{1, 2104, "HI-FI Sound Set", 70000, 0.0, 0.0},
	{1, 2229, "HI-FI Speaker", 50000, 0.0, 0.0},
	{1, 2232, "HI-FI Speaker 2", 50000, 0.0, 0.0},
	{1, 1717, "Television on Stand", 50000, 0.0, 0.0},
	{1, 11721, "Electric Heater", 30000, 0.0, 0.0},
	{1, 1738, "Old Radiator", 20000, 0.0, 0.0},
	{1, 2099, "Large Stero", 22000, 0.0, 0.0},
	{1, 14806, "TV & Stero Set", 80000, 0.0, 0.0},
	{1, 14527, "Ceiling Fan", 12000, 0.0, 0.0},
	{1, 19623, "Camera", 8000, 0.0, 0.0},
	{1, 19786, "LCD TV Big", 180000, 0.0, 0.0},
	{1, 19787, "LCD TV Medium", 150000, 0.0, 0.0},
	{1, 19893, "Laptop Open", 70000, 0.0, 0.0},
	{1, 19894, "Laptop Closed", 60000, 0.0, 0.0},
	{1, 19273, "Keypad of Security", 8000, 0.0, 0.0},
	{1, 1616, "Security Camera", 8000, 0.0, 0.0},
	{1, 920, "Generator", 45000, 0.0, 0.0},
	{1, 927, "Electric Wires", 8000, 0.0, 0.0},
	{1, 955, "Sprunk Machine", 45000, 0.0, 0.0},
	{1, 956, "Snack Machine", 50000, 0.0, 0.0},
	{1, 2966, "Cellphone1", 1000, 0.0, 0.0},
	{1, 2967, "Cellphone2", 1000, 0.0, 0.0},
	{1, 3785, "Wall Light", 5000, 0.0, 0.0},//Lights 540
	{1, 11727, "Wall Light 2", 5000, 0.0, 0.0},
	{1, 2239, "Light Stand", 5000, 0.0, 0.0},
	{1, 2107, "Desk Lamp", 5000, 0.0, 0.0},
	{1, 2069, "Floor Lamp", 5000, 0.0, 0.0},
	{1, 1731, "Wall Lamp", 5000, 0.0, 0.0},
	{1, 3801, "Wall Lamp 2", 5000, 0.0, 0.0},
	{1, 1734, "Ceiling Light", 5000, 0.0, 0.0},
	{1, 2073, "Ceiling Light 2", 5000, 0.0, 0.0},
	{1, 2074, "Ceiling Light 3", 5000, 0.0, 0.0},
	{1, 2076, "Ceiling Light 4", 5000, 0.0, 0.0},
	{1, 2026, "Ceiling Light 5", 5000, 0.0, 0.0},
	{1, 1893, "Ceiling Light 6", 5000, 0.0, 0.0},
	{1, 16779, "Ceiling Light & Fan", 10000, 0.0, 0.0},
	{1, 2196, "Desk Lamp", 5000, 0.0, 0.0},
	{1, 2238, "Lava Lamp", 5000, 0.0, 0.0},
	{1, 2726, "Red Lamp", 5000, 0.0, 0.0},
	{1, 3534, "Red Lamp 2", 5000, 0.0, 0.0},
	{1, 19806, "Chandelier", 30000, 0.0, 0.0},
	{1, 11726, "Chandelier 2", 30000, 0.0, 0.0},
	{1, 19827, "Light Switch", 2000, 0.0, 0.0},
	{1, 1280, "Wooden Bench", 5000, 0.0, 0.0},//Outdoor 561
	{1, 1430, "Trash Bin", 5000, 0.0, 0.0},
	{1, 1344, "Dumpster", 5000, 0.0, 0.0},
	{1, 1439, "Dumpster 2", 5000, 0.0, 0.0},
	{1, 1331, "Glass Dumpster", 5000, 0.0, 0.0},
	{1, 1333, "Waste Dumpster", 5000, 0.0, 0.0},
	{1, 1334, "Secured Dumpster", 5000, 0.0, 0.0},
	{1, 19831, "BBQ", 7000, 0.0, 0.0},
	{1, 1481, "BBQ 2", 10000, 0.0, 0.0},
	{1, 1369, "Old Wheelchair", 5000, 0.0, 0.0},
	{1, 1679, "Table & Chairs", 9000, 0.0, 0.0},
	{1, 1594, "Table & Chairs 2", 9000, 0.0, 0.0},
	{1, 1825, "Table & Chairs 3", 9000, 0.0, 0.0},
	{1, 642, "Umbrella", 5000, 0.0, 0.0},
	{1, 1438, "Boxes", 5000, 0.0, 0.0},
	{1, 1440, "Boxes 2", 5000, 0.0, 0.0},
	{1, 3408, "Mail Box", 5000, 0.0, 0.0},
	{1, 19867, "Mail Box 2", 5000, 0.0, 0.0},
	{1, 19626, "Spade", 6000, 0.0, 0.0},
	{1, 19631, "Sledge Hammer", 6000, 0.0, 0.0},
	{1, 1255, "Pool seat", 9000, 0.0, 0.0},
	{1, 1432, "Table", 10000, 0.0, 0.0},
	{1, 1547, "Beach towel", 2000, 0.0, 0.0},
	{1, 1640, "Beach towel 2", 2000, 0.0, 0.0},
	{1, 1642, "Beach towel 3", 2000, 0.0, 0.0},
	{1, 1643, "Beach towel 4", 2000, 0.0, 0.0},
	{1, 2818, "Beach towel 5", 2000, 0.0, 0.0},
	{1, 2817, "Beach towel 6", 2000, 0.0, 0.0},
	{1, 914, "Window vent", 4000, 0.0, 0.0},
	{1, 916, "Empty tray", 2000, 0.0, 0.0},
	{1, 917, "Empty tray large", 1000, 0.0, 0.0},
	{1, 922, "Boxes", 1000, 0.0, 0.0},
	{1, 923, "Boxes 2", 1000, 0.0, 0.0},
	{1, 924, "Empty trays", 2000, 0.0, 0.0},
	{1, 925, "Oil tanks", 2000, 0.0, 0.0},
	{1, 926, "Empty boxes", 2000, 0.0, 0.0},
	{1, 930, "Tanks", 2000, 0.0, 0.0},
	{1, 931, "Empty cage", 2000, 0.0, 0.0},
	{1, 933, "Wheel", 2000, 0.0, 0.0},
	{1, 935, "Oil Drum", 10000, 0.0, 0.0},
	{1, 1220, "Boxes 3", 10000, 0.0, 0.0},
	{1, 1227, "Dumpster", 5000, 0.0, 0.0},
	{1, 1264, "Trash bag", 5000, 0.0, 0.0},
	{1, 1271, "Wood crate", 5000, 0.0, 0.0},
	{1, 1300, "Outdoor bin", 5000, 0.0, 0.0},
	{1, 1327, "Tire", 2000, 0.0, 0.0},
	{1, 1328, "Trashcan", 5000, 0.0, 0.0},
	{1, 1349, "Trolley", 5000, 0.0, 0.0},
	{1, 1428, "Ladder", 5000, 0.0, 0.0},
	{1, 1437, "Ladder2", 5000, 0.0, 0.0},
	{1, 1440, "Trash bags", 10000, 0.0, 0.0},
	{1, 1451, "Kennel", 10000, 0.0, 0.0},
	{1, 1454, "Hay ball", 10000, 0.0, 0.0},
	{1, 1457, "Shack", 10000, 0.0, 0.0},
	{1, 1458, "Trailer", 10000, 0.0, 0.0},
	{1, 1463, "Wood pile", 10000, 0.0, 0.0},
	{1, 1490, "Tagging", 10000, 0.0, 0.0},
	{1, 1595, "Satellite dish", 100000, 0.0, 0.0},
	{1, 1598, "Beach Ball", 10000, 0.0, 0.0},
	{1, 1946, "Basket Ball", 10000, 0.0, 0.0},
	{1, 1617, "AC 2", 10000, 0.0, 0.0},
	{1, 1639, "Volleyball net", 30000, 0.0, 0.0},
	{1, 1689, "Roof vent", 10000, 0.0, 0.0},
	{1, 1691, "Roof vent2", 10000, 0.0, 0.0},
	{1, 2203, "Pot plants", 10000, 0.0, 0.0},
	{1, 2228, "Shovel", 10000, 0.0, 0.0},
	{1, 2237, "Shovel2", 10000, 0.0, 0.0},
	{1, 1492, "Door 1 (moves)", 5000, 0.0, 0.0},//Structural Changes 628
	{1, 1493, "Door 2", 5000, 0.0, 0.0},
	{1, 3061, "Door 3", 5000, 0.0, 0.0},
	{1, 1505, "Door 4", 5000, 0.0, 0.0},
	{1, 1495, "Door 5", 5000, 0.0, 0.0},
	{1, 1507, "Door 6", 5000, 0.0, 0.0},
	{1, 19860, "Door 7", 5000, 0.0, 0.0},
	{1, 1532, "Door 8", 5000, 0.0, 0.0},
	{1, 2911, "Door 9", 5000, 0.0, 0.0},
	{1, 3029, "Door 10 (breaks)", 10000, 0.0, 0.0},
	{1, 1569, "Door 11", 2500, 0.0, 0.0},
	{1, 2877, "Door 12", 2500, 0.0, 0.0},
	{1, 1566, "Door 13", 2500, 0.0, 0.0},
	{1, 19802, "Door 14", 2500, 0.0, 0.0},
	{1, 11714, "Door 15", 2500, 0.0, 0.0},
	{1, 19858, "Door 16", 2500, 0.0, 0.0},
	{1, 1535, "Door 17", 2500, 0.0, 0.0},
	{1, 1556, "Door 18", 2500, 0.0, 0.0},
	{1, 1555, "Door 19", 3000, 0.0, 0.0},
	{1, 1538, "Door 20", 3000, 0.0, 0.0},
	{1, 1536, "Door 21", 3000, 0.0, 0.0},
	{1, 1506, "Door 22", 5000, 0.0, 0.0},
	{1, 1504, "Door 23", 5000, 0.0, 0.0},
	{1, 1502, "Door 24 (moves)", 5000, 0.0, 0.0},
	{1, 1501, "Door 25", 2500, 0.0, 0.0},
	{1, 1499, "Door 26 (moves)", 5000, 0.0, 0.0},
	{1, 1498, "Door 27", 2500, 0.0, 0.0},
	{1, 1497, "Door 28", 2500, 0.0, 0.0},
	{1, 1496, "Door 29", 2500, 0.0, 0.0},
	{1, 1494, "Door 30 (moves)", 5000, 0.0, 0.0},
	{1, 1491, "Door 31 (moves)", 5000, 0.0, 0.0},
	{1, 2949, "Door 32", 2500, 0.0, 0.0},
	{1, 19466, "Window 1", 5000, 0.0, 0.0},
	{1, 3034, "Window 2", 5000, 0.0, 0.0},
	{1, 3032, "Window 3", 5000, 0.0, 0.0},
	{1, 14902, "Windows", 6000, 0.0, 0.0},
	{1, 14892, "Posh Windows", 10000, 0.0, 0.0},
	{1, 11103, "Old Window", 4000, 0.0, 0.0},
	{1, 3278, "Cottage Window", 5000, 0.0, 0.0},
	{1, 14740, "Blinds", 9000, 0.0, 0.0},
	{1, 18084, "Blind", 6000, 0.0, 0.0},
	{1, 2558, "Curtains 1", 5000, 0.0, 0.0},
	{1, 2559, "Curtains 2", 5000, 0.0, 0.0},
	{1, 2560, "Curtains 3", 5000, 0.0, 0.0},
	{1, 2561, "Curtains 4", 5000, 0.0, 0.0},
	{1, 14752, "Curtains 5", 5000, 0.0, 0.0},
	{1, 19368, "Beige Wall", 2500, 0.0, 0.0},
	{1, 19397, "Beige Doorway", 2500, 0.0, 0.0},
	{1, 19367, "Blue Wall", 2500, 0.0, 0.0},
	{1, 19395, "Blue Doorway", 2500, 0.0, 0.0},
	{1, 19369, "Grey Wall", 2500, 0.0, 0.0},
	{1, 19396, "Grey Doorway", 2500, 0.0, 0.0},
	{1, 19364, "Brick Wall", 2500, 0.0, 0.0},
	{1, 19393, "Brick Doorway", 2500, 0.0, 0.0},
	{1, 19354, "Wallpaper Wall", 2500, 0.0, 0.0},
	{1, 19384, "Wallpaper Doorway", 2500, 0.0, 0.0},
	{1, 19358, "Carpet Wall", 2500, 0.0, 0.0},
	{1, 19388, "Carpet Doorway", 2500, 0.0, 0.0},
	{1, 2755, "China Wall", 2500, 0.0, 0.0},
	{1, 14456, "Roof Patterns", 2500, 0.0, 0.0},
	{1, 1354, "Fan", 2500, 0.0, 0.0},
	{1, 919, "Air Con", 2500, 0.0, 0.0},
	{1, 2653, "Vent", 2500, 0.0, 0.0},
	{1, 2649, "Vent 2", 2500, 0.0, 0.0},
	{1, 3470, "Vent 3", 2500, 0.0, 0.0},
	{1, 19814, "Wall Socket", 2500, 0.0, 0.0}
};

enum furnInfo {
	f_ID, //id (prob un-needed)
	f_HouseID,
	f_SQLHouseID, //sql house id
	f_ModelID,
	f_ModelName[50],
	f_Spawned,
	f_ObjectID,
	f_Outdoor,
	f_VirtualWorld,
	Float:f_X,
	Float:f_Y,
	Float:f_Z,
	Float:f_RX,
	Float:f_RY,
	Float:f_RZ
}
new FurnitureInfo[MAX_HOUSES][MAX_FURNITURE][furnInfo];

new EditingFurnitureObjectID[MAX_PLAYERS] = -1;
new EditingFurnitureHouseID[MAX_PLAYERS] = -1;
new EditingFurnitureID[MAX_PLAYERS] = -1;

stock FindEmptyFurnitureSlot(houseid) {
	for(new i = 0; i < MAX_FURNITURE; i++) {
		if( !FurnitureInfo[houseid][i][f_ModelID] ) { //find empty slot without a modelid
			return i;
		}
	}
	return -1;
}

stock GetTotalFurnitureOutdoor(houseid) {
	new count = 0;
	for(new i = 0; i < MAX_FURNITURE; i++) {
		if(FurnitureInfo[houseid][i][f_Outdoor]) { //find empty slot without a modelid
			if( count >= MAX_OUTDOOR_FURNITURE ) { //limit max outside
				return count;
			}
			count++;
		}
	}
	return count;
}

stock LoadFurniture(houseid, sqlhouseid) {
	new query[200];
	mysql_format(sqlGameConnection, query, sizeof(query), "SELECT * FROM `furniture` WHERE `HouseID` = '%d' AND `Active`=1", sqlhouseid);
	mysql_tquery(sqlGameConnection, query, "OnLoadFurniture", "i", houseid);
}

forward OnLoadFurniture(houseid);
public OnLoadFurniture(houseid) {
	if(cache_num_rows() == 0) return printf("[Furniture] WARNING: No furniture found for house id %d but hasFurniture is set to true.", houseid);
	
	new count = 0, outdoorc, query[ 450 ]; //count of outdoor furniture
	//for each furniture object found in the database, put the data into array
	for(new i = 0; i < cache_num_rows(); i++)
	{
		FurnitureInfo[houseid][i][f_ID] = cache_get_field_content_int(i, "ID");
		FurnitureInfo[houseid][i][f_HouseID] = houseid;
		FurnitureInfo[houseid][i][f_SQLHouseID] = cache_get_field_content_int(i, "HouseID"); //sql id
		FurnitureInfo[houseid][i][f_ModelID] = cache_get_field_content_int(i, "ModelID");
		cache_get_field_content(i, "ModelName", FurnitureInfo[houseid][i][f_ModelName], sqlGameConnection, 50);
		FurnitureInfo[houseid][i][f_Spawned] = cache_get_field_content_int(i, "Spawned");
		FurnitureInfo[houseid][i][f_Outdoor] = cache_get_field_content_int(i, "Outdoor");
		if( FurnitureInfo[houseid][i][f_Outdoor] ) {
			if( outdoorc == MAX_OUTDOOR_FURNITURE ) {
				continue;
			}
			outdoorc++;
		}
		// FurnitureInfo[houseid][i][f_VirtualWorld] = cache_get_field_content_int(i, "VirtualWorld");
		FurnitureInfo[houseid][i][f_VirtualWorld] = HouseInfo[houseid][hWorld];
		FurnitureInfo[houseid][i][f_X] = cache_get_field_content_float(i, "X");
		FurnitureInfo[houseid][i][f_Y] = cache_get_field_content_float(i, "Y");
		FurnitureInfo[houseid][i][f_Z] = cache_get_field_content_float(i, "Z");
		FurnitureInfo[houseid][i][f_RX] = cache_get_field_content_float(i, "rX");
		FurnitureInfo[houseid][i][f_RY] = cache_get_field_content_float(i, "rY");
		FurnitureInfo[houseid][i][f_RZ] = cache_get_field_content_float(i, "rZ");
		
		if( FurnitureInfo[houseid][i][f_Spawned] ) {
			FurnitureInfo[houseid][i][f_ObjectID] = CreateDynamicObject(FurnitureInfo[houseid][i][f_ModelID], FurnitureInfo[houseid][i][f_X], FurnitureInfo[houseid][i][f_Y], FurnitureInfo[houseid][i][f_Z], FurnitureInfo[houseid][i][f_RX], FurnitureInfo[houseid][i][f_RY], FurnitureInfo[houseid][i][f_RZ], FurnitureInfo[houseid][i][f_VirtualWorld]);
		}
		HouseInfo[houseid][hFurnitureAmount]++;
		count++;
	}
	for(new id = 0; id < MAX_HOUSES; id++)
		{
			for(new i = 0; i < MAX_FURNITURE; i++) // loop through all furniture
			{
				if(!FurnitureInfo[id][i][f_Spawned]) // if it's a not spawned furniture in the player's house, create it it (unhide)
				{ 
					FurnitureInfo[id][i][f_ObjectID] = CreateDynamicObject(FurnitureInfo[id][i][f_ModelID], FurnitureInfo[id][i][f_X], FurnitureInfo[id][i][f_Y], FurnitureInfo[id][i][f_Z], FurnitureInfo[id][i][f_RX], FurnitureInfo[id][i][f_RY], FurnitureInfo[id][i][f_RZ], FurnitureInfo[id][i][f_VirtualWorld]);
					FurnitureInfo[id][i][f_Spawned] = 1;
					mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `furniture` SET `Spawned`=1 WHERE `houseid` = %d", HouseInfo[id][hSQLID]);
					mysql_pquery(sqlGameConnection, query);
				}
			} 
		}
	//printf("[Furniture] Successfully loaded %d furniture objects for houseid %d.", count, houseid);
	return 1;
}

stock CreateHouseFurniture(playerid, houseid, modelid, index) {
	
	//check for empty furniture slot in owners house
	new empty_slot = FindEmptyFurnitureSlot(houseid);
	
	if(empty_slot != -1) {
	
		//is it outdoor?
		new vw = GetPlayerVirtualWorld(playerid);
		if(vw) { //player is inside
			FurnitureInfo[houseid][empty_slot][f_VirtualWorld] = vw;
		} else {
			if(PlayerInfo[playerid][pDonateRank] < 3) {
				return SendClientMessage(playerid, COLOR_GREY, "You must be a Gold VIP to build outside your house.");
			}
			if( GetTotalFurnitureOutdoor( houseid ) >= MAX_OUTDOOR_FURNITURE ) {
				return SendClientMessage( playerid, COLOR_GREY, "You cannot have anymore furniture outside, the limit is 30." );
			}
			FurnitureInfo[houseid][empty_slot][f_Outdoor] = 1;
		}
		
		//spawn the object
		new Float:fPos[4];
		GetXYInFrontOfPlayer(playerid, fPos[0], fPos[1], fPos[2], fPos[3], 4);
		FurnitureInfo[houseid][empty_slot][f_ObjectID] = CreateDynamicObject(modelid, fPos[0], fPos[1], fPos[2], 0.0, 0.0, 0.0, FurnitureInfo[houseid][empty_slot][f_VirtualWorld] );
		EditDynamicObject(playerid, FurnitureInfo[houseid][empty_slot][f_ObjectID]);
		
		//update array
		FurnitureInfo[houseid][empty_slot][f_Spawned] = 1;
		FurnitureInfo[houseid][empty_slot][f_X] = fPos[0];
		FurnitureInfo[houseid][empty_slot][f_Y] = fPos[1];
		FurnitureInfo[houseid][empty_slot][f_Z] = fPos[2];
		
		FurnitureInfo[houseid][empty_slot][f_ModelID] = modelid;
		format(FurnitureInfo[houseid][empty_slot][f_ModelName], 64, "%s", FurnitureObjectList[index][fD_name]);
		FurnitureInfo[houseid][empty_slot][f_HouseID] = houseid;
		FurnitureInfo[houseid][empty_slot][f_SQLHouseID] = HouseInfo[houseid][hSQLID];
		

		
		//set editmode vars
		EditingFurnitureObjectID[playerid] = FurnitureInfo[houseid][empty_slot][f_ObjectID];
		EditingFurnitureHouseID[playerid] = houseid;
		EditingFurnitureID[playerid] = empty_slot;
		
		//purchase it
		GiveMoney(playerid, -FurnitureObjectList[index][fD_price]);
		TaxValue += FurnitureObjectList[index][fD_price]; // all money goes to government treasury
		format(str, sizeof(str), "* You have purchased a %s for $%d.", FurnitureObjectList[index][fD_name], FurnitureObjectList[index][fD_price]);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		
		//this tells the loadhouses function to load the furniture when there is a gmx
		if(!HouseInfo[houseid][hHasFurniture]) {
			HouseInfo[houseid][hHasFurniture] = 1;
			HouseInfo[houseid][hModified] = 1;
		}
			
		HouseInfo[houseid][hFurnitureAmount]++;
		
		//no more "hash", there could've been collisions, instead there's a simplier more effective method,
		new query[400];
		mysql_format(sqlGameConnection, query, sizeof(query), "INSERT INTO `furniture` (`Active`, `HouseID`, `ModelID`, `ModelName`, `VirtualWorld`, `Outdoor`, `Spawned`, `X`, `Y`, `Z`) VALUES(1, '%d', '%d', '%e', '%d', '%d', '%d', '%f', '%f', '%f')",
		FurnitureInfo[houseid][empty_slot][f_SQLHouseID], FurnitureInfo[houseid][empty_slot][f_ModelID], FurnitureInfo[houseid][empty_slot][f_ModelName], FurnitureInfo[houseid][empty_slot][f_VirtualWorld], FurnitureInfo[houseid][empty_slot][f_Outdoor], FurnitureInfo[houseid][empty_slot][f_Spawned], FurnitureInfo[houseid][empty_slot][f_X], FurnitureInfo[houseid][empty_slot][f_Y], FurnitureInfo[houseid][empty_slot][f_Z]);
		mysql_pquery(sqlGameConnection, query, "onCreateFurniture", "ii", houseid, empty_slot);
		
	}
	return 1;
}

forward onCreateFurniture(houseid, empty_slot);
public onCreateFurniture(houseid, empty_slot) {
	new query[200];
	//ORDER BY ID DESC LIMIT 1: gets the last "unique" ID in list, AKA last created furniture given houseID
	mysql_format(sqlGameConnection, query, sizeof(query), "SELECT `ID` FROM `furniture` WHERE `HouseID` = %d ORDER BY ID DESC LIMIT 1;", HouseInfo[houseid][hSQLID]);
	mysql_pquery(sqlGameConnection, query, "SetPlayerFurnitureID", "ii", houseid, empty_slot);
	return 1;
}

forward SetPlayerFurnitureID(houseid, empty_slot);
public SetPlayerFurnitureID(houseid, empty_slot) {
	FurnitureInfo[houseid][empty_slot][f_ID] = cache_get_field_content_int(0, "ID");
}

stock DeleteHouseFurniture(houseid, furnitureid) {
	//destroy the object
	if( IsValidDynamicObject( FurnitureInfo[houseid][furnitureid][f_ObjectID] ) ) {
		DestroyDynamicObjectEx(FurnitureInfo[houseid][furnitureid][f_ObjectID]);
	}
	
	//sql update
	new query[200];
	mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `furniture` SET `Active`=0 WHERE `ID`=%d", FurnitureInfo[houseid][furnitureid][f_ID]);
	mysql_pquery(sqlGameConnection, query);
	
	//clear info
	FurnitureInfo[houseid][furnitureid][f_ID] = 0;
	FurnitureInfo[houseid][furnitureid][f_HouseID] = 0;
	FurnitureInfo[houseid][furnitureid][f_SQLHouseID] = 0;
	FurnitureInfo[houseid][furnitureid][f_ModelID] = 0;
	format(FurnitureInfo[houseid][furnitureid][f_ModelName], 64, "%s", "Empty");
	FurnitureInfo[houseid][furnitureid][f_Spawned] = 0;
	FurnitureInfo[houseid][furnitureid][f_ObjectID] = 0;
	FurnitureInfo[houseid][furnitureid][f_Outdoor] = 0;
	FurnitureInfo[houseid][furnitureid][f_VirtualWorld] = 0;
	FurnitureInfo[houseid][furnitureid][f_X] = 0;
	FurnitureInfo[houseid][furnitureid][f_Y] = 0;
	FurnitureInfo[houseid][furnitureid][f_Z] = 0;
	FurnitureInfo[houseid][furnitureid][f_RX] = 0;
	FurnitureInfo[houseid][furnitureid][f_RY] = 0;
	FurnitureInfo[houseid][furnitureid][f_RZ] = 0;
	
	HouseInfo[houseid][hFurnitureAmount]--;
	
	if(HouseInfo[houseid][hFurnitureAmount] < 1) {
		HouseInfo[houseid][hHasFurniture] = 0;
		HouseInfo[houseid][hModified] = 1;
	}
	return 1;
}

stock ClearHouseFurniture(houseid) {
	for(new i = 0; i < MAX_FURNITURE; i++) {
		if(FurnitureInfo[houseid][i][f_ObjectID] && FurnitureInfo[houseid][i][f_Spawned]) {
			if(IsValidDynamicObject(FurnitureInfo[houseid][i][f_ObjectID])) {
				DestroyDynamicObjectEx(FurnitureInfo[houseid][i][f_ObjectID]);
			}
		}
		
		FurnitureInfo[houseid][i][f_ID] = 0;
		FurnitureInfo[houseid][i][f_HouseID] = 0;
		FurnitureInfo[houseid][i][f_SQLHouseID] = 0;
		FurnitureInfo[houseid][i][f_ModelID] = 0;
		format(FurnitureInfo[houseid][i][f_ModelName], 64, "%s", "Empty");
		FurnitureInfo[houseid][i][f_Spawned] = 0;
		FurnitureInfo[houseid][i][f_ObjectID] = 0;
		FurnitureInfo[houseid][i][f_Outdoor] = 0;
		FurnitureInfo[houseid][i][f_VirtualWorld] = 0;
		FurnitureInfo[houseid][i][f_X] = 0;
		FurnitureInfo[houseid][i][f_Y] = 0;
		FurnitureInfo[houseid][i][f_Z] = 0;
		FurnitureInfo[houseid][i][f_RX] = 0;
		FurnitureInfo[houseid][i][f_RY] = 0;
		FurnitureInfo[houseid][i][f_RZ] = 0;
	}
	
	new query[200];
	mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `furniture` SET `Active` = 0 WHERE `houseid` = %d", HouseInfo[houseid][hSQLID]);
	mysql_tquery(sqlGameConnection, query);
	return 1;
}

//commands
CMD:furniture( playerid, params[] ) {
	//if(PlayerInfo[playerid][pDev] < 1 && PlayerInfo[playerid][pAdmin] < 1) return SendClientMessage(playerid, COLOR_GREY, "You are not authorized to use this.");
	
	new houseid = PlayerInfo[playerid][pHouseID];
	if(houseid == 0) return SendClientMessage( playerid, COLOR_GRAD1, "You do not own a house.");
	if(HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID]) return SendClientMessage(playerid, COLOR_GREY, "You don't own this house.");
	
	if(IsPlayerInRangeOfPoint(playerid, 100.0, HouseInfo[houseid][hInsideX], HouseInfo[houseid][hInsideY], HouseInfo[houseid][hInsideZ]) && GetPlayerVirtualWorld(playerid) == HouseInfo[houseid][hSQLID]) { //inside
		ShowPlayerDialog(playerid, DIALOG_FURNITURE_MAIN, DIALOG_STYLE_LIST, "Furniture", "Buy furniture\nEdit furniture", "Select", "Cancel");
		return 1;
	} else if(IsPlayerInRangeOfPoint(playerid, 60.0, HouseInfo[houseid][hOutsideX], HouseInfo[houseid][hOutsideY], HouseInfo[houseid][hOutsideZ])) { //outside
		if(PlayerInfo[playerid][pDonateRank] < 3) return SendClientMessage(playerid, COLOR_GREY, "You must be a Gold/or Diamond VIP to build outside your house.");
		
		ShowPlayerDialog(playerid, DIALOG_FURNITURE_MAIN, DIALOG_STYLE_LIST, "Furniture", "Buy furniture\nEdit furniture", "Select", "Cancel");
		return 1;
	} else {
		SendClientMessage(playerid, COLOR_GRAD1, "You are not inside your house or within 60m of the house.");
	}
	
	return 1;
}

CMD:furnitureinfo(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 1 || PlayerInfo[playerid][pBizMod] == 1)
	{
		for(new i = 0; i < MAX_HOUSES; i++)
		{
			for(new f = 0; f < MAX_FURNITURE; f++) {
				if(FurnitureInfo[i][f][f_ObjectID]) {
					if(FurnitureInfo[i][f][f_Spawned]) {
						if(IsPlayerInRangeOfPoint(playerid, 2.5, FurnitureInfo[i][f][f_X], FurnitureInfo[i][f][f_Y], FurnitureInfo[i][f][f_Z]))
						{
							format(str, sizeof(str), "Furniture ID: %d | House ID: %d | SQL ID %d | | Model ID %d | Model Name: %s.", f, FurnitureInfo[i][f][f_HouseID], FurnitureInfo[i][f][f_ID], FurnitureInfo[i][f][f_ModelID], FurnitureInfo[i][f][f_ModelName]);
							SendClientMessage(playerid, COLOR_GREY, str);
							format(str, sizeof(str), "Object ID: %d | Virtual World: %d | ObjectVW: %d.", FurnitureInfo[i][f][f_ObjectID], FurnitureInfo[i][f][f_VirtualWorld], Streamer_GetIntData(STREAMER_TYPE_OBJECT, FurnitureInfo[i][f][f_ObjectID], E_STREAMER_WORLD_ID));
							SendClientMessage(playerid, COLOR_GREY, str);
						}
					}
				}
			}	
		}
	}
	return 1;
}

CMD:deletefurniture(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 4 || PlayerInfo[playerid][pBizMod] == 1 || PlayerInfo[playerid][pDev] >= 3 )
	{
		new houseid,furnitureid;
		if(sscanf(params, "dd", houseid, furnitureid)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /deletefurniture [houseid] [furnitureid]");
		if(!FurnitureInfo[houseid][furnitureid][f_ObjectID]) return SendClientMessage(playerid, COLOR_GREY, "There is no furniture object in that slot.");
		
		format(str, sizeof(str), "* You have deleted the %s.", FurnitureInfo[houseid][furnitureid][f_ModelName]);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		
		DeleteHouseFurniture(houseid, furnitureid);
	}
	else
	{
		AdmErrorMsg;
	}
	return 1;
}

CMD:resethousefurniture(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] >= 4 || PlayerInfo[playerid][pBizMod] == 1)
	{
		new houseid;
		if(sscanf(params, "d", houseid)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /resethousefurniture [houseid]");
		
		ClearHouseFurniture(houseid);
		
		HouseInfo[houseid][hFurnitureAmount] = 0;
		HouseInfo[houseid][hHasFurniture] = 0;
		HouseInfo[houseid][hModified] = 1;
		format(str, sizeof(str), "You have reset houseid %d's furniture.", houseid);
		SendClientMessage(playerid, COLOR_GRAD1, str);
	}
	else
	{
		AdmErrorMsg;
	}
	return 1;
}

CMD:fdb(playerid, params[])
{
	new houseid;
	if(sscanf(params, "d", houseid)) return SendClientMessage(playerid, COLOR_GREY, "{00BFFF}Usage:{FFFFFF} /fdb [houseid]");
	
	for(new f = 0; f < 5; f++) {
		
		format(str, sizeof(str), "Furniture ID: %d | House ID: %d | SQL ID %d | | Model ID %d | Model Name: %s.", f, FurnitureInfo[houseid][f][f_HouseID], FurnitureInfo[houseid][f][f_ID], FurnitureInfo[houseid][f][f_ModelID], FurnitureInfo[houseid][f][f_ModelName]);
		SendClientMessage(playerid, COLOR_GREY, str);
		format(str, sizeof(str), "Spawned: %d | Object ID: %d | Outdoor:%d | Virtual World: %d.", FurnitureInfo[houseid][f][f_Spawned], FurnitureInfo[houseid][f][f_ObjectID], FurnitureInfo[houseid][f][f_Outdoor], FurnitureInfo[houseid][f][f_VirtualWorld]);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, str);
		format(str, sizeof(str), "f_X: %f | f_Y: %f | f_Z: %f", FurnitureInfo[houseid][f][f_X], FurnitureInfo[houseid][f][f_Y], FurnitureInfo[houseid][f][f_Z]);
		SendClientMessage(playerid, COLOR_WHITE, str);
	}
	return 1;
}

CMD:deleteallfurniture(playerid, params[])
{
	new houseid = PlayerInfo[playerid][pHouseID];
	if(houseid == 0) return SendClientMessage( playerid, COLOR_GRAD1, "You do not own a house.");
	if(HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID]) return SendClientMessage(playerid, COLOR_GREY, "You don't own this house.");
	if(!IsPlayerInRangeOfPoint(playerid, 100.0, HouseInfo[houseid][hInsideX], HouseInfo[houseid][hInsideY], HouseInfo[houseid][hInsideZ]) && GetPlayerVirtualWorld(playerid) != HouseInfo[houseid][hSQLID]) return SendClientMessage(playerid, COLOR_GRAD1, "You are not inside your house.");  //inside

	ClearHouseFurniture(houseid);	
	SendClientMessage(playerid, COLOR_GREY, "You have deleted all of your house's furniture.");
	return 1;
}


CMD:togglefurniture(playerid, params[])
{
	new houseid = PlayerInfo[playerid][pHouseID];
	if(houseid == 0) return SendClientMessage( playerid, COLOR_GRAD1, "You do not own a house.");
	if(HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID]) return SendClientMessage(playerid, COLOR_GREY, "You don't own this house.");
	new action[4];
	if(sscanf(params, "s[4]", action)) return SendClientMessage(playerid, -1, "{00BFFF}Usage:{FFFFFF} /togglefurniture [on/off]");
	if(!strcmp(action, "off", true)) // hide
	{
		for(new i = 0; i < MAX_FURNITURE; i++) // loop through all furniture
		{
			if(FurnitureInfo[houseid][i][f_Spawned]) // if it's a spawned furniture in the player's house, destroy it (hide)
			{ 
				FurnitureInfo[houseid][i][f_Spawned] = 0;
				if(FurnitureInfo[houseid][i][f_ObjectID]) { DestroyDynamicObjectEx(FurnitureInfo[houseid][i][f_ObjectID]); }
				FurnitureInfo[houseid][i][f_ObjectID] = 0;
			}
		} 

		//sql update
		new query[200];
		mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `furniture` SET `Spawned`=0 WHERE `houseid` = %d", HouseInfo[houseid][hSQLID]);
		mysql_pquery(sqlGameConnection, query);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have hidden all of your furniture.");
	}
	else if(!strcmp(action, "on", true)) //unhide
	{
		for(new i = 0; i < MAX_FURNITURE; i++) // loop through all furniture
		{
			if(!FurnitureInfo[houseid][i][f_Spawned]) // if it's a not spawned furniture in the player's house, create it it (unhide)
			{ 
				FurnitureInfo[houseid][i][f_ObjectID] = CreateDynamicObject(FurnitureInfo[houseid][i][f_ModelID], FurnitureInfo[houseid][i][f_X], FurnitureInfo[houseid][i][f_Y], FurnitureInfo[houseid][i][f_Z], FurnitureInfo[houseid][i][f_RX], FurnitureInfo[houseid][i][f_RY], FurnitureInfo[houseid][i][f_RZ]);
				FurnitureInfo[houseid][i][f_Spawned] = 1;
				Streamer_Update(playerid);
			}
		} 

		//sql update
		new query[200];
		mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `furniture` SET `Spawned`=1 WHERE `houseid` = %d", HouseInfo[houseid][hSQLID]);
		mysql_pquery(sqlGameConnection, query);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have unhidden all of your furniture.");	
	}		
	return 1;
}

/*CMD:showallfurnitures( playerid )
{
	new query[ 800 ];
	if( PlayerInfo[ playerid ] [ pAdmin ] < 1337 ) SendClientMessage( playerid, COLOR_GREY, "You're not authorized to use this command!" );
	else
	{
		for(new id = 0; id < MAX_HOUSES; id++)
		{
			for(new i = 0; i < MAX_FURNITURE; i++) // loop through all furniture
			{
				for(new ie = 0; i < MAX_PLAYERS; ie++) // loop through all furniture
				{
					if(!FurnitureInfo[id][i][f_Spawned]) // if it's a not spawned furniture in the player's house, create it it (unhide)
					{ 
						FurnitureInfo[id][i][f_ObjectID] = CreateDynamicObject(FurnitureInfo[id][i][f_ModelID], FurnitureInfo[id][i][f_X], FurnitureInfo[id][i][f_Y], FurnitureInfo[id][i][f_Z], FurnitureInfo[id][i][f_RX], FurnitureInfo[id][i][f_RY], FurnitureInfo[id][i][f_RZ], FurnitureInfo[id][i][f_VirtualWorld]);
						FurnitureInfo[id][i][f_Spawned] = 1;
						Streamer_Update(ie);
						mysql_format(sqlGameConnection, query, sizeof(query), "UPDATE `furniture` SET `Spawned`=1 WHERE `houseid` = %d", HouseInfo[id][hSQLID]);
						mysql_pquery(sqlGameConnection, query);
					}
				}
			} 
		}
		SendClientMessage( playerid, COLOR_GREY, "All house furnitures successfully spawned!" );
	}
	return 1;
}*/