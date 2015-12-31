<?
	class BASE_VIEW_APPLICATION_ERROR extends VIEW {

		protected $_message = "An error has occured. Please contact customer service";

		function __construct() {
			$this->set_route("application","error");
			$this->disable_authorization();			
		}

		function init() {			
			$this->set_var("message",$this->_message);
		}

		public function set_message($message) { $this->_message = $message; }
	}