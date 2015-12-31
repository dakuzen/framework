<?
	$table_data = array();

	foreach($site_content_widgets as $site_content_widget) {

		$actions = array();
		$preview_link = HTML_UTIL::get_link("javascript:show_box('".$view."preview:1/scwid:".$site_content_widget->get_site_content_widget_id()."/','80%','60%')",$preview_url_html);
		
		$actions[] = $preview_link;
		$actions[] = HTML_UTIL::get_link(sprintf($edit_url,$site_content_widget->get_site_content_widget_id()),$edit_url_html);
		$actions[] = HTML_UTIL::get_link(sprintf($delete_url,$site_content_widget->get_site_content_widget_id()),$delete_url_html,array("onclick"=>"return confirm('Are you sure you would like to delete this content widget?');"));
		
		$content = STRING_UTIL::shorten(trim(strip_tags($site_content_widget->get_content(false))),100);

		$name = HTML_UTIL::link(sprintf($edit_url,$site_content_widget->get_site_content_widget_id()),$site_content_widget->get_name()).
				HTML_UTIL::span("(".$site_content_widget->get_tag().")",array("class"=>"fssr ml5")).
				HTML_UTIL::div($site_content_widget->get_path(),array("class"=>"fssr"));
		
		$table_data[] = array($name,$content,array("data"=>implode(" ",$actions),"class"=>"wsnw tar"));
	}

	HTML_TABLE_UTIL::create()
		->set_data($table_data)
		->set_headings(array("Name","Content",""))
		->set_width("100%")
		->render();
	
	$this->show_view("paging");