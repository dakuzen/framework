<?php

	class BASE_DBQ_FACEBOOK_PAGE extends DBQ {
	
		const STATE_ACTIVE 	= "A";
		const STATE_DELETED 	= "D";
		
		static function get_state_list() { return array(self::STATE_ACTIVE=>"Active",self::STATE_DELETED=>"Deleted"); }
			

		public function __construct() {
			parent::__construct("facebook_pages", "facebook_page_id");
		}
	}