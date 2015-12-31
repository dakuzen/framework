<?
	class BASE_VIEW_ARTICLE_AUTHORLIST extends VIEW {
		
		protected $_article_author_cmodels 		= array();
		protected $_selected_article_author_id	= null;

		function __construct() {
			$this->set_route("article","authorlist")->type_blank();
		}
		
		function init() {

			if(SERVER_UTIL::is_post())
				$this->load();
			
			$this->set_var("article_authors",$this->_article_author_cmodels);	
			$this->set_var("selected_article_author_id",$this->_selected_article_author_id);	
		}

		function load() {

			$search		= (array)$this->request("search");
			$state 		= value($search,"state") ? value($search,"state") : "";
			
			$article_hmodel = HMODEL_FF_ARTICLE_AUTHOR::create()
									->order("name");

			if($state)
				$article_hmodel->set_state($state);

			$this->_article_author_cmodels = $article_hmodel->gets();

			$article_cmodel = HMODEL_FF_ARTICLE::create()->get($this->post("aid"));

			if($article_cmodel)
				$this->_selected_article_author_id = $article_cmodel->get_article_author_id();
			
			$this->set_var("search",$search);
		}	
	}
	
	
				
				