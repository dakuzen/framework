<?php
        /**
        * Class BASE_CMODEL_LOG
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_LOG extends MODEL_COMPLEX {
                
                /**
                 * The log dbo
                 * @var object 
                 */
		private $_log_dbo	= null;

                /**
                 * Constructor
                 */
		public function __construct() {
			$this->_log_dbo = new BASE_DBO_LOG();
			$this->register_dbo($this->_log_dbo);
		}

                /**
                 * emergency
                 * @param array $data
                 * @param string $code
                 * @return boolean 
                 */
		public function emergency($data,$code="") 		{ return BASE_CMODEL_LOGGER::get_instance()->emergency($data,$code); }
		
                /**
                 * alert
                 * @param array $data
                 * @param string $code
                 * @return boolean 
                 */
                public function alert($data,$code="")			{ return BASE_CMODEL_LOGGER::get_instance()->alert($data,$code); }
		
                /**
                 * critical
                 * @param array $data
                 * @param string $code
                 * @return boolean 
                 */
                public function critical($data,$code="")		{ return BASE_CMODEL_LOGGER::get_instance()->critical($data,$code); }
		
                /**
                 * error
                 * @param array $data
                 * @param string $code
                 * @return boolean 
                 */
                public function error($data,$code="")			{ return BASE_CMODEL_LOGGER::get_instance()->error($data,$code); }
		
                /**
                 * warning
                 * @param array $data
                 * @param string $code
                 * @return boolean 
                 */
                public function warning($data,$code="")			{ return BASE_CMODEL_LOGGER::get_instance()->warning($data,$code); }		
		
                /**
                 * notice
                 * @param array $data
                 * @param string $code
                 * @return boolean 
                 */
                public function notice($data,$code="") 			{ return BASE_CMODEL_LOGGER::get_instance()->notice($data,$code); }		
		
                /**
                 * info
                 * @param array $data
                 * @param string $code
                 * @return boolean 
                 */
                public function info($data,$code="")			{ return BASE_CMODEL_LOGGER::get_instance()->info($data,$code); }
		
                /**
                 * debug
                 * @param array $data
                 * @param string $code
                 * @return boolean 
                 */
                public function debug($data,$code="")			{ return BASE_CMODEL_LOGGER::get_instance()->debug($data,$code); }
		
                /**
                 * Returns the type name
                 * @return string 
                 */
		public function get_type_name()				{ return get_value(BASE_DBQ_LOG::get_type_list(),$this->get_type()); }
		
                /**
                 * Returns the level name
                 * @return string 
                 */
                public function get_level_name()			{ return get_value(BASE_CMODEL_LOGGER::get_level_list(),$this->get_level()); }
		
                /**
                 * Enables the state success
                 */
		public function enable_state_success()			{ $this->set_state(BASE_DBQ_LOG::STATE_SUCCESS); }
		
                /**
                 * Enables the state failed
                 */
                public function enable_state_failed()			{ $this->set_state(BASE_DBQ_LOG::STATE_FAILED); }
		
                /**
                 * Success 
                 */
		public function success()				{ $this->enable_state_success(); }
		
                /**
                 * Failed 
                 */
                public function failed()				{ $this->enable_state_failed(); }
		
                /**
                 * Checks whether is success
                 * @return boolean 
                 */
		public function is_success()				{ return $this->_log_dbo->get_state()==BASE_DBQ_LOG::STATE_SUCCESS; }
		
                /**
                 * Checks whether is failed
                 * @return boolean 
                 */
                public function is_failed()				{ return $this->_log_dbo->get_state()==BASE_DBQ_LOG::STATE_FAILED; }
				
		/**
                 * Checks whether is type object
                 * @return boolean 
                 */
		public function is_type_object()			{ return BASE_DBQ_LOG::TYPE_OBJECT==$this->get_type(); }
		
                /**
                 * Checks whether is email object
                 * @return boolean 
                 */
                public function is_email_object()			{ return BASE_DBQ_LOG::TYPE_OBJECT==$this->get_type(); }
		
                /**
                 * Returns the medium date time
                 * @return string 
                 */
                public function get_medium_date_time()			{ return FORMAT_UTIL::get_formatted_time("%B %e %Y %0g:%M:%S %0a",$this->get_create_date()); }
		
                /**
                 * Returns the short date time
                 * @return string 
                 */
                public function get_short_date_time()			{ return FORMAT_UTIL::get_short_date_time($this->get_create_date()); }
		
                /**
                 * Add
                 */
		public function add() {
			return self::save();
		}

                /**
                 * Saves
                 * @return boolean 
                 */
		public function save() {

			DB::start_transaction();

			$has_success = false;

			if(is_numeric($this->get_log_id())) {
				$has_success = $this->_log_dbo->update();
			} else {
				
				if(!$this->get_type())	
					$this->set_type(BASE_DBQ_LOG::TYPE_GENERIC);
			
				$this->set_create_date(DB::get_date_time());
				
				$has_success = $this->_log_dbo->insert();
			}

			$has_success = DB::has_transaction_success();
			DB::complete_transaction();

			return $has_success;

		}

                /**
                 * populates
                 * @param array $data
                 * @param boolean $untag
                 * @return boolean 
                 */
		public function populate($data,$untag=false) {
			return $this->populate_dbo($data,$untag);
		}
			
	}