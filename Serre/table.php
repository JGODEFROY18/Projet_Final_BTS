<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="utf-8">
  <title>SITE | Tableau de données</title>
  <meta content="width=device-width, initial-scale=1.0" name="viewport">
  <meta content="" name="keywords">
  <meta content="" name="description">

  <!-- Favicons -->
  <link href="img/favicon.png" rel="icon">
  <link href="img/apple-touch-icon.png" rel="apple-touch-icon">

  <!-- Google Fonts -->
  <link href="https://fonts.googleapis.com/css?family=Lato:300,400,700,300italic,400italic%7CRaleway:400,300,700" rel="stylesheet">

  <!-- Bootstrap CSS File -->
  <link href="lib/bootstrap/css/bootstrap.min.css" rel="stylesheet">

  <!-- Main Stylesheet File -->
  <link href="css/style.css" rel="stylesheet">
  <script type="text/javascript"> </script>
</head>

<body data-spy="scroll" data-offset="0" data-target="#navigation">
  <script>
    const socket = new WebSocket('ws://192.168.64.179:12345');
    //const socket = new WebSocket('ws://192.168.65.32:12345');

    socket.onmessage = function(event) {
      // On parse la chaîne JSON en objet JavaScript
      const data = JSON.parse(event.data);

      // On affiche la valeur de la propriété "ma_valeur" de l'objet
      console.log('Ma valeur : ', data.HumidInt);
      // On peut également afficher la valeur sur la page web
      var div = document.getElementById('capHumInt');
      div.innerHTML = data.HumidInt;

      console.log('Ma valeur : ', data.TempInt);
      div = document.getElementById('capTempInt');
      div.innerHTML = data.TempInt;

      console.log('Ma valeur : ', data.HumidSol1);
      div = document.getElementById('capHumBac1');
      div.innerHTML = data.HumidSol1;

      console.log('Ma valeur : ', data.HumidSol2);
      div = document.getElementById('capHumBac2');
      div.innerHTML = data.HumidSol2;

      console.log('Ma valeur : ', data.HumidSol3);
      div = document.getElementById('capHumBac3');
      div.innerHTML = data.HumidSol3;

      console.log('Ma valeur : ', data.TempExt);
      div = document.getElementById('capTempExt');
      div.innerHTML = data.TempExt;

      console.log('Ma valeur : ', data.ConsoElec);
      div = document.getElementById('consoElec');
      div.innerHTML = data.ConsoElec;

      console.log('Ma valeur : ', data.ConsoEauPluie);
      div = document.getElementById('consoEauPluie');
      div.innerHTML = data.ConsoEauPluie;

      console.log('Ma valeur : ', data.ConsoEauCourante);
      div = document.getElementById('consoEauCourante');
      div.innerHTML = data.ConsoEauCourante;

      console.log('Ma valeur : ', data.NiveauEau);
      div = document.getElementById('NiveauEau');
      div.innerHTML = data.NiveauEau;

    };


    socket.onerror = function(error) {
      console.error('WebSocket Error: ', error);
    };

    socket.onclose = function(event) {
      console.log('WebSocket Closed with code: ', event.code);
    };

    async function getDateFromTimeService() {
      try {
        const response = await fetch('https://worldtimeapi.org/api/ip');
        const data = await response.json();
        const date = new Date(data.datetime);
        const options = {
          weekday: 'long',
          year: 'numeric',
          month: 'long',
          day: 'numeric'
        };
        const dateDuJour = date.toLocaleDateString('fr-FR', options);
        document.getElementById('date').textContent = dateDuJour;
      } catch (error) {
        console.error('Erreur lors de la récupération de la date depuis le service de temps :', error);
      }
    }

    getDateFromTimeService();;
  </script>
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
          <div class="col-lg-12">
            <h3><b>Tableau de données</b></h3>
          </div>
          <div id="ma_div"></div>
          <div class="col-lg-12">
            <h3><b>DONNÉE EN TEMPS RÉELLE</b></h3>
            <table border="10">
              <tr>
                <th>Mesure</th>
                <th>Data</th>
              </tr>
              <tr>
                <td>Consommation Electrique</td>
                <td id="consoElec"></td>
              </tr>
              <tr>
                <td>Consommation Eau de Pluie</td>
                <td id="consoEauPluie"></td>
              </tr>
              <tr>
                <td>Consommation Eau Courante</td>
                <td id="consoEauCourante"></td>
              </tr>
              <tr>
                <td>Niveau Eau</td>
                <td id="NiveauEau"></td>
              </tr>
              <tr>
                <td>Capteur Humdité Intérieur</td>
                <td id="capHumInt"></td>
              </tr>
              <tr>
                <td>Capteur Température Intérieur</td>
                <td id="capTempInt"></td>
              </tr>
              <tr>
                <td>Capteur Température Exterieur</td>
                <td id="capTempExt"></td>
              </tr>
              <tr>
                <td>Capteur Humdité Bac 1</td>
                <td id="capHumBac1"></td>
              </tr>
              <tr>
                <td>Capteur Humdité Bac 2</td>
                <td id="capHumBac2"></td>
              </tr>
              <tr>
                <td>Capteur Humdité Bac 3</td>
                <td id="capHumBac3"></td>
              </tr>
              <tr>
                <td>Date</td>
                <td id="date"></td>
              </tr>
            </table>
          </div>
        </div>
      </div>
      <!--/ .container -->
    </div>
    <!--/ #headerwrap -->
  </section>

  <section id="contact">
    <div id="footerwrap">
      <div class="container">
        <div class="col-lg-5">
          <p>
          <h3>Address</h3>
          Association ou organisation<br>
          146 Bd de Saint-Quentin<br>
          Amiens<br>
          2023<br>
          France
          </p>
        </div>
      </div>
    </div>
  </section>
  <div id="copyrights">
    <div class="credits">

    </div>
  </div>

  <!-- JavaScript Libraries -->
  <script src="lib/jquery/jquery.min.js"></script>
  <script src="lib/bootstrap/js/bootstrap.min.js"></script>
  <script src="lib/php-mail-form/validate.js"></script>
  <script src="lib/easing/easing.min.js"></script>

  <!--<script>
    setInterval(getDonnees, 1000);
  </script>-->

  <!-- Template Main Javascript File -->
  <script src="js/main.js"></script>
  </section>
</body>

</html>