<?
    /**
    * Class BASE_CMODEL_SMARTY_OBJECT
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_SMARTY_OBJECT extends MODEL {
				
        /**
         * The data
         * @var array $data 
         */
		protected $_data = array();
		
        /**
         * Constructor
         * @param array $data 
         */
		public function __construct($data=array()) {
			
			foreach($data as $key => $value) 
				$this->add_data($key,$value);

			return $this;
		}
		
		/**
		 * Add functions
		 * @param object $object
		 * @param array $functions 
		 */
		public function add_functions($object,$functions) { 

			foreach($functions as $function) {
				
				$value 	= call_user_func(array($object,$function));
				$key 	= preg_replace("/^(set_|get_)/","",$function);

				$this->add_data($key,$value);
			}

			return $this;
		}

		public function add_function($name,$function) { 
			$this->$name = $function;
			return $this;
		}

        /**
         * Add data
         * @param string $name
         * @param string $value 
         */
		public function add_data($name,$value) { 
			$this->_data[$name] = $value; 
			$this->{$name} = $value;
			return $this;
		}

		public function data($name,$value="") { 
			if(func_num_args()==2)
				return $this->add_data($name,$value);

			return value($this->_data,$name);
		}
		
		public function init($data) {
			
			foreach($data as $key) 
				$this->add_data($key,"");

			return $this;
		}

        /**
         * Call
         * @param string $function
         * @param array $args
         * @return string
         * @todo check return type 
         */
		public function __call($function,$args) {

			if(method_exists($this,$function))
				return call_user_func_array(array($this,$function),$args);

			preg_match("/^(set_|get_)?(.*)/",$function,$matches);
			
			$name = get_value($matches,2);

			if(array_key_exists($name,$this->_data))
				return value($this->_data,$name);		
								
			$has_property = isset($this->$name);
			
			if($has_property) {

				if(get_value($matches,1)=="set_")
					$this->$name = get_value($args,0);

				return $this->$name;
			}
		}	
	}