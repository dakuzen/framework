<?
	/**
        * Class THUMB_PROVIDER_PAGEGLIMPSE  
        *
        * @package Library
        * @subpackage Thumbnailer
        */
	class THUMB_PROVIDER_PAGEGLIMPSE extends THUMB_PROVIDER {
                
                /**
                 * the thumb response
                 * @var string $thumb_response
                 */
		protected $_thumb_response = null;
		
                /**
                 * Constructor
                 * @param string $api_key 
                 */
		function __construct($api_key) {
			parent::__construct($api_key,null);
			
			$this->_thumb_response = new THUMB_RESPONSE();
		}
		
                /**
                 * Exists
                 * @param string $url 
                 */
		function exists($url) {
			$this->debug();

			try {	
			
					      
				$exists_url = "http://images.pageglimpse.com/v1/thumbnails/exists?url=".urlencode($url)."&devkey=".$this->get_api_key()."&size=small";
				$exists_url = "http://images.pageglimpse.com/v1/thumbnails/exists?url=".urlencode($url)."&size=large&devkey=".$this->get_api_key();
				
				
				if($this->is_debug())
					p("Exists URL: ".$exists_url);
				
				$this->_thumb_response->set_request($exists_url);
				
				$http_transport_util = new HTTP_TRANSPORT_UTIL(HTTP_TRANSPORT_UTIL::TRANSPORT_TYPE_FOPEN);
				$http_transport_util->get($exists_url);
				
				$response = $http_transport_util->get_http_response();
				
				p($http_transport_util);
				
				$this->_thumb_response->set_response(print_r($response,true));
			
				
			} catch(Exception $e) {
				$this->_thumb_response->set_error_message($e->getMessage());
			}		
		}
		
                /**
                 * Returns the url
                 * @param string $url
                 * @return string 
                 */
		function get_url($url) {
			
			$args["api_key"] 	= $this->get_api_key();
			$args["quality"] 	= $this->get_quality();
			$args["width"] 		= $this->get_width();
			$args["height"] 	= $this->get_height();
			$args["encoding"] 	= $this->get_format();
			$args["mode"] 		= $this->is_fullpage() ? "screen" : "page";
			$args["bwidth"] 	= $this->get_browser_width();
			$args["hwidth"] 	= $this->get_browser_height();
			$args["url"] 		= urlencode($url);
			
			return "http://api.thumbalizr.com/?".ARRAY_UTIL::get_imploded_string($args,"&","","=");
		}		
		
		/**
                 * Process
                 * @param string $url
                 * @param string $file
                 * @param string $queue_id
                 * @return string 
                 */
		function _process($url,$file,$queue_id=null) { 
			
			$this->_thumb_response = new THUMB_RESPONSE();

			
			$exists = $this->exists($url);
			
			die;
			
			return $this->_thumb_response;							
		}
		
                /**
                 * queue
                 * @param string $url 
                 */
		function queue($url) { 

		}
	}
	
	
	