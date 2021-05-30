#include <a_samp>

//Dialogs
#define DIALOG_EDIT_ON 6001
#define DIALOG_STATIONS 6003
#define DIALOG_SARP_STATIONS_LIST1 6004
#define DIALOG_SARP_STATIONS_LIST2 6005
#define DIALOG_SARP_STATIONS_CUSTOM 6006
#define DIALOG_SARP_STATIONS_GENRES 6007
#define DIALOG_CAR_MENU_MAIN 6008 //vehicle radio

#define EDIT_RADIO_NONE (0)
#define EDIT_RADIO_BOOMBOX (1)
#define EDIT_RADIO_VEHICLE (2)

//simple vars
new editingBoomboxID[MAX_PLAYERS];
new streamingFromBoombox[MAX_PLAYERS];
new streamingFromVehicle[MAX_PLAYERS];

new EDITING_TYPE[MAX_PLAYERS];
new listItemGenre[MAX_PLAYERS];


//Official (dynamic) radios
enum rInfo
{
	taken,
	genreName[32],
	channelName[128],
	ownerName[32],
	url[128],
	online
};
new radioInfo[10][rInfo];

//Static radios
enum radioStationsInfo 
{
	//genre[32],
	radioname[64],
	radiourl[128],
	listeners
}
new radioStations[220][radioStationsInfo] = {
// Alternative (0-9)

{"92.5 The River","http://yp.shoutcast.com/sbin/tunein-station.pls?id=190126"},
{".113 FM Alt Nation","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1712686"},
{"102.3/105.7 The River","http://yp.shoutcast.com/sbin/tunein-station.pls?id=164322"},
{"PYG Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=830430"},
{"1 Pure Alternative","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1188685"},
{"CampusFM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=567161"},
{"Radio Javan","http://yp.shoutcast.com/sbin/tunein-station.pls?id=52684"},
{"D-ONE","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1503687"},
{"WPCD 88.7","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1505077"},
{"Radio Asheghaneh (GLWiZ)","http://yp.shoutcast.com/sbin/tunein-station.pls?id=381703"},

// Blues 10-19)
{"1.FM - Blues (www.1.fm)","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1709502"},
{"Velvet 96.8","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1459248"},
{"Aardvark Blues FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=621720"},
{"Power FM 987","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1529454"},
{"Blues Rock Legends","http://yp.shoutcast.com/sbin/tunein-station.pls?id=99210029"},
{"AddictedToRadio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1491163"},
{"XRDS.fm","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1552292"},
{"Factor 96.1","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1754379"},
{"All Memphis Music","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1083195"},
{"Legacy 90.1","http://yp.shoutcast.com/sbin/tunein-station.pls?id=621169"},

// Classical (20-29)
{"Venice Classic Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=22146"},
{"Classical WETA 90.9","http://yp.shoutcast.com/sbin/tunein-station.pls?id=103145"},
{"Calm Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1251578"},
{"Calm Radio - Solo Piano","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1219771"},
{"AbacusFM Mozart","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1083969"},
{"AbacusFM Classical","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1322506"},
{"Audiophile Classical","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1423129"},
{"1000 Classical Hits","http://yp.shoutcast.com/sbin/tunein-station.pls?id=99180855"},
{"Musiq 3","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1440583"},
{"Radio City - Hindi Classics","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1742211"},

// Country (30-39)
{"MGT Sertaneja","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1603772"},
{"New Country 93","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1524300"},
{"Radio Lautaru","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1501881"},
{"CJSE FM89 Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1560626"},
{"Discofox-FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=122187"},
{"Atlantis Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=99237349"},
{"Top Tonic Western","http://yp.shoutcast.com/sbin/tunein-station.pls?id=977015"},
{"Dixie Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1702200"},
{"Highway 65 Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=99319424"},
{"The Bull","http://yp.shoutcast.com/sbin/tunein-station.pls?id=706496"},

// Decades (40-49)
{"DJ Shocker","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1177488"},
{"Golden Sixties","http://yp.shoutcast.com/sbin/tunein-station.pls?id=639191"},
{"Radio Star Music","http://yp.shoutcast.com/sbin/tunein-station.pls?id=99181797"},
{"80's Planet","http://yp.shoutcast.com/sbin/tunein-station.pls?id=613184"},
{"DeejayFox Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=99181938"},
{"Radio-Happy.EU","http://yp.shoutcast.com/sbin/tunein-station.pls?id=239984"},
{"Old Hits 60, 70, 80, 90","http://yp.shoutcast.com/sbin/tunein-station.pls?id=99182061"},
{"Webradio Imaginland","http://yp.shoutcast.com/sbin/tunein-station.pls?id=99180891"},
{"Radio Star Music","http://yp.shoutcast.com/sbin/tunein-station.pls?id=99181797"},
{"HitRadio Studio 24","http://yp.shoutcast.com/sbin/tunein-station.pls?id=99180858"},

// Easy Listening (50-59)
{"COOLfarenheit 93 (1)","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1453011"},
{"KLUX 89.6HD","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1482287"},
{"Sukson Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=193157"},
{"101 Smooth Jazz","http://yp.shoutcast.com/sbin/tunein-station.pls?id=917832"},
{"Instrumental Hits","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1543832"},
{"1.FM - Sax4Love","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1718173"},
{"Crystal Radio Beautiful Music","http://yp.shoutcast.com/sbin/tunein-station.pls?id=329196"},
{"MusicArtClub","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1470448"},
{"The Oasis - Easy Listening","http://yp.shoutcast.com/sbin/tunein-station.pls?id=99181020"},
{"1.FM Deep House","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1647608"},

// Electronic (60-69)
{"ETN.FM 1","http://ch1relay1.etn.fm:8130/listen.pls?sid=1"},
{"ETN.FM 2","http://ch1relay1.etn.fm:8000/listen.pls?sid=6"},
{"Pure FM (RTBF)","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1566708"},
{"Blue Marlin Ibiza Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1131869"},
{"IBIZA SONICA","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1192927"},
{"MxRadio By PulsRadio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1280173"},
{"PlusFM Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1494548"},
{"Electro Madness","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1648446"},
{"We House Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1734407"},
{"Replica Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1364991"},

// Folk (70-79)
{"TelstarFM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1750343"},
{"COOL Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1437438"},
{"Piratenhits.FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1634183"},
{"Sahib Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1366941"},
{"Radio Sljivovica Kraljevo","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1468209"},
{"88OR","http://yp.shoutcast.com/sbin/tunein-station.pls?id=330023"},
{"MASQUELOUR RADIO","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1482310"},
{"Radio Schlagertempel","http://yp.shoutcast.com/sbin/tunein-station.pls?id=335073"},
{"Limburgia","http://yp.shoutcast.com/sbin/tunein-station.pls?id=521849"},
{"Acoustic Outpost","http://yp.shoutcast.com/sbin/tunein-station.pls?id=985347"},

// Inspirational (80-89)
{"Volna Schastiya","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1333839"},
{"Volna Schastiya 2","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1275642"},
{"The Voice of Pilgrim","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1087642"},
{"Radio Vocea Sperantei 2","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1668887"},
{"Tropicalisima.fm","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1404396"},
{"LightDigital","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1680233"},
{"Z WAVE Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1197728"},
{"Radio Flavour","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1236154"},
{"Southern Gospel","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1531188"},
{"Quran Coran","http://yp.shoutcast.com/sbin/tunein-station.pls?id=687035"},

// International (90-99)
{"Calm Radio - Solo Guitar","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1249229"},
{"BTC Tamil","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1455917"},
{"New World Buzz","http://yp.shoutcast.com/sbin/tunein-station.pls?id=876868"},
{"Radio City - Tamil Gold","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1576369"},
{"Hot Digital Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1562515"},
{"Radio UAEM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=236965"},
{"Sounds of Shalom","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1541545"},
{"Muzaiko","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1266332"},
{"RadioMusik4Ever","http://yp.shoutcast.com/sbin/tunein-station.pls?id=486185"},
{"Omega FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1726082"},

// Jazz (100-109)
{"ABC Lounge","http://yp.shoutcast.com/sbin/tunein-station.pls?id=709809"},
{"Audiophile Jazz","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1459011"},
{"Calm Radio Jazz Piano","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1096950"},
{"Big Band","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1681779"},
{"Play Misty For Me","http://yp.shoutcast.com/sbin/tunein-station.pls?id=99179955"},
{"A.1 SMOOTHLY JAZZ","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1228869"},
{"Zappa Stream Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=2369878"},
{"Very Vintage Jazz","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1606299"},
{"TheBocX","http://yp.shoutcast.com/sbin/tunein-station.pls?id=99180984"},
{"Swingtime Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=921741"},

// Latin (110-119)
{"La Ranchera 106.1 FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1610088"},
{"Calm Radio Latin Pop Charts","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1049123"},
{"1.FM - Absolute Pop Latino","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1717957"},
{"Emisora Galaxia FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1723326"},
{"Tropical Songs","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1054346"},
{"Stereo Beso 94.5FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1322585"},
{"Super K FM AAC","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1758476"},
{"Super K FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1587873"},
{"Tropicalisima Bachata","http://yp.shoutcast.com/sbin/tunein-station.pls?id=504454"},
{"Amor FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1391432"},

// Metal (120-129)
{"MetalRock.FM CSNX-8766","http://yp.shoutcast.com/sbin/tunein-station.pls?id=826801"},
{"Metal Nation Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1404695"},
{"Classical Metal Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1633217"},
{"Metal Devastation Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=398592"},
{"Metal Tavern Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1661976"},
{"Tyrant Metal Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=375078"},
{"Fusion109 Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=407834"},
{"Anarchy Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1298974"},
{"ROCKYOU.fm","http://yp.shoutcast.com/sbin/tunein-station.pls?id=175150"},
{"Hard Rock Radio Live","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1243130"},

// Misc (130-139)
{"S Stream","http://yp.shoutcast.com/sbin/tunein-station.pls?id=417115"},
{"Ujala Radio NL","http://yp.shoutcast.com/sbin/tunein-station.pls?id=277522"},
{"Letio Tonga","http://yp.shoutcast.com/sbin/tunein-station.pls?id=394689"},
{"Radio Crazy","http://yp.shoutcast.com/sbin/tunein-station.pls?id=70532"},
{"Power Station Stream","http://yp.shoutcast.com/sbin/tunein-station.pls?id=662469"},
{"Spider FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=847927"},
{"4CRB","http://yp.shoutcast.com/sbin/tunein-station.pls?id=713375"},
{"Radio Crazy","http://yp.shoutcast.com/sbin/tunein-station.pls?id=70532"},
{"Sinatras Best","http://yp.shoutcast.com/sbin/tunein-station.pls?id=67233"},
{"Magnetica 107.1FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=13374"},

// New Age (140-149)
{"CALMRADIO.COM - SPA","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1247247"},
{"CALMRADIO.COM - POSITIVITY","http://yp.shoutcast.com/sbin/tunein-station.pls?id=806605"},
{"CALMRADIO.COM - SLEEP","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1216562"},
{"CALMRADIO.COM - ZEN","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1259077"},
{"Ambi Nature Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1074331"},
{"CALM RADIO - REIKI","http://yp.shoutcast.com/sbin/tunein-station.pls?id=967175"},
{"CALMRADIO.COM - HEALING","http://yp.shoutcast.com/sbin/tunein-station.pls?id=965970"},
{"CALMRADIO.COM - BALANCE","http://yp.shoutcast.com/sbin/tunein-station.pls?id=937502"},
{"CALMRADIO.COM - ELITE","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1250378"},
{"DREAM 90.6","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1385465"},

// Pop (150-159)
{"Alpha FM 101,7","http://yp.shoutcast.com/sbin/tunein-station.pls?id=99226864"},
{"ANTENA1 - 94 7 FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=38486"},
{"JARADIO JAMAICA","http://yp.shoutcast.com/sbin/tunein-station.pls?id=583000"},
{"Fly 104","http://yp.shoutcast.com/sbin/tunein-station.pls?id=959305"},
{"Fungfungfung Internet Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1587673"},
{"Globus Guld","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1201658"},
{"Cheesy FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=251835"},
{"96 FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1466271"},
{"Radio Dukagjini","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1053227"},
{"89 Rainbow","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1592967"},

// Public Radio (160-169)
{"Bestfriend FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1129845"},
{"Radio Capital","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1479492"},
{"Radio Sines","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1352403"},
{"RTW","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1676539"},
{"Simulator Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=561186"},
{"Pirate 91 FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=182881"},
{"3WBC Box Hill Melbourne","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1625657"},
{"Hayes FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1348007"},
{"Dimension FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1598968"},
{"SestoNetwork","http://yp.shoutcast.com/sbin/tunein-station.pls?id=636839"},

// R&B/Urban (170-179)
{"Omnia.7","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1455278"},
{"The RnB Channel","http://yp.shoutcast.com/sbin/tunein-station.pls?id=96672"},
{"BoomSVG106","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1106313"},
{"PopRadio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=631523"},
{"141 Radio Hits","http://yp.shoutcast.com/sbin/tunein-station.pls?id=118350"},
{"A_A R&B","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1607287"},
{"Deluxe FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1356278"},
{"365 Soul","http://yp.shoutcast.com/sbin/tunein-station.pls?id=564815"},
{"The Moroccan Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=572596"},
{"ABCD Gold Hits","http://yp.shoutcast.com/sbin/tunein-station.pls?id=643594"},

// Rap (180-189)
{".113FM Power","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1385953"},
{"AAZO Radio Rap","http://yp.shoutcast.com/sbin/tunein-station.pls?id=199694"},
{"A_A Rap","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1654024"},
{"Radio HIPHOP","http://yp.shoutcast.com/sbin/tunein-station.pls?id=583795"},
{"Rap Trap","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1587397"},
{"Rap On Air Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1153440"},
{"Radio104.4 FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=99214829"},
{"Wicked Intent Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1127878"},
{"Power104.4 FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=99214829"},
{"DSF Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1646414"},

// Reggae (190-199)
{"Party Vibe Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=939852"},
{"Radio Verbier Reggae","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1500964"},
{"Reggae Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=390979"},
{"Reggae141.com","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1654897"},
{"GTriddim Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1011022"},
{"Roots Yard","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1347255"},
{"CALM RADIO - REGGAE","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1172449"},
{"Wee FM Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1681005"},
{"SeneEthiopia","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1388412"},
{"Radio Guyana","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1337084"},

// Rock (200-209)
{"Radio Kampus 97.1 FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1693740"},
{"Rock101 Segunda Odisea","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1684232"},
{"Rockin Planet Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=879504"},
{"Top Europa","http://yp.shoutcast.com/sbin/tunein-station.pls?id=99182079"},
{"Rock Radio Beograd","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1375764"},
{"Zenith Classic Rock","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1549124"},
{"All Rock FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1573388"},
{"RockNet Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1535039"},
{"Codigo.FM","http://yp.shoutcast.com/sbin/tunein-station.pls?id=32530"},
{"Definte Rock","http://yp.shoutcast.com/sbin/tunein-station.pls?id=256959"},

// Talk (210-219)
{"Alex Jones - Infowars.com","http://www.infowars.com/stream.pls"},
{"The Ultimate Art Bell","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1591414"},
{"Roswell UFO Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=65720"},
{"Blank Talk Radio Network","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1061082"},
{"Skynet","http://yp.shoutcast.com/sbin/tunein-station.pls?id=729116"},
{"Mastering Bitcoin Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1012475"},
{"Indianapolis Metro Police Dept","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1414078"},
{"Real News with David Knight","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1581743"},
{"Madhouse Prank Call Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1464642"},
{"British Comedy Radio","http://yp.shoutcast.com/sbin/tunein-station.pls?id=1510991"}
};


//vehicle Stations
enum vInfo
{
	radioURL[128],
	radioStation
};
new VehicleInfo[MAX_VEHICLES][vInfo];

//Boomboxes
enum bbInfo
{
	placedBoombox,
	boomboxTurnedOn,
	locked,
    Float:bpos[4],
	bbInt,
	bbVW,
	BoomboxURL[128],
	bbObjectID,
	BoomboxWorld,
	Text3D:bbLabelID
};
new BoomboxInfo[MAX_PLAYERS][bbInfo];

LoadRadios()
{
    new arrCoords[6][128];
    new strFromFile2[512];
    new File: file = fopen("radios.cfg", io_read);
    if(file)
    {
        new idx;
        while (idx < sizeof(radioInfo))
        {
            fread(file, strFromFile2);
            split(strFromFile2, arrCoords, '|');
            radioInfo[idx][taken] = strvalEx(arrCoords[0]);
            strmid(radioInfo[idx][genreName], arrCoords[1], 0, strlen(arrCoords[1]), 255);
            strmid(radioInfo[idx][channelName], arrCoords[2], 0, strlen(arrCoords[2]), 255);
            strmid(radioInfo[idx][ownerName], arrCoords[3], 0, strlen(arrCoords[3]), 255);
            strmid(radioInfo[idx][url], arrCoords[4], 0, strlen(arrCoords[4]), 255);
			radioInfo[idx][online] = strvalEx(arrCoords[5]);
            idx++;
        }
        fclose(file);
	}
	return 1;
}

SaveRadios()
{
    new idx;
	new File:file;
	file = fopen("radios.cfg", io_write);
	fclose(file);
	file = fopen("radios.cfg", io_append);
    while (idx < sizeof(radioInfo))
    {
        new coordsstring[512];
        format(coordsstring, sizeof(coordsstring), "%d|%s|%s|%s|%s|%d\n",
        radioInfo[idx][taken],
        radioInfo[idx][genreName],
        radioInfo[idx][channelName],
        radioInfo[idx][ownerName],
        radioInfo[idx][url],
		radioInfo[idx][online]);
		fwrite(file, coordsstring);
		idx++;
    }
    fclose(file);
    return 1;
}

stock clearRadioInfo(number)
{
	new string[128];
    radioInfo[number][taken] = 0;
	
	format(string, sizeof(string), "Radio %d", number+1);
    strmid(radioInfo[number][genreName], string, 0, strlen(string), 255);
	format(string, sizeof(string), "Reserved");
    strmid(radioInfo[number][channelName], string, 0, strlen(string), 255);
	format(string, sizeof(string), "Nobody");
    strmid(radioInfo[number][ownerName], string, 0, strlen(string), 255);
	format(string, sizeof(string), "None");
	strmid(radioInfo[number][url], string, 0, strlen(string), 255);
	
	format(string, sizeof(string), "None");
	radioInfo[number][online] = 0;
    SaveRadios();
    return 1;
}

stock displayBoomboxEditDialog(playerid) {
	if(editingBoomboxID[playerid] != -1) {
		if(BoomboxInfo[editingBoomboxID[playerid]][boomboxTurnedOn] != 0) {
			ShowPlayerDialog(playerid, DIALOG_EDIT_ON, DIALOG_STYLE_LIST, "Edit Boombox","Turn Off\nChange Station\nLock\nPickup","Select","Cancel");
		} else {
			ShowPlayerDialog(playerid, DIALOG_EDIT_ON, DIALOG_STYLE_LIST, "Edit Boombox","Turn On\nChange Station\nLock\nPickup","Select","Cancel");
		}
	}
}

stock isPlayerStreaming(playerid) {
	if(streamingFromBoombox[playerid] == -1 && streamingFromVehicle[playerid] == -1) {
		return 0;
	}
	return 1; //player streaming
}