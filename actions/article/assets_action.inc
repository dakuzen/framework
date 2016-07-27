<?
	class BASE_ACTION_ARTICLE_ASSETS extends BASE_ACTION_ARTICLE_BASE {

		protected $_file_browser_cmodel = null;
		protected $_target_uri			= null;
		protected $_valid_extensions	= array("jpg","gif","html","htm","css","png","js","woff","ttf","otf","svg","eot","less","swf","pdf","xml","flv","woff2");

		function __construct($root_directory="",$target_uri="") {
			parent::__construct();
			$this->_file_browser_cmodel = CMODEL_FILE_BROWSER::create($root_directory)
											->set_valid_extensions($this->_valid_extensions);

			$this->_target_uri = $target_uri;
		}

		function process() {

			$this->_file_browser_cmodel->set_root_directory($this->get_dir());

			$dir 		= XSS_UTIL::decode($this->request("dir"));
			$file 		= $this->request("file");
			$action		= $this->request("action");

			try {

				if($action=="list") {

					$listing = $this->_file_browser_cmodel->get_listing($dir);

					$files = array();
					foreach($listing as $item) {
						$file = $item->get_object();
						$file->url = $this->_target_uri."action:stream/?file=".$dir.XSS_UTIL::encode($item->get_filename());
						$files[] = $file;
					}

					$this->data("files",$files)->success();

				} elseif($action=="delete_file") {


					$this->_file_browser_cmodel->delete_file($file);
					$this->success();

				} elseif($action=="delete_dir") {

					if($file!="/") {

						$this->_file_browser_cmodel->delete_directory($file);
						$this->success();
					}

				} elseif($action=="stream") {

					$this->_file_browser_cmodel->stream_file($file);
					die;

				} elseif($action=="get") {

					$contents = $this->_file_browser_cmodel->get_file_contents($file);
					$this
						->data("contents",$contents)
						->success();

				} elseif($action=="save") {

					$contents = XSS_UTIL::decode($this->request("contents"));

					$this->_file_browser_cmodel->put_file_contents($file,$contents);

					$this->success();


				} elseif($action=="create_dir") {

					$this->_file_browser_cmodel->create_directory($this->request("dir"));

					$this->success();

				} elseif($action=="rename_dir") {

					$this->_file_browser_cmodel->rename_directory($this->request("old_dir"),$this->request("new_dir"));

					$this->success();

				} elseif($action=="rename_file") {

					$this->_file_browser_cmodel->rename_file($this->request("old_file"),$this->request("new_file"));

					$this->success();

				} elseif($action=="create_file") {

					$file = $this->request("file");

					$this->_file_browser_cmodel->create_file($file);

					$this->success();

				} elseif($action=="upload") {

					$dir = $this->request("dir");

					$this->data("dir",$dir);

					$temp_cmodel = CMODEL_TEMP::create();

					try {

						foreach(UPLOAD_UTIL::uploads("file", $temp_cmodel->dir(),$this->_valid_extensions) as $upload_util) {

							$upload_util->upload();

							$this->_file_browser_cmodel->copy($upload_util->file(),$dir.$upload_util->get_store_filename());
						}

						$this->success(true);

					} catch(Exception $e) {
						$temp_cmodel->cleanup();
					}

					$temp_cmodel->cleanup();

				} elseif($action=="download") {

					$file = $this->request("file");

					$this->_file_browser_cmodel->stream_zip($file);

				} elseif($action=="download_url") {

					$file = $this->request("file");

					if($this->_file_browser_cmodel->is_dir($file)) {

						$url = $this->_target_uri."action:download/?file=".$file;

						$this->data("download_url",$url);
						$this->success();
					} else
						throw new Exception("Invalid file for download");
				}
			} catch(Exception $e) {
				$this->error($e);
			}
		}

		public function get_file_browser() 		{ return $this->_file_browser_cmodel; }
		public function set_target_uri($target_uri)	{ $this->_target_uri = $target_uri; }

	}