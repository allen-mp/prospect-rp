-- phpMyAdmin SQL Dump
-- version 4.5.1
-- http://www.phpmyadmin.net
--
-- Host: 127.0.0.1
-- Generation Time: Jan 09, 2018 at 02:00 AM
-- Server version: 10.1.13-MariaDB
-- PHP Version: 5.6.21

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `sarp_game`
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

--
-- Dumping data for table `activitylog`
--

INSERT INTO `activitylog` (`ID`, `UserID`, `Minutes`, `Date`) VALUES
(1787072, 7372, 1, '2018-01-08'),
(1787073, 7372, 1, '2018-01-09'),
(1787074, 7373, 1, '2018-01-09'),
(1787075, 7374, 2, '2018-01-09');

-- --------------------------------------------------------

--
-- Table structure for table `banlist`
--

CREATE TABLE `banlist` (
  `ID` int(11) NOT NULL,
  `Name` varchar(32) NOT NULL,
  `Key_` varchar(64) NOT NULL,
  `IP` varchar(32) NOT NULL,
  `Reason` varchar(255) NOT NULL,
  `BannedBy` varchar(32) NOT NULL,
  `Date` date NOT NULL,
  `Expires` date NOT NULL,
  `Banned` tinyint(1) NOT NULL,
  `userID` int(11) UNSIGNED DEFAULT NULL,
  `Serial` varchar(128) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

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
-- Table structure for table `furniture`
--

CREATE TABLE `furniture` (
  `ID` int(11) UNSIGNED NOT NULL,
  `Active` tinyint(1) NOT NULL,
  `Key_` varchar(32) NOT NULL,
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
  `OwnerID` bigint(20) UNSIGNED NOT NULL DEFAULT '0',
  `Name` varchar(50) NOT NULL DEFAULT 'Unnamed hotel'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `houses`
--

CREATE TABLE `houses` (
  `HouseID` int(11) NOT NULL,
  `OwnerID` int(11) UNSIGNED NOT NULL,
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
  `Locked` tinyint(1) NOT NULL,
  `Alarm` tinyint(1) NOT NULL,
  `AlarmStatus` tinyint(1) NOT NULL,
  `Cash` int(11) NOT NULL,
  `Materials` int(11) NOT NULL,
  `Pot` int(11) NOT NULL,
  `Crack` int(11) NOT NULL,
  `Gun1` tinyint(2) NOT NULL,
  `Gun2` tinyint(2) NOT NULL,
  `Gun3` tinyint(2) NOT NULL,
  `Gun4` tinyint(2) NOT NULL,
  `Tag` varchar(32) NOT NULL,
  `Level` tinyint(2) NOT NULL,
  `Ammo4` int(11) NOT NULL DEFAULT '0',
  `Ammo3` int(11) NOT NULL DEFAULT '0',
  `Ammo2` int(11) NOT NULL DEFAULT '0',
  `Ammo1` int(11) NOT NULL DEFAULT '0',
  `Freeze` tinyint(1) NOT NULL,
  `HasFurniture` tinyint(1) NOT NULL DEFAULT '0',
  `Cannabis` int(11) DEFAULT NULL,
  `Cocaine` int(11) DEFAULT NULL,
  `Meth` int(11) DEFAULT NULL
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
  `RealName` varchar(45) NOT NULL
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

--
-- Dumping data for table `paylog`
--

INSERT INTO `paylog` (`ID`, `Date`, `Info`) VALUES
(134, '2018-01-09', 'Possible Moneyfarm: Calvin Catt just left with $0 cash and $0 cash in bank.');

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
  `Name` varchar(32) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL,
  `Key_` varchar(64) NOT NULL,
  `Level` int(11) UNSIGNED NOT NULL,
  `AdminLvl` int(11) UNSIGNED NOT NULL,
  `AdminLevel` int(11) UNSIGNED NOT NULL,
  `SecurityKey` int(11) UNSIGNED NOT NULL,
  `Stealthed` tinyint(1) UNSIGNED NOT NULL,
  `Band` tinyint(1) UNSIGNED NOT NULL,
  `PermBand` tinyint(1) UNSIGNED NOT NULL,
  `BanReason` varchar(255) NOT NULL,
  `PrisonReason` varchar(255) NOT NULL,
  `AdminJailed` tinyint(1) UNSIGNED NOT NULL,
  `PrisonedBy` varchar(32) NOT NULL,
  `Warnings` tinyint(1) UNSIGNED NOT NULL,
  `Disabled` tinyint(1) UNSIGNED NOT NULL,
  `Bleeding` tinyint(4) NOT NULL DEFAULT '0',
  `DonateRank` tinyint(1) UNSIGNED NOT NULL,
  `Tag` tinyint(1) UNSIGNED NOT NULL,
  `VIPJoinDate` varchar(32) NOT NULL,
  `VIPExpDate` varchar(32) NOT NULL,
  `BanAppealer` tinyint(1) UNSIGNED NOT NULL,
  `GangModd` tinyint(1) UNSIGNED NOT NULL,
  `EventModd` tinyint(1) UNSIGNED NOT NULL,
  `ComplaintModd` tinyint(1) UNSIGNED NOT NULL,
  `ShopMod` tinyint(1) UNSIGNED NOT NULL,
  `BizMod` tinyint(1) UNSIGNED NOT NULL,
  `FactionMod` tinyint(1) UNSIGNED NOT NULL,
  `GangDirector` tinyint(1) UNSIGNED NOT NULL,
  `UpgradePoints` int(11) UNSIGNED NOT NULL,
  `ConnectedTime` int(11) NOT NULL,
  `Registered` tinyint(1) UNSIGNED NOT NULL,
  `Sex` tinyint(1) UNSIGNED NOT NULL,
  `Age` tinyint(1) UNSIGNED NOT NULL,
  `Appearance` varchar(255) NOT NULL,
  `Muted` tinyint(1) UNSIGNED NOT NULL,
  `Respect` int(11) UNSIGNED NOT NULL,
  `Money` int(11) NOT NULL,
  `Bank` int(11) NOT NULL,
  `Crimes` int(11) UNSIGNED NOT NULL,
  `Accent` varchar(64) NOT NULL,
  `Kills` int(11) UNSIGNED NOT NULL,
  `Deaths` int(11) UNSIGNED NOT NULL,
  `CHits` int(11) UNSIGNED NOT NULL,
  `FHits` int(11) UNSIGNED NOT NULL,
  `Arrested` int(11) UNSIGNED NOT NULL,
  `LottoNr` smallint(4) UNSIGNED NOT NULL,
  `BiggestFish` int(11) UNSIGNED NOT NULL,
  `Job` int(11) UNSIGNED NOT NULL,
  `Paycheck` smallint(4) UNSIGNED NOT NULL,
  `HeadValue` int(11) UNSIGNED NOT NULL,
  `Jailed` tinyint(1) UNSIGNED NOT NULL,
  `JailCell` tinyint(1) UNSIGNED NOT NULL,
  `JailTime` int(11) UNSIGNED NOT NULL,
  `Leader` tinyint(3) UNSIGNED NOT NULL,
  `Member` tinyint(3) UNSIGNED NOT NULL,
  `FMember` tinyint(3) UNSIGNED NOT NULL,
  `Rank` tinyint(3) UNSIGNED NOT NULL,
  `RadioStation` tinyint(3) UNSIGNED NOT NULL,
  `Instructor` tinyint(1) UNSIGNED NOT NULL,
  `Passes` tinyint(3) UNSIGNED NOT NULL,
  `Fails` tinyint(1) UNSIGNED NOT NULL,
  `DetSkill` int(11) UNSIGNED NOT NULL,
  `SexSkill` int(11) UNSIGNED NOT NULL,
  `BoxSkill` int(11) UNSIGNED NOT NULL,
  `LawSkill` int(11) UNSIGNED NOT NULL,
  `MechSkill` int(11) UNSIGNED NOT NULL,
  `JackSkill` int(11) UNSIGNED NOT NULL,
  `CarSkill` int(11) UNSIGNED NOT NULL,
  `DrugsSkill` int(11) UNSIGNED NOT NULL,
  `ArmsSkill` int(11) UNSIGNED NOT NULL,
  `SmugglerSkill` int(11) UNSIGNED NOT NULL,
  `ThieveSkill` int(11) UNSIGNED NOT NULL,
  `FishSkill` int(11) UNSIGNED NOT NULL,
  `FightingStyle` int(11) UNSIGNED NOT NULL,
  `pHealth` tinyint(3) UNSIGNED NOT NULL,
  `pArmor` tinyint(3) UNSIGNED NOT NULL,
  `pSHP` tinyint(3) UNSIGNED NOT NULL,
  `pStrength` tinyint(3) UNSIGNED NOT NULL,
  `pSHealth` tinyint(3) UNSIGNED NOT NULL,
  `InteriorID` tinyint(3) UNSIGNED NOT NULL,
  `Local` int(11) UNSIGNED NOT NULL,
  `VirtualWorld` int(11) UNSIGNED NOT NULL,
  `Model` int(11) UNSIGNED NOT NULL,
  `Clothes` int(11) UNSIGNED NOT NULL,
  `PhoneNr` int(11) NOT NULL,
  `PhoneType` int(11) UNSIGNED NOT NULL,
  `BizKey` int(11) UNSIGNED NOT NULL,
  `BizFixed` int(11) UNSIGNED NOT NULL,
  `SetVehSpawn` int(11) NOT NULL,
  `CarLic` tinyint(1) UNSIGNED NOT NULL,
  `FlyLic` tinyint(1) UNSIGNED NOT NULL,
  `BoatLic` tinyint(1) UNSIGNED NOT NULL,
  `FishLic` tinyint(1) UNSIGNED NOT NULL,
  `GunLic` tinyint(1) UNSIGNED NOT NULL,
  `Gun0` tinyint(3) UNSIGNED NOT NULL,
  `Gun1` tinyint(3) UNSIGNED NOT NULL,
  `Gun2` tinyint(3) UNSIGNED NOT NULL,
  `Gun3` tinyint(3) UNSIGNED NOT NULL,
  `Gun4` tinyint(3) UNSIGNED NOT NULL,
  `Gun5` tinyint(3) UNSIGNED NOT NULL,
  `Gun6` tinyint(3) UNSIGNED NOT NULL,
  `Gun7` tinyint(3) UNSIGNED NOT NULL,
  `Gun8` tinyint(3) UNSIGNED NOT NULL,
  `Gun9` tinyint(3) UNSIGNED NOT NULL,
  `Gun10` tinyint(3) UNSIGNED NOT NULL,
  `Gun11` tinyint(3) UNSIGNED NOT NULL,
  `Gun12` tinyint(3) UNSIGNED NOT NULL,
  `CarTime` int(11) UNSIGNED NOT NULL,
  `DrugsTime` int(11) UNSIGNED NOT NULL,
  `LawyerTime` int(11) UNSIGNED NOT NULL,
  `LawyerFreeTime` int(11) UNSIGNED NOT NULL,
  `MechTime` int(11) UNSIGNED NOT NULL,
  `SexTime` int(11) UNSIGNED NOT NULL,
  `PlantTime` int(11) UNSIGNED NOT NULL,
  `RepairTime` int(11) UNSIGNED NOT NULL,
  `RobTime` int(11) UNSIGNED NOT NULL,
  `PayDay` int(11) NOT NULL,
  `PayDayHad` int(11) NOT NULL,
  `Wins` int(11) UNSIGNED NOT NULL,
  `Loses` int(11) UNSIGNED NOT NULL,
  `Tutorial` tinyint(1) UNSIGNED NOT NULL,
  `OnDuty` tinyint(1) UNSIGNED NOT NULL,
  `Hospital` tinyint(1) UNSIGNED NOT NULL,
  `Adjustable` tinyint(1) UNSIGNED NOT NULL,
  `Married` tinyint(1) UNSIGNED NOT NULL,
  `MarriedTo` varchar(32) NOT NULL,
  `ContractBy` varchar(32) NOT NULL,
  `IP` varchar(32) NOT NULL,
  `Country` varchar(64) NOT NULL,
  `WantedLevel` tinyint(3) UNSIGNED NOT NULL,
  `NewbieMuted` tinyint(1) UNSIGNED NOT NULL,
  `NewbieMutedTime` int(11) UNSIGNED NOT NULL,
  `NewbieWarns` tinyint(3) UNSIGNED NOT NULL,
  `AdMuted` tinyint(1) UNSIGNED NOT NULL,
  `BugMuted` tinyint(1) UNSIGNED NOT NULL,
  `SPos_x` float DEFAULT NULL,
  `SPos_y` float DEFAULT NULL,
  `SPos_z` float DEFAULT NULL,
  `SPos_r` float DEFAULT NULL,
  `HelperLevel` tinyint(3) UNSIGNED NOT NULL,
  `DevLevel` tinyint(3) UNSIGNED NOT NULL,
  `TesterLevel` tinyint(3) UNSIGNED NOT NULL,
  `Famed` tinyint(1) UNSIGNED NOT NULL,
  `Enlightened` tinyint(1) UNSIGNED NOT NULL,
  `BanDay` tinyint(2) UNSIGNED NOT NULL,
  `BanMonth` tinyint(2) UNSIGNED NOT NULL,
  `BanYear` smallint(4) UNSIGNED NOT NULL,
  `BanTime` int(11) UNSIGNED NOT NULL,
  `GottenGift` tinyint(1) UNSIGNED NOT NULL,
  `GottenTimeLeft` int(11) UNSIGNED NOT NULL,
  `pHouseID` smallint(11) NOT NULL,
  `hTicket` tinyint(1) UNSIGNED NOT NULL,
  `gTicket` tinyint(3) UNSIGNED NOT NULL,
  `gRefund` tinyint(1) UNSIGNED NOT NULL,
  `Economist` tinyint(1) UNSIGNED NOT NULL,
  `Mission_spawn` tinyint(3) UNSIGNED NOT NULL,
  `Mission_mafia` tinyint(3) UNSIGNED NOT NULL,
  `Mission_triad` tinyint(3) UNSIGNED NOT NULL,
  `Mission_smoke` tinyint(3) UNSIGNED NOT NULL,
  `Mission_ryder` tinyint(3) UNSIGNED NOT NULL,
  `Mission_taxi` tinyint(3) UNSIGNED NOT NULL,
  `Mission_redneck3` tinyint(3) UNSIGNED NOT NULL,
  `Mission_cargo` tinyint(3) UNSIGNED NOT NULL,
  `HAFakeBadge` tinyint(1) UNSIGNED NOT NULL,
  `Badge` tinyint(1) UNSIGNED NOT NULL,
  `Accessory1` mediumint(8) UNSIGNED NOT NULL,
  `Accessory2` mediumint(8) UNSIGNED NOT NULL,
  `Accessory3` mediumint(8) UNSIGNED NOT NULL,
  `Accessory4` mediumint(8) UNSIGNED NOT NULL,
  `Accessory5` mediumint(8) UNSIGNED NOT NULL,
  `Accessory6` mediumint(8) UNSIGNED NOT NULL,
  `Accessory7` mediumint(8) UNSIGNED NOT NULL,
  `AccessoryB1` tinyint(2) UNSIGNED NOT NULL,
  `AccessoryB2` tinyint(2) UNSIGNED NOT NULL,
  `AccessoryB3` tinyint(2) UNSIGNED NOT NULL,
  `AccessoryB4` tinyint(2) UNSIGNED NOT NULL,
  `AccessoryB5` tinyint(2) UNSIGNED NOT NULL,
  `AccessoryB6` tinyint(2) UNSIGNED NOT NULL,
  `AccessoryB7` tinyint(2) UNSIGNED NOT NULL,
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
  `AccessorysC1` mediumint(8) UNSIGNED NOT NULL,
  `AccessorysC2` mediumint(8) UNSIGNED NOT NULL,
  `AccessorysC3` mediumint(8) UNSIGNED NOT NULL,
  `AccessorysC4` mediumint(8) UNSIGNED NOT NULL,
  `AccessorysC5` mediumint(8) UNSIGNED NOT NULL,
  `AccessorysC6` mediumint(8) UNSIGNED NOT NULL,
  `AccessorysC7` mediumint(8) UNSIGNED NOT NULL,
  `CKey1` smallint(5) UNSIGNED NOT NULL,
  `CKey2` smallint(5) UNSIGNED NOT NULL,
  `Mapper` tinyint(1) UNSIGNED NOT NULL,
  `customTag` varchar(32) NOT NULL,
  `customRank` varchar(32) NOT NULL,
  `Graphic` tinyint(1) UNSIGNED NOT NULL,
  `LastLogin` varchar(64) NOT NULL,
  `isinHosp` tinyint(1) UNSIGNED NOT NULL,
  `HasMap` tinyint(1) UNSIGNED NOT NULL,
  `JailNumber` int(11) NOT NULL,
  `NewCarLic` tinyint(1) UNSIGNED NOT NULL,
  `NewFlyLic` tinyint(1) UNSIGNED NOT NULL,
  `NewBoatLic` tinyint(1) UNSIGNED NOT NULL,
  `NewGunLic` tinyint(1) UNSIGNED NOT NULL,
  `SueUnix` int(11) NOT NULL,
  `SueNotice` int(11) NOT NULL,
  `AllowedArmory` tinyint(1) UNSIGNED NOT NULL,
  `WalkStyle` tinyint(3) UNSIGNED NOT NULL,
  `Fishes` int(11) NOT NULL,
  `Products` int(11) NOT NULL,
  `Materials` int(11) UNSIGNED NOT NULL,
  `Pot` int(11) UNSIGNED NOT NULL,
  `Crack` int(11) UNSIGNED NOT NULL,
  `Seeds` int(11) UNSIGNED NOT NULL,
  `Calculator` int(11) UNSIGNED NOT NULL,
  `Suitcase` int(11) UNSIGNED NOT NULL,
  `SuitCrack` int(11) UNSIGNED NOT NULL,
  `SuitMats` int(11) UNSIGNED NOT NULL,
  `SuitWeapon` int(11) UNSIGNED NOT NULL,
  `SuitPot` int(11) UNSIGNED NOT NULL,
  `SuitMoney` int(11) UNSIGNED NOT NULL,
  `Phonebook` int(11) UNSIGNED NOT NULL,
  `FishRod` int(11) UNSIGNED NOT NULL,
  `Bait` int(11) UNSIGNED NOT NULL,
  `BaitAmount` int(11) UNSIGNED NOT NULL,
  `Line` int(11) UNSIGNED NOT NULL,
  `iPod` int(11) UNSIGNED NOT NULL,
  `Dice` int(11) UNSIGNED NOT NULL,
  `Spraycan` int(11) UNSIGNED NOT NULL,
  `Screw` int(11) UNSIGNED NOT NULL,
  `Rope` int(11) UNSIGNED NOT NULL,
  `Cigars` int(11) UNSIGNED NOT NULL,
  `Sprunk` int(11) UNSIGNED NOT NULL,
  `Tikis` int(11) UNSIGNED NOT NULL,
  `WT` tinyint(1) UNSIGNED NOT NULL,
  `WTc` int(11) UNSIGNED NOT NULL,
  `Radio` int(11) UNSIGNED NOT NULL,
  `Handtazer` tinyint(3) UNSIGNED NOT NULL,
  `tazerGun` tinyint(3) UNSIGNED NOT NULL,
  `Bombs` tinyint(1) UNSIGNED NOT NULL,
  `Poison` tinyint(1) UNSIGNED NOT NULL,
  `Glasses` tinyint(1) UNSIGNED NOT NULL,
  `Bandana` tinyint(1) UNSIGNED NOT NULL,
  `Helmet` tinyint(1) UNSIGNED NOT NULL,
  `Mask` tinyint(3) UNSIGNED NOT NULL,
  `Blindfolds` int(11) UNSIGNED NOT NULL,
  `Deck` tinyint(1) UNSIGNED NOT NULL,
  `Stereo` tinyint(3) UNSIGNED NOT NULL,
  `Matches` tinyint(3) UNSIGNED NOT NULL,
  `Jerrycan` tinyint(1) UNSIGNED NOT NULL,
  `MP5Laser` tinyint(1) UNSIGNED NOT NULL,
  `M4Laser` tinyint(1) UNSIGNED NOT NULL,
  `AKLaser` tinyint(1) UNSIGNED NOT NULL,
  `SpasLaser` tinyint(1) UNSIGNED NOT NULL,
  `DeagleLaser` tinyint(1) UNSIGNED NOT NULL,
  `AddressBook` tinyint(1) UNSIGNED NOT NULL,
  `DeathState` tinyint(3) UNSIGNED NOT NULL,
  `GearFixed` int(11) UNSIGNED NOT NULL,
  `Division` int(11) NOT NULL DEFAULT '0',
  `SpawnPos` int(11) NOT NULL DEFAULT '0',
  `LogUnix` int(11) NOT NULL DEFAULT '0',
  `HelperReports` int(11) NOT NULL DEFAULT '0',
  `TesterReports` int(11) NOT NULL DEFAULT '0',
  `WasteSkill` int(11) UNSIGNED NOT NULL DEFAULT '0',
  `HouseFire` int(11) NOT NULL DEFAULT '0',
  `TikiRefund` int(11) NOT NULL DEFAULT '0',
  `ImperiumRank` int(11) NOT NULL DEFAULT '0',
  `Wire` int(11) NOT NULL DEFAULT '0',
  `NewMariLicense` tinyint(1) UNSIGNED NOT NULL,
  `HitmanRank` int(1) NOT NULL DEFAULT '0',
  `SuitAmmo` int(11) NOT NULL DEFAULT '0',
  `Group` int(1) NOT NULL DEFAULT '0',
  `GroupRank` int(1) NOT NULL DEFAULT '0',
  `Serial` varchar(128) NOT NULL,
  `avatar_image_url` varchar(128) NOT NULL DEFAULT 'players/avatars/default/avatar_small.png',
  `GameAccount` int(1) NOT NULL DEFAULT '0',
  `Password` varchar(64) NOT NULL,
  `Salt` varchar(32) NOT NULL,
  `ContractReason` varchar(32) NOT NULL,
  `CKey3` int(11) DEFAULT NULL,
  `CKey4` int(11) DEFAULT NULL,
  `lastCarID` int(11) DEFAULT NULL,
  `maxcarslots` int(11) DEFAULT NULL,
  `C2` int(11) DEFAULT NULL,
  `MedicCount` int(11) NOT NULL,
  `FakeFaction` tinyint(1) NOT NULL,
  `FactionKickTime` int(11) NOT NULL,
  `MDCLogged` tinyint(1) DEFAULT NULL,
  `isBOLO` tinyint(1) DEFAULT NULL,
  `HasVoted1` int(1) DEFAULT NULL,
  `HasVoted2` int(1) DEFAULT NULL,
  `HasVoted3` int(1) DEFAULT NULL,
  `HasMDC` int(1) NOT NULL,
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
  `OldSchool` int(11) NOT NULL,
  `DriverPoints` int(1) DEFAULT NULL,
  `NewPassword` varchar(130) NOT NULL,
  `NewSalt` varchar(33) NOT NULL,
  `NewbRequestUnmute` int(11) NOT NULL,
  `Gag` tinyint(1) NOT NULL DEFAULT '0',
  `TogNewbie` int(1) DEFAULT NULL,
  `PresentCount` int(20) DEFAULT NULL,
  `HidingStatus` int(2) DEFAULT NULL,
  `ReportMuted` int(5) DEFAULT NULL,
  `TogAutoChat` int(2) DEFAULT NULL,
  `Insurance` int(1) DEFAULT NULL,
  `Origin` varchar(128) NOT NULL,
  `FactionDirector` TINYINT(1) DEFAULT NULL,
  `Xanax` INT DEFAULT NULL,
  `NSB` TINYINT(1) DEFAULT NULL,
  `FakeRank` TINYINT(1) DEFAULT NULL,
  `PizzaSkill` INT(11) DEFAULT NULL,
  `RubberBullets` INT(11) DEFAULT NULL,
  `BadgeNumber` INT(11) DEFAULT NULL,
  `Beer` INT(11) DEFAULT NULL,
  `PropDate` TEXT NOT NULL,
  `Recruiter` INT NOT NULL
  
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `players`
--

INSERT INTO `players` (`ID`, `Name`, `Key_`, `Level`, `AdminLvl`, `AdminLevel`, `SecurityKey`, `Stealthed`, `Band`, `PermBand`, `BanReason`, `PrisonReason`, `AdminJailed`, `PrisonedBy`, `Warnings`, `Disabled`, `Bleeding`, `DonateRank`, `Tag`, `VIPJoinDate`, `VIPExpDate`, `BanAppealer`, `GangModd`, `EventModd`, `ComplaintModd`, `ShopMod`, `BizMod`, `FactionMod`, `GangDirector`, `UpgradePoints`, `ConnectedTime`, `Registered`, `Sex`, `Age`, `Appearance`, `Muted`, `Respect`, `Money`, `Bank`, `Crimes`, `Accent`, `Kills`, `Deaths`, `CHits`, `FHits`, `Arrested`, `LottoNr`, `BiggestFish`, `Job`, `Paycheck`, `HeadValue`, `Jailed`, `JailCell`, `JailTime`, `Leader`, `Member`, `FMember`, `Rank`, `RadioStation`, `Instructor`, `Passes`, `Fails`, `DetSkill`, `SexSkill`, `BoxSkill`, `LawSkill`, `MechSkill`, `JackSkill`, `CarSkill`, `DrugsSkill`, `ArmsSkill`, `SmugglerSkill`, `ThieveSkill`, `FishSkill`, `FightingStyle`, `pHealth`, `pArmor`, `pSHP`, `pStrength`, `pSHealth`, `InteriorID`, `Local`, `VirtualWorld`, `Model`, `Clothes`, `PhoneNr`, `PhoneType`, `BizKey`, `BizFixed`, `SetVehSpawn`, `CarLic`, `FlyLic`, `BoatLic`, `FishLic`, `GunLic`, `Gun0`, `Gun1`, `Gun2`, `Gun3`, `Gun4`, `Gun5`, `Gun6`, `Gun7`, `Gun8`, `Gun9`, `Gun10`, `Gun11`, `Gun12`, `CarTime`, `DrugsTime`, `LawyerTime`, `LawyerFreeTime`, `MechTime`, `SexTime`, `PlantTime`, `RepairTime`, `RobTime`, `PayDay`, `PayDayHad`, `Wins`, `Loses`, `Tutorial`, `OnDuty`, `Hospital`, `Adjustable`, `Married`, `MarriedTo`, `ContractBy`, `IP`, `Country`, `WantedLevel`, `NewbieMuted`, `NewbieMutedTime`, `NewbieWarns`, `AdMuted`, `BugMuted`, `SPos_x`, `SPos_y`, `SPos_z`, `SPos_r`, `HelperLevel`, `DevLevel`, `TesterLevel`, `Famed`, `Enlightened`, `BanDay`, `BanMonth`, `BanYear`, `BanTime`, `GottenGift`, `GottenTimeLeft`, `pHouseID`, `hTicket`, `gTicket`, `gRefund`, `Economist`, `Mission_spawn`, `Mission_mafia`, `Mission_triad`, `Mission_smoke`, `Mission_ryder`, `Mission_taxi`, `Mission_redneck3`, `Mission_cargo`, `HAFakeBadge`, `Badge`, `Accessory1`, `Accessory2`, `Accessory3`, `Accessory4`, `Accessory5`, `Accessory6`, `Accessory7`, `AccessoryB1`, `AccessoryB2`, `AccessoryB3`, `AccessoryB4`, `AccessoryB5`, `AccessoryB6`, `AccessoryB7`, `AccessoryX1`, `AccessoryX2`, `AccessoryX3`, `AccessoryX4`, `AccessoryX5`, `AccessoryX6`, `AccessoryX7`, `AccessoryY1`, `AccessoryY2`, `AccessoryY3`, `AccessoryY4`, `AccessoryY5`, `AccessoryY6`, `AccessoryY7`, `AccessoryZ1`, `AccessoryZ2`, `AccessoryZ3`, `AccessoryZ4`, `AccessoryZ5`, `AccessoryZ6`, `AccessoryZ7`, `AccessoryrX1`, `AccessoryrX2`, `AccessoryrX3`, `AccessoryrX4`, `AccessoryrX5`, `AccessoryrX6`, `AccessoryrX7`, `AccessoryrY1`, `AccessoryrY2`, `AccessoryrY3`, `AccessoryrY4`, `AccessoryrY5`, `AccessoryrY6`, `AccessoryrY7`, `AccessoryrZ1`, `AccessoryrZ2`, `AccessoryrZ3`, `AccessoryrZ4`, `AccessoryrZ5`, `AccessoryrZ6`, `AccessoryrZ7`, `AccessorysX1`, `AccessorysX2`, `AccessorysX3`, `AccessorysX4`, `AccessorysX5`, `AccessorysX6`, `AccessorysX7`, `AccessorysY1`, `AccessorysY2`, `AccessorysY3`, `AccessorysY4`, `AccessorysY5`, `AccessorysY6`, `AccessorysY7`, `AccessorysZ1`, `AccessorysZ2`, `AccessorysZ3`, `AccessorysZ4`, `AccessorysZ5`, `AccessorysZ6`, `AccessorysZ7`, `AccessorysC1`, `AccessorysC2`, `AccessorysC3`, `AccessorysC4`, `AccessorysC5`, `AccessorysC6`, `AccessorysC7`, `CKey1`, `CKey2`, `Mapper`, `customTag`, `customRank`, `Graphic`, `LastLogin`, `isinHosp`, `HasMap`, `JailNumber`, `NewCarLic`, `NewFlyLic`, `NewBoatLic`, `NewGunLic`, `SueUnix`, `SueNotice`, `AllowedArmory`, `WalkStyle`, `Fishes`, `Products`, `Materials`, `Pot`, `Crack`, `Seeds`, `Calculator`, `Suitcase`, `SuitCrack`, `SuitMats`, `SuitWeapon`, `SuitPot`, `SuitMoney`, `Phonebook`, `FishRod`, `Bait`, `BaitAmount`, `Line`, `iPod`, `Dice`, `Spraycan`, `Screw`, `Rope`, `Cigars`, `Sprunk`, `Tikis`, `WT`, `WTc`, `Radio`, `Handtazer`, `tazerGun`, `Bombs`, `Poison`, `Glasses`, `Bandana`, `Helmet`, `Mask`, `Blindfolds`, `Deck`, `Stereo`, `Matches`, `Jerrycan`, `MP5Laser`, `M4Laser`, `AKLaser`, `SpasLaser`, `DeagleLaser`, `AddressBook`, `DeathState`, `GearFixed`, `Division`, `SpawnPos`, `LogUnix`, `HelperReports`, `TesterReports`, `WasteSkill`, `HouseFire`, `TikiRefund`, `ImperiumRank`, `Wire`, `NewMariLicense`, `HitmanRank`, `SuitAmmo`, `Group`, `GroupRank`, `Serial`, `avatar_image_url`, `GameAccount`, `Password`, `Salt`, `ContractReason`, `CKey3`, `CKey4`, `lastCarID`, `maxcarslots`, `C2`, `MedicCount`, `FakeFaction`, `FactionKickTime`, `MDCLogged`, `isBOLO`, `HasVoted1`, `HasVoted2`, `HasVoted3`, `HasMDC`, `HasJammer`, `brokenleg`, `Cannabis`, `Cocaine`, `Meth`, `Build`, `EyeColour`, `Ethnicity`, `Height`, `MaskName`, `OldSchool`, `DriverPoints`, `NewPassword`, `NewSalt`, `NewbRequestUnmute`, `Gag`, `TogNewbie`, `PresentCount`, `HidingStatus`, `ReportMuted`, `TogAutoChat`, `Insurance`, `Origin`) VALUES
(7374, 'Calvin_Catt', '', 1, 0, 999999, 1234, 0, 0, 0, '', 'None', 0, 'Nobody', 0, 0, 0, 0, 1, '0/0/0', '0/0/0', 0, 0, 0, 0, 0, 0, 0, 0, 0, 3222, 1, 1, 18, 'None', 0, 5432, 98000, 20000, 0, 'None', 0, 0, 0, 0, 0, 0, 0, 0, 1052, 0, 0, 0, 0, 0, 0, 255, 0, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 100, 0, 0, 0, 0, 4, 999, 20, 299, 0, 5904, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0, 0, 0, 'Nobody', 'Nobody', '127.0.0.1', '', 0, 0, 0, 0, 0, 0, -29.3259, -18.5304, 1003.56, 0.268341, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'None', 'None', 0, 'None', 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1515460289, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'D840AAC0D995A4D4CE8EDEEDC8048DF48CA944F4', 'players/avatars/default/avatar_small.png', 0, '', '', '', NULL, NULL, -1, 3, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 180, '', 0, 0, 'A8638327CB6195C7DB0E24EB0906A3F0BB03A0A96F23122E42169574FC970B927C6C8DC593CD1075391E7C79D976FB856D89D98A77A8E62BBE4231697AF5D926', 'SG?KpACbJ]/6R[K^>rv/>62!ClwIsGw', 0, 0, 0, 0, 0, 0, 0, 0, '');

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
-- Table structure for table `pointsrules`
--

CREATE TABLE `pointsrules` (
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
-- Indexes for table `banlist`
--
ALTER TABLE `banlist`
  ADD PRIMARY KEY (`ID`),
  ADD KEY `banlist_ibfk_1` (`userID`);

--
-- Indexes for table `crimes`
--
ALTER TABLE `crimes`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `divisions`
--
ALTER TABLE `divisions`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `factionbanks`
--
ALTER TABLE `factionbanks`
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
  ADD KEY `OwnerID` (`OwnerID`);
  
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
-- Indexes for table `pointsrules`
--
ALTER TABLE `pointsrules`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `prisons`
--
ALTER TABLE `prisons`
  ADD PRIMARY KEY (`ID`);

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
  MODIFY `ID` int(11) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1787076;
--
-- AUTO_INCREMENT for table `banlist`
--
ALTER TABLE `banlist`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `crimes`
--
ALTER TABLE `crimes`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `divisions`
--
ALTER TABLE `divisions`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `factionbanks`
--
ALTER TABLE `factionbanks`
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
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=10;
--
-- AUTO_INCREMENT for table `hotelrooms`
--
ALTER TABLE `hotelrooms`
  MODIFY `ID` bigint(20) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;
--
-- AUTO_INCREMENT for table `hotels`
--
ALTER TABLE `hotels`
  MODIFY `ID` bigint(20) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=13;
--
-- AUTO_INCREMENT for table `houses`
--
ALTER TABLE `houses`
  MODIFY `HouseID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `maps`
--
ALTER TABLE `maps`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `namechanges`
--
ALTER TABLE `namechanges`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;
--
-- AUTO_INCREMENT for table `newcrimes`
--
ALTER TABLE `newcrimes`
  MODIFY `CrimeID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;
--
-- AUTO_INCREMENT for table `paylog`
--
ALTER TABLE `paylog`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=135;
--
-- AUTO_INCREMENT for table `players`
--
ALTER TABLE `players`
  MODIFY `ID` int(11) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7375;
--
-- AUTO_INCREMENT for table `playervehicles`
--
ALTER TABLE `playervehicles`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;
  --
-- AUTO_INCREMENT for table `pointsrules`
--
ALTER TABLE `pointsrules`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `prisons`
--
ALTER TABLE `prisons`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;
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
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;
--
-- AUTO_INCREMENT for table `version`
--
ALTER TABLE `version`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
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
