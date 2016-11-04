<?
	/**
        * Class BASE_TASK
        *
        * @package Library
        * @subpackage Web
        */
	class BASE_TASK {

		const TYPE_BLANK				= "K";
		const TYPE_COMPONENT			= "C";

		protected $_authorizable		= true;
		protected $_route				= null;
		protected $_is_force_ssl		= false;
		protected $_is_skip_ssl			= false;
		protected $_roles				= null;
		protected $_permissions			= null;
		protected $_type				= null;
		protected $_login_redirect		= false;

		function registry()						{ return REGISTRY::get_instance(); }
		function get()							{ return $this->registry()->get(func_get_args()); }
		function post()							{ return $this->registry()->post(func_get_args()); }
		function posts()						{ return $this->registry()->posts(); }
		function request()						{ return $this->registry()->request(func_get_args()); }
		function has_post($value)				{ return $this->registry()->has_post($value); }
		function has_get($value)				{ return $this->registry()->has_get($value); }
		function get_route() 					{ return $this->_route; }
		function get_roles() 					{ return $this->_roles; }
		function get_permissions()				{ return $this->_permissions; }
		function get_type()						{ return $this->_type; }
		function has_role($role)				{ return @in_array($role,$this->_roles); }
		function has_permissions($permission)	{ return @in_array($role,$this->_permissions); }

		function is_force_ssl()					{ return $this->_is_force_ssl; }
		function is_skip_ssl()					{ return $this->_is_skip_ssl; }
		function is_type_component()			{ return $this->_type == self::TYPE_COMPONENT; }
		function is_type_blank()				{ return $this->_type == self::TYPE_BLANK; }
		function is_view() 						{ return is_a($this,"BASE_VIEW"); }
		function is_action() 					{ return is_a($this,"BASE_ACTION"); }
		function can_login_redirect()			{ return $this->_login_redirect; }

		function enable_force_ssl()				{ $this->enable_force_ssl(); }
		function force_ssl()					{ $this->_is_force_ssl = true; }
		function skip_ssl()						{ $this->_is_skip_ssl = true; }
		function type_component()				{ $this->set_type(self::TYPE_COMPONENT); }
		function type_blank()					{ $this->set_type(self::TYPE_BLANK); }
		function enable_type_component()		{ $this->type_component(); }
		function enable_type_blank()			{ $this->type_blank(); }

		function set_roles($roles) 				{ $this->_roles = (array)$roles; return $this; }
		function set_role($role) 				{ $this->set_roles(array($role)); return $this; }
		function set_permissions($value) 		{ $this->_permissions = (array)$value; return $this; }
		function set_permission($value) 		{ $this->set_permissions($value); return $this; }
		function set_type($type)				{ $this->_type = $type; return $this; }
		function debug_format_text()			{ DEBUG_UTIL::format_text(); }
		function debug_format_html()			{ DEBUG_UTIL::format_html(); }

		function login_redirect($value=true) 	{ $this->_login_redirect = $value; }

		function add_notify($error) 			{ APPLICATION::add_notify($error); }
		function add_error($error) 				{ APPLICATION::add_error($error); }
		function has_error() 					{ return APPLICATION::has_error_messages(); }

		function success($error) 				{ $this->add_notify($error); }
		function notify($error) 				{ $this->add_notify($error); }
		function error($error) 					{ $this->add_error($error); }

		function set_route($controller,$task) {
			$this->_route = new ROUTE($controller,$task);
			return $this;
		}

		function disable_route() {
			$this->set_route("","");
		}

		function get_task_url() {

			if($this instanceof ACTION)
				return "/".$this->_route->get_controller()."/do".$this->_route->get_task()."/";

			elseif($this instanceof VIEW)
				return "/".$this->_route->get_controller()."/".$this->_route->get_task()."/";
		}

		function disable_authorization() 	{ $this->_authorizable = false; return $this; }
		function is_authorizable() 			{ return $this->_authorizable && !$this->is_type_component(); }

		function is_authorized() {

			if(!$this->is_authorizable())
				return true;

			if($this->_permissions===null && $this->_roles===null) {

				$type = "task";

				if($this->is_view())
					$type = "view";

				if($this->is_action())
					$type = "action";

				throw new Exception("The ".$type." ".get_class($this)." does not have any permissions or roles specified.");
			}

			return SYSTEM_MANAGER::has_access($this->get_roles(),$this->get_permissions(),$this);
		}
	}