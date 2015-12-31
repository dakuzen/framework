<?
	/**
        * Class THUMB_PROVIDER_THUMBALIZR  
        *
        * @package Library
        * @subpackage Thumbnailer
        */
	class THUMB_PROVIDER_THUMBALIZR extends THUMB_PROVIDER {
	
                /**
                 * thumb response
                 * @var object 
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
                 * Return the url
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
                 * process
                 * @param string $url
                 * @param string $file
                 * @param integer $queue_id
                 * @return string 
                 */
		function _process($url,$file,$queue_id=null) { 
			
			$this->_thumb_response = new THUMB_RESPONSE();

			try {	
				$service_url = $this->get_url($url);
				
				if($this->is_debug())
					p("Service URL: ".$service_url);
				
				$this->_thumb_response->set_request($service_url);
				
				$http_transport_util = new HTTP_TRANSPORT_UTIL(HTTP_TRANSPORT_UTIL::TRANSPORT_TYPE_FOPEN);
				$http_transport_util->get($service_url);
				
				$response = $http_transport_util->get_http_response();
				
				$this->_thumb_response->set_response(print_r($response->get_header(),true));
			
				$status = $response->get_header()->get_field("X-Thumbalizr");
			
				if(preg_match("/,QUEUED$/",$status)) 
					$this->_thumb_response->queued();
					
				elseif(preg_match("/,OK$/",$status)) 
					$this->_thumb_response->completed();
					
				elseif(preg_match("/,FAILED$/",$status)) 
					$this->_thumb_response->failed();	

				
			} catch(Exception $e) {
				$this->_thumb_response->set_error_message($e->getMessage());
			}
			
			return $this->_thumb_response;							
		}
		
                /**
                 * queue
                 * @param string $url 
                 */
		function queue($url) { 

		}
	}
	
	
	