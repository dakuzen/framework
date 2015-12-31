<?
	class BASE_DBQ_MESSAGE_LOG extends DBQ {

		public function __construct() {
			parent::__construct("message_logs", "message_log_id");
		}
	}