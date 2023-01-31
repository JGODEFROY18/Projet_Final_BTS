<?php class User
{

    private $Base;
    private $id;
    private $name;

    public function __construct($BDD)
    {
        $this->Base = $BDD;
    }

    public function getID()
    {
        return $this->id;
    }

    public function autorisation()
    {
        $Req = $this->Base->query("SELECT * FROM User WHERE Login = '" . $_POST["txtUser"] . "' AND MDP = '" . $_POST["pwdUser"] . "'");
        $test = $Req->fetch();
        if (isset($test["IDUser"])) {
            $this->id = $test["IDUser"];
            $this->name = $test["Login"];
            return true;
        } else {
            echo "Utilisateur inconnu";
            return false;
        }
    }

    /*public function VerificationUser($id)
    {
        $Req = $this->Base->query("SELECT IsAdmin FROM User WHERE id = '" .$id. "'");
        $test = $Req->fetch();
        if (isset($test["IDUser"])) {
            $this->id = $test["IDUser"];
            $this->name = $test["Login"];
            return true;
        } else {
            echo "Utilisateur inconnu";
            return false;
        }
    }*/

    public function affichageUtilisateur($id)
    {
        $requete = "SELECT `IDUser`,`Login` FROM `User` WHERE `IDUser` = '" . $id . "'";
        $resultat = $this->Base->query($requete);
        return $resultat->fetch();
    }

    public function creationUtilisateur($login, $passwd)
    {
        $requete = "INSERT INTO `User`(`Login`,`MDP`) VALUES ('" . $login . "','" . $passwd . "')";
        $this->Base->query($requete);
        return true;
    }

    public function modifUtilisateur($login, $passwd)
    {
        $requete = "UPDATE `User` SET `MDP` = '" . $passwd . "' WHERE `Login` = '" . $login . "'";
        $this->Base->query($requete);
        return true;
    }

    public function suppressionUtilisateur($login, $passwd)
    {
        $requete = "DELETE FROM `User` WHERE `Login` = '" . $login . "' AND `MDP` = '" . $passwd . "'";
        $this->Base->query($requete);
    }
}
