<?php
	class BASE_VIEW_SOCIAL_GOOGLE_ANALYTICS extends VIEW {
		
		protected $_account_code 	= "";
		protected $_track_pageview 	= "";
				
		public function __construct() {
			$this->set_route("social","google_analytics");
			$this->disable_authorization();
			$this->type_component();
		}

		public function init() {
			
			$this->set_var("account_code",$this->_account_code);
			$this->set_var("track_pageview",$this->_track_pageview);
		}
		
		public function set_account_code($account_code) 		{ $this->_account_code = $account_code; }
		public function set_track_pageview($track_pageview) 		{ $this->_track_pageview = $track_pageview; }
		
	}