<?
    /**
    * Class BASE_CMODEL_FACEBOOK_BATCH_CLIENT
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_FACEBOOK_BATCH_CLIENT {
		
		/**
		 * facebook batch client request cmodels
		 * @var array $facebook_batch_client_request_cmodels 
		 */
		protected $_facebook_batch_client_request_cmodels	= array();

		/**
		 * facebook client cmodel
		 * @var object $facebook_client_cmodel 
		 */
		protected $_facebook_client_cmodel			= null;	

		/**
		 * The access token
		 * @var string $access_token
		 */
		protected $_access_token				= null;		

		/**
		 * Constructor
		 * @param object $facebook_client_cmodel 
		 */
		public function __construct($facebook_client_cmodel) {
			$this->_facebook_client_cmodel = $facebook_client_cmodel;

			$this->_access_token = $this->_facebook_client_cmodel->getAccessToken();
		}

        /**
         * add request
         * @param string $url
         * @param string $method
         * @param array $data 
         */
		public function add_request($url,$method="GET",$data=array(),$reference="") {
			$this->_facebook_batch_client_request_cmodels[] = new BASE_CMODEL_FACEBOOK_BATCH_CLIENT_REQUEST($url,$method,$data,$reference);
		}
				
        /**
         * process
         */
		public function process() {

			$facebook_batch_client_response_cmodels = $requests = array();
			
			foreach($this->_facebook_batch_client_request_cmodels as $facebook_batch_client_request_cmodel)
				$requests[] = $facebook_batch_client_request_cmodel->get_array();
	
			$group_requests = array_chunk($requests,50);
			
			$facebook_client_cmodel_cmodel = MODEL_FACEBOOK::create_client();
			
			foreach($group_requests as $requests) {
				
				$post["batch"] 		= JSON_UTIL::encode($requests);
				$post["access_token"] 	= $this->_access_token;

				$reponse = $facebook_client_cmodel_cmodel->api("/","POST",$post);
				
                                if(is_array($reponse)) {

					foreach($reponse as $index=>$item) {

						$data = JSON_UTIL::decode(get_value($item,"body"));

                                                $code = get_value($item,"code");

                                                $reference = get_value($requests,array($index,"reference"));

						$facebook_batch_client_response_cmodels[] = new BASE_CMODEL_FACEBOOK_BATCH_CLIENT_RESPONSE($code,$data,$reference);
					}
				}
			}
			
			return $facebook_batch_client_response_cmodels;
		}

	}
	
        /**
        * Class BASE_CMODEL_FACEBOOK_BATCH_CLIENT_REQUEST
        *
        * @package Framework
        * @subpackage Cmodels
        */
        
	class BASE_CMODEL_FACEBOOK_BATCH_CLIENT_REQUEST {
            
                /**
                 * The url
                 * @var string $url 
                 */
		protected $_url		= "";
                
                /**
                 * The method
                 * @var string $method 
                 */
		protected $_method	= "";
                
                /**
                 * The data
                 * @var array $data 
                 */
                protected $_data        = "";

                /**
                 * The reference
                 * @var string $reference 
                 */
                protected $_reference        = "";

                /**
                 * Consturctor
                 * @param string $url
                 * @param string $method
                 * @param array $data 
                 */
		public function __construct($url="",$method="",$data="",$reference="") {
			$this->_url = $url;
			$this->_method = $method;
			$this->_data = $data;
                        $this->_reference = $reference;
		}

                /**
                 * Sets the url
                 * @param string $url 
                 */
		public function set_url($url)			{ $this->_url = $url; }
                
                 /**
                 * Sets the reference
                 * @param string $reference 
                 */
                public function set_reference($reference)       { $this->_reference = $reference; }

                /**
                 * Sets the method
                 * @param string $method 
                 */
		public function set_method($method)		{ $this->_method = $method; }
                
                /**
                 * Sets the data
                 * @param array $data 
                 */
		public function set_data($data)			{ $this->_data = $data; }

                /**
                 * Returns the url
                 * @return string 
                 */
		public function get_url()			{ return $this->_url; }
                
                /**
                 * Returns the method
                 * @return string 
                 */
		public function get_method()			{ return $this->_method; }
                
                /**
                 * Returns the data
                 * @return array 
                 */
                public function get_data()                      { return $this->_data; }
               
                /**
                 * Returns the reference
                 * @return string 
                 */
                public function get_reference()                      { return $this->_reference; }
		
                /**
                 * Returns the array
                 * @return array
                 */
		public function get_array() {
			
			$array = array();			
			$array["relative_url"]       = $this->get_url();
			$array["method"]             = $this->get_method();
                        $array["reference"]          = $this->get_reference();

			return $array; 
		}
	}
        
        /**
        * Class BASE_CMODEL_FACEBOOK_BATCH_CLIENT_RESPONSE
        *
        * @package Framework
        * @subpackage Cmodels
        */
	
	class BASE_CMODEL_FACEBOOK_BATCH_CLIENT_RESPONSE {

                /**
                 * The code
                 * @var string $code 
                 */
		protected $_code	= "";
                
                /**
                 * The data
                 * @var array $data 
                 */
                protected $_data        = "";

                /**
                 * The reference
                 * @var string $reference 
                 */
                protected $_reference        = "";

                /**
                 * Constructor
                 * @param string $code
                 * @param array $data 
                 */
		public function __construct($code="",$data=array(),$reference="") {
			$this->_code = $code;
			$this->_data = $data;
                        $this->_reference = $reference;
		}

                /**
                 * Sets the code
                 * @param string $code 
                 */
		public function set_code($code)			{ $this->_code = $code; }
                
                /**
                 * Sets the data
                 * @param array $data 
                 */
		public function set_data($data)			{ $this->_data = $data; }
	
                /**
                 * Checks whether has success
                 * @return boolean 
                 */
		public function has_success()			{ return (int)$this->_code==200; }
                
                /**
                 * Reutnrs the code
                 * @return string 
                 */
		public function get_code()			{ return $this->_code; }
                
                /**
                 * Reutnrs the data
                 * @return array 
                 */
                public function get_data()                      { return $this->_data; }


                /**
                 * Reutnrs the reference
                 * @return string 
                 */
                public function get_reference()                      { return $this->_reference; }
                
                /**
                 * Get
                 * @param string $name
                 * @param mixed $default
                 * @return mixed 
                 */
		public function get($name,$default=null)	{ return get_value($this->_data,$name,$default); }
                
                /**
                 * Returns the error message
                 * @return mixed 
                 */
		public function get_error_message()		{ return $this->get(array("error","message")); }
	}	