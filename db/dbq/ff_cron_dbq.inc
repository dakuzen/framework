<?
	class BASE_DBQ_FF_CRON extends DBQ {

		public function __construct() {
			parent::__construct("ff_crons", "cron_id");
		}
	}