-- MySQL dump 10.13  Distrib 8.0.28, for Linux (x86_64)
--
-- Host: localhost    Database: Access_Control_System
-- ------------------------------------------------------
-- Server version	8.0.28

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `card`
--

DROP TABLE IF EXISTS `card`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `card` (
  `card_uid` int NOT NULL AUTO_INCREMENT,
  `associated_name` varchar(40) NOT NULL,
  `associated_phone_number` varchar(20) NOT NULL,
  `granted_access_level` int NOT NULL,
  `card_code` varchar(40) DEFAULT NULL,
  PRIMARY KEY (`card_uid`),
  KEY `card_code_index` (`card_code`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `card`
--

LOCK TABLES `card` WRITE;
/*!40000 ALTER TABLE `card` DISABLE KEYS */;
INSERT INTO `card` VALUES (1,'Georgi Gostapo','00 359 000 000 000',1,NULL),(2,'Ivan Dali Da','00 359 000 000 000',2,NULL),(3,'Real chip','00 359 000 000 000',2,'c4728803');
/*!40000 ALTER TABLE `card` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `check_attempt`
--

DROP TABLE IF EXISTS `check_attempt`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `check_attempt` (
  `attempt_id` int NOT NULL AUTO_INCREMENT,
  `card` int DEFAULT NULL,
  `time` timestamp NULL DEFAULT NULL,
  `device_name` varchar(40) DEFAULT NULL,
  `status_code` int DEFAULT NULL,
  PRIMARY KEY (`attempt_id`),
  KEY `card` (`card`),
  CONSTRAINT `check_attempt_ibfk_1` FOREIGN KEY (`card`) REFERENCES `card` (`card_uid`) ON DELETE SET NULL
) ENGINE=InnoDB AUTO_INCREMENT=72 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `check_attempt`
--

LOCK TABLES `check_attempt` WRITE;
/*!40000 ALTER TABLE `check_attempt` DISABLE KEYS */;
INSERT INTO `check_attempt` VALUES (1,2,'2022-03-27 19:22:21','SOUTH GATE #2',0),(2,2,'2022-03-27 19:33:31','SOUTH GATE #2',0),(3,2,'2022-03-27 19:33:32','SOUTH GATE #2',0),(4,2,'2022-03-27 19:33:45','SOUTH GATE #2',0),(5,1,'2022-03-27 19:34:05','SOUTH GATE #2',0),(6,1,'2022-03-27 19:34:07','SOUTH GATE #2',0),(7,1,'2022-03-30 18:50:31','SOUTH GATE #3',0),(8,1,'2022-03-30 18:50:31','SOUTH GATE #3',0),(9,1,'2022-03-30 18:50:31','SOUTH GATE #3',0),(10,1,'2022-03-30 18:50:31','SOUTH GATE #3',2),(13,NULL,'2022-03-30 19:24:42','SOUTH GATE #3',1),(14,1,'2022-03-30 19:26:54','SOUTH GATE #3',0),(15,1,'2022-03-30 19:26:54','SOUTH GATE #3',0),(16,1,'2022-03-30 19:26:54','SOUTH GATE #3',0),(17,1,'2022-03-30 19:26:54','SOUTH GATE #3',0),(18,NULL,'2022-03-30 19:26:54','SOUTH GATE #3',1),(19,1,'2022-03-30 19:29:41','SOUTH GATE #3',0),(20,1,'2022-03-30 19:29:41','SOUTH GATE #3',0),(21,1,'2022-03-30 19:29:41','SOUTH GATE #3',0),(22,1,'2022-03-30 19:29:41','SOUTH GATE #3',0),(23,NULL,'2022-03-30 19:29:41','SOUTH GATE #3',1),(24,1,'2022-03-30 19:32:21','SOUTH GATE #3',2),(25,1,'2022-03-30 19:32:31','SOUTH GATE #3',2),(26,1,'2022-03-30 19:32:31','SOUTH GATE #3',2),(27,1,'2022-03-30 19:37:30','SOUTH GATE #3',0),(28,1,'2022-03-30 19:37:30','SOUTH GATE #3',0),(29,1,'2022-03-30 19:37:30','SOUTH GATE #3',0),(30,1,'2022-03-30 19:37:30','SOUTH GATE #3',2),(31,NULL,'2022-03-30 19:37:30','SOUTH GATE #3',1),(32,3,'2022-03-30 20:32:01','SOUTH GATE #3',1),(33,NULL,'2022-03-30 20:32:27','SOUTH GATE #3',2),(34,NULL,'2022-03-31 08:27:58','unknown',2),(35,NULL,'2022-03-31 08:28:05','unknown',2),(36,NULL,'2022-03-31 08:30:56','unknown',2),(37,3,'2022-03-31 08:31:03','unknown',1),(38,NULL,'2022-03-31 08:32:11','unknown',2),(39,3,'2022-03-31 08:32:18','unknown',1),(40,NULL,'2022-03-31 08:32:47','unknown',2),(41,3,'2022-03-31 08:32:54','unknown',1),(42,NULL,'2022-03-31 08:36:04','unknown',2),(43,3,'2022-03-31 08:41:34','unknown',1),(44,NULL,'2022-03-31 08:41:42','unknown',2),(45,3,'2022-03-31 08:42:30','unknown',1),(46,NULL,'2022-03-31 08:42:43','unknown',2),(47,3,'2022-03-31 10:22:50','unknown',1),(48,NULL,'2022-03-31 10:22:57','unknown',2),(49,NULL,'2022-03-31 13:35:42','',2),(50,NULL,'2022-03-31 13:36:43','',2),(51,NULL,'2022-04-05 14:19:29','Raspberry Pi #1&1',2),(52,3,'2022-04-05 14:19:36','Raspberry Pi #1&c',1),(53,3,'2022-04-05 14:19:43','Raspberry Pi #1&c',1),(54,NULL,'2022-04-05 14:19:50','Raspberry Pi #1&1',2),(55,3,'2022-04-05 14:19:58','Raspberry Pi #1&c',1),(56,NULL,'2022-04-05 14:20:09','Raspberry Pi #1&1',2),(57,3,'2022-04-05 14:47:16','Raspberry Pi #1',1),(58,3,'2022-04-05 14:47:23','Raspberry Pi #1',1),(59,NULL,'2022-04-05 14:47:31','Raspberry Pi #1',2),(60,NULL,'2022-04-05 15:26:45','Raspberry Pi #1',2),(61,3,'2022-04-05 15:26:52','Raspberry Pi #1',1),(62,3,'2022-04-05 15:26:59','Raspberry Pi #1',1),(63,NULL,'2022-04-05 15:27:06','Raspberry Pi #1',2),(64,NULL,'2022-04-05 16:05:11','Raspberry Pi #1',2),(65,NULL,'2022-04-05 16:05:21','Raspberry Pi #1',2),(66,3,'2022-04-05 16:05:31','Raspberry Pi #1',1),(67,NULL,'2022-04-05 16:05:53','Raspberry Pi #1',2),(68,3,'2022-04-05 16:18:09','Raspberry Pi #1',1),(69,NULL,'2022-04-05 16:18:16','Raspberry Pi #1',2),(70,NULL,'2022-04-05 16:18:23','Raspberry Pi #1',2),(71,3,'2022-04-05 16:18:30','Raspberry Pi #1',1);
/*!40000 ALTER TABLE `check_attempt` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-04-10 17:55:03
