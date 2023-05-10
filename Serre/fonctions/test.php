<?php
if (isset($_POST["donnees"])){
    //transformation du post d'abord en json puis en array
    $donnees = json_decode($_POST["donnees"]);
    $data = get_object_vars($donnees);
    print_r($data);
}
else echo "pas de donnees";
