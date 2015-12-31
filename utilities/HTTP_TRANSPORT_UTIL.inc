<?
	require_once(DIR_FRAMEWORK."libraries/transport/__prepend.inc");

    /**
    * Class HTTP_TRANSPORT_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */

    /*

		try {

			$response = HTTP_TRANSPORT_UTIL::create()->post($url,$body,$headers);

			$json = $response->json();


		} catch(HTTP_TRANSPORT_EXCEPTION $e) {}
		} catch(Exception $e) {}

    */
	
	class HTTP_TRANSPORT_UTIL {

		/**
		 * The redirect limit
		 * @var integer $redirect_limit 
		 */
		protected $_redirect_limit 	= 3;

		/**
		 * The http request
		 * @var string $http_request 
		 */
		protected $_http_request 	= null;

		/**
		 * The http response
		 * @var string $http_response 
		 */
		protected $_http_response 	= null;

		/**
		 * The redirected url 
		 * @var string $redirected_url 
		 */
		protected $_redirected_url	= null;

		protected $_debug = false;

		/**
		 * The error message
		 * @var string $error_message 
		 */
		protected $_error_message	= "";
		
		const TRANSPORT_TYPE_CURL 		= "curl";
		const TRANSPORT_TYPE_HTTP 		= "http";
		const TRANSPORT_TYPE_FOPEN 		= "fopen";
		const TRANSPORT_TYPE_PHPSOCKET 	= "phpsocket";

		/**
		 * Inializes data for the transport util
		 *
		 * @param string $transport_type string transport type
		 */
	 
		function __construct($options=array("type"=>self::TRANSPORT_TYPE_CURL,"request_timeout"=>100)) {

			if(is_string($options))
				$options=array("type"=>$options);

			$this->_http_request = new HTTP_REQUEST("",value($options,"type"));

			if(array_key_exists("request_timeout",$options))
				$this->_http_request->getAdapter()->set_request_timeout(value($options,"request_timeout"));

			$this->_http_response 	= new HTTP_RESPONSE();
		}

		public static function create($options=array("type"=>self::TRANSPORT_TYPE_CURL,"request_timeout"=>100)) {
			return new HTTP_TRANSPORT_UTIL($options);
		}

		/**
		 * Preforms a "post" to the server
		 *
		 * @param string $url string url of the server
		 * @param string $body string body 
		 * @param array $headers array headers
                 * @param string $username
                 * @param string $password
		 */
		 
		function post($url,$body="",$headers=array(),$username="",$password="") {
			return $this->send($url,$body,$headers,"POST",$username,$password);
		}

		/**
		 * Preforms a "get" from the server
		 *
		 * @param string $url string url of the server
		 * @param string $body string body 
		 * @param array $headers array headers
                 * @param string $username
                 * @param string $password
		 */
		 
		function get($url,$parameters=array(),$headers=array(),$username="",$password="") {

			if($parameters)
				$url = URL_UTIL::url($url,$parameters);

			return $this->send($url,"",$headers,"GET",$username,$password);
		}

		/**
		 * Preforms a "put" from the server
		 *
		 * @param string $url string url of the server
		 * @param string $body string body 
		 * @param array $headers array headers
                 * @param string $username
                 * @param string $password
		 */
		 
		function put($url,$body="",$headers=array(),$username="",$password="") {
			return $this->send($url,$body,$headers,"PUT",$username,$password);
		}


		/**
		 * Preforms a "delete" from the server
		 *
		 * @param string $url string url of the server
		 * @param string $body string body 
		 * @param array $headers array headers
                 * @param string $username
                 * @param string $password
		 */
		 
		function delete($url,$body="",$headers=array(),$username="",$password="") {
			return $this->send($url,$body,$headers,"DELETE",$username,$password);
		}

		/**
		 * Sends to the server
		 *
		 * @param string $url string url of the server
		 * @param string $body string body 
		 * @param array $headers array headers
		 * @param string $method string method
         * @param string $username
         * @param string $password
		 */
		 
		function send($url,$body="",$headers=array(),$method="",$username="",$password="") {
			
			if($this->_debug)
				p("URL:".$url,"Body:",$body,"Headers:",$headers,"Method: ".$method,"Username: ".$username,"Password: ".$password);

			$has_success = false;

			if($method=="GET") {
				$url .= $body ? "?".ARRAY_UTIL::get_imploded($body,"&","","=") : "";
				$body = "";
			}
			
			$this->_url = trim($url);

			try {
				
				$redirect_count = 0;
				
				do {	
					$this->_http_request
						->set_url($this->_url)
						->set_username($username)
						->set_password($password)
						->set_body($body);

					if($method)
						$this->_http_request->set_verb($method);
					
					$this->_http_request->setHeaders($headers);
			
					$this->_http_response = $this->_http_request->sendRequest();

					if($this->_debug)
						p("Response:",$this->_http_response);

					$is_redirect = $this->_http_response->is_code_redirect();					
					
					if($is_redirect) {
					
						foreach($this->_http_response->get_header()->get_fields() as $field=>$value) {

							if(strtolower($field)=="location") {
							
								$locations = explode(",",$value);
								
								$location = array_shift($locations);
								
								foreach($locations as $sub_location)
									$location = URL_UTIL::get_appended_resolved_url($location,$sub_location);
								
								if(preg_match("/^https?/",$value))
									$this->_url = $location;
								else {
								
									$parsed_url = parse_url($url);

									$path = strpos($location,"/")===0 ? $location : dirname(get_value($parsed_url,"path")).$location;

									$this->_url = get_value($parsed_url,"scheme")."://".get_value($parsed_url,"host").$path;
								}
							}
						}
						
						if($redirect_count>=$this->_redirect_limit) 
							throw new Exception("Exceeded redirect limit");
						
						$redirect_count++;
					
					} elseif(!$this->_http_response->is_code_success())
						throw new HTTP_TRANSPORT_EXCEPTION(value(HTTP_UTIL::codes(),$this->_http_response->get_code()),$this->_http_response->get_code(),$this->_http_request,$this->_http_response);
					
				} while($is_redirect && $redirect_count<$this->_redirect_limit);
				
			} catch(Exception $exception) {
				$this->_error_message = $exception->getMessage();
				throw $exception;
			}
			
			return $this->get_http_response();
		}
		
        /**
         * Save
         * @param string $file
         * @param string $url
         * @return boolean
         * @throws Exception 
         */
		function save($file,$url) {
		
			if(!ini_get("allow_url_fopen"))
				throw new Exception("The allow_url_fopen ini setting is not enabled");
			
			$has_success = false;
			
			$this->_url = $url;
			
			$redirect_count = 0;
			
			do {	
				$this->_http_request = new HTTP_REQUEST("",self::TRANSPORT_TYPE_FOPEN);
				$this->_http_request->set_url($this->_url);
				
				$this->_http_response = $this->_http_request->writeRequest($file);
				
				$is_redirect = $this->_http_response->is_code_redirect();
				
				if($is_redirect) {
				
					foreach($this->_http_response->get_header()->get_fields() as $field=>$value) {

						if(strtolower($field)=="location") {
						
							$locations = explode(",",$value);
							
							$location = array_shift($locations);
							
							foreach($locations as $sub_location)
								$location = URL_UTIL::get_appended_resolved_url($location,$sub_location);
							
							if(preg_match("/^https?/",$value))
								$this->_url = $location;
							else {
							
								$parsed_url = parse_url($url);

								$path = strpos($location,"/")===0 ? $location : dirname(get_value($parsed_url,"path")).$location;

								$this->_url = get_value($parsed_url,"scheme")."://".get_value($parsed_url,"host").$path;
							}
						}
					}
					
					if($redirect_count>=$this->_redirect_limit) 
						throw new Exception("Exceeded redirect limit");
					
					$redirect_count++;
				}
				
			} while($is_redirect && $redirect_count<$this->_redirect_limit);
			
			if(!is_file($file))
				throw new Exception("Failed to validate saved file");

			return true;
		}		

		/**
		 * Gets the body
		 *
		 * @return string 
		 */
		 
		function get_body() {
			return $this->_http_response ? $this->_http_response->get_body() : "";
		}

		/**
		 * Returns an error message
		 *
		 * @return string 
		 */
		 
		function get_error_message() 	{ return $this->_error_message; }

		/**
		 * Gets the url
		 *
		 * @return string
		 */
		 
		function get_http_response() 	{ return $this->_http_response; }
		
                /**
                 * Returns the http request
                 * @return string 
                 */
		function get_http_request() 	{ return $this->_http_request; }
		
		/**
		 * Gets the url
		 *
		 * @return string
		 */
		 
		function get_url() 		{ return $this->_url; }

		function debug() 		{ $this->_debug = true; return $this; }
		
		/**
		 * Gets the path
		 *
		 * @return string
		 */
		 
		function get_path() { 
			preg_match("/^[a-z]+:\/\/[a-z0-9-\.]+(.*)/i",$this->get_url(),$matches);
			return get_value($matches,1);
		}

		function file($file) {
			if(version_compare(phpversion(), '5.5.0')>=0)
				return new CurlFile($file);
			return "@".$file;						
		}
	}

	class HTTP_TRANSPORT_EXCEPTION extends Exception {

		protected $_http_request 	= null;
		protected $_http_response 	= null;

		public function __construct($message,$code,$http_request,$http_response) {
			parent::__construct($message,$code);
			$this->_http_request = $http_request;
			$this->_http_response = $http_response;
		}

		public function getRequest() 			{ return $this->_http_request; }
		public function getResponse() 			{ return $this->_http_response; }
		public function setMessage($message) 	{ $this->message = $message; return $this; }
	}