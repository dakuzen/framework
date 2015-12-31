<?
    /**
    * Class DB_UTILITY  
    *
    * @package Library
    * @subpackage Db
    */

	abstract class DB_UTILITY {
                
		/**
		 * The debug
		 * @var boolean $debug 
		 */
		protected $_debug 	= false;

		/**
		 * The db
		 * @var object $db 
		 */
		protected $_db 		= null;

		/**
		 * The db user
		 * @var string $db_user 
		 */
		protected $_db_user	= null;

		/**
		 * The db port
		 * @var string $db_port 
		 */
		protected $db_port	= null;

		/**
		 * The db pass
		 * @var string $db_pass 
		 */
		protected $_db_pass	= null;		

		/**
		 * The db sock
		 * @var string $db_sock 
		 */
		protected $_db_sock	= null;

		/**
		 * The db host
		 * @var string $db_host 
		 */
		protected $_db_host 	= "127.0.0.1";

		/**
		 * Constructor
		 * @param string $db_host
		 * @param string $db_user
		 * @param string $db_pass
		 * @param string $db_name
		 * @param array $options
		 * @throws Exception 
		 */
		function __construct($db_host=null,$db_user=null,$db_pass=null,$db_name=null,$options=array()) {
			
			$db_sock = get_value($options,"sock");
			$db_port = get_value($options,"port");
         
            if($db_host || $db_user || $db_pass || $db_name || $db_sock) {
				
				if(!$db_host && !$db_sock)
					throw new Exception("Invalid database host address or socket");

				if(!$db_user)
					throw new Exception("Invalid database user");

				if(!$db_pass)
					throw new Exception("Invalid database password");

				$this->_db = DB::get_db_connection($db_host,$db_user,$db_pass,$db_name,null,$options);
				
				$this->_db_host = $db_host;
				$this->_db_pass = $db_pass;
				$this->_db_user = $db_user;
				$this->_db_port = $db_port;
			} else 
				$this->_db = DB::get_instance();
		}
		
		/**
		 * Debug
		 * @param boolean $debug 
		 */
		function debug($debug=true)	{ $this->_debug = $debug; }

		/**
		 * Set db connection
		 * @param object $db 
		 */
		function set_db_connection($db) { $this->_db = $db; }

		/**
		 * Return the db
		 * @return object 
		 */
		function get_db() 		{ return $this->_db; }

		/**
		 * Export
		 * @param string $db_name
		 * @param string $db_file 
		 * @param array $options
		 */
		abstract function export($db_name,$db_file,$options=array());

		/**
		 * Import 
		 * @param string $db_name
		 * @param string $db_file 
		 */
		abstract function import($db_name,$db_file);

		/**
		 * drop database 
		 * @param string $db_name
		 */
		abstract function drop_database($db_name);

		/**
		 * create database 
		 * @param string $db_name
		 */
		abstract function create_database($db_name);

		/**
		 * Check whether user exist
		 * @param string $host
		 * @param string $username
		 */
		abstract function is_user_exist($host,$username);

		/**
		 * truncate table
		 * @param string $table_name 
		 */
		abstract function truncate_table($table_name);

		/**
		 * Checks whether table exists
		 * @param string $table_name 
		 */
		abstract function is_table_exist($table_name);

		/**
		 * Checks whether database exists
		 * @param string $db_name 
		 */
		abstract function is_database_exist($db_name);	

		/**
		 * Checks whether table column exist
		 * @param string $table_name 
		 * @param string $column
		 */
		abstract function is_table_column_exist($table_name,$column);
		public function column_exist($table_name,$column) {
			return $this->is_table_column_exist($table_name,$column);
		}

		/**
		 * Return table names
		 */
		abstract function get_table_names();		

		/**
		 * Return table fields
		 * @param string $table_name
		 */
		abstract function get_table_fields($table_name);

		/**
		 * Return table column list
		 * @param string $table_name
		 */
		abstract function get_table_column_list($table_name);

		/**
		 * Return table columns
		 * @param string $table_name
		 */
		abstract function get_table_columns($table_name);

		/**
		 * Return table field names
		 * @param string $table_name
		 */
		abstract function get_table_field_names($table_name);

		/**
		 * drop user
		 * @param string $db_host
		 * @param string $db_user
		 */
		abstract function drop_user($db_host,$db_user);

		/**
		 * grant user
		 * @param string $host
		 * @param string $database
		 * @param string $username
		 * @param string $password
		 * @param boolean $select
		 * @param boolean $update
		 * @param boolean $delete
		 * @param boolean $insert
		 * @param array $privs
		 */
		abstract function grant_user($host,$database,$username,$password,$select=true,$update=true,$delete=true,$insert=true,$privs=array());	

		/**
		 * revoke user
		 * @param string $host
		 * @param string $database
		 * @param string $username
		 * @param boolean $select
		 * @param boolean $update
		 * @param boolean $delete
		 * @param boolean $insert
		 */
		abstract function revoke_user($host,$database,$username,$select=true,$update=true,$delete=true,$insert=true);	
	}