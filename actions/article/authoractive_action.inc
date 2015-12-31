<?
	class BASE_ACTION_ARTICLE_AUTHORACTIVE extends BASE_ACTION_ARTICLE_BASE {
	
		function process() {
		
			$article_id			= $this->post("aid");
			$article_author_id	= $this->post("author");
			
			$article_cmodel = HMODEL_FF_ARTICLE::get($article_id);

			if(!$article_cmodel)
				throw new Exception("Invalid article");				

			$article_author_cmodel = HMODEL_FF_ARTICLE_AUTHOR::get($article_author_id);

			if(!$article_author_cmodel)
				throw new Exception("Invalid author");	

			try {
				
				$article_cmodel
					->set_article_author_id($article_author_id)
					->save();
				
				$this
					->data("article_author",$article_author_cmodel->arry())
					->success();

			} catch(Exception $e) {
				$this->error($e);
			}
			
			if(!$this->has_success() && !$this->has_error())
				$this->error("There was a problem trying to save the author");
		}	
	}
	

