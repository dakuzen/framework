<?

	class BASE_DBQ_FF_ARTICLE extends DBQ {

		const STATE_ACTIVE	= "A";
		const STATE_DELETED	= "D";
		const STATE_PENDING	= "E";
		const STATE_DRAFT	= "P";

		static function get_state_list()	{ return array(self::STATE_PENDING=>"Pending",self::STATE_DRAFT=>"Draft", self::STATE_ACTIVE=>"Published",  self::STATE_DELETED=>"Deleted"); }

		const TYPE_PAGE		= "page";
		const TYPE_POST		= "post";

		static function get_type_list()	{ return array(self::TYPE_PAGE=>"Page",self::TYPE_POST=>"Post"); }

		public function __construct() {
			parent::__construct("ff_articles", "article_id");
		}
	}