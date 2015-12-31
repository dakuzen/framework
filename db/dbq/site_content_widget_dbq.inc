<?php

	class BASE_DBQ_SITE_CONTENT_WIDGET extends DBQ {

		const STATE_ACTIVE = "A";
		const STATE_DELETE = "D";
		
		const SOURCE_CUSTOM 	= "C";
		const SOURCE_DEFAULT 	= "D";
		
		static function get_source_list() 	{ return array(self::SOURCE_CUSTOM=>"Custom",self::SOURCE_DEFAULT=>"Default"); }		
		
		public function __construct() {
			parent::__construct("site_content_widgets", "site_content_widget_id");
		}
	}