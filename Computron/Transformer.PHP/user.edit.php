<?php include_once("session.php"); ?>
<?php include_once("authorize.php"); ?>
<?php include_once("header.php"); ?>

	<center>
	
	<?php
	
		if (isset($_POST['password1']) && isset($_POST['password2'])) {
			if (strlen($_POST['password1']) != 0 || strlen($_POST['password2']) != 0) {
				if ($_POST['password1'] != $_POST['password2']) {
					$error_msg .= "<li>Passwords do not match.</li>\n";
				} else {
					$password = xml_md5($xml_user->handle, $_POST['password1']);
				}
			} else {
				$password = $xml_user->password;
			}
		}
		
		if (isset($_POST['email'])) {
			$email = $_POST['email'];
			if (valid_email($email) == false) {
				$error_msg .= "<li>Invalid email.</li>\n";
			}
		}
		
		if (!isset($error_msg) && isset($email)) {
			$editUser = xml_editUser($xml_user->handle, $password, $email);
			if ($editUser == 1) {
				$xml_user = xml_getUser($xml_user->handle);
				$confirm_msg .= "<li>Edit complete.</li>\n";
			} else if ($editUser == 2) {
				$error_msg .= "<li>Email already in use.</li>\n";
			} else if ($editUser == 0) {
				$error_msg .= "<li>Error editing user.</li>\n";
			}
		}
		
		unset($password);
		
		if (isset($_POST['email']) == false) {
			$email = $xml_user->email;
		}
		
		include_once("message.print.php");
		
	?>
	
	<form action="user.edit.php" method="post" autocomplete="off">
	
		<div class="root" style="width: 166px;">
		
			<div class="header">
				Edit: <?php print($xml_user->handle); ?>
			</div>
			
			<div class="body" style="width: 160px;">
			
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
			
				<div class="inline" style="text-align: right;">
				
					<div class="data">
						<input type="submit" value="Submit" onclick="hideLayer('body', true); hideLayer('processing', false);">
					</div>
					
				</div>
			
			</div>
			
		</div>
		
	</form>
	
	</center>
	
<?php include_once("footer.php"); ?>