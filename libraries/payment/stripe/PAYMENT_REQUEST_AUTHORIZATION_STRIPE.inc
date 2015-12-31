<?

	/**
	 * Class PAYMENT_REQUEST_AUTHORIZATION_PAYMENTSPRO  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_REQUEST_AUTHORIZATION_PAYMENTSPRO extends PAYMENT_REQUEST_AUTHORIZATION {

		/**
		 * Constructor 
		 */
		function __construct() {
			parent::__construct();

			require_once("PAYMENT_CREDIT_CARD_PAYMENTSPRO.inc");

			$this->_credit_card = new PAYMENT_CREDIT_CARD_PAYMENTSPRO();
		}

		/**
		 * Return formatted amount
		 */
		function get_formatted_amount() {
			return round($this->get_amount(), 2);
		}

	}