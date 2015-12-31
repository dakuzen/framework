<?
	class BASE_ACTION_ARTICLE_CATEGORY extends BASE_ACTION_ARTICLE_BASE {
	
		function process() {
		
			$article_id				= $this->post("aid");
			$article_category_id	= $this->post("acid");
			
			$article_cmodel = HMODEL_FF_ARTICLE::get($article_id);

			if(!$article_cmodel)
				throw new Exception("Invalid article");				

			$article_category_cmodel = ($article_category_cmodel=HMODEL_FF_ARTICLE_CATEGORY::get($article_category_id)) ? $article_category_cmodel : CMODEL_FF_ARTICLE_CATEGORY::create();

			if($article_category_id)
				$article_category_cmodel->set_name($this->post("edit-name"));
			else
				$article_category_cmodel->set_name($this->post("add-name"));

			try {
				
				$article_category_cmodel->save();
				
				$this
					->data("article_category",$article_category_cmodel->arry())
					->success();

			} catch(Exception $e) {
				$this->error($e);
			}
			
			if(!$this->has_success() && !$this->has_error())
				$this->error("There was a problem trying to save the category");
		}	
	}
	

