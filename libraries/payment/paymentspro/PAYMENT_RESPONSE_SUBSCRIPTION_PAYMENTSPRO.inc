<?

	/**
	 * Class PAYMENT_RESPONSE_SUBSCRIPTION_PAYMENTSPRO  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_RESPONSE_SUBSCRIPTION_PAYMENTSPRO extends PAYMENT_RESPONSE {

		/**
		 * The profile id
		 * @var integer $profile_id
		 */
		protected $_profile_id = "";

		/**
		 * Returns the profile id
		 * @return integer 
		 */
		function get_profile_id() {
			return $this->_profile_id;
		}

		/**
		 * Set profile id
		 * @param integer $profile_id
		 */
		function set_profile_id($profile_id) {
			$this->_profile_id = $profile_id;
		}

	}