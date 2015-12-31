<?
	class BHMODEL_FF_ARTICLE_CATEGORY extends HMODEL {
	
		protected $_select_fields = array("ff_article_categories.*");
		
		public function __construct() {
			return $this->register_dbq(DBQ_FF_ARTICLE_CATEGORY::create());
		}
		
		public function gets() {			
			$article_categories = $this->get_dbq()->select($this->_select_fields);

			$article_category_cmodels = array();			
			foreach($article_categories as $article_category) {
				$article_category_cmodel = CMODEL_FF_ARTICLE_CATEGORY::create()->populate($article_category,true,false);
				$this->apply_properties($article_category_cmodel,$article_category);
				$article_category_cmodels[] = $article_category_cmodel;
			}
			
			if(!$article_category_cmodels)
				return array();
			
			return $article_category_cmodels;
		}
		
		public function where_state_active()			{ return $this->set_state(DBQ_FF_ARTICLE_CATEGORY::STATE_ACTIVE); }
		public function set_article_category_ids($values)	{ return $this->where("ff_article_categories.article_category_id","IN",$values); }
		public function set_article_category_id($value)	{ return $this->where("ff_article_categories.article_category_id","=",$value); }
		public function set_state($value)		{ return $this->where("ff_article_categories.state","=",$value); }
		public function set_states($values)			{ return $this->where("ff_article_categories.state","IN",$values); }
		

		public static function create($defaults=true) {
			return $defaults ? parent::create()->where_state_active() : parent::create();
		}		
	}
	