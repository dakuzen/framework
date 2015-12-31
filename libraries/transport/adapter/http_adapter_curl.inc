<?php
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
		public function sendRequest() {
			$this->_setupRequest();
			return $this->_sendRequest();
		}

        /**
         * request To File
         * @param string $file
         * @return string 
         */
		public function requestToFile($file) {
			$this->_setupRequest();
	
			$this->_fp = fopen($file,'w');
			curl_setopt($this->_curl,CURLOPT_FILE,$this->_fp);

			return $this->_sendRequest();
		}

        /**
         * setupRequest
         */
		protected function _setupRequest() {
			
			$this->_curl = curl_init($this->uri->url);

			curl_setopt($this->_curl,CURLOPT_TIMEOUT,$this->requestTimeout);

			switch(strtolower($this->httpVersion)) {
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
			
			curl_setopt($this->_curl,CURLOPT_CUSTOMREQUEST,$this->verb);
			
			// headers
			curl_setopt($this->_curl,CURLOPT_HTTPHEADER,$this->get_formatted_header_array());

			// general stuff
			curl_setopt($this->_curl,CURLOPT_BINARYTRANSFER,true);
			curl_setopt($this->_curl,CURLOPT_RETURNTRANSFER,true);

			//curl_setopt($this->_curl,CURLOPT_ENCODING,"gzip,deflate");
			
			if($this->get_username()) {
				curl_setopt($this->_curl, CURLOPT_USERPWD, $this->get_username().":".$this->get_password());
				curl_setopt($this->_curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
			}

			// setup a callback to handle header info
			curl_setopt($this->_curl,CURLOPT_HEADERFUNCTION,array($this,"_headerCallback"));
			curl_setopt($this->_curl, CURLINFO_HEADER_OUT, true);

			//default the content type on a post/put application/x-www-form-urlencoded
			if(in_array($this->verb,array("POST","PUT")) && !array_key_exists("content-type",array_change_key_case($this->headers, CASE_LOWER)))
				$this->headers["Content-Type"] = "application/x-www-form-urlencoded";
		
			// post data
			if($this->body) {

				$this->headers = array_change_key_case($this->headers, CASE_LOWER);

				$body = $this->body;

				if(is_array($body)) {

					foreach($body as $key=>&$value) {
						
						// Backwards compatability for PHP <5.4
						if(is_a($value,"CurlFile") && version_compare(phpversion(), '5.5.0')<0)
							$value = "@".$value->name;

						if(is_a($value,"CurlFile") || (is_string($value) && preg_match("/^@/",$value)))
							$this->headers["content-type"] = "multipart/form-data";
					}
				}

				$content_type = value($this->headers,"content-type");

				if(preg_match("/application\/x-www-form-urlencoded/i",$content_type) && (is_array($body) || is_object($body)))
					$body = http_build_query($body);

				curl_setopt($this->_curl,CURLOPT_POSTFIELDS,$body);
			}

			curl_setopt($this->_curl, CURLOPT_VERBOSE, 1);
		}

        /**
         * sendRequest
         * @return HTTP_RESPONSE
         * @throws Exception 
         */
		protected function _sendRequest() {

			$body = curl_exec($this->_curl);

			if($error_number=curl_errno($this->_curl)) {
				$error_message = curl_error($this->_curl);
				throw new Exception($error_message,$error_number);
			}
			
			if($this->_fp !== false)
				fclose($this->_fp);
			
			$code = curl_getinfo($this->_curl,CURLINFO_HTTP_CODE);

			$http_header = new HTTP_HEADER($this->headers);
						
			$request_headers = explode("\n",trim(curl_getinfo($this->_curl,CURLINFO_HEADER_OUT)));
			
			if($this->body) {
				$request_headers[] = "";
				$request_headers[] = $this->body;
			}
			
			$http_response = new HTTP_RESPONSE($code, $body, $http_header, array(), "");
			$http_response->set_request_header(new HTTP_HEADER($request_headers));
			
			return $http_response;
		}
                
        /**
         * header Call back
         * @param string $curl
         * @param string $data
         * @return integer 
         */
		protected function _headerCallback($curl,$data) {
			$this->processHeader(trim($data));
			return strlen($data);
		}
	}
