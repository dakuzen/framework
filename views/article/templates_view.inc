<?
	class BASE_VIEW_ARTICLE_TEMPLATES extends BASE_VIEW_ARTICLE_BASE {
		
		protected $_article_template_cmodels = array();

		function __construct() {
			$this->set_route("article","templates");
		}
		
		function init() {
			
			$this->load();
			
			$this->set_var("article_templates",$this->_article_template_cmodels);	
		}

		function load() {

			$search		= (array)$this->request("search");
			$state 		= value($search,"state") ? value($search,"state") : "";
			$type 		= value($search,"type",$this->request("type"));

			$article_template_hmodel = HMODEL_FF_ARTICLE_TEMPLATE::create()
											->order("name");

			if($state)
				$article_template_hmodel->set_state($state);
		
			$this->_article_template_cmodels = $article_template_hmodel->gets();

			$this->set_var("search",$search);
		}	
	}
	
	
				
				