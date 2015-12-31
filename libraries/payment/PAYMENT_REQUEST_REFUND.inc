<?

	/**
	 * Class PAYMENT_REQUEST_REFUND  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_REQUEST_REFUND extends PAYMENT_REQUEST {

		const REFUND_TYPE_FULL		= "F";
		const REFUND_TYPE_PARTIAL	= "P";

		/**
		 * The transaction id
		 * @var integer $transaction_id 
		 */
		protected $_transaction_id = null;

		/**
		 * The refund amount 
		 * @var float $amount 
		 */
		protected $_amount = 0;		

		/**
		 * The refund type 
		 * @var string $refund_type 
		 */
		protected $_refund_type = self::REFUND_TYPE_FULL;				

		/**
		 * set the transaction id
		 * @param integer $transaction_id 
		 */
		public function set_transaction_id($transaction_id) {
			$this->_transaction_id = $transaction_id;
		}


		/**
		 * Return the set_transaction id
		 * @return integer 
		 */
		public function get_transaction_id() {
			return $this->_transaction_id;
		}		

		/**
		 * Return the amount
		 * @return float 
		 */
		public function get_amount() {
			return $this->_amount;
		}

		/**
		 * set the amount
		 * @param float $amount 
		 */
		public function set_amount($amount) {
			$this->_amount = $amount; return $this;
		}

		/**
		 * set the refund type to partial
		 */
		public function refund_type_partial() {
			$this->_refund_type = self::REFUND_TYPE_PARTIAL;
		}

		/**
		 * checks if the refund type is partial
		 */
		public function is_refund_type_partial() { return $this->_refund_type == self::REFUND_TYPE_PARTIAL; }	

		/**
		 * checks if the refund type is full
		 */
		public function is_refund_type_full() { return $this->_refund_type == self::REFUND_TYPE_FULL; }							

	}