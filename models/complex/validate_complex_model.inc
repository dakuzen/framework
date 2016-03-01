<?
    /**
    * Class BASE_CMODEL_VALIDATE
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_VALIDATE {

        /**
         * The value
         * @var mixed $value 
         */
        private $_value = null;
        private $_throw = false;

        /**
         * Constructor
         * @param integer $time 
         */
        public function __construct($value,$throw=false) {
            $this->_value = $value;
            $this->_throw = $throw;
        }

        public static function create($value,$throw=false) {
            return new BASE_CMODEL_VALIDATE($value,$throw);
        } 

        public function email() {
            if(!VALIDATE_UTIL::is_valid_email($this->_value)) {
                if($this->_throw)
                    throw new Exception("Invalid email");
                return false;
            }

            return true;
        }

        public function phone() {
            if(!VALIDATE_UTIL::is_valid_phone($this->_value)) {
                if($this->_throw)
                    throw new Exception("Invalid phone number");
                return false;
            }

            return true;
        }

        public function filter($value,$valid) {
            
            $values = is_array($value) ? $value : array();

            $filtered = [];
            foreach($values as $value) {
                if(in_array($value, $valid))
                    $filtered[] = $value;
            }

            return $filtered;
        }

        public function values($value,$valid) {
            
            $values = is_array($value) ? $value : array();

            foreach($values as $value) {
                if(!in_array($value, $valid))
                    return false;
            }

            return true;
        }		
	}