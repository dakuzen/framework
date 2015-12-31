<?
	class BASE_VIEW_SERVICE_BASE extends VIEW {

		const CODE_SUCCESS				= 200;
		const CODE_FAIL					= 400;
		const CODE_FATAL				= 500;
		
		protected $_service_response	= null;
		protected $_method				= null;
		protected $_username 			= "";
		protected $_password			= "";
		
		public function __construct() {
			$this->set_route("service","base");
			$this->type_blank();
			$this->disable_authorization();
			
			$this->_username = get_value($_SERVER,"PHP_AUTH_USER");
			$this->_password = get_value($_SERVER,"PHP_AUTH_PW");
			
			DEBUG_UTIL::enable_format_html();
			
			$this->_method = strtoupper(get_value($_SERVER,"REQUEST_METHOD"));

			$this->_service_response = new BASE_CMODEL_SERVICE_RESPONSE();
			$this->_service_response->set_code(self::CODE_FAIL);
		}
			
		public function post_init() {
			$this->_service_response->render();	
		}
		
		public function init() {
			
			try {

				if(	!method_exists($this,"request") && !method_exists($this,"post") && !method_exists($this,"get") && 
					!method_exists($this,"put") && !method_exists($this,"delete"))
						throw new Exception("Service not found",404);						

				if(method_exists($this,"request"))
					$this->request();

				elseif($this->_method=="POST") {
					
					if(!method_exists($this,"post"))
						throw new Exception("POST Method not supported",405);
						
					$this->post();
				}

				elseif($this->_method=="GET")	{
					
					if(!method_exists($this,"get"))
						throw new Exception("GET Method not supported",405);

					$this->get();
				}

				elseif($this->_method=="PUT")	{
					
					if(!method_exists($this,"put"))
						throw new Exception("PUT Method not supported",405);

					$this->put();
				}

				elseif($this->_method=="DELETE") {
					
					if(!method_exists($this,"delete"))
						throw new Exception("DELETE Method not supported",405);

					$this->delete();
				}

			} catch(Exception $e) {

				$this
					->code($e->getCode())
					->message($e->getMessage());
			}

			if($this->get_code()==self::CODE_FAIL && !$this->get_message())
				$this->message("The request cannot be fulfilled due to an unknown error");
		}

		public function response($response) 					{ return $this->_service_response->set_response($response); }
		public function code($code) 							{ return $this->_service_response->set_code($code); }
		public function message($message)						{ return $this->_service_response->set_message($message); }
		
		public function get_service_response() 					{ return $this->_service_response; }
		public function get_code() 								{ return $this->_service_response->get_code(); }
		public function get_message() 							{ return $this->_service_response->get_message(); }
		public function get_response() 							{ return $this->_service_response->get_response(); }

		public function get_username() 							{ return $this->_username; }
		public function get_method() 							{ return $this->_method; }
		public function get_password() 							{ return $this->_password; }

		public function fail($message,$code=self::CODE_FAIL) {

			if($message instanceof Exception) {
				$code = $message->getCode();
				$message = $message->getMessage();
			}

			$code = $code===false ? self::CODE_FAIL : $code;

			return $this->_service_response
								->set_code($code)
								->set_message($message); 
		}

		public function success($code=self::CODE_SUCCESS,$message="") {

			$code = $code===true ? self::CODE_SUCCESS : $code;

			return $this->_service_response
								->set_code($code)
								->set_message($message); 
		}

		public function has_code_success() 						{ return $this->get_code()>=200 && $this->get_code()<300; }

		
	}