<?
	if($article_authors) {

		$table_data = array();

		foreach($article_authors as $article_author) {

			$actions = array();
			$actions[] = HTML_UTIL::link("javascript:;",BASE_MODEL_IMAGE_ICON::get_edit(),array("class"=>"edit","data-aaid"=>$article_author->get_article_author_id()));
			$actions[] = HTML_UTIL::link("javascript:;",BASE_MODEL_IMAGE_ICON::get_delete(),array("class"=>"remove","data-aaid"=>$article_author->get_article_author_id()));

			$image			= HTML_UTIL::span($article_author->get_image()->img("micro",array("class"=>"w20 h20 br10")),array("class"=>"preview"));
			$name 			= HTML_UTIL::span(HTML_UTIL::div($image,array("class"=>"dib br10 h20 w20 bcg mr5 vab author-image","data-aaid"=>$article_author->get_article_author_id())).$article_author->get_name(),array("class"=>"name"));
			$url 			= HTML_UTIL::span($article_author->get_url(),array("class"=>"url"));
			$checkbox 		= HTML_UTIL::radiobutton("author",$article_author->get_article_author_id(),$name." / ".$url,$article_author->get_article_author_id()==$selected_article_author_id,array("class"=>"mr10"));
			$table_data[] 	= array($checkbox,array("data"=>implode(" ",$actions),"class"=>"wsnw w1"));
		}

		HTML_TABLE_UTIL::create()
			->set_data($table_data)
			->set_headings(array("",""))
			->set_id("article_author-table")
			->set_class("w100p")
			->render();

		$this->show_view("paging");
	} else
		echo "There are currently no authors";
