<?php

	class BASE_VIEW_COMPONENTS_PAGING_LIST extends VIEW {

		private $_list 			= array();
		private $_selected	 	= null;
		private $_link			= "";
		private $_name			= "";
		private $_display_page_limit	= 10;
		
		function __construct() {
			$this->set_route("components","paging_list");
			$this->disable_authorization();
		}

		function init() {
			$this->set_var("list",$this->_list);
			$this->set_var("link",$this->_link);
			$this->set_var("name",$this->_name);
			$this->set_var("selected",$this->_selected);
			$this->set_var("display_page_limit",$this->_display_page_limit);
		}
		
		function process_request() {
		
			$name = $this->_name ? "_".$this->_name : "";
		
			$registry = REGISTRY::get_instance();
			
			$page_index_name = "page_index".$name;
			$page_limit_name = "page_limit".$name;
			
			$this->_page_index	= is_numeric($registry->get_request_parm($page_index_name)) ? $registry->get_request_parm($page_index_name) : $this->_page_index;
			$this->_page_limit	= is_numeric($registry->get_request_parm($page_limit_name)) ? $registry->get_request_parm($page_limit_name) : $this->_page_limit;

		}
		
		function set_list($list) 			{ $this->_list = $list; }
		function set_selected($selected) 		{ $this->_selected = $selected; }
		function set_link($link) 			{ $this->_link = $link; }
		function set_name($name) 			{ $this->_name = $name; }
		function disable_page_limit() 			{ $this->_is_page_limit = false; }
	}