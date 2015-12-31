<?

	class BASE_DBQ_FF_LOG extends DBQ {


		const NOTIFIED_DEFAULT	= "1";

		static function get_notified_list()	{ return array(self::NOTIFIED_DEFAULT=>"Default"); }

		public function __construct() {
			parent::__construct("ff_logs", "ff_log_id");
		}
	}