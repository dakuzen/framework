<?php

	class BASE_ACTION_MANAGE_SETTINGS extends ACTION {
		
		protected $_setting_cmodels = array();
		
		function process() {
			
			$registry = REGISTRY::get_instance();
			$settings = $registry->get_request_parm("settings");
			
			$setting_handler = new BASE_HMODEL_SETTING();
			$this->_setting_cmodels = $setting_handler->get_settings();
			
			DB::start_transaction();
			
			foreach($this->_setting_cmodels as $setting_cmodel) {
			
				if($setting_cmodel->is_interface_static())
					continue;
				
				$value = get_value($settings,$setting_cmodel->get_name());
				
				$setting_cmodel->set_value($value);
				
				if($setting_cmodel->is_data_type_file()) {
				
					$upload = new UPLOAD_UTIL($setting_cmodel->get_name());
					$upload->disable_check_extension();
					
					if($upload->is_file_upload_exist()) {

						$temp_dir = MODEL_PATH::get_temporary_directory();
						FILE_UTIL::mkdir($temp_dir);

						$upload->set_store_path($temp_dir);
						$has_success = $upload->upload();

						if($has_success) {
							$has_success = $setting_cmodel->commit_file($upload->get_store_file());
							
							if(!$has_success)	
								APPLICATION::add_error_message("Failed to save the ".$setting_cmodel->get_label()." name");
						} else
							APPLICATION::add_error_message($upload->get_error_message());

						FILE_UTIL::rmdir($temp_dir);
					}
					
				} else 
					$setting_cmodel->save();
			}
			
			$has_success = DB::has_transaction_success();
			
			DB::complete_transaction();
			
			if($has_success)
				CMODEL_SETTING::get_instance()->set_settings($settings);
				
			return $has_success;
		}
		
		function get_settings() { return $this->_setting_cmodels; }
	}
	
