<?
    /**
    * Class BASE_CMODEL_DATABASE_EXTRACT
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_DATABASE_EXTRACT extends MODEL {
                
        /**
         * The db host
         * @var string $db_host
         */
		protected $_db_host 		= null;

		/**
		 * The db name
		 * @var string $db_name
		 */
		protected $_db_name 		= null;

		/**
		 * The db user 
		 * @var string $db_user 
		 */
		protected $_db_user 		= null;

		/**
		 * The db pass
		 * @var string $db_pass
		 */
		protected $_db_pass 		= null;

		/**
		 * The db options
		 * @var string $db_options
		 */
		protected $_db_options 		= array();

		/**
		 * The start time
		 * @var string $start_time
		 */
		protected $_start_time 		= 0;

		/**
		 * The end time
		 * @var string $end_time
		 */
		protected $_end_time 		= 0;

		/**
		 * The file
		 * @var string $file
		 */
		protected $_file 		= "";

		/**
		 * The instance directory
		 * @var string $instance_directory
		 */
		protected $_instance_directory	= "";

		/**
		 * Constructor
		 * @param string $db_host
		 * @param string $db_name
		 * @param string $db_user
		 * @param string $db_pass
		 * @param string $file
		 * @param string $instance_directory 
		 */
		function __construct($db_host,$db_name,$db_user,$db_pass,$file,$instance_directory="",$db_options=array()) {
			$this->_db_host				= $db_host;			
			$this->_db_name				= $db_name;
			$this->_db_user				= $db_user;
			$this->_db_pass				= $db_pass;
			$this->_file				= $file;
			$this->_instance_directory	= $instance_directory;
			$this->_db_options			= $db_options;
		}		
		
		/**
		 * Returns the time
		 * @return integer 
		 */
		public function get_time() 				{ return $this->_end_time - $this->_start_time; }

		/**
		 * Returns the start time
		 * @return integer
		 */
		public function get_start_time() 			{ return $this->_start_time; }

		/**
		 * Returns the end time
		 * @return integer
		 */
		public function get_end_time() 				{ return $this->_end_time; }

		/**
		 * Extracts data
		 * @param boolean $database_restore
		 * @param boolean $data_directory_restore
		 * @todo review param type
		 * @return boolean
		 * @throws Exception 
		 */
		public function extract($database_restore,$data_directory_restore) {
			
			$this->_start_time = microtime(true);
			
			if(!is_file($this->_file)) 
				throw new Exception("The sql file does not exist");
			
			if(!$this->_db_user)
				throw new Exception("The db user name is not supplied");
				
			if(!$this->_db_pass) 
				throw new Exception("The db password is not supplied");
			
			$db_sock = get_value($this->_db_options,"db_sock");

			if(!$this->_db_host && !$db_sock) {
				
				if(!$this->_db_host)
					throw new Exception("The db host is not supplied");

				if(!$db_sock)
					throw new Exception("The db sock is not supplied");
			}
				
			if(!$this->_db_name) 
				throw new Exception("The db name not supplied");

			$extension 	= FILE_UTIL::get_file_extension($this->_file);
			$is_zip 	= $extension=="zip";
			$is_tar 	= in_array($extension,array("gz","tar"));
			$sql_files 	= $is_zip || $is_tar ? array() : array($this->_file);	
			
			$temp_cmodel = CMODEL_TEMP::create();

			try {

				if($database_restore) {								

					$timer_cmodel = new BASE_CMODEL_TIMER();
					$timer_cmodel->start();				
					
					if($is_zip) {
						
						$this->message_callback("Unzipping the file `".basename($this->_file)."`");
						
						$unzip_util = new UNZIP_UTIL($this->_file);
						$unzip_util->extract($temp_cmodel->dir(),array(),array("sql"));
						
						foreach($unzip_util->get_entries() as $entry) 
							if(!$entry->has_directory() && $entry->get_extension()=="sql")
								$sql_files[] = $temp_cmodel->dir().$entry->get_name();				
					}

					if($is_tar) {
						
						$this->message_callback("Untaring the file `".basename($this->_file)."`");
						
						$tar_util = new TAR_UTIL($this->_file);
					
						foreach($tar_util->get_entries("*.sql") as $entry) {
							$tar_util->extract($temp_cmodel->dir(),$entry->get_name());
							$sql_files[] = $temp_cmodel->dir().$entry->get_name();
						}
					}

					if(!$sql_files)
						throw new Exception("Failed to locate sql file");	

				
					$db = DB::get_db_connection($this->_db_host,$this->_db_user,$this->_db_pass,"",DB::DB_TYPE_MYSQLI,$this->_db_options);

					$db_utility = $db->get_db_utility($this->_db_host,$this->_db_user,$this->_db_pass,"",$this->_db_options);

					if($db_utility->is_database_exist($this->_db_name)) {

						$db_utility->drop_database($this->_db_name);

						$this->message_callback("Successfully dropped the `".$this->_db_name."` database");
					}

					$db_utility->create_database($this->_db_name);
					
					$this->message_callback("Successfully created the `".$this->_db_name."` database");

					$parms[] = "--user=".$this->_db_user;
					$parms[] = "--password=".$this->_db_pass;
					
					if($db_sock=get_value($this->_db_options,"sock"))
						$parms[] = " --socket=".$db_sock;
					else
						$parms[] = "--host=".$this->_db_host;

					if($db_port=get_value($this->_db_options,"port"))
						$parms[] = "--port=".$db_port;

					$parms[] = $this->_db_name;

					rsort($sql_files);

					foreach($sql_files as $sql_file) {

						$cmd = "mysql ".implode(" ",$parms)." 2>&1 < ".$sql_file;

						$this->message_callback("Importing the file ".basename($sql_file)." to the ".$this->_db_name." database...");

						$has_success = SERVER_UTIL::exec($cmd,$output,true);

						$timer_cmodel->end();

						if(!$has_success)
							throw new Exception(implode(" ",$output));
						
						$this->message_callback("Successfully imported ".basename($sql_file)." in ".$timer_cmodel->get_pretty());						
					}
				}

				if($data_directory_restore) {

					$timer_cmodel = new BASE_CMODEL_TIMER();
					$timer_cmodel->start();				
					
					$this->message_callback("Determining the archive file count...");

					$file_count = 0;

					if($is_zip) {
						$unzip_util = new UNZIP_UTIL($this->_file);
						$file_count = count($unzip_util->get_entries());
					}

					if($is_tar) {
						$tar_util = new TAR_UTIL($this->_file);
						$file_count = count($tar_util->get_entries());
					}

					$this->message_callback("Started extracting data folder with ".FORMAT_UTIL::get_number($file_count,0)." files...");

					$data_directory = $this->_instance_directory."data/";

					if($is_zip) {
						if($unzip_util->has_command_unzip())
							$unzip_util->extract_command($data_directory);							
						else 
							$unzip_util->extract($data_directory,array("data"));
					}

					if($is_tar)
						$tar_util->extract($this->_instance_directory,"data");

					$timer_cmodel->end();
					
					$this->message_callback("Completed extracting the data folder in ".$timer_cmodel->get_pretty());
					
					$this->message_callback("Chown'ing the instance directory...");
					
					$timer_cmodel->start();
					
					FILE_UTIL::chown($this->_instance_directory,"apache","apache");
					
					$timer_cmodel->end();
					
					$this->message_callback("Completed Chown'ing the instance directory in ".$timer_cmodel->get_pretty());					
				}
				
			} catch(Exception $e) {
				throw new Exception($e->getMessage());
			}

			$temp_cmodel->cleanup();

			$this->_end_time = microtime(true);
			
			return true;
		}
		
        /**
         * Message callback
         * @param string $message 
         */
		function message_callback($message) {}				
	}
	