<?
	if($form_fields) {
		$table_data = array();
		foreach($form_fields as $form_field) {

			$actions = array();
			$actions[] = HTML_UTIL::get_link("javascript:;",BASE_MODEL_IMAGE_ICON::get_edit(),array("class"=>"field-update","data-ffid"=>$form_field->get_form_field_id()));
			$actions[] = HTML_UTIL::get_link("javascript:;",BASE_MODEL_IMAGE_ICON::get_delete(),array("class"=>"field-remove","data-ffid"=>$form_field->get_form_field_id()));			
			$actions[] = HTML_UTIL::get_link("javascript:;",BASE_MODEL_IMAGE_ICON::get_drag(),array("class"=>"field-drag","data-ffid"=>$form_field->get_form_field_id()));			
			
			$name = HTML_UTIL::get_link("javascript:;",$form_field->get_name(),array("class"=>"field-update","data-ffid"=>$form_field->get_form_field_id()));

			$table_data[$form_field->get_form_field_id()] = array($name,$form_field->get_interface_name(),array("data"=>implode(" ",$actions),"class"=>"wsnw w1"));
		}

		$html_table = new HTML_TABLE_UTIL();
		$html_table->set_data($table_data);
		$html_table->set_headings(array("Field","Type",""));
		$html_table->set_width("100%");
		$html_table->set_id("form-field-table");
		$html_table->set_row_id_prefix("ffid_");			
		$html_table->set_class("table table-bordered table-striped");
		$html_table->render();
	
	} else {
		echo "There are no fields configured";
	}