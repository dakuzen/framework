<?
	class BASE_ACTION_ARTICLE_TEMPLATE extends BASE_ACTION_ARTICLE_BASE {
	
		function process() {
		
			$article_template_id	= $this->post("atid");
			
			$article_template_cmodel = ($article_template_cmodel=HMODEL_FF_ARTICLE_TEMPLATE::get($article_template_id)) ? $article_template_cmodel : CMODEL_FF_ARTICLE_TEMPLATE::create();

			$article_template_cmodel
				->populate($this->post("form"));

			try {
				
				$article_template_cmodel->save();

				if(!$article_template_id) {
					APPLICATION::add_persistent_notify("Successfully added the template"); 
					$this->redirect("/manage/articles/view:template/atid:".$article_template_cmodel->get_article_template_id());
				}

				$this
					->data("article_template",$article_template_cmodel->arry())
					->success();

			} catch(Exception $e) {
				$this->error($e);
			}
			
			if(!$this->has_success() && !$this->has_error())
				$this->error("There was a problem trying to save the template");
		}	
	}
	

