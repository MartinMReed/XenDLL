<?php include_once("session.php"); ?>
<?php include_once("header.php"); ?>

	<center>
	
	<?php
	
		if (isset($_POST['email'])) {
			$email = $_POST['email'];
			if (valid_email($email) == false) {
				$error_msg .= "<li>Invalid email.</li>\n";
			}
		}
		
		if (!isset($error_msg) && isset($email)) {
			$recoverHandle = xml_recoverHandle($email);
			if ($recoverHandle == 1) {
				$confirm_msg .= "<li>Your handle has been emailed to: " . $email . ".</li>\n";
				unset($email);
			} else if ($recoverHandle == 2) {
				$error_msg .= "<li>Email not found.</li>\n";
			} else if ($recoverHandle == 0) {
				$error_msg .= "<li>Error recovering handle.</li>\n";
			}
		}
		
		include_once("message.print.php");
		
	?>
	
	<form action="handle.recover.php" method="post">
	
		<div class="root" style="width: 166px;">
		
			<div class="header">
				Recover Handle
			</div>
			
			<div class="body" style="width: 160px;">
			
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