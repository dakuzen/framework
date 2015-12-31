<?
	class BCMODEL_URL {

        /**
         * The value
         * @var mixed $value 
         */
        protected $_parts   = array();

        /**
         * Constructor
         * @param integer $time 
         */
        public function __construct($url="") {
            if($url) {
                $this->_parts = parse_url($url);

                parse_str(value($this->_parts,"query"),$this->_parts["query"]);
            }
        }

        public static function create($url="") {
            return new CMODEL_URL($url);
        } 

        public function host() { 
            if($host=@func_get_arg(0)) {
                $this->_parts["host"] = $host;
                return $this;
            }

            return value($this->_parts,"host"); 
        }

        public function url() {

            $url = value($this->_parts,"scheme")."://".value($this->_parts,"host").value($this->_parts,"path");

            if($query=value($this->_parts,"query"))
                $url .= "?".http_build_query($query);

            return $url;
        }

        public function query($key,$value) { 
            $this->_parts["query"][$key] = $value;
            return $this;
        }
       
	}