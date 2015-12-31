<?
	require_once(MODEL_PATH::get_framework_libraries_dir()."payment/PAYMENT_PROCESSOR.inc");

	class BASE_MODEL_PAYMENT {
		
		public static function create_paymentspro_processor($api_username, $api_password, $api_signature) {
			return self::init(PAYMENT_PROCESSOR::create_paymentspro_processor($api_username, $api_password, $api_signature));
		}

		public static function create_stripe_processor($api_key) {

			require_once(MODEL_PATH::get_framework_packages_dir()."Stripe/lib/Stripe.php");

			Stripe::setApiKey($api_key);

			return self::init(PAYMENT_PROCESSOR::create_stripe_processor($api_key));
		}

		public static function include_paymentspro() {
			PAYMENT_PROCESSOR::include_paymentspro();
		}

		public static function include_stripe() {
			PAYMENT_PROCESSOR::include_stripe();
		}		

		public static function init($payment_processor) {
			$payment_processor->set_log_directory(MODEL_PATH::get_logs_directory());

			if (SYSTEM_MANAGER::is_system_mode_production())
				$payment_processor->mode_live();

			return $payment_processor;
		}		

	}