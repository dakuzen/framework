<?
	class BCMODEL_SYSTEM_INSTANCE extends CMODEL {

		protected $_ini_util				= null;
		protected $_directory				= null;
		protected $_config_file				= null;
		
		public function __construct($directory="") {
			
			$this->_ini_util = new INI_UTIL();

			if($directory)
				$this->set_directory($directory);
		}
		
		public function get_db_host() 								{ return $this->_ini_util->get("DB_HOST"); } 		
		public function get_db_sock() 								{ return $this->_ini_util->get("DB_SOCK"); } 		
		public function get_db_port() 								{ return $this->_ini_util->get("DB_PORT"); } 		
		public function get_db_name() 								{ return $this->_ini_util->get("DB_NAME"); } 		
		public function get_db_user() 								{ return $this->_ini_util->get("DB_USER"); }
		public function get_db_pass() 								{ return $this->_ini_util->get("DB_PASS"); }
		public function get_backup() 								{ return $this->_ini_util->get("BACKUP",true); }
		public function get_maintenance_db_pass()					{ return $this->_ini_util->get(array("maintenance","DB_PASS")); }
		public function get_maintenance_db_user()					{ return $this->_ini_util->get(array("maintenance","DB_USER")); }
		public function get_system_mode() 							{ return array_key_exists($this->_ini_util->get("SYSTEM_MODE"),BASE_SYSTEM_MANAGER::get_system_mode_list()) ? $this->_ini_util->get("SYSTEM_MODE") : BASE_SYSTEM_MANAGER::SYSTEM_MODE_DEBUG; }
		public function get_system_mode_name() 						{ return get_value(BASE_SYSTEM_MANAGER::get_system_mode_list(),$this->get_system_mode()); }
		public function get_notify_recipients() 					{ return BASE_SYSTEM_MANAGER::parse_notify_recipients($this->_ini_util->get("NOTIFY_RECIPIENTS")); }
		public function get_config_file() 							{ return $this->_config_file; }
		public function get_config($name) 							{ return $this->_ini_util->get($name); }
		public function get_backups_directory() 					{ return $this->get_directory()."backups/"; }
		public function get_directory() 							{ return $this->_directory; }
		public function get_application_directory() 				{ return $this->get_directory()."application/"; }
		public function get_framework_directory() 					{ return $this->get_directory()."framework/"; }
		public function get_instance_directory_name() 				{ return basename($this->get_directory()); }
		public function get_data_directory() 						{ return $this->get_directory()."data/"; }
		public function get_logs_directory() 						{ return $this->get_directory()."logs/"; }
		public function get_svn_application_version()				{ return $this->get_svn_version($this->get_application_directory()); }	
		public function get_svn_framework_version()					{ return $this->get_svn_version($this->get_framework_directory()); }	
		public function get_application_version() 					{ return BASE_MODEL_UPGRADE::get_current_version($this->get_directory()."application/upgrade/version"); }			
		public function is_valid() 									{ return is_file($this->get_directory()."/framework/boot/bootstrap.inc"); }		
		public function create_instance_api_client()				{ return new CMODEL_INSTANCE_API_CLIENT($this); }
		
		public function get_svn_framework_url($verbose=false) {

			$svn_cmodel = new CMODEL_SVN();

			if($verbose)
				$svn_cmodel->verbose();

			$framework_info = $svn_cmodel->get_info($this->get_directory()."framework/");
				
			return get_value($framework_info,"url");
		}

		public function get_svn_application_url($verbose=false) {	
			$svn_cmodel = new CMODEL_SVN();
			
			if($verbose)
				$svn_cmodel->verbose();

			$application_info = $svn_cmodel->get_info($this->get_directory()."application/");
			return get_value($application_info,"url");
		}

		public function get_svn_framework_info() {	
			return $this->parse_repository_url($this->get_svn_framework_url());
		}

		public function get_svn_application_info() {	
			return $this->parse_repository_url($this->get_svn_application_url());
		}

		public function parse_repository_url($url) {
			preg_match("/(https?):\/\/([^\/]+)\/(.*)\/(?:trunk|branches)/",$url,$matches);
			return array("protocol"=>get_value($matches,1),"domain"=>get_value($matches,2),"directory"=>get_value($matches,3));
		}					

		public function create_db_connection($type=DB::DB_TYPE_MYSQL) {
			return DB::get_db_connection($this->get_db_host(),$this->get_db_user(),$this->get_db_pass(),$this->get_db_name(),$type);
		}

		public function update() {
		
			$update_cmodel = new CMODEL_UPDATE($this->get_directory());
			$has_success = $update_cmodel->update();		

			if(!$has_success)
				throw new Exception(implode(", ",$update_cmodel->get_error_messages()));
				
			return true;
		}

		public function set_directory($directory) {

			$this->_directory = FILE_UTIL::get_sanitized_directory($directory,"/");
						
			if(is_file($config_file=$this->get_data_directory()."config.ini"))
				$this->_config_file = $config_file; 

			elseif(is_file($config_file=$this->get_directory()."config.ini"))
				$this->_config_file = $config_file;
			
			$this->_ini_util = new INI_UTIL($this->get_config_file(),false);
		}

		public function get_name() {
			preg_match("/([^\\/\\\]+)[\\/\\\]?$/",$this->get_directory(),$matches);
			return get_value($matches,1);
		}
				
		public function get_svn_version($directory) {
			
			$all_wcprops = FILE_UTIL::get_file_contents($directory.".svn/all-wcprops");
		
			$version = "trunk";
			
			if(preg_match("/branches\/(.*)/",$all_wcprops,$matches))
				$version = get_value($matches,1);
				
			return $version;
		}
	}