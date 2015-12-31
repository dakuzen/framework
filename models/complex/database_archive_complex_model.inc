<?
	/**
	* Class BASE_CMODEL_DATABASE_ARCHIVE
	*
	* @package Framework
	* @subpackage Cmodels
	*/

	class BASE_CMODEL_DATABASE_ARCHIVE extends MODEL {
	
		const RETENTION_DAYS 			= 7;
		
		/**
		 * The backup directory
		 * @var string $backup_directory
		 */
		protected $_backup_directory	= "";

		/**
		 * The db host
		 * @var string $db_host 
		 */
		protected $_db_host 			= null;

		/**
		 * The db name
		 * @var string $db_name 
		 */
		protected $_db_name 			= null;

		/**
		 * The db user 
		 * @var string $db_user  
		 */
		protected $_db_user 			= null;

		/**
		 * The db pass
		 * @var string $db_pass 
		 */	
		protected $_db_pass 			= null;

		/**
		 * The db sock
		 * @var string $db_sock 
		 */
		protected $_db_sock 			= null;

		/**
		 * The db sock
		 * @var string $db_port 
		 */
		protected $_db_port 			= null;

		/**
		 * The start time
		 * @var integer $start_time 
		 */
		protected $_start_time 			= 0;

		/**
		 * The end time
		 * @var integer $end_time 
		 */
		protected $_end_time 			= 0;

		/**
		 * Include structure
		 * @var boolean $include_structure 
		 */
		protected $_include_structure 	= true;

		/**
		 * include data
		 * @var boolean $include_data 
		 */
		protected $_include_data 		= true;

		/**
		 * The zip archive
		 * @var boolean $zip_archive 
		 */
		protected $_zip_archive 		= true;

		/**
		 * the archive file
		 * @var string $archive_file
		 */
		protected $_structure_file 		= "";

		protected $_data_file	 		= "";

		protected $_basename 			= "";

		/**
		 * Constructor
		 * @param string $db_host
		 * @param string $db_name
		 * @param string $db_user
		 * @param string $db_pass
		 * @param string $backup_directory
		 * @param array $options 
		 */
		function __construct($db_host,$db_name,$db_user,$db_pass,$backup_directory="",$options=array()) {
			$this->_db_host 			= $db_host;			
			$this->_db_name 			= $db_name;
			$this->_db_user 			= $db_user;
			$this->_db_pass 			= $db_pass;
			$this->_backup_directory 	= $backup_directory;
			$this->_db_sock 			= value($options,"sock");
			$this->_db_port 			= value($options,"port");			
			$this->_include_structure 	= value($options,"include_structure",true);
			$this->_include_data		= value($options,"include_data",true);
			$this->_basename			= $db_name."_".date("Y-m-d-H-i-s");
		}

		/**
		* Return time
		* @return integer 
		*/
		public function get_time() 					{ return $this->_end_time - $this->_start_time; }

		/**
		* Return the start time
		* @return integer 
		*/
		public function get_start_time() 			{ return $this->_start_time; }

		/**
		* Return the end time
		* @return integer 
		*/
		public function get_end_time() 				{ return $this->_end_time; }

		/**
		* Return the archive file
		* @return string
		*/
		public function get_structure_file()		{ return $this->_structure_file; }

		public function get_data_file()				{ return $this->_data_file; }

		/**
		* Returns the archive filename
		* @return string
		*/
		public function get_structure_filename()	{ return basename($this->_structure_file); }

		public function get_data_filename()			{ return basename($this->_data_file); }
		
		public function get_backup_directory()		{ return $this->_backup_directory; }
		/**
		* Return the db host
		* @return string
		*/
		public function get_db_host()				{ return $this->_db_host; }

		/**
		* Return the db name
		* @return string
		*/
		public function get_db_name()				{ return $this->_db_name; }

		/**
		* Return the db user
		* @return string
		*/
		public function get_db_user()				{ return $this->_db_user; }

		/**
		* Return the db pass
		* @return string
		*/
		public function get_db_pass()				{ return $this->_db_pass; }

		/**
		* Return the db sock
		* @return string
		*/
		public function get_db_sock()				{ return $this->_db_sock; }

		/**
		* Return the db sock
		* @return string
		*/
		public function get_db_port()				{ return $this->_db_port; }

		public function get_basename()				{ return $this->_basename; }
		public function set_basename($basename)		{ $this->_basename = $basename; return $this; }

		/**
		* Sets the db sock
		* @param string $value 
		*/
		public function set_db_sock($value)			{ $this->_db_sock = $value; }

		/**
		* Disables the zip archive 
		*/
		public function disable_zip_archive()		{ $this->_zip_archive = false; }

	    /**
		* Sets the backup direcotry
		* @param string $directory
		* @return boolean 
		*/
		public function set_backup_directory($directory)	{ return $this->_backup_directory = $directory; }

		/**
		* Creates the backup directory
		* @param string $backup_directory 
		*/
		public function create_backup_directory($backup_directory="") {

			$backup_directory = $backup_directory ? $backup_directory : $this->_backup_directory;
			
			if($backup_directory) {			 
				FILE_UTIL::mkdir($backup_directory); 
				FILE_UTIL::chown($backup_directory,"apache","apache");
			}
		}
		
        /**
         * Archive
         * @return boolean
         * @throws Exception 
         */
		public function archive() {

			$this->_start_time = microtime(true);
			
			$basename				= $this->get_basename();
			$backup_dir				= $this->_backup_directory;
			$this->_structure_file	= $backup_dir.$basename.".sql";
			$this->_data_file		= $backup_dir.$basename."-data.sql";

			$this->create_backup_directory();

			if(!$this->_backup_directory) 
				throw new Exception("The backup destination directory is not supplied");
			
			if(!is_dir($this->_backup_directory)) 
				throw new Exception("The backup destination directory does not exist (".$this->_backup_directory.")");

			if(!$this->_db_user)
				throw new Exception("The db user name is not supplied");
				
			if(!$this->_db_pass) 
				throw new Exception("The db password is not supplied");
				
			if(!$this->_db_host && !$this->_db_sock) 
				throw new Exception("The db host or db sock is not supplied");
				
			if(!$this->_db_name) 
				throw new Exception("The db name not supplied");					
			
			if(!is_dir($backup_dir)) 
				throw new Exception("The backup directory does not exist (".$backup_dir.")");
				
			if(!is_writeable($backup_dir)) 
				throw new Exception("The backup directory is not writable (".$backup_dir.")");
			
			DB::include_db_mysqli();
			
			$timer_cmodel = new BASE_CMODEL_TIMER();
			$timer_cmodel->start();

			$this->message_callback("Exporting the ".$this->_db_name." database...");
		
			$db_utility = DB_MYSQLI::get_db_utility($this->get_db_host(),$this->get_db_user(),$this->get_db_pass(),$this->get_db_name(),array("port"=>$this->get_db_port(),"sock"=>$this->get_db_sock()));
			
			$db_utility->export($this->get_db_name(),$this->_structure_file,array("--single-transaction","--no-data"));
			$db_utility->export($this->get_db_name(),$this->_data_file,array("--single-transaction","--no-create-info","--skip-add-locks","--skip-comments","--disable-keys"));
			
			$timer_cmodel->end();

			$this->message_callback("Completed the database export in ".$timer_cmodel->get_pretty());
			
			FILE_UTIL::chown($this->_structure_file,SYSTEM_MANAGER::get_filesystem_user(),SYSTEM_MANAGER::get_filesystem_group());
			FILE_UTIL::chown($this->_data_file,SYSTEM_MANAGER::get_filesystem_user(),SYSTEM_MANAGER::get_filesystem_group());

			$file = FILE_UTIL::get($this->_structure_file);

			$file = preg_replace("/AUTO_INCREMENT=\d+/i","",$file);

			$file = "SET AUTOCOMMIT = 0;
					SET FOREIGN_KEY_CHECKS = 0;
					".$file."
					SET FOREIGN_KEY_CHECKS = 1;
					COMMIT;
					SET AUTOCOMMIT = 1;";

			FILE_UTIL::put($this->_structure_file,$file);

			if($this->_zip_archive) {
				
				$this->message_callback("Zipping the sql files...");
				
				ZIP_UTIL::create($this->get_archive_file())
					->add($this->_structure_file,basename($this->_structure_file))
					->add($this->_data_file,basename($this->_data_file))
					->close();

				FILE_UTIL::delete($this->_structure_file);
				FILE_UTIL::delete($this->_data_file);
			}						

			$this->_end_time = microtime(true);
			
			return $this;
		}

		function get_archive_filename() {
			return $this->get_basename().".zip";
		}

		function get_archive_file() {
			return $this->get_backup_directory().$this->get_archive_filename();
		}

		function get_archive_filesize() {	
			return FILE_UTIL::filesize($this->get_archive_file());
		}

        /**
         * Purge expired
         * @param integer $retention_days 
         */
		function purge_expired($retention_days=self::RETENTION_DAYS) {
			
			$retention_time = mktime(0,0,0,date("n"),date("j")-$retention_days,date("Y"));

			$archived_backup_filenames = FILE_UTIL::get_directory_listing($this->_backup_directory);

			foreach($archived_backup_filenames as $filename) {

				$file 			= $this->_backup_directory.$filename;
				$modified_time 	= FILE_UTIL::get_file_modified_time($file);

				if($this->can_purge($filename,$modified_time,$retention_time)) 
					FILE_UTIL::delete($file);
			}
		}

		function can_purge($filename,$modified_time,$retention_time) {
			return $modified_time<$retention_time;
		}
		
        /**
         * message callback
         * @param string $message 
         */
		function message_callback($message) {}
	}
	