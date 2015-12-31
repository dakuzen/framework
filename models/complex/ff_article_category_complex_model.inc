<?
	class BCMODEL_FF_ARTICLE_CATEGORY extends CMODEL {

		public function __construct() {
			return parent::__construct(DBO_FF_ARTICLE_CATEGORY::create());
		}

		public function is_state_active()	{ return $this->get_state()==BASE_DBQ_FF_ARTICLE_CATEGORY::STATE_ACTIVE; }
		public function is_state_deleted()	{ return $this->get_state()==BASE_DBQ_FF_ARTICLE_CATEGORY::STATE_DELETED; }
		public function get_state_name()	{ return value(BASE_DBQ_FF_ARTICLE_CATEGORY::get_state_list(),$this->get_state()); }
		public function get_manage_url()	{ return "/manage/ffarticlecategory/facid:".$this->get_article_category_id()."/"; }
		public function delete()			{ return $this->set_state(BASE_DBQ_FF_ARTICLE_CATEGORY::STATE_DELETED)->save(); }

		public function save() {
			
			$this->set_url(trim(preg_replace(array("/\s/","/\-{1,}/","/[^a-z0-9\-_\.~:\?#\[\]@\!$&'\(\)\*+,;=]/i"),array("-","-",""),$this->get_name()),"/"));

			if(is_numeric(self::get_article_category_id())) {
				if(!$this->dbo("ff_article_category")->update())
					throw new Exception("Error updating ".get_class());

			} else {
				if(!$this->get_state())
					$this->set_state(BASE_DBQ_FF_ARTICLE_CATEGORY::STATE_ACTIVE);

				if(!$this->dbo("ff_article_category")->insert())
					throw new Exception("Error inserting ".get_class());
			}

			return $this;
		}


		public function arry($arry=array()) {

			$arry = CMODEL_OBJECTER::create($this,
											array(	"id"=>"get_article_category_id",
													"get_state",
													"get_name",
													"get_url",
													))->arry();

			return $arry;
		}
	}