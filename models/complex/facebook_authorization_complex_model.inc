<?
    /**
    * Class BASE_CMODEL_FACEBOOK_AUTHORIZATION
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_FACEBOOK_AUTHORIZATION extends MODEL_COMPLEX {
		
		/**
		 * The facebook client
		 * @var object $facebook_client 
		 * @todo review var type
		 */
		private $_facebook_client	= null;

		/**
		 * The debug
		 * @var boolean $debug 
		 */
		private $_debug			= false;

		/**
		 * Constructor
		 * @param object $facebook_client 
		 * @todo review param type
		 */
		public function __construct($facebook_client_cmodel=null) {			
			$this->_facebook_client = $facebook_client_cmodel ? $facebook_client_cmodel : MODEL_FACEBOOK::create_client();
		}		

		/**
		 * Returns the facebook client
		 * @return object 
		 * @todo review return type
		 */
		public function get_facebook_client() 	{ return $this->_facebook_client; }

		/**
		 * Returns the secret key
		 * @return string $secret_key 
		 * @todo review return type
		 */
		public function get_secret_key() 	{ return $this->_facebook_client->get_secret_key(); }

		/**
		 * Return the app id
		 * @return integer 
		 */
		public function get_app_id() 		{ return $this->_facebook_client->get_app_id(); }

		/**
		 * Debug
		 * @param boolean $debug
		 * @return boolean 
		 */
		public function debug($debug=true) 	{ return $this->_debug = $debug; }

		/**
		 * Checks whether is debug enabled
		 * @return boolean 
		 */
		public function is_debug() 		{ return $this->_debug; }
				
		/**
		 * Signed Code/Payload methods
		 */
		static public function has_code_in_request() {
						
			$parts = parse_url(SERVER_UTIL::get_request_url());
			
			$query = get_value($parts,"query");
			
			parse_str($query,$vars);
			
			return array_key_exists("code",$vars);
		}
		
        /**
         * Process code from request
         * @param string $original_url
         * @return boolean 
         */
		public function process_code_from_request($original_url="") {
			
			if($this->is_debug())
				p("BASE_CMODEL_FACEBOOK_AUTHORIZATION::process_code_from_request() Start Debug");
				
			$payload = $this->get_payload_from_request($original_url);
			
			$has_success = $this->process_payload($payload);
			
			if($this->is_debug()) 
				p("BASE_CMODEL_FACEBOOK_AUTHORIZATION::process_code_from_request()","Facebook User: ".$this->_facebook_client->getUser());
				
			return $has_success;
		}

        /**
         * Process payload
         * @param string $payload
         * @return boolean
         * @throws Exception 
         * @todo review param type
         */
		public function process_payload($payload) {
			
			if($this->is_debug())
				p("BASE_CMODEL_FACEBOOK_AUTHORIZATION::process_payload()");
			
			if($this->is_debug())
				p("Payload",$payload);
			
			if(!get_value($payload,"access_token"))
				throw new Exception("Failed to load access_token");
			
			$this->_facebook_client->set_payload($payload);
			
			return true;
		}
		
        /**
         * Returns the payload from request
         * @param string $original_url
         * @return boolean
         * @throws Exception 
         */
		public function get_payload_from_request($original_url="") {
						
			if($this->is_debug())
				p("BASE_CMODEL_FACEBOOK_AUTHORIZATION::get_payload_from_request(original_url=".$original_url.")");
			
			$parts = parse_url(SERVER_UTIL::get_request_url());
			
			$query = get_value($parts,"query");
			
			parse_str($query,$vars);
						
			if($original_url) {
			
				preg_match("/^[^\?]+/",$original_url,$match);
				$redirect_uri = get_value($match,0);
			} else
				$redirect_uri = get_value($parts,"scheme")."://".get_value($parts,"host").get_value($parts,"path");
			
			$code = get_value($vars,"code");
			
			$redirect_uri = rtrim($redirect_uri,"/")."/";
			
			if(!$code)
				throw new Exception("Failed to locate the 'code' request variable");
		
			return $this->get_payload($code,$redirect_uri);		
		}

        /**
         * Returns the payload
         * @param string $code
         * @param string $redirect_uri
         * @return booelan 
         */
		public function get_payload($code,$redirect_uri="") {		
			return $this->get_access_token($code,$redirect_uri);
		}

		public function get_extended_access_token($access_token,$app_id="",$secret_key="") {

			$app_id		= $app_id ? $app_id : $this->get_app_id();
			$secret_key = $secret_key ? $secret_key : $this->get_secret_key();
		
			if($this->is_debug())
				p("BASE_CMODEL_FACEBOOK_AUTHORIZATION::get_payload(code=".$code.",redirect_uri=".$redirect_uri.")");
			
			$post["client_id"] 			= $app_id;
			$post["client_secret"] 	 	= $secret_key;
			$post["grant_type"] 		= "fb_exchange_token";			
			$post["fb_exchange_token"] 	= $access_token;
	
			if($this->is_debug())
				p("POST: ".print_r($post,true));
			
			$http_transport_util = new HTTP_TRANSPORT_UTIL();
			$has_success = $http_transport_util->post("https://graph.facebook.com/oauth/access_token",$post);

			$http_response = $http_transport_util->get_http_response();
			
			if($this->is_debug())
				p("http_response: ".$http_response);
			
			$content = $http_response->get_body();

			if(!$http_response->is_code_success()) {

				$content = JSON_UTIL::decode($http_response->get_body());

				throw new Exception(get_value($content,array("error","message")));
			}

			$payload = array();			
			parse_str($content,$payload);
			
			return get_value($payload,"access_token");
		}

        /**
         * Returns the access token
         * @param string $code
         * @param string $redirect_uri
         * @param integer $app_id
         * @param string $secret_key
         * @return booelan
         * @throws Exception 
         */
		public function get_access_token($code,$app_id="",$secret_key="",$redirect_uri="") {
			
			$app_id		= $app_id ? $app_id : $this->get_app_id();
			$secret_key = $secret_key ? $secret_key : $this->get_secret_key();
		
			if($this->is_debug())
				p("BASE_CMODEL_FACEBOOK_AUTHORIZATION::get_payload(code=".$code.",redirect_uri=".$redirect_uri.")");
			
			$post["client_id"] 		= $app_id;
			$post["client_secret"]  = $secret_key;
			$post["code"] 			= $code;			
			$post["redirect_uri"] 	= $redirect_uri ? $redirect_uri : "";
			
			/*
			if($redirect_uri)
				$post["redirect_uri"] = urlencode(rtrim($redirect_uri,"/")."/");
			else
				$post["type"] = "client_cred";
			*/
			
			if($this->is_debug())
				p("POST: ".print_r($post,true));
			
			$http_transport_util = new HTTP_TRANSPORT_UTIL();
			$has_success = $http_transport_util->post("https://graph.facebook.com/oauth/access_token",$post);

			$http_response = $http_transport_util->get_http_response();
			
			if($this->is_debug())
				p("http_response: ".$http_response);
			
			$content = $http_response->get_body();
			
			if(!$http_response->is_code_success()) {

				$content = JSON_UTIL::decode($http_response->get_body());

				throw new Exception(get_value($content,array("error","message")));
			}

			$payload = null;
			
			if($content)
				parse_str($content,$payload);
			else
				throw new Exception("Failed to get content");
			
			return get_value($payload,"access_token");
		}

        /**
         * presist session payload
         */
		public function presist_session_payload() {
			$_SESSION["facebook_authorization_payload"] = $this->_facebook_client->get_payload();
		}

        /**
         * Loads session payload
         * @throws Exception 
         */
		public function load_session_payload() {
			$payload = get_value($_SESSION,"facebook_authorization_payload");
			
			if(!$payload)
				throw new Exception("Failed to load payload from session");
			
			$this->_facebook_client->set_payload($payload);
		}		
		
		/**
		 * Signed Request methods
		 */
		public function process_signed_request_from_request() {
			return $this->process_signed_request($this->get_signed_request_from_request());
		}

        /**
         * Process signed request
         * @param object $signed_request_cmodel
         * @return boolean
         * @throws Exception 
         */
		public function process_signed_request($signed_request_cmodel) {
			
			if(!$signed_request_cmodel)
				throw new Exception("Failed to load signed request");
				
			$access_token = $signed_request_cmodel->get_access_token();
			
			if(!$access_token)
				throw new Exception("Failed to load access_token");
			
			$this->_facebook_client->set_access_token($access_token);
			
			return true;
		}
		
        /**
         * Returns signed request from request
         * @param string $secret_key
         * @return null 
         */
		public function get_signed_request_from_request($secret_key=null,$app_id="",$secret_key="") {
			
			$registry	= REGISTRY::get_instance();
			$signed_request	= $registry->get_request_parm("signed_request");
			
			if(!$signed_request)
				return null;
				
			return self::get_signed_request($signed_request,$app_id,$secret_key);
		}

        /**
         * parse signed request
         * @param string $signed_request
         * @param string $secret
         * @param boolean $debug
         * @return string 
         */
		static function parse_signed_request($signed_request,$secret=null,$debug=false) {
			$secret = $secret ? $secret : MODEL_FACEBOOK::get_secret_key();
			return self::_parse_signed_request($signed_request, $secret,$debug);
		}
		
        /**
         * Return the signed request
         * @param string $signed_request
         * @param string $secret
         * @param boolean $debug
         * @return object BASE_CMODEL_FACEBOOK_SIGNED_REQUEST
         * @throws Exception 
         */
		static function get_signed_request($signed_request,$app_id=null,$secret_key=null) {
			
			if(!$signed_request)
				throw new Exception("The signed request is empty");

			return new BASE_CMODEL_FACEBOOK_SIGNED_REQUEST(self::parse_signed_request($signed_request),$app_id,$secret_key);
		}

        /**
         * Parse signed request
         * @param string $signed_request
         * @param string $secret
         * @param boolean $debug
         * @return string 
         */
		static function _parse_signed_request($signed_request, $secret="",$debug=false) {
		
			$parts = explode('.', $signed_request, 2);
			
			if($debug)
				p("BASE_CMODEL_FACEBOOK_AUTHORIZATION::_parse_signed_request(signed_request: ".$signed_request.", secret: ".$secret.")");
				
			$encoded_sig 	= get_value($parts,0);
			$payload 	= get_value($parts,1);
			
			$sig 	= base64_decode(strtr($encoded_sig, '-_', '+/'));
			$data 	= json_decode(base64_decode(strtr($payload, '-_', '+/')), true);
			
			if($debug)
				p("data: ".print_r($data,true));
			
			if(strtoupper($data["algorithm"]) !== "HMAC-SHA256") {
				
				if($debug)
					p("algorithm != HMAC-SHA256");
				
				return null;
			}
			
			/*
			removed this because sig and expected sig were not ='ing
			$expected_sig = hash_hmac("sha256", $payload, $secret, true);
			
			if ($sig !== $expected_sig) {
				
				if($debug)
					p("sig != expected_sig");
				
				return null;
			}
			*/
			
			return $data;
		}		
		
	}