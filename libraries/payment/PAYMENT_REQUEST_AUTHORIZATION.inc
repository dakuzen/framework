<?

	/**
	 * Class PAYMENT_REQUEST_AUTHORIZATION  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_REQUEST_AUTHORIZATION extends PAYMENT_REQUEST {

		/**
		 * the amount
		 * @var float $amount 
		 */
		protected $_amount = "";

		/**
		 * The credit card
		 * @var string $credit_card 
		 */
		protected $_credit_card = null;

		/**
		 * The billing address
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
		 * Set amount
		 * @param float $amount 
		 */
		public function set_amount($amount) {
			$this->_amount = $amount;
		}

		/**
		 * Set the credit card
		 * @param string $credit_card 
		 */
		public function set_credit_card($credit_card) {
			$this->_credit_card = $credit_card;
		}

		/**
		 * Set the billing address
		 * @param string $bill_address 
		 */
		public function set_bill_address($bill_address) {
			$this->_bill_address = $bill_address;
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
		 * Returns the bill address
		 * @return string 
		 */
		public function get_bill_address() {
			return $this->_bill_address;
		}

	}