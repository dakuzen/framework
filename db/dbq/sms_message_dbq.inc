<?

	class BASE_DBQ_SMS_MESSAGE extends DBQ {

		public function __construct() {
			parent::__construct("sms_messages", "sms_message_id");
		}
	}