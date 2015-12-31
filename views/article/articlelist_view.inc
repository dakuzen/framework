<?
	class BASE_VIEW_ARTICLE_ARTICLELIST extends VIEW {

		protected $_article_cmodels = array();
		protected $_type 			= null;

		function __construct() {
			$this->set_route("article","articlelist")->type_blank();
		}

		function init() {

			if(SERVER_UTIL::is_post())
				$this->load();

			$this->set_var("articles",$this->_article_cmodels);
		}

		function set_type($type) { $this->_type = $type; return $this; }

		function load() {

			$page_index	= $this->request("page_index") ? $this->request("page_index") : 0;
			$page_limit	= $this->request("page_limit") ? $this->request("page_limit") : 15;

			$search		= (array)$this->request("search");
			$state 		= value($search,"state") ? value($search,"state") : "";
			$type 		= value($search,"type",$this->request("type"),$this->_type);

			$article_hmodel = HMODEL_FF_ARTICLE::create(false)
									->order("create_date","DESC");

			if($state)
				$article_hmodel->set_state($state);

			if($type)
				$article_hmodel->where("type","=",$type);

			if($keyword=strtolower(value($search,"keyword"))) {
				$article_hmodel
					->where("url","LIKE","%".$keyword."%","OR","searchgroup");
			}

			$record_count = $article_hmodel->count();

			$article_hmodel->set_limit($page_limit,$page_limit * $page_index,$record_count);

			$this->_article_cmodels = $article_hmodel->gets();

			$paging = APPLICATION::get_base_view_instance("components","paging")
							->populate($page_index,$page_limit,$record_count,'$("#article-list").trigger("load")');

			$this->set_view("paging",$paging);

			$this->set_var("search",$search);
		}
	}



