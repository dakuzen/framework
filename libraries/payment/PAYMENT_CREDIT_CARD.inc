<?

	/**
	 * Class PAYMENT_CREDIT_CARD  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_CREDIT_CARD {

		const TYPE_VISA = "V";
		const TYPE_MC = "M";
		const TYPE_AMEX = "A";

		/**
		 * The credit card id
		 * @var integer $credit_card_id 
		 */
		protected $_credit_card_id = null;

		/**
		 * The number
		 * @var string $number 
		 */
		protected $_number = "";

		/**
		 * The expiry month
		 * @var integer $expiry_month 
		 */
		protected $_expiry_month = 0;

		/**
		 * The expiry year
		 * @var integer $expiry_year 
		 */
		protected $_expiry_year = 0;

		/**
		 * The cvv2
		 * @var integer $cvv2 
		 */
		protected $_cvv2 = "";

		/**
		 * the type
		 * @var string $type 
		 */
		protected $_type = null;

		/**
		 * The name
		 * @var string $name 
		 */
		protected $_name = null;

		/**
		 * Returns the type list
		 * @return array 
		 */
		static function get_type_list() {
			return array(self::TYPE_VISA => "Visa", self::TYPE_MC => "Master Card", self::TYPE_AMEX => "American Express");
		}

		/**
		 * Checks whether type is visa
		 * @return boolean
		 */
		function is_type_visa() {
			return $this->get_type() == self::TYPE_VISA;
		}

		/**
		 * Checks whether type is mastercard
		 * @return boolean
		 */
		function is_type_mastercard() {
			return $this->get_type() == self::TYPE_MC;
		}

		/**
		 * Checks whether type is amex
		 * @return boolean
		 */
		function is_type_amex() {
			return $this->get_type() == self::TYPE_AMEX;
		}

		/**
		 * Returns the number
		 * @return string 
		 */
		function get_number() {
			return $this->_number;
		}

		/**
		 * Returns the expiry month
		 * @return integer 
		 */
		function get_expiry_month() {
			return $this->_expiry_month;
		}

		/**
		 * Returns the expiry year
		 * @return integer 
		 */
		function get_expiry_year() {
			return $this->_expiry_year;
		}

		/**
		 * Returns the cvv2
		 * @return integer 
		 */
		function get_cvv2() {
			return $this->_cvv2;
		}

		/**
		 * Returns the credit card id
		 * @return integer 
		 */
		function get_credit_card_id() {
			return $this->_credit_card_id;
		}

		/**
		 * Returns the type
		 * @return string
		 */
		function get_type() {
			return $this->_type;
		}

		/**
		 * Returns the name
		 * @return string 
		 */
		function get_name() {
			return $this->_name;
		}

		/**
		 * Returns the type name
		 * @return string 
		 */
		function get_type_name() {
			return get_value(self::get_type_list(), $this->get_type());
		}

		/**
		 * Sets the number
		 * @param string $value 
		 */
		function set_number($value) {
			$this->_number = $value; return $this;
		}

		/**
		 * Sets the expiry year
		 * @param integer $year 
		 */
		function set_expiry_year($year) {
			$this->_expiry_year = $year; return $this;
		}

		/**
		 * Sets the expiry month
		 * @param integer $month 
		 */
		function set_expiry_month($month) {
			$this->_expiry_month = $month; return $this;
		}

		/**
		 * Sets the cvv2
		 * @param string $value 
		 */
		function set_cvv2($value) {
			$this->_cvv2 = $value; return $this;
		}

		/**
		 * Sets the credit card id
		 * @param integer $value 
		 */
		function set_credit_card_id($value) {
			$this->_credit_card_id = $value; return $this;
		}

		/**
		 * Sets the type
		 * @param string $value 
		 */
		function set_type($value) {
			$this->_type = $value; return $this;
		}

		/**
		 * Sets the name
		 * @param string $value 
		 */
		function set_name($value) {
			$this->_name = $value; return $this;
		}

		/**
		 * Sets type to visa 
		 */
		function type_visa() {
			$this->set_type(self::TYPE_VISA);
		}

		/**
		 * Sets type to mastercard 
		 */
		function type_mastercard() {
			$this->set_type(self::TYPE_MC);
		}

		/**
		 * Sets type to amex 
		 */
		function type_amex() {
			$this->set_type(self::TYPE_AMEX);
		}

		/**
		 * Returns the mapped type
		 * @param array $mapping
		 * @return mixed
		 * @todo review return type 
		 */
		function get_mapped_type($mapping) {
			return get_value($mapping, $this->get_type());
		}

		/**
		 * Returns masked number
		 * @return string 
		 */
		function get_masked_number() {
			$number = strlen($this->get_number()) > 4 ? substr($this->get_number(), -4) : $this->get_number();
			return sprintf("%'*16s", $number);
		}

		/**
		 * Validates a card
		 * @return boolean 
		 */
		function validate() {

			if (!trim($this->get_name()))
				$this->add_error_message("Credit card name is not supplied");

			if (!trim($this->get_number()))
				$this->add_error_message("Credit card number is not supplied");

			if (!trim($this->get_cvv2()))
				$this->add_error_message("Credit card verification number not supplied");

			return !$this->has_error_messages();
		}

	}