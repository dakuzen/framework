<?

	class BASE_DBQ_EMAIL_MESSAGE_QUEUE extends DBQ {
	
		const FORMAT_TEXT = BASE_DBQ_EMAIL_MESSAGE::FORMAT_TEXT;
		const FORMAT_HTML = BASE_DBQ_EMAIL_MESSAGE::FORMAT_HTML;

		static function get_format_list() 	{ return BASE_DBQ_EMAIL_MESSAGE::get_format_list(); }		

		public function __construct() {
			parent::__construct("email_message_queues", "message_queue_id");
		}
	}