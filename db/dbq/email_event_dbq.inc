<?php
	class BASE_DBQ_EMAIL_EVENT extends DBQ {
	
		const FORMAT_TEXT = "T";
		const FORMAT_HTML = "H";
			
		const STATE_ACTIVE						= "A";
		const STATE_DELETED						= "D";
		const STATE_INACTIVE						= "I";
		
		static function get_format_list() 	{ return array(self::FORMAT_TEXT=>"Text",self::FORMAT_HTML=>"HTML"); }
		static function get_state_list() 	{ return array(self::STATE_ACTIVE=>"Active",self::STATE_INACTIVE=>"Inactive"); }

		public function __construct() {
			parent::__construct("email_events", "email_event_id");
		}
	}