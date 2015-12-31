<?
	class BCMODEL_FF_LOG extends CMODEL {
	
		public function __construct() {
			return parent::__construct(BASE_DBO_FF_LOG::create());
		}
		
		public function is_notified_default()	{ return $this->get_notified()==BASE_DBQ_FF_LOG::NOTIFIED_DEFAULT; }
		public function get_notified_name()		{ return value(BASE_DBQ_FF_LOG::get_notified_list(),$this->get_notified()); }
		public function get_manage_url()		{ return "/manage/log/flid:".$this->get_ff_log_id()."/"; }
		
		public function notify() {
			return $this
						->set_notify_date(DB::get_date_time())
						->set_notified(true)
						->save();
		}

		public function save() {

			if(is_numeric(self::get_ff_log_id())) {
				if(!$this->dbo("ff_log")->update())
					throw new Exception("Error updating ".get_class());
				
			} else {
				if(!$this->get_create_date())
					$this->set_create_date(DB::get_date_time());
					
				if(!$this->dbo("ff_log")->insert())
					throw new Exception("Error inserting ".get_class());
			}

			return $this;
		}		
	}