<?php
        /**
        * Class TAR_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */
	
	class TAR_UTIL {
                
                /**
                 * The file
                 * @var string $file 
                 */
		protected $_file 	= null;
                
                /**
                 * Debug mode
                 * @var boolean $debug
                 */
		protected $_debug	= false;

		/**
		 * Creates a TAR_UTIL
		 *
		 * @param string $file The file that will be used to create a tar
		 * 
		 * @return bool
		 */
		public function __construct($file=null) {
			$this->_file = $file;
		}

		public static function create($file=null) {
			return new TAR_UTIL($file);
		}

		/**
		 * Adds files to a tar archive
		 *
		 * @param string $file
		 * @param array $options
		 * @return bool
		 */
		public function add($file,$options=array()) {

			$option_compress	= FILE_UTIL::get_file_extension($file)=="gz";
			$option_excludes	= $exclude=get_value($options,"exclude");

			$tar_dir 		= dirname($this->_file);
			$tar_name 		= basename($this->_file);

			if(!is_dir($tar_dir))
				throw new Exception("The destination directory ".$tar_dir." does not exist");				

			$file_dir 		= dirname($file);
			$file_name 		= basename($file);

			$flags = array();
			$flags[] = is_file($this->_file) ? "r" : "c";

			$modifiers = array();
			$modifiers[] = "-C ".$file_dir;

			if($option_excludes) {

				$excludes = is_array($option_excludes) ? $option_excludes : array($option_excludes);

				foreach($excludes as $exclude)
					$modifiers[] = "--exclude=".$exclude."";
			}

			if(is_file($this->_file)) {

				$flags[] = "f";

				$cmd = "-".implode("",$flags)." %s ".implode(" ",$modifiers)." ".$file_name;
			} else {
				
				if($option_compress)
					$flags[] = "z";
				$flags[] = "f";

				$cmd = "-".implode("",$flags)." %s ".implode(" ",$modifiers)." ".$file_name;
			}

			if(SERVER_UTIL::is_os_windows())
				$cmd = "cd ".$tar_dir." && ".$this->get_tar_exe()." ".sprintf($cmd,$tar_name);
			else
				$cmd = $this->get_tar_exe()." ".sprintf($cmd,$this->_file);

			if($this->_debug)
				p($cmd);

			$return_var = 0;
			$output = array();

			SERVER_UTIL::exec($cmd,$output,true,$return_var);

			if($return_var!=0)
				throw new Exception(implode(" ",$output));				
		}
                
                /**
                 * Returns the tar exe
                 * @return string 
                 */
		public function get_tar_exe() { return SERVER_UTIL::is_os_windows() ? MODEL_PATH::get_framework_directory()."assets/bsdtar/bsdtar" : "tar"; }

                /**
                 * Extracts
                 * @param string $destination
                 * @param string $filter
                 * @throws Exception 
                 */
		public function extract($destination,$filter="") {

			$cmd = $this->get_tar_exe()." -xf ".$this->_file." -C ".$destination." ".$filter;

			if($this->_debug)
				p($cmd);

			$return_var = 0;
			$output = array();

			SERVER_UTIL::exec($cmd,$output,true,$return_var);

			if($return_var!=0)
				throw new Exception(implode(" ",$output));				
		}

                /**
                 * Add directory contents
                 * @param string $directory
                 * @return boolean 
                 */
		public function add_directory_contents($directory) {
			$directory = preg_replace("/[\\/]$/","/",$directory);
			$directory .= ".";
			return $this->add($directory);
		}

    /**
     * Returns entries
     * @param string $filter
     * @return array
     * @throws Exception 
     */
		public function get_entries($filter="") {
				
			$cmd = $this->get_tar_exe()." --verbose -tf ".$this->_file." ".$filter;

			if($this->_debug)
				p($cmd);

			$return_var = 0;
			$output = array();

			SERVER_UTIL::exec($cmd,$output,true,$return_var);

			if($return_var!=0)
				throw new Exception(implode(" ",$output));	

			$entries = array();

			if($output=implode("\n",$output)) {

				$output = explode("\n",$output);
				rsort($output);

				$dirs = array();
				foreach($output as $line) {
					$is_dir = preg_match("/^[d]/i",$line);

					if(preg_match("/(\d+) ((?:(?:[a-z]{3} \d{2})|(?:\d{4}-\d{2}-\d{2})) \d{2}:\d{2}) (.*)$/i",$line,$matches)) {

						$entry = UNZIP_ENTRY::create(value($matches,3),value($matches,1),$is_dir);

						if(!array_key_exists($entry->get_dir(),$dirs))
							$dirs[$entry->get_dir()] = 0;

						foreach($dirs as $name=>&$size)  {
							if(preg_match("/^".preg_quote($name,"/")."/",$entry->get_dir()))
								$size += $entry->get_size();
						}

						$entries[] = $entry;
					}
				}

				foreach($entries as &$entry) {

					if($entry->is_dir())
						foreach($dirs as $name=>$size)
							if($entry->get_dir()==$name)
								$entry->set_size($size);
				}
			}

			return $entries;
		}
	}


	class UNZIP_ENTRY extends OBJECT {
		
		/**
		* Name
		* @var string $name 
		*/
		public $_name 		= "";
		/**
		* Size
		* @var integer $size 
		*/
		public $_size		= 0;

		/**
		* Size
		* @var integer $size 
		*/

		public $_is_dir		= false;		

		/**
		* Create
		* @param array $data
		* @param string $base_directory
		* @return object UNZIP_ENTRY 
		*/
		static function create($name,$size,$is_dir) {
			$unzip_entry = new UNZIP_ENTRY();
			$unzip_entry->populate($name,$size,$is_dir);
			return $unzip_entry;
		}

        /**
         * Populate
         * @param array $data
         * @return boolean 
         */
		function populate($name,$size,$is_dir) {	
			$this->_name		= $name;
			$this->_size		= $size;
			$this->_is_dir		= $is_dir;
		}
     	/**
		* Returns the name
		* @return string 
		*/
		function get_name() 		{ return $this->_name; }

		function get_dir() 		{ return dirname($this->_name); }

		
		function get_pretty_filesize() 		{ return CMODEL_FORMAT::create($this->_size)->filesize(); }
		/**
		* Returns the size
		* @return integer 
		*/
		function get_size() 		{ return $this->_size; }

		function set_size($size) 		{ $this->_size = $size; }

		function is_dir() 		{ return $this->_is_dir; }		
	}	