<?php

	class BASE_VIEW_COMPONENTS_PAGING extends VIEW {

		private $_page_index 				= 0;
		private $_record_count 				= 0;
		private $_page_count 				= 0;
		private $_page_limit 				= 0;
		private $_is_page_limit 			= true;
		private $_link						= "";
		private $_page_limit_list			= array(5=>5,15=>15,25=>25,50=>50,100=>100,200=>200);
		private $_name						= "";
		private $_display_javascript		= true;
		private $_submit_paging_javascript	= '$("form").submit();';
		private $_result_label				= "Record";
		private $_result_page_label			= "Records/Page ";
		private $_paging_page_limit			= 4;
		private $_next_label				= "Next";
		private $_back_label				= "Back";
		private $_first						= true;
		private $_last						= true;
		private $_is_display_pages			= true;
		
		function __construct() {
			$this->set_route("components","paging");
			$this->disable_authorization();
			
			$this->_page_limit = get_value(array_values($this->_page_limit_list),0);
		}

		function init() {

			$list = array_values($this->_page_limit_list);

			$this->_page_limit_list = array_combine($list,$list);
			
			if($this->_page_limit<=0)
				$this->_page_limit = get_value($list,0);
				
			if(!$this->_page_count)
				$this->_page_count = $this->_page_limit>0 ? ceil($this->_record_count/$this->_page_limit) : 0;
			
			$this->set_var("paging_page_limit",$this->_paging_page_limit);
			$this->set_var("is_display_pages",$this->_is_display_pages);
			$this->set_var("page_limit_list",$this->_page_limit_list);
			$this->set_var("page_index",$this->_page_index);
			$this->set_var("page_count",$this->_page_count);
			$this->set_var("link",$this->_link);
			$this->set_var("next_label",$this->_next_label);
			$this->set_var("back_label",$this->_back_label);
			$this->set_var("page_number",$this->_page_index + 1);
			$this->set_var("page_limit",$this->_page_limit);
			$this->set_var("is_page_limit",$this->_is_page_limit);
			$this->set_var("record_count",$this->_record_count);
			$this->set_var("name",$this->_name);
			$this->set_var("first",$this->_first);
			$this->set_var("last",$this->_last);
			$this->set_var("display_javascript",$this->_display_javascript);
			$this->set_var("submit_paging_javascript",$this->_submit_paging_javascript);
			$this->set_var("result_label",$this->_result_label);
			$this->set_var("result_page_label",$this->_result_page_label);
			$this->set_var("id_prefix",$this->_name ? "-".$this->_name : "");
			$this->set_var("input_prefix",$this->_name ? "_".$this->_name : "");
		}
		
		function process_request() {
		
			$name = $this->_name ? "_".$this->_name : "";
		
			$registry = REGISTRY::get_instance();
			
			$page_index_name = "page_index".$name;
			$page_limit_name = "page_limit".$name;
			
			$this->_page_index	= is_numeric($registry->get_request_parm($page_index_name)) ? $registry->get_request_parm($page_index_name) : $this->_page_index;
			$this->_page_limit	= is_numeric($registry->get_request_parm($page_limit_name)) ? $registry->get_request_parm($page_limit_name) : $this->_page_limit;

		}

		function populate($page_index,$page_limit,$record_count,$submit_js) {
			return $this->load($page_index,$page_limit,$record_count,$submit_js);
		}
		
		function load($page_index,$page_limit,$record_count,$submit_js) {
			
			$page_count = $page_limit ? ceil($record_count/$page_limit) : 0;
			
			$this->set_page_count($page_count);
			$this->set_page_index($page_index);
			$this->set_page_limit($page_limit);
			$this->set_record_count($record_count);
			$this->set_submit_paging_javascript($submit_js);

			return $this;
		}

		function first($first) 								{ $this->_first = $first; return $this; }
		function last($last) 								{ $this->_last = $last; return $this; }
		function set_next_label($next_label) 				{ $this->_next_label = $next_label; return $this; }
		function set_back_label($back_label) 				{ $this->_back_label = $back_label; return $this; }
		function set_page_index($page_index) 				{ $this->_page_index = $page_index; return $this; }
		function set_paging_page_limit($paging_page_limit) 	{ $this->_paging_page_limit = $paging_page_limit; return $this; }
		function set_page_count($page_count) 				{ $this->_page_count = $page_count; return $this; }
		function set_page_limit($page_limit) 				{ $this->_page_limit = $page_limit; return $this; }
		function set_link($link) 							{ $this->_link = $link; return $this; }
		function set_submit_paging_javascript($value)		{ $this->_submit_paging_javascript = $value; return $this; }
		function set_name($name) 							{ $this->_name = $name; return $this; }
		function set_record_count($record_count) 			{ $this->_record_count = $record_count; return $this; }
		function set_result_label($result_label) 			{ $this->_result_label = $result_label; return $this; }
		function set_result_page_label($result_page_label)	{ $this->_result_page_label = $result_page_label; return $this; }
		function set_page_limit_list($page_limit_list)		{ $this->_page_limit_list = is_array($page_limit_list) ? $page_limit_list : array(); return $this; }
		function add_page_limit($page_limit)				{ $this->_page_limit_list = array_merge($this->_page_limit_list,(array)$page_limit); return $this; }
		function disable_page_limit() 						{ $this->_is_page_limit = false; return $this; }
		function disable_javascript() 						{ $this->_display_javascript = false; return $this; }
		function disable_pages() 							{ $this->_is_display_pages = false; return $this; }
		function disable_first() 							{ $this->_first = false; return $this; }
		function disable_last() 							{ $this->_last = false; return $this; }
		
		function has_records() 								{ return $this->get_record_count()>0; }
		function get_record_count() 						{ return $this->_record_count; }
		function get_page_limit() 							{ return $this->_page_limit; }
		function get_page_index() 							{ return $this->_page_index; }
	}