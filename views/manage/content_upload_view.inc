<?
	class BASE_VIEW_MANAGE_CONTENT_UPLOAD extends VIEW {
		
		protected $_view 		= "upload_file";
		protected $_cancel_url		= "";
		protected $_site_content_cmodel	= null;
		protected $_entries		= array();
		protected $_root_directory	= null;
		protected $_root_url		= null;
		protected $_names		= array();
		protected $_valid_extensions	= array("css","jpg","swf","png","gif","pdf","tif","tiff","doc","docx","xls","xlsx","js");
		
		function __construct() {
			$this->set_route("manage","content_upload");
			
			$this->_site_content_cmodel 	= new BASE_CMODEL_SITE_CONTENT();
			$this->_root_directory 		= MODEL_PATH::get_content_directory();
			$this->_root_url 		= "/public/content/";			
		}
		
		function init() {
			$registry = REGISTRY::get_instance();
				

			if($site_content_id=$registry->get_numeric_post_parm("scid")) {

				$site_content_hmodel = new BASE_HMODEL_SITE_CONTENT();
				$site_content_hmodel->set_site_content_id($site_content_id);

				$site_content_cmodel = get_value($site_content_hmodel->get_site_contents(),0);

				if($site_content_cmodel)
					$this->_site_content_cmodel = $site_content_cmodel;
			}
			
			
			
			if($registry->get_post_parm("upload_file")) {
				
				$upload_util = new UPLOAD_UTIL("uploadfile");

				if($upload_util->is_file_upload_exist()) {
					
					$temp_dir = MODEL_PATH::get_temporary_directory();
					FILE_UTIL::mkdir($temp_dir);				

					$upload_util->set_store_path($temp_dir);
					$upload_util->set_valid_extensions(array("zip"));

					$this->_has_success = $upload_util->upload();

					if($this->_has_success) {

						USER_SESSION::get_instance()->set_setting("backend_site_content_upload_directory",$temp_dir);
						USER_SESSION::get_instance()->set_setting("backend_site_content_upload_filename",$upload_util->get_store_filename());
						
						$this->_view = "upload_confirm";
						
						$unzip_util = new UNZIP_UTIL($upload_util->get_store_file());
						$this->_entries = $unzip_util->get_entries();
						
						foreach($this->_entries as $entry)
							$this->_names[] = $entry->get_name();
						
					} else {
						FILE_UTIL::rmdir($temp_dir);
						APPLICATION::add_error_message($upload_util->get_error_message());
					}

				} else
					APPLICATION::add_error_message("Please supply a file to upload");				
			
			} elseif($registry->get_post_parm("upload_confirm")) {
				
				$zip_filename 	= USER_SESSION::get_instance()->get_setting("backend_site_content_upload_filename");
				$zip_directory 	= USER_SESSION::get_instance()->get_setting("backend_site_content_upload_directory");
				
				$zip_file 	= $zip_directory.$zip_filename;
				
				$this->_names 	= is_array($registry->get_post_parm("names")) ? $registry->get_post_parm("names") : array();
				$content_name	= $registry->get_post_parm("content_name");
				
				if(is_file($zip_file)) {
				
					if($this->_names) {
					
						$unzip_util = new UNZIP_UTIL($zip_file);
						$has_success = $unzip_util->extract($this->_root_directory,$this->_names);
						
						$html_content_filename = $this->get_html_content_filename($unzip_util->get_entries());
						
						if($content_name===$html_content_filename) {
							
							$temp_dir = MODEL_PATH::get_temporary_directory();
							FILE_UTIL::mkdir($temp_dir);					
							
							$has_success = $unzip_util->extract($temp_dir,$html_content_filename);
							
							if($has_success) {
								$content = FILE_UTIL::get_file_contents($temp_dir.$html_content_filename);
								
								$this->_site_content_cmodel->set_content($content);
								$this->_site_content_cmodel->save();
							}
							
							FILE_UTIL::rmdir($temp_dir);
						}

						if($has_success) {
							APPLICATION::add_notify_message("Successfully uploaded");
							
							$links = array();
							
							foreach($this->_names as $name) {
								$url = $this->_root_url.ltrim($name,"/");
								$links[] = HTML_UTIL::get_link("javascript:show_box('".$url."','100%','100%')",$url);
							}
							
							APPLICATION::add_notify_message(HTML_UTIL::get_list($links));
							
							$this->_view = "upload_finish";
						} else
							APPLICATION::add_error_message($unzip_util->get_error_message());
							
						$unzip_util->close();
					}
				}
				
				FILE_UTIL::rmdir($zip_directory);
				
				USER_SESSION::get_instance()->remove_setting("backend_site_content_upload_filename");
				USER_SESSION::get_instance()->remove_setting("backend_site_content_upload_directory");
			
			} elseif($registry->get_post_parm("upload_finish") || $registry->get_post_parm("cancel")) {
				
				if($this->_cancel_url)
					HTTP_UTIL::redirect(sprintf($this->_cancel_url,$this->_site_content_cmodel->get_site_content_id()));
			}
			
			$this->set_var("view",$this->_view);
			$this->set_var("site_content",$this->_site_content_cmodel);
			$this->set_var("cancel_url",$this->_cancel_url);
			$this->set_var("root_directory",$this->_root_directory);
			$this->set_var("root_url",$this->_root_url);
			$this->set_var("entries",$this->_entries);
			$this->set_var("names",$this->_names);
			$this->set_var("valid_extensions",$this->_valid_extensions);
		}
		
		function load($site_content_id) {

			$site_content_cmodel = BASE_HMODEL_SITE_CONTENT::get_site_content($site_content_id);

			if($site_content_cmodel) {				
				$this->_site_content_cmodel = $site_content_cmodel;
			}
		}		
		
		function set_cancel_url($cancel_url) 			{ $this->_cancel_url = $cancel_url; }
		function set_root_directory($root_directory) 		{ $this->_root_directory = $root_directory; }
		function set_root_url($root_url) 			{ $this->_root_url = $root_url; }
		
		function get_html_content_filename($entries) {
		
			$valid_filenames = array("index.html","index.php","index.htm");
			
			foreach($valid_filenames as $valid_filename)
				foreach($entries as $entry)
					if(strtolower($entry->get_name())==$valid_filename)
						return $entry->get_name();
					
			return "";
		}
	}