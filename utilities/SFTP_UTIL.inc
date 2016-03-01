<?
	/**
	* Class SFTP_UTIL
	*
	* @package Framework
	* @subpackage Utilities
	*/
	
	class SFTP_UTIL extends SSH_UTIL {
        
        /**
         * Constructor
         * @param string $host
         * @param integer $port
         * @throws Exception 
         */
		public function __construct($host,$port=22) {

			if(!$host)
				throw new Exception("No host address supplied");				
			
			SERVER_UTIL::add_include_path(DIR_FRAMEWORK."packages/phpseclib/");

			require_once("Net/SFTP.php");		
			require_once("Crypt/RSA.php");

			//define('NET_SFTP_LOGGING', NET_SFTP_LOG_COMPLEX); // or NET_SFTP_LOG_SIMPLE

			$this->start_error_handler();

			$this->_connection = new Net_SFTP($host);

			$this->complete_error_handler();
		}

		public static function create($host,$port=22) {
			return new SFTP_UTIL($host,$port);
		}
         
        public function listing() {
        	return $this->_connection->nlist();
        }

        public function mkdir($dir) {
        	$result = $this->_connection->mkdir($dir);

        	if(!$result)
        		throw new Exception("Failed to make directory");
        	
        	return $this;
        }

        public function cd($dir) {
        	$result = $this->_connection->chdir($dir);

        	if(!$result)
        		throw new Exception("Failed to change directory");
        	
        	return $this;
        }

        public function mv($source,$dest) {
        	$result = $this->_connection->rename($source,$dest);

        	if(!$result)
        		throw new Exception("Failed to move file");
        	
        	return $this;
        }

        public function delete($file) {
        	$result = $this->_connection->delete($file);

        	if(!$result)
        		throw new Exception("Failed to delete file");
        	
        	return $this;
        }
 
        /**
         * Exec
         * @param string $exec 
         */
		public function exec($exec) {}

        /**
         * pwd
         * @return string 
         */
		public function pwd() { return $this->_connection->pwd(); }

        /**
         * Put
         * @param string $remote_file
         * @param array $data
         * @return boolean
         * @throws Exception 
         */
		public function put($remote_file,$data) {

			$data = str_replace(array("\r\n","\r"),array("\n",""),$data);

			$remote_dir 		= dirname($remote_file);
			$remote_filename 	= basename($remote_file);

			$this->start_error_handler();

			if($remote_dir)
				if(!$this->_connection->chdir($remote_dir))
					throw new Exception($this->_connection->getLastSFTPError().": ".$remote_dir);

			$return = $this->_connection->put($remote_file,$data);
			$this->complete_error_handler();

			if(!$return)
				throw new Exception("Failed to put file: ".$this->_connection->getLastSFTPError());
			
			return $this;
		}

		public function get($remote_file,$local_file=null) {
			
			if($local_file===null)
				return $this->_connection->get($remote_file);

			$this->_connection->get($remote_file,$local_file);

			return $this;
		}		
	}
