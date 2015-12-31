<div id="logs-list">
<?
	$table_data = array();
	foreach($logs as $log) {

		$actions = array();
		$actions[] = HTML_UTIL::get_link("javascript:;",BASE_MODEL_IMAGE_ICON::get_view(),array("class"=>"log-update","data-lid"=>$log->get_ff_log_id()));
		
		$name = HTML_UTIL::get_link($log->get_manage_url(),$log->get_ff_log_id());

		$create_time = CMODEL_TIME::create($log->get_create_date());

		$date = $create_time->format()->short_date_time().HTML_UTIL::span(" (".$create_time->format()->age().")",array("class"=>"fssr"));

		$table_data[] = array($log->get_message(),$log->get_type(),array("data"=>$date,"class"=>"wsnw"),array("data"=>implode(" ",$actions),"class"=>"wsnw w1"));
	}

	HTML_TABLE_UTIL::create()
		->set_data($table_data)
		->set_headings(array("Message","Type","Date",""))
		->set_width("100%")
		->set_id("log-table")
		->set_class("table table-bordered table-striped")
		->render();
		
	$this->show_view("paging");
?>
</div>