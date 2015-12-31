<?
	class BASE_ACTION_MANAGE_MESSAGE extends ACTION {
		
		protected $_has_success 	= false;
		protected $_message_cmodel 	= null;
		protected $_registry	 	= null;
		
		function __construct($roles=array()) {			
			$this->set_roles($roles);			
			$this->_registry = REGISTRY::get_instance();
			$this->debug_format_text();
		}

		function populate() {

			$message_id = $this->_registry->get_numeric_post_parm("mid");
			
			$message_handler = new BASE_HMODEL_MESSAGE();
			$message_handler->set_message_id($message_id);
			
			$message_cmodel = get_value($message_handler->get_messages(),0);

			if($message_cmodel) {
				
				$this->_message_cmodel = $message_cmodel;
				
				$this->_message_cmodel->set_state($this->_registry->post("form","state"));
				$this->_message_cmodel->set_name($this->_registry->post("form","name"));
				$this->_message_cmodel->set_description($this->_registry->post("form","description"));			

				if($message_cmodel->has_email()) {
					$email_message_cmodel = $message_cmodel->get_email_message();
					$email_message_cmodel->set_format($this->_registry->post("email","format"));
					$email_message_cmodel->set_subject(XSS_UTIL::decode($this->_registry->post("email","subject")));
					$email_message_cmodel->set_body(XSS_UTIL::decode($this->_registry->post("email","body")));			
					$email_message_cmodel->set_message_template_id($this->_registry->get_numeric_post_parm("email","message_template_id"));
					$email_message_cmodel->set_from_email($this->_registry->post("email","from_email"));
					$email_message_cmodel->set_from_name($this->_registry->post("email","from_email"));
					$email_message_cmodel->set_to_recipients($this->_registry->post("email","to_recipients"));
					$email_message_cmodel->set_cc_recipients($this->_registry->post("email","cc_recipients"));
					$email_message_cmodel->set_bcc_recipients($this->_registry->post("email","bcc_recipients"));
				}

				if($message_cmodel->has_sms()) {
					$sms_message_cmodel = $message_cmodel->get_sms_message();
					$sms_message_cmodel->set_from_number($this->_registry->post("sms","from_number"));
					$sms_message_cmodel->set_message(XSS_UTIL::decode($this->_registry->post("sms","message")));
				}

				if($message_cmodel->has_internal()) {
					$internal_message_cmodel = $message_cmodel->get_internal_message();
					$internal_message_cmodel->set_message(XSS_UTIL::decode($this->_registry->post("internal","message")));				
				}
			}
		}
		function preview($response_cmodel) {
			
			$this->populate();

			if(!$this->_message_cmodel)
				throw new Exception("Failed to load the message");				
		
			$message_queue_cmodel = $this->_message_cmodel->get_email_message()->create_message_queue(false);
			$response_cmodel->data("body",$message_queue_cmodel->get_body());			
		}

		function send_email($email) {
			
			$this->populate();

			if(!$this->_message_cmodel)
				throw new Exception("Failed to load the message");				
		
			$message_queue_cmodel = $this->_message_cmodel->get_email_message()->create_message_queue(false);
			$message_queue_cmodel->add_to_recipient($email);
			$has_success = $message_queue_cmodel->send();

			if(!$has_success)
				throw new Exception(implode("\n",$this->_message_cmodel->get_error_messages()));
		}

		function send_sms($number) {
			
			$this->populate();

			if(!$this->_message_cmodel)
				throw new Exception("Failed to load the message");				
		
			$message_queue_cmodel = $this->_message_cmodel->get_sms_message()->create_message_queue(false);
			$message_queue_cmodel->set_to_number($number);
			$has_success = $message_queue_cmodel->send();

			if(!$has_success)
				throw new Exception(implode("\n",$this->_message_cmodel->get_error_messages()));
		}
		function save() {

			$this->populate();

			$valid 	= $this->_registry->post("validate") ? $this->validate() : true;

			if($valid) {

				DB::start_transaction();

				$this->_message_cmodel->save();
				
				if($this->_message_cmodel->has_email())
					$this->_message_cmodel->save_email_message();
				
				if($this->_message_cmodel->has_sms())
					$this->_message_cmodel->save_sms_message();

				if($this->_message_cmodel->has_internal()) 
					$this->_message_cmodel->save_internal_message();

				$this->_has_success = DB::complete_transaction();
			}
			
			return $this->_has_success;			
		}
		
		function has_success() 					{ return $this->_has_success; }
		function get_message()				{ return $this->_message_cmodel; }
		
		function validate() {

			$message_cmodel = $this->_message_cmodel;

			$smarty_cmodel = $message_cmodel->get_message_renderer()->get_smarty();
			$smarty_cmodel->enable_throw_exception(true);

			foreach($message_cmodel->get_default_variables() as $var=>$value)
				$message_cmodel->set_variable($var,$value);

			try {
				$message_cmodel->get_email_message()->get_body(true);

			} catch(Exception $e) {
				throw new Exception("Email Body: ".$e->getMessage());
			}

			try {
				$message_cmodel->get_email_message()->get_subject(true);
			} catch(Exception $e) {
				throw new Exception("Email Subject: ".$e->getMessage());
			}

			try {
				$message_cmodel->get_sms_message()->get_message(true);
			} catch(Exception $e) {
				throw new Exception("SMS Message: ".$e->getMessage());
			}	

			try {
				$message_cmodel->get_internal_message()->get_message(true);
			} catch(Exception $e) {
				throw new Exception("Internal Message: ".$e->getMessage());
			}			
			
			$smarty_cmodel->disable_throw_exception();
			
			return true;
		}		
	}