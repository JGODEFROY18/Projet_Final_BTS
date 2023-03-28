<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="utf-8">
    <title>SITE | Graphique de données</title>
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
    <link href="css/TCP.css" rel="stylesheet">
    <script type="text/javascript" src="js/recupdonnees.js"> </script>
</head>

<body data-spy="scroll" data-offset="0" data-target="#navigation">
<script>
    const socket = new WebSocket('ws://192.168.64.93:12345');

    socket.onopen = function(event) {
      socket.send('Hello Server!');
    };

    socket.onmessage = function(event) {
      console.log('Message from server ', event.data);
    };

    socket.onerror = function(error) {
      console.error('WebSocket Error: ', error);
    };

    socket.onclose = function(event) {
      console.log('WebSocket Closed with code: ', event.code);
    };
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
                        <h3><b>Graphique des données</b></h3>
                    </div>

                    <div class="graphique">
                        <h3><b>DONNÉE EN TEMPS RÉELLE</b></h3>
                        <center>
                            <?php
                            (include 'api/graph1.php')
                            ?>
                        </center>
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

    <script>
        setInterval(getDonnees, 1000);
    </script>

    <!-- Template Main Javascript File -->
    <script src="js/main.js"></script>
    </section>
</body>

</html>