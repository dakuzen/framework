<?
	class BASE_VIEW_API_BASE extends VIEW {
		
		protected $_api_cmodel		= null;
		protected $_username 		= "";
		protected $_password		= "";
		
		function __construct() {
			$this->set_route("api","base");
			$this->type_blank();
			
			$this->_username = get_value($_SERVER,"PHP_AUTH_USER");
			$this->_password = get_value($_SERVER,"PHP_AUTH_PW");
		
			DEBUG_UTIL::enable_format_html();
			
			$this->_api_response_cmodel = new BASE_CMODEL_API_RESPONSE();
		}
		
		function init() {
			
		}

		function post_init() {
			$this->_api_response_cmodel->render();	
		}
		
		function get_api_response() 		{ return $this->_api_response_cmodel; }
		function get_username() 		{ return $this->_username; }
		function get_password() 		{ return $this->_password; }
	}