<?
        /**
        * Class BASE_CMODEL_API_RESPONSE
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_API_RESPONSE {


        const CODE_SUCCESS  = 200;
        const CODE_FAIL     = 400;

        /**
         * The reference
         * @var string $reference
         */
        protected $_reference     = null;

        /**
         * The message
         * @var string $message
         */
        protected $_messages     = array();

        /**
         * The payload
         * @var string $message
         */
        protected $_payload     = array();

        protected $_exception     = array();

        /**
         * The code
         * @var integer $code
         */
        protected $_code 	= 200;

        protected $_codes   = array("success"=>self::CODE_SUCCESS,"fail"=>self::CODE_FAIL);

        /**
         * The data
         * @var array $data
         */
        protected $_data	= array();

        /**
         * Render
         */
		public function render($jsonp="",$die=true) {

			header("Content-Type: ".HTTP_UTIL::get_content_type("json"));

			try {

            	$payload = JSON_UTIL::encode($this->get_payload());

	        } catch(Exception $e) {
	        	$this->exception($e);
	        	$payload = JSON_UTIL::encode(self::create_payload($this->_code,$this->_messages,[],$this->_exception));
	        }

	        $code = $this->_code && is_int($this->_code) ? $this->_code : self::CODE_FAIL;
	        http_response_code($code);

            if($jsonp)
                $payload = $jsonp."(".$payload.")";

            echo $payload;

            if($die)
            	die;
		}

		public function get_payload() {
			return self::create_payload($this->_code,$this->_messages,$this->_data,$this->_exception,$this->_payload);
		}

        public function create_payload($code,$messages,$data,$exception,$payload=[]) {
        	$code = (int)$code;

        	$payload = $payload ? $payload : [];
            $payload["success"] = $code>=200 && $code<300;
            $payload["code"]    = $code;
            $payload["data"]    = $data;

            if($messages) {
                $payload["message"]     = implode(". ",ARRAY_UTIL::listing($messages,"message"));
                $payload["messages"]    = $messages;
            }

            /*if($this->_reference)
                $payload["ref"] = $this->_reference;*/

            if($exception) {

                $trace = [];
                foreach($exception->getTrace() as $line)
                    $trace[] = value($line,"class")."->".value($line,"function")."() @ ".value($line,"file")." on ".value($line,"line");

                $payload["exception_message"] = $exception->getMessage();
                $payload["exception_code"] = $exception->getCode();
                $payload["exception_trace"] = $trace;
            }

            return $payload;
        }

        /**
         * Response
         */
        public function get_response() {
            return JSON_UTIL::encode($this->get_payload());
        }

        /**
         * Success
         * @param integer $success
         * @param integer $code
         */
		public function success($success=1,$code=self::CODE_SUCCESS) {
            $this->_success = !is_empty($success);
            $this->_code = $code;
        }

        /**
         * Sets data
         * @param string $name
         * @param string $value
         */
        public function data($name,$value)      { $this->_data[$name] = $value; return $this; }

        /**
         * Sets payload
         * @param string $name
         * @param string $value
         */
        public function payload($name,$value)      { $this->_payload[$name] = $value; return $this; }

        public function exception($exception) {

            if(!SYSTEM_MANAGER::is_system_mode_production()) {
        	
            	$this->_exception = $exception;
            }

            return $this;
		}

        /**
         * Sets data
         * @param string $name
         * @param string $value
         */
        public function set_data($value)      { $this->_data = $value; return $this; }

        /**
         * Sets the message
         * @param string $message
         */
        public function set_message($message)       { $this->_messages = array($message); return $this; }

        public function get_messages()              { return $this->_messages; }

        /**
         * Sets the reference
         * @param string $reference
         */
        public function set_reference($reference)       { $this->_reference = $reference; return $this;  }

        /**
         * Sets the code
         * @param integer $code
         */
		public function set_code($code)			{ $this->_code = $code; return $this; }

        /**
         * Sets the error
         * @param string $message
         * @param integer $code
         */
		public function error($message,$code=null) {
			$code = $code ? $code : self::CODE_FAIL;
            $this->_messages[] = array("message"=>$message,"code"=>$code,"type"=>"error");
            $this->_code = $code;
            return $this;
        }

        public function set_error($message,$code=null)  { return $this->error($message,$code); }

        /**
         * Fail
         * @param string $message
         * @param integer $code
         */
        public function fail($message,$code=null)  {
            $code = $code===null ? $this->_codes["fail"] : $code;
            return $this->set_error($message,$code)->success(false,$code);
        }

        /**
         * Returns the code
         * @return integer
         */
        public function get_code()          { return $this->_code; }

        /**
         * Returns the reference
         * @return integer
         */
        public function get_reference()          { return $this->_reference; }

        /**
         * Returns success
         * @return integer
         */
        public function get_success()           { return $this->_success; }

        public function is_success()           { return $this->_success; }

        /**
         * Checks whether is has errors
         * @return boolean
         */
        public function has_errors()                    { return !is_empty($this->_messages); }

        /**
         * Listen for requests
         * @return boolean
         */
        public function listen() {}
	}