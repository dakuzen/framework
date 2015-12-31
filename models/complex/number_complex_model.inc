<?
	class BASE_CMODEL_NUMBER extends BASE_CMODEL_OBJECT {
            
        /**
         * The time
         * @var integer $number 
         */
		private $_number	= null;

        /**
         * Constructor
         * @param integer $time 
         */
		public function __construct($number) {
			$this->_number = $number;
		}

        public static function create($number) { return new BASE_CMODEL_NUMBER($number); }
		

        public function abbreviated($options=array()) {

            $number = $this->_number;

            if($this->_number>1000) {

                $thousand_precision = get_value($options,array("throusand","precision"),1);

                $number = round(($this->_number/1000),$thousand_precision,PHP_ROUND_HALF_DOWN)."k";
            }

            return $number;
        }
    
	}