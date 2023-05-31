<?php
if (isset($_POST["donnees"])) {
    //transformation du post d'abord en json puis en array
    $donnees = json_decode($_POST["donnees"]);
    $data = get_object_vars($donnees);
    //envoi des donnees en BDD
    $base = new PDO("mysql:host=192.168.64.189;dbname=BDDProjet", "ADMIN", "ADMIN");
    $base->query("Insert into data(consoEau,consoElec,capTempInt,capHumInt,capTempExt,capHumBac1,capHumBac2,capHumBac3) 
    values('" . $data["consoEau"] . "','" . $data["consoElec"] . "',
    '" . $data["capTempInt"] . "','" . $data["capHumInt"] . "',
    '" . $data["capTempExt"] . "','" . $data["capHumBac1"] . "',
    '" . $data["capHumBac2"] . "','" . $data["capHumBac3"] . "')");
    //print_r($data);
} else echo "pas de donnees";
