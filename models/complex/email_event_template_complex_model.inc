<?php
        /**
        * Class BASE_CMODEL_EMAIL_EVENT_TEMPLATE
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_EMAIL_EVENT_TEMPLATE extends MODEL_COMPLEX {
	
		const VAR_EMAIL_CONTENT = "{\$email_content}";
		
                /**
                 * The email event template dbo
                 * @var object $email_event_template_dbo
                 */
		protected $_email_event_template_dbo	= null;
		
                /**
                 * Constructor 
                 */
		public function __construct() {
			$this->_email_event_template_dbo = new BASE_DBO_EMAIL_EVENT_TEMPLATE();
			$this->register_dbo($this->_email_event_template_dbo);
		}
		
                /**
                 * Checks whether state is active
                 * @return boolean 
                 */
		public function is_state_active()	{ return $this->get_state()==BASE_DBQ_EMAIL_EVENT_TEMPLATE::STATE_ACTIVE; }
		
                /**
                 * Checks whether state is deleted
                 * @return boolean 
                 */
                public function is_state_deleted()	{ return $this->get_state()==BASE_DBQ_EMAIL_EVENT_TEMPLATE::STATE_DELETED; }

                /**
                 * Returns state name
                 * @return string
                 */
		public function get_state_name()	{ return get_value(BASE_DBQ_EMAIL_EVENT_TEMPLATE::get_state_list(),$this->get_state()); }
		
                /**
                 * Returns format name
                 * @return string
                 */
                public function get_format_name()	{ return get_value(BASE_DBQ_EMAIL_EVENT_TEMPLATE::get_format_list(),$this->get_format()); }

                /**
                 * Checks whether format is html
                 * @return boolean 
                 */
		public function is_format_html()	{ return $this->get_format()==BASE_DBQ_EMAIL_EVENT_TEMPLATE::FORMAT_HTML; }		
		
                /**
                 * Checks whether format is text
                 * @return boolean 
                 */
                public function is_format_text()	{ return $this->get_format()==BASE_DBQ_EMAIL_EVENT_TEMPLATE::FORMAT_TEXT; }

                /**
                 * Deletes
                 * @return boolean 
                 */
		public function delete() {
			$this->set_state(BASE_DBQ_EMAIL_EVENT_TEMPLATE::STATE_DELETED);
			return $this->_email_event_template_dbo->update();
		}
		
                /**
                 * Saves
                 * @return boolean 
                 */
		public function save() {
			DB::start_transaction();
			
			$has_success = false;
			if(is_numeric(self::get_email_event_template_id())) {
				$has_success = $this->_email_event_template_dbo->update();
				
			} else {
				if(!$this->get_guid())
					$this->set_guid(MISC_UTIL::get_guid());
					
				if(!$this->get_state())
					$this->set_state(BASE_DBQ_EMAIL_EVENT_TEMPLATE::STATE_ACTIVE);
					
				$has_success = $this->_email_event_template_dbo->insert();
			}
			
			$has_success = DB::has_transaction_success();
			DB::complete_transaction();
			
			return $has_success;
		}
		
                /**
                 * populates
                 * @param array $data
                 * @param boolean $untag
                 * @param boolean $previous
                 * @return boolean 
                 */
		public function populate($data,$untag=false,$previous=false) {								
			return $this->populate_dbo($data,$untag,$previous);
		}
	}