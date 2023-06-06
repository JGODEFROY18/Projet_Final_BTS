
<?php 
    $base = new PDO("mysql:host=192.168.64.189;dbname=BDDProjet", "ADMIN", "ADMIN");
    //$resultat = $base->query("Select * from data where date > DATE_SUB(CURDATE(), INTERVAL 1 DAY);");
    $resultat = $base->query("Select * from data");
    $donnees = $resultat->fetchAll(PDO::FETCH_ASSOC);
    
  if (empty($donnees)){        
    $donnees["consoElec"]=0;
    $donnees["consoEauPluie"]=0;
    $donnees["consoEauCourante"]=0;
    $donnees["capHumInt"]=0;
    $donnees["capTempInt"]=0;
    $donnees["capTempExt"]=0;
    $donnees["capHumBac1"]=0;
    $donnees["capHumBac2"]=0;
    $donnees["capHumBac3"]=0;
  }

?>

    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <script type="text/javascript">
      google.charts.load('current', {'packages':['bar']});
      google.charts.setOnLoadCallback(drawChart);


      <?php //calcul des moyennes 

        $dataMoyenne["consoElec"]=0;
        $dataMoyenne["consoEauPluie"]=0;
        $dataMoyenne["consoEauCourante"]=0;
        $dataMoyenne["capHumInt"]=0;
        $dataMoyenne["capTempInt"]=0;
        $dataMoyenne["capTempExt"]=0;
        $dataMoyenne["capHumBac1"]=0;
        $dataMoyenne["capHumBac2"]=0;
        $dataMoyenne["capHumBac3"]=0;

        if ($resultat->rowCount() == 0) $nombreDeDonne = 1;
        else $nombreDeDonne = $resultat->rowCount();

      
        foreach ($donnees as $donnee) {
          $dataMoyenne["consoElec"]+=$donnee["consoElec"];
          $dataMoyenne["consoEauPluie"]+=$donnee["consoEauPluie"];
          $dataMoyenne["consoEauCourante"]+=$donnee["consoEauCourante"];
          $dataMoyenne["capHumInt"]+=$donnee["capHumInt"];
          $dataMoyenne["capTempInt"]+=$donnee["capTempInt"];
          $dataMoyenne["capTempExt"]+=$donnee["capTempExt"];
          $dataMoyenne["capHumBac1"]+=$donnee["capHumBac1"];
          $dataMoyenne["capHumBac2"]+=$donnee["capHumBac2"];
          $dataMoyenne["capHumBac3"]+=$donnee["capHumBac3"];
  
        }

        $dataMoyenne["consoElec"] = round( $dataMoyenne["consoElec"]/$nombreDeDonne,2);
        $dataMoyenne["consoEauPluie"] = round( $dataMoyenne["consoEauPluie"]/$nombreDeDonne,2);
        $dataMoyenne["consoEauCourante"] = round( $dataMoyenne["consoEauCourante"]/$nombreDeDonne,2);
        $dataMoyenne["capHumInt"] = round( $dataMoyenne["capHumInt"]/$nombreDeDonne,2);
        $dataMoyenne["capTempInt"] = round( $dataMoyenne["capTempInt"]/$nombreDeDonne,2);
        $dataMoyenne["capTempExt"] = round( $dataMoyenne["capTempExt"]/$nombreDeDonne,2);
        $dataMoyenne["capHumBac1"] = round( $dataMoyenne["capHumBac1"]/$nombreDeDonne,2);
        $dataMoyenne["capHumBac2"] = round( $dataMoyenne["capHumBac2"]/$nombreDeDonne,2);
        $dataMoyenne["capHumBac3"] = round( $dataMoyenne["capHumBac3"]/$nombreDeDonne,2);

      
      ?>

      console.log("langlace"+<?php echo  '"'.$donnees[0]["consoElec"].$donnees[1]["consoElec"].'"';?>);

      var donnees = <?php echo json_encode($donnees);?>;
      console.log(donnees);
      function drawChart() {
        var data = google.visualization.arrayToDataTable([
          ['Date', 'Conso Electrique', 'Conso Eau Pluie', 'Conso Eau Courante' , 'Capt Humidité Intérieur', 'Capt Température Intérieur' ,
          'Capt Température Exterieur' ,'Capt Humidité Bac 1','Capt Humidité Bac 2','Capt Humidité Bac 3'],
         [donnees[0].date,
          <?= $dataMoyenne["consoElec"]?>,
          <?= $dataMoyenne["consoEauPluie"]?>,
          <?= $dataMoyenne["consoEauCourante"]?>,
          <?= $dataMoyenne["capHumInt"]?>, 
          <?= $dataMoyenne["capTempInt"]?>, 
          <?= $dataMoyenne["capTempExt"]?>, 
          <?= $dataMoyenne["capHumBac1"]?>, 
          <?= $dataMoyenne["capHumBac2"]?>, 
          <?= $dataMoyenne["capHumBac3"]?>
        ]
      
        ]);

        var options = {
          chart: {
            title: 'Consommation Serre',
          }
        };

        var chart = new google.charts.Bar(document.getElementById('columnchart_material'));

        chart.draw(data, google.charts.Bar.convertOptions(options));
      }
    </script>
    <div id="columnchart_material" style="width: 800px; height: 500px;"></div>