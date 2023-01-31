-- phpMyAdmin SQL Dump
-- version 5.2.0
-- https://www.phpmyadmin.net/
--
-- Hôte : localhost
-- Généré le : lun. 17 oct. 2022 à 14:05
-- Version du serveur : 10.5.15-MariaDB-0+deb11u1
-- Version de PHP : 7.4.28

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de données : `Lawrence`
--

-- --------------------------------------------------------

--
-- Structure de la table `Bateau`
--

CREATE TABLE `Bateau` (
  `IDBateau` int(11) NOT NULL,
  `IdUser` int(11) NOT NULL,
  `Type` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Structure de la table `GPS`
--

CREATE TABLE `GPS` (
  `BateauID` int(11) NOT NULL,
  `Date` date NOT NULL,
  `Heure` time NOT NULL,
  `Latitude` varchar(100) NOT NULL,
  `Longitude` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Structure de la table `User`
--

CREATE TABLE `User` (
  `IDUser` int(11) NOT NULL,
  `Login` varchar(100) NOT NULL,
  `MDP` varchar(100) NOT NULL,
  `IsAdmin` tinyint(4) NOT NULL DEFAULT 0 COMMENT '1 : admin. 0 : autre'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Déchargement des données de la table `User`
--

INSERT INTO `User` (`IDUser`, `Login`, `MDP`, `IsAdmin`) VALUES
(1, 'admin', 'admin', 1),
(2, 'test', 'test', 0);

-- --------------------------------------------------------

--
-- Structure de la table `Vitesse`
--

CREATE TABLE `Vitesse` (
  `BateauId` int(11) NOT NULL,
  `Date` date NOT NULL,
  `Heure` time NOT NULL,
  `Vitesse` int(11) NOT NULL,
  `VitesseMoyenne` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Index pour les tables déchargées
--

--
-- Index pour la table `Bateau`
--
ALTER TABLE `Bateau`
  ADD PRIMARY KEY (`IDBateau`),
  ADD KEY `IdUser` (`IdUser`);

--
-- Index pour la table `GPS`
--
ALTER TABLE `GPS`
  ADD KEY `BateauID` (`BateauID`);

--
-- Index pour la table `User`
--
ALTER TABLE `User`
  ADD PRIMARY KEY (`IDUser`);

--
-- Index pour la table `Vitesse`
--
ALTER TABLE `Vitesse`
  ADD UNIQUE KEY `BateauId` (`BateauId`);

--
-- AUTO_INCREMENT pour les tables déchargées
--

--
-- AUTO_INCREMENT pour la table `Bateau`
--
ALTER TABLE `Bateau`
  MODIFY `IDBateau` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT pour la table `User`
--
ALTER TABLE `User`
  MODIFY `IDUser` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;

--
-- Contraintes pour les tables déchargées
--

--
-- Contraintes pour la table `Bateau`
--
ALTER TABLE `Bateau`
  ADD CONSTRAINT `Bateau_ibfk_1` FOREIGN KEY (`IdUser`) REFERENCES `User` (`IDUser`);

--
-- Contraintes pour la table `GPS`
--
ALTER TABLE `GPS`
  ADD CONSTRAINT `GPS_ibfk_1` FOREIGN KEY (`BateauID`) REFERENCES `Bateau` (`IDBateau`);

--
-- Contraintes pour la table `Vitesse`
--
ALTER TABLE `Vitesse`
  ADD CONSTRAINT `Vitesse_ibfk_1` FOREIGN KEY (`BateauId`) REFERENCES `Bateau` (`IDBateau`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
