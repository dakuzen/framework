<form id="log-form">
	
	<table>
	<tbody>
		<tr>
		<td><?=HTML_UTIL::input("search[keyword]","",array("class"=>"log-search-keyword","placeholder"=>"Search"))?></td>
		<td><span class="date"></span></td>
		<td><a href="javascript:;" class="btn refresh"><i class="icon-refresh"></i></a></td>
		</tr>
	</tbody>
	</table>

	<div class="list"><?$this->show_view("logs")?></div>

</form>

<script>
	$(function() {

		$("#log-form .list").off().bind("load",function() {
			APP.spin.start();
			$(this).load("/manage/logs/action:list",$("#log-form").serializeArray(),function() { $(this).trigger("bind"); });

		}).bind("bind",function() {

			APP.spin.stop();
			$(".log-update").off().on("click",function() {
				FF.popup.show("/manage/logs/action:view/lid:" + $(this).data("lid"),"90%","90%", { onClosed: function() { $("#log-form .list").trigger("load") } });
			});	
		});

		$("#log-form .refresh").off().click(function() {
			$("#log-form .list").trigger("load");
		});

		$("#log-form .log-search-interface").off().on("change",function() { $("#log-form .list").trigger("load") });
		
		$("#log-form .date")
		.daterange({ 	start: {name: "search[start_date]" },
						end: { 	name: "search[end_date]" },
						select: function() { $("#log-form .list").trigger("load"); }}).daterange("select");

		$("#log-form .log-search-keyword").off().autocomplete({
			minLength: 0,
			source: [],
			search: function() { $("#log-form .list").trigger("load") }
		});
	});	
</script>		
	
