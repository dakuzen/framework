<?php
	class BASE_VIEW_FACEBOOK_COMMENT extends VIEW {
		
		protected $_href = "";
		protected $_css = "";
		
		function __construct() {
			$this->set_route("facebook","comment");
			$this->enable_type_component();
		}

		function init() {
		
			if(!$this->_href)
				$this->_href = SYSTEM_MANAGER::get_system_url(false).SERVER_UTIL::get_request_uri();
			
			$this->set_var("href",$this->_href);
			$this->set_var("css",$this->_css);
		}
	}