<?=HTML_UTIL::get_link($add_url,"Add Template",array("class"=>"fr btn btn-sm btn-default"))?>

<div class="cb pt20"></div>

<?
	$table_data = array();

	foreach($message_templates as $message_template) {
		
		$name = HTML_UTIL::get_link(sprintf($edit_url,$message_template->get_message_template_id()),$message_template->get_name());
		
		$actions = array();
		$actions[] = HTML_UTIL::get_link("javascript:;",$delete_link_html,array("data-mtid"=>$message_template->get_message_template_id(),"class"=>"message-template-remove"));

		$actions = HTML_UTIL::get_div(implode(" ",$actions),array("class"=>"tar"));
		$table_data[] = array($name,$actions);
	}

	HTML_TABLE_UTIL::create()
		->set_data($table_data)
		->set_headings(array("Name",""))
		->render();

	$this->show_view("paging");
?>

<script>
	
	$(function() {
		$(".message-template-remove").click(function() {

			if(!confirm('Are you sure you would like to delete this template?'))
				return false;

			mtid = $(this).data("mtid");

			$.post("<?=$current?>action:remove",{ mtid: mtid },function(response) {

				if(response.has_success) {
					FF.msg.success("Successfully removed the message template");
					$(".message-template-remove[data-mtid='" + mtid + "']").parents("tr").remove();
				} else
					FF.msg.error(response.errors);
			});
		});
		
	});

</script>