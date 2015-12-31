<?
	class BASE_MODEL_JS extends MODEL {
		
		static function get_script_includes($includes,$is_compressed=true) {
		
			$includes = is_array($includes) ? $includes : array($includes);
			
			$path = "/lib/js/".($is_compressed ? "com" : "uncom")."/";
			
			$scripts = "";
			
			foreach($includes as $include) 
				$scripts[] = '<script src="'.$path.ltrim($include,"/").'" type="text/javascript"></script>';	
			
			return implode("\n",$scripts);
		}			
	}