<?php

	$scriptVersion = 2;

	$scriptItem = array(
		array(
			array("script_screen_sizeA", "number", "0"),
			array("script_screen_sizeB", "number", "0"),
			
			array("script_useHealthKey", "bool", "false"),
			array("script_HealthPercent", "number", "0"),
			array("script_HealthCode", "string", "0x00"),
			
			array("script_useManaKey", "bool", "false"),
			array("script_ManaPercent", "number", "0"),
			array("script_ManaCode", "string", "0x00")),
		
		array(
			array("instruc_isSpecial", "bool", "false"),
			array("instruc_clearReturnStack", "bool", "false"),
			array("instruc_useKeystroke", "bool", "false"),
			array("instruc_keystrokeCode", "string", "0x00"),
			array("instruc_useMouseClick", "bool", "false"),
			array("instruc_mouseClickX", "number", "0"),
			array("instruc_mouseClickY", "number", "0"),
			array("instruc_sendPacket", "bool", "false"),
			array("instruc_packetFile", "file", ""),
			array("instruc_pauseA", "number", "0"),
			array("instruc_pauseB", "number", "0"),
			array("instruc_comment", "string", ""),
			array("instruc_useTimedUse", "bool", "false"),
			array("instruc_TimedUseA", "number", "0"),
			array("instruc_TimedUseB", "number", "0"),
			array("instruc_notFirstTimedUse", "bool", "false"),
			
			array("safety_surfMin", "number", "0"),
			array("safety_surfMax", "number", "0"),
			array("safety_useSurfCheck", "bool", "false"),
			array("safety_useSurfCheckcon", "bool", "false"),
			array("safety_useCoordCheck", "bool", "false"),
			array("safety_useCoordCheckcon", "bool", "false"),
			array("safety_CoordX", "number", "0"),
			array("safety_CoordY", "number", "0"),
			array("safety_useMPWait", "bool", "false"),
			array("safety_useHPWait", "bool", "false"),
			
			array("warnings_takeScreenshot", "bool", "false"),
			array("warnings_playAudio", "bool", "false"),
			array("warnings_audioFile", "file", ""),
			array("warnings_jump", "bool", "false"),
			array("warnings_jumpA", "number", "0"),
			array("warnings_jumpB", "number", "0"),
			array("warnings_jal", "bool", "false")));
	
	class xml_script_settings {
		
		var $script_screen_sizeA = "0";
		var $script_screen_sizeB = "0";
		
		var $script_useHealthKey = "false";
		var $script_HealthPercent = "0";
		var $script_HealthCode = "0x00";
		
		var $script_useManaKey = "false";
		var $script_ManaPercent = "0";
		var $script_ManaCode = "0x00";
	}
	
	class xml_instruction_entry {
		
		var $instruc_isSpecial = "false";
		var $instruc_clearReturnStack = "false";
		var $instruc_useKeystroke = "false";
		var $instruc_keystrokeCode = "0x00";
		var $instruc_useMouseClick = "false";
		var $instruc_mouseClickX = "0";
		var $instruc_mouseClickY = "0";
		var $instruc_sendPacket = "false";
		var $instruc_packetFile = "";
		var $instruc_pauseA = "0";
		var $instruc_pauseB = "0";
		var $instruc_comment = "";
		var $instruc_useTimedUse = "false";
		var $instruc_TimedUseA = "0";
		var $instruc_TimedUseB = "0";
		var $instruc_notFirstTimedUse = "false";
				
		var $safety_surfMin = "0";
		var $safety_surfMax = "0";
		var $safety_useSurfCheck = "false";
		var $safety_useSurfCheckcon = "false";
		var $safety_useCoordCheck = "false";
		var $safety_useCoordCheckcon = "false";
		var $safety_CoordX = "0";
		var $safety_CoordY = "0";
		var $safety_useMPWait = "false";
		var $safety_useHPWait = "false";
		
		var $warnings_takeScreenshot = "false";
		var $warnings_playAudio = "false";
		var $warnings_audioFile = "";
		var $warnings_jump = "false";
		var $warnings_jumpA = "0";
		var $warnings_jumpB = "0";
		var $warnings_jal = "false";
		
		var $tracker_jumpA;
		var $tracker_jumpB;
	}
	
	// parse xml for instructions
	
	function xml_load_instructions($handle, $filename) {
		global $xml_content;
		global $xml_counter;
		
		global $userfolder;
		
		$filename = $userfolder . "/" . $handle . "/" . $filename . ".xml";
		
		$xml_content = array(new xml_script_settings(), array());
		$xml_counter = 0;
		
		$xml_parser = xml_parser_create();
		
		xml_set_element_handler($xml_parser, "startTag_instructions", "endTag_instructions");
		
		xml_set_character_data_handler($xml_parser, "content_instructions");
		
		$fp = fopen($filename, "r") or die("Could not open file");
		
		$data = fread($fp, filesize($filename)) or die("Could not read file");
		
		xml_parse($xml_parser, $data, feof($fp)) or die("Error on line " . xml_get_current_line_number($xml_parser));
		
		xml_parser_free($xml_parser);
		
		fclose($fp);
		
		return $xml_content;
	}
	
	function instruc_c2a($c) {
		if (isset($c)) {
			$script = array($c[0], array());
			for ($i = 0; $i < count($c[1]); $i++) {
				$script[1][$i] = array(
					$c[1][$i]->instruc_isSpecial,
					$c[1][$i]->instruc_clearReturnStack,
					$c[1][$i]->instruc_useKeystroke,
					$c[1][$i]->instruc_keystrokeCode,
					$c[1][$i]->instruc_useMouseClick,
					$c[1][$i]->instruc_mouseClickX,
					$c[1][$i]->instruc_mouseClickY,
					$c[1][$i]->instruc_sendPacket,
					$c[1][$i]->instruc_packetFile,
					$c[1][$i]->instruc_pauseA,
					$c[1][$i]->instruc_pauseB,
					$c[1][$i]->instruc_comment,
					$c[1][$i]->instruc_useTimedUse,
					$c[1][$i]->instruc_TimedUseA,
					$c[1][$i]->instruc_TimedUseB,
					$c[1][$i]->instruc_notFirstTimedUse,
					
					$c[1][$i]->safety_surfMin,
					$c[1][$i]->safety_surfMax,
					$c[1][$i]->safety_useSurfCheck,
					$c[1][$i]->safety_useSurfCheckcon,
					$c[1][$i]->safety_useCoordCheck,
					$c[1][$i]->safety_useCoordCheckcon,
					$c[1][$i]->safety_CoordX,
					$c[1][$i]->safety_CoordY,
					$c[1][$i]->safety_useMPWait,
					$c[1][$i]->safety_useHPWait,
					
					$c[1][$i]->warnings_takeScreenshot,
					$c[1][$i]->warnings_playAudio,
					$c[1][$i]->warnings_audioFile,
					$c[1][$i]->warnings_jump,
					$c[1][$i]->warnings_jumpA,
					$c[1][$i]->warnings_jumpB,
					$c[1][$i]->warnings_jal);
			}
		}
		
		return $script;
	}
	
	function startTag_instructions($parser, $data) {
	    global $current_tag;
	    global $xml_counter;
	    global $xml_content;
	    
	    $current_tag .= "*$data";
	    
	    if ($data == "INSTRUCTION") {
            $xml_content[1][$xml_counter] = new xml_instruction_entry();
	    }
	}
	
	function endTag_instructions($parser, $data) {
	    global $current_tag;
	    global $xml_counter;
	    global $xml_content;
	    
	    $tag_key = strrpos($current_tag, "*");
	    $current_tag = substr($current_tag, 0, $tag_key);
	    
	    if ($data == "INSTRUCTION") {
            $xml_counter++;
	    }
	}
	
	function content_instructions($parser, $data) {
	    global $current_tag;
	    global $xml_counter;
	    global $xml_content;
	    
	    switch($current_tag) {
	        case "*SCRIPT*SETTING*SCRIPT_SCREEN_SIZEA":
	            $xml_content[0]->script_screen_sizeA = $data;
	            break;
	        case "*SCRIPT*SETTING*SCRIPT_SCREEN_SIZEB":
	            $xml_content[0]->script_screen_sizeB = $data;
	            break;
	        
	        case "*SCRIPT*SETTING*SCRIPT_USEHEALTHKEY":
	            $xml_content[0]->script_useHealthKey = $data;
	            break;
	        case "*SCRIPT*SETTING*SCRIPT_HEALTHPERCENT":
	            $xml_content[0]->script_HealthPercent = $data;
	            break;
	        case "*SCRIPT*SETTING*SCRIPT_HEALTHCODE":
	            $xml_content[0]->script_HealthCode = $data;
	            break;
	        
	        case "*SCRIPT*SETTING*SCRIPT_USEMANAKEY":
	            $xml_content[0]->script_useManaKey = $data;
	            break;
	        case "*SCRIPT*SETTING*SCRIPT_MANAPERCENT":
	            $xml_content[0]->script_ManaPercent = $data;
	            break;
	        case "*SCRIPT*SETTING*SCRIPT_MANACODE":
	            $xml_content[0]->script_ManaCode = $data;
	            break;
	        
	        case "*SCRIPT*INSTRUCTION*INSTRUC_ISSPECIAL":
	            $xml_content[1][$xml_counter]->instruc_isSpecial = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*INSTRUC_CLEARRETURNSTACK":
	            $xml_content[1][$xml_counter]->instruc_clearReturnStack = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*INSTRUC_USEKEYSTROKE":
	            $xml_content[1][$xml_counter]->instruc_useKeystroke = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*INSTRUC_KEYSTROKECODE":
	            $xml_content[1][$xml_counter]->instruc_keystrokeCode = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*INSTRUC_USEMOUSECLICK":
	            $xml_content[1][$xml_counter]->instruc_useMouseClick = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*INSTRUC_MOUSECLICKX":
	            $xml_content[1][$xml_counter]->instruc_mouseClickX = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*INSTRUC_MOUSECLICKY":
	            $xml_content[1][$xml_counter]->instruc_mouseClickY = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*INSTRUC_SENDPACKET":
	            $xml_content[1][$xml_counter]->instruc_sendPacket = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*INSTRUC_PACKETFILE":
	            $xml_content[1][$xml_counter]->instruc_packetFile = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*INSTRUC_PAUSEA":
	            $xml_content[1][$xml_counter]->instruc_pauseA = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*INSTRUC_PAUSEB":
	            $xml_content[1][$xml_counter]->instruc_pauseB = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*INSTRUC_COMMENT":
	            $xml_content[1][$xml_counter]->instruc_comment = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*INSTRUC_USETIMEDUSE":
	            $xml_content[1][$xml_counter]->instruc_useTimedUse = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*INSTRUC_TIMEDUSEA":
	            $xml_content[1][$xml_counter]->instruc_TimedUseA = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*INSTRUC_TIMEDUSEB":
	            $xml_content[1][$xml_counter]->instruc_TimedUseB = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*INSTRUC_NOTFIRSTTIMEDUSE":
	            $xml_content[1][$xml_counter]->instruc_notFirstTimedUse = $data;
	            break;
			
			case "*SCRIPT*INSTRUCTION*SAFETY_SURFMIN":
	            $xml_content[1][$xml_counter]->safety_surfMin = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*SAFETY_SURFMAX":
	            $xml_content[1][$xml_counter]->safety_surfMax = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*SAFETY_USESURFCHECK":
	            $xml_content[1][$xml_counter]->safety_useSurfCheck = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*SAFETY_USESURFCHECKCON":
	            $xml_content[1][$xml_counter]->safety_useSurfCheckcon = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*SAFETY_USECOORDCHECK":
	            $xml_content[1][$xml_counter]->safety_useCoordCheck = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*SAFETY_USECOORDCHECKCON":
	            $xml_content[1][$xml_counter]->safety_useCoordCheckcon = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*SAFETY_COORDX":
	            $xml_content[1][$xml_counter]->safety_CoordX = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*SAFETY_COORDY":
	            $xml_content[1][$xml_counter]->safety_CoordY = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*SAFETY_USEMPWAIT":
	            $xml_content[1][$xml_counter]->safety_useMPWait = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*SAFETY_USEHPWAIT":
	            $xml_content[1][$xml_counter]->safety_useHPWait = $data;
	            break;
			
			case "*SCRIPT*INSTRUCTION*WARNINGS_TAKESCREENSHOT":
	            $xml_content[1][$xml_counter]->warnings_takeScreenshot = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*WARNINGS_PLAYAUDIO":
	            $xml_content[1][$xml_counter]->warnings_playAudio = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*WARNINGS_AUDIOFILE":
	            $xml_content[1][$xml_counter]->warnings_audioFile = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*WARNINGS_JUMP":
	            $xml_content[1][$xml_counter]->warnings_jump = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*WARNINGS_JUMPA":
	            $xml_content[1][$xml_counter]->warnings_jumpA = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*WARNINGS_JUMPB":
	            $xml_content[1][$xml_counter]->warnings_jumpB = $data;
	            break;
			case "*SCRIPT*INSTRUCTION*WARNINGS_JAL":
	            $xml_content[1][$xml_counter]->warnings_jal = $data;
	            break;
	    }
	}
	
?>