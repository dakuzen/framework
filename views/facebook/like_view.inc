<?php
	class BASE_VIEW_FACEBOOK_LIKE extends VIEW {
			
		const LAYOUT_STANDARD 		= "standard";
		const LAYOUT_BUTTON_COUNT 	= "button_count";
		const LAYOUT_BOX_COUNT 		= "box_count";
		
		protected $_layout		= self::LAYOUT_STANDARD;
		protected $_width		= "";
		protected $_href		= "";
		
		function __construct() {
			$this->set_route("facebook","like");
			$this->enable_type_component();
		}

		function init() {
			
			if(!$this->_href)
				$this->_href = SYSTEM_MANAGER::get_system_url(false).SERVER_UTIL::get_request_url();
				
			$this->set_var("app_id",MODEL_FACEBOOK::get_app_id());
			$this->set_var("layout",$this->_layout);
			$this->set_var("href",$this->_href);
			$this->set_var("width",$this->_width);
		}
		
		function layout_box_count() 	{ $this->_layout = self::LAYOUT_BOX_COUNT; }
		function layout_button_count() 	{ $this->_layout = self::LAYOUT_BUTTON_COUNT; }
		
	}