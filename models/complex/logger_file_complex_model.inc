<?
    /**
    * Class BASE_CMODEL_LOGGER_FILE
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_LOGGER_FILE extends BASE_CMODEL_LOGGER {
		
		const ENCODING_JSON 		= "json";

        /**
         * The log directory
         * @var string $log_directory 
         */
        protected $_log_directory 	= "";

        /**
         * The file name
         * @var string $file_name 
         */
        protected $_file_name 		= "application.log";

        /**
         * The encoding
         * @var string $encoding 
         */
        protected $_encoding	 	= null;

        /**
         * rotate limit bites
         * @var integer 
         */
        protected $_rotate_limit_bites 	= 0;

        /**
         * timestamp format
         * @var string $timestamp_format 
         */
        protected $_timestamp_format 	= "%Y-%m-%d %H:%M:%S";

        /**
         * The prefix
         * @var string $prefix 
         */
		protected $_prefix	 	= "[%Y-%m-%d %H:%M:%S {microseconds}] [{level}] ";
				
        /**
         * Constructor
         * @param string $log_directory
         * @param string $file_name
         * @param integer $rotate_limit_bites 
         */
		public function __construct($log_directory,$file_name="application.log",$rotate_limit_bites=5000) {
			$this->_log_directory 	 	= $log_directory;	
			$this->_file_name 		= $file_name;
			$this->_rotate_limit_bites 	= $rotate_limit_bites;
			
			if($log_directory)
				FILE_UTIL::mkdir($log_directory);
		}

        /**
         * Create 
         * @param string $level
         * @param string $log_directory
         * @param string $file_name
         * @return object 
         */
		public function create($log_directory="",$file_name="") {
			
			$file_name 	= $file_name ? $file_name : "application.log";
			$log_directory 	= $log_directory ? $log_directory : MODEL_PATH::get_logs_directory();
			
			$log_cmodel = new BASE_CMODEL_LOGGER_FILE($log_directory,$file_name);
			
			self::$_instance = $log_cmodel;
			
			return self::$_instance;
		}
		
        /**
         * Return log file
         * @return string 
         */
		public function get_log_file() 			{ return $this->_log_directory.$this->_file_name; }
        
        /**
         * Returns prefix
         * @return string 
         */
		public function get_prefix() 			{ return $this->_prefix; }
            
        /**
         * Sets prefix 
         * @param string $prefix 
         */
		public function set_prefix($prefix) 		{ $this->_prefix = $prefix; }

        /**
         * log
         * @param array $data
         * @param string $level
         * @param string $code
         * @return boolean 
         */
		public function log($data,$level,$code="") {
		
			if(!$this->has_level($level))
				return true;
			
			$filesize = FILE_UTIL::get_filesize($this->get_log_file());
			
			if($filesize>($this->_rotate_limit_bites*1000)) {
				
				preg_match("/(.*?)(?:\.([^\.]+)|)$/",$this->get_log_file(),$matches);
				
				$basename = get_value($matches,1);
				$ext 	  = get_value($matches,2);
				
				$rotate_file = $basename."-".strftime("%Y-%m-%d").($ext ? ".".$ext : "");
				
				FILE_UTIL::rename($this->get_log_file(),$rotate_file);
			}
			
	  		$mtime = explode(" ", microtime());
	    		$msec = number_format(get_value($mtime,0),8);
	    		
			$replaces["{level}"] 		= $level;
			$replaces["{microseconds}"] 	= $msec;
				
			$prefix = str_replace(array_keys($replaces),array_values($replaces),strftime($this->_prefix));
			
			if($this->_encoding==self::ENCODING_JSON || is_array($data))
				$data = JSON_UTIL::encode($data);
			
			$log = $prefix.$data;
			$fileh = @fopen($this->get_log_file(), "a");
			
			if($fileh)
				fwrite($fileh, $log."\r\n");
		}
	}
	