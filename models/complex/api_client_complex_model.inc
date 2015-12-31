<?
    /**
    * Class BASE_CMODEL_API_CLIENT
    *
    * @package Framework
    * @subpackage Cmodels
    */

    class BASE_CMODEL_API_CLIENT {
        
        /**
         * The data
         * @var array $data 
         */
        protected $_data = array();
       
        /**
         * The debug
         * @var bool $debug
         */
        protected $_debug = false;                

        /**
         * process the response
         * @param array $data
         * @return boolean 
         */
        public function process_response($data) { 
                
            if(!$data)
                throw new Exception("The response is empty");

            $this->_data = JSON_UTIL::decode($data);

            if(!is_array($this->_data))
                throw new Exception("The response is invalid or corrupted: ".$data);

            $success = get_value($this->_data,"success");

            if(!$success) 
                throw new Exception(get_value($this->_data,"message"));

            return $this->_data;
        }

        /**
         * Data
         * @param string $path
         * @param mixed $default
         * @return mixed 
         */
        public function data($path,$default=null) {
            $path = is_array($path) ? $path : array($path);
            return get_value($this->_data,array_merge(array("data"),$path),$default);
        }

        /**
         * Get Data
         * @return mixed 
         */
        public function get_data() {
            return $this->_data;
        } 

        /**
         * Set debug
         */
        public function debug($debug=true) {
            $this->_debug = $debug;
            return $this;
        }                 

        /**
         * Is debug
         * @return bool
         */
        public function is_debug() {
            return $this->_debug;
        }

        /**
         * Is debug
         * @return bool
         */
        public function log($log) {
            if($this->is_debug())
            	echo "LOG: ".$log."\n\n";
        }
    }