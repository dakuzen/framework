<?php
	/**
        * Class HTTP_ADAPTER  
        *
        * @package Library
        * @subpackage Transport
        */
	class HTTP_ADAPTER {
		
                /**
                 * The httpVersion
                 * @var string $httpVersion
                 */
		public $httpVersion 	= "HTTP/1.1";
                
                /**
                 * The uri
                 * @var string $uri 
                 */
		public $uri		= "";
                
                /**
                 * The verb
                 * @var string $verb 
                 */
		public $verb 		= "GET";
                
                /**
                 * The headers
                 * @var array $headers 
                 */
		public $headers 	= array();
                
                /**
                 * The body 
                 * @var string $body  
                 */
		public $body		= "";
                
                /**
                 * The requestTimeout
                 * @var integer $requestTimeout
                 */
		public $requestTimeout 	= 100;
                
                /**
                 * The proxy
                 * @var string $proxy 
                 */
		public $proxy 		= null;
                
                /**
                 * The code
                 * @var integer $code 
                 */
		public $code 		= 0;
                
                /**
                 * The username
                 * @var string $username
                 */
		public $username 	= "";
                
                /**
                 * The password
                 * @var string $password
                 */
		public $password	= "";
                
                /**
                 * The listeners
                 * @var array $listeners
                 */
		public $_listeners 	= array();
                
                /**
                 * The cookies
                 * @var array $cookies 
                 */
		public $cookies 	= array();
		
                /**
                 * set Listeners
                 * @param array $listeners 
                 */
		public function setListeners(&$listeners) {
			$this->_listeners &= $listeners;
		}
		
                /**
                 * send Request
                 */
		public function sendRequest() 		{}
                
                /**
                 * write Request
                 * @param string $file 
                 */
		public function writeRequest($file) 	{}

                /**
                 * parse Response Code
                 * @param string $line
                 * @return array
                 * @throws HTTP_EXCEPTION 
                 */
		protected function parseResponseCode($line) {
			if (sscanf($line, "HTTP/%s %s", $http_version, $returncode) != 2) {
				throw new HTTP_EXCEPTION("Malformed response");
			} else {
				return array("code" => intval($returncode), "httpVersion" => $http_version);
			}
		}

                /**
                 * processHeader
                 * @param string $header
                 * @return 
                 */
		protected function processHeader($header) {
			
			if (strpos($header, ":") === false) 
				return;
	
			list($headername, $headervalue) = explode(":", $header, 2);
			if (strstr($headername,'-')) {
				list($p1,$p2) = explode('-',$headername);
				$headername  = ucfirst(strtolower($p1)).'-'.ucfirst(strtolower($p2));
			} else 
				$headername  = ucfirst($headername);
			
			$headervalue = ltrim($headervalue);
	
			if ('set-cookie' != $headername) {
				if (isset($this->headers[$headername])) {
					$this->headers[$headername] .= ',' . $headervalue;
				} else {
					
					$this->headers[$headername]  = $headervalue;
				}
			} else 
				$this->cookies[] = $this->parseCookie($headervalue);
		}

                /**
                 * parse Cookie
                 * @param string $headervalue
                 * @return null 
                 */
		protected function parseCookie($headervalue) {
			
			$cookie = array("expires" => null,
					"domain"  => null,
					"path"    => null,
					"secure"  => false);

			if (!strpos($headervalue, ';')) {
				$pos = strpos($headervalue, '=');
				$cookie['name']  = trim(substr($headervalue, 0, $pos));
				$cookie['value'] = trim(substr($headervalue, $pos + 1));				
			} else {
				$elements = explode(';', $headervalue);
				$pos = strpos($elements[0], '=');
				$cookie['name']  = trim(substr($elements[0], 0, $pos));
				$cookie['value'] = trim(substr($elements[0], $pos + 1));

				for ($i = 1; $i < count($elements); $i++) {
					
					if (false === strpos($elements[$i], '=')) {
						$elName  = trim($elements[$i]);
						$elValue = null;
					} else
						list ($elName, $elValue) = array_map('trim', explode('=', $elements[$i]));
					
					$elName = strtolower($elName);

					if ($elName == 'secure') 
						$cookie['secure'] = true;
					
					elseif ($elName == 'expires')
						$cookie['expires'] = str_replace('"', '', $elValue);
					
					elseif ($elName == 'path' || $elName == 'domain')
						$cookie[$elName] = urldecode($elValue);
					
					else
						$cookie[$elName] = $elValue;
				}
			}
			
			return $cookie;
		}
		
                /**
                 * Set body
                 * @param string $body 
                 */
		public function set_body($body) 			{ $this->body = $body; }
                
                /**
                 * Set verb
                 * @param string $verb 
                 */
		public function set_verb($verb) 			{ $this->verb = $verb; }
                
                /**
                 * Set headers
                 * @param array $headers 
                 */
		public function set_headers($headers) 			{ $this->headers = $headers; }
		
                /**
                 * Set the username
                 * @param string $username 
                 */
		public function set_username($username) 		{ $this->username = $username; }
                
                /**
                 * Set the password
                 * @param string $password 
                 */
		public function set_password($password) 		{ $this->password = $password; }
                
                /**
                 * Set the requestTimeout
                 * @param string $password 
                 */
                public function set_request_timeout($request_timeout)                 { $this->requestTimeout = $request_timeout; return $this; }

                /**
                 * Return username
                 * @return string 
                 */
		public function get_username() 				{ return $this->username; }
                
                /**
                 * Return password
                 * @return string 
                 */
		public function get_password() 				{ return $this->password; }
                
                /**
                 * Return uri
                 * @return string 
                 */
                public function get_uri()                               { return $this->uri; }

                 /**
                 * Return body
                 * @return string 
                 */
                public function get_body()                               { return $this->body; }
				
                /**
                 * add header
                 * @param string $header 
                 */
		public function add_header($header) 			{ $this->headers[] = $header; }
		
		/**
                 * Returns formatted header array
                 * @return array
                 */
		public function get_formatted_header_array() { 
			$headers = array();
			
			foreach($this->headers as $key=>$header)
				$headers[] = ($key ? $key.":" : "").$header;
				
			return $headers;
		}

                /**
                 * Returns formatted header
                 * @return string 
                 */
		public function get_formatted_header() {
			return implode("\r\n",$this->get_formatted_header_array())."\r\n";
		}		
		
	}
