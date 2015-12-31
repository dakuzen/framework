<?

	/**
	 * Class PAYMENT_REQUEST_SUBSCRIPTION_PAYMENTSPRO  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_REQUEST_SUBSCRIPTION_PAYMENTSPRO extends PAYMENT_REQUEST_SUBSCRIPTION {

		/**
		 * the start time
		 * @var integer $start_time
		 */
		protected $_start_time = "";

		/**
		 * Constructor 
		 */
		function __construct() {
			parent::__construct();

			require_once("PAYMENT_CREDIT_CARD_PAYMENTSPRO.inc");

			$this->_credit_card = new PAYMENT_CREDIT_CARD_PAYMENTSPRO();
		}

		/**
		 * Return the formatted period
		 * @return mixed 
		 */
		function get_formatted_period() {
			return $this->get_mapped_period(array(self::PERIOD_MONTH => "Month", self::PERIOD_DAY => "Day",
					self::PERIOD_WEEK => "Week", self::PERIOD_SEMIMONTH => "SemiMonth",
					self::PERIOD_YEAR => "Year"));
		}

		/**
		 * Sets start date
		 * @param mixed $start_date
		 */
		function set_start_date($start_date) {
			$start_time = is_numeric($start_date) ? $start_date : strtotime($start_date);
			$this->_start_time = $start_time;
		}

		/**
		 * Return the formatted amount
		 * @return float 
		 */
		function get_formatted_amount() {
			return round($this->get_amount(), 2);
		}

		/**
		 * Return the formatted start date
		 * @return string 
		 */
		function get_formatted_start_date() {
			return gmdate("Y-n-j\T0:0:0", $this->_start_time);
		}

	}