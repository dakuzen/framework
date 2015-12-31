<?php
        /**
        * Class BASE_CMODEL_GOOGLE_CAPTCHA_RESPONSE
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_GOOGLE_CAPTCHA_RESPONSE {
                
                /**
                 * captcha path
                 * @var string $captcha_path 
                 */
		protected $_captcha_path	= "";
                
                /**
                 * captcha token
                 * @var string $captcha_token 
                 */
		protected $_captcha_token	= "";
                
                /**
                 * error code
                 * @var string $error_code 
                 */
		protected $_error_code		= "";
                
                /**
                 * the sid
                 * @var integer $sid 
                 */
		protected $_sid			= "";
                
                /**
                 * The lsid
                 * @var integer $lsid 
                 */
		protected $_lsid		= "";
                
                /**
                 * The auth
                 * @var string $auth 
                 */
		protected $_auth		= "";
		
                /**
                 * Returns the error code list
                 * @return array 
                 */
		static function get_error_code_list() {
			return array(	"BadAuthentication"=>"The login request used a username or password that is not recognized.",
					"NotVerified"=>"The account email address has not been verified. The user will need to access their Google account directly to resolve the issue before logging in using a non-Google application.",
					"TermsNotAgreed"=>"The user has not agreed to terms. The user will need to access their Google account directly to resolve the issue before logging in using a non-Google application.",
					"CaptchaRequired"=>"A CAPTCHA is required. (A response with this error code will also contain an image URL and a CAPTCHA token.)",
					"Unknown"=>"The error is unknown or unspecified; the request contained invalid input or was malformed.",
					"AccountDeleted"=>"The user account has been deleted.",
					"AccountDisabled"=>"The user account has been disabled.",
					"ServiceDisabled"=>"The user's access to the specified service has been disabled. (The user account may still be valid.)",
					"ServiceUnavailable"=>"The service is not available; try again later.");		
		}

                /**
                 * set captcha path
                 * @param string $captcha_path 
                 */
		public function set_captcha_path($captcha_path)		{ $this->_captcha_path = $captcha_path; }
                
                /**
                 * set captcha token
                 * @param string $captcha_token
                 */
		public function set_captcha_token($captcha_token)	{ $this->_captcha_token = $captcha_token; }
                
                /**
                 * set error code
                 * @param string $error_code 
                 */
		public function set_error_code($error_code)		{ $this->_error_code = $error_code; }
				
                /**
                 * Returns the captcha path
                 * @return string 
                 */
		public function get_captcha_path()			{ return $this->_captcha_path; }
                
                /**
                 * Returns the sid
                 * @return integer 
                 */
		public function get_sid()				{ return $this->_sid; }
                
                /**
                 * Returns the lsid
                 * @return integer 
                 */
		public function get_lsid()				{ return $this->_lsid; }
                
                /**
                 * Returns the auth
                 * @return string 
                 */
		public function get_auth()				{ return $this->_auth; }
                
                /**
                 * Returns the captcha token
                 * @return string
                 */
		public function get_captcha_token()			{ return $this->_captcha_token; }
                
                /**
                 * Returns the error code
                 * @return string
                 */
		public function get_error_code()			{ return $this->_error_code; }
                
                /**
                 * Returns the error message
                 * @return string
                 */
		public function get_error_message()			{ return get_value(self::get_error_code_list(),$this->get_error_code()); }
		
                /**
                 * Returns the captcha url
                 * @return string
                 */
                public function get_captcha_url()			{ return "http://www.google.com/accounts/".$this->_captcha_path; }
		
                /**
                 * Checks whether the error code captcha is required
                 * @return boolean 
                 */
		public function is_error_code_captcha_required()	{ return $this->get_error_code()=="CaptchaRequired"; }
		
                /**
                 * Checks whethers is successful
                 * @return boolean
                 */
                public function is_successful()				{ return !$this->_error_code; }
		
                /**
                 * Populates
                 * @param object $response 
                 */
		public function populate($response) { 
			$body = $response->get_body();
			
			$parms = preg_match_all("/([^=]+)=([^\n]+)\n/",$body,$matches);
			
			foreach(get_value($matches,1,array()) as $index=>$key) {
			
				$value = get_value($matches,array(2,$index));
				
				if($key=="CaptchaToken")
					$this->_captcha_token = $value;
				elseif($key=="CaptchaUrl")
					$this->_captcha_path = $value;
				elseif($key=="Error")
					$this->_error_code = $value;
				elseif($key=="SID")
					$this->_sid = $value;
				elseif($key=="LSID")
					$this->_lsid = $value;
				elseif($key=="Auth")
					$this->_auth = $value;					
					
			}
		}
        }