<?php

	include_once("function.php");
	
	$type = $_GET['type'];
	$handle = strtolower($_GET['handle']);
	
	print(xml_getIP($type, $handle));
	
?>