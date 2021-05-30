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
-- Table structure for table `version`
--

DROP TABLE IF EXISTS `version`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `version` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `developer` varchar(25) NOT NULL,
  `text` varchar(128) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=37 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `version`
--

LOCK TABLES `version` WRITE;
/*!40000 ALTER TABLE `version` DISABLE KEYS */;
INSERT INTO `version` VALUES (14,'Fred Heinrich','Fixes detective job reload time'),(15,'Fred Heinrich','Fixes /fare while imprisoned'),(16,'Fred Heinrich','Fixes issue with gates saving where the gate ID is above 999.'),(17,'Fred Heinrich','Fixes /gotogate for IDs above 999'),(18,'Fred Heinrich','Fixes lock bug when new vehicles are created (the one that resets your player\'s animation).'),(19,'Fred Heinrich','Fixes BMX engine issue.'),(20,'Fred Heinrich','Fixes lock issue on dynamic vehicles (other). Not /dlock -> is /dlock bugged?'),(23,'Fred Heinrich','Fixes exit freeze for houses (/edithouse freeze [houseid] 1)'),(26,'Fred Heinrich','Fixes bars showing in all virtual worlds (now only current VW the admin is when creating it).'),(27,'Fred Heinrich','Adds /closestgate to get the closest gate within an optional radius'),(28,'Fred Heinrich','Adds unlocking/locking vehicles with hotkey: Y'),(29,'Fred Heinrich','Adds \"Change Owner\" to /editdoor so administrators can set door owners (displays owner in /doorinfo)'),(30,'Fred Heinrich','Adds /doorpass so door owners can set their own door passwords'),(31,'Fred Heinrich','Adds Deagle to ammunation'),(32,'Fred Heinrich','Adds ability to /setannouncement for faction leaders, developers and Administrators. Shows announcement at login.'),(33,'Fred Heinrich','Adds ability to lock announcement setting for faction leaders (/lockannouncement).'),(35,'Fred Heinrich','ETTORE: Adds a location which /ad is required, location requirement can be toggled by administrators (/togadrange'),(36,'Fred Heinrich','SMO: Fixes objects disappearing for i.e: boomboxes, weed, gates, furniture.');
/*!40000 ALTER TABLE `version` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-05-06  7:58:41
