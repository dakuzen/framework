<?
	
	class BASE_VIEW_APPLICATION_POPUP extends BASE_VIEW_APPLICATION_BODY_BASE {
		
		public function __construct() {
			parent::__construct();	
			$this->set_view("messages",APPLICATION::get_base_view_instance("application","messages"));
			$this->add_body_class("body-popup");
		}
			
		public function post_init() {
			
			parent::post_init();
			
			$registry = REGISTRY::get_instance();
		
			if(isset($_SESSION)) {

				if($persistent_notify_messages=APPLICATION::get_persistent_notify_messages()) {
					APPLICATION::add_notify_messages($persistent_notify_messages);
					APPLICATION::set_persistent_notify_messages(array());
				}

				if($persistent_error_messages=APPLICATION::get_persistent_error_messages()) {
					APPLICATION::add_error_messages($persistent_error_messages);
					APPLICATION::set_persistent_error_messages(array());
				}
				
				if($persistent_debug_messages=APPLICATION::get_persistent_debug_messages()) {
					APPLICATION::add_debug_messages($persistent_debug_messages);
					APPLICATION::set_persistent_debug_messages(array());
				}
			}

			if($registry->get_message_handler()->has_debug_messages() && !SYSTEM_MANAGER::is_system_mode_production()) {
				
				if(!$this->has_view("debugmessages")) {
					$view = APPLICATION::get_base_view_instance("application","debugmessages");
					$view->set_messages($registry->get_message_handler()->get_debug_messages());
					$this->set_view("debugmessages",$view);
				} else 
					$this->get_view("debugmessages")->set_messages($registry->get_message_handler()->get_debug_messages());
			}
		}
	}