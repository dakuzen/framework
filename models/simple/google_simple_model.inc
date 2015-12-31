<?
	class BASE_MODEL_GOOGLE extends MODEL {
		
		static function get_plus_meta($meta,$value) 	{ return '<meta itemprop="'.$meta.'" content="'.$value.'"></meta>'; }
		
		static function get_plus_metas($title="",$description="",$image="") { 
		
			$meta = array();
			
			if($title)
				$meta[] = self::get_plus_meta("title",$title);
			
			if($image)
				$meta[] = self::get_plus_meta("image",$image);
				
			if($description)
				$meta[] = self::get_plus_meta("description",$description);
			
			return $meta;
		}
	}