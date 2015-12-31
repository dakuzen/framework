<?php
        /**
        * Class BASE_CMODEL_PAYPAL_WEBSITE_PAYMENTS_PRO
        *
        * @package Framework
        * @subpackage Cmodels
        */
			
	class BASE_CMODEL_PAYPAL_WEBSITE_PAYMENTS_PRO extends MODEL {
		
		const MODE_LIVE 	= "L";
		const MODE_TEST 	= "T";
		
                /**
                 * The api username
                 * @var string $api_username 
                 */
		protected $_api_username 	= "";
                
                /**
                 * The api password
                 * @var string $api_password 
                 */
		protected $_api_password 	= "";
		
                /**
                 * The api signature
                 * @var string $api_signature 
                 */
                protected $_api_signature 	= "";
		
                /**
                 * Constructor
                 * @param string $api_username
                 * @param string $api_password
                 * @param string $api_signature
                 * @param boolean $is_live 
                 */
		function __construct($api_username,$api_password,$api_signature,$is_live=false) {		
			$this->_api_username 	= $api_username;
			$this->_api_password 	= $api_password;
			$this->_api_signature 	= $api_signature;
			$this->_mode	 	= $is_live ? self::MODE_LIVE : self::MODE_TEST;
		}
		
                /**
                 * Checks whether is in live mode
                 * @return boolean 
                 */
		function is_live() 					{ return $this->_mode==self::MODE_LIVE; }
                
                /**
                 * Checks whether is in test mode
                 * @return boolean 
                 */
		function is_test() 					{ return $this->_mode==self::MODE_TEST; }
		
                /**
                 * Returns the api username
                 * @return string 
                 */
		function get_api_username() 				{ return $this->_api_username; }
                
                /**
                 * Returns the api password
                 * @return string 
                 */
		function get_api_password() 				{ return $this->_api_password; }
		
                /**
                 * Returns the api signature
                 * @return string 
                 */
                function get_api_signature() 				{ return $this->_api_signature; }
				
                /**
                 * Creates capture request
                 * @return object PAYPAL_WPP_CAPTURE_REQUEST 
                 */
		function create_capture_request()  			{ return new PAYPAL_WPP_CAPTURE_REQUEST($this); }
                
                /**
                 * Creates reference transaction request
                 * @return object PAYPAL_WPP_REFERENCE_TRANSACTION 
                 */
		function create_reference_transaction_request()  	{ return new PAYPAL_WPP_REFERENCE_TRANSACTION($this); }
                
                /**
                 * Creates credit card payment request
                 * @return object PAYPAL_WPP_PAYMENT_REQUEST 
                 */
		function create_credit_card_payment_request()  		{ return new PAYPAL_WPP_PAYMENT_REQUEST($this); }
                
                /**
                 * Creates credit card refund request
                 * @return object PAYPAL_WPP_REFUND_REQUEST 
                 */
		function create_credit_card_refund_request()  		{ return new PAYPAL_WPP_REFUND_REQUEST($this); }
	}
	
        SERVER_UTIL::add_include_path_pear();
	SERVER_UTIL::add_include_path(MODEL_PATH::get_framework_directory()."libraries/payment/paymentspro/php-sdk/lib/");

	require_once("PayPal.php");
	require_once("PayPal/Profile/Handler/Array.php");
	require_once("PayPal/Profile/API.php");
	require_once("PayPal/Type/DoDirectPaymentRequestType.php");
	require_once("PayPal/Type/DoDirectPaymentRequestDetailsType.php");
	require_once("PayPal/Type/DoDirectPaymentResponseType.php");
        
	/*
	
	$paypal_website_payments_pro_cmodel = new BASE_CMODEL_PAYPAL_WEBSITE_PAYMENTS_PRO($api_username,$api_password,$api_signature,false);
	$request->set_refund_transaction_id("transaction_id");
	$response = $request->process();
	
	if($response->has_success())
		APPLICATION::add_notify_message("Successful");
	
	*/	

        /**
        * Class PAYPAL_WPP_REFUND_REQUEST
        *
        * @package Framework
        * @subpackage Cmodels
        */        
        
	class PAYPAL_WPP_REFUND_REQUEST extends PAYPAL_WPP_REQUEST {
		
                /**
                 * refund transaction id
                 * @var string $refund_transaction_id 
                 */
		protected $_refund_transaction_id	= "";
                
                /**
                 * The refund type
                 * @var string $refund_type
                 */
		protected $_refund_type			= "Full"; // or "Partial"
                
                /**
                 * the memo
                 * @var string $memo 
                 */
		protected $_memo			= "";
		
                /**
                 * Sets the refund transaction id
                 * @param string $refund_transaction_id 
                 */
		function set_refund_transaction_id($refund_transaction_id) 	{ $this->_refund_transaction_id = $refund_transaction_id; }
		
                /**
                 * Sets memo
                 * @param string $memo 
                 */
                function set_memo($memo) 					{ $this->_memo = $memo; }
                
                /**
                 * Enables refund type partial 
                 */
		function enable_refund_type_partial() 				{ $this->_refund_type = "Partial"; }
		
                /**
                 * Returns the refund transaction id
                 * @return string 
                 */
		function get_refund_transaction_id() 				{ return $this->_refund_transaction_id; }
		
                /**
                 * Process
                 * @return object PAYPAL_WPP_RESPONSE 
                 */
		function process() {
			
			$paypal_request =& PayPal::getType("RefundTransactionRequestType");
			$paypal_request->setTransactionId($this->get_refund_transaction_id());	
			$paypal_request->setRefundType($this->_refund_type);
			
			if(preg_match("/^Partial$/i",$this->_refund_type)) {
				$Amount =& PayPal::getType("BasicAmountType");
				$Amount->setattr("currencyID", $this->_currency_id);
				$Amount->setval($this->_amount,$this->get_charset());
				$paypal_request->setAmount($Amount);
				$paypal_request->setMemo($this->_memo);
			}
			
			$response = new PAYPAL_WPP_RESPONSE();
			
			$paypal_response = $this->get_caller()->RefundTransaction($paypal_request);
			
			if(get_class($paypal_response)=="RefundTransactionResponseType") {

				if($paypal_response->getAck()==PAYPAL_WPP_REQUEST::ACK_SUCCESS || $paypal_response->getAck()==PAYPAL_WPP_REQUEST::ACK_SUCCESS_WITH_WARNING) 
					$response->success();
			}
			
			$response->inherit_messages($paypal_response);
			
			return $response;
		}
	}
	
	/*
	
	$paypal_website_payments_pro_cmodel = new BASE_CMODEL_PAYPAL_WEBSITE_PAYMENTS_PRO($api_username,$api_password,$api_signature,false);
	$request = $paypal_website_payments_pro_cmodel->create_capture_request();
	$request->set_authorization_transaction_id("transaction_id");
	$request->set_amount(300);

	$response = $request->process();
	
	if($response->has_success()) {
		APPLICATION::add_notify_message("Successful");
	
	*/
	
        /**
        * Class PAYPAL_WPP_CAPTURE_REQUEST
        *
        * @package Framework
        * @subpackage Cmodels
        */           
        
        
	class PAYPAL_WPP_CAPTURE_REQUEST extends PAYPAL_WPP_REQUEST {
		
                /**
                 * The authorization transaction id
                 * @var string 
                 */
		protected $_authorization_transaction_id = "";
		
                /**
                 * Process
                 */
		function process() {

			$paypal_request = PayPal::getType("DoCaptureRequestType");
			$paypal_request->setAuthorizationID($this->_authorization_transaction_id, $this->get_charset());
			$paypal_request->setCompleteType("Complete");
			
			$amtType = PayPal::getType("BasicAmountType");
			$amtType->setattr("currencyID", $this->_currency_id);
			$amtType->setval($this->get_amount(), $this->get_charset());
			$paypal_request->setAmount($amtType);
			
			$paypal_response = $this->get_caller()->DoCapture($paypal_request);
			
			$response = new PAYPAL_WPP_RESPONSE();

			if(get_class($paypal_response)=="DoCaptureResponseType") {			
			
				if($paypal_response->getAck()==PAYPAL_WPP_REQUEST::ACK_SUCCESS || $paypal_response->getAck()==PAYPAL_WPP_REQUEST::ACK_SUCCESS_WITH_WARNING)
					$response->success();				
			}
			
			$response->inherit_messages($paypal_response);

			return $response;
			
		}
		
                /**
                 * sets authorization transaction id
                 * @param string $authorization_transaction_id 
                 */
		public function set_authorization_transaction_id($authorization_transaction_id) { $this->_authorization_transaction_id = $authorization_transaction_id; }
	}
	
	/*
	
	$paypal_website_payments_pro_cmodel = new BASE_CMODEL_PAYPAL_WEBSITE_PAYMENTS_PRO($api_username,$api_password,$api_signature,false);
	$request = $paypal_website_payments_pro_cmodel->create_reference_transaction_request();
	$request->set_amount(300);
	$request->set_reference_transaction_id("transaction_id");

	$response = $request->process();

	if($response->has_success()) {
		APPLICATION::add_notify_message("Successful");	
	
	*/
	
        /**
        * Class PAYPAL_WPP_REFERENCE_TRANSACTION
        *
        * @package Framework
        * @subpackage Cmodels
        */  
        
	class PAYPAL_WPP_REFERENCE_TRANSACTION extends PAYPAL_WPP_REQUEST {
		
                /**
                 * reference transaction id
                 * @var string $reference_transaction_id 
                 */
		protected $_reference_transaction_id 	= "";
                
                /**
                 * The payment action
                 * @var string $payment_action
                 */
		protected $_payment_action 		= "Sale";
		
                /**
                 * Process
                 * @return object PAYPAL_WPP_RESPONSE 
                 */
		function process() {

			$payment_detail = PayPal::getType("PaymentDetailsType");
			$payment_detail->setOrderTotal($this->get_amount());
			
			$details =& PayPal::getType("DoReferenceTransactionRequestDetailsType");
			$details->setReferenceID($this->_reference_transaction_id);
			$details->setPaymentAction($this->_payment_action);
			$details->setPaymentDetails($payment_detail);
			
			$paypal_request = PayPal::getType("DoReferenceTransactionRequestType");
			$paypal_request->setDoReferenceTransactionRequestDetails($details);
			
			$paypal_response = $this->get_caller()->DoReferenceTransaction($paypal_request);
			
			$response = new PAYPAL_WPP_RESPONSE();
			
			if(get_class($paypal_response)=="DoReferenceTransactionResponseType") {

				if($paypal_response->getAck()==PAYPAL_WPP_REQUEST::ACK_SUCCESS || $paypal_response->getAck()==PAYPAL_WPP_REQUEST::ACK_SUCCESS_WITH_WARNING) {
					$response->success();	
					$response->set_transaction_id($paypal_response->getDoReferenceTransactionResponseDetails()->getTransactionID());
				}
			}

			$response->inherit_messages($paypal_response);

			return $response;
		}
		
                /**
                 * set reference transaction id
                 * @param string $reference_transaction_id 
                 */
		public function set_reference_transaction_id($reference_transaction_id) { $this->_reference_transaction_id = $reference_transaction_id; }
		
                /**
                 * enable payment action authorization
                 */
                public function enable_payment_action_authorization() 			{ $this->_payment_action = "Authorization"; }
	}
	
	/*
	
	$paypal_website_payments_pro_cmodel = new BASE_CMODEL_PAYPAL_WEBSITE_PAYMENTS_PRO($api_username,$api_password,$api_signature,false);
	$request = $paypal_website_payments_pro_cmodel->create_credit_card_payment_request();
	$request->populate_credit_card("4962463370111039",05,2015,"000","");
	$request->populate_billing_address("123 Main St","","Omaha","NE","68114","US");
	$request->populate_person("John","Doe");
	$request->get_credit_card()->enable_type_visa();
	$request->set_amount(100);

	$response = $request->process();

	if($response->has_success()) {
		APPLICATION::add_notify_message("Successful");
	*/
        
        /**
        * Class PAYPAL_WPP_PAYMENT_REQUEST
        *
        * @package Framework
        * @subpackage Cmodels
        */  
        
	class PAYPAL_WPP_PAYMENT_REQUEST extends PAYPAL_WPP_REQUEST {
                
                /**
                 * Process
                 * @return object PAYPAL_WPP_RESPONSE 
                 */
		function process() {
			
			$credit_card_cmodel 	= $this->get_credit_card();
			$billing_address_cmodel = $this->get_billing_address();

			$paypal_request =& PayPal::getType("DoDirectPaymentRequestType");
			$paypal_request->setVersion(PAYPAL_WPP_REQUEST::PAYPAL_VERSION);

			$credit_card_type = null;
			if($credit_card_cmodel->is_type_visa())
				$credit_card_type = "Visa";
				
			elseif($credit_card_cmodel->is_type_mastercard())
				$credit_card_type = "MasterCard";
				
			elseif($credit_card_cmodel->is_type_amex())
				$credit_card_type = "Amex";
				
			$paymentType 		= "Sale";
			$amount 		= round($this->get_amount(),2);
			
			$OrderTotal = &PayPal::getType("BasicAmountType");
			$OrderTotal->setattr("currencyID", $this->_currency_id);
			$OrderTotal->setval($amount, $this->get_charset());
			
			$shipTo =& PayPal::getType("AddressType");
			$shipTo->setName($this->get_full_name());
			$shipTo->setStreet1($billing_address_cmodel->get_address1());
			$shipTo->setCityName($billing_address_cmodel->get_city());
			$shipTo->setStateOrProvince($billing_address_cmodel->get_state());
			$shipTo->setCountry($billing_address_cmodel->get_country());
			$shipTo->setPostalCode($billing_address_cmodel->get_zip());
			
			$PaymentDetails =& PayPal::getType("PaymentDetailsType");
			$PaymentDetails->setOrderTotal($OrderTotal);
			$PaymentDetails->setShipToAddress($shipTo);

			$person_name =& PayPal::getType("PersonNameType");
			$person_name->setFirstName($this->get_first_name());
			$person_name->setLastName($this->get_last_name());
			
			$payer =& PayPal::getType("PayerInfoType");
			$payer->setPayerName($person_name);
			$payer->setPayerCountry($billing_address_cmodel->get_country());
			$payer->setAddress($shipTo);

			$card_details =& PayPal::getType("CreditCardDetailsType");
			$card_details->setCardOwner($payer);
			$card_details->setCreditCardType($credit_card_type);
			$card_details->setCreditCardNumber($credit_card_cmodel->get_number());
			$card_details->setExpMonth($credit_card_cmodel->get_padding_expiry_month());
			$card_details->setExpYear($credit_card_cmodel->get_full_expiry_year());
			$card_details->setCVV2($credit_card_cmodel->get_cvv2());
			
			$dp_details =& PayPal::getType("DoDirectPaymentRequestDetailsType");
			$dp_details->setPaymentDetails($PaymentDetails);
			$dp_details->setCreditCard($card_details);
			$dp_details->setIPAddress(SERVER_UTIL::get_remote_ip());
			$dp_details->setPaymentAction($paymentType);

			$paypal_request->setDoDirectPaymentRequestDetails($dp_details);
			$paypal_request->setVersion(PAYPAL_WPP_REQUEST::PAYPAL_VERSION);

			$response = new PAYPAL_WPP_RESPONSE();
			
			$paypal_response = $this->get_caller()->DoDirectPayment($paypal_request);
			
			if(get_class($paypal_response)=="DoDirectPaymentResponseType") {

				if($paypal_response->getAck()==PAYPAL_WPP_REQUEST::ACK_SUCCESS || $paypal_response->getAck()==PAYPAL_WPP_REQUEST::ACK_SUCCESS_WITH_WARNING) {
					$response->success();
					$response->set_transaction_id($paypal_response->getTransactionID());
				}
			}
			
			$response->inherit_messages($paypal_response);
			
			return $response;
		}		
	}	

        /**
        * Class PAYPAL_WPP_REQUEST
        *
        * @package Framework
        * @subpackage Cmodels
        */          
        
	abstract class PAYPAL_WPP_REQUEST {
		
		const PAYPAL_VERSION	 		= "51.0";
		
		const ACK_SUCCESS			= "Success";
		const ACK_SUCCESS_WITH_WARNING		= "SuccessWithWarning";
		
                /**
                 * The profile
                 * @var object $profile
                 */
		protected $_profile			= null;
                
                /**
                 * The credit card cmodel
                 * @var object $credit_card_cmodel 
                 */
		protected $_credit_card_cmodel 		= null;
                
                /**
                 * The billing address cmodel
                 * @var object $billing_address_cmodel
                 */
		protected $_billing_address_cmodel 	= null;
                
                /**
                 * The amount
                 * @var float 
                 */
		protected $_amount		 	= 0;
                
                /**
                 * The currency id
                 * @var string
                 */
		protected $_currency_id			= "USD"; // or other currency ("GBP", "EUR", "JPY", "CAD", "AUD")
                
                /**
                 * the first name
                 * @var string $first_name
                 */
		protected $_first_name		 	= "";
                
                /**
                 * the last name
                 * @var string $last_name 
                 */
		protected $_last_name		 	= "";
                
                /**
                 * the charset
                 * @var string
                 */
		protected $_charset		 	= "iso-8859-1";
		
                /**
                 * Process 
                 */
		abstract function process();
		
                /**
                 * Constructor
                 * @param object $wrapper 
                 */
		function __construct($wrapper) {
		
			$environment = $wrapper->is_live() ? "live" : "sandbox";
			
			$handler = & ProfileHandler_Array::getInstance(array("username"=>$wrapper->get_api_username(),"certificateFile"=>null,"subject"=>null,"environment"=>$environment));
			
			$this->_profile = & new APIProfile(ProfileHandler::generateID(), $handler);
			$this->_profile->setAPIUsername($wrapper->get_api_username());
			$this->_profile->setAPIPassword($wrapper->get_api_password());
			$this->_profile->setSignature($wrapper->get_api_signature());
			$this->_profile->setEnvironment($environment);
			
			$this->_credit_card_cmodel 	= new BASE_CMODEL_CREDIT_CARD();
			$this->_billing_address_cmodel 	= new BASE_CMODEL_ADDRESS();
		}
		
                /**
                 * Return caller
                 * @return object 
                 */
		function get_caller() {
			$caller =& PayPal::getCallerServices($this->_profile);		
			$caller->_logDir = MODEL_PATH::get_logs_directory();
			$caller->_logLevel = 7;
			
			FILE_UTIL::mkdir($caller->_logDir);
			
			return $caller;
		}
		
                /**
                 * Populates credit card
                 * @param string $number
                 * @param integer $expiry_year
                 * @param integer $expiry_month
                 * @param integer $cvv2
                 * @param string $name
                 * @param string $type 
                 */
		function populate_credit_card($number,$expiry_year,$expiry_month,$cvv2,$name,$type=null) {
			$this->_credit_card_cmodel = new BASE_CMODEL_CREDIT_CARD($number,$expiry_year,$expiry_month,$cvv2,$name,$type);
		}
		
                /**
                 * Populates billing address
                 * @param string $address1
                 * @param string $address2
                 * @param string $city
                 * @param string $state
                 * @param string $zip
                 * @param string $country
                 * @param string $name 
                 */
		function populate_billing_address($address1,$address2,$city,$state,$zip,$country,$name="") {
			$this->_billing_address_cmodel = new BASE_CMODEL_ADDRESS($address1,$address2,$city,$state,$zip,$country,$name);			
		}
		
                /**
                 * Populates a person
                 * @param string $first_name
                 * @param string $last_name 
                 */
		function populate_person($first_name,$last_name) {
			$this->_first_name = $first_name;
			$this->_last_name = $last_name;
		}
		
                /**
                 * Sets the amount
                 * @param float $amount 
                 */
		function set_amount($amount) 			{ $this->_amount = $amount; }
                
                /**
                 * Enables currency cad 
                 */
		function enable_currency_cad() 			{ $this->_currency_id = "CAD"; }
		
                /**
                 * Returns credit card
                 * @return object 
                 */
		function get_credit_card() 			{ return $this->_credit_card_cmodel; }
                
                /**
                 * Returns billing address
                 * @return object 
                 */
		function get_billing_address() 			{ return $this->_billing_address_cmodel; }
                
                
                /**
                 * Returns the amount
                 * @return float
                 */
		function get_amount() 				{ return $this->_amount; }
                
                /**
                 * Returns the first name
                 * @return string 
                 */
		function get_first_name() 			{ return $this->_first_name; }
                
                /**
                 * Returns the charset
                 * @return string 
                 */
		function get_charset() 				{ return $this->_charset; }
		
                /**
                 * Returns the last name
                 * @return string 
                 */
                function get_last_name() 			{ return $this->_last_name; }
		
                /**
                 * Returns the full name
                 * @return string 
                 */
                function get_full_name() 			{ return $this->get_first_name()." ".$this->get_last_name(); }		
	}
        
        /**
        * Class PAYPAL_WPP_MESSAGE
        *
        * @package Framework
        * @subpackage Cmodels
        */    
        
	class PAYPAL_WPP_MESSAGE {
		
		const TYPE_ERROR = "E";
		
                /**
                 * The message
                 * @var string $message
                 */
		protected $_message		= "";
                
                /**
                 * The user message
                 * @var string $user_message
                 */
		protected $_user_message	= "";
                
                /**
                 * The code
                 * @var string $code 
                 */
		protected $_code		= "";
                
                /**
                 * The type
                 * @var string $type 
                 */
		protected $_type		= "";

                /**
                 * Constructor
                 * @param string $type
                 * @param string $message
                 * @param string $code 
                 */
		public function __construct($type="",$message="",$code="") {
			$this->_message = $message;
			$this->_code = $code;
			$this->_type = $type;
		}
                
                /**
                 * Sets the message
                 * @param string $message 
                 */
		public function set_message($message)		{ $this->_message = $message; }
                
                /**
                 * Sets the code
                 * @param string $code 
                 */
		public function set_code($code)			{ $this->_code = $code; }

                /**
                 * Returns the message
                 * @return string
                 */
		public function get_message()		{ return $this->_message; }
                
                /**
                 * Returns the code
                 * @return string
                 */
		public function get_code()			{ return $this->_code; }
	}

        /**
        * Class PAYPAL_WPP_RESPONSE
        *
        * @package Framework
        * @subpackage Cmodels
        */    
        
	class PAYPAL_WPP_RESPONSE {
		
                /**
                 * Has success
                 * @var boolean $has_success
                 */
		protected $_has_success 	= false;
                
                /**
                 * the response
                 * @var string
                 */
		protected $_response 		= "";
                
                /**
                 * the messages
                 * @var array $messages
                 */
		protected $_messages 		= array();
                
                /**
                 * Transaction id
                 * @var string
                 */
		protected $_transaction_id	= "";
		
                /**
                 * Set success 
                 */
		function success() 				{ $this->_has_success = true; }
		
                /**
                 * Set the response
                 * @param string $response 
                 */
		function set_response($response) 		{ $this->_response; }
                
                /**
                 * Adds a mesage
                 * @param string $message 
                 */
		function add_message($message) 			{ $this->_messages[] = $message; }
                
                /**
                 * Sets user message
                 * @param string $user_message 
                 */
		function set_user_message($user_message) 	{ $this->_user_message; }
                
                /**
                 * sets the transaction id
                 * @param string $transaction_id 
                 */
		function set_transaction_id($transaction_id) 	{ $this->_transaction_id = $transaction_id; }
		
                /**
                 * Return the response
                 * @return string 
                 */
		function get_response() 			{ return $this->_response; }
                
                /**
                 * Return the messages
                 * @return array 
                 */
		function get_messages() 			{ return $this->_messages; }
                
                /**
                 * Return the user message
                 * @return string 
                 */
		function get_user_message() 			{ return $this->_user_message; }
                
                /**
                 * Return the transaction id
                 * @return string 
                 */
		function get_transaction_id() 			{ return $this->_transaction_id; }
		
                /**
                 * Checks whether has success
                 * @return boolean
                 */
		function has_success() 				{ return $this->_has_success; }
		
                /**
                 * inherit messages
                 * @param object $paypal_response 
                 */
		function inherit_messages($paypal_response) {
			
			if($paypal_response->getErrors()) {
				$errors = is_array($paypal_response->getErrors()) ? $paypal_response->getErrors() : array($paypal_response->getErrors());
			
				foreach($errors as $error)
					$this->add_message(new PAYPAL_WPP_MESSAGE(PAYPAL_WPP_MESSAGE::TYPE_ERROR,$error->getLongMessage(),$error->getErrorCode()));
			}
		}					
	}	