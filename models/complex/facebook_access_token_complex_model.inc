<?php
        /**
        * Class BASE_CMODEL_FACEBOOK_ACCESS_TOKEN
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_FACEBOOK_ACCESS_TOKEN {
                
                /**
                 * The algorithm
                 * @var string $algorithm
                 */
		protected $_algorithm		= "";
                
                /**
                 * The expires
                 * @var string $expires
                 */
		protected $_expires		= "";
		
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
		protected $_page_id		= "";
                
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
                 * The ser locale
                 * @var string $user_locale
                 */
		protected $_user_locale		= "";
                
                /**
                 * The user id
                 * @var string $user_id
                 */
		protected $_user_id		= "";
                
                /**
                 * The app id
                 * @var string $app_id
                 */
		protected $_app_id		= "";
		
                /**
                 * Constructor
                 * @param array $signed_request 
                 */
		public function __construct($signed_request=array()) {
			if($signed_request)
				$this->populate($signed_request);			
		}
		
                /**
                 * Populate
                 * @param array $data 
                 */
		public function populate($data) {
		
			$this->_algorithm 	= get_value($data,"algorithm");
			$this->_expires 	= get_value($data,"expires");
			$this->_issued_at 	= get_value($data,"issued_at");
			$this->_oauth_token 	= get_value($data,"oauth_token");
			$this->_page_id		= get_value($data,array("page","id"));
			$this->_page_liked	= get_value($data,array("page","liked"));
			$this->_page_admin	= get_value($data,array("page","admin"));
			$this->_user_country	= get_value($data,array("user","country"));
			$this->_user_locale	= get_value($data,array("user","locale"));
			$this->_user_id		= get_value($data,array("user_id"));
			
			preg_match("/^[^\|]+/",$this->_oauth_token,$matches);
			
			$this->_app_id 		= get_value($matches,0);
		}
		
                /**
                 * Checks whether is valid
                 * @return boolean 
                 */
		public function is_valid()			{ return is_numeric($this->_app_id); }
		
                /**
                 * Sets algorithm
                 * @param string $algorithm 
                 */
		public function set_algorithm($algorithm)	{ $this->_algorithm = $algorithm; }
                
                /**
                 * Sets expires
                 * @param string $expires 
                 */
		public function set_expires($expires)		{ $this->_expires = $expires; }
                
                /**
                 * Sets issued at
                 * @param string $issued_at 
                 */
		public function set_issued_at($issued_at)	{ $this->_issued_at = $issued_at; }
                
                /**
                 * Sets oauth token
                 * @param string $oauth_token 
                 */
		public function set_oauth_token($oauth_token)	{ $this->_oauth_token = $oauth_token; }
                
                /**
                 * Sets page id
                 * @param string $page_id 
                 */
		public function set_page_id($page_id)		{ $this->_page_id = $page_id; }
                
                /**
                 * Sets page liked
                 * @param string $page_liked 
                 */
		public function set_page_liked($page_liked)	{ $this->_page_liked = $page_liked; }
                
                /**
                 * Sets page admin
                 * @param string $page_admin 
                 */
		public function set_page_admin($page_admin)	{ $this->_page_admin = $page_admin; }
                
                /**
                 * Sets user country
                 * @param string $user_country 
                 */
		public function set_user_country($user_country)	{ $this->_user_country = $user_country; }
                
                /**
                 * Sets user locale
                 * @param string $user_locale 
                 */
		public function set_user_locale($user_locale)	{ $this->_user_locale = $user_locale; }
                
                /**
                 * Sets user id
                 * @param string $user_id 
                 */
		public function set_user_id($user_id)		{ $this->_user_id = $user_id; }
                
                /**
                 * Sets app_id
                 * @param string $app_id 
                 */
		public function set_app_id($app_id)		{ $this->_app_id = $app_id; }
		
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
		public function get_app_id()			{ return $this->_app_id; }
	}