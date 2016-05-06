<?
	class BASE_ACTION_MANAGE_MESSAGE extends ACTION {
		
		protected $_has_success 	= false;
		protected $_message_cmodel 	= null;
		
		function __construct($roles=array()) {			
			$this->set_roles($roles);			
			$this->debug_format_text();
		}

		function populate() {
			
			$message_cmodel = BASE_HMODEL_MESSAGE::create()
										->set_message_id($this->post("mid"))
										->get();
			if(!$message_cmodel) 
				throw new Exception("Failed to load message");
			
			$this->_message_cmodel = $message_cmodel;
			
			$this->_message_cmodel
					->set_state($this->post("form","state"))
					->set_name($this->post("form","name"))
					->set_description($this->post("form","description"));

			if($message_cmodel->has_email()) {
				$email_message_cmodel = $message_cmodel->get_email_message()
											->set_format($this->post("email","format"))
											->set_subject(XSS_UTIL::decode($this->post("email","subject")))
											->set_body(XSS_UTIL::decode($this->post("email","body")))		
											->set_message_template_id($this->post("email","message_template_id"))
											->set_from_email($this->post("email","from_email"))
											->set_from_name($this->post("email","from_name"))
											->set_to_recipients($this->post("email","to_recipients"))
											->set_cc_recipients($this->post("email","cc_recipients"))
											->set_bcc_recipients($this->post("email","bcc_recipients"));
			}

			if($message_cmodel->has_sms()) {
				$sms_message_cmodel = $message_cmodel->get_sms_message()
											->set_from_number($this->post("sms","from_number"))
											->set_message(XSS_UTIL::decode($this->post("sms","message")));
			}

			foreach($this->_message_cmodel->get_message_renderer()->get_variables() as $name=>$value) {
				$this->_message_cmodel->set_variable($name,$this->post("send",$name));
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
		
			$message_queue_cmodel = $this->_message_cmodel->get_email_message()->create_message_queue();
			$message_queue_cmodel->add_to_recipient($email);
			$has_success = $message_queue_cmodel->send();

			if(!$has_success)
				throw new Exception(implode("\n",$this->_message_cmodel->get_error_messages()));
		}

		function send_sms($number) {
			
			$this->populate();

			if(!$this->_message_cmodel)
				throw new Exception("Failed to load the message");				
		
			$message_queue_cmodel = $this->_message_cmodel->get_sms_message()->create_message_queue();
			$message_queue_cmodel->set_to_number($number);
			$has_success = $message_queue_cmodel->send();

			if(!$has_success)
				throw new Exception(implode("\n",$this->_message_cmodel->get_error_messages()));
		}
		function save() {

			$this->populate();

			$valid 	= $this->post("validate") ? $this->validate() : true;

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

			$smarty_cmodel = $message_cmodel->get_message_renderer()->get_smarty()
									->enable_throw_exception(true)
									->plugin_enabled(false);

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
				$message_cmodel->get_email_message()->get_from_name(true);
			} catch(Exception $e) {
				throw new Exception("Email From Name: ".$e->getMessage());
			}

			try {
				$message_cmodel->get_email_message()->get_from_email(true);
			} catch(Exception $e) {
				throw new Exception("Email From Email: ".$e->getMessage());
			}

			try {
				$message_cmodel->get_sms_message()->get_message(true);
			} catch(Exception $e) {
				throw new Exception("SMS Message: ".$e->getMessage());
			}			
			
			$smarty_cmodel->disable_throw_exception();
			
			return true;
		}		
	}