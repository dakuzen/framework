<?
    /**
    * Class BASE_CMODEL_COOKIE
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_COOKIE {

        /**
         * The data
         * @var array $data 
         */
		protected $_data		= array();
        
        /**
         * The cookie key
         * @var string $cookie_key 
         */
		protected $_cookie_key		= "";
                
        /**
         * The expiry hours
         * @var integer $expiry_hours 
         */
		protected $_expiry_hours	= null;

        /**
         * Constructor
         * @param string $cookie_key 
         */
		public function __construct($cookie_key) {
			$this->_cookie_key = $cookie_key;
			
			$cookie = COOKIE_UTIL::get_cookie($this->_cookie_key);

			if($cookie && is_string($cookie))
				$this->_data = JSON_UTIL::decode($cookie);
		}

        public static function create($cookie_key) {
            return new CMODEL_COOKIE($cookie_key);
        }
		
        /**
         * Get key
         * @param string $key
         * @return boolean 
         * @todo check types
         */
		public function get($key)     { return value($this->_data,$key); }

        public function data()       { return $this->_data; }
        
        /**
         * Sets key
         * @param string $key
         * @param string $value 
         */
		public function set($key,$value)	{ $this->_data[$key] = $value; }
		
        /**
         * Save 
         */
		public function save() {
			$cookie = JSON_UTIL::encode($this->_data);
			COOKIE_UTIL::set_cookie($this->_cookie_key,$cookie,$this->_expiry_hours);
		}
	}