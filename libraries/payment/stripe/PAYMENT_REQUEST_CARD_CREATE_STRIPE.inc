<?

	/**
	 * Class PAYMENT_REQUEST_CARD_CREATE_STRIPE  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_REQUEST_CARD_CREATE_STRIPE extends PAYMENT_REQUEST_CARD_CREATE {

		/**
		 * Constructor 
		 */
		function __construct() {
			parent::__construct();

			require_once("PAYMENT_CREDIT_CARD_STRIPE.inc");
			require_once("PAYMENT_REQUEST_CUSTOMER.inc");


			$this->_credit_card = new PAYMENT_CREDIT_CARD_STRIPE();
			$this->_customer = new PAYMENT_REQUEST_CUSTOMER();
		}

		function get_customer() {
			return $this->_customer;
		}

	}