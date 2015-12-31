<?

	/**
	 * Class PAYMENT_REQUEST  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_REQUEST {

		/**
		 * The note
		 * @var string $note 
		 */
		protected $_note = "";

		/**
		 * Check whether type is sale
		 * @return boolean
		 */
		function is_type_sale() {
			return preg_match("/^PAYMENT_REQUEST_SALE/", get_class($this));
		}

		/**
		 * Checks whether type is ondemand create
		 * @return boolean
		 */
		function is_type_card_create() {
			return preg_match("/^PAYMENT_REQUEST_CARD_CREATE/", get_class($this));
		}

		/**
		 * Checks whether type is ondemand payment
		 * @return boolean
		 */
		function is_type_ondemand_payment() {
			return preg_match("/^PAYMENT_REQUEST_CARD_SALE/", get_class($this));
		}

		/**
		 * Checks whether type is subscription create
		 * @return boolean
		 */
		function is_type_subscription_create() {
			return preg_match("/^PAYMENT_REQUEST_SUBSCRIPTION/", get_class($this));
		}

		/**
		 * Checks whether type is authorization
		 * @return boolean
		 */
		function is_type_authorization() {
			return preg_match("/^PAYMENT_REQUEST_AUTHORIZATION/", get_class($this));
		}

		/**
		 * Checks whether type is void
		 * @return boolean
		 */
		function is_type_void() {
			return preg_match("/^PAYMENT_REQUEST_VOID/", get_class($this));
		}

		/**
		 * Checks whether type is refund
		 * @return boolean
		 */
		function is_type_refund() {
			return preg_match("/^PAYMENT_REQUEST_REFUND/", get_class($this));
		}

		/**
		 * Checks whether type is send
		 * @return boolean
		 */
		function is_type_send() {
			return preg_match("/^PAYMENT_REQUEST_SEND/", get_class($this));
		}		

		/**
		 * sets the note
		 * @param string $note 
		 */
		function set_note($note) {
			$this->_note = $note; return $this;
		}

		/**
		 * Returns the note
		 * @return string
		 */
		function get_note() {
			return $this->_note;
		}

	}