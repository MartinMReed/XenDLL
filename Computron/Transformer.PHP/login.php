<?php include_once("session.php"); ?>
<?php include_once("header.php"); ?>

	<center>
	
	<?php
	
		unset($_SESSION['handle']);
		unset($_SESSION['password']);
		
		if (isset($_POST['handle'])) {
			$handle = strtolower($_POST['handle']);
			
			if (valid_alphabetic($handle, 1, 10) == 0) {
				$error_msg .= "<li>Invalid handle. Handles consist of only letters, and are between 1 to 10 characters long.</li>\n";
			}
		}
		
		if (isset($_POST['password'])) {
			if (strlen($_POST['password']) == 0) {
				$error_msg .= "<li>Invalid password.</li>\n";
			} else {
				$password = xml_md5($handle, $_POST['password']);
			}
		}
		
		if (!isset($error_msg) && isset($handle) && isset($password)) {
			if (xml_authenticate($handle, $password)) {
				xml_setLogin("web", $handle);
				header("Location: script.php");
			} else {
				$error_msg .= "<li>Unknown handle or incorrect password.</li>\n";
			}
		}
		
		unset($password);
		
		include_once("message.print.php");
		
	?>
	
	<form action="login.php" method="post">
	
		<div class="root" style="width: 166px;">
		
			<div class="header">
				User Login
			</div>
		
			<div class="body" style="width: 160px;">
			
				<div class="inline">
				
					<div class="inline" style="width: 50%;">
					
						<div class="data">
							<label for="handle">Handle:</label>
						</div>
						
					</div>
					
					<div class="inline" style="text-align: right; width: 50%;">
					
						<div class="data">
							<a href="handle.recover.php">Recover</a>
						</div>
						
					</div>
					
				</div>
				
				<div class="inline">
				
					<div class="data">
						<input type="text" id="handle" name="handle" value="<?php print($handle); ?>" maxlength="10">
					</div>
					
				</div>
				
				<div class="inline">
				
					<div class="inline" style="width: 50%;">
					
						<div class="data">
							<label for="password">Password:</label>
						</div>
						
					</div>
					
					<div class="inline" style="text-align: right; width: 50%;">
					
						<div class="data">
							<a href="password.reset.php">Reset</a>
						</div>
						
					</div>
					
				</div>
				
				<div class="inline">
				
					<div class="data">
						<input type="password" id="password" name="password" value="">
					</div>
					
				</div>
				
				<div class="inline">
				
					<div class="inline" style="width: 50%;">
					
						<div class="data">
							<a href="user.add.php">New User</a>
						</div>
						
					</div>
					
					<div class="inline" style="text-align: right; width: 50%;">
					
						<div class="data">
							<input type="submit" value="Submit" onclick="hideLayer('body', true); hideLayer('processing', false);">
						</div>
						
					</div>
					
				</div>
				
			</div>
			
		</div>
		
	</form>
	
	</center>
	
<?php include_once("footer.php"); ?>