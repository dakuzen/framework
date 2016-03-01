<?php
    /**
    * Class BASE_APPLICATION
    *
    * @package Library
    * @subpackage Common
    */

	abstract class BASE_APPLICATION {

		const MIN_PHP_VERSION		= "5.3";

        /**
         * The instance
         * @var object $instance
         */
		protected static $_instance	= null;

        /**
         * The registry
         * @var string $registry
         */
		protected $_registry		= null;

        /**
         * Has extend
         * @var boolean $has_extend
         */
		protected $_has_extend		= false;

        /**
         * The guid
         * @var string $guid
         */
		protected $_guid			= "";

        /**
         * The timestamps
         * @var array $timestamps
         */
		protected $_timestamps		= array();

        /**
         * Returns the instance
         */
		static abstract function get_instance();

        /**
         * Constructor
         */
		public function __construct() {
			$this->add_timestamp("first");
		}

		public function is_web()		{ return is_a($this,"WEB_APPLICATION"); }
		public function is_command()	{ return is_a($this,"COMMAND_APPLICATION"); }

        /**
         * Returns the guid
         * @return string
         */
		public function get_guid() 		{ return $this->_guid; }

        /**
         * Returns the registry
         * @return string
         */
		public function get_registry() 		{ return $this->_registry; }

        /**
         * Returns timestamps
         * @return array
         */
		public function get_timestamps() 	{ return $this->_timestamps; }

        /**
         * Adds timestamp
         * @param string $name
         */
		public function add_timestamp($name)	{ $this->_timestamps[$name] = microtime(true); }

        /**
         * Checks whether has extend
         * @return boolean
         */
		public function has_extend() 		{ return $this->_has_extend; }

        /**
         * Print timestamps
         */
		public function print_timestamps() 	{ p(self::get_timestamp_ages()); }

        /**
         * Returns timestamp ages
         * @param boolean $accumulated
         * @return array
         */
		public function get_timestamp_ages($accumulated=true) {

			$timestamps = $this->_timestamps;

			$first = get_value($this->_timestamps,"first");

			foreach($timestamps as $name=>$time) {

				if($accumulated)
					$timestamps[$name] = $time - $first;
				else
					$timestamps[$name] = $time;
			}

			return $timestamps;
		}

		public function validate_server() {

			if(version_compare(phpversion(),self::MIN_PHP_VERSION)<=0) {
				echo "\nThe current version of PHP is ".phpversion().". The framework requires at minimum PHP version ".self::MIN_PHP_VERSION." or higher.\n\n";
				die(1003);
			}

			if(!ini_get("short_open_tag")) {
				echo "\nPHP's short open tags are not enabled. The framework requires this to be enabled.\n\n";
				die(1004);
			}
		}

        /**
         * Initialize
         */
		protected function initialize() {

			spl_autoload_register(array($this, "autoload"));
			$this->_registry = REGISTRY::get_instance();
			$this->_has_extend = is_dir(self::get_extend_directory());

			date_default_timezone_set(SYSTEM_MANAGER::get_timezone());
			ini_set("date.timezone",SYSTEM_MANAGER::get_timezone());

			$this->_guid = MISC_UTIL::get_guid(12);
		}


		protected function initialize_error_handling() {

			DEBUG_UTIL::enable_system_error_handler();
			
			if(SYSTEM_MANAGER::is_system_mode_production()) {
				DEBUG_UTIL::output(false);
				if($logger_cmodel=SYSTEM_MANAGER::get_logger())
					$logger_cmodel->notify();
			}

			DEBUG_UTIL::set_fatal_error_handler(function() {

				if($error=error_get_last()) {

				 	$error = DEBUG_UTIL::error(value($error,"type"),value($error,"message"),value($error,"file"),value($error,"line"));

				 	if(DEBUG_UTIL::is_reportable(value($error,"type"),value($error,"message"),array(E_ERROR,E_USER_ERROR,E_COMPILE_ERROR))) {

				 		if(SYSTEM_MANAGER::is_system_mode_production()) {
							DEBUG_UTIL::notify_error($error);
							$this->render_fatal_error($error);
						} else 
							echo DEBUG_UTIL::debug("Fatal Error: ".value($error,"message")."\n".value($error,"file")." @ ".value($error,"line"));
					}							
				}

				return true;
			});
		}

        /**
         * Initialize database
         */
		protected function initialize_database() {

			$this->add_timestamp("start_initialize_database");

			try {
				SYSTEM_MANAGER::create_default_db_connection();

				$db = DB::get_instance();

				if(SYSTEM_MANAGER::is_db_log())
					$db->log(MODEL_PATH::get_logs_directory()."db/");

			} catch(Exception $e) {

				if(SYSTEM_MANAGER::requires_db_connection()) {
					$error_message = SYSTEM_MANAGER::is_system_mode_production() ? "" : ": ".$e->getMessage();
					self::display_low_level_error_message("Unable to connect to database".$error_message,true);
				}
			}

			if(SYSTEM_MANAGER::requires_db_connection() && !DB::get_instance())
				self::display_low_level_error_message("Unable to connect to database");

			$this->add_timestamp("end_initialize_database");
		}

		public function __destruct() {
			$this->deinitialize_database();
		}

		protected function deinitialize_database() {
			if(SYSTEM_MANAGER::requires_db_connection())
				if($instance=DB::get_instance())
					$instance->close();
		}


        /**
         * Returns instance directory
         * @return string
         */
		static function get_instance_directory() 		{ return DIR_INSTANCE; }

        /**
         * Returns main application directory
         * @return string
         */
		static function get_main_application_directory() 	{ return DIR_INSTANCE."backend/"; }

        /**
         * Returns application directory
         * @return string
         */
        static function get_application_directory() 		{ return DIR_APPLICATION; }

        /**
         * Returns framework directory
         * @return string
         */
		static function get_framework_directory() 		{ return DIR_FRAMEWORK; }

        /**
         * Returns framework base directory
         * @return string
         */
		static function get_framework_base_directory() 		{ return self::get_framework_directory(); }

        /**
         * Returns extended directory
         * @return string
         */
		static function get_extend_directory() 			{ return self::get_instance_directory()."extend/"; }

		/**
         * Display low level error message
         * @param string $msg
         * @param boolean $die
         */
		static function display_low_level_error_message($msg,$die=true) {

			if(SERVER_UTIL::is_command_application())
				echo "[ERROR] ".$msg;
			else
				echo '<div style="font-family: Verdana;color: #FF0000;font-size: 12px;"><strong>System Error Message: '.$msg.'<br>Timestamp: ['.date("F jS, Y g:i a").']</strong></div>';
			if($die)
				die(1001);
		}

        /**
         * display low level notify message
         * @param string $msg
         * @param boolean $die
         */
		static function display_low_level_notify_message($msg,$die=true) {
			if(SERVER_UTIL::is_command_application())
				echo "[NOTIFY] ".$msg;
			else
				echo '<div style="font-family: Verdana;color: green;font-size: 12px;"><strong>System Notify Message: '.$msg.'<br>Timestamp: ['.date("F jS, Y g:i a").']</strong></div>';
			if($die)
				die(1002);
		}

        /**
         * include base dbo
         * @param string $class_name
         * @return mixed
         */
		static function include_base_dbo($class_name) 	{ return self::include_dbo($class_name,self::get_framework_base_directory()); }

		/**
		 * include base dbq
		 * @param string $class_name
		 * @return mixed
		 */
		static function include_base_dbq($class_name) 	{ return self::include_dbq($class_name,self::get_framework_base_directory()); }

        /**
         * include dbo
         * @param string $class_name
         * @param string $base_dir
         * @return mixed
         */
		static function include_dbo($class_name,$base_dir=null,$options=array("trigger"=>true,"framework"=>true)) {
			require_once(DIR_FRAMEWORK."libraries/db/DBO.inc");

			$basename = preg_replace("/^[a-z0-9_]*DBO_/i","",strtolower($class_name));

			$filename = str_replace("dbo_","",$basename)."_dbo.inc";

			$base_dir = $base_dir ? $base_dir : self::get_application_directory();

			$success = self::include_once_debug($base_dir."db/dbo/".$filename);
			
			if(!$success) {

				if(value($options,"framework",true) && preg_match("/^DBO/",$class_name)) {

					if(is_file(self::get_framework_directory()."db/dbo/".$filename)) {

						$success = class_alias("BASE_".$class_name,$class_name,true);
					}	
				}
			}

			if(value($options,"trigger",true) && !$success)
				trigger_error("Failed to load ".$class_name);

			return $success;
		}

		/**
		 * include dbq
		 * @param string $class_name
		 * @param string $base_dir
		 * @return mixed
		 */
		static function include_dbq($class_name,$base_dir=null,$options=array("trigger"=>true,"framework"=>true)) {

			$basename = preg_replace("/^[a-z0-9_]*DBQ_/i","",strtolower($class_name));

			$dbq_filename = "db/dbq/".str_replace("dbq_","",$basename)."_dbq.inc";

			$base_dir = $base_dir ? $base_dir : self::get_application_directory();

			$success = self::include_once_debug($base_dir.$dbq_filename);
	
			if(!$success) {

				if(value($options,"framework") && preg_match("/^DBQ/",$class_name)) {

					if(is_file(self::get_framework_directory().$dbq_filename)) {

						$success = class_alias("BASE_".$class_name,$class_name,true);
					}	
				}
			}

			if(value($options,"trigger") && !$success)
				trigger_error("Failed to load ".$class_name);

			return $success;
		}

		/**
		 * include complex model
		 * @param string $class_name
		 * @param string $base_dir
		 * @return boolean
		 */
		static function include_complex_model($class_name,$base_dir=null) {
			return self::include_model($class_name,"complex",$base_dir);
		}

		/**
		 * Include handler model
		 * @param string $class_name
		 * @param string $base_dir
		 * @return boolean
		 */
		static function include_handler_model($class_name,$base_dir=null) {
			return self::include_model($class_name,"handler",$base_dir);
		}

        /**
         * include simple model
         * @param string $class_name
         * @param string $base_dir
         * @return boolean
         */
		static function include_simple_model($class_name,$base_dir=null) {
			return self::include_model($class_name,"simple",$base_dir);
		}

        /**
         * include model
         * @param string $class_name
         * @param string $type
         * @param string $base_dir
         * @return boolean
         */
		static function include_model($class_name,$model_type,$base_dir=null,$options=array("trigger"=>true,"framework"=>true)) {

			$base_dir = $base_dir ? $base_dir : self::get_application_directory();

			$model_file = self::get_model_file($class_name,$model_type,$base_dir);

			$has_success = self::include_once_debug($model_file);
			
			if($has_success) {

				$match = preg_match("/^(EXTEND_|BASE_|B)?((CMODEL|HMODEL)_(.*))/",$class_name,$matches);

				if($match) {

					$prefix = value($matches,1);
					$model 	= value($matches,2);

					if(preg_match("/^B/",$prefix)) {

						if(class_exists("BASE_".$model) && !class_exists("B".$model)) 
							class_alias("BASE_".$model,"B".$model,false);

						elseif(class_exists("B".$model) && !class_exists("BASE_".$model)) 
							class_alias("B".$model,"BASE_".$model,false);
					}
				}
			
			} elseif(value($options,"framework",true)) {

				if(preg_match("/^(CMODEL|HMODEL|MODEL)/",$class_name)) {

					$model_file = self::get_model_file($class_name,$model_type,self::get_framework_directory());

					if(is_file($model_file)) {
						
						$base_class_name = "BASE_".$class_name;
						$has_success = class_alias($base_class_name,$class_name,true);
					}	
				}
			}

			if(value($options,"trigger",true) && !$has_success)
				trigger_error("Failed to load model ".$class_name);

			return $has_success;
		}

		/**
		 * Returns model file
		 * @param string $class_name
		 * @param string $type
		 * @param string $base_dir
		 * @return string
		 */
		static function get_model_file($class_name,$type,$base_dir="") {

			$base_dir = $base_dir ? $base_dir : self::get_application_directory();

			$regex = $type=="simple" ? "/^(?:base_|extend_)?model_(.*)/" : "/^(?:base_|extend_|b)?(?:cmodel_|hmodel_)(.*)/";

			preg_match($regex,strtolower($class_name),$matches);

			$basename = get_value($matches,1);

			$filename = $basename."_".$type."_model.inc";

			return $base_dir."models/".$type."/".$filename;
		}

		/**
		 * Include extend complex model
		 * @param string $class_name
		 * @return boolean
		 */
		static function include_extend_complex_model($class_name) {
			return self::include_complex_model($class_name,self::get_extend_directory());
		}

		/**
		 * Include extend handler model
		 * @param string $class_name
		 * @return boolean
		 */
		static function include_extend_handler_model($class_name) {
			return self::include_handler_model($class_name,self::get_extend_directory());
		}

		/**
		 * Include extend simple model
		 * @param string $class_name
		 * @return boolean
		 */
		static function include_extend_simple_model($class_name) {
			return self::include_simple_model($class_name,self::get_extend_directory());
		}

		/**
		 * Include base complex model
		 * @param string $class_name
		 * @return boolean
		 */
		static function include_base_complex_model($class_name) {
			return self::include_complex_model($class_name,self::get_framework_base_directory());
		}

		/**
		 * Include base handler model
		 * @param string $class_name
		 * @return boolean
		 */
		static function include_base_handler_model($class_name) {
			return self::include_handler_model($class_name,self::get_framework_base_directory());
		}

		/**
		 * Include base simple model
		 * @param string $class_name
		 * @return boolean
		 */
		static function include_base_simple_model($class_name) {
			return self::include_simple_model($class_name,self::get_framework_base_directory());
		}

		/**
		 * Include base template
		 * The scope of this function has to be public and non-static
		 *
		 * @param string $controller
		 * @param string $view
		 * @param array $vars
		 * @param boolean $supress_error
		 * @return object
		 */
		public function include_base_template($controller,$view,$vars=array(),$supress_error=false) {
			return VIEW::include_template($controller,$view,$vars,self::get_framework_base_directory(),$supress_error);
		}

		/**
		 * Include the template
		 * The scope of this function has to be public and non-static
		 *
		 * @param string $controller
		 * @param string $view
		 * @param array $vars
		 * @param string $base_dir
		 * @param boolean $supress_error
		 * @return object
		 */
		public function include_template($controller,$view,$vars=array(),$base_dir="",$supress_error=false) {
			return VIEW::include_template($controller,$view,$vars,$base_dir,$supress_error);
		}

		/**
		 * Returns base compled model instance
		 * @param string $class_name
		 * @return object|null
		 */
		static function get_base_complex_model_instance($class_name) {

			if(self::include_base_complex_model($class_name)) {
				$model_class_name = "BCMODEL_".$class_name;
				return new $model_class_name();
			}

			return null;
		}

		/**
		 * Include base model
		 * @param string $class_name
		 * @param string $type
		 * @return mixed
		 * @todo review return type
		 */
		static function include_base_model($class_name,$type) {

			$model_file = self::get_framework_base_directory()."models/".$type."/base_".strtolower($class_name)."_".$type."_model.inc";
			return self::include_once_debug($model_file);
		}

		/**
		 * Include util
		 * @param string $util
		 * @return mixed
		 * @todo review return type
		 */
		static function include_util($util) {
			$util_file = DIR_FRAMEWORK."utilities/".$util.".inc";
			return self::include_once_debug($util_file);
		}

		/**
		 * Inlude htmlform
		 * @param string $classname
		 * @return mixed
		 * @todo review return type
		 */
		static function include_htmlform($classname) {
			require_once(APPLICATION::get_framework_directory()."libraries/htmlform/htmlform.inc");

			$filename = str_replace("htmlform_","",strtolower($classname))."_htmlform.inc";

			$htmlform_file = self::get_application_directory()."forms/".$filename;

			return self::include_once_debug($htmlform_file);
		}

		/**
		 * Include session
		 * @param string $session_name
		 * @return mixed
		 * @todo review return type
		 */
		static function include_session($session_name) {

			$session_name = strtolower($session_name);

			$session_file = self::get_application_directory()."system/session/".$session_name.".inc";

			return self::include_once_debug($session_file);
		}

        /**
         * include constant
         * @param string $name
         * @return mixed
         */
		static function include_constant($name) {

			$constant_file = self::get_application_directory()."constants/".$name.".inc";
			return self::include_once_debug($constant_file);
		}

        /**
         * include once debug
         * @param string $file
         * @return mixed
         * @todo review return type
         */
		static function include_once_debug($file) {

			if(!file_exists($file))
				return false;
			
			//if(SYSTEM_MANAGER::is_system_mode_production())
			return include_once($file);

			
			//	return include_once($file);

			//return false;
		}

        /**
         * initialize framework
         */
		static function initialize_framework() {

			require_once(DIR_FRAMEWORK."utilities/MISC_UTIL.inc");
			require_once(DIR_FRAMEWORK."utilities/SERVER_UTIL.inc");
			require_once(DIR_FRAMEWORK."utilities/DEBUG_UTIL.inc");

			require_once(DIR_FRAMEWORK."libraries/framework/base_system_manager.inc");
			require_once(DIR_FRAMEWORK."libraries/framework/constant.inc");
			require_once(DIR_FRAMEWORK."libraries/framework/object.inc");
			require_once(DIR_FRAMEWORK."libraries/framework/registry.inc");
			require_once(DIR_FRAMEWORK."libraries/framework/message.inc");
			require_once(DIR_FRAMEWORK."libraries/framework/message_handler.inc");
			require_once(DIR_FRAMEWORK."libraries/framework/route.inc");
			require_once(DIR_FRAMEWORK."libraries/framework/base_task.inc");
			require_once(DIR_FRAMEWORK."libraries/framework/base_view.inc");
			require_once(DIR_FRAMEWORK."libraries/framework/base_action.inc");			
			require_once(DIR_FRAMEWORK."libraries/db/DBQ.inc");

			require_once(self::get_application_directory()."system/managers/system_manager.inc");
			require_once(self::get_application_directory()."system/managers/security_manager.inc");

			require_once(DIR_FRAMEWORK."libraries/framework/base_session_manager.inc");
			require_once(DIR_FRAMEWORK."libraries/framework/session.inc");
			require_once(DIR_FRAMEWORK."libraries/framework/uri.inc");

			require_once(DIR_FRAMEWORK."libraries/framework/model.inc");
			require_once(DIR_FRAMEWORK."libraries/framework/model_complex.inc");
			require_once(DIR_FRAMEWORK."libraries/framework/model_handler.inc");

			if(!@include_once(self::get_application_directory()."system/framework/view.inc"))
				MISC_UTIL::class_alias("BASE_VIEW","VIEW");

			if(!@include_once(self::get_application_directory()."system/framework/action.inc")) 
				MISC_UTIL::class_alias("BASE_ACTION","ACTION");

			require_once(DIR_FRAMEWORK."libraries/framework/base_action_json.inc");
			if(!@include_once(self::get_application_directory()."system/framework/action_json.inc")) 
				MISC_UTIL::class_alias("BASE_ACTION_JSON","ACTION_JSON");
		}

        /**
         * initialize web application
         */
		static function initialize_web_application() {

			self::initialize_framework();

			require_once(DIR_FRAMEWORK."libraries/framework/web_application.inc");
			require_once(self::get_application_directory()."system/managers/session_manager.inc");

			if(is_file($inc=self::get_application_directory()."system/framework/web_application.inc"))
				include_once($inc);
			else
				MISC_UTIL::class_alias("WEB_APPLICATION","APPLICATION");
		}

        /**
         * initialize command application
         */
		static function initialize_command_application() {
			self::initialize_framework();

			require_once(DIR_FRAMEWORK."libraries/framework/command_application.inc");

			if(is_file($application_inc=self::get_application_directory()."system/command_application.inc"))
				include_once($application_inc);
			else
				MISC_UTIL::class_alias("COMMAND_APPLICATION","APPLICATION");
		}

		/**
		 * Adds error
		 * @param string $message
		 */
		static function add_error($message) 		{ self::add_error_message($message); }

		/**
		 * Adds notify
		 * @param string $message
		 */
		static function add_notify($message)		{ self::add_notify_message($message); }

		/**
		 * Adds warning
		 * @param string $message
		 */
		static function add_warning($message) 		{ self::add_warning_message($message); }

		/**
		 * Adds errors
		 * @param array $messages
		 */
		static function add_errors($messages) 		{ self::add_error_messages($messages); }

		/**
		 * Adds notifies
		 * @param array $messages
		 */
		static function add_notifies($messages)		{ self::add_notify_messages($messages); }

		/**
		 * Adds warnings
		 * @param array $messages
		 */
		static function add_warningings($messages) 	{ self::add_warning_messages($messages); }

		/**
		 * Adds error messages
		 * @param array $messages
		 */
		static function add_error_messages($messages) {
			foreach($messages as $message)
				self::add_error_message($message);
		}

		/**
		 * Adds error message
		 * @param string $message
		 * @return boolean
		 */
		static function add_error_message($message) {
			return REGISTRY::get_instance()->get_message_handler()->add_error_message($message);
		}

		/**
		 * Returns error messages
		 * @return array
		 */
		static function get_error_messages() {
			return REGISTRY::get_instance()->get_message_handler()->get_error_messages();
		}

                /**
                 * Checks whether it has  error messages
                 * @return boolean
                 */
		static function has_error_messages() {
			return REGISTRY::get_instance()->get_message_handler()->has_error_messages();
		}

		/**
		 * Adds notify messages
		 * @param array $messages
		 */
		static function add_notify_messages($messages) {
			foreach($messages as $message)
				REGISTRY::get_instance()->get_message_handler()->add_notify_message($message);
		}

		/**
		 * Adds notify message
		 * @param string $message
		 * @return boolean
		 */
		static function add_notify_message($message) {
			return REGISTRY::get_instance()->get_message_handler()->add_notify_message($message);
		}

		/**
		 * Returns notify messages
		 * @return array
		 */
		static function get_notify_messages() {
			return REGISTRY::get_instance()->get_message_handler()->get_notify_messages();
		}

		/**
		 * Checks whether it has notify messages
		 * @return boolean
		 */
		static function has_notify_messages() {
			return REGISTRY::get_instance()->get_message_handler()->has_notify_messages();
		}

		/**
		 * Adds warning messages
		 * @param array $messages
		 */
		static function add_warning_messages($messages) {
			foreach($messages as $message)
				REGISTRY::get_instance()->get_message_handler()->add_warning_message($message);
		}

		/**
		 * Adds warning message
		 * @param string $message
		 * @return boolean
		 */
		static function add_warning_message($message) {
			return REGISTRY::get_instance()->get_message_handler()->add_warning_message($message);
		}

		/**
		 * Returns warning messages
		 * @return array
		 */
		static function get_warning_messages() {
			return REGISTRY::get_instance()->get_message_handler()->get_warning_messages();
		}

		/**
		 * Checks whether it has warning messages
		 * @return boolean
		 */
		static function has_warning_messages() {
			return REGISTRY::get_instance()->get_message_handler()->has_warning_messages();
		}

		/**
		 * Adss debug messages
		 * @param array $messages
		 */
		static function add_debug_messages($messages) {
			foreach($messages as $message)
				self::add_debug_message($message);
		}

		/**
		 * Add debug message
		 * @param string $message
		 * @param boolean $identify
		 * @return boolean
		 */
		static function add_debug_message($message,$identify=true) {
			return REGISTRY::get_instance()->get_message_handler()->add_debug_message("Debug: ".$message);
		}

        /**
         * Return debug messages
         * @return array
         */
		static function get_debug_messages() {
			return REGISTRY::get_instance()->get_message_handler()->get_debug_messages();
		}

        /**
         * Checks whether it has debug messages
         * @return boolean
         */
		static function has_debug_messages() {
			return REGISTRY::get_instance()->get_message_handler()->has_debug_messages();
		}

        /**
         * inherit messages
         * @param MESSAGE $obj
         * @return mixed
         * @review return type
         */
		static function inherit_messages(MESSAGE $obj) {
			return REGISTRY::get_instance()->get_message_handler()->inherit_messages($obj);
		}

        /**
         * inherit user messages
         * @param MESSAGE $obj
         * @return boolean
         * @todo review return type
         */
		static function inherit_user_messages(MESSAGE $obj) {
			return REGISTRY::get_instance()->get_message_handler()->inherit_user_messages($obj);
		}

        /**
         * add messages
         * @param array $messages
         * @return boolean
         * @todo review return type
         */
		static function add_messages($messages) {
			return REGISTRY::get_instance()->get_message_handler()->add_messages($messages);
		}

        /**
         * Returns the view instance
         * @param string $controller
         * @param string $view
         * @param string $base_dir
         * @param string $class_prefix
         * @param string $args
         * @return object view_class_name
         */
		static function get_view($controller,$view,$base_dir="",$class_prefix="",$args=null) {

			$class_prefix = $class_prefix ? $class_prefix : "VIEW_";

			$base_dir = $base_dir ? $base_dir : self::get_application_directory();

			$has_success = self::include_view($controller,$view,$base_dir);

			$view_object = null;

			if($has_success) {

				$view 				= str_replace(array(".","-"),array("_","_"),$view);
				$view_class_name	= $class_prefix.strtoupper($controller)."_".strtoupper($view);

				if(APPLICATION::get_instance()->has_extend()) {

					$has_success = self::include_view($controller,$view,self::get_extend_directory());

					if($has_success)
						$view_class_name = "EXTEND_".$view_class_name;
				}

				if(!SYSTEM_MANAGER::is_system_mode_production() && !class_exists($view_class_name))
					DEBUG_UTIL::print_backtrace();

				$view_object = new $view_class_name($args);
				$view_object->set_base_directory($base_dir);

				if(!$view_object->get_route())
					$view_object->set_route($controller,$view);
			}

			return $view_object;
		}

		static function get_view_instance($controller,$view,$base_dir="",$class_prefix="",$args=null) {
			return self::get_view($controller,$view,$base_dir,$class_prefix,$args);
		}

		/**
		 * Returns the base view instance
		 * @param string $controller
		 * @param string $view
		 * @return object
		 */
		static function get_base_view_instance($controller,$view) {
			return self::get_view_instance($controller,$view,self::get_framework_base_directory(),"BASE_VIEW_");
		}

        /**
         * Include view
         * @param string $controller
         * @param string $view
         * @param string $base_dir
         * @return boolean
         */
		static function include_view($controller,$view,$base_dir=null) {

			$base_dir = $base_dir ? $base_dir : self::get_application_directory();

			$view_file = $base_dir."views/".strtolower($controller)."/".strtolower($view)."_view.inc";

			if(is_file($view_file))
				return self::include_once_debug($view_file);

			return false;
		}

        /**
         * Return action inctance
         * @param string $controller
         * @param string $action
         * @param string $base_dir
         * @param string $class_prefix
         * @return action_class_name|null
         */
        static function get_action_instance($controller,$action,$base_dir="",$class_prefix="ACTION_") {
        	return self::get_action($controller,$action,$base_dir,$class_prefix);
        }

		static function get_action($controller,$action,$base_dir="",$class_prefix="ACTION_") {

			$has_success = self::include_action($controller,$action,$base_dir);

			if($has_success) {
				$action = str_replace("-","_",$action);

				$action_class_name = $class_prefix.strtoupper($controller)."_".strtoupper($action);

				if(APPLICATION::get_instance()->has_extend()) {

					$has_success = self::include_action($controller,$action,self::get_extend_directory());

					if($has_success)
						$action_class_name = "EXTEND_".$action_class_name;
				}

				$action_object = new $action_class_name();
				$action_object->set_route($controller,$action);
				return $action_object;
			}

			return null;
		}

        /**
         * include_base_action
         * @param string $controller
         * @param string $action
         * @return boolean
         */
		static function include_base_action($controller,$action) {
			return self::include_action($controller,$action,self::get_framework_base_directory());
		}

        /**
         * include base view
         * @param string $controller
         * @param string $action
         * @return boolean
         */
		static function include_base_view($controller,$action) {
			return self::include_view($controller,$action,self::get_framework_base_directory());
		}

        /**
         * include action
         * @param string $controller
         * @param string $action
         * @param string $base_dir
         * @return boolean
         */
		static function include_action($controller,$action,$base_dir="") {

			$base_dir = $base_dir ? $base_dir : self::get_application_directory();

			$action_file = $base_dir."actions/".strtolower($controller)."/".strtolower($action)."_action.inc";

			$has_success = false;

			if(is_file($action_file))
				$has_success = self::include_once_debug($action_file);

			return $has_success;
		}

        /**
         * Returns the base action instance
         * @param string $controller
         * @param string $action
         * @return mixed
         */
		static function get_base_action_instance($controller,$action) {
			return self::get_action_instance($controller,$action,self::get_framework_base_directory(),"BASE_ACTION_");
		}

        /**
         * Autoload
         * @param string $class_name
         * @return boolean
         */
		function autoload($class_name) {

			if(preg_match("/^DBQ_/",$class_name))
				return $this->include_dbq($class_name);

			if(preg_match("/^DBO_/",$class_name))
				return $this->include_dbo($class_name);

			if(preg_match("/^(BASE_|EXTEND_|B)?(CMODEL)/",$class_name,$matches)) {

				if(preg_match("/^B/",$matches[1]))
					return self::include_base_complex_model($class_name);

				elseif($matches[1]=="EXTEND_")
					return self::include_extend_complex_model($class_name);

				return $this->include_complex_model($class_name);
			}

			if(preg_match("/^(BASE_|EXTEND_|B)?(HMODEL)/",$class_name,$matches)) {

				if(preg_match("/^B/",$matches[1]))
					return self::include_base_handler_model($class_name);

				elseif($matches[1]=="EXTEND_")
					return self::include_extend_handler_model($class_name);

				return $this->include_handler_model($class_name);
			}

			if(preg_match("/^MODEL_/",$class_name))
				return $this->include_simple_model($class_name);

			if(preg_match("/_UTIL$/",$class_name))
				return $this->include_util($class_name);

			if(preg_match("/^(BASE_|B)MODEL_/",$class_name))
				return $this->include_base_simple_model($class_name);

			if(preg_match("/^BASE_DBO_/",$class_name))
				return $this->include_base_dbo($class_name);

			if(preg_match("/^BASE_DBQ_/",$class_name))
				return $this->include_base_dbq($class_name);

			if($this->has_extend() && preg_match("/^EXTEND_(.*)/",$class_name)) {
				if(preg_match("/^EXTEND_MODEL_/",$class_name))
					return $this->include_extend_simple_model($class_name);
			}

			if(preg_match("/_SESSION$/",$class_name))
				return $this->include_session($class_name);

			elseif(preg_match("/^VIEW_([^_]+)_(.*)/",$class_name,$match))
				return self::include_view(get_value($match,1),get_value($match,2));

			elseif(preg_match("/^ACTION_([^_]+)_(.*)/",$class_name,$match))
				return self::include_action(get_value($match,1),get_value($match,2));

			elseif(preg_match("/^BASE_VIEW_([^_]+)_(.*)/",$class_name,$match))
				return self::include_view(get_value($match,1),get_value($match,2),self::get_framework_base_directory());

			elseif(preg_match("/^BASE_ACTION_([^_]+)_(.*)/",$class_name,$match))
				return self::include_action(get_value($match,1),get_value($match,2),self::get_framework_base_directory());

			if(preg_match("/HTMLFORM_/",$class_name))
				return $this->include_htmlform($class_name);

			return false;
		}
	}