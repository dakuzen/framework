<?
    /**
    * Class COMMAND_APPLICATION  
    *
    * @package Library
    * @subpackage Command
    */
 
	class COMMAND_APPLICATION extends BASE_APPLICATION {

	    /**
	     * The uri
	     * @var string $uri 
	     */
		private $_uri = null;

        /**
         * Returns instance
         * @return object 
         */
		static function get_instance() {
			if( self::$_instance==null ){
				self::$_instance = new APPLICATION();
			}
			return self::$_instance;
		}

        /**
         * Initialize
         * @param boolean $enable_error_reporting 
         */
		function initialize($enable_error_reporting=true) {
			
			parent::initialize($enable_error_reporting);
			
			DEBUG_UTIL::format_text();
			DEBUG_UTIL::enable_system_error_handler();
			
			SYSTEM_MANAGER::initialize();

			$this->initialize_error_handling();
			
			$this->initialize_registry();
					
			$this->initialize_database();
		}
		
        /**
         * process fatal error
         */
		public function render_fatal_error() {}

        /**
         * initialize registry
         * @global array $argv 
         */
		protected function initialize_registry() {
			
			$this->_registry->set_parm(REGISTRY::PARM_LANG,CONSTANT::LANGUAGE_EN);
			
			global $argv;
			array_shift($argv);
			
			$args = array();
			
			for($i=0;$i<count($argv);$i++) {
				
				$arg = get_value($argv,$i);
				
				if(strpos($arg,"-")===0) {
					
					$next_val = get_value($argv,($i+1));
					
					if(strpos($next_val,"-")===false) 					
						$i++;
					else $next_val = "";
					
					$arg = ltrim($arg,"-");
					
					$args[$arg] = $next_val;
				}				
			}
			
			$this->_registry->set_parm("args",$args);					
		}
		
        /**
         * Process 
         */
		function process() {}

        /**
         * fatal error cleanup
         */
		function fatal_error_cleanup() 				{ return "Fatal Error Detected"; }
		
        /**
         * display error message
         * @param string $message 
         */
		static function display_error_message($message) 	{ self::display_message("ERROR:    ".$message); }
		
        /**
         * display warning message
         * @param string $message 
         */
        static function display_warning_message($message) 	{ self::display_message("WANRNING: ".$message); }		

        /**
         * display notify message
         * @param string $message 
         */
        static function display_notify_message($message) 	{ self::display_message("NOTIFY:   ".$message); }

        /**
         * display error messages
         * @param array $messages
         */
		static function display_error_messages($messages) { 
			foreach($messages as $message)
				self::display_error_message($message); 
		}
		
        /**
         * display notify messages
         * @param array $messages
         */
		static function display_notify_messages($messages) { 
			foreach($messages as $message)
				self::display_notify_message($message); 
		}
		
        /**
         * display warning messages
         * @param array $messages
         */
		static function display_warning_messages($messages) { 
			foreach($messages as $message)
				self::display_warning_message($message); 
		}		

        /**
         * Display messages
         * @param MESSAGE $obj 
         */
		static function display_messages(MESSAGE $obj) {
			self::display_error_messages($obj->get_error_messages());
			self::display_notify_messages($obj->get_notify_messages());
		}		

        /**
         * display
         * @param string $string
         * @param integer $new_lines 
         */
		static function display($string,$new_lines=0) 		{ echo $string; self::display_newline($new_lines); }
		
        /**
         * display message
         * @param string $message
         * @param integer $new_lines 
         */
        static function display_message($message,$new_lines=1) 	{ self::display($message).self::display_newline($new_lines); }

        /**
         * display newline
         * @param integer $new_lines 
         */
        static function display_newline($new_lines=1) 		{ echo str_repeat(" \n",$new_lines); }
        
        /**
         * Display header
         * @param string $message 
         */
		static function display_header($message)		{ echo "\n".$message."\n".self::get_horizontal_line()."\n\n"; }	
		
        /**
         * display horizontal line
         * @param integer $width 
         */
        static function display_horizontal_line($width=80)	{ echo self::get_horizontal_line($width); }	

        /**
         * Return horizontal line
         * @param integer $width
         * @return string 
         */
		static function get_horizontal_line($width=80)		{ return implode("",array_fill(0,$width,"-"))."\n"; }
		
        /**
         * Prompt message
         * @param string $message
         * @param string $default 
         */
		static function prompt_message($message,$default="") {
			$default = $default ? " [".$default."]" : "";
			echo "\n".$message.$default.": "; 
		}	
		
		
	}
