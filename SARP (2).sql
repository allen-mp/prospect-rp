-- phpMyAdmin SQL Dump
-- version 4.5.4.1deb2ubuntu2.1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Jun 14, 2019 at 01:22 PM
-- Server version: 5.7.26-0ubuntu0.16.04.1
-- PHP Version: 7.0.33-0ubuntu0.16.04.5

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `zmatejparanos0`
--

-- --------------------------------------------------------

--
-- Table structure for table `activitylog`
--

CREATE TABLE `activitylog` (
  `ID` int(11) UNSIGNED NOT NULL,
  `UserID` int(11) UNSIGNED NOT NULL,
  `Minutes` int(11) UNSIGNED NOT NULL,
  `Date` date NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `atm`
--

CREATE TABLE `atm` (
  `ID` int(11) NOT NULL,
  `atm_X` float NOT NULL,
  `atm_Y` float NOT NULL,
  `atm_Z` float NOT NULL,
  `atm_fX` float NOT NULL,
  `atm_fY` float NOT NULL,
  `atm_fZ` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `banlist`
--

CREATE TABLE `banlist` (
  `ID` int(11) NOT NULL,
  `Name` varchar(32) NOT NULL DEFAULT 'Bugged_Probably',
  `Key_` varchar(64) NOT NULL DEFAULT 'Bugged_Probably',
  `IP` varchar(32) NOT NULL DEFAULT '001.110.001.110',
  `Reason` varchar(255) NOT NULL DEFAULT 'Bugged Reason?',
  `BannedBy` varchar(32) NOT NULL DEFAULT 'Bugged_Ban',
  `Date` date DEFAULT NULL,
  `Expires` date DEFAULT NULL,
  `Banned` tinyint(1) DEFAULT NULL,
  `userID` int(11) UNSIGNED DEFAULT NULL,
  `Serial` varchar(128) NOT NULL DEFAULT '01010101010101010101001011832756752158962865168826295586256518612685126985581892658652916512496892168521598612986521569852196852'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `bans`
--

CREATE TABLE `bans` (
  `username` varchar(32) NOT NULL,
  `banner` varchar(32) NOT NULL,
  `reason` varchar(255) NOT NULL,
  `ip` varchar(32) NOT NULL,
  `Serial` varchar(128) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `charges`
--

CREATE TABLE `charges` (
  `ID` int(11) NOT NULL,
  `Type` int(3) NOT NULL,
  `Time` int(3) NOT NULL,
  `Name` varchar(70) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `crimes`
--

CREATE TABLE `crimes` (
  `ID` int(11) NOT NULL,
  `Name` varchar(24) NOT NULL,
  `Officer` varchar(24) NOT NULL,
  `Crime` varchar(128) NOT NULL,
  `Active` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `deaths`
--

CREATE TABLE `deaths` (
  `ID` int(11) NOT NULL,
  `KilledName` varchar(24) NOT NULL,
  `KillerName` varchar(24) NOT NULL,
  `Gun1` int(11) NOT NULL,
  `Gun2` int(11) NOT NULL,
  `Gun3` int(11) NOT NULL,
  `Gun4` int(11) NOT NULL,
  `Gun5` int(11) NOT NULL,
  `Gun6` int(11) NOT NULL,
  `Gun7` int(11) NOT NULL,
  `Gun8` int(11) NOT NULL,
  `Gun9` int(11) NOT NULL,
  `Gun10` int(11) NOT NULL,
  `Gun11` int(11) NOT NULL,
  `Gun12` int(11) NOT NULL,
  `Date` varchar(11) NOT NULL,
  `KilledX` float NOT NULL,
  `KilledY` float NOT NULL,
  `KilledZ` float NOT NULL,
  `KillerX` float NOT NULL,
  `KillerY` float NOT NULL,
  `KillerZ` float NOT NULL,
  `Refunded` int(2) DEFAULT NULL,
  `KilledSQL` int(11) NOT NULL,
  `KillerSQL` int(11) NOT NULL,
  `KillerWeapon` varchar(24) NOT NULL,
  `RefundedBy` varchar(24) NOT NULL,
  `Gun0` int(11) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `divisions`
--

CREATE TABLE `divisions` (
  `ID` int(11) NOT NULL,
  `Faction` int(11) NOT NULL,
  `Leader` int(11) NOT NULL,
  `Name` varchar(24) NOT NULL,
  `Motd` varchar(128) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `donatedgroups`
--

CREATE TABLE `donatedgroups` (
  `ID` int(11) NOT NULL,
  `GroupOwner` varchar(69) NOT NULL,
  `GroupMaxRanks` int(11) NOT NULL,
  `GroupName` varchar(150) NOT NULL,
  `Rank1Active` int(11) NOT NULL DEFAULT '1',
  `RankName1` varchar(150) NOT NULL DEFAULT 'None',
  `Rank2Active` int(11) NOT NULL DEFAULT '1',
  `RankName2` varchar(150) NOT NULL DEFAULT 'None',
  `Rank3Active` int(11) NOT NULL DEFAULT '1',
  `RankName3` varchar(150) NOT NULL DEFAULT 'None',
  `Rank4Active` int(11) NOT NULL DEFAULT '1',
  `RankName4` varchar(150) NOT NULL DEFAULT 'None',
  `Rank5Active` int(11) NOT NULL DEFAULT '1',
  `RankName5` varchar(150) NOT NULL DEFAULT 'None',
  `Rank6Active` int(11) NOT NULL DEFAULT '1',
  `RankName6` varchar(150) NOT NULL DEFAULT 'None',
  `Rank7Active` int(11) NOT NULL DEFAULT '1',
  `RankName7` varchar(150) NOT NULL DEFAULT 'None',
  `Rank8Active` int(11) NOT NULL DEFAULT '1',
  `RankName8` varchar(150) NOT NULL DEFAULT 'None',
  `MaskPerm` int(11) NOT NULL DEFAULT '0',
  `SetUcNamePerm` int(11) NOT NULL,
  `DisguisePerm` int(11) NOT NULL DEFAULT '0',
  `CallerIDPerm` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `election`
--

CREATE TABLE `election` (
  `id` int(11) NOT NULL,
  `voted_team` text NOT NULL,
  `team_candidate` text NOT NULL,
  `voter_name` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `factionbanks`
--

CREATE TABLE `factionbanks` (
  `ID` int(11) NOT NULL,
  `LSPD` int(11) NOT NULL,
  `FBI` int(11) NOT NULL,
  `SAST` int(11) NOT NULL,
  `FMD` int(11) NOT NULL,
  `SANG` int(11) NOT NULL,
  `GOV` int(11) NOT NULL,
  `SCOTUS` int(11) NOT NULL,
  `NEWS` int(11) NOT NULL,
  `TAXI` int(11) NOT NULL,
  `SAPD` int(20) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `familysafelogs`
--

CREATE TABLE `familysafelogs` (
  `Player` varchar(24) NOT NULL,
  `PlayerSQL` int(11) NOT NULL,
  `Action` varchar(128) NOT NULL,
  `FamilyID` int(11) NOT NULL,
  `Date` varchar(11) DEFAULT NULL,
  `ID` int(11) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `furniture`
--

CREATE TABLE `furniture` (
  `ID` int(11) UNSIGNED NOT NULL,
  `Active` tinyint(1) NOT NULL,
  `Key_` varchar(32) DEFAULT NULL,
  `HouseID` int(11) UNSIGNED NOT NULL,
  `ModelID` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `ModelName` varchar(50) NOT NULL,
  `Spawned` tinyint(1) NOT NULL DEFAULT '0',
  `Outdoor` tinyint(1) NOT NULL DEFAULT '0',
  `VirtualWorld` int(11) NOT NULL DEFAULT '0',
  `X` float NOT NULL DEFAULT '0',
  `Y` float NOT NULL DEFAULT '0',
  `Z` float NOT NULL DEFAULT '0',
  `rX` float NOT NULL DEFAULT '0',
  `rY` float NOT NULL DEFAULT '0',
  `rZ` float NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `gamefeed`
--

CREATE TABLE `gamefeed` (
  `ID` int(11) NOT NULL,
  `Date` datetime NOT NULL,
  `Message` varchar(255) NOT NULL,
  `Color` varchar(24) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `hotelrooms`
--

CREATE TABLE `hotelrooms` (
  `ID` bigint(20) UNSIGNED NOT NULL,
  `OwnerID` int(11) NOT NULL DEFAULT '0',
  `X` float NOT NULL DEFAULT '0',
  `Y` float NOT NULL DEFAULT '0',
  `Z` float NOT NULL DEFAULT '0',
  `VW` int(11) NOT NULL,
  `Interior` int(11) NOT NULL,
  `CreatedBy` varchar(25) NOT NULL DEFAULT 'Unknown',
  `HotelID` bigint(20) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `hotels`
--

CREATE TABLE `hotels` (
  `ID` bigint(20) UNSIGNED NOT NULL,
  `HotelCreated` int(11) NOT NULL DEFAULT '1',
  `HotelX` float NOT NULL,
  `HotelY` float NOT NULL,
  `HotelZ` float NOT NULL,
  `IHotelX` float NOT NULL,
  `IHotelY` float NOT NULL,
  `IHotelZ` float NOT NULL,
  `HotelPrice` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `houses`
--

CREATE TABLE `houses` (
  `HouseID` int(255) NOT NULL,
  `OwnerID` varchar(2000) DEFAULT NULL,
  `Created` tinyint(1) NOT NULL,
  `Owned` tinyint(1) NOT NULL,
  `Value` int(11) NOT NULL,
  `OutsideX` float NOT NULL,
  `OutsideY` float NOT NULL,
  `OutsideZ` float NOT NULL,
  `OutsideA` float NOT NULL,
  `InsideX` float NOT NULL,
  `InsideY` float NOT NULL,
  `InsideZ` float NOT NULL,
  `InsideA` float NOT NULL,
  `Interior` int(11) NOT NULL,
  `Locked` tinyint(1) NOT NULL DEFAULT '0',
  `Alarm` tinyint(1) NOT NULL DEFAULT '0',
  `AlarmStatus` tinyint(1) NOT NULL DEFAULT '0',
  `Cash` int(11) DEFAULT '0',
  `Materials` int(11) DEFAULT '0',
  `Pot` int(11) DEFAULT '0',
  `Crack` int(11) DEFAULT '0',
  `Gun1` tinyint(2) DEFAULT '0',
  `Gun2` tinyint(2) DEFAULT '0',
  `Gun3` tinyint(2) DEFAULT '0',
  `Gun4` tinyint(2) DEFAULT '0',
  `Gun5` tinyint(2) DEFAULT '0',
  `Gun6` tinyint(2) DEFAULT '0',
  `Gun7` tinyint(2) DEFAULT '0',
  `Tag` varchar(32) NOT NULL DEFAULT 'House',
  `Level` tinyint(2) DEFAULT '1',
  `Ammo4` int(11) DEFAULT '0',
  `Ammo3` int(11) DEFAULT '0',
  `Ammo2` int(11) DEFAULT '0',
  `Ammo1` int(11) DEFAULT '0',
  `Freeze` tinyint(1) NOT NULL DEFAULT '1',
  `HasFurniture` tinyint(1) DEFAULT NULL,
  `Donated` int(11) DEFAULT NULL,
  `Cannabis` int(11) DEFAULT NULL,
  `Xanax` int(11) DEFAULT NULL,
  `Codeine` int(11) DEFAULT NULL,
  `Lean` int(11) DEFAULT NULL,
  `Cocaine` int(11) DEFAULT NULL,
  `Meth` int(11) DEFAULT NULL,
  `Promethazine` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `locker`
--

CREATE TABLE `locker` (
  `ID` int(11) NOT NULL,
  `LockerX` float NOT NULL,
  `LockerY` int(11) NOT NULL,
  `LockerZ` int(11) NOT NULL,
  `FactionID` int(11) NOT NULL DEFAULT '0',
  `GroupID` int(11) NOT NULL DEFAULT '0',
  `LockerName` varchar(150) NOT NULL DEFAULT 'Locker',
  `Weapon1` int(11) NOT NULL DEFAULT '0',
  `Weapon2` int(11) NOT NULL DEFAULT '0',
  `Weapon3` int(11) NOT NULL DEFAULT '0',
  `Weapon4` int(11) NOT NULL DEFAULT '0',
  `Weapon5` int(11) NOT NULL DEFAULT '0',
  `Weapon6` int(11) NOT NULL DEFAULT '0',
  `Weapon7` int(11) NOT NULL DEFAULT '0',
  `Weapon8` int(11) NOT NULL DEFAULT '0',
  `Weapon9` int(11) NOT NULL DEFAULT '0',
  `Weapon10` int(11) NOT NULL DEFAULT '0',
  `WeaponPrice1` int(11) NOT NULL,
  `WeaponPrice2` int(11) NOT NULL,
  `WeaponPrice3` int(11) NOT NULL,
  `WeaponPrice4` int(11) NOT NULL,
  `WeaponPrice5` int(11) NOT NULL,
  `WeaponPrice6` int(11) NOT NULL,
  `WeaponPrice7` int(11) NOT NULL,
  `WeaponPrice8` int(11) NOT NULL,
  `WeaponPrice9` int(11) NOT NULL,
  `WeaponPrice10` int(11) NOT NULL,
  `DGID` int(11) NOT NULL DEFAULT '-1'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `maps`
--

CREATE TABLE `maps` (
  `ID` int(11) NOT NULL,
  `sqlid` int(10) NOT NULL,
  `comment` varchar(50) NOT NULL,
  `X` float NOT NULL,
  `Y` float NOT NULL,
  `Z` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `namechanges`
--

CREATE TABLE `namechanges` (
  `ID` int(11) NOT NULL,
  `OldName` varchar(24) NOT NULL,
  `NewName` varchar(24) NOT NULL,
  `ApprovedBy` varchar(24) NOT NULL,
  `Date` varchar(40) NOT NULL,
  `pID` int(11) NOT NULL,
  `PIDAdmin` int(11) NOT NULL,
  `RealName` varchar(45) NOT NULL DEFAULT 'Unspecified'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `newcrimes`
--

CREATE TABLE `newcrimes` (
  `CrimeID` int(11) NOT NULL,
  `CrimePID` int(11) NOT NULL,
  `CrimeIssuer` int(11) NOT NULL,
  `Date` varchar(40) NOT NULL,
  `CrimeReason` varchar(45) NOT NULL,
  `CrimeType` int(11) NOT NULL,
  `CrimeEnabled` tinyint(1) DEFAULT NULL,
  `RealName` varchar(45) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `paylog`
--

CREATE TABLE `paylog` (
  `ID` int(11) NOT NULL,
  `Date` date NOT NULL,
  `Info` varchar(256) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `phonecontacts`
--

CREATE TABLE `phonecontacts` (
  `contact_sql` int(11) NOT NULL,
  `contact_number` bigint(20) NOT NULL,
  `contact_name` varchar(30) NOT NULL DEFAULT ''
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `players`
--

CREATE TABLE `players` (
  `ID` int(11) UNSIGNED NOT NULL,
  `TruckLicense` int(11) NOT NULL DEFAULT '0',
  `BussSkil` int(11) NOT NULL DEFAULT '0',
  `Name` varchar(32) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL,
  `Key_` varchar(64) DEFAULT NULL,
  `Level` int(11) UNSIGNED NOT NULL DEFAULT '2',
  `AdminLvl` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `AdminLevel` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `SecurityKey` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Stealthed` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Band` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `PermBand` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `BanReason` varchar(255) NOT NULL DEFAULT 'None',
  `PrisonReason` varchar(255) NOT NULL DEFAULT 'None',
  `AdminJailed` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `PrisonedBy` varchar(32) NOT NULL DEFAULT 'Nobody',
  `Warnings` int(11) DEFAULT NULL,
  `Disabled` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Bleeding` tinyint(4) NOT NULL DEFAULT '0',
  `DonateRank` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Tag` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `VIPJoinDate` varchar(32) DEFAULT NULL,
  `VIPExpDate` varchar(32) DEFAULT NULL,
  `BanAppealer` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `GangModd` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `EventModd` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `ComplaintModd` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `ShopMod` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `BizMod` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `FactionMod` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `GangDirector` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `UpgradePoints` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `ConnectedTime` int(11) NOT NULL DEFAULT '2',
  `Registered` tinyint(1) UNSIGNED NOT NULL DEFAULT '1',
  `Sex` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Age` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Appearance` varchar(255) DEFAULT NULL,
  `Muted` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Respect` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Money` int(11) NOT NULL DEFAULT '200000',
  `Bank` int(11) NOT NULL DEFAULT '1000000',
  `Crimes` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Accent` varchar(64) NOT NULL DEFAULT 'None',
  `Kills` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Deaths` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `CHits` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `FHits` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Arrested` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `LottoNr` smallint(4) UNSIGNED NOT NULL DEFAULT '0',
  `BiggestFish` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Job` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Paycheck` mediumint(6) UNSIGNED NOT NULL DEFAULT '0',
  `HeadValue` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Jailed` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `JailCell` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `JailTime` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Leader` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Member` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `FMember` tinyint(3) UNSIGNED NOT NULL DEFAULT '255',
  `Rank` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `RadioStation` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Instructor` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Passes` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Fails` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `DetSkill` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `SexSkill` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `BoxSkill` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `LawSkill` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `MechSkill` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `JackSkill` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `CarSkill` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `DrugsSkill` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `ArmsSkill` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `SmugglerSkill` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `ThieveSkill` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `FishSkill` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `FightingStyle` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `pHealth` tinyint(3) UNSIGNED NOT NULL DEFAULT '100',
  `pArmor` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `pSHP` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `pStrength` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `pSHealth` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `InteriorID` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Local` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `VirtualWorld` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Model` int(11) UNSIGNED NOT NULL DEFAULT '299',
  `Clothes` int(11) UNSIGNED NOT NULL DEFAULT '299',
  `PhoneNr` int(11) NOT NULL DEFAULT '0',
  `PhoneType` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `BizFixed` int(11) UNSIGNED DEFAULT NULL,
  `SetVehSpawn` int(11) DEFAULT NULL,
  `CarLic` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `FlyLic` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `BoatLic` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `FishLic` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `GunLic` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Gun0` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Gun1` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Gun2` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Gun3` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Gun4` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Gun5` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Gun6` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Gun7` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Gun8` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Gun9` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Gun10` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Gun11` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Gun12` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `CarTime` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `DrugsTime` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `LawyerTime` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `LawyerFreeTime` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `MechTime` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `SexTime` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `PlantTime` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `RepairTime` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `RobTime` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `PayDay` int(11) NOT NULL DEFAULT '0',
  `PayDayHad` int(11) NOT NULL DEFAULT '0',
  `Wins` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Loses` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Tutorial` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `OnDuty` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Hospital` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Adjustable` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Married` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `MarriedTo` varchar(32) DEFAULT NULL,
  `ContractBy` varchar(32) NOT NULL DEFAULT '0',
  `IP` varchar(32) DEFAULT NULL,
  `Country` varchar(64) DEFAULT NULL,
  `WantedLevel` tinyint(3) UNSIGNED DEFAULT NULL,
  `NewbieMuted` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `NewbieMutedTime` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `NewbieWarns` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `AdMuted` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `BugMuted` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `HelperLevel` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `DevLevel` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `TesterLevel` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Famed` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Enlightened` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `BanDay` tinyint(2) UNSIGNED NOT NULL DEFAULT '0',
  `BanMonth` tinyint(2) UNSIGNED NOT NULL DEFAULT '0',
  `BanYear` smallint(4) UNSIGNED NOT NULL DEFAULT '0',
  `BanTime` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `GottenGift` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `GottenTimeLeft` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `pHouseID` smallint(11) NOT NULL DEFAULT '0',
  `hTicket` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `gTicket` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `gRefund` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Economist` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Mission_spawn` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Mission_mafia` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Mission_triad` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Mission_smoke` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Mission_ryder` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Mission_taxi` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Mission_redneck3` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Mission_cargo` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `HAFakeBadge` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Badge` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Accessory1` mediumint(8) UNSIGNED DEFAULT NULL,
  `Accessory2` mediumint(8) UNSIGNED DEFAULT NULL,
  `Accessory3` mediumint(8) UNSIGNED DEFAULT NULL,
  `Accessory4` mediumint(8) UNSIGNED DEFAULT NULL,
  `Accessory5` mediumint(8) UNSIGNED DEFAULT NULL,
  `Accessory6` mediumint(8) UNSIGNED DEFAULT NULL,
  `Accessory7` mediumint(8) UNSIGNED DEFAULT NULL,
  `AccessoryB1` tinyint(2) UNSIGNED DEFAULT NULL,
  `AccessoryB2` tinyint(2) UNSIGNED DEFAULT NULL,
  `AccessoryB3` tinyint(2) UNSIGNED DEFAULT NULL,
  `AccessoryB4` tinyint(2) UNSIGNED DEFAULT NULL,
  `AccessoryB5` tinyint(2) UNSIGNED DEFAULT NULL,
  `AccessoryB6` tinyint(2) UNSIGNED DEFAULT NULL,
  `AccessoryB7` tinyint(2) UNSIGNED DEFAULT NULL,
  `AccessoryX1` float DEFAULT NULL,
  `AccessoryX2` float DEFAULT NULL,
  `AccessoryX3` float DEFAULT NULL,
  `AccessoryX4` float DEFAULT NULL,
  `AccessoryX5` float DEFAULT NULL,
  `AccessoryX6` float DEFAULT NULL,
  `AccessoryX7` float DEFAULT NULL,
  `AccessoryY1` float DEFAULT NULL,
  `AccessoryY2` float DEFAULT NULL,
  `AccessoryY3` float DEFAULT NULL,
  `AccessoryY4` float DEFAULT NULL,
  `AccessoryY5` float DEFAULT NULL,
  `AccessoryY6` float DEFAULT NULL,
  `AccessoryY7` float DEFAULT NULL,
  `AccessoryZ1` float DEFAULT NULL,
  `AccessoryZ2` float DEFAULT NULL,
  `AccessoryZ3` float DEFAULT NULL,
  `AccessoryZ4` float DEFAULT NULL,
  `AccessoryZ5` float DEFAULT NULL,
  `AccessoryZ6` float DEFAULT NULL,
  `AccessoryZ7` float DEFAULT NULL,
  `AccessoryrX1` float DEFAULT NULL,
  `AccessoryrX2` float DEFAULT NULL,
  `AccessoryrX3` float DEFAULT NULL,
  `AccessoryrX4` float DEFAULT NULL,
  `AccessoryrX5` float DEFAULT NULL,
  `AccessoryrX6` float DEFAULT NULL,
  `AccessoryrX7` float DEFAULT NULL,
  `AccessoryrY1` float DEFAULT NULL,
  `AccessoryrY2` float DEFAULT NULL,
  `AccessoryrY3` float DEFAULT NULL,
  `AccessoryrY4` float DEFAULT NULL,
  `AccessoryrY5` float DEFAULT NULL,
  `AccessoryrY6` float DEFAULT NULL,
  `AccessoryrY7` float DEFAULT NULL,
  `AccessoryrZ1` float DEFAULT NULL,
  `AccessoryrZ2` float DEFAULT NULL,
  `AccessoryrZ3` float DEFAULT NULL,
  `AccessoryrZ4` float DEFAULT NULL,
  `AccessoryrZ5` float DEFAULT NULL,
  `AccessoryrZ6` float DEFAULT NULL,
  `AccessoryrZ7` float DEFAULT NULL,
  `AccessorysX1` float DEFAULT NULL,
  `AccessorysX2` float DEFAULT NULL,
  `AccessorysX3` float DEFAULT NULL,
  `AccessorysX4` float DEFAULT NULL,
  `AccessorysX5` float DEFAULT NULL,
  `AccessorysX6` float DEFAULT NULL,
  `AccessorysX7` float DEFAULT NULL,
  `AccessorysY1` float DEFAULT NULL,
  `AccessorysY2` float DEFAULT NULL,
  `AccessorysY3` float DEFAULT NULL,
  `AccessorysY4` float DEFAULT NULL,
  `AccessorysY5` float DEFAULT NULL,
  `AccessorysY6` float DEFAULT NULL,
  `AccessorysY7` float DEFAULT NULL,
  `AccessorysZ1` float DEFAULT NULL,
  `AccessorysZ2` float DEFAULT NULL,
  `AccessorysZ3` float DEFAULT NULL,
  `AccessorysZ4` float DEFAULT NULL,
  `AccessorysZ5` float DEFAULT NULL,
  `AccessorysZ6` float DEFAULT NULL,
  `AccessorysZ7` float DEFAULT NULL,
  `AccessorysC1` mediumint(8) UNSIGNED DEFAULT NULL,
  `AccessorysC2` mediumint(8) UNSIGNED DEFAULT NULL,
  `AccessorysC3` mediumint(8) UNSIGNED DEFAULT NULL,
  `AccessorysC4` mediumint(8) UNSIGNED DEFAULT NULL,
  `AccessorysC5` mediumint(8) UNSIGNED DEFAULT NULL,
  `AccessorysC6` mediumint(8) UNSIGNED DEFAULT NULL,
  `AccessorysC7` mediumint(8) UNSIGNED DEFAULT NULL,
  `CKey1` smallint(5) UNSIGNED NOT NULL DEFAULT '0',
  `CKey2` smallint(5) UNSIGNED NOT NULL DEFAULT '0',
  `Mapper` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `customTag` varchar(32) NOT NULL DEFAULT 'None',
  `customRank` varchar(32) NOT NULL DEFAULT '0',
  `LastLogin` varchar(64) NOT NULL DEFAULT '0',
  `isinHosp` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `HasMap` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `JailNumber` int(11) NOT NULL DEFAULT '0',
  `NewCarLic` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `NewFlyLic` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `NewBoatLic` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `NewGunLic` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `SueUnix` int(11) NOT NULL DEFAULT '0',
  `SueNotice` int(11) NOT NULL DEFAULT '0',
  `AllowedArmory` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `WalkStyle` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Fishes` int(11) NOT NULL DEFAULT '0',
  `Products` int(11) NOT NULL DEFAULT '0',
  `Materials` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Seeds` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Calculator` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Suitcase` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `SuitCrack` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `SuitMats` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `SuitWeapon` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `SuitPot` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `SuitMoney` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Phonebook` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `FishRod` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Bait` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `BaitAmount` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Line` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `iPod` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Dice` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Spraycan` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Screw` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Rope` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Cigars` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Sprunk` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Tikis` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `WT` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `WTc` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Radio` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Handtazer` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `tazerGun` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Bombs` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Poison` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Glasses` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Bandana` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Helmet` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Mask` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Blindfolds` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Deck` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `Stereo` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Matches` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `Jerrycan` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `MP5Laser` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `M4Laser` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `AKLaser` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `SpasLaser` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `DeagleLaser` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `AddressBook` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `DeathState` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `GearFixed` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `Division` int(11) NOT NULL DEFAULT '0',
  `LogUnix` int(11) NOT NULL DEFAULT '0',
  `HelperReports` int(11) NOT NULL DEFAULT '0',
  `TesterReports` int(11) NOT NULL DEFAULT '0',
  `WasteSkill` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `HouseFire` int(11) NOT NULL DEFAULT '0',
  `TikiRefund` int(11) NOT NULL DEFAULT '0',
  `ImperiumRank` int(11) NOT NULL DEFAULT '0',
  `Wire` int(11) NOT NULL DEFAULT '0',
  `NewMariLicense` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `HitmanRank` int(1) NOT NULL DEFAULT '0',
  `SuitAmmo` int(11) NOT NULL DEFAULT '0',
  `Group` int(1) NOT NULL DEFAULT '0',
  `GroupRank` int(1) NOT NULL DEFAULT '0',
  `Serial` varchar(128) NOT NULL DEFAULT '0',
  `avatar_image_url` varchar(128) NOT NULL DEFAULT 'players/avatars/default/avatar_small.png',
  `GameAccount` int(1) NOT NULL DEFAULT '0',
  `Password` varchar(64) NOT NULL DEFAULT ' ',
  `Salt` varchar(32) NOT NULL DEFAULT ' ',
  `ContractReason` varchar(32) DEFAULT NULL,
  `CKey3` int(11) DEFAULT NULL,
  `CKey4` int(11) DEFAULT NULL,
  `lastCarID` int(11) DEFAULT NULL,
  `maxcarslots` int(11) NOT NULL DEFAULT '2',
  `C2` int(11) DEFAULT NULL,
  `MedicCount` int(11) NOT NULL DEFAULT '0',
  `FakeFaction` tinyint(1) NOT NULL DEFAULT '0',
  `FactionKickTime` int(11) NOT NULL DEFAULT '0',
  `MDCLogged` tinyint(1) DEFAULT NULL,
  `isBOLO` tinyint(1) DEFAULT NULL,
  `HasVoted1` int(1) DEFAULT NULL,
  `HasVoted2` int(1) DEFAULT NULL,
  `HasVoted3` int(1) DEFAULT NULL,
  `HasMDC` int(1) NOT NULL DEFAULT '0',
  `HasJammer` int(11) DEFAULT NULL,
  `brokenleg` int(1) DEFAULT '0',
  `Cannabis` int(11) DEFAULT NULL,
  `Cocaine` int(11) DEFAULT NULL,
  `Meth` int(11) DEFAULT NULL,
  `Build` int(1) DEFAULT NULL,
  `EyeColour` int(1) DEFAULT NULL,
  `Ethnicity` int(2) DEFAULT NULL,
  `Height` int(3) DEFAULT NULL,
  `MaskName` varchar(21) DEFAULT NULL,
  `OldSchool` int(11) NOT NULL DEFAULT '0',
  `DriverPoints` int(1) DEFAULT NULL,
  `NewPassword` varchar(130) NOT NULL DEFAULT ' ',
  `NewSalt` varchar(33) NOT NULL DEFAULT ' ',
  `NewbRequestUnmute` int(11) NOT NULL DEFAULT '0',
  `Gag` tinyint(1) NOT NULL DEFAULT '0',
  `TogNewbie` int(1) DEFAULT NULL,
  `PresentCount` int(20) DEFAULT NULL,
  `HidingStatus` int(2) DEFAULT NULL,
  `ReportMuted` int(5) DEFAULT NULL,
  `TogAutoChat` int(2) DEFAULT NULL,
  `Insurance` int(1) DEFAULT NULL,
  `Origin` varchar(128) NOT NULL DEFAULT '0',
  `FactionDirector` tinyint(1) DEFAULT NULL,
  `Xanax` int(11) DEFAULT NULL,
  `NSB` tinyint(1) DEFAULT NULL,
  `FakeRank` tinyint(1) DEFAULT NULL,
  `PizzaSkill` int(11) DEFAULT NULL,
  `RubberBullets` int(11) DEFAULT NULL,
  `BadgeNumber` int(11) DEFAULT NULL,
  `Beer` int(11) DEFAULT NULL,
  `PropDate` text,
  `Recruiter` int(11) NOT NULL DEFAULT '0',
  `GroupNick` varchar(13) NOT NULL DEFAULT 'Stranger',
  `promethazine` int(11) NOT NULL DEFAULT '0',
  `codeine` int(11) NOT NULL DEFAULT '0',
  `lean` int(11) NOT NULL DEFAULT '0',
  `DuffleCooldown` int(11) NOT NULL DEFAULT '0',
  `S9BadgeName` varchar(48) NOT NULL DEFAULT 'None',
  `Drill` int(25) NOT NULL DEFAULT '0',
  `MonthlyMedicCount` int(25) NOT NULL DEFAULT '0',
  `SPos_x` float NOT NULL DEFAULT '0',
  `SPos_y` float NOT NULL DEFAULT '0',
  `SPos_z` float NOT NULL DEFAULT '0',
  `SPos_r` float NOT NULL DEFAULT '0',
  `SpawnPos` int(11) NOT NULL DEFAULT '0',
  `BizKey` int(11) NOT NULL DEFAULT '-1',
  `Adrenaline` tinyint(1) DEFAULT '0',
  `CBand` tinyint(1) UNSIGNED NOT NULL DEFAULT '0',
  `pGroupNick` varchar(30) DEFAULT 'None',
  `Cookies` int(11) UNSIGNED DEFAULT '0',
  `FishB` int(11) NOT NULL DEFAULT '0',
  `GFX` int(11) NOT NULL DEFAULT '0',
  `RefSqlID` int(11) NOT NULL DEFAULT '-1',
  `ReferredSomeone` int(11) NOT NULL DEFAULT '0',
  `Tokens` int(11) NOT NULL DEFAULT '0',
  `DonatedRank` int(11) NOT NULL DEFAULT '0',
  `DonatedGroup` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `playervehicles`
--

CREATE TABLE `playervehicles` (
  `ID` int(11) NOT NULL,
  `pID` int(11) NOT NULL,
  `x` float NOT NULL DEFAULT '0',
  `y` float NOT NULL DEFAULT '0',
  `z` float NOT NULL DEFAULT '0',
  `rz` float NOT NULL DEFAULT '0',
  `model` int(11) NOT NULL,
  `locked` int(11) NOT NULL DEFAULT '1',
  `color1` int(11) NOT NULL DEFAULT '0',
  `color2` int(11) NOT NULL DEFAULT '0',
  `damage` float DEFAULT '0',
  `gas` int(11) DEFAULT '0',
  `gps` int(11) DEFAULT '0',
  `alarm` int(11) DEFAULT '0',
  `tow` int(11) DEFAULT '0',
  `Gun1` int(11) DEFAULT '0',
  `Gun2` int(11) DEFAULT '0',
  `Pot` int(11) DEFAULT '0',
  `Crack` int(11) DEFAULT '0',
  `Armor` int(11) DEFAULT '0',
  `sparekey` int(11) DEFAULT '0',
  `parkx` float DEFAULT '0',
  `parky` float DEFAULT '0',
  `parkz` float DEFAULT '0',
  `parkrz` float DEFAULT '0',
  `plate` varchar(10) DEFAULT 'None',
  `insurance` int(11) DEFAULT '0',
  `dpanels` int(11) DEFAULT '0',
  `ddoors` int(11) DEFAULT '0',
  `dlights` int(11) DEFAULT '0',
  `dtires` int(11) DEFAULT '0',
  `paintjob` int(11) DEFAULT '0',
  `component0` int(8) DEFAULT '0',
  `component1` int(8) DEFAULT '0',
  `component2` int(8) DEFAULT '0',
  `component3` int(8) DEFAULT '0',
  `component4` int(8) DEFAULT '0',
  `component5` int(8) DEFAULT '0',
  `component6` int(8) DEFAULT '0',
  `component7` int(8) DEFAULT '0',
  `component8` int(8) DEFAULT '0',
  `component9` int(8) DEFAULT '0',
  `component10` int(8) DEFAULT '0',
  `component11` int(8) DEFAULT '0',
  `component12` int(8) DEFAULT '0',
  `component13` int(8) DEFAULT '0',
  `vw` int(15) DEFAULT '0',
  `impounded` int(1) DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `PNS`
--

CREATE TABLE `PNS` (
  `ID` int(11) NOT NULL,
  `pnsX` float NOT NULL,
  `pnsY` float NOT NULL,
  `pnsZ` float NOT NULL,
  `pnsIX` float NOT NULL,
  `pnsIY` float NOT NULL,
  `pnsIZ` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `pointsrules`
--

CREATE TABLE `pointsrules` (
  `ID` int(11) NOT NULL,
  `admin` varchar(25) NOT NULL,
  `text` varchar(128) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `prices`
--

CREATE TABLE `prices` (
  `ID` int(11) NOT NULL,
  `admin` varchar(25) NOT NULL,
  `text` varchar(128) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `prisons`
--

CREATE TABLE `prisons` (
  `ID` int(11) NOT NULL,
  `User` varchar(24) NOT NULL,
  `Admin` varchar(24) NOT NULL,
  `Reason` varchar(128) NOT NULL,
  `Time` int(11) NOT NULL,
  `Date` varchar(40) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `safes`
--

CREATE TABLE `safes` (
  `safeid` int(11) NOT NULL,
  `OwnerID` int(11) NOT NULL,
  `posX` float NOT NULL,
  `posY` float NOT NULL,
  `posZ` float NOT NULL,
  `VW` int(11) NOT NULL,
  `interior` int(11) NOT NULL,
  `cash` int(11) NOT NULL,
  `mats` int(11) NOT NULL,
  `meth` int(11) NOT NULL,
  `cocaine` int(11) NOT NULL,
  `cannabis` int(11) NOT NULL,
  `weapon0` tinyint(3) NOT NULL,
  `weapon1` tinyint(3) NOT NULL,
  `weapon2` tinyint(3) NOT NULL,
  `weapon3` tinyint(3) NOT NULL,
  `weapon4` tinyint(3) NOT NULL,
  `weapon5` tinyint(3) NOT NULL,
  `weapon6` tinyint(3) NOT NULL,
  `weapon7` tinyint(3) NOT NULL,
  `weapon8` tinyint(3) NOT NULL,
  `weapon9` tinyint(3) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `stuff`
--

CREATE TABLE `stuff` (
  `ID` int(11) NOT NULL,
  `versionstr` varchar(3000) NOT NULL,
  `Jackpot` int(11) NOT NULL,
  `Tax` int(11) NOT NULL,
  `TaxValue` int(11) NOT NULL,
  `Security` int(11) NOT NULL,
  `TimeAfterBankJob` int(11) NOT NULL,
  `dhstock` int(11) NOT NULL,
  `chstock` int(11) NOT NULL,
  `anticheatbans` int(11) NOT NULL,
  `anticheatwarns` int(11) NOT NULL,
  `matprice` int(11) NOT NULL,
  `seedprice` int(11) NOT NULL,
  `crackgprice` int(11) NOT NULL,
  `prodprice` int(11) NOT NULL,
  `potcprice` int(11) NOT NULL,
  `crackcprice` int(11) NOT NULL,
  `HitmanBadgeLSPD` int(11) NOT NULL,
  `HitmanBadgeFBI` int(11) NOT NULL,
  `HitmanBadgeRCSD` int(11) NOT NULL,
  `HitmanBadgeLSFMD` int(11) NOT NULL,
  `HitmanBadgeNG` int(11) NOT NULL,
  `HitmanBadgeGOV` int(11) NOT NULL,
  `HitmanBadgeSCOTUS` int(11) NOT NULL,
  `HitmanBadgeDEA` int(11) NOT NULL,
  `HitmanBadgeATF` int(11) NOT NULL,
  `devmotd` varchar(128) DEFAULT 'DEVMOTD:None',
  `testermotd` varchar(128) DEFAULT 'TESTERMOTD:None'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `tickets`
--

CREATE TABLE `tickets` (
  `TicketID` int(11) NOT NULL,
  `TicketPID` int(11) NOT NULL,
  `TicketIssuer` int(11) NOT NULL,
  `TicketReason` varchar(45) NOT NULL,
  `Amount` int(11) NOT NULL,
  `Active` int(11) NOT NULL,
  `Date` varchar(45) NOT NULL,
  `IssuerName` varchar(45) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `tower`
--

CREATE TABLE `tower` (
  `ID` int(11) NOT NULL,
  `tower_X` float NOT NULL,
  `tower_Y` float NOT NULL,
  `tower_Z` float NOT NULL,
  `tower_fX` float NOT NULL,
  `tower_fY` float NOT NULL,
  `tower_fZ` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `vehicles`
--

CREATE TABLE `vehicles` (
  `ID` int(11) NOT NULL,
  `model` int(11) NOT NULL,
  `x` float NOT NULL,
  `y` float NOT NULL,
  `z` float NOT NULL,
  `rz` float NOT NULL,
  `color1` int(11) NOT NULL,
  `color2` int(11) NOT NULL,
  `faction` int(11) NOT NULL,
  `family` int(2) NOT NULL DEFAULT '0',
  `lockfeature` int(2) NOT NULL DEFAULT '0',
  `Siren` tinyint(1) NOT NULL DEFAULT '0',
  `group` tinyint(1) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `version`
--

CREATE TABLE `version` (
  `ID` int(11) NOT NULL,
  `developer` varchar(25) NOT NULL,
  `text` varchar(128) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `voters`
--

CREATE TABLE `voters` (
  `ID` int(11) NOT NULL,
  `votedFor` varchar(24) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `warnings`
--

CREATE TABLE `warnings` (
  `WarnID` int(11) NOT NULL,
  `PlayerID` int(11) DEFAULT NULL,
  `AdminName` varchar(32) CHARACTER SET latin1 COLLATE latin1_bin DEFAULT NULL,
  `Date` varchar(32) NOT NULL DEFAULT 'Never',
  `Reason` varchar(32) DEFAULT NULL,
  `Active` tinyint(1) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `weaponslog`
--

CREATE TABLE `weaponslog` (
  `ID` int(11) NOT NULL,
  `Name` varchar(45) NOT NULL,
  `GivenBy` varchar(45) NOT NULL,
  `Date` varchar(45) NOT NULL,
  `Taken` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `activitylog`
--
ALTER TABLE `activitylog`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `atm`
--
ALTER TABLE `atm`
  ADD UNIQUE KEY `ID` (`ID`);

--
-- Indexes for table `banlist`
--
ALTER TABLE `banlist`
  ADD PRIMARY KEY (`ID`),
  ADD KEY `banlist_ibfk_1` (`userID`);

--
-- Indexes for table `charges`
--
ALTER TABLE `charges`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `crimes`
--
ALTER TABLE `crimes`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `deaths`
--
ALTER TABLE `deaths`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `divisions`
--
ALTER TABLE `divisions`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `donatedgroups`
--
ALTER TABLE `donatedgroups`
  ADD UNIQUE KEY `ID` (`ID`);

--
-- Indexes for table `election`
--
ALTER TABLE `election`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `factionbanks`
--
ALTER TABLE `factionbanks`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `familysafelogs`
--
ALTER TABLE `familysafelogs`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `furniture`
--
ALTER TABLE `furniture`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `gamefeed`
--
ALTER TABLE `gamefeed`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `hotelrooms`
--
ALTER TABLE `hotelrooms`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `hotels`
--
ALTER TABLE `hotels`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `houses`
--
ALTER TABLE `houses`
  ADD PRIMARY KEY (`HouseID`),
  ADD KEY `OwnerID` (`OwnerID`(767));

--
-- Indexes for table `locker`
--
ALTER TABLE `locker`
  ADD UNIQUE KEY `ID` (`ID`);

--
-- Indexes for table `maps`
--
ALTER TABLE `maps`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `namechanges`
--
ALTER TABLE `namechanges`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `newcrimes`
--
ALTER TABLE `newcrimes`
  ADD PRIMARY KEY (`CrimeID`);

--
-- Indexes for table `paylog`
--
ALTER TABLE `paylog`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `phonecontacts`
--
ALTER TABLE `phonecontacts`
  ADD PRIMARY KEY (`contact_sql`,`contact_number`);

--
-- Indexes for table `players`
--
ALTER TABLE `players`
  ADD PRIMARY KEY (`ID`),
  ADD UNIQUE KEY `Name` (`Name`);

--
-- Indexes for table `playervehicles`
--
ALTER TABLE `playervehicles`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `PNS`
--
ALTER TABLE `PNS`
  ADD UNIQUE KEY `ID` (`ID`);

--
-- Indexes for table `pointsrules`
--
ALTER TABLE `pointsrules`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `prices`
--
ALTER TABLE `prices`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `prisons`
--
ALTER TABLE `prisons`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `safes`
--
ALTER TABLE `safes`
  ADD PRIMARY KEY (`safeid`);

--
-- Indexes for table `stuff`
--
ALTER TABLE `stuff`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `tickets`
--
ALTER TABLE `tickets`
  ADD PRIMARY KEY (`TicketID`);

--
-- Indexes for table `tower`
--
ALTER TABLE `tower`
  ADD UNIQUE KEY `ID` (`ID`);

--
-- Indexes for table `vehicles`
--
ALTER TABLE `vehicles`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `version`
--
ALTER TABLE `version`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `warnings`
--
ALTER TABLE `warnings`
  ADD PRIMARY KEY (`WarnID`);

--
-- Indexes for table `weaponslog`
--
ALTER TABLE `weaponslog`
  ADD PRIMARY KEY (`ID`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `activitylog`
--
ALTER TABLE `activitylog`
  MODIFY `ID` int(11) UNSIGNED NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `banlist`
--
ALTER TABLE `banlist`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `charges`
--
ALTER TABLE `charges`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `crimes`
--
ALTER TABLE `crimes`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `deaths`
--
ALTER TABLE `deaths`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `divisions`
--
ALTER TABLE `divisions`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `election`
--
ALTER TABLE `election`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `factionbanks`
--
ALTER TABLE `factionbanks`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `familysafelogs`
--
ALTER TABLE `familysafelogs`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `furniture`
--
ALTER TABLE `furniture`
  MODIFY `ID` int(11) UNSIGNED NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `gamefeed`
--
ALTER TABLE `gamefeed`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `hotelrooms`
--
ALTER TABLE `hotelrooms`
  MODIFY `ID` bigint(20) UNSIGNED NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `hotels`
--
ALTER TABLE `hotels`
  MODIFY `ID` bigint(20) UNSIGNED NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `houses`
--
ALTER TABLE `houses`
  MODIFY `HouseID` int(255) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `maps`
--
ALTER TABLE `maps`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `namechanges`
--
ALTER TABLE `namechanges`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `newcrimes`
--
ALTER TABLE `newcrimes`
  MODIFY `CrimeID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `paylog`
--
ALTER TABLE `paylog`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `players`
--
ALTER TABLE `players`
  MODIFY `ID` int(11) UNSIGNED NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `playervehicles`
--
ALTER TABLE `playervehicles`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `pointsrules`
--
ALTER TABLE `pointsrules`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `prices`
--
ALTER TABLE `prices`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `prisons`
--
ALTER TABLE `prisons`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `safes`
--
ALTER TABLE `safes`
  MODIFY `safeid` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `stuff`
--
ALTER TABLE `stuff`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `tickets`
--
ALTER TABLE `tickets`
  MODIFY `TicketID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `vehicles`
--
ALTER TABLE `vehicles`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `version`
--
ALTER TABLE `version`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `warnings`
--
ALTER TABLE `warnings`
  MODIFY `WarnID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `weaponslog`
--
ALTER TABLE `weaponslog`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- Constraints for dumped tables
--

--
-- Constraints for table `banlist`
--
ALTER TABLE `banlist`
  ADD CONSTRAINT `banlist_ibfk_1` FOREIGN KEY (`userID`) REFERENCES `players` (`ID`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
