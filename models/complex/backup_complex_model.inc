<?php
        /**
        * Class BASE_CMODEL_BACKUP
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_BACKUP extends MODEL_COMPLEX {
                
                /**
                 * The backup_dbo
                 * @var object $backup_dbo 
                 */
		private $_backup_dbo	= null;
                
                /**
                 * The directory
                 * @var string $directory 
                 */
		private $_directory	= null;

                /**
                 * Constructor 
                 */
		public function __construct() {
			$this->_backup_dbo = new BASE_DBO_BACKUP();
			$this->register_dbo($this->_backup_dbo);
		}
                
                /**
                 * Returns the state name
                 * @return string 
                 */
		public function get_state_name()		{ return get_value(BASE_DBQ_BACKUP::get_state_list(),$this->get_state()); }
		
                /**
                 * Returns the duration
                 * @return integer 
                 */
		public function get_duration() 			{ return $this->get_end_time() - $this->get_start_time(); }
		
                /**
                 * Returns the directory
                 * @return string 
                 */
		public function get_directory() 		{ return $this->_directory; }
		
                /**
                 * Returns the file
                 * @return string 
                 */
		public function get_file() 			{ return $this->get_directory().$this->get_filename(); }
		
                /**
                 * Set the directory
                 * @param string $directory
                 */
		public function set_directory($directory) 	{ $this->_directory = $directory; }
		
                /**
                 * Returns the creation time of filename
                 * @return string 
                 */
		public function get_filename_create_time() 	{ return FILE_UTIL::get_file_modified_time($this->get_file()); }
		
                /**
                 * Returns the creation date of filename
                 * @return string 
                 */
                public function get_filename_create_date() 	{ return $this->get_filename_create_time(); }
		
                /**
                 * Returns the size of filename
                 * @return string 
                 */
		public function get_pretty_filesize() 		{ return FORMAT_UTIL::get_formatted_filesize(FILE_UTIL::get_filesize($this->get_file())); }
		
                /**
                 * Saves
                 * @return boolean 
                 */
		public function save() {

			$has_success = false;

			if(is_numeric($this->get_backup_id())) {
				$has_success = $this->_backup_dbo->update();
			} else {
				if(!$this->get_state())
					$this->set_state(BASE_DBQ_BACKUP::STATE_SUCCESS);
			
				$has_success = $this->_backup_dbo->insert();
			}

			return $has_success;

		}

                /**
                 * Populates
                 * @param array $data
                 * @param boolean $untag
                 * @return mix
                 * @todo review return type 
                 */
		public function populate($data,$untag=false) {
			return $this->populate_dbo($data,$untag);
		}

	}