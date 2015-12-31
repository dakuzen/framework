<?
	trait BASE_VIEW_ARTICLE_API {

		function skip_group() { return true; }

		function posts($guid=null,$action=null) {

			if($this->is_get()) {

				$article_hmodel = HMODEL_FF_ARTICLE::create()
										->load_article_author()
										->load_article_categories()
										->set_type(BASE_DBQ_FF_ARTICLE::TYPE_POST)
										->order("publish_date","DESC");

				$url = $this->get("url");

				if($url)
					$article_hmodel->where("url","=",ltrim($url,"/"));

				$category_url = ltrim($this->get("category_url"),"/");

				if($category_url)
					$article_hmodel
						->join("ff_articles","ff_article_category_categories","article_id","article_id")
						->join("ff_article_category_categories","ff_article_categories","article_category_id","article_category_id")
						->where("ff_article_categories.state","=",DBQ_FF_ARTICLE_CATEGORY::STATE_ACTIVE)
						->where("ff_article_categories.url","=",$category_url);

				if($year=$this->get("year"))
					$article_hmodel->where("DATE_FORMAT(publish_date,'%Y')","=",$year);

				if($month=$this->get("month"))
					$article_hmodel->where("DATE_FORMAT(publish_date,'%m')","=",$month);


				if($author_url=$this->get("author_url")) {
					$article_hmodel
					->join("ff_articles","ff_article_authors","article_author_id","article_author_id")
					->where("ff_article_authors.url","=",$author_url);
				}


				$this->handler_paging($article_hmodel,10);
//DB::debug();
				$posts = array();
				foreach($article_hmodel->gets() as $article_cmodel) {
					$post = $article_cmodel->arry();
					$post["meta"] = $article_cmodel->get_meta(true);
					$posts[] = $post;
				}

				if($url)
					$this->data("post",value($posts,0));
				else
					$this->data("posts",$posts);
			}
		}

		function pages($guid=null,$action=null) {

			if($this->is_get()) {

				$article_hmodel = HMODEL_FF_ARTICLE::create()
										->load_article_templates()
										->set_type(BASE_DBQ_FF_ARTICLE::TYPE_PAGE);

				$url = $this->get("url");

				if($url)
					$article_hmodel->where("url","=",ltrim($url,"/"));

				$this->handler_paging($article_hmodel,5);

				$pages = array();
				foreach($article_hmodel->gets() as $article_cmodel) {

					$article = $article_cmodel->arry();
					$article["content"] = $article_cmodel->get_content(true);
					$article["meta"] = $article_cmodel->get_meta(true);

					$pages[] = $article;
				}

				if($url)
					$this->data("page",value($pages,0));
				else
					$this->data("pages",$pages);
			}
		}


		function all($guid=null, $action=null) {

			if($this->is_get()) {

				$article_hmodel = HMODEL_FF_ARTICLE::create()
										->load_article_templates()
										->load_article_author()
										->load_article_categories()
										->order("publish_date","DESC");
										;

				$url = $this->get("url");
				if($url)
					$article_hmodel->where("url","=",ltrim($url,"/"));



				if($search_term = $this->get("search_term"))
					$article_hmodel
						->where("title","like","%".$search_term."%","OR","search")
						->where("content","like","%".$search_term."%","OR","search")
						->where("excerpt","like","%".$search_term."%","OR","search")
						;


				$this->handler_paging($article_hmodel,50);

				$pages = array();
				foreach($article_hmodel->gets() as $article_cmodel) {

					$article = $article_cmodel->arry();
					$article["content"] = $article_cmodel->get_content(true);
					$article["meta"] = $article_cmodel->get_meta(true);

					$pages[] = $article;
				}

				if($url)
					$this->data("articles",value($pages,0));
				else
					$this->data("articles",$pages);
			}

		}

		function forms($id=null,$action=null) {

			if($this->is_post()) {

				$form_hmodel = HMODEL_FF_FORM::create()
									->load_form_fields();

				if(is_numeric($id))
					$form_hmodel->where("form_id","=",$id);
				else
					$form_hmodel->where("guid","=",$id);

				$form_cmodel = $form_hmodel->get();

				if(!$form_cmodel)
					throw new Exception("Failed to load form");

				$form_cmodel->submit($this->post("form"));

				$this->data("form",$form_cmodel->arry());
			}
		}

		function widgets() {

			if($this->is_get()) {

				$limit 		= $this->get("limit") ? $this->get("limit") : 5;
				$widgets 	= explode(",",$this->get("widget"));
				$format 	= in_array($this->get("format"),array("html","json")) ? $this->get("format") : "html";

				if(in_array("recent",$widgets)) {

					$archive_cmodels = HMODEL_FF_ARTICLE::create()
											->set_type(BASE_DBQ_FF_ARTICLE::TYPE_POST)
											->set_limit($limit)
											->order("publish_date","DESC")
											->gets();

					if($format=="html") {

						$content = "";
						foreach($archive_cmodels as $archive_cmodel)
							$content .= HTML_UTIL::li(HTML_UTIL::link($this->get("base_url").$archive_cmodel->get_url(),$archive_cmodel->get_title()));

						$content = HTML_UTIL::div(	HTML_UTIL::h4("Recent Posts").
													HTML_UTIL::ul($content),array("class"=>"recent-posts"));

						$this->data("recent",$content);
					} elseif($format=="json") {

						$content = [];
						foreach($archive_cmodels as $archive_cmodel)
							$content[] = array(	"title"=>$archive_cmodel->get_title(),
												"url"=>$archive_cmodel->get_url());

						$this->data("recent",$content);
					}
				}

				if(in_array("categories",$widgets)) {

					$category_cmodels = HMODEL_FF_ARTICLE_CATEGORY::create()
										->set_limit($limit)
										->gets();

					if($format=="html") {

						$content = "";
						foreach($category_cmodels as $category_cmodel)
							$content .= HTML_UTIL::li(HTML_UTIL::link($this->get("base_url").$category_cmodel->get_url(),$category_cmodel->get_name()));

						$content = HTML_UTIL::div(	HTML_UTIL::h4("Categories").
													HTML_UTIL::ul($content),array("class"=>"post-categories"));

						$this->data("categories",$content);

					} elseif($format=="json") {

						$categories = array();
						foreach($category_cmodels as $category_cmodel)
							$categories[] = $category_cmodel->arry();

						$this->data("categories",$categories);
					}
				}

				if(in_array("archives",$widgets)) {

					$archives = BASE_DBQ_FF_ARTICLE::create()
									->set_limit($limit)
									->where("type","=",BASE_DBQ_FF_ARTICLE::TYPE_POST)
									->group_by("DATE_FORMAT(publish_date,'%b-%y')")
									->select(array(	"DATE_FORMAT(publish_date,'%M %Y') AS title",
													"DATE_FORMAT(publish_date,'%Y') AS year",
													"DATE_FORMAT(publish_date,'%M') AS month",
													"CONCAT(DATE_FORMAT(publish_date,'%Y'),'/',DATE_FORMAT(publish_date,'%m')) AS url",
													));

					if($format=="html") {

						$content = "";
						foreach($archives as $archive)
							$content .= HTML_UTIL::li(HTML_UTIL::link($this->get("base_url").value($archive,"url"),value($archive,"title")));

						$content = HTML_UTIL::div(	HTML_UTIL::h4("Archives").
													HTML_UTIL::ul($content),array("class"=>"archived-posts"));

						$this->data("archives",$content);

					} elseif($format=="json")
						$this->data("archives",$archives);
				}
			}
		}
	}
