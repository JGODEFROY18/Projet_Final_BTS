<html>
  <head>
    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <script type="text/javascript">
      google.charts.load('current', {'packages':['bar']});
      google.charts.setOnLoadCallback(drawChart);

      function drawChart() {
        var data = google.visualization.arrayToDataTable([
          ['Date', 'Conso Electrique', 'Conso Eau', 'Capt Humidité Intérieur', 'Capt Température Intérieur','Capt Humidité Bac 1','Capt Humidité Bac 2','Capt Humidité Bac 3'],
          ['2014', 1000, 400, 200, 5, 6, 7, 8],
          ['2015', 1170, 460, 250, 5, 6, 7, 8],
          ['2016', 660, 1120, 300, 5, 6, 7, 8],
          ['2017', 1030, 540, 350, 5, 6, 7, 8]
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