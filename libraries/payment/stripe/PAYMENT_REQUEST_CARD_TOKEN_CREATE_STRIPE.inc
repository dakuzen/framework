<?

	/**
	 * Class PAYMENT_REQUEST_CARD_TOKEN_CREATE_STRIPE  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_REQUEST_CARD_TOKEN_CREATE_STRIPE extends PAYMENT_REQUEST_CARD_CREATE {

		/**
		 * Constructor 
		 */
		function __construct() {
			parent::__construct();

			require_once("PAYMENT_CREDIT_CARD_STRIPE.inc");
			$this->_credit_card = new PAYMENT_CREDIT_CARD_STRIPE();
		}

	}