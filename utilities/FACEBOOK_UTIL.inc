<?php
        /**
        * Class FACEBOOK_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */

	class FACEBOOK_UTIL {
                
                /**
                 * Creates a client
                 * 
                 * @param string $api_key
                 * @param string $secret_key
                 * @param boolean $debug
                 * 
                 * @return object 
                 */
		static function create_client($api_key, $secret_key,$debug=false) {
			return BASE_CMODEL_FACEBOOK_CLIENT::create($api_key, $secret_key,$debug);
		}
		
                /**
                 * Creates a rest client
                 * 
                 * @param string $api_key
                 * @param string $secret_key
                 * @param boolean $debug
                 * 
                 * @return object 
                 */
		static function create_rest_client($api_key, $secret_key,$debug=false) {
		
			require_once(DIR_FRAMEWORK."packages/Facebook/php/facebook-rest.php");	
			
			$facebook = new FacebookRest($api_key, $secret_key);
			
			$fbs_key =  'fbs_'.$api_key;
			
			$fbs = trim(get_value($_COOKIE,$fbs_key),'"');
			
			$fbs = ARRAY_UTIL::get_exploded_array($fbs,"&","=",true);
			$facebook->set_user(get_value($fbs,"uid"),get_value($fbs,"session_key"),get_value($fbs,"expires"));
			
			return $facebook;
		}
	}
