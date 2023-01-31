<?php
include "TP-GPS/fonctions/connectBDD.php";
include "TP-GPS/classes/User.php"; 


?>

<!DOCTYPE html>
<html lang="fr">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="TP-GPS/css/connexion.css">
    <title>Site</title>

</head>

<body>
    <script>
        // Création de l'objet WebSocket en spécifiant l'URL du serveur
        var socket = new WebSocket('ws://192.168.64.215:12345');

        // Définition de la fonction qui sera appelée lorsque la connexion sera ouverte
        socket.onopen = function(event) {
        // Affiche un message dans la console
        console.log('Connecté au serveur');
        // Envoie un message au serveur
        socket.send('Bonjour tout le monde !');
        };

        // Définition de la fonction qui sera appelée lorsque le serveur enverra un message
        socket.onmessage = function(event) {
        // Affiche le message reçu dans la console
        console.log('Message reçu : ' + event.data);
        };

        // Définition de la fonction qui sera appelée en cas d'erreur
        socket.onerror = function(event) {
        // Affiche l'erreur dans la console
        console.error('Erreur : ' + event.data);
        };

        // Définition de la fonction qui sera appelée lorsque la connexion sera fermée
        socket.onclose = function(event) {
        // Affiche un message dans la console
        console.log('Déconnecté du serveur');
        };
    </script>
    <?php
    //si l'utilisateur a appuyé sur le bouton de connexion
    if (isset($_POST["btnConnexion"])) {
        //données à changer
        $util = new User(Connexion());
        if ($util->autorisation() == true) {
            //si l'utilisateur est reconnu dans la base de données
            header("Location: TP-GPS/accueil.php");
        } else {
            // on affiche le formulaire de connexion
    ?>
            <form class="box" method="post">
                <h1 class="box-title">Connexion</h1>
                <input type="text" class="box-input" name="txtUser" placeholder="Nom d'utilisateur" required>
                <input type="password" class="box-input" name="pwdUser" placeholder="Mot de passe" required>
                <button type="submit" class="box-button" name="btnConnexion" value="Connexion">Connexion</button>
                <?php echo "<p class='errorMessage'>Nom d'utilisateur ou mot de passe inconnu</p>"; ?>
            </form>
        <?php }
    } else {
        // on affiche le formulaire de connexion
        ?>
        <form class="box" method="post">
            <h1 class="box-title">Connexion</h1>
            <input type="text" class="box-input" name="txtUser" placeholder="Nom d'utilisateur" required>
            <input type="password" class="box-input" name="pwdUser" placeholder="Mot de passe" required>
            <button type="submit" class="box-button" name="btnConnexion" value="Connexion">Connexion</button>
            <?php if (isset($_SESSION["TryAgain"])) echo "<p class='errorMessage'>Nom d'utilisateur ou mot de passe inconnu</p>"; ?>
        </form>
    <?php } ?>
</body>

</html>