<?
	class BASE_ACTION_ARTICLE_ARTICLEREMOVE extends BASE_ACTION_ARTICLE_BASE {
	
		function process() {
		
			$article_id		= $this->post("aid");
				
			$article_cmodel = HMODEL_FF_ARTICLE::get($article_id);

			try {

				if(!$article_cmodel)
					throw new Exception("Invalid article");				
						
				$article_cmodel->delete();
				
				$this->success();

			} catch(Exception $e) {
				$this->error($e);
			}
			
			if(!$this->has_success() && !$this->has_error())
				$this->error("There was a problem trying to delete the article");
		}	
	}
	

