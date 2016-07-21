<?
	require_once("PAYMENT_REQUEST_STRIPE.inc");

	/**
	 * Class PAYMENT_PROCESSOR_STRIPE
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_PROCESSOR_STRIPE extends PAYMENT_PROCESSOR {

		const ENVIRONMENT_LIVE = "live";
		const ENVIRONMENT_TEST = "sandbox";

		/**
		 * The api username
		 * @var string $api_key
		 */
		protected $_api_key = null;

		/**
		 * The log level
		 * @var string $log_level
		 */
		protected $_log_level = null;

		/**
		 * Constructor
		 * @param string $api_key
		 * @param string $api_password
		 * @param string $api_signature
		 */
		public function __construct($api_key) {
			parent::__construct();
			$this->_api_key 	= $api_key;
		}

		/**
		 * Create sale request
		 * @return object PAYMENT_REQUEST_SALE_STRIPE
		 *
		 * $processor = BASE_MODEL_PAYMENT::create_stripe_processor("sk_test_zcKhbUcl9JLeMlXzvz9Th5t4");
		 *
		 * $request = $processor->create_sale_request();
		 *
		 * $request
		 * 		->set_currency("CAD")
		 * 		->set_amount(99.00);
		 *
		 * $credit_card = $request
		 * 					->get_credit_card()
		 * 						->set_number("4242424242424242")
		 * 						->set_expiry_month(5)
		 * 						->set_expiry_year(2020);
		 *
		 */

		 public function create_sale_request() {
			require_once("PAYMENT_REQUEST_SALE.inc");
			require_once("PAYMENT_REQUEST_SALE_STRIPE.inc");
			return new PAYMENT_REQUEST_SALE_STRIPE();
		}

		public function create_card_token_create_request() {
			require_once("PAYMENT_REQUEST_CARD_CREATE.inc");
			require_once("PAYMENT_REQUEST_CARD_TOKEN_CREATE_STRIPE.inc");
			return new PAYMENT_REQUEST_CARD_TOKEN_CREATE_STRIPE();
		}

		/**
		 * create authorization request
		 * @return object PAYMENT_REQUEST_AUTHORIZATION_STRIPE
		 */
		public function create_authorization_request() {
			require_once("PAYMENT_REQUEST_AUTHORIZATION.inc");
			require_once("PAYMENT_REQUEST_AUTHORIZATION_STRIPE.inc");
			return new PAYMENT_REQUEST_AUTHORIZATION_STRIPE();
		}

		/**
		 * create ondemand payment request
		 * @return object PAYMENT_REQUEST_ONDEMAND_PAYMENT_STRIPE
		 */
		public function create_ondemand_payment_request() {
			require_once("PAYMENT_REQUEST_ONDEMAND_PAYMENT.inc");
			require_once("PAYMENT_REQUEST_ONDEMAND_PAYMENT_STRIPE.inc");
			return new PAYMENT_REQUEST_ONDEMAND_PAYMENT_STRIPE();
		}

		/**
		 * create ondemand create request
		 * @return object PAYMENT_REQUEST_ONDEMAND_CREATE_STRIPE
		 */
		public function create_card_create_request() {
			require_once("PAYMENT_REQUEST_CARD_CREATE.inc");
			require_once("PAYMENT_REQUEST_CARD_CREATE_STRIPE.inc");
			return new PAYMENT_REQUEST_CARD_CREATE_STRIPE();
		}

		/**
		 * create ondemand create request
		 * @return object PAYMENT_REQUEST_ONDEMAND_CREATE_STRIPE
		 */
		public function create_customer_update_request($customer_id) {
			require_once("PAYMENT_REQUEST_CUSTOMER_UPDATE_STRIPE.inc");
			return new PAYMENT_REQUEST_CUSTOMER_UPDATE_STRIPE($customer_id);
		}

		/**
		 * create ondemand create request
		 * @return object PAYMENT_REQUEST_ONDEMAND_CREATE_STRIPE
		 */
		public function create_customer_create_request() {
			require_once("PAYMENT_REQUEST_CUSTOMER_CREATE_STRIPE.inc");
			return new PAYMENT_REQUEST_CUSTOMER_CREATE_STRIPE();
		}


		/**
		 * create refund request
		 * @return object PAYMENT_REQUEST_REFUND
		 */
		public function create_refund_request() {
			require_once("PAYMENT_REQUEST_REFUND.inc");
			return new PAYMENT_REQUEST_REFUND();
		}

		/**
		 * create void request
		 * @return object PAYMENT_REQUEST_VOID
		 */
		public function create_void_request() {
			require_once("PAYMENT_REQUEST_VOID.inc");
			return new PAYMENT_REQUEST_VOID();
		}

		/**
		 * create response
		 * @param string $payment_request
		 * @return object PAYMENT_RESPONSE_SUBSCRIPTION_STRIPE
		 */
		public function create_response($payment_request) {

			$reponse = new PAYMENT_RESPONSE();

			if ($payment_request->is_type_subscription_create()) {
				require_once("PAYMENT_RESPONSE_SUBSCRIPTION_STRIPE.inc");
				$reponse = new PAYMENT_RESPONSE_SUBSCRIPTION_STRIPE();
			}

			return $reponse;
		}

		/**
		 * create send request
		 * @return object PAYMENT_REQUEST_SEND
		 */
		public function create_send_request() {
			require_once("PAYMENT_REQUEST_SEND.inc");
			return new PAYMENT_REQUEST_SEND();
		}

		/**
		 * Process
		 * @param string $payment_request
		 * @return object
		 * @todo review return type
		 * @throws PAYMENT_EXCEPTION
		 */
		public function process($payment_request) {

			if($payment_request->is_type_sale()) {

				$card = $payment_request->get_card_id();

				if(!$card) {
					$credit_card = $payment_request->get_credit_card();

					$card = array(
						"number"=>$credit_card->get_number(),
						"exp_month"=>$credit_card->get_expiry_month(),
						"exp_year"=>$credit_card->get_expiry_year()
					);
					if($credit_card->get_cvv2())
						$card["cvc"] = $credit_card->get_cvv2();

					$optional_fields = ["name","address_line1","address_line2","address_city","address_state","address_country","address_zip"];
					foreach($optional_fields as $field)
						if($credit_card->{"get_".$field}())
							$card[$field] = $credit_card->{"get_".$field}();

				}


				$args = array(	"amount"=>$payment_request->get_amount(),
								"currency" => $payment_request->get_currency(),
								"description"=>$payment_request->get_note());

				if($shipping = $payment_request->get_shipping())
					$args["shipping"] = $shipping;


				if($payment_request->get_access_token()) {

					$token = Stripe_Token::create(array("customer"=>$payment_request->get_customer_id(),
														"card"=>$card),$payment_request->get_access_token());

					if($application_fee=$payment_request->get_application_fee())
						$args["application_fee"] = $application_fee * 100;

					$args["card"] = $token->id;

					return Stripe_Charge::create($args,$payment_request->get_access_token());
				} else {

					if($custom=$payment_request->get_customer_id())
						$args["customer"] = $custom;

					$args["card"] = $card;

					return Stripe_Charge::create($args);
				}

			} elseif(preg_match("/^PAYMENT_REQUEST_CUSTOMER_CREATE_STRIPE/",get_class($payment_request))) {
				$args = array(	"email"=>$payment_request->get_email(),
								"metadata"=>array("uid"=>$payment_request->get_customer_identifier())
				);

				if($shipping=$payment_request->get_shipping())
					$args["shipping"] = $shipping;

				return Stripe_Customer::create($args);

			} elseif(preg_match("/^PAYMENT_REQUEST_CUSTOMER_UPDATE_STRIPE/",get_class($payment_request))) {

				return $payment_request->get_customer()->save();

			} elseif(preg_match("/^PAYMENT_REQUEST_CARD_TOKEN_CREATE_STRIPE/",get_class($payment_request))) {

				$credit_card = $payment_request->get_credit_card();
				$card = array("number"=>$credit_card->get_number(),"exp_month"=>$credit_card->get_expiry_month(),"exp_year"=>$credit_card->get_expiry_year());

				return Stripe_Token::create(array("card"=>$card));

			} elseif ($payment_request->is_type_card_create()) {

				$credit_card = $payment_request->get_credit_card();

				$card = array(
					"number"=>$credit_card->get_number(),
					"exp_month"=>$credit_card->get_expiry_month(),
					"exp_year"=>$credit_card->get_expiry_year()
					);
				if($credit_card->get_cvv2())
					$card["cvc"] = $credit_card->get_cvv2();

				$optional_fields = ["name","address_line1","address_line2","address_city","address_state","address_country","address_zip"];
				foreach($optional_fields as $field)
					if($credit_card->{"get_".$field}())
						$card[$field] = $credit_card->{"get_".$field}();

				$customer = Stripe_Customer::retrieve($payment_request->get_customer()->get_id());
				return $customer->cards->create(array("card"=>$card));

			} elseif ($payment_request->is_type_refund()) {

				$charge = Stripe_Charge::retrieve($payment_request->get_transaction_id());

				$parms = [];
				if($amount=$payment_request->get_amount())
					$parms["amount"] = $amount * 100;

				return $charge->refund($parms);
			}

			throw new Exception("Unknown payment error");
		}
	}

