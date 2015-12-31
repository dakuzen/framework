<?
	class BASE_CMODEL_TEMP extends MODEL_COMPLEX {

		protected $_dir 		= "";
		protected $_guid	 	= "";
		protected $_extension 	= "";

		public function __construct($extension="",$guid="",$dir="") {
			$this->_extension 	= $extension;
			$this->_guid 		= $guid;
			$this->_dir 		= $dir ? $dir : MODEL_PATH::get_temporary_directory();
			
			if(!$guid)
				$this->generate();
		}
		
		public static function instance($extension="",$guid="",$dir="") { return new CMODEL_TEMP($extension,$guid,$dir=""); }
		
		public function generate() {			
			$this->_guid = MISC_UTIL::get_guid(); return $this;
		}

		public static function create($extension="",$guid="",$dir="") {
			$temp_cmodel = new CMODEL_TEMP($extension,$guid,$dir);
			return $temp_cmodel->init();
		}

		public function init() {
			FILE_UTIL::mkdir($this->_dir); return $this;
		}

		public function cleanup() {	
			FILE_UTIL::rmdir($this->_dir); return $this;
		}

		public function file($extension="",$guid="") {
			$extension = $extension ? $extension : $this->_extension;
			return $this->_dir.$this->filename($extension,$guid);
		}

		public function filename($extension="",$guid="") {
			return ($guid ? $guid : $this->_guid).".".($extension ? $extension : $this->_extension);
		}		

		public function extract($dir) {
			$unzip_util = new UNZIP_UTIL($this->file());
			return $unzip_util->extract($dir);
		}

		public function dir() { return $this->_dir; }

		public function set_dir($dir) { $this->_dir = $dir; return $this; }

		public function save($data) { FILE_UTIL::put_file_contents($this->file(),$data); return $this; }

		public function stream($filename) { HTTP_UTIL::stream_file($this->file(),$filename); return $this; }
	}