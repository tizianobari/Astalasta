-- MySQL dump 10.14  Distrib 5.5.39-MariaDB, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: Astalasta
-- ------------------------------------------------------
-- Server version	5.5.39-MariaDB-0ubuntu0.14.04.1-log

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
-- Table structure for table `Articoli`
--

DROP TABLE IF EXISTS `Articoli`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Articoli` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nome` char(30) NOT NULL,
  `descrizione` char(50) DEFAULT NULL,
  `prezzoIniziale` decimal(9,2) NOT NULL,
  `prezzoAttuale` decimal(9,2) NOT NULL,
  `dataInizio` datetime DEFAULT NULL,
  `dataFine` datetime DEFAULT NULL,
  `rifFoto` int(11) DEFAULT NULL,
  `rifCliente` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Articoli`
--

LOCK TABLES `Articoli` WRITE;
/*!40000 ALTER TABLE `Articoli` DISABLE KEYS */;
INSERT INTO `Articoli` VALUES (1,'Fiori bianchi','bellissimi fiori',1.20,2.96,'2015-03-24 22:00:40','2015-03-29 00:00:00',NULL,NULL),(2,'pippone','prov',2.00,2.03,'2015-03-26 02:12:22','2015-04-02 02:12:22',NULL,NULL);
/*!40000 ALTER TABLE `Articoli` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Clienti`
--

DROP TABLE IF EXISTS `Clienti`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Clienti` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nome` char(20) NOT NULL,
  `cognome` char(30) NOT NULL,
  `dataNascita` date DEFAULT NULL,
  `username` char(32) DEFAULT NULL,
  `password` char(32) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `username` (`username`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Clienti`
--

LOCK TABLES `Clienti` WRITE;
/*!40000 ALTER TABLE `Clienti` DISABLE KEYS */;
INSERT INTO `Clienti` VALUES (1,'Tiziano','Bari','1996-11-15','189bbbb00c5f1fb7fba9ad9285f193d1','7921d2e27263c075cb14731c3f3abca0');
/*!40000 ALTER TABLE `Clienti` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Foto`
--

DROP TABLE IF EXISTS `Foto`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Foto` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `fotografia` mediumblob,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Foto`
--

LOCK TABLES `Foto` WRITE;
/*!40000 ALTER TABLE `Foto` DISABLE KEYS */;
/*!40000 ALTER TABLE `Foto` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Puntate`
--

DROP TABLE IF EXISTS `Puntate`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Puntate` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `prezzo` decimal(9,2) NOT NULL,
  `data` datetime NOT NULL,
  `rifCliente` int(11) NOT NULL,
  `rifArticolo` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=84 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Puntate`
--

LOCK TABLES `Puntate` WRITE;
/*!40000 ALTER TABLE `Puntate` DISABLE KEYS */;
INSERT INTO `Puntate` VALUES (1,1.21,'2015-03-25 20:01:06',1,1),(2,1.21,'2015-03-25 20:21:30',1,1),(3,2.22,'2015-03-25 20:23:08',1,1),(4,2.23,'2015-03-25 20:31:47',1,1),(5,2.24,'2015-03-25 22:27:16',1,1),(6,2.25,'2015-03-25 22:29:01',1,1),(7,2.25,'2015-03-25 22:29:05',1,1),(8,2.26,'2015-03-25 22:31:42',1,1),(9,2.27,'2015-03-25 22:34:06',1,1),(10,2.28,'2015-03-25 22:35:31',1,1),(11,2.29,'2015-03-25 22:46:03',1,1),(12,2.30,'2015-03-25 22:50:36',1,1),(13,2.31,'2015-03-25 22:53:48',1,1),(14,2.32,'2015-03-25 23:01:32',1,1),(15,2.33,'2015-03-25 23:02:59',1,1),(16,2.34,'2015-03-25 23:07:18',1,1),(17,2.35,'2015-03-25 23:07:52',1,1),(18,2.36,'2015-03-25 23:09:28',1,1),(19,2.37,'2015-03-25 23:17:21',1,1),(20,2.38,'2015-03-25 23:19:30',1,1),(21,2.39,'2015-03-25 23:21:15',1,1),(22,2.40,'2015-03-25 23:23:47',1,1),(23,2.41,'2015-03-25 23:26:46',1,1),(24,2.42,'2015-03-25 23:28:19',1,1),(25,2.43,'2015-03-25 23:31:23',1,1),(26,2.44,'2015-03-25 23:34:29',1,1),(27,2.45,'2015-03-25 23:34:31',1,1),(28,2.45,'2015-03-25 23:34:43',1,1),(29,2.45,'2015-03-25 23:34:44',1,1),(30,2.46,'2015-03-25 23:35:39',1,1),(31,2.47,'2015-03-25 23:35:39',1,1),(32,2.48,'2015-03-25 23:35:40',1,1),(33,2.49,'2015-03-25 23:35:41',1,1),(34,2.50,'2015-03-25 23:35:41',1,1),(35,2.51,'2015-03-25 23:35:41',1,1),(36,2.52,'2015-03-25 23:35:41',1,1),(37,2.53,'2015-03-25 23:35:41',1,1),(38,2.54,'2015-03-25 23:35:42',1,1),(39,2.55,'2015-03-25 23:35:42',1,1),(40,2.56,'2015-03-25 23:35:42',1,1),(41,2.57,'2015-03-25 23:35:42',1,1),(42,2.58,'2015-03-25 23:35:42',1,1),(43,2.59,'2015-03-25 23:35:42',1,1),(44,2.60,'2015-03-25 23:35:42',1,1),(45,2.61,'2015-03-25 23:35:42',1,1),(46,2.62,'2015-03-25 23:35:43',1,1),(47,2.63,'2015-03-25 23:35:43',1,1),(48,2.64,'2015-03-25 23:35:43',1,1),(49,2.65,'2015-03-25 23:35:43',1,1),(50,2.66,'2015-03-25 23:35:43',1,1),(51,2.67,'2015-03-25 23:35:43',1,1),(52,2.68,'2015-03-25 23:37:48',1,1),(53,2.68,'2015-03-25 23:37:52',1,1),(54,2.68,'2015-03-25 23:37:53',1,1),(55,2.68,'2015-03-25 23:37:53',1,1),(56,2.70,'2015-03-25 23:37:55',1,1),(57,2.70,'2015-03-25 23:37:56',1,1),(58,2.70,'2015-03-25 23:37:58',1,1),(59,2.72,'2015-03-25 23:38:01',1,1),(60,2.73,'2015-03-25 23:39:03',1,1),(61,2.74,'2015-03-25 23:40:37',1,1),(62,2.75,'2015-03-25 23:40:42',1,1),(63,2.74,'2015-03-25 23:41:01',1,1),(64,2.76,'2015-03-25 23:41:08',1,1),(65,2.74,'2015-03-25 23:41:13',1,1),(66,2.76,'2015-03-25 23:41:17',1,1),(67,2.76,'2015-03-25 23:41:22',1,1),(68,2.79,'2015-03-25 23:41:25',1,1),(69,2.80,'2015-03-25 23:45:31',1,1),(70,2.81,'2015-03-25 23:49:00',1,1),(71,2.82,'2015-03-25 23:57:58',1,1),(72,2.84,'2015-03-25 23:58:35',1,1),(73,2.85,'2015-03-26 00:00:28',1,1),(74,2.86,'2015-03-26 00:05:22',1,1),(75,2.87,'2015-03-26 00:07:09',1,1),(76,2.88,'2015-03-26 00:07:10',1,1),(77,2.92,'2015-03-26 00:07:14',1,1),(78,2.94,'2015-03-26 00:07:16',1,1),(79,2.95,'2015-03-26 02:14:52',1,1),(80,2.01,'2015-03-26 02:14:53',1,2),(81,2.02,'2015-03-26 02:14:54',1,2),(82,2.03,'2015-03-26 02:14:54',1,2),(83,2.96,'2015-03-26 02:14:55',1,1);
/*!40000 ALTER TABLE `Puntate` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-03-26  2:17:11
