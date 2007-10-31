<?php include_once("session.php"); ?>
<?php include_once("authorize.php"); ?>
<?php include_once("header.php"); ?>

	<?php
	
		if (isset($_GET['script']) == false) {
			header("Location: script.php");
		}
		
		$filename = $_GET['script'];
		
		if (file_exists($userfolder . "/" . $xml_user->handle . "/" . $filename . ".xml") == false) {
			header("Location: script.php");
		}
		
		$search = array("<", ">");
		$replace = array("[", "]");
		if (isset($_POST['instruc_isSpecial0'])) {
			
			$script = array(new xml_script_settings(), array());
			
			$script[0]->script_screen_sizeA = str_replace($search, $replace, $_POST['script_screen_sizeA']);
			$script[0]->script_screen_sizeB = str_replace($search, $replace, $_POST['script_screen_sizeB']);
			
			if ($_POST['script_useHealthKey'] == "on") {
				$script[0]->script_useHealthKey = "true";
			} else {
				$script[0]->script_useHealthKey = "false";
			}
			$script[0]->script_HealthPercent = str_replace($search, $replace, $_POST['script_HealthPercent']);
			$script[0]->script_HealthCode = str_replace($search, $replace, $_POST['script_HealthCode']);
			
			if ($_POST['script_useManaKey'] == "on") {
				$script[0]->script_useManaKey = "true";
			} else {
				$script[0]->script_useManaKey = "false";
			}
			$script[0]->script_ManaPercent = str_replace($search, $replace, $_POST['script_ManaPercent']);
			$script[0]->script_ManaCode = str_replace($search, $replace, $_POST['script_ManaCode']);
			
			for ($i = 0; isset($_POST['instruc_isSpecial' . $i]); $i++) {
				for ($j = 0; $j < count($scriptItem[1]); $j++) {
					$script[1][$i][$j] = str_replace($search, $replace, $_POST[$scriptItem[1][$j][0] . $i]);
				}
			}
			
			if (xml_setScript($xml_user->handle, $filename, $script)) {
				$confirm_msg .= "<li>Your script has been saved.</li>\n";
				unset($script);
			} else {
				$error_msg .= "<li>Error saving script.</li>\n";
			}
		}
		
	?>
	
	<script language="javascript">
		var DIK = new Array (
			<?php
			
				for ($i = 0; $i < count($DIK); $i++) {
					print('new Array (\'' . $DIK[$i] . '\', \'');
					if ($DIK[$i] != "0x00") {
						print(substr($DIK[$i], 4));
					}
					print('\')');
					if ($i < count($DIK)-1) {
						print(', ');
					}
				}
				
			?>);
		
		var script = new Array (
			<?php
			
				if (isset($script) == false) {
					$script = instruc_c2a(xml_load_instructions($xml_user->handle, $filename));
				}
				
				for ($i = 0; $i < count($script[1]); $i++) {
					print('new Array (');
					
					for ($j = 0; $j < count($scriptItem[1]); $j++) {
						if ($scriptItem[1][$j][1] == "bool" || $scriptItem[1][$j][1] == "number") {
							print($script[1][$i][$j]);
						} else {
							print('\'' . $script[1][$i][$j] . '\'');
						}
						if ($j < count($scriptItem[1])-1) {
							print(', ');
						}
					}
					
					print(')');
					
					if ($i < count($script[1])-1) {
						print(', ');
					}
				}
				
			?>);
		
		<?php
		
			for ($i = 0; $i < count($scriptItem[1]); $i++) {
				print('var m_' . $scriptItem[1][$i][0] . ' = ' . $i . ';');
				print("\n");
			}
			
		?>
		
		var scriptItem = new Array (
			<?php
			
				for ($i = 0; $i < count($scriptItem); $i++) {
					print('new Array (');
					
					for ($j = 0; $j < count($scriptItem[$i]); $j++) {
						print('new Array (\'' . $scriptItem[$i][$j][0] . '\', \'' . $scriptItem[$i][$j][1] . '\', ');
						if ($scriptItem[$i][$j][1] == "bool" || $scriptItem[$i][$j][1] == "number") {
							print($scriptItem[$i][$j][2] . ')');
						} else {
							print('\'' . $scriptItem[$i][$j][2] . '\')');
						}
						if ($j < count($scriptItem[$i])-1) {
							print(', ');
						}
					}
					
					print(')');
					
					if ($i < count($scriptItem)-1) {
						print(', ');
					}
				}
				
			?>);
		
		function current_instruc() {
			return new Array (
				<?php
				
					for ($i = 0; $i < count($scriptItem[1]); $i++) {
						if ($scriptItem[1][$i][1] == "bool") {
							print('document.getElementById(\'' . $scriptItem[1][$i][0] . '\').checked');
						} else if ($scriptItem[1][$i][1] == "number") {
							print('Number(document.getElementById(\'' . $scriptItem[1][$i][0] . '\').value)');
						} else if ($scriptItem[1][$i][1] == "file") {
							print('document.getElementById(\'text_' . $scriptItem[1][$i][0] . '\').value');
						} else {
							for ($j = 0; $j < count($search); $j++) {
								print('(');
							}
							
							print('document.getElementById(\'' . $scriptItem[1][$i][0] . '\').value');
							
							for ($j = 0; $j < count($search); $j++) {
								if (isset($replace[$j])) {
									print(').replace(/' . $search[$j] . '/g, "' . $replace[$j] . '")');
								} else {
									print(').replace(/' . $search[$j] . '/g, "")');
								}
							}
						}
						
						if ($i < count($scriptItem[1])-1) {
							print(', ');
						}
					}
					
				?>);
		}
		
		function set_current(index) {
			document.getElementById('instruc_position').value = index;
			if (index > -1 && index < script.length) {
				document.getElementById('instruc_save').disabled = false;
				setSpanText('span_position', index);
			} else {
				document.getElementById('instruc_save').disabled = true;
				setSpanText('span_position', "");
			}
		}
		
		function save_instruc() {
			var index = document.getElementById('instruc_position').value;
			if (index > -1 && index < script.length) {
				script[index] = current_instruc();
				refresh_list(index, index);
			}
		}
		
		function insert_instruc(index) {
			if (index > -1 && index < script.length+1) {
				script.splice(index, 0, current_instruc());
				set_current(index);
				set_list();
			}
		}
		
		function delete_instruc(index) {
			if (index > -1 && index < script.length) {
				script.splice(index, 1);
				if (script.length == 0) {
					display_instruc(-1);
					insert_instruc(0);
				} else {
					if (index == document.getElementById('instruc_position').value) {
						set_current(-1);
					} else if (index < document.getElementById('instruc_position').value) {
						set_current(document.getElementById('instruc_position').value-1);
					}
					set_list();
				}
			}
		}
		
		function up_instruc(index) {
			if (index > 0 && index < script.length) {
				var instruction = script[index];
				script.splice(index, 1);
				script.splice(index-1, 0, instruction);
				if (index == document.getElementById('instruc_position').value) {
					set_current(index-1);
				}
				refresh_list(index-1, index);
			}
		}
		
		function down_instruc(index) {
			if (index > -1 && index < script.length-1) {
				var instruction = script[index];
				script.splice(index, 1);
				script.splice(index+1, 0, instruction);
				if (index == document.getElementById('instruc_position').value) {
					set_current(index+1);
				}
				refresh_list(index, index+1);
			}
		}
		
		function display_instruc(index) {
			var _time_maximum = document.getElementById('time_maximum').checked;
			var _time_minimum = document.getElementById('time_minimum').checked;
			var _time_random = document.getElementById('time_random').checked;
			var _hide_comments = document.getElementById('hide_comments').checked;
			
			var _script_screen_sizeA = document.getElementById('script_screen_sizeA').value;
			var _script_screen_sizeB = document.getElementById('script_screen_sizeB').value;
			var _script_useHealthKey = document.getElementById('script_useHealthKey').checked;
			var _script_HealthPercent = document.getElementById('script_HealthPercent').value;
			var _script_HealthCode = document.getElementById('script_HealthCode').value;
			var _script_useManaKey = document.getElementById('script_useManaKey').checked;
			var _script_ManaPercent = document.getElementById('script_ManaPercent').value;
			var _script_ManaCode = document.getElementById('script_ManaCode').value;
			
			document.getElementById('form_transformer').reset();
			
			document.getElementById('time_maximum').checked = _time_maximum;
			document.getElementById('time_minimum').checked = _time_minimum;
			document.getElementById('time_random').checked = _time_random;
			document.getElementById('hide_comments').checked = _hide_comments;
			
			document.getElementById('script_screen_sizeA').value = _script_screen_sizeA;
			document.getElementById('script_screen_sizeB').value = _script_screen_sizeB;
			document.getElementById('script_useHealthKey').checked = _script_useHealthKey;
			document.getElementById('script_HealthPercent').value = _script_HealthPercent;
			document.getElementById('script_HealthCode').value = _script_HealthCode;
			document.getElementById('script_useManaKey').checked = _script_useManaKey;
			document.getElementById('script_ManaPercent').value = _script_ManaPercent;
			document.getElementById('script_ManaCode').value = _script_ManaCode;
			
			for (var i = 0; i < scriptItem[1].length; i++) {
				var value;
				if (index > -1 && index < script.length) {
					value = script[index][i];
				} else {
					value = scriptItem[1][i][2];
				}
				
				if (scriptItem[1][i][1] == "bool") {
					document.getElementById(scriptItem[1][i][0]).checked = value;
				} else if (scriptItem[1][i][1] == "file") {
					document.getElementById('text_' + scriptItem[1][i][0]).value = value;
				} else {
					document.getElementById(scriptItem[1][i][0]).value = value;
				}
			}
			
			set_current(index);
		}
		
		function set_list() {
			var text = '';
			for (i = 0; i < script.length+1; i++) {
				text += '<div id="list_instruc' + i + '" name="list_instruc' + i + '" style="text-align: left;"></div>';
			}
			setSpanText('list_root', text);
			refresh_list(0, -1);
		}
		
		function refresh_pause() {
			var pause = 0;
			for (i = 0; i < script.length; i++) {
				
													// pause = 7000
				var minutes = Math.floor(pause/60);	// minutes = 116
				var seconds = pause - (minutes*60);	// seconds = 40
				var hours = Math.floor(minutes/60);	// hours = 1
				minutes = minutes - (hours*60);		// minutes = 56
													// pause = seconds + (minutes*60) + ((hours*60)*60)
													// pause = 40 + (56*60) + ((1*60)*60)
													// pause = 40 + 3360 + 3600
													// pause = 7000
				
				var time = '';
				if (hours < 10) {
					time += '0';
				}
				time += hours + ':';
				
				if (minutes < 10) {
					time += '0';
				}
				time += minutes + ':';
				
				if (seconds < 10) {
					time += '0';
				}
				time += seconds;
				
				setSpanText('list_pause' + i, time);
				
				if (document.getElementById('time_minimum').checked) {
					pause += script[i][m_instruc_pauseA];
				} else if (document.getElementById('time_maximum').checked) {
					pause += script[i][m_instruc_pauseB];
				} else if (document.getElementById('time_random').checked) {
					var limit = (script[i][m_instruc_pauseB] - script[i][m_instruc_pauseA]) + 1;
					pause += script[i][m_instruc_pauseA] + Math.floor(Math.random()*limit);
				}
			}
		}
		
		function refresh_list(start, end) {
			if (end == -1) {
				end = script.length;
			}
			
			for (i = start; i < end+1 && i < script.length+1; i++) {
				text = '<a href="javascript:insert_instruc(' + i + ');"><img src="images/add.png" border="0"></a>&nbsp;';
				
				if (i < script.length) {
					text += '<a href="javascript:delete_instruc(' + i + ');"><img src="images/delete.png" border="0"></a>&nbsp;';
				} else {
					text += '<img src="images/blank.png" border="0">&nbsp;';
				}
				
				text += '<span id="list_position' + i + '" name="list_position' + i + '">';
				
				if (i > 0 && i < script.length) {
					text += '<a href="javascript:up_instruc(' + i + ');"><img src="images/up.png" border="0"></a>&nbsp;';
				} else {
					text += '<img src="images/blank.png" border="0">&nbsp;';
				}
				
				if (i < script.length-1) {
					text += '<a href="javascript:down_instruc(' + i + ');"><img src="images/down.png" border="0"></a>&nbsp;';
				} else {
					text += '<img src="images/blank.png" border="0">&nbsp;';
				}
				
				text += '</span>';
				
				if (i < script.length) {
					text += '<a href="javascript:display_instruc(' + i + ');"><span style="font-size: xx-small;">' + i + ' - ';
					text += '<span id="list_pause' + i + '" name="list_pause' + i + '"></span> -> ';
					
					if (script[i][m_instruc_useKeystroke] == true) {
						for (var j = 0; j < DIK.length; j++) {
							if (DIK[j][0] == script[i][m_instruc_keystrokeCode]) {
								text += DIK[j][1];
								break;
							}
						}
					} else if (script[i][m_instruc_useMouseClick] == true) {
						text += script[i][m_instruc_mouseClickX] + ', ' + script[i][m_instruc_mouseClickY];
					} else {
						text += 'Pause Only';
					}
					
					text += '<span id="list_comment' + i + '" name="list_comment' + i + '"';
					
					if (document.getElementById('hide_comments').checked) {
						text += ' class="hidden"';
					}
					
					text += '> - "' + script[i][m_instruc_comment] + '"</span>';
					
					text += '</span></a>';
				}
				
				setSpanText('list_instruc' + i, text);
			}
			
			refresh_pause();
			refresh_comments();
		}
		
		function save_script() {
			for (var i = 0; i < script.length; i++) {
				for (var j = 0; j < scriptItem[1].length; j++) {
					addHiddenElement('rootDiv', scriptItem[1][j][0]+i, script[i][j]);
				}
			}
		}
		
		function lower(lower, upper) {
			var element = document.getElementById(upper);
			if (Number(lower.value) > Number(element.value)) {
				element.value = lower.value;
			}
		}
		
		function upper(lower, upper) {
			var element = document.getElementById(lower);
			if (Number(upper.value) < Number(element.value)) {
				element.value = upper.value;
			}
		}
		
		function number(element) {
			if (element.value == "" || isNaN(element.value)) {
				for (var i = 0; i < scriptItem.length; i++) {
					for (var j = 0; j < scriptItem[i].length; j++) {
						if (scriptItem[i][j][0] == element.id) {
							element.value = scriptItem[i][j][2];
						}
					}
				}
			}
		}
		
		function restrict_command(element) {
			if (element.id == "instruc_clearReturnStack") {
				if (document.getElementById('instruc_useKeystroke').checked ||
					document.getElementById('instruc_useMouseClick').checked ||
					document.getElementById('instruc_sendPacket').checked) {
					document.getElementById('instruc_clearReturnStack').checked = false;
				}
			} else if (element.id == "instruc_useKeystroke") {
				if (document.getElementById('instruc_clearReturnStack').checked ||
					document.getElementById('instruc_useMouseClick').checked ||
					document.getElementById('instruc_sendPacket').checked) {
					document.getElementById('instruc_useKeystroke').checked = false;
				}
			} else if (element.id == "instruc_useMouseClick") {
				if (document.getElementById('instruc_clearReturnStack').checked ||
					document.getElementById('instruc_useKeystroke').checked ||
					document.getElementById('instruc_sendPacket').checked) {
					document.getElementById('instruc_useMouseClick').checked = false;
				}
			} else if (element.id == "instruc_sendPacket") {
				if (document.getElementById('instruc_clearReturnStack').checked ||
					document.getElementById('instruc_useKeystroke').checked ||
					document.getElementById('instruc_useMouseClick').checked) {
					document.getElementById('instruc_sendPacket').checked = false;
				}
			}
		}
		
		function refresh_comments() {
			for (i = 0; i < script.length; i++) {
				if (document.getElementById('hide_comments').checked) {
					document.getElementById('list_comment' + i).style.display = "none";
				} else {
					document.getElementById('list_comment' + i).style.display = "inline";
				}
			}
		}
		
		function set_file(element) {
			var text_element = document.getElementById('text_' + element.id);
			text_element.value = element.value;
		}
		
		function displayTab(id) {
			hideLayer('command', true);
			document.getElementById('tab_command').style.marginTop = "6px";
			document.getElementById('tab_command').style.marginBottom = "6px";
			
			hideLayer('safety_check', true);
			document.getElementById('tab_safety_check').style.marginTop = "6px";
			document.getElementById('tab_safety_check').style.marginBottom = "6px";
			
			hideLayer('warning', true);
			document.getElementById('tab_warning').style.marginTop = "6px";
			document.getElementById('tab_warning').style.marginBottom = "6px";
			
			hideLayer(id, false);
			document.getElementById('tab_' + id).style.marginTop = "12px";
			document.getElementById('tab_' + id).style.marginBottom = "0px";
		}
	</script>
	
	<center>
	
	<?php include_once("message.print.php"); ?>
	
	<br>
	
	<a href="download.script.php?script=<?php print($filename); ?>"><img src="images/download.png" border="0" style="vertical-align: middle;"><span style="font-weight: bold; vertical-align: middle;"> Download</span></a><br>
	
	<br>
	
	<form action="transformer.php?script=<?php print($filename); ?>" method="post" id="form_transformer" name="form_transformer">
	
		<input type="button" value="Clear" onclick="display_instruc(-1);">
		<input type="button" id="instruc_save" name="instruc_save" value="Save Instruction" onclick="save_instruc();" disabled="true">
		<input type="submit" value="Save Script" onclick="save_script(); hideLayer('body', true); hideLayer('processing', false);"><br>
		<br>

		<div class="root" style="width: 800px;">
		
			<div class="header">
				Script: <?php print($filename); ?>
			</div>
			
			<div class="body" style="width: 794px;">
			
				<div class="inline" style="width: 200px;">
				
					<div class="subheader">
						Screen Size
					</div>
					
					<div class="data">
						<a href="#">?</a>
						<label for="script_screen_sizeA">Width x Height</label>
					</div>
					
					<div class="data-indent">
						<input type="text" id="script_screen_sizeA" name="script_screen_sizeA" size="5" value="<?php print($script[0]->script_screen_sizeA); ?>">
						x <input type="text" id="script_screen_sizeB" name="script_screen_sizeB" size="5" value="<?php print($script[0]->script_screen_sizeB); ?>">
					</div>
					
				</div>
				
				<div class="inline" style="margin-left: 3px; width: 294px;">
				
					<div class="subheader">
						Monitor Health
					</div>
					
					<div class="data">
						<a href="#">?</a>
						<input type="checkbox" id="script_useHealthKey" name="script_useHealthKey"
						<?php
						
							if ($script[0]->script_useHealthKey == "true") {
								print(' checked');
							}
							
						?>>
						<label for="script_useHealthKey">Use key if HP is below</label>
						<input type="text" id="script_HealthPercent" name="script_HealthPercent" maxlength="3" size="5" value="<?php print($script[0]->script_HealthPercent); ?>"> %
					</div>
					
					<div class="data-indent">
						<select id="script_HealthCode" name="script_HealthCode">
							<?php
							
								for ($i = 0; $i < count($DIK); $i++) {
									if ($DIK[$i] != "0x00" && $script[0]->script_HealthCode == $DIK[$i]) {
										print('<option value="' . $DIK[$i] . '" selected>' . substr($DIK[$i], 4) . '</option>');
									} else {
										print('<option value="' . $DIK[$i] . '">' . substr($DIK[$i], 4) . '</option>');
									}
								}
								
							?>
						</select>
					</div>
					
				</div>
				
				<div class="inline" style="margin-left: 3px; width: 294px;">
				
					<div class="subheader">
						Monitor Mana
					</div>
					
					<div class="data">
						<a href="#">?</a>
						<input type="checkbox" id="script_useManaKey" name="script_useManaKey"
						<?php
						
							if ($script[0]->script_useManaKey == "true") {
								print(' checked');
							}
							
						?>>
						<label for="script_useManaKey">Use key if MP is below</label>
						<input type="text" id="script_ManaPercent" name="script_ManaPercent" maxlength="3" size="5" value="<?php print($script[0]->script_ManaPercent); ?>"> %
					</div>
					
					<div class="data-indent">
						<select id="script_ManaCode" name="script_ManaCode">
							<?php
							
								for ($i = 0; $i < count($DIK); $i++) {
									if ($DIK[$i] != "0x00" && $script[0]->script_ManaCode == $DIK[$i]) {
										print('<option value="' . $DIK[$i] . '" selected>' . substr($DIK[$i], 4) . '</option>');
									} else {
										print('<option value="' . $DIK[$i] . '">' . substr($DIK[$i], 4) . '</option>');
									}
								}
								
							?>
						</select>
					</div>
					
				</div>
				
				<div class="inline" style="margin-top: 3px; width: 286px;">
				
					<div class="inline">
					
						<div class="data">
							<input type="radio" id="time_maximum" name="time_display">
							<label for="time_maximum">Maximum Time</label>
						</div>
					
					</div>
					
					<div class="inline">
					
						<div class="data">
							<input type="radio" id="time_minimum" name="time_display">
							<label for="time_minimum">Minimum Time</label>
						</div>
						
					</div>
					
					<div class="inline">
					
						<div class="inline" style="width: 50%;">
						
							<div class="data">
								<input type="radio" id="time_random" name="time_display">
								<label for="time_random">Random Time</label>
							</div>
						
						</div>
						
						<div class="inline" style="width: 50%;">
						
							<div class="data">
								<input type="checkbox" id="hide_comments" name="hide_comments">
								<label for="hide_comments">Hide Comments</label>
							</div>
						
						</div>
						
					</div>
					
					<div class="inline">
					
						<div class="data">
							<hr>
						</div>
					
					</div>
					
					<div class="inline">
					
						<div id="list_root" name="list_root" style="margin: 3px; height: 420; white-space: nowrap; overflow: scroll;">
						</div>
						
					</div>
					
					<div class="inline">
					
						<div class="data" style="text-align: center;">
							<input type="button" value="Refresh List" onclick="refresh_list(0, -1);">
						</div>
						
						<div class="data">
							<img src="images/add.png" border="0"> Add new instruction at this position
						</div>
						
						<div class="data">
							<img src="images/delete.png" border="0"> Delete this instruction
						</div>
						
						<div class="data">
							<img src="images/up.png" border="0"> Move up one position
						</div>
						
						<div class="data">
							<img src="images/down.png" border="0"> Move down one position
						</div>
						
					</div>
					
				</div>
				
				<div class="inline" style="margin-top: 3px; margin-left: 3px; background-color: #AAAAAA; width: 505px;">
				
					<div class="header" style="margin: 0px 0px 0px 0px;">
						<input type="hidden" id="instruc_position" name="instruc_position" value="-1">
						Instruction: <span id="span_position" name="span_position"></span>
					</div>
					
					<div id="tab_command" name="tab_command" class="inline" style="margin: 6px 0px 6px 3px; width: 163px;">
					
						<a href="javascript:displayTab('command');">
							<div class="subheader" style="margin-bottom: 3px;">
								Command
							</div>
						</a>
						
					</div>
					
					<div id="tab_safety_check" name="tab_safety_check" class="inline" style="margin: 6px 0px 6px 5px; width: 163px;">
					
						<a href="javascript:displayTab('safety_check');">
							<div class="subheader" style="margin-bottom: 3px;">
								Safety Check
							</div>
						</a>
						
					</div>
					
					<div id="tab_warning" name="tab_warning" class="inline" style="margin: 6px 3px 6px 5px; width: 161px;">
					
						<a href="javascript:displayTab('warning');">
							<div class="subheader" style="margin-bottom: 3px;">
								Warning
							</div>
						</a>
						
					</div>
					
					<div id="command" name="command" class="inline hidden">
					
						<div class="data">
							<a href="#">?</a>
							<input type="checkbox" id="instruc_isSpecial" name="instruc_isSpecial">
							<label for="instruc_isSpecial">Special</label>
						</div>
						
						<div class="data">
							<hr>
						</div>
						
						<div class="data">
							<a href="#">?</a>
							<input type="checkbox" id="instruc_clearReturnStack" name="instruc_clearReturnStack">
							<label for="instruc_clearReturnStack">Clear return stack</label>
						</div>
						
						<div class="data">
							<hr>
						</div>
						
						<div class="data">
							<a href="#">?</a>
							<input type="checkbox" id="instruc_useKeystroke" name="instruc_useKeystroke">
							<label for="instruc_useKeystroke">Simulate keystroke</label>
						</div>
						
						<div class="data-indent">
							<select id="instruc_keystrokeCode" name="instruc_keystrokeCode">
								<?php
									
									for ($i = 0; $i < count($DIK); $i++) {
										print('<option value="' . $DIK[$i] . '">' . substr($DIK[$i], 4) . '</option>');
									}
									
								?>
							</select>
						</div>
						
						<div class="data">
							<hr>
						</div>
						
						<div class="data">
							<a href="#">?</a>
							<input type="checkbox" id="instruc_useMouseClick" name="instruc_useMouseClick">
							<label for="instruc_useMouseClick">Simulate mouse click</label>
						</div>
						
						<div class="data-indent">
							<input type="text" id="instruc_mouseClickX" name="instruc_mouseClickX" size="5">
							, <input type="text" id="instruc_mouseClickY" name="instruc_mouseClickY" size="5">
						</div>
						
						<div class="data">
							<hr>
						</div>
						
						<div class="data">
							<a href="#">?</a>
							<input type="checkbox" id="instruc_sendPacket" name="instruc_sendPacket" disabled>
							<label for="instruc_sendPacket">Send packet</label>
						</div>
						
						<div class="data-indent">
							<input type="text" id="text_instruc_packetFile" name="text_instruc_packetFile" readonly>
						</div>
						
						<div class="data-indent">
							<input type="file" id="instruc_packetFile" name="instruc_packetFile" disabled>
						</div>
						
						<div class="data">
							<hr>
						</div>
						
						<div class="data">
							<a href="#">?</a>
							<label for="instruc_pauseA">Pause length after simualtion</label>
						</div>
						
						<div class="data-indent">
							<input type="text" id="instruc_pauseA" name="instruc_pauseA" size="5">
							to <input type="text" id="instruc_pauseB" name="instruc_pauseB" size="5">
						</div>
						
						<div class="data-indent">
							<input type="checkbox" id="safety_useHPWait" name="safety_useHPWait">
							<label for="safety_useHPWait">Wait for 100% HP</label>
						</div>
						
						<div class="data-indent">
							<input type="checkbox" id="safety_useMPWait" name="safety_useMPWait">
							<label for="safety_useMPWait">Wait for 100% MP</label>
						</div>
						
						<div class="data">
							<hr>
						</div>
						
						<div class="data">
							<a href="#">?</a>
							<input type="checkbox" id="instruc_useTimedUse" name="instruc_useTimedUse">
							<label for="instruc_useTimedUse">Only use every ___ seconds</label>
						</div>
						
						<div class="data-indent">
							<input type="text" id="instruc_TimedUseA" name="instruc_TimedUseA" size="5">
							to <input type="text" id="instruc_TimedUseB" name="instruc_TimedUseB" size="5">
						</div>
						
						<div class="data-indent">
							<input type="checkbox" id="instruc_notFirstTimedUse" name="instruc_notFirstTimedUse">
							<label for="instruc_notFirstTimedUse">Ignore first pass</label>
						</div>
						
						<div class="data">
							<hr>
						</div>
						
						<div class="data">
							<a href="#">?</a>
							<label for="instruc_comment">Instruction comment</label>
						</div>
						
						<div class="data-indent">
							<input type="text" id="instruc_comment" name="instruc_comment">
						</div>
						
					</div>
					
					<div id="safety_check" name="safety_check" class="inline hidden">
					
						<div class="data">
							<a href="#">?</a>
							<input type="checkbox" id="safety_useSurfCheck" name="safety_useSurfCheck">
							<label for="safety_useSurfCheck">Use surf check</label>
						</div>
						
						<div class="data-indent">
							<input type="checkbox" id="safety_useSurfCheckcon" name="safety_useSurfCheckcon">
							<label for="safety_useSurfCheckcon">Continuous surf check</label>
						</div>
						
						<div class="data-indent">
							<label for="safety_surfMin">Surf min</label>
							<input type="text" id="safety_surfMin" name="safety_surfMin" size="5">
						</div>
						
						<div class="data-indent">
							<label for="safety_surfMax">Surf max</label>
							<input type="text" id="safety_surfMax" name="safety_surfMax" size="5">
						</div>
						
						<div class="data">
							<hr>
						</div>
						
						<div class="data">
							<a href="#">?</a>
							<input type="checkbox" id="safety_useCoordCheck" name="safety_useCoordCheck">
							<label for="safety_useCoordCheck">Check char coordinates</label>
						</div>
						
						<div class="data-indent">
							<input type="checkbox" id="safety_useCoordCheckcon" name="safety_useCoordCheckcon">
							<label for="safety_useCoordCheckcon">Continuous coord check</label>
						</div>
						
						<div class="data-indent">
							<input type="text" id="safety_CoordX" name="safety_CoordX" size="5">
							, <input type="text" id="safety_CoordY" name="safety_CoordY" size="5">
						</div>
						
					</div>
					
					<div id="warning" name="warning" class="inline hidden">
					
						<div class="data">
							<a href="#">?</a>
							<input type="checkbox" id="warnings_takeScreenshot" name="warnings_takeScreenshot">
							<label for="warnings_takeScreenshot">Take screenshot</label>
						</div>
						
						<div class="data">
							<hr>
						</div>
						
						<div class="data">
							<a href="#">?</a>
							<input type="checkbox" id="warnings_playAudio" name="warnings_playAudio">
							<label for="warnings_playAudio">Play audio file</label>
						</div>
						
						<div class="data-indent">
							<input type="text" id="text_warnings_audioFile" name="text_warnings_audioFile" readonly>
						</div>
						
						<div class="data-indent">
							<input type="file" id="warnings_audioFile" name="warnings_audioFile">
						</div>
						
						<div class="data">
							<hr>
						</div>
						
						<div class="data">
							<a href="#">?</a>
							<input type="checkbox" id="warnings_jump" name="warnings_jump">
							<label for="warnings_jump">Jump to instruction range</label>
						</div>
						
						<div class="data-indent">
							<input type="text" id="warnings_jumpA" name="warnings_jumpA" size="5">
							to <input type="text" id="warnings_jumpB" name="warnings_jumpB" size="5">
						</div>
						
						<div class="data-indent">
							<input type="checkbox" id="warnings_jal" name="warnings_jal">
							<label for="warnings_jal">Return</label>
						</div>
						
					</div>
					
				</div>
				
			</div>
			
		</div>
		
		<div id="rootDiv" name="rootDiv"></div>
		
	</form>
	
	</center>
	
	<script language="javascript">
		for (var i = 0; i < scriptItem.length; i++) {
			for (var j = 0; j < scriptItem[i].length; j++) {
				if (scriptItem[i][j][1] == "file") {
					document.getElementById(scriptItem[i][j][0]).onblur = new Function("set_file(this);");
					document.getElementById(scriptItem[i][j][0]).onkeyup = new Function("set_file(this);");
					document.getElementById(scriptItem[i][j][0]).onchange = new Function("set_file(this);");
				} else if (scriptItem[i][j][1] == "number") {
					document.getElementById(scriptItem[i][j][0]).onchange = new Function("number(this);");
				}
			}
		}
		
		document.getElementById('instruc_clearReturnStack').onclick = new Function("restrict_command(this);");
		document.getElementById('instruc_useKeystroke').onclick = new Function("restrict_command(this);");
		document.getElementById('instruc_useMouseClick').onclick = new Function("restrict_command(this);");
		document.getElementById('instruc_sendPacket').onclick = new Function("restrict_command(this);");
		
		document.getElementById('instruc_pauseA').onchange = new Function("number(this); lower(this, 'instruc_pauseB');");
		document.getElementById('instruc_pauseB').onchange = new Function("number(this); upper('instruc_pauseA', this);");
		
		document.getElementById('instruc_TimedUseA').onchange = new Function("number(this); lower(this, 'instruc_TimedUseB');");
		document.getElementById('instruc_TimedUseB').onchange = new Function("number(this); upper('instruc_TimedUseA', this);");
		
		document.getElementById('safety_surfMin').onchange = new Function("number(this); lower(this, 'safety_surfMax');");
		document.getElementById('safety_surfMax').onchange = new Function("number(this); upper('safety_surfMin', this);");
		
		document.getElementById('warnings_jumpA').onchange = new Function("number(this); lower(this, 'warnings_jumpB');");
		document.getElementById('warnings_jumpB').onchange = new Function("number(this); upper('warnings_jumpA', this);");
		
		document.getElementById('time_maximum').onclick = new Function("refresh_pause();");
		document.getElementById('time_minimum').onclick = new Function("refresh_pause();");
		document.getElementById('time_random').onclick = new Function("refresh_pause();");
		
		document.getElementById('hide_comments').onclick = new Function("refresh_comments();");
		
		document.getElementById('time_random').checked = true;
		
		set_list();
		display_instruc(-1);
		
		displayTab('command');
	</script>
	
<?php include_once("footer.php"); ?>