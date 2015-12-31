<?
	class BASE_VIEW_ARTICLE_SETTINGS extends BASE_VIEW_ARTICLE_BASE {
	
		function __construct() {
			parent::__construct("article","settings")->type_blank();
		}
		
		function init() {
			
			$values = BASE_DBQ_FF_ARTICLE_SETTING::create()
									->column("value","type");

			$this->set_var("values",$values);
			$this->set_var("settings",CMODEL_FF_ARTICLE_SETTING::get_list());
		}	
	}