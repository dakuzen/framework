<?
	class BASE_VIEW_SERVICE_BASE_SOAP extends VIEW {

		public function __construct($wsdl="",$options=array()) {

			$this->set_route("service","base_soap");
			$this->disable_authorization();
			$this->type_blank();
			
			DEBUG_UTIL::enable_format_html();
	
			$this->_soap_service_cmodel = new CMODEL_SOAP_SERVICE($wsdl,$options);
		}

		public function get_soap_service() { return $this->_soap_service_cmodel; }
			
		public function set_object($object) {
			$this->_soap_service_cmodel->set_object($object);
		}
		
		public function post_init() {
			$this->_soap_service_cmodel->handle();
		}

		public function get_response() 		{ return $this->_soap_service_cmodel->get_response(); }
		public function get_request() 		{ return $this->_soap_service_cmodel->get_request(); }
		public function get_method() 		{ return get_value($_SERVER,"REQUEST_METHOD"); }
		public function has_code_success() 	{ return $this->get_code()==200; }
		public function get_code() 			{ return get_value($_SERVER,"REDIRECT_STATUS"); }
		public function get_message()		{ }
	}