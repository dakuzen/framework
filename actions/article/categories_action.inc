<?
	class BASE_ACTION_ARTICLE_CATEGORIES extends BASE_ACTION_ARTICLE_BASE {
	
		function process() {
		
			$article_id				= $this->post("aid");
			$article_category_ids	= (array)$this->post("categories");
			
			$article_cmodel = HMODEL_FF_ARTICLE::get($article_id);

			if(!$article_cmodel)
				throw new Exception("Invalid article");				
			
			$existing 	= DBQ_FF_ARTICLE_CATEGORY_CATEGORY::create()
									->where("article_id","=",$article_cmodel->get_article_id())
									->column("article_category_id");

			$new 		= array_diff($article_category_ids,$existing);
			$remove 	= array_diff($existing,$article_category_ids);

			try {

				DB::start_transaction();

				if($remove)
					DBQ_FF_ARTICLE_CATEGORY_CATEGORY::create()
						->where("article_id","=",$article_cmodel->get_article_id())
						->where("article_category_id","IN",$remove)
						->delete();
			
				foreach($new as $article_category_id) 
					CMODEL_FF_ARTICLE_CATEGORY_CATEGORY::create()
						->set_article_id($article_cmodel->get_article_id())
						->set_article_category_id($article_category_id)
						->save();

				if(!DB::complete_transaction())
					throw new Exception("Error Processing Request", 1);
				
				$categories = array();
				foreach($article_cmodel->get_article_categories(true) as $article_category_cmodel)
					$categories[] = $article_category_cmodel->get_name();

				$this
					->data("categories",$categories)
					->success();

			} catch(Exception $e) {
				$this->error($e);
			}
			
			if(!$this->has_success() && !$this->has_error())
				$this->error("There was a problem trying to save the categories");
		}	
	}
	

