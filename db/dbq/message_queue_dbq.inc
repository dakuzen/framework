<?
	class BASE_DBQ_MESSAGE_QUEUE extends DBQ {

		const TYPE_EMAIL 				= "E";
		const TYPE_SMS	 				= "S";
		const TYPE_INTERNAL				= "I";
		
		const STATE_SENT 				= "S";
		const STATE_QUEUED 				= "Q";
		const STATE_CANCELED			= "C";
		const STATE_INVALID_RECIPIENTS 	= "I";
		const STATE_FAILED				= "F";
			
		static function get_type_list() 	{ return array(self::TYPE_EMAIL=>"Email",self::TYPE_SMS=>"SMS",self::TYPE_INTERNAL=>"Internal"); }		
		
		static function get_state_list() 	{ return array(	self::STATE_SENT=>"Sent",self::STATE_QUEUED=>"Queued",self::STATE_CANCELED=>"Canceled",
															self::STATE_INVALID_RECIPIENTS=>"Invalid Recipients",self::STATE_FAILED=>"Failed"); }

		static function get_format_list() 	{ return BASE_DBQ_EMAIL_MESSAGE::get_format_list(); }		


		public function __construct() {
			parent::__construct("message_queues", "message_queue_id");
		}
	}