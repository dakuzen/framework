<?
	class BASE_ACTION_ARTICLE_FORMFIELDARCHIVE extends BASE_ACTION_ARTICLE_FORM_BASE {
	
		function __construct() {
			parent::__construct(array(SECURITY_MANAGER::ROLE_MASTER));
		}
		
		function process() {
				
			if($form_field_id=$this->post("sfid")) {
				
				if($form_field_cmodel=HMODEL_FORM_FIELD::get($form_field_id))
					$has_success = $form_field_cmodel->archive();
					
				$this->success($has_success);
			}
			
			if(!$this->has_success())
				$this->error("There was a problem trying to remove the form question");
		}	
	}