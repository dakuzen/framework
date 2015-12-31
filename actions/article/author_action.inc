<?
	class BASE_ACTION_ARTICLE_AUTHOR extends BASE_ACTION_ARTICLE_BASE {

		function process() {

			$article_id			= $this->post("aid");
			$article_author_id	= $this->post("aaid");

			$article_cmodel = HMODEL_FF_ARTICLE::get($article_id);

			if(!$article_cmodel)
				throw new Exception("Invalid article");

			$article_author_cmodel = ($article_author_cmodel=HMODEL_FF_ARTICLE_AUTHOR::get($article_author_id)) ? $article_author_cmodel : CMODEL_FF_ARTICLE_AUTHOR::create();

			if($article_author_id) {
				$article_author_cmodel
					->set_name($this->post("edit-name"))
					->set_url($this->post("edit-url"))
					;
			}else {
				$article_author_cmodel
					->set_name($this->post("add-name"))
					->set_url($this->post("add-url"))
					;
			}

			try {

				$article_author_cmodel->save();

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


