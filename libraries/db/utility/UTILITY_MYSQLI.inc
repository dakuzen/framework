<?
	/**
    * Class UTILITY_MYSQLI  
    *
    * @package Library
    * @subpackage Db
    */
	class UTILITY_MYSQLI extends DB_UTILITY {
		
		const EXPORT_OPTOIN_NO_DATA = "--no-data";

        /**
         * Export
         * @param string $db_name
         * @param string $db_file
         * @param array $options
         * @return boolean
         * @throws Exception 
         */
		function export($db_name,$db_file,$options=array()) {
			
			if(!$db_name)
				throw new Exception("Invalid database name");
			
			if(is_empty($db_file))
				throw new Exception("Invalid export file");
						
			$options[] = "--user=".$this->_db_user;
			$options[] = "--password=".$this->_db_pass;
			
			if($this->_db_sock)
				$options[] = "--socket=".$this->_db_sock;
			else
				$options[] = "--host=".$this->_db_host;

			if($this->_db_port)
				$options[] = "--port=".$this->_db_port;
			
			$options[] = "--no-create-db";
			$options[] = "--skip-comments";			
			$options[] = "--result-file=".$db_file;
			$options[] = $db_name;
			
			$cmd = "mysqldump ".implode(" ",$options);
			
			if($this->_debug)
				p($cmd);
			
			$has_success = SERVER_UTIL::exec($cmd,$output,true);
			
			if(!$has_success) 
				throw new Exception(implode(",",$output));
			
			return $has_success; 
		}

        /**
         * Import
         * @param string $db_name
         * @param string $db_file
         * @return boolean
         * @throws Exception 
         */
		function import($db_name,$db_file) {
					
			if(!$db_name)
				throw new Exception("Invalid database name");
				
			if(!is_file($db_file)) 
				throw new Exception("The db file does not exist");
				
			$options = array();			
			$options[] = "--host=".$this->_db_host;
			$options[] = "--user=".$this->_db_user;
			$options[] = "--password=".$this->_db_pass;
			$options[] = $db_name;
			$options[] = "< ".$db_file;
			
			$cmd = "mysql ".implode(" ",$options);
			
			$has_success = SERVER_UTIL::exec($cmd,$output,true);
						
			if(!$has_success)
				throw new Exception(implode(",",$output));
			
			return $has_success; 
		}
		
	    /**
	     * Drop database
	     * @param string $db_name
	     * @return boolean
	     * @throws Exception 
	     */
		function drop_database($db_name) {
		
			if(!$db_name)
				throw new Exception("Invalid database name");
				
			return $this->_db->_query("DROP DATABASE `".$db_name."`");
		}
		
        /**
         * drop user
         * @param string $db_host
         * @param string $db_user
         * @return boolean 
         */
		function drop_user($db_host,$db_user) {
			return $this->_db->_query("DROP USER `".$db_user."`@`".$db_host."`");
		}
		
        /**
         * create database
         * @param string $db_name
         * @return boolean 
         */
		function create_database($db_name) {
			return $this->_db->_query("CREATE DATABASE `".$db_name."`");
		}
		
        /**
         * Checks whether user exists
         * @param string $host
         * @param string $username
         * @return boolean 
         */
		function is_user_exist($host,$username) {
			$sql = "SELECT count(*) FROM `mysql`.`user` WHERE `User` = '".$username."' AND `Host` = '".$host."'";	
			return $this->_db->_select_one($sql)>0;
		}
			
        /**
         * truncate table
         * @param string $tablename
         * @return boolean 
         */
		function truncate_table($tablename) {
			return $this->_db->_query("TRUNCATE TABLE `".$tablename."`");
		}
		
        /**
         * Checks whether table exists
         * @param string $table_name
         * @return boolean 
         */
		function is_table_exist($table_name) {
			return !is_empty($this->_db->_select("SHOW TABLES LIKE '".$table_name."'"));			
		}		
		
        /**
         * Checks whether database exists
         * @param string $db_name
         * @return boolean 
         */
		function is_database_exist($db_name) {
			$sql = "SELECT count(*) FROM `information_schema`.`schemata` WHERE `schema_name` = '".$db_name."'";									
			return $this->_db->_select_one($sql)>0;
		}
		
        /**
         * Checks whether  foreign key exists
         * @param string $table
         * @param string $name
         * @param string $db_name
         * @return boolean 
         */
		function is_foreign_key_exists($table,$name="",$db_name="") {
		
			$db_name = $db_name ? $db_name : $this->get_db()->get_db_name();
			
			$sql = "SELECT COUNT(*) FROM information_schema.TABLE_CONSTRAINTS WHERE TABLE_NAME = '".$table."' AND CONSTRAINT_SCHEMA = '".$db_name."' AND CONSTRAINT_NAME = '".$name."'";
			
			return $this->_db->_select_one($sql)>0;
		}
		
        /**
         * Checks whether table column exists
         * @param string $table_name
         * @param string $column
         * @return boolean 
         */
		function is_table_column_exist($table_name,$column) {							
			return !is_empty($this->_db->_select("DESC `".$table_name."` `".$column."`"));			
		}		
		
        /**
         * Reutnrs table names
         * @return array 
         */
		function get_table_names() {
			
			$tables = array();			
			$results = $this->_db->_select("SHOW TABLES");			
			foreach($results as $result) 				
				if($table=get_value(array_values($result),0)) 
					$tables[$table] = $table;		
			return $tables;		
		}		
		
        /**
         * Reutnrs table fields
         * @param string $tablename
         * @return array 
         */
		function get_table_fields($tablename) {
			return $this->get_table_columns($tablename);
		}
		
        /**
         * Returns table columns
         * @param string $tablename
         * @return array 
         */
		function get_table_columns($tablename) {
			return $this->_db->_select("SHOW FULL FIELDS FROM `".$tablename."`");		
		}		

        /**
         * Returns table column list
         * @param string $tablename
         * @return array 
         */
		function get_table_column_list($tablename) {
			return ARRAY_UTIL::get_list_from_array($this->get_table_columns($tablename),"Field");
		}		
			
        /**
         * Returns table field names
         * @param string $tablename
         * @return array 
         */
		function get_table_field_names($tablename) {
			$field_names = array();
			foreach($this->get_table_fields($tablename) as $table_field)
				$field_names[] = get_value($table_field,"Field");
			
			return $field_names;
		}

        /**
         * Returns table column names
         * @param string $tablename
         * @return array 
         */
		function get_table_column_names($tablename) {
			$columns = array();
			
			foreach(self::get_table_fields($tablename) as $field)
				$columns[get_value($field,"Field")] = get_value($field,"Field");
			
			return $columns;
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
         * @return boolean 
         */
		function grant_user($host,$database,$username,$password,$select=true,$update=true,$delete=true,$insert=true,$privs=array()) {
			
			if($select) $privs[] = "SELECT";
			if($update) $privs[] = "UPDATE";
			if($delete) $privs[] = "DELETE";
			if($insert) $privs[] = "INSERT";
			
			$priv_str = implode(",",$privs);
			
			$sql = "GRANT ".$priv_str." ON ".$database.".* TO '".$username."'@'".$host."' IDENTIFIED BY '".$password."';";
		
			return $this->_db->_query($sql);		
		}

        /**
         * revoke user
         * @param string $database
         * @param string $username
         * @param string $host
         * @param boolean $select
         * @param boolean $update
         * @param boolean $delete
         * @param boolean $insert
         * @return boolean 
         */
		function revoke_user($database,$username,$host,$select=false,$update=false,$delete=false,$insert=false) {
			
			$privs = array();
			
			if($select) $privs[] = "SELECT";
			if($update) $privs[] = "UPDATE";
			if($delete) $privs[] = "DELETE";
			if($insert) $privs[] = "INSERT";
			
			$privs = $privs ? $privs : array("ALL PRIVILEGES");
			
			$priv_str = implode(",",$privs);
			
			$sql = "REVOKE ".$priv_str." ON ".$database.".* FROM '".$username."'@'".$host."';";
		
			return $this->_db->_query($sql);
		}
			
        /**
         * load data file
         * @param string $file
         * @param string $tablename
         * @param string $fields_terminated_by
         * @param string $enclosed_by
         * @param boolean $optionally_enclosed
         * @param string $lines_terminated_by
         * @param string $ignore_lines
         * @param string $mapping - use @dummy for dummy field placeholders
         * @return boolean 
         */
		function load_data_file($file,$tablename,$fields_terminated_by=null,$enclosed_by=null,$optionally_enclosed=false,
					$lines_terminated_by=null,$ignore_lines=null,$mapping=null) {

			$file = FILE_UTIL::get_sanitized_path($file,"/");
			
			$parms[] = "LOAD DATA LOCAL INFILE '".$file."'";
			$parms[] = "INTO TABLE `".$tablename."`";
			
			if($fields_terminated_by)
				$parms[] = "FIELDS TERMINATED BY '".$fields_terminated_by."'";
			
			if($enclosed_by)
				$parms[] = ($optionally_enclosed ? "OPTIONALLY " : "")."ENCLOSED BY '".$enclosed_by."'";
				
			if($lines_terminated_by)
				$parms[] = "LINES TERMINATED BY '".$lines_terminated_by."'";
				
			if($ignore_lines)
				$parms[] = "IGNORE ".$ignore_lines." LINES";
				
			if($mapping)
				$parms[] = "(".implode(",",$mapping).")";	
				
			$sql = implode(" ",$parms);
			
			return $this->_db->_query($sql);
		}		
	}