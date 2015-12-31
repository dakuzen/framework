<?

	class BASE_CMODEL_IMAGE {

		const BLANK_IMAGE = "data:image/gif;base64,R0lGODlhAQABAAAAACH5BAEKAAEALAAAAAABAAEAAAICTAEAOw==";

		protected $_debug 					= false;
		protected $_directory 				= "";
		protected $_extension				= "";
		protected $_id 						= "";
		protected $_path 					= "";
		protected $_public_root				= "";
		protected $_public_path				= "";
		protected $_sizes 					= "";
		protected $_timestamp				= null;
		protected $_storage_cmodel			= null;
		protected $_use_filename_timestamp 	= false;

		public function __construct($directory,$path,$name,$extension,$sizes,$timestamp=null) {

			$this->_directory 		= $directory;
			$this->_extension 		= $extension;
			$this->_name 			= $name;
			$this->_timestamp 		= $timestamp;
			$this->_path 			= $path;
			$this->_storage_cmodel 	= SYSTEM_MANAGER::storage();
			$this->_public_root		= SYSTEM_MANAGER::get_public_root();
			$this->_public_path		= SYSTEM_MANAGER::get_public_path();

			foreach($sizes as $name=>$size) {
				$name = $size==="actual" ? "actual" : $name;
				$this->_sizes[$name] = $size;
			}
		}

		public function use_filename_timestamp($enable=true) {
			$this->_use_filename_timestamp = $enable;
			return $this;
		}

		public static function create($directory,$path,$name,$extension,$sizes,$timestamp=null) {
			return new CMODEL_IMAGE($directory,$path,$name,$extension,$sizes,$timestamp);
		}

		public function purge($pattern) {


				if($this->_storage_cmodel->is_local()) {

					foreach($this->_sizes as $name=>$size) {

						$dir = $this->_directory.$name."/";

						foreach($this->_storage_cmodel->files($dir,$pattern) as $file)
							$this->_storage_cmodel->delete($dir.$file);
					}

				} elseif($this->_storage_cmodel->is_s3()) {

					$files = [];
					foreach($this->_sizes as $name=>$size)
						$files[] = $this->file($size);

					$this->_storage_cmodel->delete($files);
				}

			return $this;
		}

		public function process_url($url) {

			$temp_cmodel = CMODEL_TEMP::create("jpg");

			$http_transport_util = new HTTP_TRANSPORT_UTIL();
			$http_transport_util->save($temp_cmodel->file(),$url);

			$this->process($temp_cmodel->file());

			$temp_cmodel->cleanup();
		}

		public function reprocess($original,$sizes=array()) {

			$sizes = $sizes ? (array)$sizes : array_keys($this->get_sizes());

			$sizes = array_filter($sizes, function($test) use ($original) { return $original!=$test; });

			$file = $this->file($original);

			if(!$file)
				throw new Exception("Failed to find the original file");

			$sizes = ARRAY_UTIL::names($this->get_sizes(),$sizes);

			return $this->process($file,$sizes);
		}

		const OPTION_ROTATE			= "rotate";
		const OPTION_AUTO_ORIENT	= "auto-orient";

		public function process($file,$sizes=array(),$options=array(),$page_index=null) {

			$sizes = $sizes ? $sizes : $this->_sizes;

			$convert_image_util = CONVERT_IMAGE_UTIL::create()->debug($this->_debug);

			// Convert transparent backgrounds to white for jpgs
			if(FILE_UTIL::get_file_extension($file)=="png" && $this->is_extension("jpg"))
				$options["flatten"] = "";

			foreach($sizes as $name=>$size) {

				$dir 			= $this->get_size_directory($name);
				$dest 			= $dir.$this->get_filename();

				$this->_storage_cmodel->mkdir($dir);

				if($name==="actual") {
					if($this->_storage_cmodel->is_s3()) {
						$this->_storage_cmodel->put($file,$dest);
					} else {
						$this->_storage_cmodel->copy($file,$dest);
					}

				} else {

					if($this->_storage_cmodel->is_s3()) {
						$temp_cmodel	= CMODEL_TEMP::create();
						$dest 			= $temp_cmodel->dir().$this->get_filename();
					}

					if(is_array($size))
						$convert_image_util->convert($file,$dest,$size,$page_index);

					else {

						$dims = explode("x",$size);

						$width 	= value($dims,0);
						$height = value($dims,1);

						if(preg_match("/^s(\d+)/",$size,$matches))
							$convert_image_util->squarify($file,$dest,value($matches,1),CONVERT_IMAGE_UTIL::GRAVITY_CENTER,$options,$page_index);
						else
							$convert_image_util->resize($file,$dest,$width,$height,true,true,false,$options,$page_index);
					}

					if($this->_storage_cmodel->is_s3()) {
						$this->_storage_cmodel->put($dest,$dir.$this->get_filename());
						$temp_cmodel->cleanup();
					}
				}
			}

			return $this;
		}

		public function url($name,$full=false,$default="",$protocol=true) {

			if($default && !$this->exists($name))
				return $default;

			$protocol = $protocol ? (SERVER_UTIL::is_server_https() ? "https:" : "http:") : "";

			//Failsafe incase the full path was passed /public/zzz and zzz/
			$path = str_replace(SYSTEM_MANAGER::get_public_path(),"",$this->_path);

			$timestamp = $this->_timestamp && $this->_storage_cmodel->is_local() ? $this->_timestamp."/" : "";

			//Ensure the URL has no //
			$path = preg_replace("/\/{2,}/","/","/".$this->_public_root."/".$this->_public_path."/".$timestamp.$path."/");

			return ($full || !$this->_storage_cmodel->is_local() ? $protocol."//".SYSTEM_MANAGER::get_public_domain() : "").$path.$name."/".$this->get_filename();
		}

		public function urls() {

			$arry = [];

			foreach($this->_sizes as $name=>$size) {
				$arry[$name] = $this->exists($name) ? $this->url($name) : "";
			}

			return $arry;
		}


		public function size($name)					{ return IMAGE_UTIL::get_image_size($this->file($name)); }
		public function timestamp()					{ return $this->_timestamp; }
		public function debug()						{ $this->_debug = true; return $this; }
		public function exists($name) 				{ return $this->_timestamp!==null; }
		public function file($name) 				{ return $this->get_size_directory($name).$this->get_filename(); }
		public function filename() 					{ return $this->get_filename(); }


		public function get_sizes() 				{ return $this->_sizes; }
		public function get_filename() 				{
			if($this->_use_filename_timestamp)
				return $this->_name."_".$this->timestamp().".".$this->_extension;
			else
				return $this->_name.".".$this->_extension;
		}
		public function is_extension($extension) 	{ return strcasecmp($this->_extension,$extension)==0; }
		public function set_extension($extension) 	{ $this->_extension = $extension; return $this; }
		public function get_size_directory($name)	{ return $this->_directory.$name."/"; }

		public function set_id($name) 				{ return $this->_name; }

		public function stream($size) {
			HTTP_UTIL::stream_file($this->file($size));
		}

		public function blank($attribs=array()) {
			return HTML_UTIL::img(self::BLANK_IMAGE,$attribs);
		}

		public function img($name,$attribs=array(),$default="") {

			$url = $this->url($name);
			if(!$this->exists($name))
				$url = $default;

			return $url ? HTML_UTIL::get_image($url,$attribs) : "";
		}
	}