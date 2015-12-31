<?	
	class BASE_DBQ_FF_UPGRADE extends DBQ {

		public function __construct() {
			parent::__construct("ff_upgrades", "upgrade_id");
		}
	}