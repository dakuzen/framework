<?php
	/**
        * Class HTTP_HEADER  
        *
        * @package Library
        * @subpackage Transport
        */
	class HTTP_HEADER implements Iterator, ArrayAccess, Countable {

		const ORIGINAL_CASE 	= 'fields';
		const LOWER_CASE 	= 'lowerCase';
		const CAMEL_CASE 	= 'camelCase';

		/**
		 * The iteration Style
		 * @var string $iterationStyle 
		 */
		public $iterationStyle 	= self::LOWER_CASE;

		/**
		 * Fields
		 * @var array $fields
		 */
		protected $fields 	= array();
		
		/**
		 * Camel case
		 * @var string $camelCase
		 */
		protected $camelCase 	= null;
		
		/**
		 * Lower case
		 * @var string $lowerCase
		 */
		protected $lowerCase 	= null;

		/**
		 * Constructor
		 * @param array $fields 
		 */
		public function __construct($fields) {
			$this->fields = $fields;
		}

		/**
		 * Call
		 * @param string $function
		 * @param array $args
		 * @return boolean 
		 */
		function __call($function,$args) {
			
			if(preg_match("/^get_(.*)/",$function,$matches)) {
				
				foreach($this->fields as $key=>$value) {
					$field = str_replace("_","-",get_value($matches,1));
					
					if(preg_match("/^".$field."$/i",$key))
						return $value;
				}
			}
			
			trigger_error ('Call to undefined method '.get_class($this).'::'.$function.'() ', E_USER_ERROR);
		}
				
		/**
		 * Set
		 * @param string $key
		 * @param string $value 
		 */
		public function __set($key,$value) {
			$this->camelCase[$key] = $value;
		}

		/**
		 * Isset
		 * @param string $key
		 * @return boolean 
		 */
		public function __isset($key) {
			if (is_null($this->camelCase)) 
				$this->camelCaseFields();
			
			return isset($this->camelCase[$key]);
		}

		/**
		 * Unset
		 * @param string $key 
		 */
		public function __unset($key) {
			if (is_null($this->camelCase))
				$this->camelCaseFields();
		
			unset($this->camelCase[$key]);
		}

		/**
		 * offset Exists
		 * @param string $key
		 * @return boolean 
		 */
		public function offsetExists($key) {
			if (is_null($this->lowerCase))
				$this->lowerCaseFields();
			
			$key = strtolower($key);
			return isset($this->lowerCase[$key]);
		}

		/**
		 * offset Get
		 * @param string $key
		 * @return null 
		 */
		public function offsetGet($key) {
			if (is_null($this->lowerCase)) 
				$this->lowerCaseFields();
			
			$key = strtolower($key);
			if (isset($this->lowerCase[$key])) 
				return $this->lowerCase[$key];
			return null;
		}

		/**
		 * offset Set
		 * @param string $key
		 * @param string $value 
		 */
		public function offsetSet($key,$value) {
			$key = strtolower($key);

			$this->lowerCase[$key] = $value;
		}

		/**
		 * offset Unset
		 * @param string $key 
		 */
		public function offsetUnset($key) {
			if (is_null($this->lowerCase))
				$this->lowerCaseFields();
			
			$key = strtolower($key);
			unset($this->lowerCase[$key]);
		}

		/**
		 * Count
		 * @return integer 
		 */
		public function count() {
			return count($this->fields);
		}

		/**
		 * Current
		 * @return mixed 
		 */
		public function current() {
			return current($this->{$this->iterationStyle});
		}
		
		/**
		 * Returns field
		 * @param string $key
		 * @return mixed 
		 */
		public function get_field($key) { return get_value($this->fields,$key); }
		
		/**
		 * Returns fields
		 * @return array 
		 */
		public function get_fields() { return $this->fields; }

		/**
		 * Key
		 * @return mixed
		 */
		public function key() {
			return key($this->{$this->iterationStyle});
		}

		/**
		 * Next
		 * @return mixed 
		 */
		public function next() {
			return next($this->{$this->iterationStyle});
		}

		/**
		 * rewind
		 */
		public function rewind() {
			if (is_null($this->{$this->iterationStyle})) {
				$m = $this->iterationStyle."Fields";
				$this->$m();
			}
			reset($this->{$this->iterationStyle});
		}

		/**
		 * Valid
		 * @return boolean 
		 */
		public function valid() {
			return (boolean) current($this->{$this->iterationStyle});
		}

		/**
		 * lower Case Fields
		 */
		protected function lowerCaseFields() {
			$fields = array();
			foreach($this->fields as $k => $v)
				$fields[strtolower($k)] = $v;
			
			$this->lowerCase = $fields;
		}
		
		/**
		 * camel Case Fields
		 */
		protected function camelCaseFields() {
			$fields = array();
			foreach($this->fields as $k => $v) {
				$pieces = explode('-',$k);
				$pieces = array_map('ucfirst',$pieces);
				$fields[implode('',$pieces)] = $v;
			}
			$this->camelCase = $fields;
		}
	}
