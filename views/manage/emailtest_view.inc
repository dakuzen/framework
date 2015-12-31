<?
	class BASE_VIEW_MANAGE_EMAILTEST extends VIEW {
	
		protected $_to_addresses 	= array("");
		protected $_text_body		= "";
		protected $_html_body		= "";
		protected $_from_name		= "";
		protected $_from_email		= "";
		protected $_subject 		= "";
		protected $_debug		= CONSTANT::FALSE;
		protected $_custom_connection 	= 0;
		protected $_smtp_host		= "";
		protected $_smtp_port		= "";
		protected $_smtp_user		= "";
		protected $_smtp_pass		= "";
		protected $_send_method		= EMAIL_UTIL::METHOD_SENDMAIL;
		
		function __construct() {
			$this->set_route("manage","emailtest");
			$this->set_title("Email Test");
			
			$email_cmodel 		= SYSTEM_MANAGER::get_email();
			
			$this->_text_body 	= "This is a test message sent from ".SYSTEM_MANAGER::get_system_host();
			$this->_html_body 	= "This is a test message sent from ".SYSTEM_MANAGER::get_system_host();
			$this->_subject 	= "Test Message: ".SYSTEM_MANAGER::get_system_host();
			$this->_from_email 	= $email_cmodel->get_from_email();
			$this->_from_name	= $email_cmodel->get_from_name();

		}
		
		function init() {
			$registry = REGISTRY::get_instance();
			
			if($registry->get_request_parm("cmd_submit")) {

				$this->_subject			= $registry->get_request_parm("subject");
				$this->_text_body 		= XSS_UTIL::get_decoded_xss_string($registry->get_request_parm("text_body"));
				$this->_html_body 		= XSS_UTIL::get_decoded_xss_string($registry->get_request_parm("html_body"));
				$this->_to_addresses 		= $registry->get_request_parm("to_addresses");
				$this->_debug 			= $registry->get_request_parm("debug");
				$this->_from_email 		= $registry->get_request_parm("from_email");
				$this->_from_name		= $registry->get_request_parm("from_name");
				$this->_custom_connection	= $registry->get_request_parm("custom_connection");
				$this->_send_method		= $registry->get_request_parm("send_method");

				$email_cmodel = SYSTEM_MANAGER::get_email_cmodel($this->_to_addresses,$this->_subject);
				
				if($this->_custom_connection) {
					$this->_smtp_host	= $registry->get_request_parm("smtp_host");
					$this->_smtp_port	= $registry->get_request_parm("smtp_port");
					$this->_smtp_user	= $registry->get_request_parm("smtp_user");
					$this->_smtp_pass	= $registry->get_request_parm("smtp_pass");
					
					$email_cmodel->set_smtp_host($this->_smtp_host);
					$email_cmodel->set_smtp_port($this->_smtp_port);
					$email_cmodel->set_smtp_user($this->_smtp_user);
					$email_cmodel->set_smtp_pass($this->_smtp_pass);
					
					$email_cmodel->set_method($this->_send_method);
				}
				
				if($this->_html_body) {
					$email_cmodel->enable_html();
					$email_cmodel->set_body($this->_html_body);
					$email_cmodel->set_body_alt($this->_text_body);
				} else	
					$email_cmodel->set_body($this->_text_body);
				
				if($this->_debug)
					$email_cmodel->debug();
				
				$email_cmodel->set_from_email($this->_from_email);
				$email_cmodel->set_from_name($this->_from_name);
				
				$has_success = $email_cmodel->send();
				
				if($has_success)
					APPLICATION::add_notify_message("Successfully sent to ".implode(", ",$this->_to_addresses));
				else {
					APPLICATION::add_error_message($email_cmodel->get_error_message());				
				}
				
				if($this->_debug)
					APPLICATION::add_warning_message($email_cmodel->get_debug_message());
			}
			
			$this->set_var("to_addresses",$this->_to_addresses);
			$this->set_var("from_name",$this->_from_name);
			$this->set_var("from_email",$this->_from_email);
			$this->set_var("subject",$this->_subject);
			$this->set_var("html_body",$this->_html_body);
			$this->set_var("text_body",$this->_text_body);
			$this->set_var("smtp_host",$this->_smtp_host);
			$this->set_var("smtp_port",$this->_smtp_port);
			$this->set_var("smtp_user",$this->_smtp_user);
			$this->set_var("smtp_pass",$this->_smtp_pass);
			$this->set_var("debug",$this->_debug);
			$this->set_var("send_method",$this->_send_method);
			$this->set_var("custom_connection",$this->_custom_connection);
		}
		
		function set_to_addresses($to_addresses) { $this->_to_addresses = $to_addresses; }
	}