<?
	class BCMODEL_FF_ARTICLE extends CMODEL {

		public function __construct() {
			return parent::__construct(DBO_FF_ARTICLE::create())
						->register_json("meta")
						->register_json("configs");
		}

		public function is_state_active()		{ return $this->get_state()==DBQ_FF_ARTICLE::STATE_ACTIVE; }
		public function is_state_deleted()		{ return $this->get_state()==DBQ_FF_ARTICLE::STATE_DELETED; }
		public function is_type_page()			{ return $this->get_type()==DBQ_FF_ARTICLE::TYPE_PAGE; }
		public function is_type_post()			{ return $this->get_type()==DBQ_FF_ARTICLE::TYPE_POST; }
		public function get_state_name()		{ return value(DBQ_FF_ARTICLE::get_state_list(),$this->get_state()); }
		public function get_type_name()			{ return value(DBQ_FF_ARTICLE::get_type_list(),$this->get_type()); }
		public function get_config($name)		{ return value($this->get_configs(),$name); }
		public function set_config($name,$value){ return $this->set_configs(array_merge($this->get_configs(),array($name=>$value))); }
		public function get_image_time()		{ return $this->get_config("image_time"); }
		public function get_manage_url()		{ return "/manage/article/aid:".$this->get_article_id()."/"; }
		public function delete()				{ return $this->set_state(DBQ_FF_ARTICLE::STATE_DELETED)->save(); }

		public function set_article_categories($value) { return $this->data("article_categories",$value); }

		public function get_article_categories($hmodel=true) {
			if($hmodel && !$this->has_data("article_categories") && $this->get_article_id()) {
				$hmodel = is_a($hmodel,"HMODEL") ? $hmodel : HMODEL_FF_ARTICLE_CATEGORY::create(false);
				$this->data("article_categories",$hmodel
													->join("ff_article_categories","ff_article_category_categories","article_category_id")
													->where("ff_article_category_categories.article_id","=",$this->get_article_id())
													->gets());
			}
			return $this->data("article_categories");
		}

		public function get_article_author($hmodel=true) {
			if($hmodel && !$this->has_data("article_author") && $this->get_article_id()) {
				$hmodel = is_a($hmodel,"HMODEL") ? $hmodel : HMODEL_FF_ARTICLE_AUTHOR::create(false);
				$this->data("article_author",$hmodel->get($this->get_article_author_id()));
			}
			return $this->data("article_author");
		}

		public function set_article_author($cmodel) {
			return $this->data("article_author",$cmodel);
		}

		public function get_article_template($hmodel=true) {
			if($hmodel && !$this->has_data("article_template") && $this->get_article_id()) {
				$hmodel = is_a($hmodel,"HMODEL") ? $hmodel : HMODEL_FF_ARTICLE_TEMPLATE::create(false);
				$this->data("article_template",$hmodel->get($this->get_article_template_id()));
			}
			return $this->data("article_template");
		}

		public function set_article_template($cmodel) {
			return $this->data("article_template",$cmodel);
		}

		static function get_sanitized_path($path) {
			$path = URL_UTIL::remove_query($path);
			return "/".STRING_UTIL::remove_duplicates(trim($path,"/"),"/");
		}

		public function get_publish_timestamp() { return CMODEL_TIME::create($this->get_publish_date())->time(); }
		public function get_modify_timestamp() { return CMODEL_TIME::create($this->get_modify_date())->time(); }

		public function get_meta($format=false) {

			$meta = parent::get_meta();

			if($format) {

				foreach($meta as $guid=>&$item) {

					$tmp = array("content"=>value($item,"content"));

					if(in_array(value($item,"type"),array("custom-name")))
						$tmp["name"] = value($item,"name");

					elseif(in_array(value($item,"type"),array("description","keywords")))
						$tmp["name"] = value($item,"type");

					elseif(value($item,"type")=="custom-property")
						$tmp["property"] = value($item,"name");

					else
						$tmp["property"] = value($item,"type");

					$item = $tmp;
				}
			}

			return $meta;
		}

		public static function create_from_request() {

			$path = SERVER_UTIL::get_request_uri();

			return HMODEL_FF_ARTICLE::create()
						->where("state","=",BASE_DBQ_FF_ARTICLE::STATE_ACTIVE)
						->group(DB_GROUP::create()
									->where("url","REGEXP","'^/?".preg_quote(trim($path,"/"))."/?$'","OR","",false))
						->get();
		}

		public function commit_image($file) {

			$this->get_image(false)
					->purge($this->get_guid()."*")
						->process($file);

			return $this
					->set_config("image_time",time())
					->save();
		}

		public function get_image($cache=true) {

			if(!$cache || !$this->has_data("article_image"))
				$this->data("article_image",CMODEL_IMAGE::create("/pub/assets/article/",
																"/pub/assets/article/",
																$this->get_guid(),
																"jpg",
																array("actual"=>"actual","large"=>"800","small"=>"653","tiny"=>"200","micro"=>"s100"),
																$this->get_image_time()));
			return $this->data("article_image");
		}

		public function save() {

			if(!$this->get_url())
				throw new Exception("Invalid URL");

			$this->set_modify_date(DB::get_date_time());

			if($this->get_state()==DBQ_FF_ARTICLE::STATE_ACTIVE && !$this->get_publish_date())
				$this->set_publish_date(DB::get_date_time());

			$this->set_url(trim(preg_replace(array("/\s/","/\-{1,}/","/[^a-z0-9\-_\.~:\?#\[\]@\!$&'\(\)\*+,;=]/i"),array("-","-",""),$this->get_url()),"/"));

			$this->set_js( preg_replace("/\<\/?script\>/", "", $this->get_js()) );

			if(is_numeric(self::get_article_id())) {
				if(!$this->dbo("ff_article")->update())
					throw new Exception("Error updating ".get_class());

			} else {
				if(!$this->get_guid())
					$this->set_guid(MISC_UTIL::get_guid());

				if(!$this->get_state())
					$this->set_state(DBQ_FF_ARTICLE::STATE_ACTIVE);

				if(!$this->get_create_date())
					$this->set_create_date(DB::get_date_time());

				if(!$this->dbo("ff_article")->insert())
					throw new Exception("Error inserting ".get_class());
			}

			return $this;
		}

		public function get_content($render=false) {
			$content = parent::get_content();

			if($render) {
				if($article_template_cmodel=$this->get_article_template(false))
					$content = str_replace('{$body}',$content,$article_template_cmodel->get_content());

				preg_match_all('/{\$form_([^}]+)}/',$content,$matches);

				foreach((array)value($matches,1) as $tag) {

					$form_cmodel = HMODEL_FF_FORM::create()
										->where("tag","=",$tag)
										->load_form_fields()
										->get();

					if($form_cmodel) {
						$form = '<form article-form data-guid="'.$form_cmodel->get_guid().'" data-id="'.$form_cmodel->get_form_id().'"><table><tbody>';

						foreach($form_cmodel->get_form_fields() as $form_field_cmodel) {
							$form .= "<tr><td>".$form_field_cmodel->get_name()."</td><td>".$form_field_cmodel->render("form[".$form_field_cmodel->get_guid()."]")."</td></tr>";
						}

						$form .= '<tr><td></td><td><button type="button" class="submit">Submit</button></td></tr>';

						$form .= "</tbody></table></form>";

						$content = str_replace($form_cmodel->get_shortcode(),$form,$content);
					}
				}
			}

			return $content;
		}

		public function arry($arry=array()) {

			$arry = CMODEL_OBJECTER::create($this,
											array(	"id"=>"get_article_id",
													"get_article_author_id",
													"get_title",
													"get_publish_date",
													"get_publish_timestamp",
													"get_modify_date",
													"get_modify_timestamp",
													"get_views",
													"get_guid",
													"get_url",
													"get_content",
													"get_excerpt",
													"get_meta",
													"get_css",
													"get_head",
													"get_js",
													"get_state",
													"get_type",
													))->arry();

			if($image_cmodel=$this->get_image()) {
				if($image_cmodel->exists("actual")) {
					$arry["image"]["actual"] 	= $image_cmodel->url("actual");
					$arry["image"]["large"] 	= $image_cmodel->url("large");
					$arry["image"]["small"] 	= $image_cmodel->url("small");
				}
			}

			if($this->has_data("article_author") && $this->get_article_author())
				$arry["article_author"] = $this->get_article_author()->arry();

			if($this->has_data("article_template") && $this->get_article_template())
				$arry["article_template"] = $this->get_article_template()->arry();

			if($this->has_data("article_categories")) {

				$article_categories = array();
				foreach($this->get_article_categories() as $article_category_cmodel)
					$article_categories[] = $article_category_cmodel->arry();
				$arry["article_categories"] = $article_categories;
			}

			return $arry;
		}
	}