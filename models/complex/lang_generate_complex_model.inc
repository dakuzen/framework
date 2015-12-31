<?php
        /**
        * Class BASE_CMODEL_LANG_GENERATE
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_LANG_GENERATE {
		
                /**
                 * The web dirs
                 * @var array $web_dirs
                 */
		protected $_web_dirs = array();
		
                /**
                 * Constructor 
                 */
		public function __construct() {
			$this->_web_dirs[] = MODEL_PATH::get_application_directory()."web/js/langs/";
		}
		
                /**
                 * Adds web dir
                 * @param string $web_dir 
                 */
		public function add_web_dir($web_dir) { $this->_web_dirs[] = $web_dir; }
		
                /**
                 * generate
                 */
		public function generate() {
			
			$langs_dir = MODEL_PATH::get_application_directory()."langs/";
			
			$langs = FILE_UTIL::get_directory_listing($langs_dir,"*",false,true);
			
			foreach($this->_web_dirs as $web_langs_dir)
				FILE_UTIL::mkdir($web_langs_dir);

			foreach($langs as $lang) {

				$lang_dir 	= $langs_dir.$lang."/";
				$lang_ini_file	= $lang_dir.$lang.".ini";
				$lang_inc_file	= $lang_dir.$lang.".inc";
				
				$init_util = new INI_UTIL($lang_ini_file,true);
				
				$inc = "<?\n\n";

				foreach(get_value($init_util->get_properties(),"PHP",array()) as $name=>$value) {

					$value = str_replace('"','\\"',$value);

					$inc .= "\t".'define("'.$name.'","'.$value.'",true);'.chr(10);		
				}

				FILE_UTIL::put_file_contents($lang_inc_file,$inc);
				
				foreach($this->_web_dirs as $web_dir) {
				
					$lang_js_file	= $web_langs_dir.$lang.".js";

					$js = "Lang = {}\n";

					foreach(get_value($init_util->get_properties(),"JS",array()) as $name=>$value) {

						$value = str_replace('"','\\"',$value);

						$js .= 'Lang.'.$name.' = "'.$value.'";'.chr(10);
					}

					FILE_UTIL::put_file_contents($lang_js_file,$js);
				}
			}
		}		
	}
	
	
	
	