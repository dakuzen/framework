<?
	class BCMODEL_FF_ARTICLE_TEMPLATE extends CMODEL {
	
		public function __construct() {
			return parent::__construct(DBO_FF_ARTICLE_TEMPLATE::create());
		}
		
		public function is_state_active()	{ return $this->get_state()==DBQ_FF_ARTICLE_TEMPLATE::STATE_ACTIVE; }
		public function is_state_deleted()	{ return $this->get_state()==DBQ_FF_ARTICLE_TEMPLATE::STATE_DELETED; }
		public function is_type_default()	{ return $this->get_type()==DBQ_FF_ARTICLE_TEMPLATE::TYPE_DEFAULT; }
		public function get_state_name()	{ return value(DBQ_FF_ARTICLE_TEMPLATE::get_state_list(),$this->get_state()); }
		public function get_type_name()		{ return value(DBQ_FF_ARTICLE_TEMPLATE::get_type_list(),$this->get_type()); }
		public function get_manage_url()	{ return "/manage/articletemplate/atid:".$this->get_article_template_id()."/"; }
		public function get_url()			{ return "/articletemplates/articletemplate/atid:".$this->get_article_template_id()."/"; }
		public function delete()			{ return $this->set_state(DBQ_FF_ARTICLE_TEMPLATE::STATE_DELETED)->save(); }		

		public function save() {
			//Used for objects that extend objects with save() or transactions
			//try {
			//
			//	DB::start_transaction();
			//	if(parent::save()) {
			
			if(is_numeric(self::get_article_template_id())) {
				if(!$this->dbo("ff_article_template")->update())
					throw new Exception("Error updating ".get_class());
				
			} else {
				if(!$this->get_state())
					$this->set_state(DBQ_FF_ARTICLE_TEMPLATE::STATE_ACTIVE);
				
				if(!$this->dbo("ff_article_template")->insert())
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
											array(	"id"=>"get_article_template_id",
													"get_state",
													"get_content",
													"get_name",
													"get_type",
													))->arry();

			return $arry;
		}		
	}