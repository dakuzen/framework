<?php
    /**
    * Class TWITTER_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */

	require_once(DIR_FRAMEWORK."packages/twitter/twitter.php");

	class TWITTER_UTIL {
	
		/**
		 * Creates a new twitter client
		 *
		 * @param string $consumer_key string consumer key
		 * @param string $secret_key string secret key
                 * 
		 * @return object
		 */
		static function create_client($consumer_key, $secret_key) {
			$twitter = new Twitter($consumer_key, $secret_key);  
			return $twitter;
		}
	}
        
        
	
