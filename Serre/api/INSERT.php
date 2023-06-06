<?php
if (isset($_POST["donnees"])) {
    //transformation du post d'abord en json puis en array
    $donnees = json_decode($_POST["donnees"]);
    $data = get_object_vars($donnees);
    $base = new PDO("mysql:host=192.168.64.189;dbname=BDDProjet", "ADMIN", "ADMIN");
    $base->query("Insert into data(consoEauPluie,consoEauCourante,NiveauEau,capTempInt,capHumInt,capTempExt,capHumBac1,capHumBac2,capHumBac3) 
    values('" . $data["ConsoEauPluie"] . "','" . $data["ConsoEauCourante"] . "','" . $data["NiveauEau"] . "',
    '" . $data["TempInt"] . "','" . $data["HumidInt"] . "','" . $data["TempExt"] . "',
    '" . $data["HumidSol1"] . "','" . $data["HumidSol2"] . "','" . $data["HumidSol3"] . "')");
} else echo "pas de donnees";
