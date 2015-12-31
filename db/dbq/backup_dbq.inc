<?php

	class BASE_DBQ_BACKUP extends DBQ {
		
		const STATE_SUCCESS	= "S";
		const STATE_FAILED	= "F";		



		static function get_state_list() 	{ return  array(	self::STATE_SUCCESS=>"Success",
										self::STATE_FAILED=>"Failed"); }

		
		public function __construct() {
			parent::__construct("backups", "backup_id");
		}
	}