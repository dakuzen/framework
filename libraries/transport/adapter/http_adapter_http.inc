<?php
	/**
        * Class HTTP_ADAPTER_HTTP  
        *
        * @package Library
        * @subpackage Transport
        */
	class HTTP_ADAPTER_HTTP extends HTTP_ADAPTER {
                
                /**
                 * Constructor
                 * @throws HTTP_EXCEPTION 
                 */
		public function __construct() {
			if (!extension_loaded('http')) 
				throw new HTTP_EXCEPTION('The http extension must be loaded in order to use the Peclhttp adapter');
		}

                /**
                 * send Request
                 * @return HTTP_RESPONSE 
                 */
		public function sendRequest() {
			$options = array('connecttimeout'=>$this->requestTimeout);

			if (count($this->_listeners) > 0) 
				$options['onprogress'] = array($this,'_onprogress');

			$tmp = 'HTTP_METH_'.strtoupper($this->verb);
			
			if (defined($tmp))
				$method = constant($tmp);			
			else 
				$method = HTTP_METH_GET;
			
			$request = new HTTP_REQUEST($this->uri->url,$method,$options);
			$request->setHeaders($this->headers);
			$request->setRawPostData($this->body);

			$request->send();
			$response = $request->getResponseMessage();
			$body = $response->getBody();

			$header = new HTTP_HEADER($response->getHeaders());
			$cookies = $request->getResponseCookies();

			return new HTTP_RESPONSE($request->getResponseCode(), $body, $header, $cookies, $response->getHttpVersion());
		}	   

                /**
                 * on progress
                 * @param string $status 
                 */
		public function _onprogress($status) {}
	}
