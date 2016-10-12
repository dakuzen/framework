<?
        /**
        * Class DB_GROUP
        *
        * @package Library
        * @subpackage Db
        */

	class DB_GROUP extends OBJECT {

		protected $_conditions 		= array();
		protected $_relation 		= DB_CONSTRAINT::RELATION_AND;

		function get_conditions() 		{ return $this->_conditions; }
		function get_constraints() 		{ return $this->_conditions; }
		function get_relation() 		{ return $this->_relation; }
		function set_relation($value) 	{ $this->_relation = $value; return $this; }
		function get_condition_count() 	{ return count($this->_conditions); }
		function has_conditions() 		{ return count($this->_conditions)>0; }

		function initialize() {
			$this->_conditions 	= array();
			$this->_relation 	= DB_CONSTRAINT::RELATION_AND;
			return $this;
		}

		function where($value1,$operator,$value2,$relation=DB_CONSTRAINT::RELATION_AND,$parms=true,$pad=false,$escape=true) {
			$this->_conditions[] = new DB_CONSTRAINT($value1,$operator,$value2,$relation,$parms,$pad,$escape);
			return $this;
		}

		function add_where($value1,$operator,$value2,$relation=DB_CONSTRAINT::RELATION_AND,$parms=true,$pad=false,$escape=true) {
			return $this->where($value1,$operator,$value2,$relation,$parms,$pad,$escape);
		}

		function add_subquery($subquery,$relation=DB_CONSTRAINT::RELATION_AND) {
			return $this->subquery($subquery,$relation);
		}

		function subquery($subquery,$relation=DB_CONSTRAINT::RELATION_AND) {
			return $this->add_where("","",$subquery,$relation,false,false,false);
		}

		function add_group($db_group,$relation=DB_CONSTRAINT::RELATION_AND) {
			return $this->group($db_group,$relation);
		}

		function group($db_group,$relation=DB_CONSTRAINT::RELATION_AND) {
			$db_group->set_relation($relation);
			$this->_conditions[] = $db_group;
			return $this;
		}

		function get_condition_string($parms=null,$pad=null) {
			$condition_ary = array();
			foreach($this->_conditions as $idx=>$condition) {

				$relation = ($idx>0) ? $condition->get_relation()." " : "";

				if(is_a($condition,"DB_CONSTRAINT"))
					$condition_ary[] = $relation." ".$condition->get_condition($parms,$pad);

				elseif(is_a($condition,"DB_GROUP"))
					$condition_ary[] = $relation." ".$condition->get_condition_string($parms,$pad);
			}

			$condition_string = implode(" ",$condition_ary);

			if(count($condition_ary)>1)
				$condition_string = "(".$condition_string.")";

			return $condition_string;
		}

		function disable_parms() {
			if(!is_a($this,"DB_GROUP_SUBQUERY"))
				foreach($this->_conditions as $condition)
					$condition->disable_parms();
		}

		function has_wheres() {
			return !is_empty($this->_conditions);
		}
	}

	class DB_GROUP_SUBQUERY extends DB_GROUP {

		function where($value1,$operator,$value2,$relation=DB_CONSTRAINT::RELATION_AND,$pad=true,$escape=true) {
			$this->_conditions[] = new DB_CONSTRAINT($value1,$operator,$value2,$relation,false,$pad,$escape);
			return $this;
		}
	}

    /**
    * Class DB_CONSTRAINT
    *
    * @package Library
    * @subpackage Db
    */

	class DB_CONSTRAINT extends OBJECT {

		const RELATION_OR 		= "OR";
		const RELATION_AND 		= "AND";

		protected $_field 		= null;
		protected $_value 		= null;
		protected $_operator 	= null;
		protected $_relation 	= null;
		protected $_parms		= null; //true
		protected $_pad			= null; //false
		protected $_escape		= null; //true

		function __construct($field,$operator="=",$value,$relation=DB_CONSTRAINT::RELATION_AND,$parms=true,$pad=false,$escape=true) {

			$this->_field 		= $field;
			$this->_operator 	= $operator;
			$this->_relation 	= $relation;
			$this->_parms 		= $parms;
			$this->_pad 		= $pad;
			$this->_escape 		= $escape;

			if(is_array($value)) {
				$this->_value = array_values($value);

			} else {
				if(stripos($operator,"IN")!==false)
					$this->_value = explode(",",$value);
				else
					$this->_value = array($value);
			}
		}

		function get_field() 		{ return $this->_field; }
		function get_operator()		{ return $this->_operator; }
		function get_value() 		{ return $this->_value; }
		function get_relation() 	{ return $this->_relation; }
		function parms() 			{ return $this->_parms; }
		function pad() 				{ return $this->_pad; }
		function escape() 			{ return $this->_escape; }

		function disable_parms() {
			$this->_parms = false;
			$this->_pad = true;
			return $this;
		}

		function get_condition($parms=null,$pad=null) {

			$value		= null;
			$delimit 	= ",";
			$left_bracket 	= "";
			$right_bracket 	= "";
			$is_in_operator = false;

			if(preg_match("/IN/i",$this->_operator)) {
				$is_in_operator = true;
				$left_bracket 	= "(";
				$right_bracket 	= ")";

			} elseif(stripos($this->_operator,"BETWEEN")!==false)
				$delimit = " AND ";

			$parms 	= is_null($parms) ? $this->parms() : $parms;
			$pad 		= is_null($pad) ? $this->pad() : $pad;

			if($parms) {
				if($this->_value)
					$value = implode($delimit,array_fill(0,count($this->_value),DBQ::get_parm_holder()));
			} else {

				$is_null = value($this->_value,0,null)===null;

				$value = $is_null ? array("null") : $this->_value;

				foreach($value as &$item) {

					//if the item is not a select statement
					if(!preg_match("/^[\(\s]*(count|exists|select)/i",$item)) {

						//This was redundent because it was being done below
						//if the item is encapsulated with '
						//if(preg_match("/^'[^']'$/",$item)) {
						//	$item = trim($item,"'");
						//	if($this->_escape)
						//		$item = addslashes($item);
						//	$pad = true;
						//} else {

							//if the item is wrapped in " or ' then remove the quotes
							if(preg_match("/^[\"']([^\"']*)[\"']$/",$item,$matches)) {
								$item = value($matches,1);
								if($this->escape())
									$item = "'".addslashes($item)."'";
							} elseif(preg_match("/^(NOT\s+)?IN$/i",trim($this->_operator))) {
								$item = addslashes($item);

							} elseif($this->escape())
								$item = addslashes($item);
						//}

						// Never pad a null value
						if($pad && !$is_null)
							$item = "'".$item."'";

					}
				}

				$value = implode($delimit,$value);
			}

			$field = $this->get_field();

			if(in_array(strtolower($this->get_field()),array("default")))
				$field = "`".$this->get_field()."`";

			$condition = $field." ".$this->get_operator()." ".$left_bracket.$value.$right_bracket."";

			if($is_in_operator && is_array($this->_value)) {

				// If there is an IN operator and there is null in the array use the "is null" operator
				// otherwise kill the query with 1 = 0
				if(in_array(null,$this->_value,true))
					$condition = "(".$condition." OR ".$this->_field." IS null)";
				elseif(!$this->_value)
					$condition = "1 = 0";
			}

			return $condition;
		}
	}

    /**
    * Class DB_JOIN
    *
    * @package Library
    * @subpackage Db
    */

	class DB_JOIN extends OBJECT {

		protected $_tablename2 		= null;
		protected $_tablename2_alias 	= null;
		protected $_type			= null;
		protected $_db_group 		= null;

		function __construct($tablename1=null,$tablename2=null,$column1=null,$column2=null,$type="INNER") {
			$this->_db_group	= new DB_GROUP();
			$this->_type		= $type;
			$this->_tablename2 	= $tablename2;

			$column2 = $column2 ? $column2 : $column1;

			if($tablename1!=null && $tablename2!=null && $column1!=null && $column2!=null)
				$this->add_condition($tablename1,$tablename2,$column1,$column2);
		}

		public static function create($tablename1=null,$tablename2=null,$column1=null,$column2=null,$type="INNER") {
			return new DB_JOIN($tablename1,$tablename2,$column1,$column2,$type);
		}

		public function add_condition($tablename1,$tablename2,$column1,$column2,$operator="=",$relation=DB_CONSTRAINT::RELATION_AND) {
			return $this->condition($tablename1,$tablename2,$column1,$column2,$operator,$relation);
		}

		public function condition($tablename1,$tablename2,$column1,$column2,$operator="=",$relation=DB_CONSTRAINT::RELATION_AND) {

			$tablename1_ary 		= explode(" ",$tablename1);
			$tablename1	 		= $tablename1_ary[0];

			if(count($tablename1_ary)>1) {
				$second_arg = get_value($tablename1_ary,1);
				if(strtoupper($second_arg)=="AS")
					$tablename1 = get_value($tablename1_ary,2);
				else
					$tablename1 = $second_arg;
			}

			$tablename2_ary 		= explode(" ",$tablename2);
			$this->_tablename2 		= get_value($tablename2_ary,0);
			$this->_tablename2_alias	= get_value($tablename2_ary,0);

			if(count($tablename2_ary)>1) {
				$second_arg = get_value($tablename2_ary,1);
				if(strtoupper($second_arg)=="AS")
					$this->_tablename2_alias = get_value($tablename2_ary,2);
				else $this->_tablename2_alias = $second_arg;

				$this->_tablename2 .= " ".$this->_tablename2_alias;
			}

			$value1 = $this->get_function_safe_column($tablename1,$column1);
			$value2 = $this->get_function_safe_column($this->_tablename2_alias,$column2);

			$this->_db_group->add_where($value1,$operator,$value2,$relation,false,false);

			return $this;
		}

		public function get_function_safe_column($table,$column) {

			//If there is a sign that this column is a mySQL function
			if(preg_match("/\(/",$column))
				return $column;
			if(preg_match("/^([a-z0-9]+)\(([^\)]+)/i",$column,$matches)) {
				return get_value($matches,1)."(".$table.".".get_value($matches,2).")";
			} else
				return $table.".".$column;
		}

		public function add_simple_condition($value1,$operator,$value2,$relation=DB_CONSTRAINT::RELATION_AND,$parms=false) {
			return $this->on($value1,$operator,$value2,$relation,$parms);
		}

		public function on($value1,$operator,$value2,$relation=DB_CONSTRAINT::RELATION_AND,$parms=false,$quote=false) {
			$this->_db_group->add_where($value1,$operator,$value2,$relation,$parms,$quote);
			return $this;
		}

		public function group($db_group,$relation=DB_CONSTRAINT::RELATION_AND,$parms=false) {
			return $this->add_group($db_group,$relation,$parms);
		}

		public function add_group($db_group,$relation=DB_CONSTRAINT::RELATION_AND,$parms=false) {
			$this->_db_group->add_group($db_group,$relation,$parms);
			return $this;
		}

		public function get_conditions() 	{ return $this->_db_group->get_conditions(); }
		public function type($value) 		{ $this->_type = $value; return $this; }
		public function set_type($value) 	{ return $this->type($value); }
		public function set_db_group($value) 	{ $this->_db_group = $value; return $this; }

		public function get_custom_join_statement($sql,$type="") {
			$this->_type = $type;
			$this->_tablename2 = $sql;
		}

		public function get_join_statement() {
			$sql = $this->_type." JOIN ".$this->_tablename2;

			if($this->_db_group->has_conditions())
				$sql .= " ON ".$this->_db_group->get_condition_string();

			return $sql;
		}
	}