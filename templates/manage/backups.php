<?
	
	$table_data = array();	
	foreach($ff_backups as $ff_backup) {
		
		$table_data[] = array(	FORMAT_UTIL::get_long_date_time($ff_backup->get_start_time()),
								$ff_backup->get_state_name(),
								round($ff_backup->get_duration(),2),
								$ff_backup->get_filename(),
								FORMAT_UTIL::get_formatted_filesize($ff_backup->get_filesize()),
								$ff_backup->get_message());
	}

		
	$html_table = new HTML_TABLE_UTIL();
	$html_table->set_data($table_data);
	$html_table->set_headings(array("Backup Date","State","Duration (seconds)","File Name","File Size","Message"));
	$html_table->set_column_attribute(1,"align","center");
	$html_table->set_column_attribute(2,"align","center");
	$html_table->set_column_attribute(0,"nowrap","nowrap");
	$html_table->set_column_attribute(4,"nowrap","nowrap");	
	$html_table->set_width("100%");
	$html_table->render();
	
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