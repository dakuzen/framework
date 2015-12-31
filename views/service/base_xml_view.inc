<?
	class BASE_VIEW_SERVICE_BASE_XML extends VIEW {

		protected $_xml_writer_util = null;
		protected $_response		= "";

		public function __construct() {

			$this->set_route("service","base_xml");
			$this->disable_authorization();
			$this->type_blank();
			
			DEBUG_UTIL::enable_format_html();
			$this->_xml_writer_util = XML_WRITER_UTIL::create();
		}

		public function get_soap_service() { return $this->_soap_service_cmodel; }
			
		public function open($name) 	{ return $this->_xml_writer_util->open($name); }
		public function close($name)	{ return $this->_xml_writer_util->close($name); }

		public function add($name,$value)	{ return $this->_xml_writer_util->add($name,$value); }			 	
		
		public function post_init() {
			$this->_response = $this->_xml_writer_util->xml();
			echo $this->_response;
		}

		public function get_request_xpath() { 
			$xml_reader_util = XML_READER_UTIL::create();
			$xml_reader_util->set_xml($this->get_request());
			return $xml_reader_util->get_dom_xpath();
		}

		public function get_response() 		{ return $this->_response; }
		public function get_request() 		{ return @file_get_contents("php://input"); }
		public function get_method() 		{ return get_value($_SERVER,"REQUEST_METHOD"); }
		public function has_code_success() 	{ return $this->get_code()==200; }
		public function get_code() 			{ return get_value($_SERVER,"REDIRECT_STATUS"); }
		public function get_message()		{ }
	}