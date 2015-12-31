<?
	/**
        * Class THUMB_PROVIDER_BLUGA  
        *
        * @package Library
        * @subpackage Thumbnailer
        */
	class THUMB_PROVIDER_BLUGA extends THUMB_PROVIDER {
	
                /**
                 * the thumb response
                 * @var object $thumb_response
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
                 * Fetch
                 * @param string $file
                 * @param string $url
                 * @param integer $queue_id
                 * @return boolean
                 */
		function fetch($file,$url,$queue_id=null) {
		
			$queue_ids = is_array($queue_id) ? $queue_id : array($queue_id);
			
			$xml_writer = new XML_WRITER_UTIL();

			$xml_writer->startElement("webthumb");

			$xml_writer->addElement("apikey",$this->get_api_key());

			$xml_writer->startElement("fetch");

			if($queue_id) {
				
				if($this->is_debug())
					p("Fetch Request Queue IDs: ".$queue_ids);
				
				foreach($queue_ids as $queue_id)
					$xml_writer->addElement("job",$queue_id);
			} else {
				
				if($this->is_debug())
					p("Fetch Request URL: ".$url);
				
				$xml_writer->addElement("url",$url);
			}
			
			$xml_writer->addElement("size","custom");
			
			$xml_writer->endElement();

			$xml_writer->endElement();

			$request_xml = $xml_writer->getString();

			if($this->is_debug())
				p("Fetch Request: ".$request_xml);

			$this->_thumb_response->set_request($request_xml);

			$http_transport_util = new HTTP_TRANSPORT_UTIL(HTTP_TRANSPORT_UTIL::TRANSPORT_TYPE_FOPEN);
			$has_success = $http_transport_util->post("http://webthumb.bluga.net/api.php",$request_xml,array("Content-Type"=>"application/x-www-form-urlencoded"));

			$body = $http_transport_util->get_body();
			
			$has_success = FILE_UTIL::put_file_contents($file,$body,$error_message);
				
			if($has_success)
				$this->_thumb_response->completed();
			
			else {
				$this->_thumb_response->failed();
				$this->set_error_message("Failed to save image to disk: ".$error_message);
			}

			return $has_success;
		}

                /**
                 * Status
                 * @param string $url
                 * @param integer $queue_id
                 * @return string 
                 */
		function status($url,$queue_id=null) {
		
			$queue_ids = is_array($queue_id) ? $queue_id : array($queue_id);
			
			$xml_writer = new XML_WRITER_UTIL();

			$xml_writer->startElement("webthumb");

			$xml_writer->addElement("apikey",$this->get_api_key());

			$xml_writer->startElement("status");

			if($queue_id) {
				
				if($this->is_debug())
					p("Status Request Queue IDs: ".$queue_ids);
				
				foreach($queue_ids as $queue_id)
					$xml_writer->addElement("job",$queue_id);
			} else {
				
				if($this->is_debug())
					p("Status Request URL: ".$url);
				
				$xml_writer->addElement("url",$url);
			}
			
			$xml_writer->endElement();

			$xml_writer->endElement();

			$request_xml = $xml_writer->getString();

			if($this->is_debug())
				p("Status Request: ".$request_xml);

			$this->_thumb_response->set_request($request_xml);

			$http_transport_util = new HTTP_TRANSPORT_UTIL(HTTP_TRANSPORT_UTIL::TRANSPORT_TYPE_FOPEN);
			$has_success = $http_transport_util->post("http://webthumb.bluga.net/api.php",$request_xml,array("Content-Type"=>"application/x-www-form-urlencoded"));

			$response_xml = $http_transport_util->get_body();

			if($this->is_debug())
				p("Status Response: ".$response_xml);				
			
			$this->_thumb_response->set_response($response_xml);
			
			return $response_xml;
		}
		
		/**
                 * process
                 * @param string $url
                 * @param string $file
                 * @param integer $queue_id
                 * @return object 
                 */
		function _process($url,$file,$queue_id=null) { 
			
			$this->_thumb_response = new THUMB_RESPONSE();

			try {	
				$response_xml = $this->status($url,$queue_id);
					
				$xml_reader_util = new XML_READER_UTIL();
				$xml_reader_util->set_xml($response_xml);
				$xpath = $xml_reader_util->get_dom_xpath();

				$in_process = $xpath->exists('/webthumb/jobStatus/status[@inProcess="1"]');

				if($in_process) {
					$this->_thumb_response->processing();
					return $this->_thumb_response;
				}

				$xml_reader_util = new XML_READER_UTIL();
				$xml_reader_util->set_xml($response_xml);
				$xpath = $xml_reader_util->get_dom_xpath();

				$pickup = $xpath->select_attribute('/webthumb/jobStatus/status[@pickup]',"pickup");
				
				if($pickup) {
					$has_success = $this->fetch($file,$url,$queue_id);
					
					if($has_success) 
						return $this->_thumb_response;
				}
				
				$response_xml = $this->queue($url);

				$xml_reader_util = new XML_READER_UTIL();
				$xml_reader_util->set_xml($response_xml);
				$xpath = $xml_reader_util->get_dom_xpath();

				$queue_id = $xpath->select_value('/webthumb/jobs/job');

				if($queue_id) {
					$this->_thumb_response->set_queue_id($queue_id);
					$this->_thumb_response->queued();
				} else
					$this->_thumb_response->set_error_message($response_xml);
			
			} catch(Exception $e) {
				$this->_thumb_response->set_error_message($e->getMessage());
			}
			
			return $this->_thumb_response;							
		}
		
                /**
                 * queue
                 * @param string $url
                 * @return string 
                 */
		function queue($url) { 
			
			$xml_writer = new XML_WRITER_UTIL();

			$xml_writer->startElement("webthumb");

			$xml_writer->addElement("apikey",$this->get_api_key());

			$xml_writer->startElement("request");

			$xml_writer->addElement("url",$url);
			$xml_writer->addElement("outputType",$this->get_format());
			
			if($this->get_browser_width())
				$xml_writer->addElement("width",$this->get_browser_width());
				
			if($this->get_browser_height())
				$xml_writer->addElement("height",$this->get_browser_height());
			
			if($this->get_height() && $this->get_width())
				$xml_writer->addElement("customThumbnail","",array("width"=>$this->get_width(),"height"=>$this->get_height()));
			
			if($this->is_fullpage())
				$xml_writer->addElement("fullthumb",1);

			$xml_writer->endElement();

			$xml_writer->endElement();

			$request_xml = $xml_writer->getString();

			if($this->is_debug())
				p("Queue Request: ".$request_xml);

			$this->_thumb_response->set_request($request_xml);

			$http_transport_util = new HTTP_TRANSPORT_UTIL(HTTP_TRANSPORT_UTIL::TRANSPORT_TYPE_FOPEN);
			$http_transport_util->post("http://webthumb.bluga.net/api.php",$request_xml,array("Content-Type"=>"application/x-www-form-urlencoded"));
			
			$response_xml = $http_transport_util->get_body();
			
			$this->_thumb_response->set_response($response_xml);
			
			if($this->is_debug())
				p("Queue Response: ".$response_xml);			
			
			return $response_xml;
		}
	}
	
	
	