<?
	/**
        * Class THUMB_PROVIDER_SNAPCASA  
        *
        * @package Library
        * @subpackage Thumbnailer
        */
	class THUMB_PROVIDER_SNAPCASA extends THUMB_PROVIDER {
		
                /**
                 * Constructor
                 * @param string $user_code
                 * @param string $premium_key 
                 */
		function __construct($user_code,$premium_key="") {
			parent::__construct($user_code,$premium_key);
		}
		
                /**
                 * Process
                 * @param string $url
                 * @param string $file
                 * @return string 
                 */
		function _process($url,$file) {
		
			$image_url = $this->get_url($url);
		
			if($this->is_debug())
				p($image_url);
					
			$http_transport_util = new HTTP_TRANSPORT_UTIL(HTTP_TRANSPORT_UTIL::TRANSPORT_TYPE_FOPEN);
			$http_transport_util->get($image_url);			
			
			$response = $http_transport_util->get_http_response();
			
			$this->_thumb_response->set_response(print_r($response->get_header(),true));
			$this->_thumb_response->set_request($url." - ".$image_url);
						
			$image_status = (int)$response->get_header()->get_field("Snapshot-Status");
				
			$this->_thumb_response->failed();
			
			if($image_status===0)
				$this->_thumb_response->queued();
				
			if($image_status===1) {
				$contents = $response->get_body();							
				$has_success = FILE_UTIL::put_file_contents($file,$contents,$error_message);
				
				if($has_success) {
					$this->_thumb_response->completed();
					$this->_thumb_response->set_error_message("Failed to save image to disk: ".$error_message);
				}
			}
			
			if($image_status===2)
				$this->_thumb_response->unavailable();
				
			if($image_status===3)
				$this->_thumb_response->invalid();
						
			return $this->_thumb_response;
		}
		
                /**
                 * Return url
                 * @param string $url
                 * @return string
                 */
		function get_url($url) {
			
			$args = array();
			$args["code"] = $this->get_api_key();
			$args["width"] = $this->get_width();

			if($this->get_fullpage())
				$args["page"] = 1;
			elseif($this->get_height())
				$args["height"] = $this->get_height();

			if($this->get_account_key())
				$args["key"] = $this->get_account_key();

			$args["file"] 	= "JPG";
			$args["comp"]	= $this->get_quality();
			$args["wait"] 	= 1;
			$args["update"] = 1;
			$args["url"] 	= urlencode($url); //Make sure this is the last parameter sent in the GET request

			return "http://snapcasa.com/get.aspx?".ARRAY_UTIL::get_imploded_string($args,"&","","=");
		}			
	}
	