<?php

	/**
	 * Class BASE_CMODEL_RESPONSE
	 *
	 * @package Framework
	 * @subpackage Cmodels
	 */
	class BASE_CMODEL_RESPONSE extends CMODEL {

		/**
		 * The message
		 * @var array $messages 
		 */
		protected $_messages = array();

		/**
		 * The errors
		 * @var array $errors
		 */
		protected $_errors = array();

		/**
		 * The debugs
		 * @var array $debugs
		 */
		protected $_debugs = array();		

		/**
		 * The warnings
		 * @var array $warnings 
		 */
		protected $_warnings = array();

		/**
		 * The notifies
		 * @var array $notifies 
		 */
		protected $_notifies = array();

		/**
		 * The data
		 * @var array $data 
		 */
		protected $_data = array();

		protected $_redirect = null;		

		/**
		 * The has success
		 * @var boolean $has_success 
		 */
		protected $_has_success = false;

		/**
		 * Adds a message
		 * @param string $message 
		 */
		function add_message($message,$encode=true) {
			$this->_messages[] = $encode ? XSS_UTIL::encode($message) : $message;
		}

		/**
		 * Adds messages
		 * @param array $messages
		 */
		function add_messages($messages) {
			$this->_messages = $messages;
		}

		/**
		 * Adds an error
		 * @param string $message 
		 */
		function add_error($message,$encode=true) {

			if(is_a($message,"Exception"))
				$message = $message->getMessage();

			if(!in_array($message,$this->_errors))
				$this->_errors[] = $encode ? XSS_UTIL::encode($message) : $message;
		}

		function error($message,$encode=true) { $this->add_error($message,$encode); }

		/**
		 * Adds errors
		 * @param array $messages
		 */
		function add_errors($messages) {
			$this->_errors = array_merge($this->_errors, $messages);
		}

		/**
		 * Adds an debug
		 * @param string $message 
		 */
		function add_debug($message,$encode=true) {
			$this->_debugs[] = $encode ? XSS_UTIL::encode($message) : $message;
		}

		/**
		 * Adds a warning
		 * @param string $message 
		 */
		function add_warning($message,$encode=true) {
			$this->_warnings[] = $encode ? XSS_UTIL::encode($message) : $message;
		}

		/**
		 * Adds warnings
		 * @param string $messages 
		 */
		function add_warnings($messages) {
			$this->_warnings = array_merge($this->_warnings, $messages);
		}

		/**
		 * Adds notify
		 * @param string $message 
		 */
		function add_notify($message,$encode=true) {
			$this->_notifies[] = $encode ? XSS_UTIL::encode($message) : $message;
		}

		/**
		 * Adds notifies
		 * @param string $messages 
		 */
		function add_notifies($messages) {
			$this->_notifies = array_merge($this->_notifies, $messages);
		}

		/**
		 * sets data
		 * @param string $name
		 * @param string $value
		 */
		function add_data($name, $value) {
			$this->_data[$name] = $value; return $this;
		}

		function set_data($value) {
			$this->_data = $value; return $this;
		}
		
		function get_data($name) {
			return get_value($this->_data,$name);
		}		

		function data($name, $value) { return $this->add_data($name,$value); }

		/**
		 * Success
		 * @param boolean $success 
		 */
		function success($success=true) {
			$this->_has_success = $success==true;
			return $this;
		}

		/**
		 * checks whether has error
		 * @return boolean 
		 */
		function has_error() {
			return !is_empty($this->_errors);
		}

		/**
		 * checks whether has message
		 * @return boolean 
		 */
		function has_message() {
			return !is_empty($this->_messages);
		}

		/**
		 * checks whether has warning
		 * @return boolean 
		 */
		function has_warning() {
			return !is_empty($this->_warnings);
		}

		/**
		 * checks whether has notify
		 * @return boolean 
		 */
		function has_notify() {
			return !is_empty($this->_notifies);
		}

		/**
		 * checks whether has success
		 * @return boolean 
		 */
		function has_success() {
			return $this->_has_success;
		}

		function redirect($redirect="") {

			if(!func_get_args())
				return $this->_redirect;

			$this->_redirect = $redirect; return $this;
		}


		/**
		 * Returns errors
		 * @return array 
		 */
		function get_errors() {
			return $this->_errors;
		}

		/**
		 * Returns messages
		 * @return array 
		 */
		function get_messages() {
			return $this->_messages;
		}

		/**
		 * Clear errors 
		 */
		function clear_errors() {
			$this->_errors = array();
		}

		/**
		 * Returns the response
		 * @return array 
		 */
		function get_response() {

			$response["data"] 			= array();
			$response["has_success"] 	= $this->_has_success;
			
			if($this->has_warning())
				$response["has_warnings"] = $this->has_warning();

			if($this->has_error())
				$response["has_errors"] = $this->has_error();

			if($this->has_message())
				$response["has_messages"] = $this->has_message();
			
			$errors = $messages = $warnings = $notifies = $debugs = array();

			foreach($this->_messages as $message)
				$messages[] = $message;

			foreach ($this->_errors as $error)
				$errors[] = $error;

			foreach ($this->_warnings as $warning)
				$warnings[] = $warning;

			foreach ($this->_notifies as $notify)
				$notifies[] = $notify;

			foreach ($this->_debugs as $debug)
				$debugs[] = $debug;

			if($messages) 	$response["messages"] = $messages;
			if($errors) 	$response["errors"] = $errors;
			if($warnings) 	$response["warnings"] = $warnings;
			if($notifies) 	$response["notifies"] = $notifies;
			if($debugs) 	$response["debugs"] = $debugs;

			if($this->_redirect)
				$response["redirect"] = $this->_redirect;

			foreach ($this->_data as $name => $value)
				$response["data"][$name] = $value;

			return $response;
		}

		/**
		 * Renders
		 * @param boolean $die 
		 */
		function render($die=false) {

			echo $this->render_content_type()->get_json();

			if($die)
				die;
		}

		function render_content_type() {
			header("Content-type: application/json");
			return $this;
		}

		/**
		 * Renders jsonp
		 * @param string $callback
		 * @param boolean $die 
		 */
		function render_jsonp($callback = null, $die = false) {

			$callback = $callback ? $callback : REGISTRY::get_instance()->get_request_parm("callback");

			header("Content-type: application/json");
			echo JSON_UTIL::encode_jsonp($callback, $this->get_response());

			if ($die)
				die;
		}

		/**
		 * Returns the json response
		 * @return string 
		 */
		function get_json() {
			return JSON_UTIL::encode($this->get_response());
		}

		/**
		 * Inherits messages
		 * @param object $message 
		 */
		function inherit_messages($message) {

			foreach ($message->get_error_messages() as $message)
				$this->add_error($message);
		}

	}