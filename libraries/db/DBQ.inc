<?
   /**
    * DBQ class file
    *
    * @package Library
    * @subpackage Db
    */

	require_once("DB.inc");
	require_once("DB_CONSTRAINT.inc");

   /**
    * Class DBQ
    *
    * @package Library
    * @subpackage Db
    */
	class DBQ extends OBJECT {

		const ORDER_ASC 		= "ASC";
		const ORDER_DESC 		= "DESC";
		const PARM_CHARACTER	= DB::PARM_CHARACTER;

        /**
         * The db.
         * @var string $db
         * @todo the type (string) needs validation
         */
		protected $_db		 			= null;

		/**
		* The primary key.
		* @var string $primary_key
		*/
		protected $_primary_key 		= null;

		/**
		* The tablename.
		* @var string $tablename
		*/
		protected $_tablename 			= null;

		/**
		* The tablename alias.
		* @var string $tablename_alias
		*/
		protected $_tablename_alias 	= null;

		/**
		* The last inserted id.
		* @var integer $last_inserted_id
		*/
		protected $_last_inserted_id 	= null;

		/**
		* The distinct field.
		* @var string $distinct_field
		*/
		protected $_distinct_field		= null;

		/**
		* The where constraints.
		* @var array $where_constraints
		*/
		protected $_where_constraints 	= array();

		/**
		* The having constraints.
		* @var array $having_constraints
		*/
		protected $_having_constraints 	= array();

		/**
		* The orders.
		* @var array $orders
		*/
		protected $_orders 				= array();

		/**
		* The group by.
		* @var array $group_by
		*/
		protected $_group_by 			= array();

		/**
		* The db joins.
		* @var array $db_joins
		*/
		protected $_db_joins			= array();

		/**
		* The limit count.
		* @var integer $limit_count
		*/
		protected $_limit_count 		= null;

		/**
		* The login.
		* @var string $login
		*/
		protected $_limit_start 		= null;

		static function create($tablename="",$primary_key=null,$db=null,$debug=false) {
			$class = get_called_class();
		 	return new $class($tablename,$primary_key,$db,$debug);
		}

		/**
		* Constructor.
		* @param string $tablename the table name.
		* @param integer $primary_key the primary key.
		* @param string $db the database.
		* @param boolean $debug wheather to debug.
		*/
		function __construct($tablename="",$primary_key=null,$db=null,$debug=false) {
			$this->_tablename 	= $tablename;
			$this->_primary_key = $primary_key;

			if($db)
				$this->set_db_connection($db);
			else
				$this->set_db_connection(DB::get_default_db_connection());

			if($debug)
				$this->_db->set_debug(true);
		}

        /**
         * Initialize.
         *
         */
		function initialize() {
			$this->_last_inserted_id 	= null;
			$this->_group_by 			= array();
			$this->_orders 				= array();
			$this->_where_constraints 	= array();
			$this->_having_constraints 	= array();
			$this->_limit_count 		= null;
			$this->_limit_start 		= null;
			$this->_db_joins			= array();
			return $this;
		}

		function get_db_joins() 					{ return $this->_db_joins; }

		function set_db_joins($value) 				{ $this->_db_joins = $value; }
		function set_where_constraints($value) 		{ $this->_where_constraints = $value; }

		/**
		* Clear database joins.
		*
		*/
		function clear_db_joins() 			{ $this->set_db_joins(array()); }

		/**
		* Clear database groups.
		*
		*/
		function clear_db_groups() 			{ $this->_where_constraints = array(); }

		/**
		* Clear constraints.
		*
		*/
		function clear_constraints() 			{ $this->_where_constraints = array(); }

		/**
		* Clear orders.
		*
		*/
		function clear_orders() 			{ $this->_orders = array(); }

		/**
		* Disable logging.
		*
		*/
		function disable_logging() 			{ $this->_db->disable_logging(); }

		/**
		* Debug.
		*
		* @param boolean $debug whether to debug
		*/
		function debug($debug=true) 			{ $this->_db->set_debug($debug); }

		/**
		* Has error.
		*
		*/
		function has_error() 				{ return $this->_db->has_error(); }

		/**
		* Get constraint count.
		*
		* @return integer the constraints
		*/
		function get_constraint_count() 		{ return count($this->_where_constraints); }

		/**
		* Get table name.
		*
		* @return string the table name
		*/
		function get_tablename()			{ return $this->_tablename; }

		/**
		* Get table name alias.
		*
		* @return string the table name alias
		*/
		function get_tablename_alias() 			{ return $this->_tablename_alias==null ? $this->_tablename : $this->_tablename_alias; }

		/**
		* Get table name with the alias if is different from table name.
		*
		* @return string the name
		*/
		function get_tablename_aliased($quote=false) {

			if($this->get_tablename_alias()!=$this->get_tablename())
				return $this->get_tablename()." ".$this->get_tablename_alias();

			if($quote && !preg_match("/select/i",$this->get_tablename()))
				return "`".$this->get_tablename()."`";

			return $this->get_tablename();
		}

		/**
		* Get the last inseted id.
		*
		* @return integer the id
		*/
		function get_last_inserted_id() 		{ return $this->_last_inserted_id; }

		/**
		* Get the error message.
		*
		* @return string the error message
		*/
		function get_error_message() 			{ return $this->_db->get_error_message(); }

		/**
		* Get the primary key.
		*
		* @return integer the primary key
		*/
		function get_primary_key() 			{ return $this->_primary_key; }

		/**
		* Get the database connection.
		* @todo the return type
		* @return integer the database connection
		*/
		function get_db_connection()			{ return $this->_db; }

		/**
		* Get the where constraints.
		*
		* @return array the where constraints
		*/
		function get_where_constraints()		{ return $this->_where_constraints; }

		/**
		* Get the having constraints.
		*
		* @return array the having constraints
		*/
		function get_having_constraints()		{ return $this->_having_constraints; }

		/**
		* Get distinct field
		* @return string
		*/
		function get_distinct_field()			{ return $this->_distinct_field; }

		/**
		* Get limit count
		* @return integer
		*/
		function get_limit_count()			{ return $this->_limit_count; }

		/**
		* Get limit start
		* @return integer
		*/
		function get_limit_start()			{ return $this->_limit_start; }

		/**
		* Get orders
		* @return array
		*/
		function get_orders()				{ return $this->_orders; }

		/**
		* Sets distinct field
		* @param mix $value
		*/
		function set_distinct_field($value) 		{ $this->_distinct_field = $value; }

		/**
		* Set table name alias
		* @param string $value
		*/
		function set_tablename_alias($value) 		{ $this->_tablename_alias = $value; }

		/**
		* Get parm holder
		* @return string
		*/
		static function get_parm_holder() 		{ return self::PARM_CHARACTER; }

		/**
		* Set the database connection
		* @param object $db
		* @throws Exception
		*/
		function set_db_connection($db) {
			if(!$db)
				throw new Exception("Failed to get a valid db connection");
			$this->_db = $db;
		}


		public static function __callStatic($function,$args) {

			if($function=="create_subquery") {
				$class = get_called_class();
				return DB_SUBQUERY::create(new $class(),value($args,0));
			}

			trigger_error("Call to undefined method ".get_class($this)."::".$function."()", E_USER_ERROR);
		}

		public function __call($function,$args) {

			if($function=="create_subquery")
				return DB_SUBQUERY::create($this,value($args,0));

			return parent::__call($function,$args);
		}

        /**
         * Insert data
         * @param mix $data
         * @todo check data type
         * @return boolean
         */
		function insert($data) {

			$has_success = $this->_db->insert($this,$data);

			if($has_success)
				$this->_last_inserted_id = $this->_db->get_insert_id();

			return $has_success;
		}

        /**
         * Select command
         *
         * @param array $fields
         * @param boolean $index_field
         * @todo check return type
         * @return object
         */
		function select($fields=array(),$index_field=false) {
			return $this->all($fields,$index_field);
		}

		function all($fields=array(),$index_field=false) {
			return $this->_db->select($this,$fields,$index_field);
		}

        /**
         * Select column
         *
         * @param string $field
         * @param boolean $index_field
         * @todo check return type
         * @return object
         */

        function select_column($field,$index_field=false) {
			return $this->column($field,$index_field);
		}

		function column($field,$index_field=false) {
			return $this->_db->select_column($this,$field,$index_field);
		}

        /**
         * Selects a row
         *
         * @param array $fields
         * @todo check return type
         * @return object
         */

        function select_row($fields=array()) {
			return $this->row($fields);
		}

		function row($fields=array()) {
			return $this->_db->select_row($this,$fields);
		}

        /**
         * Selects one row
         * @param string $field
         * @return mix
         */

        function select_one($field) {
			return $this->one($field);
		}

		function one($field) {
			return $this->_db->select_one($this,$field);
		}

        /**
         * Select count
         * @param string $field
         * @return integer
         */
		function select_count($field=null) {
			return $this->_db->select_count($this,$field);
		}

        /**
         * Delete
         * @param array $tables
         * @return boolean
         */
		function delete($tables=array()) {
			return $this->_db->delete($this,$tables);
		}

        /**
         * Truncate
         * @todo return type
         * @return boolean
         */
		function truncate() {
			return $this->_db->truncate($this->_tablename);
		}

        /**
         * Update
         * @param array $data
         * @return boolean
         */
		function update($data) {
			return $this->_db->update($this,$data);
		}

        /**
         * Replace
         * @param array $keys
         * @param array $data
         * @return boolean
         */
		function replace($keys, $data) {
			return $this->_db->replace($this,$keys,$data);
		}

        /**
         * Count
         * @param string $field
         * @return integer
         */
		function count($field=null) {
			return (int)$this->select_count($field);
		}

        /**
         * Exists
         * @param string $field
         * @return boolean
         */
		function exists($field=1) {
			if(!$field)
				$field = is_array($this->get_primary_key()) ? "*" : $this->get_tablename_alias().".".$this->get_primary_key();
			return $this->select_one($field)!==false;
		}

		/**
         * Generate Functions
         * @param array $query_con_array
         * @return string
		 */
		function generate_where_clause($query_con_array,$parm=null) {
			if(count($query_con_array)==0)
				return "";

			$group_str = "";

			foreach($query_con_array as $idx=>$constraint) {
				switch(get_class($constraint)) {
					case "DB_CONSTRAINT":
						$relation = $idx>0 ? " ".$constraint->get_relation()." " : "";
						$group_str .= $relation.$constraint->get_condition($parm);
						break;

					case "DB_GROUP":
						$relation = $idx>0 ? " ".$constraint->get_relation()." " : "";

						$constraint_string = $this->generate_where_clause($constraint->get_constraints(),$parm);

						if(count($constraint->get_constraints())>1)
							$constraint_string = "(".$constraint_string.")";

						$group_str .= $relation.$constraint_string;
						break;
				}
			}
			return $group_str;
		}

        /**
         * generate having clause
         * @param array $query_con_array
         * @return string
         */
		function generate_having_clause($query_con_array) {
			if(count($query_con_array)==0) return "";
			$group_str = "";

			foreach($query_con_array as $idx=>$constraint) {
				switch( get_class($constraint) ){
					case 'DB_CONSTRAINT':
						$relation = $idx>0 ? " ".$constraint->get_relation()." " : "";
						$group_str .= $relation.$constraint->get_condition();
						break;

					case 'DB_GROUP':
						$relation = $idx>0 ? " ".$constraint->get_relation()." " : "";
						$group_str .= $relation."(".$this->generate_having_clause($constraint->get_constraints(), false).")";
						break;
				}
			}
			return $group_str;
		}


		/**
		 * Get select data
         * @return array
		 */
		function get_select_data() {
			return array_merge($this->get_where_data(),$this->get_having_data());
		}

        /**
         * Get where data
         * @return array
         */
		function get_where_data() {
			return $this->get_constraint_data($this->_where_constraints);
		}

        /**
         * Get having data
         * @return array
         */
		function get_having_data() {
			return $this->get_constraint_data($this->_having_constraints);
		}

        /**
         * Get constraint data
         * @param array $constraints
         * @return array
         */
		private function get_constraint_data($constraints) {
			$data = array();
			foreach($constraints as $constraint) {

				switch( get_class($constraint) ) {
					case 'DB_CONSTRAINT':
						if($constraint->parms() && is_array($constraint->get_value()))
							foreach($constraint->get_value() as $value)
								$data[] = $value;
						break;

					case 'DB_GROUP':
						$data = array_merge($data, $this->get_constraint_data($constraint->get_constraints()));
						break;
				}
			}

			return $data;
		}

        /**
         * Get where clause
         * @param array $constraints
         * @return string
         */
		function get_where_clause($constraints,$parm=null) {
			$where = self::generate_where_clause($constraints,$parm);
			return $where ? "WHERE ".$where : "";
		}

        /**
         * Get having clause
         * @param array $constraints
         * @return string
         */
		function get_having_clause($constraints) {
			$having = self::generate_having_clause($constraints);
			return $having ? "HAVING ".$having : "";
		}

        /**
         * Get select sql
         * @param array $fields
         * @return boolean
         */
		function get_select_sql($fields) {
			return $this->_db->get_select_sql($this,$fields);
		}

		function get_insert_sql($data) {
			return $this->_db->get_insert_sql($this,$data);
		}

		function sql($data,$operation="select") {
			if($operation=="select")
				return $this->_db->get_select_sql($this,$data,array("parm"=>false));
		}

        /**
         * Get order by clause
         * @return string
         */
		function get_order_by_clause() {
			if(!$this->_orders)
				return "";

			$order_bys = array();
			foreach($this->_orders as $order)
				$order_bys[] = $order[0]." ".$order[1];

			return " ORDER BY ".implode(",",$order_bys);
		}

        /**
         * Get group by clause
         * @return string
         */
		function get_group_by_clause() {
			if(count($this->_group_by)==0)
				return "";

			return " GROUP BY ".implode(",",$this->_group_by);
		}

        /**
         * Add join
         * @param string $table1_name
         * @param string $table2_name
         * @param string $table1_column
         * @param string $table2_column
         * @param string $join
         * @return object
         */
		function join($table1_name,$table2_name,$table1_column,$table2_column=null,$join="INNER") {
			$this->add_db_join(new DB_JOIN($table1_name,$table2_name,$table1_column,$table2_column,$join));
			return $this;
		}

		function add_join($table1_name,$table2_name,$table1_column,$table2_column=null,$join="INNER") {
			return $this->join($table1_name,$table2_name,$table1_column,$table2_column,$join);
		}

        /**
         * Add db join
         * @param DB_JOIN $db_join object
         * @return object
         */
		function db_join(DB_JOIN $db_join) {
			$this->_db_joins[] = $db_join;
			return $this;
		}

		function add_db_join(DB_JOIN $db_join) {
             return $this->db_join($db_join);
        }

        /**
         * Add db joins
         * @param array $db_joins
         * @return object
         */
		function add_db_joins($db_joins) {
			$this->_db_joins = array_merge($this->_db_joins,$db_joins);
			return $this;
		}

        /**
         * Set limit
         * @param integer $count
         * @param integer $start
         * @return object
         */
		function set_limit($count,$start=null) {
			$this->_limit_count = $count;
			$this->_limit_start = $start;
			return $this;
		}

		function limit($count,$start=null) { return $this->set_limit($count,$start); }

        /**
         * Add order by
         * @param string $field
         * @param string $direction
         * @return object
         */
		function add_order_by($field,$direction=DBQ::ORDER_ASC) {
			$this->_orders[] = array($field,$direction);
			return $this;
		}

		function order($field,$direction=DBQ::ORDER_ASC) { return $this->add_order_by($field, $direction); }

        /**
         * Add group by
         * @param string $field
         * @return object
         */
		function group_by($field) {
			$this->_group_by[] = $field;
			return $this;
		}

		function add_group_by($field) {
			return $this->group_by($field);
		}


        /**
         * Set group by
         * @param string $group_by
         * @return object
         */
		function set_group_by($group_by) {
			$this->_group_by = $group_by;
			return $this;
		}

        /**
         * Get date
         * @param integer $time
         * @return string
         */
		static public function get_date($time=null) {
			$time = func_num_args()==0 ? time() : $time;
			return DBQ::get_formatted_time("Y-m-d",$time);
		}

        /**
         * Get date time
         * @param integer $time
         * @return string
         */
		static public function get_date_time($time=null) {
			$time = func_num_args()==0 ? time() : $time;
			return DBQ::get_formatted_time("Y-m-d H:i:s",$time);
		}

        /**
         * Get formatted time
         * @param string $format
         * @param integer $time
         * @return string
         */
		static public function get_formatted_time($format,$time) {
			if(!is_numeric($time))
				return null;
			elseif($time<=0)
				return null;
			return date($format,$time);
		}

        /**
         * Adds inner join
         * @param string $table2_name
         * @param string $table1_column
         * @param string $table2_column
         * @return object
         */
		function add_inner_join($table2_name,$table1_column,$table2_column=null) {
			$this->add_join($this->get_tablename_alias(),$table2_name,$table1_column,($table2_column==null ? $table1_column : $table2_column));
			return $this;
		}

		function inner($table2_name,$table1_column,$table2_column=null) { return $this->add_inner_join($table2_name,$table1_column,$table2_column); }

        /**
         * Adds right join
         * @param string $table2_name
         * @param string $table1_column
         * @param string $table2_column
         * @return object
         */
		function add_right_join($table2_name,$table1_column,$table2_column=null) {
			$this->add_join($this->get_tablename_alias(),$table2_name,$table1_column,($table2_column==null ? $table1_column : $table2_column),"RIGHT");
			return $this;
		}

		function right($table2_name,$table1_column,$table2_column=null) { return $this->add_right_join($table2_name,$table1_column,$table2_column); }

        /**
         * Adds left join
         * @param string $table2_name
         * @param string $table1_column
         * @param string $table2_column
         * @return object
         */
		function add_left_join($table2_name,$table1_column,$table2_column=null) {
			$this->add_join($this->get_tablename_alias(),$table2_name,$table1_column,($table2_column==null ? $table1_column : $table2_column),"LEFT");
			return $this;
		}

		function left($table2_name,$table1_column,$table2_column=null) { return $this->add_left_join($table2_name,$table1_column,$table2_column); }


        /**
         * Adds where
         * @param string $column_name
         * @param string $operator
         * @param string $value
         * @param string $relation
         * @param boolean $parms
         * @param boolean $pad_value
         * @param boolean $escape
         * @return object
         */
		function add_where($column_name,$operator,$value,$relation=DB_CONSTRAINT::RELATION_AND,$parms=true,$pad_value=false,$escape=true) {
			$this->add_db_constraint(new DB_CONSTRAINT($column_name,$operator,$value,$relation,$parms,$pad_value,$escape));
			return $this;
		}

		function where($column_name,$operator,$value,$relation=DB_CONSTRAINT::RELATION_AND,$parms=true,$pad_value=false,$escape=true) {
			return $this->add_where($column_name,$operator,$value,$relation,$parms,$pad_value,$escape);
		}

        /**
         * Adds subquery
         * @param string $subquery
         * @param string $relation
         * @return object
         */
		function subquery($subquery,$relation=DB_CONSTRAINT::RELATION_AND) {
			$this->add_where("","",$subquery,$relation,false,false,false);
			return $this;
		}

		function between($column,$value1,$value2,$relation=DB_CONSTRAINT::RELATION_AND) {
			$this->add_where($column,"BETWEEN",$value1." AND ".$value2,$relation,false,false,false);
			return $this;
		}

		function add_subquery($subquery,$relation=DB_CONSTRAINT::RELATION_AND) {
			return $this->subquery($subquery,$relation);
		}

        /**
         * Adds having
         *
         * @param string $column_name
         * @param string $operator
         * @param string $value
         * @param string $relation
         * @param boolean $parms
         * @param boolean $pad_value
         * @param boolean $escape
         *
         * @return object
         */

        function add_having($column_name,$operator,$value,$relation=DB_CONSTRAINT::RELATION_AND,$parms=true,$pad_value=false,$escape=true) {
			return $this->having($column_name,$operator,$value,$relation,$parms,$pad_value,$escape);
		}

		function having($column_name,$operator,$value,$relation=DB_CONSTRAINT::RELATION_AND,$parms=true,$pad_value=false,$escape=true) {
			$this->add_having_constraint(new DB_CONSTRAINT($column_name,$operator,$value,$relation,$parms,$pad_value,$escape));
			return $this;
		}

        /**
         * Adds having condition
         *
         * @param string $column_name
         * @param string $operator
         * @param string $value
         * @param string $relation
         * @param boolean $parms
         * @param boolean $pad_value
         * @param boolean $escape
         *
         * @return object
         */
		function add_having_condition($column_name,$operator,$value,$relation=DB_CONSTRAINT::RELATION_AND,$parms=true,$pad_value=false,$escape=true) {
			$this->add_having($column_name,$operator,$value,$relation,$parms,$pad_value,$escape);
			return $this;
		}

        /**
         * Adds db constraint
         * @param string $db_constraint
         * @return object
         */
		function add_db_constraint($db_constraint) {
			$this->_where_constraints[] = $db_constraint;
			return $this;
		}

        /**
         * Adds having constraint
         * @param string $having_constraint
         * @return object
         */
		function add_having_constraint($having_constraint) {
			$this->_having_constraints[] = $having_constraint;
			return $this;
		}

        /**
         * Adds DB_CONSTRAINTs
         * @param array $db_constraints
         * @return object
         */
		function add_db_constraints($db_constraints) {
			$this->_where_constraints = array_merge($this->_where_constraints,$db_constraints);
			return $this;
		}

        /**
         * Get join clause
         * @return string
         */
		function get_join_clause() {
			$join_clause = array();
			foreach($this->_db_joins as $join)
				$join_clause[] = $join->get_join_statement();

			return implode(" ",$join_clause);
		}

        /**
         * Adds group
         * @param DB_GROUP $db_group
         * @param string $relation
         * @return object
         */

        function add_group($db_group,$relation=DB_CONSTRAINT::RELATION_AND) {
        	return $this->group($db_group,$relation);
        }

		function group($db_group,$relation=DB_CONSTRAINT::RELATION_AND) {

			if($db_group->has_conditions()) {
				$db_group->set_relation($relation);
				$this->_where_constraints[] = $db_group;
			}

			return $this;
		}

		function disable_parms() {
			foreach($this->get_where_constraints() as $db_group)
				$db_group->disable_parms();
		}

		function nested_dbq($fields,$alias) {
			return DBQ::create("(".$this->sql($fields).") as ".$alias);
		}
	}


    /**
    * Class DB_SUBQUERY
    *
    * @package Library
    * @subpackage Db
    */
	class DB_SUBQUERY extends DBQ {

		public static function create($dbq,$tablename_alias=null) {
			return new DB_SUBQUERY($dbq,$tablename_alias);
		}

		public function __construct($dbq,$tablename_alias=null) {

			if(is_a($dbq,"DBQ"))
				$dbq->disable_parms();

			elseif(is_string($dbq))
				$dbq = new DBQ($dbq);

			parent::__construct($dbq->get_tablename(),$dbq->get_primary_key());

			if($tablename_alias)
				$this->set_tablename_alias($tablename_alias);

			$this->set_db_joins($dbq->get_db_joins());
			$this->set_where_constraints($dbq->get_where_constraints());
		}

		public function where($column_name,$operator,$value,$relation=DB_CONSTRAINT::RELATION_AND,$pad_value=false,$escape=false) {
			parent::add_where($column_name,$operator,$value,$relation,false,$pad_value,$escape);
			return $this;
		}

		public function add_where($column_name,$operator,$value,$relation=DB_CONSTRAINT::RELATION_AND,$pad_value=false,$escape=false) {
			return $this->where($column_name,$operator,$value,$relation,$pad_value,$escape);
		}

		public function get_subquery($field) { return $this->select($field);	}

		public function get_select($field) {return $this->select($field); }

		public function select($field) { return $this->get_select_sql($field);  }

		public function insert($data) {

			foreach($data as $key=>$value)
				$data[$key] = array("parm"=>"'".$value."'");
			return $this->get_insert_sql($data);
		}

		public function get_count($field) {
			return $this->count($field);
		}

		public function count($field) {
			return $this->get_select_sql("COUNT(".$field.")");
		}

		public function not_exists($field) {
			$this->limit(1);
			return "NOT EXISTS(".$this->select($field).")";
		}

		public function exists($field=1) {
			$this->limit(1);
			return "EXISTS(".$this->select($field).")";
		}

		public function add_group($db_group) {
			return $this->group($db_group);
		}

		public function group($db_group) {

			$db_group->disable_parms();

			$this->add_subquery($db_group->get_condition_string());
			return $this;
		}
	}

	class DBQ_PARM {

		protected $_parm = "";

		public function __construct($parm) {
			$this->_parm = $parm;
		}

		public static function create($parm) { return new DBQ_PARM($parm); }

		public function get_parm() { return "(".$this->_parm.")"; }
	}