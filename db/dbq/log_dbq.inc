<?php

	class BASE_DBQ_LOG extends DBQ {	
		
		const TYPE_GENERIC 	= "G";
		const TYPE_EMAIL_EVENT 	= "E";
		
		const STATE_SUCCESS	= "S";
		const STATE_FAILED	= "F";
		
		static function get_type_list() 	{ return array(self::TYPE_GENERIC=>"Generic",self::TYPE_EMAIL_EVENT=>"Email Event"); }
		
		static function get_state_list() 	{ return array(self::STATE_SUCCESS=>"Success",self::STATE_FAILED=>"Failed"); }

		public function __construct() {
			parent::__construct("logs", "log_id");
		}
	}