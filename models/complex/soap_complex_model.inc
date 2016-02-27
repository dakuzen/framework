<?
	
	if(extension_loaded("soap")) {

		class BASE_CMODEL_SOAP extends SoapClient {

			const SOAP_1_2 				= SOAP_1_2;

			protected $_debug 			= false;
			protected $_request 		= "";
			protected $_response 		= "";
			protected $_last_call 		= array();
			protected $_timeout 		= 0;
			protected $_connecttimeout 	= 0;
			protected $_sslverifypeer 	= true;

			public function __construct($wsdl,$options=array()) {

				$options["uri"] 		= value($options,"uri","http://service.org");
				$options["exceptions"] 	= true;

				$exception = null;
				
				try {
					
					DEBUG_UTIL::disable_fatal_error_handler();
					DEBUG_UTIL::enable_exception_error_handler();

					if(preg_match("/^(https?)/i",strtolower($wsdl),$matches))
						if(!in_array(value($matches,1),stream_get_wrappers()))
							throw new Exception("Unable to find the wrapper ".value($matches,1)." - did you forget to enable it?");

			    	$parent = @parent::__construct($wsdl,$options);
			    
			    } catch(Exception $exception) {
			    
			    }

			    DEBUG_UTIL::restore_error_handler();
			    
			    //HACK: Cannot enable this becuase it will show the last error and in this case its an uncatchable error from SoapClient
			    //DEBUG_UTIL::enable_fatal_error_handler();

			    if($exception)
			    	throw $exception;

		    	return $parent;
		    }

			public static function create($wsdl,$options=array()) { return new BASE_CMODEL_SOAP($wsdl,$options); }

			public function debug() 		{ $this->_debug = true; return $this; }
			
			public function get_request($formatted=false) 	{ return $formatted ? XML_READER_UTIL::get_pretty_xml($this->_request) : $this->_request ; }
			public function get_response($formatted=false) 	{ return $formatted ? XML_READER_UTIL::get_pretty_xml($this->_response) : $this->_response ; }
			public function request($formatted=false)		{ return $this->get_request($formatted); }
			public function response($formatted=false)	 	{ return $this->get_response($formatted); }
			public function last_call() 					{ return $this->_last_call; }

			public function audit($value) { 
				if($this->_debug)
					p($value);
			}

			public function __doRequest($request,$location,$action,$version,$one_way=FALSE) {

				$this->_request = (string)$request;

				$this->audit("URL: ".$location);
				$this->audit("Request:\n".$this->get_request(true));

				if (($this->_timeout===0) && ($this->_connecttimeout===0)) {
					// Call via parent because we require no timeout
					$this->_response = (string)parent::__doRequest($request,$location,$action,$version,$one_way);

				} else {
					// Call via Curl and use the timeout
					$curl = curl_init($location);
					if ($curl === false)
						throw new Exception('Curl initialisation failed');
					
					$options = array(
						CURLOPT_VERBOSE => false,
						CURLOPT_RETURNTRANSFER => true,
						CURLOPT_POST => true,
						CURLOPT_POSTFIELDS => $request,
						CURLOPT_HEADER => false,
						CURLOPT_NOSIGNAL => true,	//http://www.php.net/manual/en/function.curl-setopt.php#104597
						CURLOPT_HTTPHEADER => array(sprintf('Content-Type: %s', $version == 2 ? 'application/soap+xml' : 'text/xml'), sprintf('SOAPAction: %s', $action)),
						CURLOPT_SSL_VERIFYPEER => $this->_sslverifypeer,
						CURLOPT_SSL_VERIFYHOST=>2,
						CURLOPT_CAINFO=>MODEL_PATH::get_framework_assets_dir()."curl/cacert.pem");
					
					if ($this->_timeout>0) {
						if (defined('CURLOPT_TIMEOUT_MS')) {	//Timeout in MS supported? 
							$options[CURLOPT_TIMEOUT_MS] = $this->_timeout;		
						} else	{ //Round(up) to second precision
							$options[CURLOPT_TIMEOUT] = ceil($this->_timeout/1000);	
						}
					}

					if ($this->_connecttimeout>0) {
						if (defined('CURLOPT_CONNECTTIMEOUT_MS')) {	//ConnectTimeout in MS supported? 
							$options[CURLOPT_CONNECTTIMEOUT_MS] = $this->_connecttimeout;	
						} else { //Round(up) to second precision
							$options[CURLOPT_CONNECTTIMEOUT] = ceil($this->_connecttimeout/1000);	
						}
					}
					
					if (curl_setopt_array($curl, $options) === false)
						throw new Exception('Failed setting CURL options');
					
					$this->_response = curl_exec($curl);
					
					if (curl_errno($curl))
						throw new Exception(curl_error($curl));
					
					curl_close($curl);
				}

				$this->_last_call = array("response"=>$this->get_response(1),"request"=> $this->get_request(1),"url"=>(string)$location);

				$this->audit("Response:\n".$this->get_response(true));
				
				// Return?
				if (!$one_way)
					return $this->_response;
			}
		}

	} else {

		class BASE_CMODEL_SOAP {

			public function __construct() {
		    	throw new Exception("SOAP extension not loaded");		    	
		    }

		    public static function create() {
		    	return new BASE_CMODEL_SOAP();
		    }
		}		
	}