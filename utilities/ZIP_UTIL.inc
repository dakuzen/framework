<?
    /**
    * Class ZIP_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */

	class ZIP_UTIL extends ZipArchive {

        /**
         * The zip file.
         * @var string $zip_file
         */
		protected $_zip_file 		= null;

        /**
         * The error messages.
         * @var array $error_messages
         */
		protected $_error_messages	= array();

		/**
         * Construct
         *
         * @param string $zip_file the zip file
         */
		function __construct($zip_file=null) {

			if(!extension_loaded("zip"))
				throw new Exception("The zip extension does not exist");

			$this->_zip_file = $zip_file;
			if($zip_file)
				$this->open($zip_file,ZIPARCHIVE::CREATE);
		}

        /**
         * Creates a zip file
         * @param string $zip_file the zip file
         * @return boolean
         */
		function create($zip_file) {

			$dir = dirname($zip_file);

			if(!is_dir($dir))
				throw new Exception("The directory to create the zip does not exist");

			return new ZIP_UTIL($zip_file);
		}

        /**
         * Deconstruct
         */
		function __deconstruct() {
			$this->close();
		}

		/**
		 * Closes the message window
		 *
	 	 */

		function close() {

			if($this->_error_messages)
				return false;

			return parent::close();
		}

	 	/**
         * Downloads a file
         * @param string $filename
         * @param bool $die
         * @return file
         */
		function download($filename=null,$die=true) {
			$filename = $filename ? $filename : MISC_UTIL::get_guid().".zip";
			$this->close();
			return HTTP_UTIL::stream_file($this->_zip_file,$filename,$die);
		}

	 	/**
         * Stream Download
         * @param string $filename
         * @param bool $die die after streaming
         * @return boolean
         */
		function stream_download($filename,$die=true) {
			return HTTP_UTIL::stream_stream($this->getStream($filename),basename($filename),$die);
		}

		function stream($filename,$die=true) {
			return $this->stream_download($filename,$die);
		}

		/**
         * Adds a file
         * @param string $file
         * @param string $local_filename
         * @return boolean
         */
		function add_file($file,$local_filename=null) {
			return $this->addFile($file,$local_filename);
		}

		function add($file,$local_filename=null) {
			return $this->add_file($file,$local_filename);
		}

        /**
         * Adds a string
         * @param string $filename
         * @param string $string
         */
		function add_string($filename,$string) {
			return $this->addFromString($filename,$string);
		}

        /**
         * Adds a file
         * @param string $file
         * @param string $local_filename
         * @return boolean
         * @throws Exception
         */
		function addFile($file,$local_filename=null) {

			$local_filename = $local_filename ? $local_filename : basename($file);

			if(!is_file($file))
				throw new Exception("The file ".$file." does not exist");

			self::error_handler();

			parent::addFile($file,$local_filename);

			self::reset_error_handler();

			return $this;
		}

        /**
         * Adds a directory
         * @param string $dir
         * @param string $local_dir
         * @return boolean
         */
		function add_directory($dir, $local_dir=null) {
			return $this->addDirectory($dir,$local_dir);
		}

        /**
         * Adds directory contents
         * @param string $dir
         * @return boolean
         * @throws Exception
         */
		function add_directory_contents($dir) {

			if(!is_dir($dir))
				throw new Exception("The directory ".$dir." does not exist");

			$dir = FILE_UTIL::get_sanitized_directory($dir,"/");

			$filenames = FILE_UTIL::get_directory_listing($dir);

			foreach($filenames as $filename) {

				$file = $dir.$filename;

				if(is_file($file))
					$this->addFile($file,$filename);

				elseif(is_dir($file))
					$this->addDirectory($file."/",$filename);
			}

			return true;
		}

        /**
         * Add a directory
         * @param string $dir
         * @param string $local_dir
         * @return boolean
         * @throws Exception
         */
		function addDirectory($dir, $local_dir="") {

			if(!is_dir($dir))
				throw new Exception("The directory ".$dir." does not exist");

			$filenames = FILE_UTIL::get_directory_listing($dir);

			if($local_dir)
				$path = FILE_UTIL::get_sanitized_directory($local_dir,"/");
			else
				$path = FILE_UTIL::get_sanitized_directory(basename($dir),"/");

			$dir = FILE_UTIL::get_sanitized_directory($dir,"/");

			$this->addEmptyDir($path);

			foreach($filenames as $filename) {

				$file = $dir.$filename;

				if(is_file($file))
					$this->addFile($file,$path.$filename);

				elseif(is_dir($file))
					self::addDirectory($file."/",$path.$filename);
			}

			return true;
		}

		/**
		 * Returns an error message
		 *
		 * @return string
	 	 */
		function get_error_message() {
			return implode(", ",$this->get_error_messages());
		}

		/**
		 * Returns an error message
		 *
		 * @return array
	 	 */
		function get_error_messages() {
			return array_merge($this->_error_messages,array($this->GetStatusString()));
		}

		function get_zip_filename() { return basename($this->_zip_file); }

		public static $_previous_error_handler = null;


		static function error_handler() {

			self::$_previous_error_handler = set_error_handler(function($errno, $errmsg, $filename, $linenum) {
			 	if(!DEBUG_UTIL::is_reportable($errno,$errmsg))
			 		return;

				if(self::$_previous_error_handler)
					set_error_handler(self::$_previous_error_handler);
				throw new Exception($errmsg,$errno);
			});
		}

		static function reset_error_handler($return=null) {

			if(self::$_previous_error_handler) {
				set_error_handler(self::$_previous_error_handler);
				self::$_previous_error_handler = null;
			}
            /**
             * @var $return Exception
             */
			if(is_a($return,"Exception"))
				throw $return;

			return $return;
		}
	}