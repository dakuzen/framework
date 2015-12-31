<?
	class BASE_ACTION_ARTICLE_TEMPLATEREMOVE extends BASE_ACTION_ARTICLE_BASE {
	
		function process() {
		
			$article_template_id = $this->post("atid");
				
			$article_template_cmodel = HMODEL_FF_ARTICLE_TEMPLATE::get($article_template_id);

			try {

				if(!$article_template_cmodel)
					throw new Exception("Invalid template");				
						
				$article_template_cmodel->delete();
				
				$this->success();

			} catch(Exception $e) {
				$this->error($e);
			}
			
			if(!$this->has_success() && !$this->has_error())
				$this->error("There was a problem trying to delete the template");
		}	
	}
	

