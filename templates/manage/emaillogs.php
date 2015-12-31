<?
	
	$start_cal 	= $this->get_view("calendar")->get_html("start_date",$start_date);
	$end_cal 	= $this->get_view("calendar")->get_html("end_date",$end_date);
	
	$email_event_dd = HTML_UTIL::get_dropdown("email_event_id",array("") + $email_event_list,$email_event_id);
	$state_dd	= HTML_UTIL::get_dropdown("state",array_merge(array(""=>""),BASE_DBQ_EMAIL_EVENT_QUEUE::get_state_list()),$state);
	
	$search_table_data[] = array("Email Event: ",$email_event_dd,"Status:",$state_dd,"Schedule Start Date:",$start_cal);
	$search_table_data[] = array("Keywords:",HTML_UTIL::get_input("keywords",$keywords),"","","Schedule End Date:",$end_cal);
	
	$search_table_data[] = array("",HTML_UTIL::get_button("cmd_submit","Search"));
	
	$search_table = new HTML_TABLE_UTIL();
	$search_table->disable_css();
	$search_table->set_data($search_table_data);
	
	$email_queue_table_data = array();
	
	foreach($email_event_queues as $index=>$email_event_queue) { 
		
		$to_recipients = is_array($email_event_queue->get_to_recipients()) ? array_keys($email_event_queue->get_to_recipients()) : array();
		$cc_recipients = is_array($email_event_queue->get_cc_recipients()) ? array_keys($email_event_queue->get_cc_recipients()) : array();
		$bcc_recipients = is_array($email_event_queue->get_bcc_recipients()) ? array_keys($email_event_queue->get_bcc_recipients()) : array();
		
		$recipients = HTML_UTIL::get_div(STRING_UTIL::shorten(implode(", ",$to_recipients),30));
		
		if($cc_recipients)
			$recipients .= HTML_UTIL::get_div("cc: ".STRING_UTIL::shorten(implode(", ",$cc_recipients),30),array("class"=>""));
			
		if($bcc_recipients)
			$recipients .= HTML_UTIL::get_div("bcc: ".STRING_UTIL::shorten(implode(", ",$bcc_recipients),30),array("class"=>""));
		
		$cols = array();
		$cols[] = $recipients;
		$cols[] = $email_event_queue->get_subject();
		$cols[] = FORMAT_UTIL::get_short_date_time($email_event_queue->get_create_date());
		$cols[] = $email_event_queue->get_state_name();
		$cols[] = array("data"=>HTML_UTIL::get_link("/manage/emaillog/".$email_event_queue->get_email_event_queue_id()."/",$view_link_html),"class"=>"wsnw");
		
		$email_queue_table_data[] = $cols;
	}
	
	$email_event_queues_table = new HTML_TABLE_UTIL();
	$email_event_queues_table->set_data($email_queue_table_data);
	$email_event_queues_table->set_headings(array("Recipients","Subject","Date Created","State",""));
	$email_event_queues_table->set_width("100%");
	
	$email_event_queues_table->set_column_attribute(2,"class","wsnw");
	$email_event_queues_table->set_column_attribute(3,"class","wsnw");
	$email_event_queues_table->set_empty_table_row("No Emails Found");	
	
	$search_table->render();
	$email_event_queues_table->render();
	
	echo $this->get_view("paging",true);
