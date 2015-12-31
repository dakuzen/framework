<?
	class BASE_VIEW_ARTICLE_FORMS extends BASE_VIEW_ARTICLE_BASE {
	
		function __construct() {
			parent::__construct("article","forms",array(SECURITY_MANAGER::ROLE_BACKEND),"ff-form-form");
		}
		
		function init() {
		
		}

		function load() { $this->get_view("ff_forms")->load(); }
	}