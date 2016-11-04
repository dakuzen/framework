<?
    /**
    * Class DB
    *
    * @package Library
    * @subpackage Db
    */

	abstract class DB {

		const DB_TYPE_MYSQLI 				= "mysqli";
		const DB_TYPE_MYSQL 				= "mysql";
		const DB_TYPE_TXT_SQL 				= "txtsql";

		const EVENT_ERROR					= "error";
		const EVENT_QUERY					= "query";

		const PARM_CHARACTER				= "~~!~~";

		const ERROR_METHOD_TRIGGER_ERROR	= "trigger_error";
		const ERROR_METHOD_EXCEPTION		= "exception";

		protected $_config				= array("error_method"=>self::ERROR_METHOD_EXCEPTION);
		/**
		 * The debug mode
		 * @var boolean $debug
		 */
		protected static $_debug 			= false;

		/**
		 * The debug extend status
		 * @var boolean $debug_extend
		 */
		protected static $_debug_extend		= false;

        /**
         * instance
         * @var object $instance
         */
		static $_instance				= null;

        /**
         * The guid
         * @var string $guid
         */
		protected $_guid				= "";

        /**
         * The connection
         * @var object $connection
         * @todo review var type
         */
		protected $_connection 				= null;

        /**
         * The database name
         * @var string $db_name
         */
		protected $_db_name				= null;

	    /**
	     * The insert id
	     * @var integer $insert_id
	     */
		protected $_insert_id				= null;

        /**
         * The transactions
         * @var boolean $transactions
         */
		protected $_transactions			= true;

        /**
         * The error message
         * @var string $error_message
         */
		protected $_error_message			= "";

        /**
         * The logging status
         * @var boolean $is_logging
         */
		protected $_is_logging				= true;


		/**
		 * The logger cmodel
		 * @var object $logger_cmodel
		 */
		protected $_logger_cmodel			= null;

		/**
		 * Returns the db name
		 * @return string
		 */
		public function get_db_name() 			{ return $this->_db_name; }

		/**
		 * Returns the insert id
		 * @return string
		 */
		public function get_insert_id() 			{ return $this->_insert_id; }
		public function get_error_message()			{ return $this->_error_message; }
		public function get_connection() 			{ return $this->_connection; }

		public function is_debug() 					{ return self::$_debug; }
		public function is_debug_extend()			{ return self::$_debug_extend; }
		public function query($sql)					{ return $this->_query($sql); }
		public function script($script)				{ return $this->_script($script); }
		public function has_transactions() 			{ return $this->_transactions; }

		public function set_insert_id($insert_id) 	{ $this->_insert_id = $insert_id; }
		public function set_db_name($db_name) 		{ $this->_db_name = $db_name; }
		public function set_connection($connection) { $this->_connection = $connection; }

		public function enable_debug() 				{ self::$_debug = true; }
		public function transactions($value=true)	{ $this->_transactions = $value; }

		public function disable_debug() 			{ self::$_debug = false; }

		public function __construct() {
			$this->_guid = MISC_UTIL::get_guid(12);
		}

		public function set_debug($debug,$debug_extend=false) {
			self::$_debug_extend = $debug_extend;
			self::$_debug = $debug;
		}

		public function config($key,$value=null) {
			if($value!==null) {
				$this->_config[$key] = $value;
				return $this;
			}

			return value($this->_config,$key);
		}

		abstract function connect($db_host,$db_user,$db_pass,$db_name=null,$options=array());
		abstract function delete($dbq,$data);
		abstract function update($dbq,$data);
		abstract function insert($dbq,$data);
		abstract function replace($dbq,$keys,$data);
		abstract function select_one($dbq,$field);
		abstract function select($dbq,$fields,$index_field=null);
		abstract function select_row($dbq,$fields,$index_field=null);
		abstract function select_column($dbq,$field,$index_field=null);
		abstract function close();

		abstract function get_select_sql($dbq,$fields);

		abstract function _query($sql,$data=array());
		abstract function _select($sql);

        /**
         * select one
         * @param string $sql
         */
		abstract function _select_one($sql);
		abstract function _select_row($sql);
		abstract function _start_transaction();
		abstract function _has_transaction_failed();
		abstract function _has_transaction_success();
		abstract function _complete_transaction();
		abstract function _fail_transaction();
		abstract function _disable_foreign_keys();
		abstract function _enable_foreign_keys();

		abstract static function get_db_utility($db_host=null,$db_user=null,$db_pass=null,$db_name=null);

		static function set_default_db_connection($db) 	{ self::$_instance = $db; }
		static function has_default_db_connection() 	{ return self::$_instance != null; }
		static function get_default_db_connection() 	{ return self::$_instance; }
		static function get_instance() 					{ return self::$_instance; }
		static function instance() 						{ return self::$_instance; }

		public function one($sql) { return $this->_select_one($sql); }
		public function row($sql) { return $this->_select_row($sql); }
		public function all($sql) { return $this->_select($sql); }

		public function log($log_directory) {
			$this->_logger_cmodel = BASE_CMODEL_LOGGER_FILE::create(BASE_CMODEL_LOGGER::LEVEL_DEBUG,$log_directory,"db.log");
			$this->_logger_cmodel->set_prefix("[".$this->_guid."] ".$this->_logger_cmodel->get_prefix());
		}

		public function event($event,$data=null,$duration=0) {

			if($this->_logger_cmodel) {

				$data = "[".number_format($duration,5)."] ".$data;

				if($event==self::EVENT_ERROR)
					$this->_logger_cmodel->error($data);

				if($event==self::EVENT_QUERY)
					$this->_logger_cmodel->debug($data);
			}
		}

        /**
         * Returns the db connection
         * @param string $db_host
         * @param string $db_user
         * @param string $db_pass
         * @param string $db_name
         * @param string $type
         * @param array $options
         * @return object
         * @throws Exception
         */
		static function get_db_connection($db_host,$db_user,$db_pass,$db_name=null,$type=null,$options=array()) {

			$type = $type ? $type : self::DB_TYPE_MYSQLI;

			$db = null;

			if($type==self::DB_TYPE_MYSQLI) {
				$db = self::create_db_mysqli();
				$db->connect($db_host,$db_user,$db_pass,$db_name,$options);

			} elseif($type==self::DB_TYPE_MYSQL) {
				$db = self::create_db_mysql();
				$db->connect($db_host,$db_user,$db_pass,$db_name,$options);

			} elseif($type==self::DB_TYPE_TXT_SQL) {
				$db = self::create_db_txtsql();
				$db->connect($db_host,$db_user,$db_pass,$db_name,$options);
			} else
				throw new BCMODEL_DB_EXCEPTION("Invalid database type");

			return $db;
		}

        /**
         * create db mysqli
         * @return DB_MYSQLI
         */
		static function create_db_mysqli() {
			self::include_db_mysqli();
			return new DB_MYSQLI();
		}

        /**
         * create db mysql
         * @return DB_MYSQL
         */
		static function create_db_mysql() {
			self::include_db_mysql();
			return new DB_MYSQL();
		}

		static function create_db_txtsql() {
			self::include_db_txtsql();
			return new DB_TXTSQL();
		}

		static function include_db_mysqli() 	{ require_once("driver/DB_MYSQLI.inc"); }
		static function include_db_mysql() 		{ require_once("driver/DB_MYSQL.inc"); }
		static function include_db_txtsql() 	{ require_once("driver/DB_TXTSQL.inc"); }

		static function debugs($session_id,$value=true,$debug_extend=false) {

			if($session_id==SESSION_MANAGER::get_session_id())
				self::debug($value,$debug_extend);
		}

		static function debug($value=true,$debug_extend=false) {

			if($debug=get_value(debug_backtrace(),0)) {

				if(DEBUG_UTIL::is_format_html())
					echo '<div style="padding:2px;border: 0px dashed red;font-weight:bold">DEBUG: '.value($debug,"file").' @ line number '.value($debug,"line")."</div>";
				else
					echo 'DEBUG: '.value($debug,"file").' @ line number '.value($debug,"line")."\n\n";

				DB::get_instance()->set_debug($value,$debug_extend);

				flush();
			}
		}

		static function disable_transactions() {
			if(!(isset($this) && get_class($this) == __CLASS__))
				return DB::get_instance()->transactions(false);

			return $this->disable_transactions();
		}

		static function enable_transactions() {
			if(!(isset($this) && get_class($this) == __CLASS__))
				return DB::get_instance()->transactions(true);

			return $this->enable_transactions();
		}

		static function start_transaction() {
			if(!(isset($this) && get_class($this) == __CLASS__))
				return DB::get_instance()->_start_transaction();

			return $this->start_transaction();
		}

		static function has_transaction_failed() {
			if(!(isset($this) && get_class($this) == __CLASS__))
				return DB::get_instance()->_has_transaction_failed();

			return $this->has_transaction_failed();
		}

		static function has_transaction_success() {
			if(!(isset($this) && get_class($this) == __CLASS__))
				return DB::get_instance()->_has_transaction_success();

			return $this->has_transaction_success();
		}

		static function complete_transaction() {
			if(!(isset($this) && get_class($this) == __CLASS__))
				return DB::get_instance()->_complete_transaction();

			return $this->complete_transaction();
		}

        /**
         * fail transaction
         * @return mixed
         * @review return type
         */
		static function fail_transaction() {
			if(!(isset($this) && get_class($this) == __CLASS__))
				return DB::get_instance()->_fail_transaction();

			return $this->fail_transaction();
		}

		static function enable_foreign_keys() {
			if(!(isset($this) && get_class($this) == __CLASS__))
				return DB::get_instance()->_enable_foreign_keys();

			return $this->fail_transaction();
		}

        /**
         * disable foreign keys
         * @return mixed
         * @review return type
         */
		static function disable_foreign_keys() {
			if(!(isset($this) && get_class($this) == __CLASS__))
				return DB::get_instance()->_disable_foreign_keys();

			return $this->fail_transaction();
		}

        /**
         * extract column name
         * @param string $column
         * @return null
         */
		static function extract_column_name($column) {

			if(($as_pos=strripos($column," as "))!==false)
				return trim(substr($column,$as_pos+4));

			$column_parts = explode(".",$column);
			return get_value($column_parts,1,get_value($column_parts,0));
		}

        /**
         * Return date
         * @param integer $time
         * @return string
         */
		static public function get_date($time=null) {
			$time = func_num_args()==0 ? time() : $time;
			return self::get_formatted_time("Y-m-d",$time);
		}

        /**
         * Returns date time
         * @param integer $time
         * @return string
         */
		static public function get_date_time($time=null) {
			$time = func_num_args()==0 ? time() : $time;
			return self::get_formatted_time("Y-m-d H:i:s",$time);
		}

        /**
         * Returns the formatted time
         * @param string $format
         * @param integer $time
         * @return string
         */
		static public function get_formatted_time($format,$time) {

			if(!is_numeric($time))
				return null;
			elseif(is_null($time))
				return null;

			return date($format,$time);
		}
	}

	class DB_EXCEPTION extends Exception {}