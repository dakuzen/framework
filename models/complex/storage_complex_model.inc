<?
	class BCMODEL_STORAGE {

		public static $_previous_error_handler = null;

		function is_local() { return false; }
		function is_s3() 	{ return false; }

		public static function create_object() {
			return BCMODEL_STORAGE_OBJECT::create();
		}

		static function error_handler($debug=true) {

			if($debug)
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

			if(is_a($return,"Exception"))
				throw $return;

			return $return;
		}
	}

	class BCMODEL_STORAGE_OBJECT {

		/**
		 * the filename
		 * @var string $filename
		 */
		protected $_filename 		= "";

		/**
		 * The directory
		 * @var string $directory
		 */
		protected $_directory		= "";

		protected $_filesize		= 0;

		/**
		 * modified time
		 * @var integer $modified_time
		 */
		protected $_modified	= 0;

		protected $_is_file		= false;
		protected $_is_dir		= false;

		/**
		 * Constructor
		 * @param string $filename
		 * @param string $directory
		 */
		function __construct($directory,$filename,$is_file,$is_dir) {
			$this->_filename 		= $filename;
			$this->_directory 		= $directory;
			//$this->_filesize		= FILE_UTIL::get_filesize($this->file());
			//$this->_modified_time	= FILE_UTIL::get_file_modified_time($this->get_file());
			$this->_is_file 		= $is_file;
			$this->_is_dir 			= $is_dir;
		}

		/**
		 * Returns the file
		 * @return string
		 */
		function file() 			{ return $this->_directory.$this->_filename; }

		/**
		 * Returns the filename
		 * @return string
		 */
		function filename() 		{ return $this->_filename; }

		/**
		 * Returns the directory
		 * @return string
		 */
		function directory() 		{ return $this->_directory; }

		/**
		 * Returns the object
		 * @return object
		 */
		function get_object() {

			$object = new Object();
			$object->icon_url = $this->_icon_url;
			$object->filesize = $this->_filesize;
			$object->is_directory = $this->is_directory();
			$object->is_file = $this->is_file();
			$object->filename = $this->_filename;
			$object->modified_time = $this->_modified_time;
			$object->extension = $this->get_file_extension();

			return $object;
		}

		/**
		 * Checks whether is directory
		 * @return boolean
		 */
		function is_dir() 		{ return $this->_is_dir; }

		/**
		 * Checks whether is file
		 * @return boolean
		 */
		function is_file() 			{ return $this->_is_file; }

		/**
		 * Returns the filesize
		 * @return string
		 */
		function filesize()			{ return $this->_filesize; }

		/**
		 * Returns the modified time
		 * @return string
		 */
		function modified()				{ return $this->_modified; }

		/**
		 * Returns the file extension
		 * @return string
		 */
		function extension()		{ return FILE_UTIL::get_file_extension($this->filename()); }

		//function __toString() {
			//return $this->filename();
		//}
	}
