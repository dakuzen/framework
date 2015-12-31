<?

	/**
	 * Class PAYMENT_REQUEST_ONDEMAND_PAYMENT_PAYMENTSPRO  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_REQUEST_ONDEMAND_PAYMENT_PAYMENTSPRO extends PAYMENT_REQUEST_ONDEMAND_PAYMENT {

		/**
		 * Return formatted amount
		 * @return float 
		 */
		function get_formatted_amount() {
			return round($this->get_amount(), 2);
		}

	}