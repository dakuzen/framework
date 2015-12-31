<?php

	class BASE_DBQ_PROCESS extends DBQ {

		const STATE_RUNNING		= "R";
		const STATE_DELETED		= "D";
		const STATE_QUEUED		= "Q";
		const STATE_COMPLETED	= "C";
		const STATE_CANCELED	= "L";
		const STATE_FAILED		= "F";
		
		static function get_state_list()	{ return array(	self::STATE_QUEUED=>"Queued",
															self::STATE_RUNNING=>"Running",
															self::STATE_COMPLETED=>"Completed",
															self::STATE_CANCELED=>"Canceled",
															self::STATE_DELETED=>"Deleted",
															self::STATE_FAILED=>"Failed"); }
	
		public function __construct() {
			parent::__construct("processes", "process_id");
		}
	}