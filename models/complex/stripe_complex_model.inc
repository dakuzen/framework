<?
	require_once(MODEL_PATH::get_framework_packages_dir()."Stripe/lib/Stripe.php");

	class BCMODEL_STRIPE extends OBJECT {

		public function __construct($api_key) {
			Stripe::setApiKey($api_key);
		}

		public function account() {

		}
	}

