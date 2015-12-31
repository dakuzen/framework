<?php
        /**
        * This constant defines the txtSQL path.
        * 
        * @package Framework
        * @subpackage Utilities
        */
        
	define("TXTSQL_CORE_PATH",DIR_FRAMEWORK."packages/txtSQL/");

	SERVER_UTIL::add_include_path(TXTSQL_CORE_PATH);
	
	
	require_once("txtSQL.class.php");
	require_once("txtSQL.core.php");      

        /**
        * Class TXTSQL_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */
	
	class TXTSQL_UTIL {
		
                /**
                 * Create a database connection
                 * @param string $db_data_dir
                 * @param string $db_user
                 * @param string $db_pass
                 * @param string $db_name
                 * @return object TXTSQLE
                 * @throws Exception 
                 */
		function create_db_connection($db_data_dir,$db_user,$db_pass,$db_name=null) {
			
			$db = new TXTSQLE($db_data_dir);
			$db->strict(0);
			
			if($db->connect($db_user, $db_pass)) {
				
				if($db_name && !$db->selectdb($db_name))
					throw new Exception($db->get_last_error());			
				
				$db->debug(false);
				
				return $db;
				
			} else {
				throw new Exception($db->get_last_error());
			}
		}
		
		/**
		 * Get database file
		 *
		 * @return string
		 */	
		 
		static function get_default_db_file() 			{ return DIR_FRAMEWORK."packages/txtSQL/data/txtsql/".self::get_default_db_filename(); }
		
		/**
		 * Gets default database file name
		 *
		 * @return string
		 */
		 
		static function get_default_db_filename() 		{ return "txtsql.MYI"; }
		
		/**
		 * Gets sql database directory
		 *
		 * @param string $db_data_dir string database directory
		 * @return string
		 */
		 
		static function get_txtsql_db_dir($db_data_dir) 	{ return $db_data_dir."/txtsql/"; }
		
		/**
		 * Gets sql database file
		 *
		 * @param string $db_data_dir string database directory
		 * @return string
		 */
		 
		static function get_txtsql_db_file($db_data_dir) 	{ return self::get_txtsql_db_dir($db_data_dir).self::get_default_db_filename(); }
		
                /**
                 * Install database
                 * @param string $db_data_dir
                 * @return boolean 
                 * @todo review description, parameters, return type
                 */
		static function install_db($db_data_dir) {
			
			$txtsql_dir = self::get_txtsql_db_dir($db_data_dir);
			
			$db_data_file = $txtsql_dir.self::get_default_db_filename();
			
			if(is_file($db_data_file))
				true;
			
			FILE_UTIL::mkdir($txtsql_dir);
			
			return FILE_UTIL::copy(self::get_default_db_file(),$db_data_file);
		}
		
		/**
		 * Checks if sql database exists
		 *
		 * @param string $db_data_dir string database directory
		 * @return boolean
		 */
		 
		static function txtsql_db_exists($db_data_dir) {
			return is_file(self::get_txtsql_db_file($db_data_dir));	
		}
	}
				
	/**
	 * Class TXTSQLE  
	 *
	 * @package Framework
	 * @subpackage Utilities
	 */
	 
	class TXTSQLE extends txtSQL {
		
                /**
                 * Debug
                 * @var boolean $debug 
                 */
		protected $_debug = false;
	
		/**
		 * Connects to a database
		 *
		 * @param string $user string username
		 * @param string $pass string password
		 * @return boolean
		 */
		 
		function connect($user, $pass) {
			
			$has_success = parent::connect($user,$pass);
			
			$this->_query            = new TXTSQLCOREE($this->_debug);
			$this->_query->_LIBPATH  = $this->_LIBPATH;
			
			return $has_success;			
		}

                /**
                 * Display an error
                 * 
                 * @param integer $errno
                 * @param string $errstr
                 * @param string $errtype 
                 */
		function display_error($errno, $errstr, $errtype=null) {
			if(SYSTEM_MANAGER::is_system_mode_debug() || $this->_debug)
				p("DB Error: ".$errstr);	
		}
		
		/**
                 * Setup an error
                 * 
                 * @param integer $errno
                 * @param string $errstr
                 * @param string $errtype 
                 */
		function _error($errno, $errstr, $errtype=null) {
			$this->_ERRORSPLAIN[] 	= $errstr;
			$this->_ERRORS[] 	= $errstr;
		
			$this->display_error($errno, $errstr, $errtype);
		}
		
                /**
                 * Dumps an error 
                 */
		function errordump() {}	
		
		/**
		 * Allows debugging
		 *
		 * @param boolean $debug boolean 
		 */
		 
		function debug($debug=true) { $this->_debug = $debug; }	
	}	

	/**
	 * Class TXTSQLCOREE  
	 *
	 * @package Framework
	 * @subpackage Utilities
	 */
	 
	class TXTSQLCOREE extends txtSQLCore {
		
                /**
                 * Debug
                 * @var boolean $debug 
                 */
		protected $_debug = false;
		
                /**
                 * Construct
                 * 
                 * @param boolean $debug the debug mode
                 */
		function __construct($debug) {
			parent::__construct();
			$this->_debug = $debug;
		}
		
                /**
                 * Setup an error
                 * @param integer $errno
                 * @param string $errstr
                 * @param string $errtype 
                 */
		function _error($errno, $errstr, $errtype=null) {
			$this->_ERRORSPLAIN[] 	= $errstr;
			$this->_ERRORS[] 	= $errstr;
				
			$this->display_error($errno, $errstr, $errtype);
		}

                /**
                 * Displays an error
                 * @param integer $errno
                 * @param string $errstr
                 * @param string $errtype 
                 */
		function display_error($errno, $errstr, $errtype=null) {
			if(SYSTEM_MANAGER::is_system_mode_debug() || $this->_debug)
				p("DB Error: ".$errstr);	
		}

                /**
                 * Dumps an error 
                 */
		function errordump() {}
	}  