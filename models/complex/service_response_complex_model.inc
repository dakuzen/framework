<?
    /**
    * Class BASE_CMODEL_SERVICE_RESPONSE
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_SERVICE_RESPONSE {

		/**
		 * The message
		 * @var string $message
		 */
		protected $_message 	= "";

		/**
		 * The code
		 * @var integer $code
		 */
		protected $_code 	= 0;

        /**
         * The success
         * @var integer $success
         */
		protected $_success 	= 0;


		/**
		 * The response
		 * @var array $response
		 */
		protected $_response 	= array();

		/**
		 * Constructor
		 */
		public function __construct() {

		}

        /**
         * Render
         */
		public function get_response() {
			$payload = array("code"=>$this->_code,"message"=>$this->_message,"response"=>$this->_response,"success"=>$this->_success);

			return JSON_UTIL::encode($payload);
		}

		public function render() {
			die($this->get_response());
		}

        /**
         * The success
         * @param integer $success
         */
		public function success($success=1) 		{ $this->_success = $success; }

		/**
		* Returns the success
		* @return integer
		*/
		public function get_success()			{ return $this->_success; }

		/**
		 * Set the response
		 * @param array $response
		 */
		public function set_response($response)	 		{ $this->_response = $response; return $this; }

		/**
		 * Set the data
		 * @param string $name
		 * @param array $response
		 */
		public function set_data($name,$response)	 	{ $this->_response[$name] = $response; return $this; }

		/**
		 * Set the message
		 * @param string $message
		 */
		public function set_message($message)			{ return $this->message($message); }
		public function message($message)				{ $this->_message = $message; return $this; }

		/**
		 * Set the code
		 * @param integer $code
		 */
		public function code($code)						{ $this->_code = $code; return $this; }
		public function set_code($code)					{ return $this->code($code); }

		/**
		 * Set the error
		 * @param string $message
		 * @param integer $code
		 */
		public function set_error($message,$code=null)	{ $this->_message = $message; $this->_code = $code; return $this; }

		/**
		 * Fail
		 * @param string $message
		 * @param integer $code
		 */
		public function fail($message,$code=null)		{ $this->_message = $message; $this->_code = $code; return $this; }

		/**
		 * Returns the message
		 * @return string
		 */
		public function get_message()					{ return $this->_message; }

		/**
		 * Returns the code
		 * @return integer
		 */
		public function get_code()						{ return $this->_code; }

		/**
		 * Chechs whether it has errors
		 * @return boolean
		 */
		public function has_errors()					{ return !is_empty($this->_message); }

	}