<?
	/**
    * Class EMAIL_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */
	
	class EMAIL_UTIL {
        
        /**
		 * Parse email 
		 * 
	     * @param string $string string
	     * @param boolean $validate whether to validate
	     * @param boolean $throw_expcetion whether to throw an expcetion
	     * 
		 * @return array()
		 */
		public static function parse_email($string,$validate=true,$throw_expcetion=false) {
			
			$errors = array();
			
			$string = trim($string);
			
			if(!$string)
				return array();
			
			$delims = array(";"=>0,","=>0);
			
			foreach(array_keys($delims) as $delim)
				$delims[$delim] = substr_count($string,$delim);
			
			arsort($delims);
			
			$delim = array_shift(array_keys($delims));
			
			$emails = explode($delim,$string);
			
			foreach($emails as $index=>$email) {
				
				$email = trim($email);

				$valid = $validate ? VALIDATE_UTIL::is_valid_email($email) : true;				
				
				if($valid)
					$emails[$index] = $email;
				else
					$errors[] = "Invalid email address: ".$email;					
			}
			
			if($throw_expcetion && $errors)
				throw new Exception(implode(", ",$errors));				

			return $emails;
		}
	}