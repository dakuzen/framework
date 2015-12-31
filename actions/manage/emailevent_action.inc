<?
	class BASE_ACTION_MANAGE_EMAILEVENT extends ACTION {
		
		protected $_has_success 		= false;
		protected $_email_event_cmodel 	= null;
		protected $_registry	 		= null;
		
		function __construct($roles=array()) {			
			$this->set_roles($roles);
			$this->_email_event_cmodel = new BASE_CMODEL_EMAIL_EVENT();
			$this->_registry = REGISTRY::get_instance();
			$this->debug_format_text();
		}

		function populate() {

			$email_event_id 			= $this->_registry->get_numeric_post_parm("eeid");
			$email_event_template_id 	= $this->_registry->get_numeric_post_parm("form","email_event_template_id");
			$from_email 				= $this->_registry->get_post_parm("form","from_email");
			$from_name 					= $this->_registry->get_post_parm("form","from_name");
			$to_recipients 				= $this->_registry->get_post_parm("form","to_recipients");
			$cc_recipients 				= $this->_registry->get_post_parm("form","cc_recipients");
			$bcc_recipients 			= $this->_registry->get_post_parm("form","bcc_recipients");
			$sms 						= $this->_registry->get_post_parm("form","sms");
			$sms_message 				= $this->_registry->get_post_parm("form","sms_message");
			$sms_from_number 			= $this->_registry->get_post_parm("form","sms_from_number");
			
			$view = null;

			$email_event_handler = new BASE_HMODEL_EMAIL_EVENT();
			$email_event_handler->set_email_event_id($email_event_id);
			
			if($email_event_cmodel=get_value($email_event_handler->get_email_events(),0)) 
				$this->_email_event_cmodel = $email_event_cmodel;		
			
			$db_utility = DB::get_instance()->get_db_utility();
			
			$columns = $db_utility->get_table_column_list("email_events");

			$this->_email_event_cmodel->set_state($this->_registry->get_post_parm("form","state"));
			$this->_email_event_cmodel->set_format($this->_registry->get_post_parm("form","format"));
			$this->_email_event_cmodel->set_name($this->_registry->get_post_parm("form","name"));
			$this->_email_event_cmodel->set_subject(XSS_UTIL::decode($this->_registry->get_post_parm("form","subject")));
			$this->_email_event_cmodel->set_description($this->_registry->get_post_parm("form","description"));
			$this->_email_event_cmodel->set_body_html(XSS_UTIL::decode($this->_registry->get_post_parm("form","body_html")));
			$this->_email_event_cmodel->set_body_text(XSS_UTIL::decode($this->_registry->get_post_parm("form","body_text")));

			if(in_array("email_event_template_id",$columns))			
				$this->_email_event_cmodel->set_email_event_template_id($email_event_template_id);

			if(in_array("from_email",$columns))			
				$this->_email_event_cmodel->set_from_email($from_email);
			
			if(in_array("from_name",$columns))
				$this->_email_event_cmodel->set_from_name($from_name);
			
			if(in_array("to_recipients",$columns))
				$this->_email_event_cmodel->set_to_recipients($to_recipients);
			
			if(in_array("cc_recipients",$columns))
				$this->_email_event_cmodel->set_cc_recipients($cc_recipients);
			
			if(in_array("bcc_recipients",$columns))
				$this->_email_event_cmodel->set_bcc_recipients($bcc_recipients);

			if(in_array("sms",$columns))
				$this->_email_event_cmodel->set_sms($sms);

			if(in_array("sms",$columns))
				$this->_email_event_cmodel->set_sms_message($sms_message);

			if(in_array("sms",$columns))
				$this->_email_event_cmodel->set_sms_from_number($sms_from_number);
		}

		function send($email) {
			
			$this->populate();
		
			$has_success = $this->_email_event_cmodel->send($email,array(),array(),false);

			if(!$has_success)
				throw new Exception(implode("\n",$this->_email_event_cmodel->get_error_messages()));
		}

		function process() {

			$this->populate();

			$valid 	= $this->_registry->get_post_parm("validate") ? $this->validate() : true;

			if($valid) {
				
				$this->_has_success = $this->_email_event_cmodel->save();

				if($this->_has_success) {
					
					foreach($this->_email_event_cmodel->get_email_event_settings() as $email_event_setting_cmodel) {
						$email_event_setting_cmodel->set_value($this->_registry->get_post_parm("settings",$email_event_setting_cmodel->get_name()));
						$email_event_setting_cmodel->set_email_event_id($this->_email_event_cmodel->get_email_event_id());
						$email_event_setting_cmodel->save();
					}			
				}				
			}
			
			return $this->_has_success;
			
		}
		
		function has_success() 					{ return $this->_has_success; }
		function get_email_event()				{ return $this->_email_event_cmodel; }
		
		function validate() {

			$email_event_cmodel = $this->_email_event_cmodel;

			$smarty_cmodel = $email_event_cmodel->get_smarty();
			$smarty_cmodel->enable_throw_exception(true);

			foreach($email_event_cmodel->get_variables() as $var=>$value)
				$email_event_cmodel->set_variable($var,$value);

			try {
				$email_event_cmodel->get_rendered_body_text();

			} catch(Exception $e) {
				throw new Exception("Body: ".$e->getMessage());
			}

			try {
				$email_event_cmodel->get_rendered_body_html();

			} catch(Exception $e) {
				throw new Exception("Body: ".$e->getMessage());
			}

			try {
				$email_event_cmodel->get_rendered_subject();
			} catch(Exception $e) {
				throw new Exception("Subject: ".$e->getMessage());
			}
			
			$smarty_cmodel->disable_throw_exception();
						
			try {
				EMAIL_UTIL::parse_email($email_event_cmodel->get_to_recipients(),true,true);
			} catch(Exception $e) {
				throw new Exception($e->getMessage());
			}

			try {
				EMAIL_UTIL::parse_email($email_event_cmodel->get_cc_recipients(),true,true);
			} catch(Exception $e) {
				throw new Exception($e->getMessage());
			}

			try {
				EMAIL_UTIL::parse_email($email_event_cmodel->get_bcc_recipients(),true,true);
			} catch(Exception $e) {
				throw new Exception($e->getMessage());
			}
			
			return true;
		}		
	}