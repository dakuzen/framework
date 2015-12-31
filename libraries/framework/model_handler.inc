<?php

        /**
        * Class MODEL_HANDLER  
        *
        * @package Library
        * @subpackage Common
        */
	 
	class MODEL_HANDLER extends MODEL {
		
		/**
		 * The filters
		 * @var array $filters
		 */
		protected $_filters 			= array();

		protected $_handlers			= array();
		
		/**
		 * The filter group relations 
		 * @var array $filter_group_relations
		 */
		protected $_filter_group_relations 	= array();
		
		/**
		 * The limit offset
		 * @var integer 
		 */
		protected $_limit_offset		= null;
		
		/**
		 * Limit count
		 * @var integer 
		 */
		protected $_limit_count			= null;
		
		/**
		 * The dbq
		 * @var object 
		 */
		protected $_dbq				= null;
		
		/**
		 * select fields
		 * @var array $select_fields
		 */
		protected $_select_fields 		= array("*");
		
		/**
		 * order bys
		 * @var array $order_bys 
		 */
		protected $_order_bys			= array();
		
		/**
		 * group bys
		 * @var array $group_bys 
		 */
		protected $_group_bys			= array();
		
		/**
		 * db joins
		 * @var array $db_joins
		 */
		protected $_db_joins			= array();
		
		/**
		 * The properties
		 * @var array $properties 
		 */
		protected $_properties			= array();
		
		/**
		 * Db groups
		 * @var array $db_groups 
		 */
		protected $_db_groups			= array();
		
		/**
		 * Returns the dbq 
		 */
		public function dbq($dbq=null) {
			$dbq = $this->_dbq ? $this->_dbq : $dbq;
			return $this->apply_constraints(clone $dbq);
		}

		public function get_dbq($dbq=null) {
			return $this->dbq($dbq);
		}

		/*
		 * Support legacy gets() function ie. get_site_content_widgets()
		 */
		public function gets() {
			$function = "get_".$this->get_dbq()->get_tablename();
			return $this->$function();
		}

		public function count($field=null) {
			return $this->get_count($field);
		}	

		public function reset() {		
			$this->_limit_offset = $this->_limit_count = $this->_select_fields = $this->_group_bys = $this->_db_groups = $this->_filters = $this->_order_bys = $this->_db_joins = array();
			return $this;
		}
		
		public static function __callStatic($name, $arguments) {
			
			$class = get_called_class();

			if($name=="get") {

				$key = value($arguments,0);
				$hmodel = new $class();
				return $hmodel->get($key);
		
			} elseif($name=="create")
				return new $class();

			elseif($name=="get_default_handler" || $name=="default_handler") {
				
				if(method_exists($class,"get_default_handler"))
					return $class::get_default_handler();

				return new $class();
			}

			trigger_error("Call to undefined method ".$class."::".$name."()", E_USER_ERROR);
		}

		public function __call($name, $arguments) {

			$class = get_called_class();
				
			if($name=="get") {
				
				if(array_key_exists(0,$arguments)) {
					$key = get_value($arguments,0);
					$this->where($this->get_dbq()->get_tablename().".".$this->get_dbq()->get_primary_key(),"=",$key);
				}

				$this->limit(1);

				return value($this->gets(),0);
			} elseif($name=="create")
			 	return new $class();

			trigger_error("Call to undefined method ".$class."->".$name."()", E_USER_ERROR);
		}

		function create_dbq($dbq) {
			$dbq = $this->_dbq ? clone $this->_dbq : $dbq;
			return $this->apply_constraints($dbq);
		}
		
		
		function register_dbq($dbq) { $this->_dbq = $dbq; return $dbq; }

		/**
		 * apply constraints
		 * @param object $db_object
		 * @param boolean $allow_parms 
		 */
		function apply_constraints($db_object,$allow_parms=true) {

			$this->apply_joins($db_object);
			
			$this->apply_filters($db_object);			
			
			foreach($this->_db_groups as $db_group)
				$db_object->add_group($db_group[0],$db_group[1]);

			if($this->get_limit())
				$db_object->set_limit($this->get_limit(),$this->get_offset());

			foreach($this->_order_bys as $field=>$direction)
				$db_object->add_order_by($field,$direction);
			
			foreach($this->_group_bys as $group)
				$db_object->add_group_by($group);				

			return $db_object;
		}
		
		/**
		 * Apply joins
		 * @param object $db_object 
		 */
		function apply_joins(&$db_object) {
		
			if($this->_db_joins)
				$db_object->add_db_joins($this->_db_joins);
		}
		
		/**
		 * apply filters
		 * @param object $db_object
		 * @param boolean $allow_parms 
		 */
		function apply_filters(&$db_object,$allow_parms=true) {
			
			if(count($this->_filters)) {

				$filter_groups = array();
				foreach($this->_filters as $filter) 
					if($filter->is_type_where())
						$filter_groups[$filter->get_group()][] = $filter;
				
				foreach($filter_groups as $group_name=>$filters) {

					$field_group = new DB_GROUP();

					foreach($filters as $filter) {
						if($filter->is_type_where()) {
							
							$parms = $allow_parms && $filter->get_parms() ? true : false;
						
							$field_group->add_where($filter->get_field(),$filter->get_operator(),$filter->get_value(),$filter->get_relation(),$parms,$filter->get_pad_value(),$filter->get_escape_value());						
						}
					}
					
					$db_object->add_group($field_group,$this->get_filter_group_relation($group_name));
				}
				
				foreach($this->_filters as $filter)
					if($filter->is_type_having())
						$db_object->add_having($filter->get_field(),$filter->get_operator(),$filter->get_value(),$filter->get_relation(),$filter->get_parms(),$filter->get_pad_value(),$filter->get_escape_value());
			}

		}
		
		/**
		 * apply properties
		 * @param object $complex_model
		 * @param array $data 
		 */
		function apply_properties(&$complex_model,$data) {
			foreach(array_keys($this->_properties) as $property) {
				$complex_model->data($property,value($data,$property));
			}

		}
		
		/**
		 * set filter group relation
		 * @param string $group_name
		 * @param string $relation 
		 */
		function set_filter_group_relation($group_name,$relation)	{ $this->_filter_group_relations[$group_name] = $relation; }
		
		/**
		 * Return the filter group relation
		 * @param string $group_name
		 * @return string 
		 */
		function get_filter_group_relation($group_name) 		{ return get_value($this->_filter_group_relations,$group_name,"AND"); }

		/**
		 * Set limit
		 * @param intger $limit
		 * @param integer $offset
		 * @return MODEL_HANDLER 
		 */
		public function set_limit($limit,$offset=null,$count=null) {			

			if($count!==null && $offset>=$count)
				$offset = 0;

			$this->_limit_count		= $limit;
			$this->_limit_offset 	= $offset;
			return $this;
		}

		public function limit($limit,$offset=null,$count=null) { return $this->set_limit($limit,$offset,$count); }
		
		
		/**
		 * Set dbq
		 * @param object $dbq 
		 */
		function set_dbq($dbq) 		{ $this->_dbq = $dbq; return $this; }
		
		/**
		 * Returns the limit
		 * @return integer 
		 */
		function get_limit() 		{ return $this->_limit_count; }
		
		/**
		 * Return select fields
		 * @return array 
		 */
		function get_select_fields() 	{ return $this->_select_fields; }
		
		/**
		 * Returns the offset
		 * @return integer 
		 */
		function get_offset() 		{ return $this->_limit_offset; }
		
		/**
		 * Adds filter
		 * @param string $field
		 * @param string $operator
		 * @param string $value
		 * @param string $reation
		 * @param string $group
		 * @param boolean $parms
		 * @param boolean $pad_value
		 * @param boolean $escape_value
		 * @return MODEL_HANDLER 
		 */
		function add_filter($field,$operator,$value,$reation=null,$group=null,$parms=true,$pad_value=true,$escape_value=true) {		
			$this->_filters[] = new MODEL_HANDLER_FILTER($field,$operator,$value,$reation,$group,$parms,$pad_value,$escape_value);
			return $this;
		}

		function filter($field,$operator,$value,$reation=null,$group=null,$parms=true,$pad_value=true,$escape_value=true) {
			return $this->add_filter($field,$operator,$value,$reation,$group,$parms,$pad_value,$escape_value);
		}		

		function where($field,$operator,$value,$reation=null,$group=null,$parms=true,$pad_value=true,$escape_value=true) {
			return $this->add_filter($field,$operator,$value,$reation,$group,$parms,$pad_value,$escape_value);
		}	
		
		/**
		 * Adds having
		 * @param string $field
		 * @param string $operator
		 * @param string $value
		 * @param string $reation
		 * @param string $group
		 * @param boolean $parms
		 * @param boolean $pad_value
		 * @param boolean $escape_value
		 * @return MODEL_HANDLER 
		 */
		
		function add_having($field,$operator,$value,$reation=null,$group=null,$parms=true,$pad_value=true,$escape_value=true) {		
			return $this->having($field,$operator,$value,$reation,$group,$parms,$pad_value,$escape_value);
		}

		function having($field,$operator,$value,$reation=null,$group=null,$parms=true,$pad_value=true,$escape_value=true) {	
			$this->_filters[] = new MODEL_HANDLER_FILTER($field,$operator,$value,$reation,$group,$parms,$pad_value,$escape_value,MODEL_HANDLER_FILTER::TYPE_HAVING);
			return $this;
		}		
		
		/**
		 * Add order by
		 * @param string $field
		 * @param string $direction
		 * @return MODEL_HANDLER 
		 */
		function add_order_by($field,$direction="ASC") { 
			$this->_order_bys[$field] = $direction; 
			return $this;
		}
		
		function order_by($field,$direction="ASC") { 
			return $this->add_order_by($field,$direction);
		}

		function order($field,$direction="ASC") {
			return $this->add_order_by($field,$direction);
		}

		/**
		 * Add group
		 * @param string $db_group
		 * @param string $relation
		 * @return MODEL_HANDLER 
		 */

		function add_group($db_group,$relation="AND") {
			return $this->group($db_group,$relation);
		}

		function group($db_group,$relation="AND") {
			$this->_db_groups[] = array($db_group,$relation); 
			return $this;
		}
		
		/**
		 * Add join
		 * @param string $tablename1
		 * @param string $tablename2
		 * @param string $column1
		 * @param string $column2
		 * @param string $type
		 * @return MODEL_HANDLER 
		 */
		function add_join($tablename1,$tablename2,$column1,$column2=null,$type="INNER") {
			$column2 = $column2 ? $column2 : $column1;		
			$this->add_db_join(new DB_JOIN($tablename1,$tablename2,$column1,$column2,$type));
			
			return $this;
		}
		
		function inner($tablename1,$tablename2,$column1,$column2=null,$type="INNER") {
			return $this->add_join($tablename1,$tablename2,$column1,$column2,$type);
		}

		function join($tablename1,$tablename2,$column1,$column2=null,$type="INNER") {
			return $this->add_join($tablename1,$tablename2,$column1,$column2,$type);
		}
		

		/**
		 * Adds db join
		 * @param string $db_join
		 * @return MODEL_HANDLER 
		 */

		function add_db_join($db_join) {
			return $this->db_join($db_join);
		}

		function db_join($db_join) {
			$this->_db_joins[] = $db_join;
			return $this;
		}
		
		/**
		 * Adds group by
		 * @param string $group_by
		 * @return MODEL_HANDLER 
		 */
		function group_by($group_by) {
			$this->_group_bys[] = $group_by;
			return $this;
		}		
		
		function add_group_by($group_by) {
			return $this->group_by($group_by);
		}

		/**
		 * Adds select field
		 * @param string $field
		 * @return MODEL_HANDLER 
		 */
		function add_select_field($field) {
			$this->_select_fields[] = $field;
			return $this;
		}
		
		/**
		 * Prepend select field
		 * @param string $field 
		 */
		function prepend_select_field($field) {
			array_unshift($this->_select_fields,$field);
		}		
		
		/**
		 * Set select fields
		 * @param array $fields
		 * @return MODEL_HANDLER 
		 */
		function set_select_fields($fields) {
			$this->_select_fields = $fields;
			return $this;
		}		
		
		/**
		 * Adds subquery
		 * @param string $subquery
		 * @param string $alias
		 * @return MODEL_HANDLER 
		 */
		function subquery($subquery,$alias=null) { 
			return $this->add_select_subquery($subquery,$alias); 
			return $this;
		}

		function add_subquery($subquery,$alias=null) { 
			return $this->subquery($subquery,$alias);
		}		

		
		/**
		 * add select subquery
		 * @param string $subquery
		 * @param string $alias
		 * @return MODEL_HANDLER 
		 */
		function add_select_subquery($subquery,$alias=null) {
			if(!preg_match("/^\([^\)]+\)$/",$subquery))
				$subquery = "(".$subquery.")";
				
			$this->add_property($subquery,$alias);
			return $this;
		}
		
		/**
		 * Add where subquery
		 * @param string $subquery
		 * @param string $relation
		 * @param string $group
		 * @return MODEL_HANDLER 
		 */
		function where_subquery($subquery,$relation=null,$group=null) {
			if(!preg_match("/^\([^\)]+\)$/",$subquery))
				$subquery = "(".$subquery.")";
				
			$this->add_filter($subquery,"","",$relation,$group,false,false,false);
			return $this;
		}

		function add_where_subquery($subquery,$relation=null,$group=null) {
			return $this->where_subquery($subquery,$relation,$group);
		}		
		
		/**
		 * Adds having subquery
		 * @param string $subquery
		 * @param string $relation
		 * @param string $group
		 * @return MODEL_HANDLER 
		 */
		function add_having_subquery($subquery,$relation=null,$group=null) {
			if(!preg_match("/^\([^\)]+\)$/",$subquery))
				$subquery = "(".$subquery.")";
				
			$this->add_having($subquery,"","",$relation,$group,false,false,false);
			return $this;
		}		
		
		/**
		 * Adds property
		 * @param string $select_field
		 * @param string $alias
		 * @return MODEL_HANDLER 
		 */
		function add_property($select_field,$alias=null) {
			return $this->property($select_field,$alias);
		}

		function property($select_field,$alias=null) {
			
			if(!$alias) {
				preg_match("/\.?([^\.]+)$/",$select_field,$matches);
				$alias = get_value($matches,1);

				$select_field = "(".$select_field.")";

			} else
				$select_field = "(".$select_field.") AS ".$alias;
			
			$this->add_select_field($select_field);			
			$this->_properties[$alias] = "";	
			return $this;
		}

		function data($select_field,$alias=null) {
			return $this->property($select_field,$alias);
		}

		function handler($name,$hmodel=null) {
			if($hmodel) {
				$this->_handlers[$name] = $hmodel;
				return $this;
			}

			return value($this->_handlers,$name);
		}		
		
		/**
		 * Returns count
		 * @param string $field
		 * @return integer 
		 */
		function get_count($field=null) {
			
			$dbq = clone $this->get_dbq();

			return $dbq->count($field);
		}
		
		/**
		 * exists
		 * @return boolean 
		 */
		function exists() {
			return $this->get_dbq()->exists();
		}
		
		/**
		 * Checks whether is called staticly
		 * @param string $class
		 * @return boolean 
		 */
		function is_called_staticly($class) {
			
			if(preg_match("/^(?:BASE_)?(?:H|HANDLER_)MODEL_([_\w\d]+)/i",$class,$matches)) {
				
				$class = isset($this) ? get_class($this) : "";
				
				if(@!preg_match("/^(?:BASE_)?(?:H|HANDLER_)MODEL_".get_value($matches,1)."/i",$class))
					return true;
			}

			return false;
		}		
		
		/**
		 * Returns the grouped
		 * @param array $cmodels
		 * @param array $grouping
		 * @return array 
		 */
		function get_grouped($cmodels,$grouping) {
			
			foreach($grouping as &$group)
				$group = $group ? "[\$cmodel->".$group."()]" : "[]";
			
			$grouped_cmodels = array();
			foreach($cmodels as $cmodel)
				eval("\$grouped_cmodels".implode("",$grouping)." = \$cmodel;");
			
			return $grouped_cmodels;
		}
			
		public function map_many_many(	$cmodels,
										$handler,
										$source_object,
										$joiner_table,
										$source_col, //The column in the source table to identify the ids
										$reference_table,
										$reference_joiner_col,
										$referance_col,
										$object) {

			if($cmodels && $handler) {

				$source_function 		= "get_".$source_object."_id";
				$object_set_function 	= "set_".LANG_UTIL::plural($object);

				$ids = array_filter(array_unique(ARRAY_UTIL::listing($cmodels,$source_function)));
				
				if($ids) {

					$tmp_cmodels =
						$handler
							->join($reference_table,$joiner_table,$reference_joiner_col,$referance_col)
							->data($source_object."_id")
							->where($source_col,"IN",$ids)
							->gets();

					foreach($cmodels as $cmodel) {
						$array_cmodels = array();
						foreach($tmp_cmodels as $tmp_cmodel) {

							if($cmodel->$source_function()==$tmp_cmodel->data($source_object."_id"))
								$array_cmodels[] = 	$tmp_cmodel;
						}

						$cmodel->$object_set_function($array_cmodels);
					}
				}
			}
		}

		public function map_many($cmodels,$handler,$source,$relation,$object=null) {

			if($cmodels && $handler) {

				$object 				= $object ? $object : $relation;
				$source_function 		= "get_".$source."_id";
				$relation_function 		= "get_".$relation."_id";
				$relation_id 			= $relation."_id";
				$object_set_function 	= "set_".LANG_UTIL::plural($object);
	
				$ids = array_filter(array_unique(ARRAY_UTIL::listing($cmodels,$source_function)));

				if($ids) {

					$tmp_cmodels = $handler
										->where($relation_id,"IN",$ids)
										->gets();

					foreach($cmodels as $cmodel) {
						$array_cmodels = array();
						foreach($tmp_cmodels as $tmp_cmodel) {
							if($cmodel->$source_function()==$tmp_cmodel->$relation_function()) {
								$array_cmodels[] = 	$tmp_cmodel;
							}
						}

						$cmodel->$object_set_function($array_cmodels);
					}
				}
			}
		}			

		public function map_one($cmodels,$handler,$source,$relation,$object=null) {

			if($cmodels && $handler) {

				$object 				= $object ? $object : $relation;
				$source_function 		= "get_".$source."_id";
				$relation_function 		= "get_".$relation."_id";
				$relation_id 			= $relation."_id";
				$object_set_function 	= "set_".$object;
	
				$ids = array_filter(array_unique(ARRAY_UTIL::listing($cmodels,$source_function)));

				if($ids) {
					
					// This is done when mapping the $cmodels for speed
					$tmp_cmodels = ARRAY_UTIL::index($handler
														->where($handler->get_dbq()->get_tablename().".".$relation_id,"IN",$ids)
														->gets(),"get_".$relation_id);
					foreach($cmodels as $cmodel)
						$cmodel->$object_set_function(value($tmp_cmodels,$cmodel->$source_function()));
				}
			}
		}
	}


    /**
    * Class MODEL_HANDLER_FILTER  
    *
    * @package Library
    * @subpackage Common
    */
	class MODEL_HANDLER_FILTER {

		const RELATION_OR 		= "OR";
		const RELATION_AND 		= "AND";

		const TYPE_WHERE 		= "W";
		const TYPE_HAVING		= "H";
		
		/**
		 * The field
		 * @var string $field
		 */
		private $_field 		= null;
		
		/**
		 * the value
		 * @var string $value
		 */
		private $_value 		= null;
		
		/**
		 * The operator
		 * @var string $operator 
		 */
		private $_operator 		= null;
		
		/**
		 * The relation
		 * @var string $relation
		 */
		private $_relation 		= null;
		
		/**
		 * The group
		 * @var string $group 
		 */
		private $_group			= null;
		
		/**
		 * The parms
		 * @var boolean $parms 
		 */
		private $_parms			= true;
		
		/**
		 * pad value
		 * @var boolean $pad_value 
		 */
		private $_pad_value		= false;
		
		/**
		 * escape value
		 * @var boolean $escape_value 
		 */
		private $_escape_value		= true;
		
		/**
		 * the type
		 * @var string $type 
		 */
		private $_type			= self::TYPE_WHERE;
		
		/**
		 * Constructor
		 * @param string $field
		 * @param string $operator
		 * @param string $value
		 * @param string $relation
		 * @param string $group
		 * @param boolean $parms
		 * @param boolean $pad_value
		 * @param boolean $escape_value
		 * @param string $type 
		 */
		function __construct($field,$operator="=",$value,$relation=null,$group=null,$parms=true,$pad_value=false,$escape_value=true,$type=self::TYPE_WHERE) {			
			$this->_field 			= $field;
			$this->_operator 		= $operator;
			$this->_relation 		= $relation ? $relation : DB_CONSTRAINT::RELATION_AND;
			$this->_value 			= $value;
			$this->_group 			= $group;
			$this->_parms 			= $parms;
			$this->_pad_value 		= $pad_value;
			$this->_escape_value 	= $escape_value;
			$this->_type	 		= $type;
		}

		/**
		 * Returns the field
		 * @return string 
		 */
		function get_field() 		{ return $this->_field; }
		
		/**
		 * Returns the field
		 * @return string 
		 */
		function get_value() 		{ return $this->_value; }
		
		/**
		 * Returns the field
		 * @return string 
		 */
		function get_operator()		{ return $this->_operator; }
		
		/**
		 * Returns the field
		 * @return string 
		 */
		function get_relation() 	{ return $this->_relation; }
		
		/**
		 * Returns the field
		 * @return string 
		 */
		function get_group() 		{ return $this->_group; }
		
		/**
		 * Returns the parms
		 * @return boolean 
		 */
		function get_parms() 		{ return $this->_parms; }
		
		/**
		 * Returns the pad value
		 * @return boolean 
		 */
		function get_pad_value() 	{ return $this->_pad_value; }
		
		/**
		 * Returns the escape alue
		 * @return boolean 
		 */
		function get_escape_value() 	{ return $this->_escape_value; }
		
		/**
		 * Checks whether type is where
		 * @return boolean 
		 */
		function is_type_where() 	{ return $this->_type==self::TYPE_WHERE; }
		
		/**
		 * Checks whether type is having
		 * @return boolean 
		 */
		function is_type_having() 	{ return $this->_type==self::TYPE_HAVING; }
	}	

	class HMODEL extends MODEL_HANDLER {}