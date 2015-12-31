<?

	/**
	 * Class PAYMENT_CREDIT_CARD_PAYMENTSPRO  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_CREDIT_CARD_PAYMENTSPRO extends PAYMENT_CREDIT_CARD {

		/**
		 * Return formatted type
		 * @return boolean
		 * @todo review return type 
		 */
		public function get_formatted_type() {
			return $this->get_mapped_type(array(self::TYPE_VISA => "Visa", self::TYPE_MC => "MasterCard", self::TYPE_AMEX => "Amex"));
		}

		/**
		 * Return formatted expiry month
		 * @return string 
		 */
		public function get_formatted_expiry_month() {
			return str_pad($this->get_expiry_month(), 2, "0", STR_PAD_LEFT);
		}

	}