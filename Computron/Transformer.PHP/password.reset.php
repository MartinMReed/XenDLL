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
		
		if (isset($_POST['email'])) {
			$email = $_POST['email'];
			if (valid_email($email) == false) {
				$error_msg .= "<li>Invalid email.</li>\n";
			}
		}
		
		if (!isset($error_msg) && isset($handle) && isset($email)) {
			$resetPassword = xml_resetPassword($handle, $email);
			if ($resetPassword == 1) {
				$confirm_msg .= "<li>Your new password has been emailed to: " . $email . ".</li>\n";
				unset($handle);
				unset($email);
			} else if ($resetPassword == 2) {
				$error_msg .= "<li>Handle not found, or incorrect email.</li>\n";
			} else if ($resetPassword == 0) {
				$error_msg .= "<li>Error resetting password.</li>\n";
			}
		}
		
		include_once("message.print.php");
		
	?>
	
	<form action="password.reset.php" method="post">
	
		<div class="root" style="width: 166px;">
		
			<div class="header">
				Reset Password
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