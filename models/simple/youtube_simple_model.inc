<?
	class BASE_MODEL_YOUTUBE extends MODEL {
		
		public function get_large_preview_url($id)	{ return "https://img.youtube.com/vi/".$id."/0.jpg"; }
		public function get_small_preview_url($id)	{ return "https://img.youtube.com/vi/".$id."/1.jpg"; }
		public function get_watch_url($id)		{ return "https://www.youtube.com/watch?v=".$id; }
		public function get_video_url($id)		{ return "https://www.youtube.com/v/".$id."?rel=1&fs=1"; }
		
		
		public function parse_id($url) {
			
			$url = trim($url);
			
			if(preg_match("/v=([-_a-z0-9]+)/i",$url,$matches))
				return get_value($matches,1);
				
			elseif(preg_match("/\/v\/([-_a-z0-9]+)/i",$url,$matches))
				return get_value($matches,1);

			elseif(preg_match("/\/([-_a-z0-9]+)$/i",$url,$matches))
				return get_value($matches,1);
				
			return null;
		}
	}