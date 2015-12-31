<?
    /**
    * Class BASE_CMODEL_FACEBOOK_SIGNED_REQUEST
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_FACEBOOK_SIGNED_REQUEST {

		/**
		 * The code
		 * @var string $code
		 */
		protected $_code			= "";

		/**
		 * The algorithm
		 * @var string $algorithm
		 */
		protected $_algorithm		= "";

		/**
		 * The expires
		 * @var string $expires
		 */
		protected $_expires			= "";

		/**
		 * The issued at
		 * @var string $issued_at
		 */
		protected $_issued_at		= "";

		/**
		 * The oauth token
		 * @var string $oauth_token
		 */
		protected $_oauth_token		= "";

		/**
		 * The page id
		 * @var string $page_id
		 */
		protected $_page_id			= "";

		/**
		 * The page liked
		 * @var string $page_liked
		 */
		protected $_page_liked		= "";

		/**
		 * The page admin
		 * @var string $page_admin
		 */
		protected $_page_admin		= "";

		/**
		 * The user country
		 * @var string $user_country
		 */
		protected $_user_country	= "";

		/**
		 * The user locale
		 * @var string $user_locale
		 */
		protected $_user_locale		= "";

		/**
		 * The user id
		 * @var string $user_id
		 */
		protected $_user_id			= "";

		/**
		 * The profile id
		 * @var string $profile_id
		 */
		protected $_profile_id		= "";

		/**
		 * The facebook client
		 * @var string $this->_facebook_client_cmodel
		 */
		protected $_facebook_client_cmodel		= "";

		/**
		 * Constructor
		 * @param array $signed_request 
		 */
		public function __construct($signed_request=array(),$app_id="",$secret_key="") {

			$this->_facebook_client_cmodel = CMODEL_FACEBOOK_CLIENT::create($app_id,$secret_key);

			if($signed_request)
				$this->populate($signed_request);
		}
		
        /**
         * Populate
         * @param array $data 
         */
		public function populate($data) {
		
			$this->_code 			= get_value($data,"code");
			$this->_algorithm 		= get_value($data,"algorithm");
			$this->_expires 		= get_value($data,"expires");
			$this->_issued_at 		= get_value($data,"issued_at");
			$this->_oauth_token 	= get_value($data,"oauth_token");
			$this->_page_id			= get_value($data,array("page","id"));
			$this->_page_liked		= get_value($data,array("page","liked"));
			$this->_page_admin		= get_value($data,array("page","admin"));
			$this->_user_country	= get_value($data,array("user","country"));
			$this->_user_locale		= get_value($data,array("user","locale"));
			$this->_user_id			= get_value($data,array("user_id"));
			$this->_profile_id		= get_value($data,array("profile_id"));
			
			preg_match("/^[^\|]+/",$this->_oauth_token,$matches);
			
			if($app_id=get_value($matches,0))
				$this->set_app_id($app_id);
		}
		
		/**
		 * Set the code
		 * @param string $code 
		 */
		public function set_code($code)			{ $this->_code = $code; }

		/**
		 * Set the algorithm
		 * @param string $algorithm 
		 */
		public function set_algorithm($algorithm)	{ $this->_algorithm = $algorithm; }

		/**
		 * Set the expires
		 * @param string $expires 
		 */
		public function set_expires($expires)		{ $this->_expires = $expires; }

		/**
		 * Set the issued at
		 * @param string $issued_at 
		 */
		public function set_issued_at($issued_at)	{ $this->_issued_at = $issued_at; }

		/**
		 * Set the oauth token
		 * @param string $oauth_token 
		 */
		public function set_oauth_token($oauth_token)	{ $this->_oauth_token = $oauth_token; }

		/**
		 * Set the page id
		 * @param string $page_id 
		 */
		public function set_page_id($page_id)		{ $this->_page_id = $page_id; }

		/**
		 * Set the page liked
		 * @param string $page_liked 
		 */
		public function set_page_liked($page_liked)	{ $this->_page_liked = $page_liked; }

		/**
		 * Set the page admin
		 * @param string $page_admin 
		 */
		public function set_page_admin($page_admin)	{ $this->_page_admin = $page_admin; }

		/**
		 * Set the user country
		 * @param string $user_country 
		 */
		public function set_user_country($user_country)	{ $this->_user_country = $user_country; }

		/**
		 * Set the user locale
		 * @param string $user_locale 
		 */
		public function set_user_locale($user_locale)	{ $this->_user_locale = $user_locale; }

		/**
		 * Set the user id
		 * @param string $user_id 
		 */
		public function set_user_id($user_id)		{ $this->_user_id = $user_id; }

		/**
		 * Set the app id
		 * @param string $app_id 
		 */
		public function set_app_id($app_id)		{ $this->_facebook_client_cmodel->set_app_id($app_id); }

		/**
		 * Set the profile id
		 * @param string $profile_id 
		 */
		public function set_profile_id($profile_id)	{ $this->_profile_id = $profile_id; }

		/**
		 * Set the secret key
		 * @param string $secret_key 
		 */
		public function set_secret_key($secret_key)	{ $this->_facebook_client_cmodel->set_secret_key($secret_key); }

		/**
		 * Returns the code
		 * @return string 
		 */
		public function get_code()				{ return $this->_code; }

		/**
		 * Returns the algorithm
		 * @return string 
		 */
		public function get_algorithm()			{ return $this->_algorithm; }

		/**
		 * Returns the expires
		 * @return string 
		 */
		public function get_expires()			{ return $this->_expires; }

		/**
		 * Returns the issued at
		 * @return string 
		 */
		public function get_issued_at()			{ return $this->_issued_at; }

		/**
		 * Returns the oauth token
		 * @return string 
		 */
		public function get_oauth_token()		{ return $this->_oauth_token; }

		/**
		 * Returns the access token
		 * @return string 
		 */
		public function get_access_token()		{ return $this->get_oauth_token(); }

		/**
		 * Returns the page id
		 * @return string 
		 */
		public function get_page_id()			{ return $this->_page_id; }

		/**
		 * Returns the page liked
		 * @return string 
		 */
		public function get_page_liked()		{ return $this->_page_liked; }

		/**
		 * Returns the page admin
		 * @return string 
		 */
		public function get_page_admin()		{ return $this->_page_admin; }

		/**
		 * Returns the user country
		 * @return string 
		 */
		public function get_user_country()		{ return $this->_user_country; }

		/**
		 * Returns the user locale
		 * @return string 
		 */
		public function get_user_locale()		{ return $this->_user_locale; }

		/**
		 * Returns the user id
		 * @return string 
		 */
		public function get_user_id()			{ return $this->_user_id; }

		/**
		 * Returns the app id
		 * @return string 
		 */
		public function get_app_id()			{ return $this->_facebook_client_cmodel->get_app_id(); }

		/**
		 * Returns the profile id
		 * @return string 
		 */
		public function get_profile_id()		{ return $this->_profile_id; }

		/**
		 * Returns the secret key
		 * @return string 
		 */
		public function get_secret_key()		{ return $this->_facebook_client_cmodel->get_secret_key(); }

		/**
		 * Checks whether is page liked
		 * @return boolean 
		 */
		public function is_page_liked()			{ return $this->get_page_liked(); }

		/**
		 * Checks whether is page admin
		 * @return boolean 
		 */
		public function is_page_admin()			{ return $this->get_page_admin(); }

		/**
		 * Checks whether is valid
		 * @return boolean 
		 */
		public function is_valid()				{ return is_numeric($this->_facebook_client_cmodel->get_app_id()); }

		/**
		 * create client
		 * @return object 
		 */
		public function create_client() {
			
			if($this->get_access_token())
				$this->_facebook_client_cmodel->set_access_token($this->get_access_token());

			if(!$this->_facebook_client_cmodel->get_access_token() && $this->get_code())
				$this->process();

			return $this->_facebook_client_cmodel;
		}

		public function persist_access_token() {
			$this->create_client();
		}

		public function process() {
			
			$facebook_authorization_cmodel = new BASE_CMODEL_FACEBOOK_AUTHORIZATION($this->_facebook_client_cmodel);
			$access_token = $facebook_authorization_cmodel->get_access_token($this->get_code());

			$this->_facebook_client_cmodel->set_access_token($access_token);
		}
	}