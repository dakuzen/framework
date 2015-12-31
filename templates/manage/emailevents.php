
<table>
	<tr>
		<td>Search:</td>
		<td><?=HTML_UTIL::get_dropdown("state",BASE_DBQ_EMAIL_EVENT::get_state_list(),$state,array("id"=>"email-state"))?></td>
	</tr>
</table>

<?php
	
	$data = array();
	
	foreach($email_events as $email_event) {
		
		$actions = array();
		$actions[] = HTML_UTIL::get_link(sprintf($edit_url,$email_event->get_email_event_id()),$edit_link_html);
		
		$description 	= HTML_UTIL::get_div($email_event->get_description(),array("class"=>"fss fsi"));
		$subject 	= STRING_UTIL::shorten($email_event->get_subject(),80);
		
		$name 		= HTML_UTIL::get_link(sprintf($edit_url,$email_event->get_email_event_id()),$email_event->get_name());
		
		$data[] = array($name,$subject.$description,implode(" ",$actions));
	}

		
	$html_table = new HTML_TABLE_UTIL();
	$html_table->set_data($data);
	$html_table->set_headings(array("Name","Subject",""));
	$html_table->set_width("100%");
	$html_table->render();
	
?>

<script>

	$(function() {
	
		$("#email-state").change(function() { 
		
			redirect("/manage/emailevents/?" + $("form").serialize());
		
		});	
	});

</script>