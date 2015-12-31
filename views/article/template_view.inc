<?
	class BASE_VIEW_ARTICLE_TEMPLATE extends BASE_VIEW_ARTICLE_BASE {
	
		protected $_article_template_cmodel = null;
		
		function __construct() {
			parent::__construct("article","template");
			$this->_article_template_cmodel = CMODEL_FF_ARTICLE_TEMPLATE::create()
													->set_content('{$body}');
		}
		
		function init() {
			
			if($article_template_id=$this->get("atid"))
				$this->load($article_template_id);

			$this->set_var("article_template",$this->_article_template_cmodel);		
		}	

		function load($article_template_id) {

			$article_template_cmodel = HMODEL_FF_ARTICLE_TEMPLATE::create(false)->get($article_template_id);

			if($article_template_cmodel)
				$this->_article_template_cmodel = $article_template_cmodel;
		}		
	}