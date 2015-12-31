<?

	/**
	 * Class PAYMENT_REQUEST_SALE_PAYMENTSPRO  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_REQUEST_SALE_PAYMENTSPRO extends PAYMENT_REQUEST_SALE {

		/**
		 * Constructor 
		 */
		function __construct() {
			parent::__construct();

			require_once("PAYMENT_CREDIT_CARD_PAYMENTSPRO.inc");

			$this->_credit_card = new PAYMENT_CREDIT_CARD_PAYMENTSPRO();
		}

		/**
		 * Returns formatted ammount
		 * @return float 
		 */
		function get_formatted_amount() {
			return round($this->get_amount(), 2);
		}

	}