<?

	class BASE_DBQ_FF_ARTICLE_CATEGORY extends DBQ {

		const STATE_ACTIVE	= "A";
		const STATE_DELETED	= "D";

		static function get_state_list()	{ return array(self::STATE_ACTIVE=>"Active", self::STATE_DELETED=>"Deleted"); }

		public function __construct() {
			parent::__construct("ff_article_categories", "article_category_id");
		}
	}