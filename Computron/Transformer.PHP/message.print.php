<?php

	// print confirm
	if (isset($confirm_msg)) {
		print('
<div class="valid">
	CONFIRMATION:
	<ul>' . $confirm_msg . '</ul>
</div>
<br>');
	}
	
	// print error
	if (isset($error_msg)) {
		print('
<div class="invalid">
	ERROR:
	<ul>' . $error_msg . '</ul>
</div>
<br>');
	}
	
?>