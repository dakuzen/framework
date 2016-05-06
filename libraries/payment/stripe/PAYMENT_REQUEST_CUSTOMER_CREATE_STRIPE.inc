<?

	/**
	 * Class PAYMENT_REQUEST_SALE_STRIPE
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_REQUEST_CUSTOMER_CREATE_STRIPE extends PAYMENT_REQUEST_STRIPE {

		protected $_customer_identifier = "";
		protected $_email = "";
		protected $_shipping = [];

		/**
		 * Constructor
		 */
		function __construct() {

		}

		/**
		 * Returns formatted ammount
		 * @return float
		 */
		function get_customer_identifier() {
			return $this->_customer_identifier;
		}

		function set_customer_identifier($customer_identifier) {
			$this->_customer_identifier = $customer_identifier; return $this;
		}

		function get_email() {
			return $this->_email;
		}
		function set_email($email) {
			$this->_email = $email; return $this;
		}

		function get_shipping() {
			return $this->_shipping;
		}
		function set_shipping($value) {
			$this->_shipping = $value; return $this;
		}

	}