<?php

    /**
    * Class REGISTRY
    *
    * @package Library
    * @subpackage Common
    */

	class REGISTRY {

		const PARM_URI 				= "PU";
		const PARM_REQUEST 			= "PR";
		const PARM_POST 			= "PP";
		const PARM_GET 				= "PG";
		const PARM_RAW_REQUEST 		= "RR";
		const PARM_RAW_POST			= "RP";
		const PARM_RAW_GET 			= "RG";
		const PARM_STRING_REQUEST 	= "SR";
		const PARM_STRING_POST		= "SP";
		const PARM_STRING_GET 		= "SG";
		const PARM_LANG				= "PL";
		const PARM_REQUEST_INPUT	= "RI";

		const IMPORT_FRAMEWORK		= "F";
		const IMPORT_APP			= "A";

		const OPTION_RAW 			= "raw";
		const OPTION_STRING			= "string";

		/**
		 *The data
		 * @var array $data
		 */
		private $_data 			= array();

		/**
		 * The parms
		 * @var array $parms
		 */
		private $_parms 		= array();

		/**
		 * message handler
		 * @var object $message_handler
		 */
		private $_message_handler 	= null;

		/**
		 * instance
		 * @var object $instance
		 */
		private static $_instance 	= null;

		/**
		 * Constructor
		 */
		protected function __construct() {
			$this->_message_handler = new MESSAGE_HANDLER();
		}

        /**
         * Returns the instance
         * @return object
         */
		static function get_instance() {
			if(!self::$_instance)
				self::$_instance = new REGISTRY();

			return self::$_instance;
		}

		static function instance() {
			return self::get_instance();
		}

        /**
         * Returns message handler
         * @return object
         */
		function get_message_handler()	{ return $this->_message_handler; }

		/**
		 * Set parm
		 * @param string $key
		 * @param string $var
		 */
		function set_parm($key, $var) 		{ $this->_parms[$key] = $var; return $this; }

		function set_post_parms($var) 		{ return $this->set_parm(self::PARM_POST,$var); }
		function set_get_parms($var) 		{ return $this->set_parm(self::PARM_GET,$var); }
		function set_request_parms($var) 	{ return $this->set_parm(self::PARM_REQUEST,$var); }

		function set_posts($var) 			{ return $this->set_post_parms($var); }
		function set_gets($var) 			{ return $this->set_get_parms($var); }
		function set_requests($var) 		{ return $this->set_request_parms($var); }

		/**
		 * Checks whether it has parm
		 * @param string $key
		 * @return boolean
		 */
		function has_parm($key) 	{ return isset($this->_parms[$key]); }

		/**
		 * Returns parm
		 * @param array $parm_keys
		 * @param string $default
		 * @return mixed
		 */
		function get_parm($parm_keys,$default=null) {

			if(is_array($parm_keys)) {
				$parm_num_keys = count($parm_keys);
				$value = $this->_parms;
				for($i=0;$i<$parm_num_keys;$i++) {

					$parm_key_value = get_value($parm_keys,$i);

					if($parm_key_value!==null && isset($value[$parm_key_value]))
						$value = $value[$parm_keys[$i]];
					else
						return $default;
				}

				return $value;
			} else
				return get_value($this->_parms,$parm_keys,$default);
		}

        /**
         * Remove parm
         * @param string $var
         */
		function remove_parm($var) {
			unset($this->_parms[$var]);
		}

        /**
         * Set data
         * @param string $key
         * @param string $var
         */
		function set_data($key, $var) { $this->_data[$key] = $var; }

        /**
         * Returns data
         * @param string $key
         * @return null
         */
		function get_data($key) {
			if (isset($this->_data[$key]) == false)
				return null;
			return $this->_data[$key];
		}

        /**
         * Remove data
         * @param string $var
         */
		function remove_data($var) {
			unset($this->_data[$var]);
		}

	    /**
	     * Return parms
	     * @return mixed
	     */
		function requests() 				{ return $this->get_parm(REGISTRY::PARM_REQUEST); }
		function get_request_parms() 		{ return $this->requests(); }

        /**
         * Return post parms
         * @return mixed
         */
		function posts() 					{ return $this->get_parm(REGISTRY::PARM_POST); }
		function get_post_parms() 			{ return $this->posts(); }

        /**
         * Return get parms
         * @return mixed
         */
		function gets() 					{ return $this->get_parm(REGISTRY::PARM_GET); }
		function get_get_parms() 			{ return $this->gets(); }

        /**
         * set request parm
         * @param string $parm
         * @param string $values
         * @todo review all param types
         */
		function set_request_parm($parm,$values)	{ $this->_parms[REGISTRY::PARM_REQUEST][$parm] = $values; }

        /**
         * Check whether has post parm
         * @param string $parm
         * @return boolean
         */
		function has_post($parm) 				{ return array_key_exists($parm,$this->get_parm(REGISTRY::PARM_POST)); }
		function has_get($parm) 				{ return array_key_exists($parm,$this->get_parm(REGISTRY::PARM_GET)); }

		function exists($parm) 				{ return array_key_exists($parm,$this->get_parm(REGISTRY::PARM_REQUEST)); }

		function is_post() 						{ return !is_empty($this->get_parm(REGISTRY::PARM_POST)); }

		/**
		 * Checks whether has get parm
		 * @param string $parm
		 * @return boolean
		 */
		function has_get_parm($parm) 			{ return array_key_exists($parm,$this->get_parm(REGISTRY::PARM_GET)); }

		/**
		 * Checks whether has request parm
		 * @param string $parm
		 * @return boolean
		 */
		function has_request_parm($parm) 		{ return array_key_exists($parm,$this->get_parm(REGISTRY::PARM_REQUEST)); }

		/**
		 * Checks whether has post parms
		 * @return boolean
		 */
		function has_post_parms() 			{ return !is_empty($this->get_parm(REGISTRY::PARM_POST)); }


        /**
         * Checks whether has get parms
         * @return boolean
         */
        function has_get_parms() 			{ return !is_empty($this->get_parm(REGISTRY::PARM_GET)); }

        /**
         * Checks whether the request has parms
         * @return boolean
         */
        function has_request_parms() 			{ return !is_empty($this->get_parm(REGISTRY::PARM_REQUEST)); }

        /**
         * Retuns numeric request parm
         * @return mixed
         */
		function get_numeric_request_parm() {
			$function_args = func_get_args();
			$parm = $this->get_function_parm(REGISTRY::PARM_REQUEST,$function_args);
			return is_numeric($parm) ? $parm : null;
		}

        /**
         * Returns numeric post parm
         * @return mixed
         */
		function get_numeric_post_parm() {
			$function_args = func_get_args();
			$parm = $this->get_function_parm(REGISTRY::PARM_POST,$function_args);
			return is_numeric($parm) ? $parm : null;
		}

        /**
         * Returns numeric get parm
         * @return mixed
         */
		function get_numeric_get_parm() {
			$function_args = func_get_args();
			$parm = $this->get_function_parm(REGISTRY::PARM_GET,$function_args);
			return is_numeric($parm) ? $parm : null;
		}

        /**
         * Returns request parm
         * @return mixed
         */
		function get_request_parm() {
			return $this->get_function_parm(REGISTRY::PARM_REQUEST,func_get_args());
		}

		function request() {
			return $this->get_function_parm(REGISTRY::PARM_REQUEST,func_get_args());
		}

        /**
         * Reutnr post parm
         * @return mixed
         */
		function get_post_parm() {
			return $this->get_function_parm(REGISTRY::PARM_POST,func_get_args());
		}

		function post() {
			return $this->get_function_parm(REGISTRY::PARM_POST,func_get_args());
		}

        /**
         * Returns get parm
         * @return mixed
         */
		function get() {
			return $this->get_function_parm(REGISTRY::PARM_GET,func_get_args());
		}

		function get_get_parm() {
			return $this->get_function_parm(REGISTRY::PARM_GET,func_get_args());
		}

        /**
         * Return request parm array
         * @return array
         */
		function get_request_parm_array() {
			$function_args = func_get_args();
			$parm = $this->get_function_parm(REGISTRY::PARM_REQUEST,$function_args);
			return is_array($parm) ? $parm : array();
		}

        /**
         * Returns function parm
         * @param string $parm
         * @param array $function_args
         * @return mixed
         */
		function get_function_parm($parm,$args) {

			if(count($args)==1)
				$args = (array)value($args,0);

			$options = array();
			foreach($args as $arg) {
				if($arg===self::OPTION_RAW) {
					array_pop($args);
					if($parm==self::PARM_REQUEST)
						$parm = self::PARM_RAW_REQUEST;

					elseif($parm==self::PARM_POST)
						$parm = self::PARM_RAW_POST;

					elseif($parm==self::PARM_GET)
						$parm = self::PARM_RAW_GET;

				} elseif($arg===self::OPTION_STRING) {
					array_pop($args);
					if($parm==self::PARM_REQUEST)
						$parm = self::PARM_STRING_REQUEST;

					elseif($parm==self::PARM_POST)
						$parm = self::PARM_STRING_POST;

					elseif($parm==self::PARM_GET)
						$parm = self::PARM_STRING_GET;
				}
			}

			$parm_keys = array_merge(array($parm),$args);
			return $this->get_parm($parm_keys);
		}
	}
