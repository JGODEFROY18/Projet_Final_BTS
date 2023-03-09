<?php
include "fonctions/connectBDD.php";
include "classes/User.php"; ?>
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="utf-8">
  <title>Le GPS - GPS Localisation</title>
  <meta content="width=device-width, initial-scale=1.0" name="viewport">
  <meta content="" name="keywords">
  <meta content="" name="description">

  <!-- Favicons -->
  <link href="img/favicon.png" rel="icon">
  <link href="img/apple-touch-icon.png" rel="apple-touch-icon">

  <!-- Google Fonts -->
  <link href="https://fonts.googleapis.com/css?family=Lato:300,400,700,300italic,400italic|Raleway:400,300,700" rel="stylesheet">

  <!-- Bootstrap CSS File -->
  <link href="lib/bootstrap/css/bootstrap.min.css" rel="stylesheet">

  <!-- Main Stylesheet File -->
  <link href="css/style.css" rel="stylesheet">

</head>

<body data-spy="scroll" data-offset="0" data-target="#navigation">

  <!-- Fixed navbar -->
  <div id="navigation" class="navbar navbar-default navbar-fixed-top">
    <div class="container">
      <div class="navbar-header">
        <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
        </button>
        <a class="navbar-brand smothscroll" href="#home"><b>GPS</b></a>
      </div>
      <div class="navbar-collapse collapse">
        <ul class="nav navbar-nav">
          <li class="active"><a href="accueil.php">Home</a></li>
          <li><a href="table.php">Tableau</a></li>
          <li><a href="graph.php">Graphique</a></li>
          <li><a href="modification.php">Compte</a></li>
          <li><a href="Admin.php">Admin</a></li>
          <li><a href="fonctions/deconnexion.php">Deconnexion</a></li>
        </ul>
      </div>
    </div>
  </div>
  <?php
  //si le bouton de modification a été utilisé
  if (isset($_POST["btnConnexion"])) {
    $util = new User(Connexion("192.168.64.189", "bddProjet", "ADMIN", "ADMIN"));
    if ($util->modifUtilisateur($_POST["txtUser"], $_POST["pwdUser"]) == true) {
      echo "Les données ont été modifiées";

      echo '<a href="accueil.php">accueil</a>';
    }
  } else {
    //formulaire de modification
  ?>
    <div>
      <form class="box" method="post">
        <h1 class="box-title">Modification</h1>
        <input type="text" class="box-input" name="txtUser" placeholder="Nom d'utilisateur" required>
        <input type="password" class="box-input" name="pwdUser" placeholder="Mot de passe" required>
        <button type="submit" class="box-button" name="btnConnexion" value="Connexion">modification</button>
      </form>
    </div>
  <?php
  }
  ?>
</body>