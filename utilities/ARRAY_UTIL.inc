<?php
        /**
        * Class ARRAY_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */

	class ARRAY_UTIL {

		/**
		 * Returns an array that is padded using the $pad_str on the left and right
		 *
		 * <code>
		 *   ARRAY_UTIL::get_padded(array("dog","cat","mouse"),"bird");
		 * </code>
		 * <pre>
		 *	Array
		 *	(
		 *	    [0] => birddogbird
		 *	    [1] => birdcatbird
		 *	    [2] => birdmousebird
		 * 	)
		 * </pre>
		 *
		 * @param array $array to be padded
		 * @param string $pad_str string padded to each element
		 * @return array
		 */

		static function get_padded($array,$pad_str) {
			foreach($array as &$element)
				$element = $pad_str.$element.$pad_str;

			return $array;

		}

        /**
         * Return the padded array elements
         * @param array $array
         * @param string $pad_str
         * @return array
         */
		static function get_padded_array_elements($array,$pad_str) {
			return self::get_padded($array,$pad_str);

		}

		/**
		 * Returns an array that is padded using the $pad_str on the left and right
		 * <code>
		 * 	ARRAY_UTIL::get_exploded($string,$delimiter)
		 * </code>
		 * <pre>
		 *	Array
		 *	(
		 *	    [0] => r
		 *	    [1] => e
		 *	    [2] => p
		 *	)
		 *
		 * </pre>
		 *
		 * @param string $string string to be exploded
		 * @param string $delimiter string delimiter used to explode the $string
		 * @param string $key_value_delim string used to delimit the keys and the values
		 * @param boolean $retain_keys boolean used to retain key values
		 * @param string $valid_regex string that validates it being a regular expression
		 * @return array
		 */
		static function get_exploded($string,$delimiter,$key_value_delim="",$retain_keys=false,$valid_regex=null) {
			return self::explode($string,$delimiter,$key_value_delim,$retain_keys,$valid_regex);
		}

		static function explode($string,$delimiter,$key_value_delim="",$retain_keys=false,$valid_regex=null) {

			$string = trim($string);

			if(strlen($string)==0)
				return array();

			$array = array();
			$values = explode($delimiter,$string);

			foreach($values as $value) {

				$is_valid = true;

				$key = null;
				if($key_value_delim) {
					$values = explode($key_value_delim,$value);
					$key 	= value($values,0);
					$value 	= value($values,1);
				}

				if($valid_regex)
					$is_valid = preg_match($valid_regex,$value,$matches);

				if($is_valid) {
					if($retain_keys)
						$array[$key] = trim($value);
					else
						$array[] = trim($value);
				}
			}

			return $array;
		}

        /**
         * Return the exploded array
         * @param string $string
         * @param string $delimiter
         * @param string $key_value_delim
         * @param boolean $retain_keys
         * @param string $valid_regex
         * @return array
         */
		static function get_exploded_array($string,$delimiter,$key_value_delim="",$retain_keys=false,$valid_regex=null) {
			return self::get_exploded($string,$delimiter,$key_value_delim,$retain_keys,$valid_regex);
		}


		/**
		 * Passes an array and returns a key value string
		 * <code>
		 * 	ARRAY_UTIL::get_imploded(array("dog","cat","pig"))
		 * </code>
		 * <pre>
		 *	0="dog" 1="cat" 2="pig"
		 * </pre>
		 *
		 * @param array $array array to be imploded
		 * @param string $delim string that is placed between values
		 * @param string $quote string that is placed around each value
		 * @param string $key_value_delim string that is placed between key and value
		 * @return string
		 */
		static function get_imploded($array,$delim=" ",$quote='"',$key_value_delim="=") {
			return self::implode($array,$delim,$quote,$key_value_delim) ;
		}

		static function implode($array,$delim=" ",$quote='"',$key_value_delim="=") {
			$temp_array = array();
			foreach($array as $key=>$value)
				$temp_array[] = $key.$key_value_delim.$quote.$value.$quote;
			return implode($delim,$temp_array);
		}

        /**
         * Return the imploded string
         * @param array $array
         * @param string $delim
         * @param string $quote
         * @param string $key_value_delim
         * @return string
         */
		static function get_imploded_string($array,$delim=" ",$quote='"',$key_value_delim="=") {
			return self::get_imploded($array,$delim,$quote,$key_value_delim);
		}

		/**
		 * Passes an array and removes any empty values
		 * <code>
		 * 	ARRAY_UTIL::remove_empty_attributes(array("dog","cat","mouse",""))
		 * </code>
		 * <pre>
		 *	Array
		 *	(
		 *	    [0] => dog
		 *	    [1] => cat
		 *	    [2] => pig
		 *	)
 		 * </pre>
		 *
		 * @param array $attributes array to be passed
		 * @return string
		 */
		static function remove_empty_attributes(&$attributes) {
			foreach($attributes as $key=>$value) {
				if($value===null || @strlen(trim($value))==0)
					unset($attributes[$key]);
			}
			return $attributes;
		}

		/**
		 * Returns a numbered list represented by an array
		 * <code>
		 *	ARRAY_UTIL::get_numbered_list(5,10);
		 * </code>
		 * <pre>
		 *	 Array
		 *	 (
		 *	     [2] => 2
		 *	     [3] => 3
		 *	     [4] => 4
		 *	     [5] => 5
		 *	 )
 		 * </pre>
		 *
		 * @param int $from integer of the starting point of the list
		 * @param int $to integer of the ending point of the list
                 * @param string $value_format
                 * @param string $key_format
		 * @return array
		 */
		static function get_numbered_list($from,$to,$value_format="",$key_format="") {
			$list = array();
			for($i=$from;$i<=$to;$i++) {
				$value = $key = $i;

				if($value_format)
					$value = sprintf($value_format,$value);

				if($key_format)
					$key = sprintf($key_format,$value);

				$list[$key] = $value;
			}
			return $list;
		}

	    /**
	     * Return the numeric ranges
	     * @param array $numbers
	     * @return string
	     */
		static function get_numeric_ranges($numbers) {

			$numbers = array_unique(array_values($numbers));
			sort($numbers);

			$ranges = array();

			for($i=0;$i<count($numbers);$i++) {
				$current_number = get_value($numbers,$i);

				$next_number = $joiner = "";

				while($tmp_next_number=get_value($numbers,$i + 1)) {

					if($tmp_next_number == get_value($numbers,$i) + 1) {
						$joiner 	= "-";
						$next_number 	= $tmp_next_number;

					} else
						break;
					$i++;
				}

				$ranges[] = $current_number.$joiner.$next_number;
			}
			return $ranges;
		}

		/**
		 * Returns an array with grouped objects based on the function name given
		 *
		 * <code>
		 *	class ABC {
		 *
		 *		public $_x = "";
		 *
		 *		function __construct($x) {
		 *			$this->_x = $x;
		 *		}
		 *
		 *		function get_x() { return $this->_x; }
		 *	}
		 *
		 * 	ARRAY_UTIL::group(array(new ABC("1"),new ABC("2"),new ABC("2")),"get_x")
		 * </code>
		 * <pre>
		 *	 Array
		 *	 (
		 *	     [1] => Array
		 *		 (
		 *		     [0] => ABC Object
		 *			 (
		 *			     [_x] => 1
		 *			 )
		 *		 )
 		 *
		 *	     [2] => Array
		 *		 (
		 *		     [0] => ABC Object
		 *			 (
		 *			     [_x] => 2
		 *			 )
 		 *
		 * 		     [1] => ABC Object
		 *			 (
		 *			     [_x] => 2
		 *			 )
		 *		 )
		 *	 )
		 * </pre>
		 *
		 * @param array $objects array of objects to be grouped
		 * @param string $groupings string to be grouped by
		 * @return array
		 */

		const GROUP_TYPE_OBJECT = "O";
		const GROUP_TYPE_ARRAY 	= "A";

		static function group($objects,$groupings,$type=self::GROUP_TYPE_OBJECT) {

			if(!is_array($objects))
				return array();

			if(!is_array($groupings))
				$groupings = array($groupings,"");

			$group = $grouped_objects = array();

			if($type==self::GROUP_TYPE_OBJECT) {

				foreach($groupings as &$group)
					$group = $group ? "[\$object->".$group."()]" : "[]";

				foreach($objects as $object)
					eval("\$grouped_objects".implode("",$groupings)." = \$object;");

			} elseif($type==self::GROUP_TYPE_ARRAY) {

				foreach($groupings as &$group)
					$group = $group ? "[get_value(\$object,'".$group."')]" : "[]";

				foreach($objects as $object)
					eval("\$grouped_objects".implode("",$groupings)." = \$object;");
			}

			return $grouped_objects;
		}

		static function get_grouped_objects($objects,$groupings,$type=self::GROUP_TYPE_OBJECT) { return self::group($objects,$groupings,$type); }

		static function get_grouped($objects,$groupings) { return self::get_grouped_objects($objects,$groupings); }

	    /**
	     * Return the indexed objects
	     * @param objects $objects
	     * @param function $index_function
	     * @return object
	     */

		static function get_indexed_objects($objects,$name) {

			$list = [];
			foreach($objects as $object) {

				$index = null;
				if(is_array($object) && isset($object[$name])) {
					$index = $object[$name];
				} else {
					$index = $object->$name();
				}

				$list[$index] = $object;
			}

			return $list;
		}

		static function index($objects,$index_function) { return self::get_indexed_objects($objects,$index_function); }

		static function get_list($objects,$value,$key=null,$multiple_values=false) {

			$object = get_value(array_values($objects),0);

			$list =  array();
			if(is_array($object))
				$list = self::get_list_from_array($objects,$value,$key,$multiple_values);
			else
				$list = self::get_list_from_objects($objects,$value,$key);
			return $list;
		}

		static function listing($objects,$value,$key=null,$multiple_values=false) { return self::get_list($objects,$value,$key,$multiple_values); }

		/**
		 * Returns an array of values based on the function name
		 * <code>
		 *	class ABC {
		 *		public $_x = "";
		 *
		 *		function __construct($x) {
		 *			$this->_x = $x;
		 *		}
		 *
		 *		function get_x() { return $this->_x; }
		 *	}
		 *
		 *	ARRAY_UTIL::get_list_from_objects(array(new ABC("1"),new ABC("2"),new ABC("2")),"get_x")
		 * </code>
		 * <pre>
		 *	 Array
		 *	 (
		 *	     [0] => 1
		 *	     [1] => 2
		 *	     [2] => 2
		 *	 )
 		 * </pre>
		 *
		 * @param array $objects array of objects to get values from
		 * @param string $function_name string to be grouped by
		 * @param string $function_name_key string to set the key value of each list item
		 * @return array
		 */
		static function get_list_from_objects($objects,$function_name,$function_name_key=null) {

			if(!$objects)
				return array();

			$function_name 	= is_array($function_name) ? $function_name : array($function_name);
			$value_args 	= get_value($function_name,1)===null ? array() : array(get_value($function_name,1));

			if($function_name)
				$key_args = get_value($function_name,1)===null ? array() : array(get_value($function_name,1));

			$list = array();
			foreach($objects as $object) {

				$value 		= call_user_func_array(array($object,get_value($function_name,0)),$value_args);

				if($function_name_key) {
					$key = call_user_func_array(array($object, $function_name_key),$key_args);
					$list[$key] = $value;
				} else
					$list[] = $value;
			}
			return $list;
		}

		/**
		 * Returns an array of values based on the key name
		 *
		 * @param array $array array of values to get values from
		 * @param string $value_key string to be grouped by
		 * @param string $value_key_key string to set the key value of each list item
                 * @param boolean $multiple_values
		 * @return array
		 */

		static function get_list_from_array($array,$value_key,$value_key_key=null,$multiple_values=false) {
			$list = array();
			if($multiple_values && !is_array($value_key))
				$value_key = array($value_key);

			foreach($array as $item) {

				if($multiple_values) {
					$values = array();
					foreach($value_key as $key)
						$values[$key] = get_value($item,$key);
				} else
					$values = get_value($item,$value_key);

				if($value_key_key!==null)
					$list[get_value($item,$value_key_key)] = $values;
				else
					$list[] = $values;
			}
			return $list;
		}

		static function get_list_names($array,$selected) {
			return self::names($array,$selected);
		}

		static function names($array,$selected) {

			$list = array();
			if(is_array($selected))
				foreach($selected as $index)
					if($value=get_value($array,$index))
						$list[$index] = $value;

			return $list;
		}

		/**
         * Return the padded array elements
         * @param array $array
         * @param string $pad_str
         * @return array
         */
		static function in($needle,$hastack) {
			$needle = is_array($needle) ? $needle : array($needle);

			foreach($hastack as $item)
				if(in_array($item,$needle))
					return true;

			return false;
		}

		static function filter_keys($array,$keys) {

			$list = array();
			foreach($keys as $key)
				if($value=value($array,$key))
					$list[$key] = $value;

			return $list;
		}

		static function array_slice_assoc($array,$keys) {
			return array_intersect_key($array,array_flip($keys));
		}

	}