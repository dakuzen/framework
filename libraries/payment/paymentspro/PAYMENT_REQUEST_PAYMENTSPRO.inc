<?

	/**
	 * Class PAYMENT_REQUEST_SALE_PAYMENTSPRO  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_REQUEST_SALE_PAYMENTSPRO extends PAYMENT_REQUEST {

		/**
		 * construct
		 */
		function __construct() {
			parent::__construct();

			require_once("PAYMENT_CREDIT_CARD_PAYMENTSPRO.inc");
			$this->_credit_card = new PAYMENT_CREDIT_CARD_PAYMENTSPRO();
		}

	}