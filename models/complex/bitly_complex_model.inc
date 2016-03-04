<?	
	class BASE_CMODEL_BITLY {
            
		/**
         * The login.
         * @var string $login 
         */
		protected $_login 		= "";
                
        /**
         * The app key.
         * @var string $app_key 
         */
		protected $_app_key 	= "";
		
		const VERSION 			= "2.0.1";
			 
        /**
         * Constructor.
         * @param string $login the login.
         * @param string $app_key the app key.
         */
		function __construct($login,$app_key) {
			$this->_login 	= $login;
			$this->_app_key = $app_key;
		}

		public static function create($login,$app_key) {
			return new CMODEL_BITLY($login,$app_key);
		}
		
        /**
         * Shorten a url address
         * @param string $url the login.
         */
		function shorten($url) {
		
			if(preg_match("/[^\/](\?)/",$url)) 				
				$url = str_replace("?","/?",$url);
			
			$url = 'http://api.bit.ly/shorten?version='.self::VERSION.'&longUrl='.urlencode($url).'&login='.$this->_login.'&apiKey='.$this->_app_key.'&format=json';

		 	$http_transport_util = new HTTP_TRANSPORT_UTIL();
			$http_transport_util->get($url);
	
			$response = $http_transport_util->get_http_response();

			$body = JSON_UTIL::decode($response->get_body());
			
			$bitly_response = new BITLY_RESPONSE();
			$bitly_response->populate($body);

			if($bitly_response->get_status_code()!="OK")
				throw new Exception($bitly_response->get_error_message());				
		
			return $bitly_response;
		}
	}
        
    /**
	 * Class BITLY_RESPONSE 
	 *
     * @package Framework
     * @subpackage Utilities
	 */

	class BITLY_RESPONSE {

		/**
		 * The status code of the response.
		 * @var string $status_code 
		 */
		protected $_status_code		= "";
		/**
		 * The error code of the response.
		 * @var string $error_code 
		 */
		protected $_error_code		= "";
		/**
		 * The error message of the response.
		 * @var string $error_message 
		 */
		protected $_error_message	= "";
		/**
		 * The results of the response.
		 * @var array $results 
		 */
		protected $_results		= array();

		/**
		 * Sets the status code
		 * @param string $status_code the status code.
		 */
		public function set_status_code($status_code)			{ $this->_status_code = $status_code; }

		/**
		 * Sets the error code
		 * @param string $error_code the error code.
		 */
		public function set_error_code($error_code)			{ $this->_error_code = $error_code; }

		/**
		 * Sets the error message.
		 * @param string $error_message the error message.
		 */
		public function set_error_message($error_message)		{ $this->_error_message = $error_message; }

		/**
		 * Sets the error message.
		 * @param array $results the results.
		 */
		public function set_results($results)				{ $this->_results = $results; }

		/**
		 * Gets the status code.
		 * 
		 */
		public function get_status_code()				{ return $this->_status_code; }

		/**
		 * Gets the error code.
		 * 
		 */
		public function get_error_code()				{ return $this->_error_code; }

		/**
		 * Gets the error message.
		 * 
		 */
		public function get_error_message()				{ return $this->_error_message; }

		/**
		 * Gets the results.
		 * 
		 */
		public function get_results()					{ return $this->_results; }

		/**
		 * Gets the result.
		 * 
		 */
		public function get_result()					{ return get_value($this->_results,0); }

		/**
		 * Has success
		 * 
		 * @todo Ray
		 */
		public function has_success()					{ return $this->get_error_code()==0; }

		/**
		 * Populates the response.
		 * @param array $response the response.
		 */
		public function populate($response) {
		
			if(!is_array($response))
				return false;
			
			$this->set_status_code(get_value($response,"statusCode"));
			$this->set_error_code(get_value($response,"errorCode"));
			$this->set_error_message(get_value($response,"errorMessage"));
			
			$results = get_value($response,"results",array());
			
			if(is_array($results)) {
				foreach($results as $long_url=>$result) {


					$bitly_result = new BITLY_RESULT();				
					$bitly_result->set_long_url($long_url);
					$bitly_result->populate($result);

					$this->_results[] = $bitly_result;
				}
			}
		}
	}	
        
    /**
	 * Class BITLY_RESULT 
	 *
     * @package Framework
     * @subpackage Utilities
	 */

	class BITLY_RESULT {
                
		/**
		 * The user hash.
		 * @var string $user_hash
		 */
		protected $_user_hash		= "";

		/**
		 * The hash.
		 * @var string $hash
		 */
		protected $_hash		= "";

		/**
		 * The short url.
		 * @var string $short_url
		 */
		protected $_short_url		= "";

		/**
		 * The long url.
		 * @var string $long_url
		 */
		protected $_long_url		= "";

		/**
		 * The short cname url.
		 * @var string $short_cname_url
		 */
		protected $_short_cname_url	= "";

		/**
		 * The short keyword url.
		 * @var string $short_keyword_url
		 */
		protected $_short_keyword_url	= "";

		/**
		 * Set user hash.
		 * @param string $user_hash user hash
		 */
		public function set_user_hash($user_hash)			{ $this->_user_hash = $user_hash; }

		/**
		 * Set the long url.
		 * @param string $long_url long url
		 */
		public function set_long_url($long_url)				{ $this->_long_url = $long_url; }

		/**
		 * Set the hash.
		 * @param string $hash hash
		 */
		public function set_hash($hash)					{ $this->_hash = $hash; }

		/**
		 * Set the short url.
		 * @param string $short_url short url
		 */
		public function set_short_url($short_url)			{ $this->_short_url = $short_url; }

		/**
		 * Set the short cname url.
		 * @param string $short_cname_url short cname url
		 */
		public function set_short_cname_url($short_cname_url)		{ $this->_short_cname_url = $short_cname_url; }

		/**
		 * Set the short keyword url.
		 * @param string $short_keyword_url short keyword url
		 */
		public function set_short_keyword_url($short_keyword_url)	{ $this->_short_keyword_url = $short_keyword_url; }

		/**
		 * Get the user hash.
		 * 
		 */
		public function get_user_hash()					{ return $this->_user_hash; }

		/**
		 * Get the long url.
		 * 
		 */
		public function get_long_url()					{ return $this->_long_url; }

		/**
		 * Get the hash.
		 * 
		 */
		public function get_hash()					{ return $this->_hash; }

		/**
		 * Get the short url.
		 * 
		 */
		public function get_short_url()					{ return $this->_short_url; }

		/**
		 * Get the short cname url.
		 * 
		 */
		public function get_short_cname_url()				{ return $this->_short_cname_url; }

		/**
		 * Get the short keyword url.
		 * 
		 */
		public function get_short_keyword_url()				{ return $this->_short_keyword_url; }

		/**
		 * Populates the result.
		 * @param array $result the result
		 */
		public function populate($result) {
		
			if(!is_array($result))
				return false;
			
			$this->set_user_hash(get_value($result,"userHash"));
			$this->set_hash(get_value($result,"hash"));
			$this->set_short_url(get_value($result,"shortUrl"));
			$this->set_short_cname_url(get_value($result,"shortCNAMEUrl"));
			$this->set_short_keyword_url(get_value($result,"shortKeywordUrl"));
		}
		
	}		