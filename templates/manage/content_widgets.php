<?
	$data = [];
	foreach($content_widgets as $content_widget) {

		$actions = array();
		$preview_link = HTML_UTIL::get_link("javascript:show_box('".$view."preview:1/cwid:".$content_widget->get_content_widget_id()."/','80%','60%')",$preview_url_html);

		$actions[] = $preview_link;
		$actions[] = HTML_UTIL::link(sprintf($edit_url,$content_widget->get_content_widget_id()),$edit_url_html);
		$actions[] = HTML_UTIL::link("javascript:;",$delete_url_html,array("class"=>"content-widget-remove","data-cwid"=>$content_widget->get_content_widget_id()));

		$content = STRING_UTIL::shorten(trim(strip_tags($content_widget->get_content(false))),100);

		$name = HTML_UTIL::link(sprintf($edit_url,$content_widget->get_content_widget_id()),$content_widget->get_name()).
				HTML_UTIL::div($content_widget->get_path(),array("class"=>"fssr"));

		$data[] = array($name,$content,$content_widget->get_tag(),array("data"=>implode(" ",$actions),"class"=>"wsnw tar"));
	}

	HTML_TABLE_UTIL::create()
		->set_data($data)
		->set_headings(array("Name","Content","Tag",""))
		->set_width("100%")
		->render();
?>
<script>

	$(function() {

		$(".content-widget-remove")
		.go("<?=$view_url?>action:remove",
				{ 	data: { cwid: null },
					begin: function() {
						if(confirm('Are you sure you would like to delete this content widget?')) {
							this.data.cwid = $(this.el).data("cwid");
							return true;
						}
						return false;
					},
					success: function() {
						location.reload();
					}
				});
	});

</script>

