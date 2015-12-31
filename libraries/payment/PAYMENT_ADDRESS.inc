<?

	/**
	 * Class PAYMENT_ADDRESS  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_ADDRESS {

		/**
		 * The name
		 * @var string $name 
		 */
		protected $_name = "";

		/**
		 * The address 1
		 * @var string $address1
		 */
		protected $_address1 = "";

		/**
		 * The address 2
		 * @var string $address2
		 */
		protected $_address2 = "";

		/**
		 * The city
		 * @var string $city
		 */
		protected $_city = "";

		/**
		 * The state
		 * @var string $state
		 */
		protected $_state = "";

		/**
		 * The zip
		 * @var string $zip
		 */
		protected $_zip = "";

		/**
		 * The country
		 * @var string $country
		 */
		protected $_country = "";

		/**
		 * Constructor
		 * @param string $name
		 * @param string $address1
		 * @param string $address2
		 * @param string $city
		 * @param string $state
		 * @param string $zip
		 * @param string $country 
		 */
		public function __construct($name = "", $address1 = "", $address2 = "", $city = "", $state = "", $zip = "", $country = "") {
			$this->_name = $name;
			$this->_address1 = $address1;
			$this->_address2 = $address2;
			$this->_city = $city;
			$this->_state = $state;
			$this->_zip = $zip;
			$this->_country = $country;
		}

		/**
		 * Sets the name
		 * @param string $name 
		 */
		public function set_name($name) {
			$this->_name = $name;
		}

		/**
		 * Sets the address1
		 * @param string $address1 
		 */
		public function set_address1($address1) {
			$this->_address1 = $address1;
		}

		/**
		 * Sets the address2
		 * @param string $address2 
		 */
		public function set_address2($address2) {
			$this->_address2 = $address2;
		}

		/**
		 * Sets the city
		 * @param string $city 
		 */
		public function set_city($city) {
			$this->_city = $city;
		}

		/**
		 * Sets the state
		 * @param string $state 
		 */
		public function set_state($state) {
			$this->_state = $state;
		}

		/**
		 * Sets the zip
		 * @param string $zip 
		 */
		public function set_zip($zip) {
			$this->_zip = $zip;
		}

		/**
		 * Sets the country
		 * @param string $country 
		 */
		public function set_country($country) {
			$this->_country = $country;
		}

		/**
		 * Returns the name
		 * @return string
		 */
		public function get_name() {
			return $this->_name;
		}

		/**
		 * Returns the address1
		 * @return string
		 */
		public function get_address1() {
			return $this->_address1;
		}

		/**
		 * Returns the address2
		 * @return string
		 */
		public function get_address2() {
			return $this->_address2;
		}

		/**
		 * Returns the city
		 * @return string
		 */
		public function get_city() {
			return $this->_city;
		}

		/**
		 * Returns the state
		 * @return string
		 */
		public function get_state() {
			return $this->_state;
		}

		/**
		 * Returns the zip
		 * @return string
		 */
		public function get_zip() {
			return $this->_zip;
		}

		/**
		 * Returns the country
		 * @return string
		 */
		public function get_country() {
			return $this->_country;
		}

	}