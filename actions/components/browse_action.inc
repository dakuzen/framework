<?
	class BASE_ACTION_COMPONENTS_BROWSE extends ACTION {
		
		protected $_file_browser_cmodel = null;
		protected $_target_uri			= null;
		protected $_valid_extensions	= array("jpg","gif","html","htm","css","png","js","woff","ttf","otf","svg","eot","less","swf","pdf","xml","flv","woff2");
		
		function __construct($root_directory="",$target_uri="") {
			$this->_file_browser_cmodel = new BASE_CMODEL_FILE_BROWSER($root_directory);
			$this->_file_browser_cmodel->set_valid_extensions($this->_valid_extensions);
			$this->_target_uri = $target_uri;
			$this->debug_format_text();
		}
		
		
		function process() {
			
			$dir 		= XSS_UTIL::decode($this->request("dir"));
			$file 		= $this->request("file");
			$action		= $this->request("action");
			
			$ajax_response_cmodel = new BASE_CMODEL_AJAX_RESPONSE();
			
			if($action=="list") {
			
				try {
					$listing = $this->_file_browser_cmodel->get_listing($dir);

					$files = array();

					foreach($listing as $item) {
						$file = $item->get_object();
						$file->url = $this->_target_uri."action:stream/?file=".$dir.XSS_UTIL::encode($item->get_filename());
						$files[] = $file;
					}
					
					$ajax_response_cmodel->add_data("files",$files);
					$ajax_response_cmodel->success(true);
					
				} catch(Exception $e) {
					$ajax_response_cmodel->add_error($e->getMessage()); 
				}
				
			} elseif($action=="delete_file") {
				
				try {
					$this->_file_browser_cmodel->delete_file($file);
					$ajax_response_cmodel->success(true);
					
				} catch(Exception $e) {
					$ajax_response_cmodel->add_error($e->getMessage()); 
				}			
			
			} elseif($action=="delete_dir") {
				
				if($file!="/") {

					try {
						
						$this->_file_browser_cmodel->delete_directory($file);
						$ajax_response_cmodel->success(true);
						
					} catch(Exception $e) {
						$ajax_response_cmodel->add_error($e->getMessage()); 
					}
				}		
			
			} elseif($action=="stream") {
				
				try {
					$this->_file_browser_cmodel->stream_file($file);
					die;
				} catch(Exception $e) {
					$ajax_response_cmodel->add_error($e->getMessage()); 
				}			
			
			} elseif($action=="get") {
				
				try {
					$contents = $this->_file_browser_cmodel->get_file_contents($file);
					$ajax_response_cmodel->add_data("contents",$contents);
					
					$ajax_response_cmodel->success(true);
				} catch(Exception $e) {
					$ajax_response_cmodel->add_error($e->getMessage()); 
				}			
			
			} elseif($action=="save") {
				
				try {
					$contents = XSS_UTIL::decode($this->request("contents"));
			
					$this->_file_browser_cmodel->put_file_contents($file,$contents);
					
					$ajax_response_cmodel->success(true);
				} catch(Exception $e) {
					$ajax_response_cmodel->add_error($e->getMessage()); 
				}			
			
			} elseif($action=="create_dir") {
				
				try {
					$dir = $this->request("dir");
			
					$this->_file_browser_cmodel->create_directory($dir);
					
					$ajax_response_cmodel->success(true);
				} catch(Exception $e) {
					$ajax_response_cmodel->add_error($e->getMessage()); 
				}			
			
			} elseif($action=="rename_dir") {
				
				try {
					$old_dir = $this->request("old_dir");
					$new_dir = $this->request("new_dir");
			
					$this->_file_browser_cmodel->rename_directory($old_dir,$new_dir);
					
					$ajax_response_cmodel->success(true);
					
				} catch(Exception $e) {
					$ajax_response_cmodel->add_error($e->getMessage()); 
				}			
			
			} elseif($action=="rename_file") {
				
				try {
					$old_file = $this->request("old_file");
					$new_file = $this->request("new_file");
			
					$this->_file_browser_cmodel->rename_file($old_file,$new_file);
					
					$ajax_response_cmodel->success(true);
					
				} catch(Exception $e) {
					$ajax_response_cmodel->add_error($e->getMessage()); 
				}			
			
			} elseif($action=="create_file") {
				
				try {
					$file = $this->request("file");
			
					$this->_file_browser_cmodel->create_file($file);
					
					$ajax_response_cmodel->success(true);
				} catch(Exception $e) {
					$ajax_response_cmodel->add_error($e->getMessage()); 
				}			
			
			} elseif($action=="upload") {
				
				try {
					$dir = $this->request("dir");
					
					$ajax_response_cmodel->add_data("dir",$dir);	

					$temp_dir = MODEL_PATH::get_temporary_directory();
					FILE_UTIL::mkdir($temp_dir);
					
					$upload_util = UPLOAD_UTIL::create("file",$temp_dir,array_merge($this->_valid_extensions,array("zip")))->upload();

					if($upload_util->get_file_extension()=="zip") {
						$has_success = $this->_file_browser_cmodel->file_unzip($upload_util->get_store_file(),$dir,$this->_valid_extensions);
									
					} else {
					
						$file = $dir.$upload_util->get_store_filename();

						$has_success = $this->_file_browser_cmodel->copy($upload_util->get_store_file(),$file);
					}

					$ajax_response_cmodel->success($has_success);	
				
					FILE_UTIL::rmdir($temp_dir);

				} catch(Exception $e) {
					$ajax_response_cmodel->add_error($e->getMessage()); 
				}			
			
			} elseif($action=="download") {
				 
				try {
					
					$file = $this->request("file");
					
					$this->_file_browser_cmodel->stream_zip($file);
					
				} catch(Exception $e) {
					$ajax_response_cmodel->add_error($e->getMessage()); 
				}
				
				
			} elseif($action=="download_url") {
				
				try {
					$file = $this->request("file");

					if($this->_file_browser_cmodel->is_dir($file)) {
				
						$url = $this->_target_uri."action:download/?file=".$file;

						$ajax_response_cmodel->add_data("download_url",$url);
						$ajax_response_cmodel->success(true);
					} else
						throw new Exception("Invalid file for download");						
					
				} catch(Exception $e) {
					$ajax_response_cmodel->add_error($e->getMessage()); 
				}
				
				
			}

			$ajax_response_cmodel->render();			
		}
		
		public function get_file_browser() 		{ return $this->_file_browser_cmodel; }
		public function set_target_uri($target_uri)	{ $this->_target_uri = $target_uri; }
		
	}