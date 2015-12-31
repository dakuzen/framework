<?
	class BASE_HMODEL_MESSAGE_TEMPLATE extends MODEL_HANDLER {
	
		protected $_select_fields = array("message_templates.*");
		
		function get_dbq() {
			$message_template_dbq = new BASE_DBQ_MESSAGE_TEMPLATE();
			$this->apply_constraints($message_template_dbq);
			return $message_template_dbq;
		}
		
		function get_message_templates() {
			$message_template_dbq = $this->get_dbq();
			$message_templates = $message_template_dbq->select($this->_select_fields);
			$message_template_cmodels = array();
			
			foreach($message_templates as $message_template) {
				$message_template_cmodel = new BASE_CMODEL_MESSAGE_TEMPLATE();
				$message_template_cmodel->populate($message_template,true,false);
				$this->apply_properties($message_template_cmodel,$message_template);
				$message_template_cmodels[] = $message_template_cmodel;
			}
			
			if(!$message_template_cmodels)
				return array();
			
			return $message_template_cmodels;
		}
		
		function get_message_template($message_template_id) {
			
			if(self::is_called_staticly(__CLASS__)) {
				$message_template_hmodel = new BASE_HMODEL_MESSAGE_TEMPLATE();
				return $message_template_hmodel->get_message_template($message_template_id);
			}
			
			$this->set_message_template_id($message_template_id);
			return get_value($this->get_message_templates(),0);
		}
		
		function get_count($field=null) 			{ return $this->get_dbq()->count($field); }
		function exists($field=null)				{ return $this->get_dbq()->exists($field); }
		function set_message_template_ids($values)	{ return $this->add_filter("message_templates.message_template_id","IN",$values); }
		function set_message_template_id($value)	{ return $this->add_filter("message_templates.message_template_id","=",$value); }
		function set_state($value)					{ return $this->add_filter("message_templates.state","=",$value); }
		function set_states($values)				{ return $this->add_filter("message_templates.state","IN",$values); }
		function set_guid($value)					{ return $this->add_filter("message_templates.guid","=",$value); }
		function set_guids($values)					{ return $this->add_filter("message_templates.guid","IN",$values); }
		function filter_active() 					{ return $this->add_filter("state","=",BASE_DBQ_MESSAGE_TEMPLATE::STATE_ACTIVE); }

	}
	