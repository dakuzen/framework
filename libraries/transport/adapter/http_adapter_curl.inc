<?
	/**
        * Class HTTP_ADAPTER_CURL  
        *
        * @package Library
        * @subpackage Transport
        */
	class HTTP_ADAPTER_CURL extends HTTP_ADAPTER {

		/**
		 * The curl
		 * @var boolean $curl 
		 */
		protected $_curl 	= false;

		/**
		 * The fp
		 * @var boolean $fp 
		 */
		protected $_fp 		= false;

		/**
		 * sendRequest
		 * @return string 
		 */


        /**
         * setupRequest
         */
		public function send($http_request) {

			$this->_curl = curl_init($http_request->get_url());

			curl_setopt($this->_curl,CURLOPT_TIMEOUT,$http_request->requestTimeout);

			switch(strtolower($http_request->httpVersion)) {
				case 'http/1.0':
					curl_setopt($this->_curl,CURLOPT_HTTP_VERSION,CURL_HTTP_VERSION_1_0);
					break;
				case 'http/1.1':
				default:
					curl_setopt($this->_curl,CURLOPT_HTTP_VERSION,CURL_HTTP_VERSION_NONE);
				break;
			}

			curl_setopt($this->_curl,CURLOPT_SSL_VERIFYPEER,false);
			curl_setopt($this->_curl,CURLOPT_CAINFO,MODEL_PATH::get_framework_assets_dir()."curl/cacert.pem");
			
			curl_setopt($this->_curl,CURLOPT_CUSTOMREQUEST,$http_request->get_verb());
			
			
			// general stuff
			curl_setopt($this->_curl,CURLOPT_BINARYTRANSFER,true);
			curl_setopt($this->_curl,CURLOPT_RETURNTRANSFER,true);

			//curl_setopt($this->_curl,CURLOPT_ENCODING,"gzip,deflate");
			
			if($http_request->get_username()) {
				curl_setopt($this->_curl, CURLOPT_USERPWD, $http_request->get_username().":".$http_request->get_password());
				curl_setopt($this->_curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
			}

			curl_setopt($this->_curl,CURLOPT_HEADER,1);
			curl_setopt($this->_curl, CURLINFO_HEADER_OUT, true);

			//default the content type on a post/put application/x-www-form-urlencoded
			if(in_array($http_request->get_verb(),array("POST","PUT")) && $http_request->get_header()->exists("content-type"))
				$http_request->get_header()->set("Content-Type","application/x-www-form-urlencoded");
		
			// post data
			if($http_request->get_body()) {

				$body = $this->get_body();

				if(is_array($body)) {

					foreach($body as $key=>&$value) {
						
						// Backwards compatability for PHP <5.4
						if(is_a($value,"CurlFile") && version_compare(phpversion(), '5.5.0')<0)
							$value = "@".$value->name;

						if(is_a($value,"CurlFile") || (is_string($value) && preg_match("/^@/",$value)))
							$http_request->get_header()->set("content-type","multipart/form-data");
					}
				}

				$content_type = $http_request->get_header()->get("content-type");

				if(preg_match("/application\/x-www-form-urlencoded/i",$content_type) && (is_array($body) || is_object($body)))
					$body = http_build_query($body);

				curl_setopt($this->_curl,CURLOPT_POSTFIELDS,$body);
			}

			curl_setopt($this->_curl,CURLOPT_VERBOSE, 1);	
			curl_setopt($this->_curl,CURLOPT_HTTPHEADER,$http_request->get_header()->get_formatted());

			$response = curl_exec($this->_curl);

			if($error_number=curl_errno($this->_curl)) {
				$error_message = curl_error($this->_curl);
				throw new Exception($error_message,$error_number);
			}
			
			if($this->_fp !== false)
				fclose($this->_fp);
			
			$code = curl_getinfo($this->_curl,CURLINFO_HTTP_CODE);
			$info = curl_getinfo($this->_curl);

			$http_request->get_header()->populate(value($info,"request_header"));

			$header_size = curl_getinfo($this->_curl, CURLINFO_HEADER_SIZE);
			$header = substr($response, 0, $header_size);
			$body = substr($response, $header_size);


			$response_header = new HTTP_HEADER($header);

			$http_response = new HTTP_RESPONSE($code, $body, $response_header, array(), "");
			$http_response->set_request_header($http_request->get_header());
			
			return $http_response;
		}

		public function write($http_request, $file) { }
	}
