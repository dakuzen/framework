<?php
        /**
        * Class BASE_CMODEL_EMAIL_EVENT_LOG
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_EMAIL_EVENT_LOG extends BASE_CMODEL_LOG {
                
                /**
                 * email event log dbo
                 * @var object $email_event_log_dbo 
                 */
		private $_email_event_log_dbo	= null;

                /**
                 * Constructor
                 */
		public function __construct() {
			parent::__construct();
			$this->_email_event_log_dbo = new BASE_DBO_EMAIL_EVENT_LOG();
		}

                /**
                 * Return email event log id
                 * @return integer 
                 */
		public function get_email_event_log_id()		{ return $this->_email_event_log_dbo->get_email_event_log_id(); }
		
                /**
                 * Returns the log id
                 * @return integer 
                 */
                public function get_log_id()				{ return $this->_email_event_log_dbo->get_log_id(); }
		
                /**
                 * Returns the email event id
                 * @return integer
                 */
                public function get_email_event_id()			{ return $this->_email_event_log_dbo->get_email_event_id(); }
		
                /**
                 * Returns the email event queue id
                 * @return integer 
                 */
                public function get_email_event_queue_id()		{ return $this->_email_event_log_dbo->get_email_event_queue_id(); }

                /**
                 * sets the email event log id
                 * @param integer $value 
                 */
		public function set_email_event_log_id($value)		{ $this->_email_event_log_dbo->set_email_event_log_id($value); }
		
                /**
                 * sets the email log id
                 * @param integer $value 
                 */
                public function set_log_id($value)			{ $this->_email_event_log_dbo->set_log_id($value); }
		
                /**
                 * sets the email event id
                 * @param integer $value 
                 */
                public function set_email_event_id($value)		{ $this->_email_event_log_dbo->set_email_event_id($value); }
		
                /**
                 * sets the email queue log id
                 * @param integer $value 
                 */
                public function set_email_event_queue_id($value)	{ $this->_email_event_log_dbo->set_email_event_queue_id($value); }

                /**
                 * Add
                 * @return boolean 
                 */
		public function add() {

			DB::start_transaction();

			$has_success = false;
			
			$this->set_type(BASE_DBQ_LOG::TYPE_EMAIL_EVENT);
			
			if(parent::save()) {
				
				self::set_log_id(parent::get_log_id());
				
				if(is_numeric($this->get_email_event_log_id())) {
					$has_success = $this->_email_event_log_dbo->update();
				} else {
					$has_success = $this->_email_event_log_dbo->insert();
				}
			}

			$has_success = DB::has_transaction_success();
			DB::complete_transaction();

			return $has_success;

		}

                /**
                 * populate
                 * @param array $data
                 * @param boolean $untag
                 * @return boolean 
                 */
		public function populate($data,$untag=false) {

			if(!is_array($data) || !$data)
				return false;
				
			parent::populate($data,$untag);

			if(array_key_exists("email_event_log_id",$data))
				self::set_email_event_log_id(get_value($data,"email_event_log_id"));

			if(array_key_exists("log_id",$data))
				self::set_log_id(get_value($data,"log_id"));

			if(array_key_exists("email_event_id",$data))
				self::set_email_event_id(get_value($data,"email_event_id"));

			if(array_key_exists("email_event_queue_id",$data))
				self::set_email_event_queue_id(get_value($data,"email_event_queue_id"));

			if($untag)
				$this->_email_event_log_dbo->untag_object();

			return true;

		}
                
                /**
                 * Returns the default name
                 * @return string 
                 */
		function get_default_name(){
			return "";
		}		
                
                /**
                 * Returns the default description
                 * @return string 
                 */
		function get_default_description(){
			return "";
		}


	}