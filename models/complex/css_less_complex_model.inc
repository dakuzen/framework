<?php
       /**
        * BASE_CMODEL_CSS_LESS class file
        *
        * @package Framework
        * @subpackage Cmodels
        */
    
	require_once BASE_MODEL_PATH::get_framework_packages_directory()."less/lessc.inc.php";
	
       /**
        * Class BASE_CMODEL_CSS_LESS
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_CSS_LESS extends lessc {
		
                /**
                 * Previous error handler
                 * @var string 
                 */
		protected $_previous_error_handler = null;

                /**
                 * Parse css
                 * @param string $css
                 * @return string
                 * @throws null 
                 */
		public function parse($css) {

			$this->_previous_error_handler = set_error_handler(array($this,"error_handler"));

			$exception = null;
			try {
				$css = parent::parse($css);
			} catch(Exception $exception) {

			}

			if($this->_previous_error_handler)
					set_error_handler($this->_previous_error_handler);

			if($exception)
				throw $exception;

			return $css;
		}

                /**
                 * error handler
                 * @param string $errno
                 * @param string $errmsg
                 * @param string $filename
                 * @param string $linenum
                 * @throws Exception 
                 */
		public function error_handler($errno, $errmsg, $filename, $linenum) {
			throw new Exception("There was an internal LESS parsing engine error. Please be sure that you are suppling valid CSS.");			
		}

                /**
                 * Add import directory
                 * @param string $dir 
                 */
		public function add_import_directory($dir) {
			$this->importDir = is_array($this->importDir) ? $this->importDir : array();
			$this->importDir = array_merge($this->importDir,array($dir));
		}			
	}