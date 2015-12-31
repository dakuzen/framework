<?
	class BASE_HMODEL_EMAIL_EVENT_TEMPLATE extends MODEL_HANDLER {
	
		protected $_select_fields = array("email_event_templates.*");
		
		function get_dbq() {
			$email_event_template_dbq = new BASE_DBQ_EMAIL_EVENT_TEMPLATE();
			$this->apply_constraints($email_event_template_dbq);
			return $email_event_template_dbq;
		}
		
		function get_email_event_templates() {
			$email_event_template_dbq = $this->get_dbq();
			$email_event_templates = $email_event_template_dbq->select($this->_select_fields);
			$email_event_template_cmodels = array();
			
			foreach($email_event_templates as $email_event_template) {
				$email_event_template_cmodel = new BASE_CMODEL_EMAIL_EVENT_TEMPLATE();
				$email_event_template_cmodel->populate($email_event_template,true,false);
				$this->apply_properties($email_event_template_cmodel,$email_event_template);
				$email_event_template_cmodels[] = $email_event_template_cmodel;
			}
			
			return $email_event_template_cmodels;
		}
		
		function get_email_event_template($email_event_template_id) {
			
			if(self::is_called_staticly(__CLASS__)) {
				$email_event_template_hmodel = new BASE_HMODEL_EMAIL_EVENT_TEMPLATE();
				return $email_event_template_hmodel->get_email_event_template($email_event_template_id);
			}
			
			$this->set_email_event_template_id($email_event_template_id);
			return get_value($this->get_email_event_templates(),0);
		}
		
		function get_count() 				{ return $this->get_dbq()->count(); }
		function exists() 				{ return $this->get_dbq()->exists(); }
		function set_email_event_template_ids($values)	{ return $this->add_filter("email_event_templates.email_event_template_id","IN",$values); }
		function set_email_event_template_id($value)	{ return $this->add_filter("email_event_templates.email_event_template_id","=",$value); }
		function set_state($value)			{ return $this->add_filter("email_event_templates.state","=",$value); }
		function set_states($values)			{ return $this->add_filter("email_event_templates.state","IN",$values); }
		

	}
	