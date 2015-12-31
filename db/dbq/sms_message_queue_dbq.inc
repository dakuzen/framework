<?

	class BASE_DBQ_SMS_MESSAGE_QUEUE extends DBQ {

		public function __construct() {
			parent::__construct("sms_message_queues", "sms_message_queue_id");
		}
	}