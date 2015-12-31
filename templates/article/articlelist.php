<?
	if($articles) {

		$table_data = array();

		foreach($articles as $article) {

			$actions = array();
			$actions[] = HTML_UTIL::link("javascript:;",BASE_MODEL_IMAGE_ICON::get_edit(),array("class"=>"article-update","data-aid"=>$article->get_article_id()));
			$actions[] = HTML_UTIL::link("javascript:;",BASE_MODEL_IMAGE_ICON::get_delete(),array("class"=>"article-remove","data-aid"=>$article->get_article_id()));
			//$actions[] = HTML_UTIL::link("javascript:;",BASE_MODEL_IMAGE_ICON::get_drag(),array("class"=>"order"));

			$image = $article->get_image()->exists("micro") ? $article->get_image()->img("micro",array("class"=>"h30 mr5")) : CMODEL_IMAGE::blank(array("class"=>"h30 w30 mr5"));

			$name = HTML_UTIL::link("javascript:;",$article->get_title(),array("class"=>"article-update","data-aid"=>$article->get_article_id()));
			$table_data[$article->get_article_id()] = array(
				$image.$name,
				$article->get_url(),
				$article->get_state_name(),
				array("data"=>implode(" ",$actions),"class"=>"wsnw w1"));
		}

		HTML_TABLE_UTIL::create()
			->set_data($table_data)
			->set_headings(array("Title","Url","Status",""))
			->set_id("article-table")
			->set_row_id_prefix("aids_")
			->add_class("w100p")
			->render();

		$this->show_view("paging");
	} else
		echo "There are currently no articles";
