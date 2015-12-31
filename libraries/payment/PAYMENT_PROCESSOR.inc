<?

	SERVER_UTIL::add_include_path(MODEL_PATH::get_framework_libraries_directory() . "payment/");

	require_once("PAYMENT_EXCEPTION.inc");
	require_once("PAYMENT_REQUEST.inc");
	require_once("PAYMENT_RESPONSE.inc");
	require_once("PAYMENT_PROCESSOR.inc");

	/**
	 * Class PAYMENT_PROCESSOR  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	abstract class PAYMENT_PROCESSOR {

		const MERCHANT_PAYMENTSPRO 				= "paymentspro";
		const MERCHANT_STRIPE					= "stripe";

		const MODE_TEST 						= "test";
		const MODE_LIVE 						= "live";
		
		const CODE_UNSPECIFIED 					= "50";
		const CODE_SUCCESS 						= "100";
		const CODE_ERROR_REQUEST_NOT_SUPPORTED 	= "101";
		const CODE_ERROR_CALLER_SERVICE 		= "102";
		const CODE_ERROR_MERCHANT				= "103";
		const CODE_ERROR_INVALID_RESPONSE 		= "104";
		const CODE_ERROR_SOAT_FAULT 			= "105";
		const CODE_ERROR_UNKNOWN				= "106";
		const CODE_ERROR_ADAPTIVE_PAYMENT 		= "107";

		/**
		 * log directory
		 * @var string $log_directory 
		 */
		protected $_log_directory = null;

		/**
		 * The mode
		 * @var string $mode 
		 */
		protected $_mode = self::MODE_TEST;

		/**
		 * Constructor 
		 */
		public function __construct() {
		}

		/**
		 * Returns the code list
		 * @return array 
		 */
		public static function get_code_list() {
			return array(self::CODE_UNSPECIFIED => "Unspecified",
				self::CODE_SUCCESS => "Success",
				self::CODE_ERROR_REQUEST_NOT_SUPPORTED => "Request not Supported",
				self::CODE_ERROR_CALLER_SERVICE => "Caller Service Error",
				self::CODE_ERROR_MERCHANT => "Merchant Error",
				self::CODE_ERROR_INVALID_RESPONSE => "Invalid response",
				self::CODE_ERROR_SOAT_FAULT => "Soat Fault Error",
				self::CODE_ERROR_UNKNOWN => "Unknown Error");
		}

		/**
		 * Checks whether is in test mode
		 * @return boolean 
		 */
		public function is_mode_test() {
			return $this->_mode == self::MODE_TEST;
		}

		/**
		 * Checks whether is in live mode
		 * @return boolean 
		 */
		public function is_mode_live() {
			return $this->_mode == self::MODE_LIVE;
		}

		/**
		 * Mode test 
		 */
		public function mode_test() {
			$this->_mode = self::MODE_TEST;
		}

		/**
		 * Mode live 
		 */
		public function mode_live() {
			$this->_mode = self::MODE_LIVE;
		}

		/**
		 * Returns the log directory
		 * @return string 
		 */
		public function get_log_directory() {
			return $this->_log_directory;
		}

		/**
		 * Sets the log directory
		 * @param string $log_directory 
		 */
		public function set_log_directory($log_directory) {
			$this->_log_directory = $log_directory;
		}

		/**
		 * Creates paymentspro processor
		 * @param string $api_username
		 * @param string $api_password
		 * @param string $api_signature
		 * @return object PAYMENT_PROCESSOR_PAYMENTSPRO 
		 */
		public static function create_paymentspro_processor($api_username, $api_password, $api_signature) {
			self::include_paymentspro();
			return new PAYMENT_PROCESSOR_PAYMENTSPRO($api_username, $api_password, $api_signature);
		}

		public static function create_stripe_processor($api_key) {
			self::include_stripe();
			return new PAYMENT_PROCESSOR_STRIPE($api_key);
		}

		public static function include_stripe() {
			require_once("stripe/PAYMENT_PROCESSOR_STRIPE.inc");
		}

		public static function include_paymentspro() {
			require_once("paymentspro/PAYMENT_PROCESSOR_PAYMENTSPRO.inc");
		}
		
		/**
		 * create sale request
		 */
		abstract public function create_sale_request();

		/**
		 * create authorization request 
		 */
		abstract public function create_authorization_request();

		/**
		 * create void request
		 */
		abstract public function create_void_request();

		/**
		 * create refund request 
		 */
		abstract public function create_refund_request();

		/**
		 * create response
		 * @param string $payment_request
		 */
		abstract public function create_response($payment_request);

		/**
		 * process
		 * @param string $payment_request 
		 */
		abstract public function process($payment_request);
	}