<?

	/**
	 * Class PAYMENT_REQUEST_ONDEMAND_PAYMENT  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_REQUEST_CARD_SALE extends PAYMENT_REQUEST {

		/**
		 * The amount
		 * @var float $amount 
		 */
		protected $_amount = "";

		/**
		 * The reference id
		 * @var integer $reference_id
		 */
		protected $_reference_id = "";

		/**
		 * The credit card
		 * @var string $credit_card
		 */
		protected $_credit_card = null;

		/**
		 * The bill address
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
		 * Set the reference id
		 * @param integer $reference_id 
		 */
		public function set_reference_id($reference_id) {
			$this->_reference_id = $reference_id;
		}

		/**
		 * Set the credit card
		 * @param string $credit_card 
		 */
		public function set_credit_card($credit_card) {
			$this->_credit_card = $credit_card;
		}

		/**
		 * Set the bill address
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
		 * Returns the reference id
		 * @return integer 
		 */
		public function get_reference_id() {
			return $this->_reference_id;
		}

		/**
		 * Returns the reference id
		 * @return string
		 */
		public function get_credit_card() {
			return $this->_credit_card;
		}

		/**
		 * Returns the bill_address
		 * @return string 
		 */
		public function get_bill_address() {
			return $this->_bill_address;
		}

	}