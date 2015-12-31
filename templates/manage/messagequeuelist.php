<?
	$data = array();
	foreach($message_queues as $message_queue) { 

		$edit_url = "/manage/messagequeue/mqid:".$message_queue->get_message_queue_id()."/";

		if($message_queue->is_type_email()) {

			$email_message_queue = $message_queue->get_email_message_queue();

			if($email_message_queue) {

				$to_recipients 	= $email_message_queue->get_to_recipients();
				$cc_recipients 	= $email_message_queue->get_cc_recipients();
				$bcc_recipients = $email_message_queue->get_bcc_recipients();

				$recipients = HTML_UTIL::get_div(STRING_UTIL::shorten(@implode(", ",$to_recipients),30));
				
				if($cc_recipients)
					$recipients .= HTML_UTIL::get_div("cc: ".STRING_UTIL::shorten(@implode(", ",$cc_recipients),30),array("class"=>""));
					
				if($bcc_recipients)
					$recipients .= HTML_UTIL::get_div("bcc: ".STRING_UTIL::shorten(@implode(", ",$bcc_recipients),30),array("class"=>""));
				
				$cols = array();
				$cols[] = HTML_UTIL::get_link($edit_url,$recipients);
				$cols[] = HTML_UTIL::get_link($edit_url,$email_message_queue->get_subject());
				$cols[] = TIME_UTIL::get_time_age($message_queue->get_created_date());
				$cols[] = $message_queue->get_state_name();
				
				$data[] = $cols;
			}

		} elseif($message_queue->is_type_sms()) {

			$sms_message_queue = $message_queue->get_sms_message_queue();

			if($sms_message_queue) {

				$cols = array();
				$cols[] = HTML_UTIL::get_link($edit_url,$sms_message_queue->get_to_number());
				$cols[] = HTML_UTIL::get_link($edit_url,STRING_UTIL::shorten($sms_message_queue->get_body(),120));
				$cols[] = TIME_UTIL::get_time_age($message_queue->get_created_date());
				$cols[] = $message_queue->get_state_name();
					
				$data[] = $cols;
			}
		}		
	}
	
	$message_queues_table = new HTML_TABLE_UTIL();
	$message_queues_table->set_data($data);
	$message_queues_table->set_headings(array("Recipients","Subject","Created","State"));
	$message_queues_table->set_width("100%");
	
	$message_queues_table->set_column_attribute(2,"class","wsnw");
	$message_queues_table->set_column_attribute(3,"class","wsnw");
	$message_queues_table->set_empty_table_row("No Messages Found");		
	$message_queues_table->render();
	
	echo $this->get_view("paging",true);
