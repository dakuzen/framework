<?

	class BASE_HMODEL_SETTING extends MODEL_HANDLER {

		protected $_select_fields = array("settings.*");

		function get_dbq() {

			$setting_dbq = new BASE_DBQ_SETTING();

			$this->apply_constraints($setting_dbq);

			return $setting_dbq;
		}

		function get_settings() {

			$setting_dbq = $this->get_dbq();

			$settings = $setting_dbq->select($this->_select_fields);

			$setting_cmodels = array();

			foreach($settings as $setting) {

				$setting_cmodel = new BASE_CMODEL_SETTING();
				$setting_cmodel->populate($setting,true);
				$this->apply_properties($setting_cmodel,$setting);
				$setting_cmodels[] = $setting_cmodel;
			}
			
			$setting_initialize_cmodel = new CMODEL_SETTING_INITIALIZE();
			$default_setting_cmodels = $setting_initialize_cmodel->get_settings();
		
			
			foreach($setting_cmodels as $setting_cmodel) {
			
				foreach($default_setting_cmodels as $default_setting_cmodel) {
					
					if($default_setting_cmodel->get_name()==$setting_cmodel->get_name()) {
						$setting_cmodel->set_property("group",$default_setting_cmodel->get_property("group"));
						$setting_cmodel->set_label($default_setting_cmodel->get_label());
						$setting_cmodel->set_values($default_setting_cmodel->get_values());
						$setting_cmodel->set_interface_type($default_setting_cmodel->get_interface_type());
						$setting_cmodel->set_multiple_value($default_setting_cmodel->get_multiple_value());
						$setting_cmodel->set_data_type($default_setting_cmodel->get_data_type());
						$setting_cmodel->set_property("instruction",$default_setting_cmodel->get_property("instruction"));
					}
				}
			}

			return $setting_cmodels;

		}

		function get_count() {
			return $this->get_dbq()->count();
		}

		function exists() {
			return $this->get_dbq()->exists();
		}

		function get_setting($name) { 

			$setting_handler = new BASE_HMODEL_SETTING();
			$setting_handler->set_name($name);
			return get_value($setting_handler->get_settings(),0);
		} 

		function set_name($name) 		{ $this->add_filter("settings.name","=",$name); }
		function set_setting_ids($setting_ids) 	{ $this->add_filter("settings.setting_id","IN",$setting_ids); }
		function set_setting_id($setting_id) 	{ $this->add_filter("settings.setting_id","=",$setting_id); }
		function set_setting($setting) 		{ $this->add_filter("settings.setting","=",$setting); }
		function set_value($value) 		{ $this->add_filter("settings.value","=",$value); }

	}