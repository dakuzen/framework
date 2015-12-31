<?

	/**
	 * Class PAYMENT_EXCEPTION  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_EXCEPTION extends EXCEPTION {

		/**
		 * the message
		 * @var string $message 
		 */
		protected $_message = "";

		/**
		 * The system code
		 * @var $system_code
		 */
		protected $_system_code = "";

		/**
		 * The merchant code
		 * @var string $merchant_code
		 */
		protected $_merchant_code = "";

		/**
		 * The user message
		 * @var string $user_message
		 */
		protected $_user_message = "";

		/**
		 * The log
		 * @var string $log 
		 */
		protected $_log = "";

		/**
		 * The request
		 * @var string $request
		 */
		protected $_request = "";

		/**
		 * the response
		 * @var string $response 
		 */
		protected $_response = "";

		/**
		 * Constructor
		 * @param string $message
		 * @param string $system_code
		 * @param string $merchant_code
		 * @param string $user_message
		 * @param string $log
		 * @param string $request
		 * @param string $response 
		 */
		public function __construct($message, $system_code, $merchant_code = null, $user_message = "", $log = "", $request = "", $response = "") {
			parent::__construct($message);
			$this->_system_code = $system_code;
			$this->_merchant_code = $merchant_code;
			$this->_user_message = $user_message;
			$this->_log = $log;
			$this->_request = $request;
			$this->_response = $response;
		}

		/**
		 * Returns the message
		 * @return string 
		 */
		public function get_message() {
			return $this->getMessage();
		}

		/**
		 * Returns the system code
		 * @return string 
		 */
		public function get_system_code() {
			return $this->_system_code;
		}

		/**
		 * Returns the merchant code
		 * @return string 
		 */
		public function get_merchant_code() {
			return $this->_merchant_code;
		}

		/**
		 * Returns the user message
		 * @return string 
		 */
		public function get_user_message() {
			return $this->_user_message;
		}

		/**
		 * Returns the log
		 * @return string 
		 */
		public function get_log() {
			return $this->_log;
		}

		/**
		 * Returns the request
		 * @return string 
		 */
		public function get_request() {
			return $this->_request;
		}

		/**
		 * Returns the response
		 * @return string 
		 */
		public function get_response() {
			return $this->_response;
		}

	}