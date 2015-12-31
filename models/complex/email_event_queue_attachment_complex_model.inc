<?php
        /**
        * Class BASE_CMODEL_EMAIL_EVENT_QUEUE_ATTACHMENT
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_EMAIL_EVENT_QUEUE_ATTACHMENT extends MODEL_COMPLEX {
                
                /**
                 * The email event queue attachment dbo
                 * @var object $email_event_queue_attachment_dbo 
                 */
		private $_email_event_queue_attachment_dbo	= null;
		
                /**
                 * Constructor 
                 */
		public function __construct() {
			$this->_email_event_queue_attachment_dbo = new BASE_DBO_EMAIL_EVENT_QUEUE_ATTACHMENT();
			$this->register_dbo($this->_email_event_queue_attachment_dbo);
		}
		
                /**
                 * Returns the local file
                 * @return string
                 */
		public function get_local_file() 	{ return MODEL_PATH::get_temp_directory().$this->get_local_filename(); }
		
                /**
                 * Returns the local filename
                 * @return string 
                 */
                public function get_local_filename() 	{ return $this->get_guid().".".$this->get_extension(); }
		
                /**
                 * Returns the extension
                 * @return string 
                 */
                public function get_extension() 	{ return FILE_UTIL::get_file_extension($this->get_filename()); }
		
                /**
                 * Returns the state name
                 * @return string 
                 */
		public function get_state_name()	{ return get_value(BASE_DBQ_EMAIL_EVENT_QUEUE_ATTACHMENT::get_state_list(),$this->get_state()); }

                /**
                 * Deletes the email event queue attachment
                 * @return boolean 
                 */
		public function delete() {
			$this->set_state(BASE_DBQ_EMAIL_EVENT_QUEUE_ATTACHMENT::STATE_DELETED);
			return $this->_email_event_queue_attachment_dbo->update();
		}
                
                /**
                 * Saves
                 * @return boolean 
                 */
		public function save() {
			DB::start_transaction();
			
			$has_success = false;
			if(is_numeric(self::get_email_event_queue_attachment_id())) {
				$has_success = $this->_email_event_queue_attachment_dbo->update();
			} else {
				if(!$this->get_guid())
					$this->set_guid(MISC_UTIL::get_guid());
				if(!$this->get_state())
					$this->set_state(BASE_DBQ_EMAIL_EVENT_QUEUE_ATTACHMENT::STATE_ACTIVE);
				$has_success = $this->_email_event_queue_attachment_dbo->insert();
			}
			
			$has_success = DB::has_transaction_success();
			DB::complete_transaction();
			
			return $has_success;
		}
		
                /**
                 * Commits the file
                 * @param string $file
                 * @return boolean
                 */
		public function commit_file($file) {
			return FILE_UTIL::copy($file,$this->get_local_file());					
		}
		
                /**
                 * Populates
                 * @param array $data
                 * @param boolean $untag
                 * @return mixed
                 * @todo review return type 
                 */
		public function populate($data,$untag=false) {								
			return $this->populate_dbo($data,$untag);
		}
	}