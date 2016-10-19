<?php
	/**
    * Class BASE_VIEW
    *
    * @package Library
    * @subpackage Web
    */
	class BASE_VIEW extends BASE_TASK {

		const TYPE_POPUP						= "P";
		const TYPE_BODY							= "B";

		protected $_vars 						= array();
		protected $_views 						= array();
		protected $_body_classes				= array();
		protected $_form						= null;
		protected $_type						= VIEW::TYPE_BODY;
		protected $_title						= "";
		protected $_reroute_view				= null;
		protected $_is_autoload					= true;
		protected $_base_dir					= "";
		protected $_body_route					= null;
		protected $_has_process					= false;
		protected $_has_post_process			= false;
		protected $_has_pre_process				= false;

		static function create() { return new BASE_VIEW(); }

		function __construct($controller="",$task="",$roles=array(),$form_id="",$form_url="") {
			$this
				->set_route($controller,$task)
				->set_roles($roles);

			if($form_id || $form_url) {

				if(!$form_url)
					$form_url = "javascript:;";

				$this->set_form($form_url,false,$form_id);
			}

			return $this;
		}

		function pre_body() 					{}
		function pre_init() 					{}
		function init()							{}
		function post_init() 					{}
		function cleanup() 						{}

		function set_var($varname, $value) 		{ $this->_vars[$varname] = $value; return $this; }
		function set_vars($value)		 		{ $this->_vars = $value; return $this; }
		function set_title($value) 				{ $this->_title = $value; return $this; }
		function set_base_directory($value) 	{ $this->_base_dir = $value; return $this; }
		function set_body_classes($value) 		{ $this->_body_classes = $value; return $this; }
		function set_reroute_view($reroute_view){ $this->_reroute_view = $reroute_view; return $this; }
		function set_autoload($is_autoload) 	{ $this->_is_autoload = $is_autoload; return $this; }
		function set_view($viewname, $value) 	{ $this->_views[$viewname] = $value; return $this; }

		function get_var($varname) 				{ return get_value($this->_vars,$varname); }
		function get_vars() 					{ return $this->_vars; }
		function has_view($viewname) 			{ return isset($this->_views[$viewname]); }
		function get_form()						{ return $this->_form; }
		function get_title()					{ return $this->_title; }
		function has_views()					{ return !is_empty($this->_views); }
		function get_body_route()				{ return $this->_body_route; }
		function get_base_directory()			{ return $this->_base_dir; }
		function get_views()					{ return $this->_views; }
		function get_autoload() 				{ return $this->_is_autoload; }
		function get_body_classes() 			{ return $this->_body_classes; }
		function has_form()						{ return $this->_form!=null; }

		function type_popup()					{ $this->set_type(self::TYPE_POPUP); }
		function type_body()					{ $this->set_type(self::TYPE_BODY); }
		function type_blank()					{ $this->set_type(self::TYPE_BLANK); }
		function enable_type_popup()			{ $this->type_popup(); }
		function enable_type_body()				{ $this->type_body(); }
		function add_body_class($class)		 	{ $this->_body_classes[] = $class; }

		function type_ajax() {
			DEBUG_UTIL::format_text();
			$this->type_blank();
		}

		function disable_autoload() 			{ $this->_is_autoload = false; return $this; }
		function is_autoload()					{ return $this->_is_autoload; }

		function is_type_popup()				{ return $this->_type == self::TYPE_POPUP; }
		function is_type_body()					{ return $this->_type == self::TYPE_BODY; }
		function is_type_blank()				{ return $this->_type == self::TYPE_BLANK; }

		function get_url() {
			$route = $this->get_route();
			return "/".$route->get_controller()."/".$route->get_task()."/";
		}

		function set_body_route($controller,$task) {
			$this->_body_route = new ROUTE($controller,$task);
		}

		function set_form($action, $data_encoding=false,$id="form",$name="form",$attributes=array()) {
			if($action===null)
				$this->_form = null;
			else
				$this->_form = new HTML_FORM($action, $data_encoding, $id, $name, $attributes);

			return $this;
		}

		function reroute($controller,$task) {
			$this->_reroute_view = APPLICATION::get_view_instance($controller,$task);
		}

		function reroute_view($view) {
			$this->_reroute_view = $view;
		}

		function reset_process_flag() {
			$this->_has_process = false;
			$this->_has_pre_process = false;
			$this->_has_post_process = false;
		}

		function get_reroute_view() {

			if($this->_reroute_view)
				return $this->_reroute_view;

			foreach($this->_views as $view)
				if($view && ($reroute_view=$view->get_reroute_view()))
					return $reroute_view;
			return null;
		}

		function show($reset_process_flag=false) {
			$this->_show($reset_process_flag);
		}

		function _show($reset_process_flag=false) {

			if($reset_process_flag)
				$this->reset_process_flag();

	    	if(!$this->_has_pre_process)
	    		$this->pre_process();

	    	if(!$this->_has_process)
	    		$this->process();

	    	if(!$this->_has_post_process)
	    		$this->post_process();

			if(!$this->get_route())
				return false;

			if($this->get_form()!=null)
				$this->get_form()->render_open_tag();

			$has_success = VIEW::include_template($this->get_route()->get_controller(),$this->get_route()->get_task(),$this->_vars,$this->get_base_directory(),true);

			if(!$has_success) {

				$framework = false;
				foreach(MISC_UTIL::get_subclasses($this) as $class)
					if(preg_match("/^BASE_VIEW_/",$class))
						$framework = true;

				if($framework)
					$has_success = VIEW::include_template($this->get_route()->get_controller(),$this->get_route()->get_task(),$this->_vars,APPLICATION::get_framework_base_directory(),true);
			}

			if(!$has_success)
				trigger_error("Could not find template file for ".$this->get_base_directory()."template/".$this->get_route()->get_controller()."/".$this->get_route()->get_task().".php",E_USER_WARNING);

			if($this->get_form()!=null)
				$this->get_form()->render_close_tag();

			$this->cleanup();
		}

		function pre_process() {

			$this->_has_pre_process = true;

			$this->pre_init();

			foreach($this->_views as $key=>$view) {
				if($view)
					$view->pre_process();
				else
					APPLICATION::add_debug_message("Attempted to pre_process() a view on null object with name ".$key,false);
			}
		}

		function process() {

			$this->_has_process = true;

			$this->init();

			foreach($this->_views as $key=>$view) {

				if($view) {
					$view->process();

					if(!$view->is_authorized())
						return false;
				} else
					APPLICATION::add_debug_message("Attempted to process() a view on null object with name ".$key,false);
			}
		}

		function post_process() {

			$this->_has_post_process = true;

			$this->post_init();

			foreach($this->_views as $key=>$view) {
				if($view)
					$view->post_process();
				else
					APPLICATION::add_debug_message("Attempted to post_process() a view on null object with name ".$key,false);
			}
		}

		function show_view($name,$reset_process_flag=false) {

			$view = $this->get_view($name);

			if($view) {

				if($reset_process_flag)
					$view->reset_process_flag();

				$view->show();
			}
		}

		function get_view_html($name) {
			ob_start();
			$this->show_view($name);
			$string = ob_get_contents();
			ob_end_clean();
			return $string;
		}

		function get_view($name,$rendered=false) {
			if(!$rendered)
				return get_value($this->_views,$name);
			return $this->get_view_html($name);
		}

		function get_string() { return $this->get_html(); }

		function get_html($reset_process_flag=false) {

			if($reset_process_flag)
				$this->reset_process_flag();

		    ob_start();
			$this->_show();
			$string = ob_get_contents();

			if(ob_get_level())
				ob_end_clean();

			return $string;
		}

		function is_force_ssl() {
			if(parent::is_force_ssl())
				return true;

			foreach($this->_views as $name=>$view)
				if($view && $view->is_force_ssl())
					return true;

			return false;
		}

		function include_template($controller,$view,$vars=array(),$base_dir="",$supress_error=false) {

			$template_file = null;

			$application = APPLICATION::get_instance();

			if($application->has_extend()) {

				$extend_template_file = $application->get_extend_directory()."templates/".$controller."/".$view.".php";

				if(is_file($extend_template_file))
					$template_file = $extend_template_file;
			}

			if(!$template_file) {

				$base_dir = $base_dir ? $base_dir : $application->get_application_directory();

				$template_tmp_file = $base_dir."templates/".$controller."/".$view.".php";

				if(is_file($template_tmp_file))
					$template_file = $template_tmp_file;
			}

			if($template_file) {
				foreach($vars as $var_name=>$var_value)
					$$var_name = $var_value;

				return include($template_file);
			}

			if(!$supress_error)
				trigger_error("Could not find template file for /".$controller."/".$view,E_USER_WARNING);

			return false;
		}

		function add_css_custom_asset($url) 	{ $this->get_web_asset_manager()->add_css_custom_asset($url); return $this; }
		function add_js_custom_asset($url) 		{ $this->get_web_asset_manager()->add_js_custom_asset($url); return $this; }
		function add_js_lib_asset($url)			{ $this->get_web_asset_manager()->add_js_lib_asset($url); return $this; }
		function add_js_app_asset($url) 		{ $this->get_web_asset_manager()->add_js_app_asset($url); return $this; }

		function add_meta($name,$attribs) 		{ $this->get_web_asset_manager()->add_meta($name,$attribs); return $this; }
		function add_css_custom($url) 			{ $this->get_web_asset_manager()->add_css_url($url); return $this; }
		function add_js_custom($url) 			{ $this->get_web_asset_manager()->add_js_url($url); return $this; }
		function add_css_url($url) 				{ $this->get_web_asset_manager()->add_css_url($url); return $this; }
		function add_js_url($url) 				{ $this->get_web_asset_manager()->add_js_url($url); return $this; }
		function add_js_lib($url)				{ $this->get_web_asset_manager()->add_js_lib_asset($url); return $this; }
		function add_js_app($url) 				{ $this->get_web_asset_manager()->add_js_app_asset($url); return $this; }
		function add_css_app($url) 				{ $this->get_web_asset_manager()->add_css_app_asset($url); return $this; }
		function add_css_lib($url) 				{ $this->get_web_asset_manager()->add_css_lib_asset($url); return $this; }

		function get_web_asset_manager() {
			$registry = REGISTRY::get_instance();
			return $registry->get_parm("web_asset_manager");
		}

		function __call($function,$args) {

			$has_success = false;

			if(preg_match("/^(?:(set_)|get_)(.*)/",$function,$matches)) {

				$name = $matches[2];

				if($matches[1]=="set_") {

					if(count($args)>0) {

						$has_success = array_key_exists($name,$this->_vars);

						if($has_success)
							$this->_vars[$name] = $args[0];

					} else {
						trigger_error('Missing argument 1 for '.get_class($this).'->'.$function.'() ', E_USER_ERROR);
						return;
					}
				}
			}

			if(!$has_success)
				trigger_error("Call to undefined method ".get_class($this)."->".$function."()", E_USER_ERROR);
		}
	}
	/**
        * Class HTML_FORM
        *
        * @package Library
        * @subpackage Web
        */
	class HTML_FORM {

		private $_action 		= null;
		private $_name 			= null;
		private $_id 			= null;
		private $_data_encoding	= false;
		private $_attributes	= array();


		function __construct($action, $data_encoding=false,$id="form",$name=null,$attributes=array()) {
			$this->_attributes = $attributes;
			$this->set_action($action);
			$this->set_name($name);
			$this->set_id($id);
			$this->_data_encoding = $data_encoding;
		}


		function set_action($action)			{ $this->_action = $action; }
		function set_name($name)				{ $this->_name = $name; }
		function set_id($id)					{ $this->_id = $id; }
		function set_attribute($name,$value)	{ $this->_attributes[$name] = $value; }

		function get_open_tag() {

            $method = $this->_action=="javascript:;" ? "" : "POST";

		    return HTML_UTIL::get_open_form($this->_action,$this->_id,$this->_name,$this->_data_encoding,$method,$this->_attributes);
		}

		function get_close_tag() {
			return HTML_UTIL::get_close_form();
		}

		function render_open_tag(){
			echo $this->get_open_tag();
		}
		function render_close_tag(){
			echo $this->get_close_tag();
		}
	}