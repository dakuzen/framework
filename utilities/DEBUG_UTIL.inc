<?php
    /**
    * Class DEBUG_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */

	class DEBUG_UTIL {
	
		const OUTPUT_FORMAT_HTML 			= "H";
		const OUTPUT_FORMAT_TEXT 			= "T";
		
		/**
		 * SHUTDOWN CALLBACK
		 * @var string $SHUTDOWN_CALLBACK 
		 */
		static $_SHUTDOWN_CALLBACK			= null;

		/**
		 * LAST ERROR
		 * @var array $LAST_ERROR 
		 */                
		static $_LAST_ERROR		 			= array();

		/**
		 * PREVIOUS ERROR HANDLER
		 * @var string $PREVIOUS_ERROR_HANDLER 
		 */
		static $_PREVIOUS_ERROR_HANDLER 	= "";

		static $_PREVIOUS_ERROR_REPORTING 	= null;

		/**
		 * ERROR HANDLER
		 * @var string $ERROR_HANDLER 
		 */
		static $_ERROR_HANDLER 			= null;		

		/**
		 * OUTPUT FORMAT
		 * @var string $OUTPUT_FORMAT 
		 */
		static $_OUTPUT_FORMAT				= self::OUTPUT_FORMAT_HTML;

		/**
		 * OUTPUT FORMAT
		 * @var string $OUTPUT_FORMAT 
		 */
		static $_OUTPUT				= true;

		/**
		 * REPORTABLE ERRORS
		 * @var array $REPORTABLE_ERRORS 
		 */
		static $_REPORTABLE_ERRORS			= array(E_ERROR=>true, 
													E_WARNING=>true,
													E_PARSE=>true,
													E_NOTICE=>true,
													E_CORE_ERROR=>true,
													E_CORE_WARNING=>true,
													E_COMPILE_ERROR=>true,
													E_COMPILE_WARNING=>true,
													E_USER_ERROR=>true,
													E_USER_WARNING=>true,
													E_USER_NOTICE=>true);
			
		/**
		 * Checks debug output format is text
	 	 */
		
		static function is_format_text() 		{ return self::output_format()==self::OUTPUT_FORMAT_TEXT; }
		
		/**
		 * Checks debug output format is html
	 	 */
	 	
		static function is_format_html() 		{ return self::output_format()==self::OUTPUT_FORMAT_HTML; }
		
		/**
		 * Enables debug output format to text
	 	 */
		
		static function enable_format_text() 		{ self::format_text(); }
        
        /**
         * Format text 
         */
		static function format_text() 			{ self::$_OUTPUT_FORMAT = self::OUTPUT_FORMAT_TEXT; }


		static function output_format() 			{ return self::$_OUTPUT_FORMAT; }
		
		/**
		 * Enables debug output format to html
	 	 */
	 	 
		static function enable_format_html() 		{ self::format_html(); }
                
        /**
         * Format html 
         */
		static function format_html() 			{ self::$_OUTPUT_FORMAT = self::OUTPUT_FORMAT_HTML; }
		

		static function output($output=true) { 
			if(func_num_args())
				self::$_OUTPUT = $output;

			return self::$_OUTPUT;
		}
		
        /**
         * Returns the last error
         * @return array 
         */
		static function get_last_error() 		{ return self::$_LAST_ERROR; }
		
		/**
		 * Sets the reportable errors to bits
		 *
		 * @param array $bits 
	 	 */
		
		static function set_reportable_errors($bits)	{ self::$_REPORTABLE_ERRORS = $bits; }
	
		/**
         * Enables system error handler
         * @param string $class
         * @param string $function 
         */
		static function enable_system_error_handler($class="DEBUG_UTIL",$function="system_error_handler") {			
			self::set_error_handler(array($class,$function));
		}

		static function print_backtrace() {
			echo self::get_backtrace_table(self::error(),self::output_format())->get();
		}

		static function error($type=null,$message=null,$file=null,$line=null,$trace=array()) {

			$file = FILE_UTIL::get_sanitized_path($file);

			$error = array("type"=>$type,"message"=>$message,"file"=>$file,"line"=>$line,"backtrace"=>array());

			if(!$trace) {
				$trace = debug_backtrace();
				array_shift($trace);
			}		
		
			for($index=0;$index<count($trace);$index++) {
				
				$backtrace = value($trace,$index);				
				$arguments = value($backtrace,"args",array());
				
				$function_arguments = array();
				foreach($arguments as $argument) {

					$type = gettype($argument);
					
					if($type=="integer" || $type=="double")
						$function_arguments[] = $argument;
						
					elseif($type=="string") {
						
						$argument = htmlspecialchars(STRING_UTIL::get_dot_dot_dot_string($argument,64));
						$function_arguments[] = '"'.str_replace("\\","/",$argument).'"';
					
					} elseif($type=="array")
						$function_arguments[] = "Array(".count($argument).")";
					
					elseif($type=="object")
						$function_arguments[] = 'Object('.get_class($argument).")";
								
					elseif($type=="resource")
						$function_arguments[] = "Resource(".$argument.")";
					
					elseif($type=="boolean")
						$function_arguments[] = $argument ? "True" : "False";
					
					elseif($type=="NULL")
						$function_arguments[] = "null";
					
					else
						$function_arguments[] = "Unknown";									
				}
				
				$file 		= FILE_UTIL::get_sanitized_path(value($backtrace,"file","N/A"));
				$line		= value($backtrace,"line");
				$class 		= value($backtrace,"class");
				$function 	= value($backtrace,"function");
				$type 		= value($backtrace,"type");

				if($function=="system_error_handler" || ($class=="MODEL_COMPLEX" && $function=="__call"))
					continue;
					
				if(preg_match("/Call to undefined CMODEL method/",value($function_arguments,0))) {
					$index++;
					$error["line"] = value($trace,array($index ,"line"));
					$error["file"] = value($trace,array($index ,"file"));
					continue;
				}

				if($class=="DEBUG_UTIL")
					continue;

				$error["backtrace"][] = array("line"=>$line,"file"=>$file,"class"=>$class,"type"=>$type,"function"=>$function,"function_arguments"=>$function_arguments);				
			}

			return $error;
		}
		
		/**
		 * Checks whether the error is reportable and displays it
		 *
		 * @param int $errno integer value of the error type 
		 * @param string $errmsg details on what the error is
		 * @param string $filename file that the error occured
		 * @param int $linenum line number on which the error occured
		 * @return string
		 */

		static function system_error_handler($type,$message,$file,$line) {

			if(!DEBUG_UTIL::is_reportable($type,$message)) 
				return false;

			self::$_LAST_ERROR = self::error($type,$message,$file,$line);
			self::log_last_error();
			self::display_last_error();			
		}
		
		/**
		 * Enables exception error handling whihc throws exceptions for legacy functions that echo out error messages
	 	 */
		
		static function enable_exception_error_handler() {
			self::set_error_handler(array("DEBUG_UTIL","exception_error_handler"));
		}

		static function set_error_handler($function,$error_reporting=null) {

			if(!is_null($error_reporting)) {
				self::$_PREVIOUS_ERROR_REPORTING = ini_get('error_reporting');
				error_reporting($error_reporting);
			}

			set_error_handler($function);
		}

		static function disable_error_handler() {
			self::restore_error_handler();
		}

		static function restore_error_handler() {

			if(!is_null(self::$_PREVIOUS_ERROR_REPORTING)) {
				self::set_error_reporting(self::$_PREVIOUS_ERROR_REPORTING);
				self::$_PREVIOUS_ERROR_REPORTING = null;
			}

			restore_error_handler();
		}
		
		/**
		 * Throws an exception for all error messages
		 *
		 * @param int $errno integer value of the error type 
		 * @param string $errmsg details on what the error is
		 * @param string $filename file that the error occured
		 * @param int $linenum line number on which the error occured
		 * @return null
		 */

		static function exception_error_handler($errno, $errmsg, $filename, $linenum) {
			throw new Exception($errmsg,$errno);
		}

		/**
		 * Displays information about the error 
		 *
		 * @param array $error 
		 * @return array
	 	 */
		
		static function get_backtrace_table($error,$format=null) {

			if(!$format)
				$format = self::is_format_text() ? self::OUTPUT_FORMAT_TEXT : self::OUTPUT_FORMAT_HTML;
				
			if($message=value($error,"message"))
				$data[] = array($message);

			$data[] = array(value($error,"file").(value($error,"line") ? " @ ".value($error,"line") : ""));
			
			if($format==self::OUTPUT_FORMAT_TEXT) {
				
				$data[] = array("");
				$data[] = array("");

				foreach(value($error,"backtrace") as $index=>$trace) {

					$line 					= value($trace,"line");
					$file 					= value($trace,"file");
					$class 					= value($trace,"class");
					$type 					= value($trace,"type");
					$function 				= value($trace,"function");
					$function_arguments 	= (array)value($trace,"function_arguments");

					$data[] = array($file." @ ".$line);
					$data[] = array($class.$type.$function."(".implode(",",$function_arguments).")");
					$data[] = array("");
				}
				
				$data[] = array("");	

			} elseif($format==self::OUTPUT_FORMAT_HTML) {

				$backtrace = "";

				$data[] = array("<br>");
				foreach((array)value($error,"backtrace") as $trace) {

					$line 					= value($trace,"line");
					$file 					= value($trace,"file");
					$class 					= value($trace,"class");
					$type 					= value($trace,"type");
					$function 				= value($trace,"function");
					$function_arguments 	= value($trace,"function_arguments");

					$backtrace .= 	$class.$type.$function."(".implode(",",$function_arguments).")<br>".
									$file." @ ".$line."<br><br>";				
				}

				$data[] = array($backtrace);
			}
			
			if($format==self::OUTPUT_FORMAT_HTML) {
	
				$table = HTML_TABLE_UTIL::create()
							->set_data($data)
							->disable_css();
			} else {

				$table = TEXT_TABLE_UTIL::create()
							->set_data($data)
							->disable_borders()
							->set_column_attributes(0,array("width"=>12))
							->set_column_attributes(1,array("width"=>88));

			}

			return $table;					
		}
		

		static function get_error_type($value) {
			return value(self::get_error_type_list(),$value);
		}

		/**
		 * Returns a list of error codes
		 *
		 * @return array
	 	 */

		static function get_error_type_list() {
			return array(	E_ERROR=> "Error",
							E_WARNING=> "Warning",
							E_PARSE=> "Parsing Error",
							E_NOTICE=> "Notice",
							E_CORE_ERROR=> "Core Error",
							E_CORE_WARNING=> "Core Warning",
							E_COMPILE_ERROR	=> "Compile Error",
							E_COMPILE_WARNING=> "Compile Warning",
							E_USER_ERROR=> "User Error",
							E_USER_WARNING=> "User Warning",
							E_USER_NOTICE=> "User Notice",
							E_STRICT=> "Runtime Notice");
		}
	
		
		/**
		 * Besure for all registerd functions that they are all set to a public scope
                 * @param boolean $handler
		 */
		static function set_fatal_error_handler($handler) {

			//Remove the E_ERROR & E_USER_ERROR error types so it does not echo out "Fatal Error:..."
			DEBUG_UTIL::remove_error_reporting_level(E_USER_ERROR + E_ERROR + E_COMPILE_ERROR);
				
			if($handler) {
				
				self::$_SHUTDOWN_CALLBACK = new SHUTDOWN_CALLBACK($handler);				
				register_shutdown_function(array(self::$_SHUTDOWN_CALLBACK,"call"));
			}
		}
                
        /**
         * Disable fatal error handler
         */
		static function disable_fatal_error_handler() {			
			self::$_SHUTDOWN_CALLBACK->enable(false);
		}

		static function enable_fatal_error_handler() {			
			self::$_SHUTDOWN_CALLBACK->enable(true);
		}

		static function unregister_fatal_error_handler() {
			
			if(DEBUG_UTIL::$_SHUTDOWN_CALLBACK)
				DEBUG_UTIL::$_SHUTDOWN_CALLBACK->unregister();
		}

		/**
		 * Determines whether the error should be displayed
		 *
		 * @param int $errno error code being checked
		 * @param string $errmsg error message
         * @param array $reportable_errors
		 * @return string
	 	 */

		static function is_reportable($errno,$errmsg,$reportable_errors=array()) {

			if(strpos($errmsg,"headers already sent")!==false) return false;
			
			$reportable_errors = $reportable_errors ? $reportable_errors : array_keys(self::$_REPORTABLE_ERRORS);

			if(!in_array($errno,$reportable_errors)) 
				return false;
			
			//If the error reporting is set to zero or a function has a @ placed infront of it
			if(error_reporting()==0)
				return false;

			return true;
		}		

        /**
         * last_error reportable check
         * @return boolean 
         */
		static public function is_last_error_reportable() {	

			//If the error reporting is set to zero or a function has a @ placed infront of it
			if(error_reporting()==0)
				return false;
			
			$errno = value(error_get_last(),"type");

			return self::is_reportable($errno,"");
		}

		static function notify_exception($exception) {
			$error = array("message"=>$exception->getMessage(),"file"=>$exception->getFile(),"line"=>$exception->getLine(),"backtrace"=>$exception->getTrace());
			self::notify_error($error);
		}

		/**
		 * Sends the error message to the recipient 
		 *
		 * @param int $errno integer value of the error type 
		 * @param string $errmsg details on what the error is
         * @param string $filename
		 * @param int $linenum line number on which the error occured
	 	 */
		
		static function notify_error($error) {

			$line = str_repeat("-",100);
			$message = array();
			$message[] = "Date Time: ".date("F j, Y, g:i:s a e");
			
			if(SERVER_UTIL::is_web_application())
				$message[]  = "URL: ".SERVER_UTIL::get_request_url();
				
			$message[] = "Hostname: ".SERVER_UTIL::get_hostname();			
			$message[] = "";
				
			$message[] = self::get_backtrace_table($error,self::OUTPUT_FORMAT_TEXT)->get();
	
			$message[] = '$_SERVER';
			$message[] = $line;
			
			$message[] = MISC_UTIL::print_r($_SERVER);
			$message[] = "";

			if($_POST) {
				$message[] = '$_POST'.$line;
				$message[] = MISC_UTIL::print_r($_POST);
			}			

			if($_GET) {
				$message[] = '$_GET'.$line;
				$message[] = MISC_UTIL::print_r($_GET);
			}
			
			if($_FILES) {
				$message[] = '$_FILES';
				$message[] = $line;
				$message[] = MISC_UTIL::print_r($_FILES);
			}

			try {

				$system_name = SYSTEM_MANAGER::get_system_name() ? SYSTEM_MANAGER::get_system_name() : SYSTEM_MANAGER::get_system_host();
				
				$subject = $system_name.": ".value($error,"message");

				if($error_name=value(self::get_error_type_list(),value($error,"type")))
					$subject = $error_name.": ".$subject;
			
				SYSTEM_MANAGER::get_notify_email($subject,implode("\n\r",$message))->send();
			
			} catch(Exception $e) {

				$message[] = array_merge(array("Failed to send error notification: ".$e->getMessage()."\n\n"),$message);

				if(SERVER_UTIL::is_command_application())
					echo implode("\n\r",$message);
			}
		}
		
		/**
		 * Logs the error message to a flat file
		 *
		 * @param int $errno error code being checked
		 * @param string $errmsg error message
		 * @param string $filename file that the error occured
		 * @param int $linenum line number
		 * @return string
	 	 */

		static function log_last_error() {
			if($log_cmodel=SYSTEM_MANAGER::get_logger()) {
				$error = self::get_last_error();	
				return $log_cmodel->php(value($error,"message"),value($error,"type"),value($error,"file"),value($error,"line"),value($error,"backtrace"));
			}
		}

		static function display_last_error() {

			if(self::output()) {
				$value = self::get_backtrace_table(self::get_last_error())->get();
				
				if(self::is_format_html())		
					$value = self::debug($value);
				
				echo $value;
			}
		}
		
		static function debug($message) {

			if(self::is_format_html())
				return HTML_UTIL::div(HTML_UTIL::tag("div",array("style"=>"font-family:monospace;border:1px solid #ccc;display:inline-block;padding:15px;margin:15px;white-space: pre;"),$message));

			return $message;
		}

        /**
         * Error bit to array
         * @param string $value
         * @return string 
         */
		static function error_bit_to_array($value) {

			$level_names = array(	E_ERROR=>"E_ERROR", 
									E_WARNING => "E_WARNING",
									E_PARSE => 'E_PARSE', 
									E_NOTICE => 'E_NOTICE',
									E_CORE_ERROR => 'E_CORE_ERROR', 
									E_CORE_WARNING => 'E_CORE_WARNING',
									E_COMPILE_ERROR => 'E_COMPILE_ERROR', 
									E_COMPILE_WARNING => 'E_COMPILE_WARNING',
									E_USER_ERROR => 'E_USER_ERROR', 
									E_USER_WARNING => 'E_USER_WARNING',
									E_USER_NOTICE => 'E_USER_NOTICE' );
			
			if(defined("E_STRICT")) 
				$level_names[E_STRICT] = "E_STRICT";
				
			$levels = array();
			if(($value & E_ALL)==E_ALL) {
				$levels[E_ALL]= "E_ALL";
				$value &= ~E_ALL;
			}
			
			foreach($level_names as $level=>$name)
				if(($value & $level)==$level) 
					$levels[$level]=$name;
					
			return $levels;
		}
		
		/**
		 * Sets the reportable error 
		 *
		 * @param array $bit
	 	 */
		
		static function set_error_reporting($bit) {
			error_reporting($bit);			
			$bits = self::error_bit_to_array($bit);			
			self::set_reportable_errors($bits);
		}

        /**
         * Remove error reporting
         * @param string $bit
         * @todo review type 
         */
		static function remove_error_reporting_level($bit) {		
			error_reporting(error_reporting() ^ $bit); 
		}
	}

    /**
    * Class SHUTDOWN_CALLBACK
    *
    * @package Framework
    * @subpackage Utilities
    */
    
	class SHUTDOWN_CALLBACK {

        /**
         * Callback
         * @var string $callback
         */
		private $callback;

		private $enabled = true;
        
        /**
         * Constructor
         * @param string $callback 
         */
		public function __construct($callback) {
			if(is_callable($callback))
				$this->callback = $callback;
			else
				die("The shutdown callback is not callback");
		}
        
        /**
         * Call
         * @return boolean 
         */
		public function call() {

			if(!$this->callback || !$this->enabled)
				return false;

			return call_user_func($this->callback);
		}

        /**
         * Unregister 
         */
		public function unregister() {
			$this->callback = false;
		}

		public function enable($enable) {
			$this->enabled = $enable;
		}
	}	