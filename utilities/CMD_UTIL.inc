<?php
        /**
        * Class CMD_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */

	class CMD_UTIL {
            
                /**
                 * The tty.
                 * @var string $tty 
                 */
		static $tty;
                
                /**
                 * Whether is initialized.
                 * @var boolean $initialized 
                 */
		static $initialized = false;

		/**
                 * Initialize.
		 * @todo: Ray
	 	 */
	 	static private function initialize() {
			if(!self::$initialized) {
				set_time_limit(0);
				@ob_end_flush();
				ob_implicit_flush(true);			
				self::$initialized = true;
			}
		}

                /**
                 * Get the input.
		 * 
                 * @param integer $length the length
                 * 
                 * @return input string
	 	 */
		static private function get_input($length = 1024) {
			self::initialize();
			
			if (substr(PHP_OS, 0, 3) == "WIN") {
				self::$tty = fOpen("\con", "rb");
			} else {
				if (!(self::$tty = fOpen("/dev/tty", "r"))) 
					self::$tty = fOpen("php://stdin", "r");				
			}			
			$value = fGets(self::$tty, $length);			
			
			if($value===false) exit(0);			
			return $value;
		}
		
                /**
                 * The prompt.
                 * 
		 * @param string $message the message
                 * @param string $default_response the default response
                 * @param array $valid_responses the valid responses
                 * 
                 * @return the response
	 	 */
		static function prompt($message,$default_response=null,$valid_responses=array()) {
			
			$message = rtrim($message,":");
			$message = $default_response ? $message." [".$default_response."]: " : $message.": ";
				
			do {
				
				echo rtrim($message)." ";
				
				$response = self::get();
				
				if($default_response && !$response)
					$response = $default_response;
				
			} while(!is_empty($valid_responses) && !in_array($response,$valid_responses));
			
			return $response;
		}
		
                /**
                 * Get.
                 * 
		 * @return input
	 	 */
		static function get() {

			do {			
				$input = self::get_input();
				
				$input = trim(str_replace("\n","",$input));
				return $input;
				
			} while(true);
		}
		
                /**
                 * Whether is default response.
                 * 
                 * @param string $response the reponse
                 * 
		 * @return boolean
	 	 */
		static function is_default_response($response) {
			 $value = self::get();
			 return ($value==$response || $value=="");		 
		}		
		
	}