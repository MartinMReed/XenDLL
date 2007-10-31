<?php

	if (isset($_GET['handle']) == false && isset($_GET['password']) == false) {
		include_once("session.php");
		include_once("authorize.php");
	} else {
		include_once("function.php");
	}
	
	$filename = $_GET['script'];
	
	if (isset($_GET['handle']) || isset($_GET['password'])) {
		$handle = strtolower($_GET['handle']);
		$password = xml_md5($handle, $_GET['password']);
		$authenticate = xml_authenticate($handle, $password);
	} else {
		$handle = $xml_user->handle;
		$password = $xml_user->password;
		$authenticate = true;
	}
	
	if ($authenticate) {
		if (isset($filename) && file_exists($userfolder . "/" . $handle . "/" . $filename . ".xml")) {
		
			header("Vary: User-Agent");
			header("Content-Type: text/xml");
			
			if (isset($_GET['handle']) == false && isset($_GET['password']) == false) {
				header("Content-Disposition: attachment; filename=\"" . $filename . ".xml\"");
			}
			
			xml_setLogin("game", $handle);
			
			$script = instruc_c2a(xml_load_instructions($handle, $filename));
			
			print("<script version=\"" . $scriptVersion . "\">\n");
			
			print("    <setting>\n");
			print("        <script_screen_sizeA>" . $script[0]->script_screen_sizeA . "</script_screen_sizeA>\n");
			print("        <script_screen_sizeB>" . $script[0]->script_screen_sizeB . "</script_screen_sizeB>\n");
			print("        <script_useHealthKey>" . $script[0]->script_useHealthKey . "</script_useHealthKey>\n");
			print("        <script_HealthPercent>" . $script[0]->script_HealthPercent . "</script_HealthPercent>\n");
			print("        <script_HealthCode>" . $script[0]->script_HealthCode . "</script_HealthCode>\n");
			print("        <script_useManaKey>" . $script[0]->script_useManaKey . "</script_useManaKey>\n");
			print("        <script_ManaPercent>" . $script[0]->script_ManaPercent . "</script_ManaPercent>\n");
			print("        <script_ManaCode>" . $script[0]->script_ManaCode . "</script_ManaCode>\n");
			print("    </setting>\n");
			
			for ($i = 0; $i < count($script[1]); $i++) {
				print("    <instruction>\n");
				
				for ($j = 0; $j < count($scriptItem[1]); $j++) {
					if ($script[1][$i][$j] != $scriptItem[1][$j][2]) {
						print("        <" . $scriptItem[1][$j][0] . ">" . $script[1][$i][$j] . "</" . $scriptItem[1][$j][0] . ">\n");
					}
				}
				
				print("    </instruction>\n");
			}
			
			print("</script>");

		} else {
			print("Script not found.");
		}
	} else {
		print("Incorrect login.");
	}
	
?>