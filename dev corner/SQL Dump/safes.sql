-- phpMyAdmin SQL Dump
-- version 4.5.1
-- http://www.phpmyadmin.net
--
-- Host: 127.0.0.1
-- Generation Time: Feb 07, 2018 at 09:07 PM
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

--
-- Indexes for dumped tables
--

--
-- Indexes for table `safes`
--
ALTER TABLE `safes`
  ADD PRIMARY KEY (`safeid`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `safes`
--
ALTER TABLE `safes`
  MODIFY `safeid` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=23;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
