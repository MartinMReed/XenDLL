<?php include_once("function.php"); ?>

<html>

<head>
	<title>Transformer</title>
	<link rel="stylesheet" type="text/css" href="style.css">
	<script language="javascript" src="javascript.js"></script>
</head>

<body>

<div id="body" name="body">
	<center>
	<img src="images/logo.png"><br>
	<?php
		
		if (isset($xml_user)) {
			print('<a href="login.php">logout</a> | <a href="user.edit.php">account</a> | <a href="script.php">script</a>');
		}
	?><br>
	</center>
	
	<hr>