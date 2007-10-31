<?php

	foreach($_POST as $key => $value) {
		$_POST[$key] = addslashes(trim($value));
	}
	
	foreach($_GET as $key => $value) {
		$_GET[$key] = addslashes(trim($value));
	}
	
	$session_name = "computron";
	
	$userfolder = "9f3d81e60e5ed142c31dd71a4855c739";
	$dbasfolder = "52a27ad880a82240e566c50dfce8424c";
	$database = $dbasfolder . "/database.xml";
	
	umask(0070);
	
	if (file_exists($userfolder) == false) {
		mkdir($userfolder, 0707);
	}
	
	$userfolder_htaccess = $userfolder . "/.htaccess";
	if (file_exists($userfolder_htaccess) == false) {
		if ($file = fopen($userfolder_htaccess, "w")) {
			fwrite($file, "deny from all");
			fclose($file);
		}
	}
	
	if (file_exists($dbasfolder) == false) {
		mkdir($dbasfolder, 0707);
	}
	
	$dbasfolder_htaccess = $dbasfolder . "/.htaccess";
	if (file_exists($dbasfolder_htaccess) == false) {
		if ($file = fopen($dbasfolder_htaccess, "w")) {
			fwrite($file, "deny from all");
			fclose($file);
		}
	}
	
	if (file_exists($database) == false) {
		if ($file = fopen($database, "w")) {
			fwrite($file, "<database>\n");
			fwrite($file, "</database>");
			fclose($file);
		}
	}
	
	//unlink($database);
	//rmdir($userfolder);
	//rmdir($dbasfolder);
	
	include_once("instruction.php");
	
	$DIK = array(	"0x00", "DIK_1", "DIK_2", "DIK_3", "DIK_4", "DIK_5", "DIK_6", "DIK_7", "DIK_8", 
					"DIK_9", "DIK_0", "0x00", "DIK_A", "DIK_B", "DIK_C", "DIK_D", "DIK_E", 
					"DIK_F", "DIK_G", "DIK_H", "DIK_I", "DIK_J", "DIK_K", "DIK_L", "DIK_M", 
					"DIK_N", "DIK_O", "DIK_P", "DIK_Q", "DIK_R", "DIK_S", "DIK_T", "DIK_U", 
					"DIK_V", "DIK_W", "DIK_X", "DIK_Y", "DIK_Z", "0x00", "DIK_F1", "DIK_F2", 
					"DIK_F3", "DIK_F4", "DIK_F5", "DIK_F6", "DIK_F7", "DIK_F8", "DIK_F9", 
					"DIK_F10", "DIK_F11", "DIK_F12", "0x00", "DIK_ESCAPE", "DIK_TAB", "0x00", 
					"DIK_MINUS", "DIK_EQUALS", "0x00", "DIK_BACK", "DIK_RETURN", "0x00", 
					"DIK_LBRACKET", "DIK_RBRACKET", "0x00", "DIK_LCONTROL", 
					"DIK_RCONTROL", "0x00", "DIK_LSHIFT", "DIK_RSHIFT", "0x00", "DIK_LALT", 
					"DIK_RALT", "0x00", "DIK_SEMICOLON", "DIK_APOSTROPHE", "0x00", 
					"DIK_BACKSLASH", "0x00", "DIK_COMMA", "DIK_PERIOD", "0x00", "DIK_SPACE", 
					"0x00", "DIK_CAPITAL", "0x00", "DIK_NUMLOCK", "DIK_SCROLL", "0x00", "DIK_LEFT", 
					"DIK_RIGHT", "DIK_UP", "DIK_DOWN", "0x00", "DIK_PAUSE", "DIK_HOME", "0x00", 
					"DIK_INSERT", "DIK_DELETE", "0x00", "DIK_NUMPAD1", "DIK_NUMPAD2", 
					"DIK_NUMPAD3", "DIK_NUMPAD4", "DIK_NUMPAD5", "DIK_NUMPAD6", 
					"DIK_NUMPAD7", "DIK_NUMPAD8", "DIK_NUMPAD9", "DIK_NUMPAD0", "0x00", 
					"DIK_NUMPADENTER", "DIK_NUMPADEQUALS", "DIK_DECIMAL", "DIK_ADD", 
					"DIK_SUBTRACT", "DIK_MULTIPLY", "DIK_DIVIDE");
	
	//function stricmp($str1, $str2) {
	//	return strcmp(strtolower($str1), strtolower($str2));
	//}
	
	// parse xml for $database
	
	class xml_user_entry {
	    var $handle;
	    var $password;
	    var $email;
	    
	    var $web_date;
	    var $web_ipaddr;
	    var $web_count;
	    
	    var $game_date;
	    var $game_ipaddr;
	    var $game_count;
	}
	
	function xml_load_users() {
		global $xml_content;
		global $xml_counter;
		
		global $database;
		
		$xml_content = array();
		$xml_counter = 0;
	
		$xml_parser = xml_parser_create();
		
		xml_set_element_handler($xml_parser, "startTag_users", "endTag_users");
		
		xml_set_character_data_handler($xml_parser, "content_users");
		
		$fp = fopen($database, "r") or die("Could not open file");
		
		$data = fread($fp, filesize($database)) or die("Could not read file");
		
		xml_parse($xml_parser, $data, feof($fp)) or die("Error on line " . xml_get_current_line_number($xml_parser));
		
		xml_parser_free($xml_parser);
		
		fclose($fp);
		
		return $xml_content;
	}
	
	function startTag_users($parser, $data) {
	    global $current_tag;
	    global $xml_counter;
	    global $xml_content;
	    
	    $current_tag .= "*$data";
	    
	    if ($data == "USER") {
            $xml_content[$xml_counter] = new xml_user_entry();
	    }
	}
	
	function endTag_users($parser, $data) {
	    global $current_tag;
	    global $xml_counter;
	    global $xml_content;
	    
	    $tag_key = strrpos($current_tag, "*");
	    $current_tag = substr($current_tag, 0, $tag_key);
	    
	    if ($data == "USER") {
            $xml_counter++;
	    }
	}
	
	function content_users($parser, $data) {
	    global $current_tag;
	    global $xml_counter;
	    global $xml_content;
	    
	    switch($current_tag) {
	        case "*DATABASE*USER*HANDLE":
	            $xml_content[$xml_counter]->handle = $data;
	            break;
	        case "*DATABASE*USER*PASSWORD":
	            $xml_content[$xml_counter]->password = $data;
	            break;
	        case "*DATABASE*USER*EMAIL":
	            $xml_content[$xml_counter]->email = $data;
	            break;
	        case "*DATABASE*USER*WEB*DATE":
	            $xml_content[$xml_counter]->web_date = $data;
	            break;
	        case "*DATABASE*USER*WEB*IPADDR":
	            $xml_content[$xml_counter]->web_ipaddr = $data;
	            break;
	        case "*DATABASE*USER*WEB*COUNT":
	            $xml_content[$xml_counter]->web_count = $data;
	            break;
	        case "*DATABASE*USER*GAME*DATE":
	            $xml_content[$xml_counter]->game_date = $data;
	            break;
	        case "*DATABASE*USER*GAME*IPADDR":
	            $xml_content[$xml_counter]->game_ipaddr = $data;
	            break;
	        case "*DATABASE*USER*GAME*COUNT":
	            $xml_content[$xml_counter]->game_count = $data;
	            break;
	    }
	}
	
	///
	
	function xml_md5($handle, $password) {
		if (isset($handle) && isset($password)) {
			
			$handle = strtolower($handle);
			
			$salt  = "Chief among these motives was the overwhelming idea of the great whale himself. ";
			$salt .= "Such a portentous and mysterious monster roused all my curiosity. Then the wild and distant ";
			$salt .= "seas where he rolled his island bulk; the undeliverable, nameless perils of the whale; these, ";
			$salt .= "with all the attending marvels of a thousand Patagonian sights and sounds, helped to sway me ";
			$salt .= "to my wish. With other men, perhaps, such things would not have been inducements; but as for ";
			$salt .= "me, I am tormented with an everlasting itch for things remote. I love to sail forbidden seas, ";
			$salt .= "and land on barbarous coasts. Not ignoring what is good, I am quick to perceive a horror, and ";
			$salt .= "could still be social with it - would they let me - since it is but well to be on friendly ";
			$salt .= "terms with all the inmates of the place one lodges in.";
			return md5(md5(md5($handle . $salt . $password)));
		}
		
		return null;
	}
	
	function xml_authenticate($handle, $password) {
		global $userfolder;
		
		if (isset($handle) && isset($password)) {
			
			$handle = strtolower($handle);
			
			$xml_content = xml_load_users();
			
			for ($i = 0; $i < count($xml_content); $i++) {
			    if (strcasecmp($handle, $xml_content[$i]->handle) == 0 && strcasecmp($password, $xml_content[$i]->password) == 0) {
				    
				    $folder = $userfolder . "/" . $xml_content[$i]->handle;
					if (file_exists($folder) == false) {
						mkdir($folder, 0707);
					}
					
				    $_SESSION['handle'] = $handle;
					$_SESSION['password'] = $password;
					
				    return true;
		        }
			}
		}
		
		return false;
	}
	
	function xml_addUser($handle, $password, $email) {		
		global $database;
		global $userfolder;
		
		return 4;
		
		if (isset($handle) && isset($password) && isset($email)) {
			
			$xml_content = xml_load_users();
			
			for ($i = 0; $i < count($xml_content); $i++) {
			    if (strcasecmp($handle, $xml_content[$i]->handle) == 0) {
					return 2;
		        } else if (strcasecmp($email, $xml_content[$i]->email) == 0) {
					return 3;
		        }
			}
			
			$handle = strtolower($handle);
			
			if ($file = fopen($database, "w")) {
				
				fwrite($file, "<database>\n");
				
				for ($i = 0; $i < count($xml_content); $i++) {
				    fwrite($file, "    <user>\n");
					fwrite($file, "        <handle>" . $xml_content[$i]->handle . "</handle>\n");
					fwrite($file, "        <password>" . $xml_content[$i]->password . "</password>\n");
					fwrite($file, "        <email>" . $xml_content[$i]->email . "</email>\n");
					fwrite($file, "        <web>\n");
					fwrite($file, "            <date>" . $xml_content[$i]->web_date . "</date>\n");
					fwrite($file, "            <ipaddr>" . $xml_content[$i]->web_ipaddr . "</ipaddr>\n");
					fwrite($file, "            <count>" . $xml_content[$i]->web_count . "</count>\n");
					fwrite($file, "        </web>\n");
					fwrite($file, "        <game>\n");
					fwrite($file, "            <date>" . $xml_content[$i]->game_date . "</date>\n");
					fwrite($file, "            <ipaddr>" . $xml_content[$i]->game_ipaddr . "</ipaddr>\n");
					fwrite($file, "            <count>" . $xml_content[$i]->game_count . "</count>\n");
					fwrite($file, "        </game>\n");
					fwrite($file, "    </user>\n");
				}
				
				fwrite($file, "    <user>\n");
				fwrite($file, "        <handle>" . $handle . "</handle>\n");
				fwrite($file, "        <password>" . $password . "</password>\n");
				fwrite($file, "        <email>" . $email . "</email>\n");
			    fwrite($file, "        <web>\n");
				fwrite($file, "            <date>" . gmdate("m/d/y h:i:s \G\M\T") . "</date>\n");
				fwrite($file, "            <ipaddr>" . $_SERVER["REMOTE_ADDR"] . "</ipaddr>\n");
				fwrite($file, "            <count>1</count>\n");
			    fwrite($file, "        </web>\n");
			    fwrite($file, "        <game>\n");
				fwrite($file, "            <date>00/00/00 00:00:00 GMT</date>\n");
				fwrite($file, "            <ipaddr>000.000.000.000</ipaddr>\n");
				fwrite($file, "            <count>0</count>\n");
			    fwrite($file, "        </game>\n");
				fwrite($file, "    </user>\n");
				
				fwrite($file, "</database>");
				
				fclose($file);
				
				mkdir($userfolder . "/" . $handle, 0707);
				
				$_SESSION['handle'] = $handle;
				$_SESSION['password'] = $password;
			
				return 1;
			}
		}
		
		return 0;
	}
	
	function xml_getUser($handle) {
		
		if (isset($handle)) {
			
			$xml_content = xml_load_users();
			
			for ($i = 0; $i < count($xml_content); $i++) {
			    if (strcasecmp($handle, $xml_content[$i]->handle) == 0) {
					return $xml_content[$i];
		        }
			}
		}
		
		return 0;
	}
	
	function xml_setLogin($type, $handle) {
		global $database;
		
		if (isset($type) && isset($handle)) {
			
			$xml_content = xml_load_users();
			
			if ($file = fopen($database, "w")) {
				
				fwrite($file, "<database>\n");
				
				for ($i = 0; $i < count($xml_content); $i++) {
					fwrite($file, "    <user>\n");
					fwrite($file, "        <handle>" . $xml_content[$i]->handle . "</handle>\n");
					fwrite($file, "        <password>" . $xml_content[$i]->password . "</password>\n");
					fwrite($file, "        <email>" . $xml_content[$i]->email . "</email>\n");
				    fwrite($file, "        <web>\n");
			        if (strcasecmp($type, "web") == 0 && strcasecmp($handle, $xml_content[$i]->handle) == 0) {
						fwrite($file, "            <date>" . gmdate("m/d/y h:i:s \G\M\T") . "</date>\n");
						fwrite($file, "            <ipaddr>" . $_SERVER["REMOTE_ADDR"] . "</ipaddr>\n");
						fwrite($file, "            <count>" . ($xml_content[$i]->web_count + 1) . "</count>\n");
			        } else {
						fwrite($file, "            <date>" . $xml_content[$i]->web_date . "</date>\n");
						fwrite($file, "            <ipaddr>" . $xml_content[$i]->web_ipaddr . "</ipaddr>\n");
						fwrite($file, "            <count>" . $xml_content[$i]->web_count . "</count>\n");
			        }
				    fwrite($file, "        </web>\n");
				    fwrite($file, "        <game>\n");
			        if (strcasecmp($type, "game") == 0 && strcasecmp($handle, $xml_content[$i]->handle) == 0) {
						fwrite($file, "            <date>" . gmdate("m/d/y h:i:s \G\M\T") . "</date>\n");
						fwrite($file, "            <ipaddr>" . $_SERVER["REMOTE_ADDR"] . "</ipaddr>\n");
						fwrite($file, "            <count>" . ($xml_content[$i]->game_count + 1) . "</count>\n");
			        } else {
						fwrite($file, "            <date>" . $xml_content[$i]->game_date . "</date>\n");
						fwrite($file, "            <ipaddr>" . $xml_content[$i]->game_ipaddr . "</ipaddr>\n");
						fwrite($file, "            <count>" . $xml_content[$i]->game_count . "</count>\n");
			        }
				    fwrite($file, "        </game>\n");
					fwrite($file, "    </user>\n");
				}
				
				fwrite($file, "</database>");
				
				fclose($file);
				
				return 1;
			}
		}
		
		return 0;
	}
	
	function xml_getIP($type, $handle) {
		global $database;
		
		if (isset($type) && isset($handle)) {
			
			$xml_content = xml_load_users();
			
			for ($i = 0; $i < count($xml_content); $i++) {
				if (strcasecmp($handle, $xml_content[$i]->handle) == 0) {
			        if (strcasecmp($type, "web") == 0) {
						return $xml_content[$i]->web_ipaddr;
			        } else if (strcasecmp($type, "game") == 0) {
						return $xml_content[$i]->game_ipaddr;
			        }
		        }
			}
		}
		
		return "000.000.000.000";
	}
	
	function xml_editUser($handle, $password, $email) {
		global $database;
		
		if (isset($handle) && isset($email)) {
			
			$xml_content = xml_load_users();
			
			for ($i = 0; $i < count($xml_content); $i++) {
			    if (strcasecmp($email, $xml_content[$i]->email) == 0 &&
			    	strcasecmp($handle, $xml_content[$i]->handle) != 0) {
					return 2;
		        }
			}
			
			if ($file = fopen($database, "w")) {
				
				fwrite($file, "<database>\n");
				
				for ($i = 0; $i < count($xml_content); $i++) {
					fwrite($file, "    <user>\n");
					fwrite($file, "        <handle>" . $xml_content[$i]->handle . "</handle>\n");
			        if (strcasecmp($handle, $xml_content[$i]->handle) == 0) {
				        fwrite($file, "        <password>" . $password . "</password>\n");
						fwrite($file, "        <email>" . $email . "</email>\n");
			        } else {
						fwrite($file, "        <password>" . $xml_content[$i]->password . "</password>\n");
						fwrite($file, "        <email>" . $xml_content[$i]->email . "</email>\n");
			        }
				    fwrite($file, "        <web>\n");
					fwrite($file, "            <date>" . $xml_content[$i]->web_date . "</date>\n");
					fwrite($file, "            <ipaddr>" . $xml_content[$i]->web_ipaddr . "</ipaddr>\n");
					fwrite($file, "            <count>" . $xml_content[$i]->web_count . "</count>\n");
				    fwrite($file, "        </web>\n");
				    fwrite($file, "        <game>\n");
					fwrite($file, "            <date>" . $xml_content[$i]->game_date . "</date>\n");
					fwrite($file, "            <ipaddr>" . $xml_content[$i]->game_ipaddr . "</ipaddr>\n");
					fwrite($file, "            <count>" . $xml_content[$i]->game_count . "</count>\n");
				    fwrite($file, "        </game>\n");
					fwrite($file, "    </user>\n");
				}
				
				fwrite($file, "</database>");
				
				fclose($file);
				
				$_SESSION['password'] = $password;
				
				return 1;
			}
		}
		
		return 0;
	}
	
	function xml_addScript($handle, $filename) {
		global $userfolder;	
		
		if (isset($handle) && isset($filename)) {
			
			$handle = strtolower($handle);
			
			if ($file = fopen($userfolder . "/" . $handle . "/" . $filename . ".xml", "r")) {
				fclose($file);
				return 2;
			}
			
			return xml_setScript($handle, $filename, instruc_c2a(array(new xml_script_settings(), array(new xml_instruction_entry()))));
		}
		
		return 0;
	}
	
	function xml_setScript($handle, $filename, $script) {
		global $userfolder;
		global $scriptItem;
		global $scriptVersion;
		
		if (isset($handle) && isset($filename) && isset($script)) {
			
			$handle = strtolower($handle);
			
			if ($file = fopen($userfolder . "/" . $handle . "/" . $filename . ".xml", "w")) {
				
				fwrite($file, "<script version=\"" . $scriptVersion . "\">\n");
				
				fwrite($file, "    <setting>\n");
				fwrite($file, "        <script_screen_sizeA>" . $script[0]->script_screen_sizeA . "</script_screen_sizeA>\n");
				fwrite($file, "        <script_screen_sizeB>" . $script[0]->script_screen_sizeB . "</script_screen_sizeB>\n");
				fwrite($file, "        <script_useHealthKey>" . $script[0]->script_useHealthKey . "</script_useHealthKey>\n");
				fwrite($file, "        <script_HealthPercent>" . $script[0]->script_HealthPercent . "</script_HealthPercent>\n");
				fwrite($file, "        <script_HealthCode>" . $script[0]->script_HealthCode . "</script_HealthCode>\n");
				fwrite($file, "        <script_useManaKey>" . $script[0]->script_useManaKey . "</script_useManaKey>\n");
				fwrite($file, "        <script_ManaPercent>" . $script[0]->script_ManaPercent . "</script_ManaPercent>\n");
				fwrite($file, "        <script_ManaCode>" . $script[0]->script_ManaCode . "</script_ManaCode>\n");
				fwrite($file, "    </setting>\n");
				
				for ($i = 0; $i < count($script[1]); $i++) {
					fwrite($file, "    <instruction>\n");
					
					for ($j = 0; $j < count($scriptItem[1]); $j++) {
						if ($script[1][$i][$j] != $scriptItem[1][$j][2]) {
							fwrite($file, "        <" . $scriptItem[1][$j][0] . ">" . $script[1][$i][$j] . "</" . $scriptItem[1][$j][0] . ">\n");
						}
					}
					
					fwrite($file, "    </instruction>\n");
				}
				
				fwrite($file, "</script>");
				
				fclose($file);
				
				return 1;
			}
		}
		
		return 0;
	}
	
	function xml_recoverHandle($email) {
		if (isset($email)) {
			
			$xml_content = xml_load_users();
			
			for ($i = 0; $i < count($xml_content); $i++) {
			    if (strcasecmp($email, $xml_content[$i]->email) == 0) {
					$to = $xml_content[$i]->handle . "<" . $xml_content[$i]->email . ">";
					$subject = $xml_content[$i]->handle . ": handle recover";
					$body = "A handle recover was issued from: " . $_SERVER["REMOTE_ADDR"] . "\n";
					$body .= "The handle belonging to this email is: " . $xml_content[$i]->handle;
					//$body .= "\n\nhttp://calc.xentales.com/XenDLL/Computron/login.php";
					mail($to, $subject, $body, "From: " . $to);
					return 1;
		        }
			}
			
			return 2;
		}
		
		return 0;
	}
	
	function xml_resetPassword($handle, $email) {
		global $database;
		
		if (isset($handle) && isset($email)) {
			
			$xml_content = xml_load_users();
			
			for ($i = 0; $i < count($xml_content); $i++) {
			    if (strcasecmp($handle, $xml_content[$i]->handle) == 0) {
				    if ($email != $xml_content[$i]->email) {
						return 2;
			        }
			        $userFound = true;
		        }
			}
			
			if ($userFound == false) {
				return 2;
			}
			
			if ($file = fopen($database, "w")) {
				
				fwrite($file, "<database>\n");
				
				for ($i = 0; $i < count($xml_content); $i++) {
					fwrite($file, "    <user>\n");
					fwrite($file, "        <handle>" . $xml_content[$i]->handle . "</handle>\n");
			        
					if (strcasecmp($handle, $xml_content[$i]->handle) == 0) {
				        $newPassword = substr(md5(gmdate("mdyhis") . $xml_content[$i]->handle), 0, 10);
						fwrite($file, "        <password>" . xml_md5($xml_content[$i]->handle, $newPassword) . "</password>\n");
						$to = $xml_content[$i]->handle . "<" . $xml_content[$i]->email . ">";
						$subject = $xml_content[$i]->handle . ": password reset";
						$body = "A password reset was issued from: " . $_SERVER["REMOTE_ADDR"] . "\n";
						$body .= "Your password has been reset to: " . $newPassword;
						//$body .= "\n\nhttp://calc.xentales.com/XenDLL/Computron/login.php";
						mail($to, $subject, $body, "From: " . $to);
			        } else {
						fwrite($file, "        <password>" . $xml_content[$i]->password . "</password>\n");
			        }
			        
					fwrite($file, "        <email>" . $xml_content[$i]->email . "</email>\n");
				    fwrite($file, "        <web>\n");
			        fwrite($file, "            <date>" . $xml_content[$i]->web_date . "</date>\n");
					fwrite($file, "            <ipaddr>" . $xml_content[$i]->web_ipaddr . "</ipaddr>\n");
					fwrite($file, "            <count>" . $xml_content[$i]->web_count . "</count>\n");
				    fwrite($file, "        </web>\n");
				    fwrite($file, "        <game>\n");
					fwrite($file, "            <date>" . $xml_content[$i]->game_date . "</date>\n");
					fwrite($file, "            <ipaddr>" . $xml_content[$i]->game_ipaddr . "</ipaddr>\n");
					fwrite($file, "            <count>" . $xml_content[$i]->game_count . "</count>\n");
				    fwrite($file, "        </game>\n");
					fwrite($file, "    </user>\n");
				}
				
				fwrite($file, "</database>");
				
				fclose($file);
				
				return 1;
			}
		}
		
		return 0;
		
	}
	
	function valid_email($email) {
		return ereg("^(.{1,}@.{1,}\\..{1,})$", $email);
	}
	
	function valid_alphabetic($text, $low, $high) {
		if (isset($text) == false) {
			return 2;
		} else if (isset($low) && isset($high) && ereg("^([a-zA-Z]{" . $low . "," . $high . "})$", $text)) {
			return 1;
		} else if (isset($low) && !isset($high) && ereg("^([a-zA-Z]{" . $low . ",})$", $text)) {
			return 1;
		} else if (!isset($low) && isset($high) && ereg("^([a-zA-Z]{0," . $high . "})$", $text)) {
			return 1;
		}
		
		return 0;
	}
	
?>