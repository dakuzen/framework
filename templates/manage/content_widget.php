
<form action="javascript:;" id="content-widget-form">
	<?
		
		if($site_content_widget->is_tag_readonly()) {
			$data[] = array("Tag:",$site_content_widget->get_tag().HTML_UTIL::hidden("form[tag]",$site_content_widget->get_tag()));
			$data[] = array("Name:",HTML_UTIL::input("form[name]",$site_content_widget->get_name(),array("class"=>"w50p")));
		} else {
			$data[] = array("Name:",HTML_UTIL::input("form[name]",$site_content_widget->get_name(),array("class"=>"w50p")));
			$data[] = array("Tag:",HTML_UTIL::input("form[tag]",$site_content_widget->get_tag(),array("class"=>"w50p")));
		}
			
		HTML_TABLE_UTIL::create()
			->set_data($data)
			->disable_css()
			->set_column_attribute(0,"class","wsnw w1")
			->set_width("100%")
			->render();			
	?>

	<div class="coder"><?=XSS_UTIL::encode($site_content_widget->get_content())?></div>

	<?=HTML_UTIL::link("javascript:;","Save",array("class"=>"btn btn-primary save mt10"))?>

	<?=HTML_UTIL::hidden("scwid",$site_content_widget->get_site_content_widget_id())?>
	<?=HTML_UTIL::hidden("validate",0)?>
</form>

<script>
	
	$(function() {	

		$(".coder").coder({ mode: "html", name: "content" });
		
		$(".save")
		.go("<?=$view_url?>action:save/",
			{ 	data: "#content-widget-form", 
				message: "Successfully saved the widget" });
	});
				
</script>

