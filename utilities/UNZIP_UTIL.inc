<?
    /**
    * Class UNZIP_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */
 
	class UNZIP_UTIL extends ZipArchive {

        /**
         * Zip file
         * @var string $zip_file 
         */
		protected $_zip_file = null;
		
        /**
         * Construct
         * 
         * @param string $zip_file the zip file
         */
		public function __construct($zip_file) {
			$this->_zip_file = $zip_file;
			return $this->open($zip_file);			
		}

		public static function create($zip_file) { return new UNZIP_UTIL($zip_file); }
		
        /**
         * Deconstruct
         * 
         */
		public function __deconstruct() {
			$this->close();
		}

		public function close() {
			@parent::close();
			return $this;
		}

        /**
         * Extracts a file
         * @param string $destination
         * @param array $valid_entries
         * @param array $valid_extensions
         * @return boolean 
         */
		public function extract($destination,$valid_entries=array(),$valid_extensions=array()) {
			
			$destination = FILE_UTIL::sanitize($destination);

			if(!is_file($this->_zip_file))
				throw new Exception("Zip file does not exist");				
			
			$valid_entries = is_array($valid_entries) ? $valid_entries : array($valid_entries);
			
			$valid_extensions = is_array($valid_extensions) ? $valid_extensions : array($valid_extensions);
			
			$entries = $this->get_entries();
			
			foreach($entries as $entry) {
			
				$file = $destination.$entry->get_name();
				
				if($entry->is_directory()) {
					FILE_UTIL::mkdir($file);
					continue;
				}
				
				if($valid_extensions && !in_array($entry->get_extension(),$valid_extensions)) 
					continue;
					
				if($valid_entries && !in_array($entry->get_name(),$valid_entries)) 
					continue;
				
				FILE_UTIL::mkdir($destination.$entry->get_directory_path());
				
				copy("zip://".$this->_zip_file."#".$entry->get_name(), $file);
			}
			
			return $this;
		}
		
        /**
         * Extract command
         * @param string $destination
         * @param array $output
         * @return boolean 
         */
		public function extract_command($destination,$output=array()) {
			
			$cmd = "unzip -o ".$this->_zip_file." -d ".$destination;
			
			return SERVER_UTIL::exec($cmd,$output,true);	
		}

        /**
         * Checks whether command unzip exists
         */
		public function has_command_unzip() {
			SERVER_UTIL::exec("unzip",$output,1,$return_val);
			return $return_val===0;
		}		
		
        /**
         * Returns the error message
         * @return string 
         */
		public function get_error_message() { return $this->getStatusString(); }
		
        /**
         * Returns the entries
         * @param string $base_directory
         * @param string $preg_name
         * @return array 
         */
		public function get_entries($base_directory="",$preg_name="") {
			$entries = array();
			
			if($preg_name) {
				
				for($i=0; $i<$this->numFiles; $i++) {
					$name = get_value($this->statIndex($i),"name");
					
					if(preg_match($preg_name,$name)) {
						$entry = UNZIP_ENTRY::create($this,$i,$base_directory);	
						
						if($entry->is_valid())
							$entries[] = $entry;
					}
				}

			} else {

				for($i=0; $i<$this->numFiles; $i++) {
					
					$entry = UNZIP_ENTRY::create($this,$i,$base_directory);

					if($entry->is_valid())
						$entries[] = $entry;	
				}
			}

			return $entries;
		}
		
        /**
         * Checks whether there is an entry from a name
         * @param string $name
         * @param boolean $recursive
         * @return boolean 
         */
		public function has_entry($name,$recursive=false) {
			$flags = 0;
			
			if($recursive)
				$flags |= ZIPARCHIVE::FL_NODIR;
			
			return $this->getFromName($name,$flags)!==false;
		}

        public function get($name) {        
                return $this->getFromName($name);
        }

        public function get_zip_filename() { return basename($this->_zip_file); }
	}
	
    /**
    * Class UNZIP_ENTRY
    *
    * @package Framework
    * @subpackage Utilities
    */        
	
	class UNZIP_ENTRY {
		
		/**
		* Name
		* @var string $name 
		*/
		public $_name 		= "";

		/**
		* Index
		* @var integer $index 
		*/
		public $_index 		= 0;

		/**
		* Crc
		* @var integer $crc 
		*/
		public $_crc 		= 0;

		/**
		* Size
		* @var integer $size 
		*/
		public $_size		= 0;

		/**
		* Mtime
		* @var integer $mtime 
		*/
		public $_mtime 		= 0;

		/**
		* Comp size
		* @var integer $comp_size 
		*/
		public $_comp_size 	= 0;

		/**
		* Comp method
		* @var integer $comp_method 
		*/
		public $_comp_method	= 0;

		/**
		* Base directory
		* @var string $base_directory 
		*/
		public $_base_directory	= "";

        /**
        * Zip file
        * @var UNZIP_UTIL $zip
        */

        private $_zip = null;

		/**
		* Create
		* @param array $data
		* @param string $base_directory
		* @return object UNZIP_ENTRY 
		*/
		static function create($zip,$index,$base_directory="") {
			$unzip_entry = new UNZIP_ENTRY();
            $data = $zip->statIndex($index);
			$unzip_entry->populate($data);
			$unzip_entry->set_base_directory($base_directory);
            $unzip_entry->set_zip($zip);
			return $unzip_entry;
		}

        /**
         * Populate
         * @param array $data
         * @return boolean 
         */
		function populate($data) {
		
			if(!is_array($data))
				return false;
				
			$this->_name		= get_value($data,"name");
			$this->_index		= get_value($data,"index");
			$this->_crc			= get_value($data,"crc");
			$this->_size		= get_value($data,"size");
			$this->_mtime		= get_value($data,"mtime");
			$this->_comp_size	= get_value($data,"comp_size");
			$this->_comp_method	= get_value($data,"comp_method");
		}
                
        /**
         * Checks whether the name is valid
         * @return boolean 
         */
		function is_valid() 		{ return !preg_match("/^__MACOSX\//",$this->get_name()); }

		/**
		* Returns the file
		* @return string 
		*/
		function get_file() 		{ return $this->_base_directory.$this->get_name(); }

		/**
		* Returns the name
		* @return string 
		*/
		function get_name() 		{ return $this->_name; }

		/**
		* Returns the index
		* @return integer 
		*/
		function get_index()		{ return $this->_index; }

		/**
		* Returns the crc
		* @return integer 
		*/
		function get_crc() 		{ return $this->_crc; }

		/**
		* Returns the size
		* @return integer 
		*/
		function get_size() 		{ return $this->_size; }
        function filesize()         { return get_size(); }

		/**
		* Returns the mtime
		* @return integer 
		*/
		function get_mtime() 		{ return $this->_mtime; }

		/**
		* Returns the comp size
		* @return inetegr 
		*/
		function get_comp_size() 	{ return $this->_comp_size; }

		/**
		* Returns the comp method
		* @return integer 
		*/
		function get_comp_method() 	{ return $this->_comp_method; }

		/**
		* Checks whether the name is directory
		* @return boolean 
		*/
		function is_directory() 	{ return preg_match("/[\\/\\/]$/",$this->get_name()); }

		/**
		* Returns the formatted size
		* @return string 
		*/
		function get_formatted_size() 	{ return FORMAT_UTIL::get_formatted_filesize($this->_size); }

		/**
		* Checks whether a name has directory
		* @return boolean 
		*/
		function has_directory() 	{ return preg_match("/[\\/\\/]/",$this->get_name()); }

		/**
        * Returns the filename
        * @return string 
        */
        function filename()    { return FILE_UTIL::get_filename($this->get_name()); }

        /**
		* Returns the extension
		* @return string 
		*/
		function get_extension() 	{ return FILE_UTIL::get_file_extension($this->get_name()); }
        function extension()    { return $this->get_extension(); }

		/**
		* Checks whether an extention is extension
		* @param string $ext
		* @return boolean 
		*/
		function is_extension($ext) 	{ return $this->get_extension()==$ext; }

		/**
		* Checks whether an extention is an extension list
		* @param array $exts
		* @return boolean 
		*/
		function has_extension($exts) 	{ return in_array($this->get_extension(),$exts); }

		/**
		* Sets the base directory
		* @param string $base_directory
		*/
		function set_base_directory($base_directory) { 
			$this->_base_directory = $base_directory; 
		}

        /**
         * Returns the directory path
         * @return boolean 
         */
		function get_directory_path() {
			
			if($this->is_directory())
				return $this->get_name();
			
			preg_match("/(.*?)[^\/]*$/",$this->get_name(),$matches);
			
			return get_value($matches,1);
		}		

        /**
         * Checks whether a file exists
         * @param string $dir
         * @return boolean 
         */
		function file_exists($dir) {
			$dir = preg_replace("/[\\/\\/]$/","",$dir);
			return file_exists($dir.$this->get_name());
		}

        /**
        * Set zip file
        * @param UNZIP_UTIL $zip
        */

        public function set_zip($zip) {
            $this->_zip = $zip;
        }

        /**
        * Get entry content
        * @return string
        */

        public function file(){
            return $this->_zip->getFromIndex($this->get_index());
        }
	}