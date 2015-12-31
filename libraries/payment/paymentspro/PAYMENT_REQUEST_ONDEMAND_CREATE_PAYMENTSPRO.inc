<?

	/**
	 * Class PAYMENT_REQUEST_ONDEMAND_CREATE_PAYMENTSPRO  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_REQUEST_ONDEMAND_CREATE_PAYMENTSPRO extends PAYMENT_REQUEST_CARD_CREATE {

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
		 * @return float
		 */
		function get_formatted_amount() {
			return round($this->get_amount(), 2);
		}

	}