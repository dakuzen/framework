<?php
    /**
    * Class BASE_CMODEL_ADDRESS
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_ADDRESS extends MODEL {

		/**
		 * The address1
		 * @var string $address1
		 */
		protected $_address1		= "";

		/**
		 * The address2
		 * @var string $address2
		 */
		protected $_address2		= "";

		/**
		 * The city
		 * @var string $city
		 */
		protected $_city		= "";

		/**
		 * The state
		 * @var string $state
		 */
		protected $_state		= "";

		/**
		 * The zip
		 * @var string $zip
		 */
		protected $_zip			= "";

		/**
		 * The country
		 * @var type
		 */
		protected $_country		= "";

		/**
		 * The name
		 * @var string $name
		 */
		protected $_name		= "";

		/**
		 * Constructor
		 * @param string $address1
		 * @param string $address2
		 * @param string $city
		 * @param string $state
		 * @param string $zip
		 * @param string $country
		 * @param string $name
		 */
		public function __construct($address1="",$address2="",$city="",$state="",$zip="",$country="",$name="") {
			$this->_address1 = $address1;
			$this->_address2 = $address2;
			$this->_city = $city;
			$this->_state = $state;
			$this->_zip = $zip;
			$this->_country = $country;
			$this->_name = $name;
		}

		public static function create($address1="",$address2="",$city="",$state="",$zip="",$country="",$name="") {
			return new CMODEL_ADDRESS($address1,$address2,$city,$state,$zip,$country,$name);
		}

		/**
		 * Sets the address1
		 * @param string $address1
		 */
		public function set_address1($address1)		{ $this->_address1 = $address1; }

		/**
		 * Sets the address2
		 * @param string $address2
		 */
		public function set_address2($address2)		{ $this->_address2 = $address2; }

		/**
		 * Sets the city
		 * @param string $city
		 */
		public function set_city($city)			{ $this->_city = $city; }

		/**
		 * Sets the state
		 * @param string $state
		 */
		public function set_state($state)		{ $this->_state = $state; }

		/**
		 * Sets the zip
		 * @param string $zip
		 */
		public function set_zip($zip)			{ $this->_zip = $zip; }

		/**
		 * Sets the country
		 * @param string $country
		 */
		public function set_country($country)		{ $this->_country = $country; }

		/**
		 * Sets the name
		 * @param string $name
		 */
		public function set_name($name)			{ $this->_name = $name; }

		/**
		 * Returns address1
		 * @return string
		 */
		public function get_address1()			{ return $this->_address1; }

		/**
		 * Returns address2
		 * @return string
		 */
		public function get_address2()			{ return $this->_address2; }

		/**
		 * Returns city
		 * @return string
		 */
		public function get_city()			{ return $this->_city; }

		/**
		 * Returns state
		 * @return string
		 */
		public function get_state()			{ return $this->_state; }

		/**
		 * Returns zip
		 * @return string
		 */
		public function get_zip()			{ return $this->_zip; }

		/**
		 * Returns country
		 * @return string
		 */
		public function get_country()			{ return $this->_country; }

		/**
		 * Returns name
		 * @return string
		 */
		public function get_name()			{ return $this->_name; }

		public function get_inline_address() {

			$parts = array();
			if($address1=trim($this->get_address1()))
				$parts[] = $address1;

			if($address2=trim($this->get_address2()))
				$parts[] = $address2;

			if($city=trim($this->get_city()))
				$parts[] = $city;

			if($state=trim($this->get_state()))
				$parts[] = $state;

			if($country=trim($this->get_country()))
				$parts[] = $country;

			return implode(", ",$parts);
		}
	}