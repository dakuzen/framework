<?
	class BCMODEL_STORAGE_LOCAL extends BCMODEL_STORAGE {

		protected $_debug = true;

		public static function create() {
			return new CMODEL_STORAGE_LOCAL();
		}

		public function is_local() { return true; }

		public function mkdir($dir,$options=array("override"=>false)) {
			FILE_UTIL::mkdir($dir,value($options,"override"),$this->_debug); return $this;
		}

		public function chown($path,$uid,$gid,$options=array("recursive"=>true))  {
			FILE_UTIL::chown($path,$uid,$gid,value($options,"recursive"),$this->_debug); return $this;
		}

		public function group_id($path) {
			FILE_UTIL::set_group_id($path,$this->_debug); return $this;
		}

		public function chmod($path,$bit,$options=array("recursive"=>true))  {
			FILE_UTIL::chmod($path,$bit,value($options,"recursive"),$this->_debug); return $this;
		}

		public function rmdir($dir) {
			FILE_UTIL::rmdir($dir,$this->_debug); return $this;
		}

		public function delete($file) {
			FILE_UTIL::delete($file,$this->_debug); return $this;
		}

		public function move($from,$to,$options=array("override"=>false)) {
			FILE_UTIL::move($from,$to,value($options,"override"),false,$this->_debug); return $this;
		}

		public function rename($from,$to,$options=array("override"=>false)) {
			FILE_UTIL::move($from,$to,value($options,"override"),$this->_debug); return $this;
		}

		public function copy($file_src,$file_dest) {
			FILE_UTIL::copy($file_src,$file_dest,$this->_debug); return $this;
		}

		public function listing($directory,$pattern="*",$options=array("expand_braces"=>true,"only_directories"=>false,"only_files"=>false,"absolute"=>false)) {

			$directory = $this->sanitize($directory);

			$flag_bit = 0;
			if(value($options,"only_files"))
				$flag_bit += GLOB_BRACE;

			$listing = FILE_UTIL::get_directory_listing($directory,$pattern,false,value($options,"only_directories"),value($options,"only_files"),$flag_bit,$this->_debug);

			if(value($options,"absolute"))
				foreach($listing as &$item)
					$item = $directory.$item;

			return $listing;
		}

		public function files($directory,$pattern="*",$options=array("absolute"=>true)) {
			$options["only_files"] = true;
			return self::listing($directory,$pattern,$options);
		}

		public function directories($directory,$pattern="*",$options=array("absolute"=>false)) {
			$options["only_directories"] = true;
			return self::listing($directory,$pattern,$options);
		}

		public function count($directory,$pattern="*") {
			return FILE_UTIL::get_file_count($directory,$pattern,$this->_debug);
		}

		public function put($file,$contents,$options=array()) {
			return FILE_UTIL::put($file,$contents,0,$this->_debug);
		}

		public function get($file) {
			return FILE_UTIL::get($file,$this->_debug);
		}

		public function modified($file) {
			return FILE_UTIL::get_file_modified_time($file,$this->_debug);
		}

		public function size($file) {
			return FILE_UTIL::get_filesize($file,$this->_debug);
		}

		public function sanitize($dir) {
			return FILE_UTIL::sanitize($dir);
		}
	}