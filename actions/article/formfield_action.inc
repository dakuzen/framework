<?
	class BASE_ACTION_ARTICLE_FORMFIELD extends BASE_ACTION_ARTICLE_BASE {

		function process() {
			
			$form_field_id		= $this->post("ffid");
			$form				= $this->post("form");
			$field				= $this->post("field");
			$interface			= $this->post("interface");
			$configs			= (array)$this->post("configs");
			$options			= (array)$this->post("options");
				
			$form_field_cmodel = null;
			
			if($form_field_id)
				$form_field_cmodel = HMODEL_FF_FORM_FIELD::get($form_field_id);
				
			if(!$form_field_cmodel)
				$form_field_cmodel = CMODEL_FF_FORM_FIELD::create_object($interface)
											->set_form_id($this->post("fid"));
			
			
			$form_field_cmodel = $form_field_cmodel
									->set_options((array)$this->post("options"))
									->set_configs($configs)
									->set_interface($interface)
									->set_name($this->post("name"))
									->set_tag($this->post("tag"))
									->set_state($this->post("state"));

			$form_field_cmodel->save();

			if(!$form_field_id) {
				$this->redirect("/manage/articles/view:form-field/ffid:".$form_field_cmodel->get_form_field_id());
				APPLICATION::add_persistent_notify("Successfully added the field");
			}

			$this->success();
		
		
			if(!$this->has_success())
				$this->error("There was a problem trying to save the form field");
		}	
	}