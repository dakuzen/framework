<?
	class BASE_ACTION_MANAGE_EMAILTEMPLATE extends ACTION {
		
		protected $_email_event_template_cmodel = null;
		protected $_send_email			= null;
		
		function process() {
			$registry			= REGISTRY::get_instance();
			$email_event_template_id	= $registry->get_numeric_post_parm("eetid");
			$form				= $registry->get_post_parm("form");
			$is_save_send			= $registry->get_post_parm("savesend");
			
			$this->_email_event_template_cmodel= new BASE_CMODEL_EMAIL_EVENT_TEMPLATE();
			$this->_email_event_template_cmodel->populate($form);
			
			$this->_email_event_template_cmodel->set_content(XSS_UTIL::decode($this->_email_event_template_cmodel->get_content()));

			$has_success 	= false;
			$view 		= null;

			if($this->is_form_valid()) {
				if($email_event_template_id) 
					$this->_email_event_template_cmodel->set_email_event_template_id($email_event_template_id);
				
				$has_success = $this->_email_event_template_cmodel->save();				
				
				if($has_success && $is_save_send) {
					
					
					if($this->_send_email) {
						
						$subject		= "Test Email Template - ".$this->_email_event_template_cmodel->get_name();
						$email_cmodel 	= SYSTEM_MANAGER::get_email($this->_send_email,$subject,"","","Email Template Test");
							
						if($email_cmodel) {

							if($this->_email_event_template_cmodel->is_format_html())
								$email_cmodel->enable_html();
							
							elseif($this->_email_event_template_cmodel->is_format_text())
								$email_cmodel->enable_text();
								
							$email_cmodel->set_body($this->_email_event_template_cmodel->get_content());
							$has_email_success = $email_cmodel->send();

							if($has_email_success)
								APPLICATION::add_notify_message("Successfully sent the HTML email template to ".$this->_send_email);
							else
								APPLICATION::add_error_messages($email_cmodel->get_error_messages());

						} else
							APPLICATION::add_error_message("Failed to load the email utility");
						
					} else 
						APPLICATION::add_error_message("The sample email fail to send because there is not email address to send to");					
				}				
			}
			
			return $has_success;
		}
		
		function is_form_valid() {
			return !APPLICATION::has_error_messages();
		}	
		
		function get_email_event_template() 	{ return $this->_email_event_template_cmodel; }
		function set_send_email($send_email) 	{ $this->_send_email= $send_email; }
	}
	

