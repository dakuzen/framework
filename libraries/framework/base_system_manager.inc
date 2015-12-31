<?php

    /**
    * Class BASE_SYSTEM_MANAGER  
    *
    * @package Library
    * @subpackage Common
    */
	 	 
	abstract class BASE_SYSTEM_MANAGER {
	
		const SYSTEM_MODE_DEBUG			= "D";
		const SYSTEM_MODE_PRODUCTION	= "P";
		const SYSTEM_MODE_STAGING		= "S";
		
		const LANG_EN_US				= "en-us";

		/**
		 * Returns system mode list
		 * @return array 
		 */
		static function get_system_mode_list() { return array(self::SYSTEM_MODE_DEBUG=>"Debug",self::SYSTEM_MODE_PRODUCTION=>"Production",self::SYSTEM_MODE_STAGING=>"Staging"); }

		/**
		 * system host update
		 * @var boolean $system_host_update 
		 */
		protected static $_system_host_update 	= true;

		/**
		 * SYSTEM MODE
		 * @var string $SYSTEM_MODE 
		 */
		protected static $_SYSTEM_MODE 		= self::SYSTEM_MODE_DEBUG;

		/**
		 * SYSTEM NAME
		 * @var string $SYSTEM_NAME 
		 */
		protected static $_SYSTEM_NAME 		= "";

		/**
		 * SYSTEM HOST
		 * @var string $SYSTEM_HOST
		 */
		protected static $_SYSTEM_HOST 		= "";

		/**
		 * SYSTEM APP DIR
		 * @var string $SYSTEM_APP_DIR 
		 */
		protected static $_SYSTEM_APP_DIR 	= "";

		/**
		 * SYSTEM LOG LEVEL
		 * @var string $SYSTEM_LOG_LEVEL 
		 */
		protected static $_SYSTEM_LOG_LEVEL	= "";

		/**
		 * DB LOG
		 * @var boolean $DB_LOG 
		 */
		protected static $_DB_LOG		= false;

		/**
		 * SSL
		 * @var boolean $SSL 
		 */
		protected static $_SSL			= true;

		/**
		 * CRON
		 * @var boolean $CRON 
		 */
		protected static $_CRON			= true;

		/**
		 * NOTIFY RECIPIENTS
		 * @var array $NOTIFY_RECIPIENTS 
		 */
		protected static $_NOTIFY_RECIPIENTS 	= array();

		/**
		 * LANG
		 * @var string $LANG 
		 */
		protected static $_LANG		 	= self::LANG_EN_US;

		/**
		 * create default db connection
		 */
		static function create_default_db_connection() {
		
			$ini_util = new INI_UTIL(self::get_config_file());
			
			$db_host = $ini_util->get("DB_HOST");
			$db_user = $ini_util->get("DB_USER");
			$db_pass = $ini_util->get("DB_PASS");
			$db_name = $ini_util->get("DB_NAME");
			$db_type = $ini_util->get("DB_TYPE");
			$db_sock = $ini_util->get("DB_SOCK");
			$db_port = $ini_util->get("DB_PORT");
			
			$options = array("sock"=>$db_sock,"port"=>$db_port);
			
			// Command processes sometimes perform tasks such as unzipping and downloading 
			// external resources. These tasks can cause the wait timeouts to be triggered so 
			// here is were these timeouts are set for command processes.
			if(!SERVER_UTIL::is_command_application())
				$options["commands"] = array("set session wait_timeout=10000,interactive_timeout=10000,net_read_timeout=10000");

			$db = DB::get_db_connection($db_host,$db_user,$db_pass,$db_name,$db_type,$options);

			DB::set_default_db_connection($db);
		}

		/**
		 * Returns restricted view 
		 */
		abstract static function get_restricted_view();

		/**
		 * Returns login view 
		 */
		abstract static function get_login_view();

		/**
		 * Returns not found view
		 */
		abstract static function get_not_found_view();	

		/**
		 * has access 
		 * @param array $object_roles
		 */
		static function has_access($object_roles,$task=null) { return false; }

		/**
		 * Checks whether system is in mode debug
		 * @return boolean 
		 */
		static function is_system_mode_debug()			{ return SYSTEM_MANAGER::get_system_mode()==self::SYSTEM_MODE_DEBUG; }

        /**
         * Checks whether system is in mode production
         * @return boolean 
         */
        static function is_system_mode_production()		{ return SYSTEM_MANAGER::get_system_mode()==self::SYSTEM_MODE_PRODUCTION; }

        /**
         * Checks whether system is in mode staging
         * @return boolean 
         */
        static function is_system_mode_staging()		{ return SYSTEM_MANAGER::get_system_mode()==self::SYSTEM_MODE_STAGING; }

        /**
         * Checks whether applicatio has ssl enabled
         * @return boolean 
         */
        static function is_application_ssl_enabled() 		{ return false; }

        /**
         * Checks whether is error debug messages
         * @return boolean 
         */
        static function is_error_debug_messages() 		{ return SYSTEM_MANAGER::is_system_mode_debug(); } 

        /**
         * Checks whether is logged in
         * @return boolean 
         */
        static function is_logged_in() 				{ return USER_SESSION::get_instance()->is_logged_in(); }		

        /**
         * Checks whether is db log
         * @return boolean 
         */
        static function is_db_log() 				{ return self::$_DB_LOG; }
        
        /**
         * Checks whether is controller force ssl
         * @param string $controller
         * @todo review param type
         * @return boolean 
         */
		static function is_controller_force_ssl($controller) 	{ return false; }
        
        /**
         * Checks whether is controller skip ssl
         * @param string $controller
         * @todo review param type
         * @return boolean 
         */
		static function is_controller_skip_ssl($controller) 	{ return false; }		
		
		/**
		 * Set system name
		 * @param string $system_name 
		 */
		static function set_system_name($system_name)		{ self::$_SYSTEM_NAME = $system_name; }

		/**
		 * Set system mode
		 * @param string $system_mode 
		 */
		static function set_system_mode($system_mode)		{ self::$_SYSTEM_MODE = $system_mode; }
		/**
		 * Set system host
		 * @param string $system_host 
		 */
		static function set_system_host($system_host)		{ self::$_SYSTEM_HOST = $system_host; }

		/**
		 * Set system log level
		 * @param string $system_log_level 
		 */
		static function set_system_log_level($system_log_level)	{ self::$_SYSTEM_LOG_LEVEL = $system_log_level; }

		/**
		 * set application directory
		 * @param string $value 
		 */
		static function set_application_directory($value)	{ self::$_SYSTEM_APP_DIR = $value; }
	
		/**
		 * db log
		 */
		static function db_log() 				{ self::$_DB_LOG = true; }

		/**
		 * ssl
		 * @param boolean $ssl 
		 */
		static function ssl($ssl=true)				{ self::$_SSL = $ssl; }

		/**
		 * cron
		 * @param boolean $cron 
		 */
		static function cron($cron=true)			{ self::$_CRON = $cron; }

		/**
		 * disable system host update
		 */
		static function disable_system_host_update()		{ self::$_system_host_update = false; }

		/**
		 * Returns notify recipients
		 * @return array 
		 */
		static function get_notify_recipients()			{ return self::$_NOTIFY_RECIPIENTS; }

		/**
		 * Returns system mode
		 * @return string 
		 */
		static function get_system_mode()			{ return self::$_SYSTEM_MODE; }

		/**
		 * Returns system host
		 * @return string 
		 */
		static function get_system_host() {

			if(self::$_SYSTEM_HOST)
				return self::$_SYSTEM_HOST;

			if(SERVER_UTIL::get_server_host())
				return SERVER_UTIL::get_server_host();

			return MODEL_PATH::get_instance_directory_name();
		}

		static function get_system_id() {

			if($name=self::get_system_name())
				return $name;

			return self::get_system_host();
		}

		/**
		 * Returns system name
		 * @return string 
		 */
		static function get_system_name() 			{ return self::$_SYSTEM_NAME; }

		/**
		 * Returns ssl
		 * @return boolean 
		 */
		static function get_ssl() 				{ return self::$_SSL; }

		/**
		 * Returns cron
		 * @return boolean 
		 */
		static function get_cron() 				{ return self::$_CRON; }

		/**
		 * Returns filesystem user
		 * @return string 
		 */
		static function get_filesystem_user()			{ return "apache"; }

		/**
		 * Returns filesystem group
		 * @return string 
		 */
		static function get_filesystem_group()			{ return "apache"; }

		/**
		 * Returns max execution minutes
		 * @return integer 
		 */
		static function get_max_execution_minutes()             { return 10; }

		/**
		 * Returns max execution minutes
		 * @return integer 
		 */
		static function get_command_max_execution_minutes()         { return 60 * 10; }		

		/**
		 * Returns max input minutes
		 * @return integer 
		 */
		static function get_max_input_minutes()             { return 5; }                

		/**
		 * Returns session timeout minutes
		 * @return integer 
		 */
		static function get_session_timeout_minutes()		{ return 60; }

		/**
		 * Returns session timeout seconds
		 * @return integer 
		 */
		static function get_session_timeout_seconds()		{ return SYSTEM_MANAGER::get_session_timeout_minutes() * 60; }

		/**
		 * Returns timezone
		 * @return string 
		 */
		static function get_timezone()				{ return "America/Toronto"; }
		
		static function get_timezone_offset() {
			$dateTimeZone = new DateTimeZone(SYSTEM_MANAGER::get_timezone());
			return $dateTimeZone->getOffset(new DateTime("now", $dateTimeZone))/3600;
		}

		/**
		 * Returns email util
		 * @return null 
		 */
		static function get_email()					{ return null; }
		static function get_sms()					{ return null; }
		static function get_email_util()			{ return self::get_email(); }

		static function get_storage()				{ return self::storage(); }
		static function storage()					{ return BCMODEL_STORAGE_LOCAL::create(); }

		static function get_notify_email($subject="",$message="") {

			$notify_recipients = SYSTEM_MANAGER::get_notify_recipients();

			if(!$notify_recipients)
				throw new Exception("No notify recipients available");
			
			$email_cmodel = SYSTEM_MANAGER::get_email();
			
			if(!$email_cmodel)
				throw new Exception("No email class available");
				
			$system_name = SYSTEM_MANAGER::get_system_name() ? SYSTEM_MANAGER::get_system_name() : SYSTEM_MANAGER::get_system_host();
				
			$subject = $system_name.": ".$subject;

			return $email_cmodel
						->enable_text()
						->set_to($notify_recipients)
						->set_subject($subject)
						->set_body($message);
		}
						

		/**
		 * Returns system mode name
		 * @return string 
		 */
		static function get_system_mode_name()			{ return get_value(self::get_system_mode_list(),SYSTEM_MANAGER::get_system_mode()); }

		/**
		 * Returns error view
		 * @return object 
		 */
		static function get_error_view()			{ return APPLICATION::get_base_view_instance("application","error"); }	

		/**
		 * Returns logger
		 * @return object
		 * @review return type 
		 */
		static function get_logger()				{ return BASE_CMODEL_LOGGER::get_instance(); }

		/**
		 * Returns secure system url
		 * @param boolean $end_slash
		 * @return boolean 
		 */
		static function get_secure_system_url($end_slash=true) 	{ return "https://".SYSTEM_MANAGER::get_system_host().($end_slash ? "/" : ""); }

		/**
		 * Returns lang
		 * @return string 
		 */
		static function get_lang() 				{ return self::$_LANG; }

		/**
		 * Returns web asset manager
		 * @return mixed
		 * @todo review return type 
		 */
		static function get_web_asset_manager()			{ return APPLICATION::get_web_asset_manager(); }

		/**
		 * requires db connection
		 * @return boolean 
		 */
		static function requires_db_connection()		{ return true; }

		/**
		 * requires db session
		 * @return boolean 
		 */
		static function requires_db_session()			{ return true; }

		/**
		 * requires session
		 * @return boolean 
		 */
		static function requires_session()			{ return true; }

		/**
		 * require session key check
		 * @return boolean 
		 */
		static function require_session_key_check()              { return false; }                

		/**
		 * process task
		 * 
		 * @param string $task 
		 */
		static function process_task($task)			{}

		/**
		 * post initialize
		 */
		static function post_initialize() {

			if(SYSTEM_MANAGER::get_cron() && SYSTEM_MANAGER::is_system_mode_production() && ($notify_recipients=SYSTEM_MANAGER::get_notify_recipients())) {

				$rand = (rand(1,100) % 100);

				if($rand==0) {
					
					$last_cron_pulse = CMODEL_SETTING::get_instance_cron_pulse();

					$is_cron_dead = $last_cron_pulse < (time() - (CONSTANT::SECONDS_HOUR * 5));

					if($is_cron_dead) {

						try {

							$email_cmodel = SYSTEM_MANAGER::get_email();
							
							if($email_cmodel) {
								
								$system_name = SYSTEM_MANAGER::get_system_name();
								$system_host = SYSTEM_MANAGER::get_system_host();
								$system_location = $system_host ? $system_host : MODEL_PATH::get_instance_directory_name();

								$system = ($system_name ? $system_name." (".$system_location.")" : $system_location)." on ".SERVER_UTIL::get_hostname();
								
								foreach($notify_recipients as $notify_recipient)
									$email_cmodel->add_to($notify_recipient);
								
								$subject = "The cron pulse needs attention on ".$system;
								$message = $last_cron_pulse ? "The last cron pulse ran on ".DATE_UTIL::get_long_date_time($last_cron_pulse) : "The cron has never ran for this instance";

								$email_cmodel->enable_text();
								$email_cmodel->set_subject($subject);
								$email_cmodel->set_body($message);
								
								$has_success = $email_cmodel->send($notify_recipients);
							}	

						} catch(Exception $e) {}
					}
				}
			}		
		}

		static function get_index_view() { return null; }

		/**
		 * process failed authorization
		 * @param string $task 
		 */
		static function process_failed_authorization($task)	{}

		/**
		 * process post failed authorization
		 * @param string $task 
		 */
		static function process_post_failed_authorization($task){}

		/**
		 * Returns an array of controller names that allow for controller level js includes
		 */
		static function get_controller_js_names()     { return array(); }

		/**
		 * Returns an array of controller names that allow for controller level css includes
		 */
		static function get_controller_css_names()     { return array(); }                                

		/**
		 * Returns config file
		 * @return string 
		 */
		static function get_config_file() { 
			
			if(is_file($file=MODEL_PATH::get_data_directory()."config.ini"))
             	return $file;

            if(is_file($file=MODEL_PATH::get_instance_directory()."config.ini"))
            	return $file;
            
			return MODEL_PATH::get_data_directory()."config.ini";
		}
				
		/**
         * Returns system url
         * @param boolean $end_slash
         * @param string $protocol
         * @return string 
         */
		static function get_system_url($end_slash=true,$protocol=null) {
			
			$protocol = $protocol ? $protocol : (SERVER_UTIL::is_server_https() ? "https" : "http");
			
			$host = SERVER_UTIL::is_web_application() ? SERVER_UTIL::get_server_host() : SYSTEM_MANAGER::get_system_host();

			return $protocol."://".$host.($end_slash ? "/" : ""); 
		}

		static function get_public_domain()			{ return SERVER_UTIL::get_server_host(); }
		static function get_public_root() 			{ return "/"; }
		static function get_public_path() 			{ return "/public/"; }

		static function get_public_url($path="")	{ return "//".FILE_UTIL::sanitize_path(SYSTEM_MANAGER::get_public_domain()."/".SYSTEM_MANAGER::get_public_root()."/".SYSTEM_MANAGER::get_public_path().$path,"/"); }
		
        /**
         * Returns log level
         * @return string 
         */
		static function get_log_level() {
			
			if(is_numeric($system_log_level=self::$_SYSTEM_LOG_LEVEL))
				return $system_log_level;
			
			return SYSTEM_MANAGER::is_system_mode_production() ? BASE_CMODEL_LOGGER::LEVEL_ERROR : BASE_CMODEL_LOGGER::LEVEL_DEBUG;	
		}
	
        /**
         * initialize log 
         */
		static function initialize_log() {		
			BASE_CMODEL_LOGGER::initialize_log_db(SYSTEM_MANAGER::get_log_level());
		}
			
        /**
         * initialize
         */
		static function initialize() {

			HTML_UTIL::set_defaults(array("button"=>array("class"=>"btn")));
			HTML_TABLE_UTIL::set_defaults("table_class table table-striped table-bordered" ,"","");			

			if(APPLICATION::get_instance()->is_web() && APPLICATION::get_instance()->get_uri()->is_controller("manage")) {
				$registry = REGISTRY::get_instance();
				$registry->set_post_parms(XSS_UTIL::decode($registry->get_post_parms()));
				$registry->set_get_parms(XSS_UTIL::decode($registry->get_get_parms()));
				$registry->set_request_parms(XSS_UTIL::decode($registry->get_request_parms()));
			}
				
			if(APPLICATION::get_instance()->is_web())
					SERVER_UTIL::set_max_execution_minutes(SYSTEM_MANAGER::get_max_execution_minutes());
			else
				SERVER_UTIL::set_max_execution_minutes(SYSTEM_MANAGER::get_command_max_execution_minutes());

            SERVER_UTIL::set_max_input_minutes(SYSTEM_MANAGER::get_max_input_minutes());           
			
			if(!is_dir(MODEL_PATH::get_logs_directory()))
				FILE_UTIL::mkdir(MODEL_PATH::get_logs_directory());

			$ini_util = new INI_UTIL();
			
			try {

				$ini_util->load(SYSTEM_MANAGER::get_config_file());
			
			} catch(Exception $e) {
				
				if($e->getCode()==INI_UTIL::EXCEPTION_FILE_NOT_EXIST) {

					if(!$ini_util->exists("NOTIFY_RECIPIENTS"))
						$ini_util->set("NOTIFY_RECIPIENTS","");
					
					if(!$ini_util->get("SYSTEM_MODE"))
						$ini_util->set("SYSTEM_MODE",self::SYSTEM_MODE_DEBUG);
					
					if(SERVER_UTIL::is_web_application() && SYSTEM_MANAGER::$_system_host_update && !$ini_util->get("SYSTEM_HOST"))
						$ini_util->set("SYSTEM_HOST",SERVER_UTIL::get_server_host());
					
					if($ini_util->get("DIR_APPLICATION")!=DIR_APPLICATION)
						$ini_util->set("DIR_APPLICATION",DIR_APPLICATION);
						
					if(!$ini_util->exists("SYSTEM_LOG_LEVEL"))
						$ini_util->set("SYSTEM_LOG_LEVEL","");

					if(!$ini_util->exists("DB_USER"))
						$ini_util->set("DB_USER","");

					if(!$ini_util->exists("DB_PASS"))
						$ini_util->set("DB_PASS","");

					if(!$ini_util->exists("DB_HOST"))
						$ini_util->set("DB_HOST","");
														
					if(!$ini_util->exists("DB_NAME"))
						$ini_util->set("DB_NAME","");

					$ini_util->save();
				}		
			}

			if($ini_util->get("DB_LOG"))
				SYSTEM_MANAGER::db_log();
			
			if($ini_util->exists("SSL"))
				SYSTEM_MANAGER::ssl(filter_var($ini_util->get("SSL"),FILTER_VALIDATE_BOOLEAN));
			
			if($ini_util->exists("CRON"))
				SYSTEM_MANAGER::cron(filter_var($ini_util->get("CRON"),FILTER_VALIDATE_BOOLEAN));
								
			SYSTEM_MANAGER::set_system_mode($ini_util->get("SYSTEM_MODE",self::SYSTEM_MODE_DEBUG));
			SYSTEM_MANAGER::set_system_name($ini_util->get("SYSTEM_NAME"));
			SYSTEM_MANAGER::set_system_log_level($ini_util->get("SYSTEM_LOG_LEVEL"));
			SYSTEM_MANAGER::set_notify_recipients($ini_util->get("NOTIFY_RECIPIENTS"));						
			SYSTEM_MANAGER::set_application_directory($ini_util->get("DIR_APPLICATION"));
			SYSTEM_MANAGER::set_system_host($ini_util->get("SYSTEM_HOST"));			
			SYSTEM_MANAGER::initialize_log();
		}
		
        /**
         * set notify recipients
         * @param string $notify_recipient_string 
         */
		static function set_notify_recipients($notify_recipient_string) {
			self::$_NOTIFY_RECIPIENTS = self::parse_notify_recipients($notify_recipient_string); 
		}

        /**
         * parse notify recipients
         * @param string $notify_recipient_string
         * @return boolean 
         */
		static function parse_notify_recipients($notify_recipient_string) { return EMAIL_UTIL::parse_email($notify_recipient_string); }
	}