<?

	/**
	 * Class PAYMENT_PAY_RECEIPT  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_PAY_RECEIPT {
		public static function create_adaptive_payment_processor($api_username, $api_password, $api_signature) {
			require_once("adaptivepayments/PAYMENT_PAY_RECEIPT_ADAPTIVEPAYMENTS.inc");
			return new PAYMENT_PAY_RECEIPT_ADAPTIVEPAYMENTS($api_username, $api_password, $api_signature);
		}
	}