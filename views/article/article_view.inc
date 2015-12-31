<?
	class BASE_VIEW_ARTICLE_ARTICLE extends BASE_VIEW_ARTICLE_BASE {

		protected $_article_cmodel 	= null;
		protected $_base_url 		= null;

		function __construct() {
			parent::__construct("article","article");
			$this->_article_cmodel = CMODEL_FF_ARTICLE::create()
										->set_type($this->get("type"));
		}

		function init() {

			$this->get_web_asset_manager()
				->add_redactor()
				->add_daterangepicker()
				->add_package("ff-articles/article.js");

			if($article_id=$this->get("aid"))
				$this->load($article_id);

			$templates = CMODEL_LISTING::create(HMODEL_FF_ARTICLE_TEMPLATE::create()
													->order("name")
													->gets())->column("get_name","get_article_template_id");

			$settings = BASE_DBQ_FF_ARTICLE_SETTING::create()
									->column("value","type");

			if($this->_article_cmodel->is_type_page())
				$this->_base_url = value($settings,CMODEL_FF_ARTICLE_SETTING::SETTING_SITE_PAGE_URL);

			elseif($this->_article_cmodel->is_type_post())
				$this->_base_url = value($settings,CMODEL_FF_ARTICLE_SETTING::SETTING_SITE_POST_URL);

			$this->set_var("base_url",$this->_base_url);
			$this->set_var("templates", $templates);
			$this->set_var("article", $this->_article_cmodel);
		}

		function load($article_id) {

			$article_cmodel = HMODEL_FF_ARTICLE::create(false)->get($article_id);

			if($article_cmodel)
				$this->_article_cmodel = $article_cmodel;
		}
	}