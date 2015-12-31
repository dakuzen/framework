<?php
    /**
    * Class BASE_CMODEL_FILE
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_FILE {

        /**
         * The file
         * @var string $file 
         */
        public $_file 		= "";

        /**
         * The index
         * @var integer $index 
         */
        public $_index 		= 0;

        /**
         * The size
         * @var integer $size 
         */
        public $_size		= 0;

        /**
         * The base directory
         * @var type 
         */
        public $_base_directory	= "";

        /**
         * Constructor
         * @param string $file
         * @param integer $size 
         */
		function __construct($file="",$size=0) {
			$this->_file		= $file;
			$this->_size		= $size;
		}

        /**
         * Populates
         * @param array $data
         * @return boolean 
         */
		function populate($data) {
		
			if(!is_array($data))
				return false;
				
			$this->_file		= get_value($data,"file");
			$this->_index		= get_value($data,"index");
			$this->_size		= get_value($data,"size");
		}
	
        /**
         * Checks whether is valid
         * @return boolean 
         */
        function is_valid() 		{ return !preg_match("/^__MACOSX\//",$this->get_file()); }

        /**
         * Returns the file
         * @return string 
         */
        //function get_file() 		{ return $this->_base_directory.$this->get_file(); }

        /**
         * Returns the file
         * @return sting  
         */
        function get_file() 		{ return $this->_file; }

        /**
         * Returns the filename
         * @return string
         */
        function get_filename()		{ return basename($this->_file); }

        /**
         * Returns the index
         * @return integer
         */
        function get_index()		{ return $this->_index; }

        /**
         * Return the size
         * @return integer
         */
        function get_size() 		{ return $this->_size; }

        /**
         * Check whether is directory
         * @return boolean 
         */
        function is_directory() 	{ return preg_match("/[\\/\\/]$/",$this->get_file()); }

        /**
         * Return the formatted size
         * @return integer 
         */
        function get_formatted_size() 	{ return FORMAT_UTIL::get_formatted_filesize($this->_size); }

        /**
         * Check whether is has directory
         * @return boolean
         */
        function has_directory() 	{ return preg_match("/[\\/\\/]/",$this->get_file()); }

        /**
         * Returns the extension
         * @return string 
         */
        function get_extension() 	{ return FILE_UTIL::get_file_extension($this->get_file()); }

        /**
         * Check whether is extension
         * @param string $ext
         * @return boolean 
         */
        function is_extension($ext) 	{ return $this->get_extension()==$ext; }

        /**
         * Check whether it has extension
         * @param string $exts
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
         * Return the directory path
         * @return string 
         */
        function get_directory_path() {
			
			if($this->is_directory())
				return $this->get_file();
			
			preg_match("/(.*?)[^\/]*$/",$this->get_file(),$matches);
			
			return get_value($matches,1);
		}		
		
        /**
         * Exists 
         * @param string $dir
         * @return boolean 
         */
		function exists($dir) {
			$dir = preg_replace("/[\\/\\/]$/","",$dir);
			return file_exists($dir.$this->get_file());
		}
	}