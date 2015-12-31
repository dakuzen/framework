<?
	/**
	* Class BASE_CMODEL_API
	*
	* @package Framework
	* @subpackage Cmodels
	*/

	class BASE_CMODEL_API {
        
        /**
         * The registry
         * @var object $registry 
         */
        protected $_registry            = null;
        
        /**
         * The api response cmodel
         * @var object $api_response_cmodel
         */
        protected $_api_response_cmodel = null;

        /**
         * Constructor 
         */
        public function __construct() { 
            $this->_api_response_cmodel = new BASE_CMODEL_API_RESPONSE();
            $this->_registry = REGISTRY::get_instance();
        }

        public static function create() { 
            $class = get_called_class();
            return new $class();  
        }        
                
        /**
         * Returns the registry
         * @return object 
         */
        public function registry()          { return $this->_registry; }
        
        /**
         * Returns the api response
         * @return object 
         */
        public function response()              { return $this->_api_response_cmodel; }
        public function get_api_response()      { return $this->response(); }
        
        /**
         * The parm
         * @param string $path
         * @return mixed 
         */
        public function parm($path)             { return $this->registry()->get_parm($path); }
        
        /**
         * Listen for requests
         * @return boolean 
         */
        public function listen() {
            return $this->_api_response_cmodel->render();
        }
	                
}