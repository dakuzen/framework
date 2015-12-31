<?
	class BASE_VIEW_ARTICLE_FORMFIELDLIST extends BASE_VIEW_ARTICLE_BASE {
		
		protected $_form_field_cmodels = array();

		function __construct() {
			$this->set_route("article","formfieldlist");
			$this->set_roles(array(SECURITY_MANAGER::ROLE_BACKEND));
			$this->type_blank();
		}
		
		function init() {

			if($form_id=$this->request("fid"))
				$this->load($form_id);
			
			$this->set_var("form_fields",$this->_form_field_cmodels);	
		}

		function load($form_id) {
				
			$this->_form_field_cmodels = HMODEL_FF_FORM_FIELD::create()
												->where("form_id","=",$form_id)
												->order("priority")
												->gets();
		}	
	}
	
	
				
				