<?php
        /**
        * Class XSS_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */
	 
	class XSS_UTIL {
	
		/**
		 * Todo: Ray
                 * 
                 * @param array $array the array 
		 */

		static function get_encoded_xss($array) {
			foreach($array as $key=>$value) {
				$value = is_array($value) ? self::get_encoded_xss($value) : self::encode($value);
				$array[$key] = $value;
			}
			return $array;
		}
		
		/**
		 * Todo: Ray
                 * 
                 * @param string $value the value 
		 */
		 
		static function get_encoded_xss_string($value) {
			return self::encode($value);
		}
                
                /**
		 * Todo: Ray
                 * 
                 * @param string $value the value 
		 */
		 
		static function encode($value) {
			$value = htmlspecialchars($value);
			if(ini_get("magic_quotes_gpc"))
				$value = stripslashes($value);
			return $value;
		}
		
		/**
		 * Todo: Ray
                 * 
                 * @param array $array the array
		 */
		 
		static function get_decoded_xss($array) {
			foreach($array as $key=>$value) {
				$value = is_array($value) ? self::get_decoded_xss($value) : self::decode($value);
				$array[$key] = $value;
			}
			return $array;
		}
		
		/**
		 * Todo: Ray
                 * 
                 * @param string $str the string
                 * @param string $options the options
		 */
		 
		static function get_decoded_xss_string($str, $options=ENT_COMPAT) {
			return self::decode($str,$options);
		}
                
                /**
		 * Todo: Ray
                 * 
                 * @param string $str the string
                 * @param string $options the options
		 */
		
		static function decode($str, $options=ENT_COMPAT) {
			
			if(is_array($str))
				return self::get_decoded_xss($str);	
			
			$trans = get_html_translation_table(HTML_SPECIALCHARS, $options);
			return strtr($str, array_flip($trans));
		}		
		
		/**
		 * Todo: Ray
                 * 
                 * @param string $value the value
		 */
		 
		static function get_decoded_html($value) {
			return self::decode(htmlspecialchars($value));
		}		
		
	}