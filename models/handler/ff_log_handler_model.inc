<?
	class BHMODEL_FF_LOG extends HMODEL {
	
		protected $_select_fields = array("ff_logs.*");
		
		public function __construct() {
			return $this->register_dbq(BASE_DBQ_FF_LOG::create());
		}
		
		public function gets() {			
			$ff_logs = $this->get_dbq()->select($this->_select_fields);

			$ff_log_cmodels = array();			
			foreach($ff_logs as $ff_log) {
				$ff_log_cmodel = CMODEL_FF_LOG::create()->populate($ff_log,true,false);
				$this->apply_properties($ff_log_cmodel,$ff_log);
				$ff_log_cmodels[] = $ff_log_cmodel;
			}
			
			if(!$ff_log_cmodels)
				return array();
			
			return $ff_log_cmodels;
		}
		
		public function set_ff_log_ids($values)	{ return $this->where("ff_logs.ff_log_id","IN",$values); }
		public function set_ff_log_id($value)	{ return $this->where("ff_logs.ff_log_id","=",$value); }
		
		public function set_notified($value)		{ return $this->where("ff_logs.notified","=",$value); }

		public static function create($defaults=true) {
			return $defaults ? parent::create() : parent::create();
		}		
	}
	