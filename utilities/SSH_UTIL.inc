<?
    /**
    * Class SSH_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */
	
	class SSH_UTIL {

        /**
         * the connection
         * @var object $connection 
         */
		protected $_connection 		= null;
        
        /**
         * previous handler
         * @var object $previous_handler 
         */
		protected $_previous_handler 	= null;
                
        /**
         * The debug
         * @var boolean $debug 
         */
		protected $_debug 		= false;


		/**
		 * Creates a new twitter client
		 *
		 * @param string $host the host address of the SSH connection
		 * @param string $port the port of the SSH connection
                 * 
		 * @return null
		 */
		public function __construct($host,$port=22) {

			if(!$host)
				throw new Exception("No host address supplied");				
			
			SERVER_UTIL::add_include_path(DIR_FRAMEWORK."packages/phpseclib/");

			require_once("Net/SSH2.php");		
			require_once("Crypt/RSA.php");
			
			$this->start_error_handler();

			$this->_connection = new Net_SSH2($host);

			$this->complete_error_handler();
		}

        /**
         * Debug
         * @param boolean $debug 
         */
		public function debug($debug=true) { $this->_debug = $debug; }

        /**
         * connect password
         * @param string $user
         * @param string $password
         * @throws Exception 
         */
		public function connect_password($user,$password) {
			
			if(!$user)
				throw new Exception("No user supplied");

			if(!$password)
				throw new Exception("No password supplied");

			$this->start_error_handler();

			$exception = null;
			if(!$this->_connection->login($user,$password))
				$exception = new Exception("Failed to login using password authentication");

			$this->complete_error_handler();

			if($exception)
				throw $exception;

			return $this;
		}

        /**
         * connect key
         * @param string $user
         * @param string $key
         * @param string $password
         * @throws Exception 
         */
		public function connect_key($user,$key,$password="") {

			if(!$user)
				throw new Exception("No user supplied");

			if(!$key)
				throw new Exception("No key supplied");


			$crypt_rsa = new Crypt_RSA();
			
			if($password)
				$crypt_rsa->setPassword($password);

			$crypt_rsa->loadKey($key);

			$this->start_error_handler();

			$exception = null;
			if(!$this->_connection->login($user,$crypt_rsa))
				$exception = new Exception("Failed to login using key authentication");

			$this->complete_error_handler();

			if($exception)
				throw $exception;

			return $this;
		}

        /**
         * exec
         * @param string $exec
         * @return string 
         */
		public function exec($exec) {
			
			if($this->_debug)
				p($exec);

			$this->start_error_handler();
			$return = $this->_connection->exec($exec);
			$this->complete_error_handler();

			return $return;
		}

        /**
         * Read
         * @param string $exec 
         */
		public function read($exec) {
			return $this->_connection->read($exec);		
		}

        /**
         * Write
         * @param string $exec
         */
		public function write($exec) {
			return $this->_connection->write($exec);		
		}

        /**
         * error handler
         * @param string $errno
         * @param string $errmsg
         * @param string $filename
         * @param string $linenum
         * @return boolean
         * @throws Exception 
         */
		public function error_handler($errno, $errmsg, $filename, $linenum) {
			
			if(!DEBUG_UTIL::is_reportable($errno,$errmsg)) 
				return false;

			throw new Exception($errmsg);
		}

        /**
         * start error handler
         */
		protected function start_error_handler() {
			$this->_previous_handler = set_error_handler(array($this,"error_handler"));
		}

        /**
         * complete error handler 
         */
		protected function complete_error_handler() {
			set_error_handler($this->_previous_handler);
		}
	}
