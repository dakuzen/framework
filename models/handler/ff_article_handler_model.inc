<?
	class BHMODEL_FF_ARTICLE extends HMODEL {
	
		protected $_select_fields = array("ff_articles.*");
		
		public function __construct() {
			return $this->register_dbq(DBQ_FF_ARTICLE::create());
		}
		
		public function gets() {			
			$articles = $this->get_dbq()->select($this->_select_fields);

			$article_cmodels = array();			
			foreach($articles as $article) {
				$article_cmodel = CMODEL_FF_ARTICLE::create()->populate($article,true,false);
				$this->apply_properties($article_cmodel,$article);
				$article_cmodels[] = $article_cmodel;
			}
			
			if(!$article_cmodels)
				return array();

			$this->map_one($article_cmodels,$this->handler("article_author_handler"),"article_author","article_author","article_author");
			$this->map_one($article_cmodels,$this->handler("article_template_handler"),"article_template","article_template","article_template");

			$this->map_many_many($article_cmodels,$this->handler("article_category_handler"),"article","ff_article_category_categories","article_id","ff_article_categories","article_category_id","article_category_id","article_category");
			
			return $article_cmodels;
		}
		
		public function where_state_active()			{ return $this->set_state(DBQ_FF_ARTICLE::STATE_ACTIVE); }
		public function set_article_ids($values)	{ return $this->where("ff_articles.article_id","IN",$values); }
		public function set_article_id($value)	{ return $this->where("ff_articles.article_id","=",$value); }
		public function set_article_author_id($value)		{ return $this->where("ff_articles.article_author_id","=",$value); }
		public function set_article_author_ids($values)			{ return $this->where("ff_articles.article_author_id","IN",$values); }
		public function set_article_template_id($value)		{ return $this->where("ff_articles.article_template_id","=",$value); }
		public function set_article_template_ids($values)			{ return $this->where("ff_articles.article_template_id","IN",$values); }
		public function set_article_category_category_id($value)		{ return $this->where("ff_articles.article_category_category_id","=",$value); }
		public function set_article_category_category_ids($values)			{ return $this->where("ff_articles.article_category_category_id","IN",$values); }
		public function set_guid($value)		{ return $this->where("ff_articles.guid","=",$value); }
		public function set_guids($values)			{ return $this->where("ff_articles.guid","IN",$values); }
		public function set_state($value)		{ return $this->where("ff_articles.state","=",$value); }
		public function set_states($values)			{ return $this->where("ff_articles.state","IN",$values); }
		
		public function set_type($value)		{ return $this->where("ff_articles.type","=",$value); }

		public static function create($defaults=true) {
			return $defaults ? parent::create()->where_state_active() : parent::create();
		}

		public function load_article_categories($article_category_hmodel=null) {
		 	return $this->handler("article_category_handler",$article_category_hmodel ? $article_category_hmodel : HMODEL_FF_ARTICLE_CATEGORY::create());
		}

		public function load_article_author($hmodel=null) {
			return $this->handler("article_author_handler",$hmodel ? $hmodel : HMODEL_FF_ARTICLE_AUTHOR::create());
		}

		public function load_article_templates($hmodel=null) {
			return $this->handler("article_template_handler",$hmodel ? $hmodel : HMODEL_FF_ARTICLE_TEMPLATE::create());
		}		
	}
	