<?

	class BASE_CMODEL_SOAP_SERVICE  {

		protected $_soap_server = null;
		protected $_request		= "";
		protected $_response	= "";
		
		public function __construct($wsdl=null,$options=array()) {

			if(!extension_loaded("soap"))
				throw new Exception("Soap extension not loaded");

			$options["cache_wsdl"] 		=  WSDL_CACHE_NONE;
			$options["soap_version"] 	=  SOAP_1_2;
			$options["uri"] 			=  "http://MD.org/";
			$options["features"] 		=  SOAP_SINGLE_ELEMENT_ARRAYS;

			$this->_soap_server = new BASE_CMODEL_SOAP_SERVER($wsdl,$options);
		}

		public function set_class($class) 		{ $this->_soap_server->setClass($class); return $this; }
		public function add_function($function) { $this->_soap_server->addFunction($function); return $this; }
		public function set_object($object) 	{ $this->_soap_server->setObject($object); return $this; }

		public function get_request()			{ return $this->_request; }
		public function get_response()			{ return $this->_response; }
		
		public function handle() {
		   
		    global $HTTP_RAW_POST_DATA;
  
		    if(isset($HTTP_RAW_POST_DATA) && $HTTP_RAW_POST_DATA)
		        $this->_request = $HTTP_RAW_POST_DATA;
		    
		    $this->_request = implode(" ",file("php://input"));

		   	ob_start();

			$this->_soap_server->handle();

			$this->_response = ob_get_contents();
		}
	}

	if(!extension_loaded("soap"))
		throw new Exception("Soap extension not loaded");	

	class BASE_CMODEL_SOAP_SERVER extends SOAPServer {}

	class SOAP_OBJECT {
		static function create() {
			return new SOAP_OBJECT();
		}
	}