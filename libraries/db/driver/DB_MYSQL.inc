<?
       /**
        * DB_MYSQL class file  
        *
        * @package Library
        * @subpackage Db
        */

	require_once("DB_MYSQLI.inc");

       /**
        * Class DB_MYSQL  
        *
        * @package Library
        * @subpackage Db
        */
	class DB_MYSQL extends DB_MYSQLI {
		
                /**
                 * Connect
                 * @param string $db_host
                 * @param string $db_user
                 * @param string $db_pass
                 * @param string $db_name
                 * @param array $options
                 * @return boolean
                 * @throws Exception 
                 */
		function connect($db_host,$db_user,$db_pass,$db_name=null,$options=array()) {

			if(!extension_loaded("mysql"))
				throw new Exception("mySQL extension not loaded");

			$sock = get_value($options,"sock","");
			$db_host = $sock ? $sock : $db_host;
			
			$link = @mysql_connect($db_host,$db_user,$db_pass,true);

			if(!$link)
				throw new Exception("Connect failed: ".@mysql_error());

			if(!mysql_select_db($db_name,$link))
				throw new Exception("Failed to select DB: ".@mysql_error());

			$this->set_connection($link);
			$this->set_db_name($db_name);

			return true;
		}
		
                /**
                 * select
                 * @param string $dbq
                 * @param array $fields
                 * @param string $index_field
                 * @param boolean $multi_row
                 * @param array $options
                 * @return array 
                 */
		function select($dbq,$fields,$index_field=null,$multi_row=true,$options=array()) {
		
			if($index_field) {
				
				if(!is_string($index_field) && is_string($dbq->get_primary_key()))
					$index_field = $dbq->get_tablename().".".$dbq->get_primary_key();								
			
				if(!self::column_name_exists($fields,$index_field))
					$fields[] = $index_field;
			}
			
			$query = $this->get_select_sql($dbq,$fields,$options);
					
			$index_field = $index_field ? self::extract_column_name($index_field) : false;
			
			$rows = array();
			
			$data = array_merge($dbq->get_where_data(),$dbq->get_having_data());
			
			if($result=$this->_query($query,$data)) {
			
				while($row = mysql_fetch_assoc($result)) {
				
					if($multi_row) {
						if($index_field) {
							$index_value = get_value($row,$index_field);

							$rows[$index_value] = $row;
						} else 
							$rows[] = $row;
					} else {
						$fields = array_values($fields);
						
						foreach($fields as &$field)
							$field = self::extract_column_name($field);
						
						if($index_field) {
							
							$index_value = get_value($row,$index_field);
							unset($fields[$index_field]);
								
							$rows[$index_value] = get_value($row,get_value($fields,0));
						} else 
							$rows[] = get_value($row,get_value($fields,0));						
					}
				}
				
				mysql_free_result($result);
			}
			
			return $rows;		
		}	
		
                /**
                 * Insert
                 * @param object $dbq
                 * @param array $data
                 * @return boolean 
                 */
		function insert($dbq,$data) {
			
			$sets = $fields = array();
			
			if(!is_array(get_value($data,0)))
				$data = array($data);

			foreach($data as $d){
				$values = array();
				$fields = array();
				foreach($d as $field=>$item) {
					$fields[] = $field;
					$values[] = DB::PARM_CHARACTER;
				}
				$sets[] = implode(',',$values);
			}
			
			foreach($fields as &$field)
				$field = "`".$field."`";

			$query = 'INSERT INTO '.$dbq->get_tablename().' ('.implode(',',$fields).') VALUES ('.implode('),(',$sets).');';
			
			$has_success = $this->_query($query,$data);
			
			if($has_success)
				$this->set_insert_id(mysql_insert_id($this->_connection));
			
			return $has_success;
		}		
		
                /**
                 * Select
                 * @param string $sql
                 * @return array 
                 */
		function _select($sql) {
			$rows = array();
			
			if($result=$this->_query($sql)) {
				while($row = mysql_fetch_assoc($result)) 
					$rows[] = $row;
				
				mysql_free_result($result);
			}
			
			return $rows;	
		}
		
                /**
                 * query
                 * @param string $query
                 * @param array $data
                 * @return boolean 
                 */
		function _query($query,$data=array()) {
					
			$data = self::get_prepared_data($data);	
			
			$sql = $this->get_sql($query,$data);			
			
			$result = false;
			
			if($this->_connection) {
				
				$result = mysql_query($sql,$this->_connection);
				
				$this->_error_message = @mysql_error($this->_connection);

				// To preserve the error message variable which can be overrriten by the DEBUG_UTIL::display_error() call
				$error_message = $this->_error_message;

				if(!$result || $this->_error_message) {
					
					if($this->_is_logging) {
						
						$values = "";
						if($data) {

							$data_items = array();
							foreach($data as $key=>$value) {
								$value = is_array($value) ? implode(",",$value) : $value;
								$data_items[] = $key." => ".STRING_UTIL::get_dot_dot_dot_string($value,50);
							}

							$values = " with values ".implode(", ",$data_items);
						}
					
						DEBUG_UTIL::display_error(E_ERROR, "DB Database Layer: ".$sql.$values."\n".$this->_error_message, __FILE__, "N/A", null);
					}
				}

				$this->_error_message = $error_message;			
			} else 
				$this->_error_message = "No db connection exists";
			
			if(DB::is_debug())
				echo $sql.(DEBUG_UTIL::is_format_html() ? "<hr/>" : "\n\n");
						
			return $result;
		}		
	}	