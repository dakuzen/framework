<?

	$message_dd = HTML_UTIL::get_dropdown("message_id",array(""=>"All") + $message_list,"",array("class"=>"message-queue-search-interface"));
	$state_dd	= HTML_UTIL::get_dropdown("state",array_merge(array(""=>"All"),BASE_DBQ_message_QUEUE::get_state_list()),"",array("class"=>"message-queue-search-interface"));

	$search_table_data = array();
	$search_table_data[] = array("Email Event: ",$message_dd,"Status:",$state_dd,"Schedule Start Date:",'<span data-id="search[start_date]" class="calendar_container"></span>');
	$search_table_data[] = array("Keywords:",HTML_UTIL::get_input("keywords",""),"","","Schedule End Date:",'<span data-id="search[end_date]" class="calendar_container"></span>');

	$search_table = new HTML_TABLE_UTIL();
	$search_table->disable_css();
	$search_table->set_data($search_table_data);
	$search_table->render();
?>

<div id="message-queue"></div>

<script>

	$("#message-queue").on("load",function() {
		$(this).load("/manage/messagequeues/action:search/",$(this).parents("form").serializeArray());
	}).trigger("load");

	$(function() {

		$("input[name='keywords']").autocomplete({
			minLength: 0,
			source: [],
			search: function() {
				$("#message-queue").trigger("load");
			}
		});

		$(".message-queue-search-interface").change(function() {
			$("#message-queue").trigger("load");
		});

		$(".calendar_container").calendar({ select: function() {
			$("#message-queue").trigger("load");
		}});
	});

</script>