<?

	/**
	* Class MODEL_COMPLEX  
	*
	* @package Library
	* @subpackage Common
	* @method static $this create() return self
	*/
	 	
	class MODEL_COMPLEX extends MODEL {
		
		/**
		* The dbos
		* @var array $dbos
		*/
		protected $_dbos 				= array();
		protected $_registered_objects	= array();
		                    
		/**
		* is debug
		* @var boolean is_debug 
		*/
		protected $_is_debug		 	= false;


        protected $_rules = [];
        protected $_dirty = [];
        protected $_sanitized = [];
        protected $_validate_errors = [];
        protected $_skip_save = false;
						
		public function __construct($dbo=null) {
				
			$dbos = is_array($dbo) ? $dbo : array($dbo);

			foreach($dbos as $dbo)
				if($dbo)
					$this->register_dbo($dbo);

			return $this;
		}

		/**
		* register dbo
		* @param object $dbo 
		*/
		function register_dbo($dbo) {
			if(!array_key_exists(get_class($dbo),$this->_dbos))
				$this->set_dbo($dbo);

			return $this;
		}

		function set_dbo($dbo) {
			$this->_dbos[get_class($dbo)] = $dbo; return $this;
		}
		
		/**
		* register db
		* @param object $db 
		*/
		function register_db($db=null) {
			
			$db = $db ? $db : DB::get_default_db_connection();
			
			foreach($this->_dbos as $dbo)
				$dbo->set_db_connection($db);

			return $this;
		}

		function register_dbo_db($db=null) {
			return $this->register_db();
		}

		function register_delimited($name,$delimiter=",",$default=array()) {
			$this->_registered_objects[$name] = array("type"=>"delimit","delimiter"=>$delimiter,"default"=>$default);
			return $this;
		}

		function register_json($name,$default=array()) {
			$this->_registered_objects[$name] = array("type"=>"json","default"=>$default);
			return $this;
		}

		function update_registered($registered_name,$key,$value) {
			$object = $this->get_{$registered_name}();
			$object[$key] = $value;
			$this->set_{$registered_name}($object);
			return $this;
		}
		
		/**
		* Returns the dbos
		* @return array 
		*/
		function get_dbos() 		{ return $this->_dbos; }
        

        function dbo($name) {
        	return $this->get_dbo($name);
        }

		/**
		* Returns the dbo
		* @param string $name
		* @return object 
		*/
		function get_dbo($name) { 
			
			$name = strtoupper($name);

			$dbo = value($this->_dbos,$name);

			if(!$dbo)
				$dbo = value($this->_dbos,"DBO_".$name);

			if(!$dbo)
				$dbo = value($this->_dbos,"BASE_DBO_".$name);

			return $dbo;			
		}
                
		/**
		* Checks whether is debug
		* @return boolean 
		*/
		function is_debug() 		{ return $this->_is_debug; }
                
		/**
		* Checks whether is debug
		* @return boolean 
		*/
		function log($message) { 
			if($this->is_debug())
				p($message);
		}
                
		/**
		* debug
		* @param boolean $debug
		* @return boolean 
		*/
		function debug($debug=true) 	{ $this->_is_debug = $debug; return $this; }
		

		/**
		 * Call
		 * @param string $function
		 * @param array $args
		 * @return boolean
		 * @throws Exception 
		 */
		function __call($function,$args) {
				
			if($function=="create") {
				$class = get_called_class();
				return new $class();
			}

			$has_success = false;			
			if(preg_match("/^(?:(set_)|get_)(.*)/",$function,$matches)) {
			
				$name = $matches[2];

				if(array_key_exists($name,$this->_registered_objects)) {

					if($matches[1]=="set_") {

						$this->data($name,$args[0]);

						if($object=value($this->_registered_objects,$name)) {

							foreach($this->get_dbos() as $dbo) {

								if($column=value($dbo->get_columns(),$name)) {

									$objects = $this->data($name);

									$set = "set_".$name;
									if($object["type"]=="json")
										$objects = JSON_UTIL::encode($objects);

									elseif($object["type"]=="delimit" && is_array($objects))
										$objects = implode($object["delimiter"],$objects);

									$dbo->$set($objects);
								}
							}
						}
					
						return $this;
					}
					
					elseif($this->has_data($name))
						return (array)$this->data($name);
					
					return value($this->_registered_objects,array($name,"default"));
				}
				
				foreach($this->_dbos as &$dbo) {

					if($dbo->has_column($name)) {
					
						if($matches[1]=="set_") {
							
							if(count($args)>0) {

								$arg1 	= $args[0];
								$arg2 	= isset($args[1]) ? $args[1] : null;
								$arg3 	= isset($args[2]) ? $args[2] : null;
														
								$dbo->$function($arg1,$arg2);

								if($arg3)
									$this->untag_column($name);

								$has_success = true;
							} else								
								throw new Exception('Missing argument 1 for '.get_class($this).'->'.$function.'() ');
							
						} else 
							return $dbo->$function();
					}
				}
			}
			
			if(!$has_success)				
				return parent::__call($function,$args);

			return $this;
		}
		
		/**
		* Checks whether column is tagged
		* @param string $column
		* @param string $table
		* @return null 
		*/
		public function is_column_tagged($column,$table="") {
			foreach($this->_dbos as $dbo) {
				if($table) {
					if($table==$dbo->get_tablename())
						return $dbo->is_column_tagged($column);
					
				} elseif($dbo->has_column($column))
					return $dbo->is_column_tagged($column);					
			}
			
			return null;
		}
		
		/*
		removed because there is complexity with keeping track of the state of a column value
		function is_column_changed($column,$table="") {
			foreach($this->_dbos as $dbo) {
				if($table) {
					if($table==$dbo->get_tablename())
						return $dbo->is_column_changed($column);
					
				} elseif($dbo->has_column($column))
					return $dbo->is_column_changed($column);					
			}
			
			return false;
		}
		*/
		
        /**
         * Returns previous value
         * @param string $column
         * @param string $table
         * @return null 
         */
		public function get_previous_value($column,$table="") {
			foreach($this->_dbos as $dbo) {
				if($table) {
					if($table==$dbo->get_tablename())
						return $dbo->get_previous_value($column);
					
				} elseif($dbo->has_column($column))
					return $dbo->get_previous_value($column);
			}
			
			return null;
		}
		
        /**
         * Returns column names
         * @return array 
         */
		public function get_column_names() {
			$column_names = array();
			foreach($this->_dbos as $dbo)
				$column_names  += $dbo->get_column_names();
			
			return $column_names;
		}
		
        /**
         * Checks whether has column
         * @param string $column
         * @return boolean 
         */
		public function has_column($column) {
			foreach($this->_dbos as $dbo)
				if($dbo->has_column($column))
					return true;
			
			return false;
		}		
		
        /**
         * Returns column
         * @param string $column
         * @param string $table
         * @return null 
         */
		public function get_column($column,$table="") {
			foreach($this->_dbos as $dbo) {
				if($table) {
					if($table==$dbo->get_tablename())
						return $dbo->get_column($column);
					
				} elseif($dbo->has_column($column))
					return $dbo->get_column($column);					
			}
			
			return null;
		}
		
        /**
         * Return column values
         * @return array 
         */
		public function get_column_values() {
			$values = array();
			
			foreach($this->_dbos as $dbo) 
				foreach($dbo->get_columns() as $name=>$column)
					$values[$name] = $column->get_value();					
			
			return $values;
		}		
		
        /**
         * Populate dbo
         * @param array $data
         * @param boolean $untag
         * @param boolean $previous
         * @return boolean 
         */
		public function populate_dbo($data,$untag,$previous=false) {

			if(!is_array($data) || !$data)
				return $this;

			foreach($data as $field=>$value) {

				$function = "set_".$field;
				
				if(method_exists($this,$function)) {
					$this->$function($value);
					unset($data[$field]);
				}
			}
	
			foreach($this->_dbos as &$dbo)
				$dbo->populate($data,$untag,$previous);

			return $this;
		}
		
        /**
         * Returns the dbo data
         * @return array 
         */
		public function get_dbo_data() {
			$data = array();
			
			foreach($this->get_dbos() as $dbo)
				$data = $data + $dbo->get_data();
			
			return $data;
		}		
		
	    /**
	     * untag object
	     */
		public function untag_object() {
			foreach($this->get_dbos() as $dbo)
				$dbo->untag_object();
			return $this;
		}

		/**
		* tag object
		*/
		function tag_object() {
			foreach($this->_dbos as $dbo)
				$dbo->tag_object();
			return $this;
		}
		
        /**
         * untag column
         */
		public function untag_column($column) {
			if($column=$this->get_column($column))
				$column->disable_tag();

			return $this;
		}	

		public function populate($data,$untag=false,$previous=false) {

			foreach($this->_registered_objects as $name=>$object) {

				if(@array_key_exists($name,$data)) {
							
					$value = value($data,$name);

					if(is_string($value)) {

						if(value($object,"type")=="delimit") {
							$value = strlen($value) ? (array)explode(value($object,"delimiter"),$value) : array();
							$this->data($name,$value);

						} elseif(value($object,"type")=="json") 
							$this->data($name,JSON_UTIL::decode($value));
						
						unset($data[$name]);

					} elseif(is_array($value) || is_object($value)) 
						$this->data($name,$value);
				}
			}

			return $this->populate_dbo($data,$untag,$previous);
		}

		public function load() {

			foreach($this->get_dbos() as $dbo)
				$dbo->select();

			return $this;
		}

        /**
         * Clone
         */
		function __clone() {
			
			parent::__clone();
			
			$this->clone_map_dbo($this);
		}
		
		//Note: All child CMODEL and DBOs must have a protected (no private) scope to be cloned and mapped
		
        /**
         * clone map dbo
         * @param object $object 
         */
		function clone_map_dbo($object) {
			
			foreach($this as $key=>$val) {
			
				if($val instanceof MODEL_COMPLEX) 
					$val->clone_map_dbo($object);
				
				if($val instanceof DBO)
					$object->set_dbo($val);
			}
		}


        public function exist(){
            return $this;
        }


        protected $_validations = [];
        public function validations() { return []; }

        public function validate_message($validation,$string,$vars=[]) {
        	
        	$subject = value($validation,"humanize");

        	if(!$subject) {
        		$subject = preg_replace("/^get_/i","",$validation["name"]);
        		$subject = str_replace('_', ' ', $subject);
        	}

        	$replace = ["{name}"=>$subject];

        	foreach ($vars as $key=>$value) {
        		$replace["{".$key."}"] = $value;
        	}

        	return str_replace(array_keys($replace),array_values($replace),$string);
        }

        public function validate() {

        	$exception = BCMODEL_VALIDATION_EXCEPTION::create();
        	foreach($this->validations() as $validation) {

        		$name = $validation["name"];

              	$value = call_user_func([$this,$name]);

        		foreach(value($validation,"validations",[]) as $type=>$config) {

        			if($type=="required" && !strlen($value)) {
						$exception->add($this->validate_message($validation,value($validation,["messages","required"],"The {name} is required")));
        			}

        			if($type=="minlength" && strlen($value)<$config) {
						$exception->add($this->validate_message($validation,value($validation,["messages","minlength"],"The {name} has to be a minimum of {min} characters"),["min"=>$config]));
        			}        			

        			if($type=="maxlength" && strlen($value)>$config) {
						$exception->add($this->validate_message($validation,value($validation,["messages","maxlength"],"The {name} cannot exceed {max} characters"),["max"=>$config]));
        			}

        			if($type=="email") {
						try {
							CMODEL_VALIDATE::create($value)->email();
						} catch(Exception $e) {
							$exception->add(value($validation,["messages","email"],"The email address is invalid"));
						}
        			}

        			if($type=="custom") {
        				try {
        					call_user_func($config,$value);
        				} catch(Exception $e) {
        					$exception->add($e);
        				}
        			}
        		}
        	}

        	if($exception->has_messages())
        		throw $exception;        		

        	return $this;
        }

		public function array_merge_recursive_distinct ( array &$array1, array &$array2 )
		{
			$merged = $array1;

			foreach ( $array2 as $key => &$value )
			{
				if ( is_array ( $value ) && isset ( $merged [$key] ) && is_array ( $merged [$key] ) )
				{
					$merged [$key] = $this->array_merge_recursive_distinct ( $merged [$key], $value );
				}
				else
				{
					$merged [$key] = $value;
				}
			}

			return $merged;
		}

        public function setup_rules($options = []){
            $rules = $this->_rules;

            if (array_key_exists('rules', $options))
                $rules = $this->array_merge_recursive_distinct($rules, $options['rules']);

            if (array_key_exists('skip_required', $options) && $options['skip_required']===true)
                foreach ($rules as $key=>$rule)
                    unset($rules[$key]['required']);

            $this->_rules = $rules;
            return $this;
        }

        public function setup($data, $options=[]){
            $this->_dirty = $data;
            $this->setup_rules($options);
            $this->_sanitized = filter_var_array($this->_dirty, $this->_rules, true);

            if ($this->_sanitized)
                foreach ($this->_sanitized as $key=>$value){
                    if (is_null($value) && !empty($this->_rules[$key]['default'])){
                        if (is_callable($this->_rules[$key]['default']))
                            $this->_sanitized[$key] = $value = call_user_func($this->_rules[$key]['default']);
                        else
                            $this->_sanitized[$key] = $value = $this->_rules[$key]['default'];
                    }

                    if ($value === false){
                        $message = 'Wrong '.ucfirst(str_replace('_', ' ', $key)) . " value";
                        if (!empty($this->_rules[$key]['message']))
                            $message = $this->_rules[$key]['message'];

                        if (array_key_exists('collect', $options) && $options['collect']==true)
                            $this->_validate_errors[$key] = $message;
                        else
                            throw new Exception($message);
                    }


                    if (!empty($this->_rules[$key]['required']) && $this->_rules[$key]['required'] == true && is_null($this->{'get_'.$key}()) && is_null($value)){
                        $message = ucfirst(str_replace('_', ' ', $key)) . " is required";

                        if (array_key_exists('collect', $options) && $options['collect']==true)
                            $this->_validate_errors[$key] = $message;
                        else
                            throw new Exception($message);
                    }

                    if (!is_null($value) && $value !== false)
                        $this->{'set_'.$key}($value);

                }
            return $this;
        }

        /**
         * @return array
         */
        public function get_validate_errors()
        {
            return $this->_validate_errors;
        }

        /**
         * return bool
         */
        public function skip_save(){
            return $this->_skip_save;
        }

        /**
         * @return array
         */
        public function get_sanitized(){
            return $this->_sanitized;
        }

        /**
         * @return array
         */
        public function get_dirty(){
            return $this->_dirty;
        }
	}

	class CMODEL extends MODEL_COMPLEX {}