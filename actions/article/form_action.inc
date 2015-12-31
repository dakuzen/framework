<?
	class BASE_ACTION_ARTICLE_FORM extends BASE_ACTION_ARTICLE_BASE {

		function __construct() {
			parent::__construct(array(SECURITY_MANAGER::ROLE_BACKEND));
		}

		function process() {

			$form_id	= $this->post("fid");
			$form		= $this->post("form");

			$ff_form_cmodel = ($ff_form_cmodel=HMODEL_FF_FORM::get($form_id)) ? $ff_form_cmodel : CMODEL_FF_FORM::create();
			$ff_form_cmodel
				->populate($form)
				->set_configs((array)$this->post("configs"));

			try {

				$ff_form_cmodel->save();

				if(!$form_id) {
					APPLICATION::add_persistent_notify("Successfully added the form");
					$this->redirect($ff_form_cmodel->get_manage_url());
				}

				$this
					->data("fid",$ff_form_cmodel->get_form_id())
					->success();

			} catch(Exception $e) {
				$this->error($e);
			}

			if(!$this->has_success() && !$this->has_error())
				$this->error("There was a problem trying to save the form");
		}
	}


