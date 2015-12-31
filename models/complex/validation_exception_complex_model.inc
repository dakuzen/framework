<? 
	class BCMODEL_VALIDATION_EXCEPTION extends Exception {

		protected $_messages = array();

		public static function create() {
			return new CMODEL_VALIDATION_EXCEPTION();
		}

		public function add($message,$code=0) {

			if(is_a($message,"Exception"))
				return $this->add($message->getMessage(),$message->getCode());

			$this->_messages[] = array("message"=>$message,"code"=>$code);
			$this->message = $message;
			$this->code = $code;

			return $this;
		}

		public function process() {
			if($this->_messages)
				throw $this;
		}

		public function messages() {
			return $this->_messages;
		}		
	}