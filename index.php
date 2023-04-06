<?php
include "Serre/fonctions/connectBDD.php";
include "Serre/classes/User.php"; 


?>

<!DOCTYPE html>
<html lang="fr">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="Serre/css/connexion.css">
     <!-- Favicons -->
    <link href="Serre/img/favicon.png" rel="icon">
    <title>CONNEXION | SITE SERRE</title>

</head>

<body>
<div class="bg">
	<?php
	//si l'utilisateur a appuyé sur le bouton de connexion
	if (isset($_POST["btnConnexion"])) {
		//données à changer
		$util = new User(Connexion());
		if ($util->autorisation() == true) {
			//si l'utilisateur est reconnu dans la base de données
			header("Location: Serre/accueil.php");
		} else {
			// on affiche le formulaire de connexion
	?>
			<form class="box" method="post">
				<h1 class="box-title">Connexion</h1>
				<input type="text" class="box-input" name="txtUser" placeholder="Nom d'utilisateur" required>
				<input type="password" class="box-input" name="pwdUser" placeholder="Mot de passe" required>
				<button type="submit" class="box-button" name="btnConnexion" value="Connexion">Connexion</button>
				<?php echo "<p class='errorMessage'>Nom d'utilisateur ou mot de passe inconnu</p>"; ?>
			</form>
		<?php }
	} else {
		// on affiche le formulaire de connexion
		?>
		<form class="box" method="post">
			<h1 class="box-title">Connexion</h1>
			<input type="text" class="box-input" name="txtUser" placeholder="Nom d'utilisateur" required>
			<input type="password" class="box-input" name="pwdUser" placeholder="Mot de passe" required>
			<button type="submit" class="box-button" name="btnConnexion" value="Connexion">Connexion</button>
			<?php if (isset($_SESSION["TryAgain"])) echo "<p class='errorMessage'>Nom d'utilisateur ou mot de passe inconnu</p>"; ?>
		</form>
	<?php } ?>
</div>
</body>

</html>
