<?
	class BASE_CMODEL_MESSAGE_LOG extends MODEL_COMPLEX {
	
		protected $_message_log_dbo	= null;
		
		public function __construct() {
			$this->_message_log_dbo = new BASE_DBO_MESSAGE_LOG();
			$this->register_dbo($this->_message_log_dbo);
		}
		
		public function save() {
				
			$has_success = false;
			if(is_numeric(self::get_message_log_id())) {
				$has_success = $this->_message_log_dbo->update();
				
			} else {
				if(!$this->get_created_date())
					$this->set_created_date(DB::get_date_time());
					
				$has_success = $this->_message_log_dbo->insert();
			}
				
			return $has_success;
		}
	}