<?php include_once("session.php"); ?>
<?php include_once("authorize.php"); ?>
<?php include_once("header.php"); ?>

	<center>
	
	<?php
	
		if ($_POST['action'] == "open") {
			header("Location: transformer.php?script=" . $_POST['filename']);
		} else if ($_POST['action'] == "add") {
			if (isset($_POST['new_filename'])) {
				$filename = $_POST['new_filename'];
				if (valid_alphabetic($filename, 1, 10) == 0) {
					$error_msg .= "<li>Invalid filename. Filename consist of only letters, and are between 1 to 10 characters long.</li>\n";
				}
			}
			
			if (!isset($error_msg) && isset($filename)) {
				$addScript = xml_addScript($xml_user->handle, $filename);
				if ($addScript == 1) {
					header("Location: transformer.php?script=" . $filename);
				} else if ($addScript == 2) {
					$error_msg .= "<li>Filename already in use.</li>\n";
				} else if ($addScript == 0) {
					$error_msg .= "<li>Error adding new file.</li>\n";
				}
			}
		}
		
		include_once("message.print.php");
		
		if ($folder = opendir($userfolder . "/" . $xml_user->handle)) {
			while ($file = readdir($folder)) {
				if ($file != "." && $file != "..") {
					$containsFiles = true;
					break;
				}
			}
			
			if ($containsFiles) {
				rewinddir($folder);
				
				print('
	<form action="script.php" method="post">
	
		<div class="root" style="width: 166px;">
		
			<div class="header">
				Open Script
			</div>
			
			<div class="body" style="width: 160px;">
			
				<div class="inline">
				
					<div class="data">
						<select id="filename" name="filename">');
				
				while ($file = readdir($folder)) {
					if ($file != "." && $file != "..") {
						$file = explode(".", $file);
						print('<option value="' . $file[0] . '" selected>' . $file[0] . '</option>');
					}
				}
				
				print('
					</select>
					</div>
					
				</div>
				
				<div class="inline" style="text-align: right;">
				
					<div class="data">
						<input type="submit" value="Submit" onclick="hideLayer(\'body\', true); hideLayer(\'processing\', false);">
					</div>
					
				</div>
				
			</div>
			
		</div>
		
		<input type="hidden" id="action" name="action" value="open">
		
	</form>');
			}
			
			closedir($folder);
		}
		
	?>
	
	<form action="script.php" method="post">
	
		<div class="root" style="width: 166px;">
		
			<div class="header">
				Add New Script
			</div>
			
			<div class="body" style="width: 160px;">
			
				<div class="inline">
				
					<div class="inline" style="width: 50%;">
					
						<div class="data">
							<label for="new_filename">Filename:</label>
						</div>
						
					</div>
					
					<div class="inline" style="text-align: right; width: 50%;">
					
						<div class="data">
							<a href="#">?</a>
						</div>
						
					</div>
					
				</div>
				
				<div class="inline">
				
					<div class="data">
						<input type="text" id="new_filename" name="new_filename" value="<?php print($filename); ?>" maxlength="10">
					</div>
					
				</div>
				
				<div class="inline" style="text-align: right;">
				
					<div class="data">
						<input type="submit" value="Submit" onclick="hideLayer('body', true); hideLayer('processing', false);">
					</div>
					
				</div>
				
			</div>
			
		</div>
		
		<input type="hidden" id="action" name="action" value="add">
		
	</form>
	
	</center>
	
<?php include_once("footer.php"); ?>