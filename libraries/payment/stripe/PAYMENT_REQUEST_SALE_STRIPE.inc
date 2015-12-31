<?

	/**
	 * Class PAYMENT_REQUEST_SALE_STRIPE  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_REQUEST_SALE_STRIPE extends PAYMENT_REQUEST_SALE {
		
		protected $_access_token 	= null;
		protected $_customer_id 	= "";
		protected $_card_id 		= "";
		protected $_application_fee = "";
		protected $_email 			= "";

		/**
		 * Constructor 
		 */
		function __construct() {
			parent::__construct();

			require_once("PAYMENT_CREDIT_CARD_STRIPE.inc");
			$this->_credit_card = new PAYMENT_CREDIT_CARD_STRIPE();
		}

		/**
		 * Returns formatted ammount
		 * @return float 
		 */
		function get_amount() {
			return parent::get_amount() * 100;
		}

		function get_application_fee() {
			return $this->_application_fee;
		}

		function get_card_id() {
			return $this->_card_id;
		}

		function get_access_token() {
			return $this->_access_token;
		}	

		function set_card_id($card_id) {
			$this->_card_id = $card_id; return $this;
		}

		function get_customer_id() {
			return $this->_customer_id;
		}

		function set_application_fee($application_fee) {
			$this->_application_fee = $application_fee; return $this;
		}	

		function set_customer_id($customer_id) {
			$this->_customer_id = $customer_id; return $this;
		}

		function set_access_token($access_token) {
			$this->_access_token = $access_token; return $this;
		}	

		function set_email($email) {
			$this->_email = $email; return $this;
		}		
	}