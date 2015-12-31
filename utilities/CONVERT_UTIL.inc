<?php
        /**
        * Class CONVERT_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */
	
	class CONVERT_UTIL {
	
		/**
		 * Get the ip address as integer
		 * @param string $ip
                 * 
                 * @return integer
		 */
		static function get_integer_ip_address($ip) { return self::get_binary_ip_address($ip); }
		
                /**
		 * Get the ip address as binary
		 * @param string $ip
                 * 
                 * @return integer
		 */
		static function get_binary_ip_address($ip) {

			$parts = explode(".",$ip);

			$mutiples = array(16777216,65536,256,1);

			$total = 0;
			foreach($parts as $index=>$part)
				$total += $part * get_value($mutiples,$index,0);
			
			return $total;
		}
		
                /**
		 * Get the ip address
		 * @param string $decimal_ip
                 * 
                 * @return string
		 */
		static function get_ip_address($decimal_ip) {
			return long2ip($decimal_ip);
		}
		
	}