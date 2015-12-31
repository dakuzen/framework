<?
	class BASE_ACTION_ARTICLE_CATEGORYREMOVE extends BASE_ACTION_ARTICLE_BASE {
	
		function process() {
		
			$article_id				= $this->post("aid");
			$article_category_id	= $this->post("acid");
			
			$article_cmodel = HMODEL_FF_ARTICLE::get($article_id);

			try {

				if(!$article_cmodel)
					throw new Exception("Invalid article");				
				
				$article_category_cmodel = HMODEL_FF_ARTICLE_CATEGORY::create()->get($article_category_id);

				if(!$article_category_cmodel)
					throw new Exception("Invalid article category");
				
				$article_category_cmodel
					->delete();
				
				DBQ_FF_ARTICLE_CATEGORY_CATEGORY::create()
					->where("article_category_id","=",$article_category_id)
					->delete();
				
				$this->success();

			} catch(Exception $e) {
				$this->error($e);
			}
			
			if(!$this->has_success() && !$this->has_error())
				$this->error("There was a problem trying to delete the category");
		}	
	}
	

