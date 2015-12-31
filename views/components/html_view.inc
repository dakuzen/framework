<?php

	class BASE_VIEW_COMPONENTS_HTML extends VIEW {
		
		protected $_html = "";
			
		function __construct() {
			$this->set_route("components","html");
			$this->disable_authorization();
		}
		
		function init() {
			$this->set_var("html",$this->_html);
		}
		
		function set_html($html) { $this->_html = $html; }		
	}