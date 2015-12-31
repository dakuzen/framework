<?	
	class BASE_HMODEL_EMAIL_EVENT extends MODEL_HANDLER {

		protected $_select_fields 					= array("email_events.*");
		protected $_dbq_class						= "BASE_DBQ_EMAIL_EVENT";
		protected $_cmodel_class					= "BASE_CMODEL_EMAIL_EVENT";
		protected $_handler_class					= "BASE_HMODEL_EMAIL_EVENT";
		protected $_is_load_email_event_settings	= true;
		protected $_email_event_setting_hmodel		= null;
		
		function get_dbq() {

			$email_event_dbq = new $this->_dbq_class();

			$this->apply_constraints($email_event_dbq);

			return $email_event_dbq;
		}

		function get_email_events() {

			$email_event_dbq = $this->get_dbq();

			$email_events = $email_event_dbq->select($this->_select_fields);

			$email_event_cmodels = array();			
			foreach($email_events as $email_event) {

				$cmodel_class = "CMODEL_EMAIL_EVENT_".$email_event["tag"];
				
				if(class_exists($cmodel_class)) {
				
					$email_event_cmodel = new $cmodel_class();
					$email_event_cmodel->populate($email_event,true);
					$email_event_cmodels[] = $email_event_cmodel;
				}
			}
			
			if($email_event_cmodels) {
				
				$email_event_settings_exists = DB::get_instance()->get_db_utility()->is_table_exist("email_event_settings");

				if($email_event_settings_exists && $this->_is_load_email_event_settings) {
					$email_event_ids = array_unique(MISC_UTIL::get_list_from_object_function($email_event_cmodels, "get_email_event_id"));
					$email_event_setting_hmodel = $this->_email_event_setting_hmodel ? $this->_email_event_setting_hmodel : new BASE_HMODEL_EMAIL_EVENT_SETTING();
					$email_event_setting_hmodel->add_filter("email_event_id","IN",$email_event_ids);
					$email_event_setting_cmodels = $email_event_setting_hmodel->get_email_event_settings();

					foreach($email_event_setting_cmodels as $tmp_email_event_setting_cmodel) {
					
						foreach($email_event_cmodels as $email_event_cmodel) {

							$email_event_setting_cmodel = $email_event_cmodel->get_email_event_setting($tmp_email_event_setting_cmodel->get_name());
							
							if($email_event_setting_cmodel)
								$email_event_setting_cmodel->populate($tmp_email_event_setting_cmodel->get_column_values());
						}
					}
				}
			}
			
			return $email_event_cmodels;

		}

		function get_email_event($tag) {
			
			if(self::is_called_staticly(__CLASS__)) {
				$email_event_hmodel = new BASE_HMODEL_EMAIL_EVENT();
				return $email_event_hmodel->get_email_event($tag);
			}
			
			$this->set_tag($tag);
			return get_value($this->get_email_events(),0);
		} 

		function set_email_event_ids($email_event_ids) 	{ $this->add_filter("email_events.email_event_id","IN",$email_event_ids); }
		function set_tag($tag) 							{ $this->add_filter("email_events.tag","=",$tag); }
		function set_email_event_id($email_event_id) 	{ $this->add_filter("email_events.email_event_id","=",$email_event_id); }
		function set_state($state,$operator="=") 		{ $this->add_filter("email_events.state",$operator,$state); }
	}