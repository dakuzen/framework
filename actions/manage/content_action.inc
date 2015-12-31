<?php

	class BASE_ACTION_MANAGE_CONTENT extends ACTION {
		
		protected $_has_success 	= true;
		protected $_site_content_id 	= null;
		protected $_site_content_cmodel	= null;
		protected $_mode		= 0;
		protected $_complex_class	= "BASE_CMODEL_SITE_CONTENT";
		protected $_handler_class	= "BASE_HMODEL_SITE_CONTENT";

		function __construct($complex_class="BASE_CMODEL_SITE_CONTENT",$handler_class="BASE_HMODEL_SITE_CONTENT") {
			$this->_complex_class = $complex_class;
			$this->_handler_class = $handler_class;
		}

		function process() {
			
			$registry				= REGISTRY::get_instance();
			$this->_site_content_id	= $registry->get_numeric_post_parm("scid");
			$this->_mode			= $registry->get_post_parm("mode");
			$form					= $registry->get_post_parm("form");
			$path					= $registry->get_post_parm("form","path");
			$source					= $registry->get_post_parm("form","source");
			$title					= $registry->get_post_parm("form","title");
			$type					= $registry->get_post_parm("form","type");
			$state					= $registry->get_post_parm("state");
			$redirect				= $registry->get_post_parm("form","redirect");
			$show_header_nav		= $registry->get_post_parm("show_header_nav") ? CONSTANT::TRUE : CONSTANT::FALSE;
			$show_footer_nav		= $registry->get_post_parm("show_footer_nav") ? CONSTANT::TRUE : CONSTANT::FALSE;
			$body_wrap				= $registry->get_post_parm("body_wrap");
			$head					= XSS_UTIL::get_decoded_xss_string(get_value($form,"head"));
			
			$site_content_hmodel 	= new $this->_handler_class();
			
			if($this->_site_content_id) {
				$site_content_hmodel->set_complex_class($this->_complex_class);
				$site_content_hmodel->set_site_content_id($this->_site_content_id);
				$this->_site_content_cmodel = get_value($site_content_hmodel->get_site_contents(),0);
			}
			
			$db_utility = DB::get_instance()->get_db_utility();			
			$columns = $db_utility->get_table_column_list("site_contents");
			
			$this->_site_content_cmodel = $this->_site_content_cmodel ? $this->_site_content_cmodel : new $this->_complex_class();
			$this->_site_content_cmodel->populate($form);
			$this->_site_content_cmodel->set_show_header_nav($show_header_nav);
			$this->_site_content_cmodel->set_show_footer_nav($show_footer_nav);
			$this->_site_content_cmodel->set_title($title);
			$this->_site_content_cmodel->set_type($type);
			$this->_site_content_cmodel->set_state($state);
			$this->_site_content_cmodel->set_head($head);

			if(in_array("source",$columns))			
				$this->_site_content_cmodel->set_source($source);
			
			if(in_array("redirect",$columns))
				$this->_site_content_cmodel->set_redirect($redirect);

			if(in_array("body_wrap",$columns))
				$this->_site_content_cmodel->set_body_wrap($body_wrap);
				
			if($this->_site_content_cmodel->is_type_html())
				$path = BASE_CMODEL_SITE_CONTENT::get_sanitized_path($path);
			
			$this->_site_content_cmodel->set_path($path);

			if($this->_site_content_id) 
				$this->_site_content_cmodel->set_site_content_id($this->_site_content_id);
				
			if(($this->is_update() || $this->is_update_close()) && $this->is_form_valid($this->_site_content_cmodel))
				$this->_has_success = $this->_site_content_cmodel->save(true);
			
			return $this->_has_success;
		}
		
		function is_update() {
			return REGISTRY::get_instance()->get_post_parm("cmd_update");
		}
		
		function is_update_close() {
			return REGISTRY::get_instance()->get_post_parm("cmd_update_close");
		}
		
		function is_form_valid($site_content_cmodel) {
			return !APPLICATION::has_error_messages();
		}
		
		function get_mode() 				{ return $this->_mode; }
		function get_site_content() 			{ return $this->_site_content_cmodel; }
		
		function set_complex_class($complex_class) 	{ $this->_complex_class = $complex_class; }
		function set_handler_class($handler_class) 	{ $this->_handler_class = $handler_class; }
	}
	