<?php
    /**
    * Class BASE_CMODEL_LOGGER
    *
    * @package Framework
    * @subpackage Cmodels
    */

	abstract class BASE_CMODEL_LOGGER {
                
		const LEVEL_EMERGENCY	= "emergency";
		const LEVEL_ALERT		= "alert";
		const LEVEL_CRITICAL	= "critical";
		const LEVEL_ERROR		= "error";
		const LEVEL_WARNING		= "warning";
		const LEVEL_NOTICE		= "notice";
		const LEVEL_INFO		= "info";
		const LEVEL_DEBUG		= "debug";

        /**
         * The instance
         * @var object $instance
         */
		static protected $_instance 	= "";

        protected $_notify              = false;
        /**
         * Returns the level list
         * @return array 
         */
		static function get_level_list() { return array(	self::LEVEL_EMERGENCY=>"Emergency",
															self::LEVEL_ALERT=>"Alert",
															self::LEVEL_CRITICAL=>"Critical",
															self::LEVEL_ERROR=>"Error",
															self::LEVEL_WARNING=>"Warning",
															self::LEVEL_NOTICE=>"Notice",
															self::LEVEL_INFO=>"Info",
															self::LEVEL_DEBUG=>"Debug"); }
                                                                
		/**
         * the level
         * @var string $level 
         */
		protected $_level = self::LEVEL_DEBUG;

        /**
         * emergency
         * @param object $data
         * @param string $code
         * @return boolean
         * @todo review return type 
         */
        public function emergency($data,$code="") { 
            if($this->has_level($error_type))                
                $this->log($data,self::LEVEL_EMERGENCY,array("code"=>$code)); 
            return $this;
        }
        
        /**
         * alert
         * @param object $data
         * @param string $code
         * @return boolean 
         */
        public function alert($data,$code="") { 
            if($this->has_level($error_type))    
                $this->log($data,self::LEVEL_ALERT,array("code"=>$code)); 
            return $this;
        }

        /**
         * critical
         * @param object $data
         * @param string $code
         * @return boolean 
         */
        public function critical($data,$code="") {
            if($this->has_level($error_type))    
                $this->log($data,self::LEVEL_CRITICAL,array("code"=>$code)); 
            return $this;
        }

        /**
         * error
         * @param object $data
         * @param string $code
         * @return boolean 
         */
        public function error($data,$code="") {
            if($this->has_level($error_type))    
                $this->log($data,self::LEVEL_ERROR,array("code"=>$code)); 
            return $this;
        }

        /**
         * warning
         * @param object $data
         * @param string $code
         * @return boolean 
         */
        public function warning($data,$code="") {
            if($this->has_level($error_type))    
                $this->log($data,self::LEVEL_WARNING,array("code"=>$code));
            return $this; 
        }

        /**
         * notice
         * @param object $data
         * @param string $code
         * @return boolean 
         */
        public function notice($data,$code="") {
            if($this->has_level($error_type))    
                $this->log($data,self::LEVEL_NOTICE,array("code"=>$code)); 
            return $this;
        }

        /**
         * info
         * @param object $data
         * @param string $code
         * @return boolean 
         */
        public function info($data,$code="") {
            if($this->has_level($error_type))    
                $this->log($data,self::LEVEL_INFO,array("code"=>$code));
            return $this; 
        }

        /**
         * debug
         * @param object $data
         * @param string $code
         * @return boolean 
         */
        public function debug($data,$code="") {
            if($this->has_level($error_type))    
                $this->log($data,self::LEVEL_DEBUG,array("code"=>$code));
            return $this; 
        }

        /**
         * debug
         * @param object $data
         * @param string $code
         * @return boolean 
         */
        public function php($message, $type, $file, $line, $backtrace) {
            $this->log($message,DEBUG_UTIL::get_error_type($type),array("file"=>$file,"line"=>$line,"backtrace"=>$backtrace)); 
            return $this;
        }

        /**
         * log
         * @param array $data
         * @param string $level
         * @param string $code 
         */
		abstract public function log($message,$type,$data=array());

        /**
         * Returns instance
         * @return object 
         */
		static public function get_instance() 		{ return self::$_instance; }
		
        /**
         * Sets the level
         * @param string $level
         * @return boolean 
         */
		public function set_level($level)		{ return $this->_level = $level; }
        
        /**
         * Checks whether has level
         * @param string $level
         * @return boolean 
         */
		public function has_level($level)		{ return $this->_level>=$level; }
				
        /**
         * initialize log file
         * @param string $level
         * @param string $log_directory
         * @param string $file_name
         * @return boolean
         * @todo review return type 
         */
		public function initialize_log_file($log_directory="",$file_name="") {
			return BASE_CMODEL_LOGGER_FILE::create($log_directory,$file_name);			
		}
		
        /**
         * initialize log db
         * @param string $level
         * @return boolean
         * @todo review return type 
         */
		public function initialize_log_db() {
			return BASE_CMODEL_LOGGER_DB::create();
		}

        public function notify()        { $this->_notify = true; return $this; }
        
	}