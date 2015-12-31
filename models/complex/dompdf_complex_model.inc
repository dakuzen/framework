<?php
     require_once(MODEL_PATH::get_framework_packages_directory()."dompdf/lib/class.pdf.php");

     /**
    * Class BASE_CMODEL_DOMPDF
    *
    * @package Framework
    * @subpackage Cmodels
    */	
	class BASE_CMODEL_DOMPDF {
                
                /**
                 * Constructor 
                 */
		public function __construct() {
			define("DOMPDF_DIR",MODEL_PATH::get_framework_packages_directory()."dompdf/");
			define("DOMPDF_LIB_DIR",DOMPDF_DIR."lib/");
			define("DOMPDF_INC_DIR",DOMPDF_DIR."include/");
			require_once(DOMPDF_INC_DIR . "/functions.inc.php");
			
			def("DOMPDF_FONT_DIR", DOMPDF_DIR . "/lib/fonts/");
			def("DOMPDF_FONT_CACHE", DOMPDF_FONT_DIR);
			def("DOMPDF_TEMP_DIR", sys_get_temp_dir());
			def("DOMPDF_CHROOT", realpath(DOMPDF_DIR));
			def("DOMPDF_UNICODE_ENABLED", true);
			def("DOMPDF_PDF_BACKEND", "CPDF");
			def("DOMPDF_DEFAULT_MEDIA_TYPE", "screen");
			def("DOMPDF_DEFAULT_PAPER_SIZE", "letter");
			def("DOMPDF_DEFAULT_FONT", "serif");
			def("DOMPDF_DPI", 96);
			def("DOMPDF_ENABLE_PHP", false);
			def("DOMPDF_ENABLE_JAVASCRIPT", true);
			def("DOMPDF_ENABLE_REMOTE", false);
			def("DOMPDF_LOG_OUTPUT_FILE", DOMPDF_FONT_DIR."log.htm");
			def("DOMPDF_FONT_HEIGHT_RATIO", 1.1);
			def("DOMPDF_ENABLE_CSS_FLOAT", false);
			def('DEBUGPNG', false);
			def('DEBUGKEEPTEMP', false);
			def('DEBUGCSS', false);
			def('DEBUG_LAYOUT', false);
			def('DEBUG_LAYOUT_LINES', true);
			def('DEBUG_LAYOUT_BLOCKS', true);
			def('DEBUG_LAYOUT_INLINE', true);
			def('DEBUG_LAYOUT_PADDINGBOX', true);

		}

                /**
                 * DOMPDF autoload
                 * @param string $class 
                 */
		public function DOMPDF_autoload($class) {
			
			$filename = DOMPDF_INC_DIR.mb_strtolower($class).".cls.php";
			
			if (is_file($filename))
				require_once($filename);
		}
		
                /**
                 * Init
                 * @throws Exception 
                 */
		public function init() {

			// If SPL autoload functions are available (PHP >= 5.1.2)
			if ( function_exists("spl_autoload_register") ) {
			  $autoload = array($this,"DOMPDF_autoload");
			  $funcs = spl_autoload_functions();

			  // No functions currently in the stack. 
			  if ( $funcs === false ) { 
			    spl_autoload_register($autoload); 
			  }

			  // If PHP >= 5.3 the $prepend argument is available
			  else if ( version_compare(PHP_VERSION, '5.3', '>=') ) {
			    spl_autoload_register($autoload, true, true); 
			  }

			  else {
			    // Unregister existing autoloaders... 
			    $compat = version_compare(PHP_VERSION, '5.1.2', '<=') && 
				      version_compare(PHP_VERSION, '5.1.0', '>=');

			    foreach ($funcs as $func) { 
			      if (is_array($func)) { 
				// :TRICKY: There are some compatibility issues and some 
				// places where we need to error out 
				$reflector = new ReflectionMethod($func[0], $func[1]); 
				if (!$reflector->isStatic()) { 
				  throw new Exception('This function is not compatible with non-static object methods due to PHP Bug #44144.'); 
				}

				// Suprisingly, spl_autoload_register supports the 
				// Class::staticMethod callback format, although call_user_func doesn't 
				if ($compat) $func = implode('::', $func); 
			      }

			      spl_autoload_unregister($func); 
			    } 

			    // Register the new one, thus putting it at the front of the stack... 
			    spl_autoload_register($autoload); 

			    // Now, go back and re-register all of our old ones. 
			    foreach ($funcs as $func) { 
			      spl_autoload_register($func); 
			    }

			    // Be polite and ensure that userland autoload gets retained
			    if ( function_exists("__autoload") ) {
			      spl_autoload_register("__autoload");
			    }
			  }
			}
		}
	}
        
       