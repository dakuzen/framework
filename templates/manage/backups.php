<?
	
	$table_data = array();	
	foreach($ff_backups as $ff_backup) {
		
		$backup = FORMAT_UTIL::get_long_date_time($ff_backup->get_start_time()).HTML_UTIL::span(round($ff_backup->get_duration(),2)." seconds",array("class"=>"fssr ml5"));

		if($message=$ff_backup->get_message())
			$backup .= HTML_UTIL::div($message);

		$table_data[] = array(	$backup,
								$ff_backup->get_state_name(),								
								HTML_UTIL::a($this->get_task_url()."bid:".$ff_backup->get_ff_backup_id()."/action:download",$ff_backup->get_filename(),array("target"=>"_blank")),
								FORMAT_UTIL::get_formatted_filesize($ff_backup->get_filesize()));
	}

		
	HTML_TABLE_UTIL::create()
		->set_data($table_data)
		->set_headings(array("Backup","Status","File Name","File Size"))
		->set_column_attribute(1,"align","center")
		->set_column_attribute(2,"align","center")
		->set_width("100%")
		->render();
	
?>
<script>

	function backup_db() {

		$.ajax( { url : "<?=$this->get_task_url();?>", 
				type: "POST",
				dataType: 'json',
				data : { backup : "1" },
				success : function(response) {
				
					if(response.has_success) {
						if(confirm("Successfully backed up database. Would you like to refresh the log?"))
							window.location = "<?=$this->get_task_url();?>";

					} else {
						
						message = response.message ? response.message : "Unknown error";
						
						alert("Failed to backup database: " + message);
					}

				},
				error : function(response) {
					alert("Failed to backup database: " + response.responseText);
				}
		});
	}
	
</script>