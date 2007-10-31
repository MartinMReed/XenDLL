<?php
	
	include_once("function.php");
	
	if (xml_authenticate($_SESSION['handle'], $_SESSION['password']) == false) {
		header("Location: login.php");
	}
	
	$xml_user = xml_getUser($_SESSION['handle']);
	
?>