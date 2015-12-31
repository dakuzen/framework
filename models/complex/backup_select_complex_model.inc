<?
    /**
    * Class BASE_CMODEL_BACKUP_SELECT
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_BACKUP_SELECT extends MODEL_COMPLEX {

		/**
		 * The backup directory
		 * @var string $backup_directory 
		 */
		private $_backup_directory	= "";

		/**
		 * The backup cmodels
		 * @var array $backup_cmodels
		 */
		private $_backup_cmodels	= array();

		/**
		 * Constructor
		 * @param string $backup_directory 
		 */
		public function __construct($backup_directory) {
			$this->_backup_directory = $backup_directory;
		}
		
        /**
         * Returns the backups
         * @return array 
         */
		public function get_backups() {
			
			$this->_backup_cmodels = array();
			
			$index = 1;

			$backup_directory = $this->_backup_directory."/";

			$filenames = FILE_UTIL::get_directory_listing($backup_directory,"*.{zip,tar,gz}",false,false,false,GLOB_BRACE);

			sort($filenames);
			
			foreach($filenames as $filename) {
			
				$file = $backup_directory.$filename;
				
				$time = FILE_UTIL::get_file_modified_time($file);
				$filesize = FILE_UTIL::get_filesize($file);
				
				$backup_cmodel = new BASE_CMODEL_BACKUP();
				$backup_cmodel->set_filename($filename);
				$backup_cmodel->set_start_time($time);
				$backup_cmodel->set_filesize($filesize);
				$backup_cmodel->set_end_time($time);
				$backup_cmodel->set_directory($backup_directory);

				$this->_backup_cmodels[$index] = $backup_cmodel;

				$index++;
			}

			return $this->_backup_cmodels;
		}
		
        /**
         * Returns the maximum filename length
         * @return integer 
         */
		public function get_max_filename_length() {
			
			$max_length = 0;
		
			foreach($this->get_backups() as $number=>$backup_cmodel) {
				$length = strlen($backup_cmodel->get_filename());
				
				if($length>$max_length)
					$max_length = $length;
			}
			
			return $max_length;
		}
		
        /**
         * Returns the backup
         * @param mix $number
         * @return boolean 
         */
		public function get_backup($number)	{ return get_value($this->_backup_cmodels,$number); }
	}