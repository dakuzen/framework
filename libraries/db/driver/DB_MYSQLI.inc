<?
	if(extension_loaded("mysqli") && !class_exists("extend_mysqli")) {

        /**
        * Class extend_mysqli
        *
        * @package Library
        * @subpackage Db
        */
		class extend_mysqli extends mysqli {

			public function __construct($host,$user,$pass,$db=null,$options) {

				$port = get_value($options,"port") ? get_value($options,"port") : 3306;
				$sock = get_value($options,"sock");
				$host = $sock ? "" : $host;

				if(SERVER_UTIL::is_os_linux())
					$host = "p:".$host;

				@parent::init();

				foreach(value($options,"commands") as $command)
					@parent::options(MYSQLI_INIT_COMMAND,$command);

				@parent::options(MYSQLI_OPT_CONNECT_TIMEOUT,30);
				// Removed Use of undefined constant MYSQL_OPT_RECONNECT - assumed 'MYSQL_OPT_RECONNECT'
				//@parent::options(MYSQL_OPT_RECONNECT,1);
				@parent::real_connect($host,$user,$pass,$db,$port,$sock,MYSQLI_CLIENT_INTERACTIVE);
			}
		}
	}

    /**
    * Class DB_MYSQLI
    *
    * @package Library
    * @subpackage Db
    */
	class DB_MYSQLI extends DB {

		protected $_has_transaction_failed 		= false;
		protected $_transaction_starts			= 0;
		protected $_is_transactions				= true;

		function connect($db_host,$db_user,$db_pass,$db_name=null,$options=array()) {

			if(!$db_host && !get_value($options,"sock"))
				throw new BCMODEL_DB_EXCEPTION("No host or sock supplied");

			if(array_key_exists("disable_logging",$options))
				$this->disable_logging();

			if(!extension_loaded("mysqli"))
				throw new BCMODEL_DB_EXCEPTION("mySQLi extension not loaded");

			$commands = (array)value($options,"commands",array());

			$commands[] = "SET NAMES 'utf8';";
			$commands[] = "SET SESSION sql_auto_is_null = 0;";

			$options["commands"] = $commands;

			if($db_name!==null)
				$connection = new extend_mysqli($db_host,$db_user,$db_pass,$db_name,$options);
			else
				$connection = new extend_mysqli($db_host,$db_user,$db_pass,null,$options);

			if(mysqli_connect_errno())
				throw new BCMODEL_DB_EXCEPTION("DB Connection failed: ".@mysqli_connect_error());

			$connection->set_charset("utf8");

			$this->set_connection($connection);
			$this->set_db_name($db_name);

			return true;
		}

		function close() {
			if($connection=$this->get_connection())
				$connection->close();
		}

		function delete($dbq,$tables) {

			$tables = is_array($tables) ? $tables : array($tables);

			$where_clause = $dbq->get_where_clause($dbq->get_where_constraints());

			$tables = implode(",",$tables);

			$delete_extras = implode(" ",array($dbq->get_tablename(),$dbq->get_join_clause(),$where_clause));

			$query = "DELETE ".$tables." FROM ".$delete_extras;

			return $this->_query($query,$dbq->get_where_data());
		}

		function update($dbq,$data) {

			$values = array();
			foreach($data as $key=>$field) {

				if(!is_array($field)) {

					if(strpos($key, '.')!==false) {
						list($table,$field) = explode('.', $key);
						$key = "`".$table."`.`".$field."`";
					} else
						$key = "`".$key."`";

					$values[] = $key." = ".DB::PARM_CHARACTER;

				} elseif(($parm=value($field,"parm")) || is_a($parm,"DBQ_PARM")) {

					if(is_a($parm,"DBQ_PARM"))
						$parm = $parm->get_parm();

					if(strpos($key, '.')===false) {
						$values[] = "`".$key."` = ".$parm;
					} else {
						$values[] = $key." = ".$parm;
					}
				}
			}

			$query =  "UPDATE ".$dbq->get_tablename_aliased(true)." ".$dbq->get_join_clause(). " SET ".implode(",",$values)." ".$dbq->get_where_clause($dbq->get_where_constraints());

			$where_data = array();
			foreach($data as $key=>$value)
				if(!is_array($value))
					$where_data[$key] = $value;

			$data = array_merge($where_data,$dbq->get_where_data());

			return $this->_query($query,$data);
		}

		function insert($dbq,$data) {

			$query = $this->get_insert_sql($dbq,$data).";";

			$has_success = $this->_query($query,$data);

			if($has_success)
				$this->set_insert_id($this->_connection->insert_id);

			return $has_success;
		}

		function get_insert_sql($dbq,$data) {

			$sets = $fields = array();

			if(!is_array(value($data,0)))
				$data = array($data);

			foreach($data as $row) {
				$fields = $values = array();

				foreach($row as $field=>$item) {

					$value = DB::PARM_CHARACTER;

					if(is_array($value) && isset($item["parm"]))
						$value = "'".$item["parm"]."'";

					$fields[] = $field;
					$values[] = $value;
				}

				$sets[] = implode(',',$values);
			}

			foreach($fields as &$field)
				$field = "`".$field."`";

			return 'INSERT INTO `'.$dbq->get_tablename().'` ('.implode(',',$fields).') VALUES ('.implode('),(',$sets).')';
		}

		function replace($dbq,$keys,$data) {

			// keys is array('fieldname'=> 'value')
			// data is array('fieldname' => 'value')

			foreach($keys as $key=>$item)
				if(!array_key_exists($key,$data))
					$data[$key] = stripslashes($item);

			$fields = $values = array();

			foreach($data as $field=>$item) {
				$fields[] = "`".$field."`";
				$values[] = DB::PARM_CHARACTER;
			}

			$query = 'REPLACE INTO `'.$dbq->get_tablename().'` ('.implode(',',$fields).') VALUES ('.implode(',',$values).');';

			return $this->_query($query,$data);
		}

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

			$data = $dbq->get_where_data();

			if(!value($options,"disable_having"))
				$data = array_merge($data,$dbq->get_having_data());

			if($result=$this->_query($query,$data)) {

				while($row = $result->fetch_assoc()) {

					if($multi_row) {
						if($index_field) {
							$index_value = value($row,$index_field);

							$rows[$index_value] = $row;
						} else
							$rows[] = $row;
					} else {
						$fields = array_values($fields);

						foreach($fields as &$field)
							$field = self::extract_column_name($field);

						if($index_field) {

							$index_value = value($row,$index_field);
							unset($fields[$index_field]);

							$rows[$index_value] = value($row,value($fields,0));
						} else
							$rows[] = value($row,value($fields,0));
					}
				}

				$result->close();
			}

			return $rows;
		}

		function get_select_sql($dbq,$fields,$options=array()) {

			$fields = is_array($fields) ? $fields : array($fields);

			$select_fields = count($fields)>0 ? $fields : array("*");

			if($dbq->get_distinct_field()!=null) {
				for($i=0;$i<count($select_fields);$i++)
					if($dbq->get_distinct_field()==$select_fields[$i])
						unset($select_fields[$i]);

				$select_fields = array_merge(array("DISTINCT ".$dbq->get_distinct_field()),$select_fields);
			}

			$join_clause = $dbq->get_join_clause();

			$where_clause = $dbq->get_where_clause($dbq->get_where_constraints(),get_value($options,"parm"));

			$group_clause = $dbq->get_group_by_clause();

			$having_clause = value($options,"disable_having") ? "" : $dbq->get_having_clause($dbq->get_having_constraints());

			$order_clause = value($options,"disable_order_by") ? "" : $dbq->get_order_by_clause();

			$limit_clause = "";

			$limit_rows 	= $dbq->get_limit_count() <0 ? null : $dbq->get_limit_count();
			$offset 		= is_null($dbq->get_limit_start()) ? null : $dbq->get_limit_start();

			$limit_options = array();

			if($offset!==null)
				$limit_options[] = $offset;

			if($limit_rows!==null)
				$limit_options[] = $limit_rows;

			if($limit_options)
				$limit_clause = " LIMIT ".implode(",",$limit_options);

			$select_extras = array_filter(array($dbq->get_tablename_aliased(true),$join_clause,$where_clause,$group_clause,$having_clause,$order_clause,$limit_clause), "strlen");

			$select_extras = implode(" ",$select_extras);

			// not sure why % are being escaped, but it plays funny with things like DATE_FORMAT(col,'%Y')
			//foreach($select_fields as &$field)
			//	$field = preg_replace("/%{1}/","%%",$field);

			return "SELECT ".implode(",",$select_fields)." FROM ".$select_extras;
		}

		function select_one($dbq,$field,$options=array()) {
			$fields = is_array($field) ? $field : array($field);
			$dbq = $dbq->clone()->set_limit(1,$dbq->get_limit_start());
			$result = get_value($this->select($dbq,$fields,null,true,$options),0,array());
			return get_value(array_values($result),0,false);
		}

		function select_count($dbq,$field) {

			if(!$field) {
				$field = is_string($dbq->get_primary_key()) ?  $dbq->get_tablename_alias().".".$dbq->get_primary_key() : "*";
				$field = "COUNT(".$field.")";
			}

			$fields = is_array($field) ? $field : array($field);

			return $this->select_one($dbq,$fields,array("disable_order_by"=>true,"disable_having"=>true));
		}

		function select_row($dbq,$fields,$index_field=false) {
			$dbq = $dbq->clone()->set_limit(1);
			$result = $this->select($dbq,$fields,$index_field);
			return get_value(array_values($result),0,array());
		}

		function select_column($dbq,$field,$index_field=false) {
			return $this->select($dbq,array($field),$index_field,false);
		}

		function _script($script,$data=array()) {
			$queries = explode(";",trim(trim($script),";"));

			foreach($queries as $query)
				$this->_query($query);
		}

		function _query($query,$data=array()) {

			$data = self::get_prepared_data($data);

			$sql = $this->get_sql($query,$data);

			$result = false;

			if($this->is_debug()) {
				if(DEBUG_UTIL::is_format_html())
					echo CMODEL_SQLFORMATTER::format($sql)."<hr/>";
				else
					echo CMODEL_SQLFORMATTER::format($sql,false)."\n\n";
				flush();
			}

			$start_time = microtime(true);
			$error_number = null;

			if($this->_connection) {

				$result = $this->_connection->query($sql);

				$this->_error_message = @mysqli_connect_error();
				$error_number = @mysqli_errno($this->_connection);

				if(!$this->_error_message)
					$this->_error_message = @mysqli_error($this->_connection);
			} else
				$this->_error_message = "No db connection exists";

			$end_time = microtime(true);

			$time = round($end_time - $start_time,5);

			if($this->is_debug_extend()) {
				echo $time." seconds".(DEBUG_UTIL::is_format_html() ? "<hr/>" : "\n\n");
				flush();
			}

			if($this->_error_message)
				$this->event(self::EVENT_ERROR,$this->_error_message,$time);
			else
				$this->event(self::EVENT_QUERY,$sql,$time);

			// To preserve the error message variable which can be overrriten by the DEBUG_UTIL::display_error() call
			$error_message = $this->_error_message;

			if(!$result || $this->_error_message) {

				if($this->_is_logging) {

					if($this->config("error_method")==self::ERROR_METHOD_TRIGGER_ERROR)
						trigger_error("Database Error: ".$error_number." - ".$this->_error_message."  ".$sql,E_USER_ERROR);

					elseif($this->config("error_method")==self::ERROR_METHOD_EXCEPTION)
						throw new BCMODEL_DB_EXCEPTION("Database Error: ".$this->_error_message."  ".$sql,$error_number);
				}

				if($this->_transaction_starts>0)
					$this->_has_transaction_failed = true;
			}

			$this->_error_message = $error_message;

			return $result;
		}


		protected function get_sql($query,$data) {

			if($data) {

				$query_data = array();

				foreach($data as $item) {
					if(is_array($item))
						$query_data = array_merge($query_data,array_values($item));
					else
						$query_data[] = $item;
				}

				$parts = explode(DB::PARM_CHARACTER,$query);

				if($parts) {

					if((count($parts)-1)==count($query_data)) {

						$query = "";
						foreach($parts as $index=>$part)
							$query .= $part.get_value($query_data,$index);

					}
				}
			}

			return $query;
		}

		protected static function get_prepared_data($data) {

			if(!is_array($data)) return array();

			foreach($data as &$item) {

				if(is_array($item))
					$item = self::get_prepared_data($item);
				elseif($item===null)
					$item = "null";
				elseif(is_bool($item))
					$item = $item ? "1" : "0";
				elseif(is_a($item,"DBQ_PARM"))
					$item = $item->get_parm();
				else
					$item = "'".addslashes($item)."'";
			}

			return $data;
		}

		static function extract_column_name($column) {

			if(($as_pos=strripos($column," AS "))!==false)
				$column = trim(substr($column,$as_pos+4));

			// Only process columns with the format "tablename.column"
			elseif(preg_match("/^[^\s]+\.[^\s]+$/",$column)) {
				$column_parts = explode(".",$column);
				$column = get_value($column_parts,1,get_value($column_parts,0));
			}

			return $column;
		}

		function _select($sql) {
			$rows = array();

			if($result=$this->_query($sql)) {

				while($row = $result->fetch_assoc())
					$rows[] = $row;

				$result->close();
			}

			return $rows;
		}

		function _select_one($sql) {
			$row = $this->_select_row($sql);
			return get_value(array_values($row),0,false);
		}

		function _select_row($sql) {
			return get_value(array_values($this->_select($sql)),0,array());
		}

		function _start_transaction() {

			if($this->has_transactions()) {
				if($this->_is_transactions)
					if($this->_transaction_starts==0)
						$this->_query("BEGIN");

				$this->_transaction_starts++;
			}
		}

		function _has_transaction_failed() {

			return $this->_has_transaction_failed;
		}

		function _has_transaction_success() {
			return !$this->has_transaction_failed();
		}

		function _complete_transaction() {

			$has_success = $this->_has_transaction_success();

			if($this->has_transactions()) {
				$this->_transaction_starts--;

				if($this->_transaction_starts==0) {

					if($this->_is_transactions) {

						if(!$this->_has_transaction_failed)
							$this->_query("COMMIT");
						else
							$this->fail_transaction();
					}
				}
			}

			$this->_has_transaction_failed = false;

			return $has_success;
		}

		function _fail_transaction() {

			$this->_has_transaction_failed = true;

			if($this->_is_transactions)
				$this->_query("ROLLBACK");
		}

		function disable_table_keys($tablename) {
			return $this->_query("ALTER TABLE `".$tablename."` DISABLE KEYS");
		}

		function enable_table_keys($tablename) {
			return $this->_query("ALTER TABLE `".$tablename."` ENABLE KEYS");
		}

		function enable_lock_table_write($tablename) {
			return $this->_query("LOCK TABLE ".$tablename." WRITE");
		}

		function disable_lock_table() {
			return $this->_query("UNLOCK TABLES");
		}

		function enable_unique_check() {
			return $this->_query("SET UNIQUE_CHECKS=1");
		}

		function disable_unique_check(){
			return $this->_query("SET UNIQUE_CHECKS=0");
		}

		function _disable_foreign_keys() {
			return $this->_query("SET FOREIGN_KEY_CHECKS=0");
		}

		function _enable_foreign_keys() {
			return $this->_query("SET FOREIGN_KEY_CHECKS=1");
		}

		function ping() {
			$this->_connection->ping();
		}

		static function column_name_exists($columns,$column_name) {
			$absolute_column_name 	= $column_name;
			$column_name 		= DB::extract_column_name($column_name);

			foreach($columns as $column)
				if(preg_match("/(AS\s+|^)".$column_name."\s*$/i",$column))
					return true;

			return false;

			return in_array($absolute_column_name,$columns) || in_array($column_name,$columns);
		}

		static function get_db_utility($db_host=null,$db_user=null,$db_pass=null,$db_name=null,$options=array()) {
			require_once(dirname(__FILE__)."/../DB_UTILITY.inc");
			require_once(dirname(__FILE__)."/../utility/UTILITY_MYSQLI.inc");
			return new UTILITY_MYSQLI($db_host,$db_user,$db_pass,$db_name,$options);
		}
	}