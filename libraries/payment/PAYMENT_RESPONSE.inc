<?

	/**
	 * Class PAYMENT_RESPONSE  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_RESPONSE {

		/**
		 * The system code
		 * @var string $system_code
		 */
		protected $_system_code = PAYMENT_PROCESSOR::CODE_UNSPECIFIED;

		/**
		 * The transaction id
		 * @var integer transaction_id
		 */
		protected $_transaction_id = "";

		/**
		 * The log
		 * @var string $log 
		 */
		protected $_log = "";

		/**
		 * Sets the transaction id
		 * @param integer $transaction_id 
		 */
		public function set_transaction_id($transaction_id) {
			$this->_transaction_id = $transaction_id;
		}

		/**
		 * Sets the log
		 * @param string $log 
		 */
		public function set_log($log) {
			$this->_log = $log;
		}

		/**
		 * Returns the transaction id
		 * @return integer 
		 */
		public function get_transaction_id() {
			return $this->_transaction_id;
		}

		/**
		 * Returns the system code
		 * @return string 
		 */
		public function get_system_code() {
			return $this->_system_code;
		}

		/**
		 * Sets the system code
		 * @param string $system_code 
		 */
		public function set_system_code($system_code) {
			$this->_system_code = $system_code;
		}

		/**
		 * success
		 */
		public function success() {
			$this->set_system_code(PAYMENT_PROCESSOR::CODE_SUCCESS);
		}

		/**
		 * Checks whether has success
		 * @return boolean
		 */
		public function has_success() {
			return $this->_system_code == PAYMENT_PROCESSOR::CODE_SUCCESS;
		}

	}