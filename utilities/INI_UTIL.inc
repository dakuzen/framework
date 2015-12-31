<?
    /**
    * Class INI_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */
 
	class INI_UTIL {
		
		const EXCEPTION_FILE_NOT_EXIST = 10000;

        /**
         * Ini
         * @var array $ini 
         */
		protected $_ini 		= array();
                
        /**
         * Has sections
         * @var boolean $has_sections 
         */
		protected $_has_sections 	= true;
        
        /**
         * Exception
         * @var boolean $exception 
         */
		protected $_exception	 	= false;
        
        /**
         * Ini file
         * @var string $ini_file 
         */
		protected $_ini_file		= null;

		protected $_changed		= false;

        /**
         * Constructor.
         * 
         * @param string $ini_file string path of the file name
         * @param boolean $exception the exception
         */
		function __construct($ini_file=null,$exception=true) {
			
			$this->_exception 	= $exception;
			
			if($ini_file)
				$this->load($ini_file);			
		}
		
		/**
		 * Loads an ini file
		 *
		 * @param string $ini_file string path of the file name
                 * @param boolean $exception the exception
		 * @return boolean
		 */
	 
		function load($ini_file) {
			
			$this->_ini_file = $ini_file;

			if(!is_file($ini_file)) {
				if($this->_exception)
					throw new Exception("The ini file does not exist",self::EXCEPTION_FILE_NOT_EXIST);

				return false;
			}
			
			$e = null;
			
			if($this->_exception)
				DEBUG_UTIL::enable_exception_error_handler();

			try {
			
				$this->_ini = @parse_ini_file($ini_file,$this->_has_sections,INI_SCANNER_RAW);
			
			} catch(Exception $e) {
				DEBUG_UTIL::restore_error_handler();
			}
			
			if($this->_exception)  
				DEBUG_UTIL::restore_error_handler();
				
			if($e) 
				throw $e;
		}
		
        /**
         * Return ini properties
         * @return string 
         */
		function get_properties() {  return $this->_ini; }
		
        /**
         * Return values
         * @param mixed $keys
         * @return string 
         */
		function get($keys,$default=null) { 
			if(!is_array($keys))
				$keys = array($keys);
			
			$value = value($this->_ini,$keys,$default);

			if(is_string($value))
				$value = trim($value);

			return MISC_UTIL::type_cast($value);
		}

		function exists($keys) {			
			return $this->get($keys)!==null;
		}

        /**
         * set values to keys
         * @param mix $keys
         * @param mix $value 
         */
		function set($keys,$value) { 
			
			$this->_changed = true;

			if(is_array($keys))
				$this->_ini[get_value($keys,0)][get_value($keys,1)] = $value;
			else {
				$ini = is_array($this->_ini) ? $this->_ini : array();
				unset($ini[$keys]);
				$this->_ini = array_merge(array($keys=>$value),$ini);
			}
		}
                
        /**
         * Return the contents
         * @param array $array
         * @return string 
         */
		function get_content($array) {
			$content = "";
			
			if(!is_array($array)) 
				return $content;
				
			foreach($array as $key=>$item) {
				if(is_array($item)) {
					$content .= "\n[".$key."]\n";
					$content .= $this->get_content($item)."\n";
				} else 
					$content .= $key." = ".$item."\n";						
			}
			
			return $content;
		}
		
		/**
		 * Saves the file contents
		 *
		 * @return string
		 */
		function save() {
			$content = $this->get_content($this->_ini);			
			return FILE_UTIL::put_file_contents($this->_ini_file,$content);			
		}	

		function is_changed() { return $this->_changed; }
					
	}

