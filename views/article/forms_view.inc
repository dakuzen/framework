<?
	class BASE_VIEW_ARTICLE_FORMS extends BASE_VIEW_ARTICLE_BASE {
	
		function __construct() {
			parent::__construct("article","forms",array(),"ff-form-form");
		}
		
		function init() {
		
		}

		function load() { $this->get_view("ff_forms")->load(); }
	}