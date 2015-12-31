<?
	class BASE_MODEL_SITE_CONTENT extends MODEL {
		
		public static function get_site_content_from_request($base_path="",$complex_class="CMODEL_SITE_CONTENT") {				
				
			$path = BASE_CMODEL_SITE_CONTENT::get_sanitized_path(SERVER_UTIL::get_request_uri());
			
			if($base_path)
				$path = str_replace($base_path,"",$path);

			return self::get_site_content($path);
		}

		public static function get_site_content($path,$complex_class="CMODEL_SITE_CONTENT") {				
		
			$exp = "^/?".preg_quote(trim($path,"/"))."/?$";
					
			return HMODEL_SITE_CONTENT::create()
					->where("state","=",BASE_DBQ_SITE_CONTENT::STATE_ACTIVE)
					->where("path","REGEXP",$exp,"AND","",false)
					->set_complex_class($complex_class)
					->get();
		}			
		
		public static function get_path_list() {
			$base_site_content_dbq = new BASE_DBQ_SITE_CONTENT();
			$base_site_content_dbq->add_where("state","=",BASE_DBQ_SITE_CONTENT::STATE_ACTIVE);
			return $base_site_content_dbq->select_column("title","path");
		}
		
		public static function get_sitemap_xml($complex_class="BASE_HMODEL_SITE_CONTENT") {
			
			$site_content_hmodel = new $complex_class();
			$site_content_hmodel->set_state(BASE_DBQ_SITE_CONTENT::STATE_ACTIVE);
			$site_content_cmodels = $site_content_hmodel->get_site_contents();
			
			$xml_writer_util = new XML_WRITER_UTIL();				
			$xml_writer_util->startElement("urlset",array("xmlns"=>"http://www.sitemaps.org/schemas/sitemap/0.9"));

			foreach($site_content_cmodels as $site_content_cmodel) {

				$modified_date = date("Y-m-d",$site_content_cmodel->get_modified_time());

				$xml_writer_util->startElement("url");

				$xml_writer_util->addElement("loc",$site_content_cmodel->get_url(true));
				$xml_writer_util->addElement("lastmod",$modified_date);
				$xml_writer_util->addElement("changefreq","monthly");
				$xml_writer_util->addElement("priority","1");

				$xml_writer_util->endElement();				
			}
			
			$xml_writer_util->endElement();
			return $xml_writer_util->getString();
		}
	}