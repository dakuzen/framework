<?
	class BASE_VIEW_MANAGE_LOG extends VIEW {
	
		protected $_ff_log_cmodel 	= null;
		protected $_next_id 		= null;
		protected $_back_id 		= null;
		
		function __construct() {
			parent::__construct("manage","log")->type_popup();
		}
		
		function init() {
			
			if($log_id=$this->get("lid"))
				$this->load($log_id);
			
			$this->set_var("log", $this->_ff_log_cmodel);
			$this->set_var("next_id", $this->_next_id);
			$this->set_var("back_id", $this->_back_id);
		}	

		function load($log_id) {

			$ff_log_cmodel = HMODEL_FF_LOG::create()->get($log_id);

			if($ff_log_cmodel) {
				$this->_ff_log_cmodel = $ff_log_cmodel;

				$this->_next_id = DBQ_FF_LOG::create()
											->where("ff_log_id",">",$log_id)
											->one("ff_log_id");


				$this->_back_id = DBQ_FF_LOG::create()
											->where("ff_log_id","<",$log_id)
											->order("ff_log_id","DESC")
											->one("ff_log_id");											

			}
		}		
	}