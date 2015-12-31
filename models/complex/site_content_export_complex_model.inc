<?php
        /**
        * Class BASE_CMODEL_SITE_CONTENT_EXPORT
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_SITE_CONTENT_EXPORT extends MODEL_COMPLEX {
		
		const TEMPLATE_FORMAT_HTML	= "H";
		const TEMPLATE_FORMAT_PHP	= "P";
		const BODY_PLACEHOLDER		= "{BODY}";
		
                /**
                 * public dirs
                 * @var array $public_dirs 
                 */
		protected $_public_dirs 	= array();
                
                /**
                 * The template format
                 * @var string $template_format
                 */
		protected $_template_format	= self::TEMPLATE_FORMAT_HTML;
		
                /**
                 * Sets public dirs
                 * @param array $public_dirs 
                 */
		public function set_public_dirs($public_dirs) 	{ $this->_public_dirs = $public_dirs; }
                
                /**
                 * template format php
                 */
		public function template_format_php() 		{ $this->_template_format = self::TEMPLATE_FORMAT_PHP; }
				
                /**
                 * Returns zip
                 * @param string $zip_file
                 * @return boolean 
                 */
		public function get_zip($zip_file) {
			
			BASE_CMODEL_WEB_ASSET_MANAGER::disable();
				
			$body = APPLICATION::get_base_view_instance("components","html");
			$body->set_html(self::BODY_PLACEHOLDER);
			
			$view = APPLICATION::get_view_instance("application","body");
			$view->set_view("body",$body);
			$view->process();
			$html = $view->get_html();
			
			$temp_dir = MODEL_PATH::get_temp_directory().MISC_UTIL::get_guid()."/";
			
			FILE_UTIL::mkdir($temp_dir);

			preg_match_all("/(\\/lib\\/[^\"']+)/",$html,$matches);
			
			$matches = get_value($matches,1,array());
			
			$frontend_dir 	= MODEL_PATH::get_framework_directory()."frontend/";
			$web_dir 	= MODEL_PATH::get_application_directory()."web/";			
			$web_dirs	= FILE_UTIL::get_directory_listing($web_dir,"*",false,true);
			
			try {
				$zip_util = new ZIP_UTIL($zip_file);
			
				$replaces = array();

				foreach($web_dirs as $dir) {
					
					if(in_array($dir,array("unavailable")))
						continue;

					$zip_util->add_directory($web_dir.$dir,$dir);
					
					preg_match_all("/[\"'](\\/".$dir."\\/[^\"']+)/",$html,$web_matches);

					$web_matches = get_value($web_matches,1,array());

					foreach($web_matches as $web_match)
						$replaces[$web_match] = ltrim($web_match,"/");
				}

				foreach($matches as $match) {

					$trim_match = ltrim($match,"/");

					$file = $frontend_dir.$trim_match;
					$zip_util->add_file($file,$trim_match);

					$replaces[$match] = $trim_match;
				}
				
				preg_match("/(.*)".self::BODY_PLACEHOLDER."(.*)/is",$html,$body_matches);
				
				$zip_util->add_string("inc/header.php",get_value($body_matches,1));
				$zip_util->add_string("inc/footer.php",get_value($body_matches,2));
				
				$html = str_replace(array_keys($replaces),array_values($replaces),$html);
				
				$html = preg_replace("/\\/(public\\/)/","$1",$html);
				
				$site_content_dbq = new BASE_DBQ_SITE_CONTENT();
				$site_content_dbq->add_where("state","=",BASE_DBQ_SITE_CONTENT::STATE_ACTIVE);
				$site_content_dbq->add_where("type","=",BASE_DBQ_SITE_CONTENT::TYPE_HTML);
				$contents = $site_content_dbq->select_column("content","path");
				
				foreach($contents as $path=>$content) {
				
					$path = ltrim($path,"/");
					
					$path = $path ? $path : "index";
					
					$path = str_replace("/"," ",$path);
					
					$content = self::sanitize_content($content);
					
					$content = '<? include("inc/header.php") ?>'.chr(10).$content.chr(10).'<? include("inc/footer.php") ?>';
					
					$zip_util->add_string($path.".php",$content);
				}
				
				foreach($this->_public_dirs as $public_dir) {
				
					$content_dir = MODEL_PATH::get_public_directory().$public_dir."/";
					
					$zip_util->add_directory($content_dir,"public/".$public_dir);
				}
				
				$zip_util->close();
			
			} catch(Exception $e) {}
			
			FILE_UTIL::rmdir($temp_dir);
			
			return true;
		}
		
                /**
                 * sanitize content
                 * @param string $content
                 * @return string 
                 */
		public function sanitize_content($content) {
		
			$replaces["<!--@@CONTENT-TOP Do not remove comment CONTENT-TOP@@-->"] = "";
			$replaces["<!--@@CONTENT-BOTTOM Do not remove comment CONTENT-BOTTOM@@-->"] = "";
			
			return str_replace(array_keys($replaces),array_values($replaces),$content);			
		}
		
                /**
                 * Download
                 * @return boolean 
                 */
		public function download() {
				
			$temp_dir = MODEL_PATH::get_temp_directory();
			$zip_file = $temp_dir.MISC_UTIL::get_guid().".zip";
			
			$this->get_zip($zip_file);
			
			$filename = "site-content-".SYSTEM_MANAGER::get_system_host().".zip";
			
			HTTP_UTIL::stream_file($zip_file,$filename);
			
			return true;
		}					
	}