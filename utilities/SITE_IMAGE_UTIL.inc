<?php
        /**
        * Class SITE_IMAGE_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */

	class SITE_IMAGE_UTIL {

		/**
                 * Creates thumb provider shrinktheweb
		 * @todo put some better description
                 * @param string $access_key
                 * @param string $secret_key
                 * 
                 * @return object
		 */
		function create_thumb_provider_shrinktheweb($access_key,$secret_key) 	{ return THUMB_PROVIDER::create_shrinktheweb($access_key,$secret_key); }
		
                /**
                 * Creates thumb provider snapcasa
		 * @todo put some better description
                 * @param string $user_code
                 * @param string $premium_key
                 * 
                 * @return object
		 */
                function create_thumb_provider_snapcasa($user_code,$premium_key="") 	{ return THUMB_PROVIDER::create_snapcasa($user_code,$premium_key); }
		
                /**
                 * Creates thumb provider bluga
		 * @todo put some better description
                 * @param string $api_key
                 * 
                 * @return object
		 */
                function create_thumb_provider_bluga($api_key) 				{ return THUMB_PROVIDER::create_bluga($api_key); }
		
                /**
                 * Creates thumb provider thumbalizr
		 * @todo put some better description
                 * @param string $api_key
                 * 
                 * @return object
		 */
                function create_thumb_provider_thumbalizr($api_key) 			{ return THUMB_PROVIDER::create_thumbalizr($api_key); }
		
                /**
                 * Creates thumb provider pageglimpse
		 * @todo put some better description
                 * @param string $api_key
                 * 
                 * @return object
		 */
                function create_thumb_provider_pageglimpse($api_key) 			{ return THUMB_PROVIDER::create_pageglimpse($api_key); }
	}
        
        require_once(DIR_FRAMEWORK."libraries/thumbnailer/THUMB_PROVIDER.inc");
	
	