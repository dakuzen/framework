<div id="email-template-tabs">

	<ul>
		<li><a href="#overview"><span>Overview</span></a></li>
		
		<? if($email_event_template->get_email_event_template_id()) { ?>
			<li><a href="#template"><span>Template</span></a></li>			
		<? } ?>
	</ul>
	
	<div id="overview">

		<?
			$html_form = new HTML_FORM_UTIL();
			$html_form->add_dropdown("form[state]","State",BASE_DBQ_EMAIL_EVENT_TEMPLATE::get_state_list(),$email_event_template->get_state());
			$html_form->add_dropdown("form[format]","Format",BASE_DBQ_EMAIL_EVENT_TEMPLATE::get_format_list(),$email_event_template->get_format());
			$html_form->add_input("form[name]","Name",$email_event_template->get_name());
			$html_form->add_static("", HTML_UTIL::get_button("save","Save")." ".HTML_UTIL::get_button("savesend","Save &amp; Send Sample")." ".HTML_UTIL::get_redirect_button("Cancel",$cancel_url));
			$html_form->set_width("100%");
			$html_form->render();
		?>

	</div>
	
	<div id="template">
	
		<div class="fr">Variable: <?=BASE_CMODEL_EMAIL_EVENT_TEMPLATE::VAR_EMAIL_CONTENT?> - Placeholder for the main email textual content</div>
					
		<? $this->show_view("code_editor") ?>
		
		<div class="pt5"><?=HTML_UTIL::get_button("save","Save")?></div>

	</div>

</div>

<?=HTML_UTIL::get_hidden_field("eetid",$email_event_template->get_email_event_template_id())?>

<script>

	$(function() {
	
		$("#email-template-tabs").tabs(); 
	});
	
</script>



<style>
.table-form-label {
	width: 1px;
	white-space: no-break;
}

</style>