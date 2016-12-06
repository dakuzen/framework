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

		/**
		* a copy of this model, cloned when start_diff() is called so that we can compare when end_diff is called
		* @var CMODEL clone_cmodel
		*/
		protected $_clone_cmodel 		= null;


        protected $_dirty = [];

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

			foreach($this->describe() as $name=>$config) {
				if($format = value($config,"format")) {
					if(value($format,"type")=="json")
						$this->register_json($name, value($format,"default",[]));
					else if(value($format,"type")=="delimited")
						$this->register_delimited($name, value($format,"delimiter",","), value($format,"default",[]));
					else if(value($format,"type")=="serialized")
						$this->register_serialized($name, value($format,"default",[]));
				}

				if($default=value($config,"default")) {

					$column = $this->get_column($name);

					if($column) {
						if($default=="guid")
							$column->set_value(MISC_UTIL::get_guid());

						elseif($default=="datetime")
							$column->set_value(DB::get_date_time());

						elseif((is_array($default) && is_callable($default)) || (is_object($default) && $default instanceof Closure))
							$column->set_value(call_user_func($default));

						else
							$column->set_value($default);
					}
				}
			}

			return $this;
		}

		function set_dbo($dbo) {
			$dbo
				->on("update",[$this,"on_update_insert"])
				->on("insert",[$this,"on_update_insert"]);

			$this->_dbos[get_class($dbo)] = $dbo; return $this;
		}

		function on_update_insert($dbo, &$data) {

    		foreach($this->describe() as $name=>$config) {

    			if($encode=value($config,"encode")) {

    				if($column=$dbo->get_column($name)) {

    					if($column->is_tagged()) {
    						$data[$name] = call_user_func($encode,$data[$name]);
    					}
	            	}
        		}
        	}
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

		function register_serialized($name,$default=array()) {
			$this->_registered_objects[$name] = array("type"=>"serialized","default"=>$default);
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

						if(count($args)==2) {

							$data = (array)$this->data($name);
							$data[$args[0]] = $args[1];
							$this->data($name,$data);

						} else {
							$this->data($name,$args[0]);
						}

						if($object=value($this->_registered_objects,$name)) {

							foreach($this->get_dbos() as $dbo) {

								if($column=value($dbo->get_columns(),$name)) {

									$objects = $this->data($name);

									if($object["type"]=="json")
										$objects = JSON_UTIL::encode($objects);

									elseif($object["type"]=="serialized")
										$objects = serialize($objects);

									elseif($object["type"]=="delimit" && is_array($objects))
										$objects = implode($object["delimiter"],$objects);

									$dbo->{"set_".$name}($objects);
								}
							}
						}

						return $this;

					} elseif($this->has_data($name)) {

						$data = (array)$this->data($name);

						if(count($args)==1)
							return value($data,$args[0]);

						return $data;
					}

					return value($this->_registered_objects,array($name,"default"));

				} else {

					foreach($this->_dbos as &$dbo) {

						if($dbo->has_column($name)) {

							if($matches[1]=="set_") {

								if(count($args)>0) {

									$arg1 	= $args[0];
									$arg2 	= isset($args[1]) ? $args[1] : null;

									$dbo->$function($arg1,$arg2);

									$has_success = true;
								} else
									throw new Exception('Missing argument 1 for '.get_class($this).'->'.$function.'() ');

							} else
								return $dbo->$function();
						}
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
		public function is_column_tagged($columns,$table="") {
			$columns = (array)$columns;
			foreach($this->_dbos as $dbo)
				if($table==$dbo->get_tablename() || !$table)
					foreach($columns as $name)
						if($column=$dbo->get_column($name))
							if($column->is_tagged())
								return true;

			return false;
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

		public function populate($data,$untag=false) {

			if(!is_array($data) || !$data)
				return $this;

			foreach($data as $field=>$value) {

				$function = "set_".$field;

				if(method_exists($this,$function)) {
					$this->$function($value);
					unset($data[$field]);
				}
			}

			foreach($this->_registered_objects as $name=>$object) {

				if(@array_key_exists($name,$data)) {

					$value = value($data,$name);

					if(is_string($value)) {

						if(value($object,"type")=="delimit") {
							$value = strlen($value) ? (array)explode(value($object,"delimiter"),$value) : array();
							$this->data($name,$value);

						} elseif(value($object,"type")=="json") {

							$json = [];
							if($value)
								$json = JSON_UTIL::decode($value);
							$this->data($name,$json);
						} elseif(value($object,"type")=="serialized") {
							$this->data($name,unserialize($value));
						}

						unset($data[$name]);

					} elseif(is_array($value) || is_object($value))
						$this->data($name,$value);
				}
			}

			foreach($this->_dbos as &$dbo) {
				$dbo->populate($data,$untag);
				if($untag)
					$dbo->untag_object();
			}

			return $this;
		}

		// Legacy function
		public function populate_dbo($data,$untag=false) {
			return $this->populate($data,$untag);
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
        public function describe() { return []; }

        public function validate_message($config,$string,$vars=[]) {

        	$subject = value($config,"humanize");

        	if(!$subject) {
        		$subject = preg_replace("/^get_/i","",$config["name"]);
        		$subject = str_replace('_', ' ', $subject);
        	}

        	$replace = ["{name}"=>$subject];

        	foreach ($vars as $key=>$value) {
        		$replace["{".$key."}"] = $value;
        	}

        	return str_replace(array_keys($replace),array_values($replace),$string);
        }

        public function fill($data,$allow) {

        	foreach($allow as $name) {
        		$config = value($this->describe(),$name);
        		if($config || is_array($config)) {

        			if(!array_key_exists($name,$data))
        				continue;

        			$value = value($data,$name);

        			if($sanitize=value($config,"sanitize")) {

        				if(strlen($value)) {

		        			if($sanitize=="time") {
		        				$value = CMODEL_TIME::create($value);
		        			} else {

		        				if($sanitize instanceof Closure) {
		        					$value = $sanitize($value);
		        				} else {
		        					$value = call_user_func([$this,$sanitize],$value);
		        				}
		        			}
                        }else{
                            if($sanitize=="time") {
                                $value = null;
                            }
                        }
	        		}

	        		if(value($config,"source")=="function") {
	        			call_user_func([$this,$name],$value);
					} else if(isset($this->_registered_objects[$name])) {
						$this->{"set_".$name}($value);
	        		} else {
						foreach($this->_dbos as $dbo) {
							if($column=$dbo->get_column($name))
								$column->set_value($value);
						}
	        		}
	        	}
        	}

        	return $this;
        }

        public function validate($options=["validate"=>true]) {

        	if(is_bool($options))
        		$options = ["validate"=>$options];

        	if(!value($options,"validate")) {
        		return $this;
        	}

        	$exception = BCMODEL_VALIDATION_EXCEPTION::create();
        	foreach($this->describe() as $name=>$config) {

        		$config["name"] = $name;

        		if(value($config,"source")=="function")
	              	$value = call_user_func([$this,$name]);
        		else if(value($config,"source")=="object")
	              	$value = call_user_func([$this,$name]);
				else if(in_array(value($config,["format","type"]), ["json","delimited","serialized"]))
					$value = $this->{"get_".$name}();
	            else {
	            	$column = $this->get_column($name);

	            	if(!$column)
	            		throw new Exception("CMODEL::validate() Failed: The name ".$name." does not exist");

	            	$value = $column->get_value();
	            }

        		foreach(value($config,"validations",[]) as $type=>$validation) {

        			if($type=="required")
        				if($validation && !strlen(trim($value)))
							$exception->add($this->validate_message($config,value($validation,["messages","required"],"The {name} is required")));

					if(is_string($value) && !strlen(trim($value)))
						continue;

        			if($type=="minlength") {
        				if(strlen(trim($value))<$validation)
							$exception->add($this->validate_message($config,value($validation,["messages","minlength"],"The {name} has to be a minimum of {min} characters"),["min"=>$validation]));

        			} elseif($type=="maxlength") {
        				if(strlen(trim($value))>$validation)
							$exception->add($this->validate_message($config,value($validation,["messages","maxlength"],"The {name} cannot exceed {max} characters"),["max"=>$validation]));

        			} elseif($type=="email") {
        				if($validation && trim($value) && !CMODEL_VALIDATE::create($value)->email())
							$exception->add(value($config,["messages","email"],"The email address is invalid"));

					} elseif($type=="integer") {
        				if($validation && trim($value) && !preg_match("/^\d+$/",$value))
        					$exception->add($this->validate_message($config,value($validation,["messages","integer"],"The {name} is not a valid number")));

					} elseif($type=="range") {
        				if($validation && trim($value) && ($value<$validation[0] || $value>$validation[1]))
        					$exception->add($this->validate_message($config,value($validation,["messages","range"],"The {name} must be between ".$validation[0]." and ".$validation[1])));

					} elseif($type=="lesser") {
        				if(is_numeric($validation) && trim($value) && $value>$validation)
        					$exception->add($this->validate_message($config,value($validation,["messages","lesser"],"The {name} cannot be less then ".$validation)));

					} elseif($type=="greater") {
        				if(is_numeric($validation) && trim($value) && $value<$validation)
        					$exception->add($this->validate_message($config,value($validation,["messages","greater"],"The {name} must be greater then ".$validation)));

					} elseif($type=="decimal") {
        				if($validation && trim($value) && !preg_match("/^\d+(\.\d*)?$/",$value))
        					$exception->add($this->validate_message($config,value($validation,["messages","decimal"],"The {name} is not a valid decimal")));

        			} elseif($type!="required") {

        				try {
        					call_user_func($validation,$value);
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



		public function start_diff($empty_cmodel=null) {
			if($empty_cmodel)
				$this->_clone_cmodel = $empty_cmodel;
			else
				$this->_clone_cmodel = $this->clone();

			//ensure all the needed subobject lists are loaded
			foreach($this->describe() as $name=>$config) {
				if(value($config,"source")=="function") {
					$this->$name(true);
					$this->_clone_cmodel->$name(true);

				} elseif(value($config,"source")=="object") {
					if($this->$name(true))
						$this->$name(true)->start_diff();

					if($this->_clone_cmodel->$name(true))
						$this->_clone_cmodel->$name(true)->start_diff();
				}
			}
			return $this;
		}


		public function end_diff() {

			if(!$this->_clone_cmodel)
				throw new Exception("Unable to calculate changes on ".get_class($this).", start_diff() was not called.");

			$diff = [];

			foreach($this->describe() as $name=>$config) {

				//field is excluded from diff, just skip it
				if(value($config,"diff")===false) {
					continue;


				//compare a field value, then get a nice display value from an array for the diff.  ie.  compare state values, but use state name
				} elseif(value($config,"diff-value")) {
					$label = value($config,["diff-value","label"], ucwords(str_replace("_", " ", $name)));

					if($this->{"get_".$name}()!=$this->_clone_cmodel->{"get_".$name}() || value($config,["diff-lookup","always"])) {
						$list = call_user_func($config["diff-value"]["value"][0]."::".$config["diff-value"]["value"][1]);
						$value = value($list, $this->{"get_".$name}());
						$diff[] = $label." = `".$value."`";
					}


				//compare a field, but lookup the human name.  i.e. compare office_id but lookup the office name if it changed
				} elseif(value($config,"diff-lookup")) {
					$label = value($config,["diff-lookup","label"], ucwords(str_replace("_", " ", $name)));

					if($this->{"get_".$name}()!=$this->_clone_cmodel->{"get_".$name}() || value($config,["diff-lookup","always"])) {
						$callstack = value($config,["diff-lookup","value"]);
                        $object = $callstack[0]->{$callstack[1]}(true);
                        if($object){
                            $value = $object->{$callstack[2]}();
                        }else{
                            $value = 'NONE';
                        }
						$diff[] = $label." = `".$value."`";
					}


				//compare lists of child objects. used when adding/subtracting items from a list. i.e. adding a user permission
				} elseif(value($config,"diff-list")) {
					$new_list = $this->$name();
					$old_list = $this->_clone_cmodel->$name();
					$compare_func = value($config,["diff-list","compare"]);
					$value_func = value($config,["diff-list","value"]);
					$label = value($config,["diff-list","label"], ucwords(str_replace("_", " ", $name)) );

					foreach($new_list as $new) {
						foreach($old_list as $old) {
							if($new->$compare_func()==$old->$compare_func())
								continue 2;
						}

						$diff[] = "Added ".$label." `".$new->$value_func()."`";
					}

					foreach($old_list as $old) {
						foreach($new_list as $new) {
							if($new->$compare_func()==$old->$compare_func())
								continue 2;
						}

						$diff[] = "Removed ".$label." `".$old->$value_func()."`";
					}


				} else {
					$label = value($config,["diff","label"], ucwords(str_replace("_", " ", $name)));

					//compare fields in child object. i.e. an office->get_address()
					if(value($config,"source")=="object") {

						$child_diff = $this->$name()->end_diff();
						foreach($child_diff as $row) {
							$diff[] = $label." ".$row;
						}

					//boring old default compare of 2 fields.  i.e.  users name
					} else {
						$column = $this->get_column($name);
						$value1 = $this->{"get_".$name}();
						$value2 = $this->_clone_cmodel->{"get_".$name}();

						if($column->is_data_type_datetime() || $column->is_data_type_date()) {
							if(!is_a($value1,"CMODEL_TIME"))
								$value1 = CMODEL_TIME::create($value1);
							if(!is_a($value2,"CMODEL_TIME"))
								$value2 = CMODEL_TIME::create($value2);
						}

						if($value1!=$value2 || value($config,["diff","always"])) {) {
							$diff[] = $label." = `".$value1."`";
						}
					}
				}
			}


			return $diff;
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
	}

	class CMODEL extends MODEL_COMPLEX {}