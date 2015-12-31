<?php
	$actions = array();
	
	if($edit_link)
		$actions[] = HTML_UTIL::get_link(sprintf($edit_link,$email_event_queue->get_email_event_id()),$edit_link_html);
	
	if($cancel_link)
		$actions[] = HTML_UTIL::get_link(sprintf($cancel_link,$email_event_queue->get_email_event_queue_id()),$cancel_link_html);
		
	if($resend_link)
		$actions[] = HTML_UTIL::get_link(sprintf($resend_link,$email_event_queue->get_email_event_queue_id()),$resend_link_html);
	
	$html_form = new HTML_FORM_UTIL();
	$html_form->add_static("Actions",implode(" ",$actions));
	$html_form->add_static("State",$email_event_queue->get_state_name());
	$html_form->add_static("Scheduled",FORMAT_UTIL::get_long_date_time($email_event_queue->get_schedule_date()));
	$html_form->add_static("Created",FORMAT_UTIL::get_long_date_time($email_event_queue->get_create_date()));
	$html_form->add_static("Attempts",$email_event_queue->get_attempts());
	
	if($from)
		$html_form->add_static("From",$from);
	
	$to_recipients = array();
	
	$html_form->add_static("Recipients",implode(", ",array_keys($email_event_queue->get_to_recipients())));
	
	if($email_event_queue->get_cc_recipients())
		$html_form->add_static("CC Recipients",implode(", ",array_keys($email_event_queue->get_cc_recipients())));
		
	if($email_event_queue->get_bcc_recipients())
		$html_form->add_static("BCC Recipient",implode(", ",array_keys($email_event_queue->get_bcc_recipients())));

	
	$body = $email_event_queue->get_body();

	if($email_event_queue->is_format_text())
		$body = str_replace("\n","<br>",$body);
		
	$html_form->add_static("Format",$email_event_queue->get_format_name());
	$html_form->add_static("Subject",$email_event_queue->get_subject());
	$html_form->add_static("Body",$body);
	
	
	$attach_data = array();
	foreach($email_event_queue->get_email_event_queue_attachments() as $email_event_queue_attachment) {
		$filesize = FORMAT_UTIL::get_formatted_filesize($email_event_queue_attachment->get_filesize());
		$attach_data[] = array($email_event_queue_attachment->get_filename(),$filesize,$email_event_queue_attachment->get_state_name());
	}
	
	$attach_table = new HTML_TABLE_UTIL();
	$attach_table->set_data($attach_data);
	$attach_table->set_headings(array("Filename","File Size","State"));
	$attach_table->set_empty_table_row("No Attachments");	
	$attach_table->set_width("100%");

	$email_event_log_table_data = array();
	foreach($email_event_logs as $index=>$email_event_log) { 
		
		$cols = array();
		$cols[] = FORMAT_UTIL::get_long_date_time($email_event_log->get_create_date());
		$cols[] = $email_event_log->get_message();
		
		$email_event_log_table_data[] = $cols;
	}
	
	
	$email_event_logs_table = new HTML_TABLE_UTIL();
	$email_event_logs_table->set_data($email_event_log_table_data);
	$email_event_logs_table->set_headings(array("Log Date","Message"));
	$email_event_logs_table->set_width("100%");
	$email_event_logs_table->set_empty_table_row("No Emails Logs Found");	
	
?>

<h2>Email Log</h2>

<?=$html_form->get_html()?>

<h2>Attachments</h2>

<?=$attach_table->get_html()?>

<h2>Email Log</h2>
	
<?=$email_event_logs_table->get_html()?>
	