<?

	/**
	 * Class PAYMENT_REQUEST_CARD_CREATE  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_REQUEST_CARD_CREATE extends PAYMENT_REQUEST {

		/**
		 * The credit card.
		 * @var string $credit_card 
		 */
		protected $_credit_card = null;

		/**
		 * The bill address.
		 * @var string $bill_address 
		 */
		protected $_bill_address = null;

		/**
		 * Constructor.
		 * 
		 */
		function __construct() {
			require_once("PAYMENT_CREDIT_CARD.inc");
			require_once("PAYMENT_ADDRESS.inc");

			$this->_credit_card = new PAYMENT_CREDIT_CARD();
			$this->_bill_address = new PAYMENT_ADDRESS();
		}

		/**
		 * Set the credit card.
		 * @param string $credit_card the credit card.
		 */
		public function set_credit_card($credit_card) {
			$this->_credit_card = $credit_card;
		}

		/**
		 * Set the bill address.
		 * @param string $bill_address the bill address.
		 */
		public function set_bill_address($bill_address) {
			$this->_bill_address = $bill_address;
		}

		/**
		 * Get the credit card.
		 * 
		 * @return string
		 */
		public function get_credit_card() {
			return $this->_credit_card;
		}

		/**
		 * Get the bill address.
		 * 
		 * @return string
		 */
		public function get_bill_address() {
			return $this->_bill_address;
		}

	}