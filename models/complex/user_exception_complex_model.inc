<?php
        /**
        * Class BASE_CMODEL_USER_EXCEPTION
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_USER_EXCEPTION extends Exception {
		
                /**
                 * user message
                 * @var string $user_message
                 */
		protected $_user_message = "";
		
                /**
                 * Constructor
                 * @param string $user_message
                 * @param string $message
                 * @param string $code
                 * @param string $level 
                 */
		public function __construct($user_message,$message,$code=null,$level=BASE_CMODEL_LOGGER::LEVEL_DEBUG) {			
			$message = $message ? $message : $user_message;			
			$this->_user_message = $user_message;
			parent::__construct($message,$code);
		}
		
                /**
                 * emergency message
                 * @param string $user_message
                 * @param string $message
                 * @param string $code
                 * @return object BASE_CMODEL_USER_EXCEPTION 
                 */
		public static function emergency($user_message,$message="",$code=null) 	{ return new BASE_CMODEL_USER_EXCEPTION($user_message,$message,$code,BASE_CMODEL_LOGGER::LEVEL_EMERGENCY); }
		
                /**
                 * alert message
                 * @param string $user_message
                 * @param string $message
                 * @param string $code
                 * @return object BASE_CMODEL_USER_EXCEPTION 
                 */
                public static function alert($user_message,$message="",$code=null) 	{ return new BASE_CMODEL_USER_EXCEPTION($user_message,$message,$code,BASE_CMODEL_LOGGER::LEVEL_ALERT); }
		
                /**
                 * critical message
                 * @param string $user_message
                 * @param string $message
                 * @param string $code
                 * @return object BASE_CMODEL_USER_EXCEPTION 
                 */
                public static function critical($user_message,$message="",$code=null) 	{ return new BASE_CMODEL_USER_EXCEPTION($user_message,$message,$code,BASE_CMODEL_LOGGER::LEVEL_CRITICAL); }
		
                /**
                 * error message
                 * @param string $user_message
                 * @param string $message
                 * @param string $code
                 * @return object BASE_CMODEL_USER_EXCEPTION 
                 */
                public static function error($user_message,$message="",$code=null) 	{ return new BASE_CMODEL_USER_EXCEPTION($user_message,$message,$code,BASE_CMODEL_LOGGER::LEVEL_ERROR); }
		
                /**
                 * warning message
                 * @param string $user_message
                 * @param string $message
                 * @param string $code
                 * @return object BASE_CMODEL_USER_EXCEPTION 
                 */
                public static function warning($user_message,$message="",$code=null) 	{ return new BASE_CMODEL_USER_EXCEPTION($user_message,$message,$code,BASE_CMODEL_LOGGER::LEVEL_WARNING); }
		
                /**
                 * notice message
                 * @param stringtype $user_message
                 * @param string $message
                 * @param string $code
                 * @return obejct BASE_CMODEL_USER_EXCEPTION 
                 */
                public static function notice($user_message,$message="",$code=null) 	{ return new BASE_CMODEL_USER_EXCEPTION($user_message,$message,$code,BASE_CMODEL_LOGGER::LEVEL_NOTICE); }
		
                /**
                 * info message
                 * @param string $user_message
                 * @param string $message
                 * @param string $code
                 * @return object BASE_CMODEL_USER_EXCEPTION 
                 */
                public static function info($user_message,$message="",$code=null) 	{ return new BASE_CMODEL_USER_EXCEPTION($user_message,$message,$code,BASE_CMODEL_LOGGER::LEVEL_INFO); }
		
                /**
                 * debug message
                 * @param string $user_message
                 * @param string $message
                 * @param string $code
                 * @return object BASE_CMODEL_USER_EXCEPTION 
                 */
                public static function debug($user_message,$message="",$code=null) 	{ return new BASE_CMODEL_USER_EXCEPTION($user_message,$message,$code,BASE_CMODEL_LOGGER::LEVEL_DEBUG); }
			
                /**
                 * Returns the user message
                 * @return string 
                 */
		public function get_user_message() 					{ return $this->_user_message; }
                
                /**
                 * returns the message
                 * @return string
                 */
		public function get_message() 						{ return $this->getMessage(); }
	}