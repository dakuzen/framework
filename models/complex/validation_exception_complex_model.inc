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

			$this->message = $this->get_message();

			return $this;
		}

		public function process() {
			if($this->_messages)
				throw $this;
		}

		public function messages() {
			return $this->_messages;
		}

		public function has_messages() {
			return !!$this->_messages;
		}

		public function get_message() {
			$messages = [];
			foreach($this->_messages as $message)
				$messages[] = $message["message"];

			return implode(", ",$messages);
		}		
	}