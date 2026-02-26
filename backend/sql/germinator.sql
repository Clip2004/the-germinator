-- phpMyAdmin SQL Dump
-- version 5.2.2
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: Sep 23, 2025 at 12:31 AM
-- Server version: 5.7.24
-- PHP Version: 8.3.1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `germinator`
--

-- --------------------------------------------------------

--
-- Table structure for table `germination_batches`
--

CREATE TABLE `germination_batches` (
  `batch_id` int(11) NOT NULL,
  `profile_id` int(11) DEFAULT NULL,
  `start_date` date NOT NULL,
  `end_date` date DEFAULT NULL,
  `status` varchar(50) DEFAULT 'En progreso',
  `quantity_seeded` int(11) DEFAULT NULL,
  `success_rate` decimal(5,2) DEFAULT NULL,
  `batch_timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `germination_batches`
--

INSERT INTO `germination_batches` (`batch_id`, `profile_id`, `start_date`, `end_date`, `status`, `quantity_seeded`, `success_rate`, `batch_timestamp`) VALUES
(1, 1, '2025-09-16', NULL, 'Terminado', 50, NULL, '2025-09-16 16:24:02'),
(2, 1, '2025-09-16', NULL, 'Iniciando', 150, NULL, '2025-09-16 20:45:51'),
(3, 1, '2025-09-16', NULL, 'Iniciando', 150, NULL, '2025-09-16 21:07:54'),
(4, 1, '2025-09-16', NULL, 'Iniciando', 150, NULL, '2025-09-16 21:09:31');

-- --------------------------------------------------------

--
-- Table structure for table `germination_profiles`
--

CREATE TABLE `germination_profiles` (
  `profile_id` int(11) NOT NULL,
  `plant_species` varchar(30) NOT NULL,
  `profile_name` varchar(30) NOT NULL,
  `temp_min_celsius` decimal(4,2) NOT NULL,
  `temp_max_celsius` decimal(4,2) NOT NULL,
  `light_duration_hours` int(11) NOT NULL,
  `profile_timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `germination_profiles`
--

INSERT INTO `germination_profiles` (`profile_id`, `plant_species`, `profile_name`, `temp_min_celsius`, `temp_max_celsius`, `light_duration_hours`, `profile_timestamp`) VALUES
(1, 'Zanahoria', 'Perfil Inicial', 18.00, 22.50, 14, '2025-09-16 21:00:12'),
(2, 'Tomate', 'Verano', 22.00, 28.00, 16, '2025-09-16 20:39:52'),
(3, 'Arroz', 'Verano', 22.00, 28.00, 16, '2025-09-16 21:07:29');

-- --------------------------------------------------------

--
-- Table structure for table `sensor_log`
--

CREATE TABLE `sensor_log` (
  `log_id` bigint(20) NOT NULL,
  `batch_id` int(11) NOT NULL,
  `temperature_celsius` decimal(5,2) DEFAULT NULL,
  `humidity_percent` decimal(5,2) DEFAULT NULL,
  `light_intensity_percentage` int(11) DEFAULT NULL,
  `reading_timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `sensor_log`
--

INSERT INTO `sensor_log` (`log_id`, `batch_id`, `temperature_celsius`, `humidity_percent`, `light_intensity_percentage`, `reading_timestamp`) VALUES
(1, 1, 21.70, 82.00, 95, '2025-09-16 20:37:08'),
(2, 1, 21.70, 82.00, 95, '2025-09-16 21:06:42'),
(3, 1, 21.70, 82.00, 95, '2025-09-16 21:06:50'),
(4, 2, 21.70, 82.00, 95, '2025-09-16 21:06:57'),
(5, 1, 21.70, 99.00, 100, '2025-09-19 17:29:11'),
(6, 1, 21.70, 99.00, 100, '2025-09-19 17:36:05'),
(7, 1, 21.70, 99.00, 100, '2025-09-19 18:14:15'),
(8, 1, 21.70, 99.00, 100, '2025-09-19 18:14:49'),
(9, 1, 21.70, 10.00, 100, '2025-09-19 18:25:49'),
(10, 1, 21.70, 15.00, 100, '2025-09-19 18:27:23'),
(11, 1, 21.70, 15.00, 100, '2025-09-22 19:11:06'),
(12, 1, 27.70, 99.00, 50, '2025-09-22 19:13:41');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `germination_batches`
--
ALTER TABLE `germination_batches`
  ADD PRIMARY KEY (`batch_id`),
  ADD KEY `profile_id` (`profile_id`);

--
-- Indexes for table `germination_profiles`
--
ALTER TABLE `germination_profiles`
  ADD PRIMARY KEY (`profile_id`) USING BTREE;

--
-- Indexes for table `sensor_log`
--
ALTER TABLE `sensor_log`
  ADD PRIMARY KEY (`log_id`),
  ADD KEY `batch_id` (`batch_id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `germination_batches`
--
ALTER TABLE `germination_batches`
  MODIFY `batch_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;

--
-- AUTO_INCREMENT for table `germination_profiles`
--
ALTER TABLE `germination_profiles`
  MODIFY `profile_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;

--
-- AUTO_INCREMENT for table `sensor_log`
--
ALTER TABLE `sensor_log`
  MODIFY `log_id` bigint(20) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=13;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `germination_batches`
--
ALTER TABLE `germination_batches`
  ADD CONSTRAINT `germination_batches_ibfk_1` FOREIGN KEY (`profile_id`) REFERENCES `germination_profiles` (`profile_id`);

--
-- Constraints for table `sensor_log`
--
ALTER TABLE `sensor_log`
  ADD CONSTRAINT `sensor_log_ibfk_1` FOREIGN KEY (`batch_id`) REFERENCES `germination_batches` (`batch_id`) ON DELETE CASCADE;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
