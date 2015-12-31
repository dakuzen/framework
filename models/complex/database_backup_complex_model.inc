<?php
        /**
        * Class BASE_CMODEL_DATABASE_BACKUP
        *
        * @package Framework
        * @subpackage Cmodels
        */

	/*
		CREATE TABLE `backups` (                                
		`backup_id` int(11) NOT NULL AUTO_INCREMENT,          
		`start_time` decimal(15,2) DEFAULT NULL,              
		`end_time` decimal(15,2) DEFAULT NULL,                
		`filesize` int(11) DEFAULT NULL,                      
		`filename` varchar(50) DEFAULT NULL,                  
		`message` varchar(255) DEFAULT NULL,                  
		`state` char(1) DEFAULT NULL,                         
		PRIMARY KEY (`backup_id`)) ENGINE=InnoDB;
	*/
	
	class BASE_CMODEL_DATABASE_BACKUP extends MODEL {
                
                /**
                 * The backup dir
                 * @var string $backup_dir
                 */
		protected $_backup_dir 		= "";
                
                /**
                 * The retention days
                 * @var integer $retention_days
                 */
		protected $_retention_days 	= "7";
                
                /**
                 * The ini util
                 * @var string $ini_util 
                 */
		protected $_ini_util 		= null;
                
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
                 * the db pass
                 * @var string $db_pass 
                 */
		protected $_db_pass 		= null;
                
                /**
                 * The db sock
                 * @var string $db_sock 
                 */
		protected $_db_sock 		= null;
	
                /**
                 * Constructor
                 * @param string $db_host
                 * @param string $db_user
                 * @param string $db_pass
                 * @param string $db_name
                 * @param string $backup_dir 
                 */
		function __construct($db_host=null,$db_user=null,$db_pass=null,$db_name=null,$backup_dir=null) {
			
			$this->_ini_util 	= new INI_UTIL(SYSTEM_MANAGER::get_config_file(),true);
			
			$this->_backup_dir 	= $backup_dir===null ? BASE_MODEL_PATH::get_instance_directory()."backups".DIRECTORY_SEPARATOR : $backup_dir;
			
			$this->_db_host 	= $db_host===null ? $this->_ini_util->get("DB_HOST") : $db_host;
			$this->_db_name		= $db_name===null ? $this->_ini_util->get("DB_NAME") : $db_name;
			$this->_db_sock		= $this->_ini_util->get("DB_SOCK");
			$this->_db_user 	= $db_user===null ? $this->_ini_util->get(array("backup","DB_USER")) : $db_user;
			$this->_db_pass 	= $db_pass===null ? $this->_ini_util->get(array("backup","DB_PASS")) : $db_pass;
		}
		
                /**
                 * Sets the retention days
                 * @param integer $days 
                 */
		public function set_retention_days($days) { $this->_retention_days = $days; }
				
                /**
                 * Process a backup
                 * @return boolean 
                 */
		public function process_backup() {
					
			if(!$this->_db_user)
				$this->add_error_message("The config.ini [backup] DB_USER not supplied (E10011)");
				
			if(!$this->_db_pass) 
				$this->add_error_message("The config.ini [backup] DB_PASS not supplied (E10012)");
				
			if(!$this->_db_host && !$this->_db_sock) {
				
				if(!$this->_db_host)
					$this->add_error_message("The config.ini DB_HOST not supplied  (E10013)");
				
				if(!$this->_db_sock)
					$this->add_error_message("The config.ini DB_SOCK not supplied  (E10015)");
			}
				
			if(!$this->_db_name) 
				$this->add_error_message("The config.ini DB_NAME not supplied  (E10014)");
			
			if($this->get_error_messages())
				return false;
				
			try {
				$db = DB::get_db_connection($this->_db_host, $this->_db_user, $this->_db_pass,$this->_db_name,DB::DB_TYPE_MYSQLI,array("db_sock"=>$this->_db_sock));
				
				DB::set_default_db_connection($db);
				
				$db_utility = $db->get_db_utility();				
				
			} catch(Exception $e) {
				$this->add_error_message("Could not connect to application: ".$e->getMessage());
				return false;
			}

			
			$database_archive_cmodel = new BASE_CMODEL_DATABASE_ARCHIVE($this->_db_host,$this->_db_name,$this->_db_user,$this->_db_pass);
			$database_archive_cmodel->create_backup_directory($this->_backup_dir);
			$database_archive_cmodel->set_backup_directory($this->_backup_dir);
				
			$has_success = false;
			
			try {
				$has_success = $database_archive_cmodel->archive();
				
				if($this->_retention_days)
					$database_archive_cmodel->purge_expired($this->_retention_days);
				
			} catch(Exception $e) {
				$this->add_error_message($e->getMessage());
			}
			
			if(!$has_success && SERVER_UTIL::is_command_application()) {

				$has_notify_success = false;

				if(SYSTEM_MANAGER::get_notify_recipients()) {
					$email_cmodel = SYSTEM_MANAGER::get_email();

					if($email_cmodel) {
						$email_cmodel->add_to_recipients(SYSTEM_MANAGER::get_notify_recipients());
						$email_cmodel->set_body(implode(",",$this->get_error_messages()));
						$email_cmodel->set_subject("Database Backup Failure");
						$has_notify_success = $email_cmodel->send();

						if(!$has_notify_success)
							$this->add_error_message("Failed to send notification to: ".implode(", ",SYSTEM_MANAGER::get_notify_recipients())." - ".$email_cmodel->get_error_message()."  (E10014)");
					} else
						$this->add_error_message("Failed to load email utility (E10015)");
				} else
					$this->add_error_message("There are no notify recipients specified (E10016)");
			}

			DB::debug();

			$backup_cmodel = new BASE_CMODEL_BACKUP();
			$backup_cmodel->set_start_time($database_archive_cmodel->get_start_time());
			$backup_cmodel->set_end_time($database_archive_cmodel->get_end_time());
			$backup_cmodel->set_filename($database_archive_cmodel->get_archive_filename());
			$backup_cmodel->set_filesize($database_archive_cmodel->get_archive_filesize());
			$backup_cmodel->set_message(implode(", ",$this->get_error_messages()));
			$backup_cmodel->set_state(($has_success ? BASE_DBQ_BACKUP::STATE_SUCCESS : BASE_DBQ_BACKUP::STATE_FAILED));
			$backup_cmodel->save();
							
			CMODEL_SETTING::set_instance_db_backup(time());
				
			return $has_success;
		}
	}
	