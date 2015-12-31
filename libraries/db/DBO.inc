<?php
    /**
    * Class DBO_COLUMN  
    *
    * @package Library
    * @subpackage Db
    */

	class DBO_COLUMN extends OBJECT {
        
        /**
         * STRING DATA TYPES
         * @var array $STRING_DATA_TYPES 
         */
		static $STRING_DATA_TYPES = array("char","varchar","binary","varbinary","blob","tinyblob","mediumblob","longblob","tinytext","text","mediumtext","longtext");
		
		const CONFIG_PRIMARY 	= CONSTANT::BIT_01;
		const CONFIG_TAGGED 	= CONSTANT::BIT_02;
		//const CONFIG_CHANGED	= CONSTANT::BIT_03;

		const ENCRYPT_AES	= "AES";

		/**
		 * The data type
		 * @var string $data_type 
		 */
		protected $_data_type 	= null;

		/**
		 * The config
		 * @var integer $config
		 */
		protected $_config 	= 0;

		/**
		 * The value
		 * @var string $value 
		 */
		protected $_value	= null;

		/**
		 * The encrypt
		 * @var string $encrypt 
		 */
		protected $_encrypt	= null;

		/**
		 * encrypt key
		 * @var string $encrypt_key 
		 */
		protected $_encrypt_key	= null;

		/**
		 * Constructor
		 * @param string $data_type
		 * @param string $size
		 * @param boolean $not_null
		 * @param boolean $primary 
		 */
		function __construct($data_type,$size=null,$not_null=false,$primary=false) {
			$this->_data_type = $data_type;
			$this->_config += $primary ? CONSTANT::BIT_01 : 0;
		}

		/**
		 * Returns the data type
		 * @return string 
		 */
		function get_data_type() 	{ return $this->_data_type; }

		function is_data_type($value) 	{ return strtolower($this->_data_type)==strtolower($value); }

		/**
		 * Returns the value
		 * @return string
		 */
		function get_value() 		{ return $this->_value; }

		/**
		 * Returns the encrypt key
		 * @return string 
		 */
		function get_encrypt_key() 	{ return $this->_encrypt_key; }

		/**
		 * Checks whether is primary
		 * @return boolean 
		 */
		function is_primary() 		{ return ($this->_config & self::CONFIG_PRIMARY) == self::CONFIG_PRIMARY; }

        /**
         * Check whether is tagged
         * @return boolean 
         */
        function is_tagged() 		{ return ($this->_config & self::CONFIG_TAGGED) == self::CONFIG_TAGGED; }
		//function is_changed() 		{ return ($this->_config & self::CONFIG_CHANGED) == self::CONFIG_CHANGED; }
		
        /**
         * Check whether is encrypt aes
         * @return boolean 
         */
        function is_encrypt_aes() 	{ return $this->_encrypt==self::ENCRYPT_AES; }

        /**
         * Enable tag
         */
		function enable_tag() {
			if(!$this->is_tagged())
				$this->_config += self::CONFIG_TAGGED; 
		}
		
		/*function enable_changed() {
			if(!$this->is_changed())
				$this->_config += self::CONFIG_CHANGED; 
		}
		*/

        /**
         * Disable tag 
         */
		function disable_tag() { 
			if($this->is_tagged())
				$this->_config -= self::CONFIG_TAGGED; 
		}
		
		/*
		function disable_change() { 
			if($this->is_changed())
				$this->_config -= self::CONFIG_CHANGED; 
		}		
		*/

        /**
         * Set value
         * @param string $value
         * @param boolean $untag 
         */
		function set_value($value,$untag=true) {
			
			if(!$untag)
				$this->enable_tag();

			if($value!==null) {

				if(!$this->is_data_type_string()) {

					if($this->is_data_type_int())
						$value = strlen($value)==0 ? null : (int)$value; 

					elseif($this->is_data_type_decimal())
						$value = strlen($value)==0 ? null : (float)$value; 

					elseif(($is_datetime=$this->is_data_type_datetime()) || ($is_date=$this->is_data_type_date())) {

						if(is_numeric($value)) {
							if($is_datetime)
								$value = DB::get_date_time($value);

							elseif($is_date)
								$value = DB::get_date($value);
						}
					}
				}
			}			
				
			$this->_value = $value;
		}
		
        /**
         * encrypt aes
         * @param string $key 
         */
		function encrypt_aes($key) {
			$this->_encrypt 	= self::ENCRYPT_AES;
			$this->_encrypt_key 	= $key;
		}

        /**
         * Check whether data type is integer
         * @return boolean 
         */
		function is_data_type_int() 		{ return stripos("int",$this->get_data_type())===0; }
                
        /**
         * Check whether data type is decimal
         * @return boolean 
         */
		function is_data_type_decimal() 	{ return $this->get_data_type()=="decimal" || $this->get_data_type()=="float"; }
		
        /**
         * Check whether data type is date
         * @return boolean 
         */
        function is_data_type_date() 		{ return $this->get_data_type()=="date"; }

        /**
         * Check whether data type is datetime
         * @return boolean 
         */
        function is_data_type_datetime() 	{ return $this->get_data_type()=="datetime"; }

        /**
         * Check whether data type is string
         * @return boolean 
         */
        function is_data_type_string() 		{ return in_array($this->get_data_type(),self::$STRING_DATA_TYPES); }
	}
        
    /**
    * Class DBO  
    *
    * @package Library
    * @subpackage Db
    */
	class DBO extends OBJECT {

		/**
		 * The columns
		 * @var array $columns 
		 */
		protected $_columns 			= array();

		/**
		 * The previous values
		 * @var array $previous_values
		 */
		protected $_previous_values		= array();

		/**
		 * The tablename
		 * @var string $tablename 
		 */
		protected $_tablename 			= "";

		/**
		 * The db object
		 * @var object $db_object 
		 */
		protected $_db_object			= null;

		/**
		 * The db
		 * @var object $db 
		 */
		protected $_db				= null;

		/**
		 * The logging
		 * @var boolean $logging
		 */
		protected $_logging			= true;

		/**
		 * The transactions
		 * @var array $transactions
		 */
		protected $_transactions		= true;

		/**
		 * set the db connection
		 * @param object $db 
		 */
		function set_db_connection($db)	{
			$this->initialize_db($db);
		}

		/**
		* Returns the db object
		* @return object 
		*/
		function get_db_object()		{ return $this->_db_object; }

		/**
		* Returns the tablename
		* @return string 
		*/
		function get_tablename()		{ return $this->_tablename; }

		/**
		* Returns the columns
		* @return array 
		*/
		function get_columns()			{ return $this->_columns; }

		/**
		* Returns the column names
		* @return array 
		*/
		function get_column_names()		{ return array_keys($this->_columns); }

		/**
		* Returns the error message
		* @return string 
		*/
		function get_error_message()		{ return $this->get_db_object()->get_error_message(); }

		/**
		* Returns the column
		* @param string $name
		* @return string 
		*/
		function get_column($name) 		{ return @$this->_columns[$name]; }

		/**
		* Checks whether has column
		* @param string $name
		* @return boolean 
		*/
		function has_column($name) 		{ return array_key_exists($name,$this->_columns); }

		/**
		* disable logging
		*/
		function disable_logging() 		{ $this->_logging = false; }

		/**
		* disable transactions
		*/
		function disable_transactions() 	{ $this->_transactions = false; }

		/**
		* Clone
		*/
		function __clone() {
			foreach($this->_columns as $idx=>$column)
				$this->_columns[$idx] = clone $column;
		}

        /**
         * Call
         * @param string $function
         * @param array $args
         * @return DBO 
         */
		public function __call($function,$args) {

			if(preg_match("/^(?:(set_)|get_)(.*)/",$function,$matches)) {

				$name = $matches[2];
				
				if(@($column=$this->_columns[$name])) {
				
					if($matches[1]=="set_") {

						if(count($args)>0) {

							$value 	= @$args[0];
							$untag 	= isset($args[1]) ? $args[1] : null;

							//if($valid_value) {
								//if($this->_previous_values && @strcmp($value,$this->get_previous_value($name))!==0)
									//$column->enable_changed();

							//} else
							//	$value = null;

							$column->set_value($value,$untag);
							
							return $this;

						} else 
							trigger_error ('Missing argument 1 for '.get_class($this).'::'.$function.'() ', E_USER_ERROR);
					} else 
						return $column->get_value();
				}
			}

			DEBUG_UTIL::print_backtrace(1);
			trigger_error ("Call to undefined method ".get_class($this)."::".$function."() ", E_USER_ERROR);
		}
		
        /**
         * populate
         * @param array $data
         * @param boolean $untag
         * @param boolean $previous 
         */
		public function populate($data,$untag,$previous=false) {
		
			foreach($this->get_columns() as $name=>$column) {
				
				if(array_key_exists($name,$data)) {
					
					$value = $data[$name];

					if($untag) {

						if($column->is_encrypt_aes() && $column->get_encrypt_key())								
							$value = ENCRYPT_UTIL::aes_decrypt($value,$column->get_encrypt_key());
					}	

					$column->set_value($value,$untag);				

					if($previous)
						$this->set_previous_value($name,$value);
				}
				
				if($untag)
					$column->disable_tag();

			}
		}

        /**
         * initialize db
         * @param object $db 
         */
		public function initialize_db($db=null) {

			$columns = $this->get_primary_keys();
			$primary_key = array();
			if(is_array($columns))
				foreach($columns as $name=>$column)
					$primary_key[] = $column!=null ? $name : null;

			$this->_db_object = new DBQ($this->_tablename, $primary_key);

			if(!$this->_logging)
				$this->_db_object->disable_logging();

			if(!$this->_transactions)
				$this->_db_object->disable_transactions();

			if($db)
				$this->_db_object->set_db_connection($db);
		}
		
        /**
         * initialize dbo
         */
		public function initialize_dbo() {
			if($this->_db_object==null)
				$this->initialize_db();
			$this->_db_object->initialize();
		}

        /**
         * initialize
         */
		public function initialize() {
			foreach($this->_columns as $column) {
				$column->disable_tag();
			}
			if($this->_db_object)
				$this->_db_object->initialize();
		}

        /**
         * Returns the primary keys
         * @return array 
         */
		public function get_primary_keys() {
			$primary_columns = array();
			foreach($this->_columns as $name=>$column)
				if($column->is_primary())
					$primary_columns[$name] = $column;
			return $primary_columns;
		}

        /**
         * Returns the column data
         * @param boolean $only_tagged
         * @param boolean $include_pri_key
         * @return array 
         */

		public function get_data($only_tagged=false,$include_pri_key=true) {
			return $this->get_column_data($only_tagged,$include_pri_key);
		}

        /**
         * Returns the column data
         * @param boolean $only_tagged
         * @param boolean $include_pri_key
         * @return array 
         */
		public function get_column_data($only_tagged=false,$include_pri_key=true) {
			$data = array();

			foreach($this->_columns as $name=>$column) {
				
				if($only_tagged && !$column->is_tagged())
					continue;

				if(!$include_pri_key && $column->is_primary())
					continue;

				$value = $this->get_column_value($name);

				if(!is_empty($value) && ($column->get_data_type()=="date" || $column->get_data_type()=="datetime") && is_numeric($value)) {

					if($column->get_data_type()=="date")
						$value = @date("Y-m-d",$value);

					if($column->get_data_type()=="datetime")
						$value = @date("Y-m-d h:i:s",$value);
				}
				
				if($column->is_encrypt_aes() && $column->get_encrypt_key() && strlen($value))
					$value = ENCRYPT_UTIL::aes_encrypt($value,$column->get_encrypt_key());

				$data[$name] = $value;
			}

			return $data;
		}

        /**
         * reset
         */
		public function reset() {
			
		}

        /**
         * untag object
         */
		public function untag_object() {
			foreach($this->_columns as $column)
				$column->disable_tag();
		}

		/*
		public function unchange_object() {
			foreach($this->_columns as $column)
				$column->disable_change();
		}
		*/
		
        /**
         * set previous value
         * @param string $column
         * @param string $value 
         */
		public function set_previous_value($column,$value) {
			$this->_previous_values[$column] = $value;
		}

        /**
         * untag
         * @param string $column_name 
         */
		public function untag($column_name) {
			$column = get_value($this->_columns,$column_name);
			if($column)
				$column->disable_tag();
		}

        /**
         * tag object
         */
		public function tag_object() {
			foreach($this->_columns as $column)
				$column->enable_tag();
		}

        /**
         * untag primary
         */
		public function untag_primary() {
			$columns = $this->get_primary_keys();
			foreach($columns as $column)
				$column->disable_tag();
		}

        /**
         * Checks whether has tagged
         * @return boolean 
         */
		public function has_tagged() {
			foreach($this->_columns as $column)
				if($column->is_tagged())
					return true;
			return false;
		}
		
        /**
         * Checks whether column is tagged
         * @param string $column_name
         * @return boolean 
         */
		public function is_tagged($column_name) {		
			if($column=$this->get_column($column_name))
				return $column->is_tagged();
			return false;
		}

		public function is_column_tagged($column_name) {		
			return $this->is_tagged($column_name);
		}
		
		/*
		public function is_column_changed($column_name) {	
			if($column=$this->get_column($column_name))
				return $column->is_changed();
			return false;
		}
		*/

        /**
         * clear values
         */
		public function clear_values() {
			foreach($this->_columns as $column)
				$column->set_value(null);
		}

        /**
         * Returns the column value
         * @param string $column_name
         * @return null 
         */
		private function get_column_value($column_name) {
			if($column=$this->get_column($column_name))
				return $column->get_value();
			return null;
		}

        /**
         * Updates keys
         * @param array $keys
         * @return boolean 
         */
		public function update($keys=array()) {
			$this->initialize_dbo();

			
			$has_success = true;
			
			if($keys) {
				foreach($keys as $key=>$value)
					$this->_db_object->add_where($key,"=",$value);
			} else {

				$columns = $this->get_primary_keys();

				foreach($columns as $name=>$column) {
					$value = $this->get_column_value($name);

					if($value!=null)
						$this->_db_object->add_where($name,"=",$value);
				}
			}
			
			if(count($data=$this->get_column_data(true,false)) && $this->_db_object->get_constraint_count()) {
				$has_success = $this->_db_object->update($data);
				
				if($has_success)
					$this->untag_object();
			}
			
			return $has_success;
		}

        /**
         * Delete 
         * @param array $keys
         * @return boolean 
         */
		public function delete($keys=array()) {
			$this->initialize_dbo();

			if(count($keys)) {
				foreach($keys as $key=>$value)
					$this->_db_object->add_where($key,"=",$value);
			} else {
				$columns = $this->get_primary_keys();

				if($columns) {
					foreach($columns as $name=>$column) {
						$value = $this->get_column_value($name);
						if($value!=null)
							$this->_db_object->add_where($name,"=",$value);

					}
				}
			}

			if($this->_db_object->get_constraint_count())
				return $this->_db_object->delete();
			return false;
		}

        /**
         * Insert
         * @return boolean 
         */
		public function insert() {
			$this->initialize_dbo();

			$data = $this->get_column_data(true);

			$result = $this->_db_object->insert($data);

			if($result) {
				if($primary_columns=$this->get_primary_keys()) {

					$primary_column = get_value(array_keys($primary_columns),0);
					
					if($primary_column && $this->_db_object->get_last_inserted_id()) {
						$column = $this->get_column($primary_column);
						$column->set_value($this->_db_object->get_last_inserted_id());
					}
				}
				
				$this->untag_object();

			}
			return $result;
		}

        /**
         * replace
         * @param array $keys
         * @return boolean 
         */
		public function replace($keys) {
			// keys is array('fieldname'=> 'value')

			$this->initialize_dbo();

			$data = $this->get_column_data(true);
			return $this->_db_object->replace($keys,$data);			
		}

        /**
         * Select 
         * @param array $fields
         * @param array $where_data
         * @return boolean 
         */
		public function select($fields=array("*"),$where_data=array()) {
			$this->initialize_dbo();

			$columns = $this->get_primary_keys();

			if(count($where_data)==0 && $columns && is_array($columns)) {
				foreach($columns as $name=>$column) {
					$value = $this->get_column_value($name);
					$where_data[$name] = $value;
				}
			}

			if(count($where_data)>0) {

				foreach($where_data as $column=>$value)
					$this->_db_object->add_where($column,"=",$value);

				if($row=$this->_db_object->select_row($fields)) {
				
					foreach($this->_columns as $name=>$column) {

						if($column->is_encrypt_aes() && $column->get_encrypt_key())
							$value = ENCRYPT_UTIL::aes_decrypt($value,$column->get_encrypt_key());
							
						if(isset($row[$name])) {
							$function = "set_".$name;
							$value = $row[$name];

							switch($column->get_data_type()) {
								case "float":
									settype($value,"float");
									break;
								case "int":
									settype($value,"integer");
									break;
							};

							$column->set_value($value);
						}
					}

					$this->untag_object();
					return true;
				} else {
					$this->clear_values();
					return false;
				}
			}


			return false;
		}
		
        /**
         * encrypt column
         * @param string $column_name
         * @param string $key
         * @param string $type 
         */
		public function encrypt_column($column_name,$key,$type=DBO_COLUMN::ENCRYPT_AES) {
			foreach($this->_columns as $name=>$column) {
				if($column_name==$name)
					$column->encrypt_aes($key);
			}
		}
		
        /**
         * Returns the previous values
         * @return array 
         */
		function get_previous_values()			{ return $this->_previous_values; }
                
        /**
         * Returns the previous value
         * @param string $column_name
         * @return string 
         */
		function get_previous_value($column_name) 	{ return get_value($this->_previous_values,$column_name); }
	}
