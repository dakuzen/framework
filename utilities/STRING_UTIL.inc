<?php
        /**
        * Class STRING_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */

	class STRING_UTIL {

		const PAIR_DELIMITER		= "@@||@@";
		const FIELD_DELIMITER		= "@@--@@";

		/**
		 * Trims a string to the right of the character specified
		 *
		 * @param string $string string word trimmed
		 * @param string $search string character delimitor
		 * @return string
		 */
	 
		static function right_trim($string,$search) {
			if(($idx=strrpos($string,$search))==(strlen($string)-strlen($search)))
				return substr($string,0,$idx);
			return $string;
		}

		/**
		 * Returns a string with ".." attached
		 *
		 * <code>
		 * echo STRING_UTIL::shorten("reptile","t");
		 * </code>
		 * <pre>
		 * rept...
		 * </pre>
		 *
		 * @param string $string string word 
		 * @param int $limit string character delimitor
		 * @return string
		 */
		 
		static function shorten($string,$limit) {

			$limit -= 3;
			return strlen($string)>=$limit ? trim(substr($string, 0, $limit))."..." : $string;
		}
		
        /**
         * Returns the dot dot dot string
         * @param string $string
         * @param integer $limit
         * @return string 
         */
		static function get_dot_dot_dot_string($string,$limit) {
			return self::shorten($string,$limit);
		}		

		/**
		 * Implodes PAIR_DELIMITER constant using an array and returns its string
		 *
		 * @param array $array array 
		 * @return string
		 */
		 
		static function get_delimited_string($array) {
			$data = array();
			foreach($array as $key=>$value)
				$data[] = $key.STRING_UTIL::FIELD_DELIMITER.$value;
			return implode(STRING_UTIL::PAIR_DELIMITER,$data);
		}

		/**
		 * Creates the delimited array using a string
		 *
		 * @param string $string string
		 * @return array
		 */
		 
		static function get_delimited_array($string) {
			if(!$string)
				return array();

			$data = array();
			$pairs = explode(STRING_UTIL::PAIR_DELIMITER,$string);
			foreach($pairs as $pair) {
				$field = explode(STRING_UTIL::FIELD_DELIMITER,$pair);
				$data[get_value($field,0)] = get_value($field,1);
			}
			return $data;
		}

		/**
		 * Returns the propercasing for a string by making the first letter capital
		 *
		 * <code>
		 * echo STRING_UTIL::get_propercase("computer");
		 * </code>
		 * <pre>
		 * Computer
		 * </pre>
		 *
		 * @param string $string string
                 * @param boolean $all
		 * @return string
		 */
		 
		static function propercase($string,$all=false) {			
			return $all ? ucwords($string) : ucfirst($string);
		}

		static function get_propercase($string,$all=false) {			
			return self::propercase($string,$all);
		}

		/**
		 * Returns lower casing of a string
		 *
		 * <code>
		 * echo STRING_UTIL::get_lowercase("COMPUTER");
		 * </code>
		 * <pre>
		 * computer
		 * </pre>
		 *
		 * @param string $string string
		 * @return string
		 */
		 
		static function get_lowercase($string) {
			if(extension_loaded("mbstring"))
				return @mb_strtolower($string,"UTF-8");
			else
				trigger_error("Failed to load the mb string extension", E_USER_ERROR);
			return $string;
		}

		/**
		 * Returns upper casing of a string
		 *
		 * <code>
		 * echo STRING_UTIL::get_uppercase("COMPUTER");
		 * </code>
		 * <pre>
		 * COMPUTER
		 * </pre>
		 *
		 * @param string $string string
		 * @return string
		 */
		 
		static function get_uppercase($string) {
			if(extension_loaded("mbstring"))
				return @mb_strtoupper($string,"UTF-8");
			else
				trigger_error("Failed to load the mb string extension", E_USER_ERROR);
			return $string;
		}
		
		/**
		 * Converts utf-8 standard to iso-8859
		 *
		 * <code>
		 * echo STRING_UTIL::utf8_to_iso8859("computer");
		 * </code>
		 *
		 * @param string $string string converted
		 * @return string
		 */
		 
		static function utf8_to_iso8859($string) {
			if(extension_loaded("mbstring"))
				return @mb_convert_encoding($string, "ISO-8859-1", mb_detect_encoding($string, "UTF-8, ISO-8859-1, ISO-8859-15", true));
			else
				trigger_error("Failed to load the mb string extension", E_USER_ERROR);
			return $string;	
		}

		/**
		 * Gets utf-8 regardless the standard
		 *
		 * <code>
		 * echo STRING_UTIL::get_utf8("computer");
		 * </code>
		 *
		 * @param string $string string converted
		 * @return string
		 */
		 
		static function get_utf8($string) {
			if(extension_loaded("mbstring"))
				return @mb_convert_encoding($string, "UTF-8", mb_detect_encoding($string, "UTF-8, ISO-8859-1, ISO-8859-15", true));
			else
				trigger_error("Failed to load the mb string extension", E_USER_ERROR);
			return $string;	
		}

		/**
		 * Returns the number suffix
		 *
		 * <code>
		 * echo STRING_UTIL::get_number_suffix("2");
		 * </code>
		 * <pre>
		 * nd
		 * </pre>
		 * @param string $number string value
		 * @return string
		 */
		 
		static function get_number_suffix($number) {
			$number = intval($number);
			
			switch ($number) {
				case 0:
					return "";
				case 1:
				case 21:
				case 31:
					return 'st';
				case 2:
				case 22:
					return 'nd';
				case 3:
				case 23:
					return 'rd';
			}
			
			return 'th';
		}

		/**
		 * Returns the number/character representation
		 *
		 * <code>
		 * echo STRING_UTIL::print_characters("computer");
		 * </code>
		 * <pre>
		 * c - 99 o - 111 m - 109 p - 112 u - 117 t - 116 e - 101 r - 114 
		 * </pre>
		 *
		 * @param string $string string value
		 * @return string
		 */
		 
		static function print_characters($string) {
			
			$length = strlen($string);
			for($i=0;$i<$length;$i++) {
				$char = substr($string,$i,1);				
				echo $char." - ".ord($char)."\n";			
			}		
		}
		
		/**
		 * Removes the duplicates
		 *
		 *
		 * @param string $string string value
		 * @param string $duplicate string value
		 * @return string
		 */
		 
		static function remove_duplicates($string,$duplicate) {			
			return preg_replace("/".self::get_safe_regex_string($duplicate)."+/",$duplicate,$string);
		}
		

                /**
                 * Returns the safe regex string
                 * 
                 * @param string $str
                 */
		function get_safe_regex_string($str) {
			$patterns = array(	'/\//', '/\^/', '/\./', '/\$/', '/\|/',
		 				'/\(/', '/\)/', '/\[/', '/\]/', '/\*/', '/\+/',
						'/\?/', '/\{/', '/\}/', '/\,/');
			$replace = array(	'\/', '\^', '\.', '\$', '\|', '\(', '\)',
						'\[', '\]', '\*', '\+', '\?', '\{', '\}', '\,');

			return preg_replace($patterns,$replace, $str);
		}
		
		/**
		 * Cleans up a url replacing irregular slashes
		 *
		 * @param string $url string url 
		 * @return string
		 */
		 
		static function get_sanitized_url($url) {
			return preg_replace("/\/{2,}/","/",$url);
		}
		
		/**
		 * Prase out the decimal number and return it
		 *
		 * @param string $string 
		 * @return float
		 */
		 
		static function get_decimal($string) {
			$string = str_replace(",","",$string);
			preg_match("/^[^\d]*(\d*)?(\.\d+)?/",$string,$matches);
			return (float)get_value($matches,1).get_value($matches,2);
		}
		
		/**
		 * Prase out the number and return it
		 *
		 * @param string $string 
		 * @return int
		 */
		 
		static function get_number($string) {
			$string = str_replace(",","",$string);
			preg_match("/^[^\d]*(\d*)?/",$string,$matches);
			return (int)get_value($matches,1).get_value($matches,2);
		}	
		
                /**
                 * Parse a tag
                 * @param string $content
                 * @param string $tag
                 * @return mixed 
                 */
		static function parse_tag($content,$tag="") {
			$close = $inner = $open = "";
			
			if(!$tag) {
				preg_match("/<\s*([^\s>]+)/",$content,$match);				
				$tag = get_value($match,1);
			}
			
			if(preg_match("/(<".$tag."[^>]*>)(.*)(<\/".$tag.">)/ism",$content,$matches)) {
				$open	= get_value($matches,1);
				$inner 	= get_value($matches,2);
				$close 	= get_value($matches,3);
				
				return new PARSE_TAG($open,$inner,$close);
			}
			
			return false;
		}
		
                /**
                 * Parse tags
                 * @param string $content
                 * @param string $search_tag
                 * @return object 
                 */
		static function parse_tags($content,$search_tag) {
			$tags = array();
			
			if(preg_match_all("/(<".$search_tag."[^>]*>)(.*?)(<\/".$search_tag.">)/ism",$content,$matches)) {
				
				for($i=0;$i<count(get_value($matches,0,array()));$i++) {
					
					$open 	= get_value($matches,array(1,$i));
					$inner	= get_value($matches,array(2,$i));
					$close	= get_value($matches,array(3,$i));
					
					$tags[] = new PARSE_TAG($open,$inner,$close);
				}
			}
			
			return $tags;
		}		
		
		/**
                 * Returns tag attributes
                 * @param string $tag
                 * @return string 
                 */
		function get_tag_attributes($tag) {
			
			$attributes = array();
			
			if(preg_match("/<[^\s]+[\s]+(.*?)>/is",$tag,$matches)) {
				
				$tag = get_value($matches,1);
				
				if(preg_match_all("/([^\s]+)\s*=\s*[\"'](.*?)[\"']\s*/is",$tag,$matches)) {

					for($j=0;$j<count(get_value($matches,0,array()));$j++) {

						$key 	= get_value($matches,array(1,$j));
						$value 	= get_value($matches,array(2,$j));

						$attributes[$key] = $value;
					}
				}
			}
			
			return $attributes;
		}
		
		const MASK_AREA_FRONT 	= "F";
		const MASK_AREA_BACK	= "B";
		
                /**
                 * Returns masked
                 * @param string $string
                 * @param integer $length
                 * @param string $mask_area
                 * @param string $mask_char
                 * @return string 
                 */
		static function get_masked($string,$length,$mask_area=self::MASK_AREA_BACK,$mask_char="*") {
			
			$len = strlen($string);
			
			$masked = "";
			
			if($mask_area!=self::MASK_AREA_BACK)
				$masked = substr($string,0,$length).str_repeat($mask_char,$len - $length);
			else
				$masked = str_repeat($mask_char,$len - $length).substr($string,$len-$length);
				
			return $masked;
		}		
	}
	
        /**
        * Class PARSE_TAG
        *
        * @package Framework
        * @subpackage Utilities
        */        
        
	class PARSE_TAG {
		
                /**
                * Attributes
                * @var array $attributes 
                */
		protected $_attributes 	= array();
                
                /**
                * Inner
                * @var string $inner 
                */
		protected $_inner	= "";
                
                /**
                * open
                * @var string $open 
                */
		protected $_open	= "";
                
                /**
                * close
                * @var string $close 
                */
		protected $_close	= "";
                
                /**
                * type
                * @var string $type 
                */
		protected $_type	= "";
                
                /**
                * namespace
                * @var string $namespace
                */
		protected $_namespace	= "";
		
                /**
                 * Construct
                 * 
                 * @param string $open the open tag
                 * @param string $inner the inner tag
                 * @param string $close the close tag
                 */
		function __construct($open,$inner,$close) {
			$this->_open		= $open;
			$this->_inner 		= $inner;
			$this->_close		= $close;
			$this->_attributes	= STRING_UTIL::get_tag_attributes($open);
			
			preg_match("/<([^:]+:)?([^\s]+)/",$open,$matches);
			
			$this->_namespace = get_value($matches,1);
			$this->_type = get_value($matches,2);
		}
		
                /**
                 * Returns open
                 * @return string 
                 */
		function get_open() 			{ return $this->_open; }
                
                /**
                 * Returns type
                 * @return string 
                 */
		function get_type()			{ return $this->_type; }
                
                /**
                 * Returns inner
                 * @return string 
                 */
		function get_inner() 			{ return $this->_inner; }
                
                /**
                 * Returns close
                 * @return string 
                 */
		function get_close() 			{ return $this->_close; }
                
                /**
                 * Returns type
                 * @return string 
                 */
		function get_attributes() 		{ return $this->_attributes; }	
                
                /**
                 * Checks whether attribute exists
                 * @param string $attribute
                 * @return boolean
                 */
		function attribute_exists($attribute) 	{ return array_key_exists($attribute,$this->_attributes); }
                
                /**
                 * Returns attribute
                 * @param string $attribute
                 * @return string 
                 */
		function get_attribute($attribute) 	{ return get_value($this->_attributes,$attribute); }	
                
                /**
                 * Returns tag
                 * @return string 
                 */
		function get_tag() 			{ return HTML_UTIL::get_tag($this->_type,$this->_attributes,$this->get_inner()); }
		
                /**
                 * Removes attribute 
                 * @param string $attribute
                 */
		function remove_attribute($attribute) 	{ unset($this->_attributes[$attribute]); }		
	}