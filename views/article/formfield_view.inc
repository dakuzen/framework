<?
	class BASE_VIEW_ARTICLE_FORMFIELD extends BASE_VIEW_ARTICLE_BASE {
	
		protected $_form_field_cmodel = null;
		
		function __construct() {
			$this->set_route("article","formfield");
			$this->set_form("javascript:;",false,"field-form");
			$this->set_roles(array(SECURITY_MANAGER::ROLE_BACKEND));
			$this->type_popup();
			
			$this->_form_field_cmodel = CMODEL_FF_FORM_FIELD::create();
		}
		
		function init() {
			
			if(is_numeric($form_field_id=$this->get("ffid")))
				$this->load($form_field_id);

			if(is_numeric($form_id=$this->get("fid")))
				$this->_form_field_cmodel->set_form_id($form_id);
			
			$this->set_var("form_field", $this->_form_field_cmodel);
		}	

		function load($form_field_id) {

			$form_field_cmodel = HMODEL_FF_FORM_FIELD::create()->get($form_field_id);

			if($form_field_cmodel)
				$this->_form_field_cmodel = $form_field_cmodel;
		}		
	}