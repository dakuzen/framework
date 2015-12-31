<?php
        /**
        * Class GOOGLE_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */

	class GOOGLE_UTIL {
	
		/**
		 * Creates a new client
		 * Todo: Ray
		 */
		 
		
		static function create_client() {
			$apiClient = new apiClient();					
			
			return $apiClient;
		}
	}
        
	require_once DIR_FRAMEWORK."packages/Google/src/apiClient.php";
	require_once DIR_FRAMEWORK."packages/Google/src/contrib/apiBuzzService.php";