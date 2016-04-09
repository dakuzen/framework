<?
	class BASE_VIEW_ARTICLE_FIELDS extends BASE_VIEW_ARTICLE_BASE {
	
		function __construct() {
			$this->set_route("article","fields");
			$this->set_form("javascript:;",false,"survey-field-form");
			$this->type_blank();
		}
		
		function init() {
			$form_id = $this->get("sid");		
			
			$this->set_var("form_id",$form_id);
			$this->set_var("search",array());
		}

		function load($form_id) { $this->get_view("form_fields")->load($form_id); }
	}