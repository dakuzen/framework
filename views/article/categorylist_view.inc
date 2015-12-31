<?
	class BASE_VIEW_ARTICLE_CATEGORYLIST extends BASE_VIEW_ARTICLE_BASE {
		
		protected $_article_category_cmodels 	= array();
		protected $_selected					= array();

		function __construct() {
			$this->set_route("article","categorylist")->type_blank();
		}
		
		function init() {

			if(SERVER_UTIL::is_post())
				$this->load();
			
			$this->set_var("article_categories",$this->_article_category_cmodels);	
			$this->set_var("selected",$this->_selected);	
		}

		function load() {

			$search		= (array)$this->request("search");
			$state 		= value($search,"state") ? value($search,"state") : "";
			
			$article_hmodel = HMODEL_FF_ARTICLE_CATEGORY::create()
									->order("name");

			if($state)
				$article_hmodel->set_state($state);

			$this->_selected = BASE_DBQ_FF_ARTICLE_CATEGORY_CATEGORY::create()
									->where("article_id","=",$this->post("aid"))
									->column("article_category_id");
			
			$this->_article_category_cmodels = $article_hmodel->gets();

			$this->set_var("search",$search);
		}	
	}
	
	
				
				