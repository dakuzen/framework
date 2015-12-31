<?	
	/**
	 * THUMB_PROVIDER class file 
	 *
	 * @package Library
	 * @subpackage Thumbnailer
	 */

	require_once("THUMB_OPTION.inc");
	
	/**
	 * Class THUMB_PROVIDER  
	 *
	 * @package Library
	 * @subpackage Thumbnailer
	 */
	abstract class THUMB_PROVIDER extends THUMB_OPTION {
		
		/**
		 * thumb response
		 * @var object $thumb_response
		 */
		protected $_thumb_response 	= null;
		
		const PROVIDER_SNAPCASA 	= "snapcasa";
		const PROVIDER_SHRINKTHEWEB 	= "shrinktheweb";
		const PROVIDER_BLUGA 		= "bluga";
		const PROVIDER_THUMBALIZR 	= "thumbalizr";
		const PROVIDER_PAGEGLIMPSE 	= "pageglimpse";
		
		/**
		 * Constructor
		 * @param string $api_key
		 * @param string $account_key 
		 */
		function __construct($api_key,$account_key) {
			parent::__construct($api_key,$account_key);
			
			$this->_thumb_response = new THUMB_RESPONSE();
		}
		
		/**
		 * Process
		 * @param string $url
		 * @param string $file
		 * @param string $queue_id
		 */
		abstract function _process($url,$file,$queue_id=null);
		
		/**
		 * Create shrinktheweb
		 * @param string $access_key
		 * @param string $secret_key
		 * @return mixed 
		 */
		function create_shrinktheweb($access_key,$secret_key) 	{ return self::create(self::PROVIDER_SHRINKTHEWEB,$access_key,$secret_key); }
		
		/**
		 * create snapcasa
		 * @param string $user_code
		 * @param string $premium_key
		 * @return mixed 
		 */
		function create_snapcasa($user_code,$premium_key="") 	{ return self::create(self::PROVIDER_SNAPCASA,$user_code,$premium_key); }
		
		/**
		 * create bluga
		 * @param string $api_key
		 * @return mixed 
		 */
		function create_bluga($api_key) 			{ return self::create(self::PROVIDER_BLUGA,$api_key); }
		
		/**
		 * create thumbalizr
		 * @param string $api_key
		 * @return mixed 
		 */
		function create_thumbalizr($api_key) 			{ return self::create(self::PROVIDER_THUMBALIZR,$api_key); }
		
		/**
		 * create pageglimpse
		 * @param string $api_key
		 * @return mixed 
		 */
		function create_pageglimpse($api_key) 			{ return self::create(self::PROVIDER_PAGEGLIMPSE,$api_key); }
		
		/**
		 * Create
		 * @param string $provider
		 * @param string $api_key
		 * @param string $account_key
		 * @return THUMB_PROVIDER_SHRINKTHEWEB|THUMB_PROVIDER_SNAPCASA|\THUMB_PROVIDER_BLUGA|THUMB_PROVIDER_THUMBALIZR|THUMB_PROVIDER_PAGEGLIMPSE|null 
		 */
		static function create($provider,$api_key="",$account_key="") {
			
			if($provider==self::PROVIDER_SHRINKTHEWEB) {
				require_once("THUMB_PROVIDER_SHRINKTHEWEB.inc");
				return new THUMB_PROVIDER_SHRINKTHEWEB($api_key,$account_key);
			
			} elseif($provider==self::PROVIDER_SNAPCASA) {
				require_once("THUMB_PROVIDER_SNAPCASA.inc");
				return new THUMB_PROVIDER_SNAPCASA($api_key,$account_key);
			
			} elseif($provider==self::PROVIDER_BLUGA) {
				require_once("THUMB_PROVIDER_BLUGA.inc");
				return new THUMB_PROVIDER_BLUGA($api_key);
					
			} elseif($provider==self::PROVIDER_THUMBALIZR) {
				require_once("THUMB_PROVIDER_THUMBALIZR.inc");
				return new THUMB_PROVIDER_THUMBALIZR($api_key);
			
			} elseif($provider==self::PROVIDER_PAGEGLIMPSE) {
				require_once("THUMB_PROVIDER_PAGEGLIMPSE.inc");
				return new THUMB_PROVIDER_PAGEGLIMPSE($api_key);
			}			
			return null;		
		}
		
		/**
		 * Process
		 * @param string $file
		 * @param string $url
		 * @param string $queue_id
		 * @return object 
		 */
		function process($file,$url,$queue_id=null) {
			
			$this->_is_queued = false;
			
			$has_success = false;
			
			$this->_thumb_response = new THUMB_RESPONSE();
			
			try {
				$this->_thumb_response = $this->_process($url,$file,$queue_id);
							
			} catch(Exception $e) {
				$this->_thumb_response->set_error_message($e->getMessage());
			}
			
			return $this->_thumb_response;
		}
		
		/**
		 * disable cache
		 */
		function disable_cache() 		{ $this->set_cached(false); }
		
		/**
		 * Debug 
		 */
		function debug() 			{ $this->set_debug(true); }
		
		/**
		 * enable fullpage
		 */
		function enable_fullpage() 		{ $this->set_fullpage(true); }
	}
	/**
	 * Class THUMB_RESPONSE  
	 *
	 * @package Library
	 * @subpackage Thumbnailer
	 */
	class THUMB_RESPONSE {
		
		const CODE_PROCESSING 	= 100;
		const CODE_QUEUED 	= 200;
		const CODE_UNAVAILABLE	= 300;
		const CODE_INVALID	= 400;
		const CODE_COMPLETED	= 500;
		const CODE_FAILED	= 600;
		
		/**
		 * queue ids
		 * @var array $queue_ids 
		 */
		protected $_queue_ids		= array();
		
		/**
		 * response
		 * @var object $response 
		 */
		protected $_response		= null;
		
		/**
		 * The error message
		 * @var string $error_message
		 */
		protected $_error_message	= "";
		
		/**
		 * The code
		 * @var integer 
		 */
		protected $_code		= self::CODE_INVALID;
		
		/**
		 * Checks whether is processing
		 * @return boolean
		 */
		function is_processing() 			{ return $this->get_code()==self::CODE_PROCESSING; }
		
		/**
		 * Checks whether is queued
		 * @return boolean
		 */
		function is_queued() 				{ return $this->get_code()==self::CODE_QUEUED; }
		
		/**
		 * Checks whether is unavailable
		 * @return boolean
		 */
		function is_unavailable() 			{ return $this->get_code()==self::CODE_UNAVAILABLE; }
		
		/**
		 * Checks whether is invalid
		 * @return boolean
		 */
		function is_invalid() 				{ return $this->get_code()==self::CODE_INVALID; }
		
		/**
		 * Checks whether is completed
		 * @return boolean
		 */
		function is_completed() 			{ return $this->get_code()==self::CODE_COMPLETED; }
		
		/**
		 * Checks whether is failed
		 * @return boolean
		 */
		function is_failed() 				{ return $this->get_code()==self::CODE_FAILED; }
		
		/**
		 * Processing 
		 */
		function processing() 				{ $this->set_code(self::CODE_PROCESSING); }
		
		/**
		 * Queued 
		 */
		function queued() 				{ $this->set_code(self::CODE_QUEUED); }
		
		/**
		 * Unavailable
		 */
		function unavailable() 				{ $this->set_code(self::CODE_UNAVAILABLE); }
		
		/**
		 * Invalid
		 */
		function invalid() 				{ $this->set_code(self::CODE_INVALID); }
		
		/**
		 * Completed
		 */
		function completed() 				{ $this->set_code(self::CODE_COMPLETED); }	
		
		/**
		 * Failed 
		 */
		function failed() 				{ $this->set_code(self::CODE_FAILED); }		
		
		/**
		 * Set queue ids
		 * @param array $queue_ids 
		 */
		function set_queue_ids($queue_ids)		{ $this->_queue_ids = $queue_ids; }
		
		/**
		 * Set the queue id
		 * @param integer $queue_id 
		 */
		function set_queue_id($queue_id)		{ $this->set_queue_ids(array($queue_id)); }
		
		/**
		 * Set error message
		 * @param string $error_message 
		 */
		function set_error_message($error_message)	{ $this->_error_message = $error_message; }
		
		/**
		 * Set the response
		 * @param object $response 
		 */
		function set_response($response)		{ $this->_response = $response; }
		
		/**
		 * Set the code
		 * @param string $code 
		 */
		function set_code($code)			{ $this->_code = $code; }
		
		/**
		 * Sets the request
		 * @param object $request 
		 * @todo review param type
		 */
		function set_request($request)			{ $this->_request = $request; }
		
		/**
		 * Returns queue ids
		 * @return array 
		 */
		function get_queue_ids() 			{ return $this->_queue_ids; }
		
		/**
		 * Returns the queue id
		 * @return integer 
		 */
		function get_queue_id() 			{ return get_value($this->_queue_ids,0); }
		
		/**
		 * Returns the code
		 * @return string 
		 */
		function get_code() 				{ return $this->_code; }
		
		/**
		 * Returns error message
		 * @return string 
		 */
		function get_error_message() 			{ return $this->_error_message; }
		
		/**
		 * Returns the request
		 * @return object $request 
		 * @todo review return type
		 */
		function get_request()				{ return $this->_request; }	
		
		/**
		 * Returns the response
		 * @return object 
		 */
		function get_response()				{ return $this->_response; }	
		
		/**
		 * Returns code name
		 * @return string 
		 */
		function get_code_name()			{ return get_value(self::get_code_list(),$this->get_code()); }
		
		/**
		 * Returns code list
		 * @return array 
		 */
		function get_code_list() {
			
			$code_list[self::CODE_PROCESSING]	= "Processing";
			$code_list[self::CODE_QUEUED]		= "Queued";
			$code_list[self::CODE_UNAVAILABLE]	= "Unavailable";
			$code_list[self::CODE_INVALID]		= "Invalid";
			$code_list[self::CODE_COMPLETED]	= "Completed";
			$code_list[self::CODE_FAILED]		= "Failed";
			
			return $code_list;
		}
		
	}
	
	
	