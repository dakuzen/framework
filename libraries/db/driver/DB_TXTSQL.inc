<?	
        /**
        * Class DB_TXTSQL  
        *
        * @package Library
        * @subpackage Db
        */
	class DB_TXTSQL extends DB {
		
                /**
                * connect
                * @param string $db_data_dir
                * @param string $db_user
                * @param string $db_pass
                * @param string $db_name
                * @param array $options
                * @return boolean 
                */
		function connect($db_data_dir,$db_user,$db_pass,$db_name=null,$options=array()) {

			$connection = TXTSQL_UTIL::create_db_connection($db_data_dir,$db_user,$db_pass,$db_name);
			
			if($db_name)
				$connection->selectdb($db_name);

			$this->set_connection($connection);

			return true;
		}
		
                /**
                 * Return db name
                 * @return string 
                 */
		function get_db_name() 		{ return $this->get_connection()->_SELECTEDDB; }
		
                /**
                 * Return the error message
                 * @return string 
                 */
		function get_error_message() 	{ return $this->get_connection()->get_last_error(); }
		
                /**
                 * Delete
                 * @param object $dbq
                 * @param string $data
                 * @return boolean 
                 */
		function delete($dbq,$data) {			
			$parms["table"] 	= $dbq->get_tablename();
			return $this->get_connection()->delete($parms);	
		}
		
                /**
                 * Update
                 * @param object $dbq
                 * @param string $data
                 * @return boolean 
                 */
		function update($dbq,$data) {
			$parms["table"] 	= $dbq->get_tablename();
			$parms["values"] 	= $data;
			
			if($wheres=self::get_where_conditions($dbq))
				$parms["where"] = $wheres;			
			
			return $this->get_connection()->update($parms);	
		}		
		
                /**
                 * Insert
                 * @param object $dbq
                 * @param string $data
                 * @return mixed 
                 * @todo review return type & param type
                 */
		function insert($dbq,$data)	{
			$parms["table"] 	= $dbq->get_tablename();
			$parms["values"] 	= $data;
			
			return $this->get_connection()->insert($parms);	
		}
		
                /**
                 * replace
                 * @param object $dbq
                 * @param string $keys
                 * @param string $data 
                 */
		function replace($dbq,$keys,$data) {
			p("Not Implemented TXTSQL - replace"); 
		}
		
                /**
                 * Select
                 * @param object $dbq
                 * @param array $fields
                 * @param string $index_field
                 * @return array 
                 */
		function select($dbq,$fields,$index_field=null)	{

			$parms["table"] = $dbq->get_tablename();
			
			
			
			if($dbq->get_limit_start()>-1 && $dbq->get_limit_count()>-1)
				$parms["limit"] = array($dbq->get_limit_start(),$dbq->get_limit_start() + $dbq->get_limit_count());
			elseif($dbq->get_limit_count()>-1)
				$parms["limit"] = array(0,$dbq->get_limit_count());
			
			if($order=get_value($dbq->get_orders(),0)) 		
				$parms["orderby"] = array($order[0],$order[1]);
			
			if($wheres=self::get_where_conditions($dbq))
				$parms["where"] = $wheres;
			
			$result = $this->get_connection()->select($parms);
			
			return is_array($result) ? $result : array();	
		}
		
                /**
                 * Returns the where conditions
                 * @param object $dbq
                 * @return array 
                 */
		function get_where_conditions($dbq) {		
			$wheres = array();
			
			foreach($dbq->get_where_constraints() as $where_constraint) {

				if(get_class($where_constraint)=='DB_CONSTRAINT') {
					
					$wheres[] = self::get_where_condition($where_constraint);
					$wheres[] = $where_constraint->get_relation();
					
				} elseif(get_class($where_constraint)) {
				
					foreach($where_constraint->get_constraints() as $constraint) {
						$wheres[] = self::get_where_condition($constraint);
						$wheres[] = $constraint->get_relation();
					}	
				}				
			}
			
			unset($wheres[count($wheres)-1]);
			
			return $wheres;
		}
		
                /**
                 * Returns the where condition
                 * @param object $where_constraint
                 * @return mixed
                 * @todo review return type 
                 */
		function get_where_condition($where_constraint) {
			return DB::extract_column_name($where_constraint->get_field())." ".$where_constraint->get_operator()." ".implode(",",$where_constraint->get_value());
		}
		
                /**
                 * Select one
                 * @param object $dbq
                 * @param string $field
                 * @return mixed
                 * @todo review the return type 
                 */
		function select_one($dbq,$field) { 
			return get_value($this->select_row($dbq,array($field)),DB::extract_column_name($field),false);
		}
		
                /**
                 * Select row
                 * @param object $dbq
                 * @param array $fields
                 * @param string $index_field
                 * @return array 
                 */
		function select_row($dbq,$fields,$index_field=null) {
			$dbq->set_limit(1);
			return get_value($this->select($dbq,$fields),0,array());
		}				
		
                /**
                 * select column
                 * @param object $dbq
                 * @param string $field
                 * @param string $index_field 
                 */
		function select_column($dbq,$field,$index_field=null) {
			p("Not Implemented TXTSQL - select_column"); 
		}		
		
                /**
                 * Return select sql
                 * @param object $dbq
                 * @param array $fields 
                 */
		function get_select_sql($dbq,$fields) {
			p("Not Implemented TXTSQL - get_select_sql"); 
		}
		
                /**
                 * Query
                 * @param string $sql
                 * @param array $data 
                 */
		function _query($sql,$data=array()) {
			p("Not Implemented TXTSQL - _query"); 
		}
		
                /**
                 * Select one
                 * @param string $sql 
                 */
		function _select_one($sql) {
			p("Not Implemented TXTSQL - _select_one"); 		
		}
		
                /**
                 * Select row
                 * @param string $sql 
                 */
		function _select_row($sql) {
			p("Not Implemented TXTSQL - _select_row"); 		
		}		
		
                /**
                 * Select
                 * @param string $sql 
                 */
		function _select($sql) {
			p("Not Implemented TXTSQL - _select"); 	
		}
		
                /**
                 * disable foreign keys 
                 */
		function _disable_foreign_keys() {
			p("Not Implemented TXTSQL - _disable_foreign_keys"); 	
		}

                /**
                 * enable foreign keys
                 */
		function _enable_foreign_keys() {
			p("Not Implemented TXTSQL - _enable_foreign_keys"); 	
		}
		
                /**
                 * start transaction
                 */
		function _start_transaction()		{}
		
                /**
                 * has transaction failed
                 * @return boolean 
                 */
		function _has_transaction_failed() 	{ return false; }
		
                /**
                 * has transaction success
                 * @return boolean 
                 */
		function _has_transaction_success() 	{ return true; }
		
                /**
                 * complete transaction
                 */
		function _complete_transaction() 	{}
		
                /**
                 * fail transaction
                 */
		function _fail_transaction() 		{}
		
                /**
                 * Return db utility
                 * @param string $db_host
                 * @param string $db_user
                 * @param string $db_pass
                 * @param string $db_name
                 * @return UTILITY_TXTSQL 
                 */
		static function get_db_utility($db_host=null,$db_user=null,$db_pass=null,$db_name=null) {
			
			require_once(dirname(__FILE__)."/../DB_UTILITY.inc");	
			require_once(dirname(__FILE__)."/../utility/UTILITY_TXTSQL.inc");
			
			return new UTILITY_TXTSQL($db_host,$db_user,$db_pass,$db_name);
		}
	}	