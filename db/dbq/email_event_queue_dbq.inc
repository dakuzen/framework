<?php

	class BASE_DBQ_EMAIL_EVENT_QUEUE extends DBQ {
	
		const STATE_SENT 		= "S";
		const STATE_QUEUED 		= "Q";
		const STATE_CANCELED		= "C";
		const STATE_INVALID_RECIPIENTS 	= "I";
		const STATE_FAILED		= "F";
		
		static function get_format_list() 	{ return BASE_DBQ_EMAIL_EVENT::get_format_list(); }
		
		static function get_state_list() 	{ return array(	self::STATE_SENT=>"Sent",self::STATE_QUEUED=>"Queued",self::STATE_CANCELED=>"Canceled",
									self::STATE_INVALID_RECIPIENTS=>"Invalid Recipients",self::STATE_FAILED=>"Failed"); }
	
		public function __construct() {
			parent::__construct("email_event_queues", "email_event_queue_id");
		}
	}