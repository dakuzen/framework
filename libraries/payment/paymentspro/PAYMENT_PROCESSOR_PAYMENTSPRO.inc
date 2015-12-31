<?

	//API Error Codes - https://cms.paypal.com/us/cgi-bin/?cmd=_render-content&content_ID=developer/e_howto_api_nvp_errorcodes#id09C3G0PJ0N9
	/**
	 * Class PAYMENT_PROCESSOR_PAYMENTSPRO  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_PROCESSOR_PAYMENTSPRO extends PAYMENT_PROCESSOR {

		const ENVIRONMENT_LIVE = "live";
		const ENVIRONMENT_TEST = "sandbox";

		/**
		 * The api username
		 * @var string $api_username 
		 */
		protected $_api_username = null;

		/**
		 * The api password
		 * @var string $api_password 
		 */
		protected $_api_password = null;

		/**
		 * The api signature
		 * @var string $api_signature
		 */
		protected $_api_signature = null;

		/**
		 * The log level
		 * @var string $log_level 
		 */
		protected $_log_level = null;

		/**
		 * Constructor
		 * @param string $api_username
		 * @param string $api_password
		 * @param string $api_signature 
		 */
		public function __construct($api_username, $api_password, $api_signature) {
			parent::__construct();

			$this->_api_username 	= $api_username;
			$this->_api_password 	= $api_password;
			$this->_api_signature 	= $api_signature;			
		}

		/**
		 * Create sale request
		 * @return object PAYMENT_REQUEST_SALE_PAYMENTSPRO 
		 */
		public function create_sale_request() {
			require_once("PAYMENT_REQUEST_SALE.inc");
			require_once("PAYMENT_REQUEST_SALE_PAYMENTSPRO.inc");
			return new PAYMENT_REQUEST_SALE_PAYMENTSPRO();
		}

		/**
		 * create authorization request
		 * @return object PAYMENT_REQUEST_AUTHORIZATION_PAYMENTSPRO 
		 */
		public function create_authorization_request() {
			require_once("PAYMENT_REQUEST_AUTHORIZATION.inc");
			require_once("PAYMENT_REQUEST_AUTHORIZATION_PAYMENTSPRO.inc");
			return new PAYMENT_REQUEST_AUTHORIZATION_PAYMENTSPRO();
		}

		/**
		 * create ondemand payment request
		 * @return object PAYMENT_REQUEST_ONDEMAND_PAYMENT_PAYMENTSPRO 
		 */
		public function create_ondemand_payment_request() {
			require_once("PAYMENT_REQUEST_CARD_SALE.inc");
			require_once("PAYMENT_REQUEST_ONDEMAND_PAYMENT_PAYMENTSPRO.inc");
			return new PAYMENT_REQUEST_ONDEMAND_PAYMENT_PAYMENTSPRO();
		}

		/**
		 * create ondemand create request
		 * @return object PAYMENT_REQUEST_ONDEMAND_CREATE_PAYMENTSPRO 
		 */
		public function create_ondemand_create_request() {
			require_once("PAYMENT_REQUEST_CARD_CREATE.inc");
			require_once("PAYMENT_REQUEST_ONDEMAND_CREATE_PAYMENTSPRO.inc");
			return new PAYMENT_REQUEST_ONDEMAND_CREATE_PAYMENTSPRO();
		}

		/**
		 * create subscription create request
		 * @return object PAYMENT_REQUEST_SUBSCRIPTION_PAYMENTSPRO 
		 */
		public function create_subscription_create_request() {
			require_once("PAYMENT_REQUEST_SUBSCRIPTION.inc");
			require_once("PAYMENT_REQUEST_SUBSCRIPTION_PAYMENTSPRO.inc");
			return new PAYMENT_REQUEST_SUBSCRIPTION_PAYMENTSPRO();
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
		 * @return object PAYMENT_RESPONSE_SUBSCRIPTION_PAYMENTSPRO 
		 */
		public function create_response($payment_request) {

			$reponse = new PAYMENT_RESPONSE();

			if ($payment_request->is_type_subscription_create()) {
				require_once("PAYMENT_RESPONSE_SUBSCRIPTION_PAYMENTSPRO.inc");
				$reponse = new PAYMENT_RESPONSE_SUBSCRIPTION_PAYMENTSPRO();
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

			if ($payment_request->is_type_send()) {

				SERVER_UTIL::add_include_path_pear();
				SERVER_UTIL::add_include_path(MODEL_PATH::get_framework_packages_directory() . "Payment/Paypal/AdaptivePayments/lib/");

				require_once("AdaptivePayments.php");
				require_once("Stub/AP/AdaptivePaymentsProxy.php");
				require_once("Stub/AP/AdaptivePaymentsProxy.php");
				require_once(DIR_FRAMEWORK."libraries/payment/PAYMENT_PAY_RECEIPT.inc");
			

				$sender = $payment_request->get_sender();
				$receiver = $payment_request->get_receiver();
				$amount = $payment_request->get_amount();

				// create the request object
				$payRequest = new PayRequest();
				$payRequest->actionType = "PAY";
				$payRequest->cancelUrl = SYSTEM_MANAGER::get_system_url();
				$payRequest->returnUrl = SYSTEM_MANAGER::get_system_url();
				$payRequest->clientDetails = new ClientDetailsType();
				$payRequest->clientDetails->applicationId = "APP-1JE4291016473214C";
				$payRequest->clientDetails->deviceId = 'PayPal_Platform_PHP_SDK';
				$payRequest->clientDetails->ipAddress = "127.0.0.1";
				$payRequest->currencyCode = 'USD';
				$payRequest->senderEmail = $sender;
				$payRequest->requestEnvelope = new RequestEnvelope();
				$payRequest->requestEnvelope->errorLanguage = "en_US";
				$receiverObj = new receiver();
				$receiverObj->email = $receiver;
				$receiverObj->amount = $amount;
				$payRequest->receiverList = new ReceiverList();
				$payRequest->receiverList = array($receiverObj);

				try {
					// initiate the pay action
					$ap = new AdaptivePayments($this->_api_username, $this->_api_password, $this->_api_signature);
					$response = $ap->Pay($payRequest);

					if (strtoupper($ap->isSuccess) == 'FAILURE') {
						$error = $ap->getLastError();
						throw new PAYMENT_EXCEPTION($error->error->message, PAYMENT_PROCESSOR::CODE_ERROR_ADAPTIVE_PAYMENT);
					} else {
						// get the pay key
						$payKey = $response->payKey;
						if ($response->paymentExecStatus == "COMPLETED") {

							try {
								$pdRequest = new PaymentDetailsRequest();
								$pdRequest->payKey = $payKey;
								$rEnvelope = new RequestEnvelope();
								$rEnvelope->errorLanguage = "en_US";
								$pdRequest->requestEnvelope = $rEnvelope;
								$ap = new AdaptivePayments($this->_api_username, $this->_api_password, $this->_api_signature);
								$response = $ap->PaymentDetails($pdRequest);

								if (strtoupper($ap->isSuccess) == 'FAILURE') {
									$error = $ap->getLastError();
									throw new PAYMENT_EXCEPTION($error->error->message, PAYMENT_PROCESSOR::CODE_ERROR_ADAPTIVE_PAYMENT);
								}
							} catch (Exception $ex) {
								throw new PAYMENT_EXCEPTION($ex->getMessage(), PAYMENT_PROCESSOR::CODE_ERROR_ADAPTIVE_PAYMENT);
							}

							return $response;
						} else {
							throw new PAYMENT_EXCEPTION('API credenatials are different from sender email so you need to redirect to paypal to get the PreAproval Key', PAYMENT_PROCESSOR::CODE_ERROR_ADAPTIVE_PAYMENT);
						}
					}
				} catch (Exception $ex) {
					throw new PAYMENT_EXCEPTION($ex->getMessage(), PAYMENT_PROCESSOR::CODE_ERROR_ADAPTIVE_PAYMENT);
				}
				throw new PAYMENT_EXCEPTION("Unknown error", PAYMENT_PROCESSOR::CODE_ERROR_UNKNOWN);
		
			} else {

				SERVER_UTIL::add_include_path_pear();
				SERVER_UTIL::add_include_path(MODEL_PATH::get_framework_packages_directory() . "Payment/PayPal/Legacy/lib/");

				require_once "PayPal.php";
				require_once "PayPal/Profile/Handler/Array.php";
				require_once "PayPal/Profile/API.php";
				require_once "PayPal/Type/DoDirectPaymentRequestType.php";
				require_once "PayPal/Type/DoDirectPaymentRequestDetailsType.php";
				require_once "PayPal/Type/DoDirectPaymentResponseType.php";
				require_once "Log.php";
				require_once "Log/observer.php";
				require_once "PAYMENT_LOG_PAYMENTSPRO.inc";
				
				$this->_log_level = $this->is_mode_test() ? PEAR_LOG_DEBUG : PEAR_LOG_NOTICE;					

				$environment = $this->is_mode_test() ? self::ENVIRONMENT_TEST : self::ENVIRONMENT_LIVE;

				$handler = ProfileHandler_Array::getInstance(array("username" => $this->_api_username,
						"certificateFile" => null,
						"subject" => null,
						"environment" => $environment));
				$pid = ProfileHandler::generateID();

				$profile = new APIProfile($pid, $handler);

				$profile->setAPIUsername($this->_api_username);
				$profile->setAPIPassword($this->_api_password);
				$profile->setSignature($this->_api_signature);
				$profile->setEnvironment($environment);

				$request = null;
				$currency = "USD";

				if ($payment_request->is_type_sale() || $payment_request->is_type_authorization()) {

					$request = PayPal::getType("DoDirectPaymentRequestType");
					$request->setVersion("51.0");

					$credit_card = $payment_request->get_credit_card();
					$bill_address = $payment_request->get_bill_address();

					$basic_amount = PayPal::getType("BasicAmountType");
					$basic_amount->setattr("currencyID", $currency);
					$basic_amount->setval($payment_request->get_formatted_amount(), "iso-8859-1");

					$ship = PayPal::getType("AddressType");
					$ship->setName($bill_address->get_name());
					$ship->setStreet1($bill_address->get_address1());
					$ship->setCityName($bill_address->get_city());
					$ship->setStateOrProvince($bill_address->get_state());
					$ship->setCountry($bill_address->get_country());
					$ship->setPostalCode($bill_address->get_zip());

					$payment_detalis = PayPal::getType("PaymentDetailsType");
					$payment_detalis->setOrderTotal($basic_amount);
					$payment_detalis->setShipToAddress($ship);
					$payment_detalis->setCustom($payment_request->get_note());

					$person_name = PayPal::getType("PersonNameType");
					$person_name->setFirstName($credit_card->get_name());

					$payer = PayPal::getType("PayerInfoType");
					$payer->setPayerName($person_name);
					$payer->setPayerCountry($bill_address->get_country());
					$payer->setAddress($ship);

					$card_details = PayPal::getType("CreditCardDetailsType");
					$card_details->setCardOwner($payer);
					$card_details->setCreditCardType($credit_card->get_formatted_type());
					$card_details->setCreditCardNumber($credit_card->get_number());
					$card_details->setExpMonth($credit_card->get_formatted_expiry_month());
					$card_details->setExpYear($credit_card->get_expiry_year());
					$card_details->setCVV2($credit_card->get_cvv2());

					$payment_action = $payment_request->is_type_sale() ? "Sale" : "Authorization";

					$details = PayPal::getType("DoDirectPaymentRequestDetailsType");
					$details->setPaymentDetails($payment_detalis);
					$details->setCreditCard($card_details);
					$details->setIPAddress(SERVER_UTIL::get_remote_ip());
					$details->setPaymentAction($payment_action);

					$request->setDoDirectPaymentRequestDetails($details);
				} elseif ($payment_request->is_type_ondemand_create()) {

					$authorization_request = self::create_authorization_request();

					$authorization_request->set_credit_card($payment_request->get_credit_card());
					$authorization_request->set_bill_address($payment_request->get_bill_address());
					$authorization_request->set_amount(1);

					$authorization_response = $this->process($authorization_request);

					$payment_response = $this->create_response($payment_request);

					if ($transaction_id = $authorization_response->get_transaction_id()) {
						$void_request = self::create_void_request();
						$void_request->set_transaction_id($transaction_id);

						$void_response = $this->process($void_request);

						$payment_response->set_transaction_id($authorization_response->get_transaction_id());
						$payment_response->success();
					}

					return $payment_response;

				} elseif ($payment_request->is_type_ondemand_payment()) {

					$bill_address = $payment_request->get_bill_address();

					$request = PayPal::getType("DoReferenceTransactionRequestType");
					$request->setVersion("51.0");

					$basic_amount = PayPal::getType("BasicAmountType");
					$basic_amount->setattr("currencyID", $currency);
					$basic_amount->setval($payment_request->get_formatted_amount(), "iso-8859-1");

					$payment_detalis = PayPal::getType("PaymentDetailsType");
					$payment_detalis->setOrderTotal($basic_amount);
					$payment_detalis->setItemTotal($basic_amount);
					$payment_detalis->setCustom($payment_request->get_note());

					$details = PayPal::getType("DoReferenceTransactionRequestDetailsType");
					$details->setReferenceID($payment_request->get_reference_id());
					$details->setPaymentDetails($payment_detalis);
					$details->setIPAddress(SERVER_UTIL::get_remote_ip());
					$details->setPaymentAction("Sale");

					$request->setDoReferenceTransactionRequestDetails($details);
				} elseif ($payment_request->is_type_subscription_create()) {

					$request = PayPal::getType("CreateRecurringPaymentsProfileRequestType");
					$request->setVersion("51.0");

					$details = PayPal::getType("CreateRecurringPaymentsProfileRequestDetailsType");

					$recurring_payments_details = PayPal::getType("RecurringPaymentsProfileDetailsType");
					$recurring_payments_details->setBillingStartDate($payment_request->get_formatted_start_date());
					$details->setRecurringPaymentsProfileDetails($recurring_payments_details);

					$billing_period_details = PayPal::getType("BillingPeriodDetailsType");
					$billing_period_details->setBillingPeriod($payment_request->get_formatted_period());
					$billing_period_details->setBillingFrequency($payment_request->get_frequency());

					$basic_amount = PayPal::getType("BasicAmountType");
					$basic_amount->setattr("currencyID", $currency);
					$basic_amount->setval($payment_request->get_formatted_amount());
					$billing_period_details->setAmount($basic_amount);

					$schedule_details = PayPal::getType("ScheduleDetailsType");
					$schedule_details->setDescription("This is the desc");
					$schedule_details->setPaymentPeriod($billing_period_details);

					$bill_address = $payment_request->get_bill_address();
					$credit_card = $payment_request->get_credit_card();

					$ship = PayPal::getType("AddressType");
					$ship->setName($bill_address->get_name());
					$ship->setStreet1($bill_address->get_address1());
					$ship->setCityName($bill_address->get_city());
					$ship->setStateOrProvince($bill_address->get_state());
					$ship->setCountry($bill_address->get_country());
					$ship->setPostalCode($bill_address->get_zip());

					$payment_detalis = PayPal::getType("PaymentDetailsType");
					$payment_detalis->setOrderTotal($basic_amount);
					$payment_detalis->setShipToAddress($ship);

					$person_name = PayPal::getType("PersonNameType");
					$person_name->setFirstName($bill_address->get_name());

					$payer = PayPal::getType("PayerInfoType");
					$payer->setPayerName($person_name);
					$payer->setPayerCountry($bill_address->get_country());
					$payer->setAddress($ship);

					$card_details = PayPal::getType("CreditCardDetailsType");
					$card_details->setCardOwner($payer);
					$card_details->setCreditCardType($credit_card->get_formatted_type());
					$card_details->setCreditCardNumber($credit_card->get_number());
					$card_details->setExpMonth($credit_card->get_formatted_expiry_month());
					$card_details->setExpYear($credit_card->get_expiry_year());
					$card_details->setCVV2($credit_card->get_cvv2());

					$details->setScheduleDetails($schedule_details);
					$details->setCreditCard($card_details);

					$request->setCreateRecurringPaymentsProfileRequestDetails($details);
				
				} elseif($payment_request->is_type_void()) {

					$request = PayPal::getType("DoVoidRequestType");
					$request->setVersion("51.0");
					$request->setAuthorizationID($payment_request->get_transaction_id());
				
				} elseif($payment_request->is_type_refund()) {

					$request = PayPal::getType("RefundTransactionRequestType");
					$request->setVersion("51.0");
					$request->setTransactionID($payment_request->get_transaction_id());
					$request->setMemo($payment_request->get_note());

					if($payment_request->is_refund_type_partial()) {
						$request->setAmount(FORMAT_UTIL::get_currency($payment_request->get_amount()));
						$request->setRefundType("Partial");					
					}

				} else
					throw new PAYMENT_EXCEPTION("The payment request type is not supported", PAYMENT_PROCESSOR::CODE_ERROR_REQUEST_NOT_SUPPORTED);

				$caller = PayPal::getCallerServices($profile);

				FILE_UTIL::mkdir($this->get_log_directory());

				$caller->_logDir = $this->get_log_directory();
				$caller->_logLevel = $this->_log_level;

				$payment_log = new PAYMENT_LOG_PAYMENTSPRO();
				$payment_log->setPriority($this->_log_level);

				if (get_class($caller) == "PayPal_Error")
					throw new PAYMENT_EXCEPTION($caller->getMessage(), PAYMENT_PROCESSOR::CODE_ERROR_UNKNOWN);

				$logger = Log::singleton("null", "name", $caller->_profile->getAPIUsername(), array("append" => true));
				$logger->setPriority($this->_log_level);
				$logger->attach($payment_log);

				$caller->setLogger($logger);

				$payment_response = $this->create_response($payment_request);

				if (get_class($caller) == "CallerServices") {

					$paypal_response = null;

					if ($payment_request->is_type_sale() || $payment_request->is_type_authorization())
						$paypal_response = $caller->DoDirectPayment($request);

					elseif ($payment_request->is_type_ondemand_payment())
						$paypal_response = $caller->DoReferenceTransaction($request);

					elseif ($payment_request->is_type_subscription_create())
						$paypal_response = $caller->CreateRecurringPaymentsProfile($request);

					elseif ($payment_request->is_type_void())
						$paypal_response = $caller->DoVoid($request);

					elseif ($payment_request->is_type_refund())
						$paypal_response = $caller->RefundTransaction($request);

					$class = get_class($paypal_response);

					$request = $response = "";

					foreach ($payment_log->get_events() as $event) {

						if (preg_match("/^Request XML: (.*)/s", get_value($event, "message"), $match))
							$request = get_value($match, 1);

						elseif (preg_match("/^Response XML: (.*)/s", get_value($event, "message"), $match))
							$response = get_value($match, 1);
					}

					if ($paypal_response instanceof AbstractResponseType) {

						if (in_array($paypal_response->getAck(), array("Success", "SuccessWithWarning"))) {

							if ($class == "DoDirectPaymentResponseType") {

								$payment_response->set_transaction_id($paypal_response->getTransactionID());
								$payment_response->success();
							} elseif ($class == "DoReferenceTransactionResponseType") {

								$payment_response->set_transaction_id($paypal_response->getDoReferenceTransactionResponseDetails()->getTransactionID());
								$payment_response->success();
							} elseif ($class == "CreateRecurringPaymentsProfileResponseType") {

								$recurring_payments_response_details = $paypal_response->getCreateRecurringPaymentsProfileResponseDetails();

								$payment_response->set_profile_id($recurring_payments_response_details->getProfileID());
								$payment_response->set_transaction_id($recurring_payments_response_details->getTransactionID());
								$payment_response->success();
							} elseif ($class == "DoVoidResponseType") {

								$payment_response->set_transaction_id($paypal_response->getCorrelationID());
								$payment_response->success();
							} elseif ($class == "RefundTransactionResponseType") {

								$payment_response->set_transaction_id($paypal_response->getRefundTransactionID());
								$payment_response->success();
							}

							$payment_response->set_log($payment_log->get_log());

							return $payment_response;
						} else {

							$errors = $paypal_response->getErrors();

							$errors = is_array($errors) ? $errors : array($errors);

							if ($error = get_value($errors, 0))
								throw new PAYMENT_EXCEPTION($error->getLongMessage(), PAYMENT_PROCESSOR::CODE_ERROR_MERCHANT, $error->getErrorCode(), null, $payment_log->get_log(), $request, $response);
						}
					} elseif ($paypal_response instanceof SOAP_Fault)
						throw new PAYMENT_EXCEPTION("Soap Fault", PAYMENT_PROCESSOR::CODE_ERROR_SOAT_FAULT, null, null, $payment_log->get_log(), $request, $response);
					else
						throw new PAYMENT_EXCEPTION("Invalid processor response", PAYMENT_PROCESSOR::CODE_ERROR_INVALID_RESPONSE, null, null, $payment_log->get_log(), $request, $response);
				} else
					throw new PAYMENT_EXCEPTION("Failed to initialize the caller services", PAYMENT_PROCESSOR::CODE_ERROR_CALLER_SERVICE);

				throw new PAYMENT_EXCEPTION("Unknown error", PAYMENT_PROCESSOR::CODE_ERROR_UNKNOWN);
			}
		}

	}

	