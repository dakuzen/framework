<?
    /**
    * Class UPLOAD_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    *
	*	Single file upload
	*
	*	$upload_util = UPLOAD_UTIL::create("file")->upload();
	*	
	*	$cmodel = CMODEL_DOCUMENT::create()
	*					->set_filename($upload_util->filename())
	*					->set_extension($upload_util->extension())
	*					->commit_file($upload_util->file());
	*
	*
    *   Multiple files uploads
    *  
	*	foreach(UPLOAD_UTIL::uploads("files") as $upload_util) {
	*		$upload_util->upload();
	*		$cmodel->commit($upload_util->file());
	*	}
	*/

	class UPLOAD_UTIL {

		const ERROR_STORE_FILENAME_INVALID	= 0;
		const ERROR_NO_FILES_ARRAY			= 1;
		const ERROR_FILE_EXISTS				= 2;
		const ERROR_DEST_DIR_NOT_CREATED	= 3;
		const ERROR_NO_FILENAME_SUPPLIED	= 4;
		const ERROR_NO_STORE_PATH			= 5;
		const ERROR_FILE_EXT_NOT_VALID		= 6;
		const ERROR_UPLOADING_FILE			= 7;
		const ERROR_MIME_INVALID			= 8;
		const ERROR_MAX_FILE_SIZE			= 20;
		const ERROR_MAX_FORM_SIZE			= 21;
		const ERROR_PARTIAL					= 22;
		const ERROR_NO_FILE					= 23;
		const ERROR_NO_TMP_DIR				= 24;
		const ERROR_CANT_WRITE				= 25;
		const ERROR_EXTENSION				= 26;
		const ERROR_INTERNAL_CONFIG_ERROR	= 27;
                
		/**
		 * The store filename of this class.
		 * @var string $store_filename 
		 */
		private $_store_filename 		= null;

		/**
		 * The uploaded filename.
		 * @var string $filename 
		 */
		private $_filename 		= null;	

		/**
		 * The uploaded filesize.
		 * @var string $filesize 
		 */
		private $_filesize 		= null;				

		/**
		 * The store path of this class.
		 * @var string $store_path 
		 */
		private $_store_path			= null;

		/**
		 * The valid extensions of this class.
		 * @var array $valid_extensions 
		 */
		private $_valid_extensions 		= array();

		/**
		 * The valid mime types of this class.
		 * @var array $valid_mime_types 
		 */
		private $_valid_mime_types 		= array();

		/**
		 * The error message of this class.
		 * @var string $error_message 
		 */
		private $_error_message 		= "";

		/**
		 * The error code of this class.
		 * @var string $error_code 
		 */
		private $_error_code 			= null;

		/**
		 * The files name of this class.
		 * @var string $files_name 
		 */
		private $_files_name 			= "";

		/**
		 * The file of this class.
		 * @var string $file 
		 */
		private $_file 				= null;

		/**
		 * The override option.
		 * @var boolean $override 
		 */
		private $_override			= false;

		/**
		 * The check mime type option.
		 * @var boolean $check_mime_type 
		 */
		private $_check_mime_type		= true;

		/**
		 * The check extension option.
		 * @var boolean $check_extension 
		 */
		private $_check_extension		= true;

		/**
		 * The upload error list of this class.
		 * @var array $UPLOAD_ERROR_LIST 
		 */
		static private $UPLOAD_ERROR_LIST	= array();

		private $_exception			= true;


		/**
		 * Constructor.
		 * 
		 * @param mix $files_name file name indexes
		 */
		public function __construct($files_name,$store_path="",$valid_extensions=array(),$store_filename="") {

			self::$UPLOAD_ERROR_LIST = array(	self::ERROR_MAX_FILE_SIZE=>"The uploaded file exceeds the max file size of %s",
												self::ERROR_MAX_FORM_SIZE=>"The uploaded file exceeds the max size specified in the HTML form",
												self::ERROR_PARTIAL=>"The uploaded file was only partially uploaded",
												self::ERROR_NO_FILE=>"No file was uploaded",
												self::ERROR_NO_TMP_DIR=>"Missing a temporary folder",
												self::ERROR_CANT_WRITE=>"Failed to write file",
												self::ERROR_EXTENSION=>"File upload stopped by extension",
												self::ERROR_INTERNAL_CONFIG_ERROR=>"The file failed to upload",
												self::ERROR_STORE_FILENAME_INVALID=>"The store filename is invalid",
												self::ERROR_NO_FILES_ARRAY=>"No form array passed",
												self::ERROR_FILE_EXISTS=>"The file already exists",
												self::ERROR_DEST_DIR_NOT_CREATED=>"The destination directory could not be created",
												self::ERROR_NO_FILENAME_SUPPLIED=>"No filename supplied",
												self::ERROR_NO_STORE_PATH=>"No store path supplied",
												self::ERROR_FILE_EXT_NOT_VALID=>"The file extension is not valid",
												self::ERROR_UPLOADING_FILE=>"There was an error while trying to upload the file",
												self::ERROR_MIME_INVALID=>"The MIME type is invalid");	
			
			if(is_array($files_name) && array_key_exists("name",$files_name))
				$this->_file = $files_name;
			else
				$this->_file = value($_FILES,$files_name,null);

			if($this->_file) {
				if(!$store_filename)
					$store_filename = basename(value($this->_file,"tmp_name",""));

				if(!$store_path)
					$store_path = dirname(value($this->_file,"tmp_name")).DIRECTORY_SEPARATOR;
			}

			$this
				->set_store_path($store_path)
				->set_valid_extensions($valid_extensions)
				->set_store_filename($store_filename)
				->set_filename(value($this->_file,"name",""))
				->set_filesize(value($this->_file,"size",0));
		}

		public static function create($file,$store_path="",$valid_extensions=array(),$store_filename="") { return new UPLOAD_UTIL($file,$store_path,$valid_extensions,$store_filename); }
		
		public static function has_files($name=null) {

			if($name)
				return !!value($_FILES,$name);
			
			return !empty($_FILES);
		}

		public static function uploads($name,$dir=null,$extensions=array()) {
			
			$upload_utils = array();			
			foreach(UPLOAD_UTIL::files($name) as $file)
				$upload_utils[] = UPLOAD_UTIL::create($file,$dir,$extensions);

			return $upload_utils;
		}

		public static function files($name="") {

			if(!$_FILES)
				throw new Exception("Missing file");			
			
			/*
		    [file] => Array
		        (
		            [name] => IMG_0142.PNG
		            [type] => image/png
		            [tmp_name] => C:\Users\Ray\AppData\Local\Temp\php52EF.tmp
		            [error] => 0
		            [size] => 133714
		        )
		    */

			$files = array();

			if(count($_FILES)==1 && is_string(value(array_values($_FILES),array(0,"name")))) {
				foreach($_FILES as $key=>$file)
					$files = array($key=>array($file));

			} else {
								    
			    $files = array_map(function($group) {
								        $result = array();
								        foreach($group as $property=>$arr)
								            foreach($arr as $item=>$value)
								                $result[$item][$property] = $value;
								   		return $result;
								    },$_FILES);
			}
			
		   	if($name)
		   		$files = value($files,$name,array());

		   	else {
		   		$list = array();
		   		foreach($files as $file)
		   			$list = array_merge($list,$file);

		   		$files = $list;
		   	}

		   	return $files;
   		}

		/*

		    [file] => Array
		        (
		            [name] => IMG_0142.PNG
		            [type] => image/png
		            [tmp_name] => C:\Users\Ray\AppData\Local\Temp\php52EF.tmp
		            [error] => 0
		            [size] => 133714
		        )


		    [file] => Array
		        (
		            [name] => Array
		                (
		                    [0] => IMG_0780.jpg
		                )

		            [type] => Array
		                (
		                    [0] => image/jpeg
		                )

		            [tmp_name] => Array
		                (
		                    [0] => C:\Users\Ray\AppData\Local\Temp\phpB00D.tmp
		                )

		            [error] => Array
		                (
		                    [0] => 0
		                )

		            [size] => Array
		                (
		                    [0] => 1945825
		                )

		        )
		*/

		/**
		 * Disable extension checking
		 * 
		 * @return null
		 */		
		function disable_check_extension() 	{ return $this->_check_extension = false; }
		
		/**
		 * Enables override
		 * 
		 * @return boolean
		 */
		function enable_override() 		{ return $this->_override = true; }
		
		/**
		 * Disables override
		 * 
		 * @return boolean
		 */
		function disable_override() 		{ return $this->_override = false; }

		/**
		 * Checks for error
		 * 
		 * @return int
		 */
		function has_error() 			{ return $this->_error_code!=null; }
		
		/**
		 * Returns error code
		 * 
		 * @return int
		 */
		function get_error_code()		{ return $this->_error_code; }
		
		/**
		 * Returns error message
		 * 
		 * @return string
		 */
		function get_error_message() 		{ return get_value(self::$UPLOAD_ERROR_LIST,$this->_error_code); }
		
		/**
		 * Sets the store path 
                 * @param $path the path
		 */
		function set_store_path($path) 		{ $this->_store_path = $path; return $this; }
		
		/**
		 * Sets the store file name
                 * @param $filename the filename
		 */
		function set_store_filename($filename) 	{ $this->_store_filename = $filename; return $this; }

		function set_filename($filename) 	{ $this->_filename = $filename; return $this; }

		function set_filesize($filesize) 	{ $this->_filesize = $filesize; return $this; }
		
		/**
		 * Gets the store file name
		 *
		 * @return string
		 */
		function get_store_filename() 		{ return $this->_store_filename; }
		
		/**
		 * Gets the store file name
		 *
		 * @return string
		 */
		function get_store_file() 		{ return $this->get_store_path().$this->get_store_filename(); }
		function file() 				{ return $this->get_store_file(); }
		function exists() 				{ return !!$this->_file; }
		function extension() 			{ return $this->get_file_extension(); }
		function filename() 			{ return $this->_filename; }
		function filesize() 			{ return $this->_filesize; }
		
		/**
		 * Gets the file extension
		 *
		 * @return string
		 */
		function get_file_extension() 		{ return strtolower(FILE_UTIL::get_file_extension($this->filename())); }

		function is_extension($ext) 		{ return $this->extension()===$ext; }
		
		/**
		 * Gets path
		 *
		 * @return string
		 */
		function get_store_path() 		{ return $this->_store_path; }
		
		/**
		 * Adds mime type
		 *
		 * @param string $mime_type the mime type
		 */
		function add_mime_type($mime_type)	{ $this->_valid_mime_types[$mime_type] = true; }
		
		/**
		 * Removes mime type
		 *
		 * @param string $mime_type string 
		 */
		 
		function remove_mime_type($mime_type)	{ unset($this->_valid_mime_types[$mime_type]); }
		
		/**
		 * Disables mime check
		 *
		 */
		 
		function ignore_mime_check() 		{ $this->_check_mime_type = false; }
		
		/**
		 * Checks for valid file extension
		 *
		 * @return boolean
		 */
		 
		function is_valid_file_extension() {

			if(!$this->_check_extension || !$this->_valid_extensions)
				return true;
			
			$valid_extension = $this->_valid_extensions;
			
			if(in_array("jpg",$valid_extension))
				$valid_extension[] = "jpeg";
				
			if(in_array("tif",$valid_extension))
				$valid_extension[] = "tiff";
			
			return in_array($this->extension(),$valid_extension); 
		}
		
		/**
		 * Returns a temp file
		 *
		 * @return string
		 */		
		
		function get_temp_file() {
			return get_value($this->_file, 'tmp_name');
		}

        /**
         * Returns file name
         * 
         * @return string 
         */
		function get_file_name(){
			return get_value($this->_file,"name");
		}

		/**
		 * Sets valid extensions
		 *
		 * @param array $extensions array of extensions 
		 */
		 
		function set_valid_extensions($extensions)	{
			//$mime_ext_list = FILE_MIME_UTIL::get_mime_extension_list();
		
			foreach($extensions as $extension) {
				$extension = strtolower(str_replace(".","",$extension));
				
				$this->_valid_extensions[] = $extension;
				
				//if($mime_exts=get_value($mime_ext_list,$extension,array())) {
				//	foreach($mime_exts as $mime)
						//$this->add_mime_type($extension);
				//}
				
			}
			return $this;
		}

		/**
		 * Checks file upload exist
		 *
		 * @return boolean 
		 */
		 
		function is_file_upload_exist() {
			return (is_array($this->_file) && ((get_value($this->_file,"name")) || count($this->_file)==0));
		}

		/**
		 *
		 * Todo : Ray
		 * 
		 */

		function error($code,$message="") {

			$this->_error_code = $code;

			if($this->_exception) {
				$message = $message ? $message : get_value(self::$UPLOAD_ERROR_LIST,$code);
				throw new Exception($message,$this->_error_code);
			}

			return $this;
		}
		
		function is_type_image() { return preg_match("/image/",value($this->_file,"type")); }
		function is_type_video() { return preg_match("/video/",value($this->_file,"type")); }
		 
		function upload() {

			// Check post_max_size (http://us3.php.net/manual/en/features.file-upload.php#73762)
		    $post_max_size = (int)ini_get('post_max_size');			
			$unit = strtoupper(substr(ini_get('post_max_size'), -1));

		    $multiplier = ($unit == 'M' ? 1048576 : ($unit == 'K' ? 1024 : ($unit == 'G' ? 1073741824 : 1)));

		    if((int)value($_SERVER,"CONTENT_LENGTH")>=$multiplier*$post_max_size)
		        throw new Exception("Upload file size limit exceeded",500);
		        
			if(!$this->_file)
				return $this->error(self::ERROR_INTERNAL_CONFIG_ERROR);			

			if($error=get_value($this->_file,"error")) {
				switch($error) {
					case UPLOAD_ERR_INI_SIZE:
						
						$updated = sprintf(self::$UPLOAD_ERROR_LIST[self::ERROR_MAX_FILE_SIZE],ini_get("upload_max_filesize"));

						self::$UPLOAD_ERROR_LIST[self::ERROR_MAX_FILE_SIZE] = $updated;

						return $this->error(self::ERROR_MAX_FILE_SIZE);

					case UPLOAD_ERR_FORM_SIZE:
						return $this->error(self::ERROR_MAX_FORM_SIZE);

					case UPLOAD_ERR_PARTIAL:
						return $this->error(self::ERROR_PARTIAL);

					case UPLOAD_ERR_NO_FILE:
						return $this->error(self::ERROR_NO_FILE);

					case UPLOAD_ERR_NO_TMP_DIR:
						return $this->error(self::ERROR_NO_TMP_DIR);

					case UPLOAD_ERR_CANT_WRITE:
						return $this->error(self::ERROR_CANT_WRITE);

					case UPLOAD_ERR_EXTENSION:
						return $this->error(self::ERROR_EXTENSION);

				}
			}

			//if(!$this->is_post_file_exist()) {
			//	$this->error(self::ERROR_NO_FILES_ARRAY);
			//	return false;
			//}

			if($this->_store_path==null) 
				return $this->error(self::ERROR_NO_STORE_PATH);

			if(trim($this->_store_filename)=="") 
				return $this->error(self::ERROR_NO_FILENAME_SUPPLIED);

			if(!$this->is_valid_file_extension()) 
				return $this->error(self::ERROR_FILE_EXT_NOT_VALID);

			if(!VALIDATE_UTIL::is_valid_filename($this->_store_filename))
				return $this->error(self::ERROR_STORE_FILENAME_INVALID);

			/*
			if($this->_check_mime_type) {

				$file_mime_util_obj = new FILE_MIME_UTIL($this->_file['tmp_name']);

				$has_valid_type = false;

				foreach(array_keys($this->_valid_mime_types) as $type)
					if($file_mime_util_obj->is_type($type))
						$has_valid_type = true;


				if(!$has_valid_type) {
					$this->error(self::ERROR_MIME_INVALID);
					return false;
				}
			}
			*/
			
			$this->_store_filename = FILE_UTIL::get_safe_file($this->_store_filename);
			
			$destination_file = FILE_UTIL::get_sanitized_path($this->_store_path."/".$this->_store_filename);

			if($destination_file!=$this->_file['tmp_name']) {
				
				if(!$this->_override && is_file($destination_file))
					return $this->error(self::ERROR_FILE_EXISTS);

				if(!is_dir($this->_store_path)) 
					return $this->error(self::ERROR_DEST_DIR_NOT_CREATED);

				$move_result = move_uploaded_file($this->_file['tmp_name'], $destination_file);

				if (!$move_result) 
					return $this->error(self::ERROR_UPLOADING_FILE);

				FILE_UTIL::chmod($destination_file,"0770");
			}

			return $this;
		}
	}