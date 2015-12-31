<?
	if($article_categories) {

		$table_data = array();

		foreach($article_categories as $article_category) {

			$actions = array();
			$actions[] = HTML_UTIL::link("javascript:;",BASE_MODEL_IMAGE_ICON::get_edit(),array("class"=>"edit","data-acid"=>$article_category->get_article_category_id()));
			$actions[] = HTML_UTIL::link("javascript:;",BASE_MODEL_IMAGE_ICON::get_delete(),array("class"=>"remove","data-acid"=>$article_category->get_article_category_id()));			
			
			$name = HTML_UTIL::span($article_category->get_name(),array("class"=>"name"));
			$checkbox = HTML_UTIL::checkbox("categories[]",$article_category->get_article_category_id(),in_array($article_category->get_article_category_id(),$selected),array("class"=>"mr10"),$name);
			$table_data[] = array($checkbox,array("data"=>implode(" ",$actions),"class"=>"wsnw w1"));
		}

		HTML_TABLE_UTIL::create()
			->set_data($table_data)
			->set_headings(array("",""))
			->set_id("article_category-table")
			->set_class("w100p")
			->render();
		
		$this->show_view("paging");
	} else
		echo "There are currently no categories";
