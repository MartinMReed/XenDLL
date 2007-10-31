<?php

	include_once("function.php");
	
	$handle = strtolower($_GET['handle']);
	$password = xml_md5($handle, $_GET['password']);
	print(xml_md5("computron", xml_authenticate($handle, $password)));
	
?>