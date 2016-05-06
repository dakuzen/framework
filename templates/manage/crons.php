<?
	$table_data = array();	
	foreach($processes as $process) {
	
		$status = "Idle";
		$last_ran = "Never";
		
		$cron = value($crons,$process,CMODEL_FF_CRON::create()->set_state(BASE_CMODEL_FF_CRON::STATE_IDLE));

		$names = array();
		foreach(explode("_",$process) as $name)
			if($name!="process")
				$names[] = STRING_UTIL::get_propercase($name);

		$actions = [];
		
		if(!$cron->is_state_active())
			$actions[] = HTML_UTIL::link("javascript:;",BASE_MODEL_IMAGE_ICON::get_play("Queue process to run"),array("class"=>"action","data-action"=>"queue","data-process"=>$process));

		if(!$cron->is_state_idle())
			$actions[] = HTML_UTIL::link("javascript:;",BASE_MODEL_IMAGE_ICON::get_reset("Reset the cron"),array("class"=>"tip action","data-action"=>"reset","data-process"=>$process));

		$name = HTML_UTIL::link($task_url."action:process/process:".$process."/",implode(" ",$names),array("class"=>"fwb","target"=>"_blank","onclick"=>"return confirm('Are you sure you would like to run this cron?')"));


		$last_ran_ago 		= HTML_UTIL::span($cron->get_create_time()->age(),array("class"=>"fss"));
		$last_ran 			= CMODEL_FORMAT::create($cron->get_create_date())->iso8601(["seconds"=>true])." ".HTML_UTIL::div($last_ran_ago);
		$status 			= $cron->get_state_name();

		if($cron->is_state_active())
			$status .= " for ".TIME_UTIL::get_pretty_length(time()-$cron->get_create_time()->time());

		if($process_id=$cron->get_process_id())
			$status .= HTML_UTIL::div("(Process ".$process_id.")",array("class"=>"fssr"));
		
		
		$last_ran_details = "";
		if($cron->get_duration())
			$last_ran_details .= " for ".TIME_UTIL::get_pretty_length($cron->get_duration());

		$last_ran .= HTML_UTIL::div($last_ran_details,array("class"=>"fssr"));

		$message = HTML_UTIL::div($cron->get_message(),array("class"=>"cr fssr"));	


		$setting = value($settings,$process,CMODEL_CRON_SETTING::create());

		$reoccurance 	= HTML_UTIL::div($setting->get_summary(),array("class"=>"fss"));
		
		$table_data[] = array(implode(" ",$actions),$name.$reoccurance.$message,array("data"=>$status,"class"=>"tac"),$last_ran);
	}
	
	HTML_TABLE_UTIL::create()
		->set_headings(array("","Process","Status","Last Ran"))
		->set_data($table_data)
		->set_column_attributes(0,array("class"=>"tac w1 wsnw"))
		->set_column_attributes(1,array("class"=>"w50p"))
		->set_column_attribute(3,"align","center")
		->set_width("100%")
		->render();
?>

<script>

	$(function() {

		$(".tip").tooltip();

		$(".action").click(function() {
			$.ajax({
			  url: "<?=$task_url;?>",
			  data: { action: $(this).data("action"), process: $(this).data("process") },
			  dataType: "json",
			  type: "post",
			  success: function(response) {
				
				if(response.has_success)
					FF.msg.success(response.data.message);
				else
					FF.msg.error(response.errors);
			}});
		});
	});

</script>