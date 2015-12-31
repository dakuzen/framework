<?
	class BASE_HMODEL_EMAIL_EVENT_SETTING extends MODEL_HANDLER {
	
		protected $_select_fields = array("email_event_settings.*");
		
		function get_dbq() {
			$email_event_setting_dbq = new BASE_DBQ_EMAIL_EVENT_SETTING();
			$this->apply_constraints($email_event_setting_dbq);
			return $email_event_setting_dbq;
		}
		
		function get_email_event_settings() {
			
			$email_event_setting_dbq = $this->get_dbq();
			$email_event_settings = $email_event_setting_dbq->select($this->_select_fields);
			$email_event_setting_cmodels = array();
			
			foreach($email_event_settings as $email_event_setting) {
				$email_event_setting_cmodel = new BASE_CMODEL_EMAIL_EVENT_SETTING();
				$email_event_setting_cmodel->populate($email_event_setting,true,false);
				$this->apply_properties($email_event_setting_cmodel,$email_event_setting);
				$email_event_setting_cmodels[] = $email_event_setting_cmodel;
			}
			
			return $email_event_setting_cmodels;
		}
		
		function get_email_event_setting($email_event_setting_id) {
			
			if(self::is_called_staticly(__CLASS__)) {
				$email_event_setting_hmodel = new BASE_HMODEL_EMAIL_EVENT_SETTING();
				return $email_event_setting_hmodel->get_email_event_setting($email_event_setting_id);
			}
			
			$this->set_email_event_setting_id($email_event_setting_id);
			return get_value($this->get_email_event_settings(),0);
		}
		
		function get_count($field=null) 		{ return $this->get_dbq()->count($field); }
		function exists($field=null)			{ return $this->get_dbq()->exists($field); }
		function set_email_event_setting_ids($values)	{ return $this->add_filter("email_event_settings.email_event_setting_id","IN",$values); }
		function set_email_event_setting_id($value)	{ return $this->add_filter("email_event_settings.email_event_setting_id","=",$value); }
		function set_email_event_id($value)		{ return $this->add_filter("email_event_settings.email_event_id","=",$value); }
		function set_email_event_ids($values)		{ return $this->add_filter("email_event_settings.email_event_id","IN",$values); }
		

	}
	