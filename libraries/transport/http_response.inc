<?php
	/**
	 * Class HTTP_RESPONSE  
	 *
	 * @package Library
	 * @subpackage Transport
	 */
	class HTTP_RESPONSE {
	   
		/**
		 * The code
		 * @var integer $code 
		 */
		public $_code 		= 100;
		
		/**
		 * The header
		 * @var string $header 
		 */
		public $_header		= null;
		
		/**
		 * Request header
		 * @var string $equest_header 
		 */
		public $_request_header	= null;
		
		/**
		 * The cookies
		 * @var array $cookies
		 */
		public $_cookies	= array();
		
		/**
		 * The body
		 * @var string $body 
		 */
		public $_body 		= '';
		
		/**
		 * The http version
		 * @var string $http_version 
		 */
		public $_http_version	= '';
		
		/**
		 * The path
		 * @var string $path 
		 */
		public $_path		= '';
		
		/**
		 * SHORT DESCRIPTION
		 * @var array $SHORT_DESCRIPTION 
		 */
		static $SHORT_DESCRIPTION = array(	"301"=>"Moved",
											"302"=>"Found",
											"303"=>"Method",
											"304"=>"Not Modified",		
											"400"=>"Bad request",
											"401"=>"Unauthorized",
											"402"=>"PaymentRequired",
											"403"=>"Forbidden",
											"404"=>"Not found",
											"500"=>"Internal Server Error",
											"501"=>"Not implemented",
											"502"=>"Service temporarily overloaded",
											"503"=>"Gateway timeout");

		/**
		 * Constructor
		 * @param integer $code
		 * @param string $body
		 * @param string $header
		 * @param array $cookies
		 * @param string $http_version
		 * @param string $uri 
		 */
		public function __construct($code=0, $body="", $header=null, $cookies=array(), $http_version="",$uri=null) {			
			$this->_code    	= (int)$code;
			$this->_body    	= $body;
			$this->_header 		= $header;
			$this->_cookies 	= $cookies;
			$this->_http_version 	= $http_version;
			$this->_uri		= $uri;
		}

		/**
		 * To string
		 * @return string 
		 */
		public function __toString() 		{ return $this->_body; }
		
		/**
		 * Returns the body
		 * @return string 
		 */
		public function get_body() 		{ return $this->_body; }
		public function body() 			{ return $this->_body; }
		
		/**
		 * Returns the json body
		 * @return string 
		 */
		public function get_json_body() 	{ return JSON_UTIL::decode($this->_body); }

		public function json() 	{ return $this->get_json_body(); }
		
		/**
		 * Returns the header
		 * @return string 
		 */
		public function get_header() 		{ return $this->_header; }
		
		/**
		 * Returns the cookies
		 * @return array 
		 */
		public function get_cookies() 		{ return $this->_cookies; }
		
		/**
		 * Returns the code
		 * @return string 
		 */
		public function get_code() 		{ return (int)$this->_code; }
		
		/**
		 * Returns the path
		 * @return string 
		 */
		public function get_path() 		{ return $this->_path; }
		
		/**
		 * Returns the content type
		 * @return string 
		 */
		public function get_content_type() 	{   

			p($this->_header); return $this->_header->get_content_type(); }
		
		/**
		 * Set request header
		 * @param string $request_header
		 */
		public function set_request_header($request_header) 	{ $this->_request_header = $request_header; }

		/**
		 * Get request header
		 * @param string $request_header
		 */
		public function get_request_header() 	{ return $this->_request_header; }		
		
		/**
		 * Checks whether the code is success
		 * @return boolean 
		 */
		public function is_code_success() 	{ return preg_match("/^2[\d]{2}/",$this->get_code()); }
		
		/**
		 * Checks whether the code is redirect
		 * @return boolean 
		 */
		public function is_code_redirect() 	{ return preg_match("/^3[\d]{2}/",$this->get_code()); }
		
		/**
		 * Checks whether the code is client error
		 * @return boolean 
		 */
		public function is_code_client_error() 	{ return preg_match("/^4[\d]{2}/",$this->get_code()); }
		
		/**
		 * Checks whether the code is server error
		 * @return boolean 
		 */
		public function is_code_server_error() 	{ return preg_match("/^5[\d]{2}/",$this->get_code()); }
		

		public function success() 	{ return $this->is_code_success(); }
		/**
		 * Set the body
		 * @param string $body 
		 */
		public function set_body($body) 	{ $this->_body = $body; }
		
		/**
		 * Returns the content extension
		 * @return string|null 
		 */
		public function get_content_extension() { 
			$content_type = $this->_header->get_content_type(); 
			
			if(preg_match("/image\/(.*)$/",$content_type,$matches)) {
				$extension = get_value($matches,1);
				
				if($extension=="jpeg")
					return "jpg";
				
				return $extension;
			}
			
			return null;
		}
		
		/**
		 * Returns the charset
		 * @return string 
		 */
		public function get_charset() { 
			$content_type = $this->_header->get_content_type(); 
			
			if(preg_match("/charset\s*=\s*(.*)/",$content_type,$matches))
				return strtolower(get_value($matches,1));
			
			return null;
		}		
		
		/**
		 * Returns the code descriptions
		 * @return string 
		 */
		public function get_code_description() { return $this->get_code().": ".$this->get_code_short_description(); }
		
		/**
		 * Returns the code short description
		 * @return string 
		 */
		public function get_code_short_description() {
			return get_value(self::$SHORT_DESCRIPTION,$this->_code);
		}
	}
