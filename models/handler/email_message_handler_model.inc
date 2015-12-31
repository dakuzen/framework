<?
	class BASE_HMODEL_EMAIL_MESSAGE extends MODEL_HANDLER {
	
		protected $_select_fields = array("messages.*","email_messages.*");
		
		function get_dbq() {
			$email_message_dbq = new BASE_DBQ_EMAIL_MESSAGE();
			$email_message_dbq->add_inner_join("messages","email_message_id","message_id");
			$this->apply_constraints($email_message_dbq);
			return $email_message_dbq;
		}
		
		function get_email_messages() {
			$email_message_dbq = $this->get_dbq();
			$email_messages = $email_message_dbq->select($this->_select_fields);
			$email_message_cmodels = array();
			
			foreach($email_messages as $email_message) {
				$email_message_cmodel = new BASE_CMODEL_EMAIL_MESSAGE();
				$email_message_cmodel->populate($email_message,true,false);
				$this->apply_properties($email_message_cmodel,$email_message);
				$email_message_cmodels[] = $email_message_cmodel;
			}
			
			if(!$email_message_cmodels)
				return array();
			
			return $email_message_cmodels;
		}
		
		function get_email_message($email_message_id) {
			
			if(self::is_called_staticly(__CLASS__)) {
				$email_message_hmodel = new BASE_HMODEL_EMAIL_MESSAGE();
				return $email_message_hmodel->get_email_message($email_message_id);
			}
			
			$this->set_email_message_id($email_message_id);
			return get_value($this->get_email_messages(),0);
		}
		
		function get_count($field=null) 			{ return $this->get_dbq()->count($field); }
		function exists($field=null)				{ return $this->get_dbq()->exists($field); }
		function set_email_message_ids($values)		{ return $this->add_filter("email_messages.email_message_id","IN",$values); }
		function set_email_message_id($value)		{ return $this->add_filter("email_messages.email_message_id","=",$value); }
		function set_message_template_id($value)	{ return $this->add_filter("email_messages.message_template_id","=",$value); }
		function set_message_template_ids($values)	{ return $this->add_filter("email_messages.message_template_id","IN",$values); }
		

	}
	