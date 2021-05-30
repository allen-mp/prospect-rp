CREATE TABLE `familysafelogs` (
  `Player` varchar(24) NOT NULL,
  `PlayerSQL` int(11) NOT NULL,
  `Action` varchar(128) NOT NULL,
  `FamilyID` int(11) NOT NULL,
  `Date` varchar(11) DEFAULT NULL,
  `ID` int(11) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

ALTER TABLE `familysafelogs`
  ADD PRIMARY KEY (`ID`);
  
  ALTER TABLE `familysafelogs`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;