<?php function Connexion()
{
    try {
        /*on teste la connexion à la base de données
        $serveur = addresse IP de la base de données
        $bdd = nom de la base de données
        $user,$passswd = login et mot de passe d'un utilisateur de la base de données
        */
        return new PDO("mysql:host=192.168.64.189;dbname=BDDProjet", "ADMIN", "ADMIN");
    } catch (Exception $e) {
        echo $e;
    }
}
