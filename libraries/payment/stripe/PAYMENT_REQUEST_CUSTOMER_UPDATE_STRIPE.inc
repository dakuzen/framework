<?

	/**
	 * Class PAYMENT_REQUEST_SALE_STRIPE  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_REQUEST_CUSTOMER_UPDATE_STRIPE extends PAYMENT_REQUEST_STRIPE {

		protected $_customer = null;

		public function __construct($customer_id) {				
			$this->_customer = Stripe_Customer::retrieve($customer_id);
		}

		public function get_customer() { return $this->_customer; }
	}