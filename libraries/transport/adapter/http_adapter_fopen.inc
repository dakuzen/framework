<?
	/**
    * Class HTTP_ADAPTER_FOPEN  
    *
    * @package Library
    * @subpackage Transport
    */
	class HTTP_ADAPTER_FOPEN extends HTTP_ADAPTER {
		
        /**
         * The phpErrorStr
         * @var string $phpErrorStr
         */
		private $_phpErrorStr;

        /**
         * Constructor
         * @throws HTTP_EXCEPTION 
         */
		public function __construct() {
			if(!ini_get("allow_url_fopen")) 
				throw new HTTP_EXCEPTION("allow_url_fopen is off, the http:// stream wrapper will not function");
		}

        /**
         * sendRequest
         * @return object HTTP_RESPONSE
         * @throws HTTP_EXCEPTION 
         */
		private function _sendRequest($http_request) {

			$proxyurl = !is_null($http_request->get_proxy()) ? $http_request->get_proxy()->url : "";
		
			$ctx = stream_context_create(array($http_request->get_protocol() => array(	"method" =>$http_request->get_verb(),
												"content" => $http_request->get_body(),
												"header" => $http_request->get_header()->get_formatted(),
												"proxy"  => $proxyurl)));
						
			set_error_handler(array($this,"_errorHandler"));
			$fp = fopen($http_request->get_url(), "rb", false, $ctx);
			
			if (!is_resource($fp)) {
				// php sucks
				if (strpos($this->_phpErrorStr, "HTTP/1.1 304")) {
					restore_error_handler();
					return new HTTP_RESPONSE(304,"",null,array(),"1.1");
				}
			
				restore_error_handler();
			
				throw new HTTP_EXCEPTION("Url ".$http_request->get_url()." could not be opened (fopen Adapter (".$this->_phpErrorStr."))");
			} else 
				restore_error_handler();
			
			stream_set_timeout($fp, $http_request->get_request_timeout());	
			
			return $fp;
		}
		
        /**
         * write Request
         * @param string $file
         * @return object
         * @throws HTTP_EXCEPTION 
         */
		public function write($http_request, $file) {
			
			$fp = $this->_sendRequest($http_request);

			$http_response = $this->process_http_request($fp);
			
			$wfp = fopen($file,"w");
			
			if(!$wfp)
				throw new HTTP_EXCEPTION("Cannot open file ".$file." for writting");

			stream_copy_to_stream($fp,$wfp);
		
			if (!$wfp)
				throw new HTTP_EXCEPTION("Cannot open file ".$file." for writting");

			while (!feof($fp)) {

				$data = fread($fp, 8192);

				fwrite($wfp, $data);
			}

			fclose($wfp);
		
			fclose($fp);
			
			return $http_response;
		}

        /**
         * send Request
         * @return object
         * @throws HTTP_EXCEPTION 
         */
		public function send($http_request) {
			
			$fp = $this->_sendRequest($http_request);
			
			$body = stream_get_contents($fp);

			if($body===false) 
				throw new HTTP_EXCEPTION("Url ".$http_request->get_url()." did not return a response");
			
			$http_response = $this->process_http_request($fp,$body);
			
			fclose($fp);
			
			return $http_response;
		}
		
        /**
         * process http request
         * @param string $fp
         * @param string $body
         * @return object HTTP_RESPONSE 
         */
		private function process_http_request($fp,$body="") {
			
			$meta = stream_get_meta_data($fp);
			
			$headers = get_value($meta,"wrapper_data");
			
			$response = $this->parseResponseCode(get_value($headers,0));
			
			$this->cookies = $this->headers = array();

			return new HTTP_RESPONSE(value($response,"code"),$body,new HTTP_HEADER($this->headers),$this->cookies,get_value($response,"httpVersion"));
		}
		
    /**
     * error Handler
     * @param string $errno
     * @param string $errstr 
     */
		private function _errorHandler($errno,$errstr) { $this->_phpErrorStr = $errstr; }
	}

