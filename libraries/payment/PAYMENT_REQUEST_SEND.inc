<?

	/**
	 * Class PAYMENT_REQUEST_SEND  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_REQUEST_SEND extends PAYMENT_REQUEST {

		/**
		 * The sender
		 * @var string 
		 */
		private $_sender;
		
		/**
		 * The amount
		 * @var double 
		 */
		private $_amount;
		
		/**
		 * The receiver
		 * @var string 
		 */
		Private $_receiver;

		/**
		 * Sets the sender
		 * @param string $sender 
		 */
		public function set_sender($sender) {
			$this->_sender = $sender;
		}

		/**
		 * Sets the amount
		 * @param double $amount 
		 */
		public function set_amount($amount) {
			$this->_amount = $amount;
		}

		/**
		 * Sets the receiver
		 * @param string $receiver 
		 */
		public function set_receiver($receiver) {
			$this->_receiver = $receiver;
		}

		/**
		 * Returns the sender
		 * @return string 
		 */
		public function get_sender() {
			return $this->_sender;
		}

		/**
		 * Returns the ammount
		 * @return double 
		 */
		public function get_amount() {
			return $this->_amount;
		}

		/**
		 * Returns the receiver
		 * @return string 
		 */
		public function get_receiver() {
			return $this->_receiver;
		}

	}