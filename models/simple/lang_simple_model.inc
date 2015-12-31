<?
	class BASE_MODEL_LANG extends MODEL {
	
		const LANGUAGE_ENGLISH 	= "en-us";
		const LANGUAGE_DUTCH	= "nl-nl";
		const LANGUAGE_SPANISH	= "es";
		const LANGUAGE_ARABAIC	= "ar-ae";

	
		public static function get_language_list() 		{ return array(	self::LANGUAGE_ENGLISH=>"English",
																		self::LANGUAGE_DUTCH=>"Dutch",
																		self::LANGUAGE_SPANISH=>"Spanish",
																		self::LANGUAGE_ARABAIC=>"Arabic"); }
		
		public static function get_lang($lang=null)		{ return $lang ? $lang : SYSTEM_MANAGER::get_lang(); }
	
		public static function include_lang($lang=null,$debug=false) {
			
			$lang = self::get_lang($lang);
			
			$lang_include = $debug ? "debug" : $lang;
			
			$lang_file = MODEL_PATH::get_langs_directory().$lang."/".$lang_include.".inc";
			
			include_once($lang_file);
		}
		
		public static function get($name) {			
			return constant($name);		
		}
	}
	
	function lang($name) {
		return BASE_MODEL_LANG::get($name);
	}