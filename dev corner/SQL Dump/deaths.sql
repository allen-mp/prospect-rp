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

ALTER TABLE `deaths`
  ADD PRIMARY KEY (`ID`);

ALTER TABLE `deaths`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
