<?
	/**
        * Class THUMB_PROVIDER_SHRINKTHEWEB  
        *
        * @package Library
        * @subpackage Thumbnailer
        */
	class THUMB_PROVIDER_SHRINKTHEWEB extends THUMB_PROVIDER {
		
                /**
                 * Constructor
                 * @param string $api_key
                 * @param string $account_key 
                 */
		function __construct($api_key,$account_key) {
			parent::__construct($api_key,$account_key);
		}		
		
                /**
                 * Return the url
                 * @param string $url
                 * @return string 
                 */
		function get_url($url) {
			
			$args["stwu"] 		= $this->get_account_key();
			$args["STWAccessKeyId"] = $this->get_api_key();
			$args["stwUrl"] 	= urlencode($url);
			$args["Size"] 		= "lg";
			$args["Service"] 	= "ShrinkWebUrlThumbnail";
			$args["Action"] 	= "Thumbnail";
			
			return "http://www.shrinktheweb.com/xino.php?".ARRAY_UTIL::get_imploded_string($args,"&","","=");
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
				
			$this->_thumb_response->set_request($image_url);
			
			$http_transport_util = new HTTP_TRANSPORT_UTIL(HTTP_TRANSPORT_UTIL::TRANSPORT_TYPE_FOPEN);
			$http_transport_util->get($image_url);
			
			$body = $http_transport_util->get_body();
			
			$this->_thumb_response->set_response($body);
			
			$xml_reader_util = new XML_READER_UTIL();
			$xml_reader_util->set_xml($body);
			$xpath = $xml_reader_util->get_dom_xpath();

			$dl_url = $xpath->select_value('/stw:ThumbnailResponse/stw:Response/stw:ThumbnailResult/stw:Thumbnail[@Exists="true"]');

			if($this->is_debug())
				p($response);
			
			$has_success = false;
			
			$this->_thumb_response->failed();
			
			if($dl_url) {
							
				$http_transport_util = new HTTP_TRANSPORT_UTIL(HTTP_TRANSPORT_UTIL::TRANSPORT_TYPE_FOPEN);
				$has_success = $http_transport_util->send($dl_url);

				if($has_success) {

					$contents = $http_transport_util->get_http_response()->get_body();

					$has_success = FILE_UTIL::put_file_contents($file,$contents,$error_message);
					
					if($has_success)
						$this->_thumb_response->completed();
				} else 					
					$this->_thumb_response->queued();
			}
			
			return $this->_thumb_response;

			/*
			<stw:thumbnailresponse xmlns:stw="http://www.shrinktheweb.com/doc/stwresponse.xsd">
				<stw:response>
					<stw:thumbnailresult>
						<stw:thumbnail exists="true">http://www.shrinktheweb.com/thumbs/temp/f3/77/ad/95/8cd5dfb37bec43b70ed0066a.jpg</stw:thumbnail>
						<stw:thumbnail verified="true">delivered</stw:thumbnail>
					</stw:thumbnailresult>
					<stw:responsestatus>
						<stw:statuscode>Success</stw:statuscode>

					</stw:responsestatus>
					<stw:responsetimestamp>
						<stw:statuscode>1263915974</stw:statuscode>
					</stw:responsetimestamp>
					<stw:responsecode>
						<stw:statuscode>HTTP/1.1 200 OK</stw:statuscode>
					</stw:responsecode>
					<stw:categorycode>

						<stw:statuscode>Family</stw:statuscode>
					</stw:categorycode>
					<stw:quota_remaining>
						<stw:statuscode>179</stw:statuscode>
					</stw:quota_remaining>
				</stw:response>
			</stw:thumbnailresponse>
			*/
		}				
	}
	
	
	