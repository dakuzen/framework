<?
    /**
    * Class FTP_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */

	class FTP_UTIL {

        /**
         * Passive mode
         * @var boolean passive_mode
         */
		protected $_passive_mode	= true;

        /**
         * Timeout
         * @var integer $timeout
         */
		protected $_timeout		= 30;

        /**
         * Connection
         * @var resource $connection
         */
		protected $_connection		= null;

        /**
         * Error message
         * @var string $error_message
         */
		protected $_error_message	= null;

		/**
         * SSL
         * @var string $ssl
         */
		protected $_ssl	= null;

        /**
         * Constructor.
         * @param string $host the host.
         * @param string $user the user.
         * @param string $pass the password.
         * @param integer $port the port number.
         * @param boolean $passive_mode the passive mode.
         */
		public function __construct($host="", $user="", $pass="", $port=21,$options=array("passive_mode"=>true,"ssl"=>false)) {
			$this->_host 			= $host;
			$this->_user 			= $user;
			$this->_pass 			= $pass;
			$this->_port 			= $port;
			$this->_passive_mode	= value($options,"passive_mode",true);
			$this->_ssl				= value($options,"ssl",false);
		}

		public static function create($host="", $user="", $pass="", $port=21,$passive_mode=true) {
			return new FTP_UTIL($host,$user,$pass,$port,$passive_mode);
		}


        /**
         * Set passive mode
         * @param boolean $passive_mode
         */
		public function set_passive_mode($passive_mode) 	{ $this->_passive_mode = $passive_mode; }

        /**
         * Set the error message
         * @param string $error_message
         */
		public function set_error_message($error_message) 	{ $this->_error_message = $error_message; }

        /**
         * Connect to ftp
         * @return boolean
         * @throws Exception
         */
		public function connect() {

			if(!$this->_user)
				throw new Exception("Username not specified");

			DEBUG_UTIL::enable_exception_error_handler();

			try {

				if($this->_ssl)
					$this->_connection = ftp_ssl_connect($this->_host, $this->_port, $this->_timeout);
				else
					$this->_connection = ftp_connect($this->_host, $this->_port, $this->_timeout);

				if($this->_connection) {

					$has_success = @ftp_login($this->_connection, $this->_user, $this->_pass);

					if (!$has_success) {
						throw new Exception("Login failed for ".$this->_user."@".$this->_host);

					} else {
						$has_success = @ftp_pasv($this->_connection, $this->_passive_mode);

						if(!$has_success)
							throw new Exception("Failed to switch to passive mode");
					}

				} else
					throw new Exception("Failed to connect to ".$this->_host.":".$this->_port);

			} catch(Exception $e) {
				self::throw_exception($e);
			}

			DEBUG_UTIL::restore_error_handler();

			return $this;
		}

		public function get_listing($path) { return $this->get_listing($path); }

        /**
         * Get the listing
         * @param string $path
         * @return array
         */
		public function listing($path) {

			$this->has_connection();

			DEBUG_UTIL::enable_exception_error_handler();

			try {

				$contents = @ftp_nlist($this->_connection, $path);

				if(!is_array($contents))
					$contents = array();

			} catch(Exception $e) {
				self::throw_exception($e);
			}

			DEBUG_UTIL::restore_error_handler();

			return $contents;
		}

		public function get($file) {

			$this->has_connection();

			DEBUG_UTIL::enable_exception_error_handler();

			try {

				ob_start();
				$result = @ftp_get($ftp, "php://output", $file, FTP_BINARY);
				$data = ob_get_contents();
				ob_end_clean();
				return $data;

			} catch(Exception $e) {
				self::throw_exception($e);
			}

			DEBUG_UTIL::restore_error_handler();

			return $this;
		}

        /**
         * Download a file
         * @param string $local_file
         * @param string $remote_file
         * @return null
         */
		public function download_file($local_file,$remote_file) {

			$this->has_connection();

			DEBUG_UTIL::enable_exception_error_handler();

			try {

				$has_success = @ftp_get($this->_connection, $local_file, $remote_file, FTP_BINARY);

			} catch(Exception $e) {
				self::throw_exception($e);
			}

			DEBUG_UTIL::restore_error_handler();

			return $this;
		}

        /**
         * Check whether a file exists
         * @param string $file
         * @return boolean
         */
		public function is_file_exist($file) {
			return $this->get_file_size($file)!==-1;
		}

        /**
         * Returns the size of a file
         * @param string $file
         * @return integer
         */
		public function get_file_size($file) {

			DEBUG_UTIL::enable_exception_error_handler();

			try {

				$size = @ftp_size($this->_connection, $file);

			} catch(Exception $e) {
				self::throw_exception($e);
			}

			DEBUG_UTIL::restore_error_handler();

			return $size;
		}

        /**
         * Uploads a file
         * @param string $local_file
         * @param string $remote_file
         * @return boolean
         * @throws Exception
         */
		public function put($local_file,$remote_file) {

			$this->has_connection();

			$has_success = false;

			DEBUG_UTIL::enable_exception_error_handler();

			try {

				if(is_file($local_file)) {

					$has_success = @ftp_put($this->_connection, $remote_file, $local_file, FTP_BINARY);

				} else
					throw new Exception("The local file ".$local_file." does not exist");

			} catch(Exception $e) {
				self::throw_exception($e);
			}

			DEBUG_UTIL::restore_error_handler();

			return $has_success;
		}

		public function upload_file($local_file,$remote_file) { return $this->put($local_file,$remote_file); }

        /**
         * Deletes a file
         * @param string $remote_file
         * @return boolean
         */
		public function delete_file($remote_file) {

			$this->has_connection();

			DEBUG_UTIL::enable_exception_error_handler();

			try {

				$has_success = @ftp_delete($this->_connection,$remote_file);

			} catch(Exception $e) {
				self::throw_exception($e);
			}

			DEBUG_UTIL::restore_error_handler();

			return $has_success;
		}

		public function change_directory($dir) { return $this->cd($dir); }

        /**
         * Changes directory
         * @param string $dir
         */
		public function cd($dir) {

			$this->has_connection();

			DEBUG_UTIL::enable_exception_error_handler();

			try {

				$has_success = @ftp_chdir($this->_connection,$dir);

			} catch(Exception $e) {
				self::throw_exception($e);
			}

			DEBUG_UTIL::restore_error_handler();

			return $has_success;
		}

        /**
         * Check whether there is a connection
         * @return boolean
         */
		public function has_connection() {

			if(is_empty($this->_connection))
				throw new Exception("There is no active connection");
		}

        /**
         * Disconnects
         * @return boolean
         */
		public function disconnect() {

			$this->has_connection();

			$has_success = true;

			DEBUG_UTIL::enable_exception_error_handler();

			try {

					$has_success = @ftp_close($this->_connection);

			} catch(Exception $e) {
				self::throw_exception($e);
			}

			DEBUG_UTIL::restore_error_handler();

			return $has_success;
		}

        /**
         * Throws an exception
         * @param object $exception
         * @throws Exception
         */
		public function throw_exception($exception) {

			DEBUG_UTIL::restore_error_handler();
			throw $exception;

		}
	}