<?
   /**
    * HTTP_REQUEST class file
    *
    * @package Library
    * @subpackage Transport
    */

	require_once(DIR_FRAMEWORK."libraries/transport/http_adapter.inc");

   /**
    * Class HTTP_REQUEST  
    *
    * @package Library
    * @subpackage Transport
    */
	class HTTP_REQUEST  {
		
        /**
         * the adapter
         * @var string $adapter 
         */
		protected $_adapter	= null;


		
        /**
         * The httpVersion
         * @var string $httpVersion
         */
		public $httpVersion 	= "HTTP/1.1";
             
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
		 * The protocol
		 * @var string $protocol
		 */
		public $protocol	= "http";

		/**
		 * The cookies
		 * @var array $cookies 
		 */
		public $cookies 	= array();
		
		

		/**
         * the adapter
         * @var string $adapter 
         */
		protected $_http_header	= null;
        
        /**
         * The listeners 
         * @var array $listeners 
         */
		protected $_listeners 	= array();
        
        /**
         * The type
         * @var string $type 
         */
		protected $_type	= null;
		


        /**
         * sets up the adapter
         * @param string $url
         * @param string $type 
         */
		public function __construct($url=null,$type=null)  {
			
			$this->_type = $type;
			$this->_http_header = new HTTP_HEADER();
			
			if(extension_loaded('curl') && ($this->_type=="curl" || !$this->_type)) {
				require_once(DIR_FRAMEWORK."libraries/transport/adapter/http_adapter_curl.inc");
				$this->_adapter = new HTTP_ADAPTER_CURL();			
				
			} elseif($this->_type=="fopen" || !$this->_type) {				
				require_once(DIR_FRAMEWORK."libraries/transport/adapter/http_adapter_fopen.inc");
				$this->_adapter = new HTTP_ADAPTER_FOPEN();	
			}
			
			if(!$this->_adapter)
				$this->_adapter->setListeners($this->_listeners);

			if($url) 
				$this->url = $url;
		}


		/**
		 * Set the username
		 * @param string $username 
		 */
		public function set_username($username) 		{ $this->username = $username; return $this; }


		/**
		 * Set the requestTimeout
		 * @param string $password 
		 */
		public function set_request_timeout($request_timeout)                 { $this->requestTimeout = $request_timeout; return $this; }

		/**
		 * Return username
		 * @return string 
		 */
		public function get_request_timeout() 				{ return $this->requestTimeout; }

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
		 * Return body
		 * @return string 
		 */
		public function get_body()                               { return $this->body; }

		 /**
		 * Return proxy
		 * @return string 
		 */
		public function get_proxy()                               { return $this->proxy; }

		 /**
		 * Return url
		 * @return string 
		 */
		public function get_url()                               { return $this->url; }		


		 /**
		 * Return url
		 * @return string 
		 */
		public function get_verb()                               { return $this->verb; }	


		/**
		 * Return url
		 * @return string 
		 */
		public function get_header()                               { return $this->_http_header; }	
	
		/**
		 * Return protocol
		 * @return string 
		 */
		public function get_protocol()                               { return $this->protocol; }	

        /**
         * Set body
         * @param string $value 
         */
		function set_body($value) {

			$this->body = $value;
			
			if (is_array($value)) {				
				if ($this->_verb == 'GET') 
					$this->_verb = 'POST';
			}

			return $this;
		}
		
        /**
         * Returns transport type
         * @return string 
         */
		public function get_transport_type() { return $this->_type; }
		

        /**
         * set Header
         * @param string $header
         * @param string $value 
         */
		public function set_header($header, $value) {
			$this->get_header()->set($header,$value);
			return $this; 
		}
		
        /**
         * add header
         * @param string $header
         * @param string $value 
         */
		public function add_header($header, $value) {
			$this->get_header()->set($header,$value);
			return $this; 
		}		
		
        /**
         * set Headers
         * @param string $headers 
         */
		public function set_headers($headers) { 
			$this->get_header()->sets($headers);
			return $this; 
		}

        
        /**
         * set password
         * @param string $password 
         */
		public function set_password($password) 	{ $this->password = $password; return $this; }

        /**
         * attach
         * @param HTTP_LISTENER $listener
         * @return boolean 
         */
		public function attach(HTTP_LISTENER &$listener) {
			$this->_listeners[$listener->getId()] =& $listener;
			return true;
		}
                
        /**
         * detach
         * @param HTTP_LISTENER $listener
         * @return boolean 
         */
		public function detach(HTTP_LISTENER &$listener) {
			if (isset($this->_listeners[$listener->getId()]))
				$this->_listeners[$listener->getId()] = null;

			return true;
		}

        /**
         * notify
         * @param string $event
         * @param string $data 
         */
		protected function _notify($event, $data = null) {
			if (!empty($this->_listeners)) 
				foreach (array_keys($this->_listeners) as $id) 
					$this->_listeners[$id]->update($this, $event, $data);
		}
		
        /**
         * Set url
         * @param string $url 
         */
		public function set_url($url) 		{ $this->url = $url; return $this; }
                
        /**
         * Set verb
         * @param string $verb 
         */
		public function set_verb($verb) 	{ $this->verb = $verb; return $this; }
		
        /**
         * Return Adapter Name
         * @return string 
         */
		public function getAdapterName() 	{ return get_class($this->_adapter); }
		
        /**
         * Return the addapter
         * @return string 
         */
		public function getAdapter() 		{ return $this->_adapter; }
     

		/*
		public function get_request() {
			$request = $this->_adapter->get_formatted_header();

			if($body=$this->_adapter->get_body()) {
				$request .= "\n\n";
			
				if(is_array($body))
					$body = http_build_query($body);

				$request .= $body;
			}

			return $request;
		}
		*/


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

		public function send() {
			return $this->_adapter->send($this);
		}


        /**
         * Write request
         * @param string $file
         * @return mixed
         * @todo review return type
         * @throws HTTP_EXCEPTION 
         */
		public function write($file) {
			
			$response = $this->_adapter->write($this,$file);
				
			return $response;
		}		
	}
	