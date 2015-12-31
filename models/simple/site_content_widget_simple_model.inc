<?
	class BASE_MODEL_SITE_CONTENT_WIDGET {
		
		static function get_site_content_widget_id($tag) {
			$site_content_widget_dbq = new BASE_DBQ_SITE_CONTENT_WIDGET();
			$site_content_widget_dbq->add_where("tag","=",$tag);
			return $site_content_widget_dbq->select_one("site_content_widget_id");		
		}
		
		static function get_content($tag,$render=true) {		
			$site_content_widget_cmodel = self::get_site_content_widget($tag);
			return $site_content_widget_cmodel ? $site_content_widget_cmodel->get_content($render) : "";
		}
		
		static function get_contents($tags,$render=true,$handler="BASE_HMODEL_SITE_CONTENT_WIDGET") {
			$site_content_widget_hmodel = new $handler();
			$site_content_widget_hmodel->add_filter("tag","IN",$tags);
			
			$contents = array();
			foreach($site_content_widget_hmodel->get_site_content_widgets() as $site_content_widget_cmodel)
				$contents[$site_content_widget_cmodel->get_tag()] = $site_content_widget_cmodel->get_content($render);
			
			return $contents;
		}
		
		static function get_site_content_widget($tag,$handler_class="",$complex_class="") {
			
			$site_content_widget_hmodel = $handler_class ? new $handler_class() : new BASE_HMODEL_SITE_CONTENT_WIDGET();
			
			if($complex_class)
				$site_content_widget_hmodel->set_complex_class($complex_class);

			$site_content_widget_hmodel->set_tag($tag);
			return get_value($site_content_widget_hmodel->get_site_content_widgets(),0);		
		}
		
		static function get_site_content_widgets($tags,$handler_class="",$complex_class="") {
			
			$site_content_widget_hmodel = $handler_class ? new $handler_class() : new BASE_HMODEL_SITE_CONTENT_WIDGET();
			
			if($complex_class)
				$site_content_widget_hmodel->set_complex_class($complex_class);
			
			$site_content_widget_hmodel->set_tags($tags);
			return ARRAY_UTIL::get_indexed_objects($site_content_widget_hmodel->get_site_content_widgets(),"get_tag");
		}		
	}