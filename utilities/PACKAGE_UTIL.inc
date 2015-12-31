<?php
        /**
        * Class PACKAGE_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */
	
	class PACKAGE_UTIL {
	
		/**
		 * Creates a browercap
		 *
		 * @param string $cache_dir
		 * 
		 * @return object
		 */
		static function create_browercap($cache_dir) {
					
			if(!is_dir($cache_dir)) {
				FILE_UTIL::mkdir($cache_dir);
				FILE_UTIL::chown($cache_dir,"apache","apache");
			}
				
			require_once(DIR_FRAMEWORK."packages/Browsercap/Browsercap.inc");
			
			$browser_cap = new Browscap($cache_dir);  
			$browser_cap->doAutoUpdate = true;
			
			return $browser_cap;
		}
	}
	
