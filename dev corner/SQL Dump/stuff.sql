-- MySQL dump 10.13  Distrib 5.5.41, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: sarp_game
-- ------------------------------------------------------
-- Server version	5.5.41-0+wheezy1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `stuff`
--

DROP TABLE IF EXISTS `stuff`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `stuff` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
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
  `testermotd` varchar(128) DEFAULT 'TESTERMOTD:None',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `stuff`
--

LOCK TABLES `stuff` WRITE;
/*!40000 ALTER TABLE `stuff` DISABLE KEYS */;
INSERT INTO `stuff` VALUES (1,'- {FFFF00}Fixed{FFFFFF} server crashing.\r\n- {FFFF00}Fixed{FFFFFF} \'married to\' not displaying correctly.\r\n- {FFFF00}Fixed{FFFFFF} /contracts not displaying correctly.\r\n- {FFFF00}Fixed{FFFFFF} ametag not being removed on disconnect.\r\n- {FFFF00}Fixed{FFFFFF} prison gates not loading.\r\n- {FFFF00}Fixed{FFFFFF} RRK\'s not working needing a GMX to fix, they are now dynamic.\r\n- {FFFF00}Fixed{FFFFFF} injured text not disappearing on death/revive.\r\n- {FFFF00}Fixed{FFFFFF} an error with the admin whisper system.\r\n\r\n\r\n- {FF0000}Removed{FFFFFF} current mining jobs, will be added in a full update.\r\n\r\n\r\n\r\n- {00FF00}Added{FFFFFF} new commissioner rank for LSPD.\r\n- {00FF00}Added{FFFFFF} registration warnings for admins.\r\n- {00FF00}Added{FFFFFF} property moderators and shop moderators more commands related to their work.\r\n- {00FF00}Added{FFFFFF} an /enter and /exit timer to avoid glitching/exploiting.\r\n- {00FF00}Added{FFFFFF} the ability for custom mappings to be added without need of GMX.\r\n- {00FF00}Added{FFFFFF} a unique system for FBI who want to keep it a secret for now?.\r\n- {00FF00}Added{FFFFFF} a character limit to amechange that could potentially bug your account.\r\n- {00FF00}Added{FFFFFF} new roleplay command /amy.\r\n- {00FF00}Added{FFFFFF} Added a newbie guide at spawn, re-added welcome dialog.\r\n\r\n- {FFFF00}Adjusted{FFFFFF} the way timers work (/report) for example .\r\n- {FFFF00}Adjusted{FFFFFF} all automatic /me, /do and /my\'s (damien_knowles has turned on his engine) will now show as a /ame to avoid spam.\r\n- {FFFF00}Adjusted{FFFFFF} commands such as /showbadge now show as a chatbubble (like /wt).\r\n- {FFFF00}Adjusted{FFFFFF} /stopani.\r\n- {FFFF00}Adjusted{FFFFFF} the NPC system (now CreateActor), should reduce lag and allow more bots in future.\r\n- {FFFF00}Adjusted{FFFFFF} /jobhelp, now merged with /map.',46185,1,163379688,0,9,310,500,33986,105848,500,1000,1000,500,1000,1000,2,3,0,4,1,1,1,1,1,'','');
/*!40000 ALTER TABLE `stuff` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-05-06  7:56:42
