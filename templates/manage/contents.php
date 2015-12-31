<?
	$table_data = array();

	foreach($site_contents as $site_content) {

		$site_content_edit_url 	= sprintf($edit_url,$site_content->get_site_content_id());
		$link 			= HTML_UTIL::get_link($site_content->get_url(),$site_content->get_url(),array("target"=>"_blank"));
		
		$actions = array();
		$actions[] = HTML_UTIL::get_link($site_content->get_url(),$preview_url_html,array("target"=>"_blank"));
		$actions[] = HTML_UTIL::get_link(sprintf($site_content_edit_url,$site_content->get_site_content_id()),$edit_url_html);
		$actions[] = HTML_UTIL::get_link(sprintf($delete_url,$site_content->get_site_content_id()),$delete_url_html,array("onclick"=>"return confirm('Are you sure you would like to delete this page?');"));
		
		$title		= $site_content->get_title() ? $site_content->get_title() : "Untitled";
		
		$modified_date 	= DATE_UTIL::get_long_date_time($site_content->get_modified_date());
		$title 		= HTML_UTIL::get_link($site_content_edit_url,$title);
		
		$table_data[] 	= array($title,$link,$site_content->get_type_name(),$modified_date,array("data"=>implode(" ",$actions),"class"=>"wsnw tac"));
	}

	$html_table = new HTML_TABLE_UTIL();
	$html_table->set_data($table_data);
	$html_table->set_headings(array("Title","Path","Type","Modified Date",""));	
	$html_table->set_width("100%");	
	$html_table->render();
	
	$paging->show_view("paging");