<?
	class BCMODEL_FF_ARTICLE_CATEGORY_CATEGORY extends CMODEL {
	
		public function __construct() {
			return parent::__construct(BASE_DBO_FF_ARTICLE_CATEGORY_CATEGORY::create());
		}
		
		public function get_manage_url()	{ return "/manage/ffarticlecategorycategory/faccid:".$this->get_article_category_category_id()."/"; }
		public function get_url()			{ return "/ffarticlecategorycategorys/ffarticlecategorycategory/faccid:".$this->get_article_category_category_id()."/"; }

		public function save() {
			//Used for objects that extend objects with save() or transactions
			//try {
			//
			//	DB::start_transaction();
			//	if(parent::save()) {
			
			if(is_numeric(self::get_article_category_category_id())) {
				if(!$this->dbo("ff_article_category_category")->update())
					throw new Exception("Error updating ".get_class());
				
			} else {
				if(!$this->dbo("ff_article_category_category")->insert())
					throw new Exception("Error inserting ".get_class());
			}

			//	}
			//	if(!DB::complete_transaction())
			//		throw new Exception("Error saving ".get_class());
			//} catch(Exception $e) {
			//	DB::complete_transaction();
			//	throw $e;
			//}
	
			return $this;
		}


		public function arry($arry=array()) {

			$arry = CMODEL_OBJECTER::create($this,
											array(	"id"=>"get_article_category_category_id",
													"get_article_category_id",
													"get_article_id",
													))->arry();

			return $arry;
		}		
	}