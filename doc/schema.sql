SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

CREATE TABLE `aip` (
  `FlightId` varchar(50) NOT NULL,
  `DeviceId` int(11) NOT NULL,
  `PacketId` bigint(20) NOT NULL,
  `TimesStamp` datetime NOT NULL,
  `DI` int(11) NOT NULL,
  `Altitude` int(11) NOT NULL,
  `Rate` double NOT NULL,
  `A1` int(11) default NULL,
  `A2` int(11) default NULL,
  `A3` int(11) default NULL,
  `A4` int(11) default NULL,
  `A5` int(11) default NULL,
  `A6` int(11) default NULL,
  `A7` int(11) default NULL,
  `A8` int(11) default NULL,
  `A9` int(11) default NULL,
  `A10` int(11) default NULL,
  `A11` int(11) default NULL,
  `A12` int(11) default NULL,
  `A13` int(11) default NULL,
  `A14` int(11) default NULL,
  `A15` int(11) default NULL,
  `A16` int(11) default NULL,
  `A17` int(11) default NULL,
  `A18` int(11) default NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `dip` (
  `FlightId` varchar(50) NOT NULL,
  `DeviceId` int(11) NOT NULL,
  `PacketId` bigint(20) NOT NULL,
  `Timestamp` datetime NOT NULL,
  `DI` int(11) default NULL,
  `Altitude` int(11) default NULL,
  `Rate` double default NULL,
  PRIMARY KEY  (`FlightId`,`DeviceId`,`PacketId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `flights` (
  `ID` varchar(50) NOT NULL,
  `Description` varchar(200) default NULL,
  `Timestamp` datetime NOT NULL,
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `gps` (
  `FlightId` varchar(50) NOT NULL,
  `DeviceId` int(11) NOT NULL,
  `PacketId` bigint(20) NOT NULL,
  `Timestamp` datetime NOT NULL,
  `Status` int(11) default NULL,
  `Altitude` int(11) default NULL,
  `Rate` double default NULL,
  `Lat` varchar(10) default NULL,
  `LatRef` varchar(3) default NULL,
  `Lon` varchar(10) default NULL,
  `LonRef` varchar(3) default NULL,
  `Spd` varchar(10) default NULL,
  `Hdg` varchar(10) default NULL,
  PRIMARY KEY  (`FlightId`,`DeviceId`,`PacketId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
