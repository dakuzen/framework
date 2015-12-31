<?php
        /**
        * Class LANG_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */
	 
	class LANG_UTIL {
		
		/**
		 * Gets the plural of the given string 
		 *
		 * @param string $string string word to be pluralized
                 * @param integer $count the counter
                 * @param boolean $is_noun whether is noun
                 * 
		 * @return string
		 */
		function get_plural_string($string,$count=null,$is_noun=true) {
			return self::get_plural($string,$count,$is_noun);
		}

		function get_propercase($string) {
			return strtoupper(substr($string,0,1)).substr($string,1);
		}		
		
        /**
		 * Gets the posessive 
		 *
		 * @param string $string string word
                 * 
		 * @return string
		 */
		function get_posessive($string) {			
			return strtolower(substr($string,-1,1))=="s" ? $string."'" : $string."'s";			
		}		
		 
                /**
		 * Gets the plural of the given string 
		 *
		 * @param string $string string word to be pluralized
                 * @param integer $count the counter
                 * @param boolean $is_noun whether is noun
                 * 
		 * @return string
		 */

        function get_plural($string,$count=null,$is_noun=true) {
			return self::plural($string,$count,$is_noun);
		}

		function plural($string,$count=null,$is_noun=true) {
				
			$count = is_array($count) ? count($count) : $count;
			if($count>1 || !$count) {

				if(preg_match("/(.*)ss$/i",$string,$matches))
					$string = get_value($matches,1)."sses";

				elseif(preg_match("/(.*)ey$/i",$string,$matches))
					$string .= "s";				
					
				elseif(preg_match("/(.*)y$/i",$string,$matches))
					$string = get_value($matches,1).($is_noun ? "ies" : "ys");

				elseif(preg_match("/(.*)s$/i",$string,$matches))
					$string = get_value($matches,1)."es";
				
				elseif(preg_match("/x$/i",$string))
					$string = $string."es";				
				else
					$string = $string."s";
			}
			
			return $string;
		}	
	}