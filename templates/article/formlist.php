<?
	if($ff_forms) {

		$table_data = array();

		foreach($ff_forms as $ff_form) {

			$actions = array();
			$actions[] = HTML_UTIL::link("javascript:;",MODEL_IMAGE_ICON::get_edit(),array("class"=>"ff-form-update","data-fid"=>$ff_form->get_form_id()));
			$actions[] = HTML_UTIL::link("javascript:;",MODEL_IMAGE_ICON::get_delete(),array("class"=>"ff-form-remove","data-fid"=>$ff_form->get_form_id()));

			$name = HTML_UTIL::link("javascript:;",$ff_form->get_name(),array("class"=>"ff-form-update","data-fid"=>$ff_form->get_form_id()));
			$table_data[] = array($name,$ff_form->get_shortcode(),array("data"=>implode(" ",$actions),"class"=>"wsnw w1"));
		}

		HTML_TABLE_UTIL::create()
			->set_data($table_data)
			->set_headings(array("Name","Short Code",""))
			->set_id("ff-form-table")
			->add_class("w100p")
			->render();

		$this->show_view("paging");
	} else
		echo "There are currently no forms";
