<?
	class BASE_DBQ_FF_CONTENT extends DBQ {

		const STATE_ACTIVE 	= "A";
		const STATE_DELETE 	= "D";

		const TYPE_HTML 	= "H";
		const TYPE_LINK 	= "L";
		const TYPE_META 	= "M";
		const TYPE_REDIRECT 	= "R";

		const BODY_WRAP_FULL 	= "B";
		const BODY_WRAP_POPUP 	= "P";
		const BODY_WRAP_NONE 	= "N";

		const SOURCE_CUSTOM 	= "C";
		const SOURCE_DEFAULT 	= "D";

		static function get_state_list()	{ return array(self::STATE_ACTIVE=>"Active",self::STATE_DELETE=>"Delete"); }

		static function get_type_list() 	{ return array(	self::TYPE_HTML=>"HTML Page - Includes an HTML template for this content page",
															self::TYPE_LINK=>"Link - Only used for automatic menu implementations",
															self::TYPE_META=>"Meta - Allows for the addition of Meta Data on App-driven pages",
															self::TYPE_REDIRECT=>"Redirect - Performs a HTTP redirect"); }

		static function get_source_list() 	{ return array(self::SOURCE_CUSTOM=>"Custom",self::SOURCE_DEFAULT=>"Default"); }

		static function get_body_wrap_list() 	{ return array(self::BODY_WRAP_FULL=>"Body",self::BODY_WRAP_POPUP=>"Popup",self::BODY_WRAP_NONE=>"None"); }

		public function __construct() {
			parent::__construct("ff_contents", "content_id");
		}
	}