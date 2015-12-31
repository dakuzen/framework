<?
	class BASE_MODEL_GOOGLE_MAP extends MODEL {
		
		static function get_map_js_url($google_map_key) {
			return "http://maps.google.com/maps?file=api&amp;v=2&amp;sensor=false&amp;key=".$google_map_key;
		}			
	}