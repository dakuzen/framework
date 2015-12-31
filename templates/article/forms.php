<?=HTML_UTIL::link("javascript:;","Add Form",array("class"=>"btn btn-default btn-sm btn-primary btn-add fr ff-form-update"))?>

<h1>Forms</h1>


<div class="cb"></div>

<div id="ff-form-list"><?$this->show_view("ff_forms")?></div>

<script>

$(function() {
	
	$("#ff-form-list").bind("load",function() {
		$(this).load("/manage/articles/view:form-list/",$("#ff-form-form").serializeArray(),function() { $(this).trigger("bind") });	
	}).bind("bind",function() {

		$(".ff-form-update").off().on("click",function() {
			FF.popup.show("/manage/articles/view:form/" + ($(this).data("fid") ? "fid:" + $(this).data("fid") + "/" : ""),"80%","80%", { onClosed: function() { $("#ff-form-list").trigger("load") } });
		});
	
		
		$(".ff-form-remove").off().on("click",function() {
			if(confirm("Are you sure you would like to delete this form?"))
				$.post("/manage/articles/action:form-remove/", { fid: $(this).data("fid") },
																		function(response) { 
																			if(response.has_success)
																				$("#ff-form-list").trigger("load"); 
																			else
																				FF.msg.error(response.errors);
																		});

		});

	}).trigger("load");
	
	$(".ff-form-search-interface").off().on("change",function() { $("#ff-form-list").trigger("load") });
	
});	
</script>		
	
