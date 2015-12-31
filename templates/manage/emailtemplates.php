<?
	 
	echo HTML_UTIL::get_link($add_url,"Add Email Template");

	$table_data = array();

	foreach($email_event_templates as $email_event_template) {
		
		$actions = array();
		$actions[] = HTML_UTIL::get_link(sprintf($edit_url,$email_event_template->get_email_event_template_id()),$edit_link_html);
		$actions[] = HTML_UTIL::get_link(sprintf($delete_url,$email_event_template->get_email_event_template_id()),$delete_link_html,array("onclick"=>"return confirm('Are you sure you would like to delete this email event template?');"));

		$table_data[] = array($email_event_template->get_name(),$email_event_template->get_format_name(),implode(" ",$actions));
	}

	$html_table = new HTML_TABLE_UTIL();
	$html_table->set_data($table_data);
	$html_table->set_headings(array("Name","Format",""));
	$html_table->render();

	$this->show_view("paging");
