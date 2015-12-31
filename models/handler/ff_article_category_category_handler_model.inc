<?
	class BHMODEL_FF_ARTICLE_CATEGORY_CATEGORY extends HMODEL {
	
		protected $_select_fields = array("ff_article_category_categories.*");
		
		public function __construct() {
			return $this->register_dbq(DBQ_FF_ARTICLE_CATEGORY_CATEGORY::create());
		}
		
		public function gets() {			
			$article_category_categories = $this->get_dbq()->select($this->_select_fields);

			$article_category_category_cmodels = array();			
			foreach($article_category_categories as $article_category_category) {
				$article_category_category_cmodel = CMODEL_FF_ARTICLE_CATEGORY_CATEGORY::create()->populate($article_category_category,true,false);
				$this->apply_properties($article_category_category_cmodel,$article_category_category);
				$article_category_category_cmodels[] = $article_category_category_cmodel;
			}
			
			if(!$article_category_category_cmodels)
				return array();
			
			return $article_category_category_cmodels;
		}
		
		public function set_article_category_category_ids($values)	{ return $this->where("ff_article_category_categories.article_category_category_id","IN",$values); }
		public function set_article_category_category_id($value)	{ return $this->where("ff_article_category_categories.article_category_category_id","=",$value); }
		public function set_article_category_id($value)		{ return $this->where("ff_article_category_categories.article_category_id","=",$value); }
		public function set_article_category_ids($values)			{ return $this->where("ff_article_category_categories.article_category_id","IN",$values); }
		public function set_article_id($value)		{ return $this->where("ff_article_category_categories.article_id","=",$value); }
		public function set_article_ids($values)			{ return $this->where("ff_article_category_categories.article_id","IN",$values); }
		

		public static function create($defaults=true) {
			return $defaults ? parent::create() : parent::create();
		}		
	}
	