<?
	/**
        * Class UTILITY_TXTSQL  
        *
        * @package Library
        * @subpackage Db
        */
	class UTILITY_TXTSQL extends DB_UTILITY {
		
                /**
                 * Sonstructor
                 * @param string $db_host
                 * @param string $db_user
                 * @param string $db_pass
                 * @param string $db_name 
                 */
		function __construct($db_host=null,$db_user=null,$db_pass=null,$db_name=null) {
			
			if($db_host || $db_user || $db_pass || $db_name) {
				self::validate_db_credentials($db_host,$db_user,$db_pass);
				
				$this->_db = DB::get_db_connection($db_host,$db_user,$db_pass,$db_name);
				
				$this->_db_host = $db_host;
				$this->_db_pass = $db_pass;
				$this->_db_user = $db_user;
			} else 
				$this->_db = DB::get_instance();
		}
		
                /**
                 * validate db credentials
                 * @param string $db_host
                 * @param string $db_user
                 * @param string $db_pass
                 * @return boolean
                 * @throws Exception 
                 */
		static function validate_db_credentials($db_host,$db_user,$db_pass) {
			
			if(!$db_host)
				throw new Exception("Invalid database host address");
				
			if(!$db_user)
				throw new Exception("Invalid database user");
				
			if(!$db_pass)
				throw new Exception("Invalid database password");
				
			return true;
		}
		
                /**
                 * export
                 * @param string $db_name
                 * @param string $db_file
                 * @param boolean $is_clone_database_structure
                 * @param boolean $is_clone_database_data
                 * @param array $options 
                 */
		function export($db_name,$db_file,$is_clone_database_structure=true,$is_clone_database_data=true,$options=array()) {		
			p("Not Implemented TXTSQL - export"); 
		}
		
                /**
                 * import
                 * @param string $db_name
                 * @param string $db_file 
                 */
		function import($db_name,$db_file) {
			p("Not Implemented TXTSQL - import"); 
		}
		
                /**
                 * drop database
                 * @param string $db_name 
                 */
		function drop_database($db_name) {
			p("Not Implemented TXTSQL - drop_database"); 
		}
		
                /**
                 * drop user
                 * @param string $db_host
                 * @param string $db_user 
                 */
		function drop_user($db_host,$db_user) {
			p("Not Implemented TXTSQL - drop_user"); 
		}
		
                /**
                 * create database
                 * @param string $db_name 
                 */
		function create_database($db_name) {
			p("Not Implemented TXTSQL - create_database"); 
		}
		
                /**
                 * Checks whether user exist
                 * @param string $host
                 * @param string $username 
                 */
		function is_user_exist($host,$username) {
			p("Not Implemented TXTSQL - is_user_exist"); 
		}
			
                /**
                 * truncate table
                 * @param string $tablename 
                 */
		function truncate_table($tablename) {
			p("Not Implemented TXTSQL - truncate_table"); 
		}
		
                /**
                 * Checks whether table exist
                 * @param string $table_name 
                 */
		function is_table_exist($table_name) {
			p("Not Implemented TXTSQL - is_table_exist"); 			
		}		
		
                /**
                 * Checks whether database exist
                 * @param string $db_name 
                 */
		function is_database_exist($db_name) {
			p("Not Implemented TXTSQL - is_database_exist"); 
		}
		
                /**
                 * Checks whether table column exist
                 * @param string $table_name
                 * @param string $column 
                 */
		function is_table_column_exist($table_name,$column) {							
			p("Not Implemented TXTSQL - is_table_column_exist"); 		
		}		
		
                /**
                 * Returns table field names
                 * @param string $table_name 
                 */
		function get_table_field_names($table_name) {							
			p("Not Implemented TXTSQL - get_table_field_names"); 		
		}	
		
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
		function revoke_user($host,$database,$username,$select=true,$update=true,$delete=true,$insert=true) {
			p("Not Implemented TXTSQL - revoke_user"); 		
		}		
		
                /**
                 * Returns table names
                 * @return array 
                 */
		function get_table_names() {
			$tables = $this->_db->get_connection()->showtables(array("db"=>$this->_db->get_db_name())); 
			return array_combine($tables,$tables);
		}		
		
                /**
                 * Returns table fields
                 * @param string $tablename
                 * @return array 
                 */
		function get_table_fields($tablename) {
			
			$describe = $this->_db->get_connection()->describe(array('db'=>$this->_db->get_db_name(),'table'=>$tablename)); 	
			
			$primary = get_value($describe,"primary");			
			
			$fields = array();
			foreach($describe as $field=>$value) {
				if(is_array($value)) {
					$is_key = $primary==$field;
					$fields[$field] = array("Field"=>$field,"Null"=>true,"Key"=>$is_key,"Type"=>get_value($value,"type"));
				}
			}
			return $fields;
		}
		
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
		function grant_user($host,$database,$username,$password,$select=true,$update=true,$delete=true,$insert=true,$privs=array()) {
			p("Not Implemented TXTSQL - grant_user"); 	
		}
		
		
		
	}