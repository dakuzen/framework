<?

	/**
	 * Class PAYMENT_REQUEST_SUBSCRIPTION  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_REQUEST_SUBSCRIPTION extends PAYMENT_REQUEST {

		const PERIOD_MONTH = "month";
		const PERIOD_DAY = "day";
		const PERIOD_WEEK = "week";
		const PERIOD_SEMIMONTH = "semimonth";
		const PERIOD_YEAR = "year";

		/**
		 * The amount
		 * @var float $amount
		 */
		protected $_amount = 0;

		/**
		 * The credit card
		 * @var string $credit_card
		 */
		protected $_credit_card = null;

		/**
		 * The bill address
		 * @var string $bill_address 
		 */
		protected $_bill_address = null;

		/**
		 * The start date
		 * @var string $start_date
		 */
		protected $_start_date = null;

		/**
		 * the period
		 * @var string $period
		 */
		protected $_period = null;

		/**
		 * The frequency
		 * @var string $frequency
		 */
		protected $_frequency = null;

		/**
		 * Constructor 
		 */
		function __construct() {
			require_once("PAYMENT_CREDIT_CARD.inc");
			require_once("PAYMENT_ADDRESS.inc");

			$this->_credit_card = new PAYMENT_CREDIT_CARD();
			$this->_bill_address = new PAYMENT_ADDRESS();
		}

		/**
		 * Sets the amount
		 * @param float $amount 
		 */
		public function set_amount($amount) {
			$this->_amount = $amount;
		}

		/**
		 * Sets the credit card
		 * @param string $credit_card 
		 */
		public function set_credit_card($credit_card) {
			$this->_credit_card = $credit_card;
		}

		/**
		 * Sets the bill address
		 * @param string $bill_address 
		 */
		public function set_bill_address($bill_address) {
			$this->_bill_address = $bill_address;
		}

		/**
		 * Sets the start date
		 * @param string $start_date 
		 */
		public function set_start_date($start_date) {
			$this->_start_date = $start_date;
		}

		/**
		 * Sets the period
		 * @param string $period 
		 */
		public function set_period($period) {
			$this->_period = $period;
		}

		/**
		 * Sets the frequency
		 * @param string $frequency 
		 */
		public function set_frequency($frequency) {
			$this->_frequency = $frequency;
		}

		/**
		 * Returns the amount
		 * @return float 
		 */
		public function get_amount() {
			return $this->_amount;
		}

		/**
		 * Returns the credit card
		 * @return string 
		 */
		public function get_credit_card() {
			return $this->_credit_card;
		}

		/**
		 * Returns the bill address
		 * @return string 
		 */
		public function get_bill_address() {
			return $this->_bill_address;
		}

		/**
		 * Returns the start date
		 * @return string 
		 */
		public function get_start_date() {
			return $this->_start_date;
		}

		/**
		 * Returns the period
		 * @return string 
		 */
		public function get_period() {
			return $this->_period;
		}

		/**
		 * Returns the frequency
		 * @return string 
		 */
		public function get_frequency() {
			return $this->_frequency;
		}

		/**
		 * Checks is period is month
		 * @return boolean 
		 */
		public function is_period_month() {
			return $this->get_period() == self::PERIOD_MONTH;
		}

		/**
		 * Checks is period is day
		 * @return boolean 
		 */
		public function is_period_day() {
			return $this->get_period() == self::PERIOD_DAY;
		}

		/**
		 * Checks is period is week
		 * @return boolean 
		 */
		public function is_period_week() {
			return $this->get_period() == self::PERIOD_WEEK;
		}

		/**
		 * Checks is period is semimonth
		 * @return boolean 
		 */
		public function is_period_semimonth() {
			return $this->get_period() == self::PERIOD_SEMIMONTH;
		}

		/**
		 * Checks is period is year
		 * @return boolean 
		 */
		public function is_period_year() {
			return $this->get_period() == self::PERIOD_YEAR;
		}

		/**
		 * Sets period to month
		 * @return string 
		 */
		public function period_month() {
			return $this->set_period(self::PERIOD_MONTH);
		}

		/**
		 * Sets period to day
		 * @return string 
		 */
		public function period_day() {
			return $this->set_period(self::PERIOD_DAY);
		}

		/**
		 * Sets period to week
		 * @return string 
		 */
		public function period_week() {
			return $this->set_period(self::PERIOD_WEEK);
		}

		/**
		 * Sets period to semimonth
		 * @return string 
		 */
		public function period_semimonth() {
			return $this->set_period(self::PERIOD_SEMIMONTH);
		}

		/**
		 * Sets period to year
		 * @return string 
		 */
		public function period_year() {
			return $this->set_period(self::PERIOD_YEAR);
		}

		/**
		 * Returns the mapped period
		 * @param string $mapping
		 * @return mixed
		 * @todo review return type 
		 */
		public function get_mapped_period($mapping) {
			return get_value($mapping, $this->get_period());
		}

	}