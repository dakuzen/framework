<?
    /**
    * Class CONSTANT  
    *
    * @package Library
    * @subpackage Common
    */

	class CONSTANT {
		
		const COUNTRY_CANADA 		= "CA";
		const COUNTRY_UNITED_STATES = "US";
		
		const COUNTRY_CA 			= "CA";
		const COUNTRY_US			= "US";		
		
		const LANGUAGE_EN			= "en";	
		const LANGUAGE_FR			= "fr";	
		
		const TRUE					= 1;
		const FALSE					= 0;

		const SECONDS_HOUR 			= 3600;
		const SECONDS_DAY 			= 86400;
		const SECONDS_WEEK			= 604800;
		
		const BYTES_MB 				= 1048576;
		const BYTES_GB 				= 1073741824;
		
		const NOT_SET				= "ns";
		const NA					= self::NOT_SET;
		
		const BIT_01 = "1";
		const BIT_02 = "2";
		const BIT_03 = "4";
		const BIT_04 = "8";
		const BIT_05 = "16";
		const BIT_06 = "32";
		const BIT_07 = "64";
		const BIT_08 = "128";
		const BIT_09 = "256";
		const BIT_10 = "1024";
		const BIT_11 = "2048";
		const BIT_12 = "4096";
		const BIT_13 = "8192";
		const BIT_14 = "16384";
		const BIT_15 = "32768";		
		
        /**
         * Returns yes no list
         * @return array 
         */
		static function get_yes_no_list() 		{ return array(self::TRUE=>"Yes",self::FALSE=>"No"); }
                
        /**
         * Returns no yes list
         * @return array 
         */
		static function get_no_yes_list() 		{ return array(self::FALSE=>"No",self::TRUE=>"Yes"); }
                
        /**
         * Returns yes no name
         * @param string $value
         * @return boolean 
         */
		static function get_yes_no_name($value) 	{ return get_value(self::get_yes_no_list(),$value); }
	}



