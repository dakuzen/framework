<?
   /**
    * SMARTY_UTIL class page
    *
    * @package Framework
    * @subpackage Utilities
    */

    require_once(MODEL_PATH::get_framework_packages_directory()."Smarty/Smarty.class.php");
    
    /**
    * Class SMARTY_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */
	
	class SMARTY_UTIL extends Smarty {
		
        /**
         * The string.
         * @var string $string 
         */
		static $_string = "";

        /**
         * Construct
         * 
         * @param string $compile_dir the compile directory
         * @param string $cache_dir the cache directory
         * @param string $template_dir the template directory
         * @param string $config_dir the config directory
         * @param string $plugins_dir the plugins directory
         */
		function __construct($compile_dir=null,$cache_dir=null,$template_dir=null,$config_dir=null,$plugins_dir=null) {
		
			$temp_dir = MODEL_PATH::get_temp_directory()."smarty/";
			
			$this->template_dir 	= $template_dir ? $template_dir : $temp_dir."templates/";
			$this->compile_dir  	= $compile_dir ? $compile_dir : $temp_dir."templates_c/";
			$this->config_dir   	= $config_dir ? $config_dir : $temp_dir."configs/";
			$this->cache_dir   	= $cache_dir ? $cache_dir : $temp_dir."cache/";
			$this->plugins_dir   	= $plugins_dir ? $plugins_dir : MODEL_PATH::get_framework_packages_directory()."Smarty/plugins/";
			$this->force_compile	= true;
			
			$this->create_directories();
		}
		
        /**
         * Sets the delimiters
         * 
         * @param integer $left the left delimiter
         * @param integer $right the right delimiter
         */
		function set_delimiters($left,$right) {
			$this->right_delimiter	= $right;
			$this->left_delimiter	= $left;
		}
		
		/**
         * Create directories
         * 
         */
		function create_directories() {	
			FILE_UTIL::mkdir($this->template_dir);
			FILE_UTIL::mkdir($this->compile_dir);
			FILE_UTIL::mkdir($this->config_dir);
			FILE_UTIL::mkdir($this->cache_dir);
		}

        /**
         * Debug set to true
         * 
         */
		function debug() {
			$this->debugging = true;			
		}
		
        /**
         * fetch a string
         * 
         * @param string $string the string to fetch
         */
		function fetch_string($string) {
			$smarty->caching = false;
			return $this->fetch("string:".$string);
		}
	}
        
        