<div id="email-template-tabs">

	<ul>
		<li><a href="#overview"><span>Overview</span></a></li>
		
		<li><a href="#template"><span>Template</span></a></li>
		
	</ul>
	
	<div id="overview">

		<?
			HTML_FORM_UTIL::create()
				->add_dropdown("form[state]","State",BASE_DBQ_message_TEMPLATE::get_state_list(),$message_template->get_state())
				->add_input("form[name]","Name",$message_template->get_name())
				->text("", HTML_UTIL::get_link("javascript:;","Save",array("class"=>"btn btn-default","id"=>"save")))
				->render();
		?>

	</div>
	
	<div id="template">
	
		<div class="fr">Variable: <?=BASE_CMODEL_MESSAGE_TEMPLATE::VAR_CONTENT?> - Placeholder for the main email textual content</div>
					

		<?=$this->get_view("code_editor")->container()?>
		
	
	</div>

</div>

<?=HTML_UTIL::get_hidden("mtid",$message_template->get_message_template_id())?>

<script>

	$(function() {

		$("#email-template-tabs").tabs({activate: function(e,ui) {

									if(ui.newPanel.selector=="#template")
										<?=$this->get_view("code_editor")->js()?>
								}});
		
		$("#save").click(function() {

			$.post("<?=$current?>action:save",$("form").serialize(),function(response) {
				if(response.has_success) {
					FF.msg.success("Successfully saved the template");
					$("#mtid").val(response.data.mtid);
				} else
					FF.msg.error(response.errors);
			});
		});
	});
	
</script>
