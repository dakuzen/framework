<?

	/**
	 * Class PAYMENT_REQUEST_SALE  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_REQUEST_SALE extends PAYMENT_REQUEST {

		/**
		 * The amount
		 * @var float $amount
		 */
		protected $_amount = "";

		/**
		 * The amount
		 * @var float $amount
		 */
		protected $_currency = "";		

		/**
		 * The credit card
		 * @var string $credit_card 
		 */
		protected $_credit_card = null;

		/**
		 * the billing address
		 * @var string $bill_address 
		 */
		protected $_bill_address = null;

		/**
		 * Constructor 
		 */
		function __construct() {
			require_once("PAYMENT_CREDIT_CARD.inc");
			require_once("PAYMENT_ADDRESS.inc");

			$this->_credit_card = new PAYMENT_CREDIT_CARD();
			$this->_bill_address = new PAYMENT_ADDRESS();
		}

		/**
		 * Sets the amount
		 * @param float $amount 
		 */
		public function set_amount($amount) {
			$this->_amount = $amount;  return $this;
		}

		/**
		 * Sets the currency
		 * @param string $currency 
		 */
		public function set_currency($currency) {
			$this->_currency = $currency; return $this;
		}

		/**
		 * Sets the credit card
		 * @param string $credit_card 
		 */
		public function set_credit_card($credit_card) {
			$this->_credit_card = $credit_card;  return $this;
		}

		/**
		 * Sets the amount
		 * @param string $bill_address 
		 */
		public function set_bill_address($bill_address) {
			$this->_bill_address = $bill_address; return $this;
		}

		/**
		 * Returns the amount
		 * @return float
		 */
		public function get_amount() {
			return $this->_amount;
		}

		/**
		 * Returns the credit card
		 * @return string 
		 */
		public function get_credit_card() {
			return $this->_credit_card;
		}

		/**
		 * Returns the currency
		 * @return string 
		 */
		public function get_currency() {
			return $this->_currency;
		}		

		/**
		 * Returns the billing address
		 * @return string
		 */
		public function get_bill_address() {
			return $this->_bill_address;
		}

	}