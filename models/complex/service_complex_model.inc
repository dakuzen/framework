<?php
        /**
        * Class BASE_CMODEL_SERVICE
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_SERVICE extends MODEL {
		
                /**
                * The service host
                * @var string $service_host 
                */
		protected $_service_host	= "";
                
                /**
                 * The success
                 * @var boolean $success 
                 */
		protected $_success		= false;
                
                /**
                 * The controller
                 * @var string $controller
                 */
		protected $_controller		= "";
                
                /**
                 * The message
                 * @var string $message 
                 */
		protected $_message		= "";
                
                /**
                 * The code
                 * @var string $code 
                 */
		protected $_code		= "";
                
                /**
                 * The response
                 * @var string $response 
                 */
		protected $_response		= "";
		
                /**
                 * Constructor
                 * @param string $service_host
                 * @param string $controller 
                 */
		function __construct($service_host,$controller="service") {
			$this->_service_host	= $service_host;
			$this->_controller	= $controller;
		}
		
                /**
                 * Returns
                 * @param string $method
                 * @param array $vars
                 * @return boolean 
                 */
		function get($method,$vars=array()) {

			$http_transport_util = new HTTP_TRANSPORT_UTIL("fopen");
			
			$protocol = URL_UTIL::has_protocol($this->_service_host) ? "" : "http://";
			
			$url = $protocol.$this->_service_host."/".$this->_controller."/".$method."/";
			
			$this->_success = $http_transport_util->post($url,$vars);
			
			$this->_message 	= "";
			$this->_code 		= 0;
			$this->_response	= "";
			
			if($this->_success) {
				$body = $http_transport_util->get_body();
				
				if(JSON_UTIL::is_encoded($body)) {
					
					$payload = json_decode($body,true);
					
					$this->_success  = get_value($payload,"success");
					$this->_message  = get_value($payload,"message");
					$this->_code 	 = get_value($payload,"code");
					$this->_response = get_value($payload,"response");
					
				} else {
					$this->_message = "Failed to decode payload";
					$this->_success = false;
				}
			} else {
				$this->_message = $http_transport_util->get_error_message();
			}
			
			return $this->_success;
		}
		
                /**
                 * Checks whether is success
                 * @return booelan 
                 */
		function has_success() 	{ return $this->_success==1; }
                
                /**
                 * Returns the message
                 * @return string 
                 */
		function get_message() 	{ return $this->_message; }
                
                /**
                 * Returns the code
                 * @return string 
                 */
		function get_code() 	{ return $this->_code; }
                
                /**
                 * Returns the response
                 * @return string 
                 */
		function get_response() { return $this->_response; }
		
	}