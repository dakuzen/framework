<?
	class BASE_ACTION_ARTICLE_FORMREMOVE extends BASE_ACTION_ARTICLE_BASE {

		function __construct() {
			parent::__construct(array(SECURITY_MANAGER::ROLE_BACKEND));
		}

		function process() {

			if($ff_form_id=$this->post("fid")) {

				if($ff_form_cmodel=HMODEL_FF_FORM::get($ff_form_id))
					$has_success = $ff_form_cmodel->delete();

				$this->success($has_success);
			}

			if(!$this->has_success())
				$this->error("There was a problem trying to remove the form");
		}
	}