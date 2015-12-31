<?
	class BCMODEL_FF_ARTICLE_AUTHOR extends CMODEL {

		public function __construct() {
			return parent::__construct(DBO_FF_ARTICLE_AUTHOR::create())
						->register_json("configs");
		}

		public function is_state_active()		{ return $this->get_state()==DBQ_FF_ARTICLE_AUTHOR::STATE_ACTIVE; }
		public function is_state_deleted()		{ return $this->get_state()==DBQ_FF_ARTICLE_AUTHOR::STATE_DELETED; }
		public function get_state_name()		{ return value(DBQ_FF_ARTICLE_AUTHOR::get_state_list(),$this->get_state()); }
		public function get_manage_url()		{ return "/manage/articleauthor/aaid:".$this->get_article_author_id()."/"; }
		//public function get_url()				{ return "/articleauthors/articleauthor/aaid:".$this->get_article_author_id()."/"; }
		public function delete()				{ return $this->set_state(DBQ_FF_ARTICLE_AUTHOR::STATE_DELETED)->save(); }
		public function get_config($name)		{ return value($this->get_configs(),$name); }
		public function set_config($name,$value){ return $this->set_configs(array_merge($this->get_configs(),array($name=>$value))); }
		public function get_image_time()		{ return $this->get_config("image_time"); }

		public function commit_image($file) {

			$this->get_image(false)
					->purge($this->get_guid()."*")
						->process($file);

			return $this
					->set_config("image_time",time())
					->save();
		}

		public function get_image($cache=true) {

			if(!$cache || !$this->has_data("author_image"))
				$this->data("author_image",CMODEL_IMAGE::create("/pub/assets/author/",
																"/pub/assets/author/",
																$this->get_guid(),
																"jpg",
																array("large"=>"800","small"=>"400","tiny"=>"200","micro"=>"s100"),
																$this->get_image_time()));
			return $this->data("author_image");
		}

		public function save() {

			if(is_numeric(self::get_article_author_id())) {
				if(!$this->dbo("ff_article_author")->update())
					throw new Exception("Error updating ".get_class());

			} else {
				if(!$this->get_guid())
					$this->set_guid(MISC_UTIL::get_guid());

				if(!$this->get_state())
					$this->set_state(DBQ_FF_ARTICLE_AUTHOR::STATE_ACTIVE);

				if(!$this->dbo("ff_article_author")->insert())
					throw new Exception("Error inserting ".get_class());
			}

			return $this;
		}


		public function arry($arry=array()) {

			$arry = CMODEL_OBJECTER::create($this,
											array(	"id"=>"get_article_author_id",
													"get_state",
													"get_name",
													"get_url",
													"get_guid",
													))->arry();

			if($image_cmodel=$this->get_image()) {
				$arry["image"]["large"] 	= $image_cmodel->url("large");
				$arry["image"]["small"] 	= $image_cmodel->url("small");
				$arry["image"]["tiny"] 		= $image_cmodel->url("tiny");
			}

			return $arry;
		}
	}