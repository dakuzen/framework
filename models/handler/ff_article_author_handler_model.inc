<?
	class BHMODEL_FF_ARTICLE_AUTHOR extends HMODEL {
	
		protected $_select_fields = array("ff_article_authors.*");
		
		public function __construct() {
			return $this->register_dbq(DBQ_FF_ARTICLE_AUTHOR::create());
		}
		
		public function gets() {			
			$article_authors = $this->get_dbq()->select($this->_select_fields);

			$article_author_cmodels = array();			
			foreach($article_authors as $article_author) {
				$article_author_cmodel = CMODEL_FF_ARTICLE_AUTHOR::create()->populate($article_author,true,false);
				$this->apply_properties($article_author_cmodel,$article_author);
				$article_author_cmodels[] = $article_author_cmodel;
			}
			
			if(!$article_author_cmodels)
				return array();
			
			return $article_author_cmodels;
		}
		
		public function where_state_active()			{ return $this->set_state(DBQ_FF_ARTICLE_AUTHOR::STATE_ACTIVE); }
		public function set_article_author_ids($values)	{ return $this->where("ff_article_authors.article_author_id","IN",$values); }
		public function set_article_author_id($value)	{ return $this->where("ff_article_authors.article_author_id","=",$value); }
		public function set_state($value)		{ return $this->where("ff_article_authors.state","=",$value); }
		public function set_states($values)			{ return $this->where("ff_article_authors.state","IN",$values); }
		public function set_guid($value)		{ return $this->where("ff_article_authors.guid","=",$value); }
		public function set_guids($values)			{ return $this->where("ff_article_authors.guid","IN",$values); }
		

		public static function create($defaults=true) {
			return $defaults ? parent::create()->where_state_active() : parent::create();
		}		
	}
	