<?
    /**
    * Class BASE_CMODEL_UPGRADE
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_UPGRADE extends CMODEL {


        /**
         * instance directory
         * @var object
         */
		protected $_instance_directory 	= "";

        /**
         * The db
         * @var object
         */
		protected $_db			= null;

        /**
         * The callback handler
         * @var object
         */
		protected $_callback_handler	= null;

        /**
         * Constructor
         * @param string $instance_directory
         * @param object $db
         */
		public function __construct($instance_directory=DIR_INSTANCE,$db=null) {

			if($instance_directory)
				$this->_instance_directory = $instance_directory;

			$this->_db = $db;

			$this->_callback_handler = array($this,"process_message_callback");
		}

        /**
         * Returns the upgrade directory
         * @return string
         */
		public function get_upgrade_directory()	{
			
			$dir = $this->_instance_directory."backend/upgrade/";

			if(is_dir($dir))
				return $dir;

			//Legacy
			$dir = $this->_instance_directory."application/upgrade/";

			if(is_dir($dir))
				return $dir;

			throw new Exception("Failed to locate upgrade directory in ".$this->_instance_directory);
		}

        /**
         * Returns the upgrade script directory
         * @return string
         */
        public function get_upgrade_script_directory()	{ return $this->get_upgrade_directory()."scripts/"; }

        /**
         * Returns the current version
         * @return string
         */
        public function get_current_version()		{ return BASE_MODEL_UPGRADE::get_current_version($this->get_upgrade_directory()."version"); }

        /**
         * process
         * @param string $version
         * @return boolean
         */
		public function process($version) {

			$version = self::sanitize_version($version);

			SERVER_UTIL::set_max_execution_minutes(60 * 10);
			SERVER_UTIL::set_memory_limit(500);
			DEBUG_UTIL::enable_system_error_handler();

			if(!self::is_valid_version($version)) {
				call_user_func_array($this->_callback_handler,array("Invalid version number",false));
				return false;
			}

			$this->init_upgrade_framework();

			$has_success = true;

			$upgrade_functions = $this->get_upgrade_functions($version);

			$upgrade = $this->create_upgrade($version);

			$has_upgrade_function = false;

			try {

				foreach($upgrade_functions as $upgrade_function) {

					if(!$this->function_exists($upgrade_function,$version)) {

						$has_upgrade_function = true;

						$has_success = $upgrade->$upgrade_function();

						if(!$has_success)
							throw new Exception("The upgrade function ".$upgrade_function." failed");

						 $this->create_upgrade_item_dbo($upgrade_function,$version)->insert();

						call_user_func_array($this->_callback_handler,array("Successfully ran `".$upgrade_function."`",true));
					}
				}

			} catch(Exception $e) {
				call_user_func_array($this->_callback_handler,array("Failed Upgrade: ".$upgrade_function." - ".$e->getMessage(),false));
				$has_success = false;

				throw $e;
			}

			$this->process_framework_upgrades();

			if($has_success)
				CMODEL_SETTING::set_instance_upgraded(time());

			foreach($upgrade->get_error_messages() as $message)
				call_user_func_array($this->_callback_handler,array($message,false));

			foreach($upgrade->get_notify_messages() as $message)
				call_user_func_array($this->_callback_handler,array($message,true));

			if(!$has_upgrade_function)
				call_user_func_array($this->_callback_handler,array("There are no upgrade functions to run for version ".str_replace("_",".",$version),true));

			return $has_success;
		}

		public function set_callback_handler($callback_handler) { $this->_callback_handler = $callback_handler; }

        /**
         * process message callback
         * @param string $message
         * @param boolean $has_success
         */
		public function process_message_callback($message,$has_success) {

			if($has_success)
				$this->add_notify_message($message);
			else
				$this->add_error_message($message);
		}

        /**
         * mark completed
         * @param string $version
         */
		public function mark_completed($version) {

			$upgrade_functions = $this->get_upgrade_functions($version);

			foreach($upgrade_functions as $upgrade_function) {
				if(!$this->function_exists($upgrade_function,$version)) {
					$this->create_upgrade_item_dbo($upgrade_function,$version)->insert();
				}
			}
		}

		public function init_upgrade_framework() {

			$db = $this->_db ? $this->_db : DB::get_instance();

			if(!$db)
				throw new Exception("There is no DB connection available");				
			
			$db_utility = $db->get_db_utility();

			//This has to be left becauase the first time the upgrade is ran the upgrade table
			//is called and updated for statuses
			if($db_utility->is_table_exist("upgrade_items"))
				$db->script("	RENAME TABLE `upgrade_items` TO `ff_upgrades`;
								ALTER TABLE `ff_upgrades` 	CHANGE COLUMN `upgrade_item_id` `upgrade_id` INT NOT NULL AUTO_INCREMENT,
															CHANGE COLUMN `function_name` `name` VARCHAR(255)");

			if(!$db_utility->is_table_exist("ff_upgrades")) {
				$db->query("CREATE TABLE `ff_upgrades` (
				                 `upgrade_id` int(11) NOT NULL auto_increment,
				                 `create_date` datetime default NULL,
				                 `name` varchar(255) default NULL,
				                 `version` varchar(10),
				                PRIMARY KEY  (`upgrade_id`))");
			}
		}

		public function process_framework_upgrades() {

			$db = $this->_db ? $this->_db : DB::get_instance();
			$db_utility = $db->get_db_utility();

			if(!$db_utility->is_table_exist("ff_logs"))
				$db->query("CREATE TABLE `ff_logs` (
							  `ff_log_id` INT AUTO_INCREMENT NOT NULL,
							  `create_date` DATETIME,
							  `line` INT,
							  `file` VARCHAR(200),
							  `message` VARCHAR(500),
							  `backtrace` TEXT,
							  `notified` CHAR(1),
							  `notify_date` DATETIME,
							  `url` VARCHAR(500),
							  `type` VARCHAR(20),
							  `server` TEXT,
							  `post` TEXT,
							  `get` TEXT,
							  CONSTRAINT `PK_ff_logs` PRIMARY KEY (`ff_log_id`));");

			if(!$db_utility->is_table_exist("ff_backups"))
				$db->query("CREATE TABLE `ff_backups` (
							  `ff_backup_id` INTEGER(11) AUTO_INCREMENT NOT NULL,
							  `start_time` INTEGER(11),
							  `end_time` INTEGER(11),
							  `filesize` INTEGER(11),
							  `filename` VARCHAR(50),
							  `message` VARCHAR(255),
							  `state` CHAR(1),
							  `create_date` DATETIME,
							  CONSTRAINT `PRIMARY_KEY` PRIMARY KEY (`ff_backup_id`));");

			if($db_utility->is_table_exist("cron_schedules"))
				$db->script("drop table cron_schedules");

			if(!$db_utility->is_table_exist("ff_crons"))
				$db->script("CREATE TABLE `ff_crons` (
									  `cron_id` int(11) NOT NULL AUTO_INCREMENT,
									  `state` char(1) NOT NULL,
									  `create_date` datetime NOT NULL,
									  `message` text,
									  `process` varchar(100) DEFAULT NULL,
									  `duration` int(11) DEFAULT NULL,
									  `process_id` int(11),
									  PRIMARY KEY (`cron_id`));");


			if(!$db_utility->is_table_column_exist("ff_article_authors","url"))
				$db->script("ALTER TABLE `ff_article_authors` ADD COLUMN url VARCHAR(40);");

			if(!$db_utility->is_table_column_exist("ff_crons","long_running"))
				$db->script("ALTER TABLE `ff_crons` ADD COLUMN long_running CHAR(1) default 0;");

			//Run any upgrades from the site content system.
			//CMODEL_SITE_CONTENT::create()->upgrade(); 
			CMODEL_SITE_CONTENT_WIDGET::create()->upgrade();
			CMODEL_MESSAGE::create()->upgrade();
		}

        /**
         * mark all completed
         * @return boolean
         */
		public function mark_all_completed() {

			DB::start_transaction();

			$versions = array_keys($this->get_upgrade_versions());

			foreach($versions as $version)
				$this->mark_completed($version);

			$has_success = DB::has_transaction_success();
			DB::complete_transaction();

			return $has_success;
		}

        /**
         * Returns the upgrade file
         * @param string $version
         * @return string
         */
		public function get_upgrade_file($version) {
			return $this->get_upgrade_script_directory()."/upgrade_".$version.".inc";
		}

	    /**
	     * Returns the upgrade versions
	     * @return array
	     */
		public function get_upgrade_versions() {
			$versions = array();
			$upgrade_filenames = FILE_UTIL::get_directory_listing($this->get_upgrade_script_directory());


			$versions = array();
			foreach($upgrade_filenames as $upgrade_filename) {

				if(preg_match("/^upgrade_(.*).inc$/",$upgrade_filename,$matches)) {

					$version = get_value($matches,1);

					if(self::is_valid_version($version))
						$versions[$version] = str_replace("_",".",$version);
				}
			}

			uasort($versions,array($this,"compare_versions"));

			return $versions;
		}

        /**
         * compare_versions
         * @param string $version1
         * @param string $version2
         * @return mixed
         */
		private function compare_versions($version1,$version2) {

			$version1 = str_replace("_",".",$version1);
			$version2 = str_replace("_",".",$version2);

			return version_compare($version1,$version2);
		}

        /**
         * create upgrade
         * @param string $version
         * @return upgrade_class|null
         * @throws Exception
         */
		public function create_upgrade($version) {

			$upgrade_file = $this->get_upgrade_file($version);

			if(is_file($upgrade_file)) {

				require_once($upgrade_file);

				$upgrade_class = "UPGRADE_".$version;

				if(!class_exists($upgrade_class))
					throw new Exception("Unable to locate upgrade class");

				return new $upgrade_class();

			} else
				throw new Exception("Failed to locate upgrade file for version ".$version);

			return null;
		}

        /**
         * Returns the upgrade functions
         * @param array $versions
         * @return array
         * @throws Exception
         */
		public function get_upgrade_functions($versions) {

			$versions = is_array($versions) ? $versions : array($versions);

			$upgrade_functions = array();
			foreach($versions as $version) {

				$upgrade = $this->create_upgrade($version);

				$class_methods = get_class_methods($upgrade);

				foreach($class_methods as $function)
					if(strpos($function,"upgrade_")===0)
						$upgrade_functions[] = $function;
			}

			return $upgrade_functions;
		}

        /**
         * Returns the queued upgrade items
         * @param string $version
         * @return array
         */
		public function get_queued_upgrade_items($version) {

			$upgrade_functions = $this->get_upgrade_functions($version);
			$names = ARRAY_UTIL::get_list_from_array($this->get_upgrade_items($version),"name");

			return array_diff($upgrade_functions,$names);
		}

        /**
         * Returns the upgrade item dbq
         * @return DBQ_FF_UPGRADE
         */
		public function get_upgrade_item_dbq() {

			$upgrade_dbq = BASE_DBQ_FF_UPGRADE::create();

			if($this->_db)
				$upgrade_dbq->set_db_connection($this->_db);

			return $upgrade_dbq;
		}

        /**
         * Returns the version upgrade items
         * @param array $versions
         * @return array
         */
		public function get_version_upgrade_items($versions) {

			$upgrades = $this->get_upgrade_item_dbq()
								->where("version","IN",$versions)
								->select(array("version","name"));

			$version_functions = array();
			foreach($upgrades as $upgrade)
				$version_functions[value($upgrade,"version")][] = value($upgrade,"name");

			return $version_functions;
		}

        /**
         * Returns the queued version upgrade items
         * @return array
         */
		public function get_queued_version_upgrade_items() {

			$versions = array_keys($this->get_upgrade_versions());

			$version_upgrade_items = $this->get_version_upgrade_items($versions);

			$queued_version_upgrade_items = array();

			foreach($versions as $version) {

				$upgrade_functions 	= $this->get_upgrade_functions($version);
				$upgrade_items 		= value($version_upgrade_items,$version,array());

				$upgrade_items		= array_diff($upgrade_functions,$upgrade_items);

				if($upgrade_items)
					$queued_version_upgrade_items[$version] = $upgrade_items;
			}

			return $queued_version_upgrade_items;
		}

        /**
         * Returns the upgrade items
         * @param string $version
         * @return object
         */
		public function get_upgrade_items($version="") {
			$upgrade_item_dbq = $this->get_upgrade_item_dbq();

			if($version)
				$upgrade_item_dbq->where("version","=",$version);

			return $upgrade_item_dbq->select(array("*"),"name");
		}

        /**
         * Returns the current upgrade items
         * @return object
         */
		public function get_current_upgrade_items() {
			return $this->get_upgrade_items($this->get_current_version());
		}

        /**
         * create upgrade item dbo
         * @param string $upgrade_function
         * @param string $version
         * @return DBO_FF_UPGRADE
         */
		public function create_upgrade_item_dbo($name,$version) {
			$upgrade_dbo = BASE_DBO_FF_UPGRADE::create()
								->set_name($name)
								->set_create_date(DB::get_date_time())
								->set_version($version);

			if($this->_db)
				$upgrade_dbo->set_db_connection($this->_db);

			return $upgrade_dbo;
		}

        /**
         * function exists
         * @param string $upgrade_function
         * @param string $version
         * @return boolean
         */
		public function function_exists($name,$version) {
			return $this->get_upgrade_item_dbq()
						->where("name","=",$name)
						->where("version","=",$version)
						->exists();
		}

        /**
         * sanitize version
         * @param string $version
         * @return mixed
         */
		static function sanitize_version($version) {
			return preg_replace("/[^_\da-z]/i","",$version);
		}

        /**
         * Checks whether is a valid version
         * @param string $version
         * @return integer
         */
		static function is_valid_version($version) {
			return preg_match("/^(trunk|[0-9]+_[0-9]+_[0-9]+?)$/",$version);
		}
	}

    /**
    * Class UPGRADE
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class UPGRADE extends MODEL {

		protected $_success = true;

        /**
         * query
         * @param array $queries
         * @return boolean
         */
		function query($queries) {

			if(!is_array($queries))
				$queries = array($queries);

			$db = DB::get_instance();

			foreach($queries as $query)
				if(!$db->_query($query))
					throw new Exception($db->get_error_message()." ".$query);

			return true;
		}

        /**
         * query script
         * @param string $script
         * @return boolean
         */
		function query_script($script) {
			$script = trim(trim($script),";");
			$queries = explode(";",$script);

			return $this->query($queries);
		}

		function drop_foreign_key($table,$key,$column="") {

			$this->query("ALTER TABLE `".$table."` DROP FOREIGN KEY `".$key."`;");

			if($column)
				$this->drop_column($table,$column);

			return $this;
		}

		function column_exists($table,$column) {
			return DB::get_instance()->get_db_utility()->is_table_column_exist($table,$column);
		}

		function drop_column($table,$column="",$foreign_key="") {

			if($foreign_key)
				$this->drop_foreign_key($table,$foreign_key);

			$this->query("ALTER TABLE `".$table."` DROP COLUMN `".$column."`;");

			return $this;
		}

		function drop_table($table) {

			$this->query("DROP TABLE `".$table."`;");

			return $this;
		}

		function add_column($table,$column,$meta,$foreign_table="",$foreign_key="") {

			$this->query("ALTER TABLE `".$table."` ADD COLUMN `".$column."` ".$meta.";");

			if($foreign_table && $foreign_key)
				$this->query("ALTER TABLE `".$table."` ADD CONSTRAINT `".$foreign_key."` FOREIGN KEY (`".$column."`) REFERENCES `".$foreign_table."` (`".$column."`);");

			return $this;
		}
	}