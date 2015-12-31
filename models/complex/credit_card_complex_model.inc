<?php
        /**
        * Class BASE_CMODEL_CREDIT_CARD
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_CREDIT_CARD extends MODEL {
                
                /**
                 * The credit card id
                 * @var string $credit_card_id
                 */
		protected $_credit_card_id 	= null;	
                
                /**
                 * The number
                 * @var string $number
                 */
		protected $_number 		= "";
                
                /**
                 * The expiry year
                 * @var integer $expiry_year
                 */
		protected $_expiry_year		= null;
                
                /**
                 * The expiry month
                 * @var integer $expiry_month
                 */
		protected $_expiry_month	= null;
                
                /**
                 * The cvv2
                 * @var integer $cvv2
                 */
		protected $_cvv2 		= "";
                
                /**
                 * The subscription id
                 * @var integer $subscription_id
                 */
		protected $_subscription_id	= null;
                
                /**
                 * The type
                 * @var string $type
                 */
		protected $_type		= null;
                
                /**
                 * The name
                 * @var string $name
                 */
		protected $_name		= null;
		
		const TYPE_VISA			= "V";
		const TYPE_MC			= "M";
		const TYPE_AMEX			= "A";
		
                /**
                 * Returns type list
                 * @return array 
                 */
		static function get_type_list() { return array(self::TYPE_VISA=>"Visa",self::TYPE_MC=>"Mastercard",self::TYPE_AMEX=>"American Express"); }

                /**
                 * Constructor
                 * @param string $number
                 * @param integer $expiry_month
                 * @param integer $expiry_year
                 * @param integer $cvv2
                 * @param string $name
                 * @param string $type 
                 */
		function __construct($number=null,$expiry_month=null,$expiry_year=null,$cvv2=null,$name=null,$type=null) {
			$this->_number 			= $number;
			$this->_expiry_month		= $expiry_month;
			$this->_expiry_year		= $expiry_year;
			$this->_cvv2			= $cvv2;
			$this->_type			= $type;
			$this->_name			= $name;			
		}

                /**
                 * Returns the number of the card
                 * @return string 
                 */
		function get_number() 			{ return $this->_number; }
                
                /**
                 * Returns the expiry month of the card
                 * @return integer 
                 */
		function get_expiry_month() 		{ return $this->_expiry_month; }
                
                /**
                 * Returns the expiry year of the card
                 * @return integer 
                 */
		function get_expiry_year() 		{ return $this->_expiry_year; }
                
                /**
                 * Returns the full expiry year of the card
                 * @return integer 
                 */
		function get_full_expiry_year() 	{ return strlen($this->get_expiry_year())==2 ? "20".$this->get_expiry_year() : $this->get_expiry_year(); }
		
                /**
                 * Returns the padding expiry month of the card
                 * @return string 
                 */
                function get_padding_expiry_month() 	{ return str_pad($this->get_expiry_month(), 2, '0', STR_PAD_LEFT); }
			
                /**
                 * Returns the cvv2 of the card
                 * @return integer 
                 */
		function get_cvv2() 			{ return $this->_cvv2; }
                
                /**
                 * Returns the credit card id
                 * @return integer 
                 */
		function get_credit_card_id() 		{ return $this->_credit_card_id; }
                
                /**
                 * Returns the subscription id
                 * @return integer 
                 */
		function get_subscription_id() 		{ return $this->_subscription_id; }
                
                /**
                 * Returns the type of the card
                 * @return integer 
                 */
		function get_type() 			{ return $this->_type; }
                
                /**
                 * Returns the name
                 * @return string
                 */
		function get_name() 			{ return $this->_name; }

                /**
                 * Set the number
                 * @param string $value 
                 */
		function set_number($value) 		{ $this->_number = $value; }
                
                /**
                 * Set the cvv2
                 * @param string $value 
                 */
		function set_cvv2($value) 		{ $this->_cvv2 = $value; }
                
                /**
                 * Set the credit card id
                 * @param string $value 
                 */
		function set_credit_card_id($value) 	{ $this->_credit_card_id = $value; }
                
                /**
                 * Set the subscription id
                 * @param string $value 
                 */
		function set_subscription_id($value) 	{ $this->_subscription_id = $value; }
                
                /**
                 * Set the type
                 * @param string $value 
                 */
		function set_type($value) 		{ $this->_type = $value; }
                
                /**
                 * Set the name
                 * @param string $value 
                 */
		function set_name($value) 		{ $this->_name = $value; }
		
                /**
                 * Returns the type name
                 * @return string 
                 */
		function get_type_name() 		{ return get_value(self::get_type_list(),$this->get_type()); }
				
                /**
                 * Returns the masked number
                 * @return float 
                 */
		function get_masked_number() {
			$number = strlen($this->get_number()) > 4 ? substr($this->get_number(),-4) : $this->get_number();
			return sprintf("%'*16s",$number);
		}		
		
                /**
                 * Checks whether the type is visa
                 * @return boolean 
                 */
		function is_type_visa()			{ return $this->get_type()==self::TYPE_VISA; }
                
                /**
                 * Checks whether the type is mastercard
                 * @return boolean 
                 */
		function is_type_mastercard()		{ return $this->get_type()==self::TYPE_MC; }
                
                /**
                 * Checks whether the type is amex
                 * @return boolean 
                 */
		function is_type_amex()			{ return $this->get_type()==self::TYPE_AMEX; }		
		
                /**
                 * Enables the visa type
                 * @return string 
                 */
		function enable_type_visa()		{ return $this->set_type(self::TYPE_VISA); }
                
                /**
                 * Enables the mastercard type
                 * @return string 
                 */
		function enable_type_mastercard()	{ return $this->set_type(self::TYPE_MC); }
                
                /**
                 * Enables the amex type
                 * @return string 
                 */
		function enable_type_amex()		{ return $this->set_type(self::TYPE_AMEX); }		
		
		/**
                 * Validates
                 * @return boolean 
                 */
		function validate() {
			
			if(!trim($this->get_name())) 
				$this->add_error_message("Credit card name is not supplied");				
			
			if(!trim($this->get_number())) 
				$this->add_error_message("Credit card number is not supplied");	
			
			if(!trim($this->get_cvv2())) 
				$this->add_error_message("Credit card verification number not supplied");	
				
			
			return !$this->has_error_messages();
		}		
	}	