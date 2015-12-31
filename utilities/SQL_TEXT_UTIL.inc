<?php
        /**
        * Class SQL_TEXT_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */
		
	class SQL_TEXT_UTIL {
		
		/**
		 * Creates a database connection  
		 *
		 * @param string $db_data_dir string directory
		 * @param string $db_user string database user
		 * @param string $db_pass string database password
		 * @param string $db_name string database name
		 * @return string
		 */
		function create_db_connection($db_data_dir,$db_user,$db_pass,$db_name=null) {
			
			$db = new DB_SQL_TEXT($db_data_dir);
			$db->strict(0);
			
			if($db->connect($db_user, $db_pass)) {
			
				if($db_name && !$db->selectdb($db_name))
					throw new Exception($db->get_last_error());			
				
				return $db;
				
			} else
				throw new Exception($db->get_last_error());
		}
	}
				
	/**
	 * Class DB_SQL_TEXT  
	 *
	 * @package Framework
	 * @subpackage Utilities
	 */
	class DB_SQL_TEXT extends txtSQL {
	
		/**
		 * Makes a connection
		 *
		 * @param string $user string user
		 * @param string $pass string password
		 * @return boolean
		 */
		function connect($user, $pass) {
			
			$has_success = parent::connect($user,$pass);
			
			$this->_query            = new DB_SQL_TEXT_CORE;
			$this->_query->_LIBPATH  = $this->_LIBPATH;
			
			return $has_success;			
		}
		
		/**
                 * Returns error
                 * @param ing $errno
                 * @param integer $errstr
                 * @param string $errtype 
                 */
		static function __error($errno, $errstr, $errtype=NULL) {
			if(SYSTEM_MANAGER::is_system_mode_debug())
				p("DB Error: ".$errstr);
		}
	
		/**
                 * Returns a dump with error
                 * @param object $object 
                 */
		function __error_dump($object) {
			$messages = array();
			foreach ( $object->_ERRORS as $key => $value )
				$messages[] = $key." - ".$value;
			
			foreach ( $object->_query->_ERRORS as $key => $value )
				$messages[] = $key." - ".$value;
				
			if($messages)
				p(implode(", ",$messages));		
		}

		/**
                 * Returns an error
                 * @param integer $errno
                 * @param string $errstr
                 * @param string $errtype 
                 */
		function _error ($errno, $errstr, $errtype=NULL) {
			self::__error($errno, $errstr, $errtype);
		}

		/**
                 * Returns an dump of error
                 */
		function errordump() {
			self::__error_dump($this);		
		}		
	}	

	/**
	 * Class DB_SQL_TEXT  
	 *
	 * @package Framework
	 * @subpackage Utilities
	 */
	class DB_SQL_TEXT_CORE extends txtSQLCore {
		
		/**
                 * Returns an error
                 * @param integer $errno
                 * @param string $errstr
                 * @param string $errtype 
                 */
		function _error ($errno, $errstr, $errtype=NULL) {
			DB_SQL_TEXT::__error($errno, $errstr, $errtype);
		}

		/**
                 * Dumps an error
                 */
		function errordump() {
			DB_SQL_TEXT::__errordump($this);	
		}
	}
        

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