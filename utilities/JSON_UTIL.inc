<?php
    /**
    * Class JSON_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */

	class JSON_UTIL {

        /**
         * Encode json object
         * @param object $object
         * @return string
         */
		static function encode($object,$options=array("type_cast"=>true,"bigint_string"=>true,"exponential_string"=>true)) {

			$options = (array)$options;
			$bits = value($options,"bigint_string") ? JSON_BIGINT_AS_STRING : 0;

			//removed because JSON_NUMERIC_CHECK always created 5.5555555555556e+25, needed a way to override.

			if(value($options,"type_cast"))
				$object = MISC_UTIL::type_cast($object);

			return json_encode($object,$bits);
		}

        /**
         * Decode a json object
         * @param string $string
         * @return mix
         */
		static function decode($string) {

			if(is_empty($string))
				return $string;

			$type = gettype($string);

			if(in_array($type,array("array","object","resource")))
				return $string;

			$json =  json_decode($string,true);

			if(!$json)
				$json = json_decode(preg_replace('/[\x00-\x1F\x80-\xFF]/', '', $string), true);

			return $json;
		}

		/**
		 * Checks if its encoded
		 *
		 * @param string $string string being checked
		 * @return string
		 */
		static function is_encoded($string) {

			if(!is_string($string))
				return false;

			return preg_match('/(^{.+}$|^\"\"$|^\[.*\]$|^".+"$)/',$string);
		}

        /**
         * Stream the object
         * @param object $object
         */
		static function stream($object,$die=true) {
			HTTP_UTIL::stream_string(self::encode($object),"json.json");
			if($die) die;
		}

        /**
         * Encode jsonp
         * @param string $callback
         * @param object $object
         * @return string
         * @todo review description and parameters
         */
		static function encode_jsonp($callback,$object) {
			return $callback."(".self::encode($object).")";
		}

	}