<?php
        /**
        * Class BASE_CMODEL_PAYPAL_INSTANT_PAYMENT_NOTIFICATION
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_PAYPAL_INSTANT_PAYMENT_NOTIFICATION {
		
                /**
                 * The request
                 * @var array $request 
                 */
		protected $_request 		= array();
                
                /**
                 * The paypal url
                 * @var string $paypal_url 
                 */
		protected $_paypal_url 		= "";
                
                /**
                 * The http response
                 * @var string $http_response 
                 */
		protected $_http_response 	= null;
		
                /**
                 * Constructor
                 * @param array $request 
                 */
		function __construct($request) {
		
			$this->_request = $request;
			
			$this->_paypal_url = "https://www.sandbox.paypal.com/cgi-bin/webscr";

			if(SYSTEM_MANAGER::is_system_mode_production())
				$this->_paypal_url = "https://www.paypal.com/cgi-bin/webscr";
		}
                
                /**
                 * Checks whether is verified
                 * @return boolean 
                 */
		function is_verified() {		
			
			$this->_request["cmd"] = "_notify-validate"; 

			$headers["Content-Type"] = "application/x-www-form-urlencoded";

			$has_success = false;

			$url = $this->_paypal_url;

			try {
				$http_transport_util = new HTTP_TRANSPORT_UTIL();
				$has_success = $http_transport_util->post($url,$this->_request,$headers);

				$this->_http_response = $http_transport_util->get_http_response();
				
				$has_success = preg_match("/VERIFIED/",$this->_http_response->get_body());

			} catch(Exception $e) {

			}
			
			return $has_success;

		}
		
                /**
                 * Returns the http response
                 * @return string 
                 */
		function get_http_response() { return $this->_http_response; }
	}