<?
    /**
    * Class BASE_CMODEL_BACKUP
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_FF_BACKUP extends CMODEL {
            
        /**
         * The backup_dbo
         * @var object $backup_dbo 
         */
		private $_ff_backup_dbo	= null;
                
        /**
         * The directory
         * @var string $directory 
         */
		private $_directory	= null;

        /**
         * Constructor 
         */
		public function __construct() {
			parent::__construct(BASE_DBO_FF_BACKUP::create());
		}
                
        /**
         * Returns the state name
         * @return string 
         */
		public function get_state_name()		{ return value(BASE_DBQ_FF_BACKUP::get_state_list(),$this->get_state()); }

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

            $success = false;
            if(is_numeric(self::get_ff_backup_id())) {
                $success = $this->dbo("ff_backup")->update();

            } else {
                if(!$this->get_state())
                    $this->set_state(BASE_DBQ_FF_BACKUP::STATE_SUCCESS);

                if(!$this->get_create_date())
                    $this->set_create_date(DB::get_date_time());

                $success = $this->dbo("ff_backup")->insert();
            }

            if(!$success)
                throw new Exception("Error saving ".get_class());

            return $this;
		}
	}