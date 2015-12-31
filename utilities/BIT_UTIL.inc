<?php
        /**
        * Class BIT_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */
	 
	class BIT_UTIL {
		
		const BIT_01 = CONSTANT::BIT_01;
		const BIT_02 = CONSTANT::BIT_02;
		const BIT_03 = CONSTANT::BIT_03;
		const BIT_04 = CONSTANT::BIT_04;
		const BIT_05 = CONSTANT::BIT_05;
		const BIT_06 = CONSTANT::BIT_06;
		const BIT_07 = CONSTANT::BIT_07;
		const BIT_08 = CONSTANT::BIT_08;
		const BIT_09 = CONSTANT::BIT_09;
		const BIT_10 = CONSTANT::BIT_10;
		const BIT_11 = CONSTANT::BIT_11;
		const BIT_12 = CONSTANT::BIT_12;
		const BIT_13 = CONSTANT::BIT_13;
		const BIT_14 = CONSTANT::BIT_14;
		const BIT_15 = CONSTANT::BIT_15;
                
		/**
                 * The bit of this class.
                 * @var integer $bit 
                 */
		private $_bit = null;
		
		/** 
		 * Initializes the bit variable
		 * @param int $bit integer passed through the constructor
		 */
		
		function __construct($bit=0) {
			$this->_bit = $bit; 
		}
		/** 
		 * Checks for a bit 
		 * @param integer $bit integer being checked
		 * @return boolean
		 */
		
		function is_bit_exist($bit) {
			return $this->bit_exists($bit); 
		}
		/** 
		 * Checks for a specific bit
		 * @param int $bit integer being checked
		 * @return boolean
		 */

		function bit_exists($bit) {
			return $this->_bit && (intval($this->_bit) & intval($bit))==intval($bit); 
		}
		
		/** 
		 * Sets the bit value 
		 * @param int $value integer being set
		 */
		
		function set_bit($value) {
			$this->_bit = $value; 
		}
		
		/** 
		 * Gets the bit value 
		 * 
		 * @return int
		 */
		
		function get_bit() { 
			return $this->_bit; 
		}
		
		/** 
		 * Adds a bit integer 
		 * @param int $value integer being added
		 */
		
		function add_bit($value) {
			if(!$this->is_bit_exist($value)) 
				$this->_bit += $value; 
		}
		
		/** 
		 * Removes a bit integer 
		 * @param int $value integer being removed
		 */
		
		function remove_bit($value) { 
			if($this->is_bit_exist($value))
				$this->_bit -= $value; 
		}
		
		/** 
		 * Returns the array of bits
		 * <code>
		 * $bit_list = array("8"=>"Eight","16"=>"sixteen","4"=>"Four","5"=>"Five");
		 * $bit = new BIT_UTIL();
		 * $bit->add_bit(8);
		 * $bit->add_bit(16);
		 * $bit->add_bit(4);
		 * $list = $bit->get_bit_list($bit_list);
		 * </code>
		 * <pre>
		 * Array
		 * (
		 *	[8] => Eight
		 *	[16] => sixteen
		 *	[4] => Four
		 * )
 		 * </pre>
		 * @param int $list array list of integers
		 * @return array
		 */
		
		function get_bit_list($list) {
			$bit_list = array();
			foreach($list as $bit=>$value)
				if($this->is_bit_exist($bit))
					$bit_list[$bit] = $value;					
			return $bit_list;
		}
		
		/**
		 * Sets the bit
		 *
		 * @param int $bit integer value being set
		 */
		
		function set($bit) 	{ $this->set_bit($bit); }
		
		/**
		 * Gets the bit
		 *
		 * @param int $bit integer bit value to be returned
		 * @return integer
		 */
		 
		function get($bit) 	{ return $this->get_bit($bit); }
		
		/**
		 * Adds a bit to the list
		 *
		 * @param int $bit integer value to be added
		 */
		 
		function add($bit) 	{ $this->add_bit($bit); }
		
		/**
		 * Removes a bit from the list
		 *
		 * @param int $bit integer value to be removed
		 */
		 
		function remove($bit)	{ $this->remove_bit($bit); }
		
		/**
		 * Makes a check to see if the bit exists
		 *
		 * @param int $bit integer value to be checked
		 */
		 
		function exists($bit) 	{ return $this->bit_exists($bit); }
				
		
	}
	