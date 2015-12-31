<?
	class BHMODEL_FF_ARTICLE_TEMPLATE extends HMODEL {
	
		protected $_select_fields = array("ff_article_templates.*");
		
		public function __construct() {
			return $this->register_dbq(DBQ_FF_ARTICLE_TEMPLATE::create());
		}
		
		public function gets() {			
			$article_templates = $this->get_dbq()->select($this->_select_fields);

			$article_template_cmodels = array();			
			foreach($article_templates as $article_template) {
				$article_template_cmodel = CMODEL_FF_ARTICLE_TEMPLATE::create()->populate($article_template,true,false);
				$this->apply_properties($article_template_cmodel,$article_template);
				$article_template_cmodels[] = $article_template_cmodel;
			}
			
			if(!$article_template_cmodels)
				return array();
			
			return $article_template_cmodels;
		}
		
		public function where_state_active()			{ return $this->set_state(DBQ_FF_ARTICLE_TEMPLATE::STATE_ACTIVE); }
		public function set_article_template_ids($values)	{ return $this->where("ff_article_templates.article_template_id","IN",$values); }
		public function set_article_template_id($value)	{ return $this->where("ff_article_templates.article_template_id","=",$value); }
		public function set_state($value)		{ return $this->where("ff_article_templates.state","=",$value); }
		public function set_states($values)			{ return $this->where("ff_article_templates.state","IN",$values); }
		
		public function set_type($value)		{ return $this->where("ff_article_templates.type","=",$value); }

		public static function create($defaults=true) {
			return $defaults ? parent::create()->where_state_active() : parent::create();
		}		
	}
	