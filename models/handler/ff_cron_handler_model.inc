<?
	class BHMODEL_FF_CRON extends HMODEL {
	
		protected $_select_fields = array("ff_crons.*");
		
		public function __construct() {
			return $this->register_dbq(BASE_DBQ_FF_CRON::create());
		}
		
		public function gets() {			
			$crons = $this->get_dbq()->select($this->_select_fields);

			$cron_cmodels = array();			
			foreach($crons as $cron) {
				$cron_cmodel = CMODEL_FF_CRON::create()->populate($cron,true,false);
				$this->apply_properties($cron_cmodel,$cron);
				$cron_cmodels[] = $cron_cmodel;
			}
			
			if(!$cron_cmodels)
				return array();
			
			return $cron_cmodels;
		}
		
		public function set_cron_ids($values)		{ return $this->where("ff_crons.cron_id","IN",$values); }
		public function set_cron_id($value)			{ return $this->where("ff_crons.cron_id","=",$value); }
		public function set_state($value)			{ return $this->where("ff_crons.state","=",$value); }
		public function set_states($values)			{ return $this->where("ff_crons.state","IN",$values); }
		public function set_guid($value)			{ return $this->where("ff_crons.guid","=",$value); }
		public function set_guids($values)			{ return $this->where("ff_crons.guid","IN",$values); }
		
		public static function create($defaults=true) {
			return $defaults ? parent::create() : parent::create();
		}		
	}
	