<?

    /**
    * Class MODEL
    *
    * @package Library
    * @subpackage Common
    */
	class MODEL extends MESSAGE {

        /**
         * The data
         * @var array $data
         */

        protected static $cache = array();

        protected $_data = array();

        const DATA_APPEND = "append";

        public function data() {
            $args = func_get_args();

            if(count($args)==2) {

                $this->_data[$args[0]] = $args[1];
                return $this;

            } elseif(count($args)==3) {

                if(value($args,2)==self::DATA_APPEND)
                    $this->_data[$args[0]][] = $args[1];
                else
                    $this->_data[$args[0]][$args[1]] = $args[2];

                return $this;
            }

            return value($this->_data,value($args,0),value($args,1));
        }

		public function set_property($name,$value) 	 { return $this->data($name,$value); }
		public function get_property($name)		     { return $this->data($name); }

		public function has_property($name)          { return $this->has_data($name); }
        public function remove_data($name)           { unset($this->_data[$name]); return $this; }
        public function has_data($name)              { return array_key_exists($name,$this->_data); }

        public function has_cache($name)             { return array_key_exists($name,self::$cache); }


        public function cache() {
            $args = func_get_args();

            if(count($args)==2) {
                self::$cache[$args[0]] = $args[1];
                return $this;
            }

            return value(self::$cache,value($args,0),value($args,1));
        }
	}