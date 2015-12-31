<?

	class BASE_DBQ_INTERNAL_MESSAGE extends DBQ {

		public function __construct() {
			parent::__construct("internal_messages", "internal_message_id");
		}
	}