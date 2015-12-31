<?php
	class BASE_VIEW_TWITTER_TWEET extends VIEW {
			
		const COUNT_NONE 	= "none";
		const COUNT_HORIZONTAL 	= "horizontal";
		const COUNT_VERTICAL 	= "vertical";
				
		protected $_count	= self::COUNT_VERTICAL;
		protected $_data_text	= "";
		protected $_data_url	= "";
		
		function __construct() {
			$this->set_route("twitter","tweet");
			$this->enable_type_component();
		}

		function init() {
			
			$this->set_var("count",$this->_count);
			$this->set_var("data_url",$this->_data_url);
			$this->set_var("data_text",$this->_data_text);
		}
		
		function layout_none() 		{ $this->_layout = self::COUNT_NONE; }
		function layout_horizontal() 	{ $this->_layout = self::COUNT_HORIZONTAL; }
		function layout_vertical() 	{ $this->_layout = self::COUNT_VERTICAL; }
		
	}