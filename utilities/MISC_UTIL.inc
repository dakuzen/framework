<?php
        /**
        * Class MISC_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */

	class MISC_UTIL {

		/**
		 * Returns a array list from the named values
		 *
		 * @param array $objects array
		 * @param string $function_name_value string value
		 * @param string $function_name_key string key
		 * @return list
	 	 */

		static function get_list_from_object_function($objects,$function_name_value,$function_name_key=null) {
			$list = array();
			foreach($objects as $object) {
				$value = $object->$function_name_value();
				if($function_name_key) {
					$key = $object->$function_name_key();
					$list[$key] = $value;
				} else {
					$list[] = $value;
				}
			}
			return $list;
		}

		/**
		 * Preforms a check to see if the string is empty
		 *
		 * <code>
		 * echo MISC_UTIL::is_empty($myname);
		 * </code>
		 *
		 * @param string $value string checked
		 * @return boolean
	 	 */

		static function is_empty($value) {
			if(is_string($value))
				$value = trim($value);
			return empty($value);
		}

		/**
		 * Returns a random string
		 *
		 * <code>
		 * echo MISC_UTIL::get_guid();
		 * </code>
		 * <pre>
		 * b115eaf5267a3fea00a58634e6510641
		 * </pre>
		 *
		 * @param int $length int length
                 * @param boolean $mixed_case
		 * @return string
	 	 */

		static function get_guid($length=null,$mixed_case=false) {
			return self::guid($length,$mixed_case);
		}

		static function guid($length=null,$mixed_case=false) {

			//generate a random id encrypt it and store it in $guid
			$guid = md5(uniqid(mt_rand(),true));

			if($length)
				$guid = substr($guid,0,$length);

			if($mixed_case) {

				$chars = str_split($guid);
				$guid = "";

				foreach($chars as $char)
					$guid .= rand(0,1) ? strtoupper($char) : $char;
			}

			//if guid is numeric or infinite regenerate
			if(is_numeric($guid) || @is_infinite($guid))
				$guid = self::guid($length,$mixed_case);

			return $guid;
		}

		/**
		 * Returns a random string
		 *
		 * <code>
		 * echo MISC_UTIL::get_random_uid();
		 * </code>
		 * <pre>
		 * b115eaf5267a3fea00a58634e6510641
		 * </pre>
		 *
		 * @return string
	 	 */

		static function get_random_uid() {
			return self::get_guid();
		}

		/**
		 * Merges to arrayss
		 *
		 * <code>
		 * $words = array("1"=>"cat","2"=>"dog","3"=>"mouse");
		 * $words2 = array("4"=>"horse","5"=>"rabbit","6"=>"eagle");
		 * echo MISC_UTIL::array_merge($words,$words2)
		 * </code>
		 * <pre>
		 * Array
		 * (
	         *	[1] => cat
    		 *	[2] => dog
    		 *	[3] => mouse
      		 *	[4] => horse
    		 *	[5] => rabbit
    		 *	[6] => eagle
		 * )
		 * </pre>
		 *
		 * @param array $array1 array first list
		 * @param array $array2 array second list
		 * @return array
	 	 */

		static function array_merge($array1,$array2) {
			foreach($array2 as $key=>$value)
				$array1[$key] = $value;
			return $array1;
		}

		public static function casts($old_objects,$new_classname) {

			foreach($old_objects as &$old_object)
				$old_object = self::cast($old_object,$new_classname);

			return $old_objects;
		}

		/**
		 * Type casts to a new object
		 *
		 * @param object $old_object object old
		 * @param object $new_classname object new
                 * @param boolean $constructor
		 * @return object
	 	 */

		public static function cast($old_object, $new_classname, $constructor=false) {
			if(class_exists($new_classname)) {

				$old_serialized_prefix  = "O:".strlen(get_class($old_object));
				$old_serialized_prefix .= ":\"".get_class($old_object)."\":";

				$old_serialized_object = serialize($old_object);
				$new_serialized_object = 'O:'.strlen($new_classname).':"'.$new_classname . '":';
				$new_serialized_object .= substr($old_serialized_object,strlen($old_serialized_prefix));

				$object = unserialize($new_serialized_object);
				if($constructor && method_exists($object,"__construct"))
					$object->__construct();

				return $object;
			}

			return false;
		}

		public static function type_cast($value,$options=["cast_float"=>true]) {

			if($value==="true")
				$value = true;
			elseif($value==="false")
				$value = false;
			elseif($value==="null")
				$value = null;
			elseif(is_numeric($value)) {

				if(preg_match("/^[0-9]+$/",$value)) {
					if(!preg_match("/^0\d+/",$value) && (int)$value!=PHP_INT_MAX)
						$value = (int)round($value,0);
				} elseif(value($options,"cast_float",true)) {
					$value = (float)$value;
				}

			} elseif(is_array($value)) {
				foreach($value as &$item)
					$item = self::type_cast($item,$options);
			}

			return $value;
		}

		/**
		 * Gives a class an alias name
		 *
		 * <code>
		 * MISC_UTIL::class_alias("MISC_UTIL","miscutil")
		 * miscutil::array_merge($array1,$array2)
		 * </code>
		 *
		 * @param string $original string original class name
		 * @param string $alias string alias name given to the class
	 	 */

		static function class_alias($original, $alias) {
			if (!function_exists('class_alias'))
				eval('class '.$alias.' extends '.$original.' {}');
			else
				class_alias($original, $alias);
		}

        /**
         * Is class
         * @param object $class
         * @param string $class_name
         * @return boolean
         */
		static function is_class($class,$class_name) {

			if(!is_object($class))
				return false;

			//this is wrong and needs to be redone, didnt have time, but it kinda works
			$class_name_tmp = preg_replace("/^(H|C|HANDLER_|COMPLEX_)MODEL_/","",get_class($class));
			$class_name 	= preg_replace("/^(H|C|HANDLER_|COMPLEX_)MODEL_/","",$class_name);

			return $class_name==$class_name_tmp;
		}

		static function get_subclasses($class) {

			$subclassses = array();
			$subclasss = get_parent_class($class);

			if($subclasss)
				$subclassses = array_merge(array($subclasss),self::get_subclasses($subclasss));

			return $subclassses;
		}

		public static function print_r($array,$return=true,$steps=0) {
			$s = "";

			$level = value(ob_get_status(),"level");

			if($level==0)
				return print_r($array,true);

			$type = "";

			if(is_array($array) || is_object($array)) {

				if(is_array($array))
					$type = "Array";

				elseif(is_object($array)) {

					$reflect = new ReflectionClass($array);

					$props = $reflect->getProperties();

					$object = $array;
					$array = array();
					foreach($props as $key=>$value) {

						$name = $value->name;
						$array[$name] = $value->class;
					}

					$type = "Object";
				}

				$s = $type."\n".str_repeat(" ",($steps) * 8)."(\n";

				foreach($array as $name=>$value) {

					$val = "";

					if(is_array($value))
						$val = self::print_r($value,$return,($steps + 1));

					elseif(is_object($value)) {
						$name = $value->name;

						$val = self::print_r($array->$name,$return,($steps + 1));

					} elseif(!is_resource($value))
					 	$val = $value."\n";

					$s .= str_repeat(" ",($steps * 7) + 4)."[".$name."] => ".$val;
				}

				$s .= str_repeat(" ",($steps) * 8).")\n";
			}

			if(!$return)
				echo $s;

			return $s;
		}


	}

	/**
	 * Prints output to the screen
	 *
	 * <code>
	 * p(array(1=>"hello"));
	 * </code>
	 * <pre>
	 * Array
	 * (
	 *   [1] => hello
	 * )
	 * </pre>
         *
	 * @return boolean
	 */

	function p() {

		$backtrace = debug_backtrace();

		if($debug=get_value($backtrace,0)) {

			$if_format_html = DEBUG_UTIL::is_format_html();

			if($if_format_html)
				echo '<div style="padding:2px;border: 0px dashed red;font-weight:bold">DEBUG: '.get_value($debug,"file").' @ line number '.get_value($debug,"line");
			else
				echo 'DEBUG: '.get_value($debug,"file").' @ line number '.get_value($debug,"line")."\n\n";

			foreach(func_get_args() as $value) {

				if($value===null)
					$value = "NULL";
				elseif($value===FALSE)
					$value = "FALSE";
				elseif($value===TRUE)
					$value = "TRUE";
				elseif(is_object($value) && is_a($value,"stdObject"))
					$value = (string)$value;

				if(is_array($value) || is_object($value)) {

					if($if_format_html)
						print "<pre style='margin:4px;margin-left:10px;font-weight:normal'>";

					print_r($value);

					if($if_format_html)
						print "</pre>";
				} else {

					if($if_format_html)
						print "<pre style='margin:4px;margin-left:10px;font-weight:normal'>";

					if(is_string($value) && $if_format_html)
						$value = htmlentities($value);

					print($value);

					if($if_format_html)
						print "</pre>";
					else
						print "\n\n";
				}
			}

			if($if_format_html)
				echo '</div>';
		}
	}

	function s($session_id) {

		if($session_id==SESSION_MANAGER::get_session_id()) {
			$args = func_get_args();
			array_shift($args);
			p($args);
		}
	}

	/**
         * H
         */
	function h() {
		$backtrace = debug_backtrace();

		if($debug=get_value($backtrace,0)) {

			$values = array();

			foreach(func_get_args() as $value) {

				if($value===null) $value = "NULL";
				elseif($value===FALSE) $value = "FALSE";
				elseif($value===TRUE)  $value = "TRUE";
				else
					$value = print_r($value,true);

				$values[] = $value;
			}

			$value = count($values)>1 ? print_r($values,true) : get_value($values,0);

			$value = 'DEBUG: '.get_value($debug,"file").' @ line number '.get_value($debug,"line")." ".$value;

			echo HTML_UTIL::get_hidden("debug",$value);
		}
	}

	/**
	 * Preforms a check to see if a string is empty
	 *
	 * <code>
	 * echo is_empty("mystring");
	 * </code>
	 * <pre>
	 * false
	 * </pre>
	 *
	 * @param string $str string checked
	 * @return boolean
	 */

	function is_empty($str) { return MISC_UTIL::is_empty($str); }

	/**
	 * Returns the value of a given index
	 *
	 * <code>
	 * Ex 1.
	 * $words = array("1"=>"cat","2"=>"dog","3"=>"mouse");
	 * echo get_value($words,"2");
	 * </code>
	 * <pre>
	 * dog
	 * </pre>
	 * <code>
	 * Ex 2.
	 * $animals = array(1=>array(3=>"dog"),2=>array(4=>"cat"));
	 * echo $animals,get_value($animals,array(2,4));
	 * </code>
	 * <pre>
	 * cat
	 * </pre>
	 *
	 * @param array $var array
	 * @param mixed $index or path of indexes
	 * @param mixed $default
	 * @return mixed
         * @todo review return type
	 */

	function get_value($var,$index,$default=null) {
		return value($var,$index,$default);
	}

	function value($var,$index,$default=null) {

		if($var===null)
			return $default;

		$is_array = is_array($index);

		if($is_array && count($index)==1) {
			$index = array_shift($index);
			$is_array = is_array($index);
		}

		if($is_array) {

			$first_index = @$index[0];

			if($first_index!==null) {

				$vars = null;
				if(is_array($var))
					$vars = @$var[$first_index];

				elseif(is_object($var))
					$vars = @$var->$first_index;

				if($vars!==null) {

					array_shift($index);

					if(count($index)==1)
						$index = $index[0];

					if(is_array($vars) || is_object($vars))
						return value($vars,$index,$default);

					return $default;
				}
			}

			return $default;

        } elseif(is_array($var)) {
            if(@array_key_exists($index,$var))
                return $var[$index];

        } elseif(is_object($var)) {

        	if(isset($var->$index))
        		return $var->$index;
        }

		return $default;
	}

	/**
	 * Returns the numeric value
	 *
	 * <code>
	 * Ex 1.
	 * $numbers = array("1"=>"42","2"=>"56","3"=>"89");
	 * echo get_numeric_value($numbers,"2");
	 * </code>
	 * <pre>
	 * 56
	 * </pre>
	 *
	 * @param array $var array
	 * @param mixed $index or path of indexes
	 * @param mixed $default
	 * @return boolean
         * @todo review return type - it seems wrong
	 */

	function get_numeric_value($var,$index,$default=null) {
		$value = get_value($var,$index,$default);
		return is_numeric($value) ? $value : null;
	}


