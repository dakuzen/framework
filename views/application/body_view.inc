<?
	class BASE_VIEW_APPLICATION_BODY extends BASE_VIEW_APPLICATION_BODY_BASE {
			
		protected $_show_messages = true;

		public function __construct() {
			parent::__construct();			
			$this->set_route("application","body");
					
			$this->set_view("messages",APPLICATION::get_base_view_instance("application","messages"));
		}

		public function disable_messages()	{ $this->set_view("messages",null); }
		public function hide_messages() 	{ $this->_show_messages = false; }
		
		public function post_init() {

			$this->set_var("show_messages",$this->_show_messages);
			parent::post_init();
			
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

			if($header=$this->get_view("header"))
				$header->set_title($this->get_view("body")->get_title());
			
			$controller = APPLICATION::get_instance()->get_uri()->get_controller();
			
			if(!SYSTEM_MANAGER::is_system_mode_production() || $controller=="manage") {
				
				if($db=DB::get_instance()) {
					
					try {

						$upgrade_cmodel = new BASE_CMODEL_UPGRADE();
						
						foreach($upgrade_cmodel->get_queued_version_upgrade_items() as $version=>$functions) {
							$message = "There are ".count($functions)." upgrade items that have not been ran yet for version ".$version.HTML_UTIL::get_list($functions);
							APPLICATION::add_error($message);
						}
						
					} catch(Exception $e) {
						APPLICATION::add_error($e->getMessage());
					}
				}
			}
			
			$registry = REGISTRY::get_instance();
			if($registry->get_message_handler()->has_debug_messages() && !SYSTEM_MANAGER::is_system_mode_production()) {
				
				$debug_messages = $registry->get_message_handler()->get_debug_messages();
				
				if(!$this->has_view("debugmessages")) {
					$view = APPLICATION::get_base_view_instance("application","debugmessages");
					$view->set_messages($debug_messages);
					$this->set_view("debugmessages",$view);
				} else 
					$this->get_view("debugmessages")->set_messages($debug_messages);
			}
		}
	}

