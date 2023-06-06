<?php
include "fonctions/connectBDD.php";
include "classes/User.php"; ?>
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="utf-8">
  <title>SITE | Modification</title>
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
  <section id="home">
    <div id="headerwrap">
      <div class="container">
        <div class="row centered">
          <script>
            //const socket = new WebSocket('ws://192.168.64.97:12345');
            const socket = new WebSocket('ws://192.168.64.179:12345');
            //const socket = new WebSocket('ws://192.168.64.215:12345');
            //const socket = new WebSocket('ws://192.168.65.32:12345');

            socket.onmessage = function(event) {
              console.log('Message from server ', event.data);
            };

            socket.onerror = function(error) {
              console.error('WebSocket Error: ', error);
            };

            socket.onclose = function(event) {
              console.log('WebSocket Closed with code: ', event.code);
            };


            socket.onopen = function(event) {
              //const message = { data: 'Hello Server!' }; // Création de la variable contenant les données à envoyer
              socket.send(message.data); // Envoi des données au serveur
            };

            // Ouverture de la fenétre ! 
            // Créer un élément de bouton 
            var bouton = document.createElement("button");

            // Ajouter du texte au bouton
            bouton.innerHTML = "Ouverture Fenêtre";

            // Ajouter une fonctionnalité au bouton
            bouton.onclick = function() {
              const message = { data: '0' }; // Mettre à jour la variable message.data
              socket.send(message.data); // Envoi des données au serveur
              alert("La fenêtre est ouverte !");
            };
            document.body.appendChild(bouton);
            
            var bouton = document.createElement("button");
            bouton.innerHTML = "Fermeture Fenêtre";
            bouton.onclick = function() {
              const message = { data: '1' }; // Mettre à jour la variable message.data
              socket.send(message.data); // Envoi des données au serveur
              alert("La fenêtre est Fermé !");
            };
            document.body.appendChild(bouton);

            //Ouvrir Arrosage 

            var bouton = document.createElement("button");
            bouton.innerHTML = "Ouverture Arrosage";
            bouton.onclick = function() {
              const message = { data: '2' }; // Mettre à jour la variable message.data
              socket.send(message.data); // Envoi des données au serveur
              alert("L'arrosage est activé");
            };
            document.body.appendChild(bouton);

            var bouton = document.createElement("button");
            bouton.innerHTML = "Fermeture Arrosage";
            bouton.onclick = function() {
              const message = { data: '3' }; // Mettre à jour la variable message.data
              socket.send(message.data); // Envoi des données au serveur
              alert("L'arrosage est désactivé  !");
            };
            document.body.appendChild(bouton);

            //Allumer Chauffage 
            
            var bouton = document.createElement("button");
            bouton.innerHTML = "Allumer Chauffage";
            bouton.onclick = function() {
              const message = { data: '4' }; // Mettre à jour la variable message.data
              socket.send(message.data); // Envoi des données au serveur
              alert("Le chauffage est allumé!");
            };
            document.body.appendChild(bouton);

            var bouton = document.createElement("button");
            bouton.innerHTML = "Eteindre Chauffage";
            bouton.onclick = function() {
              const message = { data: '5' }; // Mettre à jour la variable message.data
              socket.send(message.data); // Envoi des données au serveur
              alert("Le Chauffage est eteint !");
            };
            document.body.appendChild(bouton);

            //Activer Brumisation 

            var bouton = document.createElement("button");
            bouton.innerHTML = "Activer Brumisation ";
            bouton.onclick = function() {
              const message = { data: '6' }; // Mettre à jour la variable message.data
              socket.send(message.data); // Envoi des données au serveur
              alert("La Brumisation est activé !");
            };
            document.body.appendChild(bouton);

            var bouton = document.createElement("button");
            bouton.innerHTML = "Désactiver Brumisation ";
            bouton.onclick = function() {
              const message = { data: '7' }; // Mettre à jour la variable message.data
              socket.send(message.data); // Envoi des données au serveur
              alert("La Brumisation est désactivé !");
            };
            document.body.appendChild(bouton);
  
          </script>
          <?php
          //si le bouton de modification a été utilisé
          if (isset($_POST["btnModif"])) {
            $util = new User(Connexion("192.168.64.189", "bddProjet", "ADMIN", "ADMIN"));
            if ($util->modifUtilisateur($_POST["txtUser"], $_POST["pwdUser"]) == true) {
              echo "Les données ont été modifiées";
              echo '<a href="accueil.php">accueil</a>';
            }
          } else {
            //formulaire de modification
            $util = new User(Connexion("192.168.64.189", "bddProjet", "ADMIN", "ADMIN"));
            $quest = $util->getallUtilisateur();
          ?>
            <div>
              <form class="box" method="post">
                <h1 class="box-title">Modification</h1>
                <span>
                  <select class="box-input" name="<?php echo "idUser" . ($r + 1) ?>" >
                      <option selected="selected" disabled="disabled">Sélectionner l'utilisateur</option>
                      <?php foreach ($quest as $row => $dataQuest) {
                          echo "<option value='" . $dataQuest["IdUser"] . "'>" . $dataQuest["Login"] . "</option>";
                      } ?>
                  </select>
                </span>
                <input type="password" class="box-input" name="pwdUser" placeholder="Mot de passe" required>
                <button type="submit" class="box-button" name="btnModif" value="Modification">Modification</button>
              </form>
            </div>
          <?php
          }
          ?>
        </div>
      </div>
    </div>
  </section>
</body>