<?php
        /**
        * Class BASE_CMODEL_EMAIL_EVENT_SETTING
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_EMAIL_EVENT_SETTING extends MODEL_COMPLEX {
                
                /**
                 * The email event setting dbo
                 * @var object $email_event_setting_dbo 
                 */
		protected $_email_event_setting_dbo	= null;
                
                /**
                 * Label
                 * @var string $label 
                 */
		protected $_label			= "";
		
                /**
                 * Constructor
                 * @param string $name
                 * @param string $label 
                 */
		public function __construct($name="",$label="") {
			$this->_email_event_setting_dbo = new BASE_DBO_EMAIL_EVENT_SETTING();
			$this->register_dbo($this->_email_event_setting_dbo);
			
			$this->set_name($name);
			$this->_label = $label;
		}
		
                /**
                 * Returns the label
                 * @return string 
                 */
		public function get_label()		{ return $this->_label; }
		
                /**
                 * Sets the label
                 * @param string $label 
                 */
		public function set_label($label)	{ $this->_label = $label; }
		
                /**
                 * Check whether it exists
                 * @return boolean 
                 */
		public function exists() 		{ return !is_empty($this->get_db_email_event_setting_id()); }
		
                /**
                 * Returns the db email event setting id
                 * @return mix 
                 */
		public function get_db_email_event_setting_id() {
			
			$email_event_setting_dbq = new BASE_DBQ_EMAIL_EVENT_SETTING();
			$email_event_setting_dbq->add_where("name","=",$this->get_name());
			return $email_event_setting_dbq->select_one("email_event_setting_id");			
		}		
		
                /**
                 * Saves
                 * @return boolean 
                 */
		public function save() {
			
			$has_success = false;
			
			$this->set_email_event_setting_id($this->get_db_email_event_setting_id());
			
			if(is_numeric(self::get_email_event_setting_id())) {
				$has_success = $this->_email_event_setting_dbo->update();
				
			} else {
				$has_success = $this->_email_event_setting_dbo->insert();
			}
			
			return $has_success;
		}
		
                /**
                 * Populates
                 * @param array $data
                 * @param boolean $untag
                 * @param boolean $previous
                 * @return boolean 
                 * @todo review description and return type
                 */
		public function populate($data,$untag=false,$previous=false) {
			return $this->populate_dbo($data,$untag,$previous);
		}
	}