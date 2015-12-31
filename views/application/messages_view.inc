<?php
	class BASE_VIEW_APPLICATION_MESSAGES extends VIEW {
		
		protected $_message_close 	= true;

		function __construct() {
			$this->set_route("application","messages");
			$this->disable_authorization();
		}

		function set_message_close($value) 	{ $this->_message_close = $value; } 
		function close($value) 				{ $this->_message_close = $value; } 
	 	
	 	function post_init() {

			$registry = REGISTRY::get_instance();
			
			if(!$this->has_view("warningmessages") && $registry->get_message_handler()->has_warning_messages()) {
				$view = APPLICATION::get_base_view_instance("application","warningmessages");
				$view->set_messages($registry->get_message_handler()->get_warning_messages());
				$view->set_message_close($this->_message_close);
				$this->set_view("warningmessages",$view);
			}

			if(!$this->has_view("notifymessages") && $registry->get_message_handler()->has_notify_messages()) {
				$view = APPLICATION::get_base_view_instance("application","notifymessages");
				$view->set_messages($registry->get_message_handler()->get_notify_messages());
				$view->set_message_close($this->_message_close);
				$this->set_view("notifymessages",$view);
			}
			
			if(!$this->has_view("errormessages") && ($registry->get_message_handler()->has_error_messages() || $registry->get_message_handler()->has_user_error_messages())) {
				$view = APPLICATION::get_base_view_instance("application","errormessages");
				$view->add_messages($registry->get_message_handler()->get_error_messages());
				$view->add_messages($registry->get_message_handler()->get_user_error_messages());
				$view->set_message_close($this->_message_close);
				$this->set_view("errormessages",$view);
			}
		}
	}

