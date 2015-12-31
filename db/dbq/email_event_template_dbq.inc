<?php

	class BASE_DBQ_EMAIL_EVENT_TEMPLATE extends DBQ {

		const STATE_ACTIVE	= "A";
		const STATE_DELETED	= "D";

		const FORMAT_TEXT = "T";
		const FORMAT_HTML = "H";
		
		static function get_format_list() 	{ return array(self::FORMAT_TEXT=>"Text",self::FORMAT_HTML=>"HTML"); }
		static function get_state_list()	{ return array(self::STATE_ACTIVE=>"Active", self::STATE_DELETED=>"Deleted"); }

		public function __construct() {
			parent::__construct("email_event_templates", "email_event_template_id");
		}
	}