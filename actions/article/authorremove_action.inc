<?
	class BASE_ACTION_ARTICLE_AUTHORREMOVE extends BASE_ACTION_ARTICLE_BASE {
	
		function process() {
		
			$article_id			= $this->post("aid");
			$article_author_id	= $this->post("aaid");
			
			$article_cmodel = HMODEL_FF_ARTICLE::get($article_id);

			try {

				if(!$article_cmodel)
					throw new Exception("Invalid article");				
				
				$article_author_cmodel = HMODEL_FF_ARTICLE_AUTHOR::create()->get($article_author_id);

				if(!$article_author_cmodel)
					throw new Exception("Invalid article author");
				
				$article_author_cmodel
					->delete();
					
				$this->success();

			} catch(Exception $e) {
				$this->error($e);
			}
			
			if(!$this->has_success() && !$this->has_error())
				$this->error("There was a problem trying to delete the author");
		}	
	}
	

