<?
	if($view=="upload_file") {
		
		$buttons[] = HTML_UTIL::get_button("upload_file","Upload");
		
		if($cancel_url)	
			$buttons[] = HTML_UTIL::get_button("cancel","Cancel");
		
		$table_data[] = array("Zip File:",HTML_UTIL::get_filefield("uploadfile"));
		$table_data[] = array("",implode(" ",$buttons));
		
		$html_table = new HTML_TABLE_UTIL();
		$html_table->set_data($table_data);
		$html_table->disable_css();
		$html_table->set_padding(3);
		
		echo HTML_UTIL::get_div("Supply a zip file with web assets and click the upload button. After 
					the file has uploaded you will be presented with a listing of the files
					which will be validated. You will also be able to select individual files for upload",array("class"=>"w450 p10"));
		
		$html_table->render();	
	
	} elseif($view=="upload_confirm") {

		$table_data = array();
		
		$html_content_filename = $this->get_html_content_filename($entries);
		
		foreach($entries as $entry) {
			
			if(!$entry->is_directory()) {
			
				$checkbox = $action = "";
				
				$valid_extension = true;
				
				if($html_content_filename!=$entry->get_name()) {

					$valid_extension = in_array($entry->get_extension(),$valid_extensions);
					
					if($valid_extension) {

						$action = "New File";

						if($entry->file_exists($root_directory))
							$action = "Overriding";	
							
						$checkbox = HTML_UTIL::get_checkbox("names[]",$entry->get_name(),in_array($entry->get_name(),$names));
				
					} else
						$action = "Invalid File Format";
				} else {
					$action 	= "HTML Content to be Imported";
					$checkbox 	= HTML_UTIL::get_checkbox("content_name",$entry->get_name(),true);
				}
				
				$table_data[] = array($checkbox,$entry->get_name(),$entry->get_formatted_size(),$action);
			}
		}
		
		$html_table = new HTML_TABLE_UTIL();
		$html_table->set_data($table_data);
		$html_table->set_headings(array("","File Name","File Size","Action"));
		$html_table->render();
		
		$table_data = array();
		
		$html_table = new HTML_TABLE_UTIL();
		$html_table->set_data(array(array(HTML_UTIL::get_button("upload_confirm","Continue"), HTML_UTIL::get_button("cancel","Cancel"))));
		$html_table->disable_css();
		$html_table->set_padding(3);
		$html_table->render();	
	
	} elseif($view=="upload_finish") {
	
		echo HTML_UTIL::get_button("upload_finish","Continue");
	}	
	
	echo HTML_UTIL::get_hidden_field("scid",$site_content->get_site_content_id());