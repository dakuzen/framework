<?
	class BASE_VIEW_ARTICLE_JS extends BASE_VIEW_ARTICLE_BASE {
	
		function __construct() {
			parent::__construct("article","js");
		}
		
		function init() {			
			$this->set_var("content",BCMODEL_FF_ARTICLE_SETTING::get(BASE_CMODEL_FF_ARTICLE_SETTING::SETTING_SITE_JS));
		}
	}