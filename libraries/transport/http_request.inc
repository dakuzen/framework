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
         * Get
         * @param string $name
         * @return string 
         */
		public function __get($name) {
			if (isset($this->_adapter->$name))
				return $this->_adapter->$name;
		}

        /**
         * Set
         * @param string $name
         * @param string $value 
         */
		public function __set($name, $value) {
			
			switch($name) {
				case "verb":
					$this->_adapter->verb = strtoupper($value);
					break;
				
				case "uri":
				case "url":
					$this->_adapter->uri = new HTTP_URI($value);
					break;
				
				case "requestTimeout":
					$this->_adapter->$name = (int)$value;
					break;
				
				case "proxy":
					$this->_adapter->$name = new HTTP_URI($value);
					break;				
				
				default:
					$this->_adapter->$name = $value;
					break;
			} 
		}
		
        /**
         * Set body
         * @param string $value 
         */
		function set_body($value) {
			
			if (is_array($value)) {
				$this->_adapter->set_body($value);
				
				if ($this->_adapter->verb == 'GET') 
					$this->_adapter->verb = 'POST';
			} else 
				$this->_adapter->set_body($value);
		}

        /**
         * sets up the adapter
         * @param string $url
         * @param string $type 
         */
		public function __construct($url=null,$type=null)  {
			
			$this->_type = $type;
			
			if(extension_loaded('curl') && ($this->_type=="curl" || !$this->_type)) {
				require_once(DIR_FRAMEWORK."libraries/transport/adapter/http_adapter_curl.inc");
				$this->_adapter = new HTTP_ADAPTER_CURL();
			
			} elseif(extension_loaded('http') && ($this->_type=="http" || !$this->_type)) {
				require_once(DIR_FRAMEWORK."libraries/transport/adapter/http_adapter_http.inc");
				$this->_adapter = new HTTP_ADAPTER_HTTP();
				
			} elseif($this->_type=="fopen" || !$this->_type) {				
				require_once(DIR_FRAMEWORK."libraries/transport/adapter/http_adapter_fopen.inc");
				$this->_adapter = new HTTP_ADAPTER_FOPEN();				

			} else {				
				require_once(DIR_FRAMEWORK."libraries/transport/adapter/http_adapter_phpsocket.inc");
				$this->_adapter = new HTTP_ADAPTER_PHPSOCKET();
			}
			
			if(!$this->_adapter)
				$this->_adapter->setListeners($this->_listeners);

			if($url) 
				$this->url = $url;
		}
		
        /**
         * Returns transport type
         * @return string 
         */
		public function get_transport_type() { return $this->_type; }

        /**
         * Send a request
         */
		public function sendRequest() {
			
			$response = $this->_adapter->sendRequest();
			
			if(!$response->get_code())
				throw new HTTP_EXCEPTION("The domain does not exist");	
				
			return $response;
		}

        /**
         * Write request
         * @param string $file
         * @return mixed
         * @todo review return type
         * @throws HTTP_EXCEPTION 
         */
		public function writeRequest($file) {
			
			$response = $this->_adapter->writeRequest($file);
			
			if(!$response->get_code())
				throw new HTTP_EXCEPTION("The domain does not exist");	
				
			return $response;
		}		

        /**
         * request To File
         * @param string $file
         * @return mixed
         * @todo review return type
         */
		public function requestToFile($file) {
			$response = $this->_adapter->requestToFile($file);
			return $response;
		}

        /**
         * set Header
         * @param string $header
         * @param string $value 
         */
		public function setHeader($header, $value) {
			$this->_adapter->headers[$header] = $value;
		}
		
        /**
         * add header
         * @param string $header
         * @param string $value 
         */
		public function add_header($header, $value) {
			$this->_adapter->headers[$header] = $value;
		}		
		
        /**
         * set Headers
         * @param string $headers 
         */
		public function setHeaders($headers) 		{ $this->_adapter->headers = $headers; return $this; }
                
        /**
         * set username
         * @param string $username 
         */
		public function set_username($username) 	{ $this->_adapter->set_username($username); return $this; }	
        
        /**
         * set password
         * @param string $password 
         */
		public function set_password($password) 	{ $this->_adapter->set_password($password); return $this; }

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
                
        /**
         * Returns uri
         * @return string
         */
		public function get_uri() 		{ return $this->_adapter->uri; }

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
	}
	