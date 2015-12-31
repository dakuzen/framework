<?
	class BASE_CMODEL_FF_CRON extends CMODEL {
	
		const STATE_ACTIVE		= "A";
		const STATE_IDLE		= "C";
		const STATE_FAILED		= "F";
		const STATE_QUEUED		= "Q";

		static function get_state_list() { return array(self::STATE_FAILED=>"Failed",
														self::STATE_QUEUED=>"Queued",
														self::STATE_IDLE=>"Idle",
														self::STATE_ACTIVE=>"Active"); }
		
		public function __construct() {
			return parent::__construct(BASE_DBO_FF_CRON::create());
		}

		public function get_create_time() 		{ return CMODEL_TIME::create($this->get_create_date()); }
		public function get_state_name() 		{ return get_value(self::get_state_list(),$this->get_state()); }		
		public function is_state_idle() 		{ return $this->get_state()==self::STATE_IDLE; }
		public function is_state_active() 		{ return $this->get_state()==self::STATE_ACTIVE; }
		public function is_state_queued() 		{ return $this->get_state()==self::STATE_QUEUED; }
		public function queue() 				{ return $this->set_state(self::STATE_QUEUED)->save(); }
		
		public function reset($state=self::STATE_IDLE) {
			return $this
					->set_message("")
					->set_state($state)
					->set_create_date(DB::get_date_time())
					->set_process_id(null)
					->set_long_running(0)
					->save();
		}

		public function save() {
			
			$this->set_cron_id(BASE_DBQ_FF_CRON::create()
									->where("process","=",$this->get_process())
									->one("cron_id"));

			if(is_numeric(self::get_cron_id())) {
				if(!$this->dbo("ff_cron")->update())
					throw new Exception("Error updating ".get_class());
				
			} else {
				if(!$this->dbo("ff_cron")->insert())
						throw new Exception("Error inserting ".get_class());
			}
		
			return $this;
		}

		public static function create_cron($process) {
			$cron_cmodel = HMODEL_FF_CRON::create(false)
								->where("process","=",$process)
								->get();

			if(!$cron_cmodel)
				$cron_cmodel = CMODEL_FF_CRON::create()
								->set_process($process);

			return $cron_cmodel;
		}
	}