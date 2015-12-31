<?
    /**
    * BASE_CMODEL_FACEBOOK_CLIENT class file
    *
    * @package Framework
    * @subpackage Cmodels
    */

    require_once(DIR_FRAMEWORK."packages/Facebook/php/base_facebook.php");
    
    /**
    * Class BASE_CMODEL_FACEBOOK_CLIENT
    *
    * @package Framework
    * @subpackage Cmodels
    */
	
	class BASE_CMODEL_FACEBOOK_CLIENT extends BaseFacebook {
                
        /**
         * The kSupportedKeys
         * @var array $kSupportedKeys 
         */
		protected static $kSupportedKeys 	= array("state","code","access_token","user_id");

        /**
         * Constructor
         * @param integer $app_id
         * @param string $secret_key 
         */
		public function __construct($app_id,$secret_key,$access_token=null) {

			self::$CURL_OPTS[CURLOPT_CONNECTTIMEOUT] 	= 30;
			self::$CURL_OPTS[CURLOPT_TIMEOUT]			= 120;

			$this->set_app_id($app_id);
			$this->set_secret_key($secret_key);
			
			if($access_token)
				$this->setAccessToken($access_token);
		}

		public function set_app_id($app_id) 		{ $this->setAppId($app_id); }
		public function set_secret_key($secret_key) { $this->setAppSecret($secret_key); }

		public function get_app_id() 				{ return $this->getAppId(); }
		public function get_secret_key() 			{ return $this->getAppSecret(); }		

        /**
         * Create
         * 
         * @param integer $app_id
         * @param string $secret_key
         * @return BASE_CMODEL_FACEBOOK_CLIENT 
         */
		public static function create($app_id=null,$secret_key=null) {
			$class = get_called_class();
			return new $class($app_id,$secret_key);
		}

        /**
         * Returns access token
         * @return mixed
         * @todo review return type 
         */
		public function get_access_token() {
			return $this->getPersistentData('access_token');
		}

		public function getAccessToken() { return $this->get_access_token(); }

		public function getUserAccessToken() { return $this->get_access_token(); }

		public function setAccessToken($access_token) { return $this->set_access_token($access_token); }

        /**
         * set access token
         * @param string $access_token 
         */
		public function set_access_token($access_token) { 
			$this->setPersistentData("access_token",$access_token);
		}	

        /**
         * Check whether is has session
         * @return boolean 
         */
		public function has_session() 			{ return !is_empty($this->getUser()); }
        
        /**
         * Returns the session
         * @return  boolean
         */
		public function getSession() 			{ return !is_empty($this->getUser()); }

        /**
         * Select
         * @param string $fql
         * @return array 
         */
		public function select($fql) {
			$response = get_value($this->_graph("/fql?q=".urlencode($fql)),"data",array());
			return is_array($response) ? $response : array();
		}

        /**
         * select row
         * @param string $fql
         * @return array 
         */
		public function select_row($fql) {
			return get_value($this->select($fql),0,array());
		}	
		
        /**
         * Exists
         * @param string $fql
         * @return boolean 
         */
		public function exists($fql) {
			return !is_empty($this->select_one($fql));
		}
		
        /**
         * Select one
         * @param string $fql
         * @return null 
         */
		public function select_one($fql) {
			$value = get_value($this->select($fql),0);

			if(!$value) 
				return null;

			return get_value(array_values($value),0);
		}

        /**
         * Select column
         * @param string $fql
         * @param string $field
         * @param string $key_field
         * @return array 
         */
		public function select_column($fql,$field,$key_field=null) {
			return ARRAY_UTIL::get_list_from_array($this->select($fql),$field,$key_field);
		}
			
        /**
         * Graph
         *
         * This function is a copy from Facebook base client due to the json_decode BIGINT string issue
		 * http://stackoverflow.com/questions/2907806/handling-big-user-ids-returned-by-fql-in-php
		 * When the servers have PHP 5.4 installed it is safe to remove this override
         *
         * @param string $path
         * @param string $method
         * @param array $params
         * @return string 
         */

		protected function _graph($path, $method = 'GET', $params = array()) {
			
			if(is_array($method) && empty($params)) {
				$params = $method;
				$method = 'GET';
			}
			
			$params['method'] = $method; // method override as we always do a POST
			
			$request = $this->_oauthRequest($this->getUrl('graph', $path),$params);

			$result = json_decode(preg_replace('/("\w+"):(\d+)/', '\\1:"\\2"',$request), true);

			// results are returned, errors are thrown
			if (is_array($result) && isset($result['error']))
				$this->throwAPIException($result);
			
			return $result;
		}	

		protected function getPersistentData($key, $default=false) {	

			if(!in_array($key, self::$kSupportedKeys)) 
				return $default;
				
			$value = USER_SESSION::get_instance()->get_setting("fb_persistense");
	
			return get_value($value,$key,$default);
		}
		
		protected function setPersistentData($key, $value) {
			
			if(!in_array($key, self::$kSupportedKeys)) 
				return;
				
			$fb_persistense = $this->get_fb_persistense();
			
			$fb_persistense[$key] = $value;
			
			USER_SESSION::get_instance()->set_setting("fb_persistense",$fb_persistense);
		}
		
		protected function clearPersistentData($key) {		
			if(!in_array($key, self::$kSupportedKeys))
				return;
			 
			$this->setPersistentData($key,null);
		}
		
		public function clearAllPersistentData() {
			USER_SESSION::get_instance()->remove_setting("fb_persistense");
		}
		
		private function get_fb_persistense() {
			$fb_persistense = USER_SESSION::get_instance()->get_setting("fb_persistense");
			return is_array($fb_persistense) ? $fb_persistense : array();
		}

        public function get_user_id() 							{ return BASE_MODEL_FACEBOOK::get_active_facebook_user_id($this->getAppId()); }
	}
        
       