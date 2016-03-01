<?    /**
    * Class BASE_CMODEL_FILE_BROWSER
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_FILE_BROWSER extends CMODEL {

		protected $_storage_cmodel		= null;

        /**
         * The root directory
         * @var string $root_directory
         */
		protected $_root_directory 	= "";

		/**
		 * valid extensions
		 * @var array $valid_extensions
		 */
		protected $_valid_extensions	= array("jpg","gif","html","htm","css","png");

		/**
		 * Constructor
		 * @param string $root_directory
		 */
		function __construct($root_directory) {
			$this->_storage_cmodel = SYSTEM_MANAGER::storage();
			$this->set_root_directory($root_directory);
		}

        /**
         * Sets the valid extensions
         * @param array $extensions
         * @return array
         */
		function set_valid_extensions($extensions) {
			$this->_valid_extensions = $extensions; return $this;
		}

        /**
         * Checks whether is valid files
         * @param string $file
         * @return boolean
         */
		function is_valid_file($file) 		{ return in_array(FILE_UTIL::get_file_extension($file),$this->_valid_extensions); }

		/**
		 * Checks whether extension is valid
		 * @param string $ext
		 * @return boolean
		 */
		function is_valid_extension($ext) 	{ return in_array($ext,$this->_valid_extensions); }

		/**
		 * Sets root directory
		 * @param string $root_directory
		 */
		function set_root_directory($root_directory) {
			$this->_root_directory = $root_directory;
		}

        /**
         * Returns root directory
         * @return string
         */
		function get_root_directory() {
			return $this->_root_directory();
		}

        /**
         * Return safe path
         * @param string $path
         * @return mixed
         */
		function get_safe_path($path) {
			return preg_replace("/(\.\.\/)+/","",$path);
		}

        /**
         * Validate file
         * @param string $file
         * @throws Exception
         */
		function validate_file($file) {

			if(!$this->_root_directory)
				throw new Exception("Invalid root directory");

			if(!$this->is_valid_file($file))
				throw new Exception("Invalid file type");
		}

		/**
		* Validate directory
		* @param string $dir
		* @throws Exception
		*/
		function validate_directory($dir) {

			if(!$this->_root_directory)
				throw new Exception("Invalid root directory");
		}

        /**
         * Returns the listing
         * @param boolean $only_directories
         * @return array
         */
		function get_listing($path,$only_directories=false) {

			$this->validate_directory("");

			$listing = array();

			$directory = $this->get_full_directory($path);

			$filenames = $this->_storage_cmodel->listing($directory,"*");

			foreach($filenames as $filename) {

				if(is_a($filename,"BCMODEL_STORAGE_OBJECT")) {

					$is_file	= $filename->is_file();
					$is_dir 	= $filename->is_dir();
					$filename 	= $filename->filename();
				} else {
					$is_file	= is_file($directory.$filename);
					$is_dir 	= is_dir($directory.$filename);
					$filename 	= (string)$filename;
				}

				$index = $is_file ? "zzorderzz".strtolower($filename) : strtolower($filename);

				$directory = $this->_storage_cmodel->sanitize($directory);

				$listing[$index] = new BASE_CMODEL_FILE_BROWSER_ITEM($filename,$directory,$is_file,$is_dir);
			}

			ksort($listing,SORT_STRING);

			return array_values($listing);
		}

        /**
         * Stream zip
         * @param string $file_path
         * @return boolean
         */
		function stream_zip($file_path) {

			if($file_path!="/")
				$this->validate_file($file_path);

			$file_path 	= $this->get_safe_path($file_path);
			$file 		= $this->_root_directory.$file_path;

			$temp_dir = MODEL_PATH::get_temporary_directory();
			FILE_UTIL::mkdir($temp_dir);

			$zip_file = $temp_dir.MISC_UTIL::get_guid(5).".zip";

			$zip_util = new ZIP_UTIL($zip_file);
			$zip_util->add_directory_contents($file);
			$zip_util->close();

			HTTP_UTIL::stream_file($zip_file);

			FILE_UTIL::rmdir($temp_dir);

			return true;
		}

        /**
         * File unzip
         * @param string $file_path
         * @param string $dir_path
         * @param array $valid_extensions
         * @return boolean
         */
		function file_unzip($file_path,$dir_path,$valid_extensions) {

			$is_valid = FILE_UTIL::is_extension($file_path,"zip");

			if(!$is_valid)
				throw new Exception("Invalid zip file");

			$has_success = false;

			$full_directory	= $this->_root_directory.$dir_path;

			FILE_UTIL::mkdir($full_directory);

			$unzip_util = new UNZIP_UTIL($file_path);
			$has_success = $unzip_util->extract($full_directory,array(),$valid_extensions);

			return $has_success;
		}

        /**
         * Stream a file
         * @param string $file_path
         * @return boolean
         * @throws Exception
         */
		function stream_file($file_path) {

			$this->validate_file($file_path);

			$file_path 	= $this->get_safe_path($file_path);
			$file 		= $this->_root_directory.$file_path;

			$has_success 	= HTTP_UTIL::stream_file($file);

			if(!$has_success)
				throw new Exception("There was a problem streaming the file");

			return $has_success;
		}

        /**
         * Delete a file
         * @param string $file_path
         * @return boolean
         * @throws Exception
         */
		function delete_file($file_path) {

			$this->validate_file($file_path);

			$file_path 	= $this->get_safe_path($file_path);
			$file 		= $this->_root_directory.$file_path;


			$has_success = $this->_storage_cmodel->delete($file);

			if(!$has_success)
				throw new Exception("There was a problem trying to delete the file");

			return $has_success;
		}

        /**
         * Returns file contents
         * @param string $file_path
         * @return string
         * @throws Exception
         */
		function get_file_contents($file_path) {

			$this->validate_file($file_path);

			$file_path 	= $this->get_safe_path($file_path);
			$file 		= $this->_root_directory.$file_path;

			return $this->_storage_cmodel->get($file);
		}

        /**
         * Put file contents
         * @param string $file_path
         * @param string $content
         * @return boolean
         * @throws Exception
         */
		function put_file_contents($file_path,$content) {

			$this->validate_file($file_path);

			$file_path 	= $this->get_safe_path($file_path);
			$file 		= $this->_root_directory.$file_path;

			$this->_storage_cmodel->put($file,$content);
		}

        /**
         * Create directory
         * @param string $dir_path
         * @return boolean
         * @throws Exception
         */
		function create_directory($dir_path) {

			$this->validate_directory($dir_path);

			$dir_path 	= $this->get_safe_path($dir_path);
			$file 		= $this->_root_directory.$dir_path;

			$has_success = $this->_storage_cmodel->mkdir($file);

			if(!$has_success)
				throw new Exception("Failed to create directory");

			return $has_success;
		}

        /**
         * Rename directory
         * @param string $old_path
         * @param string $new_path
         * @return boolean
         * @throws Exception
         */
		function rename_directory($old_path,$new_path) {

			$this->validate_directory($new_path);

			$old_dir 	= $this->_root_directory.$this->get_safe_path($old_path);
			$new_dir 	= $this->_root_directory.$this->get_safe_path($new_path);

			$has_success = $this->_storage_cmodel->rename($old_dir,$new_dir);

			if(!$has_success)
				throw new Exception("Failed to rename the directory");

			return $has_success;
		}

        /**
         * Rename file
         * @param string $old_path
         * @param string $new_path
         * @return boolean
         * @throws Exception
         */
		function rename_file($old_path,$new_path) {

			$this->validate_file($new_path);

			$old_file	= $this->_root_directory.$this->get_safe_path($old_path);
			$new_file 	= $this->_root_directory.$this->get_safe_path($new_path);

			$has_success = $this->_storage_cmodel->rename($old_file,$new_file);

			if(!$has_success)
				throw new Exception("Failed to rename the directory");

			return $has_success;
		}

        /**
         * Checks whether is a file
         * @param string $path
         * @return boolean
         */
		function is_file($path) {

			$this->validate_file($path);

			$file = $this->_root_directory.$this->get_safe_path($path);


			return is_file($file);
		}

        /**
         * Checks whether is a path
         * @param string $path
         * @return boolean
         */
		function is_dir($path) {

			$this->validate_directory($path);

			$file = $this->_root_directory.$this->get_safe_path($path);


			return is_dir($file);
		}

        /**
         * Create file
         * @param string $file_path
         * @return boolean
         */
		function create_file($file_path) {

			$this->validate_file($file_path);

			return $this->put_file_contents($file_path,"");
		}

        /**
         * Delete directory
         * @param string $dir_path
         * @return boolean
         * @throws Exception
         */
		function delete_directory($dir_path) {

			$this->validate_directory($dir_path);

			$dir_path 	= $this->get_safe_path($dir_path);
			$file 		= $this->_root_directory.$dir_path;

			$has_success = $this->_storage_cmodel->rmdir($file);

			if(!$has_success)
				throw new Exception("Failed to delete directory");

			return $has_success;
		}

        /**
         * Copy
         * @param string $source_file
         * @param string $file_path
         * @return boolean
         * @throws Exception
         */
		function copy($source_file,$file_path) {

			$this->validate_file($file_path);

			$file_path 	= $this->get_safe_path($file_path);
			$file 		= $this->_root_directory.$file_path;
			$directory	= dirname($this->_root_directory.$file_path);

			$this->_storage_cmodel->mkdir($directory);

			$has_success = $this->_storage_cmodel->copy($source_file,$file);

			if(!$has_success)
				throw new Exception("Failed to copy the file");

			return $has_success;
		}

        /**
         * Returns the directories
         * @return array
         */
		function get_directories() {
			return $this->get_listing(true);
		}

        /**
         * Returns the files
         * @return array
         */
		function get_files() {
			$listing = $this->get_listing();

			$files = array();

			foreach($listing as $item) {
				if($item->is_file())
					$files[] = $item;
			}

			return $files;
		}

        /**
         * Returns the full directory
         * @return string
         */
		function get_full_directory($dir) {
			return $this->_storage_cmodel->sanitize($this->_root_directory.$dir);
		}

        /**
         * Returns the directory steps
         * @return array
         */
		function get_directory_steps($dir) {
			if($dir)
				return preg_split("/(\/|\\\\)+/",$dir);
			return array();
		}



	}

    /**
    * Class BASE_CMODEL_FILE_BROWSER_ITEM
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_FILE_BROWSER_ITEM {

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

		/**
		 * The icon url
		 * @var string $icon_url
		 */
		protected $_icon_url		= "/lib/images/icons/files/%s";

		/**
		 * The filesize
		 * @var integer $filesize
		 */
		protected $_filesize		= 0;

		/**
		 * modified time
		 * @var integer $modified_time
		 */
		protected $_modified_time	= 0;

		protected $_is_file		= false;
		protected $_is_dir		= false;

		/**
		 * Constructor
		 * @param string $filename
		 * @param string $directory
		 */
		function __construct($filename,$directory,$is_file,$is_dir) {
			$this->_filename 		= $filename;
			$this->_directory 		= FILE_UTIL::get_sanitized_directory($directory);
			$this->_filesize		= FILE_UTIL::get_filesize($this->get_file());
			$this->_modified_time	= FILE_UTIL::get_file_modified_time($this->get_file());
			$this->_is_file 		= $is_file;
			$this->_is_dir 			= $is_dir;
		}

		/**
		 * Returns the file
		 * @return string
		 */
		function get_file() 			{ return $this->_directory.$this->_filename; }

		/**
		 * Returns the filename
		 * @return string
		 */
		function get_filename() 		{ return $this->_filename; }

		/**
		 * Returns the directory
		 * @return string
		 */
		function get_directory() 		{ return $this->_directory; }

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
		function is_directory() 		{ return $this->_is_dir; }

		/**
		 * Checks whether is file
		 * @return boolean
		 */
		function is_file() 			{ return $this->_is_file; }

		/**
		 * Returns the filesize
		 * @return string
		 */
		function get_filesize()			{ return $this->_filesize; }

		/**
		 * Returns the modified time
		 * @return string
		 */
		function get_modified_time()		{ return $this->_modified_time; }

		/**
		 * Returns the file extension
		 * @return string
		 */
		function get_file_extension()		{ return FILE_UTIL::get_file_extension($this->get_filename()); }

		/**
		 * Returns the icon directory url
		 * @return string
		 */
		function get_icon_dir_url()		{ return $this->get_icon_file_url(); }

		/**
		 * Returns the icon file url
		 * @return string
		 */
		function get_icon_file_url() {

			$filename = $this->get_file_extension().".png";

			if($this->is_directory())
				$filename = "folder-close.png";

			$icon_file = DIR_FRAMEWORK."frontend".sprintf($this->_icon_url,$filename);

			if(!is_file($icon_file))
				return sprintf($this->_icon_url,"file.png");

			return sprintf($this->_icon_url,$filename);
		}

        /**
         * Returns the puclic url
         * @return string
         */
		function get_public_url() {

			$public_dir = str_replace("\\","/",MODEL_PATH::get_public_directory());
			$item_dir = str_replace("\\","/",$this->_directory);

			if(stripos($item_dir,$public_dir)===0) {

				$path = str_replace($public_dir,"",$item_dir).$this->get_filename();

				return "/public/".$path;
			}

			return "";
		}
	}