<?
	class BASE_VIEW_APPLICATION_BODY_BASE extends VIEW {
		
		protected $_body_id						= null;
		protected $_web_asset_manager_cmodel	= null;
		protected $_body_classes				= array();

		public function __construct() {
			$this->disable_authorization();				
			$this->_web_asset_manager_cmodel = APPLICATION::get_web_asset_manager();
		}

		public function init() {		
			$this->set_var("web_asset_manager",$this->_web_asset_manager_cmodel);
		}		
		
		public function add_body_class($class) { $this->_body_classes[] = $class; }

		public function get_web_asset_manager() { return $this->_web_asset_manager_cmodel; }
		
		public function post_init() {

			$body_view = $this->get_view("body");

			if(!$body_view)
				trigger_error("Failed to load the body view",E_USER_ERROR);

			$uri = APPLICATION::get_instance()->get_uri();

			$route = $this->get_view("body")->get_route();

			if(in_array($uri->get_controller(),SYSTEM_MANAGER::get_controller_js_names()))
				$this->get_web_asset_manager()->add_js_app($uri->get_controller().".js");

			if(in_array($route->get_controller(),SYSTEM_MANAGER::get_controller_js_names()))
				$this->get_web_asset_manager()->add_js_app($route->get_controller().".js");

			if(in_array($uri->get_controller(),SYSTEM_MANAGER::get_controller_css_names()))
				$this->get_web_asset_manager()->add_css_app($uri->get_controller().".css");

			if(in_array($route->get_controller(),SYSTEM_MANAGER::get_controller_css_names()))
				$this->get_web_asset_manager()->add_css_app($route->get_controller().".css");

			$this->set_var("web_assets",$this->get_web_asset_manager()->get_html(false));				
		
			if(!$this->_body_id) {
				
				$task = $uri->get_task();

				if($uri->is_action_task())
					$task = preg_replace("/^do/","",$task);

				$this->_body_id = "body-".$uri->get_controller()."-".$task;
			}

			$body_classes = $this->get_body_classes();
	
			$this->set_var("body_id",$this->_body_id);
			$this->set_var("body_class",implode(" ",$body_classes));
			$this->set_var("body_classes",$body_classes);			
		}

		public function get_body_classes() {

			$route 	= $this->get_view("body")->get_route();
			$uri 	= APPLICATION::get_instance()->get_uri();

			$body_classes = $this->get_view("body")->get_body_classes();
					
			$body_classes[] = "body-".$uri->get_controller();
			$body_classes[] = "body-".$uri->get_controller()."-".$uri->get_task();

			if($uri->get_controller()!=$route->get_controller()) {
				$body_classes[] = "body-".$route->get_controller();

				if($uri->get_task()!=$route->get_task())
					$body_classes[] = "body-".$route->get_controller()."-".$route->get_task();
			}
			
			$body_classes = array_merge($body_classes,$this->_body_classes);

			return $body_classes;
		}
	}