<?php

        /**
        * Class MESSAGE  
        *
        * @package Library
        * @subpackage Common
        */
	 
	class MESSAGE extends OBJECT {

		const MSG_ERROR			= 0;
		const MSG_NOTIFY		= 1;
		const MSG_WARNING		= 2;
		const MSG_DEBUG			= 3;
		const MSG_USER_ERROR	= 4;

		/**
		 * log report
		 * @var string $log_report 
		 */
		private $_log_report	= "";

		/**
		 * The messages
		 * @var aray $messages 
		 */
		private $_messages = array();

		/**
		 * Constructor 
		 */
		function __construct() {}

		/**
		 * Adds error message
		 * @param string $message 
		 */
		function add_error_message($message) 		{ $this->add_message(MESSAGE::MSG_ERROR,$message); }
		
		/**
		 * Adds notify message
		 * @param string $message 
		 */
		function add_notify_message($message) 		{ $this->add_message(MESSAGE::MSG_NOTIFY,$message); }
		
		/**
		 * Adds warning message
		 * @param string $message 
		 */
		function add_warning_message($message) 		{ $this->add_message(MESSAGE::MSG_WARNING,$message); }
		
		/**
		 * Adds debug message
		 * @param string $message 
		 */
		function add_debug_message($message) 		{ $this->add_message(MESSAGE::MSG_DEBUG,$message); }
		
		/**
		 * Adds user error message
		 * @param string $message 
		 */
		function add_user_error_message($message) 	{ $this->add_message(MESSAGE::MSG_USER_ERROR,$message); }
		
		/**
		 * Adds error messages
		 * @param array $messages 
		 */
		function add_error_messages($messages) 		{ $this->add_messages_by_type(MESSAGE::MSG_ERROR,$messages); }
		
		/**
		 * Adds notify messages
		 * @param array $messages 
		 */
		function add_notify_messages($messages) 	{ $this->add_messages_by_type(MESSAGE::MSG_NOTIFY,$messages); }
		
		/**
		 * Adds warning messages
		 * @param array $messages 
		 */
		function add_warning_messages($messages) 	{ $this->add_messages_by_type(MESSAGE::MSG_WARNING,$messages); }
		
		/**
		 * Adds debug messages
		 * @param array $messages 
		 */
		function add_debug_messages($messages) 		{ $this->add_messages_by_type(MESSAGE::MSG_DEBUG,$messages); }
		
		/**
		 * Adds user error messages
		 * @param array $messages 
		 */
		function add_user_error_messages($messages) 	{ $this->add_messages_by_type(MESSAGE::MSG_USER_ERROR,$messages); }

		/**
		 * Returns the error messages
		 * @return array 
		 */
		function get_error_messages() 			{ return get_value($this->_messages,MESSAGE::MSG_ERROR,array()); }
		
		/**
		 * Returns the notify messages
		 * @return array 
		 */
		function get_notify_messages() 			{ return get_value($this->_messages,MESSAGE::MSG_NOTIFY,array()); }
		
		/**
		 * Returns the warning messages
		 * @return array 
		 */
		function get_warning_messages() 		{ return get_value($this->_messages,MESSAGE::MSG_WARNING,array()); }
		
		/**
		 * Returns the debug messages
		 * @return array 
		 */
		function get_debug_messages() 			{ return get_value($this->_messages,MESSAGE::MSG_DEBUG,array()); }
		
		/**
		 * Returns the user error messages
		 * @return array 
		 */
		function get_user_error_messages() 		{ return get_value($this->_messages,MESSAGE::MSG_USER_ERROR,array()); }
		
		/**
		 * Sets the error messages
		 * @param array $messages 
		 */
		function set_error_messages($messages) 		{ $this->_messages[MESSAGE::MSG_ERROR] = $messages; }
		
		/**
		 * Sets the notifym essages
		 * @param array $messages 
		 */
		function set_notify_messages($messages) 	{ $this->_messages[MESSAGE::MSG_NOTIFY] = $messages; }
		
		/**
		 * Sets the warning messages
		 * @param array $messages 
		 */
		function set_warning_messages($messages) 	{ $this->_messages[MESSAGE::MSG_WARNING] = $messages; }
		
		/**
		 * Sets the debug messages
		 * @param array $messages 
		 */
		function set_debug_messages($messages) 		{ $this->_messages[MESSAGE::MSG_DEBUG] = $messages; }
		
		/**
		 * Sets the user error messages
		 * @param array $messages 
		 */
		function set_user_error_messages($messages) 	{ $this->_messages[MESSAGE::MSG_USER_ERROR] = $messages; }		

		/**
		 * Checks whether has failed
		 * @return boolean
		 */
		function has_failed() 				{ return $this->has_error_messages(); }

		/**
		 * Checks whether has error messages
		 * @return boolean
		 */
		function has_error_messages() 			{ return count($this->get_error_messages())>0; }
		
		/**
		 * Checks whether has notify messages
		 * @return boolean
		 */
		function has_notify_messages() 			{ return count($this->get_notify_messages())>0; }
		
		/**
		 * Checks whether has warning messages
		 * @return boolean
		 */
		function has_warning_messages() 		{ return count($this->get_warning_messages())>0; }
		
		/**
		 * Checks whether has debug messages
		 * @return boolean
		 */
		function has_debug_messages() 			{ return count($this->get_debug_messages())>0; }
		
		/**
		 * Checks whether user has error messages
		 * @return boolean
		 */
		function has_user_error_messages() 		{ return count($this->get_user_error_messages())>0; }

                /**
                 * Adds a message
                 * @param string $type
                 * @param string $message 
                 */
		private function add_message($type,$message) 	{

			$type_block = array(MESSAGE::MSG_ERROR=>	"ERROR:         ",
							    MESSAGE::MSG_NOTIFY=>	"NOTIFY:        ",
							    MESSAGE::MSG_WARNING=>	"WARNING:       ",
							    MESSAGE::MSG_DEBUG=>	"DEBUG:         ",
							    MESSAGE::MSG_USER_ERROR=>	"USER ERROR:    ");

			$this->_log_report .= get_value($type_block,$type)." ".$message."\r\n";
			$this->_messages[$type][] = $message;
		}

		/**
		 * Adds messages by type
		 * @param string $type
		 * @param array $messages 
		 */
		private function add_messages_by_type($type,$messages) {
			foreach($messages as $message)
				$this->add_message($type,$message);
		}

		/**
		 * Adds messages
		 * @param array $messages 
		 */
		function add_messages($messages) {
			foreach($messages as $type=>$messages)
				$this->add_messages_by_type($type,$messages);
		}

		/**
		 * Returns messages
		 * @return array 
		 */
		function get_messages() { return $this->_messages; }

		/**
		 * initialize messages
		 */
		function initialize_messages() {
			$this->_messages = array();
			$this->_log_report = "";
		}

		/**
		 * Returns message log
		 * @return string 
		 */
		function get_message_log() { return $this->_log_report; }
		
		/**
		 * inherit user messages
		 * @param MESSAGE $obj 
		 */
		public function inherit_user_messages(MESSAGE $obj) {
			if($obj->has_user_error_messages())
				$this->add_user_error_messages($obj->get_user_error_messages());
		}

		/**
		 * inherit messages
		 * @param MESSAGE $obj 
		 */
		public function inherit_messages(MESSAGE $obj) {
			if($obj->has_error_messages())
				$this->add_error_messages($obj->get_error_messages());
				
			if($obj->has_notify_messages())
				$this->add_notify_messages($obj->get_notify_messages());
				
			if($obj->has_warning_messages())
				$this->add_warning_messages($obj->get_warning_messages());
				
			if($obj->has_debug_messages())
				$this->add_debug_messages($obj->get_debug_messages());
			
			if($obj->has_user_error_messages())
				$this->add_user_error_messages($obj->get_user_error_messages());			
		}

	}
