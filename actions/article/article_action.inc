<?
	class BASE_ACTION_ARTICLE_ARTICLE extends BASE_ACTION_ARTICLE_BASE {

		function process() {

			$article_id		= $this->post("aid");

			$article_cmodel = ($article_cmodel=HMODEL_FF_ARTICLE::get($article_id)) ? $article_cmodel : CMODEL_FF_ARTICLE::create();

			$article_cmodel->populate($this->post("form"));

			if($publish_date = value($this->post("form"), "publish_date"))
				$article_cmodel->set_publish_date(DB::get_date_time(strtotime($publish_date)));

			try {

				$article_cmodel
					->set_meta((array)$this->post("meta"))
					->save();

				if(!$article_id) {
					APPLICATION::add_persistent_notify("Successfully added the article");
					$this->redirect("/manage/articles/view:article/aid:".$article_cmodel->get_article_id());
				}

				$this
					->data("article",$article_cmodel->arry())
					->success();

			} catch(Exception $e) {
				$this->error($e);
			}

			if(!$this->has_success() && !$this->has_error())
				$this->error("There was a problem trying to save the article");
		}
	}

