<?php include_once("session.php"); ?>
<?php include_once("header.php"); ?>

	<center>
	
	<?php
	
		if (isset($_POST['handle'])) {
			$handle = strtolower($_POST['handle']);
			if (valid_alphabetic($handle, 1, 10) == 0) {
				$error_msg .= "<li>Invalid handle. Handles consist of only letters, and are between 1 to 10 characters long.</li>\n";
			}
		}
		
		if (isset($_POST['password1']) && isset($_POST['password2'])) {
			if (strlen($_POST['password1']) == 0 && strlen($_POST['password2']) == 0) {
				$error_msg .= "<li>Invalid password.</li>\n";
			} else if ($_POST['password1'] != $_POST['password2']) {
				$error_msg .= "<li>Passwords do not match.</li>\n";
			} else {
				$password = xml_md5($handle, $_POST['password1']);
			}
		}
		
		if (isset($_POST['email'])) {
			$email = $_POST['email'];
			if (valid_email($email) == false) {
				$error_msg .= "<li>Invalid email.</li>\n";
			}
		}
		
		if (!isset($error_msg) && isset($handle) && isset($password) && isset($email)) {
			$addUser = xml_addUser($handle, $password, $email);
			if ($addUser == 1) {
				header("Location: script.php");
			} else if ($addUser == 2) {
				$error_msg .= "<li>Handle already in use.</li>\n";
			} else if ($addUser == 3) {
				$error_msg .= "<li>Email already in use.</li>\n";
			} else if ($addUser == 4) {
				$error_msg .= "<li>Registration disabled.</li>\n";
			} else if ($addUser == 0) {
				$error_msg .= "<li>Error adding new user.</li>\n";
			}
		}
		
		unset($password);
		
		include_once("message.print.php");
		
	?>
	
	<form action="user.add.php" method="post" autocomplete="off">
	
		<div class="root" style="width: 166px;">
		
			<div class="header">
				Add New User
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
							<a href="#">?</a>
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
							<label for="password1">Password:</label>
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
						<input type="password" id="password1" name="password1" value="">
					</div>
					
				</div>
				
				<div class="inline">
				
					<div class="inline" style="width: 50%;">
					
						<div class="data">
							<label for="password2">Confirm:</label>
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
						<input type="password" id="password2" name="password2" value="">
					</div>
					
				</div>
				
				<div class="inline">
				
					<div class="inline" style="width: 50%;">
					
						<div class="data">
							<label for="email">Email:</label>
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
						<input type="text" id="email" name="email" value="<?php print($email); ?>">
					</div>
					
				</div>
				
				<div class="inline">
				
					<div class="inline" style="width: 50%;">
					
						<div class="data">
							<a href="login.php">User Login</a>
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