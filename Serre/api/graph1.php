<?php 
    $base = new PDO("mysql:host=192.168.64.189;dbname=BDDProjet", "ADMIN", "ADMIN");
    $resultat = $base->query("Select * from data");
    $donnees = $resultat->fetchAll(PDO::FETCH_ASSOC);
?>

<html>
  <head>
    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <script type="text/javascript">
      google.charts.load('current', {'packages':['bar']});
      google.charts.setOnLoadCallback(drawChart);
      var donnees = <?php echo json_encode($donnees);?>;
      console.log(donnees);
      function drawChart() {
        var data = google.visualization.arrayToDataTable([
          ['Date', 'Conso Electrique', 'Conso Eau', 'Capt Humidité Intérieur', 'Capt Température Intérieur' ,'Capt Température Exterieur' ,'Capt Humidité Bac 1','Capt Humidité Bac 2','Capt Humidité Bac 3'],
         [donnees[0].date, donnees[0].consoElec, donnees[0].consoEau, donnees[0].capHumInt, donnees[0].capTempInt, donnees[0].capTempExt, donnees[0].capHumBac1, donnees[0].capHumBac2, donnees[0].capHumBac3]
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
  </head>
  <body>
    <div id="columnchart_material" style="width: 800px; height: 500px;"></div>
  </body>
</html>
