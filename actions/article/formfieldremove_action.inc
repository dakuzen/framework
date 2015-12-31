<?
	class BASE_ACTION_ARTICLE_FORMFIELDREMOVE extends BASE_ACTION_ARTICLE_BASE {
	
		function __construct() {
			parent::__construct(array(SECURITY_MANAGER::ROLE_BACKEND));
		}
		
		function process() {
			
			if($form_field_id=$this->post("ffid")) {
				
				if($form_field_cmodel=HMODEL_FF_FORM_FIELD::get($form_field_id))
					$has_success = $form_field_cmodel->delete();
					
				$this->success($has_success);
			}
			
			if(!$this->has_success())
				$this->error("There was a problem trying to remove the form question");
		}	
	}