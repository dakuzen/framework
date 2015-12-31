<?

	/**
	 * Class PAYMENT_REQUEST_VOID  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_REQUEST_VOID extends PAYMENT_REQUEST {

		/**
		 * The transaction id.
		 * @var integer $transaction_id 
		 */
		protected $_transaction_id = null;

		/**
		 * Set the transaction id.
		 * @param integer $transaction_id
		 */
		public function set_transaction_id($transaction_id) {
			$this->_transaction_id = $transaction_id;
		}

		/**
		 * Get the transaction id.
		 * 
		 * @return integer
		 */
		public function get_transaction_id() {
			return $this->_transaction_id;
		}

	}