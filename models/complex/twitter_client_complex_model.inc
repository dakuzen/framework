<?
    require_once(DIR_FRAMEWORK."packages/codebird/src/codebird.php");

    /**
    * Class BASE_CMODEL_TWITTER_CLIENT
    *
    * @package Framework
    * @subpackage Cmodels
    */

				
	/*	
	 *	$this->_twitter_client_cmodel = BASE_CMODEL_TWITTER_CLIENT::create("****","****");
	 *	
	 *	try {				
	 *		$this->_twitter_client_cmodel->authenticate_application();
	 *	} catch(Exception $exception) {}
	 *
	 *	if(!$this->_twitter_client_cmodel->get_bearer_token())
	 *		return true;
	 *
	 *	do {
	 *
	 *		try {
	 *
	 *			$next 		= false;
	 *			$exception 	= null;
	 *
	 *			$url = $this->get_tweet_url($channel_cmodel,100);
	 *
	 *			$response = $this->_twitter_client_cmodel->search_tweets($url,true);
	 *
	 *			if($response) {
	 *
	 *				foreach($response->statuses as $status) {
	 *
	 *					$date = DB::get_date_time(TIME_UTIL::totime($status->created_at));
	 *
	 *				}
	 *
	 *				$channel_cmodel
	 *					->set_update_pointer($response->search_metadata->max_id_str);
	 *				
	 *				$next = @$response->search_metadata->next_results;
	 *			}
	 *			
	 *		} catch(Exception $exception) {}
	 *
	 *		if($exception)				
	 *			throw $exception;
	 *		
	 *	} while($next);
	 */

	class BASE_CMODEL_TWITTER_CLIENT extends \Codebird\Codebird {

		protected $_response = "";
		
		public function get_request_oauth_token()			{ return @get_value($_SESSION,"twitter_request_oauth_token"); }
		public function get_request_oauth_token_secret()	{ return @get_value($_SESSION,"twitter_request_oauth_token_secret"); }

		public function get_oauth_token()					{ return @get_value($_SESSION,"twitter_oauth_token"); }
		public function get_oauth_token_secret() 			{ return @get_value($_SESSION,"twitter_oauth_token_secret"); }
		public function get_response() 						{ return $this->_response; }
		public function get_bearer_token() 					{ return self::$_oauth_bearer_token; }

		public function is_authenticated() 					{ return $this->get_oauth_token() && $this->get_oauth_token_secret(); }

		public function create($consumer_key,$secret_key,$oauth_token="",$oauth_token_secret="") {

			BASE_CMODEL_TWITTER_CLIENT::setConsumerKey($consumer_key,$secret_key);

			$instance = MISC_UTIL::cast(parent::getInstance(),"BASE_CMODEL_TWITTER_CLIENT");
			
			$oauth_token 		= $oauth_token ? $oauth_token : $instance->get_oauth_token();
			$oauth_token_secret = $oauth_token_secret ? $oauth_token_secret : $instance->get_oauth_token_secret();

			$instance->setToken($oauth_token,$oauth_token_secret);

			return $instance;
		}

		public function authenticate_application() {
			
			$reply = @$this->oauth2_token();

			if(!isset($reply->access_token))
				throw new Exception("Authenticate application access token failed");

			$token = $reply->access_token;

			\Codebird\Codebird::setBearerToken($token);

			return $token;
		}

		public function request_token($callback) {

			$this->setToken("","");

			$reply = $this->oauth_requestToken(array("oauth_callback"=>$callback));

			$this->setToken($reply->oauth_token,$reply->oauth_token_secret);
			
			$_SESSION["twitter_request_oauth_token"] = $reply->oauth_token;
			$_SESSION["twitter_request_oauth_token_secret"] = $reply->oauth_token_secret;
		}

		public function access_token($oauth_verifier) {

		    $this->setToken($this->get_request_oauth_token(),$this->get_request_oauth_token_secret());
  
		    $reply = $this->oauth_accessToken(array("oauth_verifier"=>$oauth_verifier));

		    $_SESSION["twitter_oauth_token"]				= $reply->oauth_token;
		    $_SESSION["twitter_oauth_token_secret"]			= $reply->oauth_token_secret;
			$_SESSION["twitter_request_oauth_token"]		= "";
			$_SESSION["twitter_request_oauth_token_secret"] = "";
		    
		    $this->setToken($reply->oauth_token,$reply->oauth_token_secret);

		    return $reply;
		}

		protected function _callApi($httpmethod, $method, $method_template, $params = array(), $multipart = false, $app_only_auth = false) {
			$response = parent::_callApi($httpmethod,$method,$method_template,$params,$multipart,$app_only_auth);

			if($response->httpstatus!=200) {
				
				if(!$response)
					throw new TwitterClientException("Empty response",0,$response->httpstatus);

				if(isset($response->errors) && ($error=get_value($response->errors,0)))
					throw new TwitterClientException($error->message,$error->code,$response->httpstatus);

				throw new TwitterClientException(print_r($response,true),0,$response->httpstatus);
			}

			unset($response->httpstatus);
			
			return $response;
		}

		public function get_authorize_url() {
			return $this->oauth_authorize();
		}

		public function _parseApiReply($method,$reply) {
			$this->_response = get_value(explode("\r\n\r\n", $reply, 2),1);			
			return parent::_parseApiReply($method,$reply);
		}		
	}

	class TwitterClientException extends Exception {

		public function __construct($message,$code,$http_code) {
			parent::__construct($message,$code);
		}
	}