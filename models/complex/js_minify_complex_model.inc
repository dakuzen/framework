<?php
       /**
        * BASE_CMODEL_CSS_MINIFY class file
        *
        * @package Framework
        * @subpackage Cmodels
        */

	require_once BASE_MODEL_PATH::get_framework_packages_directory()."jshrink/jshrink.inc";

       /**
        * Class BASE_CMODEL_CSS_MINIFY
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_JS_MINIFY {
		
        public static function create() {
            return new BASE_CMODEL_JS_MINIFY();  
        }

        /**
         * Minify
         * @param string $css
         * @return string 
         */
		public function minify($js) {
			return JShrink\Minifier::minify($js);
		}		
	}